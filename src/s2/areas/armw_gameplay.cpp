#include "s2/areas/area_armw.hpp"
#include "s2/battle_qte.hpp"
#include "s2/camera.hpp"
#include "s2/conversation.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/player.hpp"
#include "s2/pause.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <algorithm>
#include <bit>
#include <cmath>
#include <limits>
namespace shenmue::s2::areas {
namespace {
float SnapZero(float value) {
    return std::fabs(value) < std::numeric_limits<float>::epsilon() ? 0.0f : value;
}
void Face(shen_name character, std::int32_t expression, float frames) {
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(character), expression, frames, 1);
}
std::uint32_t ArmwButtons(ArmwWork &work) {
    std::uint32_t buttons = GetCurrentControlInput(7) == 0x400 ? 1 : 0;
    GetBattleDataConfig(0, ARMW_BATTLE_INPUT);
    std::int32_t adjustment = 0;
    switch (work.strategy) {
    case 1:
        if (work.position > 0.75f && work.stamina > 0) {
            if (!work.staminaVoice && !cvsHasValidConverseData(0))
                work.staminaVoice = 1;
            work.stamina -= 3;
            adjustment = 6;
        } else if (work.position > 0.65f && work.stamina > 0) {
            --work.stamina;
            adjustment = 3;
        } else {
            work.staminaVoice = 0;
            adjustment = -3;
        }
        break;
    case 2:
        if (!work.staminaVoice && !cvsHasValidConverseData(0))
            work.staminaVoice = 1;
        adjustment = work.stamina-- > 0 ? 6 : -3;
        break;
    case 3:
        adjustment = work.stamina-- > 0 ? 0 : 5;
        break;
    case 4:
        if (work.stamina <= 0)
            adjustment = -4;
        else {
            if (work.position > 0.6f)
                --work.stamina;
            if (work.position <= 0.5f)
                adjustment = 2;
            else {
                work.stamina -= 2;
                work.strategyFlags |= 1;
            }
        }
        break;
    case 5:
    case 6:
        if (work.position > 0.65f)
            --work.stamina;
        if (work.position > 0.75f) {
            const std::int32_t stamina = work.stamina--;
            if (stamina > 0) {
                work.strategyFlags |= 1;
                work.stamina = stamina - 3;
            }
        }
        adjustment = work.initialStamina / 2 >= work.stamina ? -2 : 2;
        if (work.strategy == 6 && work.position < 0.4f)
            work.stamina = std::min(work.initialStamina, work.stamina + 1);
        break;
    default:
        break;
    }
    work.stamina = std::max(work.stamina, 0);

    std::int32_t strength = 0;
    if (adjustment + work.strength + shenGetRandomInt32(2) < 0 || adjustment + work.strength + shenGetRandomInt32(2) < 31) {
        if (adjustment + work.strength + shenGetRandomInt32(2) >= 0)
            strength = adjustment + work.strength + shenGetRandomInt32(2);
    } else
        strength = 31;
    if (strength > shenGetRandomInt32(32))
        buttons |= 2;
    return buttons;
}
void UpdateFaces(ArmwWork &work) {
    const std::int32_t player = work.position < 0.2f ? Pitiable : work.position < 0.4f ? Suffer : Angry;
    const std::int32_t opponent = work.position < 0.2f ? Scheme : work.position < 0.4f ? Angry : work.position < 0.6f ? Suffer : Pitiable;
    if (player != work.playerFace)
        Face(work.player, player, 10);
    if (opponent != work.opponentFace)
        Face(work.opponent, opponent, 10);
    work.playerFace = player;
    work.opponentFace = opponent;
}
} // namespace
void ArmwMatchHands(ArmwWork &work) {
    constexpr std::int16_t player[]{1029, 1028, 1028, 1027}, opponent[]{1034, 1026, 1026, 1024};
    for (std::int32_t side = 0; side < 2; ++side) {
        ConfigureCharacterHandPoseSequence(ctrlGetCharaByName(work.player), side, player[armw.opponentMotion], 0, 1, 0, 4, 1, 0);
        ConfigureCharacterHandPoseSequence(ctrlGetCharaByName(work.opponent), side, opponent[armw.opponentMotion], 0, 1, 0, 4, 1, 0);
    }
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1293, 1);
}
void ArmwRestoreHands(ArmwWork &work) {
    SetHighModelXYZ(ctrlGetCharaByName(work.player), 1, 1, 0);
    SetHighModelXYZ(ctrlGetCharaByName(work.opponent), 1, 1, 0);
    SetHighModelXYZ(ctrlGetCharaByName(work.referee), 1, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(work.referee), 1, 1, 0);
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    FinishLoading();
    work.step = 0;
}
void ArmwUpdatePose(ArmwWork &work) {
    constexpr std::uint32_t playerMotions[]{9169, 9164, 9164, 9168}, opponentMotions[]{9359, 9292, 9292, 9231};
    const float angle = SnapZero(SnapZero(1 - SnapZero(work.position + work.shake)) * 90) + 90;
    ArmwPosition(work.player, ARMW_BASE, {-0.03f, 0, -0.8f}, 32768);
    switch (armw.opponentMotion) {
    case 0:
        ArmwPosition(work.opponent, work.player, {0, 0, -1.383f}, 32768);
        break;
    case 1:
    case 2:
        ArmwPosition(work.opponent, work.player, {0, 0, -1.391f}, 0);
        break;
    case 3:
        ArmwPosition(work.opponent, work.player, {0, 0, -1.533f}, 32768);
        break;
    }
    EVT_AT_SetMot(work.player, playerMotions[armw.opponentMotion], angle, 0, 0x44000000U, 0);
    EVT_AT_SetMot(work.opponent, opponentMotions[armw.opponentMotion], angle, 0, 0x44000000U, 0);
}
void ArmwResetMatch(ArmwWork &work) {
    EV_LoadAudio("0008A000", 1);
    ArmwNullsub();
    work.target = 0.5f;
    work.opponentRate = 0;
    work.presses = 0;
    work.previousFrames = 0;
    work.reserveda0 = 0;
    work.strategyFlags = 0;
    work.fatigueVoice = 0;
    work.position = 0.5f;
    work.acceleration = -0.00001f;
    work.opponentFace = Angry;
    work.playerHistory = 0xff000000U;
    work.opponentHistory = 0xff000000U;
    work.playerFace = Angry;
    work.recentRate = 2;
    work.stalemate = 250;
    work.reservedb0 = 50;
    work.fatigueFrames = 0;
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1293, 0);
    Face(work.player, Angry, 0);
    Face(work.opponent, Angry, 0);
    EVS_LoopCamera(shenGetRandomInt32(3) + 5000, ARMW_BASE, ARMW_BASE);
    camCtrlDisableFilmBars();
    SetHighModelXYZ(ctrlGetCharaByName(work.player), 0, 1, 0);
    SetHighModelXYZ(ctrlGetCharaByName(work.opponent), 0, 1, 0);
    SetHighModelXYZ(ctrlGetCharaByName(work.referee), 0, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(work.referee), 0, 1, 0);
}
std::int32_t ArmwPlayMatch(ArmwWork &work) {
    if (work.previousFrames < 15 && work.frames >= 15)
        Loop3DSoundEffect(0x847);
    work.previousFrames = work.frames;
    work.frames += g_stabilizeFps ? GetFrameTime() * 30.0f : 1.0f;
    work.strategyFlags &= ~1U;
    const std::uint32_t buttons = ArmwButtons(work);
    if (buttons & 1)
        ++work.presses;
    work.playerHistory = (work.playerHistory << 1) | (buttons & 1);
    work.opponentHistory = (work.opponentHistory << 1) | ((buttons >> 1) & 1);
    const std::uint32_t rate = std::popcount(work.playerHistory);
    std::uint32_t fatigue = work.fatigueFrames ? 3 : 0;
    if (rate >= static_cast<std::uint32_t>(work.recentRate * 0.7) || work.frames <= static_cast<std::uint16_t>(work.warmupFrames)) {
        if (work.fatigueFrames && --work.fatigueFrames == 0) {
            work.fatigueVoice = 0;
            fatigue = 0;
        }
    } else {
        if (!(fatigue & 2)) {
            fatigue |= 3;
            for (std::uint32_t bit = 1, count = 32 / work.recentRate + 3; count; --count, bit <<= 1)
                if (work.playerHistory & bit)
                    fatigue &= ~1U;
        }
        work.fatigueFrames = 10;
    }
    float playerRate = 0, opponentRate = 0;
    for (std::uint32_t bit = 1; bit < 0x10000; bit <<= 1) {
        if (work.playerHistory & bit)
            playerRate += g_stabilizeFps ? (1.0f / (((1.0f * 30.0f) * GetFrameTime()) / 1.0f)) * 1.0f : 1.0f;
        if (work.opponentHistory & bit)
            opponentRate += shenGetScaledDeltaTickTime(1);
    }
    if (work.opponentRate - opponentRate > 1)
        opponentRate = work.opponentRate - 1;
    if (work.opponentRate - opponentRate < -1)
        opponentRate = work.opponentRate + 1;
    opponentRate = std::clamp(opponentRate, static_cast<float>(work.minimumStrength), static_cast<float>(work.maximumStrength));
    work.opponentRate = opponentRate;
    const std::int32_t frame = static_cast<std::int32_t>(work.frames), previous = static_cast<std::int32_t>(work.previousFrames);
    if (frame % 60 == 0 && previous % 60 != 0)
        Loop3DSoundEffect(188);
    else if (frame % 120 == 0 && previous % 120 != 0)
        Loop3DSoundEffect(187);
    float movement;
    if (fatigue & 2)
        opponentRate = std::max(2.0f, opponentRate);
    if (!(fatigue & 2) || work.settling) {
        work.acceleration = opponentRate <= playerRate ? 0.001f : -0.001f;
        movement = std::clamp(SnapZero((playerRate - opponentRate) * 0.001f), -0.06f, 0.06f);
        if (work.strategyFlags & 1) {
            movement = 0;
            work.target = work.position;
        }
        if (work.frames > 5400)
            movement = -0.06f;
    } else {
        work.acceleration -= 0.002f;
        if (!work.fatigueVoice) {
            work.fatigueVoice = 1;
            ArmwVoice(3, "011");
        }
        work.acceleration = std::max(work.acceleration, -0.03f);
        movement = std::clamp(SnapZero(opponentRate * work.acceleration), -0.14f, 0.14f);
    }
    if (work.settling) {
        if (work.position - work.target < 0.002 && work.position - work.target > -0.002)
            work.settling = 0;
    } else {
        if (g_stabilizeFps)
            movement = (movement * 30.0f) * GetFrameTime();
        work.target = SnapZero(std::clamp(work.target + movement, 0.0f, 1.0f));
    }
    if (frame % 10 == 0 && previous % 10 != 0 && rate > 2)
        work.recentRate = rate;
    const std::int32_t difference = static_cast<std::int32_t>(std::fabs(playerRate - opponentRate));
    if (difference < 3 && !work.settling) {
        if (!(fatigue & 3) && !work.fatigueFrames)
            work.stalemate -= difference == 0 ? 4 : difference == 1 ? 2 : 1;
        if (work.stalemate <= 0) {
            if (buttons & 1) {
                work.step = 0;
                work.phase = 11;
            } else
                work.stalemate = 0;
        }
    }
    work.shake = SnapZero(SnapZero(GetRandomNumberInRange(20) - 10) * 0.0001f);
    ArmwUpdatePose(work);
    UpdateFaces(work);
    const float differenceToTarget = work.target - work.position;
    if (std::fabs(differenceToTarget) >= std::numeric_limits<float>::epsilon()) {
        const float step = fatigue & 2 ? 0.12f : 0.008f;
        if (differenceToTarget > step)
            work.position += step;
        else if (differenceToTarget < -step)
            work.position -= step;
        else
            work.position = work.target;
    }
    if (!work.settling) {
        if (work.position >= 0.85f) {
            work.step = 0;
            work.phase = 12;
            work.position = 0.8f;
            work.target = 0.8f;
        } else if (work.position < 0.01f) {
            ArmwRestoreHands(work);
            EVS_LoopCamera(1306, ARMW_BASE, ARMW_BASE);
            camCtrlDisableFilmBars();
            work.flags |= 2;
            work.step = 0;
            work.phase = 16;
            work.position = -1.75f;
        }
    }
    return 0;
}
std::int32_t ArmwDirectionQte(ArmwWork &work) {
    if (!work.step) {
        armw.qtePenalty = 0;
        CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
        EVS_LoopCamera(shenGetRandomInt32(2) + 5010, ARMW_BASE, ARMW_BASE);
        armw.qteButton = shenGetRandomInt32(100) < work.buttonUp ? 1 :
            shenGetRandomInt32(100) < work.buttonDown            ? 16 :
            shenGetRandomInt32(100) < work.buttonLeft            ? 4 :
                                                                   64;
        SetShowQteFlag(armw.qteButton, 18, 2);
        EVT_SetQteButtonState(64511);
        ++work.step;
        return 0;
    }
    if (!GetShouldRenderIcons(RenderIconSlot::Qte)) {
        const std::int32_t band = work.position < 0.1f ? 0 :
            work.position < 0.22f                      ? 1 :
            work.position < 0.36f                      ? 2 :
            work.position < 0.48f                      ? 3 :
            work.position < 0.6f                       ? 4 :
            work.position < 0.72f                      ? 5 :
                                                         6;
        constexpr const char *success[2][7]{{"020", "023", "020", "019", "022", "021", "009"}, {"016", "018", "010", "021", "008", "009", "007"}};
        constexpr const char *failure[2][7]{{"012", "009", "018", "016", "015", "014", "013"}, {"013", "013", "015", "014", "012", "049", "015"}};
        if ((EV_GetCurrentQteStatus() & 0xfff) == armw.qteButton) {
            ArmwVoice(armw.qteSpeaker ? 0 : 3, success[armw.qteSpeaker][band]);
            work.target = (work.target + 0.082727268f) - armw.qtePenalty;
        } else {
            ArmwVoice(armw.qteSpeaker ? 0 : 3, failure[armw.qteSpeaker][band]);
            armw.qtePenalty += 0.015f;
            work.target = (work.target - 0.07f) - armw.qtePenalty / 10;
        }
        armw.qteSpeaker = (armw.qteSpeaker + 1) % 2;
        work.target = SnapZero(work.target);
        EVS_LoopCamera(shenGetRandomInt32(3) + 5000, ARMW_BASE, ARMW_BASE);
        Loop3DSoundEffect(0x847);
        work.stalemate = 250;
        work.phase = 10;
        work.settling = 45;
    }
    armw.qtePenalty += 0.003f;
    return 0;
}
std::int32_t ArmwFinishQte(ArmwWork &work) {
    if (work.step) {
        if (qteBattleUtil(BattleQteMode::GetResult) == 0)
            return 0;
        work.step = 0;
        if (qteBattleUtil(BattleQteMode::GetResult) != 1) {
            ArmwVoice(3, "013");
            work.target -= 0.15f;
            work.settling = 45;
            work.phase = 10;
            return 0;
        }
        ArmwVoice(3, "023");
    } else if (work.finishQte) {
        constexpr std::int16_t directions[]{64, 4, 1, 16};
        std::int16_t sequence[4]{};
        const std::int32_t count = work.finishQte >> 8, difficulty = work.finishQte & 255;
        for (std::int32_t i = 0; i < count - 1; ++i)
            sequence[i] = directions[shenGetRandomInt32(4)];
        sequence[count - 1] = 256;
        qteBattleUtil(BattleQteMode::SetWait, 30, 7 - difficulty, count * (8 - difficulty) + 5);
        qteBattleUtil(BattleQteMode::SetFirstCommands, sequence[0], sequence[1], sequence[2], sequence[3]);
        qteBattleUtil(BattleQteMode::SetLastCommands);
        qteBattleUtil(BattleQteMode::ResetStates);
        qteBattleUtil(BattleQteMode::Start);
        ++work.step;
        return 0;
    }
    ArmwRestoreHands(work);
    EVS_LoopCamera(1301, ARMW_BASE, ARMW_BASE);
    camCtrlSetRotationXYZ(0.1f, 0.18f, 0.25f);
    work.flags |= 1;
    work.phase = 13;
    return 0;
}
std::int32_t ArmwWinAnimation(ArmwWork &work) {
    const float previous = work.position;
    if (previous >= 3) {
        camCtrlSetRotationXYZ(0, 0, 0);
        Face(work.player, Normal, 10);
        Face(work.opponent, Normal, 10);
        work.phase = 14;
        return 0;
    }
    float position;
    if (previous >= 2) {
        position = previous + 0.006f;
        work.position = position;
        if (position < 3) {
            if (position > 2.8 && position <= 2.81)
                Loop3DSoundEffect(344);
        } else
            position = 3.75f;
        work.position = 1;
    } else {
        position = previous + 0.02f;
        work.position = position;
        if (previous < 1) {
            if (position >= 1) {
                EVS_LoopCamera(1302, ARMW_BASE, ARMW_BASE);
                position = 1.8f;
            } else if (position > 0.85f && position <= 0.875f)
                Loop3DSoundEffect(343);
            else if (position > 0.75f && position <= 0.775f)
                camCtrlSetRotationXYZ(0.05f, 0.09f, 0.25f);
        } else {
            if (position >= 2) {
                EVS_LoopCamera(1303, ARMW_BASE, ARMW_BASE);
                position = 2.8f;
                camCtrlSetRotationXYZ(0.05f, 0.09f, 0.25f);
            } else if (position > 1.95 && position <= 1.975f)
                Loop3DSoundEffect(343);
            else if (position > 1.85 && position <= 1.875)
                camCtrlSetRotationXYZ(0.05f, 0.09f, 0.25f);
            work.position -= 1;
        }
    }
    ArmwUpdatePose(work);
    work.position = position;
    return 0;
}
std::int32_t ArmwLoseAnimation(ArmwWork &work) {
    const float previous = work.position;
    if (previous < -2) {
        EVT_AT_SetFaceLooks(work.player, Normal, 10, 1, 30);
        EVT_AT_SetFaceLooks(work.opponent, Normal, 10, 1, 30);
        work.phase = 17;
        HLib::SetupTaskDelay(30);
        return 0;
    }
    float position;
    if (previous < -1) {
        position = previous - 0.025f;
        work.position = position + 2;
        if (position < -2 || SnapZero(position) == 0)
            position = -2.75f;
        else if (position < -1.86f && position >= -1.8875f) {
            Loop3DSoundEffect(344);
            camCtrlSetRotationXYZ(0.05f, 0.18f, 0.25f);
        }
    } else {
        position = previous - 0.04f;
        work.position = position;
        if (previous < 0) {
            work.position += 1;
            if (position < -1 || SnapZero(position) == 0) {
                Loop3DSoundEffect(343);
                position = -1.75f;
            }
        } else if (position < 0 || SnapZero(position) == 0) {
            Loop3DSoundEffect(343);
            position = -0.75f;
        }
    }
    work.position = SnapZero(work.position);
    ArmwUpdatePose(work);
    work.position = position;
    return 0;
}
} // namespace shenmue::s2::areas
