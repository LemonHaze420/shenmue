#if defined(SHENMUE_EVENT_OVERLAY) && SHENMUE_TARGET_S2_DC_JP_GAMEJAM
#include "s2/areas/area_smbl_dc.hpp"
void *memcpy(void *, const void *, unsigned int);
#define NAME(a, b, c, d) SHEN_NAME4(a, b, c, d)
#define RYO NAME('R', 'Y', 'O', '_')
#define DEALER NAME('S', 'H', 'I', '_')
#if defined(SHENMUE_EVENT_ENTRY)
void QueueSmallGame(void) {
    HLibTask *task;
    SmallGameWork *work;
    task = FTBL.HLib_EnqueueTaskWithParameter(SmallGameCallback, 4, 11, sizeof(SmallGameWork), TASK_EVNT);
    work = (SmallGameWork *)FTBL.HLib_GetTaskParameter(task);
    InitSmallGame(work, task);
}
#else
void InitializeSmallGame(int unused, SmallGameWork *work) {
    work->task = 0;
    QueueSmallGame();
}
void InitSmallGame(SmallGameWork *work, HLibTask *task) {
    Vector3 origin = {0, 0, 0}, offset = {0, .35f, 0};
    Vector3i zeroRotation = {0, 0, 0}, rotation = {49152, 32768, 16384};
    shen_name parents[] = {NAME('D', 'U', 'M', '0'), NAME('D', 'U', 'M', '1'), NAME('D', 'U', 'M', '2'), NAME('D', 'U', 'M', '3'), NAME('D', 'U', 'M', '4'),
        NAME('D', 'U', 'M', '5'), NAME('D', 'U', 'M', '6'), NAME('D', 'U', 'M', '7'), NAME('D', 'U', 'M', '8'), NAME('D', 'U', 'M', '9'), 0};
    shen_name boards[] = {NAME('B', 'D', 'B', '0'), NAME('B', 'D', 'B', '1'), NAME('B', 'D', 'B', '2'), NAME('B', 'D', 'B', '3'), NAME('B', 'D', 'B', '4'),
        NAME('B', 'D', 'B', '5'), NAME('B', 'D', 'B', '6'), NAME('B', 'D', 'B', '7'), NAME('B', 'D', 'B', '8'), NAME('B', 'D', 'B', '9'), 0};
    shen_name nails[] = {NAME('N', 'D', '_', '0'), NAME('N', 'D', '_', '1'), NAME('N', 'D', '_', '2'), NAME('N', 'D', '_', '3'), NAME('N', 'D', '_', '4'),
        NAME('N', 'D', '_', '5'), NAME('N', 'D', '_', '6'), NAME('N', 'D', '_', '7'), NAME('N', 'D', '_', '8'), NAME('N', 'D', '_', '9'), 0};
    unsigned short index = 0;
    unsigned char hasDuck;
    for (; parents[index]; index++) {
        FTBL.AT_SetLocalBaseChara(boards[index], parents[index]);
        FTBL.EV_AT_SetPos(boards[index], 0x38000000, &offset);
        FTBL.EV_AT_SetAngY_0(boards[index], 0x38000000, &rotation, 0);
        FTBL.AT_SetLocalBaseChara(nails[index], parents[index]);
        FTBL.EV_AT_SetPos(nails[index], 0x38000000, &origin);
        FTBL.EV_AT_SetAngY_0(nails[index], 0x38000000, &zeroRotation, 0);
    }
    FTBL.SetPlayerControlEnabled(1);
    hasDuck = FTBL.DoesScenarioCharacterExist(NAME('R', 'U', 'C', 'K'));
    work->task = task;
    work->state = 17;
    work->busy = 0;
    work->flag = 0;
    work->ball.boardType = 0;
    work->ball.pollState = 0;
    if (hasDuck)
        work->ball.hasDuck = 1;
    else
        work->ball.hasDuck = 0;
}
void FadeSmallGame(void) {
    FTBL.SetPlayerControlEnabled(0);
    FTBL.HLib_SetupTaskDelay(60);
    FTBL.SetScreenFilmFade(60, 0, 0, 0, 0, 255, 0, 0, 0);
}
void ExitSmallGame(void) {
    FTBL.HLib_DestroyCurrentEventTask();
    FTBL.EV_scenQueueLoad(1, NAME('W', 'S', '0', '0'), 50);
}
void SmallGameCallback(void *data) {
    SmallGameWork *work = (SmallGameWork *)data;
    int near;
    unsigned char busy, *busyField;
    switch (work->state) {
    case 1:
        near = IsNearSmallGame();
        if (near == 1 && !(busy = *(busyField = &work->busy))) {
            FTBL.SetPlayerControlEnabled(0);
            QueueSmallGameIntro();
            work->frames = busy;
            work->wait = 100;
            *busyField = near;
            work->state = 2;
        } else
            work->busy = 0;
        break;
    case 2:
        if (work->frames > work->wait) {
            short choices[] = {-1, 74, 75, -1};
            FTBL.SetupControlSpeakAndUI(choices, 300, 65535, 0);
            work->state = 3;
        } else
            work->frames++;
        break;
    case 3:
        if (FTBL.shenGetCtrlSpeakIconFlag() == 74) {
            FTBL.EVS_LoopCamera(910, 0, 0);
            work->state = 5;
        } else if (FTBL.shenGetCtrlSpeakIconFlag() == 75)
            work->state = 4;
        else if (FTBL.GetShouldRenderIcons(2))
            work->state = 4;
        break;
    case 4:
        FTBL.SetPlayerControlEnabled(1);
        FTBL.EVS_LoopCamera(0, 0, 0);
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        work->state = 1;
        break;
    case 5:
        if (ReadSmallGameInput(work) == 1)
            work->state = 6;
        if (IsLeavingSmallGame() == 1)
            work->state = 15;
        break;
    case 6:
        if (PollSmallBall(&work->ball) == 1)
            work->state = 5;
        break;
    case 15:
        FadeSmallGame();
        work->state = 16;
        break;
    case 16:
        ExitSmallGame();
        break;
    case 17:
        work->state = 18;
        break;
    case 18:
        FTBL.EV_NPC_NewLoadSw_0(0);
        FTBL.EV_ResetFreeTalkFlags();
        work->state = 5;
        break;
    }
}
int IsNearSmallGame(void) {
    Vector3 player, dealer;
    int result;
    FTBL.GetCharaPosition(RYO, -1, &player, 0);
    FTBL.GetCharaPosition(DEALER, -1, &dealer, 0);
    result = FTBL.ComputeAngle(&dealer, 2.0f, 0, 32768, &player);
    FTBL.ComputeAngle(&player, 3.0f, 0, 8192, &dealer);
    return result == 1;
}
int ReadSmallGameInput(SmallGameWork *work) {
    if (FTBL.GetCurrentControlInput(7) != 1024)
        return 0;
    else {
        work->ball.boardType = 0;
        work->ball.board = NAME('D', 'U', 'M', '5');
        work->ball.dealer = NAME('N', 'D', '_', '5');
        return 1;
    }
}

void QueueSmallGameIntro(void) {
    QueueEventMessage("おう兄ちゃん。遊ぶかい？", 0, 60);
    EVT_AT_SetMot(RYO, 61443, 0.0f, 1.0f, 0x80000000, 1);
    EVT_AT_SetCam(950, DEALER, DEALER, 0, 1);
    EVT_AT_SetPosAng(RYO, -1, 39.1055984497f, 113.193901062f, 134.96699524f, 1);
    EVT_AT_SetPosAng(DEALER, -1, 38.2695007324f, 114.125801086f, 315.010986328f, 1);
}
int IsLeavingSmallGame(void) {
    Vector3 player;
    FTBL.GetCharaPosition(RYO, -1, &player, 0);
    {
        int result;
        if (player.z > 117.5f)
            result = 1;
        else
            result = 0;
        return result;
    }
}

#endif
#endif
