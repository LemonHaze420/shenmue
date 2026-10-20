#include "s2/areas/area_gabt.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/motion_work.hpp"
#include "s2/pause.hpp"
#include "s2/screen_film.hpp"
#include <cstring>
namespace shenmue::s2::areas {
namespace {
std::int32_t ReadBattleScratch(const CompiledSequenceWork &work, std::size_t offset) {
    std::int32_t value;
    std::memcpy(&value, reinterpret_cast<const std::byte *>(&work) + offset, sizeof(value));
    return value;
}
void WriteBattleScratch(CompiledSequenceWork &work, std::size_t offset, std::int32_t value) {
    std::memcpy(reinterpret_cast<std::byte *>(&work) + offset, &value, sizeof(value));
}
void BattleTransition(CompiledSequenceWork &work, std::uint16_t mode) {
    work.previousMode = work.mode;
    work.mode = mode;
    work.continueAfterWait = 1;
    work.continueLoop = 1;
}
void BattleWait(CompiledSequenceWork &work, std::uint16_t wait, std::uint16_t next, std::int32_t delay) {
    work.mode = wait;
    work.nextMode = next;
    work.continueAfterWait = 1;
    work.delay = delay;
}
// @verified - p: callback1079a0 is a battle-dialogue key producer, not a QTE prompt.
std::int64_t GabtCycleBattleAdviceKey(void *data) {
    std::uint16_t &index = *static_cast<std::uint16_t *>(data);
    constexpr std::uint16_t keys[]{184, 189, 185, 190, 186, 191, 187, 192, 188, 193};
    const std::int64_t result = keys[index];
    ++index;
    if (index >= 10)
        index = 0;
    return result;
}
} // namespace
// @verified - p: native107360 complete SEQP graph; scratch offsets1d0/248 verified in live x64 instructions.
void GabtBattleSequenceCallback(void *data) {
    using enum CameraState;
    CompiledSequenceWork &work = *static_cast<CompiledSequenceWork *>(data);
    work.callbackFlag = 0;
    do {
        work.continueLoop = 0;
        switch (work.mode) {
        case 0: {
            AT_SetLocalBaseChara(GABT_MOLE_RYO, GABT_MOLE_INVALID);
            AT_SetLocalBaseChara(GABT_MOLE_GAI, GABT_MOLE_INVALID);
            EV_AT_SetPos(GABT_MOLE_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, GABT_PLAYER_POSITION);
            EV_AT_SetPos(GABT_MOLE_GAI, EventTransformFlags::World | EventTransformFlags::AllAxes, GABT_OPPONENT_POSITION);
            Vector3i rotation{.x = 0, .y = 49152, .z = 0};
            EV_AT_SetAngY(GABT_MOLE_RYO, EventTransformFlags::AllAxes, rotation);
            rotation.y = 0x4000;
            EV_AT_SetAngY(GABT_MOLE_GAI, EventTransformFlags::AllAxes, rotation);
            EV_AT_SetPos(GABT_MOLE_SCENERY, EventTransformFlags::World | EventTransformFlags::AllAxes, GABT_SCENERY_POSITION);
            rotation.y = 0;
            EV_AT_SetAngY(GABT_MOLE_SCENERY, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
            mtwkRequestMotion(scenFindMotionwork(GABT_MOLE_RYO), 306, MotionRegion::LowerBody);
            mtwkRequestMotion(scenFindMotionwork(GABT_MOLE_GAI), 306, MotionRegion::LowerBody);
            SetMotionRegionFlag(scenFindMotionwork(GABT_MOLE_RYO), 71, 0, MotionRegion::LowerBody);
            SetMotionRegionFlag(scenFindMotionwork(GABT_MOLE_GAI), 71, 0, MotionRegion::LowerBody);
            EV_EnableCharacterDisplay(GABT_MOLE_RYO);
            EV_EnableCharacterDisplay(GABT_MOLE_GAI);
            BattleWait(work, 2, 100, 6);
            break;
        }
        case 12:
            if (!IsFightActive()) {
                SetScreenFilmFade(5, 0, 0, 0, 0, 255, 0, 0, 0);
                BattleTransition(work, 106);
            }
            break;
        case 100: {
            const std::uint64_t camera[]{9025, 0, 0};
            camCtrlToggleSplineRebuildFlag(3, camera);
            camCtrlSetState(Scripted);
            SetScreenFilmFade(30, 255, 0, 0, 0, 70, 0, 0, 0);
            shenStartFGST(10, 10, 0, 0);
            vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("GABT"), 1);
            shenInitFightData(FindCharacterByScenarioName(GABT_MOLE_RYO), nullptr, 0, 0, 0, 0);
            shenInitFightData(FindCharacterByScenarioName(GABT_MOLE_GAI), nullptr, 1, 1, 20000, 0x2c);
            vfUpdateFlagsFor(FindCharacterByScenarioName(GABT_MOLE_RYO), 0, 0x20000, 30);
            vfCopyPosFromEntity(0, nullptr, 287.619995f, 76.4599991f, 2.2f);
            EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x200}, 1);
            EV_vfBattleUtil(FightBattleUtilOp::CombatCameraFlags, shen_name{2}, 1);
            vfmQueueFightStart(2, 0x63, 1, 0, -1);
            EV_vfBattleUtil(FightBattleUtilOp::MoveFlag400000, GABT_MOLE_RYO, 1);
            camCtrlSetFovTransitionTo(false);
            GabtPrepareReflectors();
            const std::uint32_t dialogue = AllocateBattleDialogueCondition15(12, -2, GABT_MOLE_RYO, shen_name{}, 900, 100, 1);
            WriteBattleScratch(work, 0x248, static_cast<std::int32_t>(dialogue));
            const std::uint32_t advice = AllocateBattleDialogueCondition15(20, -1, GABT_MOLE_RYO, shen_name{}, 150, 100, 1);
            AssignBattleDialogueCallback(advice, GabtCycleBattleAdviceKey);
            WriteBattleScratch(work, 0x1d0, 0);
            BattleTransition(work, 102);
            break;
        }
        case 101:
            BattleTransition(work, 100);
            break;
        case 102:
            if (!filmCheckFadeTrackColour()) {
                FinishLoading();
                BattleTransition(work, 103);
            }
            break;
        case 103:
            if (!IsFightActive())
                BattleTransition(work, 12);
            else if (ReadBattleScratch(work, 0x1d0) > 1800) {
                check_dead_status(1);
                BattleTransition(work, 12);
            } else if (!GetBattleDialogueCompletionCount(static_cast<std::uint32_t>(ReadBattleScratch(work, 0x248))))
                WriteBattleScratch(work, 0x1d0, ReadBattleScratch(work, 0x1d0) + 1);
            else {
                EV_SetCinemaBarVisibility(FilmControlFlag::Show, 5);
                EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{4}, 1);
                SetFightFlag400000(1);
                WriteBattleScratch(work, 0x1d0, 0);
                BattleTransition(work, 104);
            }
            break;
        case 104:
            if (ReadBattleScratch(work, 0x1d0) > 300) {
                check_dead_status(1);
                EV_SetCinemaBarVisibility(FilmControlFlag::Hide, 5);
                BattleTransition(work, 12);
            } else
                WriteBattleScratch(work, 0x1d0, ReadBattleScratch(work, 0x1d0) + 1);
            break;
        case 106:
            BattleWait(work, 11, 107, 2);
            break;
        case 107:
            figpAssignModeFor(GABT_MOLE_RYO, 0);
            figpAssignModeFor(GABT_MOLE_GAI, 0);
            RetireCompiledSequenceLinks(&work);
            HLib::DestroyCurrentTask();
            break;
        default:
            UpdateCompiledSequenceWait(&work);
            break;
        }
    } while (work.continueLoop);
}
} // namespace shenmue::s2::areas
