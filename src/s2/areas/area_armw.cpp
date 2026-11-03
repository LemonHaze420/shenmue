#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_armw.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void RunArmwEvent(int argument)
{
    FTBL.SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    FTBL.SetPlayerControlEnabled(0);
    FTBL.SetLoadingBusy();
    armwArgument = argument;
    FTBL.SetWeatherUpdateEnabledTo(1);
    FTBL.RefreshWeatherForCurrentTime(0);
    FTBL.UpdateWeather();
    QueueClearNpcs(ArmwStart, 0);
}
#elif defined(SHENMUE_EVENT_ARMW_CROWD)
const unsigned int armwCrowdMotions[15] = {9025, 9026, 9027, 9028, 9029, 9030, 9031, 9032, 9033, 9034, 9035, 9036, 9037, 9038, 9039};

void ArmwPlaceCrowd(const shen_name *names)
{
    ArmwCrowdPosition positions[9] = {{1.8f, 0.0f, -0.8f, 112.5f}, {1.3f, 0.0f, -1.3f, 135.0f}, {0.4f, 0.0f, -1.4f, 180.0f}, {-2.1f, 0.0f, -0.4f, 270.0f},
        {-1.3f, 0.0f, 0.5f, 300.0f}, {0.5f, 0.0f, 1.8f, 22.5f}, {-1.4f, 0.0f, -0.7f, 230.0f}, {2.2f, 0.0f, 0.9f, 67.5f}, {0.0f, 0.0f, 0.0f, 120.0f}};
    ArmwCrowdPosition *position = positions;
    while (*names != ARMW_INVALID) {
        EVT_AT_SetPos(*names, ARMW_BASE, position->x, position->y, position->z, 0);
        EVT_AT_SetAngY(*names, ARMW_BASE, position->yaw, 0);
        EVT_AT_SetMot(*names++, armwCrowdMotions[FTBL.shenGetRandomInt32(15)], 0.0f, 1.0f, ARMW_MOTION_FLAGS, 0);
        position++;
    }
}
#else
const float armwBases[12][4] = {{205.0f, 100.22f, 320.0f, 120.0f}, {234.0f, 100.22f, 348.0f, 90.0f}, {247.9f, 100.22f, 363.1f, 180.0f}, {299.4f, 107.87f, 329.3f, 120.0f}, {300.6f, 107.87f, 331.3f, 120.0f}, {301.8f, 107.87f, 333.4f, 120.0f}, {240.3f, 100.0f, 132.3f, 180.0f}, {237.6f, 100.0f, 132.3f, 180.0f}, {234.9f, 100.0f, 132.3f, 180.0f}, {232.2f, 100.0f, 132.3f, 180.0f}, {229.5f, 100.0f, 132.3f, 180.0f}, {226.8f, 100.0f, 132.3f, 180.0f}};
const int armwExits[12][2] = {{0, 20}, {0, 21}, {0, 22}, {0, 0}, {0, 0}, {0, 0}, {0, 20}, {0, 21}, {0, 22}, {0, 0}, {0, 0}, {0, 0}};
const shen_name armwReferees[12][2] = {{ARMW_GAU, 0}, {ARMW_WD3, 0}, {ARMW_AD2, 0}, {ARMW_SD1, 0}, {ARMW_SD1, 0}, {ARMW_SD1, 0}, {ARMW_WD1, 0}, {ARMW_WD1, 0}, {ARMW_WD1, 0}, {ARMW_WD1, 0}, {ARMW_WD1, 0}, {ARMW_WD1, 0}};
const int armwFlagIds[12] = {3, 4, 5, 6, 7, 8, 4, 3, 5, 7, 6, 8};
const ArmwOpponent armwOpponents[60] = {
    {ARMW_HONG, 3, 1, 6, 60, 2, 15, 100, 0, 0, 0, 0, 0},
    {ARMW_HONG, 4, 2, 7, 90, 2, 15, 100, 0, 0, 0, 0, 0},
    {ARMW_HONG, 5, 3, 7, 120, 2, 15, 100, 0, 0, 0, 0, 0},
    {ARMW_HONG, 5, 3, 8, 180, 2, 15, 100, 0, 0, 0, 0, 0},
    {ARMW_HONG, 6, 4, 9, 250, 2, 15, 100, 0, 0, 0, 0, 0},
    {ARMW_SF8, 5, 2, 5, 60, 0, 10, 50, 50, 0, 0, 0, 0},
    {ARMW_SF8, 5, 3, 6, 60, 0, 10, 50, 50, 0, 0, 0, 0},
    {ARMW_SF8, 6, 3, 6, 60, 0, 10, 25, 75, 0, 0, 0, 0},
    {ARMW_SF8, 6, 4, 7, 90, 0, 10, 25, 75, 0, 0, 0, 0},
    {ARMW_SF8, 7, 4, 7, 90, 0, 10, 25, 75, 0, 0, 0, 0},
    {ARMW_SFC, 7, 3, 5, 120, 1, 4, 50, 50, 0, 0, 0, 0},
    {ARMW_SFC, 8, 3, 5, 240, 1, 4, 50, 0, 50, 0, 0, 0},
    {ARMW_SFC, 8, 4, 6, 460, 1, 4, 33, 33, 33, 0, 0, 0},
    {ARMW_SFC, 9, 5, 6, 580, 1, 4, 25, 25, 50, 0, 0, 0},
    {ARMW_SFC, 9, 5, 7, 600, 1, 4, 20, 40, 40, 0, 0, 0},
    {ARMW_KZN, 4, 3, 5, 450, 3, 7, 50, 50, 0, 0, 0, 0},
    {ARMW_KZN, 5, 4, 6, 400, 3, 7, 50, 0, 50, 0, 0, 0},
    {ARMW_KZN, 5, 4, 7, 350, 3, 7, 50, 0, 0, 50, 0, 0},
    {ARMW_KZN, 6, 5, 7, 300, 3, 7, 33, 33, 33, 0, 0, 0},
    {ARMW_KZN, 6, 5, 8, 250, 3, 7, 25, 25, 25, 25, 0, 0},
    {ARMW_SSY, 5, 3, 5, 150, 4, 4, 25, 25, 25, 25, 512, 0},
    {ARMW_SSY, 6, 3, 6, 210, 4, 4, 25, 25, 25, 25, 512, 0},
    {ARMW_SSY, 6, 3, 6, 270, 4, 4, 25, 25, 25, 25, 513, 0},
    {ARMW_SSY, 7, 4, 7, 450, 4, 4, 25, 25, 25, 25, 514, 0},
    {ARMW_SSY, 8, 4, 8, 600, 4, 4, 25, 25, 25, 25, 768, 0},
    {ARMW_SF2, 5, 3, 5, 90, 5, 3, 25, 25, 25, 25, 514, 0},
    {ARMW_SF2, 6, 3, 6, 150, 5, 3, 25, 25, 25, 25, 768, 0},
    {ARMW_SF2, 6, 3, 6, 210, 5, 3, 25, 25, 25, 25, 769, 0},
    {ARMW_SF2, 7, 4, 7, 300, 5, 3, 25, 25, 25, 25, 770, 0},
    {ARMW_SF2, 8, 4, 8, 450, 5, 3, 25, 25, 25, 25, 771, 0},
    {ARMW_FRS, 2, 1, 4, 60, 0, 20, 90, 10, 0, 0, 0, 0},
    {ARMW_FRS, 3, 2, 45, 60, 0, 20, 80, 20, 0, 0, 0, 0},
    {ARMW_FRS, 4, 2, 5, 60, 0, 20, 70, 30, 0, 0, 0, 0},
    {ARMW_FRS, 5, 3, 6, 90, 0, 15, 60, 40, 0, 0, 0, 0},
    {ARMW_FRS, 6, 3, 7, 90, 0, 15, 50, 50, 0, 0, 0, 0},
    {ARMW_HONG, 4, 2, 7, 90, 2, 9, 80, 10, 10, 0, 0, 0},
    {ARMW_HONG, 5, 3, 8, 120, 2, 9, 70, 15, 15, 0, 0, 0},
    {ARMW_HONG, 6, 3, 8, 180, 2, 9, 60, 20, 20, 0, 0, 0},
    {ARMW_HONG, 6, 4, 9, 250, 2, 9, 50, 25, 25, 0, 0, 0},
    {ARMW_HONG, 7, 4, 10, 350, 2, 9, 40, 30, 30, 0, 0, 0},
    {ARMW_MHK, 5, 3, 5, 90, 1, 15, 10, 30, 10, 50, 0, 0},
    {ARMW_MHK, 5, 4, 6, 150, 1, 15, 50, 10, 30, 10, 0, 0},
    {ARMW_MHK, 5, 4, 6, 210, 1, 15, 10, 50, 10, 30, 0, 0},
    {ARMW_MHK, 6, 5, 7, 270, 1, 15, 30, 10, 50, 10, 0, 0},
    {ARMW_MHK, 6, 5, 7, 350, 1, 15, 25, 25, 25, 25, 0, 0},
    {ARMW_DML, 5, 4, 6, 150, 4, 7, 25, 25, 25, 25, 512, 0},
    {ARMW_DML, 6, 4, 6, 210, 4, 7, 25, 25, 25, 25, 513, 0},
    {ARMW_DML, 6, 5, 7, 270, 4, 7, 25, 25, 25, 25, 514, 0},
    {ARMW_DML, 7, 5, 7, 450, 4, 7, 25, 25, 25, 25, 515, 0},
    {ARMW_DML, 8, 6, 8, 600, 4, 7, 25, 25, 25, 25, 516, 0},
    {ARMW_MSH, 6, 4, 6, 150, 5, 5, 25, 25, 25, 25, 768, 0},
    {ARMW_MSH, 6, 5, 7, 210, 5, 5, 25, 25, 25, 25, 769, 0},
    {ARMW_MSH, 7, 5, 7, 270, 5, 5, 25, 25, 25, 25, 770, 0},
    {ARMW_MSH, 7, 6, 8, 450, 5, 5, 25, 25, 25, 25, 771, 0},
    {ARMW_MSH, 8, 6, 8, 600, 5, 5, 25, 25, 25, 25, 772, 0},
    {ARMW_DGY, 6, 5, 7, 150, 6, 3, 25, 25, 25, 25, 1024, 0},
    {ARMW_DGY, 7, 5, 7, 210, 6, 3, 25, 25, 25, 25, 1025, 0},
    {ARMW_DGY, 7, 6, 8, 270, 6, 3, 25, 25, 25, 25, 1026, 0},
    {ARMW_DGY, 8, 6, 8, 450, 6, 3, 25, 25, 25, 25, 1027, 0},
    {ARMW_DGY, 8, 7, 9, 600, 6, 3, 25, 25, 25, 25, 1028, 0},
};
const short armwOrder[12][6] = {{0, 1, 2, 3, 4, -1}, {5, 6, 7, 8, 9, -1}, {10, 11, 12, 13, 14, -1}, {15, 16, 17, 18, 19, -1}, {20, 21, 22, 23, 24, -1}, {25, 26, 27, 28, 29, -1}, {30, 31, 32, 33, 34, -1}, {35, 36, 37, 38, 39, -1}, {40, 41, 42, 43, 44, -1}, {45, 46, 47, 48, 49, -1}, {50, 51, 52, 53, 54, -1}, {55, 56, 57, 58, 59, -1}};
const short armwRotations[7][2][3] = {{{0, 1, 2}, {4, 1, 3}}, {{3, 4, 2}, {1, 0, 2}}, {{2, 1, 0}, {3, 2, 4}}, {{3, 4, 0}, {2, 3, 1}}, {{2, 3, 1}, {0, 1, 4}}, {{4, 3, 0}, {1, 3, 2}}, {{2, 1, 3}, {4, 2, 0}}};
shen_name armwCharacters[15] = {ARMW_RYO, ARMW_INVALID};
shen_name armwTutorialCharacters[2] = {ARMW_SHIYO, ARMW_INVALID};
int armwBets[12][5] = {{10, 20, 30, 40, 50}, {20, 30, 40, 50, 60}, {40, 50, 60, 70, 80}, {30, 40, 50, 60, 70}, {50, 60, 70, 80, 90}, {60, 70, 80, 90, 100}, {10, 20, 30, 40, 50}, {20, 30, 40, 50, 60}, {30, 40, 50, 60, 70}, {40, 50, 60, 70, 80}, {50, 60, 70, 80, 90}, {60, 70, 80, 90, 100}};
int armwStarted = 0;
int armwStartFrames = 0;
unsigned int armwIntroductionVariant __attribute__((section(".bss")));
float armwQtePenalty __attribute__((section(".bss")));
int armwQteButton __attribute__((section(".bss")));
int armwQteSpeaker __attribute__((section(".bss")));
int armwHandMotion __attribute__((section(".bss")));
int armwArgument __attribute__((section(".bss")));
short armwLocation __attribute__((section(".bss")));

void ArmwStart(void *parameter)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(ArmwMainCallback, 4, 11, sizeof(ArmwWork), ARMW_TASK);
    ArmwWork *work = (ArmwWork *)FTBL.HLib_GetTaskParameter(task);
    memset(work, 0, sizeof(ArmwWork));
    work->self = task;
    work->phase = 0;
    if (g_currentMap == ARMW_AREA) {
        if ((unsigned int)g_entryPoint > 89)
            armwLocation = ((unsigned int)g_entryPoint - 90) % 12;
        else
            armwLocation = (unsigned int)g_entryPoint % 12;
    } else
        armwLocation = armwArgument;
    work->step = 0;
    ArmwSelectCharacters(work);
    FTBL.SetWeatherUpdateEnabledTo(1);
    FTBL.RefreshWeatherForCurrentTime(0);
    FTBL.UpdateWeather();
    work->characters = Queue_EVT_AT_ReadChr(task, armwCharacters);
    FTBL.SetLoadingBusy();
    FTBL.EV_EventSetUpHelpIcon_0(65, 0);
    EventDebugRegister(3, ArmwDebugExit, work, 3000000);
}

void ArmwInitializeResources(ArmwWork *work)
{
    const char *path;
    int i;
    armwHandMotion = -1;
    FTBL.EV_SetNewSceneIndex();
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(ARMW_RYO));
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(ARMW_RYO), 1, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(ARMW_RYO), 1, 1, 0);
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(work->referee));
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(work->referee), 1, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(work->referee), 1, 1, 0);
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(work->opponent));
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(work->opponent), 1, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(work->opponent), 1, 1, 0);
    if (g_currentMap == ARMW_AR02)
        FTBL.LoadNPCRoute("NPC_ARAW.BIN");
    FTBL.GetCharacterFaceCacheIndex(FTBL.ctrlGetCharaByName(ARMW_RYO));
    FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(ARMW_RYO), 1);
    FTBL.qteBattleUtil(8, 45, 6, 18, 0);
    FTBL.qteBattleUtil(9, 0, 0, 0, 0);
    FTBL.qteBattleUtil(10, 0, 0, 0, 0);
    FTBL.qteBattleUtil(1, 0, 0, 0, 0);
    FTBL.qteBattleUtil(0, 0, 0, 0, 0);
    FTBL.qteBattleUtil(2, 0, 0, 0, 0);
    if (g_currentMap == ARMW_QTB4)
        FTBL.EV_FreeAuthorPak(1, "0368");
    else
        FTBL.EV_FreeAuthorPak(1, "0008");
    ArmwSetVoicePrefixes(work->referee, work->opponent);
    path = FTBL.Filepath_Gen(0);
    FTBL.EV_ControlPackageResources(0, 0, path, (int)"ARMW", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, "ARMW", 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "ARMW", 0, 0);
    FTBL.EV_LoadCutsceneFromCurrentScene("0008A000");
    FTBL.CreateEventCharacter(3, 0, ARMW_BASE);
    if (work->opponent == ARMW_SFC || work->opponent == ARMW_DGY || work->opponent == ARMW_MHK)
        armwOpponentVariant = 2;
    else if (FTBL.GetMotionRigBoneId(FTBL.scenFindMotionwork(work->opponent)) == 7)
        armwOpponentVariant = 0;
    else if (FTBL.GetMotionRigBoneId(FTBL.scenFindMotionwork(work->opponent)) == 9)
        armwOpponentVariant = 1;
    else
        armwOpponentVariant = 3;
    if (FTBL.EV_GetShenmueFlag(2) && !armwLocation) {
        for (i = 0; i < 6; i++)
            FTBL.EV_SetPlayerFlag(16, armwFlagIds[i], 0);
        path = FTBL.Filepath_Gen(0);
        FTBL.EV_ControlPackageResources(0, 0, path, (int)"ARMWAUTH", 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        armwHandMotion = (int)FTBL.LoadHandPoseBank(g_actNumber, g_currentMap, "HMOT_0008E");
        Queue_EVT_UTL_LoadNPCChara_3(armwTutorialCharacters, work->self);
    } else {
        path = FTBL.Filepath_Gen(0);
        FTBL.EV_ControlPackageResources(0, 0, path, (int)"ARMWCOMN", 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        FTBL.EV_ControlPackageResources(2, 2, "ARMWCOMN", 0, 0);
        armwHandMotion = (int)FTBL.LoadHandPoseBank(g_actNumber, g_currentMap, "HMOT_0008E");
        ArmwLoadRefereePackage(armwRefereeVariant);
        ArmwLoadOpponentPackage(armwOpponentVariant);
    }
    FTBL.charaSetDisplayEnabledFlag(FTBL.ctrlGetCharaByName(work->player), 0);
    FTBL.charaSetDisplayEnabledFlag(FTBL.ctrlGetCharaByName(work->opponent), 0);
    FTBL.charaSetDisplayEnabledFlag(FTBL.ctrlGetCharaByName(work->referee), 0);
    work->phase = 1;
}

void ArmwLoadRefereePackage(int variant)
{
    const char *names[2] = {"ARMWSAMM", "ARMWLLYM"};
    const char *path = FTBL.Filepath_Gen(0);
    FTBL.EV_ControlPackageResources(0, 0, path, (int)names[variant], 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
}

void ArmwFreeRefereePackages(void)
{
    const char *names[2] = {"ARMWSAMM", "ARMWLLYM"};
    int i;
    for (i = 0; i < 2; i++)
        if (FTBL.EV_ControlPackageResources(0, 5, names[i], 0, 0))
            FTBL.EV_ControlPackageResources(0, 6, names[i], 0, 0);
}

void ArmwLoadOpponentPackage(int variant)
{
    const char *names[4] = {"ARMWSAMR", "ARMWLLYR", "ARMWLLYR", "ARMWHGNR"};
    const char *path;
    int i;
    for (i = 0; i < 4; i++)
        if (FTBL.EV_ControlPackageResources(0, 5, names[i], 0, 0))
            FTBL.EV_ControlPackageResources(0, 6, names[i], 0, 0);
    path = FTBL.Filepath_Gen(0);
    FTBL.EV_ControlPackageResources(0, 0, path, (int)names[variant], 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
}

void ArmwFreeOpponentPackages(void)
{
    const char *names[3] = {"ARMWSAMR", "ARMWLLYR", "ARMWHGNR"};
    int i;
    for (i = 0; i < 4; i++)
        if (FTBL.EV_ControlPackageResources(0, 5, names[i], 0, 0))
            FTBL.EV_ControlPackageResources(0, 6, names[i], 0, 0);
}

int ArmwBeginExit(ArmwWork *work)
{
    int frames = 0;
    if (FTBL.EV_GetScreenFilmIsNotShowing()) {
        frames = 90;
        FTBL.SetScreenFilmFade(frames, 0, 0, 0, 0, 255, 0, 0, 0);
    }
    FTBL.spmFadeOut(0);
    FTBL.HLib_SetCurrentTaskTo(ArmwEndCallback);
    return frames;
}

void ArmwSetExitStep(ArmwWork *work)
{
    int step = work->flags & 8 ? 3 : 4;
    if (work->flags & 1)
        FTBL.EV_stepSetFlags(step, 0);
    else if (work->flags & 32)
        FTBL.EV_stepSetFlags(step, 2);
    else
        FTBL.EV_stepSetFlags(step, 1);
}

void ArmwDebugExit(void *data)
{
    ArmwWork *work = (ArmwWork *)data;
    work->flags |= 1;
    ArmwEndCallback(work);
}

void ArmwEndCallback(void *data)
{
    ArmwWork *work = (ArmwWork *)data;
    FTBL.EV_SetPlayerFlag(16, armwFlagIds[armwLocation], work->opponentIndex);
    FTBL.EV_SetPlayerFlag(12, 1292, 0);
    FTBL.EV_SetPlayerFlag(12, 1293, 0);
    FTBL.charaSetDisplayEnabledFlag(FTBL.ctrlGetCharaByName(ARMW_RYO), 1);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(ARMW_RYO), 0, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(ARMW_RYO), 0, 1, 0);
    FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(work->opponent));
    FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(work->referee));
    FTBL.SetScenarioCharacterKillMode(ARMW_BASE, 1);
    ET_AT_KillWork(work->characters);
    FTBL.qteBattleUtil(5, 0, 0, 0, 0);
    if (armwHandMotion != -1)
        FTBL.ReleaseHandPoseBank((void *)armwHandMotion);
    FTBL.SetPlayerGameFlags(4, 1);
    if (FTBL.DoesScenarioCharacterExist(work->opponent)) {
        FTBL.npcFree(work->opponent);
        FTBL.npcLockUnlock(work->opponent);
    }
    if (g_currentMap == ARMW_QTB4)
        FTBL.EV_FreeAuthorPak(0, "0368");
    else
        FTBL.EV_FreeAuthorPak(0, "0008");
    EVT_UTL_FreeNPCChara(armwTutorialCharacters);
    FTBL.EV_EventSetUpHelpIcon_0(0, 0);
    FTBL.CheckHelpIcon(0, 0, 0);
    if (FTBL.EV_ControlPackageResources(0, 5, "ARMW", 0, 0))
        FTBL.EV_ControlPackageResources(0, 6, "ARMW", 0, 0);
    if (FTBL.EV_ControlPackageResources(0, 5, "ARMWCOMN", 0, 0))
        FTBL.EV_ControlPackageResources(0, 6, "ARMWCOMN", 0, 0);
    if (FTBL.EV_ControlPackageResources(0, 5, "ARMWAUTH", 0, 0))
        FTBL.EV_ControlPackageResources(0, 6, "ARMWAUTH", 0, 0);
    ArmwFreeRefereePackages();
    ArmwFreeOpponentPackages();
    if (g_currentMap == ARMW_AR02)
        FTBL.LoadNPCRoute("NPC.BIN");
    ArmwSetExitStep(work);
    FTBL.EV_NPC_NewLoadSw_0(1);
    if (work->nextScene) {
        FTBL.scenSetTaskFlags(6);
        FTBL.EV_scenQueueLoad(g_actNumber, work->nextScene, work->nextEntry);
    } else {
        FTBL.LoadCutsceneAndSoundProgramForArea();
        FTBL.EV_spmBeginSampleExecute(0);
        EVT_AT_SetMot(ARMW_RYO, 0xf002, 0.0f, 1.0f, ARMW_MOTION_FLAGS, 0);
        switch (armwArgument) {
        case 0:
            EVT_AT_SetPos(ARMW_RYO, ARMW_INVALID, 207.9f, 100.22f, 323.0f, 0);
            EVT_AT_SetAngY(ARMW_RYO, ARMW_INVALID, 240.0f, 0);
            break;
        case 1:
            EVT_AT_SetPos(ARMW_RYO, ARMW_INVALID, 227.8f, 100.22f, 351.5f, 0);
            EVT_AT_SetAngY(ARMW_RYO, ARMW_INVALID, 90.0f, 0);
            break;
        case 2:
            EVT_AT_SetPos(ARMW_RYO, ARMW_INVALID, 252.0f, 100.22f, 354.5f, 0);
            EVT_AT_SetAngY(ARMW_RYO, ARMW_INVALID, 0.0f, 0);
            break;
        }
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    }
    FTBL.EV_HLKillOtherHierarchyTask(work->self);
    FTBL.scenFreeEvent("ARMW.EV3");
    EventDebugUnregister(3);
}

void ArmwSelectCharacters(ArmwWork *work)
{
    short ladder;
    work->day = ArmwGetDay();
    work->rotation = ArmwGetRotation();
    work->opponentIndex = ArmwGetOpponentIndex();
    work->flags |= 0x40000000U;
    work->nextScene = armwExits[armwLocation][0];
    work->nextEntry = armwExits[armwLocation][1];
    work->player = ARMW_RYO;
    work->referee = armwReferees[armwLocation][0];
    if (FTBL.GetMotionRigBoneId(FTBL.scenFindMotionwork(work->referee)) == 7)
        armwRefereeVariant = 0;
    else if (FTBL.GetMotionRigBoneId(FTBL.scenFindMotionwork(work->referee)) == 9)
        armwRefereeVariant = 1;
    else
        armwRefereeVariant = 1;
    ArmwSelectOpponent(work);
    armwCharacters[0] = work->player;
    armwCharacters[1] = work->referee;
    if (!(work->flags & 0x20000000U)) {
        armwCharacters[2] = work->opponent;
        armwCharacters[3] = ARMW_INVALID;
    } else {
        ladder = ArmwGetLadder(work);
        armwCharacters[2] = armwOpponents[armwOrder[ladder][0]].character;
        armwCharacters[3] = armwOpponents[armwOrder[ladder][1]].character;
        armwCharacters[4] = armwOpponents[armwOrder[ladder][2]].character;
        armwCharacters[5] = ARMW_INVALID;
        work->secondOpponent = armwOpponents[armwOrder[ladder][1]].character;
        work->thirdOpponent = armwOpponents[armwOrder[ladder][2]].character;
    }
    if ((FTBL.EV_GetShenmueFlag(3) || FTBL.EV_GetShenmueFlag(2)) && !armwLocation)
        work->flags |= 8;
}

void ArmwSelectOpponent(ArmwWork *work)
{
    short ladder;
    short index;
    if (work->flags & 0x10000000U) {
        armwWager = 20;
        armwLosingBet = -10;
    } else if (work->flags & 0x20000000U) {
        armwWager = 100;
        armwLosingBet = -30;
    } else {
        armwWager = armwBets[armwLocation][work->opponentIndex];
        armwLosingBet = -armwWager;
    }
    ladder = ArmwGetLadder(work);
    index = armwOrder[ladder][work->opponentIndex];
    work->opponent = armwOpponents[index].character;
    work->strength = armwOpponents[index].strength;
    work->minimumStrength = armwOpponents[index].minimumStrength;
    work->maximumStrength = armwOpponents[index].maximumStrength;
    work->stamina = armwOpponents[index].stamina;
    work->initialStamina = armwOpponents[index].stamina;
    work->strategy = (unsigned short)armwOpponents[index].strategy;
    work->buttonA = armwOpponents[index].buttonA;
    work->buttonLeft = armwOpponents[index].buttonLeft;
    work->buttonDown = armwOpponents[index].buttonDown;
    work->buttonUp = armwOpponents[index].buttonUp;
    work->finishQte = armwOpponents[index].finishQte;
    work->warmupFrames = armwOpponents[index].warmupFrames;
    if (work->opponent == ARMW_SFC || work->opponent == ARMW_DGY || work->opponent == ARMW_MHK)
        armwOpponentVariant = 2;
    else if (FTBL.GetMotionRigBoneId(FTBL.scenFindMotionwork(work->opponent)) == 7)
        armwOpponentVariant = 0;
    else if (FTBL.GetMotionRigBoneId(FTBL.scenFindMotionwork(work->opponent)) == 9)
        armwOpponentVariant = 1;
    else
        armwOpponentVariant = 3;
}

int ArmwGetDay(void)
{
    int day = 0;
    if (FTBL.EV_GetShenmueFlag(556))
        day = 0;
    else if (FTBL.EV_GetShenmueFlag(557))
        day = 1;
    else if (FTBL.EV_GetShenmueFlag(558))
        day = 2;
    else if (FTBL.EV_GetShenmueFlag(559))
        day = 3;
    else if (FTBL.EV_GetShenmueFlag(560))
        day = 4;
    else if (FTBL.EV_GetShenmueFlag(561))
        day = 5;
    else if (FTBL.EV_GetShenmueFlag(562))
        day = 6;
    return day;
}

int ArmwGetRotation(void)
{
    int rotation = 0;
    if (armwLocation == 0) {
        if (FTBL.EV_GetShenmueFlag(563))
            rotation = 0;
        else if (FTBL.EV_GetShenmueFlag(564))
            rotation = 1;
        else if (FTBL.EV_GetShenmueFlag(565))
            rotation = 2;
    } else if (armwLocation == 1) {
        if (FTBL.EV_GetShenmueFlag(566))
            rotation = 0;
        else if (FTBL.EV_GetShenmueFlag(567))
            rotation = 1;
        else if (FTBL.EV_GetShenmueFlag(568))
            rotation = 2;
    }
    return rotation;
}

int ArmwGetOpponentIndex(void)
{
    return (unsigned char)FTBL.EV_GetPlayerFlag(15, armwFlagIds[armwLocation]);
}

void ArmwFinishLadderFlags(void)
{
    if (armwLocation == 0) {
        if (FTBL.EV_GetShenmueFlag(574)) {
            if (FTBL.EV_GetShenmueFlag(563))
                FTBL.EV_stepSetFlags(563, 0);
            else if (FTBL.EV_GetShenmueFlag(564))
                FTBL.EV_stepSetFlags(564, 0);
            else if (FTBL.EV_GetShenmueFlag(565))
                FTBL.EV_stepSetFlags(565, 0);
        }
    } else {
        if (FTBL.EV_GetShenmueFlag(563))
            FTBL.EV_stepSetFlags(563, 0);
        else if (FTBL.EV_GetShenmueFlag(564))
            FTBL.EV_stepSetFlags(564, 0);
        else if (FTBL.EV_GetShenmueFlag(565))
            FTBL.EV_stepSetFlags(565, 0);
    }
}

void ArmwResetAuthFlags(void)
{
    if (FTBL.EV_GetShenmueFlag(572))
        FTBL.EV_stepSetFlags(572, 0);
    else if (FTBL.EV_GetShenmueFlag(573))
        FTBL.EV_stepSetFlags(573, 0);
    else if (FTBL.EV_GetShenmueFlag(574))
        FTBL.EV_stepSetFlags(574, 0);
}

short ArmwGetLadder(ArmwWork *work)
{
    if (!(work->flags & 0x40000000U))
        return armwRotations[work->day][armwLocation][work->rotation];
    return armwLocation;
}

int ArmwReady(ArmwWork *work)
{
    int location;
    work->falseStarts = 0;
    location = armwLocation;
    EVT_AT_SetPos(ARMW_BASE, ARMW_INVALID, armwBases[location][0], armwBases[location][1], armwBases[location][2], 0);
    EVT_AT_SetAng(ARMW_BASE, ARMW_INVALID, 0.0f, armwBases[location][3], 0.0f, 0);
    EVT_AT_SetPos(ARMW_MONEY, ARMW_BASE, -0.2f, 0.9f, -0.16f, 0);
    EVT_AT_SetAng(ARMW_MONEY, ARMW_BASE, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_CharaDisp(work->player, 0, 0);
    EVT_AT_CharaDisp(work->opponent, 0, 0);
    EVT_AT_CharaDisp(work->referee, 0, 0);
    FTBL.CheckHelpIcon(1, 0, 0);
    if (work->flags & 0x20000000U) {
        EVT_AT_CharaDraw(work->secondOpponent, 0, 1);
        EVT_AT_CharaDraw(work->thirdOpponent, 0, 1);
    }
    if (FTBL.EV_GetShenmueFlag(2) && !armwLocation)
        work->phase = 2;
    else {
        FTBL.FinishLoading();
        FTBL.EV_LoadAudio("0008A000", 1);
        FTBL.EV_spmBeginSampleExecute(0);
        work->phase = 5;
        work->step = 1;
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    }
    EventDebugCheck(3, 4);
    FTBL.EV_NPC_NewLoadSw_0(1);
    return 0;
}

int ArmwNextOpponent(ArmwWork *work)
{
    short ladder;
    switch (work->step) {
    case 0:
        work->opponentIndex++;
        if (FTBL.DoesScenarioCharacterExist(work->opponent) && (work->flags & 0x10000000U)) {
            FTBL.ReleaseCharacterFaceCache(FTBL.ctrlGetCharaByName(work->opponent));
            FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(work->opponent));
            FTBL.npcFree(work->opponent);
            FTBL.npcLockUnlock(work->opponent);
        }
        work->step++;
        work->falseStarts = 0;
        ladder = ArmwGetLadder(work);
        if (armwOrder[ladder][work->opponentIndex] != -1) {
            ArmwResetAuthFlags();
            ArmwSelectOpponent(work);
            if (work->flags & 0x10000000U) {
                FTBL.EV_NPC_NewLoadSw_0(0);
                FTBL.EV_ResetFreeTalkFlags();
                ArmwLoadOpponentPackage(armwOpponentVariant);
                FTBL.EV_NPC_NewLoadSw_0(1);
            }
        } else {
            ArmwFinishLadderFlags();
            work->step = 0;
            work->phase = 15;
        }
        break;
    case 1:
        if (ArmwOpponentLoaded(work->opponent)) {
            if (!FTBL.EV_GetScreenFilmIsNotShowing())
                FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
            work->step = 0;
            work->phase = 8;
        }
        break;
    }
    return 0;
}

void ArmwReplaceOpponent(ArmwWork *work)
{
    FTBL.ReleaseCharacterFaceCache(FTBL.ctrlGetCharaByName(work->opponent));
    FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(work->opponent));
    FTBL.charaSetDisplayEnabledFlag(FTBL.ctrlGetCharaByName(work->opponent), 0);
    EVT_AT_CharaDraw(work->opponent, 0, 1);
    work->falseStarts = 0;
    work->opponentIndex++;
    work->phase = 4;
    ArmwSelectOpponent(work);
    ArmwLoadOpponentPackage(armwOpponentVariant);
    EVT_AT_CharaDraw(work->opponent, 0, 0);
    FTBL.GetCharacterFaceCacheIndex(FTBL.ctrlGetCharaByName(work->opponent));
    FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(work->opponent), 1);
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(work->opponent));
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(work->opponent), 1, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(work->opponent), 1, 1, 0);
    ArmwSetVoicePrefixes(work->referee, work->opponent);
}

void ArmwMainCallback(void *data)
{
    ArmwWork *work = (ArmwWork *)data;
    int frames = 0;
    if (FTBL.DoesScenarioCharacterExist(ARMW_BASE)) {
        EVT_AT_SetPos(ARMW_BASE, ARMW_INVALID, armwBases[armwLocation][0], armwBases[armwLocation][1], armwBases[armwLocation][2], 0);
        EVT_AT_SetAng(ARMW_BASE, ARMW_INVALID, 0.0f, armwBases[armwLocation][3], 0.0f, 0);
    }
    switch (work->phase) {
    case 0:
        ArmwInitializeResources(work);
        break;
    case 1:
        frames = ArmwReady(work);
        break;
    case 2:
        frames = ArmwTutorial(work);
        break;
    case 3:
        frames = ArmwAskTutorial(work);
        break;
    case 8:
        frames = ArmwOfferNextMatch(work);
        break;
    case 4:
        frames = ArmwPrepareMatch(work);
        break;
    case 5:
        frames = ArmwExplain(work);
        break;
    case 7:
        frames = ArmwRefuse(work);
        break;
    case 9:
        frames = ArmwFalseStart(work);
        break;
    case 10:
        frames = ArmwPlayMatch(work);
        break;
    case 11:
        frames = ArmwDirectionQte(work);
        break;
    case 12:
        frames = ArmwFinishQte(work);
        break;
    case 13:
        frames = ArmwWinAnimation(work);
        break;
    case 16:
        frames = ArmwLoseAnimation(work);
        break;
    case 14:
        frames = ArmwWin(work);
        break;
    case 15:
        frames = ArmwEndLadder(work);
        break;
    case 17:
        frames = ArmwLose(work);
        break;
    case 18:
        frames = ArmwTutorialResult(work);
        break;
    case 19:
        frames = ArmwNextOpponent(work);
        break;
    case 20:
        frames = ArmwBeginExit(work);
        break;
    }
    if (frames > 0)
        FTBL.HLib_SetupTaskDelay(frames);
}

int ArmwAskTutorial(ArmwWork *work)
{
    int frames = 0;
    switch (work->step) {
    case 0:
        FTBL.EV_LoadAudio("0008A000", 1);
        FTBL.FinishLoading();
        Queue_EVT_AT_CtrlHandUp_1(work->player, 1, (void *)areaLoopHandPoses[47], 0, 0);
        EVT_AT_SetMot(work->player, 0xf002, 0.0f, 1.0f, ARMW_MOTION_FLAGS, 0);
        EVT_AT_SetMot(work->opponent, armwOpponentMotions[0][armwOpponentVariant], 0.0f, 1.0f, ARMW_MOTION_FLAGS, 0);
        EVT_AT_SetMot(work->referee, armwRefereeMotions[0][armwRefereeVariant], 0.0f, 1.0f, ARMW_MOTION_FLAGS, 0);
        EVT_AT_SetPos(work->player, ARMW_BASE, -4.3f, 0.0f, -4.1f, 0);
        EVT_AT_SetAng(work->player, ARMW_BASE, 0.0f, 245.0f, 0.0f, 0);
        EVT_AT_SetPos(work->opponent, ARMW_BASE, -1.2f, 0.0f, 0.9f, 0);
        EVT_AT_SetAng(work->opponent, ARMW_BASE, 0.0f, 0.0f, 0.0f, 0);
        EVT_AT_SetPos(work->referee, ARMW_BASE, -2.9f, 0.0f, -3.5f, 0);
        EVT_AT_SetAng(work->referee, ARMW_BASE, 0.0f, 65.0f, 0.0f, 0);
        EVT_AT_SetCam(2101, ARMW_BASE, ARMW_BASE, 0, 0, 0);
        if (FTBL.GetPlayerStat(2) > armwWager) {
            QueueSkChoiceWait(work->self, 0xffff, 157, 153, 0xffff, 153, 32767, 10);
            work->step++;
        } else {
            frames = ArmwWalkAway(work);
            work->phase = 20;
            work->step = 0;
        }
        break;
    case 1:
        if (FTBL.shenGetCtrlSpeakIconFlag() == 157) {
            ArmwVoice(0, "028", 0);
            work->phase = 5;
            work->step = 0;
            frames = 30;
        } else {
            frames = ArmwWalkAway(work);
            work->phase = 20;
            work->step = 0;
        }
        break;
    }
    return frames;
}

int ArmwExplain(ArmwWork *work)
{
    int frames = 0;
    switch (work->step) {
    case 0:
        EVT_AT_SetVoice("0008E065", 0);
        EVT_AT_SetVoice("0008E066", 80);
        EVT_AT_SetVoice("0008A001", 180);
        EVT_AT_SetVoice("0008E067", 260);
        EVT_AT_SetVoice("0008E068", 320);
        EVT_AT_SetCam(2130, ARMW_BASE, ARMW_BASE, 0, 0, 0);
        EVT_AT_SetCam(2131, ARMW_BASE, ARMW_BASE, 0, 0, 180);
        EVT_AT_SetCam(2132, ARMW_BASE, ARMW_BASE, 0, 0, 260);
        EVT_AT_SetMot(work->referee, 9095, 60.0f, 1.0f, ARMW_MOTION_FLAGS, 260);
        frames = 460;
        EVT_AT_SetFaceLooks(work->referee, 3, 0, 1.0f, 0);
        EVT_AT_SetFaceLooks(work->referee, 0, 0, 1.0f, frames);
        EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, frames - 60);
        work->step++;
        break;
    case 1:
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.SetPlayerGameFlags(4, 0);
        EVT_AT_SetPos(work->player, ARMW_BASE, 0.5f, 0.0f, -0.8f, 0);
        EVT_AT_SetAng(work->player, ARMW_BASE, 0.0f, 157.5f, 0.0f, 0);
        EVT_AT_SetPos(work->referee, ARMW_BASE, 1.2f, 0.0f, 0.6f, 0);
        EVT_AT_SetAng(work->referee, ARMW_BASE, 0.0f, 337.5f, 0.0f, 0);
        EVT_AT_SetPos(work->opponent, ARMW_BASE, 0.0f, 0.0f, 0.8f, 0);
        EVT_AT_SetAng(work->opponent, ARMW_BASE, 0.0f, 0.0f, 0.0f, 0);
        EVT_AT_SetMot(work->player, 0xf003, 0.0f, 1.0f, ARMW_MOTION_FLAGS, 0);
        EVT_AT_SetMot(work->referee, armwRefereeMotions[13][armwRefereeVariant], 0.0f, 1.0f, ARMW_MOTION_FLAGS, 0);
        EVT_AT_SetMot(work->opponent, armwOpponentMotions[1][armwOpponentVariant], 0.0f, 1.0f, ARMW_MOTION_FLAGS, 0);
        EVT_AT_SetCam(2150, ARMW_BASE, ARMW_BASE, 0, 0, 0);
        EVT_AT_SetSound(188, 10);
        EVT_AT_SetSound(187, 30);
        EVT_AT_SetSound(271, 40);
        if (work->referee != ARMW_SD1)
            ArmwAppendVoice(2, "009");
        else
            ArmwAppendVoice(1, "009");
        ArmwAppendVoice(2, "010");
        ArmwWaitVoices(work->self, 8, 0x80000000U, 0);
        work->step++;
        break;
    case 2:
        if (ArmwGetVoiceSkipped()) {
            work->step = 1;
            work->phase = 4;
            EVT_AT_MenCtrl(0, 0, 0, 0, 0, 255, 0, 0, 0, 0);
        } else {
            EVT_AT_SetMot(work->player, 9157, 20.0f, 0.7f, ARMW_MOTION_FLAGS, 0);
            EVT_AT_SetMot(work->referee, armwRefereeMotions[13][armwRefereeVariant], 200.0f, 1.0f, ARMW_MOTION_FLAGS, 0);
            ArmwAppendVoice(1, "011");
            EVT_AT_SetCam(2151, ARMW_BASE, ARMW_BASE, 0, 0, 0);
            EVT_AT_SetSound(187, 30);
            EVT_AT_SetSound(188, 40);
            EVT_AT_SetSound(271, 50);
            EVT_AT_SetSound(348, 60);
            ArmwWaitVoices(work->self, 8, 0x80000000U, 0);
            work->step++;
        }
        break;
    case 3:
        if (ArmwGetVoiceSkipped()) {
            work->step = 1;
            work->phase = 4;
            EVT_AT_MenCtrl(0, 0, 0, 0, 0, 255, 0, 0, 0, 0);
        } else {
            EVT_AT_CharaDisp(work->opponent, 0, 1);
            EVT_AT_SetMot(work->player, 0xf003, 0.0f, 1.0f, ARMW_MOTION_FLAGS, 0);
            EVT_AT_SetMot(work->referee, armwRefereeMotions[13][armwRefereeVariant], 320.0f, 1.0f, ARMW_MOTION_FLAGS, 0);
            EVT_AT_SetPos(work->player, ARMW_BASE, 0.5f, 0.0f, -0.8f, 0);
            EVT_AT_SetAng(work->player, ARMW_BASE, 0.0f, 157.5f, 0.0f, 0);
            ArmwAppendVoice(2, "012");
            ArmwAppendVoice(2, "001");
            EVT_AT_SetCam(2152, ARMW_BASE, ARMW_BASE, 0, 0, 0);
            ArmwWaitVoices(work->self, 8, 0x80000000U, 0);
            work->step++;
        }
        break;
    case 4:
        if (ArmwGetVoiceSkipped()) {
            work->step = 1;
            work->phase = 4;
            EVT_AT_MenCtrl(0, 0, 0, 0, 0, 255, 0, 0, 0, 0);
        } else {
            EVT_AT_SetSound(187, 20);
            EVT_AT_SetSound(349, 60);
            EVT_AT_SetSound(187, 120);
            switch (work->opponent) {
            case ARMW_HONG:
            case ARMW_MSH:
            case ARMW_SSY:
                frames += ArmwIntroduceHong(work->player, work->opponent, work->referee, 0);
                break;
            case ARMW_SF8:
            case ARMW_SF2:
            case ARMW_DML:
                frames += ArmwIntroduceSf8(work->player, work->opponent, work->referee, 0);
                break;
            case ARMW_SFC:
            case ARMW_MHK:
                frames += ArmwIntroduceSfc(work->player, work->opponent, work->referee, 0);
                break;
            case ARMW_DGY:
                frames += ArmwIntroduceSfc(work->player, work->opponent, work->referee, 0);
                break;
            case ARMW_KZN:
            case ARMW_FRS:
                frames += ArmwIntroduceKzn(work->player, work->opponent, work->referee, 0);
                break;
            }
            ArmwVoice(2, "002", 0);
            ArmwVoice(3, "007", frames - 60);
            work->step = 1;
            work->phase = 4;
            EVT_AT_CharaDisp(work->referee, 0, 1);
            EVT_AT_CharaDisp(work->opponent, 0, 0);
            frames += 30;
            EVT_AT_CharaDisp(work->referee, frames, 0);
            EVT_AT_MenCtrl(60, 0, 0, 0, 0, 255, 0, 0, 0, frames - 60);
        }
        break;
    }
    return frames;
}

int ArmwWalkAway(ArmwWork *work)
{
    FTBL.SetPlayerGameFlags(4, 1);
    EVT_AT_SetMot(work->player, 9165, 0.0f, 1.0f, ARMW_MOTION_FLAGS, 40);
    EVT_AT_SetMot(work->opponent, armwOpponentMotions[0][armwOpponentVariant], 0.0f, 1.0f, ARMW_MOTION_FLAGS, 25);
    ArmwVoice(0, "027", 0);
    ArmwVoice(2, "029", 90);
    EVT_AT_SetCam(1803, ARMW_BASE, ARMW_BASE, 0, 0, 40);
    EVT_AT_SetPoint(work->referee, work->player, 5, 3, 0, 40, 180, 0);
    work->flags |= 32;
    return 180;
}

int ArmwRefuse(ArmwWork *work)
{
    ArmwAppendVoice(0, "027");
    ArmwAppendVoice(1, "029");
    ArmwAppendVoice(1, "030");
    work->phase = 20;
    ArmwWaitVoices(work->self, 8, 0, 0);
    return 220;
}

int ArmwPrepareMatch(ArmwWork *work)
{
    int frames = 0;
    switch (work->step) {
    case 0:
        FTBL.SetPlayerGameFlags(4, 0);
        FTBL.EV_LoadAudio("0008A000", 1);
        FTBL.EV_spmBeginSampleExecute(0);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        switch (work->opponent) {
        case ARMW_HONG:
        case ARMW_MSH:
        case ARMW_SSY:
            frames = ArmwIntroduceHong(work->player, work->opponent, work->referee, 0);
            break;
        case ARMW_SF8:
        case ARMW_SF2:
        case ARMW_DML:
            frames = ArmwIntroduceSf8(work->player, work->opponent, work->referee, 0);
            break;
        case ARMW_SFC:
        case ARMW_MHK:
            frames = ArmwIntroduceSfc(work->player, work->opponent, work->referee, 0);
            break;
        case ARMW_DGY:
            frames = ArmwIntroduceSfc(work->player, work->opponent, work->referee, 0);
            break;
        case ARMW_KZN:
        case ARMW_FRS:
            frames = ArmwIntroduceKzn(work->player, work->opponent, work->referee, 0);
            break;
        }
        EVT_AT_CharaDisp(work->referee, 0, 1);
        EVT_AT_CharaDisp(work->referee, frames, 0);
        EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, frames - 30);
        EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, frames);
        work->step++;
        break;
    case 1:
        FTBL.SetPlayerGameFlags(4, 0);
        work->position = 0.5f;
        if (!FTBL.EV_GetScreenFilmIsNotShowing())
            FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EVT_AT_CharaDisp(work->opponent, 0, 0);
        EVT_AT_CharaDisp(work->referee, 0, 0);
        ArmwMatchHands(work);
        ArmwUpdatePose(work, 0);
        EVT_AT_SetPos(work->referee, ARMW_BASE, 0.7f, 0.0f, 0.05f, 0);
        EVT_AT_SetAng(work->referee, ARMW_BASE, 0.0f, 90.0f, 0.0f, 0);
        EVT_AT_SetPos(ARMW_MONEY, ARMW_BASE, -0.2f, 0.9f, -0.16f, 0);
        EVT_AT_SetAng(ARMW_MONEY, ARMW_BASE, 0.0f, 0.0f, 0.0f, 0);
        EVT_AT_SetMot(work->referee, armwRefereeMotions[3][armwRefereeVariant], 0.0f, 1.0f, ARMW_MOTION_FLAGS, 0);
        EVT_AT_SetPoint(work->player, work->opponent, 5, 3, 0, 0, 0, 0xa0000000U);
        EVT_AT_SetPoint(work->opponent, work->player, 5, 3, 0, 0, 0, 0xa0000000U);
        EVT_AT_SetCam(3000, ARMW_BASE, ARMW_BASE, 0, 0, 0);
        armwIntroductionVariant = FTBL.shenGetRandomInt32(2);
        if (!armwStarted)
            ArmwAppendVoice(3, "001");
        else if (armwIntroductionVariant)
            ArmwAppendVoice(3, "003");
        else
            ArmwAppendVoice(3, "004");
        ArmwWaitVoices(work->self, 8, 0, 0);
        work->step++;
        break;
    case 2: {
        int index = 0;
        const char *voices[11][3] = {{"002", "003", "004"}, {"029", "030", "031"}, {"034", "035", "036"}, {"039", "040", "041"}, {"043", "044", "045"},
            {"002", "028", "045"}, {"067", "068", "069"}, {"050", "051", "052"}, {"063", "064", "030"}, {"061", "031", "062"}, {"055", "056", "057"}};
        switch (work->opponent) {
        case ARMW_HONG:
            index = 0;
            break;
        case ARMW_SF8:
            index = 1;
            break;
        case ARMW_SFC:
            index = 2;
            break;
        case ARMW_KZN:
            index = 3;
            break;
        case ARMW_SSY:
            index = 4;
            break;
        case ARMW_SF2:
            index = 5;
            break;
        case ARMW_FRS:
            index = 6;
            break;
        case ARMW_MHK:
            index = 7;
            break;
        case ARMW_DML:
            index = 8;
            break;
        case ARMW_MSH:
            index = 9;
            break;
        case ARMW_DGY:
            index = 10;
            break;
        }
        EVT_AT_SetCam(3001, ARMW_BASE, ARMW_BASE, 0, 0, 0);
        if (!armwStarted)
            ArmwAppendVoice(0, voices[index][0]);
        else if (armwIntroductionVariant)
            ArmwAppendVoice(0, voices[index][1]);
        else
            ArmwAppendVoice(0, voices[index][2]);
        ArmwWaitVoices(work->self, 8, 0, 0);
        work->step++;
        break;
    }
    case 3:
        EVT_AT_SetCam(3002, ARMW_BASE, ARMW_BASE, 0, 0, 0);
        if (!armwStarted)
            ArmwAppendVoice(3, "002");
        else if (armwIntroductionVariant)
            ArmwAppendVoice(3, "005");
        else
            ArmwAppendVoice(3, "006");
        ArmwWaitVoices(work->self, 8, 0, 0);
        work->step++;
        break;
    case 4:
        if (!armwStarted) {
            EVT_AT_SetMot(work->referee, armwRefereeMotions[10][armwRefereeVariant], 0.0f, 1.0f, ARMW_MOTION_FLAGS, 0);
            EVT_AT_SetCam(3003, ARMW_BASE, ARMW_BASE, 0, 0, 0);
            ArmwAppendVoice(1, "013");
            ArmwWaitVoices(work->self, 8, 0, 0);
        }
        work->step++;
        break;
    case 5:
        if (!armwStartFrames) {
            armwStarted = 1;
            EVT_AT_SetPoint(work->player, 0, 0, 3, 0, 0, 0, 0x10000000U);
            EVT_AT_SetPoint(work->opponent, 0, 0, 3, 0, 0, 0, 0x10000000U);
            ArmwMatchHands(work);
            ArmwUpdatePose(work, 0);
            EVT_AT_SetPos(work->referee, ARMW_BASE, 0.7f, 0.0f, 0.05f, 0);
            EVT_AT_SetAng(work->referee, ARMW_BASE, 0.0f, 90.0f, 0.0f, 0);
            EVT_AT_SetMot(work->referee, armwRefereeMotions[11][armwRefereeVariant], 0.0f, 1.0f, ARMW_MOTION_FLAGS, 0);
            EVT_AT_SetCam(3004, ARMW_BASE, ARMW_BASE, 0, 0, 0);
            ArmwVoice(1, "014", 0);
        }
        if (armwStartFrames == 40)
            EVT_AT_SetCam(3005, ARMW_BASE, ARMW_BASE, 0, 0, 0);
        if (armwStartFrames++ > 50) {
            FTBL.GetBattleDataConfig(0, ARMW_BATTLE_INPUT);
            EVT_AT_SetMot(work->referee, armwRefereeMotions[12][armwRefereeVariant], 120.0f, 1.0f, 0, 0);
            ArmwVoice(1, "015", 0);
            ArmwResetMatch(work);
            armwStartFrames = 0;
            work->step = 0;
            work->phase = 10;
        } else if ((FTBL.GetCurrentControlInput(7) >> 10) & 1) {
            FTBL.CheckHelpIcon(0, 0, 0);
            armwStartFrames = 0;
            work->step = 0;
            work->phase = 9;
        }
        break;
    }
    return frames;
}

int ArmwOfferNextMatch(ArmwWork *work)
{
    int frames = 0;
    switch (work->step) {
    case 0:
        if (work->flags & 0x10000000U)
            frames = ArmwOfferRematch(work->player, work->opponent, work->referee);
        else
            frames = ArmwAnnounceBet(work->player, work->opponent, work->referee);
        if (FTBL.shenGetMinutesUntilNextFrameMinute()) {
            EVT_AT_SetCam(3205, ARMW_BASE, ARMW_BASE, 0, 0, frames);
            QueueSkChoiceWait(work->self, 0xffff, 157, 153, 0xffff, 153, 32767, frames);
            work->step++;
            frames = 0;
        } else {
            EVT_AT_CharaDisp(work->opponent, frames, 1);
            work->step = 0;
            work->phase = 7;
        }
        break;
    case 1:
        if (FTBL.shenGetCtrlSpeakIconFlag() == 157) {
            ArmwVoice(0, "028", 0);
            frames = 30;
            FTBL.SetScreenFilmFade(frames, 0, 0, 0, 0, 255, 0, 0, 0);
            work->phase = 4;
            work->step = 1;
        } else {
            work->phase = 7;
            work->step = 0;
        }
        break;
    }
    return frames;
}

int ArmwFalseStart(ArmwWork *work)
{
    int frames = ArmwWarnFalseStart(work->player, work->opponent, work->referee, work->falseStarts);
    if (++work->falseStarts <= 2) {
        work->step = 4;
        work->phase = 4;
        EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, frames);
    } else {
        work->step = 1;
        work->phase = 17;
        frames += 30;
        EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, frames);
    }
    return frames;
}

int ArmwWin(ArmwWork *work)
{
    int frames = 0;
    Vector3 refereeMoney = {0.075f, 0.0f, 0.032f};
    Vector3i refereeMoneyRotation = {19114, 34133, 41870};
    Vector3 playerMoney = {0.086f, -0.002f, -0.011f};
    Vector3i playerMoneyRotation = {49152, 0, 49152};
    switch (work->step) {
    case 0:
        EVT_AT_SetPos(work->player, ARMW_BASE, -0.02f, 0.0f, -0.76f, 0);
        EVT_AT_SetAng(work->player, ARMW_BASE, 0.0f, 180.0f, 0.0f, 0);
        EVT_AT_SetPos(work->opponent, ARMW_BASE, 0.02f, 0.0f, 0.76f, 0);
        EVT_AT_SetAng(work->opponent, ARMW_BASE, 0.0f, 0.0f, 0.0f, 0);
        EVT_AT_SetPos(work->referee, ARMW_BASE, 1.5f, 0.0f, 0.0f, 0);
        EVT_AT_SetAng(work->referee, ARMW_BASE, 0.0f, 315.0f, 0.0f, 0);
        EVT_AT_SetAngY(work->referee, ARMW_BASE, 45.0f, 120);
        EVT_AT_SetMot(work->player, 9167, 0.0f, 1.0f, ARMW_MOTION_FLAGS, 0);
        EVT_AT_SetMot(work->opponent, armwOpponentMotions[3][armwOpponentVariant], 0.0f, 1.0f, ARMW_MOTION_FLAGS, 0);
        EVT_AT_SetMot(work->referee, armwRefereeMotions[0][armwRefereeVariant], 0.0f, 1.0f, ARMW_MOTION_FLAGS, 90);
        ArmwVoice(0, "022", 60);
        EVT_AT_SetSound(349, 0);
        EVT_AT_SetSound(269, 20);
        EVT_AT_SetSound(347, 30);
        EVT_AT_SetSound(349, 10);
        EVT_AT_SetCam(FTBL.shenGetRandomInt32(3) + 3100, ARMW_BASE, ARMW_BASE, 0, 0, 0);
        if ((work->flags & 0x20000000U) && work->opponentIndex != 2)
            ArmwReplaceOpponent(work);
        else
            work->step++;
        if (work->flags & 8) {
            work->phase = 18;
            work->step = 0;
        }
        frames = 120;
        break;
    case 1:
        EVT_AT_SetPos(work->player, ARMW_BASE, -0.4f, 0.0f, -0.7f, 0);
        EVT_AT_SetAng(work->player, ARMW_BASE, 0.0f, 180.0f, 0.0f, 0);
        EVT_AT_SetPos(work->referee, work->player, -0.75f, 0.0f, -0.1f, 0);
        EVT_AT_SetAng(work->referee, work->player, 0.0f, 260.0f, 0.0f, 0);
        EVT_AT_SetPos(work->opponent, ARMW_BASE, 0.2f, 0.0f, 0.76f, 0);
        EVT_AT_SetAng(work->opponent, ARMW_BASE, 0.0f, 0.0f, 0.0f, 0);
        EVT_AT_SetMot(work->player, 9160, 70.0f, 1.0f, ARMW_MOTION_FLAGS, 0);
        switch (work->opponent) {
        case ARMW_HONG:
        case ARMW_MSH:
        case ARMW_SSY:
            EVT_AT_SetMot(work->opponent, 9299, 30.0f, 1.0f, ARMW_MOTION_FLAGS, 0);
            break;
        case ARMW_SF8:
        case ARMW_SF2:
        case ARMW_DML:
            EVT_AT_SetAngY(work->opponent, work->opponent, -90.0f, 0);
            EVT_AT_SetMot(work->opponent, 9232, 20.0f, 1.0f, ARMW_MOTION_FLAGS, 0);
            break;
        case ARMW_SFC:
        case ARMW_MHK:
            EVT_AT_SetMot(work->opponent, 9297, 80.0f, 1.0f, ARMW_MOTION_FLAGS, 0);
            break;
        case ARMW_DGY:
            EVT_AT_SetMot(work->opponent, 9295, 30.0f, 1.0f, ARMW_MOTION_FLAGS, 0);
            break;
        case ARMW_KZN:
        case ARMW_FRS:
            EVT_AT_SetMot(work->opponent, 9360, 70.0f, 1.0f, ARMW_MOTION_FLAGS, 0);
            break;
        }
        EVT_AT_SetMot(work->referee, armwRefereeMotions[7][armwRefereeVariant], 70.0f, 1.0f, ARMW_MOTION_FLAGS, 0);
        Queue_EVT_AT_Cling(ARMW_MONEY, work->referee, 21, &refereeMoney, &refereeMoneyRotation, 70, 0);
        Queue_EVT_AT_Cling(ARMW_MONEY, work->player, 14, &playerMoney, &playerMoneyRotation, 120, 0);
        Queue_EVT_AT_Cling(ARMW_MONEY, 0, 0, 0, 0, 220, 1);
        ArmwVoice(1, "005", 30);
        ArmwVoice(1, "006", 100);
        EVT_AT_SetFaceLooks(work->referee, 3, 0, 1.0f, 0);
        EVT_AT_SetFaceLooks(work->referee, 0, 8, 1.0f, 220);
        EVT_AT_SetCam(FTBL.shenGetRandomInt32(3) + 3103, ARMW_BASE, ARMW_BASE, 0, 0, 0);
        EVT_AT_StartMoneyCount(armwWager, 90);
        work->step = 0;
        work->phase = 19;
        frames = 220;
        break;
    }
    return frames;
}

int ArmwEndLadder(ArmwWork *work)
{
    switch (work->step) {
    case 0:
        EVT_AT_SetPos(work->player, ARMW_BASE, -0.02f, 0.0f, -0.76f, 0);
        EVT_AT_SetAng(work->player, ARMW_BASE, 0.0f, 180.0f, 0.0f, 0);
        EVT_AT_SetPos(work->referee, ARMW_BASE, 0.5f, 0.0f, 0.4f, 0);
        EVT_AT_SetAng(work->referee, ARMW_BASE, 0.0f, 22.5f, 0.0f, 0);
        EVT_AT_SetMot(work->player, 0xf003, 0.0f, 1.0f, ARMW_MOTION_FLAGS, 0);
        EVT_AT_SetMot(work->referee, armwRefereeMotions[0][armwRefereeVariant], 0.0f, 1.0f, ARMW_MOTION_FLAGS, 0);
        ArmwAppendVoice(1, "007");
        EVT_AT_SetCam(3150, ARMW_BASE, ARMW_BASE, 0, 0, 0);
        work->step++;
        break;
    case 1:
        ArmwAppendVoice(0, "024");
        EVT_AT_SetCam(3151, ARMW_BASE, ARMW_BASE, 0, 0, 0);
        work->step++;
        break;
    case 2:
        EVT_AT_SetPos(work->opponent, ARMW_BASE, -0.2f, 0.0f, 1.1f, 0);
        EVT_AT_SetAng(work->opponent, ARMW_BASE, 0.0f, 337.5f, 0.0f, 0);
        switch (work->opponent) {
        case ARMW_HONG:
        case ARMW_MSH:
        case ARMW_SSY:
            EVT_AT_SetMot(work->opponent, 9299, 30.0f, 0.6f, ARMW_MOTION_FLAGS, 0);
            break;
        case ARMW_SF8:
        case ARMW_SF2:
        case ARMW_DML:
            EVT_AT_SetAngY(work->opponent, work->opponent, -90.0f, 0);
            EVT_AT_SetMot(work->opponent, 9232, 20.0f, 0.6f, ARMW_MOTION_FLAGS, 0);
            break;
        case ARMW_SFC:
        case ARMW_MHK:
            EVT_AT_SetMot(work->opponent, 9297, 80.0f, 0.6f, ARMW_MOTION_FLAGS, 0);
            break;
        case ARMW_DGY:
            EVT_AT_SetMot(work->opponent, 9295, 30.0f, 0.6f, ARMW_MOTION_FLAGS, 0);
            break;
        case ARMW_KZN:
        case ARMW_FRS:
            EVT_AT_SetMot(work->opponent, 9360, 70.0f, 0.6f, ARMW_MOTION_FLAGS, 0);
            break;
        }
        ArmwAppendVoice(1, "008");
        ArmwAppendVoice(1, "022");
        EVT_AT_SetCam(3152, ARMW_BASE, ARMW_BASE, 0, 0, 0);
        EVT_AT_CharaDisp(ARMW_MONEY, 0, 1);
        work->step++;
        break;
    case 3:
        ArmwAppendVoice(0, "025");
        EVT_AT_SetCam(3151, ARMW_BASE, ARMW_BASE, 0, 0, 0);
        work->phase = 20;
        work->step = 0;
        break;
    }
    ArmwWaitVoices(work->self, 8, 0, 0);
    return 0;
}

int ArmwLose(ArmwWork *work)
{
    int frames = 0;
    switch (work->step) {
    case 0:
        if (FTBL.shenGetRandomInt32(2))
            ArmwVoice(3, "024", 0);
        else
            ArmwVoice(3, "025", 0);
        EVT_AT_SetSound(345, 0);
        EVT_AT_SetSound(349, 20);
        EVT_AT_SetSound(3135, 40);
        switch (work->opponent) {
        case ARMW_HONG:
        case ARMW_MSH:
        case ARMW_SSY:
            frames = ArmwHongCollects(work->player, work->opponent, work->referee, 0);
            break;
        case ARMW_SF8:
        case ARMW_SF2:
        case ARMW_DML:
            frames = ArmwSf8Collects(work->player, work->opponent, work->referee, 0);
            break;
        case ARMW_SFC:
        case ARMW_MHK:
            frames = ArmwSfcCollects(work->player, work->opponent, work->referee, 0);
            break;
        case ARMW_DGY:
            frames = ArmwDgyCollects(work->player, work->opponent, work->referee, 0);
            break;
        case ARMW_KZN:
        case ARMW_FRS:
            frames = ArmwKznCollects(work->player, work->opponent, work->referee, 0);
            break;
        }
        work->step++;
        frames += 30;
        break;
    case 1:
        ArmwRefereeCollects(work->player, work->opponent, work->referee, 0);
        EVT_AT_SetSound(346, 5);
        ArmwWaitVoices(work->self, 8, 0, 0);
        if (work->falseStarts == 3)
            EVT_AT_StartMoneyCount(-armwWager, 0);
        Queue_EVT_AT_Cling(ARMW_MONEY, 0, 0, 0, 0, 0, 1);
        EVT_AT_CharaDisp(ARMW_MONEY, 0, 1);
        work->step++;
        break;
    case 2:
        EVT_AT_SetCam(3391, ARMW_BASE, ARMW_BASE, 0, 0, 0);
        ArmwVoice(3, "026", 0);
        work->phase = 20;
        frames = 100;
        work->step = 0;
        break;
    }
    return frames;
}

int ArmwTutorial(ArmwWork *work)
{
    int frames = 0;
    const char *path;
    switch (work->step) {
    case 0:
        FTBL.EV_LoadAudio("0008A000", 0);
        FTBL.EV_spmBeginSampleExecute(0);
        FTBL.SetPlayerGameFlags(4, 1);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        QueueSkAuthorStart(work->self, ARMW_YASA, 0, 0, 0);
        FTBL.EV_stepSetFlags(2, 0);
        FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(ARMW_SHIYO));
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(ARMW_SHIYO), 1, 0, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(ARMW_SHIYO), 1, 1, 0);
        FTBL.SetScenarioCharacterAnimation(FTBL.ctrlGetCharaByName(ARMW_HONG), 0, 1025, 0, 1.0f, 1, 0, 4, 0);
        FTBL.SetScenarioCharacterAnimation(FTBL.ctrlGetCharaByName(ARMW_SHIYO), 0, 1033, 0, 1.0f, 1, 0, 4, 0);
        work->step++;
        break;
    default:
        FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(ARMW_SHIYO));
        EVT_UTL_FreeNPCChara(armwTutorialCharacters);
        if (armwHandMotion != -1)
            FTBL.ReleaseHandPoseBank((void *)armwHandMotion);
        if (FTBL.EV_ControlPackageResources(0, 5, "ARMWAUTH", 0, 0))
            FTBL.EV_ControlPackageResources(0, 6, "ARMWAUTH", 0, 0);
        FTBL.EV_NPC_NewLoadSw_0(0);
        FTBL.EV_ResetFreeTalkFlags();
        path = FTBL.Filepath_Gen(0);
        FTBL.EV_ControlPackageResources(0, 0, path, (int)"ARMWCOMN", 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        FTBL.EV_ControlPackageResources(2, 2, "ARMWCOMN", 0, 0);
        armwHandMotion = (int)FTBL.LoadHandPoseBank(g_actNumber, g_currentMap, "HMOT_0008E");
        ArmwLoadRefereePackage(armwRefereeVariant);
        ArmwLoadOpponentPackage(armwOpponentVariant);
        FTBL.EV_NPC_NewLoadSw_0(1);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        work->phase = 3;
        work->step = 0;
        break;
    }
    return frames;
}

int ArmwTutorialResult(ArmwWork *work)
{
    int frames = 0;
    const char *path;
    switch (work->step) {
    case 0:
        EVT_AT_SetPos(work->opponent, ARMW_BASE, 0.2f, 0.0f, 0.76f, 0);
        EVT_AT_SetAng(work->opponent, ARMW_BASE, 0.0f, 0.0f, 0.0f, 0);
        EVT_AT_SetMot(work->opponent, 9299, 0.0f, 1.0f, ARMW_MOTION_FLAGS, 0);
        ArmwVoice(3, "020", 0);
        EVT_AT_SetCam(6000, ARMW_BASE, ARMW_BASE, 0, 0, 0);
        EVT_AT_CharaDisp(work->referee, 0, 1);
        EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, 60);
        frames = 90;
        work->step++;
        break;
    case 1:
        FTBL.EV_LoadAudio("0008A000", 0);
        FTBL.EV_NPC_NewLoadSw_0(0);
        FTBL.EV_ResetFreeTalkFlags();
        ArmwResetAuthFlags();
        ArmwFreeRefereePackages();
        ArmwFreeOpponentPackages();
        path = FTBL.Filepath_Gen(0);
        FTBL.EV_ControlPackageResources(0, 0, path, (int)"ARMWAUTH", 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        FTBL.EV_NPC_NewLoadSw_0(1);
        EVT_AT_CharaDisp(work->opponent, 0, 1);
        EVT_AT_CharaDisp(work->referee, 0, 0);
        QueueSkNpcWait(work->self);
        work->step++;
        break;
    case 2:
        FTBL.SetPlayerGameFlags(4, 1);
        FTBL.EV_authSceneCtrl(-11);
        QueueSkAuthorStart(work->self, ARMW_WON, 0, 0, 0);
        EVT_AT_StartMoneyCount(armwWager, 30);
        work->step++;
        break;
    default:
        FTBL.EV_authSceneCtrl(-10);
        EVT_AT_CharaDisp(work->opponent, 90, 0);
        work->opponentIndex = 1;
        work->phase = 20;
        work->step = 0;
        break;
    }
    return frames;
}

void ArmwResetMatch(ArmwWork *work)
{
    FTBL.EV_LoadAudio("0008A000", 1);
    FTBL.SetLoadingBusy();
    work->target = 0.5f;
    work->position = 0.5f;
    work->acceleration = -0.00001f;
    work->shake = 0.0f;
    work->playerHistory = 0xff000000U;
    work->opponentHistory = 0xff000000U;
    work->opponentRate = 0;
    work->playerFace = 1;
    work->opponentFace = 1;
    work->presses = 0;
    work->frames = 0;
    work->reserved94 = 0;
    work->strategyFlags = 0;
    work->recentRate = 2;
    work->stalemate = 250;
    work->settling = 0;
    work->fatigueVoice = 0;
    work->staminaVoice = 0;
    work->fatigueRate = 50.0f;
    work->fatigueFrames = 0;
    FTBL.CheckHelpIcon(0, 0, 0);
    FTBL.EV_SetPlayerFlag(12, 1292, 1);
    FTBL.EV_SetPlayerFlag(12, 1293, 0);
    EVT_AT_SetFaceLooks(work->player, 1, 0, 1.0f, 0);
    EVT_AT_SetFaceLooks(work->opponent, 1, 0, 1.0f, 0);
    FTBL.EVS_LoopCamera(FTBL.shenGetRandomInt32(3) + 5000, ARMW_BASE, ARMW_BASE);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(work->player), 0, 1, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(work->opponent), 0, 1, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(work->referee), 0, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(work->referee), 0, 1, 0);
}

static __inline__ int ArmwMaxInt(int a, int b)
{
    return a < b ? b : a;
}

static __inline__ int ArmwMinInt(int a, int b)
{
    return a < b ? a : b;
}

int ArmwPlayMatch(ArmwWork *work)
{
    unsigned int buttons;
    unsigned int fatigue;
    unsigned int rate;
    unsigned int bit;
    int i;
    int player;
    int opponent;
    int difference;
    float movement;
    float shake;
    float step;
    if (work->frames == 15)
        EVT_AT_SetSound(2119, 0);
    work->frames++;
    if (!work->fatigueFrames)
        fatigue = 0;
    else
        fatigue = 3;
    work->strategyFlags &= ~1U;
    buttons = ArmwButtons(work);
    if (buttons & 1)
        work->presses++;
    work->playerHistory <<= 1;
    work->opponentHistory <<= 1;
    if (buttons & 1)
        work->playerHistory |= 1;
    if (buttons & 2)
        work->opponentHistory |= 1;
    rate = 0;
    for (bit = 0x80000000U; bit; bit >>= 1)
        if (work->playerHistory & bit)
            rate++;
    if (rate < (unsigned int)(work->recentRate * 0.7f) && work->frames > (unsigned short)work->warmupFrames) {
        if (!(fatigue & 2)) {
            fatigue |= 3;
            for (i = 0, bit = 1; i < (int)(32 / work->recentRate + 3); bit <<= 1, i++)
                if (work->playerHistory & bit)
                    fatigue &= ~1U;
        }
        work->fatigueFrames = 10;
    } else if (work->fatigueFrames) {
        if (--work->fatigueFrames == 0) {
            work->fatigueVoice = 0;
            fatigue &= ~2U;
            fatigue &= ~1U;
        } else
            fatigue |= 2;
    }
    player = 0;
    opponent = 0;
    for (bit = 1; bit <= 0xffff; bit <<= 1) {
        if (work->playerHistory & bit)
            player++;
        if (work->opponentHistory & bit)
            opponent++;
    }
    if (work->opponentRate - opponent > 1)
        opponent = work->opponentRate - 1;
    if (work->opponentRate - opponent < -1)
        opponent = work->opponentRate + 1;
    opponent = ArmwMinInt(ArmwMaxInt(opponent, work->minimumStrength), work->maximumStrength);
    work->opponentRate = opponent;
    if (work->frames % 60 == 0)
        FTBL.Loop3DSoundEffect(188);
    else if (work->frames % 120 == 0)
        FTBL.Loop3DSoundEffect(187);
    if (fatigue & 2) {
        if (opponent < 2)
            opponent = 2;
    }
    if ((fatigue & 2) && !work->settling) {
        work->acceleration -= 0.002f;
        if (!work->fatigueVoice) {
            work->fatigueVoice = 1;
            ArmwVoice(3, "011", 0);
        }
        if (work->acceleration < -0.03f)
            work->acceleration = -0.03f;
        movement = work->acceleration * opponent;
        ARMW_SNAP(movement);
        if (movement > 0.14f)
            movement = 0.14f;
        if (movement < -0.14f)
            movement = -0.14f;
    } else {
        if (player < opponent) {
            work->acceleration = -0.001f;
            movement = work->acceleration * (opponent - player);
        } else {
            work->acceleration = 0.001f;
            movement = work->acceleration * (player - opponent);
        }
        ARMW_SNAP(movement);
        if (movement > 0.06f)
            movement = 0.06f;
        if (movement < -0.06f)
            movement = -0.06f;
        if (work->strategyFlags & 1) {
            movement = 0.0f;
            work->target = work->position;
        }
        if (work->frames > 5400)
            movement = -0.06f;
    }
    if (!work->settling) {
        work->target += movement;
        if (work->target > 1.0f)
            work->target = 1.0f;
        if (work->target < 0.0f || __builtin_fabs(work->target) < ARMW_EPSILON)
            work->target = 0.0f;
    } else {
        if (work->position - work->target < 0.002f && work->position - work->target > -0.002f)
            work->settling = 0;
    }
    if (work->frames % 10 == 0 && rate > 2)
        work->recentRate = rate;
    difference = (int)__builtin_fabs((float)(player - opponent));
    if (difference <= 2 && !work->settling) {
        if (!(fatigue & 3)) {
            if (!work->fatigueFrames) {
                if (difference == 0)
                    work->stalemate -= 4;
                else if (difference == 1)
                    work->stalemate -= 2;
                else
                    work->stalemate -= 1;
            }
        }
        if (work->stalemate <= 0) {
            if (buttons & 1) {
                work->phase = 11;
                work->step = 0;
            } else
                work->stalemate = 0;
        }
    }
    shake = FTBL.GetRandomNumberInRange(20.0f) - 10.0f;
    ARMW_SNAP(shake);
    shake *= 0.0001f;
    ARMW_SNAP(shake);
    work->shake = shake;
    ArmwUpdatePose(work, 0);
    ArmwUpdateFaces(work);
    shake = work->target - work->position;
    if (__builtin_fabs(shake) >= ARMW_EPSILON) {
        step = !(fatigue & 2) ? 0.008f : 0.12f;
        if (shake > 0.0f) {
            if (shake > step)
                work->position += step;
            else
                work->position = work->target;
        } else {
            if (shake < -step)
                work->position -= step;
            else
                work->position = work->target;
        }
    }
    if (!work->settling) {
        if (work->position >= 0.85f) {
            work->step = 0;
            work->position = 0.8f;
            work->target = 0.8f;
            work->phase = 12;
        } else if (work->position < 0.01f) {
            ArmwRestoreHands(work);
            FTBL.EVS_LoopCamera(1306, ARMW_BASE, ARMW_BASE);
            work->step = 0;
            work->position = -1.75f;
            work->flags |= 2;
            work->phase = 16;
        }
    }
    return 0;
}

unsigned int ArmwButtons(ArmwWork *work)
{
    int adjustment = 0;
    unsigned int buttons = 0;
    unsigned int input = FTBL.GetCurrentControlInput(7);
    int strength;
    FTBL.GetBattleDataConfig(0, ARMW_BATTLE_INPUT);
    if (input == 1024)
        buttons |= 1;
    switch (work->strategy) {
    case 0:
        do {
            adjustment = 0;
        } while (0);
        break;
    case 1:
        if (work->position > 0.75f && work->stamina > 0) {
            if (!work->staminaVoice && !FTBL.cvsHasValidConverseData(0))
                work->staminaVoice = 1;
            work->stamina -= 3;
            adjustment = 6;
        } else if (work->position > 0.65f && work->stamina > 0) {
            work->stamina--;
            adjustment = 3;
        } else {
            work->staminaVoice = 0;
            adjustment = -3;
        }
        break;
    case 2:
        if (!work->staminaVoice && !FTBL.cvsHasValidConverseData(0))
            work->staminaVoice = 1;
        if (work->stamina-- > 0)
            adjustment = 6;
        else
            adjustment = -3;
        break;
    case 3:
        if (work->stamina-- <= 0)
            adjustment = 5;
        break;
    case 4:
        if (work->stamina > 0) {
            if (work->position > 0.6f)
                work->stamina--;
            if (work->position > 0.5f) {
                work->stamina -= 2;
                work->strategyFlags |= 1;
            } else
                adjustment = 2;
        } else
            adjustment = -4;
        break;
    case 5:
    case 6:
        if (work->position > 0.65f)
            work->stamina--;
        if (work->position > 0.75f) {
            if (work->stamina-- > 0) {
                work->strategyFlags |= 1;
                work->stamina -= 2;
            }
        }
        if (work->initialStamina / 2 < work->stamina)
            adjustment = 2;
        else
            adjustment = -2;
        if (work->strategy == 6 && work->position < 0.4f)
            work->stamina = work->stamina + 1 > work->initialStamina ? work->initialStamina : work->stamina + 1;
        break;
    default:
        adjustment = 0;
        break;
    }
    work->stamina = ARMW_MAX(work->stamina, 0);
    strength = ARMW_MIN(ARMW_MAX(work->strength + adjustment + FTBL.shenGetRandomInt32(2), 0), 31);
    if (strength > FTBL.shenGetRandomInt32(32))
        buttons |= 2;
    return buttons;
}

void ArmwUpdatePose(ArmwWork *work, int mode)
{
    unsigned int playerMotions[4] = {9169, 9164, 9164, 9168};
    unsigned int opponentMotions[4] = {9359, 9292, 9292, 9231};
    unsigned int flags = 0;
    float sum = work->position + work->shake;
    float frame;
    float scale;
    float opponentFrame;
    ARMW_SNAP(sum);
    frame = 1.0f - sum;
    ARMW_SNAP(frame);
    opponentFrame = frame;
    scale = 90.0f;
    ARMW_SNAP(scale);
    frame *= scale;
    if (__builtin_fabs(frame) < ARMW_EPSILON)
        frame = 90.0f;
    else
        frame += 90.0f;
    scale = 90.0f;
    ARMW_SNAP(scale);
    opponentFrame *= scale;
    if (__builtin_fabs(opponentFrame) < ARMW_EPSILON)
        opponentFrame = 90.0f;
    else
        opponentFrame += 90.0f;
    if (!(mode & 1))
        flags = 0x40000000U;
    EVT_AT_SetPos(work->player, ARMW_BASE, -0.03f, 0.0f, -0.8f, 0);
    EVT_AT_SetAng(work->player, ARMW_BASE, 0.0f, 180.0f, 0.0f, 0);
    switch (armwOpponentVariant) {
    case 0:
        EVT_AT_SetPos(work->opponent, work->player, 0.0f, 0.0f, -1.383f, 0);
        EVT_AT_SetAng(work->opponent, work->player, 0.0f, 180.0f, 0.0f, 0);
        break;
    case 1:
    case 2:
        EVT_AT_SetPos(work->opponent, work->player, 0.0f, 0.0f, -1.391f, 0);
        EVT_AT_SetAng(work->opponent, work->player, 0.0f, 0.0f, 0.0f, 0);
        break;
    case 3:
        EVT_AT_SetPos(work->opponent, work->player, 0.0f, 0.0f, -1.533f, 0);
        EVT_AT_SetAng(work->opponent, work->player, 0.0f, 180.0f, 0.0f, 0);
        break;
    }
    EVT_AT_SetMot(work->player, playerMotions[armwOpponentVariant], frame, 0.0f, flags | 0x04000000U, 0);
    EVT_AT_SetMot(work->opponent, opponentMotions[armwOpponentVariant], opponentFrame, 0.0f, flags | 0x04000000U, 0);
}

void ArmwUpdateFaces(ArmwWork *work)
{
    int player;
    int opponent;
    if (work->position < 0.2f)
        player = 10;
    else if (work->position >= 0.4f)
        player = 1;
    else
        player = 6;
    if (work->position < 0.2f)
        opponent = 13;
    else if (work->position < 0.4f)
        opponent = 1;
    else if (work->position < 0.6f)
        opponent = 6;
    else
        opponent = 10;
    if (player != work->playerFace)
        EVT_AT_SetFaceLooks(work->player, player, 10, 1.0f, 0);
    if (opponent != work->opponentFace)
        EVT_AT_SetFaceLooks(work->opponent, opponent, 10, 1.0f, 0);
    work->playerFace = player;
    work->opponentFace = opponent;
}

int ArmwDirectionQte(ArmwWork *work)
{
    float scale;
    unsigned int status;
    if (!work->step) {
        armwQtePenalty = 0.0f;
        FTBL.CheckHelpIcon(0, 0, 0);
        FTBL.EVS_LoopCamera(FTBL.shenGetRandomInt32(2) + 5010, ARMW_BASE, ARMW_BASE);
        if (FTBL.shenGetRandomInt32(100) < work->buttonUp)
            armwQteButton = 1;
        else if (FTBL.shenGetRandomInt32(100) < work->buttonDown)
            armwQteButton = 16;
        else if (FTBL.shenGetRandomInt32(100) < work->buttonLeft)
            armwQteButton = 4;
        else
            armwQteButton = 64;
        FTBL.SetShowQteFlagForButtonType((unsigned short)armwQteButton, 18, 2);
        FTBL.SetQteButtonState(0xfbff);
        work->step++;
    } else {
        if (!FTBL.GetShouldRenderIcons(3)) {
            scale = 0.7f / 11.0f;
            status = FTBL.GetCurrentQteStatus();
            if ((status & 4095) == armwQteButton) {
                if (armwQteSpeaker) {
                    if (work->position < 0.1f)
                        ArmwVoice(0, "016", 0);
                    else if (work->position < 0.22f)
                        ArmwVoice(0, "018", 0);
                    else if (work->position < 0.36f)
                        ArmwVoice(0, "010", 0);
                    else if (work->position < 0.48f)
                        ArmwVoice(0, "021", 0);
                    else if (work->position < 0.6f)
                        ArmwVoice(0, "008", 0);
                    else if (work->position < 0.72f)
                        ArmwVoice(0, "009", 0);
                    else
                        ArmwVoice(0, "007", 0);
                } else {
                    if (work->position < 0.1f)
                        ArmwVoice(3, "020", 0);
                    else if (work->position < 0.22f)
                        ArmwVoice(3, "023", 0);
                    else if (work->position < 0.36f)
                        ArmwVoice(3, "020", 0);
                    else if (work->position < 0.48f)
                        ArmwVoice(3, "019", 0);
                    else if (work->position < 0.6f)
                        ArmwVoice(3, "022", 0);
                    else if (work->position < 0.72f)
                        ArmwVoice(3, "021", 0);
                    else
                        ArmwVoice(3, "009", 0);
                }
                work->target = work->target + 1.3f * scale - armwQtePenalty;
            } else {
                if (armwQteSpeaker) {
                    if (work->position < 0.1f)
                        ArmwVoice(0, "013", 0);
                    else if (work->position < 0.22f)
                        ArmwVoice(0, "013", 0);
                    else if (work->position < 0.36f)
                        ArmwVoice(0, "015", 0);
                    else if (work->position < 0.48f)
                        ArmwVoice(0, "014", 0);
                    else if (work->position < 0.6f)
                        ArmwVoice(0, "012", 0);
                    else if (work->position < 0.72f)
                        ArmwVoice(0, "049", 0);
                    else
                        ArmwVoice(0, "015", 0);
                } else {
                    if (work->position < 0.1f)
                        ArmwVoice(3, "012", 0);
                    else if (work->position < 0.22f)
                        ArmwVoice(3, "009", 0);
                    else if (work->position < 0.36f)
                        ArmwVoice(3, "018", 0);
                    else if (work->position < 0.48f)
                        ArmwVoice(3, "016", 0);
                    else if (work->position < 0.6f)
                        ArmwVoice(3, "015", 0);
                    else if (work->position < 0.72f)
                        ArmwVoice(3, "014", 0);
                    else
                        ArmwVoice(3, "013", 0);
                }
                if ((status & 4095) != 0xa000)
                    armwQtePenalty += 0.015f;
                work->target = work->target - 1.1f * scale - armwQtePenalty / 10.0f;
            }
            armwQteSpeaker = (armwQteSpeaker + 1) % 2;
            if (__builtin_fabs(work->target) < ARMW_EPSILON)
                work->target = 0.0f;
            FTBL.EVS_LoopCamera(FTBL.shenGetRandomInt32(3) + 5000, ARMW_BASE, ARMW_BASE);
            EVT_AT_SetSound(2119, 0);
            work->stalemate = 250;
            work->settling = 45;
            work->phase = 10;
        }
        armwQtePenalty += 0.003f;
    }
    return 0;
}

int ArmwFinishQte(ArmwWork *work)
{
    short sequence[4];
    int buttons[4] = {64, 4, 1, 16};
    int count;
    int length;
    short i;
    if (!work->step) {
        if (work->finishQte) {
            count = work->finishQte & 0xff;
            length = work->finishQte >> 8;
            for (i = 0; i < 4; i++)
                sequence[i] = 0;
            for (i = 0; i < length - 1; i++)
                sequence[i] = buttons[FTBL.shenGetRandomInt32(4)];
            sequence[i] = 256;
            FTBL.qteBattleUtil(8, 30, 7 - count, (8 - count) * length + 5, 0);
            FTBL.qteBattleUtil(9, sequence[0], sequence[1], sequence[2], sequence[3]);
            FTBL.qteBattleUtil(10, 0, 0, 0, 0);
            FTBL.qteBattleUtil(1, 0, 0, 0, 0);
            FTBL.qteBattleUtil(3, 0, 0, 0, 0);
            work->step++;
        } else {
            ArmwRestoreHands(work);
            FTBL.EVS_LoopCamera(1301, ARMW_BASE, ARMW_BASE);
            FTBL.camCtrlSetRotationXYZ(0.1f, 0.18f, 0.25f);
            work->phase = 13;
            work->flags |= 1;
        }
    } else if (FTBL.qteBattleUtil(4, 0, 0, 0, 0)) {
        work->step = 0;
        if (FTBL.qteBattleUtil(4, 0, 0, 0, 0) == 1) {
            ArmwVoice(3, "023", 0);
            ArmwRestoreHands(work);
            FTBL.EVS_LoopCamera(1301, ARMW_BASE, ARMW_BASE);
            FTBL.camCtrlSetRotationXYZ(0.1f, 0.18f, 0.25f);
            work->phase = 13;
            work->flags |= 1;
        } else {
            ArmwVoice(3, "013", 0);
            work->phase = 10;
            work->settling = 45;
            work->target -= 0.15f;
        }
    }
    return 0;
}

void ArmwMatchHands(ArmwWork *work)
{
    int player[4] = {1029, 1028, 1028, 1027};
    int opponent[4] = {1034, 1026, 1026, 1024};
    FTBL.SetScenarioCharacterAnimation(FTBL.ctrlGetCharaByName(work->player), 0, player[armwOpponentVariant], 0, 1.0f, 1, 0, 4, 0);
    FTBL.SetScenarioCharacterAnimation(FTBL.ctrlGetCharaByName(work->opponent), 0, opponent[armwOpponentVariant], 0, 1.0f, 1, 0, 4, 0);
    FTBL.SetScenarioCharacterAnimation(FTBL.ctrlGetCharaByName(work->player), 1, player[armwOpponentVariant], 0, 1.0f, 1, 0, 4, 0);
    FTBL.SetScenarioCharacterAnimation(FTBL.ctrlGetCharaByName(work->opponent), 1, opponent[armwOpponentVariant], 0, 1.0f, 1, 0, 4, 0);
    FTBL.EV_SetPlayerFlag(12, 1292, 0);
    FTBL.EV_SetPlayerFlag(12, 1293, 1);
}

void ArmwRestoreHands(ArmwWork *work)
{
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(work->player), 1, 1, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(work->opponent), 1, 1, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(work->referee), 1, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(work->referee), 1, 1, 0);
    FTBL.CheckHelpIcon(0, 0, 0);
    FTBL.FinishLoading();
    work->step = 0;
}

int ArmwWinAnimation(ArmwWork *work)
{
    float position;
    if (work->position >= 3.0f) {
        FTBL.camCtrlSetRotationXYZ(0.0f, 0.0f, 0.0f);
        EVT_AT_SetFaceLooks(work->player, 0, 10, 1.0f, 0);
        EVT_AT_SetFaceLooks(work->opponent, 0, 10, 1.0f, 0);
        work->phase = 14;
    } else if (work->position >= 2.0f) {
        position = work->position += 0.006f;
        if (position >= 3.0f)
            position = 3.75f;
        else if (position > 2.8f && position <= 2.81f)
            FTBL.Loop3DSoundEffect(344);
        work->position = 1.0f;
        ArmwUpdatePose(work, 0);
        work->position = position;
    } else if (work->position >= 1.0f) {
        position = work->position += 0.02f;
        if (position >= 2.0f) {
            FTBL.EVS_LoopCamera(1303, ARMW_BASE, ARMW_BASE);
            position = 2.8f;
            FTBL.camCtrlSetRotationXYZ(0.05f, 0.09f, 0.25f);
        } else if (position > 1.95f && position <= 1.975f)
            FTBL.Loop3DSoundEffect(343);
        else if (position > 1.85f && position <= 1.875f)
            FTBL.camCtrlSetRotationXYZ(0.05f, 0.09f, 0.25f);
        work->position -= 1.0f;
        ArmwUpdatePose(work, 0);
        work->position = position;
    } else {
        position = work->position += 0.02f;
        if (position >= 1.0f) {
            FTBL.EVS_LoopCamera(1302, ARMW_BASE, ARMW_BASE);
            position = 1.8f;
        } else if (position > 0.85f && position <= 0.875f)
            FTBL.Loop3DSoundEffect(343);
        else if (position > 0.75f && position <= 0.775f)
            FTBL.camCtrlSetRotationXYZ(0.05f, 0.09f, 0.25f);
        ArmwUpdatePose(work, 0);
        work->position = position;
    }
    return 0;
}

int ArmwLoseAnimation(ArmwWork *work)
{
    float position;
    if (work->position < -2.0f) {
        EVT_AT_SetFaceLooks(work->player, 0, 10, 1.0f, 30);
        EVT_AT_SetFaceLooks(work->opponent, 0, 10, 1.0f, 30);
        work->phase = 17;
        FTBL.HLib_SetupTaskDelay(30);
        return 0;
    }
    if (work->position < -1.0f) {
        position = work->position -= 0.025f;
        if (position < -2.0f || __builtin_fabs(position) < ARMW_EPSILON)
            position = -2.75f;
        else if (position < -1.86f && position >= -1.8875f) {
            FTBL.Loop3DSoundEffect(344);
            FTBL.camCtrlSetRotationXYZ(0.05f, 0.18f, 0.25f);
        }
        work->position += 2.0f;
        if (__builtin_fabs(work->position) < ARMW_EPSILON)
            work->position = 0.0f;
        ArmwUpdatePose(work, 0);
        work->position = position;
    } else if (work->position < 0.0f) {
        position = work->position -= 0.04f;
        if (position < -1.0f || __builtin_fabs(position) < ARMW_EPSILON) {
            FTBL.Loop3DSoundEffect(343);
            position = -1.75f;
        }
        work->position += 1.0f;
        if (__builtin_fabs(work->position) < ARMW_EPSILON)
            work->position = 0.0f;
        ArmwUpdatePose(work, 0);
        work->position = position;
    } else {
        position = work->position -= 0.04f;
        if (position < 0.0f || __builtin_fabs(position) < ARMW_EPSILON) {
            FTBL.Loop3DSoundEffect(343);
            position = -0.75f;
        }
        if (__builtin_fabs(work->position) < ARMW_EPSILON)
            work->position = 0.0f;
        ArmwUpdatePose(work, 0);
        work->position = position;
    }
    return 0;
}

int ArmwOpponentLoaded(shen_name name)
{
    FTBL.CheckWarmStartNpcRoute(name);
    if (FTBL.ForceLoadNpc(name)) {
        FTBL.EV_SetCharacterAlphaEnabled(name, 0);
        FTBL.EV_SetCharaControllerFlags(name, 2, 0);
        FTBL.EV_SetCharaControllerFlags(name, 3, 0);
        FTBL.EV_SetCharaControllerFlags(name, 1, 0);
        FTBL.SetRefBoardParamsForCharaById(name, 1);
        FTBL.AT_SetLocalBaseChara(name, ARMW_INVALID);
        FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(name), 3);
        FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(name), 7, 0);
        FTBL.EV_EnableCharacterDisplay(name);
        FTBL.GetCharacterFaceCacheIndex(FTBL.ctrlGetCharaByName(name));
        FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(name), 1);
        FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(name));
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(name), 1, 0, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(name), 1, 1, 0);
        FTBL.charaSetDisplayEnabledFlag(FTBL.ctrlGetCharaByName(name), 0);
        return 1;
    }
    return 0;
}

#endif
#else
#include "s2/areas/area_armw.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/pause.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sk_event.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
#include <cstring>
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
ArmwState armw{};
void ArmwNullsub() {
}
std::int32_t GetArmwGameplayActive() {
    return armw.active;
}
namespace {
constexpr const char *refereePackages[]{"ARMWSAMM", "ARMWLLYM"};
constexpr const char *opponentPackages[]{"ARMWSAMR", "ARMWLLYR", "ARMWLLYR", "ARMWHGNR"};
constexpr shen_name referees[]{SHEN_NAME(GAU_), SHEN_NAME(WD3_), SHEN_NAME(AD2_), SHEN_NAME(SD1_), SHEN_NAME(SD1_), SHEN_NAME(SD1_), SHEN_NAME(WD1_),
    SHEN_NAME(WD1_), SHEN_NAME(WD1_), SHEN_NAME(WD1_), SHEN_NAME(WD1_), SHEN_NAME(WD1_)};
constexpr std::int32_t nextEntries[]{20, 21, 22, 0, 0, 0, 20, 21, 22, 0, 0, 0};
std::int32_t leftHandPose[19][3]{{0, 0, 58253}, {0, 2182, 61167}, {0, 0, 5824}, {0, 65172, 53886}, {0, 1635, 52429}, {0, 0, 55705}, {0, 0, 0}, {0, 1456, 56434},
    {0, 0, 50425}, {0, 726, 54796}, {0, 0, 0}, {0, 2183, 57891}, {0, 0, 51518}, {0, 727, 54795}, {0, 182, 0}, {2912, 2001, 53156}, {725, 65353, 52613},
    {0, 910, 54431}, {65518, 255, 0}};
void MainCallback(void *);
void EndCallback(void *);
void Phase(ArmwWork &work, std::int16_t phase, std::int16_t step = 0) {
    work.step = step;
    work.phase = phase;
}
float Tick() {
    return g_stabilizeFps ? GetFrameTime() * 30.0f : 1.0f;
}
void Fade(std::int32_t frames, bool toBlack) {
    SetScreenFilmFade(frames, toBlack ? 0 : 255, 0, 0, 0, toBlack ? 255 : 0, 0, 0, 0);
}
void QueueFade(std::int32_t frames, bool toBlack, std::int32_t delay) {
    EVT_AT_MenCtrl({.cleanup = 0,
        .frames = frames,
        .startAlpha = toBlack ? 0 : 255,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = toBlack ? 255 : 0,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = static_cast<float>(delay)});
}
void FreePackage(const char *name) {
    if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(name), 0))
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(name), 0);
}
void LoadPackage(const char *name) {
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>(name));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
}
// @verified - p
void FreeRefereePackages() {
    for (const char *name : refereePackages)
        FreePackage(name);
}
// @verified - p
void FreeOpponentPackages() {
    for (const char *name : opponentPackages)
        FreePackage(name);
}
void LoadOpponentPackage() {
    FreeOpponentPackages();
    LoadPackage(opponentPackages[armw.opponentMotion]);
}
void LoadMatchPackages() {
    LoadPackage("ARMWCOMN");
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("ARMWCOMN"), 0);
    armw.handMotion = LoadHandPoseBank(g_actNumber, g_currentMap, "HMOT_0008E");
    LoadPackage(refereePackages[armw.refereeMotion]);
    LoadOpponentPackage();
}
void ReleaseHandMotion() {
    if (armw.handMotion != reinterpret_cast<void *>(~std::uintptr_t{}))
        ReleaseHandPoseBank(armw.handMotion);
}
void PrepareHands(shen_name name) {
    SetLowQualityHands(ctrlGetCharaByName(name));
    SetHighModelXYZ(ctrlGetCharaByName(name), 1, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(name), 1, 1, 0);
}
// @verified - p
void PinBase() {
    const float *base = ARMW_BASES[armw.location];
    ArmwPosition(ARMW_BASE, EVENT_CHARACTER_INVALID, {base[0], base[1], base[2]}, static_cast<std::int32_t>(base[3] * 8192.0f / 45.0f));
}
std::int32_t OpponentMotion(shen_name opponent) {
    if (opponent.name == SHEN_NAME(SFC_) || opponent.name == SHEN_NAME(DGY_) || opponent.name == SHEN_NAME(MHK_))
        return 2;
    const std::int32_t bone = mtwkGetRigBoneId(scenFindMotionwork(opponent));
    return bone == 7 ? 0 : bone == 9 ? 1 : 3;
}
std::int32_t Ladder(const ArmwWork &work) {
    return work.flags & 0x40000000U ? armw.location : ARMW_ROTATIONS[work.day][armw.location][work.rotation];
}
// @verified - p
void SelectOpponent(ArmwWork &work) {
    armw.bet = work.flags & 0x10000000U ? 20 : work.flags & 0x20000000U ? 100 : ARMW_BETS[armw.location][work.opponentIndex];
    const ArmwOpponent &opponent = armwOpponents[ARMW_OPPONENT_ORDER[Ladder(work)][work.opponentIndex]];
    work.opponent = opponent.character;
    work.strength = opponent.strength;
    work.minimumStrength = opponent.minimumStrength;
    work.maximumStrength = opponent.maximumStrength;
    work.stamina = opponent.stamina;
    work.initialStamina = opponent.stamina;
    work.strategy = static_cast<std::uint16_t>(opponent.strategy);
    work.warmupFrames = opponent.warmupFrames;
    work.buttonA = opponent.buttonA;
    work.buttonLeft = opponent.buttonLeft;
    work.buttonDown = opponent.buttonDown;
    work.buttonUp = opponent.buttonUp;
    work.finishQte = opponent.finishQte;
    armw.opponentMotion = OpponentMotion(work.opponent);
}
// @verified - p
void ResetAuthFlags() {
    for (std::int32_t flag = 0x23c; flag <= 0x23e; ++flag)
        if (EV_GetShenmueFlag(flag)) {
            EV_stepSetFlags(0, flag);
            break;
        }
}
// @verified - p
void FinishLadderFlags() {
    if (armw.location != 0 || EV_GetShenmueFlag(0x23e))
        for (std::int32_t flag = 0x233; flag <= 0x235; ++flag)
            if (EV_GetShenmueFlag(flag)) {
                EV_stepSetFlags(0, flag);
                break;
            }
}
void SelectCharacters(ArmwWork &work) {
    for (std::int32_t day = 0; day < 7; ++day)
        if (EV_GetShenmueFlag(0x22c + day)) {
            work.day = day;
            break;
        }
    if (armw.location == 0 || armw.location == 1)
        for (std::int32_t rotation = 0; rotation < 3; ++rotation)
            if (EV_GetShenmueFlag(0x233 + 3 * armw.location + rotation)) {
                work.rotation = rotation;
                break;
            }
    work.opponentIndex = static_cast<std::uint8_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, ARMW_FLAGS[armw.location]));
    work.flags |= 0x40000000U;
    work.nextScene = shen_name{};
    work.nextEntry = nextEntries[armw.location];
    work.player = ARMW_RYO;
    work.referee = referees[armw.location];
    armw.refereeMotion = mtwkGetRigBoneId(scenFindMotionwork(work.referee)) == 7 ? 0 : 1;
    SelectOpponent(work);
    armw.characters[0] = work.player;
    armw.characters[1] = work.referee;
    if (work.flags & 0x20000000U) {
        const std::int32_t ladder = Ladder(work);
        armw.characters[2] = armwOpponents[ARMW_OPPONENT_ORDER[ladder][0]].character;
        armw.characters[3] = armwOpponents[ARMW_OPPONENT_ORDER[ladder][1]].character;
        armw.characters[4] = armwOpponents[ARMW_OPPONENT_ORDER[ladder][2]].character;
        armw.characters[5] = EVENT_CHARACTER_INVALID;
        work.secondOpponent = armw.characters[3];
        work.thirdOpponent = armw.characters[4];
    } else {
        armw.characters[2] = work.opponent;
        armw.characters[3] = EVENT_CHARACTER_INVALID;
    }
    if ((EV_GetShenmueFlag(3) || EV_GetShenmueFlag(2)) && armw.location == 0)
        work.flags |= 8;
}
// @verified - p
void InitializeResources(ArmwWork &work) {
    armw.handMotion = reinterpret_cast<void *>(~std::uintptr_t{});
    EV_SetNewSceneIndex();
    PrepareHands(ARMW_RYO);
    PrepareHands(work.referee);
    PrepareHands(work.opponent);
    if (g_currentMap.name == SHEN_NAME(AR02))
        EV_LoadNPCRoute("NPC_ARAW.BIN");
    GetCharacterMotionCompatibilityResult(ctrlGetCharaByName(ARMW_RYO));
    SetFaceFlagsForChara_0(ctrlGetCharaByName(ARMW_RYO), FacialNodeReplacement);
    qteBattleUtil(BattleQteMode::Queue);
    qteBattleUtil(BattleQteMode::SetWait, 45, 6, 18);
    qteBattleUtil(BattleQteMode::SetFirstCommands);
    qteBattleUtil(BattleQteMode::SetLastCommands);
    qteBattleUtil(BattleQteMode::ResetStates);
    qteBattleUtil(BattleQteMode::Load);
    EV_FreeAuthorPak(1, g_currentMap == AREA_QTB4 ? "0368" : "0008");
    ArmwSetVoicePrefixes(work.referee, work.opponent);
    LoadPackage("ARMW");
    EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("ARMW"), 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("ARMW"), 0);
    EV_LoadCutsceneFromCurrentScene("0008A000");
    CreateEventCharacter(CharacterCreationHighModel, nullptr, ARMW_BASE);
    armw.opponentMotion = OpponentMotion(work.opponent);
    if (!EV_GetShenmueFlag(2) || armw.location != 0)
        LoadMatchPackages();
    else {
        for (std::int32_t i = 0; i < 6; ++i)
            EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, ARMW_FLAGS[i], 0);
        LoadPackage("ARMWAUTH");
        armw.handMotion = LoadHandPoseBank(g_actNumber, g_currentMap, "HMOT_0008E");
        Queue_EVT_UTL_LoadNPCChara_3(armw.tutorialCharacters, work.self);
    }
    charaSetDisplayEnabledFlag(ctrlGetCharaByName(work.player), 0);
    charaSetDisplayEnabledFlag(ctrlGetCharaByName(work.opponent), 0);
    charaSetDisplayEnabledFlag(ctrlGetCharaByName(work.referee), 0);
    if (DoesScenarioCharacterExist(ARMW_ROCKO))
        charaSetDisplayEnabledFlag(ctrlGetCharaByName(ARMW_ROCKO), 0);
    work.phase = 1;
}
std::int32_t Ready(ArmwWork &work) {
    work.falseStarts = 0;
    PinBase();
    ArmwPosition(ARMW_MONEY, ARMW_BASE, {-0.2f, 0.9f, -0.16f}, 0);
    EV_EnableCharacterDisplay(work.player);
    EV_EnableCharacterDisplay(work.opponent);
    EV_EnableCharacterDisplay(work.referee);
    CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
    if (work.flags & 0x20000000U) {
        EV_ToggleCharacterFlag(work.secondOpponent);
        EV_DisableCharacterDisplay(work.secondOpponent);
        EV_ToggleCharacterFlag(work.thirdOpponent);
        EV_DisableCharacterDisplay(work.thirdOpponent);
    }
    if (!EV_GetShenmueFlag(2) || armw.location != 0) {
        FinishLoading();
        EV_LoadAudio("0008A000", 1);
        EV_spmBeginSampleExecute();
        Phase(work, 5, 1);
        Fade(30, false);
    } else
        work.phase = 2;
    EV_NPC_NewLoadSw(1);
    return 0;
}
std::int32_t Tutorial(ArmwWork &work) {
    if (work.step) {
        hndlSetFlags(ctrlGetCharaByName(ARMW_SHIYO));
        for (shen_name name : armw.tutorialCharacters) {
            if (name.name == EVENT_CHARACTER_INVALID.name)
                break;
            if (DoesScenarioCharacterExist(name)) {
                EV_npcLockUnlock(name);
                EV_npcFree(name);
            }
        }
        ReleaseHandMotion();
        FreePackage("ARMWAUTH");
        EV_NPC_NewLoadSw(0);
        EV_ResetFreeTalkFlags();
        LoadMatchPackages();
        EV_NPC_NewLoadSw(1);
        Fade(30, false);
        Phase(work, 3);
    } else {
        EV_LoadAudio("0008A000", 0);
        EV_spmBeginSampleExecute();
        SetPlayerGameFlags(PlayerGameFlagMode::Bit5, reinterpret_cast<void *>(1));
        Fade(30, false);
        QueueSkAuthorStart(work.self, SHEN_NAME(YASA), nullptr, nullptr, 0);
        EV_stepSetFlags(0, 2);
        PrepareHands(ARMW_SHIYO);
        ConfigureCharacterHandPoseSequence(ctrlGetCharaByName(ARMW_HONG), 0, 1025, 0, 1, 0, 4, 1, 0);
        ConfigureCharacterHandPoseSequence(ctrlGetCharaByName(ARMW_SHIYO), 0, 1033, 0, 1, 0, 4, 1, 0);
        ++work.step;
    }
    return 0;
}
std::int32_t WalkAway(ArmwWork &work) {
    SetPlayerGameFlags(PlayerGameFlagMode::Bit5, reinterpret_cast<void *>(1));
    EVT_AT_SetMot(work.player, 9165, 0, 1, 0x40000000U, 40);
    EVT_AT_SetMot(work.opponent, ARMW_OPPONENT_MOTIONS[0][armw.opponentMotion], 0, 1, 0x40000000U, 25);
    ArmwVoice(0, "027");
    ArmwVoice(2, "029", 90);
    ArmwCamera(1803, 40);
    EVT_AT_SetPoint(work.referee, work.player, 5, 3, 0, 40, 180, 0);
    work.flags |= 0x20;
    return 180;
}
std::int32_t AskAfterTutorial(ArmwWork &work) {
    if (work.step) {
        if (work.step != 1)
            return 0;
        if (shenGetCtrlSpeakIconFlag() == 22) {
            ArmwVoice(0, "028");
            Phase(work, 5);
            return 30;
        }
    } else {
        EV_LoadAudio("0008A000", 1);
        FinishLoading();
        ApplyCharacterHandPoseCommand(ctrlGetCharaByName(work.player), 1, reinterpret_cast<std::byte *>(leftHandPose), 0);
        EVT_AT_SetMot(work.player, 61442, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(work.opponent, ARMW_OPPONENT_MOTIONS[0][armw.opponentMotion], 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(work.referee, ARMW_REFEREE_MOTIONS[0][armw.refereeMotion], 0, 1, 0x40000000U, 0);
        ArmwPosition(work.player, ARMW_BASE, {-4.3f, 0, -4.1f}, 44600);
        ArmwPosition(work.opponent, ARMW_BASE, {-1.2f, 0, 0.9f}, 0);
        ArmwPosition(work.referee, ARMW_BASE, {-2.9f, 0, -3.5f}, 11832);
        ArmwCamera(2101);
        if (GetPlayerStat(PlayerStatsId::Cash) > static_cast<std::uint32_t>(armw.bet)) {
            QueueSkChoiceWait(work.self, 0, 22, 153, -1, 153, 0, 10);
            ++work.step;
            return 0;
        }
    }
    const std::int32_t frames = WalkAway(work);
    Phase(work, 20);
    return frames;
}
std::int32_t Explain(ArmwWork &work) {
    std::int32_t frames = 0;
    switch (work.step) {
    case 0:
        EVT_AT_SetVoice("0008E065", 0); // @text EN: "All right!\nThat's the spirit!" | JA: "よっしゃ！\nそうこなくっちゃ！"
        EVT_AT_SetVoice("0008E066", 80); // @text EN: "You're Japanese, aren't you?\nWhat's your name?" | JA: "兄ちゃん日本人だろ。…名前は？"
        EVT_AT_SetVoice("0008A001", 180); // @text EN: "Ryo.\nRyo Hazuki." | JA: "涼…芭月涼だ。"
        EVT_AT_SetVoice("0008E067", 260); // @text EN: "Ryo Hazuki, eh?" | JA: "ハヅキリョウか…"
        EVT_AT_SetVoice("0008E068", 320); // @text EN: "Okay!\nLet's go!" | JA: "よーし！\nさあ、入った入った！"
        ArmwCamera(2130);
        ArmwCamera(2131, 180);
        ArmwCamera(2132, 260);
        EVT_AT_SetMot(work.referee, 9095, 60, 1, 0x40000000U, 260);
        SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(work.referee), Smile, 0, 1);
        EVT_AT_SetFaceLooks(work.referee, Normal, 0, 1, 460);
        QueueFade(30, true, 400);
        frames = 460;
        ++work.step;
        break;
    case 1:
        Fade(30, false);
        SetPlayerGameFlags(PlayerGameFlagMode::Bit5, nullptr);
        ArmwPosition(work.player, ARMW_BASE, {0.5f, 0, -0.8f}, 28672);
        ArmwPosition(work.referee, ARMW_BASE, {1.2f, 0, 0.6f}, 61440);
        ArmwPosition(work.opponent, ARMW_BASE, {0, 0, 0.8f}, 0);
        EVT_AT_SetMot(work.player, 61443, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(work.referee, ARMW_REFEREE_MOTIONS[13][armw.refereeMotion], 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(work.opponent, ARMW_OPPONENT_MOTIONS[1][armw.opponentMotion], 0, 1, 0x40000000U, 0);
        ArmwCamera(2150);
        EVT_AT_SetSound(188, 10);
        EVT_AT_SetSound(187, 30);
        EVT_AT_SetSound(271, 40);
        ArmwAppendVoice(work.referee.name == SHEN_NAME(SD1_) ? 1 : 2, "009");
        ArmwAppendVoice(2, "010");
        ArmwWaitVoices(work.self, 0x80000000U);
        ++work.step;
        break;
    case 2:
        if (armw.voiceSkipped) {
            Phase(work, 4, 1);
            Fade(0, true);
            break;
        }
        EVT_AT_SetMot(work.player, 9157, 20, 0.7f, 0x40000000U, 0);
        EVT_AT_SetMot(work.referee, ARMW_REFEREE_MOTIONS[13][armw.refereeMotion], 200, 1, 0x40000000U, 0);
        ArmwAppendVoice(1, "011");
        ArmwCamera(2151);
        EVT_AT_SetSound(187, 30);
        EVT_AT_SetSound(188, 40);
        EVT_AT_SetSound(271, 50);
        EVT_AT_SetSound(348, 60);
        ArmwWaitVoices(work.self, 0x80000000U);
        ++work.step;
        break;
    case 3:
        if (armw.voiceSkipped) {
            Phase(work, 4, 1);
            Fade(0, true);
            break;
        }
        EV_DisableCharacterDisplay(work.opponent);
        EVT_AT_SetMot(work.player, 61443, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(work.referee, ARMW_REFEREE_MOTIONS[13][armw.refereeMotion], 320, 1, 0x40000000U, 0);
        ArmwPosition(work.player, ARMW_BASE, {0.5f, 0, -0.8f}, 28672);
        ArmwAppendVoice(2, "012");
        ArmwAppendVoice(2, "001");
        ArmwCamera(2152);
        ArmwWaitVoices(work.self, 0x80000000U);
        ++work.step;
        break;
    case 4:
        if (armw.voiceSkipped) {
            Phase(work, 4, 1);
            Fade(0, true);
            break;
        }
        EVT_AT_SetSound(187, 20);
        EVT_AT_SetSound(349, 60);
        EVT_AT_SetSound(187, 120);
        frames = ArmwIntroduceOpponent(work);
        ArmwVoice(2, "002");
        ArmwVoice(3, "007", frames - 60);
        Phase(work, 4, 1);
        EV_DisableCharacterDisplay(work.referee);
        EV_EnableCharacterDisplay(work.opponent);
        frames += 30;
        EVT_AT_ObjectDisp_1(work.referee, frames, 0);
        QueueFade(60, true, frames - 60);
        break;
    }
    return frames;
}
std::int32_t PrepareMatch(ArmwWork &work) {
    std::int32_t frames = 0;
    switch (work.step) {
    case 0:
        SetPlayerGameFlags(PlayerGameFlagMode::Bit5, nullptr);
        EV_LoadAudio("0008A000", 1);
        EV_spmBeginSampleExecute();
        Fade(30, false);
        frames = ArmwIntroduceOpponent(work);
        EV_DisableCharacterDisplay(work.referee);
        EVT_AT_ObjectDisp_1(work.referee, frames, 0);
        QueueFade(30, true, frames - 30);
        QueueFade(30, false, frames);
        ++work.step;
        break;
    case 1:
        SetPlayerGameFlags(PlayerGameFlagMode::Bit5, nullptr);
        work.position = 0.5f;
        if (!EV_GetScreenFilmIsNotShowing())
            Fade(30, false);
        EV_EnableCharacterDisplay(work.opponent);
        EV_EnableCharacterDisplay(work.referee);
        ArmwMatchHands(work);
        ArmwUpdatePose(work);
        ArmwPosition(work.referee, ARMW_BASE, {0.7f, 0, 0.05f}, 16384);
        ArmwPosition(ARMW_MONEY, ARMW_BASE, {-0.2f, 0.9f, -0.16f}, 0);
        EVT_AT_SetMot(work.referee, ARMW_REFEREE_MOTIONS[3][armw.refereeMotion], 0, 1, 0x40000000U, 0);
        EVT_AT_SetPoint(work.player, work.opponent, 5, 3, 0, 0, 0, 0xa0000000U);
        EVT_AT_SetPoint(work.opponent, work.player, 5, 3, 0, 0, 0, 0xa0000000U);
        ArmwCamera(3000);
        armw.introductionVariant = shenGetRandomInt32(2);
        ArmwAppendVoice(3, armw.started ? (armw.introductionVariant ? "003" : "004") : "001");
        ArmwWaitVoices(work.self);
        ++work.step;
        break;
    case 2: {
        constexpr const char *taunts[11][3]{{"002", "003", "004"}, {"029", "030", "031"}, {"034", "035", "036"}, {"039", "040", "041"}, {"043", "044", "045"},
            {"002", "028", "045"}, {"067", "068", "069"}, {"050", "051", "052"}, {"063", "064", "030"}, {"061", "031", "062"}, {"055", "056", "057"}};
        std::int32_t index = 0;
        switch (work.opponent.name) {
        case SHEN_NAME(SF8_):
            index = 1;
            break;
        case SHEN_NAME(SFC_):
            index = 2;
            break;
        case SHEN_NAME(KZN_):
            index = 3;
            break;
        case SHEN_NAME(SSY_):
            index = 4;
            break;
        case SHEN_NAME(SF2_):
            index = 5;
            break;
        case SHEN_NAME(FRS_):
            index = 6;
            break;
        case SHEN_NAME(MHK_):
            index = 7;
            break;
        case SHEN_NAME(DML_):
            index = 8;
            break;
        case SHEN_NAME(MSH_):
            index = 9;
            break;
        case SHEN_NAME(DGY_):
            index = 10;
            break;
        }
        ArmwCamera(3001);
        ArmwAppendVoice(0, taunts[index][armw.started ? (armw.introductionVariant ? 1 : 2) : 0]);
        ArmwWaitVoices(work.self);
        ++work.step;
        break;
    }
    case 3:
        ArmwCamera(3002);
        ArmwAppendVoice(3, armw.started ? (armw.introductionVariant ? "005" : "006") : "002");
        ArmwWaitVoices(work.self);
        ++work.step;
        break;
    case 4:
        if (!armw.started) {
            EVT_AT_SetMot(work.referee, ARMW_REFEREE_MOTIONS[10][armw.refereeMotion], 0, 1, 0x40000000U, 0);
            ArmwCamera(3003);
            ArmwAppendVoice(1, "013");
            ArmwWaitVoices(work.self);
        }
        ++work.step;
        break;
    case 5: {
        if (!armw.startFrames) {
            armw.started = 1;
            EVT_AT_SetPoint(work.player, shen_name{}, 0, 3, 0, 0, 0, 0x10000000U);
            EVT_AT_SetPoint(work.opponent, shen_name{}, 0, 3, 0, 0, 0, 0x10000000U);
            ArmwMatchHands(work);
            ArmwUpdatePose(work);
            ArmwPosition(work.referee, ARMW_BASE, {0.7f, 0, 0.05f}, 16384);
            EVT_AT_SetMot(work.referee, ARMW_REFEREE_MOTIONS[11][armw.refereeMotion], 0, 1, 0x40000000U, 0);
            ArmwCamera(3004);
            ArmwVoice(1, "014");
        }
        if (armw.startFrames == 40)
            ArmwCamera(3005);
        const std::int32_t frame = armw.startFrames++;
        if (frame <= 50) {
            if (!(GetCurrentControlInput(INPUT_NEW_PRESSES) & 0x400))
                break;
            CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
            Phase(work, 9);
        } else {
            GetBattleDataConfig(0, ARMW_BATTLE_INPUT);
            EVT_AT_SetMot(work.referee, ARMW_REFEREE_MOTIONS[12][armw.refereeMotion], 120, 1, 0, 0);
            ArmwVoice(1, "015");
            ArmwResetMatch(work);
            Phase(work, 10);
        }
        armw.startFrames = 0;
        break;
    }
    }
    return frames;
}
std::int32_t Refuse(ArmwWork &work) {
    ArmwAppendVoice(0, "027");
    ArmwAppendVoice(1, "029");
    ArmwAppendVoice(1, "030");
    work.phase = 20;
    ArmwWaitVoices(work.self);
    return 220;
}
std::int32_t Rematch(ArmwWork &work) {
    if (!work.step) {
        const std::int32_t frames = ArmwOfferRematch(work);
        if (shenGetMinutesUntilNextFrameMinute()) {
            ArmwCamera(3205, frames);
            QueueSkChoiceWait(work.self, 0, 22, 153, -1, 153, 0, frames);
            ++work.step;
            return 0;
        }
        EVT_AT_ObjectDisp_1(work.opponent, frames, 1);
        Phase(work, 7);
        return frames;
    }
    if (work.step == 1) {
        if (shenGetCtrlSpeakIconFlag() == 22) {
            ArmwVoice(0, "028");
            Fade(30, true);
            Phase(work, 4, 1);
            return 30;
        }
        Phase(work, 7);
    }
    return 0;
}
std::int32_t FalseStart(ArmwWork &work) {
    std::int32_t frames = ArmwWarnFalseStart(work);
    ++work.falseStarts;
    if (work.falseStarts >= 3) {
        Phase(work, 17, 1);
        frames += 30;
    } else
        Phase(work, 4, 4);
    QueueFade(30, false, frames);
    return frames;
}
// @verified - p
void AdvanceTournament(ArmwWork &work) {
    HasCharacterMotionWork(ctrlGetCharaByName(work.opponent));
    hndlSetFlags(ctrlGetCharaByName(work.opponent));
    charaSetDisplayEnabledFlag(ctrlGetCharaByName(work.opponent), 0);
    EV_ToggleCharacterFlag(work.opponent);
    EV_DisableCharacterDisplay(work.opponent);
    ++work.opponentIndex;
    work.phase = 4;
    work.falseStarts = 0;
    SelectOpponent(work);
    LoadOpponentPackage();
    EV_EnsureCharacterDraw(work.opponent);
    EV_EnableCharacterDisplay(work.opponent);
    GetCharacterMotionCompatibilityResult(ctrlGetCharaByName(work.opponent));
    SetFaceFlagsForChara_0(ctrlGetCharaByName(work.opponent), FacialNodeReplacement);
    PrepareHands(work.opponent);
    ArmwSetVoicePrefixes(work.referee, work.opponent);
}
void OpponentLoseMotion(ArmwWork &work, float speed) {
    std::uint32_t motion;
    float frame;
    switch (work.opponent.name) {
    case SHEN_NAME(SF2_):
    case SHEN_NAME(SF8_):
    case SHEN_NAME(DML_):
        EVT_AT_SetAngY(work.opponent, work.opponent, -90, 0);
        motion = 9232;
        frame = 20;
        break;
    case SHEN_NAME(SFC_):
    case SHEN_NAME(MHK_):
        motion = 9297;
        frame = 80;
        break;
    case SHEN_NAME(HNG_):
    case SHEN_NAME(MSH_):
    case SHEN_NAME(SSY_):
        motion = 9299;
        frame = 30;
        break;
    case SHEN_NAME(KZN_):
    case SHEN_NAME(FRS_):
        motion = 9360;
        frame = 70;
        break;
    case SHEN_NAME(DGY_):
        motion = 9295;
        frame = 30;
        break;
    default:
        return;
    }
    EVT_AT_SetMot(work.opponent, motion, frame, speed, 0x40000000U, 0);
}
void QueueCling(shen_name character, std::int32_t bone, const Vector3 &position, const Vector3i &rotation, std::int32_t delay) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), AREA_EVENT_TASK);
    RegisterEventTask(task);
    EventCling &request = *static_cast<EventCling *>(HLib::GetTaskParameter(task));
    request.cleanup = 1;
    request.target = ARMW_MONEY;
    request.character = character;
    request.bone = bone;
    request.position = position;
    request.rotation = rotation;
    request.mode = 0;
    request.delay = static_cast<float>(delay);
}
std::int32_t Win(ArmwWork &work) {
    if (!work.step) {
        ArmwPosition(work.player, ARMW_BASE, {-0.02f, 0, -0.76f}, 32768);
        ArmwPosition(work.opponent, ARMW_BASE, {0.02f, 0, 0.76f}, 0);
        ArmwPosition(work.referee, ARMW_BASE, {1.5f, 0, 0}, 57344);
        EVT_AT_SetAngY(work.referee, ARMW_BASE, 45, 120);
        EVT_AT_SetMot(work.player, 9167, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(work.opponent, ARMW_OPPONENT_MOTIONS[3][armw.opponentMotion], 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(work.referee, ARMW_REFEREE_MOTIONS[0][armw.refereeMotion], 0, 1, 0x40000000U, 90);
        ArmwVoice(0, "022", 60);
        Loop3DSoundEffect(349);
        EVT_AT_SetSound(269, 20);
        EVT_AT_SetSound(347, 30);
        EVT_AT_SetSound(349, 10);
        ArmwCamera(shenGetRandomInt32(3) + 3100);
        if (!(work.flags & 0x20000000U) || work.opponentIndex == 2) {
            IncrementSteamStat(20);
            ++work.step;
        } else
            AdvanceTournament(work);
        if (work.flags & 8)
            Phase(work, 18);
        if (work.phase != 14 || work.step != 1)
            return 120;
        work.authorFrames = 120;
        work.step = 10;
        return 0;
    }
    if (work.step == 10) {
        if (HasPressedCancel()) {
            EV_CleanupMapControlTask();
            work.authorFrames = 0;
        }
        work.authorFrames -= Tick();
        if (work.authorFrames <= 0)
            work.step = 1;
        return 0;
    }
    if (work.step != 1)
        return 0;
    ArmwPosition(work.player, ARMW_BASE, {-0.4f, 0, -0.7f}, 32768);
    ArmwPosition(work.referee, work.player, {-0.75f, 0, -0.1f}, 47331);
    ArmwPosition(work.opponent, ARMW_BASE, {0.2f, 0, 0.76f}, 0);
    EVT_AT_SetMot(work.player, 9160, 70, 1, 0x40000000U, 0);
    OpponentLoseMotion(work, 1);
    EVT_AT_SetMot(work.referee, ARMW_REFEREE_MOTIONS[7][armw.refereeMotion], 70, 1, 0x40000000U, 0);
    QueueCling(work.referee, 21, {0.075f, 0, 0.032f}, {19114, 34133, 41870}, 70);
    QueueCling(work.player, 14, {0.086f, -0.002f, -0.011f}, {49152, 0, 49152}, 120);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), AREA_EVENT_TASK);
    RegisterEventTask(task);
    EventCling &detach = *static_cast<EventCling *>(HLib::GetTaskParameter(task));
    detach.cleanup = 1;
    detach.target = ARMW_MONEY;
    detach.character = shen_name{};
    detach.mode = 1;
    detach.delay = 220;
    ArmwVoice(1, "005", 30);
    ArmwVoice(1, "006", 100);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(work.referee), Smile, 0, 1);
    EVT_AT_SetFaceLooks(work.referee, Normal, 8, 1, 220);
    ArmwCamera(shenGetRandomInt32(3) + 3103);
    PATI_StartMoneyCount(armw.bet, 90);
    Phase(work, 19);
    return 220;
}
std::int32_t EndLadder(ArmwWork &work) {
    switch (work.step) {
    case 0:
        ArmwPosition(work.player, ARMW_BASE, {-0.02f, 0, -0.76f}, 32768);
        ArmwPosition(work.referee, ARMW_BASE, {0.5f, 0, 0.4f}, 4096);
        EVT_AT_SetMot(work.player, 61443, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(work.referee, ARMW_REFEREE_MOTIONS[0][armw.refereeMotion], 0, 1, 0x40000000U, 0);
        ArmwAppendVoice(1, "007");
        ArmwCamera(3150);
        ++work.step;
        break;
    case 1:
        ArmwAppendVoice(0, "024");
        ArmwCamera(3151);
        ++work.step;
        break;
    case 2:
        ArmwPosition(work.opponent, ARMW_BASE, {-0.2f, 0, 1.1f}, 61440);
        OpponentLoseMotion(work, 0.6f);
        ArmwAppendVoice(1, "008");
        ArmwAppendVoice(1, "022");
        ArmwCamera(3152);
        EV_DisableCharacterDisplay(ARMW_MONEY);
        ++work.step;
        break;
    case 3:
        ArmwAppendVoice(0, "025");
        ArmwCamera(3151);
        Phase(work, 20);
        break;
    }
    ArmwWaitVoices(work.self);
    return 0;
}
std::int32_t Lose(ArmwWork &work) {
    switch (work.step) {
    case 0:
        ArmwVoice(3, shenGetRandomInt32(2) ? "024" : "025");
        Loop3DSoundEffect(345);
        EVT_AT_SetSound(349, 20);
        EVT_AT_SetSound(3135, 40);
        work.authorFrames = static_cast<float>(ArmwOpponentCollects(work) + 30);
        work.step = 10;
        break;
    case 1: {
        ArmwRefereeCollects(work);
        EVT_AT_SetSound(346, 5);
        ArmwWaitVoices(work.self);
        if (work.falseStarts == 3)
            AddThisToCashOrSegaCoins(-armw.bet, ECashOrCoins::Cash2);
        EventCling detach{.cleanup = 0, .target = ARMW_MONEY, .character = {}, .bone = 0, .position = {}, .rotation = {}, .mode = 1, .delay = 0};
        EVT_AT_Cling(&detach);
        EV_DisableCharacterDisplay(ARMW_MONEY);
        work.authorFrames = 0;
        work.step = 11;
        break;
    }
    case 2:
        ArmwCamera(3391);
        ArmwVoice(3, "026");
        Phase(work, 20);
        return 100;
    case 10:
        if (HasPressedCancel()) {
            EV_CleanupMapControlTask();
            work.authorFrames = 0;
        }
        work.authorFrames -= Tick();
        if (work.authorFrames <= 0) {
            AddThisToCashOrSegaCoins(-armw.bet, ECashOrCoins::Cash2);
            work.step = 1;
        }
        break;
    case 11:
        if (HasPressedCancel())
            work.authorFrames = 0;
        work.authorFrames -= Tick();
        if (work.authorFrames <= 0) {
            EV_CleanupMapControlTask();
            work.step = 2;
        }
        break;
    }
    return 0;
}
std::int32_t StoryWin(ArmwWork &work) {
    switch (work.step) {
    case 0:
        ArmwPosition(work.opponent, ARMW_BASE, {0.2f, 0, 0.76f}, 0);
        EVT_AT_SetMot(work.opponent, 9299, 0, 1, 0x40000000U, 0);
        ArmwVoice(3, "020");
        ArmwCamera(6000);
        EV_DisableCharacterDisplay(work.referee);
        QueueFade(30, true, 60);
        ++work.step;
        return 90;
    case 1:
        EV_LoadAudio("0008A000", 0);
        EV_NPC_NewLoadSw(0);
        EV_ResetFreeTalkFlags();
        ResetAuthFlags();
        FreeRefereePackages();
        FreeOpponentPackages();
        LoadPackage("ARMWAUTH");
        EV_NPC_NewLoadSw(1);
        EV_DisableCharacterDisplay(work.opponent);
        EV_EnableCharacterDisplay(work.referee);
        QueueSkNpcWait(work.self);
        ++work.step;
        break;
    case 2:
        SetPlayerGameFlags(PlayerGameFlagMode::Bit5, reinterpret_cast<void *>(1));
        EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
        QueueSkAuthorStart(work.self, SHEN_NAME(WON_), nullptr, nullptr, 0);
        PATI_StartMoneyCount(armw.bet, 30);
        ++work.step;
        break;
    default:
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        EVT_AT_ObjectDisp_1(work.opponent, 90, 0);
        work.opponentIndex = 1;
        Phase(work, 20);
        break;
    }
    return 0;
}
// @verified - p
bool LoadNextOpponent(ArmwWork &work) {
    EV_CheckWarmStartNpcRoute(work.opponent);
    if (!EV_ForceLoadNpc(work.opponent))
        return false;
    EV_SetCharacterAlphaEnabled(work.opponent, false);
    EV_SetCharaControllerFlags(work.opponent, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(work.opponent, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(work.opponent, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetRefBoardParamsFor(work.opponent, 1);
    AT_SetLocalBaseChara(work.opponent, EVENT_CHARACTER_INVALID);
    mtwkSetRequestedRootMode(scenFindMotionwork(work.opponent), 3);
    mtwkSetFrameTime(scenFindMotionwork(work.opponent), 7, MotionRegion::LowerBody);
    EV_EnableCharacterDisplay(work.opponent);
    GetCharacterMotionCompatibilityResult(ctrlGetCharaByName(work.opponent));
    SetFaceFlagsForChara_0(ctrlGetCharaByName(work.opponent), FacialNodeReplacement);
    PrepareHands(work.opponent);
    charaSetDisplayEnabledFlag(ctrlGetCharaByName(work.opponent), 0);
    return true;
}
std::int32_t NextOpponent(ArmwWork &work) {
    if (!work.step) {
        ++work.opponentIndex;
        if (DoesScenarioCharacterExist(work.opponent) && (work.flags & 0x10000000U)) {
            HasCharacterMotionWork(ctrlGetCharaByName(work.opponent));
            hndlSetFlags(ctrlGetCharaByName(work.opponent));
            EV_npcFree(work.opponent);
            EV_npcLockUnlock(work.opponent);
        }
        ++work.step;
        work.falseStarts = 0;
        if (ARMW_OPPONENT_ORDER[Ladder(work)][work.opponentIndex] == -1) {
            FinishLadderFlags();
            Phase(work, 15);
        } else {
            ResetAuthFlags();
            SelectOpponent(work);
            if (work.flags & 0x10000000U) {
                EV_NPC_NewLoadSw(0);
                EV_ResetFreeTalkFlags();
                LoadOpponentPackage();
                EV_NPC_NewLoadSw(1);
            }
        }
    } else if (work.step == 1 && LoadNextOpponent(work)) {
        if (!EV_GetScreenFilmIsNotShowing())
            Fade(30, false);
        Phase(work, 8);
    }
    return 0;
}
// @verified - p
void EndCallback(void *data) {
    ArmwWork &work = *static_cast<ArmwWork *>(data);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, ARMW_FLAGS[armw.location], work.opponentIndex);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1293, 0);
    charaSetDisplayEnabledFlag(ctrlGetCharaByName(ARMW_RYO), 1);
    if (DoesScenarioCharacterExist(ARMW_ROCKO))
        charaSetDisplayEnabledFlag(ctrlGetCharaByName(ARMW_ROCKO), 1);
    SetHighModelXYZ(ctrlGetCharaByName(ARMW_RYO), 0, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(ARMW_RYO), 0, 1, 0);
    hndlSetFlags(ctrlGetCharaByName(work.opponent));
    hndlSetFlags(ctrlGetCharaByName(work.referee));
    SetCharaKill(ARMW_BASE, 1);
    ET_AT_KillWork(work.characters);
    qteBattleUtil(BattleQteMode::RetireAndFree);
    ReleaseHandMotion();
    SetPlayerGameFlags(PlayerGameFlagMode::Bit5, reinterpret_cast<void *>(1));
    if (DoesScenarioCharacterExist(work.opponent)) {
        EV_npcFree(work.opponent);
        EV_npcLockUnlock(work.opponent);
    }
    EV_FreeAuthorPak(0, g_currentMap == AREA_QTB4 ? "0368" : "0008");
    for (shen_name name : armw.tutorialCharacters) {
        if (name.name == EVENT_CHARACTER_INVALID.name)
            break;
        if (DoesScenarioCharacterExist(name)) {
            EV_npcLockUnlock(name);
            EV_npcFree(name);
        }
    }
    EV_EventSetUpHelpIcon_0(0, 0);
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    FreePackage("ARMW");
    FreePackage("ARMWCOMN");
    FreePackage("ARMWAUTH");
    FreeRefereePackages();
    FreeOpponentPackages();
    if (g_currentMap.name == SHEN_NAME(AR02))
        EV_LoadNPCRoute("NPC.BIN");
    EV_stepSetFlags(work.flags & 1 ? 0 : work.flags & 0x20 ? 2 : 1, 4 - ((work.flags & 8) != 0));
    EV_NPC_NewLoadSw(1);
    if (!work.nextScene.name) {
        sbtLoadProgram();
        EV_spmBeginSampleExecute();
        EVT_AT_SetMot(ARMW_RYO, 61442, 0, 1, 0x40000000U, 0);
        // The exit facing goes through EVT_AT_SetAngY, which keeps Ryo's current X and Z rotation.
        const auto place = [](const Vector3 &position, float degrees) {
            EventSetPosition pos{.cleanup = 0, .character = ARMW_RYO, .parent = EVENT_CHARACTER_INVALID, .position = position, .delay = 0};
            EVT_AT_SetPosRot(&pos);
            EventSetAngle angle{.cleanup = 0, .character = ARMW_RYO, .parent = EVENT_CHARACTER_INVALID, .degrees = degrees, .delay = 0};
            EVT_AT_SetAngY_Callback(&angle);
        };
        switch (armw.argument) {
        case 0:
            place({207.9f, 100.22f, 323}, 240);
            break;
        case 1:
            place({227.8f, 100.22f, 351.5f}, 90);
            break;
        case 2:
            place({252, 100.22f, 354.5f}, 0);
            break;
        }
        Fade(30, false);
    } else {
        EV_SetScenarioTaskFlags(6);
        EV_scenQueueLoad(g_actNumber, work.nextScene, work.nextEntry);
    }
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.self);
    scenFreeEvent("ARMW.EV3");
}
// @verified - p
void MainCallback(void *data) {
    ArmwWork &work = *static_cast<ArmwWork *>(data);
    if (DoesScenarioCharacterExist(ARMW_BASE))
        PinBase();
    std::int32_t delay = 0;
    switch (work.phase) {
    case 0:
        InitializeResources(work);
        armw.active = 0;
        return;
    case 1:
        delay = Ready(work);
        break;
    case 2:
        delay = Tutorial(work);
        break;
    case 3:
        delay = AskAfterTutorial(work);
        break;
    case 4:
        delay = PrepareMatch(work);
        break;
    case 5:
        delay = Explain(work);
        break;
    case 7:
        delay = Refuse(work);
        break;
    case 8:
        delay = Rematch(work);
        break;
    case 9:
        delay = FalseStart(work);
        armw.active = 1;
        if (delay > 0)
            HLib::SetupTaskDelay(delay);
        return;
    case 10:
        delay = ArmwPlayMatch(work);
        armw.active = 1;
        if (delay > 0)
            HLib::SetupTaskDelay(delay);
        return;
    case 11:
        delay = ArmwDirectionQte(work);
        armw.active = 1;
        if (delay > 0)
            HLib::SetupTaskDelay(delay);
        return;
    case 12:
        delay = ArmwFinishQte(work);
        armw.active = 1;
        if (delay > 0)
            HLib::SetupTaskDelay(delay);
        return;
    case 13:
        delay = ArmwWinAnimation(work);
        break;
    case 14:
        delay = Win(work);
        break;
    case 15:
        delay = EndLadder(work);
        break;
    case 16:
        delay = ArmwLoseAnimation(work);
        break;
    case 17:
        delay = Lose(work);
        break;
    case 18:
        delay = StoryWin(work);
        break;
    case 19:
        delay = NextOpponent(work);
        break;
    case 20:
        if (EV_GetScreenFilmIsNotShowing()) {
            delay = 90;
            Fade(90, true);
        }
        spmFadeOut();
        HLib::SetCurrentTaskTo(EndCallback);
        break;
    default:
        return;
    }
    armw.active = 0;
    if (delay > 0)
        HLib::SetupTaskDelay(delay);
}
// @verified - p
void QueueArmwMain(void *) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(MainCallback, 4, 0x0b, sizeof(ArmwWork), ARMW_TASK);
    ArmwWork &work = *static_cast<ArmwWork *>(HLib::GetTaskParameter(task));
    std::memset(&work, 0, sizeof(work));
    work.self = task;
    armw.location = g_currentMap.name == ARMW_AREA.name ? static_cast<std::int16_t>((g_entryPoint < 90 ? g_entryPoint : g_entryPoint - 90) % 12) :
                                                          static_cast<std::int16_t>(armw.argument);
    work.step = 0;
    SelectCharacters(work);
    SetWeatherUpdateEnabledTo(true);
    RefreshWeatherForCurrentTime(0);
    EV_UpdateWeather();
    work.characters = Queue_EVT_AT_ReadChr(task, armw.characters);
    ArmwNullsub();
    EV_EventSetUpHelpIcon_0(65, 0);
}
} // namespace
// @verified - p
void InitializeArmwEvent() {
    armw.characters[0] = ARMW_RYO;
    armw.handMotion = nullptr;
    armw.argument = 0;
    armw.qtePenalty = 0;
    armw.location = 0;
    armw.started = 0;
    armw.startFrames = 0;
    armw.introductionVariant = 0;
    armw.qteButton = 0;
    armw.qteSpeaker = 0;
    armw.refereeMotion = 0;
    armw.opponentMotion = 0;
    armw.bet = 0;
    armw.voiceCount = 0;
    std::memset(armw.voicePrefixes, 0, sizeof(armw.voicePrefixes));
    armw.characters[1] = EVENT_CHARACTER_INVALID;
    armw.tutorialCharacters[0] = ARMW_SHIYO;
    armw.tutorialCharacters[1] = EVENT_CHARACTER_INVALID;
    std::memset(armw.voices, 0, sizeof(armw.voices));
    armw.voiceSkipped = 0;
}
// @verified - p
void RunArmwEvent(std::uintptr_t argument) {
    Fade(1, true);
    SetPlayerControlEnabled(0);
    ArmwNullsub();
    armw.argument = static_cast<std::int32_t>(argument);
    SetWeatherUpdateEnabledTo(true);
    RefreshWeatherForCurrentTime(0);
    EV_UpdateWeather();
    SetPlayerControlEnabled(0);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_NPCClear_Callback, 6, 0x0b, sizeof(StepNpcClearWork), AREA_EVENT_TASK);
    RegisterEventTask(task);
    StepNpcClearWork &work = *static_cast<StepNpcClearWork *>(HLib::GetTaskParameter(task));
    work.callback = QueueArmwMain;
    work.callbackData = nullptr;
    work.reserved10 = 0;
    work.state = 0;
    work.delay = 3;
}
const ScriptAreaDefinition armwScriptDefinition{.areaId = ARMW_AREA, .field04 = 0, .init = InitializeArmwEvent, .main = RunArmwEvent};
} // namespace shenmue::s2::areas
#endif
