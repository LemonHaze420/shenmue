#include "s2/areas/pati_event.hpp"
#include "s2/areas/pati_event_tables.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/help.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <cstdio>
#include <cstring>
namespace shenmue::s2 {
namespace {
inline constexpr shen_name PATI_CHALLENGE_MAP{SHEN_NAME(Q100)};
}
// @verified - p
void LockPatiEvent(PatiEventWork *work, const char *argument) {
    work->entryFlag = 0;
    SetPlayerControlEnabled(0);
    EV_SetCharaControllerFlags(PATI_PLAYER, 2, static_cast<NpcCharacterControllerState>(0));
    EV_SetCharaControllerFlags(PATI_PLAYER, 3, static_cast<NpcCharacterControllerState>(0));
    EV_NPC_NewLoadSw(0);
    npcReleaseUnusedStreams();
    work->nearbyDealer = FindNearestPatiDealer();
    if (work->nearbyDealer.name)
        EV_CheckWarmStartNpcRoute(work->nearbyDealer);
    EV_ResetFreeTalkFlags();
    if (g_currentMap.name == PATI_CHALLENGE_MAP.name)
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1234, 1);
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    std::memcpy(work->argument, argument, 4);
    work->argument[4] = 0;
    if (!std::memcmp(work->argument, "PVSJ", 4)) {
        std::memcpy(work->opponent, argument + 4, 4);
        work->opponent[4] = 0;
        std::memcpy(work->versusBoard, argument + 8, 4);
        work->versusBoard[4] = 0;
        std::memcpy(work->dealerGroup, argument + 12, 4);
        work->dealerGroup[4] = 0;
    } else {
        work->opponent[0] = 0;
        work->versusBoard[0] = 0;
        work->dealerGroup[0] = 0;
    }
    HLib::SetupTaskDelay(0);
}
// @verified - p
bool LoadPatiEvent(PatiEventWork *work) {
    if (g_currentMap.name == PATI_CHALLENGE_MAP.name && EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1234))
        return false;
    if (!EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("PATI"), 0)) {
        if (!EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load,
                reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::PackDirectory)), reinterpret_cast<std::uintptr_t>("PATI")))
            return false;
        if (!EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0))
            return false;
    }
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("PATI"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("PATI"), 0);
    EV_LoadCutsceneFromCurrentScene("01040000.smp");
    EV_LoadAudio("01040000", 0);
    PATI_TBL_Parser(&work->tables);
    return true;
}
// @verified - p
void CreatePatiEventResources(PatiEventWork *work) {
    Vector3 player;
    EV_GetCharaPosition(PATI_PLAYER, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::World);
    work->savedMotion = CaptureMotionSnapshot(work->dealer);
    const PatiDealer *dealer = PATI_DEALERS;
    while (dealer->character.name && dealer->character.name != work->dealer.name)
        ++dealer;
    g_patiPlayerCamera = 901;
    g_patiDealerCamera = 911;
    if (dealer->motions) {
        work->motions = dealer->motions;
        g_patiDealerCamera = dealer->camera;
    } else
        work->motions = &PATI_DEALER_MOTIONS;
    const std::uint32_t name = work->dealer.name;
    std::sprintf(work->game.voicePrefix, "0104%c%c%c", name & 255, (name >> 8) & 255, (name >> 16) & 255);
    const PatiVoicePartition *voice = PATI_VOICE_PARTITIONS;
    while (voice->name && std::strcmp(voice->name, work->game.voicePrefix))
        ++voice;
    if (!voice->name)
        std::sprintf(work->game.voicePrefix, "0104");
    const std::int64_t partition = EV_ControlPackageResources(PakCategory::Partition, PakPartitionOp::Mount,
        reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::PackDirectory)), reinterpret_cast<std::uintptr_t>("PATI.afs"));
    EV_ControlPackageResources(
        PakCategory::Package, PakPackageOp::LoadAfs, static_cast<std::uintptr_t>(partition), static_cast<std::uint32_t>(voice->text), voice->sound);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(
        PakCategory::Task, PakTaskOp::SetCurrentAfs, static_cast<std::uintptr_t>(partition), static_cast<std::uint32_t>(voice->text), voice->sound);
    const void *source = LoadCurrentPakBinary(work->game.voicePrefix);
    const std::int32_t bytes = static_cast<std::int32_t>(GetCurrentPakBinarySize(work->game.voicePrefix));
    const std::int32_t rounded = (bytes + ((bytes >> 31) & 3)) & ~3;
    work->game.voiceLengths = static_cast<PatiVoiceLength *>(HLib::AllocateTaskData(static_cast<std::uint64_t>(rounded), PATI_AREA));
    std::memcpy(work->game.voiceLengths, source, static_cast<std::size_t>(rounded));
    EV_ControlPackageResources(
        PakCategory::Package, PakPackageOp::FreeAfs, static_cast<std::uintptr_t>(partition), static_cast<std::uint32_t>(voice->text), voice->sound);
    EV_ControlPackageResources(PakCategory::Partition, PakPartitionOp::Unmount, static_cast<std::uintptr_t>(partition), 0);
    EV_FreeAuthorPak(1, work->game.voicePrefix);
    UpdateMotionWorkWithPosition(FindMotionWork(FindCharacterByScenarioName(PATI_PLAYER)), nullptr, 0);
    UpdateMotionWorkWithPosition(FindMotionWork(FindCharacterByScenarioName(work->dealer)), nullptr, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("PATI"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("PATI"), 0);
    EV_SetCharacterAlphaEnabled(work->dealer, false);
    EV_SetRefBoardParamsFor(PATI_PLAYER, 1);
    EV_SetRefBoardParamsFor(work->dealer, 1);
    FindMotionWork(FindCharacterByScenarioName(PATI_PLAYER));
    FindMotionWork(FindCharacterByScenarioName(work->dealer));
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(PATI_PLAYER), 0, 10.0f, 1.0f);
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(work->dealer), 0, 10.0f, 1.0f);
}
// @verified - p
void PositionPatiPlayer(PatiEventWork *work) {
    work->dealer = FindPatiDealer(work);
    Vector3 player, dealer;
    Vector3i playerRotation, dealerRotation;
    EV_GetCharaPosition(PATI_PLAYER, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::World);
    EV_GetCharaPosition(work->dealer, CHARACTER_TRANSLATION_ROOT, &dealer, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(PATI_PLAYER, &playerRotation, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(work->dealer, &dealerRotation, EventTransformFlags::World);
    dealerRotation.y += 0x8000;
    const float sine = sin_fixed16(static_cast<std::int16_t>(dealerRotation.y));
    const float cosine = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(dealerRotation.y))));
    playerRotation.y = dealerRotation.y;
    EventSetPositionAngle position;
    position.cleanup = 0;
    position.character = PATI_PLAYER;
    position.parent = shen_name{0xffffffffU};
    position.position.y = 0.0f;
    position.delay = 0.0f;
    player.x = (sine * 1.8f + cosine * 0.0f) + dealer.x;
    position.position.x = player.x;
    player.z = (cosine * 1.8f - sine * 0.0f) + dealer.z;
    position.position.z = player.z;
    position.degrees = static_cast<float>(dealerRotation.y) / 182.04445f;
    EVT_AT_SetPosAng_Callback(&position);
}
// @verified - p
void BeginPatiExit(PatiEventWork *work) {
    RestorePatiNailModels();
    for (PatiBallCharacter &ball : work->game.ballCharacters)
        if (ball.character.name) {
            SetScenarioCharacterKillMode(ball.character, 1);
            ball.character = {};
        }
    SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    if (g_currentMap.name == PATI_CHALLENGE_MAP.name || work->challengeComplete)
        ProcessAicaCommand(0xaa0, 30);
    else
        HLib::SetupTaskDelay(30);
}
// @verified - p
bool RestorePatiEvent(PatiEventWork *work) {
    if (g_currentMap.name == PATI_CHALLENGE_MAP.name || work->challengeComplete) {
        if (filmCheckFadeTrackColour())
            return false;
    } else {
        if (!g_patiVersus)
            EV_DisableCharacterDisplay(PATI_PLAYER);
        EV_AT_SetPos(PATI_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, work->game.savedPlayerPosition);
        EV_AT_SetAngY(PATI_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, work->game.savedPlayerRotation, nullptr);
    }
    RestoreMotionSnapshot(work->dealer, static_cast<std::uint32_t>(work->savedMotion));
    ReleaseMotionSnapshot(static_cast<std::uint32_t>(work->savedMotion));
    if (std::memcmp(work->argument, "PVSJ", 4)) {
        EV_SetRefBoardParamsFor(PATI_PLAYER, 0);
        EV_SetRefBoardParamsFor(work->dealer, 0);
        if (FindPatiDealer(work).name)
            EV_SetCharacterAlphaEnabled(work->dealer, true);
        EV_EventSetUpHelpIcon_0(0, 0);
        EVS_LoopCamera(0, {}, {});
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(CameraState::Init);
    }
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(PATI_PLAYER), 0, 10.0f, 1.0f);
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(work->dealer), 0, 10.0f, 1.0f);
    FindMotionWork(FindCharacterByScenarioName(PATI_PLAYER));
    FindMotionWork(FindCharacterByScenarioName(work->dealer));
    RemoveConversationSpeakerAlias(work->dealer);
    EV_FreeAuthorPak(0, work->game.voicePrefix);
    EV_LoadCutsceneFromCurrentScene(work->bgm);
    EV_LoadAudio(work->sbt, work->soundScene);
    EV_EnableCharacterDisplay(work->dealer);
    EV_EnableCharacterDisplay(PATI_PLAYER);
    if (DoesScenarioCharacterExist(PATI_BACKPACK))
        EV_EnableCharacterDisplay(PATI_BACKPACK);
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    return true;
}
// @verified - p
void FreePatiEvent(PatiEventWork *work) {
    if (g_currentMap.name != PATI_CHALLENGE_MAP.name && !work->challengeComplete)
        SetScreenFilmFade(1, 255, 0, 0, 0, 0, 0, 0, 0);
    if (std::memcmp(work->argument, "PVSJ", 4)) {
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("PATI"), 0);
        if (work->dealer.name)
            EV_npcLockUnlock(work->dealer);
        EV_NPC_NewLoadSw(1);
        if (g_currentMap.name != PATI_CHALLENGE_MAP.name)
            SetPlayerControlEnabled(1);
    }
    EV_CleanupMapControlTask();
    HLib::MarkCurrentTaskForCleanup();
    HLib::DestroyCurrentTask();
    HLib::ResolveDeferredCallback(work->parent);
    EV_SetCharaControllerFlags(PATI_PLAYER, 2, static_cast<NpcCharacterControllerState>(1));
    EV_SetCharaControllerFlags(PATI_PLAYER, 3, static_cast<NpcCharacterControllerState>(1));
    if (g_currentMap.name == PATI_CHALLENGE_MAP.name)
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1233, 1);
    if (work->challengeComplete)
        EV_scenQueueLoad(g_actNumber, g_currentMap, 12);
}
} // namespace shenmue::s2
