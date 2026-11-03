#ifndef SHENMUE_S2_EVENT_QUEUED_DC_H
#define SHENMUE_S2_EVENT_QUEUED_DC_H
#include "s2/events/event_dc.h"

#define EVENT_LIBRARY_REVISION_TKYB 1
#define EVENT_LIBRARY_REVISION_FRBT 2
#define EVENT_LIBRARY_REVISION_0337 3
#define EVENT_LIBRARY_REVISION_WRS2 5
#define EVENT_LIBRARY_REVISION_0009 10
#define EVENT_LIBRARY_REVISION_AKTM 20
#define EVENT_LIBRARY_REVISION_Q200 25
#define EVENT_LIBRARY_REVISION_4BTL 28
#define EVENT_LIBRARY_REVISION_0549 30
#define EVENT_LIBRARY_REVISION_DGCT 40
#ifndef SHENMUE_EVENT_LIBRARY_REVISION
#define SHENMUE_EVENT_LIBRARY_REVISION EVENT_LIBRARY_REVISION_DGCT
#endif
#if SHENMUE_EVENT_LIBRARY_REVISION < EVENT_LIBRARY_REVISION_WRS2
#define EVENT_VOICE_LIST_COUNT 10
#define EVENT_VOICE_LIST_LENGTH 10
#else
#define EVENT_VOICE_LIST_COUNT 16
#define EVENT_VOICE_LIST_LENGTH 12
#endif

typedef struct EventSetCamera {
    int cleanup, camera;
    shen_name eye, target;
    int context, angle;
} EventSetCamera;
typedef struct EventSetCameraRotation {
    int cleanup;
    float x, y, duration;
} EventSetCameraRotation;
typedef struct EventSetMotion {
    int cleanup;
    shen_name character;
    unsigned int animation;
    float frame, speed;
    unsigned int flags;
} EventSetMotion;
typedef struct EventSetPosition {
    int cleanup;
    shen_name character, parent;
    Vector3 position;
} EventSetPosition;
typedef struct EventSetRawAngle {
    int cleanup;
    shen_name character, parent;
    Vector3i rotation;
} EventSetRawAngle;
typedef struct EventSetAngle {
    int cleanup;
    shen_name character, parent;
    float degrees;
} EventSetAngle;
typedef struct EventSetPositionAngle {
    int cleanup;
    shen_name character, parent;
    Vector3 position;
    float degrees;
} EventSetPositionAngle;
typedef struct EventSetVoice {
    int cleanup, mode;
#if SHENMUE_EVENT_LIBRARY_REVISION < EVENT_LIBRARY_REVISION_0009
    const char *voice;
#elif defined(SHENMUE_EVENT_LIBRARY_PATI)
    char voice[64];
#else
    char voice[32];
#endif
} EventSetVoice;
typedef struct EventVoiceListWork {
    HLibTask *parent;
    int voiceDelayFrames, index, skipPressFrames;
    unsigned int mode;
    char voices[EVENT_VOICE_LIST_COUNT][EVENT_VOICE_LIST_LENGTH];
} EventVoiceListWork;
typedef struct EventSetSound {
    int cleanup;
    unsigned int sound;
} EventSetSound;
typedef struct EventSetSample {
    int cleanup;
    int sample;
    int fadeOut;
} EventSetSample;
typedef struct EventChoiceIcon {
    int cleanup;
    short choices[4];
    int timeout;
    int defaultChoice;
    int mode;
} EventChoiceIcon;
typedef struct EventMoneyUI {
    int maximum;
    short duration;
    short step;
    int cleanup;
} EventMoneyUI;
typedef struct EventMoneyCount {
    int cleanup;
    int cash;
} EventMoneyCount;
typedef void (*EventRepeatFunction)(void *argument);
typedef struct EventRepeatCall {
    EventRepeatFunction function;
    void *argument;
    int count;
    int index;
    int size;
} EventRepeatCall;
typedef void (*EventRepeatDataFunction)(void *data, int index);
typedef struct EventRepeatDataCall {
    EventRepeatDataFunction function;
    void *argument;
    int count;
    int index;
    int size;
} EventRepeatDataCall;
typedef struct EventFilmControl {
    int cleanup;
    unsigned int frames;
    int startRed;
    int startGreen;
    int startBlue;
    int startAlpha;
    int targetRed;
    int targetGreen;
    int targetBlue;
    int targetAlpha;
} EventFilmControl;
typedef struct EventQtePrompt {
    int cleanup;
    unsigned short requiredStatus;
    short maximumFrames;
    int resultState;
    int displayState;
} EventQtePrompt;
typedef struct EventCling {
    int cleanup;
    shen_name target;
    shen_name character;
    int bone;
    Vector3 position;
    Vector3i rotation;
    int mode;
} EventCling;
typedef struct EventCharaDisplay {
    int cleanup;
    shen_name character;
    int hide;
} EventCharaDisplay;
typedef struct EventDisplayBlur {
    shen_name character;
    unsigned char mode;
    unsigned char scale;
    unsigned char direction;
    int flags;
    int frames;
} EventDisplayBlur;
typedef struct EventControlHand {
    int cleanup;
    shen_name character;
    int side;
    void *command;
    int ticks;
} EventControlHand;
typedef struct EventFaceLooks {
    int cleanup;
    shen_name character;
    int expression;
    int duration;
    float intensity;
} EventFaceLooks;
#define EVENT_CHARACTER_INVALID 0xffffffffU
typedef struct EventSetPoint {
    shen_name character, target;
    int targetBone, trackingMode, positionMode, duration;
    unsigned int flags;
    int frame;
#if SHENMUE_EVENT_LIBRARY_REVISION >= EVENT_LIBRARY_REVISION_Q200
    int cleanup;
#endif
} EventSetPoint;
typedef struct EventPlayDynamics {
    shen_name character;
    const void *resource;
    int trackCount;
    const int *parts;
    float endFrame, frameStep, currentFrame;
} EventPlayDynamics;
typedef struct ClearNpcTask {
    void (*callback)(void *parameter);
    void *parameter;
    int reserved08, state;
} ClearNpcTask;

typedef struct EventDateTime {
    signed char year, month, day;
    unsigned char week;
    signed char hour, minute, second;
    signed char reserved07[5];
} EventDateTime;
typedef struct EventReadCharacterRecord {
    unsigned int state;
    int newlyLoaded;
    shen_name character, previousBase;
    int restoreGravity, restoreHeightGravity, reserved18, motionRootMode;
} EventReadCharacterRecord;
typedef struct EventReadCharactersWork {
    EventReadCharacterRecord characters[30];
    shen_name stack[32];
    HLibTask *task, *parentTask;
    const shen_name *names;
    int characterIndex, stackDepth, maxNpc, needsReload;
} EventReadCharactersWork;
typedef void (*EventQteAuthoredCallback)(void *resultParameters, int delayFrames);
typedef struct EventQteData {
    unsigned short requiredQteStatus;
    short retryReferenceFrames, maxTime, callbackTimeA, callbackTimeB, callbackTimeC;
    EventQteAuthoredCallback beginSuccess, beginFailure, resultSuccess, resultFailure;
} EventQteData;
typedef struct EventQteResultTaskData {
    int *result;
    void *resultParameters;
    const EventQteData *qteData;
} EventQteResultTaskData;
extern int g_eventQteFailureCount, g_eventQteRetryFrames;
void EVT_AT_SetCam_Callback(void *data);
void EVT_AT_SetCam(int camera, shen_name eye, shen_name target, int context, int angle, int delay);
void EVT_AT_SetCamRot_Callback(void *data);
void EVT_AT_SetCamRot(float x, float y, float duration, int delay);
void EVT_AT_SetMot_Callback(void *data);
int EVT_AT_SetMot(shen_name character, unsigned int animation, float frame, float speed, unsigned int flags, int delay);
void EVT_AT_SetPosRot(void *data);
void EVT_AT_SetPos(shen_name character, shen_name parent, float x, float y, float z, int delay);
void EVT_AT_SetAng_Callback(void *data);
void EVT_AT_SetAng(shen_name character, shen_name parent, float x, float y, float z, int delay);
void EVT_AT_SetAngY_Callback(void *data);
void EVT_AT_SetAngY(shen_name character, shen_name parent, float degrees, int delay);
void EVT_AT_SetPosAng_Callback(void *data);
void EVT_AT_SetPosAng(shen_name character, shen_name parent, float x, float z, float degrees, int delay);
void EVT_AT_SetVoice_Callback(void *data);
void EVT_AT_SetVoice(const char *voice, int delay);
void EVT_AT_SetVoiceOutsideConversation(const char *voice, int delay);
void SetEventVoicePrefix(const char *prefix);
void EventVoiceListCallback(void *data);
void QueueEventVoiceList(HLibTask *parent, const char **voices, int voiceDelayFrames, unsigned int mode, int delay);
void EVT_AT_SetSound_Callback(void *data);
void EVT_AT_SetSound(unsigned int sound, int delay);
void EVT_AT_SetSample_Callback(void *data);
void EVT_AT_SetSample(int sample, int fadeOut, int delay);
void EVT_AT_KsIcon_Callback(void *data);
void EVT_AT_KsIcon_Enqueue(int first, int second, int third, int fourth, int timeout, int defaultChoice, int mode, int delay);
void EVT_AT_MoneyUI_Callback(void *data);
void EVT_AT_MoneyUI(int maximum, short duration, short step, int delay);
void EVT_AT_StartMoneyCount_Callback(void *data);
void EVT_AT_StartMoneyCount(int cash, int delay);
void EVT_RepeatCall_Callback(void *data);
void EVT_RepeatCallData_Callback(void *data);
void EVT_RepeatCall(EventRepeatFunction function, void *argument, int delay, int count);
void *EVT_RepeatCallData(EventRepeatDataFunction function, int size, int delay, int count);
void EVT_AT_MenCtrl_Callback(void *data);
void EVT_AT_MenCtrl(unsigned int frames, int startRed, int startGreen, int startBlue, int startAlpha, int targetRed, int targetGreen, int targetBlue,
    int targetAlpha, int delay);
void EVT_SetQteRetryFrames(short frames);
void EVT_IncrementQteFailureCount(void);
void EVT_AT_Qte_Callback(void *data);
void EVT_AT_QTE(unsigned short requiredStatus, short maximumFrames, int resultState, int delay, int displayState);
void EVT_AT_Cling(void *data);
void Queue_EVT_AT_Cling(shen_name target, shen_name character, int bone, const Vector3 *position, const Vector3i *rotation, int delay, int mode);
void EVT_AT_CharaDisp_Callback(void *data);
void EVT_AT_CharaDisp(shen_name character, int delay, int hide);
void EVT_AT_CharaDraw_Callback(void *data);
void EVT_AT_CharaDraw(shen_name character, int delay, int hide);
void EVT_AT_DispBlur_Callback(void *data);
void EVT_AT_DispBlur(shen_name character, unsigned char mode, unsigned char scale, unsigned char direction, int flags, int frames, int delay);
void EVT_AT_CtrlHandUp_Callback(void *data);
void Queue_EVT_AT_CtrlHandUp_1(shen_name character, int side, void *command, int ticks, int delay);
void EVT_AT_SetFaceLooks_Callback(void *data);
void EVT_AT_SetFaceLooks(shen_name character, int expression, int duration, float intensity, int delay);
void EVT_AT_SetPoint_Callback(void *data);
void EVT_AT_SetPoint(shen_name character, shen_name target, int targetBone, int trackingMode, int positionMode, int delay, int duration, unsigned int flags);
void EVT_AT_PlayDynamix_Callback(void *data);
HLibTask *Queue_EVT_AT_PlayDynamix(shen_name character, const void *resource, float frame, float endFrame, int trackCount, const int *parts, float frameStep,
    int delay);
void QueueClearNpcs(void (*callback)(void *parameter), void *parameter);
void ClearNpcs(void *data);
void SaveEventDateTime(void);
void RestoreEventDateTime(void);
EventReadCharactersWork *Queue_EVT_AT_ReadChr(HLibTask *parent, const shen_name *names);
void ET_AT_KillWork(EventReadCharactersWork *work);
void EVT_AT_ReadChr_Callback(void *data);
int LoadOneNpcAndAdvance(EventReadCharactersWork *work, shen_name name);
void RestoreEventCharacter(EventReadCharactersWork *work);
void PushReadChrStack(EventReadCharactersWork *work, shen_name name);
void PopReadChrStack(EventReadCharactersWork *work);
int Queue_EVT_AT_ResultQTE(int *result, void *resultParameters, const EventQteData *qteData, int delay);
void EVT_AT_ResultQTE_Callback(void *data);

#endif
