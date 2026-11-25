#include "platform/dc/weather_texture.hpp"

#if SHENMUE_IS_DC && (SHENMUE_IS_S1 || SHENMUE_IS_S2)

#include <cstring>

#if SHENMUE_IS_S2
namespace shenmue {
namespace s2 {
#else
namespace shenmue {
namespace s1 {
#endif

WeatherTextureSurfaces g_weatherTextureSurfaces = {};

// @verified - d
NJS_TEXSURFACE *CreateWeatherTextureSurface(std::int32_t width, std::int32_t height, std::uint32_t format, const void *pixels, std::int32_t index) {
    NJS_TEXSURFACE *surface;
    if (!index) {
        surface = &g_weatherTextureSurfaces.surfaces[0];
        g_weatherTextureSurfaces.active[0] = surface;
    } else {
        surface = &g_weatherTextureSurfaces.surfaces[1];
        g_weatherTextureSurfaces.active[1] = surface;
    }
    if (kmCreateTextureSurface(reinterpret_cast<KMSURFACEDESC *>(surface), width, height, format))
        return 0;
    if (kmLoadTexture(reinterpret_cast<KMSURFACEDESC *>(surface), const_cast<Uint32 *>(static_cast<const Uint32 *>(pixels))))
        return 0;
    if (!index) {
        g_weatherTextureSurfaces.dimensions[0][0] = width;
        g_weatherTextureSurfaces.dimensions[0][1] = height;
    } else {
        g_weatherTextureSurfaces.dimensions[1][0] = width;
        g_weatherTextureSurfaces.dimensions[1][1] = height;
    }
    return surface;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void LoadWeatherTextureSurface(NJS_TEXSURFACE *surface, const void *pixels) {
    kmLoadTexture(reinterpret_cast<KMSURFACEDESC *>(surface), const_cast<Uint32 *>(static_cast<const Uint32 *>(pixels)), 0, 0);
}
#endif


// @verified - d
void InitializeWeatherTextureSurfaces() {
    std::memset(&g_weatherTextureSurfaces.surfaces[0], 0, sizeof(NJS_TEXSURFACE));
    std::memset(&g_weatherTextureSurfaces.surfaces[1], 0, sizeof(NJS_TEXSURFACE));
    g_weatherTextureSurfaces.active[0] = 0;
    g_weatherTextureSurfaces.active[1] = 0;
    g_weatherTextureSurfaces.dimensions[0][0] = 0;
    g_weatherTextureSurfaces.dimensions[0][1] = 0;
    g_weatherTextureSurfaces.dimensions[1][0] = 0;
    g_weatherTextureSurfaces.dimensions[1][1] = 0;
}
}
} // namespace shenmue

#endif
