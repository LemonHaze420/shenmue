#pragma once

#include "s2/model_component.hpp"
#include "s2/hand.hpp"

namespace shenmue::s2::model_component_detail {

struct MhndHandShape {
    std::array<Vector3i, 4> right;
    std::array<Vector3i, 4> left;
};

// MT7 node ids of a hand, its three-axis first joint and tip, and two two-axis joints with their tips.
// Both hands search their first joint for tip 0x1a.
struct MhndHandNodes {
    std::uint16_t hand;
    std::uint16_t first;
    std::uint16_t firstTip;
    std::uint16_t second;
    std::uint16_t secondTip;
    std::uint16_t third;
    std::uint16_t thirdTip;
};

inline constexpr shen_name kHandPoseToken{SHEN_NAME(MHND)};



alignas(8) inline constexpr std::array<std::int64_t, 10> mhndIdlePoseScript{11, 0, 15, 6, 120, 11, 3, 15, 5, 0};

inline constexpr MhndHandNodes mhndRightHandNodes{.hand = 0xffbf, .first = 0x19, .firstTip = 0x1a, .second = 0x1c, .secondTip = 0x1d, .third = 0x1f, .thirdTip = 0x20};

inline constexpr MhndHandNodes mhndLeftHandNodes{.hand = 0xffbe, .first = 0x28, .firstTip = 0x1a, .second = 0x2b, .secondTip = 0x2c, .third = 0x2e, .thirdTip = 0x2f};


inline constexpr std::array<MhndHandShape, 15> mhndHandShapes{{
    {.right = {{{0, 0, 0}, {0, 0, 0}, {12743, 0, 12743}, {12743, 0, 12743}}}, .left = {{{0, 0, 0}, {0, 0, 0}, {52793, 0, 52793}, {52793, 0, 52793}}}},
    {.right = {{{0, 0, 0}, {0, 0, 3640}, {0, 910, 0}, {0, 64626, 0}}}, .left = {{{0, 0, 0}, {0, 0, 3640}, {0, 910, 0}, {0, 64626, 0}}}},
    {.right = {{{0, 0, 0}, {0, 0, 2421}, {4241, 600, 4241}, {4241, 64936, 4241}}},
        .left = {{{0, 0, 0}, {0, 0, 2421}, {61295, 600, 61295}, {61295, 64936, 61295}}}},
    {.right = {{{0, 0, 0}, {0, 0, 1219}, {8501, 309, 8501}, {8501, 65227, 8501}}},
        .left = {{{0, 0, 0}, {0, 0, 1219}, {57035, 309, 57035}, {57035, 65227, 57035}}}},
    {.right = {{{0, 0, 0}, {0, 0, 3640}, {0, 910, 0}, {12743, 0, 12743}}}, .left = {{{0, 0, 0}, {0, 0, 3640}, {0, 910, 0}, {52793, 0, 52793}}}},
    {.right = {{{0, 49152, 0}, {0, 0, 3640}, {0, 910, 0}, {0, 64626, 0}}}, .left = {{{0, 16384, 0}, {0, 0, 3640}, {0, 910, 0}, {0, 64626, 0}}}},
    {.right = {{{0, 0, 0}, {0, 0, 2421}, {2730, 600, 4241}, {4241, 64936, 6371}}},
        .left = {{{0, 0, 0}, {0, 0, 2421}, {62806, 600, 61295}, {61295, 64936, 59165}}}},
    {.right = {{{0, 0, 0}, {0, 0, 0}, {8192, 910, 8192}, {0, 0, 3640}}}, .left = {{{0, 0, 0}, {0, 0, 0}, {57344, 910, 57344}, {0, 0, 61896}}}},
    {.right = {{{0, 0, 0}, {0, 0, 1219}, {2730, 600, 4241}, {4241, 64936, 10012}}},
        .left = {{{0, 0, 0}, {0, 0, 1219}, {62806, 600, 61295}, {61295, 64936, 55524}}}},
    {.right = {{{0, 0, 0}, {0, 0, 1638}, {3640, 364, 3640}, {3640, 65172, 7281}}},
        .left = {{{0, 0, 0}, {0, 0, 1638}, {61896, 364, 61896}, {61896, 65172, 58255}}}},
    {.right = {{{0, 0, 0}, {0, 0, 1820}, {4551, 364, 4551}, {4551, 65172, 10922}}},
        .left = {{{0, 0, 0}, {0, 0, 1820}, {60985, 364, 60985}, {60985, 65172, 54614}}}},
    {.right = {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}}, .left = {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}}},
    {.right = {{{0, 16384, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}}, .left = {{{0, 49152, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}}},
    {.right = {{{0, 0, 0}, {0, 0, 3640}, {6371, 1820, 4369}, {6371, 0, 5461}}}, .left = {{{0, 0, 0}, {0, 0, 3640}, {59165, 1820, 61167}, {59165, 0, 60075}}}},
    {.right = {{{0, 61896, 0}, {0, 0, 0}, {12743, 910, 12743}, {12743, 64626, 12743}}},
        .left = {{{0, 3640, 0}, {0, 0, 0}, {52793, 910, 52793}, {52793, 64626, 52793}}}},
}};

} // namespace shenmue::s2::model_component_detail
