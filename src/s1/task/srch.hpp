#pragma once

#include "s1/hlib.hpp"
#include "s1/task/char.hpp"
#include "common/math.hpp"
#include <array>
#include <cstdint>

namespace shenmue::s1 {
enum class SearchState : std::int32_t;
#pragma pack(push, 1)
struct SearchEntry {
    std::int16_t id = -1;
    std::int16_t entryType = 0;
    std::uint32_t name = 0;
    std::int16_t rotationY = 0;
    std::int16_t useRotationY = 0;
    Vector3 position{};
    std::uint32_t reserved1 = 0;
    std::uint32_t reserved2 = 0;
};
#pragma pack(pop)

static_assert(sizeof(SearchEntry) == 0x20);

enum class SearchCommand : std::int32_t {
    UpdateNormal = -1,
    UpdateAndGetFocused = 1,
    Reset = 2,
    GetCurrentTargetIndex = 3,
    SetEntrySelectorFilter = 4,
    SetExplicitTargetMapInfoId = 5,
    ReturnFocused = 6,
};


struct SearchFocusMapping {
    std::uint32_t identifier;
    std::uint32_t reserved;
    const char *uiToken;
};

static_assert(sizeof(SearchFocusMapping) == 0x10);

inline constexpr SearchFocusMapping kSearchMappingDCBN[]{
    {0x0000000aU, 0U, "UISRCHDCB00"}, // @text EN: "Cassette Players\nBlack" | JA: "ラジカセ（黒）"
    {0x0000000bU, 0U, "UISRCHDCB01"}, // @text EN: "Cassette Players\nRed" | JA: "ラジカセ（赤）"
    {0x0000000cU, 0U, "UISRCHDCB02"}, // @text EN: "Cassette Players\nSilver" | JA: "ラジカセ（銀）"
    {0x00000014U, 0U, "UISRCHDCB03"}, // @text EN: "Game Softs\nSpace Harrier" | JA: "セガサターンソフト\nスペースハリアー"
    {0x00000015U, 0U, "UISRCHDCB04"}, // @text EN: "Game Softs\nHang On" | JA: "セガサターンソフト\nハングオン"
    {0x0000001eU, 0U, "UISRCHDCB05"}, // @text EN: "Cassette Tapes\nM.S.SHOWER" | JA: "ミュージックテープ\nマジカルサウンドシャワー"
    {0x0000001fU, 0U, "UISRCHDCB06"}, // @text EN: "Cassette Tapes\nHANG ON" | JA: "ミュージックテープ\nハングオン"
    {0x00000020U, 0U, "UISRCHDCB07"}, // @text EN: "Cassette Tapes\nS.HARRIER" | JA: "ミュージックテープ\nスペースハリアー"
    {0x00000021U, 0U, "UISRCHDCB08"}, // @text EN: "Cassette Tapes\nF.T.OFF" | JA: "ミュージックテープ\nファイナルテイクオフ"
    {0x00000028U, 0U, "UISRCHDCB09"}, // @text EN: "Rare Capsule Toys\nSuper Sonic" | JA: "ガチャガチャグッズ\nスーパーソニック"
    {0x00000029U, 0U, "UISRCHDCB10"}, // @text EN: "Rare Capsule Toys\nMetal Sonic" | JA: "ガチャガチャグッズ\nメタルソニック"
    {0x0000002aU, 0U, "UISRCHDCB11"}, // @text EN: "Rare Capsule Toys\nHot Dog Truck" | JA: "ガチャガチャグッズ\nアメリカンフードカー"
    {0x0000002bU, 0U, "UISRCHDCB12"}, // @text EN: "Rare Capsule Toys\nB.B. Ultra" | JA: "ガチャガチャグッズ\nＢ．Ｂウルトラ"
    {0x00000032U, 0U, "UISRCHDCB13"}, // @text EN: "Capsule Toys\nLucky Prize" | JA: "ガチャガチャグッズ\n何が当たるかお楽しみ！"
    {SHEN_NAME(cns1), 0U, "UISRCHDCB14"}, // @text EN: "Potato Chips\n120 Yen" | JA: "ポテチ\n１２０円"
    {SHEN_NAME(cnsk), 0U, "UISRCHDCB15"}, // @text EN: "Caramel\n120 Yen" | JA: "キャラメル\n１２０円"
    {SHEN_NAME(cnsc), 0U, "UISRCHDCB16"}, // @text EN: "Chocolate\n120 Yen" | JA: "チョコ\n１２０円"
    {SHEN_NAME(cnkd), 0U, "UISRCHDCB17"}, // @text EN: "Flashlight\n500 Yen" | JA: "懐中電灯\n５００円"
    {SHEN_NAME(cnb1), 0U, "UISRCHDCB18"}, // @text EN: "C Size Batteries\n150 Yen" | JA: "単一電池\n１５０円"
    {SHEN_NAME(cnb3), 0U, "UISRCHDCB19"}, // @text EN: "AA Size Batteries\n150 Yen" | JA: "単三電池\n１５０円"
    {SHEN_NAME(cnrs), 0U, "UISRCHDCB20"}, // @text EN: "Candles\n150 Yen" | JA: "ロウソク\n１５０円"
    {SHEN_NAME(cndg), 0U, "UISRCHDCB21"}, // @text EN: "Light Bulb\n90 Yen" | JA: "電球\n９０円"
    {SHEN_NAME(cnmk), 0U, "UISRCHDCB22"}, // @text EN: "Milk\n100 Yen" | JA: "牛乳\n１００円"
    {SHEN_NAME(cnmp), 0U, "UISRCHDCB23"}, // @text EN: "Sliced Fish\n500 Yen" | JA: "さかなの切り身\n５００円"
    {SHEN_NAME(cnmc), 0U, "UISRCHDCB24"}, // @text EN: "Box of Matches\n50 Yen" | JA: "マッチ\n５０円"
    {SHEN_NAME(cnnp), 0U, "UISRCHDCB25"}, // @text EN: "Dried Fish\n300 Yen" | JA: "煮干し\n３００円"
    {SHEN_NAME(cntk), 0U, "UISRCHDCB26"}, // @text EN: "Canned Tuna\n180 Yen" | JA: "ツナ缶\n１8０円"
    {SHEN_NAME(cnc1), 0U, "UISRCHDCB27"}, // @text EN: "Antiquity Tree\n300 Yen" | JA: "Antiquity Tree\n３００円"
    {SHEN_NAME(cnc2), 0U, "UISRCHDCB28"}, // @text EN: "Flower Girl\n300 Yen" | JA: "Flower Girl\n３００円"
    {SHEN_NAME(cnc3), 0U, "UISRCHDCB29"}, // @text EN: "Linda\n300 Yen" | JA: "Linda\n３００円"
    {SHEN_NAME(cnc4), 0U, "UISRCHDCB30"}, // @text EN: "GoGo\n300 Yen" | JA: "GoGo\n３００円"
    {SHEN_NAME(cnc5), 0U, "UISRCHDCB31"}, // @text EN: "Hip De Hop\n300 Yen" | JA: "Hip de Hop\n３００円"
    {SHEN_NAME(cnc6), 0U, "UISRCHDCB32"}, // @text EN: "Heart Beats\n300 Yen" | JA: "Heart Beats\n３００円"
    {SHEN_NAME(cnc7), 0U, "UISRCHDCB33"}, // @text EN: "Like a Feeling\n300 Yen" | JA: "Like A Feeling\n３００円"
    {SHEN_NAME(cnc8), 0U, "UISRCHDCB34"}, // @text EN: "Feel Tired Song\n300 Yen" | JA: "Feel Tired Song\n３００円"
    {SHEN_NAME(cnc9), 0U, "UISRCHDCB35"}, // @text EN: "Dandy Old Man\n300 Yen" | JA: "Dandy Old Man\n３００円"
    {SHEN_NAME(cnca), 0U, "UISRCHDCB36"}, // @text EN: "Liquor\n300 Yen" | JA: "Liquor\n３００円"
    {SHEN_NAME(cncb), 0U, "UISRCHDCB37"}, // @text EN: "Destiny\n300 Yen" | JA: "Destiny\n３００円"
    {SHEN_NAME(cncc), 0U, "UISRCHDCB38"}, // @text EN: "Boz Nov\n300 Yen" | JA: "Boz Nov\n３００円"
    {SHEN_NAME(cncd), 0U, "UISRCHDCB39"}, // @text EN: "Be-Witch\n300 Yen" | JA: "Be-witch\n３００円"
    {SHEN_NAME(cnce), 0U, "UISRCHDCB40"}, // @text EN: "MJQ\n300 Yen" | JA: "MJQ\n３００円"
    {SHEN_NAME(cncf), 0U, "UISRCHDCB41"}, // @text EN: "Harbor Bar\n300 Yen" | JA: "Harbor Bar\n３００円"
    {SHEN_NAME(cncg), 0U, "UISRCHDCB42"}, // @text EN: "NaNa\n300 Yen" | JA: "NaNa\n３００円"
    {SHEN_NAME(cnch), 0U, "UISRCHDCB43"}, // @text EN: "Spider\n300 Yen" | JA: "Spider\n３００円"
    {SHEN_NAME(cnci), 0U, "UISRCHDCB44"}, // @text EN: "GLYFADA\n300 Yen" | JA: "Glyfada\n３００円"
    {SHEN_NAME(cncj), 0U, "UISRCHDCB45"}, // @text EN: "Y.A.D.A.\n300 Yen" | JA: "Y.A.D.A\n３００円"
    {SHEN_NAME(cnck), 0U, "UISRCHDCB46"}, // @text EN: "Yokosuka Blues\n300 Yen" | JA: "Yokosuka Blues\n３００円"
    {0xffffffffU, 0U, nullptr},
};

inline constexpr SearchFocusMapping kSearchMappingJU00[]{
    {0x00000010U, 0U, "UISRCHJU00"}, // @text EN: "Yamanose Shrine" | JA: "山の瀬稲荷神社"
    {0x00000005U, 0U, "UISRCHJU01"}, // @text EN: "Azumaya" | JA: "東屋"
    {0x00000006U, 0U, "UISRCHJU02"}, // @text EN: "Nakamura" | JA: "中村"
    {0x00000007U, 0U, "UISRCHJU03"}, // @text EN: "Aoyama" | JA: "青山"
    {0x0000000aU, 0U, "UISRCHJU04"}, // @text EN: "Sakurada" | JA: "桜田"
    {0x0000000bU, 0U, "UISRCHJU05"}, // @text EN: "Yoshioka" | JA: "吉岡"
    {0x00000000U, 0U, "UISRCHJU06"}, // @text EN: "Yamanose Apartments" | JA: "山の瀬荘"
    {0x00000001U, 0U, "UISRCHJU07"}, // @text EN: "Kitazawa" | JA: "北沢"
    {0x00000002U, 0U, "UISRCHJU08"}, // @text EN: "Sato" | JA: "佐藤"
    {0x00000003U, 0U, "UISRCHJU09"}, // @text EN: "Usui" | JA: "臼井"
    {0x00000004U, 0U, "UISRCHJU10"}, // @text EN: "Morino" | JA: "森野"
    {0x00000000U, 0U, "UISRCHJU11"}, // @text EN: "Hazuki Residence" | JA: "芭月家"
    {0xffffffffU, 0U, nullptr},
};

inline constexpr SearchFocusMapping kSearchMappingD000[]{
    {0x00001388U, 0U, "UISRCHD000"}, // @text EN: "Hirata\nTobacco Shop" | JA: "タバコ屋\n「平田商店」"
    {0x00001389U, 0U, "UISRCHD001"}, // @text EN: "Smiley\nFlower Shop" | JA: "花屋\n「スマイリー」"
    {0x0000138aU, 0U, "UISRCHD002"}, // @text EN: "Parking Lot" | JA: "「駐車場」"
    {0x0000138bU, 0U, "UISRCHD003"}, // @text EN: "Tamura\nButcher Shop" | JA: "肉屋\n「タムラ精肉店」"
    {0x0000138cU, 0U, "UISRCHD004"}, // @text EN: "Suzume Park" | JA: "「すずめ公園」"
    {0x0000138dU, 0U, "UISRCHD005"}, // @text EN: "Uokichi\nSeafood" | JA: "魚屋\n「魚吉」"
    {0x0000138eU, 0U, "UISRCHD006"}, // @text EN: "Yaokatsu\nProduce" | JA: "八百屋\n「八百勝」"
    {0x0000138fU, 0U, "UISRCHD007"}, // @text EN: "Hattori\nSporting Goods" | JA: "スポーツショップ \nハットリスポーツ"
    {0x00001390U, 0U, "UISRCHD008"}, // @text EN: "Komine\nBakery" | JA: "パン屋\n「コミネベーカリー」"
    {0x00001391U, 0U, "UISRCHD009"}, // @text EN: "Jupitor's\nJackets" | JA: "スカジャン屋\n「ジュピトリス」"
    {0x00001392U, 0U, "UISRCHD010"}, // @text EN: "Mary's\nPatches & Embroidery" | JA: "ワッペン屋\n「マリー商会」"
    {0x00001393U, 0U, "UISRCHD011"}, // @text EN: "Water Dragon\nThrift Store" | JA: "古着屋\n「WaterDragon」"
    {0x00001394U, 0U, "UISRCHD012"}, // @text EN: "Hokuhoku\nLunches" | JA: "「ホクホク弁当」"
    {0x00001395U, 0U, "UISRCHD013"}, // @text EN: "Honda\nDrugstore" | JA: "「本田薬局」"
    {0x00001396U, 0U, "UISRCHD014"}, // @text EN: "Asia\nTravel Co." | JA: "「アジア旅行社」"
    {0x00001397U, 0U, "UISRCHD015"}, // @text EN: "Funny\nBear Burgers" | JA: "バーガーショップ\n「ファニーベア」"
    {0x00001398U, 0U, "UISRCHD016"}, // @text EN: "Water Dragon 2\nThrift Store" | JA: "古着屋\n「JeansWaterDragon」"
    {0x00001399U, 0U, "UISRCHD017"}, // @text EN: "Kurita\nMilitary Surplus" | JA: "ミリタリーショップ\n「クリタ」"
    {0x0000139aU, 0U, "UISRCHD018"}, // @text EN: "Ajiichi\nChinese Restaurant" | JA: "中華\n「味壱」"
    {0x0000139bU, 0U, "UISRCHD019"}, // @text EN: "Lapis\nFortune Teller" | JA: "占いの館\n「ラピス」"
    {0x0000139cU, 0U, "UISRCHD020"}, // @text EN: "Knocking\nMotorcycle Shop" | JA: "バイク屋\n「KNOCKING」"
    {0x0000139dU, 0U, "UISRCHD021"}, // @text EN: "Russiya\nChina Shop" | JA: "陶器屋\n「ロシヤ」"
    {0x0000139eU, 0U, "UISRCHD022"}, // @text EN: "You Arcade" | JA: "「ゲームセンターＹＯＵ」"
    {0x0000139fU, 0U, "UISRCHD023"}, // @text EN: "Slot House" | JA: "「スロットハウス」"
    {0x000013a0U, 0U, "UISRCHD024"}, // @text EN: "Bob's Pizzeria" | JA: "「Bob's Pizza」"
    {0x000013a1U, 0U, "UISRCHD025"}, // @text EN: "Manpukuken\nRamen" | JA: "ラーメン屋\n「満福軒」"
    {0x000013a2U, 0U, "UISRCHD026"}, // @text EN: "MJQ Jazz Bar" | JA: "ジャズバー\n「ＭＪＱ」"
    {0x000013a3U, 0U, "UISRCHD027"}, // @text EN: "Nana's\nKaraoke Bar" | JA: "カラオケパブ\n「ナナ」"
    {0x000013a4U, 0U, "UISRCHD028"}, // @text EN: "Takara\nSushi" | JA: "「宝ずし」"
    {0x000013a5U, 0U, "UISRCHD029"}, // @text EN: "Yamaji\nSoba Noodles" | JA: "そば処\n「やま路」"
    {0x000013a6U, 0U, "UISRCHD030"}, // @text EN: "Aida\nFlorist" | JA: "花屋\n「アイダ」"
    {0x000013a7U, 0U, "UISRCHD031"}, // @text EN: "Maeda\nBarbershop" | JA: "「理容マエダ」"
    {0x000013a8U, 0U, "UISRCHD032"}, // @text EN: "Global\nTravel Agency" | JA: "「世界旅行社」"
    {0x000013a9U, 0U, "UISRCHD033"}, // @text EN: "Tom's Hot Dogs" | JA: "「ホットドッグ屋」"
    {0x000013aaU, 0U, "UISRCHD034"}, // @text EN: "New Yokosuka\nMovie Theatre" | JA: "「新ヨコスカ名画座」"
    {0x000013abU, 0U, "UISRCHD035"}, // @text EN: "Bunkado\nAntiques" | JA: "「骨董文化堂」"
    {0x000013acU, 0U, "UISRCHD036"}, // @text EN: "Tomato\nConvenience Store" | JA: "「トマトマート」"
    {0x000013adU, 0U, "UISRCHD037"}, // @text EN: "Daisangen\nMahjong Parlor" | JA: "麻雀\n「大三元」"
    {0x000013aeU, 0U, "UISRCHD038"}, // @text EN: "Heartbeats Bar" | JA: "「ハートビーツ」"
    {0x000013afU, 0U, "UISRCHD039"}, // @text EN: "Bar Linda" | JA: "スナック\n「リンダ」"
    {0x000013b0U, 0U, "UISRCHD040"}, // @text EN: "Bar Yokosuka" | JA: "ＢＡＲ\n「ヨコスカ」"
    {0x000013b1U, 0U, "UISRCHD041"}, // @text EN: "Nagai Industries" | JA: "「永井興業」"
    {0x000013b2U, 0U, "UISRCHD042"}, // @text EN: "Liu\nBarber and Hair Salon" | JA: "「劉・理髪店」"
    {0x000013b3U, 0U, "UISRCHD043"}, // @text EN: "Okayama\nHeights" | JA: "アパート\n「岡山ハイツ」"
    {0x000013b4U, 0U, "UISRCHD044"}, // @text EN: "Oiwa\nApartments" | JA: "「コーポ大岩」"
    {0x000013b5U, 0U, "UISRCHD045"}, // @text EN: "Mochizuki\nApartments" | JA: "「望月荘」"
    {0x000013b6U, 0U, "UISRCHD046"}, // @text EN: "Maison de Ishikawa" | JA: "「メゾン石川」"
    {0x000013b7U, 0U, "UISRCHD047"}, // @text EN: "Wakaba\nApartments" | JA: "「わかば荘」"
    {0x000013b8U, 0U, "UISRCHD048"}, // @text EN: "Tsukushi\nApartments" | JA: "「つくし荘」"
    {0x00000064U, 0U, "UISRCHD049"}, // @text EN: "Hirata Tobacco Shop\n7:00-18:30" | JA: "タバコ屋 平田商店\n7:00～18:30"
    {0x00000065U, 0U, "UISRCHD050"}, // @text EN: "Smiley Flower Shop\n7:00-18:30" | JA: "花屋スマイリー\n7:00～18:30"
    {0x00000066U, 0U, "UISRCHD051"}, // @text EN: "Maeda Barbershop\n10:00-18:00" | JA: "理容マエダ\n10:00～18:00"
    {0x00000067U, 0U, "UISRCHD052"}, // @text EN: "You Arcade\n10:00-24:00" | JA: "ゲームセンターＹＯＵ\n10:00～24:00"
    {0x00000068U, 0U, "UISRCHD053"}, // @text EN: "Komine Bakery\n7:00-18:30" | JA: "パン屋コミネベーカリー\n7:00～18:30"
    {0x00000069U, 0U, "UISRCHD054"}, // @text EN: "Jupitor's Jacket Shop\n10:00-20:00" | JA: "スカジャン屋ジュピトリス\n10:00～20:00"
    {0x0000006aU, 0U, "UISRCHD055"}, // @text EN: "Mary Patches & Embroidery Shop\n10:00-20:00" | JA: "ワッペン屋マリー商会\n10:00～20:00"
    {0x0000006bU, 0U, "UISRCHD056"}, // @text EN: "Kurita Military Surplus\n10:00-21:00" | JA: "ミリタリーショップ クリタ\n10:00～21:00"
    {0x0000006cU, 0U, "UISRCHD057"}, // @text EN: "Global Travel Agency\n9:00-20:00" | JA: "世界旅行社\n9:00～20:00"
    {0x0000006dU, 0U, "UISRCHD058"}, // @text EN: "Water Dragon Thrift Store\n10:00-20:00" | JA: "古着屋「WaterDragon」\n10:00～20:00"
    {0x0000006eU, 0U, "UISRCHD059"}, // @text EN: "Hokuhoku Lunch Box shop\n10:00-20:00" | JA: "ホクホク弁当\n10:00～20:00"
    {0x0000006fU, 0U, "UISRCHD060"}, // @text EN: "Bunkado Antiques\n10:00-22:00" | JA: "骨董文化堂\n10:00～22:00"
    {0x00000070U, 0U, "UISRCHD061"}, // @text EN: "Takara Sushi\n11:00-21:00" | JA: "宝ずし\n11:00～21:00"
    {0x00000071U, 0U, "UISRCHD062"}, // @text EN: "Aida Flower Shop\n7:00-22:30" | JA: "花屋 アイダ\n7:00～22:30"
    {0x00000072U, 0U, "UISRCHD063"}, // @text EN: "Funny Bear Burgers\n10:00-20:00" | JA: "バーガーショップ ファニーベア\n10:00～20:00"
    {0x00000073U, 0U, "UISRCHD064"}, // @text EN: "Lapis Fortune Teller\n10:00-24:00" | JA: "ラピス\n10:00～24:00"
    {0x00000074U, 0U, "UISRCHD065"}, // @text EN: "Russiya China Shop\n10:00-21:00" | JA: "陶器屋ロシヤ\n10:00～21:00"
    {0x00000075U, 0U, "UISRCHD066"}, // @text EN: "Water Dragon 2 Thrift Store\n10:00-20:00" | JA: "古着屋「JeansWaterDragon」\n10:00～20:00"
    {0x00000076U, 0U, "UISRCHD067"}, // @text EN: "Liu Barber and Hair Salon\n10:00-19:00" | JA: "劉・理髪店\n10:00～19:00"
    {0x00000077U, 0U, "UISRCHD068"}, // @text EN: "Honda Drugstore\n10:00-20:00" | JA: "本田薬局\n10:00～20:00"
    {0x00000078U, 0U, "UISRCHD069"}, // @text EN: "Ajiichi Chinese Restaurant\n10:00-21:00" | JA: "中華 味壱\n10:00～21:00"
    {0x00000079U, 0U, "UISRCHD070"}, // @text EN: "Hattori Sporting Goods\n7:00-18:30" | JA: "スポーツショップ ハットリスポーツ\n7:00～18:30"
    {0x0000007aU, 0U, "UISRCHD071"}, // @text EN: "Daisangen Mahjong Parlor\n12:00-5:00" | JA: "麻雀 大三元\n12:00～5:00"
    {0x0000007bU, 0U, "UISRCHD072"}, // @text EN: "Heartbeats Bar\n19:00-4:00" | JA: "ハートビーツ\n19:00～4:00"
    {0x0000007cU, 0U, "UISRCHD073"}, // @text EN: "Bar Linda \n18:30-5:00" | JA: "スナック リンダ\n18:30～5:00"
    {0x0000007dU, 0U, "UISRCHD074"}, // @text EN: "Bob's Pizzeria\n17:00-4:00" | JA: "ボブスピザ\n17:00～4:00"
    {0x0000007eU, 0U, "UISRCHD075"}, // @text EN: "Nana's Karaoke Bar\n19:00-4:00" | JA: "パブカラオケ ナナ\n19:00～4:00"
    {0x0000007fU, 0U, "UISRCHD076"}, // @text EN: "Manpukuken Ramen\n17:00-4:00" | JA: "満福軒\n17:00～4:00"
    {0x00000080U, 0U, "UISRCHD077"}, // @text EN: "Bar Yokosuka\n17:00-3:00" | JA: "ＢＡＲヨコスカ\n17:00～3:00"
    {0x00000081U, 0U, "UISRCHD078"}, // @text EN: "MJQ Jazz Bar\n19:00-4:00" | JA: "ジャズバーＭＪＱ\n19:00～4:00"
    {0x00000082U, 0U, "UISRCHD079"}, // @text EN: "Asia Travel Co.\n10:00-20:00" | JA: "アジア旅行社\n10:00～20:00"
    {0x00000083U, 0U, "UISRCHD080"}, // @text EN: "Yamaji Soba Noodles\n11:00-21:00" | JA: "そば屋やま路\n11:00～21:00"
    {0x00000084U, 0U, "UISRCHD081"}, // @text EN: "Slot House\n10:00-24:00" | JA: "スロットハウス\n10:00～24:00"
    {0x000001b0U, 0U, "UISRCHD082"}, // @text EN: "Aoki" | JA: "青木"
    {0x000001b0U, 0U, "UISRCHD083"}, // @text EN: "Kurita" | JA: "栗田"
    {0x000001b0U, 0U, "UISRCHD084"}, // @text EN: "Yamamoto" | JA: "山本"
    {0x000001b0U, 0U, "UISRCHD085"}, // @text EN: "Tattoo Parlor" | JA: "タトゥショップ"
    {0x000001b0U, 0U, "UISRCHD086"}, // @text EN: "Kikuchi" | JA: "菊地"
    {0x000001b0U, 0U, "UISRCHD087"}, // @text EN: "Ei" | JA: "永"
    {0x000001b0U, 0U, "UISRCHD088"}, // @text EN: "Go" | JA: "呉"
    {0x000001b0U, 0U, "UISRCHD089"}, // @text EN: "Sawano" | JA: "沢野"
    {0x000001b0U, 0U, "UISRCHD090"}, // @text EN: "Yamaoka" | JA: "山岡"
    {0x000001b0U, 0U, "UISRCHD091"}, // @text EN: "Stellada" | JA: "ステラダ"
    {0x000001b0U, 0U, "UISRCHD092"}, // @text EN: "Hayashi" | JA: "林"
    {0x000001b0U, 0U, "UISRCHD093"}, // @text EN: "Hato" | JA: "波戸"
    {0x000001b0U, 0U, "UISRCHD094"}, // @text EN: "Mochizuki" | JA: "望月"
    {0x000001b0U, 0U, "UISRCHD095"}, // @text EN: "Jimmy" | JA: "ジミ―"
    {0x000001b0U, 0U, "UISRCHD096"}, // @text EN: "Wan" | JA: "王"
    {0x000001b0U, 0U, "UISRCHD097"}, // @text EN: "Minato" | JA: "湊"
    {0x000001b0U, 0U, "UISRCHD098"}, // @text EN: "Nito" | JA: "仁藤"
    {0x000001b0U, 0U, "UISRCHD099"}, // @text EN: "Sato" | JA: "佐藤"
    {0x000001b0U, 0U, "UISRCHD100"}, // @text EN: "Komine" | JA: "小峯"
    {0x000001b0U, 0U, "UISRCHD101"}, // @text EN: "Nemoto" | JA: "根本"
    {0x000001b0U, 0U, "UISRCHD102"}, // @text EN: "Saijo" | JA: "西条"
    {0x000001b0U, 0U, "UISRCHD103"}, // @text EN: "Hirai" | JA: "平井"
    {0x000001b0U, 0U, "UISRCHD104"}, // @text EN: "Tatsumi" | JA: "辰巳"
    {0x000001b0U, 0U, "UISRCHD105"}, // @text EN: "Tsuruoka," | JA: "鶴岡"
    {0x000001b0U, 0U, "UISRCHD106"}, // @text EN: "Ono" | JA: "小野"
    {0x000001b0U, 0U, "UISRCHD107"}, // @text EN: "Yamanaka" | JA: "山中"
    {0x000001b0U, 0U, "UISRCHD108"}, // @text EN: "Hanging Screens" | JA: "垂れ幕"
    {SHEN_NAME(TBK1), 0U, "UISRCHD109"}, // @text EN: "Telephone Directory" | JA: "電話帳"
    {0xffffffffU, 0U, nullptr},
};

inline constexpr SearchFocusMapping kSearchMappingJABE[]{
    {0x00000063U, 0U, "UISRCHJB00"}, // @text EN: "Dural Gold" | JA: "デュラルゴールド"
    {0x0000000aU, 0U, "UISRCHJB01"}, // @text EN: "Dural Silver" | JA: "デュラルシルバー"
    {0x00000014U, 0U, "UISRCHJB02"}, // @text EN: "Mr.Yukawa Happi" | JA: "湯川専務ハッピバージョン"
    {0x0000001eU, 0U, "UISRCHJB03"}, // @text EN: "Mr.Yukawa Suit" | JA: "湯川専務スーツバージョン"
    {0x00000028U, 0U, "UISRCHJB04"}, // @text EN: "Rare Capsule Toys\nR-360" | JA: "ガチャガチャグッズ\nＲ－３６０"
    {0x00000029U, 0U, "UISRCHJB05"}, // @text EN: "Rare Capsule Toys\nSpace Harrier" | JA: "ガチャガチャグッズ\nスペースハリアー"
    {0x0000002aU, 0U, "UISRCHJB06"}, // @text EN: "Rare Capsule Toys\nRent-A-Hero" | JA: "ガチャガチャグッズ\nレンタヒーロー"
    {0x0000002bU, 0U, "UISRCHJB07"}, // @text EN: "Rare Capsule Toys\nBonanza Brothers" | JA: "ガチャガチャグッズ\nボナンザブラザーズ"
    {0x00000032U, 0U, "UISRCHJB08"}, // @text EN: "Capsule Toys\nLucky Prize" | JA: "ガチャガチャグッズ\n何が当たるかお楽しみ！"
    {SHEN_NAME(dap1), 0U, "UISRCHJB09"}, // @text EN: "Potato Chips\n120 Yen" | JA: "ポテチ\n１２０円"
    {SHEN_NAME(dchr), 0U, "UISRCHJB10"}, // @text EN: "Caramel\n120 Yen" | JA: "キャラメル\n１２０円"
    {SHEN_NAME(dcho), 0U, "UISRCHJB11"}, // @text EN: "Chocolate\n120 Yen" | JA: "チョコ\n１２０円"
    {SHEN_NAME(dasr), 0U, "UISRCHJB12"}, // @text EN: "Salami\n90 Yen" | JA: "サラミ\n９０円"
    {SHEN_NAME(daip), 0U, "UISRCHJB13"}, // @text EN: "Squid Legs\n90 Yen" | JA: "イカのあし\n９０円"
    {0xffffffffU, 0U, nullptr},
};

inline constexpr SearchFocusMapping kSearchMappingMFSY[]{
    {0x00001770U, 0U, "UISRCHMFSY00"}, // @text EN: "Hokuhoku\nLunches" | JA: "ホクホク弁当"
    {0x00001771U, 0U, "UISRCHMFSY01"}, // @text EN: "Alpha\nTrading Office" | JA: "アルファ\n貿易事務所"
    {0x00001772U, 0U, "UISRCHMFSY02"}, // @text EN: "Harbor Lounge" | JA: "港湾休憩場"
    {0x00001773U, 0U, "UISRCHMFSY03"}, // @text EN: "Harbor Cafeteria" | JA: "港湾食堂"
    {0x00000009U, 0U, "UISRCHMFSY04"}, // @text EN: " " | JA: " "
    {0x00000009U, 0U, "UISRCHMFSY05"}, // @text EN: "Amihama Town" | JA: "網浜町"
    {0x00000009U, 0U, "UISRCHMFSY06"}, // @text EN: "Central New Yokosuka" | JA: "新港中央"
    {0x00000009U, 0U, "UISRCHMFSY07"}, // @text EN: "Central Pier" | JA: "中央突堤"
    {0x00000009U, 0U, "UISRCHMFSY08"}, // @text EN: "Amihama Wharf" | JA: "網浜埠頭"
    {0x00000009U, 0U, "UISRCHMFSY09"}, // @text EN: "New Warehouse District(Depot)" | JA: "新倉庫街"
    {0x00000009U, 0U, "UISRCHMFSY10"}, // @text EN: "Warehouse No.1(Depot)" | JA: "第１倉庫"
    {0x00000009U, 0U, "UISRCHMFSY11"}, // @text EN: "Warehouse No.2" | JA: "第２倉庫"
    {0x00000009U, 0U, "UISRCHMFSY12"}, // @text EN: "Warehouse No.3" | JA: "第３倉庫"
    {0x00000009U, 0U, "UISRCHMFSY13"}, // @text EN: "Warehouse No.4" | JA: "第４倉庫"
    {0x00000009U, 0U, "UISRCHMFSY14"}, // @text EN: "Warehouse No.5" | JA: "第５倉庫"
    {0x00000009U, 0U, "UISRCHMFSY15"}, // @text EN: "Warehouse No.6" | JA: "第６倉庫"
    {0x00000009U, 0U, "UISRCHMFSY16"}, // @text EN: "Warehouse No.7" | JA: "第７倉庫"
    {0x00000009U, 0U, "UISRCHMFSY17"}, // @text EN: "Warehouse No.8" | JA: "第８倉庫"
    {0x00000009U, 0U, "UISRCHMFSY18"}, // @text EN: "Warehouse No.9" | JA: "第９倉庫"
    {0x00000009U, 0U, "UISRCHMFSY19"}, // @text EN: "Warehouse No.10" | JA: "第１０倉庫"
    {0x00000009U, 0U, "UISRCHMFSY20"}, // @text EN: "Warehouse No.11" | JA: "第１１倉庫"
    {0x00000009U, 0U, "UISRCHMFSY21"}, // @text EN: "Warehouse No.12" | JA: "第１２倉庫"
    {0x00000009U, 0U, "UISRCHMFSY22"}, // @text EN: "Warehouse No.13" | JA: "第１３倉庫"
    {0x00000009U, 0U, "UISRCHMFSY23"}, // @text EN: "Warehouse No.14" | JA: "第１４倉庫"
    {0x00000009U, 0U, "UISRCHMFSY24"}, // @text EN: "Warehouse No.15" | JA: "第１５倉庫"
    {0x00000009U, 0U, "UISRCHMFSY25"}, // @text EN: "Warehouse No.16" | JA: "第１６倉庫"
    {0x00000009U, 0U, "UISRCHMFSY26"}, // @text EN: "Warehouse No.17" | JA: "第１７倉庫"
    {0x00000009U, 0U, "UISRCHMFSY27"}, // @text EN: "Warehouse No.18" | JA: "第１８倉庫"
    {0x00000009U, 0U, "UISRCHMFSY28"}, // @text EN: "Alpha Trading Office" | JA: "アルファ貿易事務所"
    {0x0000001bU, 0U, "UISRCHMFSY29"}, // @text EN: "Harbor Cafeteria\n8:00-20:00" | JA: "港湾食堂\n8:00〜20:00"
    {0x0000001aU, 0U, "UISRCHMFSY30"}, // @text EN: "Harbor Lounge\n8:00-22:00" | JA: "港湾休憩所\n8:00〜22:00"
    {0x00000009U, 0U, "UISRCHMFSY31"}, // @text EN: "Yohai Company Office" | JA: "（株）陽配事務所"
    {0x00000009U, 0U, "UISRCHMFSY32"}, // @text EN: "Yokosuka Distribution Corp." | JA: "（株）横須賀物流事務所"
    {0x00000009U, 0U, "UISRCHMFSY33"}, // @text EN: "Seaside Security" | JA: "臨海警備事業"
    {0x00000009U, 0U, "UISRCHMFSY34"}, // @text EN: "Old Warehouse District" | JA: "旧倉庫街"
    {0x00000009U, 0U, "UISRCHMFSY35"}, // @text EN: "Old Warehouse No.1(Dept)??" | JA: "旧第１倉庫"
    {0x00000009U, 0U, "UISRCHMFSY36"}, // @text EN: "Old Warehouse No.2" | JA: "旧第２倉庫"
    {0x00000009U, 0U, "UISRCHMFSY37"}, // @text EN: "Old Warehouse No.3" | JA: "旧第３倉庫"
    {0x00000009U, 0U, "UISRCHMFSY38"}, // @text EN: "Old Warehouse No.4" | JA: "旧第４倉庫"
    {0x00000009U, 0U, "UISRCHMFSY39"}, // @text EN: "Old Warehouse No.5" | JA: "旧第５倉庫"
    {0x00000009U, 0U, "UISRCHMFSY40"}, // @text EN: "Old Warehouse No.6" | JA: "旧第７倉庫"
    {0x00000009U, 0U, "UISRCHMFSY41"}, // @text EN: "Old Warehouse No.7" | JA: "旧第６倉庫"
    {0x00000009U, 0U, "UISRCHMFSY42"}, // @text EN: "Old Warehouse No.8" | JA: "旧第８倉庫"
    {0x00000009U, 0U, "UISRCHMFSY43"}, // @text EN: "Old Warehouse No.9" | JA: "旧第９倉庫"
    {0x00000009U, 0U, "UISRCHMFSY44"}, // @text EN: "Old Warehouse No.10" | JA: "旧第１０倉庫"
    {0xffffffffU, 0U, nullptr},
};

inline constexpr SearchFocusMapping kSearchMappingMKYU[]{
    {0x0000000aU, 0U, "UISRCHMKYU00"}, // @text EN: "Cassette Players\nBlack" | JA: "ラジカセ（黒）"
    {0x0000000bU, 0U, "UISRCHMKYU01"}, // @text EN: "Cassette Players\nRed" | JA: "ラジカセ（赤）"
    {0x0000000cU, 0U, "UISRCHMKYU02"}, // @text EN: "Cassette Players\nSilver" | JA: "ラジカセ（銀）"
    {0x00000014U, 0U, "UISRCHMKYU03"}, // @text EN: "Game Softs\nSpace Harrier" | JA: "セガサターンソフト\nスペースハリアー"
    {0x00000015U, 0U, "UISRCHMKYU04"}, // @text EN: "Game Softs\nHang On" | JA: "セガサターンソフト\nハングオン"
    {0x0000001eU, 0U, "UISRCHMKYU05"}, // @text EN: "Cassette Tapes\nM.S.SHOWER" | JA: "ミュージックテープ\nマジカルサウンドシャワー"
    {0x0000001fU, 0U, "UISRCHMKYU06"}, // @text EN: "Cassette Tapes\nHANG ON" | JA: "ミュージックテープ\nハングオン"
    {0x00000020U, 0U, "UISRCHMKYU07"}, // @text EN: "Cassette Tapes\nS.HARRIER" | JA: "ミュージックテープ\nスペースハリアー"
    {0x00000021U, 0U, "UISRCHMKYU08"}, // @text EN: "Cassette Tapes\nF.T.OFF" | JA: "ミュージックテープ\nファイナルテイクオフ"
    {0x00000028U, 0U, "UISRCHMKYU09"}, // @text EN: "Rare Capsule Toys\nForklift Red" | JA: "ガチャガチャグッズ\nフォークリフト（赤）"
    {0x00000029U, 0U, "UISRCHMKYU10"}, // @text EN: "Rare Capsule Toys\nForklift Blue" | JA: "ガチャガチャグッズ\nフォークリフト（青）"
    {0x0000002aU, 0U, "UISRCHMKYU11"}, // @text EN: "Rare Capsule Toys\nFerry" | JA: "ガチャガチャグッズ\nフェリー"
    {0x0000002bU, 0U, "UISRCHMKYU12"}, // @text EN: "Rare Capsule Toys\nCrane" | JA: "ガチャガチャグッズ\nクレーン"
    {0x00000032U, 0U, "UISRCHMKYU13"}, // @text EN: "Capsule Toys\nLucky Prize" | JA: "ガチャガチャグッズ\n何が当たるかお楽しみ！"
    {SHEN_NAME(cns1), 0U, "UISRCHMKYU14"}, // @text EN: "Potato Chips\n120 Yen" | JA: "ポテチ\n１２０円"
    {SHEN_NAME(cnsk), 0U, "UISRCHMKYU15"}, // @text EN: "Caramel\n120 Yen" | JA: "キャラメル\n１２０円"
    {SHEN_NAME(cnsc), 0U, "UISRCHMKYU16"}, // @text EN: "Chocolate\n120 Yen" | JA: "チョコ\n１２０円"
    {SHEN_NAME(cnb1), 0U, "UISRCHMKYU17"}, // @text EN: "C Size Batteries\n150 Yen" | JA: "単一電池\n１５０円"
    {SHEN_NAME(cnb3), 0U, "UISRCHMKYU18"}, // @text EN: "AA Size Batteries\n150 Yen" | JA: "単三電池\n１５０円"
    {SHEN_NAME(cnmk), 0U, "UISRCHMKYU19"}, // @text EN: "Milk\n100 Yen" | JA: "牛乳\n１００円"
    {SHEN_NAME(cnmp), 0U, "UISRCHMKYU20"}, // @text EN: "Sliced Fish\n500 Yen" | JA: "さかなの切り身\n５００円"
    {SHEN_NAME(cnnp), 0U, "UISRCHMKYU21"}, // @text EN: "Dried Fish\n300 Yen" | JA: "煮干し\n３００円"
    {SHEN_NAME(cntk), 0U, "UISRCHMKYU22"}, // @text EN: "Canned Tuna\n180 Yen" | JA: "ツナ缶\n１８０円"
    {SHEN_NAME(mnc1), 0U, "UISRCHMKYU23"}, // @text EN: "Hip De Hop\n300 Yen" | JA: "Hip de Hop\n３００円"
    {SHEN_NAME(mnc2), 0U, "UISRCHMKYU24"}, // @text EN: "Like a Feeling\n300 Yen" | JA: "Like A Feeling\n３００円"
    {SHEN_NAME(mnc3), 0U, "UISRCHMKYU25"}, // @text EN: "NaNa\n300 Yen" | JA: "NaNa\n３００円"
    {SHEN_NAME(mnc4), 0U, "UISRCHMKYU26"}, // @text EN: "Spider\n300 Yen" | JA: "Spider\n３００円"
    {SHEN_NAME(mnc5), 0U, "UISRCHMKYU27"}, // @text EN: "Strong\n300 Yen" | JA: "Strong\n３００円"
    {SHEN_NAME(mnc6), 0U, "UISRCHMKYU28"}, // @text EN: "Liquor\n300 Yen" | JA: "Liquor\n３００円"
    {SHEN_NAME(mnc7), 0U, "UISRCHMKYU29"}, // @text EN: "Destiny\n300 Yen" | JA: "Destiny\n３００円"
    {SHEN_NAME(mnc8), 0U, "UISRCHMKYU30"}, // @text EN: "Boz Nov\n300 Yen" | JA: "Boz Nov\n３００円"
    {SHEN_NAME(mnc9), 0U, "UISRCHMKYU31"}, // @text EN: "Harbor Beats\n300 Yen" | JA: "Harbor Beats\n３００円"
    {SHEN_NAME(mnca), 0U, "UISRCHMKYU32"}, // @text EN: "Antiquity Tree\n300 Yen" | JA: "Antiquity Tree\n３００円"
    {SHEN_NAME(mncb), 0U, "UISRCHMKYU33"}, // @text EN: "Flower Girl\n300 Yen" | JA: "Flower Girl\n３００円"
    {SHEN_NAME(mncc), 0U, "UISRCHMKYU34"}, // @text EN: "Be-Witch\n300 Yen" | JA: "Be-witch\n３００円"
    {SHEN_NAME(mncd), 0U, "UISRCHMKYU35"}, // @text EN: "Harbor Bar\n300 Yen" | JA: "Harbor Bar\n３００円"
    {SHEN_NAME(mnce), 0U, "UISRCHMKYU36"}, // @text EN: "Feel Tired Song\n300 Yen" | JA: "Feel Tired Song\n３００円"
    {SHEN_NAME(mncf), 0U, "UISRCHMKYU37"}, // @text EN: "Dandy Old Man\n300 Yen" | JA: "Dandy Old Man\n３００円"
    {SHEN_NAME(mncg), 0U, "UISRCHMKYU38"}, // @text EN: "GLYFADA\n300 Yen" | JA: "Glyfada\n３００円"
    {SHEN_NAME(mnch), 0U, "UISRCHMKYU39"}, // @text EN: "Y.A.D.A.\n300 Yen" | JA: "Y.A.D.A\n３００円"
    {SHEN_NAME(mnci), 0U, "UISRCHMKYU40"}, // @text EN: "Yokosuka Blues\n300 Yen" | JA: "Yokosuka Blues\n３００円"
    {SHEN_NAME(mncj), 0U, "UISRCHMKYU41"}, // @text EN: "Linda\n300 Yen" | JA: "Linda\n３００円"
    {SHEN_NAME(mnck), 0U, "UISRCHMKYU42"}, // @text EN: "GoGo\n300 Yen" | JA: "GoGo\n３００円"
    {SHEN_NAME(GCH0), 0U, nullptr},
    {SHEN_NAME(GCH1), 0U, nullptr},
    {SHEN_NAME(GCH2), 0U, nullptr},
    {SHEN_NAME(GCH3), 0U, nullptr},
    {SHEN_NAME(GCH4), 0U, nullptr},
    {SHEN_NAME(GCH5), 0U, nullptr},
    {0xffffffffU, 0U, nullptr},
};

inline constexpr SearchFocusMapping kSearchMappingJD00[]{
    {0x00000fa0U, 0U, "UISRCHJD00"}, // @text EN: "Abe Store" | JA: "阿部商店"
    {0x00000fa1U, 0U, "UISRCHJD01"}, // @text EN: "Sakura Heights" | JA: "ハイツさくら"
    {0x00000fa2U, 0U, "UISRCHJD02"}, // @text EN: "Sakura Park" | JA: "桜ヶ丘公園"
    {0x00000067U, 0U, "UISRCHJD03"}, // @text EN: "Hirano" | JA: "平野"
    {0x00000068U, 0U, "UISRCHJD04"}, // @text EN: "Kondo" | JA: "近藤"
    {0x00000069U, 0U, "UISRCHJD05"}, // @text EN: "Iwata" | JA: "岩田"
    {0x0000006aU, 0U, "UISRCHJD06"}, // @text EN: "Nomura" | JA: "野村"
    {0x00000065U, 0U, "UISRCHJD07"}, // @text EN: "Yamamoto" | JA: "山本"
    {0x0000006bU, 0U, "UISRCHJD08"}, // @text EN: "Mitsui" | JA: "三井"
    {0x0000006cU, 0U, "UISRCHJD09"}, // @text EN: "Akasaka" | JA: "赤坂"
    {0x0000006dU, 0U, "UISRCHJD10"}, // @text EN: "Tajima" | JA: "田嶋"
    {0x00000066U, 0U, "UISRCHJD11"}, // @text EN: "Ito" | JA: "伊藤"
    {0x0000006eU, 0U, "UISRCHJD12"}, // @text EN: "Yamagishi" | JA: "山岸"
    {0x0000006fU, 0U, "UISRCHJD13"}, // @text EN: "Nishida" | JA: "西田"
    {0x00000070U, 0U, "UISRCHJD14"}, // @text EN: "Liu" | JA: "劉"
    {0x00000071U, 0U, "UISRCHJD15"}, // @text EN: "Mishima" | JA: "三島"
    {0x00000072U, 0U, "UISRCHJD16"}, // @text EN: "Takahashi" | JA: "高橋"
    {0x00000073U, 0U, "UISRCHJD17"}, // @text EN: "Takashima" | JA: "高島"
    {0x00000074U, 0U, "UISRCHJD18"}, // @text EN: "Sumiya" | JA: "住谷"
    {0x00000075U, 0U, "UISRCHJD19"}, // @text EN: "Tachibana" | JA: "橘"
    {0x00000076U, 0U, "UISRCHJD20"}, // @text EN: "Sugisawa" | JA: "杉沢"
    {0x00000077U, 0U, "UISRCHJD21"}, // @text EN: "Kimura" | JA: "木村"
    {0x00000078U, 0U, "UISRCHJD22"}, // @text EN: "Inoue" | JA: "井上"
    {0x00000079U, 0U, "UISRCHJD23"}, // @text EN: "Itoi" | JA: "糸井"
    {0x0000007aU, 0U, "UISRCHJD24"}, // @text EN: "Motoyama" | JA: "本山"
    {0x0000007bU, 0U, "UISRCHJD25"}, // @text EN: "Udagawa" | JA: "宇田川"
    {0x0000007cU, 0U, "UISRCHJD26"}, // @text EN: "Aizawa" | JA: "相澤"
    {0x0000007dU, 0U, "UISRCHJD27"}, // @text EN: "Iwasaki" | JA: "岩崎"
    {0x0000007eU, 0U, "UISRCHJD28"}, // @text EN: "Kayama" | JA: "加山"
    {0x0000007fU, 0U, "UISRCHJD29"}, // @text EN: "Sakura Heights" | JA: "ハイツさくら"
    {0x00000080U, 0U, "UISRCHJD30"}, // @text EN: "Sakura Heights" | JA: "ハイツさくら"
    {0x00000085U, 0U, "UISRCHJD31"}, // @text EN: "Sakura Park" | JA: "桜ヶ丘公園"
    {0x00000089U, 0U, "UISRCHJD32"}, // @text EN: "Abe Store\n9:00-19:00" | JA: "阿部商店\n9:00〜19:00"
    {0xffffffffU, 0U, nullptr},
};

inline constexpr SearchFocusMapping kSearchMappingJOMO[]{
    {SHEN_NAME(IGAK), 0U, "UISRCHJM00"}, // @text EN: "“Be ever unyielding”" | JA: "不撓不屈"
    {0x00000066U, 0U, "UISRCHJM01"}, // @text EN: "“Always maintain composure & presence of mind”" | JA: "泰然自若"
    {0x00000064U, 0U, "UISRCHJM02"}, // @text EN: "“Family Altar (Buddhist)”" | JA: "仏壇"
    {0x00000067U, 0U, "UISRCHJM03"}, // @text EN: "“Heaven Dragon Earth Comrade”" | JA: "天龍地友"
    {SHEN_NAME(firi), 0U, "UISRCHJM04"}, // @text EN: "“Fukuhara”" | JA: "福原"
    {SHEN_NAME(FGAK), 0U, "UISRCHJM05"}, // @text EN: "“Keep your mind as clean as a polished mirror”" | JA: "明鏡止水"
    {SHEN_NAME(TLBK), 0U, "UISRCHJM06"}, // @text EN: "“Telephone Directory”" | JA: "電話帳"
    {0xffffffffU, 0U, nullptr},
};

inline constexpr SearchFocusMapping kSearchMappingJHD0[]{
    {0x000007d0U, 0U, "UISRCHJH00"}, // @text EN: "“The Eight Principles of Yin and Yang”" | JA: "八陰八陽"
    {SHEN_NAME(GART), 0U, "UISRCHJH00"}, // @text EN: "“The Eight Principles of Yin and Yang”" | JA: "八陰八陽"
    {SHEN_NAME(KAK1), 0U, "UISRCHJH01"}, // @text EN: "“Principles cannot be expressed only by words”" | JA: "道可道非常道"
    {0xffffffffU, 0U, nullptr},
};

inline constexpr SearchFocusMapping kSearchMappingDKTY[]{
    {SHEN_NAME(TOKE), 0U, nullptr},
    {SHEN_NAME(howt), 0U, nullptr},
    {SHEN_NAME(kttb), 0U, nullptr},
    {SHEN_NAME(bdor), 0U, nullptr},
    {SHEN_NAME(ma01), 0U, "UISRCHDKT00"}, // @text EN: "A scroll of the move\n'Mud Spider.' 1000 Yen" | JA: "土蜘蛛の技書\n１０００円"
    {SHEN_NAME(ma02), 0U, "UISRCHDKT01"}, // @text EN: "A scroll of the move\n'Rising Flash.' 500 Yen" | JA: "裂晃閃の技書\n５００円"
    {SHEN_NAME(ma03), 0U, "UISRCHDKT02"}, // @text EN: "A scroll of the move\n'Crawl Cyclone.' 1000 Yen" | JA: "旋風の技書\n１０００円"
    {SHEN_NAME(ma04), 0U, "UISRCHDKT03"}, // @text EN: "A scroll of the move\n'Twin Swallow Leap.' 1000 Yen" | JA: "飛燕連刈の技書\n１０００円"
    {SHEN_NAME(ma05), 0U, "UISRCHDKT04"}, // @text EN: "A scroll of the move\n'Tiger Storm.' 2000 Yen" | JA: "虎身崩しの技書\n２０００円"
    {SHEN_NAME(ma06), 0U, "UISRCHDKT05"}, // @text EN: "A scroll of the move\n'Arm Break Fire.' 3000 Yen" | JA: "朧無双の技書\n３０００円"
    {0xffffffffU, 0U, nullptr},
};

extern std::uint32_t g_searchTemporaryFocusedCharacter;

enum class SearchState : std::int32_t {
    Inactive = 0,
    IdleAwaitingTarget = 1,
    PendingScriptedLockOn = 2,
    ScriptedLockOnActive = 3,
    AutoCharacterLockOn = 4,
    RecenterAndFlipToScript = 5,
    ExitAndRestoreState = 6,
};

void SRCH_Init();
void SRCH_Shutdown();
HLib::Task *SRCH_GetTask();
std::int32_t SRCH_GetBoundEntryCount();
void SRCH_InitEx(const SearchEntry *entries);
void SRCH_Reset();
void SRCH_ClearLockAndHandleReset();
void SRCH_Callback(HLib::TaskData *taskData);
void srchUpdateFocusedItem();
std::int32_t SRCH_FindCandidate(bool screenSpace);
std::uint32_t SRCH_ExecuteCommand(SearchCommand command, std::int32_t operand = 0);
void SRCH_ReleaseLockOnCamera();
std::int32_t SRCH_GetCurrentIdentifier();
void SRCH_SetCharacterSearchExtent(bool primary, float value);

void SRCH_SetState(SearchState state, bool cameraTransitionActive);
SearchState SRCH_GetState();
bool SRCH_AllowsIconInputPriority();
bool shenHasFocusedNotLockedOn();
void SRCH_UpdateCameraEyeAndTarget(Vector3 &eye, Vector3 &target);

void SRCH_UpdatePromptState(std::uint16_t pressedPriority, std::uint16_t triggers, bool alternatePromptLayout, bool swapTriggers);
const std::array<std::int16_t, 5> &SRCH_GetPromptIconSlots();
const std::array<std::int16_t, 5> &SRCH_GetRequestedPromptIconSlots();

struct SearchScreenConstraint {
    float x;
    float z;
    std::uint16_t angle;
    std::int32_t value;
};

inline constexpr SearchScreenConstraint kSearchScreenConstraints[] = {
    {-6.8000002f, -16.5f, 32768, 2000},
    {14.5f, 21.5f, 48969, 4000},
    {-10.5f, 23.4f, 17294, 4001},
    {13.8f, 2.0f, 1820, 4002},
    {13.6f, 1.6f, 50062, 4002},
    {38.299999f, 8.8999996f, 43890, 5000},
    {28.1f, 13.7f, 10868, 5001},
    {35.400002f, 16.1f, 44309, 5002},
    {12.3f, 31.9f, 32622, 5003},
    {15.5f, 48.5f, 7955, 5004},
    {15.4f, 48.099998f, 23993, 5004},
    {-34.599998f, 39.0f, 983, 5005},
    {-73.300003f, 63.0f, 32312, 5006},
    {-96.400002f, 64.0f, 4751, 5007},
    {6.1999998f, 26.9f, 65171, 5008},
    {2.3f, 31.4f, 32403, 5009},
    {-9.0f, 27.1f, 345, 5010},
    {-51.799999f, 41.200001f, 6590, 5011},
    {-51.799999f, 48.400002f, 38447, 5012},
    {-91.599998f, 62.299999f, 2439, 5013},
    {-70.300003f, 77.900002f, 15674, 5014},
    {-72.199997f, 80.099998f, 200, 5014},
    {-67.099998f, 91.199997f, 32058, 5015},
    {-31.1f, 73.400002f, 65135, 5016},
    {-14.4f, 30.0f, 7645, 5017},
    {-95.900002f, 66.900002f, 37428, 5018},
    {-54.200001f, 82.099998f, 4496, 5019},
    {-117.3f, 84.099998f, 42070, 5020},
    {-40.400002f, 75.300003f, 2821, 5021},
    {22.4f, 32.900002f, 31839, 5022},
    {-123.1f, 84.0f, 9393, 5023},
    {-123.3f, 94.699997f, 18149, 5024},
    {-111.3f, 102.4f, 27980, 5025},
    {-97.599998f, 85.400002f, 4132, 5026},
    {-114.9f, 101.0f, 27889, 5027},
    {-64.199997f, 68.800003f, 49388, 5028},
    {19.9f, 49.200001f, 49388, 5029},
    {-64.0f, 71.5f, 49261, 5030},
    {22.9f, 23.299999f, 11122, 5031},
    {-18.799999f, 33.200001f, 7136, 5032},
    {-17.0f, 38.200001f, 39667, 5033},
    {-40.099998f, 45.799999f, 31675, 5034},
    {-58.099998f, 45.099998f, 6353, 5035},
    {-67.5f, 53.099998f, 12925, 5036},
    {-111.4f, 69.400002f, 4205, 5037},
    {-125.9f, 65.099998f, 25886, 5038},
    {-124.1f, 88.699997f, 18786, 5039},
    {-105.6f, 93.300003f, 9357, 5040},
    {-19.0f, 72.599998f, 64971, 5041},
    {5.1999998f, 84.599998f, 58636, 5042},
    {-103.9f, 98.5f, 42088, 5043},
    {-51.0f, 86.300003f, 37865, 5044},
    {-31.700001f, 82.199997f, 31493, 5045},
    {-5.0f, 77.599998f, 60912, 5046},
    {44.200001f, 87.099998f, 23993, 5047},
    {-1.4f, 89.699997f, 25759, 5048},
    {2.2f, 103.7f, 41870, 6000},
    {-17.6f, 80.0f, 24576, 6001},
    {-24.4f, 73.699997f, 24576, 6001},
    {-86.599998f, 56.5f, 57344, 6002},
    {-83.599998f, 60.0f, 57344, 6002},
    {-82.699997f, 68.5f, 57344, 6003},
    {0.0f, 0.0f, 0, -1},
};

} // namespace shenmue::s1
