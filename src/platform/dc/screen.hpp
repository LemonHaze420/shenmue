#pragma once
#include "shenmue.h"
#if SHENMUE_IS_DC
#include "platform/dc/ninja.hpp"
#if !SHENMUE_S2_GCC295
#include <cstdint>
#endif
namespace shenmue {
namespace dc {
namespace sdk {
void SubmitScreenTextureStrip(const float *vertices, std::int32_t texture, std::uint32_t flags, std::int32_t blendMode);
#if SHENMUE_IS_S2
void ResetScreenTextureState();
#endif
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetScreenUserClipMode(std::uint32_t mode);
void SetVertexListClipMode(std::uint32_t lists, std::uint32_t mode);
void SetVertexListClipping(std::int32_t left, std::int32_t top, std::int32_t right, std::int32_t bottom, std::uint32_t lists);
#endif
}
}
}
#endif
