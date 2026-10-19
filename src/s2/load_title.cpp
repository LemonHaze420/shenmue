#include "s2/load_title.hpp"
#include "s2/area_jump.hpp"

#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/map_info.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/renderer_initialization.hpp"
#include "s2/system_modules.hpp"
#include "s2/ui.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/string_utility.hpp"

#include <array>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <span>

namespace shenmue::s2 {
namespace {


constexpr std::uint32_t Tag(const char (&text)[5]) {
    return static_cast<std::uint32_t>(static_cast<unsigned char>(text[0])) << 24U | static_cast<std::uint32_t>(static_cast<unsigned char>(text[1])) << 16U |
        static_cast<std::uint32_t>(static_cast<unsigned char>(text[2])) << 8U | static_cast<unsigned char>(text[3]);
}

// g_stLoadTitleTable (area plus optional entry digits to title label) and
// g_SCENMapRegions (sub-area rectangles, circles and height bands).
constexpr std::array<LoadTitleEntry, 297> loadTitleEntries{{
    {"1000", "MV"},
    {"2000", "RC"},
    {"A000", nullptr},
    {"AB00", nullptr},
    {"ABB0", nullptr},
    {"ABR0", nullptr},
    {"ABLB", "AR00"},
    {"0055", "AB00"},
    {"AK00", nullptr},
    {"AKY0", nullptr},
    {"AKS0", nullptr},
    {"AKA3", "AKA4"},
    {"AKA4", nullptr},
    {"AR00", nullptr},
    {"ARA0", "ARA0"},
    {"ARC0", nullptr},
    {"ARCB", nullptr},
    {"ARSF", nullptr},
    {"ARK0", nullptr},
    {"W000", nullptr},
    {"WB00", nullptr},
    {"WB01", nullptr},
    {"WE00", nullptr},
    {"WK00", nullptr},
    {"WR00", nullptr},
    {"WS00", nullptr},
    {"WSG1", nullptr},
    {"WT00", nullptr},
    {"ARXB", nullptr},
    {"WKA0", nullptr},
    {"WTA0", nullptr},
    {"030A", "ARXB"},
    {"SM00", nullptr},
    {"SMMK", nullptr},
    {"AR004", "AR00"},
    {"WS001", "WS00"},
    {"WN00", nullptr},
    {"AR01", "AR00"},
    {"AR010", "AR004"},
    {"CB00", "ARCB"},
    {"0034", "WB01"},
    {"KRF1", nullptr},
    {"KRH1", nullptr},
    {"KRZ1", nullptr},
    {"KWM1", nullptr},
    {"KRS2", nullptr},
    {"KRM1", nullptr},
    {"KRM2", nullptr},
    {"KRM3", nullptr},
    {"KWW1", nullptr},
    {"KWW5", nullptr},
    {"KRC1", nullptr},
    {"KRC2", nullptr},
    {"KWW2", nullptr},
    {"KRK2", nullptr},
    {"KWW6", nullptr},
    {"KSH1", "KSHJ"},
    {"KSH3", nullptr},
    {"KSH4", nullptr},
    {"KWW4", nullptr},
    {"KES1", nullptr},
    {"TBCN", "TSGO"},
    {"AR03", nullptr},
    {"Q100", nullptr},
    {"Q200", nullptr},
    {"QA00", "QA10"},
    {"QB00", nullptr},
    {"QBAA", nullptr},
    {"QC00", nullptr},
    {"QD10", "QD00"},
    {"QE00", nullptr},
    {"QF00", "QF10"},
    {"QFTS", "QF10"},
    {"QFHG", "QF10"},
    {"QF01", "QF10"},
    {"QF02", "QF10"},
    {"QF06", "QF10"},
    {"QR00", nullptr},
    {"0297", "QR00"},
    {"AR02", "AR00"},
    {"QJBT", nullptr},
    {"QKBT", "QF11"},
    {"QE001", nullptr},
    {"QE002", nullptr},
    {"QLBT", nullptr},
    {"QF001", nullptr},
    {"QF002", nullptr},
    {"QGBT1", nullptr},
    {"Q300", nullptr},
    {"AKB0", nullptr},
    {"ARM0", nullptr},
    {"ART0", nullptr},
    {"ARZ0", nullptr},
    {"WSC1", nullptr},
    {"WSP0", nullptr},
    {"WSS0", nullptr},
    {"WSY0", nullptr},
    {"WSUR", nullptr},
    {"WSS1", nullptr},
    {"WSS2", nullptr},
    {"WSS3", nullptr},
    {"WSS4", nullptr},
    {"WSS5", nullptr},
    {"WKP0", nullptr},
    {"WEA0", nullptr},
    {"WEA1", nullptr},
    {"WECF", nullptr},
    {"WEG0", nullptr},
    {"WEG1", "WEG0"},
    {"WEG2", "WEG0"},
    {"WEG3", "WEG0"},
    {"WEG4", "WEG0"},
    {"WEG5", "WEG0"},
    {"WEH0", nullptr},
    {"WES1", nullptr},
    {"WEC0", nullptr},
    {"WNC0", nullptr},
    {"WND0", nullptr},
    {"WNDA", nullptr},
    {"WNXE", nullptr},
    {"WNXF", nullptr},
    {"WNXG", nullptr},
    {"WNXH", nullptr},
    {"WEIC", nullptr},
    {"WEM1", nullptr},
    {"WNN1", nullptr},
    {"WNS4", nullptr},
    {"TOFB", "WNS4"},
    {"WNS5", nullptr},
    {"WNT0", nullptr},
    {"WNUR", nullptr},
    {"WNX7", nullptr},
    {"WNX8", nullptr},
    {"WNX9", nullptr},
    {"WNXJ", nullptr},
    {"WNXL", nullptr},
    {"WNXM", nullptr},
    {"WRP0", nullptr},
    {"WRS1", nullptr},
    {"WRS2", nullptr},
    {"WTK0", nullptr},
    {"WTKB", nullptr},
    {"WTP0", nullptr},
    {"WTP1", nullptr},
    {"WTP2", nullptr},
    {"WTP3", nullptr},
    {"WTP4", nullptr},
    {"WTR0", nullptr},
    {"WTS3", nullptr},
    {"WTTB", nullptr},
    {"WTY0", nullptr},
    {"ZZZZ", nullptr},
    {"WTK1", nullptr},
    {"WTK2", nullptr},
    {"WTX2", nullptr},
    {"WTX3", nullptr},
    {"WTX4", nullptr},
    {"WTXA", nullptr},
    {"WTXB", nullptr},
    {"0004", "AR01"},
    {"WTE0", nullptr},
    {"AKS1", nullptr},
    {"AR0186", "ARK0"},
    {"QAE104", nullptr},
    {"QAE105", nullptr},
    {"QAE106", nullptr},
    {"QAE110", nullptr},
    {"QAE154", nullptr},
    {"QAW101", nullptr},
    {"QAW102", nullptr},
    {"QAW110", nullptr},
    {"QE0101", nullptr},
    {"QE0102", nullptr},
    {"QRR0", nullptr},
    {"QUG0", "QUZO"},
    {"QF12", "QF11"},
    {"QF39", "QF11"},
    {"QF40", "QF11"},
    {"QFRR", "QF11"},
    {"KRK3", nullptr},
    {"KRK4", nullptr},
    {"KRK5", nullptr},
    {"KRK5", nullptr},
    {"KRK6", nullptr},
    {"KRK7", nullptr},
    {"QSFA", nullptr},
    {"QSFB", nullptr},
    {"QSFC", nullptr},
    {"QSFD", nullptr},
    {"QTB1", nullptr},
    {"QTB2", nullptr},
    {"QTB3", nullptr},
    {"WTSH", "WTA080"},
    {"QA10", nullptr},
    {"QA14", nullptr},
    {"QA12", nullptr},
    {"QA13", nullptr},
    {"QE10", nullptr},
    {"QE11", nullptr},
    {"QE12", nullptr},
    {"QA15", nullptr},
    {"QC00", nullptr},
    {"QC01", nullptr},
    {"QC06", nullptr},
    {"QC10", "QC00"},
    {"QD00", nullptr},
    {"QD01", nullptr},
    {"QA22", nullptr},
    {"QAAG", nullptr},
    {"QAAJ", nullptr},
    {"QAEN", nullptr},
    {"QAFJ", nullptr},
    {"QAFK", nullptr},
    {"QCAE", nullptr},
    {"QCCE", nullptr},
    {"QCEF", nullptr},
    {"QDKJ", nullptr},
    {"QEDJ", nullptr},
    {"QEEC", nullptr},
    {"QEHH", nullptr},
    {"QEHI", nullptr},
    {"QRC0", nullptr},
    {"FHL1", nullptr},
    {"NDAY", nullptr},
    {"SDAY", nullptr},
    {"1DAY", nullptr},
    {"2DAY", nullptr},
    {"3DAY", nullptr},
    {"4DAY", nullptr},
    {"2KAM", nullptr},
    {"3KAM", nullptr},
    {"4KAM", nullptr},
    {"QD20", nullptr},
    {"QF10", nullptr},
    {"QF11", nullptr},
    {"QF12", "QF11"},
    {"QAAA", nullptr},
    {"KWWW", nullptr},
    {"KWA1", nullptr},
    {"KWQ2", nullptr},
    {"KWA2", nullptr},
    {"KWQ3", nullptr},
    {"KMZ1", nullptr},
    {"KMZ2", nullptr},
    {"KMZ3", nullptr},
    {"KMZ4", nullptr},
    {"WTA099", "WTA080"},
    {"TMOV", "TIME02"},
    {"QAW002", nullptr},
    {"QE53", nullptr},
    {"QTB4", nullptr},
    {"0221", "QRR0"},
    {"0225", "QRR0"},
    {"0240", "QRR0"},
    {"0252", "QRR0"},
    {"0276", "QRR0"},
    {"0278", "WB00"},
    {"0308", "QRR0"},
    {"GOV1", "GAME01"},
    {"GOV2", "GAME02"},
    {"RTR1", "RETR01"},
    {"RTR2", "RETR02"},
    {"TOV1", "TIME01"},
    {"0504", "KRM1"},
    {"0505", "KRM2"},
    {"0250", "3DAY"},
    {"0523", "KRM3"},
    {"WB0033", "NDAY"},
    {"AKT0", nullptr},
    {"AKT1", nullptr},
    {"AKT2", nullptr},
    {"AKT3", nullptr},
    {"ARP0", nullptr},
    {"WKP1", nullptr},
    {"WER0", nullptr},
    {"WESM", nullptr},
    {"WET0", nullptr},
    {"WNXK", nullptr},
    {"WNN0", nullptr},
    {"WBS2", nullptr},
    {"WB205", "WB00B"},
    {"WB1040", "WB00A"},
    {"WB10", "WB00B"},
    {"WB20", "WB00A"},
    {"ARKQ", "ABR0"},
    {"GABT", "QE001"},
    {"0362", "WTA080"},
    {"KWQA", "KWA1"},
    {"WSY1", nullptr},
    {"KSHJ", nullptr},
    {"QCHF", nullptr},
    {"QCJE", nullptr},
    {"QCLG", nullptr},
    {"WBBK", "WB00B"},
    {"0200", "QA00"},
    {"00391", "WSY0"},
    {"00395", "WTA080"},
}};
constexpr std::array<MapRegionEntry, 106> mapRegions{{
    {"AB10", "AB00", {254.10000610351562f, 539.5f, 277.5f, 545.7999877929688f}},
    {"AB20", "AB00", {116.0f, 547.0f, 122.80000305175781f, 557.7999877929688f}},
    {"AR12", "AR02", {199.8000030517578f, 303.0f, 210.39999389648438f, 322.1000061035156f}},
    {"AR13", "AR02", {226.8000030517578f, 344.8999938964844f, 236.8000030517578f, 353.0f}},
    {"AR14", "AR02", {244.0f, 341.79998779296875f, 260.6000061035156f, 367.29998779296875f}},
    {"ARK0", "AR01", {173.0f, 302.5f, 187.0f, 313.0f}},
    {"ARX0", "AR02", {188.5f, 376.6000061035156f, 231.1999969482422f, 434.3999938964844f}},
    {"ARX6", "AR02", {255.60000610351562f, 392.5f, 264.5f, 405.8999938964844f}},
    {"ARX7", "AR02", {243.60000610351562f, 423.20001220703125f, 251.8000030517578f, 428.6000061035156f}},
    {"ARXA", "AR03", {299.79998779296875f, 472.6000061035156f, 308.5f, 483.5f}},
    {"AR30", "AR03", {261.3999938964844f, 471.6000061035156f, 273.70001220703125f, 475.5f}},
    {"AR30", "AB00", {261.3999938964844f, 471.6000061035156f, 273.70001220703125f, 475.5f}},
    {"AR30", "AR03", {257.0f, 460.3999938964844f, 265.5f, 471.6000061035156f}},
    {"AR30", "AB00", {257.0f, 460.3999938964844f, 265.5f, 471.6000061035156f}},
    {"ARKL", "AR03", {380.70001220703125f, 424.3999938964844f, 406.0f, 453.6000061035156f}},
    {"WB10", "WB00", {326.70001220703125f, 227.0f, 344.20001220703125f, 277.70001220703125f}},
    {"WB10", "WB01", {326.70001220703125f, 227.0f, 344.20001220703125f, 277.70001220703125f}},
    {"WB20", "WB00", {327.3999938964844f, 230.0f, 353.0f, 276.6000061035156f}},
    {"WB20", "WN00", {280.29998779296875f, 277.70001220703125f, 346.0f, 289.79998779296875f}},
    {"WB20", "WT00", {280.29998779296875f, 277.70001220703125f, 346.0f, 289.79998779296875f}},
    {"WNN6", "WN00", {202.6999969482422f, 203.89999389648438f, 4.5f, 999.9000244140625f}},
    {"WNN7", "WN00", {187.5f, 220.3000030517578f, 196.5f, 225.5f}},
    {"WNN9", "WN00", {223.10000610351562f, 206.5f, 3.0999999046325684f, 999.9000244140625f}},
    {"WNN9", "WN00", {202.6999969482422f, 203.89999389648438f, 4.5f, 999.9000244140625f}},
    {"WNNA", "WN00", {233.60000610351562f, 200.0f, 242.1999969482422f, 209.0f}},
    {"WN20", "WN00", {223.1999969482422f, 154.60000610351562f, 232.1999969482422f, 191.0f}},
    {"WN20", "WN00", {232.1999969482422f, 154.60000610351562f, 244.10000610351562f, 162.1999969482422f}},
    {"WN20", "WN00", {237.0f, 138.5f, 244.10000610351562f, 154.60000610351562f}},
    {"WR10", "WR00", {180.60000610351562f, 183.60000610351562f, 185.60000610351562f, 194.39999389648438f}},
    {"WSC0", "WS00", {56.79999923706055f, 180.0f, 63.20000076293945f, 186.39999389648438f}},
    {"WSC1", "WS00", {27.299999237060547f, 186.89999389648438f, 16.0f, 999.9000244140625f}},
    {"WSKB", "WS00", {99.0f, 186.0f, 126.0f, 222.0f}},
    {"WSKB", "WS00", {98.0f, 202.39999389648438f, 101.0f, 206.5f}},
    {"WTX0", "WT00", {231.5f, 337.5f, 261.79998779296875f, 356.79998779296875f}},
    {"WN10", "WB00", {262.0f, 250.0f, 272.0f, 276.5f}},
    {"WN10", "WT00", {262.0f, 250.0f, 272.0f, 276.5f}},
    {"WT10", "WB00", {260.0f, 276.3999938964844f, 285.0f, 290.0f}},
    {"WT10", "WN00", {260.0f, 276.3999938964844f, 285.0f, 290.0f}},
    {"WEWE", "WE00", {53.0f, 93.0f, 130.5f, 133.0f}},
    {"WEEA", "WE00", {130.5f, 73.0f, 244.5f, 133.0f}},
    {"WEG3", "WEG0", {107.0f, 200.0f, 999.9000244140625f, 999.9000244140625f}},
    {"WEG2", "WEG0", {103.9000015258789f, 200.0f, 999.9000244140625f, 999.9000244140625f}},
    {"QTBB", "Q100", {219.5f, 204.0f, 253.0f, 221.0f}},
    {"Q130", "Q100", {172.6999969482422f, 187.8000030517578f, 177.6999969482422f, 194.3000030517578f}},
    {"Q140", "Q100", {183.1999969482422f, 78.5999984741211f, 198.89999389648438f, 108.0999984741211f}},
    {"Q110", "Q100", {168.1999969482422f, 73.19999694824219f, 259.0f, 154.60000610351562f}},
    {"Q120", "Q100", {168.1999969482422f, 154.60000610351562f, 259.0f, 222.5f}},
    {"Q210", "Q200", {216.89999389648438f, 61.70000076293945f, 299.0f, 116.80000305175781f}},
    {"Q220", "Q200", {251.10000610351562f, 116.9000015258789f, 293.0f, 156.0f}},
    {"QTBB", "Q300", {219.5f, 204.0f, 253.0f, 221.0f}},
    {"QAAQ", "Q300", {245.8000030517578f, 194.3000030517578f, 256.3999938964844f, 201.60000610351562f}},
    {"QAAR", "Q300", {256.29998779296875f, 192.1999969482422f, 267.0f, 201.5f}},
    {"Q330", "Q300", {258.20001220703125f, 158.3000030517578f, 265.0f, 165.60000610351562f}},
    {"Q320", "Q300", {210.0f, 163.8000030517578f, 256.5f, 208.89999389648438f}},
    {"Q310", "Q300", {253.5f, 142.10000610351562f, 296.70001220703125f, 208.89999389648438f}},
    {"QA12", "QA00", {245.39999389648438f, 194.5f, 274.1000061035156f, 207.1999969482422f}},
    {"QA12", "QAE1", {245.39999389648438f, 194.5f, 274.1000061035156f, 207.1999969482422f}},
    {"QA12", "QAE6", {245.39999389648438f, 194.5f, 274.1000061035156f, 207.1999969482422f}},
    {"QA12", "QA11", {245.39999389648438f, 194.5f, 274.1000061035156f, 207.1999969482422f}},
    {"QA12", "QA00", {256.29998779296875f, 205.1999969482422f, 287.29998779296875f, 218.0f}},
    {"QA12", "QAE1", {256.29998779296875f, 205.1999969482422f, 287.29998779296875f, 218.0f}},
    {"QA12", "QAE6", {256.29998779296875f, 205.1999969482422f, 287.29998779296875f, 218.0f}},
    {"QA12", "QA11", {256.29998779296875f, 205.1999969482422f, 287.29998779296875f, 218.0f}},
    {"QA13", "QA00", {215.0f, 195.1999969482422f, 235.0f, 216.0f}},
    {"QA13", "QAE1", {215.0f, 195.1999969482422f, 235.0f, 216.0f}},
    {"QA13", "QAE6", {215.0f, 195.1999969482422f, 235.0f, 216.0f}},
    {"QA13", "QA11", {215.0f, 195.1999969482422f, 235.0f, 216.0f}},
    {"QA13", "QA00", {213.5f, 207.1999969482422f, 256.0f, 230.39999389648438f}},
    {"QA13", "QAE1", {213.5f, 207.1999969482422f, 256.0f, 230.39999389648438f}},
    {"QA13", "QAE6", {213.5f, 207.1999969482422f, 256.0f, 230.39999389648438f}},
    {"QA13", "QA11", {213.5f, 207.1999969482422f, 256.0f, 230.39999389648438f}},
    {"QA10", "QA00", {274.3999938964844f, 123.0f, 303.5f, 167.89999389648438f}},
    {"QA10", "QAW1", {274.3999938964844f, 123.0f, 303.5f, 167.89999389648438f}},
    {"QA10", "QAW6", {274.3999938964844f, 123.0f, 303.5f, 167.89999389648438f}},
    {"QA14", "QA00", {283.3999938964844f, 168.10000610351562f, 303.79998779296875f, 205.39999389648438f}},
    {"QA14", "QAW1", {283.3999938964844f, 168.10000610351562f, 303.79998779296875f, 205.39999389648438f}},
    {"QA14", "QAW6", {283.3999938964844f, 168.10000610351562f, 303.79998779296875f, 205.39999389648438f}},
    {"QA14", "QA22", {283.3999938964844f, 168.10000610351562f, 303.79998779296875f, 205.39999389648438f}},
    {"QB10", "QB00", {204.89999389648438f, 191.60000610351562f, 1.7999999523162842f, 999.9000244140625f}},
    {"QB11", "QB00", {208.6999969482422f, 195.5f, 1.7000000476837158f, 999.9000244140625f}},
    {"QCFE", "QC06", {166.5f, 147.8000030517578f, 180.8000030517578f, 177.39999389648438f}},
    {"QD20", "QD00", {190.0f, 65.5999984741211f, 220.5f, 90.5999984741211f}},
    {"QD20", "QD01", {190.0f, 65.5999984741211f, 220.5f, 90.5999984741211f}},
    {"QEFO", "QE06", {278.29998779296875f, 61.0f, 281.79998779296875f, 66.5f}},
    {"QEFP", "QE06", {283.5f, 59.5f, 288.79998779296875f, 66.5f}},
    {"QEFQ", "QE06", {289.0f, 61.29999923706055f, 292.70001220703125f, 66.5f}},
    {"QEFR", "QE06", {294.3999938964844f, 59.5f, 299.6000061035156f, 66.5f}},
    {"QEFS", "QE06", {299.8999938964844f, 61.099998474121094f, 303.5f, 66.5f}},
    {"QE10", "QE00", {234.8000030517578f, 79.0999984741211f, 258.0f, 97.19999694824219f}},
    {"QE10", "QE01", {234.8000030517578f, 79.0999984741211f, 258.0f, 97.19999694824219f}},
    {"QE10", "QE06", {234.8000030517578f, 79.0999984741211f, 258.0f, 97.19999694824219f}},
    {"QE10", "QLBT", {234.8000030517578f, 79.0999984741211f, 258.0f, 97.19999694824219f}},
    {"QE10", "QE00", {220.60000610351562f, 72.0f, 234.8000030517578f, 83.0f}},
    {"QE10", "QE01", {220.60000610351562f, 72.0f, 234.8000030517578f, 83.0f}},
    {"QE10", "QE06", {220.60000610351562f, 72.0f, 234.8000030517578f, 83.0f}},
    {"QE10", "QLBT", {220.60000610351562f, 72.0f, 234.8000030517578f, 83.0f}},
    {"QE11", "QE00", {238.8000030517578f, 57.599998474121094f, 267.1000061035156f, 79.0999984741211f}},
    {"QE11", "QE01", {238.8000030517578f, 57.599998474121094f, 267.1000061035156f, 79.0999984741211f}},
    {"QE11", "QE06", {238.8000030517578f, 57.599998474121094f, 267.1000061035156f, 79.0999984741211f}},
    {"QE12", "QE00", {267.1000061035156f, 58.0f, 309.20001220703125f, 110.0f}},
    {"QE12", "QE01", {267.1000061035156f, 58.0f, 309.20001220703125f, 110.0f}},
    {"QE12", "QE06", {267.1000061035156f, 58.0f, 309.20001220703125f, 110.0f}},
    {"QA15", "QA00", {256.29998779296875f, 216.5f, 283.29998779296875f, 244.0f}},
    {"QA15", "QJBT", {256.29998779296875f, 216.5f, 283.29998779296875f, 244.0f}},
    {"QF12", "QUG0", {246.60000610351562f, 116.5f, 254.5f, 129.10000610351562f}},
    {"KESS", "KES1", {60.0f, -18.0f, 142.5f, 39.0f}},
}};

// g_SCENMapRegionFallbacks: fallback area name for maps with no matching region.
constexpr std::array<std::array<const char *, 2>, 2> mapRegionFallbacks{{{"AR15", "AR02"}, {"WEG1", "WEG0"}}};
// g_stFloorTitleTable: floor/sub-title sprites keyed by area and entry.
constexpr std::array<FloorTitleEntry, 5> floorTitles{{
    {"WS00", 1, "L_WS001.GZ"},
    {"AR03", 4, "L_AR004.GZ"},
    {"AR01", 0, "L_AR01.GZ"},
    {"A000", 0, "L_AR01.GZ"},
    {"Q100", 1, "L_QA00.GZ"},
}};
// g_stQuestTitleTable: battle-quest title pairs used under internal flag 1015.
constexpr std::array<LoadTitleEntry, 4> questTitles{{{"QJBT", "MG01"}, {"QGBT", "MG02"}, {"QLBT", "MG03"}, {"QKBT", "MG04"}}};

shen_name temporaryAreaJumpAuthor{};
LoadTitleState loadTitle{nullptr, 0, 0, 0, 0, nullptr, 0, 0, -1, -1, 0, 0, -1, {}, 0, {}, 0};
constexpr std::array<std::uint16_t, 8> loadingSpriteRectangles{98, 99, 28, 28, 1, 1, 2, 2};
constexpr std::array<std::array<std::int32_t, 2>, 4> loadingSpritePositions{{{395, 431}, {395, 431}, {44, 438}, {395, 431}}};



void LoadTitleSpriteNullsub(std::uint32_t colour, std::int32_t texture) {
    static_cast<void>(colour);
    static_cast<void>(texture);
}

const MapInfoChunk *CurrentMapInfo() {
    const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return control ? static_cast<const MapInfoChunk *>(control->mapInfo) : nullptr;
}

bool SameName(std::uint32_t name, const char *text) {
    return text && std::memcmp(&name, text, 4) == 0;
}

std::uint32_t NameOf(const char *text) {
    std::uint32_t name{};
    std::memcpy(&name, text, 4);
    return name;
}



std::uint32_t FindMapRegionName(const Vector3 &position) {
    for (const MapRegionEntry &region : mapRegions) {
        if (!SameName(g_currentMap.name, region.map))
            continue;
        const std::array<float, 4> &bounds = region.bounds;
        bool inside = false;
        if (bounds[3] >= 999.0f) {
            if (bounds[2] >= 999.0f) {
                inside = position.y >= bounds[0] && bounds[1] >= position.y;
            } else {
                const float dz = position.z - bounds[1];
                const float dx = position.x - bounds[0];
                inside = bounds[2] > std::sqrt(dz * dz + dx * dx);
            }
        } else {
            inside = position.x >= bounds[0] && bounds[2] >= position.x && position.z >= bounds[1] && bounds[3] >= position.z;
        }
        if (inside)
            return NameOf(region.area);
    }
    for (const auto &fallback : mapRegionFallbacks) {
        if (SameName(g_currentMap.name, fallback[1]))
            return NameOf(fallback[0]);
    }
    return 0;
}



std::int32_t ResolveLoadTitleStream(std::uint32_t &subarea) {
    Vector3 position{};
    std::int32_t yaw{};
    const std::int32_t stream = ResolveQronEntryLocation(position, yaw);
    subarea = FindMapRegionName(position);
    return stream;
}

std::int32_t ParseEntryDigits(const char *key) {
    std::int32_t value = 0;
    for (const char *cursor = key + 4; *cursor >= '0'; ++cursor) {
        if (*cursor > '9')
            break;
        value = *cursor + 2 * (5 * value - 24);
    }
    return value;
}

const LoadTitleEntry *SearchLoadTitle(std::uint32_t name, std::int32_t entry, std::int32_t &fallback) {
    for (std::size_t index = 0; index < loadTitleEntries.size(); ++index) {
        const LoadTitleEntry &row = loadTitleEntries[index];
        if (std::strncmp(reinterpret_cast<const char *>(&name), row.key, 4) != 0)
            continue;
        if (std::strlen(row.key) < 5)
            fallback = static_cast<std::int32_t>(index);
        else if (ParseEntryDigits(row.key) == entry)
            return &row;
    }
    return nullptr;
}


const LoadTitleEntry *FindLoadTitle(std::uint32_t area, std::uint32_t subarea, std::int32_t entry) {
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1015)) {
        const std::uint32_t flags = (loadTitle.flags & ~0xcU) | 4U;
        loadTitle.flags = flags;
        for (const LoadTitleEntry &quest : questTitles) {
            if (SameName(g_currentMap.name, quest.key)) {
                loadTitle.flags = flags & ~0x20U;
                return &quest;
            }
        }
    }
    std::int32_t fallback = -1;
    if (subarea != Tag("9999") && area != Tag("10BW") && area != Tag("22AQ")) {
        if (const LoadTitleEntry *const row = SearchLoadTitle(subarea, entry, fallback))
            return row;
        if (fallback >= 0)
            return &loadTitleEntries[static_cast<std::size_t>(fallback)];
    }
    if (const LoadTitleEntry *const row = SearchLoadTitle(area, entry, fallback))
        return row;
    return fallback >= 0 ? &loadTitleEntries[static_cast<std::size_t>(fallback)] : nullptr;
}

std::uint16_t TexnWidth(const void *texn) {
    std::uint16_t width{};
    std::memcpy(&width, static_cast<const std::byte *>(texn) + 40, sizeof(width));
    return width;
}


void LoadFloorTitle(std::uint32_t area, std::int32_t entry) {
    loadTitle.floorTexture = -1;
    for (const FloorTitleEntry &floor : floorTitles) {
        if (!SameName(area, floor.area) || floor.entry != entry)
            continue;
        void *allocation = nullptr;
        std::size_t size = 0;
        if (LoadLegacySprite("sprite", floor.sprite, SHEN_NAME(LDSB), allocation, size)) {
            loadTitle.floorTexture = static_cast<std::int32_t>(ReadTexnTexture(allocation));
            loadTitle.floorWidth = TexnWidth(allocation);
            HLib::FreeTaskData(allocation);
            return;
        }
    }
}

void ReleaseLoadTitleTexture(std::int32_t &texture) {
    if (texture != -1)
        ReleaseNamedTexture(texture);
    texture = -1;
}


void DrawFloorTitle() {
    if (loadTitle.floorTexture >= 0)
        LoadTitleSpriteNullsub(static_cast<std::uint32_t>(loadTitle.alpha) << 24U | 0xffffffU, loadTitle.floorTexture);
}


void DrawFloorNumber(std::int32_t value, bool special) {
    const std::uint32_t colour = static_cast<std::uint32_t>(loadTitle.alpha) << 24U | 0xffffffU;
    if (special) {
        LoadTitleSpriteNullsub(colour, loadTitle.digitTextures[12]);
        return;
    }
    const std::int32_t magnitude = std::abs(value > 0 ? value : value - 1);
    const std::int32_t tens = magnitude / 10;
    if (tens)
        LoadTitleSpriteNullsub(colour, loadTitle.digitTextures[static_cast<std::size_t>(tens)]);
    LoadTitleSpriteNullsub(colour, loadTitle.digitTextures[static_cast<std::size_t>(magnitude - 10 * tens)]);
    LoadTitleSpriteNullsub(colour, loadTitle.digitTextures[value > 0 ? 11 : 10]);
}


void DrawLoadNumber(std::int32_t value, std::uint8_t mode, std::int32_t digits) {
    constexpr std::array<std::int32_t, 4> powers{1, 10, 100, 1000};
    const std::uint32_t colour = static_cast<std::uint32_t>(loadTitle.alpha) << 24U | 0xffffffU;
    bool started = (mode & 1U) != 0;
    for (std::int32_t index = digits - 1; index >= 0; --index) {
        const std::int32_t digit = value / powers[static_cast<std::size_t>(index)];
        value %= powers[static_cast<std::size_t>(index)];
        if (digit)
            started = true;
        else if (!started && index > 0)
            continue;
        LoadTitleSpriteNullsub(colour, loadTitle.digitTextures[static_cast<std::size_t>(digit)]);
    }
}


void DrawLoadingLabel(std::uint32_t colour) {
    LoadTitleSpriteNullsub(colour | 0xffffffU, loadTitle.labelTexture);
}


void ST_LoadTitleFadeIn_Main(void *) {
    const std::uint32_t colour = static_cast<std::uint32_t>(loadTitle.alpha) << 24U;
    if (loadTitle.fadeMode == 1) {
        SubmitScreenColourOverlay(colour | 0xffffffU);
        return;
    }
    if (loadTitle.fadeMode == 6) {
        SubmitScreenColourOverlay(colour);
        return;
    }
    if (g_bPaused || loadTitle.fadeMode == 8) {
        DrawLoadingLabel(colour);
        return;
    }
    if (loadTitle.titleTexture != -1) {
        LoadTitleSpriteNullsub(colour | 0xffffffU, loadTitle.titleTexture);
        DrawFloorTitle();
        if (loadTitle.flags & 8U) {
            const bool special = (loadTitle.flags & 0x10U) != 0;
            DrawFloorNumber(special ? static_cast<std::int32_t>(loadTitle.flags) : loadTitle.stream, special);
        }
    }
    if ((loadTitle.flags & 4U) == 0) {
        const std::array<std::int8_t, 12> &date = loadTitle.date;
        LoadTitleSpriteNullsub(colour | 0xffffffU, loadTitle.digitTextures[static_cast<std::size_t>(date[DayUnits])]);
        DrawLoadNumber(date[YearUnits] + 10 * (date[YearTens] + 190), 0, 4);
        DrawLoadNumber(date[HourUnits] + 10 * date[HourTens], 10, 2);
        DrawLoadNumber(date[MinuteUnits] + 10 * date[MinuteTens], 3, 2);
        LoadTitleSpriteNullsub(colour | 0xffffffU, loadTitle.digitTextures[static_cast<std::size_t>(loadTitle.meridiemSprite)]);
    }
    if (loadTitle.author != Tag("NCBT") && loadTitle.author != Tag("2VOG"))
        DrawLoadingLabel(colour);
}

void ST_LoadTitleFadeIn_Callback(void *) {
    // SetPPFXEnabled(1): the PPFX bypass flag is middleware state.
    if ((g_loadingFlag & 0x102U) == 0) {
        loadTitle.alpha = static_cast<std::int16_t>(loadTitle.alpha + 8);
        if (loadTitle.alpha > 255)
            loadTitle.alpha = 255;
    }
    HLib::ExecuteCallbackNow(ST_LoadTitleFadeIn_Main, nullptr);
}

void ST_LoadTitleFadeOut_Callback(void *) {
    if (loadTitle.loadingDrawActive)
        RetireLoadingDraw();
    std::int32_t alpha = loadTitle.alpha;
    if ((g_loadingFlag & 0x102U) == 0) {
        alpha = loadTitle.alpha - 8;
        loadTitle.alpha = static_cast<std::int16_t>(alpha);
    }
    if (alpha < 0)
        ReleaseLoadTitleFade();
    else
        HLib::ExecuteCallbackNow(ST_LoadTitleFadeIn_Main, nullptr);
}


// @verified - p
void ST_WaitSubTitle(void *) {
}



// @verified - p
void ST_DrawSubTitle(void *taskData) {
    SubtitleSpriteWork &work = *static_cast<SubtitleSpriteWork *>(taskData);
    work.sprite.colour = static_cast<std::uint32_t>(work.alpha) << 24U | 0xffffffU;
    if (!work.stacked) {
        AdjustScreenAspectRatio(work.sprite.quad, ScreenAlignment::HorizontalStretch, TruncateSpriteCoordinate(work.x), TruncateSpriteCoordinate(work.y), 512,
            512, ScreenMode::Centered);
        LegacySpriteNullsub(work.sprite, static_cast<std::uint32_t>(work.textures[0]), 1.0f);
        return;
    }
    for (std::int32_t index = 0; index < 4; ++index) {
        const float rotation = index == 0 ? work.rotation : 0.0f;
        if (work.textures[index] == -1)
            continue;
        TransformScreenSpace(work.sprite.quad, ScreenAlignment::HorizontalStretch, 320, SUBTITLE_STACK_Y[index], 512, SUBTITLE_STACK_HEIGHT[index], 1.0f, 1.0f,
            TruncateSpriteCoordinate(rotation), ScreenMode::Centered);
        LegacySpriteNullsub(work.sprite, static_cast<std::uint32_t>(work.textures[index]), static_cast<float>(index) + 1.0f);
    }
}

// @verified - p
void ST_FadeInSubTitle_Callback(void *taskData) {
    SubtitleSpriteWork &work = *static_cast<SubtitleSpriteWork *>(taskData);
    work.alpha = static_cast<std::int16_t>(work.alpha + work.alphaStep);
    work.rotation += SUBTITLE_ROTATION_STEP;
    if (work.alpha > 255)
        work.alpha = 255;
    HLib::ExecuteCallbackNow(ST_DrawSubTitle, taskData);
}


// @verified - p
void ST_FadeOutSubTitle_Callback(void *taskData) {
    SubtitleSpriteWork &work = *static_cast<SubtitleSpriteWork *>(taskData);
    work.rotation += SUBTITLE_ROTATION_STEP;
    work.alpha = static_cast<std::int16_t>(work.alpha - work.alphaStep);
    if (work.alpha >= 0) {
        HLib::ExecuteCallbackNow(ST_DrawSubTitle, taskData);
        return;
    }
    work.alpha = 0;
    for (std::int32_t &texture : work.textures) {
        if (texture != -1)
            ReleaseNamedTexture(texture);
        texture = -1;
    }
    HLib::Task *const task = work.task;
    FreeTASK(task);
    HLib::DestroyTaskInternal(task);
    loadTitle.fadeInActive = 0;
}

} // namespace



// @verified - p
HLib::Task *Queue_ST_WaitSubTitle(const char *filename) {
    loadTitle.fadeInActive = 0;
    HLib::Task *const task = HLib::EnqueueTaskWithoutParameter(ST_WaitSubTitle, 9, 0, static_cast<std::int32_t>(SUBTITLE_WAIT_TASK.name));
    SubtitleSpriteWork *const work = static_cast<SubtitleSpriteWork *>(AllocateTASK(task, sizeof(SubtitleSpriteWork)));
    work->task = task;
    work->textures = {-1, -1, -1, -1};
    work->stacked = 0;
    if (CompareTokenIgnoreCase(filename, "SUBTITLE.SPR") == 0)
        work->stacked = 1;
    void *const file = LoadLegacySceneFile("sprite", filename, SUBTITLE_SPRITE_FILE);
    ReadTEXN_Recursive(file, work->textures.data());
    HLib::FreeTaskData(file);
    return task;
}

// @verified - p
std::int64_t ST_FadeInSubTitle(HLib::Task *task, float x, float y, std::int16_t alphaStep) {
    HLib::AssignCallbackForTask(task, ST_FadeInSubTitle_Callback);
    loadTitle.fadeInActive = 1;
    SubtitleSpriteWork &work = *static_cast<SubtitleSpriteWork *>(HLib::GetTaskParameter(task));
    work.x = x;
    work.y = y;
    work.sprite.quad.flags = 0;
    work.alpha = 0;
    work.alphaStep = alphaStep;
    work.rotation = 0.0f;
    work.sprite.colour = 0xffffffffU;
    work.sprite.secondaryColour = 0;
    work.sprite.uv = {0.0f, 0.0f, 1.0f, 1.0f};
    work.sprite.quad.flags |= 0x400U;
    work.sprite.reservedTail = 0;
    return 1;
}

// @verified - p
std::int64_t ST_FadeOutSubTitle(HLib::Task *task) {
    HLib::AssignCallbackForTask(task, ST_FadeOutSubTitle_Callback);
    return 1;
}

void ReleaseLoadTitleFade() {
    loadTitle.alpha = -1;
    ReleaseLoadTitleTexture(loadTitle.floorTexture);
    ReleaseLoadTitleTexture(loadTitle.titleTexture);
    for (std::int32_t index = 0; index < loadTitle.digitCount; ++index)
        ReleaseLoadTitleTexture(loadTitle.digitTextures[static_cast<std::size_t>(index)]);
    loadTitle.digitCount = 0;
    loadTitle.fadeMode = 0;
    loadTitle.flags = 0;
    HLib::Task *const task = loadTitle.titleTask;
    loadTitle.titleTask = nullptr;
    loadTitle.fadeInActive = 0;
    if (task)
        HLib::DestroyTask(task);
}

void InitST_TitleFunc() {
    loadTitle.titleTask = nullptr;
    loadTitle.digitTextures.fill(-1);
    loadTitle.titleTexture = -1;
    loadTitle.floorTexture = -1;
}



void DoLoadStuff(std::uint32_t author, bool nextStream) {
    SetupLoadingScreen(shen_name{0xffffffffU}, shen_name{0xffffffffU}, DateTime{}, nullptr, 0, 0, 0, 0);
    loadTitle.author = author;
    std::uint32_t subarea = 0;
    const std::int32_t stream = nextStream ? static_cast<std::int32_t>(GetQronNextStream()) : ResolveLoadTitleStream(subarea);
    loadTitle.stream = stream;
    std::uint32_t flags = loadTitle.flags & 0xffffffe7U;
    loadTitle.flags = flags;
    const std::uint32_t map = g_currentMap.name;
    bool addFloor = stream != -1000;
    if (author == Tag("STFQ") || map == Tag("STFQ") || author == Tag("TBAG") || map == Tag("TBAG")) {
        loadTitle.stream = 5;
        addFloor = true;
    } else if (author == Tag("GHFQ") || map == Tag("GHFQ")) {
        loadTitle.stream = 2;
        addFloor = true;
    }
    if (addFloor)
        flags = (map == Tag("11AQ") ? loadTitle.flags | 0x10U : loadTitle.flags) | 8U;
    loadTitle.titleTask = nullptr;
    loadTitle.digitCount = 0;
    loadTitle.flags = (flags & 0xffffffd9U) | 0x20U;
    const MapInfoChunk *const mapInfo = CurrentMapInfo();
    if (!mapInfo)
        return;
    loadTitle.loadScene = FindMapInfoChunk("LSCN", mapInfo);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1021))
        loadTitle.flags |= 4U;
    if (loadTitle.fadeMode < 10) {
        if (loadTitle.fadeMode == 6 || loadTitle.fadeMode == 1)
            return;
    } else {
        loadTitle.fadeMode = 0;
    }
    DateTime now{};
    GetCurrentDateTime(now);
    std::array<std::int8_t, 12> &date = loadTitle.date;
    date[YearTens] = static_cast<std::int8_t>(now.year / 10);
    date[YearUnits] = static_cast<std::int8_t>(now.year % 10);
    date[MonthTens] = static_cast<std::int8_t>(now.month / 10);
    date[MonthUnits] = static_cast<std::int8_t>(now.month % 10);
    date[DayTens] = static_cast<std::int8_t>(now.day / 10);
    date[DayUnits] = static_cast<std::int8_t>(now.day % 10);
    std::int32_t hour = now.hour;
    if (now.hour >= 12) {
        loadTitle.meridiemSprite = 21;
        if (now.hour != 12)
            hour = now.hour - 12;
    } else {
        loadTitle.meridiemSprite = 22;
    }
    if (!hour)
        hour = 12;
    date[HourTens] = static_cast<std::int8_t>(hour / 10);
    date[HourUnits] = static_cast<std::int8_t>(hour % 10);
    date[MinuteTens] = static_cast<std::int8_t>(now.minute / 10);
    date[Week] = static_cast<std::int8_t>(now.week);
    date[MinuteUnits] = static_cast<std::int8_t>(now.minute % 10);
    const std::uint32_t area = author ? author : map;
    const std::uint32_t titleSubarea = !author && subarea ? subarea : Tag("9999");
    const LoadTitleEntry *title = nullptr;
    if (loadTitle.flags & 1U) {
        loadTitle.flags |= 4U;
        title = FindLoadTitle(Tag("ZZZZ"), Tag("9999"), 0);
    } else if (g_actNumber && loadTitle.fadeMode != 7) {
        title = FindLoadTitle(area, titleSubarea, g_entryPoint);
    }
    // texGetMotionFlag / texSetMotionFlag save and clear the TXMO texture-motion flag
    // around the sprite loads; the TXMO reader is deferred, so it stays 0.
    if (map >= Tag("0001")) {
        if (map <= Tag("0002") || map == Tag("KAMO") || map == Tag("WVTS"))
            loadTitle.flags |= 4U;
        else if (map == Tag("10FM"))
            loadTitle.fadeMode = 8;
    }
    if (author == Tag("2RTR") || author == Tag("1RTR") || author == Tag("1VOG") || author == Tag("1VOT") || author == Tag("2VOG") || author == Tag("NCBT") ||
        author == Tag("VOMT")) {
        loadTitle.flags = (loadTitle.flags & 0xfffffff3U) | 4U;
    }
    LoadFloorTitle(area, g_entryPoint);
    const char *label = nullptr;
    if (title) {
        label = title->alias ? title->alias : title->key;
        std::array<char, 32> sprite{};
        std::snprintf(sprite.data(), sprite.size(), "L_%s.GZ", label);
        void *allocation = nullptr;
        std::size_t size = 0;
        if (LoadLegacySprite("sprite", sprite.data(), SHEN_NAME(LDAR), allocation, size)) {
            loadTitle.titleTexture = static_cast<std::int32_t>(ReadTexnTexture(allocation));
            loadTitle.titleWidth = TexnWidth(allocation);
            HLib::FreeTaskData(allocation);
        }
    }
    void *allocation = nullptr;
    std::size_t size = 0;
    if (LoadLegacySprite("sprite", "LOADNUM.GZ", SHEN_NAME(LDTD), allocation, size)) {
        loadTitle.digitCount = static_cast<std::int32_t>(ReadTEXN_Recursive(allocation, loadTitle.digitTextures.data()));
        HLib::FreeTaskData(allocation);
    }
    DisableFilmBarAndUpdateFramePacing();
    SetupLoadingScreen(shen_name{area}, shen_name{titleSubarea}, now, label, loadTitle.flags, loadTitle.stream, loadTitle.fadeMode, g_entryPoint);
}

// @verified - p
shen_name GetMapRegionName(const Vector3 &position) {
    return shen_name{FindMapRegionName(position)};
}

// @verified - p
void SetTemporaryAreaJumpAuthor(shen_name author) {
    temporaryAreaJumpAuthor = author;
}

// @verified - p
void ResetTemporaryAreaJumpAuthor() {
    temporaryAreaJumpAuthor = {};
}



void ST_LoadTitleFadeIn() {
    loadTitle.fadeInActive = 1;
    const bool areaJump = HasSavedAreaJumpLocation();
    DoLoadStuff(areaJump ? temporaryAreaJumpAuthor.name : 0, areaJump && temporaryAreaJumpAuthor.name != 0);
    if (areaJump)
        temporaryAreaJumpAuthor = {};
    loadTitle.alpha = 0;
    do {
        ST_LoadTitleFadeIn_Main(nullptr);
        loadTitle.alpha = static_cast<std::int16_t>(loadTitle.alpha + 8);
    } while (loadTitle.alpha <= 255);
}



void ST_LoadTitleFade() {
    loadTitle.fadeInActive = 0;
    if (!loadTitle.digitCount && loadTitle.fadeMode != 6 && loadTitle.fadeMode != 1)
        return;
    if (loadTitle.titleTask) {
        WaitForNextRenderFrame(nullptr, 0);
        SetD3TRenderSettings(1);
        HLib::AssignCallbackForTask(loadTitle.titleTask, ST_LoadTitleFadeOut_Callback);
        return;
    }
    HLib::TaskCallback callback = ST_LoadTitleFadeIn_Callback;
    if (!loadTitle.loadScene) {
        WaitForNextRenderFrame(nullptr, 0);
        SetD3TRenderSettings(1);
        callback = ST_LoadTitleFadeOut_Callback;
    }
    loadTitle.titleTask = HLib::EnqueueTaskWithoutParameter(callback, 10, 0, static_cast<std::int32_t>(SHEN_NAME(CDTT)));
    loadTitle.alpha = 255;
}


void Queue_ST_LoadTitleFadeIn(shen_name author) {
    if (loadTitle.titleTask)
        return;
    DoLoadStuff(author.name, true);
    loadTitle.titleTask = HLib::EnqueueTaskWithoutParameter(ST_LoadTitleFadeIn_Callback, 7, 0, static_cast<std::int32_t>(SHEN_NAME(CDTT)));
    loadTitle.flags |= 2U;
    loadTitle.fadeInActive = author.name == Tag("1RTR") ? -1 : 1;
}


std::uint64_t scenSetTaskFlags(std::uint8_t mode) {
    std::uint32_t result = loadTitle.flags | 1U;
    const std::uint32_t flags = mode == 9 ? result : loadTitle.flags & ~1U;
    loadTitle.flags = flags;
    if (loadTitle.fadeMode != 4) {
        result = flags ? 3U : mode;
        loadTitle.fadeMode = static_cast<std::uint8_t>(result);
    }
    return result;
}

std::int16_t GetLoadTitleAlpha() {
    return loadTitle.alpha;
}


// @verified - p
void SetLoadingDrawFlags(std::int8_t mode) {
    RegisterBorrowedLegacySpriteSlot(static_cast<std::int16_t>(g_loadScreenSpriteTexture), 128, 128, 35, loadingSpriteRectangles.data());
    loadTitle.loadingDrawActive = 1;
    loadTitle.alpha = 255;
    loadTitle.loadingDrawMode = mode;
    loadTitle.loadingDotFrame = 0;
}


// @verified - p
void RetireLoadingDraw() {
    loadTitle.loadingDrawActive = 0;
    ReleaseBorrowedLegacySpriteSlot(35);
}


// @verified - p
void ResetLoadingDrawState() {
    loadTitle.loadingDrawMode = 0;
    loadTitle.loadingDrawActive = 0;
}

bool IsLoadingDrawActive() {
    return loadTitle.loadingDrawActive != 0;
}

void DrawLoadingModes() {
    const std::int32_t mode = loadTitle.loadingDrawMode;
    if (mode < 0 || mode > 3)
        return;
    const auto &position = loadingSpritePositions[mode];
    if (mode == 2) {
        loadTitle.loadingSpin = static_cast<std::uint16_t>(loadTitle.loadingSpin + 63352U);
        DrawBorrowedLegacySprite(0x230000U, position[0], position[1], 0.81999999f, 0.81999999f, loadTitle.loadingSpin, static_cast<std::uint32_t>(-3618616));
        LegacyIconSprite marker{.x = 84.0f,
            .y = 448.0f,
            .scaleX = 1.0f,
            .scaleY = 3.0f,
            .depth = 1.0f,
            .flags = 0,
            .reserved16 = 0,
            .rotation = 0,
            .icon = 0,
            .width = 38,
            .height = 38,
            .alignment = 9,
            .reserved23 = {}};
        PrepareDrawWithMode(marker, ScreenMode::Centered);
        return;
    }
    if (GetDebugForceUIEnabled(UIElement::LoadingScreen))
        return;
    if (mode == 1)
        ST_LoadTitleFadeIn_Main(nullptr);
    else {
        if (mode == 3) {
            LegacyColourQuad background{.quad = {.flags = 0x400U}, .colour = 0xff000000U, .alternateColours = {0, 0, 0}};
            TransformScreenSpace(background.quad, ScreenAlignment::HorizontalStretch, 320, 240, 640, 480, 1.0f, 1.0f, 0, ScreenMode::Centered);
            shenSubmitQuad(background, 1.0f);
        }
        LoadTitleSpriteNullsub(0xffffffffU, g_loadScreenSpriteTexture);
    }
    loadTitle.loadingDotFrame = static_cast<std::uint8_t>(loadTitle.loadingDotFrame + 1U);
    if (loadTitle.loadingDotFrame >= 84)
        loadTitle.loadingDotFrame = 0;
    for (std::int32_t index = 0; index < loadTitle.loadingDotFrame / 14; ++index)
        DrawBorrowedLegacySprite(0x230001U, position[0] + 6 * index, position[1], 1.25f, 1.3125f, 0, 0xffffffffU);
}

} // namespace shenmue::s2
