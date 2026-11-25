#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC

#if SHENMUE_S2_GCC295
#include "common/legacy_types.hpp"
#else
#include <cstddef>
#endif
#include <sg_xpt.h>
#include <ninjadef.h>
#include <ninjastr.h>
#include <kmbase.h>
#include <km2defs.h>

#if SHENMUE_S2_GCC295
typedef char NinjaTextureVertexSizeCheck[sizeof(NJS_TEXTUREH_VTX) == 0x1c ? 1 : -1];
typedef char NinjaVertexContextSizeCheck[sizeof(KMVERTEXCONTEXT) == 0xc0 ? 1 : -1];
typedef char NinjaSourceBlendOffsetCheck[offsetof(KMVERTEXCONTEXT, SRCBlendingMode) == 0x2c ? 1 : -1];
typedef char NinjaDestinationBlendOffsetCheck[offsetof(KMVERTEXCONTEXT, DSTBlendingMode) == 0x30 ? 1 : -1];
#else
static_assert(sizeof(NJS_TEXTUREH_VTX) == 0x1c);
static_assert(sizeof(KMVERTEXCONTEXT) == 0xc0);
static_assert(offsetof(KMVERTEXCONTEXT, SRCBlendingMode) == 0x2c);
static_assert(offsetof(KMVERTEXCONTEXT, DSTBlendingMode) == 0x30);

#endif

extern "C" {
void njInitTexture(NJS_TEXMEMLIST *memory, Uint32 count);
Sint32 njLoadTexture(NJS_TEXLIST *textures);
void njSetTextureInfo(NJS_TEXINFO *info, Uint16 *pixels, Sint32 type, Sint32 width, Sint32 height);
void njSetTextureName(NJS_TEXNAME *name, void *data, Uint32 globalIndex, Uint32 attributes);
Sint32 njSetTexture(NJS_TEXLIST *textures);
void njReleaseTextureAll();
void njReleaseCacheTextureAll();
void njDrawTextureH(NJS_TEXTUREH_VTX *vertices, int count, int texture, int flags);
void njDrawPolygon(NJS_POLYGON_VTX *vertices, int count, int transparent);
void njSetFogColor(Uint32 colour);
void njFogEnable();
void njFogDisable();
void njSetFogDensity(Uint32 density);
void njSetFogTable(float *table);
Sint32 njReleaseTexture(NJS_TEXLIST *textures);
Sint32 njReleaseTextureNumG(Uint32 texture);
Sint32 njReLoadTextureNumG(Uint32 texture, void *pixels, Uint32 attributes, Uint32 lod);
void njSetPaletteMode(Uint32 mode);
void njSetPaletteData(Sint32 offset, Sint32 count, void *data);
int kmProcessVertexRenderState(KMVERTEXCONTEXT *context);
int kmSetVertexRenderState(KMVERTEXCONTEXT *context);
int kmUseAnotherModifier(Uint32 enabled);
int kmSetCheapShadowMode(Sint32 strength);
int kmCreateTextureSurface(KMSURFACEDESC *surface, Sint32 width, Sint32 height, KMTEXTURETYPE type);
int kmLoadTexture(KMSURFACEDESC *surface, Uint32 *pixels, Uint32 option0 = 0, Uint32 option1 = 0);
int kmFreeTexture(KMSURFACEDESC *surface);
int kmGetFreeTextureMem(KMUINT32 *total, KMUINT32 *largest);
extern KMVERTEXCONTEXT *g_ninjaCurrentContext asm("__nj_current_ctx_");
}

#endif
