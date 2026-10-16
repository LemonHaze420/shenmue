#include "s2/areas/area_ycpa.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/character.hpp"
#include "s2/hand.hpp"
#include "s2/camera.hpp"
#include "s2/motion.hpp"
#include "s2/sk_event.hpp"
namespace shenmue::s2::areas {

std::int32_t YcpaDoorInterruption() {
    SetHighModelXYZ(FindCharacterByScenarioName(SHEN_NAME(RYO_)), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SHEN_NAME(RYO_)), 0, 1, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SHEN_NAME(REN_)), 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(SHEN_NAME(REN_)), 0, 1, 0);
    {
        EventSetPosition cue{.cleanup = 0, .character = YCPA_RYO, .parent = YCPA_INVALID, .position = {.x = 290.9f, .y = 100.0f, .z = 74.1f}, .delay = 0.0f};
        EVT_AT_SetPosRot(&cue);
    }
    {
        EventSetAngle cue{.cleanup = 0, .character = YCPA_RYO, .parent = YCPA_INVALID, .degrees = 0.0f, .delay = 0.0f};
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
        EventSetPosition cue{.cleanup = 0, .character = YCPA_YUA, .parent = YCPA_INVALID, .position = {.x = 286.9f, .y = 100.0f, .z = 67.6f}, .delay = 0.0f};
        EVT_AT_SetPosRot(&cue);
    }
    {
        EventSetAngle cue{.cleanup = 0, .character = YCPA_YUA, .parent = YCPA_INVALID, .degrees = 0.0f, .delay = 0.0f};
        EVT_AT_SetAngY_Callback(&cue);
    }
    {
        EventSetPosition cue{.cleanup = 0, .character = YCPA_OB4, .parent = YCPA_INVALID, .position = {.x = 286.0f, .y = 100.0f, .z = 67.4f}, .delay = 0.0f};
        EVT_AT_SetPosRot(&cue);
    }
    {
        EventSetAngle cue{.cleanup = 0, .character = YCPA_OB4, .parent = YCPA_INVALID, .degrees = 247.5f, .delay = 0.0f};
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
        EventSetMotion cue{.cleanup = 0, .character = YCPA_YUA, .animation = 10016, .frame = 0.0f, .speed = 1.0f, .flags = YCPA_MOTION_NO_BLEND, .delay = 0.0f};
        EVT_AT_SetMot_Callback(&cue);
    }
    (void)GetMotionFrameCount(10016);
    {
        EventSetMotion cue{.cleanup = 0, .character = YCPA_OB4, .animation = 9986, .frame = 0.0f, .speed = 1.0f, .flags = YCPA_MOTION_NO_BLEND, .delay = 0.0f};
        EVT_AT_SetMot_Callback(&cue);
    }
    (void)GetMotionFrameCount(9986);
    {
        EventSetCamera cue{.cleanup = 0,
            .camera = 1700,
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
        cue->camera = 1701;
        cue->eye = YCPA_YUAB;
        cue->target = YCPA_YUAB;
        cue->context = 0;
        cue->reserved11[0] = std::byte{0};
        cue->reserved11[1] = std::byte{0};
        cue->reserved11[2] = std::byte{0};
        cue->angle = 0.0f;
        cue->delay = 320.0f;
    }
    EVT_AT_SetVoice("0235C014", 0); // @text EN: "That fridge I bought here,\nit's not working right." | JA: "この前、おたくで買った\n冷蔵庫、調子わるいんだけど…"
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 120.0f;
        std::memcpy(cue->voice, "0235G001", sizeof("0235G001"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 180.0f;
        std::memcpy(cue->voice, "0235G002", sizeof("0235G002"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 240.0f;
        std::memcpy(cue->voice, "0235G003", sizeof("0235G003"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 320.0f;
        std::memcpy(cue->voice, "0235C015", sizeof("0235C015"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 420.0f;
        std::memcpy(cue->voice, "0235G004", sizeof("0235G004"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 550.0f;
        std::memcpy(cue->voice, "0235C016", sizeof("0235C016"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 630.0f;
        std::memcpy(cue->voice, "0235G005", sizeof("0235G005"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 720.0f;
        std::memcpy(cue->voice, "0235C017", sizeof("0235C017"));
    }
    {
        EventSetVoice *const cue = static_cast<EventSetVoice *>(YcpaQueueRecord(EVT_AT_SetVoice_Callback, sizeof(EventSetVoice)));
        cue->cleanup = 1;
        cue->mode = YCPA_VOICE_FORCE_PLAYBACK;
        cue->delay = 820.0f;
        std::memcpy(cue->voice, "0235G006", sizeof("0235G006"));
    }
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    QueueSkAuthorStart(HLib::GetCurrentTask(), SHEN_NAME(DNST), YcpaAuthPursuitCallback, nullptr, 860);
    return 0;
}

void YcpaAuthPursuitCallback(void *) {
    const std::int64_t frame = EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame);
    if (frame == 540) {
        qteBattleUtil(BattleQteMode::Start);
        return;
    }
    if (frame <= 540)
        return;
    const std::int64_t result = qteBattleUtil(BattleQteMode::GetResult);
    if (!result)
        return;
    if (result == 1)
        ycpaPursuitSucceeded = 1;
    else
        EV_authSceneCtrl(AuthSceneCtrl::RefreshAndEnqueue);
    ycpaEvent = 10;
    ycpaPhase = 1;
}

std::int32_t YcpaStreetQteSuccess() {
    ycpaEvent = 12;
    ycpaPhase = 1;
    return 0;
}

void YcpaQueueReturn() {
    QueueSkAuthorStart(HLib::GetCurrentTask(), SHEN_NAME(NFOK), nullptr, nullptr, 0);
}

void YcpaQueuePursuit(YcpaWork &work) {
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    if (ycpaPursuitSucceeded) {
        QueueSkAuthorStart(HLib::GetCurrentTask(), SHEN_NAME(DNOK), nullptr, nullptr, 0);
        work.finished = 1;
    } else
        work.finished = 0;
}
} // namespace shenmue::s2::areas
