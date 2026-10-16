#include "s2/areas/area_uede.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/help.hpp"
#include "s2/inventory.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/save.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2::areas {
namespace {
void Camera(std::int32_t id, shen_name character = UEDE_BASE) {
    EventSetCamera camera{.camera = id, .eye = character, .target = character};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
}
void Face(UedeWork &work, FacialExpressionId expression) {
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(work.opponent), static_cast<std::int32_t>(expression), 16, 1);
}
void Motion(shen_name character, std::uint32_t animation) {
    EventSetMotion motion{.character = character, .animation = animation, .speed = 1, .flags = 0x40000000U};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(static_cast<std::uint16_t>(animation));
}
void ContinueVoice() {
    static constexpr const char *voices[]{"055", "056", "057"};
    UedeAppendVoice(1, voices[shenGetRandomInt32(3)]);
}
void CashVoice() {
    static constexpr const char *lower[]{"062", "063"};
    static constexpr const char *higher[]{"064", "065"};
    if (uedeStartingCash > static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)))
        UedeAppendVoice(1, lower[shenGetRandomInt32(2)]);
    if (uedeStartingCash < static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)))
        UedeAppendVoice(1, higher[shenGetRandomInt32(2)]);
}
void ContinueResult(UedeWork &work) {
    std::uint32_t flags = 0;
    if (shenGetMinutesUntilNextFrameMinute()) {
        ContinueVoice();
        CashVoice();
        if (!UedeCheckMoney(work))
            flags = 0x20000000U;
    } else
        HLib::SetCurrentTaskTo(UedeSleep);
    UedeQueueVoices(work.self, flags);
}
} // namespace
void UedeJudge(void *data) {
    UedeWork &work = *static_cast<UedeWork *>(data);
    if (g_currentMap.name != UEDE_AREA.name)
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(uedePlayedFlags[uedeOpponentIndex]), 1);
    const std::int16_t opponent = static_cast<std::int16_t>(work.opponentDice[0] + work.opponentDice[1]);
    const std::int16_t player = static_cast<std::int16_t>(work.playerDice[0] + work.playerDice[1]);
    EventSetPositionAngle position{.character = work.opponent, .parent = UEDE_BASE, .position = {0, 0, -.9f}, .degrees = 170};
    EVT_AT_SetPosAng_Callback(&position);
    Motion(UEDE_RYO, 4424);
    Motion(work.opponent, uedeMiddleMotions[uedeOpponentPose]);
    EventSetPoint ryo{.character = UEDE_RYO, .target = work.opponent, .targetBone = 5, .trackingMode = 3, .flags = 0x20000000U};
    EVT_AT_SetPoint_Callback(&ryo);
    EventSetPoint opponentLook{.character = work.opponent, .target = UEDE_RYO, .targetBone = 5, .trackingMode = 3, .flags = 0x20000000U};
    EVT_AT_SetPoint_Callback(&opponentLook);
    Camera(3700);
    HLib::SetCurrentTaskTo(opponent < player ? UedeWin : opponent == player ? UedeDraw : UedeLose);
    work.played = 1;
    work.repeat = 1;
}
void UedeWin(void *data) {
    UedeWork &work = *static_cast<UedeWork *>(data);
    if (work.state) {
        if (!GetShouldRenderIcons(RenderIconSlot::Any)) {
            UedeCheckMoney(work);
            work.state = 0;
        }
        return;
    }
    Face(work, FacialExpressionId::Pitiable);
    static constexpr const char *voices[]{"042", "043", "044"};
    UedeAppendVoice(1, voices[shenGetRandomInt32(3)]);
    std::uint32_t flags = 0;
    if (shenGetMinutesUntilNextFrameMinute()) {
        if (++uedeWinStreak == 5 && g_actNumber < 3)
            HLib::SetCurrentTaskTo(UedeAwardMedal);
        else {
            ContinueVoice();
            static constexpr const char *higher[]{"058", "059"};
            static constexpr const char *lower[]{"060", "061"};
            if (uedeStartingCash < static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) + 2 * work.wager)
                UedeAppendVoice(1, higher[shenGetRandomInt32(2)]);
            if (uedeStartingCash > static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) + 2 * work.wager)
                UedeAppendVoice(1, lower[shenGetRandomInt32(2)]);
            ++work.state;
            flags = 0x20000000U;
        }
    } else
        HLib::SetCurrentTaskTo(UedeSleep);
    UedeQueueVoices(work.self, flags);
    AddThisToCashOrSegaCoins(2 * work.wager, ECashOrCoins::Cash2);
}
void UedeDraw(void *data) {
    UedeWork &work = *static_cast<UedeWork *>(data);
    uedeWinStreak = 0;
    Face(work, FacialExpressionId::Smile);
    static constexpr const char *voices[]{"045", "046", "047"};
    UedeAppendVoice(1, voices[shenGetRandomInt32(3)]);
    ContinueResult(work);
}
void UedeLose(void *data) {
    UedeWork &work = *static_cast<UedeWork *>(data);
    static constexpr const char *voices[]{"048", "049", "050"};
    static constexpr const char *follow[]{"051", "052"};
    UedeAppendVoice(1, voices[shenGetRandomInt32(3)]);
    UedeAppendVoice(1, follow[shenGetRandomInt32(2)]);
    Face(work, FacialExpressionId::Smile);
    uedeWinStreak = 0;
    ContinueResult(work);
}
void UedeAwardMedal(void *data) {
    UedeWork &work = *static_cast<UedeWork *>(data);
    switch (work.state) {
    case 0:
        Camera(4000, work.opponent);
        EV_DisableCharacterDisplay(UEDE_MEDAL);
        ++work.state;
        UedeAppendVoice(1, "053");
        UedeAppendVoice(1, "054");
        Face(work, FacialExpressionId::Smile);
        UedeQueueVoices(work.self);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1644, 1);
        break;
    case 1:
        Loop3DSoundEffect(0xbef);
        ++work.state;
        HLib::SetupTaskDelay(10);
        break;
    case 2: {
        EV_EnableCharacterDisplay(UEDE_MEDAL);
        EventSetPosition position{.character = UEDE_MEDAL, .parent = UEDE_BASE, .position = {.3f, .77f, .1f}};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle angle{.character = UEDE_MEDAL, .parent = UEDE_BASE, .rotation = {49152, 0, 0}};
        EVT_AT_SetAng_Callback(&angle);
        Camera(4001);
        AddInventoryItem(0x1b, 1, 1);
        ++work.state;
        break;
    }
    case 3:
        if (!GetShouldRenderIcons(RenderIconSlot::Any)) {
            EV_DisableCharacterDisplay(UEDE_MEDAL);
            Camera(3700);
            if (shenGetMinutesUntilNextFrameMinute()) {
                ContinueVoice();
                static constexpr const char *higher[]{"058", "059"};
                static constexpr const char *lower[]{"060", "061"};
                UedeAppendVoice(1, (uedeStartingCash >= static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) ? lower : higher)[shenGetRandomInt32(2)]);
                UedeQueueVoices(work.self, UedeCheckMoney(work) ? 0 : 0x20000000U);
                work.state = 0;
            } else {
                HLib::SetCurrentTaskTo(UedeSleep);
                work.state = 0;
            }
        }
        break;
    default:
        break;
    }
}
} // namespace shenmue::s2::areas
