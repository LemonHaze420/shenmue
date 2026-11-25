#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/fight_stage.hpp"
#include "s2/fight_blur_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/time_hud_dc.hpp"
#include "s2/pause.hpp"
#include "platform/dc/sdk.hpp"
#include <cmath>
#include "s2/character_placement_dc.hpp"
#include "s2/hand_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/pad_control_dc.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/screen_film.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/character_yaw_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/conversation_playback_dc.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/blur_figure_pose_dc.hpp"
#include "s2/battle_qte_sprites.hpp"
#include "s2/date_time.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/save_screen_state_dc.hpp"
#include "s2/camera_state_query_dc.hpp"
#include "s2/frame_dc.hpp"
#include "s2/reflector_board_dc.hpp"
#include "s2/character_alpha_dc.hpp"
#include <cstring>
#else
#include "s2/fight_stage.hpp"

#include "s2/camera.hpp"
#include "s2/map_control.hpp"
#include "s2/battle_qte_sprites.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/face.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/gauge.hpp"
#include "s2/hand.hpp"
#include "s2/input.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/model_component.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/ui.hpp"

#include <algorithm>
#include <bit>
#include <limits>
#include <cmath>
#include <cstring>
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

std::uint8_t g_savedFightBlurFlag{};

std::int32_t g_fightCombatCounterSnapshot{};

namespace {

std::int32_t savedPostFxBank{};

std::uint16_t postBattleWaitFrames;

float accumulatedFightBlur{};

std::uint8_t blurModelFlag{}, blurModelPreviousFlag{};

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
namespace {

template <class T> T FightValue(const void *owner, std::size_t offset);
void UpdateFightCameraDialogue(VFMA_t *game);
void UpdateFightGaugeAnimation(VFActor *actor);
void SetupFightReflectorBoard(void *parameter);
void UpdateFightGauge(void *parameter);
void adjustStreetFighterHUD();
void DrawFightHud(VFMA_t *game);
void PublishFightHud(VFMA_t *game);
void UpdateFightTimers(VFMA_t *game);
void UpdateFormationRuntimeFlags(VFMA_t *game);
void SetOtherFightCharaAlphas(VFActor *actor, float alpha);
void UpdateFightCharaAlphas(VFMA_t *game);
void AccumulateFightCombatStats(VFMA_t *game);
void ReleaseFight(VFMA_t *game);
void vfBeginScriptedFormationMotion(VFMA_t *game);
void vfReleaseUnheldScriptedFormation(VFMA_t *game);

} // namespace

void vfFightNullsub();
void vfStartCameraDialogue(VFMA_t *game, std::uint32_t id);
std::intptr_t vfmQueueFightStart(std::uint8_t startSlot, std::uint8_t maximumEnemies, float knockback, std::uint8_t type, std::int32_t guardEffectSeconds);
void vfm_Callback_FightTaskEntry_Callback(void *parameter);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// Native vfSetupFightUiAndCameraState: camera half of the native UI/camera admission transaction.
void vfSetupFightCameraState(VFMA_t *game) {
    using enum CameraState;
    const auto map = g_currentMap.name;
    if (((map - SHEN_NAME(QJBT)) & 0xfffffcffU) == 0 && map != SHEN_NAME(QMBT))
        game->cameraParameter = 13;
    float distance = 0, height = 0;
    game->savedPerspective = static_cast<std::int16_t>(camCtrlGetPerspective());
    auto fov = game->savedPerspective;
    auto mode = CameraMotionMode::Default;
    const auto flags = g_pFightStage->battleFlags;
    if (flags & 0x80)
        mode = CameraMotionMode::FirstPerson;
    else if (game->type == 11)
        mode = CameraMotionMode::FieldFight;
    else if ((flags & 2) == 0) {
        switch (map) {
        case SHEN_NAME(QFRR):
        case SHEN_NAME(QGBT):
            distance = 8.0799999f;
            height = 1.7f;
            fov = 10922;
            mode = CameraMotionMode::FieldFight;
            break;
        case SHEN_NAME(QJBT):
        case SHEN_NAME(QKBT):
        case SHEN_NAME(QLBT):
        case SHEN_NAME(QSFC):
        case SHEN_NAME(WEG0):
            distance = 2.8f;
            height = 2.5f;
            fov = 10922;
            mode = CameraMotionMode::FieldFight;
            break;
        case SHEN_NAME(QSFD):
        case SHEN_NAME(QTB1):
        case SHEN_NAME(AB00):
            distance = 2.8f;
            height = 2.5f;
            fov = 7281;
            mode = CameraMotionMode::Default1;
            break;
        case SHEN_NAME(Q100):
        case SHEN_NAME(QSFA):
        case SHEN_NAME(QSFB):
        case SHEN_NAME(BTST):
            distance = 8.0799999f;
            height = 1.7f;
            fov = 3640;
            mode = CameraMotionMode::Default1;
            break;
        default:
            fov = 10922;
            break;
        }
    }
    game->cameraDistance = distance;
    game->cameraHeight = height;
    game->perspective = fov;
    camCtrlSetInterpModeTo(mode);
    camCtrlSetFightDistance(distance);
    camCtrlSetFightHeight(height);
    if ((flags & 2) == 0 && game->type != 9) {
        camCtrlSetPerspective(static_cast<std::uint16_t>(fov));
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(ChaseGoal);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

void FreeVFGW(FightActorRuntimePrefix *actor);

void vfReleaseUnheldScriptedFormation(FightRuntimePrefix *game);

void vfTransitionAllEnemySlotsToInactive(FightRuntimePrefix *game);

void FreeFightFormations();

void FreeFightGenerators();

void FreeFightStage();

std::int32_t g_fightCombatCounterSnapshot = 0;

namespace {

std::uint16_t postBattleWaitFrames;

} // namespace

void vfm_Callback_FightTaskEntry_Callback(void *parameter);

void vfSetupFightUiAndCameraState(FightRuntimePrefix *game);

bool vfEnemySlotResetAndRestartCycle(FightRuntimePrefix *game, FCMG_t *slot);

std::uint8_t vfRegisterContactSlot(FightRuntimePrefix *game, FCMG_t *slot);

void vfInitializeFightDialogue(FightRuntimePrefix *game);

HLib::Task *Queue_vfc_VF_RobCollision(FightRuntimePrefix *game);

HLib::Task *vfg_Callback_GameInfoEntry_Enqueue(FightRuntimePrefix *game);

void vfUpdatePairingsAndDistances(FightRuntimePrefix *game);

void vfInitializeFightActor(FightRuntimePrefix *game, std::uint8_t index);

void UpdateFightCameraDialogue(FightRuntimePrefix *game);

void vfUpdateMain(FightActorRuntimePrefix *actor);

void vfmaUpdateNextReaction(FightActorRuntimePrefix *actor);

void vfUpdateVFHDState(FightActorRuntimePrefix *actor);

void vfResolvePairInteraction(FightActorRuntimePrefix *actor);

void vfUpdatePoseWithSfx(FightActorRuntimePrefix *actor);

void vfmaSetupDialogueReactionsEtc(FightRuntimePrefix *game);

void SetupFightReflectorBoard(void *parameter);

void UpdateFightGauge(void *parameter);

void ConstructColouredText(std::uint32_t frames, std::int32_t x, std::int32_t y, float depth);

void adjustStreetFighterHUD();

void constructNumStrikesLeft();

void InitializeCameraGoal();

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfUpdateWorldTransformAndFacingAngle(FightActorRuntimePrefix *actor) {
    GetCharacterWorldPosition(actor->character, &actor->position);
    actor->facingAngle = GetCharacterWorldYaw(actor->character);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfApplyYawRecursive(FightActorRuntimePrefix *actor) {
    SetCharacterWorldYaw(actor->character, actor->facingAngle);
    SetCharacterWorldPosition(actor->character, &actor->position);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

template <class T> inline T &FightValue(void *owner, std::size_t offset) {
    return *reinterpret_cast<T *>(static_cast<std::byte *>(owner) + offset);
}

} // namespace

#else

namespace {

template <class T> T FightValue(const void *owner, std::size_t offset) {
    T value;
    std::memcpy(&value, static_cast<const std::byte *>(owner) + offset, sizeof(value));
    return value;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

template <class T> void SetFightValue(void *owner, std::size_t offset, T value) {
    std::memcpy(static_cast<std::byte *>(owner) + offset, &value, sizeof(value));
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

// CVTTSS2SI returns INT32_MIN for an out-of-range or unordered operand.
std::int32_t FightTruncate(float value) {
    return value >= -2147483648.0f && value < 2147483648.0f ? static_cast<std::int32_t>(value) : std::numeric_limits<std::int32_t>::min();
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

VCDS_t *FindCameraDialogue(VFMA_t *game, std::uint32_t id) {
    if (!game->cameraDialogue)
        return nullptr;
    for (auto *record = game->cameraDialogue->records; record; record = record->next)
        if (record->id == id)
            return record;
    return nullptr;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

// @verified - p
void DisableCameraDialogue(VFMA_t *game, std::uint32_t id) {
    if (id)
        if (auto *record = FindCameraDialogue(game, id))
            record->state = 0;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

// @verified - p
void TriggerCameraDialogue(VFMA_t *game, VCDS_t *record) {
    if (record->dialogue == -2) {
        if (record->callback)
            record->callback(&record->callbackArgument);
    } else
        vfStartCameraDialogue(game, record->id);
    if (!(record->flags & 1))
        DisableCameraDialogue(game, record->id);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

// @verified - p
void RetireCameraDialogueRecords() {
    VFMA_t *const game = vfmaGetTaskParam();
    if (!game || !game->cameraDialogue)
        return;
    VCDS_t *record = game->cameraDialogue->records;
    while (record && record->next)
        record = record->next;
    while (record) {
        VCDS_t *const previous = record->previous;
        const std::uint32_t id = record->id;
        VFMA_t *const selectedGame = vfmaGetTaskParam();
        if (selectedGame && selectedGame->cameraDialogue) {
            VCDW_t *const owner = selectedGame->cameraDialogue;
            VCDS_t *const selected = FindCameraDialogue(selectedGame, id);
            if (selected && id) {
                if (selected->previous)
                    selected->previous->next = selected->next;
                else
                    owner->records = selected->next;
                if (selected->next)
                    selected->next->previous = selected->previous;
                HLib::FreeHeapSpace(selected, static_cast<std::int32_t>(TASK_VCDS.name));
            }
        }
        record = previous;
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

// Authored VCDS conditions run before the six actor update passes.
// @verified - p
void UpdateFightCameraDialogue(VFMA_t *game) {
    auto *owner = game->cameraDialogue;
    if (!owner)
        return;
    auto *begin = owner->records;
    if (owner->selected != -1) {
        if (owner->state) {
            begin = FindCameraDialogue(game, owner->state);
            if (begin) {
                if (begin->state == 2)
                    begin->state = 1;
                if (!(begin->flags & 1))
                    DisableCameraDialogue(game, begin->id);
            }
        }
        owner->flags = (owner->flags & 0xffff0000U) | owner->parameters;
        owner->state = 0xff;
        owner->counter = 60;
        SetFightValue(game, 0x31a, std::int16_t{-1});
        scenCvsCtrlFree();
        vfSetFightDialogue(game, owner->selected);
        owner->selected = -1;
    }
    auto expiration = static_cast<std::uint16_t>(owner->flags >> 16);
    if (expiration && --expiration == 0) {
        owner->flags &= 0xffff;
        RetireCameraDialogueRecords();
        return;
    }
    owner->flags = (owner->flags & 0xffff) | (std::uint32_t(expiration) << 16);
    // A failed slot lookup continues past the record without its flags-2 retirement, as native does.
    for (auto *record = begin; record; record = record->next) {
        if (record->state != 1 || record->id == owner->state)
            continue;
        auto *self = vfGetActorFromSlot(game, record->character);
        auto *other = vfGetActorFromSlot(game, record->otherCharacter);
        bool match = false, repeat = true;
        const auto accepts = [&](const VFActor *actor) { return !record->otherCharacter.name || record->otherCharacter.name == actor->slot->id; };
        const auto actorMatches = [&](auto predicate) {
            for (std::uint8_t i = 0; i < 6; ++i)
                if (game->characters[i] && predicate(game->actors[i]))
                    return true;
            return false;
        };
        // Priorities compare as signed words; the chance is a double product.
        const auto admits = [&]() {
            return (!owner->state || static_cast<std::int16_t>(owner->flags) > static_cast<std::int16_t>(record->priority)) &&
                double(record->probability) > double(IncrementSharedPrngFloat()) * 100.0;
        };
        switch (record->condition) {
        case 1:
            match = (owner->enabled >> 32) != 0 && (owner->enabled >> 32) % record->period == 0;
            repeat = false;
            break;
        case 2:
            if (!self)
                continue;
            match = double(record->healthPercent) > double(self->hp / float(self->initialHp)) * 100.0;
            repeat = false;
            break;
        case 3:
            if (!self)
                continue;
            match = float(record->health) > self->hp;
            repeat = false;
            break;
        case 4:
            match = actorMatches([&](VFActor *actor) {
                return (actor->hitStateFlags & VFHSF_SpecialGuardOrParry) && actor->actor && record->character.name == actor->actor->slot->id && accepts(actor);
            });
            break;
        case 5:
            if (!self)
                continue;
            match = actorMatches([&](VFActor *actor) {
                return (actor->hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_AirborneOrDowned)) && (actor->hitStateFlags & VFHSF_AttackActive) &&
                    actor->actor && record->character.name == actor->actor->slot->id && accepts(actor);
            });
            break;
        case 6: {
            VFActor *defeated = other;
            if (record->otherCharacter.name) {
                if (!other)
                    continue;
                if (!(std::fabs(other->hp) < 0.00000011920929f) || record->otherCharacter.name != other->slot->id || !other->actor ||
                    record->character.name != other->actor->slot->id)
                    break;
            } else {
                // The scan leaves the last live actor selected, not the last defeated match.
                bool found = false;
                for (std::uint8_t i = 0; i < 6; ++i)
                    if (game->characters[i]) {
                        defeated = game->actors[i];
                        if (std::fabs(defeated->hp) < 0.00000011920929f && defeated->actor && record->character.name == defeated->actor->slot->id)
                            found = true;
                    }
                if (!found)
                    break;
            }
            if (record->lastCharacter.name == defeated->slot->id)
                continue;
            ++record->occurrences;
            if ((record->repeatEvery == -1 || record->occurrences % record->repeatEvery == 0) && admits()) {
                record->lastCharacter = shen_name{defeated->slot->id};
                TriggerCameraDialogue(game, record);
            }
            break;
        }
        case 7:
            match = actorMatches([&](VFActor *actor) {
                return actor->queuedReactionEvent == 30 && actor->actor && record->character.name == actor->actor->slot->id && accepts(actor);
            });
            break;
        case 8:
            if (!self)
                continue;
            if (record->animation == record->previousAnimation) {
                if (self->currentAnimation != record->previousAnimation)
                    record->previousAnimation = 0;
            } else if (self->currentAnimation == record->animation) {
                if (record->otherCharacter.name) {
                    if (!other)
                        continue;
                    if (record->otherCharacter.name != other->slot->id)
                        break;
                }
                record->previousAnimation = record->animation;
                match = true;
            }
            break;
        case 9:
            if (!self)
                continue;
            if (record->otherCharacter.name) {
                if (!other)
                    continue;
                const auto x = self->contactPosition.x - other->contactPosition.x;
                const auto z = self->contactPosition.z - other->contactPosition.z;
                match = !(std::sqrt(x * x + z * z) <= record->distance);
            } else {
                match = true;
                for (std::uint8_t i = 0; i < 6; ++i)
                    if (game->characters[i] && i != self->groupIndex && !(record->distance <= self->contactDistances[i]))
                        match = false;
            }
            break;
        case 10:
            if (!self)
                continue;
            match = actorMatches([&](VFActor *actor) {
                auto *paired = actor->secondActor;
                return (self->hitStateFlags & VFHSF_MidAirRecovery) && (actor->hitStateFlags & VFHSF_AirborneOrDowned) &&
                    !(self->hitStateFlags & VFHSF_Unknown100000) && paired &&
                    static_cast<std::uint16_t>(paired->animationTime) == paired->parameter33c[1] + 1 && record->character.name == paired->slot->id &&
                    accepts(actor);
            });
            break;
        case 11:
            if (!self)
                continue;
            if ((self->previousHitStateFlags & VFHSF_Unknown100000) || !(self->hitStateFlags & VFHSF_Unknown100000) || !self->secondActor)
                break;
            if (record->otherCharacter.name) {
                if (record->otherCharacter.name != self->secondActor->slot->id)
                    break;
                if (!other)
                    continue;
            }
            match = true;
            break;
        case 12:
            if (!self)
                continue;
            match = (self->hitStateFlags & (VFHSF_AttackActive | VFHSF_AirborneOrDowned)) == 0x100 && self->currentAnimation != self->previousAnimation &&
                (!record->otherCharacter.name || (self->actor && record->otherCharacter.name == self->actor->slot->id));
            break;
        case 13:
            if (!self)
                continue;
            if ((self->activeMotionFlags & VFSMF_Unknown200000) && accepts(self)) {
                match = true;
                break;
            }
            [[fallthrough]];
        case 16:
            if (!self)
                continue;
            if ((self->previousHitStateFlags & VFHSF_ForcedReaction) || !(self->hitStateFlags & VFHSF_ForcedReaction))
                break;
            if (record->otherCharacter.name) {
                if (!self->actor || record->otherCharacter.name != self->actor->slot->id)
                    break;
                if (!other)
                    continue;
                if (!(other->hitStateFlags & VFHSF_AttackActive))
                    break;
            }
            match = true;
            break;
        case 14:
            if (!self)
                continue;
            match = (vfSelectHitReactionSequence(self) & record->reactionMask) &&
                (!record->otherCharacter.name || (self->actor && record->otherCharacter.name == self->actor->slot->id));
            break;
        case 15:
            if (!self)
                continue;
            if (record->idleFrames && record->idleFrames % record->period == 0) {
                ++record->occurrences;
                if (admits())
                    TriggerCameraDialogue(game, record);
            }
            record->idleFrames = (self->inputFlags & 15) ? 0 : record->idleFrames + 1;
            break;
        case 17:
            if (!self)
                continue;
            match = (self->hitResolutionFlags & 1) && (!record->otherCharacter.name || (self->actor && record->otherCharacter.name == self->actor->slot->id));
            break;
        default:
            break;
        }
        if (match) {
            ++record->occurrences;
            if ((!repeat || record->repeatEvery == -1 || record->occurrences % record->repeatEvery == 0) && admits())
                TriggerCameraDialogue(game, record);
        }
        if (record->flags & 2)
            DisableCameraDialogue(game, record->id);
    }
    if (owner->counter)
        --owner->counter;
    else if (FightValue<std::uint16_t>(game, 0x31a) == 0xffff && !cvsHasValidConverseData() && owner->state) {
        if (auto *record = FindCameraDialogue(game, owner->state)) {
            if (record->state == 2)
                record->state = 1;
            ++record->completions;
        }
        owner->flags = (owner->flags & 0xffff0000) | 0xffff;
        owner->state = 0;
    }
    owner->enabled += std::uint64_t{1} << 32;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void vfUpdateWorldTransformAndFacingAngle(FightActorRuntimePrefix *actor);
void vfApplyYawRecursive(FightActorRuntimePrefix *actor);
void vfInitSfxSlotState(FightRuntimePrefix *game);
void vfDecrementSfxSlotTimers(FightRuntimePrefix *game);
void UpdateFightTimers(FightRuntimePrefix *game);
void UpdateFormationRuntimeFlags(FightRuntimePrefix *game);
void SetOtherFightCharaAlphas(FightActorRuntimePrefix *actor, float alpha);
void UpdateFightCharaAlphas(FightRuntimePrefix *game);
void vfUpdateAuthCameraFrame(FightRuntimePrefix *game);
void AccumulateFightCombatStats(FightRuntimePrefix *game);
void vfFreeVFHDSpace(FightRuntimePrefix *game);
std::int32_t vfResetInstructionMode(std::uint16_t motion);
void vfReleaseFightInput(FightRuntimePrefix *game);
void FreeFightStyles();
void FreeFightContactSlots();
void FreeMarkedFightTaskData(void *data);
void FreeFGTM(FightActorRuntimePrefix *actor);
void vfRestorePostFightUIState(FightRuntimePrefix *game);
void ReleaseFight(FightRuntimePrefix *game);
void vfmQueueFightStart(std::uint8_t startSlot, std::uint8_t maximumEnemies, float knockback, std::uint8_t type, std::int32_t guardEffectSeconds);
void vfSetupFightUiAndCameraState(FightRuntimePrefix *game);
void vfSetPairedActor(FightRuntimePrefix *game, std::uint8_t index, std::uint8_t target);
void vfSetTargetActor(FightRuntimePrefix *game, std::uint8_t index, std::uint8_t target);
void vfUpdatePairingsAndDistances(FightRuntimePrefix *game);
void UpdateFightGaugeAnimation(FightActorRuntimePrefix *actor);
void vfDrawFightGaugeOrb(FightActorRuntimePrefix *actor, std::uint16_t index, std::uint32_t alpha, std::uint8_t icon);
void vfDrawFightRoundMarkers(FightRuntimePrefix *game, std::uint32_t alpha);
void vfDrawFightActorGauge(FightActorRuntimePrefix *actor);
void UpdateFightGauge(void *parameter);
void SetupFightReflectorBoard(void *parameter);
void vfm_Callback_FightTaskEntry_Callback(void *parameter);

namespace {

template <class T> T &FightValue(void *owner, std::size_t offset);
FightRuntimePrefix *AllocateVFMA();
void *AllocateVFHD(FightRuntimePrefix *game);
inline void AddFightStatistic(std::size_t offset, std::uint32_t amount);
void vfFightNullsub();
void vfBeginScriptedFormationMotion(FightRuntimePrefix *game);
void vfSetupFightInput(FightRuntimePrefix *game);

} // namespace
#else
void vfSetupFightCameraState(VFMA_t *game);
void vfFightNullsub();
std::uint32_t vfRegisterCameraDialogue(VCDS_t *record);
void vfStartCameraDialogue(VFMA_t *game, std::uint32_t id);
std::uint32_t AllocateBattleDialogue(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, std::int16_t repeatEvery,
    float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition2(
    std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, float healthPercent, float probability);
std::uint32_t AllocateBattleDialogueCondition3(
    std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, std::uint16_t health, float probability);
std::uint32_t AllocateBattleDialogueCondition5(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition6(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition9(
    std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, float distance, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition12(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition16(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocateMotionBattleDialogue(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, std::uint16_t animation,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition7(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocateReactionBattleDialogue(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::uint32_t reactionMask, std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition15(
    std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, std::uint32_t period, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition10(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition13(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocatePeriodicBattleDialogue(
    std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, std::uint32_t period, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition11(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
std::uint32_t AllocateBattleDialogueCondition17(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags);
VCDS_t *SetBattleDialogueFlags(std::uint32_t id, std::uint16_t flags);
VCDS_t *AssignBattleDialogueCallback(std::uint32_t id, std::int64_t (*callback)(void *));
VCDS_t *SuspendBattleDialogue(std::uint32_t id);
VCDS_t *ResumeBattleDialogue(std::uint32_t id);
std::int64_t QueueBattleDialogue(std::int16_t priority, std::int16_t dialogue);
void BeginFightDialogueRetirement();
std::uint32_t GetFightTaskFlags();
VFMA_t *SetActiveFightModeFlag();
std::uint64_t GetFightCharacterFlag(std::uint8_t mode, Character *character, float value);
void blurInit();
std::intptr_t vfmQueueFightStart(std::uint8_t startSlot, std::uint8_t maximumEnemies, float knockback, std::uint8_t type, std::int32_t guardEffectSeconds);
void vfm_Callback_FightTaskEntry_Callback(void *parameter);

namespace {

template <class T> T FightValue(const void *owner, std::size_t offset);
template <class T> void SetFightValue(void *owner, std::size_t offset, T value);
std::int32_t FightTruncate(float value);
VCDS_t *FindCameraDialogue(VFMA_t *game, std::uint32_t id);
void DisableCameraDialogue(VFMA_t *game, std::uint32_t id);
void TriggerCameraDialogue(VFMA_t *game, VCDS_t *record);
void RetireCameraDialogueRecords();
void UpdateFightCameraDialogue(VFMA_t *game);
void UpdateFightTimers(VFMA_t *game);
bool SetupContactSlot(VFMA_t *game, FRMG_t *group);
void ActivateBlockedFormations(VFMA_t *game, FCMG_t *retired);
void UpdateDefeatAndRespawn(VFMA_t *game);
void UpdateEnemySlotActivation(VFMA_t *game);
void UpdateFormationRuntimeFlags(VFMA_t *game);
void SetOtherFightCharaAlphas(VFActor *actor, float alpha);
void UpdateFightCharaAlphas(VFMA_t *game);
void AdvanceFightFrame(VFMA_t *game);
void FreeFightCameraDialogue(VFMA_t *game);
void AccumulateFightCombatStats(VFMA_t *game);
void ReleaseFight(VFMA_t *game);
void vfBeginScriptedFormationMotion(VFMA_t *game);
void vfReleaseUnheldScriptedFormation(VFMA_t *game);
void UpdateFightGaugeAnimation(VFActor *actor);
std::int32_t PrimaryFightGaugeState(const VFActor *actor);
void UpdateFightGauge(void *parameter);
void adjustStreetFighterHUD();
void SetupFightReflectorBoard(void *parameter);
void DrawFightHud(VFMA_t *game);
void PublishFightHud(VFMA_t *game);

} // namespace
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfInitSfxSlotState(FightRuntimePrefix *game) {
    FightValue<std::uint8_t>(game, 0x1c0) = 0;
    FightValue<std::uint8_t>(game, 0x1c1) = 0;
    FightValue<std::uint16_t>(game, 0x1bc) = FightValue<std::uint16_t>(game, 0x1be) = 0;
    for (std::uint8_t index = 0; index < 6; ++index) {
        (&FightValue<std::uint8_t>(game, 0x1c2))[index] = 0;
        FightValue<std::uint32_t>(&FightValue<std::uint32_t>(game, 0x198), index * 4) = 0;
        (&FightValue<std::uint8_t>(game, 0x1cb))[index] = 0;
        if (game->characters[index])
            FightValue<std::uint8_t>(game->actors[index]->slot, 0x5a) = 255;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfDecrementSfxSlotTimers(FightRuntimePrefix *game) {
    if (FightValue<std::uint16_t>(game, 0x1bc))
        --FightValue<std::uint16_t>(game, 0x1bc);
    if (FightValue<std::uint16_t>(game, 0x1be))
        --FightValue<std::uint16_t>(game, 0x1be);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
void vfFightNullsub() {
}

} // namespace

void vfFightUnusedNullsub() {
}

#else

// The shared native nullsub at the fight start, result, post-result, release and learned-move close boundaries.
void vfFightNullsub() {
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// Attach an allocated native VCDS record to its VFMA owner.
std::uint32_t vfRegisterCameraDialogue(VCDS_t *record) {
    auto *game = vfmaGetTaskParam();
    if (!game || !game->cameraDialogue)
        return 0;
    auto *owner = game->cameraDialogue;
    record->state = 1;
    record->completions = 0;
    record->occurrences = 0;
    record->idleFrames = 0;
    record->lastCharacter = shen_name{};
    record->callback = nullptr;
    record->previousDialogue = -1;
    record->callbackArgument = 0;
    if (record->probability != 0)
        record->probability += .1f;
    auto *tail = owner->records;
    while (tail && tail->next)
        tail = tail->next;
    record->previous = tail;
    if (tail)
        tail->next = record;
    else
        owner->records = record;
    record->next = nullptr;
    record->id = static_cast<std::uint32_t>(owner->enabled);
    owner->enabled = (owner->enabled & 0xffffffff00000000ULL) | std::uint32_t(record->id + 1);
    return record->id;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// Callback-selected authored dialogue admission.
void vfStartCameraDialogue(VFMA_t *game, std::uint32_t id) {
    auto *owner = game->cameraDialogue;
    if (!owner || !id)
        return;
    if (owner->state)
        if (auto *previous = FindCameraDialogue(game, owner->state)) {
            previous->state = 1;
            if (!(previous->flags & 1))
                DisableCameraDialogue(game, previous->id);
        }
    auto *record = FindCameraDialogue(game, id);
    record->state = 2;
    if (record->callback) {
        const auto selected = static_cast<std::int16_t>(record->callback(&record->callbackArgument));
        if (static_cast<std::uint16_t>(selected + 2) <= 1) {
            record->state = 1;
            return;
        }
        record->dialogue = selected;
    }
    owner->flags = (owner->flags & 0xffff0000) | record->priority;
    owner->state = static_cast<std::uint8_t>(id);
    owner->counter = 60;
    SetFightValue(game, 0x31a, std::int16_t{-1});
    scenCvsCtrlFree();
    vfSetFightDialogue(game, record->dialogue);
    record->previousDialogue = record->dialogue;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint32_t AllocateBattleDialogue(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, std::int16_t repeatEvery,
    float probability, std::uint16_t flags) {
    VCDS_t *const record = static_cast<VCDS_t *>(HLib::AllocateTaskData(sizeof(VCDS_t), TASK_VCDS));
    HLib::AssignDynamicAlignmentForTaskData(record);
    record->repeatEvery = repeatEvery;
    record->probability = probability;
    record->condition = 4;
    record->flags = flags;
    record->priority = static_cast<std::uint16_t>(priority);
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    vfRegisterCameraDialogue(record);
    return record->id;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint32_t AllocateBattleDialogueCondition2(
    std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, float healthPercent, float probability) {
    VCDS_t *const record = static_cast<VCDS_t *>(HLib::AllocateTaskData(sizeof(VCDS_t), TASK_VCDS));
    HLib::AssignDynamicAlignmentForTaskData(record);
    record->probability = probability;
    record->healthPercent = healthPercent;
    record->flags = 0;
    record->repeatEvery = 1;
    record->condition = 2;
    record->priority = static_cast<std::uint16_t>(priority);
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    vfRegisterCameraDialogue(record);
    return record->id;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint32_t AllocateBattleDialogueCondition3(
    std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, std::uint16_t health, float probability) {
    VCDS_t *const record = static_cast<VCDS_t *>(HLib::AllocateTaskData(sizeof(VCDS_t), TASK_VCDS));
    HLib::AssignDynamicAlignmentForTaskData(record);
    record->flags = 0;
    record->health = health;
    record->repeatEvery = 1;
    record->probability = probability;
    record->condition = 3;
    record->priority = static_cast<std::uint16_t>(priority);
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    vfRegisterCameraDialogue(record);
    return record->id;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint32_t AllocateBattleDialogueCondition5(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    VCDS_t *const record = static_cast<VCDS_t *>(HLib::AllocateTaskData(sizeof(VCDS_t), TASK_VCDS));
    HLib::AssignDynamicAlignmentForTaskData(record);
    record->flags = flags;
    record->repeatEvery = repeatEvery;
    record->probability = probability;
    record->condition = 5;
    record->priority = static_cast<std::uint16_t>(priority);
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    vfRegisterCameraDialogue(record);
    return record->id;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint32_t AllocateBattleDialogueCondition6(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    VCDS_t *const record = static_cast<VCDS_t *>(HLib::AllocateTaskData(sizeof(VCDS_t), TASK_VCDS));
    HLib::AssignDynamicAlignmentForTaskData(record);
    record->flags = flags;
    record->repeatEvery = repeatEvery;
    record->probability = probability;
    record->condition = 6;
    record->priority = static_cast<std::uint16_t>(priority);
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    vfRegisterCameraDialogue(record);
    return record->id;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint32_t AllocateBattleDialogueCondition9(
    std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, float distance, float probability, std::uint16_t flags) {
    VCDS_t *const record = static_cast<VCDS_t *>(HLib::AllocateTaskData(sizeof(VCDS_t), TASK_VCDS));
    HLib::AssignDynamicAlignmentForTaskData(record);
    record->probability = probability;
    record->distance = distance;
    record->flags = flags;
    record->repeatEvery = -1;
    record->condition = 9;
    record->priority = static_cast<std::uint16_t>(priority);
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    vfRegisterCameraDialogue(record);
    return record->id;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint32_t AllocateBattleDialogueCondition12(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    VCDS_t *const record = static_cast<VCDS_t *>(HLib::AllocateTaskData(sizeof(VCDS_t), TASK_VCDS));
    HLib::AssignDynamicAlignmentForTaskData(record);
    record->flags = flags;
    record->repeatEvery = repeatEvery;
    record->probability = probability;
    record->condition = 12;
    record->priority = static_cast<std::uint16_t>(priority);
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    vfRegisterCameraDialogue(record);
    return record->id;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint32_t AllocateBattleDialogueCondition16(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    VCDS_t *const record = static_cast<VCDS_t *>(HLib::AllocateTaskData(sizeof(VCDS_t), TASK_VCDS));
    HLib::AssignDynamicAlignmentForTaskData(record);
    record->flags = flags;
    record->repeatEvery = repeatEvery;
    record->probability = probability;
    record->condition = 16;
    record->priority = static_cast<std::uint16_t>(priority);
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    vfRegisterCameraDialogue(record);
    return record->id;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint32_t AllocateMotionBattleDialogue(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, std::uint16_t animation,
    std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    VCDS_t *const record = static_cast<VCDS_t *>(HLib::AllocateTaskData(sizeof(VCDS_t), TASK_VCDS));
    HLib::AssignDynamicAlignmentForTaskData(record);
    record->previousAnimation = 0;
    record->animation = animation;
    record->repeatEvery = repeatEvery;
    record->probability = probability;
    record->condition = 8;
    record->flags = flags;
    record->priority = static_cast<std::uint16_t>(priority);
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    vfRegisterCameraDialogue(record);
    return record->id;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p: condition7 producer retains its own record writes and registration boundary.
std::uint32_t AllocateBattleDialogueCondition7(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    VCDS_t *const record = static_cast<VCDS_t *>(HLib::AllocateTaskData(sizeof(VCDS_t), TASK_VCDS));
    HLib::AssignDynamicAlignmentForTaskData(record);
    record->flags = flags;
    record->repeatEvery = repeatEvery;
    record->probability = probability;
    record->condition = 7;
    record->priority = static_cast<std::uint16_t>(priority);
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    vfRegisterCameraDialogue(record);
    return record->id;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p: the retail reaction-mask producer uses condition14, independent of the animation condition8.
std::uint32_t AllocateReactionBattleDialogue(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::uint32_t reactionMask, std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    VCDS_t *const record = static_cast<VCDS_t *>(HLib::AllocateTaskData(sizeof(VCDS_t), TASK_VCDS));
    HLib::AssignDynamicAlignmentForTaskData(record);
    record->reactionMask = reactionMask;
    record->repeatEvery = repeatEvery;
    record->probability = probability;
    record->condition = 14;
    record->flags = flags;
    record->priority = static_cast<std::uint16_t>(priority);
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    vfRegisterCameraDialogue(record);
    return record->id;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p: condition15 has a full DWORD period and an unconditional repeatEvery1 write.
std::uint32_t AllocateBattleDialogueCondition15(
    std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, std::uint32_t period, float probability, std::uint16_t flags) {
    VCDS_t *const record = static_cast<VCDS_t *>(HLib::AllocateTaskData(sizeof(VCDS_t), TASK_VCDS));
    HLib::AssignDynamicAlignmentForTaskData(record);
    record->period = period;
    record->probability = probability;
    record->condition = 15;
    record->flags = flags;
    record->priority = static_cast<std::uint16_t>(priority);
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    record->repeatEvery = 1;
    vfRegisterCameraDialogue(record);
    return record->id;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint32_t AllocateBattleDialogueCondition10(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    VCDS_t *const record = static_cast<VCDS_t *>(HLib::AllocateTaskData(sizeof(VCDS_t), TASK_VCDS));
    HLib::AssignDynamicAlignmentForTaskData(record);
    record->flags = flags;
    record->repeatEvery = repeatEvery;
    record->probability = probability;
    record->condition = 10;
    record->priority = static_cast<std::uint16_t>(priority);
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    vfRegisterCameraDialogue(record);
    return record->id;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint32_t AllocateBattleDialogueCondition13(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    VCDS_t *const record = static_cast<VCDS_t *>(HLib::AllocateTaskData(sizeof(VCDS_t), TASK_VCDS));
    HLib::AssignDynamicAlignmentForTaskData(record);
    record->flags = flags;
    record->repeatEvery = repeatEvery;
    record->probability = probability;
    record->condition = 13;
    record->priority = static_cast<std::uint16_t>(priority);
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    vfRegisterCameraDialogue(record);
    return record->id;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint32_t AllocatePeriodicBattleDialogue(
    std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter, std::uint32_t period, float probability, std::uint16_t flags) {
    VCDS_t *const record = static_cast<VCDS_t *>(HLib::AllocateTaskData(sizeof(VCDS_t), TASK_VCDS));
    HLib::AssignDynamicAlignmentForTaskData(record);
    record->period = period;
    record->probability = probability;
    record->condition = 1;
    record->flags = flags;
    record->priority = static_cast<std::uint16_t>(priority);
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    record->repeatEvery = 1;
    vfRegisterCameraDialogue(record);
    return record->id;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint32_t AllocateBattleDialogueCondition11(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    VCDS_t *const record = static_cast<VCDS_t *>(HLib::AllocateTaskData(sizeof(VCDS_t), TASK_VCDS));
    HLib::AssignDynamicAlignmentForTaskData(record);
    record->flags = flags;
    record->repeatEvery = repeatEvery;
    record->probability = probability;
    record->condition = 11;
    record->priority = static_cast<std::uint16_t>(priority);
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    record->previousAnimation = 0;
    vfRegisterCameraDialogue(record);
    return record->id;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint32_t AllocateBattleDialogueCondition17(std::int16_t priority, std::int16_t dialogue, shen_name character, shen_name otherCharacter,
    std::int16_t repeatEvery, float probability, std::uint16_t flags) {
    VCDS_t *const record = static_cast<VCDS_t *>(HLib::AllocateTaskData(sizeof(VCDS_t), TASK_VCDS));
    HLib::AssignDynamicAlignmentForTaskData(record);
    record->flags = flags;
    record->repeatEvery = repeatEvery;
    record->probability = probability;
    record->condition = 17;
    record->priority = static_cast<std::uint16_t>(priority);
    record->dialogue = dialogue;
    record->character = character;
    record->otherCharacter = otherCharacter;
    vfRegisterCameraDialogue(record);
    return record->id;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
VCDS_t *SetBattleDialogueFlags(std::uint32_t id, std::uint16_t flags) {
    VFMA_t *const game = vfmaGetTaskParam();
    if (!game || !game->cameraDialogue)
        return nullptr;
    VCDS_t *record = game->cameraDialogue->records;
    while (record && record->id != id)
        record = record->next;
    if (record && id)
        record->flags = flags;
    return record;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
VCDS_t *AssignBattleDialogueCallback(std::uint32_t id, std::int64_t (*callback)(void *)) {
    VFMA_t *const game = vfmaGetTaskParam();
    if (!game)
        return nullptr;
    VCDS_t *const record = FindCameraDialogue(game, id);
    if (record && id)
        record->callback = callback;
    return record;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p: suspend changes only the selected record's state byte; id0 deliberately performs no write.
VCDS_t *SuspendBattleDialogue(std::uint32_t id) {
    VFMA_t *const game = vfmaGetTaskParam();
    if (!game || !game->cameraDialogue)
        return nullptr;
    VCDS_t *record = game->cameraDialogue->records;
    while (record && record->id != id)
        record = record->next;
    if (record && id)
        record->state = 0;
    return record;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p: resume resets the native word at0x4a, the previous-animation field, not occurrence/completion counts.
VCDS_t *ResumeBattleDialogue(std::uint32_t id) {
    VFMA_t *const game = vfmaGetTaskParam();
    if (!game || !game->cameraDialogue)
        return nullptr;
    VCDS_t *record = game->cameraDialogue->records;
    while (record && record->id != id)
        record = record->next;
    if (record && id) {
        record->state = 1;
        record->previousAnimation = 0;
    }
    return record;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::int64_t QueueBattleDialogue(std::int16_t priority, std::int16_t dialogue) {
    VFMA_t *const game = vfmaGetTaskParam();
    if (!game)
        return 0;
    VCDW_t *const owner = game->cameraDialogue;
    if (!owner || (owner->state && static_cast<std::uint16_t>(owner->flags) <= owner->parameters))
        return 0;
    if (dialogue == -1) {
        SetFightValue(game, 0x31a, std::int16_t{-1});
        scenCvsCtrlFree();
    } else {
        owner->selected = dialogue;
        owner->parameters = static_cast<std::uint16_t>(priority);
    }
    return 1;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
void BeginFightDialogueRetirement() {
    VFMA_t *const game = vfmaGetTaskParam();
    if (game && game->cameraDialogue)
        game->cameraDialogue->flags = (game->cameraDialogue->flags & 0xffffU) | 0x10000U;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint32_t GetFightTaskFlags() {
    VFMA_t *const game = vfmaGetTaskParam();
    return game ? game->mode : 0;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
VFMA_t *SetActiveFightModeFlag() {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    if (!task)
        return nullptr;
    VFMA_t *const game = static_cast<VFMA_t *>(HLib::GetTaskParameter(task));
    if (game->state == 3)
        game->mode |= 0x80U;
    return game;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

// @verified - p
std::uint64_t GetFightCharacterFlag(std::uint8_t mode, Character *character, float value) {
    if (mode != 2) {
        for (std::uint8_t i = 0; i < 16; ++i) {
            if (g_activeFightData[i].character != character)
                continue;
            if (mode == 0)
                return g_activeFightData[i].hp;
            if (mode == 1)
                return static_cast<std::uint32_t>(g_activeFightData[i].initialHp);
        }
        return 0xffffffffU;
    }
    VFMA_t *const game = vfmaGetTaskParam();
    if (!game)
        return 0;
    std::uint8_t slot = 0;
    while (slot < 6 && game->characters[slot] != character)
        ++slot;
    if (slot == 6)
        slot = 99;
    VFActor *const actor = game->actors[slot];
    const float minimum = 0.1f > value ? 0.1f : value;
    const float maximum = static_cast<float>(actor->initialHp);
    actor->hp = maximum < minimum ? maximum : minimum;
    return 1;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void blurInit() {
    blurModelFlag = blurModelPreviousFlag = g_savedFightBlurFlag = 0;
    accumulatedFightBlur = 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
void *AllocateVFHD(FightRuntimePrefix *game) {
    void *const owner = HLib::AllocateTaskData(0x4e8, SHEN_NAME(VFHD));
    HLib::AssignDynamicAlignmentForTaskData(owner);
    FightValue<FightRuntimePrefix *>(owner, 0) = game;
    FightValue<std::uint8_t>(owner, 0x438) = 0;
    return owner;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfFreeVFHDSpace(FightRuntimePrefix *game) {
    if (game->dialogue) {
        HLib::FreeHeapSpace(game->dialogue, SHEN_NAME(VFHD));
        game->dialogue = 0;
    }
}

std::int32_t vfResetInstructionMode(std::uint16_t motion) {
    HLib::Task *const task = HLib::FindNextTaskInQueueByIndexWithDepth(9, 1);
    if (!task)
        return 0;
    FightRuntimePrefix *const game = static_cast<FightRuntimePrefix *>(HLib::GetTaskParameter(task));
    if (!game)
        return 0;
    std::byte *owner = static_cast<std::byte *>(game->dialogue);
    if (!owner)
        owner = static_cast<std::byte *>(game->dialogue = AllocateVFHD(game));
    owner[0x438] = std::byte(0);
    reinterpret_cast<std::byte *>(game)[0x168] = std::byte(0);
    if (vfmDispatchControl(1, *reinterpret_cast<shen_name *>(owner + 0x46c), 0, -1) == 2)
        vfmDispatchControl(1, *reinterpret_cast<shen_name *>(owner + 0x46c), 1, 1);
    if (motion)
        vfPublishInstructionActorMotion(
            game->actors[game->playerSlot], *reinterpret_cast<Character **>(owner + 0x474), *reinterpret_cast<shen_name *>(owner + 0x46c), motion);
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

#define FIGHT_FIELD(type, offset) (*reinterpret_cast<type *>(reinterpret_cast<std::byte *>(game) + (offset)))
void vfmQueueFightStart(std::uint8_t startSlot, std::uint8_t maximumEnemies, float knockback, std::uint8_t type, std::int32_t guardEffectSeconds) {
    FightRuntimePrefix *game;
    if (type == 99) {
        game = vfmaGetTaskParam();
        if (game && game->type == 4) {
            SetPlayerControlEnabled(0);
            game->inputBuffer = BindPadControlEntry(static_cast<PadControlEntry *>(game->savedInput));
            game->stateFlags &= ~2U;
            game->timedBattleTime = 0;
        }
        return;
    }
    game = AllocateVFMA();
    if (!(g_pFightStage->battleFlags & 2U))
        g_fightUiActive = 1;
    g_fightActive = 1;
    game->retained00 = SHEN_NAME4('F', 'G', 'H', 'T');
    game->playerSlot = 0;
    reinterpret_cast<std::int8_t &>(game->opponentSlot) = -1;
    game->playerCharacter = 0;
    FIGHT_FIELD(std::uint32_t, 0xe8) = 0;
    game->state = 0;
    game->slideDragScale = 1;
    game->cameraResponse = 0.6f;
    game->gravityScale = 0.010888889f;
    game->guardEffectSeconds = guardEffectSeconds;
    game->type = type;
    game->cameraParameter = 13;
    game->spriteDepth = 255;
    if (type == 10) {
        game->maximumActiveEnemies = 99;
        game->strikesLeft = maximumEnemies;
    } else {
        if (maximumEnemies > g_pFightStage->enemyCount)
            maximumEnemies = g_pFightStage->enemyCount;
        game->maximumActiveEnemies = maximumEnemies;
    }
    if (type == 9) {
        game->contactRange = 200;
        game->dialogue = AllocateVFHD(game);
    } else {
        game->contactRange = 4;
        game->dialogue = 0;
    }
    game->playerCameraScale = static_cast<float>(static_cast<std::int16_t>(g_nativePlayerStats.reactionCameraScale)) / 100.0f;
    if (game->type == 5)
        game->playerAssistScale = 0.4f;
    else
        game->playerAssistScale = static_cast<float>(static_cast<std::int16_t>(g_nativePlayerStats.assistScale)) / 100.0f;
    float amount = knockback;
    if (0.2f > amount)
        amount = 0.2f;
    else if (amount > 1.5f)
        amount = 1.5f;
    game->knockbackAmount = amount;
    game->dragScale = 1;
    game->controlFlags = 0;
    game->stateFlags = 0;
    game->frame = 0;
    game->cameraSet = 0;
    FIGHT_FIELD(std::uint32_t, 0x1e4) = 0;
    FIGHT_FIELD(std::uint32_t, 0x1e0) = 0;
    std::uint8_t index;
    for (index = 0; index < g_pFightStage->spawnPointCount; ++index) {
        FightSpawnPoint *source = &g_pFightStage->spawnPoints[index], *target = &game->spawnPoints[index];
        target->x = source->x;
        target->z = source->z;
        target->parameter = source->parameter;
    }
    game->spawnPointCount = g_pFightStage->spawnPointCount;
    for (index = 0; index < 6; ++index)
        game->characters[index] = 0;
    game->hpTimer = g_pFightStage->accumulatedHp * 13U + 0x3138U;
    if (type == 7)
        game->hpTimer = 0x1518;
    if (g_pFightStage->battleFlags & 6U)
        game->stateFlags |= 0x4000U;
    game->timedBattleTime = 0;
    game->elapsedTicks = 0;
    game->controlFlags = 0;
    FIGHT_FIELD(std::uint16_t, 0x444) = 0;
    FIGHT_FIELD(std::uint16_t, 0x232) = 0;
    FIGHT_FIELD(std::uint16_t, 0x43e) = 0;
    game->retained44 = 0;
    FIGHT_FIELD(std::uint16_t, 0x442) = 0;
    FIGHT_FIELD(std::uint16_t, 0x446) = 0;
    FIGHT_FIELD(std::uint16_t, 0x46) = 0;
    game->authCameraFrame = 0;
    if (game->guardEffectSeconds == -1)
        game->elapsedFrames = 0xffffffffU;
    else
        game->elapsedFrames = game->guardEffectSeconds * 30;
    game->remainingEnemies = g_pFightStage->activeFightersByTeam[1] + g_pFightStage->readyEnemySlots[1] + g_pFightStage->secondaryGroupCounters[1];
    vfSetupFightUiAndCameraState(game);
    vfInitSfxSlotState(game);
    vfFightNullsub();
    for (index = 0; index < g_pFightStage->enemyCount; ++index) {
        FCMG_t *slot = &g_pFightStage->enemySlots[index];
        if (slot->character) {
            slot->fighter = AllocateFGTM();
            slot->fighter->slot = AllocateFENE();
            vfEnemySlotResetAndRestartCycle(game, slot);
            AllocateVFGW(slot->fighter);
            FightValue<std::uint8_t>(slot, 0xcc) = 0;
        }
    }
    if (!startSlot)
        startSlot = 1;
    if (startSlot > 4)
        startSlot = 4;
    std::uint8_t admitted = 0;
    g_pFightStage->unknownGroupCounts[0] = g_pFightStage->unknownGroupCounts[1] = 0;
    for (index = 0; index < g_pFightStage->groupCount; ++index) {
        FRMG_t *group = &g_pFightStage->groups[index];
        if (group->character && reinterpret_cast<std::uintptr_t>(group->character) != FIGHT_DEAD && !group->blocker) {
            if (vfRegisterContactSlot(game, group->slot) != 99) {
                if (!admitted) {
                    Vector3 position;
                    GetCharacterWorldPosition(group->character, &position);
                    position.y += 1;
                    game->floorIntersectionY = flddFindIntersectionBy(position);
                    game->floorSnapY = game->floorIntersectionY - 0.5f;
                    if (g_currentMap == SHEN_NAME4('Q', '1', '0', '0')) {
                        game->floorIntersectionY += 0.5f;
                        game->floorSnapY += 0.5f;
                    }
                    if (g_currentMap == SHEN_NAME4('Q', 'L', 'B', 'T'))
                        game->floorSnapY = 111;
                }
                ++g_pFightStage->unknownGroupCounts[group->slot->counterGroup];
                ++admitted;
            }
        }
        if (admitted == startSlot)
            break;
    }
    vfInitializeFightDialogue(game);
    FIGHT_FIELD(std::uint8_t, 0x236) = 0;
    for (index = 0; index < 128; ++index) {
        std::uint16_t *first = reinterpret_cast<std::uint16_t *>(game->retained23e);
        first[index] = 0;
        std::uint16_t *second = reinterpret_cast<std::uint16_t *>(game->retained23e + 0x100);
        second[index] = 0;
    }
    game->outcome = 0;
    game->battleCounter = 0;
    FIGHT_FIELD(std::uint8_t, 0x1dd) = 0;
    game->retained23b = 0;
    g_fightCombatCounterSnapshot = 0;
    if (game->elapsedFrames == 0xffffffffU)
        g_pFightStage->battleFlags &= ~1U;
    if (g_pFightStage->battleFlags & 1U) {
        g_hasColouredText = 1;
        game->blurAmount = game->elapsedFrames;
    }
    game->collisionTask = Queue_vfc_VF_RobCollision(game);
    game->gameInfoTask = vfg_Callback_GameInfoEntry_Enqueue(game);
    vfUpdatePairingsAndDistances(game);
    vfBeginScriptedFormationMotion(game);
    InitializeFightCameraDialogue(game);
    game->playerCharacter = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo;
    for (index = 0; index < 6; ++index) {
        if (game->characters[index]) {
            vfInitializeFightActor(game, index);
            CharacterHandComponent *hand = AllocateHNDR(game->characters[index]);
            if (hand)
                SetCharacterHandRegionEnabled(hand, 0, 0, 1);
            hand = AllocateHNDL(game->characters[index]);
            if (hand)
                SetCharacterHandRegionEnabled(hand, 0, 0, 1);
        }
    }
    vfReleaseUnheldScriptedFormation(game);
    if (g_pFightStage->battleFlags & 8U)
        game->stateFlags |= 1U;
    if (DoesScreenFilmColourExist(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->filmTask))
        game->stateFlags |= 0x40000U;
    vfSetupFightInput(game);
    if (!blurModelFlag)
        blurModelPreviousFlag = blurModelFlag;
    ++blurModelFlag;
}
#undef FIGHT_FIELD

#else

// Native FGST/FCMG producer and suspended-free-battle resume.
// @verified - p
std::intptr_t vfmQueueFightStart(std::uint8_t startSlot, std::uint8_t maximumEnemies, float knockback, std::uint8_t type, std::int32_t guardEffectSeconds) {
    savedPostFxBank = GetPostFxBankValue(3);
    if (!(g_pFightStage->battleFlags & 2)) {
        SelectPostFxBank(3, 1);
        UploadBlurStrength(.6f);
    }
    if (type == 99) {
        auto *game = vfmaGetTaskParam();
        if (game && game->type == 4) {
            SetPlayerControlEnabled(0);
            game->inputBuffer = BindPadControlEntry(game->savedInput);
            game->mode &= ~2U;
            game->timedBattleTime = 0;
            return reinterpret_cast<std::intptr_t>(game->inputBuffer);
        }
        return reinterpret_cast<std::intptr_t>(game);
    }
    ResetStreetFighterHUD();
    auto *task = HLib::EnqueueTaskWithParameter(vfm_Callback_FightTaskEntry_Callback, 8, 9, sizeof(VFMA_t), SHEN_NAME(VFMA));
    auto *game = static_cast<VFMA_t *>(HLib::GetTaskParameter(task));
    game->task = task;
    g_fightActive = 1;
    if (!(g_pFightStage->battleFlags & 2))
        g_fightUiActive = 1;
    game->name = SHEN_NAME(FGHT);
    game->playerSlot = 0;
    game->opponentSlot = 0xff;
    game->playerCharacter = nullptr;
    std::fill(std::begin(game->participantParameters), std::end(game->participantParameters), 0);
    game->state = 0;
    game->slideDragScale = 1;
    game->cameraResponse = .6f;
    game->gravityScale = .010888889f;
    game->guardEffectSeconds = guardEffectSeconds;
    game->type = type;
    game->cameraParameter = 13;
    game->spriteDepth = 0xff;
    game->maximumActiveEnemies = type == 10 ? 99 : std::min(maximumEnemies, g_pFightStage->enemyCount);
    if (type == 10)
        game->strikesLeft = maximumEnemies;
    game->contactRange = type == 9 ? 200.0f : 4.0f;
    game->dialogue = type == 9 ? AllocateVFHD(game) : nullptr;
    const auto &stats = S2Save_GetPersistentPlayerStats();
    game->playerCameraScale = float(stats.reactionCameraScale) / 100;
    game->playerAssistScale = float(stats.assistScale) / 100;
    game->knockbackAmount = knockback < .2f ? .2f : std::fmin(1.5f, knockback);
    game->dragScale = 1;
    game->gameFlags = game->mode = game->reserved2c = 0;
    game->cameraSet = 0;
    game->parameter2c0 = game->parameter2c8 = 0;
    game->spawnPointCount = static_cast<std::int8_t>(g_pFightStage->spawnPointCount);
    for (std::uint8_t i = 0; i < g_pFightStage->spawnPointCount; ++i)
        std::memcpy(reinterpret_cast<std::byte *>(game) + 0x2f0 + 12 * i, &g_pFightStage->spawnPoints[i], sizeof(FightSpawnPoint));
    std::fill(std::begin(game->characters), std::end(game->characters), nullptr);
    game->hpTimer = std::clamp(float(17 * g_pFightStage->accumulatedHp + 4040), 5400.0f, 18000.0f);
    if (type == 7)
        game->hpTimer = 5400;
    if (g_pFightStage->battleFlags & 6)
        game->mode |= 0x4000;
    game->previousFrame = game->frame = game->timedBattleTime = game->frame3 = game->cameraTimer = 0;
    game->timer320 = game->parameter530 = game->targetTime = game->timeParameter = game->parameter570 = 0;
    game->hitStopFrames = 0;
    game->guardTimer = guardEffectSeconds == -1 ? 4294967300.0f : float(std::bit_cast<std::int32_t>(std::uint32_t(guardEffectSeconds) * 30U));
    game->remainingEnemies = g_pFightStage->readyEnemySlots[1] + g_pFightStage->activeFightersByTeam[1] + g_pFightStage->secondaryGroupCounters[1];
    vfSetupFightUiAndCameraState(game);
    vfInitSfxSlotState(game);
    vfFightNullsub();
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        auto &slot = g_pFightStage->enemySlots[i];
        if (slot.character)
            slot.position = GetCharacterWorldPosition(slot.character);
    }
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        auto &slot = g_pFightStage->enemySlots[i];
        if (!slot.character)
            continue;
        slot.fighter = AllocateFGTM();
        slot.fighter->slot = AllocateFENE();
        vfEnemySlotResetAndRestartCycle(game, &slot);
        AllocateVFGW(slot.fighter);
        slot.setPosition = 0;
    }
    g_pFightStage->unknownGroupCounts[0] = g_pFightStage->unknownGroupCounts[1] = 0;
    const auto limit = std::min<std::uint8_t>(startSlot ? startSlot : 1, 4);
    std::uint8_t admitted = 0;
    for (std::uint8_t i = 0; i < g_pFightStage->groupCount; ++i) {
        auto &group = g_pFightStage->groups[i];
        if (!group.character || reinterpret_cast<std::uintptr_t>(group.character) == FIGHT_DEAD.name || group.blocker)
            continue;
        if (vfRegisterContactSlot(game, group.slot) == 99)
            continue;
        if (!admitted) {
            auto position = GetCharacterWorldPosition(group.character);
            position.y += 1;
            game->floorIntersectionY = flddFindIntersectionBy(position);
            game->floorSnapY = game->floorIntersectionY - .5f;
            if (g_currentMap.name == SHEN_NAME(Q100)) {
                game->floorIntersectionY += .5f;
                game->floorSnapY += .5f;
            }
            if (g_currentMap.name == SHEN_NAME(QLBT))
                game->floorSnapY = 111;
        }
        ++g_pFightStage->unknownGroupCounts[group.slot->counterGroup];
        if (++admitted == limit)
            break;
    }
    vfInitializeFightDialogue(game);
    SetFightValue(game, 0x328, std::uint8_t{0});
    std::memset(game->collisionHistory, 0, sizeof(game->collisionHistory));
    game->counter32c = 0;
    game->parameter2b9 = 0;
    g_fightCombatCounterSnapshot = 0;
    if (FightTruncate(game->guardTimer) == -1)
        g_pFightStage->battleFlags &= ~1U;
    if (g_pFightStage->battleFlags & 1) {
        g_hasColouredText = 1;
        game->blurAmount = game->guardTimer;
    }
    game->collisionTask = Queue_vfc_VF_RobCollision(game);
    game->gameInfoTask = vfg_Callback_GameInfoEntry_Enqueue(game);
    vfUpdatePairingsAndDistances(game);
    vfBeginScriptedFormationMotion(game);
    AllocateVCDW(game);
    auto *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    game->playerCharacter = static_cast<Character *>(control->ryo);
    for (std::uint8_t i = 0; i < 6; ++i) {
        if (!game->characters[i])
            continue;
        vfInitializeFightActor(game, i);
        vfResetActorActionState(game->actors[i]);
        vfInitializeActorCollision(game, i);
        if (game->playerCharacter && game->characters[i] == game->playerCharacter)
            game->playerSlot = static_cast<std::int8_t>(i);
        SetCharacterHandRegionEnabled(game->characters[i], 0, false, 1);
        SetCharacterHandRegionEnabled(game->characters[i], 1, false, 1);
    }
    vfReleaseUnheldScriptedFormation(game);
    if (g_pFightStage->battleFlags & 8)
        game->mode |= 1;
    if (DoesScreenFilmColourExist(control->screenFilmTask))
        game->mode |= 0x40000;
    game->inputLockOnStart = plymIsSearchStartRequested();
    if (g_pFightStage->battleFlags & 2)
        game->inputBuffer = game->savedInput = nullptr;
    else {
        if (game->inputLockOnStart)
            SetPlayerControlEnabled(0);
        game->savedInput = AllocatePadControlEntry();
        game->inputBuffer = BindPadControlEntry(game->savedInput);
    }
    const auto previous = blurModelFlag ? blurModelPreviousFlag : std::uint8_t{0};
    ++blurModelFlag;
    blurModelPreviousFlag = previous;
    game->parameter574 = 0;
    return previous;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

void vfm_Callback_FightTaskEntry_Callback(void *parameter) {
    FightRuntimePrefix *const game = static_cast<FightRuntimePrefix *>(parameter);
    std::uint8_t index;
    FCMG_t *slot;
    for (index = 0; index < g_pFightStage->enemyCount; ++index) {
        slot = &g_pFightStage->enemySlots[index];
        if (slot->character)
            slot->retained21 &= 0xfeU;
    }
    UpdateFightCameraDialogue(game);
    if (camCtrlGetInterpMode() != 5 && !IsWazamakiInactive() && !IsFightStageFlag2Set()) {
        const std::int32_t current = camCtrlGetPerspective();
        std::int16_t difference = FightValue<std::int16_t>(game, 0x44c) - current;
        if (std::fabs(static_cast<float>(difference)) > 910.0f) {
            if (difference < 0)
                difference = -910;
            else
                difference = 910;
        }
        const std::int32_t adjusted = current + difference / 2;
        camCtrlSetPerspective(static_cast<std::int16_t>(adjusted));
    }
    for (index = 0; index < 6; ++index) {
        if (!game->characters[index])
            continue;
        FightActorRuntimePrefix *const actor = game->actors[index];
        actor->runtimeFlags &= 0xfffff0ffU;
        actor->hitResolutionFlags = 0;
        actor->stateFlags &= ~0x40000U;
    }
    game->frame = 0;
    MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (!DoesScreenFilmColourExist(control->filmTask))
        game->stateFlags &= ~0x40000U;
    if (static_cast<std::uint32_t>(game->state) > 3U) {
        game->stateFlags |= 1U;
        for (index = 0; index < 6; ++index)
            if (game->characters[index])
                vfResetReactionProgram(game->actors[index]);
    }
    typedef void (*FightActorPass)(FightActorRuntimePrefix *);
    static FightActorPass passes[8] = {vfUpdateWorldTransformAndFacingAngle, vfUpdateMain, vfmaUpdateNextReaction, vfUpdateVFHDState, vfResolvePairInteraction,
        vfUpdatePoseWithSfx, vfApplyYawRecursive, 0};
    for (std::uint8_t pass = 0; passes[pass]; ++pass)
        for (index = 0; index < 6; ++index)
            if (game->characters[index])
                passes[pass](game->actors[index]);
    UpdateFightTimers(game);
    HLib::GetTaskParameter(g_mapControlTask);
    switch (game->state) {
    case 0:
        game->state = 1;
        if ((g_inputBuffer[3].held & 0xc0U) == 0xc0U) {
            game->controlFlags |= 0x400000U;
            game->stateFlags |= 0x200U;
        }
        if ((g_inputBuffer[0].held & 0x606U) == 0x606U) {
            game->stateFlags |= 0x80000000U;
            if (static_cast<std::uint32_t>(static_cast<std::int8_t>(g_inputBuffer[0].leftStickY)) <= 63U)
                camCtrlSetInterpModeTo(3);
        }
    case 1:
        game->state = 2;
    case 2:
        for (index = 0; index < 6; ++index)
            if (game->characters[index])
                vfUpdateGuardLevel(game, index);
        game->state = 3;
    case 3:
        vfUpdateRenderOpacityForAll(game);
        UpdateDefeatAndRespawn(game);
        if (!(game->actors[game->playerSlot]->formationFlags & 0x40000U) && !(g_pFightStage->battleFlags & 0x1000U))
            vfUpdateEnemySlotFormationAndYaw(game);
        vfUpdateEnemySlotSpawnFade(game);
        UpdateEnemySlotActivation(game);
        HLib::ExecuteCallbackNow(SetupFightReflectorBoard, game);
        if ((!g_pFightStage->activeFightersByTeam[0] && !g_pFightStage->secondaryGroupCounters[0]) ||
            (!g_pFightStage->activeFightersByTeam[1] && !g_pFightStage->secondaryGroupCounters[1])) {
            for (index = 0; index < g_pFightStage->groupCount; ++index) {
                FRMG_t &group = g_pFightStage->groups[index];
                if (group.character && group.state == 1) {
                    group.state = 3;
                    g_activeFightData[group.slot->fightEntryIndex].status = 3;
                }
            }
            if (g_pFightStage->battleFlags & 0x100U)
                game->state = 9;
            else
                game->state = 4;
        } else if (!game->elapsedFrames) {
            std::uint8_t status[2];
            if (!g_pFightStage->unknownGroupCounts[1]) {
                if (!g_pFightStage->unknownGroupCounts[0]) {
                    status[0] = 4;
                    status[1] = 4;
                } else {
                    status[0] = 3;
                    status[1] = 2;
                }
            } else if (!g_pFightStage->unknownGroupCounts[0]) {
                status[0] = 2;
                status[1] = 3;
            } else if (g_pFightStage->activeFightersByTeam[0] > g_pFightStage->activeFightersByTeam[1]) {
                status[0] = 6;
                status[1] = 5;
            } else if (g_pFightStage->activeFightersByTeam[0] < g_pFightStage->activeFightersByTeam[1]) {
                status[0] = 5;
                status[1] = 6;
            } else
                status[0] = status[1] = 7;
            for (index = 0; index < g_pFightStage->groupCount; ++index) {
                FRMG_t &group = g_pFightStage->groups[index];
                if (group.character && group.state == 1) {
                    group.state = status[group.team];
                    g_activeFightData[group.slot->fightEntryIndex].status = status[group.team];
                }
            }
            if (g_pFightStage->battleFlags & 0x100U)
                game->state = 9;
            else
                game->state = 4;
        } else {
            vfUpdatePairingsAndDistances(game);
            if (!(g_pFightStage->battleFlags & 2U) && !game->cameraSet && game->type != 9) {
                InitializeCameraGoal();
                camCtrlSetFovTransitionTo(0);
                camCtrlSetState(10);
                game->cameraSet = 1;
            }
        }
        UpdateFormationRuntimeFlags(game);
        for (index = 0; index < g_pFightStage->enemyCount; ++index) {
            slot = &g_pFightStage->enemySlots[index];
            if (!slot->character || slot->state != 3)
                continue;
            slot->collisionFlags &= ~0x10U;
            if (slot->collisionFlags & 8U)
                slot->collisionFlags |= 0x10U;
            slot->collisionFlags &= ~8U;
        }
        if (g_hasColouredText)
            ConstructColouredText(game->blurAmount, 188, 49, 1.0f);
        adjustStreetFighterHUD();
        constructNumStrikesLeft();
        for (index = 0; index < g_pFightStage->enemyCount; ++index) {
            slot = &g_pFightStage->enemySlots[index];
            if (!slot->character)
                continue;
            FightActorRuntimePrefix *const actor = slot->fighter;
            float percentage;
            switch (slot->state) {
            case 3:
            case 4:
                percentage = 0;
                break;
            case 2:
                percentage = 0;
                if (actor->initialHp) {
                    percentage = actor->hp / static_cast<float>(actor->initialHp);
                    if (std::fabs(percentage) < 0.00000011920929f)
                        percentage = 0;
                }
                break;
            default:
                percentage = 1;
                break;
            }
            g_activeFightData[index].hp = static_cast<std::int32_t>(actor->hp + 0.5f);
            g_activeFightData[index].initialHp = static_cast<std::int32_t>(percentage * 100);
        }
        HLib::ExecuteCallbackNow(UpdateFightGauge, game);
        if (game->elapsedFrames != 0xffffffffU && !(game->controlFlags & 0x800000U) && !(game->stateFlags & 0x80001U) && game->blurAmount)
            --game->blurAmount;
        break;
    case 4:
        SetLoadingBusy();
        if (FindBattleStatusEntry(game->playerCharacter) == 3) {
            FightActorRuntimePrefix *const actor = game->actors[game->playerSlot];
            if (game->playerSlot != -1 && !(actor->hitStateFlags & 0x81d010U) && *reinterpret_cast<std::uint16_t *>(actor->retained29c) != 4)
                *reinterpret_cast<std::uint32_t *>(actor->retained2f8 + 0x20) = 0x24000000U;
        }
        postBattleWaitFrames = 0;
        HLib::ExecuteCallbackNow(SetupFightReflectorBoard, game);
        HLib::ExecuteCallbackNow(UpdateFightGauge, game);
        if (g_hasColouredText)
            ConstructColouredText(game->blurAmount, 188, 49, 1.0f);
        adjustStreetFighterHUD();
        constructNumStrikesLeft();
        game->state = 5;
        break;
    case 5:
        HLib::ExecuteCallbackNow(SetupFightReflectorBoard, game);
        HLib::ExecuteCallbackNow(UpdateFightGauge, game);
        if (g_hasColouredText)
            ConstructColouredText(game->blurAmount, 188, 49, 1.0f);
        adjustStreetFighterHUD();
        constructNumStrikesLeft();
        game->state = 6;
        break;
    case 6:
        SetLoadingBusy();
        game->state = 7;
        postBattleWaitFrames = 0;
        HLib::ExecuteCallbackNow(SetupFightReflectorBoard, game);
        HLib::ExecuteCallbackNow(UpdateFightGauge, game);
        if (g_hasColouredText)
            ConstructColouredText(game->blurAmount, 188, 49, 1.0f);
        adjustStreetFighterHUD();
        constructNumStrikesLeft();
        break;
    case 7: {
        if (g_hasColouredText)
            ConstructColouredText(game->blurAmount, 188, 49, 1.0f);
        adjustStreetFighterHUD();
        constructNumStrikesLeft();
        std::uint8_t settled = 0;
        for (index = 0; index < 6; ++index) {
            if (!game->characters[index]) {
                ++settled;
                continue;
            }
            FightActorRuntimePrefix *const actor = game->actors[index];
            MotionWork *const work = charFindMotionwork(game->characters[index]);
            if (mtwkSetExtraFlag(work, 3, 1))
                continue;
            if (!(FightValue<std::uint32_t>(actor, 0x32c) & 0x40000U))
                ++settled;
            else {
                const float frame = mtwkGetFrame(work, static_cast<MotionRegion>(0));
                if (!(mtwkGetEndFrame(work, static_cast<MotionRegion>(0)) > frame))
                    ++settled;
            }
        }
        if (settled == 6 || ++postBattleWaitFrames > 90) {
            game->state = 8;
            postBattleWaitFrames = 60;
        }
        HLib::ExecuteCallbackNow(SetupFightReflectorBoard, game);
        HLib::ExecuteCallbackNow(UpdateFightGauge, game);
        break;
    }
    case 8:
        if (!--postBattleWaitFrames)
            game->state = 9;
        HLib::ExecuteCallbackNow(SetupFightReflectorBoard, game);
        HLib::ExecuteCallbackNow(UpdateFightGauge, game);
        break;
    case 9: {
        SetLoadingBusy();
        AccumulateFightCombatStats(game);
        ApplyMoveProficiencyUsage();
        if (game->type == 5)
            StartShowScreenOfId(7);
        game->state = 10;
        const std::int32_t status = FindBattleStatusEntry(game->playerCharacter);
        g_nativePlayerStats.actionState = blurModelFlag;
        switch (game->type) {
        case 2:
        case 4:
        case 5:
            blurModelFlag = 0;
            break;
        default:
            if (status != 2 && status != 5)
                blurModelFlag = 0;
            else
                ++blurModelPreviousFlag;
            break;
        }
        SetupWZ2_CtrlWazamaki();
        vfUnknownCamRelated(game);
    }
    case 10:
        if (game->type == 9)
            vfFreeVFHDSpace(game);
        FreeFightCameraDialogue(game);
        mtwkSetUseEnvFlag(1);
        g_hasColouredText = 0;
        vfReleaseFightInput(game);
        vfTransitionAllEnemySlotsToInactive(game);
        if (!g_pFightStage->smokeInitTask)
            FreeFightSmokeEffects();
        else
            HLib::DestroyTaskInternal(g_pFightStage->smokeInitTask);
        vfFreeStageObjects();
        FreeFightStyles();
        if (!g_is_vfm_active) {
            FreeBattleGaugeSprites();
            FreeLoadSprites();
        }
        for (index = 0; index < g_pFightStage->enemyCount; ++index) {
            FCMG_t *const slot = &g_pFightStage->enemySlots[index];
            if (!slot || !slot->character || !slot->fighter)
                continue;
            FreeVFGW(slot->fighter);
            figpAssignModeForCharacter(slot->character, 0);
            vfSetFightFaceExpression(slot->character, 0, 1);
        }
        vfRestorePostFightUIState(game);
        if (game->type == 5)
            ResetHelpIconsToItemPrompt();
        mtwkSetUseEnvFlag(1);
        g_fightUiActive = g_fightActive = 0;
        FinishLoading();
        HLib::DestroyTaskInternal(game->gameInfoTask);
        HLib::DestroyTaskInternal(game->collisionTask);
        HLib::DestroyCurrentEventTask();
        FreeFightContactSlots();
        FreeFightFormations();
        FreeFightAreaMaskNullsub();
        FreeFightGenerators();
        FreeFightStage();
        return;
    default:
        break;
    }
    ++game->elapsedTicks;
    if (!(game->stateFlags & 0x18U))
        ++game->timedBattleTime;
    vfUpdateAuthCameraFrame(game);
    g_fightCombatCounterSnapshot = static_cast<std::int8_t>(FightValue<std::uint8_t>(game, 0x23b));
    for (index = 0; index < g_pFightStage->enemyCount; ++index) {
        slot = &g_pFightStage->enemySlots[index];
        if (!slot->character)
            continue;
        if (slot->runningTicks < 65500)
            ++slot->runningTicks;
        if (slot->retained21 & 1U) {
            if (slot->fighter && slot->state == 2)
                mtwkSampleCurrentMotion(slot->motion, 0, reinterpret_cast<Vector3 *>(reinterpret_cast<std::byte *>(slot->fighter) + 0x4c));
            else {
                Vector3 discarded;
                mtwkSampleCurrentMotion(slot->motion, 0, &discarded);
            }
        }
    }
    if (!IsFightStageFlag2Set())
        UpdateFightCharaAlphas(game);
    vfmaSetupDialogueReactionsEtc(game);
}

#else

// Pass-major native actor scheduling and battle/result ownership.
// @verified - p
void vfm_Callback_FightTaskEntry_Callback(void *parameter) {
    using enum CameraState;
    auto *game = static_cast<VFMA_t *>(parameter);
    if (ReturnZero() != 0)
        return;
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i)
        if (g_pFightStage->enemySlots[i].character)
            g_pFightStage->enemySlots[i].flags &= ~1U;
    UpdateFightCameraDialogue(game);
    if (camCtrlGetInterpMode() != CameraMotionMode::MotionUpdate && !IsWazamakiInactive() && !(g_pFightStage->battleFlags & 2)) {
        const auto current = camCtrlGetPerspective();
        const auto delta = std::clamp<std::int16_t>(static_cast<std::int16_t>(game->perspective - current), -910, 910);
        camCtrlSetPerspective(static_cast<std::uint16_t>(current + delta / 2));
    }
    for (std::uint8_t i = 0; i < 6; ++i)
        if (game->characters[i]) {
            auto *actor = game->actors[i];
            actor->formationRuntimeFlags &= 0xfffff0ff;
            actor->stateFlags &= ~VFSF_Unknown40000;
            actor->hitResolutionFlags = 0;
        }
    game->reserved2c = 0;
    auto *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (!DoesScreenFilmColourExist(control->screenFilmTask))
        game->mode &= ~0x40000U;
    if (static_cast<std::uint32_t>(game->state) >= 4) {
        game->mode |= 1;
        for (std::uint8_t i = 0; i < 6; ++i)
            if (game->characters[i])
                vfResetReactionProgram(game->actors[i]);
    }
    constexpr void (*passes[])(VFActor *){vfUpdateWorldTransformAndFacingAngle, vfUpdateMain, vfmaUpdateNextReaction, vfUpdateVFHDState,
        vfResolvePairInteraction, vfUpdatePoseWithSfx, vfApplyYawRecursive};
    for (auto pass : passes)
        for (std::uint8_t i = 0; i < 6; ++i)
            if (game->characters[i])
                pass(game->actors[i]);
    UpdateFightTimers(game);
    switch (game->state) {
    case 0:
    case 1:
    case 2:
        for (std::uint8_t i = 0; i < 6; ++i)
            if (game->characters[i])
                vfUpdateGuardLevel(game, i);
        game->state = 3;
        [[fallthrough]];
    case 3: {
        vfUpdateRenderOpacityForAll(game);
        UpdateDefeatAndRespawn(game);
        if (!(game->actors[game->playerSlot]->formationFlags & 0x40000) && !(g_pFightStage->battleFlags & 0x1000))
            vfUpdateEnemySlotFormationAndYaw(game);
        vfUpdateEnemySlotSpawnFade(game);
        UpdateEnemySlotActivation(game);
        HLib::ExecuteCallbackNow(SetupFightReflectorBoard, game);
        auto *stage = g_pFightStage;
        const auto left = stage->activeFightersByTeam[0], right = stage->activeFightersByTeam[1];
        const bool finished = (!left && !stage->secondaryGroupCounters[0]) || (!right && !stage->secondaryGroupCounters[1]);
        if (finished || game->guardTimer <= 0) {
            std::uint16_t statuses = 0x0303;
            if (!finished) {
                if (stage->unknownGroupCounts[1]) {
                    if (stage->unknownGroupCounts[0])
                        statuses = left > right ? 0x0506 : left < right ? 0x0605 : 0x0707;
                    else
                        statuses = 0x0302;
                } else
                    statuses = stage->unknownGroupCounts[0] ? 0x0203 : 0x0404;
            }
            for (std::uint8_t i = 0; i < stage->groupCount; ++i) {
                auto &group = stage->groups[i];
                if (group.character && group.state == 1) {
                    group.state = static_cast<std::uint8_t>(statuses >> (8 * group.poolMode));
                    g_activeFightData[group.slot->fightEntryIndex].status = group.state;
                }
            }
            game->state = (stage->battleFlags & 0x100) ? 9 : 4;
        } else {
            vfUpdatePairingsAndDistances(game);
            if (!(stage->battleFlags & 2) && !game->cameraSet && game->type != 9) {
                resetCharaPosYandIntersection();
                camCtrlSetFovTransitionTo(false);
                camCtrlSetState(ChaseGoal);
                game->cameraSet = 1;
            }
        }
        UpdateFormationRuntimeFlags(game);
        for (std::uint8_t i = 0; i < stage->enemyCount; ++i) {
            auto &slot = stage->enemySlots[i];
            if (!slot.character || slot.state != 3)
                continue;
            slot.collisionFlags &= ~0x10U;
            if (slot.collisionFlags & 8)
                slot.collisionFlags |= 0x10;
            slot.collisionFlags &= ~8U;
        }
        DrawFightHud(game);
        for (std::uint8_t i = 0; i < stage->enemyCount; ++i) {
            const auto &slot = stage->enemySlots[i];
            if (!slot.character)
                continue;
            const auto *actor = slot.fighter;
            float percent = 0;
            if (slot.state == 2) {
                if (actor->initialHp) {
                    percent = actor->hp / float(actor->initialHp);
                    if (std::fabs(percent) < 0.00000011920929f)
                        percent = 0;
                }
            } else if (static_cast<std::uint8_t>(slot.state - 3) > 1)
                percent = 1;
            g_activeFightData[i].hp = static_cast<std::uint32_t>(actor->hp + .5f);
            g_activeFightData[i].initialHp = static_cast<std::int32_t>(percent * 100);
        }
        HLib::ExecuteCallbackNow(UpdateFightGauge, game);
        if (FightTruncate(game->guardTimer) != -1 && !(game->gameFlags & 0x800000) && !(game->mode & 0x80011) && game->blurAmount > 0) {
            game->blurAmount -= shenGetScaledDeltaTickTime(1);
            if (game->blurAmount < 0)
                game->blurAmount = 0;
        }
        break;
    }
    case 4:
        vfFightNullsub();
        for (const auto &entry : g_activeFightData)
            if (entry.character == game->playerCharacter) {
                if (entry.status == 3 && game->playerSlot != -1) {
                    auto *actor = game->actors[game->playerSlot];
                    if (!(actor->hitStateFlags &
                            (VFHSF_FallLandingRequired | VFHSF_LightReaction | VFHSF_SpecialGuardOrParry | VFHSF_SpecialThrow | VFHSF_GuardState |
                                VFHSF_AlignedToGround)) &&
                        actor->parameter2dc != 4)
                        actor->queuedAction = 0x24000000;
                }
                break;
            }
        postBattleWaitFrames = 0;
        PublishFightHud(game);
        game->state = 5;
        break;
    case 5:
        PublishFightHud(game);
        game->state = 6;
        break;
    case 6:
        vfFightNullsub();
        game->state = 7;
        postBattleWaitFrames = 0;
        PublishFightHud(game);
        break;
    case 7: {
        DrawFightHud(game);
        std::uint8_t settled = 0;
        for (std::uint8_t i = 0; i < 6; ++i) {
            if (!game->characters[i]) {
                ++settled;
                continue;
            }
            auto *actor = game->actors[i];
            auto *motion =
#if SHENMUE_IS_DC
charFindMotionwork
#else
FindMotionWork
#endif
(game->characters[i]);
            if (!mtwkSetExtraFlag(motion, 3, 1) &&
                (!(actor->hitStateFlags & VFHSF_MidAirRecovery) ||
                    mtwkGetEndFrame(motion, MotionRegion::LowerBody) <= mtwkGetFrame(motion, MotionRegion::LowerBody)))
                ++settled;
        }
        if (settled == 6 || ++postBattleWaitFrames > 90) {
            game->state = 8;
            postBattleWaitFrames = 60;
        }
        HLib::ExecuteCallbackNow(SetupFightReflectorBoard, game);
        HLib::ExecuteCallbackNow(UpdateFightGauge, game);
        break;
    }
    case 8:
        if (!--postBattleWaitFrames)
            game->state = 9;
        HLib::ExecuteCallbackNow(SetupFightReflectorBoard, game);
        HLib::ExecuteCallbackNow(UpdateFightGauge, game);
        break;
    case 9: {
        vfFightNullsub();
        AccumulateFightCombatStats(game);
        vfConsumeMoveTrainingGains();
        game->state = 10;
        std::int32_t status = -1;
        for (const auto &entry : g_activeFightData)
            if (entry.character == game->playerCharacter) {
                status = entry.status;
                break;
            }
        g_savedFightBlurFlag = blurModelFlag;
        if (((game->type - 2) & 0xfd) && (status == 2 || status == 5))
            ++blurModelPreviousFlag;
        else
            blurModelFlag = 0;
        SetupWZ2_CtrlWazamaki();
        SetNewEwzoTypeForMoveLearned(1, 0, 0);
        vfUnknownCamRelated(game);
        [[fallthrough]];
    }
    case 10:
        ReleaseFight(game);
        return;
    default:
        break;
    }
    AdvanceFightFrame(game);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void UpdateFightCharaAlphas(FightRuntimePrefix *game) {
    std::uint8_t index;
    FightActorRuntimePrefix *actor;
    if (!FightValue<std::uint16_t>(game, 0x446))
        return;
    std::int32_t remaining = FightValue<std::uint16_t>(game, 0x446) - 2;
    if (remaining > 0) {
        for (index = 0; index < 6; ++index) {
            if (game->characters[index]) {
                actor = game->actors[index];
                if (actor->moveFlags & 0x10000U)
                    SetOtherFightCharaAlphas(actor, 0.0f);
            }
        }
    } else {
        remaining = 0;
        camCtrlSetState(10);
        for (index = 0; index < 6; ++index) {
            if (!game->characters[index])
                continue;
            actor = game->actors[index];
            cpamSetFlag(game->characters[index], 1);
            if ((actor->motionParameters[0x19] & 1) == 0)
                actor->moveFlags &= ~0x40000U;
            if (actor->moveFlags & 0x10000U) {
                actor->moveFlags &= ~0x10000U;
                SetOtherFightCharaAlphas(actor, 1.0f);
            }
        }
        game->stateFlags &= 0xffffbffeU;
    }
    FightValue<std::uint16_t>(game, 0x446) = remaining;
}

#else

namespace {

// The remaining time is published only after the camera, CPAM and alpha writes.
// @verified - p
void UpdateFightCharaAlphas(VFMA_t *game) {
    using enum CameraState;
    if (!(game->targetTime > 0))
        return;
    float remaining = game->targetTime - shenGetScaledDeltaTickTime(2);
    if (remaining <= 0) {
        remaining = 0;
        camCtrlSetState(ChaseGoal);
        for (std::uint8_t i = 0; i < 6; ++i) {
            if (!game->characters[i])
                continue;
            auto &actor = *game->actors[i];
            cpamSetFlag(actor.character, 1);
            if (!(std::to_integer<std::uint8_t>(actor.motionParameters[2][25]) & 1))
                actor.moveFlags &= ~VFMF_NoContactPairing;
            if (actor.moveFlags & VFMF_Unknown10000) {
                actor.moveFlags &= ~VFMF_Unknown10000;
                SetOtherFightCharaAlphas(&actor, 1);
            }
        }
        game->mode &= ~0x4001U;
    } else {
        for (std::uint8_t i = 0; i < 6; ++i)
            if (game->characters[i] && (game->actors[i]->moveFlags & 0x10000))
                SetOtherFightCharaAlphas(game->actors[i], 0);
    }
    game->targetTime = remaining;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void AdvanceFightFrame(VFMA_t *game) {
    const float delta = shenGetScaledDeltaTickTime(1);
    game->previousFrame = game->frame;
    game->frame += delta;
    if (!(game->mode & 0x18))
        game->timedBattleTime += delta;
    if (game->mode & 0x80000) {
        if (static_cast<std::int32_t>(game->parameter570) < 65000)
            game->parameter570 += delta;
    } else
        game->parameter570 = 0;
    g_fightCombatCounterSnapshot = static_cast<std::int8_t>(game->counter32c >> 8);
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        auto &slot = g_pFightStage->enemySlots[i];
        if (!slot.character)
            continue;
        if (static_cast<std::int32_t>(slot.runningTicks) < 65500)
            slot.runningTicks += delta;
        if (slot.flags & 1) {
            Vector3 discarded;
            auto &output = slot.fighter && slot.state == 2 ? slot.fighter->moveDelta : discarded;
            mtwkSampleCurrentMotion(slot.motion, 0, output);
        }
    }
    if (!(g_pFightStage->battleFlags & 2))
        UpdateFightCharaAlphas(game);
    vfmaSetupDialogueReactionsEtc(game);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

// @verified - p
void FreeFightCameraDialogue(VFMA_t *game) {
    RetireCameraDialogueRecords();
    if (game->cameraDialogue)
        HLib::FreeHeapSpace(game->cameraDialogue, static_cast<std::int32_t>(TASK_VCDW.name));
    game->cameraDialogue = nullptr;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfUpdateAuthCameraFrame(FightRuntimePrefix *game) {
    if (game->stateFlags & 0x80000U) {
        if (game->authCameraFrame < 65000)
            ++game->authCameraFrame;
    } else {
        game->authCameraFrame = 0;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

inline void AddFightStatistic(std::size_t offset, std::uint32_t amount) {
    std::uint16_t &value = FightValue<std::uint16_t>(&g_combatStatistics, offset);
    const std::uint32_t total = value + amount;
    if (total < 65536U)
        value = total;
    else
        value = 65535;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void SetOtherFightCharaAlphas(FightActorRuntimePrefix *actor, float alpha) {
    FightRuntimePrefix *const game = actor->game;
    for (std::int32_t index = 0; index < 6; ++index)
        if (game->characters[index] && actor != game->actors[index])
            SetCharacterAlpha(game->characters[index], alpha);
}

bool SetupContactSlot(FightRuntimePrefix *game, FRMG_t *group);

std::int32_t vfAdmitCharacterFormation(Character *character) {
    FightRuntimePrefix *const game = vfmaGetTaskParam();
    if (!game)
        return 0;
    FCMG_t *slot = 0;
    FRMG_t *group = 0;
    for (std::uint8_t index = 0; index < g_pFightStage->groupCount; ++index) {
        group = &g_pFightStage->groups[index];
        if (group->character == character && !group->state) {
            slot = group->slot;
            break;
        }
    }
    FightValue<std::uint8_t>(slot, 0xcc) = 0;
    if (!slot->fighter) {
        slot->fighter = AllocateFGTM();
        slot->fighter->slot = AllocateFENE();
    }
    if (g_pFightStage->activeFightersByTeam[0] + g_pFightStage->activeFightersByTeam[1] == 6) {
        const std::uint8_t index = vfSelectOpponentSlotByPriority(game, static_cast<std::uint8_t>(group->selectFlags));
        if (index == 99) {
            vfEnemySlotResetAndRestartCycle(game, slot);
            return 0;
        }
        vfEnemySlotResetAndRestartCycle(game, game->enemySlots[index]);
    }
    if (game->state != 3)
        return 0;
    if (slot->state == 2)
        return 0;
    if (SetupContactSlot(game, group) == true)
        return 1;
    return 0;
}

#else

namespace {

// Native vfmaSetCharaAlphas: isolate the actor by fading every other live participant.
// @verified - p
void SetOtherFightCharaAlphas(VFActor *actor, float alpha) {
    for (std::uint8_t i = 0; i < 6; ++i)
        if (actor->game->characters[i] && actor->game->actors[i] != actor)
            modelSetCharaAlpha(actor->game->characters[i], alpha);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void FreeFightContactSlots() {
    for (std::uint8_t index = 0; index < g_pFightStage->enemyCount; ++index) {
        if (g_pFightStage->enemySlots[index].character) {
            FightActorRuntimePrefix *const fighter = g_pFightStage->enemySlots[index].fighter;
            FreeMarkedFightTaskData(fighter->slot);
            FreeFGTM(fighter);
        }
    }
    HLib::ClearTaskDataMark(g_pFightStage->enemySlots);
    HLib::FreeTaskData(g_pFightStage->enemySlots);
    g_pFightStage->enemySlots = 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void FreeFightStyles() {
    for (std::uint8_t index = 0; index < 8; ++index) {
        if (g_pFightStage->styles[index].file) {
            HLib::FreeTaskData(g_pFightStage->styles[index].file);
            g_pFightStage->styles[index].style = 0;
        }
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
FightRuntimePrefix *AllocateVFMA() {
    HLib::Task *const task = HLib::CreateNewCurrentTaskWithParameter(vfm_Callback_FightTaskEntry_Callback, 8, 9, sizeof(FightRuntimePrefix), SHEN_NAME(VFMA));
    FightRuntimePrefix *const game = static_cast<FightRuntimePrefix *>(HLib::GetTaskParameter(task));
    game->task = task;
    return game;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

void vfSetupFightUiAndCameraState(FightRuntimePrefix *game) {
    const std::uint8_t type = game->type;
    std::uint32_t saved = 0;
    if (plymGetSpecialMode() == 1) {
        plymSetSpecialMode(0);
        saved = 0x400U;
    }
    if (!(g_loadingFlag & 0x20U))
        saved |= 0x800U;
    FinishLoading();
    if (type == 4) {
        if (GetUiFlag(2) == 1) {
            saved |= GetUiFlag(0);
            saved |= GetUiFlag(1) << 16U;
            saved |= 0x200U;
            SetHelpIconMenuFlags(0x401, 1);
        } else
            AddHelpIconMenuFlags(0x401, 1);
    }
    const std::uint8_t icon = helpGetCurrentIconIdx(1);
    FightValue<std::uint8_t>(game, 0x237) = icon;
    if (!(g_pFightStage->battleFlags & 2U) && type != 2) {
        if (type == 9)
            EV_EventSetUpHelpIcon_0(14, 0);
        else if (!helpGetCurrentIconIdx(1))
            EV_EventSetUpHelpIcon_0(20, 0);
    }
    if (CheckTimeHudVisibility() == 1) {
        if (!(g_pFightStage->battleFlags & 2U))
            shenSetClockAndCompassRenderTo(1);
        saved |= 0x4000U;
    }
    switch (g_currentMap) {
    case SHEN_NAME4('Q', 'J', 'B', 'T'):
    case SHEN_NAME4('Q', 'K', 'B', 'T'):
    case SHEN_NAME4('Q', 'L', 'B', 'T'):
        game->cameraParameter = 13;
        break;
    }
    float height = 0, distance = 0;
    std::int16_t perspective = camCtrlGetPerspective();
    FightValue<std::int16_t>(game, 0x44e) = perspective;
    std::int32_t mode = 0;
    if (g_pFightStage->battleFlags & 0x80U)
        mode = 3;
    else if (type == 11)
        mode = 2;
    else if (!IsFightStageFlag2Set()) {
        switch (g_currentMap) {
        case SHEN_NAME4('Q', '1', '0', '0'):
        case SHEN_NAME4('Q', 'S', 'F', 'A'):
        case SHEN_NAME4('Q', 'S', 'F', 'B'):
        case SHEN_NAME4('B', 'T', 'S', 'T'):
            perspective = 0x0e38;
            mode = 1;
            distance = 8.08f;
            height = 1.7f;
            break;
        case SHEN_NAME4('Q', 'S', 'F', 'D'):
        case SHEN_NAME4('Q', 'T', 'B', '1'):
        case SHEN_NAME4('A', 'B', '0', '0'):
            perspective = 0x1c71;
            mode = 1;
            distance = 2.8f;
            height = 2.5f;
            break;
        case SHEN_NAME4('Q', 'S', 'F', 'C'):
        case SHEN_NAME4('Q', 'J', 'B', 'T'):
        case SHEN_NAME4('Q', 'K', 'B', 'T'):
        case SHEN_NAME4('Q', 'L', 'B', 'T'):
            perspective = 0x2aaa;
            mode = 2;
            distance = 2.8f;
            height = 2.5f;
            break;
        case SHEN_NAME4('Q', 'G', 'B', 'T'):
        case SHEN_NAME4('T', 'G', 'L', 'B'):
            perspective = 0x2aaa;
            mode = 2;
            distance = 8.08f;
            height = 1.7f;
            break;
        }
    }
    FightValue<std::int16_t>(game, 0x44c) = perspective;
    FightValue<float>(game, 0x454) = distance;
    FightValue<float>(game, 0x458) = height;
    camCtrlSetInterpModeTo(mode);
    camCtrlSetFightDistance(distance);
    camCtrlSetFightHeight(height);
    if (!IsFightStageFlag2Set()) {
        camCtrlSetPerspective(perspective);
        camCtrlSetFovTransitionTo(1);
        camCtrlSetState(10);
    }
    game->savedUiFlags = saved;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfRestorePostFightUIState(FightRuntimePrefix *game) {
    const std::uint32_t saved = game->savedUiFlags;
    if (saved & 0x400U)
        plymSetSpecialMode(1);
    if (!(saved & 0x800U))
        SetLoadingBusy();
    EV_EventSetUpHelpIcon_0(FightValue<std::uint8_t>(game, 0x237), 0);
    if (game->type == 4) {
        if (saved & 0x200U)
            SetHelpIconMenuFlags(static_cast<std::uint8_t>(saved), (saved >> 16) & 1U);
        else
            ResetHelpIconsToItemPrompt();
    }
    if (saved & 0x4000U)
        shenSetClockAndCompassRenderTo(0);
    if (game->type != 9 && !EV_vfBattleUtil(1, shen_name(2), -1) && camCtrlGetState() == 10) {
        camCtrlSetFovTransitionTo(1);
        camCtrlSetState(9);
        camCtrlSetState(0);
    }
    camCtrlSetPerspective(FightValue<std::int16_t>(game, 0x44e));
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

// @verified - p
void ReleaseFight(VFMA_t *game) {
    if (game->type == 9)
        vfFreeVFHDSpace(game);
    FreeFightCameraDialogue(game);
    mtwkSetEnvironmentEnabled(1);
    g_hasColouredText = 0;
    if (game->inputBuffer)
        BindPadControlEntry(game->inputBuffer);
    if (game->savedInput)
        ReleasePadControlEntry(game->savedInput);
    if (!(g_pFightStage->battleFlags & 2) && game->inputLockOnStart) {
        SetPlayerControlEnabled(1);
        game->inputLockOnStart = 0;
    }
    vfTransitionAllEnemySlotsToInactive(game);
    if (g_pFightStage->smokeInitTask)
        HLib::DestroyTask(g_pFightStage->smokeInitTask);
    else
        FreeFightSmokeEffects();
    vfFreeStageObjects();
    for (auto &style : g_pFightStage->styles) {
        if (!style.file)
            continue;
        HLib::FreeTaskData(style.file);
        style.style = 0;
    }
    if (!g_is_vfm_active) {
        FreeBattleGaugeSprites();
        FreeLoadSprites();
    }
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        auto &slot = g_pFightStage->enemySlots[i];
        if (!slot.character || !slot.fighter)
            continue;
        FreeVFGW(slot.fighter);
        figpAssignModeForCharacter(slot.character, 0);
        EV_figpAssignFlagsFor(slot.character, 0x40, 1);
        if (!(g_pFightStage->battleFlags & 0x80000))
            SetHighModelAndFaceFlagsForChara(slot.character, 1, 1, 1);
    }
    vfRestorePostFightUIState(game);
    mtwkSetEnvironmentEnabled(1);
    g_fightActive = g_fightUiActive = 0;
    HLib::DestroyTask(game->gameInfoTask);
    HLib::DestroyTask(game->collisionTask);
    HLib::DestroyCurrentTask();
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        auto &slot = g_pFightStage->enemySlots[i];
        if (!slot.character)
            continue;
        HLib::ClearTaskDataMark(slot.fighter->slot);
        HLib::FreeTaskData(slot.fighter->slot);
        HLib::ClearTaskDataMark(slot.fighter);
        HLib::FreeTaskData(slot.fighter);
    }
    HLib::ClearTaskDataMark(g_pFightStage->enemySlots);
    HLib::FreeTaskData(g_pFightStage->enemySlots);
    g_pFightStage->enemySlots = nullptr;
    HLib::ClearTaskDataMark(g_pFightStage->groups);
    HLib::FreeTaskData(g_pFightStage->groups);
    g_pFightStage->groups = nullptr;
    vfFightNullsub();
    HLib::ClearTaskDataMark(g_pFightStage->generators);
    HLib::FreeTaskData(g_pFightStage->generators);
    g_pFightStage->generators = nullptr;
    DestroyElementByIndex(UIElement::StreetFighterHud);
    HLib::ClearTaskDataMark(g_pFightStage);
    HLib::FreeTaskData(g_pFightStage);
    g_pFightStage = nullptr;
    SelectPostFxBank(3, savedPostFxBank);
    if (!savedPostFxBank)
        UploadBlurStrength(1);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void UpdateFightTimers(FightRuntimePrefix *game) {
    if (game->elapsedFrames != 0xffffffffU && !(game->controlFlags & 0x800000U) && (game->stateFlags & 1U) == 0)
        --game->elapsedFrames;
    vfDecrementSfxSlotTimers(game);
    UpdateFightSmokeEffects();
    for (std::uint8_t index = 0; index < 6; ++index)
        if (game->characters[index] && FightValue<std::uint16_t>(game->actors[index], 0x432) <= 64999)
            ++FightValue<std::uint16_t>(game->actors[index], 0x432);
    if (cvsHasValidConverseData(0) || FightValue<std::int16_t>(game, 0x22e) != -1)
        FightValue<std::uint16_t>(game, 0x232) = 240;
    else if (FightValue<std::uint16_t>(game, 0x232))
        --FightValue<std::uint16_t>(game, 0x232);
    if (game->hpTimer && !(game->controlFlags & 0x800000U))
        --game->hpTimer;
    if (FightValue<std::uint16_t>(game, 0x43e))
        --FightValue<std::uint16_t>(game, 0x43e);
    if (game->retained44)
        --game->retained44;
}

#else

namespace {

// @verified - p
void UpdateFightTimers(VFMA_t *game) {
    const float delta = shenGetScaledDeltaTickTime(1);
    const auto subtract = [delta](float &value) {
        value -= delta;
        if (value < 0)
            value = 0;
    };
    const auto decrement = [subtract](float &value) {
        if (value > 0)
            subtract(value);
    };
    // Only the -1 sentinel stops the guard timer; unlike the other timers it has no positive test.
    if (FightTruncate(game->guardTimer) != -1 && !(game->gameFlags & 0x800000) && !(game->mode & 0x80011))
        subtract(game->guardTimer);
    decrement(game->parameter294);
    decrement(game->parameter298);
    UpdateFightSmokeEffects();
    for (std::uint8_t i = 0; i < 6; ++i)
        if (game->characters[i] && static_cast<std::int32_t>(game->actors[i]->ticks) < 65000)
            game->actors[i]->ticks += delta;
    if (cvsHasValidConverseData() || FightValue<std::uint16_t>(game, 0x31a) != 0xffff)
        game->timer320 = 240;
    else
        decrement(game->timer320);
    if (!(game->gameFlags & 0x800000))
        decrement(game->hpTimer);
    decrement(game->parameter530);
    if (game->hitStopFrames)
        --game->hitStopFrames;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

// @verified - p
bool SetupContactSlot(VFMA_t *game, FRMG_t *group) {
    const auto index = vfRegisterContactSlot(game, group->slot);
    if (index == 99)
        return false;
    vfUpdatePairingsAndDistances(game);
    // Native vfResetActorReactionStateForContact publishes the player slot after the actor resets.
    vfInitializeFightActor(game, index);
    vfResetActorActionState(game->actors[index]);
    vfInitializeActorCollision(game, index);
    if (game->playerCharacter && game->characters[index] == game->playerCharacter)
        game->playerSlot = static_cast<std::int8_t>(index);
    vfUpdateGuardLevel(game, index);
    return true;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

// Native vfmaTaskData_ProcessTempFileMatch: unblock authored formations when their owning defeated slot retires.
// @verified - p
void ActivateBlockedFormations(VFMA_t *game, FCMG_t *retired) {
    if (g_pFightStage->activeFightersByTeam[0] + g_pFightStage->activeFightersByTeam[1] == 6)
        return;
    for (std::uint8_t i = 0; i < g_pFightStage->groupCount; ++i) {
        auto &group = g_pFightStage->groups[i];
        if (group.state || !group.character || reinterpret_cast<std::uintptr_t>(group.character) == FIGHT_DEAD.name || group.slot->state != 1 ||
            group.blocker != retired->character || group.poolMode != retired->counterGroup)
            continue;
        const auto other = static_cast<std::uint8_t>(group.poolMode ^ 1);
        if (g_pFightStage->activeFightersByTeam[other] || g_pFightStage->secondaryGroupCounters[other])
            SetupContactSlot(game, &group);
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

// @verified - p
void UpdateDefeatAndRespawn(VFMA_t *game) {
    for (std::uint8_t i = 0; i < 6; ++i) {
        if (!game->characters[i])
            continue;
        auto *const actor = game->actors[i];
        if ((actor->moveFlags & (VFMF_Airborne | VFMF_ForceDefeat)) == 0x20 && mtwkSetRegionFlag(actor->motion, 65, 1, MotionRegion::LowerBody)) {
            auto *const slot = game->enemySlots[i];
            vfTransitionEnemySlotToCooldown(game, slot);
            if (static_cast<std::uint8_t>(g_pFightStage->groupCounters[0] + g_pFightStage->groupCounters[1]) > game->maximumActiveEnemies)
                vfUpdateEnemySlotCooldowns(game);
            ActivateBlockedFormations(game, slot);
        }
        if (std::fabs(actor->hp) < 0.00000011920929f && !(actor->moveFlags & VFMF_ForceDefeat)) {
            actor->moveFlags |= VFMF_ForceDefeat;
            if (g_pFightStage->battleFlags & 0x40000) {
                const std::uint8_t player = actor->groupIndex == static_cast<std::uint8_t>(game->playerSlot);
                vfAddRoundResults(player ^ 1, player);
            }
        }
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

// @verified - p
void UpdateEnemySlotActivation(VFMA_t *game) {
    auto active = static_cast<std::uint8_t>(g_pFightStage->activeFightersByTeam[0] + g_pFightStage->activeFightersByTeam[1]);
    while (active > 4) {
        const auto index = vfSelectOpponentSlotByPriority(game, 0);
        if (index == 99)
            break;
        vfEnemySlotResetAndRestartCycle(game, game->enemySlots[index]);
        --active;
    }
    std::uint8_t counts[2]{}, normalCounts[2]{};
    for (std::uint8_t i = 0; i < 6; ++i) {
        if (!game->characters[i] || (game->actors[i]->moveFlags & 0x20))
            continue;
        ++counts[game->slotTeams[i]];
        if (!(game->actors[i]->activeMotionFlags & 0x800))
            ++normalCounts[game->slotTeams[i]];
    }
    for (std::uint8_t i = 0; i < g_pFightStage->groupCount; ++i) {
        auto &group = g_pFightStage->groups[i];
        const auto other = static_cast<std::uint8_t>(group.poolMode ^ 1);
        if ((!g_pFightStage->activeFightersByTeam[other] && !g_pFightStage->secondaryGroupCounters[other]) || group.state || !group.character ||
            reinterpret_cast<std::uintptr_t>(group.character) == FIGHT_DEAD.name || group.blocker || group.slot->state != 1 || (group.activationState & 0xff))
            continue;
        bool forced = false, replace = false;
        if (((group.waveFlags & 1) || (game->type == 1 && !(group.selectFlags & 0xa0))) && active < 4)
            forced = true;
        else if (game->playerSlot != -1) {
            const float radius = (group.waveFlags & 8) ? 10 : (group.waveFlags & 4) ? 7 : (group.waveFlags & 2) ? 5 : 2.5f;
            const auto position = GetCharacterWorldPosition(group.character);
            const auto &player = game->actors[game->playerSlot]->position;
            const float x = position.x - player.x, z = position.z - player.z;
            const float distance = std::sqrt(x * x + z * z);
            if (distance < radius) {
                forced = true;
                replace = distance < 2.5f;
            }
        }
        if (!forced && (!(group.waveFlags & 0x800) || normalCounts[group.poolMode]))
            continue;
        const auto total = static_cast<std::uint8_t>(g_pFightStage->activeFightersByTeam[0] + g_pFightStage->activeFightersByTeam[1]);
        bool admit = total < 4;
        if (!admit && replace) {
            const auto index = vfSelectOpponentSlotByPriority(game, static_cast<std::uint8_t>(group.selectFlags));
            if (index != 99) {
                vfEnemySlotResetAndRestartCycle(game, game->enemySlots[index]);
                admit = true;
            } else
                admit = total != 6;
        }
        if (admit && SetupContactSlot(game, &group))
            ++counts[group.poolMode];
    }
    g_pFightStage->unknownGroupCounts[0] = counts[0];
    g_pFightStage->unknownGroupCounts[1] = counts[1];
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfUpdatePairingsAndDistances(FightRuntimePrefix *game) {
    std::int8_t targets[6];
    float distances[6][6];
    std::int16_t angles[6][6];
    std::uint8_t index;
    std::uint8_t team;
    std::int8_t lastSelected;
    std::int8_t player;
    std::int8_t opponent;
    std::int8_t playerPaired;
    std::int16_t angle;
    std::uint8_t target;
    FightActorRuntimePrefix *other;
    FightActorRuntimePrefix *actor;
    float dx, dz;
    float nearest, distance, fromPlayer;
    bool facingFound;
    std::int16_t yaw;
    std::uint8_t mode;
    std::int8_t nearestEnemy, nearestAlly, playerEnemy;
    float enemyDistance, allyDistance;

    player = game->playerSlot;
    opponent = -1;
    for (index = 0; index < 6; ++index) {
        targets[index] = -1;
        for (target = 0; target < 6; ++target)
            distances[index][target] = 9999;
    }
    for (index = 0; index < 6; ++index) {
        if (!game->characters[index])
            continue;
        actor = game->actors[index];
        team = game->slotTeams[index];
        angle = actor->temporaryFacingAngle;
        Vector2 position;
        position.x = actor->position.x;
        position.y = actor->position.z;
        if (team == 1 && !(game->formations[index]->selectFlags & 0xa0U))
            opponent = index;
        for (target = 0; target < 6; ++target) {
            if (target == index || !game->characters[target] || game->slotTeams[target] == team)
                continue;
            other = game->actors[target];
            Vector2 otherPosition;
            otherPosition.x = other->position.x;
            otherPosition.y = other->position.z;
            dx = position.x - otherPosition.x;
            dz = position.y - otherPosition.y;
            const float length = std::sqrt(dx * dx + dz * dz);
            distances[index][target] = distances[target][index] = length;
            angles[index][target] = angle - dc::sdk::ArcTan2Fixed(dx, dz);
            angles[target][index] = other->temporaryFacingAngle - dc::sdk::ArcTan2Fixed(otherPosition.x - position.x, otherPosition.y - position.y);
        }
    }
    game->opponentSlot = opponent;
    lastSelected = -1;
    for (index = 0; index < 6; ++index) {
        if (!game->characters[index])
            continue;
        actor = game->actors[index];
        team = game->slotTeams[index];
        lastSelected = -1;
        nearest = 9999;
        facingFound = false;
        if (index == player)
            yaw = static_cast<std::int16_t>(*reinterpret_cast<std::int16_t *>(game->retained23e + 0x202) - actor->facingYaw);
        else
            yaw = 0;
        for (target = 0; target < 6; ++target) {
            if (!game->characters[target] || game->slotTeams[target] == team)
                continue;
            other = game->actors[target];
            distance = distances[index][target];
            if (other->moveFlags & 0x40000U)
                distance = 999;
            else if (yaw && (other->moveFlags & 0x20U)) {
                other->moveFlags |= 0x40000U;
                distance = 999;
            }
            if (static_cast<std::uint16_t>(angles[index][target] + yaw + 9100) < 0x4719U && distance < 3.0f) {
                if (!facingFound || distance < nearest) {
                    lastSelected = target;
                    nearest = distance;
                    facingFound = true;
                }
            } else if (!facingFound && distance < nearest) {
                lastSelected = target;
                nearest = distance;
            }
        }
        if (lastSelected != -1 &&
            (*reinterpret_cast<std::int8_t *>(actor->retained946 + 0x78) == -1 ||
                ((lastSelected != *reinterpret_cast<std::int8_t *>(actor->retained946 + 0x78) ||
                     game->characters[lastSelected] != *reinterpret_cast<Character **>(actor->retained946 + 0x7a)) &&
                    !*reinterpret_cast<std::uint32_t *>(actor->retained2f8 + 0x2c) && !((actor->hitStateFlags | actor->motionFlags) & 0xc0000U) &&
                    !((actor->activeMotionFlags | actor->secondaryMotionFlags) & 0xc00U))))
            vfSetPairedActor(game, index, lastSelected);
        targets[index] = *reinterpret_cast<std::int8_t *>(actor->retained946 + 0x78);
    }
    for (index = 0; index < 6; ++index) {
        if (!game->characters[index])
            continue;
        actor = game->actors[index];
        if ((actor->activeMotionFlags & 0x2000000U) && FightValue<Character *>(actor, 0x10) && actor->actor && !(actor->actor->moveFlags & 0x20U))
            targets[index] = FightValue<std::int8_t>(actor, 1);
        else if (!(g_pFightStage->battleFlags & 2U) && game->characters[player] && index != player) {
            if (index == opponent)
                targets[index] = player;
            else {
                if (game->slotTeams[index] != 1)
                    mode = 2;
                else if (opponent == -1)
                    mode = 0;
                else {
                    if (lastSelected != player || !(distances[lastSelected][opponent] < 4.0f))
                        continue;
                    mode = 1;
                }
                playerPaired = FightValue<std::int8_t>(game->actors[player], 0x9be);
                nearestEnemy = -1;
                nearestAlly = -1;
                playerEnemy = -1;
                enemyDistance = 9999;
                allyDistance = 9999;
                nearest = 9999;
                for (target = 0; target < 6; ++target) {
                    if (!game->characters[target] || target == player)
                        continue;
                    FightActorRuntimePrefix *const candidate = game->actors[target];
                    if (candidate->moveFlags & 0x20U)
                        continue;
                    distance = distances[index][target];
                    fromPlayer = distances[player][target];
                    if (game->slotTeams[target] == 1 && fromPlayer < nearest && !(candidate->motionFlags & 0x80000U) &&
                        !(candidate->secondaryMotionFlags & 0x800U)) {
                        playerEnemy = target;
                        nearest = fromPlayer;
                    }
                    if (!game->slotTeams[target] && distance < allyDistance) {
                        nearestAlly = target;
                        allyDistance = distance;
                    }
                    if (game->slotTeams[target] == 1 && target != playerPaired && distance < enemyDistance) {
                        nearestEnemy = target;
                        enemyDistance = distance;
                    }
                }
                switch (mode) {
                case 0:
                    if (playerEnemy != -1 && nearestAlly != -1) {
                        if (index == playerEnemy && distances[index][nearestAlly] > 1.5f)
                            targets[index] = player;
                        else
                            targets[index] = nearestAlly;
                    }
                    break;
                case 1:
                    if (nearestAlly != -1)
                        targets[index] = nearestAlly;
                    break;
                case 2:
                    if (nearestEnemy != -1 && distances[index][targets[index]] > 1.5f)
                        targets[index] = nearestEnemy;
                    break;
                }
            }
        }
    }
    for (index = 0; index < 6; ++index) {
        if (!game->characters[index])
            continue;
        actor = game->actors[index];
        lastSelected = targets[index];
        if (lastSelected != -1 && game->characters[lastSelected] &&
            (lastSelected != FightValue<std::int8_t>(actor, 1) || game->characters[lastSelected] != FightValue<Character *>(actor, 0x10)))
            vfSetTargetActor(game, index, lastSelected);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

static const std::int16_t fightGaugePoints[12][2] = {
    {-38, 0}, {-34, -17}, {-21, -31}, {-4, -38}, {14, -35}, {29, -25}, {38, -8}, {38, 9}, {29, 26}, {14, 36}, {-4, 39}, {-21, 32}};

static std::uint8_t fightGaugeCapFull = 1, fightGaugeColourMode = 0, fightGaugeSplitMode = 0;

static std::uint8_t fightGaugeOpponentColours[8] = {15, 17, 14, 19, 20, 21, 22, 18};

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

#define FIGHT_GAUGE_FIELD(type, offset) (*reinterpret_cast<type *>(reinterpret_cast<std::byte *>(gauge) + (offset)))

// @verified - d
void UpdateFightGaugeAnimation(FightActorRuntimePrefix *actor) {
    FightGaugeWork *const gauge = actor->gauge;
    if (!gauge)
        return;
    FightRuntimePrefix *const game = actor->game;
    if (!FIGHT_GAUGE_FIELD(std::uint8_t, 0x20))
        FIGHT_GAUGE_FIELD(std::int16_t, 0x10) += FIGHT_GAUGE_FIELD(std::uint16_t, 0x16) * FIGHT_GAUGE_FIELD(std::int16_t, 0x18);
    if (FIGHT_GAUGE_FIELD(std::int16_t, 0x10) >= FIGHT_GAUGE_FIELD(std::int16_t, 0x1c)) {
        if (FIGHT_GAUGE_FIELD(std::int16_t, 0x14) >= 0) {
            FIGHT_GAUGE_FIELD(std::int16_t, 0x18) = 0;
            FIGHT_GAUGE_FIELD(std::int16_t, 0x10) = FIGHT_GAUGE_FIELD(std::int16_t, 0x1c);
            FIGHT_GAUGE_FIELD(std::uint8_t, 0x20) = 1;
        } else {
            FIGHT_GAUGE_FIELD(std::int16_t, 0x18) = -1;
            FIGHT_GAUGE_FIELD(std::int16_t, 0x10) = FIGHT_GAUGE_FIELD(std::int16_t, 0x1c);
            FIGHT_GAUGE_FIELD(std::uint8_t, 0x20) = 0;
        }
        if (FIGHT_GAUGE_FIELD(std::uint8_t, 0x20) == 1)
            FIGHT_GAUGE_FIELD(std::int16_t, 0x14) -= FIGHT_GAUGE_FIELD(std::uint16_t, 0x16);
    } else if (FIGHT_GAUGE_FIELD(std::int16_t, 0x10) <= FIGHT_GAUGE_FIELD(std::int16_t, 0x1e)) {
        FIGHT_GAUGE_FIELD(std::int16_t, 0x18) = 1;
        FIGHT_GAUGE_FIELD(std::int16_t, 0x10) = FIGHT_GAUGE_FIELD(std::int16_t, 0x1e);
        FIGHT_GAUGE_FIELD(std::uint8_t, 0x20) = 0;
        FIGHT_GAUGE_FIELD(std::int16_t, 0x14) = FIGHT_GAUGE_FIELD(std::int16_t, 0x12);
    }
    if (!FIGHT_GAUGE_FIELD(std::uint8_t, 0x36))
        FIGHT_GAUGE_FIELD(std::int16_t, 0x28) += FIGHT_GAUGE_FIELD(std::uint16_t, 0x2c) * FIGHT_GAUGE_FIELD(std::int16_t, 0x30);
    if (FIGHT_GAUGE_FIELD(std::int16_t, 0x28) >= FIGHT_GAUGE_FIELD(std::int16_t, 0x32)) {
        if (FIGHT_GAUGE_FIELD(std::int16_t, 0x2e) >= 0) {
            FIGHT_GAUGE_FIELD(std::int16_t, 0x30) = 0;
            FIGHT_GAUGE_FIELD(std::int16_t, 0x28) = FIGHT_GAUGE_FIELD(std::int16_t, 0x32);
            FIGHT_GAUGE_FIELD(std::uint8_t, 0x36) = 1;
        } else {
            FIGHT_GAUGE_FIELD(std::int16_t, 0x30) = -1;
            FIGHT_GAUGE_FIELD(std::int16_t, 0x28) = FIGHT_GAUGE_FIELD(std::int16_t, 0x32);
            FIGHT_GAUGE_FIELD(std::uint8_t, 0x36) = 0;
        }
        if (FIGHT_GAUGE_FIELD(std::uint8_t, 0x36) == 1)
            FIGHT_GAUGE_FIELD(std::int16_t, 0x2e) -= FIGHT_GAUGE_FIELD(std::uint16_t, 0x2c);
    } else if (FIGHT_GAUGE_FIELD(std::int16_t, 0x28) <= FIGHT_GAUGE_FIELD(std::int16_t, 0x34)) {
        FIGHT_GAUGE_FIELD(std::int16_t, 0x30) = 1;
        FIGHT_GAUGE_FIELD(std::int16_t, 0x28) = FIGHT_GAUGE_FIELD(std::int16_t, 0x34);
        FIGHT_GAUGE_FIELD(std::uint8_t, 0x36) = 0;
        FIGHT_GAUGE_FIELD(std::int16_t, 0x2e) = FIGHT_GAUGE_FIELD(std::int16_t, 0x2a);
    }
    float divisor;
    if (actor->initialHp < 24)
        divisor = static_cast<float>(actor->initialHp);
    else
        divisor = 24.0f;
    float step;
    if (game->playerSlot == FightValue<std::int8_t>(actor, 0))
        step = static_cast<float>(actor->initialHp) / divisor;
    else
        step = static_cast<float>(actor->initialHp) / divisor * 2.0f;
    float delta;
    if (static_cast<std::int32_t>(game->controlFlags) >= 0) {
        delta = actor->hp - FIGHT_GAUGE_FIELD(float, 0xc);
        if (std::fabs(delta) > step) {
            if (delta < 0.0f)
                delta = -step;
            else
                delta = step;
        }
        FIGHT_GAUGE_FIELD(float, 0xc) += delta;
    } else {
        delta = FIGHT_GAUGE_FIELD(float, 0xc) - (static_cast<float>(actor->initialHp) - actor->hp);
        if (std::fabs(delta) > step) {
            if (delta < 0.0f)
                delta = -step;
            else
                delta = step;
        }
        FIGHT_GAUGE_FIELD(float, 0xc) -= delta;
    }
}

#undef FIGHT_GAUGE_FIELD

#else

namespace {

void UpdateFightGaugeAnimation(VFActor *actor) {
    auto *const gauge = actor->gauge;
    if (!gauge)
        return;
    const auto updateTrack = [gauge](std::size_t valueOffset, std::size_t stateOffset) {
        auto value = FightValue<float>(gauge, valueOffset);
        if (!FightValue<std::uint8_t>(gauge, stateOffset))
            value += shenGetScaledDeltaTickTime(float(FightValue<std::uint16_t>(gauge, valueOffset + 8) * FightValue<std::int16_t>(gauge, valueOffset + 10)));
        const auto upper = FightValue<std::int16_t>(gauge, valueOffset + 14);
        const auto lower = FightValue<std::int16_t>(gauge, valueOffset + 16);
        if (static_cast<std::int32_t>(value) >= upper) {
            const auto terminal = FightValue<std::int16_t>(gauge, valueOffset + 6);
            value = float(upper);
            SetFightValue(gauge, valueOffset + 10, static_cast<std::int16_t>(terminal < 0 ? -1 : 0));
            SetFightValue(gauge, stateOffset, static_cast<std::uint8_t>(terminal >= 0));
            if (terminal >= 0)
                SetFightValue(gauge, valueOffset + 6, static_cast<std::int16_t>(terminal - FightValue<std::uint16_t>(gauge, valueOffset + 8)));
        } else if (static_cast<std::int32_t>(value) <= lower) {
            value = float(lower);
            SetFightValue(gauge, valueOffset + 10, std::int16_t{1});
            SetFightValue(gauge, stateOffset, std::uint8_t{0});
            SetFightValue(gauge, valueOffset + 6, FightValue<std::int16_t>(gauge, valueOffset + 4));
        }
        SetFightValue(gauge, valueOffset, value);
    };
    updateTrack(16, 34);
    // The second track stores its step before its direction (46/50).
    float alpha = FightValue<float>(gauge, 40);
    if (!FightValue<std::uint8_t>(gauge, 56))
        alpha += shenGetScaledDeltaTickTime(float(FightValue<std::uint16_t>(gauge, 46) * FightValue<std::int16_t>(gauge, 50)));
    const auto upper = FightValue<std::int16_t>(gauge, 52), lower = FightValue<std::int16_t>(gauge, 54);
    if (static_cast<std::int32_t>(alpha) >= upper) {
        const auto terminal = FightValue<std::int16_t>(gauge, 48);
        alpha = float(upper);
        SetFightValue(gauge, 50, static_cast<std::int16_t>(terminal < 0 ? -1 : 0));
        SetFightValue(gauge, 56, static_cast<std::uint8_t>(terminal >= 0));
        if (terminal >= 0)
            SetFightValue(gauge, 48, static_cast<std::int16_t>(terminal - FightValue<std::uint16_t>(gauge, 46)));
    } else if (static_cast<std::int32_t>(alpha) <= lower) {
        alpha = float(lower);
        SetFightValue(gauge, 50, std::int16_t{1});
        SetFightValue(gauge, 56, std::uint8_t{0});
        SetFightValue(gauge, 48, FightValue<std::int16_t>(gauge, 44));
    }
    SetFightValue(gauge, 40, alpha);
    const float maximum = float(actor->initialHp), divisor = std::min(maximum, 24.0f);
    float step = maximum / divisor;
    if (actor->game->playerSlot != actor->groupIndex)
        step += step;
    const float displayed = FightValue<float>(gauge, 12);
    const float target = static_cast<std::int32_t>(actor->game->gameFlags) < 0 ? maximum - actor->hp : actor->hp;
    const float delta = std::clamp(target - displayed, -step, step);
    SetFightValue(gauge, 12, displayed + delta);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

std::int32_t PrimaryFightGaugeState(const VFActor *actor) {
    const float unit = float(actor->initialHp) / 12.0f;
    auto full = static_cast<std::uint8_t>(static_cast<std::int32_t>(FightValue<float>(actor->gauge, 12) / unit));
    if (full == 12)
        full = 11;
    if (unit * .5f <= FightValue<float>(actor->gauge, 12) - float(full) * unit)
        return 1;
    return std::fabs(actor->hp) < 0.00000011920929f ? 2 : 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfSetPairedActor(FightRuntimePrefix *game, std::uint8_t index, std::uint8_t target) {
    FightActorRuntimePrefix *actor;
    Character *previous;
    actor = game->actors[index];
    *reinterpret_cast<std::int8_t *>(reinterpret_cast<std::byte *>(actor) + 0x9be) = target;
    *reinterpret_cast<FightActorRuntimePrefix **>(reinterpret_cast<std::byte *>(actor) + 0x9c4) = game->actors[target];
    previous = *reinterpret_cast<Character **>(reinterpret_cast<std::byte *>(actor) + 0x9c0);
    *reinterpret_cast<Character **>(reinterpret_cast<std::byte *>(actor) + 0x9c0) = game->characters[target];
    if (previous != *reinterpret_cast<Character **>(reinterpret_cast<std::byte *>(actor) + 0x9c0))
        actor->reactionFlags = 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfSetTargetActor(FightRuntimePrefix *game, std::uint8_t index, std::uint8_t target) {
    FightActorRuntimePrefix *actor;
    Character *previous;
    actor = game->actors[index];
    FightValue<std::int8_t>(actor, 1) = target;
    actor->actor = game->actors[target];
    previous = FightValue<Character *>(actor, 0x10);
    FightValue<Character *>(actor, 0x10) = game->characters[target];
    if (previous != FightValue<Character *>(actor, 0x10))
        FightValue<std::int8_t>(actor, 0x9a4) = -1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void UpdateFormationRuntimeFlags(FightRuntimePrefix *game) {
    for (std::uint8_t index = 0; index < 6; ++index) {
        if (!game->characters[index])
            continue;
        FightActorRuntimePrefix *const actor = game->actors[index];
        const std::uint32_t flags = actor->formationFlags;
        if ((flags & 0x10U) && *reinterpret_cast<std::int8_t *>(reinterpret_cast<std::byte *>(actor) + 0x87f) != -1)
            actor->runtimeFlags |= 0x10U;
        if ((flags & 4U) &&
            *reinterpret_cast<std::int8_t *>(reinterpret_cast<std::byte *>(actor) + 0x87f) ==
                *reinterpret_cast<std::int16_t *>(reinterpret_cast<std::byte *>(actor) + 0x934))
            actor->runtimeFlags |= 4U;
        if ((flags & 0x40U) && (actor->hitStateFlags & 0x2000U))
            actor->runtimeFlags |= 0x40U;
        if ((flags & 0x80U) && (actor->hitStateFlags & 0x4000U))
            actor->runtimeFlags |= 0x80U;
        if ((flags & 0x100U) &&
            *reinterpret_cast<std::uint16_t *>(reinterpret_cast<std::byte *>(actor) + 0x93e) ==
                *reinterpret_cast<std::uint16_t *>(reinterpret_cast<std::byte *>(actor) + 0x93a)) {
            actor->runtimeFlags |= 0x100U;
            *reinterpret_cast<std::uint16_t *>(reinterpret_cast<std::byte *>(actor) + 0x93e) = 0;
        }
        if ((flags & 0x200U) &&
            *reinterpret_cast<std::uint16_t *>(reinterpret_cast<std::byte *>(actor) + 0x940) ==
                *reinterpret_cast<std::uint16_t *>(reinterpret_cast<std::byte *>(actor) + 0x93c)) {
            actor->runtimeFlags |= 0x200U;
            *reinterpret_cast<std::uint16_t *>(reinterpret_cast<std::byte *>(actor) + 0x940) = 0;
        }
    }
}

#else

namespace {

// @verified - p
void UpdateFormationRuntimeFlags(VFMA_t *game) {
    for (std::uint8_t i = 0; i < 6; ++i) {
        if (!game->characters[i])
            continue;
        auto &actor = *game->actors[i];
        const auto flags = actor.formationFlags;
        const auto paired = FightValue<std::int8_t>(&actor, 0x99f);
        if ((flags & 0x10) && paired != -1)
            actor.formationRuntimeFlags |= 0x10;
        if ((flags & 4) && paired == FightValue<std::int16_t>(&actor, 0xa54))
            actor.formationRuntimeFlags |= 4;
        if ((flags & 0x40) && (actor.hitStateFlags & VFHSF_HeavyReaction))
            actor.formationRuntimeFlags |= 0x40;
        if ((flags & 0x80) && (actor.hitStateFlags & VFHSF_SpecialGuardOrParry))
            actor.formationRuntimeFlags |= 0x80;
        if ((flags & 0x100) && static_cast<std::int32_t>(actor.moveAngleTableIndex) >= actor.spawnYaw[1]) {
            actor.formationRuntimeFlags |= 0x100;
            actor.moveAngleTableIndex = 0;
        }
        if ((flags & 0x200) && static_cast<std::int32_t>(FightValue<float>(&actor, 0xa64)) >= actor.sfxPeriod) {
            actor.formationRuntimeFlags |= 0x200;
            SetFightValue(&actor, 0xa64, 0.0f);
        }
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfReleaseFightInput(FightRuntimePrefix *game) {
    if (game->inputBuffer)
        BindPadControlEntry(static_cast<PadControlEntry *>(game->inputBuffer));
    if (game->savedInput)
        ReleasePadControlEntry(static_cast<PadControlEntry *>(game->savedInput));
    if (!(g_pFightStage->battleFlags & 2U) && FightValue<std::uint8_t>(game, 0x1db))
        SetPlayerControlEnabled(1);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfDrawFightActorGauge(FightActorRuntimePrefix *actor) {
    std::uint32_t partialState;
    FightRuntimePrefix *const game = actor->game;
    FightGaugeWork *const gauge = actor->gauge;
    const float scale = FightValue<float>(gauge, 0) / 100.0f;
    const std::uint16_t opacityWord = FightValue<std::uint16_t>(gauge, 0x1a);
    float opacity;
    if (opacityWord)
        opacity = static_cast<float>(opacityWord) / 255.0f;
    else
        opacity = 0;
    if (game->playerSlot == FightValue<std::int8_t>(actor, 0)) {
        vfDrawBattleGaugeIcon(6, 72, 404, scale, 1.0f);
        if (g_pFightStage->battleFlags & 0x8000U)
            DrawBattleQteSprite(9, 144, 404, 1.3f, opacity, 0, 0, 0.0f);
    } else {
        vfDrawBattleGaugeIcon(6, 568, 404, scale, 1.0f);
        if (g_pFightStage->battleFlags & 0x8000U)
            DrawBattleQteSprite(9, 494, 404, 1.3f, opacity, 0, 0x100, 0.0f);
    }
    float unit = actor->initialHp;
    unit /= 12.0f;
    std::uint8_t full = static_cast<std::int32_t>(FightValue<float>(gauge, 0xc) / unit);
    if (fightGaugeCapFull == 1 && full == 12)
        full = 11;
    const float partial = FightValue<float>(gauge, 0xc) - full * unit;
    if (static_cast<std::int32_t>(game->controlFlags) >= 0) {
        if (partial < unit * 0.5f) {
            if (std::fabs(actor->hp) < 0.00000011920929f)
                partialState = 2;
            else
                partialState = 0;
        } else {
            partialState = 1;
        }
    } else {
        if (partial < unit * 0.5f) {
            if (actor->hp >= static_cast<float>(actor->initialHp))
                partialState = 2;
            else
                partialState = 0;
        } else {
            partialState = 1;
        }
    }
    std::uint32_t icon = 24;
    if ((actor->formationFlags & 0x40000U) && game->playerSlot == FightValue<std::int8_t>(actor, 0))
        icon = 17;
    else if ((game->controlFlags & 0x400000U) && game->playerSlot == FightValue<std::int8_t>(actor, 0))
        icon = 14;
    else if (!fightGaugeColourMode) {
        if (game->playerSlot == FightValue<std::int8_t>(actor, 0))
            icon = 15;
        else
            icon = *(fightGaugeOpponentColours + static_cast<std::uint8_t>(FightValue<std::int8_t>(game->actors[game->playerSlot], 1)));
    } else if (fightGaugeColourMode == 1) {
        if (full <= 4)
            icon = 17;
        else if (full <= 8)
            icon = 19;
        else
            icon = 15;
    } else if (fightGaugeColourMode == 2) {
        if (full <= 4)
            icon = 15;
        else if (full <= 8)
            icon = 19;
        else
            icon = 17;
    }
    std::uint8_t index = 0;
    if (!fightGaugeSplitMode) {
        for (index = 0; index < 12; ++index) {
            if (index < full) {
                vfDrawFightGaugeOrb(actor, index, static_cast<std::int16_t>(FightValue<std::int16_t>(gauge, 0x10)), icon);
                vfDrawFightGaugeOrb(actor, index, 255, 7);
            }
        }
    } else if (fightGaugeSplitMode == 1) {
        for (index = 0; index < 12; ++index) {
            if (index < full) {
                std::int32_t band;
                if (full <= 3)
                    band = 0;
                else if (full <= 7)
                    band = index <= 3;
                else
                    band = index > 7 ? 0 : index <= 3 ? 1 : 2;
                switch (band) {
                case 0:
                    vfDrawFightGaugeOrb(actor, index, static_cast<std::int16_t>(FightValue<std::int16_t>(gauge, 0x10)), icon);
                    break;
                case 1:
                    vfDrawFightGaugeOrb(actor, index, static_cast<std::int16_t>(FightValue<std::int16_t>(gauge, 0x10)), 15);
                    break;
                case 2:
                    vfDrawFightGaugeOrb(actor, index, static_cast<std::int16_t>(FightValue<std::int16_t>(gauge, 0x10)), 19);
                    break;
                }
                vfDrawFightGaugeOrb(actor, index, 255, 7);
            }
        }
    }
    if (!fightGaugeColourMode) {
        switch (partialState) {
        case 0:
            vfDrawFightGaugeOrb(actor, full, static_cast<std::int16_t>(FightValue<std::int16_t>(gauge, 0x28)), 19);
            break;
        case 1:
            vfDrawFightGaugeOrb(actor, full, static_cast<std::int16_t>(FightValue<std::int16_t>(gauge, 0x28)), icon);
            break;
        default:
            break;
        }
        vfDrawFightGaugeOrb(actor, full, 255, 7);
    }
    if (g_pFightStage->battleFlags & 0x8000U)
        vfDrawFightRoundMarkers(game, static_cast<std::int16_t>(FightValue<std::int16_t>(gauge, 0x28)));
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void UpdateFightGauge(void *parameter) {
    FightRuntimePrefix *const game = static_cast<FightRuntimePrefix *>(parameter);
    if (game->type == 2 || game->type == 4 || game->type == 9 || (game->stateFlags & 0x4018U))
        return;
    FightActorRuntimePrefix *const player = game->actors[game->playerSlot];
    FightActorRuntimePrefix *const opponent = game->actors[FightValue<std::int8_t>(player, 1)];
    for (std::uint8_t index = 0; index < g_pFightStage->enemyCount; ++index) {
        FCMG_t *const slot = &g_pFightStage->enemySlots[index];
        if (slot && slot->character && slot->fighter && slot->state == 2)
            UpdateFightGaugeAnimation(slot->fighter);
    }
    vfDrawFightActorGauge(player);
    if (!(game->controlFlags & 0x40000000U))
        vfDrawFightActorGauge(opponent);
}

#else

namespace {

// @verified - p
void UpdateFightGauge(void *parameter) {
    auto *const game = static_cast<VFMA_t *>(parameter);
    if (game->type <= 9 && (532U & (1U << game->type)))
        return;
    if (game->parameter574 == 1 || (game->mode & 0x4018))
        return;
    auto *const player = game->actors[game->playerSlot];
    auto *const enemy = game->actors[player->actionIndex];
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        auto &slot = g_pFightStage->enemySlots[i];
        if (slot.character && slot.fighter && slot.state == 2)
            UpdateFightGaugeAnimation(slot.fighter);
    }
    ExecuteFightGaugeUI();
    if (GetDebugForceUIEnabled(UIElement::FightGauge)) {
        SetFightGaugeDisplayLayout(0);
        SetFightGaugePrimaryVisuals(PrimaryFightGaugeState(player), FightValue<float>(player->gauge, 40));
        SetPlayerEnemyPercents(
            FightValue<float>(player->gauge, 12) / float(player->initialHp) * 100, FightValue<float>(enemy->gauge, 12) / float(enemy->initialHp) * 100);
        FillFightGaugeSecondaryOrbStates(enemy->game->actors[enemy->game->playerSlot]->actionIndex);
        EnableElement(UIElement::FightGauge);
        if (g_pFightStage->battleFlags & 0x40000)
            UpdateFightRoundMarkers(g_vfRoundResultCounters[0], g_vfRoundResultCounters[1]);
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

// PC's debug-force gate always claims the HUD for UI35; the legacy level strip below is dormant there.
// @verified - p
void adjustStreetFighterHUD() {
    if (GetDebugForceUIEnabled(UIElement::StreetFighterHud))
        return;
    VFMA_t *const game = vfmaGetTaskParam();
    if (!game || !(g_pFightStage->battleFlags & 0x10000U))
        return;
    LegacySpritePacket packet;
    packet.colour = 0xffffffffU;
    packet.secondaryColour = 0;
    packet.uv = {0, 0, 1, 1};
    packet.reservedTail = 0;
    packet.quad.flags = 0x400U;
    std::int32_t value = g_vfStreetFightLevel;
    std::int32_t x = 600;
    std::int32_t next;
    do {
        next = value / 10;
        const std::int32_t digit = value % 10;
        if (g_fightLoadDigits[digit] != -1) {
            AdjustScreenAspectRatio(packet.quad, ScreenAlignment::HorizontalStretch, x, 90, 32, 32, ScreenMode::Stretched);
            LegacySpriteNullsub(packet, static_cast<std::uint32_t>(g_fightLoadDigits[digit]), static_cast<float>(game->spriteDepth));
            x -= 32;
        }
        value = next;
    } while (next > 0);
    DrawBattleQteSprite(12, x - 32, 90, 1.0f, 1.0f, 0, 0, 0.0f, ScreenMode::Stretched);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

std::int8_t g_fightReflectorFadeSteps = 10;
Vector4 g_fightReflectorTarget = {1.0f, 0.2f, 0.2f, 0.33f};

void SetupFightReflectorBoard(void *parameter) {
    FightRuntimePrefix *const game = static_cast<FightRuntimePrefix *>(parameter);
    if (g_pFightStage->battleFlags & 0x200U)
        return;
    if (g_pFightStage->battleFlags & 2U)
        return;
    for (std::int32_t index = 0; index < g_pFightStage->enemyCount; ++index) {
        FCMG_t *const slot = &g_pFightStage->enemySlots[index];
        if (!slot || !slot->character)
            continue;
        if (game->stateFlags & 1U)
            return;
        std::int32_t active;
        if (game->controlFlags & 0x4000000U)
            active = 0;
        else
            active = static_cast<std::uint8_t>(slot->state - 2) <= 1;
        if (active) {
            slot->reflectorColour.x += g_fightReflectorTarget.x / g_fightReflectorFadeSteps;
            slot->reflectorColour.y += g_fightReflectorTarget.y / g_fightReflectorFadeSteps;
            slot->reflectorColour.z += g_fightReflectorTarget.z / g_fightReflectorFadeSteps;
            slot->reflectorColour.w += g_fightReflectorTarget.w / g_fightReflectorFadeSteps;
            if (slot->reflectorColour.x > g_fightReflectorTarget.x)
                slot->reflectorColour.x = g_fightReflectorTarget.x;
            if (slot->reflectorColour.y > g_fightReflectorTarget.y)
                slot->reflectorColour.y = g_fightReflectorTarget.y;
            if (slot->reflectorColour.z > g_fightReflectorTarget.z)
                slot->reflectorColour.z = g_fightReflectorTarget.z;
            if (slot->reflectorColour.w > g_fightReflectorTarget.w)
                slot->reflectorColour.w = g_fightReflectorTarget.w;
            ResetReflectorBoardMode(slot->character, 1);
            SetReflectorBoardValues(slot->character, &slot->reflectorColour);
        } else {
            slot->reflectorColour.x -= g_fightReflectorTarget.x / g_fightReflectorFadeSteps;
            slot->reflectorColour.y -= g_fightReflectorTarget.y / g_fightReflectorFadeSteps;
            slot->reflectorColour.z -= g_fightReflectorTarget.z / g_fightReflectorFadeSteps;
            slot->reflectorColour.w -= g_fightReflectorTarget.w / g_fightReflectorFadeSteps;
            if (slot->reflectorColour.y < 0.0f)
                slot->reflectorColour.y = 0.0f;
            if (slot->reflectorColour.z < 0.0f)
                slot->reflectorColour.z = 0.0f;
            if (slot->reflectorColour.w < 0.0f)
                slot->reflectorColour.w = 0.0f;
            if (slot->reflectorColour.x < 0.0f) {
                slot->reflectorColour.x = 0.0f;
                ResetReflectorBoardMode(slot->character, 0);
            } else {
                ResetReflectorBoardMode(slot->character, 1);
                SetReflectorBoardValues(slot->character, &slot->reflectorColour);
            }
        }
    }
}

#else

namespace {

// Native setupRefBoard: reflector values belong to each FCMG, not to the UI renderer.
// @verified - p
void SetupFightReflectorBoard(void *parameter) {
    auto *const game = static_cast<VFMA_t *>(parameter);
    if (g_pFightStage->battleFlags & 0x202)
        return;
    constexpr Vector4 target{1.0f, .2f, .2f, .33f};
    for (std::uint8_t i = 0; i < g_pFightStage->enemyCount; ++i) {
        auto &slot = g_pFightStage->enemySlots[i];
        if (!slot.character)
            continue;
        if (game->mode & 1)
            return;
        // Fading clamps only at zero and brightening only at the target; colours clamp before the intensity test.
        if ((game->gameFlags & 0x4000000) || static_cast<std::uint8_t>(slot.state - 1) > 2) {
            slot.reflectorIntensity -= target.x / 10;
            slot.reflectorColour.x -= target.y / 10;
            slot.reflectorColour.y -= target.z / 10;
            slot.reflectorColour.z -= target.w / 10;
            if (slot.reflectorColour.x < 0)
                slot.reflectorColour.x = 0;
            if (slot.reflectorColour.y < 0)
                slot.reflectorColour.y = 0;
            if (slot.reflectorColour.z < 0)
                slot.reflectorColour.z = 0;
            if (slot.reflectorIntensity < 0) {
                slot.reflectorIntensity = 0;
                ResetReflectorBoardMode(slot.character, 0);
                continue;
            }
        } else {
            slot.reflectorIntensity += target.x / 10;
            slot.reflectorColour.x += target.y / 10;
            slot.reflectorColour.y += target.z / 10;
            slot.reflectorColour.z += target.w / 10;
            if (slot.reflectorIntensity > target.x)
                slot.reflectorIntensity = target.x;
            if (slot.reflectorColour.x > target.y)
                slot.reflectorColour.x = target.y;
            if (slot.reflectorColour.y > target.z)
                slot.reflectorColour.y = target.z;
            if (slot.reflectorColour.z > target.w)
                slot.reflectorColour.z = target.w;
        }
        SetReflectorBoardMode(slot.character, 1);
        SetReflectorBoardValues(slot.character, {slot.reflectorIntensity, slot.reflectorColour.x, slot.reflectorColour.y, slot.reflectorColour.z});
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void DrawFightHud(VFMA_t *game) {
    if (g_hasColouredText)
        ConstructColouredText(static_cast<std::uint32_t>(FightTruncate(game->blurAmount)), 188, 49, 1.0f);
    adjustStreetFighterHUD();
    constructNumStrikesLeft();
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void PublishFightHud(VFMA_t *game) {
    HLib::ExecuteCallbackNow(SetupFightReflectorBoard, game);
    HLib::ExecuteCallbackNow(UpdateFightGauge, game);
    DrawFightHud(game);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void vfDrawFightGaugeOrb(FightActorRuntimePrefix *actor, std::uint16_t index, std::uint32_t alpha, std::uint8_t icon) {
    FightGaugeWork *const gauge = actor->gauge;
    const float offsetScale = FightValue<float>(gauge, 8) / 100.0f;
    const float scale = FightValue<float>(gauge, 4) / 100.0f;
    std::uint16_t x;
    std::uint16_t y;
    if (FightValue<std::int8_t>(actor, 0) == actor->game->playerSlot) {
        x = static_cast<std::int32_t>(72.0f + fightGaugePoints[index][0] * offsetScale);
        y = static_cast<std::int32_t>(404.0f + fightGaugePoints[index][1] * offsetScale);
    } else {
        x = static_cast<std::int32_t>(568.0f + fightGaugePoints[index][0] * offsetScale);
        y = static_cast<std::int32_t>(404.0f + fightGaugePoints[index][1] * offsetScale);
    }
    const float opacity = alpha ? static_cast<float>(alpha) / 255.0f : 0.0f;
    vfDrawBattleGaugeIcon(icon, x, y, scale, opacity);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

void vfDrawFightRoundMarkers(FightRuntimePrefix *game, std::uint32_t alpha) {
    std::int32_t enabled[4] = {0, 0, 0, 0};
    std::int32_t positions[4][2] = {{132, 432}, {146, 400}, {508, 432}, {494, 400}};
    if (game->stateFlags & 0x4000U)
        return;
    if (g_vfRoundWins != 0)
        enabled[0] = 1;
    if (g_vfRoundWins > 1)
        enabled[1] = 1;
    if (g_vfRoundLosses != 0)
        enabled[2] = 1;
    if (g_vfRoundLosses > 1)
        enabled[3] = 1;
    float opacity;
    opacity = static_cast<float>(alpha);
    opacity /= 255.0f;
    for (std::int32_t index = 0; index < 4; ++index) {
        if (enabled[index]) {
            vfDrawBattleGaugeIcon(10, positions[index][0], positions[index][1], 1.3f, opacity);
            vfDrawBattleGaugeIcon(11, positions[index][0], positions[index][1], 1.3f, 1.0f);
        }
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

void AccumulateFightCombatStats(FightRuntimePrefix *game) {
    accumulatedFightBlur += game->blurAmount;
    switch (game->type) {
    case 2:
    case 4:
        return;
    case 5: {
        if (game->stateFlags & 0x200U)
            return;
        const std::uint16_t count = static_cast<std::int8_t>(FightValue<std::uint8_t>(game, 0x23b));
        if (count)
            AddFightStatistic(0xe76, count);
        AddFightStatistic(0xe46, 1);
        std::uint32_t &time = FightValue<std::uint32_t>(&g_combatStatistics, 0xe48);
        const std::uint32_t total = time + static_cast<std::uint32_t>(accumulatedFightBlur);
        time = total > 1080000000U ? 1080000000U : total;
        std::uint32_t *const record = reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(&g_combatStatistics) + 0xb7c);
        if (record[0] > 100 && FindBattleStatusEntry(game->playerCharacter) == 3 && game->blurAmount < record[5])
            record[5] = game->blurAmount;
        return;
    }
    }
    for (std::uint8_t index = 0; index < 128; ++index) {
        if (static_cast<std::uint8_t>(index - 97) < 3)
            continue;
        const std::uint16_t first = FightValue<std::uint16_t>(game, 0x23e + index * 2);
        const std::uint16_t second = FightValue<std::uint16_t>(game, 0x33e + index * 2);
        if (first)
            AddFightStatistic(0xba4 + index * 2, first);
        if (second)
            AddFightStatistic(0xca4 + index * 2, second);
    }
    const std::uint16_t difference =
        static_cast<std::int8_t>(FightValue<std::uint8_t>(game, 0x23b)) - static_cast<std::int8_t>(FightValue<std::uint8_t>(game, 0x1dd));
    if (difference)
        AddFightStatistic(0xe44, difference);
    if (game->playerCharacter)
        g_nativePlayerStats.defaultHealth = ReadFightCharacterValue(0, game->playerCharacter);
}

#else

namespace {

void AccumulateFightCombatStats(VFMA_t *game) {
    accumulatedFightBlur += game->blurAmount;
    const auto current = static_cast<std::int8_t>(game->counter32c >> 8);
    const auto previous = static_cast<std::int8_t>(game->parameter2b9);
    const auto delta = current == previous ? std::uint16_t{0} : static_cast<std::uint16_t>(current - previous);
    std::int32_t health = -1;
    if (game->playerCharacter) {
        for (const auto &entry : g_activeFightData)
            if (entry.character == game->playerCharacter) {
                health = static_cast<std::int32_t>(entry.hp);
                break;
            }
    }
    S2Save_AccumulateCombatStatistics(game->collisionHistory, delta, health);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
void vfBeginScriptedFormationMotion(FightRuntimePrefix *game) {
    game->stateFlags |= 0x800U;
}

} // namespace

#else

namespace {

void vfBeginScriptedFormationMotion(VFMA_t *game) {
    game->mode |= 0x800;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

// Drop the scripted-wave lock unless an initialized actor still holds it.
void vfReleaseUnheldScriptedFormation(VFMA_t *game) {
    std::uint32_t formations = 0;
    for (std::uint8_t i = 0; i < 6; ++i)
        if (game->characters[i])
            formations |= game->actors[i]->formationFlags;
    if (!(formations & 0x8000))
        game->mode &= ~0x801U;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

namespace {

// @verified - d
void vfSetupFightInput(FightRuntimePrefix *game) {
    const std::int32_t enabled = IsPlayerControlEnabled();
    FightValue<std::uint8_t>(game, 0x1db) = enabled;
    if (g_pFightStage->battleFlags & 2U) {
        game->inputBuffer = 0;
        game->savedInput = 0;
    } else {
        if (FightValue<std::uint8_t>(game, 0x1db))
            SetPlayerControlEnabled(0);
        PadControlEntry *const entry = AllocatePadControlEntry();
        game->savedInput = entry;
        game->inputBuffer = BindPadControlEntry(entry);
    }
}

} // namespace

#endif

} // namespace s2
} // namespace shenmue
