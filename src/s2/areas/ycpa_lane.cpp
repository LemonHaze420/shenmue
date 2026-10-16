#include "s2/areas/area_ycpa.hpp"
#include "s2/character.hpp"
#include "s2/face.hpp"
#include "s2/camera.hpp"
#include "s2/motion.hpp"
namespace shenmue::s2::areas {

std::int32_t YcpaLaneConversation() {
    if (ycpaEvent == 7) {
        {
            EventSetPosition cue{
                .cleanup = 0, .character = YCPA_RYO, .parent = YCPA_INVALID, .position = {.x = 278.8f, .y = 100.0f, .z = 89.6f}, .delay = 0.0f};
            EVT_AT_SetPosRot(&cue);
        }
    }
    if (ycpaEvent != 7) {
        {
            EventSetPosition cue{
                .cleanup = 0, .character = YCPA_RYO, .parent = YCPA_INVALID, .position = {.x = 282.9f, .y = 100.0f, .z = 108.1f}, .delay = 0.0f};
            EVT_AT_SetPosRot(&cue);
        }
    }
    {
        EventSetAngle cue{.cleanup = 0, .character = YCPA_RYO, .parent = YCPA_INVALID, .degrees = ycpaEvent == 7 ? 22.5f : 122.5f, .delay = 0.0f};
        EVT_AT_SetAngY_Callback(&cue);
    }
    {
        EventSetPosition cue{.cleanup = 0, .character = YCPA_REN, .parent = YCPA_RYO, .position = {.x = -0.6f, .y = 0.0f, .z = -0.6f}, .delay = 0.0f};
        EVT_AT_SetPosRot(&cue);
    }
    {
        EventSetAngle cue{.cleanup = 0, .character = YCPA_REN, .parent = YCPA_RYO, .degrees = 0.0f, .delay = 0.0f};
        EVT_AT_SetAngY_Callback(&cue);
    }
    {
        EventSetPosition cue{.cleanup = 0, .character = YCPA_YUA, .parent = YCPA_INVALID, .position = {.x = 286.1f, .y = 100.0f, .z = 92.1f}, .delay = 0.0f};
        EVT_AT_SetPosRot(&cue);
    }
    {
        EventSetAngle cue{.cleanup = 0, .character = YCPA_YUA, .parent = YCPA_INVALID, .degrees = 292.5f, .delay = 0.0f};
        EVT_AT_SetAngY_Callback(&cue);
    }
    EVT_AT_SetPos(
        SHEN_NAME(YUA_), YCPA_INVALID, Vector3{.x = static_cast<float>(286.79999), .y = static_cast<float>(100.0), .z = static_cast<float>(92.900002)}, 1800);
    {
        EventSetAngle *const cue = static_cast<EventSetAngle *>(YcpaQueueRecord(EVT_AT_SetAngY_Callback, sizeof(EventSetAngle)));
        cue->cleanup = 1;
        cue->character = YCPA_YUA;
        cue->parent = YCPA_INVALID;
        cue->degrees = 225.0f;
        cue->delay = 1800.0f;
    }
    {
        EventSetPosition cue{.cleanup = 0, .character = YCPA_RYW, .parent = YCPA_INVALID, .position = {.x = 286.4f, .y = 100.0f, .z = 90.2f}, .delay = 0.0f};
        EVT_AT_SetPosRot(&cue);
    }
    {
        EventSetAngle cue{.cleanup = 0, .character = YCPA_RYW, .parent = YCPA_INVALID, .degrees = 0.0f, .delay = 0.0f};
        EVT_AT_SetAngY_Callback(&cue);
    }
    {
        EventSetPosition cue{.cleanup = 0, .character = YCPA_RYOB, .parent = YCPA_RYO, .position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .delay = 0.0f};
        EVT_AT_SetPosRot(&cue);
    }
    {
        EventSetAngle cue{.cleanup = 0, .character = YCPA_RYOB, .parent = YCPA_RYO, .degrees = 0.0f, .delay = 0.0f};
        EVT_AT_SetAngY_Callback(&cue);
    }
    {
        EventSetPosition cue{.cleanup = 0, .character = YCPA_YUAB, .parent = YCPA_YUA, .position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .delay = 0.0f};
        EVT_AT_SetPosRot(&cue);
    }
    {
        EventSetAngle cue{.cleanup = 0, .character = YCPA_YUAB, .parent = YCPA_YUA, .degrees = 0.0f, .delay = 0.0f};
        EVT_AT_SetAngY_Callback(&cue);
    }
    {
        EventSetMotion cue{.cleanup = 0, .character = YCPA_RYO, .animation = 9998, .frame = 0.0f, .speed = 1.0f, .flags = YCPA_MOTION_NO_BLEND, .delay = 0.0f};
        EVT_AT_SetMot_Callback(&cue);
    }
    (void)GetMotionFrameCount(9998);
    {
        EventSetMotion cue{.cleanup = 0, .character = YCPA_REN, .animation = 9991, .frame = 0.0f, .speed = 1.0f, .flags = YCPA_MOTION_NO_BLEND, .delay = 0.0f};
        EVT_AT_SetMot_Callback(&cue);
    }
    (void)GetMotionFrameCount(9991);
    {
        EventSetMotion *const cue = static_cast<EventSetMotion *>(YcpaQueueRecord(EVT_AT_SetMot_Callback, sizeof(EventSetMotion)));
        cue->cleanup = 1;
        cue->character = YCPA_RYO;
        cue->animation = 9997;
        cue->frame = 0.0f;
        cue->speed = 1.0f;
        cue->flags = YCPA_MOTION_NO_BLEND;
        cue->delay = 1060.0f;
    }
    (void)GetMotionFrameCount(9997);
    {
        EventSetMotion *const cue = static_cast<EventSetMotion *>(YcpaQueueRecord(EVT_AT_SetMot_Callback, sizeof(EventSetMotion)));
        cue->cleanup = 1;
        cue->character = YCPA_REN;
        cue->animation = 9990;
        cue->frame = 0.0f;
        cue->speed = 1.0f;
        cue->flags = YCPA_MOTION_NO_BLEND;
        cue->delay = 1060.0f;
    }
    (void)GetMotionFrameCount(9990);
    {
        EventSetMotion *const cue = static_cast<EventSetMotion *>(YcpaQueueRecord(EVT_AT_SetMot_Callback, sizeof(EventSetMotion)));
        cue->cleanup = 1;
        cue->character = YCPA_RYO;
        cue->animation = 9997;
        cue->frame = 950.0f;
        cue->speed = 1.0f;
        cue->flags = YCPA_MOTION_NO_BLEND | YCPA_MOTION_MIRRORED;
        cue->delay = 1800.0f;
    }
    (void)GetMotionFrameCount(9997);
    {
        EventSetMotion *const cue = static_cast<EventSetMotion *>(YcpaQueueRecord(EVT_AT_SetMot_Callback, sizeof(EventSetMotion)));
        cue->cleanup = 1;
        cue->character = YCPA_REN;
        cue->animation = 9990;
        cue->frame = 950.0f;
        cue->speed = 1.0f;
        cue->flags = YCPA_MOTION_NO_BLEND | YCPA_MOTION_MIRRORED;
        cue->delay = 1800.0f;
    }
    (void)GetMotionFrameCount(9990);
    {
        EventSetMotion cue{.cleanup = 0, .character = YCPA_YUA, .animation = 10016, .frame = 0.0f, .speed = 1.0f, .flags = YCPA_MOTION_NO_BLEND, .delay = 0.0f};
        EVT_AT_SetMot_Callback(&cue);
    }
    (void)GetMotionFrameCount(10016);
    {
        EventSetMotion *const cue = static_cast<EventSetMotion *>(YcpaQueueRecord(EVT_AT_SetMot_Callback, sizeof(EventSetMotion)));
        cue->cleanup = 1;
        cue->character = YCPA_YUA;
        cue->animation = 10015;
        cue->frame = 0.0f;
        cue->speed = 0.4f;
        cue->flags = 0;
        cue->delay = 1640.0f;
    }
    (void)GetMotionFrameCount(10015);
    {
        EventSetMotion cue{.cleanup = 0, .character = YCPA_RYW, .animation = 10011, .frame = 0.0f, .speed = 1.0f, .flags = YCPA_MOTION_NO_BLEND, .delay = 0.0f};
        EVT_AT_SetMot_Callback(&cue);
    }
    (void)GetMotionFrameCount(10011);
    {
        EventSetMotion *const cue = static_cast<EventSetMotion *>(YcpaQueueRecord(EVT_AT_SetMot_Callback, sizeof(EventSetMotion)));
        cue->cleanup = 1;
        cue->character = YCPA_RYW;
        cue->animation = 10010;
        cue->frame = 0.0f;
        cue->speed = 0.4f;
        cue->flags = 0;
        cue->delay = 1640.0f;
    }
    (void)GetMotionFrameCount(10010);
    {
        EventSetMotion *const cue = static_cast<EventSetMotion *>(YcpaQueueRecord(EVT_AT_SetMot_Callback, sizeof(EventSetMotion)));
        cue->cleanup = 1;
        cue->character = YCPA_YUA;
        cue->animation = 10048;
        cue->frame = 0.0f;
        cue->speed = 1.0f;
        cue->flags = YCPA_MOTION_NO_BLEND;
        cue->delay = 1800.0f;
    }
    (void)GetMotionFrameCount(10048);
    if (ycpaEvent == 7) {
        {
            EventSetCamera cue{.cleanup = 0,
                .camera = 1500,
                .eye = YCPA_RYOB,
                .target = YCPA_RYOB,
                .context = 0,
                .reserved11 = {std::byte{0}, std::byte{0}, std::byte{0}},
                .angle = 0.0f,
                .delay = 0.0f};
            EVT_AT_SetCam_Callback(&cue);
        }
        camCtrlDisableFilmBars();
        {
            EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
            cue->cleanup = 1;
            cue->camera = 1501;
            cue->eye = YCPA_YUAB;
            cue->target = YCPA_YUAB;
            cue->context = 0;
            cue->reserved11[0] = std::byte{0};
            cue->reserved11[1] = std::byte{0};
            cue->reserved11[2] = std::byte{0};
            cue->angle = 0.0f;
            cue->delay = 300.0f;
        }
        {
            EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
            cue->cleanup = 1;
            cue->camera = 1502;
            cue->eye = YCPA_RYOB;
            cue->target = YCPA_RYOB;
            cue->context = 0;
            cue->reserved11[0] = std::byte{0};
            cue->reserved11[1] = std::byte{0};
            cue->reserved11[2] = std::byte{0};
            cue->angle = 0.0f;
            cue->delay = 1060.0f;
        }
        {
            EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
            cue->cleanup = 1;
            cue->camera = 1503;
            cue->eye = YCPA_RYOB;
            cue->target = YCPA_RYOB;
            cue->context = 0;
            cue->reserved11[0] = std::byte{0};
            cue->reserved11[1] = std::byte{0};
            cue->reserved11[2] = std::byte{0};
            cue->angle = 0.0f;
            cue->delay = 1500.0f;
        }
        {
            EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
            cue->camera = 1504;
            cue->delay = 1800.0f;
            cue->angle = 0.0f;
            cue->target = YCPA_RYOB;
            cue->context = 0;
            cue->reserved11[0] = std::byte{0};
            cue->reserved11[1] = std::byte{0};
            cue->reserved11[2] = std::byte{0};
            cue->eye = YCPA_RYOB;
            cue->cleanup = 1;
        }
    } else {
        {
            EventSetCamera cue{.cleanup = 0,
                .camera = 1600,
                .eye = YCPA_RYOB,
                .target = YCPA_RYOB,
                .context = 0,
                .reserved11 = {std::byte{0}, std::byte{0}, std::byte{0}},
                .angle = 0.0f,
                .delay = 0.0f};
            EVT_AT_SetCam_Callback(&cue);
        }
        camCtrlDisableFilmBars();
        {
            EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
            cue->cleanup = 1;
            cue->camera = 1601;
            cue->eye = YCPA_YUAB;
            cue->target = YCPA_YUAB;
            cue->context = 0;
            cue->reserved11[0] = std::byte{0};
            cue->reserved11[1] = std::byte{0};
            cue->reserved11[2] = std::byte{0};
            cue->angle = 0.0f;
            cue->delay = 300.0f;
        }
        {
            EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
            cue->cleanup = 1;
            cue->camera = 1602;
            cue->eye = YCPA_RYOB;
            cue->target = YCPA_RYOB;
            cue->context = 0;
            cue->reserved11[0] = std::byte{0};
            cue->reserved11[1] = std::byte{0};
            cue->reserved11[2] = std::byte{0};
            cue->angle = 0.0f;
            cue->delay = 1060.0f;
        }
        {
            EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
            cue->cleanup = 1;
            cue->camera = 1603;
            cue->eye = YCPA_YUAB;
            cue->target = YCPA_YUAB;
            cue->context = 0;
            cue->reserved11[0] = std::byte{0};
            cue->reserved11[1] = std::byte{0};
            cue->reserved11[2] = std::byte{0};
            cue->angle = 0.0f;
            cue->delay = 1500.0f;
        }
        {
            EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
            cue->camera = 1604;
            cue->delay = 1800.0f;
            cue->angle = 0.0f;
            cue->target = YCPA_RYOB;
            cue->context = 0;
            cue->reserved11[0] = std::byte{0};
            cue->reserved11[1] = std::byte{0};
            cue->reserved11[2] = std::byte{0};
            cue->eye = YCPA_RYOB;
            cue->cleanup = 1;
        }
    }
    EVT_AT_SetVoice("0235C037", 0); // @text EN: "What is this?\nIt looks so filthy!" | JA: "…なあに？　これ？\n汚らしいわねっ！"
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 80.0f;
        std::memcpy(cue->voice, "0235I001", sizeof("0235I001"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 180.0f;
        std::memcpy(cue->voice, "0235I002", sizeof("0235I002"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 300.0f;
        std::memcpy(cue->voice, "0235C038", sizeof("0235C038"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 380.0f;
        std::memcpy(cue->voice, "0235C039", sizeof("0235C039"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 480.0f;
        std::memcpy(cue->voice, "0235I003", sizeof("0235I003"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 600.0f;
        std::memcpy(cue->voice, "0235I004", sizeof("0235I004"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 680.0f;
        std::memcpy(cue->voice, "0235C040", sizeof("0235C040"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 760.0f;
        std::memcpy(cue->voice, "0235I005", sizeof("0235I005"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 820.0f;
        std::memcpy(cue->voice, "0235I006", sizeof("0235I006"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 920.0f;
        std::memcpy(cue->voice, "0235C041", sizeof("0235C041"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 1060.0f;
        std::memcpy(cue->voice, "0235B057", sizeof("0235B057"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 1180.0f;
        std::memcpy(cue->voice, "0235B058", sizeof("0235B058"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 1260.0f;
        std::memcpy(cue->voice, "0235A038", sizeof("0235A038"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 1320.0f;
        std::memcpy(cue->voice, "0235B059", sizeof("0235B059"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 1340.0f;
        std::memcpy(cue->voice, "0235A039", sizeof("0235A039"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 1400.0f;
        std::memcpy(cue->voice, "0235B060", sizeof("0235B060"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 1420.0f;
        std::memcpy(cue->voice, "0235B061", sizeof("0235B061"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 1460.0f;
        std::memcpy(cue->voice, "0235B056", sizeof("0235B056"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 1500.0f;
        std::memcpy(cue->voice, "0235C042", sizeof("0235C042"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 1560.0f;
        std::memcpy(cue->voice, "0235C043", sizeof("0235C043"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 1640.0f;
        std::memcpy(cue->voice, "0235I007", sizeof("0235I007"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 1860.0f;
        std::memcpy(cue->voice, "0235B062", sizeof("0235B062"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 1900.0f;
        std::memcpy(cue->voice, "0235A040", sizeof("0235A040"));
    }
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(YCPA_YUA), 2, 0, 1);
    return 1930;
}
} // namespace shenmue::s2::areas
