#pragma once

#include "s1/hlib.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct SYGHSprite {
    float x, y, scaleX, scaleY, depth;
    std::int16_t surface;
    std::uint16_t reserved16;
    std::int16_t u, v;
    std::uint16_t reserved1c, width, height;
    std::uint8_t flags, reserved23;
};
static_assert(sizeof(SYGHSprite) == 36);
static_assert(offsetof(SYGHSprite, surface) == 20 && offsetof(SYGHSprite, width) == 30 && offsetof(SYGHSprite, flags) == 34);

struct SYGHColourQuad {
    float x, y, scaleX, scaleY, depth;
    std::uint32_t colour;
    std::uint16_t width, height;
    std::uint32_t reserved1c;
    std::uint8_t flags;
    std::array<std::byte, 3> reserved21;
};
static_assert(sizeof(SYGHColourQuad) == 36);

struct SYGHPopupRectangle {
    float centreX, x, reserved08, reserved0c, y;
    std::uint32_t colour;
    std::int16_t width, height;
};
static_assert(sizeof(SYGHPopupRectangle) == 28);

struct SYGHDrawState {
    SYGHPopupRectangle popup;
    SYGHColourQuad side, underline, footer;
    std::array<SYGHSprite, 2> buttons;
    SYGHSprite background, frame, title, marker, move, help, description, cursor;
    float slideX, slideY, scale;
    std::array<std::uint16_t, 2> slideFlags;
    std::uint16_t slideState, selectionLimit;
    float depth;
    std::uint8_t moveListState, runtime;
    std::uint16_t reservedState;
};

inline constexpr SYGHDrawState kSyghInitialDrawState{
    .popup = {.centreX = 320.0f, .x = 264.0f, .y = 8.0f, .colour = 0x7f000000, .width = 256, .height = 176},
    .side = {.y = 215.0f, .depth = 2.0f, .colour = 0x1f0000ff, .width = 48, .height = 379},
    .underline = {.x = 320.0f, .y = 404.0f, .scaleX = 1.0f, .scaleY = 1.0f, .depth = 2.0f, .colour = 0x7f507dae, .width = 32, .height = 1, .flags = 1},
    .footer = {.x = 288.0f, .y = 403.0f, .depth = 3.0f, .colour = 0x7f0000ff, .width = 608, .height = 1},
    .buttons = {{{.x = 240.0f, .y = 310.0f, .scaleX = 1.0f, .scaleY = 1.0f, .depth = 8.0f, .width = 64, .height = 32, .flags = 5},
        {.x = 400.0f, .y = 310.0f, .scaleX = 1.0f, .scaleY = 1.0f, .depth = 8.0f, .width = 64, .height = 32, .flags = 5}}},
    .background = {.scaleX = 1.0f, .scaleY = 1.0f, .depth = 1.0f, .width = 256, .height = 256, .flags = 5},
    .frame = {.scaleX = 1.0f, .scaleY = 1.0f, .depth = 2.0f, .width = 128, .height = 128, .flags = 5},
    .title = {.x = 320.0f, .scaleX = 1.0f, .scaleY = 1.0f, .depth = 12.0f, .width = 512, .height = 32, .flags = 5},
    .marker = {.x = 616.0f, .y = 403.0f, .scaleX = 1.0f, .scaleY = 1.0f, .depth = 6.0f, .width = 32, .height = 64, .flags = 5},
    .move = {.depth = 8.0f, .flags = 5},
    .help = {.scaleX = 1.0f, .scaleY = 1.0f, .depth = 4.0f, .width = 32, .height = 512, .flags = 5},
    .description = {.y = 152.0f, .scaleX = 1.0f, .scaleY = 1.0f, .depth = 4.0f, .width = 32, .height = 512, .flags = 5},
    .cursor = {.y = 432.0f, .scaleY = 1.0f, .depth = 3.0f, .width = 16, .height = 16, .flags = 1},
    .slideX = -304.0f,
    .slideY = 240.0f,
    .scale = 1.0f,
    .selectionLimit = 90,
    .depth = 1.0f,
};

extern SYGHDrawState g_syghDrawState;
extern std::uint8_t g_moveHelpPopupState;
extern std::array<std::int16_t, 7> g_moveHelpSurfaceIds;

struct SYGHOverlayState {
    bool visible;
    bool opening;
    float x;
    float y;
    std::uint8_t animationSet;
    std::uint16_t moveId;
    std::uint16_t skillValue;
    std::array<char, 256> moveName;
    std::array<char, 256> instruction;
    std::array<std::array<char, 37>, 6> descriptionLines;
};

void InitializeMovesScreenRuntimeData();
HLib::Task *EnqueueSYGH();
void CtrlTrainingHelp_SYGH_Callback(HLib::TaskData *taskData);
void SYGH_Destroy();
void SYGH_SetMoveSkill(std::uint16_t animationSet, std::uint16_t moveId, std::uint16_t value);
void SYGH_RemoveMoveSkill(std::uint16_t animationSet, std::uint16_t moveId);
std::uint16_t SYGH_GetMoveSkill(std::uint16_t animationSet, std::uint16_t moveId);
void SYGH_ApplyMajorStepMoveSkillProgression();
void SYGH_OpenMoveList(std::uint16_t animationSet);
void SYGH_SetSelectedMove(std::uint16_t selection);
const SYGHOverlayState &SYGH_GetOverlayState() noexcept;

} // namespace shenmue::s1
