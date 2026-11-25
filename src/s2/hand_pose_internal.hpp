#pragma once

#include "s2/model_component.hpp"
#include "s2/hand.hpp"
#include "s2/hand_pose_data.hpp"

namespace shenmue::s2::model_component_detail {

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

// PC v1.07's borrowed idle script: shape 0, wait 120, shape 3, repeat.
// Slots (including arguments) are eight bytes; both hands borrow this same immutable storage.
alignas(8) inline constexpr std::array<std::int64_t, 10> mhndIdlePoseScript{11, 0, 15, 6, 120, 11, 3, 15, 5, 0};

inline constexpr MhndHandNodes mhndRightHandNodes{.hand = 0xffbf, .first = 0x19, .firstTip = 0x1a, .second = 0x1c, .secondTip = 0x1d, .third = 0x1f, .thirdTip = 0x20};

inline constexpr MhndHandNodes mhndLeftHandNodes{.hand = 0xffbe, .first = 0x28, .firstTip = 0x1a, .second = 0x2b, .secondTip = 0x2c, .third = 0x2e, .thirdTip = 0x2f};

} // namespace shenmue::s2::model_component_detail
