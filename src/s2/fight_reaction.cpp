#include "s2/fight_stage.hpp"
#include "s2/motion_work.hpp"
#include "s2/math.hpp"
#include "s2/motion.hpp"
#include "s2/save.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/event_runtime.hpp"
#include "s2/ui.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/input.hpp"
#include "s2/pause.hpp"
#include "s2/area_event.hpp"
#include "s2/flag.hpp"
#include "s2/frame_pacing.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>

namespace shenmue::s2 {
namespace {
std::uint8_t AuthoredByte(const VFActor &self, std::size_t offset) {
    return std::to_integer<std::uint8_t>(self.motionParameters[2][offset]);
}
bool PhaseSelected(const VFSlotPhaseConfig &phase) {
    return phase.enabled && (phase.reserved ? phase.countdown != phase.selected : phase.countdown == phase.selected);
}
bool Chance(std::uint8_t value, std::uint8_t random) {
    return value && value >= random;
}
std::int8_t GuardProximity(VFActor &self, VFSlotRuntimeState &slot) {
    if ((self.actor->hitStateFlags & 0x104) != 0x100 || (self.hitStateFlags & 0x1000) || (slot.cameraFlags & 0x2000) || (self.activeMotionFlags & 0x4000) ||
        self.hitPhaseCounters[0] > 1 || self.contactDistances[self.actionIndex] > 2.5f)
        return -1;
    return 5;
}
std::int8_t GuardPhase(VFActor &self, VFSlotRuntimeState &slot) {
    std::uint8_t phase;
    switch (self.actor->guardState) {
    case 0:
    case 3:
        phase = 0;
        break;
    case 1:
        phase = 1;
        break;
    case 2:
    case 5:
    case 6:
        phase = 2;
        break;
    default:
        return -1;
    }
    if ((self.savedMotionFlags & 0x8000) && slot.randomBytes[0] <= 128)
        return 5;
    return PhaseSelected(slot.phases[phase]) ? 5 : -1;
}
std::int8_t HitPhase(VFActor &self, VFSlotRuntimeState &slot) {
    const auto &other = *g_pFightMain->actors[self.actionIndex];
    if ((static_cast<std::uint16_t>(other.parameter2d0) > 4 &&
            other.parameter2d0 - static_cast<std::int16_t>(static_cast<std::int32_t>(other.animationTime)) > 4) ||
        !PhaseSelected(slot.phases[4]))
        return -1;
    return 22;
}
std::int8_t AttackChance(VFActor &self, VFSlotRuntimeState &slot) {
    if (!(self.reactionFlags & 1) || (self.hitStateFlags & 0x1d110) || (self.actor->hitStateFlags & 0x1d118) || slot.hitTimer > 0)
        return -1;
    if (static_cast<std::int32_t>(slot.opponentMotionTimer) > 128 && slot.parameter31)
        return 6;
    return Chance(static_cast<std::uint8_t>(slot.parameter31), slot.randomBytes[0]) ? 6 : -1;
}
std::int8_t PairGuard(VFActor &self, VFSlotRuntimeState &slot) {
    auto &other = *self.actor;
    if ((self.motionFlags & 0x80000000) && (other.motionFlags & 0x100) && other.impulseMagnitude > self.contactDistances[self.actionIndex] &&
        other.actionIndex == self.groupIndex)
        return 26;
    if (!(other.hitStateFlags & 0x10000100) || (other.hitStateFlags & 4) || (self.hitStateFlags & 0x1000) || !(other.hitStateFlags & 0x100) ||
        self.contactDistances[self.actionIndex] > 2 || !PhaseSelected(slot.phases[3]))
        return -1;
    return 26;
}
constexpr std::int16_t sectorAngles[16]{6656, 8192, 9472, 12288, 12288, 13056, 13056, 13824,
    13824,14592,14592,16896,16896,17664,17664,17664};
constexpr std::uint8_t sectorChances[16]{0, 0, 0, 8, 8, 8, 19, 38, 48, 72, 80, 88, 96, 104, 112, 120};



std::uint16_t moveTrainingGains[3][40]{};
constexpr std::uint8_t moveStarts[]{0, 0, 32, 64, 96};
constexpr std::int8_t moveCounts[]{0, 28, 23, 22};
std::uint8_t MoveCategory(std::uint8_t move) {
    std::uint8_t category = 1;
    while (move >= moveStarts[category + 1]) {
        if (++category >= 4)
            break;
    }
    return category;
}



template <std::uint8_t Index> std::int8_t UpdatePredicate(VFActor *actor, VFSlotRuntimeState *runtime) {
    if constexpr (Index == 0)
        return 0;
    if constexpr (Index == 40 || Index == 41 || Index == 44 || Index == 50 || Index == 51 || Index == 54)
        return vfUpdatePredicate_40_41_44_50_51_54();
    auto &s = *actor;
    auto &r = *runtime;
    auto &o = *s.actor;
    const auto d = s.contactDistances[s.actionIndex];
    if constexpr (Index == 1) {
        if (r.guardTimer > 0 || (s.groundContactFlags & 1) || (s.hitStateFlags & 0xdd010) || (o.hitStateFlags & 0xdd010) || d > 3)
            return -1;
        return Chance(static_cast<std::uint8_t>(r.parameter2a), r.randomBytes[0]) ? 1 : -1;
    }
    if constexpr (Index == 2) {
        const float previous = s.assistHistory.currentWindow;
        s.assistHistory.currentWindow = 0.0f;
        if ((s.hitStateFlags & 0x1d010) || o.pairContactPriority || ((s.secondaryMotionFlags & 0x8000000) && (s.motionFlags & 4)))
            return -1;
        if ((previous == 0 ? r.assistWindowCurrent + 0.5f : r.assistWindowCurrent - 0.30000001f) > d)
            return -1;
        if ((s.motionFlags & 0x80000000) && (!(o.motionFlags & 0x100) || !(o.secondaryMotionFlags & 0x80000))) {
            if (o.actionIndex == s.groupIndex)
                return -1;
            for (std::uint8_t i = 0; i < 6; ++i) {
                const auto *a = s.game->actors[i];
                if (s.game->characters[i] && a->groupIndex != o.groupIndex && a->groupIndex != s.groupIndex && a->hp > a->initialHp * 0.75f)
                    return -1;
            }
        }
        if (!s.reactionMovement.state.mode) {
            const bool distant = static_cast<double>(d) > static_cast<double>(r.assistWindowCurrent) + 2.0;
            auto mode = static_cast<std::uint8_t>((s.groundContactFlags & 1) || distant ? 2 : 1);
            if ((AuthoredByte(s, 25) & 2) || s.fightStyle == 1)
                mode = 1;
            s.reactionMovement.state.mode = mode;
        }
        s.assistHistory.currentWindow = r.assistWindowCurrent;
        return 2;
    }
    if constexpr (Index == 3 || Index == 29)
        return (s.groundContactFlags & 1) && !(s.hitStateFlags & 0x1d010) && !o.pairContactPriority ? Index : -1;
    if constexpr (Index == 4)
        return r.guardPhase || r.guardTicks > 90 || (r.guardTicks > 0.00000011920929f && !o.pairContactPriority) ? 4 : -1;
    if constexpr (Index == 5) {
        if (GuardProximity(s, r) == -1)
            return -1;
        return (AuthoredByte(s, 25) & 2) || GuardPhase(s, r) != -1 ? 5 : -1;
    }
    if constexpr (Index == 6 || Index == 30)
        return AttackChance(s, r);
    if constexpr (Index == 7) {
        if (!(s.hitStateFlags & 0x80000) || (r.cameraFlags & 0x4000) || !s.secondActor || !s.secondActor->pairContactPriority)
            return -1;
        const auto animation = s.secondActor->currentAnimation & 0xfff;
        if (r.parameter128 && (animation == 259 || animation == 223 || animation == 30))
            return 15;
        for (std::uint8_t i = 0; i < r.parameter128; ++i) {
            std::uint16_t entry;
            std::memcpy(&entry, r.reservedE8 + 2 * i, sizeof(entry));
            if (entry == animation)
                return 15;
        }
        if ((s.slot->parameter1a5 != 1 || !(s.savedSecondaryMotionFlags & 4)) && r.parameter128 < 32) {
            const auto value = static_cast<std::uint16_t>(animation);
            std::memcpy(r.reservedE8 + 2 * r.parameter128, &value, sizeof(value));
            ++r.parameter128;
            r.cameraFlags |= 0x4000;
        }
        return -1;
    }
    if constexpr (Index == 8) {
        for (std::uint8_t i = 0; i < 6; ++i)
            if (s.game->characters[i] && s.game->actors[i]->fightStyle == 12 && s.game->actorSlots[i]->hitTimer <= 0.00000011920929f)
                return 8;
        return -1;
    }
    if constexpr (Index == 9) {
        if (AttackChance(s, r) == -1)
            return -1;
        for (std::uint8_t i = 0; i < 6; ++i)
            if (s.game->characters[i] && s.game->actors[i]->fightStyle == 11 && !(std::fabs(s.game->actors[i]->hp) < 0.00000011920929f))
                return r.facingDifferences[i] > 0x4000 && s.contactDistances[i] < 2.5f ? 6 : -1;
        return -1;
    }
    if constexpr (Index == 10)
        return !(s.hitStateFlags & 0x10000000) ? 10 : -1;
    if constexpr (Index == 11)
        return (s.reactionFlags & 8) && Chance(static_cast<std::uint8_t>(r.parameter31), r.randomBytes[0]) ? 11 : -1;
    if constexpr (Index == 12)
        return !(o.hp > static_cast<double>(o.initialHp) * 0.25) && !(d > 1.25f) && !(s.hitStateFlags & 0x1c000) && !(o.hitStateFlags & 0x1d010) &&
                (o.hitStateFlags & 0x102) == 0x102 && !(s.groundContactFlags & 1) ?
            12 :
            -1;
    if constexpr (Index == 13)
        return !(o.animationTime > static_cast<std::uint16_t>(o.parameter2d0) && (o.hitStateFlags & 0x100)) && !(o.hitStateFlags & 0x1d014) && !(d > 2.25f) &&
                r.facingDifferences[s.actionIndex] <= r.parameter20 && Chance(std::to_integer<std::uint8_t>(r.reserved2b[4]), r.randomBytes[0]) ?
            13 :
            -1;
    if constexpr (Index == 14) {
        if (!((o.groundContactFlags & 1) || (o.activeMotionFlags & 4) ||
                ((o.motionFlags & 0x100) && (o.hitStateFlags & 4) && o.reactionEndFrame - o.animationTime > 5 * o.scale)) ||
            d > 2)
            return -1;
        const auto chance = std::min(255,static_cast<std::uint8_t>(r.parameter2a)+std::to_integer<std::uint8_t>(r.reserved2b[4]));
        return Chance(static_cast<std::uint8_t>(chance), r.randomBytes[0]) ? 1 : -1;
    }
    if constexpr (Index == 15)
        return (s.hitStateFlags & 0x80000) && s.secondActor && s.secondActor->pairContactPriority &&
                ((AuthoredByte(s, 25) & 2) || (PhaseSelected(r.phases[5]) && (s.slot->parameter1a5 != 1 || !(s.savedSecondaryMotionFlags & 4)))) ?
            15 :
            -1;
    if constexpr (Index == 16) {
        if (o.actionIndex == s.groupIndex && (o.hitStateFlags & 0x100)) {
            if (d > s.slot->assistWindowDefault && !(o.groundContactFlags & 1))
                return 16;
            const auto multiplier = static_cast<std::int8_t>(o.motionFlags) >= 0 && (o.previousHitStateFlags & 0x100) ? 2 : 1;
            const auto chance =
                std::min(255,std::to_integer<std::uint8_t>(s.slot->reserved2b[0])
                +std::to_integer<std::uint8_t>(s.slot->reserved2b[4])*multiplier);
            if (Chance(static_cast<std::uint8_t>(chance), s.slot->randomBytes[0]))
                return 16;
        }
        if (GuardProximity(s, *s.slot) != -1 && (!(r.cameraFlags & 0x400) || (g_pFightMain->actors[s.actionIndex]->currentAnimation & 0xfff) != r.parameterA0))
            GuardPhase(s, r);
        return -1;
    }
    if constexpr (Index == 17) {
        std::uint32_t frame;
        if (s.fightStyle == 9 && s.reactionRank)
            frame = s.reactionRank == 3 ? 300 : 0;
        else if (s.fightStyle == 31 && s.reactionRank == 2)
            frame = 150;
        else
            return -1;
        return static_cast<std::uint32_t>(static_cast<std::int32_t>(s.game->frame)) >= frame && (s.reactionFlags & 1) && !(s.hitStateFlags & 0x1d110) &&
                !(o.hitStateFlags & 0x1d118) ?
            17 :
            -1;
    }
    if constexpr (Index == 18) {
        if (GuardProximity(s, r) == -1)
            return -1;
        if ((r.cameraFlags & 0x400) && (g_pFightMain->actors[s.actionIndex]->currentAnimation & 0xfff) == r.parameterA0)
            return 5;
        return GuardPhase(s, r) != -1 ? 5 : -1;
    }
    if constexpr (Index == 19) {
        if (o.actionIndex != s.groupIndex)
            return -1;
        if (s.groundContactFlags & 1)
            return 29;
        if (!(o.motionFlags & 0x100) || ((o.hitStateFlags & 0x100) && d > o.impulseMagnitude + 0.5f))
            return -1;
        return GuardProximity(s, r) != -1 ? 5 : -1;
    }
    if constexpr (Index == 21)
        return (s.reactionFlags & 0x6000000) && Chance(static_cast<std::uint8_t>(r.parameter31), r.randomBytes[0]) ? 21 : -1;
    if constexpr (Index == 22 || Index == 35)
        return (s.hitFlags & 0x2400) && HitPhase(s, r) != -1 &&
                !((s.moveFlags & 0x40000) && (o.guardState == 3 || o.animationPhaseWord == 127 || !(o.activeMotionFlags & 0x10000000))) ?
            22 :
            -1;
    if constexpr (Index == 23)
        return (s.hitFlags & 0x5000) && HitPhase(s, r) != -1 ? 23 : -1;
    if constexpr (Index == 24)
        return (s.hitFlags & 0x8800) && HitPhase(s, r) != -1 ? 24 : -1;
    if constexpr (Index == 25) {
        const auto index = vfMotnNibbleGet(67);
        const auto angle = static_cast<std::int16_t>(s.facingAngle - s.contactAngles[s.actionIndex]);
        if ((s.secondaryMotionFlags & 0x10) || (s.motionFlags & 0x100e0040) || (s.secondaryMotionFlags & 0x200004))
            r.cameraFlags &= ~0x10000U;
        if (angle >= 24576 || angle <= -sectorAngles[index]) {
            if (Chance(sectorChances[index], r.randomBytes[1]))
                return 34;
            return !(r.secondaryMotionTimer > 0) && (r.cameraFlags & 0x10000) ? 25 : -1;
        }
        r.cameraFlags |= 0x10000;
        return -1;
    }
    if constexpr (Index == 26)
        return PairGuard(s, r);
    if constexpr (Index == 27)
        return !(s.moveFlags & 0x40000) &&
                !((o.hitStateFlags & 0x100) &&
                    static_cast<std::int16_t>(static_cast<std::int32_t>(o.animationTime)) < static_cast<std::uint16_t>(o.reactionMiddleFrame)) &&
                !(s.groundContactFlags & 1) && !(o.groundContactFlags & 1) && !(o.hitStateFlags & 0x1c000) && !(d > r.assistWindowCurrent - 1) ?
            27 :
            -1;
    if constexpr (Index == 28) {
        const auto time = static_cast<std::int16_t>(static_cast<std::int32_t>(o.animationTime));
        if (!(o.hitStateFlags & 0x100) && (o.activeMotionFlags & 0x4000)) {
            r.animationPhase = 0;
            return -1;
        }
        if (!(static_cast<double>(d) > 1.800000023841858) && (o.activeMotionFlags & 0x80000))
            return (o.secondaryMotionFlags & 0x80000) && time > static_cast<std::uint16_t>(o.reactionMiddleFrame) ? -1 : 28;
        if (static_cast<double>(d) > 1.800000023841858 && (o.activeMotionFlags & 0x80000))
            return (o.hitStateFlags & 0x4000) && PhaseSelected(r.phases[3]) ? 28 : -1;
        if (s.rightIntersectionDistance < 0.5f && s.leftIntersectionDistance < 0.5f)
            return -1;
        if (PairGuard(s, r) != -1) {
            if (static_cast<std::uint16_t>(o.temporaryFacingAngle - s.contactAngles[s.actionIndex] - 16380) > 0x7ff8)
                return -1;
            if (!(d > o.impulseMagnitude)) {
                if (r.animationPhase == 1)
                    return -1;
                float remaining = o.impulseMagnitude - (o.impulseMagnitude / static_cast<std::uint16_t>(o.parameter2d0)) * o.animationTime;
                if (remaining < 0)
                    remaining = 0;
                const float distance = d - remaining;
                if (!(o.impulseMagnitude > distance && distance > 1)) {
                    if (!(o.moveFlags & 1) && !(s.activeMotionFlags & 0x2000000) &&
                        o.parameter2d0 - time > ((o.guardState == 3 && (o.activeMotionFlags & 0x10000000)) ? 5 : 3))
                        return -1;
                    return 28;
                }
            }
            r.animationPhase = 1;
            return -1;
        }
        return d < 1 && !(o.groundContactFlags & 1) ? 28 : -1;
    }
    if constexpr (Index == 31)
        return (r.cameraFlags & 1) && r.facingDifferences[s.actionIndex] >= r.parameter22 &&
                static_cast<std::int16_t>(r.facingDifferences[s.actionIndex]) <= 24576 && !o.pairContactPriority && !(s.hitStateFlags & 0xdc000) &&
                !((o.hitStateFlags & 0x100) && static_cast<double>(d) < 2.7) ?
            31 :
            -1;
    if constexpr (Index == 32)
        return (s.hitStateFlags & 0x20000000) && !(static_cast<double>(d) > static_cast<double>(s.assistHistory.currentWindow) + 0.2) &&
                Chance(static_cast<std::uint8_t>(r.parameter2a), r.randomBytes[0]) ?
            32 :
            -1;
    if constexpr (Index == 33)
        return o.actionIndex == s.groupIndex && (o.hitStateFlags & 0x100) && !(o.motionFlags & 0x10000000) &&
                static_cast<std::uint16_t>(o.parameter2d0) >= o.animationTime && static_cast<std::uint16_t>(o.parameter2d0) - o.animationTime < 3 ?
            33 :
            -1;
    if constexpr (Index == 34) {
        if ((s.slot->motionTimer > 0 && !(o.motionFlags & 0x1c000)) || d > 4)
            return -1;
        if (o.actionIndex != s.groupIndex) {
            for (std::uint8_t i = 0; i < 6; ++i) {
                const auto *a = s.game->actors[i];
                if (s.game->characters[i] && a->groupIndex != o.groupIndex && a->groupIndex != s.groupIndex && a->initialHp * 0.75f > a->hp)
                    return -1;
            }
            if (s.scale > s.animationTime)
                return 34;
        }
        return !(o.motionFlags & 0x100) && !(o.secondaryMotionFlags & 0x80000) && !(s.motionFlags & 0x80000000) ? 34 : -1;
    }
    if constexpr (Index == 42)
        return !(s.hitStateFlags & 0x40000) && (o.hitStateFlags & 0x100) && !(s.activeMotionFlags & 0x2000000) && !(r.cameraFlags & 0x2000) &&
                static_cast<std::uint16_t>(static_cast<std::int32_t>(o.animationTime + 2)) >= static_cast<std::uint16_t>(o.parameter2d0) && !(d > 2.5f) ?
            5 :
            -1;
    if constexpr (Index == 45) {
        const auto authored = AuthoredByte(s, 110);
        if (authored == 3 && static_cast<std::int32_t>(s.game->frame) > 300) {
            const auto &slot = *s.slot;
            if (!((s.secondaryMotionFlags & 2) || ((s.reactionFlags & 1) && !(s.hitStateFlags & 0x1d110) && !(o.hitStateFlags & 0x1d118))) || slot.hitTimer > 0)
                return -1;
            if (static_cast<std::int32_t>(slot.opponentMotionTimer) > 128 && slot.parameter31)
                return 6;
            return Chance(static_cast<std::uint8_t>(slot.parameter31), slot.randomBytes[0]) ? 6 : -1;
        }
        return authored == 4 && static_cast<std::int8_t>(s.reactionRank) <= 0 ? 45 : -1;
    }
    if constexpr (Index == 53)
        return !(s.groundContactFlags & 1) && !o.pairContactPriority && !(s.hitStateFlags & 0x2001d010) && !(s.activeMotionFlags & 0x8000000) &&
                g_pFightStage->unknownGroupCounts[g_pFightMain->enemySlots[s.groupIndex]->counterGroup] >= 2 && !(o.hitStateFlags & 0x20000000) &&
                !(d > r.assistWindowCurrent - 0.2f) && !(r.assistWindowCurrent - 0.8f > d) ?
            53 :
            -1;
    if constexpr (Index == 55) {
        if (!(g_pFightStage->battleFlags & 0x20) && !(s.game->mode & 0x2000)) {
            for (std::uint8_t i = 0; i < 6; ++i)
                if (g_pFightMain->characters[i] && i != s.groupIndex && g_pFightMain->slotTeams[i] == g_pFightMain->slotTeams[s.groupIndex] &&
                    (g_pFightMain->actors[i]->motionFlags & 0x100))
                    return 0;
            if (g_pFightMain->playerSlot != -1 && s.groupIndex != g_pFightMain->actors[g_pFightMain->playerSlot]->pairedSlot)
                r.parameter2a = static_cast<std::uint8_t>(r.parameter2a) >> 2;
        }
        return -1;
    }
    if constexpr (Index == 57)
        return !(d > s.game->contactRange) && !(r.assistWindowCurrent - 1 > d) ? 57 : -1;
}

constexpr std::uint32_t moveCategories[44]{0, 8, 64, 16, 128, 48, 384, 2048, 16384, 6144, 49152, 6144, 49152,
    432,7168,57344,2,4,514,6,514,55296,24,192,26,194,56,448,55296,18432,6192,49536,66,10,
    9730,76,20480,34816,55296,432,504,0xffffffffU,65026,1022};
std::uint8_t HitReactionCategory(const VFActor &actor) {
    if (!(actor.hitStateFlags & 0x100))
        return 0;
    const auto mask = moveCategories[actor.moveId];
    return ((mask & 0x1c0) ? 4 : 0) | ((mask & 0x38) ? 8 : 0) | ((mask & 0xe000) ? 16 : 0) | ((mask & 0x1c00) ? 32 : 0) | ((mask & 3) ? 2 : 0) |
        ((mask & 4) ? 1 : 0);
}
void ResetPhase(VFSlotRuntimeState &slot) {
    slot.parameter89 = 0;
    slot.parameter8c = 0;
    slot.parameterCc = 0;
    slot.parameter12a = 0;
    slot.resetIndex = 0x7f7f;
    slot.cameraFlags = (slot.cameraFlags & ~0x2001U) | 1;
}
std::uint8_t RandomAuthoredChoice(VFActor &self, std::size_t offset, float count) {
    return AuthoredByte(self, offset + static_cast<std::uint8_t>(static_cast<std::int32_t>(IncrementSharedPrngFloat() * count)));
}
void SelectPhase(VFActor &self, std::uint8_t choice, std::uint16_t delay = 0) {
    vfResetAndSelectCameraPhaseChoice(&self, choice, delay);
}
constexpr std::uint8_t nearbyChoices[4]{201, 202, 201, 202};
constexpr std::uint8_t retreatChoices[4]{204, 204, 201, 202};
constexpr std::uint8_t guardChoices[3]{219, 220, 221};
constexpr std::uint8_t hitChoices[8]{204, 204, 201, 202, 202, 201, 217, 217};
constexpr std::uint16_t cameraSectors[8]{0x20, 0xa0, 0x80, 0x90, 0x10, 0x50, 0x40, 0x60};
std::uint8_t RandomIndex(float count) {
    return static_cast<std::uint8_t>(static_cast<std::int32_t>(IncrementSharedPrngFloat() * count));
}
void PostDefaultAttack(VFActor &self, VFSlotRuntimeState &slot) {
    constexpr float rankSpeeds[4]{0.0055555556900799274f, 0.0027777778450399637f, 0.0033333334140479565f, 0.0023809524718672037f};
    slot.randomBytes[0] = static_cast<std::uint8_t>(IncrementSharedPrngUInt16());
    slot.randomBytes[1] = static_cast<std::uint8_t>(IncrementSharedPrngUInt16());
    auto &other = *self.actor;
    const auto setting = vfMotnNibbleGet(66);
    const auto frame = static_cast<std::int32_t>(self.game->frame);
    auto delay = static_cast<std::uint16_t>(
        frame >= 150 ? static_cast<std::int32_t>(self.game->frame * (setting > 2 ? 0.0023809525f : rankSpeeds[self.reactionRank])) : 0);
    if (static_cast<double>(self.contactDistances[other.groupIndex]) > static_cast<double>(other.frontContactRange) + 1) {
        if (frame % 64 == 0 && static_cast<std::int32_t>(self.game->previousFrame) % 64 != 0)
            SelectPhase(self, 202 - ((IncrementSharedPrngUInt16() & 1) != 0), delay);
        return;
    }
    if (delay >= 2) {
        if (!(self.hitStateFlags & 0x200))
            return;
        delay -= 2;
        if (AttackChance(self, slot) != -1) {
            SelectPhase(self, RandomAuthoredChoice(self, 81, 4));
            return;
        }
    } else if (setting > 2) {
        const auto seconds = static_cast<std::int32_t>(self.game->guardTimer) / 30;
        const bool pause = setting < 10 ? seconds % (17 - setting) == 0 : setting < 14 ? seconds % (setting / 2) == 0 : seconds % (setting + 18) < 2;
        if (pause)
            self.activeMotionFlags &= ~0x20U;
        else
            other.activeMotionFlags |= 0x20;
        const auto chance = static_cast<std::uint8_t>(19 * (setting - 2));
        if (Chance(chance, slot.randomBytes[1])) {
            SelectPhase(self, static_cast<std::uint8_t>((slot.randomBytes[0] > 127) + 2), delay);
            return;
        }
    }
    SelectPhase(self, RandomIndex(static_cast<float>(AuthoredByte(self, 24))), delay);
}

template <std::uint8_t Index> void PostCallback(VFActor *actor, VFSlotRuntimeState *runtime) {
    if constexpr (Index == 0)
        if (!actor)
            return;
    if constexpr (Index == 40 || Index == 41 || Index == 44 || Index == 50 || Index == 51 || Index == 54)
        return;
    auto &s = *actor;
    auto &r = *runtime;
    if constexpr (Index == 0) {
        if ((static_cast<std::int32_t>(r.ticks) & 15) == 0 && (static_cast<std::int32_t>(r.previousTicks) & 15) != 0) {
            const auto random = static_cast<std::uint8_t>(IncrementSharedPrngUInt16());
            ++r.parameter129;
            r.randomBytes[0] = random;
        }
    }
    if constexpr (Index == 1) {
        if (r.randomBytes[1] & 0x70) {
            const auto offset = 53 + (((s.fightStyle == 10 || s.fightStyle == 11) && s.reactionRank) ? 8 : 0);
            SelectPhase(s, RandomAuthoredChoice(s, offset, 8));
        } else
            SelectPhase(s, RandomIndex(static_cast<float>(AuthoredByte(s, 24))));
    }
    if constexpr (Index == 2) {
        const auto &other = *g_pFightMain->actors[static_cast<std::int8_t>(r.opponentIndex)];
        s.cameraControlFlags = 0x2000;
        if (s.reactionMovement.state.mode == 1 && (s.contactDistances[r.opponentIndex] > 5 || (other.hitStateFlags & 0x20000000)))
            s.reactionMovement.state.mode = static_cast<std::uint8_t>((AuthoredByte(s, 25) & 2) || s.fightStyle == 1 ? 1 : 2);
        s.targetCache = -1;
        s.reactionMovement.state.phase = std::uint8_t{0};
        s.parameterA78 = other.position.x;
        s.parameterA80 = std::bit_cast<std::uint32_t>(other.position.z);
    }
    if constexpr (Index == 3) {
        SelectPhase(s,
            s.contactDistances[s.actionIndex] < 2 && Chance(static_cast<std::uint8_t>(r.parameter2a), r.randomBytes[1]) ? RandomAuthoredChoice(s, 77, 4) :
                                                                                                                          nearbyChoices[RandomIndex(4)]);
    }
    if constexpr (Index == 4)
        s.cameraControlFlags = (static_cast<std::int32_t>(g_pFightMain->frame) & 2) && !(static_cast<std::int32_t>(g_pFightMain->previousFrame) & 2) ? 32 : 16;
    if constexpr (Index == 5) {
        ResetPhase(*s.slot);
        vfResetReactionProgram(&s);
        SelectPhase(s, 217);
    }
    if constexpr (Index == 6 || Index == 30)
        SelectPhase(s, RandomAuthoredChoice(s, static_cast<std::int32_t>(s.game->frame) < 600 && AuthoredByte(s, 110) == 3 ? 83 : 81, 4), r.parameter28);
    if constexpr (Index == 8) {
        for (std::uint8_t i = 0; i < 6; ++i)
            if (s.game->characters[i] && s.game->actors[i]->fightStyle == 12) {
                const auto &other = *s.game->actors[i];
                const auto angle = static_cast<std::uint16_t>(-other.contactAngles[s.game->playerSlot]);
                const auto sine = sin_fixed16(static_cast<std::int16_t>(angle)), cosine = cos_fixed16(angle);
                Vector3 point{-cosine - sine, 0, -sine + cosine};
                point.x += other.position.x;
                point.y += other.position.y;
                point.z += other.position.z;
                const auto x = s.position.x - point.x, z = s.position.z - point.z;
                if (std::sqrt(x * x + z * z) > 1) {
                    s.parameterA78 = point.x;
                    s.parameterA80 = std::bit_cast<std::uint32_t>(point.z);
                    s.cameraControlFlags = 0x2000;
                    s.reactionMovement.state.mode = std::uint8_t{2};
                }
            }
    }
    if constexpr (Index == 10)
        PostDefaultAttack(s, r);
    if constexpr (Index == 11)
        SelectPhase(s, RandomAuthoredChoice(s, 85, 2), r.parameter28);
    if constexpr (Index == 12) {
        const auto &other = *g_pFightMain->actors[s.actionIndex];
        if (other.parameter2d0 - static_cast<std::int16_t>(static_cast<std::int32_t>(other.animationTime)) < 5)
            SelectPhase(s, RandomAuthoredChoice(s, 97, 1));
    }
    if constexpr (Index == 13) {
        const auto &other = *g_pFightMain->actors[s.actionIndex];
        if ((other.hitStateFlags & 0x200) || (other.activeMotionFlags & 0x80000) ||
            other.parameter2d0 - static_cast<std::int16_t>(static_cast<std::int32_t>(other.animationTime)) >= 5)
            SelectPhase(s, RandomAuthoredChoice(s, static_cast<double>(s.contactDistances[s.actionIndex]) > 2.2 ? 104 : 102, 2));
    }
    if constexpr (Index == 15) {
        ResetPhase(*s.slot);
        s.cameraControlFlags = r.reactionTicks > 0.00000011920929f && static_cast<std::uint16_t>(static_cast<std::int32_t>(r.reactionTicks) - 1) <= 1 ? 4 : 0;
    }
    if constexpr (Index == 16) {
        if (!(s.activeMotionFlags & 0x2000000)) {
            ResetPhase(*s.slot);
            vfResetReactionProgram(&s);
        }
        if ((static_cast<std::int32_t>(s.game->frame) & 1) && !(static_cast<std::int32_t>(s.game->previousFrame) & 1))
            SelectPhase(s, s.actor->guardState == 3 || s.actor->guardState == 0 || (s.activeMotionFlags & 0x2000000) ? guardChoices[RandomIndex(3)] : 217);
    }
    if constexpr (Index == 17 || Index == 25)
        SelectPhase(s, 209);
    if constexpr (Index == 21)
        SelectPhase(s, RandomAuthoredChoice(s, 87, 2), r.parameter28);
    if constexpr (Index == 22 || Index == 35 || Index == 23 || Index == 24) {
        if (Index == 24 ? (s.moveFlags & 0x100) != 0 : !(s.moveFlags & 0x100)) {
            SelectPhase(s, Index == 23 ? 206 : Index == 24 ? 207 : 205);
            r.cameraFlags |= 0x2000;
        }
    }
    if constexpr (Index == 26) {
        const auto &other = *g_pFightMain->actors[s.actionIndex];
        ResetPhase(*s.slot);
        if (!(static_cast<std::int32_t>(s.game->frame) & 1)) {
            s.cameraControlFlags = 0;
            return;
        }
        std::uint16_t code = 24;
        const auto direction = static_cast<std::int16_t>(other.hitDirection);
        if (direction > 0)
            code = s.leftIntersectionDistance > 3 ? 136 : 72;
        else if (direction < 0)
            code = !(s.rightIntersectionDistance > 3) ? 136 : 72;
        s.cameraControlFlags = other.eventFrame30a &&
                static_cast<std::int16_t>(other.eventFrame30a) - static_cast<std::int16_t>(static_cast<std::int32_t>(other.animationTime)) < 4 ?
            24 :
            code;
    }
    if constexpr (Index == 27)
        SelectPhase(s, retreatChoices[RandomIndex(4)]);
    if constexpr (Index == 28) {
        const auto &other = *s.actor;
        const auto category = HitReactionCategory(other);
        std::uint8_t index;
        if ((other.activeMotionFlags & 0x10000000) && other.guardState == 3)
            index = 6;
        else if ((other.activeMotionFlags & 0x10000000) && (other.guardState == 2 || static_cast<std::uint8_t>(other.guardState - 5) <= 1))
            index = 0;
        else if (category & 0x24)
            index = 2;
        else if (category & 0x18)
            index = 4;
        else
            index = (other.activeMotionFlags & 0x100) ? 2 : 0;
        const std::uint32_t direction = other.hitDirection;
        if (direction - 1821 <= 0xf1c5 && (((category & 0x24) && direction > 0x8000) || ((category & 0x18) && direction < 0x8000)))
            ++index;
        SelectPhase(s, hitChoices[index]);
    }
    if constexpr (Index == 29)
        SelectPhase(s, nearbyChoices[RandomIndex(4)]);
    if constexpr (Index == 31) {
        ResetPhase(*s.slot);
        s.cameraControlFlags = 32;
    }
    if constexpr (Index == 32) {
        ResetPhase(*s.slot);
        SelectPhase(s, RandomAuthoredChoice(s, 93, 4));
    }
    if constexpr (Index == 33)
        SelectPhase(s, 217);
    if constexpr (Index == 34)
        SelectPhase(s, 218);
    if constexpr (Index == 45) {
        if (AuthoredByte(s, 110) == 4 && s.hp / static_cast<float>(s.initialHp) < 0.5f)
            SelectPhase(s, 13);
    }
    if constexpr (Index == 53) {
        auto *bytes = reinterpret_cast<std::uint8_t *>(g_pFightMain);
        const auto target = bytes[0x29e + s.groupIndex];
        if ((static_cast<std::int32_t>(r.reactionTicks) & 31) == 0) {
            bytes[0x2af + target] = (IncrementSharedPrngUInt16() & 1) ? 1 : 2;
            if (s.actor->activeMotionFlags & 0x800)
                bytes[0x2af + target] = (static_cast<std::int16_t>(s.actor->facingAngle) - s.actor->contactAngles[target] >= 0) + 1;
        }
        s.cameraControlFlags = bytes[0x2af + target] == 2 ? 4160 : 4224;
    }
    if constexpr (Index == 57) {
        const auto ticks = static_cast<std::int32_t>(r.ticks), previous = static_cast<std::int32_t>(r.previousTicks);
        if ((ticks & 32) || !(previous & 32)) {
            s.cameraControlFlags = 0;
            return;
        }
        const auto distance = s.contactDistances[s.actionIndex];
        if (!(ticks & 63) && (previous & 63))
            r.reserved12b = distance > 3 ? 0 : r.assistWindowCurrent - 0.69999999f > distance ? 4 : IncrementSharedPrngUInt16() & 7;
        s.cameraControlFlags = cameraSectors[r.reserved12b];
    }
}

struct ReactionCallbacks {
    std::int8_t (*update)(VFActor *, VFSlotRuntimeState *);
    void (*post)(VFActor *, VFSlotRuntimeState *);
};
constexpr ReactionCallbacks reactionCallbacks[58]{{UpdatePredicate<0>, PostCallback<0>}, {UpdatePredicate<1>, PostCallback<1>},
    {UpdatePredicate<2>, PostCallback<2>}, {UpdatePredicate<3>, PostCallback<3>}, {UpdatePredicate<4>, PostCallback<4>}, {UpdatePredicate<5>, PostCallback<5>},
    {UpdatePredicate<6>, PostCallback<6>}, {UpdatePredicate<7>, nullptr}, {UpdatePredicate<8>, PostCallback<8>}, {UpdatePredicate<9>, nullptr},
    {UpdatePredicate<10>, PostCallback<10>}, {UpdatePredicate<11>, PostCallback<11>}, {UpdatePredicate<12>, PostCallback<12>},
    {UpdatePredicate<13>, PostCallback<13>}, {UpdatePredicate<14>, nullptr}, {UpdatePredicate<15>, PostCallback<15>}, {UpdatePredicate<16>, PostCallback<16>},
    {UpdatePredicate<17>, PostCallback<17>}, {UpdatePredicate<18>, nullptr}, {UpdatePredicate<19>, nullptr}, {nullptr, nullptr},
    {UpdatePredicate<21>, PostCallback<21>}, {UpdatePredicate<22>, PostCallback<22>}, {UpdatePredicate<23>, PostCallback<23>},
    {UpdatePredicate<24>, PostCallback<24>}, {UpdatePredicate<25>, PostCallback<25>}, {UpdatePredicate<26>, PostCallback<26>},
    {UpdatePredicate<27>, PostCallback<27>}, {UpdatePredicate<28>, PostCallback<28>}, {UpdatePredicate<29>, PostCallback<29>},
    {UpdatePredicate<6>, PostCallback<6>}, {UpdatePredicate<31>, PostCallback<31>}, {UpdatePredicate<32>, PostCallback<32>},
    {UpdatePredicate<33>, PostCallback<33>}, {UpdatePredicate<34>, PostCallback<34>}, {UpdatePredicate<22>, PostCallback<22>}, {nullptr, nullptr},
    {nullptr, nullptr}, {nullptr, nullptr}, {nullptr, nullptr}, {UpdatePredicate<40>, PostCallback<40>}, {UpdatePredicate<40>, PostCallback<40>},
    {UpdatePredicate<42>, nullptr}, {nullptr, nullptr}, {UpdatePredicate<40>, PostCallback<40>}, {UpdatePredicate<45>, PostCallback<45>}, {nullptr, nullptr},
    {nullptr, nullptr}, {nullptr, nullptr}, {nullptr, nullptr}, {UpdatePredicate<40>, PostCallback<40>}, {UpdatePredicate<40>, PostCallback<40>},
    {nullptr, nullptr}, {UpdatePredicate<53>, PostCallback<53>}, {UpdatePredicate<40>, nullptr}, {UpdatePredicate<55>, nullptr}, {nullptr, nullptr},
    {UpdatePredicate<57>, PostCallback<57>}};
}

// The saved continuation and current program remain actor-owned.
// @verified - p
void vfResetReactionProgram(VFActor *self) {
    self->reactionPc = reinterpret_cast<const std::uint8_t *>(self->motionParameters[0]);
    self->reactionContinuation = nullptr;
    self->reactionDelay.count.remaining = std::uint16_t{0};
    self->reactionDelayActive = std::uint8_t{0};
}


// @verified - p
void vfResetActorActionState(VFActor *self) {
    auto &actor = *self;
    actor.moveFlags |= 0x4000000;
    std::memset(&actor.pairContactMiddleFrame, 0, sizeof(actor.pairContactMiddleFrame) + sizeof(actor.pairContactLastFrame));
    actor.activeContactCount = std::uint16_t{0};
    actor.reactionOverrideFlags.words[0] = std::uint16_t{0};
    actor.reactionOverrideFlags.words[1] = std::uint16_t{0};
    actor.reactionMaskA.words[0] = std::uint16_t{0};
    actor.reactionMaskA.words[1] = std::uint16_t{0};
    actor.reactionContinuation = nullptr;
    actor.savedRootReaction.words[0] = std::uint16_t{0};
    actor.savedRootReaction.words[1] = std::uint16_t{0};
    actor.savedRootReaction.words[2] = std::uint16_t{0};
    actor.savedRootReaction.words[3] = std::uint16_t{0};
    actor.deferredPriorityActive = std::uint16_t{0};
    actor.reactionDelay.count.remaining = std::uint16_t{0};
    actor.reactionDelay.count.allowed = std::uint16_t{0};
    actor.reactionDelayActive = std::uint8_t{0};
    actor.reactionMaskB.bits = std::uint32_t{0};
    actor.reactionCursors.endpoints = std::uint16_t{0x101};
    actor.reactionHistory[0].mask = actor.cameraControlFlags;
    actor.reactionHistory[0].sectorMask = actor.cameraControlFlags;
    actor.reactions.pressed = actor.followupFlags;
    const std::uint8_t head = actor.reactionCursors.endpoint[1];
    actor.reactionCursors.tracked[0] = head;
    actor.reactionCursors.tracked[1] = head;
    actor.reactionCursors.tracked[2] = head;
}


// @verified - p
void vfUpdateMain(VFActor *self) {
    g_pFightMain = self->game;
    const auto index = self->actionIndex;
    auto &slot = *g_pFightMain->actorSlots[self->groupIndex];
    self->previousReactionInput = self->cameraControlFlags;
    self->cameraControlFlags = 0;
    if (index == -1 || g_pFightMain->slotPoolModes[self->groupIndex] > 1)
        return;
    auto &other = *g_pFightMain->actors[index];
    vfUpdateSlotRuntimeMetricsAndTimers(self);
    vfUpdateSlotReactionHistory(self);
    vfUpdateSlotRankParameters(self);
    vfUpdateScenarioHPPhaseConfig(self);
    vfUpdateCurrentAssistWindow(self);
    if (self->hitStateFlags & 0xa4000) {
        ResetPhase(*self->slot);
        slot.parameterAe = 0;
    }
    if ((other.hitStateFlags & 0x14000) == 0x10000) {
        slot.resetIndex = 0x7f7f;
        slot.parameterAe = 0;
        slot.parameter12a = 0;
    }
    if ((slot.cameraFlags & 0x4000) && !(self->hitStateFlags & 0x80000))
        slot.cameraFlags &= ~0x4000U;
    slot.opponentIndex = index;
    if (other.scale > other.animationTime)
        slot.animationPhase = 0;


    constexpr std::uint8_t defaultProgram[]{0};
    constexpr std::uint8_t sparringProgram[]{42, 0};
    const auto *program = g_pFightMain->type == 2 ? sparringProgram : defaultProgram;
    if (g_pFightMain->slotPoolModes[self->groupIndex] || (g_pFightMain->gameFlags & 0x400000))
        program = reinterpret_cast<const std::uint8_t *>(self->motionParameters[2]) + 111;
    std::int8_t selected;
    do {
        selected = reactionCallbacks[*program++].update(self, &slot);
    } while (selected == -1);
    slot.reactionIndex = selected;
    if (slot.previousReactionIndex != selected) {
        slot.previousReactionIndex = selected;
        slot.reactionTicks = 0;
    }
    if (!(slot.secondaryMotionTimer >= 0)) {
        if (self->secondaryMotionFlags & 0x10)
            self->slot->secondaryMotionTimer = 60;
    } else {
        slot.secondaryMotionTimer -= shenGetScaledDeltaTickTime(1.0f);
        if (slot.secondaryMotionTimer < 0)
            slot.secondaryMotionTimer = 0;
    }
    if (other.hp <= 0 || static_cast<std::int8_t>(other.stateFlags) < 0 || (other.moveFlags & 0x20))
        ResetPhase(*self->slot);
    else
        reactionCallbacks[selected].post(self, &slot);
    if (!(slot.cameraFlags & 1))
        vfAdvanceCameraPhaseChain(self);
}

// Reset and consume the actual combat-produced gain banks.
// @verified - p
void vfResetMoveTrainingGains() {
    std::memset(moveTrainingGains, 0, sizeof(moveTrainingGains));
}
// @verified - p
void vfConsumeMoveTrainingGains() {
    for (std::uint8_t category = 1; category < 4; ++category) {
        for (std::uint8_t index = 0; index < 40; ++index) {
            const auto old = static_cast<std::int16_t>(GetMoveProficiency(category, index));
            const auto gain = moveTrainingGains[category - 1][index];
            const auto increment = category == 3 || (category == 1 && index == 18) ? static_cast<std::int16_t>(static_cast<std::uint16_t>(gain * 3)) :
                                                                                     static_cast<std::int16_t>(static_cast<std::int16_t>(gain) / 2);
            auto proficiency = static_cast<std::int16_t>(static_cast<std::uint16_t>(old + increment));
            if (proficiency >= 200)
                proficiency = 199;
            S2Save_SetMoveProficiency(category, index, proficiency);
            moveTrainingGains[category - 1][index] = 0;
            if (old < 12 && proficiency >= 12 && index < moveCounts[category]) {
                SetMoveEquipment(0, category, index);
                S2Save_SetMoveProficiency(category, index, proficiency);
            }
        }
    }
}

// Only committed player actions increment these wrapping word counters.
// @verified - p
void vfAccumulateMoveTrainingGain(std::uint8_t move) {
    const auto category = MoveCategory(move);
    if (category >= 4)
        return;
    const auto index = static_cast<std::uint8_t>(move - moveStarts[category]);
    if (index < static_cast<std::uint8_t>(moveCounts[category]) && index != 0xff && index < 40 &&
        static_cast<std::int16_t>(GetMoveProficiency(category, index)) > 0)
        ++moveTrainingGains[category - 1][index];
}

// @verified - p
std::int8_t vfDecodeMoveCategoryAndIndex(std::uint8_t move, std::int8_t *destination) noexcept {
    destination[0] = 0;
    destination[1] = -1;
    std::int8_t category = 1;
    while (move >= moveStarts[category + 1])
        if (++category >= 4)
            return category;
    destination[0] = category;
    const std::int8_t index = static_cast<std::int8_t>(move - moveStarts[category]);
    destination[1] = index;
    const std::int8_t result = index < moveCounts[category] ? index : -1;
    destination[1] = result;
    return result;
}

// Authored action admission learns an unlearned catalog move immediately.
// @verified - p
void vfLearnCommittedMove(VFMA_t *game, std::uint8_t move) {
    const auto category = MoveCategory(move);
    if (category >= 4)
        return;
    const auto index = static_cast<std::int8_t>(move - moveStarts[category]);
    if (index >= moveCounts[category] || index == -1 || static_cast<std::int32_t>(GetMoveProficiency(category, static_cast<std::uint8_t>(index))) >= 12 ||
        (game->mode & 0xc103b))
        return;
    SetMoveProficiency(0, category, index, 12);
    if (!GetInternalFlag(1014) && SetNewEwzoTypeForMoveLearned(0, category, index)) {
        SetHelpIconMenuFlags(2);
        game->mode |= 0x10;
        nullsub();
        game->timeParameter = 0;
    }
}


// @verified - p
void vfmaUpdateCollisionCounter(VFMA_t *game, VFCollisionCounterEvent event, std::uint16_t move) {
    auto *contacts = game->collisionHistory + 2 * move;
    auto *hits = game->collisionHistory + 0x100 + 2 * move;
    std::uint16_t contact, hit;
    std::memcpy(&contact, contacts, sizeof(contact));
    std::memcpy(&hit, hits, sizeof(hit));
    if (event == VFCollisionCounterEvent::ContactRegistered) {
        if (contact != 0xffff) {
            ++contact;
            std::memcpy(contacts, &contact, sizeof(contact));
        }
        game->mode &= ~0x440U;
        if (move != 96)
            game->counter32c = (game->counter32c & 0xffffU) | (static_cast<std::uint32_t>(static_cast<std::uint16_t>((game->counter32c >> 16) + 1)) << 16);
    } else if (event == VFCollisionCounterEvent::HitConfirmed) {
        if (hit != 0xffff && !(game->mode & 0x40)) {
            ++hit;
            std::memcpy(hits, &hit, sizeof(hit));
            game->mode |= 0x40;
        }
    } else if (event == VFCollisionCounterEvent::HitSequenceEnded && contact && hit && !(game->mode & 0x400)) {
        --hit;
        std::memcpy(hits, &hit, sizeof(hit));
        game->mode |= 0x400;
    }
}

// The reaction VM's real action-commit callback and gain producer.
// @verified - p
std::uint8_t vfReactCommitActionAndResetSlots(VFActor *self) {
    auto &game = *self->game;
    const std::uint8_t *program = self->reactionPc;
    const auto bits = (static_cast<std::uint32_t>(program[4]) << 24) | (static_cast<std::uint32_t>(program[5]) << 16) |
        (static_cast<std::uint32_t>(program[6]) << 8) | program[7];
    self->action = static_cast<std::uint64_t>(static_cast<std::int64_t>(std::bit_cast<std::int32_t>(bits)));
    const std::uint8_t move = self->reactionVariantBytes[3];
    const auto state = static_cast<std::uint8_t>(self->action >> 24);
    const bool player = self->fightStyle == 0 && (state != 8 || game.hitStopFrames == 0);
    if (player) {
        vfAccumulateMoveTrainingGain(move);
        vfmaUpdateCollisionCounter(&game, VFCollisionCounterEvent::ContactRegistered, move);
        vfLearnCommittedMove(&game, move);
        if (move != 103) {
            self->packedHitImpulse = (self->packedHitImpulse & ~0xffU) | move;
            std::int8_t categoryAndIndex[8];
            (void)vfDecodeMoveCategoryAndIndex(move, categoryAndIndex);
        }
        if (move < 96)
            self->packedHitImpulse = (self->packedHitImpulse & ~0xff00U) | (static_cast<std::uint32_t>(move) << 8);
    }
    const auto motion = static_cast<std::uint16_t>(self->action) & 0xfff;
    if (state == 8) {
        const auto marker = FindLoadedMotionSecondaryEntry(motion, 15);
        if (!marker.empty()) {
            std::uint16_t flags;
            std::memcpy(&flags, marker.data() + 4, sizeof(flags));
            if (flags)
                self->stateFlags |= 0x2000;
        }
        if (player)
            vfmaUpdateCollisionCounter(&game, VFCollisionCounterEvent::HitConfirmed, move);
        if (self->formationFlags & 0x400) {
            if (self->formationHpFloor == move)
                self->formationRuntimeFlags |= 0x400;
            else if (static_cast<std::int16_t>(self->spawnYaw[0]) == move)
                self->formationRuntimeFlags |= 0x800;
        }
        for (std::size_t i = 0; i < 6; ++i)
            if (game.characters[i])
                game.actors[i]->reactionFlags &= 0x300;
    } else if (!FindLoadedMotionSecondaryEntry(motion, 4).empty())
        self->stateFlags |= 0x1000;
    self->reactionDelay.count.remaining = std::uint16_t{0};
    self->reactionDelayActive = std::uint8_t{0};
    const std::uint8_t tail = self->reactionCursors.endpoint[1];
    self->reactionCursors.tracked[0] = tail;
    self->reactionCursors.tracked[1] = tail;
    self->reactionCursors.tracked[2] = tail;
    return 1;
}

namespace {
constexpr std::int16_t cameraYawAngles[16]{-32767, 0, -32767, -32767, 16384, 8192, 24576, 24576,
    -16383,-8191,-24575,-24575,-32767,0,-32767,-32767};
using ReactionPc = const std::uint8_t *;
std::int16_t ReactionWord(ReactionPc p) {
    return static_cast<std::int16_t>((static_cast<std::uint16_t>(p[0]) << 8) | p[1]);
}
std::int32_t ReactionDword(ReactionPc p) {
    return std::bit_cast<std::int32_t>(
        (static_cast<std::uint32_t>(p[0]) << 24) | (static_cast<std::uint32_t>(p[1]) << 16) | (static_cast<std::uint32_t>(p[2]) << 8) | p[3]);
}
ReactionPc ReactionTarget(VFActor &self, std::int32_t offset) {
    return reinterpret_cast<ReactionPc>(self.motionParameters[0]) + offset;
}
void SetReactionPc(VFActor &self, ReactionPc p) {
    self.reactionPc = p;
}
void ResetReactionSlots(VFActor &self) {
    const std::uint8_t tail = self.reactionCursors.endpoint[1];
    self.reactionCursors.tracked[0] = tail;
    self.reactionCursors.tracked[1] = tail;
    self.reactionCursors.tracked[2] = tail;
}
std::int32_t ScaleReactionCount(VFActor &self, std::int32_t count) {
    return static_cast<std::int32_t>(static_cast<double>(static_cast<float>(count) * self.inverseScale) + 0.5);
}
std::uint16_t ReactionFrame(VFActor &self, std::uint8_t selector) {
    switch (selector) {
    case 1:
        return self.reactionEndFrame;
    case 2:
        return self.eventFrame30a;
    case 3:
        return static_cast<std::uint16_t>(self.parameter2d0);
    default:
        return static_cast<std::uint16_t>(self.reactionMiddleFrame);
    }
}
std::uint16_t ReactionMask(VFActor &self, ReactionPc p, bool lowOnly = false) {
    std::uint16_t value = p[3];
    if (p[2] & 4)
        value |= self.reactionMaskA.words[0];
    if (p[2] & 8)
        value |= self.reactionMaskB.words[0];
    return lowOnly ? static_cast<std::uint8_t>(value) : value;
}
std::uint32_t ReactionIntersectionMask(VFActor &self, ReactionPc p) {
    if (p[2] & 4)
        return self.reactionMaskA.bits;
    if (p[2] & 8)
        return self.reactionMaskB.bits;
    return static_cast<std::uint16_t>(ReactionWord(p + 2));
}
void PushReactionContext(VFActor &self, ReactionPc target, std::uint32_t priorityOnly, bool incrementDepth) {
    const std::uint8_t depth = self.reactions.depth;
    self.reactions.contexts[depth].program = target;
    self.reactions.contexts[depth].scanRemaining = self.reactions.scanRemaining;
    self.reactions.contexts[depth].priorityOnly = priorityOnly;
    self.reactions.contexts[depth].priorityFlags = self.priorityFlags;
    self.reactions.contexts[depth].priorityMask = self.priorityMask;
    self.reactions.contexts[depth].maskSnapshot = self.reactions.maskSnapshot;
    self.reactions.contexts[depth].allowedMask = self.reactions.window.count.allowed;
    self.reactions.contexts[depth].remainingFrames = static_cast<std::uint16_t>(ScaleReactionCount(self, self.reactions.window.count.remaining));
    self.reactions.contexts[depth].historyIndex = self.reactions.historyIndex;
    if (incrementDepth)
        self.reactions.depth = static_cast<std::uint8_t>(depth + 1);
}
std::int8_t MoveProficiencyPhase(std::uint8_t move) {
    const auto category = MoveCategory(move);
    if (category >= 4)
        return -1;
    const auto index = static_cast<std::int8_t>(move - moveStarts[category]);
    if (index >= moveCounts[category] || index == -1)
        return -1;
    const auto proficiency = static_cast<std::int32_t>(GetMoveProficiency(category, static_cast<std::uint8_t>(index)));
    return proficiency < 12 ? 0 : proficiency < 56 ? 1 : proficiency < 200 ? 2 : 3;
}


std::uint8_t RunReactionCallback(VFActor &s, std::uint8_t opcode) {
    const ReactionPc p = s.reactionPc;
    const auto arg = static_cast<std::int8_t>(p[1]);
    const std::uint16_t previous = s.reactions.maskSnapshot;
    const std::uint16_t mask = s.priorityMask;
    const std::uint16_t flags = s.priorityFlags;
    const auto advance = [&](std::uint32_t size) { SetReactionPc(s, p + size); };
    const auto gate = [&](bool condition, std::uint32_t size = 4) -> std::uint8_t {
        if (!condition)
            return 3;
        advance(size);
        return 0;
    };
    const auto loadAction = [&] { s.action = static_cast<std::uint64_t>(static_cast<std::int64_t>(ReactionDword(p + 4))); };
    const auto clearTiming = [&] {
        s.reactionDelay.count.remaining = std::uint16_t{0};
        s.reactionDelayActive = std::uint8_t{0};
    };
    switch (opcode) {
    case 0: {
        if (!flags)
            return 4;
        const auto required = ReactionMask(s, p, true);
        if (required != flags || required != mask)
            return 3;
        s.reactions.maskSnapshot = required;
        advance(4);
        return 0;
    }
    case 1: {
        const auto required = ReactionMask(s, p);
        if (required != mask)
            return (mask != previous) + 4;
        s.reactions.maskSnapshot = required;
        advance(4);
        return 2;
    }
    case 2:
    case 43: {
        const bool overlap = (mask & ReactionIntersectionMask(s, p)) != 0;
        if (overlap == (opcode == 43))
            return (mask != previous) + 4;
        s.reactions.maskSnapshot = mask;
        advance(4);
        return 2;
    }
    case 3: {
        const auto required = ReactionMask(s, p, true);
        s.reactions.window.count.remaining = static_cast<std::uint16_t>(ScaleReactionCount(s, arg));
        s.reactions.window.count.allowed = required;
        advance(4);
        return previous == mask || !(static_cast<std::uint16_t>(~required) & mask) ? 0 : 3;
    }
    case 4:
        s.reactions.scanRemaining = ScaleReactionCount(s, arg);
        advance(4);
        return 0;
    case 5:
        return vfReactCommitActionAndResetSlots(&s);
    case 6:
        loadAction();
        s.reactionCursors.tracked[1] = s.reactionCursors.endpoint[1];
        s.reactionCursors.tracked[2] = s.reactionCursors.endpoint[1];
        return 1;
    case 7:
        return gate((ReactionDword(p + 4) & s.groundContactFlags) != 0, 8);
    case 8:
        return gate((ReactionDword(p + 4) & s.moveFlags) != 0, 8);
    case 9:
        return gate((ReactionDword(p + 4) & s.hitStateFlags) != 0, 8);
    case 10:
        return gate((ReactionDword(p + 4) & s.groundContactFlags) == 0, 8);
    case 11:
        return gate((ReactionDword(p + 4) & s.moveFlags) == 0, 8);
    case 12:
        return gate((ReactionDword(p + 4) & s.hitStateFlags) == 0, 8);
    case 13:
        s.reactionContinuation = ReactionTarget(s, ReactionWord(p + 2));
        advance(4);
        return 0;
    case 14:
        SetReactionPc(
            s, static_cast<std::uint16_t>(ReactionWord(p + 2)) == s.reactions.conditionFlags ? ReactionTarget(s, ReactionWord(p + 4)) : p + 8);
        return 0;
    case 15:
        PushReactionContext(s, ReactionTarget(s, ReactionWord(p + 2)), 0, true);
        advance(4);
        return 0;
    case 16:
        return 6;
    case 17:
        SetReactionPc(s, ReactionTarget(s, ReactionWord(p + 2)));
        return 0;
    case 18: {
        auto frame = ReactionFrame(s, p[3]);
        advance(4);
        if (s.animationTime >= static_cast<float>(frame + static_cast<std::uint16_t>(arg)) - s.scale)
            return 0;
        if (s.activeMotionFlags & 4)
            s.reactionDelay.count.remaining = s.reactionEndFrame;
        else {
            if (s.eventFrame30a && frame > static_cast<std::uint16_t>(s.reactionMiddleFrame))
                frame = s.eventFrame30a;
            s.reactionDelay.count.remaining = static_cast<std::uint16_t>(frame + arg);
        }
        ResetReactionSlots(s);
        s.reactionContinuation = p + 4;
        return 1;
    }
    case 19:
        return gate(static_cast<float>(ReactionDword(p + 4)) * 0.0000152587890625f >= IncrementSharedPrngFloat(), 8);
    case 20:
        return gate(std::to_integer<std::int8_t>(s.slot->reserved84[4]) == arg && s.slot->hpPhaseTimer <= 0.00000011920929f);
    case 21: {
        const auto count = static_cast<std::uint16_t>(ScaleReactionCount(s, arg));
        const auto target = ReactionTarget(s, ReactionWord(p + 4));
        const auto selector = ReactionWord(p + 2);
        if (selector) {
            s.pendingHitMove = selector;
            s.pendingReactionTicks = count;
            s.pendingReactionProgram = target;
        } else {
            s.savedRootReactionTicks = count;
            s.savedRootReaction.program = target;
        }
        advance(8);
        return 0;
    }
    case 22:
        return gate(std::bit_cast<std::int32_t>(s.reactions.conditionFlags) == ReactionWord(p + 2));
    case 23:
        s.reactionContinuation = p;
        advance(4);
        s.reactionDelay.count.remaining = std::uint16_t{0x7fff};
        return 0;
    case 24:
        advance(4);
        s.reactionContinuation = nullptr;
        clearTiming();
        return 0;
    case 25:
        advance(4);
        s.savedRootReaction.program = nullptr;
        return 0;
    case 26:
        return gate(s.contactDistances[s.actionIndex] >= static_cast<float>(ReactionDword(p + 4)) * 0.0000152587890625f, 8);
    case 27:
        return gate(s.contactDistances[s.actionIndex] < static_cast<float>(ReactionDword(p + 4)) * 0.0000152587890625f, 8);
    case 28:
        return gate((static_cast<std::uint32_t>(s.queuedAction) & ReactionDword(p + 4)) != 0, 8);
    case 29:
        return gate(s.animationTime <= static_cast<float>(ReactionFrame(s, p[3]) + arg) - s.scale);
    case 30:
        if (!(flags & static_cast<std::uint16_t>(ReactionWord(p + 2))))
            return 4;
        advance(4);
        s.reactions.maskSnapshot = mask;
        return 0;
    case 31:
    case 63:
    case 78:
        advance(4);
        return 0;
    case 32:
        s.reactions.stopIndex = (s.reactionCursors.tracked[arg ? 1 : 0]);
        advance(4);
        return 0;
    case 33:
    case 60:
        return 3;
    case 34:
        s.reactionOverrideFlags.bits = s.reactionOverrideFlags.bits | static_cast<std::uint32_t>(ReactionDword(p + 4));
        advance(8);
        return 0;
    case 35:
        return gate((s.reactionOverrideFlags.bits & ReactionDword(p + 4)) != 0, 8);
    case 36:
        return gate(s.actor->parameter2ca == ReactionWord(p + 4), 8);
    case 37:
        return gate(std::to_integer<std::uint8_t>(s.reserved410) >= p[1]);
    case 38:
        PushReactionContext(s, nullptr, 1, false);
        advance(4);
        return 0;
    case 39: {
        const std::int8_t depth = std::bit_cast<std::int8_t>(s.reactions.depth);
        s.reactions.contexts[depth].program = ReactionTarget(s, ReactionWord(p + 2));
        s.reactions.contexts[depth].priorityOnly = std::uint32_t{1};
        s.reactions.depth = static_cast<std::uint8_t>(depth + 1);
        advance(4);
        return 0;
    }
    case 40: {
        const std::uint16_t value = s.reactionHistory[s.reactions.historyIndex].sectorMask;
        const std::int32_t offset = value & s.reactionMaskA.bits ? 2 : value & s.reactionMaskB.bits ? 4 : 6;
        SetReactionPc(s, ReactionTarget(s, ReactionWord(p + offset)));
        return 0;
    }
    case 41:
        return gate((s.actor->hitStateFlags & ReactionDword(p + 4)) == 0, 8);
    case 42:
        return gate((s.actor->hitStateFlags & ReactionDword(p + 4)) != 0, 8);
    case 44:
        s.counterOperands.value[VF_REACTION_COUNTER_BIAS + static_cast<std::int8_t>(p[2])] = p[1];
        advance(4);
        return 0;
    case 45:
    case 46: {
        std::uint8_t &counter = s.counterOperands.value[VF_REACTION_COUNTER_BIAS + arg];
        counter = static_cast<std::uint8_t>(counter + (opcode == 45 ? 1 : -1));
        advance(4);
        return 0;
    }
    case 47:
        return gate(std::bit_cast<std::int8_t>(s.counterOperands.value[VF_REACTION_COUNTER_BIAS + static_cast<std::int8_t>(p[2])]) > arg);
    case 48:
        return gate(s.counterOperands.value[VF_REACTION_COUNTER_BIAS + static_cast<std::int8_t>(p[2])] == p[1]);
    case 49:
        return gate(std::bit_cast<std::int8_t>(s.counterOperands.value[VF_REACTION_COUNTER_BIAS + static_cast<std::int8_t>(p[2])]) < arg);
    case 50: {
        if (!(s.hitStateFlags & 0x10))
            return 3;
        const auto marker = FindLoadedMotionSecondaryEntry(static_cast<std::uint16_t>(ReactionWord(p + 2)) & 0xfff, 3);
        if (marker.empty())
            return 3;
        std::uint16_t frame;
        std::memcpy(&frame, marker.data() + 6, sizeof(frame));
        if (!(static_cast<float>(static_cast<std::int16_t>(frame - 1)) > s.animationTime))
            return 3;
        loadAction();
        clearTiming();
        ResetReactionSlots(s);
        return 1;
    }
    case 51:
        advance(4);
        s.reactionImpulseBytes[1] = std::uint8_t{96};
        return 0;
    case 52: {
        const std::uint8_t head = s.reactionCursors.endpoint[0];
        s.reactionCursors.endpoint[1] = head;
        ResetReactionSlots(s);
        advance(4);
        return 0;
    }
    case 53:
        return gate((arg ^ (s.game->type == static_cast<std::int8_t>(p[2]))) == 1);
    case 54:
        return gate(vfCheckSkillFlag(shen_name{static_cast<std::uint32_t>(ReactionDword(p + 4))}), 8);
    case 55:
        s.reactions.chainFrames = std::int8_t{-1};
        s.reactionContinuation = ReactionTarget(s, ReactionWord(p + 2));
        return 7;
    case 56:
        return gate(ReactionWord(p + 2) == s.currentAnimation);
    case 57: {
        if (!s.reactionDelayActive) {
            const auto frame = static_cast<float>(ReactionFrame(s, p[7]) + static_cast<std::uint16_t>(ReactionWord(p + 2)));
            if (s.animationTime > frame) {
                s.reactionDelay.count.allowed = std::uint16_t{0};
                s.reactionDelayActive = std::uint8_t{0};
                advance(8);
                return 0;
            }
            s.reactionDelayActive = std::uint8_t{0xff};
            s.reactionDelay.count.allowed = static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<double>((frame - s.animationTime) * s.inverseScale) + 0.5));
        }
        const std::uint16_t remaining = s.reactionDelay.count.allowed;
        if (remaining) {
            s.reactionDelay.count.allowed = static_cast<std::uint16_t>(remaining - 1);
            s.reactionContinuation = p;
            return 1;
        }
        advance(8);
        s.reactionDelay.count.allowed = std::uint16_t{0};
        s.reactionDelayActive = std::uint8_t{0};
        return 0;
    }
    case 58:
        return gate((s.reactionFlags & ReactionDword(p + 4)) != 0, 8);
    case 59:
        return gate((s.reactionFlags & ReactionDword(p + 4)) == 0, 8);
    case 61:
        advance(4);
        return !(static_cast<float>(ReactionWord(p + 2)) - 1 >= s.actor->animationTime) ? 3 : 0;
    case 62:
        advance(8);
        return (s.hitFlags & ReactionDword(p + 4)) != 0 ? 0 : 3;
    case 64:
        return gate((s.pairedContactPriority & ReactionDword(p + 4)) != 0, 8);
    case 65:
        return gate(!(s.pairedContactPriority & 1));
    case 66:
        s.pairedContactPriority |= 1;
        advance(4);
        return 0;
    case 67:
        s.action = static_cast<std::uint64_t>(s.secondActor->parameter33c[0]) + 0x13000000;
        if (s.game->playerSlot != -1 && s.secondActor->groupIndex == s.game->playerSlot)
            vfmaUpdateCollisionCounter(
                s.game, VFCollisionCounterEvent::HitSequenceEnded, static_cast<std::uint16_t>(std::bit_cast<std::int8_t>(s.secondActor->reactionVariantBytes[3])));
        clearTiming();
        ResetReactionSlots(s);
        return 1;
    case 68:
        return gate(static_cast<float>(static_cast<std::uint8_t>(s.actor->guardStrength)) >= s.hp);
    case 69:
        if (arg)
            s.pairContactLastFrame = static_cast<std::uint32_t>(ReactionDword(p + 4));
        else
            s.pairContactMiddleFrame = static_cast<std::uint32_t>(ReactionDword(p + 4));
        advance(8);
        return 0;
    case 70: {
        const auto table = ReactionTarget(s, ReactionWord(p + 2)) + static_cast<std::uint8_t>(16 * (s.footFacingBytes[1] & p[1]));
        SetReactionPc(s, ReactionTarget(s, ReactionDword(table)));
        return 0;
    }
    case 71:
        s.reactionVariantBytes[3] = p[1];
        advance(4);
        return 0;
    case 72:
    case 73:
    case 74:
    case 75: {
        const auto required = ReactionMask(s, p);
        const auto direction = static_cast<std::int16_t>(cameraYawAngles[(required >> 4) & 15] + 0x8000);
        if (opcode == 73) {
            if (!flags)
                return 4;
            if (flags & 0xfe0f)
                return 3;
            const auto threshold = ReactionWord(p + 4);
            if (threshold < Abs16(vfGetFacingDeltaFromCameraSectorCode(&s, flags) - direction) ||
                threshold < Abs16(vfGetFacingDeltaFromCameraSectorCode(&s, mask) - direction))
                return 3;
            advance(8);
            s.reactions.maskSnapshot = mask;
            return 0;
        }
        if (opcode != 75) {
            if (!mask && !required) {
                s.reactions.maskSnapshot = std::uint16_t{0};
                advance(opcode == 72 ? 8 : 4);
                return 2;
            }
            if (!mask || !required)
                return 4;
            if (mask & 0xfc0f)
                return 3;
        }
        const auto difference = Abs16(vfGetFacingDeltaFromCameraSectorCode(&s, mask) - direction);
        const bool accepted = opcode == 72 ? difference <= ReactionWord(p + 4) : opcode == 74 ? difference <= 16384 : difference > 16384;
        if (!accepted)
            return (mask != previous) + 4;
        advance(opcode == 72 ? 8 : 4);
        s.reactions.maskSnapshot = mask;
        return 2;
    }
    case 76: {
        const std::int16_t angle = vfGetFacingDeltaFromCameraSectorCode(&s, s.reactionHistory[s.reactions.historyIndex].sectorMask);
        const auto offset = Abs16(angle) <= ReactionWord(p + 8) ? 2 : Abs16(angle + 0x8000) <= ReactionWord(p + 10) ? 4 : 6;
        SetReactionPc(s, ReactionTarget(s, ReactionWord(p + offset)));
        return 0;
    }
    case 77: {
        auto required = static_cast<std::uint16_t>(p[3] & 0xf0);
        if (p[2] & 4)
            required |= s.reactionMaskA.words[0];
        if (p[2] & 8)
            required |= s.reactionMaskB.words[0];
        s.reactions.window.count.allowed = required;
        s.reactions.window.count.remaining = static_cast<std::uint16_t>(ScaleReactionCount(s, arg));
        advance(8);
        const auto angle = vfGetFacingDeltaFromCameraSectorCode(&s, mask);
        return previous == mask || ((mask & 0xf0) && static_cast<std::int16_t>(0x8000 - ReactionWord(p + 12)) < Abs16(angle + 0x8000)) ? 0 : 3;
    }
    case 79: {
        for (auto *object = g_fightStageObjectRoot; object; object = object->next)
            if ((object->object.reactionFlags & 1) && object->object.reactingCharacter == s.character)
                return 3;
        auto facing = p[3];
        for (auto *object = g_fightStageObjectRoot; object; object = object->next) {
            auto &o = object->object;
            if (o.state || (o.reactionFlags & 1) || (o.motionFlags & 0xb00000) != 0xb00000 ||
                !(static_cast<std::uint32_t>(o.contactSlotMask >> 32) & (1U << s.groupIndex)))
                continue;
            if (o.definition->index == 2) {
                const auto position = GetCharacterWorldPosition(o.model);
                const auto angle = FixedAtan2(s.contactPosition.x - position.x, s.contactPosition.z - position.z);
                const auto delta = static_cast<std::int32_t>(GetAllAngYForNode(&o.model->entity.node)) - angle;
                if (static_cast<std::uint16_t>(std::abs(delta) - 10241) <= 0x2ffe) {
                    if (s.parameterAd7 == static_cast<std::uint8_t>(o.index))
                        s.parameterAd7 = std::bit_cast<std::int8_t>(std::uint8_t{0xff});
                    continue;
                }
            }
            const auto angle = FixedAtan2(s.contactPosition.x - o.contactPosition.x, s.contactPosition.z - o.contactPosition.z);
            if ((0x4000 - s.temporaryFacingAngle + angle) & 0x8000)
                facing ^= 1;
            if (facing)
                continue;
            s.parameterAd7 = std::bit_cast<std::int8_t>(static_cast<std::uint8_t>(o.index));
            advance(4);
            return 0;
        }
        return 3;
    }
    case 80:
        return gate((s.activeMotionFlags & ReactionDword(p + 4)) != 0, 8);
    case 81:
        return gate((s.activeMotionFlags & ReactionDword(p + 4)) == 0, 8);
    case 82:
        return gate((s.motionFlags & ReactionDword(p + 4)) != 0, 8);
    case 83:
        return gate((s.motionFlags & ReactionDword(p + 4)) == 0, 8);
    case 84:
        return gate(static_cast<float>(ReactionWord(p + 2)) >= s.actor->hp);
    case 85:
        return gate(s.actor->fightStyle == p[1]);
    case 86: {
        auto phase = MoveProficiencyPhase(p[1]);
        if (phase == -1)
            return 3;
        if (!phase && s.game->type == 4)
            phase = 1;
        return gate(phase >= static_cast<std::int8_t>(p[2]));
    }
    case 87:
        return gate(std::to_integer<std::int8_t>(s.slot->reserved84[4]) == arg);
    case 88:
        return gate(static_cast<std::uint8_t>(s.actor->guardState) == p[1]);
    case 89: {
        const std::uint32_t condition = s.reactions.conditionFlags;
        SetReactionPc(
            s, static_cast<std::uint16_t>(ReactionWord(p + 2)) == (condition & 0xff00) && (condition & 31) ? ReactionTarget(s, ReactionWord(p + 4)) : p + 8);
        return 0;
    }
    case 90:
        return gate((s.actor->reactionEntryFlags & (1U << (p[7] & 31))) != 0, 8);
    case 91:
        s.reactionCharacter = s.actor->character;
        advance(4);
        return 0;
    case 92:
        return gate(s.reactionCharacter == s.actor->character);
    case 93:
        return gate(static_cast<std::int8_t>(static_cast<std::uint8_t>(s.actor->guardLevel) - static_cast<std::uint8_t>(s.guardLevel)) > arg);
    case 94:
        if (!p[1])
            return 3;
        s.reactionRank = p[1];
        advance(4);
        return 0;
    case 95: {
        std::uint8_t count = 0;
        for (std::uint8_t i = 0; i < 6; ++i)
            if (s.game->characters[i] && s.game->slotTeams[i] == s.game->slotTeams[s.groupIndex] && i != s.groupIndex &&
                !(std::fabs(s.game->actors[i]->hp) < 0.00000011920929f) && !(s.game->actors[i]->hitStateFlags & 0x14001))
                ++count;
        return gate(count >= arg);
    }
    case 96:
        return gate(s.actor->hp > static_cast<float>(static_cast<std::uint8_t>(s.guardStrength)));
    case 97:
        return gate(s.pairedActor->fightStyle != arg);
    case 98:
    case 99:
        return gate((s.actor->activeMotionFlags & ReactionDword(p + 4)) != 0, 8);
    case 100:
    case 101:
        return gate((s.actor->savedMarkerMask & ReactionDword(p + 4)) != 0, 8);
    case 102:
        return gate(s.actor->animationTime <= static_cast<float>(ReactionFrame(*s.actor, p[3]) + arg) - s.actor->scale);
    case 103: {
        if (p[1] != 1)
            return 3;
        float distance = 2.5f;
        std::int8_t selected = -1;
        for (std::int8_t i = 0; i < 6; ++i) {
            if (s.game->characters[i] && s.game->actors[i] != &s && static_cast<std::int16_t>(s.contactAngles[i] - s.temporaryFacingAngle + 0x4000) < 0 &&
                s.contactDistances[i] < distance) {
                selected = i;
                distance = s.contactDistances[i];
            }
        }

        if (selected <= 0)
            return 3;
        const auto old = s.targetingCharacter;
        s.actionIndex = selected;
        s.actor = s.game->actors[selected];
        s.targetingCharacter = s.game->characters[selected];
        if (old != s.targetingCharacter)
            s.targetCache = std::bit_cast<std::int8_t>(std::uint8_t{0xff});
        advance(4);
        return 0;
    }
    }
#if defined(_MSC_VER)
    __assume(0);
#else
    __builtin_unreachable();
#endif
}
} // namespace

// Camera-relative sector angles use contact yaw only for close, frontal actors.
// @verified - p
std::int16_t vfGetFacingDeltaFromCameraSectorCode(VFActor *self, std::uint16_t code) {
    const auto sector = static_cast<std::int16_t>(cameraYawAngles[(code >> 4) & 15] + 0x8000);
    const auto facing = self->contactDistances[self->actionIndex] < 3 && !(self->groundContactFlags & 1) && !(self->actor->moveFlags & 0x20) ?
        self->contactAngles[self->actionIndex] :
        static_cast<std::int16_t>(self->facingYaw);
    return static_cast<std::int16_t>(sector - facing + camCtrlGetNextRotY());
}


// @verified - p
std::uint8_t VF_RunStateMachine(VFActor *actor) {
    auto &s = *actor;
    const std::uint16_t savedMask = s.priorityMask, savedFlags = s.priorityFlags;
    const std::uint16_t savedSnapshot = s.reactions.maskSnapshot;
    const ReactionPc savedPc = s.reactionPc;
    const std::uint64_t savedOverride = s.reactionOverrideActive;
    s.reactions.depth = std::uint8_t{0};
    const std::uint8_t index = static_cast<std::uint8_t>((s.reactionCursors.endpoint[1] - 1) & 31);
    s.reactions.historyIndex = index;
    const auto filter = static_cast<std::uint8_t>(s.queuedAction >> 24) == 6 ? 0x1f2 : 0xf0;
    s.priorityMask &= filter;
    s.priorityFlags &= filter;
    s.reactions.maskSnapshot = s.priorityMask;
    s.reactions.scanRemaining = static_cast<std::int32_t>((index - s.reactionCursors.previous) & 31);
    s.reactions.window.both = std::uint32_t{0};
    for (;;) {
        const ReactionPc pc = s.reactionPc;
        auto response = RunReactionCallback(s, *pc);
        bool pop = false, finish = false;
        if (response == 5) {
            const std::uint16_t allowed = s.reactions.window.count.allowed;
            if (allowed) {
                const std::uint16_t remaining = s.reactions.window.count.remaining;
                s.reactions.window.count.remaining = static_cast<std::uint16_t>(remaining - 1);
                pop = !remaining;
                if (!pop) {
                    s.reactions.maskSnapshot = s.priorityMask;
                    pop = s.priorityMask && (static_cast<std::uint16_t>(~allowed) & s.priorityMask);
                    if (!pop && s.priorityMask)
                        s.reactions.window.count.allowed = s.priorityMask;
                }
                response = 4;
            } else
                response = (s.priorityMask != s.reactions.maskSnapshot) + 3;
        }
        if (response == 4 && !pop) {
            const std::int32_t count = s.reactions.scanRemaining;
            s.reactions.scanRemaining = count - 1;
            const std::uint8_t cursor = s.reactions.historyIndex;
            const std::uint8_t stop = s.reactions.stopIndex;
            pop = count < 0 || cursor == stop;
            if (!pop) {
                const std::uint8_t previous = s.reactionCursors.previous;
                const std::uint8_t next = static_cast<std::uint8_t>((cursor - 1) & 31);
                s.reactions.historyIndex = next;
                s.priorityMask = s.reactionHistory[next].mask & 0xf0;
                const std::uint16_t historyFlags = s.reactionHistory[next].pressed;
                auto phase = static_cast<std::int8_t>((previous >= next) + (stop <= previous ? 0 : 1));
                if (stop > next)
                    --phase;
                if (phase < 1) {
                    s.priorityFlags = historyFlags & s.reactions.filters.mask.current;
                    pop = s.priorityFlags != (historyFlags & s.reactions.filters.mask.previous);
                } else
                    s.priorityFlags = historyFlags & 0xf0;
            }
        } else if (response == 3)
            pop = true;
        else if (response == 6) {
            s.reactions.chainFrames = std::bit_cast<std::int8_t>(std::uint8_t{0});
            finish = true;
        } else if (response == 7)
            finish = true;
        else if (response == 1)
            return 0x7f;
        if (pop) {
            const std::int8_t depth = static_cast<std::int8_t>(s.reactions.depth - 1);
            s.reactions.depth = static_cast<std::uint8_t>(depth);
            if (depth < 0) {
                s.reactions.chainFrames = std::bit_cast<std::int8_t>(std::uint8_t{0});
                finish = true;
            } else {
                if (!s.reactions.contexts[depth].priorityOnly) {
                    s.priorityFlags = s.reactions.contexts[depth].priorityFlags;
                    s.priorityMask = s.reactions.contexts[depth].priorityMask;
                    s.reactions.maskSnapshot = s.reactions.contexts[depth].maskSnapshot;
                    s.reactions.window.count.allowed = s.reactions.contexts[depth].allowedMask;
                    s.reactions.window.count.remaining = s.reactions.contexts[depth].remainingFrames;
                    s.reactions.historyIndex = s.reactions.contexts[depth].historyIndex;
                    s.reactions.scanRemaining = s.reactions.contexts[depth].scanRemaining;
                }
                SetReactionPc(s, s.reactions.contexts[depth].program);
            }
        }
        if (finish) {
            s.priorityMask = savedMask;
            s.priorityFlags = savedFlags;
            s.reactions.maskSnapshot = savedSnapshot;
            SetReactionPc(s, savedPc);
            s.reactionOverrideActive = savedOverride;
            return 1;
        }
    }
}


void vfQueueReactionFlagsForFrame(VFActor *actor, bool clearDifference) {
    auto &s = *actor;
    const std::uint8_t head = s.reactionCursors.endpoint[0];
    const std::uint8_t previous = static_cast<std::uint8_t>((head - 1) & 31);
    const std::uint16_t previousMask = s.reactionHistory[previous].mask;
    const std::uint16_t previousSector = s.reactionHistory[previous].sectorMask;
    std::uint16_t current = s.cameraControlFlags;
    const std::uint16_t difference = static_cast<std::uint16_t>(current & ~previousMask);
    const std::uint16_t released = static_cast<std::uint16_t>(previousMask & ~current);
    s.priorityMask = current;
    s.priorityFlags = difference;
    s.followupFlags = difference;
    s.reactionHistory[head].mask = current;
    s.reactionHistory[head].pressed = clearDifference ? std::uint16_t{0} : difference;
    s.reactionHistory[head].released = released;
    s.reactions.releasedFlags = released;
    if (difference)
        s.reactions.pressed = difference;
    if (!(current & 0xc0))
        current |= previousSector & 0xc0;
    if (!(current & 0x30))
        current |= previousSector & 0x30;
    s.reactionHistory[head].sectorMask = current;
    const std::uint8_t next = static_cast<std::uint8_t>((head + 1) & 31);
    const std::uint8_t following = static_cast<std::uint8_t>((head + 2) & 31);
    s.reactionCursors.endpoint[0] = next;
    for (std::uint8_t &cursor : s.reactionCursors.tracked)
        if (cursor == next)
            cursor = following;
}

// Select the actual authored base, paused, or continuation reaction program.
void vfUpdateReactionOverrideState(VFActor *actor) {
    auto &s = *actor;
    bool keep = false;
    if (s.moveFlags & 0x10)
        s.reactionOverrideActive |= 1;
    else {
        const ReactionPc continuation = s.reactionContinuation;
        SetReactionPc(s, continuation);
        if (s.hitStateFlags & 1) {
            s.reactionOverrideActive |= 1;
            keep = true;
        } else if (continuation && (s.reactionDelay.count.remaining || s.reactionDelayActive))
            keep = true;
    }
    if (!keep)
        vfResetReactionProgram(&s);
    std::uint32_t mask = s.reactionOverrideFlags.bits;
    mask = mask & 2 ? mask | 4 : mask & ~4U;
    mask = s.reactionOverrideActive & 1 ? mask | 2 : mask & ~2U;
    s.reactionOverrideFlags.bits = mask;
}



void vfUpdateDeferredReactionPriority(VFActor *actor, std::uint8_t slot) {
    auto &s = *actor;
    const auto flags = static_cast<std::uint16_t>(s.priorityFlags & 0x30f);
    const std::uint8_t active = s.deferredPriorityState[slot];
    if (active) {
        const std::uint16_t saved = s.deferredPriorityFlags[slot];
        if ((s.reactionOverrideActive & 1) || saved != 0x100 || (s.priorityMask & 0x100) || flags != 1) {
            const auto merged = static_cast<std::uint16_t>(saved | flags);
            if (merged != 0x30f)
                s.deferredPriorityState[slot] = static_cast<std::uint8_t>(active - 1);
            s.deferredPriorityState[slot] = std::uint8_t{0};
            s.priorityFlags |= merged;
        } else {
            const std::uint8_t tail = s.reactionCursors.endpoint[1];
            s.reactionCursors.deferred[slot] = tail;
            s.reactionCursors.previous = tail;
            s.deferredPriorityState[slot] = std::uint8_t{0};
            s.priorityFlags |= 1;
        }
    } else {
        if (flags && flags != 0x30f) {
            s.deferredPriorityState[slot] = std::uint8_t{1};
            s.deferredPriorityFlags[slot] = flags;
            s.priorityFlags &= 0xfcf0;
        }
        const std::uint8_t tail = s.reactionCursors.endpoint[1];
        s.reactionCursors.deferred[slot] = tail;
        s.reactionCursors.previous = tail;
    }
}

// Run the borrowed saved-root program before the current state program.
std::uint8_t vfHandleSavedCharRootPosReaction(VFActor *actor) {
    auto &s = *actor;
    s.reactions.filters.both = std::uint32_t{0xffff01f0};
    s.reactionContinuation = nullptr;
    const ReactionPc savedRoot = s.savedRootReaction.program;
    const ReactionPc current = s.reactionPc;
    s.reactions.stopIndex = s.reactionCursors.tracked[0];
    if (!savedRoot)
        return VF_RunStateMachine(actor);
    SetReactionPc(s, savedRoot);
    auto result = VF_RunStateMachine(actor);
    if (result != 0x7f) {
        SetReactionPc(s, current);
        if (current) {
            s.reactions.stopIndex = s.reactionCursors.tracked[0];
            s.reactionContinuation = nullptr;
            result = VF_RunStateMachine(actor);
        }
    }
    return result;
}

// Priority admission may consume the pending authored continuation.
std::uint8_t vfResolvePriorityReaction(VFActor *actor) {
    auto &s = *actor;
    if ((s.hitStateFlags & 0x1c000) || !(s.priorityFlags & 0x300))
        return 1;
    s.reactionOverrideFlags.bits = s.reactionOverrideFlags.bits & ~1U;
    s.reactionCursors.endpoint[1] = s.reactionCursors.endpoint[0];
    const ReactionPc continuation = s.reactionContinuation;
    if (!(continuation ? s.reactionDelay.count.remaining != 0 : (s.reactionOverrideActive & 1) || (s.hitStateFlags & 0x10))) {
        s.reactions.conditionFlags = std::uint32_t{4};
        if (vfHandleSavedCharRootPosReaction(actor) == 0x7f)
            return 0x7f;
    }
    const std::uint8_t selected = s.reactionCursors.deferred[0];
    s.reactionCursors.tracked[0] = selected;
    s.reactionCursors.tracked[1] = selected;
    s.reactionContinuation = nullptr;
    s.reactionDelay.count.remaining = std::uint16_t{0};
    s.reactionDelayActive = std::uint8_t{0};
    return 0x7f;
}

namespace {
constexpr std::uint16_t inputResponses[2][16]{
    {0,8,1,0,0,0,16384,16384,0,4,2,0,32,16,128,64},
    {0,8,1,0,0,0,16384,0,0,4,2,0,32,16,128,64}};
std::uint16_t MapFightInput(std::uint16_t input, bool secondary) {
    std::uint16_t response = 0;
    for (std::uint8_t bit = 0; bit < 16; ++bit)
        if (input & (1U << bit))
            response |= inputResponses[secondary][bit];
    return response;
}
std::uint16_t PlayerFightInput(VFActor &self) {
    if (!self.game->savedInput)
        return 0;
    auto response = MapFightInput(ctrlGetPadInputBuffer0(self.game->savedInput)->held, false);
    std::uint16_t stick = 0;
    if (g_inputBuffer[0].analog0e < 64)
        stick = 128;
    if (g_inputBuffer[0].analog0e > 192)
        stick |= 64;
    if (g_inputBuffer[0].analog0f > 192)
        stick |= 16;
    if (g_inputBuffer[0].analog0f < 64)
        stick |= 32;
    return stick ? (response & 0xff0f) | stick : response;
}
std::uint8_t CameraSectorIndex(std::uint16_t code) {
    for (std::uint8_t index = 0; index < 8; ++index)
        if (cameraSectors[index] == (code & 0xf0))
            return index;
    return 0xff;
}
void RewindReactionTail(VFActor &self, std::uint8_t cursor) {
    const std::uint8_t head = self.reactionCursors.endpoint[0];
    std::int8_t distance = static_cast<std::int8_t>(head - cursor);
    if (distance < 0)
        distance += 32;
    const auto limit = self.hitStateFlags & 0x10000000 ? 8 : 6;
    if (distance > limit)
        distance = static_cast<std::int8_t>(limit);
    self.reactionCursors.endpoint[1] = static_cast<std::uint8_t>((head - distance) & 31);
}
}

// Real player/AI input history, reaction VM admission and fallback actions.
// @verified - p
void vfmaUpdateNextReaction(VFActor *actor) {
    auto &s = *actor;
    auto &game = *s.game;
    s.stateFlags &= ~0x3000U;
    s.parameter4c4 = 0xffff;
    s.parameter4c8 = -1;
    s.processedPairs = 0;
    s.reactionEvaluationPhase = std::uint8_t{0};
    if ((game.gameFlags & 0x20) || static_cast<std::uint32_t>(game.state) < 3 || static_cast<std::int8_t>(s.stateFlags) < 0)
        return;
    bool playerInput = true, clearDifference = false;
    switch (game.slotPoolModes[s.groupIndex]) {
    case 0: {
        if (game.gameFlags & 0x40000)
            s.cameraControlFlags = PlayerFightInput(s);
        else
            s.cameraControlFlags |= PlayerFightInput(s);
        if ((s.cameraControlFlags & 0xf0) == 0xf0)
            s.cameraControlFlags = (s.cameraControlFlags & 0xff0f) | 0x4000;
        if (game.mode & 0xc103b) {
            std::memset(&s.idleInputFrames, 0, sizeof(s.idleInputFrames) + sizeof(s.noAttackInputFrames));
        } else {
            float idle = s.idleInputFrames;

            if (static_cast<std::uint8_t>(s.cameraControlFlags) || (g_inputBuffer[0].held & 0x40))
                idle = 0;
            else if (static_cast<std::int32_t>(idle) < 65535)
                idle += shenGetScaledDeltaTickTime(1.0f);
            s.idleInputFrames = idle;
            if (static_cast<std::int32_t>(idle) > 450)
                game.mode |= 0x2000;
            float noAttack = s.noAttackInputFrames;
            if (((s.formationFlags & 0x100) || game.type == 4) && !(s.cameraControlFlags & 0xf)) {
                if (static_cast<std::int32_t>(noAttack) < 65535)
                    noAttack += shenGetScaledDeltaTickTime(1.0f);
            } else
                noAttack = 0;
            s.noAttackInputFrames = noAttack;
        }
        bool closingLearned = false;
        if (game.mode & 0x10) {
            if (static_cast<std::uint32_t>(game.state) >= 9) {
                game.timeParameter = 600;
                game.mode &= ~0x100U;
            }
            const bool timeout = !(game.timeParameter < 300);
            if (!timeout)
                game.timeParameter += shenGetScaledDeltaTickTime(1.0f);
            if (((g_inputBuffer[0].pressedPriority & 0x200) || static_cast<std::int8_t>(game.mode) < 0 || timeout) && game.timeParameter > 60 &&
                !(game.mode & 0x100)) {
                SetNewEwzoTypeForMoveLearned(1, 0, 0);
                game.mode &= ~0x90U;
                FinishLoading();
                ResetHelpIconsToItemPrompt();
                game.mode |= 0x100;
                clearDifference = closingLearned = true;
                game.timeParameter = 0;
                game.cameraTimer = 4;
                vfFightNullsub();
            }
        }
        if (!closingLearned && (game.mode & 0x100)) {
            if (!(game.cameraTimer > 0)) {
                game.mode &= ~0x100U;
                FinishLoading();
            } else {
                game.cameraTimer -= shenGetScaledDeltaTickTime(1.0f);
                if (game.cameraTimer < 0)
                    game.cameraTimer = 0;
            }
        }
        if ((s.pairedActor->moveFlags & 0x20) && (s.cameraControlFlags & 0xf8)) {
            s.pairedActor->moveFlags |= 0x40000;
            vfUpdatePairingsAndDistances(&game);
            const auto angle = s.contactAngles[s.pairedActor->groupIndex] - s.temporaryFacingAngle + 0x4000;
            if (angle & 0x8000)
                s.groundContactFlags |= 1;
            else
                s.groundContactFlags &= ~1U;
        }
        break;
    }
    case 2:
    case 3:
    case 4:
        s.cameraControlFlags = MapFightInput(g_inputBuffer[game.slotPoolModes[s.groupIndex] - 1].held, true);
        break;
    case 6:
        s.cameraControlFlags = 0;
        [[fallthrough]];
    case 1:
    case 5:
        playerInput = false;
        break;
    default:
        break;
    }
    if (game.mode & 0xc103b)
        s.cameraControlFlags = 0;
    const std::uint16_t code = s.cameraControlFlags;
    const auto sector = CameraSectorIndex(code);
    const auto rotation = (static_cast<std::int16_t>(s.temporaryFacingAngle - camCtrlGetNextRotY() + 4096) >> 13) & 7;
    const auto relative = static_cast<std::uint8_t>((sector - rotation) & 7);
    s.relativeInputSectors[0] = relative;
    if ((code & 0xf0) != (s.previousReactionInput & 0xf0) || (s.secondaryMotionFlags & 0xd00) != 0x100 || (s.hitStateFlags & 0xfd010))
        s.reactionImpulseBytes[3] = playerInput && !(game.gameFlags & 0x400000) ? relative : sector;
    const auto cameraAngle = static_cast<std::uint16_t>(cameraYawAngles[(code >> 4) & 15] + 0x8000);
    s.contactFacing = cameraAngle;
    const auto worldAngle = static_cast<std::uint16_t>(cameraAngle + camCtrlGetNextRotY());
    s.reservedAea = worldAngle;
    s.alternateContactFacing = vfGetFacingDeltaFromCameraSectorCode(&s, code);
    if (!s.fightStyle) {
        game.otherFacingYaw = static_cast<std::int16_t>(s.facingYaw);
        if (code & 0xf0) {
            if (!(game.frame3 < 15))
                game.otherFacingYaw = static_cast<std::int16_t>(worldAngle);
            else {
                game.frame3 += shenGetScaledDeltaTickTime(1.0f);
                if (game.frame3 > 15)
                    game.frame3 = 15;
            }
        } else if (game.frame3 > 0)
            game.frame3 = 0;
    }
    if (s.runningCooldownFrames > 0) {
        float remaining = s.runningCooldownFrames - shenGetScaledDeltaTickTime(1.0f);
        if (remaining < 0)
            remaining = 0;
        s.runningCooldownFrames = remaining;
    }
    if (s.runningInputFrames > 0 && !(code & 0x4000)) {
        float remaining = s.runningInputFrames - shenGetScaledDeltaTickTime(1.0f);
        if (remaining < 0)
            remaining = 0;
        s.runningInputFrames = remaining;
    }
    s.reactionOverrideActive = 0;
    vfQueueReactionFlagsForFrame(&s, clearDifference);
    vfUpdateReactionOverrideState(&s);
    s.reactionMaskA.bits = std::uint32_t{32};
    s.reactionMaskB.bits = std::uint32_t{16};
    vfUpdateDeferredReactionPriority(&s, 0);
    auto conditions = static_cast<std::uint8_t>(s.hitStateFlags & 0x800 ? (s.groundContactFlags & 1 ? 2 : 0) : (~(2 * s.groundContactFlags) & 2) | 1);
    if (!(s.priorityMask & 0x20))
        conditions |= 4;
    if (!(s.moveFlags & 2))
        conditions |= 8;
    s.footFacingBytes[1] = conditions;
    if (s.moveFlags & 0x20) {
        if (!(s.moveFlags & 0x10) && !(s.hitStateFlags & 1))
            s.action = 0x01000000;
        return;
    }
    if ((s.animationTime >= static_cast<float>(s.reactionEndFrame) - s.scale && s.pairContactLastFrame) ||
        (s.animationTime >= static_cast<float>(static_cast<std::uint16_t>(s.reactionMiddleFrame)) - s.scale && s.pairContactMiddleFrame)) {
        s.action =
            s.animationTime >= static_cast<float>(s.reactionEndFrame) - s.scale && s.pairContactLastFrame ? s.pairContactLastFrame : s.pairContactMiddleFrame;
        s.pairContactMiddleFrame = s.pairContactLastFrame = 0;
        return;
    }
    if (static_cast<std::uint8_t>(s.queuedAction >> 24) == 6 && !s.reactionContinuation) {
        if (s.hitPhase < 3 && s.priorityFlags)
            ++s.hitPhaseWaitFrames;
        return;
    }
    if (vfResolvePriorityReaction(&s) == 0x7f)
        return;
    if (s.reactionContinuation) {
        const std::uint16_t timer = s.reactionDelay.count.remaining;
        if (timer || s.reactionDelayActive) {
            if (timer && timer != 0x7fff) {
                if (s.animationTime < static_cast<float>(timer) - s.scale)
                    return;
                s.reactionDelay.count.remaining = std::uint16_t{0};
            }
            s.reactions.stopIndex = s.reactionCursors.tracked[0];
            s.reactionContinuation = nullptr;
            if (VF_RunStateMachine(&s) != 0x7f)
                ResetReactionSlots(s);
            return;
        }
    }
    if (s.reactionOverrideActive & 1) {
        if (!s.savedRootReaction.program && !s.reactionPc) {
            RewindReactionTail(s, s.reactionCursors.endpoint[1]);
            return;
        }
    } else if (s.reactionOverrideFlags.bits & 4)
        RewindReactionTail(s, s.reactionCursors.tracked[0]);
    s.deferredPriorityState[1] = std::uint8_t{0};
    const std::uint16_t previousEdges = s.reactions.pressed;
    s.reactionEvaluationPhase = std::uint8_t{1};
    while (s.reactionCursors.endpoint[1] != s.reactionCursors.endpoint[0]) {
        const std::uint8_t tail = s.reactionCursors.endpoint[1];
        s.priorityMask = s.reactionHistory[tail].mask;
        s.priorityFlags = s.reactionHistory[tail].pressed;
        s.reactions.released = static_cast<std::uint32_t>(s.reactionHistory[tail].released);
        vfUpdateDeferredReactionPriority(&s, 1);
        s.reactionCursors.endpoint[1] = static_cast<std::uint8_t>((tail + 1) & 31);
        std::uint32_t flags = (s.priorityFlags & 1) | (s.priorityFlags & 2) | (s.priorityFlags & 8);
        if (s.priorityFlags & 4)
            flags |= 0x10;
        if (flags)
            flags = s.priorityFlags & 0x100 ? flags | 4 : flags & ~4U;
        s.reactions.conditionFlags = flags;
        if (s.hitStateFlags & 0x1000)
            continue;
        if (flags) {
            if (s.moveFlags & 0x10)
                s.reactions.conditionFlags = flags | 0x500;
            if (vfHandleSavedCharRootPosReaction(&s) == 0x7f || std::bit_cast<std::int8_t>(s.reactions.chainFrames) > 0)
                return;
        }
        if (((s.hitStateFlags & 0x40000) || !(s.reactionOverrideActive & 1)) &&
            ((s.priorityFlags | static_cast<std::uint8_t>(s.reactions.releasedFlags)) & 0xf0) &&
            !(previousEdges & 7)) {
            s.reactions.stopIndex = s.reactionCursors.tracked[1];
            s.reactions.conditionFlags = std::uint32_t{32};
            s.reactions.filters.both = std::uint32_t{0x00ff01f0};
            if (VF_RunStateMachine(&s) == 0x7f)
                return;
        }
    }
    if (s.deferredPriorityState[1])
        s.reactionCursors.endpoint[1] = s.reactionCursors.deferred[1];
    s.reactionEvaluationPhase = std::uint8_t{2};
    if ((s.reactionOverrideActive & 1) || (s.hitStateFlags & 0x1000))
        return;
    if (s.activeMotionFlags & 0x800000) {
        if (code & 0x40)
            s.facingAngle -= 1024;
        else if (code & 0x80)
            s.facingAngle += 1024;
        return;
    }
    if (s.fightStyle == 16 || s.fightStyle == 46) {
        if (s.priorityFlags & 0xf0)
            s.action = 0x23000000;
        return;
    }
    if (code & 0x2000) {
        s.action = 0x1d000000;
        return;
    }
    s.reactionMovement.command = std::uint16_t{0};
    s.reactionAssistScratch = std::uint64_t{0};
    constexpr auto epsilon = 0.00000011920929f;
    const bool followup = (s.activeMotionFlags & 0x8000000) != 0;
    if (followup && (s.stateFlags & 1) && s.runningCooldownFrames <= epsilon) {
        const std::int32_t offset = 0x2f4 + 12 * s.nearestSpawnPoint;
        float x, z;
        std::memcpy(&x, reinterpret_cast<const std::byte *>(&game) + offset, sizeof(x));
        std::memcpy(&z, reinterpret_cast<const std::byte *>(&game) + offset + 4, sizeof(z));
        const auto angle = static_cast<std::int16_t>(FixedAtan2(x - s.position.x, z - s.position.z)) - static_cast<std::int16_t>(s.temporaryFacingAngle);
        if (std::abs(angle) < 8192) {
            s.action = 0x17000000;
            return;
        }
    }
    if (!followup || !(s.motionFlags & 4)) {
        if ((code & 0x4000) && !(game.mode & 0xc103b) && (!playerInput || s.runningCooldownFrames <= epsilon)) {
            if (playerInput) {
                float timer = s.runningInputFrames;
                if (static_cast<std::int32_t>(timer) < 65000 && !(s.hitStateFlags & 0x400))
                    timer += shenGetScaledDeltaTickTime(1.0f);
                s.runningInputFrames = timer;
                if (static_cast<std::int32_t>(timer) > 300) {
                    s.reactionMovement.state.mode = std::uint8_t{2};
                    s.runningInputFrames = 0.0f;
                    s.runningCooldownFrames = 60.0f;
                    s.action = 0x1c000000;
                    return;
                }
            }
            s.reactionMovement.state.mode = std::uint8_t{3};
            s.action = 0x1c000000;
            return;
        }
        if (followup && !(s.motionFlags & 0x404)) {
            s.reactionMovement.state.mode = std::uint8_t{2};
            s.action = 0x1c000000;
            return;
        }
        const auto distance = game.contactRange - ((s.hitStateFlags & 0x400) && followup ? 0.30000001f : 0);
        std::uint8_t contact = 0;
        while (contact < 6 &&
            (contact == s.groupIndex || !game.characters[contact] || game.slotTeams[contact] == game.slotTeams[s.groupIndex] ||
                (distance <= s.contactDistances[contact] && camCtrlGetInterpMode() != CameraMotionMode::FirstPerson)))
            ++contact;
        if (contact == 6 && (s.priorityMask & 0xf0) && !(s.secondaryMotionFlags & 0x100)) {
            s.reactionMovement.state.mode = std::uint8_t{1};
            s.action = 0x1c000000;
            return;
        }
        if (s.priorityMask & 0xf0) {
            const std::int16_t angle = (playerInput && !(game.gameFlags & 0x400000) ? s.alternateContactFacing : s.contactFacing);
            s.action = 0x1b000000;
            if (Abs16(angle + 0x8000) <= 8192 && (s.groundContactFlags & 1) && game.contactRange > s.contactDistances[s.actionIndex]) {
                const std::uint8_t tail = s.reactionCursors.endpoint[1];
                s.reactionCursors.tracked[1] = tail;
                s.reactionCursors.tracked[2] = tail;
                s.action = 0x02000000;
            }
            return;
        }
    }
    s.relativeInputSectors[1] = s.relativeInputSectors[0];
    s.action = 0x01000000;
}

namespace {
std::uint16_t MotionMarkerWord(std::span<const std::byte> marker, std::size_t offset) {
    std::uint16_t value;
    std::memcpy(&value, marker.data() + offset, sizeof(value));
    return value;
}
void BindPairedActor(VFMA_t &game, std::uint8_t slot, std::uint8_t partner) {
    auto &self = *game.actors[slot];
    const auto previous = self.pairedCharacter;
    self.pairedSlot = static_cast<std::int8_t>(partner);
    self.pairedActor = game.actors[partner];
    self.pairedCharacter = game.characters[partner];
    if (previous != self.pairedCharacter)
        self.reactionFlags = 0;
}
void SetAuthoredSpawnPosition(VFActor &self) {
    const std::int32_t offset = 0x2f4 + 12 * self.nearestSpawnPoint;
    const auto *game = reinterpret_cast<const std::byte *>(self.game);
    std::memcpy(&self.position.x, game + offset, sizeof(float));
    std::memcpy(&self.position.z, game + offset + 4, sizeof(float));
    self.position.y = flddGetIntersectionFor(self.character, self.position);
}
// Queued/deferred pair actions precede the authored action arbitration.
void ResolveQueuedPairAction(VFActor &self, VFActor &other) {
    if (self.parameter370) {
        self.action = self.parameter370;
        self.deferredReaction = 0;
        if (static_cast<std::uint8_t>(self.action >> 24) == 8)
            other.action = other.deferredReaction = 0;
        return;
    }
    if (self.deferredReaction && self.animationTime >= self.deferredActionFrame && self.deferredActionFrame > self.animationTime - self.scale) {
        self.action = self.deferredReaction;
        self.deferredReaction = 0;
        return;
    }
    const auto state = static_cast<std::uint8_t>(self.action >> 24);
    const auto otherState = static_cast<std::uint8_t>(other.action >> 24);
    if (state == 8 && self.game->hitStopFrames) {
        self.action = 0;
        return;
    }
    if (state == 19) {
        self.deferredActionFrame = other.parameter33c[1];
        self.deferredReaction = self.action;
        self.action = 0;
        return;
    }
    if (state == 20) {
        self.action = 0;
        self.animationTime += self.scale * 3;
        self.parameter4c8 = self.animationTime;
        return;
    }
    if ((state == 8 || state == 17) && otherState <= 19 && (0x8c300U & (1U << otherState)) &&
        (!(self.activeMotionFlags & 0x400) || (self.motionFlags & 0x100) || other.secondActor != &self)) {
        self.action = 0;
        return;
    }
    if ((self.moveFlags & 0x10) && state != 6 && (!(self.secondaryMotionFlags & 0xc00) || (self.motionFlags & 0x100))) {
        self.action = 0;
        return;
    }
    if (!std::strcmp(GetAreaSlotName(3), "CWON.EV3") && (self.action & 0xfff) == 0x644 && self.game->spawnPointCount) {
        SetAuthoredSpawnPosition(self);
        self.moveFlags |= 0x10000000;
    }
    if (self.action)
        self.deferredReaction = 0;
}
// Transfer actual authored partner motion, mirror flags and target basis.
void ResolvePairedHit(VFActor &destination, VFActor &source) {
    BindPairedActor(*source.game, destination.groupIndex, source.groupIndex);
    destination.secondActor = &source;
    const auto motion = static_cast<std::uint16_t>(source.action) & 0xfff;
    const auto style = FindLoadedMotionSecondaryEntry(motion, 26);
    const auto hit = !style.empty() && destination.fightStyle == std::to_integer<std::int8_t>(style[1]) ?
        MotionMarkerWord(style, 4) :
        MotionMarkerWord(FindLoadedMotionSecondaryEntry(motion, 5), 2);
    destination.hitPhase = 0;
    destination.action = static_cast<std::uint64_t>(static_cast<std::int64_t>(static_cast<std::int16_t>(hit))) | 0x09000000;
    destination.knockbackAcceleration = destination.game->knockbackAmount;
    source.moveFlags &= ~0x440U;
    source.hitStateFlags &= ~0x200000U;
    if (static_cast<std::uint8_t>(source.action >> 16) == 1 || !FindLoadedMotionSecondaryEntry(motion, 18).empty()) {
        destination.moveFlags &= ~0x440U;
        destination.hitStateFlags &= ~0x200000U;
    } else {
        source.moveFlags |= destination.moveFlags & 0x440;
        source.hitStateFlags |= destination.hitStateFlags & 0x200000;
    }
    if (source.hitStateFlags & 0xc0000) {
        destination.facingAngleDelta = source.facingAngleDelta = 0;
        return;
    }
    const auto partner = FindLoadedMotionSecondaryEntry(motion, 16);
    if (!partner.empty() && (MotionMarkerWord(partner, 2) & 0xfff) == 0x649 && source.game->spawnPointCount)
        SetAuthoredSpawnPosition(destination);
    source.targetPosition = destination.position;
    source.targetFacingAngle = static_cast<std::int16_t>(destination.facingAngle + destination.facingAngleDelta);
    destination.targetPosition = destination.position;
    destination.targetFacingAngle = source.targetFacingAngle;
    source.moveFlags |= 0x10000000;
    destination.moveFlags |= 0x10000000;
}
// Authored opcode 16 admits the closest same-team third actor.
void ResolvePairedTeammate(VFActor &destination, VFActor &source) {
    const auto marker = FindLoadedMotionSecondaryEntry(static_cast<std::uint16_t>(destination.action) & 0xfff, 16);
    if (marker.empty())
        return;
    const auto motion = MotionMarkerWord(marker, 2);
    if (!motion)
        return;
    VFActor *selected = nullptr;
    float distance = 10000;
    auto &game = *destination.game;
    for (std::uint8_t i = 0; i < 6; ++i) {
        if (game.characters[i] && game.actors[i] != &destination && game.slotTeams[i] == game.slotTeams[destination.groupIndex] &&
            std::fabs(destination.hp) >= 0.00000011920929f && destination.contactDistances[i] < distance) {
            selected = game.actors[i];
            distance = destination.contactDistances[i];
        }
    }
    if (!selected)
        return;
    selected->deferredReaction = 0;
    selected->moveFlags = (selected->moveFlags & ~0x440U) | 0x10000000;
    selected->action = static_cast<std::uint64_t>(motion) | 0x22000000;
    selected->targetPosition = destination.targetPosition;
    selected->targetFacingAngle = destination.targetFacingAngle;
    selected->hitStateFlags &= ~0x200000U;
    if (static_cast<std::uint8_t>(destination.action >> 16) == 1) {
        selected->moveFlags &= ~0x440U;
        selected->hitStateFlags &= ~0x200000U;
    } else {
        selected->moveFlags |= source.moveFlags & 0x440;
        selected->hitStateFlags |= source.hitStateFlags & 0x200000;
    }
}
}


void vfResolvePairInteraction(VFActor *actor) {
    auto &self = *actor;
    if ((static_cast<std::uint8_t>(1U << static_cast<std::uint8_t>(self.pairedSlot)) & self.processedPairs)) {
        self.parameter370 = 0;
        return;
    }
    auto *other = self.pairedActor;
    const bool reciprocal = other->pairedActor == &self;
    other->processedPairs |= static_cast<std::uint8_t>(1U << self.groupIndex);
    self.pairAnimationTime = other->animationTime;
    if (reciprocal)
        other->pairAnimationTime = self.animationTime;
    ResolveQueuedPairAction(self, *other);
    if (reciprocal)
        ResolveQueuedPairAction(*other, self);
    const auto state = static_cast<std::uint8_t>(self.action >> 24);
    const auto otherState = static_cast<std::uint8_t>(other->action >> 24);
    if (state == 17 || (otherState == 17 && reciprocal)) {
        auto &source = state == 17 ? self : *other;
        auto &destination = state == 17 ? *other : self;
        source.pendingAnimation = static_cast<std::int16_t>(source.action);
        source.knockbackAcceleration = source.game->knockbackAmount;
        destination.knockbackAcceleration = destination.game->knockbackAmount;
        vfApplyHitReactionAndStartMotion(&source);
        source.hitReaction = destination.currentAnimation;
        source.pairTimeOffset = static_cast<float>(static_cast<std::uint16_t>(destination.parameter2d0)) - source.eventFrame328;
        destination.guardStrength = source.guardStrength;
        BindPairedActor(*source.game, destination.groupIndex, source.groupIndex);
        destination.secondActor = &source;
        const auto trigger = source.pairTimeOffset - self.scale;
        if (trigger <= destination.animationTime) {
            ResolvePairedHit(destination, source);
            self.parameter370 = other->parameter370 = 0;
            return;
        }
        const bool mirror = destination.moveFlags & 0x400 ? !(source.moveFlags & 0x40) : (source.moveFlags & 0x40) != 0;
        destination.deferredReaction = static_cast<std::uint64_t>(source.hitParameter) + 0x09000000;
        if (mirror)
            destination.deferredReaction |= 0x8000;
        destination.deferredActionFrame = static_cast<std::uint16_t>(static_cast<std::int32_t>(trigger));
    }
    if (state == 19 || (otherState == 19 && reciprocal)) {
        auto &source = state == 19 ? self : *other;
        auto &destination = state == 19 ? *other : self;
        source.action = (source.action & 0xffffff) | 0x08000000;
        mtwkSetExtraFlag(destination.motion, 0, 0);
        ResolvePairedHit(destination, source);
        source.parameter370 = destination.parameter370 = 0;
        destination.reactionContinuation = nullptr;
        SetReactionPc(destination, reinterpret_cast<ReactionPc>(self.motionParameters[0]));
        destination.reactionDelay.count.remaining = std::uint16_t{0};
        destination.reactionDelayActive = std::uint8_t{0};
        return;
    }
    if (state == 8 || otherState == 8) {
        auto *source = &self;
        bool admit = otherState != 8 || (state == 8 && (IncrementSharedPrngUInt16() & 1));
        if (!admit) {
            source = other;
            other = &self;
            admit = reciprocal;
        }
        if (admit) {
            const auto metadata = FindLoadedMotionMetadata(static_cast<std::uint16_t>(other->action) & 0xfff);
            if (!metadata.empty() && (std::to_integer<std::uint8_t>(metadata[8]) & 0x20))
                self.action = 0;
            else {
                ResolvePairedHit(*other, *source);
                ResolvePairedTeammate(*source, *other);
                source->parameter370 = other->parameter370 = 0;
            }
        }
    }
    self.parameter370 = 0;
}

// The category mask comes from the actor's authored move id.
// @verified - p
std::uint32_t vfGetMoveCategoryMaskFor(VFActor *actor) {
    return (actor->hitStateFlags & 0x100) ? moveCategories[actor->moveId] : 0;
}


std::uint8_t vfSelectHitReactionSequence(VFActor *actor) {
    return HitReactionCategory(*actor);
}
} // namespace shenmue::s2
