#if defined(SHENMUE_EVENT_OVERLAY)

#include "s2/areas/common/et_thr.h"

#if SHENMUE_ET_THR_REVISION >= 2
#define EventDebugLog FTBL.DebugLogToFileRemoved
#define THR_LINE(line) ((line) >= 515 ? (line) + 11 : (line))
#else
#define THR_LINE(line) (line)
#endif
#define THR_ASSERT(condition, line) if (!(condition)) FTBL.EventAssertNullsub(__FILE__, THR_LINE(line))
#define THR_NAME_CHARACTERS(name) (name) & 0xff, ((name) >> 8) & 0xff, ((name) >> 16) & 0xff, (name) >> 24

static shen_name thrDebugId;

Thr *ThrCreate(HLibTaskCallback callback, shen_name id, void *work)
{
    HLibTask *task;
    Thr *thr;
    int i;
    THR_ASSERT(callback, 128);
    task = FTBL.HLib_EnqueueTaskWithParameter(callback, 4, 11, THR_TASK_SIZE, TASK_EVNT);
    thr = (Thr *)FTBL.HLib_GetTaskParameter(task);
    thr->task = task;
    thr->work = work;
    thr->seq = 0;
    thr->nextSeq = 0;
    thr->waitFrames = 0;
    thr->level = 0;
    thr->id = id;
    thr->hasReturn = 0;
    thr->motionName = 0;
    thr->parent = 0;
    thr->argument = 0;
    for (i = 0; i < THRBUF_MAX; i++) {
        thr->children[i] = 0;
        thr->childDone[i] = 0;
    }
    return thr;
}

Thr *ThrMakeChild(Thr *parent, HLibTaskCallback callback, shen_name id, void *work, int index)
{
    Thr *child;
    THR_ASSERT(parent, 164);
    if (index >= THRBUF_MAX) {
        EventDebugLog("THR : idx is over THRBUF_MAX ! : %d\n", index);
        ThrDump(parent);
        THR_ASSERT(0, 165);
    }
    if (!callback) {
        EventDebugLog("THR : Make Task Error !(func address is NULL)\n");
        ThrDump(parent);
        EventDebugLog("child %d / id %c%c%c%c\n", index, THR_NAME_CHARACTERS(id));
        THR_ASSERT(0, 171);
    }
    child = ThrCreate(callback, id, work);
    if (parent->children[index] == 0) {
        parent->children[index] = child;
    } else {
        EventDebugLog("Make Child Task Error !(alread exist)\n");
        ThrDump(parent);
        EventDebugLog("child %d / id %c%c%c%c\n", index, THR_NAME_CHARACTERS(id));
        THR_ASSERT(0, 183);
    }
    child->parent = parent;
    child->index = index;
    child->level = parent->level + 1;
    parent->lastChild = index;
    parent->childDone[index] = 0;
    return child;
}

void ThrUnlink(Thr *thr)
{
    int i;
    if (thr->parent) {
        thr->parent->children[thr->index] = 0;
        thr->parent = 0;
    }
    for (i = 0; i < THRBUF_MAX; i++) {
        if (thr->children[i]) {
            thr->children[i]->parent = 0;
            thr->children[i] = 0;
        }
    }
}

int ThrKillOwn(Thr *thr)
{
    int i;
    THR_ASSERT(thr, 225);
    for (i = 0; i < THRBUF_MAX; i++) {
        if (thr->children[i]) {
            EventDebugLog("THR : Kill own task, but child link active !\n");
            ThrDump(thr);
            EventDebugLog("child %d / id %c%c%c%c\n", i, THR_NAME_CHARACTERS(thr->children[i]->id));
            THR_ASSERT(0, 233);
        }
    }
    EventDebugLog("Kill Own Task : %c%c%c%c\n", THR_NAME_CHARACTERS(thr->id));
    ThrUnlink(thr);
    FTBL.HLib_DestroyCurrentTask();
    return g_eventCallResult;
}

void ThrKillChild(Thr *thr, shen_name id, int index)
{
    if (index >= THRBUF_MAX) {
        EventDebugLog("THR : idx is over THRBUF_MAX ! : %d\n", index);
        ThrDump(thr);
        THR_ASSERT(0, 252);
    }
    if (thr->children[index] == 0) {
        EventDebugLog("THR : Warning, Kill child task, but child task is not exist !\n");
        ThrDump(thr);
        EventDebugLog("child idx = %d \n", index);
    } else {
        ThrKillOther(thr->children[index], id);
    }
}

void ThrKillChildren(Thr *thr)
{
    int i;
    for (i = 0; i < THRBUF_MAX; i++) {
        if (thr->children[i]) {
            ThrKillChildren(thr->children[i]);
            ThrKillChild(thr, 0, i);
        }
    }
}

void ThrKillTree(Thr *thr)
{
    THR_ASSERT(thr, 283);
    ThrKillChildren(thr);
    ThrKillOwn(thr);
}

void ThrKillOther(Thr *thr, shen_name id)
{
    int i;
    if (thr == 0) {
        EventDebugLog("THR : Warning, Kill other task, but her is not exist !\n");
        return;
    }
    if (id && thr->id != id) {
        EventDebugLog("THR : Kill other(child) task, but her user_id is not one !\n");
        ThrDump(thr);
        EventDebugLog("user_id %c%c%c%c / thr->user_id c%c%c%c\n", THR_NAME_CHARACTERS(id), THR_NAME_CHARACTERS(thr->id));
        THR_ASSERT(0, 306);
    }
    for (i = 0; i < THRBUF_MAX; i++) {
        if (thr->children[i]) {
            EventDebugLog("THR : Warning, Kill other(child) task, but her child link active !\n");
            ThrDump(thr);
            EventDebugLog("child idx = %d / child id = %c%c%c%c\n", i, THR_NAME_CHARACTERS(thr->children[i]->id));
            THR_ASSERT(0, 314);
        }
    }
    if (thr->task) {
        EventDebugLog("Kill Ohter Task : %c%c%c%c\n", THR_NAME_CHARACTERS(thr->id));
        ThrUnlink(thr);
        FTBL.HLib_DestroyTaskInternal(thr->task);
    }
}

void ThrKillOtherChildren(Thr *thr)
{
    int i;
    for (i = 0; i < THRBUF_MAX; i++) {
        if (thr->children[i]) {
            ThrKillOtherChildren(thr->children[i]);
            ThrKillChild(thr, 0, i);
        }
    }
}

void ThrKillOtherTree(Thr *thr, shen_name id)
{
    THR_ASSERT(thr, 346);
    ThrKillOtherChildren(thr);
    ThrKillOther(thr, id);
}

Thr *ThrGetChild(Thr *thr, int index)
{
    THR_ASSERT(thr, 357);
    if (index >= THRBUF_MAX) {
        EventDebugLog("THR : idx is over THRBUF_MAX ! : %d\n", index);
        ThrDump(thr);
        THR_ASSERT(0, 358);
    }
    return thr->children[index];
}

Thr *ThrGetChildTask(Thr *thr, int index)
{
    THR_ASSERT(thr, 368);
    if (index >= THRBUF_MAX) {
        EventDebugLog("THR : idx is over THRBUF_MAX ! : %d\n", index);
        ThrDump(thr);
        THR_ASSERT(0, 369);
    }
    return thr->children[index];
}

void ThrSetSeq(Thr *thr, unsigned short seq)
{
    THR_ASSERT(thr, 380);
    thr->prevSeq = thr->seq;
    thr->seq = seq;
    thr->loop = thr->nextLoop;
}

void ThrSetNextSeq(Thr *thr, unsigned short seq)
{
    THR_ASSERT(thr, 393);
    if (seq == THR_NEXT_SEQUENCE)
        thr->nextSeq = thr->seq + 1;
    else
        thr->nextSeq = seq;
}

void ThrChangeSeq(Thr *thr, unsigned short seq, int loop)
{
    THR_ASSERT(thr, 411);
    thr->nextLoop = loop;
    ThrSetSeq(thr, seq);
}

void ThrPrintLink(Thr *thr)
{
    if (thr->parent)
        ThrPrintLink(thr->parent);
    EventDebugLog("%c%c%c%c ", THR_NAME_CHARACTERS(thr->id));
}

void ThrDump(Thr *thr)
{
    THR_ASSERT(thr, 434);
    EventDebugLog("seq %4d / level %d / id %c%c%c%c\n", thr->seq, thr->level, THR_NAME_CHARACTERS(thr->id));
    EventDebugLog("TaskLink = ");
    ThrPrintLink(thr);
    EventDebugLog("\n");
}

void ThrLogSeqChange(Thr *thr)
{
    if (thr->prevSeq != thr->seq) {
        EventDebugLog("THR : %4d -> %4d / flag %d\n", thr->prevSeq, thr->seq, thr->loop);
        thr->prevSeq = thr->seq;
    }
}

void ThrSetDebugId(shen_name id)
{
    thrDebugId = id;
}

void ThrChangeSeqSave(Thr *thr, unsigned short seq, unsigned short saved, int loop)
{
    THR_ASSERT(thr, 465);
    ThrChangeSeq(thr, seq, loop);
    thr->savedSeq = saved;
}

void ThrReturnSeq(Thr *thr, int loop)
{
    THR_ASSERT(thr, 473);
    ThrChangeSeq(thr, thr->savedSeq, loop);
}

void ThrResetLoopCount(Thr *thr)
{
    THR_ASSERT(thr, 481);
    thr->stepCount = 0;
}

void ThrBeginStep(Thr *thr)
{
    THR_ASSERT(thr, 487);
    thr->loop = 0;
}

void ThrEndStep(Thr *thr)
{
    THR_ASSERT(thr, 493);
    if (++thr->stepCount > THR_ETERNAL_LOOP_STEPS) {
        EventDebugLog("THR : Eternal Loop !\n");
        ThrDump(thr);
        THR_ASSERT(0, 499);
    }
    if (thrDebugId == thr->id && thr->prevSeq != thr->seq) {
        EventDebugLog("%c%c%c%c %d > %d /%d\n", THR_NAME_CHARACTERS(thrDebugId), thr->prevSeq, thr->seq, thr->loop);
        thr->prevSeq = thr->seq;
    }
}

int ThrIsContinue(Thr *thr)
{
    THR_ASSERT(thr, 515);
    return thr->loop;
}

int ThrGetSeq(Thr *thr)
{
    THR_ASSERT(thr, 524);
    return thr->seq;
}

void *ThrGetWork(Thr *thr)
{
    THR_ASSERT(thr, 533);
    return thr->work;
}

shen_name ThrGetId(Thr *thr)
{
    return thr->id;
}

void ThrSetId(Thr *thr, shen_name id)
{
    thr->id = id;
}

void ThrWaitFrames(Thr *thr, unsigned short next, int loop, int frames)
{
    THR_ASSERT(thr, 555);
    ThrSetNextSeq(thr, next);
    thr->seq = THR_SEQ_WAIT_FRAMES;
    thr->nextLoop = loop;
    thr->waitFrames = frames;
}

int ThrProcWaitFrames(Thr *thr)
{
    int changed = 0;
    THR_ASSERT(thr, 571);
    if (--thr->waitFrames == 0) {
        changed = 1;
        ThrSetSeq(thr, thr->nextSeq);
    }
    return changed;
}

void ThrWaitEventCamera(Thr *thr, unsigned short next, int loop)
{
    THR_ASSERT(thr, 586);
    ThrSetNextSeq(thr, next);
    thr->seq = THR_SEQ_WAIT_EVENT_CAMERA;
    thr->nextLoop = loop;
}

int ThrProcWaitEventCamera(Thr *thr)
{
    int changed = 0;
    THR_ASSERT(thr, 601);
    if (!FTBL.camCtrlCheckIsEventCam()) {
        changed = 1;
        ThrSetSeq(thr, thr->nextSeq);
    }
    return changed;
}

void ThrWaitConversation(Thr *thr, unsigned short next, int loop, int frames)
{
    THR_ASSERT(thr, 618);
    ThrSetNextSeq(thr, next);
    thr->seq = THR_SEQ_WAIT_CONVERSATION;
    thr->nextLoop = loop;
    thr->waitFrames = frames;
}

int ThrProcWaitConversation(Thr *thr)
{
    int changed = 0;
    THR_ASSERT(thr, 634);
    if (thr->waitFrames == 0) {
        if (!FTBL.cvsHasValidConverseData(0)) {
            FTBL.ctrlStopSubtitles();
            changed = 1;
            ThrSetSeq(thr, thr->nextSeq);
        }
    } else {
        thr->waitFrames--;
    }
    return changed;
}

void ThrWaitMotion(Thr *thr, unsigned short next, int loop, shen_name name)
{
    THR_ASSERT(thr, 657);
    ThrSetNextSeq(thr, next);
    thr->seq = THR_SEQ_WAIT_MOTION;
    thr->nextLoop = loop;
    thr->motionName = name;
}

int ThrProcWaitMotion(Thr *thr)
{
    int changed = 0;
    THR_ASSERT(thr, 673);
    THR_ASSERT(thr->motionName, 674);
    if (FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(thr->motionName), 65, 1, 0)) {
        changed = 1;
        ThrSetSeq(thr, thr->nextSeq);
    }
    return changed;
}

void ThrWaitSpeakChoice(Thr *thr, unsigned short next, int loop)
{
    THR_ASSERT(thr, 690);
    ThrSetNextSeq(thr, next);
    thr->seq = THR_SEQ_WAIT_SPEAK_CHOICE;
    thr->nextLoop = loop;
}

int ThrProcWaitSpeakChoice(Thr *thr)
{
    int changed = 0;
    int icons;
    THR_ASSERT(thr, 705);
    icons = FTBL.GetShouldRenderIcons(2);
    if (icons == 0) {
        thr->speakSelection = FTBL.shenGetCtrlSpeakIconFlag();
        if (thr->speakSelection == 0xffff)
            thr->speakSelection = icons;
        changed = 1;
        ThrSetSeq(thr, thr->nextSeq);
    }
    return changed;
}

int ThrGetSpeakChoice(Thr *thr)
{
    THR_ASSERT(thr, 724);
    return thr->speakSelection;
}

void ThrSetSpeakChoice(Thr *thr, int choice)
{
    THR_ASSERT(thr, 733);
    thr->speakSelection = choice;
}

void ThrWaitTurn(Thr *thr, unsigned short next, int loop, shen_name name, const Vector3 *target, const unsigned short *motions)
{
    THR_ASSERT(thr, 742);
    ThrSetNextSeq(thr, next);
    thr->seq = THR_SEQ_WAIT_TURN;
    thr->nextLoop = loop;
    thr->turnName = name;
    thr->turnTarget = target;
    thr->turnMotions = motions;
    thr->turnYaw = 0;
    thr->turnStep = 0.0f;
    thr->turnFlags = 0;
    FTBL.TurnScenarioCharacterTowardPosition(thr->turnName, thr->turnTarget, thr->turnMotions, &thr->turnYaw, &thr->turnStep, 0);
}

int ThrProcWaitTurn(Thr *thr)
{
    int changed = 0;
    THR_ASSERT(thr, 779);
    if (FTBL.scenMotionworkUtil(thr->turnName, thr->turnYaw, thr->turnStep, thr->turnFlags, thr->turnMotions)) {
        changed = 1;
        ThrSetSeq(thr, thr->nextSeq);
    }
    return changed;
}

void ThrWaitChildExit(Thr *thr, unsigned short next, int loop, int index)
{
    THR_ASSERT(thr, 801);
    if (index >= THRBUF_MAX) {
        EventDebugLog("THR : idx is over THRBUF_MAX ! : %d\n", index);
        ThrDump(thr);
        THR_ASSERT(0, 802);
    }
    ThrSetNextSeq(thr, next);
    thr->seq = THR_SEQ_WAIT_CHILD_EXIT;
    thr->exitIndex = index;
    thr->nextLoop = loop;
}

int ThrProcWaitChildExit(Thr *thr)
{
    int changed = 0;
    THR_ASSERT(thr, 818);
    if (thr->children[thr->exitIndex] == 0) {
        changed = 1;
        ThrSetSeq(thr, thr->nextSeq);
    }
    return changed;
}

void ThrWaitFade(Thr *thr, unsigned short next, int loop)
{
    THR_ASSERT(thr, 833);
    ThrSetNextSeq(thr, next);
    thr->seq = THR_SEQ_WAIT_FADE;
    thr->nextLoop = loop;
    thr->waitFrames = 2;
}

int ThrProcWaitFade(Thr *thr)
{
    int changed = 0;
    THR_ASSERT(thr, 849);
    if (thr->waitFrames) {
        thr->waitFrames--;
    } else if (!FTBL.filmCheckFadeTrackColour()) {
        changed = 1;
        ThrSetSeq(thr, thr->nextSeq);
    }
    return changed;
}

void ThrWaitFight(Thr *thr, unsigned short next, int loop)
{
    THR_ASSERT(thr, 869);
    ThrSetNextSeq(thr, next);
    thr->seq = THR_SEQ_WAIT_FIGHT;
    thr->nextLoop = loop;
}

int ThrProcWaitFight(Thr *thr)
{
    int changed = 0;
    THR_ASSERT(thr, 884);
    if (!FTBL.IsFightActive()) {
        changed = 1;
        ThrSetSeq(thr, thr->nextSeq);
    }
    return changed;
}

void ThrWaitAuthor(Thr *thr, unsigned short next, int loop)
{
    THR_ASSERT(thr, 902);
    ThrSetNextSeq(thr, next);
    thr->seq = THR_SEQ_WAIT_AUTHOR;
    thr->waitFrames = 5;
    thr->nextLoop = loop;
}

int ThrProcWaitAuthor(Thr *thr)
{
    int changed = 0;
    THR_ASSERT(thr, 918);
    if (thr->waitFrames) {
        thr->waitFrames--;
    } else if (!FTBL.EV_authSceneCtrl(-1)) {
        changed = 1;
        ThrSetSeq(thr, thr->nextSeq);
    }
    return changed;
}

void ThrWaitChildResult(Thr *thr, unsigned short next, int loop, int index)
{
    THR_ASSERT(thr, 939);
    if (index >= THRBUF_MAX) {
        EventDebugLog("THR : idx is over THRBUF_MAX ! : %d\n", index);
        ThrDump(thr);
        THR_ASSERT(0, 940);
    }
    if (thr->lastChild != index) {
        EventDebugLog("THR : Warning, Wait Child Que, but it's child is not last making child !\n");
        ThrDump(thr);
    }
    ThrSetNextSeq(thr, next);
    thr->seq = THR_SEQ_WAIT_CHILD_RESULT;
    thr->resultIndex = index;
    thr->nextLoop = loop;
}

int ThrProcWaitChildResult(Thr *thr)
{
    int changed = 0;
    THR_ASSERT(thr, 962);
    if (thr->childDone[thr->resultIndex]) {
        changed = 1;
        ThrSetSeq(thr, thr->nextSeq);
    }
    return changed;
}

void ThrWaitChildSequence(Thr *thr, int loop, int index)
{
    THR_ASSERT(thr, 977);
    if (index >= THRBUF_MAX) {
        EventDebugLog("THR : idx is over THRBUF_MAX ! : %d\n", index);
        ThrDump(thr);
        THR_ASSERT(0, 978);
    }
    if (thr->lastChild != index) {
        EventDebugLog("THR : Warning, Wait Child Que, but it's child is not last making child !\n");
        ThrDump(thr);
    }
    thr->seq = THR_SEQ_WAIT_CHILD_SEQUENCE;
    thr->resultIndex = index;
    thr->nextLoop = loop;
}

int ThrProcWaitChildSequence(Thr *thr)
{
    int changed = 0;
    THR_ASSERT(thr, 999);
    if (thr->childDone[thr->resultIndex]) {
        changed = 1;
        ThrSetSeq(thr, thr->childValue[thr->resultIndex]);
    }
    return changed;
}

void ThrSetChildDone(Thr *thr, int index, int done)
{
    THR_ASSERT(thr, 1014);
    if (index >= THRBUF_MAX) {
        EventDebugLog("THR : idx is over THRBUF_MAX ! : %d\n", index);
        ThrDump(thr);
        THR_ASSERT(0, 1015);
    }
    thr->childDone[index] = done;
}

void ThrSignalParent(Thr *thr, int done)
{
    THR_ASSERT(thr, 1025);
    if (thr->hasReturn) {
        EventDebugLog("THR : Warning, Already set return value, but not return value !\n");
        ThrDump(thr);
    }
    if (thr->parent)
        thr->parent->childDone[thr->index] = done;
}

void ThrReturnToParent(Thr *thr, int done)
{
    THR_ASSERT(thr, 1043);
    if (thr->parent) {
        if (!thr->hasReturn) {
            EventDebugLog("THR : return value, but not set return value eat !\n");
            ThrDump(thr);
            THR_ASSERT(0, 1051);
        }
        if (thr->parent)
            thr->parent->childDone[thr->index] = done;
        thr->parent->childValue[thr->index] = thr->returnValue;
    }
}

void ThrSetChildArgument(Thr *thr, int index, int argument)
{
    THR_ASSERT(thr, 1066);
    if (index >= THRBUF_MAX) {
        EventDebugLog("THR : idx is over THRBUF_MAX ! : %d\n", index);
        ThrDump(thr);
        THR_ASSERT(0, 1067);
    }
    if (thr->children[index])
        thr->children[index]->argument = argument;
}

void ThrSetReturnValue(Thr *thr, int value)
{
    THR_ASSERT(thr, 1079);
    thr->returnValue = value;
    thr->hasReturn = 1;
}

int ThrGetReturnValue(Thr *thr)
{
    THR_ASSERT(thr, 1090);
    if (!thr->hasReturn) {
        EventDebugLog("THR : get return value to parent, but not set return value eat !\n");
        ThrDump(thr);
        THR_ASSERT(0, 1096);
    }
    return thr->returnValue;
}

int ThrGetChildValue(Thr *thr, int index)
{
    THR_ASSERT(thr, 1108);
    if (index >= THRBUF_MAX) {
        EventDebugLog("THR : idx is over THRBUF_MAX ! : %d\n", index);
        ThrDump(thr);
        THR_ASSERT(0, 1109);
    }
    return thr->childValue[index];
}

#define THR_USER_INDEX(line) \
    if (index >= THR_USRBUF_MAX) { \
        EventDebugLog("THR : usridx is over THR_USRBUF_MAX ! : %d\n", index); \
        ThrDump(thr); \
        THR_ASSERT(0, line); \
    }

int ThrGetUser(Thr *thr, int index)
{
    THR_ASSERT(thr, 1119);
    THR_USER_INDEX(1120);
    return thr->user[index].i;
}

void ThrSetUser(Thr *thr, int index, int value)
{
    THR_ASSERT(thr, 1126);
    THR_USER_INDEX(1127);
    thr->user[index].i = value;
}

void ThrSetUserUnsigned(Thr *thr, int index, unsigned int value)
{
    THR_ASSERT(thr, 1133);
    THR_USER_INDEX(1134);
    thr->user[index].i = value;
}

void ThrSetUserFloat(Thr *thr, int index, float value)
{
    THR_ASSERT(thr, 1140);
    THR_USER_INDEX(1141);
    thr->user[index].f = value;
}

void ThrSetUserPointer(Thr *thr, int index, void *value)
{
    THR_ASSERT(thr, 1147);
    THR_USER_INDEX(1148);
    thr->user[index].i = (int)value;
}

void ThrSetUserName(Thr *thr, int index, shen_name value)
{
    THR_ASSERT(thr, 1154);
    THR_USER_INDEX(1155);
    thr->user[index].i = value;
}

void ThrSetUserLong(Thr *thr, int index, long value)
{
    THR_ASSERT(thr, 1161);
    THR_USER_INDEX(1162);
    thr->user[index].i = value;
}

void ThrSetUserUnsignedLong(Thr *thr, int index, unsigned long value)
{
    THR_ASSERT(thr, 1168);
    THR_USER_INDEX(1169);
    thr->user[index].i = value;
}

void ThrSetUserConstPointer(Thr *thr, int index, const void *value)
{
    THR_ASSERT(thr, 1175);
    THR_USER_INDEX(1176);
    thr->user[index].i = (int)value;
}

int *ThrGetBuffer0Address(Thr *thr, int index)
{
    THR_ASSERT(thr, 1182);
    THR_USER_INDEX(1183);
    return &thr->buffer0[index];
}

int ThrGetBuffer0(Thr *thr, int index)
{
    THR_ASSERT(thr, 1189);
    THR_USER_INDEX(1190);
    return thr->buffer0[index];
}

void ThrSetBuffer0(Thr *thr, int index, int value)
{
    THR_ASSERT(thr, 1196);
    THR_USER_INDEX(1197);
    thr->buffer0[index] = value;
}

int *ThrGetBuffer1Address(Thr *thr, int index)
{
    THR_ASSERT(thr, 1203);
    THR_USER_INDEX(1204);
    return &thr->buffer1[index];
}

int ThrGetBuffer1(Thr *thr, int index)
{
    THR_ASSERT(thr, 1210);
    THR_USER_INDEX(1211);
    return thr->buffer1[index];
}

void ThrSetBuffer1(Thr *thr, int index, int value)
{
    THR_ASSERT(thr, 1217);
    THR_USER_INDEX(1218);
    thr->buffer1[index] = value;
}

int *ThrGetBuffer2Address(Thr *thr, int index)
{
    THR_ASSERT(thr, 1224);
    THR_USER_INDEX(1225);
    return &thr->buffer2[index];
}

int ThrGetBuffer2(Thr *thr, int index)
{
    THR_ASSERT(thr, 1231);
    THR_USER_INDEX(1232);
    return thr->buffer2[index];
}

void ThrSetBuffer2(Thr *thr, int index, int value)
{
    THR_ASSERT(thr, 1238);
    THR_USER_INDEX(1239);
    thr->buffer2[index] = value;
}

int ThrGetBuffer3(Thr *thr, int index)
{
    THR_ASSERT(thr, 1245);
    THR_USER_INDEX(1246);
    return thr->buffer3[index];
}

void ThrSetBuffer3(Thr *thr, int index, int value)
{
    THR_ASSERT(thr, 1252);
    THR_USER_INDEX(1253);
    thr->buffer3[index] = value;
}

int ThrProcess(Thr *thr)
{
    int changed = 0;
    THR_ASSERT(thr, 1264);
    switch (thr->seq) {
    case THR_SEQ_KILL_OWN:
        changed = ThrKillOwn(thr);
        break;
    case THR_SEQ_WAIT_FRAMES:
        changed = ThrProcWaitFrames(thr);
        break;
    case THR_SEQ_WAIT_EVENT_CAMERA:
        changed = ThrProcWaitEventCamera(thr);
        break;
    case THR_SEQ_WAIT_CONVERSATION:
        changed = ThrProcWaitConversation(thr);
        break;
    case THR_SEQ_WAIT_MOTION:
        changed = ThrProcWaitMotion(thr);
        break;
    case THR_SEQ_WAIT_SPEAK_CHOICE:
        changed = ThrProcWaitSpeakChoice(thr);
        break;
    case THR_SEQ_WAIT_TURN:
        changed = ThrProcWaitTurn(thr);
        break;
    case THR_SEQ_WAIT_CHILD_RESULT:
        changed = ThrProcWaitChildResult(thr);
        break;
    case THR_SEQ_WAIT_CHILD_SEQUENCE:
        changed = ThrProcWaitChildSequence(thr);
        break;
    case THR_SEQ_WAIT_CHILD_EXIT:
        changed = ThrProcWaitChildExit(thr);
        break;
    case THR_SEQ_WAIT_FADE:
        changed = ThrProcWaitFade(thr);
        break;
    case THR_SEQ_WAIT_FIGHT:
        changed = ThrProcWaitFight(thr);
        break;
    case THR_SEQ_WAIT_AUTHOR:
        changed = ThrProcWaitAuthor(thr);
        break;
    default:
        EventDebugLog("THR : Illigal Seq Number\n");
        ThrDump(thr);
        THR_ASSERT(0, 1284);
        break;
    }
    return changed;
}
#endif
