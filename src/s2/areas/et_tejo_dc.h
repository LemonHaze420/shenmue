#ifndef SHENMUE_S2_AREAS_ET_TEJO_DC_H
#define SHENMUE_S2_AREAS_ET_TEJO_DC_H
#include "s2/event_queued_dc.h"

#define REN_QTE_PLAYER SHEN_NAME4('R', 'Y', 'O', '_')
#define REN_QTE_REN SHEN_NAME4('R', 'E', 'N', '_')
#define REN_QTE_BASE SHEN_NAME4('B', 'A', 'S', 'E')
#define REN_QTE_SECOND_BASE SHEN_NAME4('B', 'A', 'S', '2')
#define REN_QTE_DOOR SHEN_NAME4('D', 'O', 'R', '1')
#define REN_QTE_BROKEN_DOOR SHEN_NAME4('D', 'O', 'R', '2')
#define REN_QTE_KD7 SHEN_NAME4('K', 'D', '7', '_')
#define REN_QTE_KK1 SHEN_NAME4('K', 'K', '1', '_')
#define REN_QTE_KT1 SHEN_NAME4('K', 'T', '1', '_')
#define REN_QTE_KU7 SHEN_NAME4('K', 'U', '7', '_')
#define REN_QTE_SOUND_FADE_TASK SHEN_NAME4('s', 'n', 'd', 'f')
#define REN_QTE_DOOR_TASK SHEN_NAME4('d', 'o', 'o', 'r')
#define REN_QTE_DOOR_DYNAMICS "DOOR"
#define REN_QTE_INIT SHEN_NAME4('I', 'N', 'I', 'T')
#define REN_QTE_START SHEN_NAME4('S', 'T', 'R', 'T')
#define REN_QTE_TEST SHEN_NAME4('T', 'E', 'S', 'T')
#define REN_QTE_INPUT SHEN_NAME4('Q', 'T', 'E', '_')
#define REN_QTE_SUCCESS SHEN_NAME4('S', 'U', 'C', 'C')
#define REN_QTE_FAILURE SHEN_NAME4('F', 'A', 'I', 'L')
#define REN_QTE_FINISH SHEN_NAME4('F', 'I', 'N', 'I')
#define REN_QTE_CAPTURE SHEN_NAME4('C', 'A', 'P', 'T')
#define REN_QTE_DOOR_PARTS 20
typedef struct RenQteCamera {
    int camera;
    shen_name attachment;
    int delay;
} RenQteCamera;
typedef struct RenQteVoice {
    const char *voice;
    int delay;
} RenQteVoice;
typedef struct RenQteEntry {
    unsigned int type;
    float startX, startZ;
    int startYaw;
    float endX, endZ;
    int endYaw;
    unsigned int command;
    float approachDistance;
    RenQteCamera *initialCameras, *successCameras, *failureCameras;
    RenQteVoice *initialVoices, *successVoices, *failureVoices;
    int successFrames, failureFrames;
} RenQteEntry;
typedef struct RenQteWork {
    shen_name base;
    int state;
    int retryCount, lives, doorAttempts, timer, hidePursuers, reserved1c;
    float triggerDistance;
    RenQteEntry *entry;
} RenQteWork;
typedef struct RenQteCrushDoorWork {
    int wait, frame, lastFrame, restoreIntactDoor;
} RenQteCrushDoorWork;
typedef void (*RenQteResultCallback)(RenQteWork *work);
extern int renQteDoorParts[REN_QTE_DOOR_PARTS];
extern RenQteResultCallback renQteSuccessCallbacks[8], renQteFailureCallbacks[8];
extern int renQtePromptPending, renQteRetryDifficulty;
int UpdateRenQteEvent(RenQteWork *work);
void PlayRenQteCameras(RenQteCamera *cameras);
void PlayRenQteVoices(RenQteVoice *voices);
void QueueRenQteFadeOutSound(int delay);
void RenQteFadeOutSoundCallback(void *data);
void InitializeRenQteEntry(RenQteWork *work);
void BeginRenQteDoorApproach(RenQteWork *work);
void BeginRenQteExitApproach(RenQteWork *work);
void BeginRenQteRightExitApproach(RenQteWork *work);
void BeginRenQteRunApproach(RenQteWork *work);
void WaitRenQteApproach(RenQteWork *work);
void FinishRenQteExit(RenQteWork *work);
void CaptureRenQtePursuers(RenQteWork *work);
void FinishRenQteRightExit(RenQteWork *work);
void PollRenQteInput(RenQteWork *work);
int IsRenQteTimerDone(RenQteWork *work);
void TurnRenQteLeftSuccess(RenQteWork *work);
void TurnRenQteLeftFailure(RenQteWork *work);
void TurnRenQteRightSuccess(RenQteWork *work);
void TurnRenQteRightFailure(RenQteWork *work);
void ContinueRenQteRunSuccess(RenQteWork *work);
void ContinueRenQteRunFailure(RenQteWork *work);
void JumpRenQtePitSuccess(RenQteWork *work);
void JumpRenQtePitFailure(RenQteWork *work);
void DodgeRenQtePoleSuccess(RenQteWork *work);
void DodgeRenQtePoleFailure(RenQteWork *work);
void SwitchRenQteLeaderSuccess(RenQteWork *work);
void SwitchRenQteLeaderFailure(RenQteWork *work);
void BreakRenQteDoorSuccess(RenQteWork *work);
void QueueRenQteCrushDoor(int lastFrame, int restoreIntactDoor, int delay);
void RenQteCrushDoorCallback(void *data);
void BreakRenQteDoorFailure(RenQteWork *work);
void RenQteUnusedResult(RenQteWork *work);

#endif
