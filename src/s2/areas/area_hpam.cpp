#include "s2/areas/area_hpam.hpp"
#include "s2/pak.hpp"
#include "s2/areas/area_hpam_data.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/conversation.hpp"
#include "s2/event_call.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/inventory.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sk_event.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"
#include "common/math.hpp"
#include <bit>

namespace shenmue::s2 {
namespace map_stand {
constexpr shen_name ryo{SHEN_NAME(RYO_)};
constexpr Vector3 playerOffset{-.664900004863739f, 0, -.06469999998807907f};
void Gravity(bool enabled) {
    auto state = enabled ? NpcCharacterControllerState::Enabled : NpcCharacterControllerState::Disabled;
    EV_SetCharaControllerFlags(ryo, 2, state);
    EV_SetCharaControllerFlags(ryo, 3, state);
}
void Camera(std::int32_t id, shen_name eye, shen_name target) {
    EventSetCamera request{0, id, eye, target, 0, {}, 0, 0};
    EVT_AT_SetCam_Callback(&request);
}
void Position(shen_name stand) {
    EventSetPosition request{0, ryo, stand, playerOffset, 0};
    EVT_AT_SetPosRot(&request);
}
void Film(int frames, int alpha) {
    EventFilmControl request{0, frames, alpha, 0, 0, 0, 0, 0, 0, 0, 0};
    EVT_AT_MenCtrl_Callback(&request);
}
void FadeOut(int delay) {
    EVT_AT_MenCtrl({1, 16, 0, 0, 0, 0, 255, 0, 0, 0, static_cast<float>(delay)});
}
void Choice() {
    std::int16_t choices[]{-1, 40, 41, -1};
    SetupControlSpeakAndUI(choices, 600, -1, 2);
}
void OwnsMapVoice() {
    const auto value = GetRandomNumberInRange(4);
    const char *voice = value == 0 ? "0099A037" : value == 1 ? "0099A038" : value == 2 ? "0099A039" : "0099A040"; // @text 0099A037 EN: "I already have one." | JA: "もう持っていたな。" ; 0099A038 EN: "I have one." | JA: "これはもう持ってるな。" ; 0099A039 EN: "I have it.\nDon't need another." | JA: "これは持ってる。いらないな…" ; 0099A040 EN: "I have this one." | JA: "これは持っていたな。"
    EVT_AT_SetVoice(voice, 55);
}
void Identify(MapStandEventWork &work) {
    std::uint32_t flag = 0;
    switch (work.stand.name) {
    case 0x30304450U:
    case 0x31304450U:
    case 0x32304450U:
    case 0x33304450U:
        work.item = 6012;
        work.map = SHEN_NAME(PAR0);
        flag = 0x5f;
        break;
    case 0x30314450U:
    case 0x31324450U:
    case 0x33314450U:
    case 0x39304450U:
        work.item = 6008;
        work.map = SHEN_NAME(PWK0);
        flag = 0x1e7;
        break;
    case 0x30324450U:
    case 0x33324450U:
    case 0x35314450U:
    case 0x36314450U:
        work.item = 6010;
        work.map = SHEN_NAME(PWN0);
        flag = 0x1e9;
        break;
    case 0x30334450U:
    case 0x32314450U:
    case 0x34314450U:
    case 0x37314450U:
        work.item = 6009;
        work.map = SHEN_NAME(PWT0);
        flag = 0x1eb;
        break;
    case 0x31314450U:
    case 0x34324450U:
    case 0x38314450U:
        work.item = 6007;
        work.map = SHEN_NAME(PWR0);
        flag = 0x1ea;
        break;
    case 0x31334450U:
    case 0x38324450U:
    case 0x39324450U:
        work.item = 6027;
        work.map = SHEN_NAME(PWB0);
        break;
    case 0x32324450U:
    case 0x35324450U:
    case 0x38304450U:
    case 0x39314450U:
        work.item = 6006;
        work.map = SHEN_NAME(PWE0);
        flag = 0x1e8;
        break;
    case 0x32334450U:
    case 0x35304450U:
        work.item = 6013;
        work.map = SHEN_NAME(PAR3);
        flag = 0x5f;
        break;
    case 0x34304450U:
        work.item = 6015;
        work.map = SHEN_NAME(PAK0);
        flag = 0x5f;
        break;
    case 0x36304450U:
    case 0x36324450U:
    case 0x37304450U:
    case 0x37324450U:
        work.item = 6011;
        work.map = SHEN_NAME(PWS0);
        flag = 0xb1;
        break;
    }
    if (flag)
        EV_stepSetFlags(0, flag);
}
void Describe(MapStandEventWork &work, int mode) {
    int reply = 0;
    const char *voice = nullptr;
    if (mode == 0)
        work.conversation = EV_PlayDialogue(GetRandomNumberInRange(2) == 0 ? "0099A003" : "0099A004"); // @text 0099A003 EN: "I'll get it later." | JA: "金がないな…\nあとにするか。" ; 0099A004 EN: "Not enough to buy one." | JA: "今は持ち合わせがないか…"
    else if (mode == 1)
        EV_PlayDialogue(GetRandomNumberInRange(2) == 0 ? "0099A001" : "0099A002"); // @text 0099A001 EN: "Huh?\nA map=@" | JA: "ん？　地図か…" ; 0099A002 EN: "Maps." | JA: "地図を売ってるな…"
    else if (mode == 2)
        EV_PlayDialogue("0099A033"); // @text EN: "Maybe later." | JA: "あとにしよう…"
    else if (mode == 3) {
        switch (work.stand.name) {
        case 0x30304450U:
        case 0x31304450U:
        case 0x32304450U:
        case 0x33304450U:
        case 0x34314450U:
        case 0x37304450U:
            voice = "0099A024"; // @text EN: "This area." | JA: "このあたりの地図か…"
            reply = 1;
            break;
        case 0x30314450U:
            voice = "0099A005"; // @text EN: "A map of this area=@" | JA: "この街の地図か…"
            reply = 2;
            break;
        case 0x30324450U:
        case 0x33324450U:
        case 0x35314450U:
        case 0x36314450U:
            voice = "0099A010"; // @text EN: "Lucky Charm Qr." | JA: "福仙街の地図か…"
            reply = 2;
            break;
        case 0x30334450U:
        case 0x32314450U:
        case 0x37314450U:
            voice = "0099A009"; // @text EN: "Wise Men's Qr." | JA: "太老街の地図か…"
            reply = 2;
            break;
        case 0x31314450U:
        case 0x34324450U:
        case 0x38314450U:
            voice = "0099A011"; // @text EN: "White Dynasty Qr." | JA: "白秦街の地図か…"
            reply = 2;
            break;
        case 0x31324450U:
        case 0x33314450U:
        case 0x39304450U:
            voice = "0099A008"; // @text EN: "South Carmain Qr." | JA: "紅南街の地図か…"
            reply = 2;
            break;
        case 0x31334450U:
            voice = "0099A049"; // @text EN: "Scarlet Hills." | JA: "紅華台の地図か…"
            reply = 1;
            break;
        case 0x32324450U:
        case 0x38304450U:
        case 0x39314450U:
            voice = "0099A012"; // @text EN: "Golden Qr." | JA: "詠黄街の地図か…"
            reply = 2;
            break;
        case 0x32334450U:
        case 0x34304450U:
        case 0x35304450U:
            voice = "0099A027"; // @text EN: "The area ahead of here." | JA: "この先の地図か…"
            reply = 2;
            break;
        case 0x35324450U:
            voice = "0099A005"; // @text EN: "A map of this area=@" | JA: "この街の地図か…"
            reply = 1;
            break;
        case 0x36304450U:
        case 0x36324450U:
        case 0x37324450U:
            voice = "0099A007"; // @text EN: "Green Market Qr." | JA: "菜珍街の地図か…"
            reply = 2;
            break;
        case 0x38324450U:
        case 0x39324450U:
            voice = "0099A049"; // @text EN: "Scarlet Hills." | JA: "紅華台の地図か…"
            reply = 2;
            break;
        }
        if (voice)
            EVT_AT_SetVoice(voice, 0);
    }
    if (work.mode == 2) {
        OwnsMapVoice();
        return;
    }
    if (reply == 1)
        voice = GetRandomNumberInRange(2) == 0 ? "0099A025" : "0099A026"; // @text 0099A025 EN: "Good." | JA: "役に立ちそうだ。" ; 0099A026 EN: "It'll be useful." | JA: "これで迷わなくてすむな。"
    else if (reply == 2)
        voice = GetRandomNumberInRange(2) == 0 ? "0099A028" : "0099A029"; // @text 0099A028 EN: "I'll get it." | JA: "持っておこう。" ; 0099A029 EN: "It'll come in handy." | JA: "あとで使えるな。"
    else if (reply == 3)
        voice = GetRandomNumberInRange(2) == 0 ? "0099A031" : "0099A032"; // @text 0099A031 EN: "I'll need it." | JA: "使えそうだな。" ; 0099A032 EN: "Good." | JA: "助かるな…"
    else
        return;
    EVT_AT_SetVoice(voice, 55);
}
void GiveMap(void *item) {
    AddInventoryItem(*static_cast<std::uint32_t *>(item), 1, 1);
}
void QueueDisplay(shen_name character, int mode, int delay) {
    auto *task = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 0x0b, sizeof(StepObjectDisplayWork), AREA_EVENT_TASK);
    RegisterEventTask(task);
    *static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(task)) = {1, character, mode, static_cast<float>(delay)};
}
void QueueCling(const EventCling &request) {
    auto *task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), AREA_EVENT_TASK);
    RegisterEventTask(task);
    *static_cast<EventCling *>(HLib::GetTaskParameter(task)) = request;
}
void QueueMessage(const char *text, int frames, int delay) {
    auto *task = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), AREA_EVENT_TASK);
    HLib::SetupTimedTask(task, delay);
    auto *work = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(task));
    work->message = text;
    work->frames = frames;
}
bool HasCash(const MapStandEventWork &work) {
    return GetPlayerStat(PlayerStatsId::Cash) >= static_cast<std::uint32_t>(work.cost);
}
int Intro(MapStandEventWork &work) {
    if (!work.phase) {
        EventSetMotion motion{0, ryo, 0xf005, 0, 1, 0x40000000U, 0};
        EVT_AT_SetMot_Callback(&motion);
        EVT_AT_SetMot(ryo, 0xf002, 0, 1, 0x40000000U, 10);
        Vector3 player{}, stand{};
        EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::World);
        EV_GetCharaPosition(work.stand, CHARACTER_TRANSLATION_ROOT, &stand, EventTransformFlags::World);
        EV_AT_SetAngY(ryo, EventTransformFlags::World | EventTransformFlags::AllAxes, {0, InverseFixedAtan2(stand.x - player.x, stand.z - player.z), 0});
        work.conversation = EV_PlayDialogue("0099A001");
        DisableScenarioCharacterDisplay(ryo);
        Camera(2000, ryo, work.stand);
        camCtrlDisableFilmBars();
        Choice();
        ++work.phase;
        return 0;
    }
    if (work.phase != 1 || GetShouldRenderIcons(RenderIconSlot::ActionSelector))
        return 0;
    if (shenGetCtrlSpeakIconFlag() != 40) {
        work.mode = 99;
        return 0;
    }
    if (HasCash(work)) {
        work.mode = 1;
        work.phase = 0;
        work.initialChoice = 1;
        EventFilmControl request{0, 20, 0, 0, 0, 0, 255, 0, 0, 0, 0};
        EVT_AT_MenCtrl_Callback(&request);
        return 20;
    }
    AddThisToCashOrSegaCoins(0, ECashOrCoins::Cash2);
    QueueMessage("お金が足りないか", 0, 60);
    QueueMessage("", 90, 10);
    work.mode = 99;
    return 120;
}
int Buy(MapStandEventWork &work) {
    switch (work.phase) {
    case 10:
        mtwkSetRequestedRootMode(scenFindMotionwork(ryo), 0);
        EVT_AT_SetMot(ryo, 0xf002, 0, 1, 0x40000000U, 0);
        EV_SetRefBoardParamsFor(ryo, 1);
        Position(work.stand);
        EVT_AT_SetAng_Enqueue(ryo, work.stand, 0, 270, 0, 0);
        Camera(1220, work.stand, work.stand);
        camCtrlDisableFilmBars();
        if (SetPlayerGameFlags(PlayerGameFlagMode::Bit5, reinterpret_cast<void *>(~std::uintptr_t{}))) {
            SetHighModelXYZ(ctrlGetCharaByName(ryo), 1, 1, 0);
            ApplyCharacterHandPoseCommand(ctrlGetCharaByName(ryo), 1, reinterpret_cast<std::byte *>(restingHand.data()), 0);
        }
        HLib::SetupTaskDelay(16);
        work.phase = 0;
        return 0;
    case 0:
        if (!HasCash(work)) {
            AddThisToCashOrSegaCoins(0, ECashOrCoins::Cash2);
            Describe(work, 0);
            work.mode = 99;
            return 120;
        }
        Describe(work, 1);
        ++work.phase;
        return 40;
    case 1:
        if (work.initialChoice) {
            Film(30, 255);
            work.phase = 2;
            return 0;
        }
        Choice();
        ++work.phase;
        return 0;
    case 2:
        if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            return 0;
        if (shenGetCtrlSpeakIconFlag() != 40) {
            work.mode = 99;
            EV_PlayDialogue("0099A033");
            FadeOut(14);
            return 30;
        }
        if (!HasCash(work)) {
            AddThisToCashOrSegaCoins(0, ECashOrCoins::Cash2);
            Describe(work, 0);
            FadeOut(104);
            work.mode = 99;
            return 120;
        }
        ++work.phase;
        return 0;
    case 3: {
        auto *character = ctrlGetCharaByName(ryo);
        SetHighModelXYZ(character, 1, 0, 0);
        const Vector3i rotation{61529, 2501, 364};
        ApplyCharacterHandRotation(character, 0, 21, &rotation);
        ApplyCharacterHandPoseCommand(character, 0, reinterpret_cast<std::byte *>(buyingHand.data()), 100);
        Vector3 position{};
        EV_GetCharaPosition(work.stand, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        UpdateMotionWorkWithPosition(scenFindMotionwork(ryo), &position, 0);
        EV_SetRefBoardParamsFor(work.map, 1);
        AddThisToCashOrSegaCoins(-work.cost, ECashOrCoins::Cash2);
        if (work.item != -1)
            QueueEventRepeatedFunction(GiveMap, &work.item, 120, 1);
        Describe(work, 3);
        EnableScenarioCharacterDisplay(ryo);
        QueueDisplay(work.map, 0, 86);
        QueueDisplay(work.map, 1, 257);
        QueueCling({1, work.map, ryo, 14, {.10199999809265137f, -.04100000113248825f, -.14499999582767487f}, {348, 22935, 13482}, 0, 86});
        QueueCling({1, work.map, ryo, 14, {.07100000232458115f, .007000000216066837f, -.16200000047683716f}, {62608, 23298, 9113}, 0, 110});
        QueueCling({1, work.map, {}, 0, {}, {}, 1, 257});
        Position(work.stand);
        EventSetAngle angle{0, ryo, work.stand, 270, 0};
        EVT_AT_SetAngY_Callback(&angle);
        EVT_AT_SetMot(ryo, 0x1541, 0, 1, 0x40000000U, 1);
        EVT_AT_SetMot(ryo, 0x1541, 112, 0, 0x40000000U, 110);
        EVT_AT_SetMot(ryo, 0x1541, 150, 1, 0x40000000U, 220);
        Camera(2010, work.stand, work.stand);
        camCtrlDisableFilmBars();
        EVT_AT_SetCam(2020, work.stand, work.stand, 0, 0, 110);
        EVT_AT_SetCam(2030, work.stand, work.stand, 0, 0, 220);
        work.mode = 99;
        FadeOut(270);
        return 286;
    }
    default:
        return 0;
    }
}
int Owned(MapStandEventWork &work) {
    mtwkSetRequestedRootMode(scenFindMotionwork(ryo), 0);
    EventSetMotion motion{0, ryo, 0xf002, 0, 1, 0x40000000U, 0};
    EVT_AT_SetMot_Callback(&motion);
    Camera(2010, work.stand, work.stand);
    camCtrlDisableFilmBars();
    Position(work.stand);
    EventSetRawAngle angle{0, ryo, work.stand, {0, 49152, 0}, 0};
    EVT_AT_SetAng_Callback(&angle);
    Describe(work, 3);
    work.mode = 99;
    FadeOut(120);
    return 140;
}
} // namespace map_stand
void HPAM_main_func(std::uintptr_t request) {
    using namespace map_stand;
    auto *input = reinterpret_cast<const std::uint32_t *>(request);
    auto *task = HLib::EnqueueTaskWithParameter(MapStandEventCallback, 5, 0x0b, sizeof(MapStandEventWork), AREA_EVENT_TASK);
    auto *work = static_cast<MapStandEventWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->mode = static_cast<std::int32_t>(input[1]);
    work->phase = 10;
    work->map = {};
    work->initialChoice = 0;
    if (request >= 99)
        work->stand = shen_name{input[0]};
    else {
        const auto n = static_cast<std::uint32_t>(request % 100);
        work->stand = shen_name{0x30304450U + (n % 10) * 0x01000000U + (n / 10) * 0x00010000U};
    }
    Identify(*work);
    if (work->item != -1 && GetItemState(work->item))
        work->mode = 2;
    work->cost = 10;
    const Vector4 light{1, 1, 1, 1};
    SetScenarioReflectorBoardValues(ryo, &light);
    EV_SetRefBoardParamsFor(ryo, 1);
    EV_SetNewSceneIndex();
    EV_EventSetUpHelpIcon_0(0x75, 0);
    SetPlayerControlEnabled(0);
    Film(16, 255);
    HLib::SetupTaskDelay(-1);
    Gravity(false);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("HPAM"), 0);
    EV_FreeAuthorPak(1, "0099");
    EV_LoadCutsceneFromCurrentScene("0099E000.smp");
}
void MapStandEventCallback(void *parameter) {
    auto &work = *static_cast<MapStandEventWork *>(parameter);
    int delay = 0;
    switch (work.mode) {
    case 0:
        delay = map_stand::Intro(work);
        break;
    case 1:
        delay = map_stand::Buy(work);
        break;
    case 2:
        delay = map_stand::Owned(work);
        break;
    case 99:
        HLib::SetCurrentTaskTo(MapStandExitCallback);
        return;
    default:
        return;
    }
    if (delay)
        HLib::SetupTaskDelay(delay);
}
void MapStandExitCallback(void *parameter) {
    using namespace map_stand;
    auto &work = *static_cast<MapStandEventWork *>(parameter);
    LoadCutsceneAndSoundProgramForArea();
    Film(8, 255);
    EnableScenarioCharacterDisplay(ryo);
    EV_SetRefBoardParamsFor(ryo, 0);
    UpdateMotionWorkWithPosition(scenFindMotionwork(ryo), nullptr, 0);
    hndlSetFlags(ctrlGetCharaByName(ryo));
    if (work.map.name)
        EV_SetRefBoardParamsFor(work.map, 0);
    Gravity(true);
    EV_NPC_NewLoadSw(1);
    EV_FreeAuthorPak(0, "0099");
    EVS_LoopCamera(0, {}, {});
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(CameraState::Init);
    SetPlayerControlEnabled(1);
    HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("HPAM"), 0);
}
const ScriptAreaDefinition areaHPAMScriptDefinition{.areaId = SHEN_NAME(HPAM), .field04 = 0, .init = nullptr, .main = HPAM_main_func};
} // namespace shenmue::s2
