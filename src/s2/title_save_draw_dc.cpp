#include "s2/title_save_dc.hpp"

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/backup_hotplug_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/title_dc.hpp"
#include "s2/title_sprite_dc.hpp"

namespace shenmue {
namespace s2 {

// @verified - d
void DrawTitleFileSelectDate(std::int32_t x, std::int32_t y, const void *date) {
    const std::uint8_t *const bytes = static_cast<const std::uint8_t *>(date);
    std::int32_t position = x - 40;
    DrawTempSpriteSlot((bytes[2] + 12) | 0x250000, (position << 16) | y, 1.4000001f, 0);
    position += 30;
    DrawTempSpriteSlot(10 | 0x250000, (position << 16) | y, 1.4000001f, 0);
    position += 12;
    if (bytes[3] / 10)
        DrawTempSpriteSlot((bytes[3] / 10) | 0x250000, (position << 16) | y, 1.4000001f, 0);
    position += 16;
    DrawTempSpriteSlot((bytes[3] % 10) | 0x250000, (position << 16) | y, 1.4000001f, 0);
    position += 14;
    DrawTempSpriteSlot(11 | 0x250000, (position << 16) | y, 1.4000001f, 0);
    position += 12;
    std::uint16_t digit;
    if (*reinterpret_cast<const std::uint16_t *>(bytes) < 100)
        digit = 1;
    else
        digit = *reinterpret_cast<const std::uint16_t *>(bytes) / 1000;
    DrawTempSpriteSlot(digit | 0x250000, (position << 16) | y, 1.4000001f, 0);
    position += 16;
    if (*reinterpret_cast<const std::uint16_t *>(bytes) < 100)
        digit = 9;
    else
        digit = *reinterpret_cast<const std::uint16_t *>(bytes) / 100 % 10;
    DrawTempSpriteSlot(digit | 0x250000, (position << 16) | y, 1.4000001f, 0);
    position += 16;
    DrawTempSpriteSlot((*reinterpret_cast<const std::uint16_t *>(bytes) / 10 % 10) | 0x250000, (position << 16) | y, 1.4000001f, 0);
    position += 16;
    DrawTempSpriteSlot((*reinterpret_cast<const std::uint16_t *>(bytes) % 10) | 0x250000, (position << 16) | y, 1.4000001f, 0);
    position += 26;
    if (bytes[4] / 10)
        DrawTempSpriteSlot((bytes[4] / 10) | 0x250000, (position << 16) | y, 1.4000001f, 0);
    position += 16;
    DrawTempSpriteSlot((bytes[4] % 10) | 0x250000, (position << 16) | y, 1.4000001f, 0);
    position += 12;
    DrawTempSpriteSlot(12 | 0x250000, (position << 16) | y, 1.4000001f, 0);
    position += 14;
    DrawTempSpriteSlot((bytes[5] / 10) | 0x250000, (position << 16) | y, 1.4000001f, 0);
    position += 16;
    DrawTempSpriteSlot((bytes[5] % 10) | 0x250000, (position << 16) | y, 1.4000001f, 0);
}

void DrawTitleFileSelectCursor(const BackupSlotMetadata *) {
    DrawTempSpriteSlot(0x20001, 0x020000e0, 1.4000001f, 0);
}

void DrawTitleFileSelectCard(std::int32_t slot) {
    const BackupSlotMetadata *metadata = titleSaveSelection.slots[slot].metadata;
    std::int32_t item;
    std::int32_t y;
    y = slot * 97 + 131;
    std::int32_t left = 109;
    std::int32_t centre = 243;
    const std::int32_t selected = slot == titleSaveSelection.selectedSlot;
    if (selected)
        DrawTitleFileSelectCursor(metadata);
    item = selected ? 29 : 30;
    SetLegacySpriteSlotSizeOffsets(-19, 0);
    DrawTempSpriteSlot(item | 0x250000, ((left + 6) << 16) | (y - 15), 1.3000000715255737f, 0);
    DrawTempSpriteSlot((slot + 31) | 0x250000, ((left + 2) << 16) | (y - 20), 1.4000001f, 0);
    SetLegacySpriteSlotSizeOffsets(0, 0);
    const std::uint32_t flags = titleSaveSelection.slots[slot].flags;
    if ((flags & 1) == 0) {
        DrawTempSpriteSlot(39 | 0x250000, ((centre + 54) << 16) | y, 1.4000001f, 0);
    } else {
        item = (flags & 2) ? 38 : -1;
        if (item >= 0) {
            DrawTempSpriteSlot(item | 0x250000, ((centre - 135) << 16) | (y + 19), 1.4000001f, 0);
        } else {
            DrawTempSpriteSlot(36 | 0x250000, ((centre - 147) << 16) | (y + 18), 1.4000001f, 0);
            DrawTempSpriteSlot(static_cast<std::int16_t>(metadata->retained12) | 0x250000, ((centre - 105) << 16) | (y + 19), 1.4000001f, 0);
        }
        DrawTempSpriteSlot(25 | 0x250000, ((centre - 35) << 16) | (y - 34), 1.4000001f, 0);
        DrawTempSpriteSlot(26 | 0x250000, ((centre - 35) << 16) | (y + 8), 1.4000001f, 0);
        DrawTitleFileSelectDate(centre - 1, y - 15, reinterpret_cast<const std::uint8_t *>(metadata) + 8);
        DrawTitleFileSelectDate(centre - 1, y + 29, metadata);
    }
    SetLegacySpriteSlotSizeOffsets(-1, 3);
    DrawTempSpriteSlot(34 | 0x250000, (centre << 16) | y, 1.2f, 0);
    SetLegacySpriteSlotSizeOffsets(0, 0);
}

void DrawTitleFileSelectCards(std::int32_t result) {
    std::uint32_t background = 1;
    if (GetBackupInfo(GetTitleSaveBackupPort()) & 1U) {
        DrawFileSelectFrame();
        for (std::int32_t slot = 0; slot < 3; ++slot)
            DrawTitleFileSelectCard(slot);
        if (result)
            DrawTitleInfoBar(4, -1);
        else
            DrawTitleInfoBar(13, 12);
        background |= 2;
    }
    DrawTitleBackgroundAndButtons(background);
}

void DrawTitleFileSelect(void *) {
    std::uint32_t background = 1;
    const std::int8_t result = titleActiveSaveSelection->result;
    if (result >= 0) {
        DrawTitleFileSelectCards(result);
        background = 3;
    }
    DrawTitleBackgroundAndButtons(background);
}

} // namespace s2
} // namespace shenmue
#endif
