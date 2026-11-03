#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_knmn.hpp"
#include "s2/training_seqp.hpp"

Vector3 knmnPlayerStartPosition = {40.7f, 102.25f, 241.4f};
Vector3 knmnTeacherStartPosition = {40.2f, 102.25f, 242.7f};
Vector3 knmnDummyStartPosition = {37.9f, 102.25f, 241.0f};
int knmnPlayerStartYaw = 0x4000, knmnTeacherStartYaw = 0xee38, knmnDummyStartYaw = 0xc000;
Vector3 knmnPlayerPracticePosition = {41.08f, 102.25f, 241.04001f};
Vector3 knmnTeacherPracticePosition = {40.2f, 102.25f, 242.7f};
Vector3 knmnDummyPracticePosition = {37.9f, 102.25f, 241.0f};
int knmnPlayerPracticeYaw = 0x4000, knmnTeacherPracticeYaw = 0xee38, knmnDummyPracticeYaw = 0xc000;
unsigned char knmnPracticeLcd[194] = {6, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 14, 3, 192, 31, 128,
    4, 146, 2, 64, 16, 128, 4, 146, 2, 64, 32, 64, 2, 100, 2, 64, 38, 64, 2, 100, 30, 120, 73, 32, 1, 8, 16, 8, 73, 32, 1, 8, 16, 8, 79, 32, 1, 8, 16, 8, 64,
    32, 2, 100, 30, 120, 64, 32, 2, 100, 2, 64, 79, 32, 4, 146, 2, 64, 73, 32, 4, 146, 2, 64, 73, 32, 7, 14, 3, 192, 121, 224};
Thr *knmnQteDialogue;

static int knmnConditionalDialogues[3];
static int knmnEndRequested, knmnEndTimer;
void KnmnInstallBattleDialogues(int line) {
    int id;
    knmnEndRequested = 0;
    id = FTBL.AllocateVCDS_6(100, -2, 0, 0, 100, 150, 0);
    FTBL.AssignNextTaskInQueueByDepth(id, KnmnInstallDialogueConditions);
}
void KnmnSetBattleDialogueFlags(void) {
    FTBL.SetFlagForTaskData();
}
int KnmnInstallDialogueConditions(void *data) {
    int id;
    id = FTBL.AllocateVCDS_2(8, -1, KNMN_KEM, 0, 1, 100, 1);
    FTBL.AssignNextTaskInQueueByDepth(id, KnmnCycleOpponentHitVoice);
    id = FTBL.AllocateVCDS_14(7, -1, KNMN_KEM, KNMN_RYO, 1, 100, 1);
    FTBL.AssignNextTaskInQueueByDepth(id, KnmnCycleOpponentAttackVoice);
    id = FTBL.AllocateVCDS_13(10, -1, KNMN_RYO, 0, 150, 100, 1);
    FTBL.AssignNextTaskInQueueByDepth(id, KnmnCyclePlayerAdviceVoice);
    id = FTBL.AllocateVCDS_16(8, -1, KNMN_RYO, 0, 1, 1, 100, 1);
    FTBL.AssignNextTaskInQueueByDepth(id, KnmnCyclePlayerMotionVoice);
    id = FTBL.AllocateVCDS_17(10, -2, KNMN_KEM, KNMN_RYO, 1, 100, 1);
    FTBL.AssignNextTaskInQueueByDepth(id, KnmnResumeConditionalDialogues);
    knmnConditionalDialogues[0] = FTBL.AllocateVCDS_16(8, -1, KNMN_RYO, 0, 283, 1, 100, 2);
    FTBL.AssignNextTaskInQueueByDepth(knmnConditionalDialogues[0], KnmnCyclePlayerEvadeVoice);
    FTBL.vfDisableCameraDialogue(knmnConditionalDialogues[0]);
    knmnConditionalDialogues[1] = FTBL.AllocateVCDS(9, -1, KNMN_RYO, 0, 12, 5, 100, 2);
    FTBL.AssignNextTaskInQueueByDepth(knmnConditionalDialogues[1], KnmnCyclePlayerFirstReactionVoice);
    FTBL.vfDisableCameraDialogue(knmnConditionalDialogues[1]);
    knmnConditionalDialogues[2] = FTBL.AllocateVCDS(9, -1, KNMN_RYO, 0, 48, 5, 100, 2);
    FTBL.AssignNextTaskInQueueByDepth(knmnConditionalDialogues[2], KnmnCyclePlayerSecondReactionVoice);
    FTBL.vfDisableCameraDialogue(knmnConditionalDialogues[2]);
}
int KnmnResumeConditionalDialogues(void *data) {
    FTBL.ResetCameraDialogueNode(knmnConditionalDialogues[0]);
    FTBL.ResetCameraDialogueNode(knmnConditionalDialogues[1]);
    FTBL.ResetCameraDialogueNode(knmnConditionalDialogues[2]);
}
int KnmnCycleVoice(const short *keys, unsigned short *index, int count) {
    short key = keys[*index];
    ++*index;
    if (*index >= count)
        *index = 0;
    return key;
}
int KnmnCyclePlayerFirstReactionVoice(void *data) {
    short keys[] = {113, 114, 115};
    return KnmnCycleVoice(keys, (unsigned short *)data, 3);
}
int KnmnCyclePlayerSecondReactionVoice(void *data) {
    short keys[] = {116, 117, 118};
    return KnmnCycleVoice(keys, (unsigned short *)data, 3);
}
int KnmnCyclePlayerEvadeVoice(void *data) {
    short keys[] = {119, 120, 121};
    return KnmnCycleVoice(keys, (unsigned short *)data, 3);
}
int KnmnCycleOpponentHitVoice(void *data) {
    short keys[] = {122, 123, 124};
    return KnmnCycleVoice(keys, (unsigned short *)data, 3);
}
int KnmnCyclePlayerMotionVoice(void *data) {
    short keys[] = {125, 126, 127};
    return KnmnCycleVoice(keys, (unsigned short *)data, 3);
}
int KnmnCycleOpponentAttackVoice(void *data) {
    short keys[] = {128, 129, 130, 131, 132};
    return KnmnCycleVoice(keys, (unsigned short *)data, 5);
}
int KnmnCyclePlayerAdviceVoice(void *data) {
    short keys[] = {133, 134, 135};
    return KnmnCycleVoice(keys, (unsigned short *)data, 3);
}
void KnmnInstallFirstEndConditions(void) {
    int id;
    knmnEndRequested = 0;
    id = FTBL.AllocateVCDS_2(0, -2, KNMN_RYO, 0, 20, 100, 1);
    FTBL.AssignNextTaskInQueueByDepth(id, KnmnRequestFirstBattleEnd);
    id = FTBL.AllocateVCDS_6(0, -2, 0, 0, 100, 1800, 0);
    FTBL.AssignNextTaskInQueueByDepth(id, KnmnRequestFirstBattleEnd);
}
int KnmnRequestFirstBattleEnd(void *data) {
    knmnEndRequested = 1;
    knmnEndTimer = 55;
    FTBL.EV_vfBattleUtil(4, 1, 1);
    FTBL.vfmDispatchControl(7, KNMN_KEM, (void *)0x08000d1e, 1);
}
int KnmnTickFirstEndTimer(void) {
    int result = 0;
    if (knmnEndRequested) {
        int frames;
        if (knmnEndTimer)
            knmnEndTimer--;
        if (knmnEndTimer == 0)
            result = 1;
        else {
            frames = knmnEndTimer;
            if (frames == 10 && !FTBL.filmCheckFadeTrackColour())
                FTBL.SetScreenFilmFade(frames, 0, 0, 0, 0, 255, 0, 0, 0);
        }
    }
    return result;
}
void KnmnInstallButokuEndConditions(void) {
    int id;
    knmnEndRequested = 0;
    id = FTBL.AllocateVCDS_2(0, -2, KNMN_RYO, 0, 20, 100, 1);
    FTBL.AssignNextTaskInQueueByDepth(id, KnmnRequestButokuBattleEndCallback);
    id = FTBL.AllocateVCDS_6(0, -2, 0, 0, 100, 1800, 0);
    FTBL.AssignNextTaskInQueueByDepth(id, KnmnRequestButokuBattleEndCallback);
}
int KnmnRequestButokuBattleEndCallback(void *data) {
    knmnEndRequested = 1;
    knmnEndTimer = 300;
}
int KnmnTickButokuEndTimer(void) {
    int result = 0;
    if (knmnEndRequested) {
        int frames;
        if (knmnEndTimer)
            knmnEndTimer--;
        if (knmnEndTimer == 0)
            result = 1;
        else {
            frames = knmnEndTimer;
            if (frames == 10 && !FTBL.filmCheckFadeTrackColour())
                FTBL.SetScreenFilmFade(frames, 0, 0, 0, 0, 255, 0, 0, 0);
        }
    }
    return result;
}
void KnmnRequestButokuBattleEnd(void) {
    knmnEndRequested = 1;
    knmnEndTimer = 300;
}

void KnmnCheckPlayerPosition(int line)
{
 union { Vector3 position; unsigned int bits[3]; } p;
 FTBL.GetCharaPosition(KNMN_RYO,-1,&p.position,0x40000000U);
 if(p.bits[0]>0x007fffffU){
  if(p.bits[0]<=0x7f7fffffU)goto checkZ;
 }
 if(p.bits[0]>0x807fffffU){
  if(p.bits[0]<=0xff7fffffU)goto checkZ;
 }
 if(p.bits[0]!=0&&p.bits[0]!=0x80000000U)goto invalid;
checkZ:
 if(p.bits[2]>0x007fffffU){
  if(p.bits[2]<=0x7f7fffffU)return;
 }
 if(p.bits[2]>0x807fffffU){
  if(p.bits[2]<=0xff7fffffU)return;
 }
 if(p.bits[2]==0||p.bits[2]==0x80000000U)return;
invalid:
 FTBL.DebugLogToFileRemoved("/%d\n",line);
 FTBL.EventAssertNullsub(__FILE__,125);
}

HLibTask *KnmnQueuePractice(KnmnGameWork *game)
{
 HLibTask *task=FTBL.HLib_EnqueueTaskWithParameter(KnmnPracticeMainCallback,4,11,sizeof(KnmnPracticeWork),TASK_EVNT);
 KnmnPracticeWork *work=(KnmnPracticeWork*)FTBL.HLib_GetTaskParameter(task);
 work->task=task;
 work->state=0;
 work->frames=0;
 work->active=1;
 work->control.fight.fightSuspended=0;
 work->game=game;
 FTBL.DebugLogToFileRemoved("wd : %x\n",work);
 FTBL.DebugLogToFileRemoved("hdw : %x\n",&work->control);
 FTBL.DebugLogToFileRemoved("htask : %x\n",work->task);
 FTBL.DebugLogToFileRemoved("End of initialyze\n");
 return task;
}

int KnmnFinishPractice(HLibTask *task)
{
 KnmnPracticeWork *work=(KnmnPracticeWork*)FTBL.HLib_GetTaskParameter(task);
 if(work->state==6){FTBL.HLib_DestroyTaskInternal(task);return 1;}
 return 0;
}

void KnmnPracticeMainCallback(void *data)
{
 KnmnPracticeWork *work=(KnmnPracticeWork*)data;
 int i;
 switch(work->state){
 case 0:
  FTBL.SetPlayerControlEnabled(0);
  work->motions[0]=0;
  work->state=1;
  break;
 case 1:work->state=2;break;
 case 2:
  work->sequence=ThrCreate(KnmnPracticeRootSequence,SHEN_NAME4('S','E','Q','P'),work);
  work->state=3;
  break;
 case 3:if(!work->active)work->state=4;break;
 case 4:work->state=5;break;
 case 5:
  if(work->control.fight.fightSuspended){
   work->control.fight.fightSuspended=0;
   ResumeTrainingFight(work,KNMN_RYO,KNMN_DEKU);
   FTBL.check_dead_status(1);
  }
  for(i=0;work->motions[i];i++)FTBL.DeleteMotion(work->motions[i]);
  work->state=6;
  break;
 case 6:break;
 default:
  FTBL.DebugLogToFileRemoved("mode error ! : %d\n",work->state);
  FTBL.EventAssertNullsub(__FILE__,248);
 }
 IncrementTrainingFrames(work);
}

void KnmnInitializeDemonstrationSequence(void *data)
{
 struct {float x,y,z,w;} reflector={1,.3f,.3f,.3f};
 FTBL.EV_EnableCharacterDisplay(KNMN_RYO);
 FTBL.EV_EnableCharacterDisplay(KNMN_KEM);
 FTBL.EV_DisableCharacterDisplay(KNMN_DEKU);
 DisableTrainingAlpha(KNMN_RYO);
 DisableTrainingAlpha(KNMN_KEM);
 FTBL.EV_SetCharaControllerFlags(KNMN_RYO,2,0);
 FTBL.EV_SetCharaControllerFlags(KNMN_RYO,3,0);
 SHENMUE_EVENT_BOUNDARY();
 FTBL.EV_SetCharaControllerFlags(KNMN_KEM,2,0);
 FTBL.EV_SetCharaControllerFlags(KNMN_KEM,3,0);
 SHENMUE_EVENT_BOUNDARY();
 FTBL.EV_SetCharaControllerFlags(KNMN_DEKU,2,0);
 FTBL.EV_SetCharaControllerFlags(KNMN_DEKU,3,0);
 FTBL.EV_SetCharaControllerFlags(KNMN_RYO,1,0);
 FTBL.EV_SetCharaControllerFlags(KNMN_KEM,1,0);
 FTBL.SetScenarioCharacterDrawControl(KNMN_RYO,1,1,0);
 FTBL.SetScenarioCharacterDrawControl(KNMN_KEM,1,1,0);
 FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(KNMN_RYO),1);
 FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(KNMN_KEM),0);
 FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(KNMN_RYO),1,0);
 FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(KNMN_KEM),1,0);
 FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(KNMN_RYO),1);
 FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(KNMN_KEM),1);
 FTBL.SetScenarioReflectorBoardValues(KNMN_RYO,&reflector.x);
 FTBL.SetScenarioReflectorBoardValues(KNMN_KEM,&reflector.x);
 FTBL.SetRefBoardParamsForCharaById(KNMN_RYO,1);
 FTBL.SetRefBoardParamsForCharaById(KNMN_KEM,1);
 FTBL.EV_AT_SetPos(KNMN_RYO,0x78000000,&knmnPlayerPracticePosition);
 SetTrainingCharacterYaw(KNMN_RYO,knmnPlayerPracticeYaw);
 ClearTrainingMotionFlag(KNMN_RYO);
 FTBL.EV_AT_SetPos(KNMN_KEM,0x78000000,&knmnTeacherPracticePosition);
 SetTrainingCharacterYaw(KNMN_KEM,knmnTeacherPracticeYaw);
 ClearTrainingMotionFlag(KNMN_KEM);
 FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(KNMN_KEM),5193,0);
 ThrSignalParent((Thr*)data,1);
 ThrKillOwn((Thr*)data);
}

void KnmnPrepareAuthorCharacters(KnmnPracticeWork *unused)
{
 FTBL.EV_EnableCharacterDisplay(KNMN_KEM);
 ResetTrainingAuthorCharacter(KNMN_RYO);
 ResetTrainingAuthorCharacter(KNMN_KEM);
}

void KnmnRestoreAuthorCharacters(void)
{
 RestoreTrainingCharacter(KNMN_RYO);
 RestoreTrainingCharacter(KNMN_KEM);
}

void KnmnExitPracticeSequence(void *data)
{
 Thr *thr=(Thr*)data;
 KnmnPracticeWork *work=(KnmnPracticeWork*)ThrGetWork(thr);
 int retry=ThrGetUser(thr,1),finish=ThrGetUser(thr,3);
 KnmnPracticeControl *control=&work->control;
 ThrResetLoopCount(thr);
 do{
  ThrBeginStep(thr);
  switch(ThrGetSeq(thr)){
  case 0:ThrChangeSeq(thr,100,1);break;
  case 100:
   FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(KNMN_KEM),5193,0);
   if(control->practiceExpired)ThrChangeSeq(thr,101,1);
   else if(control->practiceWarning)ThrChangeSeq(thr,105,1);
   else{ThrSetReturnValue(thr,retry);ThrChangeSeq(thr,125,1);}break;
  case 101:control->conversationActive=1;ThrWaitAuthor(thr,103,0);break;
  case 103:ThrWaitAuthor(thr,104,0);break;
  case 104:ThrSetReturnValue(thr,finish);control->practiceExpired=0;ThrChangeSeq(thr,125,1);break;
  case 125:ThrReturnToParent(thr,1);ThrKillOwn(thr);break;
  default:ThrProcess(thr);break;
  }
  ThrEndStep(thr);
 }while(ThrIsContinue(thr));
}

KnmnFeedbackSelection *KnmnSelectFeedback(Thr *thr,KnmnFeedbackSelection *selection)
{
 KnmnPracticeWork *work=(KnmnPracticeWork*)ThrGetWork(thr);
 KnmnPracticeControl *control=&work->control;
 KnmnMoveResult *result=&work->results[control->resultIndex];
 HLibTaskCallback callback;
 if(control->result==20)callback=KnmnIdleAdviceSequence;
 else if(result->steps==1){
  switch(result->error){
  case 12:callback=KnmnStayAndStrikeSequence;break;
  case 2:callback=KnmnRelaxShouldersSequence;break;
  case 3:callback=KnmnUseHandsSequence;break;
  case 4:callback=KnmnUseLegsSequence;break;
  case 11:callback=KnmnRelaxShouldersLegsSequence;break;
  case 10:callback=KnmnRelaxShouldersArmsSequence;break;
  case 15:
   if(work->currentResult.detail&1)callback=KnmnHandsSlowSequence;
   else callback=KnmnHandsLegsTogetherSequence;
   break;
  default:
   callback=KnmnRelaxWithoutGestureSequence;
   FTBL.DebugLogToFileRemoved("cause err : %d/n",work->currentResult.error);break;
  }
 }else{
  callback=KnmnUnclassifiedFeedbackSequence;
  FTBL.DebugLogToFileRemoved("cause err : %d : ３つめの動作はないはず/n",work->currentResult.error);
 }
 if(!callback){
  callback=KnmnRelaxWithoutGestureSequence;
  FTBL.DebugLogToFileRemoved("!! cause err : %d/n",work->currentResult.error);
 }
 selection->callback=callback;selection->argument=0;
 return selection;
}

void KnmnFeedbackSequence(void *data)
{
 Thr *thr=(Thr*)data;
 KnmnPracticeWork *work=(KnmnPracticeWork*)ThrGetWork(thr);
 KnmnPracticeControl *control=&work->control;
 KnmnFeedbackSelection selection;
 HLibTaskCallback callback;
 ThrResetLoopCount(thr);
 do{
  ThrBeginStep(thr);
  switch(ThrGetSeq(thr)){
  case 0:ThrChangeSeq(thr,100,1);break;
  case 100:
   KnmnSelectFeedback(thr,&selection);
   callback=selection.callback;
   ThrMakeChild(thr,callback,SHEN_NAME4('F','S','1','S'),work,1);
   ThrSetChildDone(thr,1,0);ThrWaitChildResult(thr,101,1,1);break;
  case 101:
   control->dialogueSequence=0;
   ThrSignalParent(thr,1);ThrKillOwn(thr);break;
  default:ThrProcess(thr);break;
  }
  ThrEndStep(thr);
 }while(ThrIsContinue(thr));
}

int KnmnPracticeExitNullsub(Thr *thr,int practice){return 0;}

void KnmnRepeatFeedbackSequence(void *data)
{
 Thr *thr=(Thr*)data;
 ThrResetLoopCount(thr);
 do{
  ThrBeginStep(thr);
  switch(ThrGetSeq(thr)){
  case 0:ThrChangeSeq(thr,100,1);break;
  case 100:
   FTBL.EV_AT_SetPos(KNMN_RYO,0x78000000,&knmnPlayerPracticePosition);SetTrainingCharacterYaw(KNMN_RYO,knmnPlayerPracticeYaw);
   FTBL.EV_AT_SetPos(KNMN_KEM,0x78000000,&knmnTeacherPracticePosition);SetTrainingCharacterYaw(KNMN_KEM,knmnTeacherPracticeYaw);
   FTBL.EV_DisableCharacterDisplay(KNMN_KEM);ThrChangeSeq(thr,101,1);break;
  case 101:
   FTBL.figpSetFlagsBlendForBlur(KNMN_RYO,8,2);
   TrainingAssignFigure(KNMN_RYO,4);ClearTrainingMotionFlag(KNMN_RYO);
   FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(KNMN_RYO),1);
   TrainingSetMotion(KNMN_RYO,1,-1,-1,-1,1);
   FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(KNMN_RYO),69,0,0);
   ThrWaitMotion(thr,102,1,KNMN_RYO);break;
  case 102:
   FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(KNMN_RYO),1,0);TrainingClearFigure(KNMN_RYO);
   ThrSignalParent(thr,1);ThrKillOwn(thr);break;
  default:ThrProcess(thr);break;
  }
  ThrEndStep(thr);
 }while(ThrIsContinue(thr));
}

void KnmnSuccessSequence(void *data)
{
 Thr *thr=(Thr*)data;
 int continuation=ThrGetUser(thr,2);
 KnmnPracticeWork *work=(KnmnPracticeWork*)ThrGetWork(thr);
 Vector3 target;
 ThrResetLoopCount(thr);
 do{
  ThrBeginStep(thr);
  switch(ThrGetSeq(thr)){
  case 0:ThrChangeSeq(thr,103,1);break;
  case 100:ThrChangeSeq(thr,101,1);break;
  case 101:
   TrainingSetCamera(work,9001);
   ThrMakeChild(thr,KnmnRepeatFeedbackSequence,SHEN_NAME4('R','E','P','L'),work,0);
   ThrWaitChildExit(thr,102,1,0);break;
  case 102:ThrChangeSeq(thr,103,1);break;
  case 103:ThrWaitAuthor(thr,133,0);break;
  case 133:
   ThrSetReturnValue(thr,continuation);KnmnPrepareAuthorCharacters(work);
   FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(KNMN_KEM),0,3);
   FTBL.GetCharaPosition(KNMN_RYO,5,&target,0x40000000);
   FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(KNMN_KEM),&target,0);
   FTBL.EV_authSceneCtrl(SHEN_NAME4('W','A','Z','A'));ThrChangeSeq(thr,144,1);break;
  case 144:
   FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(KNMN_RYO),0,3);
   FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(KNMN_RYO),0,0);
   ThrWaitFrames(thr,134,1,1);break;
  case 134:
   if(!FTBL.EV_authSceneCtrl(-1))ThrChangeSeq(thr,135,1);
   else KnmnWazaFrameEvents(work->game);break;
  case 135:KnmnRestoreAuthorCharacters();ThrWaitFrames(thr,143,1,2);break;
  case 143:ThrReturnToParent(thr,1);ThrKillOwn(thr);break;
  default:ThrProcess(thr);break;
  }
  ThrEndStep(thr);
 }while(ThrIsContinue(thr));
}

int KnmnNeedsFacingReset(int practice)
{
 int wanted=0,difference;
 Vector3i rotation;
 if(practice>=0&&(practice<2||practice==9))wanted=knmnPlayerPracticeYaw;
 FTBL.GetScenarioCharacterEulerAngles(KNMN_RYO,&rotation,0x40000000);
 difference=TrainingFacingDifference(wanted,rotation.y);
 if(difference<0)difference=-difference;
 if(difference>10922)return 1;
 return 0;
}

void KnmnPracticeRootSequence(void *data)
{
 Thr *thr=(Thr*)data;
 KnmnPracticeWork *work=(KnmnPracticeWork*)ThrGetWork(thr);
 KnmnPracticeControl *control=&work->control;
 Thr *child;
 ThrResetLoopCount(thr);
 do{
  ThrBeginStep(thr);
  switch(ThrGetSeq(thr)){
  case 0:
   control->lcdSequence=0;
   ThrChangeSeq(thr,103,1);break;
  case 103:
   if(!control->lcdSequence)control->lcdSequence=ThrMakeChild(thr,TrainingLcdSequence,SHEN_NAME4('D','L','C','D'),work,1);
   ThrMakeChild(thr,KnmnStartPracticeSequence,SHEN_NAME4('S','0','0','0'),work,0);
   ThrSetChildDone(thr,0,0);
   ThrWaitChildResult(thr,104,1,0);break;
  case 104:
   child=ThrMakeChild(thr,KnmnPracticeDecisionSequence,SHEN_NAME4('W','D','M','A'),work,0);
   ThrSetUserName(child,0,0);ThrSetUserName(child,1,3);ThrSetUserName(child,2,-1);ThrSetUserName(child,3,106);
   ThrSetUserName(child,4,106);ThrSetUserName(child,5,104);ThrSetUserName(child,6,104);ThrSetUserName(child,7,105);
   ThrSetChildDone(thr,0,0);
   ThrWaitChildSequence(thr,1,0);break;
  case 105:
   FTBL.QueueFightAdviceControl(12);
   child=ThrMakeChild(thr,KnmnQteSequence,SHEN_NAME4('W','Q','T','E'),work,0);
   ThrSetUserName(child,0,106);
   ThrSetChildDone(thr,0,0);
   ThrWaitChildSequence(thr,1,0);break;
  case 106:ThrChangeSeq(thr,107,1);break;
  case 107:
   if(work->control.fight.fightSuspended){work->control.fight.fightSuspended=0;ResumeTrainingFight(work,KNMN_RYO,KNMN_DEKU);FTBL.check_dead_status(1);}
   ThrChangeSeq(thr,108,1);break;
  case 108:
   ThrMakeChild(thr,KnmnFinishPracticeSequence,SHEN_NAME4('S','1','0','0'),work,0);
   ThrSetChildDone(thr,0,0);ThrWaitChildResult(thr,110,1,0);break;
  case 110:
   ThrMakeChild(thr,KnmnAwardMoveSequence,SHEN_NAME4('S','2','0','0'),work,0);
   ThrSetChildDone(thr,0,0);ThrWaitChildResult(thr,111,1,0);break;
  case 111:
   work->active=0;
   if(control->lcdSequence){
    ThrKillChild(thr,SHEN_NAME4('D','L','C','D'),1);control->lcdSequence=0;
    FTBL.lcdmInit(0);FTBL.CheckHelpIcon(0x800,0x800,2);ThrKillOwn(thr);
   }
   FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(KNMN_RYO),0,0);
   FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(KNMN_KEM),0,0);break;
  default:ThrProcess(thr);break;
  }
  ThrEndStep(thr);
 }while(ThrIsContinue(thr));
}

void KnmnPracticeSequence(void *data)
{
 Thr *thr=(Thr*)data;
 KnmnPracticeWork *work=(KnmnPracticeWork*)ThrGetWork(thr);
 unsigned int instruction=(unsigned short)ThrGetUser(thr,0),second=(unsigned short)ThrGetUser(thr,1);
 Thr *child;
 EventFunctionTable *table=&FTBL;
 Vector3 target;
 ThrResetLoopCount(thr);
 do{
  ThrBeginStep(thr);
  switch(ThrGetSeq(thr)){
  case 0:
   work->following=0;
   FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(KNMN_RYO),0,0);
   table->EV_SetCharaControllerFlags(KNMN_RYO,2,1);table->EV_SetCharaControllerFlags(KNMN_RYO,3,1);
   table->EV_SetCharaControllerFlags(KNMN_DEKU,2,1);table->EV_SetCharaControllerFlags(KNMN_DEKU,3,1);
   FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(KNMN_RYO),1);
   FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(KNMN_DEKU),1);
   ClearTrainingMotionFlag(KNMN_RYO);
   FTBL.EV_AT_SetPos(KNMN_DEKU,0x38000000,&knmnDummyPracticePosition);
   FTBL.FinishLoading();FTBL.CheckHelpIcon(1,0,1);
   ThrChangeSeq(thr,100,1);break;
  case 100:ThrWaitFrames(thr,101,1,1);break;
  case 101:
   FTBL.vfSetMoveInstructionConfiguration(KNMN_RYO,KNMN_KEM,(short)instruction,(short)second,&work->currentResult);
   FTBL.GetCharaPosition(KNMN_RYO,5,&target,0x40000000);
   FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(KNMN_KEM),&target,0);
   child=ThrMakeChild(thr,TrainingAttackSequence,SHEN_NAME4('B','A','T','T'),work,0);
   ThrSetUser(child,0,KNMN_RYO);ThrSetUser(child,1,KNMN_DEKU);ThrSetUserName(child,2,1);ThrSetUserName(child,3,1);
   ThrSetUserLong(child,4,0);ThrWaitFrames(thr,102,1,10);break;
  case 102:
   if(work->currentResult.status==2){TrainingSuspendFight(work,KNMN_RYO,KNMN_DEKU);ThrChangeSeq(thr,104,1);}break;
  case 103:
   if(!FTBL.IsFightActive())ThrChangeSeq(thr,104,1);
   else TrainingSuspendFight(work,KNMN_RYO,KNMN_DEKU);
   break;
  case 104:ThrWaitChildExit(thr,105,1,0);break;
  case 105:
   FTBL.SetLoadingBusy();FTBL.CheckHelpIcon(1,1,2);
   table->EV_SetCharaControllerFlags(KNMN_RYO,2,0);table->EV_SetCharaControllerFlags(KNMN_RYO,3,0);
   table->EV_SetCharaControllerFlags(KNMN_DEKU,2,0);table->EV_SetCharaControllerFlags(KNMN_DEKU,3,0);
   FTBL.DebugLogToFileRemoved(" stat//resutl//cause//m_line : %d %d %d %d\n",work->currentResult.status,work->currentResult.phase,work->currentResult.error,work->currentResult.steps);
   ThrSignalParent(thr,1);ThrKillOwn(thr);break;
  default:ThrProcess(thr);break;
  }
  ThrEndStep(thr);
 }while(ThrIsContinue(thr));
}

void KnmnPracticeCameraSequence(void *data)
{
 Thr *thr=(Thr*)data;
 KnmnPracticeWork *work=(KnmnPracticeWork*)ThrGetWork(thr);
 unsigned int camera=ThrGetUser(thr,0);
 ThrResetLoopCount(thr);
 do{
  ThrBeginStep(thr);
  switch(ThrGetSeq(thr)){
  case 0:ThrWaitFrames(thr,100,1,2);break;
  case 100:ThrWaitEventCamera(thr,101,1);break;
  case 101:TrainingSetCamera(work,camera);ThrWaitFrames(thr,100,1,2);break;
  default:ThrProcess(thr);break;
  }
  ThrEndStep(thr);
 }while(ThrIsContinue(thr));
}

int KnmnSelectPracticeCamera(KnmnPracticeWork *work,int practice)
{
 int camera=0;
 KnmnPracticeControl *control=&work->control;
 if(!control->changeCamera)return control->practiceCamera;
 if(practice>=0&&(practice<=3||practice==9)){
  if(control->practiceCamera==9010)camera=9011;
  else camera=9010;
 }else FTBL.EventAssertMessageNullsub(__FILE__,1241,"Illigal step number !");
 control->changeCamera=0;
 return camera;
}

void KnmnClearPracticeCamera(KnmnPracticeControl *control)
{
 TrainingKillOtherHandle(&control->cameraSequence,SHEN_NAME4('W','D','C','A'));
}

void KnmnSetTrainingCameraSequence(Thr *thr,unsigned int camera)
{
 KnmnPracticeWork *work=(KnmnPracticeWork*)ThrGetWork(thr);
 Thr *child;
 KnmnPracticeControl *control=&work->control;
 if(work->camera!=camera){
  FTBL.DebugLogToFileRemoved("cam old %d new %d\n",work->camera,camera);
  TrainingSetCamera(work,camera&0xffff);
 }
 KnmnClearPracticeCamera(control);
 child=ThrMakeChild(thr,KnmnPracticeCameraSequence,SHEN_NAME4('W','D','C','A'),work,3);
 ThrSetUserName(child,0,camera);
 control->cameraSequence=child;
}

void KnmnResetActors(int practice)
{
 if(practice>=0&&(practice<2||practice==9)){
  FTBL.EV_AT_SetPos(KNMN_RYO,0x78000000,&knmnPlayerPracticePosition);
  SetTrainingCharacterYaw(KNMN_RYO,knmnPlayerPracticeYaw);ClearTrainingMotionFlag(KNMN_RYO);
 }
 if(FTBL.GetMotionAnimationId(FTBL.scenFindMotionwork(KNMN_RYO),0)!=306){
  ClearTrainingMotionFlag(KNMN_RYO);
  FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(KNMN_RYO),0,0);
  FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(KNMN_RYO),306,0);
  ClearTrainingMotionFlag(KNMN_RYO);
  FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(KNMN_RYO),69,0,0);
 }
 FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(KNMN_RYO),1);
 FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(KNMN_RYO),69,0,0);
 FTBL.EV_AT_SetPos(KNMN_KEM,0x78000000,&knmnTeacherPracticePosition);
 SetTrainingCharacterYaw(KNMN_KEM,knmnTeacherPracticeYaw);ClearTrainingMotionFlag(KNMN_KEM);
 FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(KNMN_KEM),5193,0);
}

const char knmnPracticeCountLog[] = "@@@ cnt %d : Wcnt %d : time %d : time_old %d\n";
const char knmnPracticeBufferLog[] = "WD Buffer Over : step1\n";
void KnmnPracticeDecisionSequence(void *data)
{
    Thr *thr = (Thr *)data;
    KnmnPracticeWork *work = (KnmnPracticeWork *)ThrGetWork(thr);
    KnmnPracticeControl *control = &work->control;
    int *attempt = ThrGetBuffer2Address(thr,0);
    int *restartPractice = ThrGetBuffer2Address(thr,1);
    int *resetFacing = ThrGetBuffer2Address(thr,2);
    int practice = ThrGetUser(thr,0);
    int instruction = ThrGetUser(thr,1);
    int second = ThrGetUser(thr,2);
    int success = ThrGetUser(thr,3);
    int retry = ThrGetUser(thr,5);
    int restart = ThrGetUser(thr,6);
    int finish = ThrGetUser(thr,7);
    Thr *child;
    int result;
    ThrResetLoopCount(thr);
    do {
        ThrBeginStep(thr);
        switch (ThrGetSeq(thr)) {
        case 0:
            if (!TrainingIsTurning(work))
                ThrChangeSeq(thr,100,1);
            break;
        case 100:
            control->practice = practice;
            control->advicePending = 0;
            control->timedAdvice = 0;
            control->practiceWarning = 0;
            control->practiceExpired = 0;
            control->repetitions = 0;
            control->consecutiveRepetitions = 0;
            control->previousRepetitions = 0;
            control->attemptedMoves = 0;
            control->changeCamera = 1;
            control->restartMotion = 1;
            control->cameraSequence = 0;
            control->dialogueSequence = 0;
            control->timerSequence = 0;
            control->adviceSequence = 0;
            KnmnResetActors(practice);
            FTBL.EV_AT_SetPos(KNMN_KEM,0x78000000,&knmnTeacherPracticePosition);
            SetTrainingCharacterYaw(KNMN_KEM,knmnTeacherPracticeYaw);
            ClearTrainingMotionFlag(KNMN_KEM);
            FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(KNMN_KEM),5193,0);
            *resetFacing = 1;
            ThrChangeSeq(thr,101,1);
            break;
        case 101:
            control->practiceCamera = KnmnSelectPracticeCamera(work,practice);
            KnmnSetTrainingCameraSequence(thr,control->practiceCamera);
            control->adviceElapsed = 0;
            child = ThrMakeChild(thr,TrainingTimerSequence,SHEN_NAME4('T','I','M','M'),work,4);
            control->timerSequence = child;
            child = ThrMakeChild(thr,TrainingAdviceTimerSequence,SHEN_NAME4('W','A','D','V'),work,5);
            control->adviceSequence = child;
            ThrSetUserName(child,0,practice);
            ThrSetUserName(child,1,2);
            control->attempts = 0;
            *attempt = 0;
            ThrChangeSeq(thr,102,1);
            break;
        case 102:
            *restartPractice = 0;
            if (control->elapsed >= 10800) {
                ThrSetReturnValue(thr,finish);
                ThrChangeSeq(thr,132,1);
            } else {
                if (*resetFacing) {
                    control->practiceCamera = KnmnSelectPracticeCamera(work,practice);
                    KnmnSetTrainingCameraSequence(thr,control->practiceCamera);
                    KnmnResetActors(practice);
                }
                child = ThrMakeChild(thr,KnmnPracticeSequence,SHEN_NAME4('W','D','T','P'),work,0);
                ThrSetUserName(child,0,instruction);
                ThrSetUserName(child,1,second);
                ThrSetChildDone(thr,0,0);
                ThrWaitChildResult(thr,103,1,0);
            }
            break;
        case 103:
            if (KnmnNeedsFacingReset(practice)) *resetFacing = 1;
            else *resetFacing = 0;
            if (work->currentResult.phase == 2 && practice == 9 && control->advicePending)
                *resetFacing = 0;
            if (work->currentResult.error != 20 || control->timedAdvice || control->practiceWarning || control->practiceExpired ||
                KnmnPracticeExitNullsub(thr,practice) || control->advicePending)
                control->restartMotion = 1;
            else
                control->restartMotion = 0;
            control->changeCamera = 1;
            TrainingCopyResult(work->results,&work->currentResult,control->attempts);
            if (work->currentResult.error != 20)
                ++control->attemptedMoves;
            if (work->currentResult.phase == 2) ThrChangeSeq(thr,104,1);
            else ThrChangeSeq(thr,116,1);
            break;
        case 104:
            if (!*resetFacing)
                QueueTrainingReposition(thr,2,KNMN_RYO,0,&knmnPlayerPracticePosition);
            ++control->repetitions;
            ++control->consecutiveRepetitions;
            FTBL.DebugLogToFileRemoved(knmnPracticeCountLog,control->adviceElapsed,control->elapsed,control->consecutiveRepetitions,control->previousRepetitions);
            ThrWaitFrames(thr,105,1,1);
            break;
        case 105:
            if (!control->advicePending) {
                if (!*resetFacing)
                    ThrChangeSeqSave(thr,133,130,1);
                else
                    ThrChangeSeq(thr,130,1);
            } else {
                if (control->dialogueSequence)
                    TrainingKillOtherTreeHandle(&control->dialogueSequence,0);
                KnmnClearPracticeCamera(control);
                TrainingKillOtherHandle(&control->timerSequence,SHEN_NAME4('T','I','M','M'));
                TrainingKillOtherHandle(&control->adviceSequence,SHEN_NAME4('W','A','D','V'));
                control->result = TrainingResultError(work->results,control->attempts);
                control->resultIndex = TrainingResultNullsub(control->attempts,control->result);
                result = KnmnPracticeExitNullsub(thr,practice);
                if (control->practiceWarning | control->practiceExpired | control->timedAdvice | result)
                    ThrChangeSeq(thr,106,1);
                else
                    ThrChangeSeq(thr,114,1);

            }
            break;
        case 106:
            FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(KNMN_RYO),6,0);
            if (!*resetFacing)
                ThrChangeSeqSave(thr,133,107,1);
            else
                ThrChangeSeq(thr,107,1);
            break;
        case 107:
            if (control->practiceExpired && control->attemptedMoves > 8 && !practice)
                TrainingSetLcd(&work->control.fight.lcd.frames,knmnPracticeLcd);
            switch(practice) {
            case 0:
                ThrMakeChild(thr,KnmnFeedbackSequence,SHEN_NAME4('F','S','_','1'),work,1);
                ThrSetChildDone(thr,1,0);
                ThrWaitChildResult(thr,108,1,1);
                break;
            case 1:
                result = 0;
                break;
            case 2:
                break;
            }
            break;
        case 108:
            child = ThrMakeChild(thr,KnmnExitPracticeSequence,SHEN_NAME4('E','A','L','P'),work,1);
            ThrSetUserName(child,0,1);
            ThrSetUserName(child,1,retry);
            ThrSetUserName(child,2,restart);
            ThrSetUserName(child,3,finish);
            ThrSetUserName(child,4,practice);
            ThrSetChildDone(thr,1,0);
            ThrWaitChildResult(thr,112,1,1);
            break;
        case 109:
            break;
        case 111:
            break;
        case 112:
            ThrSetReturnValue(thr,ThrGetChildValue(thr,1));
            ThrChangeSeq(thr,113,1);
            break;
        case 113:
            *resetFacing = 1;
            ThrChangeSeq(thr,115,1);
            break;
        case 114:
            switch(practice) {
            case 0:
                child = ThrMakeChild(thr,KnmnFeedbackSequence,SHEN_NAME4('F','S','_','1'),work,1);
                control->dialogueSequence = child;
                ThrSetReturnValue(thr,retry);
                break;
            case 1:
                result = 0;
                break;
            case 2:
                break;
            }
            ThrChangeSeq(thr,115,1);
            break;
        case 115:
            control->attempts = 0;
            control->adviceElapsed = 0;
            control->advicePending = 0;
            control->timedAdvice = 0;
            control->practiceWarning = 0;
            control->practiceExpired = 0;
            control->consecutiveRepetitions = 0;
            control->previousRepetitions = 0;
            if (ThrGetReturnValue(thr) != retry)
                ThrChangeSeq(thr,132,1);
            else {
                if (!*resetFacing) ThrChangeSeqSave(thr,133,131,1);
                else ThrChangeSeq(thr,131,1);
            }
            break;
        case 116:
            TrainingKillOtherTreeHandle(&control->dialogueSequence,0);
            KnmnClearPracticeCamera(control);
            TrainingKillOtherHandle(&control->timerSequence,SHEN_NAME4('T','I','M','M'));
            TrainingKillOtherHandle(&control->adviceSequence,SHEN_NAME4('W','A','D','V'));
            FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(KNMN_RYO),6,0);
            ThrWaitFrames(thr,117,1,1);
            break;
        case 117:
            if (!control->firstFailures[practice]) {
                control->repeatedFailures[practice] = 0;
                control->firstFailures[practice] = 1;
            }
            ThrChangeSeq(thr,120,1);
            break;
        case 120:
            control->result = TrainingResultError(work->results,control->attempts + 1);
            control->resultIndex = TrainingResultNullsub(control->attempts + 1,control->result);
            if (!practice) ThrChangeSeq(thr,121,1);
            break;
        case 121:
            ThrSetReturnValue(thr,success);
            TrainingSetLcdOrder(&work->control.fight.lcd.frames,knmnPracticeLcd,0);
            child = ThrMakeChild(thr,KnmnSuccessSequence,SHEN_NAME4('S','S','_','1'),work,1);
            ThrSetUserName(child,0,control->result);
            ThrSetUserName(child,1,control->resultIndex);
            ThrSetUserName(child,2,success);
            ThrSetUserName(child,3,finish);
            ThrSetChildDone(thr,1,0);
            ThrWaitChildResult(thr,126,1,1);
            break;
        case 122:
            break;
        case 123:
            break;
        case 124:
            break;
        case 125:
            break;
        case 126:
            work->learnedMove = 6;
            ThrSetReturnValue(thr,ThrGetChildValue(thr,1));
            ThrChangeSeq(thr,127,1);
            break;
        case 127:
            ThrChangeSeq(thr,128,1);
            break;
        case 128:
            if (!*restartPractice) {
                control->elapsed = 0;
                ThrChangeSeq(thr,132,1);
            } else {
                *resetFacing = 1;
                child = ThrMakeChild(thr,TrainingTimerSequence,SHEN_NAME4('T','I','M','M'),work,4);
            control->timerSequence = child;
                child = ThrMakeChild(thr,TrainingAdviceTimerSequence,SHEN_NAME4('W','A','D','V'),work,5);
                control->adviceSequence = child;
                ThrSetUserName(child,0,practice);
                ThrSetUserName(child,1,2);
                ThrWaitFrames(thr,129,1,2);

            }
            break;
        case 129:
            ThrChangeSeq(thr,130,1);
            break;
        case 130:
            if (control->attempts < 100)
                ++control->attempts;
            else
                FTBL.DebugLogToFileRemoved(knmnPracticeBufferLog);
            ThrChangeSeq(thr,102,1);
            break;
        case 131:
            ThrChangeSeq(thr,101,1);
            break;
        case 132:
            TrainingKillOtherHandle(&control->timerSequence,SHEN_NAME4('T','I','M','M'));
            TrainingKillOtherHandle(&control->adviceSequence,SHEN_NAME4('W','A','D','V'));
            TrainingKillOtherTreeHandle(&control->dialogueSequence,0);
            KnmnClearPracticeCamera(control);
            ThrReturnToParent(thr,1);
            ThrKillOwn(thr);
            break;
        case 133:
            if (!TrainingIsRepositioning(work,0)) ThrReturnSeq(thr,1);
            break;
        case 134:
            if (!TrainingIsRepositioning(work,1)) ThrReturnSeq(thr,1);
            break;
        default:
            ThrProcess(thr);
            break;
        }
        KnmnCheckPlayerPosition(ThrGetSeq(thr));
        ThrEndStep(thr);
    } while (ThrIsContinue(thr));
}

const char knmnQteStartLog[] = "QteStart\n";
const char knmnQtePunchLog[] = "Punch!\n";
void KnmnQteSequence(void *data)
{
    Thr *sequence = (Thr *)data;
    TrainingWork *work = (TrainingWork *)ThrGetWork(sequence);
    TrainingStats *control = &work->control;
    int continuation = ThrGetUser(sequence,0);
    int *wait = ThrGetBuffer2Address(sequence,0);
    int *previous = ThrGetBuffer2Address(sequence,1);
    int *retry = ThrGetBuffer2Address(sequence,2);
    Thr *child;
    int result, motion;
    shen_name player = KNMN_RYO, dummy = KNMN_DEKU;
    ThrSetDebugId(ThrGetId(sequence));
    ThrResetLoopCount(sequence);
    do {
        ThrBeginStep(sequence);
        switch (ThrGetSeq(sequence)) {
        case 0:
            if (work->control.fight.fightSuspended) {
                work->control.fight.fightSuspended = 0;
                ResumeTrainingFight(work,KNMN_RYO,*(shen_name *)work->retained610);
                FTBL.check_dead_status(1);
            }
            knmnQteDialogue = 0;
            *previous = 0;
            *retry = 0;
            KnmnSelectPracticeCamera(work,2);
            KnmnSetTrainingCameraSequence(sequence,control->practiceCamera);
            ThrWaitFrames(sequence,100,1,10);
            break;
        case 100:
            FTBL.EV_EventSetUpHelpIcon_0(14,0);
            FTBL.shenSetTimeSimulationTo(0);
            FTBL.EV_AT_SetPos(player,0x78000000,&knmnPlayerPracticePosition);
            SetTrainingCharacterYaw(player,knmnPlayerPracticeYaw);
            FTBL.EV_AT_SetPos(dummy,0x78000000,&knmnDummyPracticePosition);
            SetTrainingCharacterYaw(dummy,knmnDummyPracticeYaw);
            FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(player),69,2,0);
            FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(player),306,0);
            ThrChangeSeq(sequence,101,1);
            break;
        case 101:
            *wait = 60;
            if (work->control.fight.fightSuspended) {
                ResumeTrainingFight(work,player,dummy);
                if (control->restartMotion == 1) motion = 409;
                else if (control->restartMotion == 2) motion = 408;
                else motion = 0;
                control->restartMotion = 0;
                FTBL.vfmDispatchControl(7,player,(void *)motion,1);
            } else {
                FTBL.shenStartFGST(10,10,0,0);
                FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(player),0,0,0,0,16);
                FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(player),0,0x8000000,1);
                FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(dummy),0,1,1,100,1);
                FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(player),0,0x10000,0);
                FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(dummy),0,0x10000,0);
                FTBL.EV_vfBattleUtil(3,4,1);
                FTBL.qteBattleUtil(0,0,0,0,0);
                FTBL.qteBattleUtil(8,0,12,24,0);
                FTBL.qteBattleUtil(9,1280,0,0,0);
                FTBL.qteBattleUtil(10,0,0,0,0);
                FTBL.qteBattleUtil(1,0,0,0,0);
                FTBL.qteBattleUtil(2,0,0,0,0);
                FTBL.EV_vfBattleUtil(1,2,1);
                FTBL.vfmQueueFightStart(2,99,1.0f,0,-1);
                FTBL.EV_vfBattleUtil(0,1,1);
                FTBL.DebugLogToFileRemoved(knmnQteStartLog);
            }

            control->practiceCamera = KnmnSelectPracticeCamera(work,2);
            KnmnSetTrainingCameraSequence(sequence,control->practiceCamera);
            ThrChangeSeq(sequence,102,1);
            break;
        case 102:
            if (((int (*)(MotionWork *,unsigned int))FTBL.GetMotionAnimationId)(FTBL.scenFindMotionwork(player),0) == 306 && !FTBL.cvsHasValidConverseData(0)) {
                FTBL.qteBattleUtil(3,0,0,0,0);
                ThrChangeSeq(sequence,103,1);
            }
            break;
        case 103:
            result = FTBL.qteBattleUtil(4,0,0,0,0);
            switch (result) {
            case 0:
                if (*wait) --*wait;
                else {
                    FTBL.DebugLogToFileRemoved(knmnQtePunchLog);
                    if (((int (*)(MotionWork *,unsigned int))FTBL.GetMotionAnimationId)(FTBL.scenFindMotionwork(player),0) == 306 && !FTBL.cvsHasValidConverseData(0)) {
                        FTBL.qteBattleUtil(3,0,0,0,0);
                        *wait = 60;
                    }
                }
                break;
            case 1:
                TrainingSetLcd(&work->control.fight.lcd.frames,knmnPracticeLcd);
                ThrChangeSeq(sequence,104,1);
                break;
            case -1:
                ThrChangeSeq(sequence,109,1);
                break;
            }
            break;
        case 104:
            KnmnClearPracticeCamera(control);
            if (work->control.fight.fightSuspended) {
                work->control.fight.fightSuspended = 0;
                ResumeTrainingFight(work,player,dummy);
                FTBL.check_dead_status(1);
            } else {
                work->control.fight.fightSuspended = 0;
                FTBL.check_dead_status(1);
            }
            ThrWaitFrames(sequence,105,1,2);
            break;
        case 105:
            FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(player),1,0);
            FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(player),69,0,0);
            ThrWaitFrames(sequence,106,1,12);
            break;
        case 106:
            if (knmnQteDialogue) TrainingKillOtherTreeHandle(&knmnQteDialogue,0);
            KnmnPrepareAuthorCharacters(work);
            FTBL.EV_authSceneCtrl(SHEN_NAME4('W','A','Z','A'));
            ThrWaitFrames(sequence,107,1,2);
            break;
        case 107:
            if (!FTBL.EV_authSceneCtrl(-1)) ThrChangeSeq(sequence,108,1);
            else KnmnWazaFrameEvents(work->game);
            break;
        case 108:
            KnmnRestoreAuthorCharacters();
            ThrWaitFrames(sequence,113,1,2);
            break;
        case 109:
            if (!*retry) {
                child = ThrMakeChild(sequence,KnmnQteFailureSequence,SHEN_NAME4('F','S','2','S'),work,1);
                ThrSetUserName(child,0,*previous);
                knmnQteDialogue = child;
                switch (*previous) {
                case 0: (*previous)++; break;
                case 1: (*previous)++; break;
                default: *previous = 0; break;
                }
                *retry = 1;
            } else --*retry;

            ThrWaitMotion(sequence,110,1,player);
            break;
        case 110:
            ThrWaitFrames(sequence,111,1,30);
            break;
        case 111:
            TrainingSuspendFight(work,player,dummy);
            QueueTrainingReposition(sequence,2,KNMN_RYO,0,&knmnPlayerPracticePosition);
            ThrChangeSeq(sequence,112,1);
            break;
        case 112:
            if (!TrainingIsRepositioning(work,0)) {
                control->restartMotion = 1;
                ThrChangeSeq(sequence,101,1);
            }
            break;
        case 113:
            KnmnClearPracticeCamera(control);
            FTBL.shenSetTimeSimulationTo(1);
            if (work->control.fight.fightSuspended) {
                work->control.fight.fightSuspended = 0;
                ResumeTrainingFight(work,player,dummy);
                FTBL.check_dead_status(1);
            } else {
                work->control.fight.fightSuspended = 0;
                FTBL.check_dead_status(1);
            }
            ThrSetReturnValue(sequence,continuation);
            ThrReturnToParent(sequence,1);
            FTBL.qteBattleUtil(5,0,0,0,0);
            ThrKillOwn(sequence);
            break;
        default:
            ThrProcess(sequence);
            break;
        }
        ThrEndStep(sequence);
    } while (ThrIsContinue(sequence));
}
#endif
