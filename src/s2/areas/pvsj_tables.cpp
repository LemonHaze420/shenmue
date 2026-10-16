#include "s2/areas/area_pvsj.hpp"
namespace shenmue::s2 {
namespace {
const shen_name OD1_Points[]{SHEN_NAME(GOL1), SHEN_NAME(STR1), SHEN_NAME(STR2), SHEN_NAME(STR3), shen_name{}};
PvsjCustomer OD1_Customers[]{
    {SHEN_NAME(OD1_), SHEN_NAME(GOL1), shen_name{}, 0},
    {SHEN_NAME(AHE_), SHEN_NAME(STR2), shen_name{}, 1},
    {SHEN_NAME(SYO_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(AD2_), SHEN_NAME(STR2), shen_name{}, 1},
    {SHEN_NAME(CHW_), SHEN_NAME(STR2), shen_name{}, 1},
    {SHEN_NAME(CKN_), SHEN_NAME(STR3), shen_name{}, 1},
    {SHEN_NAME(SSA_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(SNN_), SHEN_NAME(STR3), shen_name{}, 1},
    {SHEN_NAME(SYY_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(EMI_), SHEN_NAME(STR3), shen_name{}, 1},
    {SHEN_NAME(KOO_), SHEN_NAME(STR1), shen_name{}, 2},
    {SHEN_NAME(KOW_), SHEN_NAME(STR2), shen_name{}, 2},
    {SHEN_NAME(YM1_), SHEN_NAME(STR3), shen_name{}, 2},
    {SHEN_NAME(YM2_), SHEN_NAME(STR3), shen_name{}, 2},
    {shen_name{}, shen_name{}, shen_name{}, 0},
};
const std::uint8_t OD1_Greetings[]{1, 1, 0, 3, 1, 2, 1, 1, 0, 0, 3, 0, 1, 0, 0, 2};
const std::int32_t OD1_Motions[]{6051, 6059, 6057, 6058, 6056, 6049, 6040, 6042, 6041, 6043, 6044, 6039, 6046, 6055, 6045, 6052, 6054, 6047};
const shen_name KNT_Points[]{SHEN_NAME(GOL1), SHEN_NAME(STR1), SHEN_NAME(STR2), SHEN_NAME(STR3), shen_name{}};
PvsjCustomer KNT_Customers[]{
    {SHEN_NAME(KNT_), SHEN_NAME(GOL1), shen_name{}, 0},
    {SHEN_NAME(SYO_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(CHW_), SHEN_NAME(STR2), shen_name{}, 1},
    {SHEN_NAME(TJN_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(CKN_), SHEN_NAME(STR3), shen_name{}, 1},
    {SHEN_NAME(SSA_), SHEN_NAME(STR2), shen_name{}, 1},
    {SHEN_NAME(SNN_), SHEN_NAME(STR2), shen_name{}, 1},
    {SHEN_NAME(SYY_), SHEN_NAME(STR3), shen_name{}, 1},
    {SHEN_NAME(EMI_), SHEN_NAME(STR3), shen_name{}, 1},
    {SHEN_NAME(KOO_), SHEN_NAME(STR3), shen_name{}, 2},
    {SHEN_NAME(KOW_), SHEN_NAME(STR2), shen_name{}, 2},
    {SHEN_NAME(YM1_), SHEN_NAME(STR1), shen_name{}, 2},
    {SHEN_NAME(YM2_), SHEN_NAME(STR2), shen_name{}, 2},
    {shen_name{}, shen_name{}, shen_name{}, 0},
};
const std::uint8_t KNT_Greetings[]{0, 1, 1, 0, 1, 2, 0, 1, 3, 0, 1, 0, 2, 0, 0, 1};
const std::int32_t KNT_Motions[]{6051, 6059, 6057, 6058, 6056, 6049, 6040, 6042, 6041, 6043, 6044, 6039, 6046, 6055, 6045, 6052, 6054, 6047};
const shen_name KT4_Points[]{SHEN_NAME(GOL1), SHEN_NAME(STR1), SHEN_NAME(STR2), SHEN_NAME(STR3), shen_name{}};
PvsjCustomer KT4_Customers[]{
    {SHEN_NAME(KT4_), SHEN_NAME(GOL1), shen_name{}, 0},
    {SHEN_NAME(AHE_), SHEN_NAME(STR2), shen_name{}, 1},
    {SHEN_NAME(SYO_), SHEN_NAME(STR2), shen_name{}, 1},
    {SHEN_NAME(AD2_), SHEN_NAME(STR2), shen_name{}, 1},
    {SHEN_NAME(CHW_), SHEN_NAME(STR3), shen_name{}, 1},
    {SHEN_NAME(TJN_), SHEN_NAME(STR3), shen_name{}, 1},
    {SHEN_NAME(CKN_), SHEN_NAME(STR3), shen_name{}, 1},
    {SHEN_NAME(SSA_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(SNN_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(SYY_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(KOO_), SHEN_NAME(STR2), shen_name{}, 2},
    {SHEN_NAME(KOW_), SHEN_NAME(STR1), shen_name{}, 2},
    {SHEN_NAME(YM1_), SHEN_NAME(STR3), shen_name{}, 2},
    {SHEN_NAME(YM2_), SHEN_NAME(STR3), shen_name{}, 2},
    {shen_name{}, shen_name{}, shen_name{}, 0},
};
const std::uint8_t KT4_Greetings[]{1, 2, 1, 3, 0, 2, 1, 0, 2, 1, 1, 0, 2, 0, 3, 1};
const std::int32_t KT4_Motions[]{6051, 6059, 6057, 6058, 6056, 6049, 6040, 6042, 6041, 6043, 6044, 6039, 6046, 6055, 6045, 6052, 6054, 6047};
const shen_name SNS_Points[]{SHEN_NAME(GOL3), SHEN_NAME(STR3), SHEN_NAME(STR4), SHEN_NAME(STR5), shen_name{}};
PvsjCustomer SNS_Customers[]{
    {SHEN_NAME(SNS_), SHEN_NAME(GOL3), shen_name{}, 0},
    {SHEN_NAME(AHE_), SHEN_NAME(STR4), shen_name{}, 1},
    {SHEN_NAME(SYO_), SHEN_NAME(STR4), shen_name{}, 1},
    {SHEN_NAME(AD2_), SHEN_NAME(STR4), shen_name{}, 1},
    {SHEN_NAME(CHW_), SHEN_NAME(STR4), shen_name{}, 1},
    {SHEN_NAME(TJN_), SHEN_NAME(STR5), shen_name{}, 1},
    {SHEN_NAME(CKN_), SHEN_NAME(STR5), shen_name{}, 1},
    {SHEN_NAME(SSA_), SHEN_NAME(STR3), shen_name{}, 1},
    {SHEN_NAME(SNN_), SHEN_NAME(STR3), shen_name{}, 1},
    {SHEN_NAME(SYY_), SHEN_NAME(STR3), shen_name{}, 1},
    {SHEN_NAME(EMI_), SHEN_NAME(STR5), shen_name{}, 1},
    {SHEN_NAME(KOO_), SHEN_NAME(STR5), shen_name{}, 2},
    {SHEN_NAME(KOW_), SHEN_NAME(STR4), shen_name{}, 2},
    {SHEN_NAME(YM1_), SHEN_NAME(STR3), shen_name{}, 2},
    {SHEN_NAME(YM2_), SHEN_NAME(STR3), shen_name{}, 2},
    {shen_name{}, shen_name{}, shen_name{}, 0},
};
const std::uint8_t SNS_Greetings[]{0, 1, 0, 2, 1, 1, 0, 1, 3, 1, 0, 1, 2, 0, 0, 1};
const std::int32_t SNS_Motions[]{6051, 6059, 6057, 6058, 6056, 6049, 6040, 6042, 6041, 6043, 6044, 6039, 6046, 6055, 6045, 6052, 6054, 6047};
const shen_name EIN_Points[]{SHEN_NAME(GOL6), SHEN_NAME(STR6), SHEN_NAME(STR7), SHEN_NAME(STR8), shen_name{}};
PvsjCustomer EIN_Customers[]{
    {SHEN_NAME(EIN_), SHEN_NAME(GOL6), shen_name{}, 0},
    {SHEN_NAME(AHE_), SHEN_NAME(STR8), shen_name{}, 1},
    {SHEN_NAME(SYO_), SHEN_NAME(STR6), shen_name{}, 1},
    {SHEN_NAME(AD2_), SHEN_NAME(STR8), shen_name{}, 1},
    {SHEN_NAME(CHW_), SHEN_NAME(STR6), shen_name{}, 1},
    {SHEN_NAME(TJN_), SHEN_NAME(STR6), shen_name{}, 1},
    {SHEN_NAME(SSA_), SHEN_NAME(STR6), shen_name{}, 1},
    {SHEN_NAME(SNN_), SHEN_NAME(STR6), shen_name{}, 1},
    {SHEN_NAME(SYY_), SHEN_NAME(STR6), shen_name{}, 1},
    {SHEN_NAME(EMI_), SHEN_NAME(STR8), shen_name{}, 1},
    {SHEN_NAME(KOO_), SHEN_NAME(STR8), shen_name{}, 2},
    {SHEN_NAME(KOW_), SHEN_NAME(STR6), shen_name{}, 2},
    {SHEN_NAME(YM1_), SHEN_NAME(STR7), shen_name{}, 2},
    {SHEN_NAME(YM2_), SHEN_NAME(STR7), shen_name{}, 2},
    {shen_name{}, shen_name{}, shen_name{}, 0},
};
const std::uint8_t EIN_Greetings[]{3, 0, 1, 1, 2, 1, 2, 1, 3, 0, 1, 0, 1, 1, 2, 1};
const std::int32_t EIN_Motions[]{6051, 6059, 6057, 6058, 6056, 6049, 6040, 6042, 6041, 6043, 6044, 6039, 6046, 6055, 6045, 6052, 6054, 6047};
const shen_name GKS_Points[]{SHEN_NAME(GOL1), SHEN_NAME(STR1), SHEN_NAME(STR2), shen_name{}};
PvsjCustomer GKS_Customers[]{
    {SHEN_NAME(GKS_), SHEN_NAME(GOL1), shen_name{}, 0},
    {SHEN_NAME(AHE_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(SYO_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(AD2_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(CHW_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(TJN_), SHEN_NAME(STR2), shen_name{}, 1},
    {SHEN_NAME(CKN_), SHEN_NAME(STR2), shen_name{}, 1},
    {SHEN_NAME(SSA_), SHEN_NAME(STR2), shen_name{}, 1},
    {SHEN_NAME(SNN_), SHEN_NAME(STR2), shen_name{}, 1},
    {SHEN_NAME(SYY_), SHEN_NAME(STR2), shen_name{}, 1},
    {SHEN_NAME(EMI_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(KOO_), SHEN_NAME(STR1), shen_name{}, 2},
    {SHEN_NAME(KOW_), SHEN_NAME(STR1), shen_name{}, 2},
    {SHEN_NAME(YM1_), SHEN_NAME(STR2), shen_name{}, 2},
    {SHEN_NAME(YM2_), SHEN_NAME(STR2), shen_name{}, 2},
    {shen_name{}, shen_name{}, shen_name{}, 0},
};
const std::uint8_t GKS_Greetings[]{2, 1, 1, 2, 1, 0, 3, 1, 0, 1, 2, 1, 1, 2, 3, 0};
const std::int32_t GKS_Motions[]{6051, 6059, 6057, 6058, 6056, 6049, 6040, 6042, 6041, 6043, 6044, 6039, 6046, 6055, 6045, 6052, 6054, 6047};
const shen_name KU4_Points[]{SHEN_NAME(GOL1), SHEN_NAME(STR1), SHEN_NAME(STR2), SHEN_NAME(STR3), shen_name{}};
PvsjCustomer KU4_Customers[]{
    {SHEN_NAME(KU4_), SHEN_NAME(GOL1), shen_name{}, 0},
    {SHEN_NAME(MEJ_), SHEN_NAME(STR2), shen_name{}, 1},
    {SHEN_NAME(TSY_), SHEN_NAME(STR3), shen_name{}, 1},
    {SHEN_NAME(KD4_), SHEN_NAME(STR3), shen_name{}, 1},
    {SHEN_NAME(SD7_), SHEN_NAME(STR2), shen_name{}, 1},
    {SHEN_NAME(KKK_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(WD1_), SHEN_NAME(STR3), shen_name{}, 1},
    {SHEN_NAME(KD3_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(AW6_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(ILN_), SHEN_NAME(STR2), shen_name{}, 1},
    {SHEN_NAME(KOO_), SHEN_NAME(STR1), shen_name{}, 2},
    {SHEN_NAME(KOW_), SHEN_NAME(STR2), shen_name{}, 2},
    {SHEN_NAME(YM1_), SHEN_NAME(STR1), shen_name{}, 2},
    {SHEN_NAME(YM2_), SHEN_NAME(STR3), shen_name{}, 2},
    {shen_name{}, shen_name{}, shen_name{}, 0},
};
const std::uint8_t KU4_Greetings[]{3, 1, 0, 1, 2, 0, 2, 1, 3, 0, 1, 1, 2, 1, 0, 1};
const std::int32_t KU4_Motions[]{6051, 6059, 6057, 6058, 6056, 6049, 6040, 6042, 6041, 6043, 6044, 6039, 6046, 6055, 6045, 6052, 6054, 6047};
const shen_name MFG_Points[]{SHEN_NAME(GOL1), SHEN_NAME(STR1), SHEN_NAME(STR2), shen_name{}};
PvsjCustomer MFG_Customers[]{
    {SHEN_NAME(MFG_), SHEN_NAME(GOL1), shen_name{}, 0},
    {SHEN_NAME(MEJ_), SHEN_NAME(STR2), shen_name{}, 1},
    {SHEN_NAME(TSY_), SHEN_NAME(STR2), shen_name{}, 1},
    {SHEN_NAME(KD4_), SHEN_NAME(STR2), shen_name{}, 1},
    {SHEN_NAME(SD7_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(KKK_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(WD1_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(KD3_), SHEN_NAME(STR2), shen_name{}, 1},
    {SHEN_NAME(AW6_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(ILN_), SHEN_NAME(STR2), shen_name{}, 1},
    {SHEN_NAME(KOO_), SHEN_NAME(STR2), shen_name{}, 2},
    {SHEN_NAME(KOW_), SHEN_NAME(STR2), shen_name{}, 2},
    {SHEN_NAME(YM1_), SHEN_NAME(STR1), shen_name{}, 2},
    {SHEN_NAME(YM2_), SHEN_NAME(STR1), shen_name{}, 2},
    {shen_name{}, shen_name{}, shen_name{}, 0},
};
const std::uint8_t MFG_Greetings[]{3, 0, 0, 1, 0, 1, 2, 0, 1, 1, 0, 2, 0, 0, 1, 1};
const std::int32_t MFG_Motions[]{6036, 6038, 6034, 6035, 6033, 6037, 6040, 6042, 6041, 6043, 6044, 6039, 6046, 6055, 6045, 6052, 6054, 6047};
const shen_name SD2_Points[]{SHEN_NAME(GOL1), SHEN_NAME(STR1), SHEN_NAME(STR2), shen_name{}};
PvsjCustomer SD2_Customers[]{
    {SHEN_NAME(SD2_), SHEN_NAME(GOL1), shen_name{}, 0},
    {SHEN_NAME(MEJ_), SHEN_NAME(STR2), shen_name{}, 1},
    {SHEN_NAME(TSY_), SHEN_NAME(STR2), shen_name{}, 1},
    {SHEN_NAME(KD4_), SHEN_NAME(STR2), shen_name{}, 1},
    {SHEN_NAME(SD7_), SHEN_NAME(STR2), shen_name{}, 1},
    {SHEN_NAME(KKK_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(WD1_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(KD3_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(AW6_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(ILN_), SHEN_NAME(STR2), shen_name{}, 1},
    {SHEN_NAME(KOO_), SHEN_NAME(STR1), shen_name{}, 2},
    {SHEN_NAME(KOW_), SHEN_NAME(STR2), shen_name{}, 2},
    {SHEN_NAME(YM1_), SHEN_NAME(STR2), shen_name{}, 2},
    {SHEN_NAME(YM2_), SHEN_NAME(STR1), shen_name{}, 2},
    {shen_name{}, shen_name{}, shen_name{}, 0},
};
const std::uint8_t SD2_Greetings[]{2, 1, 3, 1, 1, 1, 2, 0, 2, 1, 1, 1, 3, 1, 2, 0};
const std::int32_t SD2_Motions[]{6051, 6059, 6057, 6058, 6056, 6049, 6040, 6042, 6041, 6043, 6044, 6039, 6046, 6055, 6045, 6052, 6054, 6047};
const shen_name CHX_Points[]{SHEN_NAME(GOL1), SHEN_NAME(STR1), shen_name{}};
PvsjCustomer CHX_Customers[]{
    {SHEN_NAME(CHX_), SHEN_NAME(GOL1), shen_name{}, 0},
    {SHEN_NAME(MEJ_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(SD7_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(KKK_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(WD1_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(KD3_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(AW6_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(ILN_), SHEN_NAME(STR1), shen_name{}, 1},
    {SHEN_NAME(KOO_), SHEN_NAME(STR1), shen_name{}, 2},
    {SHEN_NAME(KOW_), SHEN_NAME(STR1), shen_name{}, 2},
    {SHEN_NAME(YM1_), SHEN_NAME(STR1), shen_name{}, 2},
    {SHEN_NAME(YM2_), SHEN_NAME(STR1), shen_name{}, 2},
    {shen_name{}, shen_name{}, shen_name{}, 0},
};
const std::uint8_t CHX_Greetings[]{0, 1, 1, 3, 0, 1, 2, 1, 1, 3, 1, 2, 1, 0, 1, 2};
const std::int32_t CHX_Motions[]{6051, 6059, 6057, 6058, 6056, 6049, 6040, 6042, 6041, 6043, 6044, 6039, 6046, 6055, 6045, 6052, 6054, 6047};
} // namespace
PvsjBoardGroup pvsjBoardGroups[]{
    {SHEN_NAME(AR09), SHEN_NAME(OD1_), OD1_Points, OD1_Customers, OD1_Greetings, 32768, 0, OD1_Motions},
    {SHEN_NAME(AK09), SHEN_NAME(KNT_), KNT_Points, KNT_Customers, KNT_Greetings, 32768, 0, KNT_Motions},
    {SHEN_NAME(WS09), SHEN_NAME(KT4_), KT4_Points, KT4_Customers, KT4_Greetings, 0, 0, KT4_Motions},
    {SHEN_NAME(WK09), SHEN_NAME(SNS_), SNS_Points, SNS_Customers, SNS_Greetings, 36864, 0, SNS_Motions},
    {SHEN_NAME(WK09), SHEN_NAME(EIN_), EIN_Points, EIN_Customers, EIN_Greetings, 0, 0, EIN_Motions},
    {SHEN_NAME(WK09), SHEN_NAME(GKS_), GKS_Points, GKS_Customers, GKS_Greetings, 8192, 0, GKS_Motions},
    {SHEN_NAME(Q109), SHEN_NAME(KU4_), KU4_Points, KU4_Customers, KU4_Greetings, 32768, 0, KU4_Motions},
    {SHEN_NAME(QR09), SHEN_NAME(MFG_), MFG_Points, MFG_Customers, MFG_Greetings, 61440, 0, MFG_Motions},
    {SHEN_NAME(QE09), SHEN_NAME(SD2_), SD2_Points, SD2_Customers, SD2_Greetings, 16384, 0, SD2_Motions},
    {SHEN_NAME(QT09), SHEN_NAME(CHX_), CHX_Points, CHX_Customers, CHX_Greetings, 36864, 0, CHX_Motions},
    {},
};
const PvsjCustomerPartition PVSJ_CUSTOMER_PARTITIONS[]{
    {SHEN_NAME(AD2_), 0, 1, 0, {}},
    {SHEN_NAME(AHE_), 2, 3, 1, {}},
    {SHEN_NAME(AW6_), 4, 5, 0, {}},
    {SHEN_NAME(CHW_), 6, 7, 0, {}},
    {SHEN_NAME(CKN_), 8, 9, 0, {}},
    {SHEN_NAME(EMI_), 10, 11, 1, {}},
    {SHEN_NAME(ILN_), 12, 13, 1, {}},
    {SHEN_NAME(KD3_), 14, 15, 1, {}},
    {SHEN_NAME(KD4_), 16, 17, 0, {}},
    {SHEN_NAME(KKK_), 18, 19, 1, {}},
    {SHEN_NAME(MEJ_), 20, 21, 1, {}},
    {SHEN_NAME(SD7_), 22, 23, 0, {}},
    {SHEN_NAME(SNN_), 24, 25, 0, {}},
    {SHEN_NAME(SSA_), 26, 27, 1, {}},
    {SHEN_NAME(SYO_), 28, 29, 0, {}},
    {SHEN_NAME(SYY_), 30, 31, 0, {}},
    {SHEN_NAME(TJN_), 32, 33, 0, {}},
    {SHEN_NAME(TSY_), 36, 37, 1, {}},
    {SHEN_NAME(WD1_), 38, 39, 0, {}},
    {},
};
} // namespace shenmue::s2
