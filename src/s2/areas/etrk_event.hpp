#pragma once
#include "s2/areas/area_etrk.hpp"
namespace shenmue::s2::areas {
inline constexpr std::uint32_t ETRK_IDLE_MOTION = 0xf003, ETRK_TURN_MOTION = 0x2c41, ETRK_OPEN_MOTION = 0x2c42;
inline constexpr std::uint32_t ETRK_KEY_MOTION = 0x2c43, ETRK_WRONG_KEY_MOTION = 0x2c44, ETRK_MOTION_FLAGS = 0x40000000U;
void EtrkSetUp(EtrkWork &work);
void EtrkIntroduction(EtrkWork &work);
std::int32_t EtrkPrepareKeys(EtrkWork &work);
std::int32_t EtrkPollKeys(EtrkWork &work);
std::int32_t EtrkTryKey(EtrkWork &work);
void EtrkOpenBoard(EtrkWork &work);
void EtrkPrepareSwitches(EtrkWork &work);
std::int32_t EtrkPollSwitches(EtrkWork &work);
void EtrkPrepareExit(EtrkWork &work);
void EtrkExit();
} // namespace shenmue::s2::areas
