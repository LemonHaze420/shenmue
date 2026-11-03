#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_qrdk.hpp"

void QrdkMain(void)
{
    HLibTask *task;
    QrdkWork *work;
    char *bytes;
    int started = 0, count = 0, index;
    do {
        while (qrdkEnemies[count].character != QRDK_INVALID)
            count++;
        task = FTBL.HLib_EnqueueTaskWithParameter(QrdkInitializeCallback, 4, 11, count * 4 + 76, QRDK_TASK);
        if (!task)
            break;
        work = (QrdkWork *)(FTBL.HLib_GetTaskParameter(task));
        if (!work)
            break;
        bytes = (char *)work;
        for (index = 0; index < count * 4 + 76; index++)
            bytes[index] = 0;
        started = 1;
        work->parent = FTBL.HLib_GetCurrentTask();
        work->task = task;
        work->enemyCount = count;
        QrdkHideCharacter(QRDK_RYO);
        FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(QRDK_RYO), 1.0f, 0);
        EventDebugRegister(QRDK_DEBUG_ID, QrdkDebugSkipCallback, work, 0x7fffffff);
        FTBL.SetPlayerControlEnabled(0);
        FTBL.HLib_SetupTaskDelay(-1);
    } while (0);
    if (!started && task)
        FTBL.HLib_DestroyTask(task);
}

void QrdkInitializeCallback(void *data)
{
    QrdkWork *work = (QrdkWork *)data;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(QrdkExitCallback);
            break;
        }
        if (FTBL.filmCheckFadeTrackColour())
            break;
        if (work->flags & 3) {
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->flags &= ~3;
        } else {
            EventDebugGetState(QRDK_DEBUG_ID, 1);
            EventDebugCheck(QRDK_DEBUG_ID, 4);
            FTBL.FinishLoading();
            FTBL.EV_NPC_NewLoadSw_0(0);
            FTBL.HLib_SetCurrentTaskTo(QrdkWaitPackageCallback);
        }
        break;
    }
}

void QrdkWaitPackageCallback(void *data)
{
    if (FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)qrdkPackage, 0)) {
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        FTBL.EV_ControlPackageResources(2, 2, qrdkPackage, 0, 0);
        FTBL.EV_ControlPackageResources(2, 4, qrdkPackage, 0, 0);
        FTBL.HLib_SetCurrentTaskTo(QrdkLoadCharactersCallback);
    }
}

void QrdkLoadCharactersCallback(void *data)
{
    QrdkWork *work = (QrdkWork *)data;
    shen_name character;
    int index;
    if (work) {
        if (work->loadIndex) {
            character = qrdkCharacters[work->loadIndex - 1];
            if (character != QRDK_INVALID) {
                if (FTBL.ForceLoadNpc(character)) {
                    QrdkHideCharacter(character);
                    work->loadIndex++;
                }
            } else {
                work->loadIndex--;
                FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
                work->flags |= 3;
                FTBL.HLib_SetCurrentTaskTo(QrdkBeginGameCallback);
            }
        } else {
            for (index = 0; qrdkCharacters[index] != QRDK_INVALID; index++)
                FTBL.CheckWarmStartNpcRoute(qrdkCharacters[index]);
            FTBL.EV_ResetFreeTalkFlags();
            work->loadIndex = 1;
        }
    } else
        FTBL.HLib_SetCurrentTaskTo(QrdkExitCallback);
}

void QrdkExitCallback(void *data)
{
    QrdkWork *work = (QrdkWork *)data;
    QrdkWork copy;
    char *bytes;
    int index;
    for (;;) {
        if (FTBL.filmCheckFadeTrackColour())
            break;
        if (work)
            copy = *work;
        else {
            bytes = (char *)&copy;
            for (index = 0; index < (int)(sizeof(copy)); index++)
                bytes[index] = 0;
        }
        if (copy.flags & 3) {
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            if (work)
                work->flags &= ~3;
            break;
        }
        FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        EventDebugUnregister(QRDK_DEBUG_ID);
        QrdkShowCharacter(QRDK_RYO);
        if (copy.flags & 4)
            FTBL.ReleaseDoor(copy.door, QRDK_RYO);
        for (index = 0; qrdkCharacters[index] != QRDK_INVALID; index++) {
            QrdkShowCharacter(qrdkCharacters[index]);
            FTBL.npcLockUnlock(qrdkCharacters[index]);
        }
        if (FTBL.EV_ControlPackageResources(0, 5, qrdkPackage, 0, 0))
            FTBL.EV_ControlPackageResources(0, 6, qrdkPackage, 0, 0);
        FTBL.HLib_MarkCurrentTaskForCleanup();
        if (copy.parent)
            FTBL.HLib_ResolveDeferredCallback(copy.parent);
        FTBL.SetPlayerControlEnabled(1);
        break;
    }
}

void QrdkDebugSkipCallback(void *data)
{
    QrdkWork *work = (QrdkWork *)data;
    Vector3 position;
    Vector3i rotation;
    if (!work)
        return;
    FTBL.HLib_AssignCallbackForTask(work->task, QrdkExitCallback);
    FTBL.HLib_SetupTimedTask(work->task, 1);
    QrdkGetDoorPoint(QRDK_R122, &position, (unsigned int *)&rotation.y);
    rotation.x = 0;
    rotation.z = 0;
    FTBL.CopyMotionTransition(FTBL.scenFindMotionwork(QRDK_RYO), 0, rotation.y, 13, 0);
    FTBL.EV_AT_SetPos(QRDK_RYO, QRDK_WORLD_ALL_AXES, &position);
    FTBL.EV_AT_SetAngY_0(QRDK_RYO, QRDK_WORLD_ALL_AXES, &rotation, 0);
}

void QrdkBeginGameCallback(void *data)
{
    QrdkWork *work = (QrdkWork *)data;
    Vector3 position;
    if (work) {
        work->flags = (work->flags & ~3) | 1;
        position.x = 10000.0f;
        position.y = 10000.0f;
        position.z = 10000.0f;
        FTBL.EV_AT_SetPos(QRDK_REN, QRDK_WORLD_ALL_AXES, &position);
        FTBL.HLib_SetCurrentTaskTo(QrdkStartRouteCallback);
    } else
        FTBL.HLib_SetCurrentTaskTo(QrdkExitCallback);
}

void QrdkStartRouteCallback(void *data)
{
    if (data) {
        QrdkPlaceOnRoute((QrdkWork *)data);
        FTBL.HLib_SetCurrentTaskTo(QrdkRunCallback);
    } else
        FTBL.HLib_SetCurrentTaskTo(QrdkExitCallback);
}

void QrdkRunCallback(void *data)
{
    QrdkWork *work = (QrdkWork *)data;
    float distance;
    shen_name door;
    int counter, route, index, defeated, hasEnemies;
    if (work) {
        counter = work->routeCounter;
        route = 0;
        while (counter) {
            counter--;
            if (qrdkRoute[route].marker == 2) {
                route = 0;
                while (qrdkRoute[route].marker != 1)
                    route++;
            } else
                route++;
        }
        if ((FTBL.GetCurrentControlInput(2) >> 10) & 1)
            work->action |= 1;
        for (;;) {
            if (work->action & 1) {
                door = QrdkFindClosestDoor(&distance);
                if (door == QRDK_INVALID)
                    break;
                if (door != QRDK_INVALID && distance < 5.0f) {
                    work->action &= ~1;
                    defeated = 0;
                    hasEnemies = 0;
                    for (index = 0; qrdkEnemies[index].character != QRDK_INVALID; index++) {
                        if (qrdkEnemies[index].door == door) {
                            defeated = defeated || work->defeated[index];
                            hasEnemies = 1;
                        }
                    }
                    if (defeated && hasEnemies)
                        QrdkQueueSerif("ここはまずいな", 0, 1, 50, 0); 
                    else {
                        work->door = door;
                        FTBL.GetCharaPosition(QRDK_RYO, QRDK_TRANSLATION_ROOT, &work->savedPosition, QRDK_WORLD);
                        FTBL.GetScenarioCharacterEulerAngles(QRDK_RYO, &work->savedRotation, QRDK_WORLD);
                        if (FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(QRDK_RYO), 58, 1))
                            FTBL.CopyMotionTransition(FTBL.scenFindMotionwork(QRDK_RYO), 0, work->savedRotation.y, 13, 0);
                        FTBL.HLib_SetCurrentTaskTo(QrdkBeginDoorCallback);
                        break;
                    }
                }
            }
            if (!FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(QRDK_RYO), 58, 1)) {
                work->routeCounter++;
                QrdkPlaceOnRoute(work);
            }
            if ((work->action & 1) == 0)
                FTBL.CheckHelpIcon(4, 0, 3);
            break;
        }
    } else {
        FTBL.CopyMotionTransition(FTBL.scenFindMotionwork(QRDK_RYO), 0, 0, 13, 0);
        FTBL.HLib_SetCurrentTaskTo(QrdkExitCallback);
    }
}

void QrdkBeginDoorCallback(void *data)
{
    if (data)
        FTBL.HLib_SetCurrentTaskTo(QrdkGetDoorCallback);
    else {
        FTBL.CopyMotionTransition(FTBL.scenFindMotionwork(QRDK_RYO), 0, 0, 13, 0);
        FTBL.HLib_SetCurrentTaskTo(QrdkExitCallback);
    }
}

void QrdkGetDoorCallback(void *data)
{
    QrdkWork *work = (QrdkWork *)data;
    Vector3 ryo, doorPosition;
    Vector3i rotation;
    Vector3 position;
    unsigned int doorYaw;
    unsigned int motion;
    int side;
    float start, end;
    unsigned int doorMotion = 0xf0ca;
    if (work) {
        if (FTBL.ClaimDoor(work->door, 0)) {
            work->flags |= 4;
            FTBL.GetCharaPosition(QRDK_RYO, QRDK_TRANSLATION_ROOT, &ryo, QRDK_WORLD);
            QrdkGetDoorPoint(work->door, &doorPosition, &doorYaw);
            FTBL.GetScenarioCharacterEulerAngles(QRDK_RYO, &rotation, QRDK_WORLD);
            side = FTBL.ComputeAngle(&ryo, 10.0f, rotation.y + 0x4000, 0x4000, &doorPosition);
            FTBL.CopyMotionTransition(FTBL.scenFindMotionwork(QRDK_RYO), 0, 0, 1, 0);
            switch (FTBL.shenGetRandomInt32(4)) {
            case 0:
                motion = 0x1c44;
                start = 19.0f;
                end = 70.0f;
                position.x = -0.9f;
                position.z = 1.0f;
                break;
            case 1:
                motion = 0x1c42;
                start = 314.0f;
                end = 360.0f;
                position.x = -0.9f;
                position.z = 1.5f;
                break;
            case 2:
                motion = 0x1c41;
                start = 26.0f;
                end = 74.0f;
                position.x = -0.9f;
                position.z = 2.0f;
                break;
            default:
                motion = 0x1c43;
                start = 36.0f;
                end = 87.0f;
                position.x = -0.9f;
                position.z = 2.0f;
                break;
            }
            position.y = 0.0f;
            rotation.x = 0;
            rotation.z = 0;
            if (side) {
                rotation.y = doorYaw + 0xc000;
                position.x = -position.x;
            } else
                rotation.y = doorYaw + 0x4000;
            FTBL.ApplyScenarioTranslationAndRotation(&doorPosition, &rotation, &position);
            FTBL.EV_AT_SetPos(QRDK_RYO, QRDK_WORLD_ALL_AXES, &position);
            FTBL.EV_AT_SetAngY_0(QRDK_RYO, QRDK_WORLD_ALL_AXES, &rotation, 0);
            if (side)
                FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(QRDK_RYO), 69, 0, 0);
            work->action |= 2;
            FTBL.doorPlayRigidMotion(work->door, doorMotion, 0.0f, 1.0f);
            FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(QRDK_RYO), motion, 0);
            FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(QRDK_RYO), 0);
            FTBL.SetMotionLayerLoopStartTime(FTBL.scenFindMotionwork(QRDK_RYO), start, 0);
            FTBL.SetMotionRegionEvalTime(FTBL.scenFindMotionwork(QRDK_RYO), start, 0);
            FTBL.SetMotionLayerEndFrame(FTBL.scenFindMotionwork(QRDK_RYO), end, 0);
            switch (work->door) {
            case QRDK_R122:
                FTBL.HLib_SetCurrentTaskTo(QrdkFoundCallback);
                break;
            case QRDK_R102:
            case QRDK_R104:
            case QRDK_R117:
                FTBL.HLib_SetCurrentTaskTo(QrdkBattleMessageCallback);
                break;
            default:
                FTBL.HLib_SetCurrentTaskTo(QrdkWrongRoomCallback);
                break;
            }
            if (QrdkPassedRoutePoint(work->routeCounter))
                work->routeCounter++;
        } else {
            FTBL.EV_AT_SetPos(QRDK_RYO, QRDK_WORLD_ALL_AXES, &work->savedPosition);
            FTBL.EV_AT_SetAngY_0(QRDK_RYO, QRDK_WORLD_ALL_AXES, &work->savedRotation, 0);
            FTBL.HLib_SetCurrentTaskTo(QrdkRunCallback);
        }
    } else {
        FTBL.CopyMotionTransition(FTBL.scenFindMotionwork(QRDK_RYO), 0, 0, 13, 0);
        FTBL.HLib_SetCurrentTaskTo(QrdkExitCallback);
    }
}

void QrdkBattleMessageCallback(void *data)
{
    QrdkWork *work = (QrdkWork *)data;
    if (work) {
        if (QrdkIsDoorMotionFinished(work)) {
            FTBL.ReleaseDoor(work->door, 0);
            work->flags &= ~4;
            switch (work->door) {
            case QRDK_R104:
                QrdkQueueSerif("なんだ、てめえは！", 0, 1, 100, QrdkResumeOnSerifFinished); 
                break;
            case QRDK_R102:
                QrdkQueueSerif("ネズミが飛び込んできたか！", 0, 1, 100, QrdkResumeOnSerifFinished); 
                break;
            case QRDK_R117:
                QrdkQueueSerif("てめえは、あの時の！", 0, 1, 100, QrdkResumeOnSerifFinished); 
                break;
            }
            FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(QRDK_RYO), 0xf003, 0);
            FTBL.HLib_SetCurrentTaskTo(QrdkPlaceEnemiesCallback);
        }
    } else
        FTBL.HLib_SetCurrentTaskTo(QrdkExitCallback);
}

void QrdkPlaceEnemiesCallback(void *data)
{
    QrdkWork *work = (QrdkWork *)data;
    Vector3 door, position;
    Vector3i rotation;
    unsigned int yaw;
    float x, z;
    int index;
    if (work) {
        if (!FTBL.filmCheckFadeTrackColour()) {
            if (work->flags & 3) {
                work->flags &= ~3;
                FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            } else {
                work->flags |= 3;
                FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
                for (index = 0; qrdkEnemies[index].character != QRDK_INVALID; index++) {
                    if (work->defeated[index])
                        continue;
                    if (qrdkEnemies[index].door != work->door)
                        continue;
                    QrdkGetDoorPoint(work->door, &door, &yaw);
                    x = qrdkEnemies[index].x;
                    z = qrdkEnemies[index].z;
                    position.y = door.y + 0.3f;
                    position.x = -z * FTBL.CalculateFixedSine(yaw);
                    position.z = -z * FTBL.CalculateFixedCosine(yaw);
                    if (__builtin_fabs(position.x) < 1.1920929e-07f)
                        position.x = 0.0f;
                    if (__builtin_fabs(position.z) < 1.1920929e-07f)
                        position.z = 0.0f;
                    position.x -= x * FTBL.CalculateFixedCosine(yaw);
                    position.z += x * FTBL.CalculateFixedSine(yaw);
                    if (__builtin_fabs(position.x) < 1.1920929e-07f)
                        position.x = 0.0f;
                    if (__builtin_fabs(position.z) < 1.1920929e-07f)
                        position.z = 0.0f;
                    position.x += door.x;
                    position.z += door.z;
                    if (__builtin_fabs(position.x) < 1.1920929e-07f)
                        position.x = 0.0f;
                    if (__builtin_fabs(position.z) < 1.1920929e-07f)
                        position.z = 0.0f;
                    rotation.x = 0;
                    rotation.y = (yaw + qrdkEnemies[index].yaw) & 0xffff;
                    rotation.z = 0;
                    FTBL.EV_AT_SetPos(qrdkEnemies[index].character, QRDK_WORLD_ALL_AXES, &position);
                    FTBL.EV_AT_SetAngY_0(qrdkEnemies[index].character, QRDK_WORLD_ALL_AXES, &rotation, 0);
                    FTBL.EV_SetCharaControllerFlags(qrdkEnemies[index].character, 0, 1);
                    FTBL.EV_SetCharaControllerFlags(qrdkEnemies[index].character, 1, 1);
                }
                FTBL.HLib_SetCurrentTaskTo(QrdkStartBattleCallback);
            }
        }
    } else
        FTBL.HLib_SetCurrentTaskTo(QrdkExitCallback);
}

void QrdkStartBattleCallback(void *data)
{
    QrdkWork *work = (QrdkWork *)data;
    Vector3 position;
    int index;
    if (work) {
        QrdkGetDoorPoint(work->door, &position, 0);
        FTBL.shenStartFGST(10, 10, 0, 0);
        FTBL.vfmDispatchControl(2, 0, qrdkPackage, 1);
        FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(QRDK_RYO), 0, 0, 0, 0, 0);
        for (index = 0; qrdkEnemies[index].character != QRDK_INVALID; index++) {
            if (work->defeated[index])
                continue;
            if (qrdkEnemies[index].door != work->door)
                continue;
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(qrdkEnemies[index].character), 0, 1, 1, 40, qrdkEnemies[index].style);
            FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(qrdkEnemies[index].character), 0, 0x400000, 128);
            FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(qrdkEnemies[index].character), 0, 0x200000, 2048);
            FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(qrdkEnemies[index].character), 0, 0x8000000, 1);
        }
        FTBL.vfCopyPosFromEntity(0, 0, position.x, position.z, 7.0f);
        FTBL.vfmQueueFightStart(2, 2, 1, 1.0f, -1);
        FTBL.HLib_SetCurrentTaskTo(QrdkBattleCallback);
    } else
        FTBL.HLib_SetCurrentTaskTo(QrdkExitCallback);
}

void QrdkBattleCallback(void *data)
{
    QrdkWork *work = (QrdkWork *)data;
    Vector3 position;
    Vector3i rotation;
    int index;
    for (;;) {
        if (FTBL.IsFightActive() || FTBL.filmCheckFadeTrackColour())
            break;
        if (work->flags & 3) {
            work->flags &= ~3;
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            break;
        }
        for (index = 0; qrdkEnemies[index].character != QRDK_INVALID; index++) {
            if (qrdkEnemies[index].door != work->door)
                continue;
            position.x = 10000.0f;
            position.y = 10000.0f;
            position.z = 10000.0f;
            rotation.x = 0;
            rotation.y = 0;
            rotation.z = 0;
            FTBL.EV_AT_SetPos(qrdkEnemies[index].character, QRDK_WORLD_ALL_AXES, &position);
            FTBL.EV_AT_SetAngY_0(qrdkEnemies[index].character, QRDK_WORLD_ALL_AXES, &rotation, 0);
            work->defeated[index] = 1;
        }
        FTBL.FinishLoading();
        if (FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(QRDK_RYO)) == 3) {
            work->flags |= 3;
            FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
            QrdkPlaceOnRoute(work);
            FTBL.HLib_SetCurrentTaskTo(QrdkRunCallback);
        } else
            FTBL.HLib_SetCurrentTaskTo(QrdkRetryCallback);
        break;
    }
}

void QrdkWrongRoomCallback(void *data)
{
    QrdkWork *work = (QrdkWork *)data;
    if (work) {
        if (QrdkIsDoorMotionFinished(work)) {
            switch (FTBL.shenGetRandomInt32(5)) {
            case 0:
                QrdkQueueSerif("違う。", 0, 0, 50, 0); 
                break;
            case 1:
                QrdkQueueSerif("いない。", 0, 0, 50, 0); 
                break;
            case 2:
                QrdkQueueSerif("くそっ。", 0, 0, 50, 0); 
                break;
            case 3:
                QrdkQueueSerif("違ったか。", 0, 0, 50, 0); 
                break;
            default:
                QrdkQueueSerif("ここじゃない。", 0, 0, 50, 0); 
                break;
            }
            FTBL.HLib_SetCurrentTaskTo(QrdkReturnToRouteCallback);
        }
    } else
        FTBL.HLib_SetCurrentTaskTo(QrdkExitCallback);
}

void QrdkReturnToRouteCallback(void *data)
{
    if (data) {
        QrdkPlaceOnRoute((QrdkWork *)data);
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.HLib_SetCurrentTaskTo(QrdkRunCallback);
    } else
        FTBL.HLib_SetCurrentTaskTo(QrdkExitCallback);
}

void QrdkFoundCallback(void *data)
{
    QrdkWork *work = (QrdkWork *)data;
    if (work) {
        if (QrdkIsDoorMotionFinished(work)) {
            QrdkQueueSerif("発見　イベント終了", 0, 1, 100, QrdkResumeOnSerifFinished); 
            FTBL.camCtrlSetFovTransitionTo(1);
            FTBL.camCtrlSetState(0);
            FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(QRDK_RYO), 0xf003, 0);
            FTBL.HLib_SetCurrentTaskTo(QrdkExitCallback);
            FTBL.HLib_SetupTaskDelay(-1);
        }
    } else
        FTBL.HLib_SetCurrentTaskTo(QrdkExitCallback);
}

void QrdkRetryCallback(void *data)
{
    QrdkWork *work = (QrdkWork *)data;
    const QrdkRoutePoint *point;
    Vector3 position;
    Vector3i rotation;
    float x, z;
    int index;
    if (work) {
        for (index = 0; qrdkEnemies[index].character != QRDK_INVALID; index++) {
            position.x = 10000.0f;
            position.y = 10000.0f;
            position.z = 10000.0f;
            rotation.x = 0;
            rotation.y = 0;
            rotation.z = 0;
            FTBL.EV_AT_SetPos(qrdkEnemies[index].character, QRDK_WORLD_ALL_AXES, &position);
            FTBL.EV_AT_SetAngY_0(qrdkEnemies[index].character, QRDK_WORLD_ALL_AXES, &rotation, 0);
            work->defeated[index] = 0;
        }
        QrdkQueueSerif("りとらい", 0, 0, 50, 0); 
        point = QrdkGetRoutePoint(0);
        FTBL.GetCharaPosition(QRDK_RYO, QRDK_TRANSLATION_ROOT, &position, QRDK_WORLD);
        position.x = point->x;
        position.z = point->z;
        FTBL.EV_AT_SetPos(QRDK_RYO, QRDK_WORLD_ALL_AXES, &position);
        point = QrdkGetRoutePoint(1);
        x = point->x - position.x;
        z = point->z - position.z;
        if (__builtin_fabs(x) < 1.1920929e-07f)
            x = 0.0f;
        if (__builtin_fabs(z) < 1.1920929e-07f)
            z = 0.0f;
        rotation.x = 0;
        rotation.y = FTBL.InverseFixedAtan2(x, z);
        rotation.z = 0;
        work->routeCounter = 1;
        work->flags |= 3;
        FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        QrdkPlaceOnRoute(work);
        FTBL.HLib_SetCurrentTaskTo(QrdkRunCallback);
    } else
        FTBL.HLib_SetCurrentTaskTo(QrdkExitCallback);
}

void QrdkPlaceOnRoute(QrdkWork *work)
{
    const QrdkRoutePoint *point, *next;
    Vector3 position;
    float x, z;
    int yaw;
    if (work) {
        FTBL.GetCharaPosition(QRDK_RYO, QRDK_TRANSLATION_ROOT, &position, QRDK_WORLD);
        point = QrdkGetRoutePoint(work->routeCounter);
        position.x = point->x;
        position.z = point->z;
        next = QrdkGetRoutePoint(work->routeCounter + 1);
        x = next->x - point->x;
        z = next->z - point->z;
        if (__builtin_fabs(x) < 1.1920929e-07f)
            x = 0.0f;
        if (__builtin_fabs(z) < 1.1920929e-07f)
            z = 0.0f;
        yaw = FTBL.InverseFixedAtan2(x, z);
        FTBL.CopyMotionTransition(FTBL.scenFindMotionwork(QRDK_RYO), &position, yaw, 5, 0);
        FTBL.CopyMotionTransition(FTBL.scenFindMotionwork(QRDK_RYO), &position, yaw, 8, 0);
        FTBL.CopyMotionTransition(FTBL.scenFindMotionwork(QRDK_RYO), 0, 0, 12, 0);
    }
}

const QrdkRoutePoint *QrdkGetRoutePoint(int counter)
{
    int start, length, index;
    if (counter < 0) {
        start = 0;
        length = 1;
        while (!qrdkRoute[start].marker)
            start++;
        while (!qrdkRoute[start + length].marker)
            length++;
        length++;
        while (counter < 0)
            counter += length;
    }
    index = 0;
    while (counter) {
        index++;
        counter--;
        if (qrdkRoute[index].marker == 2) {
            index = 0;
            while (!qrdkRoute[index].marker)
                index++;
        }
    }
    return &qrdkRoute[index];
}

int QrdkPassedRoutePoint(int counter)
{
    const QrdkRoutePoint *neighbour, *point;
    Vector3 ryo, route, offset;
    float backward;
    FTBL.GetCharaPosition(QRDK_RYO, QRDK_TRANSLATION_ROOT, &ryo, QRDK_WORLD);
    neighbour = QrdkGetRoutePoint(counter - 1);
    point = QrdkGetRoutePoint(counter);
    route.x = neighbour->x - point->x;
    route.y = 0.0f;
    route.z = neighbour->z - point->z;
    offset.x = ryo.x - point->x;
    route.y = 0.0f;
    offset.z = ryo.z - point->z;
    if (__builtin_fabs(route.x) < 1.1920929e-07f)
        route.x = 0.0f;
    if (__builtin_fabs(route.z) < 1.1920929e-07f)
        route.z = 0.0f;
    if (__builtin_fabs(offset.x) < 1.1920929e-07f)
        offset.x = 0.0f;
    if (__builtin_fabs(offset.z) < 1.1920929e-07f)
        offset.z = 0.0f;
    backward = FTBL.GetVector3DotProduct(&offset, &route);
    neighbour = QrdkGetRoutePoint(counter + 1);
    route.x = neighbour->x - point->x;
    route.z = neighbour->z - point->z;
    if (__builtin_fabs(route.x) < 1.1920929e-07f)
        route.x = 0.0f;
    if (__builtin_fabs(route.z) < 1.1920929e-07f)
        route.z = 0.0f;
    return FTBL.GetVector3DotProduct(&offset, &route) > backward;
}

void QrdkGetDoorPoint(shen_name door, Vector3 *position, unsigned int *yaw)
{
    Vector3 ryo;
    int index;
    for (index = 0; qrdkDoors[index].door != QRDK_INVALID; index++) {
        if (qrdkDoors[index].door != door)
            continue;
        if (position) {
            FTBL.GetCharaPosition(QRDK_RYO, QRDK_TRANSLATION_ROOT, &ryo, QRDK_WORLD);
            position->x = qrdkDoors[index].x;
            position->y = ryo.y;
            position->z = qrdkDoors[index].z;
        }
        if (yaw)
            *yaw = qrdkDoors[index].yaw;
        return;
    }
}

shen_name QrdkFindClosestDoor(float *distance)
{
    Vector3 ryo, position;
    Vector3i rotation;
    unsigned int yaw;
    float best, candidate;
    shen_name result;
    int index;
    FTBL.GetCharaPosition(QRDK_RYO, QRDK_TRANSLATION_ROOT, &ryo, QRDK_WORLD);
    FTBL.GetScenarioCharacterEulerAngles(QRDK_RYO, &rotation, QRDK_WORLD);
    best = 10000.0f;
    result = QRDK_INVALID;
    for (index = 0; qrdkDoors[index].door != QRDK_INVALID; index++) {
        QrdkGetDoorPoint(qrdkDoors[index].door, &position, &yaw);
        if (FTBL.ComputeAngle(&ryo, best, rotation.y, 0x2000, &position) && FTBL.ComputeAngle(&position, 8.0f, yaw - 0x8000, 10012, &ryo)) {
            candidate = FTBL.CalculatePlanarDistanceBetweenPoints(&ryo, &position);
            if (best > candidate) {
                best = candidate;
                result = qrdkDoors[index].door;
            }
        }
    }
    if (distance)
        *distance = best;
    return result;
}

int QrdkIsDoorMotionFinished(QrdkWork *work)
{
    int finished;
    if (work) {
        finished = !(work->action & 2) || FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(QRDK_RYO), 65, 1, 0);
        if (finished)
            work->action &= ~2;
    } else
        finished = 1;
    return finished;
}

void QrdkQueueSerif(const char *text, int voice, int startTicks, int endTicks, QrdkSerifCallback callback)
{
    HLibTask *parent, *task;
    QrdkSerifWork *work;
    int length, index;
    parent = FTBL.HLib_GetCurrentTask();
    if (text && endTicks > 0) {
        for (length = 0; text[length]; length++)
            ;
        task = FTBL.HLib_EnqueueTaskWithParameter(QrdkSerifCallbackLoop, 4, 11, startTicks > 0 ? length + 28 : 27, QRDK_TASK);
        if (task) {
            work = (QrdkSerifWork *)(FTBL.HLib_GetTaskParameter(task));
            if (work) {
                work->parent = parent;
                work->startTicks = startTicks;
                work->endTicks = endTicks;
                work->voice = voice;
                work->callback = callback;
                work->conversation = 0;
                if (startTicks <= 0) {
                    if (voice) {
                        work->conversation = FTBL.EV_PlayDialogue(text);
                        if (!work->conversation) {
                            FTBL.HLib_DestroyTask(task);
                            if (callback)
                                callback(0, (int)parent);
                        } else if (callback)
                            callback(1, 0);
                    } else {
                        FTBL.dbgPrintEventMessage(text);
                        if (callback)
                            callback(1, 0);
                    }
                } else {
                    for (index = 0; text[index]; index++)
                        work->text[index] = text[index];
                    work->text[index] = 0;
                }
            } else {
                FTBL.HLib_DestroyTask(task);
                if (callback)
                    callback(0, (int)parent);
            }
        }
    } else if (callback)
        callback(0, (int)parent);
}

void QrdkSerifCallbackLoop(void *data)
{
    QrdkSerifWork *work = (QrdkSerifWork *)data;
    if (work) {
    if ((FTBL.GetCurrentControlInput(2) >> 9) & 1) {
        if (work->startTicks > 1)
            work->startTicks = 1;
        else if (work->endTicks > 1)
            work->endTicks = 1;
    }
    if (!--work->startTicks) {
        if (work->voice) {
            work->conversation = FTBL.EV_PlayDialogue(work->text);
            if (!work->conversation) {
                FTBL.HLib_DestroyCurrentEventTask();
                if (work->callback)
                    work->callback(0, (int)work->parent);
            } else if (work->callback)
                work->callback(1, 0);
        } else {
            FTBL.dbgPrintEventMessage(work->text);
            if (work->callback)
                work->callback(1, 0);
        }
    }
    if (!--work->endTicks) {
        if (work->startTicks <= 0) {
            if (work->voice) {
                if (work->conversation && FTBL.cvsHasValidConverseData(work->conversation))
                    FTBL.scenCvsCtrlFree(work->conversation);
                work->conversation = 0;
            } else
                FTBL.ctrlStopSubtitles();
        }
        FTBL.HLib_DestroyCurrentTask();
        if (work->callback)
            work->callback(3, (int)work->parent);
    }
    if (work->callback)
        work->callback(2, work->endTicks);
    } else
        FTBL.HLib_DestroyCurrentTask();
}

void QrdkResumeOnSerifFinished(int event, int argument)
{
    if (event == 3)
        FTBL.HLib_ResolveDeferredCallback((HLibTask *)argument);
}

void QrdkHideCharacter(shen_name character)
{
    float reflector[4];
    int hasMotion = FTBL.scenFindMotionwork(character) != 0;
    if (hasMotion) {
        reflector[0] = 1.0f;
        reflector[1] = 1.0f;
        reflector[2] = 1.0f;
        reflector[3] = 1.0f;
        FTBL.SetScenarioReflectorBoardValues(character, reflector);
        FTBL.SetRefBoardParamsForCharaById(character, 1);
    }
    FTBL.EV_SetCharaControllerFlags(character, 1, 0);
    FTBL.EV_SetCharaControllerFlags(character, 0, 0);
    if (hasMotion)
        FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 2);
    FTBL.EV_SetCharacterAlphaEnabled(character, 0);
}

void QrdkShowCharacter(shen_name character)
{
    int hasMotion = FTBL.scenFindMotionwork(character) != 0;
    FTBL.EV_SetCharacterAlphaEnabled(character, 1);
    if (hasMotion)
        FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 0);
    FTBL.EV_SetCharaControllerFlags(character, 0, 1);
    FTBL.EV_SetCharaControllerFlags(character, 1, 1);
    if (hasMotion)
        FTBL.SetRefBoardParamsForCharaById(character, 0);
}

char qrdkPackage[] = "QRDK";
QrdkRoutePoint qrdkRoute[] = {{209.0f, 177.7f, 0}, {200.0f, 179.0f, 0}, {199.2f, 176.0f, 1}, {199.2f, 150.0f, 0},
    {200.5f, 147.3f, 0}, {214.2f, 148.2f, 0}, {216.2f, 145.2f, 0}, {217.5f, 126.0f, 0}, {236.4f, 126.0f, 0}, {232.6f, 125.8f, 0},
    {217.0f, 126.4f, 0}, {216.8f, 131.6f, 0}, {216.8f, 144.8f, 0}, {214.84f, 147.2f, 0}, {198.9f, 147.8f, 0}, {199.0f, 150.6f, 2}};
QrdkDoorPoint qrdkDoors[] = {{SHEN_NAME4('R', '1', '0', '1'), 200.0f, 152.0f, 0xc000}, {QRDK_R102, 200.0f, 157.5f, 0xc000},
    {SHEN_NAME4('R', '1', '0', '3'), 200.0f, 163.0f, 0xc000}, {QRDK_R104, 200.0f, 168.4f, 0xc000},
    {SHEN_NAME4('R', '1', '0', '5'), 200.0f, 173.7f, 0xc000}, {SHEN_NAME4('R', '1', '1', '6'), 215.0f, 143.0f, 0x4000},
    {QRDK_R117, 215.0f, 138.0f, 0x4000}, {SHEN_NAME4('R', '1', '1', '8'), 215.0f, 132.0f, 0x4000},
    {SHEN_NAME4('R', '1', '1', '9'), 215.0f, 127.0f, 0x4000}, {SHEN_NAME4('R', '1', '2', '0'), 218.0f, 124.0f, 0},
    {SHEN_NAME4('R', '1', '2', '1'), 223.3f, 124.0f, 0}, {QRDK_R122, 228.6f, 124.0f, 0}, {SHEN_NAME4('R', '1', '2', '3'), 234.0f, 124.0f, 0},
    {QRDK_INVALID, 0.0f, 0.0f, 0}};
QrdkEnemy qrdkEnemies[] = {{SHEN_NAME4('P', '0', '5', 'Q'), QRDK_R102, 1.0f, -1.0f, 0, 2},
    {SHEN_NAME4('P', '0', '4', 'Q'), QRDK_R102, -1.0f, -1.0f, 0, 3}, {SHEN_NAME4('P', '0', '7', 'Q'), QRDK_R104, 1.0f, -1.0f, 0, 2},
    {SHEN_NAME4('P', '0', '1', 'Q'), QRDK_R104, -1.0f, -1.0f, 0, 3}, {SHEN_NAME4('P', '0', '8', 'Q'), QRDK_R117, 1.0f, -1.0f, 0, 2},
    {SHEN_NAME4('P', '0', '3', 'Q'), QRDK_R117, -1.0f, -1.0f, 0, 3}, {QRDK_INVALID, QRDK_INVALID, 0.0f, 0.0f, 0, 0}};
shen_name qrdkCharacters[] = {QRDK_REN, SHEN_NAME4('P', '0', '5', 'Q'), SHEN_NAME4('P', '0', '4', 'Q'), SHEN_NAME4('P', '0', '7', 'Q'),
    SHEN_NAME4('P', '0', '1', 'Q'), SHEN_NAME4('P', '0', '8', 'Q'), SHEN_NAME4('P', '0', '3', 'Q'), QRDK_INVALID};
#else
#include "s2/areas/area_qrdk.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/door.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/flag.hpp"
#include "s2/input.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/ui.hpp"
#include <bit>
#include <cmath>
#include <cstring>

namespace shenmue::s2::areas {
void QrdkInitNullsub() {
}

void QrdkMain(std::uintptr_t) {
    std::uint32_t count = 0;
    while (QRDK_ENEMIES[count].character.name != QRDK_INVALID.name)
        ++count;
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(QrdkInitializeCallback, 4, 0x0b, 84U + count * 4U, QRDK_TASK);
    if (!task)
        return;
    QrdkWork *const work = static_cast<QrdkWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    std::memset(static_cast<void *>(work), 0, static_cast<std::size_t>(84U + count * 4U));
    work->parent = HLib::GetCurrentTask();
    work->task = task;
    work->enemyCount = count;
    QrdkHideCharacter(QRDK_RYO);
    mtwkSetSpeed(scenFindMotionwork(QRDK_RYO), 1, MotionRegion::LowerBody);
    SetPlayerControlEnabled(0);
    HLib::SetupTaskDelay(-1);
}

void QrdkHideCharacter(shen_name character) {
    const bool hasMotion = scenFindMotionwork(character) != nullptr;
    if (hasMotion) {
        const Vector4 reflector{1, 1, 1, 1};
        SetScenarioReflectorBoardValues(character, &reflector);
        EV_SetRefBoardParamsFor(character, 1);
    }
    EV_SetCharaControllerFlags(character, 1, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, 0, NpcCharacterControllerState::Disabled);
    if (hasMotion)
        mtwkSetExtraFlag(scenFindMotionwork(character), 6, 2);
    EV_SetCharacterAlphaEnabled(character, false);
}

void QrdkInitializeCallback(void *data) {
    QrdkWork *const work = static_cast<QrdkWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(QrdkExitCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (work->flags & 3U) {
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags &= ~3U;
    } else {
        FinishLoading();
        EV_NPC_NewLoadSw(0);
        HLib::SetCurrentTaskTo(QrdkWaitPackageCallback);
    }
}

void QrdkWaitPackageCallback(void *) {
    if (EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("QRDK"))) {
        EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QRDK"), 0);
        EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QRDK"), 0);
        HLib::SetCurrentTaskTo(QrdkLoadCharactersCallback);
    }
}

void QrdkLoadCharactersCallback(void *data) {
    QrdkWork *const work = static_cast<QrdkWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(QrdkExitCallback);
        return;
    }
    if (!work->loadIndex) {
        for (std::size_t index = 0; QRDK_CHARACTERS[index].name != QRDK_INVALID.name; ++index)
            EV_CheckWarmStartNpcRoute(QRDK_CHARACTERS[index]);
        EV_ResetFreeTalkFlags();
        work->loadIndex = 1;
    } else {
        const shen_name character = QRDK_CHARACTERS[work->loadIndex - 1U];
        if (character.name == QRDK_INVALID.name) {
            --work->loadIndex;
            SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
            work->flags |= 3U;
            HLib::SetCurrentTaskTo(QrdkBeginGameCallback);
        } else if (EV_ForceLoadNpc(character)) {
            QrdkHideCharacter(character);
            ++work->loadIndex;
        }
    }
}

void QrdkBeginGameCallback(void *data) {
    QrdkWork *const work = static_cast<QrdkWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(QrdkExitCallback);
        return;
    }
    work->flags = (work->flags & ~2U) | 1U;
    EV_AT_SetPos(QRDK_REN, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3{10000, 10000, 10000});
    HLib::SetCurrentTaskTo(QrdkStartRouteCallback);
}

const QrdkRoutePoint *QrdkGetRoutePoint(std::int32_t counter) {
    if (counter < 0) {
        std::int32_t start = 0, length = 1;
        while (!QRDK_ROUTE.data()[start].marker)
            ++start;
        while (!QRDK_ROUTE.data()[start + length].marker)
            ++length;
        do {
            counter += length + 1;
        } while (counter < 0);
    }
    std::int32_t index = 0;
    while (counter) {
        ++index;
        --counter;
        if (QRDK_ROUTE.data()[index].marker == 2) {
            index = 0;
            while (!QRDK_ROUTE.data()[index].marker)
                ++index;
        }
    }
    return &QRDK_ROUTE.data()[index];
}

void QrdkPlaceOnRoute(QrdkWork *work) {
    if (!work)
        return;
    Vector3 position;
    EV_GetCharaPosition(QRDK_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    const QrdkRoutePoint *const point = QrdkGetRoutePoint(work->routeCounter);
    position.x = point->x;
    position.z = point->z;
    const QrdkRoutePoint *const next = QrdkGetRoutePoint(static_cast<std::int32_t>(static_cast<std::uint32_t>(work->routeCounter) + 1U));
    float x = next->x - point->x, z = next->z - point->z;
    if (std::fabs(x) < 0.00000011920929f)
        x = 0;
    if (std::fabs(z) < 0.00000011920929f)
        z = 0;
    const std::uint16_t yaw = static_cast<std::uint16_t>(InverseFixedAtan2(x, z));
    mtwkCopyTransition(scenFindMotionwork(QRDK_RYO), &position, yaw, 5, 0);
    mtwkCopyTransition(scenFindMotionwork(QRDK_RYO), &position, yaw, 8, 0);
    mtwkCopyTransition(scenFindMotionwork(QRDK_RYO), nullptr, 0, 12, 0);
}

void QrdkStartRouteCallback(void *data) {
    if (data) {
        QrdkPlaceOnRoute(static_cast<QrdkWork *>(data));
        HLib::SetCurrentTaskTo(QrdkRunCallback);
    } else
        HLib::SetCurrentTaskTo(QrdkExitCallback);
}

std::uint32_t QrdkGetDoorPoint(shen_name door, Vector3 *position, std::uint32_t *yaw) {
    std::uint32_t result = QRDK_DOORS[0].door.name;
    for (std::size_t index = 0; result != QRDK_INVALID.name; result = QRDK_DOORS[++index].door.name) {
        const QrdkDoorPoint &point = QRDK_DOORS[index];
        if (result != door.name)
            continue;
        if (position) {
            Vector3 ryo;
            EV_GetCharaPosition(QRDK_RYO, CHARACTER_TRANSLATION_ROOT, &ryo, EventTransformFlags::World);
            *position = {point.x, ryo.y, point.z};
            result = std::bit_cast<std::uint32_t>(point.z);
        }
        if (yaw)
            result = *yaw = point.yaw;
        return result;
    }
    return result;
}

shen_name QrdkFindClosestDoor(float *distance) {
    Vector3 ryo;
    Vector3i rotation;
    EV_GetCharaPosition(QRDK_RYO, CHARACTER_TRANSLATION_ROOT, &ryo, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(QRDK_RYO, &rotation, EventTransformFlags::World);
    shen_name result = QRDK_INVALID;
    float best = 10000;
    for (std::size_t index = 0; QRDK_DOORS[index].door.name != QRDK_INVALID.name; ++index) {
        Vector3 position;
        std::uint32_t yaw;
        QrdkGetDoorPoint(QRDK_DOORS[index].door, &position, &yaw);
        if (ComputeAngle(ryo, best, static_cast<std::int16_t>(rotation.y), 0x2000, position) &&
            ComputeAngle(position, 8, static_cast<std::int16_t>(yaw + 0x8000U), 10012, ryo)) {
            const float candidate = GetPlanarDistance(&ryo, &position);
            if (best > candidate) {
                result = QRDK_DOORS[index].door;
                best = candidate;
            }
        }
    }
    if (distance)
        *distance = best;
    return result;
}

void QrdkRunCallback(void *data) {
    QrdkWork *const work = static_cast<QrdkWork *>(data);
    if (!work) {
        mtwkCopyTransition(scenFindMotionwork(QRDK_RYO), nullptr, 0, 13, 0);
        HLib::SetCurrentTaskTo(QrdkExitCallback);
        return;
    }
    if (GetCurrentControlInput(INPUT_REPEAT) & 0x400U)
        work->action |= 1U;
    if (work->action & 1U) {
        float distance;
        const shen_name door = QrdkFindClosestDoor(&distance);
        if (door.name == QRDK_INVALID.name)
            return;
        if (distance < 5) {
            work->action &= ~1U;
            bool defeated = false, hasEnemies = false;
            for (std::size_t index = 0; QRDK_ENEMIES[index].character.name != QRDK_INVALID.name; ++index) {
                if (QRDK_ENEMIES[index].door.name == door.name) {
                    if (work->defeated[index])
                        defeated = true;
                    hasEnemies = true;
                }
            }
            if (defeated && hasEnemies)
                QrdkQueueSerif("ここはまずいな", 1, 50, nullptr);
            else {
                work->door = door;
                EV_GetCharaPosition(QRDK_RYO, CHARACTER_TRANSLATION_ROOT, &work->savedPosition, EventTransformFlags::World);
                GetScenarioCharacterEulerAngles(QRDK_RYO, &work->savedRotation, EventTransformFlags::World);
                if (mtwkSetExtraFlag(scenFindMotionwork(QRDK_RYO), 58, 1))
                    mtwkCopyTransition(scenFindMotionwork(QRDK_RYO), nullptr, static_cast<std::uint16_t>(work->savedRotation.y), 13, 0);
                HLib::SetCurrentTaskTo(QrdkBeginDoorCallback);
                return;
            }
        }
    }
    if (!mtwkSetExtraFlag(scenFindMotionwork(QRDK_RYO), 58, 1)) {
        work->routeCounter = static_cast<std::int32_t>(static_cast<std::uint32_t>(work->routeCounter) + 1U);
        QrdkPlaceOnRoute(work);
    }
    if (!(work->action & 1U))
        CheckHelpIcon(4, 0, CheckHelpIconOp::FrameOverlay);
}

void QrdkQueueSerif(const char *text, std::int32_t startTicks, std::int32_t endTicks, QrdkSerifCallback callback) {
    HLib::Task *const parent = HLib::GetCurrentTask();
    if (!text) {
        if (callback)
            callback(0, reinterpret_cast<std::uintptr_t>(parent));
        return;
    }
    const std::size_t length = std::strlen(text);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(QrdkSerifCallbackLoop, 4, 0x0b, startTicks > 0 ? 40 + length : 39, QRDK_TASK);
    if (!task)
        return;
    QrdkSerifWork *const work = static_cast<QrdkSerifWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        if (callback)
            callback(0, reinterpret_cast<std::uintptr_t>(parent));
        return;
    }
    work->parent = parent;
    work->conversation = nullptr;
    work->startTicks = startTicks;
    work->endTicks = endTicks;
    work->callback = callback;
    work->voice = 0;
    if (startTicks > 0)
        std::memcpy(reinterpret_cast<std::byte *>(work) + 36, text, length + 1);
    else {
        dbgPrintEventMessage(text);
        if (callback)
            callback(1, 0);
    }
}

void QrdkSerifCallbackLoop(void *data) {
    QrdkSerifWork *const work = static_cast<QrdkSerifWork *>(data);
    if (!work) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (GetCurrentControlInput(INPUT_REPEAT) & 0x200U) {
        if (work->startTicks <= 1) {
            if (work->endTicks > 1)
                work->endTicks = 1;
        } else
            work->startTicks = 1;
    }
    work->startTicks = static_cast<std::int32_t>(static_cast<std::uint32_t>(work->startTicks) - 1U);
    if (!work->startTicks) {
        const char *const text = reinterpret_cast<const char *>(work) + 36;
        if (work->voice) {
            work->conversation = EV_PlayDialogue(text);
            if (!work->conversation) {
                HLib::DestroyCurrentTask();
                if (work->callback)
                    work->callback(0, reinterpret_cast<std::uintptr_t>(work->parent));
            } else if (work->callback)
                work->callback(1, 0);
        } else {
            dbgPrintEventMessage(text);
            if (work->callback)
                work->callback(1, 0);
        }
    }
    work->endTicks = static_cast<std::int32_t>(static_cast<std::uint32_t>(work->endTicks) - 1U);
    if (!work->endTicks) {
        if (work->startTicks <= 0) {
            if (work->voice) {
                if (work->conversation && cvsHasValidConverseData(work->conversation))
                    scenCvsCtrlFree(work->conversation);
                work->conversation = nullptr;
            } else
                ctrlStopSubtitles();
        }
        HLib::DestroyCurrentTask();
        if (work->callback)
            work->callback(3, reinterpret_cast<std::uintptr_t>(work->parent));
    }

    if (work->callback)
        work->callback(2, static_cast<std::uintptr_t>(static_cast<std::intptr_t>(work->endTicks)));
}

void QrdkResumeOnSerifFinished(std::int32_t event, std::uintptr_t argument) {
    if (event == 3)
        HLib::ResolveDeferredCallback(reinterpret_cast<HLib::Task *>(argument));
}

void QrdkBeginDoorCallback(void *data) {
    if (data)
        HLib::SetCurrentTaskTo(QrdkGetDoorCallback);
    else {
        mtwkCopyTransition(scenFindMotionwork(QRDK_RYO), nullptr, 0, 13, 0);
        HLib::SetCurrentTaskTo(QrdkExitCallback);
    }
}

void QrdkGetDoorCallback(void *data) {
    QrdkWork *const work = static_cast<QrdkWork *>(data);
    if (!work) {
        mtwkCopyTransition(scenFindMotionwork(QRDK_RYO), nullptr, 0, 13, 0);
        HLib::SetCurrentTaskTo(QrdkExitCallback);
        return;
    }
    if (!ClaimDoor(work->door, shen_name{0})) {
        EV_AT_SetPos(QRDK_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, work->savedPosition);
        EV_AT_SetAngY(QRDK_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, work->savedRotation);
        HLib::SetCurrentTaskTo(QrdkRunCallback);
        return;
    }
    work->flags |= 4U;
    Vector3 ryo, doorPosition;
    Vector3i rotation;
    std::uint32_t doorYaw;
    EV_GetCharaPosition(QRDK_RYO, CHARACTER_TRANSLATION_ROOT, &ryo, EventTransformFlags::World);
    QrdkGetDoorPoint(work->door, &doorPosition, &doorYaw);
    GetScenarioCharacterEulerAngles(QRDK_RYO, &rotation, EventTransformFlags::World);
    const bool side = ComputeAngle(ryo, 10, static_cast<std::int16_t>(rotation.y + 0x4000), 0x4000, doorPosition);
    mtwkCopyTransition(scenFindMotionwork(QRDK_RYO), nullptr, 0, 1, 0);
    Vector3 position{-0.8999999761581421f, 0, 2};
    float start, end;
    std::uint16_t motion;
    switch (shenGetRandomInt32(4)) {
    case 0:
        motion = QRDK_MOTION_KT6_TALK_D;
        start = 19;
        end = 70;
        position.z = 1;
        break;
    case 1:
        motion = QRDK_MOTION_DOOR_KICK_PUSH;
        start = 314;
        end = 360;
        position.z = 1.5f;
        break;
    case 2:
        motion = QRDK_MOTION_BREAK_DOOR;
        start = 26;
        end = 74;
        break;
    default:
        motion = QRDK_MOTION_ANC_TALK_C;
        start = 36;
        end = 87;
        break;
    }
    rotation = {0, static_cast<std::int32_t>(doorYaw + (side ? 0xc000U : 0x4000U)), 0};
    if (side)
        position.x = 0.8999999761581421f;
    scenApplyTransRot(&doorPosition, &rotation, &position);
    EV_AT_SetPos(QRDK_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(QRDK_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    if (side)
        mtwkSetRegionFlag(scenFindMotionwork(QRDK_RYO), 69, 0, MotionRegion::LowerBody);
    work->action |= 2U;
    doorPlayRigidMotion(work->door, 0xf0ca, 0, 1);
    mtwkRequestMotion(scenFindMotionwork(QRDK_RYO), motion, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(scenFindMotionwork(QRDK_RYO), 0);
    mtwkSetLayerLoopStartTime(scenFindMotionwork(QRDK_RYO), start, MotionRegion::LowerBody);
    mtwkSetRegionEvalTime(scenFindMotionwork(QRDK_RYO), start, MotionRegion::LowerBody);
    mtwkSetLayerEndFrame(scenFindMotionwork(QRDK_RYO), end, MotionRegion::LowerBody);
    if (work->door.name == QRDK_R102.name || work->door.name == QRDK_R104.name || work->door.name == QRDK_R117.name)
        HLib::SetCurrentTaskTo(QrdkBattleMessageCallback);
    else
        HLib::SetCurrentTaskTo(work->door.name == QRDK_R122.name ? QrdkFoundCallback : QrdkWrongRoomCallback);
    if (QrdkPassedRoutePoint(work->routeCounter))
        work->routeCounter = static_cast<std::int32_t>(static_cast<std::uint32_t>(work->routeCounter) + 1U);
}

bool QrdkPassedRoutePoint(std::int32_t counter) {
    Vector3 ryo;
    EV_GetCharaPosition(QRDK_RYO, CHARACTER_TRANSLATION_ROOT, &ryo, EventTransformFlags::World);
    const QrdkRoutePoint *const previous = QrdkGetRoutePoint(static_cast<std::int32_t>(static_cast<std::uint32_t>(counter) - 1U));
    const QrdkRoutePoint *const point = QrdkGetRoutePoint(counter);
    float px = previous->x - point->x, pz = previous->z - point->z;
    float x = ryo.x - point->x, z = ryo.z - point->z;
    if (std::fabs(px) < 0.00000011920929f)
        px = 0;
    if (std::fabs(pz) < 0.00000011920929f)
        pz = 0;
    if (std::fabs(x) < 0.00000011920929f)
        x = 0;
    if (std::fabs(z) < 0.00000011920929f)
        z = 0;
    const float y = ryo.y * 0.0f;
    const float backward = (x * px + y) + z * pz;
    const QrdkRoutePoint *const next = QrdkGetRoutePoint(static_cast<std::int32_t>(static_cast<std::uint32_t>(counter) + 1U));
    float nx = next->x - point->x, nz = next->z - point->z;
    if (std::fabs(nx) < 0.00000011920929f)
        nx = 0;
    if (std::fabs(nz) < 0.00000011920929f)
        nz = 0;
    return (x * nx + y) + z * nz > backward;
}

void QrdkWrongRoomCallback(void *data) {
    QrdkWork *const work = static_cast<QrdkWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(QrdkExitCallback);
        return;
    }
    if ((work->action & 2U) && !mtwkSetRegionFlag(scenFindMotionwork(QRDK_RYO), 65, 1, MotionRegion::LowerBody))
        return;
    work->action &= ~2U;
    const char *text;
    switch (shenGetRandomInt32(5)) {
    case 0:
        text = "違う。";
        break;
    case 1:
        text = "いない。";
        break;
    case 2:
        text = "くそっ。";
        break;
    case 3:
        text = "違ったか。";
        break;
    default:
        text = "ここじゃない。";
        break;
    }
    QrdkQueueSerif(text, 0, 50, nullptr);
    HLib::SetCurrentTaskTo(QrdkReturnToRouteCallback);
}

void QrdkReturnToRouteCallback(void *data) {
    using enum CameraState;
    if (!data) {
        HLib::SetCurrentTaskTo(QrdkExitCallback);
        return;
    }
    QrdkPlaceOnRoute(static_cast<QrdkWork *>(data));
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    HLib::SetCurrentTaskTo(QrdkRunCallback);
}

void QrdkFoundCallback(void *data) {
    using enum CameraState;
    QrdkWork *const work = static_cast<QrdkWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(QrdkExitCallback);
        return;
    }
    if ((work->action & 2U) && !mtwkSetRegionFlag(scenFindMotionwork(QRDK_RYO), 65, 1, MotionRegion::LowerBody))
        return;
    work->action &= ~2U;
    QrdkQueueSerif("発見　イベント終了", 1, 100, QrdkResumeOnSerifFinished);
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(Init);
    mtwkRequestMotion(scenFindMotionwork(QRDK_RYO), 0xf003, MotionRegion::LowerBody);
    HLib::SetCurrentTaskTo(QrdkExitCallback);
    HLib::SetupTaskDelay(-1);
}

void QrdkBattleMessageCallback(void *data) {
    QrdkWork *const work = static_cast<QrdkWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(QrdkExitCallback);
        return;
    }
    if ((work->action & 2U) && !mtwkSetRegionFlag(scenFindMotionwork(QRDK_RYO), 65, 1, MotionRegion::LowerBody))
        return;
    work->action &= ~2U;
    ReleaseDoor(std::bit_cast<std::int32_t>(work->door.name), shen_name{0});
    work->flags &= ~4U;
    const char *text = nullptr;
    if (work->door.name == QRDK_R102.name)
        text = "ネズミが飛び込んできたか！";
    else if (work->door.name == QRDK_R104.name)
        text = "なんだ、てめえは！";
    else if (work->door.name == QRDK_R117.name)
        text = "てめえは、あの時の！";
    if (text)
        QrdkQueueSerif(text, 1, 100, QrdkResumeOnSerifFinished);
    mtwkRequestMotion(scenFindMotionwork(QRDK_RYO), 0xf003, MotionRegion::LowerBody);
    HLib::SetCurrentTaskTo(QrdkPlaceEnemiesCallback);
}

void QrdkPlaceEnemiesCallback(void *data) {
    QrdkWork *const work = static_cast<QrdkWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(QrdkExitCallback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    if (work->flags & 3U) {
        work->flags &= ~3U;
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        return;
    }
    work->flags |= 3U;
    SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    for (std::size_t index = 0; QRDK_ENEMIES[index].character.name != QRDK_INVALID.name; ++index) {
        const QrdkEnemy &enemy = QRDK_ENEMIES[index];
        if (work->defeated[index] || enemy.door.name != work->door.name)
            continue;
        Vector3 door;
        std::uint32_t yaw;
        QrdkGetDoorPoint(work->door, &door, &yaw);
        Vector3 position{-sin_fixed16(static_cast<std::int16_t>(yaw)) * enemy.z, door.y + 0.30000001f, -cos_fixed16(static_cast<std::uint16_t>(yaw)) * enemy.z};
        if (std::fabs(position.x) < 0.00000011920929f)
            position.x = 0;
        if (std::fabs(position.z) < 0.00000011920929f)
            position.z = 0;
        position.x -= cos_fixed16(static_cast<std::uint16_t>(yaw)) * enemy.x;
        position.z += sin_fixed16(static_cast<std::int16_t>(yaw)) * enemy.x;
        if (std::fabs(position.x) < 0.00000011920929f)
            position.x = 0;
        if (std::fabs(position.z) < 0.00000011920929f)
            position.z = 0;
        position.x += door.x;
        position.z += door.z;
        if (std::fabs(position.x) < 0.00000011920929f)
            position.x = 0;
        if (std::fabs(position.z) < 0.00000011920929f)
            position.z = 0;
        const Vector3i rotation{0, static_cast<std::int32_t>(yaw + enemy.yaw), 0};
        EV_AT_SetPos(enemy.character, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
        EV_AT_SetAngY(enemy.character, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
        EV_SetCharaControllerFlags(enemy.character, 0, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(enemy.character, 1, NpcCharacterControllerState::Enabled);
    }
    HLib::SetCurrentTaskTo(QrdkStartBattleCallback);
}

void QrdkStartBattleCallback(void *data) {
    QrdkWork *const work = static_cast<QrdkWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(QrdkExitCallback);
        return;
    }
    Vector3 position;
    QrdkGetDoorPoint(work->door, &position, nullptr);
    shenStartFGST(10, 10, 0, 0);
    vfmDispatchControl(FightControlOp::Package, shen_name{0}, reinterpret_cast<std::uintptr_t>("QRDK"), 1);
    shenInitFightData(ctrlGetScenarioChara(QRDK_RYO), nullptr, 0, 0, 0, 0);
    for (std::size_t index = 0; QRDK_ENEMIES[index].character.name != QRDK_INVALID.name; ++index) {
        const QrdkEnemy &enemy = QRDK_ENEMIES[index];
        if (work->defeated[index] || enemy.door.name != work->door.name)
            continue;
        shenInitFightData(ctrlGetScenarioChara(enemy.character), nullptr, 1, 1, 40, static_cast<std::uint8_t>(enemy.style));
        vfUpdateFlagsFor(ctrlGetScenarioChara(enemy.character), 0, 0x400000, 128);
        vfUpdateFlagsFor(ctrlGetScenarioChara(enemy.character), 0, 0x200000, 2048);
        vfUpdateFlagsFor(ctrlGetScenarioChara(enemy.character), 0, 0x8000000, 1);
    }
    vfCopyPosFromEntity(0, nullptr, position.x, position.z, 7);
    vfmQueueFightStart(2, 2, 1, 1, -1);
    HLib::SetCurrentTaskTo(QrdkBattleCallback);
}

void QrdkBattleCallback(void *data) {
    if (g_fightActive || filmCheckFadeTrackColour())
        return;
    QrdkWork *const work = static_cast<QrdkWork *>(data);
    if (work->flags & 3U) {
        work->flags &= ~3U;
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        return;
    }
    for (std::size_t index = 0; QRDK_ENEMIES[index].character.name != QRDK_INVALID.name; ++index) {
        const QrdkEnemy &enemy = QRDK_ENEMIES[index];
        if (enemy.door.name != work->door.name)
            continue;
        EV_AT_SetPos(enemy.character, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3{10000, 10000, 10000});
        EV_AT_SetAngY(enemy.character, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, 0, 0});
        work->defeated[index] = 1;
    }
    FinishLoading();
    if (GetBtlStatus(ctrlGetScenarioChara(QRDK_RYO)) == 3) {
        work->flags |= 3U;
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        QrdkPlaceOnRoute(work);
        HLib::SetCurrentTaskTo(QrdkRunCallback);
    } else
        HLib::SetCurrentTaskTo(QrdkRetryCallback);
}

void QrdkRetryCallback(void *data) {
    QrdkWork *const work = static_cast<QrdkWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(QrdkExitCallback);
        return;
    }
    for (std::size_t index = 0; QRDK_ENEMIES[index].character.name != QRDK_INVALID.name; ++index) {
        EV_AT_SetPos(QRDK_ENEMIES[index].character, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3{10000, 10000, 10000});
        EV_AT_SetAngY(QRDK_ENEMIES[index].character, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, 0, 0});
        work->defeated[index] = 0;
    }
    QrdkQueueSerif("りとらい", 0, 50, nullptr);
    Vector3 position;
    EV_GetCharaPosition(QRDK_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    position.x = QRDK_ROUTE[0].x;
    position.z = QRDK_ROUTE[0].z;
    EV_AT_SetPos(QRDK_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    work->flags |= 3U;
    work->routeCounter = 1;
    SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    QrdkPlaceOnRoute(work);
    HLib::SetCurrentTaskTo(QrdkRunCallback);
}

void QrdkExitCallback(void *data) {
    if (filmCheckFadeTrackColour())
        return;
    QrdkWork *const work = static_cast<QrdkWork *>(data);
    HLib::Task *const parent = work ? work->parent : nullptr;
    const std::uint32_t flags = work ? work->flags : 0;
    const shen_name door = work ? work->door : shen_name{0};
    if (flags & 3U) {
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        if (work)
            work->flags &= ~3U;
        return;
    }
    SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    const bool ryoHasMotion = scenFindMotionwork(QRDK_RYO) != nullptr;
    EV_SetCharacterAlphaEnabled(QRDK_RYO, true);
    if (ryoHasMotion)
        mtwkSetExtraFlag(scenFindMotionwork(QRDK_RYO), 6, 0);
    EV_SetCharaControllerFlags(QRDK_RYO, 0, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(QRDK_RYO, 1, NpcCharacterControllerState::Enabled);
    if (ryoHasMotion)
        EV_SetRefBoardParamsFor(QRDK_RYO, 0);
    if (flags & 4U)
        ReleaseDoor(std::bit_cast<std::int32_t>(door.name), QRDK_RYO);
    for (std::size_t index = 0; QRDK_CHARACTERS[index].name != QRDK_INVALID.name; ++index) {
        const shen_name character = QRDK_CHARACTERS[index];
        const bool hasMotion = scenFindMotionwork(character) != nullptr;
        EV_SetCharacterAlphaEnabled(character, true);
        if (hasMotion)
            mtwkSetExtraFlag(scenFindMotionwork(character), 6, 0);
        EV_SetCharaControllerFlags(character, 0, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(character, 1, NpcCharacterControllerState::Enabled);
        if (hasMotion)
            EV_SetRefBoardParamsFor(character, 0);
        EV_npcLockUnlock(character);
    }
    if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("QRDK"), 0))
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QRDK"), 0);
    EV_CleanupMapControlTask();
    HLib::MarkCurrentTaskForCleanup();
    if (parent)
        HLib::ResolveDeferredCallback(parent);
    SetPlayerControlEnabled(1);
}

const ScriptAreaDefinition qrdkScriptDefinition{.areaId = AREA_QRDK, .field04 = 0, .init = QrdkInitNullsub, .main = QrdkMain};
} // namespace shenmue::s2::areas
#endif
