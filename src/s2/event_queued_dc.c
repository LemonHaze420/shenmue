#include "s2/event_queued_dc.h"

#if defined(SHENMUE_EVENT_LIBRARY_PATI)
static char eventVoicePrefix[8];
#elif SHENMUE_EVENT_LIBRARY_REVISION < EVENT_LIBRARY_REVISION_0337
#elif SHENMUE_EVENT_LIBRARY_REVISION < EVENT_LIBRARY_REVISION_0009
static char eventVoicePrefix[8];
#elif SHENMUE_EVENT_LIBRARY_REVISION < EVENT_LIBRARY_REVISION_4BTL
static char eventVoicePrefix[16];
#else
static char eventVoicePrefix[32];
#endif

void EVT_AT_SetCam_Callback(void *data)
{
    EventSetCamera *p = (EventSetCamera *)data;
    if (p->camera < 0) {
        if (p->camera == -2)
            FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlHandleContextSwitch(p->context);
        FTBL.camCtrlSetState(0);
    } else {
        if (p->angle < 0)
            FTBL.camCtrlSetSplineShouldRebuildRotTo(1);
        if (p->eye == 0xffffffffU && p->target == 0xffffffffU)
            FTBL.EVS_LoopCamera(p->camera, 0, 0);
        else
            FTBL.EVS_LoopCamera(p->camera, p->eye, p->target);
    }
    if (p->cleanup)
        FTBL.HLib_DestroyCurrentTask();
}

void EVT_AT_SetCam(int camera, shen_name eye, shen_name target, int context, int angle, int delay)
{
    if (delay) {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_AT_SetCam_Callback, 6, 11, sizeof(EventSetCamera), TASK_EVNT);
        EventSetCamera *p;
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(task, delay);
        p = (EventSetCamera *)FTBL.HLib_GetTaskParameter(task);
        p->cleanup = 1;
        p->camera = camera;
        p->eye = eye;
        p->target = target;
        p->context = context;
        p->angle = angle;
    } else {
        EventSetCamera request;
        request.cleanup = 0;
        request.camera = camera;
        request.eye = eye;
        request.target = target;
        request.context = context;
        request.angle = angle;
        EVT_AT_SetCam_Callback(&request);
    }
}

#if !defined(SHENMUE_EVENT_LIBRARY_PATI)
void EVT_AT_SetCamRot_Callback(void *data)
{
    EventSetCameraRotation *p = (EventSetCameraRotation *)data;
    FTBL.camCtrlSetRotationXYZ(p->x, p->y, p->duration);
    if (p->cleanup)
        FTBL.HLib_DestroyCurrentTask();
}

void EVT_AT_SetCamRot(float x, float y, float duration, int delay)
{
    if (delay) {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_AT_SetCamRot_Callback, 6, 11, sizeof(EventSetCameraRotation), TASK_EVNT);
        EventSetCameraRotation *p;
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(task, delay);
        p = (EventSetCameraRotation *)FTBL.HLib_GetTaskParameter(task);
        p->cleanup = 1;
        p->x = x;
        p->y = y;
        p->duration = duration;
    } else {
        EventSetCameraRotation request;
        request.cleanup = 1;
        request.x = x;
        request.y = y;
        request.duration = duration;
        EVT_AT_SetCamRot_Callback(&request);
    }
}
#endif  

void EVT_AT_SetMot_Callback(void *data)
{
    EventSetMotion *p = (EventSetMotion *)data;
    if (!(p->flags & 0x80000000U)) {
        if (p->flags & 0x40000000U)
            FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(p->character), 71, 0, 0);
        else
            FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(p->character), 71, 2, 0);
        if (p->flags & 0x20000000U)
            FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(p->character), 69, 0, 0);
        else
            FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(p->character), 69, 2, 0);
        if (p->flags & 0x04000000U)
            FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(p->character), 14, 0);
        else
            FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(p->character), 14, 2);
        if (p->flags & 0x02000000U)
            FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(p->character), 17, 2);
        else
            FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(p->character), 17, 0);
        if (p->flags & 0x01000000U)
            FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(p->character), 66, 0, 0);
        if (p->flags & 0x00800000U)
            FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(p->character), 66, 2, 0);
        if (p->flags & 0x10000000U) {
            FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(p->character), 0);
            FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(p->character), 0, 2);
        } else {
            FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(p->character), 1);
            FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(p->character), 0, 0);
        }
    }
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(p->character), p->animation, 0);
    FTBL.SetMotionRegionEvalTime(FTBL.scenFindMotionwork(p->character), p->frame, 0);
    FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(p->character), p->speed, 0);
    if (p->cleanup)
        FTBL.HLib_DestroyCurrentTask();
}

int EVT_AT_SetMot(shen_name character, unsigned int animation, float frame, float speed, unsigned int flags, int delay)
{
    float frames;
    if (delay) {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_AT_SetMot_Callback, 6, 11, sizeof(EventSetMotion), TASK_EVNT);
        EventSetMotion *p;
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(task, delay);
        p = (EventSetMotion *)FTBL.HLib_GetTaskParameter(task);
        p->cleanup = 1;
        p->character = character;
        p->animation = animation;
        p->frame = frame;
        p->speed = speed;
        p->flags = flags;
    } else {
        EventSetMotion request;
        request.cleanup = 0;
        request.character = character;
        request.animation = animation;
        request.frame = frame;
        request.speed = speed;
        request.flags = flags;
        EVT_AT_SetMot_Callback(&request);
    }
    frames = ((float)FTBL.GetMotionFrameCount(animation) - frame) / speed;
    return (int)((float)delay + frames);
}

#if !defined(SHENMUE_EVENT_LIBRARY_PATI)
void EVT_AT_SetPosRot(void *data)
{
    EventSetPosition *p = (EventSetPosition *)data;
    if (p->character == p->parent) {
        Vector3 position;
        Vector3i rotation;
        FTBL.GetCharaPosition(p->character, 0xffffffffU, &position, 0x40000000);
        FTBL.GetScenarioCharacterEulerAngles(p->character, &rotation, 0x40000000);
        FTBL.ApplyScenarioTranslationAndRotation(&position, &rotation, &p->position);
        FTBL.EV_AT_SetPos(p->character, 0x78000000, &p->position);
    } else {
        shen_name previous = FTBL.SCEN_FindChar_Wrapper(p->character);
        FTBL.AT_SetLocalBaseChara(p->character, p->parent);
        FTBL.EV_AT_SetPos(p->character, 0x38000000, &p->position);
        FTBL.AT_SetLocalBaseChara(p->character, previous);
    }
    if (p->cleanup)
        FTBL.HLib_DestroyCurrentTask();
}

void EVT_AT_SetPos(shen_name character, shen_name parent, float x, float y, float z, int delay)
{
    if (delay) {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_AT_SetPosRot, 6, 11, sizeof(EventSetPosition), TASK_EVNT);
        EventSetPosition *p;
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(task, delay);
        p = (EventSetPosition *)FTBL.HLib_GetTaskParameter(task);
        p->cleanup = 1;
        p->character = character;
        p->parent = parent;
        p->position.x = x;
        p->position.y = y;
        p->position.z = z;
    } else {
        EventSetPosition request;
        request.cleanup = 0;
        request.character = character;
        request.parent = parent;
        request.position.x = x;
        request.position.y = y;
        request.position.z = z;
        EVT_AT_SetPosRot(&request);
    }
}

void EVT_AT_SetAng_Callback(void *data)
{
    EventSetRawAngle *p = (EventSetRawAngle *)data;
    if (p->character == p->parent) {
        Vector3i rotation;
        FTBL.GetScenarioCharacterEulerAngles(p->character, &rotation, 0);
        rotation.x += p->rotation.x;
        rotation.y += p->rotation.y;
        rotation.z += p->rotation.z;
        FTBL.EV_AT_SetAngY_0(p->character, 0x38000000, &rotation, 0);
    } else {
        shen_name previous = FTBL.SCEN_FindChar_Wrapper(p->character);
        FTBL.AT_SetLocalBaseChara(p->character, p->parent);
        FTBL.EV_AT_SetAngY_0(p->character, 0x38000000, &p->rotation, 0);
        FTBL.AT_SetLocalBaseChara(p->character, previous);
    }
    if (p->cleanup)
        FTBL.HLib_DestroyCurrentTask();
}

void EVT_AT_SetAng(shen_name character, shen_name parent, float x, float y, float z, int delay)
{
    if (delay) {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_AT_SetAng_Callback, 6, 11, sizeof(EventSetRawAngle), TASK_EVNT);
        EventSetRawAngle *p;
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(task, delay);
        p = (EventSetRawAngle *)FTBL.HLib_GetTaskParameter(task);
        p->cleanup = 1;
        p->character = character;
        p->parent = parent;
        p->rotation.x = (int)(x * 8192.0f / 45.0f) & 0xffff;
        p->rotation.y = (int)(y * 8192.0f / 45.0f) & 0xffff;
        p->rotation.z = (int)(z * 8192.0f / 45.0f) & 0xffff;
    } else {
        EventSetRawAngle request;
        request.cleanup = 0;
        request.character = character;
        request.parent = parent;
        request.rotation.x = (int)(x * 8192.0f / 45.0f) & 0xffff;
        request.rotation.y = (int)(y * 8192.0f / 45.0f) & 0xffff;
        request.rotation.z = (int)(z * 8192.0f / 45.0f) & 0xffff;
        EVT_AT_SetAng_Callback(&request);
    }
}

void EVT_AT_SetAngY_Callback(void *data)
{
    EventSetAngle *p = (EventSetAngle *)data;
    Vector3i rotation;
    if (p->character == p->parent) {
        FTBL.GetScenarioCharacterEulerAngles(p->character, &rotation, 0);
        rotation.y += (unsigned short)(p->degrees * 8192.0f / 45.0f);
        FTBL.EV_AT_SetAngY_0(p->character, 0x38000000, &rotation, 0);
    } else {
        shen_name previous = FTBL.SCEN_FindChar_Wrapper(p->character);
        FTBL.AT_SetLocalBaseChara(p->character, p->parent);
        FTBL.GetScenarioCharacterEulerAngles(p->character, &rotation, 0);
        rotation.y = (unsigned short)(p->degrees * 8192.0f / 45.0f);
        FTBL.EV_AT_SetAngY_0(p->character, 0x38000000, &rotation, 0);
        FTBL.AT_SetLocalBaseChara(p->character, previous);
    }
    if (p->cleanup)
        FTBL.HLib_DestroyCurrentTask();
}

void EVT_AT_SetAngY(shen_name character, shen_name parent, float degrees, int delay)
{
    if (delay) {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_AT_SetAngY_Callback, 6, 11, sizeof(EventSetAngle), TASK_EVNT);
        EventSetAngle *p;
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(task, delay);
        p = (EventSetAngle *)FTBL.HLib_GetTaskParameter(task);
        p->cleanup = 1;
        p->character = character;
        p->parent = parent;
        p->degrees = degrees;
    } else {
        EventSetAngle request;
        request.cleanup = 0;
        request.character = character;
        request.parent = parent;
        request.degrees = degrees;
        EVT_AT_SetAngY_Callback(&request);
    }
}
#endif  

void EVT_AT_SetPosAng_Callback(void *data)
{
    EventSetPositionAngle *p = (EventSetPositionAngle *)data;
    Vector3 position;
    Vector3i rotation;
    if (p->character == p->parent) {
        FTBL.GetCharaPosition(p->character, 0xffffffffU, &position, 0x40000000);
        FTBL.GetScenarioCharacterEulerAngles(p->character, &rotation, 0x40000000);
        FTBL.ApplyScenarioTranslationAndRotation(&position, &rotation, &p->position);
        FTBL.EV_AT_SetPos(p->character, 0x78000000, &p->position);
        FTBL.GetScenarioCharacterEulerAngles(p->character, &rotation, 0);
        rotation.y += (unsigned short)(p->degrees * 8192.0f / 45.0f);
        FTBL.EV_AT_SetAngY_0(p->character, 0x38000000, &rotation, 0);
    } else {
        shen_name previous;
        rotation.x = 0;
        rotation.y = (unsigned short)(p->degrees * 8192.0f / 45.0f);
        rotation.z = 0;
        previous = FTBL.SCEN_FindChar_Wrapper(p->character);
        FTBL.AT_SetLocalBaseChara(p->character, p->parent);
        FTBL.GetCharaPosition(p->character, 0xffffffffU, &position, 0);
        p->position.y = position.y;
        FTBL.EV_AT_SetPos(p->character, 0x38000000, &p->position);
        FTBL.EV_AT_SetAngY_0(p->character, 0x38000000, &rotation, 0);
        FTBL.AT_SetLocalBaseChara(p->character, previous);
    }
    if (p->cleanup)
        FTBL.HLib_DestroyCurrentTask();
}

void EVT_AT_SetPosAng(shen_name character, shen_name parent, float x, float z, float degrees, int delay)
{
    if (delay) {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_AT_SetPosAng_Callback, 6, 11, sizeof(EventSetPositionAngle), TASK_EVNT);
        EventSetPositionAngle *p;
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(task, delay);
        p = (EventSetPositionAngle *)FTBL.HLib_GetTaskParameter(task);
        p->cleanup = 1;
        p->character = character;
        p->parent = parent;
        p->position.x = x;
        p->position.y = 0.0f;
        p->position.z = z;
        p->degrees = degrees;
    } else {
        EventSetPositionAngle request;
        request.cleanup = 0;
        request.character = character;
        request.parent = parent;
        request.position.x = x;
        request.position.y = 0.0f;
        request.position.z = z;
        request.degrees = degrees;
        EVT_AT_SetPosAng_Callback(&request);
    }
}

void EVT_AT_SetVoice_Callback(void *data)
{
    EventSetVoice *p = (EventSetVoice *)data;
    if (p->mode || !FTBL.cvsHasValidConverseData(0)) {
#if SHENMUE_EVENT_LIBRARY_REVISION < EVENT_LIBRARY_REVISION_DGCT
        FTBL.scenCvsCtrlFree(0);
#endif
        FTBL.EV_PlayDialogue(p->voice);
    }
    if (p->cleanup)
        FTBL.HLib_DestroyCurrentTask();
}

void EVT_AT_SetVoice(const char *voice, int delay)
{
#if defined(SHENMUE_EVENT_LIBRARY_PATI)
    int i;
#endif
    if (delay) {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_AT_SetVoice_Callback, 6, 11, sizeof(EventSetVoice), TASK_EVNT);
        EventSetVoice *p;
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(task, delay);
        p = (EventSetVoice *)FTBL.HLib_GetTaskParameter(task);
        p->cleanup = 1;
        p->mode = 1;
#if SHENMUE_EVENT_LIBRARY_REVISION < EVENT_LIBRARY_REVISION_0009
        p->voice = voice;
#elif defined(SHENMUE_EVENT_LIBRARY_PATI)
        for (i = 0; i < 64; i++)
            p->voice[i] = voice[i];
#else
        strcpy(p->voice, voice);
#endif
    } else {
        EventSetVoice request;
        request.cleanup = 0;
        request.mode = 1;
#if SHENMUE_EVENT_LIBRARY_REVISION < EVENT_LIBRARY_REVISION_0009
        request.voice = voice;
#elif defined(SHENMUE_EVENT_LIBRARY_PATI)
        for (i = 0; i < 64; i++)
            request.voice[i] = voice[i];
#else
        strcpy(request.voice, voice);
#endif
        EVT_AT_SetVoice_Callback(&request);
    }
}

#if !defined(SHENMUE_EVENT_LIBRARY_PATI)
void EVT_AT_SetVoiceOutsideConversation(const char *voice, int delay)
{
    if (delay) {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_AT_SetVoice_Callback, 6, 11, sizeof(EventSetVoice), TASK_EVNT);
        EventSetVoice *p;
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(task, delay);
        p = (EventSetVoice *)FTBL.HLib_GetTaskParameter(task);
        p->cleanup = 1;
        p->mode = 0;
#if SHENMUE_EVENT_LIBRARY_REVISION < EVENT_LIBRARY_REVISION_0009
        p->voice = voice;
#else
        strcpy(p->voice, voice);
#endif
    } else {
        EventSetVoice request;
        request.cleanup = 0;
        request.mode = 0;
#if SHENMUE_EVENT_LIBRARY_REVISION < EVENT_LIBRARY_REVISION_0009
        request.voice = voice;
#else
        strcpy(request.voice, voice);
#endif
        EVT_AT_SetVoice_Callback(&request);
    }
}
#endif  

#if SHENMUE_EVENT_LIBRARY_REVISION >= EVENT_LIBRARY_REVISION_0337
void SetEventVoicePrefix(const char *prefix)
{
    strcpy(eventVoicePrefix, prefix);
}

void EventVoiceListCallback(void *data)
{
    EventVoiceListWork *work = (EventVoiceListWork *)data;
    if (!FTBL.cvsHasValidConverseData(0)) {
        if (work->voices[work->index][0]) {
            EVT_AT_SetVoice(work->voices[work->index], work->voiceDelayFrames);
            if (work->voiceDelayFrames > 0)
                FTBL.HLib_SetupTaskDelay(work->voiceDelayFrames);
            work->index++;
        } else {
            if (work->parent)
                FTBL.HLib_ResolveDeferredCallback(work->parent);
            FTBL.HLib_DestroyCurrentTask();
        }
    } else if (work->mode & 0x40000000U) {
        if (FTBL.GetCurrentControlInput(7) == 0x200) {
            if (work->skipPressFrames)
                FTBL.scenCvsCtrlFree(0);
            else
                work->skipPressFrames = 10;
        } else if (work->skipPressFrames > 0) {
            work->skipPressFrames--;
        }
    }
}

void QueueEventVoiceList(HLibTask *parent, const char **voices, int voiceDelayFrames, unsigned int mode, int delay)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EventVoiceListCallback, 6, 11, sizeof(EventVoiceListWork), TASK_EVNT);
    EventVoiceListWork *work;
    int i;
    if (delay > 0)
        FTBL.HLib_SetupTimedTask(task, delay);
    work = (EventVoiceListWork *)FTBL.HLib_GetTaskParameter(task);
    work->voiceDelayFrames = voiceDelayFrames;
    work->mode = mode;
    work->parent = parent;
    work->index = 0;
    work->skipPressFrames = 0;
#if SHENMUE_EVENT_LIBRARY_REVISION < EVENT_LIBRARY_REVISION_4BTL
    for (i = 0; i < EVENT_VOICE_LIST_COUNT; i++, voices++) {
#else
    for (i = 0; i < 32; i++, voices++) {
#endif
        if (!*voices) {
            work->voices[i][0] = 0;
            break;
        }
        if (work->mode & 0x80000000U) {
            do {
                strcpy(work->voices[i], eventVoicePrefix);
            } while (0);
            strcat(work->voices[i], *voices);
        } else {
            strcpy(work->voices[i], *voices);
        }
    }
    if (work->parent)
        FTBL.HLib_SetupTimedTask(work->parent, 0);
}

#if !defined(SHENMUE_EVENT_LIBRARY_PATI)
int g_eventQteFailureCount = 0;

int g_eventQteRetryFrames = -1;
#endif
#endif

#if !defined(SHENMUE_EVENT_LIBRARY_PATI)
void EVT_AT_SetSound_Callback(void *data)
{
    EventSetSound *p = (EventSetSound *)data;
    FTBL.Loop3DSoundEffect(p->sound);
    if (p->cleanup)
        FTBL.HLib_DestroyCurrentTask();
}

void EVT_AT_SetSound(unsigned int sound, int delay)
{
    if (delay) {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_AT_SetSound_Callback, 6, 11, sizeof(EventSetSound), TASK_EVNT);
        EventSetSound *p;
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(task, delay);
        p = (EventSetSound *)FTBL.HLib_GetTaskParameter(task);
        p->cleanup = 1;
        p->sound = sound;
    } else {
        EventSetSound p;
        p.cleanup = 0;
        p.sound = sound;
        EVT_AT_SetSound_Callback(&p);
    }
}
#endif  

#if SHENMUE_EVENT_LIBRARY_REVISION >= EVENT_LIBRARY_REVISION_0337
#if !defined(SHENMUE_EVENT_LIBRARY_PATI)
void EVT_AT_SetSample_Callback(void *data)
{
    EventSetSample *p = (EventSetSample *)data;
    if (p->fadeOut)
        FTBL.spmFadeOut(p->sample);
    else
        FTBL.EV_spmBeginSampleExecute(p->sample);
    if (p->cleanup)
        FTBL.HLib_DestroyCurrentTask();
}

void EVT_AT_SetSample(int sample, int fadeOut, int delay)
{
    if (delay) {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_AT_SetSample_Callback, 6, 11, sizeof(EventSetSample), TASK_EVNT);
        EventSetSample *p;
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(task, delay);
        p = (EventSetSample *)FTBL.HLib_GetTaskParameter(task);
        p->cleanup = 1;
        p->sample = sample;
        p->fadeOut = fadeOut;
    } else {
        EventSetSample p;
        p.cleanup = 0;
        p.sample = sample;
        p.fadeOut = fadeOut;
        EVT_AT_SetSample_Callback(&p);
    }
}
#endif  
#endif

void EVT_AT_KsIcon_Callback(void *data)
{
    EventChoiceIcon *p = (EventChoiceIcon *)data;
    FTBL.SetupControlSpeakAndUI(p->choices, (short)p->timeout, p->defaultChoice, (unsigned short)p->mode);
    if (p->cleanup)
        FTBL.HLib_DestroyCurrentTask();
}

void EVT_AT_KsIcon_Enqueue(int first, int second, int third, int fourth, int timeout, int defaultChoice, int mode, int delay)
{
    if (delay) {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_AT_KsIcon_Callback, 6, 11, sizeof(EventChoiceIcon), TASK_EVNT);
        EventChoiceIcon *p;
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(task, delay);
        p = (EventChoiceIcon *)FTBL.HLib_GetTaskParameter(task);
        p->cleanup = 1;
        p->choices[0] = first;
        p->choices[1] = second;
        p->choices[2] = third;
        p->choices[3] = fourth;
        p->timeout = timeout;
        p->defaultChoice = defaultChoice;
        p->mode = mode;
    } else {
        EventChoiceIcon p;
        p.cleanup = 0;
        p.choices[0] = first;
        p.choices[1] = second;
        p.choices[2] = third;
        p.choices[3] = fourth;
        p.timeout = timeout;
        p.defaultChoice = defaultChoice;
        p.mode = mode;
        EVT_AT_KsIcon_Callback(&p);
    }
}

#if !defined(SHENMUE_EVENT_LIBRARY_PATI)
void EVT_AT_MoneyUI_Callback(void *data)
{
    EventMoneyUI *p = (EventMoneyUI *)data;
    FTBL.SetMoneyRelatedUIElementAlternative(p->maximum, p->duration, p->step);
    if (p->cleanup)
        FTBL.HLib_DestroyCurrentTask();
}

void EVT_AT_MoneyUI(int maximum, short duration, short step, int delay)
{
    if (delay) {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_AT_MoneyUI_Callback, 6, 11, sizeof(EventMoneyUI), TASK_EVNT);
        EventMoneyUI *p;
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(task, delay);
        p = (EventMoneyUI *)FTBL.HLib_GetTaskParameter(task);
        p->cleanup = 1;
        p->maximum = maximum;
        p->duration = duration;
        p->step = step;
    } else {
        EventMoneyUI p;
        p.cleanup = 0;
        p.maximum = maximum;
        p.duration = duration;
        p.step = step;
        EVT_AT_MoneyUI_Callback(&p);
    }
}
#endif  

void EVT_AT_StartMoneyCount_Callback(void *data)
{
    EventMoneyCount *p = (EventMoneyCount *)data;
    FTBL.AddThisToCashOrSegaCoins(p->cash, 2);
    if (p->cleanup)
        FTBL.HLib_DestroyCurrentTask();
}

void EVT_AT_StartMoneyCount(int cash, int delay)
{
    if (delay) {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_AT_StartMoneyCount_Callback, 6, 11, sizeof(EventMoneyCount), TASK_EVNT);
        EventMoneyCount *p;
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(task, delay);
        p = (EventMoneyCount *)FTBL.HLib_GetTaskParameter(task);
        p->cleanup = 1;
        p->cash = cash;
    } else {
        EventMoneyCount p;
        p.cleanup = 0;
        p.cash = cash;
        EVT_AT_StartMoneyCount_Callback(&p);
    }
}

void EVT_RepeatCall_Callback(void *data)
{
    EventRepeatCall *p = (EventRepeatCall *)data;
    if (p->count-- > 0)
        p->function(p->argument);
    else
        FTBL.HLib_DestroyCurrentTask();
}

#if !defined(SHENMUE_EVENT_LIBRARY_PATI)
void EVT_RepeatCallData_Callback(void *data)
{
    EventRepeatDataCall *p = (EventRepeatDataCall *)data;
    if (p->index < p->count) {
        if (p->size)
            p->function(p + 1, p->index);
        else
            p->function(0, p->index);
        p->index++;
    } else {
        FTBL.HLib_DestroyCurrentTask();
    }
}
#endif  

void EVT_RepeatCall(EventRepeatFunction function, void *argument, int delay, int count)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_RepeatCall_Callback, 6, 11, sizeof(EventRepeatCall), TASK_EVNT);
    EventRepeatCall *p;
    if (delay > 0)
        FTBL.HLib_SetupTimedTask(task, delay);
    p = (EventRepeatCall *)FTBL.HLib_GetTaskParameter(task);
    p->function = function;
    p->argument = argument;
    p->count = count;
}

#if !defined(SHENMUE_EVENT_LIBRARY_PATI)
void *EVT_RepeatCallData(EventRepeatDataFunction function, int size, int delay, int count)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_RepeatCallData_Callback, 6, 11, size + sizeof(EventRepeatDataCall), TASK_EVNT);
    EventRepeatDataCall *p;
    if (delay > 0)
        FTBL.HLib_SetupTimedTask(task, delay);
    p = (EventRepeatDataCall *)FTBL.HLib_GetTaskParameter(task);
    p->function = function;
    p->count = count;
    p->index = 0;
    p->size = size;
    if (size)
        return p + 1;
    return 0;
}

void EVT_AT_MenCtrl_Callback(void *data)
{
    EventFilmControl *p = (EventFilmControl *)data;
    FTBL.SetScreenFilmFade(p->frames, p->startRed, p->startGreen, p->startBlue, p->startAlpha, p->targetRed, p->targetGreen, p->targetBlue,
        p->targetAlpha);
    if (p->cleanup)
        FTBL.HLib_DestroyCurrentTask();
}

void EVT_AT_MenCtrl(unsigned int frames, int startRed, int startGreen, int startBlue, int startAlpha, int targetRed, int targetGreen, int targetBlue,
    int targetAlpha, int delay)
{
    if (delay) {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_AT_MenCtrl_Callback, 6, 11, sizeof(EventFilmControl), TASK_EVNT);
        EventFilmControl *p;
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(task, delay);
        p = (EventFilmControl *)FTBL.HLib_GetTaskParameter(task);
        p->cleanup = 1;
        p->frames = frames;
        p->startRed = startRed;
        p->startGreen = startGreen;
        p->startBlue = startBlue;
        p->startAlpha = startAlpha;
        p->targetRed = targetRed;
        p->targetGreen = targetGreen;
        p->targetBlue = targetBlue;
        p->targetAlpha = targetAlpha;
    } else {
        EventFilmControl p;
        p.cleanup = 0;
        p.frames = frames;
        p.startRed = startRed;
        p.startGreen = startGreen;
        p.startBlue = startBlue;
        p.startAlpha = startAlpha;
        p.targetRed = targetRed;
        p.targetGreen = targetGreen;
        p.targetBlue = targetBlue;
        p.targetAlpha = targetAlpha;
        EVT_AT_MenCtrl_Callback(&p);
    }
}
#endif  

#if SHENMUE_EVENT_LIBRARY_REVISION >= EVENT_LIBRARY_REVISION_0337
#if !defined(SHENMUE_EVENT_LIBRARY_PATI)
void EVT_SetQteRetryFrames(short frames)
{
    g_eventQteFailureCount = 0;
    g_eventQteRetryFrames = frames;
}

void EVT_IncrementQteFailureCount(void)
{
    if (g_eventQteFailureCount < 2)
        g_eventQteFailureCount++;
}
#endif  
#endif

#if !defined(SHENMUE_EVENT_LIBRARY_PATI)
void EVT_AT_Qte_Callback(void *data)
{
    EventQtePrompt *p = (EventQtePrompt *)data;
    if (p->displayState == 0) {
        FTBL.SetShowQteFlagForButtonType(p->requiredStatus, p->maximumFrames, 0);
    } else if (p->displayState == 1) {
        FTBL.SetShowQteFlagForButtonType(p->requiredStatus, p->maximumFrames, 1);
    } else if (p->displayState == 2) {
        FTBL.SetShowQteFlagForButtonType(p->requiredStatus, p->maximumFrames, 1);
    } else if (p->displayState == 3) {
        FTBL.SetShowQteFlagForButtonType(p->requiredStatus, p->maximumFrames, 2);
#if SHENMUE_EVENT_LIBRARY_REVISION >= EVENT_LIBRARY_REVISION_FRBT
    } else if (p->displayState == 4) {
#if SHENMUE_EVENT_LIBRARY_REVISION < EVENT_LIBRARY_REVISION_0337
#elif SHENMUE_EVENT_LIBRARY_REVISION < EVENT_LIBRARY_REVISION_WRS2
        FTBL.qteBattleUtil(8, 10, 5, p->maximumFrames, 0);
#else
        FTBL.qteBattleUtil(8, 10, 6, p->maximumFrames, 0);
#endif
        FTBL.qteBattleUtil(1, p->requiredStatus, 0, 0, 0);
        FTBL.qteBattleUtil(3, 0, 0, 0, 0);
#endif
    }
    if (p->cleanup)
        FTBL.HLib_DestroyCurrentTask();
}

void EVT_AT_QTE(unsigned short requiredStatus, short maximumFrames, int resultState, int delay, int displayState)
{
#if SHENMUE_EVENT_LIBRARY_REVISION >= EVENT_LIBRARY_REVISION_0337
    if (g_eventQteRetryFrames > 0)
        maximumFrames += (g_eventQteRetryFrames - maximumFrames) / 2 * g_eventQteFailureCount;
#endif
    if (delay) {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_AT_Qte_Callback, 6, 11, sizeof(EventQtePrompt), TASK_EVNT);
        EventQtePrompt *p;
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(task, delay);
        p = (EventQtePrompt *)FTBL.HLib_GetTaskParameter(task);
        p->cleanup = 1;
        p->requiredStatus = requiredStatus;
        p->maximumFrames = maximumFrames;
        p->resultState = resultState;
        p->displayState = displayState;
    } else {
        EventQtePrompt p;
        p.cleanup = 0;
        p.requiredStatus = requiredStatus;
        p.maximumFrames = maximumFrames;
        p.resultState = resultState;
        p.displayState = displayState;
        EVT_AT_Qte_Callback(&p);
    }
}

void EVT_AT_Cling(void *data)
{
    EventCling *p = (EventCling *)data;
    switch (p->mode) {
    case 0:
        FTBL.AttachScenarioCharacter(p->target, p->character, p->bone, &p->position, &p->rotation);
        break;
    case 1:
        FTBL.ResetScenarioCharacterAttachment(p->target);
        break;
    }
    if (p->cleanup)
        FTBL.HLib_DestroyCurrentTask();
}

void Queue_EVT_AT_Cling(shen_name target, shen_name character, int bone, const Vector3 *position, const Vector3i *rotation, int delay, int mode)
{
    if (delay) {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_AT_Cling, 6, 11, sizeof(EventCling), TASK_EVNT);
        EventCling *p;
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(task, delay);
        p = (EventCling *)FTBL.HLib_GetTaskParameter(task);
        p->cleanup = 1;
        p->target = target;
        p->character = character;
        p->mode = mode;
        if (mode != 1) {
            p->bone = bone;
            p->position = *position;
            p->rotation.x = rotation->x;
            p->rotation.y = rotation->y;
            p->rotation.z = rotation->z;
        }
    } else {
        EventCling p;
        p.cleanup = 0;
        p.target = target;
        p.character = character;
        p.mode = mode;
        if (mode != 1) {
            p.bone = bone;
            p.position = *position;
            p.rotation.x = rotation->x;
            p.rotation.y = rotation->y;
            p.rotation.z = rotation->z;
        }
        EVT_AT_Cling(&p);
    }
}

void EVT_AT_CharaDisp_Callback(void *data)
{
    EventCharaDisplay *p = (EventCharaDisplay *)data;
    if (!p->hide)
        FTBL.EV_EnableCharacterDisplay(p->character);
    else
        FTBL.EV_DisableCharacterDisplay(p->character);
    if (p->cleanup)
        FTBL.HLib_DestroyCurrentTask();
}

void EVT_AT_CharaDisp(shen_name character, int delay, int hide)
{
    if (delay) {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_AT_CharaDisp_Callback, 6, 11, sizeof(EventCharaDisplay), TASK_EVNT);
        EventCharaDisplay *p;
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(task, delay);
        p = (EventCharaDisplay *)FTBL.HLib_GetTaskParameter(task);
        p->cleanup = 1;
        p->character = character;
        p->hide = hide;
    } else {
        EventCharaDisplay p;
        p.cleanup = 0;
        p.character = character;
        p.hide = hide;
        EVT_AT_CharaDisp_Callback(&p);
    }
}

void EVT_AT_CharaDraw_Callback(void *data)
{
    EventCharaDisplay *p = (EventCharaDisplay *)data;
    if (!p->hide) {
        FTBL.EV_EnsureCharacterDraw(p->character);
        FTBL.EV_EnableCharacterDisplay(p->character);
    } else {
        FTBL.EV_ToggleCharacterFlag(p->character);
        FTBL.EV_DisableCharacterDisplay(p->character);
    }
    if (p->cleanup)
        FTBL.HLib_DestroyCurrentTask();
}

void EVT_AT_CharaDraw(shen_name character, int delay, int hide)
{
    if (delay) {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_AT_CharaDraw_Callback, 6, 11, sizeof(EventCharaDisplay), TASK_EVNT);
        EventCharaDisplay *p;
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(task, delay);
        p = (EventCharaDisplay *)FTBL.HLib_GetTaskParameter(task);
        p->cleanup = 1;
        p->character = character;
        p->hide = hide;
    } else {
        EventCharaDisplay p;
        p.cleanup = 0;
        p.character = character;
        p.hide = hide;
        EVT_AT_CharaDraw_Callback(&p);
    }
}

void EVT_AT_DispBlur_Callback(void *data)
{
    EventDisplayBlur *p = (EventDisplayBlur *)data;
    if (p->frames > 0) {
        FTBL.figpAssignModeFor(p->character, p->mode);
        FTBL.figpSetFlagsBlendForBlur(p->character, p->scale, p->direction);
        if (p->flags < 0)
            FTBL.figpSetFlagModesFor(p->character, 0, 1);
        else
            FTBL.figpSetFlagModesFor(p->character, 0, 0);
        if (p->flags & 0x40000000)
            FTBL.figpSetFlagModesFor(p->character, 3, 1);
        else
            FTBL.figpSetFlagModesFor(p->character, 3, 0);
        if (p->flags & 0x20000000)
            FTBL.figpSetFlagModesFor(p->character, 4, 1);
        else
            FTBL.figpSetFlagModesFor(p->character, 4, 0);
        FTBL.HLib_SetupTaskDelay(p->frames);
        p->frames = 0;
    } else {
        FTBL.figpAssignModeFor(p->character, 0);
        FTBL.HLib_DestroyCurrentTask();
    }
}

void EVT_AT_DispBlur(shen_name character, unsigned char mode, unsigned char scale, unsigned char direction, int flags, int frames, int delay)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_AT_DispBlur_Callback, 6, 11, sizeof(EventDisplayBlur), TASK_EVNT);
    EventDisplayBlur *p;
    if (delay > 0)
        FTBL.HLib_SetupTimedTask(task, delay);
    p = (EventDisplayBlur *)FTBL.HLib_GetTaskParameter(task);
    p->character = character;
    p->mode = mode;
    p->scale = scale;
    p->direction = direction;
    p->flags = flags;
    p->frames = frames;
}

void EVT_AT_CtrlHandUp_Callback(void *data)
{
    EventControlHand *p = (EventControlHand *)data;
    FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(p->character), p->side, p->command, p->ticks);
    if (p->cleanup)
        FTBL.HLib_DestroyCurrentTask();
}

void Queue_EVT_AT_CtrlHandUp_1(shen_name character, int side, void *command, int ticks, int delay)
{
    if (delay) {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_AT_CtrlHandUp_Callback, 6, 11, sizeof(EventControlHand), TASK_EVNT);
        EventControlHand *p;
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(task, delay);
        p = (EventControlHand *)FTBL.HLib_GetTaskParameter(task);
        p->cleanup = 1;
        p->character = character;
        p->side = side;
        p->command = command;
        p->ticks = ticks;
    } else {
        EventControlHand p;
        p.cleanup = 0;
        p.character = character;
        p.side = side;
        p.command = command;
        p.ticks = ticks;
        EVT_AT_CtrlHandUp_Callback(&p);
    }
}
#endif  

void EVT_AT_SetFaceLooks_Callback(void *data)
{
    EventFaceLooks *p = (EventFaceLooks *)data;
    FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(p->character), p->expression, p->duration, p->intensity);
    if (p->cleanup)
        FTBL.HLib_DestroyCurrentTask();
}

void EVT_AT_SetFaceLooks(shen_name character, int expression, int duration, float intensity, int delay)
{
    if (delay) {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_AT_SetFaceLooks_Callback, 6, 11, sizeof(EventFaceLooks), TASK_EVNT);
        EventFaceLooks *p;
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(task, delay);
        p = (EventFaceLooks *)FTBL.HLib_GetTaskParameter(task);
        p->cleanup = 1;
        p->character = character;
        p->expression = expression;
        p->duration = duration;
        p->intensity = intensity;
    } else {
        EventFaceLooks p;
        p.cleanup = 0;
        p.character = character;
        p.expression = expression;
        p.duration = duration;
        p.intensity = intensity;
        EVT_AT_SetFaceLooks_Callback(&p);
    }
}

#if SHENMUE_EVENT_LIBRARY_REVISION >= EVENT_LIBRARY_REVISION_0337 && !defined(SHENMUE_EVENT_LIBRARY_PATI)
static EventDateTime savedDateTime;
#endif

#if SHENMUE_EVENT_LIBRARY_REVISION < EVENT_LIBRARY_REVISION_AKTM
#if !defined(SHENMUE_EVENT_LIBRARY_PATI)
void EVT_AT_SetPoint_Callback(void *data)
{
    EventSetPoint *parameter = (EventSetPoint *)data;
    if (!(parameter->flags & 0x10000000)) {
        Vector3 position;
        if (parameter->targetBone == -1)
            FTBL.GetCharaPosition(parameter->target, 0xffffffff, &position, 0x40000000);
        else
            FTBL.GetCharaPosition(parameter->target, parameter->targetBone, &position, 0x40000000);
        FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(parameter->character), &position, (parameter->trackingMode << 8) + parameter->positionMode);
        if (parameter->frame == 0 && (parameter->flags & 0x80000000))
            FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(parameter->character), 0, 3);
        if (parameter->flags & 0x20000000)
            FTBL.HLib_DestroyCurrentTask();
    }
    if (parameter->frame++ > parameter->duration || (parameter->flags & 0x10000000)) {
        if (!(parameter->flags & 0x20000000)) {
            if (parameter->flags & 0x40000000)
                FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(parameter->character), 0, 3);
            FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(parameter->character), 0, 0);
            FTBL.HLib_DestroyCurrentTask();
        }
    }
}
#endif  
#else
#if !defined(SHENMUE_EVENT_LIBRARY_PATI)
void EVT_AT_SetPoint_Callback(void *data)
{
    EventSetPoint *parameter = (EventSetPoint *)data;
    if (!(parameter->flags & 0x10000000)) {
        Vector3 position;
        short eyeFrames[6] = {0, 32, 24, 16, 8, 4};
        if (parameter->targetBone == -1)
            FTBL.GetCharaPosition(parameter->target, 0xffffffff, &position, 0x40000000);
        else
            FTBL.GetCharaPosition(parameter->target, parameter->targetBone, &position, 0x40000000);
        if (!(parameter->flags & 0x08000000)) {
            if (parameter->frame == 0 && (parameter->flags & 0x80000000))
                FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(parameter->character), 0, 3);
            FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(parameter->character), &position, (parameter->trackingMode << 8) + parameter->positionMode);
        }
        if (!(parameter->flags & 0x04000000))
            FTBL.SetEyeToChara(FTBL.ctrlGetCharaByName(parameter->character), &position, eyeFrames[parameter->trackingMode], 2);
#if SHENMUE_EVENT_LIBRARY_REVISION < EVENT_LIBRARY_REVISION_Q200
        if (parameter->flags & 0x20000000)
            FTBL.HLib_DestroyCurrentTask();
    }
    if (parameter->frame++ > parameter->duration || (parameter->flags & 0x10000000)) {
#else
        if ((parameter->flags & 0x20000000) && parameter->cleanup)
            FTBL.HLib_DestroyCurrentTask();
    }
    if (++parameter->frame > parameter->duration || (parameter->flags & 0x10000000)) {
#endif
        if (!(parameter->flags & 0x20000000)) {
            if (!(parameter->flags & 0x08000000)) {
                if (parameter->flags & 0x40000000)
                    FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(parameter->character), 0, 3);
                FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(parameter->character), 0, 0);
            }
            if (!(parameter->flags & 0x04000000))
                FTBL.SetEyeToChara(FTBL.ctrlGetCharaByName(parameter->character), 0, 16, 0);
#if SHENMUE_EVENT_LIBRARY_REVISION < EVENT_LIBRARY_REVISION_Q200
            FTBL.HLib_DestroyCurrentTask();
#else
            if (parameter->cleanup)
                FTBL.HLib_DestroyCurrentTask();
#endif
        }
    }
}
#endif  
#endif

#if SHENMUE_EVENT_LIBRARY_REVISION < EVENT_LIBRARY_REVISION_Q200
#if !defined(SHENMUE_EVENT_LIBRARY_PATI)
void EVT_AT_SetPoint(shen_name character, shen_name target, int targetBone, int trackingMode, int positionMode, int delay, int duration, unsigned int flags)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_AT_SetPoint_Callback, 6, 11, sizeof(EventSetPoint), TASK_EVNT);
    EventSetPoint *parameter;
    if (delay > 0)
        FTBL.HLib_SetupTimedTask(task, delay);
    parameter = (EventSetPoint *)FTBL.HLib_GetTaskParameter(task);
    parameter->character = character;
    parameter->target = target;
    parameter->targetBone = targetBone;
    parameter->trackingMode = trackingMode;
    parameter->positionMode = positionMode;
    parameter->duration = duration;
    parameter->flags = flags;
    parameter->frame = 0;
}
#endif  
#else
#if !defined(SHENMUE_EVENT_LIBRARY_PATI)
void EVT_AT_SetPoint(shen_name character, shen_name target, int targetBone, int trackingMode, int positionMode, int delay, int duration, unsigned int flags)
{
    if ((flags & 0x30000000) && delay == 0) {
        EventSetPoint parameter;
        parameter.character = character;
        parameter.target = target;
        parameter.targetBone = targetBone;
        parameter.trackingMode = trackingMode;
        parameter.positionMode = positionMode;
        parameter.duration = duration;
        parameter.flags = flags;
        parameter.frame = 0;
        parameter.cleanup = 0;
        EVT_AT_SetPoint_Callback(&parameter);
    } else {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_AT_SetPoint_Callback, 6, 11, sizeof(EventSetPoint), TASK_EVNT);
        EventSetPoint *parameter;
        if (delay > 0)
            FTBL.HLib_SetupTimedTask(task, delay);
        parameter = (EventSetPoint *)FTBL.HLib_GetTaskParameter(task);
        parameter->character = character;
        parameter->target = target;
        parameter->targetBone = targetBone;
        parameter->trackingMode = trackingMode;
        parameter->positionMode = positionMode;
        parameter->duration = duration;
        parameter->flags = flags;
        parameter->frame = 0;
        parameter->cleanup = 1;
    }
}
#endif  
#endif

#if !defined(SHENMUE_EVENT_LIBRARY_PATI)
void EVT_AT_PlayDynamix_Callback(void *data)
{
    EventPlayDynamics *parameter = (EventPlayDynamics *)data;
    Vector3 position;
    Vector3i rotation;
    if (parameter->currentFrame < parameter->endFrame) {
        if (parameter->trackCount == 0) {
            FTBL.dynaReadEvaluate(0, (int)parameter->currentFrame, &position, &rotation, parameter->resource);
            position.x *= 10.0f;
            position.y *= 10.0f;
            position.z *= 10.0f;
            FTBL.EV_AT_SetPos(parameter->character, 0x38000000, &position);
            FTBL.EV_AT_SetAngY_0(parameter->character, 0x38000000, &rotation, 0);
        } else {
            int track;
            for (track = 0; track < parameter->trackCount; track++) {
                FTBL.dynaReadEvaluate(track, (int)parameter->currentFrame, &position, &rotation, parameter->resource);
                position.x *= 10.0f;
                position.y *= 10.0f;
                position.z *= 10.0f;
                if (parameter->parts)
                    FTBL.scenApplyPosOrRotToChildId(parameter->character, parameter->parts[track], &position, &rotation, 0);
                else
                    FTBL.scenApplyPosOrRotToChildId(parameter->character, track + 152, &position, &rotation, 0);
            }
        }
        parameter->currentFrame += parameter->frameStep;
    } else
        FTBL.HLib_DestroyCurrentTask();
}

HLibTask *Queue_EVT_AT_PlayDynamix(shen_name character, const void *resource, float frame, float endFrame, int trackCount, const int *parts, float frameStep,
    int delay)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_AT_PlayDynamix_Callback, 6, 11, sizeof(EventPlayDynamics), TASK_EVNT);
    EventPlayDynamics *work;
    if (delay > 0)
        FTBL.HLib_SetupTimedTask(task, delay);
    work = (EventPlayDynamics *)FTBL.HLib_GetTaskParameter(task);
    work->character = character;
    work->resource = resource;
    work->endFrame = endFrame;
    work->trackCount = trackCount;
    work->parts = parts;
    work->frameStep = frameStep;
    work->currentFrame = frame;
    return task;
}

void QueueClearNpcs(void (*callback)(void *parameter), void *parameter)
{
    HLibTask *task;
    ClearNpcTask *work;
    FTBL.SetPlayerControlEnabled(0);
    task = FTBL.HLib_EnqueueTaskWithParameter(ClearNpcs, 6, 11, sizeof(ClearNpcTask), TASK_EVNT);
    work = (ClearNpcTask *)FTBL.HLib_GetTaskParameter(task);
    FTBL.HLib_SetupTimedTask(task, 3);
    work->callback = callback;
    work->parameter = parameter;
    work->reserved08 = 0;
    work->state = 0;
}

void ClearNpcs(void *data)
{
    ClearNpcTask *work = (ClearNpcTask *)data;
    if (work->state == 0) {
        FTBL.EV_NPC_NewLoadSw_0(0);
        work->state = FTBL.EV_ResetFreeTalkFlags();
    } else {
        work->callback(work->parameter);
        FTBL.HLib_DestroyCurrentTask();
    }
}
#endif  

#if SHENMUE_EVENT_LIBRARY_REVISION >= EVENT_LIBRARY_REVISION_0337
#if !defined(SHENMUE_EVENT_LIBRARY_PATI)
void SaveEventDateTime(void)
{
    FTBL.GetCurrentDateTime((DateTime *)&savedDateTime);
}

void RestoreEventDateTime(void)
{
    FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&savedDateTime);
    FTBL.shenSetTimeSimulationTo(1);
}
#endif  
#endif

#if !defined(SHENMUE_EVENT_LIBRARY_PATI)
EventReadCharactersWork *Queue_EVT_AT_ReadChr(HLibTask *parent, const shen_name *names)
{
    EventReadCharactersWork *work;
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EVT_AT_ReadChr_Callback, 6, 11, sizeof(EventReadCharactersWork), TASK_EVNT);
    int index;
    work = (EventReadCharactersWork *)FTBL.HLib_GetTaskParameter(task);
    FTBL.HLib_SetupTimedTask(parent, 0);
    FTBL.SetPlayerControlEnabled(0);
#if SHENMUE_EVENT_LIBRARY_REVISION < EVENT_LIBRARY_REVISION_0549
    FTBL.FinishLoading();
#endif
    work->task = task;
    work->parentTask = parent;
    work->names = names;
    work->characterIndex = 0;
    work->stackDepth = 0;
    work->maxNpc = FTBL.GetNpcBatchCountOverride(30);
    work->needsReload = FTBL.GetNpcNewLoadState();
    FTBL.MapModelSlotCacheUtil(4);
    FTBL.MapModelSlotCacheUtil(2);
#if SHENMUE_EVENT_LIBRARY_REVISION >= EVENT_LIBRARY_REVISION_4BTL
    FTBL.SetPlayerGameFlags(22, 1);
#endif
    if (work->needsReload)
        FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    for (index = 29; index >= 0; index--)
        work->characters[index].state = 0;
    return work;
}

void ET_AT_KillWork(EventReadCharactersWork *work)
{
    while (work->characterIndex > 0)
        RestoreEventCharacter(work);
    while (work->stackDepth > 0)
        PopReadChrStack(work);
    FTBL.GetNpcBatchCountOverride(work->maxNpc);
    FTBL.EV_NPC_NewLoadSw_0(work->needsReload);
#if SHENMUE_EVENT_LIBRARY_REVISION >= EVENT_LIBRARY_REVISION_4BTL
    FTBL.SetPlayerGameFlags(22, 0);
#endif
    FTBL.MapModelSlotCacheUtil(5);
    FTBL.MapModelSlotCacheUtil(3);
    FTBL.camCtrlHandleContextSwitch(0);
    FTBL.camCtrlSetFovTransitionTo(1);
    FTBL.camCtrlSetState(0);
#if SHENMUE_EVENT_LIBRARY_REVISION < EVENT_LIBRARY_REVISION_0549
    FTBL.FinishLoading();
#endif
    FTBL.SetPlayerControlEnabled(1);
    FTBL.HLib_DestroyTaskInternal(work->task);
}

void EVT_AT_ReadChr_Callback(void *data)
{
    EventReadCharactersWork *work = (EventReadCharactersWork *)data;
    const shen_name *name = &work->names[work->characterIndex];
    if (*name == EVENT_CHARACTER_INVALID) {
        FTBL.HLib_ResolveDeferredCallback(work->parentTask);
        FTBL.HLib_SetupTaskDelay(0);
    } else
        LoadOneNpcAndAdvance(work, *name);
}

int LoadOneNpcAndAdvance(EventReadCharactersWork *work, shen_name name)
{
    int index = work->characterIndex;
    if (index < 30) {
        EventReadCharacterRecord *record = &work->characters[index];
        switch (record->state) {
        case 0:
            record->character = name;
            if (FTBL.DoesScenarioCharacterExist(name)) {
                record->newlyLoaded = 0;
                record->state = 2;
                return 0;
            }
            FTBL.CheckWarmStartNpcRoute(name);
            record->newlyLoaded = 1;
            record->state = 1;
            return 0;
        case 1: {
            unsigned int result = FTBL.npcStreamOrLoadHuman(name);
            if (result > 1)
                FTBL.DebugLogToFileRemoved("%c%c%c%c : %d\n", name & 0xff, name >> 8 & 0xff, name >> 16 & 0xff, name >> 24, result);
            if (result == 1)
                record->state = 2;
            return 0;
        }
        case 2:
            record->restoreHeightGravity = 1;
            record->restoreGravity = 1;
            record->motionRootMode = FTBL.GetMotionTrackedRootMode(FTBL.scenFindMotionwork(name));
            record->previousBase = FTBL.SCEN_FindChar_Wrapper(name);
            FTBL.SetRefBoardParamsForCharaById(name, 1);
            FTBL.EV_SetCharacterAlphaEnabled(name, 0);
            FTBL.EV_SetCharaControllerFlags(name, 2, 0);
            FTBL.EV_SetCharaControllerFlags(name, 3, 0);
            SHENMUE_EVENT_BOUNDARY();
            FTBL.EV_SetCharaControllerFlags(name, 1, 0);
            FTBL.AT_SetLocalBaseChara(name, EVENT_CHARACTER_INVALID);
            FTBL.figpSetFlagsBlendForBlur(name, 2, 1);
            FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(name), 1);
            FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 0, 0);
            FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 3, 2);
            FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(name), 3);
            FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(name), 7, 0);
            FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(name), 66, 2, 0);
            FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(name), 69, 2, 0);
            FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(name), 71, 2, 0);
            work->characterIndex++;
            record->state = 3;
            return 1;
        default:
            return 1;
        }
    }
    return 0;
}

void RestoreEventCharacter(EventReadCharactersWork *work)
{
    EventReadCharacterRecord *record;
    shen_name name;
    if (work->characterIndex <= 0)
        return;
    record = &work->characters[--work->characterIndex];
    record->state = 0;
    name = record->character;
    if (!FTBL.DoesScenarioCharacterExist(name))
        return;
    FTBL.figpSetFlagsBlendForBlur(name, 5, 2);
    FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(name), 1);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 0, 0);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 3, 2);
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(name), record->motionRootMode);
    FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(name), 5, 0);
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(name), 66, 2, 0);
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(name), 69, 2, 0);
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(name), 71, 2, 0);
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(name), 0xf003, 0);
    FTBL.SetRefBoardParamsForCharaById(name, 0);
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
    FTBL.EV_SetCharaControllerFlags(name, 1, 1);
    FTBL.AT_SetLocalBaseChara(name, record->previousBase);
    FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(name), 0, 0);
    if (record->restoreHeightGravity)
        FTBL.EV_SetCharaControllerFlags(name, 3, 1);
    if (record->restoreGravity)
        FTBL.EV_SetCharaControllerFlags(name, 2, 1);
    if (record->newlyLoaded)
        FTBL.npcFree(name);
    if (FTBL.IsNpcRouteLoadedOrVisible(name))
        FTBL.npcLockUnlock(name);
}

void PushReadChrStack(EventReadCharactersWork *work, shen_name name)
{
    if (work->stackDepth < 32) {
        work->stack[work->stackDepth] = name;
        work->stackDepth++;
    }
}

void PopReadChrStack(EventReadCharactersWork *work)
{
    if (work->stackDepth > 0)
        work->stackDepth--;
}

int Queue_EVT_AT_ResultQTE(int *result, void *resultParameters, const EventQteData *qteData, int delay)
{
    EventQteAuthoredCallback beginSuccess = qteData->beginSuccess, beginFailure = qteData->beginFailure;
    HLibTask *task;
    EventQteResultTaskData *completion;
    if (result)
        *result = -1;
    if (beginSuccess)
        beginSuccess(resultParameters, delay + qteData->callbackTimeA);
    if (beginFailure)
        beginFailure(resultParameters, delay + qteData->callbackTimeA + qteData->callbackTimeB + qteData->callbackTimeC);
#if SHENMUE_EVENT_LIBRARY_REVISION >= EVENT_LIBRARY_REVISION_0337
    g_eventQteRetryFrames = qteData->retryReferenceFrames;
#endif
#if SHENMUE_EVENT_LIBRARY_REVISION < EVENT_LIBRARY_REVISION_4BTL
    EVT_AT_QTE(qteData->requiredQteStatus, qteData->maxTime, 0, delay + qteData->callbackTimeA + qteData->callbackTimeB, 0);
#else
    EVT_AT_QTE(qteData->requiredQteStatus, qteData->maxTime, 0, delay + qteData->callbackTimeA + qteData->callbackTimeB, 3);
#endif
    if (qteData->callbackTimeC > qteData->retryReferenceFrames)
        delay += qteData->callbackTimeA + qteData->callbackTimeB + qteData->callbackTimeC;
    else
        delay += qteData->callbackTimeA + qteData->callbackTimeB + qteData->retryReferenceFrames;
    task = FTBL.HLib_EnqueueTaskWithParameter(EVT_AT_ResultQTE_Callback, 4, 11, sizeof(EventQteResultTaskData), TASK_EVNT);
#if SHENMUE_EVENT_LIBRARY_REVISION < EVENT_LIBRARY_REVISION_FRBT
    FTBL.HLib_SetupTimedTask(task, delay);
#else
    FTBL.HLib_SetupTimedTask(task, delay + 5);
#endif
    completion = (EventQteResultTaskData *)FTBL.HLib_GetTaskParameter(task);
    completion->result = result;
    completion->resultParameters = resultParameters;
    completion->qteData = qteData;
#if SHENMUE_EVENT_LIBRARY_REVISION < EVENT_LIBRARY_REVISION_FRBT
    return delay + 1;
#else
    return delay + 5;
#endif
}

void EVT_AT_ResultQTE_Callback(void *data)
{
    EventQteResultTaskData *completion = (EventQteResultTaskData *)data;
    EventQteAuthoredCallback resultSuccess = completion->qteData->resultSuccess, resultFailure = completion->qteData->resultFailure;
    if ((unsigned short)(FTBL.GetCurrentQteStatus() & 0xfff) == completion->qteData->requiredQteStatus) {
        if (completion->result)
            *completion->result = 1;
        if (resultSuccess)
            resultSuccess(completion->resultParameters, 0);
    } else {
        if (completion->result)
            *completion->result = 0;
        if (resultFailure)
            resultFailure(completion->resultParameters, 0);
    }
    FTBL.HLib_DestroyCurrentTask();
}
#endif  
