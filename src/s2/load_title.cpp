#include "s2/load_title.hpp"
#include "s2/legacy_sprite_types.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
namespace shenmue {
namespace s2 {
const std::int16_t SUBTITLE_STACK_Y[4] = SHENMUE_SUBTITLE_STACK_Y_VALUES;
const std::int16_t SUBTITLE_STACK_HEIGHT[4] = SHENMUE_SUBTITLE_STACK_HEIGHT_VALUES;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void ReleaseNamedTexture_Wrapper(std::int32_t index);
#endif
} // namespace s2
} // namespace shenmue
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "platform/dc/sdk.hpp"
#include "s2/calendar_dc.hpp"
#include "s2/character_table_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/frame_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/load_title_dc.hpp"
#include "s2/map_info.hpp"
#include "s2/map_region_dc.hpp"
#include "s2/pause.hpp"
#include "s2/qron_dc.hpp"
#include "s2/scene_data.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/string_utility.hpp"
#include "s2/texture_dc.hpp"
#include "s2/title_sprite_dc.hpp"
#else
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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include <array>
#endif
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
#include <span>
#endif

namespace shenmue {
namespace s2 {
namespace {

// Native multi-character literal: 'ABCD' as MSVC encodes it.
#if SHENMUE_IS_DC
inline
#else
constexpr
#endif
    std::uint32_t
    Tag(const char (&text)[5]) {
    return static_cast<std::uint32_t>(static_cast<unsigned char>(text[0])) << 24U | static_cast<std::uint32_t>(static_cast<unsigned char>(text[1])) << 16U |
        static_cast<std::uint32_t>(static_cast<unsigned char>(text[2])) << 8U | static_cast<unsigned char>(text[3]);
}

// g_stLoadTitleTable (area plus optional entry digits to title label) and
// g_SCENMapRegions (sub-area rectangles, circles and height bands).
#if SHENMUE_IS_DC && SHENMUE_IS_S2
const LoadTitleEntry loadTitleEntries[280] = {
    {"1000", "MV"},
    {"2000", "RC"},
    {"A000", 0},
    {"AB00", 0},
    {"ABB0", 0},
    {"ABR0", 0},
    {"AK00", 0},
    {"AKS0", 0},
    {"AKTO", 0},
    {"AKA4", 0},
    {"AKYO", 0},
    {"AR00", 0},
    {"ARA0", "ARA0B"},
    {"ARC0", 0},
    {"ARCB", 0},
    {"ARK0", 0},
    {"ARPO", 0},
    {"W000", 0},
    {"WB00", 0},
    {"WB01", 0},
    {"WE00", 0},
    {"WK00", 0},
    {"WR00", 0},
    {"WS00", 0},
    {"WSG1", 0},
    {"WT00", 0},
    {"ARXB", 0},
    {"WKA0", 0},
    {"WB003", "WB00B"},
    {"WTA0", 0},
    {"030A", "ARXB"},
    {"SM00", 0},
    {"SMMK", 0},
    {"AR004", "AR00"},
    {"WS001", "WS00"},
    {"WN00", 0},
    {"AR01", "AR00"},
    {"AR010", "AR004"},
    {"CB00", "ARCB"},
    {"KRF1", 0},
    {"KRH1", 0},
    {"KRZ1", 0},
    {"KWM1", 0},
    {"KRS2", 0},
    {"KRM1", 0},
    {"KRM2", 0},
    {"KRM3", 0},
    {"KWW1", 0},
    {"KWW5", 0},
    {"KRC1", 0},
    {"KRC2", 0},
    {"KWW2", 0},
    {"KRK2", 0},
    {"KWW6", 0},
    {"KSH1", 0},
    {"KSH3", 0},
    {"KSH4", 0},
    {"KWW4", 0},
    {"KES1", 0},
    {"TBCN", 0},
    {"AR03", 0},
    {"Q100", 0},
    {"Q200", 0},
    {"QA00", 0},
    {"QB00", 0},
    {"QBAA", 0},
    {"QC00", 0},
    {"QD00", 0},
    {"QE00", 0},
    {"QF00", 0},
    {"QR00", 0},
    {"AR02", "AR00"},
    {"QA001", 0},
    {"QA002", 0},
    {"QA003", 0},
    {"QA004", 0},
    {"QA005", 0},
    {"QA006", 0},
    {"QJBT", 0},
    {"QKBT", 0},
    {"QD001", 0},
    {"QD002", 0},
    {"QD003", 0},
    {"QE001", 0},
    {"QE002", 0},
    {"QLBT", 0},
    {"QF001", 0},
    {"QF002", 0},
    {"QGBT1", 0},
    {"Q300", 0},
    {"AKB0", 0},
    {"ARM0", 0},
    {"ART0", 0},
    {"ARZ0", 0},
    {"WSC1", 0},
    {"WSP0", 0},
    {"WSS0", 0},
    {"WSY0", 0},
    {"WSUR", 0},
    {"WSS1", 0},
    {"WSS2", 0},
    {"WSS3", 0},
    {"WSS4", 0},
    {"WSS5", 0},
    {"WKP0", 0},
    {"WEA0", 0},
    {"WEA1", 0},
    {"WECF", 0},
    {"WEG0", 0},
    {"WEG1", "WEG0"},
    {"WEG2", "WEG0"},
    {"WEG3", "WEG0"},
    {"WEG4", "WEG0"},
    {"WEG5", "WEG0"},
    {"WEH0", 0},
    {"WES1", 0},
    {"WEC0", 0},
    {"WNC0", 0},
    {"WND0", 0},
    {"WNDA", 0},
    {"WNXE", 0},
    {"WNXF", 0},
    {"WNXG", 0},
    {"WNXH", 0},
    {"WEIC", 0},
    {"WEM1", 0},
    {"WNN1", 0},
    {"WNS4", 0},
    {"WNS5", 0},
    {"WNT0", 0},
    {"WNUR", 0},
    {"WNX7", 0},
    {"WNX8", 0},
    {"WNX9", 0},
    {"WNXJ", 0},
    {"WNXL", 0},
    {"WNXM", 0},
    {"WRP0", 0},
    {"WRS1", 0},
    {"WRS2", 0},
    {"WTK0", 0},
    {"WTKB", 0},
    {"WTP0", 0},
    {"WTP1", 0},
    {"WTP2", 0},
    {"WTP3", 0},
    {"WTP4", 0},
    {"WTR0", 0},
    {"WTS3", 0},
    {"WTTB", 0},
    {"WTY0", 0},
    {"ZZZZ", 0},
    {"WTA07", "WTA0A"},
    {"WTK1", 0},
    {"WTK2", 0},
    {"WTX2", 0},
    {"WTX3", 0},
    {"WTX4", 0},
    {"WTXA", 0},
    {"WTXB", 0},
    {"0004", "AR004"},
    {"WTE0", 0},
    {"AKS1", 0},
    {"AR0186", "ARK0"},
    {"QAE104", 0},
    {"QAE105", 0},
    {"QAE106", 0},
    {"QAE110", 0},
    {"QAE154", 0},
    {"QAW101", 0},
    {"QAW102", 0},
    {"QAW110", 0},
    {"QC01", 0},
    {"QD01", 0},
    {"QE0101", 0},
    {"QE0102", 0},
    {"QRR0", 0},
    {"QUG0", 0},
    {"KRK3", 0},
    {"KRK4", 0},
    {"KRK5", 0},
    {"KRK5", 0},
    {"KRK6", 0},
    {"KRK7", 0},
    {"QSFA", 0},
    {"QSFB", 0},
    {"QSFC", 0},
    {"QSFD", 0},
    {"QTB1", 0},
    {"QTB2", 0},
    {"QTB3", 0},
    {"WTSH", "WTA080"},
    {"QA10", 0},
    {"QA14", 0},
    {"QA12", 0},
    {"QA13", 0},
    {"QE10", 0},
    {"QE11", 0},
    {"QE12", 0},
    {"QA15", 0},
    {"QC00", 0},
    {"QC01", 0},
    {"QC06", 0},
    {"QC10", "QC00"},
    {"QD00", 0},
    {"QD01", 0},
    {"QA22", 0},
    {"QAAG", 0},
    {"QAAJ", 0},
    {"QAEN", 0},
    {"QAFJ", 0},
    {"QAFK", 0},
    {"QCAE", 0},
    {"QCCE", 0},
    {"QCEF", 0},
    {"QDKJ", 0},
    {"QEDJ", 0},
    {"QEEC", 0},
    {"QEHH", 0},
    {"QEHI", 0},
    {"QRC0", 0},
    {"NDAY", 0},
    {"SDAY", 0},
    {"1DAY", 0},
    {"2DAY", 0},
    {"3DAY", 0},
    {"4DAY", 0},
    {"2KAM", 0},
    {"3KAM", 0},
    {"4KAM", 0},
    {"QD20", 0},
    {"QF10", 0},
    {"QF11", 0},
    {"QF12", "QF11"},
    {"QAAA", 0},
    {"KWWW", 0},
    {"KWA1", 0},
    {"KWQ2", 0},
    {"KWA2", 0},
    {"KWQ3", 0},
    {"KMZ1", 0},
    {"KMZ2", 0},
    {"KMZ3", 0},
    {"KMZ4", 0},
    {"WTA099", "WTA080"},
    {"TMOV", "TIME02"},
    {"QAW002", 0},
    {"QE53", 0},
    {"QTB4", 0},
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
    {"0250", "QCCE"},
    {"0523", "KRM3"},
    {"WB0033", "NDAY"},
    {"AKT0", 0},
    {"AKT1", 0},
    {"AKT2", 0},
    {"AKT3", 0},
    {"ARP0", 0},
    {"WKP1", 0},
    {"WER0", 0},
    {"WESM", 0},
    {"WET0", 0},
    {"WNXK", 0},
    {"WNN0", 0},
    {"WBS2", 0},
    {"WB10", "WB00B"},
    {"0362", "WTA080"},
};
#else
constexpr std::array<LoadTitleEntry, 297> loadTitleEntries{{
    {"1000", "MV"},
    {"2000", "RC"},
    {"A000", 0},
    {"AB00", 0},
    {"ABB0", 0},
    {"ABR0", 0},
    {"ABLB", "AR00"},
    {"0055", "AB00"},
    {"AK00", 0},
    {"AKY0", 0},
    {"AKS0", 0},
    {"AKA3", "AKA4"},
    {"AKA4", 0},
    {"AR00", 0},
    {"ARA0", "ARA0"},
    {"ARC0", 0},
    {"ARCB", 0},
    {"ARSF", 0},
    {"ARK0", 0},
    {"W000", 0},
    {"WB00", 0},
    {"WB01", 0},
    {"WE00", 0},
    {"WK00", 0},
    {"WR00", 0},
    {"WS00", 0},
    {"WSG1", 0},
    {"WT00", 0},
    {"ARXB", 0},
    {"WKA0", 0},
    {"WTA0", 0},
    {"030A", "ARXB"},
    {"SM00", 0},
    {"SMMK", 0},
    {"AR004", "AR00"},
    {"WS001", "WS00"},
    {"WN00", 0},
    {"AR01", "AR00"},
    {"AR010", "AR004"},
    {"CB00", "ARCB"},
    {"0034", "WB01"},
    {"KRF1", 0},
    {"KRH1", 0},
    {"KRZ1", 0},
    {"KWM1", 0},
    {"KRS2", 0},
    {"KRM1", 0},
    {"KRM2", 0},
    {"KRM3", 0},
    {"KWW1", 0},
    {"KWW5", 0},
    {"KRC1", 0},
    {"KRC2", 0},
    {"KWW2", 0},
    {"KRK2", 0},
    {"KWW6", 0},
    {"KSH1", "KSHJ"},
    {"KSH3", 0},
    {"KSH4", 0},
    {"KWW4", 0},
    {"KES1", 0},
    {"TBCN", "TSGO"},
    {"AR03", 0},
    {"Q100", 0},
    {"Q200", 0},
    {"QA00", "QA10"},
    {"QB00", 0},
    {"QBAA", 0},
    {"QC00", 0},
    {"QD10", "QD00"},
    {"QE00", 0},
    {"QF00", "QF10"},
    {"QFTS", "QF10"},
    {"QFHG", "QF10"},
    {"QF01", "QF10"},
    {"QF02", "QF10"},
    {"QF06", "QF10"},
    {"QR00", 0},
    {"0297", "QR00"},
    {"AR02", "AR00"},
    {"QJBT", 0},
    {"QKBT", "QF11"},
    {"QE001", 0},
    {"QE002", 0},
    {"QLBT", 0},
    {"QF001", 0},
    {"QF002", 0},
    {"QGBT1", 0},
    {"Q300", 0},
    {"AKB0", 0},
    {"ARM0", 0},
    {"ART0", 0},
    {"ARZ0", 0},
    {"WSC1", 0},
    {"WSP0", 0},
    {"WSS0", 0},
    {"WSY0", 0},
    {"WSUR", 0},
    {"WSS1", 0},
    {"WSS2", 0},
    {"WSS3", 0},
    {"WSS4", 0},
    {"WSS5", 0},
    {"WKP0", 0},
    {"WEA0", 0},
    {"WEA1", 0},
    {"WECF", 0},
    {"WEG0", 0},
    {"WEG1", "WEG0"},
    {"WEG2", "WEG0"},
    {"WEG3", "WEG0"},
    {"WEG4", "WEG0"},
    {"WEG5", "WEG0"},
    {"WEH0", 0},
    {"WES1", 0},
    {"WEC0", 0},
    {"WNC0", 0},
    {"WND0", 0},
    {"WNDA", 0},
    {"WNXE", 0},
    {"WNXF", 0},
    {"WNXG", 0},
    {"WNXH", 0},
    {"WEIC", 0},
    {"WEM1", 0},
    {"WNN1", 0},
    {"WNS4", 0},
    {"TOFB", "WNS4"},
    {"WNS5", 0},
    {"WNT0", 0},
    {"WNUR", 0},
    {"WNX7", 0},
    {"WNX8", 0},
    {"WNX9", 0},
    {"WNXJ", 0},
    {"WNXL", 0},
    {"WNXM", 0},
    {"WRP0", 0},
    {"WRS1", 0},
    {"WRS2", 0},
    {"WTK0", 0},
    {"WTKB", 0},
    {"WTP0", 0},
    {"WTP1", 0},
    {"WTP2", 0},
    {"WTP3", 0},
    {"WTP4", 0},
    {"WTR0", 0},
    {"WTS3", 0},
    {"WTTB", 0},
    {"WTY0", 0},
    {"ZZZZ", 0},
    {"WTK1", 0},
    {"WTK2", 0},
    {"WTX2", 0},
    {"WTX3", 0},
    {"WTX4", 0},
    {"WTXA", 0},
    {"WTXB", 0},
    {"0004", "AR01"},
    {"WTE0", 0},
    {"AKS1", 0},
    {"AR0186", "ARK0"},
    {"QAE104", 0},
    {"QAE105", 0},
    {"QAE106", 0},
    {"QAE110", 0},
    {"QAE154", 0},
    {"QAW101", 0},
    {"QAW102", 0},
    {"QAW110", 0},
    {"QE0101", 0},
    {"QE0102", 0},
    {"QRR0", 0},
    {"QUG0", "QUZO"},
    {"QF12", "QF11"},
    {"QF39", "QF11"},
    {"QF40", "QF11"},
    {"QFRR", "QF11"},
    {"KRK3", 0},
    {"KRK4", 0},
    {"KRK5", 0},
    {"KRK5", 0},
    {"KRK6", 0},
    {"KRK7", 0},
    {"QSFA", 0},
    {"QSFB", 0},
    {"QSFC", 0},
    {"QSFD", 0},
    {"QTB1", 0},
    {"QTB2", 0},
    {"QTB3", 0},
    {"WTSH", "WTA080"},
    {"QA10", 0},
    {"QA14", 0},
    {"QA12", 0},
    {"QA13", 0},
    {"QE10", 0},
    {"QE11", 0},
    {"QE12", 0},
    {"QA15", 0},
    {"QC00", 0},
    {"QC01", 0},
    {"QC06", 0},
    {"QC10", "QC00"},
    {"QD00", 0},
    {"QD01", 0},
    {"QA22", 0},
    {"QAAG", 0},
    {"QAAJ", 0},
    {"QAEN", 0},
    {"QAFJ", 0},
    {"QAFK", 0},
    {"QCAE", 0},
    {"QCCE", 0},
    {"QCEF", 0},
    {"QDKJ", 0},
    {"QEDJ", 0},
    {"QEEC", 0},
    {"QEHH", 0},
    {"QEHI", 0},
    {"QRC0", 0},
    {"FHL1", 0},
    {"NDAY", 0},
    {"SDAY", 0},
    {"1DAY", 0},
    {"2DAY", 0},
    {"3DAY", 0},
    {"4DAY", 0},
    {"2KAM", 0},
    {"3KAM", 0},
    {"4KAM", 0},
    {"QD20", 0},
    {"QF10", 0},
    {"QF11", 0},
    {"QF12", "QF11"},
    {"QAAA", 0},
    {"KWWW", 0},
    {"KWA1", 0},
    {"KWQ2", 0},
    {"KWA2", 0},
    {"KWQ3", 0},
    {"KMZ1", 0},
    {"KMZ2", 0},
    {"KMZ3", 0},
    {"KMZ4", 0},
    {"WTA099", "WTA080"},
    {"TMOV", "TIME02"},
    {"QAW002", 0},
    {"QE53", 0},
    {"QTB4", 0},
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
    {"AKT0", 0},
    {"AKT1", 0},
    {"AKT2", 0},
    {"AKT3", 0},
    {"ARP0", 0},
    {"WKP1", 0},
    {"WER0", 0},
    {"WESM", 0},
    {"WET0", 0},
    {"WNXK", 0},
    {"WNN0", 0},
    {"WBS2", 0},
    {"WB205", "WB00B"},
    {"WB1040", "WB00A"},
    {"WB10", "WB00B"},
    {"WB20", "WB00A"},
    {"ARKQ", "ABR0"},
    {"GABT", "QE001"},
    {"0362", "WTA080"},
    {"KWQA", "KWA1"},
    {"WSY1", 0},
    {"KSHJ", 0},
    {"QCHF", 0},
    {"QCJE", 0},
    {"QCLG", 0},
    {"WBBK", "WB00B"},
    {"0200", "QA00"},
    {"00391", "WSY0"},
    {"00395", "WTA080"},
}};
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
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
#endif

// g_stFloorTitleTable: floor/sub-title sprites keyed by area and entry.
#if SHENMUE_IS_DC && SHENMUE_IS_S2
const FloorTitleEntry floorTitles[6] = {
    {SHEN_NAME4('W', 'S', '0', '0'), 1, "L_WS001.GZ"},
    {SHEN_NAME4('A', 'R', '0', '3'), 4, "L_AR004.GZ"},
    {SHEN_NAME4('A', 'R', '0', '1'), 0, "L_AR01.GZ"},
    {SHEN_NAME4('A', '0', '0', '0'), 0, "L_AR01.GZ"},
    {SHEN_NAME4('0', '0', '0', '4'), 0, "L_AR01.GZ"},
    {0, 0, 0},
};
const FloorTitleEntry *floorTitleCursor;
#else
const std::array<FloorTitleEntry, 5> floorTitles{{
    {"WS00", 1, "L_WS001.GZ"},
    {"AR03", 4, "L_AR004.GZ"},
    {"AR01", 0, "L_AR01.GZ"},
    {"A000", 0, "L_AR01.GZ"},
    {"Q100", 1, "L_QA00.GZ"},
}};
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t loadTitleDigitCount;
const MapInfoChunk *loadTitleScene = 0;

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
const LoadTitleEntry *FindLoadTitle(std::uint32_t sourceName, std::int32_t entry) {
    char key[5];
    std::uint32_t name = sourceName;
    std::int32_t index;
    for (index = 0; index < 4; ++index)
        key[index] = reinterpret_cast<const char *>(&name)[index];
    key[4] = 0;
    std::int32_t fallback = -1;
    const LoadTitleEntry *title = loadTitleEntries;
    for (index = 0; index < 280 && title->key; ++index, ++title) {
        if (std::strncmp(key, title->key, 4) != 0)
            continue;
        if (std::strlen(title->key) > 4) {
            if (std::atoi(title->key + 4) == entry)
                return title;
        } else {
            fallback = index;
        }
    }
    if (fallback >= 0)
        return &loadTitleEntries[fallback];
    return 0;
}
#else
// @verified - d
const LoadTitleEntry *FindLoadTitle(std::uint32_t name, std::int32_t entry) {
    const LoadTitleEntry *fallback = 0;
    for (std::size_t index = 0; index < 280; ++index) {
        const LoadTitleEntry &title = loadTitleEntries[index];
        if (std::strncmp(reinterpret_cast<const char *>(&name), title.key, 4) != 0)
            continue;
        if (std::strlen(title.key) < 5)
            fallback = &title;
        else if (std::atoi(title.key + 4) == entry)
            return &title;
    }
    return fallback;
}
#endif

// @verified - d
void LoadFloorTitle(std::uint32_t name, std::int32_t entry) {
    floorTitleCursor = floorTitles;
    g_loadFloorTexture = -1;
    for (; floorTitleCursor->sprite; ++floorTitleCursor) {
        if (floorTitleCursor->area != name || floorTitleCursor->entry != entry)
            continue;
        void *const source = shenLoadCompressedFile("sprite", floorTitleCursor->sprite, SHEN_NAME4('L', 'D', 'S', 'B'));
        if (!source)
            continue;
        g_loadFloorTexture = static_cast<std::int32_t>(LoadAndFindTEXN_Wrapper(reinterpret_cast<const TextureChunk *>(source)));
        g_loadFloorWidth = static_cast<std::int16_t>(reinterpret_cast<const TextureChunk *>(source)->width);
        HLib::FreeTaskData(source);
        return;
    }
}

// @verified - d
void ReleaseLoadTitleTexture(std::int32_t &texture) {
    if (texture != -1)
        ReleaseNamedTexture(texture);
    texture = -1;
}

// @verified - d
void ST_LoadTitleFadeIn_Callback(void *) {
    if (!(g_loadingFlag & 0x102U)) {
        g_loadTitleAlpha = static_cast<std::int16_t>(g_loadTitleAlpha + 8);
        if (g_loadTitleAlpha > 255)
            g_loadTitleAlpha = 255;
    }
    HLib::ExecuteCallbackNow(ST_LoadTitleFadeIn_Main, 0);
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void ST_LoadTitleFadeOut_Callback(void *) {
    if (g_loadingDrawActive)
        RetireLoadingDraw();
    if (!(g_loadingFlag & 0x102U))
        g_loadTitleAlpha = static_cast<std::int16_t>(g_loadTitleAlpha - 8);
    if (g_loadTitleAlpha >= 0) {
        HLib::ExecuteCallbackNow(ST_LoadTitleFadeIn_Main, 0);
        return;
    }
    if (g_loadFloorTexture != -1)
        ReleaseNamedTexture_Wrapper(g_loadFloorTexture);
    g_loadFloorTexture = -1;
    if (g_loadTitleTexture != -1)
        ReleaseNamedTexture_Wrapper(g_loadTitleTexture);
    g_loadTitleTexture = -1;
    for (std::int32_t index = 0; index < loadTitleDigitCount; ++index) {
        if (g_loadTitleDigitTextures[index] != -1)
            ReleaseNamedTexture_Wrapper(g_loadTitleDigitTextures[index]);
        g_loadTitleDigitTextures[index] = -1;
    }
    loadTitleDigitCount = 0;
    g_loadTitleFadeMode = 0;
    g_loadTitleFlags = 0;
    if (g_loadTitleTask)
        HLib::DestroyTaskInternal(g_loadTitleTask);
    g_loadTitleTask = 0;
}
#else
// @verified - d
void ST_LoadTitleFadeOut_Callback(void *) {
    if (g_loadingDrawActive)
        RetireLoadingDraw();
    if (!(g_loadingFlag & 0x102U))
        g_loadTitleAlpha = static_cast<std::int16_t>(g_loadTitleAlpha - 8);
    if (g_loadTitleAlpha < 0)
        ReleaseLoadTitleFade();
    else
        HLib::ExecuteCallbackNow(ST_LoadTitleFadeIn_Main, 0);
}
#endif

#else
// g_stQuestTitleTable: battle-quest title pairs used under internal flag 1015.
constexpr std::array<LoadTitleEntry, 4> questTitles{{{"QJBT", "MG01"}, {"QGBT", "MG02"}, {"QLBT", "MG03"}, {"QKBT", "MG04"}}};

shen_name temporaryAreaJumpAuthor = {};
LoadTitleState loadTitle = {0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0, -1, {}, 0, {}, 0};
constexpr std::array<std::uint16_t, 8> loadingSpriteRectangles{98, 99, 28, 28, 1, 1, 2, 2};
constexpr std::array<std::array<std::int32_t, 2>, 4> loadingSpritePositions{{{395, 431}, {395, 431}, {44, 438}, {395, 431}}};

// PC v1.07 submits every legacy loading sprite through `nullsub`; the HD
// loading widget draws the visible card instead.
void LoadTitleSpriteNullsub(std::uint32_t colour, std::int32_t texture) {
    static_cast<void>(colour);
    static_cast<void>(texture);
}

const MapInfoChunk *CurrentMapInfo() {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return control ? static_cast<const MapInfoChunk *>(control->mapInfo) : 0;
}

bool SameName(std::uint32_t name, const char *text) {
    return text && std::memcmp(&name, text, 4) == 0;
}

std::uint32_t NameOf(const char *text) {
    std::uint32_t name = {};
    std::memcpy(&name, text, 4);
    return name;
}

// Native GetMapName -> scenFindMapRegionName: the sub-area region containing the
// position on the current map, else the per-map fallback name.
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

// Native stResolveLoadStream asks QRON for the entry placement and stream,
// then resolves the loading-title sub-area from that placement.
std::int32_t ResolveLoadTitleStream(std::uint32_t &subarea) {
    Vector3 position = {};
    std::int32_t yaw = {};
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
    return 0;
}

// Native stFindLoadTitle: the title-table row for an area/sub-area and entry.
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
    return fallback >= 0 ? &loadTitleEntries[static_cast<std::size_t>(fallback)] : 0;
}

std::uint16_t TexnWidth(const void *texn) {
    std::uint16_t width = {};
    std::memcpy(&width, static_cast<const std::byte *>(texn) + 40, sizeof(width));
    return width;
}

// Native stLoadFloorTitle: the floor/sub-title sprite for this area and entry.
void LoadFloorTitle(std::uint32_t area, std::int32_t entry) {
    loadTitle.floorTexture = -1;
    for (const FloorTitleEntry &floor : floorTitles) {
        if (!SameName(area, floor.area) || floor.entry != entry)
            continue;
        void *allocation = 0;
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

// Native stDrawFloorTitle: the floor sub-title at 0.65 scale.
void DrawFloorTitle() {
    if (loadTitle.floorTexture >= 0)
        LoadTitleSpriteNullsub(static_cast<std::uint32_t>(loadTitle.alpha) << 24U | 0xffffffU, loadTitle.floorTexture);
}

// Native stDrawFloorNumber: the floor number built from LOADNUM digits.
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

// Native stDrawLoadNumber: a LOADNUM number; mode bit 1 keeps leading zeros.
void DrawLoadNumber(std::int32_t value, std::uint8_t mode, std::int32_t digits) {
    const std::int32_t powers[4] = {1, 10, 100, 1000};
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

// Native stDrawLoadingLabel: the NOW LOADING label sprite.
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
    HLib::ExecuteCallbackNow(ST_LoadTitleFadeIn_Main, 0);
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
        HLib::ExecuteCallbackNow(ST_LoadTitleFadeIn_Main, 0);
}

#endif

// Native SBTS holding callback: the shared nullsub, registered as ST_WaitSubTitle.
// @verified - p
void ST_WaitSubTitle(void *) {
}

// Native ST_DrawSubTitle. A single texture follows its authored position; SUBTITLE.SPR stacks up to four strips, rotating only
// the first. PC v1.07 submits each strip through the legacy-sprite nullsub.
// @verified - p
void ST_DrawSubTitle(void *taskData) {
    SubtitleSpriteWork &work = *static_cast<SubtitleSpriteWork *>(taskData);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    LegacySpritePacket *const sprite = &work.sprite;
    sprite->colour = static_cast<std::uint32_t>(work.alpha) << 24U | 0xffffffU;
#else
    work.sprite.colour = static_cast<std::uint32_t>(work.alpha) << 24U | 0xffffffU;
#endif
    if (!work.stacked) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        AdjustScreenAspectRatio(work.sprite.quad, HorizontalStretch, static_cast<std::int32_t>(work.x), static_cast<std::int32_t>(work.y), 512, 512);
        DrawLegacySpritePacket(work.sprite, static_cast<std::uint32_t>(work.textures[0]), 1.0f);
#else
        AdjustScreenAspectRatio(work.sprite.quad, ScreenAlignment::HorizontalStretch, TruncateSpriteCoordinate(work.x), TruncateSpriteCoordinate(work.y), 512,
            512, ScreenMode::Centered);
        LegacySpriteNullsub(work.sprite, static_cast<std::uint32_t>(work.textures[0]), 1.0f);
#endif
        return;
    }
    for (std::int32_t index = 0; index < 4; ++index) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        float rotation = 0.0f;
        if (index == 0)
            rotation = work.rotation;
#else
        const float rotation = index == 0 ? work.rotation : 0.0f;
#endif
        if (work.textures[index] == -1)
            continue;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        TransformScreenSpace(work.sprite.quad, HorizontalStretch, 320, SUBTITLE_STACK_Y[index], 512, SUBTITLE_STACK_HEIGHT[index], 1.0f, 1.0f,
            static_cast<std::int32_t>(rotation));
        DrawLegacySpritePacket(work.sprite, static_cast<std::uint32_t>(work.textures[index]), static_cast<float>(index) + 1.0f);
#else
        TransformScreenSpace(work.sprite.quad, ScreenAlignment::HorizontalStretch, 320, SUBTITLE_STACK_Y[index], 512, SUBTITLE_STACK_HEIGHT[index], 1.0f, 1.0f,
            TruncateSpriteCoordinate(rotation), ScreenMode::Centered);
        LegacySpriteNullsub(work.sprite, static_cast<std::uint32_t>(work.textures[index]), static_cast<float>(index) + 1.0f);
#endif
    }
}

// @verified - p
void ST_FadeInSubTitle_Callback(void *taskData) {
    SubtitleSpriteWork &work = *static_cast<SubtitleSpriteWork *>(taskData);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    work.rotation += SUBTITLE_ROTATION_STEP;
    work.alpha = static_cast<std::int16_t>(work.alpha + work.alphaStep);
#else
    work.alpha = static_cast<std::int16_t>(work.alpha + work.alphaStep);
    work.rotation += SUBTITLE_ROTATION_STEP;
#endif
    if (work.alpha > 255)
        work.alpha = 255;
    HLib::ExecuteCallbackNow(ST_DrawSubTitle, taskData);
}

// Native ST_FadeOutSubTitle_Callback: once alpha passes zero it releases the textures, the TASK block and the task.
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void ST_FadeOutSubTitle_Callback(void *taskData) {
    SubtitleSpriteWork *work = static_cast<SubtitleSpriteWork *>(taskData);
    work->rotation += SUBTITLE_ROTATION_STEP;
    work->alpha = static_cast<std::int16_t>(work->alpha - work->alphaStep);
    if (work->alpha < 0) {
        work->alpha = 0;
        for (std::int32_t index = 0; index < 4; ++index) {
            if (work->textures[index] != -1)
                ReleaseNamedTexture_Wrapper(work->textures[index]);
            work->textures[index] = -1;
        }
        FreeTASK(work->task);
        HLib::DestroyTaskInternal(work->task);
    } else {
        HLib::ExecuteCallbackNow(ST_DrawSubTitle, work);
    }
}
#else
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
    for (std::uint32_t index = 0; index < 4; ++index) {
        std::int32_t &texture = work.textures[index];
        if (texture != -1)
            ReleaseNamedTexture(texture);
        texture = -1;
    }
    HLib::Task *const task = work.task;
    FreeTASK(task);
    HLib::DestroyTaskInternal(task);
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    loadTitle.fadeInActive = 0;
#endif
}
#endif

} // namespace

// Native Queue_ST_WaitSubTitle: loads the sprite file's TEXN chunks into a held SBTS task. Native checks neither the TASK
// allocation nor the file load.
// @verified - p
HLib::Task *Queue_ST_WaitSubTitle(const char *filename) {
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    loadTitle.fadeInActive = 0;
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    HLib::Task *const task =
        HLib::EnqueueTaskWithoutParameter(ST_WaitSubTitle, 9, 0, static_cast<std::int32_t>(static_cast<std::uint32_t>(SUBTITLE_WAIT_TASK)));
#else
    HLib::Task *const task = HLib::EnqueueTaskWithoutParameter(ST_WaitSubTitle, 9, 0, static_cast<std::int32_t>(SUBTITLE_WAIT_TASK.name));
#endif
    SubtitleSpriteWork *const work = static_cast<SubtitleSpriteWork *>(AllocateTASK(task, sizeof(SubtitleSpriteWork)));
    work->task = task;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    for (std::int32_t index = 0; index < 4; ++index)
        work->textures[index] = -1;
#else
    work->textures = {-1, -1, -1, -1};
#endif
    work->stacked = 0;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    void *const file = shenLoadFile("sprite", filename, SUBTITLE_SPRITE_FILE);
    LoadTextureChunks(reinterpret_cast<const TextureChunk *>(file), work->textures);
    HLib::FreeTaskData(file);
    if (CompareCharacterTableTokens(filename, "SUBTITLE.SPR") == 0)
        work->stacked = 1;
#else
    if (CompareTokenIgnoreCase(filename, "SUBTITLE.SPR") == 0)
        work->stacked = 1;
    void *const file = LoadLegacySceneFile("sprite", filename, SUBTITLE_SPRITE_FILE);
    ReadTEXN_Recursive(file, work->textures.data());
    HLib::FreeTaskData(file);
#endif
    return task;
}

// @verified - p
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t ST_FadeInSubTitle(HLib::Task *task, float x, float y, std::int16_t alphaStep) {
#else
std::int64_t ST_FadeInSubTitle(HLib::Task *task, float x, float y, std::int16_t alphaStep) {
#endif
    HLib::AssignCallbackForTask(task, ST_FadeInSubTitle_Callback);
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    loadTitle.fadeInActive = 1;
#endif
    SubtitleSpriteWork &work = *static_cast<SubtitleSpriteWork *>(HLib::GetTaskParameter(task));
    work.x = x;
    work.y = y;
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    work.sprite.quad.flags = 0;
#endif
    work.alpha = 0;
    work.alphaStep = alphaStep;
    work.rotation = 0.0f;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    LegacySpritePacket &sprite = work.sprite;
    INIT_LEGACY_SPRITE_PACKET(sprite);
#else
    work.sprite.colour = 0xffffffffU;
    work.sprite.secondaryColour = 0;
    work.sprite.uv = {0.0f, 0.0f, 1.0f, 1.0f};
#endif
    work.sprite.quad.flags |= 0x400U;
#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
    work.sprite.reservedTail = 0;
#endif
    return 1;
}

// @verified - p
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t ST_FadeOutSubTitle(HLib::Task *task) {
#else
std::int64_t ST_FadeOutSubTitle(HLib::Task *task) {
#endif
    HLib::AssignCallbackForTask(task, ST_FadeOutSubTitle_Callback);
    return 1;
}
#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void ReleaseLoadTitleFade() {
    ReleaseLoadTitleTexture(g_loadFloorTexture);
    ReleaseLoadTitleTexture(g_loadTitleTexture);
    for (std::int32_t index = 0; index < loadTitleDigitCount; ++index)
        ReleaseLoadTitleTexture(g_loadTitleDigitTextures[index]);
    loadTitleDigitCount = 0;
    g_loadTitleFadeMode = 0;
    g_loadTitleFlags = 0;
    if (g_loadTitleTask)
        HLib::DestroyTaskInternal(g_loadTitleTask);
    g_loadTitleTask = 0;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
int ResolveQronLoadLocation(std::uint32_t *subarea);
bool IsCurrentAreaQA11();

void DoLoadStuff(std::uint32_t author, bool nextStream) {
    const LoadTitleEntry *title = 0;
    std::uint32_t subarea = 0;
    if (nextStream)
        g_loadTitleStream = GetQronNextStream();
    else
        g_loadTitleStream = ResolveQronLoadLocation(&subarea);
    g_loadTitleFlags &= ~8U;
    if (g_loadTitleStream != -1000) {
        if (IsCurrentAreaQA11())
            g_loadTitleFlags |= 0x10U;
        g_loadTitleFlags |= 8U;
    }
    g_loadTitleTask = 0;
    loadTitleDigitCount = 0;
    g_loadTitleFlags &= ~2U;
    g_loadTitleFlags &= ~4U;
    MapInfoChunk *const mapInfo = GetCurrentMapInfo();
    loadTitleScene = FindNativeChunk("LSCN", mapInfo);
    if (g_loadTitleFadeMode > 9)
        g_loadTitleFadeMode = 0;
    if (g_loadTitleFadeMode == 6 || g_loadTitleFadeMode == 1)
        return;

    DateTime date;
    shenGetCurrentDateTime(date);
    g_loadTitleYearTens = static_cast<std::int8_t>(date.year / 10);
    g_loadTitleYearUnits = static_cast<std::int8_t>(date.year % 10);
    g_loadTitleMonthTens = static_cast<std::int8_t>(date.month / 10);
    g_loadTitleMonthUnits = static_cast<std::int8_t>(date.month % 10);
    g_loadTitleDayTens = static_cast<std::int8_t>(date.day / 10);
    g_loadTitleDayUnits = static_cast<std::int8_t>(date.day % 10);
    std::int8_t hour = date.hour;
    if (hour < 12) {
        g_loadTitleMeridiemSprite = 22;
    } else {
        g_loadTitleMeridiemSprite = 21;
        if (hour != 12)
            hour -= 12;
    }
    g_loadTitleHourTens = static_cast<std::int8_t>(hour / 10);
    g_loadTitleHourUnits = static_cast<std::int8_t>(hour % 10);
    g_loadTitleMinuteTens = static_cast<std::int8_t>(date.minute / 10);
    g_loadTitleMinuteUnits = static_cast<std::int8_t>(date.minute % 10);
    g_loadTitleWeek = static_cast<std::int8_t>(date.week);

    std::uint32_t name;
    if (author == 0)
        name = (g_loadTitleFlags & 8U) && subarea ? subarea : g_currentMap;
    else
        name = author;
    if (g_loadTitleFlags & 1U) {
        g_loadTitleFlags |= 4U;
        title = FindLoadTitle(SHEN_NAME(ZZZZ), 0);
    } else if (g_actNumber && g_loadTitleFadeMode != 7) {
        title = FindLoadTitle(name, g_entryPoint);
    }
    const std::int32_t motion = texGetMotionFlag();
    texSetMotionFlag(0);
    switch (g_currentMap) {
    case SHEN_NAME4('1', '0', '0', '0'):
    case SHEN_NAME4('2', '0', '0', '0'):
        g_loadTitleFlags |= 4U;
        break;
    case SHEN_NAME4('M', 'F', '0', '1'):
        g_loadTitleFadeMode = 8;
        break;
    }
    switch (author) {
    case SHEN_NAME4('R', 'T', 'R', '2'):
    case SHEN_NAME4('R', 'T', 'R', '1'):
    case SHEN_NAME4('G', 'O', 'V', '1'):
    case SHEN_NAME4('T', 'O', 'V', '1'):
    case SHEN_NAME4('G', 'O', 'V', '2'):
    case SHEN_NAME4('T', 'B', 'C', 'N'):
    case SHEN_NAME4('T', 'M', 'O', 'V'):
        g_loadTitleFlags = (g_loadTitleFlags | 4U) & ~8U;
        break;
    }
    LoadFloorTitle(g_currentMap, g_entryPoint);
    if (title) {
        typedef char Filename[32] __attribute__((aligned(4)));
        Filename filename = {};
        const char *key;
        if (title->alias == 0)
            key = title->key;
        else
            key = title->alias;
        std::strcpy(filename, "L_");
        std::strcat(filename, key);
        std::strcat(filename, ".GZ");
        void *const source = shenLoadCompressedFile("sprite", filename, SHEN_NAME(LDAR));
        LoadTextureChunks(reinterpret_cast<const TextureChunk *>(source), &g_loadTitleTexture);
        g_loadTitleWidth = static_cast<std::int16_t>(reinterpret_cast<const TextureChunk *>(source)->width);
        HLib::FreeTaskData(source);
    }
    void *const source = shenLoadCompressedFile("sprite", "LOADNUM.GZ", SHEN_NAME(LDTD));
    loadTitleDigitCount = LoadTextureChunks(reinterpret_cast<const TextureChunk *>(source), g_loadTitleDigitTextures);
    HLib::FreeTaskData(source);
    texSetMotionFlag(motion);
}
#else
// @verified - d
void DoLoadStuff(std::uint32_t author, bool nextStream) {
    std::uint32_t subarea = 0;
    if (nextStream) {
        g_loadTitleStream = GetQronNextStream();
    } else {
        const QronMapDefinition *const definition = FindQronMapDefinition(g_currentMap);
        if (!definition) {
            g_loadTitleStream = -1000;
        } else {
            Vector3 position;
            std::int32_t yaw;
            g_loadTitleStream = ResolveQronEntryLocation(*definition, position, yaw);
            subarea = FindMapRegionName(&position);
        }
    }
    g_loadTitleFlags &= ~8U;
    if (g_loadTitleStream != -1000) {
        if (g_currentMap == SHEN_NAME(QA11))
            g_loadTitleFlags |= 0x10U;
        g_loadTitleFlags |= 8U;
    }
    g_loadTitleTask = 0;
    loadTitleDigitCount = 0;
    g_loadTitleFlags &= ~6U;
    loadTitleScene = FindNativeChunk("LSCN", GetCurrentMapInfo());
    if (g_loadTitleFadeMode > 9)
        g_loadTitleFadeMode = 0;
    if (g_loadTitleFadeMode == 6 || g_loadTitleFadeMode == 1)
        return;

    DateTime date;
    shenGetCurrentDateTime(date);
    g_loadTitleYearTens = static_cast<std::int8_t>(date.year / 10);
    g_loadTitleYearUnits = static_cast<std::int8_t>(date.year % 10);
    g_loadTitleMonthTens = static_cast<std::int8_t>(date.month / 10);
    g_loadTitleMonthUnits = static_cast<std::int8_t>(date.month % 10);
    g_loadTitleDayTens = static_cast<std::int8_t>(date.day / 10);
    g_loadTitleDayUnits = static_cast<std::int8_t>(date.day % 10);
    std::int32_t hour = date.hour;
    if (hour > 11) {
        g_loadTitleMeridiemSprite = 21;
        if (hour != 12)
            hour -= 12;
    } else {
        g_loadTitleMeridiemSprite = 22;
    }
    g_loadTitleHourTens = static_cast<std::int8_t>(hour / 10);
    g_loadTitleHourUnits = static_cast<std::int8_t>(hour % 10);
    g_loadTitleMinuteTens = static_cast<std::int8_t>(date.minute / 10);
    g_loadTitleMinuteUnits = static_cast<std::int8_t>(date.minute % 10);
    g_loadTitleWeek = static_cast<std::int8_t>(date.week);

    std::uint32_t name = author;
    if (!name)
        name = (g_loadTitleFlags & 8U) && subarea ? subarea : g_currentMap;
    const LoadTitleEntry *title = 0;
    if (g_loadTitleFlags & 1U) {
        g_loadTitleFlags |= 4U;
        title = FindLoadTitle(SHEN_NAME(ZZZZ), 0);
    } else if (g_actNumber && g_loadTitleFadeMode != 7) {
        title = FindLoadTitle(name, g_entryPoint);
    }
    const std::int32_t motion = texGetMotionFlag();
    texSetMotionFlag(0);
    if (g_currentMap == SHEN_NAME(1000) || g_currentMap == SHEN_NAME(2000))
        g_loadTitleFlags |= 4U;
    else if (g_currentMap == SHEN_NAME(MF01))
        g_loadTitleFadeMode = 8;
    if (author ==
        SHEN_NAME(RTR2) ||
            author ==
                SHEN_NAME(RTR1) ||
                    author ==
                        SHEN_NAME(GOV1) || author == SHEN_NAME(TOV1) || author == SHEN_NAME(GOV2) || author == SHEN_NAME(TBCN) || author == SHEN_NAME(TMOV))
        g_loadTitleFlags = (g_loadTitleFlags & ~8U) | 4U;
    LoadFloorTitle(g_currentMap, g_entryPoint);
    if (title) {
        char filename[32] = {};
        std::sprintf(filename, "L_%s.GZ", title->alias ? title->alias : title->key);
        void *const source = shenLoadCompressedFile("sprite", filename, SHEN_NAME(LDAR));
        LoadTextureChunks(reinterpret_cast<const TextureChunk *>(source), &g_loadTitleTexture);
        g_loadTitleWidth = static_cast<std::int16_t>(reinterpret_cast<const TextureChunk *>(source)->width);
        HLib::FreeTaskData(source);
    }
    void *const source = shenLoadCompressedFile("sprite", "LOADNUM.GZ", SHEN_NAME(LDTD));
    loadTitleDigitCount = LoadTextureChunks(reinterpret_cast<const TextureChunk *>(source), g_loadTitleDigitTextures);
    HLib::FreeTaskData(source);
    texSetMotionFlag(motion);
}
#endif

// @verified - d
void ST_LoadTitleFadeIn() {
    DoLoadStuff(0, false);
    g_loadTitleAlpha = 0;
    do {
        reinterpret_cast<void (*)()>(ST_LoadTitleFadeIn_Main)();
        dc::sdk::SubmitFrame();
        g_loadTitleAlpha = static_cast<std::int16_t>(g_loadTitleAlpha + 8);
    } while (g_loadTitleAlpha < 256);
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void ST_LoadTitleFade() {
    std::int32_t show = 0;
    if (loadTitleDigitCount || g_loadTitleFadeMode == 6 || g_loadTitleFadeMode == 1)
        show = 1;
    if (show) {
        if (!g_loadTitleTask) {
            const shen_name taskName = SHEN_NAME4('C', 'D', 'T', 'T');
            if (loadTitleScene)
                g_loadTitleTask = HLib::EnqueueTaskWithoutParameter(ST_LoadTitleFadeIn_Callback, 10, 0, taskName);
            else
                g_loadTitleTask = HLib::EnqueueTaskWithoutParameter(ST_LoadTitleFadeOut_Callback, 10, 0, taskName);
            g_loadTitleAlpha = 255;
        } else {
            HLib::AssignCallbackForTask(g_loadTitleTask, ST_LoadTitleFadeOut_Callback);
        }
    }
}
#else
// @verified - d
void ST_LoadTitleFade() {
    if (!loadTitleDigitCount && g_loadTitleFadeMode != 6 && g_loadTitleFadeMode != 1)
        return;
    if (g_loadTitleTask) {
        HLib::AssignCallbackForTask(g_loadTitleTask, ST_LoadTitleFadeOut_Callback);
    } else {
        g_loadTitleTask =
            HLib::EnqueueTaskWithoutParameter(loadTitleScene ? ST_LoadTitleFadeIn_Callback : ST_LoadTitleFadeOut_Callback, 10, 0, SHEN_NAME(CDTT));
        g_loadTitleAlpha = 255;
    }
}
#endif

// @verified - d
void Queue_ST_LoadTitleFadeIn(shen_name author) {
    if (g_loadTitleTask)
        return;
    DoLoadStuff(author, true);
    g_loadTitleTask = HLib::EnqueueTaskWithoutParameter(ST_LoadTitleFadeIn_Callback, 7, 0, SHEN_NAME(CDTT));
    g_loadTitleFlags |= 2U;
}

#else
// Native ST_ReleaseTitleFade: releases the title, floor and digit textures and CDTT.
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
    loadTitle.titleTask = 0;
    loadTitle.fadeInActive = 0;
    if (task)
        HLib::DestroyTask(task);
}

void InitST_TitleFunc() {
    loadTitle.titleTask = 0;
    loadTitle.digitTextures.fill(-1);
    loadTitle.titleTexture = -1;
    loadTitle.floorTexture = -1;
}

// Native DoLoadStuff: resolves the loading title, date digits and floor
// stream, loads the legacy title sprites and publishes the loading widget.
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
    loadTitle.titleTask = 0;
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
    DateTime now = {};
    shenGetCurrentDateTime(now);
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
    const LoadTitleEntry *title = 0;
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
    const char *label = 0;
    if (title) {
        label = title->alias ? title->alias : title->key;
        std::array<char, 32> sprite{};
        std::snprintf(sprite.data(), sprite.size(), "L_%s.GZ", label);
        void *allocation = 0;
        std::size_t size = 0;
        if (LoadLegacySprite("sprite", sprite.data(), SHEN_NAME(LDAR), allocation, size)) {
            loadTitle.titleTexture = static_cast<std::int32_t>(ReadTexnTexture(allocation));
            loadTitle.titleWidth = TexnWidth(allocation);
            HLib::FreeTaskData(allocation);
        }
    }
    void *allocation = 0;
    std::size_t size = 0;
    if (LoadLegacySprite("sprite", "LOADNUM.GZ", SHEN_NAME(LDTD), allocation, size)) {
        loadTitle.digitCount = static_cast<std::int32_t>(ReadTEXN_Recursive(allocation, loadTitle.digitTextures.data()));
        HLib::FreeTaskData(allocation);
    }
    DisableFilmBarAndUpdateFramePacing();
    SetupLoadingScreen(shen_name(area), shen_name(titleSubarea), now, label, loadTitle.flags, loadTitle.stream, loadTitle.fadeMode, g_entryPoint);
}

// @verified - p
shen_name GetMapRegionName(const Vector3 &position) {
    return shen_name(FindMapRegionName(position));
}

// @verified - p
void SetTemporaryAreaJumpAuthor(shen_name author) {
    temporaryAreaJumpAuthor = author;
}

// @verified - p
void ResetTemporaryAreaJumpAuthor() {
    temporaryAreaJumpAuthor = {};
}

// Native ST_LoadTitleFadeIn: loads the title and submits the whole fade-in in
// one call (alpha 0..255 in steps of 8).
void ST_LoadTitleFadeIn() {
    loadTitle.fadeInActive = 1;
    const bool areaJump = HasSavedAreaJumpLocation();
    DoLoadStuff(areaJump ? temporaryAreaJumpAuthor.name : 0, areaJump && temporaryAreaJumpAuthor.name != 0);
    if (areaJump)
        temporaryAreaJumpAuthor = {};
    loadTitle.alpha = 0;
    do {
        ST_LoadTitleFadeIn_Main(0);
        loadTitle.alpha = static_cast<std::int16_t>(loadTitle.alpha + 8);
    } while (loadTitle.alpha <= 255);
}

// Native LoadTitleFadeIn: the already loaded title drawn once at full alpha.
void LoadTitleFadeIn() {
    loadTitle.alpha = 255;
    ST_LoadTitleFadeIn_Main(nullptr);
}

// Native ST_LoadTitleFade: hands the title to CDTT for the fade-out, or keeps
// a load-scene title faded in.
void ST_LoadTitleFade() {
    loadTitle.fadeInActive = 0;
    if (!loadTitle.digitCount && loadTitle.fadeMode != 6 && loadTitle.fadeMode != 1)
        return;
    if (loadTitle.titleTask) {
        WaitForNextRenderFrame(0, 0);
        SetD3TRenderSettings(1);
        HLib::AssignCallbackForTask(loadTitle.titleTask, ST_LoadTitleFadeOut_Callback);
        return;
    }
    HLib::TaskCallback callback = ST_LoadTitleFadeIn_Callback;
    if (!loadTitle.loadScene) {
        WaitForNextRenderFrame(0, 0);
        SetD3TRenderSettings(1);
        callback = ST_LoadTitleFadeOut_Callback;
    }
    loadTitle.titleTask = HLib::EnqueueTaskWithoutParameter(callback, 10, 0, static_cast<std::int32_t>(SHEN_NAME(CDTT)));
    loadTitle.alpha = 255;
}

// Native Queue_ST_LoadTitleFadeIn (Queue_ST_LoadTitleFadeIn_0 for event code).
void Queue_ST_LoadTitleFadeIn(shen_name author) {
    if (loadTitle.titleTask)
        return;
    DoLoadStuff(author.name, true);
    loadTitle.titleTask = HLib::EnqueueTaskWithoutParameter(ST_LoadTitleFadeIn_Callback, 7, 0, static_cast<std::int32_t>(SHEN_NAME(CDTT)));
    loadTitle.flags |= 2U;
    loadTitle.fadeInActive = author.name == Tag("1RTR") ? -1 : 1;
}

// Fade mode 4 remains set, including in the returned old flags.
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

// Native SetLoadingDrawFlags binds a borrowed LSCR surface to slot 35.
// @verified - p
void SetLoadingDrawFlags(std::int8_t mode) {
    RegisterBorrowedLegacySpriteSlot(static_cast<std::int16_t>(g_loadScreenSpriteTexture), 128, 128, 35, loadingSpriteRectangles.data());
    loadTitle.loadingDrawActive = 1;
    loadTitle.alpha = 255;
    loadTitle.loadingDrawMode = mode;
    loadTitle.loadingDotFrame = 0;
}

// Native fadeInit / ST fade-out retire only the borrowed sprite slot.
// @verified - p
void RetireLoadingDraw() {
    loadTitle.loadingDrawActive = 0;
    ReleaseBorrowedLegacySpriteSlot(35);
}

// Leaves the registered slot and animation counters untouched.
// @verified - p
void ResetLoadingDrawState() {
    loadTitle.loadingDrawMode = 0;
    loadTitle.loadingDrawActive = 0;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t IsLoadingDrawActive() {
#else
bool IsLoadingDrawActive() {
#endif
    return loadTitle.loadingDrawActive != 0;
}

void DrawLoadingModes() {
    const std::int32_t mode = loadTitle.loadingDrawMode;
    if (mode < 0 || mode > 3)
        return;
    const std::array<std::int32_t, 2> &position = loadingSpritePositions[mode];
    if (mode == 2) {
        loadTitle.loadingSpin = static_cast<std::uint16_t>(loadTitle.loadingSpin + 63352U);
        DrawBorrowedLegacySprite(0x230000U, position[0], position[1], 0.81999999f, 0.81999999f, loadTitle.loadingSpin, static_cast<std::uint32_t>(-3618616));
        LegacyIconSprite marker = {SHEN_FIELD(x) 84.0f, SHEN_FIELD(y) 448.0f, SHEN_FIELD(scaleX) 1.0f, SHEN_FIELD(scaleY) 3.0f, SHEN_FIELD(depth) 1.0f,
            SHEN_FIELD(flags) 0, SHEN_FIELD(reserved16) 0, SHEN_FIELD(rotation) 0, SHEN_FIELD(icon) 0, SHEN_FIELD(width) 38, SHEN_FIELD(height) 38,
            SHEN_FIELD(alignment) 9, SHEN_FIELD(reserved23){}};
        PrepareDrawWithMode(marker, ScreenMode::Centered);
        return;
    }
    if (GetDebugForceUIEnabled(UIElement::LoadingScreen))
        return;
    if (mode == 1)
        ST_LoadTitleFadeIn_Main(0);
    else {
        if (mode == 3) {
            LegacyColourQuad background = {SHEN_FIELD(quad){SHEN_FIELD(flags) 0x400U}, SHEN_FIELD(colour) 0xff000000U, SHEN_FIELD(alternateColours){0, 0, 0}};
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

#endif

} // namespace s2
} // namespace shenmue
