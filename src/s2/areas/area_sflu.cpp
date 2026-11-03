#if defined(SHENMUE_EVENT_OVERLAY)
#define SHENMUE_STREET_FIGHT_PACKAGE QKBT

#if defined(SHENMUE_EVENT_SFLU_TRAINING)
int trainingInputFailed;
#else
#include "s2/areas/area_sflu.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void SfluMain(StreetFightStartParameters *start)
{
    if (start->scene == 99)
        QueueDebug(start);
    else
        QueueNormal(start);
}

#elif defined(SHENMUE_EVENT_SFLU_AUTHOR)
static const int sfluElevatorPanelSegments[] = {152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 163, 174, 185, 196, 207, 249, 250, 251, -1};
static const int sfluElevatorSegments[] = {
    152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 162, 163, 174, 185, 196, 207, 231, 233, 234, -1};
static shen_name sfluAuthorCharacters0[] = {SHEN_NAME4('K', 'F', '1', '_'), SHEN_NAME4('K', 'F', '2', '_'), SHEN_NAME4('K', 'F', '3', '_'),
    SHEN_NAME4('K', 'R', '1', '_'), SHEN_NAME4('K', 'R', '2', '_'), SHEN_NAME4('S', 'F', '4', '_'), SHEN_NAME4('S', 'F', '9', '_'), STREET_FIGHT_INVALID};
static shen_name sfluAuthorCharacters1[] = {SHEN_NAME4('J', 'O', 'P', '_'), SHEN_NAME4('K', 'F', '1', '_'), SHEN_NAME4('K', 'F', '2', '_'),
    SHEN_NAME4('K', 'R', '2', '_'), SHEN_NAME4('S', 'F', '4', '_'), STREET_FIGHT_INVALID};
static shen_name sfluAuthorCharacters2[] = {SHEN_NAME4('K', 'R', '2', '_'), SHEN_NAME4('K', 'F', '1', '_'), SHEN_NAME4('K', 'F', '2', '_'),
    SHEN_NAME4('B', 'F', '6', '_'), SHEN_NAME4('S', 'F', '4', '_'), STREET_FIGHT_INVALID};
static shen_name sfluAuthorCharacters3[] = {SHEN_NAME4('J', 'O', 'P', '_'), SHEN_NAME4('K', 'F', '1', '_'), SHEN_NAME4('K', 'F', '2', '_'),
    SHEN_NAME4('K', 'R', '2', '_'), SHEN_NAME4('S', 'F', '4', '_'), SHEN_NAME4('B', 'F', '6', '_'), STREET_FIGHT_INVALID};
static shen_name sfluAuthorCharacters4[] = {SHEN_NAME4('J', 'O', 'P', '_'), STREET_FIGHT_INVALID};
static shen_name sfluAuthorCharacters5[] = {SHEN_NAME4('S', 'F', '4', '_'), STREET_FIGHT_INVALID};
static shen_name sfluAuthorCharacters6[] = {SHEN_NAME4('S', 'F', '4', '_'), STREET_FIGHT_INVALID};
static shen_name *sfluAuthorCharacters[] = {sfluAuthorCharacters0, sfluAuthorCharacters1, sfluAuthorCharacters2, sfluAuthorCharacters3, sfluAuthorCharacters4,
    sfluAuthorCharacters5, sfluAuthorCharacters6, 0};
static int sfluElevatorTens[] = {152, 163, 174, 185, 196};
static float authoredFarClip;
static HLibTask *elevatorPanelTask;
static shen_name elevatorCharacters[9];
void QueueAuthor(shen_name command) {
    HLibTask *task;
    SfluAuthorWork *work;
    task = FTBL.HLib_EnqueueTaskWithParameter(AuthorCallback, 4, 11, sizeof(SfluAuthorWork), TASK_EVNT);
    work = (SfluAuthorWork *)FTBL.HLib_GetTaskParameter(task);
    work->task = task;
    work->state = 1;
    work->command = command;
    PrepareAuthor(command);
}

void AuthorCallback(void *data) {
    SfluAuthorWork *work = (SfluAuthorWork *)data;
    switch (work->state) {
    case 0:
        break;
    case 1:
        QueueAuthorLoop(work);
        break;
    case 3:
        EndAuthor(work);
        break;
    }
}

void QueueAuthorLoop(SfluAuthorWork *owner) {
    HLibTask *task;
    SfluAuthorLoopWork *loop;
    task = FTBL.HLib_EnqueueTaskWithParameter(AuthorLoopCallback, 4, 11, sizeof(SfluAuthorLoopWork), TASK_EVNT);
    loop = (SfluAuthorLoopWork *)FTBL.HLib_GetTaskParameter(task);
    loop->owner = owner;
    loop->frame = 0;
    if (owner->command == SFLU_AUTH_5 || owner->command == SFLU_AUTH_6)
        FTBL.EV_authSceneCtrl(-8);
    else
        FTBL.EV_authSceneCtrl(-7);
    switch (owner->command) {
    case SFLU_AUTH_4:
        FTBL.EV_LoadCutsceneFromCurrentScene("0260A090");
        break;
    case SFLU_AUTH_5:
        FTBL.EV_LoadCutsceneFromCurrentScene("0260A030");
        break;
    case SFLU_AUTH_6:
        FTBL.EV_LoadCutsceneFromCurrentScene("0260A050");
        break;
    default:
        FTBL.EV_LoadCutsceneFromCurrentScene("0260A000");
        break;
    }
    switch (owner->command) {
    case SFLU_AUTH_4:
        FTBL.EV_lightLoad(SHEN_NAME4('0', '0', '0', '0'));
        FTBL.PushGlobalLightingProgram();
        FTBL.EV_lightLoad(SHEN_NAME4('l', '9', '0', '1'));
        break;
    case SFLU_AUTH_5:
    case SFLU_AUTH_6:
        FTBL.EV_lightLoad(SHEN_NAME4('0', '0', '0', '0'));
        break;
    }
    FTBL.PushGlobalLightingProgram();
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.EV_authSceneCtrl(owner->command);
    owner->state = 2;
}

void AuthorLoopCallback(void *data) {
    SfluAuthorLoopWork *loop = (SfluAuthorLoopWork *)data;
    loop->frame = FTBL.EV_authSceneCtrl(-6);
    if (!loop->frame)
        LoadAuthorCharacters(loop->owner->command);
    switch (loop->owner->command) {
    case SFLU_AUTH_4:
        switch (loop->frame) {
        case 60:
            ShowElevatorPart(SFLU_A798, 152, 1);
            ShowElevatorPart(SFLU_A798, 153, 1);
            break;
        case 90:
            QueueElevatorPanel(1, 3);
            break;
        case 310:
            QueueElevatorPanel(2, 12);
            break;
        case 1575:
            QueueElevatorPanel(32, 36);
            break;
        }
        break;
    case SFLU_AUTH_5:
    case SFLU_AUTH_6:
        if (loop->frame == FTBL.EV_authSceneCtrl(-5) - 15)
            FTBL.SetScreenFilmFade(15, 0, 0, 0, 0, 255, 0, 0, 0);
        break;
    }
    switch (loop->owner->command) {
    case SFLU_AUTH_0: {
        switch (loop->frame) {
        case 1180:
            FTBL.EV_DisableCharacterDisplay(SFLU_KF1);
            FTBL.EV_ToggleCharacterFlag(SFLU_KF1);
            FTBL.EV_DisableCharacterDisplay(SFLU_KF2);
            FTBL.EV_ToggleCharacterFlag(SFLU_KF2);
            FTBL.EV_DisableCharacterDisplay(SFLU_KF3);
            FTBL.EV_ToggleCharacterFlag(SFLU_KF3);
            FTBL.EV_DisableCharacterDisplay(SFLU_KR1);
            FTBL.EV_ToggleCharacterFlag(SFLU_KR1);
            FTBL.EV_DisableCharacterDisplay(SFLU_KR2);
            FTBL.EV_ToggleCharacterFlag(SFLU_KR2);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 0, 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFLU_SF4), 0);
            FTBL.SetObjectFlagByIndex(1, 0);
            break;
        case 1346:
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFLU_SF4), 1);
            FTBL.SetObjectFlagByIndex(1, 1);
            break;
        case 1580:
            FTBL.EV_EnableCharacterDisplay(SFLU_KF1);
            FTBL.EV_EnsureCharacterDraw(SFLU_KF1);
            FTBL.EV_EnableCharacterDisplay(SFLU_KF2);
            FTBL.EV_EnsureCharacterDraw(SFLU_KF2);
            FTBL.EV_EnsureCharacterDraw(SFLU_KF3);
            FTBL.EV_EnsureCharacterDraw(SFLU_KR1);
            FTBL.EV_EnsureCharacterDraw(SFLU_KR2);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 1);
            break;
        default:
            break;
        }
        break;
    }
    case SFLU_AUTH_1: {
        switch (loop->frame) {
        case 311:
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFLU_JOP), 0);
            break;
        case 841:
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 0, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFLU_SF4), 0, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFLU_SF4), 0, 0, 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFLU_SF4), 0);
            FTBL.SetObjectFlagByIndex(1, 0);
            break;
        case 1066:
            Queue_EVT_AT_CtrlHandUp_1(SFLU_SF4, 1, (void *)areaLoopHandPoses[11], 0, 0);
            Queue_EVT_AT_CtrlHandUp_1(SFLU_SF4, 0, (void *)areaLoopHandPoses[11], 0, 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 1);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFLU_SF4), 1);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFLU_JOP), 1);
            FTBL.SetObjectFlagByIndex(1, 1);
            break;
        default:
            break;
        }
        break;
    }
    case SFLU_AUTH_2: {
        switch (loop->frame) {
        case 0:
            FTBL.SetObjectFlagByIndex(1, 0);
            FTBL.EV_DisableCharacterDisplay(SFLU_JOP);
            FTBL.EV_ToggleCharacterFlag(SFLU_JOP);
            FTBL.SetScenarioCharacterDrawControl(0, 2, 0, 0.0f);
            FTBL.SetScenarioCharacterDrawControl(STREET_FIGHT_RYO, 2, 0, 0.0f);
            FTBL.SetScenarioCharacterDrawControl(SFLU_SF4, 2, 0, 0.0f);
            FTBL.SetScenarioCharacterDrawControl(SFLU_KF1, 2, 0, 0.0f);
            FTBL.SetScenarioCharacterDrawControl(SFLU_KF2, 2, 0, 0.0f);
            FTBL.SetScenarioCharacterDrawControl(SFLU_KR2, 2, 0, 0.0f);
            FTBL.SetScenarioCharacterDrawControl(SFLU_BF6, 2, 0, 0.0f);
            FTBL.SetScenarioCharacterDrawControl(SFLU_JOP, 2, 0, 0.0f);
            break;
        case 421:
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 0, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 1, 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFLU_SF4), 0);
            break;
        case 422:
            FTBL.EV_ToggleCharacterFlag(SFLU_KF1);
            FTBL.EV_ToggleCharacterFlag(SFLU_KF2);
            FTBL.EV_ToggleCharacterFlag(SFLU_KR2);
            break;
        case 521:
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 0, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 1, 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 1);
            break;
        case 661:
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFLU_SF4), 0, 1, 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFLU_SF4), 1);
            FTBL.EV_EnsureCharacterDraw(SFLU_KF1);
            FTBL.EV_EnsureCharacterDraw(SFLU_KF2);
            FTBL.EV_EnsureCharacterDraw(SFLU_KR2);
            break;
        case 891:
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFLU_SF4), 0, 0, 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFLU_SF4), 0);
            FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0);
            break;
        case 892:
            FTBL.EV_ToggleCharacterFlag(SFLU_KF1);
            FTBL.EV_ToggleCharacterFlag(SFLU_KF2);
            break;
        case 1029:
            FTBL.EV_EnsureCharacterDraw(SFLU_JOP);
            FTBL.SetObjectFlagByIndex(1, 1);
            break;
        default:
            break;
        }
        break;
    }
    case SFLU_AUTH_3: {
        int frame = loop->frame;
        if (frame == 810)
            goto authorFrame810;
        if (frame > 810)
            goto authorFrameUpper;
        if (frame == 720)
            goto authorFrame720;
        if (frame > 720)
            goto authorFrameMiddle;
        if (frame == 476)
            goto authorFrame476;
        if (frame > 476)
            goto authorScene3Continue;
        if (!frame)
            goto authorFrame0;
        goto authorScene3Continue;
    authorFrameMiddle:
        if (frame == 792)
            goto authorFrame792;
        if (frame > 792)
            goto authorScene3Continue;
        if (frame == 760)
            goto authorFrame760;
        goto authorScene3Continue;
    authorFrameUpper:
        if (frame == 986)
            goto authorScene3Continue;
        if (frame > 986)
            goto authorFrameUpperMiddle;
        if (frame == 880)
            goto authorFrame880;
        if (frame > 880)
            goto authorScene3Continue;
        if (frame == 855)
            goto authorFrame855;
        goto authorScene3Continue;
    authorFrameUpperMiddle:
        if (frame == 1147)
            goto authorFrame1147;
        if (frame > 1147)
            goto authorFrameLast;
        if (frame == 1146)
            goto authorFrame1146;
        goto authorScene3Continue;
    authorFrameLast:
        if (frame == 1205)
            goto authorScene3Continue;
        if (frame == 1256)
            goto authorFrame1256;
        goto authorScene3Continue;
    authorFrame0:
        FTBL.SetObjectFlagByIndex(1, 0);
        FTBL.SetScenarioCharacterDrawControl(0, 2, 0, 0.0f);
        FTBL.SetScenarioCharacterDrawControl(STREET_FIGHT_RYO, 2, 0, 0.0f);
        FTBL.SetScenarioCharacterDrawControl(SFLU_SF4, 2, 0, 0.0f);
        FTBL.SetScenarioCharacterDrawControl(SFLU_KF1, 2, 0, 0.0f);
        FTBL.SetScenarioCharacterDrawControl(SFLU_KF2, 2, 0, 0.0f);
        FTBL.SetScenarioCharacterDrawControl(SFLU_KR2, 2, 0, 0.0f);
        FTBL.SetScenarioCharacterDrawControl(SFLU_BF6, 2, 0, 0.0f);
        FTBL.SetScenarioCharacterDrawControl(SFLU_JOP, 2, 0, 0.0f);
        goto authorScene3Continue;
    authorFrame476:
        authoredFarClip = FTBL.camCtrlSetFarDistance(13.0f);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFLU_JOP), 0, 0, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFLU_JOP), 0, 1, 0);
        FTBL.EV_DisableCharacterDisplay(SFLU_SF4);
        FTBL.EV_DisableCharacterDisplay(SFLU_KF2);
        FTBL.EV_DisableCharacterDisplay(SFLU_BF6);
        FTBL.EV_ToggleCharacterFlag(SFLU_SF4);
        goto authorScene3Continue;
    authorFrame720:
        FTBL.SetClothFlag(FTBL.ctrlGetCharaByName(SFLU_JOP), 4, 1);
        goto authorScene3Continue;
    authorFrame760:
        FTBL.EV_EnableCharacterDisplay(SFLU_BF6);
        goto authorScene3Continue;
    authorFrame792:
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 0, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 1, 0);
        goto authorScene3Continue;
    authorFrame810:
        FTBL.EV_EnableCharacterDisplay(SFLU_KF2);
        goto authorScene3Continue;
    authorFrame855:
        FTBL.EV_DisableCharacterDisplay(SFLU_KR2);
        goto authorScene3Continue;
    authorFrame880:
        FTBL.SetClothFlag(FTBL.ctrlGetCharaByName(SFLU_JOP), 4, 0);
        FTBL.camCtrlSetFarDistance(authoredFarClip);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 0, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 1, 0);
        FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFLU_JOP), 1);
        FTBL.EV_EnableCharacterDisplay(SFLU_KF1);
        FTBL.EV_EnableCharacterDisplay(SFLU_KF2);
        FTBL.EV_EnableCharacterDisplay(SFLU_KR2);
        FTBL.EV_EnableCharacterDisplay(SFLU_BF6);
        goto authorScene3Continue;
    authorFrame1146:
        FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0);
        FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(SFLU_JOP), 0);
        goto authorScene3Continue;
    authorFrame1147:
        FTBL.EV_ToggleCharacterFlag(SFLU_KR2);
        FTBL.EV_ToggleCharacterFlag(SFLU_BF6);
        FTBL.EV_ToggleCharacterFlag(SFLU_KF1);
        goto authorScene3Continue;
    authorFrame1256:
        FTBL.SetObjectFlagByIndex(1, 1);
        FTBL.EV_DisableCharacterDisplay(SFLU_KF1);
        FTBL.EV_ToggleCharacterFlag(SFLU_KF1);
        FTBL.EV_EnableCharacterDisplay(SFLU_KF2);
        FTBL.EV_DisableCharacterDisplay(SFLU_BF6);
        FTBL.EV_ToggleCharacterFlag(SFLU_BF6);
        FTBL.EV_EnsureCharacterDraw(SFLU_KF1);
        FTBL.EV_EnsureCharacterDraw(SFLU_KR2);
        goto authorScene3Continue;
    authorScene3Continue:
        if (loop->frame >= 880) {
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 0, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 1, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFLU_JOP), 0, 0, 0);
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFLU_JOP), 0, 1, 0);
        }
        break;
    }
    }
    if (!FTBL.EV_authSceneCtrl(-1)) {
        UnloadAuthorCharacters(loop->owner->command);
        loop->owner->state = 3;
        if (loop->owner->command == SFLU_AUTH_0 || loop->owner->command == SFLU_AUTH_1 || loop->owner->command == SFLU_AUTH_5) {
            FTBL.EV_lightLoad(SHEN_NAME4('0', '0', '0', '0'));
            FTBL.PushGlobalLightingProgram();
        }
        FTBL.HLib_DestroyCurrentEventTask();
    }
}

void PrepareAuthor(shen_name command) {
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_SetWeatherInstruction(12, 0);
    if (command == SFLU_AUTH_4)
        InitializeElevatorCharacters();
}

void InitializeElevatorCharacters(void) {
    Vector3 position = {249.67999267578125f, 83.2300033569336f, 123.54000091552734f};
    Vector3i rotation = {0, 0x4000, 0};
    int index, segment;
    for (index = 0; index < 9; ++index) {
        elevatorCharacters[index] = FTBL.CreateEventCharacter(3, "ELVTTBBG", 0);
        FTBL.EV_EnableCharacterDisplay(elevatorCharacters[index]);
        position.y += 0.09000000357627869f;
        if (index == 4) {
            position.z -= 0.09000000357627869f;
            position.y = 83.2300033569336f;
        }
        FTBL.EV_AT_SetPos(elevatorCharacters[index], 0x78000000, &position);
        FTBL.EV_AT_SetAngY_0(elevatorCharacters[index], 0x78000000, &rotation, 0);
        for (segment = 0; sfluElevatorSegments[segment] != -1; ++segment) {
            ShowElevatorPart(elevatorCharacters[index], sfluElevatorSegments[segment], 0);
            ShowElevatorPart(elevatorCharacters[index], 162, 1);
        }
    }
    for (segment = 0; sfluElevatorSegments[segment] != -1; ++segment)
        ShowElevatorPart(SFLU_A800, sfluElevatorSegments[segment], 0);
    for (segment = 0; sfluElevatorPanelSegments[segment] != -1; ++segment)
        ShowElevatorPart(SFLU_A798, sfluElevatorPanelSegments[segment], 0);
    ShowElevatorPart(elevatorCharacters[8], 162, 0);
    ShowElevatorPart(elevatorCharacters[8], 231, 0);
    ShowElevatorPart(elevatorCharacters[8], 232, 0);
    ShowElevatorPart(elevatorCharacters[8], 233, 1);
    ShowElevatorPart(elevatorCharacters[8], 196, 1);
    ShowElevatorPart(elevatorCharacters[8], 242, 1);
    ShowElevatorPart(SFLU_A800, 207, 1);
    ShowElevatorPart(SFLU_A800, 231, 1);
    ShowElevatorPart(SFLU_A800, 247, 1);
}

void QueueElevatorPanel(int start, int end) {
    int index;
    SfluElevatorPanelWork *work;
    for (index = 0; sfluElevatorPanelSegments[index] != -1; ++index)
        ShowElevatorPart(SFLU_A798, sfluElevatorPanelSegments[index], 0);
    ShowElevatorPart(SFLU_A798, __builtin_abs(start) % 10 + 152, 1);
    if (start < 0)
        ShowElevatorPart(SFLU_A798, 207, 1);
    else {
        int tens = start / 10;
        ShowElevatorPart(SFLU_A798, sfluElevatorTens[tens], 1);
    }
    elevatorPanelTask = FTBL.HLib_EnqueueTaskWithParameter(ElevatorPanelCallback, 4, 11, sizeof(SfluElevatorPanelWork), TASK_EVNT);
    work = (SfluElevatorPanelWork *)FTBL.HLib_GetTaskParameter(elevatorPanelTask);
    work->start = start;
    work->end = end;
    work->next = start + 1;
    work->tens = start / 10;
    work->delay = 35;
}

void ElevatorPanelCallback(void *data) {
    SfluElevatorPanelWork *work = (SfluElevatorPanelWork *)data;
    int tens = 0;
    int index;
    if (!FTBL.EV_authSceneCtrl(-1))
        FTBL.HLib_DestroyCurrentEventTask();
    if (--work->delay == 0) {
        for (index = 0; sfluElevatorPanelSegments[index] != -1; ++index)
            ShowElevatorPart(SFLU_A798, sfluElevatorPanelSegments[index], 0);
        if (work->next < 0)
            ShowElevatorPart(SFLU_A798, 207, 1);
        else {
            tens = work->next / 10;
            ShowElevatorPart(SFLU_A798, sfluElevatorTens[tens], 1);
        }
        if (tens == 0 && __builtin_abs(work->next) % 10 == 0)
            ShowElevatorPart(SFLU_A798, 153, 1);
        else
            ShowElevatorPart(SFLU_A798, __builtin_abs(work->next) % 10 + 152, 1);
        work->delay = 35;
        if (work->next == work->end)
            FTBL.HLib_DestroyCurrentEventTask();
        else
            ++work->next;
    }
}

void EndAuthor(SfluAuthorWork *work) {
    FTBL.HLib_DestroyTask(work->task);
}

void LoadAuthorCharacters(shen_name command) {
    unsigned int table = 0;
    unsigned int index;
    HideAuthorCharacter(STREET_FIGHT_RYO);
    switch (command) {
    case SFLU_AUTH_0:
        table = 0;
        break;
    case SFLU_AUTH_1:
        table = 1;
        break;
    case SFLU_AUTH_2:
        table = 2;
        break;
    case SFLU_AUTH_3:
        table = 3;
        break;
    case SFLU_AUTH_4:
        table = 4;
        break;
    case SFLU_AUTH_5:
        table = 5;
        break;
    case SFLU_AUTH_6:
        table = 6;
        break;
    }
    for (index = 0; sfluAuthorCharacters[table][index] != STREET_FIGHT_INVALID; ++index)
        HideAuthorCharacter(sfluAuthorCharacters[table][index]);
    if (command == SFLU_AUTH_4) {
        HideAuthorProp(SFLU_A800);
        HideAuthorProp(SFLU_A798);
    } else {
        HideAuthorProp(SFLU_A516);
        HideAuthorProp(SFLU_A512);
        HideAuthorProp(SFLU_A734);
    }
}

void UnloadAuthorCharacters(shen_name command) {
    unsigned int table = 0;
    unsigned int index;
    ShowAuthorCharacter(STREET_FIGHT_RYO);
    switch (command) {
    case SFLU_AUTH_0:
        table = 0;
        break;
    case SFLU_AUTH_1:
        table = 1;
        break;
    case SFLU_AUTH_2:
        table = 2;
        break;
    case SFLU_AUTH_3:
        table = 3;
        break;
    case SFLU_AUTH_4:
        table = 4;
        break;
    case SFLU_AUTH_5:
        table = 5;
        break;
    case SFLU_AUTH_6:
        table = 6;
        break;
    }
    for (index = 0; sfluAuthorCharacters[table][index] != STREET_FIGHT_INVALID; ++index)
        ShowAuthorCharacter(sfluAuthorCharacters[table][index]);
    if (command == SFLU_AUTH_4) {
        ShowAuthorProp(SFLU_A800);
        ShowAuthorProp(SFLU_A798);
    } else {
        ShowAuthorProp(SFLU_A516);
        ShowAuthorProp(SFLU_A512);
        ShowAuthorProp(SFLU_A734);
    }
}

void HideAuthorCharacter(shen_name character) {
    FTBL.EV_EnsureCharacterDraw(character);
    FTBL.EV_EnableCharacterDisplay(character);
    FTBL.EV_SetCharaControllerFlags(character, 1, 0);
    FTBL.EV_SetCharaControllerFlags(character, 3, 0);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 2);
    FTBL.EV_SetCharacterAlphaEnabled(character, 0);
    FTBL.AT_SetLocalBaseChara(character, STREET_FIGHT_INVALID);
}

void ShowAuthorCharacter(shen_name character) {
    FTBL.EV_EnableCharacterDisplay(character);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 0);
}

void HideAuthorProp(shen_name prop) {
    FTBL.EV_EnableCharacterDisplay(prop);
    FTBL.EV_SetCharaControllerFlags(prop, 1, 0);
    FTBL.EV_SetCharaControllerFlags(prop, 3, 0);
    FTBL.EV_SetCharacterAlphaEnabled(prop, 0);
}

void ShowAuthorProp(shen_name prop) {
    FTBL.EV_DisableCharacterDisplay(prop);
}

void ShowElevatorPart(shen_name character, int part, int visible) {
    Vector3 hidden = {0.0f, 1000.0f, 0.0f};
    Vector3 shown = {0.0f, 0.0f, 0.0f};
    if (visible)
        FTBL.scenApplyPosOrRotToChildId(character, part, &shown, 0, 0);
    else
        FTBL.scenApplyPosOrRotToChildId(character, part, &hidden, 0, 0);
}
#else
static const shen_name sfluNormalCharacters[] = {SHEN_NAME4('R', 'Y', 'O', '_'), SHEN_NAME4('S', 'F', '4', '_'), SHEN_NAME4('G', 'O', 'I', '_'),
    SHEN_NAME4('J', 'O', 'P', '_'), SHEN_NAME4('K', 'F', '1', '_'), SHEN_NAME4('K', 'F', '2', '_'), SHEN_NAME4('K', 'F', '3', '_'),
    SHEN_NAME4('K', 'R', '1', '_'), SHEN_NAME4('K', 'R', '2', '_'), SHEN_NAME4('B', 'F', '6', '_'), SHEN_NAME4('S', 'F', '9', '_'), STREET_FIGHT_INVALID};
static const shen_name sfluFenceCharacters[] = {SFLU_AMI1, SFLU_AMI2, SFLU_AMI3, SFLU_AMI4, SFLU_AMI5};
static Vector3i savedPlayerRotation = {0, 0, 0};
static Vector3i savedFighterRotation = {0, 0, 0};
static unsigned char qtePhase = 0;
static int trainingFrames = 0;
static int battleLevel = 0;
static int trainingLevel = 0;
static Vector3 sfluStoryExitPosition = {245.94000244140625f, 89.19999694824219f, 125.5999984741211f};
static Vector3i sfluStoryExitRotation = {0, 17476, 0};
static unsigned char SFLU_FIGHT_PARAMETERS[][12] = {{112, 160, 160, 160, 112, 32, 0, 48, 111, 0, 0, 0}, {96, 168, 168, 168, 96, 48, 0, 42, 116, 0, 0, 0},
    {90, 176, 176, 176, 80, 64, 0, 37, 143, 0, 0, 0}, {74, 184, 184, 184, 64, 80, 0, 32, 160, 0, 0, 0}, {58, 197, 197, 197, 48, 112, 0, 16, 186, 0, 0, 0},
    {31, 213, 213, 213, 32, 144, 0, 5, 186, 0, 0, 0}};
static PluralControlEntry SFLU_PLURAL_FIRST[] = {{.frame = 0, .character = SHEN_NAME4('K', 'F', '1', '_'), .state = 1},
    {.frame = 0, .character = SHEN_NAME4('K', 'F', '2', '_'), .state = 1}, {.frame = 0, .character = SHEN_NAME4('K', 'F', '3', '_'), .state = 1},
    {.frame = 0, .character = SHEN_NAME4('K', 'R', '1', '_'), .state = 1}, {.frame = 0, .character = SHEN_NAME4('K', 'R', '2', '_'), .state = 1},
    {.frame = 0, .character = SHEN_NAME4('B', 'F', '6', '_'), .state = 1}, {.frame = 0, .character = SHEN_NAME4('S', 'F', '4', '_'), .state = 1},
    {.frame = 0, .character = SHEN_NAME4('G', 'O', 'I', '_'), .state = 1}, {.frame = 600, .character = SHEN_NAME4('S', 'F', '4', '_'), .state = 0},
    {.frame = 600, .character = SHEN_NAME4('G', 'O', 'I', '_'), .state = 0}, {.frame = 1600, .character = SHEN_NAME4('K', 'F', '1', '_'), .state = 0},
    {.frame = 1600, .character = SHEN_NAME4('K', 'F', '2', '_'), .state = 0}, {.frame = -1, .character = 0, .state = 0}};
static PluralControlEntry SFLU_PLURAL_REPEAT[] = {{.frame = 841, .character = SHEN_NAME4('K', 'F', '1', '_'), .state = 1},
    {.frame = 841, .character = SHEN_NAME4('K', 'F', '2', '_'), .state = 1}, {.frame = 841, .character = SHEN_NAME4('K', 'F', '3', '_'), .state = 1},
    {.frame = 841, .character = SHEN_NAME4('K', 'R', '1', '_'), .state = 1}, {.frame = 841, .character = SHEN_NAME4('K', 'R', '2', '_'), .state = 1},
    {.frame = 841, .character = SHEN_NAME4('B', 'F', '6', '_'), .state = 1}, {.frame = -1, .character = 0, .state = 0}};
static PluralControlEntry SFLU_PLURAL_EVENING[] = {{.frame = 0, .character = SHEN_NAME4('K', 'F', '1', '_'), .state = 1},
    {.frame = 0, .character = SHEN_NAME4('K', 'F', '2', '_'), .state = 1}, {.frame = 0, .character = SHEN_NAME4('K', 'F', '3', '_'), .state = 1},
    {.frame = 0, .character = SHEN_NAME4('K', 'R', '1', '_'), .state = 1}, {.frame = 0, .character = SHEN_NAME4('K', 'R', '2', '_'), .state = 1},
    {.frame = 0, .character = SHEN_NAME4('B', 'F', '6', '_'), .state = 1}, {.frame = 421, .character = SHEN_NAME4('R', 'Y', 'O', '_'), .state = 1},
    {.frame = 421, .character = SHEN_NAME4('S', 'F', '4', '_'), .state = 1}, {.frame = -1, .character = 0, .state = 0}};
static PluralControlEntry SFLU_PLURAL_RESULT[] = {{.frame = 0, .character = SHEN_NAME4('K', 'F', '1', '_'), .state = 1},
    {.frame = 0, .character = SHEN_NAME4('K', 'F', '2', '_'), .state = 1}, {.frame = 0, .character = SHEN_NAME4('K', 'F', '3', '_'), .state = 1},
    {.frame = 0, .character = SHEN_NAME4('K', 'R', '1', '_'), .state = 1}, {.frame = 0, .character = SHEN_NAME4('K', 'R', '2', '_'), .state = 1},
    {.frame = 0, .character = SHEN_NAME4('B', 'F', '6', '_'), .state = 1}, {.frame = 476, .character = SHEN_NAME4('R', 'Y', 'O', '_'), .state = 1},
    {.frame = 476, .character = SHEN_NAME4('J', 'O', 'P', '_'), .state = 1}, {.frame = -1, .character = 0, .state = 0}};
static int cameraManualMode = 0;
static unsigned short SFLU_TRAINING_BUTTONS[] = {4, 64, 1, 256, 256};
static int sfluQteFirstWait[] = {16, 20, 24};
static int sfluQteSecondWait[] = {24, 30, 36};
static int SFLU_TRAINING_WAIT[] = {32, 40, 48};
static Vector3 sfluLostEyeFirst = {239.13760375976562f, 85.65229797363281f, 122.03849792480469f};
static Vector3 sfluLostEyeSecond = {234.8074951171875f, 82.05979919433594f, 122.96849822998047f};
static Vector3 savedPlayerPosition;
static Vector3 savedFighterPosition;
static int lostFrames;
void QueueNormal(StreetFightStartParameters *start) {
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
    FTBL.EV_FreeAuthorPak(1, "0260");
    if (!FTBL.EV_ControlPackageResources(0, 5, "QKBT", 0, 0)) {
        const char *path = FTBL.Filepath_Gen(0);
        FTBL.EV_ControlPackageResources(0, 0, path, (unsigned int)"QKBT", 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    }
    FTBL.EV_ControlPackageResources(2, 4, "QKBT", 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "QKBT", 0, 0);
    FTBL.CreateEventCharacter(3, 0, SFLU_BASE);
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_LoadCutsceneFromCurrentScene("0260A000");
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.FinishLoading();
    FTBL.SetRenderClockAndCompassTo(1);
    EventDebugRegister(3, NormalDebugCallback, work, 36000);
}

StreetFightEventWork *InitializeNormal(HLibTask *task) {
    StreetFightEventWork *work = (StreetFightEventWork *)FTBL.HLib_GetTaskParameter(task);
    work->task = task;
    work->state = 0;
    work->battleStatus = 0;
    work->previousNpcBatchCount = FTBL.GetNpcBatchCountOverride(35);
    work->fighterIndex = 0;
    work->fighters[0] = SFLU_SF4;
    work->fighters[1] = STREET_FIGHT_INVALID;
    work->characterReadTask = Queue_EVT_AT_ReadChr(task, sfluNormalCharacters);
    work->audienceTask = 0;
    work->fadeFrames = 0;
    work->attempts = 0;
    *(unsigned int *)&work->reserved125[3] = 0;
    work->stage = GetStreetFightStageForArea(g_currentMap);
    work->rank = FTBL.vfmDispatchControl(14, work->stage | 0x40, 0, -1);
    ResetStreetFightRounds(work);
    return work;
}

void PlaceNormal(StreetFightEventWork *work) {
    SetStreetFightCharacterVisible(SFLU_SF4, 1);
    EVT_AT_SetPos(SFLU_BASE, STREET_FIGHT_INVALID, 238.5f, 82.0f, 126.0f, 0);
    EVT_AT_SetAngY(SFLU_BASE, STREET_FIGHT_INVALID, 0.0f, 0);
    FTBL.AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFLU_BASE);
    FTBL.AT_SetLocalBaseChara(SFLU_SF4, SFLU_BASE);
    EVT_AT_SetMot(STREET_FIGHT_RYO, 306, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFLU_BASE, 0.0f, 0.0f, -1.1f, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFLU_BASE, 180.0f, 0);
    EVT_AT_SetMot(SFLU_SF4, 2732, 0.0f, -1.0f, 0x40000000, 0);
    EVT_AT_SetPos(SFLU_SF4, SFLU_BASE, 0.0f, 0.0f, 1.1f, 0);
    EVT_AT_SetAngY(SFLU_SF4, SFLU_BASE, 0.0f, 0);
    EVT_AT_SetAngY(SFLU_SF4, SFLU_BASE, 0.0f, 0);
}

void MainCallback(void *data) {
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    int index = 0;
    int level;
    float damageThreshold;
    Vector3 position;
    Vector3i rotation;
    EventDebugCheck(3, 4);
    switch (work->state) {
    case 8:
    case 9:
    case 10:
    case 11:
    case 16:
    case 17:
    case 18:
    case 19:
    case 39:
    case 40:
    case 41:
        UpdateFenceAlpha();
        break;
    default:
        break;
    }
    switch (work->state) {
    case 0:
        FTBL.ST_LoadTitleFade();
        FTBL.vfmDispatchControl(20, 0, 0, 1);
        FTBL.LoadBlurModelForCharacter_0(STREET_FIGHT_RYO, "RYO_B.MT7", 1);
        FTBL.LoadBlurModelForCharacter_0(SFLU_SF4, "SF4_B.MT7", 1);
        if (IsStreetFightBestOfThree())
            work->state = 7;
        else if (work->startParameters->subScene == 99)
            work->state = 1;
        else
            work->state = 4;
        break;
    case 1:
        if (FTBL.EV_GetPlayerFlag(11, 1794))
            FTBL.EV_authSceneCtrl(-16);
        Queue_ET_PluralControl(SFLU_PLURAL_FIRST);
        QueueAuthor(SFLU_AUTH_0);
        work->state = 2;
        break;
    case 2:
        if (FTBL.EV_authSceneCtrl(-1))
            work->state = 3;
        break;
    case 3:
        if (!FTBL.EV_authSceneCtrl(-1))
            work->state = 38;
        break;
    case 4:
        if (FTBL.EV_GetPlayerFlag(11, 1794))
            FTBL.EV_authSceneCtrl(-16);
        Queue_ET_PluralControl(SFLU_PLURAL_REPEAT);
        QueueAuthor(SFLU_AUTH_1);
        work->state = 5;
        break;
    case 5:
        if (FTBL.EV_authSceneCtrl(-1))
            work->state = 6;
        break;
    case 6:
        if (!FTBL.EV_authSceneCtrl(-1))
            work->state = 7;
        break;
    case 7:
        ToggleFence(1);
        ToggleTrainingAudience(0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFLU_SF4), 0, 1, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFLU_SF4), 0, 0, 0);
        FTBL.EV_EventSetUpHelpIcon_0(GetHelpIconForArea(g_currentMap), 0);
        PlaceNormal(work);
        FTBL.EV_LoadAudio("0260A000", 2);
        FTBL.EV_spmBeginSampleExecute(0);
        EVT_AT_CharaDisp(SFLU_GOI, 0, 1);
        FTBL.EV_ControlPackageResources(2, 2, "QKBT", 0, 0);
        FTBL.SetScenarioCharacterType(STREET_FIGHT_RYO, 4);
        FTBL.SetScenarioCharacterType(SFLU_SF4, 4);
        (void)IsStreetFightBestOfThree();
        work->state = 8;
        break;
    case 8: {
        EVT_AT_SetMot(STREET_FIGHT_RYO, 306, 0.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_SetPos(STREET_FIGHT_RYO, SFLU_BASE, 0, 0, -1.1f, 0);
        EVT_AT_SetAngY(STREET_FIGHT_RYO, SFLU_BASE, 180, 0);
        EVT_AT_SetMot(SFLU_SF4, 2732, 0.0f, -1.0f, 0x40000000U, 0);
        EVT_AT_SetPos(SFLU_SF4, SFLU_BASE, 0, 0, 1.1f, 0);
        EVT_AT_SetAngY(SFLU_SF4, SFLU_BASE, 0, 0);
        EVT_AT_SetAngY(SFLU_SF4, SFLU_BASE, 0, 0);
        FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 1, 1, 1);
        FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(SFLU_SF4), 1, 1, 1);
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0);
        FTBL.SetPluralControlFor(FTBL.ctrlGetCharaByName(SFLU_SF4), 0);
        if (IsStreetFightBestOfThree()) {
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
        }
        FTBL.GetCharaPosition(STREET_FIGHT_RYO, STREET_FIGHT_INVALID, &position, 0x40000000);
        FTBL.GetScenarioCharacterEulerAngles(STREET_FIGHT_RYO, &rotation, 0x40000000);
        FTBL.AT_SetLocalBaseChara(STREET_FIGHT_RYO, STREET_FIGHT_INVALID);
        FTBL.EV_AT_SetPos(STREET_FIGHT_RYO, 0x78000000, &position);
        FTBL.EV_AT_SetAngY_0(STREET_FIGHT_RYO, 0x78000000, &rotation, 0);
        savedPlayerPosition.x = position.x;
        savedPlayerPosition.y = position.y;
        savedPlayerPosition.z = position.z;
        savedPlayerRotation.y = rotation.y;
        FTBL.GetScenarioCharacterEulerAngles(SFLU_SF4, &rotation, 0x40000000);
        FTBL.GetCharaPosition(SFLU_SF4, STREET_FIGHT_INVALID, &position, 0x40000000);
        FTBL.AT_SetLocalBaseChara(SFLU_SF4, STREET_FIGHT_INVALID);
        FTBL.EV_AT_SetPos(SFLU_SF4, 0x78000000, &position);
        FTBL.EV_AT_SetAngY_0(SFLU_SF4, 0x78000000, &rotation, 0);
        savedFighterPosition.x = position.x;
        savedFighterPosition.y = position.y;
        savedFighterPosition.z = position.z;
        savedFighterRotation.y = rotation.y;
        FTBL.qteBattleUtil(0, 0, 0, 0, 0);
        EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 0);
        EVT_AT_SetCam(1400, STREET_FIGHT_RYO, STREET_FIGHT_RYO, 0, 0, 0);
        FTBL.EV_PlayDialogue("0260F001");
        work->state = 9;
        break;
    }
    case 9:
        if (!FTBL.camCtrlCheckIsEventCam()) {
            FTBL.EV_PlayDialogue("0260F002");
            FTBL.HLib_SetupTaskDelay(10);
            work->state = 10;
        }
        break;
    case 10: {
        FTBL.figpSetFlagModesFor(STREET_FIGHT_RYO, 0, 0);
        if (!IsStreetFightBestOfThree()) {
            FTBL.figpSetFlagModesFor(SFLU_SF4, 0, 1);
            FTBL.figpAssignModeFor(SFLU_SF4, 0);
        }
        qtePhase = 0;
        FTBL.shenStartFGST(10, 10, 0, 0);
        FTBL.vfmDispatchControl(2, 0, "QKBT", 1);
        work->rank = (int)(FTBL.vfmDispatchControl(14, (int)(work->stage | 0x40), 0, -1));
        {
            int *value = &battleLevel;
            *value = FTBL.vfmDispatchControl(14, 132, 0, -1);
            if (*value > 2)
                *value = 2;
        }
        if (!IsStreetFightBestOfThree()) {
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 0, 0, 400, 0);
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(SFLU_SF4), 0, 1, 1, 600 - 100 * battleLevel, 26);
        } else {
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 0, 0, 300, 0);
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(SFLU_SF4), 0, 1, 1, 80 * work->rank + 300, 45);
        }
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(SFLU_SF4), 0, 0x8000000, battleLevel);
        damageThreshold = (float)(600 - 100 * battleLevel) * 0.1f;
        if (!IsStreetFightBestOfThree())
            FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(SFLU_SF4), 0, 0x20000, (int)damageThreshold);
        FTBL.vfCopyPosFromEntity(0, 0, 238.5f, 126, 4);
        if (IsStreetFightBestOfThree()) {
            const int rank = FTBL.vfmDispatchControl(14, 68, 0, -1);
            FTBL.vfmDispatchControl(16, 0, (void *)((int)(rank + 1)), 1);
            FTBL.vfmDispatchControl(15, (int)(work->wins), (void *)((int)(work->losses)), 1);
        } else
            FTBL.EV_vfBattleUtil(3, 0x100, 1);
        FTBL.EV_vfBattleUtil(1, 2, 1);
        FTBL.EV_vfBattleUtil(3, 0x800, 1);
        FTBL.EV_vfBattleUtil(3, 0x80000, 1);
        FTBL.vfmQueueFightStart(2, 0x63, 1, 0, -1);
        FTBL.vfmDispatchControl(8, 0, (void *)(13), 1);
        FTBL.EV_SetPlayerFlag(12, 1794, 1);
        SetStreetFightStageFlag(work->stage);
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
            FTBL.btlSetCharaFlagBatch(SFLU_SF4, SFLU_FIGHT_PARAMETERS[index]);
        }
        work->state = 11;
        break;
    }
    case 11:
        if (!FTBL.IsFightActive()) {
            work->battleStatus = FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO));
            if (IsStreetFightBestOfThree()) {
                const int result = RecordStreetFightRound(work);
                if (result) {
                    FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
                    FTBL.spmFadeOut(0);
                    FTBL.HLib_SetupTaskDelay(30);
                    if (result == 1)
                        work->state = 38;
                    else
                        work->state = 39;
                } else
                    work->state = 8;
            } else {
                if (work->battleStatus != 3) {
                    int rank = FTBL.vfmDispatchControl(14, 68, 0, -1);
                    if (rank > 0)
                        --rank;
                    FTBL.vfmDispatchControl(14, 68, (void *)((int)(rank)), 1);
                    level = FTBL.vfmDispatchControl(14, 132, 0, -1);
                    if (level < 9)
                        ++level;
                    FTBL.vfmDispatchControl(14, 132, (void *)((int)(level)), 1);
                    FTBL.spmFadeOut(0);
                    work->state = 39;
                } else
                    work->state = 13;
                FTBL.SetScreenFilmFade(15, 0, 0, 0, 0, 255, 0, 0, 0);
                FTBL.HLib_SetupTaskDelay(15);
            }
        } else {
            if (!IsStreetFightBestOfThree())
                UpdateBattleQte(work);
        }
        break;
    case 13:
        FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 1, 1, 1);
        FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(SFLU_SF4), 1, 1, 1);
        QueueAuthor(SFLU_AUTH_5);
        work->state = 14;
        break;
    case 14:
        if (FTBL.EV_authSceneCtrl(-1))
            work->state = 15;
        break;
    case 15:
        if (!FTBL.EV_authSceneCtrl(-1))
            work->state = 16;
        break;
    case 16:
        FTBL.EV_AT_SetPos(STREET_FIGHT_RYO, 0x78000000, &savedPlayerPosition);
        FTBL.EV_AT_SetAngY_0(STREET_FIGHT_RYO, 0x78000000, &savedPlayerRotation, 0);
        EVT_AT_SetMot(STREET_FIGHT_RYO, 306, 0.0f, 1.0f, 0x40000000U, 0);
        FTBL.EV_AT_SetPos(SFLU_SF4, 0x78000000, &savedFighterPosition);
        FTBL.EV_AT_SetAngY_0(SFLU_SF4, 0x78000000, &savedFighterRotation, 0);
        EVT_AT_SetMot(SFLU_SF4, 2732, 0.0f, -1.0f, 0x40000000U, 0);
        FTBL.EV_LoadAudio("0260A000", 2);
        FTBL.EV_spmBeginSampleExecute(0);
        FTBL.figpSetFlagModesFor(STREET_FIGHT_RYO, 0, 0);
        FTBL.figpSetFlagModesFor(SFLU_SF4, 0, 0);
        FTBL.qteBattleUtil(0, 0, 0, 0, 0);
        EVT_AT_SetCam(1401, STREET_FIGHT_RYO, STREET_FIGHT_RYO, 0, 0, 0);
        if (FTBL.shenGetRandomInt32(2))
            EVT_AT_SetVoice("0260B045", 30);
        else
            EVT_AT_SetVoice("0260B046", 30); 
        FTBL.SetScreenFilmFade(15, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.HLib_SetupTaskDelay(15);
        work->state = 17;
        break;
    case 17:
        if (!FTBL.camCtrlCheckIsEventCam()) {
            qtePhase = 0;
            FTBL.shenStartFGST(10, 10, 0, 0);
            FTBL.vfmDispatchControl(2, 0, "QKBT", 1);
            {
                int *value = &battleLevel;
                *value = FTBL.vfmDispatchControl(14, 132, 0, -1);
                if (*value > 2)
                    *value = 2;
            }
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0, 0, 0, 400, 0);
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(SFLU_SF4), 0, 1, 1, (600 - 100 * battleLevel), 45);
            FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(SFLU_SF4), 0, 0x8000000, battleLevel);
            damageThreshold = (float)(600 - 100 * battleLevel) * 0.1f;
            if (!IsStreetFightBestOfThree())
                FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(SFLU_SF4), 0, 0x20000, (int)damageThreshold);
            if (IsStreetFightBestOfThree()) {
                const int rank = FTBL.vfmDispatchControl(14, 68, 0, -1);
                FTBL.vfmDispatchControl(16, 0, (void *)((int)(rank + 1)), 1);
            } else
                FTBL.EV_vfBattleUtil(3, 0x100, 1);
            FTBL.vfCopyPosFromEntity(0, 0, 238.5f, 126, 4);
            FTBL.EV_vfBattleUtil(1, 2, 1);
            FTBL.EV_vfBattleUtil(3, 0x800, 1);
            FTBL.vfmQueueFightStart(2, 0x63, 1, 0, -1);
            FTBL.vfmDispatchControl(8, 0, (void *)(13), 1);
            work->state = 18;
        }
        break;
    case 18:
        if (!FTBL.IsFightActive()) {
            work->battleStatus = FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO));
            if (work->battleStatus != 3) {
                RecordStreetFightDefeat(work->stage);
                FTBL.spmFadeOut(0);
                work->state = 39;
            } else {
                RecordStreetFightVictory(work->stage);
                work->state = 19;
            }
            FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        } else {
            if (!IsStreetFightBestOfThree())
                UpdateBattleQte(work);
        }
        break;
    case 19:
        if (!FTBL.filmCheckFadeTrackColour()) {
            FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 1, 1, 1);
            FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(SFLU_SF4), 1, 1, 1);
            ToggleFence(0);
            ToggleTrainingAudience(1);
            FTBL.AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFLU_BASE);
            FTBL.AT_SetLocalBaseChara(SFLU_SF4, SFLU_BASE);
            FTBL.vfmDispatchControl(20, 0, 0, 0);
            FTBL.EV_EventSetUpHelpIcon_0(0, 0);
            work->state = 20;
        }
        break;
    case 20:
        QueueAuthor(SFLU_AUTH_6);
        work->state = 21;
        break;
    case 21:
        if (FTBL.EV_authSceneCtrl(-1)) {
            ToggleTrainingAudience(0);
            work->state = 22;
        }
        break;
    case 22:
        if (FTBL.EV_authSceneCtrl(-1))
            break;
        ToggleTrainingAudience(1);
        trainingLevel = 0;
        work->state = 23;
        break;
    case 23:
        FTBL.SetLoadingBusy();
        ToggleTrainingAudience(0);
        FTBL.EV_LoadAudio("0260A000", 2);
        FTBL.EV_spmBeginSampleExecute(0);
        EVT_AT_SetPos(SFLU_BASE, STREET_FIGHT_INVALID, 239.5f, 82.0f, 124.0f, 0);
        EVT_AT_SetAngY(SFLU_BASE, STREET_FIGHT_INVALID, 0, 0);
        EVT_AT_SetMot(STREET_FIGHT_RYO, 10179, 0.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_SetPos(STREET_FIGHT_RYO, SFLU_BASE, 0, 0, 1.1f, 0);
        EVT_AT_SetAngY(STREET_FIGHT_RYO, SFLU_BASE, 0, 0);
        EVT_AT_SetMot(SFLU_SF4, 10177, 0.0f, 1.0f, 0x40000000U, 0);
        EVT_AT_SetPos(SFLU_SF4, SFLU_BASE, 0, 0, -1.1f, 0);
        EVT_AT_SetAngY(SFLU_SF4, SFLU_BASE, 0, 0);
        FTBL.SetMotionReactFlags(FTBL.scenFindMotionwork(STREET_FIGHT_RYO), 13);
        FTBL.SetMotionReactFlags(FTBL.scenFindMotionwork(SFLU_SF4), 13);
        EVT_AT_CharaDisp(STREET_FIGHT_RYO, 0, 0);
        EVT_AT_CharaDisp(SFLU_SF4, 0, 0);
        FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(SFLU_SF4));
        FTBL.SelectCharacterFacialNode(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 1);
        FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 1, 0, 1);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 1, 0, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 1, 1, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFLU_SF4), 1, 0, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(SFLU_SF4), 1, 1, 0);
        ConfigureTrainingHands();
        if (cameraManualMode) {
            Vector3 trainingCamera = {241.81500244140625f, 84.55899810791016f, 129.4429931640625f};
            Vector3 trainingTarget = {237.87210083007812f, 82.35199737548828f, 125.6709976196289f};
            FTBL.camCtrlSetState(9);
            FTBL.camCtrlSetNextPosition(&trainingCamera);
            FTBL.camCtrlSetTargetPosition(&trainingTarget);
        } else {
            EVT_AT_SetCam(1410, SFLU_BASE, SFLU_BASE, 0, 0, 0);
        }
        trainingFrames = 0;
        qtePhase = 0;
        FTBL.qteBattleUtil(0, 0, 0, 0, 0);
        FTBL.qteBattleUtil(8, 25, 6, SFLU_TRAINING_WAIT[trainingLevel], 0);
        FTBL.qteBattleUtil(9, 16, 1024, 1, 256);
        FTBL.qteBattleUtil(10, 0, 0, 0, 0);
        FTBL.qteBattleUtil(1, 0, 0, 0, 0);
        FTBL.qteBattleUtil(2, 0, 0, 0, 0);
        FTBL.SetScreenFilmFade(10, 255, 0, 0, 0, 0, 0, 0, 0);
        work->state = 24;
        break;
    case 24: {
        switch (trainingFrames++) {
        case 23:
        case 43:
        case 83:
        case 103:
            FTBL.SetShowQteFlagForButtonType(SFLU_TRAINING_BUTTONS[qtePhase], 15, 0);
            trainingInputFailed = 0;
            ++qtePhase;
            break;
        case 155:
            FTBL.qteBattleUtil(3, 0, 0, 0, 0);
            work->state = 25;
            break;
        case 42:
        case 62:
        case 102:
        case 122:
            if (trainingInputFailed) {
                trainingFrames = 0;
                work->state = 27;
                break;
            }
            break;
        case 40:
            if (!cameraManualMode)
                EVT_AT_SetCam(1411, SFLU_BASE, SFLU_BASE, 0, 0, 0);
            break;
        case 70:
            if (!cameraManualMode)
                EVT_AT_SetCam(1412, SFLU_BASE, SFLU_BASE, 0, 0, 0);
            break;
        case 105:
            if (!cameraManualMode)
                EVT_AT_SetCam(1413, SFLU_BASE, SFLU_BASE, 0, 0, 0);
            break;
        case 79:
            FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 6, 0, 1);
            break;
        case 151:
            FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 1, 0, 1);
            break;
        default:
            break;
        }
        if (trainingFrames > 23) {
            const unsigned int status = FTBL.GetCurrentQteStatus();
            if (status == 0xa000 || !status)
                trainingInputFailed = 1;
        }
        break;
    }
    case 25: {
        const int result = FTBL.qteBattleUtil(4, 0, 0, 0, 0);
        if (result) {
            trainingFrames = 0;
            if (result == 1) {
                trainingInputFailed = 0;
                work->state = 26;
            } else {
                trainingInputFailed = 1;
                work->state = 27;
            }
        }
        break;
    }
    case 26: {
        switch (trainingFrames++) {
        case 5:
            EVT_AT_SetMot(STREET_FIGHT_RYO, 10182, 0.0f, 1.0f, 0x40000000U, 0);
            EVT_AT_SetPos(STREET_FIGHT_RYO, SFLU_BASE, 1.100000023841858f, 0.0f, 0.33500000834465027f, 0);
            EVT_AT_SetAngY(STREET_FIGHT_RYO, SFLU_BASE, 115, 0);
            EVT_AT_SetMot(SFLU_SF4, 10181, 0.0f, 1.0f, 0x40000000U, 0);
            EVT_AT_SetPos(SFLU_SF4, SFLU_BASE, 0.07999999821186066f, 0.0f, 0.8999999761581421f, 0);
            EVT_AT_SetAngY(SFLU_SF4, SFLU_BASE, 115, 0);
            FTBL.SetMotionReactFlags(FTBL.scenFindMotionwork(STREET_FIGHT_RYO), 13);
            FTBL.SetMotionReactFlags(FTBL.scenFindMotionwork(SFLU_SF4), 13);
            FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 1, 0, 1);
            ConfigureVictoryHands();
            break;
        case 0:
            if (!cameraManualMode) {
                EVT_AT_SetCam(1415, SFLU_BASE, SFLU_BASE, 0, 0, 0);
            }
            break;
        case 16:
            if (!cameraManualMode) {
                EVT_AT_SetCam(1416, SFLU_BASE, SFLU_BASE, 0, 0, 0);
            }
            FTBL.camCtrlSetRotationXYZ(0, 0, 0);
            break;
        case 55:
            if (!cameraManualMode) {
                EVT_AT_SetCam(1417, SFLU_BASE, SFLU_BASE, 0, 0, 0);
            }
            break;
        case 9:
            FTBL.camCtrlSetRotationXYZ(0, .02f, .8500000238418579f);
            break;
        case 41:
            FTBL.camCtrlSetRotationXYZ(.1f, 0, .6499999761581421f);
            break;
        case 62:
            FTBL.camCtrlSetRotationXYZ(.1f, .1f, .6000000238418579f);
            break;
        case 83:
            FTBL.camCtrlSetRotationXYZ(0, .25f, .8500000238418579f);
            break;
        case 110:
            work->state = 28;
            break;
        default:
            break;
        }
        break;
    }
    case 27: {
        switch (trainingFrames++) {
        case 0:
            EVT_AT_SetMot(STREET_FIGHT_RYO, 10183, 0.0f, 1.0f, 0x40000000U, 0);
            EVT_AT_SetPos(SFLU_BASE, STREET_FIGHT_INVALID, 238.5f, 82.0f, 126.0f, 0);
            EVT_AT_SetAngY(SFLU_BASE, STREET_FIGHT_INVALID, 0, 0);
            EVT_AT_SetPos(STREET_FIGHT_RYO, SFLU_BASE, 0, 0, 0, 0);
            EVT_AT_SetAngY(STREET_FIGHT_RYO, SFLU_BASE, 180, 0);
            EVT_AT_CharaDisp(STREET_FIGHT_RYO, 0, 0);
            EVT_AT_CharaDisp(SFLU_SF4, 0, 1);
            FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(SFLU_SF4), 0.0f, 0);
            FTBL.SetMotionReactFlags(FTBL.scenFindMotionwork(STREET_FIGHT_RYO), 13);
            FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 11, 0, 1);
            if (!cameraManualMode) {
                EVT_AT_SetCam(1420, SFLU_BASE, SFLU_BASE, 0, 0, 0);
                FTBL.camCtrlSetRotationXYZ(0, .3f, .65f);
            }
            break;
        case 90:
            if (trainingLevel < 2)
                ++trainingLevel;
            work->state = 28;
            break;
        }
        break;
    }
    case 28:
        EVT_AT_SetPos(SFLU_BASE, STREET_FIGHT_INVALID, 238.5f, 82.0f, 126.0f, 0);
        if (g_entryPoint == 3) {
            FTBL.dbgPrintEventMessage("何かキーを押すとリトライ\n");
            if (FTBL.GetCurrentControlInput(7) & 0x606U) {
                FTBL.qteBattleUtil(5, 0, 0, 0, 0);
                FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
                FTBL.HLib_SetupTaskDelay(30);
                FTBL.ctrlStopSubtitles();
                work->state = 23;
            }
        } else {
            FTBL.qteBattleUtil(5, 0, 0, 0, 0);
            FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            if (trainingInputFailed) {
                FTBL.HLib_SetupTaskDelay(90);
                work->state = 23;
            } else {
                FTBL.spmFadeOut(0);
                FTBL.HLib_SetupTaskDelay(30);
                work->state = 29;
            }
        }
        break;
    case 29: {
        EventDateTime date;
        FTBL.GetCurrentDateTime((DateTime *)&date);
        if (date.hour > 17 || (date.hour == 17 && date.minute > 29))
            ++date.day;
        date.hour = 17;
        date.minute = 30;
        date.second = 0;
        FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&date);
        FTBL.shenSetTimeSimulationTo(0);
        FTBL.EV_LoadAudio("0260A000", 3);
        FTBL.EV_spmBeginSampleExecute(0);
        ToggleTrainingAudience(1);
        Queue_ET_PluralControl(SFLU_PLURAL_EVENING);
        QueueAuthor(SFLU_AUTH_2);
        work->state = 30;
        break;
    }
    case 30:
        if (FTBL.EV_authSceneCtrl(-1))
            work->state = 31;
        break;
    case 31:
        if (!FTBL.EV_authSceneCtrl(-1))
            work->state = 32;
        break;
    case 32:
        Queue_ET_PluralControl(SFLU_PLURAL_RESULT);
        QueueAuthor(SFLU_AUTH_3);
        work->state = 33;
        break;
    case 33:
        if (FTBL.EV_authSceneCtrl(-1))
            work->state = 34;
        break;
    case 34:
        if (!FTBL.EV_authSceneCtrl(-1))
            work->state = 35;
        break;
    case 35: {
        SetStreetFightCharacterVisible(SFLU_KF1, 0);
        SetStreetFightCharacterVisible(SFLU_KF2, 0);
        SetStreetFightCharacterVisible(SFLU_KF3, 0);
        SetStreetFightCharacterVisible(SFLU_KR1, 0);
        SetStreetFightCharacterVisible(SFLU_KR2, 0);
        SetStreetFightCharacterVisible(SFLU_SF4, 0);
        SetStreetFightCharacterVisible(SFLU_SF9, 0);
        SetStreetFightCharacterVisible(SFLU_BF6, 0);
        SetStreetFightCharacterVisible(SFLU_GOI, 0);
        QueueAuthor(SFLU_AUTH_4);
        work->state = 36;
        break;
    }
    case 36:
        if (FTBL.EV_authSceneCtrl(-1))
            work->state = 37;
        break;
    case 37:
        if (!FTBL.EV_authSceneCtrl(-1))
            work->state = 38;
        break;
    case 38:
        FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 1, 1, 1);
        FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(SFLU_SF4), 1, 1, 1);
        if (IsStreetFightBestOfThree())
            FTBL.HLib_SetCurrentTaskTo(RetryBeginCallback);
        else
            FTBL.HLib_SetCurrentTaskTo(CleanupNormalCallback);
        break;
    case 39:
        if (!FTBL.filmCheckFadeTrackColour()) {
            FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 1, 1, 1);
            FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(SFLU_SF4), 1, 1, 1);
            QueueStreetFightSkipWait(DefeatConversation, work, 1);
            if (!IsStreetFightBestOfThree())
                work->state = 42;
            else
                work->state = 40;
        }
        break;
    case 40:
        work->state = 41;
        break;
    case 41:
        if (!FTBL.filmCheckFadeTrackColour()) {
            FTBL.EV_EnableCharacterDisplay(STREET_FIGHT_RYO);
            ToggleFence(0);
            ToggleTrainingAudience(1);
            work->state = 38;
        }
        break;
    case 42:
        FTBL.EV_ResetFreeTalkFlags();
        FTBL.Queue_ST_LoadTitleFadeIn(SFLU_RETRY_TITLE);
        ChangeStreetFightAudienceMode(work->audienceTask, 2, 1);
        work->state = 43;
        work->frameCounter = 60;
        break;
    case 43:
        if (--work->frameCounter == 0) {
            FTBL.ST_LoadTitleFade();
            work->frameCounter = 60;
            work->state = 44;
        }
        break;
    case 44:
        if (--work->frameCounter == 0)
            work->state = 38;
        break;
    default:
        break;
    }
}

void NormalDebugCallback(void *data) {
    CleanupNormalCallback(data);
}

void CleanupNormalCallback(void *data) {
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    StreetFightStartParameters *start = work->startParameters;
    unsigned int entry = start->subScene;
    int result = work->battleStatus;
    FTBL.EV_NPC_NewLoadSw_0(1);
    EventDebugUnregister(3);
    ET_AT_KillWork(work->characterReadTask);
    FTBL.SetPlayerControlEnabled(0);
    FTBL.SetScenarioCharacterKillMode(SFLU_BASE, 1);
    FTBL.GetNpcBatchCountOverride(work->previousNpcBatchCount);
    FTBL.EV_FreeAuthorPak(0, "0260");
    if (!(start->flags & 1))
        FTBL.EV_ControlPackageResources(0, 6, "QKBT", 0, 0);
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    if (entry == 99) {
        FTBL.LoadCutsceneAndSoundProgramForArea();
        FTBL.EV_AT_SetPos(STREET_FIGHT_RYO, 0x78000000, &sfluStoryExitPosition);
        FTBL.EV_AT_SetAngY_0(STREET_FIGHT_RYO, 0x78000000, &sfluStoryExitRotation, 0);
        FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(STREET_FIGHT_RYO), 0xf003, 0);
        FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, 0);
        FTBL.camCtrlSetState(0);
        FTBL.camCtrlSetFovTransitionTo(1);
    } else if (IsStreetFightBestOfThree()) {
        FTBL.EV_scenQueueLoad(4, SHEN_NAME4('O', 'M', 'A', 'K'), 1);
    } else if (result == 3) {
        FTBL.EV_stepSetFlags(0x16a, 0);
        FTBL.EV_scenQueueLoad(g_actNumber, SFLU_STORY_NEXT_AREA, 0);
    } else {
        FTBL.LoadCutsceneAndSoundProgramForArea();
        FTBL.EV_scenQueueLoad(g_actNumber, SFLU_STORY_EXIT_AREA, 1);
    }
    FTBL.EV_SetPlayerFlag(12, 1233, 1);
    if (start->signal)
        *start->signal = 0;
}

void RetryBeginCallback(void *data) {
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

void RetryChoiceCallback(void *data) {
    unsigned int choice = PollStreetFightActionChoice();
    if (choice) {
        FTBL.ctrlStopSubtitles();
        if (choice == 38)
            ResetMain((StreetFightEventWork *)data);
        else
            RetryCancelCallback(data);
    }
}

void ResetMain(StreetFightEventWork *work) {
    FTBL.EV_ResetFreeTalkFlags();
    work->state = 7;
    work->battleStatus = 0;
    work->auxiliaryState = 0;
    ResetStreetFightRounds(work);
    FTBL.HLib_SetCurrentTaskTo(MainCallback);
    ChangeStreetFightAudienceMode(work->audienceTask, 2, -1);
    FTBL.EV_LoadAudio("0260A000", 2);
    FTBL.EV_spmBeginSampleExecute(0);
}

void RetryCancelCallback(void *data) {
    FTBL.HLib_SetCurrentTaskTo(CleanupNormalCallback);
}

void UpdateBattleQte(StreetFightEventWork *work) {
    if (IsStreetFightBestOfThree())
        return;
    switch (qtePhase) {
    case 0:
        if ((FTBL.btlGetCharaFlag(1, FTBL.ctrlGetCharaByName(STREET_FIGHT_RYO), 0.0f) < 15 &&
                FTBL.btlGetCharaFlag(1, FTBL.ctrlGetCharaByName(SFLU_SF4), 0.0f) < 50) ||
            FTBL.btlGetCharaFlag(1, FTBL.ctrlGetCharaByName(SFLU_SF4), 0.0f) < 15) {
            if (!(FTBL.GetFightStateFlags() & 0x10) && (FTBL.EV_vfBattleUtil(2, STREET_FIGHT_RYO, 13) & 1)) {
                if (work->state == 11) {
                    FTBL.qteBattleUtil(8, 25, 6, sfluQteFirstWait[battleLevel], 0);
                    FTBL.qteBattleUtil(9, 64, 1024, 0, 0);
                } else {
                    FTBL.qteBattleUtil(8, 25, 6, sfluQteSecondWait[battleLevel], 0);
                    if (FTBL.EV_vfBattleUtil(7, 0x40000004, -1))
                        FTBL.qteBattleUtil(9, 4, 4, 512, 0);
                    else
                        FTBL.qteBattleUtil(9, 64, 64, 512, 0);
                }
                FTBL.qteBattleUtil(1, 0, 0, 0, 0);
                FTBL.qteBattleUtil(2, 0, 0, 0, 0);
                FTBL.EV_vfBattleUtil(4, 0x1000, 1);
                qtePhase = 1;
            }
        }
        break;
    case 1:
        FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(SFLU_SF4), 1, 0x20000, 0);
        FTBL.figpSetFlagModesFor(STREET_FIGHT_RYO, 0, 1);
        if (work->state == 18)
            FTBL.figpSetFlagModesFor(SFLU_SF4, 0, 1);
        FTBL.figpAssignModeFor(STREET_FIGHT_RYO, 1);
        FTBL.figpSetFlagsBlendForBlur(STREET_FIGHT_RYO, 10, 2);
        FTBL.figpAssignModeFor(SFLU_SF4, 1);
        FTBL.figpSetFlagsBlendForBlur(SFLU_SF4, 10, 2);
        FTBL.qteBattleUtil(3, 0, 0, 0, 0);
        qtePhase = 2;
        break;
    case 2: {
        int result = FTBL.qteBattleUtil(4, 0, 0, 0, 0);
        if (result) {
            if (result == 1) {
                ResetQteCharacters(work);
                if (work->state == 11) {
                    FTBL.vfmDispatchControl(7, STREET_FIGHT_RYO, (void *)(0x8000ad6), 1);
                    qtePhase = 4;
                } else if (FTBL.EV_vfBattleUtil(7, 0x40000004, -1)) {
                    FTBL.QueueExtraTelop("連手砲", 99);
                    FTBL.vfmDispatchControl(7, STREET_FIGHT_RYO, (void *)(0x8000ad7), 1);
                    qtePhase = 3;
                } else {
                    FTBL.vfmDispatchControl(7, STREET_FIGHT_RYO, (void *)(0x8000ad9), 1);
                    qtePhase = 4;
                }
            } else {
                ResetQteCharacters(work);
                EVT_AT_SetCam(1421, STREET_FIGHT_RYO, STREET_FIGHT_RYO, 0, 0, 0);
                FTBL.vfmDispatchControl(7, SFLU_SF4, (void *)(0x8000ada), 1);
                qtePhase = 5;
            }
        }
        break;
    }
    case 3:
        if (work->state != 11 && FTBL.EV_vfBattleUtil(7, 0x40000004, -1)) {
            if (++trainingFrames == 292) {
                FTBL.EV_vfBattleUtil(3, 4, 1);
                FTBL.QueueExtraTelop("連手砲", 0x40000000);
                trainingFrames = 0;
                qtePhase = 6;
            }
        }
        break;
    case 4:
        if (work->state == 11) {
            if (FTBL.GetMotionFrame(FTBL.scenFindMotionwork(SFLU_SF4), 0) >= 31.0f) {
                FTBL.EV_vfBattleUtil(5, SFLU_SF4, 1);
                qtePhase = 6;
            }
        } else if (!FTBL.EV_vfBattleUtil(7, 0x40000004, -1)) {
            if (FTBL.GetMotionFrame(FTBL.scenFindMotionwork(SFLU_SF4), 0) >= 33.0f) {
                FTBL.EV_vfBattleUtil(5, SFLU_SF4, 1);
                qtePhase = 6;
            }
        }
        break;
    case 5:
        if (FTBL.GetMotionFrame(FTBL.scenFindMotionwork(STREET_FIGHT_RYO), 0) >= 51.0f)
            qtePhase = 6;
        break;
    case 6:
        FTBL.qteBattleUtil(5, 0, 0, 0, 0);
        ++qtePhase;
        break;
    }
}

void ToggleTrainingAudience(int enabled) {
    SetStreetFightCharacterVisible(SFLU_JOP, enabled);
    SetStreetFightCharacterVisible(SFLU_KF1, enabled);
    SetStreetFightCharacterVisible(SFLU_KF2, enabled);
    SetStreetFightCharacterVisible(SFLU_KF3, enabled);
    SetStreetFightCharacterVisible(SFLU_KR1, enabled);
    SetStreetFightCharacterVisible(SFLU_KR2, enabled);
    SetStreetFightCharacterVisible(SFLU_SF9, enabled);
    SetStreetFightCharacterVisible(SFLU_BF6, enabled);
    SetStreetFightCharacterVisible(SFLU_GOI, enabled);
}

void ToggleFence(int enabled) {
    int index;
    if (enabled) {
        FTBL.SetObjectFlagByIndex(1, 0);
        FTBL.SetObjectFlagByIndex(2, 0);
        FTBL.EV_DisableCharacterDisplay(SFLU_A734);
        FTBL.EV_SetCharacterAlphaEnabled(SFLU_A734, 1);
        for (index = 0; index < 5; ++index) {
            shen_name character;
            FTBL.ApplyCharacterGeometryLighting(FTBL.ctrlGetCharaByName(character = sfluFenceCharacters[index]), 1);
            FTBL.EV_EnableCharacterDisplay(character);
            FTBL.EV_SetCharacterAlphaEnabled(character, 1);
            FTBL.SetScenarioCharacterRadius(character, 10.0f);
        }
    } else {
        for (index = 0; index < 5; ++index)
            FTBL.EV_DisableCharacterDisplay(sfluFenceCharacters[index]);
        FTBL.SetObjectFlagByIndex(1, 1);
        FTBL.SetObjectFlagByIndex(2, 1);
        FTBL.EV_EnableCharacterDisplay(SFLU_A734);
        FTBL.EV_SetCharacterAlphaEnabled(SFLU_A734, 0);
    }
}

void UpdateFenceAlpha(void) {
    Vector3 camera, position;
    int index;
    FTBL.camCtrlGetNextCamPos(&camera);
    for (index = 0; index < 5; ++index) {
        shen_name character;
        float distance;
        FTBL.GetCharaPosition(character = sfluFenceCharacters[index], -1, &position, 0x40000000);
        switch (character) {
        case SFLU_AMI1:
        case SFLU_AMI4:
            distance = __builtin_fabsf(position.x - camera.x);
            break;
        case SFLU_AMI2:
        case SFLU_AMI3:
            distance = __builtin_fabsf(position.z - camera.z);
            break;
        case SFLU_AMI5:
        default:
            distance = __builtin_fabsf(position.y - camera.y);
            break;
        }
        if (distance < 0.2f)
            distance = 0.2f;
        if (distance > 3.2f)
            distance = 3.2f;
        distance = (distance - 0.2f) / 3.0f;
        character = sfluFenceCharacters[index];
        FTBL.EV_SetCharacterAlphaEnabled(character, 1);
        FTBL.SetScenarioCharacterTransparency(character, distance);
    }
}

void LostExpressionCallback(void *data) {
    switch (lostFrames) {
    case 0:
        FTBL.SetFacialBlinkDelay(FTBL.ctrlGetCharaByName(SFLU_SF4), 0);
        FTBL.SetEyeToChara(FTBL.ctrlGetCharaByName(SFLU_SF4), &sfluLostEyeFirst, 1, 2);
        break;
    case 120: {
        Vector3 camera;
        FTBL.camCtrlGetNextCamPos(&camera);
        FTBL.SetEyeToChara(FTBL.ctrlGetCharaByName(SFLU_SF4), &sfluLostEyeSecond, 12, 2);
        break;
    }
    case 150:
        FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.spmFadeOut(0);
        break;
    case 179:
        FTBL.SetEyeToChara(FTBL.ctrlGetCharaByName(SFLU_SF4), 0, 16, 0);
        FTBL.SetFacialBlinkDelay(FTBL.ctrlGetCharaByName(SFLU_SF4), 90);
        FTBL.HLib_DestroyCurrentTask();
        break;
    }
    ++lostFrames;
}

int DefeatConversation(void *data) {
    StreetFightEventWork *work = (StreetFightEventWork *)data;
    shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_CharaDisp(STREET_FIGHT_RYO, 0, 1);
    FTBL.EV_SetCharacterAlphaEnabled(fighter, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(fighter), 1, 1, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(fighter), 1, 0, 0);
    EVT_AT_SetPos(fighter, SFLU_BASE, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(fighter, SFLU_BASE, 0.0f, 0);
    EVT_AT_SetMot(fighter, 10185, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetVoice("0260B027", 41);
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 2);
    FTBL.EV_spmBeginSampleExecute(0);
    EVT_AT_SetCam(8099, SFLU_BASE, SFLU_BASE, 0, 0, 1);
    lostFrames = 0;
    FTBL.HLib_EnqueueTaskWithoutParameter(LostExpressionCallback, 5, 11, TASK_EVNT);
    return 180;
}

void ConfigureTrainingHands(void) {
    Queue_EVT_AT_CtrlHandUp_1(STREET_FIGHT_RYO, 0, (void *)areaLoopHandPoses[11], 0, 0);
    Queue_EVT_AT_CtrlHandUp_1(STREET_FIGHT_RYO, 1, (void *)areaLoopHandPoses[11], 0, 0);
    Queue_EVT_AT_CtrlHandUp_1(STREET_FIGHT_RYO, 0, (void *)areaLoopHandPoses[18], 4, 65);
    Queue_EVT_AT_CtrlHandUp_1(STREET_FIGHT_RYO, 1, (void *)areaLoopHandPoses[12], 4, 70);
    Queue_EVT_AT_CtrlHandUp_1(STREET_FIGHT_RYO, 0, (void *)areaLoopHandPoses[12], 4, 125);
    Queue_EVT_AT_CtrlHandUp_1(STREET_FIGHT_RYO, 1, (void *)areaLoopHandPoses[18], 4, 125);
    Queue_EVT_AT_CtrlHandUp_1(STREET_FIGHT_RYO, 0, (void *)areaLoopHandPoses[11], 4, 145);
    Queue_EVT_AT_CtrlHandUp_1(STREET_FIGHT_RYO, 1, (void *)areaLoopHandPoses[11], 4, 145);
    Queue_EVT_AT_CtrlHandUp_1(SFLU_SF4, 0, (void *)areaLoopHandPoses[11], 0, 0);
    Queue_EVT_AT_CtrlHandUp_1(SFLU_SF4, 1, (void *)areaLoopHandPoses[11], 0, 0);
    Queue_EVT_AT_CtrlHandUp_1(SFLU_SF4, 0, (void *)areaLoopHandPoses[12], 4, 42);
    Queue_EVT_AT_CtrlHandUp_1(SFLU_SF4, 1, (void *)areaLoopHandPoses[18], 4, 42);
    Queue_EVT_AT_CtrlHandUp_1(SFLU_SF4, 0, (void *)areaLoopHandPoses[18], 4, 56);
    Queue_EVT_AT_CtrlHandUp_1(SFLU_SF4, 1, (void *)areaLoopHandPoses[44], 4, 56);
    Queue_EVT_AT_CtrlHandUp_1(SFLU_SF4, 1, (void *)areaLoopHandPoses[11], 4, 76);
    Queue_EVT_AT_CtrlHandUp_1(SFLU_SF4, 0, (void *)areaLoopHandPoses[44], 4, 125);
    Queue_EVT_AT_CtrlHandUp_1(SFLU_SF4, 1, (void *)areaLoopHandPoses[1], 4, 125);
    Queue_EVT_AT_CtrlHandUp_1(SFLU_SF4, 0, (void *)areaLoopHandPoses[11], 4, 149);
    Queue_EVT_AT_CtrlHandUp_1(SFLU_SF4, 1, (void *)areaLoopHandPoses[11], 4, 149);
}

void ConfigureVictoryHands(void) {
    Queue_EVT_AT_CtrlHandUp_1(STREET_FIGHT_RYO, 0, (void *)areaLoopHandPoses[11], 0, 0);
    Queue_EVT_AT_CtrlHandUp_1(STREET_FIGHT_RYO, 1, (void *)areaLoopHandPoses[11], 0, 0);
    Queue_EVT_AT_CtrlHandUp_1(STREET_FIGHT_RYO, 0, (void *)areaLoopHandPoses[18], 4, 28);
    Queue_EVT_AT_CtrlHandUp_1(STREET_FIGHT_RYO, 1, (void *)areaLoopHandPoses[18], 4, 28);
    Queue_EVT_AT_CtrlHandUp_1(STREET_FIGHT_RYO, 0, (void *)areaLoopHandPoses[11], 16, 40);
    Queue_EVT_AT_CtrlHandUp_1(STREET_FIGHT_RYO, 1, (void *)areaLoopHandPoses[11], 16, 40);
    Queue_EVT_AT_CtrlHandUp_1(STREET_FIGHT_RYO, 0, (void *)areaLoopHandPoses[18], 4, 78);
    Queue_EVT_AT_CtrlHandUp_1(STREET_FIGHT_RYO, 1, (void *)areaLoopHandPoses[18], 16, 100);
}

void ResetQteCharacters(StreetFightEventWork *work) {
    shen_name fighter = work->fighters[work->fighterIndex];
    EVT_AT_SetPos(fighter, STREET_FIGHT_INVALID, 238.5f, 82.0f, 126.0f, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_INVALID, 238.5f, 82.0f, 126.0f, 0);
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, 0);
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(fighter), 71, 0, 0);
}
#endif
#endif
#else
#include "s2/areas/area_sflu.hpp"
#include "s2/author.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/debug.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/door.hpp"
#include "s2/face.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/load_title.hpp"
#include "s2/npc.hpp"
#include "s2/areas/numeric_event.hpp"
#include "s2/fight_stage.hpp"
#include "s2/flag.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/sk_event.hpp"
#include "s2/street_fight_conversation.hpp"
#include "s2/street_fight_tables.hpp"
#include "s2/ui.hpp"
#include <cmath>
#include "s2/model_component.hpp"
#include "s2/battle_qte.hpp"
#include "s2/step_event.hpp"
#include "s2/advice.hpp"
#include "s2/lighting.hpp"
#include "s2/extra_telop.hpp"
#include "s2/input.hpp"
#include "s2/event_call.hpp"
#include "s2/areas/area_sfts.hpp"
#include <cstring>
namespace shenmue::s2 {
namespace {
std::int32_t debugOpponentCount{};
shen_name debugLoadCharacters[2]{};
std::int32_t choicePhase{};
std::uint32_t ControlChoice(std::int32_t enabled) {
    if (!enabled) {
        choicePhase = 0;
        return 0;
    }
    switch (choicePhase) {
    case 0:
        if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            ++choicePhase;
        return 0;
    case 1:
        if (!GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            ++choicePhase;
        return 0;
    case 2: {
        const std::uint32_t choice = shenGetCtrlSpeakIconFlag();
        choicePhase = 0;
        return choice;
    }
    default:
        return 0;
    }
}
std::uint32_t PollChoice() {
    if (!choicePhase) {
        if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            ++choicePhase;
        return 0;
    }
    if (choicePhase == 1) {
        if (!GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            ++choicePhase;
        return 0;
    }
    if (choicePhase == 2) {
        const std::uint32_t choice = shenGetCtrlSpeakIconFlag();
        choicePhase = 0;
        return choice;
    }
    return 0;
}
void DebugMainCallback(void *);
void MainCallback(void *);
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
std::int32_t DebugIntroConversation(void *);
std::int32_t DebugAcceptConversation(void *);
std::int32_t DebugVictoryConversation(void *);
std::int32_t DebugNextOpponentConversation(void *);
std::int32_t DebugDefeatConversation(void *);
std::int32_t DebugWithdrawalConversation(void *);
std::int32_t DebugNextMatchConversation(void *);
void DebugCharacterLoadCallback(void *);
std::int32_t DebugContinueConversation(void *);
std::int32_t DebugCompletedConversation(void *);
std::int32_t DebugStopConversation(void *);
std::int64_t QueueDebugCharacterLoad(HLib::Task *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(DebugCharacterLoadCallback, 6, 0x0b, sizeof(StreetFightCharacterLoadWork), STREET_FIGHT_EVENT_TASK);
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
    work->package = SFLU_DEBUG_PACKAGE;
    work->battleFrames = 50;
    work->motions[0] = 0;
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
    work->characterReadTask = Queue_EVT_AT_ReadChr(task, SFLU_DEBUG_CHARACTERS);
    return work;
}
void LoadDebugResources(StreetFightEventWork *work) {
    work->motions[0] = 0;
    EV_FreeAuthorPak(1, "0260");
    const char *path = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("ENMY"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    path = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("QKBT"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QKBT"), 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QKBT"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, SFLU_BASE);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0260A000");
    EV_LoadAudio("0260A000", 0);
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
    EV_EnableCharacterDisplay(SFLU_FUY);
    EV_EnsureCharacterDraw(SFLU_FUY);
    EV_EnableCharacterDisplay(fighter);
    EV_EnsureCharacterDraw(fighter);
    work->playerPosition = {-3.3f, -0.5f, -5.5f};
    work->bookiePosition = {-1.864f, 0.0f, -1.8f};
    EventSetPosition position{.cleanup = 0, .character = SFLU_BASE, .parent = STREET_FIGHT_INVALID, .position = SFLU_STAGE_ORIGIN, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = SFLU_BASE, .parent = STREET_FIGHT_INVALID, .degrees = 0.0f, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFLU_BASE);
    AT_SetLocalBaseChara(SFLU_FUY, SFLU_BASE);
    AT_SetLocalBaseChara(fighter, SFLU_BASE);
    position = {.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFLU_BASE, .position = SFLU_DEBUG_PLAYER_POSITION, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFLU_BASE, .degrees = 225, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = SFLU_FUY, .parent = SFLU_BASE, .position = work->bookiePosition, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = SFLU_FUY, .parent = SFLU_BASE, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = fighter, .parent = SFLU_BASE, .position = {1.5f, 0, -1}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = fighter, .parent = SFLU_BASE, .degrees = 17, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    SetScenarioCharacterType(STREET_FIGHT_RYO, CharacterTypeStencilShadow);
    SetScenarioCharacterType(SFLU_FUY, CharacterTypeStencilShadow);
    SetScenarioCharacterType(fighter, CharacterTypeStencilShadow);
}
void CleanupDebug(StreetFightEventWork *work) {
    using enum CameraState;
    StreetFightStartParameters *const parameters = work->startParameters;
    ReleaseStreetFightCharacter(work->fighters[work->fighterIndex]);
    ET_AT_KillWork(work->characterReadTask);
    SetCharaKill(SFLU_BASE, 1);
    EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFLU_DEBUG_EXIT_POSITION);
    EV_AT_SetAngY(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFLU_DEBUG_EXIT_ROTATION);
    mtwkSetRegionFlag(scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, MotionRegion::LowerBody);
    if (work->hudEnabled)
        ToggleStreetFighterUI();
    GetNpcBatchCountOverride(work->previousNpcBatchCount);
    for (std::int32_t *motion = work->motions; *motion; ++motion)
        DeleteMotion(*motion);
    EV_FreeAuthorPak(0, "0260");
    if (!(parameters->flags & 1U))
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QKBT"), 0);
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
        EV_GetCharaPosition(SFLU_BASE, CHARACTER_TRANSLATION_ROOT, &stage, EventTransformFlags::Local);
        stage.y -= 0.5f;
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            audience->soundCentre = stage;
            audience->state = 2;
        }
        Vector3 savedPlayer, savedBookie, player, bookie;
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &savedPlayer, EventTransformFlags::Local);
        EV_GetCharaPosition(SFLU_FUY, CHARACTER_TRANSLATION_ROOT, &savedBookie, EventTransformFlags::Local);
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::AllAxes, work.playerPosition);
        EV_AT_SetPos(SFLU_FUY, EventTransformFlags::AllAxes, work.bookiePosition);
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::World);
        EV_GetCharaPosition(SFLU_FUY, CHARACTER_TRANSLATION_ROOT, &bookie, EventTransformFlags::World);
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
        EV_AT_SetPos(SFLU_FUY, EventTransformFlags::AllAxes, savedBookie);
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
            fighter, STREET_FIGHT_INVALID, {SFLU_DEBUG_OPPONENT_ORIGIN.x - 1.0f, SFLU_DEBUG_OPPONENT_ORIGIN.y + 0.5f, SFLU_DEBUG_OPPONENT_ORIGIN.z}, 0);
        work.state = 3;
        break;
    case 3:
        EV_NPC_NewLoadSw(1);
        if (!work.audienceTask || static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask))->state > 2) {
            QueueSfluStreetFightConversation(DebugIntroConversation, &work);
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
        ReadBookiePosition(SFLU_FUY);
        const std::uint32_t choice = ControlChoice(1);
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
        QueueSfluStreetFightConversation(DebugAcceptConversation, &work);
        work.state = 9;
        break;
    case 7:
        QueueSfluStreetFightConversation(DeclineStreetFightConversation, &work);
        work.state = 23;
        break;
    case 8:
        QueueSfluStreetFightConversation(InsufficientStreetFightFundsConversation, &work);
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
        AddStreetFightProgress(SFLU_DEBUG_WIN_INCREMENTS[work.completedOpponents]);
        QueueSfluStreetFightConversation(DebugVictoryConversation, &work);
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
        QueueSfluStreetFightConversation(DebugNextOpponentConversation, &work);
        work.state = 16;
        break;
    case 16:
        EVT_AT_KsIcon_Enqueue(-1, 38, 39, -1, 0x7fff, 0xffff, 0, 0);
        choicePhase = 0;
        work.state = 19;
        break;
    case 17:
        QueueSfluStreetFightConversation(DebugDefeatConversation, &work);
        work.state = 23;
        break;
    case 18:
        QueueSfluStreetFightConversation(DebugWithdrawalConversation, &work);
        work.state = 23;
        break;
    case 19: {
        ReadBookiePosition(SFLU_FUY);
        const std::uint32_t choice = ControlChoice(1);
        if (!choice)
            break;
        mtwkUpdateWithPosition(scenFindMotionwork(STREET_FIGHT_RYO), nullptr, 0);
        work.state = choice == 38 ? 20 : 21;
        break;
    }
    case 20:
        QueueSfluStreetFightConversation(DebugContinueConversation, &work);
        work.state = 22;
        break;
    case 21:
        QueueSfluStreetFightConversation(DebugStopConversation, &work);
        work.state = 23;
        break;
    case 22:
        if (work.completedOpponents < debugOpponentCount) {
            QueueSfluStreetFightConversation(DebugNextMatchConversation, &work);
            work.state = 9;
        } else {
            QueueSfluStreetFightConversation(DebugCompletedConversation, &work);
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
    EventSetPosition position{0, STREET_FIGHT_RYO, SFLU_BASE, {0, 0, -1.1f}, 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = STREET_FIGHT_RYO, .parent = SFLU_BASE, .degrees = 180, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = SFLU_FUY, .parent = SFLU_BASE, .position = SFLU_DEBUG_BATTLE_BOOKIE_POSITION, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = SFLU_FUY, .parent = SFLU_BASE, .degrees = 180, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = fighter, .parent = SFLU_BASE, .position = {0, 0, 1.1f}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = fighter, .parent = SFLU_BASE, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    HLib::SetCurrentTaskTo(DebugBattlePreEventCallback);
}
void DebugBattlePreEventCallback(void *) {
    EV_LoadAudio("0260A000", 1);
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
    EventSetCamera camera{.cleanup = 0, .camera = 2001, .eye = SFLU_BASE, .target = SFLU_BASE, .context = 0, .angle = 0, .delay = 0};
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
    vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("QKBT"), 1);
    shenInitFightData(ctrlGetScenarioChara(STREET_FIGHT_RYO), nullptr, 0, 0, 0, 0);
    vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("ENMY"), 1);
    shenInitFightData(ctrlGetScenarioChara(fighter), nullptr, 1, 1, definition->hitPoints, definition->fightStyle);
    vfUpdateFlagsFor(ctrlGetScenarioChara(fighter), 0, 0x08000000U, definition->difficulty);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{1}, 1);
    vfCopyPosFromEntity(0, nullptr, SFLU_DEBUG_OPPONENT_ORIGIN.x, SFLU_DEBUG_OPPONENT_ORIGIN.z, 2.5f);
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
    EV_LoadAudio("0260A000", 0);
    BeginSecondarySampleExecution(0);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFLU_BASE);
    AT_SetLocalBaseChara(fighter, SFLU_BASE);
    EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, false);
    EV_SetCharacterAlphaEnabled(SFLU_FUY, false);
    EV_SetCharacterAlphaEnabled(fighter, false);
    HLib::SetCurrentTaskTo(DebugMainCallback);
    work.frameCounter = 0;
}
std::int32_t DebugContinueConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFLU_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFLU_BASE, 180, 0);
    {
        EventSetMotion motion{.cleanup = 0, .character = STREET_FIGHT_RYO, .animation = 0xf002, .frame = 0, .speed = 1, .flags = 0, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(0xf002);
    };
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFLU_BASE, SFLU_DEBUG_CONVERSATION_PLAYER_POSITION, 60);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFLU_BASE, 17, 60);
    EVT_AT_SetPos(fighter, SFLU_BASE, SFLU_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
    if (work.completedOpponents < debugOpponentCount) {
        EVT_AT_SetPos(fighter, SFLU_BASE, SFLU_DEBUG_BATTLE_BOOKIE_POSITION, 60);
        EVT_AT_SetAngY(fighter, SFLU_BASE, 225, 60);
    }
    EVT_AT_SetPos(SFLU_FUY, SFLU_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFLU_FUY, SFLU_BASE, 0, 0);
    {
        EventSetMotion motion{.cleanup = 0, .character = SFLU_FUY, .animation = 0x1aed, .frame = 0, .speed = 1.1f, .flags = 0, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(0x1aed);
    };
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
    EVT_AT_SetCam(2006, SFLU_BASE, SFLU_BASE, 0, 0, 60);
    EVT_AT_SetCam(120, SFLU_BASE, SFLU_BASE, 0, 0, 150);
    return 210;
}
std::int32_t DebugCompletedConversation(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFLU_BASE, SFLU_DEBUG_CONVERSATION_PLAYER_POSITION, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFLU_BASE, 17, 0);
    {
        EventSetMotion motion{.cleanup = 0, .character = STREET_FIGHT_RYO, .animation = 0xf002, .frame = 0, .speed = 1, .flags = 0, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(0xf002);
    };
    EVT_AT_SetPos(SFLU_FUY, SFLU_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFLU_FUY, SFLU_BASE, 0, 0);
    {
        EventSetMotion motion{.cleanup = 0, .character = SFLU_FUY, .animation = 0x1aed, .frame = 0, .speed = 1.1f, .flags = 0, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(0x1aed);
    };
    EVT_AT_SetPos(fighter, SFLU_BASE, SFLU_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
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
    EVT_AT_SetCam(2007, SFLU_BASE, SFLU_BASE, 0, 0, 5);
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
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFLU_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFLU_BASE, 180, 0);
    {
        EventSetMotion motion{.cleanup = 0, .character = STREET_FIGHT_RYO, .animation = 0xf002, .frame = 0, .speed = 1, .flags = 0, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(0xf002);
    };
    EVT_AT_SetPos(SFLU_FUY, SFLU_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFLU_FUY, SFLU_BASE, 0, 0);
    {
        EventSetMotion motion{.cleanup = 0, .character = SFLU_FUY, .animation = 0x1aed, .frame = 0, .speed = 1.1f, .flags = 0, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(0x1aed);
    };
    EVT_AT_SetPos(fighter, SFLU_BASE, SFLU_DEBUG_HIDDEN_FIGHTER_POSITION, 0);
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
    EVT_AT_SetCam(2007, SFLU_BASE, SFLU_BASE, 0, 0, 5);
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
void DebugCharacterLoadCallback(void *parameter) {
    StreetFightCharacterLoadWork *const work = static_cast<StreetFightCharacterLoadWork *>(parameter);
    if (!work->state) {
        for (const shen_name *character = work->characters; character->name != STREET_FIGHT_INVALID.name; ++character)
            (void)EV_CheckWarmStartNpcRoute(*character);
        ++work->state;
        work->index = 0;
        return;
    }
    const shen_name character = work->characters[work->index];
    if (work->state == 1) {
        if (character.name == STREET_FIGHT_INVALID.name) {
            (void)HLib::ResolveDeferredCallback(work->parent);
            (void)HLib::DestroyTaskInternal(work->task);
        } else {
            EV_debug_log("D:\\Mango\\sm2\\src\\event\\common\\et_standerd_sf.c", 380, "[%c%c%c%c] ", character.name,
                static_cast<std::uint8_t>(character.name >> 8), static_cast<std::uint8_t>(character.name >> 16),
                static_cast<std::uint8_t>(character.name >> 24));
            if (DoesScenarioCharacterExist(character)) {
                EV_debug_log("D:\\Mango\\sm2\\src\\event\\common\\et_standerd_sf.c", 383, "Exist\n");
                work->state = 3;
            } else {
                EV_debug_log("D:\\Mango\\sm2\\src\\event\\common\\et_standerd_sf.c", 386, "Loading\n");
                work->state = 2;
            }
        }
        return;
    }
    if (work->state == 2) {
        const std::int32_t result = static_cast<std::int32_t>(EV_npcStreamOrLoadHuman(character));
        if (result == -4 || result == -3)
            (void)EV_ResetFreeTalkFlags();
        else if (result == 1)
            ++work->state;
        return;
    }
    if (work->state != 3)
        return;
    EV_debug_log("D:\\Mango\\sm2\\src\\event\\common\\et_standerd_sf.c", 416, "Completed.\n");
    (void)EV_SetRefBoardParamsFor(character, 1);
    EV_SetCharacterAlphaEnabled(character, false);
    EV_SetCharaControllerFlags(character, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, UpdateHeight, NpcCharacterControllerState::Disabled);
    AT_SetLocalBaseChara(character, STREET_FIGHT_INVALID);
    (void)figpSetFlagsBlendForBlur(character, 2, 1);
    mtwkSetEnabled(scenFindMotionwork(character), 1);
    (void)mtwkSetExtraFlag(scenFindMotionwork(character), 0, 0);
    (void)mtwkSetExtraFlag(scenFindMotionwork(character), 3, 2);
    (void)mtwkSetRequestedRootMode(scenFindMotionwork(character), 3);
    (void)mtwkSetFrameTime(scenFindMotionwork(character), 7, MotionRegion::LowerBody);
    (void)mtwkSetRegionFlag(scenFindMotionwork(character), 66, 2, MotionRegion::LowerBody);
    (void)mtwkSetRegionFlag(scenFindMotionwork(character), 69, 2, MotionRegion::LowerBody);
    (void)mtwkSetRegionFlag(scenFindMotionwork(character), 71, 2, MotionRegion::LowerBody);
    EV_EnableCharacterDisplay(character);
    EV_EnsureCharacterDraw(character);
    SetScenarioCharacterType(character, CharacterTypeStencilShadow);
    AT_SetLocalBaseChara(character, SFLU_BASE);
    ++work->index;
    work->state = 1;
}
std::int32_t DebugIntroConversation(void *parameter) {
    const StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    BeginSecondarySampleExecution(0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFLU_BASE, work.playerPosition, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFLU_BASE, work.playerPosition, 200);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFLU_BASE, 225, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFLU_BASE, 225, 200);
    {
        EventSetMotion motion{.cleanup = 0, .character = STREET_FIGHT_RYO, .animation = 0xf003, .frame = 0.0f, .speed = 1.0f, .flags = 0, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(0xf003);
    }
    EVT_AT_SetMot(STREET_FIGHT_RYO, 0xf002, 0, 1, 0, 199);
    EVT_AT_SetPos(SFLU_FUY, SFLU_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFLU_FUY, SFLU_BASE, 0, 0);
    {
        EventSetMotion motion{.cleanup = 0, .character = SFLU_FUY, .animation = 6893, .frame = 0.0f, .speed = 1.1f, .flags = 0, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(6893);
    }
    EVT_AT_SetPos(fighter, SFLU_BASE, SFLU_DEBUG_CONVERSATION_PLAYER_POSITION, 0);
    EVT_AT_SetAngY(fighter, SFLU_BASE, 17, 0);
    {
        EventSetMotion motion{.cleanup = 0, .character = fighter, .animation = 6875, .frame = 0.0f, .speed = 0.8f, .flags = 0x40000000U, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(6875);
    }
    EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
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
    EVT_AT_SetVoice("0100B038", 1, 50); // @text EN: "さあさあ、お立ち会い！" | JA: "さあさあ、お立ち会い！"
    EVT_AT_SetVoice("0100B067", 1, 132); // @text EN: "この強者に挑戦するヤツは\nいないか？" | JA: "この強者に挑戦するヤツは\nいないか？"
    EVT_AT_SetVoice("0100B068", 1, 240); // @text EN: "賭け金２０ドルの１本勝負だ！" | JA: "賭け金２０ドルの１本勝負だ！"
    EVT_AT_SetVoice("0100B069", 1, 330); // @text EN: "制限時間は３０秒。" | JA: "制限時間は３０秒。"
    EVT_AT_SetVoice("0100B070", 1, 412); // @text EN: "さあ、誰か勝負する\n腕自慢はいないか？" | JA: "さあ、誰か勝負する\n腕自慢はいないか？"
    {
        EventSetCamera camera{.cleanup = 0,
            .camera = g_currentMap.name == STREET_FIGHT_QSFB.name ? 8099 : 100,
            .eye = SFLU_BASE,
            .target = SFLU_BASE,
            .context = 0,
            .angle = 0,
            .delay = 0};
        EVT_AT_SetCam_Callback(&camera);
        camCtrlDisableFilmBars();
    }
    EVT_AT_SetCam(2004, SFLU_BASE, SFLU_BASE, 0, 0, 120);
    EVT_AT_SetCam(2006, SFLU_BASE, SFLU_BASE, 0, 0, 250);
    EVT_AT_SetCam(120, SFLU_BASE, SFLU_BASE, 0, 0, 350);
    return 420;
}
std::int32_t DebugAcceptConversation(void *parameter) {
    const StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    {
        EventSetMotion motion{.cleanup = 0, .character = STREET_FIGHT_RYO, .animation = 6895, .frame = 0.0f, .speed = 1.0f, .flags = 0, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(6895);
    }
    EVT_AT_SetVoice("0100A022", 1, 30); // @text EN: "I'll do it." | JA: "俺がやろう。"
    EVT_AT_SetVoice("0100B055", 1, 60); // @text EN: "よし。決まりだ。" | JA: "よし。決まりだ。"
    EVT_AT_SetPoint(SFLU_FUY, STREET_FIGHT_RYO, 5, 3, 0, 50, 156, 0);
    EVT_AT_SetPoint(fighter, STREET_FIGHT_RYO, 5, 1, 0, 70, 156, 0);
    {
        EventSetCamera camera{.cleanup = 0, .camera = 130, .eye = SFLU_BASE, .target = SFLU_BASE, .context = 0, .angle = 0, .delay = 0};
        EVT_AT_SetCam_Callback(&camera);
        camCtrlDisableFilmBars();
    }
    EVT_AT_SetCam(141, SFLU_BASE, SFLU_BASE, 0, 0, 28);
    EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
        .frames = 30,
        .startAlpha = 0,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 255,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 126});
    return 156;
}
std::int32_t DebugVictoryConversation(void *parameter) {
    const StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFLU_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFLU_BASE, 180, 0);
    {
        EventSetMotion motion{.cleanup = 0, .character = STREET_FIGHT_RYO, .animation = 0xf002, .frame = 0.0f, .speed = 1.0f, .flags = 0, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(0xf002);
    }
    EVT_AT_SetPos(SFLU_FUY, SFLU_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFLU_FUY, SFLU_BASE, 0, 0);
    {
        EventSetMotion motion{.cleanup = 0, .character = SFLU_FUY, .animation = 6893, .frame = 0.0f, .speed = 1.1f, .flags = 0, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(6893);
    }
    EVT_AT_SetPos(fighter, SFLU_BASE, SFLU_DEBUG_CONVERSATION_FIGHTER_POSITION, 0);
    EVT_AT_SetAngY(fighter, SFLU_BASE, 17, 0);
    {
        EventSetMotion motion{.cleanup = 0, .character = fighter, .animation = 6851, .frame = 0.0f, .speed = 1.0f, .flags = 0x50000000U, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(6851);
    }
    EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
        .frames = 30,
        .startAlpha = 255,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 0,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 5});
    HLib::Task *const praiseTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(praiseTask, 6);
    SkEventMessageWork *const praise = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(praiseTask));
    praise->message = STREET_FIGHT_DEBUG_VICTORY;
    praise->frames = 60;
    EVT_AT_SetCam(2007, SFLU_BASE, SFLU_BASE, 0, 0, 5);
    EVT_AT_SetCam(210, SFLU_BASE, SFLU_BASE, 0, 0, 118);
    return 120;
}
std::int32_t DebugNextOpponentConversation(void *parameter) {
    const StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFLU_BASE, work.playerPosition, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFLU_BASE, 180, 0);
    {
        EventSetMotion motion{.cleanup = 0, .character = STREET_FIGHT_RYO, .animation = 0xf002, .frame = 0.0f, .speed = 1.0f, .flags = 0, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(0xf002);
    }
    EVT_AT_SetPos(SFLU_FUY, SFLU_BASE, work.bookiePosition, 0);
    EVT_AT_SetAngY(SFLU_FUY, SFLU_BASE, 0, 0);
    {
        EventSetMotion motion{.cleanup = 0, .character = SFLU_FUY, .animation = 6893, .frame = 0.0f, .speed = 1.1f, .flags = 0, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(6893);
    }
    EVT_AT_SetPos(fighter, SFLU_BASE, SFLU_DEBUG_CONVERSATION_FIGHTER_POSITION, 0);
    EVT_AT_SetAngY(fighter, SFLU_BASE, 17, 0);
    {
        EventSetMotion motion{.cleanup = 0, .character = fighter, .animation = 6875, .frame = 0.0f, .speed = 0.8f, .flags = 0x50000000U, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(6875);
    }
    HLib::Task *const praiseTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(praiseTask, 1);
    SkEventMessageWork *const praise = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(praiseTask));
    praise->message = STREET_FIGHT_DEBUG_NEXT_OPPONENT;
    praise->frames = 59;
    HLib::Task *const offerTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(offerTask, 60);
    SkEventMessageWork *const offer = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(offerTask));
    offer->message = STREET_FIGHT_DEBUG_REMATCH;
    offer->frames = 60;
    if (work.completedOpponents >= 1 && work.completedOpponents <= 5) {
        HLib::Task *const prizeTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
        HLib::SetupTimedTask(prizeTask, 120);
        SkEventMessageWork *const prize = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(prizeTask));
        prize->message = SFLU_DEBUG_NEXT_PRIZE_MESSAGES[work.completedOpponents - 1];
        prize->frames = 60;
    }
    return 180;
}
std::int32_t DebugDefeatConversation(void *parameter) {
    const StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFLU_BASE, work.playerPosition, 1);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFLU_BASE, 180, 1);
    {
        EventSetMotion motion{.cleanup = 0, .character = STREET_FIGHT_RYO, .animation = 0xf002, .frame = 0.0f, .speed = 1.0f, .flags = 0, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(0xf002);
    }
    EVT_AT_SetPos(SFLU_FUY, SFLU_BASE, work.bookiePosition, 1);
    EVT_AT_SetAngY(SFLU_FUY, SFLU_BASE, 0, 1);
    {
        EventSetMotion motion{.cleanup = 0, .character = SFLU_FUY, .animation = 6893, .frame = 0.0f, .speed = 1.1f, .flags = 0, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(6893);
    }
    EVT_AT_SetPos(fighter, SFLU_BASE, SFLU_DEBUG_DEFEAT_FIGHTER_POSITION, 1);
    EVT_AT_SetAngY(fighter, SFLU_BASE, 17, 1);
    {
        EventSetMotion motion{.cleanup = 0, .character = fighter, .animation = 6898, .frame = 0.0f, .speed = 1.0f, .flags = 0x40000000U, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(6898);
    }
    mtwkSetLayerEndFrame(scenFindMotionwork(fighter), 1.0f, MotionRegion::LowerBody);
    EVT_AT_SetMot(fighter, 6898, 1, 1, 0x40000000U, 60);
    EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
        .frames = 30,
        .startAlpha = 255,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 0,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 5});
    EVT_AT_SetVoice("0100B077", 1, 35); // @text EN: "いやあ、惜しかったなあ、\n次はがんばんな。" | JA: "いやあ、惜しかったなあ、\n次はがんばんな。"
    {
        EventSetCamera camera{.cleanup = 0, .camera = 2008, .eye = SFLU_BASE, .target = SFLU_BASE, .context = 0, .angle = 0, .delay = 0};
        EVT_AT_SetCam_Callback(&camera);
        camCtrlDisableFilmBars();
    }
    EVT_AT_SetCam(310, SFLU_BASE, SFLU_BASE, 0, 0, 300);
    PATI_StartMoneyCount(static_cast<std::uint32_t>(-20), 180);
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
        .delay = 250});
    return 290;
}
std::int32_t DebugWithdrawalConversation(void *parameter) {
    const StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFLU_BASE, work.playerPosition, 1);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFLU_BASE, 180, 1);
    {
        EventSetMotion motion{.cleanup = 0, .character = STREET_FIGHT_RYO, .animation = 0xf002, .frame = 0.0f, .speed = 1.0f, .flags = 0, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(0xf002);
    }
    EVT_AT_SetPos(SFLU_FUY, SFLU_BASE, work.bookiePosition, 1);
    EVT_AT_SetAngY(SFLU_FUY, SFLU_BASE, 0, 1);
    {
        EventSetMotion motion{.cleanup = 0, .character = SFLU_FUY, .animation = 6893, .frame = 0.0f, .speed = 1.1f, .flags = 0, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(6893);
    }
    EVT_AT_SetPos(fighter, SFLU_BASE, SFLU_DEBUG_DEFEAT_FIGHTER_POSITION, 1);
    EVT_AT_SetAngY(fighter, SFLU_BASE, 17, 1);
    {
        EventSetMotion motion{.cleanup = 0, .character = fighter, .animation = 6898, .frame = 0.0f, .speed = 1.0f, .flags = 0x40000000U, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(6898);
    }
    EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
        .frames = 30,
        .startAlpha = 255,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 0,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 5});
    HLib::Task *const voiceTask = HLib::EnqueueTaskWithParameter(EVT_AT_SetVoice_Callback, 6, 0x0b, sizeof(EventSetVoice), STREET_FIGHT_EVENT_TASK);
    EventSetVoice *const voice = static_cast<EventSetVoice *>(HLib::GetTaskParameter(voiceTask));
    voice->cleanup = 1;
    voice->mode = 1;
    std::memcpy(voice->voice, "0100B078", 9);
    voice->delay = 35;
    EVT_AT_SetVoice("0100B079", 1, 140); // @text EN: "また今度挑戦してくれ。" | JA: "また今度挑戦してくれ。"
    {
        EventSetCamera camera{.cleanup = 0, .camera = 107, .eye = SFLU_BASE, .target = SFLU_BASE, .context = 0, .angle = 0, .delay = 0};
        EVT_AT_SetCam_Callback(&camera);
        camCtrlDisableFilmBars();
    }
    EVT_AT_SetCam(310, SFLU_BASE, SFLU_BASE, 0, 0, 300);
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
        .delay = 250});
    return 280;
}
std::int32_t DebugNextMatchConversation(void *parameter) {
    const StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(fighter, SFLU_BASE, SFLU_DEBUG_BATTLE_BOOKIE_POSITION, 0);
    EVT_AT_SetAngY(fighter, SFLU_BASE, 225, 0);
    HLib::Task *const challengeTask = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
    SkEventMessageWork *const challenge = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(challengeTask));
    challenge->message = STREET_FIGHT_DEBUG_OPPONENT_ACCEPT;
    challenge->frames = 60;
    EVT_AT_SetVoice("0100B071", 1, 60); // @text EN: "よし、決まりだ。" | JA: "よし、決まりだ。"
    {
        EventSetCamera camera{.cleanup = 0, .camera = 130, .eye = SFLU_BASE, .target = SFLU_BASE, .context = 0, .angle = 0, .delay = 0};
        EVT_AT_SetCam_Callback(&camera);
        camCtrlDisableFilmBars();
    }
    return 120;
}
Vector3 savedPlayerPosition{}, savedFighterPosition{};
Vector3i savedPlayerRotation{}, savedFighterRotation{};
std::uint8_t qtePhase{};
std::int32_t trainingLevel{}, trainingFrames{}, battleLevel{};
std::int32_t lostFrames{}, failedInput{-1}, cameraManualMode{};
float authoredFarClip{};
shen_name elevatorCharacters[9]{};
HLib::Task *elevatorPanelTask{};
struct AuthorWork {
    HLib::Task *task;
    std::uint32_t reserved08;
    shen_name command;
    std::uint16_t state, reserved12;
    std::uint32_t reserved14;
};
struct AuthorLoopWork {
    AuthorWork *owner;
    std::int16_t frame;
    std::byte reserved0a[6];
};
struct ElevatorPanelWork {
    std::int32_t start, end, next, tens, delay;
};
static_assert(sizeof(AuthorWork) == 24);
static_assert(offsetof(AuthorWork, state) == 16);
static_assert(sizeof(AuthorLoopWork) == 16);
static_assert(sizeof(ElevatorPanelWork) == 20);
void MainCallback(void *);
void CleanupNormalCallback(void *);
void RetryBeginCallback(void *);
void RetryChoiceCallback(void *);
void AuthorCallback(void *);
void AuthorLoopCallback(void *);
void ElevatorPanelCallback(void *);
void LostExpressionCallback(void *);
std::int32_t DefeatConversation(void *);
void ToggleTrainingAudience(std::int32_t enabled) {
    const shen_name characters[]{SFLU_JOP, SFLU_KF1, SFLU_KF2, SFLU_KF3, SFLU_KR1, SFLU_KR2, SFLU_SF9, SFLU_BF6, SFLU_GOI};
    for (const shen_name character : characters) {
        if (enabled) {
            EV_EnableCharacterDisplay(character);
            EV_EnsureCharacterDraw(character);
        } else {
            EV_DisableCharacterDisplay(character);
            EV_ToggleCharacterFlag(character);
        }
    }
}
void ToggleFence(std::int32_t enabled) {
    const shen_name fence[]{SFLU_AMI1, SFLU_AMI2, SFLU_AMI3, SFLU_AMI4, SFLU_AMI5};
    if (enabled) {
        EV_SetObjectFlagByIndex(1, 0);
        EV_SetObjectFlagByIndex(2, 0);
        EV_DisableCharacterDisplay(SFLU_A734);
        EV_SetCharacterAlphaEnabled(SFLU_A734, 1);
        for (const shen_name character : fence) {
            ApplyCharacterGeometryLighting(ctrlGetScenarioChara(character), 1);
            EV_EnableCharacterDisplay(character);
            EV_SetCharacterAlphaEnabled(character, 1);
            SetScenarioCharacterRadius(character, 10);
        }
    } else {
        for (const shen_name character : fence)
            EV_DisableCharacterDisplay(character);
        EV_SetObjectFlagByIndex(1, 1);
        EV_SetObjectFlagByIndex(2, 1);
        EV_EnableCharacterDisplay(SFLU_A734);
        EV_SetCharacterAlphaEnabled(SFLU_A734, 0);
    }
}
void UpdateFenceAlpha() {
    Vector3 camera;
    camCtrlGetNextCamPos(&camera);
    const shen_name fence[]{SFLU_AMI1, SFLU_AMI2, SFLU_AMI3, SFLU_AMI4, SFLU_AMI5};
    for (const shen_name character : fence) {
        Vector3 position;
        EV_GetCharaPosition(character, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        const float distance = std::fabs(character.name == SFLU_AMI1.name || character.name == SFLU_AMI4.name ? position.x - camera.x :
                character.name == SFLU_AMI2.name || character.name == SFLU_AMI3.name                          ? position.z - camera.z :
                                                                                                                position.y - camera.y);
        EV_SetCharacterAlphaEnabled(character, 1);
        SetScenarioCharacterTransparency(character, (std::fmin(3.2f, std::fmax(.2f, distance)) - .2f) / 3.0f);
    }
}
void PlaceNormal() {
    EV_EnableCharacterDisplay(SFLU_SF4);
    EV_EnsureCharacterDraw(SFLU_SF4);
    EVT_AT_SetPos(SFLU_BASE, STREET_FIGHT_INVALID, SFLU_STAGE_ORIGIN, 0);
    EVT_AT_SetAngY(SFLU_BASE, STREET_FIGHT_INVALID, 0, 0);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFLU_BASE);
    AT_SetLocalBaseChara(SFLU_SF4, SFLU_BASE);
    {
        EventSetMotion motion{.cleanup = 0, .character = STREET_FIGHT_RYO, .animation = 306, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(306);
    };
    EVT_AT_SetPos(STREET_FIGHT_RYO, SFLU_BASE, {0, 0, -1.1f}, 0);
    EVT_AT_SetAngY(STREET_FIGHT_RYO, SFLU_BASE, 180, 0);
    {
        EventSetMotion motion{.cleanup = 0, .character = SFLU_SF4, .animation = 2732, .frame = 0, .speed = -1, .flags = 0x40000000U, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(2732);
    };
    EVT_AT_SetPos(SFLU_SF4, SFLU_BASE, {0, 0, 1.1f}, 0);
    EVT_AT_SetAngY(SFLU_SF4, SFLU_BASE, 0, 0);
    EVT_AT_SetAngY(SFLU_SF4, SFLU_BASE, 0, 0);
}
void ResetQteCharacters(StreetFightEventWork &work) {
    const shen_name fighter = work.fighters[work.fighterIndex];
    EVT_AT_SetPos(fighter, STREET_FIGHT_INVALID, SFLU_STAGE_ORIGIN, 0);
    EVT_AT_SetPos(STREET_FIGHT_RYO, STREET_FIGHT_INVALID, SFLU_STAGE_ORIGIN, 0);
    mtwkSetRegionFlag(scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, MotionRegion::LowerBody);
    mtwkSetRegionFlag(scenFindMotionwork(fighter), 71, 0, MotionRegion::LowerBody);
}
void UpdateBattleQte(StreetFightEventWork &work) {
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        return;
    switch (qtePhase) {
    case 0:
        if ((GetFightCharacterFlag(1, ctrlGetScenarioChara(STREET_FIGHT_RYO), 0) < 15 &&
                GetFightCharacterFlag(1, ctrlGetScenarioChara(SFLU_SF4), 0) < 50) ||
            GetFightCharacterFlag(1, ctrlGetScenarioChara(SFLU_SF4), 0) < 15) {
            if (!(GetFightTaskFlags() & 0x10) && (EV_vfBattleUtil(FightBattleUtilOp::ActorField, STREET_FIGHT_RYO, 13) & 1)) {
                std::int32_t first, second, third;
                if (work.state == 11) {
                    qteBattleUtil(BattleQteMode::SetWait, 25, 6, SFLU_QTE_FIRST_WAIT[battleLevel], 0);
                    first = 64;
                    second = 1024;
                    third = 0;
                } else {
                    qteBattleUtil(BattleQteMode::SetWait, 25, 6, SFLU_QTE_SECOND_WAIT[battleLevel], 0);
                    first = EV_vfBattleUtil(FightBattleUtilOp::SkillMask, shen_name{0x40000004U}, -1) ? 4 : 64;
                    second = first;
                    third = 512;
                }
                qteBattleUtil(BattleQteMode::SetFirstCommands, first, second, third, 0);
                qteBattleUtil(BattleQteMode::ResetStates);
                qteBattleUtil(BattleQteMode::Queue);
                EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{0x1000}, 1);
                qtePhase = 1;
            }
        }
        break;
    case 1:
        vfUpdateFlagsFor(ctrlGetScenarioChara(SFLU_SF4), 1, 0x20000, 0);
        figpSetFlagModesFor(STREET_FIGHT_RYO, 0, 1);
        if (work.state == 18)
            figpSetFlagModesFor(SFLU_SF4, 0, 1);
        figpAssignModeFor(STREET_FIGHT_RYO, 1);
        figpSetFlagsBlendForBlur(STREET_FIGHT_RYO, 10, 2);
        figpAssignModeFor(SFLU_SF4, 1);
        figpSetFlagsBlendForBlur(SFLU_SF4, 10, 2);
        qteBattleUtil(BattleQteMode::Start);
        qtePhase = 2;
        break;
    case 2: {
        const std::int64_t result = qteBattleUtil(BattleQteMode::GetResult);
        if (!result)
            break;
        ResetQteCharacters(work);
        if (result == 1) {
            if (work.state == 11) {
                vfmDispatchControl(FightControlOp::QueuedAction, STREET_FIGHT_RYO, 0x8000ad6, 1);
                qtePhase = 4;
            } else if (EV_vfBattleUtil(FightBattleUtilOp::SkillMask, shen_name{0x40000004U}, -1)) {
                QueueExtraTelop(SFLU_MOVE_TELOP, 99);
                vfmDispatchControl(FightControlOp::QueuedAction, STREET_FIGHT_RYO, 0x8000ad7, 1);
                qtePhase = 3;
            } else {
                vfmDispatchControl(FightControlOp::QueuedAction, STREET_FIGHT_RYO, 0x8000ad9, 1);
                qtePhase = 4;
            }
        } else {
            {
                EventSetCamera camera{.cleanup = 0, .camera = 1421, .eye = STREET_FIGHT_RYO, .target = STREET_FIGHT_RYO, .context = 0, .angle = 0, .delay = 0};
                EVT_AT_SetCam_Callback(&camera);
            };
            camCtrlDisableFilmBars();
            vfmDispatchControl(FightControlOp::QueuedAction, SFLU_SF4, 0x8000ada, 1);
            qtePhase = 5;
        }
        break;
    }
    case 3:
        if (work.state != 11 && EV_vfBattleUtil(FightBattleUtilOp::SkillMask, shen_name{0x40000004U}, -1) && ++trainingFrames == 292) {
            EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{4}, 1);
            QueueExtraTelop("CCCCCC", 0x40000000);
            trainingFrames = 0;
            qtePhase = 6;
        }
        break;
    case 4:
        if (work.state != 11 && EV_vfBattleUtil(FightBattleUtilOp::SkillMask, shen_name{0x40000004U}, -1))
            return;
        if (mtwkGetFrame(scenFindMotionwork(SFLU_SF4), MotionRegion::LowerBody) >= (work.state == 11 ? 31.0f : 33.0f)) {
            EV_vfBattleUtil(FightBattleUtilOp::ForceActorDefeat, SFLU_SF4, 1);
            qtePhase = 6;
        }
        break;
    case 5:
        if (mtwkGetFrame(scenFindMotionwork(STREET_FIGHT_RYO), MotionRegion::LowerBody) >= 51)
            qtePhase = 6;
        break;
    case 6:
        qteBattleUtil(BattleQteMode::RetireAndFree);
        ++qtePhase;
        break;
    default:
        break;
    }
}
void InitializeElevatorCharacters() {
    Vector3 position{249.67999267578125f, 83.2300033569336f, 123.54000091552734f};
    const Vector3i rotation{0, 0x4000, 0};
    Vector3 hidden{0, 1000, 0}, shown{};
    for (std::uint32_t index = 0; index < 9; ++index) {
        elevatorCharacters[index] = CreateEventCharacter(CharacterCreationHighModel, reinterpret_cast<const std::byte *>("ELVTTBBG"), {});
        EV_EnableCharacterDisplay(elevatorCharacters[index]);
        position.y += .09000000357627869f;
        if (index == 4) {
            position.y = 83.2300033569336f;
            position.z -= .09000000357627869f;
        }
        EV_AT_SetPos(elevatorCharacters[index], EventTransformFlags::World | EventTransformFlags::AllAxes, position);
        EV_AT_SetAngY(elevatorCharacters[index], EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
        for (const std::int32_t *segment = SFLU_ELEVATOR_SEGMENTS; *segment != -1; ++segment) {
            EV_scenApplyPosOrRotToPartOf(
                elevatorCharacters[index], static_cast<std::uint16_t>(*segment), &hidden, nullptr, EventCharacterPartTransformMode::Absolute);
            EV_scenApplyPosOrRotToPartOf(elevatorCharacters[index], 162, &shown, nullptr, EventCharacterPartTransformMode::Absolute);
        }
    }
    for (const std::int32_t *segment = SFLU_ELEVATOR_SEGMENTS; *segment != -1; ++segment)
        EV_scenApplyPosOrRotToPartOf(SFLU_A800, static_cast<std::uint16_t>(*segment), &hidden, nullptr, EventCharacterPartTransformMode::Absolute);
    for (const std::int32_t *segment = SFLU_ELEVATOR_PANEL_SEGMENTS; *segment != -1; ++segment)
        EV_scenApplyPosOrRotToPartOf(SFLU_A798, static_cast<std::uint16_t>(*segment), &hidden, nullptr, EventCharacterPartTransformMode::Absolute);
    const std::uint16_t shownSegments[]{162, 231, 232, 233, 196, 242};
    for (const std::uint16_t segment : shownSegments)
        EV_scenApplyPosOrRotToPartOf(elevatorCharacters[8], segment, &hidden, nullptr, EventCharacterPartTransformMode::Absolute);
    const std::uint16_t buildingSegments[]{207, 231, 247};
    for (const std::uint16_t segment : buildingSegments)
        EV_scenApplyPosOrRotToPartOf(SFLU_A800, segment, &hidden, nullptr, EventCharacterPartTransformMode::Absolute);
}
void QueueElevatorPanel(std::int32_t start, std::int32_t end) {
    Vector3 hidden{0, 1000, 0}, shown{};
    for (const std::int32_t *segment = SFLU_ELEVATOR_PANEL_SEGMENTS; *segment != -1; ++segment)
        EV_scenApplyPosOrRotToPartOf(SFLU_A798, static_cast<std::uint16_t>(*segment), &hidden, nullptr, EventCharacterPartTransformMode::Absolute);
    EV_scenApplyPosOrRotToPartOf(SFLU_A798, static_cast<std::uint16_t>(std::abs(start) % 10 + 152), &shown, nullptr, EventCharacterPartTransformMode::Absolute);
    EV_scenApplyPosOrRotToPartOf(
        SFLU_A798, static_cast<std::uint16_t>(SFLU_ELEVATOR_TENS[start / 10]), &shown, nullptr, EventCharacterPartTransformMode::Absolute);
    elevatorPanelTask = HLib::EnqueueTaskWithParameter(ElevatorPanelCallback, 4, 0x0b, sizeof(ElevatorPanelWork), STREET_FIGHT_EVENT_TASK);
    ElevatorPanelWork *const work = static_cast<ElevatorPanelWork *>(HLib::GetTaskParameter(elevatorPanelTask));
    work->start = start;
    work->end = end;
    work->next = start + 1;
    work->tens = start / 10;
    work->delay = 35;
}
void ElevatorPanelCallback(void *parameter) {
    ElevatorPanelWork &work = *static_cast<ElevatorPanelWork *>(parameter);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        HLib::DestroyCurrentTask();
    if (work.delay-- != 1)
        return;
    Vector3 hidden{0, 1000, 0}, shown{};
    for (const std::int32_t *segment = SFLU_ELEVATOR_PANEL_SEGMENTS; *segment != -1; ++segment)
        EV_scenApplyPosOrRotToPartOf(SFLU_A798, static_cast<std::uint16_t>(*segment), &hidden, nullptr, EventCharacterPartTransformMode::Absolute);
    if (work.next >= 0)
        EV_scenApplyPosOrRotToPartOf(
            SFLU_A798, static_cast<std::uint16_t>(SFLU_ELEVATOR_TENS[work.next / 10]), &shown, nullptr, EventCharacterPartTransformMode::Absolute);
    else
        EV_scenApplyPosOrRotToPartOf(SFLU_A798, 207, &shown, nullptr, EventCharacterPartTransformMode::Absolute);
    const std::int32_t digit = std::abs(work.next) % 10;
    const std::uint16_t segment = (work.next < 0 || work.next / 10 == 0) && !digit ? 153 : static_cast<std::uint16_t>(digit + 152);
    EV_scenApplyPosOrRotToPartOf(SFLU_A798, segment, &shown, nullptr, EventCharacterPartTransformMode::Absolute);
    work.delay = 35;
    if (work.next == work.end)
        HLib::DestroyCurrentTask();
    else
        ++work.next;
}
void LoadAuthorCharacters(shen_name command) {
    EV_EnsureCharacterDraw(STREET_FIGHT_RYO);
    EV_EnableCharacterDisplay(STREET_FIGHT_RYO);
    EV_SetCharaControllerFlags(STREET_FIGHT_RYO, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(STREET_FIGHT_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    mtwkSetExtraFlag(scenFindMotionwork(STREET_FIGHT_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(STREET_FIGHT_RYO, 0);
    AT_SetLocalBaseChara(STREET_FIGHT_RYO, STREET_FIGHT_INVALID);
    std::uint32_t table = 0;
    if (command.name == SFLU_AUTH_1.name)
        table = 1;
    else if (command.name == SFLU_AUTH_2.name)
        table = 2;
    else if (command.name == SFLU_AUTH_3.name)
        table = 3;
    else if (command.name == SFLU_AUTH_4.name)
        table = 4;
    else if (command.name == SFLU_AUTH_5.name)
        table = 5;
    else if (command.name == SFLU_AUTH_6.name)
        table = 6;
    const shen_name *const characters = SFLU_AUTHOR_CHARACTERS[table];
    for (std::uint32_t index = 0; characters[index].name != STREET_FIGHT_INVALID.name; ++index) {
        const shen_name character = characters[index];
        EV_EnsureCharacterDraw(character);
        EV_EnableCharacterDisplay(character);
        EV_SetCharaControllerFlags(character, UpdateHeight, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
        mtwkSetExtraFlag(scenFindMotionwork(character), 6, 2);
        EV_SetCharacterAlphaEnabled(character, 0);
        AT_SetLocalBaseChara(character, STREET_FIGHT_INVALID);
    }
    const shen_name charactersElevator[]{SFLU_A800, SFLU_A798}, charactersOutdoor[]{SFLU_A516, SFLU_A512, SFLU_A734};
    const shen_name *const models = command.name == SFLU_AUTH_4.name ? charactersElevator : charactersOutdoor;
    const std::uint32_t count = command.name == SFLU_AUTH_4.name ? 2 : 3;
    for (std::uint32_t index = 0; index < count; ++index) {
        const shen_name model = models[index];
        EV_EnableCharacterDisplay(model);
        EV_SetCharaControllerFlags(model, UpdateHeight, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(model, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharacterAlphaEnabled(model, 0);
    }
}
void UnloadAuthorCharacters(shen_name command) {
    EV_EnableCharacterDisplay(STREET_FIGHT_RYO);
    mtwkSetExtraFlag(scenFindMotionwork(STREET_FIGHT_RYO), 6, 0);
    std::uint32_t table = 0;
    if (command.name == SFLU_AUTH_1.name)
        table = 1;
    else if (command.name == SFLU_AUTH_2.name)
        table = 2;
    else if (command.name == SFLU_AUTH_3.name)
        table = 3;
    else if (command.name == SFLU_AUTH_4.name)
        table = 4;
    else if (command.name == SFLU_AUTH_5.name)
        table = 5;
    else if (command.name == SFLU_AUTH_6.name)
        table = 6;
    const shen_name *const characters = SFLU_AUTHOR_CHARACTERS[table];
    for (std::uint32_t index = 0; characters[index].name != STREET_FIGHT_INVALID.name; ++index) {
        EV_EnableCharacterDisplay(characters[index]);
        mtwkSetExtraFlag(scenFindMotionwork(characters[index]), 6, 0);
    }
    if (command.name == SFLU_AUTH_4.name) {
        EV_DisableCharacterDisplay(SFLU_A800);
        EV_DisableCharacterDisplay(SFLU_A798);
    } else {
        EV_DisableCharacterDisplay(SFLU_A516);
        EV_DisableCharacterDisplay(SFLU_A512);
        EV_DisableCharacterDisplay(SFLU_A734);
    }
}
void QueueAuthorLoop(AuthorWork *owner) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AuthorLoopCallback, 4, 0x0b, sizeof(AuthorLoopWork), STREET_FIGHT_EVENT_TASK);
    AuthorLoopWork *const loop = static_cast<AuthorLoopWork *>(HLib::GetTaskParameter(task));
    loop->frame = 0;
    loop->owner = owner;
    EV_authSceneCtrl(
        owner->command.name == SFLU_AUTH_5.name || owner->command.name == SFLU_AUTH_6.name ? AuthSceneCtrl::HideFilmBars : AuthSceneCtrl::ShowFilmBars);
    const char *const author = owner->command.name == SFLU_AUTH_4.name ? "0260A090" :
        owner->command.name == SFLU_AUTH_5.name                        ? "0260A030" :
        owner->command.name == SFLU_AUTH_6.name                        ? "0260A050" :
                                                                         "0260A000";
    EV_LoadCutsceneFromCurrentScene(author);
    if (owner->command.name == SFLU_AUTH_4.name) {
        EV_lightLoad(SHEN_NAME(0000));
        PushGlobalLightingProgram();
        EV_lightLoad(SHEN_NAME(l901));
    } else {
        if (owner->command.name == SFLU_AUTH_5.name || owner->command.name == SFLU_AUTH_6.name)
            EV_lightLoad(SHEN_NAME(0000));
        PushGlobalLightingProgram();
    }
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_spmBeginSampleExecute();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(owner->command.name));
    owner->state = 2;
}
void AuthorCallback(void *parameter) {
    AuthorWork &work = *static_cast<AuthorWork *>(parameter);
    if (work.state == 1)
        QueueAuthorLoop(&work);
    else if (work.state == 3)
        HLib::DestroyTaskInternal(work.task);
}
void QueueAuthor(shen_name command) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AuthorCallback, 4, 0x0b, sizeof(AuthorWork), STREET_FIGHT_EVENT_TASK);
    AuthorWork *const work = static_cast<AuthorWork *>(HLib::GetTaskParameter(task));
    work->state = 1;
    work->task = task;
    work->command = command;
    SetPlayerControlEnabled(0);
    EV_SetWeatherInstruction(12, 0);
    if (command.name == SFLU_AUTH_4.name)
        InitializeElevatorCharacters();
}
void AuthorLoopCallback(void *parameter) {
    AuthorLoopWork &loop = *static_cast<AuthorLoopWork *>(parameter);
    loop.frame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    const shen_name command = loop.owner->command;
    const std::int32_t frame = loop.frame;
    if (!frame)
        LoadAuthorCharacters(command);
    if (command.name == SFLU_AUTH_4.name) {
        if (frame == 60) {
            Vector3 hidden{0, 1000, 0}, shown{};
            EV_scenApplyPosOrRotToPartOf(SFLU_A798, 152, &hidden, nullptr, EventCharacterPartTransformMode::Absolute);
            EV_scenApplyPosOrRotToPartOf(SFLU_A798, 153, &shown, nullptr, EventCharacterPartTransformMode::Absolute);
        } else if (frame == 90)
            QueueElevatorPanel(1, 3);
        else if (frame == 310)
            QueueElevatorPanel(2, 12);
        else if (frame == 1575)
            QueueElevatorPanel(32, 36);
    } else if ((command.name == SFLU_AUTH_5.name || command.name == SFLU_AUTH_6.name) && frame == EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining) - 15)
        SetScreenFilmFade(15, 0, 0, 0, 0, 255, 0, 0, 0);
    if (command.name == SFLU_AUTH_0.name) {
        switch (frame) {
        case 1180:
            EV_DisableCharacterDisplay(SFLU_KF1);
            EV_ToggleCharacterFlag(SFLU_KF1);
            EV_DisableCharacterDisplay(SFLU_KF2);
            EV_ToggleCharacterFlag(SFLU_KF2);
            EV_DisableCharacterDisplay(SFLU_KF3);
            EV_ToggleCharacterFlag(SFLU_KF3);
            EV_DisableCharacterDisplay(SFLU_KR1);
            EV_ToggleCharacterFlag(SFLU_KR1);
            EV_DisableCharacterDisplay(SFLU_KR2);
            EV_ToggleCharacterFlag(SFLU_KR2);
            SetHighModelXYZ(ctrlGetScenarioChara(STREET_FIGHT_RYO), 0, 1, 0);
            SetHighModelXYZ(ctrlGetScenarioChara(STREET_FIGHT_RYO), 0, 0, 0);
            SetFaceFlagsForChara_0(ctrlGetScenarioChara(STREET_FIGHT_RYO), FacialNodeOriginal);
            SetFaceFlagsForChara_0(ctrlGetScenarioChara(SFLU_SF4), FacialNodeOriginal);
            EV_SetObjectFlagByIndex(1, 0);
            break;
        case 1346:
            SetFaceFlagsForChara_0(ctrlGetScenarioChara(SFLU_SF4), FacialNodeReplacement);
            EV_SetObjectFlagByIndex(1, 1);
            break;
        case 1580:
            EV_EnableCharacterDisplay(SFLU_KF1);
            EV_EnsureCharacterDraw(SFLU_KF1);
            EV_EnableCharacterDisplay(SFLU_KF2);
            EV_EnsureCharacterDraw(SFLU_KF2);
            EV_EnsureCharacterDraw(SFLU_KF3);
            EV_EnsureCharacterDraw(SFLU_KR1);
            EV_EnsureCharacterDraw(SFLU_KR2);
            SetFaceFlagsForChara_0(ctrlGetScenarioChara(STREET_FIGHT_RYO), FacialNodeReplacement);
            break;
        default:
            break;
        }
    } else if (command.name == SFLU_AUTH_1.name) {
        switch (frame) {
        case 311:
            SetFaceFlagsForChara_0(ctrlGetScenarioChara(SFLU_JOP), FacialNodeOriginal);
            break;
        case 841:
            SetHighModelXYZ(ctrlGetScenarioChara(STREET_FIGHT_RYO), 0, 1, 0);
            SetHighModelXYZ(ctrlGetScenarioChara(STREET_FIGHT_RYO), 0, 0, 0);
            SetHighModelXYZ(ctrlGetScenarioChara(SFLU_SF4), 0, 1, 0);
            SetHighModelXYZ(ctrlGetScenarioChara(SFLU_SF4), 0, 0, 0);
            SetFaceFlagsForChara_0(ctrlGetScenarioChara(STREET_FIGHT_RYO), FacialNodeOriginal);
            SetFaceFlagsForChara_0(ctrlGetScenarioChara(SFLU_SF4), FacialNodeOriginal);
            EV_SetObjectFlagByIndex(1, 0);
            break;
        case 1066:
            ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(SFLU_SF4), 1, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
            ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(SFLU_SF4), 0, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
            SetFaceFlagsForChara_0(ctrlGetScenarioChara(STREET_FIGHT_RYO), FacialNodeReplacement);
            SetFaceFlagsForChara_0(ctrlGetScenarioChara(SFLU_SF4), FacialNodeReplacement);
            SetFaceFlagsForChara_0(ctrlGetScenarioChara(SFLU_JOP), FacialNodeReplacement);
            EV_SetObjectFlagByIndex(1, 1);
            break;
        default:
            break;
        }
    } else if (command.name == SFLU_AUTH_2.name) {
        switch (frame) {
        case 0:
            EV_SetObjectFlagByIndex(1, 0);
            EV_DisableCharacterDisplay(SFLU_JOP);
            EV_ToggleCharacterFlag(SFLU_JOP);
            SetScenarioCharacterDrawControl({}, 2, 0);
            SetScenarioCharacterDrawControl(STREET_FIGHT_RYO, 2, 0);
            SetScenarioCharacterDrawControl(SFLU_SF4, 2, 0);
            SetScenarioCharacterDrawControl(SFLU_KF1, 2, 0);
            SetScenarioCharacterDrawControl(SFLU_KF2, 2, 0);
            SetScenarioCharacterDrawControl(SFLU_KR2, 2, 0);
            SetScenarioCharacterDrawControl(SFLU_BF6, 2, 0);
            SetScenarioCharacterDrawControl(SFLU_JOP, 2, 0);
            break;
        case 421:
            SetHighModelXYZ(ctrlGetScenarioChara(STREET_FIGHT_RYO), 0, 0, 0);
            SetHighModelXYZ(ctrlGetScenarioChara(STREET_FIGHT_RYO), 0, 1, 0);
            SetFaceFlagsForChara_0(ctrlGetScenarioChara(STREET_FIGHT_RYO), FacialNodeOriginal);
            SetFaceFlagsForChara_0(ctrlGetScenarioChara(SFLU_SF4), FacialNodeOriginal);
            break;
        case 422:
            EV_ToggleCharacterFlag(SFLU_KF1);
            EV_ToggleCharacterFlag(SFLU_KF2);
            EV_ToggleCharacterFlag(SFLU_KR2);
            break;
        case 521:
            SetHighModelXYZ(ctrlGetScenarioChara(STREET_FIGHT_RYO), 0, 0, 0);
            SetHighModelXYZ(ctrlGetScenarioChara(STREET_FIGHT_RYO), 0, 1, 0);
            SetFaceFlagsForChara_0(ctrlGetScenarioChara(STREET_FIGHT_RYO), FacialNodeReplacement);
            break;
        case 661:
            SetHighModelXYZ(ctrlGetScenarioChara(SFLU_SF4), 0, 1, 0);
            SetFaceFlagsForChara_0(ctrlGetScenarioChara(STREET_FIGHT_RYO), FacialNodeOriginal);
            SetFaceFlagsForChara_0(ctrlGetScenarioChara(SFLU_SF4), FacialNodeReplacement);
            EV_EnsureCharacterDraw(SFLU_KF1);
            EV_EnsureCharacterDraw(SFLU_KF2);
            EV_EnsureCharacterDraw(SFLU_KR2);
            break;
        case 891:
            SetHighModelXYZ(ctrlGetScenarioChara(SFLU_SF4), 0, 0, 0);
            SetFaceFlagsForChara_0(ctrlGetScenarioChara(SFLU_SF4), FacialNodeOriginal);
            SetFaceFlagsForChara_0(ctrlGetScenarioChara(STREET_FIGHT_RYO), FacialNodeOriginal);
            break;
        case 892:
            EV_ToggleCharacterFlag(SFLU_KF1);
            EV_ToggleCharacterFlag(SFLU_KF2);
            break;
        case 1029:
            EV_EnsureCharacterDraw(SFLU_JOP);
            EV_SetObjectFlagByIndex(1, 1);
            break;
        default:
            break;
        }
    } else if (command.name == SFLU_AUTH_3.name) {
        switch (frame) {
        case 0:
            EV_SetObjectFlagByIndex(1, 0);
            SetScenarioCharacterDrawControl({}, 2, 0);
            SetScenarioCharacterDrawControl(STREET_FIGHT_RYO, 2, 0);
            SetScenarioCharacterDrawControl(SFLU_SF4, 2, 0);
            SetScenarioCharacterDrawControl(SFLU_KF1, 2, 0);
            SetScenarioCharacterDrawControl(SFLU_KF2, 2, 0);
            SetScenarioCharacterDrawControl(SFLU_KR2, 2, 0);
            SetScenarioCharacterDrawControl(SFLU_BF6, 2, 0);
            SetScenarioCharacterDrawControl(SFLU_JOP, 2, 0);
            break;
        case 476:
            camCtrlSetFarDistance(13);
            authoredFarClip = 13;
            SetHighModelXYZ(ctrlGetScenarioChara(SFLU_JOP), 0, 0, 0);
            SetHighModelXYZ(ctrlGetScenarioChara(SFLU_JOP), 0, 1, 0);
            EV_DisableCharacterDisplay(SFLU_SF4);
            EV_DisableCharacterDisplay(SFLU_KF2);
            EV_DisableCharacterDisplay(SFLU_BF6);
            EV_ToggleCharacterFlag(SFLU_SF4);
            break;
        case 720:
            SetClothFlag(ctrlGetScenarioChara(SFLU_JOP), 4, 1);
            break;
        case 760:
            EV_EnableCharacterDisplay(SFLU_BF6);
            break;
        case 792:
            SetHighModelXYZ(ctrlGetScenarioChara(STREET_FIGHT_RYO), 0, 0, 0);
            SetHighModelXYZ(ctrlGetScenarioChara(STREET_FIGHT_RYO), 0, 1, 0);
            break;
        case 810:
            EV_EnableCharacterDisplay(SFLU_KF2);
            break;
        case 855:
            EV_DisableCharacterDisplay(SFLU_KR2);
            break;
        case 880:
            SetClothFlag(ctrlGetScenarioChara(SFLU_JOP), 4, 0);
            camCtrlSetFarDistance(authoredFarClip);
            SetHighModelXYZ(ctrlGetScenarioChara(STREET_FIGHT_RYO), 0, 0, 0);
            SetHighModelXYZ(ctrlGetScenarioChara(STREET_FIGHT_RYO), 0, 1, 0);
            SetFaceFlagsForChara_0(ctrlGetScenarioChara(SFLU_JOP), FacialNodeReplacement);
            EV_EnableCharacterDisplay(SFLU_KF1);
            EV_EnableCharacterDisplay(SFLU_KF2);
            EV_EnableCharacterDisplay(SFLU_KR2);
            EV_EnableCharacterDisplay(SFLU_BF6);
            break;
        case 1146:
            SetFaceFlagsForChara_0(ctrlGetScenarioChara(STREET_FIGHT_RYO), FacialNodeOriginal);
            SetFaceFlagsForChara_0(ctrlGetScenarioChara(SFLU_JOP), FacialNodeOriginal);
            break;
        case 1147:
            EV_ToggleCharacterFlag(SFLU_KR2);
            EV_ToggleCharacterFlag(SFLU_BF6);
            EV_ToggleCharacterFlag(SFLU_KF1);
            break;
        case 1256:
            EV_SetObjectFlagByIndex(1, 1);
            EV_DisableCharacterDisplay(SFLU_KF1);
            EV_ToggleCharacterFlag(SFLU_KF1);
            EV_EnableCharacterDisplay(SFLU_KF2);
            EV_DisableCharacterDisplay(SFLU_BF6);
            EV_ToggleCharacterFlag(SFLU_BF6);
            EV_EnsureCharacterDraw(SFLU_KF1);
            EV_EnsureCharacterDraw(SFLU_KR2);
            break;
        default:
            break;
        }
        if (frame >= 880) {
            SetHighModelXYZ(ctrlGetScenarioChara(STREET_FIGHT_RYO), 0, 0, 0);
            SetHighModelXYZ(ctrlGetScenarioChara(STREET_FIGHT_RYO), 0, 1, 0);
            SetHighModelXYZ(ctrlGetScenarioChara(SFLU_JOP), 0, 0, 0);
            SetHighModelXYZ(ctrlGetScenarioChara(SFLU_JOP), 0, 1, 0);
        }
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        UnloadAuthorCharacters(command);
        loop.owner->state = 3;
        if ((command.name == SFLU_AUTH_0.name || command.name == SFLU_AUTH_1.name || command.name == SFLU_AUTH_4.name || command.name == SFLU_AUTH_5.name) &&
            command.name != SFLU_AUTH_4.name) {
            EV_lightLoad(SHEN_NAME(0000));
            PushGlobalLightingProgram();
        }
        HLib::DestroyCurrentTask();
    }
}
void ConfigureTrainingHands() {
    ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(STREET_FIGHT_RYO), 0, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
    ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(STREET_FIGHT_RYO), 1, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
    Queue_EVT_AT_CtrlHandUp_1(STREET_FIGHT_RYO, 0, reinterpret_cast<std::byte *>(chestHandPoseCommand), 4, 65);
    Queue_EVT_AT_CtrlHandUp_1(STREET_FIGHT_RYO, 1, reinterpret_cast<std::byte *>(SFTS_CLOSED_HAND), 4, 70);
    Queue_EVT_AT_CtrlHandUp_1(STREET_FIGHT_RYO, 0, reinterpret_cast<std::byte *>(SFTS_CLOSED_HAND), 4, 125);
    Queue_EVT_AT_CtrlHandUp_1(STREET_FIGHT_RYO, 1, reinterpret_cast<std::byte *>(chestHandPoseCommand), 4, 125);
    Queue_EVT_AT_CtrlHandUp_1(STREET_FIGHT_RYO, 0, reinterpret_cast<std::byte *>(openHandPoseCommand), 4, 145);
    Queue_EVT_AT_CtrlHandUp_1(STREET_FIGHT_RYO, 1, reinterpret_cast<std::byte *>(openHandPoseCommand), 4, 145);
    ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(SFLU_SF4), 0, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
    ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(SFLU_SF4), 1, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
    Queue_EVT_AT_CtrlHandUp_1(SFLU_SF4, 0, reinterpret_cast<std::byte *>(SFTS_CLOSED_HAND), 4, 42);
    Queue_EVT_AT_CtrlHandUp_1(SFLU_SF4, 1, reinterpret_cast<std::byte *>(chestHandPoseCommand), 4, 42);
    Queue_EVT_AT_CtrlHandUp_1(SFLU_SF4, 0, reinterpret_cast<std::byte *>(chestHandPoseCommand), 4, 56);
    Queue_EVT_AT_CtrlHandUp_1(SFLU_SF4, 1, reinterpret_cast<std::byte *>(SFLU_TRAINING_HIGH_HAND), 4, 56);
    Queue_EVT_AT_CtrlHandUp_1(SFLU_SF4, 1, reinterpret_cast<std::byte *>(openHandPoseCommand), 4, 76);
    Queue_EVT_AT_CtrlHandUp_1(SFLU_SF4, 0, reinterpret_cast<std::byte *>(SFLU_TRAINING_HIGH_HAND), 4, 125);
    Queue_EVT_AT_CtrlHandUp_1(SFLU_SF4, 1, reinterpret_cast<std::byte *>(SFLU_TRAINING_LOW_HAND), 4, 125);
    Queue_EVT_AT_CtrlHandUp_1(SFLU_SF4, 0, reinterpret_cast<std::byte *>(openHandPoseCommand), 4, 149);
    Queue_EVT_AT_CtrlHandUp_1(SFLU_SF4, 1, reinterpret_cast<std::byte *>(openHandPoseCommand), 4, 149);
}
void ConfigureVictoryHands() {
    ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(STREET_FIGHT_RYO), 0, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
    ApplyCharacterHandPoseCommand(ctrlGetScenarioChara(STREET_FIGHT_RYO), 1, reinterpret_cast<std::byte *>(openHandPoseCommand), 0);
    Queue_EVT_AT_CtrlHandUp_1(STREET_FIGHT_RYO, 0, reinterpret_cast<std::byte *>(chestHandPoseCommand), 4, 28);
    Queue_EVT_AT_CtrlHandUp_1(STREET_FIGHT_RYO, 1, reinterpret_cast<std::byte *>(chestHandPoseCommand), 4, 28);
    Queue_EVT_AT_CtrlHandUp_1(STREET_FIGHT_RYO, 0, reinterpret_cast<std::byte *>(openHandPoseCommand), 16, 40);
    Queue_EVT_AT_CtrlHandUp_1(STREET_FIGHT_RYO, 1, reinterpret_cast<std::byte *>(openHandPoseCommand), 16, 40);
    Queue_EVT_AT_CtrlHandUp_1(STREET_FIGHT_RYO, 0, reinterpret_cast<std::byte *>(chestHandPoseCommand), 4, 78);
    Queue_EVT_AT_CtrlHandUp_1(STREET_FIGHT_RYO, 1, reinterpret_cast<std::byte *>(chestHandPoseCommand), 16, 100);
}
void MainCallback(void *parameter) {
    using enum CameraState;
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    switch (work.state) {
    case 8:
    case 9:
    case 10:
    case 11:
    case 16:
    case 17:
    case 18:
    case 19:
    case 39:
    case 40:
    case 41:
        UpdateFenceAlpha();
        break;
    default:
        break;
    }
    switch (work.state) {
    case 0:
        ST_LoadTitleFade();
        vfmDispatchControl(FightControlOp::GaugeResources, {}, 0, 1);
        LoadBlurModelForCharacter(STREET_FIGHT_RYO, "RYO_B.MT7", 1);
        LoadBlurModelForCharacter(SFLU_SF4, "SF4_B.MT7", 1);
        work.state = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) ? 7 : work.startParameters->reserved08 == 99 ? 1 : 4;
        break;
    case 1:
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1794))
            EV_authSceneCtrl(AuthSceneCtrl::UnusedAuthorFlag);
        QueueAuthoredPluralControl(SFLU_PLURAL_FIRST);
        QueueAuthor(SFLU_AUTH_0);
        work.state = 2;
        break;
    case 2:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work.state = 3;
        break;
    case 3:
    case 37:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work.state = 38;
        break;
    case 4:
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1794))
            EV_authSceneCtrl(AuthSceneCtrl::UnusedAuthorFlag);
        QueueAuthoredPluralControl(SFLU_PLURAL_REPEAT);
        QueueAuthor(SFLU_AUTH_1);
        work.state = 5;
        break;
    case 5:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work.state = 6;
        break;
    case 6:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work.state = 7;
        break;
    case 7:
        ToggleFence(1);
        ToggleTrainingAudience(0);
        SetHighModelXYZ(ctrlGetScenarioChara(SFLU_SF4), 0, 1, 0);
        SetHighModelXYZ(ctrlGetScenarioChara(SFLU_SF4), 0, 0, 0);
        EV_EventSetUpHelpIcon_0(GetHelpIconForArea(), 0);
        PlaceNormal();
        EV_LoadAudio("0260A000", 2);
        EV_spmBeginSampleExecute();
        EV_DisableCharacterDisplay(SFLU_GOI);
        EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QKBT"), 0);
        SetScenarioCharacterType(STREET_FIGHT_RYO, CharacterTypeStencilShadow);
        SetScenarioCharacterType(SFLU_SF4, CharacterTypeStencilShadow);
        (void)EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014);
        work.state = 8;
        break;
    case 8: {
        {
            EventSetMotion motion{.cleanup = 0, .character = STREET_FIGHT_RYO, .animation = 306, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
            EVT_AT_SetMot_Callback(&motion);
            GetMotionFrameCount(306);
        };
        EVT_AT_SetPos(STREET_FIGHT_RYO, SFLU_BASE, {0, 0, -1.1f}, 0);
        EVT_AT_SetAngY(STREET_FIGHT_RYO, SFLU_BASE, 180, 0);
        {
            EventSetMotion motion{.cleanup = 0, .character = SFLU_SF4, .animation = 2732, .frame = 0, .speed = -1, .flags = 0x40000000U, .delay = 0};
            EVT_AT_SetMot_Callback(&motion);
            GetMotionFrameCount(2732);
        };
        EVT_AT_SetPos(SFLU_SF4, SFLU_BASE, {0, 0, 1.1f}, 0);
        EVT_AT_SetAngY(SFLU_SF4, SFLU_BASE, 0, 0);
        EVT_AT_SetAngY(SFLU_SF4, SFLU_BASE, 0, 0);
        SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(STREET_FIGHT_RYO), FacialExpressionId::Angry, 1, 1);
        SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(SFLU_SF4), FacialExpressionId::Angry, 1, 1);
        SetPluralControlFor(ctrlGetScenarioChara(STREET_FIGHT_RYO), 0);
        SetPluralControlFor(ctrlGetScenarioChara(SFLU_SF4), 0);
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) && static_cast<std::uint8_t>(work.round) < 3) {
            HLib::Task *const task = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, sizeof(SkEventMessageWork), STREET_FIGHT_EVENT_TASK);
            SkEventMessageWork *const message = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(task));
            message->frames = 60;
            message->message = SFLU_ROUND_MESSAGES[static_cast<std::uint8_t>(work.round)];
        }
        Vector3 position;
        Vector3i rotation;
        EV_GetCharaPosition(STREET_FIGHT_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        GetScenarioCharacterEulerAngles(STREET_FIGHT_RYO, &rotation, EventTransformFlags::World);
        AT_SetLocalBaseChara(STREET_FIGHT_RYO, STREET_FIGHT_INVALID);
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
        EV_AT_SetAngY(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
        savedPlayerPosition = position;
        savedPlayerRotation.y = rotation.y;
        GetScenarioCharacterEulerAngles(SFLU_SF4, &rotation, EventTransformFlags::World);
        EV_GetCharaPosition(SFLU_SF4, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        AT_SetLocalBaseChara(SFLU_SF4, STREET_FIGHT_INVALID);
        EV_AT_SetPos(SFLU_SF4, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
        EV_AT_SetAngY(SFLU_SF4, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
        savedFighterPosition = position;
        savedFighterRotation.y = rotation.y;
        qteBattleUtil(BattleQteMode::Load);
        EventFilmControl reveal{.cleanup = 0,
            .frames = 30,
            .startAlpha = 255,
            .startRed = 0,
            .startGreen = 0,
            .startBlue = 0,
            .targetAlpha = 0,
            .targetRed = 0,
            .targetGreen = 0,
            .targetBlue = 0,
            .delay = 0};
        EVT_AT_MenCtrl_Callback(&reveal);
        {
            EventSetCamera camera{.cleanup = 0, .camera = 1400, .eye = STREET_FIGHT_RYO, .target = STREET_FIGHT_RYO, .context = 0, .angle = 0, .delay = 0};
            EVT_AT_SetCam_Callback(&camera);
        };
        camCtrlDisableFilmBars();
        EV_PlayDialogue("0260F001");
        work.state = 9;
        break;
    }
    case 9:
        if (!camCtrlCheckIsEventCam()) {
            EV_PlayDialogue("0260F002");
            HLib::SetupTaskDelay(10);
            work.state = 10;
        }
        break;
    case 10: {
        figpSetFlagModesFor(STREET_FIGHT_RYO, 0, 0);
        if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014)) {
            figpSetFlagModesFor(SFLU_SF4, 0, 1);
            figpAssignModeFor(SFLU_SF4, 0);
        }
        qtePhase = 0;
        shenStartFGST(10, 10, 0, 0);
        vfmDispatchControl(FightControlOp::Package, {}, reinterpret_cast<std::uintptr_t>("QKBT"), 1);
        work.rank =
            static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{static_cast<std::uint32_t>(work.stage | 0x40)}, 0, -1));
        battleLevel = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{132}, 0, -1));
        if (battleLevel > 2)
            battleLevel = 2;
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014)) {
            shenInitFightData(ctrlGetScenarioChara(STREET_FIGHT_RYO), nullptr, 0, 0, 300, 0);
            shenInitFightData(ctrlGetScenarioChara(SFLU_SF4), nullptr, 1, 1, static_cast<std::uint16_t>(80 * work.rank + 300), 45);
        } else {
            shenInitFightData(ctrlGetScenarioChara(STREET_FIGHT_RYO), nullptr, 0, 0, 400, 0);
            shenInitFightData(ctrlGetScenarioChara(SFLU_SF4), nullptr, 1, 1, static_cast<std::uint16_t>(100 * (6 - battleLevel)), 26);
        }
        vfUpdateFlagsFor(ctrlGetScenarioChara(SFLU_SF4), 0, 0x8000000, battleLevel);
        if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
            vfUpdateFlagsFor(ctrlGetScenarioChara(SFLU_SF4), 0, 0x20000, static_cast<std::int32_t>(static_cast<float>(100 * (6 - battleLevel)) * .1f));
        vfCopyPosFromEntity(0, nullptr, 238.5f, 126, 4);
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014)) {
            const std::int64_t rank = vfmDispatchControl(FightControlOp::MotionNibble, shen_name{68}, 0, -1);
            vfmDispatchControl(FightControlOp::StreetFightLevel, {}, static_cast<std::uintptr_t>(rank + 1), 1);
            vfmDispatchControl(FightControlOp::RoundCounts, shen_name{static_cast<std::uint32_t>(work.wins)}, static_cast<std::uintptr_t>(work.losses), 1);
        } else
            EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x100}, 1);
        EV_vfBattleUtil(FightBattleUtilOp::CombatCameraFlags, shen_name{2}, 1);
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800}, 1);
        EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x80000}, 1);
        vfmQueueFightStart(2, 0x63, 1, 0, -1);
        vfmDispatchControl(FightControlOp::CameraParameter, {}, 13, 1);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1794, 1);
        SetCurrentGameFlag(static_cast<std::uint8_t>(GetStreetFightStageFlag(work.stage)), 1);
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014)) {
            const std::int32_t index = work.rank <= 1 ? 0 :
                work.rank <= 3                        ? 1 :
                work.rank <= 6                        ? 2 :
                work.rank <= 9                        ? 3 :
                work.rank <= 12                       ? 4 :
                work.rank <= 15                       ? 5 :
                                                        0;
            SetFightActorParameterBytes(SFLU_SF4, SFLU_FIGHT_PARAMETERS[index]);
        }
        work.state = 11;
        break;
    }
    case 11:
        if (IsFightActive()) {
            if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
                UpdateBattleQte(work);
        } else {
            work.battleStatus = GetBtlStatus(ctrlGetScenarioChara(STREET_FIGHT_RYO));
            if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014)) {
                const std::int64_t result = RecordStreetFightRound(&work);
                if (result) {
                    SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
                    spmFadeOut();
                    HLib::SetupTaskDelay(30);
                    work.state = result == 1 ? 38 : 39;
                } else
                    work.state = 8;
            } else {
                if (work.battleStatus == 3)
                    work.state = 13;
                else {
                    std::int64_t rank = vfmDispatchControl(FightControlOp::MotionNibble, shen_name{68}, 0, -1);
                    if (rank > 0)
                        --rank;
                    vfmDispatchControl(FightControlOp::MotionNibble, shen_name{68}, static_cast<std::uintptr_t>(rank), 1);
                    std::int64_t level = vfmDispatchControl(FightControlOp::MotionNibble, shen_name{132}, 0, -1);
                    if (level < 9)
                        ++level;
                    vfmDispatchControl(FightControlOp::MotionNibble, shen_name{132}, static_cast<std::uintptr_t>(level), 1);
                    spmFadeOut();
                    work.state = 39;
                }
                SetScreenFilmFade(15, 0, 0, 0, 0, 255, 0, 0, 0);
                HLib::SetupTaskDelay(15);
            }
        }
        break;
    case 13:
        SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(STREET_FIGHT_RYO), FacialExpressionId::Angry, 1, 1);
        SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(SFLU_SF4), FacialExpressionId::Angry, 1, 1);
        QueueAuthor(SFLU_AUTH_5);
        work.state = 14;
        break;
    case 14:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work.state = 15;
        break;
    case 15:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work.state = 16;
        break;
    case 16:
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, savedPlayerPosition);
        EV_AT_SetAngY(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, savedPlayerRotation);
        {
            EventSetMotion motion{.cleanup = 0, .character = STREET_FIGHT_RYO, .animation = 306, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
            EVT_AT_SetMot_Callback(&motion);
            GetMotionFrameCount(306);
        };
        EV_AT_SetPos(SFLU_SF4, EventTransformFlags::World | EventTransformFlags::AllAxes, savedFighterPosition);
        EV_AT_SetAngY(SFLU_SF4, EventTransformFlags::World | EventTransformFlags::AllAxes, savedFighterRotation);
        {
            EventSetMotion motion{.cleanup = 0, .character = SFLU_SF4, .animation = 2732, .frame = 0, .speed = -1, .flags = 0x40000000U, .delay = 0};
            EVT_AT_SetMot_Callback(&motion);
            GetMotionFrameCount(2732);
        };
        EV_LoadAudio("0260A000", 2);
        EV_spmBeginSampleExecute();
        figpSetFlagModesFor(STREET_FIGHT_RYO, 0, 0);
        figpSetFlagModesFor(SFLU_SF4, 0, 0);
        qteBattleUtil(BattleQteMode::Load);
        {
            EventSetCamera camera{.cleanup = 0, .camera = 1401, .eye = STREET_FIGHT_RYO, .target = STREET_FIGHT_RYO, .context = 0, .angle = 0, .delay = 0};
            EVT_AT_SetCam_Callback(&camera);
        };
        camCtrlDisableFilmBars();
        EVT_AT_SetVoice(shenGetRandomInt32(2) ? "0260B045" : "0260B046", 1, 30); // @text 0260B045 EN: "Strike me!" | JA: "来るがいい！" ; 0260B046 EN: "Strike me!" | JA: "ゆくぞっ！！"
        SetScreenFilmFade(15, 255, 0, 0, 0, 0, 0, 0, 0);
        HLib::SetupTaskDelay(15);
        work.state = 17;
        break;
    case 17:
        if (!camCtrlCheckIsEventCam()) {
            qtePhase = 0;
            shenStartFGST(10, 10, 0, 0);
            vfmDispatchControl(FightControlOp::Package, {}, reinterpret_cast<std::uintptr_t>("QKBT"), 1);
            battleLevel = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{132}, 0, -1));
            if (battleLevel > 2)
                battleLevel = 2;
            shenInitFightData(ctrlGetScenarioChara(STREET_FIGHT_RYO), nullptr, 0, 0, 400, 0);
            shenInitFightData(ctrlGetScenarioChara(SFLU_SF4), nullptr, 1, 1, static_cast<std::uint16_t>(100 * (6 - battleLevel)), 45);
            vfUpdateFlagsFor(ctrlGetScenarioChara(SFLU_SF4), 0, 0x8000000, battleLevel);
            if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
                vfUpdateFlagsFor(
                    ctrlGetScenarioChara(SFLU_SF4), 0, 0x20000, static_cast<std::int32_t>(static_cast<float>(100 * (6 - battleLevel)) * .1f));
            if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014)) {
                const std::int64_t rank = vfmDispatchControl(FightControlOp::MotionNibble, shen_name{68}, 0, -1);
                vfmDispatchControl(FightControlOp::StreetFightLevel, {}, static_cast<std::uintptr_t>(rank + 1), 1);
            } else
                EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x100}, 1);
            vfCopyPosFromEntity(0, nullptr, 238.5f, 126, 4);
            EV_vfBattleUtil(FightBattleUtilOp::CombatCameraFlags, shen_name{2}, 1);
            EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800}, 1);
            vfmQueueFightStart(2, 0x63, 1, 0, -1);
            vfmDispatchControl(FightControlOp::CameraParameter, {}, 13, 1);
            work.state = 18;
        }
        break;
    case 18:
        if (IsFightActive()) {
            if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
                UpdateBattleQte(work);
        } else {
            work.battleStatus = GetBtlStatus(ctrlGetScenarioChara(STREET_FIGHT_RYO));
            if (work.battleStatus == 3) {
                RecordStreetFightVictory(work.stage);
                work.state = 19;
            } else {
                RecordStreetFightDefeat(work.stage);
                spmFadeOut();
                work.state = 39;
            }
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        }
        break;
    case 19:
        if (!filmCheckFadeTrackColour()) {
            SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(STREET_FIGHT_RYO), FacialExpressionId::Angry, 1, 1);
            SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(SFLU_SF4), FacialExpressionId::Angry, 1, 1);
            ToggleFence(0);
            ToggleTrainingAudience(1);
            AT_SetLocalBaseChara(STREET_FIGHT_RYO, SFLU_BASE);
            AT_SetLocalBaseChara(SFLU_SF4, SFLU_BASE);
            vfmDispatchControl(FightControlOp::GaugeResources, {}, 0, 0);
            EV_EventSetUpHelpIcon_0(0, 0);
            work.state = 20;
        }
        break;
    case 20:
        QueueAuthor(SFLU_AUTH_6);
        work.state = 21;
        break;
    case 21:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            ToggleTrainingAudience(0);
            work.state = 22;
        }
        break;
    case 22:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            ToggleTrainingAudience(1);
            work.state = 23;
            trainingLevel = 0;
        }
        break;
    case 23:
        nullsub();
        ToggleTrainingAudience(0);
        EV_LoadAudio("0260A000", 2);
        EV_spmBeginSampleExecute();
        EVT_AT_SetPos(SFLU_BASE, STREET_FIGHT_INVALID, SFLU_TRAINING_BASE, 0);
        EVT_AT_SetAngY(SFLU_BASE, STREET_FIGHT_INVALID, 0, 0);
        {
            EventSetMotion motion{.cleanup = 0, .character = STREET_FIGHT_RYO, .animation = 10179, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
            EVT_AT_SetMot_Callback(&motion);
            GetMotionFrameCount(10179);
        };
        EVT_AT_SetPos(STREET_FIGHT_RYO, SFLU_BASE, {0, 0, 1.1f}, 0);
        EVT_AT_SetAngY(STREET_FIGHT_RYO, SFLU_BASE, 0, 0);
        {
            EventSetMotion motion{.cleanup = 0, .character = SFLU_SF4, .animation = 10177, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
            EVT_AT_SetMot_Callback(&motion);
            GetMotionFrameCount(10177);
        };
        EVT_AT_SetPos(SFLU_SF4, SFLU_BASE, {0, 0, -1.1f}, 0);
        EVT_AT_SetAngY(SFLU_SF4, SFLU_BASE, 0, 0);
        mtwkSetReactFlags(scenFindMotionwork(STREET_FIGHT_RYO), 13);
        mtwkSetReactFlags(scenFindMotionwork(SFLU_SF4), 13);
        EV_EnableCharacterDisplay(STREET_FIGHT_RYO);
        EV_EnableCharacterDisplay(SFLU_SF4);
        SetLowQualityHands(ctrlGetScenarioChara(SFLU_SF4));
        SetFaceFlagsForChara_0(ctrlGetScenarioChara(STREET_FIGHT_RYO), FacialNodeReplacement);
        SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(STREET_FIGHT_RYO), FacialExpressionId::Angry, 0, 1);
        SetHighModelXYZ(ctrlGetScenarioChara(STREET_FIGHT_RYO), 1, 0, 0);
        SetHighModelXYZ(ctrlGetScenarioChara(STREET_FIGHT_RYO), 1, 1, 0);
        SetHighModelXYZ(ctrlGetScenarioChara(SFLU_SF4), 1, 0, 0);
        SetHighModelXYZ(ctrlGetScenarioChara(SFLU_SF4), 1, 1, 0);
        ConfigureTrainingHands();
        if (cameraManualMode) {
            camCtrlSetState(Event);
            camCtrlSetNextPosition({241.81500244140625f, 84.55899810791016f, 129.4429931640625f});
            camCtrlSetTargetPosition({237.87210083007812f, 82.35199737548828f, 125.6709976196289f});
        } else {
            {
                EventSetCamera camera{.cleanup = 0, .camera = 1410, .eye = SFLU_BASE, .target = SFLU_BASE, .context = 0, .angle = 0, .delay = 0};
                EVT_AT_SetCam_Callback(&camera);
            };
            camCtrlDisableFilmBars();
        }
        trainingFrames = 0;
        qtePhase = 0;
        qteBattleUtil(BattleQteMode::Load);
        qteBattleUtil(BattleQteMode::SetWait, 25, 6, SFLU_TRAINING_WAIT[trainingLevel], 0);
        qteBattleUtil(BattleQteMode::SetFirstCommands, 16, 1024, 1, 256);
        qteBattleUtil(BattleQteMode::SetLastCommands, 0, 0, 0, 0);
        qteBattleUtil(BattleQteMode::ResetStates);
        qteBattleUtil(BattleQteMode::Queue);
        SetScreenFilmFade(10, 255, 0, 0, 0, 0, 0, 0, 0);
        work.state = 24;
        break;
    case 24: {
        const std::int32_t frame = trainingFrames++;
        switch (frame) {
        case 23:
        case 43:
        case 83:
        case 103:
            SetShowQteFlagForButtonType(SFLU_TRAINING_BUTTONS[qtePhase], 15, 0);
            ++qtePhase;
            failedInput = 0;
            break;
        case 40:
        case 70:
        case 105:
            if (!cameraManualMode) {
                {
                    EventSetCamera camera{.cleanup = 0,
                        .camera = frame == 40 ? 1411 :
                            frame == 70       ? 1412 :
                                                1413,
                        .eye = SFLU_BASE,
                        .target = SFLU_BASE,
                        .context = 0,
                        .angle = 0,
                        .delay = 0};
                    EVT_AT_SetCam_Callback(&camera);
                };
                camCtrlDisableFilmBars();
            }
            break;
        case 42:
        case 62:
        case 102:
        case 122:
            if (failedInput) {
                work.state = 27;
                trainingFrames = 0;
                return;
            }
            break;
        case 79:
            SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(STREET_FIGHT_RYO), FacialExpressionId::Suffer, 0, 1);
            break;
        case 151:
            SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(STREET_FIGHT_RYO), FacialExpressionId::Angry, 0, 1);
            break;
        case 155:
            qteBattleUtil(BattleQteMode::Start);
            work.state = 25;
            break;
        default:
            break;
        }
        if (trainingFrames > 23) {
            const std::int32_t status = EV_GetCurrentQteStatus();
            if (status == -24576 || !status)
                failedInput = 1;
        }
        break;
    }
    case 25: {
        const std::int64_t result = qteBattleUtil(BattleQteMode::GetResult);
        if (result) {
            trainingFrames = 0;
            if (result == 1) {
                failedInput = 0;
                work.state = 26;
            } else {
                work.state = 27;
                failedInput = 1;
            }
        }
        break;
    }
    case 26: {
        const std::int32_t frame = trainingFrames++;
        switch (frame) {
        case 0:
            if (!cameraManualMode) {
                {
                    EventSetCamera camera{.cleanup = 0, .camera = 1415, .eye = SFLU_BASE, .target = SFLU_BASE, .context = 0, .angle = 0, .delay = 0};
                    EVT_AT_SetCam_Callback(&camera);
                };
                camCtrlDisableFilmBars();
            }
            break;
        case 5: {
            EventSetMotion motion{.cleanup = 0, .character = STREET_FIGHT_RYO, .animation = 10182, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
            EVT_AT_SetMot_Callback(&motion);
            GetMotionFrameCount(10182);
        };
            EVT_AT_SetPos(STREET_FIGHT_RYO, SFLU_BASE, SFLU_TRAINING_PLAYER_SUCCESS_POSITION, 0);
            EVT_AT_SetAngY(STREET_FIGHT_RYO, SFLU_BASE, 115, 0);
            {
                EventSetMotion motion{.cleanup = 0, .character = SFLU_SF4, .animation = 10181, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
                EVT_AT_SetMot_Callback(&motion);
                GetMotionFrameCount(10181);
            };
            EVT_AT_SetPos(SFLU_SF4, SFLU_BASE, SFLU_TRAINING_FIGHTER_SUCCESS_POSITION, 0);
            EVT_AT_SetAngY(SFLU_SF4, SFLU_BASE, 115, 0);
            mtwkSetReactFlags(scenFindMotionwork(STREET_FIGHT_RYO), 13);
            mtwkSetReactFlags(scenFindMotionwork(SFLU_SF4), 13);
            SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(STREET_FIGHT_RYO), FacialExpressionId::Angry, 0, 1);
            ConfigureVictoryHands();
            break;
        case 9:
            camCtrlSetRotationXYZ(0, .02f, .8500000238418579f);
            break;
        case 16:
            if (!cameraManualMode) {
                {
                    EventSetCamera camera{.cleanup = 0, .camera = 1416, .eye = SFLU_BASE, .target = SFLU_BASE, .context = 0, .angle = 0, .delay = 0};
                    EVT_AT_SetCam_Callback(&camera);
                };
                camCtrlDisableFilmBars();
            }
            camCtrlSetRotationXYZ(0, 0, 0);
            break;
        case 41:
            camCtrlSetRotationXYZ(.1f, 0, .6499999761581421f);
            break;
        case 55:
            if (!cameraManualMode) {
                {
                    EventSetCamera camera{.cleanup = 0, .camera = 1417, .eye = SFLU_BASE, .target = SFLU_BASE, .context = 0, .angle = 0, .delay = 0};
                    EVT_AT_SetCam_Callback(&camera);
                };
                camCtrlDisableFilmBars();
            }
            break;
        case 62:
            camCtrlSetRotationXYZ(.1f, .1f, .6000000238418579f);
            break;
        case 83:
            camCtrlSetRotationXYZ(0, .25f, .8500000238418579f);
            break;
        case 110:
            work.state = 28;
            break;
        default:
            break;
        }
        break;
    }
    case 27: {
        const std::int32_t frame = trainingFrames++;
        if (!frame) {
            {
                EventSetMotion motion{
                    .cleanup = 0, .character = STREET_FIGHT_RYO, .animation = 10183, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
                EVT_AT_SetMot_Callback(&motion);
                GetMotionFrameCount(10183);
            };
            EVT_AT_SetPos(SFLU_BASE, STREET_FIGHT_INVALID, SFLU_STAGE_ORIGIN, 0);
            EVT_AT_SetAngY(SFLU_BASE, STREET_FIGHT_INVALID, 0, 0);
            EVT_AT_SetPos(STREET_FIGHT_RYO, SFLU_BASE, {0, 0, 0}, 0);
            EVT_AT_SetAngY(STREET_FIGHT_RYO, SFLU_BASE, 180, 0);
            EV_EnableCharacterDisplay(STREET_FIGHT_RYO);
            EV_DisableCharacterDisplay(SFLU_SF4);
            mtwkSetSpeed(scenFindMotionwork(SFLU_SF4), 0, MotionRegion::LowerBody);
            mtwkSetReactFlags(scenFindMotionwork(STREET_FIGHT_RYO), 13);
            SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(STREET_FIGHT_RYO), FacialExpressionId::Painful, 0, 1);
            if (!cameraManualMode) {
                {
                    EventSetCamera camera{.cleanup = 0, .camera = 1420, .eye = SFLU_BASE, .target = SFLU_BASE, .context = 0, .angle = 0, .delay = 0};
                    EVT_AT_SetCam_Callback(&camera);
                };
                camCtrlDisableFilmBars();
                camCtrlSetRotationXYZ(0, .3f, .65f);
            }
        } else if (frame == 90) {
            if (trainingLevel < 2)
                ++trainingLevel;
            work.state = 28;
        }
        break;
    }
    case 28:
        EVT_AT_SetPos(SFLU_BASE, STREET_FIGHT_INVALID, SFLU_STAGE_ORIGIN, 0);
        if (g_entryPoint == 3) {
            dbgPrintEventMessage(SFLU_TRAINING_RETRY_MESSAGE);
            if (GetCurrentControlInput(INPUT_NEW_PRESSES) & 0x606U) {
                qteBattleUtil(BattleQteMode::RetireAndFree);
                SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
                HLib::SetupTaskDelay(30);
                ctrlStopSubtitles();
                work.state = 23;
            }
        } else {
            qteBattleUtil(BattleQteMode::RetireAndFree);
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            if (failedInput) {
                HLib::SetupTaskDelay(90);
                work.state = 23;
            } else {
                spmFadeOut();
                HLib::SetupTaskDelay(30);
                work.state = 29;
            }
        }
        break;
    case 29: {
        DateTime date;
        shenGetCurrentDateTime(date);
        if (date.hour > 17 || (date.hour == 17 && date.minute >= 30))
            ++date.day;
        date.hour = 17;
        date.minute = 30;
        date.second = 0;
        SetCurrentDateTimeTo(date);
        shenSetTimeSimulationTo(0);
        EV_LoadAudio("0260A000", 3);
        EV_spmBeginSampleExecute();
        ToggleTrainingAudience(1);
        QueueAuthoredPluralControl(SFLU_PLURAL_EVENING);
        QueueAuthor(SFLU_AUTH_2);
        work.state = 30;
        break;
    }
    case 30:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work.state = 31;
        break;
    case 31:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work.state = 32;
        break;
    case 32:
        QueueAuthoredPluralControl(SFLU_PLURAL_RESULT);
        QueueAuthor(SFLU_AUTH_3);
        work.state = 33;
        break;
    case 33:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work.state = 34;
        break;
    case 34:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work.state = 35;
        break;
    case 35: {
        const shen_name characters[]{SFLU_KF1, SFLU_KF2, SFLU_KF3, SFLU_KR1, SFLU_KR2, SFLU_SF4, SFLU_SF9, SFLU_BF6, SFLU_GOI};
        for (const shen_name character : characters) {
            EV_DisableCharacterDisplay(character);
            EV_ToggleCharacterFlag(character);
        }
        QueueAuthor(SFLU_AUTH_4);
        work.state = 36;
        break;
    }
    case 36:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work.state = 37;
        break;
    case 38:
        SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(STREET_FIGHT_RYO), FacialExpressionId::Angry, 1, 1);
        SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(SFLU_SF4), FacialExpressionId::Angry, 1, 1);
        HLib::SetCurrentTaskTo(EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) ? RetryBeginCallback : CleanupNormalCallback);
        break;
    case 39:
        if (!filmCheckFadeTrackColour()) {
            SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(STREET_FIGHT_RYO), FacialExpressionId::Angry, 1, 1);
            SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(SFLU_SF4), FacialExpressionId::Angry, 1, 1);
            QueueSfluStreetFightConversation(DefeatConversation, &work);
            work.state = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) ? 40 : 42;
        }
        break;
    case 40:
        work.state = 41;
        break;
    case 41:
        if (!filmCheckFadeTrackColour()) {
            EV_EnableCharacterDisplay(STREET_FIGHT_RYO);
            ToggleFence(0);
            ToggleTrainingAudience(1);
            work.state = 38;
        }
        break;
    case 42:
        EV_ResetFreeTalkFlags();
        Queue_ST_LoadTitleFadeIn(SFLU_RETRY_TITLE);
        if (work.audienceTask) {
            StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
            if (audience)
                audience->mode |= 2;
        }
        work.state = 43;
        work.frameCounter = 60;
        break;
    case 43:
        if (work.frameCounter-- == 1) {
            ST_LoadTitleFade();
            work.frameCounter = 60;
            work.state = 44;
        }
        break;
    case 44:
        if (work.frameCounter-- == 1)
            work.state = 38;
        break;
    default:
        break;
    }
}
void LostExpressionCallback(void *) {
    switch (lostFrames) {
    case 0:
        SetFacialBlinkDelay(ctrlGetScenarioChara(SFLU_SF4), 0);
        SetEyeToChara(ctrlGetScenarioChara(SFLU_SF4), &SFLU_LOST_EYE_FIRST, 1, 2);
        break;
    case 120: {
        Vector3 camera;
        camCtrlGetNextCamPos(&camera);
        SetEyeToChara(ctrlGetScenarioChara(SFLU_SF4), &SFLU_LOST_EYE_SECOND, 12, 2);
        break;
    }
    case 150:
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        break;
    case 179:
        SetEyeToChara(ctrlGetScenarioChara(SFLU_SF4), nullptr, 16, 0);
        SetFacialBlinkDelay(ctrlGetScenarioChara(SFLU_SF4), 90);
        HLib::DestroyCurrentTask();
        break;
    default:
        break;
    }
    ++lostFrames;
}
std::int32_t DefeatConversation(void *parameter) {
    const StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    const shen_name fighter = work.fighters[work.fighterIndex];
    EV_DisableCharacterDisplay(STREET_FIGHT_RYO);
    EV_SetCharacterAlphaEnabled(fighter, 0);
    SetHighModelXYZ(ctrlGetScenarioChara(fighter), 1, 1, 0);
    SetHighModelXYZ(ctrlGetScenarioChara(fighter), 1, 0, 0);
    EVT_AT_SetPos(fighter, SFLU_BASE, {}, 0);
    EVT_AT_SetAngY(fighter, SFLU_BASE, 0, 0);
    EventSetMotion motion{.cleanup = 0, .character = fighter, .animation = 10185, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(10185);
    EVT_AT_SetVoice("0260B027", 1, 41);
    EVT_AT_MenCtrl(EventFilmControl{.cleanup = 1,
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
    EVT_AT_SetCam(8099, SFLU_BASE, SFLU_BASE, 0, 0, 1);
    lostFrames = 0;
    HLib::EnqueueTaskWithoutParameter(LostExpressionCallback, 5, 0x0b, static_cast<std::int32_t>(STREET_FIGHT_EVENT_TASK.name));
    return 180;
}
void CleanupNormalCallback(void *parameter) {
    using enum CameraState;
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    StreetFightStartParameters *const start = work.startParameters;
    const std::int32_t result = work.battleStatus;
    const std::uint32_t entry = start->reserved08;
    EV_NPC_NewLoadSw(1);
    ET_AT_KillWork(work.characterReadTask);
    SetPlayerControlEnabled(0);
    SetCharaKill(SFLU_BASE, 1);
    GetNpcBatchCountOverride(work.previousNpcBatchCount);
    EV_FreeAuthorPak(0, "0260");
    if (!(start->flags & 1))
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QKBT"), 0);
    EV_spmBeginSampleExecute();
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    if (entry == 99) {
        sbtLoadProgram();
        EV_AT_SetPos(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFLU_STORY_EXIT_POSITION);
        EV_AT_SetAngY(STREET_FIGHT_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SFLU_STORY_EXIT_ROTATION);
        mtwkRequestMotion(scenFindMotionwork(STREET_FIGHT_RYO), 0xf003, MotionRegion::LowerBody);
        mtwkSetRegionFlag(scenFindMotionwork(STREET_FIGHT_RYO), 71, 0, MotionRegion::LowerBody);
        camCtrlSetState(Init);
        camCtrlSetFovTransitionTo(true);
    } else if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        EV_scenQueueLoad(4, SHEN_NAME(OMAK), 1);
    else if (result != 3) {
        sbtLoadProgram();
        EV_scenQueueLoad(g_actNumber, SFLU_STORY_EXIT_AREA, 1);
    } else {
        EV_stepSetFlags(0, 0x16a);
        EV_scenQueueLoad(g_actNumber, SFLU_STORY_NEXT_AREA, 0);
    }
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1233, 1);
    if (start->signal)
        *start->signal = 0;
}
void ResetMain(StreetFightEventWork &work) {
    EV_ResetFreeTalkFlags();
    work.state = 7;
    work.auxiliaryState = 0;
    work.round = work.wins = work.losses = 0;
    work.roundResults[0] = work.roundResults[1] = work.roundResults[2] = 0;
    HLib::SetCurrentTaskTo(MainCallback);
    if (work.audienceTask) {
        StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
        if (audience)
            audience->mode &= ~2U;
    }
    EV_LoadAudio("0260A000", 2);
    EV_spmBeginSampleExecute();
}
void RetryBeginCallback(void *parameter) {
    StreetFightEventWork &work = *static_cast<StreetFightEventWork *>(parameter);
    if (work.battleStatus == 3)
        RecordStreetFightVictory(work.stage);
    else
        RecordStreetFightDefeat(work.stage);
    dbgPrintEventMessage(GetLocalizedSceneText("UISMRETRY").data()); // @text EN: "Retry?" | JA: "もう一度？"
    EVT_AT_KsIcon_Enqueue(-1, 82, 83, -1, 0x7fff, 0xffff, 2, 0);
    if (work.audienceTask) {
        StreetFightAudienceWork *const audience = static_cast<StreetFightAudienceWork *>(HLib::GetTaskParameter(work.audienceTask));
        if (audience)
            audience->mode |= 2U;
    }
    HLib::SetCurrentTaskTo(RetryChoiceCallback);
}
void RetryChoiceCallback(void *parameter) {
    const std::uint32_t choice = PollChoice();
    if (!choice)
        return;
    ctrlStopSubtitles();
    if (choice == 82)
        ResetMain(*static_cast<StreetFightEventWork *>(parameter));
    else
        HLib::SetCurrentTaskTo(CleanupNormalCallback);
}
StreetFightEventWork *InitializeNormal(HLib::Task *task) {
    StreetFightEventWork *const work = static_cast<StreetFightEventWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->state = work->battleStatus = 0;
    work->previousNpcBatchCount = GetNpcBatchCountOverride(35);
    work->fighterIndex = 0;
    work->fighters[0] = SFLU_SF4;
    work->fighters[1] = STREET_FIGHT_INVALID;
    work->characterReadTask = Queue_EVT_AT_ReadChr(task, SFLU_NORMAL_CHARACTERS);
    work->audienceTask = nullptr;
    work->fadeFrames = 0;
    work->attempts = 0;
    work->reserved169[3] = work->reserved169[4] = work->reserved169[5] = work->reserved169[6] = std::byte{};
    work->stage = GetStreetFightStageForCurrentArea();
    work->rank = static_cast<std::int32_t>(vfmDispatchControl(FightControlOp::MotionNibble, shen_name{static_cast<std::uint32_t>(work->stage | 0x40)}, 0, -1));
    work->round = work->wins = work->losses = 0;
    work->roundResults[0] = work->roundResults[1] = work->roundResults[2] = 0;
    return work;
}
void QueueNormal(StreetFightStartParameters *start) {
    if (start->signal)
        *start->signal = 1;
    SetPlayerControlEnabled(0);
    SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(MainCallback, 5, 0x0b, sizeof(StreetFightEventWork), STREET_FIGHT_EVENT_TASK);
    StreetFightEventWork *const work = InitializeNormal(task);
    work->startParameters = start;
    EV_FreeAuthorPak(1, "0260");
    if (!EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QKBT"), 0)) {
        const char *const path = Filepath_Gen(FilepathGenMode::SceneDirectory);
        EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("QKBT"));
        EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    }
    EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QKBT"), 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QKBT"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, SFLU_BASE);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0260A000");
    EV_spmBeginSampleExecute();
    FinishLoading();
    shenSetClockAndCompassRenderTo(1);
}
void Init() {
    savedPlayerPosition = {};
    savedFighterPosition = {};
    savedPlayerRotation = {};
    savedFighterRotation = {};
    qtePhase = 0;
    trainingFrames = 0;
    battleLevel = 0;
    trainingLevel = 0;
    lostFrames = 0;
}
void Main(std::uintptr_t parameter) {
    StreetFightStartParameters *const start = reinterpret_cast<StreetFightStartParameters *>(parameter);
    if (start->scene == 99)
        QueueDebug(start);
    else
        QueueNormal(start);
}
} // namespace
const ScriptAreaDefinition areaSfluScriptDefinition{.areaId = STREET_FIGHT_SFLU, .field04 = 0, .init = Init, .main = Main};
} // namespace shenmue::s2
#endif
