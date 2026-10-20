#include "common/archive_data.hpp"
#include <shendk/files/container/tad_reader.h>

#include <json/json.h>

#include <algorithm>
#include <array>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace shenmue::archive_data {
namespace {

namespace fs = std::filesystem;

struct TadEntry {
    fs::path tacPath;
    std::uint32_t offset;
    std::uint32_t size;
    std::uint32_t sourceIndex;
    std::uint32_t flags;
};

struct TadArchive {
    fs::path tadPath;
    fs::path tacPath;
    std::uint64_t order;
    std::uint32_t sourceIndex;
};

std::mutex g_indexMutex;
bool g_indexAttempted = false;
fs::path g_shenRoot;
std::unordered_map<std::uint64_t, TadEntry> g_tadEntries;
std::unordered_map<std::string, TadEntry> g_namedEntries;

std::uint64_t hashKey(std::uint32_t primary, std::uint32_t secondary);
std::uint64_t normalHash(const char *path);
std::string cleanFilePath(const char *input);

fs::path executableDirectory() {
#ifdef _WIN32
    std::array<wchar_t, 32768> path{};
    const DWORD length = GetModuleFileNameW(nullptr, path.data(), static_cast<DWORD>(path.size()));
    return length != 0 && length < path.size() ? fs::path(path.data(), path.data() + length).parent_path() : fs::path{};
#else
    std::array<char, 4096> path{};
    const ssize_t length = readlink("/proc/self/exe", path.data(), path.size());
    return length > 0 && static_cast<std::size_t>(length) < path.size() ? fs::path(path.data(), path.data() + length).parent_path() : fs::path{};
#endif
}

void addNamedAliases(const fs::path &databasePath) {
    std::ifstream input(databasePath, std::ios::binary);
    if (!input) {
        return;
    }
    Json::CharReaderBuilder builder;
    Json::Value root;
    std::string errors;
    if (!Json::parseFromStream(builder, input, &root, &errors) || !root.isArray()) {
        return;
    }

    for (const Json::Value &value : root) {
        if (!value["Hash"].isUInt() || !value["HashPath"].isUInt() || !value["Path"].isString()) {
            continue;
        }
        const auto found = g_tadEntries.find(hashKey(value["Hash"].asUInt(), value["HashPath"].asUInt()));
        if (found == g_tadEntries.end()) {
            continue;
        }
        const TadEntry entry = found->second;

        const std::string path = value["Path"].asString();
        const auto addAlias = [&](const std::string &alias) {
            g_tadEntries.emplace(normalHash(alias.c_str()), entry);
            g_namedEntries.emplace(cleanFilePath(alias.c_str()), entry);
        };
        addAlias(path);
        addAlias('.' + path);

        const std::size_t finalDot = path.rfind('.');
        if (finalDot != std::string::npos && path.size() - finalDot - 1 == 8) {
            const std::string twoHashPath = path + ".00000000";
            addAlias(twoHashPath);
            addAlias('.' + twoHashPath);
        }
    }
}

std::uint64_t hashKey(std::uint32_t primary, std::uint32_t secondary) {
    return (static_cast<std::uint64_t>(primary) << 32) | secondary;
}

bool isSlash(char value) {
    return value == '/' || value == '\\';
}

std::string cleanFilePath(const char *input) {
#if 0
    std::string result;
    if (!input) {
        return result;
    }
    result.reserve(std::strlen(input));
    const char *cursor = input;
    while (*cursor) {
        if (isSlash(*cursor)) {
            while (isSlash(cursor[1])) {
                ++cursor;
            }
        }
        if (*cursor == '.' && cursor != input && cursor[1] && isSlash(cursor[-1]) && isSlash(cursor[1])) {
            cursor += 2;
            if (!*cursor) {
                break;
            }
        }
        char value = *cursor++;
        if (value == '\\') {
            value = '/';
        }
        result.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(value))));
    }
    return result;
#else
    return input ? shendk::CleanFilePath(input) : std::string{};
#endif
}

std::optional<std::size_t> findHashedSuffix(const std::string &value) {
    std::int32_t digitCount = 0;
    std::int32_t groupCount = 0;
    std::uint32_t scanned = 0;
    for (std::size_t cursor = value.size(); cursor-- > 0;) {
        const char character = static_cast<char>(std::tolower(static_cast<unsigned char>(value[cursor])));
        if (character == '.') {
            if (digitCount == 8) {
                digitCount = 0;



                if (++groupCount == 2 && scanned + 1 >= 0x12) {
                    return cursor;
                }
            }
        } else if ((character >= '0' && character <= '9') || (character >= 'a' && character <= 'f')) {
            ++digitCount;
        } else {
            digitCount = 0;
            groupCount = 0;
        }
        ++scanned;
    }
    return std::nullopt;
}

std::uint32_t murmurHash2(const std::byte *data, std::uint32_t length) {
#if 0
    constexpr std::uint32_t multiplier = 0x5bd1e995;
    std::uint32_t hash = (length + length / 0xffffffffU) ^ 0x066ee5d0U;
    std::uint32_t offset = 0;
    for (; offset + 4 <= length; offset += 4) {
        std::uint32_t word;
        std::memcpy(&word, data + offset, sizeof(word));
        word *= multiplier;
        word ^= word >> 24;
        word *= multiplier;
        hash = hash * multiplier ^ word;
    }

    switch (length - offset) {
    case 3:
        hash ^= static_cast<std::uint32_t>(data[offset + 2]) << 16;
        [[fallthrough]];
    case 2:
        hash ^= static_cast<std::uint32_t>(data[offset + 1]) << 8;
        [[fallthrough]];
    case 1:
        hash = multiplier * (hash ^ static_cast<std::uint32_t>(data[offset]));
        break;
    default:
        break;
    }
    hash = multiplier * (hash ^ (hash >> 13));
    return hash ^ (hash >> 15);
#else
    return shendk::HashData(data, length);
#endif
}

std::uint32_t generatePathHash(const std::string &path) {
#if 0
    constexpr std::uint64_t allowedPunctuation = 0x2000000000ffdULL;
    std::string filtered;
    filtered.reserve(path.size());
    for (const char source : path) {
        const int character = std::tolower(static_cast<unsigned char>(source));
        const int punctuationBit = character - 46;
        if ((character >= 'a' && character <= 'z') || (punctuationBit >= 0 && punctuationBit <= 49 && ((allowedPunctuation >> punctuationBit) & 1U) != 0)) {
            filtered.push_back(static_cast<char>(character));
        }
    }

    const std::uint64_t originalLength = filtered.size();
    if (const auto suffix = findHashedSuffix(filtered)) {
        filtered.resize(*suffix + 9);
    }
    const std::uint64_t hashLength = filtered.size();
    const std::uint32_t base = murmurHash2(reinterpret_cast<const std::byte *>(filtered.data()), static_cast<std::uint32_t>(hashLength));
    const std::uint64_t product = originalLength * hashLength;
    const std::uint32_t remainder = static_cast<std::uint32_t>(product % 0xffffffffULL);
    return 0x1003fU * base + 0x2001fU * remainder;
#else
    return shendk::GeneratePathHash(path);
#endif
}

std::uint64_t normalHash(const char *path) {
#if 0
    std::string normalized = cleanFilePath(path);
    std::uint32_t secondary = 0;
    if (const auto suffix = findHashedSuffix(normalized)) {
        const std::string first = normalized.substr(*suffix + 1, 8);
        secondary = static_cast<std::uint32_t>(std::strtoul(first.c_str(), nullptr, 16));
        normalized.replace(*suffix + 10, 8, "00000000");
    }
    return hashKey(generatePathHash(normalized), secondary);
#else
    return shendk::GetPathHash(path ? path : "");
#endif
}

std::string lower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](char ch) { return static_cast<char>(std::tolower(static_cast<unsigned char>(ch))); });
    return value;
}

std::optional<fs::path> childPathCaseInsensitive(const fs::path &parent, const std::string &name) {
    std::error_code error;
    const fs::path exact = parent / name;
    if (fs::exists(exact, error)) {
        return exact;
    }
    error.clear();
    for (const fs::directory_entry &entry : fs::directory_iterator(parent, error)) {
        if (error) {
            break;
        }
        if (lower(entry.path().filename().string()) == lower(name)) {
            return entry.path();
        }
    }
    return std::nullopt;
}

bool readTadHeader(const fs::path &tadPath, const char *backend, std::uint32_t type, TadArchive &archive) {
#if 0
    std::ifstream tad(tadPath, std::ios::binary);
    if (!tad) {
        return false;
    }
    std::array<std::byte, 60> header{};
    tad.read(reinterpret_cast<char *>(header.data()), static_cast<std::streamsize>(sizeof(header)));
    if (tad.gcount() != static_cast<std::streamsize>(sizeof(header))) {
        return false;
    }

    const auto word = [&header](std::size_t offset) {
        std::uint32_t value;
        std::memcpy(&value, header.data() + offset, sizeof(value));
        return value;
    };
    const std::uint32_t storedHash = word(32);
    std::memset(header.data() + 32, 0, sizeof(storedHash));
    if (d3t_HashData(header.data(), 56) != storedHash || word(8) < 2 || word(4) < 5) {
        return false;
    }
    const char *const headerBackend = reinterpret_cast<const char *>(header.data() + 24);
    if (!backend || std::strncmp(headerBackend, backend, 8) != 0 || (type != 2 && (word(12) & 0x0aU) == 0)) {
        return false;
    }

    std::memcpy(&archive.order, header.data() + 16, sizeof(archive.order));
    return true;
#else
    if (!backend)
        return false;
    std::ifstream tad(tadPath, std::ios::binary);
    std::array<std::byte, 60> bytes;
    tad.read(reinterpret_cast<char *>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
    if (tad.gcount() != static_cast<std::streamsize>(bytes.size()))
        return false;
    shendk::TadHeader header;
    if (!shendk::ReadTadHeader(bytes, header, {}, type) || std::strncmp(header.backend.data(), backend, 8) != 0)
        return false;
    archive.order = header.order;
    return true;
#endif
}

bool indexTad(const TadArchive &archive, std::uint32_t &entryCount, std::uint64_t &packedSize) {
#if 0
    std::ifstream tad(archive.tadPath, std::ios::binary);
    if (!tad) {
        return false;
    }
    std::array<std::byte, 60> header{};
    tad.read(reinterpret_cast<char *>(header.data()), static_cast<std::streamsize>(header.size()));
    if (tad.gcount() != static_cast<std::streamsize>(header.size())) {
        return false;
    }
    std::uint32_t count;
    std::memcpy(&count, header.data() + 56, sizeof(count));

    std::error_code error;
    const std::uint64_t tacSize = fs::file_size(archive.tacPath, error);
    if (error) {
        return false;
    }

    std::vector<std::array<std::uint32_t, 8>> entries(count);
    for (std::uint32_t index = 0; index < count; ++index) {
        tad.read(reinterpret_cast<char *>(entries[index].data()), static_cast<std::streamsize>(sizeof(entries[index])));
        if (tad.gcount() != static_cast<std::streamsize>(sizeof(entries[index]))) {
            return false;
        }
    }
    if (entries.empty()) {
        return true;
    }
    const auto &last = entries.back();
    if (static_cast<std::uint64_t>(last[4]) + last[6] > tacSize) {
        return false;
    }

    for (const auto &entry : entries) {
        if (entry[0] == 0) {
            continue;
        }
        if (g_tadEntries.emplace(hashKey(entry[0], entry[1]), TadEntry{archive.tacPath, entry[4], entry[6], archive.sourceIndex, entry[7]}).second) {
            ++entryCount;
        }
    }
    packedSize += tacSize;
    return true;
#else
    std::ifstream tad(archive.tadPath, std::ios::binary);
    std::array<std::byte, 60> headerBytes;
    tad.read(reinterpret_cast<char *>(headerBytes.data()), static_cast<std::streamsize>(headerBytes.size()));
    if (tad.gcount() != static_cast<std::streamsize>(headerBytes.size()))
        return false;
    shendk::TadHeader header;
    if (!shendk::ReadTadHeader(headerBytes, header))
        return false;
    std::error_code error;
    const std::uint64_t tacSize = fs::file_size(archive.tacPath, error);
    if (error)
        return false;
    const std::size_t tableSize = static_cast<std::size_t>(header.entryCount) * sizeof(shendk::TadEntry);
    std::vector<std::byte> bytes(headerBytes.size() + tableSize);
    std::memcpy(bytes.data(), headerBytes.data(), headerBytes.size());
    tad.read(reinterpret_cast<char *>(bytes.data() + headerBytes.size()), static_cast<std::streamsize>(tableSize));
    if (tad.gcount() != static_cast<std::streamsize>(tableSize))
        return false;
    if (!header.entryCount)
        return true;
    shendk::TadEntry entry;
    if (!shendk::ReadTadEntry(bytes, header.entryCount - 1U, entry) || static_cast<std::uint64_t>(entry.fileOffset) + entry.fileSize > tacSize)
        return false;
    for (std::uint32_t index = 0; index < header.entryCount; ++index) {
        if (!shendk::ReadTadEntry(bytes, index, entry))
            return false;
        if (!entry.hash1)
            continue;
        if (g_tadEntries
                .emplace(hashKey(entry.hash1, entry.hash2), TadEntry{archive.tacPath, entry.fileOffset, entry.fileSize, archive.sourceIndex, entry.reserved1c})
                .second)
            ++entryCount;
    }
    packedSize += tacSize;
    return true;
#endif
}

void ensureIndex() {
    std::scoped_lock lock(g_indexMutex);
    if (g_indexAttempted) {
        return;
    }
    g_indexAttempted = true;
}

std::optional<fs::path> looseFilePath(const char *path) {
    if (g_shenRoot.empty() || !path) {
        return std::nullopt;
    }
    std::string relative = path;
    while (!relative.empty() && isSlash(relative.front())) {
        relative.erase(relative.begin());
    }
    const std::array candidates{g_shenRoot / relative};
    std::error_code error;
    for (const fs::path &candidate : candidates) {
        if (fs::is_regular_file(candidate, error)) {
            return candidate;
        }
    }
    return std::nullopt;
}

void findArchiveSources(const fs::path &root, const char *backend, std::uint32_t type, std::vector<TadArchive> &archives) {
    std::error_code error;
    for (fs::directory_iterator cursor(root, error), end; !error && cursor != end; cursor.increment(error)) {
        if (!cursor->is_regular_file(error) || lower(cursor->path().extension().string()) != ".tad")
            continue;
        TadArchive archive{cursor->path(), cursor->path(), 0, static_cast<std::uint32_t>(archives.size())};
        archive.tacPath.replace_extension(".tac");
        if (!fs::is_regular_file(archive.tacPath, error)) {
            archive.tacPath = cursor->path();
            archive.tacPath.replace_extension(".TAC");
        }
        if (fs::is_regular_file(archive.tacPath, error) && readTadHeader(archive.tadPath, backend, type, archive))
            archives.push_back(std::move(archive));
        error.clear();
    }
    error.clear();
    for (fs::directory_iterator cursor(root, error), end; !error && cursor != end; cursor.increment(error)) {
        if (cursor->is_directory(error))
            findArchiveSources(cursor->path(), backend, type, archives);
        error.clear();
    }
}

} // namespace

std::uint32_t d3t_HashData(const void *data, std::uint32_t size) {
    if (!data && size != 0) {
        return 0;
    }
    return murmurHash2(static_cast<const std::byte *>(data), size);
}

std::uint32_t d3t_GenerateHash(const char *path) {
    return path ? generatePathHash(path) : 0;
}

char *FilepathCleaner(char *path) {
    if (!path) {
        return nullptr;
    }
    const std::string cleaned = cleanFilePath(path);
    std::memcpy(path, cleaned.c_str(), cleaned.size() + 1);
    return path;
}

bool ArchiveData_LoadArchives(const char *root, const char *backend, std::uint32_t type, ArchiveDataLoadResult *result) {
    std::scoped_lock lock(g_indexMutex);
    g_indexAttempted = true;
    g_tadEntries.clear();
    g_namedEntries.clear();
    g_shenRoot = root ? fs::path(root) : fs::path{};

    ArchiveDataLoadResult loaded{};
    if (!root || !*root || !backend || !*backend) {
        if (result) {
            *result = loaded;
        }
        return false;
    }

    const std::optional<fs::path> archivesDirectory = childPathCaseInsensitive(g_shenRoot, "Archives");
    const std::optional<fs::path> archiveRoot = archivesDirectory ? childPathCaseInsensitive(*archivesDirectory, backend) : std::nullopt;
    std::error_code error;
    if (!archiveRoot || !fs::is_directory(*archiveRoot, error)) {
        if (result) {
            *result = loaded;
        }
        return false;
    }

    std::vector<TadArchive> archives;
    findArchiveSources(*archiveRoot, backend, type, archives);
    std::sort(archives.begin(), archives.end(), [](const TadArchive &left, const TadArchive &right) { return left.order > right.order; });
    for (const TadArchive &archive : archives) {
        if (indexTad(archive, loaded.entryCount, loaded.packedSize)) {
            ++loaded.archiveCount;
        }
    }
    const std::array nameDatabases{
        executableDirectory() / "filename_database.json",
        fs::path{"filename_database.json"},
        fs::path{"reference"} / "filename_database.json",
        g_shenRoot / "filename_database.json",
    };
    for (const fs::path &database : nameDatabases) {
        if (fs::is_regular_file(database, error)) {
            addNamedAliases(database);
            break;
        }
        error.clear();
    }
    if (result) {
        *result = loaded;
    }
    return loaded.archiveCount != 0;
}

void ArchiveData_ClearArchives() {
    std::scoped_lock lock(g_indexMutex);
    g_indexAttempted = true;
    g_shenRoot.clear();
    g_tadEntries.clear();
    g_namedEntries.clear();
}

bool ArchiveData_HasArchives() {
    std::scoped_lock lock(g_indexMutex);
    return !g_tadEntries.empty();
}

std::uint64_t ArchiveData_GetPathHash(const char *path) {
    return normalHash(path);
}

std::uint64_t ArchiveData_GetFileSize(const char *path) {
    ensureIndex();
    const auto named = g_namedEntries.find(cleanFilePath(path));
    if (named != g_namedEntries.end()) {
        return named->second.size;
    }
    const auto found = g_tadEntries.find(normalHash(path));
    if (found != g_tadEntries.end()) {
        return found->second.size;
    }
    if (const auto loose = looseFilePath(path)) {
        std::error_code error;
        const std::uint64_t size = fs::file_size(*loose, error);
        return error ? 0 : size;
    }
    return 0;
}
bool ArchiveData_IsPackedFile(const char *path) {
    ensureIndex();
    const std::unordered_map<std::string, TadEntry>::const_iterator named = g_namedEntries.find(cleanFilePath(path));
    if (named != g_namedEntries.end())
        return named->second.size != 0;
    const std::unordered_map<std::uint64_t, TadEntry>::const_iterator found = g_tadEntries.find(normalHash(path));
    return found != g_tadEntries.end() && found->second.size != 0;
}

void ArchiveData_GetSourceDesc(const char *path, ArchiveSourceDesc &source) {
    ensureIndex();
    const TadEntry *entry = nullptr;
    const std::unordered_map<std::string, TadEntry>::const_iterator named = g_namedEntries.find(cleanFilePath(path));
    if (named != g_namedEntries.end())
        entry = &named->second;
    else {
        const std::unordered_map<std::uint64_t, TadEntry>::const_iterator found = g_tadEntries.find(normalHash(path));
        if (found != g_tadEntries.end())
            entry = &found->second;
    }
    if (!entry || (entry->flags & 0x10000U)) {
        source = {};
        return;
    }
    source.sourceIndex = entry->sourceIndex;
    source.sourceOffset = entry->offset;
    source.fileSize = entry->size;
    source.flags = static_cast<std::uint16_t>(entry->flags);
    source.reserved16 = static_cast<std::uint16_t>(entry->flags >> 16U);
    source.remapEntry = entry;
}

bool ArchiveData_Read(const char *path, std::uint64_t offset, void *destination, std::uint64_t size) {
    if (!destination) {
        return false;
    }
    ensureIndex();
    fs::path source;
    std::uint64_t fileOffset = offset;
    const TadEntry *entry = nullptr;
    const auto named = g_namedEntries.find(cleanFilePath(path));
    if (named != g_namedEntries.end()) {
        entry = &named->second;
    } else {
        const auto found = g_tadEntries.find(normalHash(path));
        if (found != g_tadEntries.end()) {
            entry = &found->second;
        }
    }
    if (entry) {
        source = entry->tacPath;
        fileOffset += entry->offset;
        if (offset > entry->size || size > entry->size - offset) {
            return false;
        }
    } else if (const auto loose = looseFilePath(path)) {
        source = *loose;
    } else {
        return false;
    }

    std::ifstream input(source, std::ios::binary);
    if (entry) {
        const shendk::TadEntry member{.fileOffset = entry->offset, .fileSize = entry->size};
        return shendk::ReadTacAsset(input, member, offset, {static_cast<std::byte *>(destination), static_cast<std::size_t>(size)});
    }
    input.seekg(static_cast<std::streamoff>(fileOffset), std::ios::beg);
    input.read(static_cast<char *>(destination), static_cast<std::streamsize>(size));
    return input.gcount() == static_cast<std::streamsize>(size);
}

} // namespace shenmue::archive_data
