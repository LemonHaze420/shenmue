#pragma once

#include "s1/d3t/namespace.hpp"
#include <d3t/model.hpp>
#include "common/math.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

namespace d3t {
bool CheckForFile(const char *path);
} // namespace d3t
void SDTextureOverride();
namespace d3t {
void CheckAndRemapIfNeeded(const char *path, std::int32_t ordinal, std::uint32_t textureHandle);
} // namespace d3t
std::uint32_t ResolveStripTextureHandle(std::uint32_t textureHandle);
namespace d3t {
std::uint8_t PollTextureOverrides();
} // namespace d3t
void InitTextureSpace();

} // namespace shenmue::s1
