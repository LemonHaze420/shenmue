#pragma once

#include "platform/dc/particles.hpp"

#if SHENMUE_IS_DC && (SHENMUE_IS_DEMO || SHENMUE_IS_S1 || defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace shenmue { namespace dc { namespace sdk {

static const Matrix4x4 kParticleScreenMatrix = {{
    {320.0f, 0.0f, 0.0f, 0.0f}, {0.0f, -240.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, -0.5f, 0.0f}, {320.0f, 240.0f, 0.5f, 1.0f}}};

extern Matrix4x4 g_particleRenderMatrix asm("_shenmue_dc_particle_render_matrix");
extern Vector4 g_particleProjectionScales asm("_shenmue_dc_particle_projection_scales");
extern Matrix4x4 g_particleCallbackMatrix asm("_shenmue_dc_particle_callback_matrix");
extern Matrix4x4 g_particleFullMatrix[2] asm("_shenmue_dc_particle_full_matrix");
extern Matrix4x4 g_particleFullSecondMatrix[2] asm("_shenmue_dc_particle_full_second_matrix");
extern Matrix4x4 g_particleXyMatrix asm("_shenmue_dc_particle_xy_matrix");
extern Matrix4x4 g_particleXyzMatrix asm("_shenmue_dc_particle_xyz_matrix");
extern Matrix4x4 g_particleCompactMatrix[2] asm("_shenmue_dc_particle_compact_matrix");
extern Matrix4x4 g_particleCompactSecondMatrix[2] asm("_shenmue_dc_particle_compact_second_matrix");
extern Matrix4x4 g_particleCompactXyMatrix asm("_shenmue_dc_particle_compact_xy_matrix");
extern Matrix4x4 g_particleCompactXyzMatrix asm("_shenmue_dc_particle_compact_xyz_matrix");

void PrepareParticleTransform() asm("_shenmue_dc_prepare_particle_transform");
void DiscardParticleMatrix(std::int32_t count) asm("_shenmue_dc_discard_particle_matrix");

} } } // namespace shenmue::dc::sdk

#endif
