#include "s1/hlib.hpp"
#include "s1/boot.hpp"
#include "s1/file_loader.hpp"
#include "s1/font.hpp"
#include "s1/motion_registry.hpp"
#include "s1/task/wexe.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>

namespace shenmue::s1 {

void InitSettings();
void InitSpriteBank(std::int32_t bank);
HLib::TaskData *SetupSTCH();
void LoadShadowMesh();
void SetupMoveShadow();
void ReadMOTI();
void AllocateWEDT();
void PreLoad_InitCycleman();
void SelectAudio();
void UIAnim_ClearSlot(std::int32_t index);

std::int32_t g_actNumber = 1;
void *g_mobjData = nullptr;
void *g_pSTCH = nullptr;
void *g_pSCST = nullptr;
void *g_LCDM[2]{};
void *g_hndb = nullptr;
std::uint32_t g_hndbSize = 0;
void *g_pFont = nullptr;
void *g_BGDT = nullptr;
void *g_FMOT = nullptr;
void *g_YKBE = nullptr;
void *g_shadowMeshData = nullptr;
void *g_motiData = nullptr;
std::uint32_t g_motiSize = 0;
std::uint32_t g_motiAnimationCount = 0;

namespace {

struct UiAnimationSlot {
    std::uint64_t field_00;
    std::uint32_t field_08;
    std::byte reserved_0c[12];
    std::uint32_t field_18;
    std::byte reserved_1c[4];
    std::uint64_t field_20;
    std::uint32_t field_28;
    std::byte reserved_2c[4];
    std::uint64_t field_30;
    std::uint64_t field_38;
    std::int32_t field_40;
    std::byte reserved_44[4];
};

struct UiAnimationBinding {
    std::uint64_t activeAndState;
    std::int32_t animationSlot;
    std::int32_t reserved_0c;
    std::uint64_t firstValue;
    std::uint64_t secondValue;
    std::uint64_t thirdValue;
};

struct UiIndexTableBlock {
    std::int32_t selectedIndex;
    std::int32_t count;
    std::int32_t indices[5][128];
};

static_assert(sizeof(UiAnimationSlot) == 0x48);
static_assert(sizeof(UiAnimationBinding) == 0x28);
static_assert(sizeof(UiIndexTableBlock) == 0xa08);

UiAnimationSlot g_uiAnimationSlots[256]{};
UiAnimationBinding g_uiAnimationBindings[256]{};
std::uint64_t g_uiDefaultPairs[5][8]{};
UiIndexTableBlock g_uiIndexTables[80]{};
std::byte g_directoryEntries[13][56]{};

std::int32_t readInt32(const std::byte *address) {
    std::int32_t value;
    std::memcpy(&value, address, sizeof(value));
    return value;
}

void *copyColdStorageBlock(const std::byte *base, std::int32_t selector, std::size_t offsetField, std::size_t sizeField, std::int32_t token) {
    const std::int32_t offset = readInt32(base + selector + offsetField);
    const std::int32_t signedSize = readInt32(base + selector + sizeField);
    const auto size = static_cast<std::size_t>(static_cast<std::int64_t>(signedSize));
    void *const destination = HLib_AllocHeapMemBlock(static_cast<std::int64_t>(size), token);
    std::memcpy(destination, base + offset, size);
    return destination;
}

} // namespace

void InitializeUiIndexTables() {
    for (UiIndexTableBlock &table : g_uiIndexTables) {
        table.selectedIndex = -1;
        table.count = 0;
        for (auto &indices : table.indices) {
            for (std::int32_t &index : indices) {
                index = -1;
            }
        }
    }
}

void UIAnim_ClearSlot(std::int32_t index) {
    UiAnimationBinding &binding = g_uiAnimationBindings[index];
    if (static_cast<std::uint32_t>(binding.activeAndState) != 0) {
        UiAnimationSlot &slot = g_uiAnimationSlots[binding.animationSlot];
        slot.field_00 = 0;
        slot.field_08 = 0;
        slot.field_18 = 0;
        slot.field_20 = 0;
        slot.field_28 = 0;
        slot.field_30 = 0;
        slot.field_38 = 0;
        slot.field_40 = -1;
    }
    binding.activeAndState = 0;
    binding.animationSlot = -1;
    binding.firstValue = 0;
    binding.secondValue = 0;
    binding.thirdValue = 0;
}

HLib::TaskData *SetupSTCH() {
    constexpr std::int32_t kStch = SHEN_NAME(STCH);
    constexpr std::int64_t kBlockSize = 0x4040;

    HLib::TaskData *block = g_pFirstTaskData;
    while (block && (block->status == HLIB_USED_TOKEN.name || block->content_size < kBlockSize)) {
        block = block->next_ptr;
    }

    if (!block) {
        g_pSTCH = nullptr;
        g_TotalSceneFileReadBytes = 0;
        return nullptr;
    }

    HLib::TaskData *const next = block->next_ptr;
    const std::uint64_t oldSize = static_cast<std::uint64_t>(block->content_size);
    if (oldSize > 0x4080) {
        auto *const remainder = reinterpret_cast<HLib::TaskData *>(reinterpret_cast<std::byte *>(block) + static_cast<std::size_t>(kBlockSize));
        remainder->prev_ptr = block;
        remainder->next_ptr = next;
        remainder->status = HLIB_FREE_TOKEN.name;
        remainder->alignment = 0;
        remainder->taskToken = HLIB_FREE_TOKEN.name;
        remainder->content_size = static_cast<std::int64_t>(oldSize - kBlockSize);
        block->next_ptr = remainder;
        if (next)
            next->prev_ptr = remainder;
        block->content_size = kBlockSize;
    }

    block->taskToken = kStch;
    block->status = HLIB_USED_TOKEN.name;
    block->alignment = 0;
    g_pSTCH = block + 1;
    g_TotalSceneFileReadBytes = 0;
    return block + 1;
}

void AllocFONTs() {
    constexpr std::int64_t kBlockSize = 0xa0;

    HLib::TaskData *block = g_pFirstTaskData;
    while (block && (block->status == HLIB_USED_TOKEN.name || block->content_size < kBlockSize)) {
        block = block->next_ptr;
    }

    std::byte *font = nullptr;
    if (block) {
        HLib::TaskData *const next = block->next_ptr;
        const std::uint64_t oldSize = static_cast<std::uint64_t>(block->content_size);
        if (oldSize > 0xe0) {
            auto *const remainder = reinterpret_cast<HLib::TaskData *>(reinterpret_cast<std::byte *>(block) + kBlockSize);
            remainder->prev_ptr = block;
            remainder->next_ptr = next;
            remainder->taskToken = HLIB_FREE_TOKEN.name;
            remainder->status = HLIB_FREE_TOKEN.name;
            remainder->alignment = 0;
            remainder->content_size = static_cast<std::int64_t>(oldSize - kBlockSize);
            block->next_ptr = remainder;
            if (next)
                next->prev_ptr = remainder;
            block->content_size = kBlockSize;
        }
        block->taskToken = HLIB_FONT_TOKEN.name;
        block->status = HLIB_USED_TOKEN.name;
        block->alignment = 1;
        font = reinterpret_cast<std::byte *>(block + 1);
    }

    g_pFont = font;
    std::uint64_t zero = 0;
    for (const std::size_t offset : {0x18U, 0x20U, 0x28U, 0x30U, 0x38U, 0x40U}) {
        std::memcpy(font + offset, &zero, sizeof(zero));
    }

    const auto *const mobj = static_cast<const std::byte *>(g_mobjData);
    const std::byte *const tableBase = mobj + 0x10;
    const std::int32_t selector = readInt32(mobj + 0x1c);
    void *const kana = copyColdStorageBlock(tableBase, selector, 0x70, 0x74, HLIB_FONT_TOKEN.name);
    void *const kanji = copyColdStorageBlock(tableBase, selector, 0x5c, 0x60, HLIB_FONT_TOKEN.name);
    void *const ryou = copyColdStorageBlock(tableBase, selector, 0x84, 0x88, HLIB_FONT_TOKEN.name);
    std::memcpy(font + 0x00, &kana, sizeof(kana));
    std::memcpy(font + 0x08, &kanji, sizeof(kanji));
    std::memcpy(font + 0x10, &ryou, sizeof(ryou));
    std::strcpy(reinterpret_cast<char *>(font + 0x28), "DC_KANA.FON");
    std::strcpy(reinterpret_cast<char *>(font + 0x18), "DC_KANJI.FON");
    std::strcpy(reinterpret_cast<char *>(font + 0x38), "RYOU.FON");
}

void LoadFont(const char *filename) {
    auto *const font = static_cast<std::byte *>(g_pFont);
    auto *const currentName = reinterpret_cast<char *>(font + 0x18);
    if (*currentName) {
        const char *left = filename;
        const char *right = currentName;
        for (;;) {
            char a = *left++;
            char b = *right++;
            if (a >= 'a' && a <= 'z')
                a = static_cast<char>(a - 32);
            if (b >= 'a' && b <= 'z')
                b = static_cast<char>(b - 32);
            if (a != b)
                break;
            if (!a)
                return;
        }
    }
    void *kanji = nullptr;
    std::memcpy(&kanji, font + 0x08, sizeof(kanji));
    shenLoadFile("misc", filename, kanji, 0x55c46);
    std::strcpy(currentName, filename);
}

void InitBGDT() {
    constexpr std::int32_t kBgdt = SHEN_NAME(BGDT);
    constexpr std::int64_t kBlockSize = 0x28840;

    HLib::TaskData *block = g_pLastTaskData;
    while (block && (block->status == HLIB_USED_TOKEN.name || block->content_size < kBlockSize)) {
        block = block->prev_ptr;
    }

    void *data = nullptr;
    if (block) {
        const std::uint64_t oldSize = static_cast<std::uint64_t>(block->content_size);
        if (oldSize > 0x28880) {
            const std::uint64_t remaining = oldSize - kBlockSize;
            auto *const used = reinterpret_cast<HLib::TaskData *>(reinterpret_cast<std::byte *>(block) + remaining);
            used->prev_ptr = block;
            used->next_ptr = block->next_ptr;
            block->next_ptr->prev_ptr = used;
            block->next_ptr = used;
            block->content_size = static_cast<std::int64_t>(remaining);
            used->taskToken = kBgdt;
            used->content_size = kBlockSize;
            used->status = HLIB_USED_TOKEN.name;
            used->alignment = 4;
            data = used + 1;
        } else {
            block->taskToken = kBgdt;
            block->status = HLIB_USED_TOKEN.name;
            block->alignment = 4;
            data = block + 1;
        }
    }
    g_BGDT = data;
    if (data) {
        (static_cast<HLib::TaskData *>(data) - 1)->alignment |= 1;
    }
}

void AllocateFMOTandYKBE() {
    const auto *const mobj = static_cast<const std::byte *>(g_mobjData);
    const std::byte *const tableBase = mobj + 0x10;
    const std::int32_t selector = readInt32(mobj + 0x1c);
    g_FMOT = copyColdStorageBlock(tableBase, selector, 0xd4, 0xd8, SHEN_NAME(FMOT));
    g_YKBE = copyColdStorageBlock(tableBase, selector, 0xc0, 0xc4, SHEN_NAME(YKBE));
    HLib_EnableTokenCheck(g_YKBE);
}

void LoadShadowMesh() {
    if (!g_mobjData)
        return;
    const auto *const mobj = static_cast<const std::byte *>(g_mobjData);
    const std::byte *const tableBase = mobj + 0x10;
    const std::int32_t selector = readInt32(mobj + 0x1c);
    g_shadowMeshData = copyColdStorageBlock(tableBase, selector, 0xe8, 0xec, SHEN_NAME(SHDS));
    if (g_shadowMeshData)
        (static_cast<HLib::TaskData *>(g_shadowMeshData) - 1)->alignment |= 1;
}

void ReadMOTI() {
    if (!g_mobjData)
        return;
    const auto *const mobj = static_cast<const std::byte *>(g_mobjData);
    const std::int32_t selector = readInt32(mobj + 0x1c);
    const std::int32_t offset = readInt32(mobj + selector + 0x1c);
    const std::int32_t size = readInt32(mobj + selector + 0x20);
    if (size <= 0)
        return;
    g_motiData = HLib_AllocMemBlock(static_cast<std::uint64_t>(size), SHEN_NAME(MOTI));
    if (g_motiData) {
        (static_cast<HLib::TaskData *>(g_motiData) - 1)->alignment |= 1;
    }
    std::memcpy(g_motiData, mobj + 0x10 + offset, static_cast<std::size_t>(size));
    g_motiSize = static_cast<std::uint32_t>(size);
    if (size >= 8) {
        std::memcpy(&g_motiAnimationCount, static_cast<const std::byte *>(g_motiData) + 4, sizeof(g_motiAnimationCount));
    }
    MOTN_SetBaseMotionData(g_motiData);
}

std::uint8_t *LookupDirectoryEntry(const char *path, std::int64_t, std::int32_t allocationCount) {
    char canonicalPath[64];
    canonicalPath[0] = '/';
    std::size_t output = 1;
    if (path) {
        while (*path) {
            while (*path == '/') {
                ++path;
                if (!*path) {
                    break;
                }
            }
            while (*path && *path != '/') {
                char value = *path++;
                if (value >= 'a' && value <= 'z') {
                    value = static_cast<char>(value - ('a' - 'A'));
                }
                canonicalPath[output++] = value;
            }
            if (*path == '/' && path[1]) {
                canonicalPath[output++] = '/';
            }
        }
    }
    canonicalPath[output] = '\0';

    std::uint8_t *comparison = nullptr;
    for (auto &rawEntry : g_directoryEntries) {
        comparison = reinterpret_cast<std::uint8_t *>(rawEntry);
        const auto *expected = reinterpret_cast<const std::uint8_t *>(canonicalPath);
        while (*comparison == *expected) {
            if (!*expected) {
                return comparison;
            }
            ++comparison;
            ++expected;
        }
    }

    const std::int32_t count = allocationCount >= 1 ? allocationCount : 1;
    if (count <= 512) {
        std::int64_t size = static_cast<std::int64_t>(count) * 44 + 16;
        const std::uint32_t iterations = static_cast<std::uint32_t>(512 - count) / 25 + 1;
        std::uint64_t result = 0;
        for (std::uint32_t index = 0; index < iterations; ++index) {
            void *const allocation = HLib_AllocHeapMemBlock(size, SHEN_NAME(DIRE));
            result = HLib_FreeTaskData(static_cast<HLib::TaskData *>(allocation), 0);
            size += 1100;
        }
        return reinterpret_cast<std::uint8_t *>(result);
    }
    return comparison;
}

void ReadColdStorage() {
    HLib::TaskData *block = g_pFirstTaskData;
    while (block && (block->status == 0x20455355 || block->content_size < 0x600040)) {
        block = block->next_ptr;
    }

    void *destination = nullptr;
    if (block) {
        HLib::TaskData *const next = block->next_ptr;
        const std::uint64_t oldSize = static_cast<std::uint64_t>(block->content_size);
        if (oldSize > 0x600080) {
            auto *const remainder = reinterpret_cast<HLib::TaskData *>(reinterpret_cast<std::byte *>(block) + 0x600040);
            remainder->prev_ptr = block;
            remainder->next_ptr = next;
            remainder->status = SHEN_NAME(fREe);
            remainder->alignment = 0;
            remainder->taskToken = SHEN_NAME(fREe);
            remainder->content_size = static_cast<std::int64_t>(oldSize - 0x600040);
            block->next_ptr = remainder;
            if (next) {
                next->prev_ptr = remainder;
            }
            block->content_size = 0x600040;
        }
        block->taskToken = SHEN_NAME(COLD);
        block->status = 0x20455355;
        block->alignment = 0;
        destination = block + 1;
    }
    g_mobjData = destination;
    if (!destination || !shenLoadFile("misc", "COLD.BIN", destination, -1)) {
        if (destination) {
            HLib_FreeTaskData(static_cast<HLib::TaskData *>(destination), 0);
        }
        g_mobjData = nullptr;
    }
}

void InitMain() {
    InitSettings();
    InitSpriteBank(1);

    char path[256];
    LookupDirectoryEntry("/MISC", 0, 0);
    LookupDirectoryEntry("/MODEL/OBJECT", 0, 0);
    std::sprintf(path, "/scene/%02d/FREE", g_actNumber);
    LookupDirectoryEntry(path, 0, 0);
    std::sprintf(path, "/scene/%02d/MODEL/CHARA", g_actNumber);
    LookupDirectoryEntry(path, 0, 1);
    std::sprintf(path, "/scene/%02d/SOUND", g_actNumber);
    LookupDirectoryEntry(path, 0, 0);
    std::sprintf(path, "/scene/%02d/STREAM", g_actNumber);
    LookupDirectoryEntry(path, 0, 0);
    LookupDirectoryEntry("/SPRITE", 0, 0);
    std::sprintf(path, "/scene/%02d/D000", g_actNumber);
    LookupDirectoryEntry(path, 0, 0);
    std::sprintf(path, "/scene/%02d/JOMO", g_actNumber);
    LookupDirectoryEntry(path, 0, 0);
    std::sprintf(path, "/scene/%02d/JHD0", g_actNumber);
    LookupDirectoryEntry(path, 0, 0);

    ReadColdStorage();
    if (!g_mobjData)
        return;
    SetupSTCH();
    LoadShadowMesh();
    SetupMoveShadow();
    ReadMOTI();
    AllocFONTs();

    g_cloudNode = nullptr;
    g_rainNode = nullptr;
    g_weatherMode = 0;
    g_wdatRoot = nullptr;
    g_snowNode = nullptr;
    g_cloudCount = 0;
    g_rainCount = 0;
    g_snowCount = 0;
    g_forecastNode = nullptr;
    g_forecastCount = 0;
    AllocateWEDT();

    auto *const mobj = static_cast<std::byte *>(g_mobjData);
    const std::byte *const tableBase = mobj + 0x10;
    const std::int32_t selector = readInt32(mobj + 0x1c);
    g_pSCST = copyColdStorageBlock(tableBase, selector, 0x20, 0x24, SHEN_NAME(SCST));
    g_LCDM[1] = copyColdStorageBlock(tableBase, selector, 0x34, 0x38, SHEN_NAME(LCDM));
    AllocateFMOTandYKBE();
    g_hndb = copyColdStorageBlock(tableBase, selector, 0xac, 0xb0, SHEN_NAME(HNDB));
    if (g_hndb) {
        const std::int32_t size = readInt32(tableBase + selector + 0xb0);
        g_hndbSize = size > 0 ? static_cast<std::uint32_t>(size) : 0;
    }

    InitBGDT();
    HLib_FreeTaskData(static_cast<HLib::TaskData *>(g_mobjData), 0);
    PreLoad_InitCycleman();
    SelectAudio();
    UpdateTaskData_0();

    HLib::TaskData *const arenaBase = g_pFirstTaskData->prev_ptr;
    if ((arenaBase->alignment & 0x80) != 0 && (g_pLastTaskData->alignment & 0x80) != 0) {
        HLib_Init();
    } else {
        HLib::TaskData *const freeBlock = arenaBase->next_ptr;
        freeBlock->taskToken = SHEN_NAME(fREe);
        freeBlock->content_size = reinterpret_cast<std::byte *>(g_pLastTaskData) - reinterpret_cast<std::byte *>(freeBlock);
        freeBlock->next_ptr = g_pLastTaskData;
        freeBlock->status = SHEN_NAME(fREe);
        freeBlock->alignment = 0;
        g_pLastTaskData->prev_ptr = freeBlock;
    }

    for (std::int32_t index = 0; index < 256; ++index) {
        UiAnimationSlot &slot = g_uiAnimationSlots[index];
        slot.field_00 = 0;
        slot.field_08 = 0;
        slot.field_18 = 0;
        slot.field_20 = 0;
        slot.field_28 = 0;
        slot.field_30 = 0;
        slot.field_38 = 0;
        slot.field_40 = -1;
        UIAnim_ClearSlot(index);
    }
    for (auto &row : g_uiDefaultPairs) {
        for (std::uint64_t &value : row) {
            value = 0x400000004ULL;
        }
    }
    InitializeUiIndexTables();
}

} // namespace shenmue::s1
