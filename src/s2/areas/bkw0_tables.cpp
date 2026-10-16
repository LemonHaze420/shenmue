#include "s2/areas/bkw0_sequence.hpp"
#include "s2/areas/bkw0_tables.hpp"
#include "s2/areas/area_arbt.hpp"
#include "s2/event_runtime.hpp"
#include "s2/save.hpp"
namespace shenmue::s2::areas {
void Bkw0ResetPracticeOpponentAngle() {
    const Vector3i rotation{0, arbtCrateRotation.yaw, 0};
    EV_AT_SetAngY(BKW0_LKN, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
}
void Bkw0ShowPracticeLcd(Bkw0Work &work) {
    work.lcdFrames = 600;
    work.lcdBitmap = BKW0_PRACTICE_LCD;
    LcdmSelectMessage(1);
    LcdmInit(reinterpret_cast<std::uintptr_t>(work.lcdBitmap));
}
} // namespace shenmue::s2::areas
