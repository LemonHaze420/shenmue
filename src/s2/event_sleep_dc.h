#ifndef SHENMUE_S2_EVENT_SLEEP_DC_H
#define SHENMUE_S2_EVENT_SLEEP_DC_H
#include "s2/events/event_dc.h"

#if !defined(SHENMUE_EVENT_SLEEP_REVISION)
#define SHENMUE_EVENT_SLEEP_REVISION 2
#endif

#define SLEEP_EVENT_TASK SHEN_NAME4('i', 'n', 'n', '_')
typedef void (*SleepContinuation)(void);
typedef void (*SleepPromptCallback)(int accepted);
typedef struct SleepTriggerBox {
    Vector3 minimum;
    float reserved0c[6];
    Vector3 maximum;
    float reserved30[3];
    unsigned int state;
} SleepTriggerBox;
typedef struct SleepEventData {
    char package[32];
    shen_name actors[10];
    SleepContinuation continuation, cancelled;
    SleepPromptCallback prompt;
    unsigned int pendingResources;
    int subtitleFrames;
    unsigned int flags;
    shen_name nextScene;
#if SHENMUE_EVENT_SLEEP_REVISION >= 2
    int icon;
#endif
    int nextEntry, savedFilmBars;
} SleepEventData;
extern SleepEventData sleepEventData;

void SleepCopyString(char *destination, const char *source);
void SleepCopyNames(shen_name *destination, const shen_name *source);
void AdvanceSleepToMorning(void);
void QueueSleepCallback(SleepContinuation accepted, SleepContinuation cancelled, int expectedSchedule, unsigned int flags);
void SleepIconCallback(void *taskData);
void FinishSleepSelection(void);
void SleepSelectionCallback(void *taskData);
void QueueRandomSleepAuthor(const shen_name *authors, int count, SleepContinuation continuation, unsigned int flags);
void SleepAuthorWaitCallback(void *taskData);
void SelectBadEndingState(SleepContinuation continuation);
void SleepBadEndCallback(void *taskData);
void SleepBadEndNoticeCallback(void *taskData);
void SleepNoticeWaitCallback(void *taskData);
void SleepSubtitleStartCallback(void *taskData);
void SleepSubtitleWaitCallback(void *taskData);
void QueueSleepMemo(SleepContinuation continuation, unsigned int flags);
void SleepMemoWaitCallback(void *taskData);
void QueueSleepPackageActors(const char *package, const shen_name *actors, SleepContinuation continuation);
void SleepPackageRetryCallback(void *taskData);
void SleepPackageWaitCallback(void *taskData);
void FreeSleepPackageActors(void);
void DisableSleepControl(void);
void EnableSleepControl(void);
void QueueSleepSceneChange(shen_name scene, int entry, unsigned int flags);
void SleepSceneChangeCallback(void *taskData);
void HideSleepActor(shen_name actor);
unsigned int shenTrySleepTrigger(SleepTriggerBox *box);
void QueueSleepPromptTask(SleepPromptCallback callback);
void SleepPromptTaskCallback(void *taskData);
void QueueRandomSleepAuthorTask(const shen_name *authors, int count, SleepContinuation continuation, unsigned int flags);
void SleepAuthorTaskCallback(void *taskData);
void QueueSleepSubtitleTask(SleepContinuation continuation);
void SleepSubtitleTaskStartCallback(void *taskData);
void SleepSubtitleTaskWaitCallback(void *taskData);
void QueueSleepMemoTask(SleepContinuation continuation, unsigned int flags);
void SleepMemoTaskCallback(void *taskData);
void QueueSleepPackageActorsTask(const char *package, const shen_name *actors, SleepContinuation continuation);
void SleepPackageTaskRetryCallback(void *taskData);
void SleepPackageTaskWaitCallback(void *taskData);
int GetBadEndingState(void);

#endif
