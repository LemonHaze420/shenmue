#include "s2/areas/area_ycpa.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/flag.hpp"
#include "s2/math.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
namespace shenmue::s2::areas {
namespace {
constexpr float adviceDirections[24]{180, 165, 195, 150, 210, 135, 225, 120, 240, 105, 255, 90, 270, 75, 285, 60, 300, 45, 315, 30, 330, 15, 345, 0};
void IdleAdviceActor(shen_name character) {
    EventSetMotion cue{.cleanup = 0, .character = character, .animation = 0xf002, .frame = 0, .speed = 1, .flags = YCPA_MOTION_NO_BLEND, .delay = 0};
    EVT_AT_SetMot_Callback(&cue);
    (void)GetMotionFrameCount(0xf002);
}
} // namespace

void YcpaPrepareAdvice() {
    EV_npcLockUnlock(YCPA_YUA);
    EV_CheckWarmStartNpcRoute(YCPA_REN);
    EV_ForceLoadNpc(YCPA_REN);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_SetCharacterAlphaEnabled(YCPA_REN, false);
    UpdateMotionWorkWithPosition(scenFindMotionwork(YCPA_REN), nullptr, 3);
    UpdateMotionWorkWithPosition(scenFindMotionwork(YCPA_REN), nullptr, 0);
    mtwkSetRequestedRootMode(scenFindMotionwork(YCPA_REN), 3);
    SetMotionFrameTime(scenFindMotionwork(YCPA_REN), 7, MotionRegion::LowerBody);
    EV_SetRefBoardParamsFor(YCPA_REN, 1);
    EV_SetCharaControllerFlags(YCPA_REN, UpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(YCPA_REN, 0, NpcCharacterControllerState::Disabled);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::PackDirectory)),
        reinterpret_cast<std::uintptr_t>("YCPAMAIN"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("YCPA"), 0);
    EV_FreeAuthorPak(1, "0235");
    for (shen_name character : {YCPA_BASE, YCPA_RYOD, YCPA_YUAB, YCPA_RENB})
        CreateEventCharacter(CharacterCreationHighModel, nullptr, character);
    Vector3 center;
    EV_GetCharaPosition(YCPA_RYO, CHARACTER_TRANSLATION_ROOT, &center, EventTransformFlags::World);
    npcSetAreaName(SHEN_NAME(aera), &center, 5, 1);
    HLib::Task *const task = HLib::EnqueueTaskWithoutParameter(YcpaAdviceCallback, 4, 0x0b, static_cast<std::int32_t>(YCPA_TASK.name));
    HLib::SetupTimedTask(task, 3);
}

void YcpaAdviceCallback(void *) {
    EV_ResetFreeTalkFlags();
    Vector3 player, companion;
    Vector3i rotation;
    EV_GetCharaPosition(YCPA_RYO, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(YCPA_RYO, &rotation, EventTransformFlags::World);
    EV_GetCharaPosition(YCPA_REN, CHARACTER_TRANSLATION_ROOT, &companion, EventTransformFlags::World);
    UpdateMotionWorkWithPosition(scenFindMotionwork(YCPA_RYO), nullptr, 3);
    UpdateMotionWorkWithPosition(scenFindMotionwork(YCPA_RYO), nullptr, 0);
    for (std::int32_t index = 0; index < 24; ++index) {
        Vector3 offset{.x = 0, .y = 0, .z = 2};
        rotation.x = 0;
        rotation.z = 0;
        rotation.y = static_cast<std::int32_t>((adviceDirections[index] * 8192.0f) / 45.0f);
        ApplyScenarioTranslationAndRotation(&player, &rotation, &offset);
        companion = offset;
        Vector3 intersection, corrected;
        if (!FindNearestRayIntersection(&intersection, &player, &companion) && !FindNearestRayIntersection(&intersection, &companion, &player) &&
            !RaycastInWorldGrid(corrected, companion, 0.5f))
            break;
    }
    EVT_AT_SetPos(YCPA_REN, YCPA_INVALID, companion, 0);
    EVT_AT_SetAngY(YCPA_REN, YCPA_INVALID, 0, 0);
    const std::int16_t angle = InverseFixedAtan2(player.x - companion.x, player.z - companion.z);
    EVT_AT_SetAngY(YCPA_REN, YCPA_INVALID, static_cast<float>(angle) / 182.0444488525390625f, 0);
    YcpaSetCharacterMidpoint(YCPA_REN, YCPA_RYO);
    EVT_AT_SetAngY(YCPA_RYOD, YCPA_REN, 0, 0);
    IdleAdviceActor(YCPA_RYO);
    IdleAdviceActor(YCPA_REN);
    if (ycpaEvent == 30) {
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        {
            EventSetPosition cue{.cleanup = 0, .character = YCPA_RENB, .parent = YCPA_REN, .position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .delay = 0.0f};
            EVT_AT_SetPosRot(&cue);
        }
        {
            EventSetAngle cue{.cleanup = 0, .character = YCPA_RENB, .parent = YCPA_REN, .degrees = 0.0f, .delay = 0.0f};
            EVT_AT_SetAngY_Callback(&cue);
        }
        {
            EventSetPosition cue{.cleanup = 0, .character = YCPA_BASE, .parent = YCPA_RYO, .position = {.x = -4.0f, .y = 1.8f, .z = -4.0f}, .delay = 0.0f};
            EVT_AT_SetPosRot(&cue);
        }
        {
            EventSetAngle cue{.cleanup = 0, .character = YCPA_BASE, .parent = YCPA_RYO, .degrees = 0.0f, .delay = 0.0f};
            EVT_AT_SetAngY_Callback(&cue);
        }
        {
            EventSetPosition cue{.cleanup = 0, .character = YCPA_YUAB, .parent = YCPA_RYO, .position = {.x = 4.0f, .y = 1.8f, .z = -4.0f}, .delay = 0.0f};
            EVT_AT_SetPosRot(&cue);
        }
        {
            EventSetAngle cue{.cleanup = 0, .character = YCPA_YUAB, .parent = YCPA_RYO, .degrees = 0.0f, .delay = 0.0f};
            EVT_AT_SetAngY_Callback(&cue);
        }
        {
            EventSetPoint *const cue = static_cast<EventSetPoint *>(YcpaQueueRecord(EVT_AT_SetPoint_Callback, sizeof(EventSetPoint)));
            cue->character = YCPA_RYO;
            cue->target = YCPA_BASE;
            cue->targetBone = -1;
            cue->trackingMode = 1;
            cue->positionMode = 1;
            cue->duration = 0;
            cue->flags = YCPA_POINT_SINGLE_UPDATE;
            cue->frame = 0;
            cue->cleanup = 1;
            cue->delay = 1.0f;
        }
        {
            EventSetPoint *const cue = static_cast<EventSetPoint *>(YcpaQueueRecord(EVT_AT_SetPoint_Callback, sizeof(EventSetPoint)));
            cue->character = YCPA_RYO;
            cue->target = YCPA_YUAB;
            cue->targetBone = -1;
            cue->trackingMode = 1;
            cue->positionMode = 1;
            cue->duration = 0;
            cue->flags = YCPA_POINT_SINGLE_UPDATE;
            cue->frame = 0;
            cue->cleanup = 1;
            cue->delay = 45.0f;
        }
        {
            EventSetPoint *const cue = static_cast<EventSetPoint *>(YcpaQueueRecord(EVT_AT_SetPoint_Callback, sizeof(EventSetPoint)));
            cue->character = YCPA_RYO;
            cue->target = YCPA_BASE;
            cue->targetBone = -1;
            cue->trackingMode = 1;
            cue->positionMode = 1;
            cue->duration = 0;
            cue->flags = YCPA_POINT_SINGLE_UPDATE;
            cue->frame = 0;
            cue->cleanup = 1;
            cue->delay = 90.0f;
        }
        {
            EventSetPoint *const cue = static_cast<EventSetPoint *>(YcpaQueueRecord(EVT_AT_SetPoint_Callback, sizeof(EventSetPoint)));
            cue->character = YCPA_RYO;
            cue->target = shen_name{};
            cue->targetBone = 0;
            cue->trackingMode = 3;
            cue->positionMode = 0;
            cue->duration = 0;
            cue->flags = YCPA_POINT_STOP;
            cue->frame = 0;
            cue->cleanup = 1;
            cue->delay = 120.0f;
        }
        EV_DisableCharacterDisplay(YCPA_REN);
        {
            StepObjectDisplayWork *const cue = static_cast<StepObjectDisplayWork *>(YcpaQueueRecord(EVT_AT_ObjectDisp_Callback, sizeof(StepObjectDisplayWork)));
            cue->cleanup = 1;
            cue->character = YCPA_REN;
            cue->mode = 0;
            cue->delay = 120.0f;
        }
        {
            EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
            cue->cleanup = 1;
            cue->camera = 9100;
            cue->eye = YCPA_RYO;
            cue->target = YCPA_RYO;
            cue->context = 0;
            cue->reserved11[0] = std::byte{0};
            cue->reserved11[1] = std::byte{0};
            cue->reserved11[2] = std::byte{0};
            cue->angle = 0.0f;
            cue->delay = 1.0f;
        }
        {
            EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
            cue->cleanup = 1;
            cue->camera = 9101;
            cue->eye = YCPA_REN;
            cue->target = YCPA_REN;
            cue->context = 0;
            cue->reserved11[0] = std::byte{0};
            cue->reserved11[1] = std::byte{0};
            cue->reserved11[2] = std::byte{0};
            cue->angle = 0.0f;
            cue->delay = 120.0f;
        }
        {
            EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
            cue->cleanup = 1;
            cue->camera = 9102;
            cue->eye = YCPA_RYO;
            cue->target = YCPA_RYO;
            cue->context = 0;
            cue->reserved11[0] = std::byte{0};
            cue->reserved11[1] = std::byte{0};
            cue->reserved11[2] = std::byte{0};
            cue->angle = 0.0f;
            cue->delay = 240.0f;
        }
        const std::int32_t choice = shenGetRandomInt32(3);
        std::int32_t delay = 0;
        if (!choice) {
            EVT_AT_SetVoice("0235A021", 60); // @text EN: "Lost her?" | JA: "見失った…？"
            EVT_AT_SetVoice("0235B034", 120); // @text EN: "What?\nYou're so damn slow!" | JA: "なんだとぉ！\nトロくせえんだよ、てめぇ！"
            EVT_AT_SetVoice("0235A022", 240); // @text EN: "Sorry=@" | JA: "すまない…"
            delay = 320;
        } else if (choice == 1) {
            EVT_AT_SetVoice("0235A030", 0);
            EVT_AT_SetVoice("0235A023", 60); // @text EN: "Yuan's not here!" | JA: "…ユアンがいない！"
            EVT_AT_SetVoice("0235B035", 120); // @text EN: "What?\nDid you lose her?" | JA: "なんだとぉ？\nてめぇ見失ったのか？"
            EVT_AT_SetVoice("0235A024", 240); // @text EN: "Damn it." | JA: "くそっ…"
            delay = 320;
        } else if (choice == 2) {
            EVT_AT_SetVoice("0235A025", 60); // @text EN: "I can't find Yuan!" | JA: "…ユアンが見あたらない！"
            EVT_AT_SetVoice("0235B036", 120); // @text EN: "Fool!\nYou lost her." | JA: "…バカが！\n見失いやがったな…"
            EVT_AT_SetVoice("0235A026", 240); // @text EN: "Damn." | JA: "ちくしょう…"
            delay = 320;
        }
        YcpaFilm(60, 0, 0, 0, 0, 255, 0, 0, 0, delay - 60);
        HLib::SetCurrentTaskTo(YcpaChangeAreaAfterAdvice);
        HLib::SetupTaskDelay(delay);
        return;
    }
    if (EV_GetShenmueFlag(0x319)) {
        {
            EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
            cue->cleanup = 1;
            cue->camera = 9000;
            cue->eye = YCPA_REN;
            cue->target = YCPA_REN;
            cue->context = 0;
            cue->reserved11[0] = std::byte{0};
            cue->reserved11[1] = std::byte{0};
            cue->reserved11[2] = std::byte{0};
            cue->angle = 0.0f;
            cue->delay = 1.0f;
        }
        EV_stepSetFlags(0, 0x319);
        EVT_AT_SetVoice("0235B065", 0); // @text EN: "Hey! You're getting too close!\nStay back!" | JA: "おい！　近づきすぎだ！\n離れろっ！"
    } else {
        {
            EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
            cue->cleanup = 1;
            cue->camera = 9000;
            cue->eye = YCPA_REN;
            cue->target = YCPA_REN;
            cue->context = 0;
            cue->reserved11[0] = std::byte{0};
            cue->reserved11[1] = std::byte{0};
            cue->reserved11[2] = std::byte{0};
            cue->angle = 0.0f;
            cue->delay = 1.0f;
        }
        EV_stepSetFlags(0, 0x31a);
        EVT_AT_SetVoice("0235B066", 0); // @text EN: "You're getting too close!\nAre you that stupid?" | JA: "近づきすぎだよ！\nわかんねーのか！？"
    }
    HLib::SetCurrentTaskTo(YcpaEndAdviceCallback);
    HLib::SetupTaskDelay(100);
}
} // namespace shenmue::s2::areas
