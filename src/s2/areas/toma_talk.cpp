#include "s2/areas/area_toma.hpp"
#include "s2/camera.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2::areas {
void QueueTomaFirstTalk() {
    if (!HLib::EnqueueTaskWithParameter(TomaWaitPackageFirstTalk, 4, 0x0b, 0, AREA_EVENT_TASK))
        return;
    EV_SetNewSceneIndex();
    spmFadeOut();
    tomaRaceEntry = 1;
    EV_FreeAuthorPak(1, "0122");
    SetPlayerControlEnabled(0);
    EV_NPC_NewLoadSw(0);
}
void TomaWaitPackageFirstTalk(void *) {
    if (filmCheckFadeTrackColour())
        return;
    SetAreaLoopCharacterControl(TOMA_PLAYER, 1);
    HLib::SetCurrentTaskTo(TomaWaitNpcsFirstTalk);
    HLib::SetupTaskDelay(-1);
    QueueTomaPlusPackage(Filepath_Gen(FilepathGenMode::SceneDirectory));
}
void TomaWaitNpcsFirstTalk(void *) {
    HLib::SetCurrentTaskTo(TomaInitializeTalkAuthor);
    HLib::SetupTaskDelay(-1);
    const shen_name characters[]{TOMA_XGH, {}};
    QueueTomaNpcs(characters);
}
void TomaInitializeTalkAuthor(void *) {
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("TOMAPLUS"), 0);
    EV_LoadCutsceneFromCurrentScene("0122E000");
    EV_LoadAudio("0122E000", 1);
    EV_spmBeginSampleExecute();
    HLib::SetCurrentTaskTo(TomaStartFirstTalkChoices);
    HLib::SetupTaskDelay(-1);
    QueueTomaAuthor(TOMA_INIT_AUTHOR);
}
void TomaStartFirstTalkChoices(void *) {
    if (filmCheckFadeTrackColour())
        return;
    SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    HLib::SetCurrentTaskTo(TomaCheckFirstTalkChoices);
    std::int16_t choices[]{-1, 82, 83, -1};
    SetupControlSpeakAndUI(choices, 800, 83, 2);
}
void TomaCheckFirstTalkChoices(void *) {
    if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
        return;
    if (static_cast<std::uint16_t>(shenGetCtrlSpeakIconFlag()) == 82) {
        HLib::SetCurrentTaskTo(TomaGoRaceFirstTalk);
        EV_stepSetFlags(0, tomaRaceEntry == 0 ? 997 : 1140);
    } else {
        HLib::SetCurrentTaskTo(TomaExitPlus);
        HLib::SetupTaskDelay(-1);
        QueueTomaAuthor(TOMA_DECLINE_AUTHOR);
    }
}
void TomaGoRaceFirstTalk(void *) {
    HLib::SetCurrentTaskTo(TomaGoRace);
    HLib::SetupTaskDelay(-1);
    QueueTomaAuthor(TOMA_GO_AUTHOR);
}
void TomaGoRace(void *) {
    EV_FreeAuthorPak(0, "0122");
    spmFadeOut();
    LoadCutsceneAndSoundProgramForArea();
    EV_spmBeginSampleExecute();
    EV_scenQueueLoad(g_actNumber, TOMA_RACE_AREA, tomaRaceEntry);
}
void TomaExitPlus(void *) {
    if (filmCheckFadeTrackColour())
        return;
    SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    spmFadeOut();
    LoadCutsceneAndSoundProgramForArea();
    EV_spmBeginSampleExecute();
    const shen_name characters[]{TOMA_XGH, {}};
    RestoreAreaLoopCharacters(characters);
    EV_NPC_NewLoadSw(1);
    EV_FreeAuthorPak(0, "0122");
    camCtrlSetState(CameraState::SnapYawTarget);
    SetScenarioFollowCharacter({});
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(CameraState::Init);
    EV_CleanupMapControlTask();
    HLib::MarkCurrentTaskForCleanup();
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("TOMAPLUS"), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("TOMAPLUS"), 0);
    SetAreaLoopCharacterControl(TOMA_PLAYER, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_AssignLightParams(1, 1);
    EV_lightLoad(TOMA_DEFAULT_LIGHT);
    AssignLightProgram(0);
    CheckIsArgEquZero(1);
    EV_SetRefBoardParamsFor(TOMA_PLAYER, 1);
    SetPlayerControlEnabled(1);
}
void QueueTomaTalk() {
    if (!HLib::EnqueueTaskWithParameter(TomaWaitPackageTalk, 4, 0x0b, 0, AREA_EVENT_TASK))
        return;
    tomaRaceEntry = 0;
    EV_SetNewSceneIndex();
    spmFadeOut();
    EV_FreeAuthorPak(1, "0122");
    SetPlayerControlEnabled(0);
    EV_NPC_NewLoadSw(0);
}
void TomaWaitPackageTalk(void *) {
    if (filmCheckFadeTrackColour())
        return;
    SetAreaLoopCharacterControl(TOMA_PLAYER, 1);
    HLib::SetCurrentTaskTo(TomaWaitNpcsTalk);
    HLib::SetupTaskDelay(-1);
    QueueTomaPlusPackage(Filepath_Gen(FilepathGenMode::SceneDirectory));
}
void TomaWaitNpcsTalk(void *) {
    HLib::SetCurrentTaskTo(TomaInitializeTalkAuthor);
    HLib::SetupTaskDelay(-1);
    const shen_name characters[]{TOMA_XGH, {}};
    QueueTomaNpcs(characters);
}
void TomaWaitPackageBye(void *) {
    if (filmCheckFadeTrackColour())
        return;
    SetAreaLoopCharacterControl(TOMA_PLAYER, 1);
    HLib::SetCurrentTaskTo(TomaWaitNpcsBye);
    HLib::SetupTaskDelay(-1);
    QueueTomaPlusPackage(Filepath_Gen(FilepathGenMode::SceneDirectory));
    EV_stepSetFlags(0, 1141);
}
void TomaWaitNpcsBye(void *) {
    HLib::SetCurrentTaskTo(TomaBye);
    HLib::SetupTaskDelay(-1);
    const shen_name characters[]{TOMA_XGH, {}};
    QueueTomaNpcs(characters);
}
void TomaBye(void *) {
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("TOMAPLUS"), 0);
    EV_LoadCutsceneFromCurrentScene("0122E000");
    EV_LoadAudio("0122E000", 1);
    EV_spmBeginSampleExecute();
    HLib::SetCurrentTaskTo(TomaExitPlus);
    HLib::SetupTaskDelay(-1);
    QueueTomaAuthor(TOMA_BYE_AUTHOR);
}
void TomaWaitPackageHaveFirstTalk(void *) {
    if (filmCheckFadeTrackColour())
        return;
    SetAreaLoopCharacterControl(TOMA_PLAYER, 1);
    HLib::SetCurrentTaskTo(TomaWaitNpcsHaveFirstTalk);
    HLib::SetupTaskDelay(-1);
    QueueTomaPlusPackage(Filepath_Gen(FilepathGenMode::SceneDirectory));
}
void TomaWaitNpcsHaveFirstTalk(void *) {
    HLib::SetCurrentTaskTo(TomaInitializeHaveTalkAuthor);
    HLib::SetupTaskDelay(-1);
    const shen_name characters[]{TOMA_XGH, {}};
    QueueTomaNpcs(characters);
}
void TomaInitializeHaveTalkAuthor(void *) {
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("TOMAPLUS"), 0);
    EV_LoadCutsceneFromCurrentScene("0122E000");
    EV_LoadAudio("0122E000", 1);
    EV_spmBeginSampleExecute();
    HLib::SetCurrentTaskTo(TomaStartHaveFirstTalkChoices);
    HLib::SetupTaskDelay(-1);
    QueueTomaAuthor(TOMA_HAVE_INIT_AUTHOR);
}
void TomaStartHaveFirstTalkChoices(void *) {
    if (filmCheckFadeTrackColour())
        return;
    SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    HLib::SetCurrentTaskTo(TomaCheckHaveFirstTalkChoices);
    std::int16_t choices[]{-1, 82, 83, -1};
    SetupControlSpeakAndUI(choices, 800, 83, 2);
}
void TomaCheckHaveFirstTalkChoices(void *) {
    if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
        return;
    if (static_cast<std::uint16_t>(shenGetCtrlSpeakIconFlag()) == 82) {
        HLib::SetCurrentTaskTo(TomaGoRaceHaveFirstTalk);
        EV_stepSetFlags(0, tomaRaceEntry == 0 ? 1143 : 1142);
    } else {
        HLib::SetCurrentTaskTo(TomaExitPlus);
        HLib::SetupTaskDelay(-1);
        QueueTomaAuthor(TOMA_DECLINE_AUTHOR);
    }
}
void TomaGoRaceHaveFirstTalk(void *) {
    HLib::SetCurrentTaskTo(TomaGoRace);
    HLib::SetupTaskDelay(-1);
    QueueTomaAuthor(TOMA_HAVE_GO_AUTHOR);
}
void TomaWaitPackageHaveTalk(void *) {
    if (filmCheckFadeTrackColour())
        return;
    SetAreaLoopCharacterControl(TOMA_PLAYER, 1);
    HLib::SetCurrentTaskTo(TomaWaitNpcsHaveTalk);
    HLib::SetupTaskDelay(-1);
    QueueTomaPlusPackage(Filepath_Gen(FilepathGenMode::SceneDirectory));
}
void TomaWaitNpcsHaveTalk(void *) {
    HLib::SetCurrentTaskTo(TomaInitializeHaveTalkAuthor);
    HLib::SetupTaskDelay(-1);
    const shen_name characters[]{TOMA_XGH, {}};
    QueueTomaNpcs(characters);
}
} // namespace shenmue::s2::areas
