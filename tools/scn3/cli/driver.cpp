#include "scn3/backend/compiler.hpp"
#include "scn3/bytecode/format.hpp"
#include "scn3/bytecode/program.hpp"
#include "scn3/cli/diagnostics.hpp"
#include "scn3/cli/driver.hpp"
#include "scn3/common/util.hpp"
#include "scn3/decompiler/analysis.hpp"
#include "scn3/decompiler/discovery.hpp"
#include "scn3/decompiler/export.hpp"
#include "scn3/frontend/ast.hpp"
#include "scn3/frontend/source.hpp"
#include "scn3/native/motion.hpp"
#include "scn3/sh4/native.hpp"

namespace shenmue::scenario {

std::pair<uint32_t, uint32_t> parsePatchImmArgument(const std::string &text) {
    const size_t eq = text.find('=');
    if (eq == std::string::npos || eq == 0 || eq + 1 >= text.size()) {
        throw std::runtime_error("--patch-imm expects <offset>=<value>");
    }
    return {parseInteger(text.substr(0, eq)), parseInteger(text.substr(eq + 1))};
}

bool isCSourcePath(const std::string &path) {
    std::string ext = std::filesystem::path(path).extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
    return ext == ".c" || ext == ".scn3c";
}

Options parseArgs(int argc, char **argv) {
    Options options;
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "-I") {
            if (i + 1 >= argc)
                throw std::runtime_error("-I requires an include directory");
            options.includeDirectories.emplace_back(argv[++i]);
        } else if (arg.starts_with("-I") && arg.size() > 2) {
            options.includeDirectories.emplace_back(arg.substr(2));
        } else if (arg == "--write-c-header") {
            if (i + 1 >= argc)
                throw std::runtime_error("--write-c-header requires a path");
            options.writeCHeaderPath = argv[++i];
        } else if (arg == "--extract-scn3") {
            if (i + 1 >= argc)
                throw std::runtime_error("--extract-scn3 requires an output path");
            options.extractScn3Path = argv[++i];
        } else if (arg == "--decompile") {
            options.decompile = true;
        } else if (arg == "--explicit-decompile") {
            options.explicitDecompile = true;
        } else if (arg == "--verbose-header") {
            options.verboseHeader = true;
        } else if (arg == "--show-bytes") {
            options.showBytes = true;
        } else if (arg == "--audit-c") {
            options.auditC = true;
        } else if (arg == "--audit") {
            options.audit = true;
        } else if (arg == "--decode-uncovered") {
            options.decodeUncovered = true;
        } else if (arg == "--dump-ir") {
            if (i + 1 >= argc) {
                throw std::runtime_error("--dump-ir requires a function offset");
            }
            options.dumpIr = true;
            options.dumpIrStart = parseInteger(argv[++i]);
        } else if (arg == "--dump-layout" || arg == "--dump-lowlevel-module") {
            options.dumpLayout = true;
        } else if (arg == "--roundtrip-check") {
            options.roundtripCheck = true;
        } else if (arg == "--export-c" || arg == "--export-c11") {
            if (i + 1 >= argc) {
                throw std::runtime_error("--export-c requires an output path");
            }
            options.exportCPath = argv[++i];
            options.strictCSource = arg == "--export-c11";
        } else if (arg == "--names") {
            if (i + 1 >= argc) {
                throw std::runtime_error("--names requires a names file");
            }
            options.namesPath = argv[++i];
        } else if (arg == "--compile-c") {
            if (i + 1 >= argc) {
                throw std::runtime_error("--compile-c requires an input source path");
            }
            options.compileCPath = argv[++i];
        } else if (arg == "--disassemble") {
            if (i + 1 >= argc)
                throw std::runtime_error("--disassemble requires an output path");
            options.exportAssemblyPath = argv[++i];
        } else if (arg == "--assemble") {
            if (i + 1 >= argc)
                throw std::runtime_error("--assemble requires an input assembly path");
            options.assemblePath = argv[++i];
        } else if (arg == "--only-function") {
            if (i + 1 >= argc) {
                throw std::runtime_error("--only-function requires a function name");
            }
            options.onlyFunction = argv[++i];
        } else if (arg == "--dump-function-bytes") {
            options.dumpFunctionBytes = true;
        } else if (arg == "--dump-data-symbol") {
            if (i + 1 >= argc) {
                throw std::runtime_error("--dump-data-symbol requires a data symbol name");
            }
            options.dumpDataSymbol = argv[++i];
        } else if (arg == "--emit") {
            if (i + 1 >= argc) {
                throw std::runtime_error("--emit requires an output form (vm, sh4) or an output path");
            }
            const std::string value = argv[++i];
            if (value == "vm" || value == "sh4")
                options.emitFormat = value;
            else
                options.emitPath = value;
        } else if (arg == "--target") {
            if (i + 1 >= argc || (std::string(argv[i + 1]) != "pc" && std::string(argv[i + 1]) != "dc"))
                throw std::runtime_error("--target requires pc or dc");
            options.target = argv[++i];
        } else if (arg == "-o") {
            if (i + 1 >= argc)
                throw std::runtime_error("-o requires an output path");
            options.outputPath = argv[++i];
        } else if (arg == "--patch-imm") {
            if (i + 1 >= argc) {
                throw std::runtime_error("--patch-imm requires <offset>=<value>");
            }
            options.patchImm = parsePatchImmArgument(argv[++i]);
        } else if (arg == "--start") {
            if (i + 1 >= argc) {
                throw std::runtime_error("--start requires a value");
            }
            options.startOverride = parseInteger(argv[++i]);
        } else if (!arg.empty() && arg[0] == '-') {
            throw std::runtime_error("Unknown option: " + arg);
        } else if (isCSourcePath(arg) && !options.compileCPath) {
            options.compileCPath = arg;
        } else if (options.path.empty()) {
            options.path = arg;
        } else {
            throw std::runtime_error("Unexpected positional argument: " + arg);
        }
    }
    if (options.path.empty() && !options.compileCPath.has_value() && !options.assemblePath.has_value() && !options.writeCHeaderPath) {
        throw std::runtime_error("Usage: scn3cc <scenario.c> [--target pc|dc] [--emit vm|sh4] -o <output.scn>\n"
                                 "       scn3cc <file.scn> --export-c <output.scn3c> | --disassemble <output.scn3asm>\n"
                                 "       scn3cc --compile-c <input.scn3c> --emit <output.scn>\n"
                                 "       scn3cc --assemble <input.scn3asm> --emit <output.scn>\n"
                                 "       scn3cc <file.scn> [--explicit-decompile|--audit|--roundtrip-check|--dump-layout]\n"
                                 "       scn3cc <MAPINFO.BIN> --extract-scn3 <output.scn>");
    }
    if (options.emitFormat && !options.outputPath)
        throw std::runtime_error("--emit vm|sh4 requires -o <output>");
    if (options.outputPath && (options.emitPath || options.exportCPath || options.exportAssemblyPath || options.extractScn3Path))
        throw std::runtime_error("-o writes a compiled or converted module and does not combine with other outputs");
    if (options.emitFormat == "sh4" && options.target != "dc")
        throw std::runtime_error("Native SH-4 output is the Dreamcast form: use --target dc");
    if (options.extractScn3Path &&
        (options.path.empty() || options.emitPath || options.compileCPath || options.assemblePath || options.exportCPath || options.exportAssemblyPath ||
            options.audit || options.decompile || options.explicitDecompile || options.roundtripCheck))
        throw std::runtime_error("--extract-scn3 takes one MAPINFO input and an output path without other processing modes");
    return options;
}

std::vector<uint8_t> convertModule(const Options &options, std::vector<uint8_t> vm) {
    return options.emitFormat == "sh4" ? translateScenarioToSh4(vm) : vm;
}

bool hasScnExtension(const std::filesystem::path &path) {
    std::string ext = path.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
    return ext == ".scn";
}

std::vector<std::filesystem::path> collectScnFiles(const std::filesystem::path &dir) {
    std::vector<std::filesystem::path> files;
    for (const auto &entry : std::filesystem::recursive_directory_iterator(dir)) {
        if (entry.is_regular_file() && hasScnExtension(entry.path())) {
            files.push_back(entry.path());
        }
    }
    std::sort(files.begin(), files.end(), [](const auto &lhs, const auto &rhs) { return lhs.generic_string() < rhs.generic_string(); });
    return files;
}

void renderExplicitDirectory(const Options &options) {
    if (options.startOverride.has_value()) {
        throw std::runtime_error("--start is only supported for single-file input");
    }
    if (options.decompile) {
        throw std::runtime_error("--decompile is only supported for single-file input; directory input uses explicit decompile");
    }
    if (options.audit) {
        throw std::runtime_error("--audit is only supported for single-file input");
    }
    if (options.dumpIr || options.dumpLayout || options.roundtripCheck || options.emitPath.has_value() || options.outputPath.has_value() ||
        options.exportCPath.has_value() || options.compileCPath.has_value() || options.patchImm.has_value()) {
        throw std::runtime_error(
            "--dump-ir, --dump-layout, --roundtrip-check, --export-c, --compile-c, --emit, and --patch-imm are only supported for single-file input");
    }
    if (options.verboseHeader || options.showBytes) {
        throw std::runtime_error("--verbose-header and --show-bytes are only supported for single-file input");
    }

    const std::filesystem::path root(options.path);
    const std::vector<std::filesystem::path> files = collectScnFiles(root);
    if (files.empty()) {
        throw std::runtime_error("No .scn files found in directory: " + options.path);
    }

    for (size_t i = 0; i < files.size(); ++i) {
        const auto &file = files[i];
        if (i != 0) {
            std::cout << "\n";
        }
        std::cout << "// file: " << std::filesystem::relative(file, root).generic_string() << "\n";
        std::vector<uint8_t> blob = readFile(file.string());
        if (isNativeScenario(blob))
            blob = liftScenarioFromSh4(blob);
        Program program = buildProgram(blob, DecodeOptions{.startOverride = options.startOverride, .decodeUncovered = options.decodeUncovered});
        loadScriptMotionNames(program, file);
        std::cout << insertMotionIncludes(renderExplicitSource(program, options.abiProfile));
    }
}

int runCompiler(int argc, char **argv) {
    try {
        const Options options = parseArgs(argc, argv);
        for (const std::filesystem::path &directory : options.includeDirectories)
            addScenarioCIncludeDirectory(directory);
        if (options.writeCHeaderPath) {
            writeScenarioCHeader(*options.writeCHeaderPath);
            return 0;
        }
        if (options.assemblePath.has_value()) {
            if ((!options.emitPath && !options.outputPath) || !options.path.empty() || options.compileCPath)
                throw std::runtime_error("--assemble requires an output path and does not take another input");
            const Scn3Module module = assembleScenarioText(*options.assemblePath);
            writeFile(options.outputPath.value_or(options.emitPath.value_or("")), convertModule(options, emitScn3Module(module)));
            return 0;
        }
        if (options.compileCPath.has_value()) {
            if (options.dumpDataSymbol.has_value()) {
                SourceUnit unit = parseScn3SourceUnit(*options.compileCPath);
                dumpSourceDataSymbol(unit, *options.dumpDataSymbol);
                return 0;
            }
            if (options.dumpFunctionBytes) {
                if (!options.onlyFunction.has_value()) {
                    throw std::runtime_error("--dump-function-bytes requires --only-function");
                }
                SourceUnit unit = parseScn3SourceUnit(*options.compileCPath);
                const std::vector<uint8_t> bytes = compileSourceFunctionBytes(unit, *options.onlyFunction);
                std::cout << "function=" << *options.onlyFunction << "\n";
                std::cout << "bytes=" << bytesToHexLine(bytes) << "\n";
                std::cout << "size=" << bytes.size() << "\n";
                return 0;
            }
            if (options.outputPath) {
                if (!options.path.empty())
                    throw std::runtime_error("A C source does not take an original SCN input");
                writeFile(*options.outputPath, convertModule(options, emitScn3Module(compileScn3Source(*options.compileCPath))));
                return 0;
            }
            if (!options.emitPath.has_value()) {
                throw std::runtime_error("--compile-c requires --emit <output.scn> or -o <output>");
            }
            if (!options.path.empty()) {
                throw std::runtime_error("--compile-c does not take an original SCN input");
            }
            Scn3Module module = compileScn3Source(*options.compileCPath);
            const std::vector<uint8_t> emitted = emitScn3Module(module);
            writeFile(*options.emitPath, emitted);
            return 0;
        }
        if (std::filesystem::is_directory(options.path)) {
            if (options.auditC)
                throw std::runtime_error("--audit-c requires one SCN file");
            renderExplicitDirectory(options);
            return 0;
        }
        std::vector<uint8_t> blob = readFile(options.path);
        if (!options.extractScn3Path && isNativeScenario(blob))
            blob = liftScenarioFromSh4(blob);
        if (options.outputPath) {
            writeFile(*options.outputPath, convertModule(options, blob));
            return 0;
        }
        if (options.extractScn3Path.has_value()) {
            std::uint32_t offset = 0;
            while (offset + 8U <= blob.size()) {
                const std::uint32_t tag = readU32(blob, offset);
                const std::uint32_t size = readU32(blob, offset + 4U);
                if (size < 8U || size > blob.size() - offset)
                    break;
                if (tag == SHEN_NAME(SCN3)) {
                    const std::vector<uint8_t> chunk(blob.begin() + offset, blob.begin() + offset + size);
                    const ScenarioHeader header = parseHeader(chunk);
                    writeFile(*options.extractScn3Path, chunk);
                    std::cout << "SCN3 offset=" << hexUnsigned(offset) << " size=" << size
                              << " encoding=" << ((static_cast<std::uint32_t>(header.flags) & SCF_NativeCodeMask) ? "SH-4" : "VM") << "\n";
                    return 0;
                }
                offset += size;
            }
            throw std::runtime_error("No bounded SCN3 chunk in MAPINFO input");
        }
        Program program = buildProgram(blob, DecodeOptions{.startOverride = options.startOverride, .decodeUncovered = options.decodeUncovered});
        loadScriptMotionNames(program, options.path);
        if (options.verboseHeader) {
            printHeader(program.header);
            std::cout << "\n";
        }
        if (options.patchImm.has_value() && !options.emitPath.has_value()) {
            throw std::runtime_error("--patch-imm requires --emit");
        }
        if (options.exportAssemblyPath.has_value()) {
            if (options.patchImm.has_value())
                throw std::runtime_error("--patch-imm is not supported with --disassemble");
            exportScenarioAssembly(program, *options.exportAssemblyPath, options.abiProfile);
        } else if (options.exportCPath.has_value()) {
            if (options.patchImm.has_value()) {
                throw std::runtime_error("--patch-imm is not supported with --export-c");
            }
            if (options.onlyFunction) {
                const std::string text = renderRecoveredCFunction(program, options.abiProfile, *options.onlyFunction, options.strictCSource);
                std::ofstream file(*options.exportCPath);
                if (!file || !(file << text))
                    throw std::runtime_error("Cannot write C function");
            } else
                exportExplicitSource(program, *options.exportCPath, options.abiProfile, options.namesPath);
        } else if (options.emitPath.has_value()) {
            Scn3Module module = buildScn3Module(program);
            if (options.patchImm.has_value()) {
                applyPatchImm(module, options.patchImm->first, options.patchImm->second);
            }
            const std::vector<uint8_t> emitted = emitScn3Module(module);
            if (!options.patchImm.has_value()) {
                compareEmittedBytes(module, emitted);
            }
            writeFile(*options.emitPath, emitted);
        } else if (options.dumpIr) {
            renderDumpIr(program, options.dumpIrStart.value_or(program.codeStart));
        } else if (options.dumpLayout) {
            renderDumpLayout(program);
        } else if (options.roundtripCheck) {
            runRoundtripCheck(program);
        } else if (options.auditC) {
            std::cout << renderCRecoveryReport(program, options.abiProfile, options.onlyFunction) << "\n";
        } else if (options.audit) {
            renderAudit(program);
        } else if (options.explicitDecompile) {
            std::cout << insertMotionIncludes(renderExplicitSource(program, options.abiProfile));
        } else if (options.decompile) {
            std::cout << insertMotionIncludes(renderExplicitSource(program, options.abiProfile));
        } else {
            renderDisassembly(program, options.showBytes);
        }
        return 0;
    } catch (const std::exception &ex) {
        std::cerr << "error: " << ex.what() << "\n";
        return 1;
    }
}

} // namespace shenmue::scenario
