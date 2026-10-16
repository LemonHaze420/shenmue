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

namespace shenmue::s2 {
std::uint8_t g_savedFightBlurFlag{};
std::int32_t g_fightCombatCounterSnapshot{};


void vfSetupFightCameraState(VFMA_t *game) {
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
        camCtrlSetState(CameraState::ChaseGoal);
    }
}

namespace {
template <class T> T FightValue(const void *owner, std::size_t offset) {
    T value;
    std::memcpy(&value, static_cast<const std::byte *>(owner) + offset, sizeof(value));
    return value;
}
template <class T> void SetFightValue(void *owner, std::size_t offset, T value) {
    std::memcpy(static_cast<std::byte *>(owner) + offset, &value, sizeof(value));
}

std::int32_t FightTruncate(float value) {
    return value >= -2147483648.0f && value < 2147483648.0f ? static_cast<std::int32_t>(value) : std::numeric_limits<std::int32_t>::min();
}
std::int32_t savedPostFxBank{};
std::uint16_t postBattleWaitFrames{};
float accumulatedFightBlur{};
std::uint8_t blurModelFlag{}, blurModelPreviousFlag{};
VCDS_t *FindCameraDialogue(VFMA_t *game, std::uint32_t id) {
    if (!game->cameraDialogue)
        return nullptr;
    for (auto *record = game->cameraDialogue->records; record; record = record->next)
        if (record->id == id)
            return record;
    return nullptr;
}
// @verified - p
void DisableCameraDialogue(VFMA_t *game, std::uint32_t id) {
    if (id)
        if (auto *record = FindCameraDialogue(game, id))
            record->state = 0;
}
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
                return (actor->hitStateFlags & 0x4000) && actor->actor && record->character.name == actor->actor->slot->id && accepts(actor);
            });
            break;
        case 5:
            if (!self)
                continue;
            match = actorMatches([&](VFActor *actor) {
                return (actor->hitStateFlags & 0x84000) && (actor->hitStateFlags & 0x100) && actor->actor && record->character.name == actor->actor->slot->id &&
                    accepts(actor);
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
                return (self->hitStateFlags & 0x40000) && (actor->hitStateFlags & 0x80000) && !(self->hitStateFlags & 0x100000) && paired &&
                    static_cast<std::uint16_t>(paired->animationTime) == paired->parameter33c[1] + 1 && record->character.name == paired->slot->id &&
                    accepts(actor);
            });
            break;
        case 11:
            if (!self)
                continue;
            if ((self->previousHitStateFlags & 0x100000) || !(self->hitStateFlags & 0x100000) || !self->secondActor)
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
            match = (self->hitStateFlags & 0x80100) == 0x100 && self->currentAnimation != self->previousAnimation &&
                (!record->otherCharacter.name || (self->actor && record->otherCharacter.name == self->actor->slot->id));
            break;
        case 13:
            if (!self)
                continue;
            if ((self->activeMotionFlags & 0x200000) && accepts(self)) {
                match = true;
                break;
            }
            [[fallthrough]];
        case 16:
            if (!self)
                continue;
            if ((self->previousHitStateFlags & 0x10000000) || !(self->hitStateFlags & 0x10000000))
                break;
            if (record->otherCharacter.name) {
                if (!self->actor || record->otherCharacter.name != self->actor->slot->id)
                    break;
                if (!other)
                    continue;
                if (!(other->hitStateFlags & 0x100))
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
            record->idleFrames = (self->cameraControlFlags & 15) ? 0 : record->idleFrames + 1;
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
    const float maximum = float(actor->initialHp), divisor = std::min(maximum,24.0f);
    float step = maximum / divisor;
    if (actor->game->playerSlot != actor->groupIndex)
        step += step;
    const float displayed = FightValue<float>(gauge, 12);
    const float target = static_cast<std::int32_t>(actor->game->gameFlags) < 0 ? maximum - actor->hp : actor->hp;
    const float delta = std::clamp(target - displayed, -step, step);
    SetFightValue(gauge, 12, displayed + delta);
}

std::int32_t PrimaryFightGaugeState(const VFActor *actor) {
    const float unit = float(actor->initialHp) / 12.0f;
    auto full = static_cast<std::uint8_t>(static_cast<std::int32_t>(FightValue<float>(actor->gauge, 12) / unit));
    if (full == 12)
        full = 11;
    if (unit * .5f <= FightValue<float>(actor->gauge, 12) - float(full) * unit)
        return 1;
    return std::fabs(actor->hp) < 0.00000011920929f ? 2 : 0;
}


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



void adjustStreetFighterHUD() {
    (void)GetDebugForceUIEnabled(UIElement::StreetFighterHud);
}

void DrawFightHud(VFMA_t *game) {
    if (g_hasColouredText)
        ConstructColouredText(static_cast<std::uint32_t>(FightTruncate(game->blurAmount)), 188, 49, 1.0f);
    adjustStreetFighterHUD();
    constructNumStrikesLeft();
}
void PublishFightHud(VFMA_t *game) {
    HLib::ExecuteCallbackNow(SetupFightReflectorBoard, game);
    HLib::ExecuteCallbackNow(UpdateFightGauge, game);
    DrawFightHud(game);
}
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
// @verified - p
bool SetupContactSlot(VFMA_t *game, FRMG_t *group) {
    const auto index = vfRegisterContactSlot(game, group->slot);
    if (index == 99)
        return false;
    vfUpdatePairingsAndDistances(game);

    vfInitializeFightActor(game, index);
    vfResetActorActionState(game->actors[index]);
    vfInitializeActorCollision(game, index);
    if (game->playerCharacter && game->characters[index] == game->playerCharacter)
        game->playerSlot = static_cast<std::int8_t>(index);
    vfUpdateGuardLevel(game, index);
    return true;
}

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
// @verified - p
void UpdateDefeatAndRespawn(VFMA_t *game) {
    for (std::uint8_t i = 0; i < 6; ++i) {
        if (!game->characters[i])
            continue;
        auto *const actor = game->actors[i];
        if ((actor->moveFlags & 0x24) == 0x20 && SetMotionRegionFlag(actor->motion, 65, 1, MotionRegion::LowerBody)) {
            auto *const slot = game->enemySlots[i];
            vfTransitionEnemySlotToCooldown(game, slot);
            if (static_cast<std::uint8_t>(g_pFightStage->groupCounters[0] + g_pFightStage->groupCounters[1]) > game->maximumActiveEnemies)
                vfUpdateEnemySlotCooldowns(game);
            ActivateBlockedFormations(game, slot);
        }
        if (std::fabs(actor->hp) < 0.00000011920929f && !(actor->moveFlags & 0x20)) {
            actor->moveFlags |= 0x20;
            if (g_pFightStage->battleFlags & 0x40000) {
                const std::uint8_t player = actor->groupIndex == static_cast<std::uint8_t>(game->playerSlot);
                vfAddRoundResults(player ^ 1, player);
            }
        }
    }
}
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
        if ((flags & 0x40) && (actor.hitStateFlags & 0x2000))
            actor.formationRuntimeFlags |= 0x40;
        if ((flags & 0x80) && (actor.hitStateFlags & 0x4000))
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

// @verified - p
void SetOtherFightCharaAlphas(VFActor *actor, float alpha) {
    for (std::uint8_t i = 0; i < 6; ++i)
        if (actor->game->characters[i] && actor->game->actors[i] != actor)
            modelSetCharaAlpha(actor->game->characters[i], alpha);
}
// The remaining time is published only after the camera, CPAM and alpha writes.
// @verified - p
void UpdateFightCharaAlphas(VFMA_t *game) {
    if (!(game->targetTime > 0))
        return;
    float remaining = game->targetTime - shenGetScaledDeltaTickTime(2);
    if (remaining <= 0) {
        remaining = 0;
        camCtrlSetState(CameraState::ChaseGoal);
        for (std::uint8_t i = 0; i < 6; ++i) {
            if (!game->characters[i])
                continue;
            auto &actor = *game->actors[i];
            cpamSetFlag(actor.character, 1);
            if (!(std::to_integer<std::uint8_t>(actor.motionParameters[2][25]) & 1))
                actor.moveFlags &= ~0x40000U;
            if (actor.moveFlags & 0x10000) {
                actor.moveFlags &= ~0x10000U;
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
            vfClsampleCurrentMotion(slot.motion, 0, output);
        }
    }
    if (!(g_pFightStage->battleFlags & 2))
        UpdateFightCharaAlphas(game);
    vfmaSetupDialogueReactionsEtc(game);
}

// @verified - p
void FreeFightCameraDialogue(VFMA_t *game) {
    RetireCameraDialogueRecords();
    if (game->cameraDialogue)
        HLib::FreeHeapSpace(game->cameraDialogue, static_cast<std::int32_t>(TASK_VCDW.name));
    game->cameraDialogue = nullptr;
}
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
// @verified - p
void ReleaseFight(VFMA_t *game) {
    if (game->type == 9)
        vfFreeVFHDSpace(game);
    FreeFightCameraDialogue(game);
    SetMotionEnvironmentEnabled(1);
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
    SetMotionEnvironmentEnabled(1);
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
}


void vfFightNullsub() {
}


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

// @verified - p
void BeginFightDialogueRetirement() {
    VFMA_t *const game = vfmaGetTaskParam();
    if (game && game->cameraDialogue)
        game->cameraDialogue->flags = (game->cameraDialogue->flags & 0xffffU) | 0x10000U;
}

// @verified - p
std::uint32_t GetFightTaskFlags() {
    VFMA_t *const game = vfmaGetTaskParam();
    return game ? game->mode : 0;
}

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

void blurInit() {
    blurModelFlag = blurModelPreviousFlag = g_savedFightBlurFlag = 0;
    accumulatedFightBlur = 0;
}

namespace {
void vfBeginScriptedFormationMotion(VFMA_t *game) {
    game->mode |= 0x800;
}

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
    game->maximumActiveEnemies = type == 10 ? 99 : std::min(maximumEnemies,g_pFightStage->enemyCount);
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


// @verified - p
void vfm_Callback_FightTaskEntry_Callback(void *parameter) {
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
            actor->stateFlags &= ~0x40000U;
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
                camCtrlSetState(CameraState::ChaseGoal);
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
                    if (!(actor->hitStateFlags & 0x81d010) && actor->parameter2dc != 4)
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
            auto *motion = FindMotionWork(game->characters[i]);
            if (!mtwkSetExtraFlag(motion, 3, 1) &&
                (!(actor->hitStateFlags & 0x40000) || GetMotionEndFrame(motion, MotionRegion::LowerBody) <= GetMotionFrame(motion, MotionRegion::LowerBody)))
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

} // namespace shenmue::s2
