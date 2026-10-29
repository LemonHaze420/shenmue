#include "scn3/bytecode/format.hpp"
#include "scn3/bytecode/program.hpp"
#include "scn3/common/util.hpp"
#include "scn3/decompiler/data.hpp"
#include "scn3/native/data.hpp"

namespace shenmue::scenario {

std::string sourceDataName(uint32_t offset) {
    return "data_" + hexName(offset, offset < 0x10000u ? 4 : 0);
}

std::string sourceStringName(uint32_t offset) {
    return "str_" + hexName(offset, offset < 0x10000u ? 4 : 0);
}

std::string sourcePadName(uint32_t offset) {
    return "pad_" + hexName(offset, offset < 0x10000u ? 4 : 0);
}

bool moduleDataContains(const Scn3Module &module, uint32_t offset) {
    return offset >= module.parsedHeader.dataSegmentOffset && offset < moduleDataEnd(module);
}

std::set<uint32_t> collectSourceDataReferenceOffsets(const Scn3Module &module) {
    std::set<uint32_t> refs;
    for (const LowLevelFunction &function : module.functions) {
        for (const LowLevelInsn &insn : function.instructions) {
            if (insn.cls == InstrClass::Ctrl && controlOperation(insn.opcode) == Control::ScnfOffset && insn.imm.has_value()) {
                const std::uint32_t offset = function.baseOffset + *insn.imm;
                if (moduleDataContains(module, offset))
                    refs.insert(offset);
            }
            if (insn.cls != InstrClass::Addr || !insn.imm.has_value()) {
                continue;
            }
            const AddrBase base = addressBase(addressOperation(insn.opcode));
            const uint8_t kind = addressAccess(addressOperation(insn.opcode));
            if (base == AddrBase::CurrBase && kind != 0) {
                const uint32_t absolute = function.baseOffset + *insn.imm;
                if (moduleDataContains(module, absolute)) {
                    refs.insert(absolute);
                }
            }
        }
    }
    return refs;
}

bool isPrintableCStringByte(uint8_t byte) {
    return byte == '\n' || byte == '\r' || byte == '\t' || (byte >= 0x20u && byte <= 0x7Eu);
}

size_t printableCStringLength(const std::vector<uint8_t> &blob, uint32_t start, uint32_t end) {
    uint32_t cursor = start;
    while (cursor < end && isPrintableCStringByte(blob[cursor]) && blob[cursor] != 0) {
        ++cursor;
    }
    if (cursor < end && blob[cursor] == 0 && cursor > start) {
        return cursor - start;
    }
    return 0;
}

std::string sourceEscapeString(const std::vector<uint8_t> &blob, uint32_t start, size_t length) {
    const std::string_view range(reinterpret_cast<const char *>(blob.data()) + start, length);
    const std::optional<std::string> utf8 = shiftJisToUtf8(range);
    if (!utf8 && utf8ToShiftJis(range))
        throw std::runtime_error("String data would read back as UTF-8 text");
    std::ostringstream oss;
    oss << '"';
    for (const char character : utf8 ? std::string_view(*utf8) : range) {
        const uint8_t ch = static_cast<uint8_t>(character);
        switch (ch) {
        case '\\':
            oss << "\\\\";
            break;
        case '"':
            oss << "\\\"";
            break;
        case '\n':
            oss << "\\n";
            break;
        case '\r':
            oss << "\\r";
            break;
        case '\t':
            oss << "\\t";
            break;
        default:
            if ((ch >= 0x20u && ch <= 0x7Eu) || (utf8 && ch >= 0x80u)) {
                oss << static_cast<char>(ch);
            } else {
                oss << "\\x" << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<unsigned>(ch) << std::dec << std::nouppercase;
            }
            break;
        }
    }
    oss << '"';
    return oss.str();
}

std::vector<SourceDataDecl> planSourceDataDecls(
    const std::vector<uint8_t> &blob, uint32_t start, uint32_t end, const std::set<uint32_t> &forcedOffsets, std::span<const ScenarioDataView> typedViews) {
    std::vector<SourceDataDecl> decls;
    uint32_t cursor = start;
    while (cursor < end) {
        uint32_t limit = end;
        if (const std::set<uint32_t>::const_iterator nextForced = forcedOffsets.upper_bound(cursor); nextForced != forcedOffsets.end() && *nextForced < limit) {
            limit = *nextForced;
        }
        bool typed = false;
        for (const ScenarioDataView &view : typedViews) {
            if (view.start > cursor && view.start < limit)
                limit = view.start;
            if (cursor >= view.start && cursor < view.end && (cursor - view.start) % view.schema->size == 0) {
                const std::uint32_t typedEnd = cursor + ((std::min(limit, view.end) - cursor) / view.schema->size) * view.schema->size;
                if (typedEnd > cursor) {
                    decls.push_back(SourceDataDecl{.start = cursor, .end = typedEnd, .type = view.schema->name, .name = sourceDataName(cursor)});
                    cursor = typedEnd;
                    typed = true;
                    break;
                }
            }
        }
        if (typed)
            continue;

        uint32_t zeroEnd = cursor;
        while (zeroEnd < limit && blob[zeroEnd] == 0) {
            ++zeroEnd;
        }

        if (cursor % 4U == 0 && zeroEnd < limit)
            zeroEnd = cursor + (zeroEnd - cursor) / 4U * 4U;
        if (zeroEnd - cursor >= 4) {
            const bool isReferencedData = forcedOffsets.count(cursor) != 0;
            decls.push_back(SourceDataDecl{cursor, zeroEnd, "pad", isReferencedData ? sourceDataName(cursor) : sourcePadName(cursor)});
            cursor = zeroEnd;
            continue;
        }

        const size_t cstrLen = printableCStringLength(blob, cursor, limit);
        const bool isReferencedData = forcedOffsets.count(cursor) != 0;
        if (cstrLen >= (isReferencedData ? 4U : 8U) && cursor + cstrLen + 1 <= limit) {
            decls.push_back(SourceDataDecl{
                cursor, cursor + static_cast<uint32_t>(cstrLen) + 1, "cstr", isReferencedData ? sourceDataName(cursor) : sourceStringName(cursor)});
            cursor += static_cast<uint32_t>(cstrLen) + 1;
            continue;
        }

        if ((cursor % 4u) == 0 && cursor + 4 <= limit) {
            decls.push_back(SourceDataDecl{cursor, cursor + 4, "u32", sourceDataName(cursor)});
            cursor += 4;
            continue;
        }

        uint32_t bytesEnd = std::min<uint32_t>(limit, cursor + 16u);
        if (bytesEnd <= cursor) {
            bytesEnd = cursor + 1;
        }
        decls.push_back(SourceDataDecl{cursor, bytesEnd, "bytes", sourceDataName(cursor)});
        cursor = bytesEnd;
    }
    return decls;
}

std::map<uint32_t, std::string> sourceDataSymbolMap(const std::vector<SourceDataDecl> &decls) {
    std::map<uint32_t, std::string> symbols;
    for (const SourceDataDecl &decl : decls) {
        symbols[decl.start] = decl.name;
    }
    return symbols;
}

void writeBytesArray(std::ostream &out, const std::vector<uint8_t> &blob, uint32_t start, uint32_t end, const std::string &indent) {
    out << indent << "{";
    for (uint32_t off = start; off < end; ++off) {
        if ((off - start) % 16u == 0) {
            out << "\n" << indent << "  ";
        } else {
            out << ' ';
        }
        out << hexUnsigned(blob[off], 2);
        if (off + 1 != end) {
            out << ",";
        }
    }
    out << "\n" << indent << "}";
}

void renderSourceDataDecls(std::ostream &out, const std::vector<uint8_t> &blob, const std::vector<SourceDataDecl> &decls, const std::string &indent) {
    const std::map<std::uint32_t, std::string> symbols = sourceDataSymbolMap(decls);
    for (const SourceDataDecl &decl : decls) {
        if (const ScenarioDataSchema *schema = findScenarioDataSchema(decl.type)) {
            const std::uint32_t count = (decl.end - decl.start) / schema->size;
            out << indent << schema->name << " " << decl.name;
            if (count > 1)
                out << "[" << count << "]";
            out << " = ";
            if (count > 1)
                out << "{\n" << indent << "  ";
            for (std::uint32_t index = 0; index < count; ++index) {
                renderScenarioDataRecord(out, blob, decl.start + index * schema->size, *schema, symbols, indent + (count > 1 ? "  " : ""));
                if (index + 1 != count)
                    out << ",\n" << indent << "  ";
            }
            if (count > 1)
                out << "\n" << indent << "}";
            out << ";\n";
        } else if (decl.type == "pad") {
            out << indent << "pad " << decl.name << "[" << (decl.end - decl.start) << "] = " << hexUnsigned(blob[decl.start], 2) << ";\n";
        } else if (decl.type == "cstr") {
            out << indent << "cstr " << decl.name << " = " << sourceEscapeString(blob, decl.start, decl.end - decl.start - 1) << ";\n";
        } else if (decl.type == "u32") {
            out << indent << "u32 " << decl.name << " = " << hexUnsigned(readU32(blob, decl.start), 8) << ";\n";
        } else {
            out << indent << "bytes " << decl.name << "[] = ";
            writeBytesArray(out, blob, decl.start, decl.end, indent);
            out << ";\n";
        }
    }
}

void renderSourceRawRange(std::ostream &out, const Scn3Module &module, const ByteRange &range) {
    out << "\nraw_range " << rangeKindName(range.kind) << " " << hexUnsigned(range.start, 6) << " " << hexUnsigned(range.end, 6) << " {\n";
    const std::vector<SourceDataDecl> decls = planSourceDataDecls(module.originalBlob, range.start, range.end, {});
    renderSourceDataDecls(out, module.originalBlob, decls, "  ");
    out << "}\n";
}

} // namespace shenmue::scenario
