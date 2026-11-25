#include "platform/dc/screen.hpp"
#if SHENMUE_IS_DC
#include <cstring>
#if SHENMUE_IS_S2
#include "platform/dc/sdk.hpp"
#include "platform/dc/mt7.hpp"
#include "s2/texture_cache_dc.hpp"
#include <km2strct.h>
#endif
namespace shenmue {
namespace dc {
namespace sdk {
#if SHENMUE_IS_S2
std::int32_t g_screenTextureError = 0;
std::int32_t g_screenTextureTransformEnabled = 0;
#endif
#if SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void ScreenNullsub() {
}
#endif
namespace {
#if SHENMUE_IS_S2
void GetSpriteBlend(KMBLENDINGMODE *source, KMBLENDINGMODE *destination) {
    *source = g_ninjaCurrentContext->SRCBlendingMode;
    *destination = g_ninjaCurrentContext->DSTBlendingMode;
}

#endif
void SetSpriteBlend(KMBLENDINGMODE source, KMBLENDINGMODE destination) {
    g_ninjaCurrentContext->RenderState = 0xc0;
    g_ninjaCurrentContext->SRCBlendingMode = source;
    g_ninjaCurrentContext->DSTBlendingMode = destination;
    kmProcessVertexRenderState(g_ninjaCurrentContext);
#if !SHENMUE_IS_S2
    kmSetVertexRenderState(g_ninjaCurrentContext);
#endif
}

#if SHENMUE_IS_S2
extern "C" void syCachePREF_s(void *address, std::uint32_t size);

// @verified - d
bool SetScreenTexture(std::int32_t texture, bool transparent) {
    const s2::TextureIndexMap &index = s2::g_textureIndexMap[texture];
    if (!index.references)
        return false;
    s2::TextureCacheSurface &surface = s2::g_textureCacheSurfaces[index.surface];
    KMVERTEXCONTEXT *const context = g_ninjaCurrentContext;
    context->pTextureSurfaceDesc = &surface.surface;
    context->RenderState = 0x600608U;
    context->ListType = static_cast<KMLISTTYPE>(transparent ? 2 : 0);
    context->ColorType = static_cast<KMCOLORTYPE>(0);
    context->ShadingMode = static_cast<KMSHADINGMODE>(3);
    context->bUseAlpha = static_cast<KMBOOLEAN>(1);
    context->bUseSpecular = static_cast<KMBOOLEAN>(1);
    const std::uint32_t format = (surface.tcw >> 27U) & 7U;
    if (format == 5 || format == 6) {
        context->RenderState |= 0x80000U;
        if (s2::g_paletteDisabled) {
            context->PaletteBank = s2::g_selectedPalette;
        } else if (s2::g_selectedPalette < s2::g_paletteSlotCount && s2::g_paletteSlots[s2::g_selectedPalette].references) {
            context->PaletteBank = s2::g_paletteSlots[s2::g_selectedPalette].firstBlock;
        }
    }
    kmProcessVertexRenderState(context);
    context->GLOBALPARAMBUFFER = (context->GLOBALPARAMBUFFER & 0xfffcff3fU) | g_mt7DrawControls.flags;
    context->TSPPARAMBUFFER = (context->TSPPARAMBUFFER & 0x031ff0ffU) | g_mt7DrawControls.tsp | ((g_mt7AlphaBlendMask ^ 0xfcU) << 24U);
    kmSetVertexRenderState(context);
    return true;
}

// @verified - d
void SubmitScreenPacket(const std::uint32_t *words, std::uint32_t bytes) {
    KMVERTEXBUFFDESC *const descriptor = reinterpret_cast<KMVERTEXBUFFDESC *>(&g_sdkVertexBufferPointers);
    const std::uint32_t list = descriptor->pCurrentListState->ListType;
    const std::uint32_t cursor = reinterpret_cast<std::uintptr_t>(descriptor->pCurrentPtr[list]);
    const std::uint32_t queue = cursor >> 24U;
    *reinterpret_cast<volatile std::uint32_t *>(0xff000038U) = queue;
    *reinterpret_cast<volatile std::uint32_t *>(0xff00003cU) = queue;
    const std::uint32_t destination = (cursor & 0x03ffffffU) | 0xe0000000U;
    volatile std::uint32_t *const packet = reinterpret_cast<volatile std::uint32_t *>(destination);
    for (std::uint32_t word = 0; word < bytes / 4; ++word)
        packet[word] = words[word];
    syCachePREF_s(reinterpret_cast<void *>(destination), 32);
    if (bytes == 64)
        syCachePREF_s(reinterpret_cast<void *>(destination + 32), 32);
    descriptor->pCurrentPtr[list] = reinterpret_cast<PKMDWORD>(cursor + (bytes & ~3U));
}

// @verified - d
void SubmitScreenTextureVertices(const float *vertices) {
    KMVERTEXBUFFDESC *const descriptor = reinterpret_cast<KMVERTEXBUFFDESC *>(&g_sdkVertexBufferPointers);
    SubmitScreenPacket(descriptor->pGlobalParam, descriptor->pCurrentListState->GlobalParamSize);
    for (std::int32_t vertex = 0; vertex < 4; ++vertex, vertices += 7) {
        std::uint32_t packet[8];
        packet[0] = vertex == 3 ? 0xf0000000U : 0xe0000000U;
        float depth = vertices[2];
        if (g_screenTextureTransformEnabled) {
            Vector3 source = {vertices[0], vertices[1], vertices[2]};
            Vector3 transformed;
            njCalcPoint(0, &source, &transformed);
            std::memcpy(packet + 1, &transformed, sizeof(float) * 2);
            depth = transformed.z;
        } else {
            std::memcpy(packet + 1, vertices, sizeof(float) * 2);
        }
        packet[3] = floatAsU32(1.0f / depth);
        std::memcpy(packet + 4, vertices + 3, sizeof(float) * 4);
        SubmitScreenPacket(packet, sizeof(packet));
    }
}
#endif
} // namespace

void SubmitScreenTextureStrip(const float *vertices, std::int32_t texture, std::uint32_t flags, std::int32_t blendMode) {
#if SHENMUE_IS_S2
    SetMT7PolygonFogMode(2);
    KMBLENDINGMODE source, destination;
#else
    const KMBLENDINGMODE source = g_ninjaCurrentContext->SRCBlendingMode;
    const KMBLENDINGMODE destination = g_ninjaCurrentContext->DSTBlendingMode;
#endif
    if (flags & 0x800) {
#if SHENMUE_IS_S2
        GetSpriteBlend(&source, &destination);
#endif
        const std::uint32_t modes[4] = {8, 6, 8, 10};
        SetSpriteBlend(static_cast<KMBLENDINGMODE>(modes[blendMode * 2]), static_cast<KMBLENDINGMODE>(modes[blendMode * 2 + 1]));
    }
#if SHENMUE_IS_S2
    if (SetScreenTexture(texture + g_modelTextureIdBase, (flags & 0x400U) != 0))
        SubmitScreenTextureVertices(vertices);
#else
    NJS_TEXTUREH_VTX packet[4];
    std::memcpy(packet, vertices, sizeof(packet));
    njDrawTextureH(packet, 4, texture, (flags & 0x400) != 0);
#endif
    if (flags & 0x800)
        SetSpriteBlend(source, destination);
#if SHENMUE_IS_S2
    SetMT7PolygonFogMode(0);
#endif
}

#if SHENMUE_IS_S2
// @verified - d
void ResetScreenTextureState() {
    g_screenTextureError = 0;
    g_screenTextureTransformEnabled = 0;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern "C" std::int32_t kmSetUserClipping(
    KMVERTEXBUFFDESC *descriptor, std::uint32_t list, std::int32_t left, std::int32_t top, std::int32_t right, std::int32_t bottom);

void SetScreenUserClipMode(std::uint32_t mode) {
    g_ninjaCurrentContext->RenderState = KM_USERCLIPMODE;
    g_ninjaCurrentContext->UserClipMode = static_cast<KMUSERCLIPMODE>(mode);
    kmProcessVertexRenderState(g_ninjaCurrentContext);
    kmSetVertexRenderState(g_ninjaCurrentContext);
}

// @verified - d
void SetVertexListClipMode(std::uint32_t, std::uint32_t mode) {
    KMVERTEXBUFFDESC *const descriptor = reinterpret_cast<KMVERTEXBUFFDESC *>(&g_sdkVertexBufferPointers);
    descriptor->pGlobalParam[0] = 0x800000U | (mode << 16);
}

// @verified - d
void SetVertexListClipping(std::int32_t left, std::int32_t top, std::int32_t right, std::int32_t bottom, std::uint32_t lists) {
    KMVERTEXBUFFDESC *const descriptor = reinterpret_cast<KMVERTEXBUFFDESC *>(&g_sdkVertexBufferPointers);
    for (std::uint32_t list = 0; list < 5; ++list, lists >>= 1)
        if (lists & 1U)
            kmSetUserClipping(descriptor, list, left, top, right, bottom);
}
#endif

} // namespace sdk
} // namespace dc
} // namespace shenmue
#endif
