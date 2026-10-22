#include "s1/player_stats.hpp"
#include "s1/item_catalog.hpp"
#include "s1/d3t/settings.hpp"
#include "s1/player_control.hpp"
#include "s1/save_format.hpp"
#include "s1/task/mmtk.hpp"
#include "s1/task/modu.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/fbgg.hpp"
#include "s1/task/sygh.hpp"
#include "s1/task/vfma.hpp"
#include "s1/ui_runtime.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <cstring>

namespace shenmue::s1 {
extern std::int32_t g_debugForceShowAllItems;
extern std::array<std::uint32_t, 32> g_arcadeHighScores;

namespace {

std::uint8_t ClampU8(std::uint32_t value) {
    return static_cast<std::int32_t>(value) > 0xff ? 0xff : static_cast<std::uint8_t>(value);
}

std::uint16_t ClampU16(std::uint32_t value) {
    return static_cast<std::int32_t>(value) > 0xffff ? 0xffff : static_cast<std::uint16_t>(value);
}

std::uint32_t CharaNameToIndex(std::uint32_t name) {
    const auto found = std::find(kValidCharaNames.begin(), kValidCharaNames.end(), name);
    return found == kValidCharaNames.end() ? 0u : static_cast<std::uint32_t>(found - kValidCharaNames.begin());
}
std::uint8_t *InventoryItemCount(std::uint16_t itemId) {
    if (itemId < 67) {
        return &g_playerStats.inventoryItemCounts0To66[itemId];
    }
    if (itemId <= 234) {
        return &g_playerStats.inventoryItemCounts67To234[itemId - 67];
    }
    return nullptr;
}

void RegisterInventoryItem(std::int16_t itemId) {
    const auto begin = g_playerStats.inventoryItemIds.begin();
    const auto end = begin + g_playerStats.inventoryItemCount;
    if (std::find(begin, end, itemId) != end || g_playerStats.inventoryItemCount >= static_cast<std::int16_t>(g_playerStats.inventoryItemIds.size())) {
        return;
    }
    g_playerStats.inventoryItemIds[g_playerStats.inventoryItemCount++] = itemId;
}

void UnlockMoveSkillFromInventoryItem(std::uint16_t itemId) {
    if (itemId == ITEM_SCROLL_STAB_ARMOR)
        return;
    struct UnlockEntry {
        std::uint8_t itemId;
        std::uint8_t encodedMoveId;
    };
    static constexpr std::array<UnlockEntry, 9> kUnlocks{{
        {17, 17},
        {19, 49},
        {21, 18},
        {23, 48},
        {25, 47},
        {27, 16},
        {29, 46},
        {31, 75},
        {33, 74},
    }};
    const auto found = std::find_if(kUnlocks.begin(), kUnlocks.end(), [itemId](const UnlockEntry &entry) { return entry.itemId == itemId; });
    if (found == kUnlocks.end())
        return;
    const std::uint8_t animationSet = found->encodedMoveId >= 64 ? 3 : found->encodedMoveId >= 32 ? 2 : 1;
    const std::uint8_t base = animationSet == 3 ? 64 : animationSet == 2 ? 32 : 0;
    SYGH_SetMoveSkill(animationSet, static_cast<std::uint16_t>(found->encodedMoveId - base), 0);
}

void TrackUniqueInventoryItem(std::int16_t itemId) {
    if (g_playerStats.recentItemKeyOldest == itemId || g_playerStats.recentItemKeyPrevious == itemId || g_playerStats.recentItemKeyLatest == itemId ||
        g_playerStats.scenarioStepUniqueItemCount >= 32000) {
        return;
    }

    g_playerStats.recentItemKeyOldest = g_playerStats.recentItemKeyPrevious;
    g_playerStats.recentItemKeyPrevious = g_playerStats.recentItemKeyLatest;
    g_playerStats.recentItemKeyLatest = itemId;
    ++g_playerStats.scenarioStepUniqueItemCount;
}

PlayerStatSparsePairSlot &GetOrCreateSparsePair(std::uint8_t key0, std::uint8_t key1) {
    auto &slots = g_playerStats.pages.sparsePairU8Slots;
    for (;;) {
        for (std::size_t index = 0; index < std::size(slots); ++index) {
            auto &slot = slots[index];
            if (slot.key0 == key0 && slot.key1 == key1)
                return slot;

            if (slot.key0 != 0xff)
                continue;

            slot.key0 = key0;
            slot.key1 = key1;
            if (index + 1 < std::size(slots))
                slots[index + 1].key0 = 0xff;
            else
                g_playerStats.pages.sparsePairTerminatorPadding[0] = 0xff;
            return slot;
        }
        slots[0].key0 = 0xff;
    }
}

void GrantSpotVisitRewards(std::uint32_t spot) {
    switch (spot) {
    case 15:
    case 19:
        RegisterInventoryItem(702);
        RegisterInventoryItem(703);
        RegisterInventoryItem(704);
        break;
    case 79:
        RegisterInventoryItem(251);
        break;
    case 90:
        RegisterInventoryItem(203);
        RegisterInventoryItem(284);
        break;
    case 92:
        RegisterInventoryItem(208);
        RegisterInventoryItem(211);
        break;
    case 99:
        RegisterInventoryItem(704);
        RegisterInventoryItem(125);
        RegisterInventoryItem(253);
        break;
    case 100:
        RegisterInventoryItem(701);
        RegisterInventoryItem(705);
        RegisterInventoryItem(802);
        RegisterInventoryItem(803);
        RegisterInventoryItem(118);
        break;
    default:
        break;
    }
}

} // namespace

std::array<PlayerStatShadowRecord, 4> g_playerStatShadowRecords{};

PlayerStatsRuntimeState g_playerStats = [] {
    PlayerStatsRuntimeState state{};
    state.pages.sparsePairU8Slots[0].key0 = 0xff;
    for (CassetteTapeState &tape : state.cassetteTapes)
        tape.id = 0xff;
    return state;
}();

void RegisterInventoryItemGroup(std::uint16_t groupId) {
    const std::int16_t *entry = kInventoryGroupTable;
    while (*entry >= 0) {
        const auto group = static_cast<std::uint16_t>(*entry++);
        if (group == groupId) {
            while (*entry >= 0) {
                const std::int16_t itemId = *entry++;
                if (itemId < 500 && g_playerStats.pages.charaItemCounts[static_cast<std::size_t>(itemId)] == 0) {
                    g_playerStats.pages.charaItemCounts[static_cast<std::size_t>(itemId)] = 1;
                }
                RegisterInventoryItem(itemId);
            }
            return;
        }
        while (*entry >= 0)
            ++entry;
        ++entry;
    }
}
std::uint32_t GetPlayerSimpleStat(std::uint32_t command) {

    switch (command) {
    case 0:
        return g_playerStats.savedFlags;
    case 1:
        return g_playerStats.runtimeFlags;
    case 2:
        return static_cast<std::uint32_t>(g_playerStats.cash);
    case 3:
        return static_cast<std::uint32_t>(g_playerStats.alternateHealth);
    case 4:
        return static_cast<std::uint32_t>(g_playerStats.defaultHealth);
    case 5:
        return g_playerStats.cameraShakeAmount;
    case 6:
        return g_playerStats.fightAssistScale;
    case 7:
        return g_playerStats.contextActionMode;
    case 8:
        return g_playerStats.segaCoins;
    default:
        return 0xffffffffU;
    }
}

void SetPlayerSimpleStat(std::uint32_t command, std::uint32_t value) {
    switch (command) {
    case 0:
        g_playerStats.savedFlags = value;
        break;
    case 1:
        g_playerStats.runtimeFlags = value;
        break;
    case 2:
        g_playerStats.cash = static_cast<std::int32_t>(value);
        break;
    case 3:
        g_playerStats.alternateHealth = static_cast<std::int32_t>(value);
        break;
    case 4:
        g_playerStats.defaultHealth = static_cast<std::int32_t>(value);
        break;
    case 5:
        g_playerStats.cameraShakeAmount = value;
        break;
    case 6:
        g_playerStats.fightAssistScale = value;
        break;
    case 7:
        g_playerStats.contextActionMode = value;
        break;
    case 8:
        g_playerStats.segaCoins = value;
        g_playerStats.segaCoinsMaximum = (std::max)(g_playerStats.segaCoinsMaximum, value);
        break;
    default:
        break;
    }
}

void ResetInventoryControlValue9() {
    g_playerStats.inventoryControlValue9 = 0;
}

std::uint32_t AccessBatteryLife(std::uint32_t mode, std::uint8_t index, std::uint32_t value) {
    if (mode == 0) {
        const std::uint8_t bit = static_cast<std::uint8_t>(1U << (index & 31U));
        if (value)
            g_playerStats.batteryDecrementMask |= bit;
        else
            g_playerStats.batteryDecrementMask &= static_cast<std::uint8_t>(~bit);
        return 0;
    }
    if (mode != 1)
        return 0;
    const std::int8_t signedIndex = static_cast<std::int8_t>(index);
    if (static_cast<std::uint8_t>(signedIndex) > 1U)
        return 0;
    std::uint16_t &battery = g_playerStats.batteryLives[static_cast<std::size_t>(signedIndex)];
    if (battery > 28800U)
        battery = 28800U;
    return battery ? 100U * battery / 28800U : 0U;
}

std::int32_t AccessScenarioStepUniqueItemCount(std::uint32_t mode) {
    if (mode == 0) {
        g_playerStats.recentItemKeyLatest = -1;
        g_playerStats.recentItemKeyPrevious = -1;
        g_playerStats.recentItemKeyOldest = -1;
        g_playerStats.scenarioStepUniqueItemCount = 0;
        return 0;
    }
    return g_playerStats.scenarioStepUniqueItemCount;
}

std::uint32_t ConsumeCassetteUtilityResult(bool gameStateActive) {
    if (!gameStateActive)
        return 0;
    const std::uint32_t result = static_cast<std::uint32_t>(g_playerStats.cassetteUtilityResult);
    g_playerStats.cassetteUtilityResult = 0;
    return result;
}
std::int32_t UpdateAveragedRuntimeStatPageSlot(std::int8_t slotIndex, std::int32_t sampleValue) {
    auto *const pages = reinterpret_cast<std::byte *>(&g_playerStats.pages);
    const std::ptrdiff_t slotOffset = 0xe70 + static_cast<std::ptrdiff_t>(slotIndex) * 2;
    std::uint16_t sampleCount = 0;
    std::uint16_t storedAverage = 0;
    std::memcpy(&sampleCount, pages + slotOffset, sizeof(sampleCount));
    std::memcpy(&storedAverage, pages + slotOffset + 4, sizeof(storedAverage));

    const std::int32_t nextCount = static_cast<std::int32_t>(sampleCount) + 1;
    float average = sampleCount != 0 ?
        ((static_cast<float>(storedAverage) / 100.0F) * static_cast<float>(sampleCount) + static_cast<float>(sampleValue)) / static_cast<float>(nextCount) :
        static_cast<float>(sampleValue);
    if (average <= 0.01F)
        average = 0.01F;
    const std::int32_t result = static_cast<std::int32_t>(average * 100.0F);
    const std::uint16_t storedCount = nextCount >= 0xffff ? 0xffffU : static_cast<std::uint16_t>(nextCount);
    const std::uint16_t storedResult = static_cast<std::uint16_t>(result);
    std::memcpy(pages + slotOffset, &storedCount, sizeof(storedCount));
    std::memcpy(pages + slotOffset + 4, &storedResult, sizeof(storedResult));
    return result;
}

bool AddInventoryItemCount(std::uint16_t itemId, std::uint8_t amount) {
    std::uint8_t *const count = InventoryItemCount(itemId);
    if (!count)
        return false;
    if (itemId < 67)
        UnlockMoveSkillFromInventoryItem(itemId);

    if (itemId == 51) {
        g_playerStats.batteryLives[0] = 28800;
    } else if (itemId == 85) {
        RegisterInventoryItem(801);
    } else if (itemId == 89 || itemId == 90) {
        RegisterInventoryItem(800);
    }

    const std::uint16_t total = static_cast<std::uint16_t>(*count) + amount;
    *count = static_cast<std::uint8_t>(std::min<std::uint16_t>(total, 0xff));
    UI_ICONRecomputeInventoryGrid();
    return total <= 0xff;
}

std::uint8_t RemoveInventoryItemCount(std::uint16_t itemId, std::uint8_t amount) {
    std::uint8_t *const count = InventoryItemCount(itemId);
    if (!count)
        return 2;
    const std::uint8_t previous = *count;
    if (!previous)
        return 2;
    if (amount && previous > amount) {
        *count = static_cast<std::uint8_t>(previous - amount);
        UI_ICONRecomputeInventoryGrid();
        return 0;
    }
    *count = 0;
    if (itemId < g_playerStats.ownedItemMenuFlags0To66.size())
        g_playerStats.ownedItemMenuFlags0To66[itemId] &= 0xfdU;
    UI_ICONRecomputeInventoryGrid();
    return 1;
}

std::uint8_t GetInventoryItemCount(std::uint16_t itemId) {
    const std::uint8_t *const count = InventoryItemCount(itemId);
    if (!count)
        return 0;
    return g_debugForceShowAllItems == 1 ? 1 : *count;
}

void SetOwnedItemMenuInteractive(std::uint16_t itemId, bool enabled) {
    if (itemId >= g_playerStats.ownedItemMenuFlags0To66.size())
        return;
    std::uint8_t &flags = g_playerStats.ownedItemMenuFlags0To66[itemId];
    if (enabled && g_playerStats.inventoryItemCounts0To66[itemId])
        flags |= 2U;
    else
        flags &= 0xfdU;
}

namespace {

void CompactCassetteTapes() {
    auto write = g_playerStats.cassetteTapes.begin();
    for (const CassetteTapeState tape : g_playerStats.cassetteTapes) {
        if (tape.count != 0)
            *write++ = tape;
    }
    std::fill(write, g_playerStats.cassetteTapes.end(), CassetteTapeState{});
}

} // namespace

bool AddCassetteTapeCount(std::uint8_t tapeId, std::uint8_t amount) {
    if (tapeId > 27U)
        return false;
    CassetteTapeState *empty = nullptr;
    for (CassetteTapeState &tape : g_playerStats.cassetteTapes) {
        if (tape.id == tapeId) {
            const std::uint16_t total = static_cast<std::uint16_t>(tape.count) + amount;
            tape.count = static_cast<std::uint8_t>(std::min<std::uint16_t>(total, 0xff));
            CompactCassetteTapes();
            return true;
        }
        if (!empty && tape.id == 0xffU)
            empty = &tape;
    }
    if (!empty)
        return false;
    empty->id = tapeId;
    empty->count = amount;
    CompactCassetteTapes();
    return true;
}

std::uint8_t RemoveCassetteTapeCount(std::uint8_t tapeId, std::uint8_t amount) {
    if (tapeId > 27U)
        return 2;
    for (CassetteTapeState &tape : g_playerStats.cassetteTapes) {
        if (tape.id != tapeId)
            continue;
        if (tape.count > amount) {
            tape.count = static_cast<std::uint8_t>(tape.count - amount);
        } else {
            tape.count = 0;
            tape.id = 0xff;
        }
        const bool remains = tape.count != 0;
        CompactCassetteTapes();
        return remains;
    }
    return 2;
}

std::uint8_t GetCassetteTapeCount(std::uint8_t tapeId) {
    if (tapeId > 28U)
        return 0;
    for (const CassetteTapeState &tape : g_playerStats.cassetteTapes) {
        if (tape.id == tapeId)
            return tape.count;
    }
    return 0;
}
std::uint16_t CountOwnedCassetteTapes() {
    std::uint16_t result = 0;
    for (const CassetteTapeState &tape : g_playerStats.cassetteTapes) {
        if (tape.id != 0xffU)
            ++result;
    }
    return result;
}

std::uint32_t SetPlayerStats(const std::uint32_t *a) {
    if (!a)
        return 0;
    const std::uint32_t op = a[0];
    const std::uint32_t arg1 = a[1];
    const std::uint32_t arg2 = a[2];
    const std::uint32_t arg3 = a[3];
    auto &p = g_playerStats.pages;

    switch (op) {
    case 0:
        g_playerStats.step[arg1] = ClampU8(arg2);
        return g_playerStats.step[arg1];
    case 1:
        return g_playerStats.step[arg1];
    case 2:
    case 3:
        return 0;
    case 4:
        p.motorcycleRideFrameCountTotal = arg1;
        return p.motorcycleRideFrameCountTotal;
    case 5:
        return p.motorcycleRideFrameCountTotal;
    case 6:
        p.statU16_0 = ClampU16(arg1);
        return p.statU16_0;
    case 7:
        return p.statU16_0;
    case 8:
        p.statU32_0 = arg1;
        return p.statU32_0;
    case 9:
        return p.statU32_0;
    case 10:
        p.statU8_0 = ClampU8(arg1);
        return p.statU8_0;
    case 11:
        return p.statU8_0;
    case 12:
        p.statU8_1 = ClampU8(arg1);
        return p.statU8_1;
    case 13:
        return p.statU8_1;
    case 14:
        p.statU8_2 = ClampU8(arg1);
        return p.statU8_2;
    case 15:
        return p.statU8_2;
    case 16:
        p.statU32_1 = arg1;
        return p.statU32_1;
    case 17:
        return p.statU32_1;
    case 18:
        p.statU16_1 = ClampU16(arg1);
        return p.statU16_1;
    case 19:
        return p.statU16_1;
    case 20:
        p.statU32_2 = arg1;
        return p.statU32_2;
    case 21:
        return p.statU32_2;
    case 22:
        p.bestCompositeTime = arg1;
        return p.bestCompositeTime;
    case 23:
        return p.bestCompositeTime;
    case 24:
        p.bestSegmentTime = arg1;
        return p.bestSegmentTime;
    case 25:
        return p.bestSegmentTime;
    case 26:
        p.indexedU16Table0[arg1] = ClampU16(arg2);
        return p.indexedU16Table0[arg1];
    case 27:
        return p.indexedU16Table0[arg1];
    case 28:
        p.slotMachineCashInTotal = arg1;
        return p.slotMachineCashInTotal;
    case 29:
        return p.slotMachineCashInTotal;
    case 30:
        p.slotMachinePlayCountBySlot[arg1] = ClampU16(arg2);
        return p.slotMachinePlayCountBySlot[arg1];
    case 31:
        return p.slotMachinePlayCountBySlot[arg1];
    case 32:
        p.slotMachineTotalBetBySlot[arg1] = arg2;
        return p.slotMachineTotalBetBySlot[arg1];
    case 33:
        return p.slotMachineTotalBetBySlot[arg1];
    case 34:
        p.slotMachineTotalWinBySlot[arg1] = arg2;
        return p.slotMachineTotalWinBySlot[arg1];
    case 35:
        return p.slotMachineTotalWinBySlot[arg1];
    case 36:
        p.slotMachinePlayFramesBySlot[arg1] = arg2;
        return p.slotMachinePlayFramesBySlot[arg1];
    case 37:
        return p.slotMachinePlayFramesBySlot[arg1];
    case 38:
        p.motorcycleRaceAttemptCount = ClampU16(arg1);
        return p.motorcycleRaceAttemptCount;
    case 39:
        return p.motorcycleRaceAttemptCount;
    case 40:
        p.motorcycleRaceBestTimeFrames = arg1;
        return p.motorcycleRaceBestTimeFrames;
    case 41:
        return p.motorcycleRaceBestTimeFrames;
    case 42: {
        GrantSpotVisitRewards(arg1);
        auto &count = p.spotVisitCounts[arg1];
        count = ClampU8(static_cast<std::uint32_t>(count) + 1u);
        return count;
    }
    case 43:
        p.strayCatAffinityPoints = ClampU16(arg1);
        return p.strayCatAffinityPoints;
    case 44:
        return p.strayCatAffinityPoints;
    case 45:
        p.indexedU8Table0[arg1] = ClampU8(arg2);
        return p.indexedU8Table0[arg1];
    case 46:
        return p.indexedU8Table0[arg1];
    case 47:
        p.shugyoCourseClearCountSlots[arg1] = ClampU16(arg2);
        return p.shugyoCourseClearCountSlots[arg1];
    case 48:
        return p.shugyoCourseClearCountSlots[arg1];
    case 49:
        p.shugyoCourseActiveFrameSlots[arg1] = arg2;
        return p.shugyoCourseActiveFrameSlots[arg1];
    case 50:
        return p.shugyoCourseActiveFrameSlots[arg1];
    case 51:
        p.indexedU16Table3[arg1] = ClampU16(arg2);
        return p.indexedU16Table3[arg1];
    case 52:
        return p.indexedU16Table3[arg1];
    case 53:
        p.matrix4x8U16[arg1 * 8u + arg2] = ClampU16(arg3);
        return p.matrix4x8U16[arg1 * 8u + arg2];
    case 54:
        return p.matrix4x8U16[arg1 * 8u + arg2];
    case 55: {
        auto &slot = GetOrCreateSparsePair(static_cast<std::uint8_t>(arg1), static_cast<std::uint8_t>(arg2));
        slot.value = ClampU8(arg3);
        return slot.value;
    }
    case 56:
        return GetOrCreateSparsePair(static_cast<std::uint8_t>(arg1), static_cast<std::uint8_t>(arg2)).value;
    case 57:
        p.strayCatInteractionCount = ClampU8(arg1);
        return p.strayCatInteractionCount;
    case 58:
        return p.strayCatInteractionCount;
    case 59:
        p.strayCatSceneVisitCountSlots[arg1] = ClampU16(arg2);
        return p.strayCatSceneVisitCountSlots[arg1];
    case 60:
        return p.strayCatSceneVisitCountSlots[arg1];
    case 61:
        p.indexedU8Table1[arg1] = ClampU8(arg2);
        return p.indexedU8Table1[arg1];
    case 62:
        return p.indexedU8Table1[arg1];
    case 63:
        p.playerStatFlags |= 1u << (arg1 & 31u);
        return p.playerStatFlags;
    case 64:
        p.playerStatFlags &= ~(1u << (arg1 & 31u));
        return p.playerStatFlags;
    case 65: {
        const auto itemId = CharaNameToIndex(arg1);
        RegisterInventoryItem(static_cast<std::int16_t>(itemId));
        TrackUniqueInventoryItem(static_cast<std::int16_t>(itemId));
        p.charaItemCounts[itemId] = ClampU8(arg2);
        return p.charaItemCounts[itemId];
    }
    case 66:
        return p.charaItemCounts[CharaNameToIndex(arg1)];
    case 67:
        p.dartsModePlayCountSlots[arg1] = ClampU16(arg2);
        return p.dartsModePlayCountSlots[arg1];
    case 68:
        return p.dartsModePlayCountSlots[arg1];
    case 69:
        p.dartsModeActiveFrameSlots[arg1] = arg2;
        return p.dartsModeActiveFrameSlots[arg1];
    case 70:
        return p.dartsModeActiveFrameSlots[arg1];
    case 71:
        p.timeCap70MB[arg1] = ClampU8(arg2);
        return p.timeCap70MB[arg1];
    case 72:
        return p.timeCap70MB[arg1];
    case 73:
        p.counter0_70MB = ClampU8(arg1);
        return p.counter0_70MB;
    case 74:
        return p.counter0_70MB;
    case 75:
        p.counter1_70MB = ClampU8(arg1);
        return p.counter1_70MB;
    case 76:
        return p.counter1_70MB;
    case 77:
        return g_playerStats.currentSaveProgress;
    default:
        return 0;
    }
}

void Script_PlayerStatsUtils(ScenarioContext *context, void *rawParams) {
    if (!context || !context->stack || !rawParams)
        return;
    context->stack->accumulator = SetPlayerStats(static_cast<const std::uint32_t *>(rawParams));
}

const std::array<std::array<std::uint8_t, 2>, 4> g_dialogueModeVoiceTextEnable{{{1, 0}, {1, 0}, {1, 1}, {0, 1}}};



void InitialiseShenmueSettings() {
    const std::size_t mode = static_cast<std::uint8_t>(g_currentSettings.dialogueMode);
    std::uint32_t flags = (S1SettingsFlags_Get() & 0xffffffcfU) | (static_cast<std::uint32_t>(mode) << 4U);
    flags = g_dialogueModeVoiceTextEnable[mode][0] ? flags | 2U : flags & ~2U;
    flags = g_dialogueModeVoiceTextEnable[mode][1] ? flags | 1U : flags & ~1U;
    flags = (g_currentSettings.input.areaJump & 0xffU) != 0 ? (flags & ~0xcU) | 8U : flags & ~0xcU;
    flags |= 0x40U;
    flags = g_currentSettings.input.weatherMode ? flags | 0x80U : flags & ~0x80U;
    S1SettingsFlags_Set(flags);
}



void shenInitDefaultSettings() {
    g_playerStats.currentMove = 0xff00;
    InitialiseShenmueSettings();
    g_notebookLastEntryPage = 0;
    g_notebookPersistenceWord = 0;
}


void shenInitDefaultStats() {
    PlayerStatsRuntimeState &stats = g_playerStats;
    stats.alternateHealth = 200;
    stats.defaultHealth = 200;
    for (std::size_t index = 0; index < 16; ++index)
        g_arcadeHighScores[index] = 0;
    stats.runtimeFlags = 0;
    stats.contextActionMode = 0;
    stats.currentVfSkill.fill(0);
    g_bIsFighting = 0;
    stats.step.fill(0);
    stats.cameraShakeAmount = 10;
    stats.fightAssistScale = 35;
    stats.vfHpRegenerationProfile = 0;
    stats.cash = 9800;
    stats.segaCoins = 0;
    stats.inventoryItemIds.fill(0);
    stats.inventoryItemCount = 0;
    stats.recentItemKeyLatest = -1;
    stats.recentItemKeyPrevious = -1;
    stats.recentItemKeyOldest = -1;
    stats.scenarioStepUniqueItemCount = 0;
    InitializeMovesScreenRuntimeData();
    g_isPlayerGameStateActive = 7;
    IconPresentationOwner &icon = g_iconPresentation;
    icon.input = nullptr;
    icon.previousInput = nullptr;
    icon.overlayTransitionStage = 0;
    icon.overlayMode = 0;
    icon.iconFlags = 0;
    g_conversationPromptActivationLock = 0;
    SleepSaveTrainingMenu &menu = g_sleepSaveTrainingMenu;
    menu.phase = 0;
    menu.selection = 4;
    menu.active = 0;
    menu.requested = 0;
    menu.scriptResult = 0;
    menu.flags = 1;
    InitFlags();
    g_playerStatShadowRecords = {};
    stats.pages = {};
    stats.pages.sparsePairU8Slots[0].key0 = 0xff;
    stats.pages.sparsePairU8Slots[0].key1 = 0xff;
    g_ctrlInitialized = false;
    g_moduleFrameCount = 0;
    g_pendingSleepResumeLoad = 0;
}



void shenInitDefaultCassetteInventory() {
    g_playerStats.inventoryItemCounts67To234.fill(0);
    g_playerStats.inventoryItemCounts0To66.fill(0);
    static_cast<void>(AddInventoryItemCount(3, 1));
    for (CassetteTapeState &tape : g_playerStats.cassetteTapes)
        tape = {.count = 0, .id = 0xff};
    g_playerStats.batteryLives = {28800, 28800};
}

} // namespace shenmue::s1
