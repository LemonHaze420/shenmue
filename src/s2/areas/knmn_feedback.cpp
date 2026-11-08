#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_knmn.hpp"
#include "s2/training_seqp.hpp"

void KnmnStartPracticeSequence(void *data)
{
 Thr *thr=(Thr*)data;
 KnmnPracticeWork *work=(KnmnPracticeWork*)ThrGetWork(thr);
 KnmnPracticeControl *control=&work->control;
 ThrResetLoopCount(thr);
 do{
  ThrBeginStep(thr);
  switch(ThrGetSeq(thr)){
  case 0:ThrChangeSeq(thr,100,1);break;
  case 100:
   control->firstFailures[0]=0;control->firstFailures[1]=0;control->firstFailures[2]=0;
   work->greeting=0;work->learnedMove=0;control->restCount=0;control->elapsed=0;
   control->conversationActive=0;control->retained5a0=0;control->demonstrationCamera=0;
   TrainingClearFailures(work);
   FTBL.SetScreenFilmFade(30,255,0,0,0,0,0,0,0);
   ThrMakeChild(thr,KnmnInitializeDemonstrationSequence,SHEN_NAME4('I','W','D','E'),work,0);
   ThrSetChildDone(thr,0,0);ThrWaitChildResult(thr,105,1,0);break;
  case 105:ThrSignalParent(thr,1);ThrKillOwn(thr);break;
  default:ThrProcess(thr);break;
  }
  ThrEndStep(thr);
 }while(ThrIsContinue(thr));
}

void KnmnFinishPracticeSequence(void *data)
{
 Thr *thr=(Thr*)data;
 ThrResetLoopCount(thr);
 do{
  ThrBeginStep(thr);
  switch(ThrGetSeq(thr)){
  case 0:ThrChangeSeq(thr,100,1);break;
  case 100:
   FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(KNMN_RYO),0,0);
   FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(KNMN_KEM),0,0);
   ThrSignalParent(thr,1);ThrKillOwn(thr);break;
  default:ThrProcess(thr);break;
  }
  ThrEndStep(thr);
 }while(ThrIsContinue(thr));
}

void KnmnAwardMoveSequence(void *data)
{
 Thr *thr=(Thr*)data;
 KnmnPracticeWork *work=(KnmnPracticeWork*)ThrGetWork(thr);
 ThrResetLoopCount(thr);
 do{
  ThrBeginStep(thr);
  switch(ThrGetSeq(thr)){
  case 0:ThrChangeSeq(thr,100,1);break;
  case 100:ThrWaitFade(thr,101,1);break;
  case 101:
   if(work->learnedMove==-1)ThrChangeSeq(thr,103,1);
   else{
    FTBL.SetMoveLearningEntry(0,1,21,12);FTBL.SetNewEwzoTypeForMoveLearned(0,1,21);
    work->resultFrames=150;ThrChangeSeq(thr,102,1);
   }
   break;
  case 102:
   work->resultFrames--;
   if(FTBL.GetCurrentControlInput(1)==512||!work->resultFrames){FTBL.SetNewEwzoTypeForMoveLearned(1,0,0);ThrChangeSeq(thr,103,1);}break;
  case 103:ThrSignalParent(thr,1);ThrKillOwn(thr);break;
  default:ThrProcess(thr);break;
  }
  ThrEndStep(thr);
 }while(ThrIsContinue(thr));
}

void KnmnFollowTeacher(void)
{
 Vector3 target;
 FTBL.GetCharaPosition(KNMN_RYO,5,&target,0x40000000);
 FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(KNMN_KEM),&target,0);
}

void KnmnFeedbackOpening(void *data)
{
 KnmnPracticeWork *work=(KnmnPracticeWork*)ThrGetWork((Thr*)data);
 switch(work->greeting){
 case 0:
  FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(KNMN_KEM),0,0);
  FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(KNMN_KEM),5195,0);break;
 case 1:
  FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(KNMN_KEM),0,0);
  FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(KNMN_KEM),5196,0);break;
 case 2:
  FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(KNMN_KEM),0,0);
  FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(KNMN_KEM),5197,0);break;
 case 3:
  if(FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(KNMN_KEM),65,1,0)){
   KnmnFollowTeacher();
   FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(KNMN_KEM),5193,0);
  }
  break;
 }
 work->greeting++;
 if(work->greeting>3)work->greeting=0;
}

void KnmnFeedbackClosing(void *data)
{
 if(FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(KNMN_KEM),65,1,0)){
  KnmnFollowTeacher();
  FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(KNMN_KEM),5193,0);
 }
}

void KnmnQteFailureSequence(void *data)
{
 Thr *thr=(Thr*)data;
 int mode=(unsigned short)ThrGetUser(thr,0);const char *dialogue;
 ThrResetLoopCount(thr);
 do{
  ThrBeginStep(thr);
  switch(ThrGetSeq(thr)){
  case 0:
   KnmnFeedbackOpening(thr);
   ThrWaitFrames(thr,100,1,30);
   break;
  case 100:
   switch(mode){case 0:dialogue="0026B056";break;case 1:dialogue="0026B056";break;default:dialogue="0026B056";break;}
   FTBL.EV_PlayDialogue("0026B056");(void)dialogue;
   ThrWaitConversation(thr,0xffff,1,60);
   break;
  case 101:
   KnmnFeedbackClosing(thr);
   ThrSignalParent(thr,1);
   knmnQteDialogue=0;
   ThrKillOwn(thr);
   break;
  default:ThrProcess(thr);break;
  }
  ThrEndStep(thr);
 }while(ThrIsContinue(thr));
}

void KnmnIdleAdviceSequence(void *data)
{
 Thr *thr=(Thr*)data;
 ThrResetLoopCount(thr);
 do{
  ThrBeginStep(thr);
  switch(ThrGetSeq(thr)){
  case 0:
   ThrChangeSeq(thr,100,1);
   break;
  case 100:
   FTBL.EV_PlayDialogue("0026B039");
   ThrWaitConversation(thr,0xffff,1,60);
   break;
  case 101:
   ThrSignalParent(thr,1);
   ThrKillOwn(thr);
   break;
  default:ThrProcess(thr);break;
  }
  ThrEndStep(thr);
 }while(ThrIsContinue(thr));
}

void KnmnStayAndStrikeSequence(void *data)
{
 Thr *thr=(Thr*)data;
 ThrResetLoopCount(thr);
 do{
  ThrBeginStep(thr);
  switch(ThrGetSeq(thr)){
  case 0:
   KnmnFeedbackOpening(thr);
   ThrChangeSeq(thr,100,1);
   break;
  case 100:
   FTBL.EV_PlayDialogue("0026B038");
   ThrWaitConversation(thr,0xffff,1,60);
   break;
  case 101:
   KnmnFeedbackClosing(thr);
   ThrSignalParent(thr,1);
   ThrKillOwn(thr);
   break;
  default:ThrProcess(thr);break;
  }
  ThrEndStep(thr);
 }while(ThrIsContinue(thr));
}

void KnmnRelaxShouldersSequence(void *data)
{
 Thr *thr=(Thr*)data;
 ThrResetLoopCount(thr);
 do{
  ThrBeginStep(thr);
  switch(ThrGetSeq(thr)){
  case 0:
   KnmnFeedbackOpening(thr);
   ThrChangeSeq(thr,100,1);
   break;
  case 100:
   FTBL.EV_PlayDialogue("0026B056");
   ThrWaitConversation(thr,0xffff,1,60);
   break;
  case 101:
   KnmnFeedbackClosing(thr);
   ThrSignalParent(thr,1);
   ThrKillOwn(thr);
   break;
  default:ThrProcess(thr);break;
  }
  ThrEndStep(thr);
 }while(ThrIsContinue(thr));
}

void KnmnRelaxShouldersLegsSequence(void *data)
{
 Thr *thr=(Thr*)data;
 ThrResetLoopCount(thr);
 do{
  ThrBeginStep(thr);
  switch(ThrGetSeq(thr)){
  case 0:
   KnmnFeedbackOpening(thr);
   ThrChangeSeq(thr,100,1);
   break;
  case 100:
   FTBL.EV_PlayDialogue("0026B056");
   ThrWaitConversation(thr,0xffff,1,60);
   break;
  case 101:
   KnmnFeedbackClosing(thr);
   ThrSignalParent(thr,1);
   ThrKillOwn(thr);
   break;
  default:ThrProcess(thr);break;
  }
  ThrEndStep(thr);
 }while(ThrIsContinue(thr));
}

void KnmnRelaxShouldersArmsSequence(void *data)
{
 Thr *thr=(Thr*)data;
 ThrResetLoopCount(thr);
 do{
  ThrBeginStep(thr);
  switch(ThrGetSeq(thr)){
  case 0:
   KnmnFeedbackOpening(thr);
   ThrChangeSeq(thr,100,1);
   break;
  case 100:
   FTBL.EV_PlayDialogue("0026B056");
   ThrWaitConversation(thr,0xffff,1,60);
   break;
  case 101:
   KnmnFeedbackClosing(thr);
   ThrSignalParent(thr,1);
   ThrKillOwn(thr);
   break;
  default:ThrProcess(thr);break;
  }
  ThrEndStep(thr);
 }while(ThrIsContinue(thr));
}

void KnmnRelaxWithoutGestureSequence(void *data)
{
 Thr *thr=(Thr*)data;
 ThrResetLoopCount(thr);
 do{
  ThrBeginStep(thr);
  switch(ThrGetSeq(thr)){
  case 0:
   ThrChangeSeq(thr,100,1);
   break;
  case 100:
   FTBL.EV_PlayDialogue("0026B056");
   ThrWaitConversation(thr,0xffff,1,60);
   break;
  case 101:
   ThrSignalParent(thr,1);
   ThrKillOwn(thr);
   break;
  default:ThrProcess(thr);break;
  }
  ThrEndStep(thr);
 }while(ThrIsContinue(thr));
}

void KnmnUseHandsSequence(void *data)
{
 Thr *thr=(Thr*)data;
 ThrResetLoopCount(thr);
 do{
  ThrBeginStep(thr);
  switch(ThrGetSeq(thr)){
  case 0:
   KnmnFeedbackOpening(thr);
   ThrChangeSeq(thr,100,1);
   break;
  case 100:
   FTBL.EV_PlayDialogue("0026B037");
   ThrWaitConversation(thr,0xffff,1,60);
   break;
  case 101:
   KnmnFeedbackClosing(thr);
   ThrSignalParent(thr,1);
   ThrKillOwn(thr);
   break;
  default:ThrProcess(thr);break;
  }
  ThrEndStep(thr);
 }while(ThrIsContinue(thr));
}

void KnmnUseLegsSequence(void *data)
{
 Thr *thr=(Thr*)data;
 ThrResetLoopCount(thr);
 do{
  ThrBeginStep(thr);
  switch(ThrGetSeq(thr)){
  case 0:
   KnmnFeedbackOpening(thr);
   ThrChangeSeq(thr,100,1);
   break;
  case 100:
   FTBL.EV_PlayDialogue("0026B036");
   ThrWaitConversation(thr,0xffff,1,60);
   break;
  case 101:
   KnmnFeedbackClosing(thr);
   ThrSignalParent(thr,1);
   ThrKillOwn(thr);
   break;
  default:ThrProcess(thr);break;
  }
  ThrEndStep(thr);
 }while(ThrIsContinue(thr));
}

void KnmnHandsSlowSequence(void *data)
{
 Thr *thr=(Thr*)data;
 ThrResetLoopCount(thr);
 do{
  ThrBeginStep(thr);
  switch(ThrGetSeq(thr)){
  case 0:
   KnmnFeedbackOpening(thr);
   ThrChangeSeq(thr,100,1);
   break;
  case 100:
   FTBL.EV_PlayDialogue("0026B040");
   ThrWaitConversation(thr,0xffff,1,60);
   break;
  case 101:
   KnmnFeedbackClosing(thr);
   ThrSignalParent(thr,1);
   ThrKillOwn(thr);
   break;
  default:ThrProcess(thr);break;
  }
  ThrEndStep(thr);
 }while(ThrIsContinue(thr));
}

void KnmnHandsLegsTogetherSequence(void *data)
{
 Thr *thr=(Thr*)data;
 ThrResetLoopCount(thr);
 do{
  ThrBeginStep(thr);
  switch(ThrGetSeq(thr)){
  case 0:
   KnmnFeedbackOpening(thr);
   ThrChangeSeq(thr,100,1);
   break;
  case 100:
   FTBL.EV_PlayDialogue("0026B041");
   ThrWaitConversation(thr,0xffff,1,60);
   break;
  case 101:
   KnmnFeedbackClosing(thr);
   ThrSignalParent(thr,1);
   ThrKillOwn(thr);
   break;
  default:ThrProcess(thr);break;
  }
  ThrEndStep(thr);
 }while(ThrIsContinue(thr));
}

void KnmnUnclassifiedFeedbackSequence(void *data)
{
 KnmnRelaxWithoutGestureSequence(data);
}
#else
#include "s2/areas/knmn_practice.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/motion_work.hpp"
namespace shenmue::s2::areas {
void KnmnFeedbackOpening(CompiledSequenceWork &sequence) {
    KnmnPracticeWork &work = *static_cast<KnmnPracticeWork *>(sequence.event);
    const std::uint8_t gesture = KnmnValue<std::uint8_t>(&work, 0x65c);
    if (gesture <= 2) {
        mtwkUpdateWithPosition(scenFindMotionwork(KNMN_KEM), nullptr, 0);
        mtwkRequestMotion(scenFindMotionwork(KNMN_KEM), static_cast<std::uint16_t>(5195 + gesture), MotionRegion::LowerBody);
    } else if (gesture == 3 && mtwkSetRegionFlag(scenFindMotionwork(KNMN_KEM), 65, 1, MotionRegion::LowerBody)) {
        Vector3 target;
        EV_GetCharaPosition(KNMN_RYO, 5, &target, EventTransformFlags::World);
        mtwkUpdateWithPosition(scenFindMotionwork(KNMN_KEM), &target, 0);
        mtwkRequestMotion(scenFindMotionwork(KNMN_KEM), 5193, MotionRegion::LowerBody);
    }
    const std::uint8_t next = static_cast<std::uint8_t>(gesture + 1);
    KnmnValue(&work, 0x65c, std::uint8_t(next > 3 ? 0 : next));
}
void KnmnFeedbackClosing() {
    if (mtwkSetRegionFlag(scenFindMotionwork(KNMN_KEM), 65, 1, MotionRegion::LowerBody)) {
        Vector3 target;
        EV_GetCharaPosition(KNMN_RYO, 5, &target, EventTransformFlags::World);
        mtwkUpdateWithPosition(scenFindMotionwork(KNMN_KEM), &target, 0);
        mtwkRequestMotion(scenFindMotionwork(KNMN_KEM), 5193, MotionRegion::LowerBody);
    }
}
namespace {
void KnmnFeedbackLine(void *data, const char *dialogue, bool gesture, std::int32_t initialWait = 0) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            if (gesture)
                KnmnFeedbackOpening(sequence);
            if (initialWait)
                KnmnWait(sequence, 2, 100, initialWait);
            else
                KnmnAdvance(sequence, 100);
            break;
        case 100:
            EV_PlayDialogue(dialogue);
            KnmnWait(sequence, 4, 101, 60);
            break;
        case 101:
            if (gesture)
                KnmnFeedbackClosing();
            KnmnComplete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
} // namespace
void KnmnIdleAdviceSequence(void *data) {
    KnmnFeedbackLine(data, "0026B039", false);
}
void KnmnStayAndStrikeSequence(void *data) {
    KnmnFeedbackLine(data, "0026B038", true);
}
void KnmnRelaxShouldersSequence(void *data) {
    KnmnFeedbackLine(data, "0026B056", true);
}
void KnmnUseHandsSequence(void *data) {
    KnmnFeedbackLine(data, "0026B037", true);
}
void KnmnUseLegsSequence(void *data) {
    KnmnFeedbackLine(data, "0026B036", true);
}
void KnmnHandsSlowSequence(void *data) {
    KnmnFeedbackLine(data, "0026B040", true);
}
void KnmnHandsLegsTogetherSequence(void *data) {
    KnmnFeedbackLine(data, "0026B041", true);
}
void KnmnRelaxWithoutGestureSequence(void *data) {
    KnmnFeedbackLine(data, "0026B056", false);
}
void KnmnUnclassifiedFeedbackSequence(void *data) {
    KnmnRelaxWithoutGestureSequence(data);
}
void KnmnQteFailureSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            KnmnFeedbackOpening(sequence);
            KnmnWait(sequence, 2, 100, 30);
            break;
        case 100:
            EV_PlayDialogue("0026B056");
            KnmnWait(sequence, 4, 101, 60);
            break;
        case 101:
            KnmnFeedbackClosing();
            knmnQteDialogue = nullptr;
            KnmnComplete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void KnmnSelectFeedback(CompiledSequenceWork &sequence, HLib::TaskCallback *callback) {
    KnmnPracticeWork &work = *static_cast<KnmnPracticeWork *>(sequence.event);
    const KnmnMoveResult &result = work.results[work.resultIndex + 1];
    if (work.result == 20)
        *callback = KnmnIdleAdviceSequence;
    else if (result.steps != 1)
        *callback = KnmnUnclassifiedFeedbackSequence;
    else {
        switch (result.error) {
        case 2:
        case 10:
        case 11:
            *callback = KnmnRelaxShouldersSequence;
            break;
        case 3:
            *callback = KnmnUseHandsSequence;
            break;
        case 4:
            *callback = KnmnUseLegsSequence;
            break;
        case 12:
            *callback = KnmnStayAndStrikeSequence;
            break;
        case 15:
            *callback = (work.results[0].detail & 1) ? KnmnHandsSlowSequence : KnmnHandsLegsTogetherSequence;
            break;
        default:
            *callback = KnmnRelaxWithoutGestureSequence;
            break;
        }
    }
}
void KnmnFeedbackSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    KnmnPracticeWork &work = *static_cast<KnmnPracticeWork *>(sequence.event);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            KnmnAdvance(sequence, 100);
            break;
        case 100: {
            HLib::TaskCallback callback;
            KnmnSelectFeedback(sequence, &callback);
            KnmnChild(sequence, callback, SHEN_NAME(FS1S), 1);
            sequence.resultSlot = 1;
            KnmnWait(sequence, 8, 101);
            break;
        }
        case 101:
            work.dialogueSequence = nullptr;
            KnmnComplete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void KnmnExitPracticeSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    KnmnPracticeWork &work = *static_cast<KnmnPracticeWork *>(sequence.event);
    const std::int64_t retry = KnmnValue<std::int64_t>(data, 0x140), finish = KnmnValue<std::int64_t>(data, 0x150);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            KnmnAdvance(sequence, 100);
            break;
        case 100:
            mtwkRequestMotion(scenFindMotionwork(KNMN_KEM), 5193, MotionRegion::LowerBody);
            if (work.practiceExpired)
                KnmnAdvance(sequence, 101);
            else if (work.practiceWarning)
                KnmnAdvance(sequence, 105);
            else {
                KnmnValue(data, 0x88, static_cast<std::int32_t>(retry));
                sequence.reserved1a0 = 1;
                KnmnAdvance(sequence, 125);
            }
            break;
        case 101:
            work.conversationActive = 1;
            KnmnWait(sequence, 13, 103, 5);
            sequence.continueAfterWait = 0;
            break;
        case 103:
            KnmnWait(sequence, 13, 104, 5);
            sequence.continueAfterWait = 0;
            break;
        case 104:
            KnmnValue(data, 0x88, static_cast<std::int32_t>(finish));
            sequence.reserved1a0 = 1;
            work.practiceExpired = 0;
            KnmnAdvance(sequence, 125);
            break;
        case 125:
            KnmnComplete(sequence, true);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void KnmnRepeatFeedbackSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            KnmnAdvance(sequence, 100);
            break;
        case 100:
            EV_AT_SetPos(KNMN_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, KNMN_PLAYER_PRACTICE_POSITION);
            EV_AT_SetAngY(KNMN_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{.x = 0, .y = 16384, .z = 0});
            EV_AT_SetPos(KNMN_KEM, EventTransformFlags::World | EventTransformFlags::AllAxes, KNMN_TEACHER_PRACTICE_POSITION);
            EV_AT_SetAngY(KNMN_KEM, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{.x = 0, .y = 60984, .z = 0});
            EV_DisableCharacterDisplay(KNMN_KEM);
            KnmnAdvance(sequence, 101);
            break;
        case 101:
            figpSetFlagsBlendForBlur(KNMN_RYO, 8, 2);
            figpAssignModeFor(KNMN_RYO, 4);
            mtwkSetRegionFlag(scenFindMotionwork(KNMN_RYO), 71, 0, MotionRegion::LowerBody);
            mtwkSetEnabled(scenFindMotionwork(KNMN_RYO), 1);
            mtwkRequestMotion(scenFindMotionwork(KNMN_RYO), 1, MotionRegion::LowerBody);
            mtwkSetLayerFrameRange(scenFindMotionwork(KNMN_RYO), -1, -1, -1, MotionRegion::LowerBody);
            mtwkSetSpeed(scenFindMotionwork(KNMN_RYO), 1, MotionRegion::LowerBody);
            mtwkSetRegionFlag(scenFindMotionwork(KNMN_RYO), 69, 0, MotionRegion::LowerBody);
            sequence.waitCharacter = KNMN_RYO;
            KnmnWait(sequence, 5, 102);
            break;
        case 102:
            mtwkSetSpeed(scenFindMotionwork(KNMN_RYO), 1, MotionRegion::LowerBody);
            figpAssignModeFor(KNMN_RYO, 0);
            KnmnComplete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
} // namespace shenmue::s2::areas
#endif
