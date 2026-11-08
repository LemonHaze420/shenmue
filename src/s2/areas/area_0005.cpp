#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_0005.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void e0005_EVT_Load_AuthChara_Enqueue(void *argument)
{
    Vector3 first = {138.21f, 100.0f, 311.32f};
    Vector3 second = {148.5f, 100.0f, 315.74f};
    Vector3 third = {156.03f, 100.0f, 315.76f};
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(e0005_EVT_Load_AuthChara_Callback, 4, 11, sizeof(Area0005EventWork), TASK_EVNT);
    Area0005EventWork *event = (Area0005EventWork *)FTBL.HLib_GetTaskParameter(task);
    event->task = task;
    event->authorState = 1;
    event->motion = FTBL.ReadMotiFromDirectory(g_actNumber, g_currentMap, "0005A.MOT");
    event->loadState = 0;
    event->characterIndex = 0;
    FTBL.SetRenderClockAndCompassTo(1);
    if (g_currentMap == AREA_0005) {
        FTBL.BeginPrimarySampleExecution(0);
        event->entryId = (unsigned int)g_entryPoint > 4 ? 0 : g_entryPoint;
        switch (event->entryId) {
        case 0:
            event->characterTable = 0;
            break;
        case 1:
        case 2:
            event->characterTable = 1;
            break;
        case 3:
        case 4:
            event->characterTable = 2;
            break;
        }
    } else {
        event->entryId = *(short *)argument;
        switch (event->entryId) {
        case 0:
            FTBL.FreeWAIR();
            FTBL.UpdateWeatherForMinute(0, 0, 0, 0.0f);
            FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
            FTBL.UpdateWeather();
            FTBL.EV_stepSetFlags(69, 0);
            event->characterTable = 0;
            break;
        case 1:
            event->characterTable = 1;
            break;
        case 3:
            event->characterTable = 2;
            break;
        }
    }
    switch (event->entryId) {
    case 3:
    case 4:
        if (FTBL.npcSetAreaName(SHEN_NAME4('A', 'N', 'P', '1'), &first, 2.0f, 1) == 1)
            FTBL.DebugLogToFileRemoved("NPC_AREA1 : TRUE\n");
        else
            FTBL.DebugLogToFileRemoved("NPC_AREA1 : FALSE\n");
        break;
    case 1:
    case 2:
        if (FTBL.npcSetAreaName(SHEN_NAME4('A', 'N', 'P', '2'), &second, 5.0f, 1) == 1)
            FTBL.DebugLogToFileRemoved("NPC_AREA2 : TRUE\n");
        else
            FTBL.DebugLogToFileRemoved("NPC_AREA2 : FALSE\n");
        if (FTBL.npcSetAreaName(SHEN_NAME4('A', 'N', 'P', '3'), &third, 6.0f, 1) == 1)
            FTBL.DebugLogToFileRemoved("NPC_AREA3 : TRUE\n");
        else
            FTBL.DebugLogToFileRemoved("NPC_AREA3 : FALSE\n");
        break;
    }
    InitializeEventEnvironment();
}
#else
static const shen_name authorCharacters0[] = {SHEN_NAME4('G', 'E', 'T', '_'), SHEN_NAME4('G', 'Y', 'E', '_'), SHEN_NAME4('K', 'I', 'Z', '_'),
    SHEN_NAME4('K', 'E', 'I', '_'), SHEN_NAME4('S', 'D', 'N', '_'), SHEN_NAME4('S', 'Z', 'Y', '_'), AREA_0005_INVALID};
static const shen_name authorCharacters1[] = {SHEN_NAME4('V', 'C', 'N', '_'), SHEN_NAME4('R', 'E', '1', '_'), SHEN_NAME4('C', 'H', 'W', '_'),
    AREA_0005_KYG, AREA_0005_SYM, SHEN_NAME4('S', 'H', 'K', '_'), AREA_0005_INVALID};
static const shen_name authorCharacters2[] = {SHEN_NAME4('K', 'A', 'P', '_'), SHEN_NAME4('R', 'F', 'K', '_'), SHEN_NAME4('S', 'D', 'N', '_'),
    SHEN_NAME4('S', 'Z', 'Y', '_'), AREA_0005_INVALID};
static const shen_name npcCharacters0[] = {SHEN_NAME4('S', 'U', 'U', '_'), SHEN_NAME4('E', 'N', 'N', '_'), SHEN_NAME4('G', 'N', 'B', '_'),
    SHEN_NAME4('M', '0', '2', 'Q'), SHEN_NAME4('M', '0', '3', 'Q'), SHEN_NAME4('M', '0', '5', 'Q'), AREA_0005_INVALID};
static const shen_name npcCharacters1[] = {SHEN_NAME4('G', 'N', 'B', '_'), SHEN_NAME4('R', 'R', 'I', '_'), SHEN_NAME4('C', 'K', 'T', '_'),
    SHEN_NAME4('V', 'C', 'N', '_'), SHEN_NAME4('0', '7', 'E', '_'), SHEN_NAME4('M', 'O', 'B', '1'), SHEN_NAME4('M', 'O', 'B', '2'),
    SHEN_NAME4('M', 'O', 'B', '3'), SHEN_NAME4('M', 'O', 'B', '5'), SHEN_NAME4('M', 'O', 'B', '8'), SHEN_NAME4('M', 'O', 'B', '9'),
    SHEN_NAME4('H', '0', '4', 'Q'), SHEN_NAME4('H', '0', '3', 'Q'), SHEN_NAME4('H', '1', '3', 'Q'), SHEN_NAME4('H', '1', '6', 'Q'),
    SHEN_NAME4('Y', '1', '0', 'Q'), SHEN_NAME4('Y', '1', '1', 'Q'), AREA_0005_INVALID};
static const shen_name npcCharacters2[] = {SHEN_NAME4('S', 'O', 'G', '_'), SHEN_NAME4('C', 'S', 'Y', '_'), SHEN_NAME4('0', '0', 'E', '_'),
    SHEN_NAME4('P', '0', '2', 'Q'), SHEN_NAME4('P', '0', '3', 'Q'), SHEN_NAME4('S', 'Y', 'R', '_'), AREA_0005_INVALID};
static int secondaryRegisteredCount = 0;
static const shen_name *authorCharacters[] = {authorCharacters0, authorCharacters1, authorCharacters2, 0};
static const shen_name *npcCharacters[] = {npcCharacters0, npcCharacters1, npcCharacters2, 0};
static short secondaryLoadState = 0;
static short secondaryCharacterIndex = 0;
static unsigned char waveRowMajor[0x100];
static unsigned char waveKernelA[0x100];
static unsigned char waveKernelB[0x100];

void e0005_EVT_Auth_Main(void *taskData)
{
    Area0005EventWork *event = (Area0005EventWork *)taskData;
    switch (event->authorState) {
    case 4:
        if (!FTBL.GetShouldRenderIcons(2)) {
            if (FTBL.shenGetCtrlSpeakIconFlag() == 200) {
                if (FTBL.GetPlayerStat(2) < 10) {
                    FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
                    FTBL.HLib_SetupTimedTask(event->task, 30);
                    event->authorState = 3;
                } else {
                    FTBL.AddThisToCashOrSegaCoins(-10, 2);
                    event->authorState = 1;
                    event->entryId++;
                }
            } else {
                FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
                FTBL.HLib_SetupTimedTask(event->task, 30);
                event->authorState = 3;
            }
        }
        break;
    case 1:
        QueueAuthorLoop(event);
        break;
    case 3:
        if (event->entryId == 3) {
            FTBL.EV_authSceneCtrl(-4);
            RestorePhotographCharacters();
            PlaceRyoForEntry(3);
        } else if (event->entryId == 2)
            PlaceRyoForEntry(2);
        if (event->entryId >= 1 && event->entryId <= 2)
            FTBL.HLib_SetCurrentTaskTo(e0005_EVT_Exit_NpcChara2_Callback);
        else
            FTBL.HLib_SetCurrentTaskTo(e0005_EVT_Exit_NpcChara_Callback);
        event->authorState = 5;
        break;
    case 5:
        CompleteEvent(event);
        break;
    }
}

void QueueAuthorLoop(Area0005EventWork *parent)
{
    HLibTaskCallback callbacks[5] = {e0005_EVT_Auth_Loop, e0005_EVT_Auth_Loop_0, e0005_EVT_Auth_Loop_0005A010_Callback, e0005_EVT_Auth_Loop_1,
        e0005_EVT_Auth_Loop_2};
    shen_name scenes[5] = {SHEN_NAME4('A', 'U', '0', '1'), SHEN_NAME4('A', 'U', '0', '2'), SHEN_NAME4('A', 'U', '0', '3'), SHEN_NAME4('A', 'U', '0', '4'),
        SHEN_NAME4('A', 'U', '0', '5')};
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(callbacks[parent->entryId], 4, 11, sizeof(Area0005LoopWork), TASK_EVNT);
    Area0005LoopWork *child = (Area0005LoopWork *)FTBL.HLib_GetTaskParameter(task);
    child->parent = parent;
    child->smoke = -1;
    if (g_currentMap == AREA_0005 || parent->entryId == 0 || parent->entryId == 1 || parent->entryId == 3)
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    switch (parent->entryId) {
    case 0:
        PrepareArrivalCharacters();
        break;
    case 1:
    case 2:
        FTBL.HLib_EnqueueTaskWithoutParameter(e0005_EVT_Load_NpcChara2_Callback, 4, 11, SHEN_NAME4('a', 'u', 't', 'h'));
        PrepareLighterCharacters();
        break;
    case 3:
        FTBL.EV_authSceneCtrl(-11);
        PreparePhotographCharacters();
        break;
    }
    FTBL.EV_authSceneCtrl(scenes[parent->entryId]);
    parent->authorState = 2;
}

void e0005_EVT_Auth_Loop(void *taskData)
{
    Area0005LoopWork *child = (Area0005LoopWork *)taskData;
    if (!FTBL.EV_authSceneCtrl(-1)) {
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        RestoreArrivalCharacters();
        child->parent->authorState = 3;
        FTBL.HLib_DestroyCurrentTask();
        PlaceRyoForEntry(0);
    }
}

void e0005_EVT_Auth_Loop_0(void *taskData)
{
    Area0005LoopWork *child = (Area0005LoopWork *)taskData;
    if (!FTBL.EV_authSceneCtrl(-1)) {
        child->parent->entryId++;
        if (FTBL.EV_authSceneCtrl(-13)) {
            FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
            RestoreLighterCharacters();
            child->parent->authorState = 3;
            FTBL.HLib_DestroyCurrentTask();
        } else {
            FTBL.EV_authSceneCtrl(SHEN_NAME4('A', 'U', '0', '3'));
            FTBL.HLib_SetCurrentTaskTo(e0005_EVT_Auth_Loop_0005A010_Callback);
        }
    }
}

void e0005_EVT_Auth_Loop_0005A010_Callback(void *taskData)
{
    Area0005LoopWork *child = (Area0005LoopWork *)taskData;
    switch (FTBL.EV_authSceneCtrl(-6)) {
    case 1:
        CreateLighterSmoke(child);
        break;
    case 125:
        FTBL.SetParticleEffectBirthRate(0, child->smoke, 0.04980392f);
        break;
    case 132:
        FTBL.SetParticleEffectBirthRate(0, child->smoke, 0.0f);
        break;
    case 158:
        FTBL.SuspendParticleEffect(0, child->smoke);
        break;
    case 1335:
        FTBL.hmdlSetFlagsFor(FTBL.ctrlGetCharaByName(AREA_0005_SYM), 0, 1, 16);
        break;
    case 1385:
        FTBL.hmdlSetFlagsFor(FTBL.ctrlGetCharaByName(AREA_0005_SYM), 0, 8, 16);
        break;
    }
    if (!FTBL.EV_authSceneCtrl(-1)) {
        if (child->smoke != -1) {
            FTBL.SuspendParticleEffect(0, child->smoke);
            FTBL.FreeParticleEffect(0, child->smoke);
            child->smoke = -1;
        }
        RestoreLighterCharacters();
        child->parent->authorState = 3;
        FTBL.HLib_DestroyCurrentTask();
    }
}

void e0005_EVT_Auth_Loop_1(void *taskData)
{
    Area0005LoopWork *child = (Area0005LoopWork *)taskData;
    short frame = FTBL.EV_authSceneCtrl(-6);
    if (frame == FTBL.EV_authSceneCtrl(-5) - 1 || FTBL.EV_authSceneCtrl(-13)) {
        FTBL.EV_authSceneCtrl(-2);
        EVT_AT_KsIcon_Enqueue(0xffff, 200, 201, 0xffff, 5400, 0xffff, 2, 0);
        child->parent->authorState = 4;
        FTBL.HLib_DestroyCurrentTask();
    }
}

void e0005_EVT_Auth_Loop_2(void *taskData)
{
    Area0005LoopWork *child = (Area0005LoopWork *)taskData;
    if (FTBL.EV_authSceneCtrl(-6) == 275)
        FTBL.SetPrimarySampleMuted(1, 0);
    if (!FTBL.EV_authSceneCtrl(-1)) {
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.SetPrimarySampleMuted(1, 1);
        RestorePhotographCharacters();
        PlaceRyoForEntry(4);
        child->parent->authorState = 3;
        FTBL.HLib_DestroyCurrentTask();
    }
}

void e0005_EVT_Load_AuthChara_Callback(void *taskData)
{
    Area0005EventWork *event = (Area0005EventWork *)taskData;
    short *loadState = &event->loadState;
    if (*loadState == 1) {
        SHENMUE_EVENT_OPAQUE(loadState);
        if (FTBL.ForceLoadNpc(authorCharacters[event->characterTable][event->characterIndex]) == 1) {
            event->characterIndex++;
            *loadState = 0;
            if (authorCharacters[event->characterTable][event->characterIndex] == AREA_0005_INVALID) {
                event->characterIndex = 0;
                if (event->entryId == 1)
                    FTBL.HLib_SetCurrentTaskTo(e0005_EVT_Auth_Main);
                else
                    FTBL.HLib_SetCurrentTaskTo(e0005_EVT_Load_NpcChara);
            }
        }
    } else if (event->loadState == 0) {
        FTBL.CheckWarmStartNpcRoute(authorCharacters[event->characterTable][event->characterIndex]);
        FTBL.ForceLoadNpc(authorCharacters[event->characterTable][event->characterIndex]);
        event->loadState = 1;
    }
}

void e0005_EVT_Load_NpcChara(void *taskData)
{
    Area0005EventWork *event = (Area0005EventWork *)taskData;
    int loaded;
    if (event->loadState == 1) {
        FTBL.DebugLogToFileRemoved("%d ID : [%c%c%c%c]\n", event->characterIndex, npcCharacters[event->characterTable][event->characterIndex] & 0xff,
            npcCharacters[event->characterTable][event->characterIndex] >> 8 & 0xff, npcCharacters[event->characterTable][event->characterIndex] >> 16 & 0xff,
            npcCharacters[event->characterTable][event->characterIndex] >> 24 & 0xff);
        loaded = FTBL.ForceLoadNpc(npcCharacters[event->characterTable][event->characterIndex]);
        if (loaded == 1) {
            FTBL.RegisterNpcFlags(npcCharacters[event->characterTable][event->characterIndex], 512, 1);
            event->characterIndex++;
            do {
                event->loadState = 0;
                if (npcCharacters[event->characterTable][event->characterIndex] == AREA_0005_INVALID) {
                    event->characterIndex = 0;
                    FTBL.HLib_SetCurrentTaskTo(e0005_EVT_Auth_Main);
                }
            } while (0);
        }
    } else if (event->loadState == 0) {
        FTBL.ForceLoadNpc(npcCharacters[event->characterTable][event->characterIndex]);
        event->loadState = 1;
    }
}

void e0005_EVT_Load_NpcChara2_Callback(void *taskData)
{
    if (!FTBL.EV_authSceneCtrl(-1)) {
        secondaryLoadState = 0;
        secondaryCharacterIndex = 0;
        FTBL.HLib_DestroyCurrentTask();
    } else if (secondaryLoadState == 1) {
        if (FTBL.ForceLoadNpc(npcCharacters[1][secondaryCharacterIndex]) == 1) {
            secondaryRegisteredCount++;
            FTBL.RegisterNpcFlags(npcCharacters[1][secondaryCharacterIndex], 512, 1);
            FTBL.DebugLogToFileRemoved("%d ID : [%c%c%c%c]\n", secondaryCharacterIndex, npcCharacters[1][secondaryCharacterIndex] & 0xff,
                npcCharacters[1][secondaryCharacterIndex] >> 8 & 0xff, npcCharacters[1][secondaryCharacterIndex] >> 16 & 0xff,
                npcCharacters[1][secondaryCharacterIndex] >> 24 & 0xff);
            secondaryCharacterIndex++;
            secondaryLoadState = 0;
            if (npcCharacters[1][secondaryCharacterIndex] == AREA_0005_INVALID) {
                secondaryCharacterIndex = 0;
                FTBL.HLib_DestroyCurrentTask();
            }
        }
    } else if (secondaryLoadState == 0) {
        FTBL.ForceLoadNpc(npcCharacters[1][secondaryCharacterIndex]);
        secondaryLoadState = 1;
    }
}

void e0005_EVT_Exit_AuthChara_Callback(void *taskData)
{
    Area0005EventWork *event = (Area0005EventWork *)taskData;
    short *loadState = &event->loadState;
    if (*loadState == 1) {
        SHENMUE_EVENT_OPAQUE(loadState);
        if (!FTBL.IsNpcRouteLoadedOrVisible(authorCharacters[event->characterTable][event->characterIndex])) {
            event->characterIndex++;
            *loadState = 0;
            if (authorCharacters[event->characterTable][event->characterIndex] == AREA_0005_INVALID) {
                event->characterIndex = 0;
                FTBL.HLib_SetCurrentTaskTo(e0005_EVT_Auth_Main);
            }
        }
    } else if (event->loadState == 0) {
        if (!FTBL.npcLockUnlock(authorCharacters[event->characterTable][event->characterIndex]))
            FTBL.npcFree(authorCharacters[event->characterTable][event->characterIndex]);
        event->loadState = 1;
    }
}

void e0005_EVT_Exit_NpcChara_Callback(void *taskData)
{
    Area0005EventWork *event = (Area0005EventWork *)taskData;
    int i;
    for (i = 0; npcCharacters[event->characterTable][i] != AREA_0005_INVALID; i++) {
        FTBL.DebugLogToFileRemoved("%d ID : [%c%c%c%c]\n", i, npcCharacters[event->characterTable][i] & 0xff, npcCharacters[event->characterTable][i] >> 8 & 0xff,
            npcCharacters[event->characterTable][i] >> 16 & 0xff, npcCharacters[event->characterTable][i] >> 24 & 0xff);
        FTBL.RegisterNpcFlags(npcCharacters[event->characterTable][i], 512, 0);
    }
    FTBL.HLib_SetCurrentTaskTo(e0005_EVT_Exit_AuthChara_Callback);
}

void e0005_EVT_Exit_NpcChara2_Callback(void *taskData)
{
    int i;
    for (i = 0; i < secondaryRegisteredCount; i++) {
        FTBL.DebugLogToFileRemoved("%d ID : [%c%c%c%c]\n", i, npcCharacters[1][i] & 0xff, npcCharacters[1][i] >> 8 & 0xff, npcCharacters[1][i] >> 16 & 0xff,
            npcCharacters[1][i] >> 24 & 0xff);
        FTBL.RegisterNpcFlags(npcCharacters[1][i], 512, 0);
    }
    FTBL.HLib_SetCurrentTaskTo(e0005_EVT_Exit_AuthChara_Callback);
}

void CreateLighterSmoke(Area0005LoopWork *child)
{
    Vector3 position = {155.74f, 101.8f, 316.1f};
    Vector3 spawnVolume = {0.0f, 0.0f, 0.0f};
    Vector3 velocity = {0.0f, 0.0066666668f, 0.0f};
    Vector3 variance = {0.0f, 0.0f, 0.0015686274f};
    child->smoke = FTBL.CreateSmokeEffect(&position, &spawnVolume, 70, 0.08f, 0, 0);
    FTBL.SetSmokeEffectFadeFactor(child->smoke, 20);
    FTBL.SetSmokeEffectFalloff(child->smoke, 176);
    FTBL.SetParticleEffectColour(0, child->smoke, 0, 149, 149, 149);
    FTBL.SetParticleEffectInitialScale(0, child->smoke, 0.009803922f);
    FTBL.SetParticleEffectSize(0, child->smoke, 1.2941177f, 1.372549f);
    FTBL.SetParticleEffectBirthRate(0, child->smoke, 0.0f);
    FTBL.SetParticleEffectVelocity(0, child->smoke, &velocity, &variance);
    FTBL.ResumeParticleEffect(0, child->smoke);
}

void PlaceRyoForEntry(short entry)
{
    Vector3 positions[5] = {{138.21f, 100.2f, 306.5f}, {150.5f, 100.2f, 315.6f}, {157.4f, 100.2f, 315.7f}, {138.32f, 100.2f, 312.6f},
        {138.32f, 100.2f, 312.6f}};
    Vector3i rotations[5] = {{0, 0x8000, 0}, {0, 0xc000, 0}, {0, 0xd555, 0}, {0, 0x8000, 0}, {0, 0xa000, 0}};
    FTBL.EV_AT_SetPos(AREA_0005_RYO, AREA_0005_RECURSIVE_XYZ, &positions[entry]);
    FTBL.EV_AT_SetAngY_0(AREA_0005_RYO, AREA_0005_RECURSIVE_XYZ, &rotations[entry], 0);
}

void InitializeEventEnvironment(void)
{
    Vector3 direction;
    void *kernelA;
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_FreeAuthorPak(1, "0005");
    FTBL.EV_SetNewSceneIndex();
    PrepareAuthorCharacterMode(AREA_0005_RYO);
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(AREA_0005_RYO), 0xf003, 0);
    PrepareCharacter(SHEN_NAME4('a', '0', '0', '3'));
    PrepareCharacter(SHEN_NAME4('a', '0', '0', '6'));
    PrepareCharacter(SHEN_NAME4('a', '0', '0', '4'));
    PrepareCharacter(SHEN_NAME4('a', '0', '0', '2'));
    PrepareCharacter(SHEN_NAME4('a', '0', '0', '1'));
    PrepareCharacter(SHEN_NAME4('a', '0', '0', '8'));
    if (g_currentMap == AREA_0005) {
        FTBL.GenerateFFT(2.0f);
        FTBL.fft_fill_dist(FTBL.fft_fill_krn(23.529f, FTBL.generate_distance_falloff_row_major(2.082f, waveRowMajor)));
        kernelA = FTBL.fft_fill_krn(0.0564f, FTBL.generate_distance_falloff_kernel_8x8(0.94f, waveKernelA));
        FTBL.fft_bins_load_pairwise_tables(kernelA, FTBL.fft_fill_krn(0.035f, FTBL.generate_distance_falloff_kernel_8x8(0.952f, waveKernelB)));
        FTBL.FFTUtil(0.002f, 0.002f);
        direction.x = 0.0598f;
        direction.y = 0.9768f;
        direction.z = 0.2054f;
        FTBL.set_lighting_flags(0.1f, &direction);
        FTBL.HLib_EnqueueTaskWithoutParameter(e0005_ET_WAVE_AR01_Callback, 3, 2, TASK_EVNT);
    }
}

void CompleteEvent(Area0005EventWork *event)
{
    RestoreAuthorCharacterMode(AREA_0005_RYO);
    FTBL.EV_EnableCharacterDisplay(AREA_0005_RYO);
    FTBL.EV_NPC_NewLoadSw_0(1);
    FTBL.EV_FreeAuthorPak(0, "0005");
    FTBL.DeleteMotion(event->motion);
    switch (event->entryId) {
    case 3:
    case 4:
        FTBL.DebugLogToFileRemoved("NPC_AREA1 : FREE\n");
        FTBL.npcSetAreaName(SHEN_NAME4('A', 'N', 'P', '1'), 0, 0.0f, 0);
        break;
    case 1:
    case 2:
        FTBL.DebugLogToFileRemoved("NPC_AREA2 : FREE\n");
        FTBL.DebugLogToFileRemoved("NPC_AREA3 : FREE\n");
        FTBL.npcSetAreaName(SHEN_NAME4('A', 'N', 'P', '2'), 0, 0.0f, 0);
        FTBL.npcSetAreaName(SHEN_NAME4('A', 'N', 'P', '3'), 0, 0.0f, 0);
        break;
    }
    if (g_currentMap == AREA_0005) {
        FTBL.HLib_DestroyTask(event->task);
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, event->entryId);
    } else {
        FTBL.LoadCutsceneAndSoundProgramForArea();
        switch (event->entryId) {
        case 3:
        case 4:
            FTBL.EV_authSceneCtrl(-10);
            break;
        case 1:
        case 2:
            FTBL.EV_stepSetFlags(97, 0);
            FTBL.EV_stepSetFlags(98, 0);
            break;
        }
        FTBL.HLib_DestroyTask(event->task);
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.SetPlayerControlEnabled(1);
        FTBL.SetRenderClockAndCompassTo(0);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    }
}

void PrepareArrivalCharacters(void)
{
    int i;
    PrepareCharacter(SHEN_NAME4('a', '0', '0', '9'));
    PrepareCharacter(SHEN_NAME4('a', '1', '0', '2'));
    PrepareCharacter(SHEN_NAME4('a', '1', '0', '3'));
    PrepareCharacter(SHEN_NAME4('a', '0', '0', '5'));
    for (i = 0; authorCharacters0[i] != AREA_0005_INVALID; i++)
        PrepareAuthorCharacterMode(authorCharacters0[i]);
}

void RestoreArrivalCharacters(void)
{
    int i;
    RestoreCharacter(SHEN_NAME4('a', '0', '0', '9'));
    RestoreCharacter(SHEN_NAME4('a', '1', '0', '2'));
    RestoreCharacter(SHEN_NAME4('a', '1', '0', '3'));
    RestoreCharacter(SHEN_NAME4('a', '0', '0', '5'));
    for (i = 0; authorCharacters0[i] != AREA_0005_INVALID; i++)
        RestoreAuthorCharacterMode(authorCharacters0[i]);
}

void PrepareLighterCharacters(void)
{
    int i;
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(SHEN_NAME4('R', 'E', '1', '_')));
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(SHEN_NAME4('V', 'C', 'N', '_')));
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(SHEN_NAME4('C', 'H', 'W', '_')));
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(SHEN_NAME4('S', 'H', 'K', '_')));
    PrepareCharacter(SHEN_NAME4('a', '0', '1', '0'));
    for (i = 0; authorCharacters1[i] != AREA_0005_INVALID; i++)
        PrepareAuthorCharacterMode(authorCharacters1[i]);
}

void RestoreLighterCharacters(void)
{
    Vector3 position;
    int i;
    RestoreCharacter(SHEN_NAME4('a', '0', '1', '0'));
    position.x = 155.71f;
    position.y = 100.2f;
    position.z = 315.6f;
    FTBL.EV_AT_SetPos(AREA_0005_SYM, AREA_0005_RECURSIVE_XYZ, &position);
    FTBL.npcSetFlags(AREA_0005_SYM, 0xc000);
    position.x = 155.94f;
    position.y = 100.2f;
    position.z = 314.32f;
    FTBL.EV_AT_SetPos(AREA_0005_KYG, AREA_0005_RECURSIVE_XYZ, &position);
    FTBL.npcSetFlags(AREA_0005_KYG, 0xc000);
    for (i = 0; authorCharacters1[i] != AREA_0005_INVALID; i++)
        RestoreAuthorCharacterMode(authorCharacters1[i]);
}

void PreparePhotographCharacters(void)
{
    int i;
    PrepareCharacter(SHEN_NAME4('a', '1', '0', '2'));
    PrepareCharacter(SHEN_NAME4('a', '1', '0', '3'));
    PrepareCharacter(SHEN_NAME4('a', '0', '0', '5'));
    for (i = 0; authorCharacters2[i] != AREA_0005_INVALID; i++)
        PrepareAuthorCharacterMode(authorCharacters2[i]);
}

void RestorePhotographCharacters(void)
{
    int i;
    RestoreCharacter(SHEN_NAME4('a', '1', '0', '2'));
    RestoreCharacter(SHEN_NAME4('a', '1', '0', '3'));
    RestoreCharacter(SHEN_NAME4('a', '0', '0', '5'));
    for (i = 0; authorCharacters2[i] != AREA_0005_INVALID; i++)
        RestoreAuthorCharacterMode(authorCharacters2[i]);
}

void PrepareAuthorCharacterMode(shen_name name)
{
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 2);
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}

void RestoreAuthorCharacterMode(shen_name name)
{
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 0);
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}

void PrepareCharacter(shen_name name)
{
    FTBL.EV_EnsureCharacterDraw(name);
    FTBL.EV_EnableCharacterDisplay(name);
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}

void RestoreCharacter(shen_name name)
{
    FTBL.EV_ToggleCharacterFlag(name);
    FTBL.EV_DisableCharacterDisplay(name);
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}

void e0005_ET_WAVE_AR01_Callback(void *taskData)
{
    Area0005WaveColours *weather = (Area0005WaveColours *)((char *)FTBL.GetInterpolatedWeatherValues() + 52);
    float diffuseRed = (unsigned char)(weather->diffuse >> 16) / 255.0f;
    float diffuseGreen = (unsigned char)(weather->diffuse >> 8) / 255.0f;
    float diffuseBlue = (unsigned char)weather->diffuse / 255.0f;
    float specularRed = (unsigned char)(weather->specular >> 16) / 255.0f;
    float specularGreen = (unsigned char)(weather->specular >> 8) / 255.0f;
    float specularBlue = (unsigned char)weather->specular / 255.0f;
    float shininess;
    void *root;
    diffuseRed *= 0.3607f;
    diffuseGreen *= 0.3725f;
    diffuseBlue *= 0.4352f;
    specularRed *= 0.2549f;
    specularGreen *= 0.3647f;
    specularBlue *= 0.2862f;
    shininess = 42.54f;
    FTBL.BuildLightingTableFrom(1, 1.0f, 1.0f, diffuseRed, diffuseGreen, diffuseBlue, specularRed, specularGreen, specularBlue, shininess, 1.0f);
    root = FTBL.GetMapModelRoot(8);
    FTBL.CalculateFieldCollision(*(void **)(*(char **)((char *)root + 44) + 40));
    root = FTBL.GetMapModelRoot(9);
    FTBL.TransformFieldCollisionForModel(root);
}
#endif
#else
#include "s2/debug.hpp"
#include "s2/areas/area_0005.hpp"

#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/hand.hpp"
#include "s2/map_assets.hpp"
#include "s2/model_component.hpp"
#include "s2/motion.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/mt7.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/particle_effects.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/story_state.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

namespace shenmue::s2::areas {
namespace {

constexpr shen_name ryo{SHEN_NAME(RYO_)};
constexpr shen_name eventToken{SHEN_NAME(EVNT)};
constexpr shen_name standaloneMap{SHEN_NAME(0005)};
constexpr std::uint32_t recursiveXYZ = 0x78000000U;
constexpr shen_name invalidCharacter{0xffffffffU};

// e0005_EVT_Load_AuthChara_Enqueue allocates 0x18 bytes; the IDB type's trailing word at
// +0x1c belongs to neither this constructor nor any reached callback.
struct EventTaskParameter {
    HLib::Task *task;
    std::uint32_t motionIndex;
    std::int16_t entryId;
    std::int16_t authorState;
    std::int16_t characterTable;
    std::int16_t loadState;
    std::int16_t characterIndex;
    std::uint16_t reserved16;
};
struct AuthorLoopParameter {
    EventTaskParameter *parent;
    std::int32_t smoke;
    std::uint32_t reserved0c;
};
static_assert(sizeof(EventTaskParameter) == 0x18);
static_assert(offsetof(EventTaskParameter, authorState) == 0x0e);
static_assert(offsetof(EventTaskParameter, characterIndex) == 0x14);
static_assert(sizeof(AuthorLoopParameter) == 0x10);
static_assert(offsetof(AuthorLoopParameter, smoke) == 8);


constexpr shen_name authorCharacters0[]{SHEN_NAME(GET_), SHEN_NAME(GYE_), SHEN_NAME(KIZ_), SHEN_NAME(KEI_), SHEN_NAME(SDN_), SHEN_NAME(SZY_), invalidCharacter};
constexpr shen_name authorCharacters1[]{SHEN_NAME(VCN_), SHEN_NAME(RE1_), SHEN_NAME(CHW_), SHEN_NAME(KYG_), SHEN_NAME(SYM_), SHEN_NAME(SHK_), invalidCharacter};
constexpr shen_name authorCharacters2[]{SHEN_NAME(KAP_), SHEN_NAME(RFK_), SHEN_NAME(SDN_), SHEN_NAME(SZY_), invalidCharacter};
constexpr const shen_name *authorCharacters[]{authorCharacters0, authorCharacters1, authorCharacters2};


constexpr shen_name npcCharacters0[]{SHEN_NAME(SUU_), SHEN_NAME(ENN_), SHEN_NAME(GNB_), SHEN_NAME(M02Q), SHEN_NAME(M03Q), SHEN_NAME(M05Q), invalidCharacter};
constexpr shen_name npcCharacters1[]{SHEN_NAME(GNB_), SHEN_NAME(RRI_), SHEN_NAME(CKT_), SHEN_NAME(VCN_), SHEN_NAME(07E_), SHEN_NAME(MOB1), SHEN_NAME(MOB2),
    SHEN_NAME(MOB3), SHEN_NAME(MOB5), SHEN_NAME(MOB8), SHEN_NAME(MOB9), SHEN_NAME(H04Q), SHEN_NAME(H03Q), SHEN_NAME(H13Q), SHEN_NAME(H16Q), SHEN_NAME(Y10Q),
    SHEN_NAME(Y11Q), invalidCharacter};
constexpr shen_name npcCharacters2[]{SHEN_NAME(SOG_), SHEN_NAME(CSY_), SHEN_NAME(00E_), SHEN_NAME(P02Q), SHEN_NAME(P03Q), SHEN_NAME(SYR_), invalidCharacter};
constexpr const shen_name *npcCharacters[]{npcCharacters0, npcCharacters1, npcCharacters2};
std::int16_t secondaryLoadState{};
std::int16_t secondaryCharacterIndex{};
std::int32_t secondaryRegisteredCount{};
std::array<std::byte, 0x100> waveRowMajor{};
std::array<std::byte, 0x100> waveKernelA{};
std::array<std::byte, 0x100> waveKernelB{};

void e0005_EVT_Auth_Main(void *);
void e0005_EVT_Load_AuthChara_Callback(void *);
void e0005_EVT_Load_NpcChara(void *);
void e0005_EVT_Exit_AuthChara_Callback(void *);
void e0005_EVT_Exit_NpcChara_Callback(void *);
void e0005_EVT_Exit_NpcChara2_Callback(void *);
void e0005_EVT_Auth_Loop_0005A010_Callback(void *);


void PlaceRyoForEntry(std::int16_t entry) {
    constexpr Vector3 positions[]{
        {138.21f, 100.2f, 306.5f}, {150.5f, 100.2f, 315.6f}, {157.4f, 100.2f, 315.7f}, {138.32f, 100.2f, 312.6f}, {138.32f, 100.2f, 312.6f}};
    constexpr Vector3i rotations[]{{0, 32768, 0}, {0, 49152, 0}, {0, 54613, 0}, {0, 32768, 0}, {0, 40960, 0}};
    EV_AT_SetPos(ryo, recursiveXYZ, positions[entry]);
    EV_AT_SetAngY(ryo, recursiveXYZ, rotations[entry], 0);
}

void PrepareCharacter(shen_name name) {
    EV_EnsureCharacterDraw(name);
    EV_EnableCharacterDisplay(name);
    EV_SetCharacterAlphaEnabled(name, false);
}
void RestoreCharacter(shen_name name) {
    EV_ToggleCharacterFlag(name);
    EV_DisableCharacterDisplay(name);
    EV_SetCharacterAlphaEnabled(name, true);
}
void SetAuthorCharacterMode(const shen_name *names, bool restore) {
    for (; names->name != invalidCharacter.name; ++names) {
        mtwkSetExtraFlag(FindMotionWork(ctrlGetScenarioChara(*names)), 6, restore ? 0 : 2);
        EV_SetCharacterAlphaEnabled(*names, restore);
    }
}

// PrepareArrivalCharacters / RestoreArrivalCharacters retain distinct setup/restore boundaries.
void PrepareArrivalCharacters() {
    for (const shen_name name : std::array<shen_name, 4>{{SHEN_NAME(a009), SHEN_NAME(a102), SHEN_NAME(a103), SHEN_NAME(a005)}}) {
        PrepareCharacter(name);
    }
    SetAuthorCharacterMode(authorCharacters0, false);
}
void RestoreArrivalCharacters() {
    for (const shen_name name : std::array<shen_name, 4>{{SHEN_NAME(a009), SHEN_NAME(a102), SHEN_NAME(a103), SHEN_NAME(a005)}}) {
        RestoreCharacter(name);
    }
    SetAuthorCharacterMode(authorCharacters0, true);
}


void PrepareLighterCharacters() {
    for (const shen_name name : std::array<shen_name, 4>{{SHEN_NAME(RE1_), SHEN_NAME(VCN_), SHEN_NAME(CHW_), SHEN_NAME(SHK_)}}) {
        SetLowQualityHands(ctrlGetScenarioChara(name));
    }
    PrepareCharacter(SHEN_NAME(a010));
    SetAuthorCharacterMode(authorCharacters1, false);
}
void RestoreLighterCharacters() {
    RestoreCharacter(SHEN_NAME(a010));
    EV_AT_SetPos(SHEN_NAME(SYM_), recursiveXYZ, Vector3{155.71f, 100.2f, 315.6f});
    SetNpcRouteYaw(SHEN_NAME(SYM_), 49152);
    EV_AT_SetPos(SHEN_NAME(KYG_), recursiveXYZ, Vector3{155.94f, 100.2f, 314.32f});
    SetNpcRouteYaw(SHEN_NAME(KYG_), 49152);
    SetAuthorCharacterMode(authorCharacters1, true);
}


void PreparePhotographCharacters() {
    for (const shen_name name : std::array<shen_name, 3>{{SHEN_NAME(a102), SHEN_NAME(a103), SHEN_NAME(a005)}}) {
        PrepareCharacter(name);
    }
    SetAuthorCharacterMode(authorCharacters2, false);
    mtSetUseCustomReadOverride();
}
void RestorePhotographCharacters() {
    for (const shen_name name : std::array<shen_name, 3>{{SHEN_NAME(a102), SHEN_NAME(a103), SHEN_NAME(a005)}}) {
        RestoreCharacter(name);
    }
    SetAuthorCharacterMode(authorCharacters2, true);
    mtResetOverrideFlag();
}


void CreateLighterSmoke(AuthorLoopParameter *child) {
    child->smoke = static_cast<std::int32_t>(CreateSmokeEffect(Vector3{155.74f, 101.8f, 316.1f}, Vector3{}, 70, 0.08f, shen_name{}, 0));
    const std::uint32_t effect = static_cast<std::uint32_t>(child->smoke);
    SetSmokeEffectFalloff(effect, 20);
    SetSmokeEffectFadeFactor(effect, 176);
    SetParticleEffectColour(0, effect, 0, 149, 149, 149);
    SetParticleEffectInitialScale(0, effect, 0.009803921915590763f);
    SetParticleEffectSize(0, effect, 1.2941176891326904f, 1.372549057006836f);
    SetParticleEffectBirthRate(0, effect, 0.0f);
    SetParticleEffectVelocity(0, effect, Vector3{0.0f, 0.0066666668f, 0.0f}, Vector3{0.0f, 0.0f, 0.0015686274f});
    ResumeParticleEffect(0, effect);
}

void e0005_EVT_Auth_Loop(void *taskData) {
    AuthorLoopParameter *const child = static_cast<AuthorLoopParameter *>(taskData);
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    WaitForNextTask_0(GetCurrentAreaScreenFilmTask(), 1, 0);
    RestoreArrivalCharacters();
    child->parent->authorState = 3;
    HLib::DestroyCurrentTask();
    PlaceRyoForEntry(0);
}
void e0005_EVT_Auth_Loop_0(void *taskData) {
    AuthorLoopParameter *const child = static_cast<AuthorLoopParameter *>(taskData);
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    ++child->parent->entryId;
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCantSkipAuthor)) {
        WaitForNextTask_0(GetCurrentAreaScreenFilmTask(), 1, 0);
        RestoreLighterCharacters();
        child->parent->authorState = 3;
        HLib::DestroyCurrentTask();
    } else {
        EV_authSceneCtrl(AuthSceneCtrl::AU03);
        HLib::SetCurrentTaskTo(e0005_EVT_Auth_Loop_0005A010_Callback);
    }
}
void e0005_EVT_Auth_Loop_0005A010_Callback(void *taskData) {
    AuthorLoopParameter *const child = static_cast<AuthorLoopParameter *>(taskData);
    switch (static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame))) {
    case 1:
        CreateLighterSmoke(child);
        break;
    case 125:
        SetParticleEffectBirthRate(0, child->smoke, 0.04980392009f);
        break;
    case 132:
        SetParticleEffectBirthRate(0, child->smoke, 0.0f);
        break;
    case 158:
        SuspendParticleEffect(0, child->smoke);
        break;
    case 1335:
        hmdlSetFlagsFor(ctrlGetScenarioChara(SHEN_NAME(SYM_)), 0, 1, 16);
        break;
    case 1385:
        hmdlSetFlagsFor(ctrlGetScenarioChara(SHEN_NAME(SYM_)), 0, 8, 16);
        break;
    default:
        break;
    }
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    if (child->smoke != -1) {
        SuspendParticleEffect(0, child->smoke);
        FreeParticleEffect(0, child->smoke);
        child->smoke = -1;
    }
    RestoreLighterCharacters();
    child->parent->authorState = 3;
    HLib::DestroyCurrentTask();
}
void e0005_EVT_Auth_Loop_1(void *taskData) {
    AuthorLoopParameter *const child = static_cast<AuthorLoopParameter *>(taskData);
    if (authGetFinished()) {
        WaitForNextTask_0(GetCurrentAreaScreenFilmTask(), 1, 30);
        HLib::SetupTimedTask(child->parent->task, 30);
        child->parent->authorState = 3;
        HLib::DestroyCurrentTask();
        return;
    }
    const std::uint32_t frame = static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (frame == static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining)) - 1U || EV_authSceneCtrl(AuthSceneCtrl::GetCantSkipAuthor)) {
        EV_authSceneCtrl(AuthSceneCtrl::SetAuthorStopped);
        std::int16_t choices[]{-1, 200, 201, -1};
        SetupControlSpeakAndUI(choices, 5400, -1, 2);
        child->parent->authorState = 4;
        HLib::DestroyCurrentTask();
    }
}
void e0005_EVT_Auth_Loop_2(void *taskData) {
    AuthorLoopParameter *const child = static_cast<AuthorLoopParameter *>(taskData);
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 275)
        spmSetSampleEnabled(1, 0);
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    WaitForNextTask_0(GetCurrentAreaScreenFilmTask(), 1, 0);
    spmSetSampleEnabled(1, 1);
    RestorePhotographCharacters();
    PlaceRyoForEntry(4);
    child->parent->authorState = 3;
    HLib::DestroyCurrentTask();
}

void e0005_EVT_Load_NpcChara2_Callback(void *) {
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        secondaryLoadState = 0;
        secondaryCharacterIndex = 0;
        HLib::DestroyCurrentTask();
        return;
    }
    const shen_name character = npcCharacters1[secondaryCharacterIndex];
    if (secondaryLoadState == 1) {
        if (EV_ForceLoadNpc(character) != 1)
            return;
        ++secondaryRegisteredCount;
        RegisterNpcFlags(character, 512, 1);
        secondaryLoadState = 0;
        if (npcCharacters1[++secondaryCharacterIndex].name == invalidCharacter.name) {
            secondaryCharacterIndex = 0;
            HLib::DestroyCurrentTask();
        }
    } else if (secondaryLoadState == 0) {
        EV_ForceLoadNpc(character);
        secondaryLoadState = 1;
    }
}



void QueueAuthorLoop(EventTaskParameter *parent) {
    constexpr HLib::TaskCallback callbacks[]{
        e0005_EVT_Auth_Loop, e0005_EVT_Auth_Loop_0, e0005_EVT_Auth_Loop_0005A010_Callback, e0005_EVT_Auth_Loop_1, e0005_EVT_Auth_Loop_2};
    constexpr shen_name scenes[]{SHEN_NAME(AU01), SHEN_NAME(AU02), SHEN_NAME(AU03), SHEN_NAME(AU04), SHEN_NAME(AU05)};
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(callbacks[parent->entryId], 4, 0x0b, sizeof(AuthorLoopParameter), eventToken);
    AuthorLoopParameter *const child = static_cast<AuthorLoopParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->smoke = -1;
    if (g_currentMap.name == standaloneMap.name || ((parent->entryId & 0xfffc) == 0 && parent->entryId != 2)) {
        WaitForNextTask_0(GetCurrentAreaScreenFilmTask(), 0, 30);
    }
    switch (parent->entryId) {
    case 0:
        PrepareArrivalCharacters();
        break;
    case 1:
    case 2:
        HLib::EnqueueTaskWithoutParameter(e0005_EVT_Load_NpcChara2_Callback, 4, 0x0b, static_cast<std::int32_t>(SHEN_NAME(auth)));
        PrepareLighterCharacters();
        break;
    case 3:
        EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
        PreparePhotographCharacters();
        break;
    case 4:
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        break;
    default:
        break;
    }
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(static_cast<std::int32_t>(scenes[parent->entryId].name)));
    parent->authorState = 2;
}

void e0005_EVT_Load_AuthChara_Callback(void *taskData) {
    EventTaskParameter *const event = static_cast<EventTaskParameter *>(taskData);
    const shen_name character = authorCharacters[event->characterTable][event->characterIndex];
    if (event->loadState == 1) {
        if (EV_ForceLoadNpc(character) != 1)
            return;
        ++event->characterIndex;
        event->loadState = 0;
        if (authorCharacters[event->characterTable][event->characterIndex].name == invalidCharacter.name) {
            event->characterIndex = 0;
            HLib::SetCurrentTaskTo(event->entryId == 1 ? e0005_EVT_Auth_Main : e0005_EVT_Load_NpcChara);
        }
    } else if (event->loadState == 0) {
        EV_CheckWarmStartNpcRoute(character);
        EV_ForceLoadNpc(character);
        event->loadState = 1;
    }
}
void e0005_EVT_Load_NpcChara(void *taskData) {
    EventTaskParameter *const event = static_cast<EventTaskParameter *>(taskData);
    const shen_name character = npcCharacters[event->characterTable][event->characterIndex];
    if (event->loadState == 1) {
        if (EV_ForceLoadNpc(character) != 1)
            return;
        RegisterNpcFlags(character, 512, 1);
        ++event->characterIndex;
        event->loadState = 0;
        if (npcCharacters[event->characterTable][event->characterIndex].name == invalidCharacter.name) {
            event->characterIndex = 0;
            HLib::SetCurrentTaskTo(e0005_EVT_Auth_Main);
        }
    } else if (event->loadState == 0) {
        EV_ForceLoadNpc(character);
        event->loadState = 1;
    }
}
void e0005_EVT_Exit_AuthChara_Callback(void *taskData) {
    EventTaskParameter *const event = static_cast<EventTaskParameter *>(taskData);
    const shen_name character = authorCharacters[event->characterTable][event->characterIndex];
    if (event->loadState == 1) {
        if (IsNpcLoadedOrVisible(character))
            return;
        ++event->characterIndex;
        event->loadState = 0;
        if (authorCharacters[event->characterTable][event->characterIndex].name == invalidCharacter.name) {
            event->characterIndex = 0;
            HLib::SetCurrentTaskTo(e0005_EVT_Auth_Main);
        }
    } else if (event->loadState == 0) {
        if (!EV_npcLockUnlock(character))
            EV_npcFree(character);
        event->loadState = 1;
    }
}
void e0005_EVT_Exit_NpcChara_Callback(void *taskData) {
    const EventTaskParameter *const event = static_cast<EventTaskParameter *>(taskData);
    for (const shen_name *character = npcCharacters[event->characterTable]; character->name != invalidCharacter.name; ++character) {
        RegisterNpcFlags(*character, 512, 0);
    }
    HLib::SetCurrentTaskTo(e0005_EVT_Exit_AuthChara_Callback);
}
void e0005_EVT_Exit_NpcChara2_Callback(void *) {
    for (std::int32_t index = 0; index < secondaryRegisteredCount; ++index) {
        RegisterNpcFlags(npcCharacters1[index], 512, 0);
    }
    HLib::SetCurrentTaskTo(e0005_EVT_Exit_AuthChara_Callback);
}



void CompleteEvent(EventTaskParameter *event) {
    using enum CameraState;
    mtwkSetExtraFlag(FindMotionWork(ctrlGetScenarioChara(ryo)), 6, 0);
    EV_SetCharacterAlphaEnabled(ryo, true);
    EV_EnableCharacterDisplay(ryo);
    EV_NPC_NewLoadSw(1);
    EV_FreeAuthorPak(0, "0005");
    DeleteMotion(static_cast<std::int32_t>(event->motionIndex));
    const std::int16_t entry = event->entryId;
    if (entry > 0 && entry <= 2) {
        EV_debug_log("0005\\e0005_et_0005.c", 760, "NPC_AREA2 : FREE\n");
        EV_debug_log("0005\\e0005_et_0005.c", 761, "NPC_AREA3 : FREE\n");
        npcSetAreaName(SHEN_NAME(ANP2), nullptr, 0.0f, 0);
        npcSetAreaName(SHEN_NAME(ANP3), nullptr, 0.0f, 0);
    } else if (entry > 2 && entry <= 4) {
        EV_debug_log("0005\\e0005_et_0005.c", 754, "NPC_AREA1 : FREE\n");
        npcSetAreaName(SHEN_NAME(ANP1), nullptr, 0.0f, 0);
    }
    if (g_currentMap.name == standaloneMap.name) {
        EV_CleanupMapControlTask();
        HLib::DestroyTask(event->task);
        shenQueueChangeModuleAndScene(SystemModule::Game, g_actNumber, g_currentMap, entry);
        return;
    }
    sbtLoadProgram();
    if (entry > 0 && entry <= 2) {
        EV_stepSetFlags(0, 97);
        EV_stepSetFlags(0, 98);
    } else if (entry > 2 && entry <= 4) {
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    }
    EV_CleanupMapControlTask();
    HLib::DestroyTask(event->task);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    SetPlayerControlEnabled(1);
    shenSetClockAndCompassRenderTo(0);
    WaitForNextTask_0(GetCurrentAreaScreenFilmTask(), 0, 30);
}

void e0005_EVT_Auth_Main(void *taskData) {
    EventTaskParameter *const event = static_cast<EventTaskParameter *>(taskData);
    switch (event->authorState) {
    case 1:
        QueueAuthorLoop(event);
        return;
    case 3:
        if (event->entryId == 3) {
            EV_authSceneCtrl(AuthSceneCtrl::RefreshAndEnqueue);
            RestorePhotographCharacters();
            PlaceRyoForEntry(3);
        } else if (event->entryId == 2) {
            PlaceRyoForEntry(2);
        }
        HLib::SetCurrentTaskTo(static_cast<std::uint16_t>(event->entryId - 1) <= 1U ? e0005_EVT_Exit_NpcChara2_Callback : e0005_EVT_Exit_NpcChara_Callback);
        event->authorState = 5;
        return;
    case 4:
        if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            return;
        if (static_cast<std::uint16_t>(shenGetCtrlSpeakIconFlag()) == 200 && GetCurrentCash() >= 10) {
            AddThisToCashOrSegaCoins(-10, ECashOrCoins::Cash2);
            ++event->entryId;
            event->authorState = 1;
            IncrementSteamStat(11);
        } else {
            WaitForNextTask_0(GetCurrentAreaScreenFilmTask(), 1, 30);
            HLib::SetupTimedTask(event->task, 30);
            event->authorState = 3;
        }
        break;
    case 5:
        CompleteEvent(event);
        break;
    default:
        break;
    }
}



void e0005_ET_WAVE_AR01_Callback(void *) {
    const std::array<std::uint32_t, 2> colours = GetWeatherWaveColours();
    const Vector3 diffuse{static_cast<float>((colours[1] >> 16) & 255U) / 255.0f * 0.3607f, static_cast<float>((colours[1] >> 8) & 255U) / 255.0f * 0.3725f,
        static_cast<float>(colours[1] & 255U) / 255.0f * 0.4352f};
    const Vector3 specular{static_cast<float>((colours[0] >> 16) & 255U) / 255.0f * 0.2549f, static_cast<float>((colours[0] >> 8) & 255U) / 255.0f * 0.3647f,
        static_cast<float>(colours[0] & 255U) / 255.0f};
    BuildWaveLightingTable(1, 1.0f, 1.0f, diffuse, specular, 42.54f, 1.0f);
    ApplyWaveTextureMeshResource(8);
    ApplyWaveReflectionResource(9);
}



void InitializeEventEnvironment() {
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0005");
    EV_SetNewSceneIndex();
    MotionWork *const motion = FindMotionWork(ctrlGetScenarioChara(ryo));
    mtwkSetExtraFlag(motion, 6, 2);
    EV_SetCharacterAlphaEnabled(ryo, false);
    mtwkRequestMotion(motion, 0xf003, MotionRegion::LowerBody);
    for (const shen_name name :
        std::array<shen_name, 6>{{SHEN_NAME(a003), SHEN_NAME(a006), SHEN_NAME(a004), SHEN_NAME(a002), SHEN_NAME(a001), SHEN_NAME(a008)}}) {
        PrepareCharacter(name);
    }
    if (g_currentMap.name != standaloneMap.name)
        return;
    GenerateFFT(2.0f);
    void *row = GenerateDistanceFalloffRowMajor(2.082f, waveRowMajor);
    FillFftDistance(FillFftKernel(23.529f, row));
    void *kernelB = FillFftKernel(0.035f, GenerateDistanceFalloffKernel8x8(0.952f, waveKernelB));
    void *kernelA = FillFftKernel(0.0564f, GenerateDistanceFalloffKernel8x8(0.94f, waveKernelA));
    ApplyFft(kernelA, kernelB);
    FinalizeFft(0.002f, 0.002f);
    SetWaveLighting(0.1f, Vector3{0.0598f, 0.9768f, 0.2054f});
    HLib::EnqueueTaskWithoutParameter(e0005_ET_WAVE_AR01_Callback, 3, 2, static_cast<std::int32_t>(eventToken.name));
}

void e0005_Init() {
    waveRowMajor.fill(std::byte{});
    waveKernelA.fill(std::byte{});
    waveKernelB.fill(std::byte{});
    secondaryRegisteredCount = 0;
    secondaryLoadState = 0;
    secondaryCharacterIndex = 0;
}



void e0005_EVT_Load_AuthChara_Enqueue(std::uintptr_t argument) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(e0005_EVT_Load_AuthChara_Callback, 4, 0x0b, sizeof(EventTaskParameter), eventToken);
    EventTaskParameter *const event = static_cast<EventTaskParameter *>(HLib::GetTaskParameter(task));
    event->task = task;
    event->authorState = 1;
    event->motionIndex = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, "0005A.MOT"));
    event->loadState = 0;
    event->characterIndex = 0;
    shenSetClockAndCompassRenderTo(1);
    if (g_currentMap.name == standaloneMap.name) {
        BeginPrimarySampleExecution(0);
        event->entryId = g_entryPoint > 4 ? 0 : static_cast<std::int16_t>(g_entryPoint);
        if (event->entryId == 0)
            event->characterTable = 0;
        else if (event->entryId > 0 && event->entryId <= 2)
            event->characterTable = 1;
        else if (event->entryId > 2 && event->entryId <= 4)
            event->characterTable = 2;
    } else {
        event->entryId = *reinterpret_cast<const std::int16_t *>(argument);
        if (event->entryId == 0) {
            EV_FreeWAIR();
            EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
            EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
            EV_UpdateWeather();
            EV_stepSetFlags(0, 69);
            event->characterTable = 0;
        } else if (event->entryId == 1) {
            event->characterTable = 1;
        } else if (event->entryId == 3) {
            event->characterTable = 2;
        }
    }
    if (event->entryId > 0 && event->entryId <= 2) {
        const Vector3 second{148.5f, 100.0f, 315.74f};
        const Vector3 third{156.03f, 100.0f, 315.76f};
        if (npcSetAreaName(SHEN_NAME(ANP2), &second, 5.0f, 1) == 1)
            EV_debug_log("0005\\e0005_et_0005.c", 230, "NPC_AREA2 : TRUE\n");
        else
            EV_debug_log("0005\\e0005_et_0005.c", 232, "NPC_AREA2 : FALSE\n");
        if (npcSetAreaName(SHEN_NAME(ANP3), &third, 6.0f, 1) == 1)
            EV_debug_log("0005\\e0005_et_0005.c", 236, "NPC_AREA3 : TRUE\n");
        else
            EV_debug_log("0005\\e0005_et_0005.c", 238, "NPC_AREA3 : FALSE\n");
    } else if (event->entryId > 2 && event->entryId <= 4) {
        const Vector3 first{138.21f, 100.0f, 311.32f};
        if (npcSetAreaName(SHEN_NAME(ANP1), &first, 2.0f, 1) == 1)
            EV_debug_log("0005\\e0005_et_0005.c", 221, "NPC_AREA1 : TRUE\n");
        else
            EV_debug_log("0005\\e0005_et_0005.c", 223, "NPC_AREA1 : FALSE\n");
    }
    InitializeEventEnvironment();
}

} // namespace

const ScriptAreaDefinition area0005ScriptDefinition{standaloneMap, 0, e0005_Init, e0005_EVT_Load_AuthChara_Enqueue};

} // namespace shenmue::s2::areas
#endif
