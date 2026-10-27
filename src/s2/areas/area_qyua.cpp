#include "s2/areas/area_qyua.hpp"
#include "s2/step_event.hpp"
#include "s2/map_control.hpp"
#include "s2/author.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/character.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/load_title.hpp"
#include "s2/model_component.hpp"
#include "s2/npc_package.hpp"
#include "s2/osage.hpp"
#include "s2/pak.hpp"
#include "s2/qte_hand_poses.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/sound_program.hpp"
namespace shenmue::s2 {
DateTime qyuaStartDate{};
// @verified - p
void QyuaInit() {
}
// @verified - p
void QyuaMain(std::uintptr_t argument) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(QyuaStartCallback, 4, 0x0b, sizeof(QyuaEventWork), QYUA_EVENT_TASK);
    QyuaEventWork *const work = static_cast<QyuaEventWork *>(HLib::GetTaskParameter(task));
    work->arguments = argument ? reinterpret_cast<const void *>(argument) : QYUA_DEFAULT_ARGUMENTS;
    work->self = task;
    work->reserved08 = 0;
    InitializeQyuaResources(work);
}
// @verified - p
void InitializeQyuaResources(QyuaEventWork *work) {
    (void)EV_NPC_NewLoadSw(0);
    (void)EV_ResetFreeTalkFlags();
    GetCurrentDateTime(qyuaStartDate);
    EVT_SetQteRetryFrames(24);
    EVT_SetQteFailureCount(0);
    work->characters = Queue_EVT_AT_ReadChr(work->self, QYUA_CHARACTERS);
    work->authorHasPlayed = 0;
    (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("0217"));
    (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("QYUA"));
    (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    (void)EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QYUA"), 0);
    (void)EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QYUA"), 0);
    (void)EV_FreeAuthorPak(1, "0217");
    (void)EV_SetNewSceneIndex();
    (void)EV_LoadCutsceneFromCurrentScene("0217A000");
    SetRenderClockAndCompassTo(1);
    QyuaInit();
    (void)EV_SetObjectFlagByIndex(1, 0);
    (void)EV_SetObjectFlagByIndex(3, 0);
}
// @verified - p
void QyuaStartCallback(void *parameter) {
    QyuaEventWork *const work = static_cast<QyuaEventWork *>(parameter);
    work->nextCallback = QyuaWaitOpeningAuthorCallback;
    work->succeeded = 0;
    LoadBlurModelForCharacter(QYUA_REN, "REN_B.MT7", 1.0F);
    (void)SetLowQualityHands(FindCharacterByScenarioName(QYUA_PLAYER));
    (void)SetLowQualityHands(FindCharacterByScenarioName(QYUA_REN));
    (void)SetLowQualityHands(FindCharacterByScenarioName(QYUA_YUAN));
    (void)SetLowQualityHands(FindCharacterByScenarioName(QYUA_TOGYU));
    EventCling knife{0, QYUA_KNIFE, QYUA_YUAN, 14, QYUA_KNIFE_POSITION, QYUA_KNIFE_ROTATION, 0, 0};
    EVT_AT_Cling(&knife);
    (void)EV_cuffsAttach(QYUA_PLAYER, 1, QYUA_REN, 0, 0.30000001192092896F);
    AT_SetLocalBaseChara(QYUA_DUMMY_RYO, QYUA_PLAYER);
    AT_SetLocalBaseChara(QYUA_DUMMY_REN, QYUA_REN);
    AT_SetLocalBaseChara(QYUA_DUMMY_YUAN, QYUA_YUAN);
    AT_SetLocalBaseChara(QYUA_DUMMY_TOGYU, QYUA_TOGYU);
    EVT_AT_SetPos(QYUA_BASE, QYUA_INVALID, QYUA_BASE_POSITION, 0);
    EVT_AT_SetAngY(QYUA_BASE, QYUA_INVALID, 0, 0);
    EVT_AT_SetPos(QYUA_DUMMY_RYO, QYUA_PLAYER, {}, 0);
    EVT_AT_SetAngY(QYUA_DUMMY_RYO, QYUA_PLAYER, 0, 0);
    EVT_AT_SetPos(QYUA_DUMMY_REN, QYUA_REN, {}, 0);
    EVT_AT_SetAngY(QYUA_DUMMY_REN, QYUA_REN, 0, 0);
    EVT_AT_SetPos(QYUA_DUMMY_YUAN, QYUA_YUAN, {}, 0);
    EVT_AT_SetAngY(QYUA_DUMMY_YUAN, QYUA_YUAN, 0, 0);
    EVT_AT_SetPos(QYUA_DUMMY_TOGYU, QYUA_TOGYU, {}, 0);
    EVT_AT_SetAngY(QYUA_DUMMY_TOGYU, QYUA_TOGYU, 0, 0);
    EVT_AT_SetPos(QYUA_TOGYU, QYUA_BASE, {-1000.0F, 0, 0}, 0);
    EVT_AT_SetAngY(QYUA_TOGYU, QYUA_BASE, 0, 0);
    (void)SetPluralControlFor(FindCharacterByScenarioName(QYUA_PLAYER), 0);
    EV_EnableCharacterDisplay(QYUA_PLAYER);
    EV_EnableCharacterDisplay(QYUA_REN);
    EV_EnableCharacterDisplay(QYUA_YUAN);
    EV_EnableCharacterDisplay(QYUA_AUTHOR_KNIFE);
    EV_DisableCharacterDisplay(QYUA_KNIFE);
    (void)EV_LoadAudio("0217A000", 1);
    (void)EV_spmBeginSampleExecute();
    (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(QYUA_AUTHOR_START.name));
    if (work->authorHasPlayed == 1)
        EV_SetAuthorActiveTo(1);
    HLib::SetCurrentTaskTo(QyuaWaitOpeningAuthorCallback);
}
// @verified - p
void QyuaWaitOpeningAuthorCallback(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    EV_DisableCharacterDisplay(QYUA_AUTHOR_KNIFE);
    EV_EnableCharacterDisplay(QYUA_KNIFE);
    (void)SetHighModelXYZ(FindCharacterByScenarioName(QYUA_PLAYER), 1, 0, 0);
    (void)SetHighModelXYZ(FindCharacterByScenarioName(QYUA_PLAYER), 1, 1, 0);
    (void)ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(QYUA_PLAYER), 0, reinterpret_cast<std::byte *>(handPoseReplacementCommand), 0);
    (void)ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(QYUA_PLAYER), 1, reinterpret_cast<std::byte *>(handPoseReplacementCommand), 0);
    (void)SetHighModelXYZ(FindCharacterByScenarioName(QYUA_REN), 1, 0, 0);
    (void)SetHighModelXYZ(FindCharacterByScenarioName(QYUA_REN), 1, 1, 0);
    (void)ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(QYUA_REN), 0, reinterpret_cast<std::byte *>(handPoseReplacementCommand), 0);
    (void)ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(QYUA_REN), 1, reinterpret_cast<std::byte *>(handPoseReplacementCommand), 0);
    (void)SetHighModelXYZ(FindCharacterByScenarioName(QYUA_YUAN), 1, 0, 0);
    (void)SetHighModelXYZ(FindCharacterByScenarioName(QYUA_YUAN), 1, 1, 0);
    (void)ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(QYUA_YUAN), 0, reinterpret_cast<std::byte *>(yuanKnifeHandPose), 0);
    (void)ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(QYUA_YUAN), 1, reinterpret_cast<std::byte *>(crateGripHandPose), 0);
    const std::int32_t frames = BeginQyuaKnifeQte();
    EVT_AT_QTE(0x10, 0x10, 0, frames - 29);
    HLib::SetCurrentTaskTo(QyuaFirstResultCallback);
    HLib::SetupTaskDelay(frames);
}
// @verified - p
std::int32_t BeginQyuaKnifeQte() {
    EVT_AT_SetVoice("0217C002", 1, 35);
    EVT_AT_SetPos(QYUA_PLAYER, QYUA_BASE, {}, 0);
    EVT_AT_SetAngY(QYUA_PLAYER, QYUA_BASE, 0, 0);
    EVT_AT_SetPos(QYUA_REN, QYUA_PLAYER, {-0.15029F, 0, 0.38437F}, 0);
    EVT_AT_SetAngY(QYUA_REN, QYUA_PLAYER, 0, 0);
    EVT_AT_SetPos(QYUA_YUAN, QYUA_PLAYER, {-1.97384F, 0, 0.02722F}, 0);
    EVT_AT_SetAngY(QYUA_YUAN, QYUA_PLAYER, 0, 0);
    SetClothFlag(FindCharacterByScenarioName(QYUA_PLAYER), 16, 1);
    toggleOsagFlags(FindCharacterByScenarioName(QYUA_REN));
    EVT_AT_SetCam(7800, QYUA_DUMMY_RYO, QYUA_DUMMY_RYO, 0, 0, 0);
    (void)EVT_AT_SetMot(QYUA_PLAYER, 0x2565, 0, 1, 0x40000000U, 0);
    (void)EVT_AT_SetMot(QYUA_REN, 0x2545, 0, 1, 0x40000000U, 0);
    (void)EVT_AT_SetMot(QYUA_YUAN, 0x2590, 0, 1, 0x40000000U, 0);
    EVT_AT_SetFaceLooks(QYUA_PLAYER, FacialExpressionId::Angry, 1, 1, 0);
    EVT_AT_SetFaceLooks(QYUA_REN, FacialExpressionId::Angry, 1, 1, 0);
    EVT_AT_SetFaceLooks(QYUA_YUAN, FacialExpressionId::Scheme, 1, 1, 0);
    return 60;
}
// @verified - p
void QyuaFirstResultCallback(void *) {
    std::int32_t frames;
    if ((EV_GetCurrentQteStatus() & 0xfff) == 0x10) {
        frames = BeginQyuaSecondKnifeQte();
        EVT_AT_QTE(4, 16, 0, frames - 29);
        HLib::SetCurrentTaskTo(QyuaSecondResultCallback);
    } else {
        frames = PlayQyuaFirstKnifeFailure();
        HLib::SetCurrentTaskTo(QyuaFailureAuthorCallback);
    }
    HLib::SetupTaskDelay(frames);
}
// @verified - p
std::int32_t BeginQyuaSecondKnifeQte() {
    EVT_AT_SetVoice("0217C003", 1, 50);
    EVT_AT_SetPos(QYUA_PLAYER, QYUA_BASE, {}, 0);
    EVT_AT_SetAngY(QYUA_PLAYER, QYUA_BASE, 0, 0);
    EVT_AT_SetPos(QYUA_REN, QYUA_PLAYER, {-0.15029F, 0, 0.38437F}, 0);
    EVT_AT_SetAngY(QYUA_REN, QYUA_PLAYER, 0, 0);
    EVT_AT_SetPos(QYUA_YUAN, QYUA_PLAYER, {-1.97384F, 0, 0.02722F}, 0);
    EVT_AT_SetAngY(QYUA_YUAN, QYUA_PLAYER, 0, 0);
    EVT_AT_SetCam(7801, QYUA_DUMMY_RYO, QYUA_DUMMY_RYO, 0, 0, 0);
    EVT_AT_SetCam(7804, QYUA_BASE, QYUA_BASE, 0, 0, 60);
    (void)EVT_AT_SetMot(QYUA_PLAYER, 0x2565, 60, 1, 0x44000000U, 0);
    (void)EVT_AT_SetMot(QYUA_REN, 0x2545, 60, 1, 0x44000000U, 0);
    (void)EVT_AT_SetMot(QYUA_YUAN, 0x2590, 60, 1, 0x44000000U, 0);
    return 70;
}
// @verified - p
std::int32_t PlayQyuaFirstKnifeFailure() {
    EVT_AT_SetPos(QYUA_PLAYER, QYUA_BASE, {}, 0);
    EVT_AT_SetAngY(QYUA_PLAYER, QYUA_BASE, 0, 0);
    EVT_AT_SetPos(QYUA_REN, QYUA_PLAYER, {-0.17183F, 0, 0.42309001F}, 0);
    EVT_AT_SetAngY(QYUA_REN, QYUA_PLAYER, 0, 0);
    EVT_AT_SetPos(QYUA_YUAN, QYUA_PLAYER, {-1.77588F, 0, -0.12269F}, 0);
    EVT_AT_SetAngY(QYUA_YUAN, QYUA_PLAYER, 0, 0);
    EVT_AT_SetCam(7802, QYUA_DUMMY_YUAN, QYUA_DUMMY_YUAN, 0, 0, 0);
    (void)EVT_AT_SetMot(QYUA_PLAYER, 0x2562, 36, 1, 0x44000000U, 0);
    (void)EVT_AT_SetMot(QYUA_REN, 0x2542, 36, 1, 0x44000000U, 0);
    (void)EVT_AT_SetMot(QYUA_YUAN, 0x258d, 36, 1, 0x44000000U, 0);
    return 60;
}
// @verified - p
void QyuaSecondResultCallback(void *) {
    std::int32_t frames;
    if ((EV_GetCurrentQteStatus() & 0xfff) == 4) {
        frames = BeginQyuaThirdKnifeQte();
        EVT_AT_QTE(256, 16, 0, frames - 29);
        HLib::SetCurrentTaskTo(QyuaThirdResultCallback);
    } else {
        frames = PlayQyuaSecondKnifeFailure();
        HLib::SetCurrentTaskTo(QyuaFailureAuthorCallback);
    }
    HLib::SetupTaskDelay(frames);
}
// @verified - p
std::int32_t BeginQyuaThirdKnifeQte() {
    EVT_AT_SetVoice("0217C004", 1, 25);
    EVT_AT_SetVoice("0217C007", 1, 80); // @text EN: " " | JA: "甘いわね。"
    EVT_AT_SetPos(QYUA_PLAYER, QYUA_BASE, {}, 0);
    EVT_AT_SetAngY(QYUA_PLAYER, QYUA_BASE, 0, 0);
    EVT_AT_SetPos(QYUA_REN, QYUA_PLAYER, {-0.15029F, 0, 0.38437F}, 0);
    EVT_AT_SetAngY(QYUA_REN, QYUA_PLAYER, 0, 0);
    EVT_AT_SetPos(QYUA_YUAN, QYUA_PLAYER, {-1.97384F, 0, 0.02722F}, 0);
    EVT_AT_SetAngY(QYUA_YUAN, QYUA_PLAYER, 0, 0);
    EVT_AT_SetCam(7805, QYUA_BASE, QYUA_BASE, 0, 0, 0);
    EVT_AT_SetCam(7807, QYUA_BASE, QYUA_BASE, 0, 0, 20);
    EVT_AT_SetCam(7808, QYUA_DUMMY_YUAN, QYUA_DUMMY_YUAN, 0, 0, 60);
    EVT_AT_SetCam(7817, QYUA_DUMMY_YUAN, QYUA_DUMMY_YUAN, 0, 0, 92);
    (void)EVT_AT_SetMot(QYUA_PLAYER, 0x2565, 130, 1, 0x44000000U, 0);
    (void)EVT_AT_SetMot(QYUA_REN, 0x2545, 130, 1, 0x44000000U, 0);
    (void)EVT_AT_SetMot(QYUA_YUAN, 0x2590, 130, 1, 0x44000000U, 0);
    EVT_AT_DispBlur(QYUA_REN, 7, 12, 1, 25, 10);
    return 115;
}
// @verified - p
std::int32_t PlayQyuaSecondKnifeFailure() {
    EVT_AT_SetPos(QYUA_PLAYER, QYUA_BASE, {}, 0);
    EVT_AT_SetAngY(QYUA_PLAYER, QYUA_BASE, 0, 0);
    EVT_AT_SetPos(QYUA_REN, QYUA_PLAYER, {-0.079530001F, 0, 0.42789F}, 0);
    EVT_AT_SetAngY(QYUA_REN, QYUA_PLAYER, 0, 0);
    EVT_AT_SetPos(QYUA_YUAN, QYUA_PLAYER, {-2.0088601F, 0, 0.31832999F}, 0);
    EVT_AT_SetAngY(QYUA_YUAN, QYUA_PLAYER, 0, 0);
    EVT_AT_SetCam(7809, QYUA_DUMMY_RYO, QYUA_DUMMY_RYO, 0, 0, 0);
    (void)EVT_AT_SetMot(QYUA_PLAYER, 0x2563, 112, 1, 0x44000000U, 0);
    (void)EVT_AT_SetMot(QYUA_REN, 0x2543, 112, 1, 0x44000000U, 0);
    (void)EVT_AT_SetMot(QYUA_YUAN, 0x258e, 112, 1, 0x44000000U, 0);
    return 45;
}
// @verified - p
void QyuaThirdResultCallback(void *) {
    std::int32_t frames;
    if ((EV_GetCurrentQteStatus() & 0xfff) == 0x100) {
        frames = PlayQyuaKnifeSuccess();
        HLib::SetCurrentTaskTo(QyuaSuccessAuthorCallback);
    } else {
        frames = PlayQyuaFirstKnifeFailure();
        HLib::SetCurrentTaskTo(QyuaFailureAuthorCallback);
    }
    HLib::SetupTaskDelay(frames);
}
// @verified - p
std::int32_t PlayQyuaKnifeSuccess() {
    EVT_AT_SetVoice("0217A002", 1, 5);
    EVT_AT_SetVoice("0217B002", 1, 40); // @text EN: " " | JA: "おりゃっ！"
    EVT_AT_SetVoice("0217C006", 1, 70);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 0x0b, sizeof(StepObjectDisplayWork), QYUA_EVENT_TASK);
    (void)RegisterEventTask(task);
    *static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(task)) = {1, QYUA_KNIFE, 1, 20.0F};
    (void)EVT_AT_SetSound(1954, 30);
    EVT_AT_SetPos(QYUA_PLAYER, QYUA_BASE, {}, 0);
    EVT_AT_SetAngY(QYUA_PLAYER, QYUA_BASE, 0, 0);
    EVT_AT_SetPos(QYUA_PLAYER, QYUA_BASE, {}, 15);
    EVT_AT_SetAngY(QYUA_PLAYER, QYUA_BASE, 0, 15);
    EVT_AT_SetPos(QYUA_REN, QYUA_PLAYER, {-0.15029F, 0, 0.38437F}, 0);
    EVT_AT_SetAngY(QYUA_REN, QYUA_PLAYER, 0, 0);
    EVT_AT_SetPos(QYUA_REN, QYUA_PLAYER, {-0.15029F, 0, 0.38437F}, 15);
    EVT_AT_SetAngY(QYUA_REN, QYUA_PLAYER, 0, 15);
    EVT_AT_SetPos(QYUA_YUAN, QYUA_PLAYER, {-1.97384F, 0, 0.02722F}, 0);
    EVT_AT_SetAngY(QYUA_YUAN, QYUA_PLAYER, 0, 0);
    EVT_AT_SetPos(QYUA_YUAN, QYUA_PLAYER, {-1.97384F, 0, 0.02722F}, 15);
    EVT_AT_SetAngY(QYUA_YUAN, QYUA_PLAYER, 0, 15);
    EVT_AT_SetCam(7816, QYUA_DUMMY_RYO, QYUA_DUMMY_RYO, 0, 0, 0);
    EVT_AT_SetCam(7811, QYUA_DUMMY_RYO, QYUA_DUMMY_RYO, 0, 0, 15);
    EVT_AT_SetCam(7812, QYUA_DUMMY_REN, QYUA_DUMMY_REN, 0, 0, 40);
    EVT_AT_SetCam(7813, QYUA_DUMMY_RYO, QYUA_DUMMY_RYO, 0, 0, 95);
    (void)EVT_AT_SetMot(QYUA_PLAYER, 0x2565, 245, 0.1F, 0x44000000U, 0);
    (void)EVT_AT_SetMot(QYUA_PLAYER, 0x2565, 245, 1, 0x44000000U, 15);
    (void)EVT_AT_SetMot(QYUA_REN, 0x2545, 245, 0.1F, 0x44000000U, 0);
    (void)EVT_AT_SetMot(QYUA_REN, 0x2545, 245, 1, 0x44000000U, 15);
    (void)EVT_AT_SetMot(QYUA_YUAN, 0x2590, 245, 0.1F, 0x44000000U, 0);
    (void)EVT_AT_SetMot(QYUA_YUAN, 0x2590, 245, 1, 0x44000000U, 15);
    EVT_AT_DispBlur(QYUA_PLAYER, 6, 12, 1, 8, 15);
    EVT_AT_DispBlur(QYUA_REN, 7, 12, 1, 18, 38);
    return 135;
}
// @verified - p
void QyuaFailureAuthorCallback(void *parameter) {
    QyuaEventWork *const work = static_cast<QyuaEventWork *>(parameter);
    EV_EnableCharacterDisplay(QYUA_YUAN);
    EV_DisableCharacterDisplay(QYUA_KNIFE);
    EV_EnableCharacterDisplay(QYUA_AUTHOR_KNIFE);
    (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(QYUA_AUTHOR_FAILURE.name));
    if (work->authorHasPlayed == 1)
        EV_SetAuthorActiveTo(1);
    work->authorHasPlayed = 1;
    work->nextCallback = QyuaRetryOrFinishCallback;
    work->succeeded = 0;
    HLib::SetCurrentTaskTo(QyuaWaitResultAuthorCallback);
}
// @verified - p
void QyuaSuccessAuthorCallback(void *parameter) {
    QyuaEventWork *const work = static_cast<QyuaEventWork *>(parameter);
    EV_EnableCharacterDisplay(QYUA_YUAN);
    EV_DisableCharacterDisplay(QYUA_KNIFE);
    EV_DisableCharacterDisplay(QYUA_AUTHOR_KNIFE);
    (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(QYUA_AUTHOR_SUCCESS.name));
    work->succeeded = 1;
    work->nextCallback = QyuaRetryOrFinishCallback;
    HLib::SetCurrentTaskTo(QyuaWaitResultAuthorCallback);
}
// @verified - p
void QyuaWaitResultAuthorCallback(void *parameter) {
    QyuaEventWork *const work = static_cast<QyuaEventWork *>(parameter);
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 1)
        (void)SetPluralControlFor(FindCharacterByScenarioName(QYUA_PLAYER), 1);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        spmFadeOut();
        HLib::SetCurrentTaskTo(work->nextCallback);
        HLib::SetupTaskDelay(60);
    }
}
void QyuaRetryOrFinishCallback(void *parameter) {
    QyuaEventWork *const work = static_cast<QyuaEventWork *>(parameter);
    if (!work->succeeded || g_currentMap.name == AREA_QYUA.name) {
        const std::uint32_t failures = EVT_GetQteFailureCount();
        if (failures < 2)
            EVT_SetQteFailureCount(failures + 1);
        SetCurrentDateTimeTo(qyuaStartDate);
        (void)shenSetTimeSimulationTo(1);
        HLib::SetCurrentTaskTo(QyuaStartCallback);
    } else
        FinishQyuaEvent(work);
}
void FinishQyuaEvent(QyuaEventWork *work) {
    RestoreQyuaResources(work);
    EV_CleanupMapControlTask();
    HLKillOtherHierarchyTask(work->self);
    EV_stepSetFlags(0, 0x13c);
    if (g_currentMap.name == AREA_QYUA.name) {
        scenSetTaskFlags(6);
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
    }
}
// @verified - p
void RestoreQyuaResources(QyuaEventWork *work) {
    EV_cuffsSetFlags();
    EventCling knife{0, QYUA_KNIFE, shen_name{}, 0, {}, {}, 1, 0};
    EVT_AT_Cling(&knife);
    (void)SetPluralControlFor(FindCharacterByScenarioName(QYUA_PLAYER), 0);
    (void)hndlSetFlags(FindCharacterByScenarioName(QYUA_TOGYU));
    (void)hndlSetFlags(FindCharacterByScenarioName(QYUA_YUAN));
    (void)hndlSetFlags(FindCharacterByScenarioName(QYUA_REN));
    (void)hndlSetFlags(FindCharacterByScenarioName(QYUA_PLAYER));
    FinishLoading();
    SetRenderClockAndCompassTo(0);
    (void)LoadCutsceneAndSoundProgramForArea();
    (void)EV_spmBeginSampleExecute();
    (void)EV_FreeAuthorPak(0, "0217");
    (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QYUA"), 0);
    (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0217"), 0);
    ET_AT_KillWork(work->characters);
    SetPlayerControlEnabled(0);
    (void)EV_NPC_NewLoadSw(1);
}
const ScriptAreaDefinition qyuaScriptDefinition{.areaId = AREA_QYUA, .field04 = 0, .init = QyuaInit, .main = QyuaMain};
} // namespace shenmue::s2
