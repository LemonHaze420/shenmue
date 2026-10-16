#include "s2/areas/area_urni.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/event_voice.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/motion.hpp"
#include "s2/qte_hand_poses.hpp"
#include "s2/step_event.hpp"

namespace shenmue::s2 {
std::int64_t UranaiSehSetup(shen_name reader, std::int32_t) {
    EventSetPosition position{.cleanup = 0, .character = URNI_STICK_CUP, .parent = reader, .position = URNI_SEH_CUP, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = URNI_STICK_CUP, .parent = reader, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    EventCling cling{.cleanup = 0,
        .target = URNI_STICK_HAND,
        .character = reader,
        .bone = 21,
        .position = URNI_SEH_ATTACH,
        .rotation = URNI_SEH_ROTATE,
        .mode = 0,
        .delay = 0};
    EVT_AT_Cling(&cling);
    EV_DisableCharacterDisplay(URNI_STICK_HAND);
    EV_EnableCharacterDisplay(URNI_STICK_CUP);
    shen_name name = URNI_STICK_TABLE_FIRST;
    for (std::int32_t i = 0; i < 6; ++i, name.name += 0x1000000U) {
        position.character = name;
        position.position = URNI_SEH_TABLE[i];
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle rotation{.cleanup = 0, .character = name, .parent = reader, .rotation = {49152, 0, 0}, .delay = 0};
        EVT_AT_SetAng_Callback(&rotation);
        EV_DisableCharacterDisplay(name);
        SetMotionworkFlagForCharaById(name, &URNI_SEH_SCALE);
    }
    name = URNI_STICK_HELD_FIRST;
    for (std::int32_t i = 0; i < 5; ++i, name.name += 0x1000000U) {
        cling.target = name;
        cling.position = URNI_SEH_HAND[i];
        cling.rotation = URNI_SEH_HAND_ROTATE;
        EVT_AT_Cling(&cling);
        EV_DisableCharacterDisplay(name);
    }
    name = URNI_STICK_IDLE_FIRST;
    for (std::int32_t i = 0; i < 5; ++i, name.name += 0x1000000U) {
        position.character = name;
        position.position = URNI_SEH_IDLE[i];
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle rotation{.cleanup = 0, .character = name, .parent = URNI_INVALID, .rotation = {47331, 0, 0}, .delay = 0};
        EVT_AT_SetAng_Callback(&rotation);
        EV_EnableCharacterDisplay(name);
    }
    return 0;
}
std::int64_t UranaiSehModeOne(shen_name reader, std::int32_t delay) {
    const std::int32_t end = delay + 180;
    EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_SEH_AWAKE_MOTION, 100, 1, 0x50000000, delay);
    EVT_AT_SetCam(1217, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(nullptr, urniSehModeOneVoices, 10, 0x80000000U, delay);
    EV_DisableCharacterDisplay(URNI_PLAYER);
    EVT_AT_ObjectDisp_1(URNI_PLAYER, end, 0);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiSehIntroduction(shen_name reader, std::int32_t delay) {
    const std::int32_t end = delay + 360;
    for (std::int32_t i = 18; i > 0; --i)
        if (EV_GetShenmueFlag(URNI_SEH_FLAGS[i]))
            break;
    if (static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 45)) == -100)
        return 0;
    EVT_AT_SetPos(URNI_PLAYER, reader, {-0.44999999f, 0, -1.45f}, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_SEH_AWAKE_MOTION, 100, 1, 0x50000000, delay);
    EVT_AT_SetCam(1208, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1209, URNI_PLAYER_DUMMY, URNI_PLAYER_DUMMY, 0, 0, end);
    QueueEventVoiceList(nullptr, URNI_SEH_INTRODUCTION_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiSehSame(shen_name, std::int32_t delay) {
    const std::int32_t standing = EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_FROM_CHAIR_MOTION, 2, 1, 0, delay + 350);
    const std::int32_t shift = EVT_AT_SetMot(URNI_PLAYER, URNI_SHIFT_LEFT_MOTION, 2, 1, 0x20000000, standing - 2);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0x20000000, shift - 2) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetCam(1219, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1214, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 340);
    EVT_AT_SetSound(0x3e4, delay + 350);
    QueueEventVoiceList(nullptr, URNI_SEH_SAME_VOICES, 10, 0x80000000U, delay);
    shen_name name = URNI_STICK_TABLE_FIRST;
    for (std::int32_t i = 0; i < 6; ++i, name.name += 0x1000000U)
        EVT_AT_ObjectDisp_1(name, end, 1);
    name = URNI_STICK_IDLE_FIRST;
    for (std::int32_t i = 0; i < 5; ++i, name.name += 0x1000000U)
        EVT_AT_ObjectDisp_1(name, end, 0);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiSehDecline(shen_name reader, std::int32_t delay) {
    EVT_AT_SetPos(URNI_PLAYER, reader, {-0.44999999f, 0, -1.45f}, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2, 1, 0x40000000U, delay);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0, delay + 278) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetMot(reader, URNI_SEH_AWAKE_MOTION, 100, 1, 0x50000000, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, delay + 10, 270, 0);
    EVT_AT_SetCam(1215, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(nullptr, URNI_SEH_DECLINE_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiSehBeginReading(shen_name reader, std::int32_t delay) {
    const std::int32_t sit = delay + 120;
    EVT_AT_SetPosAng(URNI_PLAYER, reader, 0, -1.5f, 180, sit);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_SIT_MOTION, 2, 1, 0, sit);
    EVT_AT_SetMot(URNI_PLAYER, URNI_SITTING_MOTION, 2, 1, 0, end);
    EVT_AT_SetMot(reader, URNI_SEH_START_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetCam(1210, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, sit);
    EVT_AT_SetSound(0x3e1, sit);
    QueueEventVoiceList(nullptr, URNI_SEH_BEGIN_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiSehReading(shen_name reader, std::int32_t delay) {
    EVT_AT_SetMot(reader, URNI_SEH_READING_MOTION, 0, 1, 0x40000000U, delay);
    EVT_AT_SetCam(1204, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1232, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 60);
    EVT_AT_SetCam(1233, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 250);
    EVT_AT_SetCam(1234, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 340);
    QueueEventVoiceList(nullptr, urniSehReadingVoices, 10, 0x80000000U, delay);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(reader), 0, reinterpret_cast<std::byte *>(fortuneReadingHandPose), 60);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(reader), 1, reinterpret_cast<std::byte *>(fortuneReadingHandPose), 60);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(reader), 0, reinterpret_cast<std::byte *>(fortuneLeftReadingHandPose), 250);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(reader), 1, reinterpret_cast<std::byte *>(fortuneLeftReadingHandPose), 250);
    shen_name name = URNI_STICK_HELD_FIRST;
    for (std::int32_t i = 0; i < 5; ++i, name.name += 0x1000000U)
        EVT_AT_ObjectDisp_1(name, delay + 60, 0);
    name = URNI_STICK_IDLE_FIRST;
    for (std::int32_t i = 0; i < 5; ++i, name.name += 0x1000000U)
        EVT_AT_ObjectDisp_1(name, delay + 60, 1);
    name = URNI_STICK_HELD_FIRST;
    for (std::int32_t i = 0; i < 5; ++i, name.name += 0x1000000U)
        EVT_AT_ObjectDisp_1(name, delay + 250, 1);
    name = URNI_STICK_TABLE_FIRST;
    for (std::int32_t i = 0; i < 6; ++i, name.name += 0x1000000U)
        EVT_AT_ObjectDisp_1(name, delay + 340, 0);
    EVT_AT_ObjectDisp_1(URNI_STICK_HAND, delay + 250, 0);
    EVT_AT_ObjectDisp_1(URNI_STICK_HAND, delay + 340, 1);
    return static_cast<std::uint32_t>(delay + 417);
}
std::int64_t UranaiSehResult(shen_name reader, std::int32_t) {
    std::int32_t index = 18;
    while (index > 0) {
        if (EV_GetShenmueFlag(URNI_SEH_FLAGS[index]))
            break;
        --index;
    }
    if (index == static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 45)))
        return 0xffffffffLL;
    EV_stepSetFlags(0, URNI_SEH_FLAGS[index]);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 45, index);
    EventSetMotion motion{
        .cleanup = 0, .character = reader, .animation = URNI_SEH_RESULT_START_MOTION, .frame = 2, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    const std::int32_t frames = static_cast<std::int32_t>(GetMotionFrameCount(URNI_SEH_RESULT_START_MOTION));
    EVT_AT_SetMot(reader, URNI_SEH_RESULT_LOOP_MOTION, 2, 1, 0, static_cast<std::int32_t>(static_cast<float>(frames) - 2.0f + 0.0f));
    EventSetCamera camera{
        .cleanup = 0, .camera = 1218, .eye = URNI_READER_DUMMY, .target = URNI_READER_DUMMY, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    QueueEventVoiceList(HLib::GetCurrentTask(), URNI_SEH_RESULT_VOICES[index], 10, 0x80000000U, 0);
    return 0;
}
std::int64_t UranaiSehPayment(shen_name reader, std::int32_t delay) {
    EVT_AT_SetMot(reader, URNI_SEH_RESULT_END_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_SEH_DROWSY_MOTION, 2, 1, 0, delay + 340);
    const std::int32_t standing = EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_FROM_CHAIR_MOTION, 2, 1, 0, delay + 350);
    const std::int32_t shift = EVT_AT_SetMot(URNI_PLAYER, URNI_SHIFT_LEFT_MOTION, 2, 1, 0x20000000, standing - 2);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0x20000000, shift - 2) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetCam(1219, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1214, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 340);
    EVT_AT_SetSound(0x3e4, delay + 350);
    QueueEventVoiceList(nullptr, URNI_SEH_PAYMENT_VOICES, 10, 0x80000000U, delay);
    shen_name name = URNI_STICK_TABLE_FIRST;
    for (std::int32_t i = 0; i < 6; ++i, name.name += 0x1000000U)
        EVT_AT_ObjectDisp_1(name, end, 1);
    name = URNI_STICK_IDLE_FIRST;
    for (std::int32_t i = 0; i < 5; ++i, name.name += 0x1000000U)
        EVT_AT_ObjectDisp_1(name, end, 0);
    IncrementSteamStat(21);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiSehInsufficientCash(shen_name reader, std::int32_t delay) {
    const std::int32_t wallet = delay + 80;
    PATI_StartMoneyCount(0, wallet);
    EVT_AT_SetPos(URNI_PLAYER, reader, {-0.44999999f, 0, -1.45f}, wallet);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, wallet);
    EVT_AT_SetMot(URNI_PLAYER, URNI_EMPTY_WALLET_MOTION, 0, 0.5f, 0, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2, 1, 0x40000000U, wallet);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0, delay + 248) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetMot(reader, URNI_SEH_AWAKE_MOTION, 100, 1, 0x50000000, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, wallet, 170, 0);
    EVT_AT_SetCam(1215, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, wallet);
    QueueEventVoiceList(nullptr, URNI_SEH_INSUFFICIENT_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
} // namespace shenmue::s2
