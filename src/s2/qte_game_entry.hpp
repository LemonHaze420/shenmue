#pragma once

#include "s2/hlib.hpp"
#include "common/math.hpp"
#include "common/types.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

inline constexpr shen_name QTE_GAME_ENTRY_TASK{SHEN_NAME(EVNT)}, QTE_GAME_TITLE{SHEN_NAME(TITL)}, QTE_GAME_MACHINE{SHEN_NAME(QTEM)};
inline constexpr shen_name QTE_TITLE_ACTORS[]{SHEN_NAME(MIT1), SHEN_NAME(MIT2), SHEN_NAME(MIT3), SHEN_NAME(QTL1), SHEN_NAME(QTL2), SHEN_NAME(QTL3)};
inline constexpr Vector3 QTE_TITLE_ACTOR_POSITIONS[]{{-0.2750000059604645f, 1.0579999685287476f, 0.6656000018119812f},
    {0.0f, 1.0579999685287476f, 0.6656000018119812f}, {0.2750000059604645f, 1.0579999685287476f, 0.6656000018119812f},
    {-0.2750000059604645f, 1.9311000108718872f, 0.3871999979019165f}, {0.0f, 1.9311000108718872f, 0.3871999979019165f},
    {0.2750000059604645f, 1.9311000108718872f, 0.3871999979019165f}};
inline constexpr Vector3i QTE_TITLE_ACTOR_ROTATIONS[]{{57344, 0, 0}, {0, 0, 0}};
inline constexpr Vector3 QTE_TITLE_DIGIT_STEP{-0.03020000085234642f, 0.0f, 0.0f};
inline constexpr Vector3 QTE_TITLE_SCORE_POSITIONS[]{{-0.1736000031232834f, 1.8164000511169434f, 0.24500000476837158f},
    {0.10760000348091125f, 1.8164000511169434f, 0.24500000476837158f}, {0.3887999951839447f, 1.8164000511169434f, 0.24500000476837158f}};
inline constexpr std::uint16_t QTE_MACHINE_PARTS[]{166, 169, 170, 176, 171, 172, 173, 182, 183, 184, 185, 186, 187, 167, 174, 175};
inline constexpr float QTE_MACHINE_SCALES[]{1.0f, 1.3f, 1.3f, 1.3f, 1.0f, 1.0f, 1.0f, 1.0f, 0.53f, 0.53f, 0.53f, 0.53f, 0.53f, 1.0f, 1.0f, 1.0f};
inline constexpr Vector3 QTE_MACHINE_POSITIONS[]{
    {0.0f, 1.440000057220459f, 0.0f}, {-0.08341799676418304f, 1.5499999523162842f, 0.0f},
    {0.004000000189989805f, 1.6019999980926514f, 0.0f}, {0.08302900195121765f, 1.5499999523162842f, 0.0f},
    {0.0f, 1.3350000381469727f, 0.0f}, {0.0f, 1.3350000381469727f, 0.0f},
    {0.0f, 1.3350000381469727f, 0.0f}, {0.0f, 1.3350000381469727f, 0.0f},
    {}, {}, {}, {}, {}, {-0.1303199976682663f, 1.2799999713897705f, 0.0f},
    {-0.0942699983716011f, 1.2799999713897705f, 0.0f}, {-0.059050001204013824f, 1.2799999713897705f, 0.0f}};

struct QteGameEntryParameter {
    std::uint64_t reserved00;
    HLib::Task *self;
    std::uint32_t state;
    std::uint32_t exciteBest;
    std::uint32_t exciteContinued;
    std::uint32_t titleScore2;
    std::uint32_t titleScore0;
    std::uint32_t titleScore1;
};
static_assert(sizeof(QteGameEntryParameter) == 0x28);
static_assert(offsetof(QteGameEntryParameter, self) == 8);
static_assert(offsetof(QteGameEntryParameter, titleScore0) == 0x20);

void InitializeQteTitleDisplay(QteGameEntryParameter *parameter);
void InitializeQteMachineDisplay(QteGameEntryParameter *parameter);
void EVT_QteGameEnter_Callback(void *parameter);
void EVT_QteGameEnter();

} // namespace shenmue::s2
