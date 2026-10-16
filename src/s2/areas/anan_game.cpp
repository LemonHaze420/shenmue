#include "s2/areas/area_anan.hpp"
#include "s2/areas/anan_tables.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/input.hpp"
#include "s2/motion_work.hpp"
#include "s2/pause.hpp"
#include "s2/ui.hpp"
#include <bit>
#include <cmath>
#include <cstring>
namespace shenmue::s2::areas {
namespace {
float SnapAnan(float value) {
    return std::fabs(value) < 1.1920929e-7f ? 0.0f : value;
}
void ClearAnanBufferedInput(AnanWork &work) {
    work.inputPulse = 0;
    std::memset(work.inputHistory, 0, 6);
}
} // namespace
void AnanGetHandPosition(float tilt, Vector3 *output) {
    Vector3 point{-1.23f, -.3f, 0};
    Vector3 origin{0, 0, 0};
    Vector3i rotation{0, 0, static_cast<std::int32_t>(tilt * 1274.0f)};
    ApplyScenarioTranslationAndRotation(&origin, &rotation, &point);
    EV_GetCharaPosition(ANAN_BOARD, CHARACTER_TRANSLATION_ROOT, &origin, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(ANAN_BOARD, &rotation, EventTransformFlags::World);
    ApplyScenarioTranslationAndRotation(&origin, &rotation, &point);
    if (output)
        *output = point;
}
void AnanSetLiftPose(float tilt) {
    mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_MAN), 0);
    SetMotionRegionFlag(scenFindMotionwork(ANAN_MAN), 71, 0, MotionRegion::LowerBody);
    AnanSetLiftFrame(tilt);
    SetMotionSpeed(scenFindMotionwork(ANAN_MAN), 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(ANAN_MAN), 9, 0);
}
void AnanSetLiftFrame(float tilt) {
    float frame = SnapAnan(tilt);
    frame = SnapAnan(std::fmin(1.0f, frame) + 1.0f);
    frame = SnapAnan(frame * .5f);
    frame = SnapAnan(frame * 41.0f);
    mtwkRequestMotion(scenFindMotionwork(ANAN_MAN), 0x67cb, MotionRegion::LowerBody);
    SetMotionLayerLoopStartTime(scenFindMotionwork(ANAN_MAN), frame, MotionRegion::LowerBody);
    SetMotionRegionEvalTime(scenFindMotionwork(ANAN_MAN), frame, MotionRegion::LowerBody);
    SetMotionLayerEndFrame(scenFindMotionwork(ANAN_MAN), frame, MotionRegion::LowerBody);
    for (const AnanPlacement *row = ananGamePlacement; row->character.name != ANAN_INVALID.name; ++row) {
        if (row->character.name == ANAN_MAN.name) {
            EV_AT_SetPos(ANAN_MAN, EventTransformFlags::AllAxes, row->position);
            EV_AT_SetAngY(row->character, EventTransformFlags::AllAxes, row->rotation);
        }
    }
}
void AnanUpdatePose(AnanWork *work) {
    if (!work)
        return;
    AnanSetLiftPose(work->tilt);
    const Vector3i rotation{0, 0, static_cast<std::int32_t>(work->tilt * 1274.0f)};
    EV_AT_SetAngY(ANAN_BOARD, EventTransformFlags::AllAxes, rotation);
}
void AnanSetTargets(bool includeHand) {
    Vector3 origin;
    Vector3i rotation;
    EV_GetCharaPosition(ANAN_BOARD, CHARACTER_TRANSLATION_ROOT, &origin, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(ANAN_BOARD, &rotation, EventTransformFlags::World);
    mtwkSetExtraFlag(scenFindMotionwork(ANAN_MAN), 55, 0);
    if (includeHand) {
        Vector3 point{-2.4f, -.65f, .1f};
        Vector3 copy = origin;
        ApplyScenarioTranslationAndRotation(&copy, &rotation, &point);
        SetMotionBoneTarget(scenFindMotionwork(ANAN_MAN), 10, &point);
    }
    Vector3 point{-2.7f, -.4f, .05f};
    Vector3 copy = origin;
    ApplyScenarioTranslationAndRotation(&copy, &rotation, &point);
    SetMotionBoneTargetStepLimit(scenFindMotionwork(ANAN_MAN), 17, 1.0f);
    SetMotionBoneTarget(scenFindMotionwork(ANAN_MAN), 17, &point);
}
void AnanStopLift(std::int32_t confirming, std::int32_t fixedCamera) {
    MotionWork *motion = scenFindMotionwork(ANAN_MAN);
    if (!motion)
        return;
    mtwkSetExtraFlag(motion, 9, 2);
    SetMotionSpeed(motion, 1, MotionRegion::LowerBody);
    mtwkRequestMotion(motion, 0x67c6, MotionRegion::LowerBody);
    EV_mtwkSetEnabled(motion, 1);
    SetMotionLayerLoopStartTime(motion, 170, MotionRegion::LowerBody);
    SetMotionRegionEvalTime(motion, 170, MotionRegion::LowerBody);
    SetMotionLayerEndFrame(motion, 200, MotionRegion::LowerBody);
    if (!fixedCamera) {
        const std::int32_t choice = shenGetRandomInt32(confirming ? 3 : 5);
        const std::int32_t camera = choice == 0 ? 1907 : choice == 1 ? 1908 : choice == 3 ? 1910 : choice == 4 ? 1911 : 1909;
        AnanSelectCamera(camera);
    }
    ApplyAnanPlacements(ananStoppedPlacement);
}
std::uint32_t AnanHistoryInput(const AnanWork *work, std::int32_t index) {
    if (!work || index < 0)
        return 0;
    if (index >= 24)
        index = 23;
    return (work->inputHistory[index >> 2] >> (2 * (index & 3))) & 3;
}
void AnanReadInput(AnanWork *work) {
    if (!work)
        return;
    std::uint16_t input = 0;
    if (!(work->gameFlags & 0x80008000U)) {
        input = static_cast<std::uint16_t>(GetCurrentControlInput(0) & 0xfbffU);
        if (GetBattleDataConfig(1, ANAN_BUTTON) && GetBattleDataConfig(2, ANAN_BUTTON))
            input |= 0x400;
        else
            GetBattleDataConfig(0, ANAN_BUTTON);
    }
    const std::uint8_t value = input & 0x400 ? 1 : input & 0x1000 ? 2 : input & 0x2000 ? 3 : 0;
    for (std::int32_t i = 0; i < 5; ++i)
        work->inputHistory[i] = static_cast<std::uint8_t>((work->inputHistory[i] >> 2) | (work->inputHistory[i + 1] << 6));
    work->inputHistory[5] = static_cast<std::uint8_t>((work->inputHistory[5] >> 2) | (value << 6));
}
float AnanInputStep(AnanWork *work) {
    if (!work)
        return 0;
    const std::uint32_t input = AnanHistoryInput(work, 1);
    std::int16_t state = work->inputPulse;
    for (;;) {
        if (!state) {
            if (input == 2)
                state = 1;
            else if (input == 3)
                state = 51;
            else
                break;
        }
        if (static_cast<std::uint16_t>(state + 2) > 1U)
            break;
        std::int32_t i = 2;
        for (; i < 11; ++i)
            if (AnanHistoryInput(work, i) != input)
                break;
        if (input == 2)
            state = i >= 11 ? 101 : 0;
        else if (input == 3)
            state = i >= 11 ? 102 : 0;
        else if (state == -1)
            state = 201;
        else {
            state = 0;
            break;
        }
    }
    float step = 0;
    if (state) {
        switch (state) {
        case 1:
        case 6:
            step = -.012857144f;
            break;
        case 2:
        case 5:
            step = -.035714287f;
            break;
        case 3:
        case 4:
            step = -.044285715f;
            break;
        case 51:
        case 56:
            step = .010714286f;
            break;
        case 52:
        case 55:
            step = .03357143f;
            break;
        case 53:
        case 54:
            step = .043571431f;
            break;
        case 101:
            step = -.01f;
            break;
        case 102:
            step = .01f;
            break;
        case 203:
        case 204:
            step = .007142857f;
            break;
        default:
            break;
        }
        if (state == 6 || state == 101)
            state = -1;
        else if (state == 56 || state == 102 || state == 206)
            state = -2;
        else
            ++state;
    }
    work->inputPulse = state;
    return step;
}
void AnanUpdateDirectionVoice(AnanWork *work) {
    if (!work)
        return;
    AnanReadInput(work);
    if (work->directionVoice && !cvsHasValidConverseData(work->directionVoice))
        work->directionVoice = nullptr;
    const std::uint32_t current = AnanHistoryInput(work, 23), previous = AnanHistoryInput(work, 22);
    std::int32_t i = 22;
    for (; i >= 0; --i)
        if (AnanHistoryInput(work, i))
            break;
    const std::uint32_t previousNonzero = AnanHistoryInput(work, i);
    const char *voice = nullptr;
    if (current == previous) {
        if (!work->directionVoice) {
            if (current == 2)
                voice = "0020A006";
            else if (current == 3)
                voice = "0020A009";
        }
    } else {
        if (work->directionVoice && current >= 1 && current <= 3 && current != previousNonzero) {
            scenCvsCtrlFree(work->directionVoice);
            work->directionVoice = nullptr;
        }
        if (!work->directionVoice) {
            if (current == 1)
                voice = "0020A011";
            else if (current == 2)
                voice = "0020A005";
            else if (current == 3)
                voice = "0020A008";
        }
    }
    if (voice)
        work->directionVoice = EV_PlayDialogue(voice);
    if (!current) {
        ++work->idleFrames;
        return;
    }
    work->idleFrames = 0;
    if (current == 1)
        work->gameFlags = (work->gameFlags & 0xffff7ffeU) | 0x8000U;
    else
        work->gameFlags &= ~1U;
}
std::int32_t AnanUpdateGame(AnanWork *work) {
    if (!work)
        return 0;
    if (work->responseVoice && !cvsHasValidConverseData(work->responseVoice))
        work->responseVoice = nullptr;
    const std::uint32_t input = AnanHistoryInput(work, 1), previous = AnanHistoryInput(work, 2);
    if (work->gameFlags & 0x80000000U)
        return -1;
    const float step = AnanInputStep(work);
    if (input == 2 || input == 3) {
        work->idleFrames = 0;
        const float end = input == 2 ? -1.0f : 1.0f;
        if (std::fabs(work->tilt - end) >= 1.1920929e-7f) {
            work->tilt = SnapAnan(work->tilt + step);
            if ((input == 2 && work->tilt > -1) || (input == 3 && work->tilt < 1)) {
                work->endHoldFrames = 0;
                if (!work->responseVoice && GetRandomNumberInRange(100) > 90)
                    work->responseVoice = EV_PlayDialogue(input == 2 ? "0020B008" : "0020B009");
            } else {
                ++work->endHoldFrames;
                work->tilt = previous == input && work->endHoldFrames <= 30 ? std::bit_cast<float>(input == 2 ? 0xbf7fbe77U : 0x3f7fbe77U) : end;
            }
            return input == 2 ? 6 : 8;
        }
        work->gameFlags |= 0x80000000U;
        HLib::SetCurrentTaskTo(AnanRestartGame);
        HLib::SetupTaskDelay(-1);
        QueueAnanWait();
        work->gameFlags |= 1;
        AnanStopLift(0, 0);
        if (work->responseVoice) {
            scenCvsCtrlFree(work->responseVoice);
            work->responseVoice = nullptr;
        }
        QueueAnanVoice(input == 2 ? "0020B010" : "0020B011", 0, input == 2 ? 130 : 160, AnanVoiceUnlockInput);
        work->tilt = end;
        work->endHoldFrames = 0;
        ClearAnanBufferedInput(*work);
        return input == 2 ? 5 : 7;
    }
    if (input == 1) {
        work->gameFlags &= ~0x8000U;
        work->idleFrames = 0;
        HLib::SetCurrentTaskTo(AnanAskStraight);
        AnanStopLift(1, 0);
        QueueAnanVoice("0020B012", 20, 70, nullptr);
        ClearAnanBufferedInput(*work);
        return 4;
    }
    DateTime now;
    GetCurrentDateTime(now);
    if (now.hour >= 19 && now.minute >= 30) {
        AnanStopLift(0, 1);
        AnanSelectCamera(1912);
        HLib::SetCurrentTaskTo(AnanBeginTimeOver);
        return 1;
    }
    if (work->idleFrames > 750) {
        AnanStopLift(0, 1);
        AnanSelectCamera(1912);
        work->idleFrames = 0;
        HLib::SetCurrentTaskTo(AnanBeginAngry);
        ClearAnanBufferedInput(*work);
        return 2;
    }
    if (work->idleFrames > 300 && !(work->gameFlags & 1)) {
        AnanStopLift(0, 0);
        HLib::SetCurrentTaskTo(AnanRestartGame);
        HLib::SetupTaskDelay(-1);
        QueueAnanWait();
        work->gameFlags |= 0x80000001U;
        QueueAnanVoice("0020B026", 0, 80, nullptr);
        QueueAnanVoice("0020B027", 100, 200, AnanVoiceUnlockInput);
        ClearAnanBufferedInput(*work);
        return 3;
    }
    return 0;
}
bool AnanIsStraight(const AnanWork *work) {
    if (!work || work->tilt >= .084285714f || work->tilt <= -.084285714f)
        return false;
    IncrementSteamStat(14);
    return true;
}
void AnanInitializeGame(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    for (const AnanPlacement *row = ananGamePlacement; row->character.name != ANAN_INVALID.name; ++row) {
        EV_AT_SetPos(row->character, EventTransformFlags::AllAxes, row->position);
        EV_AT_SetAngY(row->character, EventTransformFlags::AllAxes, row->rotation);
        mtwkSetExtraFlag(scenFindMotionwork(row->character), 17, 0);
    }
    work->gameFlags = 0;
    work->idleFrames = 0;
    work->inputPulse = 0;
    std::memset(work->inputHistory, 0, 6);
    work->directionVoice = nullptr;
    work->responseVoice = nullptr;
    work->failures = 0;
    work->endHoldFrames = 0;
    AnanSelectCamera(1757);
    HLib::SetCurrentTaskTo(AnanRunGame);
    AnanRunGame(work);
}
void AnanRestartGame(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    ApplyAnanPlacements(ananGamePlacement);
    work->gameFlags &= ~0x8000U;
    work->inputPulse = 0;
    std::memset(work->inputHistory, 0, 6);
    work->directionVoice = nullptr;
    work->responseVoice = nullptr;
    work->endHoldFrames = 0;
    AnanSelectCamera(1757);
    AnanUpdatePose(work);
    HLib::SetCurrentTaskTo(AnanRunGame);
    AnanRunGame(work);
}
void AnanRunGame(void *data) {
    AnanWork *work = static_cast<AnanWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(AnanFinish);
        return;
    }
    AnanUpdateDirectionVoice(work);
    const std::int32_t state = AnanUpdateGame(work);
    bool normal = false;
    if (state != -1) {
        if (state >= 1 && state <= 4) {
            work->targets[0] = shen_name{};
            AnanGetHandPosition(work->tilt, &work->positions[0]);
            work->positionModes[0] = 0;
            work->positionModes[1] = 1;
            work->targets[1] = state == 2 ? shen_name{} : ANAN_RYO;
            AnanSetTargets(false);
        } else if (state == 5 || state == 7) {
            work->targets[0] = ANAN_MAN;
            work->targets[1] = ANAN_RYO;
            work->positionModes[0] = 1;
            work->positionModes[1] = 1;
            AnanSetTargets(false);
        } else {
            AnanGetHandPosition(work->tilt, &work->positions[0]);
            work->targets[0] = shen_name{};
            work->positionModes[0] = work->positionModes[1] = 0;
            normal = true;
            work->targets[1] = ANAN_INVALID;
            AnanSetTargets(true);
        }
    }
    if (static_cast<std::uint32_t>(state - 1) > 4U && state != 7)
        AnanUpdatePose(work);
    if (normal) {
        if (!(work->lifecycle & 0x8000U)) {
            FinishLoading();
            work->lifecycle |= 0x8000U;
        }
        CheckHelpIcon(12321, 0, CheckHelpIconOp::FrameOverlay);
    } else if (work->lifecycle & 0x8000U) {
        AnanResetHelp();
        work->lifecycle &= ~0x8000U;
    }
}
} // namespace shenmue::s2::areas
