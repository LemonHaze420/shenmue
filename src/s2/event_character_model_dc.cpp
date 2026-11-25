#include "s2/event_character_model_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/face_controller_dc.hpp"
#include "s2/cloth_dc.hpp"
#include "s2/osage_dc.hpp"
#include "s2/hand_pose_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/model_image_component_dc.hpp"
#include "s2/motion_node_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/prop_dc.hpp"
#include "s2/scenario_dc.hpp"

namespace shenmue {
namespace s2 {

void ResetImageModule(Character *character);

} // namespace s2
} // namespace shenmue

extern "C" {

void __attribute__((renesas)) shenmue_s2_event_EV_SetupImageModule(std::uint32_t name, const char *imageName) {
    shenmue::s2::EV_SetupImageModule(shenmue::shen_name(name), imageName);
}

bool __attribute__((renesas)) shenmue_s2_event_IsFacialReplacementSelected(const shenmue::s2::Character *character) {
    return shenmue::s2::IsFacialReplacementSelected(character);
}

void __attribute__((renesas)) shenmue_s2_event_SelectCharacterFacialNode(shenmue::s2::Character *character, std::int32_t replacement) {
    shenmue::s2::SelectCharacterFacialNode(character, replacement);
}

void __attribute__((renesas)) shenmue_s2_event_SetScenarioCharacterType(std::uint32_t name, std::int32_t type) {
    shenmue::s2::SetScenarioCharacterType(shenmue::shen_name(name), type);
}

void __attribute__((renesas)) shenmue_s2_event_CopyScenarioNodeMotionTimes(std::uint32_t name, float *begin, float *end, float *frame, float *speed) {
    shenmue::s2::CopyScenarioNodeMotionTimes(shenmue::shen_name(name), begin, end, frame, speed);
}
}

#endif
