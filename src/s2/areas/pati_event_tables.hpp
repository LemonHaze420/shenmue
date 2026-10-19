#pragma once
#include "s2/areas/area_pati.hpp"
namespace shenmue::s2 {
inline constexpr PatiDealerMotion PATI_DEALER_MOTIONS{6026, 6025, 6021, 6022, 6023, 6024};
inline constexpr PatiDealerMotion PATI_ALTERNATE_DEALER_MOTIONS{6019, 6018, 6017, 6022, 6023, 6024};
struct PatiDealer {
    shen_name character;
    std::int32_t playedFlag;
    const PatiDealerMotion *motions;
    std::int32_t camera;
    std::uint32_t reserved14;
};
static_assert(sizeof(PatiDealer) == 24);
inline constexpr PatiDealer PATI_DEALERS[]{{SHEN_NAME(OD1_), 1138, &PATI_DEALER_MOTIONS, 931, 0}, {SHEN_NAME(DY1_), 1135, &PATI_DEALER_MOTIONS, 911, 0},
    {SHEN_NAME(SOG_), 1136, &PATI_DEALER_MOTIONS, 931, 0}, {SHEN_NAME(KNT_), 1137, &PATI_DEALER_MOTIONS, 911, 0},
    {SHEN_NAME(CN6_), 1168, &PATI_DEALER_MOTIONS, 911, 0}, {SHEN_NAME(KT4_), 1169, &PATI_DEALER_MOTIONS, 931, 0},
    {SHEN_NAME(JOE_), 1170, &PATI_DEALER_MOTIONS, 931, 0}, {SHEN_NAME(SCN_), 1172, &PATI_DEALER_MOTIONS, 931, 0},
    {SHEN_NAME(SNS_), 1071, &PATI_DEALER_MOTIONS, 921, 0}, {SHEN_NAME(AA1_), 1159, &PATI_DEALER_MOTIONS, 931, 0},
    {SHEN_NAME(HAK_), 1173, &PATI_DEALER_MOTIONS, 911, 0}, {SHEN_NAME(DHO_), 1174, &PATI_DEALER_MOTIONS, 931, 0},
    {SHEN_NAME(LYO_), 1160, &PATI_DEALER_MOTIONS, 911, 0}, {SHEN_NAME(ANS_), 1171, &PATI_DEALER_MOTIONS, 931, 0},
    {SHEN_NAME(EIN_), 1175, &PATI_DEALER_MOTIONS, 931, 0}, {SHEN_NAME(KNH_), 1176, &PATI_DEALER_MOTIONS, 931, 0},
    {SHEN_NAME(RKU_), 1177, &PATI_DEALER_MOTIONS, 921, 0}, {SHEN_NAME(GKS_), 1165, &PATI_DEALER_MOTIONS, 931, 0},
    {SHEN_NAME(CHX_), 940, &PATI_DEALER_MOTIONS, 921, 0}, {SHEN_NAME(DM1_), 941, &PATI_ALTERNATE_DEALER_MOTIONS, 921, 0},
    {SHEN_NAME(DM2_), 942, &PATI_DEALER_MOTIONS, 931, 0}, {SHEN_NAME(DS4_), 943, &PATI_DEALER_MOTIONS, 931, 0},
    {SHEN_NAME(FD1_), 944, &PATI_DEALER_MOTIONS, 931, 0}, {SHEN_NAME(JI8_), 945, &PATI_DEALER_MOTIONS, 921, 0},
    {SHEN_NAME(KU4_), 946, &PATI_DEALER_MOTIONS, 911, 0}, {SHEN_NAME(MFG_), 1386, &PATI_ALTERNATE_DEALER_MOTIONS, 921, 0},
    {SHEN_NAME(OD3_), 947, &PATI_DEALER_MOTIONS, 931, 0}, {SHEN_NAME(PG5_), 1325, &PATI_ALTERNATE_DEALER_MOTIONS, 921, 0},
    {SHEN_NAME(SD2_), 948, &PATI_DEALER_MOTIONS, 931, 0}, {SHEN_NAME(SF7_), 1324, &PATI_DEALER_MOTIONS, 911, 0},
    {SHEN_NAME(TNJ_), 949, &PATI_DEALER_MOTIONS, 931, 0}, {SHEN_NAME(WO1_), 452, &PATI_DEALER_MOTIONS, 931, 0},
    {shen_name{SHEN_NAME(WO3_)}, 454, &PATI_DEALER_MOTIONS, 931, 0}, {shen_name{SHEN_NAME(ZKS_)}, 404, &PATI_DEALER_MOTIONS, 931, 0},
    {shen_name{SHEN_NAME(AHE_)}, -1, &PATI_DEALER_MOTIONS, 921, 0}, {shen_name{SHEN_NAME(SYO_)}, -1, &PATI_DEALER_MOTIONS, 931, 0},
    {shen_name{SHEN_NAME(AD2_)}, -1, &PATI_DEALER_MOTIONS, 931, 0}, {shen_name{SHEN_NAME(CHW_)}, -1, &PATI_DEALER_MOTIONS, 931, 0},
    {shen_name{SHEN_NAME(TJN_)}, -1, &PATI_DEALER_MOTIONS, 931, 0}, {shen_name{SHEN_NAME(CKN_)}, -1, &PATI_DEALER_MOTIONS, 931, 0},
    {shen_name{SHEN_NAME(SSA_)}, -1, &PATI_DEALER_MOTIONS, 921, 0}, {shen_name{SHEN_NAME(SNN_)}, -1, &PATI_DEALER_MOTIONS, 931, 0},
    {shen_name{SHEN_NAME(SYY_)}, -1, &PATI_DEALER_MOTIONS, 931, 0}, {shen_name{SHEN_NAME(EMI_)}, -1, &PATI_DEALER_MOTIONS, 921, 0},
    {shen_name{SHEN_NAME(MEJ_)}, -1, &PATI_DEALER_MOTIONS, 921, 0}, {SHEN_NAME(TSY_), -1, &PATI_DEALER_MOTIONS, 921, 0},
    {shen_name{SHEN_NAME(KD4_)}, -1, &PATI_DEALER_MOTIONS, 921, 0}, {shen_name{SHEN_NAME(SD7_)}, -1, &PATI_DEALER_MOTIONS, 931, 0},
    {shen_name{SHEN_NAME(KKK_)}, -1, &PATI_DEALER_MOTIONS, 921, 0}, {SHEN_NAME(WD1_), -1, &PATI_DEALER_MOTIONS, 931, 0},
    {shen_name{SHEN_NAME(KD3_)}, -1, &PATI_DEALER_MOTIONS, 921, 0}, {shen_name{SHEN_NAME(AW6_)}, -1, &PATI_DEALER_MOTIONS, 911, 0},
    {SHEN_NAME(ILN_), -1, &PATI_DEALER_MOTIONS, 921, 0}, {SHEN_NAME(TSO_), -1, &PATI_DEALER_MOTIONS, 921, 0},
    {SHEN_NAME(KOO_), -1, &PATI_DEALER_MOTIONS, 931, 0}, {SHEN_NAME(KOW_), -1, &PATI_DEALER_MOTIONS, 911, 0},
    {SHEN_NAME(YM1_), -1, &PATI_DEALER_MOTIONS, 931, 0}, {SHEN_NAME(YM2_), -1, &PATI_DEALER_MOTIONS, 931, 0}, {{}, -1, nullptr, 0, 0}};
struct PatiVoicePartition {
    const char *name;
    std::int32_t text, sound;
};
inline constexpr PatiVoicePartition PATI_VOICE_PARTITIONS[]{{"0104", 0, 1}, {"0104AA1", 2, 3}, {"0104AD2", 4, 5}, {"0104AHE", 6, 7}, {"0104ANS", 8, 9},
    {"0104AW6", 10, 11}, {"0104CHW", 12, 13}, {"0104CHX", 14, 15}, {"0104CKN", 16, 17}, {"0104CN6", 18, 19}, {"0104DHO", 20, 21}, {"0104DM1", 22, 23},
    {"0104DM2", 24, 25}, {"0104DS4", 26, 27}, {"0104DY1", 28, 29}, {"0104EIN", 30, 31}, {"0104EMI", 32, 33}, {"0104FD1", 34, 35}, {"0104GKS", 36, 37},
    {"0104HAK", 38, 39}, {"0104ILN", 40, 41}, {"0104JI8", 42, 43}, {"0104JOE", 44, 45}, {"0104KD3", 46, 47}, {"0104KD4", 48, 49}, {"0104KKK", 50, 51},
    {"0104KNH", 52, 53}, {"0104KNT", 54, 55}, {"0104KOO", 56, 57}, {"0104KOW", 58, 59}, {"0104KT4", 60, 61}, {"0104KU4", 62, 63}, {"0104LYO", 64, 65},
    {"0104MEJ", 66, 67}, {"0104MFG", 68, 69}, {"0104OD1", 70, 71}, {"0104PG5", 72, 73}, {"0104RKU", 74, 75}, {"0104SCN", 76, 77}, {"0104SD2", 78, 79},
    {"0104SD7", 80, 81}, {"0104SF7", 82, 83}, {"0104SNN", 84, 85}, {"0104SNS", 86, 87}, {"0104SOG", 88, 89}, {"0104SSA", 90, 91}, {"0104SYO", 92, 93},
    {"0104SYY", 94, 95}, {"0104TJN", 96, 97}, {"0104TNJ", 98, 99}, {"0104TSY", 100, 101}, {"0104WD1", 102, 103}, {"0104WO1", 104, 105}, {"0104WO3", 106, 107},
    {"0104YM1", 108, 109}, {"0104YM2", 110, 111}, {"0104ZKS", 112, 113}, {nullptr, 0, 1}};
struct PatiWagerMode {
    char name[4];
    std::uint8_t mode;
};
inline constexpr PatiWagerMode PATI_WAGER_MODES[]{{"1", 1}, {"5", 2}, {"10", 3}, {"20", 4}, {"50", 5}, {"100", 6}, {"", 3}};
struct PatiPayoutMode {
    char name[9];
    std::uint8_t mode;
};
inline constexpr PatiPayoutMode PATI_PAYOUT_MODES[]{{"2", 1}, {"5", 2}, {"10", 3}, {"20", 4}, {"30", 5}, {"40", 6}, {"50", 7}, {"60", 8}, {"70", 9}, {"80", 10},
    {"90", 11}, {"100", 12}, {"120", 13}, {"150", 14}, {"200", 15}, {"250", 16}, {"300", 17}, {"400", 18}, {"500", 19}, {"1000", 20}, {"5/10/20", 21},
    {"10/20/40", 22}, {"0/50/100", 23}, {"0/50/150", 24}, {"", 7}};
struct PatiHelpTopic {
    std::uint8_t option, type;
    std::int16_t topic;
};
inline constexpr PatiHelpTopic PATI_HELP_TOPICS[]{{0, 1, 119}, {0, 2, 120}, {0, 3, 121}, {0, 4, 122}, {0, 5, 123}, {0, 6, 124}, {0, 8, 146}, {3, 2, 125},
    {3, 5, 126}, {1, 1, 127}, {1, 2, 128}, {1, 3, 129}, {2, 1, 143}, {7, 1, 131}, {4, 1, 132}, {4, 3, 133}, {8, 1, 139}, {8, 2, 140}, {8, 3, 141}, {8, 5, 142},
    {6, 1, 119}, {255, 255, -1}};
struct PatiDealerInput {
    std::uint32_t input;
    std::int32_t duration, pause;
};
inline constexpr PatiDealerInput PATI_DEALER_INPUTS[]{{32768, 20, 10}, {32768, 15, 15}, {32768, 20, 20}, {32768, 20, 10}, {16384, 20, 20}, {16384, 60, 10},
    {16384, 20, 10}, {32768, 60, 10}, {16384, 15, 15}, {32768, 10, 20}, {16384, 10, 20}, {16384, 30, 10}, {32768, 10, 20}, {16384, 20, 10}, {32768, 30, 10},
    {16384, 10, 20}};
inline constexpr Vector3i PATI_HAND_ROTATION{52898, 4549, 3394};
inline std::int32_t patiHandPoseCommand[19][3]{{0, 725, 64079}, {0, 1, 1092}, {0, 61167, 0}, {0, 0, 63897}, {0, 363, 60622}, {0, 1456, 58437}, {0, 65172, 4913},
    {0, 0, 58437}, {0, 0, 45515}, {0, 0, 53341}, {0, 546, 4368}, {0, 0, 56435}, {0, 0, 46608}, {0, 0, 54070}, {0, 546, 3821}, {0, 0, 52978}, {0, 0, 48792},
    {0, 0, 53523}, {0, 546, 3275}};
} // namespace shenmue::s2
