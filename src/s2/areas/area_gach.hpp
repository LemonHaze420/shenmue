#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "s2/conversation.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
inline constexpr shen_name GACH_TASK{SHEN_NAME(EVNT)}, GACH_RYO{SHEN_NAME(RYO_)}, GACH_HAND{SHEN_NAME(TE_R)};
inline constexpr shen_name GACH_MACHINE{SHEN_NAME(GACH)}, GACH_BASE{SHEN_NAME(GDUM)}, GACH_TARGET{SHEN_NAME(GDM1)};
inline constexpr shen_name GACH_HAND_BASE{SHEN_NAME(GDM2)}, GACH_CAMERA{SHEN_NAME(GDM3)}, GACH_COIN{SHEN_NAME(GCIN)};
inline constexpr shen_name GACH_SHELL{SHEN_NAME(CUCL)}, GACH_EXIT_ROUTE{SHEN_NAME(AKTM) | 0x12000002U};
inline constexpr shen_name GACH_VIEW_BUTTON{SHEN_NAME(BTEV)};
#pragma pack(push, 1)
struct GachEnqueueParams {
    shen_name machine;
    std::uint8_t set;
};
#pragma pack(pop)
struct GachTaskWork {
    HLib::Task *task;
    shen_name character, machine;
    std::uint8_t set, capsule;
    std::uint16_t state, rarity, selection;
    std::uint32_t previousItem, status;
    ConversationBuffer *voice;
    std::int32_t sway[3], reserved34, handTurn;
    std::uint16_t savedFov, targetFov, currentFov, reserved42;
    Vector3 viewOffset;
    std::uint32_t viewFrames, returnFrames;
    std::uint8_t secret, debugView;
    std::byte reserved5a[6];
};
struct GachHandRecord {
    float scale;
    Vector3 position;
    Vector3i rotation;
    std::int32_t pose[33];
};
struct GachCatalogEntry {
    shen_name object;
    std::uint32_t item;
    const char *archive;
};
struct GachSetInfo {
    const std::uint16_t *tiers[5];
};
static_assert(sizeof(GachEnqueueParams) == 5 && offsetof(GachEnqueueParams, set) == 4);
static_assert(sizeof(GachTaskWork) == 0x60 && offsetof(GachTaskWork, state) == 0x12);
static_assert(offsetof(GachTaskWork, voice) == 0x20 && offsetof(GachTaskWork, viewOffset) == 0x44);
static_assert(offsetof(GachTaskWork, viewFrames) == 0x50 && offsetof(GachTaskWork, secret) == 0x58);
static_assert(sizeof(GachHandRecord) == 0xa0 && offsetof(GachHandRecord, pose) == 0x1c);
static_assert(sizeof(GachCatalogEntry) == 0x10 && offsetof(GachCatalogEntry, archive) == 8);
static_assert(sizeof(GachSetInfo) == 0x28);
void InitGach();
void QueueGach(std::uintptr_t parameters);
void GachMainCallback(void *);
void GachItemViewCallback(void *);
void GachHandReturnCallback(void *);
void GachExitCallback(void *);
void SelectGachPrize(GachTaskWork *);
void LoadGachPrizeHand(GachTaskWork *);
void GachLocalDebugNullsub();
extern const ScriptAreaDefinition gachScriptDefinition;
} // namespace shenmue::s2
