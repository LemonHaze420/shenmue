#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_anan.hpp"

double fabs(double value);

int AnanUpdateGame(AnanWork *work)
{
    AnanGame *game;
    EventDateTime now;
    int input, previous, result = 0, clear;
    float step;
    unsigned inputLocked;
    AnanVoiceCallback callback;
    callback = AnanVoiceUnlockInput;
    do {
        if (!work)
            break;
        game = &work->game;
        if (game->responseVoice && !FTBL.cvsHasValidConverseData(game->responseVoice))
            game->responseVoice = 0;
        AnanHistoryInput(game, 0);
        input = AnanHistoryInput(game, 1);
        previous = AnanHistoryInput(game, 2);
        clear = 0;
        if ((int)game->gameFlags >= 0) {
            step = AnanInputStep(game);
            switch (input) {
            case 0:
                FTBL.GetCurrentDateTime((DateTime *)&now);
                if (now.hour > 18 && now.minute > 29) {
                    result = 1;
                    AnanStopLift(0, 1);
                    QueueAnanCamera(ananPackage, 1912, ANAN_BASE, 0);
                    FTBL.HLib_SetCurrentTaskTo(AnanBeginTimeOver);
                } else if (game->idleFrames > 750) {
                    result = 2;
                    AnanStopLift(0, 1);
                    QueueAnanCamera(ananPackage, 1912, ANAN_BASE, 0);
                    game->idleFrames = 0;
                    FTBL.HLib_SetCurrentTaskTo(AnanBeginAngry);
                    clear = 1;
                } else if (game->idleFrames > 300 && !(inputLocked = game->gameFlags & 1)) {
                    result = 3;
                    AnanStopLift(0, 0);
                    FTBL.HLib_SetCurrentTaskTo(AnanRestartGame);
                    clear = 1;
                    FTBL.HLib_SetupTaskDelay(-1);
                    QueueAnanWait(0);
                    {
                        unsigned flags = game->gameFlags, mask;
                        do { mask = 0x80000001U; } while (0);
                        game->gameFlags = flags | mask;
                    }
                    QueueAnanVoice("0020B026", clear, 0, 80, (AnanVoiceCallback)inputLocked);
                    QueueAnanVoice("0020B027", clear, 100, 200, callback);
                }
                break;
            case 1:
                game->idleFrames = 0;
                game->gameFlags &= ~0x8000U;
                FTBL.HLib_SetCurrentTaskTo(AnanAskStraight);
                result = 4;
                AnanStopLift(1, 0);
                QueueAnanVoice("0020B012", 1, 20, 70, 0);
                clear = 1;
                break;
            case 2:
                game->idleFrames = 0;
                if (fabs(game->tilt + 1.0) < 1.1920929e-07) {
                    game->gameFlags |= 0x80000000U;
                    FTBL.HLib_SetCurrentTaskTo(AnanRestartGame);
                    result = 5;
                    FTBL.HLib_SetupTaskDelay(-1);
                    QueueAnanWait(0);
                    game->gameFlags |= 1;
                    AnanStopLift(0, 0);
                    if (game->responseVoice) {
                        FTBL.scenCvsCtrlFree(game->responseVoice);
                        game->responseVoice = 0;
                    }
                    QueueAnanVoice("0020B010", 1, 0, 130, callback);
                    game->tilt = -1.0f;
                    clear = 1;
                    game->endHoldFrames = 0;
                } else {
                    game->tilt = step + game->tilt;
                    result = 6;
                    if (fabs(game->tilt) < ANAN_EPSILON)
                        game->tilt = 0;
                    if (game->tilt <= -1.0f) {
                        game->endHoldFrames++;
                        if (previous != input || game->endHoldFrames > 30)
                            game->tilt = -1.0f;
                        else
                            game->tilt = -0.99900001f;
                    } else {
                        game->endHoldFrames = 0;
                        if (!game->responseVoice && FTBL.GetRandomNumberInRange(100.0f) > 90.0f)
                            game->responseVoice = FTBL.EV_PlayDialogue("0020B008");
                    }
                }
                break;
            case 3:
                game->idleFrames = 0;
                if (fabs(game->tilt - 1.0f) < ANAN_EPSILON) {
                    game->gameFlags |= 0x80000000U;
                    FTBL.HLib_SetCurrentTaskTo(AnanRestartGame);
                    result = 7;
                    FTBL.HLib_SetupTaskDelay(-1);
                    QueueAnanWait(0);
                    game->gameFlags |= 1;
                    AnanStopLift(0, 0);
                    if (game->responseVoice) {
                        FTBL.scenCvsCtrlFree(game->responseVoice);
                        game->responseVoice = 0;
                    }
                    QueueAnanVoice("0020B011", 1, 0, 160, callback);
                    game->tilt = 1.0f;
                    clear = 1;
                    game->endHoldFrames = 0;
                } else {
                    game->tilt = step + game->tilt;
                    result = 8;
                    if (fabs(game->tilt) < ANAN_EPSILON)
                        game->tilt = 0;
                    if (game->tilt >= 1.0f) {
                        game->endHoldFrames++;
                        if (previous != input || game->endHoldFrames > 30)
                            game->tilt = 1.0f;
                        else
                            game->tilt = 0.99900001f;
                    } else {
                        game->endHoldFrames = 0;
                        if (!game->responseVoice && FTBL.GetRandomNumberInRange(100.0f) > 90.0f)
                            game->responseVoice = FTBL.EV_PlayDialogue("0020B009");
                    }
                }
                break;
            }
        }
        else
            result = -1;
        if (clear)
            AnanClearInput(game);
    } while (0);
    return result;
}

void AnanStopLift(int confirming, int fixedCamera)
{
    MotionWork *motion;
    int camera, index;
    shen_name character;
    motion = FTBL.scenFindMotionwork(ANAN_MAN);
    if (!motion)
        return;
    FTBL.mtwkSetExtraFlag(motion, 9, 2);
    FTBL.mtwkSetSpeed(motion, 1.0f, 0);
    FTBL.mtwkRequestMotion(motion, 0x67c6, 0);
    FTBL.EV_mtwkSetEnabled(motion, 1);
    FTBL.SetMotionLayerLoopStartTime(motion, 170.0f, 0);
    FTBL.SetMotionRegionEvalTime(motion, 170.0f, 0);
    FTBL.SetMotionLayerEndFrame(motion, 200.0f, 0);
    if (!fixedCamera) {
        switch (FTBL.shenGetRandomInt32(!confirming ? 5 : 3)) {
        case 0:
            camera = 1907;
            break;
        case 1:
            camera = 1908;
            break;
        case 3:
            camera = 1910;
            break;
        case 4:
            camera = 1911;
            break;
        default:
            camera = 1909;
            break;
        }
        QueueAnanCamera(ananPackage, camera, ANAN_BASE, 0);
    }
    for (index = 0; (character = ananStoppedPlacement[index].character) != ANAN_INVALID; index++) {
        FTBL.EV_AT_SetPos(character, ANAN_ALL_AXES, &ananStoppedPlacement[index].position);
        FTBL.EV_AT_SetAngY_0(character, ANAN_ALL_AXES, &ananStoppedPlacement[index].rotation, 0);
    }
}

void AnanRetry(void *data)
{
    AnanWork *work = (AnanWork *)data;
    int index, finish = 1;
    while (work) {
        if (FTBL.filmCheckFadeTrackColour()) {
            finish = 0;
            break;
        }
        if (work->lifecycle & 2) {
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->lifecycle &= ~2U;
            finish = 0;
            break;
        } else {
            FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
            work->lifecycle |= 2;
            FTBL.HLib_SetCurrentTaskTo(AnanRestartGame);
            FTBL.HLib_SetupTaskDelay(-1);
            QueueAnanWait(0);
            AnanStopLift(0, 1);
            QueueAnanCamera(ananPackage, 1911, ANAN_BASE, 0);
            QueueAnanVoice("0020B023", 1, 32, 142, AnanVoiceUnlockInput);
            work->game.gameFlags |= 0x80000000U;
            work->targets[0] = ANAN_MAN;
            work->targets[1] = ANAN_RYO;
            for (index = 0; ananGamePlacement[index].character != ANAN_INVALID; index++) {
                FTBL.EV_AT_SetPos(ananGamePlacement[index].character, ANAN_ALL_AXES, &ananGamePlacement[index].position);
                FTBL.EV_AT_SetAngY_0(ananGamePlacement[index].character, ANAN_ALL_AXES, &ananGamePlacement[index].rotation, 0);
            }
        }
        finish = 0;
        break;
    }
    if (finish)
        FTBL.HLib_SetCurrentTaskTo(AnanFinish);
}

void AnanSetLiftPose(float tilt)
{
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(ANAN_MAN), 0);
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(ANAN_MAN), 71, 0, 0);
    AnanSetLiftFrame(tilt);
    FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(ANAN_MAN), 0.0f, 0);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(ANAN_MAN), 9, 0);
}

void AnanSetLiftFrame(float tilt)
{
    float frame;
    int index;
    if (fabs(tilt) < ANAN_EPSILON)
        tilt = 0;
    if (tilt > 1.0f)
        tilt = 1.0f;
    tilt += 1.0f;
    if (fabs(tilt) < ANAN_EPSILON)
        tilt = 0;
    tilt *= 0.5f;
    if (fabs(tilt) < ANAN_EPSILON)
        tilt = 0;
    frame = tilt * 41.0f;
    if (fabs(frame) < ANAN_EPSILON)
        frame = 0;
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(ANAN_MAN), 0x67cb, 0);
    FTBL.SetMotionLayerLoopStartTime(FTBL.scenFindMotionwork(ANAN_MAN), frame, 0);
    FTBL.SetMotionRegionEvalTime(FTBL.scenFindMotionwork(ANAN_MAN), frame, 0);
    FTBL.SetMotionLayerEndFrame(FTBL.scenFindMotionwork(ANAN_MAN), frame, 0);
    for (index = 0; ananGamePlacement[index].character != ANAN_INVALID; index++) {
        if (ananGamePlacement[index].character == ANAN_MAN) {
            FTBL.EV_AT_SetPos(ananGamePlacement[index].character, ANAN_ALL_AXES, &ananGamePlacement[index].position);
            FTBL.EV_AT_SetAngY_0(ananGamePlacement[index].character, ANAN_ALL_AXES, &ananGamePlacement[index].rotation, 0);
        }
    }
}

void AnanSetTargets(float tilt, int hand, int body)
{
    Vector3 origin, copy;
    Vector3i rotation;
    Vector3 handPoint, bodyPoint;
    do {
        if (!hand && !body)
            break;
        FTBL.GetCharaPosition(ANAN_BOARD, 0xffffffffU, &origin, ANAN_WORLD);
        FTBL.GetScenarioCharacterEulerAngles(ANAN_BOARD, &rotation, ANAN_WORLD);
        FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(ANAN_MAN), 55, 0);
        if (hand) {
            handPoint.x = -2.4f;
            handPoint.y = -0.65f;
            handPoint.z = 0.1f;
            copy = origin;
            FTBL.ApplyScenarioTranslationAndRotation(&copy, &rotation, &handPoint);
            FTBL.LatchMotionBoneTarget(FTBL.scenFindMotionwork(ANAN_MAN), 10, &handPoint);
        }
        if (body) {
            bodyPoint.x = -2.7f;
            bodyPoint.y = -0.4f;
            bodyPoint.z = 0.05f;
            copy = origin;
            FTBL.ApplyScenarioTranslationAndRotation(&copy, &rotation, &bodyPoint);
            FTBL.SetMotionBoneTargetStepLimit(FTBL.scenFindMotionwork(ANAN_MAN), 17, 1.0f);
            FTBL.LatchMotionBoneTarget(FTBL.scenFindMotionwork(ANAN_MAN), 17, &bodyPoint);
        }
    } while (0);
}

void AnanClearInput(AnanGame *game)
{
    int index;
    if (game) {
        game->inputPulse = 0;
        for (index = 0; index <= 5; index++)
            game->inputHistory[index] = 0;
    }
}

int AnanHistoryInput(AnanGame *game, int index)
{
    int value = 0, shift, bits, byte;
    if (game) {
        if (index > 23)
            index = 23;
        if (index >= 0) {
            shift = (index & 3) * 2;
            index >>= 2;
            byte = game->inputHistory[index];
            bits = byte >> shift;
            value = bits & 3;
        }
    }
    return value;
}

void AnanReadInput(AnanGame *game)
{
    unsigned int input;
    unsigned char carry, old;
    int direction;
    int index;
    while (game) {
        if (!(game->gameFlags & 0x80008000U)) {
            input = FTBL.GetCurrentControlInput(0);
            input &= 0xfbff;
            if (FTBL.GetBattleDataConfig(1, ANAN_BUTTON) && FTBL.GetBattleDataConfig(2, ANAN_BUTTON))
                input |= 0x400;
            else
                FTBL.GetBattleDataConfig(0, ANAN_BUTTON);
        } else
            input = 0;
        if (input >> 10 & 1)
            direction = 1;
        else if (input & 0x1000)
            direction = 2;
        else if (input & 0x2000)
            direction = 3;
        else
            direction = 0;
        carry = direction << 6;
        for (index = 5; index >= 0; index--) {
            old = game->inputHistory[index];
            game->inputHistory[index] = old >> 2 | carry;
            carry = (old & 3) << 6;
        }
        break;
    }
}

float AnanInputStep(AnanGame *game)
{
    int input, index;
    short state;
    float step = 0.0f;
    if (game) {
        input = AnanHistoryInput(game, 1);
        state = game->inputPulse;
        goto check_state;
process_state:
        {
            if (state) {
                for (index = 2; index <= 10; index++)
                    if (input != AnanHistoryInput(game, index))
                        break;
                if (input == 2) {
                    if (index <= 10)
                        state = 0;
                    else
                        state = 101;
                } else if (input == 3) {
                    if (index <= 10)
                        state = 0;
                    else
                        state = 102;
                } else if (state == -1)
                    state = 201;
                else {
                    state = 0;
                    goto done;
                }
            } else {
                if (input == 2)
                    state = 1;
                else if (input == 3)
                    state = 51;
                else
                    goto done;
                goto check_state;
            }
        }
check_state:
        if ((unsigned short)(state + 2) <= 2)
            goto process_state;
        switch (state) {
        case 1:
        case 6:
            step = -0.012857144f;
            break;
        case 2:
        case 5:
            step = -0.035714287f;
            break;
        case 3:
        case 4:
            step = -0.044285715f;
            break;
        case 51:
        case 56:
            step = 0.010714286f;
            break;
        case 52:
        case 55:
            step = 0.03357143f;
            break;
        case 53:
        case 54:
            step = 0.043571431f;
            break;
        case 101:
            step = -0.01f;
            break;
        case 102:
            step = 0.01f;
            break;
        case 201:
        case 202:
        case 205:
        case 206:
            step = 0.0f;
            break;
        case 203:
        case 204:
            step = 0.0071428572f;
            break;
        }
        if (state == 6 || state == 101)
            state = -1;
        else if (state == 56 || state == 102 || state == 206)
            state = -2;
        else
            state++;
done:
        game->inputPulse = state;
    }
    return step;
}

void AnanUpdatePose(AnanGame *game)
{
    if (game) {
        AnanSetLiftPose(game->tilt);
        AnanSetBoardTilt(game->tilt);
    }
}

void AnanAskStraight(void *data)
{
    AnanWork *work = (AnanWork *)data;
    short choices[4];
    if (work) {
        if ((int)work->game.gameFlags >= 0) {
            choices[0] = -1;
            choices[3] = -1;
            choices[1] = 82;
            choices[2] = 83;
            FTBL.SetupControlSpeakAndUI(choices, 800, 83, 2);
            FTBL.HLib_SetCurrentTaskTo(AnanWaitStraight);
            AnanGetHandPosition(work->game.tilt, &work->positions[0]);
        }
    } else
        FTBL.HLib_SetCurrentTaskTo(AnanFinish);
}

void AnanWaitStraight(void *data)
{
    AnanWork *work = (AnanWork *)data;
    HLibTaskCallback next;
    AnanVoiceCallback callback;
    callback = AnanVoiceUnlockInput;
    if (work) {
        if (!FTBL.GetShouldRenderIcons(2)) {
            QueueAnanWait(0);
            work->game.gameFlags |= 0x80000000U;
            if (FTBL.shenGetCtrlSpeakIconFlag() == 82) {
                work->targets[1] = ANAN_RYO;
                QueueAnanCamera(ananPackage, 1716, ANAN_BASE, 0);
                QueueAnanVoice("0020A012", 1, 10, 90, AnanVoiceUnlockInput);
                QueueAnanCamera(ananPackage, 1904, ANAN_BASE, 0);
                if (AnanIsStraight(&work->game))
                    next = AnanSuccess;
                else {
                    work->game.failures++;
                    next = AnanFailure;
                }
            } else {
                work->targets[0] = ANAN_INVALID;
                work->targets[1] = ANAN_INVALID;
                QueueAnanCamera(ananPackage, 1757, ANAN_BASE, 0);
                next = AnanRestartGame;
                QueueAnanVoice("0020B023", 1, 0, 110, callback);
                FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(ANAN_MAN), 0);
                FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(ANAN_MAN), 9, 2);
                AnanSetLiftFrame(work->game.tilt);
                AnanGetHandPosition(work->game.tilt, &work->positions[1]);
                work->targets[0] = 0;
                work->positionModes[0] = 0;
                work->targets[1] = ANAN_INVALID;
                work->positionModes[1] = 0;
            }
            FTBL.HLib_SetCurrentTaskTo(next);
            FTBL.HLib_SetupTaskDelay(-1);
        }
    } else
        FTBL.HLib_SetCurrentTaskTo(AnanFinish);
}

void QueueAnanWait(int updatePose)
{
    HLibTask *caller, *task;
    AnanWaitWork *wait;
    caller = FTBL.HLib_GetCurrentTask();
    if (caller) {
        task = FTBL.HLib_EnqueueTaskWithParameter(AnanWait, 4, 11, sizeof(AnanWaitWork), ANAN_TASK);
        if (task) {
            wait = (AnanWaitWork *)FTBL.HLib_GetTaskParameter(task);
            if (wait) {
                wait->caller = caller;
                wait->updatePose = updatePose;
            } else
                FTBL.HLib_DestroyTask(task);
        }
    }
}

void AnanWait(void *data)
{
    AnanWaitWork *wait = (AnanWaitWork *)data;
    AnanWork *work;
    int done = 1;
    if (wait && wait->caller) {
        work = (AnanWork *)FTBL.HLib_GetTaskParameter(wait->caller);
        if (work) {
            if (wait->updatePose)
                AnanUpdatePose(&work->game);
            done = !(work->game.gameFlags & 0x80000000U);
        }
    }
    if (done) {
        if (wait && wait->caller)
            FTBL.HLib_ResolveDeferredCallback(wait->caller);
        FTBL.HLib_DestroyCurrentEventTask();
    }
}

int AnanIsStraight(AnanGame *game)
{
    int straight = 0;
    if (game && game->tilt < 0.084285714f && game->tilt > -0.084285714f)
        straight = 1;
    return straight;
}

AnanPlacement ananStoppedPlacement[] = {{ANAN_MAN, {-1.35f, 1.27f, 0.63f}, {0, 0xfddd, 0}}, {ANAN_INVALID, {0.0f, 0.0f, 0.0f}, {0, 0, 0}}};
#else
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
    scenApplyTransRot(&origin, &rotation, &point);
    EV_GetCharaPosition(ANAN_BOARD, CHARACTER_TRANSLATION_ROOT, &origin, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(ANAN_BOARD, &rotation, EventTransformFlags::World);
    scenApplyTransRot(&origin, &rotation, &point);
    if (output)
        *output = point;
}
void AnanSetLiftPose(float tilt) {
    mtwkSetRequestedRootMode(scenFindMotionwork(ANAN_MAN), 0);
    mtwkSetRegionFlag(scenFindMotionwork(ANAN_MAN), 71, 0, MotionRegion::LowerBody);
    AnanSetLiftFrame(tilt);
    mtwkSetSpeed(scenFindMotionwork(ANAN_MAN), 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(ANAN_MAN), 9, 0);
}
void AnanSetLiftFrame(float tilt) {
    float frame = SnapAnan(tilt);
    frame = SnapAnan(std::fmin(1.0f, frame) + 1.0f);
    frame = SnapAnan(frame * .5f);
    frame = SnapAnan(frame * 41.0f);
    mtwkRequestMotion(scenFindMotionwork(ANAN_MAN), 0x67cb, MotionRegion::LowerBody);
    mtwkSetLayerLoopStartTime(scenFindMotionwork(ANAN_MAN), frame, MotionRegion::LowerBody);
    mtwkSetRegionEvalTime(scenFindMotionwork(ANAN_MAN), frame, MotionRegion::LowerBody);
    mtwkSetLayerEndFrame(scenFindMotionwork(ANAN_MAN), frame, MotionRegion::LowerBody);
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
        scenApplyTransRot(&copy, &rotation, &point);
        mtwkSetBoneTarget(scenFindMotionwork(ANAN_MAN), 10, &point);
    }
    Vector3 point{-2.7f, -.4f, .05f};
    Vector3 copy = origin;
    scenApplyTransRot(&copy, &rotation, &point);
    mtwkSetBoneTargetStepLimit(scenFindMotionwork(ANAN_MAN), 17, 1.0f);
    mtwkSetBoneTarget(scenFindMotionwork(ANAN_MAN), 17, &point);
}
void AnanStopLift(std::int32_t confirming, std::int32_t fixedCamera) {
    MotionWork *motion = scenFindMotionwork(ANAN_MAN);
    if (!motion)
        return;
    mtwkSetExtraFlag(motion, 9, 2);
    mtwkSetSpeed(motion, 1, MotionRegion::LowerBody);
    mtwkRequestMotion(motion, 0x67c6, MotionRegion::LowerBody);
    mtwkSetEnabled(motion, 1);
    mtwkSetLayerLoopStartTime(motion, 170, MotionRegion::LowerBody);
    mtwkSetRegionEvalTime(motion, 170, MotionRegion::LowerBody);
    mtwkSetLayerEndFrame(motion, 200, MotionRegion::LowerBody);
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
        input = static_cast<std::uint16_t>(GetCurrentControlInput(INPUT_HELD) & 0xfbffU);
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
    shenGetCurrentDateTime(now);
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
#endif
