#pragma once
#include "s2/area_names.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {
#pragma pack(push, 1)
struct TitleSceneEntry {
    std::uint8_t act;
    shen_name area;
    std::int8_t entry;
};
#pragma pack(pop)
static_assert(sizeof(TitleSceneEntry) == 6);
static_assert(alignof(TitleSceneEntry) == 1);
static_assert(offsetof(TitleSceneEntry, area) == 1);
static_assert(offsetof(TitleSceneEntry, entry) == 5);

inline constexpr shen_name TITLE_AREA_0000{SHEN_NAME(0000)};
inline constexpr shen_name TITLE_AREA_0004{SHEN_NAME(0004)};
inline constexpr shen_name TITLE_AREA_0005{SHEN_NAME(0005)};
inline constexpr shen_name TITLE_AREA_0200{SHEN_NAME(0200)};
inline constexpr shen_name TITLE_AREA_1000{SHEN_NAME(1000)};
inline constexpr shen_name TITLE_AREA_AB00{SHEN_NAME(AB00)};
inline constexpr shen_name TITLE_AREA_ABLB{SHEN_NAME(ABLB)};
inline constexpr shen_name TITLE_AREA_AK00{SHEN_NAME(AK00)};
inline constexpr shen_name TITLE_AREA_AKS0{SHEN_NAME(AKS0)};
inline constexpr shen_name TITLE_AREA_AKS1{SHEN_NAME(AKS1)};
inline constexpr shen_name TITLE_AREA_AKT0{SHEN_NAME(AKT0)};
inline constexpr shen_name TITLE_AREA_ANAN{SHEN_NAME(ANAN)};
inline constexpr shen_name TITLE_AREA_AR01{SHEN_NAME(AR01)};
inline constexpr shen_name TITLE_AREA_AR02{SHEN_NAME(AR02)};
inline constexpr shen_name TITLE_AREA_AR03{SHEN_NAME(AR03)};
inline constexpr shen_name TITLE_AREA_ARA0{SHEN_NAME(ARA0)};
inline constexpr shen_name TITLE_AREA_ARBT{SHEN_NAME(ARBT)};
inline constexpr shen_name TITLE_AREA_ARKQ{SHEN_NAME(ARKQ)};
inline constexpr shen_name TITLE_AREA_ARMW{SHEN_NAME(ARMW)};
inline constexpr shen_name TITLE_AREA_BGSM{SHEN_NAME(BGSM)};
inline constexpr shen_name TITLE_AREA_BKW0{SHEN_NAME(BKW0)};
inline constexpr shen_name TITLE_AREA_BOSY{SHEN_NAME(BOSY)};
inline constexpr shen_name TITLE_AREA_CWON{SHEN_NAME(CWON)};
inline constexpr shen_name TITLE_AREA_DABO{SHEN_NAME(DABO)};
inline constexpr shen_name TITLE_AREA_DMAS{SHEN_NAME(DMAS)};
inline constexpr shen_name TITLE_AREA_DUCK{SHEN_NAME(DUCK)};
inline constexpr shen_name TITLE_AREA_GABT{SHEN_NAME(GABT)};
inline constexpr shen_name TITLE_AREA_GD00{SHEN_NAME(GD00)};
inline constexpr shen_name TITLE_AREA_KMZ1{SHEN_NAME(KMZ1)};
inline constexpr shen_name TITLE_AREA_KMZ2{SHEN_NAME(KMZ2)};
inline constexpr shen_name TITLE_AREA_KMZ3{SHEN_NAME(KMZ3)};
inline constexpr shen_name TITLE_AREA_KMZ4{SHEN_NAME(KMZ4)};
inline constexpr shen_name TITLE_AREA_KNMN{SHEN_NAME(KNMN)};
inline constexpr shen_name TITLE_AREA_KNW0{SHEN_NAME(KNW0)};
inline constexpr shen_name TITLE_AREA_KRC1{SHEN_NAME(KRC1)};
inline constexpr shen_name TITLE_AREA_KRF1{SHEN_NAME(KRF1)};
inline constexpr shen_name TITLE_AREA_KRH1{SHEN_NAME(KRH1)};
inline constexpr shen_name TITLE_AREA_KRK6{SHEN_NAME(KRK6)};
inline constexpr shen_name TITLE_AREA_KRM3{SHEN_NAME(KRM3)};
inline constexpr shen_name TITLE_AREA_KSH1{SHEN_NAME(KSH1)};
inline constexpr shen_name TITLE_AREA_KWA1{SHEN_NAME(KWA1)};
inline constexpr shen_name TITLE_AREA_KWA2{SHEN_NAME(KWA2)};
inline constexpr shen_name TITLE_AREA_KWM1{SHEN_NAME(KWM1)};
inline constexpr shen_name TITLE_AREA_KWQA{SHEN_NAME(KWQA)};
inline constexpr shen_name TITLE_AREA_KWQB{SHEN_NAME(KWQB)};
inline constexpr shen_name TITLE_AREA_KWQC{SHEN_NAME(KWQC)};
inline constexpr shen_name TITLE_AREA_KWW1{SHEN_NAME(KWW1)};
inline constexpr shen_name TITLE_AREA_KWW4{SHEN_NAME(KWW4)};
inline constexpr shen_name TITLE_AREA_MAP0{SHEN_NAME(MAP0)};
inline constexpr shen_name TITLE_AREA_MFW0{SHEN_NAME(MFW0)};
inline constexpr shen_name TITLE_AREA_OMAK{SHEN_NAME(OMAK)};
inline constexpr shen_name TITLE_AREA_Q100{SHEN_NAME(Q100)};
inline constexpr shen_name TITLE_AREA_Q300{SHEN_NAME(Q300)};
inline constexpr shen_name TITLE_AREA_Q7QB{SHEN_NAME(Q7QB)};
inline constexpr shen_name TITLE_AREA_QA00{SHEN_NAME(QA00)};
inline constexpr shen_name TITLE_AREA_QC00{SHEN_NAME(QC00)};
inline constexpr shen_name TITLE_AREA_QC01{SHEN_NAME(QC01)};
inline constexpr shen_name TITLE_AREA_QD00{SHEN_NAME(QD00)};
inline constexpr shen_name TITLE_AREA_QD01{SHEN_NAME(QD01)};
inline constexpr shen_name TITLE_AREA_QEEC{SHEN_NAME(QEEC)};
inline constexpr shen_name TITLE_AREA_QES1{SHEN_NAME(QES1)};
inline constexpr shen_name TITLE_AREA_QF00{SHEN_NAME(QF00)};
inline constexpr shen_name TITLE_AREA_QF02{SHEN_NAME(QF02)};
inline constexpr shen_name TITLE_AREA_QKBT{SHEN_NAME(QKBT)};
inline constexpr shen_name TITLE_AREA_QLBT{SHEN_NAME(QLBT)};
inline constexpr shen_name TITLE_AREA_QR00{SHEN_NAME(QR00)};
inline constexpr shen_name TITLE_AREA_QRC0{SHEN_NAME(QRC0)};
inline constexpr shen_name TITLE_AREA_SLOT{SHEN_NAME(SLOT)};
inline constexpr shen_name TITLE_AREA_STEP{SHEN_NAME(STEP)};
inline constexpr shen_name TITLE_AREA_SYW0{SHEN_NAME(SYW0)};
inline constexpr shen_name TITLE_AREA_TEST{SHEN_NAME(TEST)};
inline constexpr shen_name TITLE_AREA_TOFB{SHEN_NAME(TOFB)};
inline constexpr shen_name TITLE_AREA_UEDE{SHEN_NAME(UEDE)};
inline constexpr shen_name TITLE_AREA_VIEW{SHEN_NAME(VIEW)};
inline constexpr shen_name TITLE_AREA_VSYB{SHEN_NAME(VSYB)};
inline constexpr shen_name TITLE_AREA_WB00{SHEN_NAME(WB00)};
inline constexpr shen_name TITLE_AREA_WB01{SHEN_NAME(WB01)};
inline constexpr shen_name TITLE_AREA_WBSS{SHEN_NAME(WBSS)};
inline constexpr shen_name TITLE_AREA_WEG0{SHEN_NAME(WEG0)};
inline constexpr shen_name TITLE_AREA_WEM1{SHEN_NAME(WEM1)};
inline constexpr shen_name TITLE_AREA_WK00{SHEN_NAME(WK00)};
inline constexpr shen_name TITLE_AREA_WKKT{SHEN_NAME(WKKT)};
inline constexpr shen_name TITLE_AREA_WN00{SHEN_NAME(WN00)};
inline constexpr shen_name TITLE_AREA_WR00{SHEN_NAME(WR00)};
inline constexpr shen_name TITLE_AREA_WRS2{SHEN_NAME(WRS2)};
inline constexpr shen_name TITLE_AREA_WS00{SHEN_NAME(WS00)};
inline constexpr shen_name TITLE_AREA_WTKA{SHEN_NAME(WTKA)};
inline constexpr shen_name TITLE_AREA_YTQB{SHEN_NAME(YTQB)};

inline constexpr std::array<TitleSceneEntry, 35> TITLE_SCENES_DEBUG{{
    {.act = 1, .area = TITLE_AREA_TEST, .entry = 0},
    {.act = 1, .area = TITLE_AREA_MAP0, .entry = 0},
    {.act = 1, .area = TITLE_AREA_VIEW, .entry = 0},
    {.act = 1, .area = TITLE_AREA_1000, .entry = 0},
    {.act = 2, .area = TITLE_AREA_AB00, .entry = 1},
    {.act = 3, .area = TITLE_AREA_QA00, .entry = 1},
    {.act = 3, .area = TITLE_AREA_QC00, .entry = 1},
    {.act = 3, .area = AREA_QE00, .entry = 1},
    {.act = 3, .area = TITLE_AREA_QF00, .entry = 1},
    {.act = 3, .area = AREA_QF39, .entry = 1},
    {.act = 3, .area = TITLE_AREA_Q100, .entry = 1},
    {.act = 3, .area = AREA_QTB1, .entry = 1},
    {.act = 3, .area = AREA_QSFA, .entry = 1},
    {.act = 3, .area = AREA_QSFB, .entry = 1},
    {.act = 3, .area = AREA_QSFC, .entry = 1},
    {.act = 3, .area = AREA_QSFD, .entry = 1},
    {.act = 3, .area = AREA_QGBT, .entry = 1},
    {.act = 3, .area = AREA_QJBT, .entry = 1},
    {.act = 3, .area = TITLE_AREA_QKBT, .entry = 1},
    {.act = 3, .area = TITLE_AREA_QLBT, .entry = 1},
    {.act = 2, .area = TITLE_AREA_BKW0, .entry = 0},
    {.act = 3, .area = TITLE_AREA_GABT, .entry = 0},
    {.act = 2, .area = AREA_JIAG, .entry = 0},
    {.act = 2, .area = TITLE_AREA_SYW0, .entry = 0},
    {.act = 1, .area = TITLE_AREA_WB00, .entry = 1},
    {.act = 1, .area = TITLE_AREA_WK00, .entry = 1},
    {.act = 2, .area = TITLE_AREA_AK00, .entry = 1},
    {.act = 2, .area = TITLE_AREA_WB00, .entry = 1},
    {.act = 2, .area = TITLE_AREA_WK00, .entry = 1},
    {.act = 2, .area = TITLE_AREA_WS00, .entry = 1},
    {.act = 3, .area = AREA_QDTB, .entry = 0},
    {.act = 3, .area = AREA_QFRR, .entry = 1},
    {.act = 3, .area = TITLE_AREA_STEP, .entry = 0},
    {.act = 4, .area = TITLE_AREA_GD00, .entry = 1},
    {.act = 255, .area = shen_name{}, .entry = 0},
}};

inline constexpr std::array<TitleSceneEntry, 21> TITLE_SCENES_SCENE1{{
    {.act = 1, .area = TITLE_AREA_AK00, .entry = 1},
    {.act = 1, .area = AREA_AKA3, .entry = 1},
    {.act = 1, .area = TITLE_AREA_AKT0, .entry = 1},
    {.act = 1, .area = TITLE_AREA_AR01, .entry = 0},
    {.act = 1, .area = TITLE_AREA_AR02, .entry = 25},
    {.act = 1, .area = TITLE_AREA_AR03, .entry = 28},
    {.act = 1, .area = TITLE_AREA_WB00, .entry = 1},
    {.act = 1, .area = TITLE_AREA_WB01, .entry = 99},
    {.act = 1, .area = AREA_WE00, .entry = 1},
    {.act = 1, .area = TITLE_AREA_WEG0, .entry = 1},
    {.act = 1, .area = TITLE_AREA_WEM1, .entry = 1},
    {.act = 1, .area = TITLE_AREA_WK00, .entry = 1},
    {.act = 1, .area = AREA_WKA0, .entry = 1},
    {.act = 1, .area = TITLE_AREA_WN00, .entry = 1},
    {.act = 1, .area = TITLE_AREA_WR00, .entry = 1},
    {.act = 1, .area = TITLE_AREA_WRS2, .entry = 1},
    {.act = 1, .area = TITLE_AREA_WS00, .entry = 1},
    {.act = 1, .area = AREA_WSY0, .entry = 1},
    {.act = 1, .area = AREA_WT00, .entry = 1},
    {.act = 1, .area = AREA_WTA0, .entry = 1},
    {.act = 255, .area = shen_name{}, .entry = 0},
}};

inline constexpr std::array<TitleSceneEntry, 23> TITLE_SCENES_SCENE2{{
    {.act = 2, .area = TITLE_AREA_AB00, .entry = 1},
    {.act = 2, .area = TITLE_AREA_AK00, .entry = 1},
    {.act = 2, .area = TITLE_AREA_AKT0, .entry = 1},
    {.act = 2, .area = AREA_AKA3, .entry = 1},
    {.act = 2, .area = TITLE_AREA_AR01, .entry = 0},
    {.act = 2, .area = TITLE_AREA_AR02, .entry = 25},
    {.act = 2, .area = TITLE_AREA_AR03, .entry = 28},
    {.act = 2, .area = AREA_ARC0, .entry = 1},
    {.act = 2, .area = TITLE_AREA_WB00, .entry = 1},
    {.act = 2, .area = TITLE_AREA_WB01, .entry = 99},
    {.act = 2, .area = AREA_WE00, .entry = 1},
    {.act = 2, .area = TITLE_AREA_WEG0, .entry = 1},
    {.act = 2, .area = TITLE_AREA_WEM1, .entry = 1},
    {.act = 2, .area = TITLE_AREA_WK00, .entry = 1},
    {.act = 2, .area = AREA_WKA0, .entry = 1},
    {.act = 2, .area = TITLE_AREA_WN00, .entry = 1},
    {.act = 2, .area = TITLE_AREA_WR00, .entry = 1},
    {.act = 2, .area = TITLE_AREA_WRS2, .entry = 1},
    {.act = 2, .area = TITLE_AREA_WS00, .entry = 1},
    {.act = 2, .area = AREA_WSY0, .entry = 1},
    {.act = 2, .area = AREA_WT00, .entry = 1},
    {.act = 2, .area = AREA_WTA0, .entry = 1},
    {.act = 255, .area = shen_name{}, .entry = 0},
}};

inline constexpr std::array<TitleSceneEntry, 33> TITLE_SCENES_SCENE3{{
    {.act = 3, .area = TITLE_AREA_Q100, .entry = 0},
    {.act = 3, .area = AREA_Q200, .entry = 0},
    {.act = 3, .area = TITLE_AREA_Q300, .entry = 0},
    {.act = 3, .area = TITLE_AREA_QA00, .entry = 0},
    {.act = 3, .area = AREA_QAW1, .entry = 0},
    {.act = 3, .area = AREA_QAE1, .entry = 0},
    {.act = 3, .area = AREA_QAW6, .entry = 0},
    {.act = 3, .area = AREA_QAE6, .entry = 0},
    {.act = 3, .area = AREA_QA11, .entry = 0},
    {.act = 3, .area = AREA_QAXX, .entry = 1},
    {.act = 3, .area = AREA_QB00, .entry = 1},
    {.act = 3, .area = TITLE_AREA_QC00, .entry = 0},
    {.act = 3, .area = TITLE_AREA_QC01, .entry = 0},
    {.act = 3, .area = AREA_QC06, .entry = 0},
    {.act = 3, .area = TITLE_AREA_QD00, .entry = 0},
    {.act = 3, .area = TITLE_AREA_QD01, .entry = 0},
    {.act = 3, .area = AREA_QE00, .entry = 1},
    {.act = 3, .area = AREA_QE01, .entry = 0},
    {.act = 3, .area = AREA_QE06, .entry = 0},
    {.act = 3, .area = TITLE_AREA_QEEC, .entry = 1},
    {.act = 3, .area = AREA_QEHI, .entry = 1},
    {.act = 3, .area = TITLE_AREA_QF00, .entry = 0},
    {.act = 3, .area = AREA_QF01, .entry = 0},
    {.act = 3, .area = TITLE_AREA_QF02, .entry = 0},
    {.act = 3, .area = AREA_QF06, .entry = 0},
    {.act = 3, .area = AREA_QF39, .entry = 0},
    {.act = 3, .area = AREA_QF40, .entry = 1},
    {.act = 3, .area = TITLE_AREA_QR00, .entry = 1},
    {.act = 3, .area = TITLE_AREA_QRC0, .entry = 1},
    {.act = 3, .area = AREA_QRR0, .entry = 1},
    {.act = 3, .area = AREA_QTB1, .entry = 1},
    {.act = 3, .area = AREA_QUG0, .entry = 1},
    {.act = 255, .area = shen_name{}, .entry = 0},
}};

inline constexpr std::array<TitleSceneEntry, 23> TITLE_SCENES_SCENE4{{
    {.act = 4, .area = TITLE_AREA_KRF1, .entry = 0},
    {.act = 4, .area = TITLE_AREA_KRH1, .entry = 0},
    {.act = 4, .area = TITLE_AREA_KWM1, .entry = 0},
    {.act = 4, .area = TITLE_AREA_KRM3, .entry = 0},
    {.act = 4, .area = TITLE_AREA_KWW1, .entry = 0},
    {.act = 4, .area = AREA_KWWQ, .entry = 0},
    {.act = 4, .area = TITLE_AREA_KWQA, .entry = 0},
    {.act = 4, .area = TITLE_AREA_KWA1, .entry = 0},
    {.act = 4, .area = TITLE_AREA_KWQB, .entry = 0},
    {.act = 4, .area = TITLE_AREA_KWA2, .entry = 0},
    {.act = 4, .area = TITLE_AREA_KWQC, .entry = 0},
    {.act = 4, .area = TITLE_AREA_KRC1, .entry = 0},
    {.act = 4, .area = TITLE_AREA_KMZ1, .entry = 0},
    {.act = 4, .area = AREA_KRK2, .entry = 0},
    {.act = 4, .area = TITLE_AREA_KMZ2, .entry = 0},
    {.act = 4, .area = AREA_KRK4, .entry = 0},
    {.act = 4, .area = TITLE_AREA_KMZ3, .entry = 0},
    {.act = 4, .area = TITLE_AREA_KRK6, .entry = 0},
    {.act = 4, .area = TITLE_AREA_KMZ4, .entry = 0},
    {.act = 4, .area = TITLE_AREA_KSH1, .entry = 0},
    {.act = 4, .area = TITLE_AREA_KWW4, .entry = 0},
    {.act = 4, .area = AREA_KES1, .entry = 0},
    {.act = 255, .area = shen_name{}, .entry = 0},
}};

inline constexpr std::array<TitleSceneEntry, 18> TITLE_SCENES_EVENT1{{
    {.act = 1, .area = TITLE_AREA_ANAN, .entry = 0},
    {.act = 1, .area = TITLE_AREA_ARBT, .entry = 0},
    {.act = 1, .area = TITLE_AREA_ARMW, .entry = 1},
    {.act = 1, .area = TITLE_AREA_BGSM, .entry = 0},
    {.act = 1, .area = TITLE_AREA_BOSY, .entry = 0},
    {.act = 1, .area = TITLE_AREA_CWON, .entry = 1},
    {.act = 1, .area = TITLE_AREA_DABO, .entry = 0},
    {.act = 1, .area = TITLE_AREA_DMAS, .entry = 4},
    {.act = 1, .area = TITLE_AREA_DUCK, .entry = 0},
    {.act = 1, .area = AREA_JIAG, .entry = 0},
    {.act = 1, .area = TITLE_AREA_KNMN, .entry = 99},
    {.act = 1, .area = TITLE_AREA_SLOT, .entry = 0},
    {.act = 1, .area = TITLE_AREA_UEDE, .entry = 0},
    {.act = 1, .area = TITLE_AREA_WBSS, .entry = 0},
    {.act = 1, .area = TITLE_AREA_WKKT, .entry = 99},
    {.act = 1, .area = AREA_WSSF, .entry = 0},
    {.act = 1, .area = TITLE_AREA_WTKA, .entry = 0},
    {.act = 255, .area = shen_name{}, .entry = 0},
}};

inline constexpr std::array<TitleSceneEntry, 15> TITLE_SCENES_EVENT2{{
    {.act = 2, .area = AREA_AHQB, .entry = 0},
    {.act = 2, .area = TITLE_AREA_ARBT, .entry = 0},
    {.act = 2, .area = TITLE_AREA_ARKQ, .entry = 0},
    {.act = 2, .area = AREA_KEW0, .entry = 0},
    {.act = 2, .area = TITLE_AREA_KNW0, .entry = 0},
    {.act = 2, .area = TITLE_AREA_MFW0, .entry = 0},
    {.act = 2, .area = TITLE_AREA_ABLB, .entry = 0},
    {.act = 2, .area = TITLE_AREA_TOFB, .entry = 0},
    {.act = 2, .area = TITLE_AREA_UEDE, .entry = 0},
    {.act = 2, .area = TITLE_AREA_VSYB, .entry = 0},
    {.act = 2, .area = AREA_WRSQ, .entry = 0},
    {.act = 2, .area = AREA_WTKQ, .entry = 0},
    {.act = 2, .area = TITLE_AREA_YTQB, .entry = 0},
    {.act = 2, .area = AREA_YMQB, .entry = 0},
    {.act = 255, .area = shen_name{}, .entry = 0},
}};

inline constexpr std::array<TitleSceneEntry, 12> TITLE_SCENES_EVENT3{{
    {.act = 3, .area = TITLE_AREA_Q7QB, .entry = 0},
    {.act = 3, .area = TITLE_AREA_QES1, .entry = 0},
    {.act = 3, .area = AREA_QGBT, .entry = 0},
    {.act = 3, .area = AREA_QJBT, .entry = 0},
    {.act = 3, .area = TITLE_AREA_QKBT, .entry = 0},
    {.act = 3, .area = TITLE_AREA_QLBT, .entry = 0},
    {.act = 3, .area = AREA_QSFA, .entry = 0},
    {.act = 3, .area = AREA_QSFB, .entry = 0},
    {.act = 3, .area = AREA_QSFC, .entry = 0},
    {.act = 3, .area = AREA_QSFD, .entry = 0},
    {.act = 3, .area = TITLE_AREA_STEP, .entry = 0},
    {.act = 255, .area = shen_name{}, .entry = 0},
}};

inline constexpr std::array<TitleSceneEntry, 9> TITLE_SCENES_MOVIE{{
    {.act = 1, .area = AREA_OP02, .entry = 0},
    {.act = 1, .area = AREA_0001, .entry = 0},
    {.act = 1, .area = TITLE_AREA_0004, .entry = 0},
    {.act = 1, .area = TITLE_AREA_0005, .entry = 0},
    {.act = 3, .area = TITLE_AREA_0200, .entry = 0},
    {.act = 3, .area = AREA_0278, .entry = 0},
    {.act = 3, .area = AREA_0500, .entry = 0},
    {.act = 4, .area = AREA_0500, .entry = 0},
    {.act = 255, .area = shen_name{}, .entry = 0},
}};

inline constexpr std::array<TitleSceneEntry, 20> TITLE_SCENES_EXTRAS{{
    {.act = 4, .area = TITLE_AREA_OMAK, .entry = 0},
    {.act = 4, .area = TITLE_AREA_WEG0, .entry = 1},
    {.act = 4, .area = TITLE_AREA_Q300, .entry = 98},
    {.act = 4, .area = TITLE_AREA_WEM1, .entry = 1},
    {.act = 4, .area = TITLE_AREA_WRS2, .entry = 1},
    {.act = 4, .area = TITLE_AREA_AKS1, .entry = 1},
    {.act = 4, .area = TITLE_AREA_AKS0, .entry = 1},
    {.act = 4, .area = TITLE_AREA_ARA0, .entry = 1},
    {.act = 4, .area = TITLE_AREA_AB00, .entry = 1},
    {.act = 4, .area = AREA_QSFA, .entry = 1},
    {.act = 4, .area = AREA_QTB1, .entry = 1},
    {.act = 4, .area = TITLE_AREA_Q100, .entry = 1},
    {.act = 4, .area = AREA_QSFD, .entry = 1},
    {.act = 4, .area = AREA_QSFC, .entry = 1},
    {.act = 4, .area = AREA_QSFB, .entry = 1},
    {.act = 4, .area = AREA_QGBT, .entry = 2},
    {.act = 4, .area = AREA_QJBT, .entry = 2},
    {.act = 4, .area = TITLE_AREA_QLBT, .entry = 2},
    {.act = 4, .area = TITLE_AREA_QKBT, .entry = 3},
    {.act = 255, .area = shen_name{}, .entry = 0},
}};

inline constexpr std::array<const TitleSceneEntry *, 10> TITLE_SCENE_TABLES{
    TITLE_SCENES_DEBUG.data(),
    TITLE_SCENES_SCENE1.data(),
    TITLE_SCENES_SCENE2.data(),
    TITLE_SCENES_SCENE3.data(),
    TITLE_SCENES_SCENE4.data(),
    TITLE_SCENES_EVENT1.data(),
    TITLE_SCENES_EVENT2.data(),
    TITLE_SCENES_EVENT3.data(),
    TITLE_SCENES_MOVIE.data(),
    TITLE_SCENES_EXTRAS.data(),
};

} // namespace shenmue::s2
