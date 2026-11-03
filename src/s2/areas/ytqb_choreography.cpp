#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_ytqb.hpp"

#define YTQB_RESULT ((YtqbResultParameters *)data)

const EventQteData ytqbAuthoredQte[] = {
    {512, 24, 16, 0, 0, 0, YtqbBeginKick, 0, YtqbKickSuccess, YtqbKickFailure},
    {512, 24, 16, 0, 19, 0, 0, 0, YtqbKickCounterSuccess, YtqbKickCounterFailure},
    {512, 24, 16, 0, 2, 0, YtqbBeginRearKick, 0, YtqbRearKickSuccess, YtqbRearKickFailure},
    {512, 24, 16, 0, 0, 0, YtqbBeginWristTwist, 0, YtqbWristTwistSuccess, YtqbKickFailure},
    {512, 24, 16, 0, 19, 0, 0, 0, YtqbKickCounterSuccess, YtqbKickCounterFailure},
    {256, 24, 16, 0, 2, 0, YtqbBeginBackfist, 0, YtqbBackfistSuccess, YtqbBackfistFailure},
    {512, 24, 16, 0, 9, 0, 0, 0, YtqbKneeCounterSuccess, YtqbKneeCounterFailure},
    {512, 24, 16, 0, 5, 0, YtqbBeginPin, 0, YtqbPinSuccess, YtqbPinFailure},
    {256, 24, 16, 0, 9, 0, 0, 0, YtqbPinKickSuccess, YtqbPinKickFailure},
    {256, 24, 16, 0, 0, 18, 0, YtqbBeginGrabEvadeA, YtqbGrabEvadeASuccess, YtqbGrabEvadeAFailure},
    {64, 24, 16, 0, 15, 0, 0, 0, YtqbThrowASuccess, YtqbThrowAFailure},
    {256, 24, 16, 0, 0, 13, 0, YtqbBeginGrabEvadeB, YtqbGrabEvadeBSuccess, YtqbGrabEvadeBFailure},
    {64, 24, 16, 0, 15, 0, 0, 0, YtqbThrowBSuccess, YtqbThrowBFailure},
    {1024, 24, 16, 0, 0, 18, 0, YtqbBeginKickDodge, YtqbKickDodgeSuccess, YtqbKickDodgeFailure},
    {512, 24, 16, 0, 14, 0, 0, 0, YtqbKickDodgeCounterSuccess, YtqbKickDodgeCounterFailure},
    {1024, 24, 16, 0, 2, 0, YtqbBeginBackfist, 0, YtqbFlyingKickSuccess, YtqbBackfistFailure},
    {512, 24, 16, 0, 9, 0, 0, 0, YtqbKneeCounterSuccess, YtqbKneeCounterFailure},
    {1024, 24, 16, 0, 2, 0, YtqbBeginBackfist, 0, YtqbFinishingBackfistSuccess, YtqbBackfistFailure},
    {512, 24, 16, 0, 9, 0, 0, 0, YtqbKneeCounterSuccess, YtqbKneeCounterFailure},
    {256, 24, 16, 0, 0, 0, YtqbBeginRearKickAssist, 0, YtqbRearKickAssistSuccess, YtqbRearKickAssistFailure},
    {512, 24, 16, 0, 0, 0, YtqbBeginBackfistAssist, 0, YtqbBackfistAssistSuccess, YtqbBackfistAssistFailure}};
const EventQteData ytqbPromptQte[] = {
    {512, 24, 16, 0, 0, 0, YtqbBeginKick, 0, 0, 0},
    {512, 24, 16, 0, 19, 0, 0, 0, 0, 0},
    {512, 24, 16, 0, 2, 0, YtqbBeginRearKick, 0, 0, 0},
    {512, 24, 16, 0, 0, 0, YtqbBeginWristTwist, 0, 0, 0},
    {512, 24, 16, 0, 19, 0, 0, 0, 0, 0},
    {256, 24, 16, 0, 2, 0, YtqbBeginBackfist, 0, 0, 0},
    {512, 24, 16, 0, 9, 0, 0, 0, 0, 0},
    {512, 24, 16, 0, 5, 0, YtqbBeginPin, 0, 0, 0},
    {256, 24, 16, 0, 9, 0, 0, 0, 0, 0},
    {256, 24, 16, 0, 0, 18, 0, YtqbBeginGrabEvadeA, 0, 0},
    {64, 24, 16, 0, 15, 0, 0, 0, 0, 0},
    {256, 24, 16, 0, 0, 13, 0, YtqbBeginGrabEvadeB, 0, 0},
    {64, 24, 16, 0, 15, 0, 0, 0, 0, 0},
    {1024, 24, 16, 0, 0, 18, 0, YtqbBeginKickDodge, 0, 0},
    {512, 24, 16, 0, 14, 0, 0, 0, 0, 0},
    {1024, 24, 16, 0, 2, 0, YtqbBeginBackfist, 0, 0, 0},
    {512, 24, 16, 0, 9, 0, 0, 0, 0, 0},
    {1024, 24, 16, 0, 2, 0, YtqbBeginBackfist, 0, 0, 0},
    {512, 24, 16, 0, 9, 0, 0, 0, 0, 0},
    {256, 24, 16, 0, 0, 0, YtqbBeginRearKickAssist, 0, 0, 0},
    {512, 24, 16, 0, 0, 0, YtqbBeginBackfistAssist, 0, 0, 0}};

void YtqbBeginKick(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED) {
        EVT_AT_SetPos(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.11149f, 0.0f, -2.52247f, delay);
        EVT_AT_SetAngY(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 180.0f, delay);
    } else {
        EVT_AT_SetPos(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.11149f, 0.0f, -2.52247f, delay);
        EVT_AT_SetAngY(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 180.0f, delay);
    }
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5588, 0.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->player, 1, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->firstOpponent, 1, 15, 1.0f, delay);
}

void YtqbKickSuccess(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.191f, -1.4969f, 180.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.191f, -1.4969f, 180.0f, delay);
    EVT_AT_SetSound(649, delay + 17);
    EVT_AT_SetMot(YTQB_RESULT->player, 5658, 8.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_DispBlur(YTQB_RESULT->player, 3, 12, 1, 0, 4, delay + 11);
    EVT_AT_SetVoice("S2BVA002", delay);
    EVT_AT_SetSound(653, delay + 13);
    EVT_AT_SetSound(642, delay + 58);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5586, 0.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->firstOpponent, 6, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB017", delay + 25);
}

void YtqbKickFailure(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.00969f, -1.16244f, 180.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.00969f, -1.16244f, 180.0f, delay);
    EVT_AT_SetMot(YTQB_RESULT->player, 5660, 0.0f, 1.0f, YTQB_RESULT->motionFlags, delay + 5);
    EVT_AT_SetFaceLooks(YTQB_RESULT->player, 6, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA008", delay + 8);
    EVT_AT_SetSound(652, delay + 8);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5587, 0.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetVoice("S2BVB009", delay);
}

void YtqbKickCounterSuccess(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.02815f, -0.87369f, 180.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.02815f, -0.87369f, 180.0f, delay);
    EVT_AT_SetSound(651, delay + 13);
    EVT_AT_SetMot(YTQB_RESULT->player, 5639, 0.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_DispBlur(YTQB_RESULT->player, 8, 12, 1, 0, 15, delay);
    EVT_AT_SetVoice("S2BVA001", delay);
    EVT_AT_SetSound(642, delay + 34);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5571, 31.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->firstOpponent, 6, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB023", delay + 16);
}

void YtqbKickCounterFailure(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.16294f, -0.71369f, 180.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.16294f, -0.71369f, 180.0f, delay);
    EVT_AT_SetMot(YTQB_RESULT->player, 5641, 0.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->player, 6, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA012", delay + 7);
    EVT_AT_SetSound(650, delay + 3);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5573, 34.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetVoice("S2BVB013", delay);
}

void YtqbBeginRearKick(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.15889f, 1.22727f, 0.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.15889f, 1.22727f, 0.0f, delay);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5632, 0.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->player, 1, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->firstOpponent, 1, 15, 1.0f, delay);
}

void YtqbRearKickSuccess(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.13082f, 2.17583f, 0.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.13082f, 2.17583f, 0.0f, delay);
    EVT_AT_SetSound(649, delay + 11);
    EVT_AT_SetMot(YTQB_RESULT->player, 5697, 15.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_DispBlur(YTQB_RESULT->player, 3, 12, 1, 0, 4, delay + 10);
    EVT_AT_SetVoice("S2BVA003", delay);
    EVT_AT_SetSound(642, delay + 34);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5631, 35.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->firstOpponent, 6, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB028", delay + 16);
}

void YtqbRearKickFailure(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.12889f, 1.03727f, 0.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.12889f, 1.03727f, 0.0f, delay);
    EVT_AT_SetMot(YTQB_RESULT->player, 5698, 5.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->player, 6, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA013", delay + 20);
    EVT_AT_SetSound(653, delay + 10);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5632, 32.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetVoice("S2BVB001", delay);
}

void YtqbBeginWristTwist(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED) {
        EVT_AT_SetPos(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.61177f, 0.0f, -3.44645f, delay);
        EVT_AT_SetAngY(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 180.0f, delay);
    } else {
        EVT_AT_SetPos(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.61177f, 0.0f, -3.44645f, delay);
        EVT_AT_SetAngY(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 180.0f, delay);
    }
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5588, 0.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->player, 1, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->firstOpponent, 1, 15, 1.0f, delay);
}

void YtqbWristTwistSuccess(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.17725f, -2.13321f, 180.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.17725f, -2.13321f, 180.0f, delay);
    EVT_AT_SetMot(YTQB_RESULT->player, 5661, 10.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_DispBlur(YTQB_RESULT->player, 8, 12, 1, 0, 15, delay);
    EVT_AT_SetVoice("S2BVA002", delay);
    EVT_AT_SetSound(653, delay + 6);
    EVT_AT_SetSound(642, delay + 56);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5593, 10.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->firstOpponent, 6, 15, 1.0f, delay + 10);
}

void YtqbBeginBackfist(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED) {
        EVT_AT_SetPos(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -1.44509f, 0.0f, -1.41609f, delay);
        EVT_AT_SetAngY(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -120.0f, delay);
    } else {
        EVT_AT_SetPos(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 1.44509f, 0.0f, -1.41609f, delay);
        EVT_AT_SetAngY(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 120.0f, delay);
    }
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5627, 0.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->player, 1, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->firstOpponent, 1, 15, 1.0f, delay);
}

void YtqbBackfistSuccess(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -1.44509f, -2.41609f, -120.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 1.44509f, -2.41609f, 120.0f, delay);
    EVT_AT_SetSound(1719, delay + 16);
    EVT_AT_SetSound(651, delay + 31);
    EVT_AT_SetMot(YTQB_RESULT->player, 5691, 6.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_DispBlur(YTQB_RESULT->player, 8, 12, 1, 0, 6, delay + 15);
    EVT_AT_SetVoice("S2BVA005", delay);
    EVT_AT_SetSound(642, delay + 47);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5627, 40.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->firstOpponent, 6, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB022", delay + 37);
}

void YtqbBackfistFailure(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -1.08961f, -1.73494f, -120.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 1.08961f, -1.73494f, 120.0f, delay);
    EVT_AT_SetMot(YTQB_RESULT->player, 5693, 0.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->player, 6, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA010", delay + 25);
    EVT_AT_SetSound(652, delay + 15);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5628, 40.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetVoice("S2BVB005", delay);
}

void YtqbKneeCounterSuccess(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.5771f, -1.22592f, -120.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.5771f, -1.22592f, 120.0f, delay);
    EVT_AT_SetSound(649, delay + 25);
    EVT_AT_SetMot(YTQB_RESULT->player, 5694, 5.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_DispBlur(YTQB_RESULT->player, 3, 12, 1, 0, 6, delay + 10);
    EVT_AT_SetVoice("S2BVA003", delay);
    EVT_AT_SetSound(642, delay + 70);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5629, 0.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->firstOpponent, 6, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB030", delay + 30);
}

void YtqbKneeCounterFailure(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.54642f, -0.94592f, -120.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.54642f, -0.94592f, 120.0f, delay);
    EVT_AT_SetMot(YTQB_RESULT->player, 5695, 0.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->player, 6, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA011", delay + 23);
    EVT_AT_SetSound(652, delay + 23);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5630, 0.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetVoice("S2BVB007", delay);
}

void YtqbBeginPin(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED) {
        EVT_AT_SetPos(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 1.64264f, 0.0f, -0.69735f, delay);
        EVT_AT_SetAngY(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -246.0f, delay);
        EVT_AT_SetPos(YTQB_RESULT->secondOpponent, YTQB_RESULT->player, -2.98405f, 0.0f, -0.31086f, delay);
        EVT_AT_SetAngY(YTQB_RESULT->secondOpponent, YTQB_RESULT->player, -90.0f, delay);
    } else {
        EVT_AT_SetPos(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -1.64264f, 0.0f, -0.69735f, delay);
        EVT_AT_SetAngY(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 246.0f, delay);
        EVT_AT_SetPos(YTQB_RESULT->secondOpponent, YTQB_RESULT->player, 2.98405f, 0.0f, -0.31086f, delay);
        EVT_AT_SetAngY(YTQB_RESULT->secondOpponent, YTQB_RESULT->player, 90.0f, delay);
    }
    EVT_AT_SetMot(YTQB_RESULT->player, 5666, 0.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->player, 1, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->firstOpponent, 1, 15, 1.0f, delay);
}

void YtqbPinSuccess(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 2.21219f, -0.37863f, -270.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -2.21219f, -0.37863f, 270.0f, delay);
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->secondOpponent, YTQB_RESULT->player, -2.98405f, -0.31086f, -90.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->secondOpponent, YTQB_RESULT->player, 2.98405f, -0.31086f, 90.0f, delay);
    EVT_AT_SetMot(YTQB_RESULT->player, 5662, 10.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_SetVoice("S2BVA002", delay);
    EVT_AT_SetSound(653, delay + 5);
    EVT_AT_SetSound(1538, delay + 10);
    EVT_AT_SetSound(1767, delay + 30);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5595, 26.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->firstOpponent, 6, 15, 1.0f, delay + 10);
    EVT_AT_SetMot(YTQB_RESULT->secondOpponent, 5597, 0.0f, 1.0f, YTQB_RESULT->motionFlags, delay + 7);
    EVT_AT_SetVoice("S2BVB021", delay + 20);
}

void YtqbPinFailure(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED) {
        EVT_AT_SetPos(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 1.64264f, 0.0f, -0.69735f, delay);
        EVT_AT_SetAngY(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -246.0f, delay);
        EVT_AT_SetPos(YTQB_RESULT->secondOpponent, YTQB_RESULT->player, -2.98405f, 0.0f, -0.31086f, delay);
        EVT_AT_SetAngY(YTQB_RESULT->secondOpponent, YTQB_RESULT->player, -90.0f, delay);
    } else {
        EVT_AT_SetPos(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -1.64264f, 0.0f, -0.69735f, delay);
        EVT_AT_SetAngY(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 246.0f, delay);
        EVT_AT_SetPos(YTQB_RESULT->secondOpponent, YTQB_RESULT->player, 2.98405f, 0.0f, -0.31086f, delay);
        EVT_AT_SetAngY(YTQB_RESULT->secondOpponent, YTQB_RESULT->player, 90.0f, delay);
    }
    EVT_AT_SetMot(YTQB_RESULT->player, 5666, 35.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->player, 6, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA009", delay + 26);
    EVT_AT_SetSound(652, delay + 20);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5600, 6.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetVoice("S2BVB009", delay);
}

void YtqbPinKickSuccess(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.29901f, -0.25656f, -270.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.29901f, -0.25656f, 270.0f, delay);
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->secondOpponent, YTQB_RESULT->player, -2.98405f, -0.31086f, -90.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->secondOpponent, YTQB_RESULT->player, 2.98405f, -0.31086f, 90.0f, delay);
    EVT_AT_SetSound(649, delay + 14);
    EVT_AT_SetSound(651, delay + 36);
    EVT_AT_SetMot(YTQB_RESULT->player, 5663, 0.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_DispBlur(YTQB_RESULT->player, 3, 12, 1, 0, 6, delay + 10);
    EVT_AT_SetVoice("S2BVA003", delay);
    EVT_AT_SetSound(642, delay + 76);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5596, 0.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->firstOpponent, 6, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB023", delay + 20);
    EVT_AT_SetSound(642, delay + 33);
    EVT_AT_SetMot(YTQB_RESULT->secondOpponent, 5597, 25.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetVoice("S2BVB020", delay + 40);
}

void YtqbPinKickFailure(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.50663f, -0.02464f, -270.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.50663f, -0.02464f, 270.0f, delay);
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->secondOpponent, YTQB_RESULT->player, -1.85807f, 0.77458f, 0.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->secondOpponent, YTQB_RESULT->player, 1.85807f, 0.77458f, 0.0f, delay);
    EVT_AT_SetMot(YTQB_RESULT->player, 5665, 0.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->player, 6, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA007", delay + 28);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5598, 0.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetSound(650, delay + 21);
    EVT_AT_SetMot(YTQB_RESULT->secondOpponent, 5599, 22.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetVoice("S2BVB013", delay);
}

void YtqbBeginGrabEvadeA(void *data, int delay)
{
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5618, 0.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->player, 1, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->firstOpponent, 1, 15, 1.0f, delay);
}

void YtqbGrabEvadeASuccess(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.00988f, -2.29381f, 180.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.00988f, -2.29381f, 180.0f, delay);
    EVT_AT_SetSound(651, delay + 20);
    EVT_AT_SetSound(1719, delay + 8);
    EVT_AT_SetMot(YTQB_RESULT->player, 5683, 10.0f, 1.0f, YTQB_RESULT->motionFlags ^ 0x20000000U, delay);
    EVT_AT_DispBlur(YTQB_RESULT->player, 1, 8, 1, 0, 6, delay + 17);
    EVT_AT_SetVoice("S2BVA002", delay);
    EVT_AT_SetSound(642, delay + 42);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5618, 12.0f, 1.0f, YTQB_RESULT->motionFlags ^ 0x20000000U | 0x40000000U, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->firstOpponent, 6, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB026", delay + 30);
}

void YtqbGrabEvadeAFailure(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.17864f, -2.26075f, 180.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.17864f, -2.26075f, 180.0f, delay);
    EVT_AT_SetMot(YTQB_RESULT->player, 5685, 0.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_SetMot(YTQB_RESULT->player, 5689, 0.0f, 1.0f, YTQB_RESULT->motionFlags, delay + 31);
    EVT_AT_SetFaceLooks(YTQB_RESULT->player, 6, 15, 1.0f, delay + 15);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5620, 0.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5624, 0.0f, 1.0f, YTQB_RESULT->motionFlags, delay + 31);
    EVT_AT_SetVoice("S2BVB004", delay);
}

void YtqbBeginGrabEvadeB(void *data, int delay)
{
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5619, 0.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->player, 1, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->firstOpponent, 1, 15, 1.0f, delay);
}

void YtqbGrabEvadeBSuccess(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.00079f, -2.30879f, 180.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.00079f, -2.30879f, 180.0f, delay);
    EVT_AT_SetSound(649, delay + 27);
    EVT_AT_SetSound(1719, delay + 12);
    EVT_AT_SetMot(YTQB_RESULT->player, 5684, 0.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_DispBlur(YTQB_RESULT->player, 1, 8, 1, 0, 4, delay + 25);
    EVT_AT_SetVoice("S2BVA002", delay);
    EVT_AT_SetSound(642, delay + 41);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5619, 16.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->firstOpponent, 6, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB017", delay + 30);
}

void YtqbGrabEvadeBFailure(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.17864f, -2.16075f, 180.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.17864f, -2.16075f, 180.0f, delay);
    EVT_AT_SetMot(YTQB_RESULT->player, 5685, 0.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_SetMot(YTQB_RESULT->player, 5689, 0.0f, 1.0f, YTQB_RESULT->motionFlags, delay + 31);
    EVT_AT_SetFaceLooks(YTQB_RESULT->player, 6, 15, 1.0f, delay + 15);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5620, 0.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5624, 0.0f, 1.0f, YTQB_RESULT->motionFlags, delay + 31);
    EVT_AT_SetVoice("S2BVB006", delay);
}

void YtqbThrowASuccess(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.0557f, -0.77548f, 180.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.0557f, -0.77548f, 180.0f, delay);
    EVT_AT_SetMot(YTQB_RESULT->player, 5686, 0.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_DispBlur(YTQB_RESULT->player, 1, 8, 1, 0, 15, delay);
    EVT_AT_SetVoice("S2BVA003", delay);
    EVT_AT_SetSound(642, delay + 60);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5621, 0.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->firstOpponent, 6, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB023", delay + 50);
}

void YtqbThrowAFailure(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.25895f, -0.87206f, 180.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.25895f, -0.87206f, 180.0f, delay);
    EVT_AT_SetMot(YTQB_RESULT->player, 5688, 0.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->player, 6, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA012", delay + 15);
    EVT_AT_SetSound(650, delay + 12);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5623, 0.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetVoice("S2BVB005", delay);
}

void YtqbThrowBSuccess(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.00466f, -0.69023f, 180.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.00466f, -0.69023f, 180.0f, delay);
    EVT_AT_SetMot(YTQB_RESULT->player, 5687, 0.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_DispBlur(YTQB_RESULT->player, 1, 8, 1, 0, 15, delay);
    EVT_AT_SetVoice("S2BVA002", delay);
    EVT_AT_SetSound(642, delay + 71);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5622, 0.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->firstOpponent, 6, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB018", delay + 60);
}

void YtqbThrowBFailure(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.25895f, -0.87206f, 180.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.25895f, -0.87206f, 180.0f, delay);
    EVT_AT_SetMot(YTQB_RESULT->player, 5688, 0.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->player, 6, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA009", delay + 15);
    EVT_AT_SetSound(650, delay + 12);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5623, 0.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetVoice("S2BVB003", delay);
}

void YtqbBeginKickDodge(void *data, int delay)
{
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5589, 0.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->player, 1, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->firstOpponent, 1, 15, 1.0f, delay);
}

void YtqbKickDodgeSuccess(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.00494f, -2.75157f, 180.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.00494f, -2.75157f, 180.0f, delay);
    EVT_AT_SetSound(651, delay + 22);
    EVT_AT_SetMot(YTQB_RESULT->player, 5634, 10.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_DispBlur(YTQB_RESULT->player, 1, 8, 1, 0, 4, delay + 11);
    EVT_AT_SetVoice("S2BVA004", delay);
    EVT_AT_SetSound(642, delay + 38);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5589, 10.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->firstOpponent, 6, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB023", delay + 32);
}

void YtqbKickDodgeFailure(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.00583f, -2.27803f, 180.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.00583f, -2.27803f, 180.0f, delay);
    EVT_AT_SetMot(YTQB_RESULT->player, 5636, 14.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_SetMot(YTQB_RESULT->player, 5642, 0.0f, 1.0f, YTQB_RESULT->motionFlags, delay + 2);
    EVT_AT_SetFaceLooks(YTQB_RESULT->player, 6, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA008", delay + 12);
    EVT_AT_SetSound(650, delay + 2);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5591, 14.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5574, 0.0f, 1.0f, YTQB_RESULT->motionFlags, delay + 2);
    EVT_AT_SetVoice("S2BVB005", delay);
}

void YtqbKickDodgeCounterSuccess(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.15021f, -0.90296f, 180.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.15021f, -0.90296f, 180.0f, delay);
    EVT_AT_SetSound(649, delay + 18);
    EVT_AT_SetMot(YTQB_RESULT->player, 5640, 30.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_DispBlur(YTQB_RESULT->player, 1, 8, 1, 0, 4, delay + 11);
    EVT_AT_SetVoice("S2BVA005", delay);
    EVT_AT_SetSound(642, delay + 36);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5572, 30.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->firstOpponent, 6, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB018", delay + 28);
}

void YtqbKickDodgeCounterFailure(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.15021f, -0.88232f, 180.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.15021f, -0.88232f, 180.0f, delay);
    EVT_AT_SetMot(YTQB_RESULT->player, 5642, 43.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->player, 6, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA010", delay + 28);
    EVT_AT_SetSound(652, delay + 18);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5574, 40.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetVoice("S2BVB002", delay);
}

void YtqbFlyingKickSuccess(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -2.2537f, -0.79279f, 270.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 2.2537f, -0.79279f, -270.0f, delay);
    EVT_AT_SetSound(649, delay + 22);
    EVT_AT_SetMot(YTQB_RESULT->player, 5647, 0.0f, 1.0f, YTQB_RESULT->motionFlags ^ 0x20000000U, delay);
    EVT_AT_DispBlur(YTQB_RESULT->player, 1, 6, 1, 0, 50, delay);
    EVT_AT_SetVoice("S2BVA004", delay);
    EVT_AT_SetSound(642, delay + 47);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5627, 47.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->firstOpponent, 6, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB022", delay + 30);
}

void YtqbFinishingBackfistSuccess(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -2.36129f, -1.06471f, -90.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 2.36129f, -1.06471f, 90.0f, delay);
    EVT_AT_SetSound(651, delay + 18);
    EVT_AT_SetMot(YTQB_RESULT->player, 5648, 0.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_DispBlur(YTQB_RESULT->player, 2, 12, 1, 0, 10, delay + 10);
    EVT_AT_SetVoice("S2BVA004", delay);
    EVT_AT_SetSound(642, delay + 40);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5627, 52.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->firstOpponent, 6, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB023", delay + 20);
}

void YtqbBeginRearKickAssist(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.00978f, 2.62084f, 0.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.00978f, 2.62084f, 0.0f, delay);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5631, 0.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->player, 1, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->firstOpponent, 1, 15, 1.0f, delay);
}

void YtqbRearKickAssistSuccess(void *data, int delay)
{
    EVT_AT_SetAngY(YTQB_RESULT->player, YTQB_RESULT->player, 200.0f, delay);
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.00978f, -2.62084f, 180.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.00978f, -2.62084f, 180.0f, delay);
    EVT_AT_SetMot(YTQB_RESULT->player, 5699, 138.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_SetVoice("S2BVA001", delay);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5631, 29.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetSound(649, delay + 15);
    EVT_AT_SetSound(642, delay + 40);
    EVT_AT_SetFaceLooks(YTQB_RESULT->firstOpponent, 6, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB017", delay + 20);
}

void YtqbRearKickAssistFailure(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.12889f, 1.03727f, 0.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.12889f, 1.03727f, 0.0f, delay);
    EVT_AT_SetMot(YTQB_RESULT->player, 5698, 5.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->player, 6, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA013", delay + 20);
    EVT_AT_SetSound(653, delay + 10);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5632, 32.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetVoice("S2BVB005", delay);
}

void YtqbBeginBackfistAssist(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED) {
        EVT_AT_SetAngY(YTQB_RESULT->player, YTQB_RESULT->player, -270.0f, delay);
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -1.14463f, 2.09687f, 0.0f, delay);
        EVT_AT_SetAngY(YTQB_RESULT->player, YTQB_RESULT->player, 270.0f, delay);
    } else {
        EVT_AT_SetAngY(YTQB_RESULT->player, YTQB_RESULT->player, 270.0f, delay);
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 1.14463f, 2.09687f, 0.0f, delay);
        EVT_AT_SetAngY(YTQB_RESULT->player, YTQB_RESULT->player, -270.0f, delay);
    }
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5627, 32.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->player, 1, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->firstOpponent, 1, 15, 1.0f, delay);
}

void YtqbBackfistAssistSuccess(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED) {
        EVT_AT_SetAngY(YTQB_RESULT->player, YTQB_RESULT->player, -270.0f, delay);
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -1.14463f, 2.09687f, 0.0f, delay);
    } else {
        EVT_AT_SetAngY(YTQB_RESULT->player, YTQB_RESULT->player, 270.0f, delay);
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 1.14463f, 2.09687f, 0.0f, delay);
    }
    EVT_AT_SetMot(YTQB_RESULT->player, 5700, 60.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_SetVoice("S2BVA003", delay);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5627, 49.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetSound(652, delay + 18);
    EVT_AT_SetSound(642, delay + 40);
    EVT_AT_SetFaceLooks(YTQB_RESULT->firstOpponent, 6, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB023", delay + 20);
}

void YtqbBackfistAssistFailure(void *data, int delay)
{
    if (YTQB_RESULT->motionFlags & YTQB_MIRRORED)
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, -0.12889f, 1.19727f, 0.0f, delay);
    else
        EVT_AT_SetPosAng(YTQB_RESULT->firstOpponent, YTQB_RESULT->player, 0.12889f, 1.19727f, 0.0f, delay);
    EVT_AT_SetMot(YTQB_RESULT->player, 5698, 5.0f, 1.0f, YTQB_RESULT->motionFlags, delay);
    EVT_AT_SetFaceLooks(YTQB_RESULT->player, 6, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA013", delay + 20);
    EVT_AT_SetSound(653, delay + 10);
    EVT_AT_SetMot(YTQB_RESULT->firstOpponent, 5632, 32.0f, 1.0f, YTQB_RESULT->motionFlags | 0x40000000U, delay);
    EVT_AT_SetVoice("S2BVB004", delay);
}
#else
#include "s2/areas/area_ytqb.hpp"
#include "s2/event_gameplay_queued.hpp"
#include <bit>
namespace shenmue::s2::areas {
void YtqbBeginKick(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? -0.11149f : 0.11149f;
    EVT_AT_SetPos(p.firstOpponent, p.player, {.x = lateral, .y = 0.0f, .z = -2.52247f}, delay);
    EVT_AT_SetAngY(p.firstOpponent, p.player, 180.0f, delay);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_KERI_ST_1QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Angry, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Angry, 15, 1.0f, delay);
}
void YtqbKickSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? 0.191f : -0.191f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -1.4969f, 180.0f, delay);
    EVT_AT_SetSound(0x289u, delay + 17);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_KERI_SEIKOU_1QTE, 8.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 3u, 0xCu, 1u, 4, delay + 11);
    EVT_AT_SetVoice("S2BVA002", 1, delay);
    EVT_AT_SetSound(0x28Du, delay + 13);
    EVT_AT_SetSound(0x282u, delay + 58);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_KERI_SEIKOU_1QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB017", 1, delay + 25);
}
void YtqbKickFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? -0.0096899997f : 0.0096899997f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -1.1624399f, 180.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_KERI_SIPPAI_1QTE, 0.0f, 1.0f, p.motionFlags, delay + 5);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA008", 1, delay + 8);
    EVT_AT_SetSound(0x28Cu, delay + 8);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_KERI_SIPPAI_1QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetVoice("S2BVB009", 1, delay);
}
void YtqbKickCounterSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? -0.02815f : 0.02815f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -0.87369001f, 180.0f, delay);
    EVT_AT_SetSound(0x28Bu, delay + 13);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_HANGEKI_SEIKOU_1QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 8u, 0xCu, 1u, 15, delay);
    EVT_AT_SetVoice("S2BVA001", 1, delay);
    EVT_AT_SetSound(0x282u, delay + 34);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_HANGEKI_SEIKOU_1QTE, 31.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB023", 1, delay + 16);
}
void YtqbKickCounterFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? -0.16294f : 0.16294f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -0.71368998f, 180.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_HANGEKI_SIPPAI_1QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA012", 1, delay + 7);
    EVT_AT_SetSound(0x28Au, delay + 3);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_HANGEKI_SIPPAI_1QTE, 34.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetVoice("S2BVB013", 1, delay);
}
void YtqbBeginRearKick(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? 0.15888999f : -0.15888999f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, 1.22727f, 0.0f, delay);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_USIROKERI_SIPPAI_2QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Angry, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Angry, 15, 1.0f, delay);
}
void YtqbRearKickSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? 0.13082001f : -0.13082001f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, 2.1758299f, 0.0f, delay);
    EVT_AT_SetSound(0x289u, delay + 11);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_USIROKERI_SEIKOU_2QTE, 15.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 3u, 0xCu, 1u, 4, delay + 10);
    EVT_AT_SetVoice("S2BVA003", 1, delay);
    EVT_AT_SetSound(0x282u, delay + 34);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_USIROKERI_SEIKOU_2QTE, 35.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB028", 1, delay + 16);
}
void YtqbRearKickFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? 0.12888999f : -0.12888999f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, 1.0372699f, 0.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_USIROKERI_SIPPAI_2QTE, 5.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA013", 1, delay + 20);
    EVT_AT_SetSound(0x28Du, delay + 10);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_USIROKERI_SIPPAI_2QTE, 32.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetVoice("S2BVB001", 1, delay);
}
void YtqbBeginWristTwist(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? -0.61176997f : 0.61176997f;
    EVT_AT_SetPos(p.firstOpponent, p.player, {.x = lateral, .y = 0.0f, .z = -3.44645f}, delay);
    EVT_AT_SetAngY(p.firstOpponent, p.player, 180.0f, delay);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_KERI_ST_1QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Angry, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Angry, 15, 1.0f, delay);
}
void YtqbWristTwistSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? -0.17725f : 0.17725f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -2.1332099f, 180.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_KOTEKAESI_A_3QTE, 10.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 8u, 0xCu, 1u, 15, delay);
    EVT_AT_SetVoice("S2BVA002", 1, delay);
    EVT_AT_SetSound(0x28Du, delay + 6);
    EVT_AT_SetSound(0x282u, delay + 56);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_KOTEKAESI_A_3QTE, 10.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
}
void YtqbBeginBackfist(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float yaw;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    if (unmirrored) {
        EVT_AT_SetPos(p.firstOpponent, p.player, {.x = 1.4450901f, .y = 0.0f, .z = -1.41609f}, delay);
        yaw = 120.0f;
    } else {
        EVT_AT_SetPos(p.firstOpponent, p.player, {.x = -1.4450901f, .y = 0.0f, .z = -1.41609f}, delay);
        yaw = -120.0f;
    }
    EVT_AT_SetAngY(p.firstOpponent, p.player, yaw, delay);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_URAKEN_SEIKOU_5QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Angry, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Angry, 15, 1.0f, delay);
}
void YtqbBackfistSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral, secondLateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    if (unmirrored) {
        lateral = 120.0f;
        secondLateral = 1.4450901f;
    } else {
        lateral = -120.0f;
        secondLateral = -1.4450901f;
    }
    EVT_AT_SetPosAng(p.firstOpponent, p.player, secondLateral, -2.41609f, lateral, delay);
    EVT_AT_SetSound(0x6B7u, delay + 16);
    EVT_AT_SetSound(0x28Bu, delay + 31);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_URAKEN_SEIKOU_5QTE, 6.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 8u, 0xCu, 1u, 6, delay + 15);
    EVT_AT_SetVoice("S2BVA005", 1, delay);
    EVT_AT_SetSound(0x282u, delay + 47);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_URAKEN_SEIKOU_5QTE, 40.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB022", 1, delay + 37);
}
void YtqbBackfistFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral, secondLateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    if (unmirrored) {
        secondLateral = 120.0f;
        lateral = 1.08961f;
    } else {
        secondLateral = -120.0f;
        lateral = -1.08961f;
    }
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -1.7349401f, secondLateral, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_URAKEN_SIPPAI_5QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA010", 1, delay + 25);
    EVT_AT_SetSound(0x28Cu, delay + 15);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_URAKEN_SIPPAI_5QTE, 40.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetVoice("S2BVB005", 1, delay);
}
void YtqbKneeCounterSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral, secondLateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    if (unmirrored) {
        lateral = 120.0f;
        secondLateral = 0.57709998f;
    } else {
        lateral = -120.0f;
        secondLateral = -0.57709998f;
    }
    EVT_AT_SetPosAng(p.firstOpponent, p.player, secondLateral, -1.22592f, lateral, delay);
    EVT_AT_SetSound(649u, delay + 25);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_URAKEN_SIPPAI_HIZA_SEIKOU_5QTE, 5.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 3u, 12u, 1u, 6, delay + 10);
    EVT_AT_SetVoice("S2BVA003", 1, delay);
    EVT_AT_SetSound(0x282u, delay + 70);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_URAKEN_SIPPAI_HIZA_SEIKOU_5QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB030", 1, delay + 30);
}
void YtqbKneeCounterFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral, secondLateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    if (unmirrored) {
        secondLateral = 120.0f;
        lateral = 0.54641998f;
    } else {
        secondLateral = -120.0f;
        lateral = -0.54641998f;
    }
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -0.94591999f, secondLateral, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_URAKEN_SIPPAI_HIZA_SIPPAI_5QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA011", 1, delay + 23);
    EVT_AT_SetSound(0x28Cu, delay + 23);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_URAKEN_SIPPAI_HIZA_SIPPAI_5QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetVoice("S2BVB007", 1, delay); // @text EN: "Take this!" | JA: "くらえっ！"
}
void YtqbBeginPin(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float yaw;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    if (unmirrored) {
        EVT_AT_SetPos(p.firstOpponent, p.player, {.x = -1.64264f, .y = 0.0f, .z = -0.69735003f}, delay);
        EVT_AT_SetAngY(p.firstOpponent, p.player, 246.0f, delay);
        EVT_AT_SetPos(p.secondOpponent, p.player, {.x = 2.98405f, .y = 0.0f, .z = -0.31086001f}, delay);
        yaw = 90.0f;
    } else {
        EVT_AT_SetPos(p.firstOpponent, p.player, {.x = 1.64264f, .y = 0.0f, .z = -0.69735003f}, delay);
        EVT_AT_SetAngY(p.firstOpponent, p.player, -246.0f, delay);
        EVT_AT_SetPos(p.secondOpponent, p.player, {.x = -2.98405f, .y = 0.0f, .z = -0.31086001f}, delay);
        yaw = -90.0f;
    }
    EVT_AT_SetAngY(p.secondOpponent, p.player, yaw, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_OSAE_SIPPAI_6QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Angry, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Angry, 15, 1.0f, delay);
}
void YtqbPinSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral, secondLateral, firstYaw, secondYaw;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    if (unmirrored) {
        firstYaw = 270.0f;
        lateral = -2.2121899f;
    } else {
        firstYaw = -270.0f;
        lateral = 2.2121899f;
    }
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -0.37863001f, firstYaw, delay);
    if ((p.motionFlags & 0x20000000) != 0) {
        secondYaw = -90.0f;
        secondLateral = -2.98405f;
    } else {
        secondYaw = 90.0f;
        secondLateral = 2.98405f;
    }
    EVT_AT_SetPosAng(p.secondOpponent, p.player, secondLateral, -0.31086001f, secondYaw, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_OSAE_SEIKOU_6QTE, 10.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetVoice("S2BVA002", 1, delay);
    EVT_AT_SetSound(0x28Du, delay + 5);
    EVT_AT_SetSound(0x602u, delay + 10);
    EVT_AT_SetSound(0x6E7u, delay + 30);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_OSAE_SEIKOU_6QTE, 26.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetMot(p.secondOpponent, YTQB_LLYM_XXX_0048E_OSAE_SEIKOU_KERI_SEIKOU_B_6QTE, 0.0f, 1.0f, p.motionFlags, delay + 7);
    EVT_AT_SetVoice("S2BVB021", 1, delay + 20);
}
void YtqbPinFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float yaw;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    if (unmirrored) {
        EVT_AT_SetPos(p.firstOpponent, p.player, {.x = -1.64264f, .y = 0.0f, .z = -0.69735003f}, delay);
        EVT_AT_SetAngY(p.firstOpponent, p.player, 246.0f, delay);
        EVT_AT_SetPos(p.secondOpponent, p.player, {.x = 2.98405f, .y = 0.0f, .z = -0.31086001f}, delay);
        yaw = 90.0f;
    } else {
        EVT_AT_SetPos(p.firstOpponent, p.player, {.x = 1.64264f, .y = 0.0f, .z = -0.69735003f}, delay);
        EVT_AT_SetAngY(p.firstOpponent, p.player, -246.0f, delay);
        EVT_AT_SetPos(p.secondOpponent, p.player, {.x = -2.98405f, .y = 0.0f, .z = -0.31086001f}, delay);
        yaw = -90.0f;
    }
    EVT_AT_SetAngY(p.secondOpponent, p.player, yaw, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_OSAE_SIPPAI_6QTE, 35.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA009", 1, delay + 26);
    EVT_AT_SetSound(0x28Cu, delay + 20);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_OSAE_SIPPAI_6QTE, 6.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetVoice("S2BVB009", 1, delay);
}
void YtqbPinKickSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral, secondLateral, firstYaw, secondYaw;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    if (unmirrored) {
        firstYaw = 270.0f;
        lateral = -0.29901001f;
    } else {
        firstYaw = -270.0f;
        lateral = 0.29901001f;
    }
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -0.25656f, firstYaw, delay);
    if ((p.motionFlags & 0x20000000) != 0) {
        secondYaw = -90.0f;
        secondLateral = -2.98405f;
    } else {
        secondYaw = 90.0f;
        secondLateral = 2.98405f;
    }
    EVT_AT_SetPosAng(p.secondOpponent, p.player, secondLateral, -0.31086001f, secondYaw, delay);
    EVT_AT_SetSound(0x289u, delay + 14);
    EVT_AT_SetSound(0x28Bu, delay + 36);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_OSAE_SEIKOU_KERI_SEIKOU_6QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 3u, 0xCu, 1u, 6, delay + 10);
    EVT_AT_SetVoice("S2BVA003", 1, delay);
    EVT_AT_SetSound(0x282u, delay + 76);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_OSAE_SEIKOU_KERI_SEIKOU_A_6QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB023", 1, delay + 20);
    EVT_AT_SetSound(0x282u, delay + 33);
    EVT_AT_SetMot(p.secondOpponent, YTQB_LLYM_XXX_0048E_OSAE_SEIKOU_KERI_SEIKOU_B_6QTE, 25.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetVoice("S2BVB020", 1, delay + 40);
}
void YtqbPinKickFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral, secondLateral, firstYaw;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    if (unmirrored) {
        firstYaw = 270.0f;
        lateral = -0.50663f;
    } else {
        firstYaw = -270.0f;
        lateral = 0.50663f;
    }
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -0.024639999f, firstYaw, delay);
    if ((p.motionFlags & 0x20000000) != 0)
        secondLateral = -1.85807f;
    else
        secondLateral = 1.85807f;
    EVT_AT_SetPosAng(p.secondOpponent, p.player, secondLateral, 0.77458f, 0.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_OSAE_SEIKOU_KERI_SIPPAI_6QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA007", 1, delay + 28);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_OSAE_SEIKOU_KERI_SIPPAI_A_6QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetSound(0x28Au, delay + 21);
    EVT_AT_SetMot(p.secondOpponent, YTQB_LLYM_XXX_0048E_OSAE_SEIKOU_KERI_SIPPAI_B_6QTE, 22.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetVoice("S2BVB013", 1, delay);
}
void YtqbBeginGrabEvadeA(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SEIKOU_A_QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Angry, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Angry, 15, 1.0f, delay);
}
void YtqbGrabEvadeASuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? 0.0098799998f : -0.0098799998f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -2.2938099f, 180.0f, delay);
    EVT_AT_SetSound(0x28Bu, delay + 20);
    EVT_AT_SetSound(0x6B7u, delay + 8);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_TUKAMIKAIHI_SEIKOU_A_7QTE, 10.0f, 1.0f, (p.motionFlags ^ 0x20000000), delay);
    EVT_AT_DispBlur(p.player, 1u, 8u, 1u, 6, delay + 17);
    EVT_AT_SetVoice("S2BVA002", 1, delay);
    EVT_AT_SetSound(0x282u, delay + 42);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SEIKOU_A_QTE, 12.0f, 1.0f, (p.motionFlags ^ 0x20000000 | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB026", 1, delay + 30);
}
void YtqbGrabEvadeAFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? -0.17863999f : 0.17863999f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -2.2607501f, 180.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_TUKAMIKAIHI_SIPPAI_7QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SIPPAI_B_7QTE, 0.0f, 1.0f, p.motionFlags, delay + 31);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SIPPAI_7QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SIPPAI_B_7QTE, 0.0f, 1.0f, p.motionFlags, delay + 31);
    EVT_AT_SetVoice("S2BVB004", 1, delay);
}
void YtqbBeginGrabEvadeB(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SEIKOU_B_7QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Angry, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Angry, 15, 1.0f, delay);
}
void YtqbGrabEvadeBSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? -0.00079000002f : 0.00079000002f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -2.30879f, 180.0f, delay);
    EVT_AT_SetSound(0x289u, delay + 27);
    EVT_AT_SetSound(0x6B7u, delay + 12);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_TUKAMIKAIHI_SEIKOU_B_7QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 1u, 8u, 1u, 4, delay + 25);
    EVT_AT_SetVoice("S2BVA002", 1, delay);
    EVT_AT_SetSound(0x282u, delay + 41);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SEIKOU_B_7QTE, 16.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB017", 1, delay + 30);
}
void YtqbGrabEvadeBFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? -0.17863999f : 0.17863999f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -2.1607499f, 180.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_TUKAMIKAIHI_SIPPAI_7QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SIPPAI_B_7QTE, 0.0f, 1.0f, p.motionFlags, delay + 31);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SIPPAI_7QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SIPPAI_B_7QTE, 0.0f, 1.0f, p.motionFlags, delay + 31);
    EVT_AT_SetVoice("S2BVB006", 1, delay);
}
void YtqbThrowASuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? -0.0557f : 0.0557f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -0.77547997f, 180.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SEIKOU_A_7QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 1u, 8u, 1u, 15, delay);
    EVT_AT_SetVoice("S2BVA003", 1, delay);
    EVT_AT_SetSound(0x282u, delay + 60);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SEIKOU_A_7QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB023", 1, delay + 50);
}
void YtqbThrowAFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? -0.25895f : 0.25895f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -0.87206f, 180.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SIPPAI_A_7QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA012", 1, delay + 15);
    EVT_AT_SetSound(0x28Au, delay + 12);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SIPPAI_A_7QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetVoice("S2BVB005", 1, delay);
}
void YtqbThrowBSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? -0.0046600001f : 0.0046600001f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -0.69023001f, 180.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SEIKOU_B_7QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 1u, 8u, 1u, 15, delay);
    EVT_AT_SetVoice("S2BVA002", 1, delay);
    EVT_AT_SetSound(0x282u, delay + 71);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SEIKOU_B_7QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB018", 1, delay + 60);
}
void YtqbThrowBFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? -0.25895f : 0.25895f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -0.87206f, 180.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SIPPAI_A_7QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA009", 1, delay + 15);
    EVT_AT_SetSound(0x28Au, delay + 12);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_TUKAMIKAIHI_SIPPAI_NAGE_SIPPAI_A_7QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetVoice("S2BVB003", 1, delay);
}
void YtqbBeginKickDodge(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_KICKSABAKI_SEIKOU_A_8QTE, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Angry, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Angry, 15, 1.0f, delay);
}
void YtqbKickDodgeSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? 0.0049399999f : -0.0049399999f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -2.75157f, 180.0f, delay);
    EVT_AT_SetSound(0x28Bu, delay + 22);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_AKI_KICKSABAKI_SEIKOU_A_8QTE, 10.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 1u, 8u, 1u, 4, delay + 11);
    EVT_AT_SetVoice("S2BVA004", 1, delay);
    EVT_AT_SetSound(0x282u, delay + 38);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_KICKSABAKI_SEIKOU_A_8QTE, 10.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB023", 1, delay + 32);
}
void YtqbKickDodgeFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? 0.0058300002f : -0.0058300002f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -2.2780299f, 180.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_AKI_KICKSABAKI_SIPPAI_8QTE, 14.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_HANGEKI_SIPPAI_8QTE, 0.0f, 1.0f, p.motionFlags, delay + 2);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA008", 1, delay + 12);
    EVT_AT_SetSound(0x28Au, delay + 2);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_KICKSABAKI_SIPPAI_8QTE, 14.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_HANGEKI_SIPPAI_8QTE, 0.0f, 1.0f, p.motionFlags, delay + 2);
    EVT_AT_SetVoice("S2BVB005", 1, delay);
}
void YtqbKickDodgeCounterSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? 0.15020999f : -0.15020999f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -0.90296f, 180.0f, delay);
    EVT_AT_SetSound(0x289u, delay + 18);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_HANGEKI_SEIKOU_8QTE, 30.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 1u, 8u, 1u, 4, delay + 11);
    EVT_AT_SetVoice("S2BVA005", 1, delay);
    EVT_AT_SetSound(0x282u, delay + 36);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_HANGEKI_SEIKOU_8QTE, 30.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB018", 1, delay + 28);
}
void YtqbKickDodgeCounterFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? 0.15020999f : -0.15020999f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -0.88231999f, 180.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_HANGEKI_SIPPAI_8QTE, 43.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA010", 1, delay + 28);
    EVT_AT_SetSound(0x28Cu, delay + 18);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_HANGEKI_SIPPAI_8QTE, 40.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetVoice("S2BVB002", 1, delay);
}
void YtqbFlyingKickSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral, secondLateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    if (unmirrored) {
        lateral = -270.0f;
        secondLateral = 2.2537f;
    } else {
        lateral = 270.0f;
        secondLateral = -2.2537f;
    }
    EVT_AT_SetPosAng(p.firstOpponent, p.player, secondLateral, -0.79279f, lateral, delay);
    EVT_AT_SetSound(0x289u, delay + 22);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_HISSATU_TOBIGERI_SEIKOU_QTE_NEW0300, 0.0f, 1.0f, (p.motionFlags ^ 0x20000000), delay);
    EVT_AT_DispBlur(p.player, 1u, 6u, 1u, 50, delay);
    EVT_AT_SetVoice("S2BVA004", 1, delay);
    EVT_AT_SetSound(0x282u, delay + 47);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_URAKEN_SEIKOU_5QTE, 47.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB022", 1, delay + 30);
}
void YtqbFinishingBackfistSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral, secondLateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    if (unmirrored) {
        lateral = 90.0f;
        secondLateral = 2.36129f;
    } else {
        lateral = -90.0f;
        secondLateral = -2.36129f;
    }
    EVT_AT_SetPosAng(p.firstOpponent, p.player, secondLateral, -1.06471f, lateral, delay);
    EVT_AT_SetSound(0x28Bu, delay + 18);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_HISSATU_URAKEN_SEIKOU_QTE_NEW0300, 0.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_DispBlur(p.player, 2u, 0xCu, 1u, 10, delay + 10);
    EVT_AT_SetVoice("S2BVA004", 1, delay);
    EVT_AT_SetSound(0x282u, delay + 40);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_URAKEN_SEIKOU_5QTE, 52.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB023", 1, delay + 20);
}
void YtqbBeginRearKickAssist(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? -0.00978f : 0.00978f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, 2.6208401f, 0.0f, delay);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_USIROKERI_SEIKOU_2QTE, 0.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Angry, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Angry, 15, 1.0f, delay);
}
void YtqbRearKickAssistSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    EVT_AT_SetAngY(p.player, p.player, 200.0f, delay);
    if ((p.motionFlags & 0x20000000) != 0)
        lateral = 0.00978f;
    else
        lateral = -0.00978f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, -2.6208401f, 180.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_BATTLE_B_0, 138.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetVoice("S2BVA001", 1, delay);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_USIROKERI_SEIKOU_2QTE, 29.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetSound(0x289u, delay + 15);
    EVT_AT_SetSound(0x282u, delay + 40);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB017", 1, delay + 20);
}
void YtqbRearKickAssistFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? 0.12888999f : -0.12888999f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, 1.0372699f, 0.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_USIROKERI_SIPPAI_2QTE, 5.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA013", 1, delay + 20);
    EVT_AT_SetSound(0x28Du, delay + 10);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_USIROKERI_SIPPAI_2QTE, 32.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetVoice("S2BVB005", 1, delay);
}
void YtqbBeginBackfistAssist(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float yaw;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    if (unmirrored) {
        EVT_AT_SetAngY(p.player, p.player, 270.0f, delay);
        EVT_AT_SetPosAng(p.firstOpponent, p.player, 1.14463f, 2.0968699f, 0.0f, delay);
        yaw = -270.0f;
    } else {
        EVT_AT_SetAngY(p.player, p.player, -270.0f, delay);
        EVT_AT_SetPosAng(p.firstOpponent, p.player, -1.14463f, 2.0968699f, 0.0f, delay);
        yaw = 270.0f;
    }
    EVT_AT_SetAngY(p.player, p.player, yaw, delay);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_URAKEN_SEIKOU_5QTE, 32.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Angry, 15, 1.0f, delay);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Angry, 15, 1.0f, delay);
}
void YtqbBackfistAssistSuccess(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    if (unmirrored) {
        EVT_AT_SetAngY(p.player, p.player, 270.0f, delay);
        lateral = 1.14463f;
    } else {
        EVT_AT_SetAngY(p.player, p.player, -270.0f, delay);
        lateral = -1.14463f;
    }
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, 2.0968699f, 0.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_OTASUKE_E_0, 60.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetVoice("S2BVA003", 1, delay);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_URAKEN_SEIKOU_5QTE, 49.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetSound(0x28Cu, delay + 18);
    EVT_AT_SetSound(0x282u, delay + 40);
    EVT_AT_SetFaceLooks(p.firstOpponent, FacialExpressionId::Suffer, 15, 1.0f, delay + 10);
    EVT_AT_SetVoice("S2BVB023", 1, delay + 20);
}
void YtqbBackfistAssistFailure(void *data, std::uint32_t delayFrames) {
    YtqbResultParameters &p = *static_cast<YtqbResultParameters *>(data);
    const std::int32_t delay = std::bit_cast<std::int32_t>(delayFrames);
    float lateral;
    const bool unmirrored = (p.motionFlags & 0x20000000) == 0;
    lateral = unmirrored ? 0.12888999f : -0.12888999f;
    EVT_AT_SetPosAng(p.firstOpponent, p.player, lateral, 1.19727f, 0.0f, delay);
    EVT_AT_SetMot(p.player, YTQB_RYOM_RYO_0048E_USIROKERI_SIPPAI_2QTE, 5.0f, 1.0f, p.motionFlags, delay);
    EVT_AT_SetFaceLooks(p.player, FacialExpressionId::Suffer, 15, 1.0f, delay + 15);
    EVT_AT_SetVoice("S2BVA013", 1, delay + 20);
    EVT_AT_SetSound(0x28Du, delay + 10);
    EVT_AT_SetMot(p.firstOpponent, YTQB_LLYM_XXX_0048E_USIROKERI_SIPPAI_2QTE, 32.0f, 1.0f, (p.motionFlags | 0x40000000), delay);
    EVT_AT_SetVoice("S2BVB004", 1, delay);
}
} // namespace shenmue::s2::areas
#endif
