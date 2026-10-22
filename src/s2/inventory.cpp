#include "s2/inventory.hpp"

#include "s2/ui.hpp"

#include <algorithm>
#include <array>

namespace shenmue::s2 {
std::int32_t g_inventoryStockBiasEnabled = 0;

namespace {



constexpr InventoryItemDefinition keyItems[] = {
    {11, 20, 200, 1200, nullptr, "UIITEMOVMA", 0x2U, 0x0U}, // @text EN: "Overall Map" | JA: "全体マップ"
    {12, 0, 200, 1200, nullptr, "UIITEMNOTE", 0x2U, 0x0U}, // @text EN: "Notebook" | JA: "メモ帳"
    {3, 21, 300, 800, nullptr, "UIITEMTOKE", 0x2U, 0x0U}, // @text EN: "Token(s)" | JA: "コイン"
    {10, 2, 1000, 1000, nullptr, "UIITEMWATC", 0x2U, 0x0U}, // @text EN: "Watch" | JA: "時計"
    {13, 3, 600, 1300, "TUBA1T1", "UIITEMSWHA", 0x200U, 0x0U}, // @text EN: "Sword Handguard" | JA: "刀の鍔"
    {14, 4, 400, 650, "PNX02H6", "UIITEMPHMI", 0x200U, 0x0U}, // @text EN: "Phoenix Mirror" | JA: "鳳凰鏡"
    {22, 5, 200, 450, "LMAL200", "UIITEMCIL0", 0x200U, 0x0U}, // @text EN: "Chen Intro Letter" | JA: "陳大人の紹介状"
    {15, 6, 530, 1140, "OMMR1M1", "UIITEMAMUL", 0x200U, 0x0U}, // @text EN: "Amulet" | JA: "お守り"
    {16, 7, 480, 930, "HAFK100", "UIITEMWHLE", 0x200U, 0x0U}, // @text EN: "White Leaf" | JA: "白い葉"
    {20, 24, 800, 1600, "ZIPOHHV", "UIITEMHEZI", 0x200U, 0x0U}, // @text EN: "Heavens Lighter" | JA: "ヘヴンズライター"
    {17, 22, 800, 1500, "MAGA2WH", "UIITEMCSB0", 0x0U, 0x0U}, // @text EN: "Comma Shaped Bead" | JA: "勾玉"
    {21, 10, 240, 480, "QDKJASI", "UIITEMFDB0", 0x210U, 0x0U}, // @text EN: "Four Divine Beasts" | JA: "四神獣の図面"
    {18, 11, 1250, 3100, "IKSM2OM", "UIITEMHECO", 0x202U, 0x0U}, // @text EN: "Heads Coin" | JA: "コイン表"
    {518, 12, 1250, 3100, "IKSM2UR", "UIITEMTACO", 0x202U, 0x0U}, // @text EN: "Tails Coin" | JA: "コイン裏"
    {19, 13, 450, 1200, "OMMRHH1", "UIITEMXUAM", 0x202U, 0x0U}, // @text EN: "Xun's Amulet" | JA: "薫のお守り"
    {512, 25, 525, 1300, "MEDA2GL", "UIITEMBGM0", 0x202U, 0x0U}, // @text EN: "Big Gold Medal" | JA: "大きな金メダル"
    {513, 26, 525, 1300, "MEDA2SV", "UIITEMBSM0", 0x202U, 0x0U}, // @text EN: "Big Silver Medal" | JA: "大きな銀メダル"
    {514, 27, 525, 1300, "MEDA2CP", "UIITEMBBM0", 0x202U, 0x0U}, // @text EN: "Big Bronze Medal" | JA: "大きな銅メダル"
    {23, 1, 200, 420, "LETT1L1", "UIITEMLTF0", 0x200U, 0x0U}, // @text EN: "Letter to Father" | JA: "親父宛の手紙"
    {24, 14, 360, 900, "TMEM200", "UIITEMMYPA", 0x8U, 0x0U}, // @text EN: "Mysterious Paper" | JA: "武林書の紙"
    {24, 41, 360, 900, "TMEM200", "UIITEMCHSI", 0x8U, 0x0U}, // @text EN: "Chawan Sign" | JA: "茶碗陣のメモ"
    {25, 35, 260, 460, "WS00ALE", "UIITEMZOLE", 0x200U, 0x0U}, // @text EN: "Zongquan Letter" | JA: "宗泉の手紙"
    {26, 36, 240, 480, "TYOM28J", "UIITEMDJD0", 0x200U, 0x0U}, // @text EN: "Dou Jiang Diner" | JA: "８時豆将軒"
    {26, 37, 240, 480, "BSKM2HO", "UIITEMCOGH", 0x200U, 0x0U}, // @text EN: "Come Over Guest House" | JA: "方来旅社"
    {26, 38, 240, 480, "BKOI200", "UIITEMMMP0", 0x200U, 0x0U}, // @text EN: "Man Mo Park" | JA: "文武公園"
    {26, 40, 240, 480, "TOSM29J", "UIITEMMMP0", 0x200U, 0x0U}, // @text EN: "Man Mo Park" | JA: "文武公園"
    {26, 39, 240, 460, "QEME203", "UIITEMMCB0", 0x200U, 0x0U}, // @text EN: "Moon Child Bldg." | JA: "百子楼５０３"
    {4, 9000, 330, 530, nullptr, "UIGNRLQSTN", 0x0U, 0x0U}, // @text EN: "? ? ?" | JA: "？？？"
    {263, 71, 330, 530, "QDKJAN5", "UIITEMWHTI", 0x210U, 0x0U}, // @text EN: "White Tiger" | JA: "白虎の棒"
    {258, 72, 330, 530, "QDKJAN3", "UIITEMSNTO", 0x200U, 0x0U}, // @text EN: "Snake Tortoise" | JA: "玄武の棒"
    {260, 73, 330, 530, "QDKJAN1", "UIITEMBLDR", 0x210U, 0x0U}, // @text EN: "Blue Dragon" | JA: "青龍の棒"
    {256, 74, 330, 530, "QDKJAN4", "UIITEMFISH", 0x210U, 0x0U}, // @text EN: "Fish" | JA: "魚の棒"
    {257, 76, 330, 530, "QDKJAN7", "UIITEMROOS", 0x210U, 0x0U}, // @text EN: "Rooster" | JA: "鶏の棒"
    {261, 77, 330, 530, "QDKJAN0", "UIITEMPHOE", 0x210U, 0x0U}, // @text EN: "Phoenix" | JA: "朱雀の棒"
    {262, 78, 330, 530, "QDKJAN6", "UIITEMWIBO", 0x210U, 0x0U}, // @text EN: "Wild Boar" | JA: "猪の棒"
    {259, 79, 330, 530, "QDKJAN8", "UIITEMGOAT", 0x210U, 0x0U}, // @text EN: "Goat" | JA: "山羊の棒"
    {264, 29, 600, 1000, "QFIXZK1", "UIITEMEK29", 0x200U, 0x0U}, // @text EN: "EV Key 29" | JA: "ＥＶ鍵２９"
    {264, 30, 600, 1000, "QFIXZK2", "UIITEMEK10", 0x200U, 0x0U}, // @text EN: "EV Key 10" | JA: "ＥＶ鍵１０"
    {264, 31, 600, 1000, "QFIXZK3", "UIITEMEK46", 0x200U, 0x0U}, // @text EN: "EV Key 46" | JA: "ＥＶ鍵４６"
    {264, 32, 600, 1000, "QFIXZK4", "UIITEMEK09", 0x200U, 0x0U}, // @text EN: "EV Key 9" | JA: "ＥＶ鍵９"
    {264, 33, 600, 1000, "QFIXZK5", "UIITEMEK18", 0x200U, 0x0U}, // @text EN: "EV Key 18" | JA: "ＥＶ鍵１８"
    {264, 34, 600, 1000, "QFIXZK6", "UIITEMEK21", 0x200U, 0x0U}, // @text EN: "EV Key 21" | JA: "ＥＶ鍵２１"
    {517, 80, 100, 180, "AHIR2SN", "UIITEMDUCK", 0x0U, 0x0U}, // @text EN: "Duck" | JA: "アヒル"
};

constexpr InventoryItemDefinition capsuleToys[] = {
    {2560, ITEM_AKIRA1, 300, 540, "GACIAK1", "UIGACHAKI1", 0x1U, 0x0U},       // @text EN: "Akira 1" | JA: "アキラ１"
    {2562, ITEM_AKIRA2, 300, 540, "GACIAK2", "UIGACHAKI2", 0x1U, 0x0U},       // @text EN: "Akira 2" | JA: "アキラ２"
    {2568, ITEM_JACKY1, 300, 540, "GACOJK1", "UIGACHJAC1", 0x1U, 0x0U},       // @text EN: "Jacky 1" | JA: "ジャッキー１"
    {2570, ITEM_JACKY2, 300, 540, "GACOJK2", "UIGACHJAC2", 0x1U, 0x0U},       // @text EN: "Jacky 2" | JA: "ジャッキー２"
    {2564, ITEM_SARAH1, 300, 540, "GACOSA1", "UIGACHSAR1", 0x1U, 0x0U},       // @text EN: "Sarah 1" | JA: "サラ１"
    {2565, ITEM_SARAH2, 300, 530, "GACOSA2", "UIGACHSAR2", 0x1U, 0x0U},       // @text EN: "Sarah 2" | JA: "サラ２"
    {2584, ITEM_LAU1, 300, 540, "GACOLA1", "UIGACHLAU1", 0x1U, 0x0U},         // @text EN: "Lau 1" | JA: "ラウ１"
    {2585, ITEM_LAU2, 300, 540, "GACOLA2", "UIGACHLAU2", 0x1U, 0x0U},         // @text EN: "Lau 2" | JA: "ラウ２"
    {2577, ITEM_PAI1, 300, 540, "GACOPA1", "UIGACHPAI1", 0x1U, 0x0U},         // @text EN: "Pai 1" | JA: "パイ１"
    {2576, ITEM_PAI2, 280, 500, "GACOPA2", "UIGACHPAI2", 0x1U, 0x0U},         // @text EN: "Pai 2" | JA: "パイ２"
    {2572, ITEM_WOLF1, 300, 540, "GACGWF1", "UIGACHWOL1", 0x1U, 0x0U},        // @text EN: "Wolf 1" | JA: "ウルフ１"
    {2573, ITEM_WOLF2, 300, 540, "GACGWF2", "UIGACHWOL2", 0x1U, 0x0U},        // @text EN: "Wolf 2" | JA: "ウルフ２"
    {2580, ITEM_JEFFRY1, 300, 540, "GACGJF1", "UIGACHJEF1", 0x1U, 0x0U},      // @text EN: "Jeffry 1" | JA: "ジェフリー１"
    {2581, ITEM_JEFFRY2, 300, 540, "GACGJF2", "UIGACHJEF2", 0x1U, 0x0U},      // @text EN: "Jeffry 2" | JA: "ジェフリー２"
    {2588, ITEM_KAGE1, 300, 540, "GACIKA1", "UIGACHKAG1", 0x1U, 0x0U},        // @text EN: "Kage 1" | JA: "カゲ１"
    {2589, ITEM_KAGE2, 300, 540, "GACIKA2", "UIGACHKAG2", 0x1U, 0x0U},        // @text EN: "Kage 2" | JA: "カゲ２"
    {2849, ITEM_DURAL_SILVER, 300, 540, "GACRDR1", "UIGACHDURS", 0x1U, 0x0U}, // @text EN: "Dural Silver" | JA: "デュラルシルバー"
    {2848, ITEM_DURAL_GOLD, 260, 430, "GACRDR2", "UIGACHDURG", 0x1U, 0x0U},   // @text EN: "Dural Gold" | JA: "デュラルゴールド"
    {2569, 4018, 260, 530, "GACM401", "UIGACHWOOD", 0x1U, 0x0U},              // @text EN: "Wooden man" | JA: "木人"
    {2614, 4019, 260, 520, "GACRREN", "UIGACHRENT", 0x1U, 0x0U},              // @text EN: "Rent-A-Hero" | JA: "レンタヒーロー"
    {3112, 4020, 300, 750, "GACK6TZ", "UIGACHTRYZ", 0x301U, 0x0U},            // @text EN: "Try-Z" | JA: "ＴＲＹ－Ｚ"
    {2861, 4021, 280, 700, "GACRPAN", "UIGACHSOLO", 0x1U, 0x0U},              // @text EN: "Solo Wing" | JA: "ソロ・ウイング"
    {2563, 4024, 260, 540, "GACIAKA", "UIGACHKAK1", 0x1U, 0x0U},              // @text EN: "Kids Akira 1" | JA: "キッズアキラ１"
    {2593, 4025, 260, 540, "GACIAKB", "UIGACHKAK2", 0x1U, 0x0U},              // @text EN: "Kids Akira 2" | JA: "キッズアキラ２"
    {2571, 4026, 260, 540, "GACGJKA", "UIGACHKJA1", 0x1U, 0x0U},              // @text EN: "Kids Jacky 1" | JA: "キッズジャッキー１"
    {2601, 4027, 260, 540, "GACGJKB", "UIGACHKJA2", 0x1U, 0x0U},              // @text EN: "Kids Jacky 2" | JA: "キッズジャッキー２"
    {2566, 4028, 280, 580, "GACOSAA", "UIGACHKSA1", 0x1U, 0x0U},              // @text EN: "Kids Sarah 1" | JA: "キッズサラ１"
    {2567, 4029, 300, 720, "GACOSAB", "UIGACHKSA2", 0x1U, 0x0U},              // @text EN: "Kids Sarah 2" | JA: "キッズサラ２"
    {2587, 4030, 300, 580, "GACOLAA", "UIGACHKLA1", 0x1U, 0x0U},              // @text EN: "Kids Lau 1" | JA: "キッズラウ１"
    {2586, 4031, 300, 580, "GACOLAB", "UIGACHKLA2", 0x1U, 0x0U},              // @text EN: "Kids Lau 2" | JA: "キッズラウ２"
    {2579, 4032, 280, 540, "GACOPAA", "UIGACHKPA1", 0x1U, 0x0U},              // @text EN: "Kids Pai 1" | JA: "キッズパイ１"
    {2578, 4033, 280, 540, "GACOPAB", "UIGACHKPA2", 0x1U, 0x0U},              // @text EN: "Kids Pai 2" | JA: "キッズパイ２"
    {2574, 4034, 280, 540, "GACIWLA", "UIGACHKWO1", 0x1U, 0x0U},              // @text EN: "Kids Wolf 1" | JA: "キッズウルフ１"
    {2611, 4035, 280, 540, "GACIWLB", "UIGACHKWO2", 0x1U, 0x0U},              // @text EN: "Kids Wolf 2" | JA: "キッズウルフ２"
    {2582, 4036, 280, 540, "GACGJFA", "UIGACHKJE1", 0x1U, 0x0U},              // @text EN: "Kids Jeffry 1" | JA: "キッズジェフリー１"
    {2609, 4037, 280, 540, "GACGJFB", "UIGACHKJE2", 0x1U, 0x0U},              // @text EN: "Kids Jeffry 2" | JA: "キッズジェフリー２"
    {2590, 4038, 280, 540, "GACIKAA", "UIGACHKKA1", 0x1U, 0x0U},              // @text EN: "Kids Kage 1" | JA: "キッズカゲ１"
    {2595, 4039, 320, 580, "GACIKAB", "UIGACHKKA2", 0x1U, 0x0U},              // @text EN: "Kids Kage 2" | JA: "キッズカゲ２"
    {2575, 4040, 280, 540, "GACGSNA", "UIGACHKSH1", 0x1U, 0x0U},              // @text EN: "Kids Shun Di 1" | JA: "キッズシュン１"
    {2583, 4041, 280, 580, "GACGSNB", "UIGACHKSH2", 0x1U, 0x0U},              // @text EN: "Kids Shun Di 2" | JA: "キッズシュン２"
    {2591, 4042, 280, 540, "GACILIA", "UIGACHKLI1", 0x1U, 0x0U},              // @text EN: "Kids Lion 1" | JA: "キッズリオン１"
    {2603, 4043, 280, 540, "GACILIB", "UIGACHKLI2", 0x1U, 0x0U},              // @text EN: "Kids Lion 2" | JA: "キッズリオン２"
    {2617, 4044, 280, 540, "GACIDUA", "UIGACHKDUS", 0x101U, 0x0U},            // @text EN: "Kids Dural S" | JA: "キッズデュラルＳ"
    {3081, 4045, 280, 640, "GACIDUB", "UIGACHKDUG", 0x1U, 0x0U},              // @text EN: "Kids Dural G" | JA: "キッズデュラルＧ"
    {2599, 4046, 280, 580, "GACRNT1", "UIGACHNIG1", 0x1U, 0x0U},              // @text EN: "NiGHTS 1" | JA: "ナイツ１"
    {2604, 4047, 280, 580, "GACRNT2", "UIGACHNIG2", 0x1U, 0x0U},              // @text EN: "NiGHTS 2" | JA: "ナイツ２"
    {3128, 4048, 280, 500, "GACRSO1", "UIGACHSON1", 0x1U, 0x0U},              // @text EN: "Sonic 1" | JA: "ソニック１"
    {2817, 4049, 260, 540, "GACRSO2", "UIGACHSON2", 0x1U, 0x0U},              // @text EN: "Sonic 2" | JA: "ソニック２"
    {2819, 4050, 250, 480, "GACRSSC", "UIGACHSUSO", 0x1U, 0x0U},              // @text EN: "Super Sonic" | JA: "スーパーソニック"
    {2824, 4051, 300, 600, "GACRTL1", "UIGACHTAIL", 0x1U, 0x0U},              // @text EN: "Tails" | JA: "テイルス"
    {2825, 4052, 280, 580, "GACRNUC", "UIGACHKNUC", 0x1U, 0x0U},              // @text EN: "Knuckles" | JA: "ナックルズ"
    {2835, 4053, 280, 540, "GACRBAK", "UIGACHBARK", 0x1U, 0x0U},              // @text EN: "Berk" | JA: "バーク"
    {2834, 4054, 280, 540, "GACRBIN", "UIGACHBEAN", 0x1U, 0x0U},              // @text EN: "Bean" | JA: "ビーン"
    {2832, 4055, 280, 540, "GACRFNG", "UIGACHFANG", 0x1U, 0x0U},              // @text EN: "Fang" | JA: "ファング"
    {2826, 4056, 280, 540, "GACRAMY", "UIGACHAMY0", 0x1U, 0x0U},              // @text EN: "Amy" | JA: "エミー"
    {2833, 4057, 280, 540, "GACRESP", "UIGACHESPI", 0x1U, 0x0U},              // @text EN: "Espio" | JA: "エスピオ"
    {2818, 4058, 260, 540, "GACRMET", "UIGACHMESO", 0x1U, 0x0U},              // @text EN: "Metal Sonic" | JA: "メタルソニック"
    {2827, 4059, 300, 580, "GACREGG", "UIGACHEGGM", 0x101U, 0x0U},            // @text EN: "Eggman" | JA: "エッグマン"
    {2618, 4060, 280, 540, "GACK6HO", "UIGACHHAO1", 0x301U, 0x0U},            // @text EN: "Hang-On 1" | JA: "ハングオン１"
    {2622, 4061, 280, 540, "GACK6HH", "UIGACHHAO2", 0x301U, 0x0U},            // @text EN: "Hang-On 2" | JA: "ハングオン２"
    {2621, 4062, 280, 540, "GACK6HY", "UIGACHHAO3", 0x301U, 0x0U},            // @text EN: "Hang-On 3" | JA: "ハングオン３"
    {2619, 4063, 280, 540, "GACK6HB", "UIGACHHAO4", 0x301U, 0x0U},            // @text EN: "Hang-On 4" | JA: "ハングオン４"
    {2623, 4064, 280, 540, "GACK6HG", "UIGACHHAO5", 0x301U, 0x0U},            // @text EN: "Hang-On 5" | JA: "ハングオン５"
    {3079, 4065, 280, 540, "GACK6HK", "UIGACHHAOG", 0x1U, 0x0U},              // @text EN: "Hang-On G" | JA: "ハングオンＧ"
    {2828, 4066, 280, 540, "GACK6SP", "UIGACHSPHA", 0x301U, 0x0U},            // @text EN: "Space Harrier" | JA: "スペースハリアー"
    {3089, 4067, 280, 580, "GACRLOP", "UIGACHRUBE", 0x1U, 0x0U},              // @text EN: "Ruber" | JA: "ルーパー"
    {2606, 4068, 370, 900, "GACR0BN", "UIGACHBIN1", 0x1U, 0x0U},              // @text EN: "Binsbein 1" | JA: "ビンズビーン１"
    {3090, 4069, 370, 900, "GACR0B2", "UIGACHBIN2", 0x1U, 0x0U},              // @text EN: "Binsbein 2" | JA: "ビンズビーン２"
    {3091, 4070, 370, 900, "GACR0B3", "UIGACHBIN3", 0x1U, 0x0U},              // @text EN: "Binsbein 3" | JA: "ビンズビーン３"
    {2856, 4071, 370, 900, "GACRGB1", "UIGACHBBUL", 0x1U, 0x0U},              // @text EN: "B.B. Ultra" | JA: "Ｂ.Ｂウルトラ"
    {3074, 4072, 280, 540, "GACODG1", "UIGACHPOCH", 0x1U, 0x0U},              // @text EN: "Pochi" | JA: "ポチ"
    {3072, 4073, 280, 540, "GACODG2", "UIGACHJOHN", 0x1U, 0x0U},              // @text EN: "John" | JA: "ジョン"
    {3073, 4074, 300, 600, "GACODG3", "UIGACHKELL", 0x1U, 0x0U},              // @text EN: "Kelly" | JA: "ケリー"
    {2879, 4075, 350, 640, "GACOCT2", "UIGACHKURO", 0x1U, 0x0U},              // @text EN: "Kuro" | JA: "クロ"
    {2878, 4076, 350, 640, "GACOCAT", "UIGACHTORA", 0x1U, 0x0U},              // @text EN: "Tora" | JA: "トラ"
    {2869, 4077, 250, 500, "GACO1SH", "UIGACHBIGR", 0x1U, 0x0U},              // @text EN: "Big Robin" | JA: "ビッグウィリー"
    {2868, 4078, 250, 400, "GACOKSH", "UIGACHLITR", 0x1U, 0x0U},              // @text EN: "Little Robin" | JA: "リトルウィリー"
    {2870, 4079, 300, 600, "GACOOSH", "UIGACHROBI", 0x1U, 0x0U},              // @text EN: "Robins" | JA: "ウィリーズ"
    {2862, 4080, 280, 560, "GACO1IR", "UIGACHBIGP", 0x1U, 0x0U},              // @text EN: "Big Philip" | JA: "ビッグフィリップ"
    {2860, 4081, 280, 560, "GACOKIR", "UIGACHLITP", 0x1U, 0x0U},              // @text EN: "Little Philip" | JA: "リトルフィリップ"
    {2871, 4082, 280, 560, "GACOOIR", "UIGACHPHIL", 0x1U, 0x0U},              // @text EN: "Philips" | JA: "フィリップス"
    {2872, 4083, 280, 500, "GACO1HA", "UIGACHCHIP", 0x1U, 0x0U},              // @text EN: "Chip" | JA: "チップ"
    {2877, 4084, 280, 500, "GACOHA2", "UIGACHRAP0", 0x1U, 0x0U},              // @text EN: "Rap" | JA: "ラップ"
    {2876, 4085, 280, 500, "GACOHA3", "UIGACHPOP0", 0x1U, 0x0U},              // @text EN: "Pop" | JA: "ポップ"
    {2608, 4086, 280, 500, "GACOHA4", "UIGACHPIP0", 0x1U, 0x0U},              // @text EN: "Pip" | JA: "ピップ"
    {3080, 4087, 280, 500, "GACO1YG", "UIGACHMARY", 0x1U, 0x0U},              // @text EN: "Mary" | JA: "メリー"
    {3075, 4088, 280, 500, "GACO1US", "UIGACHPYON", 0x1U, 0x0U},              // @text EN: "Pyonta" | JA: "ピョンタ"
    {2613, 4089, 260, 480, "GACRMYU", "UIGACHMYAU", 0x1U, 0x0U},              // @text EN: "Myau" | JA: "ミャウ"
    {2598, 4090, 260, 500, "GACRCKN", "UIGACHCHIC", 0x1U, 0x0U},              // @text EN: "Chicken Leg" | JA: "チキンレッグ"
    {2843, 4091, 320, 600, "GACRALF", "UIGACHCHAO", 0x1U, 0x0U},              // @text EN: "Chao & Pian" | JA: "チャオ＆ピアン"
    {2615, 4092, 300, 550, "GACRRIS", "UIGACHRIST", 0x1U, 0x0U},              // @text EN: "Ristar" | JA: "リスター"
    {2842, 4093, 300, 550, "GACRARX", "UIGACHALEX", 0x1U, 0x0U},              // @text EN: "Alex Kidd" | JA: "アレックスキッド"
    {2597, 4094, 300, 550, "GACR0OP", "UIGACHOPAO", 0x1U, 0x0U},              // @text EN: "Opa-opa" | JA: "オパオパ"
    {2596, 4095, 300, 550, "GACR0O2", "UIGACHJOPA", 0x1U, 0x0U},              // @text EN: "Jet Opa-opa" | JA: "ジェットオパオパ"
    {2873, 4096, 300, 500, "GACRPOP", "UIGACHPOPP", 0x1U, 0x0U},              // @text EN: "Poppors" | JA: "ポッポーズ"
    {3088, 4097, 300, 500, "GACRCOB", "UIGACHCOBA", 0x1U, 0x0U},              // @text EN: "Goba Beach" | JA: "コバビーチ"
    {3082, 4098, 290, 550, "GACRIDA", "UIGACHAIDA", 0x1U, 0x0U},              // @text EN: "Aida II" | JA: "アイダ２"
    {2847, 4099, 250, 425, "GACRSHP", "UIGACHSHOP", 0x1U, 0x0U},              // @text EN: "Shop" | JA: "ショップ"
    {3104, 4100, 350, 600, "GACR016", "UIGACHHBO1", 0x1U, 0x0U},              // @text EN: "Heavy Bomb 1" | JA: "ヘビーボム１"
    {3105, 4101, 350, 600, "GACK66B", "UIGACHHBO2", 0x1U, 0x0U},              // @text EN: "Heavy Bomb 2" | JA: "ヘビーボム２"
    {3106, 4102, 350, 600, "GACK66R", "UIGACHHBO3", 0x1U, 0x0U},              // @text EN: "Heavy Bomb 3" | JA: "ヘビーボム３"
    {2845, 4103, 400, 600, "GACRMOB", "UIGACHMOBO", 0x1U, 0x0U},              // @text EN: "Mobo" | JA: "ＭＯＢＯ"
    {2846, 4104, 350, 600, "GACRROB", "UIGACHROBO", 0x1U, 0x0U},              // @text EN: "Robo" | JA: "ＲＯＢＯ"
    {2612, 4105, 240, 480, "GACRBON", "UIGACHBONA", 0x1U, 0x0U},              // @text EN: "Bonanza Brothers" | JA: "ボナンザブラザーズ"
    {2838, 4106, 340, 680, "GACH5BK", "UIGACHMOTO", 0x301U, 0x0U},            // @text EN: "Motor Scooter" | JA: "スクーター"
    {2839, 4107, 270, 530, "KAB02FS", "UIGACHDELI", 0x301U, 0x0U},            // @text EN: "Delivery Moped" | JA: "ビジネスバイク"
    {2829, 4108, 280, 500, "GACO1K2", "UIGACHCOU1", 0x301U, 0x0U},            // @text EN: "Coupe 1" | JA: "クーペ１"
    {2851, 4109, 280, 500, "GACO1K3", "UIGACHCOU2", 0x301U, 0x0U},            // @text EN: "Coupe 2" | JA: "クーぺ２"
    {3121, 4110, 280, 500, "GACO1K7", "UIGACHCOU3", 0x301U, 0x0U},            // @text EN: "Coupe 3" | JA: "クーペ３"
    {3122, 4111, 280, 500, "GACO1K8", "UIGACHCOU4", 0x301U, 0x0U},            // @text EN: "Coupe 4" | JA: "クーペ４"
    {3123, 4112, 280, 500, "GACO1K9", "UIGACHCOU5", 0x301U, 0x0U},            // @text EN: "Coupe 5" | JA: "クーペ５"
    {3113, 4113, 280, 500, "KABK6DY", "UIGACHHORN", 0x301U, 0x0U},            // @text EN: "Hornet" | JA: "ホーネット"
    {2830, 4114, 280, 500, "GACO1K1", "UIGACHWAG1", 0x301U, 0x0U},            // @text EN: "Wagon 1" | JA: "ワゴン１"
    {3115, 4115, 280, 500, "GACO1K4", "UIGACHWAG2", 0x301U, 0x0U},            // @text EN: "Wagon 2" | JA: "ワゴン２"
    {3114, 4116, 280, 500, "GACO1K5", "UIGACHWAG3", 0x301U, 0x0U},            // @text EN: "Wagon 3" | JA: "ワゴン３"
    {3120, 4117, 280, 500, "GACO1K6", "UIGACHWAG4", 0x301U, 0x0U},            // @text EN: "Wagon 4" | JA: "ワゴン４"
    {2836, 4118, 250, 500, "GACGTRX", "UIGACHTRU1", 0x301U, 0x0U},            // @text EN: "Truck 1" | JA: "トラック１"
    {2620, 4119, 300, 600, "GACO1T1", "UIGACHTRU2", 0x301U, 0x0U},            // @text EN: "Truck 2" | JA: "トラック２"
    {2850, 4120, 300, 600, "GACO1T2", "UIGACHTRU3", 0x301U, 0x0U},            // @text EN: "Truck 3" | JA: "トラック３"
    {2607, 4121, 300, 600, "GACH5TK", "UIGACHTRU4", 0x301U, 0x0U},            // @text EN: "Truck 4" | JA: "トラック４"
    {2831, 4122, 300, 600, "GACH5TR", "UIGACHHOTD", 0x301U, 0x0U},            // @text EN: "Hot Dog Truck" | JA: "フードカー"
    {2837, 4123, 350, 700, "GACS5BS", "UIGACHBUS0", 0x301U, 0x0U},            // @text EN: "Bus" | JA: "バス"
    {2864, 4124, 300, 560, "GACO1FO", "UIGACHFORK", 0x301U, 0x0U},            // @text EN: "Forklift" | JA: "フォークリフト"
    {3084, 4125, 340, 600, "GACS5N1", "UIGACHFOR1", 0x301U, 0x0U},            // @text EN: "Forklift No.1" | JA: "フォークＮｏ．１"
    {3085, 4126, 340, 600, "GACS5N2", "UIGACHFOR2", 0x301U, 0x0U},            // @text EN: "Forklift No.2" | JA: "フォークＮｏ．２"
    {3086, 4127, 340, 600, "GACS5N3", "UIGACHFOR3", 0x301U, 0x0U},            // @text EN: "Forklift No.3" | JA: "フォークＮｏ．３"
    {3087, 4128, 340, 600, "GACS5N4", "UIGACHFOR4", 0x301U, 0x0U},            // @text EN: "Forklift No.4" | JA: "フォークＮｏ．４"
    {3092, 4129, 340, 600, "GACS5N5", "UIGACHFOR5", 0x301U, 0x0U},            // @text EN: "Forklift No.5" | JA: "フォークＮｏ．５"
    {3093, 4130, 340, 600, "GACS5FR", "UIGACHFORR", 0x301U, 0x0U},            // @text EN: "Forklift Red" | JA: "フォークリフト赤"
    {3094, 4131, 340, 600, "GACS5FB", "UIGACHFORB", 0x301U, 0x0U},            // @text EN: "Forklift Blue" | JA: "フォークリフト青"
    {2867, 4132, 280, 500, "GACO1KU", "UIGACHCRAN", 0x1U, 0x0U},              // @text EN: "Crane" | JA: "クレーン"
    {2561, 4133, 280, 560, "GACGSHE", "UIGACHSHEN", 0x1U, 0x0U},              // @text EN: "Shenmue container" | JA: "シェンムーコンテナ"
    {3107, 4134, 280, 560, "GACO1KO", "UIGACHCONT", 0x1U, 0x0U},              // @text EN: "Container" | JA: "コンテナ"
    {2875, 4135, 280, 560, "GACO1KB", "UIGACHWOCR", 0x1U, 0x0U},              // @text EN: "Wooden Crate" | JA: "木箱"
    {2600, 4136, 290, 500, "GACH5IK", "UIGACHANCH", 0x1U, 0x0U},              // @text EN: "Anchor" | JA: "イカリ"
    {2857, 4137, 400, 700, "GACO1FE", "UIGACHFERR", 0x301U, 0x0U},            // @text EN: "Ferry" | JA: "フェリー"
    {2859, 4138, 330, 580, "GACO1KJ", "UIGACHSTEE", 0x1U, 0x0U},              // @text EN: "Steering Wheel" | JA: "舵輪"
    {2592, 4139, 330, 580, "GACH5UK", "UIGACHFLOA", 0x1U, 0x0U},              // @text EN: "Float" | JA: "うきわ"
    {2610, 4140, 400, 900, "GACGXBN", "UIGACHXBUT", 0x1U, 0x0U},              // @text EN: "X Button" | JA: "Ｘボタン"
    {2602, 4141, 400, 900, "GACGYBN", "UIGACHYBUT", 0x1U, 0x0U},              // @text EN: "Y Button" | JA: "Ｙボタン"
    {2616, 4142, 400, 900, "GACGABN", "UIGACHABUT", 0x1U, 0x0U},              // @text EN: "A Button" | JA: "Ａボタン"
    {2594, 4143, 400, 900, "GACGBBN", "UIGACHBBUT", 0x1U, 0x0U},              // @text EN: "B Button" | JA: "Ｂボタン"
    {3110, 4144, 230, 420, "GACGCRY", "UIGACHCHER", 0x101U, 0x0U},            // @text EN: "Cherry" | JA: "チェリー"
    {3095, 4145, 280, 500, "GACH5JK", "UIGACHMJUK", 0x1U, 0x0U},              // @text EN: "Mini Jukebox" | JA: "ミニジューク"
    {3103, 4146, 320, 550, "GACH5SL", "UIHACHMSLO", 0x1U, 0x0U},              // @text EN: "Mini Slot Game" | JA: "ミニスロット"
    {3101, 4147, 360, 600, "GACM400", "UIGACHMQTT", 0x1U, 0x0U},              // @text EN: "Mini QTE Title" | JA: "ミニタイトル"
    {3108, 4148, 250, 440, "GACGSPH", "UIGACHMHAR", 0x1U, 0x0U},              // @text EN: "Mini Harrier" | JA: "ミニハリアー"
    {2852, 4149, 280, 520, "GACS5HG", "UIGACHMHAO", 0x301U, 0x0U},            // @text EN: "Mini Hang-On" | JA: "ミニハングオン"
    {3109, 4150, 360, 600, "GACG360", "UIGACHR360", 0x301U, 0x0U},            // @text EN: "R-360" | JA: "Ｒ－３６０"
    {3102, 4151, 500, 730, "GACGDT1", "UIGACHMDAR", 0x1U, 0x0U},              // @text EN: "Mini Darts" | JA: "ミニダーツ"
    {2855, 4152, 360, 550, "GACS5EQ", "UIGACHMQTE", 0x1U, 0x0U},              // @text EN: "Mini QTE" | JA: "ミニＱＴＥ"
    {3100, 4153, 320, 580, "GACGBIR", "UIGACHMPOO", 0x1U, 0x0U},              // @text EN: "Mini Pool" | JA: "ミニビリヤード"
    {3124, 4154, 200, 350, "SYOGMD1", "UIGACHMATS", 0x1U, 0x0U},              // @text EN: "Matsuyama Prize" | JA: "松山認定書"
    {3125, 4155, 200, 350, "SYOGMD5", "UIGACHMITS", 0x1U, 0x0U},              // @text EN: "Mitsuzuka Prize" | JA: "三塚認定書"
    {3097, 4160, 200, 350, "SYOGSH0", "UIGACHHATO", 0x1U, 0x0U},              // @text EN: "Harrier Token" | JA: "ハリアー認定書"
    {3096, 4161, 200, 350, "SYOGHG0", "UIGACHHAOT", 0x1U, 0x0U},              // @text EN: "Hang-On Token" | JA: "ハングオン認定書"
    {2840, 4162, 300, 550, "SPBK6RE", "UIGACHBAL1", 0x1U, 0x0U},              // @text EN: "Ball 1" | JA: "ボール１"
    {2822, 4163, 300, 550, "SPBK6BR", "UIGACHBAL2", 0x1U, 0x0U},              // @text EN: "Ball 2" | JA: "ボール２"
    {2823, 4164, 300, 550, "SPBK6YE", "UIGACHBAL3", 0x1U, 0x0U},              // @text EN: "Ball 3" | JA: "ボール３"
    {2841, 4165, 350, 650, "SAIK6WH", "UIGACHDIC1", 0x301U, 0x0U},            // @text EN: "Dice 1" | JA: "サイコロ１"
    {2820, 4166, 350, 650, "SAIK6BK", "UIGACHDIC2", 0x301U, 0x0U},            // @text EN: "Dice 2" | JA: "サイコロ２"
    {2821, 4167, 350, 650, "SAIK6RE", "UIGACHDIC3", 0x301U, 0x0U},            // @text EN: "Dice 3" | JA: "サイコロ３"
    {3116, 4257, 600, 1000, "DENI2T1", "UIGACHASB0", 0x1U, 0x0U},             // @text EN: "A Size Battery" | JA: "単一電池"
    {3111, 4258, 600, 1000, "DENI2T3", "UIGACHAAAB", 0x1U, 0x0U},             // @text EN: "AAA Size Battery" | JA: "単三電池"
    {2844, 4259, 800, 1500, "MATI200", "UIGACHBOM0", 0x1U, 0x0U},             // @text EN: "Box of Matches" | JA: "マッチ"
    {2874, 4260, 400, 700, "RMP02H1", "UIGACHLIBU", 0x1U, 0x0U},              // @text EN: "Light Bulb" | JA: "電球"
    {3098, 4261, 400, 700, "HNGS200", "UIGACHHAON", 0x1U, 0x0U},              // @text EN: "Hang-On" | JA: "ハングオン"
    {3099, 4262, 400, 700, "SPHS200", "UIGACHSPHA", 0x1U, 0x0U},              // @text EN: "Space Harrier" | JA: "スペースハリアー"
    {3355, 4202, 340, 530, "1GAS2A1", "UIGACH2AK1", 0x401U, 0x0U},            // @text EN: "VF2 Akira 1" | JA: "結城　晶１Ｐ"
    {3365, 4212, 340, 530, "1GAS2A2", "UIGACH2AK2", 0x401U, 0x0U},            // @text EN: "VF2 Akira 2" | JA: "結城　晶２Ｐ"
    {3357, 4204, 340, 530, "1GAS2P1", "UIGACH2PA1", 0x1U, 0x0U},              // @text EN: "VF2 Pai 1" | JA: "パイ・チェン１Ｐ"
    {3367, 4214, 340, 530, "1GAS2P2", "UIGACH2PA2", 0x401U, 0x0U},            // @text EN: "VF2 Pai 2" | JA: "パイ・チェン２Ｐ"
    {3356, 4203, 340, 530, "1GASLU1", "UIGACH2LA1", 0x1U, 0x0U},              // @text EN: "VF2 Lau 1" | JA: "ラウ・チェン１Ｐ"
    {3366, 4213, 340, 530, "1GASLU2", "UIGACH2LA2", 0x1U, 0x0U},              // @text EN: "VF2 Lau 2" | JA: "ラウ・チェン２Ｐ"
    {3358, 4205, 300, 530, "1GASWF1", "UIGACH2WO1", 0x1U, 0x0U},              // @text EN: "VF2 Wolf 1" | JA: "ウルフ１Ｐ"
    {3368, 4215, 340, 530, "1GASWF2", "UIGACH2WO2", 0x1U, 0x0U},              // @text EN: "VF2 Wolf 2" | JA: "ウルフ２Ｐ"
    {3359, 4206, 300, 530, "1GASJF1", "UIGACH2JE1", 0x1U, 0x0U},              // @text EN: "VF2 Jeffry 1" | JA: "ジェフリー１Ｐ"
    {3369, 4216, 340, 530, "1GASJF2", "UIGACH2JE2", 0x401U, 0x0U},            // @text EN: "VF2 Jeffry 2" | JA: "ジェフリー２Ｐ"
    {3362, 4209, 340, 530, "1GASKG1", "UIGACH2KA1", 0x1U, 0x0U},              // @text EN: "VF2 Kage 1" | JA: "影丸１Ｐ"
    {3372, 4219, 340, 530, "1GASKG2", "UIGACH2KA2", 0x401U, 0x0U},            // @text EN: "VF2 Kage 2" | JA: "影丸２Ｐ"
    {3360, 4207, 340, 530, "1GASSR1", "UIGACH2SA1", 0x1U, 0x0U},              // @text EN: "VF2 Sarah 1" | JA: "サラ１Ｐ"
    {3370, 4217, 340, 530, "1GASSR2", "UIGACH2SA2", 0x401U, 0x0U},            // @text EN: "VF2 Sarah 2" | JA: "サラ２Ｐ"
    {3361, 4208, 340, 530, "1GAS2J1", "UIGACH2JA1", 0x1U, 0x0U},              // @text EN: "VF2 Jacky 1" | JA: "ジャッキー１Ｐ"
    {3371, 4218, 340, 530, "1GAS2J2", "UIGACH2JA2", 0x1U, 0x0U},              // @text EN: "VF2 Jacky 2" | JA: "ジャッキー２Ｐ"
    {3363, 4210, 340, 530, "1GASSN1", "UIGACH2SD1", 0x1U, 0x0U},              // @text EN: "VF2 Shun Di 1" | JA: "瞬帝１Ｐ"
    {3373, 4220, 410, 700, "1GASSN2", "UIGACH2SD2", 0x401U, 0x0U},            // @text EN: "VF2 Shun Di 2" | JA: "瞬帝２Ｐ"
    {3364, 4211, 340, 530, "1GASLN1", "UIGACH2LI1", 0x401U, 0x0U},            // @text EN: "VF2 Lion 1" | JA: "リオン１Ｐ"
    {3374, 4221, 340, 530, "1GASLN2", "UIGACH2LI2", 0x1U, 0x0U},              // @text EN: "VF2 Lion 2" | JA: "リオン２Ｐ"
    {3599, 4256, 320, 520, "GACRDUC", "UIGACHKDC0", 0x1U, 0x0U},              // @text EN: "Kids Dural C" | JA: "キッズデュラルＣ"
    {3333, 4180, 330, 600, "7GATOUT", "UIGACHOUTR", 0x301U, 0x0U},            // @text EN: "Outrun Toy" | JA: "アウトラン"
    {3334, 4181, 300, 600, "7GAHF14", "UIGACHF14X", 0x301U, 0x0U},            // @text EN: "F14-XX" | JA: "Ｆ１４－ＸＸ"
    {3335, 4182, 440, 740, "4GA1TR1", "UIGACHASCO", 0x301U, 0x0U},            // @text EN: "Asphalt Cowboy" | JA: "１８ホイラー１"
    {3336, 4183, 440, 740, "4GA9TR2", "UIGACHHICA", 0x301U, 0x0U},            // @text EN: "Highway Cat" | JA: "１８ホイラー２"
    {3337, 4184, 440, 740, "4GA1TR3", "UIGACHLOHO", 0x301U, 0x0U},            // @text EN: "Long Horn" | JA: "１８ホイラー３"
    {3338, 4185, 440, 740, "4GA1TR4", "UIGACHSTLI", 0x301U, 0x0U},            // @text EN: "Stream Line" | JA: "１８ホイラー４"
    {3339, 4186, 440, 740, "4GA1TR5", "UIGACHNIPP", 0x301U, 0x0U},            // @text EN: "Nihonmaru" | JA: "１８ホイラー５"
    {3340, 4187, 350, 570, "4GA1CH1", "UIGACHTEHA", 0x1U, 0x0U},              // @text EN: "Texas Hawk" | JA: "テキサスホーク"
    {3341, 4188, 350, 570, "4GA1CH2", "UIGACHWIRO", 0x1U, 0x0U},              // @text EN: "Wild Rose" | JA: "ワイルドローズ"
    {3342, 4189, 350, 570, "4GA1CH3", "UIGACHMABU", 0x1U, 0x0U},              // @text EN: "Mad Bull" | JA: "マッドブル"
    {3343, 4190, 350, 570, "4GA1CH4", "UIGACHMOLI", 0x1U, 0x0U},              // @text EN: "Moon Light" | JA: "ムーンライト"
    {3344, 4191, 300, 500, "5GARJAY", "UIGACHJAY0", 0x1U, 0x0U},              // @text EN: "Jay" | JA: "ジェイ"
    {3345, 4192, 300, 500, "5GASALN", "UIGACHALAI", 0x1U, 0x0U},              // @text EN: "Alain" | JA: "アラン"
    {3346, 4193, 300, 500, "5GASLNA", "UIGACHLINA", 0x1U, 0x0U},              // @text EN: "Lina" | JA: "リナ"
    {3347, 4194, 300, 500, "5GASTRN", "UIGACHTALO", 0x1U, 0x0U},              // @text EN: "Talon" | JA: "タロン"
    {3348, 4195, 300, 500, "5GASTRS", "UIGACHTERE", 0x1U, 0x0U},              // @text EN: "Tereshkova" | JA: "テレシコワ"
    {3349, 4196, 300, 550, "3GA1JS2", "UIGACHKATE", 0x301U, 0x0U},            // @text EN: "Kate" | JA: "ケイト"
    {3350, 4197, 300, 550, "3GA1JS4", "UIGACHMAT0", 0x301U, 0x0U},            // @text EN: "Mat" | JA: "マット"
    {3351, 4198, 300, 500, "3GA1JS5", "UIGACHMAX0", 0x301U, 0x0U},            // @text EN: "Max" | JA: "マックス"
    {3352, 4199, 300, 500, "3GA1JS6", "UIGACHRYAN", 0x301U, 0x0U},            // @text EN: "Ryan" | JA: "ライアン"
    {3353, 4200, 300, 500, "3GA1JS3", "UIGACHSPIK", 0x301U, 0x0U},            // @text EN: "Spike" | JA: "スパイク"
    {3354, 4201, 300, 500, "3GA1JS1", "UIGACHYUKI", 0x301U, 0x0U},            // @text EN: "Yuki" | JA: "ユキ"
    {3377, 4224, 300, 500, "2GARTEM", "UIGACHTEMJ", 0x101U, 0x0U},            // @text EN: "Temjin" | JA: "テムジン"
    {3379, 4226, 300, 500, "2GAGTM1", "UIGACHTMPR", 0x101U, 0x0U},            // @text EN: "Temjin Prototype" | JA: "テムジン試作型"
    {3378, 4225, 250, 450, "2GAETE1", "UIGACH1080", 0x101U, 0x0U},            // @text EN: "10/80" | JA: "１０／８０"
    {3385, 4232, 280, 480, "2GAFGR1", "UIGACHGRYS", 0x101U, 0x0U},            // @text EN: "Grys-vok" | JA: "グリスボック"
    {3386, 4233, 280, 480, "2GAGST1", "UIGACHSTEI", 0x101U, 0x0U},            // @text EN: "Stein-vok" | JA: "シュタインボック"
    {3388, 4235, 300, 500, "2GAEBO1", "UIGACHBABD", 0x1U, 0x0U},              // @text EN: "Bal-Bados" | JA: "バル・バドス"
    {3389, 4236, 300, 500, "2GACBA1", "UIGACHBAKE", 0x1U, 0x0U},              // @text EN: "Bal-Keros" | JA: "バル・ケロス"
    {3390, 4237, 300, 500, "2GABBR1", "UIGACHBABR", 0x1U, 0x0U},              // @text EN: "Bal-Baros" | JA: "バル・バロス"
    {3391, 4238, 350, 550, "2GACFE1", "UIGACHFYDN", 0x101U, 0x0U},            // @text EN: "Fei-Yen DNA" | JA: "フェイ・イェンＤ"
    {3587, 4242, 350, 550, "2GAEAM1", "UIGACHANGE", 0x101U, 0x0U},            // @text EN: "Angelan" | JA: "エンジェラン"
    {3380, 4227, 300, 500, "2GAGAS1", "UIGACHAPHS", 0x101U, 0x0U},            // @text EN: "Apharmd S" | JA: "アファームド・Ｓ"
    {3381, 4228, 300, 500, "2GADAB1", "UIGACHAPHB", 0x101U, 0x0U},            // @text EN: "Apharmd B" | JA: "アファームド・Ｂ"
    {3382, 4229, 300, 500, "2GAFAC1", "UIGACHAPHC", 0x1U, 0x0U},              // @text EN: "Apharmd C" | JA: "アファームド・Ｃ"
    {3383, 4230, 280, 480, "2GABCY1", "UIGACHCYTH", 0x101U, 0x0U},            // @text EN: "Cypher" | JA: "サイファー"
    {3387, 4234, 300, 500, "2GADDO1", "UIGACHDORD", 0x101U, 0x0U},            // @text EN: "Dordray" | JA: "ドルドレイ"
    {3586, 4241, 250, 400, "2GADSP1", "UIGACHSPEC", 0x101U, 0x0U},            // @text EN: "Specianeff" | JA: "スペシネフ"
    {3384, 4231, 280, 480, "2GABRA1", "UIGACHRAID", 0x101U, 0x0U},            // @text EN: "Raiden" | JA: "ライデン"
    {3584, 4239, 350, 550, "2GACFE2", "UIGACHFYRN", 0x101U, 0x0U},            // @text EN: "Fei-Yen Rna" | JA: "フェイ・イェンｒ"
    {3585, 4240, 350, 550, "2GACFE3", "UIGACHRYHY", 0x101U, 0x0U},            // @text EN: "Fei-Yen Hyper" | JA: "フェイ・ハイパー"
    {3588, 4243, 350, 600, "2GACAJ1", "UIGACHAJIM", 0x1U, 0x0U},              // @text EN: "Ajim" | JA: "アジム"
    {3376, 4223, 250, 450, "2GAFBT1", "UIGACHBRAD", 0x1U, 0x0U},              // @text EN: "Bradtos" | JA: "ブラットス"
    {3375, 4222, 200, 450, "2GARTAN", "UIGACHTANG", 0x1U, 0x0U},              // @text EN: "Tangram" | JA: "タングラム"
    {3129, 4168, 250, 400, "6GA1KEJ", "UIGACHFLTR", 0x1U, 0x0U},              // @text EN: "Flame Tree" | JA: "火炎樹"
    {3130, 4169, 320, 500, "6GA5000", "UIGACHLIHS", 0x1U, 0x0U},              // @text EN: "Lighthouse" | JA: "灯台"
    {3131, 4170, 340, 600, "6GA5001", "UIGACHBOAT", 0x301U, 0x0U},            // @text EN: "Boat" | JA: "水上生活船"
    {3132, 4171, 310, 550, "6GA5003", "UIGACHCHVI", 0x201U, 0x0U},            // @text EN: "Chinese Violin" | JA: "胡弓"
    {3133, 4172, 330, 550, "6GA5002", "UIGACHKOTO", 0x201U, 0x0U},            // @text EN: "Koto" | JA: "琴"
    {3134, 4173, 300, 600, "6GAAJET", "UIGACHLRJ0", 0x301U, 0x0U},            // @text EN: "Long Range Jet" | JA: "ジャンボジェット機"
    {3135, 4174, 650, 1100, "6GATCHN", "UIGACHCHSW", 0x301U, 0x0U},           // @text EN: "Chainsaw" | JA: "チェンソー"
    {3328, 4175, 360, 600, "6GAAKEN", "UIGACHCHBR", 0x201U, 0x0U},            // @text EN: "Chinese Broadsword" | JA: "青竜刀"
    {3329, 4176, 260, 450, "6GAAMZ1", "UIGACHSNTO", 0x1U, 0x0U},              // @text EN: "Snake Tortoise" | JA: "玄武水場"
    {3330, 4177, 260, 450, "6GAAMZ3", "UIGACHWHTI", 0x1U, 0x0U},              // @text EN: "White Tiger" | JA: "白虎水場"
    {3331, 4178, 260, 450, "6GAAMZ4", "UIGACHPHOE", 0x1U, 0x0U},              // @text EN: "Phoenix" | JA: "朱雀水場"
    {3332, 4179, 260, 450, "6GAAMZ2", "UIGACHBLDR", 0x1U, 0x0U},              // @text EN: "Blue Dragon" | JA: "青龍水場"
    {3589, 4244, 220, 350, "NINT2AF", "UIGACHAFTO", 0x21U, 0x0U},             // @text EN: "After Burner II Token" | JA: "ＡＢ２認定書"
    {3590, 4245, 220, 350, "NINT2OR", "UIGACHOUTO", 0x21U, 0x0U},             // @text EN: "OutRun Token" | JA: "アウトラン認定書"
    {3591, 4246, 220, 350, "NINT2SP", "UIGACHHAT2", 0x21U, 0x0U},             // @text EN: "Space Harrier Token" | JA: "ハリアー認定書２"
    {3592, 4247, 220, 350, "NINT2HG", "UIGACHHOT2", 0x21U, 0x0U},             // @text EN: "Hang-On Token" | JA: "ハングオン認定書２"
    {3593, 4248, 220, 350, "003K2SL", "UIGACHPR31", 0x21U, 0x0U},             // @text EN: "3,000 Prize" | JA: "３千枚認定書"
    {3594, 4249, 220, 350, "010K2SL", "UIGACHPR11", 0x21U, 0x0U},             // @text EN: "10,000 Prize" | JA: "１万枚認定書"
    {3595, 4250, 220, 350, "050K2SL", "UIGACHPR51", 0x21U, 0x0U},             // @text EN: "50,000 Prize" | JA: "５万枚認定書"
    {3596, 4251, 220, 350, "200K2SL", "UIGACHPR21", 0x21U, 0x0U},             // @text EN: "200,000 Prize" | JA: "２０万枚認定書"
    {3597, 4252, 220, 350, "500K2SL", "UIGACHPR52", 0x21U, 0x0U},             // @text EN: "500,000 Prize" | JA: "５０万枚認定書"
    {3598, 4253, 220, 350, "001M2SL", "UIGACHPR12", 0x21U, 0x0U},             // @text EN: "1,000,000 Prize" | JA: "１００万枚認定書"
    {3127, 4254, 400, 700, "AFBR701", "UIGACHMIAB", 0x301U, 0x0U},            // @text EN: "Mini After Burner" | JA: "ミニＡＢ２"
    {3126, 4255, 450, 850, "OUTR701", "UIGACHMIOU", 0x301U, 0x0U},            // @text EN: "Mini OutRun" | JA: "ミニアウトラン"
    {2865, 4263, 1700, 2700, "HOSK2GR", "UIGACHGRST", 0x201U, 0x0U},          // @text EN: "Green Stone" | JA: "緑石"
    {2858, 4264, 1700, 2700, "HOSK2RE", "UIGACHREST", 0x201U, 0x0U},          // @text EN: "Red Stone" | JA: "赤石"
    {2816, 4265, 1700, 2700, "HOSK2YE", "UIGACHYEST", 0x201U, 0x0U},          // @text EN: "Yellow Stone" | JA: "黄石"
    {2853, 4266, 1700, 2700, "HOSK2PP", "UIGACHPUST", 0x201U, 0x0U},          // @text EN: "Purple Stone" | JA: "紫石"
    {2854, 4267, 1700, 2700, "HOSK2BL", "UIGACHBLST", 0x201U, 0x0U},          // @text EN: "Blue Stone" | JA: "青石"
    {2863, 4268, 1700, 2700, "HOSK2CL", "UIGACHTRST", 0x201U, 0x0U},          // @text EN: "Transparent Stone" | JA: "透石"
    {2866, 4269, 800, 1600, "ZIPOHW3", "UIGACHMOWO", 0x201U, 0x0U},           // @text EN: "Moon Wolf" | JA: "ムーンウルフ"
    {2866, 4270, 800, 1600, "ZIPOHW2", "UIGACHVAMP", 0x201U, 0x0U},           // @text EN: "Vampire" | JA: "ヴァンパイア"
    {2866, 4271, 800, 1600, "ZIPOHG2", "UIGACHWING", 0x201U, 0x0U},           // @text EN: "Wing" | JA: "ウイング"
    {2866, 4272, 800, 1600, "ZIPOHW1", "UIGACHJACK", 0x201U, 0x0U},           // @text EN: "Jackal" | JA: "ジャッカル"
    {2866, 4273, 800, 1600, "ZIPOHM1", "UIGACHMUEC", 0x201U, 0x0U},           // @text EN: "Muecas" | JA: "ムーキャス"
    {2866, 4274, 800, 1600, "ZIPOHS1", "UIGACHSCOR", 0x201U, 0x0U},           // @text EN: "Scorpion" | JA: "サソリ"
    {2866, 4275, 800, 1600, "ZIPOHG1", "UIGACHKING", 0x201U, 0x0U},           // @text EN: "King" | JA: "キング"
    {2866, 4276, 800, 1600, "ZIPOHD1", "UIGACHDRAG", 0x201U, 0x0U},           // @text EN: "Dragon" | JA: "ドラゴン"
    {2866, 4277, 800, 1600, "ZIPOHK1", "UIGACHHELM", 0x201U, 0x0U},           // @text EN: "Helm" | JA: "ヘルム"
    {2866, 4278, 800, 1600, "ZIPOHR1", "UIGACHROSE", 0x201U, 0x0U},           // @text EN: "Rose" | JA: "バラ"
    {2866, 4279, 800, 1600, "ZIPOHH1", "UIGACHHEAR", 0x201U, 0x0U},           // @text EN: "Heart" | JA: "ハート"
    {2866, 4280, 800, 1600, "ZIPOHG3", "UIGACHNOSM", 0x201U, 0x0U},           // @text EN: "No Smoking" | JA: "禁煙"
};

constexpr InventoryItemDefinition photos[] = {
    {30, 8006, 460, 1000, "SHAS503", "UIPHODNOZO", 0x0U, 0x0U}, // @text EN: "Nozomi" | JA: "原崎"
    {30, 8007, 460, 1000, "SHAS506", "UIPHODNOZO", 0x0U, 0x0U}, // @text EN: "Nozomi" | JA: "原崎"
    {30, 8008, 460, 1000, "SHAS504", "UIPHODNOZO", 0x0U, 0x0U}, // @text EN: "Nozomi" | JA: "原崎"
    {30, 8009, 460, 1000, "SHAS505", "UIPHODNOZO", 0x0U, 0x0U}, // @text EN: "Nozomi" | JA: "原崎"
    {30, 8010, 460, 1000, "SHAS507", "UIPHODNOZO", 0x0U, 0x0U}, // @text EN: "Nozomi" | JA: "原崎"
    {30, 8011, 460, 1000, "SHAS508", "UIPHODNOZO", 0x0U, 0x0U}, // @text EN: "Nozomi" | JA: "原崎"
    {31, 8005, 420, 1000, "SHAS1S1", "UIPHODFATH", 0x0U, 0x0U}, // @text EN: "Father" | JA: "親父"
    {32, 8013, 420, 1000, "SHAS1IR", "UIPHODHAZU", 0x0U, 0x0U}, // @text EN: "Hazukis" | JA: "芭月家"
    {33, 8014, 420, 1000, "SHAS1RS", "UIPHODFRIE", 0x0U, 0x0U}, // @text EN: "Friends" | JA: "友人"
    {34, 8018, 420, 1000, "PHFS2WO", "UIPHODWONG", 0x0U, 0x0U}, // @text EN: "Wong" | JA: "ウォン"
    {35, 8012, 420, 1000, "PHFS2BU", "UIPHODXIUY", 0x0U, 0x0U}, // @text EN: "Xiuying" | JA: "秀瑛たち"
    {41, 8000, 400, 800, "PCHN200", "UIPHODSCOU", 0x0U, 0x0U}, // @text EN: "Scout" | JA: "スカウトマン"
    {36, 8015, 460, 1000, "PHFS2JR", "UIPHODJOY0", 0x0U, 0x0U}, // @text EN: "Joy" | JA: "ジョイ"
    {37, 8016, 460, 1000, "PHFS2K2", "UIPHODFANG", 0x0U, 0x0U}, // @text EN: "Fangmei" | JA: "薫"
    {37, 8017, 460, 1000, "PHFS2K1", "UIPHODFANG", 0x0U, 0x0U}, // @text EN: "Fangmei" | JA: "薫"
    {39, 8002, 420, 1000, "PSF72AW", "UIPHODROD0", 0x0U, 0x0U}, // @text EN: "Rod" | JA: "ロッド"
    {38, 8001, 420, 1000, "PSF6200", "UIPHODGREG", 0x0U, 0x0U}, // @text EN: "Greg" | JA: "グレッグ"
    {40, 8003, 420, 1000, "PSF3200", "UIPHODCHUN", 0x0U, 0x0U}, // @text EN: "Chunyan" | JA: "春燕"
};

constexpr InventoryItemDefinition mapItems[] = {
    {11, 6015, 200, 550, nullptr, "UIMAPDFOPI", 0x0U, 0x0U}, // @text EN: "Fortune's Pier" | JA: "開運碼頭"
    {11, 6012, 200, 550, nullptr, "UIMAPDWOPI", 0x0U, 0x0U}, // @text EN: "Worker's Pier" | JA: "労湾碼頭"
    {11, 6013, 200, 550, nullptr, "UIMAPDQUST", 0x0U, 0x0U}, // @text EN: "Queen's St." | JA: "クイーンズストリート"
    {11, 6014, 200, 550, nullptr, "UIMAPDBHW0", 0x0U, 0x0U}, // @text EN: "Beverly Hills Wharf" | JA: "ビバリーヒルズワーフ"
    {11, 6011, 300, 800, nullptr, "UIMAPDGMQR", 0x0U, 0x0U}, // @text EN: "Green Market Qr." | JA: "菜珍街"
    {11, 6006, 300, 800, nullptr, "UIMAPDGOQR", 0x0U, 0x0U}, // @text EN: "Golden Qr." | JA: "詠黄街"
    {11, 6010, 300, 800, nullptr, "UIMAPDLCQR", 0x0U, 0x0U}, // @text EN: "Lucky Charm Qr." | JA: "福仙街"
    {11, 6007, 300, 800, nullptr, "UIMAPDWHQR", 0x0U, 0x0U}, // @text EN: "White Dynasty Qr." | JA: "白秦街"
    {11, 6009, 300, 800, nullptr, "UIMAPDWMQR", 0x0U, 0x0U}, // @text EN: "Wise Men's Qr." | JA: "太老街"
    {11, 6008, 300, 800, nullptr, "UIMAPDSCQR", 0x0U, 0x0U}, // @text EN: "South Carmain Qr." | JA: "紅南街"
    {11, 6027, 300, 800, nullptr, "UIMAPDSCHI", 0x0U, 0x0U}, // @text EN: "Scarlet Hills" | JA: "紅華台"
    {11, 6019, 300, 800, nullptr, "UIMAPDDRST", 0x0U, 0x0U}, // @text EN: "Dragon St." | JA: "龍影街"
    {11, 6020, 300, 800, nullptr, "UIMAPDTWQR", 0x0U, 0x0U}, // @text EN: "Thousand White Qr." | JA: "白千街"
    {11, 6021, 300, 800, nullptr, "UIMAPDDIQR", 0x0U, 0x0U}, // @text EN: "Dimsum Qr." | JA: "点心街"
    {11, 6022, 300, 800, nullptr, "UIMAPDSTQR", 0x0U, 0x0U}, // @text EN: "Stand Qr." | JA: "屋台街"
    {11, 6000, 300, 800, nullptr, "UIGNRLMISS", 0x0U, 0x0U}, // @text EN: "Missing" | JA: "ない"
    {11, 6001, 300, 800, nullptr, "UIMAPDABLD", 0x0U, 0x0U}, // @text EN: "A Building" | JA: "九龍城Ａビル"
    {11, 6002, 300, 800, nullptr, "UIMAPDBBLD", 0x0U, 0x0U}, // @text EN: "B Building" | JA: "九龍城Ｂビル"
    {11, 6003, 300, 800, nullptr, "UIMAPDCBLD", 0x0U, 0x0U}, // @text EN: "C Building" | JA: "九龍城Ｃビル"
    {11, 6004, 300, 800, nullptr, "UIMAPDDBLD", 0x0U, 0x0U}, // @text EN: "D Building" | JA: "九龍城Ｄビル"
    {11, 6005, 300, 800, nullptr, "UIMAPDEBLD", 0x0U, 0x0U}, // @text EN: "E Building" | JA: "九龍城Ｅビル"
    {11, 6023, 300, 800, nullptr, "UIMAPDFBLD", 0x0U, 0x0U}, // @text EN: "F Building" | JA: "九龍城Ｆビル"
    {11, 6025, 300, 560, "ABR02MP", "UIMAPDREN0", 0x100U, 0x0U}, // @text EN: "Ren" | JA: "レン"
    {11, 6026, 400, 820, "WMEM200", "UIMAPDWONG", 0x100U, 0x0U}, // @text EN: "Wong" | JA: "ウォン"
    {11, 6018, 350, 600, "WT001M1", "UIMAPDDYA0", 0x100U, 0x0U}, // @text EN: "Da Yuan Apartments" | JA: "大元楼"
    {11, 6017, 300, 540, "HKSM201", "UIMAPDSCOU", 0x100U, 0x0U}, // @text EN: "Scout" | JA: "スカウトマン"
};

constexpr InventoryItemDefinition moveItems[] = {
    {266, 3000, 160, 322, "WZIS1SJ", "UISKILTWBL", 0x0U, 0x0U}, // @text EN: "Twin Blades" | JA: "双刃の技書"
    {266, 3001, 160, 320, "MKIS101", "UISKILSHRE", 0x0U, 0x0U}, // @text EN: "Shadow Reaper" | JA: "影狩りの技書"
    {266, 3002, 160, 320, "MKIS103", "UISKILSTAR", 0x0U, 0x0U}, // @text EN: "Stab Armor" | JA: "鎧通の技書"
    {266, 3003, 160, 320, "WZIS1HE", "UISKILTSL0", 0x0U, 0x0U}, // @text EN: "Twin Swallow Leap" | JA: "飛燕連刈の技書"
    {266, 3004, 160, 320, "WZIS1TG", "UISKILMUSP", 0x0U, 0x0U}, // @text EN: "Mud Spider" | JA: "土蜘蛛の技書"
    {266, 3005, 160, 320, "WZIS1SK", "UISKILRIFL", 0x0U, 0x0U}, // @text EN: "Rising Flash" | JA: "裂晃閃の技書"
    {266, 3006, 160, 320, "WZIS1CS", "UISKILCRCY", 0x0U, 0x0U}, // @text EN: "Crawl Cyclone" | JA: "旋風の技書"
    {266, 3007, 160, 320, "WZIS1HZ", "UISKILTIST", 0x0U, 0x0U}, // @text EN: "Tiger Storm" | JA: "虎身崩しの技書"
    {266, 3008, 160, 320, "WZIS1OB", "UISKILABF0", 0x0U, 0x0U}, // @text EN: "Arm Break Fire" | JA: "朧無双の技書"
    {266, 3009, 200, 330, "MKIS104", "UISKILPOSC", 0x140U, 0x0U}, // @text EN: "Poetry Scroll" | JA: "謎の巻物"
    {266, 3010, 200, 330, "MKIS105", "UISKILMYSC", 0x140U, 0x0U}, // @text EN: "Mysterious Scroll" | JA: "歌訣の巻物"
    {266, 3011, 160, 320, "WZIS2KR", "UISKILDRSP", 0x0U, 0x0U}, // @text EN: "Dragon Spin" | JA: "降龍脚の技書"
    {266, 3012, 160, 320, "WZIS2JS", "UISKILSECO", 0x0U, 0x0U}, // @text EN: "Serpent Coil" | JA: "蛇身貼の技書"
    {266, 3013, 160, 320, "WZIS2DH", "UISKILRFS0", 0x0U, 0x0U}, // @text EN: "Rear Foot Sweep" | JA: "大展伯の技書"
    {266, 3014, 160, 320, "WZIS2SK", "UISKILDIWI", 0x0U, 0x0U}, // @text EN: "Diagonal Wipe" | JA: "斜弧捶の技書"
    {266, 3015, 160, 320, "WZIS2SH", "UISKILOBSL", 0x0U, 0x0U}, // @text EN: "Oblique Slam" | JA: "斜歩劈斬の技書"
    {266, 3016, 160, 320, "WZIS2SS", "UISKILTPT0", 0x0U, 0x0U}, // @text EN: "Twin Palm Thrust" | JA: "双掌打の技書"
    {266, 3017, 160, 320, "WZIS2BT", "UISKILHOKI", 0x0U, 0x0U}, // @text EN: "Horseshoe Kick" | JA: "馬蹄脚の技書"
    {266, 3018, 160, 320, "WZIS2RH", "UISKILMACO", 0x0U, 0x0U}, // @text EN: "Mantis Combo" | JA: "連環劈の技書"
    {266, 3019, 160, 320, "WZIS2JH", "UISKILSTST", 0x0U, 0x0U}, // @text EN: "Stepping Strike" | JA: "順歩捶の技書"
    {266, 3020, 160, 320, "WZIS2SN", "UISKILOAK0", 0x0U, 0x0U}, // @text EN: "Oblique Air Kick" | JA: "旋空脚の技書"
    {266, 3021, 160, 320, "WZIS2FS", "UISKILHESW", 0x0U, 0x0U}, // @text EN: "Heel Sweep" | JA: "伏身脚の技書"
    {266, 3022, 160, 320, "WZIS2HE", "UISKILMRD0", 0x0U, 0x0U}, // @text EN: "Monkey Roll Drop" | JA: "白猿落の技書"
    {266, 3023, 160, 320, "WZIS2RS", "UISKILMAFI", 0x0U, 0x0U}, // @text EN: "Machinegun Fist" | JA: "連手砲の技書"
    {266, 3024, 160, 320, "WZIS2RT", "UISKILDSK0", 0x0U, 0x0U}, // @text EN: "Double Storm Kick" | JA: "連環腿の技書"
    {266, 3025, 200, 330, "WZKM2ES", "UISKILDETR", 0x140U, 0x0U}, // @text EN: "Demon's Triangle" | JA: "閻王三点手の巻物"
};

constexpr InventoryItemDefinition pawnItems[] = {
    {29, 9000, 300, 800, nullptr, "UIPAWNHONE", 0x0U, 0x0U}, // @text EN: "Honest" | JA: "良心当舗のパンフ"
    {29, 9001, 300, 800, nullptr, "UIPAWNGMP0", 0x0U, 0x0U}, // @text EN: "G.M.P" | JA: "菜珍当舗のパンフ"
    {29, 9002, 300, 800, nullptr, "UIPAWNCARM", 0x0U, 0x0U}, // @text EN: "Carmain" | JA: "紅南当舗のパンフ"
    {29, 9003, 300, 800, nullptr, "UIPAWNSHIN", 0x0U, 0x0U}, // @text EN: "Shine" | JA: "光来当舗のパンフ"
    {29, 9004, 300, 800, nullptr, "UIPAWNCENT", 0x0U, 0x0U}, // @text EN: "Centre" | JA: "中心当舗のパンフ"
    {29, 9005, 300, 800, nullptr, "UIPAWNACSE", 0x0U, 0x0U}, // @text EN: "Accounts Settled" | JA: "明朗当舗のパンフ"
    {29, 9006, 300, 800, nullptr, "UIPAWNLUCK", 0x0U, 0x0U}, // @text EN: "Lucky" | JA: "招福当舗のパンフ"
    {29, 9007, 300, 800, nullptr, "UIPAWNGOFL", 0x0U, 0x0U}, // @text EN: "Golden Flower" | JA: "金華当舗のパンフ"
    {29, 9008, 300, 800, nullptr, "UIPAWNWIME", 0x0U, 0x0U}, // @text EN: "Wise Men's" | JA: "太老当舗のパンフ"
    {29, 9009, 300, 800, nullptr, "UIPAWNWHDY", 0x0U, 0x0U}, // @text EN: "White Dynasty" | JA: "白秦当舗のパンフ"
    {29, 9010, 300, 800, nullptr, "UIPAWNKOWL", 0x0U, 0x0U}, // @text EN: "Kowloon" | JA: "九龍当舗のパンフ"
    {29, 9011, 300, 800, nullptr, "UIPAWNGRVI", 0x0U, 0x0U}, // @text EN: "Great View" | JA: "安見当舗のパンフ"
};

constexpr std::array<std::span<const InventoryItemDefinition>, 6> itemBanks{keyItems, capsuleToys, photos, mapItems, moveItems, pawnItems};

constexpr std::array<std::uint8_t, 10> itemTypeByThousands{
    0, 0, 6, 4, 1, 6, 3, 6, 2, 5};

const InventoryItemDefinition *FindInventorySlot(std::uint32_t itemId, S2InventoryBank &bank, std::size_t &index) noexcept {
    const std::uint32_t group = itemId / 1000;
    if (group >= itemTypeByThousands.size())
        return nullptr;
    const std::uint8_t type = itemTypeByThousands[group];
    if (type >= itemBanks.size())
        return nullptr;
    const std::span<const InventoryItemDefinition> definitions = itemBanks[type];
    for (index = 0; index < definitions.size(); ++index) {
        if (definitions[index].id != itemId)
            continue;
        bank = static_cast<S2InventoryBank>(type);
        return &definitions[index];
    }
    return nullptr;
}

bool IsHiddenStockIndex(std::size_t index) noexcept {
    // IsHiddenStockIndex suppresses these bank-relative entries, not item IDs.
    return (index >= 162 && index <= 167) || (index >= 153 && index <= 156);
}

} // namespace

std::span<const InventoryItemDefinition> GetInventoryItemDefinitions(S2InventoryBank bank) noexcept {
    const auto type = static_cast<std::size_t>(bank);
    return type < itemBanks.size() ? itemBanks[type] : std::span<const InventoryItemDefinition>{};
}

const InventoryItemDefinition *FindInventoryItemDefinition(std::uint32_t itemId) noexcept {
    S2InventoryBank bank{};
    std::size_t index{};
    return FindInventorySlot(itemId, bank, index);
}

std::int64_t GetItemState(std::uint32_t itemId) noexcept {
    S2InventoryBank bank{};
    std::size_t index{};
    if (!FindInventorySlot(itemId, bank, index) || IsHiddenStockIndex(index))
        return 0;
    const std::span<const std::byte> stock = S2Save_GetInventoryBank(bank);
    if (index >= stock.size())
        return 0;
    const std::uint32_t quantity = std::to_integer<std::uint8_t>(stock[index]);
    return quantity + (g_inventoryStockBiasEnabled != 0 ? 1U : 0U);
}

std::uint32_t GetMaxItemsForItemType(std::uint32_t category) noexcept {
    return category < itemBanks.size() ? static_cast<std::uint32_t>(itemBanks[category].size()) : 0;
}

std::uint16_t GetInventoryItemIcon(std::uint32_t category, std::uint32_t index) noexcept {
    if (category >= itemBanks.size() || index >= itemBanks[category].size())
        return 0;
    return itemBanks[category][index].icon;
}

std::uint16_t GetInventoryItemIconById(std::uint32_t itemId) noexcept {
    const InventoryItemDefinition *item = FindInventoryItemDefinition(itemId);
    return item ? item->icon : 0;
}

std::uint32_t GetInventoryItemFlags(std::uint32_t category, std::uint32_t index) noexcept {
    if (category >= itemBanks.size() || index >= itemBanks[category].size())
        return 0;
    return itemBanks[category][index].flags;
}

std::uint32_t GetInventoryItemId(std::uint32_t category, std::uint32_t index) noexcept {
    if (category >= itemBanks.size() || index >= itemBanks[category].size())
        return 0xffffffffU;
    return itemBanks[category][index].id;
}

std::uint32_t GetInventoryItemPresentationType(std::uint32_t itemId) noexcept {
    const std::uint32_t group = itemId / 1000;
    return InventoryPresentationTypes[group <= 9 ? group : 0];
}

std::uint32_t hasItemRegistered(std::uint32_t itemId) noexcept {
    const InventoryItemDefinition *item = FindInventoryItemDefinition(itemId);
    return item ? item->flags : 0;
}

const char *GetInventoryItemModel(std::uint32_t itemId) noexcept {
    const InventoryItemDefinition *item = FindInventoryItemDefinition(itemId);
    return item ? item->model : nullptr;
}

const char *GetShenmueItem(std::uint32_t category, std::uint32_t index) noexcept {
    if (category >= itemBanks.size() || index >= itemBanks[category].size())
        return nullptr;
    return itemBanks[category][index].label;
}

const char *shenGetAmtOf(std::uint32_t itemId) noexcept {
    const InventoryItemDefinition *item = FindInventoryItemDefinition(itemId);
    return item ? item->label : nullptr;
}

void GetInventoryItemDistances(std::uint32_t itemId, float *minimum, float *maximum) noexcept {
    const InventoryItemDefinition *item = FindInventoryItemDefinition(itemId);
    *minimum = item ? static_cast<float>(item->minimumDistance) * 0.001f : 0.5f;
    *maximum = item ? static_cast<float>(item->maximumDistance) * 0.001f : 2.0f;
}

std::int64_t AddInventoryItem(std::uint32_t itemId, std::uint8_t amount, std::uint8_t flags) {
    if (static_cast<std::int32_t>(GetItemState(itemId)) >= 255)
        return 0;
    S2InventoryBank bank{};
    std::size_t index{};
    const InventoryItemDefinition *const definition = FindInventorySlot(itemId, bank, index);
    if (!definition)
        return 0;
    const std::span<const std::byte> stock = S2Save_GetInventoryBank(bank);
    if (index >= stock.size())
        return 0;
    const std::uint8_t previous = std::to_integer<std::uint8_t>(stock[index]);
    const std::uint8_t value = (definition->flags & 1U) != 0 ? static_cast<std::uint8_t>(std::min(255, previous + amount)) : 1;
    if (!S2Save_SetInventoryItem(bank, index, static_cast<std::byte>(value)))
        return 0;
    if ((flags & 1U) != 0)
        ToggleItemEffect(itemId, amount);
    if ((definition->flags & 0x10U) != 0)
        doItemEffectStuff();
    return 1;
}

std::int32_t RemoveInventoryItem(std::uint32_t itemId, std::uint8_t amount) noexcept {
    S2InventoryBank bank{};
    std::size_t index{};
    const InventoryItemDefinition *const definition = FindInventorySlot(itemId, bank, index);
    if (!definition || IsHiddenStockIndex(index))
        return -1;
    const std::span<const std::byte> stock = S2Save_GetInventoryBank(bank);
    if (index >= stock.size())
        return -1;
    const std::uint8_t previous = std::to_integer<std::uint8_t>(stock[index]);
    if (previous == 0)
        return -1;
    const std::int32_t remaining = previous - amount;
    const std::uint8_t value = remaining > 0 ? static_cast<std::uint8_t>(remaining) : static_cast<std::uint8_t>((definition->flags & 2U) != 0);
    if (!S2Save_SetInventoryItem(bank, index, static_cast<std::byte>(value)))
        return -1;
    return value != 0 ? 1 : 0;
}

// @verified - p
std::int64_t ImportS1CapsuleStock(const std::uint8_t *source) {
    // ResetItemInventoryState clears the entire persistent bank capacities,
    // including entries beyond the current descriptor counts.
    std::size_t destination = 0;
    for (std::int32_t index = 0; index < 256; ++index) {
        const std::int32_t item = index + 70;
        if (item < 92 || (item > 93 && static_cast<std::uint32_t>(item - 226) > 3U))
            S2Save_SetInventoryItem(S2InventoryBank::Type1, destination++, static_cast<std::byte>(source[index]));
    }
    return 326;
}

// @verified - p
std::int64_t ImportS1KeyItems(const std::uint8_t *source) {
    for (const S1ImportedItem &entry : S1_IMPORTED_ITEMS) {
        const std::uint8_t count = source[entry.index];
        if (count != 0)
            AddInventoryItem(entry.item, count, 0);
    }
    const std::int64_t state = GetItemState(5);
    return state == 0 ? AddInventoryItem(5, 1, 0) : state;
}

void InitItems() {
    ResetItemInventoryState();
    constexpr std::uint16_t initialItems[]{
        0, 2, 5, 1, 6, 8006, 4, 8005, 3, 20, 21,
        4080, 4081, 4082, 4083, 4084, 4085, 4086};
    for (const std::uint16_t item : initialItems)
        AddInventoryItem(item, 1);
    for (std::uint32_t item = 4000; item <= 4015; ++item)
        AddInventoryItem(item, 1);
    for (std::uint32_t item = 4024; item <= 4043; ++item)
        AddInventoryItem(item, 1);
    for (std::uint32_t item = 4060; item <= 4071; ++item)
        AddInventoryItem(item, 1);
}

void RegisterItems() {


    constexpr std::uint16_t requiredItems[]{0, 2, 20, 21};
    for (const std::uint16_t item : requiredItems) {
        if (GetItemState(item) <= 2)
            RemoveInventoryItem(item, 255);
        if (GetItemState(item) != 1)
            AddInventoryItem(item, 1);
    }
}

} // namespace shenmue::s2
