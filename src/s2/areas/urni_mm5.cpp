#include "s2/areas/area_urni.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/event_voice.hpp"
#include "s2/face.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/qte_hand_poses.hpp"
#include "s2/step_event.hpp"

namespace shenmue::s2 {
std::int64_t UranaiMm5Setup(shen_name reader, std::int32_t) {
    EventCling request{.cleanup = 0,
        .target = URNI_READING_OBJECT,
        .character = reader,
        .bone = 14,
        .position = URNI_MM5_OBJECT_POSITION,
        .rotation = URNI_MM5_OBJECT_ROTATION,
        .mode = 0,
        .delay = 0};
    EVT_AT_Cling(&request);
    EV_EnableCharacterDisplay(URNI_READING_OBJECT);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(reader), 0, reinterpret_cast<std::byte *>(chestHandPoseCommand), 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(reader), 1, reinterpret_cast<std::byte *>(fortuneLeftReadingHandPose), 0);
    return 0;
}
std::int64_t UranaiMm5ModeOne(shen_name reader, std::int32_t delay) {
    const std::int32_t end = delay + 75;
    EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_MM5_IDLE_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetCam(1216, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(nullptr, URNI_MM5_MODE_ONE_VOICES, 10, 0x80000000U, delay + 30);
    EV_DisableCharacterDisplay(URNI_PLAYER);
    if (end) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 0x0b, sizeof(StepObjectDisplayWork), URNI_TASK);
        RegisterEventTask(task);
        StepObjectDisplayWork *const work = static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(task));
        *work = {.cleanup = 1, .character = URNI_PLAYER, .mode = 0, .delay = static_cast<float>(end)};
        return static_cast<std::uint32_t>(end);
    }
    EV_EnableCharacterDisplay(URNI_PLAYER);
    return 0;
}
std::int64_t UranaiMm5Introduction(shen_name reader, std::int32_t delay) {
    const std::int32_t end = delay + 250;
    if (g_actNumber >= 3) {
        for (std::int32_t i = 14; i > 0; --i)
            if (EV_GetShenmueFlag(URNI_MM5_KL_FLAGS[i]))
                break;
    } else {
        for (std::int32_t i = 19; i > 0; --i)
            if (EV_GetShenmueFlag(URNI_MM5_HK_FLAGS[i]))
                break;
    }
    if (static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 42)) == -100)
        return 0;
    EVT_AT_SetPos(URNI_PLAYER, reader, {-0.44999999f, 0, -1.45f}, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_MM5_GREETING_MOTION, 40, 1, 0x40000000U, delay);
    EVT_AT_SetCam(1208, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1209, URNI_PLAYER_DUMMY, URNI_PLAYER_DUMMY, 0, 0, end);
    QueueEventVoiceList(nullptr, URNI_MM5_INTRODUCTION_VOICES, 10, 0x80000000U, delay + 20);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiMm5Same(shen_name reader, std::int32_t delay) {
    const std::int32_t readingEnd = EVT_AT_SetMot(reader, URNI_MM5_END_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_MM5_IDLE_MOTION, 2, 1, 0, readingEnd);
    EVT_AT_SetMot(URNI_PLAYER, URNI_PLAYER_PALM_END_MOTION, 2, 1, 0x40000000U, delay);
    const std::int32_t standing = EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_FROM_CHAIR_MOTION, 2, 1, 0, delay + 380);
    const std::int32_t shift = EVT_AT_SetMot(URNI_PLAYER, URNI_SHIFT_LEFT_MOTION, 2, 1, 0x20000000, standing - 2);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0x20000000, shift - 2) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetCam(1219, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1214, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 365);
    EVT_AT_SetSound(0x3e4, delay + 380);
    QueueEventVoiceList(nullptr, URNI_MM5_SAME_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiMm5Decline(shen_name reader, std::int32_t delay) {
    EVT_AT_SetPos(URNI_PLAYER, reader, {-0.44999999f, 0, -1.45f}, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2, 1, 0x40000000U, delay);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0, delay + 188) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetMot(reader, URNI_MM5_DECLINE_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, delay + 10, 180, 0);
    EVT_AT_SetCam(1231, URNI_READER_DUMMY, URNI_READER_DUMMY, -0.0f, 0, delay);
    QueueEventVoiceList(nullptr, URNI_MM5_DECLINE_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiMm5BeginReading(shen_name reader, std::int32_t delay) {
    const std::int32_t sit = delay + 90;
    EVT_AT_SetPosAng(URNI_PLAYER, reader, 0.1f, -0.89999998f, 180, sit);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_SIT_MOTION, 2, 1, 0, sit);
    EVT_AT_SetMot(URNI_PLAYER, URNI_SITTING_MOTION, 2, 1, 0, end);
    EVT_AT_SetCam(1210, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, sit);
    EVT_AT_SetSound(0x3e1, sit);
    QueueEventVoiceList(nullptr, URNI_MM5_BEGIN_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(delay + 140);
}
std::int64_t UranaiMm5Reading(shen_name reader, std::int32_t delay) {
    const std::int32_t loop = EVT_AT_SetMot(reader, URNI_MM5_START_MOTION, 90, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_MM5_LOOP_MOTION, 2, 1, 0, loop);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_PLAYER_PALM_START_MOTION, 90, 1, 0x40000000U, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_PLAYER_PALM_LOOP_MOTION, 2, 1, 0, end);
    EVT_AT_SetCam(1211, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(nullptr, urniMm5ReadingVoices, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiMm5Result(shen_name reader, std::int32_t) {
    std::int32_t special = -1;
    const char *const *voices;
    if (g_actNumber < 3) {
        if (shenGetRandomInt32(10) == 0 && EV_GetShenmueFlag(1147)) {
            if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 2000)) {
                special = 1;
                voices = URNI_MM5_SPECIAL_VOICES[1];
            } else {
                EV_stepSetFlags(0, 1147);
                special = 0;
                voices = URNI_MM5_SPECIAL_VOICES[0];
            }
            QueueEventVoiceList(HLib::GetCurrentTask(), voices, 10, 0x80000000U, 0);
            SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(reader), 13, 30, 1);
        } else if (shenGetRandomInt32(5) == 0 && EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 2000)) {
            special = shenGetRandomInt32(EV_GetShenmueFlag(1148) ? 2 : 3);
            voices = URNI_MM5_SPECIAL_VOICES[special + 1];
            QueueEventVoiceList(HLib::GetCurrentTask(), voices, 10, 0x80000000U, 0);
            SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(reader), 13, 30, 1);
        } else {
            std::int32_t index = 19;
            while (index > 0) {
                if (EV_GetShenmueFlag(URNI_MM5_HK_FLAGS[index]))
                    break;
                --index;
            }
            if (index == static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 42)))
                return 0xffffffffLL;
            EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 42, index);
            EV_stepSetFlags(0, URNI_MM5_HK_FLAGS[index]);
            QueueEventVoiceList(HLib::GetCurrentTask(), URNI_MM5_HK_VOICES[index], 10, 0x80000000U, 0);
        }
    } else if (shenGetRandomInt32(3) != 0 || !EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 2000)) {
        std::int32_t index = 14;
        while (index > 0) {
            if (EV_GetShenmueFlag(URNI_MM5_KL_FLAGS[index]))
                break;
            --index;
        }
        if (index == static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 42)))
            return 0xffffffffLL;
        EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 42, index);
        EV_stepSetFlags(0, URNI_MM5_KL_FLAGS[index]);
        QueueEventVoiceList(HLib::GetCurrentTask(), URNI_MM5_KL_VOICES[index], 10, 0x80000000U, 0);
    } else {
        special = shenGetRandomInt32(4);
        QueueEventVoiceList(HLib::GetCurrentTask(), URNI_MM5_SPECIAL_VOICES[special + 1], 10, 0x80000000U, 0);
        SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(reader), 13, 30, 1);
    }
    EventSetCamera camera{.cleanup = 0,
        .camera = URNI_MM5_CAMERAS[special + 1],
        .eye = URNI_READER_DUMMY,
        .target = URNI_READER_DUMMY,
        .context = 0,
        .reserved11 = {},
        .angle = 0,
        .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    return 0;
}
std::int64_t UranaiMm5Payment(shen_name reader, std::int32_t delay) {
    EVT_AT_SetFaceLooks(reader, 0, 30, 1, delay);
    const std::int32_t readingEnd = EVT_AT_SetMot(reader, URNI_MM5_END_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_MM5_IDLE_MOTION, 2, 1, 0, readingEnd);
    EVT_AT_SetMot(URNI_PLAYER, URNI_PLAYER_PALM_END_MOTION, 2, 1, 0x40000000U, delay);
    const std::int32_t standing = EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_FROM_CHAIR_MOTION, 2, 1, 0, delay + 215);
    const std::int32_t shift = EVT_AT_SetMot(URNI_PLAYER, URNI_SHIFT_LEFT_MOTION, 2, 1, 0x20000000, standing - 2);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0x20000000, shift - 2) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetCam(1219, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1214, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 200);
    EVT_AT_SetSound(0x3e4, delay + 215);
    QueueEventVoiceList(nullptr, URNI_MM5_PAYMENT_VOICES, 10, 0x80000000U, delay);
    IncrementSteamStat(21);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiMm5InsufficientCash(shen_name reader, std::int32_t delay) {
    const std::int32_t wallet = delay + 80;
    PATI_StartMoneyCount(0, wallet);
    EVT_AT_SetPos(URNI_PLAYER, reader, {-0.44999999f, 0, -1.45f}, wallet);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, wallet);
    EVT_AT_SetMot(URNI_PLAYER, URNI_EMPTY_WALLET_MOTION, 0, 0.5f, 0, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2, 1, 0x40000000U, wallet);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0, delay + 298) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetMot(reader, URNI_MM5_IDLE_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, wallet, 220, 0);
    EVT_AT_SetCam(1231, URNI_READER_DUMMY, URNI_READER_DUMMY, -0.0f, 0, wallet);
    QueueEventVoiceList(nullptr, URNI_MM5_INSUFFICIENT_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
} // namespace shenmue::s2
