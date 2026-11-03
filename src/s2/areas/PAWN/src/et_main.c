#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_pawn.hpp"

void PawnMain(shen_name shop)
{
    HLibTask *task;
    PawnWork *work;
    int fail, i;
    char *package;
    while (1) {
        fail = 1;
        task = 0;
        if (!FindPawnOwner(shop)) {
            FTBL.DebugLogToFileRemoved("illegal id given (in PAWN.EV3)\n\n\n\n\n");
            FTBL.EventAssertNullsub(__FILE__, 298);
            break;
        }
        task = FTBL.HLib_EnqueueTaskWithParameter(PawnInitialize, 4, 11, sizeof(PawnWork), TASK_EVNT);
        if (!task)
            break;
        work = (PawnWork *)FTBL.HLib_GetTaskParameter(task);
        if (!work)
            break;
        package = (char *)work;
        for (i = 0; i < (int)sizeof(PawnWork); i++)
            package[i] = 0;
        work->shop = shop;
        package = work->package;
        *package++ = 'P';
        *package++ = 'A';
        *package++ = 'W';
        *package++ = 'N';
        *package++ = ((char *)&shop)[0];
        *package++ = ((char *)&shop)[1];
        *package++ = ((char *)&shop)[2];
        *package++ = ((char *)&shop)[3];
        *package = 0;
        work->task = task;
        work->parent = FTBL.HLib_GetCurrentTask();
        FTBL.EV_setHoldStatesFlags(0);
        FTBL.srchReset();
        FTBL.camCtrlSetState(0);
        FTBL.HLib_SetupTimedTask(work->parent, -1);
        FTBL.SetPlayerControlEnabled(0);
        FTBL.SetScreenFilmFade(8, 0, 0, 0, 0, 255, 0, 0, 0);
        fail = 0;
        break;
    }
    if (fail && task)
        FTBL.HLib_DestroyTask(task);
}

#define SHENMUE_AREA_LOOP_MOTION_FRAMES
#define SHENMUE_AREA_LOOP_VOICE_REVISION 1
#include "s2/areas/area_loop_dc.c"

static char pawnConversation[] = "0121";
static char pawnCutscene[] = "01210000";
static PawnShopItem pawnShopItems[] = {
    {PAWN_ARP0, 9000}, {PAWN_WSP0, 9001}, {PAWN_WKP0, 9002}, {PAWN_WKP1, 9003}, {PAWN_WTP0, 9004}, {PAWN_WTP1, 9005},
    {PAWN_WTP2, 9006}, {PAWN_WTP3, 9007}, {PAWN_WTP4, 9008}, {PAWN_WRP0, 9009}, {PAWN_QBL0, 9010}, {PAWN_QBR0, 9011},
    {0, 0},
};
static PawnShopScene pawnShopScenes[] = {
    {PAWN_ARP0, SHEN_NAME4('T', 'J', 'N', '_'), SHEN_NAME4('p', 'A', 'P', '0'), 2227, 296.697f, 496.878f, 16384, -0.04f, -1.4f,
     296.697f, 496.878f, 16384, -0.04f, -1.4f, 457},
    {PAWN_WSP0, SHEN_NAME4('Y', 'R', 'Y', '_'), SHEN_NAME4('p', 'S', 'P', '0'), 1260, 49.618f, 120.72f, 49152, -0.025f, -1.5f,
     49.618f, 120.72f, 49152, -0.025f, -1.5f, 458},
    {PAWN_WKP0, SHEN_NAME4('H', 'A', 'C', '_'), SHEN_NAME4('p', 'K', 'P', '0'), 0, 57.49f, 270.507f, 49152, -0.091f, -1.706f,
     57.49f, 270.507f, 49152, -0.091f, -1.706f, 459},
    {PAWN_WKP1, SHEN_NAME4('E', 'E', 'I', '_'), SHEN_NAME4('p', 'K', 'P', '1'), 0, 98.8f, 316.087f, 49152, 0.046f, -1.596f,
     98.8f, 316.087f, 49152, 0.046f, -1.596f, 460},
    {PAWN_WTP0, SHEN_NAME4('M', 'L', 'I', '_'), SHEN_NAME4('p', 'T', 'P', '0'), 0, 237.7f, 256.305f, 49152, 0.111f, -1.871f,
     237.7f, 256.305f, 49152, 0.111f, -1.871f, 461},
    {PAWN_WTP1, SHEN_NAME4('N', 'O', 'L', '_'), SHEN_NAME4('p', 'T', 'P', '1'), 0, 237.9f, 265.8f, 49152, -0.052f, -1.483f,
     237.9f, 265.8f, 49152, -0.052f, -1.483f, 462},
    {PAWN_WTP2, SHEN_NAME4('J', 'N', 'T', '_'), SHEN_NAME4('p', 'T', 'P', '2'), 0, 261.079f, 261.82f, 16384, -0.052f, -1.683f,
     261.079f, 261.82f, 16384, -0.052f, -1.683f, 463},
    {PAWN_WTP3, SHEN_NAME4('X', 'N', 'O', '_'), SHEN_NAME4('p', 'T', 'P', '3'), 0, 237.937f, 244.149f, 16384, -0.027f, -1.555f,
     237.937f, 244.149f, 16384, -0.027f, -1.555f, 464},
    {PAWN_WTP4, SHEN_NAME4('C', 'K', 'N', '_'), SHEN_NAME4('p', 'T', 'P', '4'), 0, 246.6f, 246.3f, 49152, -0.006f, -1.676f,
     246.6f, 246.3f, 49152, -0.006f, -1.676f, 465},
    {PAWN_WRP0, SHEN_NAME4('E', 'M', 'N', '_'), SHEN_NAME4('p', 'R', 'P', '0'), 0, 160.202f, 144.234f, 16384, -0.101f, -1.65f,
     160.202f, 144.234f, 16384, -0.101f, -1.65f, 466},
    {PAWN_QBL0, SHEN_NAME4('M', 'M', 'H', '_'), SHEN_NAME4('p', 'B', 'L', '0'), 0, 207.018f, 190.048f, 23665, -0.101f, -1.555f,
     206.78f, 189.75f, 23665, -0.101f, -1.555f, 920},
    {PAWN_QBR0, SHEN_NAME4('I', 'L', 'N', '_'), SHEN_NAME4('p', 'B', 'R', '0'), 0, 209.749f, 193.561f, 23665, -0.2f, -1.65f,
     209.749f, 193.561f, 23665, -0.2f, -1.65f, 921},
    {0, 0, 0, 0, 0.0f, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0.0f, 0},
};

void PawnInitialize(void *data)
{
    PawnWork *work = (PawnWork *)data;
    char *package;
    int fail = 1;
    while (1) {
        if (FTBL.filmCheckFadeTrackColour()) {
            fail = 0;
            break;
        }
        FTBL.HLib_SetCurrentTaskTo(PawnWaitPackage);
        FTBL.HLib_SetupTaskDelay(-1);
        fail = 0;
        FTBL.EV_ResetFreeTalkFlags();
        FTBL.EV_NPC_NewLoadSw_0(0);
        package = work->package;
        if (package && package[0])
            QueueAreaLoopPackage(FTBL.Filepath_Gen(0), package, ResolveAreaLoopParent);
        break;
    }
    if (fail)
        FTBL.HLib_SetCurrentTaskTo(PawnDone);
}

void PawnWaitPackage(void *data)
{
    PawnWork *work = (PawnWork *)data;
    shen_name names[2];
    if (work) {
        FTBL.EV_ControlPackageResources(2, 4, work->package, 0, 0);
        FTBL.EV_ControlPackageResources(2, 2, work->package, 0, 0);
        FTBL.EV_LoadCutsceneFromCurrentScene(pawnCutscene);
        names[0] = FindPawnOwner(work->shop);
        names[1] = 0;
        FTBL.HLib_SetCurrentTaskTo(PawnWaitOwner);
        FTBL.HLib_SetupTaskDelay(-1);
        QueueAreaLoopNpcs(names, ResolveAreaLoopParent);
    } else
        FTBL.HLib_SetCurrentTaskTo(PawnDone);
}

void PawnWaitOwner(void *data)
{
    PawnWork *work = (PawnWork *)data;
    shen_name owner, flyer;
    if (work) {
        owner = FindPawnOwner(work->shop);
        FTBL.GetCharaPosition(owner, -1, &work->position, 0x40000000);
        FTBL.GetScenarioCharacterEulerAngles(owner, &work->rotation, 0x40000000);
        SetAreaLoopCharacterControl(PAWN_RYO, 1);
        flyer = FindPawnFlyer(work->shop);
        if (flyer)
            SetAreaLoopCharacterControl(flyer, 1);
        FTBL.EV_NPC_NewLoadSw_0(1);
        FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.HLib_SetCurrentTaskTo(PawnBegin);
    } else {
        FTBL.EV_NPC_NewLoadSw_0(1);
        FTBL.HLib_SetCurrentTaskTo(PawnDone);
    }
}

void PawnDone(void *data)
{
    PawnWork *work = (PawnWork *)data;
    shen_name owner, flyer;
    shen_name names[2];
    while (!FTBL.filmCheckFadeTrackColour() && work && !FTBL.IsScenarioBusy()) {
        if (work && work->awardTask)
            FTBL.HLib_DestroyTask(work->awardTask);
        SetAreaLoopCharacterControl(PAWN_RYO, 0);
        flyer = FindPawnFlyer(work->shop);
        if (flyer)
            SetAreaLoopCharacterControl(flyer, 0);
        owner = FindPawnOwner(work->shop);
        if (!owner)
            break;
        if (work && work->package[0])
            FTBL.EV_ControlPackageResources(0, 6, work->package, 0, 0);
        names[0] = owner;
        names[1] = 0;
        RestoreAreaLoopCharacters(names, 0);
        FTBL.camCtrlSetState(27);
        FTBL.SetScenarioFollowCharacter(0);
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlUtil(15, 0);
        FTBL.camCtrlSetState(0);
        FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(PAWN_RYO), 0xf003, 0);
        FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(PAWN_RYO), 0);
        FTBL.HLib_ResolveDeferredCallback(work->parent);
        FTBL.SetPlayerControlEnabled(1);
        FTBL.EV_setHoldStatesFlags(1);
        FTBL.HLib_MarkCurrentTaskForCleanup();
        break;
    }
}

void PawnBegin(void *data)
{
    PawnWork *work = (PawnWork *)data;
    Vector3 playerPosition;
    Vector3i playerRotation;
    Vector3 ownerPosition;
    Vector3i ownerRotation;
    int flags[9];
    shen_name shop, owner;
    int fail, flyer, i;
    flags[0] = 1222;
    flags[1] = 1223;
    flags[2] = 1224;
    flags[3] = 1225;
    flags[4] = 1226;
    flags[5] = 1227;
    flags[6] = 1228;
    flags[7] = 1229;
    flags[8] = -1;
    fail = 1;
    while (work) {
        shop = work->shop;
        flyer = PawnFlyerRequested(shop);
        if (!GetPawnStandData(shop, &ownerPosition, &ownerRotation.y, &playerPosition, &playerRotation.y, flyer)) {
            FTBL.DebugLogToFileRemoved("can't get stand data ( in PAWN.EV3 )\n");
            FTBL.EventAssertNullsub(__FILE__, 597);
            break;
        }
        playerRotation.x = 0;
        playerRotation.z = 0;
        ownerRotation.x = 0;
        ownerRotation.z = 0;
        owner = FindPawnOwner(shop);
        if (!owner) {
            FTBL.DebugLogToFileRemoved("illegal data on code ( in PAWN.EV3 )\n");
            FTBL.EventAssertNullsub(__FILE__, 611);
            break;
        }
        QueueAreaLoopPosition(PAWN_RYO, -1, 0, &playerPosition, &playerRotation, 0);
        QueueAreaLoopPosition(owner, -1, 0, &ownerPosition, &ownerRotation, 0);
        QueueAreaLoopPosition(PAWN_BASE, -1, 0, &ownerPosition, &ownerRotation, 0);
        for (i = 0; flags[i] != -1; i++)
            FTBL.EV_SetPlayerFlag(12, flags[i], 0);
        FTBL.HLib_SetCurrentTaskTo(flyer ? PawnBeginFlyer : PawnTalkOpening);
        fail = 0;
        break;
    }
    if (fail)
        FTBL.HLib_SetCurrentTaskTo(PawnDone);
}

void PawnBeginFlyer(void *data)
{
    PawnWork *work = (PawnWork *)data;
    Vector3i rotation;
    Vector3 playerPosition, ownerPosition;
    Vector3i playerRotation, ownerRotation;
    float reflector[4];
    shen_name owner, flyer;
    int award;
    HLibTask *task;
    PawnFlyerAwardWork *awardWork;
    do {
        if (!work || (owner = FindPawnOwner(work->shop)) == 0 || (flyer = FindPawnFlyer(work->shop)) == 0) {
            FTBL.HLib_SetCurrentTaskTo(PawnFlyerDone);
            break;
        }
        FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(PAWN_RYO), 17, 0);
        FTBL.GetScenarioCharacterEulerAngles(PAWN_RYO, &rotation, 0x40000000);
        rotation.y += 0x8000;
        FTBL.EV_AT_SetAngY_0(PAWN_RYO, 0x78000000, &rotation, 0);
        FTBL.EV_FreeAuthorPak(1, pawnConversation);
        FTBL.GetCharacterFaceCacheIndex(FTBL.ctrlGetCharaByName(PAWN_RYO));
        FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(PAWN_RYO));
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(PAWN_RYO), 1, 0, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(PAWN_RYO), 1, 1, 0);
        FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(PAWN_RYO), 1, (void *)areaLoopHandPoses[11], 1);
        FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(PAWN_RYO), 0, (void *)areaLoopHandPoses[43], 1);
        FTBL.GetCharacterFaceCacheIndex(FTBL.ctrlGetCharaByName(owner));
        FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(owner));
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(owner), 1, 0, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(owner), 1, 1, 0);
        FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(owner), 1, (void *)areaLoopHandPoses[11], 1);
        FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(owner), 0, (void *)areaLoopHandPoses[43], 1);
        FTBL.HLib_SetCurrentTaskTo(PawnFlyerDone);
        FTBL.HLib_SetupTaskDelay(-1);
        reflector[0] = 1.0f;
        reflector[1] = 1.0f;
        reflector[2] = 1.0f;
        reflector[3] = 1.0f;
        FTBL.SetRefBoardParamsForCharaById(flyer, 1);
        FTBL.SetScenarioReflectorBoardValues(flyer, reflector);
        award = 300;
        switch (work->shop) {
        case PAWN_ARP0:
            QueueAreaLoopMotion(PAWN_RYO, 7303, 0, 570, 80.0f, -1.0f, 1.0f, 0.0f, ResolveAreaLoopParent, 6);
            QueueAreaLoopMotion(owner, 7315, 0, 570, 80.0f, -1.0f, 1.0f, 0.0f, 0, 6);
            playerPosition.x = 0.243f;
            playerPosition.y = 0.049f;
            playerPosition.z = -0.084f;
            playerRotation.x = 60438;
            playerRotation.y = 48969;
            playerRotation.z = 55705;
            ownerPosition.x = 0.193f;
            ownerPosition.y = 0.155f;
            ownerPosition.z = -0.063f;
            ownerRotation.x = 34388;
            ownerRotation.y = 17694;
            ownerRotation.z = 57107;
            QueueAreaLoopAttachment(owner, 14, flyer, 180, 59, &ownerPosition, &ownerRotation, 0);
            QueueAreaLoopAttachment(PAWN_RYO, 14, flyer, 240, 330, &playerPosition, &playerRotation, 0);
            playerPosition.x = 1000.0f;
            playerPosition.y = 1000.0f;
            playerPosition.z = 1000.0f;
            QueueAreaLoopAttachment(PAWN_RYO, 14, flyer, 571, 1, &playerPosition, &playerRotation, 0);
            playerRotation.x = 0;
            playerRotation.y = 0;
            playerRotation.z = 0;
            ownerRotation.x = 3094;
            ownerRotation.y = 0;
            ownerRotation.z = 3640;
            FTBL.imgmSetHandPos(FTBL.ctrlGetCharaByName(PAWN_RYO), 0, 21, &playerRotation);
            FTBL.imgmSetHandPos(FTBL.ctrlGetCharaByName(owner), 0, 21, &ownerRotation);
            award = 500;
            QueueAreaLoopCamera(work->package, 1600, PAWN_BASE, 0, 0);
            QueueAreaLoopCamera(work->package, 1601, PAWN_BASE, 100, 0);
            QueueAreaLoopCamera(work->package, 1602, PAWN_BASE, 180, 0);
            QueueAreaLoopCamera(work->package, 1604, PAWN_BASE, 240, 0);
            QueueAreaLoopCamera(work->package, 1603, PAWN_BASE, 340, 0);
            QueueAreaLoopVoice("0121B014", 1, 33, -1, 0);
            QueueAreaLoopVoice("0121A005", 1, 127, -1, 0);
            QueueAreaLoopVoice("0121B015", 1, 165, -1, 0);
            QueueAreaLoopVoice("0121B016", 1, 270, -1, 0);
            QueueAreaLoopVoice("0121B017", 1, 470, -1, 0);
            break;
        case PAWN_WSP0:
            QueueAreaLoopMotion(PAWN_RYO, 7310, 0, 800, 540.0f, -1.0f, 1.0f, 0.0f, ResolveAreaLoopParent, 6);
            QueueAreaLoopMotion(owner, 7301, 0, 800, 540.0f, -1.0f, 1.0f, 0.0f, 0, 6);
            ownerPosition.x = 0.212f;
            ownerPosition.y = 0.104f;
            ownerPosition.z = -0.069f;
            ownerRotation.x = 58285;
            ownerRotation.y = 20201;
            ownerRotation.z = 15848;
            playerPosition.x = 0.256f;
            playerPosition.y = 0.057f;
            playerPosition.z = 0.024f;
            playerRotation.x = 34770;
            playerRotation.y = 53703;
            playerRotation.z = 14745;
            QueueAreaLoopAttachment(owner, 14, flyer, 323, 85, &ownerPosition, &ownerRotation, 0);
            QueueAreaLoopAttachment(PAWN_RYO, 14, flyer, 409, 170, &playerPosition, &playerRotation, 0);
            playerPosition.x = 1000.0f;
            playerPosition.y = 1000.0f;
            playerPosition.z = 1000.0f;
            QueueAreaLoopAttachment(PAWN_RYO, 14, flyer, 580, 1, &playerPosition, &playerRotation, 0);
            award = 500;
            QueueAreaLoopCamera(work->package, 1610, owner, 0, 0);
            QueueAreaLoopCamera(work->package, 1611, owner, 260, 0);
            QueueAreaLoopCamera(work->package, 1612, owner, 330, 0);
            QueueAreaLoopCamera(work->package, 1613, owner, 409, 0);
            QueueAreaLoopCamera(work->package, 1614, owner, 649, 0);
            QueueAreaLoopVoice("0121G002", 1, 35, -1, 0);
            QueueAreaLoopVoice("0121A005", 1, 175, -1, 0);
            QueueAreaLoopVoice("0121G003", 1, 276, -1, 0);
            QueueAreaLoopVoice("0121G004", 1, 433, -1, 0);
            QueueAreaLoopVoice("0121G005", 1, 640, -1, 0);
            break;
        case PAWN_WKP0:
            QueueAreaLoopMotion(PAWN_RYO, 7306, 0, 600, 290.0f, -1.0f, 1.0f, 0.0f, ResolveAreaLoopParent, 6);
            QueueAreaLoopMotion(owner, 7298, 0, 600, 290.0f, -1.0f, 1.0f, 0.0f, 0, 6);
            ownerPosition.x = 0.212f;
            ownerPosition.y = 0.137f;
            ownerPosition.z = -0.08f;
            ownerRotation.x = 62077;
            ownerRotation.y = 17093;
            ownerRotation.z = 20024;
            playerPosition.x = 0.216f;
            playerPosition.y = 0.061f;
            playerPosition.z = -0.119f;
            playerRotation.x = 48787;
            playerRotation.y = 50972;
            playerRotation.z = 3640;
            QueueAreaLoopAttachment(owner, 14, flyer, 230, 94, &ownerPosition, &ownerRotation, 0);
            QueueAreaLoopAttachment(PAWN_RYO, 14, flyer, 329, 100, &playerPosition, &playerRotation, 0);
            playerPosition.x = 1000.0f;
            playerPosition.y = 1000.0f;
            playerPosition.z = 1000.0f;
            QueueAreaLoopAttachment(PAWN_RYO, 14, flyer, 430, 1, &playerPosition, &playerRotation, 0);
            award = 340;
            QueueAreaLoopCamera(work->package, 1620, PAWN_BASE, 0, 0);
            QueueAreaLoopCamera(work->package, 1621, owner, 100, 0);
            QueueAreaLoopCamera(work->package, 1622, owner, 329, 0);
            QueueAreaLoopCamera(work->package, 1623, owner, 451, 0);
            QueueAreaLoopVoice("0121E002", 1, 10, -1, 0);
            QueueAreaLoopVoice("0121A005", 1, 120, -1, 0);
            QueueAreaLoopVoice("0121E003", 1, 170, -1, 0);
            QueueAreaLoopVoice("0121E004", 1, 380, -1, 0);
            QueueAreaLoopVoice("0121E005", 1, 510, -1, 0);
            break;
        case PAWN_WKP1:
            QueueAreaLoopMotion(PAWN_RYO, 7303, 0, 740, 100.0f, -1.0f, 1.0f, 0.0f, ResolveAreaLoopParent, 6);
            QueueAreaLoopMotion(owner, 7320, 0, 740, 100.0f, -1.0f, 1.0f, 0.0f, 0, 6);
            ownerPosition.x = 0.243f;
            ownerPosition.y = 0.046f;
            ownerPosition.z = -0.066f;
            ownerRotation.x = 182;
            ownerRotation.y = 21663;
            ownerRotation.z = 19660;
            playerPosition.x = 0.242f;
            playerPosition.y = 0.075f;
            playerPosition.z = -0.066f;
            playerRotation.x = 61713;
            playerRotation.y = 48241;
            playerRotation.z = 56433;
            QueueAreaLoopAttachment(owner, 14, flyer, 154, 75, &ownerPosition, &ownerRotation, 0);
            QueueAreaLoopAttachment(PAWN_RYO, 14, flyer, 230, 335, &playerPosition, &playerRotation, 0);
            playerPosition.x = 1000.0f;
            playerPosition.y = 1000.0f;
            playerPosition.z = 1000.0f;
            QueueAreaLoopAttachment(PAWN_RYO, 14, flyer, 566, 1, &playerPosition, &playerRotation, 0);
            award = 240;
            QueueAreaLoopCamera(work->package, 1630, PAWN_BASE, 0, 0);
            QueueAreaLoopCamera(work->package, 1631, PAWN_BASE, 154, 0);
            QueueAreaLoopCamera(work->package, 1635, PAWN_BASE, 220, 0);
            QueueAreaLoopCamera(work->package, 1634, PAWN_BASE, 566, 0);
            QueueAreaLoopVoice("0121H002", 1, 10, -1, 0);
            QueueAreaLoopVoice("0121A005", 1, 100, -1, 0);
            QueueAreaLoopVoice("0121H003", 1, 150, -1, 0);
            QueueAreaLoopVoice("0121H004", 1, 295, -1, 0);
            QueueAreaLoopVoice("0121H005", 1, 631, -1, 0);
            break;
        case PAWN_WRP0:
            QueueAreaLoopMotion(PAWN_RYO, 7305, 0, 700, 130.0f, -1.0f, 1.0f, 0.0f, ResolveAreaLoopParent, 6);
            QueueAreaLoopMotion(owner, 7297, 0, 700, 130.0f, -1.0f, 1.0f, 0.0f, 0, 6);
            ownerPosition.x = 0.23f;
            ownerPosition.y = 0.064f;
            ownerPosition.z = -0.08f;
            ownerRotation.x = 1638;
            ownerRotation.y = 17476;
            ownerRotation.z = 21663;
            playerPosition.x = 0.256f;
            playerPosition.y = 0.067f;
            playerPosition.z = -0.059f;
            playerRotation.x = 49880;
            playerRotation.y = 48605;
            playerRotation.z = 1820;
            QueueAreaLoopAttachment(owner, 14, flyer, 230, 109, &ownerPosition, &ownerRotation, 0);
            QueueAreaLoopAttachment(PAWN_RYO, 14, flyer, 340, 120, &playerPosition, &playerRotation, 0);
            playerPosition.x = 1000.0f;
            playerPosition.y = 1000.0f;
            playerPosition.z = 1000.0f;
            QueueAreaLoopAttachment(PAWN_RYO, 14, flyer, 461, 1, &playerPosition, &playerRotation, 0);
            award = 350;
            QueueAreaLoopCamera(work->package, 1640, PAWN_BASE, 0, 0);
            QueueAreaLoopCamera(work->package, 1641, PAWN_BASE, 275, 0);
            QueueAreaLoopCamera(work->package, 1642, PAWN_BASE, 295, 0);
            QueueAreaLoopCamera(work->package, 1643, PAWN_BASE, 340, 0);
            QueueAreaLoopCamera(work->package, 1644, PAWN_BASE, 400, 0);
            QueueAreaLoopVoice("0121F002", 1, 1, -1, 0);
            QueueAreaLoopVoice("0121A005", 1, 130, -1, 0);
            QueueAreaLoopVoice("0121F003", 1, 200, -1, 0);
            QueueAreaLoopVoice("0121F004", 1, 400, -1, 0);
            QueueAreaLoopVoice("0121F005", 1, 550, -1, 0);
            break;
        case PAWN_WTP0:
            QueueAreaLoopMotion(PAWN_RYO, 7302, 0, 640, 0.0f, -1.0f, 1.0f, 0.0f, ResolveAreaLoopParent, 6);
            QueueAreaLoopMotion(owner, 7299, 0, 640, 0.0f, -1.0f, 1.0f, 0.0f, 0, 6);
            ownerPosition.x = 0.229f;
            ownerPosition.y = 0.059f;
            ownerPosition.z = -0.085f;
            ownerRotation.x = 48787;
            ownerRotation.y = 21299;
            ownerRotation.z = 1820;
            playerPosition.x = 0.234f;
            playerPosition.y = 0.057f;
            playerPosition.z = -0.093f;
            playerRotation.x = 41506;
            playerRotation.y = 53521;
            playerRotation.z = 10558;
            QueueAreaLoopAttachment(owner, 14, flyer, 200, 94, &ownerPosition, &ownerRotation, 0);
            QueueAreaLoopAttachment(PAWN_RYO, 14, flyer, 295, 330, &playerPosition, &playerRotation, 0);
            playerPosition.x = 1000.0f;
            playerPosition.y = 1000.0f;
            playerPosition.z = 1000.0f;
            QueueAreaLoopAttachment(PAWN_RYO, 14, flyer, 626, 1, &playerPosition, &playerRotation, 0);
            award = 300;
            QueueAreaLoopCamera(work->package, 1650, PAWN_BASE, 0, 0);
            QueueAreaLoopCamera(work->package, 1651, PAWN_BASE, 215, 0);
            QueueAreaLoopCamera(work->package, 1652, PAWN_BASE, 245, 0);
            QueueAreaLoopCamera(work->package, 1653, PAWN_BASE, 295, 0);
            QueueAreaLoopCamera(work->package, 1654, PAWN_BASE, 420, 0);
            QueueAreaLoopVoice("0121D002", 1, 76, -1, 0);
            QueueAreaLoopVoice("0121A005", 1, 148, -1, 0);
            QueueAreaLoopVoice("0121D003", 1, 168, -1, 0);
            QueueAreaLoopVoice("0121D004", 1, 420, -1, 0);
            QueueAreaLoopVoice("0121D005", 1, 580, -1, 0);
            break;
        case PAWN_WTP1:
            QueueAreaLoopMotion(PAWN_RYO, 7308, 0, 500, 309.0f, -1.0f, 1.0f, 0.0f, ResolveAreaLoopParent, 6);
            QueueAreaLoopMotion(owner, 7300, 0, 500, 309.0f, -1.0f, 1.0f, 0.0f, 0, 6);
            ownerPosition.x = 0.222f;
            ownerPosition.y = 0.029f;
            ownerPosition.z = -0.092f;
            ownerRotation.x = 26214;
            ownerRotation.y = 13107;
            ownerRotation.z = 44418;
            playerPosition.x = 0.23f;
            playerPosition.y = 0.09f;
            playerPosition.z = -0.098f;
            playerRotation.x = 48423;
            playerRotation.y = 52974;
            playerRotation.z = 5825;
            QueueAreaLoopAttachment(owner, 14, flyer, 200, 74, &ownerPosition, &ownerRotation, 0);
            QueueAreaLoopAttachment(PAWN_RYO, 14, flyer, 276, 114, &playerPosition, &playerRotation, 0);
            playerPosition.x = 1000.0f;
            playerPosition.y = 1000.0f;
            playerPosition.z = 1000.0f;
            QueueAreaLoopAttachment(PAWN_RYO, 14, flyer, 391, 1, &playerPosition, &playerRotation, 0);
            award = 280;
            QueueAreaLoopCamera(work->package, 1660, PAWN_BASE, 0, 0);
            QueueAreaLoopCamera(work->package, 1661, PAWN_BASE, 200, 0);
            QueueAreaLoopCamera(work->package, 1662, PAWN_BASE, 275, 0);
            QueueAreaLoopVoice("0121M002", 1, 1, -1, 0);
            QueueAreaLoopVoice("0121A005", 1, 130, -1, 0);
            QueueAreaLoopVoice("0121M003", 1, 190, -1, 0);
            QueueAreaLoopVoice("0121M004", 1, 290, -1, 0);
            QueueAreaLoopVoice("0121M005", 1, 414, -1, 0);
            break;
        case PAWN_WTP2:
            QueueAreaLoopMotion(PAWN_RYO, 7307, 0, 515, 280.0f, -1.0f, 1.0f, 0.0f, ResolveAreaLoopParent, 6);
            QueueAreaLoopMotion(owner, 7317, 0, 515, 270.0f, -1.0f, 1.0f, 0.0f, 0, 6);
            ownerPosition.x = 0.249f;
            ownerPosition.y = 0.072f;
            ownerPosition.z = -0.056f;
            ownerRotation.x = 1638;
            ownerRotation.y = 18022;
            ownerRotation.z = 21481;
            playerPosition.x = 0.225f;
            playerPosition.y = 0.071f;
            playerPosition.z = -0.13f;
            playerRotation.x = 50426;
            playerRotation.y = 52974;
            playerRotation.z = 2548;
            QueueAreaLoopAttachment(owner, 14, flyer, 250, 59, &ownerPosition, &ownerRotation, 0);
            QueueAreaLoopAttachment(PAWN_RYO, 14, flyer, 310, 89, &playerPosition, &playerRotation, 0);
            playerPosition.x = 1000.0f;
            playerPosition.y = 1000.0f;
            playerPosition.z = 1000.0f;
            QueueAreaLoopAttachment(PAWN_RYO, 14, flyer, 400, 1, &playerPosition, &playerRotation, 0);
            award = 315;
            QueueAreaLoopCamera(work->package, 1670, owner, 0, 0);
            QueueAreaLoopCamera(work->package, 1671, owner, 125, 0);
            QueueAreaLoopCamera(work->package, 1672, owner, 215, 0);
            QueueAreaLoopCamera(work->package, 1673, owner, 310, 0);
            QueueAreaLoopCamera(work->package, 1674, owner, 340, 0);
            QueueAreaLoopVoice("0121K002", 1, 30, -1, 0);
            QueueAreaLoopVoice("0121A005", 1, 130, -1, 0);
            QueueAreaLoopVoice("0121K003", 1, 220, -1, 0);
            QueueAreaLoopVoice("0121K004", 1, 315, -1, 0);
            QueueAreaLoopVoice("0121K005", 1, 420, -1, 0);
            break;
        case PAWN_WTP4:
            QueueAreaLoopMotion(PAWN_RYO, 7304, 0, 260, 320.0f, -1.0f, 1.0f, 0.0f, 0, 6);
            QueueAreaLoopMotion(owner, 7314, 0, 260, 320.0f, -1.0f, 1.0f, 0.0f, 0, 6);
            QueueAreaLoopMotion(PAWN_RYO, 7304, 261, 60, 580.0f, -1.0f, 0.0f, 0.0f, 0, 6);
            QueueAreaLoopMotion(owner, 7314, 261, 60, 580.0f, -1.0f, 0.0f, 0.0f, 0, 6);
            QueueAreaLoopMotion(PAWN_RYO, 7304, 321, 200, 620.0f, -1.0f, 1.0f, 0.0f, ResolveAreaLoopParent, 6);
            QueueAreaLoopMotion(owner, 7314, 321, 200, 620.0f, -1.0f, 1.0f, 0.0f, 0, 6);
            ownerPosition.x = 0.252f;
            ownerPosition.y = 0.037f;
            ownerPosition.z = -0.033f;
            ownerRotation.x = 728;
            ownerRotation.y = 17476;
            ownerRotation.z = 17476;
            playerPosition.x = 0.252f;
            playerPosition.y = 0.056f;
            playerPosition.z = -0.012f;
            playerRotation.x = 63897;
            playerRotation.y = 44983;
            playerRotation.z = 52082;
            QueueAreaLoopAttachment(owner, 14, flyer, 225, 59, &ownerPosition, &ownerRotation, 0);
            QueueAreaLoopAttachment(PAWN_RYO, 14, flyer, 321, 110, &playerPosition, &playerRotation, 0);
            playerPosition.x = 1000.0f;
            playerPosition.y = 1000.0f;
            playerPosition.z = 1000.0f;
            QueueAreaLoopAttachment(PAWN_RYO, 14, flyer, 431, 1, &playerPosition, &playerRotation, 0);
            award = 300;
            QueueAreaLoopCamera(work->package, 1680, PAWN_BASE, 0, 0);
            QueueAreaLoopCamera(work->package, 1681, PAWN_BASE, 130, 0);
            QueueAreaLoopCamera(work->package, 1682, PAWN_BASE, 200, 0);
            QueueAreaLoopCamera(work->package, 1683, PAWN_BASE, 250, 0);
            QueueAreaLoopCamera(work->package, 1684, PAWN_BASE, 321, 0);
            QueueAreaLoopVoice("0121C002", 1, 30, -1, 0);
            QueueAreaLoopVoice("0121A005", 1, 130, -1, 0);
            QueueAreaLoopVoice("0121C003", 1, 250, -1, 0);
            QueueAreaLoopVoice("0121C004", 1, 320, -1, 0);
            QueueAreaLoopVoice("0121C005", 1, 430, -1, 0);
            break;
        case PAWN_WTP3:
            QueueAreaLoopMotion(PAWN_RYO, 7309, 0, 540, 250.0f, -1.0f, 1.0f, 0.0f, ResolveAreaLoopParent, 6);
            QueueAreaLoopMotion(owner, 7319, 0, 540, 250.0f, -1.0f, 1.0f, 0.0f, 0, 6);
            ownerPosition.x = 0.237f;
            ownerPosition.y = 0.05f;
            ownerPosition.z = -0.119f;
            ownerRotation.x = 50244;
            ownerRotation.y = 19114;
            ownerRotation.z = 3276;
            playerPosition.x = 0.24f;
            playerPosition.y = 0.035f;
            playerPosition.z = -0.093f;
            playerRotation.x = 49698;
            playerRotation.y = 51154;
            playerRotation.z = 65353;
            QueueAreaLoopAttachment(owner, 14, flyer, 182, 75, &ownerPosition, &ownerRotation, 0);
            QueueAreaLoopAttachment(PAWN_RYO, 14, flyer, 258, 92, &playerPosition, &playerRotation, 0);
            playerPosition.x = 1000.0f;
            playerPosition.y = 1000.0f;
            playerPosition.z = 1000.0f;
            QueueAreaLoopAttachment(PAWN_RYO, 14, flyer, 351, 1, &playerPosition, &playerRotation, 0);
            award = 260;
            QueueAreaLoopCamera(work->package, 1690, PAWN_BASE, 0, 0);
            QueueAreaLoopCamera(work->package, 1691, PAWN_BASE, 182, 0);
            QueueAreaLoopCamera(work->package, 1692, PAWN_BASE, 258, 0);
            QueueAreaLoopVoice("0121L002", 1, 40, -1, 0);
            QueueAreaLoopVoice("0121A005", 1, 140, -1, 0);
            QueueAreaLoopVoice("0121L003", 1, 210, -1, 0);
            QueueAreaLoopVoice("0121L004", 1, 350, -1, 0);
            QueueAreaLoopVoice("0121L005", 1, 470, -1, 0);
            break;
        case PAWN_QBL0:
            QueueAreaLoopMotion(PAWN_RYO, 7310, 0, 775, 540.0f, -1.0f, 1.0f, 0.0f, ResolveAreaLoopParent, 6);
            QueueAreaLoopMotion(owner, 7301, 0, 775, 540.0f, -1.0f, 1.0f, 0.0f, 0, 6);
            ownerPosition.x = 0.228f;
            ownerPosition.y = 0.079f;
            ownerPosition.z = -0.08f;
            ownerRotation.x = 3276;
            ownerRotation.y = 17658;
            ownerRotation.z = 24758;
            playerPosition.x = 0.256f;
            playerPosition.y = 0.057f;
            playerPosition.z = 0.024f;
            playerRotation.x = 34770;
            playerRotation.y = 53703;
            playerRotation.z = 14745;
            QueueAreaLoopAttachment(owner, 14, flyer, 330, 78, &ownerPosition, &ownerRotation, 0);
            QueueAreaLoopAttachment(PAWN_RYO, 14, flyer, 409, 169, &playerPosition, &playerRotation, 0);
            playerPosition.x = 1000.0f;
            playerPosition.y = 1000.0f;
            playerPosition.z = 1000.0f;
            QueueAreaLoopAttachment(PAWN_RYO, 14, flyer, 580, 1, &playerPosition, &playerRotation, 0);
            award = 410;
            QueueAreaLoopCamera(work->package, 1700, PAWN_BASE, 0, 0);
            QueueAreaLoopCamera(work->package, 1701, PAWN_BASE, 260, 0);
            QueueAreaLoopCamera(work->package, 1702, PAWN_BASE, 330, 0);
            QueueAreaLoopCamera(work->package, 1705, PAWN_BASE, 409, 0);
            QueueAreaLoopCamera(work->package, 1704, PAWN_BASE, 649, 0);
            QueueAreaLoopVoice("0121J002", 1, 1, -1, 0);
            QueueAreaLoopVoice("0121A005", 1, 150, -1, 0);
            QueueAreaLoopVoice("0121J003", 1, 200, -1, 0);
            QueueAreaLoopVoice("0121J004", 1, 400, -1, 0);
            QueueAreaLoopVoice("0121J005", 1, 600, -1, 0);
            break;
        case PAWN_QBR0:
            QueueAreaLoopMotion(PAWN_RYO, 7305, 0, 700, 130.0f, -1.0f, 1.0f, 0.0f, ResolveAreaLoopParent, 6);
            QueueAreaLoopMotion(owner, 7297, 0, 700, 130.0f, -1.0f, 1.0f, 0.0f, 0, 6);
            ownerPosition.x = 0.156f;
            ownerPosition.y = 0.11f;
            ownerPosition.z = -0.141f;
            ownerRotation.x = 46603;
            ownerRotation.y = 24557;
            ownerRotation.z = 6553;
            playerPosition.x = 0.244203f;
            playerPosition.y = 0.045057f;
            playerPosition.z = -0.084f;
            playerRotation.x = 7281;
            playerRotation.y = 45693;
            playerRotation.z = 43690;
            QueueAreaLoopAttachment(owner, 14, flyer, 230, 110, &ownerPosition, &ownerRotation, 0);
            QueueAreaLoopAttachment(PAWN_RYO, 14, flyer, 341, 119, &playerPosition, &playerRotation, 0);
            playerPosition.x = 1000.0f;
            playerPosition.y = 1000.0f;
            playerPosition.z = 1000.0f;
            QueueAreaLoopAttachment(PAWN_RYO, 14, flyer, 461, 1, &playerPosition, &playerRotation, 0);
            award = 350;
            QueueAreaLoopCamera(work->package, 1720, PAWN_BASE, 0, 0);
            QueueAreaLoopCamera(work->package, 1721, PAWN_BASE, 275, 0);
            QueueAreaLoopCamera(work->package, 1722, PAWN_BASE, 295, 0);
            QueueAreaLoopCamera(work->package, 1723, PAWN_BASE, 340, 0);
            QueueAreaLoopCamera(work->package, 1724, PAWN_BASE, 400, 0);
            QueueAreaLoopVoice("0121I002", 1, 1, -1, 0);
            QueueAreaLoopVoice("0121A005", 1, 130, -1, 0);
            QueueAreaLoopVoice("0121I003", 1, 200, -1, 0);
            QueueAreaLoopVoice("0121I004", 1, 450, -1, 0);
            QueueAreaLoopVoice("0121I005", 1, 600, -1, 0);
            break;
        }
        task = FTBL.HLib_EnqueueTaskWithParameter(PawnFlyerAward, 4, 11, sizeof(PawnFlyerAwardWork), TASK_EVNT);
        if (task) {
            work->awardTask = task;
            awardWork = (PawnFlyerAwardWork *)FTBL.HLib_GetTaskParameter(task);
            if (awardWork) {
                awardWork->elapsed = 0;
                awardWork->frame = award;
                awardWork->item = FindPawnItem(work->shop);
            } else
                FTBL.HLib_DestroyTask(task);
        }
    } while (0);
}

void PawnFlyerAward(void *data)
{
    PawnFlyerAwardWork *work = (PawnFlyerAwardWork *)data;
    if (work && work->frame >= 0 && work->frame <= ++work->elapsed) {
        FTBL.AddInventoryItem(work->item, 1, 1);
        work->frame = -1;
    }
}

void PawnFlyerDone(void *data)
{
    PawnWork *work = (PawnWork *)data;
    shen_name owner, flyer;
    Vector3i rotation;
    if (work) {
        flyer = FindPawnFlyer(work->shop);
        FTBL.SetRefBoardParamsForCharaById(flyer, 0);
        FTBL.ReleaseCharacterFaceCache(FTBL.ctrlGetCharaByName(PAWN_RYO));
        FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(PAWN_RYO));
        owner = FindPawnOwner(work->shop);
        if (owner) {
            FTBL.ReleaseCharacterFaceCache(FTBL.ctrlGetCharaByName(owner));
            FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(owner));
        }
        FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(PAWN_RYO), 17, 0);
        FTBL.GetScenarioCharacterEulerAngles(PAWN_RYO, &rotation, 0x40000000);
        rotation.y += 0x8000;
        FTBL.EV_AT_SetAngY_0(PAWN_RYO, 0x78000000, &rotation, 0);
        FTBL.EV_FreeAuthorPak(0, pawnConversation);
        RestorePawnTransactionPose(work, work->shop);
        FTBL.HLib_SetCurrentTaskTo(PawnBeginSelect);
    } else
        FTBL.HLib_SetCurrentTaskTo(PawnDone);
}

void PawnTalkOpening(void *data)
{
    PawnWork *work = (PawnWork *)data;
    shen_name owner;
    do {
        if (!work || (owner = FindPawnOwner(work->shop)) == 0) {
            FTBL.HLib_SetCurrentTaskTo(PawnDone);
            break;
        }
        FTBL.HLib_SetCurrentTaskTo(PawnToSelect);
        FTBL.HLib_SetupTaskDelay(-1);
        QueuePawnTalk(FTBL.HLib_GetCurrentTask(), owner, 1222);
    } while (0);
}

void RestorePawnTransactionPose(PawnWork *work, shen_name shop)
{
    unsigned int motion = 7324;
    unsigned int playerMotion = 7323;
    shen_name base;
    int camera;
    switch (shop) {
    case PAWN_ARP0:
        base = PAWN_BASE;
        camera = 2600;
        break;
    case PAWN_WSP0:
        motion = 7328;
        base = PAWN_BASE;
        camera = 2610;
        break;
    case PAWN_WKP0:
        motion = 7325;
        base = PAWN_BASE;
        camera = 2620;
        break;
    case PAWN_WKP1:
        base = PAWN_BASE;
        camera = 2630;
        break;
    case PAWN_WRP0:
        motion = 7325;
        base = PAWN_BASE;
        camera = 2640;
        break;
    case PAWN_WTP0:
        motion = 7325;
        base = PAWN_BASE;
        camera = 2650;
        break;
    case PAWN_WTP1:
        motion = 7322;
        base = PAWN_BASE;
        camera = 2660;
        break;
    case PAWN_WTP2:
        base = PAWN_BASE;
        camera = 2670;
        break;
    case PAWN_WTP4:
        base = PAWN_BASE;
        camera = 2680;
        break;
    case PAWN_WTP3:
        base = PAWN_BASE;
        camera = 2690;
        break;
    case PAWN_QBL0:
        motion = 7322;
        base = PAWN_BASE;
        camera = 2670;
        break;
    case PAWN_QBR0:
        motion = 7325;
        base = PAWN_BASE;
        camera = 2670;
        break;
    default:
        base = 0;
        camera = 0;
        break;
    }
    if (camera) {
        FTBL.EV_ControlPackageResources(2, 4, work->package, 0, 0);
        if (base)
            FTBL.EVS_LoopCamera(camera, base, base);
        else
            FTBL.EVS_LoopCamera(camera, 0, 0);
        QueueAreaLoopMotion(FindPawnOwner(shop), motion, 0, 1, -1.0f, -1.0f, 1.0f, 0.0f, 0, 2);
        QueueAreaLoopMotion(PAWN_RYO, playerMotion, 0, 1, -1.0f, -1.0f, 1.0f, 0.0f, 0, 2);
    }
}

void PawnToSelect(void *data)
{
    PawnWork *work = (PawnWork *)data;
    if (work) {
        if (!FTBL.IsScenarioBusy()) {
            FindPawnOwner(work->shop);
            FTBL.HLib_SetCurrentTaskTo(PawnBeginSelect);
            RestorePawnTransactionPose(work, work->shop);
        }
    } else
        FTBL.HLib_SetCurrentTaskTo(PawnDone);
}

void PawnBeginSelect(void *data)
{
    PawnWork *work = (PawnWork *)data;
    short choices[4];
    Vector3 position, offset;
    Vector3i rotation, playerRotation;
    shen_name owner;
    unsigned int modes;
    do {
        if (!work || (owner = FindPawnOwner(work->shop)) == 0) {
            FTBL.HLib_SetCurrentTaskTo(PawnDone);
            break;
        }
        FTBL.InitiateCleanupForSceneChara(owner);
        if (FTBL.shenGetMinutesUntilNextFrameMinute() <= 0) {
            FTBL.HLib_SetCurrentTaskTo(PawnNotSelling);
            break;
        }
        rotation.x = 0;
        rotation.z = 0;
        playerRotation.x = 0;
        playerRotation.z = 0;
        if (GetPawnStandData(work->shop, &position, &rotation.y, &offset, &playerRotation.y, 0)) {
            FTBL.EV_SetCharacterAlphaEnabled(owner, 0);
            FTBL.EV_SetCharacterAlphaEnabled(PAWN_RYO, 0);
            FTBL.SetPlayerGameFlags(5, (void *)1);
        }
        if (FTBL.GetShouldRenderIcons(4))
            break;
        modes = FTBL.GetItemSelectionAvailability(work->shop) & 11;
        if (modes) {
            switch (modes) {
            case 11:
            default:
                choices[1] = 237;
                choices[2] = 238;
                choices[0] = 204;
                choices[3] = 154;
                break;
            case 3:
                choices[1] = 237;
                choices[2] = 238;
                choices[0] = -1;
                choices[3] = 154;
                break;
            case 10:
                choices[1] = 238;
                choices[2] = 204;
                choices[0] = -1;
                choices[3] = 154;
                break;
            case 9:
                choices[1] = 237;
                choices[2] = 204;
                choices[0] = -1;
                choices[3] = 154;
                break;
            case 8:
                choices[1] = 204;
                choices[2] = 154;
                choices[0] = -1;
                choices[3] = -1;
                break;
            case 1:
                choices[1] = 237;
                choices[2] = 154;
                choices[0] = -1;
                choices[3] = -1;
                break;
            case 2:
                choices[1] = 238;
                choices[2] = 154;
                choices[0] = -1;
                choices[3] = -1;
                break;
            }
            FTBL.SetupControlSpeakAndUI(choices, 800, -1, 2);
            FTBL.HLib_SetCurrentTaskTo(PawnExecuteSelect);
        } else {
            FTBL.HLib_SetCurrentTaskTo(PawnDone);
            FTBL.HLib_SetupTaskDelay(-1);
            QueuePawnTalk(FTBL.HLib_GetCurrentTask(), owner, 1224);
        }
    } while (0);
}

void PawnExecuteSelect(void *data)
{
    PawnWork *work = (PawnWork *)data;
    HLibTaskCallback next;
    int open, mode;
    if (work) {
        if (FTBL.GetShouldRenderIcons(2))
            return;
        switch (FTBL.shenGetCtrlSpeakIconFlag()) {
        case 205:
        case 237:
            next = PawnSetTransaction;
            open = 1;
            mode = 2;
            break;
        case 204:
            next = PawnItemTransaction;
            open = 1;
            mode = 5;
            break;
        case 238:
            next = PawnItemTransaction;
            open = 1;
            mode = 3;
            break;
        case 154:
            next = PawnNotSelling;
            open = 0;
            mode = 0;
            break;
        default:
            next = PawnCancel;
            open = 0;
            mode = 0;
            break;
        }
        if (open && !FTBL.OpenPAWNShop(work->shop, mode)) {
            FTBL.EV_ResetFreeTalkFlags();
            next = 0;
        }
        if (next) {
            FTBL.HLib_SetCurrentTaskTo(next);
            if (!open)
                next(work);
        }
    } else
        FTBL.HLib_SetCurrentTaskTo(PawnDone);
}

void PawnCancel(void *data)
{
    PawnWork *work = (PawnWork *)data;
    shen_name owner;
    do {
        if (!work || (owner = FindPawnOwner(work->shop)) == 0) {
            FTBL.HLib_SetCurrentTaskTo(PawnDone);
            break;
        }
        FTBL.HLib_SetCurrentTaskTo(PawnDone);
        FTBL.HLib_SetupTaskDelay(-1);
        QueuePawnTalk(FTBL.HLib_GetCurrentTask(), owner, 1225);
    } while (0);
}

void PawnSetTransaction(void *data)
{
    PawnWork *work = (PawnWork *)data;
    shen_name owner;
    HLibTask *parent;
    int dialogue, value;
    do {
        if (!work || (owner = FindPawnOwner(work->shop)) == 0) {
            FTBL.HLib_SetCurrentTaskTo(PawnDone);
            break;
        }
        if (!FTBL.IsPAWNShopOpen()) {
            if (FTBL.GetItemSelectionResult() >= 0) {
                FTBL.HLib_SetCurrentTaskTo(PawnSaleDialogue);
                parent = 0;
                value = FTBL.GetItemSelectionCount();
                if (value > 200) {
                    value = 1;
                    dialogue = 1228;
                } else if (value > 100) {
                    value = 2;
                    dialogue = 1227;
                } else {
                    value = 3;
                    dialogue = 1229;
                }
            } else {
                FTBL.HLib_SetCurrentTaskTo(PawnToSelect);
                FTBL.HLib_SetupTaskDelay(-1);
                parent = FTBL.HLib_GetCurrentTask();
                dialogue = 1226;
                value = -1;
            }
            work->saleOutcome = value;
            QueuePawnTalk(parent, owner, dialogue);
        } else if (FTBL.shenGetMinutesUntilNextFrameMinute() <= 0) {
            FTBL.HLib_SetCurrentTaskTo(PawnNotSelling);
            FTBL.SetItemSelectionPageSize(2);
            FTBL.ClosePAWNShop();
        }
    } while (0);
}

void PawnItemTransaction(void *data)
{
    PawnWork *work = (PawnWork *)data;
    shen_name owner;
    HLibTask *parent;
    int dialogue, outcome;
    do {
        if (!work || (owner = FindPawnOwner(work->shop)) == 0) {
            FTBL.HLib_SetCurrentTaskTo(PawnDone);
            break;
        }
        if (FTBL.IsPAWNShopOpen()) {
            if (FTBL.shenGetMinutesUntilNextFrameMinute() <= 0) {
                FTBL.HLib_SetCurrentTaskTo(PawnNotSelling);
                FTBL.SetItemSelectionPageSize(2);
                FTBL.ClosePAWNShop();
            }
        } else {
            if (FTBL.GetItemSelectionResult() >= 0) {
                FTBL.HLib_SetCurrentTaskTo(PawnSaleDialogue);
                parent = 0;
                dialogue = 1227;
                outcome = 4;
            } else {
                FTBL.HLib_SetCurrentTaskTo(PawnToSelect);
                FTBL.HLib_SetupTaskDelay(-1);
                parent = FTBL.HLib_GetCurrentTask();
                dialogue = 1226;
                outcome = 0;
            }
            work->saleOutcome = outcome;
            QueuePawnTalk(parent, owner, dialogue);
        }
    } while (0);
}

void PawnNotSelling(void *data)
{
    FTBL.HLib_SetCurrentTaskTo(PawnCancel);
    PawnCancel(data);
}

void PawnSaleDialogue(void *data)
{
    PawnWork *work = (PawnWork *)data;
    int pending[8];
    int i;
    pending[0] = 1222;
    pending[1] = 1224;
    pending[2] = 1225;
    pending[3] = 1226;
    pending[4] = 1227;
    pending[5] = 1228;
    pending[6] = 1229;
    pending[7] = -1;
    do {
        if (!work || !FindPawnOwner(work->shop)) {
            FTBL.HLib_SetCurrentTaskTo(PawnDone);
            break;
        }
        if (FTBL.EV_GetPlayerFlag(11, 1223)) {
            FTBL.EV_SetPlayerFlag(12, 1223, 0);
            FTBL.AddThisToCashOrSegaCoins(FTBL.ConsumeItemSelection(), 2);
        }
        if (FTBL.IsScenarioBusy())
            break;
        for (i = 0; pending[i] != -1; i++)
            if (FTBL.EV_GetPlayerFlag(11, pending[i]))
                break;
        if (pending[i] != -1)
            break;
        RestorePawnTransactionPose(work, work->shop);
        FTBL.HLib_SetCurrentTaskTo(PawnBeginSelect);
    } while (0);
}

int PawnFlyerRequested(shen_name shop)
{
    int result = 0;
    int i, item, flag;
    for (i = 0; pawnShopScenes[i].shop; i++) {
        if (pawnShopScenes[i].shop == shop) {
            item = FindPawnItem(shop);
            if (item)
                result = FTBL.GetItemState(item) == 0;
            flag = pawnShopScenes[i].presentationFlag;
            if (flag) {
                result |= FTBL.EV_GetPlayerFlag(11, flag);
                FTBL.EV_SetPlayerFlag(12, flag, 0);
            }
            break;
        }
    }
    return result;
}

shen_name FindPawnOwner(shen_name shop)
{
    int i = 0;
    while (pawnShopScenes[i].shop != shop && pawnShopScenes[i].shop)
        i++;
    return pawnShopScenes[i].owner;
}

shen_name FindPawnFlyer(shen_name shop)
{
    int i = 0;
    while (pawnShopScenes[i].shop != shop && pawnShopScenes[i].shop)
        i++;
    return pawnShopScenes[i].flyer;
}

int FindPawnItem(shen_name shop)
{
    int item = 9000;
    int i;
    for (i = 0; pawnShopItems[i].shop; i++)
        if (pawnShopItems[i].shop == shop)
            item = pawnShopItems[i].item;
    return item;
}

int GetPawnStandData(shen_name shop, Vector3 *position, int *yaw, Vector3 *offset, int *playerYaw, int flyer)
{
    Vector3 standPosition;
    Vector3i rotation;
    int result = 0;
    int i;
    for (i = 0; pawnShopScenes[i].shop; i++) {
        if (pawnShopScenes[i].shop == shop) {
            FTBL.GetCharaPosition(PAWN_RYO, -1, &standPosition, 0x40000000);
            if (flyer) {
                standPosition.x = pawnShopScenes[i].flyerX;
                standPosition.z = pawnShopScenes[i].flyerZ;
                rotation.y = pawnShopScenes[i].flyerYaw;
                if (offset) {
                    offset->x = pawnShopScenes[i].flyerOffsetX;
                    offset->z = pawnShopScenes[i].flyerOffsetZ;
                }
            } else {
                standPosition.x = pawnShopScenes[i].businessX;
                standPosition.z = pawnShopScenes[i].businessZ;
                rotation.y = pawnShopScenes[i].businessYaw;
                if (offset) {
                    offset->x = pawnShopScenes[i].businessOffsetX;
                    offset->z = pawnShopScenes[i].businessOffsetZ;
                }
            }
            if (position)
                *position = standPosition;
            if (yaw)
                *yaw = rotation.y;
            if (offset) {
                offset->y = 0.0f;
                rotation.x = 0;
                rotation.z = 0;
                FTBL.ApplyScenarioTranslationAndRotation(&standPosition, &rotation, offset);
            }
            if (playerYaw)
                *playerYaw = rotation.y + 0x8000;
            result = 1;
            break;
        }
    }
    return result;
}

int QueuePawnTalk(HLibTask *parent, shen_name character, int flag)
{
    int result = 0;
    HLibTask *task = 0;
    PawnTalkWork *work;
    while (character && flag && (task = FTBL.HLib_EnqueueTaskWithParameter(PawnTalkCallback, 4, 11, sizeof(PawnTalkWork), TASK_EVNT)) != 0) {
        work = (PawnTalkWork *)FTBL.HLib_GetTaskParameter(task);
        if (!work)
            break;
        work->parent = parent;
        work->flag = flag;
        work->character = character;
        FTBL.EV_SetPlayerFlag(12, flag, 1);
        FTBL.EVS_LoopCamera(3000, PAWN_BASE, PAWN_BASE);
        FTBL.camCtrlSetState(27);
        FTBL.SetScenarioFollowCharacter(character);
        work->started = FTBL.StartScenarioFreeTalkRecord(character, -1);
        result = 1;
        break;
    }
    if (!result && task)
        FTBL.HLib_DestroyTask(task);
    return result;
}

void PawnTalkCallback(void *data)
{
    PawnTalkWork *work = (PawnTalkWork *)data;
    int done = 1;
    if (work) {
        done = FTBL.IsScenarioBusy() == 0;
        if (work->started) {
            if (FTBL.camCtrlCheckIsEventCam())
                FTBL.SetItemSelectionPageSize(0x10001);
            if (done) {
                if (work->parent) {
                    FTBL.HLib_ResolveDeferredCallback(work->parent);
                    FTBL.HLib_SetupTimedTask(work->parent, 1);
                }
                FTBL.SetPlayerGameFlags(5, (void *)1);
                if ((unsigned int)(work->flag - 1224) > 1)
                    FTBL.SetScenarioFollowCharacter(work->character);
                else {
                    FTBL.srchReset();
                    FTBL.camCtrlSetFovTransitionTo(1);
                    FTBL.camCtrlSetState(0);
                    FTBL.InitiateCleanupForSceneChara(work->character);
                }
            }
        } else {
            if (done)
                work->started = FTBL.StartScenarioFreeTalkRecord(work->character, -1);
            done = 0;
        }
    }
    if (done)
        FTBL.HLib_DestroyCurrentEventTask();
}

#define SHENMUE_AREA_LOOP_HAND_POSE_TABLE
#include "s2/areas/area_loop_dc.c"
#endif
