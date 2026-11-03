
#include "s2/areas/area_loop_dc.h"
#if !defined(SHENMUE_AREA_LOOP_HAND_POSE_TABLE)
#if !defined(SHENMUE_AREA_LOOP_VOICE_REVISION)
#define SHENMUE_AREA_LOOP_VOICE_REVISION 0
#endif

void QueueAreaLoopVoice(const char *voice, int dialogue, int delay, int duration, AreaLoopCompletion completion)
{
    HLibTask *task = 0;
    do {
        AreaLoopVoiceWork *work;
        int length;
        if (!voice)
            break;
        length = 0;
        while (voice[length])
            length++;
        task = FTBL.HLib_EnqueueTaskWithParameter(AreaLoopVoiceCallback, 4, 11, sizeof(AreaLoopVoiceWork) + length + 1, TASK_EVNT);
        if (!task)
            break;
        work = (AreaLoopVoiceWork *)FTBL.HLib_GetTaskParameter(task);
        if (!work)
            break;
        work->parent = FTBL.HLib_GetCurrentTask();
        work->conversation = 0;
        work->flags = dialogue == 0;
        work->duration = duration;
        work->elapsed = 0;
        work->completion = completion;
        work->voice = (char *)(work + 1);
        for (; length >= 0; length--)
            work->voice[length] = voice[length];
        if (delay > 0) {
            FTBL.HLib_SetupTimedTask(task, delay);
            task = 0;
        } else {
            work->flags |= 0x80000000;
            AreaLoopVoiceCallback(work);
            if (work->flags & 0x80000000) {
                task = 0;
                work->flags &= 0x7fffffff;
            }
        }
    } while (0);
    if (task)
        FTBL.HLib_DestroyTask(task);
}

void AreaLoopVoiceCallback(void *data)
{
    AreaLoopVoiceWork *work = (AreaLoopVoiceWork *)data;
    int done = 1;
    do {
        if (!work)
            break;
        if (!(work->flags & 2)) {
            if (work->flags & 1)
                FTBL.dbgPrintEventMessage(work->voice);
            else
                work->conversation = FTBL.EV_PlayDialogue(work->voice);
            if (work->completion)
                work->completion(0, 0, work->parent);
            work->flags |= 2;
        }
        if (work->completion)
            work->completion(2, work->elapsed, work->parent);
        work->elapsed++;
        if (work->duration < 0) {
#if SHENMUE_AREA_LOOP_VOICE_REVISION == 0
            if ((work->flags & 1) == 0) {
                if (work->conversation && FTBL.cvsHasValidConverseData(work->conversation))
                    done = 0;
                else
                    work->conversation = 0;
            }
#else
            if (work->flags & 1)
                break;
            if (work->conversation && FTBL.cvsHasValidConverseData(work->conversation))
                goto wait;
#if SHENMUE_AREA_LOOP_VOICE_REVISION == 3
            if (work->completion)
                work->completion(1, 0, work->parent);
#endif
            work->conversation = 0;
#if SHENMUE_AREA_LOOP_VOICE_REVISION == 1 || SHENMUE_AREA_LOOP_VOICE_REVISION == 3
            break;
#endif
#endif
        } else {
            if (work->duration >= work->elapsed)
                goto wait;
            if (work->flags & 1) {
                FTBL.ctrlStopSubtitles();
            } else {
                if (work->conversation && FTBL.cvsHasValidConverseData(work->conversation))
                    FTBL.scenCvsCtrlFree(work->conversation);
                work->conversation = 0;
            }
        }
        if (work->completion)
            work->completion(1, 0, work->parent);
        break;
    wait:
        done = 0;
    } while (0);
    if (done) {
        if (!work || !(work->flags & 0x80000000))
            FTBL.HLib_DestroyCurrentEventTask();
        else
            work->flags &= 0x7fffffff;
    }
}

void QueueAreaLoopCamera(const char *area, int camera, shen_name character, int delay, AreaLoopCompletion completion)
{
    HLibTask *task = 0;
    do {
        AreaLoopCameraWork *work;
        int length;
        if (area && area[0]) {
            length = 0;
            while (area[length])
                length++;
        } else {
            length = 0;
        }
        task = FTBL.HLib_EnqueueTaskWithParameter(AreaLoopCameraCallback, 4, 11, sizeof(AreaLoopCameraWork) + length + 1, TASK_EVNT);
        if (!task)
            break;
        work = (AreaLoopCameraWork *)FTBL.HLib_GetTaskParameter(task);
        if (!work)
            break;
        work->parent = FTBL.HLib_GetCurrentTask();
        work->flags = 0;
        work->camera = camera;
        work->character = character;
        work->completion = completion;
        work->area = (char *)(work + 1);
        for (; length >= 0; length--)
            work->area[length] = area[length];
        if (delay > 0) {
            FTBL.HLib_SetupTimedTask(task, delay);
            task = 0;
        } else {
            work->flags |= 0x80000000;
            AreaLoopCameraCallback(work);
            if (work->flags & 0x80000000) {
                task = 0;
                work->flags &= 0x7fffffff;
            }
        }
    } while (0);
    if (task)
        FTBL.HLib_DestroyTask(task);
}

void AreaLoopCameraCallback(void *data)
{
    AreaLoopCameraWork *work = (AreaLoopCameraWork *)data;
    do {
        if (!work)
            break;
        if (work->area && work->area[0])
            FTBL.EV_ControlPackageResources(2, 4, work->area, 0, 0);
        if (work->character)
            FTBL.EVS_LoopCamera(work->camera, work->character, work->character);
        else
            FTBL.EVS_LoopCamera(work->camera, 0, 0);
        if (work->completion)
            work->completion(0, 0, work->parent);
    } while (0);
    if (!work || !(work->flags & 0x80000000))
        FTBL.HLib_DestroyCurrentEventTask();
    else
        work->flags &= 0x7fffffff;
}

void QueueAreaLoopMotion(shen_name character, unsigned int motion, int delay, int duration, float startFrame, float endFrame, float speed,
                         float frameTime, AreaLoopCompletion completion, unsigned int flags)
{
    HLibTask *task = 0;
    while (character && (task = FTBL.HLib_EnqueueTaskWithParameter(AreaLoopMotionCallback, 4, 11, sizeof(AreaLoopMotionWork), TASK_EVNT))) {
        AreaLoopMotionWork *work = (AreaLoopMotionWork *)FTBL.HLib_GetTaskParameter(task);
        if (!work)
            break;
        work->parent = FTBL.HLib_GetCurrentTask();
        work->character = character;
        work->flags = flags & 0x3fffffff;
        work->motion = motion;
        work->duration = duration;
        work->elapsed = 0;
        work->startFrame = startFrame;
        work->endFrame = endFrame;
        work->speed = speed;
        work->frameTime = frameTime;
        work->completion = completion;
        if (delay > 0) {
            FTBL.HLib_SetupTimedTask(task, delay);
            task = 0;
        } else {
            work->flags |= 0x80000000;
            AreaLoopMotionCallback(work);
            if (work->flags & 0x80000000) {
                task = 0;
                work->flags &= 0x7fffffff;
            }
        }
        break;
    }
    do {
        if (task)
            FTBL.HLib_DestroyTask(task);
    } while (0);
}

void AreaLoopMotionCallback(void *data)
{
    AreaLoopMotionWork *work = (AreaLoopMotionWork *)data;
    int done = 1;
    do {
        shen_name character;
        if (!work)
            break;
        character = work->character;
        if (!(work->flags & 0x40000000)) {
            FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(character), work->motion, 0);
            if (work->flags & 1)
                FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(character), 69, 0, 0);
            if (work->flags & 2) {
                FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(character), 0);
            } else {
                FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(character), 1);
                FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(character), (unsigned int)work->frameTime, 0);
            }
            if (work->flags & 8)
                FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(character), 66, 0, 0);
#if defined(SHENMUE_AREA_LOOP_MOTION_FRAMES)
            if (__builtin_fabs(work->startFrame) >= 0.00000011920929f) {
                FTBL.SetMotionLayerLoopStartTime(FTBL.scenFindMotionwork(character), work->startFrame, 0);
                FTBL.SetMotionRegionEvalTime(FTBL.scenFindMotionwork(character), work->startFrame, 0);
            }
            if (work->endFrame > 0)
                FTBL.SetMotionLayerEndFrame(FTBL.scenFindMotionwork(character), work->endFrame, 0);
            FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(character), work->speed, 0);
#endif
            if (work->completion)
                work->completion(0, character, work->parent);
            work->flags |= 0x40000000;
        }
        FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(character), work->flags & 4);
        if (work->completion)
            work->completion(2, work->elapsed, work->parent);
        if ((work->duration < ++work->elapsed && !(work->flags & 16)) ||
            (FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(character), 65, 1, 0) && (work->flags & 16))) {
            if (work->completion)
                work->completion(1, 0, work->parent);
            break;
        }
        done = 0;
    } while (0);
    if (done) {
        if (!work || !(work->flags & 0x80000000))
            FTBL.HLib_DestroyCurrentEventTask();
        else
            work->flags &= 0x7fffffff;
    }
}

void QueueAreaLoopPosition(shen_name character, int delay, int local, const Vector3 *position, const Vector3i *rotation, AreaLoopCompletion completion)
{
    HLibTask *task = 0;
    while (character && (position || rotation) &&
           (task = FTBL.HLib_EnqueueTaskWithParameter(AreaLoopPositionCallback, 4, 11, sizeof(AreaLoopPositionWork), TASK_EVNT))) {
        AreaLoopPositionWork *work = (AreaLoopPositionWork *)FTBL.HLib_GetTaskParameter(task);
        unsigned int flags;
        if (!work)
            break;
        flags = 0;
        work->character = character;
        if (position) {
            work->position = *position;
            flags = 1;
        }
        if (rotation) {
            work->rotation.x = rotation->x;
            work->rotation.y = rotation->y;
            work->rotation.z = rotation->z;
            flags |= 2;
        }
        if (local)
            flags |= 4;
        work->parent = FTBL.HLib_GetCurrentTask();
        work->flags = flags;
        work->completion = completion;
        if (delay > 0) {
            FTBL.HLib_SetupTimedTask(task, delay);
        } else {
            work->flags |= 0x80000000;
            AreaLoopPositionCallback(work);
            if (!(work->flags & 0x80000000))
                break;
            work->flags &= 0x7fffffff;
        }
        task = 0;
        break;
    }
    if (task)
        FTBL.HLib_DestroyTask(task);
}

void AreaLoopPositionCallback(void *data)
{
    AreaLoopPositionWork *work = (AreaLoopPositionWork *)data;
    int done = 1;
#if defined(SHENMUE_AREA_LOOP_SOUND)
    do {
        int moved;
        if (!work)
            break;
        done = 0;
        moved = 0;
#else
    if (work) {
        done = 0;
#endif
        if (work->flags & 1) {
            if (work->flags & 4)
                FTBL.EV_AT_SetPos(work->character, 0x38000000, &work->position);
            else
                FTBL.EV_AT_SetPos(work->character, 0x78000000, &work->position);
#if defined(SHENMUE_AREA_LOOP_SOUND)
            moved = 1;
#endif
        }
        if (work->flags & 2) {
            if (work->flags & 4)
                FTBL.EV_AT_SetAngY_0(work->character, 0x38000000, &work->rotation, 0);
            else
                FTBL.EV_AT_SetAngY_0(work->character, 0x78000000, &work->rotation, 0);
#if defined(SHENMUE_AREA_LOOP_SOUND)
            moved = 1;
#endif
        }
#if defined(SHENMUE_AREA_LOOP_SOUND)
        if (moved)
            FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(work->character), 17, 0);
#endif
        done = 1;
        if (work->completion)
            work->completion(0, work->character, work->parent);
#if defined(SHENMUE_AREA_LOOP_SOUND)
    } while (0);
#else
    }
#endif
    if (done) {
        if (!work || !(work->flags & 0x80000000))
            FTBL.HLib_DestroyCurrentEventTask();
        else
            work->flags &= 0x7fffffff;
    }
}

void QueueAreaLoopAttachment(shen_name character, int bone, shen_name target, int delay, int duration, const Vector3 *position,
                             const Vector3i *rotation, AreaLoopCompletion completion)
{
    HLibTask *task = 0;
    while (character && target && position && rotation &&
           (task = FTBL.HLib_EnqueueTaskWithParameter(AreaLoopAttachmentCallback, 4, 11, sizeof(AreaLoopAttachmentWork), TASK_EVNT))) {
        AreaLoopAttachmentWork *work = (AreaLoopAttachmentWork *)FTBL.HLib_GetTaskParameter(task);
        if (!work)
            break;
        work->parent = FTBL.HLib_GetCurrentTask();
        work->elapsed = 0;
        work->character = character;
        work->bone = bone;
        work->target = target;
        work->flags = 0;
        work->position = *position;
        work->rotation.x = rotation->x;
        work->rotation.y = rotation->y;
        work->rotation.z = rotation->z;
        work->duration = duration;
        work->completion = completion;
        if (delay > 0) {
            FTBL.HLib_SetupTimedTask(task, delay);
        } else {
            work->flags |= 0x80000000;
            AreaLoopAttachmentCallback(work);
            if (!(work->flags & 0x80000000))
                break;
            work->flags &= 0x7fffffff;
        }
        task = 0;
        break;
    }
    if (task)
        FTBL.HLib_DestroyTask(task);
}

void AreaLoopAttachmentCallback(void *data)
{
    AreaLoopAttachmentWork *work = (AreaLoopAttachmentWork *)data;
    int done = 1;
    if (work) {
        if (!(work->flags & 0x40000000)) {
            FTBL.AttachScenarioCharacter(work->target, work->character, work->bone, &work->position, &work->rotation);
            if (work->completion)
                work->completion(0, 0, work->parent);
            work->flags |= 0x40000000;
        }
        if (work->completion)
            work->completion(2, work->elapsed, work->parent);
        if (++work->elapsed > work->duration) {
            done = 1;
            FTBL.ResetScenarioCharacterAttachment(work->target);
            if (work->completion)
                work->completion(done, 0, work->parent);
        } else {
            done = 0;
        }
    }
    if (done) {
        if (!work || !(work->flags & 0x80000000))
            FTBL.HLib_DestroyCurrentEventTask();
        else
            work->flags &= 0x7fffffff;
    }
}

void SetAreaLoopCharacterControl(shen_name character, int scripted)
{
    if (character) {
        int hasMotion = FTBL.scenFindMotionwork(character) != 0;
        if (scripted) {
#if SHENMUE_AREA_LOOP_CONTROL_REVISION == 1
            float reflector[4];
            reflector[0] = 1;
            reflector[1] = 1;
            reflector[2] = 1;
            reflector[3] = 1;
            FTBL.SetRefBoardParamsForCharaById(character, 1);
            FTBL.SetScenarioReflectorBoardValues(character, reflector);
            FTBL.EV_SetCharaControllerFlags(character, 1, 0);
            FTBL.EV_SetCharaControllerFlags(character, 0, 0);
            if (hasMotion) {
                FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 2);
                FTBL.EV_SetCharacterAlphaEnabled(character, 0);
            }
        } else {
            if (hasMotion) {
                FTBL.EV_SetCharacterAlphaEnabled(character, 1);
                FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 0);
            }
            FTBL.EV_SetCharaControllerFlags(character, 0, 1);
            FTBL.EV_SetCharaControllerFlags(character, 1, 1);
            FTBL.SetRefBoardParamsForCharaById(character, 0);
        }
#else
            FTBL.EV_SetCharaControllerFlags(character, 1, 0);
            FTBL.EV_SetCharaControllerFlags(character, 0, 0);
            if (hasMotion) {
                float reflector[4];
                reflector[0] = 1;
                reflector[1] = 1;
                reflector[2] = 1;
                reflector[3] = 1;
                FTBL.SetRefBoardParamsForCharaById(character, 1);
                FTBL.SetScenarioReflectorBoardValues(character, reflector);
                FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 2);
                FTBL.EV_SetCharacterAlphaEnabled(character, 0);
            }
        } else {
            if (hasMotion) {
                FTBL.EV_SetCharacterAlphaEnabled(character, 1);
                FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 0);
                FTBL.SetRefBoardParamsForCharaById(character, 0);
            }
            FTBL.EV_SetCharaControllerFlags(character, 0, 1);
            FTBL.EV_SetCharaControllerFlags(character, 1, 1);
        }
#endif
    }
}

int QueueAreaLoopNpcs(const shen_name *characters, AreaLoopCompletion completion)
{
    HLibTask *task = 0;
    int result = 0;
    AreaLoopNpcWork *work;
    shen_name *copy;
    int count;
    do {
        if (!characters)
            break;
        for (count = 0; characters[count]; count++)
            ;
        task = FTBL.HLib_EnqueueTaskWithParameter(AreaLoopNpcCallback, 4, 11, sizeof(AreaLoopNpcWork) + count * 4, TASK_EVNT);
        if (!task)
            break;
        work = (AreaLoopNpcWork *)FTBL.HLib_GetTaskParameter(task);
        if (!work)
            break;
        copy = (shen_name *)(work + 1);
        work->parent = FTBL.HLib_GetCurrentTask();
        work->previousLoadState = FTBL.GetNpcNewLoadState();
        work->remaining = count;
        work->characters = copy;
        work->completion = completion;
        FTBL.EV_NPC_NewLoadSw_0(0);
        FTBL.EV_ResetFreeTalkFlags();
        for (count = 0; characters[count]; count++)
            copy[count] = characters[count];
        task = 0;
        result = 1;
    } while (0);
    if (task)
        FTBL.HLib_DestroyTask(task);
    return result;
}

void AreaLoopNpcCallback(void *data)
{
    AreaLoopNpcWork *work = (AreaLoopNpcWork *)data;
    int done;
    if (work) {
        done = 1;
        if (work->remaining) {
            shen_name *characters = work->characters;
            FTBL.CheckWarmStartNpcRoute(characters[work->remaining - 1]);
            while (work->remaining) {
                int stop = 1;
                unsigned int state = FTBL.npcStreamOrLoadHuman(characters[work->remaining - 1]);
                if (state) {
                    if (state == 1) {
                        SetAreaLoopCharacterControl(characters[work->remaining - 1], 1);
                        work->remaining--;
                        stop = 0;
                    } else {
                        work->remaining--;
                    }
                }
                if (stop)
                    break;
            }
            done = work->remaining == 0;
        }
        if (work->completion)
            work->completion(2, work->remaining, work->parent);
    } else {
        done = 1;
    }
    if (done) {
        if (work) {
            if (work->previousLoadState)
                FTBL.EV_NPC_NewLoadSw_0(1);
            if (work->completion)
                work->completion(1, 0, work->parent);
        }
        FTBL.HLib_DestroyCurrentEventTask();
    }
}

void RestoreAreaLoopCharacters(const shen_name *characters, const shen_name *retained)
{
    int index;
    if (!characters)
        return;
    for (index = 0; characters[index]; index++) {
        if (FTBL.DoesScenarioCharacterExist(characters[index])) {
            int keep;
            SetAreaLoopCharacterControl(characters[index], 0);
            keep = 0;
            if (retained)
                for (keep = 0; retained[keep] && retained[keep] != characters[index]; keep++)
                    ;
            if (!retained || !retained[keep])
                FTBL.npcFree(characters[index]);
            FTBL.npcLockUnlock(characters[index]);
        }
    }
}

int QueueAreaLoopAuthor(const char *package, shen_name author, int delay, int endFrame, AreaLoopCompletion completion)
{
    HLibTask *task;
    int result = 0;
    do {
        AreaLoopAuthorWork *work;
        int length;
        if (package && package[0]) {
            length = 0;
            while (package[length])
                length++;
#if defined(SHENMUE_AREA_LOOP_SOUND) || defined(SHENMUE_AREA_LOOP_AUTHOR_COUNTER)
            length++;
#endif
        } else {
            length = 0;
        }
        task = FTBL.HLib_EnqueueTaskWithParameter(AreaLoopAuthorCallback, 4, 11, sizeof(AreaLoopAuthorWork) + length, TASK_EVNT);
        if (!task)
            break;
        work = (AreaLoopAuthorWork *)FTBL.HLib_GetTaskParameter(task);
        if (!work)
            break;
        work->parent = FTBL.HLib_GetCurrentTask();
        work->flags = 0;
        work->package = (char *)(work + 1);
        work->author = author;
#if defined(SHENMUE_AREA_LOOP_AUTHOR_COUNTER)
        work->frame = 0;
#endif
        work->endFrame = endFrame;
        work->completion = completion;
        if (length) {
            char *destination = work->package;
            for (; length >= 0; length--)
                destination[length] = package[length];
        } else {
            work->package = 0;
            work->flags |= 1;
        }
        if (delay <= 0) {
            work->flags |= 0x80000000;
            AreaLoopAuthorCallback(work);
            result = 1;
            if (!(work->flags & 0x80000000))
                break;
            work->flags &= 0x7fffffff;
        } else {
            FTBL.HLib_SetupTimedTask(task, delay);
            result = 1;
        }
        task = 0;
    } while (0);
    if (task)
        FTBL.HLib_DestroyTask(task);
    return result;
}

void AreaLoopAuthorCallback(void *data)
{
    AreaLoopAuthorWork *work = (AreaLoopAuthorWork *)data;
    int done = 1;
    do {
        int frame, finished;
        if (!work)
            break;
        done = 0;
        if ((work->flags & 1) == 0) {
            if (!FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)work->package, 0))
                break;
            FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
            work->flags |= 1;
        }
        if (!(work->flags & 2)) {
            FTBL.EV_authSceneCtrl(work->author);
            work->flags |= 2;
            if (work->completion)
                work->completion(0, 0, work->parent);
        }
#if defined(SHENMUE_AREA_LOOP_AUTHOR_COUNTER)
        frame = work->frame++;
#else
        frame = FTBL.EV_authSceneCtrl(-6);
#endif
        if (work->completion)
            work->completion(2, frame, work->parent);
        finished = FTBL.EV_authSceneCtrl(-1) == 0;
#if defined(SHENMUE_AREA_LOOP_SOUND) || defined(SHENMUE_AREA_LOOP_AUTHOR_COUNTER)
        done = finished || (frame >= work->endFrame && work->endFrame >= 0);
#else
        done = finished || frame >= work->endFrame;
#endif
        if (done) {
            if (work->completion)
                work->completion(1, 0, work->parent);
            if (!finished)
                FTBL.EV_authSceneCtrl(-4);
            if (work->package && FTBL.EV_ControlPackageResources(0, 5, work->package, 0, 0))
                FTBL.EV_ControlPackageResources(0, 6, work->package, 0, 0);
        }
    } while (0);
    if (done) {
        if (!(work->flags & 0x80000000))
            FTBL.HLib_DestroyCurrentEventTask();
        else
            work->flags &= 0x7fffffff;
    }
}

void QueueAreaLoopLookAt(shen_name character, shen_name target, int bone, const Vector3 *position, int delay, int duration,
                         AreaLoopCompletion completion)
{
    HLibTask *task = 0;
    while (character && (task = FTBL.HLib_EnqueueTaskWithParameter(AreaLoopLookAtCallback, 4, 11, sizeof(AreaLoopLookAtWork), TASK_EVNT))) {
        AreaLoopLookAtWork *work = (AreaLoopLookAtWork *)FTBL.HLib_GetTaskParameter(task);
        if (!work)
            break;
        work->parent = FTBL.HLib_GetCurrentTask();
        work->flags = 0;
        work->character = character;
        work->target = target;
        work->bone = bone;
        if (position) {
            work->character = 0;
            work->position = *position;
        }
        work->elapsed = 0;
        work->duration = duration;
        work->completion = completion;
        if (delay > 0) {
            FTBL.HLib_SetupTimedTask(task, delay);
            task = 0;
        } else {
            work->flags |= 0x80000000;
            AreaLoopLookAtCallback(work);
            if (work->flags & 0x80000000) {
                task = 0;
                work->flags &= 0x7fffffff;
            }
        }
        break;
    }
    do {
        do {
            if (task)
                FTBL.HLib_DestroyTask(task);
        } while (0);
    } while (0);
}

void AreaLoopLookAtCallback(void *data)
{
    AreaLoopLookAtWork *work = (AreaLoopLookAtWork *)data;
    int done = 1;
    if (work) {
        Vector3 position;
        if (!work->character)
            position = work->position;
        else if (work->bone >= 0)
            FTBL.GetMotionBonePosition(FTBL.scenFindMotionwork(work->target), work->bone, &position);
        else
            FTBL.GetCharaPosition(work->target, -1, &position, 0x40000000);
        FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(work->character), &position, 3);
        FTBL.SetEyeToChara(FTBL.ctrlGetCharaByName(work->character), 0, 16, 0);
        FTBL.SetEyeToChara(FTBL.ctrlGetCharaByName(work->character), &position, 0, 2);
        if (!(work->flags & 0x40000000)) {
            if (work->completion)
                work->completion(0, 0, work->parent);
            work->flags |= 0x40000000;
        }
        if (work->completion)
            work->completion(2, work->elapsed, work->parent);
        if (++work->elapsed > work->duration) {
            done = 1;
            FTBL.SetEyeToChara(FTBL.ctrlGetCharaByName(work->character), 0, 16, 0);
            FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(work->character), 0, 0);
            if (work->completion)
                work->completion(done, 0, work->parent);
        } else {
            done = 0;
        }
    }
    if (done) {
        if (!work || !(work->flags & 0x80000000))
            FTBL.HLib_DestroyCurrentEventTask();
        else
            work->flags &= 0x7fffffff;
    }
}

int QueueAreaLoopPackage(const char *folder, const char *package, AreaLoopCompletion completion)
{
    int result = 0;
    HLibTask *task = 0;
    do {
        AreaLoopPackageWork *work;
        char *packageCopy, *folderCopy;
        int packageLength, folderLength, index;
        if (!package || !package[0] || !folder || !folder[0])
            break;
        packageLength = 0;
        while (package[packageLength])
            packageLength++;
        packageLength++;
        folderLength = 0;
        while (folder[folderLength])
            folderLength++;
        packageLength = (packageLength + 3) & ~3;
        task = FTBL.HLib_EnqueueTaskWithParameter(AreaLoopPackageCallback, 4, 11, sizeof(AreaLoopPackageWork) + packageLength + 1 + folderLength,
                                                  TASK_EVNT);
        if (!task)
            break;
        work = (AreaLoopPackageWork *)FTBL.HLib_GetTaskParameter(task);
        if (!work)
            break;
        packageCopy = (char *)(work + 1);
        folderCopy = packageCopy + packageLength;
        work->parent = FTBL.HLib_GetCurrentTask();
        work->flags = 0;
        work->package = packageCopy;
        work->folder = folderCopy;
        work->completion = completion;
        for (index = 0; package[index]; index++)
            packageCopy[index] = package[index];
        packageCopy[index] = 0;
        for (index = 0; folder[index]; index++)
            folderCopy[index] = folder[index];
        folderCopy[index] = 0;
        result = 1;
        work->flags |= 0x80000000;
        AreaLoopPackageCallback(work);
        if (!(work->flags & 0x80000000))
            break;
        work->flags &= 0x7fffffff;
        task = 0;
    } while (0);
    if (task)
        FTBL.HLib_DestroyTask(task);
    return result;
}

void AreaLoopPackageCallback(void *data)
{
    AreaLoopPackageWork *work = (AreaLoopPackageWork *)data;
    int done = 1;
    do {
        if (!work)
            break;
        if (!(work->flags & 0x40000000)) {
            work->flags |= 0x40000000;
            if (work->completion)
                work->completion(0, 0, work->parent);
        }
        if (work->completion)
            work->completion(2, 0, work->parent);
        if (FTBL.EV_ControlPackageResources(0, 0, work->folder, (int)work->package, 0))
            FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        else
            done = 0;
    } while (0);
    if (done) {
        if (work && work->completion)
            work->completion(1, 0, work->parent);
        if (!work || !(work->flags & 0x80000000))
            FTBL.HLib_DestroyCurrentEventTask();
        else
            work->flags &= 0x7fffffff;
    }
}

#if defined(SHENMUE_AREA_LOOP_SOUND)
void QueueAreaLoopSound(unsigned int sound, int delay)
{
    if (delay > 0) {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(AreaLoopSoundCallback, 4, 11, 4, TASK_EVNT);
        if (task) {
            unsigned int *work = (unsigned int *)FTBL.HLib_GetTaskParameter(task);
            if (work) {
                FTBL.HLib_SetupTimedTask(task, delay);
                *work = sound;
            } else {
                FTBL.HLib_DestroyTask(task);
            }
        }
    } else {
        FTBL.Loop3DSoundEffect(sound);
    }
}

void AreaLoopSoundCallback(void *data)
{
    if (data)
        FTBL.Loop3DSoundEffect(*(unsigned int *)data);
    FTBL.HLib_DestroyCurrentEventTask();
}
#endif

int ResolveAreaLoopParent(int phase, int value, HLibTask *parent)
{
    if (phase == 1 && parent) {
        FTBL.HLib_ResolveDeferredCallback(parent);
        FTBL.HLib_SetupTimedTask(parent, 1);
    }
    return 1;
}
#endif
#if defined(SHENMUE_AREA_LOOP_HAND_POSES) || defined(SHENMUE_AREA_LOOP_HAND_POSE_TABLE)

const int areaLoopHandPoses[52][57] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0},
    {0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -16748, 0, 0, 0, 0, 0,
     -11832, 0, 0, -18386, 0, 0, -15109, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2184, -10012, 0, 0, 0},
    {0, 65172, 53886, 62624, 65172, 61167, 1638, 910, 182, 0, 0, 49517, 0, 182, -23119, 0, 546, -18932, 0, 0, 64444, 0, 0, 49152, 0, 0, -21845, 0, 0,
     -18750, 0, 0, 64990, 0, 0, -21481, 0, 0, -21299, 0, 0, -18568, 0, 0, 64808, 0, 910, -18750, 0, 182, -18750, 0, -4004, -18204, 0, 0, 64808},
    {0, 65172, 51519, 62624, 65172, 58801, 1638, 910, 2548, 0, 0, -18568, 0, 182, -25668, 0, 546, -21481, 0, 0, 61896, 0, 0, -18932, 0, 0, -24393, 0, 0,
     -21299, 0, 0, 62442, 0, 0, -24029, 0, 0, -23847, 0, 0, -21117, 0, 0, 62260, 0, 910, -21299, 0, 182, -21299, 0, -4004, -20753, 0, 0, 62260},
    {0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -2912, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0,
     1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638},
    {0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -13289, 0, 0, -5643, 0, 0, -8009, 0, -182, 1274, 0, 0, -11832, 0, 0, -5279, 0, 0, -8009, 0, 0,
     1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1092},
    {0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1274, 0, 0, -3822, 0, 0, -2730, 0, 0, -1456, 0, 0,
     1456, 0, 182, -3822, 0, 0, -2912, 0, 0, -1638, 0, 182, 1638, 0, -728, -3640, 0, -910, -2730, 0, -546, -1638, 0, 0, 1092},
    {0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0,
     1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638},
    {0, 0, -9102, 0, -2366, -2002, 0, -182, 910, 0, -546, -2366, 0, 0, -2002, 0, 0, -2912, 0, -182, 1274, 0, 0, -14199, 0, 0, -11468, 0, 0, -13653, 0, 0,
     1456, 0, 182, -11286, 0, 0, -12014, 0, 0, -12743, 0, 182, 1638, 0, -728, -10194, 0, -1274, -12196, 0, -910, -10922, 64808, 546, 1638},
    {0, 0, 63716, 0, 63170, 64808, 0, 65172, 64444, 0, 64990, 53704, 0, 910, 58801, 0, 0, 58983, 0, 65354, 1274, 0, 0, 52247, 0, 0, 59529, 0, 0, 55524, 0,
     0, 1092, 0, 182, 56798, 0, 0, 58801, 0, 0, 55706, 0, 182, 1638, 0, 64808, 58983, 0, 64262, 58255, 0, 64626, 54796, 0, 0, 1638},
    {0, 0, 60075, 0, 63170, 63898, 0, 64626, 1092, 0, 64990, 61896, 0, 0, 63170, 0, 0, 64080, 0, 65354, 1638, 0, 0, 64626, 0, 0, 62806, 0, 0, 64262, 0, 0,
     1820, 0, 182, 62806, 0, 0, 63716, 0, 0, 64262, 0, 182, 2184, 0, 64808, 62442, 0, 64626, 62988, 0, 910, 64080, 64990, 910, 2366},
    {2000, 65354, 62444, 1638, 180, 63900, 546, 64719, 3367, 0, 64809, 60258, 0, 64809, 59529, 0, 0, 62624, 0, 0, 64628, 0, 0, 58984, 0, 0, 58073, 0,
     65172, 61898, 0, 0, 65354, 0, 729, 60075, 0, 65173, 57164, 0, 65172, 63172, 0, 0, 0, 0, 547, 56253, 0, 1640, 60075, 0, 64809, 61716, 64080, 182,
     65535},
    {1999, 65354, 62444, 1638, 180, 63900, 546, 64719, 3366, 0, 64810, 60985, 0, 64627, 61894, 0, 0, 64261, 0, 0, 64628, 0, 0, 60075, 0, 0, 59711, 0,
     65172, 63353, 0, 0, 65354, 0, 729, 61166, 0, 65173, 58073, 0, 65172, 64445, 0, 0, 0, 0, 547, 57162, 0, 1639, 61166, 0, 64810, 64081, 64080, 182, 0},
    {1456, 5095, 46606, 58257, 64627, 64810, 2912, 909, 2001, 726, 64080, 46422, 0, 64810, 46240, 0, 364, 49700, 0, 0, 0, 0, 0, 48061, 0, 0, 47879, 0, 0,
     50427, 0, 0, 0, 0, 0, 48970, 0, 0, 47332, 0, 0, 51156, 0, 0, 0, 0, 0, 46058, 0, 0, 47151, 0, 64445, 52611, 0, 0, 64990},
    {0, 0, 53888, 0, 0, 61349, 0, 0, 726, 0, 0, 64080, 0, 0, 364, 0, 1273, 64628, 0, 0, 0, 0, 0, 64080, 0, 0, 364, 0, 64990, 64628, 0, 0, 0, 0, 0, 53888,
     0, 182, 46422, 0, 0, 57344, 0, 18, 0, 2182, 727, 52611, 0, 64627, 47696, 726, 64992, 56253, 726, 91, 65172},
    {2000, 65354, 62444, 1638, 180, 63172, 546, 64719, 3367, 0, 64809, 56618, 0, 64809, 52249, 0, 0, 63352, 0, 0, 64628, 0, 0, 56800, 0, 0, 50793, 0,
     65172, 60442, 0, 0, 65354, 0, 729, 57891, 0, 65173, 49156, 0, 65172, 60988, 0, 0, 0, 0, 547, 51885, 0, 1640, 49155, 0, 64809, 62444, 64080, 182,
     65535},
    {0, 364, 56071, 0, 2731, 910, 0, 62442, 1819, 0, 64990, 58801, 0, 0, 61532, 0, 0, 58984, 0, 0, 1273, 909, 364, 52248, 0, 64990, 60258, 0, 0, 57527, 0,
     0, 1456, 0, 182, 56798, 0, 64990, 59529, 0, 65172, 56798, 0, 182, 1638, 0, 64810, 60258, 1456, 64627, 58801, 0, 64627, 56798, 0, 546, 1638},
    {0, 64810, 54614, 0, 64809, 64081, 0, 0, 1273, 0, 0, 64080, 0, 0, 364, 0, 0, 64628, 0, 0, 0, 0, 0, 53340, 0, 0, 45331, 0, 0, 58255, 0, 0, 0, 0, 0,
     53888, 0, 182, 46422, 0, 0, 57344, 0, 200, 0, 0, 546, 52611, 726, 64444, 47696, 1456, 0, 54797, 0, 273, 65172},
    {0, 0, 46607, 0, 2183, 64444, 0, 0, 1456, 0, 64080, 49155, 0, 909, 44239, 0, 0, 53341, 0, 0, 0, 0, 1456, 54797, 0, 0, 43693, 0, 727, 50974, 0, 0, 0, 0,
     2912, 54798, 0, 182, 44421, 0, 1456, 50246, 0, 0, 0, 2912, 910, 51337, 726, 726, 47517, 0, 2003, 50974, 65518, 65427, 0},
    {1999, 65354, 60806, 1638, 180, 63171, 546, 64719, 3366, 0, 64810, 53706, 0, 64810, 49336, 0, 0, 60076, 0, 0, 64810, 0, 0, 56800, 0, 0, 47516, 0,
     65172, 56619, 0, 0, 65354, 0, 729, 55707, 0, 65173, 49338, 0, 64990, 56256, 0, 0, 0, 0, 0, 56434, 0, 1639, 49882, 0, 183, 59350, 64080, 65354, 64444},
    {0, 64080, 58983, 63716, 3094, 909, 0, 65263, 2912, 0, 727, 60621, 0, 62806, 55160, 5825, 182, 59893, 0, 0, 0, 0, 0, 55340, 0, 727, 48242, 727, 0,
     58438, 0, 0, 0, 0, 64990, 51701, 727, 65354, 49334, 0, 65354, 58620, 0, 0, 64809, 2912, 64080, 53338, 1456, 62806, 49698, 1456, 63170, 55890, 0, 0,
     64809},
    {727, 64809, 60257, 2366, 4369, 63897, 61714, 1456, 3458, 0, 65354, 58074, 0, 0, 56070, 0, 0, 58983, 0, 0, 64627, 364, 182, 60438, 182, 64809, 51157,
     0, 0, 59165, 0, 0, 0, 65354, 727, 60440, 0, 65354, 52248, 0, 64990, 55343, 0, 0, 65354, 0, 64262, 53886, 909, 182, 55889, 364, 64444, 52612, 0, 0,
     64627},
    {0, 65172, 57710, 63716, 2183, 909, 60987, 64263, 1091, 0, 64990, 58801, 0, 0, 59348, 0, 0, 58984, 0, 65354, 3821, 0, 0, 57889, 0, 0, 58619, 0, 0,
     58983, 0, 0, 3094, 0, 546, 60075, 0, 0, 59893, 364, 65172, 59346, 0, 182, 2547, 65354, 729, 60075, 64810, 65173, 60985, 0, 65355, 60257, 0, 364,
     1820},
    {0, 0, 60075, 0, 546, 0, 0, 64445, 2365, 0, 64990, 55344, 0, 0, 59894, 0, 0, 56436, 0, 65354, 726, 0, 0, 56436, 0, 0, 53159, 65354, 65172, 51158, 0,
     65354, 1456, 0, 727, 54069, 65172, 182, 51522, 65354, 64445, 51339, 0, 182, 1273, 1273, 729, 51522, 909, 64809, 51522, 0, 64263, 50428, 182, 182,
     726},
    {0, 0, 53888, 0, 0, 61349, 0, 0, 726, 0, 0, 64080, 0, 0, 364, 0, 64627, 64628, 0, 0, 0, 0, 0, 64080, 0, 0, 364, 0, 64990, 64628, 0, 0, 0, 0, 0, 53888,
     0, 182, 46422, 0, 0, 57344, 0, 18, 0, 2182, 364, 52611, 0, 64809, 47696, 726, 64445, 56253, 726, 91, 65172},
    {0, 1820, 64080, 0, 726, 64262, 0, 63716, 3638, 0, 65354, 62806, 0, 364, 61715, 0, 543, 62444, 0, 0, 0, 0, 0, 62806, 0, 182, 58438, 0, 64990, 62807, 0,
     0, 0, 0, 0, 53888, 0, 182, 46422, 0, 0, 57344, 0, 18, 0, 2182, 364, 52611, 0, 64627, 47696, 726, 64263, 56253, 726, 273, 65172},
    {0, 726, 63534, 0, 0, 65354, 0, 62624, 3822, 0, 0, 60258, 0, 0, 61168, 0, 0, 64080, 0, 546, 1820, 0, 0, 58802, 0, 0, 54252, 0, 0, 59894, 0, 546, 1456,
     0, 0, 56436, 0, 0, 56072, 0, 0, 59166, 0, 546, 909, 0, 0, 52978, 0, 0, 56800, 0, 0, 59348, 0, 546, 1092},
    {0, 726, 63534, 0, 64627, 63534, 0, 62623, 3821, 0, 182, 61896, 0, 0, 58801, 0, 0, 62805, 0, 546, 1820, 0, 0, 60986, 0, 0, 61531, 0, 0, 61714, 0, 546,
     1456, 0, 0, 58983, 0, 0, 62441, 0, 0, 63715, 0, 546, 908, 0, 0, 58074, 0, 0, 64262, 0, 0, 62441, 0, 546, 1091},
    {0, 64445, 0, 0, 62806, 0, 0, 0, 0, 0, 0, 58621, 0, 0, 59349, 0, 0, 0, 0, 0, 0, 0, 0, 59348, 0, 64809, 58802, 0, 0, 0, 0, 0, 0, 0, 0, 59166, 0, 0,
     60441, 0, 0, 0, 0, 0, 0, 0, 0, 59894, 0, 0, 60259, 0, 0, 0, 0, 0, 0},
    {0, 1456, 62442, 0, 3638, 1393, 0, 0, 726, 0, 0, 58620, 0, 0, 56800, 0, 1273, 62080, 0, 0, 0, 0, 0, 55344, 0, 0, 50794, 0, 64990, 57348, 0, 0, 0, 0, 0,
     53888, 0, 182, 46422, 0, 0, 57344, 0, 18, 0, 2182, 0, 52611, 0, 64080, 47696, 726, 64445, 56253, 726, 65445, 65172},
    {0, 359, 1636, 0, 63671, 5095, 0, 64049, 4913, 0, 65311, 56435, 0, 908, 47151, 0, 0, 56252, 0, 0, 0, 0, 0, 59892, 0, 0, 48060, 0, 0, 52430, 0, 0, 0, 0,
     65354, 57709, 0, 182, 51700, 0, 0, 50974, 0, 182, 0, 2912, 64810, 51336, 726, 726, 56980, 0, 64628, 49517, 65518, 437, 0},
    {0, 0, 64123, 0, 0, 1585, 0, 64186, 3262, 0, 0, 61632, 0, 315, 64208, 0, 65401, 62066, 0, 65307, 65177, 0, 494, 52248, 0, 1494, 45432, 0, 65246, 57970,
     0, 65493, 16, 0, 65356, 54486, 0, 1671, 45520, 0, 64628, 54332, 0, 364, 65195, 0, 65261, 57013, 0, 3119, 46901, 0, 63873, 52221, 0, 540, 64231},
    {0, 65170, 59351, 0, 1274, 64442, 0, 60987, 2728, 0, 0, 62806, 0, 364, 64263, 0, 0, 62988, 0, 65354, 1820, 0, 0, 61897, 0, 726, 64265, 0, 65173, 62444,
     0, 182, 1638, 0, 0, 61898, 0, 909, 62444, 0, 64627, 62444, 0, 546, 1636, 0, 0, 59350, 0, 726, 62627, 0, 181, 61898, 0, 546, 908},
    {0, 0, 62260, 0, 0, 182, 0, 63170, 4550, 0, 0, 64990, 0, 0, 58984, 364, 65172, 61168, 364, 65172, 5278, 0, 0, 0, 0, 0, 61896, 1274, 65172, 59348, 364,
     182, 4550, 0, 0, 65172, 0, 0, 61714, 1092, 0, 60622, 364, 364, 3640, 0, 0, 0, 0, 0, 61532, 1274, 364, 61896, 727, 727, 2366},
    {0, 0, 0, 0, 64809, 2912, 0, 61168, 4368, 0, 64809, 0, 0, 0, 62624, 0, 1456, 1456, 0, 0, 0, 0, 0, 0, 0, 0, 61168, 0, 0, 2184, 0, 0, 0, 0, 0, 0, 0, 0,
     60440, 0, 64080, 2912, 0, 0, 0, 0, 1456, 0, 0, 0, 60440, 0, 62624, 2184, 0, 0, 0},
    {0, 543, 57167, 0, 1638, 3820, 0, 60987, 4548, 0, 0, 58620, 0, 65172, 59895, 0, 1820, 1638, 0, 65354, 1820, 0, 0, 59713, 0, 726, 58077, 0, 64627, 730,
     0, 182, 1638, 0, 0, 59350, 0, 909, 58622, 0, 62807, 1640, 0, 364, 1272, 0, 0, 56620, 0, 1091, 59351, 0, 62623, 366, 0, 0, 908},
    {0, 178, 59897, 0, 364, 3820, 0, 59349, 4548, 0, 0, 61168, 0, 65172, 62807, 0, 1638, 1638, 0, 65354, 1820, 0, 0, 61897, 0, 726, 62081, 0, 64627, 730,
     0, 182, 1638, 0, 0, 63718, 0, 909, 60078, 0, 62807, 2732, 0, 364, 1272, 0, 0, 60078, 0, 1091, 61717, 0, 62623, 3278, 0, 0, 908},
    {0, 4368, 50430, 0, 1820, 61714, 0, 60258, 2366, 0, 64990, 0, 0, 182, 63898, 0, 64809, 63534, 0, 64990, 909, 0, 0, 0, 0, 0, 63716, 0, 64627, 64809, 0,
     0, 546, 0, 364, 0, 0, 65354, 62988, 0, 182, 0, 0, 0, 182, 0, 364, 65172, 0, 0, 64444, 0, 1456, 64080, 0, 0, 0},
    {0, 1092, 62442, 0, 3095, 63352, 0, 61168, 3640, 0, 64810, 60986, 0, 0, 59712, 0, 64809, 910, 0, 0, 0, 0, 0, 64626, 0, 0, 61168, 0, 64809, 62441, 0, 0,
     0, 0, 727, 62624, 0, 65354, 62989, 0, 65354, 62806, 0, 0, 0, 0, 364, 61168, 0, 65172, 61897, 0, 64990, 545, 0, 0, 0},
    {0, 909, 55708, 0, 3276, 59347, 0, 64808, 908, 0, 0, 64080, 0, 0, 364, 0, 1819, 64628, 0, 65172, 0, 0, 0, 64080, 0, 0, 0, 0, 364, 366, 0, 65354, 0, 0,
     0, 65172, 0, 65172, 64440, 0, 64262, 180, 0, 18, 65354, 2182, 182, 56069, 0, 1820, 48242, 726, 63717, 59165, 726, 637, 64262},
    {1999, 65354, 59168, 1638, 543, 64810, 546, 63263, 4822, 0, 64810, 56253, 0, 64081, 58800, 0, 727, 363, 0, 0, 64628, 0, 0, 57709, 0, 0, 57345, 0,
     65172, 65355, 0, 0, 65354, 0, 729, 60074, 0, 65355, 57527, 0, 64809, 183, 0, 0, 0, 0, 910, 55706, 0, 1457, 59164, 0, 63353, 1093, 64080, 182, 0},
    {1999, 1456, 60441, 1638, 65353, 64810, 546, 61988, 4093, 0, 64810, 60257, 0, 64081, 59891, 0, 726, 64625, 0, 0, 64628, 0, 0, 61348, 0, 0, 59347, 0,
     65172, 63717, 0, 0, 65354, 0, 729, 62439, 0, 65355, 59165, 0, 64810, 64991, 0, 0, 0, 0, 909, 58617, 0, 1456, 60619, 0, 63354, 183, 64080, 182, 0},
    {0, 64627, 56980, 0, 65354, 64081, 0, 182, 909, 0, 0, 61896, 0, 0, 62624, 0, 65172, 64082, 0, 0, 0, 0, 0, 53340, 0, 0, 45331, 0, 0, 58255, 0, 0, 0, 0,
     0, 54616, 0, 182, 45876, 0, 0, 57344, 0, 18, 0, 0, 0, 55341, 726, 182, 47696, 1456, 64445, 54797, 0, 65445, 65172},
    {0, 730, 63895, 0, 0, 364, 0, 64080, 2000, 0, 0, 57164, 0, 65172, 53523, 0, 65172, 58803, 0, 0, 0, 0, 0, 56251, 0, 0, 48060, 0, 0, 55888, 0, 0, 0, 0,
     0, 58073, 0, 182, 45876, 0, 364, 54432, 0, 18, 0, 0, 65354, 55340, 725, 65172, 47695, 1456, 729, 54796, 0, 65445, 65172},
    {0, 909, 47154, 0, 365, 63715, 0, 59712, 2911, 0, 63716, 45697, 0, 726, 46244, 0, 63716, 3821, 0, 364, 1274, 0, 0, 47335, 0, 726, 45334, 0, 64445,
     3275, 0, 546, 1820, 0, 1273, 46608, 0, 909, 47153, 0, 65354, 2730, 0, 546, 2182, 0, 1456, 47335, 0, 909, 46789, 0, 364, 3457, 0, 909, 1091},
    {1999, 65354, 2367, 1638, 180, 1638, 546, 63991, 3364, 0, 64811, 53705, 0, 64811, 49334, 0, 64627, 60074, 0, 0, 64811, 0, 0, 56800, 0, 0, 47516, 0,
     65172, 55526, 0, 0, 64990, 0, 729, 56434, 0, 65173, 50429, 0, 65172, 54981, 0, 0, 0, 0, 0, 54978, 0, 1639, 50428, 0, 726, 59349, 64080, 65354, 64445},
    {0, 64080, 60440, 63716, 3094, 909, 0, 65263, 2912, 0, 726, 60622, 0, 546, 56435, 0, 182, 61713, 0, 65354, 0, 0, 0, 55340, 0, 726, 48243, 726, 0,
     58438, 0, 0, 0, 0, 727, 51701, 726, 1274, 49335, 0, 0, 58620, 0, 182, 64810, 2912, 546, 52975, 1456, 65172, 49699, 1456, 65173, 55890, 0, 364, 64810},
    {0, 0, 58254, 0, 2182, 61168, 0, 0, 5824, 0, 65172, 53886, 0, 1636, 52429, 0, 0, 55706, 0, 0, 0, 0, 1456, 56434, 0, 0, 50426, 0, 726, 54796, 0, 0, 0,
     0, 2184, 57891, 0, 0, 51519, 0, 727, 54795, 0, 182, 0, 2912, 2002, 53156, 726, 65353, 52613, 0, 910, 54432, 65518, 255, 0},
    {726, 63899, 58256, 2365, 182, 1273, 61715, 1456, 3457, 0, 65354, 60441, 0, 0, 56071, 0, 0, 60986, 0, 0, 64627, 364, 182, 60439, 182, 64810, 54434, 0,
     0, 57346, 0, 0, 0, 65354, 726, 60441, 0, 65354, 52249, 0, 64990, 55344, 0, 0, 65354, 0, 64263, 53887, 909, 182, 55889, 364, 64445, 52613, 0, 0,
     64627},
    {1999, 364, 60726, 1638, 65354, 444, 546, 61988, 3418, 0, 400, 58547, 0, 65297, 53996, 0, 726, 61790, 0, 0, 64133, 0, 0, 59953, 0, 0, 57457, 0, 65172,
     59262, 0, 0, 64994, 0, 729, 59199, 0, 65355, 55475, 0, 64810, 61931, 0, 0, 0, 0, 909, 54477, 0, 1456, 57918, 0, 63354, 62343, 64080, 182, 65492},
    {1999, 1456, 60440, 1638, 909, 64082, 546, 60714, 2818, 0, 64811, 60257, 0, 1092, 61893, 0, 180, 62805, 0, 0, 548, 0, 0, 61348, 0, 0, 60075, 0, 65172,
     61715, 0, 546, 364, 0, 729, 62439, 0, 65355, 59165, 0, 65175, 61351, 0, 909, 727, 0, 909, 58617, 0, 1456, 60618, 0, 63354, 62078, 64080, 1820, 65354},
    {1999, 65354, 62444, 1638, 2728, 63900, 546, 64719, 3365, 0, 64810, 60984, 0, 730, 61347, 0, 0, 60075, 0, 0, 912, 0, 1638, 61348, 0, 0, 62258, 0,
     65172, 64991, 0, 0, 65354, 0, 729, 63713, 0, 65173, 65353, 0, 65172, 64444, 0, 0, 0, 0, 547, 1817, 0, 1639, 63167, 0, 64810, 364, 64080, 182, 0},
};
#endif
