#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_omak.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void RunOmake(void)
{
    FTBL.shenUpdateFlagEvaluation(1);
    BootOmake();
}
#else

const OmakeEntry omakeArcadeEntries[13] = {

    {QueueOmakeModuleCall, {OMAKE_NO_AREA, 3}, 0x1, 48, 0xffff, "ハングオン",
        "ライバルを抜き、チェッカーを受けろ",
        "バイクレースの決定版！", 137},

    {QueueOmakeModuleCall, {OMAKE_NO_AREA, 4}, 0x2, 49, 0xffff, "スペースハリアー",
        "軽快な操作感とスピーディーな展開の",
        "ＳＦシューティングゲームだ！", 136},

    {QueueOmakeModuleCall, {OMAKE_NO_AREA, 1}, 0x4, 46, 0xffff, "アウトラン",
        "美しく爽快なドライブゲーム",
        "全コース走破をめざせ！", 134},

    {QueueOmakeModuleCall, {OMAKE_NO_AREA, 2}, 0x8, 47, 0xffff, "アフターバーナー續",
        "戦闘機による空中戦をリアルに",
        "再現したシューティングゲーム！", 135}};
const OmakeEntry omakeVideoEntries[13] = {

    {QueueOmakeVideo, {OMAKE_NO_AREA, 1}, 0x0, 50, 0xfff0, "★極秘映像★",
        "幻のセガ・サターン版シェンムー",
        "その姿が今ここに！", 40},

    {QueueOmakeModuleCall, {OMAKE_NO_AREA, 3}, 0x1, 48, 0xffff, "ハングオン",
        "ライバルを抜き、チェッカーを受けろ",
        "バイクレースの決定版！", 137},

    {QueueOmakeModuleCall, {OMAKE_NO_AREA, 4}, 0x2, 49, 0xffff, "スペースハリアー",
        "軽快な操作感とスピーディーな展開の",
        "ＳＦシューティングゲームだ！", 136},

    {QueueOmakeModuleCall, {OMAKE_NO_AREA, 1}, 0x4, 46, 0xffff, "アウトラン",
        "美しく爽快なドライブゲーム",
        "全コース走破をめざせ！", 134},

    {QueueOmakeModuleCall, {OMAKE_NO_AREA, 2}, 0x8, 47, 0xffff, "アフターバーナー續",
        "戦闘機による空中戦をリアルに",
        "再現したシューティングゲーム！", 135}};
const OmakeEntry omakeMinigameEntries[13] = {

    {QueueOmakeSceneChange, {SHEN_NAME4('W', 'E', 'G', '0'), 8}, 0x10, 36, 0xffff, "ＱＴＥタイトル",
        "ＱＴＥの苦手な人は、",
        "これで練習しよう！", 27},

    {QueueOmakeSceneChange, {SHEN_NAME4('W', 'E', 'G', '0'), 7}, 0x20, 37, 0xffff, "エキサイトＱＴＥ２",
        "だんだん速くなっていくスピードに",
        "どこまでついてこれるかな？", 4},

    {QueueOmakeSceneChange, {SHEN_NAME4('W', 'E', 'G', '0'), 20}, 0x40, 38, 0xffff, "スロットマシン",
        "本格的スロットマシンが楽しめる",
        "狙うは７７７だ！", 148},

    {QueueOmakeSceneChange, {SHEN_NAME4('Q', '3', '0', '0'), 98}, 0x80, 39, 0xffff, "落し玉ミュージアム",
        "おもしろい台が目白押し！",
        "さあ、どれから挑戦する？", 144},

    {QueueOmakeSceneChange, {SHEN_NAME4('W', 'E', 'M', '1'), 1}, 0x100, 40, 0xffff, "ダーツセブン",
        "気軽に本格的ダーツを楽しもう",
        "", 47},

    {QueueOmakeSceneChange, {SHEN_NAME4('W', 'R', 'S', '2'), 1}, 0x200, 42, 0xffff, "ＶＳダーツ／おじいさん",
        "文 永民（ブン・エイミン）",
        "もう歳だと甘く見ると痛い目にあうぞ", 76},

    {QueueOmakeSceneChange, {SHEN_NAME4('W', 'E', 'G', '0'), 1}, 0x400, 44, 0xffff, "ＶＳダーツ／ゲーム少年",
        "宣 風林（セン・フウリン）",
        "練習相手にはちょうどいいかな？", 76},

    {QueueOmakeSceneChange, {SHEN_NAME4('A', 'K', 'S', '1'), 1}, 0x1000, 45, 0xffff, "ＶＳダーツ／キザ男",
        "柳 白陽（リュウ・ハクヨウ）",
        "キザ男の鼻っ柱をたたき折れ！", 76},

    {QueueOmakeSceneChange, {SHEN_NAME4('A', 'K', 'S', '0'), 1}, 0x800, 41, 0xffff, "ＶＳダーツ／野球男",
        "文 承佑（ブン・ショウユウ）",
        "直球勝負！　狙うはど真ん中のみ！", 48},

    {QueueOmakeSceneChange, {SHEN_NAME4('A', 'R', 'A', '0'), 1}, 0x2000, 43, 0xffff, "ＶＳダーツ／ラップ男",
        "バリー・ジョーンズ",
        "調子のいいセリフにだまされるな！", 76}};
const OmakeEntry omakeFightEntries[13] = {

    {QueueOmakeFight, {OMAKE_NO_AREA, 1}, 0x0, 35, 0xfff0, "全技巻物",
        "自分のスタイルや対戦相手によって",
        "技を選択し、闘いを有利に運ぼう！", 51},

    {QueueOmakeSceneChange, {SHEN_NAME4('A', 'B', '0', '0'), 1}, 0x4000, 24, 0xffff, "熊男",
        "厳 仁（ゲン・ジン）",
        "強力な技をその体に叩き込め！", 109},

    {QueueOmakeSceneChange, {SHEN_NAME4('Q', 'S', 'F', 'A'), 1}, 0x8000, 25, 0xffff, "モンゴル相撲",
        "ナダム／アゴン／ウズン",
        "止まるな！　そして近づけるな！", 95},

    {QueueOmakeSceneChange, {SHEN_NAME4('Q', 'T', 'B', '1'), 1}, 0x10000, 29, 0xffff, "鉄砂掌",
        "秦 漢清（シン・カンセイ）",
        "鉄砂掌は一撃必殺！　絶対食らうな！", 89},

    {QueueOmakeSceneChange, {SHEN_NAME4('Q', '1', '0', '0'), 1}, 0x20000, 30, 0xffff, "軽功",
        "周 恩将（シュウ・オンショウ）",
        "軽やかなその身を捕まえろ！", 91},

    {QueueOmakeSceneChange, {SHEN_NAME4('Q', 'S', 'F', 'D'), 1}, 0x40000, 28, 0xffff, "鉄頭功",
        "蘇 東山（ソ・トウザン）",
        "必殺の鉄頭功に気をつけろ！", 93},

    {QueueOmakeSceneChange, {SHEN_NAME4('Q', 'S', 'F', 'C'), 1}, 0x80000, 27, 0xffff, "片目の豪傑",
        "孔 周鴻（コウ・シュウコウ）",
        "手強い相手だ！　弱点を探せ！", 100},

    {QueueOmakeSceneChange, {SHEN_NAME4('Q', 'S', 'F', 'B'), 1}, 0x100000, 26, 0xffff, "化勁",
        "馬 文円（マ・ブンエン）",
        "化勁の隙を突いて、一撃を入れろ！", 97},

    {QueueOmakeSceneChange, {SHEN_NAME4('Q', 'G', 'B', 'T'), 2}, 0x200000, 32, 0xffff, "レスリング",
        "グレッグ・モア",
        "無制限２本先取！強力な投技に注意！", 150},

    {QueueOmakeSceneChange, {SHEN_NAME4('Q', 'J', 'B', 'T'), 2}, 0x400000, 31, 0xffff, "パンクラチオン",
        "ロッド・スタント",
        "無制限２本先取！打撃力は凄まじいぞ", 149},

    {QueueOmakeSceneChange, {SHEN_NAME4('Q', 'L', 'B', 'T'), 2}, 0x800000, 33, 0xffff, "截拳道",
        "春燕（シュンエン）",
        "無制限２本先取！鋭い足技は脅威だ！", 151},

    {QueueOmakeSceneChange, {SHEN_NAME4('Q', 'K', 'B', 'T'), 3}, 0x1000000, 34, 0xffff, "虎燕拳",
        "白虎（ビャッコ）",
        "無制限２本先取！連続技に気をつけろ", 152}};

static OmakeEntry omakeGameList[13], omakeMinigameList[13], omakeFightList[13];
OmakeCommand omakeColdBootCommand = {OMAKE_NO_AREA, 5};
OmakeCategory omakeCategories[4] = {{17, omakeGameList}, {16, omakeMinigameList}, {2, omakeFightList}, {0, 0}};
static int omakeDrawEnabled, omakeVideoCount;
static float omakeSpriteAnimation;

void BootOmake(void)
{
    HLibTask *main, *draw;
    OmakeWork *work;
    int index;
    FTBL.DebugLogToFileRemoved("BootUp oma'K' Menu\n");
    main = FTBL.HLib_EnqueueTaskWithParameter(OmakeMainCallback, 5, 11, sizeof(OmakeWork), TASK_EVNT);
    work = (OmakeWork *)FTBL.HLib_GetTaskParameter(main);
    draw = FTBL.HLib_EnqueueTaskWithParameter(OmakeSpriteDrawCallback, 15, 11, sizeof(OmakeDrawWork), TASK_EVNT);
    ((OmakeDrawWork *)FTBL.HLib_GetTaskParameter(draw))->owner = work;
    work->currentTask = FTBL.HLib_GetCurrentTask();
    work->mainTask = main;
    work->drawTask = draw;
    work->entry = g_entryPoint;
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_DisableCharacterDisplay(OMAKE_PLAYER);
    FTBL.CheckHelpIcon(0, 0, 0);
    FTBL.SetObjectFlagByIndex(0, 0);
    FTBL.SetWeatherUpdateEnabledTo(0);
    FTBL.SetIsSkySetup(0);
    FTBL.shenSetTimeSimulationTo(0);
    FTBL.SetRenderClockAndCompassTo(1);
    FTBL.SetPlayerGameFlags(22, 1);
    FTBL.ReinitializeLights(1);
    omakeSpriteAnimation = 0.0f;
    omakeDrawEnabled = 1;
    FTBL.SetLoadingBusy();
    work->state = 0;
    work->delay = -1;
    work->debugSprite = -1;
    InitializeOmakeSprites(work);
    InitializeOmakeTexts(work);
    FTBL.DebugLogToFileRemoved("OMAK:ENTRY = %08x\n", work->entry);
    work->selection = (OmakeSelection *)FTBL.UnknownReservedEventData;
    if (work->entry == 0) {
        work->selection->category = 0;
        for (index = 0; index < 3; index++) {
            work->selection->selected[index] = 0;
            work->selection->scroll[index] = 0;
        }
        for (index = 0; index < 16; index++) {
            FTBL.vfmDispatchControl(14, index | 0x40, 0, 1);
            FTBL.vfmDispatchControl(14, index | 0x80, 0, 1);
        }
        omakeVideoCount = FTBL.EventFunction851();
    } else
        omakeVideoCount = FTBL.EventFunction851() - 1;
    if (omakeVideoCount)
        FTBL.EV_SetPlayerFlag(12, 1018, 1);
    else
        FTBL.EV_SetPlayerFlag(12, 1018, 0);
    GiveDefaultPlayerFlags(work);
    work->animationFrames = 5;
    work->animationRemaining = 0;
    work->animationDirection = 0;
    work->refreshDescription = 1;
    if (FTBL.EV_GetPlayerFlag(11, 1018) == 0)
        InitializeOmakeList(omakeGameList, omakeArcadeEntries);
    else
        InitializeOmakeList(omakeGameList, omakeVideoEntries);
    InitializeOmakeList(omakeMinigameList, omakeMinigameEntries);
    InitializeOmakeList(omakeFightList, omakeFightEntries);
    FTBL.HLib_SetupTaskDelay(0);
}

void InitializeOmakeList(OmakeEntry *destination, const OmakeEntry *source)
{
    unsigned int flags = FTBL.GetMGOTOSIDAMA();
    int count = 0, index;
    for (index = 0; index < 13; index++)
        if (source[index].name && (flags & source[index].requiredFlags) == source[index].requiredFlags)
            destination[count++] = source[index];
    for (index = 0; index < 13; index++)
        if (source[index].name && (flags & source[index].requiredFlags) != source[index].requiredFlags)
            destination[count++] = source[index];
    for (index = 0; index < 13; index++)
        if (!source[index].name)
            destination[count++] = source[index];
}

void InitializeOmakeSprites(OmakeWork *work)
{
    int index;
    for (index = 0; index < 512; index++)
        work->sprites[index].name = 0;
}

void DeleteOmakeSprites(OmakeWork *work)
{
    int index;
    for (index = 0; index < 512; index++)
        if (work->sprites[index].name)
            DeleteOmakeSprite(work, work->sprites[index].name);
}

void CreateOmakeSprite(OmakeWork *work, shen_name name)
{
    int index;
    for (index = 0; index < 512; index++)
        if (work->sprites[index].name == name)
            break;
    if (index < 512) {
        FTBL.DebugLogToFileRemoved("EV_omak_Sprite_Create:Already Created\n");
        FTBL.EventAssertNullsub(__FILE__, 403);
    } else {
        for (index = 0; index < 512; index++)
            if (!work->sprites[index].name)
                break;
        if (index >= 512) {
            FTBL.DebugLogToFileRemoved("EV_omak_Sprite_Create:Buffer Overflow\n");
            FTBL.EventAssertNullsub(__FILE__, 410);
        }
    }
    work->sprites[index].name = name;
    FindOmakeSprite(work, name)->packet.flags = 0;
    FindOmakeSprite(work, name)->packet.colour = 0xffffffff;
    (&FindOmakeSprite(work, name)->packet)->secondaryColour = 0;
    FindOmakeSprite(work, name)->packet.tex.uv[0] = FindOmakeSprite(work, name)->packet.tex.uv[1] = 0.0f;
    FindOmakeSprite(work, name)->packet.tex.uv[2] = FindOmakeSprite(work, name)->packet.tex.uv[3] = 1.0f;
    (&FindOmakeSprite(work, name)->packet.tex)->reservedTail = 0;
}

void DeleteOmakeSprite(OmakeWork *work, shen_name name)
{
    int index;
    for (index = 0; index < 512; index++)
        if (work->sprites[index].name == name)
            break;
    if (index < 512)
        work->sprites[index].name = 0;
    else {
        FTBL.DebugLogToFileRemoved("EV_omak_Sprite_Delete:Not Found\n");
        FTBL.EventAssertNullsub(__FILE__, 430);
    }
}

OmakeSprite *FindOmakeSprite(OmakeWork *work, shen_name name)
{
    int index;
    for (index = 0; index < 512; index++)
        if (work->sprites[index].name == name)
            break;
    if (index >= 512) {
        FTBL.DebugLogToFileRemoved("EV_omak_Sprite_Search:Not Found\n");
        FTBL.EventAssertNullsub(__FILE__, 444);
        index = 0;
    }
    return &work->sprites[index];
}

void OmakeSpriteDrawCallback(void *data)
{
    if (omakeDrawEnabled)
        DrawOmakeSprites(((OmakeDrawWork *)data)->owner);
}

void DrawOmakeSprites(OmakeWork *work)
{
    int index, x, y, width, height, bit;
    unsigned int input;
    shen_name name[2];
    OmakeSpritePacket *packet;
    OmakeSelection *selection;
    for (index = 0; index < 512; index++)
        if (work->sprites[index].name)
            FTBL.UnknownEventOperation834(&work->sprites[index].packet, work->sprites[index].texture, work->sprites[index].depth + omakeSpriteAnimation);
    input = FTBL.EVT_GetControllerInput(1, 2);
    if (input & 0x800)
        work->debugSprite = ~work->debugSprite;
    if (work->debugSprite < 0)
        return;
    if (input & 0x80) {
        do {
            if (++work->debugSprite > 511) {
                work->debugSprite = 0;
                break;
            }
        } while (!work->sprites[work->debugSprite].name);
    }
    if (input & 0x40) {
        do {
            if (--work->debugSprite < 0) {
                work->debugSprite = 511;
                break;
            }
        } while (!work->sprites[work->debugSprite].name);
    }
    if (work->sprites[work->debugSprite].name) {
        packet = &work->sprites[work->debugSprite].packet;
        x = (packet->corners[0] + packet->corners[2] + packet->corners[4] + packet->corners[6]) * 0.25f;
        y = (packet->corners[1] + packet->corners[3] + packet->corners[5] + packet->corners[7]) * 0.25f;
        width = (-packet->corners[0] + packet->corners[2] - packet->corners[4] + packet->corners[6]) * 0.5f;
        height = (-packet->corners[1] - packet->corners[3] + packet->corners[5] + packet->corners[7]) * 0.5f;
        if (input & 0x8000)
            x++;
        if (input & 0x4000)
            x--;
        if (input & 0x2000)
            y++;
        if (input & 0x1000)
            y--;
        if (input & 0x200)
            width++;
        if (input & 4)
            width--;
        if (input & 2)
            height++;
        if (input & 0x400)
            height--;
        FTBL.AdjustScreenAspectRatio(packet, 5, x, y, width, height);
        name[0] = work->sprites[work->debugSprite].name;
        name[1] = 0;
        EventDebugPrintf(3, 5, "  ID:%s ", name);
        EventDebugPrintf(3, 6, "  CX:%i ", x);
        EventDebugPrintf(3, 7, "  CY:%i ", y);
        EventDebugPrintf(3, 8, "WIDT:%i ", width);
        EventDebugPrintf(3, 9, "HIGH:%i ", height);
    }
    input = FTBL.EVT_GetControllerInput(2, 2);
    if (input & 0x1000)
        work->animationFrames++;
    if (input & 0x2000)
        work->animationFrames--;
    if (input & 2) {
        for (bit = 0; bit < 32; bit++)
            if (omakeCategories[work->selection->category].entries[work->selection->selected[work->selection->category]].requiredFlags & 1 << bit)
                FTBL.SetMGOTOSIDAMA(bit, 1);
    }
    selection = work->selection;
    EventDebugPrintf(3, 11, "< >:%i ", work->animationFrames);
    EventDebugPrintf(30, 5, "M:%i ", work->selection->category);
    EventDebugPrintf(30, 6, "S:%i ", work->selection->selected[work->selection->category]);
}

void InitializeOmakeTexts(OmakeWork *work)
{
    int index;
    for (index = 0; index < 128; index++)
        work->textTextures[index].name = 0;
}

void DeleteOmakeTexts(OmakeWork *work)
{
    int index;
    for (index = 0; index < 128; index++)
        if (work->textTextures[index].name)
            QueueOmakeTextDelete(work, work->textTextures[index].name);
}

void OmakeTextDeleteCallback(void *data)
{
    OmakeDeleteTextureWork *deletion = (OmakeDeleteTextureWork *)data;
    if (deletion->frames-- <= 0) {
        FTBL.ReleaseNamedTexture(deletion->texture);
        FTBL.HLib_DestroyCurrentTask();
    }
}

void CreateOmakeText(OmakeWork *work, shen_name name, const char *text, unsigned short colour)
{
    int index;
    for (index = 0; index < 128 && work->textTextures[index].name != name; index++)
        ;
    if (index < 128) {
        FTBL.DebugLogToFileRemoved("EV_omak_MojiTexture_Create:Already Created\n");
        FTBL.EventAssertNullsub(__FILE__, 606);
    } else {
        for (index = 0; index < 128 && work->textTextures[index].name; index++)
            ;
        if (index >= 128) {
            FTBL.DebugLogToFileRemoved("EV_omak_MojiTexture_Create:Buffer Overflow\n");
            FTBL.EventAssertNullsub(__FILE__, 613);
        }
    }
    work->textTextures[index].name = name;
    work->textTextures[index].texture = FTBL.CreateCenteredWindowTextSurface(0, text, 512, colour, 0xf000, 1);
}

void QueueOmakeTextDelete(OmakeWork *work, shen_name name)
{
    int index;
    HLibTask *task;
    OmakeDeleteTextureWork *deletion;
    for (index = 0; index < 128 && work->textTextures[index].name != name; index++)
        ;
    if (index < 128) {
        work->textTextures[index].name = 0;
        task = FTBL.HLib_EnqueueTaskWithParameter(OmakeTextDeleteCallback, 5, 11, sizeof(OmakeDeleteTextureWork), TASK_EVNT);
        deletion = (OmakeDeleteTextureWork *)FTBL.HLib_GetTaskParameter(task);
        deletion->owner = work;
        deletion->frames = 2;
        deletion->texture = work->textTextures[index].texture;
    } else {
        FTBL.DebugLogToFileRemoved("EV_omak_MojiTexture_Delete:Not Found\n");
        FTBL.EventAssertNullsub(__FILE__, 652);
    }
}

int HasOmakeText(OmakeWork *work, shen_name name)
{
    int index;
    for (index = 0; index < 128 && work->textTextures[index].name != name; index++)
        ;
    if (index >= 128)
        return 0;
    return 1;
}

int FindOmakeText(OmakeWork *work, shen_name name)
{
    int index;
    for (index = 0; index < 128 && work->textTextures[index].name != name; index++)
        ;
    if (index >= 128) {
        FTBL.DebugLogToFileRemoved("EV_omak_MojiTexture_Search:Not Found\n");
        FTBL.EventAssertNullsub(__FILE__, 681);
        index = 0;
    }
    return work->textTextures[index].texture;
}

void OmakeModuleCallCallback(void *data)
{
    OmakeModuleCallWork *call = (OmakeModuleCallWork *)data;
    if (call->frames-- <= 0) {
        switch (call->command->entry) {
        case 1:
            FTBL.emuSetNextState(10, -1);
            FTBL.emuSetNextState(0, 1);
            break;
        case 2:
            FTBL.emuSetNextState(10, -1);
            FTBL.emuSetNextState(1, 1);
            break;
        case 3:
            FTBL.emuSetNextState(10, -1);
            FTBL.emuSetNextState(2, 1);
            break;
        case 4:
            FTBL.emuSetNextState(10, -1);
            FTBL.emuSetNextState(3, 1);
            break;
        case 5:
            FTBL.shenQueueChangeModuleAndScene(4, 0, 0, 0);
            break;
        default:
            FTBL.EventAssertNullsub(__FILE__, 719);
            break;
        }
        FTBL.HLib_SetupTaskDelay(0);
    }
}

void QueueOmakeModuleCall(OmakeWork *work, const OmakeCommand *command)
{
    shen_name name[2];
    HLibTask *task;
    OmakeModuleCallWork *call;
    name[0] = command->area;
    name[1] = 0;
    FTBL.DebugLogToFileRemoved("ModuleCall:[%s]%i\n", name, command->entry);
    task = FTBL.HLib_EnqueueTaskWithParameter(OmakeModuleCallCallback, 5, 11, sizeof(OmakeModuleCallWork), TASK_EVNT);
    call = (OmakeModuleCallWork *)FTBL.HLib_GetTaskParameter(task);
    call->owner = work;
    call->frames = 10;
    call->command = command;
    FTBL.HLib_SetupTaskDelay(0);
}

void QueueOmakeVideo(OmakeWork *work, const OmakeCommand *command)
{
    FTBL.EV_SetPlayerFlag(12, 1018, 0);
    FTBL.ChangeToMovieTask(command->entry);
}

void QueueOmakeSceneChange(OmakeWork *work, const OmakeCommand *command)
{
    shen_name name[2];
    name[0] = command->area;
    name[1] = 0;
    FTBL.DebugLogToFileRemoved("SceneChange:[%s]%i\n", name, command->entry);
    GiveDefaultPlayerFlags(work);
    if (command->area != OMAKE_NO_AREA)
        FTBL.EV_scenQueueLoad(4, command->area, command->entry);
}

void GiveDefaultPlayerFlags(OmakeWork *work)
{
    OmakeDateTime date;
    g_storyStep = 100000;
    date.year = 83;
    date.month = 2;
    date.day = 23;
    date.week = 1;
    date.hour = 10;
    date.minute = 0;
    date.second = 0;
    date.reserved08 = 0;
    FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&date);
    FTBL.EventFunction850(omakeVideoCount + 1);
    FTBL.EV_SetPlayerFlag(12, 1001, 1);
    FTBL.EV_SetPlayerFlag(12, 1011, 1);
    FTBL.EV_SetPlayerFlag(12, 1012, 1);
    FTBL.EV_SetPlayerFlag(12, 1013, 1);
    FTBL.EV_SetPlayerFlag(12, 1014, 1);
    FTBL.EV_SetPlayerFlag(12, 1015, 1);
    FTBL.EV_SetPlayerFlag(12, 1016, 1);
    FTBL.EV_SetPlayerFlag(12, 1017, 1);
    FTBL.SetPlayerGameFlags(4, 0);
    FTBL.EV_SetPlayerFlag(12, 123, 0);
    FTBL.SetPlayerStat(2, 500);
    FTBL.SetPlayerStat(8, 0);
}

void OmakeFightLoadCallback(void *data)
{
    OmakeFightLoadWork *load = (OmakeFightLoadWork *)data;
    if (load->delay > 0)
        load->delay--;
    else if (!FTBL.vfmDispatchControl(21, 0, 0, -1)) {
        FTBL.SetLoadingBusy();
        FTBL.HLib_ResolveDeferredCallback(load->owner->mainTask);
        FTBL.HLib_DestroyCurrentTask();
    }
}

void QueueOmakeFight(OmakeWork *work, const OmakeCommand *command)
{
    HLibTask *task;
    OmakeFightLoadWork *load;
    omakeDrawEnabled = 0;
    FTBL.FinishLoading();
    FTBL.vfmDispatchControl(21, 0, 0, 1);
    task = FTBL.HLib_EnqueueTaskWithParameter(OmakeFightLoadCallback, 5, 11, sizeof(OmakeFightLoadWork), TASK_EVNT);
    load = (OmakeFightLoadWork *)FTBL.HLib_GetTaskParameter(task);
    load->owner = work;
    load->delay = 1;
    FTBL.HLib_SetupTaskDelay(0);
}

#define OMAKE_SETUP_SPRITE(name, x, y, width, height)                                                \
    CreateOmakeSprite(work, name);                                                                   \
    (&FindOmakeSprite(work, name)->packet)->flags |= 0x400;                                           \
    FTBL.AdjustScreenAspectRatio(&FindOmakeSprite(work, name)->packet, 5, x, y, width, height)
#define OMAKE_SPRITE_TEXTURE(name, image, z)      \
    FindOmakeSprite(work, name)->texture = image; \
    FindOmakeSprite(work, name)->depth = z

void CreateOmakeMenuSprites(OmakeWork *work)
{
    DeleteOmakeSprites(work);
    CreateOmakeText(work, OMAKE_UNKNOWN, "？？？", 0xffff);
    CreateOmakeText(work, OMAKE_SPACE, " ", 0xffff);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('B', 'G', '0', '0'), 320, 240, 640, 640);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'G', '0', '0'), work->textures[23], 0.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('T', 'T', 'B', 'G'), 320, 55, 512, 512);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('T', 'T', 'B', 'G'), work->textures[10], 1.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('T', 'I', 'T', 'L'), 320, 55, 512, 512);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('T', 'I', 'T', 'L'), work->textures[8], 2.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('B', 'G', '1', '3'), 320, 129, 256, 256);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'G', '1', '3'), work->textures[0], 1.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('I', 'C', '1', '3'), 320, 129, 128, 128);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('I', 'C', '1', '3'), work->textures[17], 2.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('B', 'G', '1', '2'), 214, 127, 128, 128);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'G', '1', '2'), work->textures[0], 1.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('I', 'C', '1', '2'), 214, 127, 64, 64);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('I', 'C', '1', '2'), work->textures[16], 2.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('B', 'G', '1', '4'), 426, 127, 128, 128);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'G', '1', '4'), work->textures[0], 1.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('I', 'C', '1', '4'), 426, 127, 64, 64);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('I', 'C', '1', '4'), work->textures[2], 2.0f);
    CreateOmakeSprite(work, SHEN_NAME4('B', 'G', '1', '0'));
    (&FindOmakeSprite(work, SHEN_NAME4('B', 'G', '1', '0'))->packet)->flags |= 0x400;
    (&FindOmakeSprite(work, SHEN_NAME4('B', 'G', '1', '0'))->packet)->flags |= 0x100;
    FTBL.AdjustScreenAspectRatio(&FindOmakeSprite(work, SHEN_NAME4('B', 'G', '1', '0'))->packet, 5, 155, 129, 128, 128);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'G', '1', '0'), work->textures[1], 1.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('B', 'G', '1', '6'), 485, 129, 128, 128);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'G', '1', '6'), work->textures[1], 1.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('M', 'N', 'B', 'X'), 320, 272, 1024, 1024);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('M', 'N', 'B', 'X'), work->textures[11], 1.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('S', 'C', 'B', 'X'), 562, 272, 256, 256);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('S', 'C', 'B', 'X'), work->textures[3], 2.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('S', 'C', 'B', 'A'), 570, 220, 128, 128);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('S', 'C', 'B', 'A'), work->textures[4], 3.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('B', 'G', 'M', '0'), 376, 210, 512, 512);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'G', 'M', '0'), work->textures[5], 2.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('B', 'G', 'M', '1'), 376, 252, 512, 512);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'G', 'M', '1'), work->textures[5], 2.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('B', 'G', 'M', '2'), 376, 294, 512, 512);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'G', 'M', '2'), work->textures[5], 2.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('B', 'G', 'M', '3'), 376, 336, 512, 512);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'G', 'M', '3'), work->textures[5], 2.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('M', 'N', 'U', '0'), 376, 210, 512, 32);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('M', 'N', 'U', '0'), FindOmakeText(work, OMAKE_UNKNOWN), 3.0f);
    CreateOmakeSprite(work, SHEN_NAME4('M', 'N', 'U', '1'));
    (&FindOmakeSprite(work, SHEN_NAME4('M', 'N', 'U', '1'))->packet)->flags |= 0x400;
    FindOmakeSprite(work, SHEN_NAME4('M', 'N', 'U', '1'))->packet.secondaryColour = 0;
    FTBL.AdjustScreenAspectRatio(&FindOmakeSprite(work, SHEN_NAME4('M', 'N', 'U', '1'))->packet, 5, 376, 252, 512, 32);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('M', 'N', 'U', '1'), FindOmakeText(work, OMAKE_UNKNOWN), 3.0f);
    CreateOmakeSprite(work, SHEN_NAME4('M', 'N', 'U', '2'));
    (&FindOmakeSprite(work, SHEN_NAME4('M', 'N', 'U', '2'))->packet)->flags |= 0x400;
    (&FindOmakeSprite(work, SHEN_NAME4('M', 'N', 'U', '2'))->packet)->secondaryColour = 0;
    FTBL.AdjustScreenAspectRatio(&FindOmakeSprite(work, SHEN_NAME4('M', 'N', 'U', '2'))->packet, 5, 376, 294, 512, 32);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('M', 'N', 'U', '2'), FindOmakeText(work, OMAKE_UNKNOWN), 3.0f);
    CreateOmakeSprite(work, SHEN_NAME4('M', 'N', 'U', '3'));
    (&FindOmakeSprite(work, SHEN_NAME4('M', 'N', 'U', '3'))->packet)->flags |= 0x400;
    (&FindOmakeSprite(work, SHEN_NAME4('M', 'N', 'U', '3'))->packet)->secondaryColour = 0;
    FTBL.AdjustScreenAspectRatio(&FindOmakeSprite(work, SHEN_NAME4('M', 'N', 'U', '3'))->packet, 5, 376, 336, 512, 32);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('M', 'N', 'U', '3'), FindOmakeText(work, OMAKE_UNKNOWN), 3.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('P', 'H', 'O', 'T'), 126, 272, 256, 256);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('P', 'H', 'O', 'T'), work->textures[9], 2.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('P', 'F', 'R', 'M'), 133, 272, 256, 256);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('P', 'F', 'R', 'M'), work->textures[6], 3.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('I', 'S', 'B', 'G'), 323, 419, 512, 512);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('I', 'S', 'B', 'G'), work->textures[12], 1.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('I', 'N', 'S', '1'), 320, 395, 512, 32);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('I', 'N', 'S', '1'), FindOmakeText(work, OMAKE_UNKNOWN), 2.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('I', 'N', 'S', '2'), 320, 433, 512, 32);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('I', 'N', 'S', '2'), FindOmakeText(work, OMAKE_UNKNOWN), 2.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('K', 'E', 'Y', 'S'), 43, 395, 64, 64);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('K', 'E', 'Y', 'S'), work->textures[15], 1.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('B', 'T', 'N', 'A'), 593, 395, 64, 64);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'T', 'N', 'A'), work->textures[14], 1.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('B', 'T', 'N', 'B'), 593, 420, 64, 64);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'T', 'N', 'B'), work->textures[13], 1.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('B', 'T', 'N', 'H'), 48, 420, 128, 128);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'T', 'N', 'H'), work->textures[22], 1.0f);
}

#define OMAKE_ENTRY(category, index) (&omakeCategories[category].entries[index])
#define OMAKE_SELECTED(selection) OMAKE_ENTRY((selection)->category, (selection)->selected[(selection)->category])

void OmakeMainCallback(void *data)
{
    OmakeWork *work = (OmakeWork *)data;
    int state, refresh, category, count, index, row, offset;
    shen_name name, text;
    unsigned int flags, pressed, held;
    refresh = 0;
    flags = FTBL.GetMGOTOSIDAMA();
    omakeSpriteAnimation = 0.0f;
    omakeDrawEnabled = 1;
    if (work->delay > 0) {
        work->delay--;
        return;
    }
    work->delay = 0;
    state = work->state;
    switch (state) {
    case 0:
        work->spriteFile = FTBL.LoadLegacySceneFileWithoutSize("sprite", "OMAKE.SPR", OMAKE_AREA);
        FTBL.LoadTextureChunks(work->spriteFile, work->textures);
        work->state = 1;
        break;
    case 1:
        FTBL.HLib_FreeTaskData(work->spriteFile);
        work->state = 2;
        break;
    case 2:
        CreateOmakeMenuSprites(work);
        FTBL.EV_LoadAudio("0002S000", 0);
        FTBL.SetScreenFilmFade(1, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.EventFunction879(0);
        work->state = 3;
    case 3:
        if (!FTBL.filmCheckFadeTrackColour()) {
            FTBL.FinishLoading();
            pressed = FTBL.GetCurrentControlInput(2);
            held = FTBL.GetCurrentControlInput(1);
        } else {
            pressed = 0;
            held = 0;
            omakeSpriteAnimation = 10.0f;
        }
        if (pressed & 0x400) {
            if ((flags & OMAKE_SELECTED(work->selection)->requiredFlags) == OMAKE_SELECTED(work->selection)->requiredFlags) {
                FTBL.Loop3DSoundEffect(1015);
                OMAKE_SELECTED(work->selection)->callback(work, &OMAKE_SELECTED(work->selection)->command);
                return;
            }
            FTBL.Loop3DSoundEffect(1014);
        }
        if (pressed & 0x200) {
            FTBL.Loop3DSoundEffect(1015);
            g_hotLoadModule = 10;
            QueueOmakeModuleCall(work, &omakeColdBootCommand);
        }
        if (held & 0x4000) {
            FTBL.Loop3DSoundEffect(1007);
            work->selection->category--;
            refresh = 1;
            work->refreshDescription = 1;
            work->animationRemaining = work->animationFrames;
            work->animationDirection = 1;
        } else if (held & 0x8000) {
            FTBL.Loop3DSoundEffect(1007);
            work->selection->category++;
            refresh = 1;
            work->refreshDescription = 1;
            work->animationRemaining = work->animationFrames;
            work->animationDirection = 0;
        }
        if (work->selection->category < 0) {
            work->selection->category = -1;
            do
                work->selection->category++;
            while (omakeCategories[work->selection->category + 1].entries);
        }
        if (!omakeCategories[work->selection->category].entries)
            work->selection->category = 0;
        if (pressed & 0x1000) {
            work->selection->selected[work->selection->category]--;
            if (work->selection->selected[work->selection->category] < 0)
                work->selection->selected[work->selection->category] = 0;
            else {
                work->refreshDescription = 1;
                FTBL.Loop3DSoundEffect(1007);
            }
        } else if (pressed & 0x2000) {
            work->selection->selected[work->selection->category]++;
            if (!OMAKE_SELECTED(work->selection)->name) {
                work->selection->selected[work->selection->category] = -1;
                do
                    work->selection->selected[work->selection->category]++;
                while (OMAKE_SELECTED(work->selection)[1].name);
            } else {
                work->refreshDescription = 1;
                FTBL.Loop3DSoundEffect(1007);
            }
        }
        category = work->selection->category;
        FindOmakeSprite(work, SHEN_NAME4('I', 'C', '1', '3'))->texture = work->textures[omakeCategories[category].icon];
        FindOmakeSprite(work, SHEN_NAME4('I', 'C', '1', '3'))->depth = 2.0f;
        category = work->selection->category - 1;
        if (category < 0) {
            category = -1;
            do
                category++;
            while (omakeCategories[category + 1].entries);
        }
        FindOmakeSprite(work, SHEN_NAME4('I', 'C', '1', '2'))->texture = work->textures[omakeCategories[category].icon];
        FindOmakeSprite(work, SHEN_NAME4('I', 'C', '1', '2'))->depth = 2.0f;
        category = work->selection->category + 1;
        if (!omakeCategories[category].entries)
            category = 0;
        FindOmakeSprite(work, SHEN_NAME4('I', 'C', '1', '4'))->texture = work->textures[omakeCategories[category].icon];
        FindOmakeSprite(work, SHEN_NAME4('I', 'C', '1', '4'))->depth = 2.0f;
        offset = work->selection->selected[work->selection->category] - work->selection->scroll[work->selection->category];
        if (offset < 0)
            work->selection->scroll[work->selection->category] = work->selection->selected[work->selection->category];
        if (offset > 3)
            work->selection->scroll[work->selection->category] = work->selection->selected[work->selection->category] - 3;
        FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '0'))->texture = work->textures[5];
        FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '0'))->depth = 2.0f;
        FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '1'))->texture = work->textures[5];
        FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '1'))->depth = 2.0f;
        FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '2'))->texture = work->textures[5];
        FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '2'))->depth = 2.0f;
        FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '3'))->texture = work->textures[5];
        FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '3'))->depth = 2.0f;
        switch (work->selection->selected[work->selection->category] - work->selection->scroll[work->selection->category]) {
        case 0:
            FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '0'))->texture = work->textures[21];
            FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '0'))->depth = 2.0f;
            break;
        case 1:
            FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '1'))->texture = work->textures[21];
            FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '1'))->depth = 2.0f;
            break;
        case 2:
            FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '2'))->texture = work->textures[21];
            FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '2'))->depth = 2.0f;
            break;
        case 3:
            FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '3'))->texture = work->textures[21];
            FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '3'))->depth = 2.0f;
            break;
        default:
            FTBL.EventAssertNullsub(__FILE__, 1299);
            break;
        }
        if (work->animationRemaining > 0) {
            work->animationRemaining--;
            if (work->animationDirection == 0) {
                FindOmakeSprite(work, SHEN_NAME4('B', 'G', '1', '0'))->texture = work->textures[1];
                FindOmakeSprite(work, SHEN_NAME4('B', 'G', '1', '0'))->depth = 1.0f;
                FindOmakeSprite(work, SHEN_NAME4('B', 'G', '1', '6'))->texture = work->textures[20];
                FindOmakeSprite(work, SHEN_NAME4('B', 'G', '1', '6'))->depth = 1.0f;
            } else {
                FindOmakeSprite(work, SHEN_NAME4('B', 'G', '1', '0'))->texture = work->textures[20];
                FindOmakeSprite(work, SHEN_NAME4('B', 'G', '1', '0'))->depth = 1.0f;
                FindOmakeSprite(work, SHEN_NAME4('B', 'G', '1', '6'))->texture = work->textures[1];
                FindOmakeSprite(work, SHEN_NAME4('B', 'G', '1', '6'))->depth = 1.0f;
            }
        } else {
            FindOmakeSprite(work, SHEN_NAME4('B', 'G', '1', '0'))->texture = work->textures[1];
            FindOmakeSprite(work, SHEN_NAME4('B', 'G', '1', '0'))->depth = 1.0f;
            FindOmakeSprite(work, SHEN_NAME4('B', 'G', '1', '6'))->texture = work->textures[1];
            FindOmakeSprite(work, SHEN_NAME4('B', 'G', '1', '6'))->depth = 1.0f;
        }
        count = -1;
        do
            count++;
        while (omakeCategories[work->selection->category].entries[count + 1].name);
        if (count == 0)
            count = 1;
        FTBL.AdjustScreenAspectRatio(&FindOmakeSprite(work, SHEN_NAME4('S', 'C', 'B', 'A'))->packet, 5, 570,
            210 + work->selection->selected[work->selection->category] * 123 / count, 128, 128);
        if ((flags & OMAKE_SELECTED(work->selection)->requiredFlags) == OMAKE_SELECTED(work->selection)->requiredFlags) {
            FindOmakeSprite(work, SHEN_NAME4('P', 'H', 'O', 'T'))->texture = work->textures[OMAKE_SELECTED(work->selection)->photoTexture];
            FindOmakeSprite(work, SHEN_NAME4('P', 'H', 'O', 'T'))->depth = 2.0f;
            FTBL.EV_EventSetUpHelpIcon_0((unsigned char)OMAKE_SELECTED(work->selection)->helpTopic, 0);
        } else {
            FindOmakeSprite(work, SHEN_NAME4('P', 'H', 'O', 'T'))->texture = work->textures[9];
            FindOmakeSprite(work, SHEN_NAME4('P', 'H', 'O', 'T'))->depth = 2.0f;
            FTBL.EV_EventSetUpHelpIcon_0(130, 0);
        }
        if (work->refreshDescription) {
            work->refreshDescription = 0;
            if (HasOmakeText(work, SHEN_NAME4('I', 'N', 'S', '1')))
                QueueOmakeTextDelete(work, SHEN_NAME4('I', 'N', 'S', '1'));
            CreateOmakeText(work, SHEN_NAME4('I', 'N', 'S', '1'), OMAKE_SELECTED(work->selection)->description, 0xffff);
            if (HasOmakeText(work, SHEN_NAME4('I', 'N', 'S', '2')))
                QueueOmakeTextDelete(work, SHEN_NAME4('I', 'N', 'S', '2'));
            CreateOmakeText(work, SHEN_NAME4('I', 'N', 'S', '2'), OMAKE_SELECTED(work->selection)->descriptionEnd, 0xffff);
            if ((flags & OMAKE_SELECTED(work->selection)->requiredFlags) == OMAKE_SELECTED(work->selection)->requiredFlags) {
                FindOmakeSprite(work, SHEN_NAME4('I', 'N', 'S', '1'))->texture = FindOmakeText(work, SHEN_NAME4('I', 'N', 'S', '1'));
                FindOmakeSprite(work, SHEN_NAME4('I', 'N', 'S', '1'))->depth = 2.0f;
                FindOmakeSprite(work, SHEN_NAME4('I', 'N', 'S', '2'))->texture = FindOmakeText(work, SHEN_NAME4('I', 'N', 'S', '2'));
                FindOmakeSprite(work, SHEN_NAME4('I', 'N', 'S', '2'))->depth = 2.0f;
                if (*OMAKE_SELECTED(work->selection)->descriptionEnd == 0) {
                    FTBL.AdjustScreenAspectRatio(&FindOmakeSprite(work, SHEN_NAME4('I', 'N', 'S', '1'))->packet, 5, 320, 414, 512, 32);
                    FTBL.AdjustScreenAspectRatio(&FindOmakeSprite(work, SHEN_NAME4('I', 'N', 'S', '2'))->packet, 5, 320, 414, 512, 32);
                } else {
                    FTBL.AdjustScreenAspectRatio(&FindOmakeSprite(work, SHEN_NAME4('I', 'N', 'S', '1'))->packet, 5, 320, 395, 512, 32);
                    FTBL.AdjustScreenAspectRatio(&FindOmakeSprite(work, SHEN_NAME4('I', 'N', 'S', '2'))->packet, 5, 320, 433, 512, 32);
                }
            } else {
                FindOmakeSprite(work, SHEN_NAME4('I', 'N', 'S', '1'))->texture = FindOmakeText(work, OMAKE_SPACE);
                FindOmakeSprite(work, SHEN_NAME4('I', 'N', 'S', '1'))->depth = 2.0f;
                FindOmakeSprite(work, SHEN_NAME4('I', 'N', 'S', '2'))->texture = FindOmakeText(work, OMAKE_SPACE);
                FindOmakeSprite(work, SHEN_NAME4('I', 'N', 'S', '2'))->depth = 2.0f;
                FTBL.AdjustScreenAspectRatio(&FindOmakeSprite(work, SHEN_NAME4('I', 'N', 'S', '1'))->packet, 5, 320, 414, 512, 32);
                FTBL.AdjustScreenAspectRatio(&FindOmakeSprite(work, SHEN_NAME4('I', 'N', 'S', '2'))->packet, 5, 320, 414, 512, 32);
            }
        }
        if (refresh) {
            for (index = 0; index < 20; index++) {
                name = OMAKE_MENU_ROW(work->selection->scroll[work->selection->category] + index);
                if (HasOmakeText(work, name))
                    QueueOmakeTextDelete(work, name);
            }
        }
        name = OMAKE_MENU_ROW(work->selection->scroll[work->selection->category] - 1);
        if (HasOmakeText(work, name))
            QueueOmakeTextDelete(work, name);
        name = OMAKE_MENU_ROW(work->selection->scroll[work->selection->category] + 4);
        if (HasOmakeText(work, name))
            QueueOmakeTextDelete(work, name);
        for (row = 0; row < 4; row++) {
            text = OMAKE_MENU_ROW(work->selection->scroll[work->selection->category] + row);
            if (!HasOmakeText(work, text))
                CreateOmakeText(work, text, OMAKE_ENTRY(work->selection->category, work->selection->scroll[work->selection->category] + row)->name,
                    OMAKE_ENTRY(work->selection->category, work->selection->scroll[work->selection->category] + row)->colour);
            if ((flags & OMAKE_ENTRY(work->selection->category, work->selection->scroll[work->selection->category] + row)->requiredFlags)
                == OMAKE_ENTRY(work->selection->category, work->selection->scroll[work->selection->category] + row)->requiredFlags) {
                FindOmakeSprite(work, OMAKE_MENU_ROW(row + '0'))->texture = FindOmakeText(work, text);
                FindOmakeSprite(work, OMAKE_MENU_ROW(row + '0'))->depth = 3.0f;
            } else {
                FindOmakeSprite(work, OMAKE_MENU_ROW(row + '0'))->texture = FindOmakeText(work, OMAKE_UNKNOWN);
                FindOmakeSprite(work, OMAKE_MENU_ROW(row + '0'))->depth = 3.0f;
            }
        }
        break;
    }
    if (state != work->state)
        FTBL.DebugLogToFileRemoved("state %i -> %i\n", state, work->state);
}
#endif
#endif
