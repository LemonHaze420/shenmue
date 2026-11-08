#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_sfwf.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void SfwfMain(StreetFightStartParameters *start)
{
    if (start->scene == 99)
        QueueDebug(start);
    else
        QueueNormal(start);
}
#elif defined(SHENMUE_EVENT_SFWF_AUTHOR)
static shen_name sfwfAuthorCharacters[] = {SFWF_CHN, SFWF_CYC, SFWF_REN, SFWF_EEN, SFWF_SFA, SFWF_FIGHTER, SFWF_INVALID};

void QueueAuthor(shen_name author)
{
    HLibTask *task;
    SfwfAuthorWork *work;
    task = FTBL.HLib_EnqueueTaskWithParameter(AuthorMainCallback, 4, 11, sizeof(SfwfAuthorWork), TASK_EVNT);
    work = (SfwfAuthorWork *)FTBL.HLib_GetTaskParameter(task);
    work->task = task;
    work->state = 1;
    work->author = author;
    BeginAuthor();
}

void AuthorMainCallback(void *data)
{
    SfwfAuthorWork *work = (SfwfAuthorWork *)data;
    switch (work->state) {
    case 1:
        QueueAuthorLoop(work);
        break;
    case 3:
        EndAuthor(work);
        break;
    }
}

void QueueAuthorLoop(SfwfAuthorWork *work)
{
    HLibTask *task;
    SfwfAuthorLoopWork *loop;
    task = FTBL.HLib_EnqueueTaskWithParameter(AuthorLoopCallback, 4, 11, sizeof(SfwfAuthorLoopWork), TASK_EVNT);
    loop = (SfwfAuthorLoopWork *)FTBL.HLib_GetTaskParameter(task);
    loop->parent = work;
    loop->frame = 0;
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.EV_authSceneCtrl(work->author);
    work->state = 2;
}

void AuthorLoopCallback(void *data)
{
    SfwfAuthorLoopWork *loop = (SfwfAuthorLoopWork *)data;
    loop->frame = FTBL.EV_authSceneCtrl(-6);
    if (!loop->frame)
        PrepareAuthorCharacters();
    switch (loop->parent->author) {
    case SFWF_AUTHOR_0:
        if (loop->frame == FTBL.EV_authSceneCtrl(-5) - 30)
            FTBL.spmFadeOut(0);
        break;
    case SFWF_AUTHOR_1:
        switch (loop->frame) {
        case 744:
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_EEN), 0, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_EEN), 0, 0, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_FIGHTER), 0, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_FIGHTER), 0, 0, 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFWF_EEN), 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFWF_FIGHTER), 0);
            FTBL.SetScenarioCharacterType(SFWF_PLAYER, 0);
            FTBL.SetScenarioCharacterType(SFWF_REN, 0);
            FTBL.SetScenarioCharacterType(SFWF_EEN, 4);
            FTBL.SetScenarioCharacterType(SFWF_FIGHTER, 4);
            break;
        case 874:
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_EEN), 1, 1, 0);
            Queue_EVT_AT_CtrlHandUp_1(SFWF_EEN, 1, sfwfOpenHandPose, 0, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_EEN), 0, 0, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_FIGHTER), 0, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_FIGHTER), 1, 0, 0);
            Queue_EVT_AT_CtrlHandUp_1(SFWF_FIGHTER, 0, sfwfOpenHandPose, 0, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_PLAYER), 0, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_PLAYER), 0, 0, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_REN), 0, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_REN), 0, 0, 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFWF_EEN), 1);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFWF_PLAYER), 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFWF_REN), 0);
            break;
        case 927:
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_FIGHTER), 0, 1, 0);
            break;
        case 1094:
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_REN), 1, 0, 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFWF_REN), 1);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_FIGHTER), 1, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_FIGHTER), 1, 0, 0);
            Queue_EVT_AT_CtrlHandUp_1(SFWF_FIGHTER, 1, sfwfOpenHandPose, 0, 0);
            Queue_EVT_AT_CtrlHandUp_1(SFWF_FIGHTER, 0, sfwfOpenHandPose, 0, 0);
            break;
        case 1149:
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_PLAYER), 0, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_PLAYER), 0, 0, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_REN), 0, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_REN), 0, 0, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_EEN), 0, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_EEN), 0, 0, 0);
            Queue_EVT_AT_CtrlHandUp_1(SFWF_FIGHTER, 1, sfwfOpenHandPose, 0, 0);
            Queue_EVT_AT_CtrlHandUp_1(SFWF_FIGHTER, 0, sfwfOpenHandPose, 0, 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFWF_EEN), 0);
            break;
        case 1150:
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_EEN), 0, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_EEN), 0, 0, 0);
            Queue_EVT_AT_CtrlHandUp_1(SFWF_FIGHTER, 1, sfwfOpenHandPose, 0, 0);
            Queue_EVT_AT_CtrlHandUp_1(SFWF_FIGHTER, 0, sfwfOpenHandPose, 0, 0);
            break;
        case 1439:
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_PLAYER), 1, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_PLAYER), 1, 0, 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFWF_PLAYER), 1);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFWF_REN), 1);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFWF_FIGHTER), 1);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFWF_EEN), 1);
            Queue_EVT_AT_CtrlHandUp_1(SFWF_PLAYER, 1, sfwfOpenHandPose, 0, 0);
            Queue_EVT_AT_CtrlHandUp_1(SFWF_PLAYER, 0, sfwfOpenHandPose, 0, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_REN), 1, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_REN), 1, 0, 0);
            break;
        case 1599:
            break;
        case 1839:
            break;
        case 1899:
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_FIGHTER), 0, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_FIGHTER), 0, 0, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_EEN), 0, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_EEN), 0, 0, 0);
            break;
        case 2029:
            break;
        case 2273:
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_FIGHTER), 0, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_FIGHTER), 0, 0, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_EEN), 0, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_EEN), 0, 0, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_REN), 0, 1, 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFWF_REN), 0);
            FTBL.SetScenarioCharacterType(SFWF_PLAYER, 4);
            break;
        case 2321:
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_REN), 0, 0, 0);
            break;
        case 2422:
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_REN), 1, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_REN), 1, 0, 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFWF_REN), 1);
            break;
        case 2607:
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_FIGHTER), 1, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_FIGHTER), 1, 0, 0);
            FTBL.SetScenarioCharacterType(SFWF_EEN, 0);
            break;
        case 2809:
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_PLAYER), 0, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_PLAYER), 0, 0, 0);
            FTBL.SetScenarioCharacterType(SFWF_PLAYER, 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFWF_PLAYER), 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFWF_FIGHTER), 0);
            break;
        }
        break;
    case SFWF_AUTHOR_2:
        switch (loop->frame) {
        case 646:
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_FIGHTER), 0, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_FIGHTER), 0, 0, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_REN), 0, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_REN), 0, 0, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_CHN), 0, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_CHN), 0, 0, 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFWF_FIGHTER), 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFWF_EEN), 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFWF_REN), 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFWF_CHN), 0);
            FTBL.SetScenarioCharacterType(SFWF_PLAYER, 0);
            FTBL.SetScenarioCharacterType(SFWF_REN, 4);
            FTBL.SetScenarioCharacterType(SFWF_EEN, 0);
            FTBL.SetScenarioCharacterType(SFWF_FIGHTER, 0);
            FTBL.SetScenarioCharacterType(SFWF_CHN, 0);
            break;
        case 726:
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_EEN), 0, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_EEN), 0, 0, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_FIGHTER), 0, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_FIGHTER), 0, 0, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_PLAYER), 0, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_PLAYER), 0, 0, 0);
            break;
        case 906:
            Queue_EVT_AT_CtrlHandUp_1(SFWF_PLAYER, 1, sfwfOpenHandPose, 0, 0);
            Queue_EVT_AT_CtrlHandUp_1(SFWF_PLAYER, 0, sfwfOpenHandPose, 0, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_REN), 1, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_REN), 1, 0, 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFWF_REN), 1);
            break;
        case 999:
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_EEN), 0, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_EEN), 0, 0, 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFWF_EEN), 0);
            break;
        case 1109:
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_REN), 0, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_REN), 0, 0, 0);
            break;
        case 1400:
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_PLAYER), 0, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_PLAYER), 0, 0, 0);
            break;
        case 1480:
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_REN), 1, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFWF_REN), 1, 0, 0);
            Queue_EVT_AT_CtrlHandUp_1(SFWF_REN, 1, sfwfOpenHandPose, 0, 0);
            Queue_EVT_AT_CtrlHandUp_1(SFWF_REN, 0, sfwfOpenHandPose, 0, 0);
            break;
        }
        break;
    }
    if (!FTBL.EV_authSceneCtrl(-1)) {
        RestoreAuthorCharacters();
        loop->parent->state = 3;
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.HLib_DestroyCurrentEventTask();
    }
}

void BeginAuthor(void)
{
    FTBL.SetPlayerControlEnabled(0);
}

void EndAuthor(SfwfAuthorWork *work)
{
    FTBL.HLib_DestroyTask(work->task);
}

void PrepareAuthorCharacters(void)
{
    int i;
    HideAuthorCharacter(SFWF_PLAYER);
    HideAuthorProp(SFWF_PROP_A428);
    HideAuthorProp(SFWF_PROP_A118);
    for (i = 0; sfwfAuthorCharacters[i] != SFWF_INVALID; i++)
        HideAuthorCharacter(sfwfAuthorCharacters[i]);
}

void RestoreAuthorCharacters(void)
{
    int i;
    ShowAuthorCharacter(SFWF_PLAYER);
    ShowAuthorProp(SFWF_PROP_A428);
    ShowAuthorProp(SFWF_PROP_A118);
    for (i = 0; sfwfAuthorCharacters[i] != SFWF_INVALID; i++)
        ShowAuthorCharacter(sfwfAuthorCharacters[i]);
}

void HideAuthorCharacter(shen_name character)
{
    FTBL.EV_EnableCharacterDisplay(character);
    FTBL.EV_SetCharaControllerFlags(character, 1, 0);
    FTBL.EV_SetCharaControllerFlags(character, 3, 0);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 2);
    FTBL.EV_SetCharacterAlphaEnabled(character, 0);
}

void ShowAuthorCharacter(shen_name character)
{
    FTBL.EV_EnableCharacterDisplay(character);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 0);
    FTBL.EV_SetCharacterAlphaEnabled(character, 1);
}

void HideAuthorProp(shen_name prop)
{
    FTBL.EV_EnableCharacterDisplay(prop);
    FTBL.EV_SetCharaControllerFlags(prop, 1, 0);
    FTBL.EV_SetCharaControllerFlags(prop, 3, 0);
    FTBL.EV_SetCharacterAlphaEnabled(prop, 0);
}

void ShowAuthorProp(shen_name prop)
{
    FTBL.EV_DisableCharacterDisplay(prop);
}
#else
static const shen_name sfwfCharacters[] = {SFWF_PLAYER, SFWF_CHN, SFWF_CYC, SFWF_REN, SFWF_EEN, SFWF_SFA, SFWF_FIGHTER, SFWF_INVALID};
static const Vector3 sfwfPlayerPosition = {-3.3f, -0.5f, -5.5f};
static const Vector3 sfwfBookiePosition = {-1.864f, 0.0f, -1.8f};
static unsigned char sfwfFighterParameters[6][12] = {
    {85, 69, 69, 69, 32, 32, 80, 80, 112},
    {72, 101, 101, 101, 32, 48, 96, 106, 116},
    {58, 120, 120, 120, 32, 64, 104, 117, 120},
    {40, 128, 128, 128, 32, 80, 112, 128, 136},
    {19, 144, 144, 144, 32, 88, 120, 159, 152},
    {5, 159, 159, 159, 32, 80, 127, 175, 160},
};
static Vector3 sfwfEenPosition = {367.0f, 76.1f, 121.16f};
static Vector3i sfwfEenRotation = {0, 51190, 0};
static Vector3 sfwfAuthorExitPosition = {339.04f, 82.28f, 120.71f};
static Vector3i sfwfAuthorExitRotation = {0, 49152, 0};
static Vector3 sfwfExitPosition = {365.1f, 76.14f, 121.5f};
static Vector3i sfwfExitRotation = {0, 49152, 0};
static PluralControlEntry sfwfPreviewPluralControl[] = {
    {385, SFWF_PLAYER, 1}, {385, SFWF_FIGHTER, 1}, {385, SFWF_SFA, 1}, {645, SFWF_PLAYER, 0}, {645, SFWF_FIGHTER, 0},
    {645, SFWF_SFA, 0}, {1050, SFWF_SFA, 1}, {1050, SFWF_EEN, 1}, {-1, 0, 0},
};
static PluralControlEntry sfwfFirstPluralControl[] = {
    {874, SFWF_PLAYER, 1}, {874, SFWF_REN, 1}, {874, SFWF_FIGHTER, 1}, {1094, SFWF_REN, 0}, {1149, SFWF_REN, 1},
    {1149, SFWF_FIGHTER, 1}, {1149, SFWF_EEN, 1}, {1439, SFWF_FIGHTER, 0}, {1599, SFWF_FIGHTER, 1}, {1599, SFWF_REN, 0},
    {1599, SFWF_PLAYER, 1}, {1839, SFWF_FIGHTER, 0}, {1899, SFWF_REN, 0}, {1899, SFWF_FIGHTER, 1}, {2029, SFWF_REN, 0},
    {2273, SFWF_REN, 1}, {2422, SFWF_PLAYER, 0}, {2422, SFWF_REN, 0}, {2607, SFWF_FIGHTER, 1}, {-1, 0, 0},
};
static PluralControlEntry sfwfVictoryPluralControl[] = {
    {646, SFWF_PLAYER, 1}, {646, SFWF_REN, 1}, {646, SFWF_EEN, 0}, {646, SFWF_FIGHTER, 1}, {999, SFWF_PLAYER, 1},
    {999, SFWF_FIGHTER, 1}, {1109, SFWF_PLAYER, 1}, {1109, SFWF_REN, 0}, {1109, SFWF_FIGHTER, 1}, {1480, SFWF_PLAYER, 0},
    {1480, SFWF_REN, 0}, {-1, 0, 0},
};

void QueueNormal(StreetFightStartParameters *start)
{
    HLibTask *task;
    StreetFightEventWork *work;
    if (start->signal)
        *start->signal = 1;
    FTBL.SetPlayerControlEnabled(0);
    FTBL.SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    task = FTBL.HLib_EnqueueTaskWithParameter(MainCallback, 5, 11, sizeof(StreetFightEventWork), TASK_EVNT);
    work = InitializeNormal(task);
    work->startParameters = start;
    work->returnToQron = 0;
    LoadNormalResources(work);
    EventDebugRegister(3, NormalDebugCallback, work, 36000);
}

void LoadNormalResources(StreetFightEventWork *work)
{
    if (FTBL.IsFightActive())
        FTBL.check_dead_status(1);
    work->motions[0] = 0;
    FTBL.EV_FreeAuthorPak(1, "0258");
    if (!FTBL.EV_ControlPackageResources(0, 5, "QGBT", 0, 0)) {
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"QGBT", 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    }
    FTBL.EV_ControlPackageResources(2, 4, "QGBT", 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "QGBT", 0, 0);
    FTBL.CreateEventCharacter(3, 0, STREET_FIGHT_BASE);
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_LoadCutsceneFromCurrentScene("0258A000");
    work->previousFarClip = FTBL.camCtrlGetFarDistance();
    work->previousNearClip = FTBL.camCtrlGetNearDistance();
    FTBL.FinishLoading();
    FTBL.SetRenderClockAndCompassTo(1);
}

void PlaceNormal(StreetFightEventWork *work)
{
    int i;
    SetStreetFightCharacterVisible(SFWF_FIGHTER, 1);
    work->playerPosition.x = sfwfPlayerPosition.x;
    work->playerPosition.y = sfwfPlayerPosition.y;
    work->playerPosition.z = sfwfPlayerPosition.z;
    work->bookiePosition.x = sfwfBookiePosition.x;
    work->bookiePosition.y = sfwfBookiePosition.y;
    work->bookiePosition.z = sfwfBookiePosition.z;
    EVT_AT_SetPos(STREET_FIGHT_BASE, STREET_FIGHT_INVALID, 371.78f, 76.65f, 120.78f, 0);
    EVT_AT_SetAngY(STREET_FIGHT_BASE, STREET_FIGHT_INVALID, 0.0f, 0);
    FTBL.AT_SetLocalBaseChara(SFWF_PLAYER, STREET_FIGHT_BASE);
    FTBL.AT_SetLocalBaseChara(SFWF_FIGHTER, STREET_FIGHT_BASE);
    FTBL.AT_SetLocalBaseChara(SFWF_STAG, STREET_FIGHT_BASE);
    EVT_AT_SetMot(SFWF_PLAYER, 306, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetMot(work->fighters[work->fighterIndex], 2901, 0.0f, -1.0f, 0x40000000, 0);
    EVT_AT_SetPos(SFWF_PLAYER, STREET_FIGHT_BASE, 0.0f, 0.0f, -1.1f, 0);
    EVT_AT_SetAngY(SFWF_PLAYER, STREET_FIGHT_BASE, 180.0f, 0);
    EVT_AT_SetPos(work->fighters[work->fighterIndex], STREET_FIGHT_BASE, 0.0f, 0.0f, 1.1f, 0);
    EVT_AT_SetAngY(work->fighters[work->fighterIndex], STREET_FIGHT_BASE, 0.0f, 0);
    FTBL.vfmDispatchControl(20, 0, 0, 1);
    FTBL.LoadBlurModelForCharacter_0(SFWF_PLAYER, "RYO_B.MT7", 1.0f);
    FTBL.LoadBlurModelForCharacter_0(SFWF_FIGHTER, "SF6_B.MT7", 1.0f);
    for (i = 0; sfwfCharacters[i] != SFWF_INVALID; i++)
        FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(sfwfCharacters[i]));
    FTBL.adfsSetCameraIdxForChara(SFWF_EEN, SFWF_EEN_CAMERA);
}

void UnparentStreetFightCharacter(shen_name character)
{
    Vector3 position;
    Vector3i rotation;
    FTBL.GetCharaPosition(character, STREET_FIGHT_INVALID, &position, 0x40000000);
    FTBL.GetScenarioCharacterEulerAngles(character, &rotation, 0x40000000);
    FTBL.AT_SetLocalBaseChara(character, STREET_FIGHT_INVALID);
    FTBL.EV_AT_SetPos(character, 0x78000000, &position);
    FTBL.EV_AT_SetAngY_0(character, 0x78000000, &rotation, 0);
}

StreetFightEventWork *InitializeNormal(HLibTask *task)
{
    StreetFightEventWork *work = (StreetFightEventWork *)FTBL.HLib_GetTaskParameter(task);
    work->task = task;
    work->state = 0;
    work->battleStatus = 0;
    work->previousNpcBatchCount = FTBL.GetNpcBatchCountOverride(35);
    work->fighterIndex = 0;
    work->fighters[0] = SFWF_FIGHTER;
    work->fighters[1] = SFWF_INVALID;
    work->characterReadTask = Queue_EVT_AT_ReadChr(task, sfwfCharacters);
    work->audienceTask = 0;
    work->fadeFrames = 0;
    work->attempts = 0;
    work->audienceResultPublished = 0;
    work->stage = GetStreetFightStageForArea(g_currentMap);
    work->rank = FTBL.vfmDispatchControl(14, work->stage | 64, 0, -1);
    work->frameCounter = 60;
    return work;
}

void MainCallback(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    Vector3 position;
    int index = 0;
    int difficulty, result, active;
    switch (work->state) {
    case 0:
        EventDebugCheck(3, 4);
        PlaceNormal(work);
        FTBL.SetMapControlStatusFlag(0, 0);
        if (!work->audienceTask) {
            work->audienceTask = QueueStreetFightAudience(30, 0);
            AddStreetFightAudienceFighter(work->audienceTask, SFWF_PLAYER);
            AddStreetFightAudienceFighter(work->audienceTask, SFWF_FIGHTER);
        }
        work->state++;
        break;
    case 1:
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(work->fighters[work->fighterIndex]), 0, 1, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(work->fighters[work->fighterIndex]), 0, 0, 0);
        ResetStreetFightRounds(work);
        work->state++;
        break;
    case 2:
        if (work->startParameters->subScene != 99) {
            FTBL.EV_EventSetUpHelpIcon_0(GetHelpIconForArea(g_currentMap), 0);
            FTBL.shenStartFGST(10, 10, 0, 0);
            FTBL.vfmDispatchControl(2, 0, "QGBT", 1);
        }
        FTBL.GetCharaPosition(STREET_FIGHT_BASE, -1, &position, 0);
        SetStreetFightAudienceCentre(work->audienceTask, &position);
        FTBL.EV_NPC_NewLoadSw_0(1);
        work->state++;
        break;
    case 3:
        if (IsStreetFightBestOfThree())
            work->state = 7;
        else if (work->startParameters->subScene == 99)
            work->state = 28;
        else if (!work->round)
            work->state++;
        else
            work->state = 7;
        break;
    case 4:
        UnparentStreetFightCharacter(SFWF_PLAYER);
        UnparentStreetFightCharacter(work->fighters[work->fighterIndex]);
        if (FTBL.EV_GetPlayerFlag(11, 1792))
            FTBL.EV_authSceneCtrl(-16);
        Queue_ET_PluralControl(sfwfFirstPluralControl);
        QueueAuthor(SFWF_AUTHOR_1);
        FTBL.EV_SetPlayerFlag(12, 1292, 0);
        work->state++;
        break;
    case 5:
        if (FTBL.EV_authSceneCtrl(-1))
            work->state++;
        break;
    case 6:
        if (!FTBL.EV_authSceneCtrl(-1))
            work->state++;
        break;
    case 7:
        EVT_AT_SetMot(SFWF_PLAYER, 306, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetMot(work->fighters[work->fighterIndex], 2901, 0.0f, -1.0f, 0x40000000, 0);
        EVT_AT_SetMot(SFWF_REN, 0xf003, 0.0f, 1.0f, 0x40000000, 0);
        FTBL.EV_AT_SetPos(SFWF_EEN, 0x78000000, &sfwfEenPosition);
        FTBL.EV_AT_SetAngY_0(SFWF_EEN, 0x78000000, &sfwfEenRotation, 0);
        EVT_AT_SetMot(SFWF_EEN, 0xf003, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetPos(SFWF_PLAYER, STREET_FIGHT_BASE, 0.0f, 0.0f, -1.1f, 0);
        EVT_AT_SetAngY(SFWF_PLAYER, STREET_FIGHT_BASE, 180.0f, 0);
        EVT_AT_SetPos(work->fighters[work->fighterIndex], STREET_FIGHT_BASE, 0.0f, 0.0f, 1.1f, 0);
        EVT_AT_SetAngY(work->fighters[work->fighterIndex], STREET_FIGHT_BASE, 0.0f, 0);
        FTBL.SetScenarioCharacterType(SFWF_PLAYER, 4);
        FTBL.SetScenarioCharacterType(SFWF_FIGHTER, 4);
        FTBL.EV_SetCharaControllerFlags(SFWF_PLAYER, 2, 0);
        FTBL.EV_SetCharaControllerFlags(SFWF_PLAYER, 3, 0);
        SHENMUE_EVENT_BOUNDARY();
        FTBL.EV_SetCharaControllerFlags(SFWF_FIGHTER, 2, 0);
        FTBL.EV_SetCharaControllerFlags(SFWF_FIGHTER, 3, 0);
        SHENMUE_EVENT_BOUNDARY();
        FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(SFWF_PLAYER), 1, 1, 1.0f);
        FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(SFWF_FIGHTER), 1, 1, 1.0f);
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(SFWF_PLAYER), 1);
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(SFWF_FIGHTER), 1);
        SetStreetFightCharacterVisible(SFWF_EEN, 0);
        SetStreetFightCharacterVisible(SFWF_CYC, 0);
        SetStreetFightCharacterVisible(SFWF_SFA, 0);
        SetStreetFightCharacterVisible(SFWF_REN, 0);
        work->rank = FTBL.vfmDispatchControl(14, work->stage | 64, 0, -1);
        difficulty = FTBL.vfmDispatchControl(14, 140, 0, -1);
        if (difficulty > 2)
            difficulty = 2;
        FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(SFWF_PLAYER), 0, 0, 0, 300, 0);
        if (!IsStreetFightBestOfThree())
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(SFWF_FIGHTER), 0, 1, 1, 500 - difficulty * 100, 24);
        else
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(SFWF_FIGHTER), 0, 1, 1, work->rank * 80 + 300, 24);
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(SFWF_FIGHTER), 0, 0x8000000, difficulty);
        if (IsStreetFightBestOfThree())
            FTBL.vfmDispatchControl(16, 0, (const void *)(FTBL.vfmDispatchControl(14, 76, 0, -1) + 1), 1);
        FTBL.vfCopyPosFromEntity(0, 0, 371.78f, 120.78f, 2.5f);
        FTBL.vfmDispatchControl(15, work->wins, (const void *)work->losses, 1);
        FTBL.EV_vfBattleUtil(3, 0x8000, 1);
        work->fadeFrames = 30;
        FTBL.EV_SetPlayerFlag(12, 1292, 1);
        EVT_AT_MenCtrl(work->fadeFrames, 255, 0, 0, 0, 0, 0, 0, 0, 0);
        EVT_AT_SetCam(1400, SFWF_PLAYER, SFWF_PLAYER, 0, 0, 0);
        work->state++;
        break;
    case 8:
        FTBL.EV_LoadAudio("0258A000", 2);
        FTBL.EV_spmBeginSampleExecute(0);
        FTBL.HLib_SetupTaskDelay(work->fadeFrames);
        work->state++;
        break;
    case 9:
        EVT_AT_SetVoice("0258G001", 30);
        switch (work->round) {
        case 0:
            QueueEventMessage("「ＲＯＵＮＤ １」", 0, 60);
            break;
        case 1:
            QueueEventMessage("「ＲＯＵＮＤ ２」", 0, 60);
            break;
        case 2:
            QueueEventMessage("「ＦＩＮＡＬ ＲＯＵＮＤ」", 0, 60);
            break;
        }
        UnparentStreetFightCharacter(SFWF_PLAYER);
        UnparentStreetFightCharacter(work->fighters[work->fighterIndex]);
        FTBL.HLib_SetupTaskDelay(10);
        ResumeStreetFightAudienceOpening(work->audienceTask);
        work->audienceResultPublished = 0;
        work->state++;
        break;
    case 10:
        if (!FTBL.camCtrlCheckIsEventCam()) {
            FTBL.EV_PlayDialogue("0258G002");
            FTBL.HLib_SetupTaskDelay(10);
            work->state++;
        }
        break;
    case 11:
        FTBL.EV_vfBattleUtil(3, 0x800, 1);
        FTBL.EV_vfBattleUtil(3, 0x80000, 1);
        FTBL.vfmQueueFightStart(2, 99, 1.0f, 8, -1);
        FTBL.EV_SetCharacterAlphaEnabled(SFWF_FIGHTER, 0);
        ResumeStreetFightAudienceOpening(work->audienceTask);
        FTBL.EV_vfBattleUtil(1, 2, 1);
        FTBL.EV_SetPlayerFlag(12, 1792, 1);
        SetStreetFightStageFlag(work->stage);
        work->audienceResultPublished = 0;
        if (IsStreetFightBestOfThree()) {
            if (work->rank <= 1)
                index = 0;
            else if (work->rank <= 3)
                index = 1;
            else if (work->rank <= 6)
                index = 2;
            else if (work->rank <= 9)
                index = 3;
            else if (work->rank <= 12)
                index = 4;
            else if (work->rank <= 15)
                index = 5;
            FTBL.btlSetCharaFlagBatch(SFWF_FIGHTER, sfwfFighterParameters[index]);
        }
        work->state++;
        break;
    case 12:
        active = FTBL.IsFightActive();
        if (!active) {
            work->battleStatus = FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(SFWF_PLAYER));
            result = RecordStreetFightRound(work);
            if (IsStreetFightBestOfThree()) {
                if (result) {
                    FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
                    FTBL.spmFadeOut(0);
                    FTBL.HLib_SetupTaskDelay(30);
                    if (result == 1)
                        work->state = 23;
                    else
                        work->state = 13;
                } else {
                    FTBL.shenStartFGST(10, 10, 0, 0);
                    FTBL.vfmDispatchControl(2, 0, "QGBT", 1);
                    work->state = 7;
                }
            } else {
                switch (result) {
                case 1:
                    FTBL.spmFadeOut(0);
                    work->state++;
                    break;
                case 0:
                    work->state = 2;
                    break;
                case 2:
                    work->attempts++;
                    work->state++;
                    break;
                }
                FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
                FTBL.HLib_SetupTaskDelay(30);
                FTBL.EV_NPC_NewLoadSw_0(0);
            }
        } else if (!work->audienceResultPublished)
            switch (FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(SFWF_PLAYER))) {
            case 3:
                StartStreetFightAudienceDefeat(work->audienceTask, 0);
                work->audienceResultPublished++;
                break;
            case 2:
                StartStreetFightAudienceVictory(work->audienceTask, 0);
                work->audienceResultPublished++;
                break;
            }
        break;
    case 13:
        if (!FTBL.filmCheckFadeTrackColour()) {
            FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(SFWF_PLAYER), 1, 1, 1.0f);
            FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(SFWF_FIGHTER), 1, 1, 1.0f);
            SetStreetFightCharacterVisible(SFWF_EEN, 1);
            SetStreetFightCharacterVisible(SFWF_CYC, 1);
            SetStreetFightCharacterVisible(SFWF_SFA, 1);
            SetStreetFightCharacterVisible(SFWF_REN, 1);
            if (work->battleStatus != 3) {
                if (!IsStreetFightBestOfThree())
                    RecordStreetFightDefeat(work->stage);
                work->state = 24;
            } else
                work->state++;
        }
        break;
    case 14:
        FTBL.EV_SetPlayerFlag(12, 1292, 0);
        UnparentStreetFightCharacter(SFWF_PLAYER);
        UnparentStreetFightCharacter(work->fighters[work->fighterIndex]);
        EVT_AT_SetPos(SFWF_FIGHTER, STREET_FIGHT_INVALID, 334.5f, 84.25f, 80.72f, 0);
        EVT_AT_SetPos(SFWF_CYC, STREET_FIGHT_INVALID, 334.5f, 84.25f, 90.72f, 0);
        FTBL.EV_authSceneCtrl(-11);
        Queue_ET_PluralControl(sfwfVictoryPluralControl);
        QueueAuthor(SFWF_AUTHOR_2);
        work->state++;
        break;
    case 15:
        if (FTBL.EV_authSceneCtrl(-1))
            work->state++;
        break;
    case 16:
        if (!FTBL.EV_authSceneCtrl(-1)) {
            FTBL.EV_authSceneCtrl(-10);
            work->state++;
        } else if (FTBL.EV_authSceneCtrl(-6) == 730)
            FTBL.EventFunction875(2000, 2, 100);
        break;
    case 17:
        if (!FTBL.filmCheckFadeTrackColour()) {
            FTBL.AT_SetLocalBaseChara(SFWF_PLAYER, STREET_FIGHT_INVALID);
            FTBL.AT_SetLocalBaseChara(work->fighters[work->fighterIndex], STREET_FIGHT_INVALID);
            if (work->startParameters->subScene == 99)
                work->state = 23;
            else
                work->state++;
        }
        break;
    case 18:
        if (CanPerformForAreaFlags(g_currentMap)) {
            if (CommitStreetFightStoryRank(work->stage) == 2)
                work->state = 23;
            else
                work->state = 23;
        } else
            work->state = 23;
        break;
    case 23:
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(SFWF_PLAYER), 1);
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(SFWF_FIGHTER), 1);
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(SFWF_SFA), 1);
        if (IsStreetFightBestOfThree())
            FTBL.HLib_SetCurrentTaskTo(RetryBeginCallback);
        else
            FTBL.HLib_SetCurrentTaskTo(CleanupNormalCallback);
        break;
    case 24:
        if (!FTBL.filmCheckFadeTrackColour())
            work->state = 25;
        break;
    case 25:
        QueueStreetFightSkipWait(DefeatConversation, work, 1);
        work->state = 26;
        break;
    case 26:
        FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.spmFadeOut(0);
        ChangeStreetFightAudienceMode(work->audienceTask, 2, 1);
        work->state = 27;
        break;
    case 27:
        if (!FTBL.filmCheckFadeTrackColour()) {
            EVT_AT_CharaDisp(SFWF_PLAYER, 0, 0);
            if (!IsStreetFightBestOfThree()) {
                FTBL.EV_ResetFreeTalkFlags();
                work->state = 31;
            } else
                work->state = 23;
        }
        break;
    case 31:
        FTBL.Queue_ST_LoadTitleFadeIn(SFWF_RETRY_TITLE);
        work->state = 32;
        work->frameCounter = 60;
        break;
    case 32:
        if (!--work->frameCounter) {
            FTBL.ST_LoadTitleFade();
            work->frameCounter = 60;
            work->state = 33;
        }
        break;
    case 33:
        if (!--work->frameCounter)
            work->state = 23;
        break;
    case 28:
        UnparentStreetFightCharacter(SFWF_PLAYER);
        UnparentStreetFightCharacter(work->fighters[work->fighterIndex]);
        if (FTBL.EV_GetPlayerFlag(11, 1792))
            FTBL.EV_authSceneCtrl(-16);
        Queue_ET_PluralControl(sfwfPreviewPluralControl);
        QueueAuthor(SFWF_AUTHOR_0);
        work->state++;
        break;
    case 29:
        if (FTBL.EV_authSceneCtrl(-1))
            work->state++;
        break;
    case 30:
        if (!FTBL.EV_authSceneCtrl(-1))
            work->state = 17;
        break;
    }
}

int DefeatConversation(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_CharaDisp(SFWF_PLAYER, 0, 1);
    FTBL.EV_SetCharacterAlphaEnabled(fighter, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(fighter), 1, 1, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(fighter), 1, 0, 0);
    EVT_AT_SetPos(fighter, STREET_FIGHT_BASE, 0.0f, 0.1f, 0.0f, 0);
    EVT_AT_SetAngY(fighter, STREET_FIGHT_BASE, 0.0f, 0);
    EVT_AT_SetMot(fighter, 10945, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetVoice("0258E006", 41);
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 2);
    FTBL.EV_spmBeginSampleExecute(0);
    EVT_AT_SetCam(8099, STREET_FIGHT_BASE, STREET_FIGHT_BASE, 0, 0, 1);
    return 120;
}

void RetryBeginCallback(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    if (work->battleStatus == 3)
        RecordStreetFightVictory(work->stage);
    else
        RecordStreetFightDefeat(work->stage);
    FTBL.dbgPrintEventMessage("「リトライしますか？\n");
    EVT_AT_KsIcon_Enqueue(0xffff, 38, 39, 0xffff, 0x7fff, 0xffff, 2, 0);
    ChangeStreetFightAudienceMode(work->audienceTask, 2, 1);
    FTBL.HLib_SetCurrentTaskTo(RetryChoiceCallback);
}

void RetryChoiceCallback(void *data)
{
    unsigned int choice = PollStreetFightActionChoice();
    if (choice) {
        FTBL.ctrlStopSubtitles();
        if (choice == 38)
            RetryResetCallback(data);
        else
            RetryCancelCallback(data);
    }
}

void RetryResetCallback(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    FTBL.EV_ResetFreeTalkFlags();
    work->state = 1;
    work->battleStatus = 0;
    work->auxiliaryState = 0;
    FTBL.HLib_SetCurrentTaskTo(MainCallback);
    ChangeStreetFightAudienceMode(work->audienceTask, 2, -1);
    FTBL.EV_LoadAudio("0258A000", 2);
    FTBL.EV_spmBeginSampleExecute(0);
}

void RetryCancelCallback(void *data)
{
    FTBL.HLib_SetCurrentTaskTo(CleanupNormalCallback);
}

void NormalDebugCallback(void *data)
{
    CleanupNormalCallback(data);
}

void CleanupNormalCallback(void *data)
{
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    StreetFightStartParameters *start = work->startParameters;
    int returnToQron = work->returnToQron;
    int i;
    EventDebugUnregister(3);
    FTBL.vfmDispatchControl(20, 0, 0, 0);
    for (i = 0; sfwfCharacters[i] != SFWF_INVALID; i++)
        FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(sfwfCharacters[i]));
    FTBL.EV_SetCharaControllerFlags(SFWF_PLAYER, 2, 1);
    FTBL.EV_SetCharaControllerFlags(SFWF_PLAYER, 3, 1);
    FTBL.EV_SetCharaControllerFlags(SFWF_FIGHTER, 2, 1);
    FTBL.EV_SetCharaControllerFlags(SFWF_FIGHTER, 3, 1);
    SHENMUE_EVENT_BOUNDARY();
    FTBL.AT_SetLocalBaseChara(SFWF_STAG, STREET_FIGHT_INVALID);
    ReleaseStreetFightAudience(work->audienceTask);
    work->audienceTask = 0;
    ET_AT_KillWork(work->characterReadTask);
    FTBL.SetPlayerControlEnabled(0);
    FTBL.SetScenarioCharacterKillMode(STREET_FIGHT_BASE, 1);
    if (work->startParameters->subScene == 99) {
        FTBL.EV_AT_SetPos(SFWF_PLAYER, 0x78000000, &sfwfAuthorExitPosition);
        FTBL.EV_AT_SetAngY_0(SFWF_PLAYER, 0x78000000, &sfwfAuthorExitRotation, 0);
    } else {
        FTBL.EV_AT_SetPos(SFWF_PLAYER, 0x78000000, &sfwfExitPosition);
        FTBL.EV_AT_SetAngY_0(SFWF_PLAYER, 0x78000000, &sfwfExitRotation, 0);
    }
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(SFWF_PLAYER), 0xf002, 0);
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(SFWF_PLAYER), 71, 0, 0);
    FTBL.EV_EnableCharacterDisplay(SFWF_PLAYER);
    FTBL.GetNpcBatchCountOverride(work->previousNpcBatchCount);
    for (i = 0; work->motions[i]; i++)
        FTBL.DeleteMotion((void *)work->motions[i]);
    FTBL.EV_FreeAuthorPak(0, "0258");
    if (!(start->flags & 1))
        FTBL.EV_ControlPackageResources(0, 6, "QGBT", 0, 0);
    if (!IsStreetFightBestOfThree())
        FTBL.LoadCutsceneAndSoundProgramForArea();
    else
        FTBL.EV_spmBeginSampleExecute(0);
    FTBL.SetRenderClockAndCompassTo(0);
    FTBL.camCtrlSetFarDistance(work->previousFarClip);
    FTBL.camCtrlSetNearDistance(work->previousNearClip);
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    if (IsStreetFightBestOfThree())
        FTBL.EV_scenQueueLoad(4, SFWF_OMAKE_SCENE, 1);
    else if (returnToQron)
        FTBL.EV_scenQueueLoad(g_actNumber, SFWF_QRON_SCENE, 81);
    else {
        FTBL.camCtrlSetState(0);
        FTBL.camCtrlSetFovTransitionTo(1);
    }
    FTBL.EV_EventSetUpHelpIcon_0(0, 0);
    FTBL.EV_SetPlayerFlag(12, 1233, 1);
    if (start->signal)
        *start->signal = 0;
}
#endif
#else
#include "s2/areas/area_sfwf.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/event_call.hpp"
#include "s2/face.hpp"
#include "s2/hand.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/pause.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/conversation.hpp"
#include "s2/debug.hpp"
#include "s2/event_queued.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/flag.hpp"
#include "s2/help.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/sk_event.hpp"
#include "s2/street_fight_conversation.hpp"
#include "s2/street_fight_tables.hpp"
#include "s2/ui.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/step_event.hpp"
#include <cmath>

namespace shenmue::s2 {
namespace {
void PrepareAuthorActors() {
    EV_EnableCharacterDisplay(STREET_FIGHT_RYO);
    EV_SetCharaControllerFlags(STREET_FIGHT_RYO, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(STREET_FIGHT_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    mtwkSetExtraFlag(scenFindMotionwork(STREET_FIGHT_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, false);
    EV_EnableCharacterDisplay(SFWF_AUTHOR_PLAYER_DOUBLE);
    EV_SetCharaControllerFlags(SFWF_AUTHOR_PLAYER_DOUBLE, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(SFWF_AUTHOR_PLAYER_DOUBLE, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharacterAlphaEnabled(SFWF_AUTHOR_PLAYER_DOUBLE, false);
    EV_EnableCharacterDisplay(SFWF_AUTHOR_REN_DOUBLE);
    EV_SetCharaControllerFlags(SFWF_AUTHOR_REN_DOUBLE, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(SFWF_AUTHOR_REN_DOUBLE, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharacterAlphaEnabled(SFWF_AUTHOR_REN_DOUBLE, false);
    for (const shen_name *character = SFWF_CHARACTERS + 1; character->name != STREET_FIGHT_INVALID.name; ++character) {
        EV_EnableCharacterDisplay(*character);
        EV_SetCharaControllerFlags(*character, UpdateHeight, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(*character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}
void RestoreAuthorActors() {
    EV_EnableCharacterDisplay(STREET_FIGHT_RYO);
    mtwkSetExtraFlag(scenFindMotionwork(STREET_FIGHT_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, true);
    EV_DisableCharacterDisplay(SFWF_AUTHOR_PLAYER_DOUBLE);
    EV_DisableCharacterDisplay(SFWF_AUTHOR_REN_DOUBLE);
    for (const shen_name *character = SFWF_CHARACTERS + 1; character->name != STREET_FIGHT_INVALID.name; ++character) {
        EV_EnableCharacterDisplay(*character);
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}
void AuthorLoopCallback(void *taskData) {
    SfwfAuthorLoopWork *const work = static_cast<SfwfAuthorLoopWork *>(taskData);
    work->frame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (!work->frame)
        PrepareAuthorActors();
    if (work->parent->author.name == SFWF_AU0.name) {
        if (work->frame == EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining) - 30)
            spmFadeOut();
    } else if (work->parent->author.name == SFWF_AU1.name) {
        switch (work->frame) {
        case 744:
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_EEN), 0, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_EEN), 0, 0, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_FIGHTER), 0, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_FIGHTER), 0, 0, 0);
            SetFaceFlagsForChara_0(ctrlGetCharaByName(SFWF_EEN), FacialNodeOriginal);
            SetFaceFlagsForChara_0(ctrlGetCharaByName(SFWF_FIGHTER), FacialNodeOriginal);
            SetScenarioCharacterType(STREET_FIGHT_RYO, CharacterTypeDefault);
            SetScenarioCharacterType(SFWF_REN, CharacterTypeDefault);
            SetScenarioCharacterType(SFWF_EEN, CharacterTypeStencilShadow);
            SetScenarioCharacterType(SFWF_FIGHTER, CharacterTypeStencilShadow);
            break;
        case 874:
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_EEN), 1, 1, 0);
            ApplyCharacterHandPoseCommand(ctrlGetCharaByName(SFWF_EEN), 1, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_EEN), 0, 0, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_FIGHTER), 0, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_FIGHTER), 1, 0, 0);
            ApplyCharacterHandPoseCommand(ctrlGetCharaByName(SFWF_FIGHTER), 0, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
            SetHighModelXYZ(ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 0, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_REN), 0, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_REN), 0, 0, 0);
            SetFaceFlagsForChara_0(ctrlGetCharaByName(SFWF_EEN), FacialNodeReplacement);
            SetFaceFlagsForChara_0(ctrlGetCharaByName(STREET_FIGHT_RYO), FacialNodeOriginal);
            SetFaceFlagsForChara_0(ctrlGetCharaByName(SFWF_REN), FacialNodeOriginal);
            break;
        case 927:
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_FIGHTER), 0, 1, 0);
            break;
        case 1094:
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_REN), 1, 0, 0);
            SetFaceFlagsForChara_0(ctrlGetCharaByName(SFWF_REN), FacialNodeReplacement);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_FIGHTER), 1, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_FIGHTER), 1, 0, 0);
            ApplyCharacterHandPoseCommand(ctrlGetCharaByName(SFWF_FIGHTER), 1, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
            ApplyCharacterHandPoseCommand(ctrlGetCharaByName(SFWF_FIGHTER), 0, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
            break;
        case 1149:
            SetHighModelXYZ(ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 0, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_REN), 0, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_REN), 0, 0, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_EEN), 0, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_EEN), 0, 0, 0);
            ApplyCharacterHandPoseCommand(ctrlGetCharaByName(SFWF_FIGHTER), 1, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
            ApplyCharacterHandPoseCommand(ctrlGetCharaByName(SFWF_FIGHTER), 0, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
            SetFaceFlagsForChara_0(ctrlGetCharaByName(SFWF_EEN), FacialNodeOriginal);
            break;
        case 1150:
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_EEN), 0, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_EEN), 0, 0, 0);
            ApplyCharacterHandPoseCommand(ctrlGetCharaByName(SFWF_FIGHTER), 1, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
            ApplyCharacterHandPoseCommand(ctrlGetCharaByName(SFWF_FIGHTER), 0, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
            break;
        case 1439:
            SetHighModelXYZ(ctrlGetCharaByName(STREET_FIGHT_RYO), 1, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(STREET_FIGHT_RYO), 1, 0, 0);
            SetFaceFlagsForChara_0(ctrlGetCharaByName(STREET_FIGHT_RYO), FacialNodeReplacement);
            SetFaceFlagsForChara_0(ctrlGetCharaByName(SFWF_REN), FacialNodeReplacement);
            SetFaceFlagsForChara_0(ctrlGetCharaByName(SFWF_FIGHTER), FacialNodeReplacement);
            SetFaceFlagsForChara_0(ctrlGetCharaByName(SFWF_EEN), FacialNodeReplacement);
            ApplyCharacterHandPoseCommand(ctrlGetCharaByName(STREET_FIGHT_RYO), 1, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
            ApplyCharacterHandPoseCommand(ctrlGetCharaByName(STREET_FIGHT_RYO), 0, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_REN), 1, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_REN), 1, 0, 0);
            break;
        case 1899:
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_FIGHTER), 0, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_FIGHTER), 0, 0, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_EEN), 0, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_EEN), 0, 0, 0);
            break;
        case 2273:
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_FIGHTER), 0, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_FIGHTER), 0, 0, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_EEN), 0, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_EEN), 0, 0, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_REN), 0, 1, 0);
            SetFaceFlagsForChara_0(ctrlGetCharaByName(SFWF_REN), FacialNodeOriginal);
            SetScenarioCharacterType(STREET_FIGHT_RYO, CharacterTypeStencilShadow);
            break;
        case 2321:
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_REN), 0, 0, 0);
            break;
        case 2422:
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_REN), 1, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_REN), 1, 0, 0);
            SetFaceFlagsForChara_0(ctrlGetCharaByName(SFWF_REN), FacialNodeReplacement);
            break;
        case 2607:
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_FIGHTER), 1, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_FIGHTER), 1, 0, 0);
            SetScenarioCharacterType(SFWF_EEN, CharacterTypeDefault);
            break;
        case 2809:
            SetHighModelXYZ(ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 0, 0);
            SetScenarioCharacterType(STREET_FIGHT_RYO, CharacterTypeDefault);
            SetFaceFlagsForChara_0(ctrlGetCharaByName(STREET_FIGHT_RYO), FacialNodeOriginal);
            SetFaceFlagsForChara_0(ctrlGetCharaByName(SFWF_FIGHTER), FacialNodeOriginal);
            break;
        default:
            break;
        }
    } else if (work->parent->author.name == SFWF_AU2.name) {
        switch (work->frame) {
        case 646:
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_FIGHTER), 0, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_FIGHTER), 0, 0, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_REN), 0, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_REN), 0, 0, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_CHN), 0, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_CHN), 0, 0, 0);
            SetFaceFlagsForChara_0(ctrlGetCharaByName(SFWF_FIGHTER), FacialNodeOriginal);
            SetFaceFlagsForChara_0(ctrlGetCharaByName(SFWF_EEN), FacialNodeOriginal);
            SetFaceFlagsForChara_0(ctrlGetCharaByName(SFWF_REN), FacialNodeOriginal);
            SetFaceFlagsForChara_0(ctrlGetCharaByName(SFWF_CHN), FacialNodeOriginal);
            SetScenarioCharacterType(STREET_FIGHT_RYO, CharacterTypeDefault);
            SetScenarioCharacterType(SFWF_REN, CharacterTypeStencilShadow);
            SetScenarioCharacterType(SFWF_EEN, CharacterTypeDefault);
            SetScenarioCharacterType(SFWF_FIGHTER, CharacterTypeDefault);
            SetScenarioCharacterType(SFWF_CHN, CharacterTypeDefault);
            break;
        case 726:
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_EEN), 0, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_EEN), 0, 0, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_FIGHTER), 0, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_FIGHTER), 0, 0, 0);
            SetHighModelXYZ(ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 0, 0);
            break;
        case 906:
            ApplyCharacterHandPoseCommand(ctrlGetCharaByName(STREET_FIGHT_RYO), 1, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
            ApplyCharacterHandPoseCommand(ctrlGetCharaByName(STREET_FIGHT_RYO), 0, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_REN), 1, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_REN), 1, 0, 0);
            SetFaceFlagsForChara_0(ctrlGetCharaByName(SFWF_REN), FacialNodeReplacement);
            break;
        case 999:
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_EEN), 0, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_EEN), 0, 0, 0);
            SetFaceFlagsForChara_0(ctrlGetCharaByName(SFWF_EEN), FacialNodeOriginal);
            break;
        case 1109:
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_REN), 0, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_REN), 0, 0, 0);
            break;
        case 1400:
            SetHighModelXYZ(ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 0, 0);
            break;
        case 1480:
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_REN), 1, 1, 0);
            SetHighModelXYZ(ctrlGetCharaByName(SFWF_REN), 1, 0, 0);
            ApplyCharacterHandPoseCommand(ctrlGetCharaByName(SFWF_REN), 1, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
            ApplyCharacterHandPoseCommand(ctrlGetCharaByName(SFWF_REN), 0, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
            break;
        default:
            break;
        }
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        RestoreAuthorActors();
        work->parent->state = 3;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::DestroyCurrentTask();
    }
}
void QueueAuthorLoop(SfwfAuthorWork *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AuthorLoopCallback, 4, 11, sizeof(SfwfAuthorLoopWork), STREET_FIGHT_EVENT_TASK);
    SfwfAuthorLoopWork *const work = static_cast<SfwfAuthorLoopWork *>(HLib::GetTaskParameter(task));
    work->frame = 0;
    work->parent = parent;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->author.name));
    parent->state = 2;
}
void AuthorMainCallback(void *taskData) {
    SfwfAuthorWork *const work = static_cast<SfwfAuthorWork *>(taskData);
    if (work->state == 1)
        QueueAuthorLoop(work);
    else if (work->state == 3)
        HLib::DestroyTask(work->task);
}
void QueueAuthor(shen_name author) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AuthorMainCallback, 4, 11, sizeof(SfwfAuthorWork), STREET_FIGHT_EVENT_TASK);
    SfwfAuthorWork *const work = static_cast<SfwfAuthorWork *>(HLib::GetTaskParameter(task));
    work->state = 1;
    work->task = task;
    work->author = author;
    SetPlayerControlEnabled(0);
}
std::int32_t choicePhase{};
void MainCallback(void *);
void CleanupNormal(void *);
void RetryChoice(void *);
std::uint32_t PollChoice() {
    std::uint16_t choice = 0;
    switch (choicePhase) {
    case 0:
        if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            ++choicePhase;
        break;
    case 1:
        if (!GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            ++choicePhase;
        break;
    case 2:
        choice = static_cast<std::uint16_t>(shenGetCtrlSpeakIconFlag());
        choicePhase = 0;
        break;
    default:
        break;
    }
    return choice;
}
void PlaceNormal(StreetFightEventWork *work) {
    EV_EnableCharacterDisplay(SFWF_FIGHTER);
    EV_EnsureCharacterDraw(SFWF_FIGHTER);
    work->playerPosition = {-3.3f, -.5f, -5.5f};
    work->bookiePosition = {-1.864f, 0, -1.8f};
    EventSetPosition position{.cleanup = 0, .character = SFWF_BASE, .parent = STREET_FIGHT_INVALID, .position = SFWF_STAGE_ORIGIN, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = SFWF_BASE, .parent = STREET_FIGHT_INVALID, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFWF_BASE);
    AT_SetLocalBaseChara(SFWF_FIGHTER, SFWF_BASE);
    AT_SetLocalBaseChara(SFWF_STAG, SFWF_BASE);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 306, 0, 1, 0x40000000U, 0);
    const shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetMot(fighter, 2901, 0, -1, 0x40000000U, 0);
    position = {.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFWF_BASE, .position = {0, 0, -1.1f}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFWF_BASE, .degrees = 180, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = fighter, .parent = SFWF_BASE, .position = {0, 0, 1.1f}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = fighter, .parent = SFWF_BASE, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    vfmDispatchControl(FightControlOp::GaugeResources, {}, 0, 1);
    LoadBlurModelForCharacter(STREET_FIGHT_RYO, "RYO_B.MT7", 1);
    LoadBlurModelForCharacter(SFWF_FIGHTER, "SF6_B.MT7", 1);
    for (const shen_name *character = SFWF_CHARACTERS; character->name != STREET_FIGHT_INVALID.name; ++character)
        SetLowQualityHands(ctrlGetCharaByName(*character));
    adfsSetCameraIdxForChara(SFWF_EEN, SHEN_NAME(damB));
}
std::int32_t DefeatConversation(void *argument) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(argument);
    const shen_name fighter = work->fighters[work->fighterIndex];
    EV_DisableCharacterDisplay(STREET_FIGHT_RYO);
    EV_SetCharacterAlphaEnabled(fighter, false);
    SetHighModelXYZ(ctrlGetCharaByName(fighter), 1, 1, 0);
    SetHighModelXYZ(ctrlGetCharaByName(fighter), 1, 0, 0);
    EventSetPosition position{.cleanup = 0, .character = fighter, .parent = SFWF_BASE, .position = {0, .1f, 0}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = fighter, .parent = SFWF_BASE, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    EVT_AT_SetMot(fighter, 10945, 0, 1, 0x40000000U, 0);
    EVT_AT_SetVoice("0258E006", 1, 41);
    EVT_AT_MenCtrl({.cleanup = 1,
        .frames = 30,
        .startAlpha = 255,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 0,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 2});
    EV_spmBeginSampleExecute();
    EVT_AT_SetCam(8099, SFWF_BASE, SFWF_BASE, 0, 0, 1);
    return 120;
}
void RetryNormal(void *argument) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(argument);
    if (work->battleStatus == 3)
        RecordStreetFightVictory(work->stage);
    else
        RecordStreetFightDefeat(work->stage);
    dbgPrintEventMessage(GetLocalizedSceneText("UISMRETRY").data()); // @text EN: "Retry?" | JA: "もう一度？"
    std::int16_t choices[]{-1, 82, 83, -1};
    SetupControlSpeakAndUI(choices, 0x7fff, -1, 2);
    if (work->audienceTask) {
        StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work->audienceTask));
        if (audience)
            audience->mode |= 2;
    }
    HLib::SetCurrentTaskTo(RetryChoice);
}
void RetryChoice(void *argument) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(argument);
    const std::uint16_t choice = static_cast<std::uint16_t>(PollChoice());
    if (!choice)
        return;
    ctrlStopSubtitles();
    if (choice == 82) {
        EV_ResetFreeTalkFlags();
        work->state = 1;
        work->battleStatus = 0;
        work->auxiliaryState = 0;
        HLib::SetCurrentTaskTo(MainCallback);
        if (work->audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work->audienceTask));
            if (audience)
                audience->mode &= ~2U;
        }
        EV_LoadAudio("0258A000", 2);
        EV_spmBeginSampleExecute();
    } else
        HLib::SetCurrentTaskTo(CleanupNormal);
}
void CleanupNormal(void *argument) {
    using enum CameraState;
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(argument);
    StreetFightStartParameters *const parameters = work->startParameters;
    const std::int32_t exitRequested = work->reserved178;
    vfmDispatchControl(FightControlOp::GaugeResources, {}, 0, 0);
    for (const shen_name *character = SFWF_CHARACTERS; character->name != STREET_FIGHT_INVALID.name; ++character)
        hndlSetFlags(ctrlGetCharaByName(*character));
    EV_SetCharaControllerFlags(STREET_FIGHT_RYO, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(STREET_FIGHT_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(SFWF_FIGHTER, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(SFWF_FIGHTER, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    AT_SetLocalBaseChara(SFWF_STAG, STREET_FIGHT_INVALID);
    if (work->audienceTask) {
        HLib::GetTaskParameter(work->audienceTask);
        HLib::DestroyTaskInternal(work->audienceTask);
    }
    work->audienceTask = nullptr;
    ET_AT_KillWork(work->characterReadTask);
    SetPlayerControlEnabled(0);
    SetCharaKill(SFWF_BASE, 1);
    EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes,
        parameters->reserved08 == 99 ? SFWF_NORMAL_EXIT_POSITION : SFWF_DEBUG_EXIT_POSITION);
    EV_AT_SetAngY(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes,
        parameters->reserved08 == 99 ? SFWF_NORMAL_EXIT_ROTATION : SFWF_DEBUG_EXIT_ROTATION);
    mtwkRequestMotion(scenFindMotionwork(STREET_FIGHT_RYO), 61442, MotionRegion::LowerBody);
    mtwkSetRegionFlag(scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, MotionRegion::LowerBody);
    EV_EnableCharacterDisplay(STREET_FIGHT_RYO);
    GetNpcBatchCountOverride(work->previousNpcBatchCount);
    for (std::int32_t *motion = work->motions; *motion; ++motion)
        DeleteMotion(*motion);
    EV_FreeAuthorPak(0, "0258");
    if (!(parameters->flags & 1U))
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QGBT"), 0);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        EV_spmBeginSampleExecute();
    else
        sbtLoadProgram();
    shenSetClockAndCompassRenderTo(0);
    camCtrlSetFarDistance(work->previousFarClip);
    camCtrlSetNearDistance(work->previousNearClip);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->task);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        EV_scenQueueLoad(4, SFWF_MINIGAME_AREA, 1);
    else if (exitRequested)
        EV_scenQueueLoad(g_actNumber, SFWF_EXIT_AREA, 81);
    else {
        camCtrlSetState(Init);
        camCtrlSetFovTransitionTo(true);
    }
    EV_EventSetUpHelpIcon_0(STREET_FIGHT_HELP_FREE, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1233, 1);
    if (parameters->signal)
        *parameters->signal = 0;
}
void MainCallback(void *argument) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(argument);
    const shen_name fighter = work->fighters[work->fighterIndex];
    switch (work->state) {
    case 0:
        PlaceNormal(work);
        SetMapControlStatusFlag(0, 0);
        if (!work->audienceTask) {
            work->audienceTask = QueueStreetFightAudienceCount(30);
            if (work->audienceTask) {
                StreetFightAudienceWork *audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work->audienceTask));
                audience->fighters[audience->fighterCount++] = STREET_FIGHT_RYO;
            }
            if (work->audienceTask) {
                StreetFightAudienceWork *audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work->audienceTask));
                audience->fighters[audience->fighterCount++] = SFWF_FIGHTER;
            }
        }
        ++work->state;
        return;
    case 1:
        SetHighModelXYZ(ctrlGetCharaByName(fighter), 0, 1, 0);
        SetHighModelXYZ(ctrlGetCharaByName(fighter), 0, 0, 0);
        work->round = work->wins = work->losses = 0;
        work->roundResults[0] = work->roundResults[1] = work->roundResults[2] = 0;
        ++work->state;
        return;
    case 2: {
        if (work->startParameters->reserved08 != 99) {
            EV_EventSetUpHelpIcon_0(GetHelpIconForArea(), 0);
            shenStartFGST(10, 10, 0, 0);
            vfmDispatchControl(FightControlOp::Package, {}, reinterpret_cast<std::uintptr_t>("QGBT"), 1);
        }
        Vector3 stage;
        EV_GetCharaPosition(SFWF_BASE, CHARACTER_TRANSLATION_ROOT, &stage, EventTransformFlags::Local);
        if (work->audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work->audienceTask));
            audience->soundCentre = stage;
            audience->state = 2;
        }
        EV_NPC_NewLoadSw(1);
        ++work->state;
        return;
    }
    case 3:
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
            work->state = 7;
        else if (work->startParameters->reserved08 == 99)
            work->state = 28;
        else if (!work->round)
            ++work->state;
        else
            work->state = 7;
        return;
    case 4:
        UnparentAndSetExistingPosRot(STREET_FIGHT_RYO);
        UnparentAndSetExistingPosRot(fighter);
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1792))
            EV_authSceneCtrl(AuthSceneCtrl::UnusedAuthorFlag);
        if (IsAuthorStateFlagSet(15))
            EV_SetAuthorActiveTo(1);
        QueueAuthoredPluralControl(SFWF_FIRST_AUTHOR_CONTROL);
        QueueAuthor(SFWF_AU1);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 0);
        ++work->state;
        return;
    case 5:
    case 15:
    case 29:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            ++work->state;
        return;
    case 6:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            ++work->state;
        return;
    case 7: {
        EVT_AT_SetMot(STREET_FIGHT_RYO, 306, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(fighter, 2901, 0, -1, 0x40000000U, 0);
        EVT_AT_SetMot(SFWF_REN, 61443, 0, 1, 0x40000000U, 0);
        EV_AT_SetPos(SFWF_EEN, EventTransformFlags::World | EventTransformFlags::AllAxes, SFWF_EEN_POSITION);
        EV_AT_SetAngY(SFWF_EEN, EventTransformFlags::World | EventTransformFlags::AllAxes, SFWF_EEN_ROTATION);
        EVT_AT_SetMot(SFWF_EEN, 61443, 0, 1, 0x40000000U, 0);
        EventSetPosition position{.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFWF_BASE, .position = {0, 0, -1.1f}, .delay = 0};
        EVT_AT_SetPosRot(&position);
        EventSetAngle angle{.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFWF_BASE, .degrees = 180, .delay = 0};
        EVT_AT_SetAngY_Callback(&angle);
        position = {.cleanup = 0, .character = fighter, .parent = SFWF_BASE, .position = {0, 0, 1.1f}, .delay = 0};
        EVT_AT_SetPosRot(&position);
        angle = {.cleanup = 0, .character = fighter, .parent = SFWF_BASE, .degrees = 0, .delay = 0};
        EVT_AT_SetAngY_Callback(&angle);
        SetScenarioCharacterType(STREET_FIGHT_RYO, CharacterTypeStencilShadow);
        SetScenarioCharacterType(SFWF_FIGHTER, CharacterTypeStencilShadow);
        EV_SetCharaControllerFlags(STREET_FIGHT_RYO, ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(STREET_FIGHT_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(SFWF_FIGHTER, ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(SFWF_FIGHTER, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
        SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(STREET_FIGHT_RYO), 1, 1, 1);
        SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(SFWF_FIGHTER), 1, 1, 1);
        SetPluralControlFor(ctrlGetCharaByName(STREET_FIGHT_RYO), 1);
        SetPluralControlFor(ctrlGetCharaByName(SFWF_FIGHTER), 1);
        EV_DisableCharacterDisplay(SFWF_EEN);
        EV_ToggleCharacterFlag(SFWF_EEN);
        EV_DisableCharacterDisplay(SFWF_CYC);
        EV_ToggleCharacterFlag(SFWF_CYC);
        EV_DisableCharacterDisplay(SFWF_SFA);
        EV_ToggleCharacterFlag(SFWF_SFA);
        EV_DisableCharacterDisplay(SFWF_REN);
        EV_ToggleCharacterFlag(SFWF_REN);
        work->rank =
            static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{static_cast<std::uint32_t>(work->stage | 0x40)}, 0, -1));
        std::int32_t difficulty = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{140}, 0, -1));
        if (difficulty > 2)
            difficulty = 2;
        shenInitFightData(ctrlGetCharaByName(STREET_FIGHT_RYO), nullptr, 0, 0, 300, 0);
        const std::uint16_t hitPoints =
            static_cast<std::uint16_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) ? 80 * work->rank + 300 : 100 * (5 - difficulty));
        shenInitFightData(ctrlGetCharaByName(SFWF_FIGHTER), nullptr, 1, 1, hitPoints, 24);
        vfUpdateFlagsFor(ctrlGetCharaByName(SFWF_FIGHTER), 0, 0x8000000U, difficulty);
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014)) {
            const std::int32_t count = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{76}, 0, -1));
            vfmDispatchControl(FightControlOp::StreetFightLevel, {}, static_cast<std::uintptr_t>(count + 1), 1);
        }
        vfCopyPosFromEntity(0, nullptr, SFWF_STAGE_ORIGIN.x, SFWF_STAGE_ORIGIN.z, 2.5f);
        vfmDispatchControl(FightControlOp::RoundCounts, shen_name{static_cast<std::uint32_t>(work->wins)}, static_cast<std::uintptr_t>(work->losses), 1);
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x8000}, 1);
        work->fadeFrames = 30;
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 1);
        EventFilmControl film{.cleanup = 0,
            .frames = work->fadeFrames,
            .startAlpha = 255,
            .startRed = 0,
            .startGreen = 0,
            .startBlue = 0,
            .targetAlpha = 0,
            .targetRed = 0,
            .targetGreen = 0,
            .targetBlue = 0,
            .delay = 0};
        EVT_AT_MenCtrl_Callback(&film);
        EventSetCamera camera{
            .cleanup = 0, .camera = 1400, .eye = STREET_FIGHT_RYO, .target = STREET_FIGHT_RYO, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
        EVT_AT_SetCam_Callback(&camera);
        camCtrlDisableFilmBars();
        ++work->state;
        return;
    }
    case 8:
        EV_LoadAudio("0258A000", 2);
        EV_spmBeginSampleExecute();
        HLib::SetupTaskDelay(work->fadeFrames);
        ++work->state;
        return;
    case 9: {
        EVT_AT_SetVoice("0258G001", 1, 30);
        if (work->round >= 0 && work->round <= 2) {
            HLib::Task *const task = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 11, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
            SkEventMessageWork *const message = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(task));
            message->frames = 60;
            message->message = work->round == 0 ? SFWF_FIRST_ROUND_TEXT : work->round == 1 ? SFWF_SECOND_ROUND_TEXT : SFWF_FINAL_ROUND_TEXT;
        }
        UnparentAndSetExistingPosRot(STREET_FIGHT_RYO);
        UnparentAndSetExistingPosRot(fighter);
        HLib::SetupTaskDelay(10);
        if (work->audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work->audienceTask));
            if (audience) {
                audience->state = 4;
                audience->loopDelay = 0;
            }
        }
        ++work->state;
        work->audienceResultPublished = 0;
        return;
    }
    case 10:
        if (!camCtrlCheckIsEventCam()) {
            EV_PlayDialogue("0258G002");
            HLib::SetupTaskDelay(10);
            ++work->state;
        }
        return;
    case 11: {
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800}, 1);
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x80000}, 1);
        vfmQueueFightStart(2, 99, 1, 8, -1);
        EV_SetCharacterAlphaEnabled(SFWF_FIGHTER, false);
        if (work->audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work->audienceTask));
            if (audience) {
                audience->state = 4;
                audience->loopDelay = 0;
            }
        }
        EV_vfBattleUtil(FightBattleUtilOp::CombatCameraFlags, shen_name{2}, 1);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1792, 1);
        SetCurrentGameFlag(static_cast<std::uint8_t>(GetStreetFightStageFlag(work->stage)), 1);
        work->audienceResultPublished = 0;
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014)) {
            unsigned index = 0;
            if (work->rank > 1) {
                if (work->rank <= 3)
                    index = 1;
                else if (work->rank <= 6)
                    index = 2;
                else if (work->rank <= 9)
                    index = 3;
                else if (work->rank <= 12)
                    index = 4;
                else if (work->rank <= 15)
                    index = 5;
            }
            SetFightActorParameterBytes(SFWF_FIGHTER, SFWF_FIGHTER_PARAMETERS[index]);
        }
        ++work->state;
        return;
    }
    case 12:
        if (IsFightActive()) {
            if (!work->audienceResultPublished) {
                const std::uint32_t status = GetBtlStatus(ctrlGetCharaByName(STREET_FIGHT_RYO));
                if (status == 2 || status == 3) {
                    if (work->audienceTask) {
                        StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work->audienceTask));
                        if (audience) {
                            audience->state = status == 2 ? 7 : 6;
                            audience->loopDelay = status == 2 ? 1 : 2;
                        }
                    }
                    ++work->audienceResultPublished;
                }
            }
        } else {
            work->battleStatus = GetBtlStatus(ctrlGetCharaByName(STREET_FIGHT_RYO));
            const std::int32_t result = static_cast<std::int32_t>(RecordStreetFightRound(work));
            if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014)) {
                if (result) {
                    SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
                    spmFadeOut();
                    HLib::SetupTaskDelay(30);
                    work->state = result == 1 ? 23 : 13;
                } else {
                    shenStartFGST(10, 10, 0, 0);
                    vfmDispatchControl(FightControlOp::Package, {}, reinterpret_cast<std::uintptr_t>("QGBT"), 1);
                    work->state = 7;
                }
            } else {
                if (result == 2) {
                    ++work->attempts;
                    ++work->state;
                } else if (result == 1) {
                    spmFadeOut();
                    ++work->state;
                } else if (!result)
                    work->state = 2;
                SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
                HLib::SetupTaskDelay(30);
                EV_NPC_NewLoadSw(0);
            }
        }
        return;
    case 13:
        if (!filmCheckFadeTrackColour()) {
            SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(STREET_FIGHT_RYO), 1, 1, 1);
            SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(SFWF_FIGHTER), 1, 1, 1);
            EV_EnableCharacterDisplay(SFWF_EEN);
            EV_EnsureCharacterDraw(SFWF_EEN);
            EV_EnableCharacterDisplay(SFWF_CYC);
            EV_EnsureCharacterDraw(SFWF_CYC);
            EV_EnableCharacterDisplay(SFWF_SFA);
            EV_EnsureCharacterDraw(SFWF_SFA);
            EV_EnableCharacterDisplay(SFWF_REN);
            EV_EnsureCharacterDraw(SFWF_REN);
            if (work->battleStatus == 3)
                ++work->state;
            else {
                if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
                    RecordStreetFightDefeat(work->stage);
                work->state = 24;
            }
        }
        return;
    case 14: {
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1292, 0);
        UnparentAndSetExistingPosRot(STREET_FIGHT_RYO);
        UnparentAndSetExistingPosRot(fighter);
        EventSetPosition position{
            .cleanup = 0, .character = SFWF_FIGHTER, .parent = STREET_FIGHT_INVALID, .position = SFWF_VICTORY_FIGHTER_POSITION, .delay = 0};
        EVT_AT_SetPosRot(&position);
        position = {.cleanup = 0, .character = SFWF_CHN, .parent = STREET_FIGHT_INVALID, .position = SFWF_VICTORY_CHN_POSITION, .delay = 0};
        EVT_AT_SetPosRot(&position);
        EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
        QueueAuthoredPluralControl(SFWF_VICTORY_AUTHOR_CONTROL);
        QueueAuthor(SFWF_AU2);
        ++work->state;
        return;
    }
    case 16:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 730)
                AddCashOrCoinsWithStep(2000, 2, 100);
        } else {
            EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
            ++work->state;
        }
        return;
    case 17:
        if (!filmCheckFadeTrackColour()) {
            AT_SetLocalBaseChara(STREET_FIGHT_RYO, STREET_FIGHT_INVALID);
            AT_SetLocalBaseChara(fighter, STREET_FIGHT_INVALID);
            if (work->startParameters->reserved08 == 99)
                work->state = 23;
            else
                ++work->state;
        }
        return;
    case 18:
        if (CanPerformForCurrentAreaFlags())
            CommitStreetFightStoryRank(work->stage);
        work->state = 23;
        return;
    case 23:
        SetPluralControlFor(ctrlGetCharaByName(STREET_FIGHT_RYO), 1);
        SetPluralControlFor(ctrlGetCharaByName(SFWF_FIGHTER), 1);
        SetPluralControlFor(ctrlGetCharaByName(SFWF_SFA), 1);
        HLib::SetCurrentTaskTo(EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) ? RetryNormal : CleanupNormal);
        return;
    case 24:
        if (!filmCheckFadeTrackColour())
            work->state = 25;
        return;
    case 25:
        if (IsAuthorStateFlagSet(15)) {
            sfwfConversationSkipEnabled = 1;
            sfwfConversationSkipCallback = nullptr;
        }
        SetAuthorStateFlag(15, 1);
        QueueSfwfStreetFightConversation(DefeatConversation, work);
        work->state = 26;
        return;
    case 26:
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        if (work->audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work->audienceTask));
            if (audience)
                audience->mode |= 2;
        }
        work->state = 27;
        return;
    case 27:
        if (!filmCheckFadeTrackColour()) {
            EV_EnableCharacterDisplay(STREET_FIGHT_RYO);
            if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
                work->state = 23;
            else {
                EV_ResetFreeTalkFlags();
                work->state = 31;
            }
        }
        return;
    case 28:
        UnparentAndSetExistingPosRot(STREET_FIGHT_RYO);
        UnparentAndSetExistingPosRot(fighter);
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1792))
            EV_authSceneCtrl(AuthSceneCtrl::UnusedAuthorFlag);
        QueueAuthoredPluralControl(SFWF_PREVIEW_AUTHOR_CONTROL);
        QueueAuthor(SFWF_AU0);
        ++work->state;
        return;
    case 30:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work->state = 17;
        return;
    case 31:
        Queue_ST_LoadTitleFadeIn(SFWF_RETRY_TITLE);
        work->state = 32;
        work->frameCounter = 60;
        return;
    case 32:
        if (work->frameCounter-- == 1) {
            ST_LoadTitleFade();
            work->frameCounter = 60;
            work->state = 33;
        }
        return;
    case 33:
        if (work->frameCounter-- == 1)
            work->state = 23;
        return;
    default:
        return;
    }
}
StreetFightEventWork *InitializeNormal(HLib::Task *task) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->state = 0;
    work->battleStatus = 0;
    work->previousNpcBatchCount = GetNpcBatchCountOverride(35);
    work->fighterIndex = 0;
    work->fighters[0] = SFWF_FIGHTER;
    work->fighters[1] = STREET_FIGHT_INVALID;
    work->characterReadTask = Queue_EVT_AT_ReadChr(task, SFWF_CHARACTERS);
    work->audienceTask = nullptr;
    work->fadeFrames = 0;
    work->attempts = 0;
    work->audienceResultPublished = 0;
    work->stage = GetStreetFightStageForCurrentArea();
    work->rank = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{static_cast<std::uint32_t>(work->stage | 0x40)}, 0, -1));
    work->frameCounter = 60;
    return work;
}
void LoadNormalResources(StreetFightEventWork *work) {
    if (IsFightActive())
        check_dead_status(1);
    work->motions[0] = 0;
    EV_FreeAuthorPak(1, "0258");
    if (!EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QGBT"), 0)) {
        const char *const path = Filepath_Gen(FilepathGenMode::SceneDirectory);
        EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("QGBT"));
        EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    }
    EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QGBT"), 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QGBT"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, SFWF_BASE);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0258A000");
    work->previousFarClip = camCtrlGetFarDistance();
    work->previousNearClip = camCtrlGetNearDistance();
    FinishLoading();
    shenSetClockAndCompassRenderTo(1);
}
void QueueNormal(StreetFightStartParameters *parameters) {
    if (parameters->signal)
        *parameters->signal = 1;
    SetPlayerControlEnabled(0);
    SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(MainCallback, 5, 11, sizeof(StreetFightEventWork), STREET_FIGHT_EVENT_TASK);
    StreetFightEventWork *const work = InitializeNormal(task);
    work->startParameters = parameters;
    work->reserved178 = 0;
    LoadNormalResources(work);
}
std::int32_t DebugInsufficientConversation(void *) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_StartMoneyCount_Callback, 6, 11, sizeof(EventMoneyCount), STREET_FIGHT_EVENT_TASK);
    RegisterEventTask(task);
    EventMoneyCount *const work = static_cast<EventMoneyCount *>(HLib::GetTaskParameter(task));
    work->cleanup = 1;
    work->delay = 60;
    EV_PlayDialogue("0100A037");
    EVT_AT_MenCtrl({.cleanup = 1,
        .frames = 30,
        .startAlpha = 0,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 255,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 120});
    return 150;
}
std::int32_t debugOpponentCount{};
shen_name debugLoadCharacters[2]{};
void DebugMainCallback(void *);
void ReadBookiePosition(shen_name character) {
    Vector3 position;
    EV_GetCharaPosition(character, 5, &position, EventTransformFlags::World);
}
void DebugBattleEntryCallback(void *);
void DebugBattlePreEventCallback(void *);
void DebugBattlePreInitCallback(void *);
void DebugBattleStartCallback(void *);
void DebugBattleReadyGoCallback(void *);
void DebugBattleStartFightCallback(void *);
void DebugBattleWaitCallback(void *);
void DebugBattleWaitFadeCallback(void *);
void DebugBattleEndCallback(void *);
std::int32_t DebugContinueConversation(void *);
std::int32_t DebugCompletedConversation(void *);
std::int32_t DebugStopConversation(void *);

std::int64_t QueueDebugCharacterLoad(HLib::Task *parent) {
    HLib::Task *const task =
        HLib::EnqueueTaskWithParameter(StreetFightCharacterLoadCallback, 6, 0x0b, sizeof(StreetFightCharacterLoadWork), STREET_FIGHT_EVENT_TASK);
    StreetFightCharacterLoadWork *const work = static_cast<StreetFightCharacterLoadWork *>(HLib::GetTaskParameter(task));
    HLib::SetupTimedTask(parent, 0);
    work->task = task;
    work->characters = debugLoadCharacters;
    work->parent = parent;
    work->index = 0;
    work->state = 0;
    return 0;
}

StreetFightEventWork *InitializeDebug(HLib::Task *task) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->state = 0;
    work->battleStatus = 0;
    work->fighterIndex = 0;
    work->fighters[0] = STREET_FIGHT_INVALID;
    work->package = SFWF_DEBUG_PACKAGE;
    work->battleFrames = 50;
    work->auxiliaryState = 0;
    work->previousNpcBatchCount = GetNpcBatchCountOverride(30);
    work->audienceTask = nullptr;
    work->completedOpponents = 0;
    work->hudEnabled = 0;
    work->fighterIndex = 0;
    const StreetFightFighterDefinition *const definition = GetStreetFightDebugFighterDefinition();
    debugOpponentCount = 5;
    work->fighterDefinitions[0] = definition;
    work->fighters[0] = definition->character;
    work->characterReadTask = Queue_EVT_AT_ReadChr(task, SFWF_DEBUG_CHARACTERS);
    return work;
}

void LoadDebugResources(StreetFightEventWork *work) {
    work->motions[0] = 0;
    EV_FreeAuthorPak(1, "0100");
    const char *path = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("ENMY"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    path = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("QGBT"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QGBT"), 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QGBT"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, SFWF_BASE);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0100E000");
    EV_LoadAudio("0100E000", 0);
    FinishLoading();
    shenSetClockAndCompassRenderTo(1);
}

void QueueDebug(StreetFightStartParameters *parameters) {
    if (parameters->signal)
        *parameters->signal = 1;
    SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(DebugMainCallback, 5, 0x0b, sizeof(StreetFightEventWork), STREET_FIGHT_EVENT_TASK);
    StreetFightEventWork *const work = InitializeDebug(task);
    work->startParameters = parameters;
    LoadDebugResources(work);
}

void PlaceDebugCharacters(StreetFightEventWork *work) {
    const shen_name fighter = work->fighters[work->fighterIndex];
    EV_EnableCharacterDisplay(SFWF_FUY);
    EV_EnsureCharacterDraw(SFWF_FUY);
    EV_EnableCharacterDisplay(fighter);
    EV_EnsureCharacterDraw(fighter);
    work->playerPosition = {-3.3f, -0.5f, -5.5f};
    work->bookiePosition = {-1.864f, 0.0f, -1.8f};
    EventSetPosition position{.cleanup = 0, .character = SFWF_BASE, .parent = STREET_FIGHT_INVALID, .position = SFWF_STAGE_ORIGIN, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = SFWF_BASE, .parent = STREET_FIGHT_INVALID, .degrees = 0.0f, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFWF_BASE);
    AT_SetLocalBaseChara(SFWF_FUY, SFWF_BASE);
    AT_SetLocalBaseChara(fighter, SFWF_BASE);
    position = {.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFWF_BASE, .position = SFWF_DEBUG_PLAYER_POSITION, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFWF_BASE, .degrees = 225, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = SFWF_FUY, .parent = SFWF_BASE, .position = work->bookiePosition, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = SFWF_FUY, .parent = SFWF_BASE, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = fighter, .parent = SFWF_BASE, .position = {1.5f, 0, -1}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = fighter, .parent = SFWF_BASE, .degrees = 17, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    SetScenarioCharacterType(STREET_FIGHT_RYO, CharacterTypeStencilShadow);
    SetScenarioCharacterType(SFWF_FUY, CharacterTypeStencilShadow);
    SetScenarioCharacterType(fighter, CharacterTypeStencilShadow);
}

void CleanupDebug(StreetFightEventWork *work) {
    using enum CameraState;
    StreetFightStartParameters *const parameters = work->startParameters;
    ReleaseStreetFightCharacter(work->fighters[work->fighterIndex]);
    ET_AT_KillWork(work->characterReadTask);
    SetCharaKill(SFWF_BASE, 1);
    EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFWF_DEBUG_EXIT_POSITION);
    EV_AT_SetAngY(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFWF_DEBUG_EXIT_ROTATION);
    mtwkSetRegionFlag(scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, MotionRegion::LowerBody);
    if (work->hudEnabled)
        ToggleStreetFighterUI();
    GetNpcBatchCountOverride(work->previousNpcBatchCount);
    for (std::int32_t *motion = work->motions; *motion; ++motion)
        DeleteMotion(*motion);
    EV_FreeAuthorPak(0, "0100");
    if (!(parameters->flags & 1U))
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QGBT"), 0);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("ENMY"), 0);
    sbtLoadProgram();
    shenSetClockAndCompassRenderTo(0);
    HLib::EV_HLKillOtherHierarchyTask(work->task);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetPlayerControlEnabled(1);
    camCtrlSetState(Init);
    camCtrlSetFovTransitionTo(true);
    if (parameters->signal)
        *parameters->signal = 0;
}

void DebugMainCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    const StreetFightFighterDefinition *const definition = work.fighterDefinitions[work.fighterIndex];
    switch (work.state) {
    case 0:
        debugLoadCharacters[0] = fighter;
        debugLoadCharacters[1] = STREET_FIGHT_INVALID;
        QueueDebugCharacterLoad(work.task);
        work.state = 1;
        break;
    case 1: {
        PlaceDebugCharacters(&work);
        work.audienceTask = QueueStreetFightAudienceCount(30);
        if (work.audienceTask) {
            StreetFightAudienceWork *audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->fighters[audience->fighterCount++] = STREET_FIGHT_RYO;
            audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->fighters[audience->fighterCount++] = fighter;
        }
        Vector3 stage;
        EV_GetCharaPosition(SFWF_BASE, CHARACTER_TRANSLATION_ROOT, &stage, EventTransformFlags::Local);
        stage.y -= 0.5f;
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->soundCentre = stage;
            audience->state = 2;
        }
        Vector3 savedPlayer, savedBookie, player, bookie;
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &savedPlayer, EventTransformFlags::Local);
        EV_GetCharaPosition(SFWF_FUY, CHARACTER_TRANSLATION_ROOT, &savedBookie, EventTransformFlags::Local);
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::AllAxes, work.playerPosition);
        EV_AT_SetPos(SFWF_FUY, EventTransformFlags::AllAxes, work.bookiePosition);
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::World);
        EV_GetCharaPosition(SFWF_FUY, CHARACTER_TRANSLATION_ROOT, &bookie, EventTransformFlags::World);
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->corridorStart.x = player.x;
            audience->corridorStart.z = player.z;
            audience->corridorEnd.x = bookie.x;
            audience->corridorEnd.z = bookie.z;
            const float x = player.x - bookie.x, z = player.z - bookie.z;
            audience->corridorLength = std::sqrt(x * x + z * z);
        }
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::AllAxes, savedPlayer);
        EV_AT_SetPos(SFWF_FUY, EventTransformFlags::AllAxes, savedBookie);
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->radius = 5.4f;
            audience->radiusRange = 2.0f;
            audience->state = 2;
        }
        LoadBlurModelForCharacter(fighter, definition->blurModel, definition->blurModel ? 1.0f : 0.2f);
        work.state = 2;
        break;
    }
    case 2:
        EVT_AT_SetPos(
            fighter, STREET_FIGHT_INVALID, {SFWF_DEBUG_OPPONENT_ORIGIN.x - 1.0f, SFWF_DEBUG_OPPONENT_ORIGIN.y + 0.5f, SFWF_DEBUG_OPPONENT_ORIGIN.z}, 0);
        work.state = 3;
        break;
    case 3:
        EV_NPC_NewLoadSw(1);
        if (!work.audienceTask || static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask))->state > 2) {
            QueueSfwfStreetFightConversation(StartStreetFightBookieConversation, &work);
            work.state = 4;
            break;
        }
        [[fallthrough]];
    case 4:
        EVT_AT_KsIcon_Enqueue(-1, 38, 39, -1, 0x7fff, 0xffff, 0, 0);
        choicePhase = 0;
        work.state = 5;
        break;
    case 5: {
        ReadBookiePosition(SFWF_FUY);
        const std::uint32_t choice = PollChoice();
        if (!choice)
            break;
        mtwkUpdateWithPosition(scenFindMotionwork(STREET_FIGHT_RYO), nullptr, 0);
        if (choice == 38)
            work.state = static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) >= 20 ? 6 : 8;
        else
            work.state = 7;
        break;
    }
    case 6:
        QueueSfwfStreetFightConversation(AcceptStreetFightConversation, &work);
        work.state = 9;
        break;
    case 7:
        QueueSfwfStreetFightConversation(DeclineStreetFightConversation, &work);
        work.state = 23;
        break;
    case 8:
        QueueSfwfStreetFightConversation(DebugInsufficientConversation, &work);
        work.state = 23;
        break;
    case 9:
        HLib::SetCurrentTaskTo(DebugBattleEntryCallback);
        work.state = 12;
        break;
    case 12:
        work.state = work.battleStatus == 3 ? 13 : 17;
        break;
    case 13:
        ++work.completedOpponents;
        AddStreetFightProgress(SFWF_DEBUG_WIN_INCREMENTS[work.completedOpponents]);
        QueueSfwfStreetFightConversation(StreetFightVictoryConversation, &work);
        work.state = 14;
        break;
    case 14: {
        nullsub();
        const StreetFightFighterDefinition *const next = GetStreetFightDebugFighterDefinition();
        work.fighterDefinitions[0] = next;
        const shen_name nextFighter = next->character;
        work.fighters[0] = nextFighter;
        EV_NPC_NewLoadSw(0);
        if (fighter.name != STREET_FIGHT_INVALID.name) {
            if (work.audienceTask) {
                StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
                audience->fighters[0] = audience->fighters[1] = audience->fighters[2] = audience->fighters[3] = shen_name{};
                audience->fighterCount = 0;
            }
            ReleaseStreetFightCharacter(fighter);
        }
        if (nextFighter.name != STREET_FIGHT_INVALID.name) {
            debugLoadCharacters[0] = nextFighter;
            debugLoadCharacters[1] = STREET_FIGHT_INVALID;
            QueueDebugCharacterLoad(work.task);
        }
        work.state = 15;
        break;
    }
    case 15:
        EV_NPC_NewLoadSw(1);
        if (work.audienceTask) {
            StreetFightAudienceWork *audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->fighters[audience->fighterCount++] = STREET_FIGHT_RYO;
            audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->fighters[audience->fighterCount++] = fighter;
        }
        LoadBlurModelForCharacter(fighter, definition->blurModel, definition->blurModel ? 1.0f : 0.2f);
        QueueSfwfStreetFightConversation(StreetFightNextOpponentConversation, &work);
        work.state = 16;
        break;
    case 16:
        EVT_AT_KsIcon_Enqueue(-1, 38, 39, -1, 0x7fff, 0xffff, 0, 0);
        choicePhase = 0;
        work.state = 19;
        break;
    case 17:
        QueueSfwfStreetFightConversation(StreetFightDefeatConversation, &work);
        work.state = 23;
        break;
    case 18:
        QueueSfwfStreetFightConversation(StreetFightWithdrawalConversation, &work);
        work.state = 23;
        break;
    case 19: {
        ReadBookiePosition(SFWF_FUY);
        const std::uint32_t choice = PollChoice();
        if (!choice)
            break;
        mtwkUpdateWithPosition(scenFindMotionwork(STREET_FIGHT_RYO), nullptr, 0);
        work.state = choice == 38 ? 20 : 21;
        break;
    }
    case 20:
        QueueSfwfStreetFightConversation(DebugContinueConversation, &work);
        work.state = 22;
        break;
    case 21:
        QueueSfwfStreetFightConversation(DebugStopConversation, &work);
        work.state = 23;
        break;
    case 22:
        if (work.completedOpponents < debugOpponentCount) {
            QueueSfwfStreetFightConversation(StreetFightNextMatchConversation, &work);
            work.state = 9;
        } else {
            QueueSfwfStreetFightConversation(DebugCompletedConversation, &work);
            work.state = 23;
        }
        break;
    case 23:
        work.state = 24;
        break;
    case 24:
        if (!filmCheckFadeTrackColour() && !GetShouldRenderIcons(RenderIconSlot::MoneyMessage))
            work.state = 25;
        break;
    case 25:
        if (work.audienceTask) {
            (void)HLib::GetTaskParameter(work.audienceTask);
            HLib::DestroyTaskInternal(work.audienceTask);
        }
        CleanupDebug(&work);
        ++work.state;
        break;
    default:
        break;
    }
}

void DebugBattleEntryCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EventSetPosition position{0, STREET_FIGHT_RYO, SFWF_BASE, {0, 0, -1.1f}, 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFWF_BASE, .degrees = 180, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = SFWF_FUY, .parent = SFWF_BASE, .position = SFWF_DEBUG_BATTLE_BOOKIE_POSITION, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = SFWF_FUY, .parent = SFWF_BASE, .degrees = 180, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = fighter, .parent = SFWF_BASE, .position = {0, 0, 1.1f}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = fighter, .parent = SFWF_BASE, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    HLib::SetCurrentTaskTo(DebugBattlePreEventCallback);
}

void DebugBattlePreEventCallback(void *) {
    EV_LoadAudio("0100E000", 1);
    const EventFilmControl film{.cleanup = 1,
        .frames = 30,
        .startAlpha = 255,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 0,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 5};
    EVT_AT_MenCtrl(film);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0x1ae0, 0, 1, 0, 50);
    EV_PlayDialogue("0100B072");
    EventSetCamera camera{0, 2001, SFWF_BASE, SFWF_BASE, 0, {}, 0, 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    HLib::SetCurrentTaskTo(DebugBattlePreInitCallback);
    HLib::SetupTaskDelay(60);
}

void DebugBattlePreInitCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(STREET_FIGHT_RYO, &rotation, EventTransformFlags::World);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, STREET_FIGHT_INVALID);
    EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    const shen_name fighter = work.fighters[work.fighterIndex];
    GetScenarioCharacterEulerAngles(fighter, &rotation, EventTransformFlags::World);
    EV_GetCharaPosition(fighter, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    AT_SetLocalBaseChara(fighter, STREET_FIGHT_INVALID);
    EV_AT_SetPos(fighter, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(fighter, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    HLib::SetCurrentTaskTo(DebugBattleStartCallback);
    HLib::SetupTaskDelay(10);
}

void DebugBattleStartCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    const StreetFightFighterDefinition *const definition = work.fighterDefinitions[work.fighterIndex];
    work.hudEnabled = 1;
    shenStartFGST(10, 10, 0, 0);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{8}, 1);
    vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("QGBT"), 1);
    shenInitFightData(ctrlGetScenarioChara(STREET_FIGHT_RYO), nullptr, 0, 0, 0, 0);
    vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("ENMY"), 1);
    shenInitFightData(ctrlGetScenarioChara(fighter), nullptr, 1, 1, definition->hitPoints, definition->fightStyle);
    vfUpdateFlagsFor(ctrlGetScenarioChara(fighter), 0, 0x08000000U, definition->difficulty);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{1}, 1);
    vfCopyPosFromEntity(0, nullptr, SFWF_DEBUG_OPPONENT_ORIGIN.x, SFWF_DEBUG_OPPONENT_ORIGIN.z, 2.5f);
    vfmQueueFightStart(2, 0x63, 1, 8, 30);
    work.hudEnabled = 0;
    AccessFightActorFlag411(1, ctrlGetScenarioChara(fighter), definition->guardMode);
    AccessFightActorGroupFloat(1, ctrlGetScenarioChara(fighter), definition->strikeScale);
    AccessFightActorGroupFloat(3, ctrlGetScenarioChara(fighter), definition->guardScale);
    AccessFightActorGroupFloat(5, ctrlGetScenarioChara(fighter), definition->guardScale);
    HLib::SetCurrentTaskTo(DebugBattleReadyGoCallback);
}

void DebugBattleReadyGoCallback(void *) {
    EV_PlayDialogue("0100B073");
    HLib::SetCurrentTaskTo(DebugBattleStartFightCallback);
    HLib::SetupTaskDelay(40);
}

void DebugBattleStartFightCallback(void *) {
    EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{1}, 0);
    HLib::SetCurrentTaskTo(DebugBattleWaitCallback);
}

void DebugBattleWaitCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    if (IsFightActive())
        return;
    if (GetBtlStatus(ctrlGetScenarioChara(STREET_FIGHT_RYO)) == 7) {
        work.battleStatus = 7;
        EV_PlayDialogue("0100B075");
    } else {
        work.battleStatus = GetBtlStatus(ctrlGetScenarioChara(STREET_FIGHT_RYO)) == 3 ? 3 : 2;
        EVT_AT_SetVoice("0100B074", 1, 0); // @text EN: "勝負あり！" | JA: "勝負あり！"
    }
    HLib::SetCurrentTaskTo(DebugBattleWaitFadeCallback);
}

void DebugBattleWaitFadeCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    if (!IsFightActive()) {
        ProcessAicaCommand(0x0aa0, 30);
        EventFilmControl film{.cleanup = 0,
            .frames = 30,
            .startAlpha = 0,
            .startRed = 0,
            .startGreen = 0,
            .startBlue = 0,
            .targetAlpha = 255,
            .targetRed = 0,
            .targetGreen = 0,
            .targetBlue = 0,
            .delay = 0};
        EVT_AT_MenCtrl_Callback(&film);
        HLib::SetCurrentTaskTo(DebugBattleEndCallback);
        HLib::SetupTaskDelay(30);
    }
    work.frameCounter = 0;
}

void DebugBattleEndCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EV_LoadAudio("0100E000", 0);
    BeginSecondarySampleExecution(0);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFWF_BASE);
    AT_SetLocalBaseChara(fighter, SFWF_BASE);
    EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, false);
    EV_SetCharacterAlphaEnabled(SFWF_FUY, false);
    EV_SetCharacterAlphaEnabled(fighter, false);
    HLib::SetCurrentTaskTo(DebugMainCallback);
    work.frameCounter = 0;
}

std::int32_t DebugContinueConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFWF_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFWF_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFWF_BASE, SFWF_DEBUG_CONVERSATION_PLAYER_POSITION, 60);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFWF_BASE, 17, 60);
    EVT_AT_SetPos(fighter, SFWF_BASE, SFWF_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
    if (work.completedOpponents < debugOpponentCount) {
        EVT_AT_SetPos(fighter, SFWF_BASE, SFWF_DEBUG_BATTLE_BOOKIE_POSITION, 60);
        EVT_AT_SetAngY(fighter, SFWF_BASE, 225, 60);
    }
    EVT_AT_SetPos(SFWF_FUY, SFWF_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFWF_FUY, SFWF_BASE, 0, 0);
    EVT_AT_SetMot(SFWF_FUY, 0x1aed, 0, 1.1f, 0, 0);
    HLib::Task *const firstTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    SkEventMessageWork *const first = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(firstTask));
    first->message = STREET_FIGHT_DEBUG_ACCEPT;
    first->frames = 30;
    HLib::Task *const secondTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(secondTask, 30);
    SkEventMessageWork *const second = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(secondTask));
    second->message = STREET_FIGHT_DEBUG_WAIT;
    second->frames = 30;
    HLib::Task *const thirdTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(thirdTask, 60);
    SkEventMessageWork *const third = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(thirdTask));
    third->message = STREET_FIGHT_DEBUG_INVITATION;
    third->frames = 90;
    HLib::Task *const fourthTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(fourthTask, 150);
    SkEventMessageWork *const fourth = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(fourthTask));
    fourth->message = STREET_FIGHT_DEBUG_CHALLENGE;
    fourth->frames = 60;
    EVT_AT_SetCam(2006, SFWF_BASE, SFWF_BASE, 0, 0, 60);
    EVT_AT_SetCam(120, SFWF_BASE, SFWF_BASE, 0, 0, 150);
    return 210;
}

std::int32_t DebugCompletedConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFWF_BASE, SFWF_DEBUG_CONVERSATION_PLAYER_POSITION, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFWF_BASE, 17, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(SFWF_FUY, SFWF_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFWF_FUY, SFWF_BASE, 0, 0);
    EVT_AT_SetMot(SFWF_FUY, 0x1aed, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFWF_BASE, SFWF_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
    HLib::Task *const firstTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    SkEventMessageWork *const first = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(firstTask));
    first->message = STREET_FIGHT_DEBUG_CALL;
    first->frames = 30;
    HLib::Task *const secondTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(secondTask, 60);
    SkEventMessageWork *const second = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(secondTask));
    second->message = STREET_FIGHT_DEBUG_NO_OPPONENT;
    second->frames = 60;
    HLib::Task *const thirdTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(thirdTask, 120);
    SkEventMessageWork *const third = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(thirdTask));
    third->message = STREET_FIGHT_DEBUG_PRIZE;
    third->frames = 60;
    EVT_AT_SetCam(2007, SFWF_BASE, SFWF_BASE, 0, 0, 5);
    PATI_StartMoneyCount(static_cast<std::uint32_t>(STREET_FIGHT_PRIZES[work.completedOpponents]), 140);
    EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
        .frames = 40,
        .startAlpha = 0,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 255,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 180});
    return 240;
}

std::int32_t DebugStopConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFWF_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFWF_BASE, 180, 0);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 0);
    EVT_AT_SetPos(SFWF_FUY, SFWF_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFWF_FUY, SFWF_BASE, 0, 0);
    EVT_AT_SetMot(SFWF_FUY, 0x1aed, 0, 1.1f, 0, 0);
    EVT_AT_SetPos(fighter, SFWF_BASE, SFWF_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
    HLib::Task *const firstTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    SkEventMessageWork *const first = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(firstTask));
    first->message = STREET_FIGHT_DEBUG_STOP;
    first->frames = 60;
    HLib::Task *const secondTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(secondTask, 60);
    SkEventMessageWork *const second = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(secondTask));
    second->message = STREET_FIGHT_DEBUG_AGREEMENT;
    second->frames = 60;
    HLib::Task *const thirdTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(thirdTask, 120);
    SkEventMessageWork *const third = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(thirdTask));
    third->message = STREET_FIGHT_DEBUG_STOP_PRIZE;
    third->frames = 60;
    EVT_AT_SetCam(2007, SFWF_BASE, SFWF_BASE, 0, 0, 5);
    PATI_StartMoneyCount(static_cast<std::uint32_t>(STREET_FIGHT_PRIZES[work.completedOpponents]), 140);
    EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
        .frames = 40,
        .startAlpha = 0,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 255,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 180});
    return 240;
}
// @verified - p
void SfwfInitNullsub() {
}
void Main(std::uintptr_t argument) {
    StreetFightStartParameters *const parameters = reinterpret_cast<StreetFightStartParameters *>(argument);
    if (parameters->scene == 99)
        QueueDebug(parameters);
    else
        QueueNormal(parameters);
}
} // namespace
const ScriptAreaDefinition areaSfwfScriptDefinition{.areaId = SHEN_NAME(SFWF), .field04 = 0, .init = SfwfInitNullsub, .main = Main};
} // namespace shenmue::s2
#endif
