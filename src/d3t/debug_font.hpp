#pragma once

#include "shenmue.h"
#include <cstdint>
#include <string_view>

#ifdef _DEBUG
#if SHENMUE_IS_S1
namespace shenmue::s1 {
#else
namespace shenmue::s2 {
#endif
bool DrawLegacyDebugText(std::string_view text, float x, float y, float width, float height, float advance, float lineHeight, std::uint32_t colour);
}
#endif
