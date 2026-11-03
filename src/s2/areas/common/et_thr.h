#ifndef SHENMUE_S2_AREAS_COMMON_ET_THR_H
#define SHENMUE_S2_AREAS_COMMON_ET_THR_H

#include "s2/events/event_dc.h"

#define THRBUF_MAX 10
#define THR_USRBUF_MAX 10
#define THR_TASK_SIZE 448
#define THR_ETERNAL_LOOP_STEPS 100
#define THR_NEXT_SEQUENCE 0xffff

#define THR_SEQ_KILL_OWN 1
#define THR_SEQ_WAIT_FRAMES 2
#define THR_SEQ_WAIT_EVENT_CAMERA 3
#define THR_SEQ_WAIT_CONVERSATION 4
#define THR_SEQ_WAIT_MOTION 5
#define THR_SEQ_WAIT_SPEAK_CHOICE 6
#define THR_SEQ_WAIT_TURN 7
#define THR_SEQ_WAIT_CHILD_RESULT 8
#define THR_SEQ_WAIT_CHILD_SEQUENCE 9
#define THR_SEQ_WAIT_CHILD_EXIT 10
#define THR_SEQ_WAIT_FADE 11
#define THR_SEQ_WAIT_FIGHT 12
#define THR_SEQ_WAIT_AUTHOR 13

typedef union ThrValue {
    int i;
    float f;
} ThrValue;
typedef struct Thr {
    unsigned short seq, nextSeq, savedSeq, prevSeq;
    shen_name id;
    int loop, nextLoop, level, index, resultIndex, exitIndex;
    HLibTask *task;
    struct Thr *parent;
    struct Thr *children[THRBUF_MAX];
    int returnValue, argument;
    int childDone[THRBUF_MAX];
    int childValue[THRBUF_MAX];
    shen_name turnName;
    const Vector3 *turnTarget;
    const unsigned short *turnMotions;
    unsigned int turnYaw;
    float turnStep;
    unsigned int turnFlags;
    int reserved0c4[4];
    void *work;
    ThrValue user[THR_USRBUF_MAX];
    int reserved100, speakSelection;
    shen_name motionName;
    int waitFrames;
    unsigned int stepCount;
    int lastChild, hasReturn, reserved11c;
    int buffer0[THR_USRBUF_MAX];
    int buffer1[THR_USRBUF_MAX];
    int buffer3[THR_USRBUF_MAX];
    int buffer2[THR_USRBUF_MAX];
} Thr;

extern int g_eventCallResult;
void EventDebugLog(const char *format, ...);

Thr *ThrCreate(HLibTaskCallback callback, shen_name id, void *work);
Thr *ThrMakeChild(Thr *parent, HLibTaskCallback callback, shen_name id, void *work, int index);
void ThrUnlink(Thr *thr);
int ThrKillOwn(Thr *thr);
void ThrKillChild(Thr *thr, shen_name id, int index);
void ThrKillChildren(Thr *thr);
void ThrKillTree(Thr *thr);
void ThrKillOther(Thr *thr, shen_name id);
void ThrKillOtherChildren(Thr *thr);
void ThrKillOtherTree(Thr *thr, shen_name id);
Thr *ThrGetChild(Thr *thr, int index);
Thr *ThrGetChildTask(Thr *thr, int index);
void ThrSetSeq(Thr *thr, unsigned short seq);
void ThrSetNextSeq(Thr *thr, unsigned short seq);
void ThrChangeSeq(Thr *thr, unsigned short seq, int loop);
void ThrPrintLink(Thr *thr);
void ThrDump(Thr *thr);
void ThrLogSeqChange(Thr *thr);
void ThrSetDebugId(shen_name id);
void ThrChangeSeqSave(Thr *thr, unsigned short seq, unsigned short saved, int loop);
void ThrReturnSeq(Thr *thr, int loop);
void ThrResetLoopCount(Thr *thr);
void ThrBeginStep(Thr *thr);
void ThrEndStep(Thr *thr);
int ThrIsContinue(Thr *thr);
int ThrGetSeq(Thr *thr);
void *ThrGetWork(Thr *thr);
shen_name ThrGetId(Thr *thr);
void ThrSetId(Thr *thr, shen_name id);
void ThrWaitFrames(Thr *thr, unsigned short next, int loop, int frames);
int ThrProcWaitFrames(Thr *thr);
void ThrWaitEventCamera(Thr *thr, unsigned short next, int loop);
int ThrProcWaitEventCamera(Thr *thr);
void ThrWaitConversation(Thr *thr, unsigned short next, int loop, int frames);
int ThrProcWaitConversation(Thr *thr);
void ThrWaitMotion(Thr *thr, unsigned short next, int loop, shen_name name);
int ThrProcWaitMotion(Thr *thr);
void ThrWaitSpeakChoice(Thr *thr, unsigned short next, int loop);
int ThrProcWaitSpeakChoice(Thr *thr);
int ThrGetSpeakChoice(Thr *thr);
void ThrSetSpeakChoice(Thr *thr, int choice);
void ThrWaitTurn(Thr *thr, unsigned short next, int loop, shen_name name, const Vector3 *target, const unsigned short *motions);
int ThrProcWaitTurn(Thr *thr);
void ThrWaitChildExit(Thr *thr, unsigned short next, int loop, int index);
int ThrProcWaitChildExit(Thr *thr);
void ThrWaitFade(Thr *thr, unsigned short next, int loop);
int ThrProcWaitFade(Thr *thr);
void ThrWaitFight(Thr *thr, unsigned short next, int loop);
int ThrProcWaitFight(Thr *thr);
void ThrWaitAuthor(Thr *thr, unsigned short next, int loop);
int ThrProcWaitAuthor(Thr *thr);
void ThrWaitChildResult(Thr *thr, unsigned short next, int loop, int index);
int ThrProcWaitChildResult(Thr *thr);
void ThrWaitChildSequence(Thr *thr, int loop, int index);
int ThrProcWaitChildSequence(Thr *thr);
void ThrSetChildDone(Thr *thr, int index, int done);
void ThrSignalParent(Thr *thr, int done);
void ThrReturnToParent(Thr *thr, int done);
void ThrSetChildArgument(Thr *thr, int index, int argument);
void ThrSetReturnValue(Thr *thr, int value);
int ThrGetReturnValue(Thr *thr);
int ThrGetChildValue(Thr *thr, int index);
int ThrGetUser(Thr *thr, int index);
void ThrSetUser(Thr *thr, int index, int value);
void ThrSetUserUnsigned(Thr *thr, int index, unsigned int value);
void ThrSetUserFloat(Thr *thr, int index, float value);
void ThrSetUserPointer(Thr *thr, int index, void *value);
void ThrSetUserName(Thr *thr, int index, shen_name value);
void ThrSetUserLong(Thr *thr, int index, long value);
void ThrSetUserUnsignedLong(Thr *thr, int index, unsigned long value);
void ThrSetUserConstPointer(Thr *thr, int index, const void *value);
int *ThrGetBuffer0Address(Thr *thr, int index);
int ThrGetBuffer0(Thr *thr, int index);
void ThrSetBuffer0(Thr *thr, int index, int value);
int *ThrGetBuffer1Address(Thr *thr, int index);
int ThrGetBuffer1(Thr *thr, int index);
void ThrSetBuffer1(Thr *thr, int index, int value);
int *ThrGetBuffer2Address(Thr *thr, int index);
int ThrGetBuffer2(Thr *thr, int index);
void ThrSetBuffer2(Thr *thr, int index, int value);
int ThrGetBuffer3(Thr *thr, int index);
void ThrSetBuffer3(Thr *thr, int index, int value);
int ThrProcess(Thr *thr);

#endif
