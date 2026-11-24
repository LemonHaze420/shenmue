#include "s2/enhancements/tai_chi.hpp"

#if SHENMUE_IS_PC && SHENMUE_IS_S2 && SHENMUE_ENHANCEMENT_S2_TAI_CHI
#include "enhancements/misc_mods.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/input.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scenario.hpp"
#include "s2/save.hpp"

#include <cmath>

namespace shenmue::s2 {
namespace {
constexpr shen_name practiceArea{SHEN_NAME(WT00)};
constexpr Vector3 practiceCentre{244.774826f, 115.178f, 348.542145f};
constexpr float practiceRadius = 6.0f;
constexpr std::uint16_t practiceMotions[]{0xa805, 0xa806};

struct TaiChiState {
    Character *player{};
    Vector3 position{};
    std::uint32_t rootMode{};
    unsigned motion{};
    std::uint32_t random{1};
    float trainingTime{};
    float trainingInterval{};
    bool qHeld{};
} practice;

std::uint32_t PracticeRandom() {
    practice.random = practice.random * 1664525U + 1013904223U;
    return practice.random;
}

void TrainPracticeMove() {
    const auto banks = S2Save_GetMoveSkillBanks();
    unsigned count = 0;
    for (const auto &bank : banks)
        for (std::size_t move = 0; move < bank.count && move < bank.proficiency.size(); ++move)
            if (bank.proficiency[move] > 0 && bank.proficiency[move] < 200)
                ++count;
    if (!count)
        return;
    unsigned selected = PracticeRandom() % count;
    for (std::size_t category = 0; category < banks.size(); ++category) {
        const auto &bank = banks[category];
        for (std::size_t move = 0; move < bank.count && move < bank.proficiency.size(); ++move) {
            const auto proficiency = bank.proficiency[move];
            if (proficiency > 0 && proficiency < 200 && selected-- == 0) {
                S2Save_SetMoveProficiency(static_cast<std::uint8_t>(category + 1), move, proficiency + 1);
                return;
            }
        }
    }
}

bool InPracticeArea(const Vector3 &position) {
    const float x = position.x - practiceCentre.x;
    const float z = position.z - practiceCentre.z;
    return g_currentMap == practiceArea && std::fabs(position.y - practiceCentre.y) < 2.0f &&
        x * x + z * z <= practiceRadius * practiceRadius;
}

bool MovementRequested() {
    return IsWindowKeyPressed('W') || IsWindowKeyPressed('A') || IsWindowKeyPressed('S') || IsWindowKeyPressed('D') ||
        std::abs(static_cast<std::int32_t>(GetCurrentControlInput(INPUT_LEFT_X)) - 128) > 32 ||
        std::abs(static_cast<std::int32_t>(GetCurrentControlInput(INPUT_LEFT_Y)) - 128) > 32;
}

void EndPractice(MotionWork *motion) {
    practice.player = nullptr;
    mtwkSetRequestedRootMode(motion, practice.rootMode);
    SetPlayerControlEnabled(1);
}
}

void ResetTaiChi() {
    practice = {};
}

bool IsPractisingTaiChi() {
    return practice.player && practice.player == g_playerRuntime.character && g_currentMap == practiceArea;
}

bool TaiChiHintActive() {
    Character *const player = g_playerRuntime.character;
    if (!mods::Enabled(mods::Id::TaiChi) || !player || !g_playerRuntime.model ||
        shenIsGamePaused() || !InPracticeArea(GetCharacterWorldPosition(player)) || IsScenarioBusy() || authGetLoaded())
        return false;
#if SHENMUE_ENHANCEMENT_MISC_MODS_CAMERA
    if (mods::FreeCameraActive())
        return false;
#endif
    return practice.player == player || ((g_playerRuntime.flags & 2U) && ryoNotBusy() &&
        FindLoadedMotionSequence(practiceMotions[0]) && FindLoadedMotionSequence(practiceMotions[1]));
}

void UpdateTaiChi(Character *player) {
    const bool qHeld = IsWindowKeyPressed('Q');
    const bool pressed = qHeld && !practice.qHeld;
    practice.qHeld = qHeld;
    if (practice.player && (practice.player != player || g_currentMap != practiceArea))
        practice.player = nullptr;
    if (!player || player != g_playerRuntime.character || !g_playerRuntime.model)
        return;
    MotionWork *const motion = FindMotionWork(player);
    if (!motion)
        return;
    if (shenIsGamePaused())
        return;
    const Vector3 position = GetCharacterWorldPosition(player);
    if (practice.player) {
        // A scenario taking the player supersedes the optional practice.
        if (IsScenarioBusy() || authGetLoaded()) {
            practice.player = nullptr;
            return;
        }
        if (!mods::Enabled(mods::Id::TaiChi) || !InPracticeArea(position) || pressed || MovementRequested()) {
            EndPractice(motion);
            return;
        }
        const std::uint16_t animation = practiceMotions[practice.motion];
        const MotnSequence *const sequence = FindLoadedMotionSequence(animation);
        if (!sequence || (mtwkGetAnimationId(motion, MotionRegion::LowerBody) != animation &&
            mtwkGetQueuedAnimationId(motion, MotionRegion::LowerBody) != animation)) {
            EndPractice(motion);
            return;
        }
        mtwkSetRootPosition(motion, practice.position);
        practice.trainingTime += GetFrameTime();
        if (practice.trainingTime >= practice.trainingInterval) {
            practice.trainingTime -= practice.trainingInterval;
            TrainPracticeMove();
            practice.trainingInterval = 30.0f + static_cast<float>(PracticeRandom() % 16U);
        }
        if (sequence->frameCount && mtwkGetFrame(motion, MotionRegion::LowerBody) >= sequence->frameCount - 1.0f) {
            practice.motion ^= 1U;
            mtwkRequestMotion(motion, practiceMotions[practice.motion], MotionRegion::LowerBody);
        }
        return;
    }
    if (!pressed || !mods::Enabled(mods::Id::TaiChi) || !InPracticeArea(position) ||
        !(g_playerRuntime.flags & 2U) || !ryoNotBusy() || IsScenarioBusy() || authGetLoaded())
        return;
#if SHENMUE_ENHANCEMENT_MISC_MODS_CAMERA
    if (mods::FreeCameraActive())
        return;
#endif
    if (!FindLoadedMotionSequence(practiceMotions[0]) || !FindLoadedMotionSequence(practiceMotions[1]))
        return;
    practice.position = position;
    practice.rootMode = mtwkGetTrackedRootMode(motion);
    practice.motion = 0;
    practice.trainingTime = 0.0f;
    practice.trainingInterval = 30.0f + static_cast<float>(PracticeRandom() % 16U);
    SetPlayerControlEnabled(0);
    mtwkSetRequestedRootMode(motion, 3);
    mtwkSetFrameTime(motion, 8, MotionRegion::LowerBody);
    if (mtwkRequestMotion(motion, practiceMotions[0], MotionRegion::LowerBody))
        practice.player = player;
    else
        EndPractice(motion);
}
}
#endif
