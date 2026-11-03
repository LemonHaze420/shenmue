#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_dgct.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void RunDgct(void)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(GameCenterMain, 4, 11, sizeof(GameCenterWork), TASK_EVNT);
    InitializeGameCenterWork((GameCenterWork *)FTBL.HLib_GetTaskParameter(task), task);
}
#else
#define SET_VECTOR(v, a, b, c) do { (v).x = (a); (v).y = (b); (v).z = (c); } while (0)
void InitializeGameCenterWork(GameCenterWork *work, HLibTask *task)
{
    FTBL.SetPlayerControlEnabled(1);
    work->task = task;
    work->state = 1;
    work->game.state = 0;
    work->game.best = 30000;
    work->game.continued = 0;
}
void LeaveGameCenter(void)
{
    FTBL.SetPlayerControlEnabled(0);
    FTBL.HLib_SetupTaskDelay(60);
    FTBL.SetScreenFilmFade(60, 0, 0, 0, 0, 255, 0, 0, 0);
}
void ReturnFromGameCenter(void)
{
    FTBL.EV_scenQueueLoad(1, DGCT_RETURN, 50);
    FTBL.HLib_DestroyCurrentEventTask();
}
void GameCenterMain(void *data)
{
    GameCenterWork *work = (GameCenterWork *)data;
    switch (work->state) {
    case 1:
        if (IsGameCenterButtonPressed() == 1)
            work->state = 2;
        if (IsPlayerLeavingGameCenter() == 1)
            work->state = 11;
        FTBL.SetMinigameSeed(7);
        break;
    case 2:
        if (UpdateGameCenterQte(&work->game) == 1)
            work->state = 1;
        break;
    case 11:
        LeaveGameCenter();
        work->state = 12;
        break;
    case 12:
        ReturnFromGameCenter();
        break;
    }
}
int IsGameCenterButtonPressed(void)
{
    return FTBL.GetCurrentControlInput(7) == 0x400;
}
int IsPlayerLeavingGameCenter(void)
{
    Vector3 position;
    int leaving;
    FTBL.GetCharaPosition(EXQT_RYO, -1, &position, 0);
    if (position.z > 117.5f)
        leaving = 1;
    else
        leaving = 0;
    return leaving;
}
void InitializeGameCenterQte(ExciteState *state)
{
    Vector3 position;
    Vector3i rotation;
    rotation.x = 0;
    rotation.y = 0;
    rotation.z = 0;
    FTBL.GetCharaPosition(EXQT_MACHINE, -1, &position, 0);
    position.x += 0;
    position.y += 0;
    position.z += .718f;
    SHENMUE_EVENT_BOUNDARY();
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(EXQT_RYO), 71, 0, 0);
    FTBL.EV_AT_SetAngY_0(EXQT_RYO, 0x38000000, &rotation, 0);
    FTBL.EV_AT_SetPos(EXQT_RYO, 0x38000000, &position);
    FTBL.EV_SetCharaControllerFlags(EXQT_RYO, 2, 0);
    FTBL.EV_SetCharaControllerFlags(EXQT_RYO, 3, 0);
    SHENMUE_EVENT_BOUNDARY();
    state->score = 0;
    FTBL.SetPlayerControlEnabled(0);
    SHENMUE_EVENT_BOUNDARY();
    state->motion = FTBL.ReadMotiFromDirectory(1, DGCT_AREA, "0116E.MOT");
}
void ResetGameCenterGame(ExciteState *state)
{
    state->score = 0;
    state->successes = 0;
    state->cumulativeBonus = 0;
    state->timingBonus = 0;
    state->lives = 3;
    state->waitElapsed = 0;
    state->waitTarget = 40;
    state->cycle = 0;
    state->patternByte = 0;
    state->tier = 0;
    state->timeScale = 8;
    state->patternScale = 3;
    state->patternStage = 0;
    state->immediateAdvance = 0;
    ClearExciteInput(&state->input);
    InitializeExciteMachineParts();
    HideExciteMachineParts();
    ResetExciteMachineDisplay();
}
void ExitGameCenterQte(ExciteState *state)
{
    FTBL.DeleteMotion(state->motion);
    if (state->score >= state->best)
        state->best = state->score;
    state->continued = state->score;
    FTBL.SetPlayerControlEnabled(1);
    FTBL.EVS_LoopCamera(0, 0, 0);
    FTBL.camCtrlSetFovTransitionTo(1);
    FTBL.camCtrlSetState(0);
}
int UpdateGameCenterQte(ExciteState *state)
{
    UpdateGameCenterDisplay(state);
    switch (state->state) {
    case 0:
        InitializeGameCenterQte(state);
        ResetGameCenterGame(state);
        state->state = 1;
        return 0;
    case 1:
        StartGameCenterIntro();
        state->waitElapsed = 0;
        state->waitTarget = 200;
        state->state = 2;
        return 0;
    case 2:
        if (AdvanceGameCenterWait(state) == 1) {
            short choices[4] = {-1, 74, 75, -1};
            FTBL.SetupControlSpeakAndUI(choices, 300, 0xffff, 0);
            state->waitElapsed = 0;
            state->waitTarget = 40;
            state->state = 3;
            break;
        }
        return 0;
    case 3:
        if (FTBL.shenGetCtrlSpeakIconFlag() == 74) {
            state->state = 4;
            return 0;
        }
        if (FTBL.shenGetCtrlSpeakIconFlag() == 75) {
            state->state = 18;
            break;
        }
        if (!FTBL.GetShouldRenderIcons(2))
            return 0;
        state->state = 18;
        break;
    case 4:
        StartGameCenterPaidIntro();
        state->waitElapsed = 0;
        state->waitTarget = 205;
        state->state = 5;
        return 0;
    case 5:
        if (AdvanceGameCenterWait(state) == 1) {
            state->waitElapsed = 0;
            state->waitTarget = 40;
            state->state = 6;
            break;
        }
        return 0;
    case 6:
        HideExciteMachineParts();
        QueueExcitePart(170, 1, 0);
        QueueExcitePart(170, 0, 14);
        QueueExcitePart(170, 1, 28);
        QueueExcitePart(170, 0, 42);
        QueueExcitePart(170, 1, 56);
        QueueExcitePart(170, 0, 70);
        FTBL.EVS_LoopCamera(7090, EXQT_MACHINE, EXQT_MACHINE);
        state->waitElapsed = 0;
        state->waitTarget = 84;
        state->state = 7;
        return 0;
    case 7:
        if (AdvanceGameCenterWait(state) == 1) {
            SetExciteMachinePart(169, 1);
            SetExciteMachinePart(170, 1);
            SetExciteMachinePart(176, 1);
            UpdateExciteLives(state);
            state->waitElapsed = 0;
            state->waitTarget = 40;
            state->state = 8;
            break;
        }
        return 0;
    case 8:
        if (state->immediateAdvance == 1 || AdvanceGameCenterWait(state) == 1) {
            state->state = 10;
            return 0;
        }
        return 0;
    case 10:
        BuildGameCenterInput(state);
        state->state = 11;
        break;
    case 11:
        if (PollGameCenterInput(&state->input) == 1) {
            state->state = 12;
            break;
        }
        return 0;
    case 12:
        UpdateGameCenterTimingBonus(state);
        if (ConsumeGameCenterInput(&state->input) == 1) {
            ++state->successes;
            state->state = 14;
        } else {
            FTBL.EVS_LoopCamera(7090, EXQT_MACHINE, EXQT_MACHINE);
            state->state = 15;
        }
        break;
    case 14:
        AccumulateGameCenterBonus(state);
        UpdateGameCenterScore(state);
        state->state = 8;
        break;
    case 15:
        state->immediateAdvance = 0;
        if (DecrementGameCenterLives(state) == 1) {
            GameCenterLostLife(1);
            state->retryFrames = 0;
        } else {
            GameCenterLostLife(0);
            state->retryFrames = state->waitTarget ? state->waitTarget : 1;
        }
        state->waitElapsed = 0;
        state->waitTarget = 55;
        state->state = 16;
        return 0;
    case 16:
        if (AdvanceGameCenterWait(state) == 1) {
            if (state->retryFrames) {
                FTBL.EVS_LoopCamera(7090, EXQT_MACHINE, EXQT_MACHINE);
                state->waitTarget = 60;
                state->state = 8;
            } else if (state->score >= state->best)
                state->state = 19;
            else
                state->state = 21;
            break;
        }
        return 0;
    case 19:
        ExciteHighScoreReaction();
        state->waitElapsed = 0;
        state->waitTarget = 85;
        state->state = 20;
        return 0;
    case 20:
        if (AdvanceGameCenterWait(state) == 1) {
            state->state = 21;
            break;
        }
        return 0;
    case 17:
        if (AdvanceGameCenterWait(state) == 1)
            state->state = 11;
        break;
    case 21:
        HideExciteMachineParts();
        SetExciteMachinePart(166, 1);
        QueueExciteResultDisplay(state);
        FTBL.EVS_LoopCamera(7090, EXQT_MACHINE, EXQT_MACHINE);
        state->waitElapsed = 0;
        state->waitTarget = 84;
        state->state = 22;
        break;
    case 22:
        if (AdvanceGameCenterWait(state) == 1)
            state->state = 23;
        break;
    case 23:
        FinishGameCenterQte();
        state->waitElapsed = 0;
        state->waitTarget = 100;
        state->state = 24;
        break;
    case 24:
        if (AdvanceGameCenterWait(state) == 1)
            state->state = 18;
        break;
    case 18:
        ExitGameCenterQte(state);
        state->state = 0;
        return 1;
    }
    return 0;
}
void BuildGameCenterInput(ExciteState *state)
{
    unsigned int buttons[8] = {256, 512, 1024, 2048, 4, 64, 16, 1};
    float index;
    float scaled;
    SetExciteTiming(state);
    index = FTBL.IncrementMinigameRandFloat() * state->patternScale;
    if (state->tier == 1)
        index += 4.0f;
    SetExciteRequiredInput(&state->input, buttons[(signed char)index]);
    scaled = FTBL.IncrementMinigameRandFloat() * state->timeScale;
    state->immediateAdvance = (unsigned int)(state->timeScale - 1.0f) == (unsigned int)scaled;
    ++state->input.attempts;
}
int DecrementGameCenterLives(ExciteState *state)
{
    if (state->lives > 1) {
        --state->lives;
        UpdateExciteLives(state);
        return 0;
    }
    return 1;
}
void SetExciteTiming(ExciteState *state)
{
    switch (state->tier) {
    case 0: SetExciteWindow(state, 3.0f, 8.0f); break;
    case 1: SetExciteWindow(state, 4.0f, 8.0f); break;
    case 2: SetExciteWindow(state, 6.0f, 8.0f); break;
    case 3: SetExciteWindow(state, 8.0f, 6.0f); break;
    case 4: SetExciteWindow(state, 8.0f, 5.0f); break;
    case 5: SetExciteWindow(state, 8.0f, 4.0f); break;
    case 6: SetExciteWindow(state, 8.0f, 3.0f); break;
    case 7: SetExciteWindow(state, 8.0f, 2.0f); break;
    default: SetExciteWindow(state, 8.0f, 1.0f); break;
    }
}
void SetExciteWindow(ExciteState *state, float patternScale, float timeScale)
{
    int window, threshold;
    state->timeScale = timeScale;
    state->patternScale = patternScale;
    switch (state->tier) {
    case 0: window = 40 - state->input.attempts / 4; threshold = 30; break;
    case 1: window = 35 - state->input.attempts / 4; threshold = 30; break;
    case 2: window = 30 - state->input.attempts / 4; threshold = 30; break;
    case 3: window = 25 - state->input.attempts / 9; threshold = 50; break;
    case 4: window = 23 - state->input.attempts / 10; threshold = 50; break;
    case 5: window = 21 - state->input.attempts / 10; threshold = 50; break;
    case 6: window = 19 - state->input.attempts / 10; threshold = 50; break;
    case 7:
    default: window = 17 - state->input.attempts / 10; threshold = 50; break;
    }
    state->input.window = window < 1 ? 1 : window;
    state->waitTarget = window < 1 ? 1 : window;
    if (state->input.attempts >= threshold) {
        state->input.attempts = -1;
        ++state->tier;
    }
}
void UpdateGameCenterScore(ExciteState *state)
{
    int score = state->successes * 1000 + (int)state->cumulativeBonus;
    state->score = score;
    if (score > 99999000)
        state->score = 99999000;
}
void UpdateGameCenterTimingBonus(ExciteState *state)
{
    int remaining = state->input.window - state->input.elapsed;
    if (remaining > 0) {
        float bonus = (float)(100 * (remaining + 2) / state->input.window);
        if (bonus >= 100.0f)
            state->timingBonus = 100.0f;
        else
            state->timingBonus = bonus;
        state->cumulativeBonus += state->timingBonus;
    } else
        state->timingBonus = 0.0f;
}
void AccumulateGameCenterBonus(ExciteState *state)
{
    state->cumulativeBonus += state->timingBonus;
}
void ResetExciteMachineDisplay(void)
{
    HideExciteMachineParts();
    SetExciteMachinePart(162, 1);
    SetExciteMachinePart(163, 1);
    SetExciteMachinePart(164, 1);
    SetExciteMachinePart(165, 1);
}
void HideExciteMachineParts(void)
{
    unsigned short i;
    Vector3 position;
    for (i = 0; i < 15; i++) {
        FTBL.scenApplyPosOrRotToChildId(EXQT_MACHINE, i + 162, &position, 0, 2);
        position.z = 0.0f;
        FTBL.scenApplyPosOrRotToChildId(EXQT_MACHINE, i + 162, &position, 0, 0);
    }
    for (i = 0; i < 6; i++) {
        FTBL.scenApplyPosOrRotToChildId(EXQT_MACHINE, i + 182, &position, 0, 2);
        position.z = 0.0f;
        FTBL.scenApplyPosOrRotToChildId(EXQT_MACHINE, i + 182, &position, 0, 0);
    }
}
void SetExciteMachinePart(int part, int enabled)
{
    Vector3 position;
    FTBL.scenApplyPosOrRotToChildId(EXQT_MACHINE, part, &position, 0, 2);
    if (enabled == 1)
        position.z = 0.015f;
    else
        position.z = 0.0f;
    FTBL.scenApplyPosOrRotToChildId(EXQT_MACHINE, part, &position, 0, 0);
}
void UpdateExciteLives(ExciteState *state)
{
    SetExciteMachinePart(175, 0);
    SetExciteMachinePart(174, 0);
    SetExciteMachinePart(167, 0);
    if (state->lives == 3)
        SetExciteMachinePart(175, 1);
    if (state->lives >= 2)
        SetExciteMachinePart(174, 1);
    if (state->lives)
        SetExciteMachinePart(167, 1);
}
void UpdateGameCenterDisplay(ExciteState *state)
{
    FTBL.scenCopyCharaToGame(EXQT_MACHINE);
    switch (state->state) {
    case 0:
        break;
    case 6:
        QueueExciteScoreFlash(state, 0, 0, 14);
        QueueExciteScoreFlash(state, 0, 28, 14);
        QueueExciteScoreFlash(state, 0, 56, 14);
        QueueExciteScoreFlash(state, 0, 84, 14);
        break;
    case 8:
        DrawExciteScore(state, 0);
        DrawExciteScore(state, 1);
        DrawExciteScore(state, 2);
        if (state->waitTarget != 60 || (short)state->timingBonus == 0) {
            DrawGameCenterWindow(state);
            DrawGameCenterBonus(state);
        }
        break;
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        DrawExciteScore(state, 0);
        DrawExciteScore(state, 1);
        DrawExciteScore(state, 2);
        DrawGameCenterWindow(state);
        break;
    case 15:
        DrawExciteScore(state, 0);
        DrawExciteScore(state, 1);
        DrawExciteScore(state, 2);
        break;
    case 21:
        if (state->score >= state->best) {
            QueueExciteScoreFlash(state, 0, 1, 14);
            QueueExciteScoreFlash(state, 0, 28, 14);
            QueueExciteScoreFlash(state, 0, 56, 14);
            QueueExciteScoreFlash(state, 0, 84, 14);
        } else {
            QueueExciteScoreFlash(state, 1, 1, 14);
            QueueExciteScoreFlash(state, 1, 28, 14);
            QueueExciteScoreFlash(state, 1, 56, 14);
            QueueExciteScoreFlash(state, 1, 84, 14);
        }
        break;
    case 24:
        break;
    }
}
void DrawGameCenterWindow(ExciteState *state)
{
    unsigned short part = 177;
    Vector3 position;
    int count;
    short i;
    switch (state->patternStage) {
    case 0:
    case 3:
    case 6:
        part = 177;
        break;
    case 1:
    case 4:
    case 7:
        part = 180;
        break;
    case 2:
    case 8:
        part = 179;
        break;
    case 5:
    case 9:
        part = 178;
        break;
    }
    SET_VECTOR(position, -0.13f, 1.38f, 0.015f);
    count = 30 * (state->input.window - state->input.elapsed) / state->input.window;
    for (i = (short)count > 0 ? (short)count : 0; i > 0 && i <= 30; i--) {
        FTBL.scenGetCharaGameNode(EXQT_MACHINE, part, &position, 1.0f, 0);
        position.x += 0.007f;
    }
}
void DrawGameCenterBonus(ExciteState *state)
{
    Vector3 step;
    Vector3 scale;
    Vector3 bonusScale;
    float x;
    Vector3 position;
    SET_VECTOR(step, -0.015f, 0.0f, 0.0f);
    SET_VECTOR(scale, 1.414f, 1.414f, 1.414f);
    SET_VECTOR(bonusScale, 0.75f, 0.75f, 0.75f);
    x = (float)(state->input.window - state->input.elapsed) * 30.0f / (float)state->input.window * 0.007f + 0.02f + -0.13f;
    SET_VECTOR(position, x, 1.38f, 0.015f);
    if ((short)state->timingBonus)
        FTBL.scenGetCharaGameNode(EXQT_MACHINE, 181, &position, 1.0f, &bonusScale);
    if (state->timingBonus == 100.0f)
        SET_VECTOR(position, x + 0.055f, 1.38f, 0.015f);
    else if (state->timingBonus >= 10.0f)
        SET_VECTOR(position, x + 0.04f, 1.38f, 0.015f);
    else
        SET_VECTOR(position, x + 0.025f, 1.38f, 0.015f);
    FTBL.SetPosRotForGame(EXQT_MACHINE, 256, &step, 100, 0);
    FTBL.UpdateGameNodeFlags(EXQT_MACHINE, (short)state->timingBonus, &position, 1.0f, &scale);
}
void InitializeExciteMachineParts(void)
{
    short i;
    Vector3 positions[13] = {{0.0f, 1.44f, 0.0f}, {-0.083418f, 1.55f, 0.0f}, {0.004f, 1.602f, 0.0f}, {0.083029f, 1.55f, 0.0f}, {0.0f, 1.335f, 0.0f},
        {0.0f, 1.335f, 0.0f}, {0.0f, 1.335f, 0.0f}, {0.0f, 1.335f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f}};
    int parts[14] = {166, 169, 170, 176, 171, 172, 173, 182, 183, 184, 185, 186, 187, 0};
    float scales[13] = {1.0f, 1.3f, 1.3f, 1.3f, 1.0f, 1.0f, 1.0f, 1.0f, 0.53f, 0.53f, 0.53f, 0.53f, 0.53f};
    Vector3 scale;
    for (i = 0; parts[i] != 0; i++) {
        scale.x = scale.y = scales[i];
        scale.z = 1.0f;
        FTBL.scenApplyPosOrRotToChildId(EXQT_MACHINE, parts[i], &positions[i], 0, 0);
        FTBL.scenApplyPosOrRotToChildId(EXQT_MACHINE, parts[i], &scale, 0, 3);
    }
}
int AdvanceGameCenterWait(ExciteState *state)
{
    if (state->waitElapsed >= state->waitTarget) {
        state->waitElapsed = 0;
        return 1;
    }
    state->waitElapsed++;
    return 0;
}
void StartGameCenterIntro(void)
{
    EVT_AT_SetCam(7000, EXQT_MACHINE, EXQT_MACHINE, 0, 0, -1);
    EVT_AT_SetCam(7010, EXQT_MACHINE, EXQT_MACHINE, 0, 0, 100);
}
void StartGameCenterPaidIntro(void)
{
    EVT_AT_SetCam(7020, EXQT_MACHINE, EXQT_MACHINE, 0, 0, -1);
    EVT_AT_SetCam(7030, EXQT_MACHINE, EXQT_MACHINE, 0, 0, 125);
    EVT_AT_SetCam(7040, EXQT_MACHINE, EXQT_MACHINE, 0, 0, 155);
    EVT_AT_SetMot(EXQT_RYO, 5249, 0.0f, 1.0f, 0x80000000, -1);
    EVT_AT_SetMot(EXQT_RYO, 5251, 0.0f, 1.0f, 0x80000000, 125);
    EVT_AT_StartMoneyCount(-100, 10);
}
void GameCenterLostLife(int final)
{
    if (final) {
        EVT_AT_SetCam(7055, EXQT_MACHINE, EXQT_MACHINE, 0, 0, -1);
        EVT_AT_SetMot(EXQT_RYO, 5252, 0.0f, 1.0f, 0x80000000, -1);
    } else
        EVT_AT_SetCam(7050, EXQT_MACHINE, EXQT_MACHINE, 0, 0, -1);
}
void QueueExciteResultDisplay(ExciteState *state)
{
    if (state->score >= state->best) {
        QueueExcitePart(170, 1, 0);
        QueueExcitePart(170, 0, 14);
        QueueExcitePart(170, 1, 28);
        QueueExcitePart(170, 0, 42);
        QueueExcitePart(170, 1, 56);
        QueueExcitePart(170, 0, 70);
        QueueExcitePart(170, 1, 84);
    } else {
        QueueExcitePart(169, 1, 0);
        QueueExcitePart(169, 0, 14);
        QueueExcitePart(169, 1, 28);
        QueueExcitePart(169, 0, 42);
        QueueExcitePart(169, 1, 56);
        QueueExcitePart(169, 0, 70);
        QueueExcitePart(169, 1, 84);
    }
}
void ExciteHighScoreReaction(void)
{
    EVT_AT_SetCam(7060, EXQT_MACHINE, EXQT_MACHINE, 0, 0, -1);
    EVT_AT_SetCam(7065, EXQT_MACHINE, EXQT_MACHINE, 0, 0, 30);
    EVT_AT_SetMot(EXQT_RYO, 5250, 0.0f, 1.0f, 0x80000000, -1);
}
void FinishGameCenterQte(void)
{
    EVT_AT_SetCam(7070, EXQT_MACHINE, EXQT_MACHINE, 0, 0, -1);
}
void QueueExcitePart(int part, int enabled, int delay)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(ExcitePartCallback, 6, 11, sizeof(ExcitePartWork), TASK_EVNT);
    ExcitePartWork *work;
    if (delay > 0)
        FTBL.HLib_SetupTimedTask(task, delay);
    work = (ExcitePartWork *)FTBL.HLib_GetTaskParameter(task);
    work->part = part;
    work->enabled = enabled;
}
void ExcitePartCallback(void *data)
{
    ExcitePartWork *work = (ExcitePartWork *)data;
    Vector3 position;
    FTBL.scenApplyPosOrRotToChildId(EXQT_MACHINE, work->part, &position, 0, 2);
    if (work->enabled == 1)
        position.z = 0.015f;
    else
        position.z = 0.0f;
    FTBL.scenApplyPosOrRotToChildId(EXQT_MACHINE, work->part, &position, 0, 0);
    FTBL.HLib_DestroyCurrentTask();
}
void ExciteScoreCallback(void *data)
{
    ExciteScoreWork *work = (ExciteScoreWork *)data;
    Vector3 step;
    Vector3 scale;
    SET_VECTOR(step, -0.015f, 0.0f, 0.0f);
    scale.x = 1.414f;
    scale.y = 1.414f;
    scale.z = 1.414f;
    if (work->ticks-- > 0) {
        FTBL.SetPosRotForGame(EXQT_MACHINE, 256, &step, 99999999, 0);
        FTBL.UpdateGameNodeFlags(EXQT_MACHINE, work->score, &work->position, 1.0f, &scale);
    } else
        FTBL.HLib_DestroyCurrentTask();
}
void QueueExciteScoreFlash(ExciteState *state, signed char which, int delay, int ticks)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(ExciteScoreCallback, 6, 11, sizeof(ExciteScoreWork), TASK_EVNT);
    ExciteScoreWork *work;
    if (delay > 0)
        FTBL.HLib_SetupTimedTask(task, delay);
    work = (ExciteScoreWork *)FTBL.HLib_GetTaskParameter(task);
    work->ticks = ticks;
    if (which == 0) {
        SET_VECTOR(work->position, 0.055f, 1.58f, 0.015f);
        work->score = state->best;
    } else if (which == 1) {
        SET_VECTOR(work->position, -0.032f, 1.528f, 0.015f);
        work->score = state->score;
    } else if (which == 2) {
        SET_VECTOR(work->position, 0.13f, 1.528f, 0.015f);
        work->score = state->continued;
    }
}
void DrawExciteScore(ExciteState *state, signed char which)
{
    Vector3 position;
    Vector3 step;
    Vector3 scale;
    int score;
    SET_VECTOR(step, -0.015f, 0.0f, 0.0f);
    SET_VECTOR(scale, 1.414f, 1.414f, 1.414f);
    score = 0;
    if (which == 0) {
        SET_VECTOR(position, 0.055f, 1.58f, 0.015f);
        if (state->score >= state->best)
            score = state->score;
        else
            score = state->best;
    } else if (which == 1) {
        SET_VECTOR(position, -0.032f, 1.528f, 0.015f);
        score = state->score;
    } else if (which == 2) {
        SET_VECTOR(position, 0.13f, 1.528f, 0.015f);
        score = state->continued;
    }
    FTBL.SetPosRotForGame(EXQT_MACHINE, 256, &step, 99999999, 0);
    FTBL.UpdateGameNodeFlags(EXQT_MACHINE, score, &position, 1.0f, &scale);
}
#endif
#else
#include "s2/areas/area_dgct.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/input.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/ui.hpp"

namespace shenmue::s2::areas {

void InitializeDgct() {
}
void RunDgct(std::uintptr_t) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(GameCenterMain, 4, 11, sizeof(GameCenterWork), EXQT_TASK);
    GameCenterWork &work = *static_cast<GameCenterWork *>(HLib::GetTaskParameter(task));
    SetPlayerControlEnabled(1);
    work.task = task;
    work.state = 1;
    work.game.game.state = 0;
    work.game.game.best = 30000;
    work.game.game.continued = 0;
}
void GameCenterMain(void *data) {
    GameCenterWork &work = *static_cast<GameCenterWork *>(data);
    switch (work.state) {
    case 1: {
        if (GetCurrentControlInput(INPUT_NEW_PRESSES) == PAD_BUTTON_X)
            work.state = 2;
        Vector3 position;
        EV_GetCharaPosition(EXQT_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        if (position.z > 117.5f)
            work.state = 11;
        SetSharedPrngSeed(7);
        break;
    }
    case 2:
        if (UpdateGameCenterQte(work.game))
            work.state = 1;
        break;
    case 11:
        SetPlayerControlEnabled(0);
        HLib::SetupTaskDelay(60);
        SetScreenFilmFade(60, 0, 0, 0, 0, 255, 0, 0, 0);
        work.state = 12;
        break;
    case 12:
        EV_scenQueueLoad(1, DGCT_RETURN, 50);
        HLib::DestroyCurrentTask();
        break;
    default:
        break;
    }
}
void InitializeGameCenterQte(ExciteState &state) {
    Vector3 position;
    EV_GetCharaPosition(EXQT_MACHINE, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    position.x += 0;
    position.y += 0;
    position.z += .718f;
    mtwkSetRegionFlag(scenFindMotionwork(EXQT_RYO), 71, 0, MotionRegion::LowerBody);
    EV_AT_SetAngY(EXQT_RYO, EventTransformFlags::AllAxes, {0, 0, 0}, nullptr);
    EV_AT_SetPos(EXQT_RYO, EventTransformFlags::AllAxes, position);
    EV_SetCharaControllerFlags(EXQT_RYO, 2, static_cast<NpcCharacterControllerState>(0));
    EV_SetCharaControllerFlags(EXQT_RYO, 3, static_cast<NpcCharacterControllerState>(0));
    state.game.score = 0;
    SetPlayerControlEnabled(0);
    state.game.motion = ReadMotiFromDirectory(1, DGCT_AREA, "0116E.MOT");
}
bool UpdateGameCenterQte(ExciteState &state) {
    using enum CameraState;
    ExciteGameState &game = state.game;
    ExciteInput &input = state.controls.arcade.input;
    UpdateGameCenterDisplay(state);
    switch (game.state) {
    case 0:
        InitializeGameCenterQte(state);
        state.lives = 3;
        game.score = 0;
        game.successes = 0;
        game.cumulativeBonus = 0;
        game.timingBonus = 0;
        game.waitElapsed = 0;
        state.cycle = 0;
        state.patternByte = 0;
        state.patternStage = 0;
        state.immediateAdvance = 0;
        game.waitTarget = 40;
        state.patternScale = 3;
        state.tier = 0;
        state.timeScale = 8;
        input.required = 0;
        input.pressed = 0;
        input.window = 0;
        input.elapsed = 0;
        input.attempts = 0;
        InitializeExciteMachineParts();
        HideExciteMachineParts();
        ResetExciteMachineDisplay();
        game.state = 1;
        return false;
    case 1:
        StartGameCenterIntro();
        game.waitTarget = 200;
        game.waitElapsed = 0;
        game.state = 2;
        return false;
    case 2: {
        if (game.waitElapsed < game.waitTarget)
            break;
        std::int16_t choices[]{-1, 74, 75, -1};
        game.waitElapsed = 0;
        SetupControlSpeakAndUI(choices, 300, -1, 0);
        game.waitElapsed = 0;
        game.waitTarget = 40;
        game.state = 3;
        return false;
    }
    case 3:
        if (shenGetCtrlSpeakIconFlag() == 74)
            game.state = 4;
        else if (shenGetCtrlSpeakIconFlag() == 75 || GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            game.state = 18;
        return false;
    case 4:
        StartGameCenterPaidIntro();
        game.waitTarget = 205;
        game.waitElapsed = 0;
        game.state = 5;
        return false;
    case 5:
        if (game.waitElapsed < game.waitTarget)
            break;
        game.waitTarget = 40;
        game.waitElapsed = 0;
        game.state = 6;
        return false;
    case 6:
        HideExciteMachineParts();
        for (std::int32_t i = 0; i < 6; ++i)
            QueueExcitePart(170, (i & 1) == 0, 14 * i);
        EVS_LoopCamera(7090, EXQT_MACHINE, EXQT_MACHINE);
        game.waitTarget = 84;
        game.waitElapsed = 0;
        game.state = 7;
        return false;
    case 7:
        if (game.waitElapsed < game.waitTarget)
            break;
        game.waitElapsed = 0;
        SetExciteMachinePart(169, 1);
        SetExciteMachinePart(170, 1);
        SetExciteMachinePart(176, 1);
        UpdateExciteLives(state);
        game.waitElapsed = 0;
        game.waitTarget = 40;
        game.state = 8;
        return false;
    case 8:
        if (state.immediateAdvance == 1)
            game.state = 10;
        else {
            if (game.waitElapsed < game.waitTarget)
                break;
            game.state = 10;
            game.waitElapsed = 0;
        }
        return false;
    case 10:
        BuildGameCenterInput(state);
        game.state = 11;
        return false;
    case 11:
        if (PollGameCenterInput(input))
            game.state = 12;
        return false;
    case 12:
        if (static_cast<std::int32_t>(input.window - input.elapsed) <= 0)
            game.timingBonus = 0;
        else {
            const float bonus = static_cast<float>(100 * (input.window - input.elapsed + 2) / input.window);
            game.timingBonus = bonus < 100 ? bonus : 100;
            game.cumulativeBonus += game.timingBonus;
        }
        if (ConsumeGameCenterInput(input)) {
            ++game.successes;
            game.state = 14;
        } else {
            EVS_LoopCamera(7090, EXQT_MACHINE, EXQT_MACHINE);
            game.state = 15;
        }
        return false;
    case 14: {
        const std::int32_t base = 1000 * game.successes;
        game.cumulativeBonus += game.timingBonus;
        const std::int32_t score = static_cast<std::int32_t>(game.cumulativeBonus + base);
        game.score = score > 99999000 ? 99999000 : score;
        game.state = 8;
        return false;
    }
    case 15:
        state.immediateAdvance = 0;
        if (state.lives <= 1) {
            GameCenterLostLife(true);
            state.retryFrames = 0;
        } else {
            --state.lives;
            UpdateExciteLives(state);
            GameCenterLostLife(false);
            state.retryFrames = game.waitTarget ? game.waitTarget : 1;
        }
        game.waitElapsed = 0;
        game.waitTarget = 55;
        game.state = 16;
        return false;
    case 16:
        if (game.waitElapsed < game.waitTarget)
            break;
        game.waitElapsed = 0;
        if (state.retryFrames) {
            EVS_LoopCamera(7090, EXQT_MACHINE, EXQT_MACHINE);
            game.waitTarget = 60;
            game.state = 8;
        } else
            game.state = game.score >= game.best ? 19 : 21;
        return false;
    case 17:
        if (game.waitElapsed < game.waitTarget)
            break;
        game.state = 11;
        game.waitElapsed = 0;
        return false;
    case 18:
        DeleteMotion(game.motion);
        if (game.score >= game.best)
            game.best = game.score;
        game.continued = game.score;
        SetPlayerControlEnabled(1);
        EVS_LoopCamera(0, shen_name{0}, shen_name{0});
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        game.state = 0;
        return true;
    case 19:
        ExciteHighScoreReaction();
        game.waitTarget = 85;
        game.waitElapsed = 0;
        game.state = 20;
        return false;
    case 20:
        if (game.waitElapsed < game.waitTarget)
            break;
        game.state = 21;
        game.waitElapsed = 0;
        return false;
    case 21:
        HideExciteMachineParts();
        SetExciteMachinePart(166, 1);
        QueueExciteResultDisplay(state);
        EVS_LoopCamera(7090, EXQT_MACHINE, EXQT_MACHINE);
        game.waitTarget = 84;
        game.waitElapsed = 0;
        game.state = 22;
        return false;
    case 22:
        if (game.waitElapsed < game.waitTarget)
            break;
        game.state = 23;
        game.waitElapsed = 0;
        return false;
    case 23:
        FinishGameCenterQte();
        game.waitTarget = 100;
        game.waitElapsed = 0;
        game.state = 24;
        return false;
    case 24:
        if (game.waitElapsed < game.waitTarget)
            break;
        game.state = 18;
        game.waitElapsed = 0;
        return false;
    default:
        return false;
    }
    ++game.waitElapsed;
    return false;
}
const ScriptAreaDefinition dgctScriptDefinition{.areaId = DGCT_AREA, .field04 = 0, .init = InitializeDgct, .main = RunDgct};
} // namespace shenmue::s2::areas
#endif
