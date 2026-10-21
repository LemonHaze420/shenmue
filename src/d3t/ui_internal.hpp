#pragma once

#include "d3t/ui_runtime.hpp"
#include "shenmue.h"

#if SHENMUE_IS_S1
#include "s1/d3t/namespace.hpp"
#include "s1/d3t/settings.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/nuklear_integration.hpp"
#include "s1/font.hpp"
#include "s1/input.hpp"
#include "s1/subtitles.hpp"
#include "s1/ui_definition.hpp"
#include "s1/ui_runtime.hpp"
#else
#include "d3t/dx11_renderer.hpp"
#include "d3t/mip_streaming.hpp"
#include "d3t/image_texture_loader.hpp"
#include "d3t/texture.hpp"
#include "d3t/buffer.hpp"
#include "d3t/content.hpp"
#include "d3t/render_handles.hpp"
#include "s2/font.hpp"
#include "s2/battle_qte_ui.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/input_bindings.hpp"
#include "s2/input.hpp"
#include "s2/renderer_initialization.hpp"
#include "s2/scene_data.hpp"
#include "s2/ui.hpp"
#endif

#include <cJSON.h>

#include <algorithm>
#include <array>
#include <atomic>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <string_view>
#include <vector>

#if SHENMUE_IS_S1
namespace shenmue::s1 {
#else
namespace shenmue::s2 {
#endif

#if SHENMUE_IS_S1
using d3t::FormatVirtualKeyString;
#else
using d3t::g_originalScreenHeight;
using d3t::g_originalScreenWidth;
using d3t::GetTexture2DDimensions;
using d3t::InitTexture2DResource;
#endif
#if !SHENMUE_IS_S1
using InputMode = InputType;

inline constexpr bool (&FormatVirtualKeyString)(std::int32_t, std::array<char, 64> &) = FormatUIInputKeyName;

// The IconFontSize names, in font order.
inline constexpr std::array<std::string_view, 6> kIconFontSizeNames{"H1", "H2", "H3", "BL", "B", "BS"};
// S2's ButtonIcon action names (69: FightRun and FightAssignedMove swap places and a second FightRun follows FightRight).
inline constexpr std::array<std::string_view, uiInputActionNames.size()> kInputActionNames = [] {
    std::array<std::string_view, uiInputActionNames.size()> names{};
    for (std::size_t index = 0; index < names.size(); ++index)
        names[index] = uiInputActionNames[index];
    return names;
}();
#endif

extern std::int32_t g_uiLabelSplitting;
std::uint32_t ConstructColour(std::int32_t red, std::int32_t green, std::int32_t blue, std::int32_t alpha);
#if SHENMUE_IS_S2
std::uint32_t ReadAlignmentValue(const char *alignment);
std::int32_t GetKeyForInputType(std::int32_t action, std::int32_t inputType);
std::int32_t ResolvePressedKey(std::int32_t action, std::int32_t inputType, std::int32_t doubleWidth);
extern std::int32_t g_debugUnknown0;
extern std::array<std::byte *, 7> g_uiFontDefinitions;
#endif
extern std::int32_t g_uiDebugDrawHitAreas;
extern nk_panel g_uiCollectionSelectorPanel;

#if SHENMUE_IS_S1
} // namespace shenmue::s1
#else
} // namespace shenmue::s2
#endif
