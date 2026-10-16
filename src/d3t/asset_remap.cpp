#include "d3t/asset_remap.hpp"

#include "d3t/archive_data.hpp"

#include <json/json.h>

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <memory>
#include <string>
#include <vector>

namespace shenmue::d3t {

D3TAssetRemappingState g_d3tAssetRemapping{};

namespace {

constexpr char kAssetRoot[] = "./tex/assets/";
constexpr char kRemappingFile[] = "./tex/assets//Remap/AssetRemapping.json";

template <typename T> void clearRawArray(D3TRawArray<T> &values) {
    std::free(values.data);
    values = {};
}

template <typename T> bool allocateRawArray(D3TRawArray<T> &values, std::uint32_t count) {
    clearRawArray(values);
    if (count == 0) {
        return true;
    }

    std::uint32_t capacity = 16;
    while (capacity < count) {
        if (capacity > (std::numeric_limits<std::uint32_t>::max)() / 2) {
            return false;
        }
        capacity *= 2;
    }
    if (capacity > (std::numeric_limits<std::size_t>::max)() / sizeof(T)) {
        return false;
    }
    values.data = static_cast<T *>(std::calloc(static_cast<std::size_t>(capacity), sizeof(T)));
    if (!values.data) {
        return false;
    }
    values.count = count;
    values.capacity = capacity;
    return true;
}

std::uint32_t numericValue(const Json::Value &value) {
    if (value.isUInt()) {
        return value.asUInt();
    }
    if (value.isInt()) {
        return static_cast<std::uint32_t>(value.asInt());
    }
    if (value.isUInt64()) {
        return static_cast<std::uint32_t>(value.asUInt64());
    }
    if (value.isInt64()) {
        return static_cast<std::uint32_t>(value.asInt64());
    }
    return 0;
}

std::int64_t signedNumericValue(const Json::Value &value, std::int64_t fallback = -1) {
    if (!value.isNumeric()) {
        return fallback;
    }
    return value.isUInt64() ? static_cast<std::int64_t>(value.asUInt64()) : value.asInt64();
}

std::uint64_t md5Half(const Json::Value &values, Json::ArrayIndex first) {
    if (!values.isArray() || values.size() <= first + 1) {
        return 0;
    }
    return static_cast<std::uint64_t>(numericValue(values[first])) | (static_cast<std::uint64_t>(numericValue(values[first + 1])) << 32U);
}

std::string cleanHashPath(const char *source) {
    constexpr std::uint64_t allowedPunctuation = 0x2000000000ffdULL;
    std::string output;
    if (!source) {
        return output;
    }
    output.reserve(std::strlen(source));
    for (; *source; ++source) {
        const int character = std::tolower(static_cast<unsigned char>(*source));
        const int punctuationBit = character - 46;
        if ((character >= 'a' && character <= 'z') || (punctuationBit >= 0 && punctuationBit <= 49 && ((allowedPunctuation >> punctuationBit) & 1ULL) != 0)) {
            output.push_back(static_cast<char>(character));
        }
    }

    if (const char *const suffix = FindHashedPathSuffix(output.c_str()))
        output.resize(static_cast<std::size_t>(suffix - output.c_str()) + 9);
    return output;
}

bool readArchiveValues(const char *jsonText) {
    Json::CharReaderBuilder builder;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    Json::Value root;
    std::string errors;
    const char *const end = jsonText + std::strlen(jsonText);
    if (!reader->parse(jsonText, end, &root, &errors)) {
        return false;
    }

    g_d3tAssetRemapping.hasFullLocationPaths = root["HasFullLocationPaths"].asBool();

    const Json::Value &uniques = root["Uniques"];
    const std::uint32_t uniqueCount = uniques.isArray() ? static_cast<std::uint32_t>(uniques.size()) : 0;
    if (!allocateRawArray(g_d3tAssetRemapping.uniques, uniqueCount)) {
        return false;
    }
    for (std::uint32_t index = 0; index < uniqueCount; ++index) {
        const Json::Value &source = uniques[index];
        D3TAssetUnique &destination = g_d3tAssetRemapping.uniques.data[index];
        const Json::Value &md5 = source["ContHashMD5"];
        destination.contentMd5Low = md5Half(md5, 0);
        destination.contentMd5High = md5Half(md5, 2);
        destination.textureHash = numericValue(source["ContHashTex"]);
        destination.fileSize = numericValue(source["FileSize"]);
    }

    const Json::Value &locations = root["Locations"];
    const std::uint32_t locationCount = locations.isArray() ? static_cast<std::uint32_t>(locations.size()) : 0;
    if (!allocateRawArray(g_d3tAssetRemapping.locations, locationCount)) {
        return false;
    }
    for (std::uint32_t index = 0; index < locationCount; ++index) {
        const Json::Value &source = locations[index];
        D3TAssetLocation &destination = g_d3tAssetRemapping.locations.data[index];
        destination.locationHash = numericValue(source["LocHash"]);
        destination.uniqueIndex = numericValue(source["UniqueIdx"]);

        const std::int64_t collision = signedNumericValue(source["HashCollision"]);
        if (collision >= 0) {
            destination.hasHashCollision = 1;
            destination.hashCollision = static_cast<std::uint32_t>(collision);
        }
        const std::int64_t locationStringIndex = signedNumericValue(source["LocStrIdx"]);
        if (locationStringIndex >= 0) {
            destination.locationStringIndex = static_cast<std::uint32_t>(locationStringIndex);
        }
    }

    clearRawArray(g_d3tAssetRemapping.collisionStrings);
    const Json::Value &collisionStrings = root["HashCollisionStrings"];
    if (collisionStrings.isString()) {
        const std::string source = collisionStrings.asString();
        const std::size_t byteCount = source.size() + 1;
        if (byteCount > (std::numeric_limits<std::uint32_t>::max)()) {
            return false;
        }
        char *const data = static_cast<char *>(std::malloc(byteCount));
        if (!data) {
            return false;
        }
        std::memcpy(data, source.c_str(), byteCount);
        std::replace(data, data + byteCount, '!', '\0');
        g_d3tAssetRemapping.collisionStrings = {data, static_cast<std::uint32_t>(byteCount), static_cast<std::uint32_t>(byteCount)};
    }
    return true;
}

} // namespace
const char *FindHashedPathSuffix(const char *path) {
    if (!path)
        return nullptr;
    const std::size_t length = std::strlen(path);
    std::uint32_t digits = 0;
    std::uint32_t groups = 0;
    for (std::size_t scanned = 0; scanned <= length; ++scanned) {
        const std::size_t index = length - scanned;
        const int character = std::tolower(static_cast<unsigned char>(path[index]));
        if (character == '.') {
            if (digits == 8) {
                digits = 0;
                if (++groups == 2 && scanned >= 0x12)
                    return path + index;
            }
        } else if ((character >= '0' && character <= '9') || (character >= 'a' && character <= 'f')) {
            ++digits;
        } else {
            digits = 0;
            groups = 0;
        }
    }
    return nullptr;
}

bool d3t_ReadAssetRemapping() {
    std::int32_t expected = 0;
    if (!g_d3tAssetRemapping.state.compare_exchange_strong(expected, 9, std::memory_order_seq_cst)) {
        g_d3tAssetRemapping.state.store(0, std::memory_order_seq_cst);
        return false;
    }

    const std::uint64_t fileSize = ArchiveData_GetFileSize(kRemappingFile);
    if (fileSize == 0 || fileSize >= (std::numeric_limits<std::size_t>::max)()) {
        g_d3tAssetRemapping.state.store(0, std::memory_order_seq_cst);
        return false;
    }
    std::vector<char> contents(static_cast<std::size_t>(fileSize) + 1, '\0');
    if (!ArchiveData_Read(kRemappingFile, 0, contents.data(), fileSize) || !readArchiveValues(contents.data())) {
        g_d3tAssetRemapping.state.store(0, std::memory_order_seq_cst);
        return false;
    }

    std::memcpy(g_d3tAssetRemapping.root, kAssetRoot, sizeof(kAssetRoot));
    g_d3tAssetRemapping.state.store(10, std::memory_order_seq_cst);
    return true;
}

const D3TAssetUnique *LoadShadersForRemap(std::uint64_t info, const char *path, bool *sourceUsesRoot) {
    static_cast<void>(info);
    if (g_d3tAssetRemapping.state.load(std::memory_order_seq_cst) == 0) {
        return nullptr;
    }

    std::array<char, 260> cleaned{};
    std::snprintf(cleaned.data(), cleaned.size(), "%s", path);
    FilepathCleaner(cleaned.data());
    bool usesRoot = false;
    if (const char *const root = std::strstr(cleaned.data(), g_d3tAssetRemapping.root)) {
        usesRoot = true;
        std::array<char, 260> relative{};
        std::snprintf(relative.data(), relative.size(), "%s", root + std::strlen(g_d3tAssetRemapping.root));
        cleaned = relative;
        FilepathCleaner(cleaned.data());
    }
    if (sourceUsesRoot) {
        *sourceUsesRoot = usesRoot;
    }

    const std::uint32_t hash = d3t_GenerateHash(cleaned.data());
    for (std::uint32_t index = 0; index < g_d3tAssetRemapping.locations.count; ++index) {
        const D3TAssetLocation &location = g_d3tAssetRemapping.locations.data[index];
        if (location.locationHash != hash) {
            continue;
        }

        bool collisionMatches = location.hasHashCollision == 0;
        if (!collisionMatches) {
            const char *collisionPath = nullptr;
            if (location.hashCollision < g_d3tAssetRemapping.collisionStrings.count) {
                collisionPath = g_d3tAssetRemapping.collisionStrings.data + location.hashCollision;
            }
            collisionMatches = !collisionPath || cleanHashPath(path) == collisionPath;
        }
        if (collisionMatches && location.uniqueIndex < g_d3tAssetRemapping.uniques.count) {
            return g_d3tAssetRemapping.uniques.data + location.uniqueIndex;
        }
    }
    return nullptr;
}

bool genRemapPath(std::uint64_t info, const char *path, char *outputPath, std::uint64_t unknown, bool copyRemap) {
    static_cast<void>(unknown);

    bool sourceUsesRoot = false;
    const D3TAssetUnique *const unique = LoadShadersForRemap(info, path, &sourceUsesRoot);
    if (!unique) {
        return false;
    }

    std::array<char, 260> remap{};
    std::snprintf(remap.data(), remap.size(), "/remap/%llx-%llx-%x-%u.rmp", static_cast<unsigned long long>(unique->contentMd5Low),
        static_cast<unsigned long long>(unique->contentMd5High), unique->textureHash, unique->fileSize);
    if (!copyRemap) {
        std::snprintf(outputPath, 260, "%s", remap.data());
        return true;
    }

    const std::uint32_t hash = d3t_HashData(remap.data(), static_cast<std::uint32_t>(std::strlen(remap.data())));
    std::array<char, 260> hashed{};
    std::snprintf(hashed.data(), hashed.size(), "%s.%08x.%08x", remap.data(), hash, ~hash);
    if (sourceUsesRoot) {
        const std::size_t rootLength = std::strlen(g_d3tAssetRemapping.root);
        const std::size_t hashedLength = std::strlen(hashed.data());
        std::memcpy(outputPath, g_d3tAssetRemapping.root, rootLength);
        std::memcpy(outputPath + rootLength, hashed.data(), hashedLength + 1);
    } else {
        std::snprintf(outputPath, 260, "%s", hashed.data());
    }
    return true;
}

int GenerateFullHashString(const char *source, char *outputPath) {
    std::array<char, 260> stripped{};
    std::array<char, 260> cleaned{};
    std::snprintf(stripped.data(), stripped.size(), "%s", source);
    if (char *const query = std::strchr(stripped.data(), '?')) {
        *query = '\0';
    }
    std::snprintf(cleaned.data(), cleaned.size(), "%s", source);
    char *const query = std::strchr(cleaned.data(), '?');
    if (query) {
        *query = '\0';
    }
    FilepathCleaner(cleaned.data());
    if (query) {
        *query = '?';
    }
    const std::uint32_t hash = d3t_HashData(cleaned.data(), static_cast<std::uint32_t>(std::strlen(cleaned.data())));
    return std::snprintf(outputPath, 260, "%s.%08x.00000000", stripped.data(), hash);
}

} // namespace shenmue::d3t
