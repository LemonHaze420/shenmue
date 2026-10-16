#pragma once

#include "s1/hlib.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct Character;
struct PackOwner;
struct PackSlot;
struct VFMAData;

enum PKMLSlotState : std::uint8_t {
    PKMLS_Empty = 0,
    PKMLS_Loaded = 1,
    PKMLS_Used = 2,
    PKMLS_PendingFree = 3,
};

enum BTPKState : std::uint8_t {
    BTPKS_WaitForFreeSlot = 0,
    BTPKS_WaitForPackLoad = 1,
    BTPKS_CommitLoadedPack = 2,
    BTPKS_CancelLoad = 3,
    BTPKS_Inactive = 4,
};

enum BTPKFlags : std::uint8_t {
    BTPKF_LoadIssued = 1,
    BTPKF_CancelRequested = 2,
    BTPKF_ActivateRequested = 4,
};

#pragma pack(push, 1)
struct PKMLSlot {
    void *characterModel;
    void *model;
    PackSlot *humans;
    char objectName[8];
    std::uint8_t state;
    std::uint8_t battle70Index;
    std::byte reserved22[6];
};

struct PKMLData {
    PKMLSlot slots[12];
    std::byte reserved1e0[0x40];
};

struct BTPKTaskData {
    VFMAData *game;
    std::uint8_t flags;
    std::uint8_t battle70Count;
    std::uint8_t battle70Index;
    std::uint8_t modelIndex;
    std::uint8_t slotIndex;
    std::uint8_t state;
    std::byte reserved0e[10];
    PackSlot *humans;
    PKMLData *packModels;
};
#pragma pack(pop)

static_assert(sizeof(PKMLSlot) == 0x28);
static_assert(sizeof(PKMLData) == 0x220);
static_assert(sizeof(BTPKTaskData) == 0x28);
static_assert(offsetof(BTPKTaskData, humans) == 0x18);
static_assert(offsetof(BTPKTaskData, packModels) == 0x20);

extern HLib::TaskData *g_battle70Index;
extern PackOwner *g_battle70Afs;

HLib::Task *EnqueueBTPK(VFMAData *game);
void BTPK_Callback(HLib::TaskData *taskData);
void vfFreeBattlePack(VFMAData *game);
void FREE_70MB();

} // namespace shenmue::s1
