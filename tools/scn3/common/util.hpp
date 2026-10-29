#pragma once

#include "scn3/common/std.hpp"

namespace shenmue::scenario {

std::string hexUnsigned(uint64_t value, int width = 0);
std::string hexOffset(uint32_t value);
std::string hexName(uint32_t value, int width = 0);
uint16_t readU16(const std::vector<uint8_t> &data, uint32_t off);
uint32_t readU32(const std::vector<uint8_t> &data, uint32_t off);
void ensureCodeReadable(const std::vector<uint8_t> &data, uint32_t off, uint32_t size, uint32_t codeEnd, const char *what);
uint16_t readU16Code(const std::vector<uint8_t> &data, uint32_t off, uint32_t codeEnd);
uint32_t readU32Code(const std::vector<uint8_t> &data, uint32_t off, uint32_t codeEnd);
std::vector<uint8_t> readFile(const std::string &path);
void writeFile(const std::string &path, const std::vector<uint8_t> &data);
uint32_t floatBitsFromLiteralText(std::string value);
bool floatLiteralTextRoundTrips(const std::string &text, uint32_t bits);
std::string formatFloatLiteral(uint32_t value);
uint32_t parseIntegerText(const std::string &text);
uint32_t parseSourceIntegerLiteral(const std::string &text);
uint32_t parseSourceFloatBits(const std::string &text);
uint32_t parseInteger(const std::string &text);
std::string scriptFunctionName(uint32_t offset);
std::string trimCopy(const std::string &text);

std::optional<std::string> shiftJisToUtf8(std::string_view bytes);
std::optional<std::string> utf8ToShiftJis(std::string_view text);

void parallelFor(size_t count, const std::function<void(size_t)> &body);

} // namespace shenmue::scenario
