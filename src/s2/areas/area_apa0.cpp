#include "s2/areas/area_apa0.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_call.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2::areas {
namespace {
constexpr shen_name event{SHEN_NAME(EVNT)}, ryo{SHEN_NAME(RYO_)}, target{SHEN_NAME(CGT1)}, plane{SHEN_NAME(AIRP)}, dummy{SHEN_NAME(DUMY)},
    object{SHEN_NAME(TEGM)}, invalid{0xffffffffU};
constexpr shen_name fighters[]{SHEN_NAME(SPH1), SHEN_NAME(SPH2), SHEN_NAME(SPH3), SHEN_NAME(SPH4), SHEN_NAME(SPH5)};
struct ApartmentWork {
    HLib::Task *self;
    std::int32_t mode, phase;
    std::int32_t *source;
    HLib::Task *flight;
    float farDistance;
    std::uint32_t reserved24;
};
struct MessageWork {
    const char *message;
    std::int32_t remaining;
    std::uint32_t reserved0c;
};
static_assert(sizeof(ApartmentWork) == 0x28 && offsetof(ApartmentWork, flight) == 0x18);
static_assert(sizeof(MessageWork) == 0x10);
void Loop_Callback(void *);
void Shutter_Callback(void *);
void SayNext_Callback(void *);
void FinishSay_Callback(void *);
void MessageWait_Callback(void *data) {
    auto *work = static_cast<MessageWork *>(data);
    if (work->remaining-- == 1) {
        ctrlStopSubtitles();
        HLib::DestroyCurrentTask();
    }
}
void Message_Callback(void *data) {
    dbgPrintEventMessage(static_cast<MessageWork *>(data)->message);
    HLib::SetCurrentTaskTo(MessageWait_Callback);
}
void Message(const char *text, std::int32_t delay) {
    auto *task = HLib::EnqueueTaskWithParameter(Message_Callback, 6, 0x0b, sizeof(MessageWork), event);
    HLib::SetupTimedTask(task, delay);
    auto *work = static_cast<MessageWork *>(HLib::GetTaskParameter(task));
    work->message = text;
    work->remaining = 60;
}

void Reload(ApartmentWork *work) {
    using enum CameraState;
    EV_NPC_NewLoadSw(1);
    LoadCutsceneAndSoundProgramForArea();
    SetCharaKill(target, 1);
    UpdateMotionWorkWithPosition(scenFindMotionwork(ryo), nullptr, 0);
    camCtrlSetState(Init);
    SetPlayerControlEnabled(1);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("APA0"), 0);
    scenFreeEvent("APA0.EV3");
    if (work->farDistance != 0)
        camCtrlSetFarDistance(work->farDistance);
    if (work->flight)
        HLib::DestroyTask(work->flight);
    HLib::DestroyTask(work->self);
}

void MoveFlight(shen_name name) {
    Vector3 position;
    EV_GetCharaPosition(name, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    position.x -= 1;
    position.y += 0;
    position.z -= .5f;
    EV_AT_SetPos(name, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
}
void Plane_Callback(void *) {
    Vector3 position;
    EV_GetCharaPosition(plane, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    UpdateMotionWorkWithPosition(scenFindMotionwork(ryo), &position, 0);
    MoveFlight(plane);
}
void Fighters_Callback(void *) {
    Vector3 position;
    EV_GetCharaPosition(fighters[0], CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    UpdateMotionWorkWithPosition(scenFindMotionwork(ryo), &position, 0);
    for (auto name : fighters)
        MoveFlight(name);
}

void BeginFlight(ApartmentWork *work, bool fighter) {
    EVT_AT_MenCtrl(EventFilmControl{1, 16, 255, 0, 0, 0, 0, 0, 0, 0, 1});
    EV_AT_SetAngY(target, EventTransformFlags::AllAxes, Vector3i{});
    EV_AT_SetPos(target, EventTransformFlags::AllAxes, Vector3{220.5f, 120.2f, 266.1f});
    if (fighter) {
        for (auto name : fighters)
            SetScenarioCharacterRadius(name, 200);
        const Vector3 scale{50, 50, 50};
        for (auto name : fighters)
            SetMotionworkFlagForCharaById(name, &scale);
    } else
        SetScenarioCharacterRadius(plane, 200);
    work->farDistance = camCtrlGetFarDistance();
    camCtrlSetFarDistance(200);
    if (fighter) {
        constexpr Vector3 positions[]{{370, 160, 310}, {370, 160, 318}, {370, 160, 326}, {378, 160, 310}, {386, 160, 310}};
        for (int i = 0; i < 5; ++i)
            EV_AT_SetPos(fighters[i], EventTransformFlags::AllAxes, positions[i]);
        for (auto name : fighters)
            EV_AT_SetAngY(name, EventTransformFlags::AllAxes, Vector3i{0, 43690, 0});
    } else {
        EV_AT_SetPos(plane, EventTransformFlags::AllAxes, Vector3{370, 160, 320});
        EV_AT_SetAngY(plane, EventTransformFlags::AllAxes, Vector3i{0, 10922, 0});
    }
    mtwkSetRequestedRootMode(scenFindMotionwork(ryo), 0);
    EVT_AT_SetMot(ryo, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetMot(ryo, 0xf022, 0, .3f, 0, 100);
    EVT_AT_SetMot(ryo, 0xf002, 0, 1, 0, 220);
    EVT_AT_SetCam(110, ryo, target, 0, 0, 1);
    Loop3DSoundEffect(1801, fighter ? 386.0f : 320.0f);
    work->mode = 7;
    EVT_AT_MenCtrl(EventFilmControl{1, 16, 0, 0, 0, 0, 255, 0, 0, 0, 188});
    EVT_AT_MenCtrl(EventFilmControl{1, 16, 255, 0, 0, 0, 0, 0, 0, 0, 204});
}
void BeginPlane_Callback(void *data) {
    BeginFlight(static_cast<ApartmentWork *>(data), false);
}
void BeginFighters_Callback(void *data) {
    BeginFlight(static_cast<ApartmentWork *>(data), true);
}

std::int32_t ConstructionDialogue() {
    Message(shenGetRandomInt32(2) ? "「すみませんが、」" : "「すみませーん！」", 30);
    const auto random = shenGetRandomInt32(3);
    Message(random == 0 ? "「この先、仏具通りは工事中なんです。」" :
            random == 1 ? "「この先は工事中です。よそからまわってもらえますか。」" :
                          "「副仙街の方へまわってくださーい。」",
        90);
    return 150;
}

void Shutter_Callback(void *data) {
    auto *work = static_cast<ApartmentWork *>(data);
    DateTime time;
    switch (work->phase) {
    case 0:
        GetCurrentDateTime(time);
        if (time.hour >= 19) {
            Reload(work);
            return;
        }
        work->phase = 1;
        {
            std::int16_t choices[]{-1, 149, 155, -1};
            SetupControlSpeakAndUI(choices, 200, 155, 1);
        }
        break;
    case 1:
        if (!GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            work->phase = 2;
        break;
    case 2:
        if (shenGetCtrlSpeakIconFlag() == 155)
            Reload(work);
        EV_stepSetFlags(0, 620);
        work->phase = 3;
        break;
    case 3:
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        work->phase = 4;
        HLib::SetupTaskDelay(40);
        break;
    case 4:
        GetCurrentDateTime(time);
        if (time.hour < 19) {
            time.hour = 19;
            time.minute = 0;
        }
        SetCurrentDateTimeTo(time);
        EV_scenQueueLoad(g_actNumber, SHEN_NAME(AKA3), 1);
        work->phase = 5;
        HLib::SetupTaskDelay(10);
        break;
    default:
        Reload(work);
        break;
    }
}

void SayNext_Callback(void *) {
    EV_FreeAuthorPak(1, "0022");
    CreateEventCharacter(CharacterCreationHighModel, nullptr, dummy);
    Vector3 position;
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    const auto rotation = GetCharacterWorldRotation(FindCharacterByScenarioName(ryo));
    EV_AT_SetPos(dummy, EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(dummy, EventTransformFlags::AllAxes, rotation);
    EVT_AT_SetVoice("0022A001", 1, 30); // @text EN: "This is the Yan Tin Apartments.\nLishao Tao lives here=@" | JA: "ここが恩田楼か。\n桃李少老師は、ここに…"
    EVT_AT_SetMot(ryo, 0xf019, 59, 1, 0x40000000U, 0);
    EVT_AT_SetCam(130, invalid, invalid, 0, 0, 60);
    const Vector3 offset{.152f, .05f, -.089f};
    const Vector3i attachment{26858, 39326, 47764};
    AttachScenarioCharacter(object, ryo, 14, &offset, &attachment);
    EV_EnableCharacterDisplay(object);
    EVT_AT_SetCam(10, dummy, dummy, 0, 0, 0);
    SetLowQualityHands(FindCharacterByScenarioName(ryo));
    SetHighModelXYZ(FindCharacterByScenarioName(ryo), 1, 0, 0);
    HLib::SetCurrentTaskTo(FinishSay_Callback);
    HLib::SetupTaskDelay(150);
}

void FinishSay_Callback(void *) {
    using enum CameraState;
    EVT_AT_SetMot(ryo, 0xf003, 0, 1, 0x40000000U, 0);
    EV_FreeAuthorPak(0, "0022");
    SetCharaKill(dummy, 1);
    ResetScenarioCharacterAttachment(object);
    EV_DisableCharacterDisplay(object);
    EVS_LoopCamera(0, shen_name{}, shen_name{});
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    hndlSetFlags(FindCharacterByScenarioName(ryo));
    HLib::SetCurrentTaskTo(Loop_Callback);
}

void Loop_Callback(void *data) {
    auto *work = static_cast<ApartmentWork *>(data);
    int delay = 0;
    switch (work->mode) {
    case 0:
        Message("ＡＰＡ０ダミーＰＡ", 30);
        work->mode = 7;
        delay = 60;
        break;
    case 1:
        EVT_AT_SetCam(100, ryo, ryo, 0, 0, 0);
        EVT_AT_SetMot(ryo, 0xf002, 0, 0, 0, 0);
        Message("「赤いシャッター・・・、ここが倉庫か。」", 30);
        HLib::SetCurrentTaskTo(Shutter_Callback);
        delay = 60;
        break;
    case 2: {
        EV_LoadCutsceneFromCurrentScene("0174E000.smp");
        SetScreenFilmFade(16, 0, 0, 0, 0, 255, 0, 0, 0);
        const bool fighter = EV_GetShenmueFlag(133) && shenGetRandomInt32(2);
        work->flight = HLib::EnqueueTaskWithoutParameter(fighter ? Fighters_Callback : Plane_Callback, 5, 0x0b, static_cast<std::int32_t>(event.name));
        QueueEventRepeatedFunction(fighter ? BeginFighters_Callback : BeginPlane_Callback, work, 16, 1);
        delay = 220;
        break;
    }
    case 3: {
        EVT_AT_SetMot(ryo, 0xf002, 0, 0, 0, 0);
        auto frame = ConstructionDialogue();
        EVT_AT_MenCtrl(EventFilmControl{1, 30, 0, 0, 0, 0, 255, 0, 0, 0, static_cast<float>(frame)});
        frame += 30;
        EVT_AT_SetPos(ryo, invalid, Vector3{267.03f, 114.57f, 276.88f}, frame);
        EVT_AT_SetAngY(ryo, invalid, 0, frame);
        frame += 10;
        EVT_AT_MenCtrl(EventFilmControl{1, 30, 255, 0, 0, 0, 0, 0, 0, 0, static_cast<float>(frame)});
        work->mode = 7;
        delay = frame + 30;
        break;
    }
    case 4:
        mtwkRequestMotion(scenFindMotionwork(ryo), 0xf003, MotionRegion::LowerBody);
        EV_FreeAuthorPak(1, "0074");
        EVT_AT_SetVoice("0074A001", 1, 0); // @text EN: "I'll look for the South Carmain Qr." | JA: "次は紅南街を探すか…"
        work->mode = 5;
        delay = 80;
        break;
    case 5:
        EV_FreeAuthorPak(0, "0074");
        work->mode = 7;
        return;
    case 6:
        EVT_AT_SetMot(ryo, 0xf002, 0, 1, 0x40000000U, 0);
        EVT_AT_SetPos(ryo, invalid, Vector3{67.4f, 102.5f, 301}, 0);
        EVT_AT_SetAngY(ryo, invalid, 150, 0);
        EVT_AT_SetCam(120, ryo, invalid, 0, 0, 0);
        HLib::SetCurrentTaskTo(SayNext_Callback);
        work->mode = 7;
        delay = 100;
        break;
    case 7:
        Reload(work);
        return;
    default:
        work->mode = 7;
        delay = 60;
        break;
    }
    if (delay)
        HLib::SetupTaskDelay(delay);
}
} // namespace
void AreaAPA0Init() {
}

void AreaAPA0Main(std::uintptr_t parameter) {
    auto *source = reinterpret_cast<std::int32_t *>(parameter);
    SetPlayerControlEnabled(0);
    auto *task = HLib::EnqueueTaskWithParameter(Loop_Callback, 4, 0x0b, sizeof(ApartmentWork), event);
    auto *work = static_cast<ApartmentWork *>(HLib::GetTaskParameter(task));
    work->self = task;
    work->mode = *source;
    work->phase = 0;
    work->source = source;
    work->flight = nullptr;
    work->farDistance = 0;
    CreateEventCharacter(CharacterCreationHighModel, nullptr, target);
    EV_SetNewSceneIndex();
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("APA0"), 0);
}
const ScriptAreaDefinition areaAPA0ScriptDefinition{.areaId = SHEN_NAME(APA0), .field04 = 0, .init = AreaAPA0Init, .main = AreaAPA0Main};
} // namespace shenmue::s2::areas
