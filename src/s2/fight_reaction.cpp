#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/fight_stage.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/save_load_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/pad_control_dc.hpp"
#include "s2/frame_dc.hpp"
#include "s2/pause.hpp"
#include "s2/math.hpp"
#include "platform/dc/sdk.hpp"
#include <cmath>
#include <cstring>
#else
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
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

constexpr std::int16_t sectorAngles[16]{6656, 8192, 9472, 12288, 12288, 13056, 13056, 13824, 13824, 14592, 14592, 16896, 16896, 17664, 17664, 17664};

constexpr std::uint8_t sectorChances[16]{0, 0, 0, 8, 8, 8, 19, 38, 48, 72, 80, 88, 96, 104, 112, 120};

// Native g_vfMoveTrainingGainBanks points to three independent forty-word transient gain banks.
// These are not the saved proficiency or equipment banks.
std::uint16_t moveTrainingGains[3][40]{};

constexpr std::uint8_t moveStarts[]{0, 0, 32, 64, 96};

constexpr std::int8_t moveCounts[]{0, 28, 23, 22};

constexpr std::uint32_t moveCategories[44]{0, 8, 64, 16, 128, 48, 384, 2048, 16384, 6144, 49152, 6144, 49152, 432, 7168, 57344, 2, 4, 514, 6, 514, 55296, 24,
    192, 26, 194, 56, 448, 55296, 18432, 6192, 49536, 66, 10, 9730, 76, 20480, 34816, 55296, 432, 504, 0xffffffffU, 65026, 1022};

constexpr std::uint8_t nearbyChoices[4]{201, 202, 201, 202};

constexpr std::uint8_t retreatChoices[4]{204, 204, 201, 202};

constexpr std::uint8_t guardChoices[3]{219, 220, 221};

constexpr std::uint8_t hitChoices[8]{204, 204, 201, 202, 202, 201, 217, 217};

constexpr std::uint16_t cameraSectors[8]{0x20, 0xa0, 0x80, 0x90, 0x10, 0x50, 0x40, 0x60};

struct VFAIBehaviour {
    std::int8_t (*update)(VFActor *, VFSlotRuntimeState *);
    void (*post)(VFActor *, VFSlotRuntimeState *);
};

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
namespace {

template <std::uint8_t Index> std::int8_t UpdatePredicate(VFActor *actor, VFSlotRuntimeState *runtime);
template <std::uint8_t Index> void PostCallback(VFActor *actor, VFSlotRuntimeState *runtime);
std::uint16_t MotionMarkerWord(std::span<const std::byte> marker, std::size_t offset);
void BindPairedActor(VFMA_t &game, std::uint8_t slot, std::uint8_t partner);
void SetAuthoredSpawnPosition(VFActor &self);
void ResolveQueuedPairAction(VFActor &self, VFActor &other);
void ResolvePairedHit(VFActor &destination, VFActor &source);
void ResolvePairedTeammate(VFActor &destination, VFActor &source);

} // namespace

void vfResetReactionProgram(VFActor *self);
void vfUpdateMain(VFActor *self);
void vfResetMoveTrainingGains();
void vfConsumeMoveTrainingGains();
std::int16_t vfGetFacingDeltaFromCameraSectorCode(VFActor *self, std::uint16_t code);
void vfmaUpdateNextReaction(VFActor *actor);
void vfResolvePairInteraction(VFActor *actor);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

constexpr VFAIBehaviour aiBehaviours[58]{{UpdatePredicate<0>, PostCallback<0>}, {UpdatePredicate<1>, PostCallback<1>}, {UpdatePredicate<2>, PostCallback<2>},
    {UpdatePredicate<3>, PostCallback<3>}, {UpdatePredicate<4>, PostCallback<4>}, {UpdatePredicate<5>, PostCallback<5>}, {UpdatePredicate<6>, PostCallback<6>},
    {UpdatePredicate<7>, nullptr}, {UpdatePredicate<8>, PostCallback<8>}, {UpdatePredicate<9>, nullptr}, {UpdatePredicate<10>, PostCallback<10>},
    {UpdatePredicate<11>, PostCallback<11>}, {UpdatePredicate<12>, PostCallback<12>}, {UpdatePredicate<13>, PostCallback<13>}, {UpdatePredicate<14>, nullptr},
    {UpdatePredicate<15>, PostCallback<15>}, {UpdatePredicate<16>, PostCallback<16>}, {UpdatePredicate<17>, PostCallback<17>}, {UpdatePredicate<18>, nullptr},
    {UpdatePredicate<19>, nullptr}, {nullptr, nullptr}, {UpdatePredicate<21>, PostCallback<21>}, {UpdatePredicate<22>, PostCallback<22>},
    {UpdatePredicate<23>, PostCallback<23>}, {UpdatePredicate<24>, PostCallback<24>}, {UpdatePredicate<25>, PostCallback<25>},
    {UpdatePredicate<26>, PostCallback<26>}, {UpdatePredicate<27>, PostCallback<27>}, {UpdatePredicate<28>, PostCallback<28>},
    {UpdatePredicate<29>, PostCallback<29>}, {UpdatePredicate<6>, PostCallback<6>}, {UpdatePredicate<31>, PostCallback<31>},
    {UpdatePredicate<32>, PostCallback<32>}, {UpdatePredicate<33>, PostCallback<33>}, {UpdatePredicate<34>, PostCallback<34>},
    {UpdatePredicate<22>, PostCallback<22>}, {nullptr, nullptr}, {nullptr, nullptr}, {nullptr, nullptr}, {nullptr, nullptr},
    {UpdatePredicate<40>, PostCallback<40>}, {UpdatePredicate<40>, PostCallback<40>}, {UpdatePredicate<42>, nullptr}, {nullptr, nullptr},
    {UpdatePredicate<40>, PostCallback<40>}, {UpdatePredicate<45>, PostCallback<45>}, {nullptr, nullptr}, {nullptr, nullptr}, {nullptr, nullptr},
    {nullptr, nullptr}, {UpdatePredicate<40>, PostCallback<40>}, {UpdatePredicate<40>, PostCallback<40>}, {nullptr, nullptr},
    {UpdatePredicate<53>, PostCallback<53>}, {UpdatePredicate<40>, nullptr}, {UpdatePredicate<55>, nullptr}, {nullptr, nullptr},
    {UpdatePredicate<57>, PostCallback<57>}};

constexpr std::int16_t cameraYawAngles[16]{-32767, 0, -32767, -32767, 16384, 8192, 24576, 24576, -16383, -8191, -24575, -24575, -32767, 0, -32767, -32767};

using ReactionPc = const std::uint8_t *;

constexpr std::uint16_t inputResponses[2][16]{
    {0, 8, 1, 0, 0, 0, 16384, 16384, 0, 4, 2, 0, 32, 16, 128, 64}, {0, 8, 1, 0, 0, 0, 16384, 0, 0, 4, 2, 0, 32, 16, 128, 64}};

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::uint8_t AuthoredByte(const VFActor &self, std::size_t offset) {
    return std::to_integer<std::uint8_t>(self.motionParameters[2][offset]);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool PhaseSelected(const VFSlotPhaseConfig &phase) {
    return phase.enabled && (phase.reserved ? phase.countdown != phase.selected : phase.countdown == phase.selected);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool Chance(std::uint8_t value, std::uint8_t random) {
    return value && value >= random;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::int8_t GuardProximity(VFActor &self, VFSlotRuntimeState &slot) {
    if ((self.actor->hitStateFlags & (VFHSF_Unknown4 | VFHSF_AttackActive)) != 0x100 || (self.hitStateFlags & VFHSF_LightReaction) ||
        (slot.cameraFlags & 0x2000) || (self.activeMotionFlags & VFSMF_Unknown4000) || self.hitPhaseCounters[0] > 1 ||
        self.contactDistances[self.actionIndex] > 2.5f)
        return -1;
    return 5;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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
    if ((self.savedMotionFlags & VFMOF_Unknown8000) && slot.randomBytes[0] <= 128)
        return 5;
    return PhaseSelected(slot.phases[phase]) ? 5 : -1;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::int8_t HitPhase(VFActor &self, VFSlotRuntimeState &slot) {
    const auto &other = *g_pFightMain->actors[self.actionIndex];
    if ((static_cast<std::uint16_t>(other.parameter2d0) > 4 &&
            other.parameter2d0 - static_cast<std::int16_t>(static_cast<std::int32_t>(other.animationTime)) > 4) ||
        !PhaseSelected(slot.phases[4]))
        return -1;
    return 22;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::int8_t AttackChance(VFActor &self, VFSlotRuntimeState &slot) {
    if (!(self.reactionFlags & 1) ||
        (self.hitStateFlags &
            (VFHSF_FallLandingRequired | VFHSF_AttackActive | VFHSF_LightReaction | VFHSF_SpecialGuardOrParry | VFHSF_SpecialThrow | VFHSF_GuardState)) ||
        (self.actor->hitStateFlags &
            (VFHSF_Unknown8 | VFHSF_FallLandingRequired | VFHSF_AttackActive | VFHSF_LightReaction | VFHSF_SpecialGuardOrParry | VFHSF_SpecialThrow |
                VFHSF_GuardState)) ||
        slot.hitTimer > 0)
        return -1;
    if (static_cast<std::int32_t>(slot.opponentMotionTimer) > 128 && slot.parameter31)
        return 6;
    return Chance(static_cast<std::uint8_t>(slot.parameter31), slot.randomBytes[0]) ? 6 : -1;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::int8_t PairGuard(VFActor &self, VFSlotRuntimeState &slot) {
    auto &other = *self.actor;
    if ((self.motionFlags & VFMOF_Unknown80000000) && (other.motionFlags & VFMOF_Unknown100) && other.attackReach > self.contactDistances[self.actionIndex] &&
        other.actionIndex == self.groupIndex)
        return 26;
    if (!(other.hitStateFlags & (VFHSF_AttackActive | VFHSF_ForcedReaction)) || (other.hitStateFlags & VFHSF_Unknown4) ||
        (self.hitStateFlags & VFHSF_LightReaction) || !(other.hitStateFlags & VFHSF_AttackActive) || self.contactDistances[self.actionIndex] > 2 ||
        !PhaseSelected(slot.phases[3]))
        return -1;
    return 26;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::uint8_t MoveCategory(std::uint8_t move) {
    std::uint8_t category = 1;
    while (move >= moveStarts[category + 1]) {
        if (++category >= 4)
            break;
    }
    return category;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// Each instantiation is one actual native table entry, including predicates which
// intentionally select a different logical slot. Absent native entries stay null.
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
        if (r.guardTimer > 0 || (s.groundContactFlags & 1) ||
            (s.hitStateFlags &
                (VFHSF_FallLandingRequired | VFHSF_LightReaction | VFHSF_SpecialGuardOrParry | VFHSF_SpecialThrow | VFHSF_GuardState | VFHSF_MidAirRecovery |
                    VFHSF_AirborneOrDowned)) ||
            (o.hitStateFlags & 0xdd010) || d > 3)
            return -1;
        return Chance(static_cast<std::uint8_t>(r.parameter2a), r.randomBytes[0]) ? 1 : -1;
    }
    if constexpr (Index == 2) {
        const float previous = s.assistHistory.currentWindow;
        s.assistHistory.currentWindow = 0.0f;
        if ((s.hitStateFlags & (VFHSF_FallLandingRequired | VFHSF_LightReaction | VFHSF_SpecialGuardOrParry | VFHSF_SpecialThrow | VFHSF_GuardState)) ||
            o.pairContactPriority || ((s.secondaryMotionFlags & VFSMF_Unknown8000000) && (s.motionFlags & VFMOF_Unknown4)))
            return -1;
        if ((previous == 0 ? r.assistWindowCurrent + 0.5f : r.assistWindowCurrent - 0.30000001f) > d)
            return -1;
        if ((s.motionFlags & VFMOF_Unknown80000000) && (!(o.motionFlags & 0x100) || !(o.secondaryMotionFlags & 0x80000))) {
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
        return (s.groundContactFlags & 1) &&
                !(s.hitStateFlags & (VFHSF_FallLandingRequired | VFHSF_LightReaction | VFHSF_SpecialGuardOrParry | VFHSF_SpecialThrow | VFHSF_GuardState)) &&
                !o.pairContactPriority ?
            Index :
            -1;
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
        if (!(s.hitStateFlags & VFHSF_AirborneOrDowned) || (r.cameraFlags & 0x4000) || !s.secondActor || !s.secondActor->pairContactPriority)
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
        if ((s.slot->parameter1a5 != 1 || !(s.savedSecondaryMotionFlags & VFSMF_Unknown4)) && r.parameter128 < 32) {
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
        return !(s.hitStateFlags & VFHSF_ForcedReaction) ? 10 : -1;
    if constexpr (Index == 11)
        return (s.reactionFlags & 8) && Chance(static_cast<std::uint8_t>(r.parameter31), r.randomBytes[0]) ? 11 : -1;
    if constexpr (Index == 12)
        return !(o.hp > static_cast<double>(o.initialHp) * 0.25) && !(d > 1.25f) &&
                !(s.hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_SpecialThrow | VFHSF_GuardState)) && !(o.hitStateFlags & 0x1d010) &&
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
        const auto chance = std::min(255, static_cast<std::uint8_t>(r.parameter2a) + std::to_integer<std::uint8_t>(r.reserved2b[4]));
        return Chance(static_cast<std::uint8_t>(chance), r.randomBytes[0]) ? 1 : -1;
    }
    if constexpr (Index == 15)
        return (s.hitStateFlags & VFHSF_AirborneOrDowned) && s.secondActor && s.secondActor->pairContactPriority &&
                ((AuthoredByte(s, 25) & 2) || (PhaseSelected(r.phases[5]) && (s.slot->parameter1a5 != 1 || !(s.savedSecondaryMotionFlags & VFSMF_Unknown4)))) ?
            15 :
            -1;
    if constexpr (Index == 16) {
        if (o.actionIndex == s.groupIndex && (o.hitStateFlags & 0x100)) {
            if (d > s.slot->assistWindowDefault && !(o.groundContactFlags & 1))
                return 16;
            const auto multiplier = static_cast<std::int8_t>(o.motionFlags) >= 0 && (o.previousHitStateFlags & 0x100) ? 2 : 1;
            const auto chance =
                std::min(255, std::to_integer<std::uint8_t>(s.slot->reserved2b[0]) + std::to_integer<std::uint8_t>(s.slot->reserved2b[4]) * multiplier);
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
        return static_cast<std::uint32_t>(static_cast<std::int32_t>(s.game->frame)) >= frame && (s.reactionFlags & 1) &&
                !(s.hitStateFlags &
                    (VFHSF_FallLandingRequired | VFHSF_AttackActive | VFHSF_LightReaction | VFHSF_SpecialGuardOrParry | VFHSF_SpecialThrow |
                        VFHSF_GuardState)) &&
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
        if (!(o.motionFlags & 0x100) || ((o.hitStateFlags & 0x100) && d > o.attackReach + 0.5f))
            return -1;
        return GuardProximity(s, r) != -1 ? 5 : -1;
    }
    if constexpr (Index == 21)
        return (s.reactionFlags & 0x6000000) && Chance(static_cast<std::uint8_t>(r.parameter31), r.randomBytes[0]) ? 21 : -1;
    if constexpr (Index == 22 || Index == 35)
        return (s.hitFlags & 0x2400) && HitPhase(s, r) != -1 &&
                !((s.moveFlags & VFMF_NoContactPairing) && (o.guardState == 3 || o.animationPhaseWord == 127 || !(o.activeMotionFlags & 0x10000000))) ?
            22 :
            -1;
    if constexpr (Index == 23)
        return (s.hitFlags & 0x5000) && HitPhase(s, r) != -1 ? 23 : -1;
    if constexpr (Index == 24)
        return (s.hitFlags & 0x8800) && HitPhase(s, r) != -1 ? 24 : -1;
    if constexpr (Index == 25) {
        const auto index = vfMotnNibbleGet(67);
        const auto angle = static_cast<std::int16_t>(s.facingAngle - s.contactAngles[s.actionIndex]);
        if ((s.secondaryMotionFlags & VFSMF_Unknown10) ||
            (s.motionFlags & (VFMOF_Unknown40 | VFMOF_ForceNormalHitResolution | VFMOF_Unknown40000 | VFMOF_Unknown80000 | VFMOF_DelayGuardResolution)) ||
            (s.secondaryMotionFlags & (VFSMF_Unknown4 | VFSMF_Unknown200000)))
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
        return !(s.moveFlags & VFMF_NoContactPairing) &&
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
            if (!(d > o.attackReach)) {
                if (r.animationPhase == 1)
                    return -1;
                float remaining = o.attackReach - (o.attackReach / static_cast<std::uint16_t>(o.parameter2d0)) * o.animationTime;
                if (remaining < 0)
                    remaining = 0;
                const float distance = d - remaining;
                if (!(o.attackReach > distance && distance > 1)) {
                    if (!(o.moveFlags & 1) && !(s.activeMotionFlags & VFSMF_Unknown2000000) &&
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
                static_cast<std::int16_t>(r.facingDifferences[s.actionIndex]) <= 24576 && !o.pairContactPriority &&
                !(s.hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_SpecialThrow | VFHSF_GuardState | VFHSF_MidAirRecovery | VFHSF_AirborneOrDowned)) &&
                !((o.hitStateFlags & 0x100) && static_cast<double>(d) < 2.7) ?
            31 :
            -1;
    if constexpr (Index == 32)
        return (s.hitStateFlags & VFHSF_CameraFocusTarget) && !(static_cast<double>(d) > static_cast<double>(s.assistHistory.currentWindow) + 0.2) &&
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
        return !(o.motionFlags & 0x100) && !(o.secondaryMotionFlags & 0x80000) && !(s.motionFlags & VFMOF_Unknown80000000) ? 34 : -1;
    }
    if constexpr (Index == 42)
        return !(s.hitStateFlags & VFHSF_MidAirRecovery) && (o.hitStateFlags & 0x100) && !(s.activeMotionFlags & VFSMF_Unknown2000000) &&
                !(r.cameraFlags & 0x2000) &&
                static_cast<std::uint16_t>(static_cast<std::int32_t>(o.animationTime + 2)) >= static_cast<std::uint16_t>(o.parameter2d0) && !(d > 2.5f) ?
            5 :
            -1;
    if constexpr (Index == 45) {
        const auto authored = AuthoredByte(s, 110);
        if (authored == 3 && static_cast<std::int32_t>(s.game->frame) > 300) {
            const auto &slot = *s.slot;
            if (!((s.secondaryMotionFlags & VFSMF_Unknown2) ||
                    ((s.reactionFlags & 1) &&
                        !(s.hitStateFlags &
                            (VFHSF_FallLandingRequired | VFHSF_AttackActive | VFHSF_LightReaction | VFHSF_SpecialGuardOrParry | VFHSF_SpecialThrow |
                                VFHSF_GuardState)) &&
                        !(o.hitStateFlags & 0x1d118))) ||
                slot.hitTimer > 0)
                return -1;
            if (static_cast<std::int32_t>(slot.opponentMotionTimer) > 128 && slot.parameter31)
                return 6;
            return Chance(static_cast<std::uint8_t>(slot.parameter31), slot.randomBytes[0]) ? 6 : -1;
        }
        return authored == 4 && static_cast<std::int8_t>(s.reactionRank) <= 0 ? 45 : -1;
    }
    if constexpr (Index == 53)
        return !(s.groundContactFlags & 1) && !o.pairContactPriority &&
                !(s.hitStateFlags &
                    (VFHSF_FallLandingRequired | VFHSF_LightReaction | VFHSF_SpecialGuardOrParry | VFHSF_SpecialThrow | VFHSF_GuardState |
                        VFHSF_CameraFocusTarget)) &&
                !(s.activeMotionFlags & VFSMF_Unknown8000000) && g_pFightStage->unknownGroupCounts[g_pFightMain->enemySlots[s.groupIndex]->counterGroup] >= 2 &&
                !(o.hitStateFlags & 0x20000000) && !(d > r.assistWindowCurrent - 0.2f) && !(r.assistWindowCurrent - 0.8f > d) ?
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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::uint8_t HitReactionCategory(const VFActor &actor) {
    if (!(actor.hitStateFlags & VFHSF_AttackActive))
        return 0;
    const auto mask = moveCategories[actor.moveId];
    return ((mask & 0x1c0) ? 4 : 0) | ((mask & 0x38) ? 8 : 0) | ((mask & 0xe000) ? 16 : 0) | ((mask & 0x1c00) ? 32 : 0) | ((mask & 3) ? 2 : 0) |
        ((mask & 4) ? 1 : 0);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void ResetPhase(VFSlotRuntimeState &slot) {
    slot.parameter89 = 0;
    slot.parameter8c = 0;
    slot.parameterCc = 0;
    slot.parameter12a = 0;
    slot.resetIndex = 0x7f7f;
    slot.cameraFlags = (slot.cameraFlags & ~0x2001U) | 1;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::uint8_t RandomAuthoredChoice(VFActor &self, std::size_t offset, float count) {
    return AuthoredByte(self, offset + static_cast<std::uint8_t>(static_cast<std::int32_t>(IncrementSharedPrngFloat() * count)));
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void SelectPhase(VFActor &self, std::uint8_t choice, std::uint16_t delay = 0) {
    vfSelectAICommand(&self, choice, delay);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::uint8_t RandomIndex(float count) {
    return static_cast<std::uint8_t>(static_cast<std::int32_t>(IncrementSharedPrngFloat() * count));
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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
        if (!(self.hitStateFlags & VFHSF_AttackGuardWindow))
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
            self.activeMotionFlags &= ~VFSMF_Unknown20;
        else
            other.activeMotionFlags |= VFSMF_Unknown20;
        const auto chance = static_cast<std::uint8_t>(19 * (setting - 2));
        if (Chance(chance, slot.randomBytes[1])) {
            SelectPhase(self, static_cast<std::uint8_t>((slot.randomBytes[0] > 127) + 2), delay);
            return;
        }
    }
    SelectPhase(self, RandomIndex(static_cast<float>(AuthoredByte(self, 24))), delay);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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
        s.inputFlags = 0x2000;
        if (s.reactionMovement.state.mode == 1 && (s.contactDistances[r.opponentIndex] > 5 || (other.hitStateFlags & VFHSF_CameraFocusTarget)))
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
        s.inputFlags = (static_cast<std::int32_t>(g_pFightMain->frame) & 2) && !(static_cast<std::int32_t>(g_pFightMain->previousFrame) & 2) ? 32 : 16;
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
                    s.inputFlags = 0x2000;
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
        if ((other.hitStateFlags & VFHSF_AttackGuardWindow) || (other.activeMotionFlags & VFSMF_Unknown80000) ||
            other.parameter2d0 - static_cast<std::int16_t>(static_cast<std::int32_t>(other.animationTime)) >= 5)
            SelectPhase(s, RandomAuthoredChoice(s, static_cast<double>(s.contactDistances[s.actionIndex]) > 2.2 ? 104 : 102, 2));
    }
    if constexpr (Index == 15) {
        ResetPhase(*s.slot);
        s.inputFlags = r.reactionTicks > 0.00000011920929f && static_cast<std::uint16_t>(static_cast<std::int32_t>(r.reactionTicks) - 1) <= 1 ? 4 : 0;
    }
    if constexpr (Index == 16) {
        if (!(s.activeMotionFlags & VFSMF_Unknown2000000)) {
            ResetPhase(*s.slot);
            vfResetReactionProgram(&s);
        }
        if ((static_cast<std::int32_t>(s.game->frame) & 1) && !(static_cast<std::int32_t>(s.game->previousFrame) & 1))
            SelectPhase(
                s, s.actor->guardState == 3 || s.actor->guardState == 0 || (s.activeMotionFlags & VFSMF_Unknown2000000) ? guardChoices[RandomIndex(3)] : 217);
    }
    if constexpr (Index == 17 || Index == 25)
        SelectPhase(s, 209);
    if constexpr (Index == 21)
        SelectPhase(s, RandomAuthoredChoice(s, 87, 2), r.parameter28);
    if constexpr (Index == 22 || Index == 35 || Index == 23 || Index == 24) {
        if (Index == 24 ? (s.moveFlags & VFMF_Unknown100) != 0 : !(s.moveFlags & VFMF_Unknown100)) {
            SelectPhase(s, Index == 23 ? 206 : Index == 24 ? 207 : 205);
            r.cameraFlags |= 0x2000;
        }
    }
    if constexpr (Index == 26) {
        const auto &other = *g_pFightMain->actors[s.actionIndex];
        ResetPhase(*s.slot);
        if (!(static_cast<std::int32_t>(s.game->frame) & 1)) {
            s.inputFlags = 0;
            return;
        }
        std::uint16_t code = 24;
        const auto direction = static_cast<std::int16_t>(other.hitDirection);
        if (direction > 0)
            code = s.leftIntersectionDistance > 3 ? 136 : 72;
        else if (direction < 0)
            code = !(s.rightIntersectionDistance > 3) ? 136 : 72;
        s.inputFlags = other.eventFrame30a &&
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
        if ((other.activeMotionFlags & VFSMF_Unknown10000000) && other.guardState == 3)
            index = 6;
        else if ((other.activeMotionFlags & VFSMF_Unknown10000000) && (other.guardState == 2 || static_cast<std::uint8_t>(other.guardState - 5) <= 1))
            index = 0;
        else if (category & 0x24)
            index = 2;
        else if (category & 0x18)
            index = 4;
        else
            index = (other.activeMotionFlags & VFSMF_AlternateFootTarget) ? 2 : 0;
        const std::uint32_t direction = other.hitDirection;
        if (direction - 1821 <= 0xf1c5 && (((category & 0x24) && direction > 0x8000) || ((category & 0x18) && direction < 0x8000)))
            ++index;
        SelectPhase(s, hitChoices[index]);
    }
    if constexpr (Index == 29)
        SelectPhase(s, nearbyChoices[RandomIndex(4)]);
    if constexpr (Index == 31) {
        ResetPhase(*s.slot);
        s.inputFlags = 32;
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
            if (s.actor->activeMotionFlags & VFSMF_Unknown800)
                bytes[0x2af + target] = (static_cast<std::int16_t>(s.actor->facingAngle) - s.actor->contactAngles[target] >= 0) + 1;
        }
        s.inputFlags = bytes[0x2af + target] == 2 ? 4160 : 4224;
    }
    if constexpr (Index == 57) {
        const auto ticks = static_cast<std::int32_t>(r.ticks), previous = static_cast<std::int32_t>(r.previousTicks);
        if ((ticks & 32) || !(previous & 32)) {
            s.inputFlags = 0;
            return;
        }
        const auto distance = s.contactDistances[s.actionIndex];
        if (!(ticks & 63) && (previous & 63))
            r.reserved12b = distance > 3 ? 0 : r.assistWindowCurrent - 0.69999999f > distance ? 4 : IncrementSharedPrngUInt16() & 7;
        s.inputFlags = cameraSectors[r.reserved12b];
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

struct FightAiPhase {
    std::uint8_t enabled, countdown, selected, reversed, choices[16];
};

struct FightAiCommandOption {
    std::uint8_t command, delay, followCommand, followDelay;
};

struct FightAiCommandRecord {
    std::uint8_t choice;
    FightAiCommandOption options[4];
};

struct FightAiInputSequence {
    std::uint8_t codes[10];
    std::uint8_t ticks[10];
};

struct FightAiSlot {
    std::uint32_t id;
    FightActorRuntimePrefix *actor;
    std::uint32_t parameter08;
    float assistWindowDefault, assistWindowGuard;
    std::uint16_t facingLimit, turningLimit, hitTimerMaximum, guardTimerMaximum, commandDelay;
    std::uint8_t attackChance, parameter1f, parameter20, parameter21, parameter22, guardChance, parameter24, reactionChance;
    std::byte retained26[2];
    float assistWindowCurrent;
    std::uint32_t flags;
    float healthDifferences[8];
    std::uint8_t random[2];
    std::uint16_t randomWord;
    std::uint8_t queuedCommands[3], queuedDelays[3], opponent;
    std::byte retained5b[5];
    std::uint8_t reaction, previousReaction;
    std::uint16_t ticks, facingDifferences[6];
    std::byte retained70[5];
    std::uint8_t command, step, retained77;
    std::uint16_t inputTicks, input, animationHistory[4], inputHistory[3], pressure, repeatedAnimation;
    std::uint8_t repeatedInputs, animationHistoryIndex, inputHistoryIndex;
    std::byte retained91[9];
    std::int8_t parameter9a;
    std::byte retained9b[3];
    std::uint16_t hitTimer, guardTimer, parameterA2, guardTicks, opponentGuardTicks, opponentAttackTimer;
    std::uint16_t waitFrames, reactionTicks, motionTicks, hpPhaseTimer, retainedb2, opponentMotionTimer, parameterB6;
    std::uint16_t knownAnimations[32];
    std::uint8_t knownAnimationCount, idleCount, queuedCommandIndex, sector, guardPhase;
    FightAiPhase phases[6];
    std::int8_t parameter175;
    std::byte retained176[6];
    std::int8_t animationPhase;
    std::byte retained17d[3];
    std::uint32_t secondaryMotionTimer, motionTimer;
};

typedef char FightAiSlotSizeCheck[sizeof(FightAiSlot) == 0x188 ? 1 : -1];

typedef char FightAiSlotPhaseCheck[offsetof(FightAiSlot, phases) == 0xfd ? 1 : -1];

typedef char FightAiSlotInputCheck[offsetof(FightAiSlot, inputTicks) == 0x78 ? 1 : -1];

struct FightAiBehaviour {
    std::int8_t (*predicate)(FightActorRuntimePrefix *, FightAiSlot *);
    void (*post)(FightActorRuntimePrefix *, FightAiSlot *);
};

typedef const std::uint8_t *FightReactionPc;

struct FightReactionInput {
    std::uint16_t mask, sector, pressed, released;
};

struct FightReactionContext {
    FightReactionPc program;
    std::int32_t scanRemaining;
    std::uint32_t priorityOnly;
    std::uint16_t pressed, mask, snapshot, allowed, remaining;
    std::uint8_t history, retained17;
};

struct FightReactionMachine {
    std::uint8_t depth, history, stop, retained03;
    std::uint16_t snapshot, pressed;
    std::int32_t scanRemaining;
    std::uint32_t released, retained10;
    std::uint16_t releasedFlags, filterCurrent, filterPrevious, remaining, allowed, retained1e;
    FightReactionContext contexts[16];
    std::uint32_t condition, retained1a4;
    std::int8_t chainFrames;
    std::uint8_t counters[3];
    std::byte retained1ac[4];
};

typedef char FightReactionContextSizeCheck[sizeof(FightReactionContext) == 0x18 ? 1 : -1];

typedef char FightReactionMachineSizeCheck[sizeof(FightReactionMachine) == 0x1b0 ? 1 : -1];

typedef char FightReactionMachineContextsCheck[offsetof(FightReactionMachine, contexts) == 0x20 ? 1 : -1];

struct FightReactionActorView {
    std::byte retained000[0x492];
    std::uint16_t inputMask, retained494, inputPressed;
    FightReactionInput history[32];
    std::byte retained598[0x38];
    FightReactionMachine machine;
};

typedef char FightReactionActorViewMachineCheck[offsetof(FightReactionActorView, machine) == 0x5d0 ? 1 : -1];

struct FightAiActorView {
    std::int8_t groupIndex, target;
    std::byte retained002[0x16a];
    std::int16_t targetAngles[6];
    std::byte retained178[0x112];
    std::uint16_t retained28a;
    std::byte retained28c[4];
    std::int16_t impactFrame;
    std::byte retained292[0x17];
    std::uint8_t guardState;
    std::byte retained2aa[0x714];
    std::int8_t focusTarget;
};

typedef char FightAiActorViewAnglesCheck[offsetof(FightAiActorView, targetAngles) == 0x16c ? 1 : -1];

typedef char FightAiActorViewImpactCheck[offsetof(FightAiActorView, impactFrame) == 0x290 ? 1 : -1];

typedef char FightAiActorViewGuardCheck[offsetof(FightAiActorView, guardState) == 0x2a9 ? 1 : -1];

typedef char FightAiActorViewFocusCheck[offsetof(FightAiActorView, focusTarget) == 0x9be ? 1 : -1];

struct FightVmInputView {
    std::byte retained000[0x498];
    FightReactionInput history[32];
    std::uint8_t head;
    std::uint8_t latchSource;
    std::uint8_t tracked[3];
    std::uint8_t latches[2];
    std::uint8_t latchMirror;
    std::uint8_t pending[2];
    std::uint16_t inputs[2];
};

typedef char FightVmInputViewCheck[offsetof(FightVmInputView, inputs) == 0x5a2 ? 1 : -1];

#define SHENMUE_VM_INPUT(self) reinterpret_cast<FightVmInputView *>(self)

struct FightAiGameView {
    std::byte retained000[0x1c2];
    std::uint8_t slotTargets[6];
    std::byte retained1c8[0x0b];
    std::uint8_t targetSides[6];
};

typedef char FightAiGameViewTargetsCheck[offsetof(FightAiGameView, slotTargets) == 0x1c2 ? 1 : -1];

typedef char FightAiGameViewSidesCheck[offsetof(FightAiGameView, targetSides) == 0x1d3 ? 1 : -1];

typedef std::uint8_t (*FightReactionCallback)(FightActorRuntimePrefix *);

template <int Index> std::int8_t AiPredicate(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<10>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<16>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<25>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<33>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<34>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<45>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<53>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<55>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<57>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<0>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<1>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<2>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<3>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<4>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<5>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<7>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<8>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<9>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<11>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<12>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<13>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<14>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<15>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<17>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<18>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<19>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<21>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<22>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<23>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<24>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<27>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<28>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<29>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<30>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<31>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<32>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<35>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<40>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<41>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<42>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<44>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<50>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<51>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> std::int8_t AiPredicate<54>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

std::int8_t AiAttackChance(FightActorRuntimePrefix *self, FightAiSlot *runtime);

std::int8_t AiPairGuard(FightActorRuntimePrefix *self, FightAiSlot *runtime);

std::uint16_t VmRotateSector(FightActorRuntimePrefix *actor, std::uint16_t code);

std::uint16_t AiAdjustPlayerSector(FightActorRuntimePrefix *actor, std::uint16_t code);

template <int Index> void AiPost(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<0>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<1>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<2>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<3>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<4>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<5>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<6>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<8>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<10>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<11>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<12>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<13>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<15>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<17>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<21>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<22>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<23>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<24>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<25>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<26>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<27>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<28>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<29>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<30>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<31>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<32>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<33>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<34>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<40>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<41>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<44>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<45>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<50>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<51>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<53>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <> void AiPost<57>(FightActorRuntimePrefix *self, FightAiSlot *runtime);

template <int Opcode> std::uint8_t VmCallback(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<0>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<1>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<2>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<3>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<4>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<5>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<6>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<7>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<8>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<9>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<10>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<11>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<12>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<13>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<14>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<15>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<16>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<17>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<18>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<19>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<20>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<21>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<22>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<23>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<24>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<25>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<26>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<27>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<28>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<29>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<30>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<31>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<32>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<33>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<34>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<35>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<36>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<37>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<38>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<39>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<40>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<41>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<42>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<43>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<44>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<45>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<46>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<47>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<48>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<49>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<50>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<51>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<52>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<53>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<54>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<55>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<56>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<57>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<58>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<59>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<60>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<61>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<62>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<63>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<64>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<65>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<66>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<67>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<68>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<69>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<70>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<71>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<72>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<73>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<74>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<75>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<76>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<77>(FightActorRuntimePrefix *self);

template <> std::uint8_t VmCallback<78>(FightActorRuntimePrefix *self);

} // namespace

namespace {

FightRuntimePrefix *fightAiGame;

#pragma pack(push, 1)

#pragma pack(pop)

static const std::uint16_t aiCameraSectors[8] = {0x20, 0xa0, 0x80, 0x90, 0x10, 0x50, 0x40, 0x60};

static const FightAiInputSequence aiDefaultCommands[19] = {{0, 127, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {72, 0, 127, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0}, {136, 0, 127, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0},
    {40, 0, 127, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0}, {24, 0, 127, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0},
    {19, 0, 127, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {19, 0, 127, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {19, 0, 127, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {7, 0, 127, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {15, 0, 127, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {4, 127, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {4, 127, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 127, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {240, 241, 127, 0, 0, 0, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {240, 242, 127, 0, 0, 0, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {240, 243, 127, 0, 0, 0, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {240, 250, 127, 0, 0, 0, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {8, 0, 127, 0, 0, 0, 0, 0, 0, 0, 1, 10, 0, 0, 0, 0, 0, 0, 0, 0}, {5, 0, 127, 0, 0, 0, 0, 0, 0, 0, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0}};

static const FightAiBehaviour aiBehaviours[58] = {{AiPredicate<0>, AiPost<0>}, {AiPredicate<1>, AiPost<1>}, {AiPredicate<2>, AiPost<2>},
    {AiPredicate<3>, AiPost<3>}, {AiPredicate<4>, AiPost<4>}, {AiPredicate<5>, AiPost<5>}, {AiAttackChance, AiPost<6>}, {AiPredicate<7>, 0},
    {AiPredicate<8>, AiPost<8>}, {AiPredicate<9>, 0}, {AiPredicate<10>, AiPost<10>}, {AiPredicate<11>, AiPost<11>}, {AiPredicate<12>, AiPost<12>},
    {AiPredicate<13>, AiPost<13>}, {AiPredicate<14>, 0}, {AiPredicate<15>, AiPost<15>}, {AiPredicate<16>, 0}, {AiPredicate<17>, AiPost<17>},
    {AiPredicate<18>, 0}, {AiPredicate<19>, 0}, {0, 0}, {AiPredicate<21>, AiPost<21>}, {AiPredicate<22>, AiPost<22>}, {AiPredicate<23>, AiPost<23>},
    {AiPredicate<24>, AiPost<24>}, {AiPredicate<25>, AiPost<25>}, {AiPairGuard, AiPost<26>}, {AiPredicate<27>, AiPost<27>}, {AiPredicate<28>, AiPost<28>},
    {AiPredicate<29>, AiPost<29>}, {AiPredicate<30>, AiPost<30>}, {AiPredicate<31>, AiPost<31>}, {AiPredicate<32>, AiPost<32>}, {AiPredicate<33>, AiPost<33>},
    {AiPredicate<34>, AiPost<34>}, {AiPredicate<35>, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {AiPredicate<40>, AiPost<40>}, {AiPredicate<41>, AiPost<41>},
    {AiPredicate<42>, 0}, {0, 0}, {AiPredicate<44>, AiPost<44>}, {AiPredicate<45>, AiPost<45>}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {AiPredicate<50>, AiPost<50>},
    {AiPredicate<51>, AiPost<51>}, {0, 0}, {AiPredicate<53>, AiPost<53>}, {AiPredicate<54>, 0}, {AiPredicate<55>, 0}, {0, 0}, {AiPredicate<57>, AiPost<57>}};

static const std::int16_t vmCameraAngles[16] = {-32767, 0, -32767, -32767, 16384, 8192, 24576, 24576, -16383, -8191, -24575, -24575, -32767, 0, -32767, -32767};

} // namespace

std::uint8_t ReactSelectStageObject(FightActorRuntimePrefix *actor);
std::uint8_t ReactHasActiveMotionFlags(FightActorRuntimePrefix *actor);
std::uint8_t ReactLacksActiveMotionFlags(FightActorRuntimePrefix *actor);
std::uint8_t ReactHasMotionFlags(FightActorRuntimePrefix *actor);
std::uint8_t ReactLacksMotionFlags(FightActorRuntimePrefix *actor);
std::uint8_t ReactTargetHealthAtMost(FightActorRuntimePrefix *actor);
std::uint8_t ReactTargetStyleEquals(FightActorRuntimePrefix *actor);
std::uint8_t ReactMoveProficiencyAtLeast(FightActorRuntimePrefix *actor);
std::uint8_t ReactSlotModeEquals(FightActorRuntimePrefix *actor);
std::uint8_t ReactTargetGuardStateEquals(FightActorRuntimePrefix *actor);
std::uint8_t ReactBranchOnCondition(FightActorRuntimePrefix *actor);
std::uint8_t ReactTargetHasEntryFlag(FightActorRuntimePrefix *actor);
std::uint8_t ReactRememberTargetCharacter(FightActorRuntimePrefix *actor);
std::uint8_t ReactMatchesRememberedTarget(FightActorRuntimePrefix *actor);
std::uint8_t ReactTargetGuardDifference(FightActorRuntimePrefix *actor);
std::uint8_t ReactSetRank(FightActorRuntimePrefix *actor);
std::uint8_t ReactAliveTeamCountAtLeast(FightActorRuntimePrefix *actor);
std::uint8_t ReactTargetSurvivesDamage(FightActorRuntimePrefix *actor);
std::uint8_t ReactTargetStyleDiffers(FightActorRuntimePrefix *actor);
std::uint8_t ReactTargetHasActiveMotionFlags(FightActorRuntimePrefix *actor);
std::uint8_t ReactTargetHasActiveMotionFlagsAgain(FightActorRuntimePrefix *actor);
std::uint8_t ReactTargetHasPreviousMotionFlags(FightActorRuntimePrefix *actor);
std::uint8_t ReactTargetHasPreviousMotionFlagsAgain(FightActorRuntimePrefix *actor);
std::uint8_t ReactTargetBeforeAnimationFrame(FightActorRuntimePrefix *actor);
std::uint8_t ReactSelectNearbyTarget(FightActorRuntimePrefix *actor);
std::uint16_t vfGetReactionFrame(FightActorRuntimePrefix *actor, std::uint8_t selector);
static void VmResetAllSlots(FightActorRuntimePrefix *self);

namespace {

static const FightReactionCallback reactionCallbacks[104] = {VmCallback<0>, VmCallback<1>, VmCallback<2>, VmCallback<3>, VmCallback<4>, VmCallback<5>,
    VmCallback<6>, VmCallback<7>, VmCallback<8>, VmCallback<9>, VmCallback<10>, VmCallback<11>, VmCallback<12>, VmCallback<13>, VmCallback<14>, VmCallback<15>,
    VmCallback<16>, VmCallback<17>, VmCallback<18>, VmCallback<19>, VmCallback<20>, VmCallback<21>, VmCallback<22>, VmCallback<23>, VmCallback<24>,
    VmCallback<25>, VmCallback<26>, VmCallback<27>, VmCallback<28>, VmCallback<29>, VmCallback<30>, VmCallback<31>, VmCallback<32>, VmCallback<33>,
    VmCallback<34>, VmCallback<35>, VmCallback<36>, VmCallback<37>, VmCallback<38>, VmCallback<39>, VmCallback<40>, VmCallback<41>, VmCallback<42>,
    VmCallback<43>, VmCallback<44>, VmCallback<45>, VmCallback<46>, VmCallback<47>, VmCallback<48>, VmCallback<49>, VmCallback<50>, VmCallback<51>,
    VmCallback<52>, VmCallback<53>, VmCallback<54>, VmCallback<55>, VmCallback<56>, VmCallback<57>, VmCallback<58>, VmCallback<59>, VmCallback<60>,
    VmCallback<61>, VmCallback<62>, VmCallback<63>, VmCallback<64>, VmCallback<65>, VmCallback<66>, VmCallback<67>, VmCallback<68>, VmCallback<69>,
    VmCallback<70>, VmCallback<71>, VmCallback<72>, VmCallback<73>, VmCallback<74>, VmCallback<75>, VmCallback<76>, VmCallback<77>, VmCallback<78>,
    ReactSelectStageObject, ReactHasActiveMotionFlags, ReactLacksActiveMotionFlags, ReactHasMotionFlags, ReactLacksMotionFlags, ReactTargetHealthAtMost,
    ReactTargetStyleEquals, ReactMoveProficiencyAtLeast, ReactSlotModeEquals, ReactTargetGuardStateEquals, ReactBranchOnCondition, ReactTargetHasEntryFlag,
    ReactRememberTargetCharacter, ReactMatchesRememberedTarget, ReactTargetGuardDifference, ReactSetRank, ReactAliveTeamCountAtLeast, ReactTargetSurvivesDamage,
    ReactTargetStyleDiffers, ReactTargetHasActiveMotionFlags, ReactTargetHasActiveMotionFlagsAgain, ReactTargetHasPreviousMotionFlags,
    ReactTargetHasPreviousMotionFlagsAgain, ReactTargetBeforeAnimationFrame, ReactSelectNearbyTarget};

static const std::int32_t fightInputMap[17] = {0, 8, 1, 0, 0, 0, 0x4000, 0x4000, 0, 4, 2, 0, 0x20, 0x10, 0x80, 0x40, -1};
static const std::int32_t fightSecondaryInputMap[17] = {0, 8, 1, 0, 0, 0, 0x4000, 0, 0, 4, 2, 0, 0x20, 0x10, 0x80, 0x40, -1};

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Reset and consume the actual combat-produced gain banks.
// @verified - p
void vfResetMoveTrainingGains() {
    std::memset(moveTrainingGains, 0, sizeof(moveTrainingGains));
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void vfConsumeMoveTrainingGains() {
    NativeMoveSkillBank *const banks[3] = {&g_moveSkillBank1, &g_moveSkillBank2, &g_moveSkillBank3};
    for (std::uint8_t category = 1; category < 4; ++category) {
        for (std::int16_t move = 0; move < 40; ++move) {
            std::int16_t &proficiency = banks[category - 1]->proficiency[move];
            const std::int16_t previous = proficiency;
            const std::int16_t gain = g_fightMoveTrainingGains[category - 1][move];
            const std::int16_t increment = category == 3 ? static_cast<std::int16_t>(gain * 3) : gain / 2;
            std::int16_t value = static_cast<std::int16_t>(previous + increment);
            if (value > 199)
                value = 199;
            proficiency = value;
            g_fightMoveTrainingGains[category - 1][move] = 0;
            if (previous < 12 && value > 11)
                SetMoveLearningEntry(0, category, move, value);
        }
    }
}

#else

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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Two native 128-word histories and the contact/hit/end latches.
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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

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
                self->stateFlags |= VFSF_Unknown2000;
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
        self->stateFlags |= VFSF_Unknown1000;
    self->reactionDelay.count.remaining = std::uint16_t{0};
    self->reactionDelayActive = std::uint8_t{0};
    const std::uint8_t tail = self->reactionCursors.endpoint[1];
    self->reactionCursors.tracked[0] = tail;
    self->reactionCursors.tracked[1] = tail;
    self->reactionCursors.tracked[2] = tail;
    return 1;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::int16_t ReactionWord(ReactionPc p) {
    return static_cast<std::int16_t>((static_cast<std::uint16_t>(p[0]) << 8) | p[1]);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::int32_t ReactionDword(ReactionPc p) {
    return std::bit_cast<std::int32_t>(
        (static_cast<std::uint32_t>(p[0]) << 24) | (static_cast<std::uint32_t>(p[1]) << 16) | (static_cast<std::uint32_t>(p[2]) << 8) | p[3]);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

ReactionPc ReactionTarget(VFActor &self, std::int32_t offset) {
    return reinterpret_cast<ReactionPc>(self.motionParameters[0]) + offset;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void SetReactionPc(VFActor &self, ReactionPc p) {
    self.reactionPc = p;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void ResetReactionSlots(VFActor &self) {
    const std::uint8_t tail = self.reactionCursors.endpoint[1];
    self.reactionCursors.tracked[0] = tail;
    self.reactionCursors.tracked[1] = tail;
    self.reactionCursors.tracked[2] = tail;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::int32_t ScaleReactionCount(VFActor &self, std::int32_t count) {
    return static_cast<std::int32_t>(static_cast<double>(static_cast<float>(count) * self.inverseScale) + 0.5);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::uint16_t ReactionMask(VFActor &self, ReactionPc p, bool lowOnly = false) {
    std::uint16_t value = p[3];
    if (p[2] & 4)
        value |= self.reactionMaskA.words[0];
    if (p[2] & 8)
        value |= self.reactionMaskB.words[0];
    return lowOnly ? static_cast<std::uint8_t>(value) : value;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::uint32_t ReactionIntersectionMask(VFActor &self, ReactionPc p) {
    if (p[2] & 4)
        return self.reactionMaskA.bits;
    if (p[2] & 8)
        return self.reactionMaskB.bits;
    return static_cast<std::uint16_t>(ReactionWord(p + 2));
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void PushReactionContext(VFActor &self, ReactionPc target, std::uint32_t priorityOnly, bool incrementDepth) {
    const std::uint8_t depth = self.reactions.depth;
    self.reactions.contexts[depth].program = target;
    self.reactions.contexts[depth].scanRemaining = self.reactions.scanRemaining;
    self.reactions.contexts[depth].priorityOnly = priorityOnly;
    self.reactions.contexts[depth].inputPressed = self.inputPressed;
    self.reactions.contexts[depth].inputMask = self.inputMask;
    self.reactions.contexts[depth].maskSnapshot = self.reactions.maskSnapshot;
    self.reactions.contexts[depth].allowedMask = self.reactions.window.count.allowed;
    self.reactions.contexts[depth].remainingFrames = static_cast<std::uint16_t>(ScaleReactionCount(self, self.reactions.window.count.remaining));
    self.reactions.contexts[depth].historyIndex = self.reactions.historyIndex;
    if (incrementDepth)
        self.reactions.depth = static_cast<std::uint8_t>(depth + 1);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// Native g_vf_react_callbacks: all 104 native reaction callbacks, including genuine shared entries.
std::uint8_t RunReactionCallback(VFActor &s, std::uint8_t opcode) {
    const ReactionPc p = s.reactionPc;
    const auto arg = static_cast<std::int8_t>(p[1]);
    const std::uint16_t previous = s.reactions.maskSnapshot;
    const std::uint16_t mask = s.inputMask;
    const std::uint16_t flags = s.inputPressed;
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
        SetReactionPc(s, static_cast<std::uint16_t>(ReactionWord(p + 2)) == s.reactions.conditionFlags ? ReactionTarget(s, ReactionWord(p + 4)) : p + 8);
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
        if (s.activeMotionFlags & VFSMF_Unknown4)
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
        const std::uint16_t value = s.inputHistory[s.reactions.historyIndex].sectorMask;
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
        if (!(s.hitStateFlags & VFHSF_FallLandingRequired))
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
            s.reactionDelay.count.allowed =
                static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<double>((frame - s.animationTime) * s.inverseScale) + 0.5));
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
            vfmaUpdateCollisionCounter(s.game, VFCollisionCounterEvent::HitSequenceEnded,
                static_cast<std::uint16_t>(std::bit_cast<std::int8_t>(s.secondActor->reactionVariantBytes[3])));
        clearTiming();
        ResetReactionSlots(s);
        return 1;
    case 68:
        return gate(static_cast<float>(static_cast<std::uint8_t>(s.actor->hitDamage)) >= s.hp);
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
        const std::int16_t angle = vfGetFacingDeltaFromCameraSectorCode(&s, s.inputHistory[s.reactions.historyIndex].sectorMask);
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
        return gate(s.actor->hp > static_cast<float>(static_cast<std::uint8_t>(s.hitDamage)));
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
        // Native's final fall-through accepts slots 1..4, but not slot zero.
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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Camera-relative sector angles use contact yaw only for close, frontal actors.
// @verified - p
std::int16_t vfGetFacingDeltaFromCameraSectorCode(VFActor *self, std::uint16_t code) {
    const auto sector = static_cast<std::int16_t>(cameraYawAngles[(code >> 4) & 15] + 0x8000);
    const auto facing = self->contactDistances[self->actionIndex] < 3 && !(self->groundContactFlags & 1) && !(self->actor->moveFlags & VFMF_ForceDefeat) ?
        self->contactAngles[self->actionIndex] :
        static_cast<std::int16_t>(self->facingYaw);
    return static_cast<std::int16_t>(sector - facing + camCtrlGetNextRotY());
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Native priority-history scan and in-actor context stack.
// @verified - p
std::uint8_t VF_RunStateMachine(VFActor *actor) {
    auto &s = *actor;
    const std::uint16_t savedMask = s.inputMask, savedFlags = s.inputPressed;
    const std::uint16_t savedSnapshot = s.reactions.maskSnapshot;
    const ReactionPc savedPc = s.reactionPc;
    const std::uint64_t savedOverride = s.reactionOverrideActive;
    s.reactions.depth = std::uint8_t{0};
    const std::uint8_t index = static_cast<std::uint8_t>((s.reactionCursors.endpoint[1] - 1) & 31);
    s.reactions.historyIndex = index;
    const auto filter = static_cast<std::uint8_t>(s.queuedAction >> 24) == 6 ? 0x1f2 : 0xf0;
    s.inputMask &= filter;
    s.inputPressed &= filter;
    s.reactions.maskSnapshot = s.inputMask;
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
                    s.reactions.maskSnapshot = s.inputMask;
                    pop = s.inputMask && (static_cast<std::uint16_t>(~allowed) & s.inputMask);
                    if (!pop && s.inputMask)
                        s.reactions.window.count.allowed = s.inputMask;
                }
                response = 4;
            } else
                response = (s.inputMask != s.reactions.maskSnapshot) + 3;
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
                s.inputMask = s.inputHistory[next].mask & 0xf0;
                const std::uint16_t historyFlags = s.inputHistory[next].pressed;
                auto phase = static_cast<std::int8_t>((previous >= next) + (stop <= previous ? 0 : 1));
                if (stop > next)
                    --phase;
                if (phase < 1) {
                    s.inputPressed = historyFlags & s.reactions.filters.mask.current;
                    pop = s.inputPressed != (historyFlags & s.reactions.filters.mask.previous);
                } else
                    s.inputPressed = historyFlags & 0xf0;
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
                    s.inputPressed = s.reactions.contexts[depth].inputPressed;
                    s.inputMask = s.reactions.contexts[depth].inputMask;
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
            s.inputMask = savedMask;
            s.inputPressed = savedFlags;
            s.reactions.maskSnapshot = savedSnapshot;
            SetReactionPc(s, savedPc);
            s.reactionOverrideActive = savedOverride;
            return 1;
        }
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Publish one eight-byte priority record into the native 32-entry ring.
void vfQueueReactionFlagsForFrame(VFActor *actor, bool clearDifference) {
    auto &s = *actor;
    const std::uint8_t head = s.reactionCursors.endpoint[0];
    const std::uint8_t previous = static_cast<std::uint8_t>((head - 1) & 31);
    const std::uint16_t previousMask = s.inputHistory[previous].mask;
    const std::uint16_t previousSector = s.inputHistory[previous].sectorMask;
    std::uint16_t current = s.inputFlags;
    const std::uint16_t difference = static_cast<std::uint16_t>(current & ~previousMask);
    const std::uint16_t released = static_cast<std::uint16_t>(previousMask & ~current);
    s.inputMask = current;
    s.inputPressed = difference;
    s.followupFlags = difference;
    s.inputHistory[head].mask = current;
    s.inputHistory[head].pressed = clearDifference ? std::uint16_t{0} : difference;
    s.inputHistory[head].released = released;
    s.reactions.releasedFlags = released;
    if (difference)
        s.reactions.pressed = difference;
    if (!(current & 0xc0))
        current |= previousSector & 0xc0;
    if (!(current & 0x30))
        current |= previousSector & 0x30;
    s.inputHistory[head].sectorMask = current;
    const std::uint8_t next = static_cast<std::uint8_t>((head + 1) & 31);
    const std::uint8_t following = static_cast<std::uint8_t>((head + 2) & 31);
    s.reactionCursors.endpoint[0] = next;
    for (std::uint8_t &cursor : s.reactionCursors.tracked)
        if (cursor == next)
            cursor = following;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Select the actual authored base, paused, or continuation reaction program.
void vfUpdateReactionOverrideState(VFActor *actor) {
    auto &s = *actor;
    bool keep = false;
    if (s.moveFlags & VFMF_HitReactionLock)
        s.reactionOverrideActive |= 1;
    else {
        const ReactionPc continuation = s.reactionContinuation;
        SetReactionPc(s, continuation);
        if (s.hitStateFlags & VFHSF_ReactionActive) {
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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Two native deferred edge-flag slots; the decrement is intentionally
// followed by the native unconditional clear.
void vfUpdateDeferredReactionPriority(VFActor *actor, std::uint8_t slot) {
    auto &s = *actor;
    const auto flags = static_cast<std::uint16_t>(s.inputPressed & 0x30f);
    const std::uint8_t active = s.deferredPriorityState[slot];
    if (active) {
        const std::uint16_t saved = s.deferredPriorityFlags[slot];
        if ((s.reactionOverrideActive & 1) || saved != 0x100 || (s.inputMask & 0x100) || flags != 1) {
            const auto merged = static_cast<std::uint16_t>(saved | flags);
            if (merged != 0x30f)
                s.deferredPriorityState[slot] = static_cast<std::uint8_t>(active - 1);
            s.deferredPriorityState[slot] = std::uint8_t{0};
            s.inputPressed |= merged;
        } else {
            const std::uint8_t tail = s.reactionCursors.endpoint[1];
            s.reactionCursors.deferred[slot] = tail;
            s.reactionCursors.previous = tail;
            s.deferredPriorityState[slot] = std::uint8_t{0};
            s.inputPressed |= 1;
        }
    } else {
        if (flags && flags != 0x30f) {
            s.deferredPriorityState[slot] = std::uint8_t{1};
            s.deferredPriorityFlags[slot] = flags;
            s.inputPressed &= 0xfcf0;
        }
        const std::uint8_t tail = s.reactionCursors.endpoint[1];
        s.reactionCursors.deferred[slot] = tail;
        s.reactionCursors.previous = tail;
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Priority admission may consume the pending authored continuation.
std::uint8_t vfResolvePriorityReaction(VFActor *actor) {
    auto &s = *actor;
    if ((s.hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_SpecialThrow | VFHSF_GuardState)) || !(s.inputPressed & 0x300))
        return 1;
    s.reactionOverrideFlags.bits = s.reactionOverrideFlags.bits & ~1U;
    s.reactionCursors.endpoint[1] = s.reactionCursors.endpoint[0];
    const ReactionPc continuation = s.reactionContinuation;
    if (!(continuation ? s.reactionDelay.count.remaining != 0 : (s.reactionOverrideActive & 1) || (s.hitStateFlags & VFHSF_FallLandingRequired))) {
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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::uint16_t MapFightInput(std::uint16_t input, bool secondary) {
    std::uint16_t response = 0;
    for (std::uint8_t bit = 0; bit < 16; ++bit)
        if (input & (1U << bit))
            response |= inputResponses[secondary][bit];
    return response;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::uint8_t CameraSectorIndex(std::uint16_t code) {
    for (std::uint8_t index = 0; index < 8; ++index)
        if (cameraSectors[index] == (code & 0xf0))
            return index;
    return 0xff;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void RewindReactionTail(VFActor &self, std::uint8_t cursor) {
    const std::uint8_t head = self.reactionCursors.endpoint[0];
    std::int8_t distance = static_cast<std::int8_t>(head - cursor);
    if (distance < 0)
        distance += 32;
    const auto limit = self.hitStateFlags & VFHSF_ForcedReaction ? 8 : 6;
    if (distance > limit)
        distance = static_cast<std::int8_t>(limit);
    self.reactionCursors.endpoint[1] = static_cast<std::uint8_t>((head - distance) & 31);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfResetReactionProgram(FightActorRuntimePrefix *actor) {
    *reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(actor) + 0x5b0) = 0;
    *reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(actor) + 0x5b4) =
        *reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(actor) + 0x28);
    *reinterpret_cast<std::uint16_t *>(reinterpret_cast<std::byte *>(actor) + 0x5a8) = 0;
    *reinterpret_cast<std::uint8_t *>(reinterpret_cast<std::byte *>(actor) + 0x5a6) = 0;
}

#else

// The saved continuation and current program remain actor-owned.
// @verified - p
void vfResetReactionProgram(VFActor *self) {
    self->reactionPc = reinterpret_cast<const std::uint8_t *>(self->motionParameters[0]);
    self->reactionContinuation = nullptr;
    self->reactionDelay.count.remaining = std::uint16_t{0};
    self->reactionDelayActive = std::uint8_t{0};
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Reset the native input/reaction ring without replacing its borrowed programs.
// @verified - p
void vfResetActorActionState(VFActor *self) {
    auto &actor = *self;
    actor.moveFlags |= VFMF_LockMovementInput;
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
    actor.inputHistory[0].mask = actor.inputFlags;
    actor.inputHistory[0].sectorMask = actor.inputFlags;
    actor.reactions.pressed = actor.followupFlags;
    const std::uint8_t head = actor.reactionCursors.endpoint[1];
    actor.reactionCursors.tracked[0] = head;
    actor.reactionCursors.tracked[1] = head;
    actor.reactionCursors.tracked[2] = head;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#define SHENMUE_REACTION_VALUE(T, owner, offset) (*reinterpret_cast<T *>(static_cast<std::byte *>(static_cast<void *>(owner)) + (offset)))

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#define SHENMUE_REACTION_EXTRA_WORD(data, offset) (*reinterpret_cast<const std::uint16_t *>((data) + (offset)))

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::uint32_t GetPairedThirdActorMotion(FightActorRuntimePrefix *actor) {
    const std::byte *const record = MOTN_FindAnimationExtraRecord(SHENMUE_REACTION_VALUE(std::uint16_t, actor, 0x320) & 0xfffU, 16);
    return record ? SHENMUE_REACTION_EXTRA_WORD(record, 2) : 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void ResolveQueuedPairAction(FightActorRuntimePrefix *actor, FightActorRuntimePrefix *other) {
    if (SHENMUE_REACTION_VALUE(std::uint32_t, actor, 0x318))
        goto commitSelected;
    if (SHENMUE_REACTION_VALUE(std::uint32_t, actor, 0x324) &&
        vfDidCrossFightFrame(actor, static_cast<float>(SHENMUE_REACTION_VALUE(std::uint16_t, actor, 0x448))))
        goto commitQueued;
    {
        const std::uint8_t state = SHENMUE_REACTION_VALUE(std::uint8_t, actor, 0x323);
        const std::uint8_t otherState = SHENMUE_REACTION_VALUE(std::uint8_t, other, 0x323);
        if (state == 8 && actor->game->retained44) {
            SHENMUE_REACTION_VALUE(std::uint32_t, actor, 0x320) = 0;
            return;
        }
        if (state == 19) {
            SHENMUE_REACTION_VALUE(std::uint16_t, actor, 0x448) = SHENMUE_REACTION_VALUE(std::uint16_t, other, 0x2f6);
            SHENMUE_REACTION_VALUE(std::uint32_t, actor, 0x324) = SHENMUE_REACTION_VALUE(std::uint32_t, actor, 0x320);
            SHENMUE_REACTION_VALUE(std::uint32_t, actor, 0x320) = 0;
            return;
        }
        if (state == 20) {
            actor->animationTime += SHENMUE_REACTION_VALUE(float, actor, 0x26c) * 3.0f;
            SHENMUE_REACTION_VALUE(float, actor, 0x450) = actor->animationTime;
            SHENMUE_REACTION_VALUE(std::uint32_t, actor, 0x320) = 0;
            return;
        }
        if ((state == 8 || state == 17) && (static_cast<std::uint8_t>(otherState - 8) < 2 || otherState == 14 || otherState == 15 || otherState == 19) &&
            (!(actor->activeMotionFlags & 0x400U) || (actor->motionFlags & 0x100U) || other->secondActor != actor)) {
            SHENMUE_REACTION_VALUE(std::uint32_t, actor, 0x320) = 0;
            return;
        }
        if ((actor->moveFlags & 0x10U) && state != 6 && (!(actor->secondaryMotionFlags & 0xc00U) || (actor->motionFlags & 0x100U))) {
            SHENMUE_REACTION_VALUE(std::uint32_t, actor, 0x320) = 0;
            return;
        }
        if (SHENMUE_REACTION_VALUE(std::uint32_t, actor, 0x320))
            SHENMUE_REACTION_VALUE(std::uint32_t, actor, 0x324) = 0;
    }
    return;
commitQueued:
    SHENMUE_REACTION_VALUE(std::uint32_t, actor, 0x320) = SHENMUE_REACTION_VALUE(std::uint32_t, actor, 0x324);
    SHENMUE_REACTION_VALUE(std::uint32_t, actor, 0x324) = 0;
    return;
commitSelected:
    SHENMUE_REACTION_VALUE(std::uint32_t, actor, 0x320) = SHENMUE_REACTION_VALUE(std::uint32_t, actor, 0x318);
    SHENMUE_REACTION_VALUE(std::uint32_t, actor, 0x324) = 0;
}

} // namespace

#else

namespace {

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
        (!(self.activeMotionFlags & VFSMF_Unknown400) || (self.motionFlags & VFMOF_Unknown100) || other.secondActor != &self)) {
        self.action = 0;
        return;
    }
    if ((self.moveFlags & VFMF_HitReactionLock) && state != 6 &&
        (!(self.secondaryMotionFlags & (VFSMF_Unknown400 | VFSMF_Unknown800)) || (self.motionFlags & VFMOF_Unknown100))) {
        self.action = 0;
        return;
    }
    if (!std::strcmp(GetAreaSlotName(3), "CWON.EV3") && (self.action & 0xfff) == 0x644 && self.game->spawnPointCount) {
        SetAuthoredSpawnPosition(self);
        self.moveFlags |= VFMF_RefreshContactHistory;
    }
    if (self.action)
        self.deferredReaction = 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void ResolvePairedHit(FightActorRuntimePrefix *source, FightActorRuntimePrefix *destination) {
    FightRuntimePrefix *const game = source->game;
    vfSetPairedActor(game, SHENMUE_REACTION_VALUE(std::uint8_t, destination, 0), SHENMUE_REACTION_VALUE(std::uint8_t, source, 0));
    destination->secondActor = source;
    const std::uint16_t motion = SHENMUE_REACTION_VALUE(std::uint16_t, source, 0x320) & 0xfffU;
    const std::byte *style = MOTN_FindAnimationExtraRecord(motion, 26);
    std::int16_t hit;
    if (style && SHENMUE_REACTION_VALUE(std::uint16_t, destination, 2) == static_cast<std::int8_t>(style[1]))
        hit = SHENMUE_REACTION_EXTRA_WORD(style, 4);
    else
        hit = SHENMUE_REACTION_EXTRA_WORD(MOTN_FindAnimationExtraRecord(motion, 5), 2);
    SHENMUE_REACTION_VALUE(std::uint32_t, destination, 0x320) = static_cast<std::int32_t>(static_cast<std::int16_t>(hit)) | 0x09000000U;
    SHENMUE_REACTION_VALUE(std::uint8_t, destination, 0x27c) = 0;
    SHENMUE_REACTION_VALUE(float, destination, 0x278) = destination->game->knockbackAmount;
    source->moveFlags &= 0xfffffbbfU;
    source->hitStateFlags &= ~0x200000U;
    if (SHENMUE_REACTION_VALUE(std::int8_t, source, 0x322) == 1 || MOTN_FindAnimationExtraRecord(motion, 18)) {
        destination->moveFlags &= 0xfffffbbfU;
        destination->hitStateFlags &= ~0x200000U;
    } else {
        source->moveFlags |= destination->moveFlags & 0x440U;
        source->hitStateFlags |= destination->hitStateFlags & 0x200000U;
    }
    if ((source->hitStateFlags & 0xc0000U) || (destination->activeMotionFlags & 0x100000U)) {
        source->facingAngleDelta = destination->facingAngleDelta = 0;
        return;
    }
    if ((GetPairedThirdActorMotion(source) & 0xfffU) == 0x649 && game->spawnPointCount) {
        destination->position.x = game->spawnPoints[SHENMUE_REACTION_VALUE(std::uint8_t, destination, 0x945)].x;
        destination->position.z = game->spawnPoints[SHENMUE_REACTION_VALUE(std::uint8_t, destination, 0x945)].z;
        destination->position.y = flddGetIntersectionFor(destination->character, destination->position);
    }
    source->targetPosition.x = destination->position.x;
    source->targetPosition.y = destination->position.y;
    source->targetPosition.z = destination->position.z;
    SHENMUE_REACTION_VALUE(std::uint16_t, source, 0x64) = destination->facingAngle + destination->facingAngleDelta;
    destination->targetPosition.x = destination->position.x;
    destination->targetPosition.y = destination->position.y;
    destination->targetPosition.z = destination->position.z;
    SHENMUE_REACTION_VALUE(std::uint16_t, destination, 0x64) = SHENMUE_REACTION_VALUE(std::uint16_t, source, 0x64);
    source->moveFlags |= 0x10000000U;
    destination->moveFlags |= 0x10000000U;
}

} // namespace

#else

namespace {

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
    source.moveFlags &= ~(VFMF_MirrorReactionAngle | VFMF_MirroredStance);
    source.hitStateFlags &= ~VFHSF_MirroredReactionAngle;
    if (static_cast<std::uint8_t>(source.action >> 16) == 1 || !FindLoadedMotionSecondaryEntry(motion, 18).empty()) {
        destination.moveFlags &= ~0x440U;
        destination.hitStateFlags &= ~0x200000U;
    } else {
        source.moveFlags |= destination.moveFlags & 0x440;
        source.hitStateFlags |= destination.hitStateFlags & 0x200000;
    }
    if (source.hitStateFlags & (VFHSF_MidAirRecovery | VFHSF_AirborneOrDowned)) {
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
    source.moveFlags |= VFMF_RefreshContactHistory;
    destination.moveFlags |= 0x10000000;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void ResolvePairedTeammate(FightActorRuntimePrefix *destination, FightActorRuntimePrefix *source) {
    const std::uint32_t motion = GetPairedThirdActorMotion(destination);
    if (!motion)
        return;
    FightRuntimePrefix *const game = destination->game;
    FightActorRuntimePrefix *selected = 0;
    std::uint16_t index = 0;
    float distance = 10000.0f;
    for (; index < 6; ++index) {
        if (!game->characters[index])
            continue;
        FightActorRuntimePrefix *const teammate = game->actors[index];
        if (teammate == destination || game->slotTeams[SHENMUE_REACTION_VALUE(std::int8_t, destination, 0)] != game->slotTeams[index] ||
            std::fabs(destination->hp) < 0.00000011920929f)
            continue;
        const float candidate = destination->contactDistances[index];
        if (candidate < distance) {
            selected = teammate;
            distance = candidate;
        }
    }
    if (!selected)
        return;
    SHENMUE_REACTION_VALUE(std::uint32_t, selected, 0x318) = motion | 0x22000000U;
    selected->targetPosition.x = destination->targetPosition.x;
    selected->targetPosition.y = destination->targetPosition.y;
    selected->targetPosition.z = destination->targetPosition.z;
    SHENMUE_REACTION_VALUE(std::uint16_t, selected, 0x64) = SHENMUE_REACTION_VALUE(std::uint16_t, destination, 0x64);
    selected->moveFlags = (selected->moveFlags | 0x10000000U) & 0xfffffbbfU;
    selected->hitStateFlags &= ~0x200000U;
    const std::int8_t *const action = SHENMUE_REACTION_VALUE(const std::int8_t *, destination, 0x320);
    if (action[2] == 1) {
        selected->moveFlags &= 0xfffffbbfU;
        selected->hitStateFlags &= ~0x200000U;
    } else {
        selected->moveFlags |= source->moveFlags & 0x440U;
        selected->hitStateFlags |= source->hitStateFlags & 0x200000U;
    }
}

} // namespace

#else

namespace {

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
        selected->moveFlags |= source.moveFlags & (VFMF_MirrorReactionAngle | VFMF_MirroredStance);
        selected->hitStateFlags |= source.hitStateFlags & VFHSF_MirroredReactionAngle;
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfResolvePairInteraction(FightActorRuntimePrefix *actor) {
    const std::uint8_t paired = SHENMUE_REACTION_VALUE(std::uint8_t, actor, 0x9be);
    if (SHENMUE_REACTION_VALUE(std::uint8_t, actor, 0x44a) & (1U << (paired & 31))) {
        SHENMUE_REACTION_VALUE(std::uint32_t, actor, 0x318) = 0;
        return;
    }
    FightActorRuntimePrefix *const other = SHENMUE_REACTION_VALUE(FightActorRuntimePrefix *, actor, 0x9c4);
    SHENMUE_REACTION_VALUE(std::uint8_t, other, 0x44a) |= 1U << (SHENMUE_REACTION_VALUE(std::uint8_t, actor, 0) & 31);
    const bool reciprocal = SHENMUE_REACTION_VALUE(FightActorRuntimePrefix *, other, 0x9c4) == actor;
    SHENMUE_REACTION_VALUE(float, actor, 0x440) = other->animationTime;
    if (reciprocal)
        SHENMUE_REACTION_VALUE(float, other, 0x440) = actor->animationTime;
    ResolveQueuedPairAction(actor, other);
    if (reciprocal)
        ResolveQueuedPairAction(other, actor);
    const std::uint8_t state = SHENMUE_REACTION_VALUE(std::uint8_t, actor, 0x323);
    const std::uint8_t otherState = SHENMUE_REACTION_VALUE(std::uint8_t, other, 0x323);
    if (state == 17 || otherState == 17) {
        FightActorRuntimePrefix *const source = state == 17 ? actor : other;
        FightActorRuntimePrefix *const destination = state == 17 ? other : actor;
        if (state == 17 || reciprocal) {
            SHENMUE_REACTION_VALUE(std::int32_t, source, 0x434) = SHENMUE_REACTION_VALUE(std::int16_t, source, 0x320);
            SHENMUE_REACTION_VALUE(float, source, 0x278) = source->game->knockbackAmount;
            SHENMUE_REACTION_VALUE(float, destination, 0x278) = destination->game->knockbackAmount;
            vfApplyHitReactionAndStartMotion(source);
            SHENMUE_REACTION_VALUE(std::uint16_t, source, 0x3c8) = SHENMUE_REACTION_VALUE(std::uint16_t, destination, 0x42c);
            const float offset = static_cast<float>(SHENMUE_REACTION_VALUE(std::uint16_t, destination, 0x290)) -
                static_cast<float>(SHENMUE_REACTION_VALUE(std::uint16_t, source, 0x2e0));
            SHENMUE_REACTION_VALUE(float, source, 0x444) = offset;
            SHENMUE_REACTION_VALUE(std::uint8_t, destination, 0x2aa) = SHENMUE_REACTION_VALUE(std::uint8_t, source, 0x2aa);
            vfSetPairedActor(source->game, SHENMUE_REACTION_VALUE(std::uint8_t, destination, 0), SHENMUE_REACTION_VALUE(std::uint8_t, source, 0));
            destination->secondActor = source;
            const float trigger = offset - SHENMUE_REACTION_VALUE(float, actor, 0x26c);
            if (trigger > destination->animationTime) {
                std::uint32_t action = SHENMUE_REACTION_VALUE(std::uint16_t, source, 0x2b0) + 0x09000000U;
                bool mirror = (source->moveFlags & 0x40U) != 0;
                if (destination->moveFlags & 0x400U)
                    mirror = !mirror;
                if (mirror)
                    action |= 0x8000U;
                SHENMUE_REACTION_VALUE(std::uint32_t, destination, 0x324) = action;
                SHENMUE_REACTION_VALUE(std::uint16_t, destination, 0x448) = static_cast<std::int32_t>(trigger);
            } else {
                ResolvePairedHit(source, destination);
                SHENMUE_REACTION_VALUE(std::uint32_t, actor, 0x318) = SHENMUE_REACTION_VALUE(std::uint32_t, other, 0x318) = 0;
                return;
            }
        }
    }
    if (state == 19 || otherState == 19) {
        FightActorRuntimePrefix *const source = state == 19 ? actor : other;
        FightActorRuntimePrefix *const destination = state == 19 ? other : actor;
        if (state == 19 || reciprocal) {
            SHENMUE_REACTION_VALUE(std::uint32_t, source, 0x320) = (SHENMUE_REACTION_VALUE(std::uint32_t, source, 0x320) & 0xffffffU) | 0x08000000U;
            mtwkSetExtraFlag(destination->motion, 0, 0);
            ResolvePairedHit(source, destination);
            SHENMUE_REACTION_VALUE(std::uint32_t, source, 0x318) = SHENMUE_REACTION_VALUE(std::uint32_t, destination, 0x318) = 0;
            SHENMUE_REACTION_VALUE(std::uint32_t, destination, 0x5b0) = 0;
            SHENMUE_REACTION_VALUE(std::uint32_t, destination, 0x5b4) = SHENMUE_REACTION_VALUE(std::uint32_t, actor, 0x28);
            SHENMUE_REACTION_VALUE(std::uint16_t, destination, 0x5a8) = 0;
            SHENMUE_REACTION_VALUE(std::uint8_t, destination, 0x5a6) = 0;
            return;
        }
    }
    if (state == 8 || otherState == 8) {
        FightActorRuntimePrefix *source = actor, *destination = other;
        bool admit = true;
        if (otherState == 8 && (state != 8 || !(dc::sdk::RandomInt() & 1U))) {
            source = other;
            destination = actor;
            admit = reciprocal;
        }
        if (admit) {
            ResolvePairedHit(source, destination);
            ResolvePairedTeammate(source, destination);
            SHENMUE_REACTION_VALUE(std::uint32_t, source, 0x318) = SHENMUE_REACTION_VALUE(std::uint32_t, destination, 0x318) = 0;
        }
    }
    SHENMUE_REACTION_VALUE(std::uint32_t, actor, 0x318) = 0;
}

#else

// Native reciprocal-pair arbitration, not a fabricated second-actor view.
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
        destination.hitDamage = source.hitDamage;
        BindPairedActor(*source.game, destination.groupIndex, source.groupIndex);
        destination.secondActor = &source;
        const auto trigger = source.pairTimeOffset - self.scale;
        if (trigger <= destination.animationTime) {
            ResolvePairedHit(destination, source);
            self.parameter370 = other->parameter370 = 0;
            return;
        }
        const bool mirror = destination.moveFlags & 0x400 ? !(source.moveFlags & VFMF_MirrorReactionAngle) : (source.moveFlags & VFMF_MirrorReactionAngle) != 0;
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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// The category mask comes from the actor's authored move id.
// @verified - p
std::uint32_t vfGetMoveCategoryMaskFor(VFActor *actor) {
    return (actor->hitStateFlags & VFHSF_AttackActive) ? moveCategories[actor->moveId] : 0;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// The collision selector shares the native six category bits.
std::uint8_t vfSelectHitReactionSequence(VFActor *actor) {
    return HitReactionCategory(*actor);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

FightAiSlot &AiSlot(FightActorRuntimePrefix *actor) {
    return *static_cast<FightAiSlot *>(actor->slot);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

inline std::int8_t AiTarget(FightActorRuntimePrefix *actor) {
    return SHENMUE_REACTION_VALUE(std::int8_t, actor, 1);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

inline std::uint16_t AiStyle(FightActorRuntimePrefix *actor) {
    return SHENMUE_REACTION_VALUE(std::uint16_t, actor, 2);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

inline std::int16_t AiAngle(FightActorRuntimePrefix *actor, std::uint32_t index) {
    return reinterpret_cast<FightAiActorView *>(actor)->targetAngles[index];
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

inline bool AiChance(std::uint8_t chance, std::uint8_t random) {
    return chance && chance >= random;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static void AiRerollPhase(FightAiPhase *phase) {
    phase->countdown = phase->choices[dc::sdk::RandomInt() & 15];
    if (phase->countdown > 1)
        --phase->countdown;
    phase->selected = static_cast<std::int32_t>(dc::sdk::RandomInt()) % (phase->countdown + 1);
}

static void AiUpdatePhase(FightAiPhase *phase);

// @verified - d
static void AiUpdatePhases(FightActorRuntimePrefix *self) {
    FightActorRuntimePrefix *const actor = self->actor;
    FightAiSlot &slot = *static_cast<FightAiSlot *>(self->slot);
    if (actor->stateFlags & 0x1000U) {
        AiUpdatePhase(&slot.phases[0]);
        AiUpdatePhase(&slot.phases[1]);
        AiUpdatePhase(&slot.phases[2]);
        AiUpdatePhase(&slot.phases[3]);
        AiUpdatePhase(&slot.phases[4]);
    }
    if (actor->stateFlags & 0x2000U)
        AiUpdatePhase(&slot.phases[5]);
}

// @verified - d
static void AiUpdatePhase(FightAiPhase *phase) {
    if (phase->enabled) {
        if (phase->countdown)
            --phase->countdown;
        else
            AiRerollPhase(phase);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t AiPhaseSelected(const FightAiPhase &phase) {
    if (!phase.enabled)
        return 0;
    if (phase.reversed) {
        if (phase.countdown == phase.selected)
            return 0;
    } else if (phase.countdown != phase.selected)
        return 0;
    return 1;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void AiResetPhase(FightActorRuntimePrefix *actor) {
    FightAiSlot &slot = *static_cast<FightAiSlot *>(actor->slot);
    slot.command = 0;
    slot.step = 0;
    slot.inputTicks = 0;
    slot.waitFrames = 0;
    slot.queuedCommandIndex = 0;
    slot.queuedCommands[1] = 127;
    slot.queuedCommands[0] = 127;
    slot.flags |= 1;
    slot.flags &= ~0x2000U;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void AiSelectCommand(FightActorRuntimePrefix *actor, std::uint8_t choice, std::uint16_t delay) {
    FightAiSlot &slot = *static_cast<FightAiSlot *>(actor->slot);
    if (slot.flags & 1U) {
        slot.flags &= ~1U;
        slot.command = choice;
        slot.waitFrames = delay;
        slot.step = 0;
        slot.inputTicks = 0;
        slot.random[0] = dc::sdk::RandomInt();
        slot.queuedCommandIndex = 0;
        slot.queuedCommands[0] = slot.queuedCommands[1] = 127;
        const std::uint8_t *record = SHENMUE_REACTION_VALUE(const std::uint8_t *, actor, 0x3c);
        for (; record[0] != 127; record += 17) {
            std::uint8_t count = 4;
            if (record[0] != choice)
                continue;
            if (record[13] == 127)
                count = 3;
            if (record[9] == 127)
                --count;
            if (record[5] == 127)
                --count;
            if (record[1] == 127)
                --count;
            if (static_cast<std::int8_t>(count) > 0) {
                const std::uint8_t pick = static_cast<std::uint8_t>(dc::sdk::RandomInt()) % count;
                const FightAiCommandRecord *const entry = reinterpret_cast<const FightAiCommandRecord *>(record);
                slot.queuedCommands[0] = entry->options[pick].command;
                slot.queuedDelays[0] = entry->options[pick].delay;
                slot.queuedCommands[1] = entry->options[pick].followCommand;
                slot.queuedDelays[1] = entry->options[pick].followDelay;
            }
            return;
        }
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void AiContinueCommand(FightActorRuntimePrefix *actor, std::uint8_t choice, std::uint16_t delay) {
    FightAiSlot *slot = static_cast<FightAiSlot *>(actor->slot);
    slot->command = choice;
    slot->waitFrames = delay;
    slot->step = 0;
    slot->inputTicks = 0;
    slot->random[0] = dc::sdk::RandomInt();
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void AiChooseCommand(FightActorRuntimePrefix *actor, const std::uint8_t *choices, std::uint8_t count, std::uint16_t delay) {
    if (static_cast<std::int8_t>(count) > 0) {
        const std::uint8_t pick = static_cast<std::int32_t>(dc::sdk::RandomFloat() * count);
        AiSelectCommand(actor, choices[pick], delay);
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

inline void AiChooseAuthored(FightActorRuntimePrefix *actor, std::uint32_t offset, std::uint8_t count, std::uint16_t delay = 0) {
    AiChooseCommand(actor, reinterpret_cast<const std::uint8_t *>(actor->motionParameters) + offset, count, delay);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t AiUpdateInput(FightActorRuntimePrefix *actor) {
    FightAiSlot &slot = *static_cast<FightAiSlot *>(fightAiGame->actorSlots[actor->groupIndex]);
    const FightAiInputSequence *record = 0;
    std::uint16_t input;
    if (slot.inputTicks) {
        if (slot.inputTicks != 1)
            goto hold;
        if (++slot.step == 11) {
            slot.inputTicks = 0;
            return 1;
        }
    }
    record = slot.command <= 199 ? SHENMUE_REACTION_VALUE(const FightAiInputSequence *, actor, 0x2c) + slot.command : &aiDefaultCommands[slot.command - 200];
    if (record->codes[slot.step] == 127)
        return 1;
    input = AiAdjustPlayerSector(actor, record->codes[slot.step]);
    actor->inputFlags = input;
    slot.input = input;
    slot.inputTicks = record->ticks[slot.step];
    return 0;
hold:
    actor->inputFlags = slot.input;
    --slot.inputTicks;
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

std::uint16_t AiAdjustPlayerSector(FightActorRuntimePrefix *actor, std::uint16_t code) {
    if (actor->groupIndex == fightAiGame->playerSlot && (code & 0xf0U) != 0xf0U)
        code = VmRotateSector(actor, code);
    return code;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void AiAdvanceCommand(FightActorRuntimePrefix *actor) {
    FightAiSlot &slot = *static_cast<FightAiSlot *>(actor->slot);
    if (slot.waitFrames) {
        --slot.waitFrames;
        return;
    }
    if (AiUpdateInput(actor) != 1)
        return;
    slot.flags &= ~0x2000U;
    const std::uint8_t choice = slot.queuedCommands[slot.queuedCommandIndex];
    if (choice == 127) {
        AiResetPhase(actor);
        return;
    }
    const std::uint8_t delay = slot.queuedDelays[slot.queuedCommandIndex];
    ++slot.queuedCommandIndex;
    if (!(slot.random[1] & 7)) {
        AiResetPhase(actor);
        AiSelectCommand(actor, choice, delay);
    } else
        AiContinueCommand(actor, choice, delay);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void AiUpdateMetrics(FightActorRuntimePrefix *actor) {
    FightAiSlot &slot = *static_cast<FightAiSlot *>(actor->slot);
    FightActorRuntimePrefix *other = actor->actor;
    for (std::uint8_t index = 0; index < 6; ++index) {
        if (!fightAiGame->characters[index])
            continue;
        other = fightAiGame->actors[index];
        slot.healthDifferences[index] = actor->hp - other->hp;
        const std::int16_t *angles;
        slot.facingDifferences[index] = static_cast<std::int16_t>(
            std::fabs(static_cast<float>(static_cast<std::int16_t>(actor->facingAngle) - (angles = actor->actorFacingAngles)[index])));
    }
    slot.random[0] = dc::sdk::RandomInt();
    slot.random[1] = dc::sdk::RandomInt();
    slot.randomWord = dc::sdk::RandomInt();
    if (slot.ticks <= 65534)
        ++slot.ticks;
    if (slot.reactionTicks <= 59999)
        ++slot.reactionTicks;
    if (!(actor->hitStateFlags & 0x40000U)) {
        if (slot.hitTimer)
            --slot.hitTimer;
    } else
        slot.hitTimer = slot.hitTimerMaximum;
    if (!(actor->hitStateFlags & 0x100U)) {
        if (slot.guardTimer)
            --slot.guardTimer;
    } else
        slot.guardTimer = slot.guardTimerMaximum;
    if (actor->hitStateFlags & 0x1c000U)
        ++slot.guardTicks;
    else
        slot.guardTicks = slot.guardPhase = 0;
    if (other->hitStateFlags & 0x1c000U)
        ++slot.opponentGuardTicks;
    else
        slot.opponentGuardTicks = 0;
    if (other->hitStateFlags & 0x1000U) {
        if (slot.opponentAttackTimer <= 254)
            ++slot.opponentAttackTimer;
    } else if (slot.opponentAttackTimer)
        --slot.opponentAttackTimer;
    if (actor->hitStateFlags & 0x80000U) {
        if (slot.pressure <= 239)
            slot.pressure += 3;
    } else if (slot.pressure)
        --slot.pressure;
    if (slot.parameterA2)
        --slot.parameterA2;
    if (other->hitStateFlags & 0x10000000U) {
        if (slot.opponentMotionTimer <= 239)
            slot.opponentMotionTimer += 8;
    } else if (slot.opponentMotionTimer)
        --slot.opponentMotionTimer;
    slot.parameterB6 = 0;
    if ((actor->activeMotionFlags & 0x800000U) && slot.motionTicks <= 59999)
        ++slot.motionTicks;
    if (slot.hpPhaseTimer)
        --slot.hpPhaseTimer;
    if (slot.motionTimer)
        --slot.motionTimer;
    else if (actor->motionFlags & 0x80000000U)
        slot.motionTimer = 90;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void AiUpdateHistory(FightActorRuntimePrefix *actor) {
    FightAiSlot &slot = *static_cast<FightAiSlot *>(fightAiGame->actorSlots[actor->groupIndex]);
    FightActorRuntimePrefix *const other = fightAiGame->actors[SHENMUE_REACTION_VALUE(std::int8_t, actor, 1)];
    if ((other->motionFlags & 0x100U) && (actor->hitStateFlags & 0x24000U) &&
        SHENMUE_REACTION_VALUE(std::int16_t, actor, 0x42c) != SHENMUE_REACTION_VALUE(std::int16_t, actor, 0x430) && !(other->motionFlags & 2) &&
        !(actor->hitStateFlags & 0x10000U)) {
        slot.animationHistory[slot.animationHistoryIndex] = other->currentAnimation & 0xfffU;
        ++slot.animationHistoryIndex;
        if (slot.animationHistoryIndex > 3)
            slot.animationHistoryIndex = 0;
    }
    slot.flags &= ~0x400U;
    for (std::uint8_t i = 0; i < 3; ++i) {
        const std::uint16_t animation = slot.animationHistory[i];
        if (!animation)
            continue;
        for (std::uint8_t j = i + 1; j < 4; ++j)
            if (animation == slot.animationHistory[j]) {
                slot.flags |= 0x400U;
                slot.repeatedAnimation = animation;
                break;
            }
        if (slot.flags & 0x400U)
            break;
    }
    FightActorRuntimePrefix *const second = actor->secondActor;
    if (!(actor->previousHitStateFlags & 0x80000U) && (actor->hitStateFlags & 0x80000U) && second && !SHENMUE_REACTION_VALUE(std::uint16_t, second, 2) &&
        SHENMUE_REACTION_VALUE(std::uint32_t, second, 0x2f8)) {
        slot.inputHistory[slot.inputHistoryIndex] = SHENMUE_REACTION_VALUE(std::int8_t, second, 0x87f);
        ++slot.inputHistoryIndex;
        if (slot.inputHistoryIndex == 3)
            slot.inputHistoryIndex = 0;
    }
    slot.repeatedInputs = 0;
    if (second && !SHENMUE_REACTION_VALUE(std::uint16_t, second, 2) && slot.inputHistory[1])
        for (std::uint8_t index = 0; index < 3; ++index)
            if (SHENMUE_REACTION_VALUE(std::int8_t, second, 0x87f) == static_cast<std::uint32_t>(slot.inputHistory[index]))
                ++slot.repeatedInputs;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void AiUpdateRank(FightActorRuntimePrefix *actor) {
    FightAiSlot &slot = *static_cast<FightAiSlot *>(fightAiGame->actorSlots[actor->groupIndex]);
    std::uint8_t rank = SHENMUE_REACTION_VALUE(std::uint8_t, actor, 0x780);
    if (rank > 2)
        rank = 2;
    std::memcpy(&slot.attackChance, actor->motionParameters + 0x1a + rank * 9, 9);
    if (slot.idleCount > 20) {
        if (slot.attackChance)
            slot.attackChance = 255;
        slot.idleCount = 0;
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void AiUpdateAssistWindow(FightActorRuntimePrefix *actor) {
    const FightActorRuntimePrefix *const other = actor->actor;
    FightAiSlot &slot = *static_cast<FightAiSlot *>(actor->slot);
    float window;
    if (other->hitStateFlags & 0x1c000U)
        window = slot.assistWindowGuard;
    else if (other->groundContactFlags & 1)
        window = fightAiGame->type == 2 || fightAiGame->type == 4 ? 1.5f : 1.2f;
    else if (!(other->hitStateFlags & 0x104U))
        window = slot.assistWindowDefault;
    else
        window = 2.5f;
    slot.assistWindowCurrent = window;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

template <> std::int8_t AiPredicate<0>(FightActorRuntimePrefix *, FightAiSlot *) {
    return 0;
}

template <> std::int8_t AiPredicate<1>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    FightActorRuntimePrefix *const other = self->actor;
    if (runtime->guardTimer || (self->groundContactFlags & 1) || (self->hitStateFlags & 0xdd010U) || (other->hitStateFlags & 0xdd010U))
        return -1;
    if (!(self->contactDistances[AiTarget(self)] > 3.0f) && runtime->attackChance && runtime->attackChance >= runtime->random[0])
        return 1;
    return -1;
}

template <> std::int8_t AiPredicate<2>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    FightActorRuntimePrefix *const other = self->actor;
    const float previous = SHENMUE_REACTION_VALUE(float, self, 0x960);
    SHENMUE_REACTION_VALUE(float, self, 0x960) = 0;
    if ((self->hitStateFlags & 0x1d010U) || SHENMUE_REACTION_VALUE(std::uint32_t, other, 0x2f8))
        return -1;
    if ((self->secondaryMotionFlags & 0x8000000U) && (self->motionFlags & 4))
        return -1;
    const float current = runtime->assistWindowCurrent;
    const float distance = self->contactDistances[AiTarget(self)];
    if ((previous == 0 ? 0.5f : -0.3f) + current > distance)
        return -1;
    if ((self->motionFlags & 0x80000000U) && (!(other->motionFlags & 0x100U) || !(other->secondaryMotionFlags & 0x80000U))) {
        if (AiTarget(other) == self->groupIndex)
            return -1;
        FightRuntimePrefix *const game = self->game;
        for (std::uint8_t i = 0; i < 6; ++i) {
            FightActorRuntimePrefix *const actor = game->actors[i];
            if (game->characters[i] && actor->groupIndex != other->groupIndex && actor->groupIndex != self->groupIndex && actor->hp > actor->initialHp * 0.75f)
                return -1;
        }
    }
    if (!SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x94e)) {
        if ((self->groundContactFlags & 1) || distance > current + 2.0f)
            SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x94e) = 2;
        else
            SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x94e) = 1;
        if (reinterpret_cast<const std::uint8_t *>(self->motionParameters)[25] & 2)
            SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x94e) = 1;
        if (AiStyle(self) == 1)
            SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x94e) = 1;
    }
    SHENMUE_REACTION_VALUE(float, self, 0x960) = current;
    return 2;
}

template <> std::int8_t AiPredicate<3>(FightActorRuntimePrefix *self, FightAiSlot *) {
    FightActorRuntimePrefix *const other = self->actor;
    if ((self->groundContactFlags & 1) == 0)
        return -1;
    if ((self->hitStateFlags & 0x1d010U) == 0 && SHENMUE_REACTION_VALUE(std::uint32_t, other, 0x2f8) == 0)
        return 3;
    return -1;
}

template <> std::int8_t AiPredicate<4>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    FightActorRuntimePrefix *const other = self->actor;
    if (!runtime->guardPhase) {
        if (runtime->guardTicks > 90)
            return 4;
        if (runtime->guardTicks) {
            if (!SHENMUE_REACTION_VALUE(std::uint32_t, other, 0x2f8))
                return 4;
        }
        return -1;
    }
    return 4;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int8_t AiGuardProximity(FightActorRuntimePrefix *self, FightAiSlot &slot) {
    FightActorRuntimePrefix *const other = self->actor;
    if ((other->hitStateFlags & 0x104U) != 0x100U || (self->hitStateFlags & 0x1000U) || (slot.flags & 0x2000U) || (self->activeMotionFlags & 0x4000U))
        return -1;
    if (SHENMUE_REACTION_VALUE(std::int16_t, self, 0x3a0) > 1)
        return -1;
    if (self->contactDistances[AiTarget(self)] > 2.5f)
        return -1;
    return 5;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int8_t AiGuardPhase(FightActorRuntimePrefix *self, FightAiSlot &slot) {
    const std::uint8_t guard = reinterpret_cast<const FightAiActorView *>(self->actor)->guardState;
    FightAiPhase *phase;
    switch (guard) {
    case 0:
    case 3:
        phase = &slot.phases[0];
        break;
    case 1:
        phase = &slot.phases[1];
        break;
    case 2:
    case 5:
    case 6:
        phase = &slot.phases[2];
        break;
    default:
        goto unavailable;
    }
    if ((self->savedMotionFlags & 0x8000U) && slot.random[0] <= 128)
        return 5;
    if (!AiPhaseSelected(*phase)) {
unavailable:
        return -1;
    }
    return 5;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

template <> std::int8_t AiPredicate<5>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    if (AiGuardProximity(self, *runtime) == -1)
        return -1;
    if (!(static_cast<std::uint8_t>(self->motionParameters[25]) & 2) && AiGuardPhase(self, *runtime) == -1)
        return -1;
    return 5;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int8_t AiAttackChance(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    FightActorRuntimePrefix *const other = self->actor;
    if ((self->reactionFlags & 1U) == 0 || (self->hitStateFlags & 0x1d110U) || (other->hitStateFlags & 0x1d118U) || runtime->hitTimer)
        return -1;
    if (runtime->opponentMotionTimer > 128 && runtime->reactionChance)
        return 6;
    if (runtime->reactionChance && runtime->reactionChance >= runtime->random[0])
        return 6;
    return -1;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

template <> std::int8_t AiPredicate<9>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    if (AiAttackChance(self, runtime) == -1)
        return -1;
    for (std::uint8_t i = 0; i < 6; ++i) {
        if (!self->game->characters[i])
            continue;
        FightActorRuntimePrefix *actor = self->game->actors[i];
        if (AiStyle(actor) != 11)
            continue;
        if (std::fabs(actor->hp) < 0.00000011920929f)
            continue;
        if (runtime->facingDifferences[i] > 0x4000 && self->contactDistances[i] < 2.5f)
            return 6;
        return -1;
    }
    return -1;
}

template <> std::int8_t AiPredicate<7>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    if (!(self->hitStateFlags & 0x80000U) || (runtime->flags & 0x4000U))
        return -1;
    FightActorRuntimePrefix *const second = self->secondActor;
    if (!second || !SHENMUE_REACTION_VALUE(std::uint32_t, second, 0x2f8))
        return -1;
    const std::uint16_t animation = second->currentAnimation & 0xfffU;
    if (runtime->knownAnimationCount && (animation == 259 || animation == 223 || animation == 30))
        return 15;
    for (std::uint8_t index = 0; index < runtime->knownAnimationCount; ++index)
        if (runtime->knownAnimations[index] == animation)
            return 15;
    if (AiSlot(self).parameter175 == 1 && (self->savedSecondaryMotionFlags & 4))
        return -1;
    if (runtime->knownAnimationCount > 31)
        return -1;
    runtime->knownAnimations[runtime->knownAnimationCount++] = animation;
    runtime->flags |= 0x4000U;
    return -1;
}

template <> std::int8_t AiPredicate<8>(FightActorRuntimePrefix *self, FightAiSlot *) {
    FightRuntimePrefix *const game = self->game;
    for (std::uint8_t i = 0; i < 6; ++i)
        if (game->characters[i] && AiStyle(game->actors[i]) == 12 && !static_cast<FightAiSlot *>(game->actorSlots[i])->hitTimer)
            return 8;
    return -1;
}

template <> std::int8_t AiPredicate<11>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    if ((self->reactionFlags & 8) && runtime->reactionChance && runtime->reactionChance >= runtime->random[0])
        return 11;
    return -1;
}

template <> std::int8_t AiPredicate<12>(FightActorRuntimePrefix *self, FightAiSlot *) {
    FightActorRuntimePrefix *const other = self->actor;
    if (other->hp > other->initialHp * 0.25f)
        return -1;
    if (self->contactDistances[AiTarget(self)] > 1.25f)
        return -1;
    if ((self->hitStateFlags & 0x1c000U) || (other->hitStateFlags & 0x1d112U) != 0x102U)
        return -1;
    if (self->groundContactFlags & 1)
        return -1;
    return 12;
}

template <> std::int8_t AiPredicate<13>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    FightActorRuntimePrefix *const other = self->actor;
    if (other->animationTime > SHENMUE_REACTION_VALUE(std::uint16_t, other, 0x290) && (other->hitStateFlags & 0x100U))
        return -1;
    if (other->hitStateFlags & 0x1d014U)
        return -1;
    if (self->contactDistances[AiTarget(self)] > 2.25f)
        return -1;
    if (runtime->facingDifferences[AiTarget(self)] <= runtime->facingLimit && runtime->guardChance && runtime->guardChance >= runtime->random[0])
        return 13;
    return -1;
}

template <> std::int8_t AiPredicate<14>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    FightActorRuntimePrefix *const other = self->actor;
    std::uint8_t ready = 0;
    if ((other->groundContactFlags & 1) || (other->activeMotionFlags & 4) ||
        ((other->motionFlags & 0x100U) && (other->hitStateFlags & 4) &&
            SHENMUE_REACTION_VALUE(std::uint16_t, other, 0x294) - other->animationTime > SHENMUE_REACTION_VALUE(float, other, 0x26c) * 5.0f))
        ready = 1;
    if (!ready)
        return -1;
    if (self->contactDistances[AiTarget(self)] > 2.0f)
        return -1;
    std::uint32_t sum = runtime->attackChance + runtime->guardChance;
    if (sum > 255)
        sum = 255;
    const std::uint8_t chance = sum;
    if (chance && chance >= runtime->random[0])
        return 1;
    return -1;
}

template <> std::int8_t AiPredicate<15>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    if (!(self->hitStateFlags & 0x80000U))
        return -1;
    FightActorRuntimePrefix *second = self->secondActor;
    if (!second || !SHENMUE_REACTION_VALUE(std::uint32_t, second, 0x2f8))
        return -1;
    if (static_cast<std::uint8_t>(self->motionParameters[25]) & 2)
        return 15;
    if (!AiPhaseSelected(runtime->phases[5]))
        return -1;
    if (static_cast<std::uint8_t>(static_cast<FightAiSlot *>(self->slot)->parameter175) == 1 && (self->savedSecondaryMotionFlags & 4))
        return -1;
    return 15;
}

template <> std::int8_t AiPredicate<17>(FightActorRuntimePrefix *self, FightAiSlot *) {
    std::uint32_t frame = 0;
    FightActorRuntimePrefix *const other = self->actor;
    switch (AiStyle(self)) {
    case 9: {
        const std::int8_t rank = SHENMUE_REACTION_VALUE(std::int8_t, self, 0x780);
        if (!rank)
            return -1;
        if (rank == 3)
            frame = 300;
        break;
    }
    case 31:
        if (SHENMUE_REACTION_VALUE(std::int8_t, self, 0x780) != 2)
            return -1;
        frame = 150;
        break;
    default:
        return -1;
    }
    if (self->game->elapsedTicks < frame)
        return -1;
    if ((self->reactionFlags & 1) == 0)
        return -1;
    if ((self->hitStateFlags & 0x1d110U) || (other->hitStateFlags & 0x1d118U))
        return -1;
    return 17;
}

template <> std::int8_t AiPredicate<18>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    FightActorRuntimePrefix *other = fightAiGame->actors[AiTarget(self)];
    if (AiGuardProximity(self, *runtime) == -1)
        return -1;
    if ((runtime->flags & 0x400U) && (other->currentAnimation & 0xfffU) == runtime->repeatedAnimation)
        return 5;
    if (AiGuardPhase(self, *runtime) != -1)
        return 5;
    return -1;
}

template <> std::int8_t AiPredicate<19>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    FightActorRuntimePrefix *const other = self->actor;
    if ((other->groundContactFlags & 1) ||
        static_cast<std::int16_t>(
            std::fabs(static_cast<float>(static_cast<std::int16_t>(other->actorFacingAngles[self->groupIndex] - other->temporaryFacingAngle)))) > 8192)
        return 29;
    if ((other->motionFlags & 0x100U) == 0 && (other->activeMotionFlags & 0x4000U))
        return -1;
    if (AiGuardProximity(self, *runtime) == -1)
        return -1;
    return 5;
}

template <> std::int8_t AiPredicate<21>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    if ((self->reactionFlags & 0x6000000U) && runtime->reactionChance && runtime->reactionChance >= runtime->random[0])
        return 21;
    return -1;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int8_t AiHitPhase(FightActorRuntimePrefix *self, FightAiSlot &slot) {
    FightActorRuntimePrefix *other = fightAiGame->actors[AiTarget(self)];
    if (SHENMUE_REACTION_VALUE(std::uint16_t, other, 0x290) > 4 &&
        SHENMUE_REACTION_VALUE(std::int16_t, other, 0x290) - static_cast<std::int16_t>(static_cast<std::int32_t>(other->animationTime)) > 4)
        return -1;
    if (!AiPhaseSelected(slot.phases[4]))
        return -1;
    return 22;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

template <> std::int8_t AiPredicate<22>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    FightActorRuntimePrefix *const other = self->actor;
    if ((self->hitFlags & 0x2400U) == 0)
        return -1;
    if (AiHitPhase(self, *runtime) == -1)
        return -1;
    if ((self->moveFlags & 0x40000U) == 0)
        return 22;
    if (SHENMUE_REACTION_VALUE(std::uint8_t, other, 0x2a9) == 3 || SHENMUE_REACTION_VALUE(std::uint16_t, other, 0x298) == 127)
        return -1;
    if (other->activeMotionFlags & 0x10000000U)
        return 22;
    return -1;
}

template <> std::int8_t AiPredicate<35>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    if ((self->hitFlags & 0x400U) && AiHitPhase(self, *runtime) != -1)
        return 22;
    return -1;
}

template <> std::int8_t AiPredicate<23>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    if ((self->hitFlags & 0x5000U) && AiHitPhase(self, *runtime) != -1)
        return 23;
    return -1;
}

template <> std::int8_t AiPredicate<24>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    if ((self->hitFlags & 0x8800U) && AiHitPhase(self, *runtime) != -1)
        return 24;
    return -1;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int8_t AiPairGuard(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    FightActorRuntimePrefix *const other = self->actor;
    if ((self->motionFlags & 0x80000000U) && (other->motionFlags & 0x100U) &&
        SHENMUE_REACTION_VALUE(float, other, 0x2ac) > self->contactDistances[AiTarget(self)] && AiTarget(other) == self->groupIndex)
        return 26;
    if (!(other->hitStateFlags & 0x10000100U) || (other->hitStateFlags & 4) || (self->hitStateFlags & 0x1000U) || !(other->hitStateFlags & 0x100U) ||
        self->contactDistances[AiTarget(self)] > 2.0f || !AiPhaseSelected(runtime->phases[3]))
        return -1;
    return 26;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

template <> std::int8_t AiPredicate<27>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    FightActorRuntimePrefix *const other = self->actor;
    if (self->moveFlags & 0x40000U)
        return -1;
    if ((other->hitStateFlags & 0x100U) &&
        static_cast<std::int16_t>(static_cast<std::int32_t>(other->animationTime)) < SHENMUE_REACTION_VALUE(std::uint16_t, other, 0x292))
        return -1;
    if ((self->groundContactFlags | other->groundContactFlags) & 1)
        return -1;
    if (other->hitStateFlags & 0x1c000U)
        return -1;
    if (self->contactDistances[AiTarget(self)] > runtime->assistWindowCurrent + -1.0f)
        return -1;
    return 27;
}

template <> std::int8_t AiPredicate<28>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    FightActorRuntimePrefix *const other = self->actor;
    const std::int16_t time = static_cast<std::int16_t>(static_cast<std::int32_t>(other->animationTime));
    const std::int16_t middle = SHENMUE_REACTION_VALUE(std::int16_t, other, 0x290);
    const std::int8_t target = AiTarget(self);
    const float reach = SHENMUE_REACTION_VALUE(float, other, 0x2ac);
    float distance = self->contactDistances[target];
    if (!(other->hitStateFlags & 0x100U) && (other->activeMotionFlags & 0x4000U)) {
        runtime->animationPhase = 0;
        return -1;
    }
    if (distance > 1.8000001f) {
        if (other->activeMotionFlags & 0x80000U)
            return (other->hitStateFlags & 0x4000U) && AiPhaseSelected(runtime->phases[3]) ? 28 : -1;
    } else if (other->activeMotionFlags & 0x80000U)
        return (other->secondaryMotionFlags & 0x80000U) && time > SHENMUE_REACTION_VALUE(std::uint16_t, other, 0x292) ? -1 : 28;
    if (SHENMUE_REACTION_VALUE(float, self, 0x3b8) < 0.5f && SHENMUE_REACTION_VALUE(float, self, 0x3b4) < 0.5f)
        return -1;
    if (AiPairGuard(self, runtime) == -1) {
        if (!(distance < 1.0f) || (other->groundContactFlags & 1))
            return -1;
        return 28;
    }
    if (static_cast<std::uint16_t>(other->temporaryFacingAngle - AiAngle(self, target)) - 16380U > 0x7ff8U)
        return -1;
    if (!(distance > reach)) {
        if (runtime->animationPhase == 1)
            return -1;
        float remaining = reach - reach / SHENMUE_REACTION_VALUE(std::uint16_t, other, 0x290) * other->animationTime;
        if (remaining < 0)
            remaining = 0;
        distance -= remaining;
        if (!(reach > distance && distance > 1.0f)) {
            if (other->moveFlags & 1)
                return 28;
            if (self->activeMotionFlags & 0x2000000U)
                return 28;
            if (SHENMUE_REACTION_VALUE(std::int8_t, other, 0x2a9) == 3 && (other->activeMotionFlags & 0x10000000U))
                return middle - time > 5 ? -1 : 28;
            return middle - time > 3 ? -1 : 28;
        }
    }
    runtime->animationPhase = 1;
    return -1;
}

template <> std::int8_t AiPredicate<29>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    return AiPredicate<3>(self, runtime) != -1 ? 29 : -1;
}

template <> std::int8_t AiPredicate<30>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    if (runtime->hitTimer || (self->reactionFlags & 1) == 0 ||
        (2.5f > SHENMUE_REACTION_VALUE(float, self, 0x3ac) && 2.5f > SHENMUE_REACTION_VALUE(float, self, 0x3b0)))
        return -1;
    if (!(SHENMUE_REACTION_VALUE(float, self, 0x3ac) > 1.7f))
        return 30;
    if (!(SHENMUE_REACTION_VALUE(float, self, 0x3b0) > 0.2f))
        return 30;
    if (AiAttackChance(self, runtime) != -1)
        return 30;
    return -1;
}

template <> std::int8_t AiPredicate<31>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    if ((runtime->flags & 1) == 0 || runtime->facingDifferences[AiTarget(self)] < runtime->turningLimit)
        return -1;
    FightActorRuntimePrefix *const other = self->actor;
    if (static_cast<std::int16_t>(runtime->facingDifferences[AiTarget(self)]) > 24576 || SHENMUE_REACTION_VALUE(std::uint32_t, other, 0x2f8) ||
        (self->hitStateFlags & 0xdc000U))
        return -1;
    if ((other->hitStateFlags & 0x100U) && 2.7f > self->contactDistances[AiTarget(self)])
        return -1;
    return 31;
}

template <> std::int8_t AiPredicate<32>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    if (!(self->hitStateFlags & 0x20000000U))
        return -1;
    if (!(self->contactDistances[AiTarget(self)] > SHENMUE_REACTION_VALUE(float, self, 0x960) + 0.2f) && runtime->attackChance &&
        runtime->attackChance >= runtime->random[0])
        return 32;
    return -1;
}

template <> std::int8_t AiPredicate<40>(FightActorRuntimePrefix *, FightAiSlot *) {
    return -1;
}

template <> std::int8_t AiPredicate<41>(FightActorRuntimePrefix *, FightAiSlot *) {
    return -1;
}

template <> std::int8_t AiPredicate<42>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    FightActorRuntimePrefix *const other = self->actor;
    if (self->hitStateFlags & 0x40000U)
        return -1;
    if (!(other->hitStateFlags & 0x100U))
        return -1;
    if (self->activeMotionFlags & 0x2000000U)
        return -1;
    if (runtime->flags & 0x2000U)
        return -1;
    if (static_cast<std::uint16_t>(static_cast<std::int32_t>(other->animationTime + 2.0f)) < SHENMUE_REACTION_VALUE(std::uint16_t, other, 0x290))
        return -1;
    if (self->contactDistances[AiTarget(self)] > 2.5f)
        return -1;
    return 5;
}

template <> std::int8_t AiPredicate<44>(FightActorRuntimePrefix *, FightAiSlot *) {
    return -1;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

std::uint8_t aiSidestepCommands[4] = {201, 202, 201, 202};

template <> std::int8_t AiPredicate<45>(FightActorRuntimePrefix *self, FightAiSlot *) {
    std::int32_t state = 0;
    switch (reinterpret_cast<const std::uint8_t *>(self->motionParameters)[110]) {
    case 2:
        break;
    case 4:
        state = SHENMUE_REACTION_VALUE(std::int8_t, self, 0x780) <= 0 ? 4 : 0;
        break;
    case 3:
        if (self->game->elapsedTicks > 300) {
            FightAiSlot *const own = static_cast<FightAiSlot *>(self->slot);
            if (!((self->secondaryMotionFlags & 2) ||
                    ((self->reactionFlags & 1) && !(self->hitStateFlags & 0x1d110U) && !(self->actor->hitStateFlags & 0x1d118U))) ||
                own->hitTimer)
                state = 3;
            else if (own->opponentMotionTimer > 128 && own->reactionChance)
                state = 2;
            if (!state) {
                if (!own->reactionChance || own->reactionChance < own->random[0])
                    return -1;
                state = 2;
            }
        } else
            state = 0;
        break;
    }
    switch (state) {
    case 0:
        return -1;
    case 1:
        return 1;
    case 2:
        return 6;
    case 3:
        return -1;
    case 4:
    default:
        return 45;
    }
}

template <> std::int8_t AiPredicate<50>(FightActorRuntimePrefix *, FightAiSlot *) {
    return -1;
}

template <> std::int8_t AiPredicate<51>(FightActorRuntimePrefix *, FightAiSlot *) {
    return -1;
}

template <> std::int8_t AiPredicate<53>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    FightActorRuntimePrefix *const other = self->actor;
    if ((self->groundContactFlags & 1) || SHENMUE_REACTION_VALUE(std::uint32_t, other, 0x2f8) || (self->hitStateFlags & 0x2001d010U) ||
        (self->activeMotionFlags & 0x8000000U))
        return -1;
    const std::uint8_t group = fightAiGame->enemySlots[self->groupIndex]->counterGroup;
    if (g_pFightStage->unknownGroupCounts[group] < 2)
        return -1;
    if (other->hitStateFlags & 0x20000000U)
        return -1;
    if (self->contactDistances[AiTarget(self)] > runtime->assistWindowCurrent + -0.2f)
        return -1;
    if (self->contactDistances[AiTarget(self)] < runtime->assistWindowCurrent + -0.8f)
        return -1;
    return 53;
}

template <> std::int8_t AiPredicate<54>(FightActorRuntimePrefix *, FightAiSlot *) {
    return -1;
}

template <> std::int8_t AiPredicate<55>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    if (g_pFightStage->battleFlags & 0x20U)
        return -1;
    if (self->game->stateFlags & 0x2000U)
        return -1;
    for (std::uint8_t i = 0, team = fightAiGame->slotTeams[self->groupIndex]; i < 6; ++i)
        if (fightAiGame->characters[i] && fightAiGame->slotTeams[i] == team && i != self->groupIndex && (fightAiGame->actors[i]->motionFlags & 0x100U))
            return 0;
    if (fightAiGame->playerSlot != -1 && self->groupIndex != reinterpret_cast<FightAiActorView *>(fightAiGame->actors[fightAiGame->playerSlot])->focusTarget)
        runtime->attackChance >>= 2;
    return -1;
}

template <> std::int8_t AiPredicate<57>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    const float distance = self->contactDistances[AiTarget(self)];
    if (distance > self->game->contactRange)
        return -1;
    if (runtime->assistWindowCurrent + -1.0f > distance)
        return -1;
    return 57;
}

template <> std::int8_t AiPredicate<10>(FightActorRuntimePrefix *self, FightAiSlot *) {
    if (self->hitStateFlags & 0x10000000U)
        return -1;
    return 10;
}

template <> std::int8_t AiPredicate<33>(FightActorRuntimePrefix *self, FightAiSlot *) {
    FightActorRuntimePrefix *const other = self->actor;
    if (!(other->hitStateFlags & 0x100U))
        return -1;
    if (other->motionFlags & 0x10000000U)
        return -1;
    if (other->animationTime > SHENMUE_REACTION_VALUE(std::uint16_t, other, 0x290) ||
        !(3 > SHENMUE_REACTION_VALUE(std::uint16_t, other, 0x290) - other->animationTime))
        return -1;
    return 33;
}

template <> std::int8_t AiPredicate<34>(FightActorRuntimePrefix *self, FightAiSlot *) {
    FightActorRuntimePrefix *const other = self->actor;
    if ((static_cast<FightAiSlot *>(self->slot)->motionTimer && !(other->motionFlags & 0x1c000U)) || self->contactDistances[AiTarget(self)] > 4)
        return -1;
    if (AiTarget(other) != self->groupIndex) {
        FightRuntimePrefix *const game = self->game;
        for (std::uint8_t i = 0; i < 6; ++i) {
            FightActorRuntimePrefix *const actor = game->actors[i];
            if (game->characters[i] && actor->groupIndex != other->groupIndex && actor->groupIndex != self->groupIndex && actor->hp < actor->initialHp * 0.75f)
                return -1;
        }
        if (self->animationTime < SHENMUE_REACTION_VALUE(float, self, 0x26c))
            return 34;
    }
    if (!(other->motionFlags & 0x100U) && !(other->secondaryMotionFlags & 0x80000U) && !(self->motionFlags & 0x80000000U))
        return 34;
    return -1;
}

template <> std::int8_t AiPredicate<25>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    const std::int16_t angles[16] = {6656, 8192, 9472, 12288, 12288, 13056, 13056, 13824, 13824, 14592, 14592, 16896, 16896, 17664, 17664, 17664};
    const std::uint8_t chances[16] = {0, 0, 0, 8, 8, 8, 19, 38, 57, 76, 95, 114, 133, 152, 166, 190};
    FightAiActorView *const view = reinterpret_cast<FightAiActorView *>(self);
    runtime->random[1] = dc::sdk::RandomInt();
    const std::int16_t angle = static_cast<std::int16_t>(self->facingAngle - view->targetAngles[view->target]);
    const std::int16_t absolute = static_cast<FightAiSlot *>(self->slot)->facingDifferences[view->target];
    const std::uint8_t index = vfMotnNibbleGet(67);
    if (self->contactDistances[view->target] > 2 || angle > 0 || angles[index] > absolute || absolute < -24576 ||
        static_cast<FightAiSlot *>(self->slot)->secondaryMotionTimer || SHENMUE_REACTION_VALUE(std::int8_t, self, 0x780) == 2)
        return -1;
    if (chances[index] && chances[index] >= runtime->random[1])
        return 34;
    return 25;
}

template <> std::int8_t AiPredicate<16>(FightActorRuntimePrefix *self, FightAiSlot *) {
    FightActorRuntimePrefix *const other = self->actor;
    FightAiSlot &own = *static_cast<FightAiSlot *>(self->slot);
    if (other->hitStateFlags & 0x100U) {
        if (self->contactDistances[AiTarget(self)] > own.assistWindowDefault)
            return 5;
    }
    if ((other->hitStateFlags & 0x100U) && (other->savedMotionFlags & 0x100U)) {
        std::uint32_t chance = !(other->motionFlags & 0x80U) && (other->previousHitStateFlags & 0x100U) ? own.parameter1f + own.guardChance * 2 :
                                                                                                          own.parameter1f + own.guardChance;
        if (chance > 255)
            chance = 255;
        const std::uint8_t clamped = chance;
        if (clamped && clamped >= own.random[0])
            return 5;
    }
    if (AiGuardProximity(self, own) != -1) {
        if ((own.flags & 0x400U) && (other->currentAnimation & 0xfffU) == own.repeatedAnimation)
            return 5;
        if (AiGuardPhase(self, own) != -1)
            return 5;
    }
    return -1;
}

template <> void AiPost<0>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    FightAiSlot &slot = *runtime;
    if (self && !(slot.ticks & 15)) {
        slot.random[0] = dc::sdk::RandomInt();
        ++slot.idleCount;
    }
}

template <> void AiPost<1>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    const std::uint8_t *const authored = reinterpret_cast<const std::uint8_t *>(self->motionParameters);
    if (runtime->random[1] & 0x70) {
        const std::uint8_t *choices = authored + 53;
        if ((AiStyle(self) == 10 || AiStyle(self) == 11) && SHENMUE_REACTION_VALUE(std::int8_t, self, 0x780))
            choices += 8;
        AiChooseCommand(self, choices, 8, 0);
    } else
        AiSelectCommand(self, static_cast<std::int32_t>(dc::sdk::RandomFloat() * authored[24]), 0);
}

template <> void AiPost<2>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    FightActorRuntimePrefix *const other = fightAiGame->actors[static_cast<std::int8_t>(runtime->opponent)];
    self->inputFlags = 0x2000;
    if (SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x94e) == 1 &&
        (self->contactDistances[static_cast<std::int8_t>(runtime->opponent)] > 5 || (other->hitStateFlags & 0x20000000U))) {
        SHENMUE_REACTION_VALUE(std::int8_t, self, 0x94e) = 2;
        if (reinterpret_cast<const std::uint8_t *>(self->motionParameters)[25] & 2)
            SHENMUE_REACTION_VALUE(std::int8_t, self, 0x94e) = 1;
        if (AiStyle(self) == 1)
            SHENMUE_REACTION_VALUE(std::int8_t, self, 0x94e) = 1;
    }
    SHENMUE_REACTION_VALUE(std::int8_t, self, 0x9a4) = -1;
    SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x94f) = 0;
    SHENMUE_REACTION_VALUE(float, self, 0x950) = other->position.x;
    SHENMUE_REACTION_VALUE(float, self, 0x958) = other->position.z;
}

template <> void AiPost<3>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    const std::uint8_t *const authored = reinterpret_cast<const std::uint8_t *>(self->motionParameters);
    if (self->contactDistances[AiTarget(self)] < 2 && runtime->attackChance && runtime->attackChance >= runtime->random[1])
        AiChooseCommand(self, authored + 77, 4, 0);
    else
        AiChooseCommand(self, aiSidestepCommands, 4, 0);
}

template <> void AiPost<4>(FightActorRuntimePrefix *self, FightAiSlot *) {
    if (fightAiGame->elapsedTicks & 2)
        self->inputFlags = 32;
    else
        self->inputFlags = 16;
}

template <> void AiPost<5>(FightActorRuntimePrefix *self, FightAiSlot *) {
    AiResetPhase(self);
    SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x5b0) = 0;
    SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x5b4) = SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x28);
    SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5a8) = 0;
    SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x5a6) = 0;
    AiSelectCommand(self, 217, 0);
}

template <> void AiPost<6>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    const std::uint8_t *const authored = reinterpret_cast<const std::uint8_t *>(self->motionParameters);
    const std::uint8_t style = authored[110];
    const std::uint8_t *choices = authored + 81;
    if (self->game->elapsedTicks <= 599 && style == 3)
        choices += 2;
    AiChooseCommand(self, choices, 4, runtime->commandDelay);
}

template <> void AiPost<8>(FightActorRuntimePrefix *self, FightAiSlot *) {
    FightRuntimePrefix *const game = self->game;
    for (std::uint8_t i = 0; i < 6; ++i) {
        if (!game->characters[i])
            continue;
        FightActorRuntimePrefix *const other = game->actors[i];
        if (AiStyle(other) != 12)
            continue;
        Vector3 local, point;
        local.x = -1;
        local.y = 0;
        local.z = 1;
        vfRotateFightVector(-AiAngle(other, game->playerSlot), &local, &point);
        point.x += other->position.x;
        point.y += other->position.y;
        point.z += other->position.z;
        const float dx = self->position.x - point.x, dz = self->position.z - point.z;
        if (std::sqrt(dx * dx + dz * dz) > 1) {
            self->inputFlags = 0x2000;
            SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x94e) = 2;
            SHENMUE_REACTION_VALUE(float, self, 0x950) = point.x;
            SHENMUE_REACTION_VALUE(float, self, 0x958) = point.z;
        }
    }
}

template <> void AiPost<11>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    AiChooseCommand(self, reinterpret_cast<const std::uint8_t *>(self->motionParameters) + 85, 2, runtime->commandDelay);
}

template <> void AiPost<12>(FightActorRuntimePrefix *self, FightAiSlot *) {
    FightActorRuntimePrefix *other = fightAiGame->actors[AiTarget(self)];
    const std::uint8_t *const authored = reinterpret_cast<const std::uint8_t *>(self->motionParameters);
    if (reinterpret_cast<FightAiActorView *>(other)->impactFrame - static_cast<std::int16_t>(other->animationTime) <= 4)
        AiChooseCommand(self, authored + 97, 1, 0);
}

template <> void AiPost<13>(FightActorRuntimePrefix *self, FightAiSlot *) {
    FightActorRuntimePrefix *other = fightAiGame->actors[AiTarget(self)];
    const std::uint8_t *const authored = reinterpret_cast<const std::uint8_t *>(self->motionParameters);
    if ((other->hitStateFlags & 0x200U) || reinterpret_cast<FightAiActorView *>(other)->impactFrame - static_cast<std::int16_t>(other->animationTime) > 4) {
        const std::uint8_t *choices = authored + 102;
        if (self->contactDistances[AiTarget(self)] > 2.2f)
            choices += 2;
        AiChooseCommand(self, choices, 2, 0);
    }
}

template <> void AiPost<15>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    AiResetPhase(self);
    if (!runtime->reactionTicks) {
        self->inputFlags = 0;
        return;
    }
    switch (static_cast<std::uint16_t>(runtime->reactionTicks - 1)) {
    case 0:
    case 1:
        self->inputFlags = 4;
        break;
    default:
        self->inputFlags = 0;
        break;
    }
}

template <> void AiPost<17>(FightActorRuntimePrefix *self, FightAiSlot *) {
    AiSelectCommand(self, 209, 0);
}

template <> void AiPost<21>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    AiChooseCommand(self, reinterpret_cast<const std::uint8_t *>(self->motionParameters) + 87, 2, runtime->commandDelay);
}

template <> void AiPost<22>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    if (!(self->moveFlags & 0x100U)) {
        AiSelectCommand(self, 205, 0);
        runtime->flags |= 0x2000U;
    }
}

template <> void AiPost<23>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    if (!(self->moveFlags & 0x100U)) {
        AiSelectCommand(self, 206, 0);
        runtime->flags |= 0x2000U;
    }
}

template <> void AiPost<24>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    if (self->moveFlags & 0x100U) {
        AiSelectCommand(self, 207, 0);
        runtime->flags |= 0x2000U;
    }
}

template <> void AiPost<26>(FightActorRuntimePrefix *self, FightAiSlot *) {
    FightActorRuntimePrefix *other = fightAiGame->actors[AiTarget(self)];
    AiResetPhase(self);
    std::uint16_t code = 0;
    if (self->game->elapsedTicks & 1) {
        const std::int16_t direction = SHENMUE_REACTION_VALUE(std::int16_t, other, 0x2b2);
        if (!direction)
            code = 24;
        else if (direction < 0) {
            if (SHENMUE_REACTION_VALUE(float, self, 0x3b8) > 3)
                code = 72;
            else
                code = 136;
        } else {
            if (SHENMUE_REACTION_VALUE(float, self, 0x3b4) > 3)
                code = 136;
            else
                code = 72;
        }
        if (SHENMUE_REACTION_VALUE(std::int16_t, other, 0x2c2) - static_cast<std::int16_t>(other->animationTime) <= 3 &&
            SHENMUE_REACTION_VALUE(std::uint16_t, other, 0x2c2))
            code = 24;
    }
    self->inputFlags = code;
}

template <> void AiPost<27>(FightActorRuntimePrefix *self, FightAiSlot *) {
    static std::uint8_t choices[4] = {204, 204, 201, 202};
    AiChooseCommand(self, choices, 4, 0);
}

template <> void AiPost<28>(FightActorRuntimePrefix *self, FightAiSlot *) {
    static std::uint8_t choices[8] = {204, 204, 201, 202, 202, 201, 217, 217};
    std::uint8_t index = 0;
    FightActorRuntimePrefix *const other = self->actor;
    const std::uint8_t guard = SHENMUE_REACTION_VALUE(std::uint8_t, other, 0x2a9);
    const std::uint32_t category = vfSelectHitReactionSequence(other);
    const std::int32_t direction = SHENMUE_REACTION_VALUE(std::uint16_t, other, 0x2b2);
    if ((other->activeMotionFlags & 0x10000000U) && guard == 3)
        index = 6;
    else if ((other->activeMotionFlags & 0x10000000U) && (guard == 2 || guard == 5 || guard == 6))
        index = 0;
    else if (category & 0x24U)
        index = 2;
    else if (category & 0x18U)
        index = 4;
    else if (other->activeMotionFlags & 0x100U)
        index = 2;
    if (static_cast<std::uint32_t>(direction - 1821) <= 0xf1c5U && (((category & 0x24U) && direction > 0x8000) || ((category & 0x18U) && direction <= 0x7fff)))
        ++index;
    AiSelectCommand(self, choices[index], 0);
}

template <> void AiPost<29>(FightActorRuntimePrefix *self, FightAiSlot *) {
    AiChooseCommand(self, aiSidestepCommands, 4, 0);
}

template <> void AiPost<30>(FightActorRuntimePrefix *self, FightAiSlot *) {
    static std::uint8_t choices[6] = {17, 20, 21, 18, 22, 23};
    std::uint8_t offset = 0;
    if (SHENMUE_REACTION_VALUE(float, self, 0x3b0) > 2.5f) {
        if (1.7f > SHENMUE_REACTION_VALUE(float, self, 0x3ac) || (dc::sdk::RandomInt() & 1))
            offset = 3;
    } else if (2.5f > SHENMUE_REACTION_VALUE(float, self, 0x3ac)) {
        AiSelectCommand(self, 15, 0);
        return;
    }
    AiChooseCommand(self, choices + offset, 3, 0);
}

template <> void AiPost<31>(FightActorRuntimePrefix *self, FightAiSlot *) {
    AiResetPhase(self);
    self->inputFlags = 32;
}

template <> void AiPost<32>(FightActorRuntimePrefix *self, FightAiSlot *) {
    const std::uint8_t *const authored = reinterpret_cast<const std::uint8_t *>(self->motionParameters);
    AiResetPhase(self);
    AiChooseCommand(self, authored + 93, 4, 0);
}

template <> void AiPost<40>(FightActorRuntimePrefix *, FightAiSlot *) {
}

template <> void AiPost<41>(FightActorRuntimePrefix *, FightAiSlot *) {
}

template <> void AiPost<44>(FightActorRuntimePrefix *, FightAiSlot *) {
}

template <> void AiPost<45>(FightActorRuntimePrefix *self, FightAiSlot *) {
    static std::uint8_t commands[1] = {13};
    const std::uint8_t *const authored = reinterpret_cast<const std::uint8_t *>(self->motionParameters);
    const float health = self->hp / self->initialHp;
    switch (authored[110]) {
    case 0:
    case 1:
    case 2:
    case 3:
        break;
    case 4:
        if (0.5f > health)
            AiSelectCommand(self, commands[0], 0);
        break;
    }
}

template <> void AiPost<50>(FightActorRuntimePrefix *, FightAiSlot *) {
}

template <> void AiPost<51>(FightActorRuntimePrefix *, FightAiSlot *) {
}

template <> void AiPost<53>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    FightActorRuntimePrefix *const other = self->actor;
    const std::uint8_t target = reinterpret_cast<FightAiGameView *>(fightAiGame)->slotTargets[self->groupIndex];
    if (!(runtime->reactionTicks & 31)) {
        if (dc::sdk::RandomInt() & 1)
            reinterpret_cast<FightAiGameView *>(fightAiGame)->targetSides[target] = 1;
        else
            reinterpret_cast<FightAiGameView *>(fightAiGame)->targetSides[target] = 2;
        if (other->activeMotionFlags & 0x800U) {
            if (static_cast<std::int16_t>(other->facingAngle) - reinterpret_cast<FightAiActorView *>(other)->targetAngles[target] < 0)
                reinterpret_cast<FightAiGameView *>(fightAiGame)->targetSides[target] = 1;
            else
                reinterpret_cast<FightAiGameView *>(fightAiGame)->targetSides[target] = 2;
        }
    }
    if (reinterpret_cast<FightAiGameView *>(fightAiGame)->targetSides[target] == 2)
        self->inputFlags = 0x1040;
    else
        self->inputFlags = 0x1080;
}

template <> void AiPost<57>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    if (!(runtime->ticks & 32)) {
        const float distance = self->contactDistances[AiTarget(self)];
        if (!(runtime->ticks & 63)) {
            std::uint8_t sector;
            if (distance > 3)
                sector = 0;
            else if (runtime->assistWindowCurrent + -0.7f > distance)
                sector = 4;
            else
                sector = dc::sdk::RandomInt() & 7;
            runtime->sector = sector;
        }
        self->inputFlags = aiCameraSectors[runtime->sector];
    } else
        self->inputFlags = 0;
}

template <> void AiPost<25>(FightActorRuntimePrefix *self, FightAiSlot *) {
    AiSelectCommand(self, 209, 0);
}

template <> void AiPost<33>(FightActorRuntimePrefix *self, FightAiSlot *) {
    AiSelectCommand(self, 217, 0);
}

template <> void AiPost<34>(FightActorRuntimePrefix *self, FightAiSlot *) {
    AiSelectCommand(self, 218, 0);
}

template <> void AiPost<10>(FightActorRuntimePrefix *self, FightAiSlot *runtime) {
    std::uint16_t delay = 0;
    const float speeds[4] = {0.0055555557f, 0.0027777778f, 0.0033333334f, 0.0023809525f};
    runtime->random[0] = dc::sdk::RandomInt();
    runtime->random[1] = dc::sdk::RandomInt();
    FightActorRuntimePrefix *const other = self->actor;
    const std::uint8_t *const authored = reinterpret_cast<const std::uint8_t *>(self->motionParameters);
    const std::int32_t setting = vfMotnNibbleGet(66);
    if (self->game->elapsedTicks >= 150) {
        if (setting < 3)
            delay = static_cast<std::int32_t>(self->game->elapsedTicks * speeds[SHENMUE_REACTION_VALUE(std::int8_t, self, 0x780)]);
        else
            delay = static_cast<std::int32_t>(self->game->elapsedTicks * speeds[3]);
    }
    std::int32_t command = -1;
    if (self->contactDistances[other->groupIndex] > SHENMUE_REACTION_VALUE(float, other, 0x40c) + 1) {
        if (!(self->game->elapsedTicks & 63))
            command = dc::sdk::RandomInt() & 1 ? 201 : 202;
        goto select;
    }
    if (delay <= 1) {
        if (setting > 2) {
            bool pause;
            if (setting <= 9)
                pause = self->game->elapsedFrames / 30 % (17 - setting) == 0;
            else if (setting <= 13)
                pause = self->game->elapsedFrames / 30 % (setting / 2) == 0;
            else
                pause = self->game->elapsedFrames / 30 % (setting + 18) <= 1;
            if (pause)
                self->activeMotionFlags &= ~0x20U;
            else
                other->activeMotionFlags |= 0x20U;
            const std::uint8_t chance = 19 * (setting - 2);
            if (chance && chance >= runtime->random[1]) {
                command = static_cast<std::int8_t>(runtime->random[0]) >= 0 ? 2 : 3;
                goto select;
            }
        }
    } else {
        if (!(self->hitStateFlags & 0x200U))
            goto select;
        delay -= 2;
        if (AiAttackChance(self, runtime) != -1) {
            AiChooseCommand(self, authored + 81, 4, 0);
            goto select;
        }
    }
    command = static_cast<std::uint8_t>(static_cast<std::int32_t>(dc::sdk::RandomFloat() * authored[24]));
select:
    if (command != -1)
        AiSelectCommand(self, command, delay);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfUpdateMain(FightActorRuntimePrefix *self) {
    fightAiGame = self->game;
    FightRuntimePrefix *const game = fightAiGame;
    FightActorRuntimePrefix *const *const actor = &game->actors[SHENMUE_REACTION_VALUE(std::int8_t, self, 1)];
    FightAiSlot &slot = *static_cast<FightAiSlot *>(game->actorSlots[self->groupIndex]);
    FightActorRuntimePrefix *const other = *actor;
    const std::int8_t *const parameters = reinterpret_cast<const std::int8_t *>(self->motionParameters);
    SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x94c) = self->inputFlags;
    self->inputFlags = 0;
    if (SHENMUE_REACTION_VALUE(std::int8_t, self, 1) == -1 ||
        static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int8_t>(game->slotPoolModes[self->groupIndex]))) > 1)
        return;
    AiUpdateMetrics(self);
    AiUpdateHistory(self);
    AiUpdateRank(self);
    AiUpdatePhases(self);
    AiUpdateAssistWindow(self);
    if (self->hitStateFlags & 0xa4000U) {
        AiResetPhase(self);
        slot.parameter9a = 0;
    }
    if ((other->hitStateFlags & 0x14000U) == 0x10000U) {
        slot.queuedCommands[0] = slot.queuedCommands[1] = 127;
        slot.parameter9a = 0;
        slot.queuedCommandIndex = 0;
    }
    if ((slot.flags & 0x4000U) && !(self->hitStateFlags & 0x80000U))
        slot.flags &= ~0x4000U;
    slot.opponent = SHENMUE_REACTION_VALUE(std::int8_t, self, 1);
    if (other->animationTime < SHENMUE_REACTION_VALUE(float, other, 0x26c))
        slot.animationPhase = 0;
    static const std::int8_t defaultProgram[1] = {0}, trainingProgram[2] = {42, 0};
    const std::int8_t *program;
    if (!self->game->slotPoolModes[self->groupIndex])
        program = fightAiGame->type == 2 ? trainingProgram : defaultProgram;
    else
        program = parameters + 0x6f;
    std::int8_t selected;
    while ((selected = aiBehaviours[*program].predicate(self, &slot)) == -1)
        ++program;
    slot.reaction = selected;
    if (selected != slot.previousReaction) {
        slot.previousReaction = selected;
        slot.reactionTicks = 0;
    }
    if (slot.secondaryMotionTimer)
        --slot.secondaryMotionTimer;
    else if (self->secondaryMotionFlags & 0x10U)
        static_cast<FightAiSlot *>(self->slot)->secondaryMotionTimer = 60;
    if (other->hp > 0 && !(other->stateFlags & 0x80U) && !(other->moveFlags & 0x20U))
        (*aiBehaviours[selected].post)(self, &slot);
    else
        AiResetPhase(self);
    if ((slot.flags & 1) == 0)
        AiAdvanceCommand(self);
}

#else

// The native ordered reaction selector and all 58 update/post table slots.
// @verified - p
void vfUpdateMain(VFActor *self) {
    g_pFightMain = self->game;
    const auto index = self->actionIndex;
    auto &slot = *g_pFightMain->actorSlots[self->groupIndex];
    self->previousReactionInput = self->inputFlags;
    self->inputFlags = 0;
    if (index == -1 || g_pFightMain->slotPoolModes[self->groupIndex] > 1)
        return;
    auto &other = *g_pFightMain->actors[index];
    vfUpdateSlotRuntimeMetricsAndTimers(self);
    vfUpdateSlotReactionHistory(self);
    vfUpdateSlotRankParameters(self);
    vfUpdateScenarioHPPhaseConfig(self);
    vfUpdateCurrentAssistWindow(self);
    if (self->hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_Unknown20000 | VFHSF_AirborneOrDowned)) {
        ResetPhase(*self->slot);
        slot.parameterAe = 0;
    }
    if ((other.hitStateFlags & (VFHSF_SpecialGuardOrParry | VFHSF_GuardState)) == 0x10000) {
        slot.resetIndex = 0x7f7f;
        slot.parameterAe = 0;
        slot.parameter12a = 0;
    }
    if ((slot.cameraFlags & 0x4000) && !(self->hitStateFlags & VFHSF_AirborneOrDowned))
        slot.cameraFlags &= ~0x4000U;
    slot.opponentIndex = index;
    if (other.scale > other.animationTime)
        slot.animationPhase = 0;
    // Native g_vf_default_react_tbl is zero-initialized BSS with no writing xrefs;
    // its first entry selects the genuine constant-zero predicate.
    constexpr std::uint8_t defaultProgram[]{0};
    constexpr std::uint8_t sparringProgram[]{42, 0};
    const auto *program = g_pFightMain->type == 2 ? sparringProgram : defaultProgram;
    if (g_pFightMain->slotPoolModes[self->groupIndex] || (g_pFightMain->gameFlags & 0x400000))
        program = reinterpret_cast<const std::uint8_t *>(self->motionParameters[2]) + 111;
    std::int8_t selected;
    do {
        selected = aiBehaviours[*program++].update(self, &slot);
    } while (selected == -1);
    slot.reactionIndex = selected;
    if (slot.previousReactionIndex != selected) {
        slot.previousReactionIndex = selected;
        slot.reactionTicks = 0;
    }
    if (!(slot.secondaryMotionTimer >= 0)) {
        if (self->secondaryMotionFlags & VFSMF_Unknown10)
            self->slot->secondaryMotionTimer = 60;
    } else {
        slot.secondaryMotionTimer -= shenGetScaledDeltaTickTime(1.0f);
        if (slot.secondaryMotionTimer < 0)
            slot.secondaryMotionTimer = 0;
    }
    if (other.hp <= 0 || static_cast<std::int8_t>(other.stateFlags) < 0 || (other.moveFlags & VFMF_ForceDefeat))
        ResetPhase(*self->slot);
    else
        aiBehaviours[selected].post(self, &slot);
    if (!(slot.cameraFlags & 1))
        vfAdvanceAICommand(self);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#define SHENMUE_VM_STATE(self) SHENMUE_REACTION_VALUE(FightReactionMachine, self, 0x5d0)

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#define SHENMUE_VM_HISTORY(self) (&SHENMUE_REACTION_VALUE(FightReactionInput, self, 0x498))

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#define SHENMUE_VM_PC(self) SHENMUE_REACTION_VALUE(FightReactionPc, self, 0x5b4)

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

inline FightReactionPc VmTarget(FightActorRuntimePrefix *self, std::int32_t offset) {
    return SHENMUE_REACTION_VALUE(FightReactionPc, self, 0x28) + offset;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void VmResetSlots(FightActorRuntimePrefix *self) {
    SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x59a) = SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x599);
    SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x59b) = SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x599);
    SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x59c) = SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x599);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static std::int32_t IsActorPastMarkerFrame(FightActorRuntimePrefix *self, std::uint16_t motion) {
    if (self->hitStateFlags & 0x10U) {
        const std::byte *const marker = MOTN_FindAnimationExtraRecord(motion & 0xfffU, 3);
        if (marker) {
            const std::int16_t frame = SHENMUE_REACTION_EXTRA_WORD(marker, 6) - 1;
            if (self->animationTime < frame)
                return 1;
        }
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

std::int16_t VmCameraAngle(std::uint16_t code) {
    const std::uint32_t index = (code & 0xf0U) >> 4;
    return static_cast<std::int16_t>(vmCameraAngles[index] + 0x8000);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void FreeMarkedFightTaskData(void *data) {
    HLib::ClearTaskDataMark(data);
    HLib::FreeTaskData(data);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

std::int16_t VmWorldAngle(std::uint16_t code) {
    return static_cast<std::int16_t>(VmCameraAngle(code) + camCtrlGetNextRotY());
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

std::int16_t VmFacingDelta(FightActorRuntimePrefix *self, std::uint16_t code) {
    if (self->contactDistances[AiTarget(self)] < 3 && (self->groundContactFlags & 1) == 0 && !(self->actor->moveFlags & 0x20U))
        return static_cast<std::int16_t>(VmCameraAngle(code) + camCtrlGetNextRotY() - self->actorFacingAngles[SHENMUE_REACTION_VALUE(std::int8_t, self, 1)]);
    return static_cast<std::int16_t>(VmCameraAngle(code) + camCtrlGetNextRotY() - static_cast<std::int16_t>(self->facingYaw));
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
bool VmAngleWithin(std::int16_t a, std::int16_t b, std::int16_t distance) {
    if (!(std::fabs(static_cast<float>(static_cast<std::int16_t>(a - b))) > distance))
        return true;
    return false;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

struct VmMoveSlot {
    std::int8_t category;
    std::int8_t index;
};

void VmMoveIndex(std::uint8_t move, VmMoveSlot *slot) {
    static const std::uint8_t starts[5] = {0, 0, 32, 64, 96};
    slot->category = 0;
    slot->index = -1;
    for (std::uint8_t index = 1; index < 4; ++index)
        if (move < starts[index + 1]) {
            slot->category = index;
            slot->index = move - starts[index];
            if (slot->index >= moveDefinitionCounts[index])
                slot->index = -1;
            return;
        }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

std::int8_t VmMovePhase(std::uint8_t move) {
    VmMoveSlot slot;
    VmMoveIndex(move, &slot);
    const std::int8_t index = slot.index;
    if (index != -1) {
        const std::int32_t proficiency = GetMoveProficiency(slot.category, index);
        if (proficiency < 12)
            return 0;
        if (proficiency < 56)
            return 1;
        if (proficiency < 200)
            return 2;
        return 3;
    }
    return -1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void VmAddMoveUsage(std::uint8_t move) {
    VmMoveSlot slot;
    VmMoveIndex(move, &slot);
    if (slot.index != -1)
        AddMoveProficiencyUsage(slot.category, slot.index, 1);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void VmLearnMove(FightRuntimePrefix *game, std::uint8_t move) {
    VmMoveSlot slot;
    VmMoveIndex(move, &slot);
    const std::int8_t index = slot.index;
    const std::int8_t category = slot.category;
    if (index == -1 || GetMoveProficiency(category, index) >= 12 || (game->stateFlags & 0xc103bU))
        return;
    SetMoveLearningEntry(0, category, index, 12);
    SetNewEwzoTypeForMoveLearned(0, category, index);
    SetHelpIconMenuFlags(2);
    game->stateFlags |= 0x10U;
    SetLoadingBusy();
    SHENMUE_REACTION_VALUE(std::uint16_t, game, 0x46) = 300;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

float GetMoveProficiencyScale(std::uint8_t move) {
    VmMoveSlot slot;
    VmMoveIndex(move, &slot);
    float scale;
    if (slot.category)
        scale = static_cast<float>(static_cast<std::uint8_t>(GetMoveProficiency(slot.category, slot.index))) * 0.0020000001f + 1.0f;
    else
        scale = 1.0f;
    return scale;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

inline void VmClearDelay(FightActorRuntimePrefix *self) {
    SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5a8) = 0;
    SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x5a6) = 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void VmLatchTail(FightActorRuntimePrefix *self) {
    SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x59b) = SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x599);
    SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x59c) = SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x599);
}

template <> std::uint8_t VmCallback<18>(FightActorRuntimePrefix *self) {
    std::uint16_t frame = vfGetReactionFrame(self, SHENMUE_VM_PC(self)[3]);
    const std::uint16_t offset = static_cast<std::int8_t>(SHENMUE_VM_PC(self)[1]);
    SHENMUE_VM_PC(self) += 4;
    if (self->animationTime < static_cast<float>(frame + offset) - SHENMUE_REACTION_VALUE(float, self, 0x26c)) {
        if (!(self->activeMotionFlags & 4)) {
            if (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x2c2) && frame > SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x292))
                frame = SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x2c2);
            SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5a8) = frame + offset;
        } else
            SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5a8) = SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x294);
        SHENMUE_REACTION_VALUE(FightReactionPc, self, 0x5b0) = SHENMUE_VM_PC(self);
        VmResetSlots(self);
        return 1;
    }
    return 0;
}

template <> std::uint8_t VmCallback<57>(FightActorRuntimePrefix *self) {
    if (!SHENMUE_REACTION_VALUE(std::int8_t, self, 0x5a6)) {
        const std::uint16_t frame = vfGetReactionFrame(self, SHENMUE_VM_PC(self)[7]);
        const std::uint16_t offset = SHENMUE_VM_PC(self)[3] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[2]) << 8;
        if (self->animationTime > static_cast<float>(frame + offset)) {
            SHENMUE_REACTION_VALUE(std::int8_t, self, 0x5a6) = 0;
            SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5aa) = 0;
            SHENMUE_VM_PC(self) += 8;
            return 0;
        }
        SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5aa) =
            static_cast<std::int32_t>(SHENMUE_REACTION_VALUE(float, self, 0x270) * (static_cast<float>(frame + offset) - self->animationTime) + 0.5f);
        SHENMUE_REACTION_VALUE(std::int8_t, self, 0x5a6) = -1;
    }
    if (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5aa)) {
        --SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5aa);
        SHENMUE_REACTION_VALUE(FightReactionPc, self, 0x5b0) = SHENMUE_VM_PC(self);
        return 1;
    }
    SHENMUE_REACTION_VALUE(std::int8_t, self, 0x5a6) = 0;
    SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5aa) = 0;
    SHENMUE_VM_PC(self) += 8;
    return 0;
}

template <> std::uint8_t VmCallback<23>(FightActorRuntimePrefix *self) {
    SHENMUE_REACTION_VALUE(FightReactionPc, self, 0x5b0) = SHENMUE_VM_PC(self);
    SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5a8) = 0x7fff;
    SHENMUE_VM_PC(self) += 4;
    return 0;
}

template <> std::uint8_t VmCallback<24>(FightActorRuntimePrefix *self) {
    SHENMUE_REACTION_VALUE(FightReactionPc, self, 0x5b0) = 0;
    VmClearDelay(self);
    SHENMUE_VM_PC(self) += 4;
    return 0;
}

template <> std::uint8_t VmCallback<40>(FightActorRuntimePrefix *self) {
    FightReactionActorView *const view = reinterpret_cast<FightReactionActorView *>(self);
    const std::uint16_t value = view->history[view->machine.history].sector;
    if (value & SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x788))
        SHENMUE_VM_PC(self) = VmTarget(self, SHENMUE_VM_PC(self)[3] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[2]) << 8);
    else if (value & SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x78c))
        SHENMUE_VM_PC(self) = VmTarget(self, SHENMUE_VM_PC(self)[5] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[4]) << 8);
    else
        SHENMUE_VM_PC(self) = VmTarget(self, SHENMUE_VM_PC(self)[7] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[6]) << 8);
    return 0;
}

template <> std::uint8_t VmCallback<4>(FightActorRuntimePrefix *self) {
    SHENMUE_REACTION_VALUE(std::int32_t, self, 0x5d8) =
        static_cast<std::int32_t>(SHENMUE_REACTION_VALUE(float, self, 0x270) * static_cast<std::int8_t>(SHENMUE_VM_PC(self)[1]) + 0.5f);
    SHENMUE_VM_PC(self) += 4;
    return 0;
}

template <> std::uint8_t VmCallback<5>(FightActorRuntimePrefix *self) {
    FightRuntimePrefix *const game = self->game;
    SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x320) = static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[7] | SHENMUE_VM_PC(self)[6] << 8) |
        static_cast<std::uint32_t>(static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[5] | SHENMUE_VM_PC(self)[4] << 8)) << 16;
    const std::uint8_t move = SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x87f);
    std::int32_t player = 0;
    if (!AiStyle(self)) {
        player = 1;
        if (game->type != 5)
            VmAddMoveUsage(move);
        vfmaUpdateCollisionCounter(game, 0, move);
        VmLearnMove(game, move);
        if (move != 103)
            SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x880) = move;
        if (move <= 95)
            SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x881) = move;
    }
    const std::uint16_t motion = SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x320) & 0xfffU;
    if (SHENMUE_REACTION_VALUE(std::int8_t, self, 0x323) == 8) {
        const std::byte *const extra = MOTN_FindAnimationExtraRecord(motion, 15);
        if (extra && SHENMUE_REACTION_EXTRA_WORD(extra, 4))
            self->stateFlags |= 0x2000U;
        if (player == 1)
            vfmaUpdateCollisionCounter(game, 1, move);
        if (self->formationFlags & 0x400U) {
            if (move == SHENMUE_REACTION_VALUE(std::int16_t, self, 0x936))
                self->runtimeFlags |= 0x400U;
            else if (move == SHENMUE_REACTION_VALUE(std::int16_t, self, 0x938))
                self->runtimeFlags |= 0x800U;
        }
        for (std::uint8_t index = 0; index < 6; ++index)
            if (game->characters[index])
                game->actors[index]->reactionFlags &= 0x300U;
    } else if (MOTN_FindAnimationExtraRecord(motion, 4))
        self->stateFlags |= 0x1000U;
    SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5a8) = 0;
    SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x5a6) = 0;
    VmResetSlots(self);
    return 1;
}

template <> std::uint8_t VmCallback<69>(FightActorRuntimePrefix *self) {
    if (!SHENMUE_VM_PC(self)[1])
        SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x2fc) = static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[7] | SHENMUE_VM_PC(self)[6] << 8) |
            static_cast<std::uint32_t>(static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[5] | SHENMUE_VM_PC(self)[4] << 8)) << 16;
    else
        SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x300) = static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[7] | SHENMUE_VM_PC(self)[6] << 8) |
            static_cast<std::uint32_t>(static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[5] | SHENMUE_VM_PC(self)[4] << 8)) << 16;
    SHENMUE_VM_PC(self) += 8;
    return 0;
}

template <> std::uint8_t VmCallback<13>(FightActorRuntimePrefix *self) {
    SHENMUE_REACTION_VALUE(FightReactionPc, self, 0x5b0) = VmTarget(self, SHENMUE_VM_PC(self)[3] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[2]) << 8);
    SHENMUE_VM_PC(self) += 4;
    return 0;
}

template <> std::uint8_t VmCallback<6>(FightActorRuntimePrefix *self) {
    SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x320) = static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[7] | SHENMUE_VM_PC(self)[6] << 8) |
        static_cast<std::uint32_t>(static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[5] | SHENMUE_VM_PC(self)[4] << 8)) << 16;
    VmLatchTail(self);
    return 1;
}

template <> std::uint8_t VmCallback<55>(FightActorRuntimePrefix *self) {
    SHENMUE_REACTION_VALUE(std::int8_t, self, 0x778) = -1;
    SHENMUE_REACTION_VALUE(FightReactionPc, self, 0x5b0) = VmTarget(self, SHENMUE_VM_PC(self)[3] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[2]) << 8);
    return 7;
}

template <> std::uint8_t VmCallback<16>(FightActorRuntimePrefix *self) {
    return 6;
}

template <> std::uint8_t VmCallback<17>(FightActorRuntimePrefix *self) {
    SHENMUE_VM_PC(self) = VmTarget(self, SHENMUE_VM_PC(self)[3] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[2]) << 8);
    return 0;
}

template <> std::uint8_t VmCallback<14>(FightActorRuntimePrefix *self) {
    if (static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[3] | SHENMUE_VM_PC(self)[2] << 8) == SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x770))
        SHENMUE_VM_PC(self) = VmTarget(self, SHENMUE_VM_PC(self)[5] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[4]) << 8);
    else
        SHENMUE_VM_PC(self) += 8;
    return 0;
}

template <> std::uint8_t VmCallback<15>(FightActorRuntimePrefix *self) {
    FightReactionActorView *const view = reinterpret_cast<FightReactionActorView *>(self);
    const std::uint8_t depth = view->machine.depth;
    view->machine.contexts[depth].pressed = SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x496);
    view->machine.contexts[depth].mask = SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492);
    view->machine.contexts[depth].snapshot = view->machine.snapshot;
    view->machine.contexts[depth].history = view->machine.history;
    view->machine.contexts[depth].allowed = view->machine.allowed;
    view->machine.contexts[depth].remaining = static_cast<std::int32_t>(SHENMUE_REACTION_VALUE(float, self, 0x270) * view->machine.remaining + 0.5f);
    view->machine.contexts[depth].program =
        SHENMUE_REACTION_VALUE(FightReactionPc, self, 0x28) + (SHENMUE_VM_PC(self)[3] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[2]) << 8);
    view->machine.contexts[depth].scanRemaining = view->machine.scanRemaining;
    view->machine.contexts[depth].priorityOnly = 0;
    ++view->machine.depth;
    SHENMUE_VM_PC(self) += 4;
    return 0;
}

template <> std::uint8_t VmCallback<38>(FightActorRuntimePrefix *self) {
    FightReactionActorView *const view = reinterpret_cast<FightReactionActorView *>(self);
    const std::uint8_t depth = view->machine.depth;
    view->machine.contexts[depth].pressed = SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x496);
    view->machine.contexts[depth].mask = SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492);
    view->machine.contexts[depth].snapshot = view->machine.snapshot;
    view->machine.contexts[depth].history = view->machine.history;
    view->machine.contexts[depth].allowed = view->machine.allowed;
    view->machine.contexts[depth].remaining = static_cast<std::int32_t>(SHENMUE_REACTION_VALUE(float, self, 0x270) * view->machine.remaining + 0.5f);
    view->machine.contexts[depth].program = 0;
    view->machine.contexts[depth].scanRemaining = view->machine.scanRemaining;
    view->machine.contexts[depth].priorityOnly = 1;
    SHENMUE_VM_PC(self) += 4;
    return 0;
}

template <> std::uint8_t VmCallback<19>(FightActorRuntimePrefix *self) {
    if (!(dc::sdk::RandomFloat() > static_cast<std::int32_t>(static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[7] | SHENMUE_VM_PC(self)[6] << 8) |
                                       static_cast<std::uint32_t>(static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[5] | SHENMUE_VM_PC(self)[4] << 8)) << 16) *
                0.0000152587890625f)) {
        SHENMUE_VM_PC(self) += 8;
        return 0;
    }
    return 3;
}

template <> std::uint8_t VmCallback<20>(FightActorRuntimePrefix *self) {
    if (SHENMUE_REACTION_VALUE(std::int8_t, self->slot, 0x74) == static_cast<std::int8_t>(SHENMUE_VM_PC(self)[1]) &&
        !static_cast<FightAiSlot *>(self->slot)->hpPhaseTimer) {
        SHENMUE_VM_PC(self) += 4;
        return 0;
    }
    return 3;
}

template <> std::uint8_t VmCallback<21>(FightActorRuntimePrefix *self) {
    FightReactionPc const p = SHENMUE_VM_PC(self);
    const std::uint16_t selector = p[3] | static_cast<std::int8_t>(p[2]) << 8;
    const std::uint16_t count = static_cast<std::int32_t>(SHENMUE_REACTION_VALUE(float, self, 0x270) * static_cast<std::int8_t>(p[1]) + 0.5f);
    const FightReactionPc target = SHENMUE_REACTION_VALUE(FightReactionPc, self, 0x28) + (p[5] | static_cast<std::int8_t>(p[4]) << 8);
    if (selector) {
        SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5be) = selector;
        SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5c0) = count;
        SHENMUE_REACTION_VALUE(FightReactionPc, self, 0x5c4) = target;
    } else {
        SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5bc) = count;
        SHENMUE_REACTION_VALUE(FightReactionPc, self, 0x5b8) = target;
    }
    SHENMUE_VM_PC(self) += 8;
    return 0;
}

template <> std::uint8_t VmCallback<25>(FightActorRuntimePrefix *self) {
    SHENMUE_REACTION_VALUE(FightReactionPc, self, 0x5b8) = 0;
    SHENMUE_VM_PC(self) += 4;
    return 0;
}

template <> std::uint8_t VmCallback<31>(FightActorRuntimePrefix *self) {
    SHENMUE_VM_PC(self) += 4;
    return 0;
}

template <> std::uint8_t VmCallback<32>(FightActorRuntimePrefix *self) {
    if (!SHENMUE_VM_PC(self)[1])
        SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x5d2) = SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x59a);
    else
        SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x5d2) = SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x59b);
    SHENMUE_VM_PC(self) += 4;
    return 0;
}

template <> std::uint8_t VmCallback<33>(FightActorRuntimePrefix *self) {
    return 3;
}

template <> std::uint8_t VmCallback<39>(FightActorRuntimePrefix *self) {
    reinterpret_cast<FightReactionActorView *>(self)
        ->machine.contexts[static_cast<std::int8_t>(reinterpret_cast<FightReactionActorView *>(self)->machine.depth)]
        .program = SHENMUE_REACTION_VALUE(FightReactionPc, self, 0x28) + (SHENMUE_VM_PC(self)[3] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[2]) << 8);
    reinterpret_cast<FightReactionActorView *>(self)
        ->machine.contexts[static_cast<std::int8_t>(reinterpret_cast<FightReactionActorView *>(self)->machine.depth)]
        .priorityOnly = 1;
    ++reinterpret_cast<FightReactionActorView *>(self)->machine.depth;
    SHENMUE_VM_PC(self) += 4;
    return 0;
}

template <> std::uint8_t VmCallback<44>(FightActorRuntimePrefix *self) {
    FightReactionPc const p = SHENMUE_VM_PC(self);
    const std::int8_t index = p[2];
    std::uint8_t *counters = &SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x779);
    counters[index] = p[1];
    SHENMUE_VM_PC(self) += 4;
    return 0;
}

template <> std::uint8_t VmCallback<45>(FightActorRuntimePrefix *self) {
    ++reinterpret_cast<FightReactionActorView *>(self)->machine.counters[static_cast<std::int8_t>(SHENMUE_VM_PC(self)[1])];
    SHENMUE_VM_PC(self) += 4;
    return 0;
}

template <> std::uint8_t VmCallback<46>(FightActorRuntimePrefix *self) {
    --reinterpret_cast<FightReactionActorView *>(self)->machine.counters[static_cast<std::int8_t>(SHENMUE_VM_PC(self)[1])];
    SHENMUE_VM_PC(self) += 4;
    return 0;
}

template <> std::uint8_t VmCallback<47>(FightActorRuntimePrefix *self) {
    FightReactionPc const p = SHENMUE_VM_PC(self);
    if (static_cast<std::int8_t>(reinterpret_cast<FightReactionActorView *>(self)->machine.counters[static_cast<std::int8_t>(p[2])]) >
        static_cast<std::int8_t>(p[1])) {
        SHENMUE_VM_PC(self) += 4;
        return 0;
    }
    return 3;
}

template <> std::uint8_t VmCallback<48>(FightActorRuntimePrefix *self) {
    FightReactionPc const p = SHENMUE_VM_PC(self);
    if (static_cast<std::int8_t>(reinterpret_cast<FightReactionActorView *>(self)->machine.counters[static_cast<std::int8_t>(p[2])]) ==
        static_cast<std::int8_t>(p[1])) {
        SHENMUE_VM_PC(self) += 4;
        return 0;
    }
    return 3;
}

template <> std::uint8_t VmCallback<49>(FightActorRuntimePrefix *self) {
    FightReactionPc const p = SHENMUE_VM_PC(self);
    if (static_cast<std::int8_t>(reinterpret_cast<FightReactionActorView *>(self)->machine.counters[static_cast<std::int8_t>(p[2])]) <
        static_cast<std::int8_t>(p[1])) {
        SHENMUE_VM_PC(self) += 4;
        return 0;
    }
    return 3;
}

template <> std::uint8_t VmCallback<50>(FightActorRuntimePrefix *self) {
    if (IsActorPastMarkerFrame(self, static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[3] | SHENMUE_VM_PC(self)[2] << 8)) == 1) {
        SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x320) = static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[7] | SHENMUE_VM_PC(self)[6] << 8) |
            static_cast<std::uint32_t>(static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[5] | SHENMUE_VM_PC(self)[4] << 8)) << 16;
        SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5a8) = 0;
        SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x5a6) = 0;
        VmResetSlots(self);
        return 1;
    }
    return 3;
}

template <> std::uint8_t VmCallback<51>(FightActorRuntimePrefix *self) {
    SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x881) = 96;
    SHENMUE_VM_PC(self) += 4;
    return 0;
}

template <> std::uint8_t VmCallback<52>(FightActorRuntimePrefix *self) {
    VmResetAllSlots(self);
    SHENMUE_VM_PC(self) += 4;
    return 0;
}

template <> std::uint8_t VmCallback<53>(FightActorRuntimePrefix *self) {
    const std::int32_t same = self->game->type == static_cast<std::int8_t>(SHENMUE_VM_PC(self)[2]);
    if ((same ^ static_cast<std::int8_t>(SHENMUE_VM_PC(self)[1])) == 1) {
        SHENMUE_VM_PC(self) += 4;
        return 0;
    }
    return 3;
}

template <> std::uint8_t VmCallback<54>(FightActorRuntimePrefix *self) {
    if (vfCheckSkillFlag(shen_name(static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[7] | SHENMUE_VM_PC(self)[6] << 8) |
            static_cast<std::uint32_t>(static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[5] | SHENMUE_VM_PC(self)[4] << 8)) << 16))) {
        SHENMUE_VM_PC(self) += 8;
        return 0;
    }
    return 3;
}

template <> std::uint8_t VmCallback<56>(FightActorRuntimePrefix *self) {
    if ((SHENMUE_VM_PC(self)[3] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[2]) << 8) == self->currentAnimation) {
        SHENMUE_VM_PC(self) += 4;
        return 0;
    }
    return 3;
}

template <> std::uint8_t VmCallback<58>(FightActorRuntimePrefix *self) {
    if (self->reactionFlags &
        (static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[7] | SHENMUE_VM_PC(self)[6] << 8) |
            static_cast<std::uint32_t>(static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[5] | SHENMUE_VM_PC(self)[4] << 8)) << 16)) {
        SHENMUE_VM_PC(self) += 8;
        return 0;
    }
    return 3;
}

template <> std::uint8_t VmCallback<59>(FightActorRuntimePrefix *self) {
    if (!(self->reactionFlags &
            (static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[7] | SHENMUE_VM_PC(self)[6] << 8) |
                static_cast<std::uint32_t>(static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[5] | SHENMUE_VM_PC(self)[4] << 8)) << 16))) {
        SHENMUE_VM_PC(self) += 8;
        return 0;
    }
    return 3;
}

template <> std::uint8_t VmCallback<60>(FightActorRuntimePrefix *self) {
    return 3;
}

template <> std::uint8_t VmCallback<61>(FightActorRuntimePrefix *self) {
    if (!(self->actor->animationTime > static_cast<float>(SHENMUE_VM_PC(self)[3] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[2]) << 8) - 1)) {
        SHENMUE_VM_PC(self) += 4;
        return 0;
    }
    SHENMUE_VM_PC(self) += 4;
    return 3;
}

template <> std::uint8_t VmCallback<62>(FightActorRuntimePrefix *self) {
    if (self->hitFlags &
        (static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[7] | SHENMUE_VM_PC(self)[6] << 8) |
            static_cast<std::uint32_t>(static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[5] | SHENMUE_VM_PC(self)[4] << 8)) << 16)) {
        SHENMUE_VM_PC(self) += 8;
        return 0;
    }
    SHENMUE_VM_PC(self) += 8;
    return 3;
}

template <> std::uint8_t VmCallback<63>(FightActorRuntimePrefix *self) {
    SHENMUE_VM_PC(self) += 4;
    return 0;
}

template <> std::uint8_t VmCallback<65>(FightActorRuntimePrefix *self) {
    if ((SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x360) & 1) != 1) {
        SHENMUE_VM_PC(self) += 4;
        return 0;
    }
    return 3;
}

template <> std::uint8_t VmCallback<64>(FightActorRuntimePrefix *self) {
    if (SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x360) &
        (static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[7] | SHENMUE_VM_PC(self)[6] << 8) |
            static_cast<std::uint32_t>(static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[5] | SHENMUE_VM_PC(self)[4] << 8)) << 16)) {
        SHENMUE_VM_PC(self) += 8;
        return 0;
    }
    return 3;
}

template <> std::uint8_t VmCallback<66>(FightActorRuntimePrefix *self) {
    SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x360) |= 1;
    SHENMUE_VM_PC(self) += 4;
    return 0;
}

template <> std::uint8_t VmCallback<67>(FightActorRuntimePrefix *self) {
    FightActorRuntimePrefix *const second = self->secondActor;
    SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x320) = SHENMUE_REACTION_VALUE(std::uint16_t, second, 0x2f4) + 0x13000000U;
    if (self->game->playerSlot != -1 && second->groupIndex == self->game->playerSlot)
        vfmaUpdateCollisionCounter(self->game, 2, static_cast<std::uint16_t>(SHENMUE_REACTION_VALUE(std::int8_t, second, 0x87f)));
    SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5a8) = 0;
    SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x5a6) = 0;
    VmResetSlots(self);
    return 1;
}

template <> std::uint8_t VmCallback<68>(FightActorRuntimePrefix *self) {
    if (!(static_cast<std::uint8_t>(self->actor->retained2a9[1]) < self->hp)) {
        SHENMUE_VM_PC(self) += 4;
        return 0;
    }
    return 3;
}

template <> std::uint8_t VmCallback<7>(FightActorRuntimePrefix *self) {
    if ((static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[7] | SHENMUE_VM_PC(self)[6] << 8) |
            static_cast<std::uint32_t>(static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[5] | SHENMUE_VM_PC(self)[4] << 8)) << 16) &
        self->groundContactFlags) {
        SHENMUE_VM_PC(self) += 8;
        return 0;
    }
    return 3;
}

template <> std::uint8_t VmCallback<10>(FightActorRuntimePrefix *self) {
    if (!((static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[7] | SHENMUE_VM_PC(self)[6] << 8) |
              static_cast<std::uint32_t>(static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[5] | SHENMUE_VM_PC(self)[4] << 8)) << 16) &
            self->groundContactFlags)) {
        SHENMUE_VM_PC(self) += 8;
        return 0;
    }
    return 3;
}

template <> std::uint8_t VmCallback<8>(FightActorRuntimePrefix *self) {
    if ((static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[7] | SHENMUE_VM_PC(self)[6] << 8) |
            static_cast<std::uint32_t>(static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[5] | SHENMUE_VM_PC(self)[4] << 8)) << 16) &
        self->moveFlags) {
        SHENMUE_VM_PC(self) += 8;
        return 0;
    }
    return 3;
}

template <> std::uint8_t VmCallback<11>(FightActorRuntimePrefix *self) {
    if (!((static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[7] | SHENMUE_VM_PC(self)[6] << 8) |
              static_cast<std::uint32_t>(static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[5] | SHENMUE_VM_PC(self)[4] << 8)) << 16) &
            self->moveFlags)) {
        SHENMUE_VM_PC(self) += 8;
        return 0;
    }
    return 3;
}

template <> std::uint8_t VmCallback<9>(FightActorRuntimePrefix *self) {
    if ((static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[7] | SHENMUE_VM_PC(self)[6] << 8) |
            static_cast<std::uint32_t>(static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[5] | SHENMUE_VM_PC(self)[4] << 8)) << 16) &
        self->hitStateFlags) {
        SHENMUE_VM_PC(self) += 8;
        return 0;
    }
    return 3;
}

template <> std::uint8_t VmCallback<12>(FightActorRuntimePrefix *self) {
    if (!((static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[7] | SHENMUE_VM_PC(self)[6] << 8) |
              static_cast<std::uint32_t>(static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[5] | SHENMUE_VM_PC(self)[4] << 8)) << 16) &
            self->hitStateFlags)) {
        SHENMUE_VM_PC(self) += 8;
        return 0;
    }
    return 3;
}

template <> std::uint8_t VmCallback<22>(FightActorRuntimePrefix *self) {
    if (SHENMUE_REACTION_VALUE(std::int32_t, self, 0x770) == (SHENMUE_VM_PC(self)[3] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[2]) << 8)) {
        SHENMUE_VM_PC(self) += 4;
        return 0;
    }
    return 3;
}

template <> std::uint8_t VmCallback<26>(FightActorRuntimePrefix *self) {
    if (!(self->contactDistances[AiTarget(self)] <
            static_cast<std::int32_t>((static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[7] | SHENMUE_VM_PC(self)[6] << 8) |
                static_cast<std::uint32_t>(static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[5] | SHENMUE_VM_PC(self)[4] << 8)) << 16)) *
                0.0000152587890625f)) {
        SHENMUE_VM_PC(self) += 8;
        return 0;
    }
    return 3;
}

template <> std::uint8_t VmCallback<27>(FightActorRuntimePrefix *self) {
    if (self->contactDistances[AiTarget(self)] <
        static_cast<std::int32_t>((static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[7] | SHENMUE_VM_PC(self)[6] << 8) |
            static_cast<std::uint32_t>(static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[5] | SHENMUE_VM_PC(self)[4] << 8)) << 16)) *
            0.0000152587890625f) {
        SHENMUE_VM_PC(self) += 8;
        return 0;
    }
    return 3;
}

template <> std::uint8_t VmCallback<28>(FightActorRuntimePrefix *self) {
    if (SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x328) &
        (static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[7] | SHENMUE_VM_PC(self)[6] << 8) |
            static_cast<std::uint32_t>(static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[5] | SHENMUE_VM_PC(self)[4] << 8)) << 16)) {
        SHENMUE_VM_PC(self) += 8;
        return 0;
    }
    return 3;
}

template <> std::uint8_t VmCallback<29>(FightActorRuntimePrefix *self) {
    const std::uint16_t frame = vfGetReactionFrame(self, SHENMUE_VM_PC(self)[3]);
    const std::int8_t offset = SHENMUE_VM_PC(self)[1];
    if (!(self->animationTime > static_cast<float>(offset + frame) - SHENMUE_REACTION_VALUE(float, self, 0x26c))) {
        SHENMUE_VM_PC(self) += 4;
        return 0;
    }
    return 3;
}

template <> std::uint8_t VmCallback<34>(FightActorRuntimePrefix *self) {
    SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x784) |= (static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[7] | SHENMUE_VM_PC(self)[6] << 8) |
        static_cast<std::uint32_t>(static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[5] | SHENMUE_VM_PC(self)[4] << 8)) << 16);
    SHENMUE_VM_PC(self) += 8;
    return 0;
}

template <> std::uint8_t VmCallback<35>(FightActorRuntimePrefix *self) {
    if (SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x784) &
        (static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[7] | SHENMUE_VM_PC(self)[6] << 8) |
            static_cast<std::uint32_t>(static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[5] | SHENMUE_VM_PC(self)[4] << 8)) << 16)) {
        SHENMUE_VM_PC(self) += 8;
        return 0;
    }
    return 3;
}

template <> std::uint8_t VmCallback<36>(FightActorRuntimePrefix *self) {
    if (reinterpret_cast<FightAiActorView *>(self->actor)->retained28a == (SHENMUE_VM_PC(self)[5] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[4]) << 8)) {
        SHENMUE_VM_PC(self) += 8;
        return 0;
    }
    return 3;
}

template <> std::uint8_t VmCallback<37>(FightActorRuntimePrefix *self) {
    if (SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x3a4) >= SHENMUE_VM_PC(self)[1]) {
        SHENMUE_VM_PC(self) += 4;
        return 0;
    }
    return 3;
}

template <> std::uint8_t VmCallback<41>(FightActorRuntimePrefix *self) {
    if (!(self->actor->hitStateFlags &
            (static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[7] | SHENMUE_VM_PC(self)[6] << 8) |
                static_cast<std::uint32_t>(static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[5] | SHENMUE_VM_PC(self)[4] << 8)) << 16))) {
        SHENMUE_VM_PC(self) += 8;
        return 0;
    }
    return 3;
}

template <> std::uint8_t VmCallback<42>(FightActorRuntimePrefix *self) {
    if (self->actor->hitStateFlags &
        (static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[7] | SHENMUE_VM_PC(self)[6] << 8) |
            static_cast<std::uint32_t>(static_cast<std::uint16_t>(SHENMUE_VM_PC(self)[5] | SHENMUE_VM_PC(self)[4] << 8)) << 16)) {
        SHENMUE_VM_PC(self) += 8;
        return 0;
    }
    return 3;
}

template <> std::uint8_t VmCallback<30>(FightActorRuntimePrefix *self) {
    if (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x496) & (SHENMUE_VM_PC(self)[3] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[2]) << 8)) {
        SHENMUE_VM_PC(self) += 4;
        SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5d4) = SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492);
        return 0;
    }
    return 4;
}

template <> std::uint8_t VmCallback<70>(FightActorRuntimePrefix *self) {
    const std::uint8_t offset = 4 * (SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x39d) & SHENMUE_VM_PC(self)[1]);
    FightReactionPc const table =
        SHENMUE_REACTION_VALUE(FightReactionPc, self, 0x28) + (SHENMUE_VM_PC(self)[3] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[2]) << 8);
    FightReactionPc const entry = table + 4 * (offset % 64);
    SHENMUE_VM_PC(self) = SHENMUE_REACTION_VALUE(FightReactionPc, self, 0x28) +
        (static_cast<std::uint16_t>(entry[3] | entry[2] << 8) | static_cast<std::uint32_t>(static_cast<std::uint16_t>(entry[1] | entry[0] << 8)) << 16);
    return 0;
}

template <> std::uint8_t VmCallback<2>(FightActorRuntimePrefix *self) {
    const std::uint16_t word = SHENMUE_VM_PC(self)[3] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[2]) << 8;
    std::uint16_t required;
    if (word & 0x400)
        required = SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x788);
    else if (word & 0x800)
        required = SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x78c);
    else
        required = word;
    if (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492) & required) {
        SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5d4) = SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492);
        SHENMUE_VM_PC(self) += 4;
        return 2;
    }
    return (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492) != SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5d4)) + 4;
}

template <> std::uint8_t VmCallback<43>(FightActorRuntimePrefix *self) {
    const std::uint16_t word = SHENMUE_VM_PC(self)[3] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[2]) << 8;
    std::uint16_t required;
    if (word & 0x400)
        required = SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x788);
    else if (word & 0x800)
        required = SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x78c);
    else
        required = word;
    if (!(SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492) & required)) {
        SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5d4) = SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492);
        SHENMUE_VM_PC(self) += 4;
        return 2;
    }
    return (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492) != SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5d4)) + 4;
}

template <> std::uint8_t VmCallback<3>(FightActorRuntimePrefix *self) {
    std::uint8_t required = SHENMUE_VM_PC(self)[3];
    const std::uint16_t word = SHENMUE_VM_PC(self)[3] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[2]) << 8;
    if (word & 0x400)
        required |= SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x788);
    if (word & 0x800)
        required |= SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x78c);
    SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5ea) =
        static_cast<std::int32_t>(SHENMUE_REACTION_VALUE(float, self, 0x270) * static_cast<std::int8_t>(SHENMUE_VM_PC(self)[1]) + 0.5f);
    SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5ec) = required;
    SHENMUE_VM_PC(self) += 4;
    return SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5d4) == SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492) ||
            !(~required & SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492)) ?
        0 :
        3;
}

template <> std::uint8_t VmCallback<0>(FightActorRuntimePrefix *self) {
    if (!SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x496))
        return 4;
    std::uint8_t required = SHENMUE_VM_PC(self)[3];
    const std::uint16_t word = SHENMUE_VM_PC(self)[3] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[2]) << 8;
    if (word & 0x400)
        required |= SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x788);
    if (word & 0x800)
        required |= SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x78c);
    if (required != SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x496) || required != SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492))
        return 3;
    SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5d4) = required;
    SHENMUE_VM_PC(self) += 4;
    return 0;
}

template <> std::uint8_t VmCallback<1>(FightActorRuntimePrefix *self) {
    std::uint16_t required = static_cast<std::int8_t>(SHENMUE_VM_PC(self)[3]) & 0xff;
    const std::uint16_t word = SHENMUE_VM_PC(self)[3] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[2]) << 8;
    if (word & 0x400)
        required |= SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x788);
    if (word & 0x800)
        required |= SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x78c);
    if (required == SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492)) {
        SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5d4) = required;
        SHENMUE_VM_PC(self) += 4;
        return 2;
    }
    return (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492) != SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5d4)) + 4;
}

template <> std::uint8_t VmCallback<73>(FightActorRuntimePrefix *self) {
    if (!SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x496))
        return 4;
    if (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x496) & 0xfe0fU)
        return 3;
    std::uint16_t required = static_cast<std::int8_t>(SHENMUE_VM_PC(self)[3]) & 0xff;
    const std::uint16_t word = SHENMUE_VM_PC(self)[3] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[2]) << 8;
    if (word & 0x400)
        required |= SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x788);
    if (word & 0x800)
        required |= SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x78c);
    const std::int16_t direction = VmCameraAngle(required);
    const std::int16_t facing = VmFacingDelta(self, SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x496));
    const std::int16_t limit = SHENMUE_VM_PC(self)[5] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[4]) << 8;
    if (!VmAngleWithin(facing, direction, limit))
        return 3;
    if (!VmAngleWithin(VmFacingDelta(self, SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492)), direction, limit))
        return 3;
    SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5d4) = SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492);
    SHENMUE_VM_PC(self) += 8;
    return 0;
}

template <> std::uint8_t VmCallback<72>(FightActorRuntimePrefix *self) {
    std::uint16_t required = static_cast<std::int8_t>(SHENMUE_VM_PC(self)[3]) & 0xff;
    const std::uint16_t word = SHENMUE_VM_PC(self)[3] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[2]) << 8;
    if (word & 0x400)
        required |= SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x788);
    if (word & 0x800)
        required |= SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x78c);
    if (!SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492)) {
        if (!required) {
            SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5d4) = SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492);
            SHENMUE_VM_PC(self) += 8;
            return 2;
        }
        return 4;
    }
    if (!required)
        return 4;
    if (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492) & 0xfc0fU)
        return 3;
    const std::int16_t direction = VmCameraAngle(required);
    if (VmAngleWithin(VmFacingDelta(self, SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492)), direction,
            SHENMUE_VM_PC(self)[5] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[4]) << 8)) {
        SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5d4) = SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492);
        SHENMUE_VM_PC(self) += 8;
        return 2;
    }
    if (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5d4) == SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492))
        return 4;
    return 5;
}

template <> std::uint8_t VmCallback<71>(FightActorRuntimePrefix *self) {
    SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x87f) = SHENMUE_VM_PC(self)[1];
    SHENMUE_VM_PC(self) += 4;
    return 0;
}

template <> std::uint8_t VmCallback<74>(FightActorRuntimePrefix *self) {
    std::uint16_t required = static_cast<std::int8_t>(SHENMUE_VM_PC(self)[3]) & 0xff;
    const std::uint16_t word = SHENMUE_VM_PC(self)[3] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[2]) << 8;
    if (word & 0x400)
        required |= SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x788);
    if (word & 0x800)
        required |= SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x78c);
    if (!SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492)) {
        if (!required) {
            SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5d4) = SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492);
            SHENMUE_VM_PC(self) += 4;
            return 2;
        }
        return 4;
    }
    if (!required)
        return 4;
    if (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492) & 0xfc0fU)
        return 3;
    const std::int16_t direction = VmCameraAngle(required);
    if (VmAngleWithin(VmFacingDelta(self, SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492)), direction, 16384)) {
        SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5d4) = SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492);
        SHENMUE_VM_PC(self) += 4;
        return 2;
    }
    if (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5d4) == SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492))
        return 4;
    return 5;
}

template <> std::uint8_t VmCallback<75>(FightActorRuntimePrefix *self) {
    std::uint16_t required = static_cast<std::int8_t>(SHENMUE_VM_PC(self)[3]) & 0xff;
    const std::uint16_t word = SHENMUE_VM_PC(self)[3] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[2]) << 8;
    if (word & 0x400)
        required |= SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x788);
    if (word & 0x800)
        required |= SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x78c);
    const std::int16_t direction = VmCameraAngle(required);
    if (!VmAngleWithin(VmFacingDelta(self, SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492)), direction, 16384)) {
        SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5d4) = SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492);
        SHENMUE_VM_PC(self) += 4;
        return 2;
    }
    if (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5d4) == SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492))
        return 4;
    return 5;
}

template <> std::uint8_t VmCallback<76>(FightActorRuntimePrefix *self) {
    FightReactionActorView *const view = reinterpret_cast<FightReactionActorView *>(self);
    const std::int16_t angle = VmFacingDelta(self, view->history[view->machine.history].sector);
    const std::int16_t front = SHENMUE_VM_PC(self)[9] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[8]) << 8;
    const std::int16_t back = SHENMUE_VM_PC(self)[11] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[10]) << 8;
    if (VmAngleWithin(angle, 0, front))
        SHENMUE_VM_PC(self) =
            SHENMUE_REACTION_VALUE(FightReactionPc, self, 0x28) + (SHENMUE_VM_PC(self)[3] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[2]) << 8);
    else if (VmAngleWithin(angle, static_cast<std::int16_t>(0x8000), back))
        SHENMUE_VM_PC(self) =
            SHENMUE_REACTION_VALUE(FightReactionPc, self, 0x28) + (SHENMUE_VM_PC(self)[5] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[4]) << 8);
    else
        SHENMUE_VM_PC(self) =
            SHENMUE_REACTION_VALUE(FightReactionPc, self, 0x28) + (SHENMUE_VM_PC(self)[7] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[6]) << 8);
    return 0;
}

template <> std::uint8_t VmCallback<77>(FightActorRuntimePrefix *self) {
    const std::uint16_t word = SHENMUE_VM_PC(self)[3] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[2]) << 8;
    std::uint16_t required = word & 0xf0U;
    if (word & 0x400)
        required |= SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x788);
    if (word & 0x800)
        required |= SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x78c);
    SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5ea) =
        static_cast<std::int32_t>(SHENMUE_REACTION_VALUE(float, self, 0x270) * static_cast<std::int8_t>(SHENMUE_VM_PC(self)[1]) + 0.5f);
    SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5ec) = required;
    SHENMUE_VM_PC(self) += 8;
    const std::int16_t facing = VmFacingDelta(self, SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492));
    const std::int16_t limit = SHENMUE_VM_PC(self)[5] | static_cast<std::int8_t>(SHENMUE_VM_PC(self)[4]) << 8;
    return SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492) == SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5d4) ||
            ((SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492) & 0xf0U) &&
                !VmAngleWithin(facing, static_cast<std::int16_t>(0x8000), static_cast<std::int16_t>(0x8000 - limit))) ?
        0 :
        3;
}

template <> std::uint8_t VmCallback<78>(FightActorRuntimePrefix *self) {
    SHENMUE_VM_PC(self) += 4;
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::uint8_t VmRun(FightActorRuntimePrefix *self) {
    FightReactionMachine &state = SHENMUE_VM_STATE(self);
    const std::uint16_t savedSnapshot = state.snapshot;
    const std::uint16_t savedMask = SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492), savedPressed = SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x496);
    const FightReactionPc savedPc = SHENMUE_VM_PC(self);
    const std::uint32_t savedOverride = SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x48c);
    state.depth = 0;
    state.history = (SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x599) - 1) & 31;
    const std::uint16_t filter = static_cast<std::uint8_t>(self->queuedAction >> 24) == 6 ? 0x1f2U : 0xf0U;
    SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492) &= filter;
    SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x496) &= filter;
    state.scanRemaining = (state.history - SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x59f)) & 31;
    state.snapshot = SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492);
    state.remaining = state.allowed = 0;
    for (;;) {
        std::uint8_t response = reactionCallbacks[*SHENMUE_VM_PC(self)](self);
        if (response == 5) {
            const std::uint16_t mask = SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492);
            if (!state.allowed)
                response = mask == state.snapshot ? 3 : 4;
            else {
                --state.remaining;
                response = 3;
                if (static_cast<std::int16_t>(state.remaining) != -1) {
                    state.snapshot = mask;
                    response = 4;
                    if (mask) {
                        if (mask & static_cast<std::uint16_t>(~state.allowed))
                            response = 3;
                        else
                            state.allowed = mask;
                    }
                }
            }
        }
        if (response == 4) {
            const std::int32_t remaining = state.scanRemaining--;
            response = 3;
            if (remaining >= 0 && state.history != state.stop) {
                state.history = (state.history - 1) & 31;
                const FightReactionInput &history = SHENMUE_VM_HISTORY(self)[state.history];
                SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492) = history.mask & 0xf0U;
                const std::uint8_t previous = SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x59f);
                std::int32_t phase = state.history <= previous;
                if (previous < state.stop)
                    ++phase;
                if (state.history < state.stop)
                    --phase;
                if (phase < 1) {
                    const std::uint16_t pressed = history.pressed & state.filterCurrent;
                    SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x496) = pressed;
                    response = (history.pressed & state.filterPrevious) == pressed ? 0 : 3;
                } else {
                    SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x496) = history.pressed & 0xf0U;
                    response = 0;
                }
            }
        }
        if (response == 3) {
            state.depth = static_cast<std::uint8_t>(state.depth - 1);
            if (static_cast<std::int8_t>(state.depth) < 0)
                response = 6;
            else {
                const FightReactionContext &context = state.contexts[state.depth];
                if (!context.priorityOnly) {
                    SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x496) = context.pressed;
                    SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492) = context.mask;
                    state.snapshot = context.snapshot;
                    state.history = context.history;
                    state.allowed = context.allowed;
                    state.remaining = context.remaining;
                    state.scanRemaining = context.scanRemaining;
                }
                SHENMUE_VM_PC(self) = context.program;
                response = 0;
            }
        }
        if (response == 6) {
            state.chainFrames = 0;
            response = 7;
        }
        if (response == 7) {
            state.snapshot = savedSnapshot;
            SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492) = savedMask;
            SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x496) = savedPressed;
            SHENMUE_VM_PC(self) = savedPc;
            SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x48c) = savedOverride;
            return 1;
        }
        if (response == 1)
            return 127;
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::uint16_t VmPlayerInput(FightActorRuntimePrefix *self, std::uint8_t secondary) {
    std::uint16_t result = 0, direction = 0;
    std::uint8_t index = 0;
    FightRuntimePrefix *const game = self->game;
    if (!game->savedInput)
        return 0;
    std::uint16_t input = ctrlGetPadInputBuffer0(static_cast<PadControlEntry *>(game->savedInput))->held;
    const std::int32_t *table;
    if (!secondary)
        table = fightInputMap;
    else
        table = fightSecondaryInputMap;
    for (; table[index] != -1; ++index, input >>= 1)
        if (input & 1)
            result |= table[index];
    if (g_inputBuffer[0].leftStickX < 64)
        direction |= 0x80;
    if (g_inputBuffer[0].leftStickX > 192)
        direction |= 0x40;
    if (g_inputBuffer[0].leftStickY > 192)
        direction |= 0x10;
    if (g_inputBuffer[0].leftStickY < 64)
        direction |= 0x20;
    if (direction)
        result = direction | (result & 0xff0fU);
    return result;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

std::uint16_t VmMapPadInput(std::uint8_t pad) {
    std::uint16_t result = 0;
    std::uint8_t index = 0;
    std::uint16_t input = g_inputBuffer[pad].held;
    const std::int32_t *table = fightSecondaryInputMap;
    for (; table[index] != -1; ++index, input >>= 1)
        if (input & 1)
            result |= table[index];
    return result;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

std::int8_t VmSectorIndex(std::uint16_t code) {
    const std::uint32_t sector = code & 0xf0U;
    for (std::uint8_t index = 0; index < 8; ++index)
        if (aiCameraSectors[index] == sector)
            return index;
    return -1;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

std::uint16_t VmSectorCode(std::int8_t index) {
    return aiCameraSectors[index & 7];
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

std::uint8_t VmCameraSector(std::int16_t yaw) {
    return (static_cast<std::int16_t>(yaw - camCtrlGetNextRotY() + 0x1000) >> 13) & 7;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

std::uint16_t VmRotateSector(FightActorRuntimePrefix *actor, std::uint16_t code) {
    const std::uint16_t sector = code & 0xf0U;
    if (!sector)
        return code;
    const std::int32_t rotation = VmCameraSector(actor->facingYaw);
    std::uint32_t result = code & 0xff0fU;
    result |= VmSectorCode((VmSectorIndex(sector) + rotation) & 7);
    return result;
}

std::int32_t VmSectorWithinRange(std::int8_t first, std::int8_t second, std::int8_t range);

std::int32_t VmSectorsMatch(std::uint16_t first, std::uint16_t second, std::int8_t range) {
    const std::int8_t firstIndex = VmSectorIndex(first);
    const std::int8_t secondIndex = VmSectorIndex(second);
    if (firstIndex == -1 || secondIndex == -1) {
        if (firstIndex == secondIndex)
            return 1;
        return 0;
    }
    return VmSectorWithinRange(firstIndex, secondIndex, range);
}

std::int32_t VmSectorWithinRange(std::int8_t first, std::int8_t second, std::int8_t range) {
    if (range <= 0)
        return 0;
    std::int8_t distance = first - second;
    if (distance < 0)
        distance = -distance;
    if (distance > 4)
        distance = 8 - distance;
    const std::int32_t half = (range - 1) >> 1;
    if (distance <= half)
        return 1;
    if (range & 1)
        return 0;
    if (second != ((first + half + 1) & 7))
        return 0;
    return 1;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void VmQueueInput(FightActorRuntimePrefix *self, std::uint8_t clearDifference) {
    const std::uint8_t head = SHENMUE_VM_INPUT(self)->head;
    const std::uint8_t previous = (head - 1) & 31;
    const std::uint8_t next = (head + 1) & 31, following = (head + 2) & 31;
    const std::uint16_t previousMask = SHENMUE_VM_INPUT(self)->history[previous].mask;
    const std::uint16_t previousSector = SHENMUE_VM_INPUT(self)->history[previous].sector;
    const std::uint16_t input = SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492) = self->inputFlags;
    const std::uint16_t pressed = SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x494) = SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x496) =
        ~previousMask & input;
    SHENMUE_VM_INPUT(self)->history[head].mask = input;
    if (clearDifference == 1)
        SHENMUE_VM_INPUT(self)->history[head].pressed = 0;
    else
        SHENMUE_VM_INPUT(self)->history[head].pressed = pressed;
    std::uint16_t released;
    SHENMUE_VM_INPUT(self)->history[head].released = released = ~input & previousMask;
    SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5e4) = released;
    if (pressed)
        SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5d6) = pressed;
    std::uint16_t sector = input;
    if (!(sector & 0xc0U))
        sector |= previousSector & 0xc0U;
    if (!(sector & 0x30U))
        sector |= previousSector & 0x30U;
    SHENMUE_VM_INPUT(self)->history[head].sector = sector;
    SHENMUE_VM_INPUT(self)->head = next;
    if (next == SHENMUE_VM_INPUT(self)->tracked[0])
        SHENMUE_VM_INPUT(self)->tracked[0] = following;
    if (next == SHENMUE_VM_INPUT(self)->tracked[1])
        SHENMUE_VM_INPUT(self)->tracked[1] = following;
    if (next == SHENMUE_VM_INPUT(self)->tracked[2])
        SHENMUE_VM_INPUT(self)->tracked[2] = following;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static void ResetFightActorVmLimits(FightActorRuntimePrefix *self) {
    SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x788) = 32;
    SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x78c) = 16;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void VmUpdateOverride(FightActorRuntimePrefix *self) {
    if (self->moveFlags & 0x10U) {
        SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x48c) |= 1;
        vfResetReactionProgram(self);
    } else {
        SHENMUE_VM_PC(self) = SHENMUE_REACTION_VALUE(FightReactionPc, self, 0x5b0);
        if (self->hitStateFlags & 1)
            SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x48c) |= 1;
        else if (!SHENMUE_VM_PC(self) || (!SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5a8) && !SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x5a6)))
            vfResetReactionProgram(self);
    }
    if (SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x784) & 2)
        SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x784) |= 4;
    else
        SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x784) &= ~4U;
    if (SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x48c) & 1)
        SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x784) |= 2;
    else
        SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x784) &= ~2U;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void VmDeferredPriority(FightActorRuntimePrefix *self, std::uint8_t index) {
    std::uint16_t value = SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x496) & 0x30fU;
    if (!SHENMUE_VM_INPUT(self)->pending[index]) {
        if (value && value != 0x30f) {
            SHENMUE_VM_INPUT(self)->pending[index] = 1;
            SHENMUE_VM_INPUT(self)->inputs[index] = value;
            SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x496) &= 0xfcf0U;
        }
        SHENMUE_VM_INPUT(self)->latches[index] = SHENMUE_VM_INPUT(self)->latchSource;
        SHENMUE_VM_INPUT(self)->latchMirror = SHENMUE_VM_INPUT(self)->latchSource;
        return;
    }
    const std::uint16_t prior = SHENMUE_VM_INPUT(self)->inputs[index];
    if ((SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x48c) & 1) == 0 && prior == 0x100 && !(SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492) & prior) &&
        value == 1) {
        SHENMUE_VM_INPUT(self)->latches[index] = SHENMUE_VM_INPUT(self)->latchSource;
        SHENMUE_VM_INPUT(self)->latchMirror = SHENMUE_VM_INPUT(self)->latchSource;
        SHENMUE_VM_INPUT(self)->pending[index] = 0;
    } else {
        value |= prior;
        if (value != 0x30f)
            --SHENMUE_VM_INPUT(self)->pending[index];
        SHENMUE_VM_INPUT(self)->pending[index] = 0;
    }
    SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x496) |= value;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::uint8_t VmRunTracked(FightActorRuntimePrefix *self) {
    SHENMUE_REACTION_VALUE(FightReactionPc, self, 0x5b0) = 0;
    SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x5d2) = SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x59a);
    return VmRun(self);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::uint8_t VmRunRoot(FightActorRuntimePrefix *self) {
    SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5e6) = 0x1f0;
    SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5e8) = 0xffff;
    if (!SHENMUE_REACTION_VALUE(FightReactionPc, self, 0x5b8))
        return VmRunTracked(self);
    const FightReactionPc original = SHENMUE_VM_PC(self);
    SHENMUE_VM_PC(self) = SHENMUE_REACTION_VALUE(FightReactionPc, self, 0x5b8);
    std::uint8_t result = VmRunTracked(self);
    if (result != 127) {
        SHENMUE_VM_PC(self) = original;
        if (original)
            result = VmRunTracked(self);
    }
    return result;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::uint8_t VmRunMovement(FightActorRuntimePrefix *self) {
    SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x5d2) = SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x59b);
    SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5e6) = 0x1f0;
    SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5e8) = 0xff;
    return VmRun(self);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void VmRewindTail(FightActorRuntimePrefix *self) {
    std::int8_t difference = SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x598) - SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x599);
    if (difference < 0)
        difference += 32;
    const std::int8_t maximum = self->hitStateFlags & 0x10000000U ? 12 : 10;
    if (difference > maximum)
        difference = maximum;
    SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x599) = (SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x598) + 32 - difference) & 31;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
static void VmRewindTailFromQueued(FightActorRuntimePrefix *self) {
    std::int8_t difference = SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x598) - SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x59a);
    if (difference < 0)
        difference += 32;
    const std::int8_t maximum = self->hitStateFlags & 0x10000000U ? 12 : 10;
    if (difference > maximum)
        difference = maximum;
    SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x599) = (SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x598) + 32 - difference) & 31;
}

// @verified - d
static void VmResetAllSlots(FightActorRuntimePrefix *self) {
    SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x599) = SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x598);
    SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x59a) = SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x598);
    SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x59b) = SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x598);
    SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x59c) = SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x598);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::uint8_t VmResolvePriority(FightActorRuntimePrefix *self) {
    if (self->hitStateFlags & 0x1c000U)
        return 1;
    if (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x496) & 0x300U) {
        SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x784) &= ~1U;
        SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x599) = SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x598);
        if (!SHENMUE_REACTION_VALUE(FightReactionPc, self, 0x5b0)) {
            if (SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x48c) & 1U)
                goto finish;
            if (self->hitStateFlags & 0x10U)
                goto finish;
        } else if (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5a8)) {
            goto finish;
        }
        SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x770) = 4;
        if (VmRunRoot(self) == 127)
            return 127;
    finish:
        SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x59a) = SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x59d);
        SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x59b) = SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x59d);
        SHENMUE_REACTION_VALUE(FightReactionPc, self, 0x5b0) = 0;
        VmClearDelay(self);
        return 127;
    }
    return 1;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int32_t VmAdmitQueuedReaction(FightActorRuntimePrefix *self) {
    if (SHENMUE_REACTION_VALUE(std::int8_t, self, 0x32b) != 6 || SHENMUE_REACTION_VALUE(FightReactionPc, self, 0x5b0))
        return 1;
    if (SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x27c) < 3 && SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x496))
        ++SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x39e);
    return 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void FightReactionPostNullsub() {
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void VmEvaluateActor(FightActorRuntimePrefix *self) {
    std::uint16_t previousEdges;
    std::uint32_t group;
    std::int32_t playerInput;
    std::uint8_t clearDifference = 0;
    SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x44c) = 0xffff;
    SHENMUE_REACTION_VALUE(float, self, 0x450) = -1;
    SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x44a) = 0;
    self->stateFlags &= ~0x3000U;
    SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x9bc) = 0;
    FightRuntimePrefix *const game = self->game;
    if ((game->controlFlags & 0x20U) || static_cast<std::uint32_t>(game->state) <= 2 || (self->stateFlags & 0x80U))
        return;
    group = static_cast<std::uint8_t>(self->groupIndex);
    const std::uint8_t poolMode = game->slotPoolModes[group];
    playerInput = 1;
    switch (poolMode) {
    case 0: {
        if (game->controlFlags & 0x40000U)
            self->inputFlags = VmPlayerInput(self, 0);
        else
            self->inputFlags |= VmPlayerInput(self, 0);
        if ((self->inputFlags & 0xf0U) == 0xf0U)
            self->inputFlags = (self->inputFlags & 0xff0fU) | 0x4000U;
        if (!(game->stateFlags & 0xc103bU)) {
            if (!(self->inputFlags & 0xf7U) && !(g_inputBuffer[0].held & g_fightLoadWidth)) {
                if (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x93e) <= 65534)
                    ++SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x93e);
            } else
                SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x93e) = 0;
            if (static_cast<std::uint32_t>(SHENMUE_REACTION_VALUE(std::int16_t, self, 0x93e)) > 300)
                self->game->stateFlags |= 0x2000U;
            if (((self->formationFlags & 0x100U) || self->game->type == 4) && !(self->inputFlags & 15)) {
                if (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x940) <= 65534)
                    ++SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x940);
            } else
                SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x940) = 0;
        } else {
            SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x93e) = 0;
            SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x940) = 0;
        }
        bool closingLearned = false;
        if (game->stateFlags & 0x10U) {
            if (SHENMUE_REACTION_VALUE(std::uint16_t, game, 0x46))
                --SHENMUE_REACTION_VALUE(std::uint16_t, game, 0x46);
            if (((g_inputBuffer[0].pressedPriority & 0x200U) || (game->stateFlags & 0x80U) || !SHENMUE_REACTION_VALUE(std::uint16_t, game, 0x46)) &&
                !(game->stateFlags & 0x100U)) {
                SetNewEwzoTypeForMoveLearned(1, 0, 0);
                game->stateFlags &= 0xffffff6fU;
                clearDifference = 1;
                FinishLoading();
                ResetHelpIconsToItemPrompt();
                closingLearned = true;
                SHENMUE_REACTION_VALUE(std::uint16_t, game, 0x46) = 0;
            }
        }
        if (closingLearned) {
            game->stateFlags |= 0x100U;
            SHENMUE_REACTION_VALUE(std::uint16_t, game, 0x444) = 4;
            SetLoadingBusy();
        } else if (game->stateFlags & 0x100U) {
            if (SHENMUE_REACTION_VALUE(std::int16_t, game, 0x444) > 0)
                --SHENMUE_REACTION_VALUE(std::int16_t, game, 0x444);
            else {
                game->stateFlags &= ~0x100U;
                FinishLoading();
            }
        }
        break;
    }
    case 2:
        self->inputFlags = VmMapPadInput(1);
        break;
    case 3:
        self->inputFlags = VmMapPadInput(2);
        break;
    case 4:
        self->inputFlags = VmMapPadInput(3);
        break;
    case 1:
    case 5:
        playerInput = 0;
        break;
    case 6:
        self->inputFlags = 0;
        playerInput = 0;
        break;
    }
    if (game->stateFlags & 0xc103bU)
        self->inputFlags = 0;
    const std::int32_t sector = VmSectorIndex(self->inputFlags);
    SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x948) = (sector - (VmCameraSector(static_cast<std::int16_t>(self->temporaryFacingAngle)) - 8)) & 7;
    const std::uint16_t code = self->inputFlags;
    if ((code & 0xf0U) != (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x94c) & 0xf0U) || (self->secondaryMotionFlags & 0xd00U) != 0x100U ||
        (self->hitStateFlags & 0xfd010U)) {
        if (playerInput && !(game->controlFlags & 0x400000U))
            SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x883) = SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x948);
        else
            SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x883) = VmSectorIndex(self->inputFlags);
    }
    SHENMUE_REACTION_VALUE(std::int16_t, self, 0x9b0) = VmCameraAngle(code);
    SHENMUE_REACTION_VALUE(std::int16_t, self, 0x9b2) = VmWorldAngle(code);
    SHENMUE_REACTION_VALUE(std::int16_t, self, 0x9b4) = VmFacingDelta(self, code);
    if (!AiStyle(self)) {
        SHENMUE_REACTION_VALUE(std::uint16_t, game, 0x440) = self->facingYaw;
        if (code & 0xf0U) {
            if (static_cast<std::uint32_t>(SHENMUE_REACTION_VALUE(std::int16_t, game, 0x442)) <= 29)
                ++SHENMUE_REACTION_VALUE(std::int16_t, game, 0x442);
            else
                SHENMUE_REACTION_VALUE(std::int16_t, game, 0x440) = SHENMUE_REACTION_VALUE(std::int16_t, self, 0x9b2);
        } else if (SHENMUE_REACTION_VALUE(std::uint16_t, game, 0x442))
            SHENMUE_REACTION_VALUE(std::uint16_t, game, 0x442) = 0;
    }
    if (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x9d6))
        --SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x9d6);
    if (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x9d4) && !(self->inputFlags & 0x4000U))
        --SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x9d4);
    SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x48c) = 0;
    VmQueueInput(self, clearDifference);
    VmUpdateOverride(self);
    ResetFightActorVmLimits(self);
    VmDeferredPriority(self, 0);
    std::uint32_t conditions = 0;
    if (!(self->hitStateFlags & 0x800U))
        conditions = (self->groundContactFlags & 1) == 0 ? 3 : 1;
    else if (self->groundContactFlags & 1)
        conditions |= 2;
    if (!(SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492) & 0x20U))
        conditions |= 4;
    if (!(self->moveFlags & 2))
        conditions |= 8;
    SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x39d) = conditions;
    if (self->moveFlags & 0x20U) {
        if (self->moveFlags & 0x10U)
            return;
        if (self->hitStateFlags & 1)
            return;
        SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x320) = 0x01000000U;
        return;
    }
    if (!(self->animationTime < static_cast<float>(SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x294)) - SHENMUE_REACTION_VALUE(float, self, 0x26c)) &&
        SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x300)) {
        SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x320) = SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x300);
        SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x2fc) = 0;
        SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x300) = 0;
        return;
    }
    if (!(self->animationTime < static_cast<float>(SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x292)) - SHENMUE_REACTION_VALUE(float, self, 0x26c)) &&
        SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x2fc)) {
        SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x320) = SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x2fc);
        SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x2fc) = 0;
        SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x300) = 0;
        return;
    }
    if (!VmAdmitQueuedReaction(self) || VmResolvePriority(self) == 127)
        return;
    if (SHENMUE_REACTION_VALUE(FightReactionPc, self, 0x5b0)) {
        if (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5a8)) {
            if (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5a8) == 0x7fff) {
                if (VmRunTracked(self) == 127)
                    return;
                VmResetSlots(self);
                return;
            }
            if (self->animationTime < static_cast<float>(SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5a8)) - SHENMUE_REACTION_VALUE(float, self, 0x26c))
                return;
            SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x5a8) = 0;
            if (VmRunTracked(self) == 127)
                return;
            VmResetSlots(self);
            return;
        }
        if (SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x5a6)) {
            if (VmRunTracked(self) == 127)
                return;
            VmResetSlots(self);
            return;
        }
    }
    if (SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x48c) & 1) {
        if (!SHENMUE_REACTION_VALUE(FightReactionPc, self, 0x5b8) && !SHENMUE_VM_PC(self)) {
            VmRewindTail(self);
            return;
        }
    } else if (SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x784) & 4)
        VmRewindTailFromQueued(self);
    SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x5a1) = 0;
    SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x9bc) = 1;
    previousEdges = reinterpret_cast<FightReactionActorView *>(self)->machine.pressed;
    while (SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x599) != SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x598)) {
        const std::uint8_t tail = SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x599);
        const std::uint16_t mask = reinterpret_cast<FightReactionActorView *>(self)->history[tail].mask;
        reinterpret_cast<FightReactionActorView *>(self)->inputMask = mask;
        reinterpret_cast<FightReactionActorView *>(self)->inputPressed = reinterpret_cast<FightReactionActorView *>(self)->history[tail].pressed;
        reinterpret_cast<FightReactionActorView *>(self)->machine.released = reinterpret_cast<FightReactionActorView *>(self)->history[tail].released;
        VmDeferredPriority(self, 1);
        SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x599) = (SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x599) + 1) & 31;
        reinterpret_cast<FightReactionActorView *>(self)->machine.condition = 0;
        if (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x496) & 1)
            reinterpret_cast<FightReactionActorView *>(self)->machine.condition = 1;
        if (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x496) & 2)
            reinterpret_cast<FightReactionActorView *>(self)->machine.condition |= 2;
        if (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x496) & 8)
            reinterpret_cast<FightReactionActorView *>(self)->machine.condition |= 8;
        if (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x496) & 4)
            reinterpret_cast<FightReactionActorView *>(self)->machine.condition |= 0x10U;
        if (reinterpret_cast<FightReactionActorView *>(self)->machine.condition) {
            if (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x496) & 0x100U)
                reinterpret_cast<FightReactionActorView *>(self)->machine.condition |= 4;
            else
                reinterpret_cast<FightReactionActorView *>(self)->machine.condition &= ~4U;
        }
        if (!(self->hitStateFlags & 0x1000U)) {
            if (reinterpret_cast<FightReactionActorView *>(self)->machine.condition) {
                if (self->moveFlags & 0x10U)
                    reinterpret_cast<FightReactionActorView *>(self)->machine.condition |= 0x500U;
                if (VmRunRoot(self) == 127 || reinterpret_cast<FightReactionActorView *>(self)->machine.chainFrames > 0)
                    return;
            }
            if (((self->hitStateFlags & 0x40000U) || (SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x48c) & 1) == 0) &&
                ((reinterpret_cast<FightReactionActorView *>(self)->machine.releasedFlags | SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x496)) & 0xf0U) &&
                !(previousEdges & 7)) {
                reinterpret_cast<FightReactionActorView *>(self)->machine.condition = 32;
                if (VmRunMovement(self) == 127)
                    return;
            }
        }
    }
    if (SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x5a1))
        SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x599) = SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x59e);
    SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x9bc) = 2;
    if ((SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x48c) & 1) || (self->hitStateFlags & 0x1000U))
        return;
    if (self->activeMotionFlags & 0x800000U) {
        if (self->inputFlags & 0x40U)
            self->facingAngle -= 1024;
        else if (self->inputFlags & 0x80U)
            self->facingAngle += 1024;
        return;
    }
    if (AiStyle(self) == 16 || AiStyle(self) == 46) {
        if (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x496) & 0xf0U)
            SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x320) = 0x23000000U;
        return;
    }
    if (self->inputFlags & 0x1000U) {
        if (self->inputFlags & 0xc0U) {
            SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x320) = 0x1e000000U;
            return;
        }
    } else {
        if (self->inputFlags & 0x2000U) {
            SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x320) = 0x1d000000U;
            return;
        }
        SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x94e) = 0;
        SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x94f) = 0;
        SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x95c) = 0;
        if ((self->activeMotionFlags & 0x8000000U) && (self->stateFlags & 1) && !SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x9d6)) {
            const float *const spawn = &game->spawnPoints[self->nearestSpawnPoint].x;
            const std::int32_t angle = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(spawn[0] - self->position.x, spawn[1] - self->position.z)) -
                static_cast<std::int16_t>(self->temporaryFacingAngle);
            if (8192.0f > std::fabs(static_cast<float>(angle))) {
                SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x320) = 0x17000000U;
                return;
            }
        }
        if (!(self->activeMotionFlags & 0x8000000U) || !(self->motionFlags & 4)) {
            if ((self->inputFlags & 0x4000U) && !(game->stateFlags & 0xc103bU)) {
                if (playerInput) {
                    if (!SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x9d6)) {
                        if (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x9d4) <= 64999 && !(self->hitStateFlags & 0x400U))
                            ++SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x9d4);
                        if (static_cast<std::uint32_t>(SHENMUE_REACTION_VALUE(std::int16_t, self, 0x9d4)) > 300) {
                            SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x94e) = 2;
                            SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x9d4) = 0;
                            SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x9d6) = 60;
                        } else
                            SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x94e) = 3;
                        SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x320) = 0x1c000000U;
                        return;
                    }
                } else {
                    SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x94e) = 3;
                    SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x320) = 0x1c000000U;
                    return;
                }
            }
            if ((self->activeMotionFlags & 0x8000000U) && !(self->motionFlags & 0x404U)) {
                SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x94e) = 2;
                SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x320) = 0x1c000000U;
                return;
            }
            float range;
            if ((self->hitStateFlags & 0x400U) && (self->activeMotionFlags & 0x8000000U))
                range = game->contactRange - 0.3f;
            else
                range = game->contactRange;
            bool contact = false;
            for (std::uint8_t index = 0; index < 6; ++index)
                if (index != group && game->characters[index] && game->slotTeams[index] != game->slotTeams[group] &&
                    (range > self->contactDistances[index] || camCtrlGetInterpMode() == 3)) {
                    contact = true;
                    break;
                }
            if (!contact && (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492) & 0xf0U) && !(self->secondaryMotionFlags & 0x100U)) {
                SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x94e) = 1;
                SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x320) = 0x1c000000U;
                return;
            }
            if (SHENMUE_REACTION_VALUE(std::uint16_t, self, 0x492) & 0xf0U) {
                std::int32_t angle;
                if (playerInput && !(game->controlFlags & 0x400000U))
                    angle = SHENMUE_REACTION_VALUE(std::int16_t, self, 0x9b4);
                else
                    angle = SHENMUE_REACTION_VALUE(std::int16_t, self, 0x9b0);
                SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x320) = 0x1b000000U;
                if (VmAngleWithin(static_cast<std::int16_t>(angle), static_cast<std::int16_t>(0x8000), 8192) && (self->groundContactFlags & 1) &&
                    self->contactDistances[AiTarget(self)] < game->contactRange) {
                    SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x320) = 0x02000000U;
                    VmLatchTail(self);
                }
                return;
            }
        }
        SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x949) = SHENMUE_REACTION_VALUE(std::uint8_t, self, 0x948);
    }
    SHENMUE_REACTION_VALUE(std::uint32_t, self, 0x320) = 0x01000000U;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void vfmaUpdateNextReaction(FightActorRuntimePrefix *actor) {
    VmEvaluateActor(actor);
    FightReactionPostNullsub();
}

#else

// Real player/AI input history, reaction VM admission and fallback actions.
// @verified - p
void vfmaUpdateNextReaction(VFActor *actor) {
    auto &s = *actor;
    auto &game = *s.game;
    s.stateFlags &= ~(VFSF_Unknown1000 | VFSF_Unknown2000);
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
            s.inputFlags = PlayerFightInput(s);
        else
            s.inputFlags |= PlayerFightInput(s);
        if ((s.inputFlags & 0xf0) == 0xf0)
            s.inputFlags = (s.inputFlags & 0xff0f) | 0x4000;
        if (game.mode & 0xc103b) {
            std::memset(&s.idleInputFrames, 0, sizeof(s.idleInputFrames) + sizeof(s.noAttackInputFrames));
        } else {
            float idle = s.idleInputFrames;
            // shenStartFGST initializes this word to 0x40.
            if (static_cast<std::uint8_t>(s.inputFlags) || (g_inputBuffer[0].held & 0x40))
                idle = 0;
            else if (static_cast<std::int32_t>(idle) < 65535)
                idle += shenGetScaledDeltaTickTime(1.0f);
            s.idleInputFrames = idle;
            if (static_cast<std::int32_t>(idle) > 450)
                game.mode |= 0x2000;
            float noAttack = s.noAttackInputFrames;
            if (((s.formationFlags & 0x100) || game.type == 4) && !(s.inputFlags & 0xf)) {
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
        if ((s.pairedActor->moveFlags & 0x20) && (s.inputFlags & 0xf8)) {
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
        s.inputFlags = MapFightInput(g_inputBuffer[game.slotPoolModes[s.groupIndex] - 1].held, true);
        break;
    case 6:
        s.inputFlags = 0;
        [[fallthrough]];
    case 1:
    case 5:
        playerInput = false;
        break;
    default:
        break;
    }
    if (game.mode & 0xc103b)
        s.inputFlags = 0;
    const std::uint16_t code = s.inputFlags;
    const auto sector = CameraSectorIndex(code);
    const auto rotation = (static_cast<std::int16_t>(s.temporaryFacingAngle - camCtrlGetNextRotY() + 4096) >> 13) & 7;
    const auto relative = static_cast<std::uint8_t>((sector - rotation) & 7);
    s.relativeInputSectors[0] = relative;
    if ((code & 0xf0) != (s.previousReactionInput & 0xf0) ||
        (s.secondaryMotionFlags & (VFSMF_AlternateFootTarget | VFSMF_Unknown400 | VFSMF_Unknown800)) != 0x100 ||
        (s.hitStateFlags &
            (VFHSF_FallLandingRequired | VFHSF_LightReaction | VFHSF_SpecialGuardOrParry | VFHSF_SpecialThrow | VFHSF_GuardState | VFHSF_Unknown20000 |
                VFHSF_MidAirRecovery | VFHSF_AirborneOrDowned)))
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
    auto conditions =
        static_cast<std::uint8_t>(s.hitStateFlags & VFHSF_ReversedFacing ? (s.groundContactFlags & 1 ? 2 : 0) : (~(2 * s.groundContactFlags) & 2) | 1);
    if (!(s.inputMask & 0x20))
        conditions |= 4;
    if (!(s.moveFlags & VFMF_Unknown02))
        conditions |= 8;
    s.footFacingBytes[1] = conditions;
    if (s.moveFlags & VFMF_ForceDefeat) {
        if (!(s.moveFlags & VFMF_HitReactionLock) && !(s.hitStateFlags & VFHSF_ReactionActive))
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
        if (s.hitPhase < 3 && s.inputPressed)
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
        s.inputMask = s.inputHistory[tail].mask;
        s.inputPressed = s.inputHistory[tail].pressed;
        s.reactions.released = static_cast<std::uint32_t>(s.inputHistory[tail].released);
        vfUpdateDeferredReactionPriority(&s, 1);
        s.reactionCursors.endpoint[1] = static_cast<std::uint8_t>((tail + 1) & 31);
        std::uint32_t flags = (s.inputPressed & 1) | (s.inputPressed & 2) | (s.inputPressed & 8);
        if (s.inputPressed & 4)
            flags |= 0x10;
        if (flags)
            flags = s.inputPressed & 0x100 ? flags | 4 : flags & ~4U;
        s.reactions.conditionFlags = flags;
        if (s.hitStateFlags & VFHSF_LightReaction)
            continue;
        if (flags) {
            if (s.moveFlags & VFMF_HitReactionLock)
                s.reactions.conditionFlags = flags | 0x500;
            if (vfHandleSavedCharRootPosReaction(&s) == 0x7f || std::bit_cast<std::int8_t>(s.reactions.chainFrames) > 0)
                return;
        }
        if (((s.hitStateFlags & VFHSF_MidAirRecovery) || !(s.reactionOverrideActive & 1)) &&
            ((s.inputPressed | static_cast<std::uint8_t>(s.reactions.releasedFlags)) & 0xf0) && !(previousEdges & 7)) {
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
    if ((s.reactionOverrideActive & 1) || (s.hitStateFlags & VFHSF_LightReaction))
        return;
    if (s.activeMotionFlags & VFSMF_Unknown800000) {
        if (code & 0x40)
            s.facingAngle -= 1024;
        else if (code & 0x80)
            s.facingAngle += 1024;
        return;
    }
    if (s.fightStyle == 16 || s.fightStyle == 46) {
        if (s.inputPressed & 0xf0)
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
    const bool followup = (s.activeMotionFlags & VFSMF_Unknown8000000) != 0;
    if (followup && (s.stateFlags & VFSF_Unknown1) && s.runningCooldownFrames <= epsilon) {
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
    if (!followup || !(s.motionFlags & VFMOF_Unknown4)) {
        if ((code & 0x4000) && !(game.mode & 0xc103b) && (!playerInput || s.runningCooldownFrames <= epsilon)) {
            if (playerInput) {
                float timer = s.runningInputFrames;
                if (static_cast<std::int32_t>(timer) < 65000 && !(s.hitStateFlags & VFHSF_CameraAssistLock))
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
        if (followup && !(s.motionFlags & (VFMOF_Unknown4 | VFMOF_ContactCategoryA))) {
            s.reactionMovement.state.mode = std::uint8_t{2};
            s.action = 0x1c000000;
            return;
        }
        const auto distance = game.contactRange - ((s.hitStateFlags & VFHSF_CameraAssistLock) && followup ? 0.30000001f : 0);
        std::uint8_t contact = 0;
        while (contact < 6 &&
            (contact == s.groupIndex || !game.characters[contact] || game.slotTeams[contact] == game.slotTeams[s.groupIndex] ||
                (distance <= s.contactDistances[contact] && camCtrlGetInterpMode() != CameraMotionMode::FirstPerson)))
            ++contact;
        if (contact == 6 && (s.inputMask & 0xf0) && !(s.secondaryMotionFlags & VFSMF_AlternateFootTarget)) {
            s.reactionMovement.state.mode = std::uint8_t{1};
            s.action = 0x1c000000;
            return;
        }
        if (s.inputMask & 0xf0) {
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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::uint16_t MotionMarkerWord(std::span<const std::byte> marker, std::size_t offset) {
    std::uint16_t value;
    std::memcpy(&value, marker.data() + offset, sizeof(value));
    return value;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void BindPairedActor(VFMA_t &game, std::uint8_t slot, std::uint8_t partner) {
    auto &self = *game.actors[slot];
    const auto previous = self.pairedCharacter;
    self.pairedSlot = static_cast<std::int8_t>(partner);
    self.pairedActor = game.actors[partner];
    self.pairedCharacter = game.characters[partner];
    if (previous != self.pairedCharacter)
        self.reactionFlags = 0;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void SetAuthoredSpawnPosition(VFActor &self) {
    const std::int32_t offset = 0x2f4 + 12 * self.nearestSpawnPoint;
    const auto *game = reinterpret_cast<const std::byte *>(self.game);
    std::memcpy(&self.position.x, game + offset, sizeof(float));
    std::memcpy(&self.position.z, game + offset + 4, sizeof(float));
    self.position.y = flddGetIntersectionFor(self.character, self.position);
}

} // namespace

#endif

} // namespace s2
} // namespace shenmue
