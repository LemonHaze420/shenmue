#include "s2/motion_work_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/motion_registry_dc.hpp"
#include <cmath>
#include <cstring>

namespace shenmue {
namespace s2 {

namespace {

// @verified - d
std::uint16_t ApplyMotionRegionFlag(MotionWork *work, std::uint32_t flag, std::int32_t operation, std::uint32_t region) {
    if (flag < 64)
        return 0;
    const std::uint32_t shift = flag - 64;
    std::uint16_t &flags = work->regionFlags[region];
    const std::uint16_t previous = flags & (1U << shift);
    if (operation == 0)
        flags |= static_cast<std::uint16_t>(1U << shift);
    else if (operation == 2)
        flags &= static_cast<std::uint16_t>(~(1U << shift));
    return previous;
}

} // namespace

// @verified - d
void mtwkGetRegionClocks(MotionWork *work, MotionRegionClock **lower, MotionRegionClock **chest, MotionRegionClock **head, MotionRegionClock **rightArm,
    MotionRegionClock **leftArm) {
    if (lower)
        *lower = &work->lowerBodyClock;
    if (chest)
        *chest = &work->chestClock;
    if (head)
        *head = &work->headClock;
    if (rightArm)
        *rightArm = &work->rightArmClock;
    if (leftArm)
        *leftArm = &work->leftArmClock;
}

// @verified - d
std::uint32_t mtwkSetRegionFlag(MotionWork *work, std::uint32_t flag, std::int32_t operation, MotionRegion region) {
    std::uint16_t result = 0;
    if (!work)
        return 0;
    const std::uint32_t index = static_cast<std::uint32_t>(region);
    switch (flag) {
    case 66:
        if (!operation)
            result = ApplyMotionRegionFlag(work, 76, 0, index);
        else
            result = ApplyMotionRegionFlag(work, flag, operation, index);
        break;
    case 69:
    case 71:
    case 73:
    case 74:
    case 76:
        result = ApplyMotionRegionFlag(work, flag, operation, index);
        break;
    case 72:
        operation = 1;
        if (!index) {
            for (std::int32_t target = 1; target < 5; ++target)
                result |= ApplyMotionRegionFlag(work, flag, operation, target);
            return result;
        }
        result = ApplyMotionRegionFlag(work, flag, operation, index);
        break;
    case 70:
        result = work->regionFlags[index] & 0x40U;
        if (MOTN_GetLeadFootFlag(work->currentMotionId[index]))
            result ^= 0x40U;
        break;
    default:
        operation = 1;
        result = ApplyMotionRegionFlag(work, flag, operation, index);
        break;
    }
    return result;
}

} // namespace s2
} // namespace shenmue

#endif
