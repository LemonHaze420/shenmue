#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_omak.hpp"

#if defined(SHENMUE_TARGET_S2_DC_EU)
#define OMAKE_TEXT(japanese, english, localized) ((const char *)localized)
#define OMAKE_PHOTO(index) ((index) + 24)
#define OMAKE_UI_TEXTURE(original, localized) (localized)
#define OMAKE_SOURCE_LINE(original, localized) (localized)
#define OMAKE_LINE(text) ((text) + omakeLanguage * 40)
#elif defined(SHENMUE_TARGET_S2_DC_US)
#define OMAKE_TEXT(japanese, english, localized) english
#define OMAKE_PHOTO(index) (index)
#define OMAKE_UI_TEXTURE(original, localized) (original)
#define OMAKE_SOURCE_LINE(original, localized) (original)
#define OMAKE_LINE(text) (text)
#else
#define OMAKE_TEXT(japanese, english, localized) japanese
#define OMAKE_PHOTO(index) (index)
#define OMAKE_UI_TEXTURE(original, localized) (original)
#define OMAKE_SOURCE_LINE(original, localized) (original)
#define OMAKE_LINE(text) (text)
#endif

#if defined(SHENMUE_EVENT_ENTRY)
void RunOmake(void)
{
    FTBL.shenUpdateFlagEvaluation(1);
    BootOmake();
}
#else

#if defined(SHENMUE_TARGET_S2_DC_EU)
static char omakeClassifiedImagesName[4][40] = {
    "CLASSIFIED IMAGES",
    "IMAGES SECR\245\250TES",
    "ENTWURFSBILDER",
    "IM\245\265GENES CLASIFICADAS"
};
static char omakeClassifiedImagesDescription[4][40] = {
    "The Sega Saturn version of",
    "La version Sega Saturn de",
    "Die Sega Saturn-Version von",
    "La versi\245\342n de Shenmue para Sega"
};
static char omakeClassifiedImagesDescriptionEnd[4][40] = {
    "Shenmue -- never before shown!",
    "Shenmue=@ Du jamais vu!",
    "Shenmue - nie zuvor gezeigt!",
    "Saturn. \245\260Totalmente in\245\341dita!"
};
static char omakeHangOnName[4][40] = {
    "HANG ON",
    "HANG ON",
    "HANG ON",
    "HANG ON"
};
static char omakeSpaceHarrierName[4][40] = {
    "SPACE HARRIER",
    "SPACE HARRIER",
    "SPACE HARRIER",
    "SPACE HARRIER"
};
static char omakeOutrunName[4][40] = {
    "OutRun",
    "OutRun",
    "OutRun",
    "OutRun"
};
static char omakeAfterburnerIiName[4][40] = {
    "AFTERBURNER II",
    "AFTERBURNER II",
    "AFTERBURNER II",
    "AFTERBURNER II"
};
static char omakeQteTitleName[4][40] = {
    "QTE Title",
    "Titre QTE",
    "QTE-Titel",
    "QTE Title"
};
static char omakeExciteQte2Name[4][40] = {
    "Excite QTE2",
    "Excite QTE2",
    "Excite QTE2",
    "Excite QTE2"
};
static char omakeSlotMachineName[4][40] = {
    "Slot Machine",
    "Machine \245\312 sous",
    "Spielautomat",
    "M\245\336quina tragaperras"
};
static char omakeLuckyHitMuseumName[4][40] = {
    "Lucky Hit Museum",
    "Lucky Hit Museum",
    "Lucky Hit-Museum",
    "Lucky Hit Museum"
};
static char omakeDartsSevenName[4][40] = {
    "Darts Seven",
    "Fl\245\341chettes |Seven|",
    "Darts Seven",
    "Dardos |Siete|"
};
static char omakeVsDartsOldManName[4][40] = {
    "VS Darts/Old Man",
    "VS Darts/Old Man",
    "VS Darts/Alter Mann",
    "Dardos/Anciano"
};
static char omakeVsDartsBoyName[4][40] = {
    "VS Darts/Boy",
    "VS Darts/Boy",
    "VS Darts/Junge",
    "Dardos/Ni\245\363o"
};
static char omakeVsDartsASnobName[4][40] = {
    "VS Darts/A Snob",
    "VS Darts/A Snob",
    "VS Darts/Ein Snob",
    "Dardos/Pijo"
};
static char omakeVsDartsABaseballFanName[4][40] = {
    "VS Darts/A Baseball Fan",
    "VS Darts/A Baseball Fan",
    "VS Darts/Baseball-Fan",
    "Dardos/Fan de b\245\341isbol"
};
static char omakeVsDartsARapperName[4][40] = {
    "VS Darts/ A Rapper",
    "VS Darts/ A Rapper",
    "VS Darts/Ein Rapper",
    "Dardos/Rapero"
};
static char omakeTheBeastName[4][40] = {
    "The Beast",
    "La b\245\330te",
    "Das Tier",
    "The Beast"
};
static char omakeMongolianWrestlerName[4][40] = {
    "Mongolian Wrestler",
    "Lutte Mongole",
    "Mongolischer Ringkampf",
    "Lucha mongola"
};
static char omakeIronThrustStyleName[4][40] = {
    "Iron Thrust Style",
    "Iron Thrust Style",
    "Eisensto\245\254-Stil",
    "Iron Thrust Style"
};
static char omakeAcrobatStyleName[4][40] = {
    "Acrobat Style",
    "Acrobat Style",
    "Akrobatik-Stil",
    "Acrobat Style"
};
static char omakeIronHeadStyleName[4][40] = {
    "Iron Head Style",
    "Iron Head Style",
    "Eisenkopf-Stil",
    "Iron Head Style"
};
static char omakeOneEyedManName[4][40] = {
    "One-eyed Man",
    "Le cyclope",
    "Der Ein\245\300ugige",
    "Hombre de un solo ojo."
};
static char omakePassiveRaidStyleName[4][40] = {
    "Passive Raid Style",
    "Passive Raid Style",
    "Verteidigungsstil",
    "Passive Raid Style"
};
static char omakeWrestlingName[4][40] = {
    "Wrestling",
    "Lutte",
    "Ringen",
    "Lucha"
};
static char omakePancratiumName[4][40] = {
    "Pancratium",
    "Pancratium",
    "Pancratium",
    "Pancratium"
};
static char omakeJeetKuneDoName[4][40] = {
    "Jeet Kune Do",
    "Jeet Kune Do",
    "Jeet Kune Do",
    "Jeet Kune Do"
};
static char omakeTigerSwallowStyleName[4][40] = {
    "Tiger Swallow Style",
    "Tiger Swallow Style",
    "Tiger-Stil",
    "Tiger Swallow Style"
};
static char omakeMovesScrollName[4][40] = {
    "Moves Scroll",
    "Manuscrit des mvmnts",
    "Martial-Arts-Schrift",
    "Pergamino de A. M."
};
static char omakeHangOnDescription[4][40] = {
    "Keep your speed up to win!",
    "Acc\245\341l\245\341rez pour gagner!",
    "Du musst ein hohes Tempo",
    "\245\260No bajes la velocidad"
};
static char omakeHangOnDescriptionEnd[4][40] = {
    "",
    "",
    "hinlegen, um zu gewinnen!",
    "para ganar!"
};
static char omakeSpaceHarrierDescription[4][40] = {
    "A SciFi shooting game with easy",
    "Jeu de tir de science fiction,",
    "Sci-Fi-Ballerspiel mit einfacher",
    "Juego de ciencia ficci\245\342n f\245\336cil"
};
static char omakeSpaceHarrierDescriptionEnd[4][40] = {
    "controls and dramatic speed.",
    "hyper rapide mais abordable.",
    "Steuerung und rasantem Tempo.",
    "y con velocidad de v\245\341rtigo."
};
static char omakeOutrunDescription[4][40] = {
    "An exhilarating driving game.",
    "Jeu de conduite enivrant.",
    "Rennen in sch\245\311nen Landschaften.",
    "Juego de coches sin aliento."
};
static char omakeOutrunDescriptionEnd[4][40] = {
    "Try to complete all courses!",
    " Le but: terminer les courses!",
    "Schaffe alle Strecken!",
    "\245\260Recorre todos los circuitos!"
};
static char omakeAfterburnerIiDescription[4][40] = {
    "A shooting game that simulates",
    "Un jeu de simulation de tir",
    "Eine Luftkampfsimulation.",
    "Un juego de disparos que simula"
};
static char omakeAfterburnerIiDescriptionEnd[4][40] = {
    "aerial combat.",
    "pendant un combat a\245\341rien.",
    "",
    "el combate a\245\341reo."
};
static char omakeQteTitleDescription[4][40] = {
    "Improve your QTE skills",
    "Am\245\341liorez vos techniques QTE",
    "Verbessere deine QTE-Fertig-",
    "\245\260Mejora tus t\245\341cnicas QTE con"
};
static char omakeQteTitleDescriptionEnd[4][40] = {
    "by playing this game!",
    "avec ce jeu!",
    "keiten mit diesem Spiel!",
    "este juego!"
};
static char omakeExciteQte2Description[4][40] = {
    "Can you keep up with",
    "Pouvez-vous suivre le rythme",
    "Kannst du mit dem steigenden",
    "\245\256Puedes mantener la velocidad?"
};
static char omakeExciteQte2DescriptionEnd[4][40] = {
    "the increasing speed?",
    "soutenu de ce jeu ?",
    "Tempo mithalten?",
    ""
};
static char omakeSlotMachineDescription[4][40] = {
    "Enjoy playing with the slot",
    "Vous aimez jouer au simulateur de",
    "Viel Spa\245\254 mit dem Automaten-",
    "Juega con el simulador de la"
};
static char omakeSlotMachineDescriptionEnd[4][40] = {
    "machine simulator. Aim for 777!",
    "machines a sous ? Tentez 777!",
    "Simulator! Erreiche 777!",
    "m\245\336quina tragaperras.\245\260A por 777!"
};
static char omakeLuckyHitMuseumDescription[4][40] = {
    "Here's a collection of all kinds",
    "Voici des stands Lucky Hit!",
    "Eine Sammlung aller Lucky Hit-",
    "\245\260Una colecci\245\342n de todos los"
};
static char omakeLuckyHitMuseumDescriptionEnd[4][40] = {
    "of different Lucky Hit stands!",
    "Auquel jouerez-vous en 1er ?",
    "St\245\300nde! Wo willst du beginnen?",
    "puestos Lucky Hit!"
};
static char omakeDartsSevenDescription[4][40] = {
    "Relax and enjoy a game of darts.",
    "Amusez-vous \245\312 faire une partie",
    "Entspann dich und genie\245\254e ein",
    "Rel\245\336jate y disfruta un juego"
};
static char omakeDartsSevenDescriptionEnd[4][40] = {
    "",
    "de fl\245\341chette.",
    "Dartsspiel.",
    "de dardos."
};
static char omakeVsDartsOldManDescription[4][40] = {
    "Yongmin Wen: He isn't too old to",
    "Yongmin Wen: Il n'est pas trop",
    "Yongmin Wen: Er ist nicht zu",
    "Yongmin Wen: \245\260No es tan viejo"
};
static char omakeVsDartsOldManDescriptionEnd[4][40] = {
    "beat you, so be careful!",
    "vieux pour vous battre!",
    "alt, um dich zu besiegen!",
    "para ganarte, cuidado!"
};
static char omakeVsDartsBoyDescription[4][40] = {
    "Fenglin Xuan: He can be a good",
    "Fenglin Xuan: C'est un bon",
    "Fenglin Xuan: Er kann ein guter",
    "Fenglin Xuan: Puede ser buen"
};
static char omakeVsDartsBoyDescriptionEnd[4][40] = {
    "partner for training.",
    "partenaire pour l'entra\245\322nement.",
    "Trainingspartner sein.",
    "compa\245\363ero."
};
static char omakeVsDartsASnobDescription[4][40] = {
    "Baiyang Liu: Take the snob down",
    "Baiyang Liu: Faites d\245\341chanter",
    "Baiyang Liu: Besiege den",
    "Baiyang Liu:"
};
static char omakeVsDartsASnobDescriptionEnd[4][40] = {
    "by winning!",
    "le snob en gagnant!",
    "arroganten Kerl!",
    "\245\260G\245\336nale al pijo!"
};
static char omakeVsDartsABaseballFanDescription[4][40] = {
    "Chengyou Wen: Throw straight",
    "Chengyou Wen:",
    "Chengyou Wen:",
    "Chengyou Wen:"
};
static char omakeVsDartsABaseballFanDescriptionEnd[4][40] = {
    "at the center only!",
    "Lancez seulement dans le mille!",
    "Wirf nur direkt in die Mitte!",
    "\245\260Apunta s\245\342lo al centro!"
};
static char omakeVsDartsARapperDescription[4][40] = {
    "Barry Jones: Don't get",
    "Barry Jones: Ne soyez pas",
    "Barry Jones: Lass dich nicht von",
    "Barry Jones: \245\260No dejes que sus"
};
static char omakeVsDartsARapperDescriptionEnd[4][40] = {
    "distracted by his raps!",
    "distait par ses bavardages!",
    "seinem Rappen ablenken!",
    "rapeo te distraiga!"
};
static char omakeTheBeastDescription[4][40] = {
    "Ren Yan: Attack him",
    "Ren Yan: Attaquez-le avec un",
    "Ren Yan: Attackiere ihn mit",
    "Ren Yan: \245\260At\245\336calo con"
};
static char omakeTheBeastDescriptionEnd[4][40] = {
    "with a critical strike!",
    "coup critique!",
    "einem kritischen Treffer!",
    "ungolpe cr\245\337tico!"
};
static char omakeMongolianWrestlerDescription[4][40] = {
    "Nadam/Agon/Uzun: Don't stop!",
    "Nadam/Agon/Uzun:",
    "Nadam/Agon/Uzun:",
    "Nadam/Agon/Uzun: \245\260No pares!"
};
static char omakeMongolianWrestlerDescriptionEnd[4][40] = {
    "Don't let them get close!",
    "Ne leslaissez pas s'approcher!",
    "Lass sienicht zu nahe kommen!",
    "\245\260No dejes que se acerquen!"
};
static char omakeIronThrustStyleDescription[4][40] = {
    "Hanqing Qin: Iron Thrust is",
    "Hanqing Qin: Iron Thrust est un",
    "Hanqing Qin: Eisensto\245\254 ist",
    "Hanqing Qin: Iron Thrust es un"
};
static char omakeIronThrustStyleDescriptionEnd[4][40] = {
    "a deadly blow. Do not get hit!",
    "coup mortel. Faites attention!",
    "t\245\311dlich. Weiche ihm aus!",
    "golpe mortal. \245\260Ten cuidado!"
};
static char omakeAcrobatStyleDescription[4][40] = {
    "Enjiang Zhou: Catch the man",
    "Enjiang Zhou: Attrapez l'homme",
    "Enjiang Zhou: Fang den Mann mit",
    "Enjiang Zhou: \245\260Atrapa al"
};
static char omakeAcrobatStyleDescriptionEnd[4][40] = {
    "evading with light steps!",
    "qui s'\245\341vade discr\245\315tement!",
    "dem leichten Schritt!",
    "hombre con pasos ligeros!"
};
static char omakeIronHeadStyleDescription[4][40] = {
    "Dongshan Su: Iron Head is",
    "Dongshan Su: Iron Head est un",
    "Dongshan Su: Eisenkopf ist",
    "Dongshan Su: Iron Head es un"
};
static char omakeIronHeadStyleDescriptionEnd[4][40] = {
    "a deadly blow. Be careful!",
    "coup mortel. Attention!",
    "t\245\311dlich. Sei vorsichtig!",
    "golpe mortal. \245\260Cuidado!"
};
static char omakeOneEyedManDescription[4][40] = {
    "Zhouhong Kong: He's extremely",
    "Zhouhong Kong: Il est tr\245\315s",
    "Zhouhong Kong: Er ist sehr",
    "Zhouhong Kong: \245\260Es muy fuerte!"
};
static char omakeOneEyedManDescriptionEnd[4][40] = {
    "strong! Find out his weakness!",
    "fort! D\245\341couvrez sa faiblesse!",
    "stark! Finde seine Schw\245\300che!",
    "\245\260Descubre sus puntos d\245\341biles!"
};
static char omakePassiveRaidStyleDescription[4][40] = {
    "Wenyuan Ma: Watch for the chance",
    "Wenyuan Ma: Attaquez au bon",
    "Wenyuan Ma: Warte auf die",
    "Wenyuan Ma: \245\260Ataca y golpea en"
};
static char omakePassiveRaidStyleDescriptionEnd[4][40] = {
    "to attack and strike!",
    "moment et marquez des points!",
    "Gelegenheit zum Attackieren!",
    "el momento adecuado!"
};
static char omakeWrestlingDescription[4][40] = {
    "Greg More: 2 pt match",
    "Greg More: Le 1er quiremporte",
    "Greg More: Wer zweimal gewinnt,",
    "Greg More: \245\260Gana el que ganedos"
};
static char omakeWrestlingDescriptionEnd[4][40] = {
    "against swift throws!",
    "2 victoires est le vainqueur!",
    "ist Sieger. Starke Attacke!",
    "veces! \245\260Cuidado con su ataque!"
};
static char omakePancratiumDescription[4][40] = {
    "Rod Stunt: 2 pt match",
    "Rod Stunt: Le 1er qui gagne",
    "Rod Stunt: Wer zweimal gewinnt,",
    "Rod Stunt: \245\260Gana el que gana dos"
};
static char omakePancratiumDescriptionEnd[4][40] = {
    "against fierce blows!",
    "2 fois l'emporte!",
    "ist Sieger. Heftige Attacken!",
    "veces!! Sus ataques son feroces!"
};
static char omakeJeetKuneDoDescription[4][40] = {
    "Chunyan: 2 pt match",
    "Chunyan: Le 1er qui gagne 2",
    "Chunyan: Wer zweimal gewinnt,",
    "Chunyan:\245\260Gana el que gane dos"
};
static char omakeJeetKuneDoDescriptionEnd[4][40] = {
    "against agile kicks!",
    "fois l'emporte!",
    "ist Sieger. Beintechniken!",
    "veces!! Cuidado con su ataque!"
};
static char omakeTigerSwallowStyleDescription[4][40] = {
    "Baihu: 2 pt match ",
    "Baihu: Attention a",
    "Baihu: Wer zweimal gewinnt,",
    "Baihu: \245\260Gana el que gane dos"
};
static char omakeTigerSwallowStyleDescriptionEnd[4][40] = {
    "against punch combos!",
    "ses attaques consecutives!",
    "ist Sieger. Attackenkombos!",
    "veces! \245\260Cuidado con sus ataques!"
};
static char omakeMovesScrollDescription[4][40] = {
    "Select moves to match your style",
    "Select. les mvmnts selon votre",
    "W\245\300hle die Techniken deinem Stil",
    "Selecciona los movimientos seg\245\340n"
};
static char omakeMovesScrollDescriptionEnd[4][40] = {
    "or the level of your opponent.",
    "style ou niveau de l'adversaire.",
    "und deinem Gegner entsprechend.",
    "tu estilo o el contrincante."
};
#endif

const OmakeEntry omakeArcadeEntries[13] = {

    {QueueOmakeModuleCall, {OMAKE_NO_AREA, 3}, 0x1, OMAKE_PHOTO(48), 0xffff, OMAKE_TEXT("ハングオン", "HANG ON", omakeHangOnName),
        OMAKE_TEXT("ライバルを抜き、チェッカーを受けろ", "Keep your speed up to win!", omakeHangOnDescription),
        OMAKE_TEXT("バイクレースの決定版！", "", omakeHangOnDescriptionEnd), 137},

    {QueueOmakeModuleCall, {OMAKE_NO_AREA, 4}, 0x2, OMAKE_PHOTO(49), 0xffff, OMAKE_TEXT("スペースハリアー", "SPACE HARRIER", omakeSpaceHarrierName),
        OMAKE_TEXT("軽快な操作感とスピーディーな展開の", "A SciFi shooting game with easy", omakeSpaceHarrierDescription),
        OMAKE_TEXT("ＳＦシューティングゲームだ！", "controls and dramatic speed.", omakeSpaceHarrierDescriptionEnd), 136},

    {QueueOmakeModuleCall, {OMAKE_NO_AREA, 1}, 0x4, OMAKE_PHOTO(46), 0xffff, OMAKE_TEXT("アウトラン", "OutRun", omakeOutrunName),
        OMAKE_TEXT("美しく爽快なドライブゲーム", "An exhilarating driving game.", omakeOutrunDescription),
        OMAKE_TEXT("全コース走破をめざせ！", "Try to complete all courses!", omakeOutrunDescriptionEnd), 134},

    {QueueOmakeModuleCall, {OMAKE_NO_AREA, 2}, 0x8, OMAKE_PHOTO(47), 0xffff, OMAKE_TEXT("アフターバーナー續", "AFTERBURNER II", omakeAfterburnerIiName),
        OMAKE_TEXT("戦闘機による空中戦をリアルに", "A shooting game that simulates", omakeAfterburnerIiDescription),
        OMAKE_TEXT("再現したシューティングゲーム！", "aerial combat.", omakeAfterburnerIiDescriptionEnd), 135}};
const OmakeEntry omakeVideoEntries[13] = {

    {QueueOmakeVideo, {OMAKE_NO_AREA, 1}, 0x0, OMAKE_PHOTO(50), 0xfff0, OMAKE_TEXT("★極秘映像★", "CLASSIFIED IMAGES", omakeClassifiedImagesName),
        OMAKE_TEXT("幻のセガ・サターン版シェンムー", "The Sega Saturn version of", omakeClassifiedImagesDescription),
        OMAKE_TEXT("その姿が今ここに！", "Shenmue -- never before shown!", omakeClassifiedImagesDescriptionEnd), 40},

    {QueueOmakeModuleCall, {OMAKE_NO_AREA, 3}, 0x1, OMAKE_PHOTO(48), 0xffff, OMAKE_TEXT("ハングオン", "HANG ON", omakeHangOnName),
        OMAKE_TEXT("ライバルを抜き、チェッカーを受けろ", "Keep your speed up to win!", omakeHangOnDescription),
        OMAKE_TEXT("バイクレースの決定版！", "", omakeHangOnDescriptionEnd), 137},

    {QueueOmakeModuleCall, {OMAKE_NO_AREA, 4}, 0x2, OMAKE_PHOTO(49), 0xffff, OMAKE_TEXT("スペースハリアー", "SPACE HARRIER", omakeSpaceHarrierName),
        OMAKE_TEXT("軽快な操作感とスピーディーな展開の", "A SciFi shooting game with easy", omakeSpaceHarrierDescription),
        OMAKE_TEXT("ＳＦシューティングゲームだ！", "controls and dramatic speed.", omakeSpaceHarrierDescriptionEnd), 136},

    {QueueOmakeModuleCall, {OMAKE_NO_AREA, 1}, 0x4, OMAKE_PHOTO(46), 0xffff, OMAKE_TEXT("アウトラン", "OutRun", omakeOutrunName),
        OMAKE_TEXT("美しく爽快なドライブゲーム", "An exhilarating driving game.", omakeOutrunDescription),
        OMAKE_TEXT("全コース走破をめざせ！", "Try to complete all courses!", omakeOutrunDescriptionEnd), 134},

    {QueueOmakeModuleCall, {OMAKE_NO_AREA, 2}, 0x8, OMAKE_PHOTO(47), 0xffff, OMAKE_TEXT("アフターバーナー續", "AFTERBURNER II", omakeAfterburnerIiName),
        OMAKE_TEXT("戦闘機による空中戦をリアルに", "A shooting game that simulates", omakeAfterburnerIiDescription),
        OMAKE_TEXT("再現したシューティングゲーム！", "aerial combat.", omakeAfterburnerIiDescriptionEnd), 135}};
const OmakeEntry omakeMinigameEntries[13] = {

    {QueueOmakeSceneChange, {SHEN_NAME4('W', 'E', 'G', '0'), 8}, 0x10, OMAKE_PHOTO(36), 0xffff, OMAKE_TEXT("ＱＴＥタイトル", "QTE Title", omakeQteTitleName),
        OMAKE_TEXT("ＱＴＥの苦手な人は、", "Improve your QTE skills", omakeQteTitleDescription),
        OMAKE_TEXT("これで練習しよう！", "by playing this game!", omakeQteTitleDescriptionEnd), 27},

    {QueueOmakeSceneChange, {SHEN_NAME4('W', 'E', 'G', '0'), 7}, 0x20, OMAKE_PHOTO(37), 0xffff, OMAKE_TEXT("エキサイトＱＴＥ２", "Excite QTE2", omakeExciteQte2Name),
        OMAKE_TEXT("だんだん速くなっていくスピードに", "Can you keep up with", omakeExciteQte2Description),
        OMAKE_TEXT("どこまでついてこれるかな？", "the increasing speed?", omakeExciteQte2DescriptionEnd), 4},

    {QueueOmakeSceneChange, {SHEN_NAME4('W', 'E', 'G', '0'), 20}, 0x40, OMAKE_PHOTO(38), 0xffff, OMAKE_TEXT("スロットマシン", "Slot Machine", omakeSlotMachineName),
        OMAKE_TEXT("本格的スロットマシンが楽しめる", "Enjoy playing with the slot", omakeSlotMachineDescription),
        OMAKE_TEXT("狙うは７７７だ！", "machine simulator. Aim for 777!", omakeSlotMachineDescriptionEnd), 148},

    {QueueOmakeSceneChange, {SHEN_NAME4('Q', '3', '0', '0'), 98}, 0x80, OMAKE_PHOTO(39), 0xffff, OMAKE_TEXT("落し玉ミュージアム", "Lucky Hit Museum", omakeLuckyHitMuseumName),
        OMAKE_TEXT("おもしろい台が目白押し！", "Here's a collection of all kinds", omakeLuckyHitMuseumDescription),
        OMAKE_TEXT("さあ、どれから挑戦する？", "of different Lucky Hit stands!", omakeLuckyHitMuseumDescriptionEnd), 144},

    {QueueOmakeSceneChange, {SHEN_NAME4('W', 'E', 'M', '1'), 1}, 0x100, OMAKE_PHOTO(40), 0xffff, OMAKE_TEXT("ダーツセブン", "Darts Seven", omakeDartsSevenName),
        OMAKE_TEXT("気軽に本格的ダーツを楽しもう", "Relax and enjoy a game of darts.", omakeDartsSevenDescription),
        OMAKE_TEXT("", "", omakeDartsSevenDescriptionEnd), 47},

    {QueueOmakeSceneChange, {SHEN_NAME4('W', 'R', 'S', '2'), 1}, 0x200, OMAKE_PHOTO(42), 0xffff, OMAKE_TEXT("ＶＳダーツ／おじいさん", "VS Darts/Old Man", omakeVsDartsOldManName),
        OMAKE_TEXT("文 永民（ブン・エイミン）", "Yongmin Wen: He isn't too old to", omakeVsDartsOldManDescription),
        OMAKE_TEXT("もう歳だと甘く見ると痛い目にあうぞ", "beat you, so be careful!", omakeVsDartsOldManDescriptionEnd), 76},

    {QueueOmakeSceneChange, {SHEN_NAME4('W', 'E', 'G', '0'), 1}, 0x400, OMAKE_PHOTO(44), 0xffff, OMAKE_TEXT("ＶＳダーツ／ゲーム少年", "VS Darts/Boy", omakeVsDartsBoyName),
        OMAKE_TEXT("宣 風林（セン・フウリン）", "Fenglin Xuan: He can be a good", omakeVsDartsBoyDescription),
        OMAKE_TEXT("練習相手にはちょうどいいかな？", "partner for training.", omakeVsDartsBoyDescriptionEnd), 76},

    {QueueOmakeSceneChange, {SHEN_NAME4('A', 'K', 'S', '1'), 1}, 0x1000, OMAKE_PHOTO(45), 0xffff, OMAKE_TEXT("ＶＳダーツ／キザ男", "VS Darts/A Snob", omakeVsDartsASnobName),
        OMAKE_TEXT("柳 白陽（リュウ・ハクヨウ）", "Baiyang Liu: Take the snob down", omakeVsDartsASnobDescription),
        OMAKE_TEXT("キザ男の鼻っ柱をたたき折れ！", "by winning!", omakeVsDartsASnobDescriptionEnd), 76},

    {QueueOmakeSceneChange, {SHEN_NAME4('A', 'K', 'S', '0'), 1}, 0x800, OMAKE_PHOTO(41), 0xffff, OMAKE_TEXT("ＶＳダーツ／野球男", "VS Darts/A Baseball Fan", omakeVsDartsABaseballFanName),
        OMAKE_TEXT("文 承佑（ブン・ショウユウ）", "Chengyou Wen: Throw straight", omakeVsDartsABaseballFanDescription),
        OMAKE_TEXT("直球勝負！　狙うはど真ん中のみ！", "at the center only!", omakeVsDartsABaseballFanDescriptionEnd), 48},

    {QueueOmakeSceneChange, {SHEN_NAME4('A', 'R', 'A', '0'), 1}, 0x2000, OMAKE_PHOTO(43), 0xffff, OMAKE_TEXT("ＶＳダーツ／ラップ男", "VS Darts/ A Rapper", omakeVsDartsARapperName),
        OMAKE_TEXT("バリー・ジョーンズ", "Barry Jones: Don't get", omakeVsDartsARapperDescription),
        OMAKE_TEXT("調子のいいセリフにだまされるな！", "distracted by his raps!", omakeVsDartsARapperDescriptionEnd), 76}};
const OmakeEntry omakeFightEntries[13] = {

    {QueueOmakeFight, {OMAKE_NO_AREA, 1}, 0x0, OMAKE_PHOTO(35), 0xfff0, OMAKE_TEXT("全技巻物", "Moves Scroll", omakeMovesScrollName),
        OMAKE_TEXT("自分のスタイルや対戦相手によって", "Select moves to match your style", omakeMovesScrollDescription),
        OMAKE_TEXT("技を選択し、闘いを有利に運ぼう！", "or the level of your opponent.", omakeMovesScrollDescriptionEnd), 51},

    {QueueOmakeSceneChange, {SHEN_NAME4('A', 'B', '0', '0'), 1}, 0x4000, OMAKE_PHOTO(24), 0xffff, OMAKE_TEXT("熊男", "The Beast", omakeTheBeastName),
        OMAKE_TEXT("厳 仁（ゲン・ジン）", "Ren Yan: Attack him", omakeTheBeastDescription),
        OMAKE_TEXT("強力な技をその体に叩き込め！", "with a critical strike!", omakeTheBeastDescriptionEnd), 109},

    {QueueOmakeSceneChange, {SHEN_NAME4('Q', 'S', 'F', 'A'), 1}, 0x8000, OMAKE_PHOTO(25), 0xffff, OMAKE_TEXT("モンゴル相撲", "Mongolian Wrestler", omakeMongolianWrestlerName),
        OMAKE_TEXT("ナダム／アゴン／ウズン", "Nadam/Agon/Uzun: Don't stop!", omakeMongolianWrestlerDescription),
        OMAKE_TEXT("止まるな！　そして近づけるな！", "Don't let them get close!", omakeMongolianWrestlerDescriptionEnd), 95},

    {QueueOmakeSceneChange, {SHEN_NAME4('Q', 'T', 'B', '1'), 1}, 0x10000, OMAKE_PHOTO(29), 0xffff, OMAKE_TEXT("鉄砂掌", "Iron Thrust Style", omakeIronThrustStyleName),
        OMAKE_TEXT("秦 漢清（シン・カンセイ）", "Hanqing Qin: Iron Thrust is", omakeIronThrustStyleDescription),
        OMAKE_TEXT("鉄砂掌は一撃必殺！　絶対食らうな！", "a deadly blow. Do not get hit!", omakeIronThrustStyleDescriptionEnd), 89},

    {QueueOmakeSceneChange, {SHEN_NAME4('Q', '1', '0', '0'), 1}, 0x20000, OMAKE_PHOTO(30), 0xffff, OMAKE_TEXT("軽功", "Acrobat Style", omakeAcrobatStyleName),
        OMAKE_TEXT("周 恩将（シュウ・オンショウ）", "Enjiang Zhou: Catch the man", omakeAcrobatStyleDescription),
        OMAKE_TEXT("軽やかなその身を捕まえろ！", "evading with light steps!", omakeAcrobatStyleDescriptionEnd), 91},

    {QueueOmakeSceneChange, {SHEN_NAME4('Q', 'S', 'F', 'D'), 1}, 0x40000, OMAKE_PHOTO(28), 0xffff, OMAKE_TEXT("鉄頭功", "Iron Head Style", omakeIronHeadStyleName),
        OMAKE_TEXT("蘇 東山（ソ・トウザン）", "Dongshan Su: Iron Head is", omakeIronHeadStyleDescription),
        OMAKE_TEXT("必殺の鉄頭功に気をつけろ！", "a deadly blow. Be careful!", omakeIronHeadStyleDescriptionEnd), 93},

    {QueueOmakeSceneChange, {SHEN_NAME4('Q', 'S', 'F', 'C'), 1}, 0x80000, OMAKE_PHOTO(27), 0xffff, OMAKE_TEXT("片目の豪傑", "One-eyed Man", omakeOneEyedManName),
        OMAKE_TEXT("孔 周鴻（コウ・シュウコウ）", "Zhouhong Kong: He's extremely", omakeOneEyedManDescription),
        OMAKE_TEXT("手強い相手だ！　弱点を探せ！", "strong! Find out his weakness!", omakeOneEyedManDescriptionEnd), 100},

    {QueueOmakeSceneChange, {SHEN_NAME4('Q', 'S', 'F', 'B'), 1}, 0x100000, OMAKE_PHOTO(26), 0xffff, OMAKE_TEXT("化勁", "Passive Raid Style", omakePassiveRaidStyleName),
        OMAKE_TEXT("馬 文円（マ・ブンエン）", "Wenyuan Ma: Watch for the chance", omakePassiveRaidStyleDescription),
        OMAKE_TEXT("化勁の隙を突いて、一撃を入れろ！", "to attack and strike", omakePassiveRaidStyleDescriptionEnd), 97},

    {QueueOmakeSceneChange, {SHEN_NAME4('Q', 'G', 'B', 'T'), 2}, 0x200000, OMAKE_PHOTO(32), 0xffff, OMAKE_TEXT("レスリング", "Wrestling", omakeWrestlingName),
        OMAKE_TEXT("グレッグ・モア", "Greg More: 2 pt match", omakeWrestlingDescription),
        OMAKE_TEXT("無制限２本先取！強力な投技に注意！", "against swift throws!", omakeWrestlingDescriptionEnd), 150},

    {QueueOmakeSceneChange, {SHEN_NAME4('Q', 'J', 'B', 'T'), 2}, 0x400000, OMAKE_PHOTO(31), 0xffff, OMAKE_TEXT("パンクラチオン", "Pancratium", omakePancratiumName),
        OMAKE_TEXT("ロッド・スタント", "Rod Stunt: 2 pt match", omakePancratiumDescription),
        OMAKE_TEXT("無制限２本先取！打撃力は凄まじいぞ", "against fierce blows!", omakePancratiumDescriptionEnd), 149},

    {QueueOmakeSceneChange, {SHEN_NAME4('Q', 'L', 'B', 'T'), 2}, 0x800000, OMAKE_PHOTO(33), 0xffff, OMAKE_TEXT("截拳道", "Jeet Kune Do", omakeJeetKuneDoName),
        OMAKE_TEXT("春燕（シュンエン）", "Chunyan: 2 pt match", omakeJeetKuneDoDescription),
        OMAKE_TEXT("無制限２本先取！鋭い足技は脅威だ！", "against agile kicks!", omakeJeetKuneDoDescriptionEnd), 151},

    {QueueOmakeSceneChange, {SHEN_NAME4('Q', 'K', 'B', 'T'), 3}, 0x1000000, OMAKE_PHOTO(34), 0xffff, OMAKE_TEXT("虎燕拳", "Tiger Swallow Style", omakeTigerSwallowStyleName),
        OMAKE_TEXT("白虎（ビャッコ）", "Baihu: 2 pt match ", omakeTigerSwallowStyleDescription),
        OMAKE_TEXT("無制限２本先取！連続技に気をつけろ", "against punch combos!", omakeTigerSwallowStyleDescriptionEnd), 152}};

static OmakeEntry omakeGameList[13], omakeMinigameList[13], omakeFightList[13];
OmakeCommand omakeColdBootCommand = {OMAKE_NO_AREA, 5};
#if defined(SHENMUE_TARGET_S2_DC_EU)
OmakeCategory omakeCategories[4] = {{35, omakeGameList}, {31, omakeMinigameList}, {2, omakeFightList}, {0, 0}};
#else
OmakeCategory omakeCategories[4] = {{17, omakeGameList}, {16, omakeMinigameList}, {2, omakeFightList}, {0, 0}};
#endif
static int omakeDrawEnabled, omakeVideoCount;
static float omakeSpriteAnimation;
#if defined(SHENMUE_TARGET_S2_DC_EU)
static int omakeLanguage;
#endif

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
#if defined(SHENMUE_TARGET_S2_DC_EU)
    switch (FTBL.GetGameLanguage()) {
    case 0: omakeLanguage = 0; break;
    case 2: omakeLanguage = 1; break;
    case 1: omakeLanguage = 2; break;
    case 3: omakeLanguage = 3; break;
    }
#endif
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
        FTBL.EventAssertNullsub(__FILE__, OMAKE_SOURCE_LINE(403, 413));
    } else {
        for (index = 0; index < 512; index++)
            if (!work->sprites[index].name)
                break;
        if (index >= 512) {
            FTBL.DebugLogToFileRemoved("EV_omak_Sprite_Create:Buffer Overflow\n");
            FTBL.EventAssertNullsub(__FILE__, OMAKE_SOURCE_LINE(410, 420));
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
        FTBL.EventAssertNullsub(__FILE__, OMAKE_SOURCE_LINE(430, 440));
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
        FTBL.EventAssertNullsub(__FILE__, OMAKE_SOURCE_LINE(444, 454));
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
        FTBL.EventAssertNullsub(__FILE__, OMAKE_SOURCE_LINE(606, 616));
    } else {
        for (index = 0; index < 128 && work->textTextures[index].name; index++)
            ;
        if (index >= 128) {
            FTBL.DebugLogToFileRemoved("EV_omak_MojiTexture_Create:Buffer Overflow\n");
            FTBL.EventAssertNullsub(__FILE__, OMAKE_SOURCE_LINE(613, 623));
        }
    }
    work->textTextures[index].name = name;
    work->textTextures[index].texture = FTBL.CreateCenteredWindowTextSurface(0, text, 512, colour, 0xf000,
#if defined(SHENMUE_TARGET_S2_DC_US) || defined(SHENMUE_TARGET_S2_DC_EU)
        0);
#else
        1);
#endif
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
        FTBL.EventAssertNullsub(__FILE__, OMAKE_SOURCE_LINE(652, 662));
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
        FTBL.EventAssertNullsub(__FILE__, OMAKE_SOURCE_LINE(681, 691));
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
            FTBL.EventAssertNullsub(__FILE__, OMAKE_SOURCE_LINE(719, 729));
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
#if defined(SHENMUE_TARGET_S2_DC_EU)
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'G', '0', '0'), work->textures[47], 0.0f);
#else
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'G', '0', '0'), work->textures[23], 0.0f);
#endif
    OMAKE_SETUP_SPRITE(SHEN_NAME4('T', 'T', 'B', 'G'), 320, 55, 512, 512);
#if defined(SHENMUE_TARGET_S2_DC_EU)
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('T', 'T', 'B', 'G'), work->textures[16], 1.0f);
#else
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('T', 'T', 'B', 'G'), work->textures[10], 1.0f);
#endif
    OMAKE_SETUP_SPRITE(SHEN_NAME4('T', 'I', 'T', 'L'), 320, 55, 512, 512);
#if defined(SHENMUE_TARGET_S2_DC_EU)
    switch (FTBL.GetGameLanguage()) {
    case 0: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('T', 'I', 'T', 'L'), work->textures[11], 2.0f); break;
    case 1: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('T', 'I', 'T', 'L'), work->textures[13], 2.0f); break;
    case 2: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('T', 'I', 'T', 'L'), work->textures[12], 2.0f); break;
    case 3: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('T', 'I', 'T', 'L'), work->textures[14], 2.0f); break;
    }
#else
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('T', 'I', 'T', 'L'), work->textures[8], 2.0f);
#endif
    OMAKE_SETUP_SPRITE(SHEN_NAME4('B', 'G', '1', '3'), 320, 129, 256, 256);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'G', '1', '3'), work->textures[0], 1.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('I', 'C', '1', '3'), 320, 129, 128, 128);
#if defined(SHENMUE_TARGET_S2_DC_EU)
    switch (FTBL.GetGameLanguage()) {
    case 0: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('I', 'C', '1', '3'), work->textures[35], 2.0f); break;
    case 1: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('I', 'C', '1', '3'), work->textures[37], 2.0f); break;
    case 2: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('I', 'C', '1', '3'), work->textures[36], 2.0f); break;
    case 3: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('I', 'C', '1', '3'), work->textures[38], 2.0f); break;
    }
#else
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('I', 'C', '1', '3'), work->textures[17], 2.0f);
#endif
    OMAKE_SETUP_SPRITE(SHEN_NAME4('B', 'G', '1', '2'), 214, 127, 128, 128);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'G', '1', '2'), work->textures[0], 1.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('I', 'C', '1', '2'), 214, 127, 64, 64);
#if defined(SHENMUE_TARGET_S2_DC_EU)
    switch (FTBL.GetGameLanguage()) {
    case 0: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('I', 'C', '1', '2'), work->textures[31], 2.0f); break;
    case 1: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('I', 'C', '1', '2'), work->textures[33], 2.0f); break;
    case 2: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('I', 'C', '1', '2'), work->textures[32], 2.0f); break;
    case 3: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('I', 'C', '1', '2'), work->textures[34], 2.0f); break;
    }
#else
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('I', 'C', '1', '2'), work->textures[16], 2.0f);
#endif
    OMAKE_SETUP_SPRITE(SHEN_NAME4('B', 'G', '1', '4'), 426, 127, 128, 128);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'G', '1', '4'), work->textures[0], 1.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('I', 'C', '1', '4'), 426, 127, 64, 64);
#if defined(SHENMUE_TARGET_S2_DC_EU)
    switch (FTBL.GetGameLanguage()) {
    case 0: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('I', 'C', '1', '4'), work->textures[2], 2.0f); break;
    case 1: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('I', 'C', '1', '4'), work->textures[4], 2.0f); break;
    case 2: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('I', 'C', '1', '4'), work->textures[3], 2.0f); break;
    case 3: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('I', 'C', '1', '4'), work->textures[5], 2.0f); break;
    }
#else
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('I', 'C', '1', '4'), work->textures[2], 2.0f);
#endif
    CreateOmakeSprite(work, SHEN_NAME4('B', 'G', '1', '0'));
    (&FindOmakeSprite(work, SHEN_NAME4('B', 'G', '1', '0'))->packet)->flags |= 0x400;
    (&FindOmakeSprite(work, SHEN_NAME4('B', 'G', '1', '0'))->packet)->flags |= 0x100;
    FTBL.AdjustScreenAspectRatio(&FindOmakeSprite(work, SHEN_NAME4('B', 'G', '1', '0'))->packet, 5, 155, 129, 128, 128);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'G', '1', '0'), work->textures[1], 1.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('B', 'G', '1', '6'), 485, 129, 128, 128);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'G', '1', '6'), work->textures[1], 1.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('M', 'N', 'B', 'X'), 320, 272, 1024, 1024);
#if defined(SHENMUE_TARGET_S2_DC_EU)
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('M', 'N', 'B', 'X'), work->textures[17], 1.0f);
#else
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('M', 'N', 'B', 'X'), work->textures[11], 1.0f);
#endif
    OMAKE_SETUP_SPRITE(SHEN_NAME4('S', 'C', 'B', 'X'), 562, 272, 256, 256);
#if defined(SHENMUE_TARGET_S2_DC_EU)
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('S', 'C', 'B', 'X'), work->textures[6], 2.0f);
#else
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('S', 'C', 'B', 'X'), work->textures[3], 2.0f);
#endif
    OMAKE_SETUP_SPRITE(SHEN_NAME4('S', 'C', 'B', 'A'), 570, 220, 128, 128);
#if defined(SHENMUE_TARGET_S2_DC_EU)
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('S', 'C', 'B', 'A'), work->textures[7], 3.0f);
#else
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('S', 'C', 'B', 'A'), work->textures[4], 3.0f);
#endif
    OMAKE_SETUP_SPRITE(SHEN_NAME4('B', 'G', 'M', '0'), 376, 210, 512, 512);
#if defined(SHENMUE_TARGET_S2_DC_EU)
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'G', 'M', '0'), work->textures[8], 2.0f);
#else
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'G', 'M', '0'), work->textures[5], 2.0f);
#endif
    OMAKE_SETUP_SPRITE(SHEN_NAME4('B', 'G', 'M', '1'), 376, 252, 512, 512);
#if defined(SHENMUE_TARGET_S2_DC_EU)
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'G', 'M', '1'), work->textures[8], 2.0f);
#else
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'G', 'M', '1'), work->textures[5], 2.0f);
#endif
    OMAKE_SETUP_SPRITE(SHEN_NAME4('B', 'G', 'M', '2'), 376, 294, 512, 512);
#if defined(SHENMUE_TARGET_S2_DC_EU)
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'G', 'M', '2'), work->textures[8], 2.0f);
#else
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'G', 'M', '2'), work->textures[5], 2.0f);
#endif
    OMAKE_SETUP_SPRITE(SHEN_NAME4('B', 'G', 'M', '3'), 376, 336, 512, 512);
#if defined(SHENMUE_TARGET_S2_DC_EU)
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'G', 'M', '3'), work->textures[8], 2.0f);
#else
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'G', 'M', '3'), work->textures[5], 2.0f);
#endif
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
#if defined(SHENMUE_TARGET_S2_DC_EU)
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('P', 'H', 'O', 'T'), work->textures[15], 2.0f);
#else
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('P', 'H', 'O', 'T'), work->textures[9], 2.0f);
#endif
    OMAKE_SETUP_SPRITE(SHEN_NAME4('P', 'F', 'R', 'M'), 133, 272, 256, 256);
#if defined(SHENMUE_TARGET_S2_DC_EU)
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('P', 'F', 'R', 'M'), work->textures[9], 3.0f);
#else
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('P', 'F', 'R', 'M'), work->textures[6], 3.0f);
#endif
    OMAKE_SETUP_SPRITE(SHEN_NAME4('I', 'S', 'B', 'G'), 323, 419, 512, 512);
#if defined(SHENMUE_TARGET_S2_DC_EU)
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('I', 'S', 'B', 'G'), work->textures[18], 1.0f);
#else
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('I', 'S', 'B', 'G'), work->textures[12], 1.0f);
#endif
    OMAKE_SETUP_SPRITE(SHEN_NAME4('I', 'N', 'S', '1'), 320, 395, 512, 32);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('I', 'N', 'S', '1'), FindOmakeText(work, OMAKE_UNKNOWN), 2.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('I', 'N', 'S', '2'), 320, 433, 512, 32);
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('I', 'N', 'S', '2'), FindOmakeText(work, OMAKE_UNKNOWN), 2.0f);
    OMAKE_SETUP_SPRITE(SHEN_NAME4('K', 'E', 'Y', 'S'), 43, 395, 64, 64);
#if defined(SHENMUE_TARGET_S2_DC_EU)
    switch (FTBL.GetGameLanguage()) {
    case 0: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('K', 'E', 'Y', 'S'), work->textures[27], 2.0f); break;
    case 1: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('K', 'E', 'Y', 'S'), work->textures[29], 2.0f); break;
    case 2: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('K', 'E', 'Y', 'S'), work->textures[28], 2.0f); break;
    case 3: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('K', 'E', 'Y', 'S'), work->textures[30], 2.0f); break;
    }
#else
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('K', 'E', 'Y', 'S'), work->textures[15], 1.0f);
#endif
    OMAKE_SETUP_SPRITE(SHEN_NAME4('B', 'T', 'N', 'A'), 593, 395, 64, 64);
#if defined(SHENMUE_TARGET_S2_DC_EU)
    switch (FTBL.GetGameLanguage()) {
    case 0: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'T', 'N', 'A'), work->textures[23], 2.0f); break;
    case 1: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'T', 'N', 'A'), work->textures[25], 2.0f); break;
    case 2: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'T', 'N', 'A'), work->textures[24], 2.0f); break;
    case 3: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'T', 'N', 'A'), work->textures[26], 2.0f); break;
    }
#else
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'T', 'N', 'A'), work->textures[14], 1.0f);
#endif
    OMAKE_SETUP_SPRITE(SHEN_NAME4('B', 'T', 'N', 'B'), 593, 420, 64, 64);
#if defined(SHENMUE_TARGET_S2_DC_EU)
    switch (FTBL.GetGameLanguage()) {
    case 0: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'T', 'N', 'B'), work->textures[19], 2.0f); break;
    case 1: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'T', 'N', 'B'), work->textures[21], 2.0f); break;
    case 2: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'T', 'N', 'B'), work->textures[20], 2.0f); break;
    case 3: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'T', 'N', 'B'), work->textures[22], 2.0f); break;
    }
#else
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'T', 'N', 'B'), work->textures[13], 1.0f);
#endif
    OMAKE_SETUP_SPRITE(SHEN_NAME4('B', 'T', 'N', 'H'), 48, 420, 128, 128);
#if defined(SHENMUE_TARGET_S2_DC_EU)
    switch (FTBL.GetGameLanguage()) {
    case 0: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'T', 'N', 'H'), work->textures[43], 2.0f); break;
    case 1: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'T', 'N', 'H'), work->textures[45], 2.0f); break;
    case 2: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'T', 'N', 'H'), work->textures[44], 2.0f); break;
    case 3: OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'T', 'N', 'H'), work->textures[46], 2.0f); break;
    }
#else
    OMAKE_SPRITE_TEXTURE(SHEN_NAME4('B', 'T', 'N', 'H'), work->textures[22], 1.0f);
#endif
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
#if defined(SHENMUE_TARGET_S2_DC_EU)
        index = omakeCategories[category].icon;
        switch (FTBL.GetGameLanguage()) {
        case 1: index += 2; break;
        case 2: index += 1; break;
        case 3: index += 3; break;
        }
#endif
        FindOmakeSprite(work, SHEN_NAME4('I', 'C', '1', '3'))->texture = work->textures[omakeCategories[category].icon];
        FindOmakeSprite(work, SHEN_NAME4('I', 'C', '1', '3'))->depth = 2.0f;
        category = work->selection->category - 1;
        if (category < 0) {
            category = -1;
            do
                category++;
            while (omakeCategories[category + 1].entries);
        }
#if defined(SHENMUE_TARGET_S2_DC_EU)
        category = omakeCategories[category].icon;
        switch (FTBL.GetGameLanguage()) {
        case 1: category += 2; break;
        case 2: category += 1; break;
        case 3: category += 3; break;
        }
        FindOmakeSprite(work, SHEN_NAME4('I', 'C', '1', '2'))->texture = work->textures[category];
#else
        FindOmakeSprite(work, SHEN_NAME4('I', 'C', '1', '2'))->texture = work->textures[omakeCategories[category].icon];
#endif
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
        FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '0'))->texture = work->textures[OMAKE_UI_TEXTURE(5, 8)];
        FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '0'))->depth = 2.0f;
        FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '1'))->texture = work->textures[OMAKE_UI_TEXTURE(5, 8)];
        FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '1'))->depth = 2.0f;
        FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '2'))->texture = work->textures[OMAKE_UI_TEXTURE(5, 8)];
        FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '2'))->depth = 2.0f;
        FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '3'))->texture = work->textures[OMAKE_UI_TEXTURE(5, 8)];
        FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '3'))->depth = 2.0f;
        switch (work->selection->selected[work->selection->category] - work->selection->scroll[work->selection->category]) {
        case 0:
            FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '0'))->texture = work->textures[OMAKE_UI_TEXTURE(21, 42)];
            FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '0'))->depth = 2.0f;
            break;
        case 1:
            FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '1'))->texture = work->textures[OMAKE_UI_TEXTURE(21, 42)];
            FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '1'))->depth = 2.0f;
            break;
        case 2:
            FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '2'))->texture = work->textures[OMAKE_UI_TEXTURE(21, 42)];
            FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '2'))->depth = 2.0f;
            break;
        case 3:
            FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '3'))->texture = work->textures[OMAKE_UI_TEXTURE(21, 42)];
            FindOmakeSprite(work, SHEN_NAME4('B', 'G', 'M', '3'))->depth = 2.0f;
            break;
        default:
            FTBL.EventAssertNullsub(__FILE__, OMAKE_SOURCE_LINE(1299, 1367));
            break;
        }
        if (work->animationRemaining > 0) {
            work->animationRemaining--;
            if (work->animationDirection == 0) {
                FindOmakeSprite(work, SHEN_NAME4('B', 'G', '1', '0'))->texture = work->textures[1];
                FindOmakeSprite(work, SHEN_NAME4('B', 'G', '1', '0'))->depth = 1.0f;
                FindOmakeSprite(work, SHEN_NAME4('B', 'G', '1', '6'))->texture = work->textures[OMAKE_UI_TEXTURE(20, 41)];
                FindOmakeSprite(work, SHEN_NAME4('B', 'G', '1', '6'))->depth = 1.0f;
            } else {
                FindOmakeSprite(work, SHEN_NAME4('B', 'G', '1', '0'))->texture = work->textures[OMAKE_UI_TEXTURE(20, 41)];
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
            FindOmakeSprite(work, SHEN_NAME4('P', 'H', 'O', 'T'))->texture = work->textures[OMAKE_UI_TEXTURE(9, 15)];
            FindOmakeSprite(work, SHEN_NAME4('P', 'H', 'O', 'T'))->depth = 2.0f;
            FTBL.EV_EventSetUpHelpIcon_0(130, 0);
        }
        if (work->refreshDescription) {
            work->refreshDescription = 0;
            if (HasOmakeText(work, SHEN_NAME4('I', 'N', 'S', '1')))
                QueueOmakeTextDelete(work, SHEN_NAME4('I', 'N', 'S', '1'));
            CreateOmakeText(work, SHEN_NAME4('I', 'N', 'S', '1'), OMAKE_LINE(OMAKE_SELECTED(work->selection)->description), 0xffff);
            if (HasOmakeText(work, SHEN_NAME4('I', 'N', 'S', '2')))
                QueueOmakeTextDelete(work, SHEN_NAME4('I', 'N', 'S', '2'));
            CreateOmakeText(work, SHEN_NAME4('I', 'N', 'S', '2'), OMAKE_LINE(OMAKE_SELECTED(work->selection)->descriptionEnd), 0xffff);
            if ((flags & OMAKE_SELECTED(work->selection)->requiredFlags) == OMAKE_SELECTED(work->selection)->requiredFlags) {
                FindOmakeSprite(work, SHEN_NAME4('I', 'N', 'S', '1'))->texture = FindOmakeText(work, SHEN_NAME4('I', 'N', 'S', '1'));
                FindOmakeSprite(work, SHEN_NAME4('I', 'N', 'S', '1'))->depth = 2.0f;
                FindOmakeSprite(work, SHEN_NAME4('I', 'N', 'S', '2'))->texture = FindOmakeText(work, SHEN_NAME4('I', 'N', 'S', '2'));
                FindOmakeSprite(work, SHEN_NAME4('I', 'N', 'S', '2'))->depth = 2.0f;
#if defined(SHENMUE_TARGET_S2_DC_EU)
                if (OMAKE_LINE(OMAKE_SELECTED(work->selection)->descriptionEnd) == 0) {
#else
                if (*OMAKE_LINE(OMAKE_SELECTED(work->selection)->descriptionEnd) == 0) {
#endif
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
                CreateOmakeText(work, text, OMAKE_LINE(OMAKE_ENTRY(work->selection->category, work->selection->scroll[work->selection->category] + row)->name),
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
