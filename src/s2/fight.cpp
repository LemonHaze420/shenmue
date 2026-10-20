#include "s2/fight.hpp"

#include "s2/character.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/battle_qte.hpp"
#include "s2/battle_qte_sprites.hpp"
#include "s2/camera.hpp"
#include "s2/fight_stage.hpp"
#include "s2/ui.hpp"
#include "s2/event_runtime.hpp"
#include "s2/particle_effects.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/pak.hpp"
#include "s2/save.hpp"

#include <algorithm>
#include <bit>
#include <cstring>
#include <cstdint>

namespace shenmue::s2 {

std::uint8_t g_fightActive{};
std::uint8_t g_fightUiActive{};
std::uint16_t g_fightModeFlags{};
std::int32_t g_forcePlayerGodmode{};
std::int32_t g_hasColouredText{};
std::int32_t g_is_vfm_active{};


void InitVFM() {
    g_is_vfm_active = 0;
}
std::uint8_t g_battleQteCommandSelection{};
std::uint16_t g_fightLoadWidth{}, g_fightLoadHeight{};
FGST_t *g_pFightStage{};
std::uint8_t g_FGEN_count{};
std::uint8_t g_vfFormationGroupIndex{};
std::uint8_t g_vfStreetFightLevel{};
std::int32_t g_vfStrikeLabelSprites[2]{};

// @verified - p
std::uint64_t IsFightActive() {
    return g_fightActive != 0;
}

namespace {
std::uint8_t streetFightProgress{};

void FightAreaMaskNullsub(std::uintptr_t) noexcept {
}
}

// The reset leaf does not require FBQT admission.
// @verified - p
std::uint8_t fbqtResetStates(std::uint8_t mode) noexcept {
    if (mode == 0)
        g_battleQteCommandSelection = 0;
    return mode;
}

// @verified - p
std::uint8_t shenStartFGST(std::uint8_t enemies, std::uint8_t groups, std::uint8_t areaMasks, std::uint8_t generators) {
    ShowElementByIndex(UIElement::StreetFighterHud);
    AllocateFGST();
    AllocateFCMG(enemies);
    AllocateFRMG(groups);
    FightAreaMaskNullsub(areaMasks);
    AllocateFGEN(generators);
    for (FightEntry &entry : g_activeFightData)
        entry.character = nullptr;
    AllocateFBIT();
    g_hasColouredText = 0;
    vfInitStageObjects();
    if (!g_is_vfm_active) {
        fbLoadGauge();
        SetupLoadSprite();
    }
    g_fightLoadWidth = 64;
    g_fightLoadHeight = 128;
    (void)vfmaGetTaskParam();
    EV_camCtrlToggleCombatCamera(UINT32_MAX, 0);
    return fbqtResetStates(0);
}

// @verified - p
std::uint32_t shenInitFightData(Character *character, void *blocker, std::uint8_t poolMode, std::uint8_t team, std::uint16_t hp, std::uint8_t style) {
    return static_cast<std::uint32_t>(
        static_cast<std::int32_t>(static_cast<std::int8_t>(PublishFightFormation(character, blocker, poolMode, team, hp, style, 0, 0, 0.0f, 0.0f, 0.0f))));
}

// Unlike the FENE-name lookup, this resolves the scenario character first.
// @verified - p
VFActor *btlGetActorByScenarioName(VFMA_t *game, shen_name name) {
    if (!game)
        return nullptr;
    Character *const character = FindCharacterByScenarioName(name);
    if (!character)
        return nullptr;
    for (std::uint32_t i = 0; i < 6; ++i)
        if (game->characters[i] && game->characters[i] == character)
            return game->actors[i];
    return nullptr;
}

// @verified - p
std::uint32_t vfclControlRuntimeFlags(VFCL_t *collision, std::uint32_t mask, std::int32_t action) {
    const std::uint32_t previous = collision->runtimeFlags;
    if (action > 0)
        collision->runtimeFlags = previous | mask;
    else if (action == 0)
        collision->runtimeFlags = previous & ~mask;
    return previous & mask;
}

// @verified - p
std::uint8_t vfUpdateFlagsFor(Character *character, std::int8_t mode, std::uint32_t flag, std::int32_t value) {
    std::uint8_t index = g_vfFormationGroupIndex;
    if (character || index == 99) {
        std::int8_t candidate = std::bit_cast<std::int8_t>(static_cast<std::uint8_t>(g_pFightStage->groupCount - 1U));
        while (candidate >= 0 && g_pFightStage->groups[candidate].character != character)
            --candidate;
        if (candidate < 0 || candidate == 99)
            return 0;
        index = static_cast<std::uint8_t>(candidate);
    }
    FRMG_t &group = g_pFightStage->groups[index];
    if (mode == 1) {
        group.formationFlags &= ~flag;
        if (flag & 0x30001U) {
            VFActor *const actor = group.slot->fighter;
            if (actor)
                actor->formationFlags &= static_cast<std::int32_t>(0xfffcfffeU);
        }
        return 1;
    }
    group.formationFlags |= flag;
    const std::uint8_t byte = static_cast<std::uint8_t>(value);
    const std::uint16_t word = static_cast<std::uint16_t>(value);
    switch (flag) {
    case 1:
        group.gameplayParameters[0] = (group.gameplayParameters[0] & 0x0000ffffU) | (static_cast<std::uint32_t>(word) << 16U);
        break;
    case 2:
    case 4:
    case 0x80000U:
        group.gameplayParameters[1] = (group.gameplayParameters[1] & 0x0000ffffU) | (static_cast<std::uint32_t>(word) << 16U);
        break;
    case 0x100U:
        group.actionParameters[0] = word;
        break;
    case 0x200U:
        group.actionParameters[1] = word;
        break;
    case 0x400U:
        if (group.animation == 0xffffU)
            group.animation = word;
        else
            group.spawnYaw = word;
        break;
    case 0x8000U:
        group.activationFlags = static_cast<std::uint32_t>(value);
        break;
    case 0x20000U:
        group.gameplayParameters[1] = (group.gameplayParameters[1] & 0xffff0000U) | word;
        break;
    case 0x40000U:
        group.gameplayParameters[0] = (group.gameplayParameters[0] & 0xffffff00U) | byte;
        break;
    case 0x200000U:
        group.waveFlags |= static_cast<std::uint32_t>(value);
        break;
    case 0x400000U:
        group.selectFlags |= byte;
        break;
    case 0x800000U:
        group.activationState = (group.activationState & 0xff00ff00U) | 2U | (static_cast<std::uint32_t>(byte) << 16U);
        break;
    case 0x1000000U:
        group.activationState = (group.activationState & 0xff000000U) | 0x201U | (static_cast<std::uint32_t>(byte) << 16U);
        group.spawnWords[0] = 470;
        break;
    case 0x2000000U:
        group.activationState = (group.activationState & 0xffffff00U) | 3U;
        break;
    case 0x8000000U:
        group.activationState = (group.activationState & 0x00ffffffU) | (static_cast<std::uint32_t>(byte) << 24U);
        break;
    case 0x10000000U:
        group.assignment = byte;
        break;
    case 0x20000000U:
        group.activationState = (group.activationState & 0xffffff00U) | 5U;
        break;
    case 0x80000000U:
        (void)fbqtResetStates(byte);
        break;
    default:
        break;
    }
    return 1;
}

// @verified - p
void ToggleFightMotionCallbackFlag(Character *character, std::int32_t enabled) {
    MotionWork *const motion = FindMotionWork(character);
    if (enabled)
        motion->runtime.callbackFlags |= 8U;
    else
        motion->runtime.callbackFlags &= ~8U;
}

// @verified - p
std::uint32_t SetFightRoundCounts(std::uint8_t first, std::uint8_t second) {
    g_pFightStage->battleFlags |= 0x40000U;
    SetFightRoundPrompt(static_cast<std::int32_t>(first) + second);
    g_vfRoundResultCounters[0] = std::min<std::uint8_t>(first, 2);
    g_vfRoundResultCounters[2] = std::min<std::uint8_t>(static_cast<std::uint8_t>(g_vfRoundResultCounters[0] + 1), 2);
    g_vfRoundResultCounters[1] = std::min<std::uint8_t>(second, 2);
    g_vfRoundResultCounters[3] = std::min<std::uint8_t>(static_cast<std::uint8_t>(g_vfRoundResultCounters[1] + 1), 2);
    return g_vfRoundResultCounters[3];
}


void SetStreetFightLevel(std::uint8_t level) {
    g_vfStreetFightLevel = level;
    g_pFightStage->battleFlags |= 0x10000U;
    GetSFLevel(level);
}

// @verified - p
std::uint64_t GetStreetFightProgress() noexcept {
    return streetFightProgress;
}

// @verified - p
std::uint64_t SetStreetFightProgress(std::uint32_t value) noexcept {
    if (value > 255U)
        value = 255U;
    streetFightProgress = static_cast<std::uint8_t>(value);
    return value;
}

// @verified - p
std::uint64_t AddStreetFightProgress(std::int32_t delta) noexcept {
    std::int32_t value = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(delta) + streetFightProgress);
    if (value > 255)
        value = 255;
    else if (value < 0)
        value = 0;
    streetFightProgress = static_cast<std::uint8_t>(value);
    return static_cast<std::uint32_t>(value);
}

// @verified - p
void ResetStreetFightProgress() noexcept {
    streetFightProgress = 0;
}

// These separate legacy handles are not LOADNUM's six records.
// @verified - p
std::uint64_t CreateFightStrikeLabels() {
    g_pFightStage->battleFlags |= 0x20000U;
    MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    g_vfStrikeLabelSprites[0] = static_cast<std::int32_t>(CreateLeftAlignedWindowTextSurface(control->windowTask, "strike(s)", 256, -1, -1, 0));
    control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const std::uint64_t result = CreateLeftAlignedWindowTextSurface(control->windowTask, "left", 128, -1, -1, 0);
    g_vfStrikeLabelSprites[1] = static_cast<std::int32_t>(result);
    return result;
}

// EAX zero-extends every result, including the default UINT32_MAX.
// @verified - p
std::uint32_t EV_vfBattleUtil(FightBattleUtilOp operation, shen_name name, std::int32_t value) {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    VFMA_t *const game = task ? static_cast<VFMA_t *>(HLib::GetTaskParameter(task)) : nullptr;
    switch (operation) {
    case FightBattleUtilOp::CollisionFlags:
        if (game)
            return vfclControlRuntimeFlags(game->collision, name.name, value);
        break;
    case FightBattleUtilOp::CombatCameraFlags:
        return EV_camCtrlToggleCombatCamera(name.name, value);
    case FightBattleUtilOp::ActorField:
        if (VFActor *const actor = btlGetActorByScenarioName(game, name)) {
            switch (value) {
            case 0:
                return actor->hitStateFlags;
            case 1:
                return actor->activeMotionFlags;
            case 2:
                return actor->savedHitStateFlags;
            case 3:
                return actor->previousHitStateFlags;
            case 4:
                return actor->previousActiveMotionFlags;
            case 5:
                return actor->motionFlags;
            case 6:
                return actor->secondaryMotionFlags;
            case 7:
                return actor->savedMotionFlags;
            case 8:
                return actor->savedSecondaryMotionFlags;
            case 9:
                return actor->hitResolutionFlags;
            case 10:
                return actor->moveFlags;
            case 11:
                return actor->stateFlags;
            case 12:
                return actor->groundContactFlags;
            case 13:
                return actor->reactionFlags;
            case 14:
                return actor->hitFlags;
            default:
                break;
            }
        }
        break;
    case FightBattleUtilOp::StageBattleFlags: {
        const std::uint32_t previous = g_pFightStage->battleFlags;
        if (value > 0)
            g_pFightStage->battleFlags = previous | name.name;
        else if (value == 0)
            g_pFightStage->battleFlags = previous & ~name.name;
        if (game && name.name == 4) {
            if (value > 0)
                game->mode |= 0x4000U;
            else if (value == 0)
                game->mode &= ~0x4000U;
        }
        return previous & name.name;
    }
    case FightBattleUtilOp::FightModeFlags:
        if (game) {
            const std::uint32_t previous = game->mode;
            if (value > 0)
                game->mode = previous | name.name;
            else if (value == 0)
                game->mode = previous & ~name.name;
            return previous;
        }
        break;
    case FightBattleUtilOp::ForceActorDefeat:
        if (VFActor *const actor = btlGetActorByScenarioName(game, name)) {
            actor->moveFlags |= VFMF_ForceDefeat;
            actor->hp = 0.0f;
        }
        break;
    case FightBattleUtilOp::MoveFlag400000:
    case FightBattleUtilOp::MoveFlag40000:
        if (VFActor *const actor = btlGetActorByScenarioName(game, name)) {
            const std::uint32_t mask = operation == FightBattleUtilOp::MoveFlag400000 ? 0x400000U : 0x40000U;
            const std::uint32_t previous = actor->moveFlags;
            if (value > 0)
                actor->moveFlags = previous | mask;
            else if (value == 0)
                actor->moveFlags = previous & ~mask;
            return previous & mask;
        }
        break;
    case FightBattleUtilOp::SkillMask:
        return static_cast<std::uint8_t>(vfCheckSkillFlag(name));
    case FightBattleUtilOp::MotionOrSlotFlag:
        if (Character *const character = FindCharacterByScenarioName(name)) {
            if (value == 0)
                ToggleFightMotionCallbackFlag(character, 0);
            else if (value > 0) {
                for (std::uint32_t i = 0; i < g_pFightStage->enemyCount; ++i) {
                    if (g_pFightStage->enemySlots[i].character != character)
                        continue;
                    g_pFightStage->enemySlots[i].flags |= 2U;
                    break;
                }
            }
        }
        break;
    default:
        break;
    }
    return UINT32_MAX;
}

// The default and side-effect-only branches retain signed RAX -1.
// @verified - p
std::int64_t vfmDispatchControl(FightControlOp operation, shen_name name, std::uintptr_t payload, std::int32_t action) {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    VFMA_t *const game = task ? static_cast<VFMA_t *>(HLib::GetTaskParameter(task)) : nullptr;
    std::int64_t result = -1;
    switch (operation) {
    case FightControlOp::ReactionRank:
        if (VFActor *const actor = btlGetActorByScenarioName(game, name)) {
            result = static_cast<std::int8_t>(actor->reactionRank);
            if (action > 0)
                actor->reactionRank = static_cast<std::uint8_t>(payload);
        }
        break;
    case FightControlOp::Package:
        result = std::bit_cast<std::int64_t>(reinterpret_cast<std::uintptr_t>(g_pFightStage->packData));
        if (action > 0)
            g_pFightStage->packData = FindPakByEntryName(reinterpret_cast<const char *>(payload));
        break;
    case FightControlOp::SpawnTimer:
        if (g_pFightStage) {

            std::byte *const timer = reinterpret_cast<std::byte *>(g_pFightStage) + offsetof(FGST_t, teamRespawnTimers) + 2U * g_pFightStage->spawnPointCount;
            std::int16_t previous;
            std::memcpy(&previous, timer, sizeof(previous));
            const std::uint16_t replacement = static_cast<std::uint16_t>(payload);
            std::memcpy(timer, &replacement, sizeof(replacement));
            result = previous;
        }
        break;
    case FightControlOp::MotionUpdateCamera:
        if (game) {
            Character *const character = FindCharacterByScenarioName(name);
            if (character && EV_camCtrlToggleCombatCamera(4, 1) == 0)
                BeginMotionUpdateCamera(character);
        }
        break;
    case FightControlOp::CameraMode:
        result = static_cast<std::uint8_t>(camCtrlGetInterpMode());
        if (action > 0)
            camCtrlSetInterpModeTo(static_cast<CameraMotionMode>(static_cast<std::uint8_t>(payload)));
        break;
    case FightControlOp::QueuedAction:
        if (VFActor *const actor = btlGetActorByScenarioName(game, name)) {
            result = std::bit_cast<std::int64_t>(actor->parameter370);
            if (action > 0) {
                vfResetReactionProgram(actor);
                actor->parameter370 = payload;
                actor->parameter378 = payload;
                actor->deferredReaction = 0;
                if (((payload >> 24U) & 0xffU) == 8 && actor->pairedActor) {
                    VFActor *const paired = actor->pairedActor;
                    vfResetReactionProgram(paired);
                    paired->action = 0x1000000U;
                    paired->parameter370 = 0;
                    paired->parameter378 = 0;
                    paired->deferredReaction = 0;
                }
            } else if (action == 0) {
                actor->parameter370 = 0;
                actor->parameter378 = 0;
            }
        }
        break;
    case FightControlOp::CameraParameter:
        if (game) {
            result = static_cast<std::uint8_t>(game->cameraParameter);
            if (action >= 0) {
                game->cameraParameter = action > 0 ? static_cast<std::int8_t>(payload) : 0;
                g_pFightStage->battleFlags |= 0x40U;
            }
        }
        break;
    case FightControlOp::GroupCounter:
        if (game)
            result = g_pFightStage->groupCounters[payload & 1U];
        break;
    case FightControlOp::SpriteDepth:
        if (game) {
            result = game->spriteDepth;
            if (action > 0)
                game->spriteDepth = static_cast<std::uint8_t>(payload);
            else if (action == 0)
                game->spriteDepth = 0;
        }
        break;
    case FightControlOp::ResetCameraWaypoints:
        ResetMotionCameraWaypoints();
        break;
    case FightControlOp::AppendCameraWaypoint:
        AppendMotionCameraWaypoint(*reinterpret_cast<const Vector3 *>(payload));
        break;
    case FightControlOp::MotionInterpolationCamera:
        BeginMotionInterpolationCamera(FindCharacterByScenarioName(name));
        break;
    case FightControlOp::MotionNibble: {
        const std::int8_t code = static_cast<std::int8_t>(name.name);
        result = vfMotnNibbleGet(code);
        if (action > 0)
            vfMotnNibbleSet(code, static_cast<std::int32_t>(payload));
        break;
    }
    case FightControlOp::RoundCounts:
        SetFightRoundCounts(static_cast<std::uint8_t>(name.name), static_cast<std::uint8_t>(payload));
        break;
    case FightControlOp::StreetFightLevel:
        SetStreetFightLevel(static_cast<std::uint8_t>(payload));
        break;
    case FightControlOp::StrikeLabels:
        CreateFightStrikeLabels();
        break;
    case FightControlOp::StrikesLeft:
        result = static_cast<std::int8_t>(game->strikesLeft);
        break;
    case FightControlOp::BattleCounter:
        result = static_cast<std::int8_t>(game->counter32c);
        break;
    case FightControlOp::GaugeResources:
        if (action > 0) {
            fbLoadGauge();
            SetupLoadSprite();
            g_is_vfm_active = 1;
            result = 1;
        } else if (action == 0) {
            if (g_is_vfm_active) {
                FreeBattleGaugeSprites();
                FreeLoadSprites();
                g_is_vfm_active = 0;
                result = 1;
            } else
                result = 0;
        } else
            result = g_is_vfm_active;
        break;
    case FightControlOp::MovesScroll:
        if (action > 0) {
            QueueWazamakiTask();
            result = 1;
        } else
            result = static_cast<std::int32_t>(IsWazamakiInactive());
        break;
    case FightControlOp::BattleType:
        result = game->type;
        break;
    default:
        break;
    }
    return result;
}

// @verified - p
VFMA_t *check_dead_status(std::uint8_t mode) {
    VFMA_t *const game = vfmaGetTaskParam();
    VFMA_t *result = game;
    if (game && game->state == 3) {
        for (std::uint32_t index = 0; index < g_pFightStage->groupCount; ++index) {
            FRMG_t &group = g_pFightStage->groups[index];
            if (group.character) {
                group.state = 10;
                if (reinterpret_cast<std::uintptr_t>(group.character) != FIGHT_DEAD.name)
                    g_activeFightData[group.slot->fightEntryIndex].status = 10;
            }

            result = reinterpret_cast<VFMA_t *>(static_cast<std::uintptr_t>(g_pFightStage->groupCount));
        }
        if (mode == 0)
            game->state = 6;
        else if (mode == 1)
            game->state = 9;
    }
    return result;
}

// @verified - p
std::intptr_t ToggleStreetFighterUI() {
    if (g_fightActive)
        return reinterpret_cast<std::intptr_t>(check_dead_status(1));
    g_hasColouredText = 0;
    vfFreeStageObjects();
    for (FightStyleSlot &style : g_pFightStage->styles) {
        if (!style.file)
            continue;
        HLib::FreeTaskData(style.file);
        style.style = 0;
    }
    if (!g_is_vfm_active) {
        FreeBattleGaugeSprites();
        FreeLoadSprites();
    }
    g_fightActive = 0;
    g_fightUiActive = 0;
    if (g_pFightStage->smokeInitTask)
        HLib::DestroyTask(g_pFightStage->smokeInitTask);
    else
        FreeFightSmokeEffects();
    HLib::ClearTaskDataMark(g_pFightStage->enemySlots);
    HLib::FreeTaskData(g_pFightStage->enemySlots);
    g_pFightStage->enemySlots = nullptr;
    HLib::ClearTaskDataMark(g_pFightStage->groups);
    HLib::FreeTaskData(g_pFightStage->groups);
    g_pFightStage->groups = nullptr;
    FightAreaMaskNullsub(0);
    HLib::ClearTaskDataMark(g_pFightStage->generators);
    HLib::FreeTaskData(g_pFightStage->generators);
    g_pFightStage->generators = nullptr;
    DestroyElementByIndex(UIElement::StreetFighterHud);
    HLib::ClearTaskDataMark(g_pFightStage);
    const std::intptr_t result = HLib::FreeTaskData(g_pFightStage);
    g_pFightStage = nullptr;
    return result;
}
void fightInit(std::uint16_t mode) {
    g_fightActive = 0;
    g_fightUiActive = 0;
    const std::uint16_t requested = static_cast<std::uint16_t>(mode | 0x80U);
    SetLoadingPhaseFlags(GetLoadingPhaseFlags() & ~LoadingPhaseFightInitMask);
    g_fightModeFlags = static_cast<std::uint16_t>((requested & 0xff7fU) | 8U);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1233, 0);
}

// @verified - p
std::uint64_t GetFightModeFlags() {
    const std::uint32_t loading = GetLoadingPhaseFlags();
    std::uint16_t flags = g_fightModeFlags;
    if (loading & 0x20U)
        flags |= 1U;
    if (loading & 0x40U)
        flags |= 2U;
    if (loading & 0x80U)
        flags |= 4U;
    return flags;
}

// Clear the loading mask conditionally; retain the WORD transform on every call.
std::uint64_t SetFightModeFlags(std::uint16_t flags) {
    const std::uint16_t previous = g_fightModeFlags;
    if (flags & 0x80U)
        SetLoadingPhaseFlags(GetLoadingPhaseFlags() & ~LoadingPhaseFightInitMask);
    g_fightModeFlags = static_cast<std::uint16_t>((flags & 0xff77U) | 8U);
    return previous;
}

// @verified - p
FGST_t *AllocateFGST() {
    g_pFightStage = static_cast<FGST_t *>(HLib::AllocateTaskData(sizeof(FGST_t), TASK_FGST));
    HLib::AssignDynamicAlignmentForTaskData(g_pFightStage);
    for (std::size_t i = 0; i < 2; ++i) {
        g_pFightStage->spawnPoints[i] = {.parameter = 0.0f, .x = 0.0f, .z = 0.0f};
        g_pFightStage->teamRespawnTimers[i] = 0;
    }
    g_pFightStage->spawnPointCount = 0;
    g_pFightStage->enemySlotCount = 0;
    g_pFightStage->formationCount[1] = 0;
    g_pFightStage->formationCount[0] = 0;
    g_pFightStage->readyEnemySlots[1] = 0;
    g_pFightStage->readyEnemySlots[0] = 0;
    g_pFightStage->activeFightersByTeam[1] = 0;
    g_pFightStage->activeFightersByTeam[0] = 0;
    g_pFightStage->groupCounters[1] = 0;
    g_pFightStage->groupCounters[0] = 0;
    g_pFightStage->groupCounters[3] = 0;
    g_pFightStage->groupCounters[2] = 0;
    g_pFightStage->secondaryGroupCounters[1] = 0;
    g_pFightStage->secondaryGroupCounters[0] = 0;
    g_pFightStage->secondaryGroupCounters[3] = 0;
    g_pFightStage->secondaryGroupCounters[2] = 0;
    g_pFightStage->useCenterBias = 0;
    g_pFightStage->accumulatedHp = 0;
    g_pFightStage->battleFlags = 0;
    g_pFightStage->packData = nullptr;
    g_pFightStage->smokeInitTask = nullptr;
    for (std::size_t i = 0; i < 8; ++i) {
        g_pFightStage->styles[i].style = 0;
        g_pFightStage->styles[i].file = nullptr;
    }
    return g_pFightStage;
}

// @verified - p
void AllocateFCMG(std::uint8_t count) {
    if (count > 16)
        count = 16;
    if (count == 0)
        count = 1;
    g_pFightStage->enemySlots = static_cast<FCMG_t *>(HLib::AllocateTaskData(sizeof(FCMG_t) * count, TASK_FCMG));
    HLib::AssignDynamicAlignmentForTaskData(g_pFightStage->enemySlots);
    g_pFightStage->enemyCount = count;
    for (std::uint8_t i = 0; i < count; ++i)
        g_pFightStage->enemySlots[i].character = nullptr;
}

// @verified - p
void AllocateFRMG(std::uint8_t count) {
    if (count > 127)
        count = 127;
    if (count == 0)
        count = 1;
    g_pFightStage->groups = static_cast<FRMG_t *>(HLib::AllocateTaskData(sizeof(FRMG_t) * count, TASK_FRMG));
    HLib::AssignDynamicAlignmentForTaskData(g_pFightStage->groups);
    g_pFightStage->groupCount = count;
    for (std::uint8_t i = 0; i < count; ++i) {
        FRMG_t &group = g_pFightStage->groups[i];
        group.character = nullptr;
        group.activationState = 0xffff0000U;
        group.spawnYaw = 0xffffU;
        group.animation = 0xffffU;
        group.assignment = 0xffU;
        group.activationFlags = 0;
    }
    g_vfFormationGroupIndex = 99;
}

// @verified - p
void AllocateFGEN(std::uint8_t count) {
    if (count > 127)
        count = 127;
    if (count == 0)
        count = 1;
    g_pFightStage->generators = static_cast<FGEN_t *>(HLib::AllocateTaskData(sizeof(FGEN_t) * count, TASK_FGEN));
    HLib::AssignDynamicAlignmentForTaskData(g_pFightStage->generators);
    g_pFightStage->generatorCount = count;
    for (std::uint8_t i = 0; i < count; ++i)
        g_pFightStage->generators[i].enabled = 0;
    g_FGEN_count = 0;
}

// @verified - p
FGST_t *vfAddGeneratorPoint(float x, float y, float z) {
    FGEN_t &generator = g_pFightStage->generators[g_FGEN_count];
    generator.position = {.x = x, .y = y, .z = z};
    generator.enabled = 1;
    generator.other = 0;
    ++g_FGEN_count;
    return g_pFightStage;
}

// @verified - p
FGST_t *vfCopyPosFromEntity(std::uint8_t mode, Character *character, float x, float z, float parameter) {
    Vector3 position{};
    if (mode == 0)
        position = {.x = x, .y = 0.0f, .z = z};
    else if (mode == 1)
        position = GetCharacterWorldPosition(character);
    FightSpawnPoint &point = g_pFightStage->spawnPoints[g_pFightStage->spawnPointCount];
    point.x = position.x;
    point.z = position.z;
    point.parameter = parameter;
    ++g_pFightStage->spawnPointCount;
    return g_pFightStage;
}

// @verified - p
void AllocateFBIT() {
    g_pFightStage->smokeInitTask = HLib::CreateNewCurrentTaskWithoutParameter(FB_CallBack_InitTask_Callback, 8, 2, TASK_FBIT.name);
    FBSM_t *const smoke = static_cast<FBSM_t *>(HLib::AllocateTaskData(sizeof(FBSM_t), TASK_FBSM));
    HLib::AssignDynamicAlignmentForTaskData(smoke);
    g_pFightStage->smoke = smoke;
    smoke->activeCount = 0;
    for (std::size_t i = 0; i < fightSmokeSlotCount; ++i) {
        smoke->effects[i] = invalidEffect;
        smoke->slots[i].effect = 0xffffU;
        smoke->slots[i].remainingTicks = 0.0f;
        smoke->slots[i].birthRate = 0.0f;
    }
}

// @verified - p
void FB_CallBack_InitTask_Callback(void *) {
    std::uint64_t totalFree;
    std::uint64_t largestFree;
    HLib::TryGetFreeHeapStats(&totalFree, &largestFree);
    if (largestFree > 0x1200U) {
        InitializeFightSmokeEffects();
        HLib::DestroyCurrentTask();
        g_pFightStage->smokeInitTask = nullptr;
    }
}

// @verified - p
void InitializeFightSmokeEffects() {
    FBSM_t *const smoke = g_pFightStage->smoke;
    const Vector3 zero{};
    smoke->activeCount = 0;
    for (std::size_t i = 0; i < fightSmokeSlotCount; ++i)
        smoke->effects[i] = static_cast<std::uint32_t>(CreateSmokeEffect(zero, zero, 40, 0.0f, shen_name{}, 4));
}

// @verified - p
void UpdateFightSmokeEffects() {
    if (g_pFightStage->smokeInitTask != nullptr)
        return;
    FBSM_t *const smoke = g_pFightStage->smoke;
    for (std::size_t i = 0; i < fightSmokeSlotCount; ++i) {
        FightSmokeSlot &slot = smoke->slots[i];
        if (slot.effect == 0xffffU)
            continue;
        slot.birthRate -= 0.025f;
        if (slot.birthRate < 0.0f)
            slot.birthRate = 0.0f;
        slot.remainingTicks -= shenGetScaledDeltaTickTime(1.0f);
        if (slot.remainingTicks > 0.0f) {
            SetParticleEffectBirthRate(0, static_cast<std::uint8_t>(slot.effect), slot.birthRate);
        } else {
            slot.remainingTicks = 0.0f;
            slot.effect = 0xffffU;
            --smoke->activeCount;
        }
    }
}

// @verified - p
void FreeFightSmokeEffects() {
    FBSM_t *const smoke = g_pFightStage->smoke;
    for (std::size_t i = 0; i < fightSmokeSlotCount; ++i) {
        if (smoke->effects[i] == invalidEffect)
            continue;
        const std::uint8_t effect = static_cast<std::uint8_t>(smoke->effects[i]);
        SuspendParticleEffect(0, effect);
        FreeParticleEffect(0, effect);
    }
    HLib::ClearTaskDataMark(smoke);
    HLib::FreeTaskData(smoke);
    g_pFightStage->smoke = nullptr;
}

} // namespace shenmue::s2
