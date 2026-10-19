#include "s2/areas/area_4bil.hpp"
#include "s2/pause.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2::areas {
namespace {
constexpr shen_name ryo{SHEN_NAME(RYO_)}, ren{SHEN_NAME(REN_)}, base{SHEN_NAME(BASE)};
constexpr shen_name characters[]{ren, shen_name{0xffffffffU}};
constexpr Vector3 renRouteOrigin{320.5f, 100.0f, 446.5f};
constexpr std::uint32_t runState = make_shen_name('R', 'U', 'N', ' ');
constexpr Vector3 renRouteTarget{325.79998779296875f, 101.5999984741211f, 458.100006103515625f};
constexpr Vector3 trigger{327.0f, 100.0f, 459.0f};
constexpr Vector3 basePosition{318.0f, 100.0f, 444.79998779296875f};
constexpr Vector3 renStart{282.0f, 100.0f, 475.0f};
constexpr Vector3i renRotation{0, 32768, 0};



struct RenRouteStorage {
    NpcRouteNode node{offsetof(RenRouteStorage, points), 14, offsetof(RenRouteStorage, lengths), {}, {}, offsetof(RenRouteStorage, laneBounds),
        offsetof(RenRouteStorage, auxiliary), SHEN_NAME(REN3), 0};
    Vector3 points[14]{{282.688995361328125f, 0, 477.486724853515625f}, {283.3759765625f, 0, 481.99114990234375f},
        {284.062957763671875f, 0, 486.495574951171875f}, {289.635284423828125f, 0, 487.029998779296875f}, {302.077606201171875f, 0, 487.106353759765625f},
        {310.474273681640625f, 0, 484.892303466796875f}, {313.069610595703125f, 0, 476.723236083984375f}, {316.42828369140625f, 0, 467.867095947265625f},
        {323.06927490234375f, 0, 463.66802978515625f}, {327.649261474609375f, 0, 459.39263916015625f}, {327.878265380859375f, 0, 451.758026123046875f},
        {327.725616455078125f, 0, 446.642822265625f}, {322.992950439453125f, 0, 446.2611083984375f}, {318.260284423828125f, 0, 445.87939453125f}};
    float lengths[12]{75.618194580078125f, 4.16205120086669921875f, 5.9547328948974609375f, 10.46894741058349609375f, 8.61643695831298828125f,
        8.11046886444091796875f, 8.72388172149658203125f, 7.5309619903564453125f, 6.376817226409912109375f, 6.653209209442138671875f, 4.859306812286376953125f,
        4.161384105682373046875f};
    float laneBounds[22]{2.7f, 3.0f, 1.8f, 2.4f, 2.2f, 2.5f, 0.8f, 1.6f, 1.4f, 3.0f, 3.0f, 1.5f, 3.0f, 1.3f, 0.8f, 1.7f, 0.2f, 0.4f, 0.3f, 0.4f, 0.8f, 1.1f};
    std::int32_t auxiliary[3]{-1, -1, -1};
};
RenRouteStorage renRoute;
std::int32_t routeSelection{}, triggerDelay{};
std::uint32_t state{}, savedState{};
std::int32_t authorDelay{};
bool authorWaitRequested{};
using Stage = std::int32_t (*)();
Stage nextStage{};
std::int32_t StartGreeting(), ContinueGreeting(), FinishConversation(), StartAmbush(), StartLateGreeting(), StartFenceRun(), FinishFenceJump();

MotionWork *MotionFor(shen_name character) {
    return FindMotionWork(FindCharacterByScenarioName(character));
}
void Camera(std::int32_t index, std::int32_t delay = 0) {
    EVT_AT_SetCam(index, base, base, 0.0f, 0, delay);
    if (!delay)
        camCtrlDisableFilmBars();
}
void BeginAuthorWait(Stage following) {
    authorWaitRequested = true;
    nextStage = following;
}

void HideRen() {
    mtwkSetRequestedRootMode(MotionFor(ren), 0);
    EV_AT_SetPos(ren, EventTransformFlags::World | EventTransformFlags::AllAxes, {320.0f, 100.0f, 445.0f});
    EV_DisableCharacterDisplay(ren);
}
void LockPlayer() {
    SetPlayerControlEnabled(0);
    shenToggleAreaFlags(2);
    EV_EnableCharacterDisplay(ryo);
    EV_EnsureCharacterDraw(ryo);
    EV_SetCharaControllerFlags(ryo, 2, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(ryo, 3, NpcCharacterControllerState::Disabled);
    EV_SetRefBoardParamsFor(ryo, 1);
    EV_SetCharacterAlphaEnabled(ryo, false);
    mtwkSetRequestedRootMode(MotionFor(ryo), 0);
}
void StartRenRoute() {
    AT_SetLocalBaseChara(ren, shen_name{0xffffffffU});
    mtwkSetRequestedRootMode(MotionFor(ren), 0);
    EV_EnableCharacterDisplay(ren);
    EV_AT_SetPos(ren, EventTransformFlags::AllAxes, renStart);
    EV_AT_SetAngY(ren, EventTransformFlags::AllAxes, renRotation);
    mtwkRequestMotion(MotionFor(ren), 0xf006, MotionRegion::LowerBody);
    BindNpcScriptRoute(ren, &renRoute.node, &renRouteOrigin, 0x4000);
    if (NpcRouteStateContext *context = FindNpcRouteStateContext(ren))
        SetNpcRouteMotionState(*context, 1, false);
    SetNpcScriptRouteFlags(nullptr, ren, 0, 2);
    SetNpcScriptRouteFlags(nullptr, ren, 0, 4);
}
bool CheckRunTrigger() {
    Vector3 position;
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    if (position.x <= 326.0f || position.x >= 329.0f || position.z >= 458.0f || position.z <= 445.0f)
        return false;
    nextStage = StartFenceRun;
    return true;
}
bool CheckWaitTrigger() {
    if (triggerDelay > 0)
        --triggerDelay;
    if (!ryoNotBusy())
        return false;
    Vector3 position;
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    if (routeSelection) {
        if (EV_GetShenmueFlag(0x379) && position.x > 309.0f && position.x < 350.0f && position.z > 458.0f && position.z < 478.0f) {
            nextStage = StartGreeting;
            return true;
        }
        return false;
    }
    if (triggerDelay > 0)
        return CheckRunTrigger();
    if (triggerDelay == 0) {
        if (position.x > 326.0f && position.x < 329.0f && position.z < 455.0f && position.z > 445.0f) {
            nextStage = StartLateGreeting;
            return true;
        }
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1006, 0);
        return false;
    }
    if (!ComputeAngle(trigger, 3.0f, 0, 0x8000, position))
        return false;
    nextStage = StartAmbush;
    return true;
}
void Setup4Bil() {
    SetPlayerControlEnabled(0);
    nullsub();
    CreateEventCharacter(CharacterCreationHighModel, nullptr, base);
    EV_AT_SetPos(base, EventTransformFlags::World | EventTransformFlags::AllAxes, basePosition);
    EV_AT_SetAngY(base, EventTransformFlags::AllAxes, Vector3i{});
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("4BIL"), 0);
    EV_FreeAuthorPak(1, "0063");
    routeSelection = 0;
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 311)) {
        state = SHEN_NAME(STRT);
        nextStage = StartGreeting;
        triggerDelay = -1;
    } else if (EV_GetShenmueFlag(0x375) || EV_GetShenmueFlag(0x377)) {
        routeSelection = 1;
        state = SHEN_NAME(WAI0);
        triggerDelay = -1;
    } else if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1425)) {
        state = SHEN_NAME(WAI0);
        triggerDelay = -1;
    } else {
        state = SHEN_NAME(INIT);
        triggerDelay = 120;
    }
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1425, 1);
}
void Update4Bil(void *) {
    authorWaitRequested = false;
    switch (state) {
    case SHEN_NAME(WAIT):
        if (CheckWaitTrigger()) {
            LockPlayer();
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            state = SHEN_NAME(WFAD);
        }
        break;
    case SHEN_NAME(INIT):
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1006, 1);
        SetPlayerControlEnabled(1);
        FinishLoading();
        EV_NPC_NewLoadSw(1);
        StartRenRoute();
        EV_mtwkSetEnabled(MotionFor(ren), 1);
        mtwkSetExtraFlag(MotionFor(ren), 0, 0);
        state = runState;
        break;
    case SHEN_NAME(EXIT):
        if (!filmCheckFadeTrackColour())
            EV_scenQueueLoad(g_actNumber, SHEN_NAME(CB00), 0);
        break;
    case SHEN_NAME(STRT): {
        nullsub();
        EV_EnableCharacterDisplay(ryo);
        EV_EnableCharacterDisplay(ren);
        EV_SetCharaControllerFlags(ryo, 2, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(ryo, 3, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(ren, 2, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(ren, 3, NpcCharacterControllerState::Disabled);
        shenToggleAreaFlags(0);
        mtwkSetRequestedRootMode(MotionFor(ren), 0);
        EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("4BIL"), 0);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1006, 0);
        const std::int32_t delay = nextStage();
        if (authorWaitRequested) {
            savedState = state;
            state = SHEN_NAME(WTPA);
            authorDelay = delay;
        } else
            HLib::SetupTaskDelay(delay);
        break;
    }
    case SHEN_NAME(FADE):
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        state = SHEN_NAME(EXIT);
        break;
    case runState:
        if (CheckRunTrigger()) {
            LockPlayer();
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            state = SHEN_NAME(WFAD);
        }
        if (!IsNpcRouteActive(ren)) {
            ReleaseNpcRouteControl(nullptr, ren, -1);
            mtwkSetRequestedRootMode(MotionFor(ren), 0);
            state = SHEN_NAME(WAIT);
        }
        break;
    case SHEN_NAME(WAI0):
        SetPlayerControlEnabled(1);
        FinishLoading();
        HideRen();
        EV_NPC_NewLoadSw(1);
        state = SHEN_NAME(WAIT);
        break;
    case SHEN_NAME(WTPA):
        if (authGetFinished()) {
            EV_CleanupMapControlTask();
            SetScreenFilmFade(10, 0, 0, 0, 0, 255, 0, 0, 0);
            spmFadeOut();
            state = SHEN_NAME(EXIT);
        } else if (authorDelay-- <= 0)
            state = savedState;
        break;
    case SHEN_NAME(WFAD):
        if (!filmCheckFadeTrackColour()) {
            ReleaseNpcRouteControl(nullptr, ren, -1);
            mtwkSetRequestedRootMode(MotionFor(ren), 0);
            EV_EnableCharacterDisplay(ren);
            EV_NPC_NewLoadSw(0);
            EV_ResetFreeTalkFlags();
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
            state = SHEN_NAME(STRT);
        }
        break;
    default:
        break;
    }
}
std::int32_t StartGreeting() {
    AT_SetLocalBaseChara(ryo, base);
    AT_SetLocalBaseChara(ren, base);
    mtwkSetRequestedRootMode(MotionFor(ryo), 1);
    EVT_AT_SetPos(ryo, base, {9.0f, 0.0f, 20.6000003814697265625f}, 0);
    EVT_AT_SetAngY(ryo, base, 0, 0);
    EVT_AT_SetMot(ryo, 0xf005, 0, 1, 0, 0);
    EVT_AT_SetMot(ryo, 0xf003, 0, 1, 0, 120);
    Camera(1);
    EVT_AT_SetVoice("0063A012", 1, 150); // @text EN: "Up ahead is Lucky Plaza." | JA: "この先が、ラッキープラザか…"
    BeginAuthorWait(ContinueGreeting);
    return 150;
}
std::int32_t ContinueGreeting() {
    mtwkSetRequestedRootMode(MotionFor(ryo), 0);
    EVT_AT_SetPos(ryo, base, {9.0f, 0.0f, 16.1000003814697265625f}, 0);
    EVT_AT_SetAngY(ryo, base, 0, 0);
    EVT_AT_SetPos(ren, base, {6.5f, 0, 2}, 0);
    EVT_AT_SetAngY(ren, base, 270, 0);
    UpdateMotionWorkWithPosition(MotionFor(ryo), &renRouteTarget, 0);
    EVT_AT_SetMot(ryo, 0xf003, 0, 1, 0, 0);
    EVT_AT_SetMot(ren, 0x11c2, 75, 1, 0, 0);
    EVT_AT_SetAngY(ryo, base, 45, 75);
    EVT_AT_SetPos(ren, base, {8.3999996185302734375f, 0, 2}, 75);
    EVT_AT_SetAngY(ren, base, 270, 75);
    EVT_AT_SetMot(ryo, 0x11c3, 265, 1, 0x40000000U, 75);
    EVT_AT_SetMot(ren, 0x11c2, 125, 1, 0, 75);
    EVT_AT_SetPoint(ryo, ren, 5, 0, 0, 75, 115, 0x80000000U);
    EVT_AT_SetVoice("0063A013", 1, 75);
    EVT_AT_SetVoice("0063A014", 1, 90); // @text EN: "Ren!" | JA: "…レン！！"
    EVT_AT_SetVoice("0063B013", 1, 110); // @text EN: "Damn!" | JA: "やべっ…"
    Camera(2);
    Camera(3, 75);
    BeginAuthorWait(FinishConversation);
    return 115;
}
std::int32_t StartAmbush() {
    AT_SetLocalBaseChara(ryo, base);
    AT_SetLocalBaseChara(ren, base);
    mtwkSetRequestedRootMode(MotionFor(ryo), 0);
    EVT_AT_SetPos(ryo, base, {9, 0, 17}, 0);
    EVT_AT_SetAngY(ryo, base, 90, 0);
    EVT_AT_SetPos(ren, base, {6, 0, 2}, 0);
    EVT_AT_SetAngY(ren, base, 270, 0);
    EVT_AT_SetMot(ryo, 0x11c5, 51, 1, 0, 0);
    EVT_AT_SetMot(ren, 0x11c2, 0, 1, 0, 130);
    Camera(10);
    BeginAuthorWait(ContinueGreeting);
    return 200;
}
std::int32_t StartLateGreeting() {
    AT_SetLocalBaseChara(ryo, base);
    AT_SetLocalBaseChara(ren, base);
    mtwkSetRequestedRootMode(MotionFor(ryo), 0);
    EVT_AT_SetPos(ryo, base, {9.5f, 0, 12}, 0);
    EVT_AT_SetAngY(ryo, base, 45, 0);
    EVT_AT_SetPos(ren, base, {6, 0, 2}, 0);
    EVT_AT_SetAngY(ren, base, 270, 0);
    EVT_AT_SetMot(ren, 0x11c2, 40, 1, 0, 0);
    EVT_AT_SetMot(ryo, 0x11c3, 257, 1, 0, 0);
    EVT_AT_SetVoice("0063A013", 1, 0);
    EVT_AT_SetVoice("0063A014", 1, 20); // @text EN: "Ren!" | JA: "…レン！！"
    EVT_AT_SetVoice("0063B013", 1, 40); // @text EN: "Damn!" | JA: "やべっ…"
    BeginAuthorWait(FinishConversation);
    Camera(21);
    Camera(22, 20);
    return 70;
}
std::int32_t FinishConversation() {
    mtwkSetRequestedRootMode(MotionFor(ryo), 0);
    EVT_AT_SetPos(ryo, base, {9.5f, 0, 18.5f}, 0);
    EVT_AT_SetAngY(ryo, base, 0, 0);
    EVT_AT_SetPos(ren, base, {9.30000019073486328125f, 0, 2}, 0);
    EVT_AT_SetAngY(ren, base, 270, 0);
    EVT_AT_SetMot(ryo, 0x11c6, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(ren, 0x11c2, 165, 1, 0, 0);
    EVT_AT_SetVoice("0063A015", 1, 35); // @text EN: "Wait!!" | JA: "待て！！"
    Camera(4);
    BeginAuthorWait(StartFenceRun);
    return 50;
}
std::int32_t StartFenceRun() {
    mtwkSetRequestedRootMode(MotionFor(ryo), 0);
    hmdlSetFlagsFor(FindCharacterByScenarioName(ren), 0, 3, 16);
    hmdlSetFlagsFor(FindCharacterByScenarioName(ren), 1, 3, 16);
    EVT_AT_SetPos(ryo, base, {9.5f, 0, 2.5f}, 0);
    EVT_AT_SetAngY(ryo, base, 80, 0);
    EVT_AT_SetPos(ren, base, {2, 0.100000001490116119384765625f, 0.60000002384185791015625f}, 0);
    EVT_AT_SetAngY(ren, base, 0, 0);
    EVT_AT_SetMot(ryo, 0x11c6, 0, 0.800000011920928955078125f, 0, 0);
    EVT_AT_SetMot(ren, 0x11c1, 145, 1, 0, 0);
    EVT_AT_SetVoice("0063B014", 1, 0); // @text EN: " " | JA: "おらよっと…"
    Camera(30);
    BeginAuthorWait(FinishFenceJump);
    return 30;
}
std::int32_t FinishFenceJump() {
    EVT_AT_SetPos(ryo, base, {2, 0, 0.75f}, 0);
    EVT_AT_SetAngY(ryo, base, 0, 0);
    EV_DisableCharacterDisplay(ren);
    mtwkSetRequestedRootMode(MotionFor(ryo), 0);
    hmdlSetFlagsFor(FindCharacterByScenarioName(ryo), 0, 3, 16);
    hmdlSetFlagsFor(FindCharacterByScenarioName(ryo), 1, 3, 16);
    EVT_AT_SetMot(ryo, 0x11c4, 105, 1.5f, 0, 0);
    EVT_AT_SetVoice("0063A016", 1, 0);
    Camera(40);
    BeginAuthorWait(nullptr);
    state = SHEN_NAME(FADE);
    return 50;
}
} // namespace
void Initialize4Bil() {
    routeSelection = 0;
    nextStage = nullptr;
    state = 0;
    triggerDelay = 0;
    savedState = 0;
    authorDelay = 0;
    authorWaitRequested = false;
}
void Start4Bil(std::uintptr_t) {
    Setup4Bil();
    HLib::Task *const task = HLib::EnqueueTaskWithoutParameter(Update4Bil, 6, 11, SHEN_NAME(4bil));
    Queue_EVT_UTL_LoadNPCChara_3(characters, task);
}
const ScriptAreaDefinition area4BilScriptDefinition{.areaId = SHEN_NAME(4BIL), .field04 = 0, .init = Initialize4Bil, .main = Start4Bil};
} // namespace shenmue::s2::areas
