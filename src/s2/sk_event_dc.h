#ifndef SHENMUE_S2_SK_EVENT_DC_H
#define SHENMUE_S2_SK_EVENT_DC_H
#include "s2/events/event_dc.h"

#define SK_EVENT_TASK TASK_EVNT
#define SK_VOICE_LIST_CAPACITY 10

#define SK_EVENT_REVISION_TKYB 1
#define SK_EVENT_REVISION_ISTB 2
#if !defined(SHENMUE_EVENT_SK_REVISION)
#define SHENMUE_EVENT_SK_REVISION SK_EVENT_REVISION_ISTB
#endif

typedef int (*SkQteCallback)(void);
typedef struct SkVoiceList {
    HLibTask *parent;
    const char *prefix;
    const char *voices[SK_VOICE_LIST_CAPACITY];
    void *conversation;
    int delays[SK_VOICE_LIST_CAPACITY];
    unsigned char index, count, state;
} SkVoiceList;
typedef struct SkSimpleQteWork {
    HLibTask *task, *parent;
    HLibTaskCallback success, failure;
    int referenceFrames;
    int *elapsed;
    int maximumFrames;
    unsigned int button;
    unsigned short *status;
    unsigned short phase;
} SkSimpleQteWork;
typedef struct SkChoiceWaitWork {
    HLibTask *task, *parent;
    short choices[4];
    int timeout;
    int defaultChoice;
    unsigned short phase;
} SkChoiceWaitWork;
typedef struct SkMoneyIconWork {
    HLibTask *task, *parent;
    int maximum;
    short step;
    unsigned short phase;
} SkMoneyIconWork;
typedef struct SkAddMoneyWork {
    HLibTask *task, *parent;
    int amount;
    unsigned short phase;
} SkAddMoneyWork;
typedef struct SkAuthorStartWork {
    HLibTask *task, *parent;
    int command;
    unsigned short phase;
    HLibTaskCallback callback;
    void *callbackData;
} SkAuthorStartWork;
typedef struct SkNpcWaitWork {
    HLibTask *task, *parent;
    int reserved08, reserved0c;
} SkNpcWaitWork;
typedef struct SkWaitQteWork {
    HLibTask *task, *parent;
    SkQteCallback success, failure;
    int referenceFrames;
    int *elapsed;
    int maximumFrames;
    unsigned int button;
    unsigned short *status;
    short phase;
    int *succeeded;
} SkWaitQteWork;

extern int scenarioQteRetryCount;
void SkVoiceListInit(SkVoiceList *list, const char *prefix);
void SkVoiceListAdd(SkVoiceList *list, const char *voice, int delay);
void QueueSkVoiceList(SkVoiceList *list);
void SkVoiceList_Callback(void *data);
void QueueSkSimpleQteWait(int *result, unsigned int button, int referenceFrames, int maximumFrames, HLibTaskCallback success, HLibTaskCallback failure,
    int delay);
void SkSimpleQteWait_Callback(void *data);
void QueueSkChoiceWait(HLibTask *parent, int first, int second, int third, int fourth, int defaultChoice, int timeout, int delay);
void SkChoiceWaitCallback(void *data);
void QueueSkMoneyIconWait(HLibTask *parent, short maximum, short step, int delay);
void SkMoneyIcon_Callback(void *data);
void QueueSkAddMoney(HLibTask *parent, int amount, int delay);
void SkAddMoney_Callback(void *data);
void QueueSkAuthorStart(HLibTask *parent, int command, HLibTaskCallback callback, void *callbackData, int delay);
void EVT_SK_AuthStart_Callback(void *data);
void QueueSkNpcWait(HLibTask *parent);
void SkNpcWait_Callback(void *data);
void ResetSkQteRetryCount(void);
int GetSkQteRetryCount(void);
void IncrementSkQteRetryCount(void);
void QueueSkQteWait(int *result, unsigned int button, int referenceFrames, int baseMaximumFrames, SkQteCallback success, SkQteCallback failure, int delay);
void EVT_SK_WaitQTE_Callback(void *data);
void SaveSkSound(void);
void RestoreSkSound(void);

#endif
