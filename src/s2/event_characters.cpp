#include "s2/event_characters.hpp"

#include "s2/blur_figure_pose.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/debug.hpp"
#include "s2/event_runtime.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include <bit>

namespace shenmue::s2 {
namespace {

std::uint32_t GetEventCharacterMotionRootMode(const MotionWork *work) {
    for (std::uint32_t region = 0; region < 5; ++region) {
        if (work->runtime.regionFlags[region] & 0x10U)
            return work->runtime.currentRootMode;
    }
    return 0;
}

void SetEventCharacterReadFlag(bool enabled) {
    if (!g_playerRuntime.model)
        return;
    if (enabled)
        g_playerRuntime.flags |= 0x00400000U;
    else
        g_playerRuntime.flags &= ~0x00400000U;
}

} // namespace

void AT_SetLocalBaseChara(shen_name name, shen_name base) {
    Character *const character = FindCharacterByScenarioName(name);
    if (!character)
        return;
    const Vector3 position = GetCharacterWorldPosition(character);
    const Vector3i rotation = GetCharacterWorldRotation(character);
    CharacterNode *parent = nullptr;
    if (base.name != EVENT_CHARACTER_INVALID.name) {
        if (base.name > 31)
            parent = GetCharacterNode(FindCharacterByScenarioName(base));
        else
            parent = reinterpret_cast<CharacterNode *>(GetCharacterInMapControlTask(static_cast<std::int32_t>(base.name)));
    }
    character->entity.node.child = parent;
    shenApplyTranslationToAll(&character->entity.node, position);
    SetCharacterWorldRotation(character, rotation);
}

shen_name EV_SCEN_FindChar_Wrapper(shen_name name) {
    Character *const character = FindCharacterByScenarioName(name);
    if (!character || !character->entity.node.child)
        return EVENT_CHARACTER_INVALID;
    CharacterNode *const parent = character->entity.node.child;
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (!control)
        return EVENT_CHARACTER_INVALID;
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(parent);
    const std::uintptr_t slots = reinterpret_cast<std::uintptr_t>(control->characterControlTable.data());
    if (address >= slots && address < slots + sizeof(control->characterControlTable)) {
        return shen_name{static_cast<std::uint32_t>((address - slots) / sizeof(CharacterControlSlot))};
    }
    Character *const parentCharacter = reinterpret_cast<Character *>(reinterpret_cast<std::byte *>(parent) - offsetof(Character, entity.node));
    if (parentCharacter->entity.name.name != EVENT_CHARACTER_COMPONENT.name || !(parentCharacter->flags & CF_Valid))
        return EVENT_CHARACTER_INVALID;
    return scenGetCharaName(control->scenarioTask, parentCharacter);
}

EventReadCharactersWork *Queue_EVT_AT_ReadChr(HLib::Task *parent, const shen_name *names) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_ReadChr_Callback, 6, 0x0b, sizeof(EventReadCharactersWork), EVENT_CHARACTER_TASK);
    EventReadCharactersWork *const work = static_cast<EventReadCharactersWork *>(HLib::GetTaskParameter(task));
    HLib::SetupTimedTask(parent, 0);
    SetPlayerControlEnabled(0);
    work->task = task;
    work->parentTask = parent;
    work->names = names;
    work->characterIndex = 0;
    work->delayFrames = 0;
    work->maxNpc = GetNpcBatchCountOverride(30);
    work->needsReload = static_cast<std::uint32_t>(GetNpcNewLoadState());
    camCtrlCacheUtil(4);
    camCtrlCacheUtil(2);
    SetEventCharacterReadFlag(true);
    if (work->needsReload)
        EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    for (std::int32_t index = 29; index >= 0; --index)
        work->characters[static_cast<std::size_t>(index)].state = 0;
    return work;
}

void EVT_AT_ReadChr_Callback(void *taskData) {
    EventReadCharactersWork *const work = static_cast<EventReadCharactersWork *>(taskData);
    const shen_name name = work->names[work->characterIndex];
    if (name.name == EVENT_CHARACTER_INVALID.name) {
        HLib::ResolveDeferredCallback(work->parentTask);
        HLib::SetupTaskDelay(0);
    } else {
        LoadOneNpcAndAdvance(work, name);
    }
}

std::int64_t LoadOneNpcAndAdvance(EventReadCharactersWork *work, shen_name name) {
    if (work->characterIndex >= 30)
        return 0;
    EventReadCharacterRecord &record = work->characters[static_cast<std::size_t>(work->characterIndex)];
    switch (record.state) {
    case 0:
        record.character = name;
        if (FindCharacterByScenarioName(name)) {
            record.state = 2;
            record.newlyLoaded = 0;
            return 0;
        }
        EV_CheckWarmStartNpcRoute(name);
        record.newlyLoaded = 1;
        record.state = 1;
        return 0;
    case 1: {
        const std::uint64_t result = EV_npcStreamOrLoadHuman(name);
        if (result == 1)
            record.state = 2;
        else if (result)
            EV_debug_log("common\\et_autosp.c", 2282, "%c%c%c%c : %d\n", static_cast<int>(name.name & 0xffU), static_cast<int>((name.name >> 8) & 0xffU),
                static_cast<int>((name.name >> 16) & 0xffU), static_cast<int>(name.name >> 24),
                std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(result)));
        return 0;
    }
    case 2: {
        record.restoreHeightGravity = 1;
        record.restoreGravity = 1;
        MotionWork *const motion = FindMotionWork(FindCharacterByScenarioName(name));
        record.motionRootMode = GetEventCharacterMotionRootMode(motion);
        record.previousBase = EV_SCEN_FindChar_Wrapper(name);
        EV_SetRefBoardParamsFor(name, 1);
        EV_SetCharacterAlphaEnabled(name, false);
        EV_SetCharaControllerFlags(name, ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(name, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(name, UpdateHeight, NpcCharacterControllerState::Disabled);
        AT_SetLocalBaseChara(name, EVENT_CHARACTER_INVALID);
        figpSetFlagsBlendForBlur(name, 2, 1);
        EV_mtwkSetEnabled(motion, true);
        mtwkSetExtraFlag(motion, 0, 0);
        mtwkSetExtraFlag(motion, 3, 2);
        mtwkSetRequestedRootMode(motion, 3);
        SetMotionFrameTime(motion, 7, MotionRegion::LowerBody);
        SetMotionRegionFlag(motion, 66, 2, MotionRegion::LowerBody);
        SetMotionRegionFlag(motion, 69, 2, MotionRegion::LowerBody);
        SetMotionRegionFlag(motion, 71, 2, MotionRegion::LowerBody);
        ++work->characterIndex;
        record.state = 3;
        break;
    }
    }
    return 1;
}

std::int64_t RestoreEventCharacter(EventReadCharactersWork *work) {
    if (work->characterIndex <= 0)
        return work->characterIndex;
    EventReadCharacterRecord &record = work->characters[static_cast<std::size_t>(--work->characterIndex)];
    const shen_name name = record.character;
    record.state = 0;
    Character *const character = FindCharacterByScenarioName(name);
    if (!character)
        return 0;
    figpSetFlagsBlendForBlur(name, 5, 2);
    MotionWork *const motion = FindMotionWork(character);
    EV_mtwkSetEnabled(motion, true);
    mtwkSetExtraFlag(motion, 0, 0);
    mtwkSetExtraFlag(motion, 3, 2);
    mtwkSetRequestedRootMode(motion, record.motionRootMode);
    SetMotionFrameTime(motion, 5, MotionRegion::LowerBody);
    SetMotionRegionFlag(motion, 66, 2, MotionRegion::LowerBody);
    SetMotionRegionFlag(motion, 69, 2, MotionRegion::LowerBody);
    SetMotionRegionFlag(motion, 71, 2, MotionRegion::LowerBody);
    mtwkRequestMotion(motion, EVENT_CHARACTER_STAND_MOTION, MotionRegion::LowerBody);
    EV_SetRefBoardParamsFor(name, 0);
    EV_SetCharacterAlphaEnabled(name, true);
    EV_SetCharaControllerFlags(name, UpdateHeight, NpcCharacterControllerState::Enabled);
    AT_SetLocalBaseChara(name, record.previousBase);
    UpdateMotionWorkWithPosition(motion, nullptr, 0);
    if (record.restoreHeightGravity) {
        EV_SetCharaControllerFlags(name, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    }
    if (record.restoreGravity)
        EV_SetCharaControllerFlags(name, ApplyGravity, NpcCharacterControllerState::Enabled);
    if (record.newlyLoaded)
        EV_npcFree(name);
    const std::int64_t visible = IsNpcLoadedOrVisible(name);
    return visible ? EV_npcLockUnlock(name) : visible;
}

void ET_AT_KillWork(EventReadCharactersWork *work) {
    using enum CameraState;
    while (work->characterIndex > 0)
        RestoreEventCharacter(work);
    if (work->delayFrames > 0) {
        do {
            --work->delayFrames;
        } while (work->delayFrames > 0);
    }
    GetNpcBatchCountOverride(work->maxNpc);
    EV_NPC_NewLoadSw(static_cast<std::uint32_t>(work->needsReload));
    SetEventCharacterReadFlag(false);
    camCtrlCacheUtil(5);
    camCtrlCacheUtil(3);
    camCtrlHandleContextSwitch(0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    g_cameraControl->showFilmBars = 0;
    SetPlayerControlEnabled(1);
    HLib::DestroyTask(work->task);
}

// @verified - p
AuthoredPluralControlWork *QueueAuthoredPluralControl(const AuthoredPluralControlEntry *entries) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AuthoredPluralControlCallback, 5, 11, sizeof(AuthoredPluralControlWork), EVENT_CHARACTER_TASK);
    AuthoredPluralControlWork *const work = static_cast<AuthoredPluralControlWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->entries = entries;
    work->state = 0;
    work->count = 0;
    for (unsigned index = 0; index < 8; ++index)
        work->saved[index] = {.character = {}, .control = 1};
    return work;
}

// @verified - p
void AuthoredPluralControlCallback(void *taskData) {
    AuthoredPluralControlWork *const work = static_cast<AuthoredPluralControlWork *>(taskData);
    const std::int16_t frame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (work->state == 0) {
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work->state = 1;
    } else if (work->state == 1) {
        const AuthoredPluralControlEntry *entry = work->entries;
        while (entry->frame == frame) {
            if (DoesScenarioCharacterExist(entry->character)) {
                unsigned index = 0;
                while (index < 8 && work->saved[index].character.name != entry->character.name)
                    ++index;
                if (index == 8) {
                    work->saved[work->count].character = entry->character;
                    const std::int16_t previous = static_cast<std::int16_t>(SetPluralControlFor(ctrlGetCharaByName(entry->character), 1));
                    work->saved[work->count].control = previous;
                    SetPluralControlFor(ctrlGetCharaByName(entry->character), static_cast<std::uint16_t>(previous));
                    ++work->count;
                }
                SetPluralControlFor(ctrlGetCharaByName(entry->character), static_cast<std::uint16_t>(entry->control));
            }
            ++entry;
        }
        work->entries = entry;
        if (entry->frame < 0 || !EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work->state = 2;
    } else if (work->state == 2) {
        for (unsigned index = 0; index < 8; ++index) {
            if (work->saved[index].character.name && DoesScenarioCharacterExist(work->saved[index].character))
                SetPluralControlFor(ctrlGetCharaByName(work->saved[index].character), static_cast<std::uint16_t>(work->saved[index].control));
        }
        HLib::DestroyTaskInternal(work->task);
    }
}

} // namespace shenmue::s2
