#pragma once
#include "s2/areas/area_mqte.hpp"
namespace shenmue::s2::areas {
struct ForestCameraWork {
    std::int32_t camera;
    shen_name eye, target;
    std::int32_t cleanup;
};
struct ForestRoadCamera {
    std::int32_t camera, frames;
};
struct ForestRoadCameraSequence {
    ForestRoadCamera cameras[5];
};
inline constexpr ForestRoadCameraSequence forestFirstRoadCameras[]{
    {{{-1, 60}}}, {{{-1, 60}}}, {{{-1, 60}}}, {{{-1, 60}}}, {{{-1, 60}}}, {{{-1, 60}}}, {{{-1, 60}}}, {{{-1, 60}}}, {{{-1, 60}}}, {{{-1, 60}}}, {{{6000, 60}}}};
inline constexpr ForestRoadCameraSequence forestSecondRoadCameras[]{
    {{{-1, 60}}}, {{{-1, 60}}}, {{{-1, 60}}}, {{{-1, 60}}}, {{{-1, 60}}}, {{{-1, 60}}}, {{{-1, 60}}}, {{{-1, 60}}}, {{{-1, 60}}}, {{{6100, 60}}}};
inline constexpr ForestRoadCameraSequence forestThirdRoadCameras[]{
    {{{-1, 60}}}, {{{-1, 60}}}, {{{-1, 60}}}, {{{-1, 60}}}, {{{-1, 60}}}, {{{-1, 60}}}, {{{-1, 60}}}, {{{-1, 60}}}, {{{-1, 60}}}, {{{6200, 60}}}};
inline constexpr ForestRoadCameraSequence forestOtherRoadCameras[]{{{{-1, 60}, {6000, 0}}}, {{{6010, 60}, {-1, 0}}}, {{{-1, 60}, {6020, 0}}}, {{{-1, 60}}},
    {{{-1, 65}, {6040, 0}}}, {{{-1, 115}, {6050, 0}}}, {{{-1, 60}}}, {{{-1, 70}, {6070, 0}}}, {{{-1, 40}, {6080, 60}, {-1, 80}, {6081, 0}}},
    {{{-1, 65}, {6090, 0}}}, {{{6100, 100}, {-1, 100}, {6101, 0}}}};
inline constexpr std::int32_t forestRandomRoadCameras[]{5000, 5001, 5003, 5004, 5500, 5501, -1};
inline constexpr std::int32_t forestRyoCameras[18][4]{{2000, 2001, 2002, 2003}, {2000, 2001, 2002, 2003}, {2000, 2001, 2002, 2003}, {2000, 2001, 2002, 2003},
    {2000, 2001, 2002, 2003}, {2000, 2001, 2002, 2003}, {2000, 2001, 2002, 2003}, {2050, -1, -1, 2003}, {2000, 2001, 2002, 2003}, {2000, 2001, 2002, 2003},
    {2000, 2001, 2002, 2003}, {2000, 2001, 2002, 2003}, {2000, 2001, 2002, 2003}, {2000, 2001, 2002, 2003}, {2000, 2001, 2002, 2003}, {2000, 2001, 2002, 2003},
    {2000, 2001, 2002, 2003}, {2000, 2001, 2002, 2003}};
inline constexpr std::int32_t forestShenfaCameras[18][4]{{2500, -1, 2500, 2500}, {2501, -1, 2501, 2501}, {2502, -1, 2500, 2500}, {2503, -1, 2500, 2500},
    {2504, -1, 2500, 2500}, {2505, -1, 2500, 2500}, {2506, -1, 2500, 2500}, {2507, -1, 2500, 2500}, {2508, -1, 2500, 2500}, {2509, -1, 2500, 2500},
    {2510, -1, 2500, 2500}, {2511, -1, 2500, 2500}, {2512, -1, 2500, 2500}, {2513, -1, 2500, 2500}, {2514, -1, 2500, 2500}, {2515, -1, 2500, 2500},
    {2516, -1, 2500, 2500}, {2517, -1, 2500, 2500}};
inline constexpr std::int32_t forestFireflyTiming[4][4]{{4, 120, 5, 3}, {4, 150, 7, 3}, {4, 200, 10, 3}, {4, 300, 30, 3}};
inline constexpr shen_name forestAuthorFireflies[]{SHEN_NAME(a850), SHEN_NAME(a852), SHEN_NAME(a856)};
} // namespace shenmue::s2::areas
