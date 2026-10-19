#include "s2/areas/area_ab00.hpp"
#include "s2/areas/area_street_watch.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/event_queued.hpp"
#include "s2/flag.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/memo.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/particle_effects.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sleep_event.hpp"
#include "s2/snapshot.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include "s2/fight.hpp"
#include <array>
namespace shenmue::s2::areas {
namespace {
constexpr shen_name ryo{SHEN_NAME(RYO_)}, fighter{SHEN_NAME(SFX_)}, partner{SHEN_NAME(SFB_)};
constexpr shen_name sleepTask{SHEN_NAME(inn_)};
constexpr shen_name withAnnouncer[]{STREET_WATCH_ANNOUNCER, fighter, partner, shen_name{0xffffffffU}};
constexpr shen_name withoutAnnouncer[]{fighter, partner, shen_name{0xffffffffU}};
void PrepareParentedActors();
StreetWatchState street{.package = "ABSF",
    .centre = {266.2200012207031f, 96.8499984741211f, 526.7999877929688f},
    .approachCentre = {266.2200012207031f, 96.8499984741211f, 526.7999877929688f},
    .fighter = fighter,
    .partner = partner,
    .withAnnouncer = withAnnouncer,
    .withoutAnnouncer = withoutAnnouncer,
    .fighterStyle = 14,
    .partnerStyle = 17,
    .formationAssignments = 2,
    .approachRadiusSquared = 36.0f,
    .prepareParentedActors = PrepareParentedActors};
std::array<std::byte, 0x100> distanceWork, firstKernelWork, secondKernelWork;
std::uint32_t fire;
std::int32_t initializing, sleepYaw;
SleepTriggerBox sleepBox;
constexpr SleepTriggerBox authoredSleepBox{{116.8499984741211f, 93.79000091552734f, 546.3599853515625f}, 122.18000030517578f,
    {93.79000091552734f, 546.3599853515625f, 116.8499984741211f, 95.23999786376953f}, 550.4199829101562f,
    {122.18000030517578f, 95.23999786376953f, 550.4199829101562f}, {121.55999755859375f, 94.88999938964844f, 548.7000122070312f}, 0};
constexpr shen_name sleepActors[]{SHEN_NAME(WON_), shen_name{}};
constexpr Vector3 firePositions[]{{289.79998779296875f, 96.23999786376953f, 594.8300170898438f}, {292.0899963378906f, 96.23999786376953f, 589.219970703125f},
    {305.6099853515625f, 96.23999786376953f, 589.219970703125f}, {286.8500061035156f, 96.23999786376953f, 572.9500122070312f},
    {293.4200134277344f, 96.23999786376953f, 589.27001953125f}, {305.6099853515625f, 96.23999786376953f, 593.27001953125f}};
constexpr FloatingSurfaceBinding floatingBindings[]{{5, {132.10000610351562f, 93.9280014038086f, 492.1600036621094f}},
    {6, {132.10000610351562f, 93.9280014038086f, 524.1599731445312f}}, {7, {132.10000610351562f, 93.9280014038086f, 556.1599731445312f}},
    {8, {100.0999984741211f, 93.9280014038086f, 524.1599731445312f}}, {9, {100.0999984741211f, 93.9280014038086f, 556.1599731445312f}}, {-1, {0, 0, 0}}};
void SleepRunCallback(void *);
void SleepPackageCallback(void *);
void SleepAuthorCallback(void *);
void SleepAfterAuthorCallback(void *);
void WakeAuthorCallback(void *);
void WakeAfterAuthorCallback(void *);
void SleepCompleteCallback(void *);
void WonSleepCallback(void *);
void WonAuthorCallback(void *);
void WonAfterAuthorCallback(void *);
void SleepRunCallback(void *) {
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    if (EV_GetShenmueFlag(0x7a))
        HLib::SetCurrentTaskTo(WonSleepCallback);
    else
        QueueSleepCallback(SleepPackageCallback, eAB00_EVT_SleepEvent_SleepFunc2, 504);
}
void SleepAdmissionCallback(void *) {
    if (GetNpcNewLoadState())
        HLib::SetCurrentTaskTo(SleepRunCallback);
}
void QueueInitialSleep() {
    SetPlayerControlEnabled(0);
    EV_setHoldStatesFlags(0);
    mtwkRequestMotion(FindMotionWork(FindCharacterByScenarioName(ryo)), 0xf002, MotionRegion::LowerBody);
    HLib::Task *task = HLib::EnqueueTaskWithoutParameter(SleepAdmissionCallback, 4, 0xb, static_cast<std::int32_t>(STREET_WATCH_TASK.name));
    HLib::SetupTimedTask(task, 10);
}
void SleepPackageCallback(void *) {
    QueueSleepPackageActors("SHIP", sleepActors, SleepAuthorCallback);
}
void SleepAuthorCallback(void *) {
    constexpr shen_name authors[]{SHEN_NAME(SLP0), SHEN_NAME(SLP1), SHEN_NAME(SLP2)};
    EV_FreeAuthorPak(1, "0055");
    EV_LoadCutsceneFromCurrentScene("0055A010");
    EV_LoadAudio("0055A010", 0);
    EV_spmBeginSampleExecute();
    sleepEventData.flags = 0x80;
    sleepEventData.continuation = SleepAfterAuthorCallback;
    sleepCurrentAuthor = authors[shenGetRandomInt32(3)];
    if (sleepEventData.flags & 1U)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (!(sleepEventData.flags & 0x40U)) {
        sleepEventData.savedFilmBars = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetFilmBarsActive));
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    }
    EV_authSceneCtrl((sleepEventData.flags & 0x20U) ? AuthSceneCtrl::DisallowSkipAuthor :
            (sleepEventData.flags & 0x80U)          ? AuthSceneCtrl::AllowSkipAuthor :
                                                      static_cast<AuthSceneCtrl>(-16));
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(static_cast<std::int32_t>(sleepCurrentAuthor.name)));
    HLib::SetCurrentTaskTo(SleepAuthorWaitCallback);
    if (sleepCurrentAuthor.name == SHEN_NAME(SLP2))
        snapAssignFrmCntFlags(136, 120);
}
void SleepAfterAuthorCallback(void *) {
    SetMapModelDataDefaultVertexShader();
    EV_ReinitLights(1);
    EV_SetLightFlagForIndex(0, 2);
    EV_SetIndexForLightEntry(0, 1);
    EV_SetObjectFlagByIndex(11, 0);
    EV_SetObjectFlagByIndex(10, 1);
    SelectBadEndingState(WakeAuthorCallback);
}
void WakeAuthorCallback(void *) {
    constexpr shen_name authors[]{SHEN_NAME(WUP0), SHEN_NAME(WUP1), SHEN_NAME(WUP2)};
    sleepEventData.flags = 0;
    sleepEventData.continuation = WakeAfterAuthorCallback;
    sleepCurrentAuthor = authors[shenGetRandomInt32(3)];
    if (sleepEventData.flags & 1U)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (!(sleepEventData.flags & 0x40U)) {
        sleepEventData.savedFilmBars = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetFilmBarsActive));
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    }
    EV_authSceneCtrl((sleepEventData.flags & 0x20U) ? AuthSceneCtrl::DisallowSkipAuthor :
            (sleepEventData.flags & 0x80U)          ? AuthSceneCtrl::AllowSkipAuthor :
                                                      static_cast<AuthSceneCtrl>(-16));
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(static_cast<std::int32_t>(sleepCurrentAuthor.name)));
    HLib::SetCurrentTaskTo(SleepAuthorWaitCallback);
    if (sleepCurrentAuthor.name == SHEN_NAME(SLP2))
        snapAssignFrmCntFlags(136, 120);
}
void WakeAfterAuthorCallback(void *) {
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(CameraState::Init);
    EV_SetCharaControllerFlags(ryo, CharacterControlUpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(ryo, CharacterControlApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(ryo, CharacterControlUpdateHeight | CharacterControlApplyGravity, NpcCharacterControllerState::Enabled);
    SetMotionRegionFlag(FindMotionWork(FindCharacterByScenarioName(ryo)), 71, 0, MotionRegion::LowerBody);
    EV_AT_SetPos(ryo, EventTransformFlags::AllAxes, {120.0f, 94.30000305175781f, 550.7000122070312f});
    EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes, {0, 43691, 0});
    mtwkRequestMotion(FindMotionWork(FindCharacterByScenarioName(ryo)), 0xf002, MotionRegion::LowerBody);
    sleepEventData.flags = 1;
    sleepEventData.continuation = SleepCompleteCallback;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    RefreshPadTask(2, 1);
    HLib::SetCurrentTaskTo(SleepMemoWaitCallback);
}
void SleepCompleteCallback(void *) {
    AreaAB00Cleanup(0, 0);
    EV_FreeAuthorPak(0, "0055");
    FreeSleepPackageActors();
    SetPlayerControlEnabled(1);
    EV_setHoldStatesFlags(1);
    EV_LoadCutsceneFromCurrentScene("AB00");
    EV_LoadAudio("AB00_000", 0);
    EV_spmBeginSampleExecute();
    EV_NPC_NewLoadSw(1);
    SetPlayerControlEnabled(1);
    HLib::DestroyCurrentTask();
}
void WonLoopCallback(void *) {
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (frame == 570)
        SetChildObjectFlagByIndex(FindCharacterByScenarioName(ryo), 4);
    else if (frame == 802)
        SetChildObjectFlagByIndex(FindCharacterByScenarioName(ryo), 0);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        HLib::DestroyCurrentTask();
}
void WonSleepCallback(void *) {
    SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    spmFadeOut();
    EV_FreeAuthorPak(1, "0143");
    QueueSleepPackageActors("0143", sleepActors, WonAuthorCallback);
}
void WonAuthorCallback(void *) {
    EV_LoadCutsceneFromCurrentScene("0143A000");
    HLib::Task *task = HLib::EnqueueTaskWithoutParameter(WonLoopCallback, 4, 0xb, static_cast<std::int32_t>(sleepTask.name));
    HLib::SetupTimedTask(task, 40);
    sleepEventData.flags = 0xc0;
    sleepEventData.continuation = WonAfterAuthorCallback;
    constexpr shen_name authors[]{SHEN_NAME(0000)};
    sleepCurrentAuthor = authors[shenGetRandomInt32(1)];
    if (sleepEventData.flags & 1U)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (!(sleepEventData.flags & 0x40U)) {
        sleepEventData.savedFilmBars = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetFilmBarsActive));
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    }
    EV_authSceneCtrl((sleepEventData.flags & 0x20U) ? AuthSceneCtrl::DisallowSkipAuthor :
            (sleepEventData.flags & 0x80U)          ? AuthSceneCtrl::AllowSkipAuthor :
                                                      static_cast<AuthSceneCtrl>(-16));
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(static_cast<std::int32_t>(sleepCurrentAuthor.name)));
    HLib::SetCurrentTaskTo(SleepAuthorWaitCallback);
    if (sleepCurrentAuthor.name == SHEN_NAME(SLP2))
        snapAssignFrmCntFlags(136, 120);
}
void WonAfterAuthorCallback(void *) {
    mtwkRequestMotion(FindMotionWork(FindCharacterByScenarioName(ryo)), 0xf002, MotionRegion::LowerBody);
    FreeSleepPackageActors();
    spmFadeOut();
    EV_FreeAuthorPak(0, "0143");
    EV_stepSetFlags(0, 0x7a);
    HLib::SetupTaskDelay(30);
    HLib::SetCurrentTaskTo(SleepPackageCallback);
}
void PrepareParentedActors() {
    if (g_currentMap.name != SHEN_NAME(AB00))
        return;
    const shen_name centre{SHEN_NAME(CNT1)};
    EVT_AT_SetPos(centre, shen_name{0xffffffffU}, {266.2200012207031f, 97.3499984741211f, 526.7999877929688f}, 0);
    EVT_AT_SetAngY(centre, shen_name{0xffffffffU}, 90.0f, 0);
    EVT_AT_SetPos(fighter, centre, {0, 0, .44999998807907104f}, 0);
    EVT_AT_SetPos(partner, centre, {0, 0, -.44999998807907104f}, 0);
    EVT_AT_SetAngY(fighter, centre, 0, 0);
    EVT_AT_SetAngY(partner, centre, 0, 0);
    EVT_AT_SetMot(fighter, 0x2843, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(partner, 0x2844, 0, 1, 0x40000000U, 0);
}
void MinigameCallback(void *payload) {
    StreetWatchMinigameCallback(payload, street);
}
void StreetCallback(void *payload) {
    StreetWatchMainCallback(payload, street);
}
void MainCallback(void *) {
    if (ryoNotBusy() && shenTrySleepTrigger(&sleepBox)) {
        SetPlayerControlEnabled(0);
        EV_setHoldStatesFlags(0);
        mtwkRequestMotion(FindMotionWork(FindCharacterByScenarioName(ryo)), 0xf002, MotionRegion::LowerBody);
        HLib::EnqueueTaskWithoutParameter(SleepRunCallback, 4, 0xb, static_cast<std::int32_t>(sleepTask.name));
    }
}
void CreateAreaFire() {
    fire = static_cast<std::uint32_t>(CreateFireEffect(firePositions[0], .8999999761581421f, .8999999761581421f, 0, 4));
    SetFireFalloffScalar(fire, -.15000000596046448f);
    for (std::int32_t i = 1; i < 6; ++i)
        SetFirePosition(fire, i, &firePositions[i]);
    ResumeParticleEffect(4, fire);
}
void WaveCallback(void *) {
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 2228))
        return;
    DateTime date;
    GetCurrentDateTime(date);
    bool update = ((date.minute & 7) == 0 && date.second == 0) || GetCurrentAuthorId().name == SHEN_NAME(YORU) || initializing;
    if (!update) {
        Vector3 position;
        EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        update = position.x <= 193.0f;
    }
    if (!update)
        return;
    GetInterpolatedWeatherValues();
    const auto colours = GetWeatherWaveColours();
    const std::uint32_t specular = colours[0], diffuse = colours[1];
    BuildWaveLightingTable(1, 1, 1,
        {static_cast<float>((diffuse >> 16) & 255U) / 255.0f * .75f, static_cast<float>((diffuse >> 8) & 255U) / 255.0f * .75f,
            static_cast<float>(diffuse & 255U) / 255.0f * .8999999761581421f},
        {static_cast<float>((specular >> 16) & 255U) / 255.0f * .3100000023841858f, static_cast<float>((specular >> 8) & 255U) / 255.0f * .3100000023841858f,
            static_cast<float>(specular & 255U) / 255.0f * .30000001192092896f},
        42.540000915527344f, 1);
    SetMapModelSlotEnabled(2, 0);
    SetMapModelSlotEnabled(3, 0);
    AdvanceWaterTextureAnimation();
    UpdateFloatingSurfaceModels(2, floatingBindings);
}
} // namespace


void eAB00_EVT_SleepEvent_SleepFunc2(void *) {
    SetPlayerControlEnabled(1);
    EV_setHoldStatesFlags(1);
    EV_NPC_NewLoadSw(1);
    HLib::DestroyCurrentTask();
}
void AreaAB00Init() {
    distanceWork.fill(std::byte{});
    firstKernelWork.fill(std::byte{});
    secondKernelWork.fill(std::byte{});
    street.busy = 1;
    fire = 0;
    street.selection = {};
    street.advice = 0;
    street.motion = 0;
    initializing = 0;
    street.help = 0;
    street.speechStarted = 0;
    street.initialized = 0;
    street.restart = 0;
    street.announcerLoaded = 0;
    street.battleCount = 0;
    street.iconPrepared = 0;
}
void AreaAB00Cleanup(std::int32_t afternoon, std::uint32_t) {
    SetMapModelDataDefaultVertexShader();
    if (afternoon) {
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        CheckIsArgEquZero(1);
        EV_SetObjectFlagByIndex(10, 0);
        EV_SetObjectFlagByIndex(11, 1);
        CreateAreaFire();
    } else {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_SetObjectFlagByIndex(11, 0);
        EV_SetObjectFlagByIndex(10, 1);
        SuspendParticleEffect(4, fire);
    }
    if (!initializing && !EV_GetShenmueFlag(0xc7) && !EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1015))
        EV_LoadAudio("AB00_000", afternoon ? 1 : 0);
}
void AreaAB00Main(std::uint32_t startParameters) {
    initializing = 1;
    SetSoundProgramPhaseFlags(1);
    HLib::Task *task = HLib::EnqueueTaskWithoutParameter(MainCallback, 4, 0xb, static_cast<std::int32_t>(STREET_WATCH_TASK.name));
    HLib::SetupTimedTask(task, 3);
    sleepBox = authoredSleepBox;
    sleepYaw = 49152;
    AreaAB00Cleanup(EV_GetShenmueFlag(0xff), 0);
    if (EV_GetShenmueFlag(0xc7)) {
        SetFightModeFlags(static_cast<std::uint16_t>(GetFightModeFlags() | 1U));
        SetFightModeFlags(static_cast<std::uint16_t>(GetFightModeFlags() | 2U));
    }
    street.queueInitialSleep = !startParameters && g_entryPoint == 95 ? QueueInitialSleep : nullptr;
    QueueStreetWatch(street, MinigameCallback, StreetCallback);
    InitializeWaterTextureAnimation(3);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 2228, 0);
    GenerateFFT(2);
    FillFftDistance(FillFftKernel(10, GenerateDistanceFalloffRowMajor(1.7170000076293945f, distanceWork)));
    void *second = FillFftKernel(.0729999989271164f, GenerateDistanceFalloffKernel8x8(.6470000147819519f, secondKernelWork));
    void *first = FillFftKernel(.061000000685453415f, GenerateDistanceFalloffKernel8x8(.37599998712539673f, firstKernelWork));
    ApplyFft(first, second);
    FinalizeFft(.0010000000474974513f, .0010000000474974513f);
    SetWaveLighting(.10000000149011612f, {-.1632000058889389f, .9818000197410583f, -.09690000116825104f});
    HLib::EnqueueTaskWithoutParameter(WaveCallback, 3, 2, static_cast<std::int32_t>(SHEN_NAME(wave)));
    SetMapModelVertexShaderType(2, 1);
    SetMapModelVertexShaderType(3, 1);
    WaveCallback(nullptr);
    initializing = 0;
}
const GameplayAreaDefinition areaAB00GameplayDefinition{SHEN_NAME(AB00), 0, AreaAB00Init, AreaAB00Main, AreaAB00Cleanup};
} // namespace shenmue::s2::areas
