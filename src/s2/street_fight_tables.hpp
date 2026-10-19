#pragma once

#include "s2/street_fight.hpp"

namespace shenmue::s2 {

inline constexpr std::int16_t STREET_FIGHT_PRIZES[12]{0, 20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 0};
inline constexpr char STREET_FIGHT_DEBUG_ACCEPT[] = "\x97\xc1\x81\x75\x82\xa0\x82\xa0\x81\x42\x81\x76";
inline constexpr char STREET_FIGHT_DEBUG_WAIT[] = "\x93\xb7\x8c\xb3\x81\x75\x82\xe6\x82\xb5\x91\xd2\x82\xc1\x82\xc4\x82\xc8\x81\x42\x81\x76";
inline constexpr char STREET_FIGHT_DEBUG_INVITATION[] =
    "\x93\xb7\x8c\xb3\x81\x75\x82\xb3\x82\xa0\x82\xb3\x82\xa0\x81\x41\x82\xb1\x82\xcc\x8c\x5a\x82\xbf\x82\xe1\x82\xf1"
    "\x81\x41\x81\x95\x82\xdc\x82\xbe\x8e\xe1\x82\xa6\x82\xaa\x81\x41\x82\xaf\x82\xc1\x82\xb1\x82\xa4\x82\xc8\x83\x63"
    "\x83\x8f\x83\x82\x83\x6d\x82\xbe\x81\x42\x81\x76";
inline constexpr char STREET_FIGHT_DEBUG_CHALLENGE[] =
    "\x93\xb7\x8c\xb3\x81\x75\x82\xb3\x82\xa0\x92\x4e\x82\xa9\x81\x41\x82\xb1\x82\xa2\x82\xc2\x82\xc6\x8f\x9f\x95\x89"
    "\x82\xb7\x82\xe9\x82\xe2\x82\xc2\x82\xcd\x82\xa2\x82\xc8\x82\xa2\x82\xa9\x81\x48\x81\x76";
inline constexpr char STREET_FIGHT_DEBUG_CALL[] = "\x93\xb7\x8c\xb3\x81\x75\x82\xb3\x82\xa0\x92\x4e\x82\xa9\x81\x49\x81\x49\x81\x76";
inline constexpr char STREET_FIGHT_DEBUG_NO_OPPONENT[] =
    "\x93\xb7\x8c\xb3\x81\x75\x81\x45\x81\x45\x81\x45\x82\xc1\x82\xc6\x81\x41\x82\xa2\x82\xcb\x82\xa6\x82\xe6\x82\xa4"
    "\x82\xbe\x82\xc8\x81\x42\x81\x76";
inline constexpr char STREET_FIGHT_DEBUG_PRIZE[] =
    "\x93\xb7\x8c\xb3\x81\x75\x82\xd9\x82\xe7\x81\x41\x8c\x5a\x82\xbf\x82\xe1\x82\xf1\x81\x41\x8f\xdc\x8b\xe0\x82\xbe"
    "\x81\x42\x81\x76";
inline constexpr char STREET_FIGHT_DEBUG_STOP[] =
    "\x97\xc1\x81\x75\x82\xa2\x82\xe2\x81\x41\x82\xe0\x82\xa4\x82\xe6\x82\xb5\x82\xc6\x82\xad\x82\xe6\x81\x42\x81\x76";
inline constexpr char STREET_FIGHT_DEBUG_AGREEMENT[] = "\x93\xb7\x8c\xb3\x81\x75\x82\xbb\x82\xa4\x82\xa9\x81\x45\x81\x45\x81\x45\x81\x42\x81\x76";
inline constexpr char STREET_FIGHT_DEBUG_STOP_PRIZE[] =
    "\x93\xb7\x8c\xb3\x81\x75\x82\xb5\x82\xe1\x82\xa0\x82\xcb\x82\xa6\x81\x41\x82\xd9\x82\xe7\x81\x41\x8f\xdc\x8b\xe0"
    "\x82\xbe\x81\x42\x81\x76";
inline constexpr char STREET_FIGHT_DEBUG_OPPONENT_ACCEPT[] = "\x93\x47\x81\x75\x83\x8f\x83\x56\x82\xaa\x82\xe2\x82\xeb\x82\xa4\x81\x42\x81\x76";
inline constexpr char STREET_FIGHT_DEBUG_VICTORY[] =
    "\x93\xb7\x8c\xb3\x81\x75\x8c\x5a\x82\xbf\x82\xe1\x82\xf1\x81\x41\x82\xe2\x82\xe9\x82\xcb\x82\xa6\x81\x42\x81\x76";
inline constexpr char STREET_FIGHT_DEBUG_NEXT_OPPONENT[] =
    "\x93\xb7\x8c\xb3\x81\x75\x82\xdc\x82\xbe\x82\xdc\x82\xbe\x83\x43\x83\x50\x82\xbb\x82\xa4\x82\xbe\x82\xc8\x81\x42"
    "\x81\x76";
inline constexpr char STREET_FIGHT_DEBUG_REMATCH[] =
    "\x93\xb7\x8c\xb3\x81\x75\x82\xe0\x82\xa4\x82\xd0\x82\xc6\x8f\x9f\x95\x89\x81\x41\x8e\xf3\x82\xaf\x82\xcb\x82\xa6"
    "\x82\xa9\x81\x48\x81\x76";
inline constexpr char STREET_FIGHT_DEBUG_PRIZE_40[] =
    "\x93\xb7\x8c\xb3\x81\x75\x8f\x9f\x82\xc1\x82\xbd\x82\xe7\x82\x53\x82\x4f\x83\x68\x83\x8b\x8f\x6f\x82\xb7\x82\xba"
    "\x81\x42\x81\x76";
inline constexpr char STREET_FIGHT_DEBUG_PRIZE_60[] =
    "\x93\xb7\x8c\xb3\x81\x75\x8f\x9f\x82\xc1\x82\xbd\x82\xe7\x82\x55\x82\x4f\x83\x68\x83\x8b\x8f\x6f\x82\xb7\x82\xba"
    "\x81\x42\x81\x76";
inline constexpr char STREET_FIGHT_DEBUG_PRIZE_80[] =
    "\x93\xb7\x8c\xb3\x81\x75\x8f\x9f\x82\xc1\x82\xbd\x82\xe7\x82\x57\x82\x4f\x83\x68\x83\x8b\x8f\x6f\x82\xb7\x82\xba"
    "\x81\x42\x81\x76";
inline constexpr char STREET_FIGHT_DEBUG_PRIZE_100[] =
    "\x93\xb7\x8c\xb3\x81\x75\x8f\x9f\x82\xc1\x82\xbd\x82\xe7\x82\x50\x82\x4f\x82\x4f\x83\x68\x83\x8b\x8f\x6f\x82\xb7"
    "\x82\xba\x81\x42\x81\x76";
inline constexpr char STREET_FIGHT_DEBUG_PRIZE_120[] =
    "\x93\xb7\x8c\xb3\x81\x75\x8f\x9f\x82\xc1\x82\xbd\x82\xe7\x82\x50\x82\x51\x82\x4f\x83\x68\x83\x8b\x8f\x6f\x82\xb7"
    "\x82\xba\x81\x42\x81\x76";
inline constexpr shen_name STREET_FIGHT_AB00_AUDIENCE[]{SHEN_NAME(SD5_), SHEN_NAME(SD6_), SHEN_NAME(SD7_), SHEN_NAME(SHS_), SHEN_NAME(WD2_), SHEN_NAME(OD2_),
    SHEN_NAME(RE1_), SHEN_NAME(VCN_), SHEN_NAME(CHN_), SHEN_NAME(BKY_), SHEN_NAME(BRT_), SHEN_NAME(BZH_), SHEN_NAME(CHK_), SHEN_NAME(CJU_), SHEN_NAME(COH_),
    SHEN_NAME(CRY_), SHEN_NAME(ENC_), SHEN_NAME(ENT_), SHEN_NAME(GYE_), SHEN_NAME(H02Q), SHEN_NAME(H03Q), SHEN_NAME(H04Q), SHEN_NAME(H05Q), SHEN_NAME(H07Q),
    SHEN_NAME(H08Q), SHEN_NAME(H09Q), SHEN_NAME(H10Q), SHEN_NAME(H11Q), SHEN_NAME(H12Q), SHEN_NAME(H13Q), SHEN_NAME(H14Q), SHEN_NAME(H15Q), SHEN_NAME(H16Q),
    SHEN_NAME(H17Q), SHEN_NAME(H18Q), SHEN_NAME(H19Q), SHEN_NAME(H20Q), SHEN_NAME(HNN_), SHEN_NAME(HUR_), SHEN_NAME(INC_), SHEN_NAME(KEI_), SHEN_NAME(KIK_),
    SHEN_NAME(KIN_), SHEN_NAME(KKY_), SHEN_NAME(KMY_), SHEN_NAME(KSN_), SHEN_NAME(KUK_), SHEN_NAME(LCN_), SHEN_NAME(JUK_), SHEN_NAME(07C_), SHEN_NAME(08G_),
    SHEN_NAME(M01Q), SHEN_NAME(M02Q), SHEN_NAME(M03Q), SHEN_NAME(M04Q), SHEN_NAME(M05Q), SHEN_NAME(M06Q), SHEN_NAME(M07Q), SHEN_NAME(M08Q), SHEN_NAME(M09Q),
    SHEN_NAME(P01Q), SHEN_NAME(P02Q), SHEN_NAME(P03Q), SHEN_NAME(P04Q), SHEN_NAME(P05Q), SHEN_NAME(P06Q), SHEN_NAME(P07Q), SHEN_NAME(P08Q), SHEN_NAME(P09Q),
    SHEN_NAME(P10Q), SHEN_NAME(Y01Q), SHEN_NAME(Y03Q), SHEN_NAME(Y04Q), SHEN_NAME(Y05Q), SHEN_NAME(Y07Q), SHEN_NAME(Y08Q), SHEN_NAME(Y09Q), SHEN_NAME(Y10Q),
    SHEN_NAME(A04Q), SHEN_NAME(A05Q), SHEN_NAME(A06Q), SHEN_NAME(MAK_), SHEN_NAME(KEN_), SHEN_NAME(ONI_), SHEN_NAME(SAM_), SHEN_NAME(LLY_), SHEN_NAME(CLJ_),
    STREET_FIGHT_INVALID};

inline constexpr shen_name STREET_FIGHT_QSFB_AUDIENCE[]{SHEN_NAME(CYC_), SHEN_NAME(CHN_), SHEN_NAME(A04Q), SHEN_NAME(A05Q), SHEN_NAME(H01Q), SHEN_NAME(H02Q),
    SHEN_NAME(H03Q), SHEN_NAME(H04Q), SHEN_NAME(H05Q), SHEN_NAME(H06Q), SHEN_NAME(H07Q), SHEN_NAME(H08Q), SHEN_NAME(H09Q), SHEN_NAME(H10Q), SHEN_NAME(H11Q),
    SHEN_NAME(H12Q), SHEN_NAME(H13Q), SHEN_NAME(H15Q), SHEN_NAME(H17Q), SHEN_NAME(H19Q), SHEN_NAME(H20Q), SHEN_NAME(M01Q), SHEN_NAME(M03Q), SHEN_NAME(M04Q),
    SHEN_NAME(M05Q), SHEN_NAME(M06Q), SHEN_NAME(M07Q), SHEN_NAME(M08Q), SHEN_NAME(M09Q), SHEN_NAME(P01Q), SHEN_NAME(P02Q), SHEN_NAME(P03Q), SHEN_NAME(P04Q),
    SHEN_NAME(P05Q), SHEN_NAME(Y01Q), SHEN_NAME(Y03Q), SHEN_NAME(Y04Q), SHEN_NAME(Y07Q), SHEN_NAME(Y08Q), SHEN_NAME(Y09Q), SHEN_NAME(Y10Q), SHEN_NAME(SAM_),
    SHEN_NAME(LLY_), SHEN_NAME(CLJ_), STREET_FIGHT_INVALID};

inline constexpr shen_name STREET_FIGHT_QGBT_AUDIENCE[]{SHEN_NAME(EEN_), SHEN_NAME(REN_), SHEN_NAME(CYC_), SHEN_NAME(CHN_), SHEN_NAME(A01Q), SHEN_NAME(H14Q),
    SHEN_NAME(HUR_), SHEN_NAME(M01Q), SHEN_NAME(M02Q), SHEN_NAME(M03Q), SHEN_NAME(M05Q), SHEN_NAME(M07Q), SHEN_NAME(M09Q), SHEN_NAME(P02Q), SHEN_NAME(P03Q),
    SHEN_NAME(P04Q), SHEN_NAME(P05Q), SHEN_NAME(P08Q), SHEN_NAME(SYC_), SHEN_NAME(Y06Q), STREET_FIGHT_INVALID};

inline constexpr std::uint16_t STREET_FIGHT_ARRIVAL_SOUNDS[]{0x974, 0x975, 0x976, 0x977, 0x978, 0x979, 0x97a};
inline constexpr std::uint16_t STREET_FIGHT_OPENING_SOUNDS[]{0x97b, 0x97c, 0x97d, 0x97e};
inline constexpr std::uint16_t STREET_FIGHT_DEFEAT_SOUNDS[]{0x999, 0x99a, 0x99b, 0x99c};
inline constexpr std::uint16_t STREET_FIGHT_VICTORY_SOUNDS[]{0x99d, 0x99e, 0x99f, 0x9a0, 0x9a1};
inline constexpr std::uint16_t STREET_FIGHT_BATTLE_SOUNDS[]{0x981, 0x982, 0x983, 0x984, 0x985, 0x986, 0x987, 0x988, 0x989, 0x98a, 0x98b, 0x98c, 0x98d, 0x98e,
    0x98f, 0x990, 0x991, 0x992, 0x993, 0x994, 0x995, 0x996, 0x997, 0x998};

} // namespace shenmue::s2
