#pragma once
#include "s2/areas/area_knmn.hpp"
#include "common/math.hpp"
namespace shenmue::s2::areas {
inline constexpr Vector3 KNMN_PLAYER_PRACTICE_POSITION{.x = 41.08000183105469f, .y = 102.25f, .z = 241.04000854492188f};
inline constexpr Vector3 KNMN_TEACHER_PRACTICE_POSITION{.x = 40.20000076293945f, .y = 102.25f, .z = 242.6999969482422f};
inline constexpr Vector3 KNMN_DUMMY_PRACTICE_POSITION{.x = 37.900001525878906f, .y = 102.25f, .z = 241};
inline constexpr Vector4 KNMN_PRACTICE_REFLECTOR{.x = 1, .y = .30000001192092896f, .z = .30000001192092896f, .w = .30000001192092896f};
extern const std::uint8_t knmnPracticeLcd[194];
void KnmnShowPracticeLcd(KnmnPracticeWork &);
void KnmnRestorePracticeCharacters();
bool KnmnNeedsFacingReset(std::int32_t practice);
void KnmnFeedbackOpening(CompiledSequenceWork &);
void KnmnFeedbackClosing();
void KnmnSelectFeedback(CompiledSequenceWork &, HLib::TaskCallback *);
void KnmnRepeatFeedbackSequence(void *);
void KnmnIdleAdviceSequence(void *);
void KnmnStayAndStrikeSequence(void *);
void KnmnRelaxShouldersSequence(void *);
void KnmnUseHandsSequence(void *);
void KnmnUseLegsSequence(void *);
void KnmnHandsSlowSequence(void *);
void KnmnHandsLegsTogetherSequence(void *);
void KnmnRelaxWithoutGestureSequence(void *);
void KnmnUnclassifiedFeedbackSequence(void *);
} // namespace shenmue::s2::areas
