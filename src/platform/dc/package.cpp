#include "platform/dc/package.hpp"
#if SHENMUE_IS_DC && (SHENMUE_IS_S1 || SHENMUE_IS_S2)
#include "platform/dc/simm.hpp"
#if SHENMUE_S2_GCC295
#include <ctype.h>
#include <string.h>
#define SHENMUE_PACK_STRLEN ::strlen
#define SHENMUE_PACK_STRCPY ::strcpy
#else
#include <cstring>
#define SHENMUE_PACK_STRLEN std::strlen
#define SHENMUE_PACK_STRCPY std::strcpy
#define SHENMUE_PACK_MEMCPY std::memcpy
#define SHENMUE_PACK_SWAP __builtin_bswap32
#define SHENMUE_PACK_SIGNED static_cast<std::int32_t>
#endif
#if SHENMUE_IS_S2
namespace shenmue {
namespace s2 {
#else
namespace shenmue {
namespace s1 {
#endif
#if SHENMUE_S2_GCC295
#define PACK_ENTRY_NAME_WORD(index, word) (reinterpret_cast<const volatile std::uint32_t *>(entries[index].name)[word])
#define PACK_SWAP_NAME_WORD(word) (((word) << 24U) | (((word) << 8U) & 0xff0000U) | (((word) >> 8U) & 0xff00U) | ((word) >> 24U))

std::int32_t PACK_FindNamedEntry(PakDirectory *directory, const char *filename) {
    std::int32_t low, high, middle, foundIndex;
    std::uint32_t nameHigh, nameLow;
    if (directory->entryCount <= 0 || SHENMUE_PACK_STRLEN(filename) > 15)
        return -1;

    union {
        char text[16];
        std::uint32_t words[4];
    } normalized;
    normalized.words[0] = 0;
    normalized.words[1] = 0;
    normalized.words[2] = 0;
    normalized.words[3] = 0;
    SHENMUE_PACK_STRCPY(normalized.text, filename);

    std::int32_t index = 0;
    std::int32_t ended = 0;
    for (; index < 15; ++index) {
        if (normalized.text[index] == 0 || normalized.text[index] == '.')
            ended = 1;
        if (ended == 0) {
            std::int32_t character = normalized.text[index];
            if (islower(character))
                character -= 'a' - 'A';
            normalized.text[index] = static_cast<char>(character);
        } else {
            normalized.text[index] = 0;
        }
    }

    nameHigh = PACK_SWAP_NAME_WORD(normalized.words[0]);
    nameLow = PACK_SWAP_NAME_WORD(normalized.words[1]);
    low = 0;
    high = directory->entryCount - 1;
    const PakDirectoryEntry *entries = reinterpret_cast<const PakDirectoryEntry *>(reinterpret_cast<const std::byte *>(directory) + directory->entriesOffset);
    while (low < high) {
        middle = (low + high) / 2;
        const std::int32_t highDifference = static_cast<std::int32_t>(PACK_SWAP_NAME_WORD(PACK_ENTRY_NAME_WORD(middle, 0)) - nameHigh);
        const std::int32_t lowDifference = static_cast<std::int32_t>(PACK_SWAP_NAME_WORD(PACK_ENTRY_NAME_WORD(middle, 1)) - nameLow);
        if (highDifference != 0) {
            if (highDifference < 0)
                low = middle + 1;
            else
                high = middle;
        } else {
            if (lowDifference == 0) {
                foundIndex = middle;
                goto found;
            }
            if (lowDifference < 0)
                low = middle + 1;
            else
                high = middle;
        }
    }

    if (PACK_SWAP_NAME_WORD(PACK_ENTRY_NAME_WORD(low, 0)) == nameHigh && PACK_SWAP_NAME_WORD(PACK_ENTRY_NAME_WORD(low, 1)) == nameLow) {
        foundIndex = low;
    found:
        return foundIndex;
    }
    return -1;
}
#undef PACK_SWAP_NAME_WORD
#undef PACK_ENTRY_NAME_WORD
#else
std::int32_t PACK_FindNamedEntry(PakDirectory *directory, const char *filename) {
    if (directory->entryCount <= 0 || SHENMUE_PACK_STRLEN(filename) >= 16)
        return -1;
    char normalized[16]{};
    SHENMUE_PACK_STRCPY(normalized, filename);
    bool ended = false;
    for (std::int32_t index = 0; index < 15; ++index) {
        if (!normalized[index] || normalized[index] == '.')
            ended = true;
        normalized[index] = ended ? 0 : static_cast<char>(SIMM_UppercaseAscii(static_cast<std::uint8_t>(normalized[index])));
    }
    std::uint32_t name[2];
    SHENMUE_PACK_MEMCPY(name, normalized, sizeof(name));
    name[0] = SHENMUE_PACK_SWAP(name[0]);
    name[1] = SHENMUE_PACK_SWAP(name[1]);
    const PakDirectoryEntry *const entries =
        reinterpret_cast<const PakDirectoryEntry *>(reinterpret_cast<const std::byte *>(directory) + directory->entriesOffset);
    std::int32_t low = 0;
    std::int32_t high = directory->entryCount - 1;
    while (low < high) {
        const std::int32_t middle = (low + high) / 2;
        std::uint32_t candidate[2];
        SHENMUE_PACK_MEMCPY(candidate, entries[middle].name, sizeof(candidate));
        std::int32_t comparison = SHENMUE_PACK_SIGNED(SHENMUE_PACK_SWAP(candidate[0]) - name[0]);
        if (!comparison)
            comparison = SHENMUE_PACK_SIGNED(SHENMUE_PACK_SWAP(candidate[1]) - name[1]);
        if (!comparison)
            return middle;
        if (comparison < 0)
            low = middle + 1;
        else
            high = middle;
    }
    std::uint32_t candidate[2];
    SHENMUE_PACK_MEMCPY(candidate, entries[low].name, sizeof(candidate));
    return SHENMUE_PACK_SWAP(candidate[0]) == name[0] && SHENMUE_PACK_SWAP(candidate[1]) == name[1] ? low : -1;
}

#endif
}
} // namespace shenmue
#undef SHENMUE_PACK_STRLEN
#undef SHENMUE_PACK_STRCPY
#undef SHENMUE_PACK_MEMCPY
#undef SHENMUE_PACK_SWAP
#undef SHENMUE_PACK_SIGNED
#endif
