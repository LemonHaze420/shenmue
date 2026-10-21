#pragma once
#include "shenmue.h"
#include "common/math.hpp"

#include <cstdint>
#include <array>
#include <cstddef>

namespace shenmue::s2 {
enum class CameraState : std::int32_t;

// MemoPageState is never rebound to the save buffer.
inline constexpr float MEMO_PAGE_WIDTH = 512.0f, MEMO_PAGE_HEIGHT = 512.0f;
inline constexpr std::array<float, 2> MEMO_TEXT_FIRST_Y{54.0f, 72.0f}, MEMO_TEXT_LINE_SPACING{1300.0f, 1114.0f}, MEMO_TEXT_FONT_SCALE{1.0f, 0.9f};
inline constexpr std::array<std::int32_t, 5> MEMO_TITLE_PIECES{1, 1, 1, 1, 2};
inline constexpr std::uint32_t MEMO_TEXT_COLOUR = 0xff081219U, MEMO_TITLE_COLOUR = 0xff0f2878U;
inline constexpr std::array<const char *, 3> MEMO_TITLE_PATHS{
    "/ui/notebookpages/NotebookTitleTop.tga", "/ui/notebookpages/NotebookTitleVertical.tga", "/ui/notebookpages/NotebookTitleBot.tga"};
struct MemoPageState {
    std::int16_t selectedPage;
    std::int16_t latestPage;
    std::int16_t savedFlag;
    std::uint16_t requestFlags;
    std::int32_t category;
    std::uint32_t categoryMask;
};
static_assert(sizeof(MemoPageState) == 0x10);
static_assert(offsetof(MemoPageState, requestFlags) == 6);
static_assert(offsetof(MemoPageState, categoryMask) == 0x0c);

// BuildMemoTextures: two 256x128 RGBA TEXN records and a four-byte terminator.
struct MemoTexture {
    std::uint32_t signature;
    std::uint32_t size;
    std::array<char, 8> id;
    std::uint32_t gbix;
    std::uint32_t gbixSize;
    std::uint32_t gbixValue;
    std::uint32_t pvrt;
    std::uint32_t pvrtSize;
    std::uint32_t format;
    std::uint16_t width;
    std::uint16_t height;
#if SHENMUE_IS_PC
    std::array<std::uint32_t, 256 * 128> pixels;
#else
    std::array<std::uint16_t, 256 * 128> pixels;
#endif
};

struct MemoTextureTaskData {
    std::array<MemoTexture, 2> textures;
    std::uint32_t terminator;
};

static_assert(offsetof(MemoTexture, pixels) == 0x2c);
#if SHENMUE_IS_PC
static_assert(sizeof(MemoTexture) == 0x2002c);
static_assert(sizeof(MemoTextureTaskData) == 0x4005c);
#else
static_assert(sizeof(MemoTexture) == 0x1002c);
static_assert(sizeof(MemoTextureTaskData) == 0x2005c);
#endif

struct MemoCameraSave {
    std::uint16_t selectedHelp;
    std::uint16_t visibleHelp;
    std::uint32_t playerDisplay;
    std::uint32_t playerVisibility;
    float playerAlpha;
    CameraState state;
    std::uint16_t perspective;
    std::uint16_t reserved16;
    Vector3 position;
    Vector3 target;
    std::int32_t weatherMode;
};
static_assert(sizeof(MemoCameraSave) == 0x34);
static_assert(offsetof(MemoCameraSave, position) == 0x18);
static_assert(offsetof(MemoCameraSave, weatherMode) == 0x30);

// MEMO touches only WNDT's final control word; this view follows the selected
// platform's WIND payload rather than SCEN or lighting state.
struct MemoWindowTaskView {
#if SHENMUE_IS_PC
    std::byte precedingFields[0x2b0];
    std::int32_t customFlag;
    std::int32_t reserved2b4;
#else
    std::byte precedingFields[0x1e4];
    std::int32_t customFlag;
#endif
};
#if SHENMUE_IS_PC
static_assert(sizeof(MemoWindowTaskView) == 0x2b8);
static_assert(offsetof(MemoWindowTaskView, customFlag) == 0x2b0);
#else
static_assert(sizeof(MemoWindowTaskView) == 0x1e8);
static_assert(offsetof(MemoWindowTaskView, customFlag) == 0x1e4);
#endif

// InitializeMemoNotebookPages: bank zero borrows page-text pointers; bank one owns the
// four 800-byte text buffers. These are eight targets, not copy textures.
struct MemoPageRenderer {
    std::array<std::array<std::uint32_t, 4>, 2> targets{};
    std::array<std::array<bool, 4>, 2> dirty{};
    std::array<std::array<char, 800>, 4> text{};
    std::array<std::int32_t, 4> borrowedText{{-1, -1, -1, -1}};
    std::array<std::uint32_t, 3> titleTextures{};
    std::int32_t activePage{};
};

void MM_MemoCtrl(std::uint8_t nextFunctionIndex);
void MEMO_RefreshControllerInput();
std::int64_t RefreshPadTask(std::int32_t mode, std::int32_t inputMode);
// The GameNotebook menu's open (its Initialize, or its bind when an open was requested before it loaded).
void OpenMemoFromNotebookMenu();
// The page renderer's pending open: the memo opens on `page`.
void OpenMemoAtPage(std::int32_t page);
// The GameNotebook menu's close: 1 once the memo is shut; otherwise it requests the close (states 8 and 9 are
// already closing) and returns 0.
std::int64_t CloseMemoFromNotebookMenu();
// SetupNotebook: InitShenmue creates both banks' four 512x512 page targets and marks them for a redraw.
void SetupNotebook();
std::int64_t CreateMemoNotebookPages();
std::int64_t DestroyMemoNotebookPages();
void RenderMemoNotebookPages();

bool NotifyMemoFlag(std::int32_t flag, bool selected);
std::uint16_t GetMemoPageState(std::int32_t selector) noexcept;
void InitializeMemoPageState(std::int16_t page, std::int16_t flag) noexcept;
void ResetMemoPageState() noexcept;

void RequestMemoClose() noexcept;
} // namespace shenmue::s2
