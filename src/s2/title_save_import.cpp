#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/inventory_dc.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/save_load_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#include <cstring>

namespace shenmue {
namespace s2 {

void ImportInheritedInventory(const std::uint8_t *items);
void ImportCapsuleStock(const std::uint8_t *capsules);
std::byte *GetScenarioPersistentScratch();

struct TitleSaveMoveBank {
    std::int16_t proficiency[20], usage[20];
    std::int8_t count;
    std::uint8_t selected;
};
typedef char TitleSaveMoveBankSizeCheck[sizeof(TitleSaveMoveBank) == 0x52 ? 1 : -1];
struct TitleSavePayload {
    std::byte stats[0x32];
    TitleSaveMoveBank moves[3];
};

// @verified - d
void ImportTitleSavePayload(const std::byte *payload) {
    g_nativePlayerStats.savedFlags = reinterpret_cast<const NativePlayerStats *>(payload)->savedFlags;
    std::int32_t index;
    for (index = 0; index < 4; ++index)
        g_nativePlayerStats.statFlags[index] = reinterpret_cast<const NativePlayerStats *>(payload)->statFlags[index];
    g_nativePlayerStats.retained = reinterpret_cast<const NativePlayerStats *>(payload)->retained;
    g_nativePlayerStats.cash = reinterpret_cast<const NativePlayerStats *>(payload)->cash;
    g_nativePlayerStats.segaCoins = reinterpret_cast<const NativePlayerStats *>(payload)->segaCoins;
    g_nativePlayerStats.alternateHealth = reinterpret_cast<const NativePlayerStats *>(payload)->alternateHealth;
    g_nativePlayerStats.defaultHealth = reinterpret_cast<const NativePlayerStats *>(payload)->defaultHealth;
    g_nativePlayerStats.reactionCameraScale = reinterpret_cast<const NativePlayerStats *>(payload)->reactionCameraScale;
    g_nativePlayerStats.assistScale = reinterpret_cast<const NativePlayerStats *>(payload)->assistScale;
    g_nativePlayerStats.gameConfigurationFlags = reinterpret_cast<const NativePlayerStats *>(payload)->gameConfigurationFlags;
    g_nativePlayerStats.contextActionMode = reinterpret_cast<const NativePlayerStats *>(payload)->contextActionMode;
    g_nativePlayerStats.optionA = reinterpret_cast<const NativePlayerStats *>(payload)->optionA;
    g_nativePlayerStats.optionB = reinterpret_cast<const NativePlayerStats *>(payload)->optionB;
    g_nativePlayerStats.actionState = reinterpret_cast<const NativePlayerStats *>(payload)->actionState;
    g_transferredYen = reinterpret_cast<const NativePlayerStats *>(payload)->cash;
    g_nativePlayerStats.cash = static_cast<std::int32_t>(static_cast<float>(reinterpret_cast<const NativePlayerStats *>(payload)->cash) * 0.033333335f);
    NativeMoveSkillBank *const banks[3] = {&g_moveSkillBank1, &g_moveSkillBank2, &g_moveSkillBank3};
    NativeMoveSkillBank *const *bank = banks;
    for (index = 0; index < 3; ++index, ++bank) {
        for (std::int32_t move = 0; move < 40; ++move) {
            (*bank)->proficiency[move] = reinterpret_cast<const TitleSavePayload *>(payload)->moves[index].proficiency[move];
            (*bank)->usage[move] = reinterpret_cast<const TitleSavePayload *>(payload)->moves[index].usage[move];
        }
        (*bank)->count = reinterpret_cast<const TitleSavePayload *>(payload)->moves[index].count;
        (*bank)->selected = reinterpret_cast<const TitleSavePayload *>(payload)->moves[index].selected;
    }
    std::memcpy(GetScenarioPersistentScratch(), payload + 0x130, 0x80);
    ImportInheritedInventory(reinterpret_cast<const std::uint8_t *>(payload + 0x250));
    ImportCapsuleStock(reinterpret_cast<const std::uint8_t *>(payload + 0x350));
}

// @verified - d
void ImportTitleSaveSlot(const void *bank, std::int32_t slot) {
    if (slot >= 0) {
        ImportTitleSavePayload(static_cast<const std::byte *>(bank) + 0x2000 + slot * 0x1800);
    } else {
        ResetNativePlayerStats();
        InitItems();
    }
}

void TitleSaveImportNullsub() {
}

} // namespace s2
} // namespace shenmue

#endif
