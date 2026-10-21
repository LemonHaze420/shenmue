#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/area_loop.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2::areas {
inline constexpr shen_name BRIB_AREA{SHEN_NAME(BRIB)}, BRIB_RYO{SHEN_NAME(RYO_)}, BRIB_REN{SHEN_NAME(REN_)}, BRIB_KUK{SHEN_NAME(KUK_)};
inline constexpr shen_name BRIB_WTE{SHEN_NAME(WTE_)};
inline constexpr shen_name BRIB_GREN{SHEN_NAME(GREN)}, BRIB_RYOS{SHEN_NAME(RYOS)}, BRIB_CAMERA{SHEN_NAME(cbmd)}, BRIB_SENSE{SHEN_NAME(SENS)};
inline constexpr shen_name BRIB_CAMERA_TARGET{SHEN_NAME(camd)};
inline constexpr shen_name BRIB_EMPTY{0};
inline constexpr shen_name BRIB_REN_CHARACTERS[]{BRIB_REN, BRIB_KUK, BRIB_EMPTY};
inline constexpr shen_name BRIB_SENSE_CHARACTERS[]{BRIB_REN, BRIB_KUK, BRIB_WTE, BRIB_EMPTY};
inline constexpr Vector3 BRIB_KUK_POSITION{301.2f, 128.8f, 72.56f}, BRIB_RYO_OFFSET{0.098f, 0, 1.083f}, BRIB_RYOS_OFFSET{0.939f, 0, -0.289f};
inline constexpr Vector3i BRIB_ROTATION{0, 49152, 0};
struct BribWork {
    HLib::Task *parent;
    std::uint32_t flags, reserved0c;
};
static_assert(sizeof(BribWork) == 0x10);
static_assert(offsetof(BribWork, flags) == 8);
void InitializeBribEvent();
void RunBribEvent(std::uintptr_t argument);
extern const ScriptAreaDefinition bribScriptDefinition;
} // namespace shenmue::s2::areas
