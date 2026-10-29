#pragma once

#include "scn3/common/std.hpp"

namespace shenmue::scenario {

struct Options {
    std::vector<std::filesystem::path> includeDirectories;
    std::string path;
    std::string abiProfile = "s1-pc";
    std::optional<std::string> emitPath;
    std::string target = "pc";
    std::optional<std::string> emitFormat;
    std::optional<std::string> outputPath;
    std::optional<std::string> extractScn3Path;
    std::optional<std::string> exportCPath;
    std::optional<std::string> namesPath;
    bool strictCSource = false;
    std::optional<std::string> writeCHeaderPath;
    std::optional<std::string> compileCPath;
    std::optional<std::string> exportAssemblyPath;
    std::optional<std::string> assemblePath;
    std::optional<std::string> onlyFunction;
    std::optional<std::string> dumpDataSymbol;
    std::optional<std::pair<uint32_t, uint32_t>> patchImm;
    bool decompile = false;
    bool explicitDecompile = false;
    bool verboseHeader = false;
    bool showBytes = false;
    bool audit = false;
    bool auditC = false;
    bool decodeUncovered = false;
    bool dumpIr = false;
    bool dumpLayout = false;
    bool dumpFunctionBytes = false;
    bool roundtripCheck = false;
    std::optional<uint32_t> startOverride;
    std::optional<uint32_t> dumpIrStart;
};

int runCompiler(int argc, char **argv);

} // namespace shenmue::scenario
