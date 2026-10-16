#pragma once
#include "common/math.hpp"
#include <cstdint>
namespace shenmue::s2 {
void ConfigureSurfaceWaveModel(std::int32_t slot, float heightOffset);
void InitializeSurfaceWaveParameters();
void AdvanceSurfacePhaseAndUpdateGrid(std::int32_t sceneFrame);
float GetSurfaceHeightAt(float x, float z);
std::uint32_t DrawSurfaceWaveModel();
}
