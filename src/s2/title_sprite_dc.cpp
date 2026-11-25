#include "auto_test.hpp"
#include "s2/legacy_sprite_types.hpp"
#include "s2/title_sprite_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/sdk.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/hlib.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/legacy_sprite_slot.hpp"
#include "s2/scene_data.hpp"
#include "s2/texture_dc.hpp"
#include "s2/title_dc.hpp"
#include <cstdio>
#include <cstring>
#include <cmath>

namespace shenmue {
namespace s2 {

std::int32_t ReadTempTextureEntries(const void *source, TempTextureEntry *entries);

void LoadAreaSpriteImages();

std::int32_t ReadTitleSpriteTextures(const void *source, std::uint32_t size, std::int32_t *textures);

void InitializeTitleCommonTempSprites();

void ReleaseNamedTexture_Wrapper(std::int32_t index);

std::int32_t titleSpriteTextures[15];

// @verified - d
void FreeTempSprites(std::int32_t index) {
    TempTextureEntry *&entries = g_tempTextures[index];
    if (!entries)
        return;
    for (std::int32_t texture = tempTextureDefinitions[index].textureCount - 1; texture >= 0; --texture)
        ReleaseNamedTexture_Wrapper(entries[texture].texture);
    HLib::FreeTaskData(entries);
    entries = 0;
}

// @verified - d
std::int32_t AllocateTempSprites(std::int32_t index) {
    union {
        std::int32_t words[8];
        char text[32];
    } filename = {{0}};
    TempTextureEntry *&entries = g_tempTextures[index];
    if (entries)
        FreeTempSprites(index);
    entries =
        static_cast<TempTextureEntry *>(HLib::AllocateTaskData(tempTextureDefinitions[index].textureCount * sizeof(TempTextureEntry), TITLE_COMMON_TEMP_TASK));
    if (entries) {
        if (std::strstr(tempTextureDefinitions[index].file, ".GZ")) {
            std::strcpy(filename.text, tempTextureDefinitions[index].file);
        } else {
            std::strcpy(filename.text, "VM_");
            std::strcat(filename.text, tempTextureDefinitions[index].file);
            std::strcat(filename.text, ".GZ");
        }
        void *const allocation = shenLoadCompressedFile("sprite", filename.text, TITLE_COMMON_TEMP_TASK);
        if (ReadTempTextureEntries(allocation, entries) > 0) {
            HLib::FreeTaskData(allocation);
            return 1;
        }
        HLib::FreeTaskData(entries);
        entries = 0;
    }
    return 0;
}

std::int32_t GetTempSpriteTextureId(std::int32_t index, std::int32_t texture) {
    TempTextureEntry *const entries = g_tempTextures[index];
    if (entries)
        return entries[texture].texture;
    return -1;
}

// @verified - d
bool BindTempSprite(std::int32_t index, std::int32_t texture, std::int32_t slot, const std::uint16_t *rectangles) {
    if (!g_tempTextures[index])
        return false;
    const TempTextureWork &work = GetTempSpriteTexture(index, texture);
    return RegisterBorrowedLegacySpriteSlot(work.texture, work.width, work.height, slot, rectangles);
}

// @verified - d
void InitShotsImages() {
    InitializeTitleCommonTempSprites();
    void *const allocation = shenLoadCompressedFile(Filepath_Gen(FilepathGenMode::SpriteDirectory), "TITLE.GZ", TITLE_SPRITE_FILE);
    const std::uint32_t size = static_cast<std::uint32_t>(HLib::GetTaskDataSize(allocation));
    if (allocation) {
        if (size)
            ReadTitleSpriteTextures(allocation, size, titleSpriteTextures);
        HLib::FreeTaskData(allocation);
    }
    LoadAreaSpriteImages();
}

// @verified - d
void InitializeTitlePacket(LegacySpritePacket &packet) {
    INIT_LEGACY_SPRITE_PACKET(packet);
}

// @verified - d
void DrawTitleLogo(std::int32_t texture, float alpha) {
    const std::uint8_t opacity = static_cast<std::uint8_t>(static_cast<std::int32_t>(alpha * 255.0f));
    dc::sdk::njSetBackColor(UINT32_MAX, UINT32_MAX, UINT32_MAX);
    LegacySpritePacket packet;
    InitializeTitlePacket(packet);
    packet.quad.flags |= 0x400U;
    packet.colour = (static_cast<std::uint32_t>(opacity) << 24U) | 0x00ffffffU;
    AdjustScreenAspectRatio(packet.quad, HorizontalStretch, 320, 240, static_cast<std::int16_t>(titleLogoDimensions[texture - 2][0]),
        static_cast<std::int16_t>(titleLogoDimensions[texture - 2][1]));
    DrawLegacySpritePacket(packet, titleSpriteTextures[texture], 1.1f);
}

// @verified - d
void DrawTitlePressStart(LegacySpritePacket packet, std::int32_t frame) {
    const std::int32_t mode = GetTitleMenuUIState();
    AdjustScreenAspectRatio(packet.quad, HorizontalStretch, 320, 240, 1024, 512);
    DrawLegacySpritePacket(packet, titleSpriteTextures[0], 1.1f);
    if (mode == 0 && frame > 15) {
        AdjustScreenAspectRatio(packet.quad, HorizontalStretch, 320, 380, 512, 32);
        DrawLegacySpritePacket(packet, titleSpriteTextures[1], 1.2f);
    }
}

// @verified - d
void SetPressStart(void *data) {
    const TitleDisplayData &display = *reinterpret_cast<const TitleDisplayData *>(data);
    const std::uint8_t opacity = static_cast<std::uint8_t>(static_cast<std::int32_t>(display.currentStateFrame * 255.0f));
    LegacySpritePacket packet;
    InitializeTitlePacket(packet);
    packet.quad.flags |= 0x400U;
    packet.colour = (static_cast<std::uint32_t>(opacity) << 24U) | 0x00ffffffU;
    DrawTitlePressStart(packet, static_cast<std::int32_t>(display.frameCount));
#if SHEN_AUTO_TEST_ENABLED
    auto_test::LatchTitleFrame(titleLoopSequence == 2 && display.currentStateFrame == 1.0f && GetTitleMenuUIState() == 0 && display.frameCount == 16);
#endif
}

// @verified - d
void DrawTitleAm2Logo(void *data) {
    DrawTitleLogo(4, static_cast<TitleDisplayData *>(data)->currentStateFrame);
}

// @verified - d
void DrawTitleSegaLogo(void *data) {
    DrawTitleLogo(5, static_cast<TitleDisplayData *>(data)->currentStateFrame);
}

// @verified - d
void DrawTitleDisc(void *data) {
    DrawTitleLogo(3, static_cast<TitleDisplayData *>(data)->currentStateFrame);
}

// @verified - d
void DrawTitleYsMark(void *data) {
    DrawTitleLogo(2, static_cast<TitleDisplayData *>(data)->currentStateFrame);
}

// @verified - d
void DrawTitleAdditionalLogo(void *data) {
    DrawTitleLogo(6, static_cast<TitleDisplayData *>(data)->currentStateFrame);
}

} // namespace s2
} // namespace shenmue

#endif
