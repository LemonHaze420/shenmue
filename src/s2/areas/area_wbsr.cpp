#include "s2/areas/area_wbsr.hpp"
#include "s2/author.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"

namespace shenmue::s2 {
namespace {
constexpr shen_name ryo{SHEN_NAME(RYO_)}, sai{SHEN_NAME(SAI_)}, siz{SHEN_NAME(SIZ_)}, a048{SHEN_NAME(a048)};
constexpr shen_name characters[]{sai, SHEN_NAME(KNK_), siz, shen_name{}};
constexpr std::int32_t taskName = 1970500979;
constexpr std::uint32_t localXYZ = 0x38000000U;
struct WbsrState {
    std::int32_t phase, reserved04, reserved08, counter, frames;
    std::byte reserved14[0x3c];
    HLib::Task *task;
    std::uint64_t reserved58;
    std::int32_t timer;
    std::uint32_t reserved64;
};
static_assert(sizeof(WbsrState) == 0x68 && offsetof(WbsrState, task) == 0x50);
WbsrState state{};
std::int32_t entry[4]{}, baseEntry = 2;
void (*nextCallback)() = nullptr;
void (*choiceCallback)(std::int32_t) = nullptr;
std::uint32_t authorFlags{};
shen_name sceneArea{};
std::int32_t sceneEntry{}, choice0{}, choice1{}, defaultChoice{};
const char *package{};
const shen_name *characterList{};
HLib::Task *Queue(HLib::TaskCallback callback) {
    return HLib::EnqueueTaskWithoutParameter(callback, 4, 0x0b, taskName);
}
MotionWork *RyoMotion() {
    return FindMotionWork(FindCharacterByScenarioName(ryo));
}
void ExecuteBase() {
    (void)ExecuteCurrAreaMainFunc_Wrapper(1, reinterpret_cast<std::uintptr_t>(&baseEntry));
}
void ClearGameFlag(std::int32_t flag) {
    (void)SetPlayerGameFlags(static_cast<PlayerGameFlagMode>(flag), nullptr);
}
void Fade(std::int32_t frames, std::int32_t start, std::int32_t target) {
    SetScreenFilmFade(frames, start, 0, 0, 0, target, 0, 0, 0);
}
void SetAuthor(std::int32_t index) {
    if (IsAuthorStateFlagSet(index))
        EV_SetAuthorActiveTo(1);
}
void StartAuthor(shen_name author, void (*callback)(), std::uint32_t flags, bool allowSkip) {
    nextCallback = callback;
    authorFlags = flags;
    (void)EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    (void)EV_authSceneCtrl(static_cast<AuthSceneCtrl>(author.name));
    (void)Queue(WbsrAuthorWaitCallback);
    if (allowSkip)
        (void)EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
}
void ResetCounters(std::int32_t phase) {
    state.phase = phase;
    state.counter = state.frames = 0;
}
void QueueScenarioA();
void StartTomorrow();
void ExhaustedAuthorDone() {
    (void)SetAuthorStateFlag(31, 1);
    StartTomorrow();
}
void RefusedAuthorDone() {
    (void)SetAuthorStateFlag(32, 1);
    StartTomorrow();
}
void Choose(std::int32_t choice) {
    if (choice == 1) {
        EV_stepSetFlags(0, 0x46);
        ExecuteBase();
        SetRenderClockAndCompassTo(0);
        (void)shenSetTimeSimulationTo(1);
        SetAuthor(30);
        StartAuthor(SHEN_NAME(AUT2), QueueScenarioA, 0, true);
    } else if (!choice) {
        ClearGameFlag(32);
        SetPlayerControlEnabled(1);
        InitiateCleanupForSceneChara(sai);
        camCtrlSetFovTransitionTo(true);
        (void)camCtrlSetState(CameraState::Init);
        EV_stepSetFlags(2, 0x46);
        ExecuteBase();
        mtwkRequestMotion(RyoMotion(), 0xf002, MotionRegion::LowerBody);
        mtwkSetExtraFlag(RyoMotion(), 17, 0);
        SetMotionRegionFlag(RyoMotion(), 71, 0, MotionRegion::LowerBody);
        (void)EV_AT_SetAngY(ryo, localXYZ, {0, 0x4000, 0});
        EV_AT_SetPos(ryo, localXYZ, {336.013f, 126.97f, 269.26001f});
        EV_AT_SetPos(a048, localXYZ, {334.519989f, 128.177001f, 269.390015f});
        (void)Queue(WbsrSelect1Callback);
    }
}
void OpeningAuthorDone() {
    (void)SetAuthorStateFlag(29, 1);
    (void)SetScenarioCharacterFlag40(SHEN_NAME(dr2R), 1);
    (void)SetScenarioCharacterFlag40(SHEN_NAME(dr2L), 1);
    SetRenderClockAndCompassTo(1);
    (void)shenSetTimeSimulationTo(0);
    std::int16_t choices[]{-1, 333, 334, -1};
    (void)SetupControlSpeakAndUI(choices, 1800, 0, 2);
    choice0 = 333;
    choice1 = 334;
    defaultChoice = 0;
    choiceCallback = Choose;
    (void)Queue(WbsrChoiceWaitCallback);
}
void PositionRyo(const Vector3 &position) {
    SetMotionRegionFlag(RyoMotion(), 71, 0, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(RyoMotion(), 0);
    SetMotionFrameTime(RyoMotion(), 0, MotionRegion::LowerBody);
    EV_AT_SetPos(ryo, localXYZ, position);
    (void)EV_AT_SetAngY(ryo, localXYZ, {0, 0x8000, 0});
}
void PackageReady() {
    (void)EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("SUS1"), 0);
    (void)EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("SUS1"), 0);
    EV_SetCharacterAlphaEnabled(sai, false);
    EV_SetCharaControllerFlags(sai, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    if (entry[0] == 1) {
        camCtrlSetFovTransitionTo(true);
        (void)camCtrlSetState(CameraState::Init);
        state.timer = 450;
        PositionRyo({336.45001f, 126.97f, 263.59f});
        SetPlayerControlEnabled(1);
        Fade(30, 255, 0);
        (void)Queue(WbsrScenarioBCallback);
        ResetCounters(100);
    } else if (entry[0] == 2) {
        EV_stepSetFlags(1, 0x37c);
        ExecuteBase();
        (void)Queue(WbsrScenarioBCallback);
        ResetCounters(200);
    } else {
        EV_DisableCharacterDisplay(siz);
        (void)EV_LoadCutsceneFromCurrentScene("0031E000");
        (void)SetScenarioCharacterFlag40(SHEN_NAME(dr2R), 0);
        (void)SetScenarioCharacterFlag40(SHEN_NAME(dr2L), 0);
        nextCallback = OpeningAuthorDone;
        authorFlags = 1;
        Fade(30, 255, 0);
        StartAuthor(SHEN_NAME(AUT0), OpeningAuthorDone, 1, false);
    }
}
void QueueScenarioA() {
    (void)SetAuthorStateFlag(30, 1);
    EV_DisableCharacterDisplay(sai);
    EV_stepSetFlags(0, 0x46);
    ExecuteBase();
    EVT_AT_SetAngY(ryo, shen_name{0xffffffffU}, 0, 0);
    SetMotionFrameTime(RyoMotion(), 12, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(RyoMotion(), 1);
    mtwkSetExtraFlag(RyoMotion(), 15, 0);
    mtwkRequestMotion(RyoMotion(), 0xf002, MotionRegion::LowerBody);
    (void)Queue(WbsrScenarioACallback);
    ResetCounters(4);
}
void StartTomorrow() {
    EV_stepSetFlags(1, 0x46);
    ExecuteBase();
    camCtrlSetFovTransitionTo(true);
    (void)camCtrlSetState(CameraState::Init);
    mtwkSetExtraFlag(RyoMotion(), 17, 0);
    SetMotionRegionFlag(RyoMotion(), 71, 0, MotionRegion::LowerBody);
    EV_AT_SetPos(ryo, localXYZ, {337.67001f, 126.97f, 280.07001f});
    (void)EV_AT_SetAngY(ryo, localXYZ, {0, 10922, 0});
    Fade(60, 255, 0);
    (void)EV_FreeAuthorPak(1, "0401");
    (void)EV_PlayDialogue("0401A001");
    SetPlayerControlEnabled(0);
    SetMotionFrameTime(RyoMotion(), 0, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(RyoMotion(), 0);
    mtwkRequestMotion(RyoMotion(), 0xf002, MotionRegion::LowerBody);
    HLib::SetupTimedTask(Queue(WbsrExit2Callback), 30);
}
} // namespace
// @verified - p
void WbsrInit() {
    state = {};
    baseEntry = 2;
    choiceCallback = nullptr;
    nextCallback = nullptr;
    authorFlags = 0;
    sceneArea = {};
    sceneEntry = 0;
    package = nullptr;
    characterList = nullptr;
    choice0 = choice1 = defaultChoice = 0;
    for (auto &value : entry)
        value = 0;
}
// @verified - p
void WbsrMain(std::uintptr_t argument) {
    EV_SetCanDoAreaJumpTo(0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1006, 1);
    entry[0] = *reinterpret_cast<const std::int32_t *>(argument);
    SetPlayerControlEnabled(0);
    Fade(0, 255, 255);
    FinishLoading();
    state.task = Queue(WbsrInitializeCallback);
    (void)EV_FreeAuthorPak(1, "0031");
    HLib::SetupTimedTask(state.task, 2);
}
// @verified - p
void WbsrInitializeCallback(void *) {
    Fade(1, 255, 255);
    (void)EV_NPC_NewLoadSw(0);
    (void)EV_CheckWarmStartNpcRoute(sai);
    (void)EV_ResetFreeTalkFlags();
    nextCallback = PackageReady;
    package = "SUS1";
    characterList = characters;
    (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>(package));
    for (const auto *p = characterList; p->name; ++p) {
        (void)EV_CheckWarmStartNpcRoute(*p);
        (void)EV_ForceLoadNpc(*p);
    }
    (void)Queue(WbsrPackageWaitCallback);
    HLib::DestroyCurrentTask();
}
// @verified - p
void WbsrPackageWaitCallback(void *) {
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoadPending, 0, 0))
        return;
    if (characterList)
        for (const auto *p = characterList; p->name; ++p)
            if (!EV_ForceLoadNpc(*p))
                return;
    nextCallback();
    HLib::DestroyCurrentTask();
}
// @verified - p
void WbsrAuthorWaitCallback(void *) {
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        nextCallback();
        HLib::DestroyCurrentTask();
    }
    const auto frame = EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining) - 30;
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == frame && (authorFlags & 2))
        Fade(30, 0, 255);
}
// @verified - p
void WbsrChoiceWaitCallback(void *) {
    const auto result = shenGetCtrlSpeakIconFlag();
    if (!GetShouldRenderIcons(RenderIconSlot::ActionSelector)) {
        choiceCallback(result == static_cast<std::uint32_t>(choice0) ? 0 : result == static_cast<std::uint32_t>(choice1) ? 1 : defaultChoice);
        HLib::DestroyCurrentTask();
    }
}
// @verified - p
void WbsrScenarioBCallback(void *) {
    ++state.frames;
    if (state.phase == 100) {
        if (state.timer-- < 0 && ryoNotBusy()) {
            SetPlayerControlEnabled(0);
            EV_stepSetFlags(1, 0x37c);
            ExecuteBase();
            ResetCounters(200);
            SetMotionSpeed(RyoMotion(), 0, MotionRegion::LowerBody);
            ClearGameFlag(31);
            Fade(60, 0, 255);
            HLib::SetupTaskDelay(60);
        }
    } else if (state.phase == 200) {
        ClearGameFlag(30);
        EV_EnableCharacterDisplay(sai);
        SetAuthor(31);
        nextCallback = ExhaustedAuthorDone;
        authorFlags = 1;
        Fade(30, 255, 0);
        StartAuthor(SHEN_NAME(AUTB), ExhaustedAuthorDone, 1, true);
        ResetCounters(999);
    }
}
// @verified - p
void WbsrScenarioACallback(void *) {
    ++state.frames;
    if (state.phase == 4) {
        switch (state.counter) {
        case 0:
            EVT_AT_SetAngY(ryo, shen_name{0xffffffffU}, 0, 0);
            ++state.counter;
            break;
        case 1:
            camCtrlSetFovTransitionTo(false);
            (void)camCtrlSetState(CameraState::Init);
            SetPlayerControlEnabled(1);
            state.timer = 1800;
            ++state.counter;
            break;
        case 2: {
            if (state.timer-- < 0 && ryoNotBusy()) {
                SetPlayerControlEnabled(0);
                ResetCounters(21);
            }
            EV_set_debug_text_pos(2, 8);
            Vector3 position{};
            Vector3i rotation{};
            EV_GetCharaPosition(ryo, 0xffffffffU, &position, EventTransformFlags::Local);
            GetScenarioCharacterEulerAngles(ryo, &rotation, EventTransformFlags::Local);
            break;
        }
        default:
            break;
        }
    } else if (state.phase == 19) {
        SetMotionFrameTime(RyoMotion(), 12, MotionRegion::LowerBody);
        mtwkSetRequestedRootMode(RyoMotion(), 1);
        mtwkSetExtraFlag(RyoMotion(), 15, 0);
        mtwkRequestMotion(RyoMotion(), 0xf005, MotionRegion::LowerBody);
        EV_SetCharaControllerFlags(ryo, ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(ryo, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_AT_SetPos(ryo, localXYZ, {336.51999f, 127, 262.32999f});
        (void)EV_AT_SetAngY(ryo, localXYZ, {15, 15, 15});
        authorFlags = 2;
        sceneArea = SHEN_NAME(WB01);
        sceneEntry = 1;
        auto *task = Queue(WbsrSceneChangeCallback);
        if (authorFlags & 2) {
            Fade(30, 0, 255);
            HLib::SetupTimedTask(task, 30);
        }
        HLib::DestroyCurrentTask();
    } else if (state.phase == 21) {
        if (!state.counter) {
            Fade(60, 0, 255);
            HLib::SetupTaskDelay(60);
            ClearGameFlag(31);
            ++state.counter;
        } else if (state.counter == 1) {
            ClearGameFlag(30);
            EV_EnableCharacterDisplay(sai);
            SetAuthor(31);
            nextCallback = ExhaustedAuthorDone;
            authorFlags = 1;
            Fade(30, 255, 0);
            StartAuthor(SHEN_NAME(AUTB), ExhaustedAuthorDone, 1, true);
            EV_stepSetFlags(0, 0x2bb);
            ExecuteBase();
            ResetCounters(999);
        }
    }
}
// @verified - p
void WbsrSelect1Callback(void *) {
    if (EV_GetShenmueFlag(0x2ba)) {
        SetPlayerControlEnabled(0);
        ClearGameFlag(30);
        SetRenderClockAndCompassTo(0);
        (void)shenSetTimeSimulationTo(1);
        EV_EnableCharacterDisplay(a048);
        SetAuthor(30);
        StartAuthor(SHEN_NAME(AUT2), QueueScenarioA, 0, true);
        HLib::DestroyCurrentTask();
    }
    if (EV_GetShenmueFlag(0x2bb)) {
        SetPlayerControlEnabled(0);
        ClearGameFlag(30);
        EV_stepSetFlags(0, 0x2bb);
        ExecuteBase();
        SetRenderClockAndCompassTo(0);
        (void)shenSetTimeSimulationTo(1);
        mtwkSetExtraFlag(RyoMotion(), 55, 0);
        mtwkSetExtraFlag(RyoMotion(), 17, 0);
        HLib::SetCurrentTaskTo(WbsrSelect2Callback);
        HLib::SetupTaskDelay(15);
    }
}
// @verified - p
void WbsrSelect2Callback(void *) {
    EV_EnableCharacterDisplay(a048);
    SetAuthor(32);
    StartAuthor(SHEN_NAME(AUT1), RefusedAuthorDone, 0, true);
    HLib::DestroyCurrentTask();
}
// @verified - p
void WbsrExit2Callback(void *) {
    SetPlayerControlEnabled(0);
    ClearGameFlag(32);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1006, 0);
    plymAdvanceTime(23, 0);
    HLib::SetCurrentTaskTo(WbsrExit3Callback);
}
// @verified - p
void WbsrExit3Callback(void *) {
    const auto held = plymGetAreaExitState();
    if (!held)
        HLib::SetCurrentTaskTo(WbsrExit4Callback);
    else if (held == 1) {
        (void)EV_PlayDialogue("0401A002");
        HLib::SetCurrentTaskTo(WbsrExit4Callback);
        HLib::SetupTaskDelay(40);
    }
}
// @verified - p
void WbsrExit4Callback(void *) {
    ClearGameFlag(30);
    SetPlayerControlEnabled(1);
    (void)EV_FreeAuthorPak(0, "0031");
    if (package)
        (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(package), 0);
    if (characterList)
        for (const auto *p = characterList; p->name; ++p)
            (void)EV_npcLockUnlock(*p);
    EV_SetCanDoAreaJumpTo(1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1006, 0);
    (void)EV_NPC_NewLoadSw(1);
    HLib::DestroyCurrentTask();
}
// @verified - p
void WbsrSceneChangeCallback(void *) {
    if (!(authorFlags & 4))
        (void)scenSetTaskFlags(6);
    (void)EV_scenQueueLoad(g_actNumber, sceneArea, sceneEntry);
    HLib::DestroyCurrentTask();
}
const ScriptAreaDefinition wbsrScriptDefinition{.areaId = AREA_WBSR, .field04 = 0, .init = WbsrInit, .main = WbsrMain};
} // namespace shenmue::s2
