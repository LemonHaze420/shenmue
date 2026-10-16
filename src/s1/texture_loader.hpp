#pragma once

#include <array>
#include <cstdint>

namespace shenmue::s1 {

std::int32_t LoadTEXN(void *texnData);



struct TextureLoadRequest {
    std::uint16_t generation;
    std::uint32_t texture;
    std::uint32_t linearColour;
};
extern std::array<TextureLoadRequest, 0x2000> g_textureLoadRequests;
std::uint32_t LoadTextureFromFilepath(const char *path, std::uint32_t linearColour, std::uint32_t renderHandle);
std::uint32_t LoadTextureFromFilepathWithUsage(const char *path);

} // namespace shenmue::s1
