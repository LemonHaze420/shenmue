#pragma once

#include "d3t/ui_runtime.hpp"
#include "d3t/model.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {
struct ConversationBuffer;
struct UIElementState;
namespace ui_detail {

enum CreditsCellKind : std::uint32_t { CCK_Empty, CCK_Image, CCK_Text, CCK_Spacer };
struct CreditsCell {
    std::uint32_t kind, value;
};
struct CreditsRow {
    std::array<CreditsCell, 4> cells;
};
struct CreditsImage {
    std::uint32_t request, texture;
    struct nk_image image;
};
struct CreditsTextHeap {
    char *data;
    std::uint32_t size, capacity;
};
struct CreditsTable {
    d3t::D3TRawArray<CreditsRow> rows;
    d3t::D3TRawArray<CreditsImage> images;
    CreditsTextHeap text;
    float height;
    std::uint8_t loaded;
    std::byte reserved35[3];
    std::uint32_t contentHandle, reserved3c;
};
struct CreditsLegacyParameters {
    float unknown00;
    std::uint32_t unknown04, unknown08, unknown0c;
};
static_assert(sizeof(CreditsLegacyParameters) == 0x10);
extern CreditsLegacyParameters creditsLegacyParameters;
struct CreditsState {
    std::array<CreditsTable, 3> tables;
    d3t::D3TRawArray<CreditsImage> sections;
    CreditsImage signature;
    float scrollSpeed, scrollProgress, fadeAlpha;
    std::uint8_t fadeOutRequested, scrollInitialized;
    std::byte reserved106[2];
    ConversationBuffer *voice;
    std::uint32_t textColour;
    std::byte reserved114[12];
    nk_panel panel;
};
static_assert(sizeof(CreditsCell) == 8 && sizeof(CreditsRow) == 0x20);
static_assert(sizeof(CreditsImage) == 0x28 && offsetof(CreditsImage, image) == 8);
static_assert(sizeof(CreditsTextHeap) == 0x10);
static_assert(sizeof(CreditsTable) == 0x40 && offsetof(CreditsTable, text) == 0x20);
static_assert(offsetof(CreditsTable, height) == 0x30 && offsetof(CreditsTable, loaded) == 0x34);
static_assert(offsetof(CreditsTable, contentHandle) == 0x38);
static_assert(offsetof(CreditsState, sections) == 0xc0 && offsetof(CreditsState, signature) == 0xd0);
static_assert(offsetof(CreditsState, scrollSpeed) == 0xf8 && offsetof(CreditsState, scrollProgress) == 0xfc);
static_assert(offsetof(CreditsState, fadeAlpha) == 0x100 && offsetof(CreditsState, voice) == 0x108);
static_assert(offsetof(CreditsState, textColour) == 0x110 && offsetof(CreditsState, panel) == 0x120);
inline constexpr std::array<const char *, 3> CREDITS_TYPES{"OriginalSM2", "d3t", "Sega"};
inline constexpr float CREDITS_ROW_SPACING = 12.0f, CREDITS_COLUMN_INSET = 16.0f, CREDITS_EDGE_FADE = 128.0f;
inline constexpr float CREDITS_DURATION = 365.0f, CREDITS_SIGNATURE_DURATION = 10.0f;
inline constexpr std::uint16_t CREDITS_SKIP_INPUT = 0x600;
extern CreditsState creditsState;
extern d3t::UIWindow *creditsScrollSpace;
extern UIElementState *creditsElement;
std::uint32_t LoadEndingCredits();
void InitializeEndingCreditsElement(UIElementState &element);
std::int64_t BindEndingCredits();
std::int64_t ResetEndingCredits();
std::int64_t HandleEndingCreditsInput();
std::int64_t InitializeEndingCredits();
std::int64_t UpdateEndingCredits();
std::int64_t ReleaseEndingCredits();
} // namespace ui_detail
} // namespace shenmue::s2
