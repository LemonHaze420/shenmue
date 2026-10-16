#include "s2/areas/area_ycpa.hpp"
#include "s2/areas/ycpa_queued.hpp"
#include "s2/character.hpp"
#include "s2/camera.hpp"
#include "s2/hand.hpp"
#include "s2/motion.hpp"
#include "s2/sk_event.hpp"
#include "s2/elevator.hpp"
#include "s2/event_runtime.hpp"
namespace shenmue::s2::areas {

std::int32_t YcpaElevatorDeparture() {
    EV_EnableCharacterDisplay(SHEN_NAME(YUA_));
    {
        EventSetPosition cue{.cleanup = 0, .character = YCPA_RYO, .parent = YCPA_INVALID, .position = {.x = 256.1f, .y = 121.6f, .z = 205.0f}, .delay = 0.0f};
        EVT_AT_SetPosRot(&cue);
    }
    {
        EventSetAngle cue{.cleanup = 0, .character = YCPA_RYO, .parent = YCPA_INVALID, .degrees = 270.0f, .delay = 0.0f};
        EVT_AT_SetAngY_Callback(&cue);
    }
    {
        EventSetPosition cue{.cleanup = 0, .character = YCPA_REN, .parent = YCPA_RYO, .position = {.x = -0.5f, .y = 0.0f, .z = 0.6f}, .delay = 0.0f};
        EVT_AT_SetPosRot(&cue);
    }
    {
        EventSetAngle cue{.cleanup = 0, .character = YCPA_REN, .parent = YCPA_RYO, .degrees = 0.0f, .delay = 0.0f};
        EVT_AT_SetAngY_Callback(&cue);
    }
    {
        EventSetPosition cue{.cleanup = 0, .character = YCPA_YUA, .parent = YCPA_INVALID, .position = {.x = 266.5f, .y = 121.6f, .z = 207.9f}, .delay = 0.0f};
        EVT_AT_SetPosRot(&cue);
    }
    {
        EventSetAngle cue{.cleanup = 0, .character = YCPA_YUA, .parent = YCPA_INVALID, .degrees = 270.0f, .delay = 0.0f};
        EVT_AT_SetAngY_Callback(&cue);
    }
    {
        EventSetPosition cue{.cleanup = 0, .character = YCPA_BA6, .parent = YCPA_INVALID, .position = {.x = 265.6f, .y = 121.6f, .z = 206.9f}, .delay = 0.0f};
        EVT_AT_SetPosRot(&cue);
    }
    {
        EventSetAngle cue{.cleanup = 0, .character = YCPA_BA6, .parent = YCPA_INVALID, .degrees = 202.5f, .delay = 0.0f};
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
        EventSetMotion cue{.cleanup = 0, .character = YCPA_RYO, .animation = 9996, .frame = 0.0f, .speed = 1.0f, .flags = YCPA_MOTION_NO_BLEND, .delay = 0.0f};
        EVT_AT_SetMot_Callback(&cue);
    }
    (void)GetMotionFrameCount(9996);
    {
        EventSetMotion cue{.cleanup = 0, .character = YCPA_REN, .animation = 9989, .frame = 0.0f, .speed = 1.0f, .flags = YCPA_MOTION_NO_BLEND, .delay = 0.0f};
        EVT_AT_SetMot_Callback(&cue);
    }
    (void)GetMotionFrameCount(9989);
    {
        EventSetMotion cue{.cleanup = 0,
            .character = YCPA_YUA,
            .animation = 10006,
            .frame = 30.0f,
            .speed = 1.0f,
            .flags = YCPA_MOTION_NO_BLEND | YCPA_MOTION_MIRRORED,
            .delay = 0.0f};
        EVT_AT_SetMot_Callback(&cue);
    }
    (void)GetMotionFrameCount(10006);
    {
        EventSetMotion *const cue = static_cast<EventSetMotion *>(YcpaQueueRecord(EVT_AT_SetMot_Callback, sizeof(EventSetMotion)));
        cue->cleanup = 1;
        cue->character = YCPA_YUA;
        cue->animation = 10005;
        cue->frame = 0.0f;
        cue->speed = 0.7f;
        cue->flags = YCPA_MOTION_MIRRORED;
        cue->delay = 640.0f;
    }
    (void)GetMotionFrameCount(10005);
    {
        EventSetMotion cue{.cleanup = 0, .character = YCPA_BA6, .animation = 9986, .frame = 30.0f, .speed = 1.0f, .flags = YCPA_MOTION_NO_BLEND, .delay = 0.0f};
        EVT_AT_SetMot_Callback(&cue);
    }
    (void)GetMotionFrameCount(9986);
    {
        EventSetMotion *const cue = static_cast<EventSetMotion *>(YcpaQueueRecord(EVT_AT_SetMot_Callback, sizeof(EventSetMotion)));
        cue->cleanup = 1;
        cue->character = YCPA_BA6;
        cue->animation = 9985;
        cue->frame = 0.0f;
        cue->speed = 1.0f;
        cue->flags = YCPA_MOTION_NO_BLEND;
        cue->delay = 640.0f;
    }
    (void)GetMotionFrameCount(9985);
    {
        EventSetCamera cue{.cleanup = 0,
            .camera = 1000,
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
        cue->camera = 1001;
        cue->eye = YCPA_YUAB;
        cue->target = YCPA_YUAB;
        cue->context = 0;
        cue->reserved11[0] = std::byte{0};
        cue->reserved11[1] = std::byte{0};
        cue->reserved11[2] = std::byte{0};
        cue->angle = 0.0f;
        cue->delay = 230.0f;
    }
    {
        EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
        cue->cleanup = 1;
        cue->camera = 1002;
        cue->eye = YCPA_RYOB;
        cue->target = YCPA_RYOB;
        cue->context = 0;
        cue->reserved11[0] = std::byte{0};
        cue->reserved11[1] = std::byte{0};
        cue->reserved11[2] = std::byte{0};
        cue->angle = 0.0f;
        cue->delay = 520.0f;
    }
    {
        EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
        cue->cleanup = 1;
        cue->camera = 1003;
        cue->eye = YCPA_RYOB;
        cue->target = YCPA_RYOB;
        cue->context = 0;
        cue->reserved11[0] = std::byte{0};
        cue->reserved11[1] = std::byte{0};
        cue->reserved11[2] = std::byte{0};
        cue->angle = 0.0f;
        cue->delay = 640.0f;
    }
    {
        EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
        cue->cleanup = 1;
        cue->camera = 1004;
        cue->eye = YCPA_RYOB;
        cue->target = YCPA_RYOB;
        cue->context = 0;
        cue->reserved11[0] = std::byte{0};
        cue->reserved11[1] = std::byte{0};
        cue->reserved11[2] = std::byte{0};
        cue->angle = 0.0f;
        cue->delay = 800.0f;
    }
    EVT_AT_SetVoice("0235C020", 0); // @text EN: "This place isn't popular these days,\nam I right?" | JA: "あなたのところ、\n最近はやってないようねぇ。"
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 100.0f;
        std::memcpy(cue->voice, "0235H001", sizeof("0235H001"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 140.0f;
        std::memcpy(cue->voice, "0235H002", sizeof("0235H002"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 230.0f;
        std::memcpy(cue->voice, "0235C021", sizeof("0235C021"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 300.0f;
        std::memcpy(cue->voice, "0235C022", sizeof("0235C022"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 380.0f;
        std::memcpy(cue->voice, "0235C026", sizeof("0235C026"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 420.0f;
        std::memcpy(cue->voice, "0235H003", sizeof("0235H003"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 520.0f;
        std::memcpy(cue->voice, "0235B027", sizeof("0235B027"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 640.0f;
        std::memcpy(cue->voice, "0235C023", sizeof("0235C023"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 720.0f;
        std::memcpy(cue->voice, "0235H004", sizeof("0235H004"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 800.0f;
        std::memcpy(cue->voice, "0235B028", sizeof("0235B028"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 860.0f;
        std::memcpy(cue->voice, "0235A004", sizeof("0235A004"));
    }
    EVT_AT_SetPos(
        SHEN_NAME(RYO_), YCPA_INVALID, Vector3{.x = static_cast<float>(266.79999), .y = static_cast<float>(121.6), .z = static_cast<float>(203.39999)}, 1780);
    EVT_AT_SetAngY(SHEN_NAME(RYO_), YCPA_INVALID, static_cast<float>(0.0), 1780);
    EVT_AT_SetPos(SHEN_NAME(REN_), SHEN_NAME(RYO_), Vector3{.x = static_cast<float>(-0.1), .y = static_cast<float>(0.0), .z = static_cast<float>(-1.0)}, 1780);
    EVT_AT_SetAngY(SHEN_NAME(REN_), SHEN_NAME(RYO_), static_cast<float>(0.0), 1780);
    EVT_AT_SetPos(
        SHEN_NAME(YUA_), YCPA_INVALID, Vector3{.x = static_cast<float>(268.79999), .y = static_cast<float>(121.6), .z = static_cast<float>(204.3)}, 890);
    EVT_AT_SetAngY(SHEN_NAME(YUA_), YCPA_INVALID, static_cast<float>(270.0), 890);
    EVT_AT_SetPos(SHEN_NAME(RYOB), SHEN_NAME(RYO_), Vector3{.x = static_cast<float>(0.0), .y = static_cast<float>(0.0), .z = static_cast<float>(0.0)}, 890);
    EVT_AT_SetAngY(SHEN_NAME(RYOB), SHEN_NAME(RYO_), static_cast<float>(0.0), 890);
    EVT_AT_SetPos(SHEN_NAME(YUAB), SHEN_NAME(YUA_), Vector3{.x = static_cast<float>(0.0), .y = static_cast<float>(0.0), .z = static_cast<float>(0.0)}, 890);
    EVT_AT_SetAngY(SHEN_NAME(YUAB), SHEN_NAME(YUA_), static_cast<float>(0.0), 890);
    (void)EVT_AT_SetMot(SHEN_NAME(RYO_), 9994, static_cast<float>(0.0), static_cast<float>(1.0), 0x40000000U, 1780);
    (void)EVT_AT_SetMot(SHEN_NAME(REN_), 9987, static_cast<float>(0.0), static_cast<float>(1.0), 0x40000000U, 1780);
    (void)EVT_AT_SetMot(SHEN_NAME(YUA_), 10004, static_cast<float>(0.0), static_cast<float>(1.0), 0x40000000U, 890);
    (void)EVT_AT_SetMot(SHEN_NAME(YUA_), 10004, static_cast<float>(450.0), static_cast<float>(1.0), 0x0U, 1290);
    YcpaCamera(1010, SHEN_NAME(RYOB), SHEN_NAME(RYOB), 0, 0, 890);
    YcpaCamera(1011, SHEN_NAME(YUAB), SHEN_NAME(YUAB), 0, 0, 1100);
    YcpaCamera(1012, SHEN_NAME(RYOB), SHEN_NAME(RYOB), 0, 0, 1780);
    EVT_AT_SetVoice("0235C027", 890);
    EVT_AT_SetVoice("0235A017", 1290); // @text EN: "She's using the elevator again." | JA: "また、エレベータに乗るのか…"
    EVT_AT_SetVoice("0235B029", 1370); // @text EN: "After her!" | JA: "追うぞ！"
    EVT_AT_SetVoice("0235A005", 1410); // @text EN: "Yeah." | JA: "ああ。"
    EVT_AT_SetVoice("0235C028", 1470); // @text EN: "Ah, yes=@" | JA: "ポチッと…"
    EVT_AT_SetVoice("0235B044", 1840);
    EVT_AT_SetVoice("0235B030", 1880); // @text EN: "She got off on the first floor." | JA: "１階で降りたみてぇだな。"
    EVT_AT_SetVoice("0235A018", 1960); // @text EN: "We'll go down too." | JA: "俺たちも降りよう。"
    {
        StepObjectDisplayWork *const cue = static_cast<StepObjectDisplayWork *>(YcpaQueueRecord(EVT_AT_ObjectDisp_Callback, sizeof(StepObjectDisplayWork)));
        cue->cleanup = 1;
        cue->character = YCPA_YUA;
        cue->mode = 1;
        cue->delay = 1630.0f;
    }
    (void)QueueYcpaSetLevel(SHEN_NAME(R598), 2, 1100);
    (void)QueueYcpaSetLevel(SHEN_NAME(R598), 3, 1130);
    (void)QueueYcpaSetLevel(SHEN_NAME(R598), 4, 1160);
    (void)QueueYcpaSetLevel(SHEN_NAME(R598), 5, 1190);
    (void)QueueYcpaSetLevel(SHEN_NAME(R598), 6, 1220);
    (void)QueueYcpaSetLevel(SHEN_NAME(R598), 5, 1630);
    (void)QueueYcpaSetLevel(SHEN_NAME(R598), 4, 1660);
    (void)QueueYcpaSetLevel(SHEN_NAME(R598), 3, 1690);
    (void)QueueYcpaSetLevel(SHEN_NAME(R598), 2, 1720);
    (void)QueueYcpaSetLevel(SHEN_NAME(R598), 1, 1750);
    (void)QueueYcpaSetLamp(SHEN_NAME(R598), 1, 1, 1070);
    (void)QueueYcpaSetLamp(SHEN_NAME(R598), 1, 0, 1310);
    (void)QueueYcpaSetLamp(SHEN_NAME(R598), 2, 1, 1070);
    (void)QueueYcpaSetLamp(SHEN_NAME(R598), 2, 0, 1250);
    (void)QueueYcpaSetLamp(SHEN_NAME(R598), 3, 1, 1470);
    (void)QueueYcpaSetLamp(SHEN_NAME(R598), 3, 0, 1780);
    (void)QueueYcpaSetDoorMotion(SHEN_NAME(R598), 61648, 1250);
    (void)QueueYcpaSetDoorMotion(SHEN_NAME(R598), 61647, 1540);
    (void)EVT_AT_SetSound(0x4B6u, 1220);
    (void)EVT_AT_SetSound(0x4D3u, 1250);
    (void)EVT_AT_SetSound(0x4B6u, 1515);
    (void)EVT_AT_SetSound(0x4BCu, 1535);
    return 1970;
}


std::int32_t YcpaStreetCorner(YcpaWork &work) {
    SetHighModelXYZ(FindCharacterByScenarioName(SHEN_NAME(RYO_)), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SHEN_NAME(RYO_)), 0, 1, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SHEN_NAME(REN_)), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SHEN_NAME(REN_)), 0, 1, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SHEN_NAME(YUA_)), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SHEN_NAME(YUA_)), 0, 1, 0);
    {
        EventSetPosition cue{.cleanup = 0, .character = YCPA_RYO, .parent = YCPA_INVALID, .position = {.x = 177.5f, .y = 100.0f, .z = 137.0f}, .delay = 0.0f};
        EVT_AT_SetPosRot(&cue);
    }
    {
        EventSetAngle cue{.cleanup = 0, .character = YCPA_RYO, .parent = YCPA_INVALID, .degrees = 0.0f, .delay = 0.0f};
        EVT_AT_SetAngY_Callback(&cue);
    }
    EVT_AT_SetPos(SHEN_NAME(RYO_), YCPA_INVALID, Vector3{.x = static_cast<float>(178.0), .y = static_cast<float>(100.0), .z = static_cast<float>(138.0)}, 1000);
    {
        EventSetAngle *const cue = static_cast<EventSetAngle *>(YcpaQueueRecord(EVT_AT_SetAngY_Callback, sizeof(EventSetAngle)));
        cue->cleanup = 1;
        cue->character = YCPA_RYO;
        cue->parent = YCPA_INVALID;
        cue->degrees = 0.0f;
        cue->delay = 1000.0f;
    }
    EVT_AT_SetPos(
        SHEN_NAME(RYO_), YCPA_INVALID, Vector3{.x = static_cast<float>(177.89999), .y = static_cast<float>(100.0), .z = static_cast<float>(135.7)}, 1050);
    {
        EventSetAngle *const cue = static_cast<EventSetAngle *>(YcpaQueueRecord(EVT_AT_SetAngY_Callback, sizeof(EventSetAngle)));
        cue->cleanup = 1;
        cue->character = YCPA_RYO;
        cue->parent = YCPA_INVALID;
        cue->degrees = 90.0f;
        cue->delay = 1050.0f;
    }
    {
        EventSetPosition cue{.cleanup = 0, .character = YCPA_REN, .parent = YCPA_RYO, .position = {.x = 0.5f, .y = 0.0f, .z = 0.4f}, .delay = 0.0f};
        EVT_AT_SetPosRot(&cue);
    }
    {
        EventSetAngle cue{.cleanup = 0, .character = YCPA_REN, .parent = YCPA_RYO, .degrees = 0.0f, .delay = 0.0f};
        EVT_AT_SetAngY_Callback(&cue);
    }
    EVT_AT_SetPos(
        SHEN_NAME(REN_), YCPA_INVALID, Vector3{.x = static_cast<float>(0.5), .y = static_cast<float>(0.0), .z = static_cast<float>(0.40000001)}, 1000);
    {
        EventSetAngle *const cue = static_cast<EventSetAngle *>(YcpaQueueRecord(EVT_AT_SetAngY_Callback, sizeof(EventSetAngle)));
        cue->cleanup = 1;
        cue->character = YCPA_REN;
        cue->parent = YCPA_INVALID;
        cue->degrees = 0.0f;
        cue->delay = 1000.0f;
    }
    {
        EventSetPosition cue{.cleanup = 0, .character = YCPA_YUA, .parent = YCPA_INVALID, .position = {.x = 168.7f, .y = 100.0f, .z = 130.4f}, .delay = 0.0f};
        EVT_AT_SetPosRot(&cue);
    }
    {
        EventSetAngle cue{.cleanup = 0, .character = YCPA_YUA, .parent = YCPA_INVALID, .degrees = 45.0f, .delay = 0.0f};
        EVT_AT_SetAngY_Callback(&cue);
    }
    EVT_AT_SetPos(
        SHEN_NAME(YUA_), YCPA_INVALID, Vector3{.x = static_cast<float>(168.39999), .y = static_cast<float>(100.0), .z = static_cast<float>(131.39999)}, 1000);
    {
        EventSetAngle *const cue = static_cast<EventSetAngle *>(YcpaQueueRecord(EVT_AT_SetAngY_Callback, sizeof(EventSetAngle)));
        cue->cleanup = 1;
        cue->character = YCPA_YUA;
        cue->parent = YCPA_INVALID;
        cue->degrees = 75.0f;
        cue->delay = 1000.0f;
    }
    EVT_AT_SetPos(SHEN_NAME(YUA_), YCPA_INVALID, Vector3{.x = static_cast<float>(166.3), .y = static_cast<float>(100.0), .z = static_cast<float>(130.8)}, 1020);
    {
        EventSetAngle *const cue = static_cast<EventSetAngle *>(YcpaQueueRecord(EVT_AT_SetAngY_Callback, sizeof(EventSetAngle)));
        cue->cleanup = 1;
        cue->character = YCPA_YUA;
        cue->parent = YCPA_INVALID;
        cue->degrees = 67.5f;
        cue->delay = 1020.0f;
    }
    EVT_AT_SetPos(SHEN_NAME(YUA_), YCPA_INVALID, Vector3{.x = static_cast<float>(168.3), .y = static_cast<float>(100.0), .z = static_cast<float>(131.7)}, 1000);
    {
        EventSetAngle *const cue = static_cast<EventSetAngle *>(YcpaQueueRecord(EVT_AT_SetAngY_Callback, sizeof(EventSetAngle)));
        cue->cleanup = 1;
        cue->character = YCPA_YUA;
        cue->parent = YCPA_INVALID;
        cue->degrees = 67.5f;
        cue->delay = 1000.0f;
    }
    {
        EventSetPosition cue{.cleanup = 0, .character = YCPA_TB3, .parent = YCPA_INVALID, .position = {.x = 168.7f, .y = 100.0f, .z = 128.6f}, .delay = 0.0f};
        EVT_AT_SetPosRot(&cue);
    }
    {
        EventSetAngle cue{.cleanup = 0, .character = YCPA_TB3, .parent = YCPA_INVALID, .degrees = 90.0f, .delay = 0.0f};
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
        EventSetAngle *const cue = static_cast<EventSetAngle *>(YcpaQueueRecord(EVT_AT_SetAngY_Callback, sizeof(EventSetAngle)));
        cue->cleanup = 1;
        cue->character = YCPA_YUA;
        cue->parent = YCPA_YUA;
        cue->degrees = -90.0f;
        cue->delay = 820.0f;
    }
    {
        EventSetMotion cue{.cleanup = 0,
            .character = YCPA_RYO,
            .animation = 9996,
            .frame = 0.0f,
            .speed = 1.0f,
            .flags = YCPA_MOTION_NO_BLEND | YCPA_MOTION_MIRRORED,
            .delay = 0.0f};
        EVT_AT_SetMot_Callback(&cue);
    }
    (void)GetMotionFrameCount(9996);
    {
        EventSetMotion *const cue = static_cast<EventSetMotion *>(YcpaQueueRecord(EVT_AT_SetMot_Callback, sizeof(EventSetMotion)));
        cue->cleanup = 1;
        cue->character = YCPA_RYO;
        cue->animation = 10044;
        cue->frame = 0.0f;
        cue->speed = 1.0f;
        cue->flags = YCPA_MOTION_NO_BLEND | YCPA_MOTION_MIRRORED;
        cue->delay = 1000.0f;
    }
    (void)GetMotionFrameCount(10044);
    {
        EventSetMotion *const cue = static_cast<EventSetMotion *>(YcpaQueueRecord(EVT_AT_SetMot_Callback, sizeof(EventSetMotion)));
        cue->cleanup = 1;
        cue->character = YCPA_RYO;
        cue->animation = 10050;
        cue->frame = 0.0f;
        cue->speed = 1.0f;
        cue->flags = YCPA_MOTION_NO_BLEND;
        cue->delay = 1050.0f;
    }
    (void)GetMotionFrameCount(10050);
    {
        EventSetMotion cue{.cleanup = 0,
            .character = YCPA_REN,
            .animation = 9989,
            .frame = 0.0f,
            .speed = 1.0f,
            .flags = YCPA_MOTION_NO_BLEND | YCPA_MOTION_MIRRORED,
            .delay = 0.0f};
        EVT_AT_SetMot_Callback(&cue);
    }
    (void)GetMotionFrameCount(9989);
    {
        EventSetMotion *const cue = static_cast<EventSetMotion *>(YcpaQueueRecord(EVT_AT_SetMot_Callback, sizeof(EventSetMotion)));
        cue->cleanup = 1;
        cue->character = YCPA_REN;
        cue->animation = 10039;
        cue->frame = 0.0f;
        cue->speed = 1.0f;
        cue->flags = YCPA_MOTION_NO_BLEND | YCPA_MOTION_MIRRORED;
        cue->delay = 1020.0f;
    }
    (void)GetMotionFrameCount(10039);
    {
        EventSetMotion cue{.cleanup = 0, .character = YCPA_YUA, .animation = 10008, .frame = 0.0f, .speed = 1.0f, .flags = YCPA_MOTION_NO_BLEND, .delay = 0.0f};
        EVT_AT_SetMot_Callback(&cue);
    }
    (void)GetMotionFrameCount(10008);
    {
        EventSetMotion *const cue = static_cast<EventSetMotion *>(YcpaQueueRecord(EVT_AT_SetMot_Callback, sizeof(EventSetMotion)));
        cue->cleanup = 1;
        cue->character = YCPA_YUA;
        cue->animation = 10007;
        cue->frame = 0.0f;
        cue->speed = 0.9f;
        cue->flags = YCPA_MOTION_NO_BLEND | YCPA_MOTION_MIRRORED;
        cue->delay = 820.0f;
    }
    (void)GetMotionFrameCount(10007);
    {
        EventSetMotion *const cue = static_cast<EventSetMotion *>(YcpaQueueRecord(EVT_AT_SetMot_Callback, sizeof(EventSetMotion)));
        cue->cleanup = 1;
        cue->character = YCPA_YUA;
        cue->animation = 10048;
        cue->frame = 0.0f;
        cue->speed = 1.0f;
        cue->flags = YCPA_MOTION_NO_BLEND;
        cue->delay = 1000.0f;
    }
    (void)GetMotionFrameCount(10048);
    {
        EventSetMotion *const cue = static_cast<EventSetMotion *>(YcpaQueueRecord(EVT_AT_SetMot_Callback, sizeof(EventSetMotion)));
        cue->cleanup = 1;
        cue->character = YCPA_YUA;
        cue->animation = 10007;
        cue->frame = 110.0f;
        cue->speed = 0.6f;
        cue->flags = YCPA_MOTION_NO_BLEND | YCPA_MOTION_MIRRORED;
        cue->delay = 1020.0f;
    }
    (void)GetMotionFrameCount(10007);
    {
        EventSetMotion cue{.cleanup = 0, .character = YCPA_TB3, .animation = 10003, .frame = 0.0f, .speed = 1.0f, .flags = YCPA_MOTION_NO_BLEND, .delay = 0.0f};
        EVT_AT_SetMot_Callback(&cue);
    }
    (void)GetMotionFrameCount(10003);
    {
        EventSetMotion *const cue = static_cast<EventSetMotion *>(YcpaQueueRecord(EVT_AT_SetMot_Callback, sizeof(EventSetMotion)));
        cue->cleanup = 1;
        cue->character = YCPA_TB3;
        cue->animation = 10002;
        cue->frame = 0.0f;
        cue->speed = 1.0f;
        cue->flags = 0;
        cue->delay = 820.0f;
    }
    (void)GetMotionFrameCount(10002);
    {
        EventSetCamera cue{.cleanup = 0,
            .camera = 1100,
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
        cue->camera = 1101;
        cue->eye = YCPA_YUAB;
        cue->target = YCPA_YUAB;
        cue->context = 0;
        cue->reserved11[0] = std::byte{0};
        cue->reserved11[1] = std::byte{0};
        cue->reserved11[2] = std::byte{0};
        cue->angle = 0.0f;
        cue->delay = 240.0f;
    }
    {
        EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
        cue->cleanup = 1;
        cue->camera = 1108;
        cue->eye = YCPA_YUAB;
        cue->target = YCPA_YUAB;
        cue->context = 0;
        cue->reserved11[0] = std::byte{0};
        cue->reserved11[1] = std::byte{0};
        cue->reserved11[2] = std::byte{0};
        cue->angle = 0.0f;
        cue->delay = 320.0f;
    }
    {
        EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
        cue->cleanup = 1;
        cue->camera = 1102;
        cue->eye = YCPA_RYOB;
        cue->target = YCPA_RYOB;
        cue->context = 0;
        cue->reserved11[0] = std::byte{0};
        cue->reserved11[1] = std::byte{0};
        cue->reserved11[2] = std::byte{0};
        cue->angle = 0.0f;
        cue->delay = 560.0f;
    }
    {
        EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
        cue->cleanup = 1;
        cue->camera = 1103;
        cue->eye = YCPA_RYOB;
        cue->target = YCPA_RYOB;
        cue->context = 0;
        cue->reserved11[0] = std::byte{0};
        cue->reserved11[1] = std::byte{0};
        cue->reserved11[2] = std::byte{0};
        cue->angle = 0.0f;
        cue->delay = 820.0f;
    }
    {
        EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
        cue->cleanup = 1;
        cue->camera = 1104;
        cue->eye = YCPA_RYOB;
        cue->target = YCPA_RYOB;
        cue->context = 0;
        cue->reserved11[0] = std::byte{0};
        cue->reserved11[1] = std::byte{0};
        cue->reserved11[2] = std::byte{0};
        cue->angle = 0.0f;
        cue->delay = 940.0f;
    }
    {
        EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
        cue->cleanup = 1;
        cue->camera = 1105;
        cue->eye = YCPA_RYOB;
        cue->target = YCPA_RYOB;
        cue->context = 0;
        cue->reserved11[0] = std::byte{0};
        cue->reserved11[1] = std::byte{0};
        cue->reserved11[2] = std::byte{0};
        cue->angle = 0.0f;
        cue->delay = 1000.0f;
    }
    {
        EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
        cue->cleanup = 1;
        cue->camera = 1106;
        cue->eye = YCPA_INVALID;
        cue->target = YCPA_INVALID;
        cue->context = 0;
        cue->reserved11[0] = std::byte{0};
        cue->reserved11[1] = std::byte{0};
        cue->reserved11[2] = std::byte{0};
        cue->angle = 0.0f;
        cue->delay = 1020.0f;
    }
    EVT_AT_SetVoice("0235C009", 0); // @text EN: "So, did you get that knife I asked for?" | JA: "この間頼んどいた、\nナイフは入ったのかしら？"
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 80.0f;
        std::memcpy(cue->voice, "0235F001", sizeof("0235F001"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 140.0f;
        std::memcpy(cue->voice, "0235F002", sizeof("0235F002"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 240.0f;
        std::memcpy(cue->voice, "0235C010", sizeof("0235C010"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 320.0f;
        std::memcpy(cue->voice, "0235F003", sizeof("0235F003"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 440.0f;
        std::memcpy(cue->voice, "0235C011", sizeof("0235C011"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 560.0f;
        std::memcpy(cue->voice, "0235B011", sizeof("0235B011"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 640.0f;
        std::memcpy(cue->voice, "0235B012", sizeof("0235B012"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 700.0f;
        std::memcpy(cue->voice, "0235A009", sizeof("0235A009"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 760.0f;
        std::memcpy(cue->voice, "0235B013", sizeof("0235B013"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 820.0f;
        std::memcpy(cue->voice, "0235C012", sizeof("0235C012"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 860.0f;
        std::memcpy(cue->voice, "0235F004", sizeof("0235F004"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 940.0f;
        std::memcpy(cue->voice, "0235B014", sizeof("0235B014"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 970.0f;
        std::memcpy(cue->voice, "0235A005", sizeof("0235A005"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 1020.0f;
        std::memcpy(cue->voice, "0235C013", sizeof("0235C013"));
    }
    QueueSkQteWait(&work.qteElapsed, 64, 30, 20, YcpaStreetQteSuccess, nullptr, 1060);
    return 0;
}


std::int32_t YcpaMarketConversation() {
    EV_DisableCharacterDisplay(SHEN_NAME(AM1_));
    {
        StepObjectDisplayWork *const cue = static_cast<StepObjectDisplayWork *>(YcpaQueueRecord(EVT_AT_ObjectDisp_Callback, sizeof(StepObjectDisplayWork)));
        cue->cleanup = 1;
        cue->character = YCPA_AM1;
        cue->mode = 0;
        cue->delay = 120.0f;
    }
    {
        StepObjectDisplayWork *const cue = static_cast<StepObjectDisplayWork *>(YcpaQueueRecord(EVT_AT_ObjectDisp_Callback, sizeof(StepObjectDisplayWork)));
        cue->cleanup = 1;
        cue->character = YCPA_AM1;
        cue->mode = 1;
        cue->delay = 280.0f;
    }
    {
        EventSetPosition cue{.cleanup = 0, .character = YCPA_RYO, .parent = YCPA_INVALID, .position = {.x = 193.4f, .y = 100.0f, .z = 99.2f}, .delay = 0.0f};
        EVT_AT_SetPosRot(&cue);
    }
    {
        EventSetAngle cue{.cleanup = 0, .character = YCPA_RYO, .parent = YCPA_INVALID, .degrees = 180.0f, .delay = 0.0f};
        EVT_AT_SetAngY_Callback(&cue);
    }
    {
        EventSetPosition cue{.cleanup = 0, .character = YCPA_REN, .parent = YCPA_RYO, .position = {.x = -0.7f, .y = 0.0f, .z = -0.5f}, .delay = 0.0f};
        EVT_AT_SetPosRot(&cue);
    }
    {
        EventSetAngle cue{.cleanup = 0, .character = YCPA_REN, .parent = YCPA_RYO, .degrees = 0.0f, .delay = 0.0f};
        EVT_AT_SetAngY_Callback(&cue);
    }
    {
        EventSetPosition cue{.cleanup = 0, .character = YCPA_YUA, .parent = YCPA_INVALID, .position = {.x = 188.5f, .y = 100.0f, .z = 92.6f}, .delay = 0.0f};
        EVT_AT_SetPosRot(&cue);
    }
    {
        EventSetAngle cue{.cleanup = 0, .character = YCPA_YUA, .parent = YCPA_INVALID, .degrees = 0.0f, .delay = 0.0f};
        EVT_AT_SetAngY_Callback(&cue);
    }
    EVT_AT_SetPos(SHEN_NAME(YUA_), YCPA_INVALID, Vector3{.x = static_cast<float>(197.5), .y = static_cast<float>(100.0), .z = static_cast<float>(97.0)}, 860);
    {
        EventSetAngle *const cue = static_cast<EventSetAngle *>(YcpaQueueRecord(EVT_AT_SetAngY_Callback, sizeof(EventSetAngle)));
        cue->cleanup = 1;
        cue->character = YCPA_YUA;
        cue->parent = YCPA_INVALID;
        cue->degrees = 270.0f;
        cue->delay = 860.0f;
    }
    EVT_AT_SetPos(
        SHEN_NAME(AM1_), YCPA_INVALID, Vector3{.x = static_cast<float>(190.0), .y = static_cast<float>(100.0), .z = static_cast<float>(91.300003)}, 120);
    {
        EventSetAngle *const cue = static_cast<EventSetAngle *>(YcpaQueueRecord(EVT_AT_SetAngY_Callback, sizeof(EventSetAngle)));
        cue->cleanup = 1;
        cue->character = YCPA_AM1;
        cue->parent = YCPA_INVALID;
        cue->degrees = 150.0f;
        cue->delay = 120.0f;
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
        cue->speed = 1.1f;
        cue->flags = YCPA_MOTION_NO_BLEND;
        cue->delay = 280.0f;
    }
    (void)GetMotionFrameCount(9997);
    {
        EventSetMotion *const cue = static_cast<EventSetMotion *>(YcpaQueueRecord(EVT_AT_SetMot_Callback, sizeof(EventSetMotion)));
        cue->cleanup = 1;
        cue->character = YCPA_REN;
        cue->animation = 9990;
        cue->frame = 0.0f;
        cue->speed = 1.1f;
        cue->flags = YCPA_MOTION_NO_BLEND;
        cue->delay = 280.0f;
    }
    (void)GetMotionFrameCount(9990);
    {
        EventSetMotion *const cue = static_cast<EventSetMotion *>(YcpaQueueRecord(EVT_AT_SetMot_Callback, sizeof(EventSetMotion)));
        cue->cleanup = 1;
        cue->character = YCPA_RYO;
        cue->animation = 9997;
        cue->frame = 950.0f;
        cue->speed = 1.0f;
        cue->flags = YCPA_MOTION_NO_BLEND;
        cue->delay = 860.0f;
    }
    (void)GetMotionFrameCount(9997);
    {
        EventSetMotion *const cue = static_cast<EventSetMotion *>(YcpaQueueRecord(EVT_AT_SetMot_Callback, sizeof(EventSetMotion)));
        cue->cleanup = 1;
        cue->character = YCPA_REN;
        cue->animation = 9990;
        cue->frame = 950.0f;
        cue->speed = 1.0f;
        cue->flags = YCPA_MOTION_NO_BLEND;
        cue->delay = 860.0f;
    }
    (void)GetMotionFrameCount(9990);
    {
        EventSetMotion cue{.cleanup = 0, .character = YCPA_YUA, .animation = 10027, .frame = 0.0f, .speed = 1.0f, .flags = YCPA_MOTION_NO_BLEND, .delay = 0.0f};
        EVT_AT_SetMot_Callback(&cue);
    }
    (void)GetMotionFrameCount(10027);
    {
        EventSetMotion *const cue = static_cast<EventSetMotion *>(YcpaQueueRecord(EVT_AT_SetMot_Callback, sizeof(EventSetMotion)));
        cue->cleanup = 1;
        cue->character = YCPA_YUA;
        cue->animation = 10028;
        cue->frame = 0.0f;
        cue->speed = 1.0f;
        cue->flags = YCPA_MOTION_NO_BLEND;
        cue->delay = 790.0f;
    }
    (void)GetMotionFrameCount(10028);
    {
        EventSetMotion *const cue = static_cast<EventSetMotion *>(YcpaQueueRecord(EVT_AT_SetMot_Callback, sizeof(EventSetMotion)));
        cue->cleanup = 1;
        cue->character = YCPA_AM1;
        cue->animation = 10051;
        cue->frame = 0.0f;
        cue->speed = 0.7f;
        cue->flags = YCPA_MOTION_NO_BLEND;
        cue->delay = 120.0f;
    }
    (void)GetMotionFrameCount(10051);
    {
        EventSetCamera cue{.cleanup = 0,
            .camera = 1400,
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
        cue->camera = 1401;
        cue->eye = YCPA_YUAB;
        cue->target = YCPA_YUAB;
        cue->context = 0;
        cue->reserved11[0] = std::byte{0};
        cue->reserved11[1] = std::byte{0};
        cue->reserved11[2] = std::byte{0};
        cue->angle = 0.0f;
        cue->delay = 90.0f;
    }
    {
        EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
        cue->cleanup = 1;
        cue->camera = 1402;
        cue->eye = YCPA_RYOB;
        cue->target = YCPA_RYOB;
        cue->context = 0;
        cue->reserved11[0] = std::byte{0};
        cue->reserved11[1] = std::byte{0};
        cue->reserved11[2] = std::byte{0};
        cue->angle = 0.0f;
        cue->delay = 280.0f;
    }
    {
        EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
        cue->cleanup = 1;
        cue->camera = 1403;
        cue->eye = YCPA_RYOB;
        cue->target = YCPA_RYOB;
        cue->context = 0;
        cue->reserved11[0] = std::byte{0};
        cue->reserved11[1] = std::byte{0};
        cue->reserved11[2] = std::byte{0};
        cue->angle = 0.0f;
        cue->delay = 420.0f;
    }
    {
        EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
        cue->cleanup = 1;
        cue->camera = 1404;
        cue->eye = YCPA_RYOB;
        cue->target = YCPA_RYOB;
        cue->context = 0;
        cue->reserved11[0] = std::byte{0};
        cue->reserved11[1] = std::byte{0};
        cue->reserved11[2] = std::byte{0};
        cue->angle = 0.0f;
        cue->delay = 790.0f;
    }
    {
        EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
        cue->cleanup = 1;
        cue->camera = 1405;
        cue->eye = YCPA_RYOB;
        cue->target = YCPA_RYOB;
        cue->context = 0;
        cue->reserved11[0] = std::byte{0};
        cue->reserved11[1] = std::byte{0};
        cue->reserved11[2] = std::byte{0};
        cue->angle = 0.0f;
        cue->delay = 860.0f;
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 90.0f;
        std::memcpy(cue->voice, "0235C034", sizeof("0235C034"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 150.0f;
        std::memcpy(cue->voice, "0235C035", sizeof("0235C035"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 280.0f;
        std::memcpy(cue->voice, "0235B051", sizeof("0235B051"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 340.0f;
        std::memcpy(cue->voice, "0235B052", sizeof("0235B052"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 420.0f;
        std::memcpy(cue->voice, "0235A035", sizeof("0235A035"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 480.0f;
        std::memcpy(cue->voice, "0235B053", sizeof("0235B053"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 520.0f;
        std::memcpy(cue->voice, "0235A036", sizeof("0235A036"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 620.0f;
        std::memcpy(cue->voice, "0235B054", sizeof("0235B054"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 660.0f;
        std::memcpy(cue->voice, "0235B055", sizeof("0235B055"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 790.0f;
        std::memcpy(cue->voice, "0235C036", sizeof("0235C036"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 860.0f;
        std::memcpy(cue->voice, "0235A037", sizeof("0235A037"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 890.0f;
        std::memcpy(cue->voice, "0235B039", sizeof("0235B039"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 900.0f;
        std::memcpy(cue->voice, "0235B056", sizeof("0235B056"));
    }
    {
        EventCling cue{.cleanup = 0,
            .target = YCPA_CUP,
            .character = YCPA_YUA,
            .bone = 14,
            .position = {.x = 0.17f, .y = 0.0f, .z = -0.05f},
            .rotation = {.x = 0, .y = 16384, .z = 0},
            .mode = YCPA_ATTACH_OBJECT,
            .delay = 0.0f};
        EVT_AT_Cling(&cue);
    }
    {
        StepObjectDisplayWork *const cue = static_cast<StepObjectDisplayWork *>(YcpaQueueRecord(EVT_AT_ObjectDisp_Callback, sizeof(StepObjectDisplayWork)));
        cue->cleanup = 1;
        cue->character = YCPA_CUP;
        cue->mode = 1;
        cue->delay = 790.0f;
    }
    {
        EventCling *const cue = static_cast<EventCling *>(YcpaQueueRecord(EVT_AT_Cling, sizeof(EventCling)));
        cue->cleanup = 1;
        cue->target = YCPA_CUP;
        cue->character = shen_name{};
        cue->mode = YCPA_DETACH_OBJECT;
        cue->delay = 900.0f;
    }
    return 900;
}


std::int32_t YcpaElevatorArrival() {
    {
        EventSetPosition cue{.cleanup = 0, .character = YCPA_RYO, .parent = YCPA_INVALID, .position = {.x = 294.0f, .y = 100.0f, .z = 91.3f}, .delay = 0.0f};
        EVT_AT_SetPosRot(&cue);
    }
    {
        EventSetAngle cue{.cleanup = 0, .character = YCPA_RYO, .parent = YCPA_INVALID, .degrees = 180.0f, .delay = 0.0f};
        EVT_AT_SetAngY_Callback(&cue);
    }
    {
        EventSetPosition cue{.cleanup = 0, .character = YCPA_REN, .parent = YCPA_RYO, .position = {.x = 0.5f, .y = 0.0f, .z = 0.4f}, .delay = 0.0f};
        EVT_AT_SetPosRot(&cue);
    }
    {
        EventSetAngle cue{.cleanup = 0, .character = YCPA_REN, .parent = YCPA_RYO, .degrees = 0.0f, .delay = 0.0f};
        EVT_AT_SetAngY_Callback(&cue);
    }
    {
        EventSetPosition cue{.cleanup = 0, .character = YCPA_YUA, .parent = YCPA_INVALID, .position = {.x = 295.3f, .y = 100.0f, .z = 100.4f}, .delay = 0.0f};
        EVT_AT_SetPosRot(&cue);
    }
    {
        EventSetAngle cue{.cleanup = 0, .character = YCPA_YUA, .parent = YCPA_INVALID, .degrees = 180.0f, .delay = 0.0f};
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
        EventSetMotion cue{.cleanup = 0,
            .character = YCPA_RYO,
            .animation = 9996,
            .frame = 0.0f,
            .speed = 1.0f,
            .flags = YCPA_MOTION_NO_BLEND | YCPA_MOTION_MIRRORED,
            .delay = 0.0f};
        EVT_AT_SetMot_Callback(&cue);
    }
    (void)GetMotionFrameCount(9996);
    {
        EventSetMotion cue{.cleanup = 0,
            .character = YCPA_REN,
            .animation = 9989,
            .frame = 0.0f,
            .speed = 1.0f,
            .flags = YCPA_MOTION_NO_BLEND | YCPA_MOTION_MIRRORED,
            .delay = 0.0f};
        EVT_AT_SetMot_Callback(&cue);
    }
    (void)GetMotionFrameCount(9989);
    {
        EventSetMotion cue{.cleanup = 0, .character = YCPA_YUA, .animation = 10004, .frame = 0.0f, .speed = 1.0f, .flags = YCPA_MOTION_NO_BLEND, .delay = 0.0f};
        EVT_AT_SetMot_Callback(&cue);
    }
    (void)GetMotionFrameCount(10004);
    {
        EventSetMotion *const cue = static_cast<EventSetMotion *>(YcpaQueueRecord(EVT_AT_SetMot_Callback, sizeof(EventSetMotion)));
        cue->cleanup = 1;
        cue->character = YCPA_RYO;
        cue->animation = 9994;
        cue->frame = 0.0f;
        cue->speed = 1.0f;
        cue->flags = YCPA_MOTION_NO_BLEND;
        cue->delay = 780.0f;
    }
    (void)GetMotionFrameCount(9994);
    {
        EventSetMotion *const cue = static_cast<EventSetMotion *>(YcpaQueueRecord(EVT_AT_SetMot_Callback, sizeof(EventSetMotion)));
        cue->cleanup = 1;
        cue->character = YCPA_REN;
        cue->animation = 9987;
        cue->frame = 0.0f;
        cue->speed = 1.0f;
        cue->flags = YCPA_MOTION_NO_BLEND;
        cue->delay = 780.0f;
    }
    (void)GetMotionFrameCount(9987);
    {
        EventSetMotion *const cue = static_cast<EventSetMotion *>(YcpaQueueRecord(EVT_AT_SetMot_Callback, sizeof(EventSetMotion)));
        cue->cleanup = 1;
        cue->character = YCPA_YUA;
        cue->animation = 10004;
        cue->frame = 450.0f;
        cue->speed = 1.0f;
        cue->flags = 0;
        cue->delay = 400.0f;
    }
    (void)GetMotionFrameCount(10004);
    EVT_AT_SetPos(
        SHEN_NAME(RYO_), YCPA_INVALID, Vector3{.x = static_cast<float>(296.0), .y = static_cast<float>(100.0), .z = static_cast<float>(99.599998)}, 780);
    {
        EventSetAngle *const cue = static_cast<EventSetAngle *>(YcpaQueueRecord(EVT_AT_SetAngY_Callback, sizeof(EventSetAngle)));
        cue->cleanup = 1;
        cue->character = YCPA_RYO;
        cue->parent = YCPA_INVALID;
        cue->degrees = 270.0f;
        cue->delay = 780.0f;
    }
    EVT_AT_SetPos(SHEN_NAME(REN_), SHEN_NAME(RYO_), Vector3{.x = static_cast<float>(-0.1), .y = static_cast<float>(0.0), .z = static_cast<float>(-1.0)}, 780);
    {
        EventSetAngle *const cue = static_cast<EventSetAngle *>(YcpaQueueRecord(EVT_AT_SetAngY_Callback, sizeof(EventSetAngle)));
        cue->cleanup = 1;
        cue->character = YCPA_REN;
        cue->parent = YCPA_RYO;
        cue->degrees = 0.0f;
        cue->delay = 780.0f;
    }
    EVT_AT_SetVoice("0235C044", 0);
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 570.0f;
        std::memcpy(cue->voice, "0235C045", sizeof("0235C045"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 840.0f;
        std::memcpy(cue->voice, "0235B063", sizeof("0235B063"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 880.0f;
        std::memcpy(cue->voice, "0235A041", sizeof("0235A041"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 920.0f;
        std::memcpy(cue->voice, "0235B064", sizeof("0235B064"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 980.0f;
        std::memcpy(cue->voice, "0235A042", sizeof("0235A042"));
    }
    {
        EventSetCamera cue{.cleanup = 0,
            .camera = 1800,
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
        cue->camera = 1801;
        cue->eye = YCPA_YUAB;
        cue->target = YCPA_YUAB;
        cue->context = 0;
        cue->reserved11[0] = std::byte{0};
        cue->reserved11[1] = std::byte{0};
        cue->reserved11[2] = std::byte{0};
        cue->angle = 0.0f;
        cue->delay = 210.0f;
    }
    {
        EventSetCamera *const cue = static_cast<EventSetCamera *>(YcpaQueueRecord(EVT_AT_SetCam_Callback, sizeof(EventSetCamera)));
        cue->cleanup = 1;
        cue->camera = 1802;
        cue->eye = YCPA_RYOB;
        cue->target = YCPA_RYOB;
        cue->context = 0;
        cue->reserved11[0] = std::byte{0};
        cue->reserved11[1] = std::byte{0};
        cue->reserved11[2] = std::byte{0};
        cue->angle = 0.0f;
        cue->delay = 780.0f;
    }
    SetCurrentElevatorFloor(SHEN_NAME(R594), 6);
    {
        YcpaLevelWork *const cue = static_cast<YcpaLevelWork *>(YcpaQueueRecord(YcpaSetLevelCallback, sizeof(YcpaLevelWork)));
        cue->cleanup = 1;
        cue->elevator = YCPA_R594;
        cue->floor = 5;
        cue->delay = 210;
    }
    {
        YcpaLevelWork *const cue = static_cast<YcpaLevelWork *>(YcpaQueueRecord(YcpaSetLevelCallback, sizeof(YcpaLevelWork)));
        cue->cleanup = 1;
        cue->elevator = YCPA_R594;
        cue->floor = 4;
        cue->delay = 240;
    }
    {
        YcpaLevelWork *const cue = static_cast<YcpaLevelWork *>(YcpaQueueRecord(YcpaSetLevelCallback, sizeof(YcpaLevelWork)));
        cue->cleanup = 1;
        cue->elevator = YCPA_R594;
        cue->floor = 3;
        cue->delay = 270;
    }
    {
        YcpaLevelWork *const cue = static_cast<YcpaLevelWork *>(YcpaQueueRecord(YcpaSetLevelCallback, sizeof(YcpaLevelWork)));
        cue->cleanup = 1;
        cue->elevator = YCPA_R594;
        cue->floor = 2;
        cue->delay = 300;
    }
    {
        YcpaLevelWork *const cue = static_cast<YcpaLevelWork *>(YcpaQueueRecord(YcpaSetLevelCallback, sizeof(YcpaLevelWork)));
        cue->cleanup = 1;
        cue->elevator = YCPA_R594;
        cue->floor = 1;
        cue->delay = 330;
    }
    {
        YcpaLevelWork *const cue = static_cast<YcpaLevelWork *>(YcpaQueueRecord(YcpaSetLevelCallback, sizeof(YcpaLevelWork)));
        cue->cleanup = 1;
        cue->elevator = YCPA_R594;
        cue->floor = 2;
        cue->delay = 690;
    }
    {
        YcpaLevelWork *const cue = static_cast<YcpaLevelWork *>(YcpaQueueRecord(YcpaSetLevelCallback, sizeof(YcpaLevelWork)));
        cue->cleanup = 1;
        cue->elevator = YCPA_R594;
        cue->floor = 3;
        cue->delay = 720;
    }
    {
        YcpaLevelWork *const cue = static_cast<YcpaLevelWork *>(YcpaQueueRecord(YcpaSetLevelCallback, sizeof(YcpaLevelWork)));
        cue->cleanup = 1;
        cue->elevator = YCPA_R594;
        cue->floor = 4;
        cue->delay = 750;
    }
    (void)QueueYcpaSetLamp(SHEN_NAME(R594), 0, 1, 190);
    (void)QueueYcpaSetLamp(SHEN_NAME(R594), 0, 0, 420);
    (void)QueueYcpaSetLamp(SHEN_NAME(R594), 3, 1, 190);
    (void)QueueYcpaSetLamp(SHEN_NAME(R594), 3, 0, 330);
    (void)QueueYcpaSetLamp(SHEN_NAME(R594), 2, 1, 570);
    (void)QueueYcpaSetLamp(SHEN_NAME(R594), 2, 0, 780);
    {
        YcpaDoorMotionWork *const cue = static_cast<YcpaDoorMotionWork *>(YcpaQueueRecord(YcpaSetDoorMotionCallback, sizeof(YcpaDoorMotionWork)));
        cue->cleanup = 1;
        cue->door = YCPA_R594;
        cue->animation = 61648;
        cue->delay = 360;
    }
    {
        YcpaDoorMotionWork *const cue = static_cast<YcpaDoorMotionWork *>(YcpaQueueRecord(YcpaSetDoorMotionCallback, sizeof(YcpaDoorMotionWork)));
        cue->cleanup = 1;
        cue->door = YCPA_R594;
        cue->animation = 61647;
        cue->delay = 600;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 1206;
        cue->delay = 330.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 1235;
        cue->delay = 360.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 1212;
        cue->delay = 620.0f;
    }
    {
        EventSetSound *const cue = static_cast<EventSetSound *>(YcpaQueueRecord(EVT_AT_SetSound_Callback, sizeof(EventSetSound)));
        cue->cleanup = 1;
        cue->sound = 1206;
        cue->delay = 620.0f;
    }
    return 970;
}

} // namespace shenmue::s2::areas
