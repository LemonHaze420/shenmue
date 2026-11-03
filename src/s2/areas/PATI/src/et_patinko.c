#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/pati_event.hpp"
#if defined(SHENMUE_EVENT_ENTRY)
extern shen_name patiHandModel;
extern int g_patiVersus;
extern float g_patiModelAlpha;
void QueuePatiEvent(const char *argument);
void PatiEntry(const char *argument) {
    patiHandModel = SHEN_NAME4('h','a','n','d');
    g_patiVersus = 0;
    g_patiModelAlpha = 0;
    QueuePatiEvent(argument);
}

#else
#include "s2/areas/pati_event.hpp"
static unsigned char g_patiMuseumMode, g_patiSingleMode;
static int g_patiMinimumBet, g_patiCameraMoving;
static Vector3 g_patiCameraPosition, g_patiCameraLookAt, g_patiFilteredEye, g_patiFilteredTarget, g_patiDesiredEye, g_patiDesiredTarget;
static float g_patiCameraHeight;
static int g_patiCameraStep, g_patiSelectionAccepted, g_patiInspectRequested, g_patiHeightRepeat;
static char g_patiBoardSize;
static Vector3i g_patiBoardRotation;
static Vector3 g_patiBoardPosition;
static int g_patiPlayerCamera, g_patiDealerCamera, g_patiCameraId;
static shen_name g_patiCameraEye, g_patiCameraTarget;
static int g_patiCameraTime, g_patiDealerInputClock, g_patiDealerInputIndex;
static PatiMotionCache g_patiMotionCache[4];

extern PatiDealerMotion patiDealerMotions,patiAlternateDealerMotions;
const PatiWagerMode patiWagerModes[] = {{"1", 1}, {"5", 2}, {"10", 3}, {"20", 4}, {"50", 5}, {"100", 6}, {"", 3}};
const PatiPayoutMode patiPayoutModes[] = {{"2", 1}, {"5", 2}, {"10", 3}, {"20", 4}, {"30", 5}, {"40", 6}, {"50", 7}, {"60", 8}, {"70", 9}, {"80", 10},
    {"90", 11}, {"100", 12}, {"120", 13}, {"150", 14}, {"200", 15}, {"250", 16}, {"300", 17}, {"400", 18}, {"500", 19}, {"1000", 20}, {"5/10/20", 21},
    {"10/20/40", 22}, {"0/50/100", 23}, {"0/50/150", 24}, {"", 7}};
const PatiDealer patiDealers[] = {{SHEN_NAME4('O','D','1','_'), 1138, &patiDealerMotions, 931}, {SHEN_NAME4('D','Y','1','_'), 1135, &patiDealerMotions, 911},
    {SHEN_NAME4('S','O','G','_'), 1136, &patiDealerMotions, 931}, {SHEN_NAME4('K','N','T','_'), 1137, &patiDealerMotions, 911},
    {SHEN_NAME4('C','N','6','_'), 1168, &patiDealerMotions, 911}, {SHEN_NAME4('K','T','4','_'), 1169, &patiDealerMotions, 931},
    {SHEN_NAME4('J','O','E','_'), 1170, &patiDealerMotions, 931}, {SHEN_NAME4('S','C','N','_'), 1172, &patiDealerMotions, 931},
    {SHEN_NAME4('S','N','S','_'), 1071, &patiDealerMotions, 921}, {SHEN_NAME4('A','A','1','_'), 1159, &patiDealerMotions, 931},
    {SHEN_NAME4('H','A','K','_'), 1173, &patiDealerMotions, 911}, {SHEN_NAME4('D','H','O','_'), 1174, &patiDealerMotions, 931},
    {SHEN_NAME4('L','Y','O','_'), 1160, &patiDealerMotions, 911}, {SHEN_NAME4('A','N','S','_'), 1171, &patiDealerMotions, 931},
    {SHEN_NAME4('E','I','N','_'), 1175, &patiDealerMotions, 931}, {SHEN_NAME4('K','N','H','_'), 1176, &patiDealerMotions, 931},
    {SHEN_NAME4('R','K','U','_'), 1177, &patiDealerMotions, 921}, {SHEN_NAME4('G','K','S','_'), 1165, &patiDealerMotions, 931},
    {SHEN_NAME4('C','H','X','_'), 940, &patiDealerMotions, 921}, {SHEN_NAME4('D','M','1','_'), 941, &patiAlternateDealerMotions, 921},
    {SHEN_NAME4('D','M','2','_'), 942, &patiDealerMotions, 931}, {SHEN_NAME4('D','S','4','_'), 943, &patiDealerMotions, 931},
    {SHEN_NAME4('F','D','1','_'), 944, &patiDealerMotions, 931}, {SHEN_NAME4('J','I','8','_'), 945, &patiDealerMotions, 921},
    {SHEN_NAME4('K','U','4','_'), 946, &patiDealerMotions, 911}, {SHEN_NAME4('M','F','G','_'), 1386, &patiAlternateDealerMotions, 921},
    {SHEN_NAME4('O','D','3','_'), 947, &patiDealerMotions, 931}, {SHEN_NAME4('P','G','5','_'), 1325, &patiAlternateDealerMotions, 921},
    {SHEN_NAME4('S','D','2','_'), 948, &patiDealerMotions, 931}, {SHEN_NAME4('S','F','7','_'), 1324, &patiDealerMotions, 911},
    {SHEN_NAME4('T','N','J','_'), 949, &patiDealerMotions, 931}, {SHEN_NAME4('W','O','1','_'), 452, &patiDealerMotions, 931},
    {SHEN_NAME4('W','O','3','_'), 454, &patiDealerMotions, 931}, {SHEN_NAME4('Z','K','S','_'), 404, &patiDealerMotions, 931},
    {SHEN_NAME4('A','H','E','_'), -1, &patiDealerMotions, 921}, {SHEN_NAME4('S','Y','O','_'), -1, &patiDealerMotions, 931},
    {SHEN_NAME4('A','D','2','_'), -1, &patiDealerMotions, 931}, {SHEN_NAME4('C','H','W','_'), -1, &patiDealerMotions, 931},
    {SHEN_NAME4('T','J','N','_'), -1, &patiDealerMotions, 931}, {SHEN_NAME4('C','K','N','_'), -1, &patiDealerMotions, 931},
    {SHEN_NAME4('S','S','A','_'), -1, &patiDealerMotions, 921}, {SHEN_NAME4('S','N','N','_'), -1, &patiDealerMotions, 931},
    {SHEN_NAME4('S','Y','Y','_'), -1, &patiDealerMotions, 931}, {SHEN_NAME4('E','M','I','_'), -1, &patiDealerMotions, 921},
    {SHEN_NAME4('M','E','J','_'), -1, &patiDealerMotions, 921}, {SHEN_NAME4('T','S','Y','_'), -1, &patiDealerMotions, 921},
    {SHEN_NAME4('K','D','4','_'), -1, &patiDealerMotions, 921}, {SHEN_NAME4('S','D','7','_'), -1, &patiDealerMotions, 931},
    {SHEN_NAME4('K','K','K','_'), -1, &patiDealerMotions, 921}, {SHEN_NAME4('W','D','1','_'), -1, &patiDealerMotions, 931},
    {SHEN_NAME4('K','D','3','_'), -1, &patiDealerMotions, 921}, {SHEN_NAME4('A','W','6','_'), -1, &patiDealerMotions, 911},
    {SHEN_NAME4('I','L','N','_'), -1, &patiDealerMotions, 921}, {SHEN_NAME4('T','S','O','_'), -1, &patiDealerMotions, 921},
    {SHEN_NAME4('K','O','O','_'), -1, &patiDealerMotions, 931}, {SHEN_NAME4('K','O','W','_'), -1, &patiDealerMotions, 911},
    {SHEN_NAME4('Y','M','1','_'), -1, &patiDealerMotions, 931}, {SHEN_NAME4('Y','M','2','_'), -1, &patiDealerMotions, 931}, {0, -1, 0, 0}};
const PatiHelpTopic patiHelpTopics[] = {{0, 1, 119}, {0, 2, 120}, {0, 3, 121}, {0, 4, 122}, {0, 5, 123}, {0, 6, 124}, {0, 8, 146}, {3, 2, 125},
    {3, 5, 126}, {1, 1, 127}, {1, 2, 128}, {1, 3, 129}, {2, 1, 143}, {7, 1, 131}, {4, 1, 132}, {4, 3, 133}, {8, 1, 139}, {8, 2, 140}, {8, 3, 141}, {8, 5, 142},
    {6, 1, 119}, {255, 255, -1}};
shen_name patiHandModel = SHEN_NAME4('h','a','n','d');
int g_patiVersus = 0;
float g_patiModelAlpha = 0;
PatiDealerMotion patiDealerMotions = {6026, 6025, 6021, 6022, 6023, 6024};
PatiDealerMotion patiAlternateDealerMotions = {6019, 6018, 6017, 6022, 6023, 6024};
PatiVoicePartition patiVoicePartitions[] = {{"0104", 0, 1}, {"0104AA1", 2, 3}, {"0104AD2", 4, 5}, {"0104AHE", 6, 7}, {"0104ANS", 8, 9},
    {"0104AW6", 10, 11}, {"0104CHW", 12, 13}, {"0104CHX", 14, 15}, {"0104CKN", 16, 17}, {"0104CN6", 18, 19}, {"0104DHO", 20, 21}, {"0104DM1", 22, 23},
    {"0104DM2", 24, 25}, {"0104DS4", 26, 27}, {"0104DY1", 28, 29}, {"0104EIN", 30, 31}, {"0104EMI", 32, 33}, {"0104FD1", 34, 35}, {"0104GKS", 36, 37},
    {"0104HAK", 38, 39}, {"0104ILN", 40, 41}, {"0104JI8", 42, 43}, {"0104JOE", 44, 45}, {"0104KD3", 46, 47}, {"0104KD4", 48, 49}, {"0104KKK", 50, 51},
    {"0104KNH", 52, 53}, {"0104KNT", 54, 55}, {"0104KOO", 56, 57}, {"0104KOW", 58, 59}, {"0104KT4", 60, 61}, {"0104KU4", 62, 63}, {"0104LYO", 64, 65},
    {"0104MEJ", 66, 67}, {"0104MFG", 68, 69}, {"0104OD1", 70, 71}, {"0104PG5", 72, 73}, {"0104RKU", 74, 75}, {"0104SCN", 76, 77}, {"0104SD2", 78, 79},
    {"0104SD7", 80, 81}, {"0104SF7", 82, 83}, {"0104SNN", 84, 85}, {"0104SNS", 86, 87}, {"0104SOG", 88, 89}, {"0104SSA", 90, 91}, {"0104SYO", 92, 93},
    {"0104SYY", 94, 95}, {"0104TJN", 96, 97}, {"0104TNJ", 98, 99}, {"0104TSY", 100, 101}, {"0104WD1", 102, 103}, {"0104WO1", 104, 105}, {"0104WO3", 106, 107},
    {"0104YM1", 108, 109}, {"0104YM2", 110, 111}, {"0104ZKS", 112, 113}, {0, 0, 1}};

#include "s2/event_library_tail_dc.h"
#define CopyPatiArgument(target,source) { for(index=0;index<7;index++) { if(!((target)[index]=(source)[index]))break; } (target)[7]=0; }
#define HidePatiActiveBall(w) { if((w)->game.activeBall>=0) { FTBL.EV_DisableCharacterDisplay((w)->game.ballCharacters[(w)->game.activeBall].character); (w)->game.activeBall=-1; } }
#define PATI_TRANSFORM(target,point) { \
    (target).x=((matrix[0].x+matrix[1].x*(point).x)+matrix[2].x*(point).y)+matrix[3].x*(point).z; \
    (target).y=((matrix[0].y+matrix[1].y*(point).x)+matrix[2].y*(point).y)+matrix[3].y*(point).z; \
    (target).z=((matrix[0].z+matrix[1].z*(point).x)+matrix[2].z*(point).y)+matrix[3].z*(point).z; \
}
#define PATI_FILTER(target,source,factor) { \
    (target).x+=((source).x-(target).x)*(factor); \
    (target).y+=((source).y-(target).y)*(factor); \
    (target).z+=((source).z-(target).z)*(factor); \
}
#define PATI_CHALLENGE_MAP SHEN_NAME4('Q','1','0','0')
#define PATI_VERSUS_ARGUMENT SHEN_NAME4('P','V','S','J')
#define PATI_LOG_NAME(name) { \
    FTBL.DebugLogToFileRemoved("%c",((name)&255)); \
    FTBL.DebugLogToFileRemoved("%c",(((name)>>8)&255)); \
    FTBL.DebugLogToFileRemoved("%c",(((name)>>16)&255)); \
    FTBL.DebugLogToFileRemoved("%c",(name)>>24); \
    FTBL.DebugLogToFileRemoved("\n"); \
}
void SetPatiHelpTopic(int option,int type,int delay)
{
    int index, topic;
    int value;
    for (index=0; patiHelpTopics[index].topic!=-1; index++) {
        if (patiHelpTopics[index].option==option && patiHelpTopics[index].type==type)
            break;
    }
    value=patiHelpTopics[index].topic;
    if (value==-1) {
        do {
            FTBL.DebugLogToFileRemoved("Undefined PATI help icon!!\n");
        } while (0);
        topic=patiHelpTopics[0].topic;
    } else
        topic=value;
    FTBL.DebugLogToFileRemoved("Set PATI help icon = %i\n",topic);
    FTBL.EV_EventSetUpHelpIcon_0((unsigned char)topic,(unsigned char)delay);
}

void QueuePatiEvent(const char *argument)
{
    HLibTask *task;
    PatiEventWork *work;
    int index;
    task=FTBL.HLib_EnqueueTaskWithParameter(PatiEventCallback,5,11,sizeof(PatiEventWork),TASK_EVNT);
    work=(PatiEventWork *)FTBL.HLib_GetTaskParameter(task);
    work->parent=FTBL.HLib_GetCurrentTask();
    work->task=task;
    work->soundScene=FTBL.GetSelectedSoundSceneIndex();
    strcpy(work->sbt,FTBL.shenGetCurrentSbt());
    strcpy(work->bgm,FTBL.GetCurrentBgm());
    FTBL.GetCharaPosition(PATI_PLAYER,-1,&work->game.savedPlayerPosition,0x40000000);
    FTBL.GetScenarioCharacterEulerAngles(PATI_PLAYER,&work->game.savedPlayerRotation,0x40000000);
    work->game.parent=task;
    work->state=0;
    work->intro=1;
    work->played=0;
    work->totalBet=0;
    work->payout=0;
    work->pendingMoney=0;
    work->game.balls=0;
    work->game.wins=0;
    work->game.result=0;
    work->savedMotion=-1;
    g_patiCameraId=0;
    g_patiCameraEye=g_patiCameraTarget=0;
    g_patiCameraMoving=1;
    work->dealer=0;
    work->challengeComplete=0;
    for (index=0;index<5;index++)
        work->game.ballCharacters[index].character=0;
    FTBL.DebugLogToFileRemoved("Clear Win/Lose flag\n");
    FTBL.EV_SetPlayerFlag(12,1290,0);
    FTBL.EV_SetPlayerFlag(12,1291,0);
    EventDebugRegister(3,ResumePatiEvent,work,0x34bc0);
    EventDebugCheck(3,4);
    LockPatiEvent(work,argument);
}

void QueuePatiVoice(PatiGameWork *game,const char *code,int *frame)
{
    shen_name name;
    int index, duration;
    int byte;
    PatiFallbackVoice *fallback, *voices;
    byte=code[3];
    name=byte<<24;
    byte=code[2];
    name|=(unsigned char)byte<<16;
    byte=code[1];
    name|=(unsigned char)byte<<8;
    byte=code[0];
    name|=(unsigned char)byte;
    index=0;
    while ((byte=game->voiceLengths[index].code) && byte!=name)
        index++;
    duration=game->voiceLengths[index].frames;
    if (game->voiceLengths[index].code) {
        PatiPlayVoiceCode(game->voicePrefix,code,*frame);
        *frame+=duration+10;
        FTBL.DebugLogToFileRemoved("VL:%i\n",duration);
    } else {
        FTBL.DebugLogToFileRemoved("PATI:No VoiceID [%s%s]\n",game->voicePrefix,code);
        index=0;
        while ((patiFallbackVoices+index)->code && (patiFallbackVoices+index)->code!=name)
            index++;
        if (!strcmp("0104",game->voicePrefix) &&
            (voices=patiFallbackVoices, fallback=voices+index, fallback->code)) {
            *frame+=20;
            EVT_RepeatCall(PatiShowVoiceFallback,(void *)fallback->text,*frame,1);
            *frame+=70;
        } else
            FTBL.EventAssertNullsub(__FILE__,740);
    }
}

void PatiGreeting(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;
    unsigned short RandomInt32;

    unsigned short v7;

    unsigned short v10;

    unsigned short v13;

    unsigned short v16;

    unsigned short v19;

    unsigned short v22;

    int v25;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 0, 10.0, 1.0, 0);
    SetPatiCamera(g_patiPlayerCamera, PATI_PLAYER, PATI_PLAYER, ctx->delay);
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "A001", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "A002", &ctx->delay);
        break;
    }
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    SetPatiMotion(ctx->dealer, ctx->motions->speak, 0.0, 1.0, 0, ctx->delay);
    switch (ctx->option) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 6:
    case 8:
        switch (ctx->type) {
        case 1:
            RandomInt32 = FTBL.shenGetRandomInt32(2);

            switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "B002", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "B003", &ctx->delay);
        break;
    }
            break;
        case 2:
            v7 = FTBL.shenGetRandomInt32(2);

            switch (v7) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "B005", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "B006", &ctx->delay);
        break;
    }
            break;
        case 5:
            v10 = FTBL.shenGetRandomInt32(2);

            switch (v10) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "B008", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "B009", &ctx->delay);
        break;
    }
            break;
        case 6:
            v13 = FTBL.shenGetRandomInt32(2);

            switch (v13) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "B011", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "B012", &ctx->delay);
        break;
    }
            break;
        case 3:
        case 4:
            v16 = FTBL.shenGetRandomInt32(2);

            switch (v16) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "B014", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "B015", &ctx->delay);
        break;
    }
            break;
        default:
            FTBL.DebugLogToFileRemoved("No Serif Data\n");
            FTBL.EventAssertNullsub(__FILE__, 838);
            break;
        }
        break;
    case 7:
        v19 = FTBL.shenGetRandomInt32(2);

        switch (v19) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "C002", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "C003", &ctx->delay);
        break;
    }
        break;
    case 4:
    case 5:
        v22 = FTBL.shenGetRandomInt32(2);

        switch (v22) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "D002", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "D003", &ctx->delay);
        break;
    }
        break;
    default:
        FTBL.DebugLogToFileRemoved("No Serif Data\n");
        FTBL.EventAssertNullsub(__FILE__, 867);
        break;
    }
    switch (ctx->option) {
    case 1:
    case 2:
        QueuePatiVoice(&ctx->game, "E004", &ctx->delay);
        break;
    }
}

void PatiExplainRules(PatiEventWork *ctx) {
    int option;
    EVT_AT_SetFaceLooks(PATI_PLAYER, 0, 10, 1.0f, 0);
    EVT_AT_SetFaceLooks(ctx->dealer, 0, 10, 1.0f, 0);
    SetPatiCamera(g_patiPlayerCamera, PATI_PLAYER, PATI_PLAYER, ctx->delay);
        QueuePatiVoice(&ctx->game, "A003", &ctx->delay);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    option = ctx->option;
    switch (option) {
    case 7:
        QueuePatiVoice(&ctx->game, "C004", &ctx->delay);
        QueuePatiVoice(&ctx->game, "C005", &ctx->delay);
        break;
    case 4:
    case 5:
        if (ctx->type == 1) {
            QueuePatiVoice(&ctx->game, "D004", &ctx->delay);
            QueuePatiVoice(&ctx->game, "B017", &ctx->delay);
        } else {
            FTBL.DebugLogToFileRemoved("No Serif Data\n");
            FTBL.EventAssertNullsub(__FILE__, 910);
        }
        break;
    default:

        switch (ctx->type) {
    case 1:
        QueuePatiVoice(&ctx->game, "B016", &ctx->delay);
        QueuePatiVoice(&ctx->game, "B017", &ctx->delay);
        break;
    case 2:
        QueuePatiVoice(&ctx->game, "B018", &ctx->delay);
        QueuePatiVoice(&ctx->game, "B019", &ctx->delay);
        break;
    case 5:
        QueuePatiVoice(&ctx->game, "B018", &ctx->delay);
        QueuePatiVoice(&ctx->game, "B020", &ctx->delay);
        break;
    case 6:
        QueuePatiVoice(&ctx->game, "B018", &ctx->delay);
        QueuePatiVoice(&ctx->game, "B021", &ctx->delay);
        break;
    case 3:
        QueuePatiVoice(&ctx->game, "B018", &ctx->delay);
        QueuePatiVoice(&ctx->game, "B022", &ctx->delay);
        break;
    case 4:
        QueuePatiVoice(&ctx->game, "B023", &ctx->delay);
        QueuePatiVoice(&ctx->game, "B022", &ctx->delay);
        break;
    default:
        FTBL.DebugLogToFileRemoved("No Serif Data\n");
        FTBL.EventAssertNullsub(__FILE__, 989);
        break;
        }
        break;
    }

    switch (ctx->option) {
    case 0:
    case 6:
        QueuePatiVoice(&ctx->game, "B024", &ctx->delay);
        break;
    case 3:
        QueuePatiVoice(&ctx->game, "B029", &ctx->delay);
        QueuePatiVoice(&ctx->game, "B030", &ctx->delay);
        QueuePatiVoice(&ctx->game, "B024", &ctx->delay);
        break;
    case 8:
        QueuePatiVoice(&ctx->game, "B031", &ctx->delay);
        QueuePatiVoice(&ctx->game, "B032", &ctx->delay);
        QueuePatiVoice(&ctx->game, "B024", &ctx->delay);
        break;
    case 1:
    case 2:
        QueuePatiVoice(&ctx->game, "E021", &ctx->delay);
        QueuePatiVoice(&ctx->game, "E022", &ctx->delay);
        QueuePatiVoice(&ctx->game, "E023", &ctx->delay);
        break;
    case 4:
    case 5:
        QueuePatiVoice(&ctx->game, "D009", &ctx->delay);
        QueuePatiVoice(&ctx->game, "D010", &ctx->delay);
        QueuePatiVoice(&ctx->game, "D011", &ctx->delay);
        break;
    case 7:
        break;
    default:
        FTBL.DebugLogToFileRemoved("No Serif Data\n");
        FTBL.EventAssertNullsub(__FILE__, 1039);
        break;
    }
    if (ctx->option == 7) {
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "C007", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "C008", &ctx->delay);
        break;
    }
    } else {
    switch ((unsigned short)FTBL.shenGetRandomInt32(3)) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "B026", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "B027", &ctx->delay);
        break;
    case 2:
        QueuePatiVoice(&ctx->game, "B028", &ctx->delay);
        break;
    }
    }
}

void PatiExplainMuseumBoard(PatiEventWork *ctx) {
    EVT_AT_SetFaceLooks(PATI_PLAYER, 0, 10, 1.0f, 0);
    EVT_AT_SetFaceLooks(ctx->dealer, 0, 10, 1.0f, 0);
    SetPatiCamera(g_patiPlayerCamera, PATI_PLAYER, PATI_PLAYER, ctx->delay);
        QueuePatiVoice(&ctx->game, "A025", &ctx->delay);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    switch (ctx->option) {
    case 4:
    case 5:
    case 7:
        FTBL.DebugLogToFileRemoved("No Serif Data\n");
        FTBL.EventAssertNullsub(__FILE__, 1084);
        break;
    default:
        switch (ctx->type) {
    case 1:
        QueuePatiVoice(&ctx->game, "B097", &ctx->delay);
        QueuePatiVoice(&ctx->game, "B016", &ctx->delay);
        break;
    case 2:
        QueuePatiVoice(&ctx->game, "B098", &ctx->delay);
        QueuePatiVoice(&ctx->game, "B019", &ctx->delay);
        break;
    case 5:
        QueuePatiVoice(&ctx->game, "B099", &ctx->delay);
        QueuePatiVoice(&ctx->game, "B020", &ctx->delay);
        break;
    case 6:
        QueuePatiVoice(&ctx->game, "B100", &ctx->delay);
        QueuePatiVoice(&ctx->game, "B021", &ctx->delay);
        break;
    case 3:
        QueuePatiVoice(&ctx->game, "B101", &ctx->delay);
        QueuePatiVoice(&ctx->game, "B022", &ctx->delay);
        break;
    case 4:
        QueuePatiVoice(&ctx->game, "B102", &ctx->delay);
        QueuePatiVoice(&ctx->game, "B103", &ctx->delay);
        break;
    default:
        FTBL.DebugLogToFileRemoved("No Serif Data\n");
        FTBL.EventAssertNullsub(__FILE__, 1162);
        break;
        }
        break;
    }
    switch (ctx->option) {
    case 0:
    case 6:
        QueuePatiVoice(&ctx->game, "B024", &ctx->delay);
        break;
    case 3:
        QueuePatiVoice(&ctx->game, "B030", &ctx->delay);
        QueuePatiVoice(&ctx->game, "B024", &ctx->delay);
        break;
    case 8:
        QueuePatiVoice(&ctx->game, "B104", &ctx->delay);
        QueuePatiVoice(&ctx->game, "B105", &ctx->delay);
        QueuePatiVoice(&ctx->game, "B024", &ctx->delay);
        break;
    default:
        FTBL.DebugLogToFileRemoved("No Serif Data\n");
        FTBL.EventAssertNullsub(__FILE__, 1196);
        break;
    }
    switch ((unsigned short)FTBL.shenGetRandomInt32(3)) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "B026", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "B027", &ctx->delay);
        break;
    case 2:
        QueuePatiVoice(&ctx->game, "B028", &ctx->delay);
        break;
    }
}

void PatiCancel(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;

    unsigned short RandomInt32;
    const char *v6;
    unsigned short v7;
    const char *v8;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 3, 10.0, 1.0, 0);

    SetPatiCamera(g_patiPlayerCamera, PATI_PLAYER, PATI_PLAYER, ctx->delay);
    RandomInt32 = FTBL.shenGetRandomInt32(2);
    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "A004", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "A005", &ctx->delay);
        break;
    }
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, *&ctx->delay);
    v7 = FTBL.shenGetRandomInt32(2);
    switch (v7) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "B034", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "B035", &ctx->delay);
        break;
    }
}

void PatiAcceptPlay(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;
    unsigned short RandomInt32;
    const char *v5;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 0, 10.0, 1.0, 0);
    SetPatiCamera(g_patiPlayerCamera, PATI_PLAYER, PATI_PLAYER, ctx->delay);
    RandomInt32 = FTBL.shenGetRandomInt32(2);
    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "A006", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "A008", &ctx->delay);
        break;
    }
}

void PatiChooseBoard(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 0, 10.0, 1.0, 0);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    QueuePatiVoice(&ctx->game, "B037", &ctx->delay);
}

void PatiSelectionReminder(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;
    unsigned short RandomInt32;

    signed int start_frame;

    start_frame = 0;
    g_patiCameraTime = 99999;
    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 2, 10.0, 1.0, 0);
    RandomInt32 = FTBL.shenGetRandomInt32(3);
    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "B038", &start_frame);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "B039", &start_frame);
        break;
    case 2:
        QueuePatiVoice(&ctx->game, "B040", &start_frame);
        break;
    }
}

void PatiBoardSelected(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;
    unsigned short RandomInt32;
    const char *v5;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 0, 10.0, 1.0, 0);
    RandomInt32 = FTBL.shenGetRandomInt32(0);
    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "A009", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "A012", &ctx->delay);
        break;
    }
}

void PatiInsufficientFunds(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;
    unsigned short RandomInt32;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 0, 10.0, 1.0, 0);
    SetPatiCamera((g_patiDealerCamera + 1), ctx->dealer, ctx->dealer, ctx->delay);
    RandomInt32 = FTBL.shenGetRandomInt32(3);

    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "B044", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "B045", &ctx->delay);
        break;
    case 2:
        QueuePatiVoice(&ctx->game, "B046", &ctx->delay);
        break;
    }
}

void PatiLowerStake(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;

    unsigned short RandomInt32;

    const char *v7;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 0, 10.0, 1.0, 0);

    SetPatiCamera((g_patiDealerCamera + 1), ctx->dealer, ctx->dealer, ctx->delay);
    RandomInt32 = FTBL.shenGetRandomInt32(2);

    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "B048", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "B049", &ctx->delay);
        break;
    }
    QueuePatiVoice(&ctx->game, "B050", &ctx->delay);
}

void PatiWin(PatiEventWork *ctx) {
    shen_name Chara;

    int Type;
    shen_name v5;
    unsigned short RandomInt32;

    const char *v9;

    int v12;
    shen_name v13;
    shen_name v14;
    unsigned short v15;

    shen_name v17;
    unsigned short v18;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);

    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    FTBL.DebugLogToFileRemoved("(%i,", ctx->payout - ctx->totalBet + GetPatiPayout(ctx));
    FTBL.DebugLogToFileRemoved("%i)", 5 * ctx->bet);
    if (ctx->payout - ctx->totalBet + GetPatiPayout(ctx) >= 5 * ctx->bet) {
        EVT_AT_SetFaceLooks(ctx->dealer, 10, 10, 1.0f, 0);
        FTBL.DebugLogToFileRemoved(" Now win, and total win over 5x");
    switch ((unsigned short)FTBL.shenGetRandomInt32(3)) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "B052", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "B053", &ctx->delay);
        break;
    case 2:
        QueuePatiVoice(&ctx->game, "B056", &ctx->delay);
        break;
    }
        QueuePatiVoice(&ctx->game, "B057", &ctx->delay);
    } else if (ctx->payout - ctx->totalBet + GetPatiPayout(ctx) >= 0) {
        EVT_AT_SetFaceLooks(ctx->dealer, 3, 10, 1.0f, 0);
        FTBL.DebugLogToFileRemoved(" Now win, and total win less 5x");
    switch ((unsigned short)FTBL.shenGetRandomInt32(4)) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "B058", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "B059", &ctx->delay);
        break;
    case 2:
        QueuePatiVoice(&ctx->game, "B063", &ctx->delay);
        break;
    case 3:
        QueuePatiVoice(&ctx->game, "B064", &ctx->delay);
        break;
    }
    } else {
        EVT_AT_SetFaceLooks(ctx->dealer, 2, 10, 1.0f, 0);
        FTBL.DebugLogToFileRemoved("Now win, but  total lose");
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "B066", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "B067", &ctx->delay);
        break;
    }
    }
}

void PatiFirstDoubleWin(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;

    unsigned short RandomInt32;

    const char *v9;
    unsigned short v10;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 3, 10.0, 1.0, 0);

    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    switch ((unsigned short)FTBL.shenGetRandomInt32(6)) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "B058", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "B059", &ctx->delay);
        break;
    case 2:
        QueuePatiVoice(&ctx->game, "E044", &ctx->delay);
        break;
    case 3:
        QueuePatiVoice(&ctx->game, "E045", &ctx->delay);
        break;
    case 4:
        QueuePatiVoice(&ctx->game, "B063", &ctx->delay);
        break;
    case 5:
        QueuePatiVoice(&ctx->game, "B064", &ctx->delay);
        break;
    }
    switch ((unsigned short)FTBL.shenGetRandomInt32(3)) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "E049", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "E050", &ctx->delay);
        break;
    case 2:
        QueuePatiVoice(&ctx->game, "E051", &ctx->delay);
        break;
    }
}

void PatiDoubleWin(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;
    unsigned short RandomInt32;

    const char *v7;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 3, 10.0, 1.0, 0);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    switch ((unsigned short)FTBL.shenGetRandomInt32(6)) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "E052", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "E053", &ctx->delay);
        break;
    case 2:
        QueuePatiVoice(&ctx->game, "B059", &ctx->delay);
        break;
    case 3:
        QueuePatiVoice(&ctx->game, "E044", &ctx->delay);
        break;
    case 4:
        QueuePatiVoice(&ctx->game, "B064", &ctx->delay);
        break;
    case 5:
        QueuePatiVoice(&ctx->game, "E054", &ctx->delay);
        break;
    }
}

void PatiFinalDoubleWin(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;

    unsigned short RandomInt32;
    const char *v7;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 10, 10.0, 1.0, 0);

    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);

    QueuePatiVoice(&ctx->game, "E072", &ctx->delay);
    SetPatiCamera(g_patiPlayerCamera, PATI_PLAYER, PATI_PLAYER, *&ctx->delay);
    RandomInt32 = FTBL.shenGetRandomInt32(2);
    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "A014", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "A015", &ctx->delay);
        break;
    }
}

void PatiFirstChallengeWin(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;
    unsigned short RandomInt32;

    const char *v7;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 3, 10.0, 1.0, 0);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    switch ((unsigned short)FTBL.shenGetRandomInt32(4)) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "B058", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "B059", &ctx->delay);
        break;
    case 2:
        QueuePatiVoice(&ctx->game, "B063", &ctx->delay);
        break;
    case 3:
        QueuePatiVoice(&ctx->game, "B064", &ctx->delay);
        break;
    }
}

void PatiChallengeWin(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;
    unsigned short RandomInt32;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 3, 10.0, 1.0, 0);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    RandomInt32 = FTBL.shenGetRandomInt32(3);

    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "D041", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "D042", &ctx->delay);
        break;
    case 2:
        QueuePatiVoice(&ctx->game, "D046", &ctx->delay);
        break;
    }
}

void PatiFinalChallengeWin(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;

    unsigned short RandomInt32;

    const char *v9;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 4, 10.0, 1.0, 0);

    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    RandomInt32 = FTBL.shenGetRandomInt32(3);

    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "D052", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "D055", &ctx->delay);
        break;
    case 2:
        QueuePatiVoice(&ctx->game, "D056", &ctx->delay);
        break;
    }
    QueuePatiVoice(&ctx->game, "D058", &ctx->delay);
}

void PatiLose(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;
    int v4;
    unsigned short v5;

    unsigned short v8;

    unsigned short RandomInt32;
    const char *v12;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 3, 10.0, 1.0, 0);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    if (ctx->totalBet - ctx->payout >= 5 * ctx->bet) {
        FTBL.DebugLogToFileRemoved("Now lose, and total lose over 5x");
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "B073", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "B074", &ctx->delay);
        break;
    }
    } else if (ctx->totalBet - ctx->payout >= 0) {
        FTBL.DebugLogToFileRemoved(" Now lose, and total lose less 5x");
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "B076", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "B079", &ctx->delay);
        break;
    }
    } else {
        FTBL.DebugLogToFileRemoved("Now lose, but total win");
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "B070", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "B071", &ctx->delay);
        break;
    }
    }
}

void PatiDoubleLose(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;
    unsigned short RandomInt32;

    const char *v10;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 4, 15.0, 1.0, 0);
    EVT_AT_SetFaceLooks(ctx->dealer, 3, 15, 1.0f, 20);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    switch ((unsigned short)FTBL.shenGetRandomInt32(4)) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "E055", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "B079", &ctx->delay);
        break;
    case 2:
        QueuePatiVoice(&ctx->game, "E059", &ctx->delay);
        break;
    case 3:
        QueuePatiVoice(&ctx->game, "E060", &ctx->delay);
        break;
    }
}

void PatiFirstChallengeLose(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;
    unsigned short RandomInt32;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 3, 10.0, 1.0, 0);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    RandomInt32 = FTBL.shenGetRandomInt32(3);

    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "B073", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "B074", &ctx->delay);
        break;
    case 2:
        QueuePatiVoice(&ctx->game, "D040", &ctx->delay);
        break;
    }
}

void PatiChallengeLose(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;
    unsigned short RandomInt32;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 3, 10.0, 1.0, 0);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    RandomInt32 = FTBL.shenGetRandomInt32(3);

    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "D047", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "D048", &ctx->delay);
        break;
    case 2:
        QueuePatiVoice(&ctx->game, "D049", &ctx->delay);
        break;
    }
}

void PatiFinalChallengeLose(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;
    unsigned short RandomInt32;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 3, 10.0, 1.0, 0);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    RandomInt32 = FTBL.shenGetRandomInt32(3);

    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "D059", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "D060", &ctx->delay);
        break;
    case 2:
        QueuePatiVoice(&ctx->game, "D063", &ctx->delay);
        break;
    }
}

void PatiOfferReplay(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;
    unsigned short RandomInt32;
    const char *v5;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 3, 10.0, 1.0, 0);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    RandomInt32 = FTBL.shenGetRandomInt32(2);
    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "B080", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "B082", &ctx->delay);
        break;
    }
}

void PatiOfferDouble(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;
    unsigned short RandomInt32;
    const char *v5;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 3, 10.0, 1.0, 0);
    RandomInt32 = FTBL.shenGetRandomInt32(2);
    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "E067", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "E068", &ctx->delay);
        break;
    }
}

void PatiAcceptContinue(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;
    unsigned short RandomInt32;
    const char *v5;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 0, 10.0, 1.0, 0);
    SetPatiCamera((g_patiPlayerCamera + 1), PATI_PLAYER, PATI_PLAYER, ctx->delay);
    RandomInt32 = FTBL.shenGetRandomInt32(2);
    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "A019", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "A021", &ctx->delay);
        break;
    }
}

void PatiDeclineContinue(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;
    unsigned short RandomInt32;
    const char *v5;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 3, 10.0, 1.0, 0);
    SetPatiCamera((g_patiPlayerCamera + 1), PATI_PLAYER, PATI_PLAYER, ctx->delay);
    RandomInt32 = FTBL.shenGetRandomInt32(2);
    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "A022", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "A023", &ctx->delay);
        break;
    }
}

void PatiOfferChallenge(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;
    unsigned short RandomInt32;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 3, 10.0, 1.0, 0);
    RandomInt32 = FTBL.shenGetRandomInt32(3);

    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "D064", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "D065", &ctx->delay);
        break;
    case 2:
        QueuePatiVoice(&ctx->game, "D066", &ctx->delay);
        break;
    }
}

void PatiFinalChallengeRound(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 3, 10.0, 1.0, 0);
    QueuePatiVoice(&ctx->game, "D069", &ctx->delay);
    QueuePatiVoice(&ctx->game, "D070", &ctx->delay);
}

void PatiAfterDouble(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 3, 10.0, 1.0, 0);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    QueuePatiVoice(&ctx->game, "E065", &ctx->delay);
}

void PatiAfterChallenge(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;
    unsigned short RandomInt32;

    const char *v7;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 3, 10.0, 1.0, 0);
    SetPatiCamera((g_patiDealerCamera + 1), ctx->dealer, ctx->dealer, ctx->delay);
    switch ((unsigned short)FTBL.shenGetRandomInt32(3)) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "D071", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "D072", &ctx->delay);
        break;
    case 2:
        QueuePatiVoice(&ctx->game, "D073", &ctx->delay);
        QueuePatiVoice(&ctx->game, "D074", &ctx->delay);
        break;
    }
}

void PatiPrizeExhausted(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;
    unsigned short RandomInt32;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 12, 10.0, 1.0, 0);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    RandomInt32 = FTBL.shenGetRandomInt32(3);

    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "D075", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "D076", &ctx->delay);
        break;
    case 2:
        QueuePatiVoice(&ctx->game, "D077", &ctx->delay);
        break;
    }
}

void PatiRejectSameDoubleBoard(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;
    unsigned short RandomInt32;

    const char *v7;
    unsigned short v8;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 0, 10.0, 1.0, 0);
    RandomInt32 = FTBL.shenGetRandomInt32(2);

    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "D078", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "D079", &ctx->delay);
        break;
    }
    v8 = FTBL.shenGetRandomInt32(3);
    switch (v8) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "D081", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "E081", &ctx->delay);
        break;
    case 2:
        QueuePatiVoice(&ctx->game, "E084", &ctx->delay);
        break;
    }

}

void PatiRejectOtherChallengeBoard(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;
    unsigned short RandomInt32;

    const char *v7;
    unsigned short v8;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 0, 10.0, 1.0, 0);
    RandomInt32 = FTBL.shenGetRandomInt32(2);

    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "D078", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "D079", &ctx->delay);
        break;
    }
    v8 = FTBL.shenGetRandomInt32(3);
    switch (v8) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "D081", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "D082", &ctx->delay);
        break;
    case 2:
        QueuePatiVoice(&ctx->game, "D083", &ctx->delay);
        break;
    }

}

void PatiStuck(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;
    unsigned short RandomInt32;
    const char *v5;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 7, 10.0, 1.0, 0);
    SetPatiCamera((g_patiDealerCamera + 1), ctx->dealer, ctx->dealer, ctx->delay);
    SetPatiMotion(ctx->dealer, ctx->motions->concern, 0.0, 1.0, 0, ctx->delay);
    RandomInt32 = FTBL.shenGetRandomInt32(2);
    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "B084", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "B085", &ctx->delay);
        break;
    }
    QueuePatiVoice(&ctx->game, "B086", &ctx->delay);
    QueuePatiVoice(&ctx->game, "B087", &ctx->delay);
    SetPatiMotion(ctx->dealer, ctx->motions->speak, 0.0, 1.0, 0, ctx->delay);
}

void PatiStuckFollowup(void *data) {
    PatiGameWork **game = (PatiGameWork **)data;
    int frame = 0;
    g_patiCameraTime = 99999;
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        QueuePatiVoice(*game, "B088", &frame);
        break;
    case 1:
        QueuePatiVoice(*game, "B090", &frame);
        break;
    }
}

void PatiClosing(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;

    unsigned short RandomInt32;

    const char *v7;
    unsigned short v8;
    const char *v9;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 7, 10.0, 1.0, 0);

    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    RandomInt32 = FTBL.shenGetRandomInt32(2);

    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "B092", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "B093", &ctx->delay);
        break;
    }
    SetPatiCamera((g_patiPlayerCamera + 1), PATI_PLAYER, PATI_PLAYER, *&ctx->delay);
    v8 = FTBL.shenGetRandomInt32(2);
    switch (v8) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "A014", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "A015", &ctx->delay);
        break;
    }
}

void PatiSelectGameCount(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;

    unsigned short RandomInt32;

    const char *v7;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 0, 10.0, 1.0, 0);

    SetPatiCamera((g_patiDealerCamera + 1), ctx->dealer, ctx->dealer, ctx->delay);
    RandomInt32 = FTBL.shenGetRandomInt32(2);

    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "C027", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "C028", &ctx->delay);
        break;
    }
    QueuePatiVoice(&ctx->game, "C031", &ctx->delay);
}

void PatiPrizeResult(PatiEventWork *ctx) {
    shen_name ryo;
    shen_name npc;
    unsigned short v4;

    unsigned short v7;

    unsigned short RandomInt32;

    ryo = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(ryo, 0, 10.0, 1.0, 0);

    npc = (ctx->dealer);
    EVT_AT_SetFaceLooks(npc, 3, 10.0, 1.0, 0);

    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    switch (patiPrizes[ctx->game.prize].quantity) {
    case 2:
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "C032", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "C034", &ctx->delay);
        break;
    }
        break;
    case 1:
    switch ((unsigned short)FTBL.shenGetRandomInt32(4)) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "C036", &ctx->delay);
        QueuePatiVoice(&ctx->game, "C037", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "C038", &ctx->delay);
        break;
    }
        break;
    case 3:
    switch ((unsigned short)FTBL.shenGetRandomInt32(2)) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "C036", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "C043", &ctx->delay);
        break;
    }
        break;
    }
}

void PatiMissingDealer(PatiEventWork *ctx) {
    shen_name Chara;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    SetPatiCamera(g_patiPlayerCamera, PATI_PLAYER, PATI_PLAYER, ctx->delay);
    QueuePatiVoice(&ctx->game, "A017", &ctx->delay);
}

void PatiDealerVersusWin(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;
    unsigned short RandomInt32;
    const char *v5;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 3, 10.0, 1.0, 0);
    SetPatiCamera((g_patiDealerCamera + 2), ctx->dealer, ctx->dealer, ctx->delay);
    SetPatiMotion(ctx->dealer, 6055, 0.0, 1.0, 0, ctx->delay);
    RandomInt32 = FTBL.shenGetRandomInt32(2);
    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "F014", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "F015", &ctx->delay);
        break;
    }
}

void PatiDealerVersusLose(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;
    unsigned short RandomInt32;
    const char *v5;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 2, 10.0, 1.0, 0);
    SetPatiCamera((g_patiDealerCamera + 2), ctx->dealer, ctx->dealer, ctx->delay);
    SetPatiMotion(ctx->dealer, 6045, 0.0, 1.0, 0, ctx->delay);
    RandomInt32 = FTBL.shenGetRandomInt32(2);
    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "F016", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "F017", &ctx->delay);
        break;
    }
}

void PatiPlayerTurn(PatiEventWork *ctx) {
    shen_name Chara;
    unsigned short RandomInt32;
    const char *v4;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    SetPatiCamera(g_patiPlayerCamera, PATI_PLAYER, PATI_PLAYER, ctx->delay);
    SetPatiMotion(PATI_PLAYER, 6042, 0.0, 1.0, 0, ctx->delay);
    SetPatiMotion(ctx->dealer, 6047, 0.0, 1.0, 0, ctx->delay);
    RandomInt32 = FTBL.shenGetRandomInt32(2);
    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "A055", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "A056", &ctx->delay);
        break;
    }
}

void PatiPlayerVersusWin(PatiEventWork *ctx) {
    shen_name Chara;
    unsigned short RandomInt32;
    const char *v4;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 3, 10.0, 0.5, 0);
    SetPatiCamera((g_patiPlayerCamera + 1), PATI_PLAYER, PATI_PLAYER, ctx->delay);
    SetPatiMotion(PATI_PLAYER, 6044, 0.0, 1.0, 0, ctx->delay);
    RandomInt32 = FTBL.shenGetRandomInt32(2);
    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "A057", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "A058", &ctx->delay);
        break;
    }
}

void PatiPlayerVersusLose(PatiEventWork *ctx) {
    shen_name Chara;
    unsigned short RandomInt32;
    const char *voiceCode;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 2, 10.0, 0.5, 0);
    SetPatiCamera((g_patiPlayerCamera + 1), PATI_PLAYER, PATI_PLAYER, ctx->delay);
    SetPatiMotion(PATI_PLAYER, 6039, 0.0, 1.0, 0, ctx->delay);
    RandomInt32 = FTBL.shenGetRandomInt32(2);
    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "A059", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "A060", &ctx->delay);
        break;
    }
}

void PatiNextVersusRound(PatiEventWork *ctx) {
    shen_name Chara;
    unsigned short RandomInt32;
    const char *v4;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    SetPatiCamera((g_patiPlayerCamera + 1), PATI_PLAYER, PATI_PLAYER, ctx->delay);
    SetPatiMotion(PATI_PLAYER, 6042, 0.0, 1.0, 0, ctx->delay);
    SetPatiMotion(ctx->dealer, 6047, 0.0, 1.0, 0, ctx->delay);
    RandomInt32 = FTBL.shenGetRandomInt32(2);
    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "A061", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "A062", &ctx->delay);
        break;
    }
}

void PatiVersusSuddenDeath(PatiEventWork *ctx) {
    shen_name Chara;

    unsigned short RandomInt32;

    const char *v6;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);

    SetPatiCamera((g_patiPlayerCamera + 1), PATI_PLAYER, PATI_PLAYER, ctx->delay);
    SetPatiMotion(PATI_PLAYER, 6042, 0.0, 1.0, 0, ctx->delay);
    SetPatiMotion(ctx->dealer, 6047, 0.0, 1.0, 0, ctx->delay);
    RandomInt32 = FTBL.shenGetRandomInt32(2);

    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "A063", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "A064", &ctx->delay);
        break;
    }
    QueuePatiVoice(&ctx->game, "A065", &ctx->delay);
}

void PatiVersusWin(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;

    signed int name;
    unsigned short RandomInt32;
    const char *v7;
    unsigned short v8;
    const char *v9;
    int v10;
    signed int v11;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 3, 10.0, 0.5, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 1, 10.0, 0.5, 0);

    name = ctx->delay;
    SetPatiCamera((g_patiPlayerCamera + 1), PATI_PLAYER, PATI_PLAYER, name);
    SetPatiMotion(PATI_PLAYER, 6044, 0.0, 1.0, 0, ctx->delay);
    SetPatiMotion(ctx->dealer, 6052, 0.0, 1.0, 0, ctx->delay);
    RandomInt32 = FTBL.shenGetRandomInt32(2);
    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "A066", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "A067", &ctx->delay);
        break;
    }
    SetPatiCamera((g_patiDealerCamera + 1), ctx->dealer, ctx->dealer, *&ctx->delay);
    v8 = FTBL.shenGetRandomInt32(2);
    switch (v8) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "F018", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "F019", &ctx->delay);
        break;
    }
    if (ctx->delay - name < 300)
        name = ctx->delay;
    else
        name += 300;
    SetPatiCamera(g_patiPlayerCamera, PATI_PLAYER, PATI_PLAYER, *&ctx->delay);
    SetPatiMotion(PATI_PLAYER, 6042, 0.0, 1.0, 0, name);
    SetPatiMotion(ctx->dealer, 6047, 0.0, 1.0, 0, name);
}

void PatiVersusLose(PatiEventWork *ctx) {

    int v2;
    shen_name Chara;
    shen_name v5;
    unsigned short RandomInt32;
    const char *v7;
    unsigned short v8;
    const char *v9;
    int v10;
    signed int v11;

    v2 = ctx->delay;
    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v5 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v5, 3, 10.0, 1.0, 0);
    SetPatiCamera((g_patiPlayerCamera + 1), PATI_PLAYER, PATI_PLAYER, *&ctx->delay);
    SetPatiMotion(PATI_PLAYER, 6039, 0.0, 1.0, 0, *&ctx->delay);
    SetPatiMotion(ctx->dealer, 6054, 0.0, 1.0, 0, *&ctx->delay);
    RandomInt32 = FTBL.shenGetRandomInt32(2);
    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "A067", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "A068", &ctx->delay);
        break;
    }
    SetPatiCamera((g_patiDealerCamera + 1), ctx->dealer, ctx->dealer, *&ctx->delay);
    v8 = FTBL.shenGetRandomInt32(2);
    switch (v8) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "F020", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "F021", &ctx->delay);
        break;
    }
    if (ctx->delay - v2 < 170)
        v2 = ctx->delay;
    else
        v2 += 170;
    SetPatiMotion(PATI_PLAYER, 6042, 0.0, 1.0, 0, v2);
    SetPatiMotion(ctx->dealer, 6047, 0.0, 1.0, 0, v2);
}

void PatiMuseumIntro(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;
    unsigned short RandomInt32;
    const char *v5;
    unsigned short v6;
    const char *v7;

    FTBL.DebugLogToFileRemoved("PATI : SA_MuseumIntro\n");
    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 0, 10.0, 1.0, 0);
    SetPatiCamera(g_patiPlayerCamera, PATI_PLAYER, PATI_PLAYER, ctx->delay);
    QueuePatiVoice(&ctx->game, "A001", &ctx->delay);
    SetPatiCamera(g_patiDealerCamera, ctx->dealer, ctx->dealer, ctx->delay);
    SetPatiMotion(ctx->dealer, ctx->motions->speak, 0.0, 1.0, 0, ctx->delay);
    RandomInt32 = FTBL.shenGetRandomInt32(2);
    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "B002", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "B003", &ctx->delay);
        break;
    }
    v6 = FTBL.shenGetRandomInt32(3);
    switch (v6) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "B094", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "B095", &ctx->delay);
        break;
    case 2:
        QueuePatiVoice(&ctx->game, "B096", &ctx->delay);
        break;
    }

}

void PatiMuseumBoardSelected(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;
    unsigned short RandomInt32;

    const char *v9;
    unsigned short v10;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 0, 10.0, 1.0, 0);
    switch ((unsigned short)FTBL.shenGetRandomInt32(5)) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "A026", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "A027", &ctx->delay);
        break;
    case 2:
        QueuePatiVoice(&ctx->game, "A028", &ctx->delay);
        break;
    case 3:
        QueuePatiVoice(&ctx->game, "A029", &ctx->delay);
        break;
    case 4:
        QueuePatiVoice(&ctx->game, "A030", &ctx->delay);
        break;
    }
    switch ((unsigned short)FTBL.shenGetRandomInt32(3)) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "B106", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "B037", &ctx->delay);
        break;
    case 2:
        QueuePatiVoice(&ctx->game, "B107", &ctx->delay);
        break;
    }
}

void PatiMuseumPlay(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;
    unsigned short RandomInt32;
    const char *v5;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 0, 10.0, 1.0, 0);
    RandomInt32 = FTBL.shenGetRandomInt32(2);
    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "A006", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "A008", &ctx->delay);
        break;
    }
}

void PatiMuseumReselect(PatiEventWork *ctx) {
    shen_name Chara;
    shen_name v3;
    unsigned short RandomInt32;

    const char *v9;

    Chara = (PATI_PLAYER);
    EVT_AT_SetFaceLooks(Chara, 0, 10.0, 1.0, 0);
    v3 = (ctx->dealer);
    EVT_AT_SetFaceLooks(v3, 0, 10.0, 1.0, 0);
    switch ((unsigned short)FTBL.shenGetRandomInt32(4)) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "A031", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "A032", &ctx->delay);
        break;
    case 2:
        QueuePatiVoice(&ctx->game, "A033", &ctx->delay);
        break;
    case 3:
        QueuePatiVoice(&ctx->game, "A034", &ctx->delay);
        break;
    }
    QueuePatiVoice(&ctx->game, "B050", &ctx->delay);
}

void PatiVersusStuck(PatiEventWork *ctx) {
    SetPatiCamera(g_patiPlayerCamera, PATI_PLAYER, PATI_PLAYER, ctx->delay);
    QueuePatiVoice(&ctx->game, "A073", &ctx->delay);
}

void PatiVersusClosing(PatiEventWork *ctx) {
    unsigned short RandomInt32;
    const char *v3;

    SetPatiCamera((g_patiPlayerCamera + 1), PATI_PLAYER, PATI_PLAYER, ctx->delay);
    RandomInt32 = FTBL.shenGetRandomInt32(2);
    switch (RandomInt32) {
    case 0:
    default:
        QueuePatiVoice(&ctx->game, "A069", &ctx->delay);
        QueuePatiVoice(&ctx->game, "A070", &ctx->delay);
        break;
    case 1:
        QueuePatiVoice(&ctx->game, "A071", &ctx->delay);
        QueuePatiVoice(&ctx->game, "A070", &ctx->delay);
        break;
    }

}

void ShowPatiChoiceIcons(int first,int second,int third,int fourth,PatiEventWork *work)
{
    EVT_AT_KsIcon_Enqueue(first,second,third,fourth,10800,0xffff,2,work->delay);
}

void ShowPatiPlayerChoiceIcons(int first,int second,int third,int fourth,PatiEventWork *work)
{
    SetPatiCamera(g_patiPlayerCamera,PATI_PLAYER,PATI_PLAYER,work->delay);
    ShowPatiChoiceIcons(first,second,third,fourth,work);
}

void PlacePatiBet(PatiEventWork *work)
{
    int amount=work->bet*work->remainingGames;
    EVT_AT_StartMoneyCount(-amount,work->delay);
    work->delay+=5;
    work->totalBet+=amount;
    FTBL.DebugLogToFileRemoved("bet:%i\n",amount);
    FTBL.DebugLogToFileRemoved("bet_sum:%i\n",work->totalBet);
}

void PayPatiWinnings(PatiEventWork *work)
{
    int amount=GetPatiPayout(work);
    SetPatiCamera(g_patiPlayerCamera+1,PATI_PLAYER,PATI_PLAYER,work->delay);
    EVT_AT_StartMoneyCount(amount,work->delay);
    work->delay+=5;
    work->payout+=amount;
    FTBL.DebugLogToFileRemoved("get:%i\n",amount);
    FTBL.DebugLogToFileRemoved("get_sum:%i\n",work->payout);
}

void RefundPatiBet(PatiEventWork *work)
{
    int amount=work->bet*work->remainingGames;
    SetPatiCamera(g_patiPlayerCamera,PATI_PLAYER,PATI_PLAYER,work->delay);
    EVT_AT_StartMoneyCount(amount,work->delay);
    work->delay+=5;
    work->totalBet-=amount;
    FTBL.DebugLogToFileRemoved("back:%i\n",amount);
    FTBL.DebugLogToFileRemoved("bet_sum:%i\n",work->totalBet);
}

void SelectPatiGameCount(PatiEventWork *work)
{
    int bet=work->bet;
    int maximum=5*bet;
    FTBL.SetMoneyRelatedUIElement_0(maximum,10800,bet,15);
    work->delay+=5;
}

void ReadPatiBetResult(PatiEventWork *work)
{
    int result=FTBL.GetNumBet();
    if (result>0)
        work->remainingGames=result/work->bet;
    else
        work->remainingGames=result;
    FTBL.DebugLogToFileRemoved("RESULT = $%i ",result);
    FTBL.DebugLogToFileRemoved("(%i)\n",work->remainingGames);
}

void KeepPatiPrize(PatiEventWork *work)
{
    int item;
    unsigned int index;
    if (!patiPrizes[work->game.prize].quantity) {
        FTBL.DebugLogToFileRemoved("Error : This is not Item.\n");
        return;
    }
    item=patiPrizes[work->game.prize].item;
    for (index=0;index<work->prizeCount;index++) {
        if (work->heldPrizes[index].item==item)
            break;
    }
    if (index<work->prizeCount)
        work->heldPrizes[index].count++;
    else {
        work->heldPrizes[work->prizeCount].item=item;
        work->heldPrizes[work->prizeCount].count=1;
        work->prizeCount++;
    }
    FTBL.DebugLogToFileRemoved("keep_item:%02x:",index);
    FTBL.DebugLogToFileRemoved("%08x(",work->heldPrizes[index].item);
    FTBL.DebugLogToFileRemoved("%02x)\n",work->heldPrizes[index].count);
}

int AwardPatiPrize(PatiEventWork *work)
{
    int item, count;
    if (!work->prizeCount) {
        FTBL.DebugLogToFileRemoved("Error : Do not get Item.\n");
        return 1;
    }
    item=work->heldPrizes[work->prizeIndex].item;
    count=work->heldPrizes[work->prizeIndex].count;
    work->prizeIndex++;
    SetPatiCamera(g_patiPlayerCamera,PATI_PLAYER,PATI_PLAYER,work->delay);
    FTBL.AddInventoryItem(item,(unsigned char)count,1);
    work->delay+=5;
    FTBL.DebugLogToFileRemoved("get_item:%08x:",item);
    FTBL.DebugLogToFileRemoved("+%08x\n",count);
    if ((unsigned int)work->prizeIndex<work->prizeCount)
        return 0;
    work->prizeCount=0;
    work->prizeIndex=0;
    return 1;
}

void AccumulatePatiDoubleWinnings(PatiEventWork *work)
{
    if (!work->round) {
        work->pendingMoney+=GetPatiPayout(work);
        FTBL.DebugLogToFileRemoved("get_sub:%i\n",work->pendingMoney);
    } else {
        work->pendingMoney+=work->pendingMoney;
        FTBL.DebugLogToFileRemoved("get_sub:%i\n",work->pendingMoney);
    }
}

void PayPendingPatiWinnings(PatiEventWork *work)
{
    int amount=work->pendingMoney;
    work->pendingMoney=0;
    SetPatiCamera(g_patiPlayerCamera,PATI_PLAYER,PATI_PLAYER,work->delay);
    EVT_AT_StartMoneyCount(amount,work->delay);
    work->delay+=5;
}

int GetPatiPayout(const PatiEventWork *work)
{
    int amount;
    do {
    switch (work->payoutMode) {
    case 1: amount=2; break;
    case 2: amount=5; break;
    case 3: amount=10; break;
    case 4: amount=20; break;
    case 5: amount=30; break;
    case 6: amount=40; break;
    case 7: amount=50; break;
    case 8: amount=60; break;
    case 9: amount=70; break;
    case 10: amount=80; break;
    case 11: amount=90; break;
    case 12: amount=100; break;
    case 13: amount=120; break;
    case 14: amount=150; break;
    case 15: amount=200; break;
    case 16: amount=250; break;
    case 17: amount=300; break;
    case 18: amount=400; break;
    case 19: amount=500; break;
    case 20: amount=1000; break;
    case 21:
        switch (work->game.wins) {
        case 1:
        default: amount=5; break;
        case 2: amount=10; break;
        case 3: amount=20; break;
        }
        break;
    case 22:
        switch (work->game.wins) {
        case 1:
        default: amount=10; break;
        case 2: amount=20; break;
        case 3: amount=40; break;
        }
        break;
    case 23:
        switch (work->game.wins) {
        case 1:
        default: amount=0; break;
        case 2: amount=50; break;
        case 3: amount=100; break;
        }
        break;
    case 24:
        switch (work->game.wins) {
        case 1:
        default: amount=0; break;
        case 2: amount=50; break;
        case 3: amount=150; break;
        }
        break;
    default:
        amount=1;
        FTBL.DebugLogToFileRemoved("Unknown Get Type!!\n");
        FTBL.EventAssertNullsub(__FILE__,2743);
        break;
    }
    } while (0);
    return amount;
}

void SetPatiCamera(int camera,shen_name eye,shen_name target,int delay)
{
    shen_name eyeName[2], targetName[2];
    if (camera==g_patiCameraId && eye==g_patiCameraEye && target==g_patiCameraTarget)
        return;
    FTBL.EV_ControlPackageResources(2,4,"PATI",0,0);
    eyeName[0]=eye;
    targetName[0]=target;
    targetName[1]=0;
    eyeName[1]=0;
    FTBL.DebugLogToFileRemoved("SetCamera:%i,%s,%s\n",camera,(char *)eyeName,(char *)targetName);
    EVT_AT_SetCam(camera,eye,target,0,0,delay);
    g_patiCameraId=camera;
    g_patiCameraEye=eye;
    g_patiCameraTarget=target;
    g_patiCameraTime=delay;
}

void ResetPatiMotionCache(void)
{
    int index;
    for (index=0;index<4;index++) {
        g_patiMotionCache[index].character=0;
        g_patiMotionCache[index].age=index;
    }
}

void SetPatiMotion(shen_name character,int motion,float frame,float speed,int flags,int delay)
{
    shen_name name[2];
    int index, selected;
    name[0]=character;
    name[1]=0;
    for (index=0;index<4;index++) {
        if (g_patiMotionCache[index].character==character && g_patiMotionCache[index].motion==motion)
            break;
    }
    if (index<4) {
        g_patiMotionCache[index].age=0;
        FTBL.DebugLogToFileRemoved("PATI:SkipMot:id=%s,mot=%i,frm=%f,flg=%x\n",(char *)name,motion,frame,flags);
        for (index=0;index<4;index++) {
            if (g_patiMotionCache[index].age<10000)
                g_patiMotionCache[index].age++;
        }
    } else {
        if (delay==g_patiCameraTime)
            flags|=0x40000000;
        FTBL.DebugLogToFileRemoved("PATI:SetMot:id=%s,mot=%i,frm=%f,flg=%x\n",(char *)name,motion,frame,flags);
        EVT_AT_SetMot(character,motion,frame,speed,flags,delay);
        selected=0;
        for (index=0;index<4;index++) {
            if (g_patiMotionCache[selected].age>g_patiMotionCache[index].age)
                selected=index;
            if (!g_patiMotionCache[index].character || g_patiMotionCache[index].character==character) {
                selected=index;
                break;
            }
        }
        g_patiMotionCache[selected].character=character;
        g_patiMotionCache[selected].motion=motion;
        g_patiMotionCache[selected].age=0;
    }
    for (index=0;index<4;index++) {
        name[0]=g_patiMotionCache[index].character;
        name[1]=0;
        FTBL.DebugLogToFileRemoved("cur_mot[%i]:id=%s,mot=%i,tch=%i\n",index,(char *)name,g_patiMotionCache[index].motion,g_patiMotionCache[index].age);
    }
}

void PatiPlayVoiceCode(const char *prefix,const char *code,int delay)
{
    char voice[32];
    sprintf(voice,"%s%s",prefix,code);
    FTBL.DebugLogToFileRemoved("VoiceCode:%s\n",voice);
    EVT_AT_SetVoice(voice,delay);
}

#undef PATI_READ_NAME
#define PATI_READ_NAME(s) (((unsigned char)(s)[3]<<24)|((unsigned char)(s)[2]<<16)|((unsigned char)(s)[1]<<8)|(unsigned char)(s)[0])
void PatiEventCallback(void *data) {
    PatiEventWork *work = (PatiEventWork *)data;
    unsigned char entryState;
    unsigned int extended;
    int index;


    struct {
        unsigned char retained[12];
        Vector3i rotation;
    } inspection;
    work->delay = 0;
    g_patiCameraTime = 99999;
    if (FTBL.GetShouldRenderIcons(0))
        goto done;
    extended = FTBL.GetCurrentControlInput(0);
    entryState = work->state;
    switch (entryState) {
    case 0:
        if (!LoadPatiEvent(work))
            goto done;
        ResetPatiMotionCache();
        CopyPatiArgument(work->selectedName, work->argument);
        work->game.mode = 0;
        if (*(shen_name *)work->argument==SHEN_NAME4('P','V','S','J')) {
            FTBL.DebugLogToFileRemoved("Force Job Mode BD = [%s] domoto=[%s]\n",work->selectedName,work->dealerGroup);
            FindPatiDealerBoards(work, work->dealerGroup);
            CopyPatiArgument(work->selectedName, work->versusBoard);
            work->type = 8;
            work->option = 0;
            ConfigurePatiEventBoard(work, work->selectedName);
            SetPatiBet(work);
            work->state = 26;
            work->dealer=PATI_READ_NAME(work->opponent);
        } else if (!((*(shen_name *)work->argument & 0xffff)==('B'|('D'<<8)) && work->argument[2]>='0' && work->argument[2]<='9' && work->argument[3]>='0' && work->argument[3]<='9')) {
            work->dealer=PATI_READ_NAME(work->argument);
            FindPatiDealerBoards(work, work->argument);
            ConfigurePatiEventBoard(work, work->selectedName);
            if (g_patiMuseumMode) work->state = 1; else work->state = 14;
        } else {

            FindPatiDealerBoards(work, 0);
            ConfigurePatiEventBoard(work, work->selectedName);
            SetPatiBet(work);
            work->state = 26;
        }
        if (work->nearbyDealer!=work->dealer && work->nearbyDealer) {
            FTBL.DebugLogToFileRemoved("Unlock Near Pati-Master:");
            FTBL.DebugLogToFileRemoved("%c",(work->nearbyDealer&255));
            FTBL.DebugLogToFileRemoved("%c",((work->nearbyDealer>>8)&255));
            FTBL.DebugLogToFileRemoved("%c",((work->nearbyDealer>>16)&255));
            FTBL.DebugLogToFileRemoved("%c",work->nearbyDealer>>24);
            FTBL.DebugLogToFileRemoved("\n");
            FTBL.npcLockUnlock(work->nearbyDealer);
        }
        FTBL.adfsSetCameraIdxForChara(work->dealer, SHEN_NAME4('d','a','m','y'));
        if (FindPatiDealer(work))
            CreatePatiEventResources(work);
        else {
            FTBL.DebugLogToFileRemoved("Pati-Master not found!!\n");
            PatiMissingDealer(work);
            work->state = 61;
        }
        break;
    case 1:
        FTBL.EV_EventSetUpHelpIcon_0(144, 0);
        PositionPatiPlayer(work);
        PatiMuseumIntro(work);
        work->state = 2;
        break;
    case 2:
        FTBL.EV_DisableCharacterDisplay(PATI_PLAYER);
        if (FTBL.DoesScenarioCharacterExist(PATI_BACKPACK)==1)
            FTBL.EV_DisableCharacterDisplay(PATI_BACKPACK);
        FTBL.CheckHelpIcon(33, 0, 0);
        InitializePatiBoardCamera(work);
        work->selectionFrames = 0;

    case 3:
        SetupPatiBoard(0);
        work->state = 3 < 20 ? 4 : 22;
        break;
    case 4: {
        if (++work->selectionFrames >= 5400 || FTBL.shenGetMinutesUntilNextFrameMinute() <= 0) {
            work->state = 61;
            break;
        }
        if (work->selectionFrames == 1800 || work->selectionFrames == 3600)
            PatiSelectionReminder(work);
        index = SelectPatiBoardInput(work);
        if (index) {
            if (FTBL.DoesScenarioCharacterExist(PATI_BACKPACK)==1)
                FTBL.EV_EnableCharacterDisplay(PATI_BACKPACK);
            FTBL.CheckHelpIcon(1, 0, 0);
            if (index > 0) {
                ConfigurePatiEventBoard(work, work->selectedName);
                SetPatiBet(work);
                work->state = 4 == 4 ? 5 : 23;
            } else if (4 == 4) {
                FTBL.EV_EnableCharacterDisplay(PATI_PLAYER);
                PatiCancel(work);
                work->state = 61;
            } else {
                if (work->type != 8)
                    work->state = 13;
                else
                    work->state = 61;
            }
        } else {
            if (!g_patiSelectionAccepted && (extended & 2))
                g_patiInspectRequested = 1;
            if (!g_patiSelectionAccepted)
                FTBL.GetBattleDataConfig(0, PATI_INSPECT);
            if (g_patiInspectRequested)
                work->state = 4 == 4 ? 6 : 24;
        }
        break;
    }
    case 6: {
        g_patiInspectRequested = 0;
        g_patiCameraStep = 0;
        {
            Vector3i *rotation = &inspection.rotation;
            rotation->z = 0;
            rotation->x = 0;
            rotation->y = work->choices[work->selectedBoard].rotation.y;
            FTBL.AT_SetLocalBaseChara(PATI_CAMERA, 0xffffffffU);
            FTBL.EV_AT_SetPos(PATI_CAMERA, 0x78000000, &work->choices[work->selectedBoard].position);
            FTBL.EV_AT_SetAngY_0(PATI_CAMERA, 0x78000000, rotation, 0);
            SetPatiCamera(979, PATI_CAMERA, PATI_CAMERA, 0);
            work->state = 7;
            FTBL.CheckHelpIcon(1, 0, 0);
        }
    }
    case 7:
        if (++work->selectionFrames >= 5400 || FTBL.shenGetMinutesUntilNextFrameMinute() <= 0) {
            work->state = 61;
            break;
        }
        if (work->selectionFrames == 1800 || work->selectionFrames == 3600)
            PatiSelectionReminder(work);
        if (FTBL.GetBattleDataConfig(2, PATI_ACCEPT))
            g_patiSelectionAccepted = 1;
        if (!g_patiSelectionAccepted && !(extended & 2)) {
            g_patiCameraStep = 8;
            work->state = 7 < 20 ? 4 : 22;
            FTBL.camCtrlGetNextCamPos(&g_patiCameraPosition);
            FTBL.camCtrlGetTargetPosition(&g_patiCameraLookAt);
            FTBL.CheckHelpIcon(33, 0, 0);
        }
        if (g_patiCameraStep >= 30) {
            if (g_patiSelectionAccepted) {
            g_patiSelectionAccepted = 0;
            g_patiInspectRequested = 0;
            g_patiCameraStep = -15;
            if (FTBL.DoesScenarioCharacterExist(PATI_BACKPACK)==1)
                FTBL.EV_EnableCharacterDisplay(PATI_BACKPACK);
            FTBL.CheckHelpIcon(1, 0, 0);
            ConfigurePatiEventBoard(work, work->selectedName);
            SetPatiBet(work);
            work->state = 7 < 20 ? 5 : 23;
            }
        } else
            ++g_patiCameraStep;
        if (!g_patiSelectionAccepted) {
            FTBL.GetBattleDataConfig(0, PATI_ACCEPT);
            FTBL.GetBattleDataConfig(0, PATI_INSPECT);
        }
        break;
    case 5:
        PatiMuseumBoardSelected(work);
        ConfigurePatiEventBoard(work, work->selectedName);
        work->state = 8;
        break;
    case 8:
        PositionPatiPlayer(work);
        work->state = 9;
        break;
    case 9:
        FTBL.EV_DisableCharacterDisplay(PATI_PLAYER);
        SetupPatiBoard(2);
        ShowPatiChoiceIcons(402,36,369,34,work);
        work->state = 10;
        break;
    case 10:
        if (FTBL.shenGetMinutesUntilNextFrameMinute() <= 0) {
        FTBL.EV_EnableCharacterDisplay(PATI_PLAYER);
        PatiCancel(work);
        work->state = 61;
            break;
        }
        switch (FTBL.shenGetCtrlSpeakIconFlag()) {
        case 402:
            PatiMuseumReselect(work);
            work->state = 2;
            break;
        case 36:
            work->state = 12;
            break;
        case 369:
            work->state = 11;
            break;
        case 34:
        default:
        FTBL.EV_EnableCharacterDisplay(PATI_PLAYER);
        PatiCancel(work);
        work->state = 61;
            break;
        }
        break;
    case 11:
        FTBL.EV_EnableCharacterDisplay(PATI_PLAYER);
        PatiExplainMuseumBoard(work);
        work->state = 9;
        break;
    case 12:
        work->intro = 0;
        PatiMuseumPlay(work);
        work->state = 26;
        break;
    case 13:
        if (g_patiMuseumMode)
            work->state = 9;
        else {
            FindPatiDealerBoards(work, work->argument);
            ConfigurePatiEventBoard(work, work->selectedName);
            work->state = 15;
        }
        break;
    case 14:
        PositionPatiPlayer(work);
        PatiGreeting(work);
        work->state = 15;
        break;
    case 15:
        if (work->type!=8) {
            FTBL.EV_EnableCharacterDisplay(PATI_PLAYER);
            ShowPatiPlayerChoiceIcons(0xffff,36,369,34,work);
            work->state=16;
        } else {
            FTBL.EventAssertNullsub(__FILE__,3142);
            work->state=20;
        }
        break;
    case 16:
        if (FTBL.shenGetMinutesUntilNextFrameMinute() <= 0) {
        PatiCancel(work);
        work->state = 61;
            break;
        }
        switch (FTBL.shenGetCtrlSpeakIconFlag()) {
        case 36:
            work->state = 18;
            break;
        case 369:
            work->state = 17;
            break;
        case 34:
        default:
        PatiCancel(work);
        work->state = 61;
            break;
        }
        break;
    case 17:
        PatiExplainRules(work);
        work->state = 13;
        break;
    case 18:
        if (g_patiSingleMode) {
            ConfigurePatiEventBoard(work, work->selectedName);
            SetPatiBet(work);
            work->intro = 0;
            work->state = 26;
        } else {
            PatiAcceptPlay(work);
            PatiChooseBoard(work);
            work->state = 20;
        }
        break;
    case 19:
        FindPatiDealerBoards(work, work->argument);
        work->state = 20;
        break;
    case 20:
        FTBL.EV_DisableCharacterDisplay(PATI_PLAYER);
        if (FTBL.DoesScenarioCharacterExist(PATI_BACKPACK)==1)
            FTBL.EV_DisableCharacterDisplay(PATI_BACKPACK);
        FTBL.CheckHelpIcon(33, 0, 0);
        InitializePatiBoardCamera(work);
        work->selectionFrames = 0;

    case 21:
        SetupPatiBoard(0);
        work->state = 21 < 20 ? 4 : 22;
        break;
    case 22: {
        if (++work->selectionFrames >= 5400 || FTBL.shenGetMinutesUntilNextFrameMinute() <= 0) {
            work->state = 61;
            break;
        }
        if (work->selectionFrames == 1800 || work->selectionFrames == 3600)
            PatiSelectionReminder(work);
        index = SelectPatiBoardInput(work);
        if (index) {
            if (FTBL.DoesScenarioCharacterExist(PATI_BACKPACK)==1)
                FTBL.EV_EnableCharacterDisplay(PATI_BACKPACK);
            FTBL.CheckHelpIcon(1, 0, 0);
            if (index > 0) {
                ConfigurePatiEventBoard(work, work->selectedName);
                SetPatiBet(work);
                work->state = 22 == 4 ? 5 : 23;
            } else if (22 == 4) {
                FTBL.EV_EnableCharacterDisplay(PATI_PLAYER);
                PatiCancel(work);
                work->state = 61;
            } else {
                if (work->type != 8)
                    work->state = 13;
                else
                    work->state = 61;
            }
        } else {
            if (!g_patiSelectionAccepted && (extended & 2))
                g_patiInspectRequested = 1;
            if (!g_patiSelectionAccepted)
                FTBL.GetBattleDataConfig(0, PATI_INSPECT);
            if (g_patiInspectRequested)
                work->state = 22 == 4 ? 6 : 24;
        }
        break;
    }
    case 24: {
        g_patiInspectRequested = 0;
        {
            Vector3i *rotation = &inspection.rotation;
            rotation->z = 0;
            rotation->x = 0;
            rotation->y = work->choices[work->selectedBoard].rotation.y;
            FTBL.AT_SetLocalBaseChara(PATI_CAMERA, 0xffffffffU);
            FTBL.EV_AT_SetPos(PATI_CAMERA, 0x78000000, &work->choices[work->selectedBoard].position);
            FTBL.EV_AT_SetAngY_0(PATI_CAMERA, 0x78000000, rotation, 0);
            SetPatiCamera(979, PATI_CAMERA, PATI_CAMERA, 0);
            work->state = 25;
            FTBL.CheckHelpIcon(1, 0, 0);
        }
    }
    case 25:
        if (++work->selectionFrames >= 5400 || FTBL.shenGetMinutesUntilNextFrameMinute() <= 0) {
            work->state = 61;
            break;
        }
        if (work->selectionFrames == 1800 || work->selectionFrames == 3600)
            PatiSelectionReminder(work);
        if (FTBL.GetBattleDataConfig(2, PATI_ACCEPT))
            g_patiSelectionAccepted = 1;
        if (!g_patiSelectionAccepted && !(extended & 2)) {
            g_patiCameraStep = 8;
            work->state = 25 < 20 ? 4 : 22;
            FTBL.camCtrlGetNextCamPos(&g_patiCameraPosition);
            FTBL.camCtrlGetTargetPosition(&g_patiCameraLookAt);
            FTBL.CheckHelpIcon(33, 0, 0);
        }
        if (g_patiCameraStep >= 30) {
            if (g_patiSelectionAccepted) {
            g_patiSelectionAccepted = 0;
            g_patiInspectRequested = 0;
            g_patiCameraStep = -15;
            if (FTBL.DoesScenarioCharacterExist(PATI_BACKPACK)==1)
                FTBL.EV_EnableCharacterDisplay(PATI_BACKPACK);
            FTBL.CheckHelpIcon(1, 0, 0);
            ConfigurePatiEventBoard(work, work->selectedName);
            SetPatiBet(work);
            work->state = 25 < 20 ? 5 : 23;
            }
        } else
            ++g_patiCameraStep;
        if (!g_patiSelectionAccepted) {
            FTBL.GetBattleDataConfig(0, PATI_ACCEPT);
            FTBL.GetBattleDataConfig(0, PATI_INSPECT);
        }
        break;
    case 23:
        work->intro = 0;
        PatiBoardSelected(work);
        work->state = 26;
        break;
    case 26:
        CopyPatiArgument(work->previousBoard, work->selectedName);
        if (work->type == 8) {
            FTBL.EV_EnableCharacterDisplay(PATI_PLAYER);
            work->state = 64;
        } else {
            CommitPatiBoardSelection(work);
            work->state = 27;
        }
        break;
    case 27:
        switch (work->option) {
        case 1:
        case 2:
            FTBL.DebugLogToFileRemoved("DoubleUp:place_opt_bd = %s ",work->excludedBoard);
            FTBL.DebugLogToFileRemoved(": bd_idx = %s\n",work->board);
            if (!strncmp(work->excludedBoard,work->board,4)) {
                PatiRejectSameDoubleBoard(work);
                work->state=19;
            }
            break;
        case 4:
        case 5:
            FTBL.DebugLogToFileRemoved("Challenge:place_opt_bd = %s ",work->excludedBoard);
            FTBL.DebugLogToFileRemoved(": bd_idx = %s\n",work->board);
            if (strncmp(work->excludedBoard,work->board,4)) {
                PatiRejectOtherChallengeBoard(work);
                work->state=19;
            }
            break;
        }
        if (work->state==27)
            work->state=28;
        break;
    case 28:
        if (!CanAffordPatiBet(work)) {
            FTBL.EV_EnableCharacterDisplay(PATI_PLAYER);
            work->state = 60;
            break;
        }

    case 29:
        work->round = 0;
        work->prizeCount = 0;
        work->prizeIndex = 0;
        work->remainingGames = 1;
        if (work->option == 7)
            work->state = 31;
        else
            work->state = 30;
        break;
    case 31:
        FTBL.EV_EventSetUpHelpIcon_0(147, (unsigned char)(work->delay));
        PatiSelectGameCount(work);
        work->state = 32;
        break;
    case 32:
        SetupPatiBoard(2);
        SelectPatiGameCount(work);
        work->state = 33;
        break;
    case 33: {
        SetPatiHelpTopic(work->option, work->type, 0);
        ReadPatiBetResult(work);
        if (work->remainingGames > 0)
            work->state = 30;
        else if (work->remainingGames == -2) {
            if (FTBL.shenGetMinutesUntilNextFrameMinute() > 0) {
                InitializePatiBoardCamera(work);
                work->state = 22;
            } else
                work->state = 61;
        }
        break;
    }
    case 30:
        SetupPatiBoard(2);
        FTBL.EV_DisableCharacterDisplay(PATI_PLAYER);
        PlacePatiBet(work);
        work->state = 34;
        break;
    case 34:
        FTBL.EV_DisableCharacterDisplay(PATI_PLAYER);
        work->game.balls = 0;
        work->game.wins = 0;

    case 35:
        work->game.result = 0;
        patiHandModel = (int)(SHEN_NAME4('h','a','n','d'));
        QueuePatiBall(&work->game);
        FTBL.HLib_SetupTaskDelay(0);
        work->state = 36;
        break;
    case 36:
        if (work->game.result == -4) {
            FTBL.DebugLogToFileRemoved("Cancel (>_<)\n");
            HidePatiActiveBall(work);
            PatiCancel(work);
            work->state = 37;
        } else if (work->game.result == 4) {
            FTBL.DebugLogToFileRemoved("TimeOut (-_-)zZ\n");
            HidePatiActiveBall(work);
            PatiClosing(work);
            work->state = 37;
        } else if (work->game.result == -1) {
            FTBL.DebugLogToFileRemoved("Stuck (-_-;)\n");
            HidePatiActiveBall(work);
            PatiStuck(work);
            work->state = 35;
        } else {
            int flag;
            for (index=0;patiDealers[index].character;index++) {
                if (patiDealers[index].character==work->dealer)
                    break;
            }
            flag=patiDealers[index].playedFlag;
            if (flag>=0) {
                FTBL.DebugLogToFileRemoved("Set HINT_M flag  FreeTalkFlag[%i] = ON\n",flag);
                FTBL.EV_SetPlayerFlag(12,flag,1);
                if (work->dealer==SHEN_NAME4('S','F','7','_')) {
                    FTBL.DebugLogToFileRemoved("Set MG_OTOSIDAMA flag = ON\n");
                    FTBL.SetMGOTOSIDAMA(7,1);
                }
            } else
                FTBL.DebugLogToFileRemoved("Can't set HINT_M flag:pati domoto_list search error!\n");
            work->played = 1;
            if (HasPatiWon(work)) {
                FTBL.DebugLogToFileRemoved("Set Win Flag V(^-^)V\n");
                FTBL.EV_SetPlayerFlag(12, 1290, 1);
                work->state = 38;
            } else {
                FTBL.DebugLogToFileRemoved("Set Lose Flag(;_;)\n");
                FTBL.EV_SetPlayerFlag(12, 1291, 1);
                work->state = 39;
            }
        }
        break;
    case 37:
        RefundPatiBet(work);
        work->state = 61;
        break;
    case 38:
        HidePatiActiveBall(work);
        switch (work->option) {
        case 0:
        case 3:
        case 6:
        case 8:
            PatiWin(work);
            work->state = 40;
            break;
        case 1:
        case 2:
            switch (work->round) {
            case 0:
                PatiFirstDoubleWin(work);
                work->state = 42;
                break;
            default:
                PatiDoubleWin(work);
                work->state = 42;
                break;
            case 5:
                PatiFinalDoubleWin(work);
                work->state = 47;
                break;
            }
            AccumulatePatiDoubleWinnings(work);
            break;
        case 7:
            KeepPatiPrize(work);
            PatiPrizeResult(work);
            work->state = 49;
            break;
        case 4:
        case 5:
            KeepPatiPrize(work);
            if (!work->round) {
                PatiFirstChallengeWin(work);
                work->state = 53;
            } else if (!((work->option == 4 && work->round >= 2) || work->round >= 4)) {
                PatiChallengeWin(work);
                work->state = 53;
            } else {
                (FTBL.EV_GetPlayerFlag(11, 1721) == 1 || FTBL.EV_GetPlayerFlag(11, 1722) == 1
                    ? PatiFinalChallengeWin : PatiPrizeExhausted)(work);
                work->state = 50;
            }
            break;
        default:
            FTBL.DebugLogToFileRemoved("Unknown Game Option!!\n");
            work->state = 61;
            break;
        }
        break;
    case 39:
        HidePatiActiveBall(work);
        work->pendingMoney = 0;
        switch (work->option) {
        case 0:
        case 3:
        case 6:
        case 7:
        case 8:
            PatiLose(work);
            work->state = 41;
            break;
        case 1:
        case 2:
            if (!work->round)
                PatiLose(work);
            else
                PatiDoubleLose(work);
            work->state = 48;
            break;
        case 4:
        case 5:
            if (!work->round) {
                PatiFirstChallengeLose(work);
                if (FTBL.shenGetMinutesUntilNextFrameMinute() > 0) {
                    PatiAfterDouble(work);
                    work->state = 13;
                } else {
                    PatiClosing(work);
                    work->state = 61;
                }
            } else {
                if (!((work->option == 4 && work->round >= 2) || work->round >= 4))
                    PatiChallengeLose(work);
                else
                    PatiFinalChallengeLose(work);
                if (FTBL.shenGetMinutesUntilNextFrameMinute() > 0) {
                    PatiAfterDouble(work);
                    work->state = 13;
                } else {
                    PatiClosing(work);
                    work->state = 61;
                }
            }
            break;
        default:
            FTBL.DebugLogToFileRemoved("Unknown Game Option!!\n");
            work->state = 61;
            break;
        }
        break;
    case 40:
        PayPatiWinnings(work);
        work->state = 41;
        break;
    case 41:
        if (FTBL.shenGetMinutesUntilNextFrameMinute() > 0) {
            PatiOfferReplay(work);
            work->state = 13;
        } else {
            PatiClosing(work);
            work->state = 61;
        }
        break;
    case 42:
        ConfigurePatiEventBoard(work, work->nextBoard);
        InitializePatiBoardCamera(work);
        SetupPatiBoard(2);
        PatiOfferDouble(work);
        work->state = 43;
        break;
    case 43:
        ShowPatiChoiceIcons(0xffff,74,75,0xffff,work);
        work->state = 43 == 43 ? 44 : 55;
        break;
    case 44:
        if (FTBL.shenGetCtrlSpeakIconFlag() == 74)
            work->state = 45;
        else
            work->state = 46;
        break;
    case 45:
        PatiAcceptContinue(work);
        ++work->round;
        work->state = 34;
        break;
    case 46:
        PatiDeclineContinue(work);
        work->state = 47;
        break;
    case 47: {
        PayPendingPatiWinnings(work);
        if (work->round == 5)
            work->state = 61;
        else
            work->state = 48;
        break;
    }
    case 48:
        PatiAfterDouble(work);
        work->state = 13;
        break;
    case 49:
        if (AwardPatiPrize(work)) {
            --work->remainingGames;
            FTBL.DebugLogToFileRemoved("PlayRemain = %i\n",work->remainingGames);
            if (work->remainingGames > 0)
                work->state = 34;
            else if (FTBL.shenGetMinutesUntilNextFrameMinute() > 0)
                work->state = 41;
            else {
                PatiClosing(work);
                work->state = 37;
            }
        }
        break;
    case 50:
        work->state = 51;
        break;
    case 51:
        if (AwardPatiPrize(work))
            work->state = 52;
        break;
    case 52:
        work->challengeComplete = 1;
        work->state = 61;
        break;
    case 53:
        ConfigurePatiEventBoard(work, work->nextBoard);
        InitializePatiBoardCamera(work);
        SetupPatiBoard(2);
        PatiOfferChallenge(work);
        work->state = 54;
        break;
    case 54:
        ShowPatiChoiceIcons(0xffff,74,75,0xffff,work);
        work->state = 54 == 43 ? 44 : 55;
        break;
    case 55:
        if (FTBL.shenGetCtrlSpeakIconFlag() == 74)
            work->state = 56;
        else
            work->state = 57;
        break;
    case 56:
        if (!((work->option == 4 && work->round >= 1) || work->round >= 3))
            PatiAcceptContinue(work);
        else {
            PatiAcceptContinue(work);
            PatiFinalChallengeRound(work);
        }
        ++work->round;
        work->state = 34;
        break;
    case 57:
        PatiDeclineContinue(work);
        work->state = 58;
        break;
    case 58:
        if (AwardPatiPrize(work))
            work->state = 59;
        break;
    case 59:
        if (FTBL.shenGetMinutesUntilNextFrameMinute() > 0) {
            PatiAfterChallenge(work);
            work->state = 13;
        } else {
            PatiClosing(work);
            work->state = 61;
        }
        break;
    case 60:
        if ((int)(FTBL.GetPlayerStat(2)) >= g_patiMinimumBet) {
            PatiLowerStake(work);
            work->state = 13;
        } else {
            PatiInsufficientFunds(work);
            work->state = 61;
        }
        break;
    case 61:
        BeginPatiExit(work);
        work->state = 62;
        break;
    case 62:
        if (RestorePatiEvent(work))
            work->state = 63;
        break;
    case 63:
        FreePatiEvent(work);
        break;
    case 64:
        g_patiVersus = 1;
        work->totalRounds = 0;
        work->playerWins = 0;
        work->dealerWins = 0;
        work->game.targetX = 0;
        work->game.targetVariation = 10000;
        work->state = 68;
        break;
    case 68:
        work->game.ballLimit = 1;
        work->game.winLimit = 1;
        work->game.missLimit = 1;
        work->game.balls = 0;
        work->game.wins = 0;
        SetupPatiBoard(2);

    case 69:
        FTBL.EV_DisableCharacterDisplay(work->dealer);
        work->game.mode = 2;
        work->game.result = 0;
        patiHandModel = (int)(PATI_OPPONENT_HAND);
        QueuePatiBall(&work->game);
        FTBL.HLib_SetupTaskDelay(0);
        work->state = 70;
        break;
    case 70:
        FTBL.EV_EnableCharacterDisplay(work->dealer);
        if (work->game.result == 4) {
            FTBL.DebugLogToFileRemoved("Timeout (-_-)zZ\n");
            HidePatiActiveBall(work);
            PatiVersusClosing(work);
            work->state = 61;
        } else if (work->game.result == -1) {
            FTBL.DebugLogToFileRemoved("Stuck (-_-;)\n");
            HidePatiActiveBall(work);
            PatiVersusStuck(work);
            work->state = 69;
        } else {
            HidePatiActiveBall(work);
            if (work->game.wins > 0) {
                PatiDealerVersusWin(work);
                ++work->dealerWins;
            } else
                PatiDealerVersusLose(work);
            FTBL.DebugLogToFileRemoved("Game:Total=%i+0.5",work->totalRounds);
            FTBL.DebugLogToFileRemoved("NPC=%i,",work->dealerWins);
            FTBL.DebugLogToFileRemoved("RYO=%iX\n",work->playerWins);
            switch (work->totalRounds) {
            case 0:
            case 1: break;
            case 2: {
                if (work->dealerWins - work->playerWins >= 2) {
                    FTBL.DebugLogToFileRemoved("Set Lose Flag (;_;)\n");
                    FTBL.EV_SetPlayerFlag(12, 1291, 1);
                    work->state = 73;
                } else if (work->playerWins > work->dealerWins) {
                    FTBL.DebugLogToFileRemoved("Set Win Flag V(^-^)V\n");
                    FTBL.EV_SetPlayerFlag(12, 1290, 1);
                    work->state = 72;
                }
            }
                break;
            }
            if (work->state == 70) {
                PatiPlayerTurn(work);
                work->state = 65;
            }
        }
        break;
    case 65:
        SetupPatiBoard(2);
        work->game.ballLimit = 1;
        work->game.winLimit = 1;
        work->game.missLimit = 1;
        work->game.balls = 0;
        work->game.wins = 0;

    case 66:
        FTBL.EV_DisableCharacterDisplay(work->dealer);
        work->game.mode = 1;
        work->game.result = 0;
        patiHandModel = (int)(SHEN_NAME4('h','a','n','d'));
        QueuePatiBall(&work->game);
        FTBL.HLib_SetupTaskDelay(0);
        work->state = 67;
        break;
    case 67:
        FTBL.EV_EnableCharacterDisplay(work->dealer);
        if (work->game.result==-4) {
            FTBL.DebugLogToFileRemoved("Cancel (>_<)\n");
            HidePatiActiveBall(work);
            PatiVersusClosing(work);
            work->state=61;
        } else if (work->game.result==4) {
            FTBL.DebugLogToFileRemoved("Timeout (-_-)zZ\n");
            HidePatiActiveBall(work);
            PatiVersusClosing(work);
            work->state=61;
        } else if (work->game.result==-1) {
            FTBL.DebugLogToFileRemoved("Stuck (-_-;)\n");
            HidePatiActiveBall(work);
            PatiVersusStuck(work);
            work->state=66;
        } else {
            HidePatiActiveBall(work);
            if (work->game.wins>0) {
                PatiPlayerVersusWin(work);
                work->playerWins++;
            } else PatiPlayerVersusLose(work);
            work->totalRounds++;
            work->state=71;
            FTBL.DebugLogToFileRemoved("Game:Total=%i",work->totalRounds);
            FTBL.DebugLogToFileRemoved("NPC=%i,",work->dealerWins);
            FTBL.DebugLogToFileRemoved("RYO=%i\n",work->playerWins);
            switch (work->totalRounds) {
            case 1: break;
            case 2:
                if (work->playerWins==2 && !work->dealerWins) {
                    FTBL.DebugLogToFileRemoved("Set Win Flag V(^-^)V\n");
                    FTBL.EV_SetPlayerFlag(12,1290,1);
                    work->state=72;
                }
                if (!work->playerWins && work->dealerWins==2) {
                    FTBL.DebugLogToFileRemoved("Set Lose Flag (;_;)\n");
                    FTBL.EV_SetPlayerFlag(12,1291,1);
                    work->state=73;
                }
                break;
            default:
                if (work->playerWins>work->dealerWins) {
                    FTBL.DebugLogToFileRemoved("Set Win Flag V(^-^)V\n");
                    FTBL.EV_SetPlayerFlag(12,1290,1);
                    work->state=72;
                }
                if (work->playerWins<work->dealerWins) {
                    FTBL.DebugLogToFileRemoved("Set Lose Flag (;_;)\n");
                    FTBL.EV_SetPlayerFlag(12,1291,1);
                    work->state=73;
                }
                break;
            }
        }
        break;
    case 71:
        if (FTBL.shenGetMinutesUntilNextFrameMinute() <= 0) {
            PatiVersusClosing(work);
            work->state = 61;
            break;
        } else {
            if (work->totalRounds == 3)
                PatiVersusSuddenDeath(work);
            else
                PatiNextVersusRound(work);
            work->state = 68;
        }
        if (work->totalRounds >= 3 && FTBL.shenGetMinutesUntilNextFrameMinute() <= 0) {
            PatiVersusClosing(work);
            work->state = 61;
        }
        break;
    case 72:
        PatiVersusWin(work);
        work->state = 74;
        break;
    case 74: {
        unsigned int cash,amount,target;
        amount = (unsigned int)work->bet;
        if (74 == 75)
            SetPatiCamera(g_patiPlayerCamera,PATI_PLAYER,PATI_PLAYER,work->delay);
        cash = (unsigned int)(FTBL.GetPlayerStat(2));
        FTBL.AnimateMoneyAmount((int)(cash), (int)(target=cash+amount), 0);
        FTBL.SetPlayerStat(2, target);
        work->delay += 5;
        work->state = 61;
        break;
    }
    case 73:
        PatiVersusLose(work);
        work->state = 75;
        break;
    case 75: {
        unsigned int cash,amount,target;
        amount = -(unsigned int)work->bet;
        if (75 == 75)
            SetPatiCamera(g_patiPlayerCamera,PATI_PLAYER,PATI_PLAYER,work->delay);
        cash = (unsigned int)(FTBL.GetPlayerStat(2));
        FTBL.AnimateMoneyAmount((int)(cash), (int)(target=cash+amount), 0);
        FTBL.SetPlayerStat(2, target);
        work->delay += 5;
        work->state = 61;
        break;
    }
    default:
        FTBL.DebugLogToFileRemoved("Illegal State!!\n");
        FTBL.EventAssertNullsub(__FILE__,4108);
        break;
    }
    if (entryState!=work->state)
        FTBL.DebugLogToFileRemoved("State = %i \n",work->state);
    if (work->delay > 0)
        FTBL.HLib_SetupTaskDelay(work->delay);
done:
    ;
}

void CreatePatiEventResources(PatiEventWork *work)
{
    Vector3 player;
    int index,partition,bytes;
    const void *source;
    FTBL.GetCharaPosition(PATI_PLAYER,-1,&player,0x40000000);
    work->savedMotion=FTBL.CaptureMotionSnapshot(work->dealer);
    for (index=0;patiDealers[index].character;index++) {
        if (patiDealers[index].character==work->dealer)
            break;
    }
    g_patiPlayerCamera=901;
    g_patiDealerCamera=911;
    if (patiDealers[index].motions) {
        work->motions=patiDealers[index].motions;
        g_patiDealerCamera=patiDealers[index].camera;
    } else
        work->motions=&patiDealerMotions;
    sprintf(work->game.voicePrefix,"0104%c%c%c",work->dealer&255,(work->dealer>>8)&255,(work->dealer>>16)&255);
    for (index=0;patiVoicePartitions[index].name;index++) {
        if (!strcmp(patiVoicePartitions[index].name,work->game.voicePrefix))
            break;
    }
    if (!patiVoicePartitions[index].name) {
        FTBL.DebugLogToFileRemoved("NoVoiceFile:%s!!\n",work->game.voicePrefix);
        FTBL.EventAssertNullsub(__FILE__,4158);
        sprintf(work->game.voicePrefix,"0104");
    }
    FTBL.DebugLogToFileRemoved("PATI:New-Load [PATI]afs\n");
    partition=(int)FTBL.EV_ControlPackageResources(1,0,FTBL.Filepath_Gen(7),(int)"PATI.afs",0);
    FTBL.DebugLogToFileRemoved("EV_PAKLOAD_AFS [PATI] %i\n",partition);
    if (partition==-1)
        FTBL.EventAssertNullsub(__FILE__,4166);
    FTBL.DebugLogToFileRemoved("EV_PAKLOAD_AFS [PATI] %i,%i,%i\n",partition,patiVoicePartitions[index].text,patiVoicePartitions[index].sound);
    FTBL.EV_ControlPackageResources(0,8,(char *)partition,patiVoicePartitions[index].text,patiVoicePartitions[index].sound);
    FTBL.EV_ControlPackageResources(0,7,0,0,0);
    FTBL.EV_ControlPackageResources(2,5,(char *)partition,patiVoicePartitions[index].text,patiVoicePartitions[index].sound);
    source=FTBL.LoadCurrentPakBinary(work->game.voicePrefix);
    bytes=FTBL.GetCurrentPakBinarySize(work->game.voicePrefix)/4*4;
    FTBL.DebugLogToFileRemoved("EV_PAK [PATI] %08x,%08x,%08x->",partition,source,bytes);
    work->game.voiceLengths=(PatiVoiceLength *)FTBL.HLib_AllocateTaskData(bytes,PATI_AREA);
    FTBL.DebugLogToFileRemoved("%08x\n",work->game.voiceLengths);
    memcpy(work->game.voiceLengths,source,bytes);
    FTBL.DebugLogToFileRemoved("EV_FREE_PAKFILE_AFS [PATI] %i,%i,%i\n",partition,patiVoicePartitions[index].text,patiVoicePartitions[index].sound);
    FTBL.EV_ControlPackageResources(0,12,(char *)partition,patiVoicePartitions[index].text,patiVoicePartitions[index].sound);
    FTBL.DebugLogToFileRemoved("EV_UNMOUNT_AFS_PARTITION [PATI].afs %i\n",partition);
    FTBL.EV_ControlPackageResources(1,1,(char *)partition,0,0);
    FTBL.DebugLogToFileRemoved("PATI_VOICE_NAME:%s\n",work->game.voicePrefix);
    FTBL.EV_FreeAuthorPak(1,work->game.voicePrefix);
    FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(PATI_PLAYER),0,0);
    FTBL.UpdateMotionWorkWithPosition(FTBL.scenFindMotionwork(work->dealer),0,0);
    FTBL.EV_ControlPackageResources(2,2,"PATI",0,0);
    FTBL.EV_ControlPackageResources(2,4,"PATI",0,0);
    FTBL.EV_SetCharacterAlphaEnabled(work->dealer,0);
    FTBL.SetRefBoardParamsForCharaById(PATI_PLAYER,1);
    FTBL.SetRefBoardParamsForCharaById(work->dealer,1);
    FTBL.GetCharacterFaceCacheIndex(FTBL.ctrlGetCharaByName(PATI_PLAYER));
    FTBL.GetCharacterFaceCacheIndex(FTBL.ctrlGetCharaByName(work->dealer));
    EVT_AT_SetFaceLooks(PATI_PLAYER,0,10,1.0f,0);
    EVT_AT_SetFaceLooks(work->dealer,0,10,1.0f,0);
}

void CommitPatiBoardSelection(PatiEventWork *work)
{
}

void PositionPatiPlayer(PatiEventWork *work)
{
    Vector3 player,dealer,position;
    Vector3i playerRotation,dealerRotation;
    float sine,cosine;
    work->dealer=FindPatiDealer(work);
    FTBL.GetCharaPosition(PATI_PLAYER,-1,&player,0x40000000);
    FTBL.GetCharaPosition(work->dealer,-1,&dealer,0x40000000);
    FTBL.GetScenarioCharacterEulerAngles(PATI_PLAYER,&playerRotation,0x40000000);
    FTBL.GetScenarioCharacterEulerAngles(work->dealer,&dealerRotation,0x40000000);
    dealerRotation.y+=0x8000;
    sine=FTBL.CalculateFixedSine(dealerRotation.y);
    cosine=FTBL.CalculateFixedCosine(dealerRotation.y);
    position.x=player.x=cosine*0.0f+sine*1.8f+dealer.x;
    position.z=player.z=cosine*1.8f-sine*0.0f+dealer.z;
    position.y=player.y+1.5f;
    playerRotation.y=dealerRotation.y;
    EVT_AT_SetPosAng(PATI_PLAYER,-1,position.x,position.z,(float)playerRotation.y/182.04445f,0);
}

void ResetPatiNailModelState(void)
{
    if (patiDetachedNailNode)
        ((PatiPlacementNode *)patiDetachedNailNode->parent)->child=(int)patiDetachedNailNode;
    patiDetachedNailNode=0;
}

void AttachPatiNails(PatiEventWork *work,shen_name board,const char *nailFile)
{
    PatiPlacementNode *placement,*transform,*pin;
    int index, nail; int count=512;
    FTBL.DebugLogToFileRemoved("set_nail(%x,",work);
    FTBL.DebugLogToFileRemoved("%c",(unsigned char)board);
    FTBL.DebugLogToFileRemoved("%c",(unsigned char)(board>>8));
    FTBL.DebugLogToFileRemoved("%c",(unsigned char)(board>>16));
    FTBL.DebugLogToFileRemoved("%c,",board>>24);
    FTBL.DebugLogToFileRemoved("%s)\n",nailFile);
    ResetPatiNailModelState();
    placement=work->nailPlacements;
    transform=(PatiPlacementNode *)((PatiModelCharacter *)FTBL.ctrlGetCharaByName(board))->model->child;
    for (index=0;index<count;index++) InitializePatiPlacementNode(&placement[index]);
    patiDetachedNailNode=(PatiPlacementNode *)transform->child;
    placement[0].parent=(int)transform;
    transform->child=(int)placement;
    for (index=1;index<count;index++) {
        placement[index].parent=(int)(&placement[index]-1);
        ((PatiPlacementNode *)placement[index].parent)->sibling=(int)&placement[index];
    }
    pin=((PatiModelCharacter *)FTBL.ctrlGetCharaByName(PATI_PIN))->model;
    FTBL.EV_DisableCharacterDisplay(PATI_PIN);
    InitializePatiNails(&work->game.nails);
    ParsePatiNailFile(&work->game.nails,nailFile);
    for (nail=0;nail<512;nail++) {
        if (work->game.nails.nails[nail].position.y>0.0f) {
            placement->mesh=pin->mesh;
            placement->position.x=work->game.nails.nails[nail].position.x;
            placement->position.y=work->game.nails.nails[nail].position.y;
            placement->position.z=work->game.nails.nails[nail].position.z;
            placement->scale.x=work->game.nails.nails[nail].scale.x;
            placement->scale.y=work->game.nails.nails[nail].scale.y;
            placement->scale.z=work->game.nails.nails[nail].scale.z;
            placement++;
        }
        work->game.nails.nails[nail].scale.x=work->scale*1.5f;
        work->game.nails.nails[nail].scale.y=work->scale*1.5f;
        work->game.nails.nails[nail].scale.z=work->scale;
        work->game.nails.nails[nail].position.x*=work->scale;
        work->game.nails.nails[nail].position.y*=work->scale;
        work->game.nails.nails[nail].position.z*=work->scale;
    }
    (placement-1)->sibling=0;
}

void InitializePatiBoardCamera(PatiEventWork *work)
{
    g_patiCameraHeight=0.0f;
    FTBL.camCtrlGetNextCamPos(&g_patiCameraPosition);
    FTBL.camCtrlGetTargetPosition(&g_patiCameraLookAt);
    FTBL.camCtrlGetNextCamPos(&g_patiFilteredEye);
    FTBL.camCtrlGetTargetPosition(&g_patiFilteredTarget);
    UpdatePatiBoardCamera(work);
}

void SelectPatiBoard(PatiEventWork *work,const char *name)
{
    int index;
    for (index=0;work->choices[index].character;index++) {
        if (work->choices[index].character==((name[3]<<24)|((unsigned char)name[2]<<16)|((unsigned char)name[1]<<8)|(unsigned char)name[0]))
            break;
    }
    if (work->choices[index].character) work->selectedBoard=index;
    else {
        FTBL.DebugLogToFileRemoved("Not found board in this domoto group [%s]\n",name);
        FTBL.EventAssertNullsub(__FILE__,4378);
        work->selectedBoard=0;
    }
    FTBL.DebugLogToFileRemoved("Change Board [%s] = %i\n",name,work->selectedBoard);
    InitializePatiBoardCamera(work);
}

void UpdatePatiBoardCamera(PatiEventWork *work)
{
    g_patiBoardRotation.x=work->choices[work->selectedBoard].rotation.x;
    g_patiBoardRotation.y=work->choices[work->selectedBoard].rotation.y;
    g_patiBoardRotation.z=work->choices[work->selectedBoard].rotation.z;
    g_patiBoardPosition.x=work->choices[work->selectedBoard].position.x;
    g_patiBoardPosition.y=work->choices[work->selectedBoard].position.y;
    g_patiBoardPosition.z=work->choices[work->selectedBoard].position.z;
    g_patiBoardSize=work->choices[work->selectedBoard].size;
}

void SetupPatiBoardSize(int mode)
{
    Vector3 matrix[4],eye,target,height,distantEye,distantTarget,closeEye,closeTarget,delta,current;
    float cosine,sine,inputX,inputY,zoom,top,skew,maximum,minimum,middle,left,right,localX,localY,distance,fraction;
    unsigned int buttons;
    int index;
    float currentDistance;
    cosine=FTBL.CalculateFixedCosine(g_patiBoardRotation.x);
    sine=FTBL.CalculateFixedSine(g_patiBoardRotation.x);
    height.x=0.0f; height.y=cosine; height.z=sine;
    cosine=FTBL.CalculateFixedCosine(g_patiBoardRotation.y);
    sine=FTBL.CalculateFixedSine(g_patiBoardRotation.y);
    matrix[0].x=g_patiBoardPosition.x; matrix[0].y=g_patiBoardPosition.y; matrix[0].z=g_patiBoardPosition.z;
    matrix[1].x=cosine; matrix[1].y=0.0f; matrix[1].z=-sine;
    matrix[2].x=0.0f; matrix[2].y=1.0f; matrix[2].z=0.0f;
    matrix[3].x=sine; matrix[3].y=0.0f; matrix[3].z=cosine;
    sine*=height.z; cosine*=height.z;
    height.x=sine; height.z=cosine;
    switch (g_patiBoardSize) {
    case 'X':
        eye.x=0.0f; eye.y=12.4497f; eye.z=16.297201f;
        target.x=0.0f; target.y=6.3852301f; target.z=-4.7069101f;
            break;
    case 'L':
        eye.x=0.0f; eye.y=1.5369999f; eye.z=2.0120001f;
        target.x=0.0f; target.y=0.78829998f; target.z=-0.58109999f;
            break;
    default:
        eye.x=0.0f; eye.y=0.88620001f; eye.z=1.1691999f;
        target.x=0.0f; target.y=0.38080001f; target.z=-0.58109999f;
            break;
    }
    PATI_TRANSFORM(distantEye,eye);
    PATI_TRANSFORM(distantTarget,target);
    switch (g_patiBoardSize) {
    case 'X':
        eye.x=0.0f; eye.y=1.14615f; eye.z=0.57348001f;
        target.x=0.0f; target.y=-3.7462502f; target.z=-7.4746804f;
            break;
    case 'L':
        eye.x=0.0f; eye.y=0.1415f; eye.z=0.070799999f;
        target.x=0.0f; target.y=-0.46250001f; target.z=-0.9228f;
            break;
    default:
        eye.x=0.0f; eye.y=0.1415f; eye.z=0.2608f;
        target.x=0.0f; target.y=-0.46250001f; target.z=-0.9228f;
            break;
    }
    inputX=(float)(unsigned char)FTBL.GetCurrentControlInput(5);
    inputY=(float)(unsigned char)FTBL.GetCurrentControlInput(6);
    switch ((unsigned int)mode) {
    case 0:
    default:
        if (FTBL.GetNormalTriggerMapping())
            zoom=(float)(unsigned char)FTBL.GetCurrentControlInput(4);
        else
            zoom=(float)(unsigned char)FTBL.GetCurrentControlInput(3);
        break;
    case 1: zoom=255.0f; break;
    case 2: zoom=0.0f; break;
    }
    inputX/=255.0f; inputY/=255.0f; zoom/=255.0f;
    switch (g_patiBoardSize) {
    case 'S':
        top=0.81f; skew=0.0099999905f; maximum=0.75999999f; minimum=0.20999999f; middle=0.39499998f; left=-0.15000001f; right=0.15000000596f; break;
    case 'M':
        top=1.01f; skew=0.0099999905f; maximum=0.96000004f; minimum=0.20999999f; middle=0.495f; left=-0.15000001f; right=0.15000000596f; break;
    case 'L':
    default:
        top=1.8099999f; skew=-0.040000021f; maximum=1.76f; minimum=0.25999999f; middle=0.89499998f; left=-0.15000001f; right=0.15000000596f;
        inputY=(inputY-0.5f)*0.5f+0.5f; break;
    case 'X':
        top=14.661f; skew=-0.32400018f; maximum=14.2560005f; minimum=2.1059999f; middle=7.2495003f; left=-1.2150002f; right=1.2150002f; break;
    }
    if (middle+g_patiCameraHeight>maximum+0.006f) g_patiCameraHeight=maximum-middle;
    if (middle+g_patiCameraHeight<minimum-0.006f) g_patiCameraHeight=minimum-middle;
    localX=left*0.5f+right*0.5f; localY=top*0.5f+skew*0.5f;
    closeEye.x=((matrix[0].x+matrix[1].x*(localX+eye.x))+matrix[2].x*(localY+eye.y))+matrix[3].x*eye.z;
    closeEye.y=((matrix[0].y+matrix[1].y*(localX+eye.x))+matrix[2].y*(localY+eye.y))+matrix[3].y*eye.z;
    closeEye.z=((matrix[0].z+matrix[1].z*(localX+eye.x))+matrix[2].z*(localY+eye.y))+matrix[3].z*eye.z;
    localX=left*(1.0f-inputX)+right*inputX; localY=top*(1.0f-inputY)+skew*inputY;
    closeTarget.x=((matrix[0].x+matrix[1].x*(localX+target.x))+matrix[2].x*(localY+target.y))+matrix[3].x*target.z;
    closeTarget.y=((matrix[0].y+matrix[1].y*(localX+target.x))+matrix[2].y*(localY+target.y))+matrix[3].y*target.z;
    closeTarget.z=((matrix[0].z+matrix[1].z*(localX+target.x))+matrix[2].z*(localY+target.y))+matrix[3].z*target.z;
    buttons=FTBL.GetCurrentControlInput(2);
    if (buttons&0x3000) g_patiHeightRepeat++; else g_patiHeightRepeat=0;
    for (index=0;index<=(g_patiHeightRepeat>44 ? 9 : g_patiHeightRepeat/5);index++) {
        if (buttons&0x1000) {
            if (maximum>middle+g_patiCameraHeight) g_patiCameraHeight+=0.005f;
        } else if ((buttons&0x2000) && middle+g_patiCameraHeight>minimum) g_patiCameraHeight-=0.005f;
    }
    closeEye.x+=height.x*g_patiCameraHeight; closeEye.y+=height.y*g_patiCameraHeight; closeEye.z+=height.z*g_patiCameraHeight;
    closeTarget.x+=height.x*g_patiCameraHeight; closeTarget.y+=height.y*g_patiCameraHeight; closeTarget.z+=height.z*g_patiCameraHeight;
    g_patiDesiredEye.x=distantEye.x*(1.0f-zoom)+closeEye.x*zoom;
    g_patiDesiredEye.y=distantEye.y*(1.0f-zoom)+closeEye.y*zoom;
    g_patiDesiredEye.z=distantEye.z*(1.0f-zoom)+closeEye.z*zoom;
    g_patiDesiredTarget.x=distantTarget.x*(1.0f-zoom)+closeTarget.x*zoom;
    g_patiDesiredTarget.y=distantTarget.y*(1.0f-zoom)+closeTarget.y*zoom;
    g_patiDesiredTarget.z=distantTarget.z*(1.0f-zoom)+closeTarget.z*zoom;
    if (g_patiCameraStep<=7) {
        PATI_FILTER(g_patiFilteredEye,g_patiDesiredEye,0.25f);
        PATI_FILTER(g_patiFilteredTarget,g_patiDesiredTarget,0.25f);
    } else {
        PATI_FILTER(g_patiFilteredEye,g_patiDesiredEye,1.0f);
        PATI_FILTER(g_patiFilteredTarget,g_patiDesiredTarget,1.0f);
    }
    FTBL.camCtrlSetState(26);
    g_patiCameraId=0; g_patiCameraEye=0; g_patiCameraTarget=0;
    if (g_patiCameraStep>0) {
        g_patiCameraPosition.x+=(g_patiFilteredEye.x-g_patiCameraPosition.x)/(float)g_patiCameraStep;
        g_patiCameraPosition.y+=(g_patiFilteredEye.y-g_patiCameraPosition.y)/(float)g_patiCameraStep;
        g_patiCameraPosition.z+=(g_patiFilteredEye.z-g_patiCameraPosition.z)/(float)g_patiCameraStep;
        g_patiCameraLookAt.x+=(g_patiFilteredTarget.x-g_patiCameraLookAt.x)/(float)g_patiCameraStep;
        g_patiCameraLookAt.y+=(g_patiFilteredTarget.y-g_patiCameraLookAt.y)/(float)g_patiCameraStep;
        g_patiCameraLookAt.z+=(g_patiFilteredTarget.z-g_patiCameraLookAt.z)/(float)g_patiCameraStep;
    } else {
        PATI_FILTER(g_patiCameraPosition,g_patiFilteredEye,1.0f);
        PATI_FILTER(g_patiCameraLookAt,g_patiFilteredTarget,1.0f);
    }
    FTBL.camCtrlSetNextPosition(&g_patiCameraPosition);
    FTBL.camCtrlSetTargetPosition(&g_patiCameraLookAt);
    FTBL.camCtrlSetPerspective(10922);
    delta.x=distantEye.x-closeEye.x; delta.y=distantEye.y-closeEye.y; delta.z=distantEye.z-closeEye.z;
    current.x=distantEye.x-g_patiCameraPosition.x; current.y=distantEye.y-g_patiCameraPosition.y; current.z=distantEye.z-g_patiCameraPosition.z;
    distance=FTBL.GetVector3DotProduct(&delta,&delta);
    currentDistance=FTBL.GetVector3DotProduct(&current,&current);
    if (distance<=0.00001f) distance=0.0001f;
    fraction=currentDistance/distance;
    delta.x=g_patiFilteredEye.x-g_patiCameraPosition.x; delta.y=g_patiFilteredEye.y-g_patiCameraPosition.y; delta.z=g_patiFilteredEye.z-g_patiCameraPosition.z;
    distance=FTBL.GetVector3DotProduct(&delta,&delta);
    if (distance<0.001f) g_patiCameraMoving=0; else g_patiCameraMoving=1;
    if (fraction<=0.0f) fraction=0.0f;
    if (fraction>=1.0f) fraction=1.0f;
    fraction*=fraction;
    if (fraction>0.5f) fraction=0.5f;
    g_patiModelAlpha+=(fraction-g_patiModelAlpha)*0.5f;
    FTBL.SetCharacterAlpha(FTBL.ctrlGetCharaByName(patiHandModel),g_patiModelAlpha);
}

void SetupPatiBoard(int mode)
{
    SetupPatiBoardSize(mode);
    g_patiCameraPosition.x=g_patiFilteredEye.x=g_patiDesiredEye.x;
    g_patiCameraPosition.y=g_patiFilteredEye.y=g_patiDesiredEye.y;
    g_patiCameraPosition.z=g_patiFilteredEye.z=g_patiDesiredEye.z;
    g_patiCameraLookAt.x=g_patiFilteredTarget.x=g_patiDesiredTarget.x;
    g_patiCameraLookAt.y=g_patiFilteredTarget.y=g_patiDesiredTarget.y;
    g_patiCameraLookAt.z=g_patiFilteredTarget.z=g_patiDesiredTarget.z;
    SetupPatiBoardSize(mode);
}

#undef PATI_READ_NAME
#define PATI_READ_NAME(s) (((s)[3]<<24)|((unsigned char)(s)[2]<<16)|((unsigned char)(s)[1]<<8)|(unsigned char)(s)[0])
void FindPatiDealerBoards(PatiEventWork *work,const char *dealer)
{
    char area[8],group[8];
    Vector3 player;
    int count=0,tableIndex=0,boardIndex,nearest=9999,hasOption=0,hasType=0,doubleBoard,challengeBoard,copyIndex,index;
    const char *selectedType=0,*selectedOption=0;
    float nearestDistance,distance;
    g_patiMuseumMode=0;
    g_patiSingleMode=0;
    g_patiMinimumBet=100000;
    FTBL.GetCharaPosition(PATI_PLAYER,-1,&player,0x40000000);
    for (copyIndex=0;copyIndex<4;copyIndex++) area[copyIndex]=g_currentMap>>(copyIndex*8);
    area[4]=0;
    for (copyIndex=0;copyIndex<4;copyIndex++) group[copyIndex]=dealer[copyIndex];
    group[4]=0;
    nearestDistance=100000000.0f;
    FTBL.DebugLogToFileRemoved("Search DM=%s:",area);
    FTBL.DebugLogToFileRemoved("%s\n",group);
    for (;work->tables.tables[tableIndex].area[0];tableIndex++) {
        if (strcmp(area,work->tables.tables[tableIndex].area) || strcmp(group,work->tables.tables[tableIndex].npc)) continue;
        for (boardIndex=work->tables.tables[tableIndex].firstBoard;
             boardIndex<work->tables.tables[tableIndex].firstBoard+work->tables.tables[tableIndex].boardCount;boardIndex++) {
            for (copyIndex=0;copyIndex<=4;copyIndex++) work->choices[count].character=PATI_READ_NAME(work->tables.boards[boardIndex].character);
            FTBL.GetCharaPosition(work->choices[count].character,-1,&work->choices[count].position,0x40000000);
            FTBL.GetScenarioCharacterEulerAngles(work->choices[count].character,&work->choices[count].rotation,0x40000000);
            if (work->choices[count].rotation.z>0x7fff) {
                work->choices[count].rotation.x+=0x8000;
                work->choices[count].rotation.y=(unsigned short)(int)((540.0f-(float)work->choices[count].rotation.y/182.04445f)*8192.0f/45.0f);
                work->choices[count].rotation.z-=0x8000;
            }
            work->choices[count].size=work->tables.boards[boardIndex].size[0];
            work->choices[count].nailFile=work->tables.boards[boardIndex].model;
            distance=FTBL.GetSquaredEuclideanDistance(&player,&work->choices[count].position);
            for (index=0;patiGameTypes[index].name[0];index++) {
                if (!strcmp(patiGameTypes[index].name,work->tables.tables[tableIndex].gameType)) break;
            }
            if (!patiGameTypes[index].name[0]) {
                FTBL.DebugLogToFileRemoved("Error:Undefined Game Type.\n");
                FTBL.EventAssertNullsub(__FILE__,4746);
            }
            work->choices[count].type=patiGameTypes[index].mode;
            for (index=0;patiGameOptions[index].name[0];index++) {
                if (!strcmp(patiGameOptions[index].name,work->tables.tables[tableIndex].gameOptions)) break;
            }
            if (!patiGameOptions[index].name[0]) {
                FTBL.DebugLogToFileRemoved("Error:Undefined Game Option.\n");
                FTBL.EventAssertNullsub(__FILE__,4755);
            }
            work->choices[count].option=patiGameOptions[index].mode;
            for (index=0;patiWagerModes[index].name[0];index++) {
                if (!strcmp(patiWagerModes[index].name,work->tables.tables[tableIndex].stake)) break;
            }
            if (!patiWagerModes[index].name[0]) {
                FTBL.DebugLogToFileRemoved("Error:Undefined Bet Type.\n");
                FTBL.EventAssertNullsub(__FILE__,4766);
            }
            work->wagerMode=patiWagerModes[index].mode;
            SetPatiBet(work);
            if (g_patiMinimumBet>work->bet) g_patiMinimumBet=work->bet;
            doubleBoard=!strcmp(work->tables.tables[tableIndex].gameOptions,"DBUP") && !strcmp(work->tables.boards[boardIndex].character,work->tables.tables[tableIndex].selectedBoard);
            challengeBoard=0;
            if ((!strcmp(work->tables.tables[tableIndex].gameOptions,"CAR3") || !strcmp(work->tables.tables[tableIndex].gameOptions,"CAR5")) &&
                strcmp(work->tables.boards[boardIndex].character,work->tables.tables[tableIndex].selectedBoard)) challengeBoard=1;
            if (hasOption && strcmp(selectedOption,work->tables.tables[tableIndex].gameOptions)) g_patiMuseumMode=1;
            if (!doubleBoard && !challengeBoard) {
                if (nearestDistance>distance) {nearestDistance=distance;nearest=count;}
                if (hasType && strcmp(selectedType,work->tables.tables[tableIndex].gameType)) g_patiMuseumMode=1;
                if (nearest==count || !hasOption) {selectedOption=work->tables.tables[tableIndex].gameOptions;hasOption=1;}
                if (nearest==count || !hasType) {selectedType=work->tables.tables[tableIndex].gameType;hasType=1;}
            }
            FTBL.DebugLogToFileRemoved("bd:%s ",work->tables.boards[boardIndex].character);
            FTBL.DebugLogToFileRemoved("(% f,",work->choices[count].position.x);
            FTBL.DebugLogToFileRemoved("% f,",work->choices[count].position.y);
            FTBL.DebugLogToFileRemoved("% f)-",work->choices[count].position.z);
            FTBL.DebugLogToFileRemoved("[% f,",(float)work->choices[count].rotation.x/182.04445f);
            FTBL.DebugLogToFileRemoved("% f,",(float)work->choices[count].rotation.y/182.04445f);
            FTBL.DebugLogToFileRemoved("% f]\n",(float)work->choices[count].rotation.z/182.04445f);
            count++;
            if (count>19) {
                FTBL.DebugLogToFileRemoved("Too much boards controled by domoto [%s]\n",group);
                FTBL.EventAssertNullsub(__FILE__,4815);
            }
        }
    }
    if (!count) {
        FTBL.DebugLogToFileRemoved("No boards controled by domoto [%s]\n",group);
        FTBL.EventAssertNullsub(__FILE__,4825);
    }
    work->choices[count].character=0;
    if (count==1) g_patiSingleMode=1;
    FTBL.DebugLogToFileRemoved("Museum Mode: ");
    if (g_patiMuseumMode) FTBL.DebugLogToFileRemoved("Enable\n"); else FTBL.DebugLogToFileRemoved("Disable\n");
    FTBL.DebugLogToFileRemoved("Single Mode: ");
    if (g_patiSingleMode) FTBL.DebugLogToFileRemoved("Enable\n"); else FTBL.DebugLogToFileRemoved("Disable\n");
    work->selectedBoard=nearest;
    AttachPatiNails(work,work->choices[nearest].character,work->choices[nearest].nailFile);
    for (copyIndex=0;copyIndex<4;copyIndex++) work->selectedName[copyIndex]=work->choices[work->selectedBoard].character>>(copyIndex*8);
    work->selectedName[4]=0;
    g_patiCameraStep=8;
    g_patiSelectionAccepted=0;
    g_patiInspectRequested=0;
    g_patiCameraMoving=1;
}

int SelectPatiBoardInput(PatiEventWork *work)
{
    Vector3 direction,delta,origin;
    int left,right,previous,index;
    unsigned int input;
    float rightDistance=100000000.0f,leftDistance=100000000.0f,distance;
    if (!g_patiCameraMoving && !g_patiSelectionAccepted && !g_patiInspectRequested) {
        input=FTBL.GetCurrentControlInput(2);
        FTBL.GetCurrentControlInput(0);
    } else {
        input=0;
        FTBL.GetCurrentControlInput(0);
    }
    previous=right=left=work->selectedBoard;
    origin.x=work->choices[previous].position.x;
    origin.y=work->choices[previous].position.y;
    origin.z=work->choices[previous].position.z;
    direction.x=FTBL.CalculateFixedCosine(work->choices[previous].rotation.y);
    direction.y=0.0f;
    direction.z=FTBL.CalculateFixedSine(work->choices[work->selectedBoard].rotation.y)*-1.0f;
    for (index=0;work->choices[index].character;index++) {
        if (index==work->selectedBoard) continue;
        distance=FTBL.GetSquaredEuclideanDistance(&origin,&work->choices[index].position);
        delta.x=work->choices[index].position.x-origin.x;
        delta.y=work->choices[index].position.y-origin.y;
        delta.z=work->choices[index].position.z-origin.z;
        if (FTBL.GetVector3DotProduct(&delta,&direction)>0.0f) {
            if (rightDistance>distance) {rightDistance=distance;right=index;}
        } else if (leftDistance>distance) {leftDistance=distance;left=index;}
    }
    if (!(input&0x3000) && (input&0xc000)) {
        if (input&0x8000) {
            work->selectedBoard=right;
            FTBL.DebugLogToFileRemoved("BD_SEL-R:%i\n",right);
        } else if (input&0x4000) {
            work->selectedBoard=left;
            FTBL.DebugLogToFileRemoved("BD_SEL-L:%i\n",left);
        }
        if (work->selectedBoard!=previous) g_patiCameraStep=8;
    }
    UpdatePatiBoardCamera(work);
    SetupPatiBoardSize(0);
    if (g_patiCameraStep==4) {
        AttachPatiNails(work,work->choices[work->selectedBoard].character,work->choices[work->selectedBoard].nailFile);
        if (work->option==1 && work->choices[work->selectedBoard].character==PATI_READ_NAME(work->nextBoard))
            SetPatiHelpTopic(2,(signed char)work->choices[work->selectedBoard].type,0);
        else SetPatiHelpTopic(work->choices[work->selectedBoard].option,(signed char)work->choices[work->selectedBoard].type,0);
    }
    for (index=0;index<4;index++) work->selectedName[index]=work->choices[work->selectedBoard].character>>(index*8);
    work->selectedName[4]=0;
    if (FTBL.GetBattleDataConfig(2,PATI_CANCEL)) return -1;
    if (FTBL.GetBattleDataConfig(2,PATI_ACCEPT)) g_patiSelectionAccepted=1;
    else if (!g_patiSelectionAccepted) {
        FTBL.GetBattleDataConfig(0,PATI_ACCEPT);
        FTBL.GetBattleDataConfig(0,PATI_CANCEL);
    }
    if (g_patiCameraStep>-15) { --g_patiCameraStep; return 0; }
    if (g_patiCameraMoving) return 0;
    if (!g_patiSelectionAccepted) return 0;
    {
        g_patiSelectionAccepted=0;
        g_patiInspectRequested=0;
        FTBL.DebugLogToFileRemoved("Result:%s\n",work->selectedName[4]);
        return 1;
    }
    return 0;
}

void LockPatiEvent(PatiEventWork *work,const char *argument)
{
    int index;
    work->entryFlag=0;
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_SetCharaControllerFlags(PATI_PLAYER,2,0);
    do {
        FTBL.EV_SetCharaControllerFlags(PATI_PLAYER,3,0);
    } while (0);
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EventFunction869();
    work->nearbyDealer=FindNearestPatiDealer();
    if (work->nearbyDealer) {
        FTBL.DebugLogToFileRemoved("Lock Near Pati-Master:");
        PATI_LOG_NAME(work->nearbyDealer);
        FTBL.CheckWarmStartNpcRoute(work->nearbyDealer);
    }
    FTBL.EV_ResetFreeTalkFlags();
    if (g_currentMap==PATI_CHALLENGE_MAP)
        FTBL.EV_SetPlayerFlag(12,1234,1);
    FTBL.CheckHelpIcon(0,0,0);
    for (index=0;index<4;index++) work->argument[index]=argument[index];
    work->argument[4]=0;
    if (*(shen_name *)work->argument==PATI_VERSUS_ARGUMENT) {
        for (index=0;index<4;index++) work->opponent[index]=argument[index+4];
        work->opponent[4]=0;
        for (index=0;index<4;index++) work->versusBoard[index]=argument[index+8];
        work->versusBoard[4]=0;
        for (index=0;index<4;index++) work->dealerGroup[index]=argument[index+12];
        work->dealerGroup[4]=0;
    } else {
        work->opponent[0]=0;
        work->versusBoard[0]=0;
        work->dealerGroup[0]=0;
    }
    FTBL.DebugLogToFileRemoved("PATI.EV3([%s][%s][%s][%s])\n",work->argument,work->opponent,work->versusBoard,work->dealerGroup);
    FTBL.HLib_SetupTaskDelay(0);
}

int LoadPatiEvent(PatiEventWork *work)
{
    int result;
    if (g_currentMap==PATI_CHALLENGE_MAP && FTBL.EV_GetPlayerFlag(11,1234)) return 0;
    result=(int)FTBL.EV_ControlPackageResources(0,5,"PATI",0,0);
    if (!result) {
        FTBL.DebugLogToFileRemoved("PATI:Load [PATI]PAK\n");
        result=(int)FTBL.EV_ControlPackageResources(0,0,FTBL.Filepath_Gen(7),(int)"PATI",0);
        FTBL.DebugLogToFileRemoved("PATI:EV_PAKLOAD = %i\n",result);
        if (!result) return 0;
        result=(int)FTBL.EV_ControlPackageResources(0,7,0,0,0);
        FTBL.DebugLogToFileRemoved("PATI:EV_WAIT_PAKLOAD = %i\n",result);
        if (!result) return 0;
    } else
        FTBL.DebugLogToFileRemoved("PATI:Already-Load [PATI]PAK\n");
    FTBL.EV_ControlPackageResources(2,2,"PATI",0,0);
    FTBL.EV_ControlPackageResources(2,4,"PATI",0,0);
    FTBL.EV_LoadCutsceneFromCurrentScene("01040000.smp");
    FTBL.EV_LoadAudio("01040000",0);
    PATI_TBL_Parser(&work->tables,"PATI_TBL");
    return 1;
}

void ConfigurePatiEventBoard(PatiEventWork *work,const char *name)
{
    char area[8],search[8];
    PatiTableEntry *table;
    PatiBoardEntry *board;
    int tableIndex=0,boardIndex=0,found,index,copyIndex;
    shen_name previous;
    for (copyIndex=0;copyIndex<4;copyIndex++) area[copyIndex]=g_currentMap>>(copyIndex*8);
    area[4]=0;
    for (copyIndex=0;copyIndex<4;copyIndex++) search[copyIndex]=name[copyIndex];
    search[4]=0;
    FTBL.DebugLogToFileRemoved("Bd Name = %s:",area);
    FTBL.DebugLogToFileRemoved("%s : ",search);
    if (search[0]=='b') search[0]='B';
    found=0;
    for (;work->tables.tables[tableIndex].area[0];tableIndex++) {
        if (!strcmp(area,work->tables.tables[tableIndex].area)) {
            for (boardIndex=work->tables.tables[tableIndex].firstBoard;
                 boardIndex<work->tables.tables[tableIndex].firstBoard+work->tables.tables[tableIndex].boardCount;boardIndex++) {
                if (!strcmp(search,work->tables.boards[boardIndex].character)) {found=1;break;}
            }
        }
        if (found) break;
    }
    if (!found) {
        FTBL.DebugLogToFileRemoved("Error:search error.\n");
        FTBL.EventAssertNullsub(__FILE__,5105);
    }
    {
        for (copyIndex=0;copyIndex<4;copyIndex++) work->board[copyIndex]=name[copyIndex];
        work->board[copyIndex]=0;
    }
    table=&work->tables.tables[tableIndex];
    board=&work->tables.boards[boardIndex];
    FTBL.DebugLogToFileRemoved(table->area);
    FTBL.DebugLogToFileRemoved(":");
    FTBL.DebugLogToFileRemoved(table->tableName);
    FTBL.DebugLogToFileRemoved(":");
    FTBL.DebugLogToFileRemoved(table->npc);
    FTBL.DebugLogToFileRemoved(":");
    FTBL.DebugLogToFileRemoved(board->character);
    FTBL.DebugLogToFileRemoved("\n");
    previous=work->dealer;
    work->dealer=PATI_READ_NAME(table->npc);
    if (work->dealer==0x20202020 || !work->dealer) work->dealer=previous;
    work->game.area=PATI_READ_NAME(table->area);
    work->game.table=PATI_READ_NAME(table->tableName);
    work->game.board=PATI_READ_NAME(board->character);
    for (index=0;patiGameOptions[index].name[0];index++) {
        if (!strcmp(patiGameOptions[index].name,table->gameOptions)) break;
    }
    if (!patiGameOptions[index].name[0]) {
        FTBL.DebugLogToFileRemoved("Error:Undefined Game Option.\n");
        FTBL.EventAssertNullsub(__FILE__,5136);
    }
    work->option=patiGameOptions[index].mode;
    work->scale=patiGameOptions[index].scale;
    for (index=0;patiGameTypes[index].name[0];index++) {
        if (!strcmp(patiGameTypes[index].name,table->gameType)) break;
    }
    if (!patiGameTypes[index].name[0]) {
        FTBL.DebugLogToFileRemoved("Error:Undefined Game Type.\n");
        FTBL.EventAssertNullsub(__FILE__,5146);
    }
    work->type=patiGameTypes[index].mode;
    for (index=0;patiWagerModes[index].name[0];index++) {
        if (!strcmp(patiWagerModes[index].name,table->stake)) break;
    }
    if (!patiWagerModes[index].name[0]) {
        FTBL.DebugLogToFileRemoved("Error:Undefined Bet Type.\n");
        FTBL.EventAssertNullsub(__FILE__,5155);
    }
    work->wagerMode=patiWagerModes[index].mode;
    for (index=0;patiPayoutModes[index].name[0];index++) {
        if (!strcmp(patiPayoutModes[index].name,table->payout)) break;
    }
    if (!patiPayoutModes[index].name[0]) {
        FTBL.DebugLogToFileRemoved("Error:Undefined Get Type.\n");
        FTBL.EventAssertNullsub(__FILE__,5164);
    }
    work->payoutMode=patiPayoutModes[index].mode;
    if (work->option==1 || work->option==4 || work->option==5) {
        char *prizeModes=board->prizeModes;
        char *selectedBoard=table->selectedBoard;
        { int copyIndex;
        for (copyIndex=0;copyIndex<7;copyIndex++) {if (!(work->nextBoard[copyIndex]=prizeModes[copyIndex])) break;}
        work->nextBoard[7]=0;
        for (copyIndex=0;copyIndex<7;copyIndex++) {if (!(work->excludedBoard[copyIndex]=selectedBoard[copyIndex])) break;}
        work->excludedBoard[7]=0;
        }
        if (!work->nextBoard[0]) FTBL.DebugLogToFileRemoved("Warning:no option board in board define.\n");
        if (!work->excludedBoard[0]) FTBL.DebugLogToFileRemoved("Warning:no option board in place define.\n");
        FTBL.DebugLogToFileRemoved("place option board idx = %s\n",work->excludedBoard);
        FTBL.DebugLogToFileRemoved("option board idx = %s\n",work->nextBoard);
    }
    SelectPatiBoard(work,work->board);
    AttachPatiNails(work,work->game.board,board->model);
    FTBL.CheckHelpIcon(1,0,0);
    if (work->option==1 && !strcmp(work->nextBoard,board->character)) SetPatiHelpTopic(2,work->type,0);
    else SetPatiHelpTopic(work->option,work->type,0);
}

void ResumePatiEvent(void *data)
{
    PatiEventWork *work=(PatiEventWork *)data;
    if (work->state!=36 && work->state!=67 && work->state!=70)
        FTBL.HLib_ResolveDeferredCallback(work->task);
    if (work->state==20 || work->state==22) {
        FTBL.EV_EnableCharacterDisplay(PATI_PLAYER);
        if (FTBL.DoesScenarioCharacterExist(PATI_BACKPACK)==1) FTBL.EV_EnableCharacterDisplay(PATI_BACKPACK);
        if (*(shen_name *)work->argument!=PATI_VERSUS_ARGUMENT && g_currentMap!=PATI_CHALLENGE_MAP) FTBL.SetPlayerControlEnabled(1);
        FTBL.EV_SetCharaControllerFlags(PATI_PLAYER,2,1);
        FTBL.EV_SetCharaControllerFlags(PATI_PLAYER,3,1);
        FTBL.EVS_LoopCamera(0,0,0);
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
    }
    FTBL.EV_refreshPadControlData(0,1);
    work->state=61;
    work->game.result=-3;
}

void BeginPatiExit(PatiEventWork *work)
{
    int index;
    EventDebugUnregister(3);
    ResetPatiNailModelState();
    for (index=0;index<5;index++) {
        if (work->game.ballCharacters[index].character) {
            FTBL.SetScenarioCharacterKillMode(work->game.ballCharacters[index].character,1);
            work->game.ballCharacters[index].character=0;
        }
    }
    if (g_currentMap==PATI_CHALLENGE_MAP || work->challengeComplete) {
        FTBL.SetScreenFilmFade(30,0,0,0,0,255,0,0,0);
        FTBL.ProcessAicaCommand(0xaa0,30);
    } else {
        if (!g_patiVersus) FTBL.EV_DisableCharacterDisplay(PATI_PLAYER);
        FTBL.EV_AT_SetPos(PATI_PLAYER,0x78000000,&work->game.savedPlayerPosition);
        FTBL.EV_AT_SetAngY_0(PATI_PLAYER,0x78000000,&work->game.savedPlayerRotation,0);
    }
}

int RestorePatiEvent(PatiEventWork *work)
{
    if (g_currentMap==PATI_CHALLENGE_MAP || work->challengeComplete) {
        if (FTBL.filmCheckFadeTrackColour()) return 0;
    }
    FTBL.RestoreMotionSnapshotPlacement(work->dealer,work->savedMotion);
    FTBL.ReleaseMotionSnapshot(work->savedMotion);
    if (*(shen_name *)work->argument!=PATI_VERSUS_ARGUMENT) {
        FTBL.SetRefBoardParamsForCharaById(PATI_PLAYER,0);
        FTBL.SetRefBoardParamsForCharaById(work->dealer,0);
        if (FindPatiDealer(work)) FTBL.EV_SetCharacterAlphaEnabled(work->dealer,1);
        FTBL.EV_EventSetUpHelpIcon_0(0,0);
        FTBL.EVS_LoopCamera(0,0,0);
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
    }
    EVT_AT_SetFaceLooks(PATI_PLAYER,0,10,1.0f,0);
    EVT_AT_SetFaceLooks(work->dealer,0,10,1.0f,0);
    FTBL.ReleaseCharacterFaceCache(FTBL.ctrlGetCharaByName(PATI_PLAYER));
    FTBL.ReleaseCharacterFaceCache(FTBL.ctrlGetCharaByName(work->dealer));
    FTBL.ClearConversationTaskRegistration(work->dealer);
    FTBL.EV_FreeAuthorPak(0,work->game.voicePrefix);
    FTBL.EV_LoadCutsceneFromCurrentScene(work->bgm);
    FTBL.EV_LoadAudio(work->sbt,work->soundScene);
    FTBL.EV_EnableCharacterDisplay(work->dealer);
    FTBL.EV_EnableCharacterDisplay(PATI_PLAYER);
    if (FTBL.DoesScenarioCharacterExist(PATI_BACKPACK)==1) FTBL.EV_EnableCharacterDisplay(PATI_BACKPACK);
    FTBL.CheckHelpIcon(0,0,0);
    return 1;
}

void FreePatiEvent(PatiEventWork *work)
{
    if (*(shen_name *)work->argument!=PATI_VERSUS_ARGUMENT) {
        FTBL.DebugLogToFileRemoved("PATI:Free [PATI]PAK\n");
        FTBL.EV_ControlPackageResources(0,6,"PATI",0,0);
        FTBL.DebugLogToFileRemoved("PATI:Free [PATI]domoto\n");
        if (work->dealer) {
            FTBL.DebugLogToFileRemoved("Unlock Near Pati-Master:");
            PATI_LOG_NAME(work->dealer);
            FTBL.npcLockUnlock(work->dealer);
        }
        FTBL.EV_NPC_NewLoadSw_0(1);
        if (g_currentMap!=PATI_CHALLENGE_MAP) FTBL.SetPlayerControlEnabled(1);
    } else {
        FTBL.DebugLogToFileRemoved("PATI:Un-Free [PATI]PAK\n");
        FTBL.DebugLogToFileRemoved("PATI:Un-Free [PATI]domoto\n");
    }
    FTBL.HLib_MarkCurrentTaskForCleanup();
    FTBL.HLib_DestroyCurrentEventTask();
    FTBL.HLib_ResolveDeferredCallback(work->parent);
    FTBL.EV_SetCharaControllerFlags(PATI_PLAYER,2,1);
    do {
        FTBL.EV_SetCharaControllerFlags(PATI_PLAYER,3,1);
    } while (0);
    if (g_currentMap==PATI_CHALLENGE_MAP) FTBL.EV_SetPlayerFlag(12,1233,1);
    if (work->challengeComplete) FTBL.EV_scenQueueLoad(g_actNumber,g_currentMap,12);
}

void SetPatiBet(PatiEventWork *work)
{
    switch (work->wagerMode) {
    case 1: work->bet=1; break;
    case 2: work->bet=5; break;
    case 3: work->bet=10; break;
    case 4: work->bet=20; break;
    case 5: work->bet=50; break;
    case 6: work->bet=100; break;
    default:
        work->bet=1;
        FTBL.DebugLogToFileRemoved("Unknown Bet Type!!\n");
        FTBL.EventAssertNullsub(__FILE__,5427);
        break;
    }
}

shen_name FindNearestPatiDealer(void)
{
    shen_name names[]={SHEN_NAME4('M','S','T','1'),SHEN_NAME4('M','S','T','2'),SHEN_NAME4('M','S','T','3'),
        SHEN_NAME4('M','S','T','4'),SHEN_NAME4('M','S','T','5'),SHEN_NAME4('M','S','T','6'),
        SHEN_NAME4('M','S','T','7'),SHEN_NAME4('M','S','T','8'),SHEN_NAME4('M','S','T','9'),0};
    Vector3 player,dealer;
    int selected=-1,index;
    unsigned int delay;
    float minimum=1000000.0f,distance;
    FTBL.GetCharaPosition(PATI_PLAYER,-1,&player,0x40000000);
    for (index=0;names[index];index++) {
        if (FTBL.DoesScenarioCharacterExist(names[index])) {
            FTBL.GetCharaPosition(names[index],-1,&dealer,0x40000000);
            distance=FTBL.GetSquaredEuclideanDistance(&player,&dealer);
            FTBL.DebugLogToFileRemoved("(%f,",player.x);
            FTBL.DebugLogToFileRemoved("%f,",player.y);
            FTBL.DebugLogToFileRemoved("%f)-",player.z);
            for (delay=0;delay<1000000;delay++) *(volatile unsigned int *)0xac000000;
            FTBL.DebugLogToFileRemoved("(%f,",dealer.x);
            FTBL.DebugLogToFileRemoved("%f,",dealer.y);
            FTBL.DebugLogToFileRemoved("%f)-",dealer.z);
            for (delay=0;delay<1000000;delay++) *(volatile unsigned int *)0xac000000;
            FTBL.DebugLogToFileRemoved("%f\n",distance);
            for (delay=0;delay<1000000;delay++) *(volatile unsigned int *)0xac000000;
            if (minimum>distance) {
                minimum=distance;
                selected=index;
            }
        }
    }
    return selected>=0 ? names[selected] : 0;
}

shen_name FindPatiDealer(PatiEventWork *work)
{
    shen_name result=0;
    if (work->dealer && work->dealer!=0x20202020) {
        if (FTBL.DoesScenarioCharacterExist(work->dealer)) result=work->dealer;
    } else
        result=FindNearestPatiDealer();
    return result;
}

void PatiShowVoiceFallback(void *data) { QueueEventMessage((const char *)data,0,60); }

int HasPatiWon(PatiEventWork *work)
{
    int won;
    switch (work->type) {
    case 1:
    case 2:
        won = work->game.wins > 0;
        break;
    case 3:
        won = work->game.wins > 2;
        break;
    case 4:
        won = work->game.wins > 3;
        break;
    case 5:
        won = work->game.wins > 1;
        break;
    case 6:
        if (work->game.wins > 0)
            won = GetPatiPayout(work) > 0;
        else
            won = 0;
        break;
    case 8:
        won = work->game.wins >= (short)work->game.winLimit;
        break;
    case 7:
        won = 0;
        FTBL.DebugLogToFileRemoved("Under construction. versus game!!\n");
        FTBL.EventAssertNullsub(__FILE__, 5537);
        break;
    default:
        won = 0;
        FTBL.DebugLogToFileRemoved("Unknown Game Type!!\n");
        FTBL.EventAssertNullsub(__FILE__, 5542);
        break;
    }
    return won;
}

int CanAffordPatiBet(PatiEventWork *work)
{
    int affordable;
    do {
    affordable = FTBL.GetPlayerStat(2) >= work->bet;
    } while (0);
    return affordable;
}

void QueuePatiBall(PatiGameWork *game)
{
    HLibTask *task;
    PatiBallWork *ball;
    task = FTBL.HLib_EnqueueTaskWithParameter(PatiBallCallback, 6, 11, sizeof(PatiBallWork), TASK_EVNT);
    ball = (PatiBallWork *)FTBL.HLib_GetTaskParameter(task);
    ball->game = game;
    InitializePatiBall(ball, task);
}

void InitializePatiBall(PatiBallWork *ball, HLibTask *task)
{
    float cosine;
    ball->task = task;
    ball->nailTask = 0;
    ball->nails = &ball->game->nails;
    InitializePatiBallNails(ball);
    if (ball->game->mode) {
        ball->ballLimit = ball->game->ballLimit;
        ball->winLimit = ball->game->winLimit;
        ball->missLimit = ball->game->missLimit;
    }
    ConfigurePatiPrizes(ball);
    ball->phase = 0;
    ball->soundFrames = 0;
    ball->previousSound = 0;
    ball->settled = 0;
    FTBL.GetCharaPosition(ball->game->board, 0xffffffffU, &ball->boardPosition, 0x40000000);
    FTBL.GetScenarioCharacterEulerAngles(ball->game->board, &ball->boardRotation, 0x40000000);
    if (ball->boardRotation.z >= 0x8000) {
        ball->boardRotation.x += 0x8000;
        ball->boardRotation.y = (unsigned short)(int)((540.0f - (float)ball->boardRotation.y / 182.04445f) * 8192.0f / 45.0f);
        ball->boardRotation.z -= 0x8000;
    }
    cosine = FTBL.CalculateFixedCosine(ball->boardRotation.x);
    cosine = __builtin_fabs(cosine);
    ball->gravity = cosine * -0.052266665f / 30.0f;
    FTBL.DebugLogToFileRemoved("board.ang : %f\n", (float)ball->boardRotation.x / 182.04445f);
    FTBL.DebugLogToFileRemoved("cos : %f\n", cosine);
    FTBL.DebugLogToFileRemoved("gravity_accel : %f\n", ball->gravity);
    FTBL.SetScenarioCharacterFlag40(ball->game->board, 1);
    FTBL.SetScenarioCharacterRadius(ball->game->board, 100.0f);
    InitializePatiBallCharacters(ball);
    SetupPatiBoardSize(0);
    FTBL.DebugLogToFileRemoved("InitEnd\n");
}

void InitializePatiBallCharacters(PatiBallWork *ball)
{
    Vector3i zeroRotation = {0, 0, 0};
    Vector3 origin = {0, 0, 0};
    Vector3i cameraRotation, ballRotation;
    Vector3 size;
    unsigned int index;
    FTBL.EV_DisableCharacterDisplay(PATI_PLAYER);
    if (FTBL.DoesScenarioCharacterExist(PATI_BACKPACK) == 1)
        FTBL.EV_DisableCharacterDisplay(PATI_BACKPACK);
    FTBL.AT_SetLocalBaseChara(PATI_CAMERA, ball->game->board);
    cameraRotation.z = 0;
    cameraRotation.x = 0;
    cameraRotation.y = ball->boardRotation.y;
    FTBL.EV_AT_SetPos(PATI_CAMERA, 0x38000000, &origin);
    FTBL.EV_AT_SetAngY_0(PATI_CAMERA, 0x78000000, &cameraRotation, 0);
    FTBL.AT_SetLocalBaseChara(ball->nails->character, ball->game->board);
    FTBL.EV_AT_SetPos(ball->nails->character, 0x38000000, &origin);
    FTBL.EV_AT_SetAngY_0(ball->nails->character, 0x38000000, &zeroRotation, 0);
    ballRotation.x = 0;
    ballRotation.y = ball->boardRotation.y;
    ballRotation.z = 0x4000;
    size.x = size.y = size.z = ball->scale * 0.73699999f * ball->radiusScale;
    for (index = 0; index < ball->ballLimit; index++) {
        if (ball->game->ballCharacters[index].character) {
            FTBL.SetScenarioCharacterKillMode(ball->game->ballCharacters[index].character, 1);
            ball->game->ballCharacters[index].character = 0;
        }
        ball->game->ballCharacters[index].character = FTBL.CreateEventCharacter(3, ball->ballModel, 0);
        FTBL.EV_AT_SetAngY_0(ball->game->ballCharacters[index].character, 0x38000000, &ballRotation, 0);
        FTBL.mtwkSetMotionworkScale3(ball->game->ballCharacters[index].character, &size);
    }
    PatiSetVector(&ball->current.position, ball->right, ball->top, ball->radius + 0.0049999999f);
    PatiSetVector(&ball->current.velocity, 0, 0, 0);
    PatiCopyVector(&ball->previous.position, &ball->current.position);
    PatiCopyVector(&ball->previous.velocity, &ball->current.velocity);
    ball->releaseX = 0;
    FTBL.DebugLogToFileRemoved("CharaInitEnd\n");
}

const PatiDealerInput PATI_DEALER_INPUTS[] = {
    {32768, 20, 10}, {32768, 15, 15}, {32768, 20, 20}, {32768, 20, 10}, {16384, 20, 20}, {16384, 60, 10},
    {16384, 20, 10}, {32768, 60, 10}, {16384, 15, 15}, {32768, 10, 20}, {16384, 10, 20}, {16384, 30, 10},
    {32768, 10, 20}, {16384, 20, 10}, {32768, 30, 10}, {16384, 10, 20}
};

void PatiBallCallback(void *data)
{
    PatiBallWork *ball = (PatiBallWork *)data;
    Vector3 origin = {0, 0, 0};
    unsigned int input;
    unsigned short mask;
    unsigned int tolerance, oldFrames;
    int index, prize, difference, zero;
    PatiGameWork *game;
    if (ball->game->result == -3)
        ball->phase = 13;
    switch (ball->phase) {
    case 0:
        FTBL.HLib_SetupTaskDelay(10);
        FTBL.SetRenderClockAndCompassTo(1);
        FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(patiHandModel));
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(patiHandModel), 1, 0, 0);
        FTBL.imgmSetHandPos(FTBL.ctrlGetCharaByName(patiHandModel), 0, 21, &PATI_HAND_ROTATION);
        FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(patiHandModel), 0, patiHandPoseCommand, 1);
        FTBL.EV_EnsureCharacterDraw(patiHandModel);
        FTBL.EV_DisableCharacterDisplay(patiHandModel);
        FTBL.SetCharacterAlpha(FTBL.ctrlGetCharaByName(patiHandModel), 0.0f);
        if (patiHandModel == SHEN_NAME4('h', 'a', 'n', 'd')) {
            Vector3 translation={-0.093699999f,0.16869999f,0.47999999f}; Vector3i rotation, handRotation;
            rotation.x=zero=0; rotation.y=zero; rotation.z=zero;
            FTBL.matrix_stack_Matrix4x4_Push(0);
            FTBL.matrix_stack_UnitCurrentMatrix();
            FTBL.Matrix4x4_RotateAroundZ_Wrapper(rotation.z);
            FTBL.Matrix4x4_RotateAroundY_Wrapper(rotation.y);
            FTBL.Matrix4x4_RotateAroundX_Wrapper(rotation.x);
            FTBL.matrix_stack_TranslateVector(&translation);
            FTBL.Matrix4x4_GetTranslationRow(&translation);
            FTBL.DebugLogToFileRemoved("PATI:YubiBase(%f,%f,%f)\n", translation.x, translation.y, translation.z);
            FTBL.PopCurrentMatrix(1);
            FTBL.scenApplyPosOrRotToChildId(patiHandModel, -1, &translation, &rotation, zero);
            handRotation.x = 61895;
            handRotation.y = ball->boardRotation.y;
            handRotation.z = zero;
            FTBL.EV_AT_SetAngY_0(patiHandModel, 0x38000000, &handRotation, 0);
        }
        ball->phase = 3;
    case 3:
        ball->releaseX = FTBL.shenGetRandomInt32(8) - 4;
        ball->releaseJitter = ball->releaseX;
        ball->current.position.x = ball->previous.position.x = (float)ball->releaseX / 1000.0f;
        ball->current.position.y = ball->previous.position.y = ball->top;
        ball->current.position.z = ball->previous.position.z = ball->radius + 0.0049999999f;
        PatiSetVector(&ball->current.velocity, 0, 0, 0);
        PatiCopyVector(&ball->previous.velocity, &ball->current.velocity);
        ball->repeatFrames = 0;
        ball->frames = 0;
        FTBL.EV_EnableCharacterDisplay(patiHandModel);
        ball->phase = 5;
        ball->cameraHeight = ball->top - 0.13f;
        g_patiCameraHeight = 9999.0f;
        if (ball->phase == 5)
            FTBL.CheckHelpIcon(33, 0, 0);
        else
            FTBL.CheckHelpIcon(33, 0, 0);
        if (ball->game->mode == 2)
            FTBL.CheckHelpIcon(1, 0, 0);
        UpdatePatiBallPosition(ball);
        UpdatePatiHandPosition(ball);
        g_patiModelAlpha = 0;
        ball->historyCursor = 0;
        for (index = 0; index < 90; index++) {
            ball->history[index].x = 0;
            ball->history[index].y = 0;
            ball->movement[index] = 0;
        }
        for (index = 0; index < 8; index++) {
            ball->contacts[index].age = index;
            ball->contacts[index].nail = -1;
            ball->contacts[index].count = 0;
        }
        if (ball->game->mode == 2) {
            ball->phase = 6;
            g_patiDealerInputClock = 0;
            g_patiDealerInputIndex = ball->game->balls + FTBL.shenGetRandomInt32(16);
        }
        break;
    case 4:
    case 5:
        if (ball->frames > 5400) {
            input = 1024;
            HandlePatiBallInput(ball, input);
        } else {
            input = FTBL.GetCurrentControlInput(2);
            HandlePatiBallInput(ball, input);
            FTBL.GetBattleDataConfig(0, SHEN_NAME4('B', 'T', 'B', 'A'));
            if (ball->option == 7)
                FTBL.GetBattleDataConfig(0, SHEN_NAME4('B', 'T', 'C', 'C'));
        }
        oldFrames = ball->frames++;
        if (oldFrames == 1800 && !g_patiVersus)
            PatiStuckFollowup(ball);
        if (FTBL.shenGetMinutesUntilNextFrameMinute() <= 0 && ball->frames >= 1800) {
            ball->phase = 13;
            ball->game->result = 4;
        }
        break;
    case 6:
        ball->game->activeBall = ball->game->balls;
        FTBL.EV_EnableCharacterDisplay(ball->game->ballCharacters[ball->game->activeBall].character);
        input = 0;
        if (ball->frames > 150) {
            difference = ball->releaseX - ball->game->targetX;
            if (difference < 0)
                difference = -difference;
            tolerance = (ball->game->targetVariation + ball->frames) / 10;
            if (difference <= tolerance || ball->frames > 210)
                input = 1024;
        }
        if (input != 1024) {
            if (g_patiDealerInputClock < PATI_DEALER_INPUTS[g_patiDealerInputIndex & 15].duration)
                input = PATI_DEALER_INPUTS[g_patiDealerInputIndex & 15].input;
            else if (g_patiDealerInputClock > PATI_DEALER_INPUTS[g_patiDealerInputIndex & 15].duration + PATI_DEALER_INPUTS[g_patiDealerInputIndex & 15].pause) {
                g_patiDealerInputIndex++;
                g_patiDealerInputClock = 0;
            }
            g_patiDealerInputClock++;
        }
        ball->frames++;
        if (HandlePatiBallInput(ball, input))
            g_patiDealerInputClock = PATI_DEALER_INPUTS[g_patiDealerInputIndex & 15].duration;
        break;
    case 7:
        g_patiModelAlpha = 0;
        FTBL.CheckHelpIcon(1, 0, 0);
        FTBL.EV_DisableCharacterDisplay(patiHandModel);
        ball->game->activeBall = ball->game->balls;
        FTBL.EV_EnableCharacterDisplay(ball->game->ballCharacters[ball->game->activeBall].character);
        AdvancePatiBall(ball);
        UpdatePatiBallPosition(ball);
        break;
    case 13:
        FTBL.SetRenderClockAndCompassTo(0);
        FTBL.EV_DisableCharacterDisplay(patiHandModel);
        FTBL.EV_ToggleCharacterFlag(patiHandModel);
        DestroyPatiBall(ball);
        break;
    }
    if (ball->settled && ball->phase == 7) {
        ball->game->result = -2;
        if (__builtin_fabs(ball->current.position.y - ball->bottom) < ball->radius) {
            index = 0;
            while (index + 1 < ball->pocketCount && !(ball->current.position.x < ball->dividers[index].x))
                index++;
            ball->game->pocket = index;
            prize = 0;
            while (patiPrizes[prize].code && patiPrizes[prize].code != ball->prizes[index])
                prize++;
            ball->game->prize = prize;
            ball->game->result = 0;
        } else {
            if (ball->option == 8) {
                for (index = 0; index < 6; index++) {
                    if (ball->centerPockets[index].type) {
                        float left, right, bottom, top;
                        left = ball->centerPockets[index].x - ball->centerPockets[index].halfWidth;
                        right = ball->centerPockets[index].x + ball->centerPockets[index].halfWidth;
                        bottom = ball->centerPockets[index].y;
                        top = ball->centerPockets[index].y + (ball->radius + ball->radius);
                        if (ball->current.position.x > left && right > ball->current.position.x && ball->current.position.y > bottom
                            && top > ball->current.position.y) {
                        ball->game->pocket = index + 10;
                        prize = 0;
                        while (patiPrizes[prize].code && patiPrizes[prize].code != ball->centerPrizes[index])
                            prize++;
                        ball->game->prize = prize;
                        ball->game->result = 0;
                    }
                    }
                }
            }
            if (ball->game->result == -2) {
                FTBL.DebugLogToFileRemoved("%f -> ", (float)ball->releaseX / 1000.0f);
                FTBL.DebugLogToFileRemoved("Stuck\n");
                ball->phase = 13;
                ball->game->result = -1;
            }
        }
        if (!ball->game->result) {
            ball->game->balls++;
            ball->phase = 3;
            index = ball->game->pocket;
            mask = (unsigned short)(1U << index);
            FTBL.DebugLogToFileRemoved("%f -> ", (float)ball->releaseX / 1000.0f);
            FTBL.DebugLogToFileRemoved("%i\n", index);
            if (mask & ball->deathMask) {
                ball->phase = 13;
                ball->game->result = 3;
            } else {
                if (mask & ball->winMask)
                    ball->game->wins++;
                if (ball->game->balls >= ball->ballLimit)
                    ball->phase = 13;
                else {
                    game = ball->game;
                    if (game->wins >= ball->winLimit || game->balls - game->wins > ball->missLimit)
                        ball->phase = 13;
                }
                if (ball->phase == 13) {
                    if (ball->game->wins >= ball->winLimit)
                        ball->game->result = 2;
                    else
                        ball->game->result = 1;
                    if (ball->game->result == 2) {
                        ball->game->targetX = FTBL.shenGetRandomInt32(10) + ball->releaseX;
                        ball->game->targetVariation = FTBL.shenGetRandomInt32(100);
                    } else {
                        ball->game->targetX = FTBL.shenGetRandomInt32(50) + ball->releaseX;
                        ball->game->targetVariation = FTBL.shenGetRandomInt32(500);
                    }
                }
            }
            if (ball->phase == 3) {
                FTBL.EV_DisableCharacterDisplay(ball->game->ballCharacters[ball->game->balls - 1].character);
                FTBL.EV_AT_SetPos(PATI_CAMERA, 0x38000000, &origin);
                SetupPatiBoardSize(1);
            }
        }
    }
}

void DestroyPatiBall(PatiBallWork *ball)
{
    FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(patiHandModel));
    FTBL.EV_EnableCharacterDisplay(PATI_PLAYER);
    FTBL.ResetScenarioCharacterAttachment(PATI_CAMERA);
    PatiBallCleanup(ball);
    if (FTBL.DoesScenarioCharacterExist(PATI_BACKPACK) == 1)
        FTBL.EV_EnableCharacterDisplay(PATI_BACKPACK);
    if (ball->nailTask)
        FTBL.HLib_DestroyTask(ball->nailTask);
    FTBL.HLib_DestroyCurrentEventTask();
    FTBL.HLib_ResolveDeferredCallback(ball->game->parent);
}

int HandlePatiBallInput(PatiBallWork *ball, unsigned int input)
{
    int edge = 0, candidate, index;
    if (ball->phase == 4) {
        if (input & 0x400) {
            FTBL.CheckHelpIcon(33, 0, 0);
            ball->phase = 5;
        } else if (input & 0x200) {
            FTBL.CheckHelpIcon(1, 0, 0);
            ball->phase = 13;
            ball->game->result = -4;
        }
    } else if (ball->phase == 5 || ball->phase == 6) {
        if (input & 0x400) {
            FTBL.CheckHelpIcon(1, 0, 0);
            ball->phase = 7;
            ball->frames = 0;
            ball->repeatFrames = 0;
            ball->soundFrames = 0;
            ball->current.position.y = ball->previous.position.y = ball->top;
            ball->current.position.z = ball->previous.position.z = ball->radius + 0.0049999999f;
            PatiSetVector(&ball->current.velocity, 0, 0, 0);
            PatiCopyVector(&ball->previous.velocity, &ball->current.velocity);
            ball->settled = 0;
            ball->cameraHeight = ball->top - 0.13f;
        } else if ((input & 0x200) && ball->option == 7) {
            FTBL.CheckHelpIcon(33, 0, 0);
            FTBL.CheckHelpIcon(1, 0, 0);
            ball->phase = 13;
            ball->game->result = -4;
        } else {
            if (input & 0xf000)
                ball->repeatFrames++;
            else
                ball->repeatFrames = 0;
            for (index = 0; index <= (ball->repeatFrames >= 45 ? 9 : (unsigned short)(ball->repeatFrames / 5U)); index++) {
                if (input & 0x4000) {
                    float position, limit;
                    candidate = ball->releaseX - 1;
                    position = (float)candidate / 1000.0f;
                    limit = ball->left + (float)(ball->releaseJitter + 4) / 1000.0f;
                    if (!(position < limit))
                        ball->releaseX = candidate;
                    else
                        edge = 1;
                } else if (input & 0x8000) {
                    float position, limit;
                    candidate = ball->releaseX + 1;
                    position = (float)candidate / 1000.0f;
                    limit = ball->right - (float)(ball->releaseJitter + 4) / 1000.0f;
                    if (!(limit < position))
                        ball->releaseX = candidate;
                    else
                        edge = 1;
                }
            }
        }
    }
    if (ball->phase == 5) {
        FTBL.EV_EnableCharacterDisplay(patiHandModel);
        ball->game->activeBall = ball->game->balls;
        FTBL.EV_EnableCharacterDisplay(ball->game->ballCharacters[ball->game->activeBall].character);
        SetupPatiBoardSize(1);
    } else if (ball->phase == 4) {
        FTBL.EV_DisableCharacterDisplay(patiHandModel);
        ball->game->activeBall = ball->game->balls;
        FTBL.EV_DisableCharacterDisplay(ball->game->ballCharacters[ball->game->activeBall].character);
        SetupPatiBoardSize(2);
    } else if (ball->phase == 6) {
        FTBL.EV_EnableCharacterDisplay(patiHandModel);
        ball->game->activeBall = ball->game->balls;
        FTBL.EV_EnableCharacterDisplay(ball->game->ballCharacters[ball->game->activeBall].character);
        SetupPatiBoardSize(1);
    }
    if (ball->phase != 7) {
        ball->current.position.x = ball->previous.position.x = (float)ball->releaseX / 1000.0f;
        if (input & 0xc000) {
            UpdatePatiBallPosition(ball);
            UpdatePatiHandPosition(ball);
        }
    }
    return edge;
}

void UpdatePatiBallPosition(PatiBallWork *ball) {
    Vector3 local, position;
    float sine, cosine;
    sine = FTBL.CalculateFixedSine(ball->boardRotation.x);
    cosine = FTBL.CalculateFixedCosine(ball->boardRotation.x);
    local.x = ball->current.position.x;
    local.y = cosine * ball->current.position.y - sine * ball->current.position.z;
    local.z = cosine * ball->current.position.z + sine * ball->current.position.y;
    sine = FTBL.CalculateFixedSine(ball->boardRotation.y);
    cosine = FTBL.CalculateFixedCosine(ball->boardRotation.y);
    position.x = cosine * local.x + sine * local.z;
    position.z = cosine * local.z - sine * local.x;
    position.x += ball->boardPosition.x;
    position.y = local.y + ball->boardPosition.y;
    position.z += ball->boardPosition.z;
    PatiCopyVector(&ball->game->ballCharacters[ball->game->balls].position, &position);
    FTBL.EV_AT_SetPos(ball->game->ballCharacters[ball->game->balls].character, 0x38000000, &ball->game->ballCharacters[ball->game->balls].position);
}

void UpdatePatiHandPosition(PatiBallWork *ball) {
    Vector3 local, position;
    Vector3i rotation;
    float sine, cosine;
    sine = FTBL.CalculateFixedSine(ball->boardRotation.x);
    cosine = FTBL.CalculateFixedCosine(ball->boardRotation.x);
    local.x = ball->current.position.x;
    local.y = cosine * ball->current.position.y - sine * ball->current.position.z;
    local.z = cosine * ball->current.position.z + sine * ball->current.position.y;
    sine = FTBL.CalculateFixedSine(ball->boardRotation.y);
    cosine = FTBL.CalculateFixedCosine(ball->boardRotation.y);
    position.x = cosine * local.x + sine * local.z;
    position.z = cosine * local.z - sine * local.x;
    position.x += ball->boardPosition.x;
    position.y = local.y + ball->boardPosition.y;
    position.z += ball->boardPosition.z;
    FTBL.EV_AT_SetPos(patiHandModel, 0x38000000, &position);
    if (patiHandModel == SHEN_NAME4('h', 'a', 'n', 'd')) {
        rotation.x = 61895;
        rotation.y = (unsigned short)(int)((25.0f - ball->current.position.x * 100.0f) * 8192.0f / 45.0f) + ball->boardRotation.y;
        rotation.z = 0;
    } else {
        rotation.x = 0;
        rotation.y = ball->boardRotation.y + (unsigned short)(int)((115.0f - ball->current.position.x * 100.0f) * 8192.0f / 45.0f);
        rotation.z = 0;
    }
    FTBL.EV_AT_SetAngY_0(patiHandModel, 0x38000000, &rotation, 0);
}

void PatiCopyVector(Vector3 *target, const Vector3 *source) {
    target->x = source->x;
    target->y = source->y;
    target->z = source->z;
}

void PatiSetVector(Vector3 *target, float x, float y, float z) {
    target->x = x;
    target->y = y;
    target->z = z;
}

PatiFallbackVoice patiFallbackVoices[] = {{0, 0}};
PatiPlacementNode *patiDetachedNailNode = 0;
Vector3i PATI_HAND_ROTATION = {52898, 4550, 3394};
int patiHandPoseCommand[19][3] = {{0, 726, 64080}, {0, 1, 1093}, {0, 61168, 0}, {0, 0, 63898}, {0, 363, 60622}, {0, 1456, 58438},
    {0, 65172, 4914}, {0, 0, 58438}, {0, 0, 45516}, {0, 0, 53342}, {0, 546, 4368}, {0, 0, 56436}, {0, 0, 46608}, {0, 0, 54070},
    {0, 546, 3821}, {0, 0, 52978}, {0, 0, 48792}, {0, 0, 53524}, {0, 546, 3276}};

#endif

#endif
