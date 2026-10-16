#include "s2/areas/area_ksht.hpp"
#include "s2/event_queued.hpp"
#include "s2/screen_film.hpp"
namespace shenmue::s2::areas {

void StartKshtZodiacConversation(void *) {
    SetupKshtConversationPose(true);
    EVT_AT_SetVoice("0521A005", 10); // @text EN: "An interesting sculpture=@" | JA: "不思議な置き物だな…"
    EVT_AT_SetVoice("0521B007", 70); // @text EN: "The mountains of Guilin,\nmade from stalactite." | JA: "鍾乳石で作った桂林の山。"
    EVT_AT_SetVoice("0521A006", 160); // @text EN: "Stalactite?\nFrom a cave nearby?" | JA: "鍾乳石って…\nこの辺の洞くつで？"
    EVT_AT_SetVoice("0521B008", 280); // @text EN: "It was brought from the stone pit." | JA: "石切り場から持ってきたの。"
    EVT_AT_SetVoice("0521B009", 370); // @text EN: "Father likes to look at stones." | JA: "父は、石を見るのが好きだから。"
    EVT_AT_SetVoice("0521A007", 490); // @text EN: "Stones?" | JA: "石を…？"
    EVT_AT_SetVoice("0521B010", 550); // @text EN: "He says he hears their voices.\nAlthough, I don't know what he means." | JA: "声が聞こえるんだって…\n私にはわからないけど。"
    EVT_AT_SetVoice("0521A008", 730); // @text EN: "Voices from the stone=@" | JA: "石の声か…"
    EVT_AT_SetCam(210, KSHT_DUMMY, KSHT_DUMMY, 0, 0, 60);
    EVT_AT_SetCam(220, KSHT_DUMMY, KSHT_DUMMY, 0, 0, 280);
    EVT_AT_SetCam(230, KSHT_DUMMY, KSHT_DUMMY, 0, 0, 550);
}

void StartKshtWeatherConversation(void *) {
    SetupKshtConversationPose(true);
    EVT_AT_SetVoice("0521A052", 10); // @text EN: "A flute=@" | JA: "横笛か…"
    EVT_AT_SetVoice("0521B034", 55); // @text EN: "My father made it." | JA: "父が作ったの。"
    EVT_AT_SetVoice("0521B035", 115); // @text EN: "He made it from a Shenmue branch\nwhen I was a baby." | JA: "私が赤ん坊の頃、\n莎木の枝で。"
    EVT_AT_SetVoice("0521A053", 205); // @text EN: "From a Shenmue branch=@" | JA: "莎木の木で…"
    EVT_AT_SetVoice("0521B036", 265); // @text EN: "I always stopped crying\nwhenever I heard its sound." | JA: "この笛の音を聞くと、\nどんなときでも泣きやんだって…"
    EVT_AT_SetVoice("0521B037", 385); // @text EN: "My father told me." | JA: "父が言ってた。"
    EVT_AT_SetVoice("0521A054", 430); // @text EN: "It was like a lullaby." | JA: "子守り歌みたいだな…"
    EVT_AT_SetVoice("0521B038", 475); // @text EN: "Yes." | JA: "うん。"
    EVT_AT_SetCam(710, KSHT_DUMMY, KSHT_DUMMY, 0, 0, 55);
    EVT_AT_SetCam(720, KSHT_DUMMY, KSHT_DUMMY, 0, 0, 430);
    EVT_AT_SetCam(730, KSHT_DUMMY, KSHT_DUMMY, 0, 0, 455);
    EVT_AT_SetCam(740, KSHT_DUMMY, KSHT_DUMMY, 0, 0, 520);
}

void StartKshtNamesConversation(void *) {
    SetupKshtConversationPose(true);
    EVT_AT_SetVoice("0521A001", 10); // @text EN: "This painting?" | JA: "この絵は？"
    EVT_AT_SetVoice("0521B001", 70); // @text EN: "Somewhere in Guilin." | JA: "桂林のどこかよ。"
    EVT_AT_SetVoice("0521A002", 160); // @text EN: "Really." | JA: "そうか。"
    EVT_AT_SetVoice("0521B002", 220); // @text EN: "I heard these mountains are scary." | JA: "恐い山だと聞いたわ。"
    EVT_AT_SetVoice("0521A003", 340); // @text EN: "Scary?" | JA: "恐い？"
    EVT_AT_SetVoice("0521B003", 400); // @text EN: "Those who set foot in these\nmountains have never returned=@" | JA: "この山に入って、\n帰ってきた人はいないって…"
    EVT_AT_SetVoice("0521B004", 550); // @text EN: "=@it's an old tale." | JA: "古くから伝わってる。"
    EVT_AT_SetVoice("0521A004", 670); // @text EN: "Why?" | JA: "なぜ？"
    EVT_AT_SetVoice("0521B005", 730); // @text EN: "I don't know=@" | JA: "わからない…"
    EVT_AT_SetVoice("0521B006", 790); // @text EN: "But no one goes near these mountains today." | JA: "でも、今じゃ誰も\nその山に入らないわ。"
    EVT_AT_SetCam(310, KSHT_DUMMY, KSHT_DUMMY, 0, 0, 180);
    EVT_AT_SetCam(320, KSHT_DUMMY, KSHT_DUMMY, 0, 0, 350);
}

void StartKshtBadFeelingConversation(void *) {
    SetupKshtConversationPose(true);
    EVT_AT_SetVoice("0521A009", 10); // @text EN: "It looks very old." | JA: "ずいぶん古い絵だな。"
    EVT_AT_SetVoice("0521B011", 70); // @text EN: "My father has had that tapestry\nfor a long time." | JA: "その織物、父が\n昔から持ってるの。"
    EVT_AT_SetVoice("0521A010", 190); // @text EN: "Where is this place?" | JA: "この場所は…？"
    EVT_AT_SetVoice("0521B012", 250); // @text EN: "Luoyang." | JA: "洛陽よ。"
    EVT_AT_SetVoice("0521A011", 295); // @text EN: "Luoyang=@" | JA: "洛陽…"
    EVT_AT_SetVoice("0521B013", 340); // @text EN: "An old city that flourished during\nthe Tang Dynasty." | JA: "唐の時代に栄えた古い都…"
    EVT_AT_SetVoice("0521B014", 460); // @text EN: "The sunset colors\nthe whole city beautifully." | JA: "夕陽の輝きが、とても\nきれいに街を染める。"
    EVT_AT_SetVoice("0521A012", 610); // @text EN: "Have you ever been there?" | JA: "行ったことあるのか？"
    EVT_AT_SetVoice("0521B015", 690); // @text EN: "No. But for some reason it gives me\na nostalgic feeling." | JA: "ううん、だけど\nなつかしい感じがするの。"
    EVT_AT_SetVoice("0521A013", 840); // @text EN: "Nostalgic feeling?" | JA: "なつかしい？"
    EVT_AT_SetVoice("0521B016", 905); // @text EN: "I see it when I close my eyes." | JA: "目を閉じると浮かんでくるの。"
    EVT_AT_SetVoice("0521B017", 1000); // @text EN: "The landscape of the city\nand the people bustling by=@" | JA: "洛陽の景色や…\n行きかう人々の息づかいが…"
    EVT_AT_SetVoice("0521A014", 1150); // @text EN: "Luoyang=@" | JA: "洛陽か…"
    EVT_AT_SetCam(410, KSHT_DUMMY, KSHT_DUMMY, 0, 0, 300);
    EVT_AT_SetCam(420, KSHT_DUMMY, KSHT_DUMMY, 0, 0, 600);
    EVT_AT_SetCam(430, KSHT_DUMMY, KSHT_DUMMY, 0, 0, 650);
    EVT_AT_SetCam(440, KSHT_DUMMY, KSHT_DUMMY, 0, 0, 1100);
    QueueKshtWaitEye(60, 1, 1, 905);
    QueueKshtWaitEye(30, 0, 0, 1150);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
}

void StartKshtDreamConversation(void *) {
    SetupKshtConversationPose(true);
    EVT_AT_SetVoice("0521A015", 10); // @text EN: "What's this?" | JA: "これは？"
    EVT_AT_SetVoice("0521B018", 70); // @text EN: "A blanket." | JA: "ひざかけよ。"
    EVT_AT_SetVoice("0521B019", 130); // @text EN: "I'm making one for my father." | JA: "今、父のを織ってるの。"
    EVT_AT_SetVoice("0521A016", 250); // @text EN: "You are?" | JA: "莎花が？"
    EVT_AT_SetVoice("0521B020", 310); // @text EN: "Yes.\nAn old lady from the village taught me." | JA: "ええ。村のおばあさんに\n習ったの。"
    EVT_AT_SetVoice("0521A017", 430); // @text EN: "An old lady?" | JA: "おばあさん？"
    EVT_AT_SetVoice("0521B021", 490); // @text EN: "One of the village elders." | JA: "村の長老よ。"
    EVT_AT_SetVoice("0521B022", 550); // @text EN: "She has bad eyesight\nbut she knows about everything." | JA: "目が不自由だけど\nなんでも知ってるの。"
    EVT_AT_SetVoice("0521B023", 670); // @text EN: "She knows about the forest\nand the history of the village." | JA: "森のことも…村の歴史も…"
    EVT_AT_SetVoice("0521B024", 790); // @text EN: "She might know about the person\nthat you're looking for." | JA: "リョウの探してる人のことも\n知ってるかも。"
    EVT_AT_SetVoice("0521A018", 910); // @text EN: "The old lady in the village." | JA: "村のおばあさんか…"
    EVT_AT_SetVoice("0521B025", 1000); // @text EN: "You should go visit her sometime." | JA: "一度会ってみるといいわ。"
    EVT_AT_SetVoice("0521A019", 1120); // @text EN: "Yeah." | JA: "ああ。"
    EVT_AT_SetCam(510, KSHT_DUMMY, KSHT_DUMMY, 0, 0, 95);
    EVT_AT_SetCam(520, KSHT_DUMMY, KSHT_DUMMY, 0, 0, 205);
    EVT_AT_SetCam(530, KSHT_DUMMY, KSHT_DUMMY, 0, 0, 475);
    EVT_AT_SetCam(540, KSHT_DUMMY, KSHT_DUMMY, 0, 0, 775);
    EVT_AT_SetCam(550, KSHT_DUMMY, KSHT_DUMMY, 0, 0, 905);
}

void StartKshtDestinyConversation(void *) {
    SetupKshtConversationPose(true);
    EVT_AT_SetVoice("0521A046", 10); // @text EN: "This book=@" | JA: "この本は…"
    EVT_AT_SetVoice("0521B026", 70); // @text EN: "Book on constellations.\nThe Xingxiu are explained." | JA: "星座の本よ。\n星宿が書かれている。"
    EVT_AT_SetVoice("0521A047", 190); // @text EN: "Xingxiu?" | JA: "星宿？"
    EVT_AT_SetVoice("0521B027", 250); // @text EN: "The stars that affect the fates of people." | JA: "人の運命に関わる星のこと。"
    EVT_AT_SetVoice("0521B028", 370); // @text EN: "The Big Dipper is one of them." | JA: "たとえば北斗七星もそう。"
    EVT_AT_SetVoice("0521A048", 430); // @text EN: "The Big Dipper?" | JA: "北斗七星が？"
    EVT_AT_SetVoice("0521B029", 490); // @text EN: "The North Star is said to be the god\nthat deals with death." | JA: "北斗は死をつかさどる神様って\n言われているわ。"
    EVT_AT_SetVoice("0521A049", 580); // @text EN: "The harbinger of death=@" | JA: "死をつかさどる神…"
    EVT_AT_SetVoice("0521B030", 640); // @text EN: "When the North Star flickers,\nsomething bad happens=@" | JA: "北斗がまたたく夜は\nよくないことが起きるって…"
    EVT_AT_SetVoice("0521B031", 790); // @text EN: "Like last night." | JA: "昨日の夜もそう。"
    EVT_AT_SetVoice("0521B032", 850); // @text EN: "I was watching the Big Dipper\nwhen I got this bad feeling=@" | JA: "北斗七星を見ていたら\n何か胸さわぎがして…"
    EVT_AT_SetVoice("0521A050", 970); // @text EN: "Bad feeling?" | JA: "胸さわぎ…？"
    EVT_AT_SetVoice("0521B033", 1015); // @text EN: "That's why I couldn't go to sleep\nand started singing." | JA: "だから寝つけなくて\n歌っていたの。"
    EVT_AT_SetVoice("0521A051", 1135); // @text EN: "So that was why=@" | JA: "そうだったのか…"
    EVT_AT_SetCam(810, KSHT_DUMMY, KSHT_DUMMY, 0, 0, 190);
    EVT_AT_SetCam(820, KSHT_DUMMY, KSHT_DUMMY, 0, 0, 250);
    EVT_AT_SetCam(830, KSHT_DUMMY, KSHT_DUMMY, 0, 0, 490);
    EVT_AT_SetCam(840, KSHT_DUMMY, KSHT_DUMMY, 0, 0, 640);
    EVT_AT_SetCam(850, KSHT_DUMMY, KSHT_DUMMY, 0, 0, 970);
    EVT_AT_SetCam(860, KSHT_DUMMY, KSHT_DUMMY, 0, 0, 1015);
}
} // namespace shenmue::s2::areas
