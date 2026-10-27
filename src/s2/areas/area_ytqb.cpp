#include "s2/areas/area_q7qb.hpp"
#include "s2/areas/area_ytqb.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/face.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/inventory.hpp"
#include "s2/load_title.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/qte_hand_poses.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"

namespace shenmue::s2::areas {
std::int32_t ytqbAmbientSmoke = -1, ytqbFinalAuthorActive = -1;
namespace {
void InitializeYtqb() {
    ytqbAmbientSmoke = -1;
}
void RunYtqb(std::uintptr_t) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(YtqbStart, 4, 11, sizeof(YtqbWork), YTQB_EVNT);
    YtqbWork *work = static_cast<YtqbWork *>(HLib::GetTaskParameter(task));
    work->reserved10 = 0;
    work->task = task;
    YtqbSetup(*work);
    GlowParameters glow{.index = 5, .intensity = .3f, .values = {.3f, .3f, .3f, .2f}};
    SelectPostFxBank(1, 1);
    UploadGlowParams(glow);
    SelectPostFxBank(3, 1);
    UploadBlurStrength(1);
    ytqbFinalAuthorActive = 0;
}
void FilmBlack() {
    EventFilmControl film{.cleanup = 0,
        .frames = 30,
        .startAlpha = 255,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 0,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 0};
    EVT_AT_MenCtrl_Callback(&film);
}
void RetireNpcRequests(EventNpcReadWork &work) {
    for (std::int32_t i = 0; i < 32; ++i)
        if (work.states[i] == 2 || work.requests[i] == 2)
            work.requests[i] = 3;
}
void EnableFinalPluralControl() {
    for (std::int32_t i = 0; i < 6; ++i)
        SetPluralControlFor(FindCharacterByScenarioName(YTQB_FinalNames[i]), 1);
}
void StartCombinationPrompt(std::int32_t second, std::int32_t third) {
    qteBattleUtil(BattleQteMode::SetLastCommands, 16, second, third);
    std::int16_t frames = 24;
    ReInitElement(UIElement::GameQte);
    if (g_eventQteRetryFrames > 0)
        frames = static_cast<std::int16_t>(static_cast<std::int16_t>(EVT_GetQteFailureCount()) * ((g_eventQteRetryFrames - 24) / 2) + 24);
    EventQtePromptTaskData prompt{.autoCleanupTask = 0, .requiredStatus = 0, .maximumFrames = frames, .reserved08 = 0, .displayState = 4, .delayFrames = 0};
    EVT_AT_Qte_Callback(&prompt);
}
} // namespace
void YtqbPosition(shen_name character, shen_name parent, const Vector3 &position) {
    EventSetPosition request{.cleanup = 0, .character = character, .parent = parent, .position = position, .delay = 0};
    EVT_AT_SetPosRot(&request);
}
void YtqbAngle(shen_name character, shen_name parent, float degrees) {
    EventSetAngle request{.cleanup = 0, .character = character, .parent = parent, .degrees = degrees, .delay = 0};
    EVT_AT_SetAngY_Callback(&request);
}
void YtqbStand(shen_name character) {
    EventSetMotion request{.cleanup = 0, .character = character, .animation = 13, .frame = 2, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&request);
    GetMotionFrameCount(13);
}
void YtqbCamera(std::int32_t camera, std::uint8_t context, float angle) {
    EventSetCamera request{.cleanup = 0, .camera = camera, .eye = YTQB_DUMR, .target = YTQB_DUMR, .context = context, .angle = angle, .delay = 0};
    EVT_AT_SetCam_Callback(&request);
    camCtrlDisableFilmBars();
}
void YtqbPrompt(YtqbWork &work, const EventQteData &prompt, const EventQteData &authored) {
    work.prompt = &prompt;
    work.authored = &authored;
    HLib::SetupTaskDelay(static_cast<std::int32_t>(Queue_EVT_AT_ResultQTE(&work.result, &work.parameters, &prompt)));
}
void YtqbApplyResult(YtqbWork &work) {
    EventQteAuthoredCallback callback = work.result ? work.authored->resultSuccess : work.authored->resultFailure;
    if (callback)
        callback(&work.parameters, 0);
}
void YtqbTransition(YtqbWork &work, HLib::TaskCallback continuation, std::int32_t delay, HLib::TaskCallback callback) {
    work.continuation = continuation;
    HLib::SetCurrentTaskTo(callback ? callback : continuation);
    if (delay)
        HLib::SetupTaskDelay(delay);
}
void YtqbSetup(YtqbWork &work) {
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    EVT_SetQteRetryFrames(24);
    EVT_SetQteFailureCount(0);
    GetCurrentDateTime(eventTemporaryDateTime);
    work.characters = Queue_EVT_AT_ReadChr(work.task, YTQB_PlayerNames);
    work.npcs = nullptr;
    for (const char *package : {"YTQB", "0048", "048E"}) {
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>(package));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    }
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("YTQB"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("YTQB"), 0);
    EV_FreeAuthorPak(1, "S2BV");
    EV_FreeAuthorPak(1, "0048");
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0048A000");
    EV_FreeWAIR();
    EV_UpdateWeatherForMinute(0, 0, 0, 0);
    EV_UpdateWeatherForMinute(1, 0, 0, 0);
    EV_UpdateWeather();
    SetWeatherInstruction(12, false);
    SetWeatherInstruction(0x780, false);
    EV_lightLoad(YTQB_0000);
    PushGlobalLightingProgram();
    shenSetClockAndCompassRenderTo(1);
    nullsub();
    qteBattleUtil(BattleQteMode::Load);
    qteBattleUtil(BattleQteMode::Queue);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EventTickFunction_Callback, 6, 11, 0x20, YTQB_EVNT);
    RegisterEventTask(task);
    EventTickFunctionWork *call = static_cast<EventTickFunctionWork *>(HLib::GetTaskParameter(task));
    call->callback = YtqbAmbientSmoke;
    call->maxTime = 200000000;
    call->currentTicks = 0;
    call->paramSize = 0;
    call->delay = 0;
}
void YtqbStart(void *data) {
    YtqbWork &work = *static_cast<YtqbWork *>(data);
    if (work.npcs)
        for (std::int32_t i = 0; i < 32; ++i)
            work.npcs->requests[i] = 3;
    else
        work.npcs = QueueEventNpcRead();
    RequestEventNpcCharacters(work.npcs, YTQB_FirstNames);
    AT_SetLocalBaseChara(YTQB_DUMR, YTQB_RYO_);
    YtqbPosition(YTQB_DUMR, YTQB_RYO_, {});
    YtqbAngle(YTQB_DUMR, YTQB_RYO_, 0);
    YtqbPosition(YTQB_BASE, YTQB_INVALID, YTQB_WorldBase);
    YtqbAngle(YTQB_BASE, YTQB_INVALID, 0);
    SetMotionFrameTime(scenFindMotionwork(YTQB_RYO_), 8, MotionRegion::LowerBody);
    SetLowQualityHands(FindCharacterByScenarioName(YTQB_RYO_));
    work.parameters.player = YTQB_RYO_;
    work.continuation = YtqbSboAttack;
    work.parameters.firstOpponent = YTQB_INVALID;
    work.parameters.secondOpponent = YTQB_INVALID;
    work.parameters.motionFlags = 0;
    work.phase = 0;
    work.result = 0;
    work.prompt = nullptr;
    work.authored = nullptr;
    YtqbSetupLantern();
    YtqbSetupWater();
    YtqbSetupBucket();
    YtqbSetupStallObjects();
    for (shen_name character : {YTQB_YAD3, YTQB_YAD2}) {
        const shen_name broken{character.name | 0x100000U};
        EV_DisableCharacterDisplay(character);
        EV_EnableCharacterDisplay(broken);
        SetScenarioCharacterDrawControl(character, 0, 0);
        SetScenarioCharacterDrawControl(broken, 0, 0);
    }
    HLib::SetCurrentTaskTo(YtqbStartIntroAuthor);
}
void YtqbStartIntroAuthor(void *data) {
    YtqbWork &work = *static_cast<YtqbWork *>(data);
    if (work.npcs->busy)
        return;
    SelectPostFxBank(3, 1);
    UploadBlurStrength(1);
    EV_LoadAudio("0048A000", 1);
    EV_spmBeginSampleExecute();
    FilmBlack();
    EV_lightLoad(YTQB_l201);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(YTQB_ATH1.name));
    work.continuation = YtqbSboAttack;
    HLib::SetCurrentTaskTo(work.continuation);
}
void YtqbStartWaterAuthor(void *data) {
    YtqbWork &work = *static_cast<YtqbWork *>(data);
    if (work.npcs->busy)
        return;
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(YTQB_STR0.name));
    camCtrlSetNearDistance(12);
    EventCling cling{.cleanup = 0,
        .target = YTQB_SUI2,
        .character = YTQB_KIC_,
        .bone = 14,
        .position = {.x = -.43379101157188416f, .y = -2.3200600147247314f, .z = -.1769540011882782f},
        .rotation = {.x = -364, .y = 22573, .z = -2730},
        .mode = 0,
        .delay = 0};
    EVT_AT_Cling(&cling);
    EV_EnableCharacterDisplay(YTQB_SUI2);
    HLib::SetCurrentTaskTo(YtqbWaitWaterAuthor);
}
void YtqbWaitWaterAuthor(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 1)
        SetPluralControlFor(FindCharacterByScenarioName(YTQB_RYO_), 1);
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(YTQB_RYO_), 0, reinterpret_cast<std::byte *>(ryoQteHandPose), 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(YTQB_RYO_), 1, reinterpret_cast<std::byte *>(ryoQteHandPose), 0);
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(YTQB_RYO_), static_cast<std::int32_t>(FacialExpressionId::Angry), 1, 1);
    StartCombinationPrompt(16, 1024);
    HLib::SetCurrentTaskTo(YtqbWaterResult);
}
void YtqbWaterResult(void *data) {
    if (!qteBattleUtil(BattleQteMode::GetResult))
        return;
    YtqbWork &work = *static_cast<YtqbWork *>(data);
    EventCling cling;
    cling.cleanup = 0;
    cling.target = YTQB_SUI2;
    cling.character = YTQB_KIC_;
    cling.mode = 1;
    cling.delay = 0;
    EVT_AT_Cling(&cling);
    EV_DisableCharacterDisplay(YTQB_SUI2);
    YtqbResetWater();
    const std::int32_t delay = qteBattleUtil(BattleQteMode::GetCompletedCount) == 3 ? YtqbWaterSuccess() : YtqbWaterFailure();
    RetireNpcRequests(*work.npcs);
    HLib::SetCurrentTaskTo(work.continuation);
    HLib::SetupTaskDelay(delay);
}
void YtqbStartFinalAuthor(void *data) {
    YtqbWork &work = *static_cast<YtqbWork *>(data);
    if (work.npcs->busy)
        return;
    LoadBlurModelForCharacter(YTQB_SYE_, "SYE_B.MT7", 1);
    if (ytqbFinalAuthorActive)
        EV_SetAuthorActiveTo(1);
    EV_spmBeginSampleExecute();
    FilmBlack();
    EV_lightLoad(YTQB_l203);
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(YTQB_ATH3.name));
    HLib::SetCurrentTaskTo(YtqbWaitFinalAuthor);
}
void YtqbWaitFinalAuthor(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 1)
        EnableFinalPluralControl();
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    StartCombinationPrompt(4, 256);
    HLib::SetCurrentTaskTo(YtqbFinalResult);
}
void YtqbFinalResult(void *data) {
    if (!qteBattleUtil(BattleQteMode::GetResult))
        return;
    YtqbWork &work = *static_cast<YtqbWork *>(data);
    if (qteBattleUtil(BattleQteMode::GetCompletedCount) == 3) {
        EV_lightLoad(YTQB_l204);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(YTQB_ATH4.name));
        work.continuation = YtqbFinish;
    } else {
        EV_lightLoad(YTQB_l205);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(YTQB_ATH5.name));
        if (EVT_GetQteFailureCount() < 2)
            EVT_SetQteFailureCount(EVT_GetQteFailureCount() + 1);
        SetCurrentDateTimeTo(eventTemporaryDateTime);
        shenSetTimeSimulationTo(1);
        work.continuation = YtqbStartFinalAuthor;
        if (ytqbFinalAuthorActive == 1)
            EV_SetAuthorActiveTo(1);
        ytqbFinalAuthorActive = 1;
    }
    HLib::SetCurrentTaskTo(YtqbWaitResultAuthor);
}
void YtqbWaitResultAuthor(void *data) {
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 1)
        EnableFinalPluralControl();
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    spmFadeOut();
    HLib::SetCurrentTaskTo(static_cast<YtqbWork *>(data)->continuation);
    HLib::SetupTaskDelay(60);
}
void YtqbFinish(void *data) {
    YtqbWork &work = *static_cast<YtqbWork *>(data);
    if (work.npcs->busy)
        return;
    for (std::int32_t i = 0; i < 32; ++i)
        work.npcs->requests[i] = 3;
    YtqbExit(work);
}
void YtqbWaitMotion(void *data) {
    YtqbWork &work = *static_cast<YtqbWork *>(data);
    if (SetMotionRegionFlag(scenFindMotionwork(YTQB_RYO_), 65, 1, MotionRegion::LowerBody) && work.npcs->characters[24].name == 0)
        HLib::SetCurrentTaskTo(work.continuation);
}
void YtqbNextQte(void *data) {
    YtqbWork &work = *static_cast<YtqbWork *>(data);
    if (work.npcs->busy)
        return;
    RetireNpcRequests(*work.npcs);
    HLib::SetCurrentTaskTo(work.continuation);
}
void YtqbRetry(void *data) {
    YtqbWork &work = *static_cast<YtqbWork *>(data);
    if (work.npcs->busy)
        return;
    const std::int32_t delay = PlayStandingQteRetryVoices();
    EventFilmControl film{.cleanup = 0,
        .frames = 30,
        .startAlpha = 0,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 255,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 0};
    EVT_AT_MenCtrl_Callback(&film);
    EVT_AT_FadeBGM(0, 1, delay - 60);
    if (EVT_GetQteFailureCount() < 2)
        EVT_SetQteFailureCount(EVT_GetQteFailureCount() + 1);
    SetCurrentDateTimeTo(eventTemporaryDateTime);
    shenSetTimeSimulationTo(1);
    EV_SetAuthorActiveTo(1);
    HLib::SetCurrentTaskTo(YtqbStart);
    HLib::SetupTaskDelay(delay);
}
void YtqbLoadFinalCharacters(void *data) {
    YtqbWork &work = *static_cast<YtqbWork *>(data);
    RequestEventNpcCharacters(work.npcs, YTQB_FinalNames);
    RetireNpcRequests(*work.npcs);
    SelectPostFxBank(3, 0);
    HLib::SetCurrentTaskTo(YtqbStartFinalAuthor);
}
void YtqbCleanup(YtqbWork &work) {
    RemoveInventoryItem(36, 1);
    RemoveInventoryItem(37, 1);
    RemoveInventoryItem(38, 1);
    RemoveInventoryItem(40, 1);
    SetPluralControlFor(FindCharacterByScenarioName(YTQB_RYO_), 0);
    hndlSetFlags(FindCharacterByScenarioName(YTQB_RYO_));
    qteBattleUtil(BattleQteMode::RetireAndFree);
    FinishLoading();
    shenSetClockAndCompassRenderTo(0);
    LoadCutsceneAndSoundProgramForArea();
    EV_spmBeginSampleExecute();
    EV_FreeAuthorPak(0, "S2BV");
    EV_FreeAuthorPak(0, "0048");
    for (const char *package : {"YTQB", "0048", "048E"})
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(package), 0);
    ET_AT_KillWork(work.characters);
}
void YtqbExit(YtqbWork &work) {
    SelectPostFxBank(1, 0);
    SelectPostFxBank(3, 0);
    UploadBlurStrength(1);
    YtqbCleanup(work);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    if (g_currentMap.name == YTQB_YTQB.name) {
        scenSetTaskFlags(8);
        EV_scenQueueLoad(g_actNumber, YTQB_0048, 2);
    }
}
const ScriptAreaDefinition ytqbScriptDefinition{.areaId = YTQB_YTQB, .field04 = 0, .init = InitializeYtqb, .main = RunYtqb};
} // namespace shenmue::s2::areas
