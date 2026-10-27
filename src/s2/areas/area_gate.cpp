#include "s2/areas/area_gate.hpp"
#include "s2/areas/area_gate_data.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/map_control.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include "common/math.hpp"
#include <array>
#include <cstring>

namespace shenmue::s2 {
namespace gate_event {
constexpr shen_name ryo{SHEN_NAME(RYO_)}, ren{SHEN_NAME(REN_)}, keep{SHEN_NAME(KEEP)}, keep2{SHEN_NAME(KEP2)};
constexpr shen_name invalid{0xffffffffU};
constexpr std::array<shen_name, 4> entrance1{ren, SHEN_NAME(KT1_), SHEN_NAME(KU6_), invalid};
constexpr std::array<shen_name, 4> entrance2{ren, SHEN_NAME(KK6_), SHEN_NAME(KK3_), invalid};
constexpr std::array<shen_name, 4> entrance3{ren, SHEN_NAME(KT3_), SHEN_NAME(KK1_), invalid};
constexpr std::array<shen_name, 8> allCharacters{
    ren, SHEN_NAME(KT1_), SHEN_NAME(KU6_), SHEN_NAME(KK6_), SHEN_NAME(KK3_), SHEN_NAME(KT3_), SHEN_NAME(KK1_), invalid};
constexpr std::array<shen_name, 2> renOnly{ren, invalid};
constexpr std::array<shen_name, 3> guards1{SHEN_NAME(KT1_), SHEN_NAME(KU6_), invalid};
constexpr std::array<shen_name, 3> guards2{SHEN_NAME(KK6_), SHEN_NAME(KK3_), invalid};
constexpr std::array<shen_name, 3> guards3{SHEN_NAME(KT3_), SHEN_NAME(KK1_), invalid};
shen_name primary = invalid, secondary = invalid;
Matrix4x4 placement{};
Vector3 savedPosition{};
Vector3i savedRotation{};
std::int16_t savedWeather{};
const shen_name *EntryCharacters(std::uint32_t entry) {
    switch (entry) {
    case 483:
        return entrance1.data();
    case 484:
        return entrance2.data();
    case 485:
        return entrance3.data();
    default:
        return nullptr;
    }
}

void ReleaseCharacters(const shen_name *names) {
    for (; names->name != invalid.name; ++names)
        if (FindCharacterByScenarioName(*names)) {
            EV_npcLockUnlock(*names);
            EV_npcFree(*names);
        }
}
void Position(shen_name character, shen_name base, const Vector3 &position) {
    EventSetPosition request{0, character, base, position, 0};
    EVT_AT_SetPosRot(&request);
}
void Angle(shen_name character, shen_name base, float degrees) {
    EventSetAngle request{0, character, base, degrees, 0};
    EVT_AT_SetAngY_Callback(&request);
}
void Camera(int id) {
    EventSetCamera request{0, id, keep, keep, 0, {}, 0, 0};
    EVT_AT_SetCam_Callback(&request);
}
void GravityOff(shen_name character) {
    EV_SetCharaControllerFlags(character, 2, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, 3, NpcCharacterControllerState::Disabled);
}
void Author(std::uint32_t id) {
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(id));
}
void Setup(GateEventWork &work) {
    Vector3 position{};
    float degrees = 0;
    const char *reply = nullptr;
    switch (work.entry) {
    case 483:
        primary = SHEN_NAME(KT1_);
        secondary = SHEN_NAME(KU6_);
        position = {191.5f, 100, 163};
        work.voices[0] = "0253A001";
        reply = "0253B001"; // @text EN: "You don't belong here!\nGet lost!" | JA: "ここはおまえの来る所じゃねぇ…\n消えな！"
        break;
    case 484:
        primary = SHEN_NAME(KK6_);
        secondary = SHEN_NAME(KK3_);
        position = {244.2f, 100, 187.8999939f};
        work.voices[0] = "0253A002";
        reply = "0253C001"; // @text EN: "I'm not letting any outsiders in here!\nGo home!" | JA: "よそ者を通すわけにはいかねぇ！\n…帰れ！"
        break;
    case 485:
        primary = SHEN_NAME(KT3_);
        secondary = SHEN_NAME(KK1_);
        position = {241.8f, 100, 115.3f};
        degrees = 180;
        work.voices[0] = "0253A003";
        reply = "0253D001"; // @text EN: "Can't allow anyone through.\nJust get outta here!" | JA: "誰も通すわけにはいかん。\nとっとと失せろ！"
        break;
    default:
        return;
    }
    work.offsets[0] = work.offsets[3] = .1f;
    work.offsets[1] = work.offsets[4] = -.050000001f;
    work.offsets[2] = work.offsets[5] = 0;
    Position(keep, invalid, position);
    Angle(keep, invalid, degrees);
    work.voices[1] = reply;
    work.voices[2] = nullptr;
    work.durations[0] = 40;
    work.durations[1] = 100;
    work.durations[2] = 0;
    work.cameras[0] = 0;
    work.cameras[1] = EV_GetShenmueFlag(0x120) ? 1030 : 1010;
    work.cameras[2] = 0;
}
void Visibility(std::uint32_t author) {
    DisableScenarioCharacterDisplay(ryo);
    switch (author) {
    case SHEN_NAME(AUT0):
        EnableScenarioCharacterDisplay(primary);
        EnableScenarioCharacterDisplay(secondary);
        [[fallthrough]];
    case SHEN_NAME(AUT1):
        EnableScenarioCharacterDisplay(ryo);
        EnableScenarioCharacterDisplay(ren);
        break;
    case SHEN_NAME(AUT2):
        DisableScenarioCharacterDisplay(ryo);
        EnableScenarioCharacterDisplay(SHEN_NAME(KT3_));
        EnableScenarioCharacterDisplay(SHEN_NAME(KK1_));
        break;
    case SHEN_NAME(AUT3):
        DisableScenarioCharacterDisplay(ryo);
        EnableScenarioCharacterDisplay(SHEN_NAME(KT1_));
        EnableScenarioCharacterDisplay(SHEN_NAME(KU6_));
        break;
    case SHEN_NAME(AUT4):
        DisableScenarioCharacterDisplay(ryo);
        EnableScenarioCharacterDisplay(SHEN_NAME(KK6_));
        EnableScenarioCharacterDisplay(SHEN_NAME(KK3_));
        break;
    }
}
void Placement(std::uint32_t author, const Vector3 *position, std::uint16_t yaw) {
    Matrix4x4_PushActiveTransform();
    switch (author) {
    case SHEN_NAME(AUT2):
        Matrix4x4::preTranslate(241.8f, 100, 116.5f);
        ApplyShearToPrePassMatrix(shenmue::sin_fixed16(0x4000), shenmue::sin_fixed16(0));
        break;
    case SHEN_NAME(AUT3):
        Matrix4x4::preTranslate(191.5f, 100, 162);
        ApplyShearToPrePassMatrix(shenmue::sin_fixed16(-16384), shenmue::sin_fixed16(0));
        break;
    case SHEN_NAME(AUT4):
        Matrix4x4::preTranslate(244.2f, 100, 186.89999f);
        ApplyShearToPrePassMatrix(shenmue::sin_fixed16(-16384), shenmue::sin_fixed16(0));
        break;
    default:
        Matrix4x4::preTranslate(position->x, position->y, position->z);
        Matrix4x4::RotateAroundY_Wrapper(yaw);
        break;
    }
    if (author == SHEN_NAME(AUT2) || author == SHEN_NAME(AUT3) || author == SHEN_NAME(AUT4))
        Matrix4x4::preTranslate(0, 0, 3.2f);
    placement = workTransformMatrix;
    Author_SetPlacementMatrix(&placement);
    Matrix4x4::Pop();
}
void HideWorld() {
    SetIsSkySetup(0);
    for (int i = 0; i < 5; ++i)
        EV_SetObjectFlagByIndex(i, 0);
    if (g_currentMap.name != SHEN_NAME(Q300))
        EV_SetObjectFlagByIndex(5, 0);
}
void Detach() {
    AT_SetLocalBaseChara(ryo, invalid);
    AT_SetLocalBaseChara(primary, invalid);
    AT_SetLocalBaseChara(secondary, invalid);
}
void Cleanup(GateEventWork &work) {
    using enum CameraState;
    EV_FreeAuthorPak(0, "0253");
    ctrlSetKillAndIncrementCounter(ctrlGetCharaByName(keep), 1);
    ctrlSetKillAndIncrementCounter(ctrlGetCharaByName(keep2), 1);
    ReleaseCharacters(allCharacters.data());
    EV_NPC_NewLoadSw(1);
    LoadCutsceneAndSoundProgramForArea();
    EV_LoadAudio(shenGetCurrentSbt(), GetSelectedSoundSceneIndex());
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    SetPlayerControlEnabled(1);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("GATE"), 0);
    shenSetClockAndCompassRenderTo(0);
    FinishLoading();
    HLib::DestroyTask(work.task);
}
void Restore(GateEventWork &work, std::uint32_t rotationFlags) {
    Vector3i rotation{};
    GetScenarioCharacterEulerAngles(ryo, &rotation, rotationFlags);
    auto *motion = scenFindMotionwork(ryo);
    mtwkRequestMotion(motion, 0xf003, MotionRegion::LowerBody);
    EV_AT_SetAngY(ryo, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    mtwkSetExtraFlag(motion, 17, 0);
    SetMotionRegionFlag(motion, 71, 0, MotionRegion::LowerBody);
    work.state = 0;
    Cleanup(work);
}
const shen_name *FirstGuards() {
    return primary.name == SHEN_NAME(KT1_) || primary.name == SHEN_NAME(KK6_) ? guards3.data() : guards1.data();
}
const shen_name *SecondGuards() {
    return primary.name == SHEN_NAME(KK6_) ? guards1.data() : guards2.data();
}
} // namespace gate_event

void GATE_init_func() {
    using namespace gate_event;
    primary = secondary = invalid;
    placement = {};
    savedPosition = {};
    savedRotation = {};
    savedWeather = 0;
}
void GATE_main_func(std::uintptr_t entry) {
    using namespace gate_event;
    SetPlayerControlEnabled(0);
    shenSetClockAndCompassRenderTo(1);
    nullsub();
    SetHighModelXYZ(ctrlGetCharaByName(ryo), 1, 1, 0);
    SetHighModelXYZ(ctrlGetCharaByName(ryo), 1, 0, 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(ryo), 1, reinterpret_cast<std::byte *>(handPose.data()), 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(ryo), 0, reinterpret_cast<std::byte *>(handPose.data()), 0);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(GateEventCallback, 4, 0x0b, sizeof(GateEventWork), GATE_EVENT_TASK);
    GateEventWork *work = static_cast<GateEventWork *>(HLib::GetTaskParameter(task));
    std::memset(work, 0, sizeof(*work));
    work->task = task;
    work->state = 1;
    work->entry = static_cast<std::uint32_t>(entry);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, keep);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, keep2);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("GATE"), 0);
    EV_FreeAuthorPak(1, "0253");
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0253E000");
    nullsub();
    if (const shen_name *names = EntryCharacters(work->entry))
        Queue_EVT_UTL_LoadNPCChara_3(names, task);
    mtwkRequestMotion(scenFindMotionwork(ryo), 0xf003, MotionRegion::LowerBody);
}
void GateAuthorWhiteCallback(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) < 28)
        return;
    EV_authSceneCtrl(AuthSceneCtrl::SetAuthorStopped);
    SetScreenFilmFade(15, 0, 255, 255, 255, 255, 255, 255, 255);
    HLib::SetCurrentTaskTo(GateAuthorWhiteCompleteCallback);
    HLib::SetupTaskDelay(15);
}
void GateAuthorWhiteCompleteCallback(void *) {
    EV_authSceneCtrl(AuthSceneCtrl::ResetAuthorStopped);
    HLib::DestroyCurrentTask();
}
void GateEventCallback(void *parameter) {
    using namespace gate_event;
    auto &work = *static_cast<GateEventWork *>(parameter);
    switch (work.state) {
    case 1: {
        Setup(work);
        GravityOff(primary);
        GravityOff(secondary);
        AT_SetLocalBaseChara(ryo, keep);
        AT_SetLocalBaseChara(primary, keep);
        AT_SetLocalBaseChara(secondary, keep);
        Position(keep2, keep, {0, 0, 1.8f});
        Angle(keep2, keep, 0);
        Position(ryo, keep, {0, 0, 3});
        Angle(ryo, keep, 0);
        EnableScenarioCharacterDisplay(ryo);
        Camera(1000);
        camCtrlDisableFilmBars();
        Vector3 position{};
        Vector3i rotation{};
        EV_GetCharaPosition(keep2, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        GetScenarioCharacterEulerAngles(keep2, &rotation, EventTransformFlags::World);
        auto *motion = scenFindMotionwork(ryo);
        CopyMotionTransition(motion, &position, static_cast<std::uint16_t>(rotation.y), 0, 0);
        SetMotionRegionFlag(motion, 71, 0, MotionRegion::LowerBody);
        mtwkSetExtraFlag(motion, 13, 0);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        work.state = 4;
        return;
    }
    case 4:
        Position(primary, keep, {-.9131550193f, 0, .6000000238f});
        Position(secondary, keep, {.9131550193f, 0, .5788000226f});
        EVT_AT_SetMot(primary, 9608, 80, 1, 0, 0);
        EVT_AT_SetMot(secondary, 9605, 80, 1, 0, 0);
        EVT_AT_SetAng_Enqueue(primary, primary, 0, 90, 0, 0);
        EVT_AT_SetAng_Enqueue(secondary, secondary, 0, 90, 0, 0);
        EVT_AT_SetPoint(primary, ryo, 5, 1, 0, 0, 0, 0x20000000U);
        EVT_AT_SetPoint(secondary, ryo, 5, 1, 0, 0, 0, 0x20000000U);
        work.state = 6;
        HLib::SetupTaskDelay(30);
        return;
    case 6: {
        int delay = 0;
        for (int i = 0; work.durations[i]; ++i) {
            if (work.voices[i] && work.voices[i][0])
                EVT_AT_SetVoice(work.voices[i], delay);
            if (work.cameras[i]) {
                switch (work.entry) {
                case 483:
                    EVT_AT_SetPos(keep, invalid, {191.5f, 100, 162}, delay);
                    break;
                case 484:
                    EVT_AT_SetPos(keep, invalid, {244.2f, 100, 186.89999f}, delay);
                    break;
                case 485:
                    EVT_AT_SetPos(keep, invalid, {241.8f, 100, 116.5f}, delay);
                    break;
                }
                EVT_AT_SetCam(work.cameras[i], keep, keep, 0, 0, delay);
            }
            delay += work.durations[i];
        }
        if (EV_GetShenmueFlag(0x120)) {
            work.state = 13;
            EV_stepSetFlags(0, 0x120);
        } else
            work.state = 7;
        if (delay)
            HLib::SetupTaskDelay(delay);
        return;
    }
    case 7:
        Camera(1020);
        camCtrlDisableFilmBars();
        EVT_AT_SetMot(ryo, 9603, 0, 1, 0x40000000U, 0);
        if (EV_GetShenmueFlag(0x1f3)) {
            EVT_AT_SetVoice("0253A005", 30); // @text EN: "Not here.\nI'll try a different entrance." | JA: "ここはダメだ…\n他の入口へ行くか。"
            EV_stepSetFlags(0, 0x1f3);
        }
        if (EV_GetShenmueFlag(0x1f2)) {
            EVT_AT_SetVoice("0253A004", 30); // @text EN: "I have to look for a different way in=@" | JA: "他の入口を探すか…"
            EV_stepSetFlags(0, 0x1f2);
        }
        EVT_AT_MenCtrl({1, 30, 0, 0, 0, 0, 255, 0, 0, 0, 120});
        work.state = 9;
        HLib::SetupTaskDelay(150);
        return;
    case 9:
        Detach();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        Restore(work, 0x40000000U);
        return;
    case 13:
        Detach();
        EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &savedPosition, EventTransformFlags::World);
        GetScenarioCharacterEulerAngles(ryo, &savedRotation, EventTransformFlags::World);
        DisableScenarioCharacterDisplay(ryo);
        EnableScenarioCharacterDisplay(primary);
        EnableScenarioCharacterDisplay(secondary);
        EnableScenarioCharacterDisplay(ryo);
        EnableScenarioCharacterDisplay(ren);
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
        Author(SHEN_NAME(AUT0));
        Placement(SHEN_NAME(AUT0), &savedPosition, static_cast<std::uint16_t>(savedRotation.y));
        work.state = 14;
        return;
    case 14:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            return;
        if (GetAuthorFlags()) {
            work.state = 22;
            return;
        }
        if (const auto *names = EntryCharacters(work.entry))
            ReleaseCharacters(names);
        Queue_EVT_UTL_LoadNPCChara_3(FirstGuards(), work.task);
        work.state = 15;
        return;
    case 15: {
        HLib::EnqueueTaskWithoutParameter(GateAuthorWhiteCallback, 4, 0x0b, static_cast<std::int32_t>(GATE_WHITE_TASK.name));
        const auto author = primary.name == SHEN_NAME(KT1_) || primary.name == SHEN_NAME(KK6_) ? SHEN_NAME(AUT2) : SHEN_NAME(AUT3);
        Visibility(author);
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
        Author(author);
        SetIsSkySetup(0);
        savedWeather = static_cast<std::int16_t>(GetCurrentWeather());
        if (savedWeather == 2)
            EV_UpdateWeatherForMinute(1, 0, 0, 0);
        HideWorld();
        Placement(author, nullptr, 0);
        work.state = 16;
        return;
    }
    case 16:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            return;
        if (GetAuthorFlags()) {
            work.state = 22;
            return;
        }
        ReleaseCharacters(FirstGuards());
        Queue_EVT_UTL_LoadNPCChara_3(SecondGuards(), work.task);
        work.state = 17;
        return;
    case 17: {
        HLib::EnqueueTaskWithoutParameter(GateAuthorWhiteCallback, 4, 0x0b, static_cast<std::int32_t>(GATE_WHITE_TASK.name));
        const auto author = primary.name == SHEN_NAME(KK6_) ? SHEN_NAME(AUT3) : SHEN_NAME(AUT4);
        Visibility(author);
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
        Author(author);
        HideWorld();
        Placement(author, nullptr, 0);
        work.state = 18;
        return;
    }
    case 18:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            return;
        ReleaseCharacters(SecondGuards());
        Queue_EVT_UTL_LoadNPCChara_3(renOnly.data(), work.task);
        work.state = 19;
        return;
    case 19:
        DisableScenarioCharacterDisplay(ryo);
        EnableScenarioCharacterDisplay(ryo);
        EnableScenarioCharacterDisplay(ren);
        if (savedWeather == 2) {
            ResetWeatherOverrides();
            EV_FreeWAIR();
            EV_UpdateWeather();
        }
        SetIsSkySetup(1);
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
        Author(SHEN_NAME(AUT1));
        Placement(SHEN_NAME(AUT1), &savedPosition, static_cast<std::uint16_t>(savedRotation.y));
        work.state = 21;
        return;
    case 21:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work.state = 22;
        return;
    case 22:
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        Restore(work, 0x42000000U);
        return;
    default:
        return;
    }
}
const ScriptAreaDefinition areaGATEScriptDefinition{.areaId = SHEN_NAME(GATE), .field04 = 0, .init = GATE_init_func, .main = GATE_main_func};
} // namespace shenmue::s2
