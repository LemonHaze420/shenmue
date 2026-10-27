#include "s2/areas/area_urni.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/event_voice.hpp"
#include "s2/game_runtime.hpp"
#include "s2/model_image.hpp"
#include "s2/motion.hpp"
#include "s2/step_event.hpp"

namespace shenmue::s2 {
std::int64_t UranaiSriSetup(shen_name reader, std::int32_t) {
    ReplaceScenarioImageModule(URNI_CRYSTAL, "SUISYO00", 0);
    const UranaiProjection &projection = URNI_SRI_PROJECTIONS[UranaiStoryPackageIndex()];
    AT_SetLocalBaseChara(projection.character, reader);
    EventSetPosition position{.cleanup = 0, .character = projection.character, .parent = reader, .position = projection.position, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = projection.character, .parent = reader, .degrees = projection.yaw, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    const Vector3 scale{projection.scale, projection.scale, projection.scale};
    mtwkSetMotionworkScale3(projection.character, &scale);
    SetScenarioCharacterTransparency(projection.character, 1);
    SetPluralControlFor(ctrlGetCharaByName(projection.character), 0);
    return 0;
}
std::int64_t UranaiSriModeOne(shen_name reader, std::int32_t delay) {
    const std::int32_t end = delay + 330;
    EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_SRI_TALK_TWO_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetCam(1216, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    QueueEventVoiceList(nullptr, urniSriModeOneVoices, 10, 0x80000000U, delay);
    EV_DisableCharacterDisplay(URNI_PLAYER);
    EVT_AT_ObjectDisp_1(URNI_PLAYER, end, 0);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiSriIntroduction(shen_name reader, std::int32_t delay) {
    const std::int32_t end = delay + 175;
    UranaiStoryPackageIndex();
    if (static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 44)) == -100)
        return 0;
    EVT_AT_SetPos(URNI_PLAYER, reader, {-0.44999999f, 0, -1.45f}, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_SRI_TALK_TWO_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetCam(1208, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1209, URNI_PLAYER_DUMMY, URNI_PLAYER_DUMMY, 0, 0, end);
    QueueEventVoiceList(nullptr, URNI_SRI_INTRODUCTION_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiSriSame(shen_name reader, std::int32_t delay) {
    const std::int32_t readingEnd = EVT_AT_SetMot(reader, URNI_SRI_END_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_SRI_TALK_TWO_MOTION, 2, 1, 0, readingEnd);
    const std::int32_t standing = EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_FROM_CHAIR_MOTION, 2, 1, 0, delay + 390);
    const std::int32_t shift = EVT_AT_SetMot(URNI_PLAYER, URNI_SHIFT_LEFT_MOTION, 2, 1, 0x20000000, standing - 2);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0x20000000, shift - 2) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetCam(1219, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1214, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 400);
    EVT_AT_SetSound(0x3e4, delay + 390);
    QueueEventVoiceList(nullptr, URNI_SRI_SAME_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiSriDecline(shen_name reader, std::int32_t delay) {
    EVT_AT_SetPos(URNI_PLAYER, reader, {-0.44999999f, 0, -1.45f}, delay);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2, 1, 0x40000000U, delay);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0, delay + 278) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetMot(reader, URNI_SRI_TALK_TWO_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, delay + 10, 270, 0);
    EVT_AT_SetCam(1231, URNI_READER_DUMMY, URNI_READER_DUMMY, -0.0f, 0, delay);
    QueueEventVoiceList(nullptr, URNI_SRI_DECLINE_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiSriBeginReading(shen_name reader, std::int32_t delay) {
    const std::int32_t sit = delay + 120;
    EVT_AT_SetPosAng(URNI_PLAYER, reader, -0.0f, -1.4f, 180, sit);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_SIT_MOTION, 2, 1, 0, sit);
    EVT_AT_SetMot(URNI_PLAYER, URNI_SITTING_MOTION, 2, 1, 0, end);
    EVT_AT_SetCam(1210, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, sit);
    EVT_AT_SetSound(0x3e1, sit);
    QueueEventVoiceList(nullptr, URNI_SRI_BEGIN_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiSriReading(shen_name reader, std::int32_t delay) {
    const std::int32_t loop = EVT_AT_SetMot(reader, URNI_SRI_START_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_SRI_LOOP_MOTION, 2, 1, 0, loop);
    EVT_AT_SetCam(1237, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    const std::int32_t index = UranaiStoryPackageIndex();
    if (index != static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 44)))
        QueueEventVoiceList(nullptr, URNI_SRI_READING_VOICES, 10, 0x80000000U, delay + 150);
    return static_cast<std::uint32_t>(delay + 340);
}
std::int64_t UranaiSriResult(shen_name reader, std::int32_t) {
    const std::int32_t index = UranaiStoryPackageIndex();
    if (index == static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 44)))
        return 0xffffffffLL;
    EV_stepSetFlags(0, URNI_SRI_STORY_FLAGS[index]);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 44, index);
    EV_EnableCharacterDisplay(URNI_SRI_PROJECTIONS[index].character);
    EventSetMotion motion{.cleanup = 0, .character = reader, .animation = URNI_SRI_TALK_MOTION, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(URNI_SRI_TALK_MOTION);
    EventSetCamera camera{
        .cleanup = 0, .camera = 1207, .eye = URNI_READER_DUMMY, .target = URNI_READER_DUMMY, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    QueueEventVoiceList(HLib::GetCurrentTask(), URNI_SRI_RESULT_VOICES[index], 10, 0x80000000U, 0);
    return 0;
}
std::int64_t UranaiSriPayment(shen_name reader, std::int32_t delay) {
    EVT_AT_ObjectDisp_1(URNI_SRI_PROJECTIONS[UranaiStoryPackageIndex()].character, delay, 1);
    const std::int32_t readingEnd = EVT_AT_SetMot(reader, URNI_SRI_END_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetMot(reader, URNI_SRI_TALK_TWO_MOTION, 2, 1, 0, readingEnd);
    const std::int32_t standing = EVT_AT_SetMot(URNI_PLAYER, URNI_STAND_FROM_CHAIR_MOTION, 2, 1, 0, delay + 320);
    const std::int32_t shift = EVT_AT_SetMot(URNI_PLAYER, URNI_SHIFT_LEFT_MOTION, 2, 1, 0x20000000, standing - 2);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0x20000000, shift - 2) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetCam(1219, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay);
    EVT_AT_SetCam(1214, URNI_READER_DUMMY, URNI_READER_DUMMY, 0, 0, delay + 330);
    EVT_AT_SetSound(0x3e4, delay + 320);
    QueueEventVoiceList(nullptr, URNI_SRI_PAYMENT_VOICES, 10, 0x80000000U, delay);
    IncrementSteamStat(21);
    return static_cast<std::uint32_t>(end);
}
std::int64_t UranaiSriInsufficientCash(shen_name reader, std::int32_t delay) {
    const std::int32_t wallet = delay + 80;
    PATI_StartMoneyCount(0, wallet);
    EVT_AT_SetPos(URNI_PLAYER, reader, {-0.44999999f, 0, -1.45f}, wallet);
    EVT_AT_SetAngY(URNI_PLAYER, reader, 180, wallet);
    EVT_AT_SetMot(URNI_PLAYER, URNI_EMPTY_WALLET_MOTION, 0, 0.5f, 0, delay);
    EVT_AT_SetMot(URNI_PLAYER, URNI_TALK_MOTION, 2, 1, 0x40000000U, wallet);
    const std::int32_t turn = EVT_AT_SetMot(URNI_PLAYER, URNI_TURN_MOTION, 2, 0.34999999f, 0, delay + 338) - 7;
    EVT_AT_SetAngY(URNI_PLAYER, URNI_PLAYER, 180, turn);
    const std::int32_t end = EVT_AT_SetMot(URNI_PLAYER, URNI_WALK_MOTION, 2, 1, 0, turn);
    EVT_AT_SetMot(reader, URNI_SRI_TALK_TWO_MOTION, 2, 1, 0x40000000U, delay);
    EVT_AT_SetPoint(URNI_PLAYER, reader, 5, 2, 0, wallet, 260, 0);
    EVT_AT_SetCam(1231, URNI_READER_DUMMY, URNI_READER_DUMMY, -0.0f, 0, wallet);
    QueueEventVoiceList(nullptr, URNI_SRI_INSUFFICIENT_VOICES, 10, 0x80000000U, delay);
    return static_cast<std::uint32_t>(end);
}
} // namespace shenmue::s2
