#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/types.hpp"
#include "platform/dc/ninja.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/palette_dc.hpp"

#define TX_DATA const
#define TX_INIT =
#define TX_LAYOUT_NAME_(line) TXLayoutCheck##line
#define TX_LAYOUT_NAME(line) TX_LAYOUT_NAME_(line)
#define TX_LAYOUT(condition) typedef char TX_LAYOUT_NAME(__LINE__)[(condition) ? 1 : -1]
namespace shenmue {
namespace s2 {

struct TextureImage {
    std::int16_t width, height;
    std::uint32_t type;
    const void *pixels;
};
TX_LAYOUT(sizeof(TextureImage) == 0x0c);
struct NamedTextureRequest {
    TextureImage image;
    std::int32_t index;
};
TX_LAYOUT(sizeof(NamedTextureRequest) == 0x10);
struct TextureChunk {
    shen_name name;
    std::uint32_t size;
    std::uint32_t nameLow, nameHigh;
    std::uint32_t globalIndexHeader[3];
    shen_name imageName;
    std::uint32_t imageSize;
    std::uint32_t type;
    std::int16_t width, height;
};
TX_LAYOUT(sizeof(TextureChunk) == 0x2c);
TX_LAYOUT(offsetof(TextureChunk, type) == 0x24);
TX_LAYOUT(offsetof(TextureChunk, width) == 0x28);
struct NamedTextureCell {
    std::int16_t index;
    std::uint16_t references;
    std::uint32_t nameLow;
    std::uint32_t nameHigh;
    NamedTextureCell *next;
};
TX_LAYOUT(sizeof(NamedTextureCell) == 0x10);
TX_LAYOUT(offsetof(NamedTextureCell, next) == 0x0c);

TX_DATA std::uint16_t kDefaultTexturePalette[32] TX_INIT{
    0xffff, 0xf000, 0xf0f0, 0xfff0, 0xff00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffff, 0xffff, 0, 0, 0, 0, 0, 0xbf80, 0, 0xbf80, 0, 0x3f80, 0, 0xbf80, 0, 0xbf80};
TX_DATA PaletteData kDefaultEffectPalette TX_INIT{
    SHEN_FIELD(index) 0, SHEN_FIELD(mode) 16, SHEN_FIELD(colours) kDefaultTexturePalette, SHEN_FIELD(colourCount) 32};

extern NamedTextureCell *g_namedTextureCells;
extern NamedTextureCell *g_freeNamedTextureCells;
extern NamedTextureCell *g_namedTextureBanks[2];
extern std::int32_t g_namedTextureBank;

std::int32_t LoadNamedTexture(const TextureImage *image, const std::uint32_t *name);
std::int32_t FindNamedTextureIndex(std::uint32_t nameLow, std::uint32_t nameHigh);
std::int32_t LoadAndFindTEXN(const TextureChunk *chunk);
std::int32_t LoadAndFindTEXN_Wrapper(const TextureChunk *chunk);
std::int32_t LoadTextureChunks(const TextureChunk *chunk, std::int32_t *textures);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
struct MemoTextureName;
std::int32_t LoadTextureChunks(const TextureChunk *chunk, std::uint32_t bytes, std::int32_t *textures, MemoTextureName *names);
#else
std::int32_t LoadTextureChunks(const TextureChunk *chunk, std::uint32_t bytes, std::int32_t *textures);
#endif
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void QueryFreeTextureMemory(KMUINT32 *total, KMUINT32 *largest);
std::int32_t GetTextureStorageSize(std::uint32_t format, std::int32_t height, std::int32_t width);
bool ParseGuiPtx(TextureChunk *texture, std::int32_t height, std::int32_t width, std::uint32_t idLow, std::uint32_t idHigh, std::uint32_t format);
void FlushTextureImage(const std::uint32_t *image);
std::int32_t ReplacePtxTexture(std::int32_t index, const TextureChunk *texture);

#endif
std::int32_t ReleaseNamedTexture(std::int32_t index);
void TXMO_Reader(const void *chunk, std::int32_t entryPoint);
extern std::int32_t textureMotionFlag;
std::int32_t texGetMotionFlag();
void texSetMotionFlag(std::int32_t enabled);
void InitializeTextureCellStorage();
NamedTextureCell *FindNamedTexture(std::uint32_t nameLow, std::uint32_t nameHigh);
std::int32_t FindNamedTexture(std::uint32_t nameLow, std::uint32_t nameHigh, NamedTextureCell **previous, NamedTextureCell **found);
std::int32_t FindNamedTextureByIndex(std::int32_t index, NamedTextureCell **previous, NamedTextureCell **found);
std::int32_t AllocateNamedTextureCell(std::uint32_t nameLow, std::uint32_t nameHigh);
void ReleaseNamedTextureCell(NamedTextureCell *previous, NamedTextureCell *cell);
void SetNamedTextureBank(std::int32_t bank);
void ClearNamedTextureBank();
void DestroyEffectTextureReferences();
void InitSceneEffects();
struct GeneratedTextureSurface;
void DestroyGeneratedSurface(GeneratedTextureSurface *surface);

} // namespace s2
} // namespace shenmue
#undef TX_LAYOUT_NAME
#undef TX_LAYOUT_NAME_
#undef TX_DATA
#undef TX_INIT
#undef TX_LAYOUT

#endif
