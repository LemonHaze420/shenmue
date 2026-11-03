#include "s2/areas/et_tejo_dc.h"

int renQtePromptPending __attribute__((section(".bss")));
int renQteRetryDifficulty __attribute__((section(".bss")));

int UpdateRenQteEvent(RenQteWork *work)
{
    if (!work)
        FTBL.EventAssertNullsub(__FILE__, 144);
    if (!work->entry)
        FTBL.EventAssertNullsub(__FILE__, 145);
    if (work->timer)
        work->timer--;
    switch (work->state) {
    case REN_QTE_INIT:
        InitializeRenQteEntry(work);
        PlayRenQteCameras(work->entry->initialCameras);
        PlayRenQteVoices(work->entry->initialVoices);
        break;
    case REN_QTE_START:
        WaitRenQteApproach(work);
        break;
    case REN_QTE_TEST:
        break;
    case REN_QTE_INPUT:
        PollRenQteInput(work);
        break;
    case REN_QTE_SUCCESS:
        renQteSuccessCallbacks[work->entry->type](work);
        PlayRenQteCameras(work->entry->successCameras);
        PlayRenQteVoices(work->entry->successVoices);
        break;
    case REN_QTE_FAILURE:
        work->lives--;
        renQteFailureCallbacks[work->entry->type](work);
        PlayRenQteCameras(work->entry->failureCameras);
        PlayRenQteVoices(work->entry->failureVoices);
        if (work->lives <= 0)
            work->state = REN_QTE_CAPTURE;
        break;
    case REN_QTE_FINISH:
        return IsRenQteTimerDone(work);
    case REN_QTE_CAPTURE:
        if (IsRenQteTimerDone(work))
            CaptureRenQtePursuers(work);
        break;
    default:
        FTBL.EventAssertMessageNullsub(__FILE__, 188, "Illegal mode.");
        break;
    }
    return 0;
}

void PlayRenQteCameras(RenQteCamera *cameras)
{
    if (!cameras)
        return;
    if (!cameras->camera) {
        FTBL.camCtrlSetState(0);
        FTBL.camCtrlSetFovTransitionTo(1);
    }
    for (; cameras->camera; cameras++)
        EVT_AT_SetCam(cameras->camera, cameras->attachment, cameras->attachment, 0, 0, cameras->delay);
}

void PlayRenQteVoices(RenQteVoice *voices)
{
    if (!voices)
        return;
    for (; voices->voice; voices++)
        EVT_AT_SetVoice(voices->voice, voices->delay);
}

void QueueRenQteFadeOutSound(int delay)
{
    FTBL.HLib_SetupTimedTask(FTBL.HLib_EnqueueTaskWithoutParameter(RenQteFadeOutSoundCallback, 6, 11, REN_QTE_SOUND_FADE_TASK), delay);
}

void RenQteFadeOutSoundCallback(void *data)
{
    FTBL.spmFadeOut(0);
    FTBL.HLib_DestroyCurrentTask();
}

void InitializeRenQteEntry(RenQteWork *work)
{
    shen_name base = work->base;
    int difficulty;
    Vector3 position;
    Vector3i rotation;
    work->state = REN_QTE_START;
    work->doorAttempts = 0;
    difficulty = work->retryCount;
    if (difficulty > 4)
        difficulty = 4;
    renQteRetryDifficulty = difficulty;
    SHENMUE_EVENT_BOUNDARY();
    work->triggerDistance = renQteRetryDifficulty + 6.5f;
    FTBL.GetCharaPosition(REN_QTE_PLAYER, -1, &position, 0x40000000);
    position.x = work->entry->startX;
    position.z = work->entry->startZ;
    rotation.x = rotation.z = 0;
    rotation.y = work->entry->startYaw;
    FTBL.EV_AT_SetPos(base, 0x78000000, &position);
    FTBL.EV_AT_SetAngY_0(base, 0x38000000, &rotation, 0);
    FTBL.AT_SetLocalBaseChara(REN_QTE_PLAYER, base);
    FTBL.AT_SetLocalBaseChara(REN_QTE_REN, base);
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(REN_QTE_PLAYER), 3);
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(REN_QTE_REN), 3);
    FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(REN_QTE_PLAYER), 5, 0);
    FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(REN_QTE_REN), 5, 0);
    FTBL.EV_EnableCharacterDisplay(REN_QTE_DOOR);
    FTBL.EV_DisableCharacterDisplay(REN_QTE_BROKEN_DOOR);
    switch (work->entry->type) {
    case 7:
        BeginRenQteDoorApproach(work);
        break;
    case 8:
        BeginRenQteExitApproach(work);
        break;
    case 9:
        BeginRenQteRightExitApproach(work);
        break;
    default:
        BeginRenQteRunApproach(work);
        break;
    }
    FTBL.camCtrlSetFovTransitionTo(1);
    renQtePromptPending = 0;
}

void BeginRenQteDoorApproach(RenQteWork *work)
{
    shen_name base = work->base;
    EVT_AT_SetPos(REN_QTE_PLAYER, base, 0.4f, 0.0f, work->entry->approachDistance, 0);
    EVT_AT_SetAngY(REN_QTE_PLAYER, base, 0.0f, 0);
    EVT_AT_SetPos(REN_QTE_REN, base, -0.4f, 0.0f, work->entry->approachDistance + 0.25f, 0);
    EVT_AT_SetAngY(REN_QTE_REN, base, 0.0f, 0);
    work->timer = EVT_AT_SetMot(REN_QTE_PLAYER, 7825, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetMot(REN_QTE_REN, 7809, 0.0f, 1.0f, 0, 0);
    FTBL.hmdlSetFlagsFor(FTBL.ctrlGetCharaByName(REN_QTE_PLAYER), 0, 0, 16);
    FTBL.EV_EnableCharacterDisplay(REN_QTE_DOOR);
    FTBL.EV_DisableCharacterDisplay(REN_QTE_BROKEN_DOOR);
    work->lives = 3;
}

void BeginRenQteExitApproach(RenQteWork *work)
{
    shen_name base = work->base;
    EVT_AT_SetPos(REN_QTE_PLAYER, base, 0.4f, 0.0f, work->entry->approachDistance, 0);
    EVT_AT_SetAngY(REN_QTE_PLAYER, base, 0.0f, 0);
    EVT_AT_SetPos(REN_QTE_REN, base, -0.4f, 0.0f, work->entry->approachDistance, 0);
    EVT_AT_SetAngY(REN_QTE_REN, base, 0.0f, 0);
    EVT_AT_SetMot(REN_QTE_PLAYER, 7835, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetMot(REN_QTE_REN, 7819, 0.0f, 1.0f, 0x40000000, 0);
    work->state = REN_QTE_START;
    work->triggerDistance = 0.0f;
}

void BeginRenQteRightExitApproach(RenQteWork *work)
{
    shen_name base = work->base;
    EVT_AT_SetPos(REN_QTE_PLAYER, base, 0.4f, 0.0f, work->entry->approachDistance, 0);
    EVT_AT_SetAngY(REN_QTE_PLAYER, base, 0.0f, 0);
    EVT_AT_SetPos(REN_QTE_REN, base, -0.4f, 0.0f, work->entry->approachDistance, 0);
    EVT_AT_SetAngY(REN_QTE_REN, base, 0.0f, 0);
    work->state = REN_QTE_START;
    work->triggerDistance = 3.0f;
}

void BeginRenQteRunApproach(RenQteWork *work)
{
    shen_name base = work->base;
    EVT_AT_SetPos(REN_QTE_PLAYER, base, 0.4f, 0.0f, work->entry->approachDistance, 0);
    EVT_AT_SetAngY(REN_QTE_PLAYER, base, 0.0f, 0);
    EVT_AT_SetPos(REN_QTE_REN, base, -0.4f, 0.0f, work->entry->approachDistance, 0);
    EVT_AT_SetAngY(REN_QTE_REN, base, 0.0f, 0);
    EVT_AT_SetMot(REN_QTE_PLAYER, 7835, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetMot(REN_QTE_REN, 7819, 0.0f, 1.0f, 0x40000000, 0);
    work->timer = 0;
}

void WaitRenQteApproach(RenQteWork *work)
{
    Vector3 position;
    if (work->timer)
        return;
    FTBL.GetCharaPosition(REN_QTE_PLAYER, -1, &position, 0);
    if (position.z > work->triggerDistance)
        return;
    work->timer = renQteRetryDifficulty * 4 + 12;
    switch (work->entry->type) {
    case 8:
        FinishRenQteExit(work);
        break;
    case 9:
        FinishRenQteRightExit(work);
        break;
    case 7:
        FTBL.qteBattleUtil(8, 1, 6, work->timer, 0);
        FTBL.qteBattleUtil(9, 1, 256, 0, 0);
        FTBL.qteBattleUtil(3, 0, 0, 0, 0);
        work->doorAttempts++;
        work->state = REN_QTE_INPUT;
        break;
    default:
        FTBL.SetShowQteFlagForButtonType(work->entry->command, work->timer, 2);
        work->state = REN_QTE_INPUT;
        break;
    }
    renQtePromptPending = 0;
}

void FinishRenQteExit(RenQteWork *work)
{
    int frames = EVT_AT_SetMot(REN_QTE_PLAYER, 0xf008, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetMot(REN_QTE_REN, 0xf008, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetPos(REN_QTE_PLAYER, REN_QTE_PLAYER, 0.0f, -0.9f, 0.0f, frames - 1);
    EVT_AT_SetPos(REN_QTE_REN, REN_QTE_REN, 0.0f, -0.9f, 0.0f, frames - 1);
    EVT_AT_MenCtrl(20, 0, 0, 0, 0, 255, 0, 0, 0, 0);
    work->timer = 20;
    work->state = REN_QTE_FINISH;
}

void CaptureRenQtePursuers(RenQteWork *work)
{
    Vector3 position;
    Vector3i rotation;
    int offset;
    FTBL.camCtrlSetState(0);
    FTBL.camCtrlSetFovTransitionTo(1);
    FTBL.EV_EnableCharacterDisplay(REN_QTE_KD7);
    FTBL.EV_EnableCharacterDisplay(REN_QTE_KK1);
    FTBL.EV_EnableCharacterDisplay(REN_QTE_KT1);
    FTBL.EV_EnableCharacterDisplay(REN_QTE_KU7);
    FTBL.EV_EnsureCharacterDraw(REN_QTE_KD7);
    FTBL.EV_EnsureCharacterDraw(REN_QTE_KK1);
    FTBL.EV_EnsureCharacterDraw(REN_QTE_KT1);
    FTBL.EV_EnsureCharacterDraw(REN_QTE_KU7);
    FTBL.charaSetDisplayEnabledFlag(FTBL.ctrlGetCharaByName(REN_QTE_KD7), 0);
    FTBL.charaSetDisplayEnabledFlag(FTBL.ctrlGetCharaByName(REN_QTE_KK1), 0);
    FTBL.charaSetDisplayEnabledFlag(FTBL.ctrlGetCharaByName(REN_QTE_KT1), 0);
    FTBL.charaSetDisplayEnabledFlag(FTBL.ctrlGetCharaByName(REN_QTE_KU7), 0);
    position.x = work->entry->endX;
    position.y = FTBL.GetQronTemporaryFloorHeight();
    position.z = work->entry->endZ;
    rotation.x = rotation.z = 0;
    rotation.y = work->entry->endYaw;
    FTBL.EV_AT_SetPos(REN_QTE_BASE, 0x78000000, &position);
    FTBL.EV_AT_SetAngY_0(REN_QTE_BASE, 0x38000000, &rotation, 0);
    if (work->entry->type == 7) {
        EVT_AT_SetPos(REN_QTE_PLAYER, REN_QTE_BASE, -0.2f, 0.0f, -0.5f, 0);
        EVT_AT_SetAngY(REN_QTE_PLAYER, REN_QTE_BASE, 0.0f, 0);
        EVT_AT_SetPos(REN_QTE_REN, REN_QTE_BASE, 0.5f, 0.0f, -0.5f, 0);
        EVT_AT_SetAngY(REN_QTE_REN, REN_QTE_BASE, 180.0f, 0);
        EVT_AT_SetPos(REN_QTE_KD7, REN_QTE_BASE, 1.0f, -0.05f, 4.0f, 0);
        EVT_AT_SetAngY(REN_QTE_KD7, REN_QTE_BASE, 0.0f, 0);
        EVT_AT_SetPos(REN_QTE_KK1, REN_QTE_BASE, 0.5f, -0.2f, 5.0f, 0);
        EVT_AT_SetAngY(REN_QTE_KK1, REN_QTE_BASE, 0.0f, 0);
        EVT_AT_SetPos(REN_QTE_KT1, REN_QTE_BASE, -1.0f, -0.15f, 5.0f, 0);
        EVT_AT_SetAngY(REN_QTE_KT1, REN_QTE_BASE, 0.0f, 0);
        EVT_AT_SetPos(REN_QTE_KU7, REN_QTE_BASE, -0.5f, -0.1f, 4.5f, 0);
        EVT_AT_SetAngY(REN_QTE_KU7, REN_QTE_BASE, 0.0f, 0);
        EVT_AT_SetCam(2001, REN_QTE_BASE, REN_QTE_BASE, 0, 0, 0);
        offset = 80;
    } else {
        EVT_AT_SetPos(REN_QTE_PLAYER, REN_QTE_BASE, 0.4f, 0.0f, 1.5f, 0);
        EVT_AT_SetAngY(REN_QTE_PLAYER, REN_QTE_BASE, 0.0f, 0);
        EVT_AT_SetPos(REN_QTE_REN, REN_QTE_BASE, -0.4f, 0.0f, 1.5f, 0);
        EVT_AT_SetAngY(REN_QTE_REN, REN_QTE_BASE, 0.0f, 0);
        EVT_AT_SetPos(REN_QTE_KD7, REN_QTE_BASE, 0.6f, -0.05f, 4.0f, 0);
        EVT_AT_SetAngY(REN_QTE_KD7, REN_QTE_BASE, 0.0f, 0);
        EVT_AT_SetPos(REN_QTE_KK1, REN_QTE_BASE, -0.6f, -0.2f, 4.0f, 0);
        EVT_AT_SetAngY(REN_QTE_KK1, REN_QTE_BASE, 0.0f, 0);
        EVT_AT_SetPos(REN_QTE_KT1, REN_QTE_BASE, 0.6f, -0.1f, -4.0f, 0);
        EVT_AT_SetAngY(REN_QTE_KT1, REN_QTE_BASE, 180.0f, 0);
        EVT_AT_SetPos(REN_QTE_KU7, REN_QTE_BASE, -0.6f, -0.15f, -4.0f, 0);
        EVT_AT_SetAngY(REN_QTE_KU7, REN_QTE_BASE, 180.0f, 0);
        EVT_AT_SetCam(2000, REN_QTE_BASE, REN_QTE_BASE, 0, 0, 0);
        offset = 0;
    }
    EVT_AT_SetMot(REN_QTE_PLAYER, 7839, offset, 1.0f, 0x40000000, 0);
    EVT_AT_SetMot(REN_QTE_REN, 7823, offset, 1.0f, 0x40000000, 0);
    EVT_AT_SetMot(REN_QTE_KD7, 7841, 0.0f, 1.0f, 0, 0);
    EVT_AT_SetMot(REN_QTE_KK1, 7842, 30.0f, 1.0f, 0, 0);
    EVT_AT_SetMot(REN_QTE_KT1, 7843, 0.0f, 1.0f, 0, 0);
    EVT_AT_SetMot(REN_QTE_KU7, 7842, 0.0f, 1.0f, 0, 0);
    EVT_AT_SetVoice("0209B041", 10);
    EVT_AT_SetVoice("0209B038", 100 - offset / 2);
    EVT_AT_SetVoice("0209F001", 240 - offset);
    if (work->entry->type == 7) {
        EVT_AT_SetVoice("0209G001", 280 - offset);
        EVT_AT_SetVoice("0209G002", 320 - offset);
    } else {
        EVT_AT_SetVoice("0209H001", 280 - offset);
        EVT_AT_SetVoice("0209H002", 310 - offset);
    }
    EVT_AT_SetSound(2828, 300 - offset);
    EVT_AT_SetVoice("0209A023", 345 - offset);
    EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, 210 - offset);
    QueueRenQteFadeOutSound(210 - offset);
    work->timer = 435 - offset;
    work->state = REN_QTE_FINISH;
}

void FinishRenQteRightExit(RenQteWork *work)
{
    EVT_AT_SetMot(REN_QTE_PLAYER, 7832, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetMot(REN_QTE_REN, 7816, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, 30);
    work->timer = 60;
    work->state = REN_QTE_FINISH;
}

void PollRenQteInput(RenQteWork *work)
{
    Vector3 position;
    int result;
    FTBL.GetCharaPosition(REN_QTE_PLAYER, -1, &position, 0);
    if (FTBL.GetShouldRenderIcons(3))
        return;
    if (work->entry->type == 7) {
        result = FTBL.qteBattleUtil(4, 0, 0, 0, 0);
        if (result == 1)
            work->state = REN_QTE_SUCCESS;
        else if (result == -1)
            work->state = REN_QTE_FAILURE;
    } else if ((FTBL.GetCurrentQteStatus() & 0xfff) == work->entry->command)
        work->state = REN_QTE_SUCCESS;
    else
        work->state = REN_QTE_FAILURE;
    renQtePromptPending = 0;
}

int IsRenQteTimerDone(RenQteWork *work)
{
    if (work->timer)
        return 0;
    work->state = REN_QTE_INIT;
    return 1;
}

void TurnRenQteLeftSuccess(RenQteWork *work)
{
    shen_name base = work->base;
    int frames;
    EVT_AT_SetPos(REN_QTE_PLAYER, base, 0.4f, 0.0f, 4.3f, 0);
    EVT_AT_SetAngY(REN_QTE_PLAYER, base, 0.0f, 0);
    EVT_AT_SetMot(REN_QTE_PLAYER, 7831, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetPos(REN_QTE_REN, base, -0.4f, 0.0f, 4.3f, 0);
    EVT_AT_SetAngY(REN_QTE_REN, base, 0.0f, 0);
    frames = EVT_AT_SetMot(REN_QTE_REN, 7815, 0.0f, 1.0f, 0x40000000, 0);
    if (work->entry->successFrames)
        work->timer = work->entry->successFrames;
    else
        work->timer = frames + 30;
    if (work->timer > frames) {
        EVT_AT_SetAngY(REN_QTE_PLAYER, base, 90.0f, frames);
        EVT_AT_SetMot(REN_QTE_PLAYER, 7835, 10.0f, 1.0f, 0, frames);
        EVT_AT_SetPos(REN_QTE_REN, REN_QTE_REN, 0.7f, 0.0f, 0.0f, frames);
        EVT_AT_SetAngY(REN_QTE_REN, REN_QTE_REN, 90.0f, frames);
        EVT_AT_SetMot(REN_QTE_REN, 7819, 0.0f, 1.0f, 0, frames);
    }
    work->state = REN_QTE_FINISH;
}

void TurnRenQteLeftFailure(RenQteWork *work)
{
    shen_name base = work->base;
    int frames;
    EVT_AT_SetPos(REN_QTE_PLAYER, base, 0.25f, 0.0f, 4.3f, 0);
    EVT_AT_SetAngY(REN_QTE_PLAYER, base, 0.0f, 0);
    frames = EVT_AT_SetMot(REN_QTE_PLAYER, 7840, 0.0f, 0.85f, 0x40000000, 0);
    EVT_AT_SetPos(REN_QTE_REN, base, -0.25f, 0.0f, 4.3f, 0);
    EVT_AT_SetAngY(REN_QTE_REN, base, 0.0f, 0);
    EVT_AT_SetMot(REN_QTE_REN, 7824, 0.0f, 0.85f, 0x40000000, 0);
    if (work->entry->failureFrames)
        work->timer = work->entry->failureFrames;
    else
        work->timer = frames;
    if (work->timer > frames) {
        EVT_AT_SetAngY(REN_QTE_PLAYER, base, 90.0f, frames);
        EVT_AT_SetMot(REN_QTE_PLAYER, 7835, 1.0f, 1.0f, 0, frames);
        EVT_AT_SetAngY(REN_QTE_REN, base, 90.0f, frames);
        EVT_AT_SetMot(REN_QTE_REN, 7819, 9.0f, 1.0f, 0, frames);
    }
    work->state = REN_QTE_FINISH;
}

void TurnRenQteRightSuccess(RenQteWork *work)
{
    shen_name base = work->base;
    int frames;
    EVT_AT_SetPos(REN_QTE_PLAYER, base, 0.4f, 0.0f, 4.3f, 0);
    EVT_AT_SetAngY(REN_QTE_PLAYER, base, 0.0f, 0);
    EVT_AT_SetMot(REN_QTE_PLAYER, 7832, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetPos(REN_QTE_REN, base, -0.4f, 0.0f, 4.3f, 0);
    EVT_AT_SetAngY(REN_QTE_REN, base, 0.0f, 0);
    frames = EVT_AT_SetMot(REN_QTE_REN, 7816, 0.0f, 1.0f, 0x40000000, 0);
    if (work->entry->successFrames)
        work->timer = work->entry->successFrames;
    else
        work->timer = frames + 30;
    if (work->timer > frames) {
        EVT_AT_SetAngY(REN_QTE_PLAYER, base, 270.0f, frames);
        EVT_AT_SetMot(REN_QTE_PLAYER, 7835, 1.0f, 1.0f, 0x40000000, frames);
        EVT_AT_SetAngY(REN_QTE_REN, base, 270.0f, frames);
        EVT_AT_SetMot(REN_QTE_REN, 7819, 1.0f, 1.0f, 0x40000000, frames);
    }
    work->state = REN_QTE_FINISH;
}

void TurnRenQteRightFailure(RenQteWork *work)
{
    shen_name base = work->base;
    int frames;
    EVT_AT_SetPos(REN_QTE_PLAYER, base, 0.408025f, 0.0f, 4.4f, 0);
    EVT_AT_SetAngY(REN_QTE_PLAYER, base, 0.0f, 0);
    frames = EVT_AT_SetMot(REN_QTE_PLAYER, 7830, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetPos(REN_QTE_REN, base, -0.408025f, 0.0f, 4.2f, 0);
    EVT_AT_SetAngY(REN_QTE_REN, base, 0.0f, 0);
    EVT_AT_SetMot(REN_QTE_REN, 7814, 0.0f, 1.0f, 0x40000000, 0);
    if (work->entry->failureFrames)
        work->timer = work->entry->failureFrames;
    else
        work->timer = frames + 30;
    if (work->timer > frames) {
        EVT_AT_SetAngY(REN_QTE_PLAYER, base, 270.0f, frames);
        EVT_AT_SetMot(REN_QTE_PLAYER, 7835, 9.0f, 1.0f, 0, frames);
        EVT_AT_SetAngY(REN_QTE_REN, base, 270.0f, frames);
        EVT_AT_SetMot(REN_QTE_REN, 7819, 1.0f, 1.0f, 0, frames);
    }
    work->state = REN_QTE_FINISH;
}

void ContinueRenQteRunSuccess(RenQteWork *work)
{
    shen_name base = work->base;
    int frames;
    EVT_AT_SetPos(REN_QTE_PLAYER, base, 0.4f, 0.0f, 4.3f, 0);
    EVT_AT_SetAngY(REN_QTE_PLAYER, base, 0.0f, 0);
    frames = EVT_AT_SetMot(REN_QTE_PLAYER, 7835, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetPos(REN_QTE_REN, base, -0.4f, 0.0f, 4.3f, 0);
    EVT_AT_SetAngY(REN_QTE_REN, base, 0.0f, 0);
    EVT_AT_SetMot(REN_QTE_REN, 7819, 0.0f, 1.0f, 0x40000000, 0);
    work->timer = frames + 30;
    work->state = REN_QTE_FINISH;
}

void ContinueRenQteRunFailure(RenQteWork *work)
{
    shen_name base = work->base;
    int frames;
    EVT_AT_SetPos(REN_QTE_PLAYER, base, 0.4f, 0.0f, 3.6f, 0);
    EVT_AT_SetAngY(REN_QTE_PLAYER, base, 0.0f, 0);
    frames = EVT_AT_SetMot(REN_QTE_PLAYER, 7830, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetMot(REN_QTE_PLAYER, 7835, 0.0f, 1.0f, 0, frames);
    EVT_AT_SetPos(REN_QTE_REN, base, -0.4f, 0.0f, 3.6f, 0);
    EVT_AT_SetAngY(REN_QTE_REN, base, 0.0f, 0);
    EVT_AT_SetMot(REN_QTE_REN, 7814, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetMot(REN_QTE_REN, 7819, 0.0f, 1.0f, 0, frames);
    work->timer = frames + 30;
    work->state = REN_QTE_FINISH;
}

void JumpRenQtePitSuccess(RenQteWork *work)
{
    shen_name base = work->base;
    int frames;
    EVT_AT_SetPos(REN_QTE_PLAYER, base, 0.4f, 0.0f, 4.3f, 0);
    EVT_AT_SetAngY(REN_QTE_PLAYER, base, 0.0f, 0);
    frames = EVT_AT_SetMot(REN_QTE_PLAYER, 7833, 0.0f, 1.0f, 0x44000000, 0);
    EVT_AT_SetPos(REN_QTE_REN, base, -0.4f, 0.0f, 4.3f, 0);
    EVT_AT_SetAngY(REN_QTE_REN, base, 0.0f, 0);
    EVT_AT_SetMot(REN_QTE_REN, 7817, 0.0f, 1.0f, 0x44000000, 0);
    if (work->entry->successFrames)
        work->timer = work->entry->successFrames;
    else
        work->timer = frames + 30;
    if (work->timer > frames) {
        EVT_AT_SetMot(REN_QTE_PLAYER, 7835, 3.0f, 1.0f, 0, frames);
        EVT_AT_SetMot(REN_QTE_REN, 7819, 3.0f, 1.0f, 0, frames);
    }
    work->state = REN_QTE_FINISH;
}

void JumpRenQtePitFailure(RenQteWork *work)
{
    shen_name base = work->base;
    int first, frames;
    EVT_AT_SetPos(REN_QTE_PLAYER, base, 0.4f, 0.0f, 3.2f, 0);
    EVT_AT_SetAngY(REN_QTE_PLAYER, base, 0.0f, 0);
    first = EVT_AT_SetMot(REN_QTE_PLAYER, 7834, 0.0f, 1.3f, 0x40000000, 0);
    EVT_AT_SetPos(REN_QTE_PLAYER, base, 0.4f, -0.8f, -1.3f, first);
    EVT_AT_SetAngY(REN_QTE_PLAYER, base, 0.0f, first);
    frames = EVT_AT_SetMot(REN_QTE_PLAYER, 7826, 0.0f, 1.0f, 0x40000000, first);
    EVT_AT_SetPos(REN_QTE_REN, base, -0.4f, 0.0f, 3.2f, 0);
    EVT_AT_SetAngY(REN_QTE_REN, base, 0.0f, 0);
    EVT_AT_SetMot(REN_QTE_REN, 7818, 0.0f, 1.3f, 0x40000000, 0);
    EVT_AT_SetPos(REN_QTE_REN, base, -0.4f, -0.8f, -1.3f, first);
    EVT_AT_SetAngY(REN_QTE_REN, base, 0.0f, first);
    EVT_AT_SetMot(REN_QTE_REN, 7810, 0.0f, 1.0f, 0x40000000, first);
    if (work->entry->failureFrames)
        work->timer = work->entry->failureFrames;
    else
        work->timer = frames;
    if (work->timer > frames) {
        EVT_AT_SetPos(REN_QTE_PLAYER, REN_QTE_PLAYER, 0.0f, 0.8f, 0.0f, frames);
        EVT_AT_SetAngY(REN_QTE_PLAYER, REN_QTE_PLAYER, 0.0f, frames);
        EVT_AT_SetPos(REN_QTE_REN, REN_QTE_REN, 0.0f, 0.8f, 0.0f, frames);
        EVT_AT_SetAngY(REN_QTE_REN, REN_QTE_REN, 0.0f, frames);
    } else if (work->lives > 0) {
        EVT_AT_SetPos(REN_QTE_PLAYER, REN_QTE_PLAYER, 0.0f, 0.8f, 0.0f, work->timer);
        EVT_AT_SetAngY(REN_QTE_PLAYER, REN_QTE_PLAYER, 0.0f, work->timer);
        EVT_AT_SetPos(REN_QTE_REN, REN_QTE_REN, 0.0f, 0.8f, 0.0f, work->timer);
        EVT_AT_SetAngY(REN_QTE_REN, REN_QTE_REN, 0.0f, work->timer);
        EVT_AT_SetMot(REN_QTE_PLAYER, 7835, 9.0f, 1.0f, 0x40000000, work->timer);
        EVT_AT_SetMot(REN_QTE_REN, 7819, 1.0f, 1.0f, 0x40000000, work->timer);
    }
    work->state = REN_QTE_FINISH;
}

void DodgeRenQtePoleSuccess(RenQteWork *work)
{
    shen_name base = work->base;
    int frames;
    EVT_AT_SetPos(REN_QTE_PLAYER, base, 0.0f, 0.0f, 1.6f, 0);
    EVT_AT_SetAngY(REN_QTE_PLAYER, base, 0.0f, 0);
    frames = EVT_AT_SetMot(REN_QTE_PLAYER, 7836, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetPos(REN_QTE_REN, base, -0.4f, 0.0f, 1.6f, 0);
    EVT_AT_SetAngY(REN_QTE_REN, base, 0.0f, 0);
    EVT_AT_SetMot(REN_QTE_REN, 7820, 0.0f, 1.0f, 0x40000000, 0);
    if (work->entry->successFrames)
        work->timer = work->entry->successFrames;
    else
        work->timer = frames + 30;
    if (work->timer > frames) {
        EVT_AT_SetAngY(REN_QTE_PLAYER, base, 0.0f, frames);
        EVT_AT_SetMot(REN_QTE_PLAYER, 7835, 1.0f, 1.0f, 0, frames);
        EVT_AT_SetAngY(REN_QTE_REN, base, 0.0f, frames);
        EVT_AT_SetMot(REN_QTE_REN, 7819, 1.0f, 1.0f, 0, frames);
    }
    work->state = REN_QTE_FINISH;
}

void DodgeRenQtePoleFailure(RenQteWork *work)
{
    shen_name base = work->base;
    int frames;
    EVT_AT_SetPos(REN_QTE_PLAYER, base, 0.4f, 0.0f, 2.0f, 0);
    EVT_AT_SetAngY(REN_QTE_PLAYER, base, 0.0f, 0);
    EVT_AT_SetMot(REN_QTE_PLAYER, 7837, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetPos(REN_QTE_PLAYER, REN_QTE_PLAYER, -1.0f, 0.0f, 0.8f, 71);
    EVT_AT_SetAngY(REN_QTE_PLAYER, REN_QTE_PLAYER, 0.0f, 71);
    frames = EVT_AT_SetMot(REN_QTE_PLAYER, 7835, 5.0f, 1.0f, 0, 160);
    EVT_AT_SetPos(REN_QTE_REN, base, -0.4f, 0.0f, 2.0f, 0);
    EVT_AT_SetAngY(REN_QTE_REN, base, 0.0f, 0);
    EVT_AT_SetMot(REN_QTE_REN, 7821, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetPos(REN_QTE_REN, REN_QTE_REN, -1.1f, 0.0f, 0.3f, 71);
    EVT_AT_SetAngY(REN_QTE_REN, REN_QTE_REN, 0.0f, 71);
    EVT_AT_SetMot(REN_QTE_REN, 7819, 1.0f, 1.0f, 0, 160);
    if (work->entry->failureFrames)
        work->timer = work->entry->failureFrames;
    else
        work->timer = frames + 30;
    work->state = REN_QTE_FINISH;
}

void SwitchRenQteLeaderSuccess(RenQteWork *work)
{
    shen_name base = work->base;
    int frames;
    EVT_AT_SetPos(REN_QTE_PLAYER, base, 0.4f, 0.0f, 1.6f, 0);
    EVT_AT_SetAngY(REN_QTE_PLAYER, base, 0.0f, 0);
    frames = EVT_AT_SetMot(REN_QTE_PLAYER, 7827, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetPos(REN_QTE_REN, base, -0.2f, 0.0f, 1.2f, 0);
    EVT_AT_SetAngY(REN_QTE_REN, base, 0.0f, 0);
    EVT_AT_SetMot(REN_QTE_REN, 7812, 0.0f, 1.0f, 0x40000000, 0);
    if (work->entry->successFrames)
        work->timer = work->entry->successFrames;
    else
        work->timer = frames + 30;
    work->state = REN_QTE_FINISH;
}

void SwitchRenQteLeaderFailure(RenQteWork *work)
{
    shen_name base = work->base;
    int frames;
    EVT_AT_SetPos(REN_QTE_PLAYER, base, 0.45000002f, 0.0f, 1.5f, 0);
    EVT_AT_SetAngY(REN_QTE_PLAYER, base, 0.0f, 0);
    frames = EVT_AT_SetMot(REN_QTE_PLAYER, 7828, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetPos(REN_QTE_REN, base, -0.15f, 0.0f, 1.5f, 0);
    EVT_AT_SetAngY(REN_QTE_REN, base, 0.0f, 0);
    EVT_AT_SetMot(REN_QTE_REN, 7811, 0.0f, 1.0f, 0x40000000, 0);
    if (work->entry->failureFrames)
        work->timer = work->entry->failureFrames;
    else
        work->timer = frames + 30;
    if (work->timer > frames) {
        EVT_AT_SetMot(REN_QTE_PLAYER, 7835, 9.0f, 1.0f, 0, frames);
        EVT_AT_SetMot(REN_QTE_REN, 7819, 1.0f, 1.0f, 0, frames);
    }
    work->state = REN_QTE_FINISH;
}

void BreakRenQteDoorSuccess(RenQteWork *work)
{
    shen_name base = work->base;
    int frames;
    EVT_AT_SetPos(REN_QTE_PLAYER, base, 0.4f, 0.0f, 1.7f, 0);
    EVT_AT_SetAngY(REN_QTE_PLAYER, base, 0.0f, 0);
    EVT_AT_SetMot(REN_QTE_PLAYER, 7838, 20.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetPos(REN_QTE_PLAYER, base, 0.4f, 0.0f, 1.7f, 16);
    EVT_AT_SetAngY(REN_QTE_PLAYER, base, 0.0f, 16);
    EVT_AT_SetMot(REN_QTE_PLAYER, 7838, 20.0f, 1.0f, 0x40000000, 16);
    EVT_AT_SetPos(REN_QTE_PLAYER, base, 0.4f, 0.0f, 1.7f, 31);
    EVT_AT_SetAngY(REN_QTE_PLAYER, base, 0.0f, 31);
    EVT_AT_SetMot(REN_QTE_PLAYER, 7838, 20.0f, 1.0f, 0x40000000, 31);
    frames = EVT_AT_SetMot(REN_QTE_PLAYER, 7838, 85.0f, 1.0f, 0x40000000, 51);
    FTBL.SetupHandPoseScript(FTBL.ctrlGetCharaByName(REN_QTE_PLAYER));
    EVT_AT_SetPos(REN_QTE_REN, base, -0.4f, 0.0f, 1.75f, 0);
    EVT_AT_SetAngY(REN_QTE_REN, base, 0.0f, 0);
    EVT_AT_SetMot(REN_QTE_REN, 7822, 20.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetPos(REN_QTE_REN, base, -0.4f, 0.0f, 1.75f, 16);
    EVT_AT_SetAngY(REN_QTE_REN, base, 0.0f, 16);
    EVT_AT_SetMot(REN_QTE_REN, 7822, 20.0f, 1.0f, 0x40000000, 16);
    EVT_AT_SetPos(REN_QTE_REN, base, -0.4f, 0.0f, 1.75f, 31);
    EVT_AT_SetAngY(REN_QTE_REN, base, 0.0f, 31);
    EVT_AT_SetMot(REN_QTE_REN, 7822, 20.0f, 1.0f, 0x40000000, 31);
    EVT_AT_SetMot(REN_QTE_REN, 7822, 85.0f, 1.0f, 0x40000000, 51);
    QueueRenQteCrushDoor(100, 0, 30);
    work->state = REN_QTE_FINISH;
    work->timer = frames - 75;
    EVT_AT_MenCtrl(20, 0, 0, 0, 0, 255, 0, 0, 0, frames - 95);
}

void QueueRenQteCrushDoor(int lastFrame, int restoreIntactDoor, int delay)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(RenQteCrushDoorCallback, 5, 11, sizeof(RenQteCrushDoorWork), REN_QTE_DOOR_TASK);
    RenQteCrushDoorWork *work = (RenQteCrushDoorWork *)FTBL.HLib_GetTaskParameter(task);
    work->wait = 13;
    work->frame = 1;
    work->lastFrame = lastFrame;
    work->restoreIntactDoor = restoreIntactDoor;
    if (delay)
        FTBL.HLib_SetupTimedTask(task, delay);
}

void RenQteCrushDoorCallback(void *data)
{
    RenQteCrushDoorWork *work = (RenQteCrushDoorWork *)data;
    Vector3 position;
    Vector3i rotation;
    int part;
    float frame;
    if (work->wait) {
        work->wait--;
        return;
    }
    FTBL.EV_DisableCharacterDisplay(REN_QTE_DOOR);
    FTBL.EV_EnableCharacterDisplay(REN_QTE_BROKEN_DOOR);
    frame = work->frame * 2.0f;
    for (part = 0; part < REN_QTE_DOOR_PARTS; part++) {
        FTBL.dynaReadEvaluate(part, frame, &position, &rotation, REN_QTE_DOOR_DYNAMICS);
        position.x *= 10.0f;
        position.y *= 10.0f;
        position.z *= 10.0f;
        FTBL.scenApplyPosOrRotToChildId(REN_QTE_BROKEN_DOOR, renQteDoorParts[part], &position, &rotation, 0);
    }
    if (++work->frame > work->lastFrame) {
        if (work->restoreIntactDoor) {
            FTBL.EV_EnableCharacterDisplay(REN_QTE_DOOR);
            FTBL.EV_DisableCharacterDisplay(REN_QTE_BROKEN_DOOR);
        }
        FTBL.HLib_DestroyCurrentTask();
    }
}

void BreakRenQteDoorFailure(RenQteWork *work)
{
    shen_name base = work->base;
    int frames;
    EVT_AT_SetPos(REN_QTE_PLAYER, base, 0.4f, 0.0f, 2.1f, 0);
    EVT_AT_SetAngY(REN_QTE_PLAYER, base, 0.0f, 0);
    EVT_AT_SetMot(REN_QTE_PLAYER, 7829, 0.0f, 1.0f, 0x40000000, 0);
    FTBL.SetupHandPoseScript(FTBL.ctrlGetCharaByName(REN_QTE_PLAYER));
    EVT_AT_SetPos(REN_QTE_REN, base, -0.4f, 0.0f, 2.1f, 0);
    EVT_AT_SetAngY(REN_QTE_REN, base, 0.0f, 0);
    EVT_AT_SetMot(REN_QTE_REN, 7813, 0.0f, 1.0f, 0x40000000, 0);
    if (work->lives > 0) {
        frames = EVT_AT_SetMot(REN_QTE_PLAYER, 7829, 200.0f, 1.0f, 0x40000000, 140);
        EVT_AT_SetMot(REN_QTE_REN, 7813, 200.0f, 1.0f, 0x40000000, 140);
        work->state = REN_QTE_START;
        work->timer = frames;
    } else
        work->timer = 140;
}

void RenQteUnusedResult(RenQteWork *work)
{
    FTBL.EventAssertMessageNullsub(__FILE__, 1034, "Illegal Function\n");
}

int renQteDoorParts[REN_QTE_DOOR_PARTS] = {152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171};
RenQteResultCallback renQteSuccessCallbacks[8] = {RenQteUnusedResult, TurnRenQteLeftSuccess, TurnRenQteRightSuccess, ContinueRenQteRunSuccess,
    JumpRenQtePitSuccess, DodgeRenQtePoleSuccess, SwitchRenQteLeaderSuccess, BreakRenQteDoorSuccess};
RenQteResultCallback renQteFailureCallbacks[8] = {RenQteUnusedResult, TurnRenQteLeftFailure, TurnRenQteRightFailure, ContinueRenQteRunFailure,
    JumpRenQtePitFailure, DodgeRenQtePoleFailure, SwitchRenQteLeaderFailure, BreakRenQteDoorFailure};
