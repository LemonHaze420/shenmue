#include "platform/dc/simm.hpp"

#if SHENMUE_IS_DC

#include "common/types.hpp"
#if SHENMUE_IS_S2
#include "platform/dc/am2_vector.hpp"
#endif
#if SHENMUE_S2_GCC295
#include <string.h>
#define SHENMUE_SIMM_MEMCPY ::memcpy
#define SHENMUE_SIMM_NULL 0
#else
#include <cstring>
#define SHENMUE_SIMM_MEMCPY std::memcpy
#define SHENMUE_SIMM_NULL 0
#endif

#if SHENMUE_IS_S2
namespace shenmue {
namespace s2 {
#else
namespace shenmue {
namespace s1 {
#endif

// @verified - d
std::int32_t SIMM_UppercaseAscii(std::int32_t value) {
    if (value >= 'a' && value <= 'z')
        value -= 'a' - 'A';
    return value;
}

// @verified - d
std::int32_t SIMM_CompareNames(const char *left, const char *right) {
    for (;;) {
        const std::int8_t a = static_cast<std::int8_t>(SIMM_UppercaseAscii(static_cast<std::int8_t>(*left++)));
        const std::int8_t b = static_cast<std::int8_t>(SIMM_UppercaseAscii(static_cast<std::int8_t>(*right++)));
        if (a > b)
            return 1;
        if (a < b)
            return -1;
        if (!a)
            return 0;
    }
}

std::int32_t SIMM_NamesEqual(const char *left, const char *right) {
    if (SIMM_CompareNames(left, right))
        return 0;
    return 1;
}

// @verified - d
const std::byte *SIMM_GetRoot(const void *buffer) {
    if (buffer) {
        union {
            char bytes[4];
            std::uint32_t value;
        } identifier;
        identifier.bytes[0] = 'S';
        identifier.bytes[1] = 'I';
        identifier.bytes[2] = 'M';
        identifier.bytes[3] = 'M';
        if (*static_cast<const std::uint32_t *>(buffer) == identifier.value)
            return static_cast<const std::byte *>(buffer) + 4;
    }
    return SHENMUE_SIMM_NULL;
}

// @verified - d
const SimmEntry *SIMM_FindEntry(const std::byte *table, const char *name) {
    while (*table != static_cast<std::byte>(0)) {
        const SimmEntry *const entry = reinterpret_cast<const SimmEntry *>(table);
        if (SIMM_NamesEqual(entry->name, name))
            return entry;
        table += sizeof(SimmEntry);
    }
    return SHENMUE_SIMM_NULL;
}

// @verified - d
const std::byte *SIMM_FindDirectory(const std::byte *root, const char *path) {
    if (!path)
        path = "";
    else if (*path == '/' || *path == '\\')
        ++path;
    const std::byte *table = root;
    while (*path && table) {
        char component[16];
        std::int32_t length;
        for (length = 0; length < 13; ++length, ++path) {
            component[length] = *path;
            if (!component[length])
                break;
            if (component[length] == '/' || component[length] == '\\') {
                ++path;
                break;
            }
        }
        component[length] = '\0';
        table = reinterpret_cast<const std::byte *>(SIMM_FindEntry(table, component));
        if (!table) {
            g_simmError = 1;
            break;
        }
        if (reinterpret_cast<const SimmEntry *>(table)->type != 1) {
            g_simmError = 1;
            table = SHENMUE_SIMM_NULL;
            break;
        }
        table = root + reinterpret_cast<const SimmEntry *>(table)->offset;
    }
    return table;
}

// @verified - d
const SimmEntry *SIMM_ResolveFile(const std::byte *root, const char *directory, const char *filename) {
    const void *entry = SIMM_FindDirectory(root, directory);
    if (entry)
        entry = SIMM_FindEntry(static_cast<const std::byte *>(entry), filename);
    return static_cast<const SimmEntry *>(entry);
}

// @verified - d
std::int32_t SIMM_GetEntrySize(const void *buffer, const char *directory, const char *filename, std::uint32_t *size) {
#if SHENMUE_IS_S2
    const std::byte *const root = SIMM_GetRoot(buffer);
    if (root) {
        const SimmEntry *const entry = SIMM_ResolveFile(root, directory, filename);
        if (entry) {
            const std::uint8_t type = entry->type;
            if (type == 3) {
                *size = am2ReadUnalignedLe32(root + entry->offset, entry->size);
                return type;
            }
            *size = entry->size;
            return 2;
        }
    }
    *size = 0;
    return 0;
#else
    const std::byte *const root = SIMM_GetRoot(buffer);
    const SimmEntry *const entry = root ? SIMM_ResolveFile(root, directory, filename) : SHENMUE_SIMM_NULL;
    if (!entry) {
        *size = 0;
        return 0;
    }
    if (entry->type == 3) {
        SHENMUE_SIMM_MEMCPY(size, root + entry->offset + entry->size - 4, sizeof(*size));
        return 3;
    }
    *size = entry->size;
    return 2;
#endif
}

// @verified - d
const std::byte *SIMM_GetEntryData(const void *buffer, const char *directory, const char *filename) {
    const std::byte *const root = SIMM_GetRoot(buffer);
    if (root) {
        const SimmEntry *const entry = SIMM_ResolveFile(root, directory, filename);
        if (entry)
            return root + entry->offset;
    }
    g_simmError = 2;
    return SHENMUE_SIMM_NULL;
}
}
} // namespace shenmue

#undef SHENMUE_SIMM_MEMCPY
#undef SHENMUE_SIMM_NULL

#endif
