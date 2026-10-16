#pragma once
#include "s2/areas/area_omak.hpp"

namespace shenmue::s2::areas {
inline const OmakeEntry OMAKE_ARCADE_ENTRIES[13]{
    {.callback = QueueOmakeModuleCall,
        .command = {.area = shen_name{0x20202020U}, .entry = 3},
        .requiredFlags = 0x1,
        .photoTexture = 48,
        .colour = 0xffff,
        .reserved1c = 0,
        .name = "UIMCOLTY01", // @text EN: "Hang-On" | JA: "ハングオン"
        .description = "UIMCOLY010", // @text EN: "Keep your speed up to win!" | JA: "ライバルを抜き、チェッカーを受けろ！"
        .descriptionEnd = nullptr,
        .helpTopic = 0x89},
    {.callback = QueueOmakeModuleCall,
        .command = {.area = shen_name{0x20202020U}, .entry = 4},
        .requiredFlags = 0x2,
        .photoTexture = 49,
        .colour = 0xffff,
        .reserved1c = 0,
        .name = "UIMCOLTY02", // @text EN: "Space Harrier" | JA: "スペースハリアー"
        .description = "UIMCOLY020", // @text EN: "A SciFi shooting game with easy" | JA: "軽快な操作感とスピーディーな展開の"
        .descriptionEnd = "UIMCOLY021", // @text EN: "controls and dramatic speed." | JA: "ＳＦシューティングゲームだ！"
        .helpTopic = 0x88},
    {.callback = QueueOmakeModuleCall,
        .command = {.area = shen_name{0x20202020U}, .entry = 1},
        .requiredFlags = 0x4,
        .photoTexture = 46,
        .colour = 0xffff,
        .reserved1c = 0,
        .name = "UIMCOLTY03", // @text EN: "OutRun" | JA: "アウトラン"
        .description = "UIMCOLY030", // @text EN: "An exhilarating driving game." | JA: "美しく爽快なドライブゲーム！"
        .descriptionEnd = "UIMCOLY031", // @text EN: "Try to complete all courses!" | JA: "全コース走破をめざせ！"
        .helpTopic = 0x86},
    {.callback = QueueOmakeModuleCall,
        .command = {.area = shen_name{0x20202020U}, .entry = 2},
        .requiredFlags = 0x8,
        .photoTexture = 47,
        .colour = 0xffff,
        .reserved1c = 0,
        .name = "UIMCOLTY04", // @text EN: "After Burner II" | JA: "アフターバーナーII"
        .description = "UIMCOLY040", // @text EN: "A shooting game that simulates" | JA: "戦闘機による空中戦を"
        .descriptionEnd = "UIMCOLY041", // @text EN: "aerial combat." | JA: "リアルに再現したシューティングゲーム！"
        .helpTopic = 0x87},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
};
inline const OmakeEntry OMAKE_VIDEO_ENTRIES[13]{
    {.callback = QueueOmakeVideo,
        .command = {.area = shen_name{0x20202020U}, .entry = 1},
        .requiredFlags = 0x0,
        .photoTexture = 50,
        .colour = 0xfff0,
        .reserved1c = 0,
        .name = "CLASSIFIED IMAGES",
        .description = "The Sega Saturn version of",
        .descriptionEnd = "Shenmue -- never before shown!",
        .helpTopic = 0x28},
    {.callback = QueueOmakeModuleCall,
        .command = {.area = shen_name{0x20202020U}, .entry = 3},
        .requiredFlags = 0x1,
        .photoTexture = 48,
        .colour = 0xffff,
        .reserved1c = 0,
        .name = "UIMCOLTY01", // @text EN: "Hang-On" | JA: "ハングオン"
        .description = "UIMCOLY010", // @text EN: "Keep your speed up to win!" | JA: "ライバルを抜き、チェッカーを受けろ！"
        .descriptionEnd = nullptr,
        .helpTopic = 0x89},
    {.callback = QueueOmakeModuleCall,
        .command = {.area = shen_name{0x20202020U}, .entry = 4},
        .requiredFlags = 0x2,
        .photoTexture = 49,
        .colour = 0xffff,
        .reserved1c = 0,
        .name = "UIMCOLTY02", // @text EN: "Space Harrier" | JA: "スペースハリアー"
        .description = "UIMCOLY020", // @text EN: "A SciFi shooting game with easy" | JA: "軽快な操作感とスピーディーな展開の"
        .descriptionEnd = "UIMCOLY021", // @text EN: "controls and dramatic speed." | JA: "ＳＦシューティングゲームだ！"
        .helpTopic = 0x88},
    {.callback = QueueOmakeModuleCall,
        .command = {.area = shen_name{0x20202020U}, .entry = 1},
        .requiredFlags = 0x4,
        .photoTexture = 46,
        .colour = 0xffff,
        .reserved1c = 0,
        .name = "UIMCOLTY03", // @text EN: "OutRun" | JA: "アウトラン"
        .description = "UIMCOLY030", // @text EN: "An exhilarating driving game." | JA: "美しく爽快なドライブゲーム！"
        .descriptionEnd = "UIMCOLY031", // @text EN: "Try to complete all courses!" | JA: "全コース走破をめざせ！"
        .helpTopic = 0x86},
    {.callback = QueueOmakeModuleCall,
        .command = {.area = shen_name{0x20202020U}, .entry = 2},
        .requiredFlags = 0x8,
        .photoTexture = 47,
        .colour = 0xffff,
        .reserved1c = 0,
        .name = "UIMCOLTY04", // @text EN: "After Burner II" | JA: "アフターバーナーII"
        .description = "UIMCOLY040", // @text EN: "A shooting game that simulates" | JA: "戦闘機による空中戦を"
        .descriptionEnd = "UIMCOLY041", // @text EN: "aerial combat." | JA: "リアルに再現したシューティングゲーム！"
        .helpTopic = 0x87},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
};
inline const OmakeEntry OMAKE_MINIGAME_ENTRIES[13]{
    {.callback = QueueOmakeSceneChange,
        .command = {.area = SHEN_NAME(WEG0), .entry = 8},
        .requiredFlags = 0x10,
        .photoTexture = 36,
        .colour = 0xffff,
        .reserved1c = 0,
        .name = "UIMCOLTS01", // @text EN: "QTE Title" | JA: "ＱＴＥタイトル"
        .description = "UIMCOLS010", // @text EN: "Improve your QTE skills" | JA: "ＱＴＥの苦手な人は、"
        .descriptionEnd = "UIMCOLS011", // @text EN: "by playing this game!" | JA: "これで練習しよう！"
        .helpTopic = 0x1b},
    {.callback = QueueOmakeSceneChange,
        .command = {.area = SHEN_NAME(WEG0), .entry = 7},
        .requiredFlags = 0x20,
        .photoTexture = 37,
        .colour = 0xffff,
        .reserved1c = 0,
        .name = "UIMCOLTS02", // @text EN: "Excite QTE2" | JA: "エキサイトＱＴＥ２"
        .description = "UIMCOLS020", // @text EN: "Can you keep up with" | JA: "だんだん速くなっていくスピードに"
        .descriptionEnd = "UIMCOLS021", // @text EN: "the increasing speed?" | JA: "どこまでついてこれるかな？"
        .helpTopic = 0x4},
    {.callback = QueueOmakeSceneChange,
        .command = {.area = SHEN_NAME(WEG0), .entry = 20},
        .requiredFlags = 0x40,
        .photoTexture = 38,
        .colour = 0xffff,
        .reserved1c = 0,
        .name = "UIMCOLTS03", // @text EN: "Slot Machine" | JA: "スロットマシン"
        .description = "UIMCOLS030", // @text EN: "Enjoy playing with the slot" | JA: "本格的スロットマシンが楽しめる！"
        .descriptionEnd = "UIMCOLS031", // @text EN: "machine simulator. Aim for 777!" | JA: "狙うは７７７だ！"
        .helpTopic = 0x94},
    {.callback = QueueOmakeSceneChange,
        .command = {.area = SHEN_NAME(Q300), .entry = 98},
        .requiredFlags = 0x80,
        .photoTexture = 39,
        .colour = 0xffff,
        .reserved1c = 0,
        .name = "UIMCOLTS04", // @text EN: "Lucky Hit Museum" | JA: "落し玉ミュージアム"
        .description = "UIMCOLS040", // @text EN: "Here's a collection of all kinds" | JA: "おもしろい台が目白押し！"
        .descriptionEnd = "UIMCOLS041", // @text EN: "of different Lucky Hit stands!" | JA: "さあ、どれから挑戦する？"
        .helpTopic = 0x90},
    {.callback = QueueOmakeSceneChange,
        .command = {.area = SHEN_NAME(WEM1), .entry = 1},
        .requiredFlags = 0x100,
        .photoTexture = 40,
        .colour = 0xffff,
        .reserved1c = 0,
        .name = "UIMCOLTS05", // @text EN: "Darts Seven" | JA: "ダーツセブン"
        .description = "UIMCOLS050", // @text EN: "Relax and enjoy a game of darts." | JA: "気軽に本格的ダーツを楽しもう！"
        .descriptionEnd = nullptr,
        .helpTopic = 0x2f},
    {.callback = QueueOmakeSceneChange,
        .command = {.area = SHEN_NAME(WRS2), .entry = 1},
        .requiredFlags = 0x200,
        .photoTexture = 42,
        .colour = 0xffff,
        .reserved1c = 0,
        .name = "UIMCOLTS06", // @text EN: "Darts vs Yongmin Wen" | JA: "ＶＳダーツ／おじいさん"
        .description = "UIMCOLS060", // @text EN: "Yongmin Wen: He isn't too old to" | JA: "対戦相手：文 永民（ブン・エイミン）"
        .descriptionEnd = "UIMCOLS061", // @text EN: "beat you, so be careful!" | JA: "もう歳だと甘く見ると痛い目にあうぞ！"
        .helpTopic = 0x4c},
    {.callback = QueueOmakeSceneChange,
        .command = {.area = SHEN_NAME(WEG0), .entry = 1},
        .requiredFlags = 0x400,
        .photoTexture = 44,
        .colour = 0xffff,
        .reserved1c = 0,
        .name = "UIMCOLTS07", // @text EN: "Darts vs Fenglin Xuan" | JA: "ＶＳダーツ／ゲーム少年"
        .description = "UIMCOLS070", // @text EN: "Fenglin Xuan: He can be a good" | JA: "対戦相手：宣 風林（セン・フウリン）"
        .descriptionEnd = "UIMCOLS071", // @text EN: "partner for training." | JA: "練習相手にはちょうどいいかな？"
        .helpTopic = 0x4c},
    {.callback = QueueOmakeSceneChange,
        .command = {.area = SHEN_NAME(AKS1), .entry = 1},
        .requiredFlags = 0x1000,
        .photoTexture = 45,
        .colour = 0xffff,
        .reserved1c = 0,
        .name = "UIMCOLTS08", // @text EN: "Darts vs Baiyang Liu" | JA: "ＶＳダーツ／キザ男"
        .description = "UIMCOLS080", // @text EN: "Baiyang Liu: Take the snob down" | JA: "対戦相手：柳 白陽（リュウ・ハクヨウ）"
        .descriptionEnd = "UIMCOLS081", // @text EN: "by winning!" | JA: "キザ男の鼻っ柱をへし折れ！"
        .helpTopic = 0x4c},
    {.callback = QueueOmakeSceneChange,
        .command = {.area = SHEN_NAME(AKS0), .entry = 1},
        .requiredFlags = 0x800,
        .photoTexture = 41,
        .colour = 0xffff,
        .reserved1c = 0,
        .name = "UIMCOLTS09", // @text EN: "Darts vs Chengyou Wen" | JA: "ＶＳダーツ／野球男"
        .description = "UIMCOLS090", // @text EN: "Chengyou Wen: Throw straight" | JA: "対戦相手：文 承佑（ブン・ショウユウ）"
        .descriptionEnd = "UIMCOLS091", // @text EN: "at the center only!" | JA: "直球勝負！狙うはど真ん中のみ！"
        .helpTopic = 0x30},
    {.callback = QueueOmakeSceneChange,
        .command = {.area = SHEN_NAME(ARA0), .entry = 1},
        .requiredFlags = 0x2000,
        .photoTexture = 43,
        .colour = 0xffff,
        .reserved1c = 0,
        .name = "UIMCOLTS10", // @text EN: "Darts vs Barry Jones" | JA: "ＶＳダーツ／ラップ男"
        .description = "UIMCOLS100", // @text EN: "Barry Jones: Don't get" | JA: "対戦相手：バリー・ジョーンズ"
        .descriptionEnd = "UIMCOLS101", // @text EN: "distracted by his raps!" | JA: "調子のいいセリフにだまされるな！"
        .helpTopic = 0x4c},
    {},
    {},
    {},
};
inline const OmakeEntry OMAKE_FIGHT_ENTRIES[13]{
    {.callback = OmakeShowMainMenu,
        .command = {.area = shen_name{0x20202020U}, .entry = 1},
        .requiredFlags = 0x0,
        .photoTexture = 35,
        .colour = 0xfff0,
        .reserved1c = 0,
        .name = "UIMCOLTB12", // @text EN: "Moves Scroll" | JA: "全技巻物"
        .description = "UIMCOLB120", // @text EN: "Select moves to match your style" | JA: "自分のスタイルや対戦相手によって技を選択し、"
        .descriptionEnd = "UIMCOLB121", // @text EN: "or the level of your opponent." | JA: "闘いを有利に運ぼう！"
        .helpTopic = 0x33},
    {.callback = QueueOmakeSceneChange,
        .command = {.area = SHEN_NAME(AB00), .entry = 1},
        .requiredFlags = 0x4000,
        .photoTexture = 24,
        .colour = 0xffff,
        .reserved1c = 0,
        .name = "UIMCOLTB01", // @text EN: "The Beast" | JA: "熊男"
        .description = "UIMCOLB010", // @text EN: "Yan Ren: Attack him" | JA: "対戦相手：厳 仁（ゲン・ジン）"
        .descriptionEnd = "UIMCOLB011", // @text EN: "with a critical strike!" | JA: "強力な技をその体に叩き込め！"
        .helpTopic = 0x6d},
    {.callback = QueueOmakeSceneChange,
        .command = {.area = SHEN_NAME(QSFA), .entry = 1},
        .requiredFlags = 0x8000,
        .photoTexture = 25,
        .colour = 0xffff,
        .reserved1c = 0,
        .name = "UIMCOLTB02", // @text EN: "Mongolian Wrestling" | JA: "モンゴル相撲"
        .description = "UIMCOLB020", // @text EN: "Nadam/Agon/Uzun: Don't stop!" | JA: "対戦相手：ナダム／アゴン／ウズン"
        .descriptionEnd = "UIMCOLB021", // @text EN: "Don't let them get close!" | JA: "止まるな！そして近づけるな！"
        .helpTopic = 0x5f},
    {.callback = QueueOmakeSceneChange,
        .command = {.area = SHEN_NAME(QTB1), .entry = 1},
        .requiredFlags = 0x10000,
        .photoTexture = 29,
        .colour = 0xffff,
        .reserved1c = 0,
        .name = "UIMCOLTB03", // @text EN: "Iron Thrust Style" | JA: "鉄砂掌"
        .description = "UIMCOLB030", // @text EN: "Hanqing Qin: Iron Thrust is" | JA: "対戦相手：秦 漢清（シン・カンセイ）"
        .descriptionEnd = "UIMCOLB031", // @text EN: "a deadly blow. Do not get hit!" | JA: "鉄砂掌は一撃必殺！絶対食らうな！"
        .helpTopic = 0x59},
    {.callback = QueueOmakeSceneChange,
        .command = {.area = SHEN_NAME(Q100), .entry = 1},
        .requiredFlags = 0x20000,
        .photoTexture = 30,
        .colour = 0xffff,
        .reserved1c = 0,
        .name = "UIMCOLTB04", // @text EN: "Acrobat Style" | JA: "軽功"
        .description = "UIMCOLB040", // @text EN: "Enjiang Zhou: Catch the man" | JA: "対戦相手：周 恩将（シュウ・オンショウ）"
        .descriptionEnd = "UIMCOLB041", // @text EN: "evading with light steps!" | JA: "軽やかなその身を捕まえろ！"
        .helpTopic = 0x5b},
    {.callback = QueueOmakeSceneChange,
        .command = {.area = SHEN_NAME(QSFD), .entry = 1},
        .requiredFlags = 0x40000,
        .photoTexture = 28,
        .colour = 0xffff,
        .reserved1c = 0,
        .name = "UIMCOLTB05", // @text EN: "Iron Head Style" | JA: "鉄頭功"
        .description = "UIMCOLB050", // @text EN: "Dongshan Su: Iron Head is" | JA: "対戦相手：蘇 東山（ソ・トウザン）"
        .descriptionEnd = "UIMCOLB051", // @text EN: "a deadly blow. Be careful!" | JA: "必殺の鉄頭功に気をつけろ！"
        .helpTopic = 0x5d},
    {.callback = QueueOmakeSceneChange,
        .command = {.area = SHEN_NAME(QSFC), .entry = 1},
        .requiredFlags = 0x80000,
        .photoTexture = 27,
        .colour = 0xffff,
        .reserved1c = 0,
        .name = "UIMCOLTB06", // @text EN: "One-eyed Man" | JA: "片目の豪傑"
        .description = "UIMCOLB060", // @text EN: "Zhouhong Kong: He's extremely" | JA: "対戦相手：孔 周鴻（コウ・シュウコウ）"
        .descriptionEnd = "UIMCOLB061", // @text EN: "strong! Find out his weakness!" | JA: "手強い相手だ！弱点を探せ！"
        .helpTopic = 0x64},
    {.callback = QueueOmakeSceneChange,
        .command = {.area = SHEN_NAME(QSFB), .entry = 1},
        .requiredFlags = 0x100000,
        .photoTexture = 26,
        .colour = 0xffff,
        .reserved1c = 0,
        .name = "UIMCOLTB07", // @text EN: "Passive Raid Style" | JA: "化勁"
        .description = "UIMCOLB070", // @text EN: "Ma Wenyuan: Watch for the chance" | JA: "対戦相手：馬 文円（マ・ブンエン）"
        .descriptionEnd = "UIMCOLB071", // @text EN: "to attack and strike!" | JA: "化勁の隙を突いて、一撃を入れろ！"
        .helpTopic = 0x61},
    {.callback = QueueOmakeSceneChange,
        .command = {.area = SHEN_NAME(QGBT), .entry = 2},
        .requiredFlags = 0x200000,
        .photoTexture = 32,
        .colour = 0xffff,
        .reserved1c = 0,
        .name = "UIMCOLTB08", // @text EN: "Wrestling" | JA: "レスリング"
        .description = "UIMCOLB080", // @text EN: "Greg More: 2 pt match" | JA: "対戦相手：グレッグ・モア"
        .descriptionEnd = "UIMCOLB081", // @text EN: "against swift throws!" | JA: "無制限２本先取！強力な投技に注意！"
        .helpTopic = 0x96},
    {.callback = QueueOmakeSceneChange,
        .command = {.area = SHEN_NAME(QJBT), .entry = 2},
        .requiredFlags = 0x400000,
        .photoTexture = 31,
        .colour = 0xffff,
        .reserved1c = 0,
        .name = "UIMCOLTB09", // @text EN: "Pancratium" | JA: "パンクラチオン"
        .description = "UIMCOLB090", // @text EN: "Rod Stunt: 2 pt match" | JA: "対戦相手：ロッド・スタント"
        .descriptionEnd = "UIMCOLB091", // @text EN: "against fierce blows!" | JA: "無制限２本先取！打撃力は凄まじいぞ！"
        .helpTopic = 0x95},
    {.callback = QueueOmakeSceneChange,
        .command = {.area = SHEN_NAME(QLBT), .entry = 2},
        .requiredFlags = 0x800000,
        .photoTexture = 33,
        .colour = 0xffff,
        .reserved1c = 0,
        .name = "UIMCOLTB10", // @text EN: "Jeet Kune Do" | JA: "截拳道"
        .description = "UIMCOLB100", // @text EN: "Chunyan: 2 pt match" | JA: "対戦相手：春燕（シュンエン）"
        .descriptionEnd = "UIMCOLB101", // @text EN: "against agile kicks!" | JA: "無制限２本先取！鋭い足技は脅威だ！"
        .helpTopic = 0x97},
    {.callback = QueueOmakeSceneChange,
        .command = {.area = SHEN_NAME(QKBT), .entry = 3},
        .requiredFlags = 0x1000000,
        .photoTexture = 34,
        .colour = 0xffff,
        .reserved1c = 0,
        .name = "UIMCOLTB11", // @text EN: "Tiger Swallow Style" | JA: "虎燕拳"
        .description = "UIMCOLB110", // @text EN: "Baihu: 2 pt match " | JA: "対戦相手：白虎（ビャッコ）"
        .descriptionEnd = "UIMCOLB111", // @text EN: "against punch combos!" | JA: "無制限２本先取！連続技に気をつけろ！"
        .helpTopic = 0x98},
    {},
};
inline const OmakeEntry *const OMAKE_COLLECTION_TABLES[]{OMAKE_ARCADE_ENTRIES, OMAKE_MINIGAME_ENTRIES, OMAKE_FIGHT_ENTRIES};
inline constexpr OmakeCommand OMAKE_COLD_RESET{.area = shen_name{0x20202020U}, .entry = 5};
} // namespace shenmue::s2::areas
