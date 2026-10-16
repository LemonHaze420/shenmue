#pragma once
#include "s2/areas/area_anan.hpp"
#include "s2/anbd_event.hpp"
namespace shenmue::s2::areas {
struct AnanPlacement {
    shen_name character;
    Vector3 position;
    Vector3i rotation;
};
static_assert(sizeof(AnanPlacement) == 28);
inline constexpr shen_name ananCharacters[]{ANAN_RYO, ANAN_MAN, ANAN_BOARD, ANAN_STOOL, ANAN_INVALID};
inline constexpr shen_name ananAuthorCharacters[]{ANAN_RYO, ANAN_MAN, ANAN_STOOL, ANAN_INVALID};
inline constexpr AnanPlacement ananAngryPlacement[]{
    {ANAN_MAN, {-1.3f, 1.27f, .43f}, {0, 64989, 0}}, {ANAN_STOOL, {-1.35f, -.2f, .46f}, {0, 14745, 0}}, {ANAN_INVALID, {}, {}}};
inline constexpr AnanPlacement ananTimeOverPlacement[]{
    {ANAN_MAN, {-1.3f, 1.27f, .43f}, {0, 64989, 0}}, {ANAN_STOOL, {-1.35f, -.2f, .46f}, {0, 14745, 0}}, {ANAN_INVALID, {}, {}}};
inline constexpr AnanPlacement ananRetryPlacement[]{
    {ANAN_RYO, {.49f, 0, 2.4f}, {0, 13343, 0}}, {ANAN_MAN, {-.69f, 0, 2.4f}, {0, 46111, 0}}, {ANAN_INVALID, {}, {}}};
inline constexpr AnanPlacement ananExplanationPlacement[]{{ANAN_RYO, {0, 0, 1.92f}, {0, 0, 0}}, {ANAN_MAN, {-1.37f, 1.27f, .69f}, {0, 65171, 0}},
    {ANAN_STOOL, {-1.35f, -.2f, .46f}, {0, 14745, 0}}, {ANAN_INVALID, {}, {}}};
inline constexpr AnanPlacement ananFirstEntryPlacement[]{{ANAN_RYO, {8.78f, -.1f, -3.56f}, {0, 24812, 0}}, {ANAN_MAN, {-.09f, 0, 2.38f}, {0, 10048, 0}},
    {ANAN_BOARD, {1.23f, 3.23f, 0}, {0, 0, 64444}}, {ANAN_STOOL, {-1.43f, -.2f, .46f}, {0, 14745, 0}}, {ANAN_INVALID, {}, {}}};
inline constexpr AnanPlacement ananRepeatEntryPlacement[]{{ANAN_RYO, {0, 0, 0}, {0, 0, 0}}, {ANAN_MAN, {0, 0, 2.44f}, {0, 12925, 0}},
    {ANAN_BOARD, {1.23f, 3.23f, 0}, {0, 0, 64444}}, {ANAN_STOOL, {-1.43f, -.2f, .46f}, {0, 14745, 0}}, {ANAN_INVALID, {}, {}}};
inline constexpr AnanPlacement ananDeclinePlacement[]{
    {ANAN_RYO, {2.14f, -.1f, 3.46f}, {0, 5461, 0}}, {ANAN_MAN, {0, 0, 2.44f}, {0, 49698, 0}}, {ANAN_INVALID, {}, {}}};
inline constexpr AnanPlacement ananFirstChoicePlacement[]{
    {ANAN_RYO, {2.14f, -.1f, 3.46f}, {0, 5461, 0}}, {ANAN_MAN, {0, 0, 2.44f}, {0, 12925, 0}}, {ANAN_INVALID, {}, {}}};
inline constexpr AnanPlacement ananStoppedPlacement[]{{ANAN_MAN, {-1.35f, 1.27f, .63f}, {0, 64989, 0}}, {ANAN_INVALID, {}, {}}};
inline constexpr AnanPlacement ananGamePlacement[]{{ANAN_RYO, {0, 0, 1.92f}, {0, 0, 0}}, {ANAN_MAN, {-1.35f, 1.27f, .43f}, {0, 64989, 0}},
    {ANAN_STOOL, {-1.35f, -.2f, .46f}, {0, 14745, 0}}, {ANAN_INVALID, {}, {}}};
inline constexpr std::uint8_t ananCameraPackageBytes[20]{0x41, 0x4e, 0x41, 0x4e, 0, 0, 0, 0, 0x30, 0x30, 0x32, 0x30, 0, 0, 0, 0, 0, 0, 0, 0};
inline constexpr Vector4 ananRefboard{1, 1, 1, 1};
inline constexpr shen_name ANAN_START_AUTHOR{SHEN_NAME(STRT)}, ANAN_ANGRY_AUTHOR{SHEN_NAME(ANGR)};
inline constexpr shen_name ANAN_TIME_FAIL_AUTHOR{SHEN_NAME(TOF1)}, ANAN_TIME_END_AUTHOR{SHEN_NAME(TOF2)};
inline constexpr shen_name ANAN_SUCCESS_AUTHOR{SHEN_NAME(SUCC)}, ANAN_FAIL_AUTHOR{SHEN_NAME(FAIL)};
inline constexpr shen_name ANAN_FAIL_FIRST_AUTHOR{SHEN_NAME(FAL1)}, ANAN_FAIL_END_AUTHOR{SHEN_NAME(FAL2)};
inline constexpr shen_name ANAN_FIFTH_AUTHOR{SHEN_NAME(F5_1)}, ANAN_FIFTH_END_AUTHOR{SHEN_NAME(F5_2)};
void ApplyAnanPlacements(const AnanPlacement *);
void AnanSelectCamera(std::int32_t);
void AnanLoadPackage(const char *);
void AnanFilm(std::uint32_t, std::uint32_t, std::uint32_t);
} // namespace shenmue::s2::areas
