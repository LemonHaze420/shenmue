#include "scn3/bytecode/format.hpp"
#include "scn3/bytecode/program.hpp"
#include "scn3/common/util.hpp"

namespace shenmue::scenario {

void writeScenarioHeader(std::ostream &out, const Scn3Module &module) {
    out << "scn3_header {\n";
    out << "  token = \"" << tokenString(module.parsedHeader.token) << "\";\n";
    out << "  wholeSize = " << hexUnsigned(module.parsedHeader.wholeSize) << ";\n";
    out << "  flags = " << scenarioFlagsText(module.parsedHeader.flags) << ";\n";
    out << "  startupFunctionOffset = " << hexUnsigned(module.parsedHeader.startupFunctionOffset) << ";\n";
    out << "  dataSegmentOffset = " << hexUnsigned(module.parsedHeader.dataSegmentOffset) << ";\n";
    out << "  codeStart = " << hexUnsigned(module.parsedHeader.functionPoolStart) << ";\n";
    out << "  codeEnd = " << hexUnsigned(module.parsedHeader.dataSegmentOffset) << ";\n";
    out << "  dataEndOffset = " << hexUnsigned(module.parsedHeader.dataEndOffset) << ";\n";
    out << "  workspaceSize = " << hexUnsigned(module.parsedHeader.workspaceSize) << ";\n";
    out << "  field1C = " << hexUnsigned(module.parsedHeader.field1C) << ";\n";
    out << "  relocationTableOffset = " << hexUnsigned(module.parsedHeader.relocationTableOffset) << ";\n";
    out << "  field28 = " << hexUnsigned(module.parsedHeader.field28) << ";\n";
    out << "  field2C = " << hexUnsigned(module.parsedHeader.field2C) << ";\n";
    out << "}\n";
}

} // namespace shenmue::scenario
