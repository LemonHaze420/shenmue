#include "scn3/common/util.hpp"

#include <atomic>
#include <thread>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#else
#include <iconv.h>
#endif

namespace shenmue::scenario {

std::string hexName(uint32_t value, int width) {
    return hexUnsigned(value, width).substr(2);
}

std::string hexUnsigned(uint64_t value, int width) {
    char digits[16];
    int count = 0;
    do {
        digits[count++] = "0123456789ABCDEF"[value & 0xFU];
        value >>= 4;
    } while (value);
    std::string text = "0x";
    for (int pad = count; pad < width; ++pad)
        text += '0';
    while (count)
        text += digits[--count];
    return text;
}

std::string hexOffset(uint32_t value) {
    return hexName(value, 6);
}

uint16_t readU16(const std::vector<uint8_t> &data, uint32_t off) {
    if (off + 1 >= data.size()) {
        throw std::runtime_error("Unexpected EOF while reading u16");
    }
    return static_cast<uint16_t>(data[off]) | (static_cast<uint16_t>(data[off + 1]) << 8);
}

uint32_t readU32(const std::vector<uint8_t> &data, uint32_t off) {
    if (off + 3 >= data.size()) {
        throw std::runtime_error("Unexpected EOF while reading u32");
    }
    return static_cast<uint32_t>(data[off]) | (static_cast<uint32_t>(data[off + 1]) << 8) | (static_cast<uint32_t>(data[off + 2]) << 16) |
        (static_cast<uint32_t>(data[off + 3]) << 24);
}

void ensureCodeReadable(const std::vector<uint8_t> &data, uint32_t off, uint32_t size, uint32_t codeEnd, const char *what) {
    if (off > codeEnd || size > codeEnd - off || off > data.size() || size > data.size() - off) {
        throw std::runtime_error(std::string("Unexpected EOF while reading ") + what + " at " + hexUnsigned(off) + " before codeEnd " + hexUnsigned(codeEnd));
    }
}

uint16_t readU16Code(const std::vector<uint8_t> &data, uint32_t off, uint32_t codeEnd) {
    ensureCodeReadable(data, off, 2, codeEnd, "u16 immediate");
    return readU16(data, off);
}

uint32_t readU32Code(const std::vector<uint8_t> &data, uint32_t off, uint32_t codeEnd) {
    ensureCodeReadable(data, off, 4, codeEnd, "u32 immediate");
    return readU32(data, off);
}

std::vector<uint8_t> readFile(const std::string &path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + path);
    }
    file.seekg(0, std::ios::end);
    const std::streamoff size = file.tellg();
    if (size < 0) {
        throw std::runtime_error("Failed to determine file size: " + path);
    }
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> data(static_cast<size_t>(size));
    if (!data.empty()) {
        file.read(reinterpret_cast<char *>(data.data()), size);
    }
    if (!file && !data.empty()) {
        throw std::runtime_error("Failed to read file: " + path);
    }
    return data;
}

void writeFile(const std::string &path, const std::vector<uint8_t> &data) {
    std::ofstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open output file: " + path);
    }
    if (!data.empty()) {
        file.write(reinterpret_cast<const char *>(data.data()), static_cast<std::streamsize>(data.size()));
    }
    if (!file) {
        throw std::runtime_error("Failed to write output file: " + path);
    }
}

uint32_t floatBitsFromLiteralText(std::string value) {
    if (!value.empty() && (value.back() == 'f' || value.back() == 'F')) {
        value.pop_back();
    }
    char *end = nullptr;
    const float f = std::strtof(value.c_str(), &end);
    if (end != value.c_str() + value.size() || end == value.c_str() || !std::isfinite(f))
        throw std::runtime_error("Invalid finite float literal: " + value);
    uint32_t bits = 0;
    std::memcpy(&bits, &f, sizeof(bits));
    return bits;
}

bool floatLiteralTextRoundTrips(const std::string &text, uint32_t bits) {
    try {
        return floatBitsFromLiteralText(text) == bits;
    } catch (const std::exception &) {
        return false;
    }
}

std::string formatFloatLiteral(uint32_t value) {
    float f = 0.0f;
    static_assert(sizeof(f) == sizeof(value), "Unexpected float size");
    std::memcpy(&f, &value, sizeof(f));
    if (!std::isfinite(f)) {
        return hexUnsigned(value);
    }

    std::ostringstream oss;
    const float rounded1 = std::round(f * 10.0f) / 10.0f;
    const float rounded2 = std::round(f * 100.0f) / 100.0f;
    if (std::fabs(f - rounded1) < 0.00001f) {
        oss << std::fixed << std::setprecision(1) << rounded1;
    } else if (std::fabs(f - rounded2) < 0.00001f) {
        oss << std::fixed << std::setprecision(2) << rounded2;
    } else {
        oss << std::fixed << std::setprecision(6) << f;
    }
    std::string out = oss.str();
    if (out.find('.') != std::string::npos) {
        while (out.size() > 3 && out.back() == '0' && out[out.size() - 2] != '.') {
            out.pop_back();
        }
    }
    out += "f";
    if (floatLiteralTextRoundTrips(out, value)) {
        return out;
    }
    std::ostringstream exact;
    exact << std::setprecision(std::numeric_limits<float>::max_digits10) << f;
    std::string literal = exact.str();
    if (literal.find_first_of(".eE") == std::string::npos)
        literal += ".0";
    literal += "f";
    if (!floatLiteralTextRoundTrips(literal, value))
        throw std::runtime_error("Cannot print finite float bits exactly");
    return literal;
}

uint32_t parseIntegerText(const std::string &text) {
    if (text.size() > 2 && text[0] == '0' && (text[1] == 'x' || text[1] == 'X')) {
        return static_cast<uint32_t>(std::stoul(text, nullptr, 16));
    }
    return static_cast<uint32_t>(std::stoul(text, nullptr, 10));
}

uint32_t parseSourceIntegerLiteral(const std::string &text) {
    std::string value = text;
    if (!value.empty() && (value.back() == 'u' || value.back() == 'U')) {
        value.pop_back();
    }
    return parseIntegerText(value);
}

uint32_t parseSourceFloatBits(const std::string &text) {
    return floatBitsFromLiteralText(text);
}

uint32_t parseInteger(const std::string &text) {
    size_t pos = 0;
    unsigned long value = std::stoul(text, &pos, 0);
    if (pos != text.size()) {
        throw std::runtime_error("Invalid integer: " + text);
    }
    if (value > 0xFFFFFFFFul) {
        throw std::runtime_error("Integer out of range: " + text);
    }
    return static_cast<uint32_t>(value);
}

std::string scriptFunctionName(uint32_t offset) {
    std::ostringstream oss;
    oss << "script_" << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << offset;
    return oss.str();
}

std::string trimCopy(const std::string &text) {
    const size_t first = text.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }
    const size_t last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

static std::optional<std::string> convertText(std::string_view text, bool toShiftJis) {
    if (text.empty())
        return std::string{};
#ifdef _WIN32
    const UINT from = toShiftJis ? CP_UTF8 : 932, to = toShiftJis ? 932 : CP_UTF8;
    const int wideLength = MultiByteToWideChar(from, MB_ERR_INVALID_CHARS, text.data(), static_cast<int>(text.size()), nullptr, 0);
    if (wideLength <= 0)
        return std::nullopt;
    std::wstring wide(static_cast<size_t>(wideLength), L'\0');
    MultiByteToWideChar(from, MB_ERR_INVALID_CHARS, text.data(), static_cast<int>(text.size()), wide.data(), wideLength);
    BOOL substituted = FALSE;
    BOOL *const substitution = toShiftJis ? &substituted : nullptr;
    const DWORD flags = toShiftJis ? WC_NO_BEST_FIT_CHARS : WC_ERR_INVALID_CHARS;
    const int length = WideCharToMultiByte(to, flags, wide.data(), wideLength, nullptr, 0, nullptr, substitution);
    if (length <= 0 || substituted)
        return std::nullopt;
    std::string out(static_cast<size_t>(length), '\0');
    WideCharToMultiByte(to, flags, wide.data(), wideLength, out.data(), length, nullptr, substitution);
    if (substituted)
        return std::nullopt;
    return out;
#else
    const iconv_t converter = iconv_open(toShiftJis ? "CP932" : "UTF-8", toShiftJis ? "UTF-8" : "CP932");
    if (converter == reinterpret_cast<iconv_t>(-1))
        throw std::runtime_error("Shift-JIS text conversion is unavailable");
    std::string out(text.size() * 4, '\0');
    char *input = const_cast<char *>(text.data()), *output = out.data();
    size_t inputLeft = text.size(), outputLeft = out.size();
    const size_t result = iconv(converter, &input, &inputLeft, &output, &outputLeft);
    iconv_close(converter);
    if (result == static_cast<size_t>(-1) || inputLeft)
        return std::nullopt;
    out.resize(out.size() - outputLeft);
    return out;
#endif
}

static bool readableText(std::string_view utf8) {
    bool wide = false;
    for (size_t index = 0; index < utf8.size(); ++index) {
        const uint8_t lead = static_cast<uint8_t>(utf8[index]);
        if (lead < 0x80U)
            continue;
        wide = true;
        size_t extra = (lead & 0xE0U) == 0xC0U ? 1 : (lead & 0xF0U) == 0xE0U ? 2 : 3;
        uint32_t point = lead & (0x3FU >> extra);
        for (; extra && ++index < utf8.size(); --extra)
            point = point << 6U | (static_cast<uint8_t>(utf8[index]) & 0x3FU);
        if (point < 0xA0U || (point >= 0xE000U && point <= 0xF8FFU))
            return false;
    }
    return wide;
}

std::optional<std::string> shiftJisToUtf8(std::string_view bytes) {
    const std::optional<std::string> text = convertText(bytes, false);
    if (!text || !readableText(*text) || convertText(*text, true) != std::string(bytes))
        return std::nullopt;
    return text;
}

std::optional<std::string> utf8ToShiftJis(std::string_view text) {
    const std::optional<std::string> bytes = convertText(text, true);
    if (!bytes || !readableText(text) || convertText(*bytes, false) != std::string(text))
        return std::nullopt;
    return bytes;
}

void parallelFor(size_t count, const std::function<void(size_t)> &body) {
    std::atomic<size_t> next = 0;
    std::vector<std::thread> threads;
    for (size_t worker = std::min<size_t>(count, std::max(1U, std::thread::hardware_concurrency())); worker; --worker)
        threads.emplace_back([&] {
            for (size_t index; (index = next++) < count;)
                body(index);
        });
    for (std::thread &thread : threads)
        thread.join();
}

} // namespace shenmue::scenario
