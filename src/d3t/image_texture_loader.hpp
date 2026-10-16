#pragma once

#include <cstddef>
#include <cstdint>

namespace shenmue::d3t {

std::int64_t LoadImageTexture(const char *path, bool linearColour, std::uint32_t *textureHandle, std::uint32_t renderHandle);
std::uint32_t CreateImageTextureFromMemory(const void *data, std::size_t size, bool linearColour);

bool ReplaceDecodedImage(std::uint32_t textureHandle, const void *loadedData, std::uint64_t loadedSize, bool linearColour);

} // namespace shenmue::d3t
