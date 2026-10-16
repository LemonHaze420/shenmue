#include "d3t/model_paths.hpp"

#include "d3t/archive_data.hpp"

#include <cstdio>
#include <cstring>

namespace shenmue::d3t {
namespace {

std::int32_t ClassifyRemapPath(const char *path) {
    std::size_t end = std::strlen(path);
    const char *name = path;
    if (end > 1) {
        while (true) {
            if (path[end - 1] == '/' || path[end - 1] == '\\') {
                name = path + end;
                break;
            }
            if (end-- == 1 || end == 1)
                break;
        }
    }
    const std::size_t length = std::strlen(name);
    if (std::strstr(name, "map") == name) {
        if (length == 7) {
            if (name[3] == '.')
                return 0;
        } else if (length == 8) {
            if (name[3] == 's' || name[4] == '.')
                return 0;
        } else if (length == 9 && static_cast<unsigned char>(name[3] - '0') <= 9U && static_cast<unsigned char>(name[4] - '0') <= 9U && name[5] == '.') {
            return 0;
        }
    }
    if (std::strstr(name, "prop") == name) {
        if (length == 8)
            return name[4] == '.' ? 0 : 2;
        if (length == 10) {
            if (static_cast<unsigned char>(name[4] - '0') <= 9U && static_cast<unsigned char>(name[5] - '0') <= 9U && name[6] == '.')
                return 0;
        } else if (length != 9 && length - 14 > 1) {
            return 2;
        }
    } else if (length != 10 && length != 9 && length - 14 > 1) {
        return 2;
    }
    if (name[3] != '_')
        return 2;
    const bool shortName = length == 9 || length == 14;
    const bool longName = length == 10 || length == 15;
    const char part = name[4];
    bool characterPart = false;
    if (shortName) {
        for (const char code : kModelPathCharacterParts) {
            characterPart |= part == code;
        }
    }
    if (longName) {
        for (const char(&pair)[2] : kModelPathCharacterPairs) {
            characterPart |= part == pair[0] && name[5] == pair[1];
        }
    }
    if (!characterPart)
        return 2;
    if (length - 14 > 1)
        return 1;
    const char *const head = name + (length != 14 ? 6 : 5);
    return head[0] == '_' && head[1] == 'h' && head[2] == 'e' && head[3] == 'a' && head[4] == 'd' ? 1 : 2;
}


bool SplitRemapPath(const char *path, const char *&name, const char *&dot) {
    std::size_t end = std::strlen(path);
    name = path;
    if (end > 1) {
        while (true) {
            if (path[end - 1] == '/' || path[end - 1] == '\\') {
                name = path + end;
                break;
            }
            if (end-- == 1 || end == 1)
                break;
        }
    }
    std::size_t cursor = std::strlen(path);
    if (cursor > 1) {
        while (static_cast<unsigned char>(path[cursor - 1] - '.') > 1U) {
            if (cursor-- == 1 || cursor == 1)
                break;
        }
    }
    dot = path[cursor - 1] == '.' ? path + cursor : nullptr;
    return dot != nullptr;
}


void RemapMapPath(const char *path, char *destination) {
    const char *name = nullptr;
    const char *dot = nullptr;
    if (!d3t::SplitRemapPath(path, name, dot))
        return;
    char stem[272]{};
    std::memcpy(stem, name, static_cast<std::size_t>(dot - name - 1));
    char stemDot[272]{};
    std::snprintf(stemDot, 260, "%s.", stem);
    const char *const stemAt = std::strstr(path, stemDot);
    if (!stemAt)
        return;
    const char *const scene = std::strstr(path, "scene");
    if (!scene)
        return;
    char map[272]{};
    char prefix[272]{};
    std::memcpy(map, scene + 9, static_cast<std::size_t>(stemAt - scene - 9));
    std::memcpy(prefix, path, static_cast<std::size_t>(scene - path));
    std::snprintf(destination, 260, "%s/Scene/%s/%s/%s/%s", prefix, "Maps", map, stem, name);
}


void RemapCharacterPath(const char *path, char *destination) {
    const char *name = nullptr;
    const char *dot = nullptr;
    if (!d3t::SplitRemapPath(path, name, dot))
        return;
    char stem[272]{};
    std::memcpy(stem, name, static_cast<std::size_t>(dot - name - 1));
    std::snprintf(destination, 260, "Scene/%s/%c%c%c/%s/%s", "Characters", name[0], name[1], name[2], stem, name);
}

// S2 has no miscellaneous suffix exceptions.
void RemapMiscPath(const char *path, char *destination) {
    const char *name = nullptr;
    const char *dot = nullptr;
    if (!d3t::SplitRemapPath(path, name, dot))
        return;
    char stem[272]{};
    std::memcpy(stem, name, static_cast<std::size_t>(dot - name - 1));
    const char *suffix = "";
#if SHENMUE_IS_S1
    for (const char *const(&entry)[2] : kModelPathMiscSuffixes) {
        if (std::strstr(path, entry[0])) {
            suffix = entry[1];
            break;
        }
    }
#endif
    std::snprintf(destination, 260, "Scene/%s/%s%s/%s", "Misc", stem, suffix, name);
}

} // namespace

std::int32_t RemapModelPath(const char *source, char *destination) {
    char cleaned[272]{};
    std::snprintf(cleaned, 0x104, "%s", source);
    FilepathCleaner(cleaned);
    const std::int32_t type = d3t::ClassifyRemapPath(cleaned);
    if (type == 0)
        d3t::RemapMapPath(cleaned, destination);
    else if (type == 1)
        d3t::RemapCharacterPath(cleaned, destination);
    else if (type == 2)
        d3t::RemapMiscPath(cleaned, destination);
    else
        std::strcpy(destination, cleaned);
    FilepathCleaner(destination);
    std::size_t end = std::strlen(destination);
    if (end > 1) {
        while (static_cast<unsigned char>(destination[end - 1] - '.') > 1U) {
            if (end-- == 1 || end == 1)
                break;
        }
    }
    char *const extension = destination[end - 1] == '.' ? destination + end : nullptr;
    if (extension && (std::memcmp(extension, "mt5", 4) == 0 || std::memcmp(extension, "mt7", 4) == 0))
        std::memcpy(extension, "fbx", 4);
    if (!std::strstr(destination, ".fbx")) {
        char *const dot = std::strrchr(destination, '.');
        char *const slash = std::strrchr(destination, '/');
        if (dot && slash && slash < dot) {
            char stem[272]{};
            const std::size_t length = static_cast<std::size_t>(dot - slash - 1);
            std::strncpy(stem, slash + 1, length);
            stem[length] = '\0';
            std::sprintf(destination + std::strlen(destination), "_/%s.fbx", stem);
        }
    }
    return type;
}

} // namespace shenmue::d3t
