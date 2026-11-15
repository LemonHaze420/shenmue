#if defined(SHENMUE_EVENT_OVERLAY) && SHENMUE_TARGET_S2_DC_JP_GAMEJAM
#include "s2/areas/area_smbl_dc.hpp"
double sqrt(double value);
#define NAME(a, b, c, d) SHEN_NAME4(a, b, c, d)
#define CAMERA_DUMMY NAME('C', 'M', 'D', 'M')
#define BALL_PLAYER NAME('R', 'Y', 'O', '_')
#define BALL_DUCK NAME('R', 'U', 'C', 'K')
#define TRANSFORM 0x38000000

int PollSmallBall(DropBallWork *work) {
    unsigned char result = 0;
    switch (work->pollState) {
    case 0:
        InitDropBall(work, FTBL.HLib_GetCurrentTask());
        work->pollState = 1;
        break;
    case 1:
        DropBallCallback(work);
        break;
    case 2:
        result = 1;
        work->pollState = 0;
        break;
    }
    return result;
}
void InitDropBall(DropBallWork *work, HLibTask *task) {
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EVS_LoopCamera(920, work->board, work->board);
    work->task = task;
    work->nailCount = 0;
    work->activeBall = 0;
    work->state = 0;
    work->settled = 1;
    work->boardType = 0;
    work->wins = 0;
    FTBL.GetCharaPosition(work->board, -1, &work->boardPosition, 0);
    FTBL.GetScenarioCharacterEulerAngles(work->board, &work->boardRotation, 0);
    CreateDropBalls(work);
    CreateSmallNails(work);
}
void CreateDropBalls(DropBallWork *work) {
    Vector3 origin = {0, 0, 0}, scale = {.87f, .87f, .87f};
    shen_name names[] = {NAME('B', 'L', '_', '0'), NAME('B', 'L', '_', '1'), NAME('B', 'L', '_', '2'), 0};
    unsigned char i;
    FTBL.EV_DisableCharacterDisplay(BALL_PLAYER);
    FTBL.EV_DisableCharacterDisplay(NAME('B', 'L', '_', '1'));
    FTBL.EV_DisableCharacterDisplay(NAME('B', 'L', '_', '2'));
    FTBL.EV_DisableCharacterDisplay(work->dealer);
    if (work->hasDuck == 1)
        FTBL.EV_DisableCharacterDisplay(BALL_DUCK);
    FTBL.AT_SetLocalBaseChara(CAMERA_DUMMY, work->board);
    FTBL.EV_AT_SetPos(CAMERA_DUMMY, TRANSFORM, &origin);
    for (i = 0; names[i]; i++) {
        FTBL.mtwkSetMotionworkScale3(names[i], &scale);
        work->balls[i].character = names[i];
        SetDropVector(&work->balls[i].position, .177999988198f, .676999986172f, .0340000018477f);
        SetDropVector(&work->balls[i].velocity, 0, 0, 0);
        CopyDropVector(&work->balls[i].previous, &work->balls[i].position);
        CopyDropVector(&work->balls[i].previousVelocity, &work->balls[i].velocity);
        work->balls[i].quietX = 0;
        work->balls[i].quietY = 0;
        work->balls[i].angle = 0;
    }
}
void CreateSmallNails(DropBallWork *work) {
    Vector3i rotation = {16384, 0, 0};
    Vector3 position;
    int i;
    GetSmallNailPoint(work, &position, work->boardType, 1);
    for (i = 0; i < work->nailCount; i++) {
        work->pockets[i] = FTBL.CreateEventCharacter(3, "NAIL", 0);
        FTBL.AT_SetLocalBaseChara(work->pockets[i], work->board);
        GetSmallNailPoint(work, &position, work->boardType, i);
        FTBL.EV_AT_SetPos(work->pockets[i], TRANSFORM, &position);
        FTBL.EV_AT_SetAngY_0(work->pockets[i], TRANSFORM, &rotation, 0);
        FTBL.EV_EnableCharacterDisplay(work->pockets[i]);
    }
    work->pockets[i] = 0;
}
void DropBallCallback(DropBallWork *work) {
    switch (work->state) {
    case 0:
        FTBL.HLib_SetupTaskDelay(10);
        work->state = 1;
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
    case 8:
        if (work->field1c > work->field20) {
            short choices[] = {-1, 37, 34, -1};
            FTBL.SetupControlSpeakAndUI(choices, 300, 65535, 0);
        }
        work->state = 9;
        CreateDropBalls(work);
        work->activeBall = 0;
        work->wins = 0;
        break;
    case 9:
        ReadDropQuitChoice(work);
        break;
    case 10:
        FreeDropBall(work);
        break;
    }
    PositionDropBall(work);
    if (work->balls[work->activeBall].velocity.x != 0)
        return;
    if (work->balls[work->activeBall].velocity.y != 0)
        return;
    if (++work->settledFrames < 60)
        return;
    if (work->state != 5)
        return;
    FTBL.EV_DisableCharacterDisplay(work->balls[work->activeBall].character);
    FTBL.EVS_LoopCamera(920, work->board, work->board);
    if (work->activeBall < 2) {
        work->activeBall++;
        work->state = 3;
    } else
        work->state = 8;
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
        work->state = 10;
        FTBL.ctrlStopSubtitles();
    } else if (FTBL.GetShouldRenderIcons(2)) {
        work->state = 10;
        FTBL.ctrlStopSubtitles();
    }
}
void ReadDropQuitChoice(DropBallWork *work) {
    if (FTBL.shenGetCtrlSpeakIconFlag() == 34)
        work->state = 10;
    else if (FTBL.shenGetCtrlSpeakIconFlag() == 37)
        work->state = 3;
    else if (FTBL.GetShouldRenderIcons(2))
        work->state = 3;
}
void FreeDropBall(DropBallWork *work) {
    int i;
    for (i = 0; work->pockets[i]; i++)
        FTBL.SetScenarioCharacterKillMode(work->pockets[i], 1);
    FTBL.EV_EnableCharacterDisplay(BALL_PLAYER);
    FTBL.EV_EnableCharacterDisplay(work->dealer);
    FTBL.ResetScenarioCharacterAttachment(CAMERA_DUMMY);
    if (work->hasDuck == 1)
        FTBL.EV_EnableCharacterDisplay(BALL_DUCK);
    FTBL.EVS_LoopCamera(0, 0, 0);
    FTBL.camCtrlSetFovTransitionTo(1);
    FTBL.camCtrlSetState(0);
    work->state = 11;
    work->pollState = 2;
    FTBL.SetPlayerControlEnabled(1);
}
void HandleDropBallInput(DropBallWork *work) {
    if (FTBL.GetCurrentControlInput(2) == 1024 && work->state == 3) {
        work->state = 5;
        work->settled = 0;
        work->settledFrames = 0;
        work->balls[work->activeBall].position.y = work->balls[work->activeBall].previous.y = .676999986172f;
        work->balls[work->activeBall].position.z = work->balls[work->activeBall].previous.z = .0340000018477f;
        SetDropVector(&work->balls[work->activeBall].velocity, 0, 0, 0);
        CopyDropVector(&work->balls[work->activeBall].previousVelocity, &work->balls[work->activeBall].velocity);
        work->balls[work->activeBall].quietX = 0;
        work->balls[work->activeBall].quietY = 0;
        work->balls[work->activeBall].angle = 0;
        work->balls[work->activeBall].scored = 0;
    } else if (work->state == 3) {
        work->balls[work->activeBall].position.x = work->balls[work->activeBall].previous.x =
            .177999988198f * FTBL.CalculateFixedCosine(work->balls[work->activeBall].angle);
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
    FollowDropBall(work);
}
void IntegrateDropBall(DropBallWork *work) {
    Vector3 position, velocity;
    CopyDropVector(&position, &work->balls[work->activeBall].previous);
    CopyDropVector(&velocity, &work->balls[work->activeBall].previousVelocity);
    if (work->balls[work->activeBall].velocity.x != 0)
        position.x += velocity.x / 30.0f;
    if (work->balls[work->activeBall].velocity.y != 0)
        position.y += velocity.y / 30.0f + -.000680555589497f;
    velocity.y += -.0408333353698f;
    CopyDropVector(&work->balls[work->activeBall].position, &position);
    CopyDropVector(&work->balls[work->activeBall].velocity, &velocity);
}
void BounceDropFloor(DropBallWork *work) {
    if (work->balls[work->activeBall].position.y <= .0210000015795f) {
        work->balls[work->activeBall].velocity.x = work->balls[work->activeBall].previousVelocity.x * .9599999785f;
        work->balls[work->activeBall].velocity.y = work->balls[work->activeBall].previousVelocity.y * -.5f;
        work->balls[work->activeBall].position.y = .0210000015795f + (.0210000015795f - work->balls[work->activeBall].position.y) * .5f;
    }
}
void BounceDropWall(DropBallWork *work) {
    if (work->balls[work->activeBall].position.x >= .177999988198f) {
        work->balls[work->activeBall].velocity.x = work->balls[work->activeBall].previousVelocity.x * -.5f;
        work->balls[work->activeBall].position.x = (.177999988198f + .177999988198f) - work->balls[work->activeBall].position.x;
    } else if (work->balls[work->activeBall].position.x <= -.177999988198f) {
        work->balls[work->activeBall].velocity.x = work->balls[work->activeBall].previousVelocity.x * -.5f;
        work->balls[work->activeBall].position.x = (-.177999988198f + -.177999988198f) - work->balls[work->activeBall].position.x;
    }
}
void FindDropNails(DropBallWork *work, unsigned char mode, unsigned int previous) {
    Vector3 point;
    unsigned short i;
    if (mode == 0)
        previous = work->nailCount;
    GetSmallNailPoint(work, &point, work->boardType, 1);
    for (i = 0; i < work->nailCount; i++) {
        GetSmallNailPoint(work, &point, work->boardType, i);
        if (ProbeDropBall(work, point, mode) == 0 && previous != i) {
            FTBL.Loop3DSoundEffect(0x1c05a9);
            BounceDropNail(work, i, .35f, point, mode);
            FindDropNails(work, 1, i);
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
    if (previousDistance <= .000289000017801f)
        return 2;
    if (currentDistance > .000289000017801f) {
        if (projection <= 0)
            goto rejected;
        if (projection >= 1)
            return 2;
        if (nearestDistance >= .000289000017801f)
            return 2;
    }
    return 0;
rejected:
    return 2;
}
void BounceDropNail(DropBallWork *work, int nail, float restitution, Vector3 point, unsigned char mode) {
    Vector3 previous, current, contact, reflected, velocity, newVelocity;
    /* Preserve GCC's original allocation of the two root distances. */
    register float plusDistance __asm__("fr6");
    register float minusDistance __asm__("fr5");
    float verticalRoot;
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
        discriminant = root * root - (((intercept - centerY) * (intercept - centerY) + centerX * centerX) - .000289000017801f) / (slope * slope + 1.0f);
        if (__builtin_fabs(discriminant) < 1.1920928955078125e-7f) {
            if (__builtin_fabs(root) < 1.1920928955078125e-7f) {
                contact.x = centerX + .001f;
                {
                    float branchTemp;
                    branchTemp = sqrt(.000289000017801f - (contact.x - centerX) * (contact.x - centerX));
                    contact.y = branchTemp + centerY;
                }
            } else {
                contact.x = root;
                {
                    float branchTemp;
                    branchTemp = sqrt(.000289000017801f - (contact.x - centerX) * (contact.x - centerX));
                    contact.y = centerY + branchTemp;
                }
            }
        } else {
            discriminant = sqrt(discriminant);
            {
                plusDistance = __builtin_fabs((root + discriminant) - previous.x);
                minusDistance = __builtin_fabs((root - discriminant) - previous.x);
                SHENMUE_EVENT_USE(plusDistance);
                if (plusDistance <= minusDistance)
                    contact.x = root + discriminant;
                else
                    contact.x = root - discriminant;
            }
            contact.y = slope * contact.x + intercept;
        }
    } else {
        contact.x = intercept;
        if (__builtin_fabs(.000289000017801f - (contact.x - centerX) * (contact.x - centerX)) >= 1.1920928955078125e-7f) {
            contact.y = previous.y;
        } else {
            verticalRoot = sqrt(.000289000017801f - (contact.x - centerX) * (contact.x - centerX));
            {
                plusDistance = __builtin_fabs((centerY + verticalRoot) - previous.y);
                minusDistance = __builtin_fabs((centerY - verticalRoot) - previous.y);
                SHENMUE_EVENT_USE(plusDistance);
                if (plusDistance <= minusDistance)
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
        if ((reflected.x - centerX) * (reflected.x - centerX) + (reflected.y - centerY) * (reflected.y - centerY) <= .000289000017801f) {
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
    Vector3 points[] = {{-.121f, .08f, 0}, {-.041f, .08f, 0}, {.037f, .08f, 0}, {.116f, .08f, 0}};
    unsigned short i;
    if (work->balls[work->activeBall].position.y < .097000002861f)
        for (i = 0; i < 4; i++)
            CheckDropDivider(work, points[i]);
}
void CheckDropDivider(DropBallWork *work, Vector3 point) {
    Vector3 previous, current;
    struct Rectangle {
        Vector3 points[4];
    } corners = {
        {{point.x, point.y, point.z}, {point.x + .004f, point.y, point.z}, {point.x, point.y - .075f, point.z}, {point.x + .004f, point.y - .075f, point.z}}};
    Vector3 edges[6];
    float radius;
    short i;
    CopyDropVector(&previous, &work->balls[work->activeBall].previous);
    CopyDropVector(&current, &work->balls[work->activeBall].position);
    edges[0].x = corners.points[0].x;
    edges[0].y = corners.points[0].y + (.0170000009f);
    edges[0].z = corners.points[0].z;
    edges[1].x = corners.points[1].x;
    edges[1].y = corners.points[1].y + (.0170000009f);
    edges[1].z = corners.points[1].z;
    edges[2].x = corners.points[0].x - (.0170000009f);
    edges[2].y = corners.points[0].y;
    edges[2].z = corners.points[0].z;
    edges[3].x = corners.points[2].x - (.0170000009f);
    edges[3].y = corners.points[2].y;
    edges[3].z = corners.points[2].z;
    edges[4].x = corners.points[1].x + (.0170000009f);
    edges[4].y = corners.points[1].y;
    edges[4].z = corners.points[1].z;
    edges[5].x = corners.points[3].x + (.0170000009f);
    edges[5].y = corners.points[3].y;
    edges[5].z = corners.points[3].z;
    for (i = 0; i < 6; i += 2) {
        if (ProbeDropLine(current, previous, edges[i], edges[i + 1]) == 1) {
            FTBL.Loop3DSoundEffect(0x1d05a9);
            BounceDropLine(work, edges[i], edges[i + 1]);
            break;
        }
    }
    if (current.x <= corners.points[0].x) {
        for (i = 0; i < 2; i++)
            if (ProbeDropBall(work, corners.points[i], 0) == 0) {
                FTBL.Loop3DSoundEffect(0x1d05a9);
                BounceDropNail(work, (unsigned short)i, .5f, corners.points[i], 0);
                return;
            }
    } else {
        for (i = 1; i >= 0; i--)
            if (ProbeDropBall(work, corners.points[i], 0) == 0) {
                FTBL.Loop3DSoundEffect(0x1d05a9);
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
    if (dy < .0005f && quietY && current.y <= .0260000005364f) {
        current.y = previous.y = .0210000015795f;
        current.z = previous.z = .0340000018477f;
        velocity.y = oldVelocity.y = 0;
        velocity.z = oldVelocity.z = 0;
        if (dx < .0001f && quietX == 1) {
            velocity.x = oldVelocity.x = 0;
            work->settled = 1;
        }
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
    if (low < .0790000036359f && current.y < .0790000036359f && scored == 0) {
        if (current.x < -.121f) {
            QueueEventMessage("ハズレ", 0, 150);
        } else if (current.x < -.041f) {
            QueueEventMessage("当り", 0, 150);
            work->wins++;
        } else if (current.x < .037f) {
            QueueEventMessage("ハズレ", 0, 150);
        } else if (current.x < .116f) {
            QueueEventMessage("当り", 0, 150);
            work->wins++;
        } else
            QueueEventMessage("ハズレ", 0, 150);
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
void FollowDropBall(DropBallWork *work) {
    Vector3 target;
    Vector3i rotation;
    Vector3 ball, current;
    if (work->balls[work->activeBall].position.y >= .55f)
        SetDropVector(&target, 0, .55f, 0);
    else if (work->balls[work->activeBall].position.y > .24f && work->balls[work->activeBall].position.y < .55f) {
        ball.y = work->balls[work->activeBall].position.y;
        FTBL.GetCharaPosition(CAMERA_DUMMY, -1, &current, 0);
        if (ball.y > current.y)
            SetDropVector(&target, 0, current.y, 0);
        else
            SetDropVector(&target, 0, ball.y, 0);
    } else
        SetDropVector(&target, 0, .24f, 0);
    rotation.x = 0;
    rotation.y = 0;
    rotation.z = 0;
    FTBL.EV_AT_SetPos(CAMERA_DUMMY, TRANSFORM, &target);
    FTBL.EV_AT_SetAngY_0(CAMERA_DUMMY, TRANSFORM, &rotation, 0);
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
void GetSmallNailPoint(DropBallWork *work, Vector3 *point, int board, unsigned int index) {
    if (board == 0) {
        work->nailCount = 79;
        switch (index) {
        case 0:
            point->x = 0;
            point->y = 0.600000024f;
            point->z = 0;
            break;
        case 1:
            point->x = -0.180000007f;
            point->y = 0.600000024f;
            point->z = 0;
            break;
        case 2:
            point->x = -0.0900000036f;
            point->y = 0.600000024f;
            point->z = 0;
            break;
        case 3:
            point->x = -0.00999999978f;
            point->y = 0.589999974f;
            point->z = 0;
            break;
        case 4:
            point->x = 0.00999999978f;
            point->y = 0.589999974f;
            point->z = 0;
            break;
        case 5:
            point->x = 0.0900000036f;
            point->y = 0.600000024f;
            point->z = 0;
            break;
        case 6:
            point->x = 0.180000007f;
            point->y = 0.600000024f;
            point->z = 0;
            break;
        case 7:
            point->x = -0.170000002f;
            point->y = 0.589999974f;
            point->z = 0;
            break;
        case 8:
            point->x = -0.100000001f;
            point->y = 0.589999974f;
            point->z = 0;
            break;
        case 9:
            point->x = -0.0799999982f;
            point->y = 0.589999974f;
            point->z = 0;
            break;
        case 10:
            point->x = -0.150000006f;
            point->y = 0.370000005f;
            point->z = 0;
            break;
        case 11:
            point->x = 0.150000006f;
            point->y = 0.370000005f;
            point->z = 0;
            break;
        case 12:
            point->x = 0.0799999982f;
            point->y = 0.589999974f;
            point->z = 0;
            break;
        case 13:
            point->x = 0.100000001f;
            point->y = 0.589999974f;
            point->z = 0;
            break;
        case 14:
            point->x = 0.170000002f;
            point->y = 0.589999974f;
            point->z = 0;
            break;
        case 15:
            point->x = -0.159999996f;
            point->y = 0.579999983f;
            point->z = 0;
            break;
        case 16:
            point->x = -0.109999999f;
            point->y = 0.560000002f;
            point->z = 0;
            break;
        case 17:
            point->x = -0.0700000003f;
            point->y = 0.560000002f;
            point->z = 0;
            break;
        case 18:
            point->x = -0.0199999996f;
            point->y = 0.560000002f;
            point->z = 0;
            break;
        case 19:
            point->x = 0.0199999996f;
            point->y = 0.560000002f;
            point->z = 0;
            break;
        case 20:
            point->x = 0.0700000003f;
            point->y = 0.560000002f;
            point->z = 0;
            break;
        case 21:
            point->x = 0.109999999f;
            point->y = 0.560000002f;
            point->z = 0;
            break;
        case 22:
            point->x = 0.159999996f;
            point->y = 0.579999983f;
            point->z = 0;
            break;
        case 23:
            point->x = -0.159999996f;
            point->y = 0.560000002f;
            point->z = 0;
            break;
        case 24:
            point->x = 0.159999996f;
            point->y = 0.560000002f;
            point->z = 0;
            break;
        case 25:
            point->x = -0.150000006f;
            point->y = 0.529999971f;
            point->z = 0;
            break;
        case 26:
            point->x = -0.0500000007f;
            point->y = 0.529999971f;
            point->z = 0;
            break;
        case 27:
            point->x = 0.0500000007f;
            point->y = 0.529999971f;
            point->z = 0;
            break;
        case 28:
            point->x = 0.150000006f;
            point->y = 0.529999971f;
            point->z = 0;
            break;
        case 29:
            point->x = 0;
            point->y = 0.5f;
            point->z = 0;
            break;
        case 30:
            point->x = -0.00999999978f;
            point->y = 0.49000001f;
            point->z = 0;
            break;
        case 31:
            point->x = 0.00999999978f;
            point->y = 0.49000001f;
            point->z = 0;
            break;
        case 32:
            point->x = -0.0299999993f;
            point->y = 0.460000008f;
            point->z = 0;
            break;
        case 33:
            point->x = 0.0299999993f;
            point->y = 0.460000008f;
            point->z = 0;
            break;
        case 34:
            point->x = -0.119999997f;
            point->y = 0.449999988f;
            point->z = 0;
            break;
        case 35:
            point->x = 0.119999997f;
            point->y = 0.449999988f;
            point->z = 0;
            break;
        case 36:
            point->x = -0.0700000003f;
            point->y = 0.430000007f;
            point->z = 0;
            break;
        case 37:
            point->x = 0.0700000003f;
            point->y = 0.430000007f;
            point->z = 0;
            break;
        case 38:
            point->x = 0;
            point->y = 0.419999987f;
            point->z = 0;
            break;
        case 39:
            point->x = -0.180000007f;
            point->y = 0.400000006f;
            point->z = 0;
            break;
        case 40:
            point->x = -0.0399999991f;
            point->y = 0.400000006f;
            point->z = 0;
            break;
        case 41:
            point->x = 0.0399999991f;
            point->y = 0.400000006f;
            point->z = 0;
            break;
        case 42:
            point->x = 0.180000007f;
            point->y = 0.400000006f;
            point->z = 0;
            break;
        case 43:
            point->x = -0.100000001f;
            point->y = 0.389999986f;
            point->z = 0;
            break;
        case 44:
            point->x = 0.100000001f;
            point->y = 0.389999986f;
            point->z = 0;
            break;
        case 45:
            point->x = 0;
            point->y = 0.379999995f;
            point->z = 0;
            break;
        case 46:
            point->x = 0;
            point->y = 0.370000005f;
            point->z = 0;
            break;
        case 47:
            point->x = -0.0799999982f;
            point->y = 0.340000004f;
            point->z = 0;
            break;
        case 48:
            point->x = 0.0799999982f;
            point->y = 0.340000004f;
            point->z = 0;
            break;
        case 49:
            point->x = -0.129999995f;
            point->y = 0.319999993f;
            point->z = 0;
            break;
        case 50:
            point->x = 0.129999995f;
            point->y = 0.319999993f;
            point->z = 0;
            break;
        case 51:
            point->x = -0.0399999991f;
            point->y = 0.310000002f;
            point->z = 0;
            break;
        case 52:
            point->x = 0.0399999991f;
            point->y = 0.310000002f;
            point->z = 0;
            break;
        case 53:
            point->x = -0.100000001f;
            point->y = 0.280000001f;
            point->z = 0;
            break;
        case 54:
            point->x = 0.100000001f;
            point->y = 0.280000001f;
            point->z = 0;
            break;
        case 55:
            point->x = -0.159999996f;
            point->y = 0.270000011f;
            point->z = 0;
            break;
        case 56:
            point->x = -0.0599999987f;
            point->y = 0.270000011f;
            point->z = 0;
            break;
        case 57:
            point->x = 0.0599999987f;
            point->y = 0.270000011f;
            point->z = 0;
            break;
        case 58:
            point->x = 0.159999996f;
            point->y = 0.270000011f;
            point->z = 0;
            break;
        case 59:
            point->x = -0.0299999993f;
            point->y = 0.25f;
            point->z = 0;
            break;
        case 60:
            point->x = -0.109999999f;
            point->y = 0.159999996f;
            point->z = 0;
            break;
        case 61:
            point->x = -0.0399999991f;
            point->y = 0.159999996f;
            point->z = 0;
            break;
        case 62:
            point->x = 0.0399999991f;
            point->y = 0.159999996f;
            point->z = 0;
            break;
        case 63:
            point->x = 0.109999999f;
            point->y = 0.159999996f;
            point->z = 0;
            break;
        case 64:
            point->x = -0.119999997f;
            point->y = 0.150000006f;
            point->z = 0;
            break;
        case 65:
            point->x = -0.100000001f;
            point->y = 0.150000006f;
            point->z = 0;
            break;
        case 66:
            point->x = -0.0500000007f;
            point->y = 0.150000006f;
            point->z = 0;
            break;
        case 67:
            point->x = -0.0299999993f;
            point->y = 0.150000006f;
            point->z = 0;
            break;
        case 68:
            point->x = 0.0299999993f;
            point->y = 0.150000006f;
            point->z = 0;
            break;
        case 69:
            point->x = 0.0500000007f;
            point->y = 0.150000006f;
            point->z = 0;
            break;
        case 70:
            point->x = 0.100000001f;
            point->y = 0.150000006f;
            point->z = 0;
            break;
        case 71:
            point->x = 0.119999997f;
            point->y = 0.150000006f;
            point->z = 0;
            break;
        case 72:
            point->x = -0.180000007f;
            point->y = 0.159999996f;
            point->z = 0;
            break;
        case 73:
            point->x = 0.180000007f;
            point->y = 0.159999996f;
            point->z = 0;
            break;
        case 74:
            point->x = -0.170000002f;
            point->y = 0.150000006f;
            point->z = 0;
            break;
        case 75:
            point->x = 0.170000002f;
            point->y = 0.150000006f;
            point->z = 0;
            break;
        case 76:
            point->x = -0.180000007f;
            point->y = 0.310000002f;
            point->z = 0;
            break;
        case 77:
            point->x = 0.180000007f;
            point->y = 0.310000002f;
            point->z = 0;
            break;
        case 78:
            point->x = 0.0299999993f;
            point->y = 0.25f;
            point->z = 0;
            break;
        case 79:
            point->x = 0.0799999982f;
            point->y = 0.150000006f;
            point->z = 0;
            break;
        case 80:
            point->x = 0.129999995f;
            point->y = 0.150000006f;
            point->z = 0;
            break;
        case 81:
            point->x = 0.150000006f;
            point->y = 0.150000006f;
            point->z = 0;
            break;
        case 82:
            point->x = 0.200000003f;
            point->y = 0.150000006f;
            point->z = 0;
            break;
        case 83:
            point->x = -0.140000001f;
            point->y = 0.140000001f;
            point->z = 0;
            break;
        case 84:
            point->x = -0.0700000003f;
            point->y = 0.140000001f;
            point->z = 0;
            break;
        case 85:
            point->x = 0;
            point->y = 0.140000001f;
            point->z = 0;
            break;
        case 86:
            point->x = 0.0700000003f;
            point->y = 0.140000001f;
            point->z = 0;
            break;
        case 87:
            point->x = 0.140000001f;
            point->y = 0.140000001f;
            point->z = 0;
            break;
        case 88:
            point->x = -0.100000001f;
            point->y = 0.409999996f;
            point->z = 0;
            break;
        case 89:
            point->x = 0.100000001f;
            point->y = 0.409999996f;
            point->z = 0;
            break;
        }
    } else {
        work->nailCount = 41;
        switch (index) {
        case 0:
            point->x = -0.0900000036f;
            point->y = 0.589999974f;
            point->z = 0;
            break;
        case 1:
            point->x = -0.0299999993f;
            point->y = 0.589999974f;
            point->z = 0;
            break;
        case 2:
            point->x = 0.0299999993f;
            point->y = 0.589999974f;
            point->z = 0;
            break;
        case 3:
            point->x = 0.0900000036f;
            point->y = 0.589999974f;
            point->z = 0;
            break;
        case 4:
            point->x = -0.150000006f;
            point->y = 0.529999971f;
            point->z = 0;
            break;
        case 5:
            point->x = -0.0900000036f;
            point->y = 0.529999971f;
            point->z = 0;
            break;
        case 6:
            point->x = 0;
            point->y = 0.529999971f;
            point->z = 0;
            break;
        case 7:
            point->x = 0.0900000036f;
            point->y = 0.529999971f;
            point->z = 0;
            break;
        case 8:
            point->x = 0.150000006f;
            point->y = 0.529999971f;
            point->z = 0;
            break;
        case 9:
            point->x = -0.150000006f;
            point->y = 0.469999999f;
            point->z = 0;
            break;
        case 10:
            point->x = -0.0900000036f;
            point->y = 0.469999999f;
            point->z = 0;
            break;
        case 11:
            point->x = -0.0299999993f;
            point->y = 0.469999999f;
            point->z = 0;
            break;
        case 12:
            point->x = 0.0299999993f;
            point->y = 0.469999999f;
            point->z = 0;
            break;
        case 13:
            point->x = 0.0900000036f;
            point->y = 0.469999999f;
            point->z = 0;
            break;
        case 14:
            point->x = 0.150000006f;
            point->y = 0.469999999f;
            point->z = 0;
            break;
        case 15:
            point->x = -0.129999995f;
            point->y = 0.409999996f;
            point->z = 0;
            break;
        case 16:
            point->x = -0.0700000003f;
            point->y = 0.409999996f;
            point->z = 0;
            break;
        case 17:
            point->x = 0;
            point->y = 0.409999996f;
            point->z = 0;
            break;
        case 18:
            point->x = 0.0700000003f;
            point->y = 0.409999996f;
            point->z = 0;
            break;
        case 19:
            point->x = 0.129999995f;
            point->y = 0.409999996f;
            point->z = 0;
            break;
        case 20:
            point->x = -0.189999998f;
            point->y = 0.349999994f;
            point->z = 0;
            break;
        case 21:
            point->x = -0.0299999993f;
            point->y = 0.349999994f;
            point->z = 0;
            break;
        case 22:
            point->x = 0.0299999993f;
            point->y = 0.349999994f;
            point->z = 0;
            break;
        case 23:
            point->x = 0.189999998f;
            point->y = 0.349999994f;
            point->z = 0;
            break;
        case 24:
            point->x = -0.140000001f;
            point->y = 0.280000001f;
            point->z = 0;
            break;
        case 25:
            point->x = -0.0799999982f;
            point->y = 0.280000001f;
            point->z = 0;
            break;
        case 26:
            point->x = 0;
            point->y = 0.280000001f;
            point->z = 0;
            break;
        case 27:
            point->x = 0.0799999982f;
            point->y = 0.280000001f;
            point->z = 0;
            break;
        case 28:
            point->x = 0.140000001f;
            point->y = 0.280000001f;
            point->z = 0;
            break;
        case 29:
            point->x = -0.150000006f;
            point->y = 0.230000004f;
            point->z = 0;
            break;
        case 30:
            point->x = -0.0900000036f;
            point->y = 0.230000004f;
            point->z = 0;
            break;
        case 31:
            point->x = -0.0299999993f;
            point->y = 0.230000004f;
            point->z = 0;
            break;
        case 32:
            point->x = 0.0299999993f;
            point->y = 0.230000004f;
            point->z = 0;
            break;
        case 33:
            point->x = 0.0900000036f;
            point->y = 0.230000004f;
            point->z = 0;
            break;
        case 34:
            point->x = 0.150000006f;
            point->y = 0.230000004f;
            point->z = 0;
            break;
        case 35:
            point->x = -0.0799999982f;
            point->y = 0.180000007f;
            point->z = 0;
            break;
        case 36:
            point->x = 0.0799999982f;
            point->y = 0.180000007f;
            point->z = 0;
            break;
        case 37:
            point->x = -0.119999997f;
            point->y = 0.109999999f;
            point->z = 0;
            break;
        case 38:
            point->x = -0.0399999991f;
            point->y = 0.109999999f;
            point->z = 0;
            break;
        case 39:
            point->x = 0.0399999991f;
            point->y = 0.109999999f;
            point->z = 0;
            break;
        case 40:
            point->x = 0.119999997f;
            point->y = 0.109999999f;
            point->z = 0;
            break;
        }
    }
}

#endif
