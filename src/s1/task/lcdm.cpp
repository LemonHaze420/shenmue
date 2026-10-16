#include "s1/task/lcdm.hpp"
#include "common/types.hpp"
#include "common/utility.hpp"

#include <bit>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace shenmue::s1 {
void HLib_InertTaskCallback(HLib::TaskData *taskData);

extern void *g_LCDM[2];



const std::uint8_t *const lcdmPackedBitmapTable[3]{};
std::uint8_t g_lcdmBlankBitmap[194]{};

namespace {
void ConfigureTaskForDestruction(HLib::Task *task) {
    if (!task)
        return;
    task->initCallbackFnPtr = nullptr;
    task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
    task->autoInitTaskCallbackFnPtr = nullptr;
}

#pragma pack(push, 1)
struct LcdmArchiveEntry {
    std::uint32_t name0;
    std::uint32_t name1;
    std::uint32_t field_08;
    std::int32_t dataOffset;
};
#pragma pack(pop)

static_assert(sizeof(LcdmArchiveEntry) == 0x10);

constexpr std::array<std::int32_t, 6> lcdmTextStartOffsets{0, 19, 13, 8, 2, 0};

constexpr std::uint32_t Tag4(char a, char b, char c, char d) {
    return make_shen_name(a, b, c, d);
}

constexpr std::uint32_t Tag4(const char (&name)[9], std::size_t offset) {
    return Tag4(name[offset], name[offset + 1], name[offset + 2], name[offset + 3]);
}

void WriteBigEndian16(std::uint8_t *destination, std::uint16_t value) {
    destination[0] = static_cast<std::uint8_t>(value >> 8);
    destination[1] = static_cast<std::uint8_t>(value);
}

const std::uint8_t *FindArchiveEntry(std::uint32_t name0, std::uint32_t name1) {
    auto *const archive = static_cast<std::uint8_t *>(g_LCDM[1]);
    if (!archive)
        return nullptr;
    const std::int32_t count = read_unaligned<std::int32_t>(archive + 4);
    const std::int32_t tableOffset = read_unaligned<std::int32_t>(archive + 8);
    if (count <= 0 || tableOffset < 0)
        return nullptr;
    auto *entry = reinterpret_cast<const LcdmArchiveEntry *>(archive + tableOffset);
    for (std::int32_t remaining = count; remaining; --remaining, ++entry) {
        if (entry->name0 == name0 && entry->name1 == name1) {
            return archive + entry->dataOffset;
        }
    }
    return nullptr;
}

const std::uint8_t *FindArchiveEntry(const char (&name)[9]) {
    return FindArchiveEntry(Tag4(name, 0), Tag4(name, 4));
}

void CopyBigNumberGlyphs(LcdmTaskData *lcdm, const std::array<const std::uint8_t *, 10> &entries) {
    for (std::size_t index = 0; index < entries.size(); ++index) {
        if (!entries[index])
            continue;
        const std::uint8_t *source = entries[index] + 2;
        std::uint8_t *const first = lcdm->largeDigitsHigh[index];
        std::uint8_t *const second = lcdm->largeDigitsLow[index];
        for (std::int32_t row = 0; row < 22; ++row) {
            const std::uint16_t bits = static_cast<std::uint16_t>(source[1] | (static_cast<std::uint16_t>(source[0]) << 8));
            WriteBigEndian16(first + 2 * row, static_cast<std::uint16_t>(bits >> 4));
            WriteBigEndian16(second + 2 * row, static_cast<std::uint16_t>(bits >> 6));
            source += 2;
        }
    }
}

void CopySmallNumberGlyphs(LcdmTaskData *lcdm, const std::array<const std::uint8_t *, 10> &entries) {
    for (std::size_t index = 0; index < entries.size(); ++index) {
        if (!entries[index])
            continue;
        const std::uint8_t *source = entries[index];
        std::uint8_t *const first = lcdm->smallDigitsHigh[index];
        std::uint8_t *const second = lcdm->smallDigitsLow[index];
        for (std::int32_t row = 0; row < 10; ++row) {
            const std::uint8_t value = source[row + 2];
            WriteBigEndian16(first + 2 * row, static_cast<std::uint16_t>((32 * value) & 0x1fff));
            WriteBigEndian16(second + 2 * row, static_cast<std::uint16_t>(value << 7));
        }
    }
}

// Each suffix writes ten rows into a 16-byte slot, so the last one overwrites the low half of buttonGlyphs[0] as natively.
void CopyEndGlyphs(LcdmTaskData *lcdm, const std::array<const std::uint8_t *, 4> &entries) {
    auto *destination = reinterpret_cast<std::uint8_t *>(lcdm) + offsetof(LcdmTaskData, ordinalSuffixes) + 18;
    for (const std::uint8_t *source : entries) {
        destination += 16;
        if (!source)
            continue;
        for (std::int32_t row = 0; row < 10; ++row) {
            WriteBigEndian16(destination - 34 + 2 * row, static_cast<std::uint16_t>(source[row + 2] << 1));
        }
    }
}

const std::uint8_t *ResolveBitmap(LcdmTaskData *lcdm, std::uintptr_t bitmap) {
    if (bitmap >= 0x10000) {
        return reinterpret_cast<const std::uint8_t *>(bitmap);
    }
    return lcdm->packedEntries[bitmap];
}

} // namespace

void LCDM_APICode() {
    LcdmTaskData *const lcdm = g_lcdmTaskData;
    if (!lcdm) {
        return;
    }

    std::memset(lcdm, 0, sizeof(*lcdm));
    lcdm->archive = g_LCDM[1];
    lcdm->field_61c = 0x2030;

    lcdm->packedEntries[0] = FindArchiveEntry("SHENMUE ");
    lcdm->packedEntries[1] = FindArchiveEntry("PK      ");
    lcdm->packedEntries[2] = FindArchiveEntry("LOAD_JP ");
    lcdm->packedEntries[3] = FindArchiveEntry("LOAD_US ");
    lcdm->packedEntries[4] = FindArchiveEntry("SAVE_JP ");
    lcdm->packedEntries[5] = FindArchiveEntry("SAVE_US ");
    lcdm->packedEntries[6] = FindArchiveEntry("OK_JP   ");
    lcdm->packedEntries[7] = FindArchiveEntry("OK_US   ");
    lcdm->packedEntries[8] = FindArchiveEntry("NG_JP   ");
    lcdm->packedEntries[9] = FindArchiveEntry("NG_US   ");
    lcdm->packedEntries[10] = FindArchiveEntry("RSM_JP  ");
    lcdm->packedEntries[11] = FindArchiveEntry("RSM_US  ");
    lcdm->packedEntries[12] = FindArchiveEntry("RSM_OK  ");
    lcdm->packedEntries[13] = FindArchiveEntry("RSM_NG  ");
    lcdm->startText = FindArchiveEntry("BT_START");
    lcdm->countText = FindArchiveEntry("BT_COUNT");
    lcdm->endText = FindArchiveEntry("BT_END  ");

    lcdm->buttonGlyphs[0] = FindArchiveEntry("WAZA_KB ");
    lcdm->buttonGlyphs[1] = FindArchiveEntry("WAZA_KF ");
    lcdm->buttonGlyphs[2] = FindArchiveEntry("WAZA_BA ");
    lcdm->buttonGlyphs[3] = FindArchiveEntry("WAZA_BB ");
    lcdm->buttonGlyphs[4] = FindArchiveEntry("WAZA_BX ");
    lcdm->buttonGlyphs[5] = FindArchiveEntry("WAZA_BY ");
    lcdm->buttonGlyphs[6] = FindArchiveEntry("WAZA_BL ");
    lcdm->buttonGlyphs[7] = FindArchiveEntry("WAZA_BR ");
    lcdm->buttonGlyphs[8] = FindArchiveEntry("WAZA_BP ");
    lcdm->glyphTable[0] = lcdm->buttonGlyphs[4];
    lcdm->glyphTable[1] = lcdm->buttonGlyphs[2];
    lcdm->glyphTable[2] = lcdm->buttonGlyphs[1];
    lcdm->glyphTable[3] = lcdm->buttonGlyphs[0];
    lcdm->glyphTable[4] = lcdm->buttonGlyphs[6];
    lcdm->glyphTable[5] = lcdm->buttonGlyphs[3];
    lcdm->glyphTable[6] = lcdm->buttonGlyphs[5];
    lcdm->glyphTable[7] = lcdm->buttonGlyphs[8];
    for (std::uint8_t index = 0; index < 8; ++index)
        lcdm->buttonLookup[index] = index;

    std::array<const std::uint8_t *, 10> smallNumbers{};
    std::array<const std::uint8_t *, 10> largeNumbers{};
    for (std::int32_t index = 0; index < 10; ++index) {
        const std::uint32_t suffix = Tag4(static_cast<char>('0' + index), ' ', ' ', ' ');
        smallNumbers[index] = FindArchiveEntry(Tag4('B', 'T', '_', 'S'), suffix);
        largeNumbers[index] = FindArchiveEntry(Tag4('B', 'T', '_', 'L'), suffix);
    }
    const std::array<const std::uint8_t *, 4> endings{
        FindArchiveEntry("ST_E    "), FindArchiveEntry("ND_E    "), FindArchiveEntry("RD_E    "), FindArchiveEntry("TH_E    ")};
    CopyBigNumberGlyphs(lcdm, largeNumbers);
    CopySmallNumberGlyphs(lcdm, smallNumbers);
    CopyEndGlyphs(lcdm, endings);
    std::memset(lcdm->largeDigitsHigh[0], 0, sizeof(lcdm->largeDigitsHigh[0]));
    std::memset(lcdm->smallDigitsHigh[0], 0, sizeof(lcdm->smallDigitsHigh[0]));
}

void lcdmInit(std::uintptr_t bitmap) {
    LcdmTaskData *const lcdm = g_lcdmTaskData;
    if (!lcdm)
        return;

    const std::uint8_t *source = ResolveBitmap(lcdm, bitmap) + 2;
    std::uint8_t *destination = lcdm->pixels;
    for (std::int32_t index = 0; index < 192; ++index) {
        const std::uint8_t value = *source++;
        destination[0] = (value >> 4) & 8;
        destination[1] = (value >> 3) & 8;
        destination[2] = (value >> 2) & 8;
        destination[3] = (value >> 1) & 8;
        destination[4] = value & 8;
        destination[5] = 2 * (value & 4);
        destination[6] = 4 * (value & 2);
        destination[7] = 8 * (value & 1);
        destination += 8;
    }
    lcdm->field_018 = 1;
}

// Composes the 48x32 fight score screen: BT_COUNT, the small round number with its ordinal suffix and the large defeat count.
void lcdmSetValues(std::int32_t round, std::int32_t defeatedEnemies) {
    LcdmTaskData *const lcdm = g_lcdmTaskData;
    if (!lcdm)
        return;

    std::uint8_t *const bitmap = reinterpret_cast<std::uint8_t *>(lcdm) + offsetof(LcdmTaskData, field_61c);
    std::uint8_t *const rows = bitmap + 2;
    std::memcpy(rows, lcdm->countText + 2, 192);
    if (round) {
        const std::int32_t ones = round % 10;
        const std::uint8_t *const high = lcdm->smallDigitsHigh[round / 10];
        const std::uint8_t *const low = lcdm->smallDigitsLow[ones];
        for (std::int32_t row = 0; row < 10; ++row) {
            std::uint8_t *const destination = rows + 15 + 6 * row;
            destination[0] = high[2 * row];
            destination[1] = high[2 * row + 1];
            destination[1] |= low[2 * row];
            destination[2] |= low[2 * row + 1];
        }
        std::int32_t suffix = 3;
        if (static_cast<std::uint32_t>(round - 11) > 8U) {
            if (ones == 1)
                suffix = 0;
            else if (ones == 2)
                suffix = 1;
            else if (ones == 3)
                suffix = 2;
        }
        const std::uint8_t *const ordinal = lcdm->ordinalSuffixes[suffix];
        for (std::int32_t row = 0; row < 8; ++row) {
            rows[0x1c + 6 * row] |= ordinal[2 * row];
            rows[0x1d + 6 * row] |= ordinal[2 * row + 1];
        }
    } else {
        for (std::int32_t row = 0; row < 7; ++row) {
            rows[0x22 + 6 * row] = 0;
            rows[0x23 + 6 * row] = 0;
        }
    }

    const std::uint8_t *const high = lcdm->largeDigitsHigh[defeatedEnemies / 10];
    const std::uint8_t *const low = lcdm->largeDigitsLow[defeatedEnemies % 10];
    for (std::int32_t row = 0; row < 22; ++row) {
        std::uint8_t *const destination = rows + 6 + 6 * row;
        destination[0] = high[2 * row];
        destination[1] = high[2 * row + 1];
        destination[1] |= low[2 * row];
        destination[2] |= low[2 * row + 1];
    }
    if (defeatedEnemies < 10) {
        for (std::int32_t row = 0; row < 23; ++row) {
            std::uint8_t *const line = rows + 6 * row;
            line[0] = static_cast<std::uint8_t>((line[0] << 3) | (line[1] >> 5));
            line[1] = static_cast<std::uint8_t>((line[1] << 3) | (line[2] >> 5));
            line[2] = static_cast<std::uint8_t>((line[2] << 3) | (line[3] >> 5));
        }
    }
    if (!round) {
        for (std::int32_t index = 193; index >= 3; --index)
            bitmap[index] = static_cast<std::uint8_t>((bitmap[index - 2] << 6) | (bitmap[index - 1] >> 2));
    }
    lcdmInit(reinterpret_cast<std::uintptr_t>(bitmap));
}

void lcdmDrawButtonGlyph(std::int32_t x, std::int32_t y, const std::uint8_t *glyph) {
    const std::int32_t width = glyph[0];
    const std::int32_t height = glyph[1];
    const std::uint8_t *source = glyph + 2;
    std::uint8_t *destination = g_lcdmTaskData->pixels + 48 * y + x;
    const std::int32_t packedWidth = (width + 7) >> 3;
    for (std::int32_t row = 0; row < height; ++row) {
        std::int32_t remaining = width;
        for (std::int32_t packedByte = 0; packedByte < packedWidth; ++packedByte) {
            for (std::int32_t bit = 0; remaining && bit < 8; ++bit, --remaining)
                *destination++ = (*source & (0x80 >> bit)) != 0 ? 8 : 0;
            ++source;
        }
        destination += 48 - width;
    }
}

void lcdmRenderButtonGlyphs(const std::uint8_t *glyphList) {
    LcdmTaskData *const lcdm = g_lcdmTaskData;
    if (!lcdm)
        return;

    std::array<const std::uint8_t *, 6> glyphs{};
    const std::int32_t count = static_cast<std::int8_t>(glyphList[0]);
    for (std::int32_t index = 0; index < count; ++index)
        glyphs[index] = lcdm->glyphTable[glyphList[index + 1]];
    std::memset(lcdm->pixels, 0, sizeof(lcdm->pixels));
    if (count >= 5) {
        lcdmDrawButtonGlyph(8, 1, glyphs[0]);
        lcdmDrawButtonGlyph(19, 1, glyphs[1]);
        std::int32_t x = lcdmTextStartOffsets[count - 2];
        for (std::int32_t index = 2; index < count; ++index, x += 11)
            lcdmDrawButtonGlyph(x, 16, glyphs[index]);
    } else {
        std::int32_t x = lcdmTextStartOffsets[count];
        for (std::int32_t index = 0; index < count; ++index, x += 11)
            lcdmDrawButtonGlyph(x, 9, glyphs[index]);
    }
    lcdm->field_018 = 1;
}

void lcdmRenderButtonCommands(const std::int32_t *commands) {
    std::array<std::uint8_t, 8> glyphList{};
    for (; *commands != 8; ++commands) {
        glyphList[glyphList[0] + 1] = g_lcdmTaskData->buttonLookup[*commands];
        ++glyphList[0];
    }
    lcdmRenderButtonGlyphs(glyphList.data());
}

void Script_LCDM_Utils(ScenarioContext *, void *rawParams) {
    LcdmTaskData *const lcdm = g_lcdmTaskData;
    const auto *const params = static_cast<const LcdmUtilityParameters *>(rawParams);
    if (!lcdm)
        return;

    const std::int32_t offset = static_cast<std::int32_t>(params->dataOffset);
    std::byte *const target = offset ? g_pCurrentTaskData + offset : nullptr;
    switch (params->mode) {
    case 0:
        lcdmInit(reinterpret_cast<std::uintptr_t>(target));
        break;
    case 1:
        lcdm->field_000 = 1;
        if (reinterpret_cast<std::uintptr_t>(target) < 0x10000)
            lcdm->data = lcdmPackedBitmapTable[reinterpret_cast<std::uintptr_t>(target)];
        else
            lcdm->data = reinterpret_cast<const std::uint8_t *>(target);
        break;
    case 2:
        lcdmRenderButtonCommands(reinterpret_cast<const std::int32_t *>(target));
        break;
    }
}

HLib::Task *g_lcdmTask = nullptr;
LcdmTaskData *g_lcdmTaskData = nullptr;

std::int64_t LCDM_Init() {
    g_lcdmTask = nullptr;
    g_lcdmTaskData = nullptr;
    g_LCDM[0] = nullptr;

    HLib::Task *const task = HLib_EnqueueTaskWithoutParameter(HLib_InertTaskCallback, 2, 2, SHEN_NAME(LCDM));
    if (!task || task->task_data_ptr) {
        ConfigureTaskForDestruction(task);
        return 0;
    }

    auto *const data = static_cast<LcdmTaskData *>(HLib_AllocHeapMemBlock(sizeof(LcdmTaskData), SHEN_NAME(TASK)));
    if (!data) {
        ConfigureTaskForDestruction(task);
        return 0;
    }
    HLib_EnableTokenCheck(data);
    task->task_data_ptr = reinterpret_cast<HLib::TaskData *>(data);
    g_lcdmTask = task;
    g_lcdmTaskData = data;
    g_LCDM[0] = task;
    LCDM_APICode();
    if (!data->archive) {
        LCDM_Shutdown();
        return 0;
    }
    lcdmInit(0);
    return 1;
}
void LCDM_Shutdown() {
    HLib::Task *const task = g_lcdmTask;
    g_lcdmTask = nullptr;
    g_lcdmTaskData = nullptr;
    g_LCDM[0] = nullptr;
    if (task)
        HLib_DestroyTask(task);
}

} // namespace shenmue::s1
