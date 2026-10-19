#pragma once
#include "s2/areas/qes2_event.hpp"
#include "s2/event_characters.hpp"
namespace shenmue::s2 {
inline constexpr shen_name QES2_AREA{SHEN_NAME(QES2)}, QES2_CHAINSAW{SHEN_NAME(CHSA)}, QES2_TIN{SHEN_NAME(ITTO)};
inline constexpr shen_name QES2_SCX{SHEN_NAME(SCX_)}, QES2_SBO{SHEN_NAME(SBO_)}, QES2_KU5{SHEN_NAME(KU5_)};
inline constexpr shen_name QES2_RKN{SHEN_NAME(RKN_)}, QES2_MRC{SHEN_NAME(MRC_)}, QES2_KK4{SHEN_NAME(KK4_)};
inline constexpr shen_name QES2_SHOH{SHEN_NAME(SHOH)}, QES2_SHOK{SHEN_NAME(SHOK)}, QES2_END_AUTHOR{SHEN_NAME(END_)};
inline constexpr shen_name QES2_TB2{SHEN_NAME(TB2_)}, QES2_SD7{SHEN_NAME(SD7_)}, QES2_WD4{SHEN_NAME(WD4_)};
inline constexpr shen_name QES2_AD4{SHEN_NAME(AD4_)}, QES2_KD7{SHEN_NAME(KD7_)};
inline constexpr shen_name QES2_DRAWERS[]{SHEN_NAME(DRA0), SHEN_NAME(DRA1), SHEN_NAME(DRA2), SHEN_NAME(DRA3), SHEN_NAME(DRA4)};
inline constexpr shen_name QES2_READ_CHARACTERS[]{QES_RYO, QES_REN, QES_YUA, QES_INVALID};
inline constexpr shen_name QES2_FIRST_PAIR_CHARACTERS[]{QES2_SCX, QES2_SBO, QES2_KU5, QES_INVALID};
inline constexpr shen_name QES2_SECOND_PAIR_CHARACTERS[]{QES2_RKN, QES2_MRC, QES2_KK4, QES_INVALID};
inline constexpr shen_name QES2_SURROUND_CHARACTERS[]{QES2_TB2, QES2_SD7, QES2_WD4, QES2_AD4, QES2_KD7, QES_INVALID};
inline constexpr std::int32_t QES2_TIN_PARTS[]{198, 181, 187, 192, 193, 194, 191, 196, 197, 195, 182, 184, 183, 185, 186, 188, 189, 190, 180, 159, 155, 163,
    168, 172, 176, 179, 158, 162, 164, 154, 167, 171, 175, 177, 152, 160, 156, 165, 169, 173, 178, 153, 161, 157, 166, 174, 170};
inline constexpr Vector3 QES2_INITIAL_RYO{-0.699999988079071f, 0, -7.800000190734863f};
inline constexpr Vector3 QES2_INITIAL_REN{0.4000000059604645f, 0, -11.800000190734863f};
inline constexpr EventMovementWaypoint QES2_FLOORBOARD_POSES[]{{.position = {224.5f, 0, 151.10000610351562f}, .yawDegrees = 157.5f},
    {.position = {225.1999969482422f, 0, 149}, .yawDegrees = 170}, {.position = {219.8000030517578f, 0, 170}, .yawDegrees = 135},
    {.position = {218.60000610351562f, 0, 172.5f}, .yawDegrees = 112.5f}};
inline constexpr Vector3 QES2_TIN_POSITION{4.800000190734863f, 0, -2.4000000953674316f};
inline constexpr Vector3 QES2_TIN_REN_POSITION{1.2000000476837158f, 0, -2.4000000953674316f};
inline constexpr Vector3 QES2_TIN_CLEAR_REN{0.5f, 0, -1.7000000476837158f};
inline constexpr Vector3 QES2_EXTINGUISHER_POSITION{-1.399999976158142f, 0.699999988079071f, 15.699999809265137f};
inline constexpr Vector3 QES2_SHOP_REN_POSITION{-0.4000000059604645f, 0, 17.5f}, QES2_SHOP_RYO_POSITION{-0.10000000149011612f, 0, 1};
inline constexpr Vector3 QES2_SURROUND_RYO{-0.6000000238418579f, 0, 13}, QES2_SURROUND_REN{0.800000011920929f, 0, 14.5f};
inline constexpr Vector3 QES2_SURROUND_WD4{-17.200000762939453f, 0, 18.200000762939453f}, QES2_SURROUND_AD4{-17, 0, 17};
inline constexpr Vector3 QES2_SURROUND_KD7{-15.899999618530273f, 0, 18.899999618530273f};
inline constexpr Vector3 QES2_DRAWER_FAILURE_POSITIONS[]{{0.6000000238418579f, 0, 1.7000000476837158f}, {-0.6000000238418579f, 0, 1.7000000476837158f}};
inline constexpr Vector3 QES2_TIN_FAILURE_RYO{3, 0, -3.5f}, QES2_SHOP_SUCCESS_RYO{-0.699999988079071f, 0, 13.100000381469727f};
inline constexpr Vector3 QES2_SHOP_SUCCESS_YUA{-0.4000000059604645f, 0, -1.100000023841858f};
inline constexpr Vector3 QES2_CLEANUP_RYO{241.39999389648438f, 157.60000610351562f, 126.4000015258789f};

struct Qes2EventWork {
    HLib::Task *task;
    std::int16_t phase, subphase;
    std::uint32_t reserved0c;
    EventReadCharactersWork *characters;
    std::int32_t qteElapsed;
    std::uint32_t reserved1c;
    std::uint16_t qteStatus, reserved22;
    std::int32_t qteSucceeded;
};
struct Qes2SmokeWork {
    std::int32_t frames, phase;
    std::uint32_t effect;
};
static_assert(sizeof(Qes2EventWork) == 0x28 && offsetof(Qes2EventWork, characters) == 0x10 && offsetof(Qes2EventWork, qteElapsed) == 0x18);
static_assert(offsetof(Qes2EventWork, qteStatus) == 0x20 && offsetof(Qes2EventWork, qteSucceeded) == 0x24 && sizeof(Qes2SmokeWork) == 0x0c);
void Qes2Init();
void Qes2Main(std::uintptr_t argument);
void Qes2Cleanup(void *);
extern const ScriptAreaDefinition qes2Definition;
} // namespace shenmue::s2
