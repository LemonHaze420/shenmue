#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_ro10.hpp"

static Ro10Shop ro10Shops[7] = {
    {SHEN_NAME4('A', 'R', 'D', 'X'), SHEN_NAME4('S', 'Y', 'R', '_'), 0, 0, 0, SHEN_NAME4('W', '0', '0', '1'), 56,
     0.25f, 0.2f, {147.34f, 101.61f, 313.12f}, {10922, 0, 0}, 4, 4, 147.44f, 314.4f, 0, 145.56f, 313.21f, 40960, 11458, 11457,
     RO10_ITEM_OBJECTS,
     {0, 50, 50, 0, 0, 50, 50, 50, 50, 50, 50, 50, 0, 50, 50, 0},
     {0, 4278, 4278, 0, 0, 4272, 4272, 4272, 4274, 4274, 4274, 4274, 0, 4277, 4277, 0},
     {0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0},
     {0, 40, 0, 0, 0, 0, 41, 0, 42, 0, 0, 0, 0, 0, 43, 0}},
    {SHEN_NAME4('A', 'R', 'X', '2'), SHEN_NAME4('M', 'Y', 'H', '_'), 0, 0, 0, SHEN_NAME4('W', '0', '0', '1'), 56,
     0.25f, 0.2f, {223.95f, 101.61f, 365.15f}, {10922, 49152, 0}, 4, 4, 222.75f, 365.5f, 49152, 223.4f, 363.78f, 14017, 11464, 11463,
     RO10_ITEM_OBJECTS,
     {0, 50, 50, 50, 50, 50, 50, 50, 0, 0, 50, 50, 0, 50, 50, 50},
     {0, 4270, 4270, 4270, 4278, 4278, 4278, 4278, 0, 0, 4269, 4269, 0, 4267, 4267, 4267},
     {0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 2, 2, 2},
     {0, 0, 1, 0, 0, 2, 0, 0, 0, 0, 0, 3, 0, 0, 4, 0}},
    {SHEN_NAME4('A', 'R', 'X', 'X'), SHEN_NAME4('K', 'A', 'N', '_'), 2040, 2041, 2056, SHEN_NAME4('W', '0', '0', '2'), 59,
     0.25f, 0.2f, {227.44f, 100.72f, 408.4f}, {10922, 49152, 0}, 4, 4, 226.35f, 408.81f, 39139, 227.27f, 410.34f, 9830, 0, 0,
     RO10_ITEM_OBJECTS,
     {25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 0, 0, 0, 0, 0, 0},
     {-1, -2, -3, -3, -4, -4, -5, -5, -6, -6, 0, 0, 0, 0, 0, 0},
     {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0},
     {30, 0, 0, 30, 0, 30, 0, 30, 0, 0, 0, 0, 0, 0, 0, 0}},
    {SHEN_NAME4('A', 'R', 'S', 'F'), SHEN_NAME4('C', 'N', '0', '_'), 0, 0, 0, SHEN_NAME4('W', '0', '0', '1'), 59,
     0.25f, 0.2f, {290.16f, 109.26f, 329.75f}, {10740, 16384, 0}, 4, 4, 291.48f, 329.3f, 29072, 290.38f, 331.38f, 59346, 0, 0,
     RO10_ITEM_OBJECTS,
     {25, 25, 25, 25, 25, 25, 0, 0, 0, 50, 50, 50, 50, 50, 0, 50},
     {-1, -2, -5, -5, -6, -6, 0, 0, 0, 4276, 4276, 4276, 4273, 4273, 0, 4264},
     {3, 3, 3, 3, 3, 3, 0, 0, 0, 1, 1, 1, 1, 1, 0, 2},
     {20, 0, 0, 0, 0, 0, 0, 0, 0, 21, 0, 0, 22, 0, 0, 23}},
    {SHEN_NAME4('A', 'K', 'X', '7'), SHEN_NAME4('G', 'N', 'B', '_'), 0, 0, 0, SHEN_NAME4('W', '0', '0', '1'), 56,
     0.25f, 0.2f, {226.22f, 101.4f, 260.95f}, {10922, 32768, 0}, 4, 4, 226.21f, 259.56f, 43745, 227.84f, 260.47f, 11104, 0, 0,
     RO10_ITEM_OBJECTS,
     {50, 50, 0, 0, 0, 50, 50, 50, 50, 50, 50, 50, 0, 50, 50, 50},
     {4278, 4278, 0, 0, 0, 4272, 4272, 4272, 4274, 4274, 4274, 4274, 0, 4263, 4268, 4265},
     {1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 2, 2, 2},
     {0, 50, 0, 0, 0, 0, 51, 0, 52, 0, 0, 0, 0, 53, 54, 55}},
    {SHEN_NAME4('Q', 'R', '0', '0'), SHEN_NAME4('M', 'F', 'M', '_'), 0, 0, 0, SHEN_NAME4('W', '0', '0', '1'), 56,
     0.25f, 0.2f, {153.25f, 101.4f, 202.98f}, {10922, 29127, 0}, 4, 4, 153.47f, 201.47f, 19642, 152.01f, 201.62f, 50535, 0, 0,
     RO10_ITEM_OBJECTS,
     {50, 50, 50, 50, 50, 50, 50, 50, 0, 50, 50, 0, 50, 50, 0, 50},
     {4280, 4280, 4280, 4280, 4271, 4271, 4271, 4271, 0, 4279, 4279, 0, 4275, 4275, 0, 4266},
     {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 2},
     {0, 10, 0, 0, 0, 11, 0, 0, 0, 0, 12, 0, 13, 0, 0, 14}},
    {0, 0, 0, 0, 0, 0, 0,
     0.25f, 0.2f, {0.0f, 0.0f, 0.0f}, {0, 0, 0}, 4, 4, 0.0f, 0.0f, 0, 0.0f, 0.0f, 0, 0, 0,
     RO10_ITEM_OBJECTS,
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}
};
static Vector3i ro10GripRotations[7] = {{0, 0, 0}, {0, 0, 16384}, {16384, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
static Vector3 ro10GripScales[4] = {{1.5f, 1.5f, 1.5f}, {1.5f, 1.5f, 1.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}};
static char ro10Conversation[] = "0110";
static int ro10Prizes1[24] = {
    4072, 4073, 4074, 4075, 4076, 4083, 4084, 4085, 4086, 4044, 4045, 4024, 4026, 4028, 4030, 4032, 4034, 4036, 4038, 4040, 4042, 4068, 4069, 4070};
static int ro10Prizes2[23] = {
    4077, 4078, 4079, 4080, 4081, 4082, 4087, 4088, 4044, 4045, 4025, 4027, 4029, 4031, 4033, 4035, 4037, 4039, 4041, 4043, 4162, 4163, 4164};
static int ro10Prizes3[24] = {
    4072, 4073, 4074, 4075, 4076, 4083, 4084, 4085, 4086, 4016, 4017, 4202, 4203, 4204, 4205, 4206, 4207, 4208, 4209, 4210, 4211, 4068, 4069, 4070};
static int ro10Prizes4[23] = {
    4077, 4078, 4079, 4080, 4081, 4082, 4087, 4088, 4016, 4017, 4212, 4213, 4214, 4215, 4216, 4217, 4218, 4219, 4220, 4221, 4162, 4163, 4164};
static int ro10Prizes5[24] = {
    4072, 4073, 4074, 4075, 4076, 4083, 4084, 4085, 4086, 4222, 4223, 4224, 4225, 4226, 4232, 4233, 4235, 4236, 4237, 4238, 4242, 4068, 4069, 4070};
static int ro10Prizes6[21] = {
    4077, 4078, 4079, 4080, 4081, 4082, 4087, 4088, 4019, 4122, 4066, 4067, 4094, 4095, 4096, 4097, 4098, 4099, 4162, 4163, 4164};
static Vector3i ro10SmallRotation = {57891, 61714, 364};
static int ro10SmallReach[19][3] = {
    {0, 1090, 64262}, {0, 1456, 4732}, {0, 61714, 3276}, {0, 909, 60440}, {0, 0, 62442}, {0, 0, 64990}, {0, 546, 3458}, {0, 0, 53160}, {0, 0, 49702},
    {0, 0, 54980}, {0, 546, 1638}, {0, 0, 54616}, {0, 0, 52978}, {0, 0, 57347}, {0, 546, 909}, {0, 0, 51704}, {0, 0, 55708}, {0, 0, 60441},
    {0, 546, 64809}};
static int ro10SmallGrip[19][3] = {
    {0, 1090, 64262}, {0, 1456, 3458}, {0, 61714, 3276}, {0, 909, 59530}, {0, 0, 62078}, {0, 0, 63898}, {0, 546, 3822}, {0, 0, 53160}, {0, 0, 49702},
    {0, 0, 54980}, {0, 546, 1638}, {0, 0, 54616}, {0, 0, 52978}, {0, 0, 57347}, {0, 546, 909}, {0, 0, 51704}, {0, 0, 55708}, {0, 0, 60441},
    {0, 546, 64809}};
static Vector3 ro10SmallOffset = {0.01f, -0.06f, 0.0f};
static Vector3i ro10TallRotation = {53704, 60622, 2184};
static int ro10TallReach[19][3] = {
    {0, 726, 58074}, {0, 1456, 3822}, {0, 62624, 5642}, {0, 1274, 58074}, {0, 1820, 60076}, {0, 909, 61168}, {0, 727, 2730}, {0, 1092, 50066},
    {0, 1092, 50612}, {0, 2002, 53524}, {0, 909, 727}, {0, 0, 52978}, {0, 0, 52614}, {0, 0, 55709}, {0, 546, 64079}, {0, 0, 51158}, {0, 0, 54434},
    {0, 0, 58257}, {0, 546, 63170}};
static int ro10TallGrip[19][3] = {
    {0, 1272, 54798}, {0, 1456, 4550}, {0, 62624, 5642}, {0, 1274, 58074}, {0, 1820, 59530}, {0, 2184, 60440}, {0, 1638, 2548}, {0, 1092, 50066},
    {0, 1092, 50612}, {0, 2002, 53524}, {0, 909, 364}, {0, 0, 52978}, {0, 0, 52614}, {0, 0, 55163}, {0, 546, 64807}, {0, 0, 51158}, {0, 0, 54434},
    {0, 0, 58257}, {0, 546, 63170}};
static Vector3 ro10TallOffset = {0.03f, -0.06f, -0.03f};
static Vector3i ro10PacketRotation = {52976, 61350, 3458};
static int ro10PacketReach[19][3] = {
    {0, 726, 64262}, {0, 1456, 4004}, {0, 64080, 3640}, {0, 64990, 60804}, {0, 0, 61896}, {0, 182, 65354}, {0, 546, 4004}, {0, 0, 60258},
    {0, 0, 61350}, {0, 0, 727}, {0, 546, 4004}, {0, 0, 62442}, {0, 0, 60440}, {0, 0, 547}, {0, 546, 4913}, {0, 0, 61350}, {0, 0, 64262},
    {0, 64444, 1275}, {0, 546, 3822}};
static int ro10PacketGrip[19][3] = {
    {0, 180, 61714}, {0, 1456, 1456}, {0, 64080, 3640}, {0, 64990, 59530}, {0, 0, 63534}, {0, 182, 364}, {0, 546, 2730}, {0, 0, 58984},
    {0, 0, 62078}, {0, 0, 182}, {0, 546, 3458}, {0, 0, 61896}, {0, 0, 60076}, {0, 0, 183}, {0, 546, 4549}, {0, 727, 61350}, {0, 727, 61350},
    {0, 64444, 910}, {0, 546, 3640}};
static Vector3 ro10PacketOffset = {0.0f, 0.0f, 0.0f};

void RunRo10(const shen_name *argument)
{
    HLibTask *task = 0;
    Ro10Work *work;
    const Ro10Shop *shop;
    shen_name name;
    int alternate, index;
    char *package;
    while (argument) {
        name = *argument;
        if (*(char *)&name >= 'a' && *(char *)&name <= 'z') {
            *(char *)&name &= 0xdf;
            alternate = 1;
        } else
            alternate = 0;
        shop = FindRo10Shop(name);
        if (!shop)
            break;
        if (alternate && shop->resetFlag)
            FTBL.EV_SetPlayerFlag(12, shop->resetFlag, 0);
        task = FTBL.HLib_EnqueueTaskWithParameter(WaitShopReady, 4, 11, sizeof(Ro10Work), TASK_EVNT);
        if (!task)
            break;
        work = (Ro10Work *)FTBL.HLib_GetTaskParameter(task);
        if (!work)
            break;
        package = (char *)work;
        for (index = 0; index < (int)sizeof(Ro10Work); index++)
            package[index] = 0;
        work->parent = FTBL.HLib_GetCurrentTask();
        work->self = task;
        package = work->package;
        *package++ = 'R';
        *package++ = 'O';
        *package++ = '1';
        *package++ = '0';
        *package++ = ((char *)&name)[0];
        *package++ = ((char *)&name)[1];
        *package++ = ((char *)&name)[2];
        *package++ = ((char *)&name)[3];
        *package = 0;
        for (index = 0; ro10Conversation[index]; index++)
            work->audio[index] = ro10Conversation[index];
        work->audio[index] = 0;
        work->shop = name;
        work->flags = alternate ? 0x20000 : 0;
        work->definition = shop;
        work->owner = shop->owner;
        EventDebugRegister(RO10_DEBUG_ID, Ro10DebugQuit, work, 0x7fffffff);
        if (FTBL.GetNpcNewLoadState())
            work->flags |= 0x10000;
        if (!alternate) {
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->flags |= 1;
        }
        FTBL.SetPlayerControlEnabled(0);
        if (shop)
            FTBL.EV_EventSetUpHelpIcon_0(shop->help, 0);
        task = 0;
        break;
    }
    if (task)
        FTBL.HLib_DestroyTask(task);
}

#include "s2/areas/area_loop_dc.c"

void Ro10DebugQuit(void *data)
{
    Ro10Work *work = (Ro10Work *)data;
    FTBL.HLib_AssignCallbackForTask(work->self, Ro10Quit);
    FTBL.HLib_SetupTimedTask(work->self, 1);
}

void Ro10Quit(void *data)
{
    Ro10Work *work = (Ro10Work *)data;
    HLibTask *parent;
    HLibTask *task;
    Ro10ExitWork *exitWork;
    const Ro10Shop *shop;
    shen_name names[2];
    int alternate = 0;
    int index;
    while (!FTBL.filmCheckFadeTrackColour()) {
        if (work) {
            if (!(work->flags & 1)) {
                FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
                work->flags |= 1;
                break;
            }
            parent = work->parent;
            if (work->definition && work->definition->prop)
                FTBL.SetDisplayFlagForProp(work->definition->prop, 1);
            for (index = 0; index < 16; index++) {
                if (work->boards[index])
                    FTBL.SetScenarioCharacterKillMode(work->boards[index], 1);
                if (work->hundreds[index])
                    FTBL.SetScenarioCharacterKillMode(work->hundreds[index], 1);
                if (work->tens[index])
                    FTBL.SetScenarioCharacterKillMode(work->tens[index], 1);
                if (work->ones[index])
                    FTBL.SetScenarioCharacterKillMode(work->ones[index], 1);
            }
            if (work->flags & 4) {
                FTBL.EV_EnableCharacterDisplay(RO10_RYO);
                FTBL.EV_EnableCharacterDisplay(work->owner);
            }
            SetAreaLoopCharacterControl(work->owner, 0);
            names[0] = work->owner;
            names[1] = 0;
            RestoreAreaLoopCharacters(names, names);
            FTBL.EV_NPC_NewLoadSw_0((work->flags >> 16) & 1);
            if ((work->flags & 2) && work->audio[0]) {
                FTBL.EV_FreeAuthorPak(0, work->audio);
                work->flags &= ~2;
            }
            if (work->package[0] && FTBL.EV_ControlPackageResources(0, 5, work->package, 0, 0)) {
                FTBL.EV_ControlPackageResources(0, 6, work->package, 0, 0);
                work->package[0] = 0;
            }
            shop = FindRo10Shop(work->shop);
            if (shop) {
                if (work->flags & 0x10) {
                    if (shop->purchasedFlag)
                        FTBL.EV_SetPlayerFlag(12, shop->purchasedFlag, 1);
                } else if (shop->declinedFlag)
                    FTBL.EV_SetPlayerFlag(12, shop->declinedFlag, 1);
            }
            alternate = work->flags & 0x20000;
            FTBL.EV_EventSetUpHelpIcon_0(0, 0);
        } else
            parent = 0;
        FTBL.camCtrlSetState(0);
        FTBL.camCtrlSetFovTransitionTo(1);
        SetAreaLoopCharacterControl(RO10_RYO, 0);
        EventDebugUnregister(RO10_DEBUG_ID);
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        QueueAreaLoopMotion(RO10_RYO, 0xf002, 0, -1, -1.0f, -1.0f, 1.0f, 0.0f, 0, 2);
        do {
            if (alternate && (task = FTBL.HLib_EnqueueTaskWithParameter(ExitAfterFade, 4, 11, sizeof(Ro10ExitWork), TASK_EVNT)) != 0) {
                exitWork = (Ro10ExitWork *)FTBL.HLib_GetTaskParameter(task);
                exitWork->parent = parent;
                exitWork->owner = FTBL.HLib_GetCurrentTask();
                FTBL.HLib_SetupTaskDelay(-1);
                FTBL.SetScreenFilmFade(16, 255, 0, 0, 0, 0, 0, 0, 0);
            } else {
                FTBL.HLib_MarkCurrentTaskForCleanup();
                if (parent)
                    FTBL.HLib_ResolveDeferredCallback(parent);
                FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
                {
                    void (*enable)(int) = FTBL.SetPlayerControlEnabled;
                    SHENMUE_EVENT_USE(FTBL.SetPlayerControlEnabled);
                    enable(1);
                }
            }
        } while (0);
        break;
    }
}

void ExitAfterFade(void *data)
{
    Ro10ExitWork *work = (Ro10ExitWork *)data;
    if (!FTBL.filmCheckFadeTrackColour()) {
        FTBL.EV_HLKillOtherHierarchyTask(work->owner);
        FTBL.HLib_ResolveDeferredCallback(work->parent);
        FTBL.HLib_DestroyCurrentEventTask();
        FTBL.SetPlayerControlEnabled(1);
    }
}

void WaitShopReady(void *data)
{
    Ro10Work *work = (Ro10Work *)data;
    while (work && !FTBL.filmCheckFadeTrackColour()) {
        if (work->flags & 0x20000) {
            FTBL.InitiateCleanupForSceneChara(work->owner);
            if (!(work->flags & 1)) {
                FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
                work->flags |= 1;
                break;
            }
        }
        FTBL.HLib_SetCurrentTaskTo(WaitShopPackage);
        FTBL.HLib_SetupTaskDelay(-1);
        FTBL.EV_NPC_NewLoadSw_0(0);
        FTBL.EV_ResetFreeTalkFlags();
        QueueAreaLoopPackage(FTBL.Filepath_Gen(0), work->package, ResolveAreaLoopParent);
        break;
    }
}

void WaitShopPackage(void *data)
{
    Ro10Work *work = (Ro10Work *)data;
    Vector3 position;
    Vector3i rotation;
    shen_name names[2];
    do {
        if (!work)
            break;
        FTBL.HLib_SetCurrentTaskTo(WaitShopOwner);
        FTBL.HLib_SetupTaskDelay(-1);
        FTBL.EV_ControlPackageResources(2, 2, work->package, 0, 0);
        FTBL.EV_ControlPackageResources(2, 4, work->package, 0, 0);
        CreateRo10PriceBoards(work->definition, work->boards, work->hundreds, work->tens, work->ones);
        GetRo10ItemTransform(work->definition, 0, 0, &position, 0, 0, 0);
        FTBL.EV_AT_SetPos(RO10_BASE, 0x78000000, &position);
        rotation.x = 0;
        rotation.y = 0;
        rotation.z = 0;
        FTBL.EV_AT_SetAngY_0(RO10_BASE, 0x78000000, &rotation, 0);
        FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(RO10_HAND));
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(RO10_HAND), 1, 0, 0);
        names[0] = work->owner;
        names[1] = 0;
        QueueAreaLoopNpcs(names, ResolveAreaLoopParent);
    } while (0);
}

void WaitShopOwner(void *data)
{
    Ro10Work *work = (Ro10Work *)data;
    int alternate;
    int delay = 0;
    Vector3 position;
    Vector3i rotation;
    do {
    if (!work)
        break;
    alternate = work->flags & 0x20000;
    EventDebugGetState(RO10_DEBUG_ID, 1);
    EventDebugCheck(RO10_DEBUG_ID, 4);
    FTBL.HLib_SetCurrentTaskTo(Ro10EnterItems);
    if (!alternate)
        FTBL.HLib_SetupTaskDelay(-1);
    SetAreaLoopCharacterControl(RO10_RYO, 1);
    SetAreaLoopCharacterControl(work->owner, 1);
    if (work->definition && work->definition->prop)
        FTBL.SetDisplayFlagForProp(work->definition->prop, 0);
    if (work->audio[0]) {
        FTBL.EV_FreeAuthorPak(1, work->audio);
        work->flags |= 2;
    }
    QueueAreaLoopMotion(RO10_RYO, 11462, 0, -1, -1.0f, -1.0f, 1.0f, 8.0f, 0, 18);
    QueueAreaLoopMotion(work->owner, work->definition->ownerMotion, 0, -1, -1.0f, -1.0f, 1.0f, 8.0f, 0, 18);
    rotation.x = 0;
    rotation.z = 0;
    FTBL.GetCharaPosition(RO10_RYO, -1, &position, 0x40000000);
    position.x = work->definition->playerX;
    position.z = work->definition->playerZ;
    position.y += 0.5f;
    position.y = FTBL.shenFindIntersectionWithScene(&position);
    rotation.y = work->definition->playerYaw;
    QueueAreaLoopPosition(RO10_RYO, 0, 0, &position, &rotation, 0);
    position.x = work->definition->ownerX;
    position.z = work->definition->ownerZ;
    rotation.y = work->definition->ownerYaw;
    QueueAreaLoopPosition(work->owner, 0, 0, &position, &rotation, 0);
    RefreshRo10Items(work->definition, work->purchased, work->boards, work->hundreds, work->tens, work->ones);
    if (alternate)
        break;
    switch (work->shop) {
    case RO10_ARDX:
        work->column = 1;
        work->row = 0;
        switch (FTBL.shenGetRandomInt32(3)) {
        case 0:
            QueueAreaLoopVoice("0110D005", 1, 1, -1, 0);
            delay = 45;
            break;
        case 1:
            QueueAreaLoopVoice("0110D006", 1, 1, -1, 0);
            delay = 50;
            break;
        case 2:
            QueueAreaLoopVoice("0110D007", 1, 1, -1, 0);
            delay = 70;
            break;
        }
        switch (FTBL.shenGetRandomInt32(3)) {
        case 0:
            QueueAreaLoopVoice("0110D008", 1, delay, -1, ResolveAreaLoopParent);
            break;
        case 1:
            QueueAreaLoopVoice("0110D009", 1, delay, -1, ResolveAreaLoopParent);
            break;
        case 2:
            QueueAreaLoopVoice("0110D010", 1, delay, -1, ResolveAreaLoopParent);
            break;
        }
        QueueAreaLoopCamera(work->package, 1500, RO10_BASE, 1, 0);
        break;
    case RO10_ARX2:
        work->column = 1;
        work->row = 0;
        QueueAreaLoopVoice("0110F003", 1, 1, -1, ResolveAreaLoopParent);
        QueueAreaLoopCamera(work->package, 1500, RO10_BASE, 1, 0);
        break;
    case RO10_ARXX:
        work->column = 0;
        work->row = 0;
        switch (FTBL.shenGetRandomInt32(3)) {
        case 0:
            QueueAreaLoopVoice("0110C004", 1, 1, -1, ResolveAreaLoopParent);
            break;
        case 1:
            QueueAreaLoopVoice("0110C005", 1, 1, -1, ResolveAreaLoopParent);
            break;
        case 2:
            QueueAreaLoopVoice("0110C006", 1, 1, -1, ResolveAreaLoopParent);
            break;
        }
        QueueAreaLoopCamera(work->package, 1500, RO10_BASE, 1, 0);
        break;
    case RO10_ARSF:
        work->column = 0;
        work->row = 0;
        QueueAreaLoopVoice("0110B004", 1, 1, -1, 0);
        QueueAreaLoopVoice("0110B005", 1, 40, -1, ResolveAreaLoopParent);
        QueueAreaLoopCamera(work->package, 1500, RO10_BASE, 1, 0);
        break;
    case RO10_AKX7:
        work->column = 0;
        work->row = 0;
        switch (FTBL.shenGetRandomInt32(3)) {
        case 0:
            QueueAreaLoopVoice("0110E004", 1, 1, -1, 0);
            break;
        case 1:
            QueueAreaLoopVoice("0110E005", 1, 1, -1, 0);
            break;
        case 2:
            QueueAreaLoopVoice("0110E006", 1, 1, -1, 0);
            break;
        }
        QueueAreaLoopVoice("0110E007", 1, 60, -1, ResolveAreaLoopParent);
        QueueAreaLoopCamera(work->package, 1500, RO10_BASE, 1, 0);
        break;
    case RO10_QR00:
        work->column = 0;
        work->row = 0;
        QueueAreaLoopVoice("0110L004", 1, 1, -1, 0);
        QueueAreaLoopVoice("0110L005", 1, 40, -1, ResolveAreaLoopParent);
        QueueAreaLoopCamera(work->package, 1500, RO10_BASE, 1, 0);
        break;
    default:
        FTBL.HLib_SetupTaskDelay(1);
        break;
    }
    FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    work->flags &= ~1;
    } while (0);
}

void Ro10EnterItems(void *data)
{
    Ro10Work *work = (Ro10Work *)data;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(Ro10Quit);
            break;
        }
        if (FTBL.filmCheckFadeTrackColour())
            break;
        if (work->flags & 1) {
            FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
            work->flags &= ~1;
        }
        RefreshRo10Items(work->definition, work->purchased, work->boards, work->hundreds, work->tens, work->ones);
        FTBL.EV_DisableCharacterDisplay(RO10_RYO);
        FTBL.EV_DisableCharacterDisplay(work->owner);
        work->flags |= 4;
        work->cameraFrames = 0;
        work->cameraDistance = 0.5f;
        UpdateRo10ItemCamera(work->definition, work->column, work->row, 0.5f, 0);
        FTBL.HLib_SetCurrentTaskTo(ShopMain);
        break;
    }
}

void ShopMain(void *data)
{
    Ro10Work *work = (Ro10Work *)data;
    const Ro10Shop *shop;
    unsigned int input;
    int moved, previous, price, dialogue, delay;
    float distance;
    for (;;) {
        if (FTBL.shenGetMinutesUntilNextFrameMinute() <= 1) {
            FTBL.HLib_SetCurrentTaskTo(Ro10Quit);
            break;
        }
        if (!work)
            break;
        shop = FindRo10Shop(work->shop);
        if (!shop)
            break;
        if (++work->frames > 10800) {
            FTBL.HLib_SetCurrentTaskTo(Ro10Quit);
            break;
        }
        input = FTBL.GetCurrentControlInput(2);
        moved = 1;
        if (input & 0x8000) {
            if (++work->column >= shop->columns) {
                moved = 0;
                work->column = shop->columns - 1;
            } else
                work->cameraFrames = 8;
        } else if (input & 0x4000) {
            if (--work->column < 0) {
                moved = 0;
                work->column = 0;
            } else
                work->cameraFrames = 8;
        } else if (input & 0x1000) {
            if (--work->row < 0) {
                moved = 0;
                work->row = 0;
            } else
                work->cameraFrames = 8;
        } else if (input & 0x2000) {
            if (++work->row >= shop->rows) {
                moved = 0;
                work->row = shop->rows - 1;
            } else
                work->cameraFrames = 8;
        } else
            moved = 0;
        distance = (float)(int)(FTBL.GetCurrentControlInput(4) - FTBL.GetCurrentControlInput(3));
        RO10_FLUSH(distance);
        distance *= 0.00390625f;
        RO10_FLUSH(distance);
        distance *= 0.2f;
        RO10_FLUSH(distance);
        distance += 0.5f;
        RO10_FLUSH(distance);
        distance *= 0.2f;
        RO10_FLUSH(distance);
        work->cameraDistance *= 0.8f;
        RO10_FLUSH(work->cameraDistance);
        work->cameraDistance = distance + work->cameraDistance;
        RO10_FLUSH(work->cameraDistance);
        UpdateRo10ItemCamera(work->definition, work->column, work->row, work->cameraDistance, work->cameraFrames);
        previous = work->cameraFrames;
        if (--work->cameraFrames < 0)
            work->cameraFrames = 0;
        price = work->definition->prices[work->column + work->row * work->definition->columns];
        dialogue = work->definition->dialogues[work->column + work->row * work->definition->columns];
        if (work->purchased[work->column + work->row * work->definition->columns] & 1) {
            price = -1;
            dialogue = 0;
        }
        if (!work->cameraFrames) {
            FTBL.GetBattleDataConfig(0, RO10_CANCEL);
            if (price > 0)
                FTBL.GetBattleDataConfig(0, RO10_SELECT);
            if (dialogue > 0) {
                if (previous)
                    FTBL.Loop3DSoundEffect(171);
                FTBL.GetBattleDataConfig(0, RO10_DESCRIBE);
            }
        }
        if (moved || work->cameraFrames)
            break;
        FTBL.CheckHelpIcon(33, 0, 3);
        if (FTBL.GetBattleDataConfig(1, RO10_CANCEL) && FTBL.GetBattleDataConfig(2, RO10_CANCEL)) {
            FTBL.HLib_SetCurrentTaskTo(Ro10Quit);
            break;
        }
        if (FTBL.GetBattleDataConfig(1, RO10_SELECT) && FTBL.GetBattleDataConfig(2, RO10_SELECT)) {
            FTBL.HLib_SetCurrentTaskTo(ShopSelect);
            break;
        }
        if (FTBL.GetBattleDataConfig(1, RO10_DESCRIBE) && FTBL.GetBattleDataConfig(2, RO10_DESCRIBE) &&
            work->definition->dialogues[work->column + work->row * work->definition->columns] > 0) {
            FTBL.HLib_SetupTaskDelay(-1);
            delay = 1;
            QueueAreaLoopVoice("0110A003", 1, delay, -1, 0);
            delay = 35;
            switch (work->definition->dialogues[work->column + work->row * work->definition->columns]) {
            case 1:
                switch (FTBL.shenGetRandomInt32(3)) {
                case 0:
                    QueueAreaLoopVoice("0110F004", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 1:
                    QueueAreaLoopVoice("0110F005", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 2:
                    QueueAreaLoopVoice("0110F005", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                }
                break;
            case 2:
                switch (FTBL.shenGetRandomInt32(3)) {
                case 0:
                    QueueAreaLoopVoice("0110F007", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 1:
                    QueueAreaLoopVoice("0110F008", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 2:
                    QueueAreaLoopVoice("0110F009", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                }
                break;
            case 3:
                switch (FTBL.shenGetRandomInt32(3)) {
                case 0:
                    QueueAreaLoopVoice("0110F010", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 1:
                    QueueAreaLoopVoice("0110F011", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 2:
                    QueueAreaLoopVoice("0110F012", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                }
                break;
            case 4:
                switch (FTBL.shenGetRandomInt32(3)) {
                case 0:
                    QueueAreaLoopVoice("0110F013", 1, delay, -1, 0);
                    delay += 60;
                    QueueAreaLoopVoice("0110F014", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 1:
                    QueueAreaLoopVoice("0110F015", 1, delay, -1, 0);
                    delay += 50;
                    QueueAreaLoopVoice("0110F016", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 2:
                    QueueAreaLoopVoice("0110F017", 1, delay, -1, 0);
                    delay += 70;
                    QueueAreaLoopVoice("0110F018", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                }
                break;
            case 10:
                switch (FTBL.shenGetRandomInt32(3)) {
                case 0:
                    QueueAreaLoopVoice("0110L006", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 1:
                    QueueAreaLoopVoice("0110L007", 1, delay, -1, 0);
                    delay += 60;
                    QueueAreaLoopVoice("0110L008", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 2:
                    QueueAreaLoopVoice("0110L009", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                }
                break;
            case 11:
                switch (FTBL.shenGetRandomInt32(3)) {
                case 0:
                    QueueAreaLoopVoice("0110L010", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 1:
                    QueueAreaLoopVoice("0110L011", 1, delay, -1, 0);
                    delay += 40;
                    QueueAreaLoopVoice("0110L012", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 2:
                    QueueAreaLoopVoice("0110L013", 1, delay, -1, 0);
                    delay += 65;
                    QueueAreaLoopVoice("0110L014", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                }
                break;
            case 12:
                switch (FTBL.shenGetRandomInt32(3)) {
                case 0:
                    QueueAreaLoopVoice("0110L015", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 1:
                    QueueAreaLoopVoice("0110L016", 1, delay, -1, 0);
                    delay += 100;
                    QueueAreaLoopVoice("0110L017", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 2:
                    QueueAreaLoopVoice("0110L018", 1, delay, -1, 0);
                    delay += 70;
                    QueueAreaLoopVoice("0110L019", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                }
                break;
            case 13:
                switch (FTBL.shenGetRandomInt32(3)) {
                case 0:
                    QueueAreaLoopVoice("0110L020", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 1:
                    QueueAreaLoopVoice("0110L021", 1, delay, -1, 0);
                    delay += 40;
                    QueueAreaLoopVoice("0110L022", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 2:
                    QueueAreaLoopVoice("0110L023", 1, delay, -1, 0);
                    delay += 100;
                    QueueAreaLoopVoice("0110L024", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                }
                break;
            case 14:
                switch (FTBL.shenGetRandomInt32(3)) {
                case 0:
                    QueueAreaLoopVoice("0110L025", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 1:
                    QueueAreaLoopVoice("0110L026", 1, delay, -1, 0);
                    delay += 80;
                    QueueAreaLoopVoice("0110L027", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 2:
                    QueueAreaLoopVoice("0110L028", 1, delay, -1, 0);
                    delay += 70;
                    QueueAreaLoopVoice("0110L029", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                }
                break;
            case 20:
                switch (FTBL.shenGetRandomInt32(3)) {
                case 0:
                    QueueAreaLoopVoice("0110B006", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 1:
                    QueueAreaLoopVoice("0110B007", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 2:
                    QueueAreaLoopVoice("0110B008", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                }
                break;
            case 21:
                switch (FTBL.shenGetRandomInt32(3)) {
                case 0:
                    QueueAreaLoopVoice("0110B009", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 1:
                    QueueAreaLoopVoice("0110B010", 1, delay, -1, 0);
                    delay += 60;
                    QueueAreaLoopVoice("0110B011", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 2:
                    QueueAreaLoopVoice("0110B012", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                }
                break;
            case 22:
                switch (FTBL.shenGetRandomInt32(3)) {
                case 0:
                    QueueAreaLoopVoice("0110B013", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 1:
                    QueueAreaLoopVoice("0110B014", 1, delay, -1, 0);
                    delay += 100;
                    QueueAreaLoopVoice("0110B015", 1, delay, -1, 0);
                    delay += 100;
                    QueueAreaLoopVoice("0110B016", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 2:
                    QueueAreaLoopVoice("0110B017", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                }
                break;
            case 23:
                switch (FTBL.shenGetRandomInt32(3)) {
                case 0:
                    QueueAreaLoopVoice("0110B018", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 1:
                    QueueAreaLoopVoice("0110B019", 1, delay, -1, 0);
                    delay += 100;
                    QueueAreaLoopVoice("0110B020", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 2:
                    QueueAreaLoopVoice("0110B021", 1, delay, -1, 0);
                    delay += 70;
                    QueueAreaLoopVoice("0110B022", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                }
                break;
            case 30:
                switch (FTBL.shenGetRandomInt32(5)) {
                case 0:
                    QueueAreaLoopVoice("0110C007", 1, delay, -1, 0);
                    delay += 60;
                    break;
                case 1:
                    QueueAreaLoopVoice("0110C008", 1, delay, -1, 0);
                    delay += 100;
                    break;
                case 2:
                    QueueAreaLoopVoice("0110C009", 1, delay, -1, 0);
                    delay += 135;
                    break;
                case 3:
                    QueueAreaLoopVoice("0110C010", 1, delay, -1, 0);
                    delay += 130;
                    break;
                case 4:
                    QueueAreaLoopVoice("0110C011", 1, delay, -1, 0);
                    delay += 145;
                    break;
                }
                switch (FTBL.shenGetRandomInt32(5)) {
                case 0:
                    QueueAreaLoopVoice("0110C012", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 1:
                    QueueAreaLoopVoice("0110C013", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 2:
                    QueueAreaLoopVoice("0110C014", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 3:
                    QueueAreaLoopVoice("0110C015", 1, delay, -1, 0);
                    delay += 70;
                    QueueAreaLoopVoice("0110C016", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 4:
                    QueueAreaLoopVoice("0110C017", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                }
                break;
            case 40:
                switch (FTBL.shenGetRandomInt32(3)) {
                case 0:
                    QueueAreaLoopVoice("0110D011", 1, delay, -1, 0);
                    delay += 60;
                    break;
                case 1:
                    QueueAreaLoopVoice("0110D012", 1, delay, -1, 0);
                    delay += 70;
                    break;
                case 2:
                    QueueAreaLoopVoice("0110D013", 1, delay, -1, 0);
                    delay += 80;
                    break;
                }
                switch (FTBL.shenGetRandomInt32(3)) {
                case 0:
                    QueueAreaLoopVoice("0110D014", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 1:
                    QueueAreaLoopVoice("0110D015", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 2:
                    QueueAreaLoopVoice("0110D016", 1, delay, 100, ResolveAreaLoopParent);
                    break;
                }
                break;
            case 41:
                switch (FTBL.shenGetRandomInt32(3)) {
                case 0:
                    QueueAreaLoopVoice("0110D011", 1, delay, -1, 0);
                    delay += 60;
                    break;
                case 1:
                    QueueAreaLoopVoice("0110D012", 1, delay, -1, 0);
                    delay += 70;
                    break;
                case 2:
                    QueueAreaLoopVoice("0110D013", 1, delay, -1, 0);
                    delay += 80;
                    break;
                }
                switch (FTBL.shenGetRandomInt32(3)) {
                case 0:
                    QueueAreaLoopVoice("0110D017", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 1:
                    QueueAreaLoopVoice("0110D018", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 2:
                    QueueAreaLoopVoice("0110D019", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                }
                break;
            case 42:
                switch (FTBL.shenGetRandomInt32(3)) {
                case 0:
                    QueueAreaLoopVoice("0110D011", 1, delay, -1, 0);
                    delay += 60;
                    break;
                case 1:
                    QueueAreaLoopVoice("0110D012", 1, delay, -1, 0);
                    delay += 70;
                    break;
                case 2:
                    QueueAreaLoopVoice("0110D013", 1, delay, -1, 0);
                    delay += 80;
                    break;
                }
                switch (FTBL.shenGetRandomInt32(3)) {
                case 0:
                    QueueAreaLoopVoice("0110D020", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 1:
                    QueueAreaLoopVoice("0110D021", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 2:
                    QueueAreaLoopVoice("0110D022", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                }
                break;
            case 43:
                switch (FTBL.shenGetRandomInt32(3)) {
                case 0:
                    QueueAreaLoopVoice("0110D011", 1, delay, -1, 0);
                    delay += 60;
                    break;
                case 1:
                    QueueAreaLoopVoice("0110D012", 1, delay, -1, 0);
                    delay += 70;
                    break;
                case 2:
                    QueueAreaLoopVoice("0110D013", 1, delay, -1, 0);
                    delay += 80;
                    break;
                }
                switch (FTBL.shenGetRandomInt32(3)) {
                case 0:
                    QueueAreaLoopVoice("0110D023", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 1:
                    QueueAreaLoopVoice("0110D015", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 2:
                    QueueAreaLoopVoice("0110D024", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                }
                break;
            case 50:
                switch (FTBL.shenGetRandomInt32(3)) {
                case 0:
                    QueueAreaLoopVoice("0110E008", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 1:
                    QueueAreaLoopVoice("0110E009", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 2:
                    QueueAreaLoopVoice("0110E010", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                }
                break;
            case 51:
                switch (FTBL.shenGetRandomInt32(3)) {
                case 0:
                    QueueAreaLoopVoice("0110E011", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 1:
                    QueueAreaLoopVoice("0110E012", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 2:
                    QueueAreaLoopVoice("0110E013", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                }
                break;
            case 52:
                QueueAreaLoopVoice("0110E014", 1, delay, -1, 0);
                delay += 80;
                switch (FTBL.shenGetRandomInt32(2)) {
                case 0:
                    QueueAreaLoopVoice("0110E015", 1, delay, -1, 0);
                    delay += 90;
                    break;
                case 1:
                    QueueAreaLoopVoice("0110E016", 1, delay, -1, 0);
                    delay += 100;
                    break;
                }
                QueueAreaLoopVoice("0110A005", 1, delay, -1, 0);
                delay += 50;
                QueueAreaLoopVoice("0110E017", 1, delay, -1, ResolveAreaLoopParent);
                break;
            case 53:
                switch (FTBL.shenGetRandomInt32(3)) {
                case 0:
                    QueueAreaLoopVoice("0110E018", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 1:
                    QueueAreaLoopVoice("0110E019", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 2:
                    QueueAreaLoopVoice("0110E020", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                }
                break;
            case 54:
                switch (FTBL.shenGetRandomInt32(3)) {
                case 0:
                    QueueAreaLoopVoice("0110E021", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 1:
                    QueueAreaLoopVoice("0110E022", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 2:
                    QueueAreaLoopVoice("0110E023", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                }
                break;
            case 55:
                switch (FTBL.shenGetRandomInt32(3)) {
                case 0:
                    QueueAreaLoopVoice("0110E018", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 1:
                    QueueAreaLoopVoice("0110E019", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case 2:
                    QueueAreaLoopVoice("0110E024", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                }
                break;
            default:
                FTBL.HLib_SetupTaskDelay(-1);
                break;
            }
        }
        break;
    }
}

void ShopSelect(void *data)
{
    Ro10Work *work = (Ro10Work *)data;
    int delay;
    if (work) {
        FTBL.HLib_SetCurrentTaskTo(ShopSelectionIcon);
        FTBL.HLib_SetupTaskDelay(40);
        delay = 1;
        switch (work->shop) {
        case RO10_ARDX:
            switch (FTBL.shenGetRandomInt32(3)) {
            case 0:
                QueueAreaLoopVoice("0110D011", 1, delay, -1, 0);
                break;
            case 1:
                QueueAreaLoopVoice("0110D012", 1, delay, -1, 0);
                break;
            case 2:
                QueueAreaLoopVoice("0110D013", 1, delay, -1, 0);
                break;
            }
            break;
        case RO10_ARX2:
            switch (FTBL.shenGetRandomInt32(3)) {
            case 0:
                QueueAreaLoopVoice("0110F019", 1, delay, -1, 0);
                break;
            case 1:
                QueueAreaLoopVoice("0110F020", 1, delay, -1, 0);
                break;
            case 2:
                QueueAreaLoopVoice("0110F021", 1, delay, -1, 0);
                break;
            }
            break;
        case RO10_ARXX:
            switch (FTBL.shenGetRandomInt32(3)) {
            case 0:
                QueueAreaLoopVoice("0110C018", 1, delay, -1, 0);
                break;
            case 1:
                QueueAreaLoopVoice("0110C019", 1, delay, -1, 0);
                break;
            case 2:
                QueueAreaLoopVoice("0110C020", 1, delay, -1, 0);
                break;
            }
            break;
        case RO10_ARSF:
            QueueAreaLoopVoice("0110B023", 1, delay, -1, 0);
            break;
        case RO10_AKX7:
            switch (FTBL.shenGetRandomInt32(3)) {
            case 0:
                QueueAreaLoopVoice("0110E025", 1, delay, -1, 0);
                break;
            case 1:
                QueueAreaLoopVoice("0110E026", 1, delay, -1, 0);
                break;
            case 2:
                QueueAreaLoopVoice("0110E027", 1, delay, -1, 0);
                break;
            }
            break;
        case RO10_QR00:
            QueueAreaLoopVoice("0110L030", 1, delay, -1, 0);
            break;
        }
    } else
        FTBL.HLib_SetCurrentTaskTo(Ro10Quit);
}

void ShopSelectionIcon(void *data)
{
    short choices[4];
    if (data) {
        choices[0] = -1;
        choices[1] = 80;
        choices[2] = 154;
        choices[3] = -1;
        FTBL.SetupControlSpeakAndUI(choices, 10800, -1, 2);
        FTBL.HLib_SetCurrentTaskTo(ShopChoice);
    } else
        FTBL.HLib_SetCurrentTaskTo(Ro10Quit);
}

void ShopChoice(void *data)
{
    Ro10Work *work = (Ro10Work *)data;
    int delay;
    if (work) {
        if (!FTBL.GetShouldRenderIcons(2)) {
            if (FTBL.shenGetCtrlSpeakIconFlag() == 80) {
                delay = 1;
                if (FTBL.GetPlayerStat(2) >= work->definition->prices[work->column + work->row * work->definition->columns]) {
                work->purchased[work->column + work->row * work->definition->columns] |= 1;
                FTBL.HLib_SetCurrentTaskTo(ShopAward);
                FTBL.HLib_SetupTaskDelay(-1);
                QueueAreaLoopVoice("0110A001", 1, delay, -1, ResolveAreaLoopParent);
            } else {
                FTBL.HLib_SetCurrentTaskTo(ShopMain);
                FTBL.HLib_SetupTaskDelay(-1);
                switch (work->shop) {
                case RO10_ARDX:
                    QueueAreaLoopVoice("0110A004", 1, delay, -1, 0);
                    delay += 70;
                    QueueAreaLoopVoice("0110D034", 1, delay, -1, 0);
                    delay += 40;
                    switch (FTBL.shenGetRandomInt32(3)) {
                    case 0:
                        QueueAreaLoopVoice("0110D035", 1, delay, -1, ResolveAreaLoopParent);
                        break;
                    case 1:
                        QueueAreaLoopVoice("0110D036", 1, delay, -1, ResolveAreaLoopParent);
                        break;
                    case 2:
                        QueueAreaLoopVoice("0110D037", 1, delay, -1, ResolveAreaLoopParent);
                        break;
                    }
                    break;
                case RO10_ARX2:
                    QueueAreaLoopVoice("0110A004", 1, delay, -1, 0);
                    delay += 70;
                    QueueAreaLoopVoice("0110A008", 1, delay, -1, 0);
                    delay += 40;
                    QueueAreaLoopVoice("0110F023", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case RO10_ARXX:
                    QueueAreaLoopVoice("0110A004", 1, delay, -1, 0);
                    delay += 60;
                    QueueAreaLoopVoice("0110C024", 1, delay, -1, 0);
                    delay += 40;
                    switch (FTBL.shenGetRandomInt32(3)) {
                    case 0:
                        QueueAreaLoopVoice("0110C025", 1, delay, -1, ResolveAreaLoopParent);
                        break;
                    case 1:
                        QueueAreaLoopVoice("0110C026", 1, delay, -1, ResolveAreaLoopParent);
                        break;
                    case 2:
                        QueueAreaLoopVoice("0110C027", 1, delay, -1, ResolveAreaLoopParent);
                        break;
                    }
                    break;
                case RO10_ARSF:
                    QueueAreaLoopVoice("0110A004", 1, delay, -1, 0);
                    delay += 60;
                    QueueAreaLoopVoice("0110B025", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case RO10_AKX7:
                    QueueAreaLoopVoice("0110A004", 1, delay, -1, 0);
                    delay += 60;
                    QueueAreaLoopVoice("0110E029", 1, delay, -1, ResolveAreaLoopParent);
                    break;
                case RO10_QR00:
                    QueueAreaLoopVoice("0110A004", 1, delay, -1, 0);
                    delay += 60;
                    QueueAreaLoopVoice("0110L032", 1, delay, 100, ResolveAreaLoopParent);
                    break;
                }
                }
            } else
                FTBL.HLib_SetCurrentTaskTo(ShopDeclined);
        } else if (__builtin_fabs(work->cameraDistance - 0.5f) >= RO10_EPSILON) {
            RO10_FLUSH(work->cameraDistance);
            work->cameraDistance *= 0.8f;
            RO10_FLUSH(work->cameraDistance);
            work->cameraDistance += 0.1f;
            RO10_FLUSH(work->cameraDistance);
            UpdateRo10ItemCamera(work->definition, work->column, work->row, work->cameraDistance, 0);
        }
    } else
        FTBL.HLib_SetCurrentTaskTo(Ro10Quit);
}

void ShopDeclined(void *data)
{
    if (data) {
        FTBL.HLib_SetCurrentTaskTo(ShopMain);
        FTBL.HLib_SetupTaskDelay(-1);
        QueueAreaLoopVoice("0110A002", 1, 1, -1, ResolveAreaLoopParent);
    } else
        FTBL.HLib_SetCurrentTaskTo(Ro10Quit);
}

void ShopAward(void *data)
{
    Ro10Work *work = (Ro10Work *)data;
    const Ro10Shop *shop;
    int grip, item, category, count, chance, value, index;
    for (;;) {
        if (!work || !work->definition) {
            FTBL.HLib_SetCurrentTaskTo(Ro10Quit);
            break;
        }
        shop = work->definition;
        grip = shop->grips[work->column + work->row * shop->columns];
        if ((unsigned int)(grip - 1) > 2) {
            FTBL.DebugLogToFileRemoved("illigal type param\n");
            FTBL.HLib_SetCurrentTaskTo(Ro10Quit);
            break;
        }
        work->handFrame = 0;
        work->grip = grip;
        item = shop->items[work->column + work->row * shop->columns];
        if (item < 0) {
            category = item;
            count = 0;
            do {
                chance = FTBL.shenGetRandomInt32(100);
                switch (category) {
                case -1:
                    if (chance < 20)
                        value = ro10Prizes1[FTBL.shenGetRandomInt32(9)];
                    else if (chance < 22)
                        value = ro10Prizes1[FTBL.shenGetRandomInt32(2) + 9];
                    else if (chance < 90)
                        value = ro10Prizes1[FTBL.shenGetRandomInt32(10) + 11];
                    else
                        value = ro10Prizes1[FTBL.shenGetRandomInt32(3) + 21];
                    break;
                case -2:
                    if (chance < 20)
                        value = ro10Prizes2[FTBL.shenGetRandomInt32(8)];
                    else if (chance < 22)
                        value = ro10Prizes2[FTBL.shenGetRandomInt32(2) + 8];
                    else if (chance < 90)
                        value = ro10Prizes2[FTBL.shenGetRandomInt32(10) + 10];
                    else
                        value = ro10Prizes2[FTBL.shenGetRandomInt32(3) + 20];
                    break;
                case -3:
                    if (chance < 20)
                        value = ro10Prizes3[FTBL.shenGetRandomInt32(9)];
                    else if (chance < 22)
                        value = ro10Prizes3[FTBL.shenGetRandomInt32(2) + 9];
                    else if (chance < 90)
                        value = ro10Prizes3[FTBL.shenGetRandomInt32(10) + 11];
                    else
                        value = ro10Prizes3[FTBL.shenGetRandomInt32(3) + 21];
                    break;
                case -4:
                    if (chance < 20)
                        value = ro10Prizes4[FTBL.shenGetRandomInt32(8)];
                    else if (chance < 22)
                        value = ro10Prizes4[FTBL.shenGetRandomInt32(2) + 8];
                    else if (chance < 90)
                        value = ro10Prizes4[FTBL.shenGetRandomInt32(10) + 10];
                    else
                        value = ro10Prizes4[FTBL.shenGetRandomInt32(3) + 20];
                    break;
                case -5:
                    if (chance < 20)
                        value = ro10Prizes5[FTBL.shenGetRandomInt32(9)];
                    else if (chance < 22)
                        value = ro10Prizes5[FTBL.shenGetRandomInt32(2) + 9];
                    else if (chance < 90)
                        value = ro10Prizes5[FTBL.shenGetRandomInt32(10) + 11];
                    else
                        value = ro10Prizes5[FTBL.shenGetRandomInt32(3) + 21];
                    break;
                case -6:
                    if (chance < 20)
                        value = ro10Prizes6[FTBL.shenGetRandomInt32(8)];
                    else if (chance < 22)
                        value = ro10Prizes6[FTBL.shenGetRandomInt32(2) + 8];
                    else if (chance < 90)
                        value = ro10Prizes6[FTBL.shenGetRandomInt32(8) + 10];
                    else
                        value = ro10Prizes6[FTBL.shenGetRandomInt32(3) + 18];
                    break;
                default:
                    value = 0;
                    break;
                }
                if (value) {
                    for (index = 0; index < count && work->awards[index] != value; index++)
                        ;
                    if (index >= count)
                        work->awards[count++] = value;
                } else {
                    for (; count < 3; count++)
                        work->awards[count] = 0;
                    break;
                }
            } while (count < 3);
        } else {
            work->awards[0] = item;
            work->awards[1] = 0;
            work->awards[2] = 0;
        }
        FTBL.HLib_SetCurrentTaskTo(ShopHand);
        break;
    }
}

void ShopHand(void *data)
{
    Ro10Work *work = (Ro10Work *)data;
    const Ro10Shop *shop;
    float x, y, z, distance;
    Vector3 position, translated;
    Vector3i rotation, handRotation;
    int again, index;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(Ro10Quit);
            break;
        }
        do {
            again = 0;
            shop = work->definition;
            switch (work->grip) {
            case 1:
                x = ro10SmallOffset.x;
                y = ro10SmallOffset.y;
                z = ro10SmallOffset.z;
                break;
            case 2:
                x = ro10TallOffset.x;
                y = ro10TallOffset.y;
                z = ro10TallOffset.z;
                break;
            default:
                x = ro10PacketOffset.x;
                y = ro10PacketOffset.y;
                z = ro10PacketOffset.z;
                break;
            }
            switch (work->handFrame) {
            case 0:
                work->flags |= 0x10;
                if (work->cameraDistance > 0.499f && work->cameraDistance < 0.501f) {
                    work->handFrame = 100;
                    again = 1;
                    FTBL.EV_EnableCharacterDisplay(RO10_HAND);
                    FTBL.EV_EnsureCharacterDraw(RO10_HAND);
                } else {
                    distance = work->cameraDistance;
                    RO10_FLUSH(distance);
                    distance *= 0.8f;
                    RO10_FLUSH(distance);
                    distance += 0.1f;
                    RO10_FLUSH(distance);
                    work->cameraDistance = distance;
                    UpdateRo10ItemCamera(shop, work->column, work->row, distance, 0);
                }
                break;
            case 100 ... 120:
                switch (work->grip) {
                case 1:
                    FTBL.imgmSetHandPos(FTBL.ctrlGetCharaByName(RO10_HAND), 0, 21, &ro10SmallRotation);
                    break;
                case 2:
                    FTBL.imgmSetHandPos(FTBL.ctrlGetCharaByName(RO10_HAND), 0, 21, &ro10TallRotation);
                    break;
                default:
                    FTBL.imgmSetHandPos(FTBL.ctrlGetCharaByName(RO10_HAND), 0, 21, &ro10PacketRotation);
                    break;
                }
                if (work->handFrame == 100) {
                    switch (work->grip) {
                    case 1:
                        FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(RO10_HAND), 0, ro10SmallReach, 1);
                        break;
                    case 2:
                        FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(RO10_HAND), 0, ro10TallReach, 1);
                        break;
                    default:
                        FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(RO10_HAND), 0, ro10PacketReach, 1);
                        break;
                    }
                } else if (work->handFrame == 101) {
                    switch (work->grip) {
                    case 1:
                        FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(RO10_HAND), 0, ro10SmallGrip, 30);
                        break;
                    case 2:
                        FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(RO10_HAND), 0, ro10TallGrip, 30);
                        break;
                    default:
                        FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(RO10_HAND), 0, ro10PacketGrip, 30);
                        break;
                    }
                }
                x *= 0.05f;
                RO10_FLUSH(x);
                x *= (float)(work->handFrame - 100);
                RO10_FLUSH(x);
                y *= 0.05f;
                RO10_FLUSH(y);
                y *= (float)(work->handFrame - 100);
                RO10_FLUSH(y);
                z *= 0.05f;
                RO10_FLUSH(z);
                z *= (float)(work->handFrame - 100);
                RO10_FLUSH(z);
                GetRo10ItemTransform(shop, work->column, work->row, &position, &rotation, 0, 0);
                translated.x = (float)(120 - work->handFrame) * 0.01f;
                RO10_FLUSH(translated.x);
                translated.x += 0.07f;
                RO10_FLUSH(translated.x);
                translated.x += x;
                RO10_FLUSH(translated.x);
                translated.y = 0.6f + y;
                RO10_FLUSH(translated.y);
                translated.z = 0.16f + z;
                RO10_FLUSH(translated.z);
                handRotation.x = 0x8000;
                handRotation.y = 27306 - shop->rotation.y;
                handRotation.z = 0x8000;
                rotation.x = shop->rotation.x;
                rotation.y = shop->rotation.y;
                rotation.z = shop->rotation.z;
                FTBL.ApplyScenarioTranslationAndRotation(&position, &rotation, &translated);
                FTBL.EV_AT_SetPos(RO10_HAND, 0x78000000, &translated);
                FTBL.EV_AT_SetAngY_0(RO10_HAND, 0x78000000, &handRotation, 0);
                work->handFrame++;
                break;
            case 121 ... 130:
                work->handFrame++;
                break;
            case 131:
                work->handFrame = 200;
                break;
            case 200:
                FTBL.AT_SetLocalBaseChara(shop->objects[work->column + work->row * shop->columns], RO10_HAND);
                work->handFrame = 300;
                break;
            case 300 ... 320:
                if (!FTBL.GetShouldRenderIcons(5)) {
                    for (index = 0; index < 3; index++)
                        if (work->awards[index]) {
                            FTBL.AddInventoryItem(work->awards[index], 1, 1);
                            work->awards[index] = 0;
                            break;
                        }
                }
                x *= 0.05f;
                RO10_FLUSH(x);
                x *= (float)(320 - work->handFrame);
                RO10_FLUSH(x);
                y *= 0.05f;
                RO10_FLUSH(y);
                y *= (float)(320 - work->handFrame);
                RO10_FLUSH(y);
                z *= 0.05f;
                RO10_FLUSH(z);
                z *= (float)(320 - work->handFrame);
                RO10_FLUSH(z);
                GetRo10ItemTransform(shop, work->column, work->row, &position, &rotation, 0, 0);
                translated.x = (float)(work->handFrame - 300) * 0.02f;
                RO10_FLUSH(translated.x);
                translated.x += 0.07f;
                RO10_FLUSH(translated.x);
                translated.x += x;
                RO10_FLUSH(translated.x);
                translated.y = (float)(work->handFrame - 300) * 0.01f;
                RO10_FLUSH(translated.y);
                translated.y += 0.6f;
                RO10_FLUSH(translated.y);
                translated.y += y;
                RO10_FLUSH(translated.y);
                translated.z = 0.16f + z;
                RO10_FLUSH(translated.z);
                handRotation.x = 0x8000;
                handRotation.y = 27306 - shop->rotation.y;
                handRotation.z = 0x8000;
                rotation.x = shop->rotation.x;
                rotation.y = shop->rotation.y;
                rotation.z = shop->rotation.z;
                FTBL.ApplyScenarioTranslationAndRotation(&position, &rotation, &translated);
                FTBL.EV_AT_SetPos(RO10_HAND, 0x78000000, &translated);
                FTBL.EV_AT_SetAngY_0(RO10_HAND, 0x78000000, &handRotation, 0);
                if (++work->handFrame == 321)
                    work->handFrame = 330;
                break;
            case 330:
                if (!FTBL.GetShouldRenderIcons(5)) {
                    for (index = 0; index < 3; index++)
                        if (work->awards[index]) {
                            FTBL.AddInventoryItem(work->awards[index], 1, 1);
                            work->awards[index] = 0;
                            break;
                        }
                    if (index == 3) {
                        work->handFrame = 400;
                        FTBL.AT_SetLocalBaseChara(shop->objects[work->column + work->row * shop->columns], RO10_FREE);
                        FTBL.EV_DisableCharacterDisplay(RO10_HAND);
                        FTBL.EV_DisableCharacterDisplay(shop->objects[work->column + work->row * shop->columns]);
                    }
                }
                break;
            case 400:
                work->handFrame = 500;
                FTBL.HLib_SetupTaskDelay(-1);
                switch (work->shop) {
                case RO10_ARDX:
                    switch (FTBL.shenGetRandomInt32(3)) {
                    case 0:
                        QueueAreaLoopVoice("0110D028", 1, 10, -1, ResolveAreaLoopParent);
                        break;
                    case 1:
                        QueueAreaLoopVoice("0110D029", 1, 10, -1, ResolveAreaLoopParent);
                        break;
                    case 2:
                        QueueAreaLoopVoice("0110D030", 1, 10, -1, ResolveAreaLoopParent);
                        break;
                    }
                    break;
                case RO10_ARX2:
                    QueueAreaLoopVoice("0110F022", 1, 10, -1, ResolveAreaLoopParent);
                    break;
                case RO10_ARXX:
                    QueueAreaLoopVoice("0110C023", 1, 10, -1, ResolveAreaLoopParent);
                    break;
                case RO10_ARSF:
                    QueueAreaLoopVoice("0110B024", 1, 10, -1, ResolveAreaLoopParent);
                    break;
                case RO10_AKX7:
                    QueueAreaLoopVoice("0110E028", 1, 10, -1, ResolveAreaLoopParent);
                    break;
                case RO10_QR00:
                    QueueAreaLoopVoice("0110L031", 1, 10, -1, ResolveAreaLoopParent);
                    break;
                }
                break;
            case 500:
                FTBL.AddThisToCashOrSegaCoins(-shop->prices[work->column + work->row * shop->columns], 2);
                work->handFrame = 600;
                break;
            case 600:
                work->handFrame = 700;
                FTBL.HLib_SetupTaskDelay(-1);
                switch (work->shop) {
                case RO10_ARDX:
                    switch (FTBL.shenGetRandomInt32(3)) {
                    case 0:
                        QueueAreaLoopVoice("0110D031", 1, 10, -1, ResolveAreaLoopParent);
                        break;
                    case 1:
                        QueueAreaLoopVoice("0110D032", 1, 10, -1, ResolveAreaLoopParent);
                        break;
                    case 2:
                        QueueAreaLoopVoice("0110D033", 1, 10, -1, ResolveAreaLoopParent);
                        break;
                    }
                    break;
                case RO10_ARX2:
                    QueueAreaLoopVoice("0110A007", 1, 10, -1, ResolveAreaLoopParent);
                    break;
                case RO10_ARXX:
                    if (FTBL.shenGetRandomInt32(2) == 0)
                        QueueAreaLoopVoice("0110C021", 1, 10, -1, ResolveAreaLoopParent);
                    else
                        QueueAreaLoopVoice("0110C022", 1, 10, -1, ResolveAreaLoopParent);
                    break;
                case RO10_ARSF:
                    FTBL.HLib_SetupTaskDelay(1);
                    break;
                case RO10_AKX7:
                    FTBL.HLib_SetupTaskDelay(1);
                    break;
                case RO10_QR00:
                    FTBL.HLib_SetupTaskDelay(1);
                    break;
                }
                break;
            case 700:
                if (!FTBL.GetShouldRenderIcons(4))
                    FTBL.HLib_SetCurrentTaskTo(Ro10EnterItems);
                break;
            }
        } while (again);
        break;
    }
}

const Ro10Shop *FindRo10Shop(shen_name shop)
{
    const Ro10Shop *result = 0;
    int index;
    for (index = 0; ro10Shops[index].shop; index++) {
        if (ro10Shops[index].shop == shop) {
            result = &ro10Shops[index];
            break;
        }
    }
    return result;
}

void RefreshRo10Items(const Ro10Shop *shop, int *purchased, shen_name *boards, shen_name *hundreds, shen_name *tens, shen_name *ones)
{
    Vector3 position, base, boardPosition;
    Vector3i rotation, baseRotation, boardRotation;
    Vector3 scale;
    shen_name item;
    int index;
    if (!shop || !purchased)
        return;
    for (index = 0; index < 16; index++) {
        item = shop->objects[index];
        if (!item || shop->prices[index] <= 0)
            continue;
        GetRo10ItemTransform(shop, index % 4, index / 4, &base, &baseRotation, 0, 0);
        GetRo10ItemTransform(shop, index % 4, index / 4, &position, &rotation, &scale, 1);
        if (!(purchased[index] & 1)) {
            FTBL.EV_AT_SetPos(item, 0x78000000, &position);
            FTBL.EV_AT_SetAngY_0(item, 0x78000000, &rotation, 0);
            if (!(purchased[index] & 8)) {
                FTBL.mtwkSetMotionworkScale3(item, &scale);
                purchased[index] |= 8;
            }
            FTBL.EV_EnableCharacterDisplay(item);
        } else {
            FTBL.EV_DisableCharacterDisplay(item);
            if (boards[index])
                FTBL.EV_DisableCharacterDisplay(boards[index]);
            if (hundreds[index])
                FTBL.EV_DisableCharacterDisplay(hundreds[index]);
            if (tens[index])
                FTBL.EV_DisableCharacterDisplay(tens[index]);
            if (ones[index])
                FTBL.EV_DisableCharacterDisplay(ones[index]);
        }
        if (boards[index] && ones[index]) {
            boardPosition.x = 0.0f;
            boardPosition.y = -0.07f;
            boardPosition.z = 0.01f;
            position = base;
            FTBL.ApplyScenarioTranslationAndRotation(&position, &baseRotation, &boardPosition);
            boardRotation.x = baseRotation.x - 44055;
            boardRotation.y = baseRotation.y;
            boardRotation.z = baseRotation.z;
            FTBL.EV_AT_SetPos(boards[index], 0x78000000, &boardPosition);
            FTBL.EV_AT_SetAngY_0(boards[index], 0x78000000, &boardRotation, 0);
            FTBL.EV_EnableCharacterDisplay(boards[index]);
            if (hundreds[index])
                FTBL.EV_EnableCharacterDisplay(hundreds[index]);
            if (tens[index])
                FTBL.EV_EnableCharacterDisplay(tens[index]);
            FTBL.EV_EnableCharacterDisplay(ones[index]);
        }
    }
}

int GetRo10ItemTransform(const Ro10Shop *shop, int column, int row, Vector3 *position, Vector3i *rotation, Vector3 *scale, int grip)
{
    Vector3 base, offset;
    Vector3i angle;
    int style;
    int result = 0;
    do {
        if (!shop)
            break;
    while (column < 0)
        column += shop->columns;
    while (column >= shop->columns)
        column -= shop->columns;
    while (row < 0)
        row += shop->rows;
    while (row >= shop->rows)
        row -= shop->rows;
    base.x = shop->position.x;
    base.y = shop->position.y;
    base.z = shop->position.z;
    angle.x = shop->rotation.x;
    angle.y = shop->rotation.y;
    angle.z = shop->rotation.z;
    offset.x = shop->columnSpacing;
    offset.z = shop->rowSpacing;
    offset.y = 0.0f;
    offset.x = (float)column * offset.x;
    offset.z = (float)row * offset.z;
    RO10_FLUSH(offset.x);
    RO10_FLUSH(offset.y);
    FTBL.ApplyScenarioTranslationAndRotation(&base, &angle, &offset);
    if (position)
        *position = offset;
    if (rotation) {
        rotation->x = shop->rotation.x + 0xc000;
        rotation->y = shop->rotation.y;
        rotation->z = shop->rotation.z;
        if (grip) {
            style = shop->grips[column + row * shop->columns];
            if (style > 0) {
                rotation->x += ro10GripRotations[style - 1].x;
                rotation->y += ro10GripRotations[style - 1].y;
                rotation->z += ro10GripRotations[style - 1].z;
            }
        }
    }
    if (scale) {
        style = shop->grips[column + row * shop->columns];
        if (style > 0)
            *scale = ro10GripScales[style - 1];
    }
    result = 1;
    } while (0);
    return result;
}

void UpdateRo10ItemCamera(const Ro10Shop *shop, int column, int row, float distance, int frames)
{
    Vector3 previousTarget, target, previous, position, item;
    Vector3i rotation;
    float blend, delta;
    Vector3 *next;
    if (!GetRo10ItemTransform(shop, column, row, &item, &rotation, 0, 0))
        return;
    target = item;
    next = &previous;
    position.x = 0.0f;
    position.y = 0.0f;
    position.z = distance;
    rotation.x += 5461;
    FTBL.ApplyScenarioTranslationAndRotation(&item, &rotation, &position);
    if (frames > 1) {
        FTBL.camCtrlGetNextCamPos(next);
        FTBL.camCtrlGetTargetPosition(&previousTarget);
        blend = 1.0f / (float)frames;
        RO10_FLUSH(blend);
        blend = 1.0f - blend;
        RO10_FLUSH(blend);
        delta = previous.x - position.x;
        RO10_FLUSH(delta);
        delta *= blend;
        RO10_FLUSH(delta);
        position.x += delta;
        RO10_FLUSH(position.x);
        delta = previous.y - position.y;
        RO10_FLUSH(delta);
        delta *= blend;
        RO10_FLUSH(delta);
        position.y += delta;
        RO10_FLUSH(position.y);
        delta = previous.z - position.z;
        RO10_FLUSH(delta);
        delta *= blend;
        RO10_FLUSH(delta);
        position.z += delta;
        RO10_FLUSH(position.z);
        delta = previousTarget.x - target.x;
        RO10_FLUSH(delta);
        delta *= blend;
        RO10_FLUSH(delta);
        target.x += delta;
        RO10_FLUSH(target.x);
        delta = previousTarget.y - target.y;
        RO10_FLUSH(delta);
        delta *= blend;
        RO10_FLUSH(delta);
        target.y += delta;
        RO10_FLUSH(target.y);
        delta = previousTarget.z - target.z;
        RO10_FLUSH(delta);
        delta *= blend;
        RO10_FLUSH(delta);
        target.z += delta;
        RO10_FLUSH(target.z);
    }
    FTBL.camCtrlSetState(26);
    FTBL.camCtrlSetNextPosition(&position);
    FTBL.camCtrlSetTargetPosition(&target);
    FTBL.camCtrlSetPerspective(7281);
}

void CreateRo10PriceBoards(const Ro10Shop *shop, shen_name *boards, shen_name *hundreds, shen_name *tens, shen_name *ones)
{
    shen_name digits[3];
    char numbers[4];
    char model[8];
    Vector3 position, scale;
    Vector3i rotation;
    int index, digit;
    const int *prices;
    if (!shop || !boards || !hundreds || !tens || !ones)
        return;
    prices = shop->prices;
    for (index = 0; index < 16; index++) {
        if (prices[index] > 0) {
            boards[index] = FTBL.CreateEventCharacter(0, "SHITAJI", 0);
            if (boards[index]) {
                numbers[2] = prices[index] / 100 % 10 + '0';
                numbers[1] = prices[index] / 10 % 10 + '0';
                numbers[0] = prices[index] % 10 + '0';
                if (numbers[2] == '0') {
                    numbers[2] = 0;
                    if (numbers[1] == '0')
                        numbers[1] = 0;
                }
                for (digit = 0; digit < 3; digit++) {
                    if (numbers[digit]) {
                        model[0] = 'N';
                        model[1] = 'U';
                        model[2] = 'M';
                        model[3] = numbers[digit];
                        model[4] = 0;
                        digits[digit] = FTBL.CreateEventCharacter(0, model, 0);
                        if (digits[digit]) {
                            FTBL.AT_SetLocalBaseChara(digits[digit], boards[index]);
                            position.x = (float)(digit - 1) * -0.02f;
                            RO10_FLUSH(position.x);
                            position.x += 0.0f;
                            RO10_FLUSH(position.x);
                            position.y = 0.001f;
                            position.z = 0.002f;
                            rotation.x = 0xc71c;
                            rotation.y = 0;
                            rotation.z = 0;
                            FTBL.EV_AT_SetPos(digits[digit], 0x38000000, &position);
                            FTBL.EV_AT_SetAngY_0(digits[digit], 0x38000000, &rotation, 0);
                            scale.x = 0.25f;
                            scale.y = 0.25f;
                            scale.z = 0.25f;
                            FTBL.mtwkSetMotionworkScale3(digits[digit], &scale);
                        }
                    } else
                        digits[digit] = 0;
                }
            } else
                digits[0] = digits[1] = digits[2] = 0;
            hundreds[index] = digits[2];
            tens[index] = digits[1];
            ones[index] = digits[0];
        } else
            boards[index] = 0;
    }
}
#else
#include "s2/areas/area_ro10.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/map_assets.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <cmath>
#include <cstring>
namespace shenmue::s2::areas {
namespace {
struct Ro10ExitWork {
    HLib::Task *parent;
    HLib::Task *owner;
};
void ShopMain(void *);
void ShopSelect(void *);
void ShopSelectionIcon(void *);
void ShopChoice(void *);
void ShopDeclined(void *);
void ShopAward(void *);
void ShopHand(void *);
void WaitShopPackage(void *);
void WaitShopOwner(void *);
void WaitShopReady(void *parameter) {
    Ro10Work *work = static_cast<Ro10Work *>(parameter);
    if (!work || filmCheckFadeTrackColour())
        return;
    if (!(work->flags & 0x20000U)) {
        HLib::SetCurrentTaskTo(WaitShopPackage);
        HLib::SetupTaskDelay(-1);
        EV_NPC_NewLoadSw(0);
        EV_ResetFreeTalkFlags();
        QueueAreaLoopPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), work->package);
    } else {
        InitiateCleanupForSceneChara(work->owner);
        if (work->flags & 1U) {
            HLib::SetCurrentTaskTo(WaitShopPackage);
            HLib::SetupTaskDelay(-1);
            EV_NPC_NewLoadSw(0);
            EV_ResetFreeTalkFlags();
            QueueAreaLoopPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), work->package);
        } else {
            SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->flags |= 1U;
        }
    }
}
void WaitShopPackage(void *parameter) {
    Ro10Work *work = static_cast<Ro10Work *>(parameter);
    if (!work)
        return;
    HLib::SetCurrentTaskTo(WaitShopOwner);
    HLib::SetupTaskDelay(-1);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>(work->package), 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>(work->package), 0);
    CreateRo10PriceBoards(*work);
    Vector3 position;
    GetRo10ItemTransform(*work->definition, 0, 0, &position, nullptr, nullptr, false);
    EV_AT_SetPos(RO10_BASE, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(RO10_BASE, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, 0, 0});
    SetLowQualityHands(ctrlGetCharaByName(RO10_HAND));
    SetHighModelXYZ(ctrlGetCharaByName(RO10_HAND), 1, 1, 0);
    const shen_name actors[]{work->owner, shen_name{0}};
    QueueAreaLoopNpcs(actors, ResolveAreaLoopParent);
}
void WaitShopOwner(void *parameter) {
    Ro10Work *work = static_cast<Ro10Work *>(parameter);
    if (!work)
        return;
    const Ro10Shop &shop = *work->definition;
    const bool alternate = (work->flags & 0x20000U) != 0;
    HLib::SetCurrentTaskTo(Ro10EnterItems);
    if (!alternate)
        HLib::SetupTaskDelay(-1);
    SetAreaLoopCharacterControl(RO10_RYO, 1);
    SetAreaLoopCharacterControl(work->owner, 1);
    if (shop.prop.name)
        SetDisplayFlagForProp(shop.prop, 0);
    if (work->audio[0]) {
        EV_FreeAuthorPak(1, work->audio);
        work->flags |= 2U;
    }
    QueueRo10Motion(RO10_RYO, 11462, 8, 18);
    QueueRo10Motion(work->owner, shop.ownerMotion, 8, 18);
    Vector3 position;
    EV_GetCharaPosition(RO10_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    position.x = shop.playerX;
    position.z = shop.playerZ;
    position.y += .5f;
    position.y = shenFindIntersectionWithScene(position, position.y);
    const AreaLoopPositionWork player{
        .parent = HLib::GetCurrentTask(), .character = RO10_RYO, .flags = 3, .position = position, .rotation = {0, shop.playerYaw, 0}, .completion = nullptr};
    QueueAreaLoopPosition(player, 0);
    position.x = shop.ownerX;
    position.z = shop.ownerZ;
    if (work->owner.name) {
        const AreaLoopPositionWork owner{.parent = HLib::GetCurrentTask(),
            .character = work->owner,
            .flags = 3,
            .position = position,
            .rotation = {0, shop.ownerYaw, 0},
            .completion = nullptr};
        QueueAreaLoopPosition(owner, 0);
    }
    RefreshRo10Items(*work);
    if (!alternate) {
        PlayRo10Greeting(*work);
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        work->flags &= ~1U;
    }
}
void ShopMain(void *parameter) {
    if (shenGetMinutesUntilNextFrameMinute() <= 1) {
        HLib::SetCurrentTaskTo(Ro10Quit);
        return;
    }
    Ro10Work *work = static_cast<Ro10Work *>(parameter);
    if (!work)
        return;
    const Ro10Shop &shop = *work->definition;
    if (++work->frames > 10800) {
        HLib::SetCurrentTaskTo(Ro10Quit);
        return;
    }
    const std::int16_t input = static_cast<std::int16_t>(GetCurrentControlInput(INPUT_REPEAT));
    bool moved = true;
    if (input < 0) {
        if (++work->column >= shop.columns) {
            work->column = shop.columns - 1;
            moved = false;
        }
    } else if (input & 0x4000) {
        if (--work->column < 0) {
            work->column = 0;
            moved = false;
        }
    } else if (input & 0x1000) {
        if (--work->row < 0) {
            work->row = 0;
            moved = false;
        }
    } else if (input & 0x2000) {
        if (++work->row >= shop.rows) {
            work->row = shop.rows - 1;
            moved = false;
        }
    } else
        moved = false;
    if (moved)
        work->cameraFrames = 8;
    float distance = Ro10FlushSmall(static_cast<float>(
        static_cast<std::int32_t>(GetCurrentControlInput(INPUT_LEFT_TRIGGER)) - static_cast<std::int32_t>(GetCurrentControlInput(INPUT_RIGHT_TRIGGER))));
    distance = Ro10FlushSmall(distance * .00390625f);
    distance = Ro10FlushSmall(distance * .2f);
    distance = Ro10FlushSmall(distance + .5f);
    distance = Ro10FlushSmall(distance * .2f);
    work->cameraDistance = Ro10FlushSmall(work->cameraDistance * .80000001f);
    work->cameraDistance = Ro10FlushSmall(distance + work->cameraDistance);
    UpdateRo10ItemCamera(shop, work->column, work->row, work->cameraDistance, work->cameraFrames);
    const std::int32_t previousFrames = work->cameraFrames;
    if (--work->cameraFrames < 0)
        work->cameraFrames = 0;
    const std::int32_t index = work->column + work->row * shop.columns;
    std::int32_t price = shop.prices[index], description = shop.dialogues[index];
    if (work->purchased[index] & 1U) {
        price = -1;
        description = 0;
    }
    if (!work->cameraFrames) {
        GetBattleDataConfig(0, SHEN_NAME(BTCC));
        if (price > 0)
            GetBattleDataConfig(0, SHEN_NAME(BTEV));
        if (description > 0) {
            if (previousFrames)
                Loop3DSoundEffect(0xab, std::fabs(work->cameraDistance));
            GetBattleDataConfig(0, SHEN_NAME(BTFT));
        }
    }
    if (moved || work->cameraFrames)
        return;
    CheckHelpIcon(33, 0, CheckHelpIconOp::FrameOverlay);
    if (GetBattleDataConfig(1, SHEN_NAME(BTCC)) && GetBattleDataConfig(2, SHEN_NAME(BTCC))) {
        HLib::SetCurrentTaskTo(Ro10Quit);
        return;
    }
    if (GetBattleDataConfig(1, SHEN_NAME(BTEV)) && GetBattleDataConfig(2, SHEN_NAME(BTEV))) {
        HLib::SetCurrentTaskTo(ShopSelect);
        return;
    }
    if (GetBattleDataConfig(1, SHEN_NAME(BTFT)) && GetBattleDataConfig(2, SHEN_NAME(BTFT)) && shop.dialogues[index] > 0) {
        HLib::SetupTaskDelay(-1);
        PlayRo10ItemDescription(shop.dialogues[index]);
    }
}
void ShopSelect(void *parameter) {
    Ro10Work *work = static_cast<Ro10Work *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(Ro10Quit);
        return;
    }
    HLib::SetCurrentTaskTo(ShopSelectionIcon);
    HLib::SetupTaskDelay(40);
    PlayRo10Selection(work->shop);
}
void ShopSelectionIcon(void *parameter) {
    if (!parameter) {
        HLib::SetCurrentTaskTo(Ro10Quit);
        return;
    }
    std::int16_t choices[]{-1, 80, 81, -1};
    SetupControlSpeakAndUI(choices, 10800, -1, 2);
    HLib::SetCurrentTaskTo(ShopChoice);
}
void ShopChoice(void *parameter) {
    Ro10Work *work = static_cast<Ro10Work *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(Ro10Quit);
        return;
    }
    if (GetShouldRenderIcons(RenderIconSlot::ActionSelector)) {
        if (std::fabs(work->cameraDistance - .5f) >= .00000011920929f) {
            work->cameraDistance = Ro10FlushSmall(work->cameraDistance);
            work->cameraDistance = Ro10FlushSmall(work->cameraDistance * .80000001f);
            work->cameraDistance = Ro10FlushSmall(work->cameraDistance + .1f);
            UpdateRo10ItemCamera(*work->definition, work->column, work->row, work->cameraDistance, 0);
        }
        return;
    }
    if (shenGetCtrlSpeakIconFlag() != 80) {
        HLib::SetCurrentTaskTo(ShopDeclined);
        return;
    }
    const std::int32_t index = work->column + work->row * work->definition->columns;
    if (static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) < work->definition->prices[index]) {
        HLib::SetCurrentTaskTo(ShopMain);
        HLib::SetupTaskDelay(-1);
        PlayRo10InsufficientCash(work->shop);
    } else {
        work->purchased[index] |= 1U;
        HLib::SetCurrentTaskTo(ShopAward);
        HLib::SetupTaskDelay(-1);
        QueueRo10Voice("0110A001", 1, -1, true);
    }
}
void ShopDeclined(void *parameter) {
    if (!parameter) {
        HLib::SetCurrentTaskTo(Ro10Quit);
        return;
    }
    HLib::SetCurrentTaskTo(ShopMain);
    HLib::SetupTaskDelay(-1);
    QueueRo10Voice("0110A002", 1, -1, true);
}
void ShopAward(void *parameter) {
    Ro10Work *work = static_cast<Ro10Work *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(Ro10Quit);
        return;
    }
    const std::int32_t grip = work->definition->grips[work->column + work->row * work->definition->columns];
    if (static_cast<std::uint32_t>(grip - 1) > 2U) {
        HLib::SetCurrentTaskTo(Ro10Quit);
        return;
    }
    PrepareRo10Award(*work);
    HLib::SetCurrentTaskTo(ShopHand);
}
void ShopHand(void *parameter) {
    Ro10Work *work = static_cast<Ro10Work *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(Ro10Quit);
        return;
    }
    UpdateRo10Hand(*work);
}
void ExitAfterFade(void *parameter) {
    if (filmCheckFadeTrackColour())
        return;
    Ro10ExitWork &work = *static_cast<Ro10ExitWork *>(parameter);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.owner);
    HLib::ResolveDeferredCallback(work.parent);
    HLib::DestroyCurrentTask();
    SetPlayerControlEnabled(1);
}
void InitializeRo10() {
}
void RunRo10(std::uintptr_t parameter) {
    const shen_name *argument = reinterpret_cast<const shen_name *>(parameter);
    if (!argument)
        return;
    shen_name shop = *argument;
    const std::uint8_t first = static_cast<std::uint8_t>(shop.name);
    const bool alternate = static_cast<std::uint32_t>(first - 'a') <= 25U;
    if (alternate)
        shop.name &= ~0x20U;
    const Ro10Shop *definition = ro10Shops;
    while (definition->shop.name && definition->shop.name != shop.name)
        ++definition;
    if (!definition->shop.name)
        return;
    if (alternate && definition->resetFlag)
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(definition->resetFlag), 0);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(WaitShopReady, 4, 0x0b, sizeof(Ro10Work), RO10_EVENT);
    if (!task)
        return;
    Ro10Work *work = static_cast<Ro10Work *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    std::memset(work, 0, sizeof(*work));
    work->parent = HLib::GetCurrentTask();
    work->self = task;
    std::memcpy(work->package, "RO10", 4);
    std::memcpy(work->package + 4, &shop.name, 4);
    work->package[8] = 0;
    std::strcpy(work->audio, "0110");
    work->flags = alternate ? 0x20000U : 0;
    work->shop = shop;
    work->owner = definition->owner;
    work->definition = definition;
    if (GetNpcNewLoadState())
        work->flags |= 0x10000U;
    if (!alternate) {
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1U;
    }
    SetPlayerControlEnabled(0);
    EV_EventSetUpHelpIcon_0(static_cast<std::uint8_t>(definition->help), 0);
}
} // namespace
void Ro10EnterItems(void *parameter) {
    Ro10Work *work = static_cast<Ro10Work *>(parameter);
    if (!work) {
        HLib::SetCurrentTaskTo(Ro10Quit);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (work->flags & 1U) {
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        work->flags &= ~1U;
    }
    RefreshRo10Items(*work);
    EV_DisableCharacterDisplay(RO10_RYO);
    EV_DisableCharacterDisplay(work->owner);
    work->flags |= 4U;
    work->cameraFrames = 0;
    work->cameraDistance = .5f;
    UpdateRo10ItemCamera(*work->definition, work->column, work->row, .5f, 0);
    HLib::SetCurrentTaskTo(ShopMain);
}
void Ro10Quit(void *parameter) {
    using enum CameraState;
    if (filmCheckFadeTrackColour())
        return;
    Ro10Work *work = static_cast<Ro10Work *>(parameter);
    HLib::Task *parent = nullptr;
    std::uint32_t alternate = 0;
    if (work) {
        if (!(work->flags & 1U)) {
            SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->flags |= 1U;
            return;
        }
        parent = work->parent;
        if (work->definition->prop.name)
            SetDisplayFlagForProp(work->definition->prop, 1);
        for (std::int32_t index = 0; index < 16; ++index) {
            if (work->boards[index].name)
                SetCharaKill(work->boards[index], 1);
            if (work->hundreds[index].name)
                SetCharaKill(work->hundreds[index], 1);
            if (work->tens[index].name)
                SetCharaKill(work->tens[index], 1);
            if (work->ones[index].name)
                SetCharaKill(work->ones[index], 1);
        }
        if (work->flags & 4U) {
            EV_EnableCharacterDisplay(RO10_RYO);
            EV_EnableCharacterDisplay(work->owner);
        }
        SetAreaLoopCharacterControl(work->owner, 0);
        if (work->owner.name && DoesScenarioCharacterExist(work->owner)) {
            SetAreaLoopCharacterControl(work->owner, 0);
            EV_npcLockUnlock(work->owner);
        }
        EV_NPC_NewLoadSw((work->flags >> 16) & 1U);
        if ((work->flags & 2U) && work->audio[0]) {
            EV_FreeAuthorPak(0, work->audio);
            work->flags &= ~2U;
        }
        if (work->package[0] && EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(work->package), 0)) {
            EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(work->package), 0);
            work->package[0] = 0;
        }
        const std::int32_t flag = work->flags & 0x10U ? work->definition->purchasedFlag : work->definition->declinedFlag;
        if (flag)
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(flag), 1);
        alternate = work->flags & 0x20000U;
        EV_EventSetUpHelpIcon_0(0, 0);
    }
    camCtrlSetState(Init);
    camCtrlSetFovTransitionTo(true);
    SetAreaLoopCharacterControl(RO10_RYO, 0);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    QueueRo10Motion(RO10_RYO, 0xf002, 0, 2);
    HLib::Task *fadeTask = alternate ? HLib::EnqueueTaskWithParameter(ExitAfterFade, 4, 0x0b, sizeof(Ro10ExitWork), RO10_EVENT) : nullptr;
    if (fadeTask) {
        Ro10ExitWork *exit = static_cast<Ro10ExitWork *>(HLib::GetTaskParameter(fadeTask));
        exit->parent = parent;
        exit->owner = HLib::GetCurrentTask();
        HLib::SetupTaskDelay(-1);
        SetScreenFilmFade(16, 255, 0, 0, 0, 0, 0, 0, 0);
    } else {
        HLib::MarkCurrentTaskForCleanup();
        if (parent)
            HLib::ResolveDeferredCallback(parent);
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        SetPlayerControlEnabled(1);
    }
}
const ScriptAreaDefinition ro10ScriptDefinition{.areaId = SHEN_NAME(RO10), .field04 = 0, .init = InitializeRo10, .main = RunRo10};
} // namespace shenmue::s2::areas
#endif
