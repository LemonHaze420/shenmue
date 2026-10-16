#include "s2/areas/area_frbt.hpp"
#include "s2/areas/area_frbt_messages.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_call.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/input.hpp"
#include "s2/inventory.hpp"
#include "s2/math.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/qte_hand_poses.hpp"
#include "s2/screen_film.hpp"
#include "s2/step_event.hpp"
#include <cstring>

namespace shenmue::s2::areas {
namespace {
void AppendFrbtVoice(std::int32_t prefix, const char *suffix) {
    std::strcpy(frbtVoiceSequence[frbtVoiceCount], frbtVoicePrefixes + prefix * 16);
    std::strcat(frbtVoiceSequence[frbtVoiceCount], suffix);
    ++frbtVoiceCount;
}
void FrbtRegisterMapItem(void *) {
    AddInventoryItem(6014, 1, 1);
}
} // namespace
void FrbtPlayPrefixVoice(std::int32_t prefix, const char *suffix, std::int32_t delay) {
    char voice[64];
    std::strcpy(voice, frbtVoicePrefixes + prefix * 16);
    std::strcat(voice, suffix);
    EVT_AT_SetVoice(voice, 1, delay);
}
void FrbtVoiceWaitCallback(void *data) {
    FrbtVoiceWork &work = *static_cast<FrbtVoiceWork *>(data);
    if (cvsHasValidConverseData(nullptr)) {
        if (work.elapsed++ > 600)
            scenCvsCtrlFree(nullptr);
        if ((work.flags & 0x40000000U) && CheckMediatedDoublePress())
            scenCvsCtrlFree(nullptr);
    } else {
        const char *voice = frbtVoiceSequence[work.index];
        if (!voice[0]) {
            if (work.parent)
                HLib::ResolveDeferredCallback(work.parent);
            HLib::DestroyCurrentTask();
            return;
        }
        EVT_AT_SetVoice(voice, 1, work.delay);
        if (work.delay > 0)
            HLib::SetupTaskDelay(work.delay);
        ++work.index;
        work.elapsed = 0;
        if (!frbtVoiceSequence[work.index][0] && (work.flags & 0x20000000U)) {
            if (work.parent)
                HLib::ResolveDeferredCallback(work.parent);
            HLib::DestroyCurrentTask();
        }
    }
}
void FrbtPositionBase() {
    Vector3 player;
    EV_GetCharaPosition(FRBT_RYO, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::World);
    float nearest = 500;
    std::int16_t selected = 0;
    const FrbtPlacement *positions = FRBT_BASES + frbtVariant * 5;
    for (std::int32_t i = 0; positions[i].position.y > 0; ++i) {
        const float distance = GetSquaredEuclideanDistance(&player, &positions[i].position);
        if (nearest > distance) {
            nearest = distance;
            selected = static_cast<std::int16_t>(i);
        }
    }
    if (positions[selected].position.y > 0) {
        EventSetPosition position{.cleanup = 0, .character = FRBT_BASE, .parent = FRBT_INVALID, .position = positions[selected].position, .delay = 0};
        EVT_AT_SetPosRot(&position);
    }
    EventSetAngle angle{.cleanup = 0, .character = FRBT_BASE, .parent = FRBT_INVALID, .degrees = positions[selected].yaw, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
}
void FrbtResetEnemyMotion(shen_name character) {
    EV_EnableCharacterDisplay(character);
    mtwkSetRequestedRootMode(FindMotionWork(FindCharacterByScenarioName(character)), 3);
    SetMotionFrameTime(FindMotionWork(FindCharacterByScenarioName(character)), 7, MotionRegion::LowerBody);
    SetMotionRegionFlag(FindMotionWork(FindCharacterByScenarioName(character)), 69, 2, MotionRegion::LowerBody);
    EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, 0, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharacterAlphaEnabled(character, false);
    mtwkSetExtraFlag(FindMotionWork(FindCharacterByScenarioName(character)), 55, 2);
}
void FrbtFacePlayer(shen_name character) {
    Vector3 player, enemy;
    EV_GetCharaPosition(character, CHARACTER_TRANSLATION_ROOT, &enemy, EventTransformFlags::World);
    EV_GetCharaPosition(FRBT_RYO, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::World);
    const Vector3i rotation{.x = 0, .y = InverseFixedAtan2(player.x - enemy.x, player.z - enemy.z), .z = 0};
    EV_AT_SetAngY(character, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation, nullptr);
}
std::int32_t FrbtStartAuthor(FrbtWork &work) {
    FrbtPositionBase();
    EventSetCamera camera{.cleanup = 0, .camera = 2000, .eye = FRBT_BASE, .target = FRBT_BASE, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    EventSetPosition position{.cleanup = 0, .character = FRBT_RYO, .parent = FRBT_BASE, .position = {}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = FRBT_RYO, .parent = FRBT_BASE, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    EventSetMotion motion{.cleanup = 0, .character = FRBT_RYO, .animation = 4945, .frame = 60, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(4945);
    EVT_AT_SetVoice(FRBT_START_VOICES[frbtVariant - 7], 1, 0);
    for (std::int32_t i = 0; i < work.enemyCount; ++i) {
        position.character = work.enemies[i];
        position.position = FRBT_START_ENEMY_POSITIONS[i];
        EVT_AT_SetPosRot(&position);
        FrbtFacePlayer(work.enemies[i]);
        motion.character = work.enemies[i];
        motion.animation = i ? 4947U : 4946U;
        motion.frame = i ? static_cast<float>(i - 1) * 50.0f + 0.0f : 20.0f;
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(static_cast<std::uint16_t>(motion.animation));
    }
    return 80;
}
std::int32_t FrbtWinMessage(FrbtWork &work) {
    for (std::int32_t i = 0; i < work.enemyCount; ++i)
        EV_DisableCharacterDisplay(work.enemies[i]);

    EventSetMotion motion;
    motion.cleanup = 0;
    motion.character = FRBT_RYO;
    motion.animation = 0xf003;
    motion.frame = 0;
    motion.speed = 1;
    motion.flags = 0x40000000U;
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(0xf003);
    return static_cast<std::int32_t>(QueueEventMessageSequence(FRBT_WIN_MESSAGES[frbtResponse]) + 60);
}
std::int32_t FrbtRunAway(FrbtWork &work) {
    EV_LoadAudio("0052E010", 1);
    for (std::int32_t i = 1; i < work.enemyCount; ++i)
        EV_DisableCharacterDisplay(work.enemies[i]);
    FrbtPositionBase();
    SetScreenFilmFade(15, 255, 0, 0, 0, 0, 0, 0, 0);
    EventSetPosition position{.cleanup = 0, .character = FRBT_RYO, .parent = FRBT_BASE, .position = {}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = FRBT_RYO, .parent = FRBT_BASE, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    EventSetMotion motion{.cleanup = 0, .character = FRBT_RYO, .animation = 0xf003, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(0xf003);
    const bool sam = GetMotionAnimationId(FindMotionWork(FindCharacterByScenarioName(work.enemies[0])), MotionRegion::LowerBody) == 7;
    EventSetCamera camera{
        .cleanup = 0, .camera = sam ? 1200 : 1201, .eye = FRBT_BASE, .target = FRBT_BASE, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    position.character = work.enemies[0];
    EVT_AT_SetPosRot(&position);
    angle.character = work.enemies[0];
    angle.degrees = sam ? 0.0f : 180.0f;
    EVT_AT_SetAngY_Callback(&angle);
    motion.character = work.enemies[0];
    motion.animation = sam ? 4949U : 4948U;
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(static_cast<std::uint16_t>(motion.animation));
    EVT_AT_SetVoice(FRBT_EXIT_VOICES[frbtVariant - 7], 1, 0);
    EVT_AT_MenCtrl({.cleanup = 1,
        .frames = 30,
        .startAlpha = 0,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 255,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 60});
    return 90;
}
void FrbtVictoryAuthorCallback(void *data) {
    FrbtWork &work = *static_cast<FrbtWork *>(data);
    if (work.phase == 1) {
        EventSetCamera camera{.cleanup = 0, .camera = 1002, .eye = FRBT_BASE, .target = FRBT_BASE, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
        EVT_AT_SetCam_Callback(&camera);
        camCtrlDisableFilmBars();
        EVT_AT_SetPoint(FRBT_RYO, FRBT_EMPTY, 0, 3, 0, 0, 0, 0x50000000U);
        EventSetMotion motion{.cleanup = 0, .character = FRBT_RYO, .animation = 4929, .frame = 0, .speed = 1, .flags = 0, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(4929);
        const bool sam = GetMotionAnimationId(FindMotionWork(FindCharacterByScenarioName(work.enemies[0])), MotionRegion::LowerBody) == 7;
        motion.character = work.enemies[0];
        motion.animation = sam ? 4932U : 4942U;
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(static_cast<std::uint16_t>(motion.animation));
        const std::int32_t delay = frbtResponse == 1 ? 60 : 235;
        if (frbtResponse != 1) {
            if (!frbtResponse)
                FrbtPlayPrefixVoice(0, "004", 120);
            EVT_AT_SetCam(1003, FRBT_BASE, FRBT_BASE, 0, 0, 60);
            EVT_AT_SetCam(1004, FRBT_BASE, FRBT_BASE, 0, 0, 120);
        }
        EVT_AT_MenCtrl({.cleanup = 1,
            .frames = 30,
            .startAlpha = 0,
            .startRed = 0,
            .startGreen = 0,
            .startBlue = 0,
            .targetAlpha = 255,
            .targetRed = 0,
            .targetGreen = 0,
            .targetBlue = 0,
            .delay = static_cast<float>(delay - 30)});
        HLib::SetCurrentTaskTo(FrbtStopCallback);
        HLib::SetupTaskDelay(delay);
        return;
    }
    if (work.phase)
        return;
    EV_LoadAudio("0052E010", 1);
    SetHighModelXYZ(FindCharacterByScenarioName(FRBT_RYO), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(FRBT_RYO), 1, 1, 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(FRBT_RYO), 0, reinterpret_cast<std::byte *>(fortuneReadingHandPose), 8);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(FRBT_RYO), 1, reinterpret_cast<std::byte *>(fortuneReadingHandPose), 8);
    for (std::int32_t i = 1; i < work.enemyCount; ++i)
        EV_DisableCharacterDisplay(work.enemies[i]);
    FrbtPositionBase();
    EventSetAngle angle{.cleanup = 0, .character = FRBT_BASE, .parent = FRBT_BASE, .degrees = -180, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (!frbtResponse) {
        AppendFrbtVoice(1, "001");
        AppendFrbtVoice(1, "002");
        AppendFrbtVoice(0, "001");
        AppendFrbtVoice(1, "003");
        AppendFrbtVoice(1, "004");
        AppendFrbtVoice(0, "002");
        AppendFrbtVoice(1, "005");
        AppendFrbtVoice(0, "003");
        AppendFrbtVoice(1, "006");
    } else if (frbtResponse == 1) {
        AppendFrbtVoice(1, "001");
        AppendFrbtVoice(1, "008");
        AppendFrbtVoice(0, "005");
        AppendFrbtVoice(1, "009");
        AppendFrbtVoice(0, "008");
        AppendFrbtVoice(1, "011");
    } else {
        AppendFrbtVoice(1, "008");
        AppendFrbtVoice(0, "001");
        AppendFrbtVoice(1, "014");
        AppendFrbtVoice(1, "015");
        AppendFrbtVoice(1, "016");
        AppendFrbtVoice(1, "017");
        AppendFrbtVoice(0, "010");
        AppendFrbtVoice(1, "018");
    }
    EventSetPosition position{.cleanup = 0, .character = FRBT_RYO, .parent = FRBT_BASE, .position = {}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = FRBT_RYO, .parent = FRBT_BASE, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position.character = work.enemies[0];
    position.parent = FRBT_RYO;
    position.position = FRBT_WIN_ENEMY_POSITION;
    EVT_AT_SetPosRot(&position);
    angle.character = work.enemies[0];
    angle.parent = FRBT_RYO;
    EVT_AT_SetAngY_Callback(&angle);
    EventSetMotion motion{.cleanup = 0, .character = FRBT_RYO, .animation = 4931, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(4931);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_SetMot_Callback, 6, 0x0b, sizeof(EventSetMotion), AREA_EVENT_TASK);
    RegisterEventTask(task);
    EventSetMotion *queuedMotion = static_cast<EventSetMotion *>(HLib::GetTaskParameter(task));
    *queuedMotion = {.cleanup = 1, .character = FRBT_RYO, .animation = 4930, .frame = 0, .speed = 1, .flags = 0, .delay = 100};
    GetMotionFrameCount(4930);
    const bool sam = GetMotionAnimationId(FindMotionWork(FindCharacterByScenarioName(work.enemies[0])), MotionRegion::LowerBody) == 7;
    motion.character = work.enemies[0];
    motion.animation = sam ? 4934U : 4944U;
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(static_cast<std::uint16_t>(motion.animation));
    EVT_AT_SetMot(work.enemies[0], sam ? 4933U : 4943U, 0, 1, 0, 100);
    EventSetCamera camera{.cleanup = 0, .camera = 1000, .eye = FRBT_BASE, .target = FRBT_BASE, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    EVT_AT_SetCam(1001, FRBT_BASE, FRBT_BASE, 0, 0, 100);
    EVT_AT_SetPoint(FRBT_RYO, work.enemies[0], 5, 3, 0, 100, 0, 0x20000000U);
    task = HLib::EnqueueTaskWithParameter(FrbtVoiceWaitCallback, 6, 0x0b, sizeof(FrbtVoiceWork), AREA_EVENT_TASK);
    FrbtVoiceWork *voice = static_cast<FrbtVoiceWork *>(HLib::GetTaskParameter(task));
    voice->parent = work.task;
    voice->delay = 10;
    voice->index = 0;
    voice->elapsed = 0;
    voice->flags = 0;
    frbtVoiceSequence[frbtVoiceCount][0] = 0;
    frbtVoiceCount = 0;
    if (voice->parent)
        HLib::SetupTimedTask(voice->parent, 0);
    ++work.phase;
}
void FrbtMapGetCallback(void *data) {
    FrbtWork &work = *static_cast<FrbtWork *>(data);
    SetHighModelXYZ(FindCharacterByScenarioName(FRBT_RYO), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(FRBT_RYO), 1, 1, 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(FRBT_RYO), 0, reinterpret_cast<std::byte *>(fortuneReadingHandPose), 8);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(FRBT_RYO), 1, reinterpret_cast<std::byte *>(fortuneReadingHandPose), 8);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_CtrlHandUp_Callback, 6, 0x0b, sizeof(EventControlHand), AREA_EVENT_TASK);
    RegisterEventTask(task);
    EventControlHand *hand = static_cast<EventControlHand *>(HLib::GetTaskParameter(task));
    *hand = {
        .cleanup = 1, .character = FRBT_RYO, .side = 0, .reserved0c = 0, .command = reinterpret_cast<std::byte *>(frbtMapHandPose), .ticks = 8, .delay = 480};
    EV_LoadAudio("0052E010", 1);
    for (std::int32_t i = 1; i < work.enemyCount; ++i)
        EV_DisableCharacterDisplay(work.enemies[i]);
    FrbtPositionBase();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EventSetPosition position{.cleanup = 0, .character = work.enemies[0], .parent = FRBT_BASE, .position = {}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = work.enemies[0], .parent = FRBT_BASE, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position.character = FRBT_RYO;
    position.parent = work.enemies[0];
    position.position = FRBT_MAP_PLAYER_POSITION;
    EVT_AT_SetPosRot(&position);
    angle.character = FRBT_RYO;
    angle.parent = work.enemies[0];
    EVT_AT_SetAngY_Callback(&angle);
    EventSetMotion motion{.cleanup = 0, .character = FRBT_RYO, .animation = 4936, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(4936);
    motion.character = work.enemies[0];
    motion.animation = 4939;
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(4939);
    FrbtPlayPrefixVoice(1, "008", 60);
    AppendFrbtVoice(1, "002");
    AppendFrbtVoice(0, "005");
    AppendFrbtVoice(1, "009");
    AppendFrbtVoice(1, "006");
    AppendFrbtVoice(1, "010");
    FrbtPlayPrefixVoice(1, "007", 650);
    FrbtPlayPrefixVoice(0, "007", 720);
    EventSetCamera camera{.cleanup = 0, .camera = 1100, .eye = FRBT_BASE, .target = FRBT_BASE, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    EVT_AT_SetCam(1101, FRBT_BASE, FRBT_BASE, 0, 0, 170);
    EVT_AT_SetCam(1103, FRBT_BASE, FRBT_BASE, 0, 0, 480);
    EVT_AT_SetCam(1104, FRBT_BASE, FRBT_BASE, 0, 0, 630);
    EVT_AT_SetPoint(FRBT_RYO, work.enemies[0], 5, 3, 0, 200, 0, 0x20000000U);
    EVT_AT_SetPoint(FRBT_RYO, FRBT_EMPTY, 0, 3, 0, 455, 0, 0x10000000U);
    QueueEventCallFunction(FrbtRegisterMapItem, nullptr, 550);
    EVT_AT_ObjectDisp_1(FRBT_MAP_PROP, 490, 0);
    EVT_AT_ObjectDisp_1(FRBT_MAP_PROP, 700, 1);
    task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), AREA_EVENT_TASK);
    RegisterEventTask(task);
    EventCling *cling = static_cast<EventCling *>(HLib::GetTaskParameter(task));
    cling->cleanup = 1;
    cling->target = FRBT_MAP_PROP;
    cling->character = FRBT_RYO;
    cling->bone = 14;
    cling->position = {.x = .16f, .y = .02f, .z = -.11f};
    cling->rotation = {.x = 20480, .y = 40960, .z = 49152};
    cling->mode = 0;
    cling->delay = 490;
    task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), AREA_EVENT_TASK);
    RegisterEventTask(task);
    cling = static_cast<EventCling *>(HLib::GetTaskParameter(task));
    cling->cleanup = 1;
    cling->target = FRBT_MAP_PROP;
    cling->character = FRBT_EMPTY;
    cling->mode = 1;
    cling->delay = 700;
    EVT_AT_MenCtrl({.cleanup = 1,
        .frames = 30,
        .startAlpha = 0,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 255,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 770});
    task = HLib::EnqueueTaskWithParameter(FrbtVoiceWaitCallback, 6, 0x0b, sizeof(FrbtVoiceWork), AREA_EVENT_TASK);
    HLib::SetupTimedTask(task, 200);
    FrbtVoiceWork *voice = static_cast<FrbtVoiceWork *>(HLib::GetTaskParameter(task));
    voice->parent = nullptr;
    voice->delay = 10;
    voice->index = 0;
    voice->elapsed = 0;
    voice->flags = 0;
    frbtVoiceSequence[frbtVoiceCount][0] = 0;
    frbtVoiceCount = 0;
    if (voice->parent)
        HLib::SetupTimedTask(voice->parent, 0);
    HLib::SetCurrentTaskTo(FrbtStopCallback);
    HLib::SetupTaskDelay(800);
}
} // namespace shenmue::s2::areas
