#include "s2/debug.hpp"
#include "s2/tglb_event.hpp"
#include "s2/tglb_author.hpp"
#include "s2/tglb_effects.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/extra_telop.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/lighting.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/model_component.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/pak.hpp"
#include "s2/particle_effects.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/weather.hpp"
#include <cstring>

namespace shenmue::s2 {
namespace {
std::int32_t qteBattleState, qteDirection, qteAttempts, qteSuccesses, qteFailures, authorRetryActive, waitCounter;
Matrix4x4 qteAuthorTransform;
SoundEffectHandle *currentSound;
std::array<std::array<std::int32_t, 3>, 19> idleHandCommands = togyuIdleHandCommands;

void EVT_Togyu_Main_Callback(void *data);
void EVT_BattleEntry_Callback(void *data);
void EVT_BattlePreEvent_Callback(void *data);
void EVT_BattleStart_Callback(void *data);
void EVT_BattleStart2_Callback(void *data);
void EVT_BattleWait_Callback(void *data);
void EVT_BattleWait2_Callback(void *data);
void EVT_BattleLose_Callback(void *data);
void EVT_ButtiAuthor_Callback(void *data);
void EVT_ButtiAuthor2_Callback(void *data);
void EVT_ButtiAuthor3_Callback(void *data);
void EVT_QteBattle_Callback(void *data);
void EVT_QteBattle2_Callback(void *data);
void EVT_QteBattleSuccessPA_Callback(void *data);
void EVT_QteBattleSuccessPA2_Callback(void *data);
void EVT_QteBattleFailPA_Callback(void *data);
void EVT_QteBattleFailPAAuthWait_Callback(void *data);
void EVT_QteBattleFailPA1_Callback(void *data);
void EVT_SyueiAuthor_Callback(void *data);
void EVT_SyueiAuthor0_Callback(void *data);
void EVT_SyueiAuthor1_Callback(void *data);
void EVT_SyueiAuthor2_Callback(void *data);
void EVT_SyueiAuthor3_Callback(void *data);
void EVT_GaimonQte_Callback(void *data);
void EVT_WonAuthor_Callback(void *data);
void EVT_WonAuthor2_Callback(void *data);
void EVT_WonAuthor3_Callback(void *data);
void EVT_LoseAuthor_Callback(void *data);
void EVT_LoseAuthor2_Callback(void *data);
void EVT_VictoryAuthor_Callback(void *data);
void EVT_VictoryAuthor2_Callback(void *data);
void EVT_VictoryAuthor3_Callback(void *data);

// @verified - p
void TGLB_ArrangeSpectators() {
    for (std::size_t i = 0; i < togyuArrangementCharacters.size(); ++i)
        EV_AT_SetPos(togyuArrangementCharacters[i], EventTransformFlags::World | EventTransformFlags::AllAxes, togyuArrangementPositions[i]);
    for (std::size_t i = 0; i < togyuArrangementCharacters.size(); ++i)
        EV_AT_SetAngY(togyuArrangementCharacters[i], EventTransformFlags::World | EventTransformFlags::AllAxes, togyuArrangementRotations[i]);
    for (std::size_t i = 0; i < togyuArrangementMotionCharacters.size(); ++i)
        mtwkRequestMotion(scenFindMotionwork(togyuArrangementMotionCharacters[i]), togyuArrangementMotions[i], MotionRegion::LowerBody);
    for (shen_name name : togyuArrangementMotionCharacters)
        SetMotionRegionFlag(scenFindMotionwork(name), 71, 0, MotionRegion::LowerBody);
}

// @verified - p
void TGLB_ArrangeCharacters(TogyuEventWork *work) {
    const shen_name opponent = work->opponents[work->selectedOpponent];
    EV_DisableCharacterDisplay(TOGYU_SYE);
    EV_ToggleCharacterFlag(TOGYU_SYE);
    EV_DisableCharacterDisplay(TOGYU_SRY);
    EV_ToggleCharacterFlag(TOGYU_SRY);
    EV_EnableCharacterDisplay(opponent);
    EV_EnsureCharacterDraw(opponent);
    AT_SetLocalBaseChara(TOGYU_RYO, TOGYU_INVALID);
    AT_SetLocalBaseChara(TOGYU_NAME, TOGYU_INVALID);
    EV_AT_SetPos(TOGYU_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, togyuRyoPosition);
    EV_AT_SetPos(TOGYU_NAME, EventTransformFlags::World | EventTransformFlags::AllAxes, togyuOpponentPosition);
    EV_AT_SetAngY(TOGYU_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, togyuRyoRotation);
    EV_AT_SetAngY(TOGYU_NAME, EventTransformFlags::World | EventTransformFlags::AllAxes, togyuOpponentRotation);
    TGLB_ArrangeSpectators();
    SetScenarioCharacterType(opponent, CharacterTypeModelShadow);
}

// @verified - p
void TGLB_SetupCutscene(TogyuEventWork *work) {
    work->motionHandles[0] = 0;
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>(togyuPackage), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>(togyuPackage), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, TOGYU_BASE);
    EV_FreeAuthorPak(1, togyuAuthorBank);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene(togyuAudioBank);
    QueueExtraTelop(togyuSetupTelop, 99);
    qteBattleUtil(BattleQteMode::Load);
    vfmDispatchControl(FightControlOp::GaugeResources, shen_name{}, 0, 1);
    SetMapControlStatusFlag(0, 1);
    SetNewWindPhaseAndSpeed(32768, 0.25f);
    EV_EventSetUpHelpIcon_0(HELP_TOPIC_FREE_BATTLE, 0);
    shenSetClockAndCompassRenderTo(1);
    FinishLoading();
}

// @verified - p
void TGLB_Cleanup(TogyuEventWork *work) {

    (void)IsAuthorMultichannelAudioEnabled();
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    for (shen_name name : togyuHandCharacters)
        hndlSetFlags(FindCharacterByScenarioName(name));
    ET_AT_KillWork(work->characters);
    SetPlayerControlEnabled(0);
    SetCharaKill(TOGYU_BASE, 1);
    SetMotionRegionFlag(scenFindMotionwork(TOGYU_RYO), 71, 0, MotionRegion::LowerBody);
    if (work->smokeId != -1) {
        SuspendParticleEffect(0, work->smokeId);
        FreeParticleEffect(0, work->smokeId);
        work->smokeId = -1;
    }
    qteBattleUtil(BattleQteMode::FreeSprites);
    if (vfmDispatchControl(FightControlOp::GaugeResources, shen_name{}, 0, -1))
        vfmDispatchControl(FightControlOp::GaugeResources, shen_name{}, 0, 0);
    const std::uint32_t *motion = work->motionHandles.data();
    while (*motion) {
        DeleteMotion(static_cast<std::int32_t>(*motion));
        ++motion;
    }
    EV_FreeAuthorPak(0, togyuAuthorBank);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(togyuPackage), 0);
    LoadCutsceneAndSoundProgramForArea();
    shenSetClockAndCompassRenderTo(0);
    EV_EventSetUpHelpIcon_0(0, 0);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->task);
    EV_scenQueueLoad(g_actNumber, TOGYU_EXIT_SCENE, 2);
}

// @verified - p
void TGLB_init_func() {
    qteBattleState = 0;
    qteAttempts = 0;
    qteDirection = 0;
    qteSuccesses = 0;
    qteFailures = 0;
    qteAuthorTransform = {};
    waitCounter = 0;
    togyuBattleVoiceTask = nullptr;
    togyuMotionVoiceIndex = 0;
    togyuVictorySmokeId = 0;
    currentSound = nullptr;
    togyuSmokeTask = nullptr;
}

// @verified - p
void TGLB_main_func(std::uintptr_t) {
    if (EV_GetShenmueFlag(TOGYU_RETRY_FLAG))
        EV_stepSetFlags(0, TOGYU_RETRY_FLAG);
    SetPlayerControlEnabled(0);
    SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_Togyu_Main_Callback, 5, 0x0b, sizeof(TogyuEventWork), TOGYU_EVENT_TOKEN);
    TogyuEventWork *const work = static_cast<TogyuEventWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->phase = 0;
    work->nextState = 0;
    work->selectedOpponent = 0;
    work->opponents[0] = TOGYU_NAME;
    work->opponents[1] = TOGYU_INVALID;
    work->area = TGLB_AREA;
    const std::uint64_t initialMotionStorage = 96;
    std::memcpy(reinterpret_cast<std::byte *>(work) + offsetof(TogyuEventWork, initialMotionStorage), &initialMotionStorage, sizeof(initialMotionStorage));
    work->laterPhase = 0;
    work->savedNpcBatchCount = GetNpcBatchCountOverride(40);
    qteDirection = 0;
    work->task = task;
    work->smokeId = -1;
    togyuVictorySmokeId = -1;
    work->characters = Queue_EVT_AT_ReadChr(task, togyuCharacterNames.data());
    authorRetryActive = 0;
    TGLB_SetupCutscene(work);
}

// @verified - p
void EVT_Togyu_Main_Callback(void *data) {
    TogyuEventWork *const work = static_cast<TogyuEventWork *>(data);
    const shen_name opponent = work->opponents[work->selectedOpponent];
    switch (work->phase) {
    case 0:
        for (shen_name name : togyuHandCharacters)
            SetLowQualityHands(FindCharacterByScenarioName(name));
        (void)FindMotionWork(FindCharacterByScenarioName(TOGYU_RYO));
        (void)FindMotionWork(FindCharacterByScenarioName(TOGYU_NAME));
        for (shen_name name : togyuFighters) {
            SetHighModelXYZ(FindCharacterByScenarioName(name), 1, 1, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(name), 1, 0, 0);
            ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(name), 1, reinterpret_cast<std::byte *>(idleHandCommands.data()), 1);
            ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(name), 0, reinterpret_cast<std::byte *>(idleHandCommands.data()), 1);
        }
        ST_LoadTitleFade();
        currentSound = nullptr;
        work->phase = 1;
        break;
    case 1:
        EV_spmBeginSampleExecute();
        EV_LoadAudio(togyuAudioBank, 0);
        BeginSecondarySampleExecution(0);
        if (!currentSound) {
            currentSound = AllocateMaterialSoundHandle(TOGYU_MAIN_SOUND);
            if (currentSound)
                ExecuteMaterialSoundHandleCommand(currentSound, 0xa6, 127);
        }
        TGLB_ArrangeCharacters(work);
        work->phase = 2;
        if (togyuSmokeTask)
            HLib::DestroyTaskInternal(togyuSmokeTask);
        QueueTogyuSmokeControl(work);
        LoadBlurModelForCharacter(TOGYU_RYO, "RYO_B.MT7", 1);
        LoadBlurModelForCharacter(opponent, "TGY_B.MT7", 1);
        shenStartFGST(10, 10, 0, 0);
        vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>(togyuPackage), 1);
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{8}, 1);
        {
            const std::int32_t hp = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{TOGYU_HEALTH_NIBBLE}, 0, -1));
            shenInitFightData(FindCharacterByScenarioName(TOGYU_RYO), nullptr, 0, 0, static_cast<std::uint16_t>(100 * (hp + 3)), 0);
        }
        shenInitFightData(FindCharacterByScenarioName(opponent), nullptr, 1, 1, 800, 8);
        vfCopyPosFromEntity(0, nullptr, togyuFightRingCentre.x, togyuFightRingCentre.y, togyuFightRingRadius);
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x100}, 1);
        EV_vfBattleUtil(FightBattleUtilOp::CombatCameraFlags, shen_name{2}, 1);
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x1000}, 1);
        break;
    case 2:
        work->phase = 3;
        break;
    case 3:
        HLib::SetCurrentTaskTo(EVT_BattleEntry_Callback);
        work->phase = 4;
        break;
    case 4:
        work->phase = work->nextState == 3 ? 5 : 6;
        break;
    case 5:
        work->phase = 9;
        break;
    case 6:
        HLib::SetupTaskDelay(30);
        work->phase = 7;
        break;
    case 7:
        Queue_ST_LoadTitleFadeIn(TOGYU_RETRY_TITLE);
        HLib::SetupTaskDelay(60);
        work->phase = 8;
        break;
    case 8: {
        ST_LoadTitleFade();
        const std::uint64_t phaseAndResult = 1;
        std::memcpy(reinterpret_cast<std::byte *>(work) + offsetof(TogyuEventWork, phase), &phaseAndResult, sizeof(phaseAndResult));
        work->laterPhase = 0;
        break;
    }
    case 9:
        TGLB_Cleanup(work);
        break;
    default:
        break;
    }
}

// @verified - p
void EVT_BattleEntry_Callback(void *data) {
    TogyuEventWork *const work = static_cast<TogyuEventWork *>(data);
    const shen_name opponent = work->opponents[work->selectedOpponent];
    EventSetPosition position{0, TOGYU_RYO, TOGYU_INVALID, togyuRyoPosition, 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{0, TOGYU_RYO, TOGYU_INVALID, 270, 0};
    EVT_AT_SetAngY_Callback(&angle);
    position.character = opponent;
    position.position = togyuOpponentPosition;
    EVT_AT_SetPosRot(&position);
    angle.character = opponent;
    angle.degrees = 90;
    EVT_AT_SetAngY_Callback(&angle);
    EventSetMotion motion{0, TOGYU_RYO, TOGYU_ENTRY_MOTION, 0, 1, 0, 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(TOGYU_ENTRY_MOTION);
    motion.character = TOGYU_NAME;
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(TOGYU_ENTRY_MOTION);
    for (shen_name name : togyuSpectators)
        (void)FindMotionWork(FindCharacterByScenarioName(name));
    for (shen_name name : togyuSpectators) {
        SetHighModelXYZ(FindCharacterByScenarioName(name), 0, 0, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(name), 0, 1, 0);
    }
    for (shen_name name : togyuSpectators)
        SetClothFlag(FindCharacterByScenarioName(name), 4, 1);
    HLib::SetCurrentTaskTo(EVT_BattlePreEvent_Callback);
    HLib::SetupTaskDelay(1);
}

// @verified - p
void EVT_BattlePreEvent_Callback(void *) {
    HLib::Task *const film = HLib::EnqueueTaskWithParameter(EVT_AT_MenCtrl_Callback, 6, 0x0b, sizeof(EventFilmControl), TOGYU_EVENT_TOKEN);
    RegisterEventTask(film);
    EventFilmControl *const fade = static_cast<EventFilmControl *>(HLib::GetTaskParameter(film));
    *fade = EventFilmControl{1, 30, 255, 0, 0, 0, 0, 0, 0, 0, 5};
    EventSetMotion motion{0, TOGYU_RYO, TOGYU_RYO_FIGHT_MOTION, 0, 1, 0, 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(TOGYU_RYO_FIGHT_MOTION);
    motion.character = TOGYU_NAME;
    motion.animation = TOGYU_OPPONENT_FIGHT_MOTION;
    motion.flags = 0x20000000U;
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(TOGYU_OPPONENT_FIGHT_MOTION);
    EventSetCamera camera{0, TOGYU_BATTLE_CAMERA, TOGYU_INVALID, TOGYU_INVALID, 0, {}, 0, 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    HLib::SetCurrentTaskTo(EVT_BattleStart_Callback);
    HLib::SetupTaskDelay(60);
}

// @verified - p
void EVT_BattleStart_Callback(void *) {
    vfmQueueFightStart(2, 99, 1, 0, -1);
    togyuBattleVoiceTask = QueueTogyuBattleVoiceControl();
    HLib::SetCurrentTaskTo(EVT_BattleStart2_Callback);
    qteBattleState = 0;
    waitCounter = 0;
}
// @verified - p
void EVT_BattleStart2_Callback(void *) {
    EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{1}, 0);
    HLib::SetCurrentTaskTo(EVT_BattleWait_Callback);
}

// @verified - p
void EVT_BattleWait_Callback(void *data) {
    TogyuEventWork *const work = static_cast<TogyuEventWork *>(data);
    const shen_name opponent = work->opponents[work->selectedOpponent];
    if (IsFightActive()) {
        if (togyuBattleVoiceTask &&
            (!GetFightCharacterFlag(0, FindCharacterByScenarioName(opponent), 0) || !GetFightCharacterFlag(0, FindCharacterByScenarioName(TOGYU_RYO), 0) ||
                (GetFightTaskFlags() & 0x80000))) {
            HLib::DestroyTaskInternal(togyuBattleVoiceTask);
            BeginFightDialogueRetirement();
            togyuBattleVoiceTask = nullptr;
        }
        if (waitCounter > 20) {
            Vector3 position;
            EV_GetCharaPosition(TOGYU_RYO, 5, &position, EventTransformFlags::World);
            for (shen_name name : togyuLookCharacters)
                UpdateMotionWorkWithPosition(scenFindMotionwork(name), &position, 0);
            waitCounter = 0;
        }
        ++waitCounter;
    } else if (GetBtlStatus(FindCharacterByScenarioName(TOGYU_RYO)) == TOGYU_BATTLE_LOSS) {
        spmFadeOut();
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetCurrentTaskTo(EVT_BattleLose_Callback);
    } else {
        SetScreenFilmFade(15, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetCurrentTaskTo(EVT_BattleWait2_Callback);
    }
}

// @verified - p
void TGLB_ClearSpectatorLooks() {
    for (shen_name name : togyuLookCharacters)
        UpdateMotionWorkWithPosition(scenFindMotionwork(name), nullptr, 0);
    for (shen_name name : togyuLookCharacters)
        UpdateMotionWorkWithPosition(scenFindMotionwork(name), nullptr, 3);
}
// @verified - p
void EVT_BattleWait2_Callback(void *) {
    if (!filmCheckFadeTrackColour()) {
        TGLB_ClearSpectatorLooks();
        for (shen_name name : togyuSpectators)
            SetClothFlag(FindCharacterByScenarioName(name), 4, 0);
        HLib::SetCurrentTaskTo(EVT_ButtiAuthor_Callback);
    }
}
// @verified - p
void EVT_BattleLose_Callback(void *data) {
    TogyuEventWork *const work = static_cast<TogyuEventWork *>(data);
    std::int32_t value = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{TOGYU_QTE_DIFFICULTY_NIBBLE}, 0, -1));
    if (value > 0)
        --value;
    vfmDispatchControl(FightControlOp::MotionNibble, shen_name{TOGYU_QTE_DIFFICULTY_NIBBLE}, static_cast<std::uintptr_t>(value), 1);
    value = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{TOGYU_HEALTH_NIBBLE}, 0, -1));
    if (value < 15)
        ++value;
    vfmDispatchControl(FightControlOp::MotionNibble, shen_name{TOGYU_HEALTH_NIBBLE}, static_cast<std::uintptr_t>(value), 1);
    HLib::SetCurrentTaskTo(EVT_Togyu_Main_Callback);
    const std::uint32_t zero = 0;
    std::memcpy(work->reserved090.data(), &zero, sizeof(zero));
}

// @verified - p
void EVT_ButtiAuthor_Callback(void *) {
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    EV_authSceneCtrl(AuthSceneCtrl::SetCharaHQFlags);
    EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
    if (authorRetryActive > 0)
        EV_SetAuthorActiveTo(1);
    QueueTglbAuthorMain(TGLB_AUTHOR_AU_0);
    HLib::SetCurrentTaskTo(EVT_ButtiAuthor2_Callback);
}
// @verified - p
void EVT_ButtiAuthor2_Callback(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        HLib::SetCurrentTaskTo(EVT_ButtiAuthor3_Callback);
}
// @verified - p
void EVT_ButtiAuthor3_Callback(void *) {
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        qteAttempts = 0;
        qteDirection = 0;
        qteSuccesses = 0;
        qteFailures = 0;
        HLib::SetCurrentTaskTo(EVT_QteBattle2_Callback);
    }
}
// @verified - p
void EVT_OtherLooks_Callback(void *) {
    Vector3 position;
    EV_GetCharaPosition(TOGYU_RYO, 5, &position, EventTransformFlags::World);
    for (shen_name name : togyuLookCharacters)
        UpdateMotionWorkWithPosition(scenFindMotionwork(name), &position, 0);
    HLib::DestroyCurrentTask();
}

void QueueTogyuOtherLooks() {
    HLib::Task *const task = HLib::EnqueueTaskWithoutParameter(EVT_OtherLooks_Callback, 6, 0x0b, static_cast<std::int32_t>(TOGYU_EVENT_TOKEN.name));
    HLib::SetupTimedTask(task, 1);
}
void PublishTogyuAuthorTransform() {
    const std::uint16_t yaw = qteDirection ? 49152 : 16384;
    Matrix4x4_Push();
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::preTranslate(togyuAuthorPosition.x, togyuAuthorPosition.y, togyuAuthorPosition.z);
    Matrix4x4::RotateAroundY_Wrapper(yaw);
    qteAuthorTransform = workTransformMatrix;
    Author_SetPlacementMatrix(&qteAuthorTransform);
    Matrix4x4::Pop();
}

// @verified - p
void EVT_QteBattle_Callback(void *) {
    QueueTogyuOtherLooks();
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(TOGYU_AUTHOR_AU_3.name));
    PublishTogyuAuthorTransform();
    qteBattleState = 0;
    HLib::SetCurrentTaskTo(EVT_QteBattle2_Callback);
    HLib::SetupTaskDelay(45);
}
// @verified - p
void TGLB_IncreaseQteDifficulty() {
    const std::int32_t value = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{TOGYU_QTE_DIFFICULTY_NIBBLE}, 0, -1));
    if (value < 2)
        vfmDispatchControl(FightControlOp::MotionNibble, shen_name{TOGYU_QTE_DIFFICULTY_NIBBLE}, static_cast<std::uintptr_t>(value + 1), 1);
}

// @verified - p
void EVT_QteBattle2_Callback(void *data) {
    TogyuEventWork *const work = static_cast<TogyuEventWork *>(data);
    switch (qteBattleState) {
    case 0:
        EV_authSceneCtrl(AuthSceneCtrl::SetAuthorStopped);
        qteBattleState = 4;
        return;
    case 4: {
        std::int32_t frames = 0;
        if (qteSuccesses >= 0 && qteSuccesses <= 2) {
            const std::int32_t value =
                static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{TOGYU_QTE_DIFFICULTY_NIBBLE}, 0, -1));
            frames = togyuQteWaitFrames[qteSuccesses][value == 0 ? 0 : value == 1 ? 1 : 2];
        }
        qteBattleUtil(BattleQteMode::SetWait, 20, 6, frames);
        EV_debug_log("TGLB\\eTGLB_et_tgy_battle.c", 1075, "QteWait : %d\n", frames);
        switch (qteSuccesses) {
        case 0:
            qteBattleUtil(BattleQteMode::SetFirstCommands, 64, 2048, 0, 0);
            break;
        case 1:
            qteBattleUtil(BattleQteMode::SetFirstCommands, 64, 64, 2048, 0);
            break;
        case 2:
            qteBattleUtil(BattleQteMode::SetFirstCommands, 16, 64, 1, 2048);
            qteBattleUtil(BattleQteMode::SetLastCommands, 0, 0, 0, 0);
            break;
        default:
            break;
        }
        qteBattleUtil(BattleQteMode::ResetStates, 3);
        qteBattleUtil(BattleQteMode::Queue);
        qteBattleUtil(BattleQteMode::Start);
        qteBattleState = 5;
        return;
    }
    case 5: {
        const std::int64_t result = qteBattleUtil(BattleQteMode::GetResult);
        if (!result)
            break;
        qteBattleUtil(BattleQteMode::Retire);
        if (result == 1) {
            work->nextState = 3;
            ++qteSuccesses;
            if (qteSuccesses == 3) {
                work->nextState = 3;
                HLib::SetCurrentTaskTo(EVT_SyueiAuthor_Callback);
            } else
                HLib::SetCurrentTaskTo(EVT_QteBattleSuccessPA_Callback);
        } else {
            work->nextState = 2;
            ++qteFailures;
            if (qteFailures == 1) {
                TGLB_IncreaseQteDifficulty();
                work->nextState = 2;
            }
            HLib::SetCurrentTaskTo(EVT_QteBattleFailPA_Callback);
        }
        ++qteAttempts;
        break;
    }
    default:
        break;
    }
}

// @verified - p
void EVT_QteBattleSuccessPA_Callback(void *) {
    QueueTogyuOtherLooks();
    EV_authSceneCtrl(AuthSceneCtrl::RefreshAndEnqueue);
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(TOGYU_AUTHOR_AU_4.name));
    PublishTogyuAuthorTransform();
    const std::array<const char *, 4> &voices = qteAttempts == 1 ? togyuFirstSuccessVoices : togyuLaterSuccessVoices;
    const std::array<std::int32_t, 3> &delays = qteAttempts == 1 ? togyuFirstSuccessVoiceDelays : togyuLaterSuccessVoiceDelays;
    EV_PlayDialogue(voices[0]);
    for (std::size_t i = 0; i < delays.size(); ++i)
        EVT_AT_SetVoice(voices[i + 1], 1, delays[i]);
    HLib::SetCurrentTaskTo(EVT_QteBattleSuccessPA2_Callback);
    HLib::SetupTaskDelay(108);
}
// @verified - p
void EVT_QteBattleSuccessPA2_Callback(void *) {
    HLib::SetCurrentTaskTo(EVT_QteBattle_Callback);
    qteDirection = qteDirection == 0;
}
// @verified - p
void EVT_QteBattleFailPA_Callback(void *) {
    QueueTogyuOtherLooks();
    EV_authSceneCtrl(AuthSceneCtrl::RefreshAndEnqueue);
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    if (authorRetryActive > 0)
        EV_SetAuthorActiveTo(1);
    authorRetryActive = 1;
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(TOGYU_AUTHOR_AU_5.name));
    PublishTogyuAuthorTransform();
    TGLB_ClearSpectatorLooks();
    HLib::SetCurrentTaskTo(EVT_QteBattleFailPAAuthWait_Callback);
}
// @verified - p
void EVT_QteBattleFailPAAuthWait_Callback(void *) {
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        if (static_cast<std::uint32_t>(GetAuthorFlags()))
            SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetCurrentTaskTo(EVT_QteBattleFailPA1_Callback);
    }
}
// @verified - p
void EVT_QteBattleFailPA1_Callback(void *) {
    spmFadeOut();
    HLib::SetCurrentTaskTo(EVT_Togyu_Main_Callback);
    HLib::SetupTaskDelay(30);
}

// @verified - p
void EVT_SyueiAuthor_Callback(void *data) {
    TogyuEventWork *const work = static_cast<TogyuEventWork *>(data);
    if (!cvsHasValidConverseData(nullptr)) {
        TGLB_ClearSpectatorLooks();
        SuspendTogyuSmoke(work, 0);
        SetNewWindPhaseAndSpeed(32768, 0.05f);
        HLib::SetCurrentTaskTo(EVT_SyueiAuthor0_Callback);
    }
}
// @verified - p
void EVT_SyueiAuthor0_Callback(void *) {
    HLib::SetCurrentTaskTo(EVT_SyueiAuthor1_Callback);
}
// @verified - p
void EVT_SyueiAuthor1_Callback(void *) {
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    EV_EnableCharacterDisplay(TOGYU_SYE);
    EV_EnsureCharacterDraw(TOGYU_SYE);
    EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
    EV_authSceneCtrl(AuthSceneCtrl::SetCharaHQFlags);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(TGLB_AUTHOR_AU_8.name));
    HLib::SetCurrentTaskTo(EVT_SyueiAuthor2_Callback);
}
// @verified - p
void EVT_SyueiAuthor2_Callback(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        HLib::SetCurrentTaskTo(EVT_SyueiAuthor3_Callback);
}
// @verified - p
void EVT_SyueiAuthor3_Callback(void *data) {
    TogyuEventWork *const work = static_cast<TogyuEventWork *>(data);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    switch (frame) {
    case 242:
        EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>(togyuPackage), 0);
        LGHT_Parser_Internal(TOGYU_LIGHT.name);
        ExecuteMaterialSoundHandleCommand(currentSound, 0xa6, 0);
        break;
    case 510:
        ReinitializeLights(1);
        SetLightFlagForIndex(0, 2);
        SetLightEntryIndex(0, 1);
        UpdateTogyuSmoke(work);
        ExecuteMaterialSoundHandleCommand(currentSound, 0xa6, 127);
        break;
    case 990:
        SetNewWindPhaseAndSpeed(32768, 0.25f);
        qteBattleState = 0;
        HLib::SetCurrentTaskTo(EVT_GaimonQte_Callback);
        EV_DisableCharacterDisplay(TOGYU_SYE);
        EV_ToggleCharacterFlag(TOGYU_SYE);
        break;
    default:
        break;
    }
}

// @verified - p
std::int64_t GetTogyuGateQteResult() {
    switch (qteBattleState) {
    case 0:
        qteBattleState = 4;
        return 0;
    case 4: {
        std::int32_t value = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{TOGYU_QTE_DIFFICULTY_NIBBLE}, 0, -1));
        std::int32_t frames = togyuQteWaitFrames[0][value == 0 ? 0 : value == 1 ? 1 : 2];
        EV_debug_log("TGLB\\eTGLB_et_tgy_battle.c", 1650, "QteWait : %d\n", frames);
        value = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{TOGYU_QTE_DIFFICULTY_NIBBLE}, 0, -1));
        frames = togyuQteWaitFrames[0][value == 0 ? 0 : value == 1 ? 1 : 2];
        qteBattleUtil(BattleQteMode::SetWait, 20, 6, frames);
        qteBattleUtil(BattleQteMode::SetFirstCommands, 4, 1280);
        qteBattleUtil(BattleQteMode::ResetStates, 3);
        qteBattleUtil(BattleQteMode::Queue);
        qteBattleUtil(BattleQteMode::Start);
        qteBattleState = 5;
        break;
    }
    case 5: {
        const std::int64_t result = qteBattleUtil(BattleQteMode::GetResult);
        if (result)
            qteBattleUtil(BattleQteMode::Retire);
        return result;
    }
    default:
        break;
    }
    return 0;
}
// @verified - p
void EVT_GaimonQte_Callback(void *data) {
    TogyuEventWork *const work = static_cast<TogyuEventWork *>(data);
    const std::int64_t result = GetTogyuGateQteResult();
    if (!result)
        return;
    if (result == 1) {
        work->nextState = 3;
        HLib::SetCurrentTaskTo(EVT_WonAuthor_Callback);
    } else {
        TGLB_IncreaseQteDifficulty();
        work->nextState = 2;
        HLib::SetCurrentTaskTo(EVT_LoseAuthor_Callback);
    }
}
// @verified - p
void EVT_WonAuthor_Callback(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        EV_authSceneCtrl(AuthSceneCtrl::RefreshAndEnqueue);
    if (currentSound) {
        FreeMaterialSoundHandle(currentSound);
        currentSound = nullptr;
    }
    if (togyuVictorySmokeId == -1)
        togyuVictorySmokeId = CreateTogyuVictorySmoke();
    ConfigureTogyuVictorySmoke();
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    EV_authSceneCtrl(AuthSceneCtrl::SetCharaHQFlags);
    QueueTglbAuthorMain(TGLB_AUTHOR_AU_1);
    HLib::SetCurrentTaskTo(EVT_WonAuthor2_Callback);
}
// @verified - p
void EVT_WonAuthor2_Callback(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        HLib::SetCurrentTaskTo(EVT_WonAuthor3_Callback);
}
// @verified - p
void EVT_WonAuthor3_Callback(void *data) {
    TogyuEventWork *const work = static_cast<TogyuEventWork *>(data);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        if (togyuVictorySmokeId != -1) {
            SuspendParticleEffect(0, togyuVictorySmokeId);
            FreeParticleEffect(0, togyuVictorySmokeId);
            togyuVictorySmokeId = -1;
        }
        HLib::SetCurrentTaskTo(EVT_VictoryAuthor_Callback);
        return;
    }
    switch (static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame))) {
    case 1:
        for (shen_name name : togyuSpectators) {
            SetHighModelXYZ(FindCharacterByScenarioName(name), 0, 0, 0);
            SetHighModelXYZ(FindCharacterByScenarioName(name), 0, 1, 0);
        }
        break;
    case 220:
        SuspendTogyuSmoke(work, 0);
        ConfigureTogyuVictorySmoke();
        ResumeParticleEffect(0, togyuVictorySmokeId);
        SetParticleEffectBirthRate(0, togyuVictorySmokeId, 0.1f);
        break;
    case 232:
    case 470:
        SuspendParticleEffect(0, togyuVictorySmokeId);
        break;
    case 350:
        ConfigureTogyuVictorySmoke();
        ResumeParticleEffect(0, togyuVictorySmokeId);
        break;
    case 361:
        SetParticleEffectVelocity(0, togyuVictorySmokeId, victorySmokeRiseVelocity, victorySmokeVerticalVariance);
        SetParticleEffectVelocity(0, togyuVictorySmokeId, victorySmokeSlowVelocity, victorySmokeVerticalVariance);
        SetParticleEffectBirthRate(0, togyuVictorySmokeId, 0.1f);
        break;
    case 500:
        UpdateTogyuSmoke(work);
        break;
    default:
        break;
    }
}
// @verified - p
void EVT_LoseAuthor_Callback(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        EV_authSceneCtrl(AuthSceneCtrl::RefreshAndEnqueue);
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    EV_authSceneCtrl(AuthSceneCtrl::SetCharaHQFlags);
    QueueTglbAuthorMain(TGLB_AUTHOR_AU_2);
    HLib::SetCurrentTaskTo(EVT_LoseAuthor2_Callback);
}
// @verified - p
void EVT_LoseAuthor2_Callback(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        SetScenarioCharacterType(TOGYU_NAME, CharacterTypeModelShadow);
        HLib::SetCurrentTaskTo(EVT_VictoryAuthor3_Callback);
    }
}
// @verified - p
void EVT_VictoryAuthor3_Callback(void *) {
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        HLib::SetCurrentTaskTo(EVT_Togyu_Main_Callback);
}
// @verified - p
void EVT_VictoryAuthor_Callback(void *data) {
    TogyuEventWork *const work = static_cast<TogyuEventWork *>(data);
    ResetAuthorAudioState();

    (void)IsAuthorMultichannelAudioEnabled();
    (void)IsAuthorMultichannelAudioEnabled();
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        EV_authSceneCtrl(AuthSceneCtrl::RefreshAndEnqueue);
    togyuVictorySmokeId = -1;
    SuspendTogyuSmoke(work, 0);
    EV_spmBeginSampleExecute();
    (void)IsAuthorMultichannelAudioEnabled();
    EV_LoadAudio(togyuAudioBank, 1);
    EV_EnableCharacterDisplay(TOGYU_SRY);
    EV_EnsureCharacterDraw(TOGYU_SRY);
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    EV_authSceneCtrl(AuthSceneCtrl::SetCharaHQFlags);
    QueueTglbAuthorMain(TGLB_AUTHOR_AU_7);
    HLib::SetCurrentTaskTo(EVT_VictoryAuthor2_Callback);
}
// @verified - p
void EVT_VictoryAuthor2_Callback(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        HLib::SetCurrentTaskTo(EVT_VictoryAuthor3_Callback);
}
} // namespace

const ScriptAreaDefinition tglbScriptDefinition{.areaId = TGLB_AREA, .field04 = 0, .init = TGLB_init_func, .main = TGLB_main_func};

} // namespace shenmue::s2
