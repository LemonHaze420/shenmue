#include "s2/areas/area_ycpa.hpp"
#include "s2/camera.hpp"
#include "s2/motion.hpp"
namespace shenmue::s2::areas {

std::int32_t YcpaDiscovery() {
    const bool reverse = YcpaSelectDiscoveryPlacement(false);
    {
        EventSetMotion cue{.cleanup = 0,
            .character = YCPA_RYO,
            .animation = 10001,
            .frame = 45.0f,
            .speed = 1.0f,
            .flags = reverse ? YCPA_MOTION_NO_BLEND | YCPA_MOTION_MIRRORED : YCPA_MOTION_NO_BLEND,
            .delay = 0.0f};
        EVT_AT_SetMot_Callback(&cue);
    }
    (void)GetMotionFrameCount(10001);
    {
        EventSetMotion cue{.cleanup = 0,
            .character = YCPA_REN,
            .animation = 10032,
            .frame = 105.0f,
            .speed = 1.0f,
            .flags = reverse ? YCPA_MOTION_NO_BLEND | YCPA_MOTION_MIRRORED : YCPA_MOTION_NO_BLEND,
            .delay = 0.0f};
        EVT_AT_SetMot_Callback(&cue);
    }
    (void)GetMotionFrameCount(10032);
    {
        EventSetMotion cue{.cleanup = 0,
            .character = YCPA_YUA,
            .animation = 10013,
            .frame = 300.0f,
            .speed = 1.0f,
            .flags = reverse ? YCPA_MOTION_NO_BLEND | YCPA_MOTION_MIRRORED : YCPA_MOTION_NO_BLEND,
            .delay = 0.0f};
        EVT_AT_SetMot_Callback(&cue);
    }
    (void)GetMotionFrameCount(10013);
    {
        EventSetMotion *const cue = static_cast<EventSetMotion *>(YcpaQueueRecord(EVT_AT_SetMot_Callback, sizeof(EventSetMotion)));
        cue->cleanup = 1;
        cue->character = YCPA_YUA;
        cue->animation = 10013;
        cue->frame = 680.0f;
        cue->speed = 1.0f;
        cue->flags = reverse ? YCPA_MOTION_NO_BLEND | YCPA_MOTION_MIRRORED : YCPA_MOTION_NO_BLEND;
        cue->delay = 520.0f;
    }
    (void)GetMotionFrameCount(10013);
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
        EventSetPosition cue{.cleanup = 0, .character = YCPA_RENB, .parent = YCPA_REN, .position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .delay = 0.0f};
        EVT_AT_SetPosRot(&cue);
    }
    {
        EventSetAngle cue{.cleanup = 0, .character = YCPA_RENB, .parent = YCPA_REN, .degrees = 0.0f, .delay = 0.0f};
        EVT_AT_SetAngY_Callback(&cue);
    }
    {
        EventSetCamera cue{.cleanup = 0,
            .camera = 2000,
            .eye = YCPA_YUAB,
            .target = YCPA_YUAB,
            .context = 0,
            .reserved11 = {std::byte{0}, std::byte{0}, std::byte{0}},
            .angle = reverse ? -0.0f : 0.0f,
            .delay = 0.0f};
        EVT_AT_SetCam_Callback(&cue);
    }
    camCtrlDisableFilmBars();
    {
        EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
        cue->cleanup = 1;
        cue->camera = 2001;
        cue->eye = YCPA_RYOB;
        cue->target = YCPA_RENB;
        cue->context = 0;
        cue->reserved11[0] = std::byte{0};
        cue->reserved11[1] = std::byte{0};
        cue->reserved11[2] = std::byte{0};
        cue->angle = reverse ? -0.0f : 0.0f;
        cue->delay = 80.0f;
    }
    {
        EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
        cue->cleanup = 1;
        cue->camera = 2002;
        cue->eye = YCPA_RENB;
        cue->target = YCPA_YUAB;
        cue->context = 0;
        cue->reserved11[0] = std::byte{0};
        cue->reserved11[1] = std::byte{0};
        cue->reserved11[2] = std::byte{0};
        cue->angle = reverse ? -0.0f : 0.0f;
        cue->delay = 180.0f;
    }
    {
        EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
        cue->cleanup = 1;
        cue->camera = 2003;
        cue->eye = YCPA_RENB;
        cue->target = YCPA_RENB;
        cue->context = 0;
        cue->reserved11[0] = std::byte{0};
        cue->reserved11[1] = std::byte{0};
        cue->reserved11[2] = std::byte{0};
        cue->angle = reverse ? -0.0f : 0.0f;
        cue->delay = 400.0f;
    }
    {
        EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
        cue->cleanup = 1;
        cue->camera = 2004;
        cue->eye = YCPA_YUAB;
        cue->target = YCPA_YUAB;
        cue->context = 0;
        cue->reserved11[0] = std::byte{0};
        cue->reserved11[1] = std::byte{0};
        cue->reserved11[2] = std::byte{0};
        cue->angle = reverse ? -0.0f : 0.0f;
        cue->delay = 520.0f;
    }
    EVT_AT_SetVoice("0235C029", 0); // @text EN: "Who's been following me?" | JA: "さっきから、私の後を\nつけてるのは誰！？"
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 80.0f;
        std::memcpy(cue->voice, "0235A031", sizeof("0235A031"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 110.0f;
        std::memcpy(cue->voice, "0235B045", sizeof("0235B045"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 180.0f;
        std::memcpy(cue->voice, "0235C030", sizeof("0235C030"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 280.0f;
        std::memcpy(cue->voice, "0235A032", sizeof("0235A032"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 320.0f;
        std::memcpy(cue->voice, "0235C031", sizeof("0235C031"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 400.0f;
        std::memcpy(cue->voice, "0235B046", sizeof("0235B046"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 460.0f;
        std::memcpy(cue->voice, "0235B047", sizeof("0235B047"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 520.0f;
        std::memcpy(cue->voice, "0235C032", sizeof("0235C032"));
    }
    YcpaFilm(60, 0, 0, 0, 0, 255, 0, 0, 0, 600);
    YcpaFilm(20, 255, 0, 0, 0, 150, 0, 0, 0, 690);
    YcpaFilm(90, 150, 0, 0, 0, 255, 0, 0, 0, 710);
    YcpaCamera(2005, SHEN_NAME(RYOB), SHEN_NAME(RYOB), 0, reverse ? -0.0f : 0.0f, 630);
    YcpaCamera(2505, SHEN_NAME(YUAB), SHEN_NAME(YUAB), 0, reverse ? -0.0f : 0.0f, 690);
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 3249;
        cue->delay = 720.0f;
    }
    EVT_AT_SetVoice("0235A033", 630); // @text EN: "Damn!" | JA: "くそっ…！"
    EVT_AT_SetVoice("0235C033", 690); // @text EN: "Some nerve you have.\nFollowing me like this." | JA: "私を尾行しようなんて、\nたいした度胸ねえ…"
    EVT_AT_SetVoice("0235B048", 790); // @text EN: "Damn you!" | JA: "ちっくしょう！　てめえら！"
    EVT_AT_SetVoice("0235B049", 860); // @text EN: " " | JA: "うん！"
    EVT_AT_SetVoice("0235A034", 880); // @text EN: " " | JA: "うわっ！"
    EVT_AT_SetVoice("0235B050", 940); // @text EN: "Hey!" | JA: "おいっ！？"
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2398;
        cue->delay = 760.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2403;
        cue->delay = 795.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2400;
        cue->delay = 1595.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2401;
        cue->delay = 805.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2398;
        cue->delay = 805.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2404;
        cue->delay = 810.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2401;
        cue->delay = 815.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2397;
        cue->delay = 830.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2398;
        cue->delay = 830.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2403;
        cue->delay = 840.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2402;
        cue->delay = 845.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2397;
        cue->delay = 860.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2402;
        cue->delay = 870.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2397;
        cue->delay = 880.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2399;
        cue->delay = 885.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2398;
        cue->delay = 885.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2400;
        cue->delay = 905.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2404;
        cue->delay = 910.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2401;
        cue->delay = 910.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2400;
        cue->delay = 920.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2397;
        cue->delay = 925.0f;
    }
    if (reverse) {
        {
            EventCling *const cue = static_cast<EventCling *>(YcpaQueueRecord(EVT_AT_Cling, sizeof(EventCling)));
            cue->position.x = 0.0f;
            cue->position.y = 0.0f;
            cue->cleanup = 1;
            cue->target = YCPA_TRSV;
            cue->character = YCPA_YUA;
            cue->mode = YCPA_ATTACH_OBJECT;
            cue->delay = 520.0f;
            cue->bone = 21;
            cue->position.z = 0.05f;
            cue->rotation.x = 0;
            cue->rotation.y = 0;
            cue->rotation.z = 53248;
        }
    } else {
        {
            EventCling *const cue = static_cast<EventCling *>(YcpaQueueRecord(EVT_AT_Cling, sizeof(EventCling)));
            cue->position.x = 0.0f;
            cue->position.y = 0.0f;
            cue->cleanup = 1;
            cue->target = YCPA_TRSV;
            cue->character = YCPA_YUA;
            cue->mode = YCPA_ATTACH_OBJECT;
            cue->delay = 520.0f;
            cue->bone = 14;
            cue->position.z = -0.05f;
            cue->rotation.x = 0;
            cue->rotation.y = 32768;
            cue->rotation.z = 53248;
        }
    }
    {
        StepObjectDisplayWork *const cue = static_cast<StepObjectDisplayWork *>(YcpaQueueRecord(EVT_AT_ObjectDisp_Callback, sizeof(StepObjectDisplayWork)));
        cue->cleanup = 1;
        cue->character = YCPA_TRSV;
        cue->mode = 1;
        cue->delay = 690.0f;
    }
    {
        EventCling *const cue = static_cast<EventCling *>(YcpaQueueRecord(EVT_AT_Cling, sizeof(EventCling)));
        cue->cleanup = 1;
        cue->target = YCPA_TRSV;
        cue->character = shen_name{};
        cue->mode = YCPA_DETACH_OBJECT;
        cue->delay = 960.0f;
    }
    return 960;
}


std::int32_t YcpaDiscoveryAgain() {
    const bool reverse = YcpaSelectDiscoveryPlacement(true);
    {
        EventSetMotion cue{.cleanup = 0,
            .character = YCPA_RYO,
            .animation = 10001,
            .frame = 300.0f,
            .speed = 1.0f,
            .flags = reverse ? YCPA_MOTION_NO_BLEND | YCPA_MOTION_MIRRORED : YCPA_MOTION_NO_BLEND,
            .delay = 0.0f};
        EVT_AT_SetMot_Callback(&cue);
    }
    (void)GetMotionFrameCount(10001);
    {
        EventSetMotion cue{.cleanup = 0,
            .character = YCPA_REN,
            .animation = 10032,
            .frame = 220.0f,
            .speed = 1.0f,
            .flags = reverse ? YCPA_MOTION_NO_BLEND | YCPA_MOTION_MIRRORED : YCPA_MOTION_NO_BLEND,
            .delay = 0.0f};
        EVT_AT_SetMot_Callback(&cue);
    }
    (void)GetMotionFrameCount(10032);
    {
        EventSetMotion cue{.cleanup = 0,
            .character = YCPA_YUA,
            .animation = 10013,
            .frame = 380.0f,
            .speed = 1.0f,
            .flags = reverse ? YCPA_MOTION_NO_BLEND | YCPA_MOTION_MIRRORED : YCPA_MOTION_NO_BLEND,
            .delay = 0.0f};
        EVT_AT_SetMot_Callback(&cue);
    }
    (void)GetMotionFrameCount(10013);
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
        EventSetPosition cue{.cleanup = 0, .character = YCPA_RENB, .parent = YCPA_REN, .position = {.x = 0.0f, .y = 0.0f, .z = 0.0f}, .delay = 0.0f};
        EVT_AT_SetPosRot(&cue);
    }
    {
        EventSetAngle cue{.cleanup = 0, .character = YCPA_RENB, .parent = YCPA_REN, .degrees = 0.0f, .delay = 0.0f};
        EVT_AT_SetAngY_Callback(&cue);
    }
    YcpaSetCharacterMidpoint(YCPA_RYO, YCPA_REN);
    {
        EventSetCamera cue{.cleanup = 0,
            .camera = 2500,
            .eye = YCPA_YUAB,
            .target = YCPA_YUAB,
            .context = 0,
            .reserved11 = {std::byte{0}, std::byte{0}, std::byte{0}},
            .angle = reverse ? -0.0f : 0.0f,
            .delay = 0.0f};
        EVT_AT_SetCam_Callback(&cue);
    }
    camCtrlDisableFilmBars();
    {
        EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
        cue->cleanup = 1;
        cue->camera = 2501;
        cue->eye = YCPA_RYOB;
        cue->target = YCPA_RYOB;
        cue->context = 0;
        cue->reserved11[0] = std::byte{0};
        cue->reserved11[1] = std::byte{0};
        cue->reserved11[2] = std::byte{0};
        cue->angle = reverse ? -0.0f : 0.0f;
        cue->delay = 100.0f;
    }
    {
        EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
        cue->cleanup = 1;
        cue->camera = 2502;
        cue->eye = YCPA_RENB;
        cue->target = YCPA_RENB;
        cue->context = 0;
        cue->reserved11[0] = std::byte{0};
        cue->reserved11[1] = std::byte{0};
        cue->reserved11[2] = std::byte{0};
        cue->angle = reverse ? -0.0f : 0.0f;
        cue->delay = 220.0f;
    }
    {
        EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
        cue->cleanup = 1;
        cue->camera = 2503;
        cue->eye = YCPA_YUAB;
        cue->target = YCPA_YUAB;
        cue->context = 0;
        cue->reserved11[0] = std::byte{0};
        cue->reserved11[1] = std::byte{0};
        cue->reserved11[2] = std::byte{0};
        cue->angle = reverse ? -0.0f : 0.0f;
        cue->delay = 340.0f;
    }
    EVT_AT_SetVoice("0235C030", 0); // @text EN: "I've seen you somewhere=@" | JA: "あーら…どこかで見た顔ねえ…"
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 100.0f;
        std::memcpy(cue->voice, "0235A032", sizeof("0235A032"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 140.0f;
        std::memcpy(cue->voice, "0235C031", sizeof("0235C031"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 220.0f;
        std::memcpy(cue->voice, "0235B046", sizeof("0235B046"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 280.0f;
        std::memcpy(cue->voice, "0235B047", sizeof("0235B047"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 340.0f;
        std::memcpy(cue->voice, "0235C032", sizeof("0235C032"));
    }
    YcpaFilm(60, 0, 0, 0, 0, 255, 0, 0, 0, 420);
    YcpaFilm(20, 255, 0, 0, 0, 150, 0, 0, 0, 510);
    YcpaFilm(90, 150, 0, 0, 0, 255, 0, 0, 0, 530);
    YcpaCamera(2005, SHEN_NAME(RYOB), SHEN_NAME(RYOB), 0, reverse ? -0.0f : 0.0f, 450);
    YcpaCamera(2504, SHEN_NAME(YUAB), SHEN_NAME(YUAB), 0, reverse ? -0.0f : 0.0f, 510);
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 3249;
        cue->delay = 540.0f;
    }
    EVT_AT_SetVoice("0235A033", 450); // @text EN: "Damn!" | JA: "くそっ…！"
    EVT_AT_SetVoice("0235C033", 510); // @text EN: "Some nerve you have.\nFollowing me like this." | JA: "私を尾行しようなんて、\nたいした度胸ねえ…"
    EVT_AT_SetVoice("0235B048", 610); // @text EN: "Damn you!" | JA: "ちっくしょう！　てめえら！"
    EVT_AT_SetVoice("0235B049", 680); // @text EN: " " | JA: "うん！"
    EVT_AT_SetVoice("0235A034", 700); // @text EN: " " | JA: "うわっ！"
    EVT_AT_SetVoice("0235B050", 760); // @text EN: "Hey!" | JA: "おいっ！？"
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2398;
        cue->delay = 580.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2403;
        cue->delay = 615.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2400;
        cue->delay = 1415.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2401;
        cue->delay = 625.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2398;
        cue->delay = 625.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2404;
        cue->delay = 630.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2401;
        cue->delay = 635.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2397;
        cue->delay = 650.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2398;
        cue->delay = 650.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2403;
        cue->delay = 660.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2402;
        cue->delay = 665.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2397;
        cue->delay = 680.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2402;
        cue->delay = 690.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2397;
        cue->delay = 700.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2399;
        cue->delay = 705.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2398;
        cue->delay = 705.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2400;
        cue->delay = 725.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2404;
        cue->delay = 730.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2401;
        cue->delay = 730.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2400;
        cue->delay = 740.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 2397;
        cue->delay = 745.0f;
    }
    if (reverse) {
        {
            EventCling *const cue = static_cast<EventCling *>(YcpaQueueRecord(EVT_AT_Cling, sizeof(EventCling)));
            cue->position.x = 0.0f;
            cue->position.y = 0.0f;
            cue->cleanup = 1;
            cue->target = YCPA_TRSV;
            cue->character = YCPA_YUA;
            cue->mode = YCPA_ATTACH_OBJECT;
            cue->delay = 340.0f;
            cue->bone = 21;
            cue->position.z = 0.05f;
            cue->rotation.x = 0;
            cue->rotation.y = 0;
            cue->rotation.z = 53248;
        }
    } else {
        {
            EventCling *const cue = static_cast<EventCling *>(YcpaQueueRecord(EVT_AT_Cling, sizeof(EventCling)));
            cue->position.x = 0.0f;
            cue->position.y = 0.0f;
            cue->cleanup = 1;
            cue->target = YCPA_TRSV;
            cue->character = YCPA_YUA;
            cue->mode = YCPA_ATTACH_OBJECT;
            cue->delay = 340.0f;
            cue->bone = 14;
            cue->position.z = -0.05f;
            cue->rotation.x = 0;
            cue->rotation.y = 32768;
            cue->rotation.z = 53248;
        }
    }
    {
        StepObjectDisplayWork *const cue = static_cast<StepObjectDisplayWork *>(YcpaQueueRecord(EVT_AT_ObjectDisp_Callback, sizeof(StepObjectDisplayWork)));
        cue->cleanup = 1;
        cue->character = YCPA_TRSV;
        cue->mode = 1;
        cue->delay = 510.0f;
    }
    {
        EventCling *const cue = static_cast<EventCling *>(YcpaQueueRecord(EVT_AT_Cling, sizeof(EventCling)));
        cue->cleanup = 1;
        cue->target = YCPA_TRSV;
        cue->character = shen_name{};
        cue->mode = YCPA_DETACH_OBJECT;
        cue->delay = 780.0f;
    }
    return 780;
}

} // namespace shenmue::s2::areas
