#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_duck.hpp"

int duckVoiceCount = 0;
static char duckVoicePrefixes[DUCK_VOICE_PREFIX_COUNT][DUCK_VOICE_PREFIX_LENGTH];
static char duckVoices[DUCK_VOICE_COUNT][DUCK_VOICE_LENGTH];

void DuckVoiceSequence_Callback(void *data)
{
    DuckVoiceWork *work = (DuckVoiceWork *)data;
    if (!FTBL.cvsHasValidConverseData(0)) {
        if (duckVoices[work->index][0]) {
            EVT_AT_SetVoice(duckVoices[work->index], work->delay);
            if (work->delay > 0)
                FTBL.HLib_SetupTaskDelay(work->delay);
            work->index++;
            work->ticks = 0;
            if (duckVoices[work->index][0] || !(work->mode & DUCK_VOICE_REPEAT))
                return;
        }
        if (work->parent)
            FTBL.HLib_ResolveDeferredCallback(work->parent);
        FTBL.HLib_DestroyCurrentTask();
    } else {
        if (work->ticks++ > 600)
            FTBL.scenCvsCtrlFree(0);
        if (FTBL.CheckMediatedDoublePress() && (work->mode & DUCK_VOICE_SKIPPABLE))
            FTBL.scenCvsCtrlFree(0);
    }
}

void QueueDuckVoiceSequence(HLibTask *parent, int delay, unsigned int mode, int timer)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(DuckVoiceSequence_Callback, 6, 11, sizeof(DuckVoiceWork), TASK_EVNT);
    DuckVoiceWork *work;
    if (timer > 0)
        FTBL.HLib_SetupTimedTask(task, timer);
    work = (DuckVoiceWork *)FTBL.HLib_GetTaskParameter(task);
    work->delay = delay;
    work->mode = mode;
    work->parent = parent;
    work->index = 0;
    work->reserved0c = 0;
    work->ticks = 0;
    duckVoices[duckVoiceCount][0] = 0;
    duckVoiceCount = 0;
    if (work->parent)
        FTBL.HLib_SetupTimedTask(work->parent, 0);
}

void SetDuckVoicePrefix(int prefix, const char *text)
{
    strcpy(duckVoicePrefixes[prefix], text);
}

void AddDuckVoice(int prefix, const char *suffix)
{
    strcpy(duckVoices[duckVoiceCount], duckVoicePrefixes[prefix]);
    strcat(duckVoices[duckVoiceCount], suffix);
    duckVoiceCount++;
}

void PlayDuckVoice(int prefix, const char *suffix, int delay)
{
    char voice[64];
    strcpy(voice, duckVoicePrefixes[prefix]);
    strcat(voice, suffix);
    EVT_AT_SetVoice(voice, delay);
}
#else
#endif
