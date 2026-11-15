#if defined(SHENMUE_EVENT_OVERLAY) && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/scripts/cwon/area_cwon.hpp"

#if defined(SHENMUE_EVENT_ENTRY)

void AreaCWONMain(int entry) {
    HLibTask *task;
    CwonWork *work;
    int selected;
    FTBL.SetPlayerControlEnabled(0);
    FTBL.SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    FTBL.SetLoadingBusy();
    work = CwonCreateTask(&task, (HLibTaskCallback)CwonMainCallback, sizeof(*work));
    if (!work)
        FTBL.EventAssertNullsub("./src/et_ChaseWon.c", 336);
    else {
        memset(work, 0, sizeof(*work));
        if (g_currentMap == SHEN_NAME4('A', 'R', '0', '2')) {
            selected = entry;
            FTBL.EV_SetNewSceneIndex();
            EVT_UTL_SetNPCLowQualityHands(0);
        } else {
            selected = g_entryPoint;
            FTBL.EV_ControlPackageResources(2, 4, "MPK00", 0, 0);
        }
        if (selected < 9) {
            CwonInitialize(work, task, selected);
            CwonInitializePlayer(work);
            Queue_EVT_UTL_LoadNPCChara_3(CWON_CHARACTERS, task);
        } else
            FTBL.DebugLogToFileRemoved("Entry Error\n");
    }
}

#else

shen_name CWON_CHARACTERS[] = {SHEN_NAME4('W', 'O', 'N', '_'), 0xffffffffU};
shen_name CWON_RELEASE_CHARACTERS_1[] = {SHEN_NAME4('G', 'E', 'T', '_'), 0xffffffffU};
shen_name CWON_RELEASE_CHARACTERS_2[] = {SHEN_NAME4('D', 'Y', 'U', '_'), SHEN_NAME4('S', 'E', 'E', '_'), SHEN_NAME4('M', 'E', 'Y', '_'),
    SHEN_NAME4('A', 'G', 'O', '_'), SHEN_NAME4('E', 'E', 'N', '_'), 0xffffffffU};
shen_name CWON_AFTER_STREET_CHARACTERS[] = {SHEN_NAME4('A', 'N', 'A', '_'), SHEN_NAME4('K', 'J', 'N', '_'), SHEN_NAME4('K', 'L', 'N', '_'),
    SHEN_NAME4('S', 'H', 'I', '_'), SHEN_NAME4('E', 'I', 'N', '_'), 0xffffffffU};
shen_name CWON_RELEASE_CHARACTERS_3[] = {SHEN_NAME4('D', 'E', 'Y', '_'), 0xffffffffU};
shen_name CWON_ADDITIONAL_CHARACTERS[] = {SHEN_NAME4('S', 'A', 'M', '_'), SHEN_NAME4('L', 'L', 'Y', '_'), 0xffffffffU};
shen_name CWON_WAREHOUSE_CHARACTERS[] = {SHEN_NAME4('C', 'H', 'S', '_'), SHEN_NAME4('H', 'L', 'N', '_'), 0xffffffffU};
shen_name CWON_CHASE_NPC_CHARACTERS[] = {SHEN_NAME4('G', 'E', 'T', '_'), SHEN_NAME4('D', 'Y', 'U', '_'), SHEN_NAME4('S', 'E', 'E', '_'),
    SHEN_NAME4('M', 'E', 'Y', '_'), SHEN_NAME4('A', 'G', 'O', '_'), SHEN_NAME4('E', 'E', 'N', '_'), 0xffffffffU};
shen_name CWON_BEE_PASS_CHARACTERS[] = {SHEN_NAME4('G', 'E', 'T', '_'), SHEN_NAME4('D', 'Y', 'U', '_'), SHEN_NAME4('S', 'E', 'E', '_'),
    SHEN_NAME4('M', 'E', 'Y', '_'), SHEN_NAME4('A', 'G', 'O', '_'), SHEN_NAME4('E', 'E', 'N', '_'), SHEN_NAME4('A', 'N', 'A', '_'),
    SHEN_NAME4('K', 'J', 'N', '_'), SHEN_NAME4('K', 'L', 'N', '_'), SHEN_NAME4('S', 'H', 'I', '_'), SHEN_NAME4('E', 'I', 'N', '_'), 0xffffffffU};
shen_name CWON_MARKET_CHARACTERS[] = {SHEN_NAME4('D', 'Y', 'U', '_'), SHEN_NAME4('S', 'E', 'E', '_'), SHEN_NAME4('M', 'E', 'Y', '_'),
    SHEN_NAME4('A', 'G', 'O', '_'), SHEN_NAME4('E', 'E', 'N', '_'), SHEN_NAME4('A', 'N', 'A', '_'), SHEN_NAME4('K', 'J', 'N', '_'),
    SHEN_NAME4('K', 'L', 'N', '_'), SHEN_NAME4('S', 'H', 'I', '_'), SHEN_NAME4('E', 'I', 'N', '_'), 0xffffffffU};
shen_name CWON_LAST_CHASE_CHARACTERS[] = {
    SHEN_NAME4('D', 'E', 'Y', '_'), SHEN_NAME4('S', 'A', 'M', '_'), SHEN_NAME4('L', 'L', 'Y', '_'), SHEN_NAME4('C', 'L', 'J', '_'), 0xffffffffU};
shen_name CWON_BEEFAIL_CHARACTERS[] = {SHEN_NAME4('R', 'Y', 'O', '_'), 0};
shen_name CWON_AUTHOR_CHARACTERS[] = {SHEN_NAME4('R', 'Y', 'O', '_'), SHEN_NAME4('W', 'O', 'N', '_'), 0};
shen_name CWON_DODGE_CHARACTERS[] = {SHEN_NAME4('R', 'Y', 'O', '_'), SHEN_NAME4('G', 'E', 'T', '_'), 0};
shen_name CWON_CHAIR_CHARACTERS[] = {SHEN_NAME4('R', 'Y', 'O', '_'), SHEN_NAME4('W', 'O', 'N', '_'), SHEN_NAME4('D', 'Y', 'U', '_'), 0};
shen_name CWON_MEY_CHARACTERS[] = {SHEN_NAME4('R', 'Y', 'O', '_'), SHEN_NAME4('M', 'E', 'Y', '_'), 0};
shen_name CWON_END_CHARACTERS[] = {SHEN_NAME4('R', 'Y', 'O', '_'), SHEN_NAME4('W', 'O', 'N', '_'), SHEN_NAME4('C', 'L', 'J', '_'),
    SHEN_NAME4('S', 'A', 'M', '_'), SHEN_NAME4('L', 'L', 'Y', '_'), 0};
shen_name CWON_ENDFAIL_CHARACTERS[] = {SHEN_NAME4('R', 'Y', 'O', '_'), SHEN_NAME4('W', 'O', 'N', '_'), SHEN_NAME4('D', 'E', 'Y', '_'), 0};
shen_name CWON_TABLE_CHARACTERS[] = {SHEN_NAME4('R', 'Y', 'O', '_'), SHEN_NAME4('W', 'O', 'N', '_'), SHEN_NAME4('A', 'G', 'O', '_'),
    SHEN_NAME4('E', 'E', 'N', '_'), SHEN_NAME4('S', 'E', 'E', '_'), 0};
shen_name CWON_TABLE_MOTION_CHARACTERS[] = {SHEN_NAME4('M', 'E', 'Y', '_'), 0};
shen_name CWON_BEE_GRAVITY_CHARACTERS[] = {SHEN_NAME4('B', 'D', 'M', 'A'), 0};
shen_name CWON_END_GRAVITY_CHARACTERS[] = {SHEN_NAME4('a', '1', '0', '0'), SHEN_NAME4('a', '1', '0', '1'), 0};

static unsigned char cwonDistanceKernel[256], cwonFirstKernel[256], cwonSecondKernel[256];
static unsigned int cwonSmoke;
static int cwonPipeStep;
static HLibTask *cwonNpcLoadTask;
unsigned int cwonPackIndex;
HLibTask *cwonWaveTask;
HLibTask *cwonSpringTask;
unsigned int cwonPack;
HLibTask *cwonDynamixTask;
unsigned int cwonPackState;

void CwonApplyLighting(void) {
    DateTime time;
    FTBL.SetWeatherUpdateEnabledTo(1);
    FTBL.RefreshWeatherForCurrentTime(0);
    FTBL.UpdateWeather();
    FTBL.GetCurrentDateTime(&time);
    if ((unsigned char)(time.hour - 5) > 13) {
        FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        FTBL.PushGlobalLightingProgram();
        FTBL.CheckIsArgEquZero(1, 1);
        FTBL.SetObjectFlagByIndex(15, 1);
        FTBL.SetObjectFlagByIndex(16, 0);
    } else {
        FTBL.ReinitializeLights(1);
        FTBL.SetLightFlagForIndex(0, 2);
        FTBL.SetLightEntryIndex(0, 1);
        FTBL.SetObjectFlagByIndex(15, 0);
        FTBL.SetObjectFlagByIndex(16, 1);
    }
}

void CwonWaveCallback(void *taskData) {
    unsigned char *weather = (unsigned char *)FTBL.GetInterpolatedWeatherValues();
    unsigned int *colours = (unsigned int *)(weather + 0x34);
    float red = (int)((colours[7] >> 16) & 255) / 255.0f;
    float green = (int)((colours[7] >> 8) & 255) / 255.0f;
    float blue = (int)weather[0x50] / 255.0f;
    float specularRed = (int)((colours[5] >> 16) & 255) / 255.0f;
    float specularGreen = (int)((colours[5] >> 8) & 255) / 255.0f;
    float specularBlue = (int)weather[0x48] / 255.0f;
    void *model;
    float power;
    red *= 0.3607f;
    green *= 0.3725f;
    blue *= 0.4352f;
    specularRed *= 0.2549f;
    specularGreen *= 0.38f;
    specularBlue *= 0.2862f;
    power = 42.54f;
    FTBL.BuildLightingTableFrom(1, 1.0f, 1.0f, red, green, blue, specularRed, specularGreen, specularBlue, power, 1.0f);
    model = FTBL.GetMapModelRoot(18);
    FTBL.CalculateFieldCollision(*(void **)((unsigned char *)*(void **)((unsigned char *)model + 0x2c) + 0x28));
    model = FTBL.GetMapModelRoot(19);
    FTBL.TransformFieldCollisionForModel(model);
    FTBL.nlSetTexanimUV(-0.001f, 0.001f);
    FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(19));
}

void CwonSpringCallback(void *data) {
    FTBL.nlSetTexanimUV(0, .055f);
    FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(20));
    FTBL.nlSetTexanimUV(0, .027f);
    FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(21));
}

void CwonPrepareCharacters(const shen_name *names) {
    int index = 0;
    shen_name name;
    while (names[index] != 0xffffffff) {
        if (FTBL.DoesScenarioCharacterExist(names[index])) {
            name = names[index];
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(name), 1);
            FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(name), 0, 1, 1);
            FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(name), 3);
            if (name == SHEN_NAME4('W', 'O', 'N', '_')) {
                FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(name), 0);
                FTBL.SetScenarioCharacterType(name, 4);
            }
        }
        index++;
    }
}

int CwonIsNpcLoadComplete(HLibTask **task) {
    return EVT_UTL_IsNPCCharaLoaded();
}

void CwonMainCallback(CwonWork *work) {
    if (work->startCallback) {
        FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(SHEN_NAME4('R', 'Y', 'O', '_')), 3);
        FTBL.SetPlayerGameFlags(2, 0);
        CwonPrepareCharacters(CWON_CHARACTERS);
        FTBL.HLib_SetCurrentTaskTo(work->startCallback);
        EVT_UTL_SetNPCLowQualityHands(0);
        if (work->entry > 6)
            Queue_EVT_UTL_LoadNPCChara_3(CWON_ADDITIONAL_CHARACTERS, FTBL.HLib_GetCurrentTask());
        work->dummyTask = FTBL.HLib_EnqueueTaskWithoutParameter(CwonDummyControlCallback, 4, 11, SHEN_NAME4('d', 'c', 't', 'l'));
    } else
        FTBL.HLib_DestroyCurrentTask();
}

void CwonDeleteMotion(CwonWork *work, int index) {
    if (index < 4) {
        if (work->motions[index]) {
            FTBL.DebugLogToFileRemoved("Motion Delete %d\n");
            FTBL.DeleteMotion((void *)work->motions[index]);
            work->motions[index] = 0;
        }
    } else
        FTBL.DebugLogToFileRemoved("Motion Delete Error\n");
}

void CwonLoadMotion(CwonWork *work, int index) {
    static const char *files[4] = {"0011E.MOT", "0012E.MOT", "0012A.MOT", "REN.MOT"};
    if (index < 4) {
        if (!work->motions[index] && files[index]) {
            FTBL.DebugLogToFileRemoved("Motion Read %s\n", files[index]);
            work->motions[index] = (unsigned)FTBL.ReadMotiFromDirectory(g_actNumber, g_currentMap, files[index]);
        }
    } else
        FTBL.DebugLogToFileRemoved("Motion Read Error\n");
}

void CwonLoadAuthorPackages(int load) {
    static const char *packages[3] = {"0011", "0012", 0};
    int i;
    if (load)
        for (i = 0; packages[i]; i++)
            FTBL.EV_FreeAuthorPak(1, packages[i]);
    else
        for (i = 0; packages[i]; i++)
            FTBL.EV_FreeAuthorPak(0, packages[i]);
}

void CwonInitialize(CwonWork *work, HLibTask *task, int entry) {
    static Vector3 CWON_ZERO_POSITION = {0.0f, 0.0f, 0.0f};
    static Vector3i CWON_ZERO_ROTATION = {0, 0, 0};

    HLibTaskCallback starts[9] = {(HLibTaskCallback)CwonFreeControlCallback, (HLibTaskCallback)CwonRootStreetIntroCallback,
        (HLibTaskCallback)CwonRootMarketIntroCallback, (HLibTaskCallback)CwonRootMarketIntroCallback, (HLibTaskCallback)CwonRootWarehouseIntroCallback,
        (HLibTaskCallback)CwonRootWarehouseIntroCallback, CwonInitializeBattleCallback, CwonInitializeBattleCallback, CwonInitializeBattleCallback};
    Vector3 light;
    float *first, *second;
    int i;
    FTBL.EV_authSceneCtrl(-7);
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    FTBL.EventSystemNullsub();
    FTBL.CreateEventCharacter(3, 0, SHEN_NAME4('C', 'E', 'N', 'T'));
    FTBL.CreateEventCharacter(3, 0, SHEN_NAME4('D', 'U', 'M', '0'));
    FTBL.CreateEventCharacter(3, 0, SHEN_NAME4('D', 'U', 'M', '1'));
    FTBL.EV_AT_SetPos(SHEN_NAME4('C', 'E', 'N', 'T'), 0x78000000, &CWON_ZERO_POSITION);
    FTBL.EV_AT_SetPos(SHEN_NAME4('D', 'U', 'M', '0'), 0x78000000, &CWON_ZERO_POSITION);
    FTBL.EV_AT_SetPos(SHEN_NAME4('D', 'U', 'M', '1'), 0x78000000, &CWON_ZERO_POSITION);
    FTBL.EV_AT_SetAngY_0(SHEN_NAME4('C', 'E', 'N', 'T'), 0x38000000, &CWON_ZERO_ROTATION, 0);
    FTBL.EV_AT_SetAngY_0(SHEN_NAME4('D', 'U', 'M', '0'), 0x38000000, &CWON_ZERO_ROTATION, 0);
    FTBL.EV_AT_SetAngY_0(SHEN_NAME4('D', 'U', 'M', '1'), 0x38000000, &CWON_ZERO_ROTATION, 0);
    FTBL.EV_stepSetFlags(5, 0);
    CwonApplyLighting();
    if (g_currentMap == SHEN_NAME4('C', 'W', 'O', 'N')) {
        FTBL.GenerateFFT(2);
        FTBL.fft_fill_dist(FTBL.fft_fill_krn(23.529f, FTBL.generate_distance_falloff_row_major(2.082f, cwonDistanceKernel)));
        first = FTBL.fft_fill_krn(.0564f, FTBL.generate_distance_falloff_kernel_8x8(.94f, cwonFirstKernel));
        second = FTBL.fft_fill_krn(.035f, FTBL.generate_distance_falloff_kernel_8x8(.952f, cwonSecondKernel));
        FTBL.fft_bins_load_pairwise_tables(first, second);
        FTBL.FFTUtil(.002f, .002f);
        light.x = .0598f;
        light.y = .9768f;
        light.z = .2054f;
        FTBL.set_lighting_flags(.1f, &light);
        cwonWaveTask = FTBL.HLib_EnqueueTaskWithoutParameter(CwonWaveCallback, 3, 2, SHEN_NAME4('w', 'a', 'v', 'e'));
        cwonSpringTask = FTBL.HLib_EnqueueTaskWithoutParameter(CwonSpringCallback, 3, 2, SHEN_NAME4('f', 'n', 't', 'n'));
        CwonInitializeWaveLighting();
    }
    work->task = task;
    work->entry = entry;
    work->startCallback = starts[work->entry];
    work->temporarySlot = 0;
    work->smoke = -1;
    cwonWaveTask = 0;
    cwonSpringTask = 0;
    for (i = 0; i < 4; i++)
        CwonDeleteMotion(work, i);
    if (work->entry < 7)
        CwonLoadMotion(work, 0);
    work->authorState = (unsigned char)(work->entry - 2) < 7 ? -1 : 0;
    CwonLoadAuthorPackages(1);
    if (g_currentMap != SHEN_NAME4('A', 'R', '0', '2')) {
        unsigned pack = FTBL.EV_ControlPackageResources(1, 0, FTBL.Filepath_Gen(0), "CWONPACK.afs", 0);
        cwonPack = pack;
        cwonPackIndex = cwonPackState = -1;
    }
    if (entry > 1)
        FTBL.EV_authSceneCtrl(-11);
    FTBL.EV_spmBeginSampleExecute(0);
}

void CwonInitializePlayer(CwonWork *work) {
    const shen_name player = SHEN_NAME4('R', 'Y', 'O', '_');
    CwonStartPlayerAt(work->entry);
    FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(player), 1);
    FTBL.EV_SetCharacterAlphaEnabled(player, 0);
    FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(player), 1);
    FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(player), 1, 1, 1);
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(player));
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(player), 1, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(player), 1, 1, 0);
}

void CwonStartPlayerAt(int entry) {
    static Vector3 CWON_PLAYER_POSITIONS[] = {{232.0f, 100.212997f, 374.600006f}, {216.0f, 99.3141022f, 409.0f}, {256.559998f, 100.900002f, 381.899994f},
        {256.559998f, 100.900002f, 381.899994f}, {263.299988f, 100.900002f, 401.5f}, {253.850006f, 100.0f, 426.25f}, {239.300003f, 98.4599991f, 466.350006f},
        {260.0f, 101.0f, 397.5f}, {207.600006f, 98.9000015f, 390.600006f}};
    static int CWON_PLAYER_YAWS[] = {270, 310, 185, 185, 180, 180, 0, 45, 0};

    EVT_AT_SetPos(SHEN_NAME4('R', 'Y', 'O', '_'), SHEN_NAME4('C', 'E', 'N', 'T'), CWON_PLAYER_POSITIONS[entry].x, CWON_PLAYER_POSITIONS[entry].y,
        CWON_PLAYER_POSITIONS[entry].z, 0);
    EVT_AT_SetAngY(SHEN_NAME4('R', 'Y', 'O', '_'), SHEN_NAME4('C', 'E', 'N', 'T'), CWON_PLAYER_YAWS[entry], 0);
}

void CwonFreeControlCallback(CwonWork *work) {
    Vector3 position;
    switch (work->authorState) {
    case 0:
        work->authorState++;
        FTBL.SetPlayerControlEnabled(1);
        FTBL.SetScreenFilmFade(20, 255, 0, 0, 0, 0, 0, 0, 0);
        break;
    case 1:
        CwonGetLocalTransform(RYO, &position, 0);
        if ((position.z > 414 && position.x < 220) || FTBL.GetCurrentControlInput(1) == 1024) {
            work->authorState = 0;
            FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)CwonRootStreetIntroCallback);
            FTBL.SetPlayerControlEnabled(0);
            EVT_AT_SetMot(RYO, 0xf085, -1, 1, 0, 0);
            FTBL.SetScreenFilmFade(20, 0, 0, 0, 0, 255, 0, 0, 0);
            CwonQueueFadeWait(work->task);
        }
        break;
    }
}

void CwonRootStreetIntroCallback(CwonWork *work) {
    switch (work->authorState) {
    case 0:
        CwonAuthNullsub();
        work->authorState++;
        CwonReleaseAndLoadPackages(0, -1, -1, 0, 1);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        FTBL.SetScreenFilmFade(20, 255, 0, 0, 0, 0, 0, 0, 0);
        CwonQueueTemporary(work, CwonBirdCallback, 0);
        CwonStartAuthor(0, work->task, 0, CWON_AUTHOR_CHARACTERS, 0);
        cwonNpcLoadTask = Queue_EVT_UTL_LoadNPCChara_3(CWON_CHASE_NPC_CHARACTERS, 0);
        break;
    case 1:
        work->authorState = 0;
        FTBL.EV_authSceneCtrl(-11);
        FTBL.EV_authSceneCtrl(-8);
        CwonQueueEscape(work, work->entry);
        CwonStartPlayerAt(1);
        EVT_AT_SetMot(RYO, 0x1029, -1, 1, 0, 0);
        EVT_AT_SetVoice("0011B016", 10);
        FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)StreetRun);
        break;
    }
}

void StreetRun(CwonWork *work) {
    EVT_AT_SetPos(RYO, CENTER, 231.8500061f, 100.21f, 393.35f, 0);
    EVT_AT_SetAng(RYO, CENTER, 0, 325, 0, 0);
    EVT_AT_SetMot(RYO, 0x1029, -1, 1, 0x40000000, 0);
    EVT_AT_SetCam(141, DUMMY, DUMMY, 0, 0, 0);
    CwonChangeAndWaitMotion((HLibTaskCallback)AdvanceStreetRoute, work->task, RYO, 0);
}

void AdvanceStreetRoute(CwonWork *work) {
    static CwonRouteEntry CWON_STREET_ROUTE[] = {{0, 0, 0.0f, 61895, 127, 0, 0, SHEN_NAME4('R', 'Y', 'O', '_')},
        {0, 0, 0.0f, 54613, 345, 0, 1, SHEN_NAME4('R', 'Y', 'O', '_')}, {3, 0, 0.0f, 0, 0, 0, 0, SHEN_NAME4('\x00', '\x00', '\x00', '\x00')}};

    work->routeState = CwonAdvanceRoute(CWON_STREET_ROUTE, work->routeState);
    if (work->routeState == -1) {
        work->routeState = 0;
        FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)BeeQte);
    }
}

void BeeQte(CwonWork *work) {
    static const int parts[20] = {166, 165, 164, 163, 162, 161, 160, 159, 158, 157, 156, 155, 154, 153, 152, 171, 170, 169, 168, 167};
    int frames;
    switch (work->authorState) {
    case 0:
        work->authorState++;
        CwonDestroyEscape(work);
        frames = CwonStartAuthor(9, work->task, 0, CWON_AUTHOR_CHARACTERS, CWON_BEE_GRAVITY_CHARACTERS);
        EVT_AT_QTE(256, 30, 0, frames - 35, 3);
        EVT_AT_SetPos(BEE, CENTER, 253.0187073f, 100.2677002f, 380.0407104f, 0);
        EVT_AT_SetAng(BEE, CENTER, 0, 345, 0, 0);
        EVT_AT_CharaDisp(WON, frames - 2, 1);
        EVT_AT_CharaDisp(BEE, 0, 1);
        EVT_AT_CharaDisp(BEE, 56, 0);
        cwonDynamixTask = Queue_EVT_AT_PlayDynamix(BEE, "BDMA", 2, 600, 20, parts, 4, 56);
        break;
    case 1:
        CwonPollQte(work, 256, (HLibTaskCallback)BeePass, (HLibTaskCallback)BeeFail);
        break;
    }
}

void BeePass(CwonWork *work) {
    int frames;
    switch (work->authorState) {
    case 0:
        if (CwonIsNpcLoadComplete(&cwonNpcLoadTask)) {
            work->authorState++;
            CwonPrepareCharacters(CWON_BEE_PASS_CHARACTERS);
            EVT_AT_SetPos(WON, CENTER, 0, 0, 0, 0);
            frames = CwonStartAuthor(10, work->task, 0, CWON_AUTHOR_CHARACTERS, 0);
            EVT_AT_QTE(64, 15, 0, frames - 20, 3);
            CwonQueueTemporary(work, CwonPlayerDustCallback, 30);
            CwonReleaseAndLoadPackages(cwonDynamixTask, 0, 1, 2, 3);
        }
        break;
    case 1:
        CwonPollQte(work, 64, (HLibTaskCallback)DodgePass, (HLibTaskCallback)DodgeFail);
        break;
    }
}

void BeeFail(CwonWork *work) {
    Vector3 prop = {260.1f, 101, 393.7f};
    switch (work->authorState) {
    case 0:
        work->authorState++;
        work->failures++;
        CwonStartAuthor(11, work->task, 0, CWON_BEEFAIL_CHARACTERS, 0);
        EVT_AT_CharaDisp(WON, 0, 1);
        CwonDisableGravity(WON);
        CwonQueueTemporary(work, CwonMarketPropsCallback, 0);
        break;
    case 1:
        work->authorState++;
        CwonDisableGravity(RYO);
        EVT_AT_SetCam(-1, DUMMY, DUMMY, 0, 0, 0);
        EVT_AT_SetPos(RYO, CENTER, 255.57f, 100.9f, 380.75f, 0);
        EVT_AT_SetAngY(RYO, CENTER, 210, 0);
        EVT_AT_SetMot(RYO, 0x1029, -1, 1, 0x60000000, 0);
        FTBL.HLib_SetupTaskDelay(10);
        break;
    case 2:
        FTBL.EV_AT_SetPos(SHEN_NAME4('I', 'S', '2', '3'), 0x78000000, &prop);
        if (CwonTurnTowardYaw(RYO, 0x838e, 364, 1)) {
            work->authorState = 0;
            FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)CheckStreet);
        }
        break;
    }
}

void DodgePass(CwonWork *work) {
    switch (work->authorState) {
    case 0:
        work->authorState++;
        CwonResetSmoke(work);
        CwonQueueTemporary(work, CwonGetDodgeCallback, 0);
        EVT_AT_SetPos(RYO, CENTER, 256.8f, 100.9f, 381.3f, 0);
        EVT_AT_SetAngY(RYO, CENTER, 180, 0);
        EVT_AT_SetCam(129, DUMMY, DUMMY, 0, 0, 0);
        EVT_AT_SetMot(RYO, 0x1031, -1, 1, 0x60000000, 0);
        EVT_AT_SetVoice("0011A021", 0);
        CwonChangeAndWaitMotion(0, work->task, RYO, 0);
        break;
    case 1:
        work->authorState = 0;
        EVT_AT_SetMot(RYO, 0x1029, -1, 1, 0, 0);
        FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)CheckStreet);
        break;
    }
}

void DodgeFail(CwonWork *work) {
    switch (work->authorState) {
    case 0:
        work->authorState++;
        CwonResetSmoke(work);
        CwonStartAuthor(12, work->task, 135, CWON_DODGE_CHARACTERS, 0);
        break;
    case 1:
        work->authorState++;
        EVT_AT_CharaDisp(WON, 0, 1);
        CwonStartAuthor(-1, work->task, 0, CWON_DODGE_CHARACTERS, 0);
        break;
    case 2:
        work->authorState = 0;
        FTBL.SetRefBoardParamsForCharaById(SHEN_NAME4('G', 'E', 'T', '_'), 1);
        FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)CheckStreet);
        CheckStreet(work);
        break;
    }
}

void CwonRootMarketIntroCallback(CwonWork *work) {
    switch (work->authorState) {
    case -1:
        work->authorState++;
        CwonReleaseAndLoadPackages(0, -1, -1, 2, 3);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        break;
    case 0:
        work->authorState++;
        FTBL.SetScreenFilmFade(20, 255, 0, 0, 0, 0, 0, 0, 0);
        if (work->entry == 2)
            CwonStartAuthor(15, work->task, 0, CWON_AUTHOR_CHARACTERS, 0);
        else
            CwonStartAuthor(14, work->task, 0, CWON_AUTHOR_CHARACTERS, 0);
        work->entry = 3;
        cwonNpcLoadTask = Queue_EVT_UTL_LoadNPCChara_3(CWON_MARKET_CHARACTERS, 0);
        break;
    case 1:
        work->authorState++;
        CwonStartAuthor(16, work->task, 0, CWON_AUTHOR_CHARACTERS, 0);
        break;
    case 2:
        if (CwonIsNpcLoadComplete(&cwonNpcLoadTask)) {
            work->authorState = 0;
            EVT_AT_SetMot(RYO, 0x1029, -1, 1, 0, 0);
            EVT_AT_SetMot(WON, 0x1029, -1, 1, 0, 0);
            FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)CheckStreet);
        }
        break;
    }
}

void CheckStreet(CwonWork *work) {
    switch (work->authorState) {
    case 0:
        FTBL.camCtrlSetFarDistance(45);
        FTBL.camCtrlSetNearDistance(45);
        FTBL.EV_authSceneCtrl(-8);
        if (work->failures >= 2)
            work->authorState++;
        else {
            work->authorState = 0;
            work->failures = 0;
            FTBL.EV_stepSetFlags(9, 0);
            if (work->entry < 2) {
                FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
                Queue_EVT_UTL_LoadNPCChara_3(CWON_AFTER_STREET_CHARACTERS, 0);
            }
            Queue_EVT_AT_PlayDynamix(SHEN_NAME4('H', 'A', 'K', '0'), "HAKB", 2, 4, 10, 0, 2, 0);
            Queue_EVT_AT_PlayDynamix(SHEN_NAME4('H', 'A', 'K', '1'), "HAKB", 2, 4, 10, 0, 2, 0);
            CwonQueueTemporary(work, CwonMarketPropsCallback, 0);
            FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)ChairQte);
        }
        break;
    case 1:
        if (CwonPositionThreshold(RYO, 385.5f, 1, 0)) {
            work->authorState = 0;
            work->routePass = 0;
            FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)LostEnd);
        }
        break;
    }
}

void ChairQte(CwonWork *work) {
    switch (work->authorState) {
    case 0:
        work->authorState++;
        CwonInitializeWaveLighting();
    case 1:
        work->authorState++;
        CwonPrepareChairProps();
        CwonStartAuthor(19, work->task, 0, CWON_CHAIR_CHARACTERS, 0);
        EVT_AT_QTE(4, 25, 0, 71, 3);
        FTBL.EV_DisableCharacterDisplay(SHEN_NAME4('I', 'S', '_', '1'));
        FTBL.EV_DisableCharacterDisplay(SHEN_NAME4('I', 'S', '_', '2'));
        FTBL.EV_DisableCharacterDisplay(SHEN_NAME4('I', 'S', '_', '3'));
        FTBL.EV_DisableCharacterDisplay(SHEN_NAME4('I', 'S', '_', '4'));
        FTBL.EV_ToggleCharacterFlag(SHEN_NAME4('I', 'S', '_', '1'));
        FTBL.EV_ToggleCharacterFlag(SHEN_NAME4('I', 'S', '_', '2'));
        FTBL.EV_ToggleCharacterFlag(SHEN_NAME4('I', 'S', '_', '3'));
        FTBL.EV_ToggleCharacterFlag(SHEN_NAME4('I', 'S', '_', '4'));
        break;
    case 2:
        CwonPollQte(work, 4, (HLibTaskCallback)ChairPass, (HLibTaskCallback)ChairFail);
        break;
    }
}

void ChairPass(CwonWork *work) {
    FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)TableQte);
    CwonStartAuthor(20, work->task, 0, CWON_CHAIR_CHARACTERS, 0);
}

void ChairFail(CwonWork *work) {
    FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)TableQte);
    CwonStartAuthor(21, work->task, 0, CWON_CHAIR_CHARACTERS, 0);
}

void TableQte(CwonWork *work) {
    int frames;
    switch (work->authorState) {
    case 0:
        work->authorState++;
        frames = CwonStartAuthor(25, work->task, 0, CWON_TABLE_CHARACTERS, 0);
        EVT_AT_QTE(256, 20, 0, frames - 25, 3);
        CwonPrepareMotionCharacters(CWON_TABLE_MOTION_CHARACTERS);
        CwonSetCollisionTables();
        CwonPrepareTableProps();
        EVT_AT_SetPos(SHEN_NAME4('D', 'Y', 'U', '_'), SHEN_NAME4('D', 'Y', 'U', '_'), -.5f, 0, 0, 1);
        break;
    case 1:
        CwonPollQte(work, 256, (HLibTaskCallback)TablePass, (HLibTaskCallback)TableFail);
        break;
    }
}

void TablePass(CwonWork *work) {
    int frames;
    FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)MeyQte);
    frames = CwonStartAuthor(26, work->task, 0, CWON_TABLE_CHARACTERS, 0);
    EVT_AT_QTE(512, 20, 0, frames - 25, 3);
}

void TableFail(CwonWork *work) {
    static const int parts[17] = {152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168};
    FTBL.EV_SetPlayerFlag(12, 0x791, 1);
    CwonSetFallenTable();
    Queue_EVT_AT_PlayDynamix(SHEN_NAME4('T', 'B', 'L', 'e'), "TUKU", 2, 200, 17, parts, 2, 0);
    FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)CheckMarket);
    FTBL.HLib_SetupTaskDelay(5);
    CwonStartAuthor(27, work->task, 0, CWON_TABLE_CHARACTERS, 0);
}

void MeyQte(CwonWork *work) {
    CwonPollQte(work, 512, (HLibTaskCallback)MeyPass, (HLibTaskCallback)MeyFail);
}

void MeyPass(CwonWork *work) {
    switch (work->authorState) {
    case 0:
        work->authorState++;
        CwonStartAuthor(17, work->task, 0, CWON_MEY_CHARACTERS, 0);
        break;
    case 1:
        work->authorState = 0;
        CwonDisableGravity(SHEN_NAME4('M', 'E', 'Y', '_'));
        FTBL.SetRefBoardParamsForCharaById(SHEN_NAME4('M', 'E', 'Y', '_'), 1);
        FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)CheckMarket);
        break;
    }
}

void MeyFail(CwonWork *work) {
    switch (work->authorState) {
    case 0:
        work->authorState++;
        CwonStartAuthor(18, work->task, 30, CWON_MEY_CHARACTERS, 0);
        break;
    case 1:
        work->authorState++;
        CwonStartAuthor(-1, work->task, 56, CWON_MEY_CHARACTERS, 0);
        break;
    case 2:
        work->authorState++;
        CwonStartAuthor(-1, work->task, 0, CWON_MEY_CHARACTERS, 0);
        break;
    case 3:
        work->authorState = 0;
        CwonDisableGravity(SHEN_NAME4('M', 'E', 'Y', '_'));
        FTBL.SetRefBoardParamsForCharaById(SHEN_NAME4('M', 'E', 'Y', '_'), 1);
        FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)CheckMarket);
        break;
    }
}

void CheckMarket(CwonWork *work) {
    switch (work->authorState) {
    case -1:
        work->authorState++;
        work->entry = 4;
        EVT_AT_SetMot(RYO, 0x1029, -1, 1, 0, 0);
        FTBL.SetScreenFilmFade(1, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.EV_authSceneCtrl(-8);
        break;
    case 0:
        FTBL.camCtrlSetFarDistance(140);
        FTBL.camCtrlSetNearDistance(140);
        CwonQueueEscape(work, 4);
        EVT_AT_SetCam(160, DUMMY, DUMMY, 0, 0, 0);
        EVT_AT_SetPos(RYO, CENTER, 263.3f, 100.9f, 401.5f, 0);
        EVT_AT_SetAngY(RYO, CENTER, 210, 0);
        EVT_AT_SetMot(RYO, 0x1037, -1, 1, 0, 0);
        EVT_AT_SetMot(RYO, 0x102f, -1, 1, 0, 45);
        CwonQueueTemporary(work, CwonWarehousePropsCallback, 0);
        EVT_AT_QTE(64, 20, 0, 35, 3);
        FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)BoxQte);
        FTBL.HLib_SetupTaskDelay(55);
        break;
    }
}

void BoxQte(CwonWork *work) {
    CwonPollQte(work, 64, (HLibTaskCallback)BoxPass, (HLibTaskCallback)BoxFail);
}

void BoxPass(CwonWork *work) {
    EVT_AT_SetCam(144, DUMMY, DUMMY, 0, 0, 0);
    EVT_AT_SetVoice("0011A021", 20);
    EVT_AT_SetMot(RYO, 0x1031, -1, 1, 0x20000000, 0);
    CwonChangeAndWaitMotion((HLibTaskCallback)MarketExit, work->task, RYO, 0);
}

void BoxFail(CwonWork *work) {
    static Vector3 CWON_BOXFAIL_PLAYER_POSITION = {265.0f, 101.0f, 404.0f};

    EVT_AT_CharaDisp(WON, 0, 1);
    EVT_AT_SetCam(166, DUMMY, DUMMY, 0, 0, 0);
    EVT_AT_SetVoice("0011A015", 10);
    EVT_AT_SetMot(RYO, 0x102c, -1, 1, 0, 10);
    FTBL.EV_AT_SetPos(RYO, 0x78000000, &CWON_BOXFAIL_PLAYER_POSITION);
    CwonChangeAndWaitMotion((HLibTaskCallback)MarketExit, work->task, RYO, 45);
}

void MarketExit(CwonWork *work) {
    int yaw, step;
    switch (work->authorState) {
    case 0:
        if (CwonIsNpcLoadComplete(&cwonNpcLoadTask)) {
            work->authorState++;
            EVT_AT_CharaDisp(WON, 0, 0);
            EVT_AT_SetMot(RYO, 0x1029, -1, 1, 0, 0);
            FTBL.HLib_SetupTaskDelay(20);
        }
        break;
    case 1:
        yaw = 0x8000;
        step = 910;
        goto turn;
    case 2:
        work->authorState++;
        EVT_UTL_FreeNPCChara(CWON_CHASE_NPC_CHARACTERS);
        CwonReleaseAndLoadPackages(cwonDynamixTask, 2, 3, 4, 5);
        EVT_AT_SetCam(162, DUMMY, DUMMY, 0, 0, 0);
        EVT_AT_SetMot(RYO, 0x102f, 0, 1, 0, 0);
        EVT_AT_SetPos(RYO, CENTER, 272.1f, 100.9f, 415, 0);
        EVT_AT_SetAngY(RYO, CENTER, 180, 0);
        EVT_AT_QTE(64, 20, 0, 10, 3);
        break;
    case 3:
        yaw = 0x4e38;
        step = 637;
    turn:
        if (CwonTurnTowardYaw(RYO, yaw, step, 1))
            work->authorState++;
        break;
    case 4:
        CwonPollQte(work, 64, (HLibTaskCallback)WorkerPass, (HLibTaskCallback)WorkerFail);
        break;
    }
}

void WorkerPass(CwonWork *work) {
    EVT_AT_SetCam(147, DUMMY, DUMMY, 0, 0, 0);
    EVT_AT_SetMot(RYO, 0x1031, -1, .8f, 0x20000000, 0);
    FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)DoorQte);
    FTBL.HLib_SetupTaskDelay(30);
    CwonAdvanceWonWarehouse();
}

void WorkerFail(CwonWork *work) {
    EVT_AT_SetCam(167, DUMMY, DUMMY, 0, 0, 0);
    EVT_AT_SetPos(RYO, CENTER, 268, 100.9f, 418.7f, 0);
    EVT_AT_SetMot(RYO, 0x102d, -1, 1, 0x20000000, 0);
    EVT_AT_SetVoice("0011A015", 0);
    EVT_AT_SetVoice("0011A016", 30);
    CwonQueueTemporary(work, CwonFallingWorkerCallback, 10);
    CwonQueueTemporary(work, CwonFallingSignCallback, 10);
    FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)DoorQte);
    FTBL.HLib_SetupTaskDelay(40);
}

void DoorQte(CwonWork *work) {
    switch (work->authorState) {
    case 0:
        work->authorState++;
        FTBL.SetDoorSubjectEnabled(SHEN_NAME4('R', '1', '2', '2'), 0);
        FTBL.EV_ToggleCharacterFlag(SHEN_NAME4('A', 'N', 'A', '_'));
        FTBL.EV_ToggleCharacterFlag(SHEN_NAME4('K', 'Y', 'A', 'T'));
        EVT_AT_CharaDisp(SHEN_NAME4('A', 'N', 'A', '_'), 1, 1);
        EVT_AT_CharaDisp(SHEN_NAME4('K', 'Y', 'A', 'T'), 1, 1);
        EVT_AT_CharaDisp(SHEN_NAME4('K', 'A', 'N', 'B'), 1, 1);
        CwonStartAuthor(28, work->task, 60, CWON_AUTHOR_CHARACTERS, 0);
        CwonDestroyEscape(work);
        break;
    case 1:
        work->authorState++;
        CwonStartAuthor(-1, work->task, 0, CWON_AUTHOR_CHARACTERS, 0);
        EVT_AT_QTE(4, 15, 0, 0, 3);
        CwonQueueLook(RYO, SHEN_NAME4('K', 'J', 'N', '_'), 5, 0, 1, 5, 5, 15);
        CwonQueueTemporary(work, CwonOpeningDoorCallback, 0);
        break;
    case 2:
        if (CwonPollQte(work, 4, (HLibTaskCallback)DoorPass, (HLibTaskCallback)DoorFail)) {
            FTBL.EV_EnableCharacterDisplay(SHEN_NAME4('K', 'A', 'N', 'B'));
            EVT_AT_CharaDisp(WON, 0, 1);
            EVT_AT_SetAngY(RYO, CENTER, 90, 0);
        }
        break;
    }
}

void DoorPass(CwonWork *work) {
    switch (work->authorState) {
    case 0:
        work->authorState++;
        EVT_AT_SetPos(RYO, CENTER, 262.2f, 100.9f, 420.5f, 0);
        EVT_AT_SetCam(149, DUMMY, DUMMY, 0, 0, 0);
        EVT_AT_SetVoice("0011A021", 0);
        EVT_AT_SetMot(RYO, 0x1031, 5, 1, 0, 0);
        CwonChangeAndWaitMotion((HLibTaskCallback)DoorPass, work->task, RYO, 20);
        break;
    case 1:
        work->authorState = 0;
        EVT_AT_SetMot(RYO, 0x1029, -1, 1, 0, 0);
        FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)CleanQte);
        break;
    }
}

void DoorFail(CwonWork *work) {
    switch (work->authorState) {
    case 0:
        work->authorState++;
        EVT_AT_SetPos(RYO, CENTER, 262.3f, 100.9f, 420.2f, 0);
        EVT_AT_SetCam(159, DUMMY, DUMMY, 0, 0, 0);
        EVT_AT_SetMot(RYO, 0x100f, 7, 1, 0x20000000, 0);
        CwonChangeAndWaitMotion((HLibTaskCallback)DoorFail, work->task, RYO, 0);
        EVT_AT_SetVoice("0011L002", 10);
        break;
    case 1:
        work->authorState = 0;
        EVT_AT_SetMot(RYO, 0x1029, -1, 1, 0, 0);
        EVT_AT_SetPos(RYO, CENTER, 261, 100.9f, 418.8f, 0);
        EVT_AT_SetCam(113, DUMMY, DUMMY, 0, 0, 0);
        FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)CleanQte);
        break;
    }
}

void CleanQte(CwonWork *work) {
    int done;
    switch (work->authorState) {
    case 0:
        work->authorState++;
        CwonQueueTemporary(work, CwonTurningPedestrianCallback, 0);
        FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(RYO), .8f, 0);
        EVT_AT_QTE(256, 20, 0, 0, 3);
        break;
    case 1:
        if (CwonPositionThreshold(RYO, 257.5f, 0, 1))
            work->authorState++;
        break;
    case 2:
        if (CwonTurnTowardYaw(RYO, 0x71c7, 910, 0))
            work->authorState++;
        break;
    case 3:
        if (CwonPositionThreshold(RYO, 420, 1, 0))
            work->authorState++;
        break;
    case 4:
        if (CwonPollQte(work, 256, (HLibTaskCallback)CleanPass, (HLibTaskCallback)CleanFail))
            FTBL.EV_SetCharaControllerFlags(RYO, 3, 0);
        break;
    }
}

void CleanPass(CwonWork *work) {
    static Vector3 CWON_LANDING_SMOKE_POSITION = {253.75f, 100.0f, 425.5f};

    Vector3 position;
    switch (work->authorState) {
    case 0:
        work->authorState++;
        if (work->routeState != 0) {
            EVT_AT_SetPos(RYO, CENTER, work->cachedPosition.x, work->cachedPosition.y, work->cachedPosition.z, 0);
            EVT_AT_SetAng(RYO, CENTER, work->cachedRotation.x / 182.04445f, work->cachedRotation.y / 182.04445f, work->cachedRotation.z / 182.04445f, 0);
        } else {
            CwonGetLocalTransform(RYO, &work->cachedPosition, &work->cachedRotation);
            CwonQueueLook(SHEN_NAME4('S', 'H', 'I', '_'), RYO, 5, 0, 1, 3, 50, 15);
        }
        CwonDisableGravity(RYO);
        EVT_AT_SetPos(RYO, CENTER, 254.9f, 100.9f, 421, 0);
        EVT_AT_SetAngY(RYO, CENTER, 160, 0);
        EVT_AT_SetMot(RYO, 0x1036, -1, 1.2f, 0x40000000, 0);
        switch (work->routeState) {
        case 0:
            EVT_AT_SetVoice("0011N001", 15);
            EVT_AT_SetCam(138, DUMMY, DUMMY, 0, 0, 0);
            break;
        case 1:
            EVT_AT_SetCam(170, DUMMY, DUMMY, 0, 0, 0);
            break;
        case 2:
            EVT_AT_SetCam(171, DUMMY, DUMMY, 0, 0, 0);
            break;
        }
        FTBL.HLib_SetupTaskDelay(7);
        break;
    case 1:
        FTBL.GetCharaPosition(RYO, -1, &position, 0);
        position.y -= .05f;
        position.z += .03f;
        if (position.y > 100) {
            if (position.y < 100.35f && position.y >= 100.3f)
                CwonCreateSmokeAt(work, &CWON_LANDING_SMOKE_POSITION);
        } else {
            work->authorState++;
            position.y = 100;
            FTBL.HLib_SetupTaskDelay(5);
        }
        EVT_AT_SetPos(RYO, CENTER, position.x, position.y, position.z, 0);
        break;
    case 2:
        work->authorState = 0;
        work->routeState = 0;
        EVT_AT_SetMot(RYO, 0x1029, 0, 1, 0x40000000, 1);
        FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(RYO), 0);
        FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)CheckWarehouse);
        break;
    }
}

void CleanFail(CwonWork *work) {
    int frames;
    work->authorState = 0;
    CwonDisableGravity(RYO);
    EVT_AT_SetCam(150, DUMMY, DUMMY, 0, 0, 0);
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(RYO), 0);
    EVT_AT_SetPos(RYO, CENTER, 254.4f, 100.9f, 418.9f, 0);
    EVT_AT_SetAngY(RYO, CENTER, 180, 0);
    EVT_AT_SetVoice("0011A029", 10);
    EVT_AT_SetMot(RYO, 0x102d, -1, 1, 0, 0);
    frames = EVT_AT_SetMot(RYO, 0x1031, 8, 1, 0, 30);
    frames = EVT_AT_SetMot(RYO, 0xf008, 0, 1, 0x20000000, frames);
    EVT_AT_SetPos(RYO, RYO, 0, -.9f, 0, frames - 1);
    frames = EVT_AT_SetMot(RYO, 0x1029, 0, 1, 0x20000000, frames);
    FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)CheckWarehouse);
    FTBL.HLib_SetupTaskDelay(frames);
}

void CwonRootWarehouseIntroCallback(CwonWork *work) {
    EventFunctionTable *table, *flags;
    switch (work->authorState) {
    case -1:
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), "CWONTRY2", 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        Queue_EVT_UTL_LoadNPCChara_3(CWON_WAREHOUSE_CHARACTERS, FTBL.HLib_GetCurrentTask());
        work->authorState++;
        break;
    case 0:
        (table = &FTBL)->SetScreenFilmFade(20, 255, 0, 0, 0, 0, 0, 0, 0);
        SHENMUE_EVENT_CLOBBER("r8");
        flags = table;
        SHENMUE_EVENT_BOUNDARY();
        flags->EV_SetCharaControllerFlags(RYO, 2, 0);
        flags->EV_SetCharaControllerFlags(RYO, 3, 0);
        CwonPrepareCharacters(CWON_WAREHOUSE_CHARACTERS);
        FTBL.EV_authSceneCtrl(-14);
        if (work->entry == 4)
            FTBL.EV_authSceneCtrl(SHEN_NAME4('R', 'A', 'U', '1'));
        else
            FTBL.EV_authSceneCtrl(SHEN_NAME4('R', 'A', 'U', '2'));
        work->entry = 5;
        CwonReleaseAndLoadPackages(0, -1, -1, 4, 5);
        work->authorState++;
        break;
    case 1:
        if (!FTBL.EV_authSceneCtrl(-1)) {
            FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
            FTBL.EV_ControlPackageResources(0, 6, "CWONTRY2", 0, 0);
            EVT_UTL_FreeNPCChara(CWON_WAREHOUSE_CHARACTERS);
            EVT_AT_SetMot(RYO, 0x1029, -1, 1, 0, 0);
            EVT_AT_SetMot(WON, 0x1029, -1, 1, 0, 0);
            FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)CheckWarehouse);
            work->authorState = 0;
        }
        return;
    default:
        return;
    }
}

void CheckWarehouse(CwonWork *work) {
    switch (work->authorState) {
    case -1:
        work->authorState++;
        work->entry = 5;
        EVT_AT_SetVoice("0011A031", 0);
        EVT_AT_SetVoice("0011B011", 20);
        FTBL.HLib_SetupTaskDelay(20);
        FTBL.SetScreenFilmFade(20, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.EV_authSceneCtrl(-8);
        break;
    case 0:
        CwonResetSmoke(work);
        CwonQueuePipeDynamics();
        if (work->failures >= 2) {
            work->routePass = 1;
            EVT_AT_SetCam(204, DUMMY, DUMMY, 0, 0, 10);
            EVT_AT_SetMot(RYO, 0x102f, -1, 1, 0x40000000, 10);
            EVT_AT_SetAngY(RYO, CENTER, 160, 10);
            EVT_AT_SetPos(RYO, CENTER, 253.8f, 100, 427.66f, 10);
            FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)LostEnd);
            FTBL.HLib_SetupTaskDelay(18);
        } else {
            work->authorState = 0;
            work->failures = 0;
            CwonRestoreGravity(RYO);
            CwonQueueEscape(work, 5);
            FTBL.EV_stepSetFlags(10, 0);
            CwonStartPlayerAt(5);
            FTBL.camCtrlSetMotionScaleFactor(1);
            EVT_AT_SetCam(137, DUMMY, DUMMY, 0, 0, 0);
            FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)StairsQte);
            EVT_UTL_FreeNPCChara(CWON_AFTER_STREET_CHARACTERS);
            cwonNpcLoadTask = Queue_EVT_UTL_LoadNPCChara_3(CWON_LAST_CHASE_CHARACTERS, 0);
        }
        break;
    }
}

void StairsQte(CwonWork *work) {
    int frames, yaw;
    switch (work->authorState) {
    case 0:
        work->authorState++;
        FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(RYO), 0);
        EVT_AT_SetMot(RYO, 0x1029, -1, 1, 0, 0);
        CwonAuthNullsub();
        break;
    case 1:
        yaw = 0x71c7;
        goto turn;
    case 2:
        if (CwonPositionThreshold(RYO, 443, 1, 0)) {
            work->authorState++;
            FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(WON), 1);
            EVT_AT_QTE(4, 20, 0, 0, 3);
            FTBL.camCtrlSetMotionScaleFactor(.3f);
            FTBL.camCtrlToggleSmoothing(0);
            EVT_AT_SetCam(114, DUMMY, DUMMY, 0, 0, 0);
            CwonDisableGravity(RYO);
            EVT_AT_SetPos(RYO, CENTER, 247.83f, 100, 443.4f, 0);
            EVT_AT_SetAngY(RYO, CENTER, 160, 0);
            frames = EVT_AT_SetMot(RYO, 0xf02a, 0, 1, 0, 0);
            EVT_AT_SetPos(RYO, RYO, 0, .9f, 0, frames - 1);
        }
        break;
    case 3:
        if (CwonPositionThreshold(RYO, 445.5f, 1, 0)) {
            EVT_AT_SetPos(RYO, CENTER, 247.15f, 101.575f, 445.5f, 0);
            EVT_AT_SetMot(RYO, 0x1029, 3, .85f, 0x02000000, 0);
            FTBL.HLib_SetupTaskDelay(5);
            work->authorState++;
        }
        break;
    case 4:
        yaw = 0x4e38;
    turn:
        if (CwonTurnTowardYaw(RYO, yaw, 910, 1))
            work->authorState++;
        break;
    case 5:
        CwonPollQte(work, 4, (HLibTaskCallback)StairsPass, (HLibTaskCallback)StairsFail);
        break;
    }
}

void StairsPass(CwonWork *work) {
    EVT_AT_SetVoice("0011A021", 0);
    EVT_AT_SetCam(164, DUMMY, DUMMY, 0, 0, 0);
    EVT_AT_SetPos(RYO, CENTER, 246, 101.6f, 448.3000183f, 0);
    EVT_AT_SetMot(RYO, 0x1031, -1, 1, 0, 0);
    CwonChangeAndWaitMotion((HLibTaskCallback)SignQte, work->task, RYO, 0);
}

void StairsFail(CwonWork *work) {
    switch (work->authorState) {
    case 0:
        work->authorState++;
        EVT_AT_SetCam(151, DUMMY, DUMMY, 0, 0, 0);
        EVT_AT_SetVoice("0011A034", 0);
        EVT_AT_SetMot(RYO, 0x100f, 10, 1, 0x20000000, 0);
        EVT_AT_SetMot(RYO, 0x102f, -1, 1, 0, 55);
        FTBL.HLib_SetupTaskDelay(45);
        break;
    case 1:
        if (CwonTurnTowardYaw(RYO, 0x5555, 910, 0)) {
            work->authorState = 0;
            FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)SignQte);
            FTBL.HLib_SetupTaskDelay(10);
        }
        break;
    }
}

void SignQte(CwonWork *work) {
    switch (work->authorState) {
    case 0:
        work->authorState++;
        CwonDestroyEscape(work);
        FTBL.EV_authSceneCtrl(-8);
        CwonStartAuthor(22, work->task, 70, CWON_AUTHOR_CHARACTERS, 0);
        break;
    case 1:
        work->authorState++;
        CwonSetPipeStep(1);
        work->routeState = CwonQueueTemporary(work, CwonPipeCallback, 0);
        EVT_AT_QTE(16, 20, 0, 9, 3);
        FTBL.HLib_SetupTaskDelay(10);
        break;
    case 2:
        CwonPollQte(work, 16, (HLibTaskCallback)SignPass, (HLibTaskCallback)SignFail);
        break;
    }
}

void SignPass(CwonWork *work) {
    switch (work->authorState) {
    case 0:
        work->authorState++;
        CwonQueueTemporary(work, CwonPlayerOffsetDustCallback, 0);
        CwonStartAuthor(23, work->task, 25, CWON_AUTHOR_CHARACTERS, 0);
        CwonDestroyTemporary(work, work->routeState);
        work->routeState = CwonQueueTemporary(work, CwonPipeCallback, 0);
        CwonSetTemporaryTicks(work, work->routeState, 32);
        if (CwonIsNpcLoadComplete(&cwonNpcLoadTask))
            FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), "CWONHAND", 0);
        break;
    case 1:
        work->authorState = 0;
        CwonDestroyTemporary(work, work->routeState);
        EVT_AT_SetSound(668, 20);
        work->routeState = CwonQueueTemporary(work, CwonPipeCallback, 0);
        CwonSetTemporaryTicks(work, work->routeState, 10);
        FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)EndQte);
        CwonStartAuthor(-1, work->task, 105, CWON_AUTHOR_CHARACTERS, 0);
        break;
    }
}

void SignFail(CwonWork *work) {
    switch (work->authorState) {
    case 0:
        work->authorState++;
        CwonStartAuthor(24, work->task, 75, CWON_AUTHOR_CHARACTERS, 0);
        if (work->failures > 1)
            EVT_AT_CharaDisp(WON, 0, 1);
        CwonDestroyTemporary(work, work->routeState);
        CwonSetPipeStep(2);
        work->routeState = CwonQueueTemporary(work, CwonPipeCallback, 0);
        CwonSetTemporaryTicks(work, work->routeState, 14);
        break;
    case 1:
        work->authorState = 0;
        if (work->failures > 1) {
            FTBL.EV_authSceneCtrl(-4);
            CwonRestoreMotionCharacters(CWON_AUTHOR_CHARACTERS);
            EVT_AT_SetPos(WON, CENTER, 0, 0, 0, 1);
            EVT_AT_SetPos(RYO, CENTER, 233.25f, 101.57f, 449.9f, 0);
            EVT_AT_SetAngY(RYO, CENTER, 180, 0);
            EVT_AT_SetMot(RYO, 0x1029, 1, 1, 0x22000000, 0);
            FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(RYO), 0);
            FTBL.camCtrlSetState(0);
            FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)RunEnd);
        } else {
            CwonFinishEscape(work);
            FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)EndQte);
            CwonStartAuthor(-1, work->task, 90, CWON_AUTHOR_CHARACTERS, 0);
        }
        break;
    }
}

void EndQte(CwonWork *work) {
    switch (work->authorState) {
    case 0:
        work->authorState++;
        EVT_AT_QTE(256, 20, 0, 0, 3);
        CwonResetSmoke(work);
        break;
    case 1:
        CwonPollQte(work, 256, (HLibTaskCallback)EndPass, (HLibTaskCallback)EndFail);
        break;
    }
}

void EndPass(CwonWork *work) {
    DateTime time;
    int i;
    switch (work->authorState) {
    case 0:
        work->authorState++;
        FTBL.EV_authSceneCtrl(-7);
        CwonStartAuthor(1, work->task, 0, CWON_AUTHOR_CHARACTERS, CWON_END_GRAVITY_CHARACTERS);
        Queue_EVT_AT_PlayDynamix(SHEN_NAME4('B', 'H', 'K', '0'), "BOX1", 2, 4, 38, 0, 2, 0);
        Queue_EVT_AT_PlayDynamix(SHEN_NAME4('B', 'H', 'K', '1'), "BOX1", 2, 4, 38, 0, 2, 0);
        Queue_EVT_AT_PlayDynamix(SHEN_NAME4('B', 'H', 'K', '2'), "BOX1", 2, 4, 38, 0, 2, 0);
        Queue_EVT_AT_PlayDynamix(SHEN_NAME4('B', 'H', 'K', '3'), "BOX1", 2, 4, 38, 0, 2, 0);
        FTBL.HLib_EnqueueTaskWithoutParameter((HLibTaskCallback)WonSmoke, 6, 11, SHEN_NAME4('s', 'm', 'o', 'k'));
        break;
    case 1:
        if (CwonIsNpcLoadComplete(&cwonNpcLoadTask)) {
            work->authorState++;
            work->routePass = 4;
            CwonPrepareCharacters(CWON_LAST_CHASE_CHARACTERS);
            FTBL.GetCurrentDateTime(&time);
            if (time.hour > 18)
                FTBL.EV_lightLoad(SHEN_NAME4('0', '0', '0', '1'));
            CwonStartAuthor(2, 0, 0, CWON_END_CHARACTERS, CWON_END_GRAVITY_CHARACTERS);
        }
    case 2:
        if (CwonUpdateBattleIntro(work))
            work->authorState++;
        break;
    case 3:
        work->authorState = 0;
        for (i = 0; i < 4; i++)
            CwonDeleteMotion(work, i);
        CwonHideCharacter(SHEN_NAME4('a', '0', '1', '4'));
        FTBL.camCtrlToggleSmoothing(0);
        FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)CwonInitializeBattleCallback);
        FTBL.EV_stepSetFlags(11, 0);
        break;
    }
}

void WonSmoke(CwonWork *work) {
    static Vector3 CWON_WON_SMOKE_POSITION = {234.100006f, 98.3249969f, 464.5f};
    static Vector3 CWON_WON_SMOKE_VOLUME = {0.00499999989f, 0.0f, 0.00499999989f};
    static Vector3 CWON_WON_SMOKE_VELOCITY = {0.0f, 0.0f, 0.0f};
    static Vector3 CWON_WON_SMOKE_VARIANCE = {0.0188235305f, 0.0f, 0.0188235305f};

    int frame;
    if (FTBL.EV_authSceneCtrl(-1)) {
        switch (FTBL.EV_authSceneCtrl(-6)) {
        case 200:
            cwonSmoke = FTBL.CreateSmokeEffect(&CWON_WON_SMOKE_POSITION, &CWON_WON_SMOKE_VOLUME, 50, .08f, 0, 0);
            FTBL.SetSmokeEffectFadeFactor(cwonSmoke, 27);
            FTBL.SetSmokeEffectFalloff(cwonSmoke, 158);
            FTBL.SetParticleEffectColour(0, cwonSmoke, 0, 106, 104, 101);
            FTBL.SetParticleEffectInitialScale(0, cwonSmoke, .21568628f);
            FTBL.SetParticleEffectSize(0, cwonSmoke, 2.3137255f, 2.3137255f);
            FTBL.SetParticleEffectBirthRate(0, cwonSmoke, .03019608f);
            FTBL.SetParticleEffectVelocity(0, cwonSmoke, &CWON_WON_SMOKE_VELOCITY, &CWON_WON_SMOKE_VARIANCE);
            FTBL.ResumeParticleEffect(0, cwonSmoke);
            break;
        case 250:
            FTBL.SuspendParticleEffect(0, cwonSmoke);
            FTBL.FreeParticleEffect(0, cwonSmoke);
            break;
        }
    } else
        FTBL.HLib_DestroyCurrentTask();
}

void EndFail(CwonWork *work) {
    switch (work->authorState) {
    case 0:
        if (CwonIsNpcLoadComplete(&cwonNpcLoadTask)) {
            work->authorState++;
            CwonPrepareCharacters(CWON_LAST_CHASE_CHARACTERS);
            CwonStartAuthor(13, work->task, 0, CWON_ENDFAIL_CHARACTERS, 0);
            EVT_AT_SetSound(899, 20);
            EVT_AT_CharaDisp(WON, 0, 1);
        }
        break;
    case 1:
        work->authorState = 0;
        work->routePass = 3;
        FTBL.EV_stepSetFlags(11, 1);
        FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)CwonEndChaseCallback);
        break;
    }
}

void RunEnd(CwonWork *work) {
    static CwonRouteEntry CWON_END_ROUTE[] = {{1, 0, 455.5f, 0, 0, 1, 0, SHEN_NAME4('R', 'Y', 'O', '_')},
        {0, 0, 0.0f, 48241, 910, 0, 0, SHEN_NAME4('R', 'Y', 'O', '_')}, {1, 0, 244.699997f, 0, 0, 0, 0, SHEN_NAME4('R', 'Y', 'O', '_')},
        {0, 0, 0.0f, 33678, 910, 0, 1, SHEN_NAME4('R', 'Y', 'O', '_')}, {1, 0, 467.700012f, 0, 0, 1, 0, SHEN_NAME4('R', 'Y', 'O', '_')},
        {0, 0, 0.0f, 49152, 910, 0, 0, SHEN_NAME4('R', 'Y', 'O', '_')}, {1, 0, 256.899994f, 0, 0, 0, 0, SHEN_NAME4('R', 'Y', 'O', '_')},
        {3, 0, 0.0f, 0, 0, 0, 0, SHEN_NAME4('\x00', '\x00', '\x00', '\x00')}};

    int route = CwonAdvanceRoute(CWON_END_ROUTE, work->routeState), frames;
    if (route != work->routeState) {
        work->routeState = route;
        switch (route) {
        case 6:
            EVT_AT_SetCam(156, DUMMY, DUMMY, 0, 0, 0);
            break;
        case -1:
            CwonDisableGravity(RYO);
            FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(RYO), 0);
            EVT_AT_SetPos(RYO, CENTER, 257.35f, 101.57f, 470.66f, 0);
            frames = EVT_AT_SetMot(RYO, 0xf008, 0, 1, 0, 0);
            EVT_AT_SetPos(RYO, RYO, 0, -.9f, 0, frames - 1);
            EVT_AT_SetPos(RYO, RYO, 0, -.67f, 0, frames + 8);
            work->routeState = 0;
            work->routePass = 2;
            FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)LostEnd);
            FTBL.HLib_SetupTaskDelay(frames + 7);
            break;
        }
    }
}

void LostEnd(CwonWork *work) {
    switch (work->authorState) {
    case 0:
        work->authorState++;
        if (work->routePass == 2) {
            EVT_AT_SetCam(203, DUMMY, DUMMY, 0, 0, 50);
            CwonQueueMotionWait(work->task, RYO, 0);
        } else if (work->routePass == 0) {
            EVT_AT_SetCam(200, DUMMY, DUMMY, 0, 0, 0);
            EVT_AT_SetCam(201, DUMMY, DUMMY, 0, 0, 60);
            CwonQueueMotionWait(work->task, RYO, 167);
        } else if (work->routePass == 1) {
            FTBL.SetAllMapFieldDepthSlotsEnabled(0);
            EVT_AT_SetCam(205, DUMMY, DUMMY, 0, 0, 60);
            CwonQueueMotionWait(work->task, RYO, 0);
        }
        FTBL.camCtrlToggleSmoothing(0);
        EVT_AT_SetMot(RYO, 0x102a, -1, 1, 0, 0);
        EVT_AT_SetVoice("0011A023", 60);
        break;
    case 1:
        work->authorState = 0;
        switch (work->routePass) {
        case 0:
            FTBL.EV_stepSetFlags(9, 1);
            break;
        case 1:
            FTBL.EV_stepSetFlags(10, 1);
            break;
        case 2:
            FTBL.EV_stepSetFlags(11, 1);
            break;
        }
        FTBL.SetScreenFilmFade(20, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.spmFadeOut(30);
        FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)CwonEndChaseCallback);
        CwonQueueFadeWait(work->task);
        break;
    }
}

void CwonEndChaseCallback(CwonWork *work) {
    int i;
    if (!CwonIsNpcLoadComplete(&cwonNpcLoadTask))
        return;
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    EVT_UTL_FreeNPCChara(CWON_CHARACTERS);
    EVT_UTL_FreeNPCChara(CWON_RELEASE_CHARACTERS_1);
    EVT_UTL_FreeNPCChara(CWON_RELEASE_CHARACTERS_2);
    EVT_UTL_FreeNPCChara(CWON_AFTER_STREET_CHARACTERS);
    EVT_UTL_FreeNPCChara(CWON_RELEASE_CHARACTERS_3);
    EVT_UTL_FreeNPCChara(CWON_ADDITIONAL_CHARACTERS);
    EVT_UTL_FreeNPCChara(CWON_WAREHOUSE_CHARACTERS);
    if (g_currentMap != SHEN_NAME4('A', 'R', '0', '2')) {
        if ((int)cwonPackIndex != -1 && cwonPackState != 1)
            FTBL.EV_ControlPackageResources(0, 12, (void *)cwonPack, cwonPackIndex, cwonPackState);
        FTBL.EV_ControlPackageResources(1, 1, (void *)cwonPack, 0, 0);
    }
    FTBL.EV_NPC_NewLoadSw_0(1);
    switch ((short)work->temporarySlot) {
    case 0:
    case 1:
        FTBL.scenSetTaskFlags(0);
        break;
    case 2:
        CwonQueueLoadingTitle(SHEN_NAME4('A', 'R', 'X', 'B'), 20);
        break;
    default:
        FTBL.DebugLogToFileRemoved("titlectrl error\n");
        break;
    }
    CwonResetSmoke(work);
    if (g_currentMap == SHEN_NAME4('A', 'R', '0', '2')) {
        CwonRestoreMarketProps(0);
        FTBL.EV_ControlPackageResources(0, 6, "CWONBATL", 0, 0);
        FTBL.LoadCutsceneAndSoundProgramForArea();
        FTBL.SetPlayerControlEnabled(1);
        FTBL.camCtrlSetState(0);
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.EV_spmBeginSampleExecute(0);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        if (work->entry == 7) {
            FTBL.ToggleFlagOnFieldEntity(SHEN_NAME4('T', 'B', 'L', 'a'), 1);
            FTBL.ToggleFlagOnFieldEntity(SHEN_NAME4('T', 'B', 'L', 'b'), 1);
            FTBL.ToggleFlagOnFieldEntity(SHEN_NAME4('T', 'B', 'L', 'c'), 1);
            FTBL.ToggleFlagOnFieldEntity(SHEN_NAME4('T', 'B', 'L', 'd'), 1);
            FTBL.ToggleFlagOnFieldEntity(SHEN_NAME4('T', 'B', 'L', 'e'), 1);
        }
        if (work->routeState) {
            if (work->entry == 7) {
                EVT_AT_SetPos(RYO, CENTER, 259.6f, 101, 396, 0);
                EVT_AT_SetAngY(RYO, CENTER, 0, 0);
                FTBL.EV_stepSetFlags(13, 0);
            } else {
                EVT_AT_SetPos(RYO, CENTER, 225.5f, 99.33f, 402, 0);
                EVT_AT_SetAngY(RYO, CENTER, 0, 0);
                FTBL.EV_stepSetFlags(14, 0);
            }
            FTBL.LoadNPCRoute("NPC_AR02.BIN");
        } else {
            if (work->entry == 7)
                FTBL.EV_stepSetFlags(13, 1);
            else
                FTBL.EV_stepSetFlags(14, 1);
        }
    } else if (work->routeChoice) {
        if (!work->nextArea)
            work->nextArea = SHEN_NAME4('A', 'R', '0', '2');
        FTBL.EV_scenQueueLoad(g_actNumber, work->nextArea, work->routePass);
    } else if (work->entry) {
        if (work->routePass < 4)
            FTBL.EV_scenQueueLoad(g_actNumber, SHEN_NAME4('A', 'R', '0', '2'), work->routePass + 70);
        else if ((unsigned char)(work->routePass - 9) < 3)
            FTBL.EV_scenQueueLoad(g_actNumber, SHEN_NAME4('A', 'R', '0', '2'), work->routePass + 65);
    }
    FTBL.SetScenarioCharacterKillMode(CENTER, 1);
    FTBL.SetScenarioCharacterKillMode(DUMMY, 1);
    FTBL.SetScenarioCharacterKillMode(SHEN_NAME4('D', 'U', 'M', '1'), 1);
    CwonDestroyEscape(work);
    for (i = 0; i < 10; i++)
        CwonDestroyTemporary(work, i);
    CwonLoadAuthorPackages(0);
    if (work->dummyTask)
        FTBL.HLib_DestroyTaskInternal(work->dummyTask);
    for (i = 0; i < 4; i++)
        CwonDeleteMotion(work, i);
    if (work->routePass == 8)
        FTBL.EV_stepSetFlags(14, 0);
    FTBL.EV_authSceneCtrl(-10);
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
}

unsigned CwonPollQte(CwonWork *work, unsigned requested, HLibTaskCallback pass, HLibTaskCallback fail) {
    unsigned result = 0;
    if (!FTBL.GetShouldRenderIcons(3)) {
        result = FTBL.GetCurrentQteStatus();
        if (result) {
            work->authorState = 0;
            if ((result & 4095) == requested) {
                result = 1;
                if (pass) {
                    FTBL.HLib_SetCurrentTaskTo(pass);
                    pass(work);
                }
            } else {
                work->failures++;
                result = 65535;
                if (fail) {
                    FTBL.HLib_SetCurrentTaskTo(fail);
                    fail(work);
                }
            }
        }
    }
    return result;
}

unsigned CwonQueueTemporary(CwonWork *work, HLibTaskCallback callback, int delay) {
    unsigned slot = CwonFindTemporarySlot(work);
    CwonTemporaryWork *data;
    if (slot != -1) {
        data = CwonCreateTask(&work->temporaryTasks[slot], callback, sizeof(*data));
        if (data) {
            data->root = work;
            data->slot = slot;
            data->ticks = 0;
            if (delay)
                FTBL.HLib_SetupTimedTask(work->temporaryTasks[slot], delay);
            FTBL.DebugLogToFileRemoved("Make TmpTaskNo = %d addr:%x\n", slot, callback);
        } else
            slot = -1;
    }
    if (slot == -1)
        FTBL.DebugLogToFileRemoved("Tmp Taskwork error\n");
    return slot;
}

int CwonDestroyTemporary(CwonWork *work, unsigned slot) {
    int result = -1;
    if (slot < 10 && work->temporaryTasks[slot]) {
        FTBL.DebugLogToFileRemoved("Destry TmpTaskNo = %d\n");
        FTBL.HLib_DestroyTaskInternal(work->temporaryTasks[slot]);
        work->temporaryTasks[slot] = 0;
        result = 0;
    }
    return result;
}

HLibTask *CwonGetTemporaryTask(CwonTemporaryWork *data) {
    HLibTask *task = 0;
    if (data)
        task = data->root->temporaryTasks[data->slot];
    return task;
}

void *CwonGetTemporaryWork(CwonWork *work, unsigned slot) {
    void *data = 0;
    if (slot < 10 && work->temporaryTasks[slot])
        data = FTBL.HLib_GetTaskParameter(work->temporaryTasks[slot]);
    return data;
}

int CwonCountTemporaryTasks(CwonWork *work) {
    int count = 0, i;
    for (i = 0; i < 10; i++)
        if (work->temporaryTasks[i])
            count++;
    return count;
}

int CwonSetTemporaryTicks(CwonWork *work, unsigned slot, int ticks) {
    int result = 0;
    CwonTemporaryWork *data = CwonGetTemporaryWork(work, slot);
    if (data) {
        data->ticks = ticks;
        result = 1;
    }
    return result;
}

int CwonFindTemporarySlot(CwonWork *work) {
    int i;
    for (i = 0; i < 10; i++)
        if (!work->temporaryTasks[i])
            break;
    if (i == 10)
        i = -1;
    return i;
}

void CwonBirdCallback(CwonTemporaryWork *work) {
    if (!work)
        FTBL.DebugLogToFileRemoved("WkTask Error\n");
    else if (FTBL.EV_authSceneCtrl(-1)) {
        switch (work->ticks++) {
        case 336:
            work->ticks = 0;
            CwonEnablePositionedCharacter(BIRD('0'), 219.3f, 99.3f, 397.65f, 0, 0, 0);
            FTBL.EV_EnsureCharacterDraw(BIRD('0'));
            CwonEnablePositionedCharacter(BIRD('1'), 219.5f, 99.3f, 397.65f, 0, 165, 0);
            FTBL.EV_EnsureCharacterDraw(BIRD('1'));
            CwonEnablePositionedCharacter(BIRD('2'), 219.1f, 99.3f, 397.65f, 0, 200, 0);
            FTBL.EV_EnsureCharacterDraw(BIRD('2'));
            CwonEnablePositionedCharacter(BIRD('3'), 219.3f, 99.3f, 397.4499817f, 0, 100, 0);
            FTBL.EV_EnsureCharacterDraw(BIRD('3'));
            FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)CwonBirdFlightCallback);
            break;
        }
    } else
        CwonDestroyTemporary(work->root, work->slot);
}

void CwonBirdFlightCallback(CwonTemporaryWork *work) {
    int finished = 0;
    if (!work)
        FTBL.DebugLogToFileRemoved("WkTask Error\n");
    else {
        if (FTBL.EV_authSceneCtrl(-1) && work->ticks < 100) {
            work->ticks += 3;
            CwonAdvanceBird(BIRD('0'), .05f, .07f, work->ticks);
            if (work->ticks > 15) {
                CwonAdvanceBird(BIRD('1'), .045f, .08f, work->ticks + 1);
                CwonAdvanceBird(BIRD('2'), .04f, .07f, work->ticks + 2);
            } else {
                CwonAdvanceBird(BIRD('1'), .045f, 0, work->ticks + 1);
                CwonAdvanceBird(BIRD('2'), .04f, 0, work->ticks + 2);
            }
            if (work->ticks > 20)
                CwonAdvanceBird(BIRD('3'), .035f, .09f, work->ticks + 3);
            else
                CwonAdvanceBird(BIRD('3'), .035f, 0, work->ticks + 3);
        } else
            finished = 1;
        if (finished) {
            FTBL.EV_DisableCharacterDisplay(BIRD('0'));
            FTBL.EV_DisableCharacterDisplay(BIRD('1'));
            FTBL.EV_DisableCharacterDisplay(BIRD('2'));
            FTBL.EV_DisableCharacterDisplay(BIRD('3'));
            FTBL.EV_ToggleCharacterFlag(BIRD('0'));
            FTBL.EV_ToggleCharacterFlag(BIRD('1'));
            FTBL.EV_ToggleCharacterFlag(BIRD('2'));
            FTBL.EV_ToggleCharacterFlag(BIRD('3'));
            CwonDestroyTemporary(work->root, work->slot);
        }
    }
}

void CwonAdvanceBird(shen_name name, float speed, float height, int tick) {
    CwonDisableGravity(name);
    CwonMoveBackward(name, speed);
    EVT_AT_SetPos(name, name, 0, height, 0, 0);
}

void CwonSetPipeStep(int step) {
    cwonPipeStep = step;
}

void CwonPipeCallback(CwonTemporaryWork *work) {
    static const int parts[12] = {152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163};
    Vector3 position;
    Vector3i rotation;
    int frame, track;
    if (!work)
        FTBL.DebugLogToFileRemoved("WkTask Error\n");
    else {
        if (++work->ticks < 200) {
            track = 0;
            frame = work->ticks * cwonPipeStep;
            for (; track < 12; track++) {
                FTBL.dynaReadEvaluate(track, frame, &position, &rotation, "PIPE");
                position.x *= 10;
                position.y *= 10;
                position.z *= 10;
                FTBL.scenApplyPosOrRotToChildId(SHEN_NAME4('P', 'I', 'P', '1'), parts[track], &position, &rotation, 0);
            }
        } else
            CwonDestroyTemporary(work->root, work->slot);
    }
}

void CwonPlayerDustCallback(CwonTemporaryWork *work) {
    Vector3 position, volume, velocity;
    if (!work)
        FTBL.DebugLogToFileRemoved("WkTask Error\n");
    else {
        FTBL.GetCharaPosition(RYO, 31, &position, 0);
        volume.x = volume.z = .005f;
        volume.y = 0;
        if (work->ticks) {
            if (work->root->smoke != -1)
                FTBL.SetParticleEffectPosition(0, work->root->smoke, &position, &volume);
            else
                CwonDestroyTemporary(work->root, work->slot);
        } else {
            work->ticks++;
            velocity.x = velocity.y = velocity.z = 0;
            if (work->root->smoke == -1)
                work->root->smoke = CwonCreateDust(&position, &volume, &velocity);
        }
    }
}

void CwonPlayerOffsetDustCallback(CwonTemporaryWork *work) {
    Vector3 position, volume, velocity;
    if (!work)
        FTBL.DebugLogToFileRemoved("WkTask Error\n");
    else {
        FTBL.GetCharaPosition(RYO, 31, &position, 0);
        position.x -= .5f;
        volume.x = volume.z = .005f;
        volume.y = 0;
        if (work->ticks) {
            if (work->root->smoke != -1)
                FTBL.SetParticleEffectPosition(0, work->root->smoke, &position, &volume);
            else
                CwonDestroyTemporary(work->root, work->slot);
        } else {
            work->ticks++;
            velocity.x = velocity.y = velocity.z = 0;
            if (work->root->smoke == -1)
                work->root->smoke = CwonCreateDust(&position, &volume, &velocity);
        }
    }
}

void CwonCreateSmokeAt(CwonWork *work, const Vector3 *position) {
    Vector3 volume, velocity;
    if (work->smoke == -1) {
        volume.x = volume.z = .005f;
        volume.y = 0;
        velocity.x = velocity.y = velocity.z = 0;
        work->smoke = CwonCreateDust(position, &volume, &velocity);
    }
}

void CwonResetSmoke(CwonWork *work) {
    if (work->smoke != -1) {
        FTBL.SuspendParticleEffect(0, work->smoke);
        FTBL.FreeParticleEffect(0, work->smoke);
        work->smoke = -1;
    }
}

void CwonLoadAudioForPhase(CwonWork *work, int phase) {
    if (work->audioScene == phase)
        return;
    switch (phase) {
    case 1:
        FTBL.EV_LoadCutsceneFromCurrentScene("0011E000.smp");
        FTBL.EV_LoadAudio("0011E000.sbt", 0);
        break;
    case 2:
        FTBL.EV_LoadCutsceneFromCurrentScene("0011E000.smp");
        FTBL.EV_LoadAudio("0011E000.sbt", 1);
        break;
    case 3:
        FTBL.EV_LoadCutsceneFromCurrentScene("0012E011.smp");
        FTBL.EV_LoadAudio("0012E011.sbt", 0);
        break;
    case 4:
        FTBL.EV_LoadCutsceneFromCurrentScene("0012E011.smp");
        FTBL.EV_LoadAudio("0012E011.sbt", 1);
        break;
    case 5:
        FTBL.EV_LoadCutsceneFromCurrentScene("0012E011.smp");
        FTBL.EV_LoadAudio("0012E011.sbt", 2);
        break;
    }
    FTBL.EV_spmBeginSampleExecute(0);
    work->audioScene = phase;
}

void CwonReleaseAndLoadPackages(HLibTask *task, int freeIndex, int freeSlot, int loadIndex, int loadSlot) {
    FTBL.DebugLogToFileRemoved("SWAP\n");
    if (task)
        FTBL.HLib_DestroyTaskInternal(task);
    if (freeIndex != -1)
        FTBL.EV_ControlPackageResources(0, 12, (void *)cwonPack, freeIndex, freeSlot);
    if (loadIndex != -1)
        FTBL.EV_ControlPackageResources(0, 8, (void *)cwonPack, loadIndex, loadSlot);
    cwonPackIndex = loadIndex;
    cwonPackState = loadSlot;
}

void CwonAuthNullsub(void) {
}

void CwonInitializeWaveLighting(void) {
}

#endif

#else
#include "s2/date_time.hpp"
#include "s2/scripts/cwon/area_cwon.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/debug.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/scene_data.hpp"
#include "s2/sound_program.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include <array>
#include <bit>
#include <cstring>
namespace shenmue::s2 {
PakAfsPartition *cwonPack;
HLib::Task *cwonDynamixTask;
HLib::Task *cwonNpcLoadTask;
std::int32_t cwonWaveDisabled, cwonSpringDisabled, cwonPipeStep, cwonSmoke, cwonPackIndex, cwonPackState;
Vector3 cwonSmokePosition, cwonSmokeVolume, cwonSmokeVelocity, cwonSmokeVariance;
const shen_name CWON_CHARACTERS[]{SHEN_NAME(WON_), CWON_INVALID};
const shen_name CWON_ADDITIONAL_CHARACTERS[]{SHEN_NAME(SAM_), SHEN_NAME(LLY_), CWON_INVALID};
const shen_name CWON_AUTHOR_CHARACTERS[]{CWON_RYO, CWON_WON, shen_name{}};
const shen_name CWON_AUTHORS[]{SHEN_NAME(AT00), SHEN_NAME(AT01), SHEN_NAME(AT02), SHEN_NAME(AT03), SHEN_NAME(AT04), SHEN_NAME(AT05), SHEN_NAME(AT06),
    SHEN_NAME(AT07), SHEN_NAME(AT08), SHEN_NAME(AT09), SHEN_NAME(AT10), SHEN_NAME(AT11), SHEN_NAME(AT12), SHEN_NAME(AT13), SHEN_NAME(AT14), SHEN_NAME(AT15),
    SHEN_NAME(AT16), SHEN_NAME(AT17), SHEN_NAME(AT18), SHEN_NAME(AT19), SHEN_NAME(AT20), SHEN_NAME(AT21), SHEN_NAME(AT22), SHEN_NAME(AT23), SHEN_NAME(AT24),
    SHEN_NAME(AT25), SHEN_NAME(AT26), SHEN_NAME(AT27), SHEN_NAME(AT28), SHEN_NAME(AT29)};
namespace {
std::array<std::byte, 0x100> distanceKernel, firstKernel, secondKernel;
constexpr Vector3 playerPositions[]{{232.0f, 100.21299743652344f, 374.6000061035156f}, {216.0f, 99.31410217285156f, 409.0f},
    {256.55999755859375f, 100.9000015258789f, 381.8999938964844f}, {256.55999755859375f, 100.9000015258789f, 381.8999938964844f},
    {263.29998779296875f, 100.9000015258789f, 419.0f}, {253.85000610351562f, 100.0f, 426.25f}, {239.3000030517578f, 98.45999908447266f, 466.3500061035156f},
    {260.0f, 101.0f, 397.5f}, {207.60000610351562f, 99.0999984741211f, 390.6000061035156f}};
constexpr float playerYaws[]{270, 310, 185, 185, 180, 160, 0, 45, 0};
void ApplyLighting() {
    SetWeatherUpdateEnabledTo(1);
    RefreshWeatherForCurrentTime(0);
    EV_UpdateWeather();
    DateTime date;
    shenGetCurrentDateTime(date);
    if (static_cast<std::uint8_t>(date.hour - 5) > 13U) {
        EV_lightLoad(SHEN_NAME(0000));
        PushGlobalLightingProgram();
        CheckIsArgEquZero(1);
        EV_SetObjectFlagByIndex(12, 1);
        EV_SetObjectFlagByIndex(13, 0);
    } else {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_SetObjectFlagByIndex(12, 0);
        EV_SetObjectFlagByIndex(13, 1);
    }
}
} // namespace

void AreaCWONInit() {
    cwonSmokePosition = {std::bit_cast<float>(0x436a199aU), std::bit_cast<float>(0x42c4a666U), std::bit_cast<float>(1139294208U)};
    cwonPipeStep = 0;
    cwonPack = nullptr;
    cwonPackState = 0;
    cwonPackIndex = 0;
    cwonDynamixTask = nullptr;
    cwonNpcLoadTask = nullptr;
    cwonWaveDisabled = 0;
    cwonSpringDisabled = 0;
    cwonSmoke = 0;
    cwonSmokeVolume = {std::bit_cast<float>(0x3ba3d70aU), 0, std::bit_cast<float>(0x3ba3d70aU)};
    cwonSmokeVelocity = {};
    cwonSmokeVariance = {std::bit_cast<float>(0x3c9a33ceU), 0, std::bit_cast<float>(0x3c9a33ceU)};
    distanceKernel.fill(std::byte{});
    firstKernel.fill(std::byte{});
    secondKernel.fill(std::byte{});
}

void AreaCWONMain(std::uintptr_t argument) {
    SetPlayerControlEnabled(0);
    SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    nullsub();
    shenSetClockAndCompassRenderTo(1);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(CwonMainCallback, 4, 11, sizeof(CwonEventWork), CWON_TASK);
    if (!task)
        return;
    auto *work = static_cast<CwonEventWork *>(HLib::GetTaskParameter(task));
    if (!work)
        return;
    std::memset(work, 0, sizeof(*work));
    std::int32_t entry = static_cast<std::int32_t>(argument);
    if (g_currentMap.name == SHEN_NAME(AR02)) {
        EV_SetNewSceneIndex();
        SetAreaNpcLowQualityHands(false);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 2228, 1);
    } else {
        entry = g_entryPoint;
        EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("MPK00"), 0);
    }
    if (entry >= 9) {
        EV_debug_log("CWON\\eCWON_et_ChaseWon.c", 422, "Entry Error\n");
        return;
    }
    CwonInitialize(work, task, entry);
    CwonInitializePlayer(work);
    cwonNpcLoadTask = Queue_EVT_UTL_LoadNPCChara_3(CWON_CHARACTERS, task);
}
const ScriptAreaDefinition areaCWONScriptDefinition{CWON_AREA, 0, AreaCWONInit, AreaCWONMain};
void CwonMainCallback(void *data) {
    auto *work = static_cast<CwonEventWork *>(data);
    if (!work->startCallback) {
        HLib::DestroyCurrentTask();
        return;
    }
    mtwkSetRequestedRootMode(scenFindMotionwork(CWON_RYO), 3);
    SetPlayerGameFlags(static_cast<PlayerGameFlagMode>(2), nullptr);
    CwonPrepareCharacters(CWON_CHARACTERS);
    HLib::SetCurrentTaskTo(work->startCallback);
    SetAreaNpcCharactersVisible(false);
    SetAreaNpcLowQualityHands(false);
    if (work->entry > 6)
        cwonNpcLoadTask = Queue_EVT_UTL_LoadNPCChara_3(CWON_ADDITIONAL_CHARACTERS, HLib::GetCurrentTask());
    work->dummyTask = HLib::EnqueueTaskWithoutParameter(CwonDummyControlCallback, 4, 11, SHEN_NAME(dctl));
}
void CwonInitialize(CwonEventWork *work, HLib::Task *task, std::int32_t entry) {
    constexpr HLib::TaskCallback starts[]{nullptr, CwonRootStreetIntroCallback, CwonRootMarketIntroCallback, CwonRootMarketIntroCallback,
        CwonRootWarehouseIntroCallback, CwonRootWarehouseIntroCallback, CwonInitializeBattleCallback, CwonInitializeBattleCallback,
        CwonInitializeBattleCallback};
    EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    nullsub();
    for (shen_name name : {CWON_CENTER, CWON_DUMMY, CWON_SECOND_DUMMY}) {
        CreateEventCharacter(CharacterCreationHighModel, nullptr, name);
        EV_AT_SetPos(name, EventTransformFlags::World | EventTransformFlags::AllAxes, {});
        EV_AT_SetAngY(name, EventTransformFlags::AllAxes, {});
    }
    EV_stepSetFlags(0, 5);
    ApplyLighting();
    if (g_currentMap.name == CWON_AREA.name) {
        InitializeWaterTextureAnimation(6);
        InitializeSpringTextureAnimation(24);
        GenerateFFT(2.0f);
        FillFftDistance(FillFftKernel(10.0f, GenerateDistanceFalloffRowMajor(2.0820000171661377f, distanceKernel)));
        void *first = FillFftKernel(0.0021875000093132257f, GenerateDistanceFalloffKernel8x8(0.9520000219345093f, firstKernel));
        void *second = FillFftKernel(0.0035250000655651093f, GenerateDistanceFalloffKernel8x8(0.9399999976158142f, secondKernel));
        ApplyFft(second, first);
        FinalizeFft(0.0010000000474974513f, 0.0010000000474974513f);
        SetWaveLighting(0.1f, {0.0598f, 0.9768f, 0.2054f});
        HLib::EnqueueTaskWithoutParameter(CwonWaveCallback, 3, 2, SHEN_NAME(wave));
        HLib::EnqueueTaskWithoutParameter(CwonSpringCallback, 3, 2, SHEN_NAME(fntn));
        EV_AssignLightParams(1, 15);
        EV_AssignLightParams(1, 16);
    }
    work->entry = static_cast<std::uint8_t>(entry);
    work->task = task;
    work->temporarySlot = 0;
    work->startCallback = starts[entry];
    work->smoke = -1;
    cwonWaveDisabled = 0;
    cwonSpringDisabled = 0;
    for (std::int32_t i = 0; i < 4; ++i)
        if (work->motions[i]) {
            EV_debug_log("CWON\\eCWON_et_ChaseWon.c", 633, "Motion Delete %d\n", i);
            DeleteMotion(work->motions[i]);
            work->motions[i] = 0;
        }
    work->authorState = static_cast<std::uint8_t>(work->entry - 2) <= 6U ? -1 : 0;
    EV_FreeAuthorPak(1, "0011");
    EV_FreeAuthorPak(1, "0012");
    if (g_currentMap.name != SHEN_NAME(AR02)) {
        cwonPack = reinterpret_cast<PakAfsPartition *>(EV_pakUtils(PakCategory::Partition, PakPartitionOp::Mount,
            reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)), reinterpret_cast<std::uintptr_t>("CWONPACK.afs")));
        cwonPackState = -1;
        cwonPackIndex = -1;
    }
    EV_spmBeginSampleExecute();
}
void CwonInitializePlayer(CwonEventWork *work) {
    CwonStartPlayerAt(work->entry);
    mtwkSetEnabled(scenFindMotionwork(CWON_RYO), 1);
    EV_SetCharacterAlphaEnabled(CWON_RYO, false);
    SetScenarioCharacterRadius(CWON_RYO, 20.0f);
    Character *player = ctrlGetScenarioChara(CWON_RYO);
    SetFaceFlagsForChara_0(player, FacialNodeReplacement);
    SetHighModelAndFaceFlagsForChara(player, FacialExpressionId::Angry, 1, 1);
    SetLowQualityHands(player);
    SetHighModelXYZ(player, 1, 0, 0);
    SetHighModelXYZ(player, 1, 1, 0);
}
void CwonStartPlayerAt(std::int32_t entry) {
    EventSetPosition position{0, CWON_RYO, CWON_CENTER, playerPositions[entry], 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{0, CWON_RYO, CWON_CENTER, playerYaws[entry], 0};
    EVT_AT_SetAngY_Callback(&angle);
}
void CwonPrepareCharacters(const shen_name *names) {
    for (; names->name != CWON_INVALID.name; ++names)
        if (DoesScenarioCharacterExist(*names)) {
            EV_EnableCharacterDisplay(*names);
            EV_EnsureCharacterDraw(*names);
            Character *character = ctrlGetScenarioChara(*names);
            SetFaceFlagsForChara_0(character, FacialNodeReplacement);
            SetHighModelAndFaceFlagsForChara(character, FacialExpressionId::Normal, 1, 1);
            mtwkSetRequestedRootMode(scenFindMotionwork(*names), 3);
            if (names->name == CWON_WON.name) {
                mtwkSetRequestedRootMode(scenFindMotionwork(*names), 0);
                SetScenarioCharacterType(*names, CharacterTypeStencilShadow);
            }
        }
}
void CwonWaveCallback(void *) {
    if (cwonWaveDisabled)
        return;
    GetInterpolatedWeatherValues();
    const auto colours = GetWeatherWaveColours();
    const auto specular = colours[0], diffuse = colours[1];
    BuildWaveLightingTable(1, 1, 1,
        {float((diffuse >> 16) & 255U) / 255 * .75f, float((diffuse >> 8) & 255U) / 255 * .75f, float(diffuse & 255U) / 255 * .8999999761581421f},
        {float((specular >> 16) & 255U) / 255 * .31f, float((specular >> 8) & 255U) / 255 * .31f, float(specular & 255U) / 255 * .3f}, 42.540000915527344f, 1);
    SetMapModelSlotEnabled(15, 0);
    SetMapModelSlotEnabled(16, 0);
    AdvanceWaterTextureAnimation();
}
void CwonSpringCallback(void *) {
    if (!cwonSpringDisabled) {
        SetMapModelSlotEnabled(17, 0);
        SetMapModelSlotEnabled(18, 0);
        AdvanceSpringTextureAnimation();
    }
}
} // namespace shenmue::s2

#endif
