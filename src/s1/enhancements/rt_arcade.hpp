#pragma once

#include "s1/modules/arcade.hpp"
#include <cstdint>

namespace shenmue::s1 {

inline constexpr char kRTArcadeAtlasPixelShader[] = "Texture2D frame : register(t8); SamplerState sampling : register(s0);"
                                                    "float4 main(float4 position : SV_POSITION, float2 uv : TEXCOORD0) : SV_TARGET"
                                                    "{ return float4(frame.Sample(sampling, uv).rgb, 1); }";

void RTArcade_Tick();
void RTArcade_BeginInteraction(ArcadeGame game);
void RTArcade_SetInteractiveFrame(ArcadeGame game, std::uint32_t texture);
void RTArcade_EndInteraction(ArcadeGame game);
std::uint32_t RTArcade_ResolveTexture(std::uint32_t texture);
void RTArcade_Shutdown();

} // namespace shenmue::s1
