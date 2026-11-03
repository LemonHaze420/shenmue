#if defined(SHENMUE_EVENT_OVERLAY) && defined(SHENMUE_EVENT_FRBT_AB00)
#include "s2/areas/area_frbt.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void RunFrbtEvent(int argument)
{
    unsigned char count;
    FTBL.SetPlayerControlEnabled(0);
    FTBL.SetScenarioFollowCharacter(0);
    frbtActive = 1;
    frbtVariant = argument;
    FTBL.HLib_EnqueueTaskWithoutParameter(FrbtStartCallback, 5, 11, FRBT_WAIT_TASK);
    for (count = 0; frbtEnemyGroups[frbtVariant][count] != FRBT_INVALID; count++)
        frbtCharacters[count + 1] = frbtEnemyGroups[frbtVariant][count];
    frbtCharacters[count + 1] = FRBT_INVALID;
}
#else
const shen_name frbtEnemyGroups[FRBT_VARIANT_COUNT][6] = {
    {SHEN_NAME4('J', 'U', 'K', '_'), SHEN_NAME4('H', '0', '8', '_'), SHEN_NAME4('H', '1', '0', 'Q'), SHEN_NAME4('H', '1', '6', 'Q'), SHEN_NAME4('H', '1', '9', 'Q'), FRBT_INVALID},
    {SHEN_NAME4('U', 'N', 'H', '_'), SHEN_NAME4('H', '0', '9', 'Q'), SHEN_NAME4('H', '1', '5', 'Q'), FRBT_INVALID},
    {SHEN_NAME4('S', 'Y', 'J', '_'), SHEN_NAME4('H', '0', '3', 'Q'), SHEN_NAME4('H', '0', '4', '_'), SHEN_NAME4('H', '0', '7', 'Q'), SHEN_NAME4('H', '1', '9', '_'), FRBT_INVALID},
    {SHEN_NAME4('E', 'N', 'F', '_'), SHEN_NAME4('H', '1', '1', 'Q'), SHEN_NAME4('H', '1', '7', 'Q'), SHEN_NAME4('H', '2', '0', 'Q'), FRBT_INVALID},
    {SHEN_NAME4('A', 'G', 'O', '_'), SHEN_NAME4('H', '0', '1', '_'), SHEN_NAME4('H', '1', '0', '_'), SHEN_NAME4('H', '1', '3', '_'), SHEN_NAME4('H', '1', '4', '_'), FRBT_INVALID},
    {SHEN_NAME4('T', 'Y', 'Z', '_'), SHEN_NAME4('H', '0', '5', 'Q'), SHEN_NAME4('H', '0', '6', 'Q'), SHEN_NAME4('H', '1', '2', '_'), FRBT_INVALID},
    {SHEN_NAME4('S', 'N', 'C', '_'), SHEN_NAME4('H', '0', '4', 'Q'), SHEN_NAME4('H', '1', '1', '_'), SHEN_NAME4('H', '1', '4', '_'), FRBT_INVALID},
    {SHEN_NAME4('H', 'V', '1', '_'), FRBT_INVALID},
    {SHEN_NAME4('H', 'V', '2', '_'), FRBT_INVALID},
    {SHEN_NAME4('H', 'V', '3', '_'), FRBT_INVALID},
    {SHEN_NAME4('H', 'V', '4', '_'), SHEN_NAME4('H', 'V', '8', '_'), FRBT_INVALID},
    {SHEN_NAME4('H', 'V', '5', '_'), SHEN_NAME4('H', 'V', '9', '_'), FRBT_INVALID},
    {SHEN_NAME4('H', 'V', '6', '_'), FRBT_INVALID},
    {SHEN_NAME4('H', 'V', '7', '_'), SHEN_NAME4('K', 'E', 'I', '_'), FRBT_INVALID},
    {SHEN_NAME4('H', 'V', 'A', '_'), FRBT_INVALID},
    {SHEN_NAME4('Y', 'O', 'H', '_'), SHEN_NAME4('R', 'I', 'K', '_'), FRBT_INVALID},
    {SHEN_NAME4('K', 'H', 'O', '_'), SHEN_NAME4('S', 'E', 'E', '_'), FRBT_INVALID},
    {SHEN_NAME4('S', 'N', 'N', '_'), SHEN_NAME4('F', 'U', 'Y', '_'), FRBT_INVALID}};
const shen_name frbtTrapCharacters[] = {SHEN_NAME4('A', 'G', 'O', '_'), SHEN_NAME4('E', 'N', 'F', '_'), SHEN_NAME4('R', 'N', 'P', '_'), SHEN_NAME4('T', 'G', 'I', '_'), FRBT_INVALID};
const FrbtMessage frbtNoMessages[] = {
    {"", 0, 0}};
const FrbtMessage frbtBeginMessages4[] = {
    {"おい…おまえ。", 60, 0},
    {"●青俺のことか？", 60, 170},
    {"ああ…見ねえ顔だな。＆おまえ何モンだ？", 60, 240},
    {"●青俺はレンを探してる。", 90, 310},
    {"なんだと…？＆レンさんに何の用だ！？", 60, 410},
    {"レンを知ってるのか？＆教えてくれ。どこにいる？", 60, 480},
    {"てめえ…レンさんを＆気安く呼ぶんじゃねえ", 60, 550},
    {"", 0, 0}};
const FrbtMessage frbtBeginMessages5[] = {
    {"●青おい…", 60, 0},
    {"なんだ、おめぇ…", 60, 70},
    {"●青クールＪはどこにいる？", 60, 140},
    {"よそ者がいったい何の用だ！？", 60, 210},
    {"●青お前には関係ない。＆クールＪの居場所を教えろ。", 90, 280},
    {"てめえ…", 60, 380},
    {"生意気な口たたくんじゃねえぞ！", 60, 450},
    {"", 0, 0}};
const FrbtMessage frbtWinMessages0[] = {
    {"くっそぉ…", 60, 0},
    {"うわっ…！す…すんません！", 90, 70},
    {"●青レンはどこだ？", 60, 170},
    {"し…知らないっス…", 60, 240},
    {"ぐぐううっ！…ほ、本当っス！＆俺ら下っ端は知らないっス！", 90, 310},
    {"くく…クールＪなら…たぶん。", 60, 480},
    {"●青クールＪ？", 60, 550},
    {"サングラスをしているでかい男だ", 90, 620},
    {"はあっ…はあっ…", 60, 720},
    {"", 0, 0}};
const FrbtMessage frbtCutsceneMessages0[] = {
    {"●青クールＪか…", 60, 0},
    {"", 0, 0}};
const FrbtMessage frbtWinMessages1[] = {
    {"くっそぉ…", 60, 0},
    {"ぐわっ…！", 60, 70},
    {"●青クールＪはどこにいる。", 60, 140},
    {"た…たぶん、スクラップ置き場っス…", 60, 210},
    {"●青どこにある！？", 60, 280},
    {"ぐううっ！…", 60, 350},
    {"ビバリーヒルズワーフの東の端っス…＆壊れたバスが捨ててあるっス…", 90, 420},
    {"●青そこにいるんだな！？", 60, 520},
    {"は…はいっ！", 60, 590},
    {"●青案内しろ。", 60, 660},
    {"", 0, 0}};
const FrbtMessage frbtCutsceneMessages5[] = {
    {"●青スクラップ置き場か…", 60, 0},
    {"", 0, 0}};
const FrbtMessage frbtWinMessages2[] = {
    {"ううっ…くっそ…", 60, 0},
    {"ぐわっ…", 90, 70},
    {"●青クールＪはどこにいる。", 60, 170},
    {"た…たぶん、スクラップ置き場だ…", 60, 240},
    {"●青どこにある！？", 60, 310},
    {"ぐううっ！…", 60, 370},
    {"ビバリーヒルズワーフの東の端だ…＆壊れたバスが捨ててある…", 90, 450},
    {"●青そこにいるんだな！？", 60, 550},
    {"あ…ああ…", 60, 620},
    {"はあっ…はあっ…", 60, 690},
    {"スクラップ置き場か…", 60, 760},
    {"", 0, 0}};
const FrbtMessage frbtHintMessages[] = {
    {"金を持ってるならクールＪにあいな", 60, 0},
    {"", 0, 0}};
const FrbtMessage frbtWinMessages6[] = {
    {"勝利ＳＡ", 60, 0},
    {"レンは公園にいる", 60, 70},
    {"", 0, 0}};
const FrbtMessage frbtLoseMessages[] = {
    {"けっ、口ほどにもねえ。", 60, 0},
    {"まだ、この辺うろついてたら、＆次はブッ殺すぞ。", 90, 70},
    {"", 0, 0}};
const FrbtMessage frbtWinMessages4[] = {
    {"敵が逃げるＰＡ", 60, 0},
    {"", 0, 0}};
const FrbtEnemyDefinition frbtEnemyDefinitions[] = {
    {SHEN_NAME4('U', 'N', 'H', '_'), 100, 2},
    {SHEN_NAME4('S', 'Y', 'J', '_'), 100, 2},
    {SHEN_NAME4('A', 'G', 'O', '_'), 100, 2},
    {SHEN_NAME4('J', 'U', 'K', '_'), 100, 2},
    {SHEN_NAME4('E', 'N', 'F', '_'), 100, 2},
    {SHEN_NAME4('K', 'E', 'I', '_'), 100, 2},
    {SHEN_NAME4('T', 'Y', 'Z', '_'), 100, 2},
    {SHEN_NAME4('T', 'G', 'I', '_'), 100, 2},
    {SHEN_NAME4('F', 'U', 'Y', '_'), 100, 2},
    {SHEN_NAME4('S', 'E', 'E', '_'), 100, 2},
    {SHEN_NAME4('Y', 'O', 'H', '_'), 100, 2},
    {SHEN_NAME4('K', 'H', 'O', '_'), 100, 2},
    {SHEN_NAME4('R', 'I', 'K', '_'), 100, 2},
    {SHEN_NAME4('S', 'N', 'N', '_'), 100, 2},
    {0, 100, 2}};
const FrbtOutcomeRecord frbtOutcomes[FRBT_VARIANT_COUNT] = {
    {171, 0, 1, 1, "0052F"},
    {172, 0, 1, 1, "0052B"},
    {173, 0, 1, 1, "0052L"},
    {192, 0, 1, 1, "0052D"},
    {193, 0, 1, 1, "0052A"},
    {194, 0, 1, 1, "0052J"},
    {195, 0, 1, 1, "0052H"},
    {464, 0, 0, 1, "0052A"},
    {465, 0, 0, 1, "0052A"},
    {466, 0, 0, 1, "0052A"},
    {467, 0, 0, 1, "0052A"},
    {468, 0, 0, 1, "0052A"},
    {469, 0, 0, 1, "0052A"},
    {470, 0, 0, 1, "0052A"},
    {471, 0, 0, 1, "0052A"},
    {164, 0, 0, 1, "0052A"},
    {166, 0, 0, 1, "0052A"},
    {165, 0, 0, 1, "0052A"}};
const FrbtMessage *const frbtResponseMessages[FRBT_RESPONSE_COUNT][4] = {
    {frbtNoMessages, frbtWinMessages0, frbtCutsceneMessages0, frbtLoseMessages},
    {frbtNoMessages, frbtWinMessages1, frbtNoMessages, frbtLoseMessages},
    {frbtNoMessages, frbtWinMessages2, frbtNoMessages, frbtLoseMessages},
    {frbtNoMessages, frbtWinMessages2, frbtNoMessages, frbtLoseMessages},
    {frbtBeginMessages4, frbtWinMessages4, frbtCutsceneMessages0, frbtLoseMessages},
    {frbtBeginMessages5, frbtWinMessages4, frbtCutsceneMessages5, frbtLoseMessages},
    {frbtNoMessages, frbtWinMessages6, frbtNoMessages, frbtLoseMessages},
    {frbtNoMessages, frbtWinMessages6, frbtNoMessages, frbtLoseMessages},
    {frbtNoMessages, frbtNoMessages, frbtNoMessages, frbtNoMessages}};
shen_name frbtCharacters[10] = {FRBT_RYO, FRBT_INVALID};
FrbtPlacement frbtPositions[7][5] = {
    {{{243.5f, 96.85f, 492.2f}, 90.0f}, {{242.4f, 96.85f, 491.0f}, 235.0f}, {{244.4f, 96.85f, 491.0f}, 135.0f}, {{243.7f, 96.85f, 490.3f}, 135.0f}, {{242.6f, 96.85f, 490.3f}, 190.0f}},
    {{{276.9f, 96.85f, 496.2f}, 305.0f}, {{278.7f, 96.85f, 496.3f}, 105.0f}, {{278.4f, 96.85f, 495.1f}, 125.0f}, {{277.0f, 96.85f, 494.6f}, 155.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}},
    {{{242.8f, 96.85f, 526.3f}, 270.0f}, {{244.2f, 96.85f, 527.7f}, 65.0f}, {{243.7f, 96.85f, 507.7f}, 50.0f}, {{244.4f, 96.85f, 525.6f}, 120.0f}, {{243.7f, 96.85f, 525.3f}, 120.0f}},
    {{{250.5f, 96.85f, 577.0f}, 210.0f}, {{251.9f, 96.85f, 578.8f}, 30.0f}, {{252.1f, 96.85f, 578.1f}, 45.0f}, {{250.7f, 96.85f, 579.5f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}},
    {{{250.7f, 96.85f, 579.5f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}},
    {{{195.3f, 96.85f, 526.6f}, 90.0f}, {{194.0f, 96.85f, 527.3f}, 290.0f}, {{193.7f, 96.85f, 525.4f}, 250.0f}, {{194.5f, 96.85f, 524.7f}, 210.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}},
    {{{197.6f, 96.85f, 569.7f}, 225.0f}, {{198.9f, 96.85f, 570.1f}, 65.0f}, {{198.4f, 96.85f, 571.0f}, 45.0f}, {{196.4f, 96.85f, 571.2f}, 350.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}}};
FrbtPlacement frbtBases[24][5] = {
    {{{245.2f, 96.85f, 492.9f}, 90.0f}, {{0.0f, -100.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}},
    {{{275.6f, 96.85f, 496.8f}, 180.0f}, {{0.0f, -100.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}},
    {{{240.6f, 96.85f, 524.0f}, 180.0f}, {{0.0f, -100.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}},
    {{{250.0f, 96.85f, 574.7f}, 0.0f}, {{0.0f, -100.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}},
    {{{221.0f, 96.85f, 549.9f}, 270.0f}, {{0.0f, -100.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}},
    {{{196.2f, 96.85f, 532.9f}, 180.0f}, {{0.0f, -100.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}},
    {{{197.2f, 96.85f, 568.8f}, 0.0f}, {{0.0f, -100.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}},
    {{{303.2f, 96.85f, 505.3f}, 247.5f}, {{0.0f, -100.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}},
    {{{288.7f, 95.3f, 555.3f}, 180.0f}, {{288.9f, 95.3f, 561.0f}, 0.0f}, {{0.0f, -100.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}},
    {{{284.6f, 95.3f, 550.0f}, 90.0f}, {{280.7f, 95.3f, 550.4f}, 270.0f}, {{0.0f, -100.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}},
    {{{279.4f, 95.3f, 569.9f}, 90.0f}, {{264.7f, 95.3f, 568.6f}, 270.0f}, {{0.0f, -100.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}},
    {{{250.7f, 96.85f, 566.6f}, 0.0f}, {{253.4f, 96.85f, 552.0f}, 160.0f}, {{247.5f, 96.85f, 551.8f}, 200.0f}, {{0.0f, -100.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}},
    {{{200.7f, 96.85f, 492.5f}, 67.5f}, {{0.0f, -100.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}},
    {{{201.7f, 96.85f, 548.3f}, 225.0f}, {{201.0f, 96.85f, 553.5f}, 292.5f}, {{207.5f, 96.85f, 548.6f}, 112.5f}, {{206.8f, 96.85f, 554.6f}, 45.0f}, {{0.0f, -100.0f, 0.0f}, 0.0f}},
    {{{178.7f, 96.85f, 572.3f}, 180.0f}, {{0.0f, -100.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}, {{0.0f, 0.0f, 0.0f}, 0.0f}}};

int frbtActive __attribute__((section(".bss")));
int frbtVariant __attribute__((section(".bss")));
unsigned int frbtResponse __attribute__((section(".bss")));
char frbtVoicePrefixes[FRBT_VOICE_PREFIX_COUNT][FRBT_VOICE_PREFIX_LENGTH] __attribute__((section(".bss")));
char frbtVoices[FRBT_VOICE_COUNT][FRBT_VOICE_LENGTH] __attribute__((section(".bss")));

void FrbtStartCallback(void *data)
{
    HLibTask *task;
    FrbtWork *work;
    int stage;
    if (!FTBL.EV_GetPlayerFlag(11, 1234) && !(stage = FTBL.EV_GetPlayerFlag(11, 1233))) {
        task = FTBL.HLib_EnqueueTaskWithParameter(FrbtInitializeCallback, 5, 11, sizeof(FrbtWork), FRBT_TASK);
        work = (FrbtWork *)FTBL.HLib_GetTaskParameter(task);
        work->task = task;
        work->motion = (void *)-1;
        work->outcome = FRBT_OUTCOME_WON;
        work->stage = stage;
        FTBL.EV_SetPlayerFlag(12, 1234, 1);
        FrbtChooseResponse();
        FrbtCopyEnemies(work);
        FTBL.EV_SetNewSceneIndex();
        work->characters = Queue_EVT_AT_ReadChr(task, frbtCharacters);
        EventDebugRegister(FRBT_DEBUG_SLOT, FrbtAbortCallback, work, FRBT_DEBUG_FRAMES);
        FTBL.HLib_DestroyCurrentTask();
    }
}

void FrbtChooseResponse(void)
{
    if (frbtVariant >= 0 && frbtVariant <= 6) {
        if (FTBL.EV_GetShenmueFlag(116))
            frbtResponse = 0;
        else if (FTBL.EV_GetShenmueFlag(117))
            frbtResponse = 3;
        else if (FTBL.EV_GetShenmueFlag(780))
            frbtResponse = 1;
        else
            frbtResponse = 6;
    } else if (frbtVariant >= 7 && frbtVariant <= 14) {
        if (FTBL.EV_GetShenmueFlag(116))
            frbtResponse = 4;
        else
            frbtResponse = 5;
    } else
        frbtResponse = 7;
}

void FrbtInitializeResources(void)
{
    FTBL.CreateEventCharacter(3, 0, FRBT_BASE);
    FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(FRBT_RYO));
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(7), "FRBT", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "FRBT", 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, "FRBT", 0, 0);
    FTBL.EV_FreeAuthorPak(1, "0052");
    FTBL.EV_FreeAuthorPak(1, "S2BV");
    if (g_currentMap == FRBT_AB00) {
        FTBL.EV_LoadCutsceneFromCurrentScene("0052E010");
        FTBL.EV_LoadAudio("0052E010", 0);
    } else {
        FTBL.EV_LoadCutsceneFromCurrentScene("0073E000");
        FTBL.EV_LoadAudio("0073E000", 0);
    }
    FrbtSetVoicePrefix(0, "0052A");
    FrbtSetVoicePrefix(1, frbtOutcomes[frbtVariant].voicePrefix);
    FTBL.DebugLogToFileRemoved("%s %d\n", frbtOutcomes[frbtVariant].voicePrefix, frbtVariant);
    FTBL.flddSetPointFlagOnGrid(FRBT_BATTLE_GRID, 1);
    FTBL.flddSetPointFlagOnGrid(FRBT_DEFAULT_GRID, 0);
    FTBL.EV_spmBeginSampleExecute(0);
}

void FrbtCopyEnemies(FrbtWork *work)
{
    int i;
    for (i = 0; frbtEnemyGroups[frbtVariant][i] != FRBT_INVALID; i++)
        work->enemies[i] = frbtEnemyGroups[frbtVariant][i];
    work->enemyCount = i;
    for (i = 0; i < work->enemyCount; i++) {
        FTBL.DebugLogToFileRemoved("EnemyEntry %d: ", i);
        FTBL.DebugLogToFileRemoved("ID : %c%c%c%c", work->enemies[i] & 0xff, work->enemies[i] >> 8 & 0xff, work->enemies[i] >> 16 & 0xff,
            work->enemies[i] >> 24);
        FTBL.DebugLogToFileRemoved("\n");
    }
    FTBL.DebugLogToFileRemoved("EventIndex  %d\n", frbtVariant);
    FTBL.DebugLogToFileRemoved("MsgNum  %d\n", frbtResponse);
}

void FrbtInitializeCallback(void *data)
{
    FrbtWork *work = (FrbtWork *)data;
    int i;
    Vector3 position;
    float yaw;
    FrbtInitializeResources();
    FrbtSetupBattle(work);
    for (i = 0; i < work->enemyCount; i++) {
        do {
            FTBL.EV_SetCharaControllerFlags(work->enemies[i], 2, 0);
            FTBL.EV_SetCharaControllerFlags(work->enemies[i], 3, 0);
        } while (0);
        FTBL.EV_EnableCharacterDisplay(work->enemies[i]);
        FTBL.EV_SetCharacterAlphaEnabled(work->enemies[i], 0);
        FTBL.EV_SetCharaControllerFlags(work->enemies[i], 1, 0);
    }
    if (frbtVariant >= 7 && frbtVariant <= 14)
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (frbtVariant >= 0 && frbtVariant <= 6)
        for (i = 0; i < work->enemyCount; i++) {
            position = frbtPositions[frbtVariant][i].position;
            yaw = frbtPositions[frbtVariant][i].yaw;
            EVT_AT_SetPos(work->enemies[i], FRBT_INVALID, position.x, position.y, position.z, 0);
            EVT_AT_SetAngY(work->enemies[i], FRBT_INVALID, yaw, 0);
            FTBL.DebugLogToFileRemoved("%3.2f  %3.2f %3.2f\n", position.x, position.y, position.z);
        }
    FTBL.HLib_SetCurrentTaskTo(FrbtAutoCallback);
}

void FrbtStopCallback(void *data)
{
    FTBL.spmFadeOut(0);
    FTBL.HLib_SetCurrentTaskTo(FrbtEndCallback);
    FTBL.HLib_SetupTaskDelay(30);
}

void FrbtAbortCallback(void *data)
{
    ((FrbtWork *)data)->outcome = FRBT_OUTCOME_WON;
    FrbtEndCallback(data);
}

void FrbtEndCallback(void *data)
{
    FrbtWork *work = (FrbtWork *)data;
    FTBL.DeleteMotion(work->motion);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(FRBT_RYO), 0, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(FRBT_RYO), 0, 1, 0);
    FTBL.SetScenarioCharacterKillMode(FRBT_BASE, 1);
    FTBL.EV_FreeAuthorPak(0, "0052");
    FTBL.EV_FreeAuthorPak(0, "S2BV");
    if (work->outcome == FRBT_OUTCOME_WON)
        FrbtApplyWonFlags(work);
    else if (work->outcome == FRBT_OUTCOME_LOST)
        FrbtApplyLostFlags(work);
    EventDebugUnregister(FRBT_DEBUG_SLOT);
    ET_AT_KillWork(work->characters);
    FTBL.SetPlayerControlEnabled(0);
    FTBL.LoadCutsceneAndSoundProgramForArea();
    FTBL.EV_spmBeginSampleExecute(0);
    FTBL.flddSetPointFlagOnGrid(FRBT_DEFAULT_GRID, 1);
    if (FTBL.EV_ControlPackageResources(0, 5, "FRBT", 0, 0))
        FTBL.EV_ControlPackageResources(0, 6, "FRBT", 0, 0);
    FTBL.EV_NPC_NewLoadSw_0(1);
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    FTBL.scenFreeEvent("FRBT.EV3");
    FTBL.EV_SetPlayerFlag(12, 1233, 1);
}

void FrbtAutoCallback(void *data)
{
    FrbtWork *work = (FrbtWork *)data;
    int delay;
    EventDebugCheck(FRBT_DEBUG_SLOT, 4);
    if (frbtVariant >= 0 && frbtVariant <= 11 && !FTBL.EV_GetShenmueFlag(519) && FTBL.GetPlayerStat(2) > 499 && FTBL.EV_GetShenmueFlag(486))
        FrbtMoneyHint(work);
    else {
        if (frbtVariant >= 7 && frbtVariant <= 14)
            delay = FrbtStartAuthor(work);
        else
            delay = FrbtBeginMessage(work);
        FTBL.HLib_SetCurrentTaskTo(FrbtStartBattleCallback);
        FTBL.HLib_SetupTaskDelay(delay);
    }
}

int FrbtBeginMessage(FrbtWork *work)
{
    EVT_AT_SetMot(FRBT_RYO, FRBT_IDLE_MOTION, 0.0f, 1.0f, 0x40000000, -1);
    return FrbtQueueMessages(frbtResponseMessages[frbtResponse][0], 0);
}

void FrbtMoneyHint(FrbtWork *work)
{
    int delay;
    EVT_AT_SetMot(FRBT_RYO, FRBT_IDLE_MOTION, 0.0f, 1.0f, 0x40000000, -1);
    delay = FrbtQueueMessages(frbtHintMessages, 0);
    work->outcome = FRBT_OUTCOME_HINT;
    FTBL.HLib_SetCurrentTaskTo(FrbtStopCallback);
    FTBL.HLib_SetupTaskDelay(delay);
}

void FrbtSetupBattle(FrbtWork *work)
{
    work->motion = FTBL.ReadMotiFromDirectory(g_actNumber, g_currentMap, "ZAKO.MOT");
    FTBL.shenStartFGST(10, 10, 0, 0);
    FTBL.EV_vfBattleUtil(3, 128, 1);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(FRBT_RYO), 0, 0, 0, 0, 0);
    FrbtSetupEnemies(work);
}

void FrbtStartBattleCallback(void *data)
{
    FrbtWork *work = (FrbtWork *)data;
    if (frbtVariant >= 0 && frbtVariant <= 6)
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.vfCopyPosFromEntity(1, FTBL.ctrlGetCharaByName(FRBT_RYO), 0.0f, 0.0f, 5.0f);
    FTBL.vfmQueueFightStart(work->enemyCount + 1, 2, 1.0f, 1, -1);
    FTBL.HLib_SetCurrentTaskTo(FrbtWaitBattleCallback);
}

void FrbtSetupEnemies(FrbtWork *work)
{
    int definition, i;
    for (i = 0; i < work->enemyCount; i++) {
        for (definition = 0; frbtEnemyDefinitions[definition].character && frbtEnemyDefinitions[definition].character != work->enemies[i]; definition++)
            ;
        if (i == 0 && frbtVariant >= 0 && frbtVariant <= 6) {
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(work->enemies[i]), 0, 1, 1, 120, 2);
            FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(work->enemies[i]), 0, 0x400000, 128);
            FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(work->enemies[i]), 0, 0x200000, 2048);
        } else {
            FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(work->enemies[i]), 0, 1, 1, 60, 2);
            FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(work->enemies[i]), 0, 0x400000, 128);
            FTBL.j_vfUpdateFlagsFor(FTBL.ctrlGetCharaByName(work->enemies[i]), 0, 0x200000, 2048);
        }
        FTBL.DebugLogToFileRemoved("ID : %c%c%c%c", work->enemies[i] & 0xff, work->enemies[i] >> 8 & 0xff, work->enemies[i] >> 16 & 0xff,
            work->enemies[i] >> 24);
        if (!frbtEnemyDefinitions[definition].character)
            FTBL.DebugLogToFileRemoved("  Is Not Enmdata\n");
    }
}

void FrbtWaitBattleCallback(void *data)
{
    FrbtWork *work = (FrbtWork *)data;
    int i;
    if (!FTBL.IsFightActive()) {
        if (FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(FRBT_RYO)) == 3) {
            if (frbtResponse == 3)
                FTBL.HLib_SetCurrentTaskTo(FrbtMapGetCallback);
            else if (frbtResponse <= 1)
                FTBL.HLib_SetCurrentTaskTo(FrbtVictoryAuthorCallback);
            else if (frbtResponse == 6)
                FTBL.HLib_SetCurrentTaskTo(FrbtVictoryAuthorCallback);
            else
                FTBL.HLib_SetCurrentTaskTo(FrbtWonCallback);
            FTBL.SetScreenFilmFade(15, 0, 0, 0, 0, 255, 0, 0, 0);
            FTBL.HLib_SetupTaskDelay(90);
        } else
            FTBL.HLib_SetCurrentTaskTo(FrbtLoseCallback);
        for (i = 0; i < work->enemyCount; i++)
            FrbtResetEnemy(work->enemies[i]);
    }
}

void FrbtWonCallback(void *data)
{
    FrbtWork *work = (FrbtWork *)data;
    int delay;
    work->outcome = FRBT_OUTCOME_WON;
    if (frbtResponse >= 4 && frbtResponse <= 5)
        delay = FrbtRunAway(work);
    else
        delay = FrbtWinMessage(work);
    FTBL.HLib_SetCurrentTaskTo(FrbtStopCallback);
    FTBL.HLib_SetupTaskDelay(delay);
}

const char *const frbtLoseVoices[] = {"S2BVE005", "S2BVE011", "S2BVE012", "S2BVE014", "S2BVE018", "S2BVF018", "S2BVF005", "S2BVF014"};

void FrbtLoseCallback(void *data)
{
    FrbtWork *work = (FrbtWork *)data;
    EventDateTime time;
    int i, delay = 0;
    switch (work->stage) {
    case 0:
        FTBL.SetScreenFilmFade(90, 0, 0, 0, 0, 255, 0, 0, 0);
        if (frbtVariant >= 7 && frbtVariant <= 14)
            EVT_AT_SetVoice(frbtLoseVoices[frbtVariant - 7], 0);
        else
            FrbtPlayPrefixVoice(1, "019", 0);
        delay = 150;
        work->stage++;
        break;
    case 1:
        for (i = 0; i < work->enemyCount; i++)
            FTBL.EV_DisableCharacterDisplay(work->enemies[i]);
        if (frbtOutcomes[frbtVariant].advanceTime) {
            FTBL.GetCurrentDateTime((DateTime *)&time);
            time.hour += 2;
            FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&time);
            FrbtPositionBase();
            EVT_AT_SetCam(3000, FRBT_BASE, FRBT_BASE, 0, 0, delay);
            EVT_AT_SetPos(FRBT_RYO, FRBT_BASE, 0.0f, 0.0f, 0.0f, 0);
            EVT_AT_SetAngY(FRBT_RYO, FRBT_BASE, 0.0f, 0);
            EVT_AT_SetMot(FRBT_RYO, 4935, 300.0f, 1.0f, 0x40000000, 0);
            delay = 200;
            frbtActive = 0;
            EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 30);
            EVT_AT_MenCtrl(10, 0, 0, 0, 0, 255, 0, 0, 0, delay - 10);
        }
        work->outcome = FRBT_OUTCOME_LOST;
        work->stage = 0;
        FTBL.HLib_SetCurrentTaskTo(FrbtStopCallback);
        break;
    }
    if (delay)
        FTBL.HLib_SetupTaskDelay(delay);
}

int FrbtApplyWonFlags(FrbtWork *work)
{
    if (frbtOutcomes[frbtVariant].won != -1)
        FTBL.EV_stepSetFlags(frbtOutcomes[frbtVariant].flag, frbtOutcomes[frbtVariant].won);
    switch (frbtVariant) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        if (FTBL.EV_GetShenmueFlag(519))
            FTBL.EV_stepSetFlags(519, 0);
        else if (FTBL.EV_GetShenmueFlag(116))
            FTBL.EV_stepSetFlags(116, 0);
        else if (FTBL.EV_GetShenmueFlag(117))
            FTBL.EV_stepSetFlags(117, 0);
        else
            FTBL.EV_stepSetFlags(780, 0);
        if (frbtResponse == 1)
            FTBL.SetScenarioFollowCharacter(work->enemies[0]);
        break;
    }
    return 0;
}

int FrbtApplyLostFlags(FrbtWork *work)
{
    if (FTBL.EV_GetShenmueFlag(135))
        FTBL.EV_stepSetFlags(135, 1);
    if (frbtOutcomes[frbtVariant].lost != -1)
        FTBL.EV_stepSetFlags(frbtOutcomes[frbtVariant].flag, frbtOutcomes[frbtVariant].lost);
    return 0;
}

int FrbtWinMessage(FrbtWork *work)
{
    int i;
    for (i = 0; i < work->enemyCount; i++)
        FTBL.EV_DisableCharacterDisplay(work->enemies[i]);
    EVT_AT_SetMot(FRBT_RYO, FRBT_IDLE_MOTION, 0.0f, 1.0f, 0x40000000, 0);
    return FrbtQueueMessages(frbtResponseMessages[frbtResponse][1], 0) + 60;
}

const FrbtOffset frbtStartOffsets[] = {{0.1f, -5.0f}, {1.5f, -5.4f}, {-0.6f, -6.5f}};
const char *const frbtStartVoices[] = {"S2BVE010", "S2BVE012", "S2BVF017", "S2BVE014", "S2BVE017", "S2BVE019", "S2BVF010", "S2BVF012"};

int FrbtStartAuthor(FrbtWork *work)
{
    int i;
    FrbtPositionBase();
    EVT_AT_SetCam(2000, FRBT_BASE, FRBT_BASE, 0, 0, 0);
    EVT_AT_SetPos(FRBT_RYO, FRBT_BASE, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(FRBT_RYO, FRBT_BASE, 0.0f, 0);
    EVT_AT_SetMot(FRBT_RYO, 4945, 60.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetVoice(frbtStartVoices[frbtVariant - 7], 0);
    for (i = 0; i < work->enemyCount; i++) {
        EVT_AT_SetPos(work->enemies[i], FRBT_BASE, frbtStartOffsets[i].x, 0.0f, frbtStartOffsets[i].z, 0);
        FrbtFacePlayer(work->enemies[i], FRBT_RYO);
        if (i == 0)
            EVT_AT_SetMot(work->enemies[i], 4946, 20.0f, 1.0f, 0x40000000, 0);
        else
            EVT_AT_SetMot(work->enemies[i], 4947, 0.0f + (i - 1) * 50.0f, 1.0f, 0x40000000, 0);
    }
    return 80;
}

void FrbtVictoryAuthorCallback(void *data)
{
    FrbtWork *work = (FrbtWork *)data;
    int i, delay = 0;
    switch (work->stage) {
    case 0:
        FTBL.EV_LoadAudio("0052E010", 1);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(FRBT_RYO), 1, 0, 0);
        FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(FRBT_RYO), 1, 1, 0);
        Queue_EVT_AT_CtrlHandUp_1(FRBT_RYO, 0, (void *)areaLoopHandPoses[19], 8, 0);
        Queue_EVT_AT_CtrlHandUp_1(FRBT_RYO, 1, (void *)areaLoopHandPoses[19], 8, 0);
        for (i = 1; i < work->enemyCount; i++)
            FTBL.EV_DisableCharacterDisplay(work->enemies[i]);
        FrbtPositionBase();
        EVT_AT_SetAngY(FRBT_BASE, FRBT_BASE, -90.0f, 0);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        if (frbtResponse == 0) {
            FrbtAddVoice(1, "001");
            FrbtAddVoice(1, "002");
            FrbtAddVoice(0, "001");
            FrbtAddVoice(1, "003");
            FrbtAddVoice(1, "004");
            FrbtAddVoice(0, "002");
            FrbtAddVoice(1, "005");
            FrbtAddVoice(0, "003");
            FrbtAddVoice(1, "006");
        } else if (frbtResponse == 1) {
            FrbtAddVoice(1, "001");
            FrbtAddVoice(1, "008");
            FrbtAddVoice(0, "005");
            FrbtAddVoice(1, "009");
            FrbtAddVoice(0, "008");
            FrbtAddVoice(1, "011");
        } else {
            FrbtAddVoice(1, "008");
            FrbtAddVoice(0, "001");
            FrbtAddVoice(1, "014");
            FrbtAddVoice(1, "015");
            FrbtAddVoice(1, "016");
            FrbtAddVoice(1, "017");
            FrbtAddVoice(0, "010");
            FrbtAddVoice(1, "018");
        }
        EVT_AT_SetPos(FRBT_RYO, FRBT_BASE, 0.0f, 0.0f, 0.0f, 0);
        EVT_AT_SetAngY(FRBT_RYO, FRBT_BASE, 0.0f, 0);
        EVT_AT_SetPos(work->enemies[0], FRBT_RYO, 0.1f, 0.0f, -0.88f, 0);
        EVT_AT_SetAngY(work->enemies[0], FRBT_RYO, 0.0f, 0);
        EVT_AT_SetMot(FRBT_RYO, 4931, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetMot(FRBT_RYO, 4930, 0.0f, 1.0f, 0, 100);
        if (FTBL.GetMotionRigBoneId(FTBL.scenFindMotionwork(work->enemies[0])) == 7) {
            EVT_AT_SetMot(work->enemies[0], 4934, 0.0f, 1.0f, 0x40000000, 0);
            EVT_AT_SetMot(work->enemies[0], 4933, 0.0f, 1.0f, 0, 100);
        } else {
            EVT_AT_SetMot(work->enemies[0], 4944, 0.0f, 1.0f, 0x40000000, 0);
            EVT_AT_SetMot(work->enemies[0], 4943, 0.0f, 1.0f, 0, 100);
        }
        EVT_AT_SetCam(1000, FRBT_BASE, FRBT_BASE, 0, 0, 0);
        EVT_AT_SetCam(1001, FRBT_BASE, FRBT_BASE, 0, 0, 100);
        EVT_AT_SetPoint(FRBT_RYO, work->enemies[0], 5, 3, 0, 100, 0, 0x20000000);
        FrbtQueueVoiceSequence(work->task, 10, 0, 0);
        work->stage++;
        break;
    case 1:
        EVT_AT_SetCam(1002, FRBT_BASE, FRBT_BASE, 0, 0, 0);
        EVT_AT_SetPoint(FRBT_RYO, 0, 0, 3, 0, 0, 0, 0x50000000);
        EVT_AT_SetMot(FRBT_RYO, 4929, 0.0f, 1.0f, 0, 0);
        if (FTBL.GetMotionRigBoneId(FTBL.scenFindMotionwork(work->enemies[0])) == 7)
            EVT_AT_SetMot(work->enemies[0], 4932, 0.0f, 1.0f, 0, 0);
        else
            EVT_AT_SetMot(work->enemies[0], 4942, 0.0f, 1.0f, 0, 0);
        if (frbtResponse == 1)
            delay = 60;
        else {
            if (frbtResponse == 0)
                FrbtPlayPrefixVoice(0, "004", 120);
            EVT_AT_SetCam(1003, FRBT_BASE, FRBT_BASE, 0, 0, 60);
            EVT_AT_SetCam(1004, FRBT_BASE, FRBT_BASE, 0, 0, 120);
            delay = 235;
        }
        EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, delay - 30);
        FTBL.HLib_SetCurrentTaskTo(FrbtStopCallback);
        break;
    }
    if (delay > 0)
        FTBL.HLib_SetupTaskDelay(delay);
}

void FrbtMapGetCallback(void *data)
{
    FrbtWork *work = (FrbtWork *)data;
    Vector3 position = {0.16f, 0.02f, -0.09f};
    Vector3i rotation = {20480, 0xa000, 0xc000};
    int i;
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(FRBT_RYO), 1, 0, 0);
    FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(FRBT_RYO), 1, 1, 0);
    Queue_EVT_AT_CtrlHandUp_1(FRBT_RYO, 0, (void *)areaLoopHandPoses[19], 8, 0);
    Queue_EVT_AT_CtrlHandUp_1(FRBT_RYO, 1, (void *)areaLoopHandPoses[19], 8, 0);
    Queue_EVT_AT_CtrlHandUp_1(FRBT_RYO, 0, (void *)areaLoopHandPoses[23], 8, 480);
    FTBL.EV_LoadAudio("0052E010", 1);
    for (i = 1; i < work->enemyCount; i++)
        FTBL.EV_DisableCharacterDisplay(work->enemies[i]);
    FrbtPositionBase();
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EVT_AT_SetPos(work->enemies[0], FRBT_BASE, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(work->enemies[0], FRBT_BASE, 0.0f, 0);
    EVT_AT_SetPos(FRBT_RYO, work->enemies[0], -0.06f, 0.0f, 2.6f, 0);
    EVT_AT_SetAngY(FRBT_RYO, work->enemies[0], 0.0f, 0);
    EVT_AT_SetMot(FRBT_RYO, 4936, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetMot(work->enemies[0], 4939, 0.0f, 1.0f, 0x40000000, 0);
    FrbtPlayPrefixVoice(1, "008", 60);
    FrbtAddVoice(1, "002");
    FrbtAddVoice(0, "005");
    FrbtAddVoice(1, "009");
    FrbtAddVoice(0, "006");
    FrbtAddVoice(1, "010");
    FrbtPlayPrefixVoice(1, "007", 650);
    FrbtPlayPrefixVoice(0, "007", 720);
    EVT_AT_SetCam(1100, FRBT_BASE, FRBT_BASE, 0, 0, 0);
    EVT_AT_SetCam(1101, FRBT_BASE, FRBT_BASE, 0, 0, 170);
    EVT_AT_SetCam(1103, FRBT_BASE, FRBT_BASE, 0, 0, 480);
    EVT_AT_SetCam(1104, FRBT_BASE, FRBT_BASE, 0, 0, 630);
    EVT_AT_SetPoint(FRBT_RYO, work->enemies[0], 5, 3, 0, 200, 0, 0x20000000);
    EVT_AT_SetPoint(FRBT_RYO, 0, 0, 3, 0, 455, 0, 0x10000000);
    EVT_RepeatCall(FrbtGiveReward, 0, 550, 1);
    EVT_AT_CharaDisp(FRBT_MAP_PROP, 490, 0);
    EVT_AT_CharaDisp(FRBT_MAP_PROP, 700, 1);
    Queue_EVT_AT_Cling(FRBT_MAP_PROP, FRBT_RYO, 14, &position, &rotation, 490, 0);
    Queue_EVT_AT_Cling(FRBT_MAP_PROP, 0, 0, 0, 0, 700, 1);
    EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, 770);
    FrbtQueueVoiceSequence(0, 10, 0, 200);
    FTBL.HLib_SetCurrentTaskTo(FrbtStopCallback);
    FTBL.HLib_SetupTaskDelay(800);
}

const char *const frbtExitVoices[] = {"S2BVE011", "S2BVE014", "S2BVF018", "S2BVE018", "S2BVE019", "S2BVF010", "S2BVF011", "S2BVF014"};

int FrbtRunAway(FrbtWork *work)
{
    int i, delay = 90;
    FTBL.EV_LoadAudio("0052E010", 1);
    for (i = 1; i < work->enemyCount; i++)
        FTBL.EV_DisableCharacterDisplay(work->enemies[i]);
    FrbtPositionBase();
    FTBL.SetScreenFilmFade(15, 255, 0, 0, 0, 0, 0, 0, 0);
    EVT_AT_SetPos(FRBT_RYO, FRBT_BASE, 0.0f, 0.0f, 2.0f, 0);
    EVT_AT_SetAngY(FRBT_RYO, FRBT_BASE, 0.0f, 0);
    EVT_AT_SetMot(FRBT_RYO, FRBT_IDLE_MOTION, 0.0f, 1.0f, 0x40000000, 0);
    if (FTBL.GetMotionRigBoneId(FTBL.scenFindMotionwork(work->enemies[0])) == 7) {
        EVT_AT_SetCam(1200, FRBT_BASE, FRBT_BASE, 0, 0, 0);
        EVT_AT_SetPos(work->enemies[0], FRBT_BASE, 0.0f, 0.0f, 0.0f, 0);
        EVT_AT_SetAngY(work->enemies[0], FRBT_BASE, 0.0f, 0);
        EVT_AT_SetMot(work->enemies[0], 4949, 0.0f, 1.0f, 0x40000000, 0);
    } else {
        EVT_AT_SetCam(1201, FRBT_BASE, FRBT_BASE, 0, 0, 0);
        EVT_AT_SetPos(work->enemies[0], FRBT_BASE, 0.0f, 0.0f, 0.0f, 0);
        EVT_AT_SetAngY(work->enemies[0], FRBT_BASE, 180.0f, 0);
        EVT_AT_SetMot(work->enemies[0], 4948, 0.0f, 1.0f, 0x40000000, 0);
    }
    EVT_AT_SetVoice(frbtExitVoices[frbtVariant - 7], 0);
    EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, delay - 30);
    return delay;
}

void FrbtPositionBase(void)
{
    Vector3 player;
    float nearest = 500.0f;
    short index;
    int i;
    FTBL.GetCharaPosition(FRBT_RYO, -1, &player, 0x40000000);
    index = 0;
    for (i = 0; frbtBases[frbtVariant][i].position.y > 0.0f; i++) {
        if (FTBL.CalculatePlanarDistanceBetweenPoints(&player, &frbtBases[frbtVariant][i].position) < nearest) {
            nearest = FTBL.CalculatePlanarDistanceBetweenPoints(&player, &frbtBases[frbtVariant][i].position);
            index = i;
        }
    }
    if (frbtBases[frbtVariant][index].position.y > 0.0f)
        EVT_AT_SetPos(FRBT_BASE, FRBT_INVALID, frbtBases[frbtVariant][index].position.x, frbtBases[frbtVariant][index].position.y,
            frbtBases[frbtVariant][index].position.z, 0);
    EVT_AT_SetAngY(FRBT_BASE, FRBT_INVALID, frbtBases[frbtVariant][index].yaw, 0);
}

int FrbtQueueMessages(const FrbtMessage *messages, int delay)
{
    const FrbtMessage *message = messages;
    int queued = 0, total;
    while (message->frames || message->delay) {
        queued = 1;
        QueueEventMessage(message->text, delay + message->delay, message->frames);
        message++;
    }
    if (queued) {
        message--;
        total = message->frames + message->delay + 5;
    } else
        total = 5;
    return total + delay;
}

void FrbtResetEnemy(shen_name character)
{
    FTBL.EV_EnableCharacterDisplay(character);
    SHENMUE_EVENT_BOUNDARY();
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(character), 3);
    FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(character), 7, 0);
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(character), 69, 2, 0);
    FTBL.EV_SetCharaControllerFlags(character, 3, 0);
    FTBL.EV_SetCharaControllerFlags(character, 0, 0);
    FTBL.EV_SetCharaControllerFlags(character, 2, 0);
    FTBL.EV_SetCharaControllerFlags(character, 3, 0);
    FTBL.EV_SetCharacterAlphaEnabled(character, 0);
    SHENMUE_EVENT_BOUNDARY();
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 55, 2);
}

void FrbtFacePlayer(shen_name character, shen_name target)
{
    Vector3 from, to;
    Vector3i rotation;
    float x, z;
    FTBL.GetCharaPosition(character, -1, &from, 0x40000000);
    FTBL.GetCharaPosition(target, -1, &to, 0x40000000);
    x = to.x - from.x;
    z = to.z - from.z;
    rotation.x = 0;
    rotation.y = (unsigned short)FTBL.InverseFixedAtan2(x, z);
    rotation.z = 0;
    FTBL.EV_AT_SetAngY_0(character, 0x78000000, &rotation, 0);
}

void FrbtGiveReward(void *argument)
{
    FTBL.AddInventoryItem(FRBT_REWARD_ITEM, 1, 1);
}

int frbtVoiceCount = 0;

void FrbtVoiceSequenceCallback(void *data)
{
    FrbtVoiceWork *work = (FrbtVoiceWork *)data;
    if (!FTBL.cvsHasValidConverseData(0)) {
        if (frbtVoices[work->index][0]) {
            EVT_AT_SetVoice(frbtVoices[work->index], work->delay);
            if (work->delay > 0)
                FTBL.HLib_SetupTaskDelay(work->delay);
            work->index++;
            work->ticks = 0;
            if (frbtVoices[work->index][0] || !(work->mode & FRBT_VOICE_REPEAT))
                return;
        }
        if (work->parent)
            FTBL.HLib_ResolveDeferredCallback(work->parent);
        FTBL.HLib_DestroyCurrentTask();
    } else {
        if (work->ticks++ > 600)
            FTBL.scenCvsCtrlFree(0);
        if ((work->mode & FRBT_VOICE_SKIPPABLE) && FTBL.CheckMediatedDoublePress())
            FTBL.scenCvsCtrlFree(0);
    }
}

void FrbtQueueVoiceSequence(HLibTask *parent, int delay, unsigned int mode, int timer)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(FrbtVoiceSequenceCallback, 6, 11, sizeof(FrbtVoiceWork), TASK_EVNT);
    FrbtVoiceWork *work;
    if (timer > 0)
        FTBL.HLib_SetupTimedTask(task, timer);
    work = (FrbtVoiceWork *)FTBL.HLib_GetTaskParameter(task);
    work->delay = delay;
    work->mode = mode;
    work->parent = parent;
    work->index = 0;
    work->reserved0c = 0;
    work->ticks = 0;
    frbtVoices[frbtVoiceCount][0] = 0;
    frbtVoiceCount = 0;
    if (work->parent)
        FTBL.HLib_SetupTimedTask(work->parent, 0);
}

void FrbtSetVoicePrefix(int prefix, const char *text)
{
    strcpy(frbtVoicePrefixes[prefix], text);
}

void FrbtAddVoice(int prefix, const char *suffix)
{
    strcpy(frbtVoices[frbtVoiceCount], frbtVoicePrefixes[prefix]);
    strcat(frbtVoices[frbtVoiceCount], suffix);
    frbtVoiceCount++;
}

void FrbtPlayPrefixVoice(int prefix, const char *suffix, int delay)
{
    char voice[64];
    strcpy(voice, frbtVoicePrefixes[prefix]);
    strcat(voice, suffix);
    EVT_AT_SetVoice(voice, delay);
}
#endif
#elif defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_frbt.hpp"

const shen_name frbtTrapCharacters[] = {SHEN_NAME4('A', 'G', 'O', '_'), SHEN_NAME4('E', 'N', 'F', '_'), SHEN_NAME4('R', 'N', 'P', '_'),
    SHEN_NAME4('T', 'G', 'I', '_'), FRBT_INVALID};
const FrbtMessage frbtNoMessages[] = {
    {"", 0, 0}};
const FrbtMessage frbtUnusedMessages[] = {
    {"よう、兄ちゃん。＆金貸してくんねえかな", 60, 0},
    {"●青どけよ。邪魔だ", 60, 70},
    {"なんだと、てめえ！", 60, 140},
    {"", 0, 0}};
const FrbtMessage frbtWinMessages0[] = {
    {"●青レンに会わせろ。", 60, 0},
    {"お、俺じゃ無理だ。＆もっと上の奴らに話をつけてくれ。", 90, 70},
    {"●青そいつらはどこにいる？", 60, 170},
    {"空き地だ。＆海沿いにある…", 60, 240},
    {"いつもそこに、＆たむろしてんだよ。", 60, 310},
    {"", 0, 0}};
const FrbtMessage frbtCutsceneMessages0[] = {
    {"●青海沿いの空き地か…", 60, 0},
    {"", 0, 0}};
const FrbtMessage frbtWinMessages1[] = {
    {"●青海沿いの空き地ってどこだ？", 60, 0},
    {"向こうだ。", 90, 70},
    {"", 0, 0}};
const FrbtMessage frbtIntroMessages8[] = {
    {"おい…おまえ。", 60, 0},
    {"●青俺のことか？", 60, 170},
    {"ああ…見ねえ顔だな。＆おまえ何モンだ？", 60, 240},
    {"●青俺はレンを探してる。", 90, 310},
    {"なんだと…？＆レンさんに何の用だ！？", 60, 410},
    {"レンを知ってるのか？＆教えてくれ。どこにいる？", 60, 480},
    {"てめえ…レンさんを＆気安く呼ぶんじゃねえ", 60, 550},
    {"", 0, 0}};
const FrbtMessage frbtIntroMessages9[] = {
    {"●青おい…", 60, 0},
    {"なんだ、おめぇ…", 60, 70},
    {"●青クールＪはどこにいる？", 60, 140},
    {"よそ者がいったい何の用だ！？", 60, 210},
    {"●青お前には関係ない。＆クールＪの居場所を教えろ。", 90, 280},
    {"てめえ…", 60, 380},
    {"生意気な口たたくんじゃねえぞ！", 60, 450},
    {"", 0, 0}};
const FrbtMessage frbtWinMessages4[] = {
    {"くっそぉ…", 60, 0},
    {"うわっ…！す…すんません！", 90, 70},
    {"●青レンはどこだ？", 60, 170},
    {"し…知らないっス…", 60, 240},
    {"ぐぐううっ！…ほ、本当っス！＆俺ら下っ端は知らないっス！", 90, 310},
    {"くく…クールＪなら…たぶん。", 60, 480},
    {"●青クールＪ？", 60, 550},
    {"サングラスをしているでかい男だ", 90, 620},
    {"はあっ…はあっ…", 60, 720},
    {"", 0, 0}};
const FrbtMessage frbtCutsceneMessages4[] = {
    {"●青クールＪか…", 60, 0},
    {"", 0, 0}};
const FrbtMessage frbtWinMessages5[] = {
    {"くっそぉ…", 60, 0},
    {"ぐわっ…！", 60, 70},
    {"●青クールＪはどこにいる。", 60, 140},
    {"た…たぶん、スクラップ置き場っス…", 60, 210},
    {"●青どこにある！？", 60, 280},
    {"ぐううっ！…", 60, 350},
    {"ビバリーヒルズワーフの東の端っス…＆壊れたバスが捨ててあるっス…", 90, 420},
    {"●青そこにいるんだな！？", 60, 520},
    {"は…はいっ！", 60, 590},
    {"●青案内しろ。", 60, 660},
    {"チェイス予定", 60, 720},
    {"", 0, 0}};
const FrbtMessage frbtCutsceneMessages9[] = {
    {"●青スクラップ置き場か…", 60, 0},
    {"", 0, 0}};
const FrbtMessage frbtWinMessages6[] = {
    {"ううっ…くっそ…", 60, 0},
    {"ぐわっ…", 90, 70},
    {"●青クールＪはどこにいる。", 60, 170},
    {"た…たぶん、スクラップ置き場だ…", 60, 240},
    {"●青どこにある！？", 60, 310},
    {"ぐううっ！…", 60, 370},
    {"ビバリーヒルズワーフの東の端だ…＆壊れたバスが捨ててある…", 90, 450},
    {"●青そこにいるんだな！？", 60, 550},
    {"あ…ああ…", 60, 620},
    {"はあっ…はあっ…", 60, 690},
    {"スクラップ置き場か…", 60, 760},
    {"", 0, 0}};
const FrbtMessage frbtHintMessages[] = {
    {"金を持ってるならクールＪにあいな", 60, 0},
    {"", 0, 0}};
const FrbtMessage frbtWinMessages10[] = {
    {"勝利ＳＡ", 60, 0},
    {"レンは公園にいる", 60, 70},
    {"", 0, 0}};
const FrbtMessage frbtIntroMessages2[] = {
    {"たむろ場所バトル導入ＳＡ", 60, 0},
    {"", 0, 0}};
const FrbtMessage frbtSpareMessages[] = {
    {"罠にはめるオーサ", 60, 0},
    {"ジャンク船オーサ", 60, 70},
    {"", 0, 0}};
const FrbtMessage frbtLoseMessages[] = {
    {"けっ、口ほどにもねえ。", 60, 0},
    {"まだ、この辺うろついてたら、＆次はブッ殺すぞ。", 90, 70},
    {"", 0, 0}};
const FrbtMessage frbtWinMessages8[] = {
    {"敵が逃げるＰＡ", 60, 0},
    {"", 0, 0}};
const shen_name frbtEnemyGroups[FRBT_ENCOUNTER_COUNT][6] = {
    {SHEN_NAME4('T', 'G', 'I', '_'), SHEN_NAME4('H', '1', '1', '_'), SHEN_NAME4('H', '1', '2', '_'), SHEN_NAME4('H', '1', '3', '_'), FRBT_INVALID},
    {SHEN_NAME4('S', 'Y', 'J', '_'), SHEN_NAME4('H', '0', '3', 'Q'), FRBT_INVALID},
    {SHEN_NAME4('U', 'N', 'H', '_'), SHEN_NAME4('H', '0', '9', '_'), FRBT_INVALID},
    {SHEN_NAME4('J', 'U', 'K', '_'), FRBT_INVALID},
    {SHEN_NAME4('E', 'N', 'F', '_'), SHEN_NAME4('H', '1', '1', '_'), FRBT_INVALID},
    {SHEN_NAME4('K', 'E', 'I', '_'), SHEN_NAME4('T', 'Y', 'Z', '_'), FRBT_INVALID},
    {SHEN_NAME4('K', 'E', 'I', '_'), SHEN_NAME4('T', 'Y', 'Z', '_'), FRBT_INVALID},
    {SHEN_NAME4('K', 'E', 'I', '_'), SHEN_NAME4('H', '0', '4', 'Q'), SHEN_NAME4('H', '0', '8', 'Q'), SHEN_NAME4('H', '1', '4', 'Q'), FRBT_INVALID},
    {SHEN_NAME4('U', 'N', 'H', '_'), SHEN_NAME4('H', '0', '9', 'Q'), SHEN_NAME4('H', '1', '5', 'Q'), SHEN_NAME4('H', '1', '8', 'Q'), FRBT_INVALID},
    {SHEN_NAME4('J', 'U', 'K', '_'), SHEN_NAME4('H', '0', '8', '_'), SHEN_NAME4('H', '1', '0', 'Q'), SHEN_NAME4('H', '1', '6', 'Q'), SHEN_NAME4('H', '1', '9', 'Q'), FRBT_INVALID},
    {SHEN_NAME4('T', 'G', 'I', '_'), SHEN_NAME4('H', '1', '1', 'Q'), SHEN_NAME4('H', '1', '2', 'Q'), SHEN_NAME4('H', '1', '3', 'Q'), FRBT_INVALID},
    {SHEN_NAME4('S', 'Y', 'J', '_'), SHEN_NAME4('H', '0', '3', 'Q'), SHEN_NAME4('H', '0', '4', '_'), SHEN_NAME4('H', '0', '7', 'Q'), SHEN_NAME4('H', '1', '9', '_'), FRBT_INVALID},
    {SHEN_NAME4('E', 'N', 'F', '_'), SHEN_NAME4('H', '1', '1', '_'), SHEN_NAME4('H', '1', '7', 'Q'), SHEN_NAME4('H', '2', '0', 'Q'), FRBT_INVALID},
    {SHEN_NAME4('S', 'N', 'C', '_'), SHEN_NAME4('H', '0', '1', 'Q'), SHEN_NAME4('H', '1', '0', '_'), SHEN_NAME4('H', '1', '4', '_'), FRBT_INVALID},
    {SHEN_NAME4('T', 'Y', 'Z', '_'), SHEN_NAME4('H', '0', '5', 'Q'), SHEN_NAME4('H', '0', '6', 'Q'), SHEN_NAME4('H', '1', '2', '_'), SHEN_NAME4('H', '1', '3', '_'), FRBT_INVALID},
    {SHEN_NAME4('H', '0', '5', '_'), SHEN_NAME4('H', '0', '2', '_'), SHEN_NAME4('H', '0', '9', '_'), FRBT_INVALID},
    {SHEN_NAME4('H', '0', '3', '_'), SHEN_NAME4('H', '1', '6', '_'), FRBT_INVALID},
    {SHEN_NAME4('H', '0', '6', '_'), SHEN_NAME4('H', '1', '8', '_'), SHEN_NAME4('H', '1', '5', '_'), FRBT_INVALID},
    {SHEN_NAME4('H', '0', '7', '_'), SHEN_NAME4('H', '1', '7', '_'), SHEN_NAME4('H', '2', '0', '_'), FRBT_INVALID},
    {SHEN_NAME4('H', '0', '1', '_'), SHEN_NAME4('H', '0', '2', 'Q'), FRBT_INVALID},
    {SHEN_NAME4('Y', 'O', 'H', '_'), SHEN_NAME4('R', 'I', 'K', '_'), FRBT_INVALID},
    {SHEN_NAME4('K', 'H', 'O', '_'), SHEN_NAME4('S', 'E', 'E', '_'), FRBT_INVALID},
    {SHEN_NAME4('S', 'N', 'N', '_'), SHEN_NAME4('F', 'U', 'Y', '_'), FRBT_INVALID}};
const FrbtEnemyDefinition frbtEnemyDefinitions[] = {
    {SHEN_NAME4('U', 'N', 'H', '_'), 40, 2},
    {SHEN_NAME4('S', 'Y', 'J', '_'), 40, 2},
    {SHEN_NAME4('A', 'G', 'O', '_'), 40, 2},
    {SHEN_NAME4('J', 'U', 'K', '_'), 40, 2},
    {SHEN_NAME4('E', 'N', 'F', '_'), 40, 2},
    {SHEN_NAME4('K', 'E', 'I', '_'), 40, 2},
    {SHEN_NAME4('T', 'Y', 'Z', '_'), 40, 2},
    {SHEN_NAME4('T', 'G', 'I', '_'), 40, 2},
    {SHEN_NAME4('F', 'U', 'Y', '_'), 40, 2},
    {SHEN_NAME4('S', 'E', 'E', '_'), 40, 2},
    {SHEN_NAME4('Y', 'O', 'H', '_'), 40, 2},
    {SHEN_NAME4('K', 'H', 'O', '_'), 40, 2},
    {SHEN_NAME4('R', 'I', 'K', '_'), 40, 2},
    {SHEN_NAME4('S', 'N', 'N', '_'), 40, 2},
    {0, 40, 2}};
const FrbtOutcomeRecord frbtOutcomes[FRBT_ENCOUNTER_COUNT] = {
    {170, 0, 0, 1},
    {477, 0, 0, 1},
    {168, 0, 0, 1},
    {169, 0, 0, 1},
    {478, 0, 0, 1},
    {136, 0, 1, 0},
    {119, 0, 1, 0},
    {171, 0, 0, 1},
    {172, 0, 0, 1},
    {173, 0, 0, 1},
    {192, 0, 0, 1},
    {193, 0, 0, 1},
    {194, 0, 0, 1},
    {195, 0, 0, 1},
    {464, 0, 0, 1},
    {465, 0, 0, 1},
    {466, 0, 0, 1},
    {467, 0, 0, 1},
    {468, 0, 0, 1},
    {469, 0, 0, 1},
    {164, 0, 0, 1},
    {166, 0, 0, 1},
    {165, 0, 0, 1}};
const FrbtMessage *const frbtVariantMessages[FRBT_VARIANT_COUNT][4] = {
    {frbtNoMessages, frbtWinMessages0, frbtCutsceneMessages0, frbtLoseMessages},
    {frbtNoMessages, frbtWinMessages1, frbtNoMessages, frbtLoseMessages},
    {frbtIntroMessages2, frbtNoMessages, frbtNoMessages, frbtLoseMessages},
    {frbtIntroMessages2, frbtNoMessages, frbtNoMessages, frbtLoseMessages},
    {frbtNoMessages, frbtWinMessages4, frbtCutsceneMessages4, frbtLoseMessages},
    {frbtNoMessages, frbtWinMessages5, frbtNoMessages, frbtLoseMessages},
    {frbtNoMessages, frbtWinMessages6, frbtNoMessages, frbtLoseMessages},
    {frbtNoMessages, frbtWinMessages6, frbtNoMessages, frbtLoseMessages},
    {frbtIntroMessages8, frbtWinMessages8, frbtCutsceneMessages4, frbtLoseMessages},
    {frbtIntroMessages9, frbtWinMessages8, frbtCutsceneMessages9, frbtLoseMessages},
    {frbtNoMessages, frbtWinMessages10, frbtNoMessages, frbtLoseMessages},
    {frbtNoMessages, frbtNoMessages, frbtNoMessages, frbtNoMessages}};
shen_name frbtCharacters[10] = {FRBT_RYO, FRBT_INVALID};
FrbtPlacement frbtBasePositions[FRBT_BASE_POSITION_COUNT] = {
    {{219.0f, 99.4f, 419.6f}, 215.0f},
    {{266.0f, 100.9f, 390.0f}, 90.0f},
    {{242.5f, 96.9f, 491.8f}, 90.0f},
    {{276.0f, 96.9f, 495.5f}, 90.0f},
    {{283.0f, 96.9f, 505.7f}, 90.0f},
    {{276.0f, 96.9f, 521.2f}, 145.0f},
    {{300.2f, 95.3f, 550.2f}, 90.0f},
    {{288.1f, 95.3f, 580.6f}, 30.0f},
    {{250.0f, 96.9f, 574.7f}, 0.0f},
    {{196.0f, 96.9f, 569.8f}, 90.0f}};

static int frbtActive;
static int frbtEncounter;
static int frbtVariant;

void RunFrbtEvent(int encounter)
{
    unsigned char i;
    FTBL.SetPlayerControlEnabled(0);
    frbtActive = 1;
    frbtEncounter = encounter;
    FTBL.HLib_EnqueueTaskWithoutParameter(FrbtWaitCallback, 5, 11, FRBT_WAIT_TASK);
    for (i = 0; frbtEnemyGroups[frbtEncounter][i] != FRBT_INVALID; i++) {
        EVT_AT_SetMot(frbtEnemyGroups[frbtEncounter][i], FRBT_IDLE_MOTION, 0.0f, 1.0f, 0, 0);
        FTBL.CheckWarmStartNpcRoute(frbtEnemyGroups[frbtEncounter][i]);
        frbtCharacters[i + 1] = frbtEnemyGroups[frbtEncounter][i];
    }
    frbtCharacters[i + 1] = FRBT_INVALID;
    FTBL.EV_SetPlayerFlag(12, 1234, 1);
}

void FrbtWaitCallback(void *data)
{
    HLibTask *task;
    FrbtWork *work;
    int stage;
    if (!FTBL.EV_GetPlayerFlag(11, 1234) && !(stage = FTBL.EV_GetPlayerFlag(11, 1233))) {
        task = FTBL.HLib_EnqueueTaskWithParameter(FrbtStartCallback, 5, 11, sizeof(FrbtWork), FRBT_TASK);
        work = (FrbtWork *)FTBL.HLib_GetTaskParameter(task);
        work->task = task;
        work->motion = (void *)-1;
        work->outcome = FRBT_OUTCOME_WON;
        work->stage = stage;
        FrbtSelectVariant();
        FrbtLogEnemies(work);
        FTBL.EV_SetNewSceneIndex();
        work->characters = Queue_EVT_AT_ReadChr(task, frbtCharacters);
        FTBL.HLib_DestroyCurrentTask();
    }
}

void FrbtSelectVariant(void)
{
    if (frbtEncounter == 0 || frbtEncounter == 1) {
        if (FTBL.EV_GetShenmueFlag(113))
            frbtVariant = 0;
        else
            frbtVariant = 1;
    } else if (frbtEncounter == 5)
        frbtVariant = 2;
    else if (frbtEncounter == 6)
        frbtVariant = 3;
    else if (frbtEncounter >= 7 && frbtEncounter <= 14) {
        if (FTBL.EV_GetShenmueFlag(116))
            frbtVariant = 4;
        else if (FTBL.EV_GetShenmueFlag(117))
            frbtVariant = 7;
        else if (FTBL.EV_GetShenmueFlag(519))
            frbtVariant = 10;
        else
            frbtVariant = 5;
    } else if (frbtEncounter >= 15 && frbtEncounter <= 19) {
        if (FTBL.EV_GetShenmueFlag(116))
            frbtVariant = 8;
        else
            frbtVariant = 9;
    } else
        frbtVariant = 11;
}

void FrbtLoadResources(void)
{
    FTBL.CreateEventCharacter(3, 0, FRBT_BASE);
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(7), "FRBT", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "FRBT", 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, "FRBT", 0, 0);
    FTBL.EV_spmBeginSampleExecute(1);
    if (g_currentMap == FRBT_AB00) {
        FTBL.EV_LoadCutsceneFromCurrentScene("0052E010");
        FTBL.EV_LoadAudio("0052E010", 0);
    } else {
        FTBL.EV_LoadCutsceneFromCurrentScene("0073E000");
        FTBL.EV_LoadAudio("0073E000", 0);
    }
}

void FrbtLogEnemies(FrbtWork *work)
{
    int i;
    for (i = 0; frbtEnemyGroups[frbtEncounter][i] != FRBT_INVALID; i++)
        work->enemies[i] = frbtEnemyGroups[frbtEncounter][i];
    work->enemyCount = i;
    for (i = 0; i < work->enemyCount; i++) {
        FTBL.DebugLogToFileRemoved("EnemyEntry %d: ", i);
        FTBL.DebugLogToFileRemoved("ID : %c%c%c%c", work->enemies[i] & 0xff, work->enemies[i] >> 8 & 0xff, work->enemies[i] >> 16 & 0xff,
            work->enemies[i] >> 24);
        FTBL.DebugLogToFileRemoved("\n");
    }
    FTBL.DebugLogToFileRemoved("EventIndex  %d\n", frbtEncounter);
    FTBL.DebugLogToFileRemoved("MsgNum  %d\n", frbtVariant);
}

void FrbtStartCallback(void *data)
{
    FrbtWork *work = (FrbtWork *)data;
    int i;
    FrbtLoadResources();
    for (i = 0; i < work->enemyCount; i++) {
        do {
            FTBL.EV_SetCharaControllerFlags(work->enemies[i], 2, 0);
            FTBL.EV_SetCharaControllerFlags(work->enemies[i], 3, 0);
        } while (0);
        FTBL.EV_EnableCharacterDisplay(work->enemies[i]);
        FTBL.EV_SetCharacterAlphaEnabled(work->enemies[i], 0);
        FTBL.EV_SetCharaControllerFlags(work->enemies[i], 1, 0);
    }
    QueueEventAbort(FRBT_ABORT_SLOT, FrbtAbortCallback, work, FRBT_ABORT_FRAMES);
    FTBL.HLib_SetCurrentTaskTo(FrbtIntroCallback);
}

void FrbtFinishCallback(void *data)
{
    FTBL.spmFadeOut(1);
    FTBL.HLib_SetCurrentTaskTo(FrbtExitCallback);
    FTBL.HLib_SetupTaskDelay(30);
}

void FrbtAbortCallback(void *data)
{
    ((FrbtWork *)data)->outcome = FRBT_OUTCOME_WON;
    FrbtExitCallback(data);
}

void FrbtExitCallback(void *data)
{
    FrbtWork *work = (FrbtWork *)data;
    int keepControl = 0;
    FTBL.SetScenarioCharacterKillMode(FRBT_BASE, 1);
    if (work->outcome == FRBT_OUTCOME_WON)
        keepControl = FrbtWonExit(work);
    else if (work->outcome == FRBT_OUTCOME_LOST)
        keepControl = FrbtLostExit(work);
    CancelEventAbort(FRBT_ABORT_SLOT);
    ET_AT_KillWork(work->characters);
    if (keepControl)
        FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_spmBeginSampleExecute(1);
    FTBL.LoadCutsceneAndSoundProgramForArea();
    if (FTBL.EV_ControlPackageResources(0, 5, "FRBT", 0, 0))
        FTBL.EV_ControlPackageResources(0, 6, "FRBT", 0, 0);
    FTBL.EV_NPC_NewLoadSw_0(1);
    if (frbtActive)
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    FTBL.scenFreeEvent("FRBT.EV3");
    FTBL.EV_SetPlayerFlag(12, 1233, 1);
}

void FrbtIntroCallback(void *data)
{
    FrbtWork *work = (FrbtWork *)data;
    SetEventAbortFlags(FRBT_ABORT_SLOT, 4);
    if (frbtEncounter >= 7 && frbtEncounter <= 19 && !FTBL.EV_GetShenmueFlag(519) && FTBL.GetPlayerStat(2) > 499 && FTBL.EV_GetShenmueFlag(486))
        FrbtStartHint(work);
    else
        FrbtStartFight(work);
}

void FrbtStartFight(FrbtWork *work)
{
    int delay;
    EVT_AT_SetMot(FRBT_RYO, FRBT_IDLE_MOTION, 0.0f, 1.0f, 0x40000000, -1);
    delay = FrbtQueueMessages(frbtVariantMessages[frbtVariant][0], 0);
    FTBL.HLib_SetCurrentTaskTo(FrbtFightCallback);
    FTBL.HLib_SetupTaskDelay(delay);
}

void FrbtStartHint(FrbtWork *work)
{
    int delay;
    EVT_AT_SetMot(FRBT_RYO, FRBT_IDLE_MOTION, 0.0f, 1.0f, 0x40000000, -1);
    delay = FrbtQueueMessages(frbtHintMessages, 0);
    work->outcome = FRBT_OUTCOME_HINT;
    FTBL.HLib_SetCurrentTaskTo(FrbtFinishCallback);
    FTBL.HLib_SetupTaskDelay(delay);
}

void FrbtFightCallback(void *data)
{
    FrbtWork *work = (FrbtWork *)data;
    work->motion = FTBL.ReadMotiFromDirectory(g_actNumber, g_currentMap, "ZAKO.MOT");
    FTBL.shenStartFGST(10, 10, 0, 0);
    FTBL.EV_vfBattleUtil(3, 128, 1);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(FRBT_RYO), 0, 0, 0, 0, 0);
    FrbtInitializeEnemies(work);
    FTBL.vfCopyPosFromEntity(1, FTBL.ctrlGetCharaByName(FRBT_RYO), 0.0f, 0.0f, 5.0f);
    FTBL.vfmQueueFightStart(2, 2, 1.0f, 1, -1);
    FTBL.HLib_SetCurrentTaskTo(FrbtFightWaitCallback);
}

void FrbtInitializeEnemies(FrbtWork *work)
{
    int j, i;
    for (i = 0; i < work->enemyCount; i++) {
        do {
            for (j = 0; frbtEnemyDefinitions[j].character && frbtEnemyDefinitions[j].character != work->enemies[i]; j++)
                ;
        } while (0);
        FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(work->enemies[i]), 0, 1, 1, frbtEnemyDefinitions[j].health, frbtEnemyDefinitions[j].style);
        FTBL.DebugLogToFileRemoved("ID : %c%c%c%c", work->enemies[i] & 0xff, work->enemies[i] >> 8 & 0xff, work->enemies[i] >> 16 & 0xff,
            work->enemies[i] >> 24);
        if (!frbtEnemyDefinitions[j].character)
            FTBL.DebugLogToFileRemoved("  Is Not Enmdta\n");
    }
}

void FrbtFightWaitCallback(void *data)
{
    FrbtWork *work = (FrbtWork *)data;
    int i;
    if (!FTBL.IsFightActive()) {
        if (FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(FRBT_RYO)) == 3)
            FTBL.HLib_SetCurrentTaskTo(FrbtWonCallback);
        else
            FTBL.HLib_SetCurrentTaskTo(FrbtLostCallback);
        for (i = 0; i < work->enemyCount; i++)
            FrbtResetEnemy(work->enemies[i]);
        FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.HLib_SetupTaskDelay(90);
    }
}

void FrbtWonCallback(void *data)
{
    FrbtWork *work = (FrbtWork *)data;
    int delay;
    FTBL.DeleteMotion(work->motion);
    work->outcome = FRBT_OUTCOME_WON;
    if (frbtVariant == 7)
        delay = FrbtMapCutscene(work);
    else if (frbtVariant == 0 || frbtVariant == 1 || (frbtVariant >= 4 && frbtVariant <= 6))
        delay = FrbtWinCutscene(work);
    else
        delay = FrbtWinMessage(work);
    if (frbtEncounter == 5 || frbtEncounter == 6) {
        FTBL.HLib_SetCurrentTaskTo(FrbtTrapCallback);
        FTBL.HLib_SetupTaskDelay(30);
    } else {
        FTBL.HLib_SetCurrentTaskTo(FrbtFinishCallback);
        FTBL.HLib_SetupTaskDelay(delay);
    }
}

void FrbtLostCallback(void *data)
{
    FrbtWork *work = (FrbtWork *)data;
    EventDateTime time;
    int i, delay;
    FTBL.DeleteMotion(work->motion);
    for (i = 0; i < work->enemyCount; i++)
        FTBL.EV_DisableCharacterDisplay(work->enemies[i]);
    delay = FrbtQueueMessages(frbtVariantMessages[frbtVariant][3], 0);
    if (frbtOutcomes[frbtEncounter].advanceTime) {
        FTBL.GetCurrentDateTime((DateTime *)&time);
        time.hour += 2;
        FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&time);
        EVT_AT_SetMot(FRBT_RYO, 4935, 300.0f, 1.0f, 0x40000000, delay);
        EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, delay + 30);
        delay += 260;
        frbtActive = 0;
    }
    work->outcome = FRBT_OUTCOME_LOST;
    FTBL.HLib_SetCurrentTaskTo(FrbtFinishCallback);
    FTBL.HLib_SetupTaskDelay(delay + 30);
}

int FrbtWonExit(FrbtWork *work)
{
    int keepControl = 0;
    if (frbtOutcomes[frbtEncounter].won != -1)
        FTBL.EV_stepSetFlags(frbtOutcomes[frbtEncounter].flag, frbtOutcomes[frbtEncounter].won);
    switch (frbtEncounter) {
    case 5:
    case 6:
        FTBL.EV_scenQueueLoad(g_actNumber, FRBT_ABLB, 0);
        keepControl = 1;
        frbtActive = 0;
        break;
    case 0:
    case 1:
        if (FTBL.EV_GetShenmueFlag(113))
            FTBL.EV_stepSetFlags(113, 0);
        else if (FTBL.EV_GetShenmueFlag(114))
            FTBL.EV_stepSetFlags(114, 0);
        else
            FTBL.EV_stepSetFlags(115, 0);
        break;
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        if (FTBL.EV_GetShenmueFlag(519)) {
            FTBL.EV_stepSetFlags(519, 0);
            FTBL.EV_scenQueueLoad(g_actNumber, FRBT_AR02, 1);
            keepControl = 1;
            frbtActive = 0;
        } else if (FTBL.EV_GetShenmueFlag(116))
            FTBL.EV_stepSetFlags(116, 0);
        else if (FTBL.EV_GetShenmueFlag(117))
            FTBL.EV_stepSetFlags(117, 0);
        else
            FTBL.EV_stepSetFlags(118, 0);
        break;
    }
    return keepControl;
}

int FrbtLostExit(FrbtWork *work)
{
    if (FTBL.EV_GetShenmueFlag(135))
        FTBL.EV_stepSetFlags(135, 1);
    if (frbtOutcomes[frbtEncounter].lost != -1)
        FTBL.EV_stepSetFlags(frbtOutcomes[frbtEncounter].flag, frbtOutcomes[frbtEncounter].lost);
    switch (frbtEncounter) {
    case 5:
    case 6:
        FTBL.EV_stepSetFlags(135, 0);
        FTBL.SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.EV_scenQueueLoad(g_actNumber, FRBT_WS00, 79);
        frbtActive = 0;
        break;
    }
    return 0;
}

int FrbtWinMessage(FrbtWork *work)
{
    int i;
    for (i = 0; i < work->enemyCount; i++)
        FTBL.EV_DisableCharacterDisplay(work->enemies[i]);
    EVT_AT_SetMot(FRBT_RYO, FRBT_IDLE_MOTION, 0.0f, 1.0f, 0x40000000, 0);
    return FrbtQueueMessages(frbtVariantMessages[frbtVariant][1], 0) + 60;
}

int FrbtWinCutscene(FrbtWork *work)
{
    int i, delay;
    FTBL.EV_LoadAudio("0052E010", 1);
    for (i = 1; i < work->enemyCount; i++)
        FTBL.EV_DisableCharacterDisplay(work->enemies[i]);
    FrbtPositionBase();
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    delay = FrbtQueueMessages(frbtVariantMessages[frbtVariant][1], 0);
    FrbtQueueMessages(frbtVariantMessages[frbtVariant][2], delay + 60);
    EVT_AT_SetPosAng(FRBT_RYO, FRBT_BASE, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetPosAng(work->enemies[0], FRBT_RYO, 0.35f, -0.88f, 0.0f, 0);
    EVT_AT_CharaDisp(work->enemies[0], delay + 235, 1);
    EVT_AT_SetMot(FRBT_RYO, 4931, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetMot(FRBT_RYO, 4930, 0.0f, 1.0f, 0, 100);
    EVT_AT_SetMot(FRBT_RYO, 4929, 0.0f, 1.0f, 0, delay);
    EVT_AT_SetMot(work->enemies[0], 4934, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetMot(work->enemies[0], 4933, 0.0f, 1.0f, 0, 100);
    EVT_AT_SetMot(work->enemies[0], 4932, 0.0f, 1.0f, 0, delay);
    EVT_AT_SetCam(1000, FRBT_BASE, FRBT_BASE, 0, 0, 0);
    EVT_AT_SetCam(1001, FRBT_BASE, FRBT_BASE, 0, 0, 100);
    EVT_AT_SetCam(1002, FRBT_BASE, FRBT_BASE, 0, 0, delay);
    EVT_AT_SetCam(1003, FRBT_BASE, FRBT_BASE, 0, 0, delay + 60);
    EVT_AT_SetCam(1004, FRBT_BASE, FRBT_BASE, 0, 0, delay + 120);
    EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, delay + 205);
    return delay + 235;
}

int FrbtMapCutscene(FrbtWork *work)
{
    Vector3 position = {0.16f, 0.02f, -0.09f};
    Vector3i rotation = {26760, 0x893e, 0xbd27};
    int i, end = 850;
    FTBL.EV_LoadAudio("0052E010", 1);
    for (i = 1; i < work->enemyCount; i++)
        FTBL.EV_DisableCharacterDisplay(work->enemies[i]);
    FrbtPositionBase();
    FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EVT_AT_SetPosAng(work->enemies[0], FRBT_BASE, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetPosAng(FRBT_RYO, work->enemies[0], -0.06f, 2.6f, 0.0f, 0);
    EVT_AT_SetMot(FRBT_RYO, 4936, 0.0f, 1.0f, 0x40000000, 0);
    EVT_AT_SetMot(work->enemies[0], 4939, 0.0f, 1.0f, 0x40000000, 0);
    QueueEventMessage("ううっ…くっそぉ…", 0, 60);
    QueueEventMessage("ぐわっ…！", 120, 60);
    QueueEventMessage("●青クールＪはどこにいる。", 180, 60);
    QueueEventMessage("ぐううっ！", 240, 60);
    QueueEventMessage("た…たぶん、スクラップ置き場だ…", 300, 60);
    QueueEventMessage("ビバリーヒルズワーフの東の端だ…＆壊"
                      "れたバスが捨ててある…",
        360, 60);
    QueueEventMessage("●青ウソじゃないだろうな！？", 420, 60);
    QueueEventMessage("ああ…これをやるから、＆もうかんべん"
                      "してくれ！",
        480, 60);
    QueueEventMessage("はあっ…はあっ…", 650, 60);
    QueueEventMessage("●青これを使って探すか…", 720, 60);
    EVT_AT_SetCam(1100, FRBT_BASE, FRBT_BASE, 0, 0, 0);
    EVT_AT_SetCam(1101, FRBT_BASE, FRBT_BASE, 0, 0, 170);
    EVT_AT_SetCam(1102, FRBT_BASE, FRBT_BASE, 0, 0, 420);
    EVT_AT_SetCam(1103, FRBT_BASE, FRBT_BASE, 0, 0, 480);
    EVT_AT_SetCam(1104, FRBT_BASE, FRBT_BASE, 0, 0, 630);
    EVT_RepeatCall(FrbtGiveReward, 0, 550, 1);
    EVT_AT_CharaDisp(FRBT_MAP_PROP, 490, 0);
    EVT_AT_CharaDisp(FRBT_MAP_PROP, 700, 1);
    Queue_EVT_AT_Cling(FRBT_MAP_PROP, FRBT_RYO, 14, &position, &rotation, 490, 0);
    Queue_EVT_AT_Cling(FRBT_MAP_PROP, 0, 0, 0, 0, 700, 1);
    EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, end - 30);
    return end;
}

void FrbtTrapCallback(void *data)
{
    FrbtWork *work = (FrbtWork *)data;
    switch (work->stage) {
    case 0:
        work->trapCharacters = Queue_EVT_AT_ReadChr(work->task, frbtTrapCharacters);
        work->stage++;
        break;
    case 1:
        FrbtResetEnemy(FRBT_KEI);
        FrbtResetEnemy(FRBT_TYZ);
        FTBL.EV_LoadCutsceneFromCurrentScene("0138A000");
        FTBL.EV_LoadAudio("0138A000", 0);
        FTBL.EV_FreeAuthorPak(1, "0138");
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), "TRAP1", 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        QueueSkAuthorStart(work->task, FRBT_TRAP1_AUTHOR, 0, 0, 0);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        work->stage++;
        break;
    case 2:
        FTBL.EV_ControlPackageResources(0, 6, "TRAP1", 0, 0);
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), "TRAP2", 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        QueueSkAuthorStart(work->task, FRBT_TRAP2_AUTHOR, 0, 0, 0);
        work->stage++;
        break;
    case 3:
        FTBL.EV_ControlPackageResources(0, 6, "TRAP2", 0, 0);
        FTBL.EV_FreeAuthorPak(0, "0138");
        work->stage = 0;
        FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.HLib_SetCurrentTaskTo(FrbtFinishCallback);
        break;
    }
}

void FrbtPositionBase(void)
{
    Vector3 player;
    float nearest = 500.0f;
    short index;
    int i;
    FTBL.GetCharaPosition(FRBT_RYO, -1, &player, 0x40000000);
    index = 0;
    for (i = 0; i < FRBT_BASE_POSITION_COUNT; i++) {
        if (FTBL.CalculatePlanarDistanceBetweenPoints(&player, &frbtBasePositions[i].position) < nearest) {
            nearest = FTBL.CalculatePlanarDistanceBetweenPoints(&player, &frbtBasePositions[i].position);
            index = i;
        }
    }
    EVT_AT_SetPos(FRBT_BASE, FRBT_INVALID, frbtBasePositions[index].position.x, frbtBasePositions[index].position.y, frbtBasePositions[index].position.z, 0);
    EVT_AT_SetAngY(FRBT_BASE, FRBT_INVALID, frbtBasePositions[index].yaw + 300.0f, 0);
}

int FrbtQueueMessages(const FrbtMessage *messages, int delay)
{
    const FrbtMessage *message = messages;
    int queued = 0, total;
    while (message->frames || message->delay) {
        queued = 1;
        QueueEventMessage(message->text, delay + message->delay, message->frames);
        message++;
    }
    if (queued) {
        message--;
        total = message->frames + message->delay + 5;
    } else
        total = 5;
    return total + delay;
}

void FrbtResetEnemy(shen_name character)
{
    FTBL.EV_EnableCharacterDisplay(character);
    SHENMUE_EVENT_BOUNDARY();
    FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(character), 3);
    FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(character), 7, 0);
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(character), 69, 2, 0);
    FTBL.EV_SetCharaControllerFlags(character, 3, 0);
    FTBL.EV_SetCharaControllerFlags(character, 0, 0);
    FTBL.EV_SetCharaControllerFlags(character, 2, 0);
    FTBL.EV_SetCharaControllerFlags(character, 3, 0);
    FTBL.EV_SetCharacterAlphaEnabled(character, 0);
    SHENMUE_EVENT_BOUNDARY();
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 55, 2);
}

void FrbtGiveReward(void *argument)
{
    FTBL.AddInventoryItem(FRBT_REWARD_ITEM, 1, 1);
}
#else
#include "s2/areas/area_frbt.hpp"
#include "s2/areas/area_frbt_messages.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/debug.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/fight_stage.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include <cstring>

namespace shenmue::s2::areas {
std::uint32_t frbtVariant;
std::int32_t frbtResponse, frbtVoiceCount;
char frbtVoicePrefixes[64], frbtVoiceSequence[16][12];
namespace {
shen_name frbtCharacters[7]{FRBT_RYO, FRBT_INVALID};
void FrbtInitializeCallback(void *);
void FrbtAutoCallback(void *);
void FrbtStartBattleCallback(void *);
void FrbtWaitBattleCallback(void *);
void FrbtWonCallback(void *);
void FrbtLoseCallback(void *);
void FrbtEndCallback(void *);

void FrbtChooseResponse() {
    if (frbtVariant <= 6) {
        if (EV_GetShenmueFlag(0x74))
            frbtResponse = 0;
        else if (EV_GetShenmueFlag(0x75))
            frbtResponse = 3;
        else
            frbtResponse = EV_GetShenmueFlag(0x30c) ? 1 : 6;
    } else if (frbtVariant - 7 <= 7)
        frbtResponse = EV_GetShenmueFlag(0x74) ? 4 : 5;
    else
        frbtResponse = 7;
}
void FrbtCopyEnemies(FrbtWork &work) {
    std::int32_t count = 0;
    while (FRBT_ENEMIES[frbtVariant][count].name != FRBT_INVALID.name) {
        work.enemies[count] = FRBT_ENEMIES[frbtVariant][count];
        ++count;
    }
    work.enemyCount = count;
    for (std::int32_t i = 0; i < work.enemyCount; ++i) {
        const unsigned char *name = reinterpret_cast<const unsigned char *>(&work.enemies[i]);
        EV_debug_log("FRBT\\eFRBT_et_talkbattle.c", 626, "ID : %c%c%c%c", name[0], name[1], name[2], name[3]);
    }
}
void FrbtStartCallback(void *) {
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1234) || EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1233))
        return;
    HLib::Task *task = HLib::EnqueueTaskWithParameter(FrbtInitializeCallback, 5, 0x0b, sizeof(FrbtWork), FRBT_TASK);
    FrbtWork *work = static_cast<FrbtWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->motion = -1;
    work->outcome = 1;
    work->phase = 0;
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1234, 1);
    FrbtChooseResponse();
    FrbtCopyEnemies(*work);
    EV_SetNewSceneIndex();
    work->characters = Queue_EVT_AT_ReadChr(task, frbtCharacters);
    HLib::DestroyCurrentTask();
}
void FrbtInitializeResources() {
    CreateEventCharacter(CharacterCreationHighModel, nullptr, FRBT_BASE);
    SetLowQualityHands(ctrlGetScenarioChara(FRBT_RYO));
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::PackDirectory)),
        reinterpret_cast<std::uintptr_t>("FRBT"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("FRBT"), 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("FRBT"), 0);
    EV_FreeAuthorPak(1, "0052");
    EV_FreeAuthorPak(1, "S2BV");
    const char *cutscene = g_currentMap.name == FRBT_AB00.name ? "0052E010" : "0073E000";
    EV_LoadCutsceneFromCurrentScene(cutscene);
    EV_LoadAudio(cutscene, 0);
    std::strcpy(frbtVoicePrefixes, "0052A");
    std::strcpy(frbtVoicePrefixes + 16, FRBT_OUTCOMES[frbtVariant].voicePrefix);
    EV_debug_log("FRBT\\eFRBT_et_talkbattle.c", 603, "%s %d\n", FRBT_OUTCOMES[frbtVariant].voicePrefix, frbtVariant);
    flddSetPointFlagOnGrid(FRBT_BATTLE_GRID, 1);
    flddSetPointFlagOnGrid(FRBT_DEFAULT_GRID, 0);
    EV_spmBeginSampleExecute();
}
void FrbtSetupEnemies(FrbtWork &work) {
    for (std::int32_t i = 0; i < work.enemyCount; ++i) {
        std::int32_t definition = 0;
        while (FRBT_ENEMY_DEFINITIONS[definition].character.name && FRBT_ENEMY_DEFINITIONS[definition].character.name != work.enemies[i].name)
            ++definition;
        shenInitFightData(ctrlGetScenarioChara(work.enemies[i]), nullptr, 1, 1, i == 0 && frbtVariant <= 6 ? 120 : 60, 2);
        vfUpdateFlagsFor(ctrlGetScenarioChara(work.enemies[i]), 0, 0x400000, 128);
        vfUpdateFlagsFor(ctrlGetScenarioChara(work.enemies[i]), 0, 0x200000, 2048);
        const unsigned char *name = reinterpret_cast<const unsigned char *>(&work.enemies[i]);
        EV_debug_log("FRBT\\eFRBT_et_talkbattle.c", 850, "ID : %c%c%c%c", name[0], name[1], name[2], name[3]);
        if (!FRBT_ENEMY_DEFINITIONS[definition].character.name)
            EV_debug_log("FRBT\\eFRBT_et_talkbattle.c", 851, "  Is Not Enmdata\n");
    }
}
void FrbtSetupBattle(FrbtWork &work) {
    work.motion = ReadMotiFromDirectory(g_actNumber, g_currentMap, "ZAKO.MOT");
    shenStartFGST(10, 10, 0, 0);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x80}, 1);
    shenInitFightData(ctrlGetScenarioChara(FRBT_RYO), nullptr, 0, 0, 0, 0);
    FrbtSetupEnemies(work);
}
void FrbtInitializeCallback(void *data) {
    FrbtWork &work = *static_cast<FrbtWork *>(data);
    FrbtInitializeResources();
    FrbtSetupBattle(work);
    for (std::int32_t i = 0; i < work.enemyCount; ++i) {
        EV_SetCharaControllerFlags(work.enemies[i], ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(work.enemies[i], UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_EnableCharacterDisplay(work.enemies[i]);
        EV_SetCharacterAlphaEnabled(work.enemies[i], false);
        EV_SetCharaControllerFlags(work.enemies[i], UpdateHeight, NpcCharacterControllerState::Disabled);
    }
    if (frbtVariant - 7 <= 7)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (frbtVariant <= 6)
        for (std::int32_t i = 0; i < work.enemyCount; ++i) {
            const FrbtPlacement &placement = FRBT_POSITIONS[frbtVariant * 5 + static_cast<std::uint32_t>(i)];
            EventSetPosition position{.cleanup = 0, .character = work.enemies[i], .parent = FRBT_INVALID, .position = placement.position, .delay = 0};
            EVT_AT_SetPosRot(&position);
            EventSetAngle angle{.cleanup = 0, .character = work.enemies[i], .parent = FRBT_INVALID, .degrees = placement.yaw, .delay = 0};
            EVT_AT_SetAngY_Callback(&angle);
            EV_debug_log("FRBT\\eFRBT_et_talkbattle.c", 671, "%3.2f  %3.2f %3.2f\n", placement.position.x, placement.position.y, placement.position.z);
        }
    HLib::SetCurrentTaskTo(FrbtAutoCallback);
}
void FrbtMoneyHint(FrbtWork &work) {
    EVT_AT_SetMot(FRBT_RYO, 0xf003, 0, 1, 0x40000000U, -1);
    const std::uint32_t delay = QueueEventMessageSequence(FRBT_MONEY_HINT_MESSAGES);
    work.outcome = 2;
    HLib::SetCurrentTaskTo(FrbtStopCallback);
    HLib::SetupTaskDelay(static_cast<std::int32_t>(delay));
}
void FrbtAutoCallback(void *data) {
    FrbtWork &work = *static_cast<FrbtWork *>(data);
    if (frbtVariant <= 11 && !EV_GetShenmueFlag(0x207) && GetPlayerStat(PlayerStatsId::Cash) >= 500 && EV_GetShenmueFlag(0x1e6)) {
        FrbtMoneyHint(work);
        return;
    }
    std::int32_t delay;
    if (frbtVariant - 7 > 7) {
        EVT_AT_SetMot(FRBT_RYO, 0xf003, 0, 1, 0x40000000U, -1);
        delay = static_cast<std::int32_t>(QueueEventMessageSequence(FRBT_BEGIN_MESSAGES[frbtResponse]));
    } else
        delay = FrbtStartAuthor(work);
    HLib::SetCurrentTaskTo(FrbtStartBattleCallback);
    HLib::SetupTaskDelay(delay);
}
void FrbtStartBattleCallback(void *data) {
    FrbtWork &work = *static_cast<FrbtWork *>(data);
    if (frbtVariant <= 6)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    vfCopyPosFromEntity(1, ctrlGetScenarioChara(FRBT_RYO), 0, 0, 5.0f);
    vfmQueueFightStart(static_cast<std::uint8_t>(work.enemyCount + 1), 2, 1, 1, -1);
    HLib::SetCurrentTaskTo(FrbtWaitBattleCallback);
}
void FrbtWaitBattleCallback(void *data) {
    if (IsFightActive())
        return;
    FrbtWork &work = *static_cast<FrbtWork *>(data);
    if (GetBtlStatus(ctrlGetScenarioChara(FRBT_RYO)) == 3) {
        if (frbtResponse == 3)
            HLib::SetCurrentTaskTo(FrbtMapGetCallback);
        else if (frbtResponse <= 1 || frbtResponse == 6)
            HLib::SetCurrentTaskTo(FrbtVictoryAuthorCallback);
        else
            HLib::SetCurrentTaskTo(FrbtWonCallback);
        SetScreenFilmFade(15, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetupTaskDelay(90);
    } else
        HLib::SetCurrentTaskTo(FrbtLoseCallback);
    for (std::int32_t i = 0; i < work.enemyCount; ++i)
        FrbtResetEnemyMotion(work.enemies[i]);
}
void FrbtWonCallback(void *data) {
    FrbtWork &work = *static_cast<FrbtWork *>(data);
    work.outcome = 1;
    const std::int32_t delay = static_cast<std::uint32_t>(frbtResponse - 4) <= 1 ? FrbtRunAway(work) : FrbtWinMessage(work);
    HLib::SetCurrentTaskTo(FrbtStopCallback);
    HLib::SetupTaskDelay(delay);
}
void FrbtLoseCallback(void *data) {
    FrbtWork &work = *static_cast<FrbtWork *>(data);
    std::int32_t delay = 0;
    if (!work.phase) {
        SetScreenFilmFade(90, 0, 0, 0, 0, 255, 0, 0, 0);
        if (frbtVariant - 7 > 7)
            FrbtPlayPrefixVoice(1, "024", 0);
        else
            EVT_AT_SetVoice(FRBT_LOSE_VOICES[frbtVariant - 7], 1, 0);
        ++work.phase;
        HLib::SetupTaskDelay(150);
        return;
    }
    if (work.phase != 1)
        return;
    for (std::int32_t i = 0; i < work.enemyCount; ++i)
        EV_DisableCharacterDisplay(work.enemies[i]);
    if (FRBT_OUTCOMES[frbtVariant].advanceTime) {
        DateTime time;
        shenGetCurrentDateTime(time);
        time.hour = static_cast<std::int8_t>(time.hour + 2);
        SetCurrentDateTimeTo(time);
        FrbtPositionBase();
        EVT_AT_SetCam(3000, FRBT_BASE, FRBT_BASE, 0, 0, 0);
        EventSetPosition position{.cleanup = 0, .character = FRBT_RYO, .parent = FRBT_BASE, .position = {}, .delay = 0};
        EVT_AT_SetPosRot(&position);
        EventSetAngle angle{.cleanup = 0, .character = FRBT_RYO, .parent = FRBT_BASE, .degrees = 0, .delay = 0};
        EVT_AT_SetAngY_Callback(&angle);
        EventSetMotion motion{.cleanup = 0, .character = FRBT_RYO, .animation = 4935, .frame = 300, .speed = 1, .flags = 0x40000000U, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(4935);
        HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_MenCtrl_Callback, 6, 0x0b, sizeof(EventFilmControl), AREA_EVENT_TASK);
        RegisterEventTask(task);
        EventFilmControl *film = static_cast<EventFilmControl *>(HLib::GetTaskParameter(task));
        *film = {.cleanup = 1,
            .frames = 30,
            .startAlpha = 255,
            .startRed = 0,
            .startGreen = 0,
            .startBlue = 0,
            .targetAlpha = 0,
            .targetRed = 0,
            .targetGreen = 0,
            .targetBlue = 0,
            .delay = 30};
        EVT_AT_MenCtrl({.cleanup = 1,
            .frames = 10,
            .startAlpha = 0,
            .startRed = 0,
            .startGreen = 0,
            .startBlue = 0,
            .targetAlpha = 255,
            .targetRed = 0,
            .targetGreen = 0,
            .targetBlue = 0,
            .delay = 190});
        delay = 200;
    }
    work.outcome = 0;
    work.phase = 0;
    HLib::SetCurrentTaskTo(FrbtStopCallback);
    if (delay)
        HLib::SetupTaskDelay(delay);
}
void FrbtApplyWonFlags(FrbtWork &work) {
    if (FRBT_OUTCOMES[frbtVariant].won != -1)
        EV_stepSetFlags(static_cast<std::uint8_t>(FRBT_OUTCOMES[frbtVariant].won), FRBT_OUTCOMES[frbtVariant].flag);
    if (frbtVariant <= 6) {
        const std::int32_t flag = EV_GetShenmueFlag(0x207) ? 0x207 : EV_GetShenmueFlag(0x74) ? 0x74 : EV_GetShenmueFlag(0x75) ? 0x75 : 0x30c;
        EV_stepSetFlags(0, flag);
        if (frbtResponse == 1)
            SetScenarioFollowCharacter(work.enemies[0]);
    }
}
void FrbtEndCallback(void *data) {
    FrbtWork &work = *static_cast<FrbtWork *>(data);
    DeleteMotion(work.motion);
    SetHighModelXYZ(ctrlGetScenarioChara(FRBT_RYO), 0, 0, 0);
    SetHighModelXYZ(ctrlGetScenarioChara(FRBT_RYO), 0, 1, 0);
    SetCharaKill(FRBT_BASE, 1);
    EV_FreeAuthorPak(0, "0052");
    EV_FreeAuthorPak(0, "S2BV");
    if (work.outcome == 1)
        FrbtApplyWonFlags(work);
    else if (!work.outcome) {
        if (EV_GetShenmueFlag(0x87))
            EV_stepSetFlags(1, 0x87);
        if (FRBT_OUTCOMES[frbtVariant].lost != -1)
            EV_stepSetFlags(static_cast<std::uint8_t>(FRBT_OUTCOMES[frbtVariant].lost), FRBT_OUTCOMES[frbtVariant].flag);
    }
    ET_AT_KillWork(work.characters);
    SetPlayerControlEnabled(0);
    sbtLoadProgram();
    EV_spmBeginSampleExecute();
    flddSetPointFlagOnGrid(FRBT_DEFAULT_GRID, 1);
    if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("FRBT"), 0))
        EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("FRBT"), 0);
    EV_NPC_NewLoadSw(1);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    scenFreeEvent("FRBT.EV3");
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1233, 1);
}
} // namespace
void FrbtStopCallback(void *) {
    spmFadeOut();
    HLib::SetCurrentTaskTo(FrbtEndCallback);
    HLib::SetupTaskDelay(30);
}
void InitializeFrbtEvent() {
    frbtVariant = 0;
    frbtResponse = 0;
    frbtVoiceCount = 0;
    std::memset(frbtVoicePrefixes, 0, sizeof(frbtVoicePrefixes));
    std::memset(frbtVoiceSequence, 0, sizeof(frbtVoiceSequence));
}
void RunFrbtEvent(std::uintptr_t argument) {
    SetPlayerControlEnabled(0);
    SetScenarioFollowCharacter(FRBT_EMPTY);
    frbtVariant = static_cast<std::uint32_t>(argument);
    HLib::EnqueueTaskWithoutParameter(FrbtStartCallback, 5, 0x0b, static_cast<std::int32_t>(FRBT_WAIT_TASK.name));
    std::uint8_t count = 0;
    while (FRBT_ENEMIES[frbtVariant][count].name != FRBT_INVALID.name) {
        frbtCharacters[count + 1] = FRBT_ENEMIES[frbtVariant][count];
        ++count;
    }
    frbtCharacters[count + 1] = FRBT_INVALID;
}
const ScriptAreaDefinition frbtScriptDefinition{.areaId = FRBT_AREA, .field04 = 0, .init = InitializeFrbtEvent, .main = RunFrbtEvent};
} // namespace shenmue::s2::areas
#endif
