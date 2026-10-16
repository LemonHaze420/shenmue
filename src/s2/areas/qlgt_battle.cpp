#include "s2/areas/area_qlgt.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/sound_program.hpp"
namespace shenmue::s2::areas::qlgt {
void StartBattle(void *data) {
    Work &work = *static_cast<Work *>(data);
    const Course &course = *work.course;
    shenStartFGST(10, 10, 0, 0);
    shenInitFightData(FindCharacterByScenarioName(ryo), nullptr, 0, 0, 0, 0);
    for (std::int32_t i = 0; i < course.enemyCount; ++i) {
        const bool advanced = static_cast<std::uint32_t>(work.entry->course - 2) <= 1;
        shenInitFightData(FindCharacterByScenarioName(actors[2 + i]), nullptr, 1, 1,
            static_cast<std::uint16_t>(advanced ? advancedHealth[i] : ordinaryHealth[i]),
            static_cast<std::uint8_t>(advanced ? advancedStyles[i] : ordinaryStyles[i]));
        SetFaceFlagsForChara_0(FindCharacterByScenarioName(actors[2 + i]), FacialNodeOriginal);
        SetHighModelXYZ(FindCharacterByScenarioName(actors[2 + i]), 0, 1, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(actors[2 + i]), 0, 0, 0);
    }
    vfCopyPosFromEntity(0, nullptr, course.failBattle.x, course.failBattle.z, course.failBattleRadius);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800}, 1);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(ryo), FacialNodeOriginal);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(ren), FacialNodeOriginal);
    SetScenarioCharacterType(ryo, CharacterTypeDefault);
    SetScenarioCharacterType(ren, CharacterTypeDefault);
    SetHighModelXYZ(FindCharacterByScenarioName(ryo), 0, 1, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(ryo), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(ren), 0, 1, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(ren), 0, 0, 0);
    if (work.entry->course != 1) {
        EV_DisableCharacterDisplay(ren);
        EV_ToggleCharacterFlag(ren);
    }
    for (std::int32_t i = 0; i < 3; ++i)
        if (EV_GetShenmueFlag(807 + i)) {
            EV_DisableCharacterDisplay(hiddenDoors[i]);
            EV_ToggleCharacterFlag(hiddenDoors[i]);
        }
    vfmQueueFightStart(static_cast<std::uint8_t>(course.enemyCount + 1), 2, 1, 1, -1);
    EV_vfBattleUtil(FightBattleUtilOp::CombatCameraFlags, shen_name{2}, 1);
    HLib::SetCurrentTaskTo(PlayBattle);
}
void PlayBattle(void *data) {
    Work &work = *static_cast<Work *>(data);
    if (IsFightActive()) {
        TrackDrumTransparency(false);
        return;
    }
    if (GetBtlStatus(FindCharacterByScenarioName(ryo)) == 3) {
        EV_stepSetFlags(0, work.entry->step);
        HLib::SetCurrentTaskTo(Win);
    } else {
        SetCurrentDateTimeTo(eventTemporaryDateTime);
        shenSetTimeSimulationTo(1);
        Queue_ST_LoadTitleFadeIn(retryTitle);
        HLib::SetCurrentTaskTo(Retry);
    }
    TrackDrumTransparency(true);
    Film(30, 0, 255);
    spmFadeOut();
    HLib::SetupTaskDelay(60);
}
void Win(void *data) {
    Work &work = *static_cast<Work *>(data);
    if (work.entry->course != 1) {
        EV_EnableCharacterDisplay(ren);
        EV_EnsureCharacterDraw(ren);
    }
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(ryo), FacialNodeReplacement);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(ren), FacialNodeReplacement);
    SetScenarioCharacterType(ryo, CharacterTypePlayer);
    SetScenarioCharacterType(ren, CharacterTypePlayer);
    SetHighModelXYZ(FindCharacterByScenarioName(ryo), 1, 1, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(ryo), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(ren), 1, 1, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(ren), 1, 0, 0);
    EV_LoadAudio("0263A000", 0);
    EV_spmBeginSampleExecute();
    Film(30, 255, 0);
    const std::int32_t frames = WinDialogue(work.endSide, work.entry->course);
    Film(30, 0, 255, frames - 30);
    if (frames == 60)
        spmFadeOut();
    else {
        HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_FadeBGM_Callback, 6, 0x0b, sizeof(EventFadeBgmTaskData), taskName);
        RegisterEventTask(task);
        EventFadeBgmTaskData *fade = static_cast<EventFadeBgmTaskData *>(HLib::GetTaskParameter(task));

        fade->cleanupTask = 1;
        fade->mode = 1;
        fade->time = static_cast<float>(frames - 60);
    }
    work.remaining = frames;
    HLib::SetCurrentTaskTo(WinWait);
}
void WinWait(void *data) {
    Work &work = *static_cast<Work *>(data);
    if (work.remaining > 0)
        --work.remaining;
    else
        HLib::SetCurrentTaskTo(End);
}
void Retry(void *data) {
    Work &work = *static_cast<Work *>(data);
    if (work.entry->course != 1) {
        EV_EnableCharacterDisplay(ren);
        EV_EnsureCharacterDraw(ren);
    }
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(ryo), FacialNodeReplacement);
    SetScenarioCharacterType(ryo, CharacterTypeDefault);
    SetFaceFlagsForChara_0(FindCharacterByScenarioName(ren), FacialNodeReplacement);
    SetScenarioCharacterType(ren, CharacterTypeDefault);
    SetHighModelXYZ(FindCharacterByScenarioName(ryo), 1, 1, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(ryo), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(ren), 1, 1, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(ren), 1, 0, 0);
    for (std::int32_t i = 0; i < work.course->enemyCount; ++i) {
        SetFaceFlagsForChara_0(FindCharacterByScenarioName(actors[2 + i]), FacialNodeReplacement);
        SetScenarioCharacterType(actors[2 + i], CharacterTypeDefault);
        SetHighModelXYZ(FindCharacterByScenarioName(actors[2 + i]), 1, 1, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(actors[2 + i]), 1, 0, 0);
    }
    work.retry = 1;
    ST_LoadTitleFade();
    HLib::SetCurrentTaskTo(Start);
    HLib::SetupTaskDelay(30);
}
} // namespace shenmue::s2::areas::qlgt
