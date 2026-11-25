#pragma once

#include "common/math.hpp"
#if !SHENMUE_IS_DC
#include <array>
#endif

namespace shenmue { namespace s2 { namespace model_component_detail {

#if SHENMUE_IS_DC
struct HandPoseJointArray { Vector3i values[4]; };
struct MhndHandShape { HandPoseJointArray right, left; };
#else
struct MhndHandShape {
    std::array<Vector3i, 4> right;
    std::array<Vector3i, 4> left;
};
#endif

#if SHENMUE_IS_DC
static const MhndHandShape mhndHandShapes[15] = {
#else
inline constexpr std::array<MhndHandShape, 15> mhndHandShapes{{
#endif
    {SHEN_FIELD(right) {{{0, 0, 0}, {0, 0, 0}, {12743, 0, 12743}, {12743, 0, 12743}}}, SHEN_FIELD(left) {{{0, 0, 0}, {0, 0, 0}, {52793, 0, 52793}, {52793, 0, 52793}}}},
    {SHEN_FIELD(right) {{{0, 0, 0}, {0, 0, 3640}, {0, 910, 0}, {0, 64626, 0}}}, SHEN_FIELD(left) {{{0, 0, 0}, {0, 0, 3640}, {0, 910, 0}, {0, 64626, 0}}}},
    {SHEN_FIELD(right) {{{0, 0, 0}, {0, 0, 2421}, {4241, 600, 4241}, {4241, 64936, 4241}}},
        SHEN_FIELD(left) {{{0, 0, 0}, {0, 0, 2421}, {61295, 600, 61295}, {61295, 64936, 61295}}}},
    {SHEN_FIELD(right) {{{0, 0, 0}, {0, 0, 1219}, {8501, 309, 8501}, {8501, 65227, 8501}}},
        SHEN_FIELD(left) {{{0, 0, 0}, {0, 0, 1219}, {57035, 309, 57035}, {57035, 65227, 57035}}}},
    {SHEN_FIELD(right) {{{0, 0, 0}, {0, 0, 3640}, {0, 910, 0}, {12743, 0, 12743}}}, SHEN_FIELD(left) {{{0, 0, 0}, {0, 0, 3640}, {0, 910, 0}, {52793, 0, 52793}}}},
    {SHEN_FIELD(right) {{{0, 49152, 0}, {0, 0, 3640}, {0, 910, 0}, {0, 64626, 0}}}, SHEN_FIELD(left) {{{0, 16384, 0}, {0, 0, 3640}, {0, 910, 0}, {0, 64626, 0}}}},
    {SHEN_FIELD(right) {{{0, 0, 0}, {0, 0, 2421}, {2730, 600, 4241}, {4241, 64936, 6371}}},
        SHEN_FIELD(left) {{{0, 0, 0}, {0, 0, 2421}, {62806, 600, 61295}, {61295, 64936, 59165}}}},
    {SHEN_FIELD(right) {{{0, 0, 0}, {0, 0, 0}, {8192, 910, 8192}, {0, 0, 3640}}}, SHEN_FIELD(left) {{{0, 0, 0}, {0, 0, 0}, {57344, 910, 57344}, {0, 0, 61896}}}},
    {SHEN_FIELD(right) {{{0, 0, 0}, {0, 0, 1219}, {2730, 600, 4241}, {4241, 64936, 10012}}},
        SHEN_FIELD(left) {{{0, 0, 0}, {0, 0, 1219}, {62806, 600, 61295}, {61295, 64936, 55524}}}},
    {SHEN_FIELD(right) {{{0, 0, 0}, {0, 0, 1638}, {3640, 364, 3640}, {3640, 65172, 7281}}},
        SHEN_FIELD(left) {{{0, 0, 0}, {0, 0, 1638}, {61896, 364, 61896}, {61896, 65172, 58255}}}},
    {SHEN_FIELD(right) {{{0, 0, 0}, {0, 0, 1820}, {4551, 364, 4551}, {4551, 65172, 10922}}},
        SHEN_FIELD(left) {{{0, 0, 0}, {0, 0, 1820}, {60985, 364, 60985}, {60985, 65172, 54614}}}},
    {SHEN_FIELD(right) {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}}, SHEN_FIELD(left) {{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}}},
    {SHEN_FIELD(right) {{{0, 16384, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}}, SHEN_FIELD(left) {{{0, 49152, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}}},
    {SHEN_FIELD(right) {{{0, 0, 0}, {0, 0, 3640}, {6371, 1820, 4369}, {6371, 0, 5461}}}, SHEN_FIELD(left) {{{0, 0, 0}, {0, 0, 3640}, {59165, 1820, 61167}, {59165, 0, 60075}}}},
    {SHEN_FIELD(right) {{{0, 61896, 0}, {0, 0, 0}, {12743, 910, 12743}, {12743, 64626, 12743}}},
        SHEN_FIELD(left) {{{0, 3640, 0}, {0, 0, 0}, {52793, 910, 52793}, {52793, 64626, 52793}}}},
#if SHENMUE_IS_DC
};
#else
}};
#endif

} } } // namespace shenmue::s2::model_component_detail
