#include "s1/task/btpk.hpp"

#include "s1/render_array.hpp"
#include "s1/task/char.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/vfma.hpp"

#include <bit>
#include <cstdio>
#include <cstring>

namespace shenmue::s1 {

HLib::TaskData *g_battle70Index = nullptr;
PackOwner *g_battle70Afs = nullptr;

namespace {

Character *g_btpkCharacters[16]{};
void *g_btpkModels[16]{};
BTPKTaskData *g_btpkRetainedData = nullptr;
HLib::Task *g_btpkRetainedTask = nullptr;

} // namespace

HLib::Task *EnqueueBTPK(VFMAData *game) {
    HLib::TaskData *const index = g_battle70Index;
    if (!index || ((game->battleType - 1U) & 0xfbU) != 0)
        return nullptr;
    HLib::Task *task = HLib_EnqueueTaskWithoutParameter(BTPK_Callback, 9, 2, static_cast<std::int32_t>(SHEN_NAME(BTPK)));
    BTPKTaskData *data = nullptr;
    if (task) {
        if (!task->task_data_ptr) {
            data = static_cast<BTPKTaskData *>(HLib_AllocHeapMemBlock(sizeof(BTPKTaskData), static_cast<std::int32_t>(SHEN_NAME(TASK))));
            if (data)
                HLib_EnableTokenCheck(data);
            task->task_data_ptr = reinterpret_cast<HLib::TaskData *>(data);
        }
        if (!data) {
            task->initCallbackFnPtr = nullptr;
            task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
            task->autoInitTaskCallbackFnPtr = nullptr;
            task = nullptr;
        }
    }
    data->game = game;
    data->flags = 0;
    data->humans = nullptr;
    data->battle70Index = 0;
    data->modelIndex = 0;
    data->slotIndex = 0;
    data->state = BTPKS_Inactive;
    game->packData = data;
    std::memset(g_btpkCharacters, 0, sizeof(g_btpkCharacters));
    g_btpkRetainedTask = nullptr;
    g_btpkRetainedData = nullptr;
    std::uint32_t count = 0;
    std::memcpy(&count, index, sizeof(count));
    data->battle70Count = static_cast<std::uint8_t>(count);
    data->packModels = static_cast<PKMLData *>(HLib_AllocHeapMemBlock(sizeof(PKMLData), static_cast<std::int32_t>(SHEN_NAME(PKML))));
    for (PKMLSlot &slot : data->packModels->slots) {
        slot.characterModel = nullptr;
        slot.model = nullptr;
        slot.humans = nullptr;
        slot.state = PKMLS_Empty;
        slot.battle70Index = 0xffU;
        slot.objectName[0] = 0;
    }
    return task;
}

void BTPK_Callback(HLib::TaskData *taskData) {
    auto *const battlePack = reinterpret_cast<BTPKTaskData *>(taskData);
    if (battlePack->state != BTPKS_Inactive) {
        for (PKMLSlot &slot : battlePack->packModels->slots) {
            if (slot.state != PKMLS_PendingFree)
                continue;
            PACK_LoadFile(slot.humans);
            slot.humans = nullptr;
            slot.characterModel = nullptr;
            HMDL_FreeAssignedModel(slot.model);
            slot.model = nullptr;
            slot.objectName[0] = 0;
            slot.state = PKMLS_Empty;
            slot.battle70Index = 0xffU;
        }
    }
    switch (battlePack->state) {
    case BTPKS_WaitForFreeSlot: {
        std::uint8_t index = 0;
        while (battlePack->packModels->slots[index].state != PKMLS_Empty) {
            ++index;
            battlePack->slotIndex = 0;
            if (index >= 12) {
                battlePack->flags = 0;
                return;
            }
        }
        battlePack->state = BTPKS_WaitForPackLoad;
        battlePack->slotIndex = index;
        battlePack->flags = 0;
        return;
    }
    case BTPKS_WaitForPackLoad: {
        const std::uint8_t flags = battlePack->flags;
        if ((flags & BTPKF_CancelRequested) != 0) {
            battlePack->state = BTPKS_CancelLoad;
            if (!PACK_MarkFree()) {
                if (!PACK_IsValidSlot(battlePack->humans))
                    return;
                PACK_LoadFile(battlePack->humans);
            }
            battlePack->flags &= ~(BTPKF_LoadIssued | BTPKF_CancelRequested);
            battlePack->state = BTPKS_WaitForFreeSlot;
            return;
        }
        if ((flags & BTPKF_LoadIssued) == 0) {
            PackSlot *const humans = PACK_LoadHumanByIndex(g_battle70Afs, 2U * battlePack->battle70Index, 2U * battlePack->battle70Index + 1U);
            battlePack->flags |= BTPKF_LoadIssued;
            battlePack->humans = humans;
            return;
        }
        if (!PACK_IsValidSlot(battlePack->humans))
            return;
        battlePack->state = BTPKS_CommitLoadedPack;
        battlePack->flags = flags & ~BTPKF_LoadIssued;
        [[fallthrough]];
    }
    case BTPKS_CommitLoadedPack: {
        const auto *const names = reinterpret_cast<const char *>(g_battle70Index) + 4 + 4 * battlePack->battle70Index;
        char objectName[8]{};
        std::snprintf(objectName, sizeof(objectName), "%c%c%c_%c", names[0], names[1], names[2], names[3]);
        PKMLSlot &slot = battlePack->packModels->slots[battlePack->slotIndex];
        PackModel *const characterModel = PACK_FindCharacterModel(battlePack->humans, objectName);
        slot.characterModel = characterModel;
        slot.model = InitChara(characterModel->model, characterModel->model);
        slot.humans = battlePack->humans;
        slot.state = PKMLS_Loaded;
        slot.battle70Index = battlePack->battle70Index;
        std::snprintf(slot.objectName, sizeof(slot.objectName), "%s", objectName);
        const std::uint8_t count = battlePack->battle70Count;
        ++battlePack->battle70Index;
        battlePack->flags = 0;
        if (battlePack->battle70Index >= count) {
            battlePack->battle70Index = count;
            battlePack->state = BTPKS_Inactive;
            return;
        }
        battlePack->state = BTPKS_WaitForFreeSlot;
        return;
    }
    case BTPKS_CancelLoad:
        battlePack->flags &= ~(BTPKF_LoadIssued | BTPKF_CancelRequested);
        battlePack->state = BTPKS_WaitForFreeSlot;
        return;
    default: {
        const std::uint8_t flags = battlePack->flags;
        if ((flags & BTPKF_ActivateRequested) != 0)
            battlePack->state = BTPKS_WaitForFreeSlot;
        battlePack->flags = 0;
        return;
    }
    }
}

void vfFreeBattlePack(VFMAData *game) {
    HLib::Task *const task = game->packTask;
    BTPKTaskData *const battlePack = game->packData;
    if (!task || !battlePack)
        return;
    VFFightStage *const stage = g_pFightStage;
    for (std::int8_t index = 0; index < static_cast<std::int8_t>(stage->maxEnemies); ++index) {
        VFEnemySlot *const slot = &stage->enemySlots[index];
        if (slot->character && slot->model) {
            g_btpkCharacters[index] = slot->character;
            g_btpkModels[index] = slot->model;
        }
    }
    g_btpkRetainedData = battlePack;
    g_btpkRetainedTask = task;
    game->packTask = nullptr;
    game->packData = nullptr;
    if (g_battle70Index) {
        HLib_FreeTaskData(g_battle70Index, 0);
        g_battle70Index = nullptr;
    }
}

void FREE_70MB() {
    BTPKTaskData *const battlePack = g_btpkRetainedData;
    if (!battlePack || !g_btpkRetainedTask)
        return;
    for (PKMLSlot &slot : battlePack->packModels->slots) {
        if (slot.humans)
            PACK_LoadFile(slot.humans);
        HMDL_FreeAssignedModel(slot.model);
        slot.model = nullptr;
        slot.battle70Index = 0xffU;
    }
    PACK_FreeOwnerADFSData(g_battle70Afs);
    g_battle70Afs = nullptr;
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(battlePack->packModels), 0);
    battlePack->packModels = nullptr;
    HLib_DestroyTask(g_btpkRetainedTask);
    for (std::size_t index = 0; index < 16; ++index) {
        if (g_btpkCharacters[index] && g_btpkModels[index])
            g_btpkCharacters[index]->model = g_btpkModels[index];
    }
    g_btpkRetainedTask = nullptr;
    g_btpkRetainedData = nullptr;
}

} // namespace shenmue::s1
