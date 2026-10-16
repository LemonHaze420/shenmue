#include "d3t/compression.hpp"

#include <cstring>
#include <cstdlib>
#include <limits>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

namespace shenmue::d3t {

std::array<D3TCompressionMode, 10> g_d3tCompressionModes{};
std::uint32_t g_oodleMode = 0;
std::uint32_t g_shouldShutdownOodle = 0;

namespace {

constexpr std::uint32_t kOodleVersion = 0x2e040030;

using OodleCheckVersionFunction = int (*)(std::uint32_t, std::uint64_t);
using OodleCompressedSizeFunction = std::int64_t (*)(std::int64_t);
using OodleDecodeSizeFunction = std::int64_t (*)(std::int64_t, bool);
using OodleCompressFunction = std::int64_t (*)(std::int32_t, const void *, std::int64_t, void *, std::int32_t, const void *, const void *, const void *);
using OodleDecompressFunction = std::int64_t (*)(const void *, std::int64_t, void *, std::int64_t, std::int32_t, std::int32_t, std::int32_t, void *,
    std::int64_t, void *, void *, void *, std::int64_t, std::int32_t);

struct OodleFunctions {
    OodleCheckVersionFunction checkVersion{};
    OodleCompressedSizeFunction getCompressedBufferSize{};
    OodleDecodeSizeFunction getDecodeBufferSize{};
    OodleCompressFunction compress{};
    OodleDecompressFunction decompress{};
};

OodleFunctions g_oodleFunctions{};

std::uint64_t IdentitySize(std::uint64_t size) {
    return size;
}

bool CopyCompressionRequest(D3TCompressionRequest *request) {
    std::memcpy(request->destination, request->source, static_cast<std::size_t>(request->size));
    return true;
}

bool OodleIsAvailable() {
    return g_oodleFunctions.checkVersion && g_oodleFunctions.checkVersion(kOodleVersion, 0) != 0;
}

std::uint64_t OodleCompressedBufferSize(std::uint64_t size) {
    return g_oodleFunctions.getCompressedBufferSize ? static_cast<std::uint64_t>(g_oodleFunctions.getCompressedBufferSize(static_cast<std::int64_t>(size))) : 0;
}

std::uint64_t OodleDecodeBufferSize(std::uint64_t size) {
    return g_oodleFunctions.getDecodeBufferSize ? static_cast<std::uint64_t>(g_oodleFunctions.getDecodeBufferSize(static_cast<std::int64_t>(size), false)) : 0;
}

bool OodleDecompress(D3TCompressionRequest *request) {
    if (!request || !request->source || !request->destination || !OodleIsAvailable() || !g_oodleFunctions.decompress) {
        return false;
    }
    return g_oodleFunctions.decompress(request->source, static_cast<std::int64_t>(request->compressedSize), request->destination,
               static_cast<std::int64_t>(request->size), 0, 1, 0, nullptr, 0, nullptr, nullptr, nullptr, 0, 3) != 0;
}

template <std::int32_t Level> bool OodleCompress(D3TCompressionRequest *request) {
    if (!request || !request->source || !request->destination || !OodleIsAvailable() || !g_oodleFunctions.compress) {
        return false;
    }
    const std::int64_t result =
        g_oodleFunctions.compress(8, request->source, static_cast<std::int64_t>(request->size), request->destination, Level, nullptr, nullptr, nullptr);
    if (result == 0) {
        return false;
    }
    request->compressedSize = static_cast<std::uint64_t>(result);
    return true;
}

#ifdef _WIN32
template <typename Function> Function GetOodleFunction(HMODULE module, const char *name) {
    return reinterpret_cast<Function>(GetProcAddress(module, name));
}

void LoadOodleFunctions() {
    HMODULE module = GetModuleHandleA("oo2core_4_win64.dll");
    if (!module) {
        module = LoadLibraryA("oo2core_4_win64.dll");
    }
    if (!module) {
        return;
    }
    g_oodleFunctions.checkVersion = GetOodleFunction<OodleCheckVersionFunction>(module, "Oodle_CheckVersion");
    g_oodleFunctions.getCompressedBufferSize = GetOodleFunction<OodleCompressedSizeFunction>(module, "OodleLZ_GetCompressedBufferSizeNeeded");
    g_oodleFunctions.getDecodeBufferSize = GetOodleFunction<OodleDecodeSizeFunction>(module, "OodleLZ_GetDecodeBufferSize");
    g_oodleFunctions.compress = GetOodleFunction<OodleCompressFunction>(module, "OodleLZ_Compress");
    g_oodleFunctions.decompress = GetOodleFunction<OodleDecompressFunction>(module, "OodleLZ_Decompress");
}
#else
void LoadOodleFunctions() {
}
#endif

D3TCompressionMode EmptyMode(const char *name) {
    return {name, nullptr, nullptr, nullptr, nullptr, false, {}};
}

D3TCompressionMode MakeOodleMode(const char *name, D3TCompressionFunction compress) {
    return {name, OodleCompressedBufferSize, compress, OodleDecodeBufferSize, OodleDecompress, false, {}};
}

} // namespace

void d3t_InitializeOodleZLib() {
    LoadOodleFunctions();
    g_oodleMode = 2;
    g_shouldShutdownOodle = 2;

    g_d3tCompressionModes[0] = {"Uncompressed", IdentitySize, CopyCompressionRequest, IdentitySize, CopyCompressionRequest, false, {}};
    g_d3tCompressionModes[1] = EmptyMode("ZLib_LowAndFast");
    g_d3tCompressionModes[2] = EmptyMode("ZLib_Balanced");
    g_d3tCompressionModes[3] = EmptyMode("ZLib_HighAndSlow");
    g_d3tCompressionModes[4] = MakeOodleMode("Oodle_LowAndFast", OodleCompress<2>);
    g_d3tCompressionModes[5] = MakeOodleMode("Oodle_Balanced", OodleCompress<4>);
    g_d3tCompressionModes[6] = MakeOodleMode("Oodle_HighAndSlow", OodleCompress<6>);
    g_d3tCompressionModes[7] = EmptyMode("ZLib_Orbis_LowAndFast");
    g_d3tCompressionModes[8] = EmptyMode("ZLib_Orbis_Balanced");
    g_d3tCompressionModes[9] = EmptyMode("ZLib_Orbis_HighAndSlow");
}

void *d3t_DecompressContent(const void *data, std::size_t *size, std::uint32_t *result) {
    *result = 0;
    if (g_oodleMode == 0) {
        *result = 1;
        return nullptr;
    }
    if (!data || !size) {
        *result = 2;
        return nullptr;
    }
    if (*size < 0x10) {
        *result = 2;
        return nullptr;
    }

    CompressedContentHeader header{};
    std::memcpy(&header, data, sizeof(header));
    if (header.compressed > 1 || header.mode >= g_d3tCompressionModes.size() || header.decompressedSize == 0) {
        *result = 3;
        return nullptr;
    }

    const std::uint32_t flagHash = 0x1003fU * header.compressed + 0x79857d2bU;
    const std::uint32_t modeHash = 0x1003fU * header.mode + 0x1bf7f578U;
    const std::uint32_t sizeFold = static_cast<std::uint32_t>(header.decompressedSize) + static_cast<std::uint32_t>(header.decompressedSize / 0xffffffffULL);
    const std::uint32_t checksumValue = 0x20020U * flagHash + 0x10040U * modeHash + 0x4000fU * sizeFold - modeHash - flagHash;
    if (static_cast<std::uint16_t>(checksumValue % 0xffffU) != header.checksum) {
        *result = 3;
        return nullptr;
    }

    D3TCompressionMode &mode = g_d3tCompressionModes[header.mode];
    if (!mode.decodeBufferSize || !mode.decompress) {
        *result = 4;
        return nullptr;
    }
    const std::uint64_t allocationSize = mode.decodeBufferSize(header.decompressedSize);
    if (allocationSize == 0 || allocationSize > (std::numeric_limits<std::size_t>::max)()) {
        *result = 6;
        return nullptr;
    }
    void *destination = std::malloc(static_cast<std::size_t>(allocationSize));
    if (!destination) {
        *result = 7;
        return nullptr;
    }

    D3TCompressionRequest request{static_cast<const std::byte *>(data) + sizeof(CompressedContentHeader), destination, static_cast<std::uint64_t>(*size),
        allocationSize,
        (static_cast<std::uint64_t>(header.reserved) << 32U) | (static_cast<std::uint64_t>(header.checksum) << 16U) |
            (static_cast<std::uint64_t>(header.mode) << 8U) | header.compressed,
        header.decompressedSize};
    if (!mode.decompress(&request)) {
        *result = 6;
        std::free(destination);
        return nullptr;
    }

    if (allocationSize != request.size) {
        if (void *const resized = std::realloc(destination, static_cast<std::size_t>(request.size))) {
            destination = resized;
        }
    }
    *size = static_cast<std::size_t>(request.size);
    return destination;
}

} // namespace shenmue::d3t
