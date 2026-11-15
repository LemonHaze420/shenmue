#if defined(SHENMUE_EVENT_OVERLAY) && SHENMUE_TARGET_S2_DC_JP_GAMEJAM
#include "s2/areas/area_drbl_dc.hpp"
double sqrt(double value);
#define NAME(a, b, c, d) SHEN_NAME4(a, b, c, d)
#define CAMERA_DUMMY NAME('C', 'M', 'D', 'M')
#define BALL_PLAYER NAME('R', 'Y', 'O', '_')
#define BALL_DUCK NAME('R', 'U', 'C', 'K')
#define TRANSFORM 0x38000000
#if defined(SHENMUE_EVENT_ENTRY)
void QueueDropBall(DropBallWork *argument) {
    HLibTask *task;
    DropBallWork *work;
    task = FTBL.HLib_EnqueueTaskWithParameter(DropBallCallback, 6, 11, sizeof(DropBallWork), TASK_EVNT);
    work = (DropBallWork *)FTBL.HLib_GetTaskParameter(task);
    work->parent = argument->parent;
    work->mode = argument->mode;
    work->board = argument->board;
    work->wins = argument->wins;
    InitDropBall(work, task);
}
#else
void InitDropBall(DropBallWork *work, HLibTask *task) {
    work->task = task;
    work->pocketCount = 0;
    work->activeBall = 0;
    work->state = 0;
    *work->wins = 0;
    work->soundFrames = 0;
    work->sound = 0;
    FTBL.GetCharaPosition(work->board, -1, &work->boardPosition, 0);
    FTBL.GetScenarioCharacterEulerAngles(work->board, &work->boardRotation, 0);
    CreateDropBalls(work);
    CreateDropPockets(work);
    switch (work->mode) {
    case 50:
    case 51:
    case 52:
    case 53:
    case 54:
    case 55:
    case 56:
    case 57:
    default:
        FTBL.EVS_LoopCamera(960, CAMERA_DUMMY, CAMERA_DUMMY);
        work->scale = 1.0f;
        break;
    case 58:
    case 59:
        FTBL.EVS_LoopCamera(965, CAMERA_DUMMY, CAMERA_DUMMY);
        work->scale = 1.0f;
        break;
    case 60:
    case 61:
    case 62:
    case 63:
        FTBL.EVS_LoopCamera(966, CAMERA_DUMMY, CAMERA_DUMMY);
        work->scale = 2.0f;
        break;
    }
}
void CreateDropBalls(DropBallWork *work) {
    Vector3i rotation = {5461, 0, 0};
    Vector3 origin = {0, 0, 0}, scale = {.87f, .87f, .87f}, largeScale = {3.48f, 3.48f, 3.48f};
    shen_name names[] = {NAME('B', 'L', '_', '0'), NAME('B', 'L', '_', '1'), NAME('B', 'L', '_', '2'), 0};
    unsigned char i;
    FTBL.EV_DisableCharacterDisplay(BALL_PLAYER);
    FTBL.EV_DisableCharacterDisplay(NAME('B', 'L', '_', '1'));
    FTBL.EV_DisableCharacterDisplay(NAME('B', 'L', '_', '2'));
    if (FTBL.DoesScenarioCharacterExist(BALL_DUCK) == 1)
        FTBL.EV_DisableCharacterDisplay(BALL_DUCK);
    FTBL.AT_SetLocalBaseChara(CAMERA_DUMMY, work->board);
    FTBL.EV_AT_SetPos(CAMERA_DUMMY, TRANSFORM, &origin);
    FTBL.EV_AT_SetAngY_0(CAMERA_DUMMY, TRANSFORM, &rotation, 0);
    switch (work->mode) {
    case 50:
    case 51:
    case 52:
    case 53:
    case 54:
    case 55:
    case 56:
    case 57:
    default:
        work->top = .847999989986f;
        work->bottom = .0219999999f;
        work->left = -.192999989f;
        work->right = .192999989f;
        work->pocketCount = 4;
        break;
    case 58:
    case 59:
        work->top = 1.48300004f;
        work->bottom = .0219999999f;
        work->left = -.192999989f;
        work->right = .192999989f;
        work->pocketCount = 8;
        break;
    case 60:
    case 61:
    case 62:
    case 63:
        work->top = 5.93200016f;
        work->bottom = .0879999995f;
        work->left = -.771999955f;
        work->right = .771999955f;
        work->pocketCount = 8;
        break;
    }
    i = 0;
    while (names[i]) {
        if ((unsigned short)(work->mode - 60) < 4)
            FTBL.mtwkSetMotionworkScale3(names[i], &largeScale);
        else
            FTBL.mtwkSetMotionworkScale3(names[i], &scale);
        work->balls[i].character = names[i];
        SetDropVector(&work->balls[i].position, work->right, work->top, .022f);
        SetDropVector(&work->balls[i].velocity, 0, 0, 0);
        CopyDropVector(&work->balls[i].previous, &work->balls[i].position);
        CopyDropVector(&work->balls[i].previousVelocity, &work->balls[i].velocity);
        work->balls[i].quietX = 0;
        work->balls[i].quietY = 0;
        work->balls[i].angle = 0;
        i++;
    }
}
void CreateDropPockets(DropBallWork *work) {
    Vector3i rotation = {0, 0, 0};
    Vector3 positions[8] = {
        {-.1f, .475f, 0}, {.1f, .475f, 0}, {-.1f, .145f, 0}, {.1f, .145f, 0}, {-.1f, .805f, 0}, {.1f, .805f, 0}, {-.1f, 1.135f, 0}, {.1f, 1.135f, 0}};
    Vector3 largePositions[8] = {
        {-.4f, 1.9f, 0}, {.4f, 1.9f, 0}, {-.4f, .58f, 0}, {.4f, .58f, 0}, {-.4f, 3.22f, 0}, {.4f, 3.22f, 0}, {-.4f, 4.54f, 0}, {.4f, 4.54f, 0}};
    Vector3 scale = {4, 4, 4};
    int i;
    switch (work->mode) {
    case 53:
    case 54:
    case 55:
        work->pockets[0] = FTBL.CreateEventCharacter(3, "NB_A", 0);
        work->pockets[1] = FTBL.CreateEventCharacter(3, "NB_A", 0);
        work->pockets[2] = FTBL.CreateEventCharacter(3, "NB_B", 0);
        work->pockets[3] = FTBL.CreateEventCharacter(3, "NB_B", 0);
        break;
    case 50:
    case 51:
    case 56:
        work->pockets[0] = FTBL.CreateEventCharacter(3, "NB_A", 0);
        work->pockets[1] = FTBL.CreateEventCharacter(3, "NB_A", 0);
        work->pockets[2] = FTBL.CreateEventCharacter(3, "NB_C", 0);
        work->pockets[3] = FTBL.CreateEventCharacter(3, "NB_C", 0);
        break;
    case 52:
    case 57:
        work->pockets[0] = FTBL.CreateEventCharacter(3, "NB_C", 0);
        work->pockets[1] = FTBL.CreateEventCharacter(3, "NB_C", 0);
        work->pockets[2] = FTBL.CreateEventCharacter(3, "NB_A", 0);
        work->pockets[3] = FTBL.CreateEventCharacter(3, "NB_A", 0);
        break;
    case 58:
    case 59:
    case 60:
    case 61:
    case 62:
    case 63:
        work->pockets[0] = FTBL.CreateEventCharacter(3, "NB_A", 0);
        work->pockets[1] = FTBL.CreateEventCharacter(3, "NB_A", 0);
        work->pockets[2] = FTBL.CreateEventCharacter(3, "NB_B", 0);
        work->pockets[3] = FTBL.CreateEventCharacter(3, "NB_B", 0);
        work->pockets[4] = FTBL.CreateEventCharacter(3, "NB_B", 0);
        work->pockets[5] = FTBL.CreateEventCharacter(3, "NB_B", 0);
        work->pockets[6] = FTBL.CreateEventCharacter(3, "NB_C", 0);
        work->pockets[7] = FTBL.CreateEventCharacter(3, "NB_C", 0);
        break;
    default:
        work->pockets[0] = FTBL.CreateEventCharacter(3, "NB_A", 0);
        work->pockets[1] = FTBL.CreateEventCharacter(3, "NB_A", 0);
        work->pockets[2] = FTBL.CreateEventCharacter(3, "NB_A", 0);
        work->pockets[3] = FTBL.CreateEventCharacter(3, "NB_A", 0);
        break;
    }
    for (i = 0; i < work->pocketCount; i++) {
        FTBL.AT_SetLocalBaseChara(work->pockets[i], work->board);
        FTBL.EV_AT_SetPos(work->pockets[i], TRANSFORM, &positions[i]);
        FTBL.EV_AT_SetAngY_0(work->pockets[i], TRANSFORM, &rotation, 0);
        FTBL.EV_EnableCharacterDisplay(work->pockets[i]);
    }
    if ((unsigned short)(work->mode - 60) < 4)
        for (i = 0; i < 8; i++) {
            FTBL.mtwkSetMotionworkScale3(work->pockets[i], &scale);
            FTBL.EV_AT_SetPos(work->pockets[i], TRANSFORM, &largePositions[i]);
        }
}
void DropBallCallback(void *data) {
    DropBallWork *work = (DropBallWork *)data;
    Vector3 origin = {0, 0, 0};
    switch (work->state) {
    case 0:
        FTBL.HLib_SetupTaskDelay(10);
        work->state = 3;
        break;
    case 1:
        ShowDropPlayChoice(work);
        break;
    case 2:
        ReadDropPlayChoice(work);
        break;
    case 3:
        FTBL.EV_EnableCharacterDisplay(work->balls[work->activeBall].character);
        HandleDropBallInput(work);
        break;
    case 5:
        AdvanceDropBall(work);
        break;
    case 8: {
        short choices[] = {-1, 37, 34, -1};
        FTBL.SetupControlSpeakAndUI(choices, 300, 65535, 0);
        work->state = 9;
        CreateDropBalls(work);
        break;
    }
    case 9:
        ReadDropQuitChoice(work);
        break;
    case 10:
        work->state = 3;
        work->activeBall = 0;
        *work->wins = 0;
        break;
    case 11:
        FreeDropBall(work);
        break;
    }
    PositionDropBall(work);
    if (work->balls[work->activeBall].velocity.x != 0.0f)
        return;
    if (work->balls[work->activeBall].velocity.y != 0.0f)
        return;
    if (++work->settledFrames < 60)
        return;
    if (work->state != 5)
        return;
    FTBL.EV_AT_SetPos(CAMERA_DUMMY, TRANSFORM, &origin);
    FTBL.EV_DisableCharacterDisplay(work->balls[work->activeBall].character);
    switch (work->mode) {
    case 50:
    case 51:
    case 52:
    case 53:
    case 54:
    case 55:
    case 56:
    case 57:
    default:
        FTBL.EVS_LoopCamera(960, CAMERA_DUMMY, CAMERA_DUMMY);
        break;
    case 58:
    case 59:
        FTBL.EVS_LoopCamera(965, CAMERA_DUMMY, CAMERA_DUMMY);
        break;
    case 60:
    case 61:
    case 62:
    case 63:
        FTBL.EVS_LoopCamera(966, CAMERA_DUMMY, CAMERA_DUMMY);
        break;
    }
    if (work->activeBall < 2) {
        work->activeBall++;
        work->state = 3;
    } else
        work->state = 11;
}

void ShowDropPlayChoice(DropBallWork *work) {
    short choices[] = {-1, 74, 75, -1};
    FTBL.SetupControlSpeakAndUI(choices, 300, 65535, 0);
    work->state = 2;
    FTBL.dbgPrintEventMessage("落し玉をしますか？");
}
void ReadDropPlayChoice(DropBallWork *work) {
    if (FTBL.shenGetCtrlSpeakIconFlag() == 74) {
        work->state = 3;
        FTBL.ctrlStopSubtitles();
    } else if (FTBL.shenGetCtrlSpeakIconFlag() == 75) {
        work->state = 11;
        FTBL.ctrlStopSubtitles();
    } else if (FTBL.GetShouldRenderIcons(2)) {
        work->state = 11;
        FTBL.ctrlStopSubtitles();
    }
}
void ReadDropQuitChoice(DropBallWork *work) {
    if (FTBL.shenGetCtrlSpeakIconFlag() == 34)
        work->state = 11;
    else if (FTBL.shenGetCtrlSpeakIconFlag() == 37)
        work->state = 10;
    else if (FTBL.GetShouldRenderIcons(2))
        work->state = 11;
}
void FreeDropBall(DropBallWork *work) {
    int i;
    for (i = 0; i < work->pocketCount; i++)
        FTBL.SetScenarioCharacterKillMode(work->pockets[i], 1);
    FTBL.EV_EnableCharacterDisplay(BALL_PLAYER);
    FTBL.ResetScenarioCharacterAttachment(CAMERA_DUMMY);
    if (FTBL.DoesScenarioCharacterExist(BALL_DUCK) == 1)
        FTBL.EV_EnableCharacterDisplay(BALL_DUCK);
    FTBL.HLib_DestroyCurrentEventTask();
    FTBL.HLib_ResolveDeferredCallback(work->parent);
}
void HandleDropBallInput(DropBallWork *work) {
    if (FTBL.GetCurrentControlInput(2) == 1024 && work->state == 3) {
        work->state = 5;
        work->settledFrames = 0;
        work->soundFrames = 0;
        work->balls[work->activeBall].position.y = work->balls[work->activeBall].previous.y = work->top;
        work->balls[work->activeBall].position.z = work->balls[work->activeBall].previous.z = .0219999999f;
        SetDropVector(&work->balls[work->activeBall].velocity, 0, 0, 0);
        CopyDropVector(&work->balls[work->activeBall].previousVelocity, &work->balls[work->activeBall].velocity);
        work->balls[work->activeBall].quietX = 0;
        work->balls[work->activeBall].quietY = 0;
        work->balls[work->activeBall].angle = 0;
        work->balls[work->activeBall].scored = 0;
    } else if (work->state == 3) {
        work->balls[work->activeBall].position.x = work->balls[work->activeBall].previous.x =
            work->right * FTBL.CalculateFixedCosine(work->balls[work->activeBall].angle);
        work->balls[work->activeBall].angle += 364;
    }
}
void AdvanceDropBall(DropBallWork *work) {
    IntegrateDropBall(work);
    BounceDropFloor(work);
    BounceDropWall(work);
    FindDropNails(work, 0, 0);
    FindDropDividers(work);
    SettleDropBall(work);
    ScoreDropBall(work);
    if (work->mode == 60)
        FTBL.EVS_LoopCamera(961, 0, 0);
    else if (work->mode == 61)
        FTBL.EVS_LoopCamera(962, 0, 0);
    else if (work->mode == 62)
        FTBL.EVS_LoopCamera(963, 0, 0);
    else if (work->mode == 63)
        FTBL.EVS_LoopCamera(964, 0, 0);
    else
        FollowDropBall(work);
}
void IntegrateDropBall(DropBallWork *work) {
    Vector3 position, velocity;
    float gravity = __builtin_fabs(FTBL.CalculateFixedCosine(work->boardRotation.x));
    CopyDropVector(&position, &work->balls[work->activeBall].previous);
    CopyDropVector(&velocity, &work->balls[work->activeBall].previousVelocity);
    if (work->balls[work->activeBall].velocity.x != 0.0f)
        position.x += velocity.x / 30.0f;
    if (work->balls[work->activeBall].velocity.y != 0.0f)
        position.y += velocity.y / 30.0f + (-.0653333366f * gravity) / 60.0f;
    velocity.y += gravity * -.0653333366f;
    CopyDropVector(&work->balls[work->activeBall].position, &position);
    CopyDropVector(&work->balls[work->activeBall].velocity, &velocity);
}
void BounceDropFloor(DropBallWork *work) {
    if (work->balls[work->activeBall].position.y <= work->bottom) {
        if (__builtin_fabs(work->balls[work->activeBall].velocity.y) > .0653333366f)
            PlayDropSound(work, 0);
        work->balls[work->activeBall].velocity.x = work->balls[work->activeBall].previousVelocity.x * .9599999785f;
        work->balls[work->activeBall].velocity.y = work->balls[work->activeBall].previousVelocity.y * -.5f;
        work->balls[work->activeBall].position.y = work->bottom + (work->bottom - work->balls[work->activeBall].position.y) * .5f;
    }
}
void BounceDropWall(DropBallWork *work) {
    if (work->balls[work->activeBall].position.x >= work->right) {
        PlayDropSound(work, 2);
        work->balls[work->activeBall].velocity.x = work->balls[work->activeBall].previousVelocity.x * -.5f;
        work->balls[work->activeBall].position.x = (work->right + work->right) - work->balls[work->activeBall].position.x;
    } else if (work->balls[work->activeBall].position.x <= work->left) {
        PlayDropSound(work, 2);
        work->balls[work->activeBall].velocity.x = work->balls[work->activeBall].previousVelocity.x * -.5f;
        work->balls[work->activeBall].position.x = (work->left + work->left) - work->balls[work->activeBall].position.x;
    }
}
void FindDropNails(DropBallWork *work, unsigned char mode, unsigned int previous) {
    Vector3 position, local;
    unsigned short nodes[] = {153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 0};
    unsigned short i, j;
    if (mode == 0)
        previous = work->pocketCount;
    for (i = 0; i < work->pocketCount; i++)
        for (j = 0; nodes[j]; j++) {
            FTBL.GetCharaPosition(work->pockets[i], -1, &position, 0);
            FTBL.scenApplyPosOrRotToChildId(work->pockets[i], nodes[j], &local, 0, 2);
            position.x += local.x;
            position.y += local.y;
            position.z += local.z;
            if (ProbeDropBall(work, position, mode) == 0 && previous != j) {
                PlayDropSound(work, 1);
                BounceDropNail(work, j, .35f, position, mode);
                FindDropNails(work, 1, j);
            }
        }
}
int ProbeDropBall(DropBallWork *work, Vector3 point, unsigned char mode) {
    Vector3 previous, current, movement;
    float previousDistance, currentDistance, projection, nearestDistance;
    switch (mode) {
    case 0:
        CopyDropVector(&previous, &work->balls[work->activeBall].previous);
        break;
    case 1:
        CopyDropVector(&previous, &work->balls[work->activeBall].segment);
        break;
    }
    CopyDropVector(&current, &work->balls[work->activeBall].position);
    SetDropVector(&movement, current.x - previous.x, current.y - previous.y, current.z - previous.z);
    previousDistance = (previous.x - point.x) * (previous.x - point.x) + (previous.y - point.y) * (previous.y - point.y);
    currentDistance = (current.x - point.x) * (current.x - point.x) + (current.y - point.y) * (current.y - point.y);
    projection = ((point.x - previous.x) * movement.x + (point.y - previous.y) * movement.y) / (movement.x * movement.x + movement.y * movement.y);
    nearestDistance = ((previous.x - point.x) + projection * movement.x) * ((previous.x - point.x) + projection * movement.x) +
        ((previous.y - point.y) + projection * movement.y) * ((previous.y - point.y) + projection * movement.y);
    if (previousDistance <= (work->scale * .0170000009f) * (work->scale * .0170000009f))
        return 2;
    if (currentDistance > (work->scale * .0170000009f) * (work->scale * .0170000009f)) {
        if (projection <= 0.0f)
            return 2;
        if (projection >= 1.0f)
            return 2;
        if (nearestDistance >= (work->scale * .0170000009f) * (work->scale * .0170000009f))
            goto rejected;
    }
    return 0;
rejected:
    return 2;
}
void BounceDropNail(DropBallWork *work, int nail, float restitution, Vector3 point, unsigned char mode) {
    Vector3 previous, current, contact, reflected, velocity, newVelocity;
    float plus, minus, verticalRoot;
    float slope, intercept, root, discriminant, centerX, centerY;
    switch (mode) {
    case 0:
        CopyDropVector(&previous, &work->balls[work->activeBall].previous);
        break;
    case 1:
        CopyDropVector(&previous, &work->balls[work->activeBall].segment);
        break;
    }
    CopyDropVector(&current, &work->balls[work->activeBall].position);
    CopyDropVector(&velocity, &work->balls[work->activeBall].velocity);
    centerX = point.x;
    centerY = point.y;
    if (__builtin_fabs(velocity.x) < 1.1920928955078125e-7f && __builtin_fabs(previous.x - centerX) < 1.1920928955078125e-7f)
        centerX += .00001f;
    if (__builtin_fabs(current.x - previous.x) < 1.1920928955078125e-7f) {
        slope = 0;
        intercept = current.x;
    } else {
        slope = (current.y - previous.y) / (current.x - previous.x);
        intercept = current.y - slope * current.x;
    }
    if (__builtin_fabs(current.x - previous.x) >= 1.1920928955078125e-7f) {
        root = (((slope * intercept - slope * centerY) - centerX) / (slope * slope + 1.0f)) * -1.0f;
        discriminant = root * root -
            (((intercept - centerY) * (intercept - centerY) + centerX * centerX) - (work->scale * .0170000009f) * (work->scale * .0170000009f)) /
                (slope * slope + 1.0f);
        if (__builtin_fabs(discriminant) < 1.1920928955078125e-7f) {
            if (__builtin_fabs(root) < 1.1920928955078125e-7f) {
                contact.x = centerX + .001f;
                {
                    float branchTemp;
                    branchTemp = sqrt((work->scale * .0170000009f) * (work->scale * .0170000009f) - (contact.x - centerX) * (contact.x - centerX));
                    contact.y = branchTemp + centerY;
                }
            } else {
                contact.x = root;
                {
                    float branchTemp;
                    branchTemp = sqrt((work->scale * .0170000009f) * (work->scale * .0170000009f) - (contact.x - centerX) * (contact.x - centerX));
                    contact.y = centerY + branchTemp;
                }
            }
        } else {
            discriminant = sqrt(discriminant);
            {
                plus = __builtin_fabs((root + discriminant) - previous.x);
                minus = __builtin_fabs((root - discriminant) - previous.x);
                if (plus <= minus)
                    contact.x = root + discriminant;
                else
                    contact.x = root - discriminant;
            }
            contact.y = slope * contact.x + intercept;
        }
    } else {
        contact.x = intercept;
        if (__builtin_fabs((work->scale * .0170000009f) * (work->scale * .0170000009f) - (contact.x - centerX) * (contact.x - centerX)) >=
            1.1920928955078125e-7f) {
            contact.y = previous.y;
        } else {
            verticalRoot = sqrt((work->scale * .0170000009f) * (work->scale * .0170000009f) -
                (contact.x - centerX) * (contact.x - centerX)); /* Keep the original SH4 square-root and delay-slot scheduling. */
            SHENMUE_EVENT_USE(verticalRoot);
            {
                plus = __builtin_fabs((centerY + verticalRoot) - previous.y);
                minus = __builtin_fabs((centerY - verticalRoot) - previous.y);
                if (plus <= minus)
                    contact.y = centerY + verticalRoot;
                else
                    contact.y = centerY - verticalRoot;
            }
        }
    }
    if (__builtin_fabs(contact.y - centerY) < 1.1920928955078125e-7f) {
        slope = 0;
        intercept = contact.y;
    } else {
        slope = ((contact.x - centerX) / (contact.y - centerY)) * -1.0f;
        intercept = contact.y - slope * contact.x;
    }
    if (__builtin_fabs(contact.y - centerY) >= 1.1920928955078125e-7f) {
        reflected.x = ((slope * slope - 1.0f) * -1.0f * current.x + (slope + slope) * (current.y - intercept)) / (slope * slope + 1.0f);
        reflected.y = (((slope * current.x + intercept) + (slope * current.x + intercept)) + (slope * slope - 1.0f) * current.y) / (slope * slope + 1.0f);
        reflected.z = current.z;
        if ((reflected.x - centerX) * (reflected.x - centerX) + (reflected.y - centerY) * (reflected.y - centerY) <=
            (work->scale * .0170000009f) * (work->scale * .0170000009f)) {
            reflected.x = contact.x;
            reflected.y = contact.y;
        }
        newVelocity.x = ((slope * slope - 1.0f) * -1.0f * velocity.x + (slope + slope) * velocity.y) / (slope * slope + 1.0f);
        newVelocity.y = ((slope + slope) * velocity.x + (slope * slope - 1.0f) * velocity.y) / (slope * slope + 1.0f);
        newVelocity.z = velocity.z;
        if (__builtin_fabs(newVelocity.y - velocity.y) < 1.1920928955078125e-7f)
            newVelocity.y = -newVelocity.y;
    } else {
        reflected.x = contact.x;
        reflected.y = (centerX + centerX) - contact.y;
        reflected.z = current.z;
        newVelocity.x = velocity.x;
        newVelocity.y = velocity.y * -1.0f;
        newVelocity.z = velocity.z;
    }
    FTBL.EV_AT_SetPos(work->balls[work->activeBall].character, 0x38000000, &contact);
    FTBL.HLib_SetupTaskDelay(1);
    CopyDropVector(&work->balls[work->activeBall].segment, &contact);
    CopyDropVector(&work->balls[work->activeBall].position, &reflected);
    SetDropVector(&work->balls[work->activeBall].velocity, restitution * newVelocity.x, restitution * newVelocity.y, restitution * newVelocity.z);
}
void FindDropDividers(DropBallWork *work) {
    Vector3 points[] = {{-.13f, .08f, 0}, {-.045f, .08f, 0}, {.04f, .08f, 0}, {.125f, .08f, 0}};
    unsigned short i;
    if (work->balls[work->activeBall].position.y < work->scale * .0170000009f + .08f)
        for (i = 0; i < 4; i++)
            CheckDropDivider(work, points[i]);
}
void CheckDropDivider(DropBallWork *work, Vector3 point) {
    Vector3 previous, current;
    struct Rectangle {
        Vector3 points[4];
    } corners = {
        {{point.x, point.y, point.z}, {point.x + .005f, point.y, point.z}, {point.x, point.y - .075f, point.z}, {point.x + .005f, point.y - .075f, point.z}}};
    Vector3 edges[6];
    float radius;
    short i;
    CopyDropVector(&previous, &work->balls[work->activeBall].previous);
    CopyDropVector(&current, &work->balls[work->activeBall].position);
    edges[0].x = corners.points[0].x;
    edges[0].y = corners.points[0].y + (work->scale * .0170000009f);
    edges[0].z = corners.points[0].z;
    edges[1].x = corners.points[1].x;
    edges[1].y = corners.points[1].y + (work->scale * .0170000009f);
    edges[1].z = corners.points[1].z;
    edges[2].x = corners.points[0].x - (work->scale * .0170000009f);
    edges[2].y = corners.points[0].y;
    edges[2].z = corners.points[0].z;
    edges[3].x = corners.points[2].x - (work->scale * .0170000009f);
    edges[3].y = corners.points[2].y;
    edges[3].z = corners.points[2].z;
    edges[4].x = corners.points[1].x + (work->scale * .0170000009f);
    edges[4].y = corners.points[1].y;
    edges[4].z = corners.points[1].z;
    edges[5].x = corners.points[3].x + (work->scale * .0170000009f);
    edges[5].y = corners.points[3].y;
    edges[5].z = corners.points[3].z;
    for (i = 0; i < 6; i += 2) {
        if (ProbeDropLine(current, previous, edges[i], edges[i + 1]) == 1) {
            PlayDropSound(work, 2);
            BounceDropLine(work, edges[i], edges[i + 1]);
            break;
        }
    }
    if (current.x <= corners.points[0].x) {
        for (i = 0; i < 2; i++)
            if (ProbeDropBall(work, corners.points[i], 0) == 0) {
                PlayDropSound(work, 2);
                BounceDropNail(work, (unsigned short)i, .5f, corners.points[i], 0);
                return;
            }
    } else {
        for (i = 1; i >= 0; i--)
            if (ProbeDropBall(work, corners.points[i], 0) == 0) {
                PlayDropSound(work, 2);
                BounceDropNail(work, (unsigned short)i, .5f, corners.points[i], 0);
                return;
            }
    }
}
int ProbeDropLine(Vector3 first, Vector3 second, Vector3 third, Vector3 fourth) {
    float cross[4];
    cross[0] = (third.x - first.x) * (second.y - third.y) - (third.y - first.y) * (second.x - third.x);
    cross[1] = (second.x - third.x) * (fourth.y - second.y) - (second.y - third.y) * (fourth.x - second.x);
    cross[2] = (fourth.x - second.x) * (first.y - fourth.y) - (fourth.y - second.y) * (first.x - fourth.x);
    cross[3] = (first.x - fourth.x) * (third.y - first.y) - (first.y - fourth.y) * (third.x - first.x);
    if (cross[0] >= 0) {
        if (cross[1] < 0)
            goto rejected;
        if (cross[2] < 0)
            goto rejected;
        if (cross[3] < 0)
            return 2;
    } else {
        if (cross[1] >= 0)
            goto rejected;
        if (cross[2] >= 0)
            goto rejected;
        if (cross[3] >= 0)
            return 2;
    }
    return 1;
rejected:
    return 2;
}
void BounceDropLine(DropBallWork *work, Vector3 first, Vector3 second) {
    Vector3 previous, current, contact, velocity, oldVelocity;
    float slope, intercept;
    CopyDropVector(&previous, &work->balls[work->activeBall].previous);
    CopyDropVector(&current, &work->balls[work->activeBall].position);
    CopyDropVector(&oldVelocity, &work->balls[work->activeBall].previousVelocity);
    CopyDropVector(&velocity, &work->balls[work->activeBall].velocity);
    slope = (current.y - previous.y) / (current.x - previous.x);
    intercept = current.y - slope * current.x;
    if (first.y == second.y) {
        contact.x = (first.y - intercept) / slope;
        contact.y = first.y;
        contact.z = 0;
        current.y = first.y + (first.y - current.y) * .5f;
        velocity.y *= -.5f;
    } else if (first.x == second.x) {
        contact.x = first.x;
        contact.y = slope * first.x + intercept;
        contact.z = 0;
        current.x = (first.x + first.x) - current.x;
        velocity.x *= -.9599999785f;
    }
    CopyDropVector(&work->balls[work->activeBall].position, &current);
    CopyDropVector(&work->balls[work->activeBall].velocity, &velocity);
}
void SettleDropBall(DropBallWork *work) {
    Vector3 previous, current, velocity, oldVelocity;
    float dx, dy;
    unsigned int quietX, quietY;
    CopyDropVector(&previous, &work->balls[work->activeBall].previous);
    CopyDropVector(&current, &work->balls[work->activeBall].position);
    CopyDropVector(&oldVelocity, &work->balls[work->activeBall].previousVelocity);
    CopyDropVector(&velocity, &work->balls[work->activeBall].velocity);
    velocity.y = work->balls[work->activeBall].velocity.y;
    velocity.z = work->balls[work->activeBall].velocity.z;
    quietX = work->balls[work->activeBall].quietX;
    quietY = work->balls[work->activeBall].quietY;
    dx = __builtin_fabs(current.x - previous.x) < 1.1920928955078125e-7f ? 0 : __builtin_fabs(current.x - previous.x);
    dy = __builtin_fabs(current.y - previous.y) < 1.1920928955078125e-7f ? 0 : __builtin_fabs(current.y - previous.y);
    if (quietY && velocity.y != 0)
        velocity.y *= .5f;
    if (dy < .0005f && quietY && current.y <= work->bottom + .005f) {
        PlayDropSound(work, 3);
        current.y = previous.y = work->bottom;
        current.z = previous.z = .022f;
        velocity.y = oldVelocity.y = 0;
        velocity.z = oldVelocity.z = 0;
        if (dx < .0001f && quietX == 1)
            velocity.x = oldVelocity.x = 0;
    }
    if (dx < .0001f)
        quietX = 1;
    else
        quietX = 0;
    if (dy < .0005f)
        quietY = 1;
    else
        quietY = 0;
    CopyDropVector(&work->balls[work->activeBall].previous, &previous);
    CopyDropVector(&work->balls[work->activeBall].position, &current);
    CopyDropVector(&work->balls[work->activeBall].previousVelocity, &oldVelocity);
    CopyDropVector(&work->balls[work->activeBall].velocity, &velocity);
    work->balls[work->activeBall].quietX = quietX;
    work->balls[work->activeBall].quietY = quietY;
}
void ScoreDropBall(DropBallWork *work) {
    Vector3 current;
    float low = work->balls[work->activeBall].lowHeight;
    int scored = work->balls[work->activeBall].scored;
    CopyDropVector(&current, &work->balls[work->activeBall].position);
    if (low < .08000000566f && current.y < .08000000566f && scored == 0) {
        if (current.x >= -.13f) {
            if (current.x < -.045f)
                (*work->wins)++;
            else if (current.x >= .04f && current.x < .125f)
                (*work->wins)++;
        }
        scored = 1;
    }
    work->balls[work->activeBall].scored = scored;
}
void PositionDropBall(DropBallWork *work) {
    Vector3 rotated, position;
    float sine, cosine;
    sine = FTBL.CalculateFixedSine(work->boardRotation.x);
    cosine = FTBL.CalculateFixedCosine(work->boardRotation.x);
    rotated.x = work->balls[work->activeBall].position.x;
    rotated.y = cosine * work->balls[work->activeBall].position.y - sine * work->balls[work->activeBall].position.z;
    rotated.z = cosine * work->balls[work->activeBall].position.z + sine * work->balls[work->activeBall].position.y;
    sine = FTBL.CalculateFixedSine(work->boardRotation.y);
    cosine = FTBL.CalculateFixedCosine(work->boardRotation.y);
    position.x = cosine * rotated.x + sine * rotated.z;
    position.z = cosine * rotated.z - sine * rotated.x;
    position.x += work->boardPosition.x;
    position.y = rotated.y + work->boardPosition.y;
    position.z += work->boardPosition.z;
    FTBL.EV_AT_SetPos(work->balls[work->activeBall].character, TRANSFORM, &position);
    if (work->balls[work->activeBall].previousVelocity.y < 0 && work->balls[work->activeBall].velocity.y >= 0) {
        if (__builtin_fabs(work->balls[work->activeBall].highHeight - work->balls[work->activeBall].position.y) < 1.1920928955078125e-7f)
            work->balls[work->activeBall].quietY++;
        work->balls[work->activeBall].highHeight = work->balls[work->activeBall].position.y;
    } else if (work->balls[work->activeBall].previousVelocity.y >= 0 && work->balls[work->activeBall].velocity.y < 0) {
        if (__builtin_fabs(work->balls[work->activeBall].lowHeight - work->balls[work->activeBall].position.y) < 1.1920928955078125e-7f)
            work->balls[work->activeBall].quietY++;
        work->balls[work->activeBall].lowHeight = work->balls[work->activeBall].position.y;
    }
    CopyDropVector(&work->balls[work->activeBall].previous, &work->balls[work->activeBall].position);
    CopyDropVector(&work->balls[work->activeBall].previousVelocity, &work->balls[work->activeBall].velocity);
}
void PlayDropSound(DropBallWork *work, short material) {
    Vector3 origin, velocity;
    int sound;
    char period;
    float quiet, loud;
    SetDropVector(&origin, 0, 0, 0);
    CopyDropVector(&velocity, &work->balls[work->activeBall].velocity);
    switch (material) {
    case 0:
        quiet = 0;
        sound = 283;
        loud = .005f;
        period = 5;
        break;
    case 1:
        quiet = 0;
        sound = 289;
        loud = .01f;
        period = 2;
        break;
    case 2:
        sound = 293;
        loud = .008f;
        quiet = .005f;
        period = 5;
        break;
    default:
        sound = 295;
        quiet = loud = 0;
        period = 60;
        break;
    }
    if (work->sound != sound)
        work->soundFrames = 0;
    if (work->soundFrames <= period) {
        if (__builtin_fabs(FTBL.GetSquaredDistanceXZ(&origin, &velocity)) > loud)
            FTBL.Loop3DSoundEffect(sound);
        else if (__builtin_fabs(FTBL.GetSquaredDistanceXZ(&origin, &velocity)) > quiet)
            FTBL.Loop3DSoundEffect(sound + 1);
        else
            FTBL.Loop3DSoundEffect(sound + 2);
        work->soundFrames++;
        work->sound = sound;
    } else
        work->soundFrames = 0;
}
void FollowDropBall(DropBallWork *work) {
    Vector3 target;
    Vector3i rotation = {5461, 0, 0};
    Vector3 ball, current;
    float *selected;
    if (work->balls[work->activeBall].position.y >= work->top - .13f)
        SetDropVector(&target, 0, work->top - .13f, 0);
    else if (work->balls[work->activeBall].position.y > .14f && work->balls[work->activeBall].position.y < work->top - .13f) {
        ball.y = work->balls[work->activeBall].position.y;
        FTBL.GetCharaPosition(CAMERA_DUMMY, -1, &current, 0);
        if (ball.y > current.y)
            SetDropVector(&target, 0, current.y, current.z);
        else
            SetDropVector(&target, 0, ball.y, ball.z);
    } else
        SetDropVector(&target, 0, .14f, 0);
    FTBL.EV_AT_SetPos(CAMERA_DUMMY, TRANSFORM, &target);
    FTBL.EV_AT_SetAngY_0(CAMERA_DUMMY, TRANSFORM, &rotation, 0);
    if ((unsigned short)(work->mode - 60) < 4)
        FTBL.EVS_LoopCamera(941, CAMERA_DUMMY, CAMERA_DUMMY);
    else
        FTBL.EVS_LoopCamera(940, CAMERA_DUMMY, CAMERA_DUMMY);
}

void CopyDropVector(Vector3 *target, const Vector3 *source) {
    target->x = source->x;
    target->y = source->y;
    target->z = source->z;
}
void SetDropVector(Vector3 *target, float x, float y, float z) {
    target->x = x;
    target->y = y;
    target->z = z;
}

#endif

#endif
