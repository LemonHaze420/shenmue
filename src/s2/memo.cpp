#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/memo.hpp"
#include "s2/event_matrix_dc.hpp"
#include "s2/camera_state_query_dc.hpp"
#include "s2/character_model_dc.hpp"
#include "s2/conversation_playback_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/font.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/math.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/motion_node_dc.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/npc_data_dc.hpp"
#include "s2/memo_flags_dc.hpp"
#include "s2/model_clone_dc.hpp"
#include "s2/character_draw_tasks_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/pad_control_dc.hpp"
#include "s2/scene_assets_dc.hpp"
#include "s2/scene_data.hpp"
#include "s2/sound_table_dc.hpp"
#include "s2/ui_state_dc.hpp"
#include "s2/window.hpp"
#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"
#include <string.h>
#include <stdio.h>
#include <math.h>
#else
#include "s2/memo.hpp"

#include "d3t/image_texture_loader.hpp"
#include "d3t/model.hpp"
#include "d3t/object_store.hpp"
#include "d3t/texture.hpp"
#include "d3t/ui_renderer.hpp"
#include "d3t/ui_runtime.hpp"
#include "d3t/dx11_renderer.hpp"
#include "s2/ui_scroll_list.hpp"
#include "s2/game_runtime.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/archive_data.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/font.hpp"
#include "s2/hlib.hpp"
#include "s2/input.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/motion.hpp"
#include "s2/mt7.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/story_state.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

#include "s2/memo_data.inl"

MemoPageState memoPageState{};

MemoPageRenderer memoPages{};

std::array<std::uint32_t, 3> memoTitleRequests{};

std::array<d3t::UIViewImage, 3> memoTitleImages{};

std::int32_t memoTitlePhase{}, memoDebugMode{};

std::uint8_t memoForcePageRedraw{};

nk_panel memoPanel{};

constexpr std::array<std::array<std::int16_t, 2>, 5> memoCategories{{{{-1, 1}}, {{138, 5}}, {{997, 65}}, {{424, 107}}, {{1348, 117}}}};

constexpr std::array<std::uint16_t, 5> bitmapLineOffsets{0x6708, 0x4f08, 0x3708, 0x1f08, 0x0708};

constexpr std::array<const char *, 4> memoTextureNames{"AAA_TEX", "BBB_TEX", "CCC_TEX", "DDD_TEX"};

HLib::Task *memoTask;

PadControlEntry *memoPad;

PadControlEntry *previousPad{};

MemoTextureTaskData *memoTextureData;

std::array<std::uint32_t, 4> memoTextureHandles;

std::array<std::uint32_t, 4> memoPageTextureAliases{};

std::uint32_t memoNotebookModelHandle{};

MT7Model memoModel;

std::byte *memoModelSource{};

std::vector<std::uint32_t> memoModelOwnedTextures;

std::array<MT7Node *, 4> memoNodes{};

MT7Node *turningNode{};

MemoCameraSave memoCamera{};

bool cameraSaved{};

ConversationBuffer *memoAudio;

std::int32_t memoState;

std::int32_t memoSecondState{};

std::int32_t memoInputMode{};

std::int32_t repeatTurn{};

std::int32_t explicitPage{-1};

// The page renderer opens the memo on memoPendingOpenPage while memoPendingOpen is set; no native writer sets it.
std::int32_t memoPendingOpen{};

std::int32_t memoPendingOpenPage{};

std::int16_t pagePosition{};

std::int16_t pageThickness{};

std::int16_t nextPage{};

std::uint16_t pageMotion{};

std::uint16_t previousRightTrigger{};

std::int32_t turnFrames{};

std::int32_t turnDelay{};

std::int32_t turnCounter{};

std::int32_t turnAngle{};

float pageOffset{};

float pageOffsetStep{};

float notebookTranslation{};

float motionFrame{};

float motionStep{1.0f};

bool drawNotebook{};

bool ownsSharedItems{};

std::int32_t previousPostFxBank{};

std::array<std::uint32_t, 4> pageModelBindings{};

} // namespace

#if defined(SHENMUE_NULL_LIBRARY)
#pragma region NullLibrary
#endif

#if defined(SHENMUE_NULL_LIBRARY)
#pragma endregion
#endif

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
namespace {

std::string FormatMemoTransferredMoney(bool hongKong);
void RebindMemoModelTextures();
void SwapMemoPageTargets(std::uint32_t first, std::uint32_t second);
void RebindNotebookWritingAreas();
void UpdateNotebook(std::int16_t page, std::uint32_t left, std::uint32_t right);
bool SetupMemoTextureTaskData();
bool LoadNotebookModel();
void ReleaseMemoResources();
bool SetupMemoCamera();
void RestoreMemoCamera();
void BeginMemoPageTurn();
bool StepMemoPageTurn();
void MEMO_Callback(void *);
void DrawMemoPageText(std::uint32_t bank, std::uint32_t index);

} // namespace

bool NotifyMemoFlag(std::int32_t flag, bool selected);
void MM_MemoCtrl(std::uint8_t nextFunctionIndex);
std::int64_t RefreshPadTask(std::int32_t mode, std::int32_t inputMode);
void MEMO_RefreshControllerInput();
void RequestMemoClose() noexcept;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
std::int32_t EnsureMemoTitleTextures() {
    if (memoTitlePhase == 3)
        return memoTitlePhase;
    if (memoTitlePhase == 0) {
        for (std::size_t index = 0; index < memoTitleRequests.size(); ++index)
            memoTitleRequests[index] = SetupUsageForTexture(MEMO_TITLE_PATHS[index], 0);
    }
    memoTitlePhase = 3;
    for (std::size_t index = 0; index < memoTitleImages.size(); ++index) {
        d3t::UIViewImage &image = memoTitleImages[index];
        if (!image.ready) {
            const std::uint32_t texture = GetShaderDataByIndex(memoTitleRequests[index]);
            std::uint32_t width, height;
            if (texture && d3t ::GetTexture2DDimensions(texture, &width, &height)) {
                image.widthScale = static_cast<float>(width) / 1920.0f;
                image.heightScale = static_cast<float>(height) / 1080.0f;
                image.textureHandle = static_cast<std::int32_t>(texture);
                image.width = static_cast<std::uint16_t>(width);
                image.height = static_cast<std::uint16_t>(height);
                image.region = {0, 0, image.width, image.height};
                image.colour = MEMO_TITLE_COLOUR;
                image.ready = 1;
                memoPages.titleTextures[index] = texture;
            }
        }
        if (!image.ready)
            memoTitlePhase = 2;
    }
    return memoTitlePhase;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::uint32_t ReadMemoWord(const std::byte *source) {
    std::uint32_t value{};
    std::memcpy(&value, source, sizeof(value));
    return value;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

MapControlData *MemoMapControl() {
    return static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void SetMemoWindowFlag(HLib::Task *task, std::int32_t flag) {
    if (auto *data = static_cast<MemoWindowTaskView *>(HLib::GetTaskParameter(task)))
        data->customFlag = flag;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::uint32_t ExpandMemoColour(std::uint16_t colour) {
    return 17U * ((colour & 15U) | (((colour >> 4U) & 15U) << 8U) | (((colour >> 8U) & 15U) << 16U) | ((colour >> 12U) << 24U));
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// RasterMemoGlyph draws a one-pixel down/right silhouette, with vertically reversed rows.
// Half-width glyphs consume 30 bytes but write thirteen columns; full-width
// glyphs consume 45 bytes, write 25 columns and advance by 52 pixels.
void RasterMemoGlyph(MemoTexture &texture, std::size_t destination, std::uint16_t code, std::uint16_t colour) {
    const FontRuntimeData *const font = GetFontRuntimeData();
    if (!font)
        return;
    const bool halfWidth = code < 0x2100U || (code & 0x8080U) == 0x8080U;
    const std::byte *source{};
    std::uint32_t columns{};
    std::uint32_t bits{};
    if (halfWidth) {
        if (!font->ryouData)
            return;
        source = static_cast<const std::byte *>(font->ryouData) + 30 * (code & 255U);
        columns = 16;
        bits = 240;
    } else {
        if (!font->kanjiData)
            return;
        source =
            static_cast<const std::byte *>(font->kanjiData) + 45 * (94 * static_cast<std::int32_t>(code >> 8U) + static_cast<std::int32_t>(code & 255U) - 3135);
        columns = 24;
        bits = 360;
    }
    std::array<std::uint8_t, 400> raster{};
    for (std::uint32_t bit = 0; bit < bits; ++bit) {
        if ((std::to_integer<std::uint8_t>(source[bit >> 3U]) & (1U << (bit & 7U))) == 0)
            continue;
        const std::uint32_t at = 25 * (bit / columns) + bit % columns;
        raster[at] |= 2;
        raster[at + 1] |= 1;
        raster[at + 25] |= 1;
        raster[at + 26] |= 1;
    }
    const std::uint32_t rgba = ExpandMemoColour(colour);
    for (std::uint32_t row = 0; row < 16; ++row) {
        const std::size_t at = destination + (15 - row) * 256;
        for (std::uint32_t col = 0; col < (halfWidth ? 13U : 25U); ++col) {
            if (at + col < texture.pixels.size())
                texture.pixels[at + col] = (raster[row * 25 + col] & 2U) ? rgba : 0;
        }
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void RasterMemoLine(std::uint32_t line, std::string_view text) {
    MemoTexture &texture = memoTextureData->textures[line / 5];
    std::size_t destination = bitmapLineOffsets[line % 5];
    std::uint16_t colour = 0xb110;
    if (!text.empty() && text.front() == '@') {
        colour = 0xbf00;
        text.remove_prefix(std::min<std::size_t>(6, text.size()));
    }
    for (std::size_t cursor = 0; cursor < text.size();) {
        const auto first = static_cast<std::uint8_t>(text[cursor++]);
        if (first >= 0xa1U && first <= 0xfeU) {
            if (cursor == text.size())
                break;
            const auto second = static_cast<std::uint8_t>(text[cursor++]);
            RasterMemoGlyph(texture, destination, static_cast<std::uint16_t>(((first << 8U) | second) & 0x7f7fU), colour);
            destination += 52;
        } else {
            RasterMemoGlyph(texture, destination, first | 0x100U, colour);
            destination += 13;
        }
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void shenExecuteAICACommand(std::uint32_t command, std::uint32_t bank, std::int32_t value);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void ExecuteAicaCommand(std::uint32_t command, std::uint32_t bank, std::int32_t value);
bool SelectMemoFlagText(char *text, char *ruby, std::int32_t line);
void RasterizeMemoLine(std::uint16_t *destination, const char *text, const char *ruby, std::int16_t width, const std::uint16_t *colours);
#define MEMO_AICA_COMMAND ExecuteAicaCommand
#else
#define MEMO_AICA_COMMAND shenExecuteAICACommand
#endif

namespace {

HLib::Task *memoTask;

struct {
    PadControlEntry *entry, *previous;
} memoPad;

ConversationBuffer *memoAudio;

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
struct ProvisionalMemoTextureState {
    std::byte retained[4];
    MemoTextureTaskData *data;
    std::int32_t modelTextureCount;
};
typedef char MemoTextureStateSizeCheck[sizeof(ProvisionalMemoTextureState) == 12 ? 1 : -1];
typedef char MemoTextureStateDataCheck[offsetof(ProvisionalMemoTextureState, data) == 4 ? 1 : -1];
typedef char MemoTextureStateCountCheck[offsetof(ProvisionalMemoTextureState, modelTextureCount) == 8 ? 1 : -1];
ProvisionalMemoTextureState memoTextureData;
#define MEMO_TEXTURE_DATA memoTextureData.data
#define MEMO_TEXTURE_COUNT memoTextureData.modelTextureCount
#else
MemoTextureTaskData *memoTextureData;
#define MEMO_TEXTURE_DATA memoTextureData
#define MEMO_TEXTURE_COUNT memoModelTextureCount
#endif

MemoTexture *memoTextures[2] = {0, 0};

std::int32_t memoTextureHandles[4] = {-1, -1, -1, -1};

MemoTextureName memoTextureNames[2][16];

std::int32_t memoOriginalTextures[32];
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t memoModelTextureCount = 0;
#endif

struct MemoWork {
    std::int16_t position, thickness, motion;
    MemoModelDrawContext draw;
    MT7Model *model;
    MT7Node *cover, *leftPages, *turningPage, *rightPages;
    float pageOffset, pageStep, motionFrame, motionStep;
    std::int32_t turnFrames, turnDelay, turnCounter, turnAngle;
    MemoCameraSave *savedCamera;
};
typedef char MemoWorkSizeCheck[sizeof(MemoWork) == 0x74 ? 1 : -1];

MemoWork memo;

MT7Node *memoMovingNode;

MemoCameraSave memoCamera;

struct MemoStateWork {
    std::int32_t phase, mode, inputMode, repeat;
};

MemoStateWork memoState;

std::int16_t memoNextPage = 0;

std::uint16_t memoPreviousTrigger = 0;

float memoBaseX, memoBaseZ, memoTranslation;

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
namespace {

void BindMemoPad(std::int32_t enabled);
void UpdateMemoTexturePage(std::int16_t page);
void PrepareMemoNextPage();
void PrepareMemoPreviousPage();
void FinishMemoTexturePage(std::int32_t previous);
void InitializeMemoTexture(MemoTexture &texture, const char *name, std::int16_t width, std::int16_t height);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t SetupMemoTextures();
#else
bool SetupMemoTextures();
#endif
bool LoadMemoModel();
void ReleaseMemoResources();
bool SetupMemoCamera();
void RestoreMemoCamera();
bool OpenMemoResources();
void RefreshMemoFlagPage();
std::int16_t GetMemoOpeningPage();
void SetMemoOpeningPage(std::int16_t page);
void BeginMemoPageTurn();
bool StepMemoPageTurn();
void UpdateMemoHelp();
void ResetMemoRuntime();
void ExpandMemoText(char *destination, const char *source);
const char *GetMemoText(const std::byte *table, std::int32_t index);
void FormatMemoTransferredMoney(char *destination, std::int32_t hongKong);
std::uint8_t PrepareMemoTextLine(char *destination, const char *source, std::uint16_t *colours, std::int32_t *selection);
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void RasterizeMemoRuby(FontRuntimeData *font, std::uint16_t glyph, std::uint16_t *destination, std::int32_t bytePitch);
void RasterizeMemoLine(std::uint16_t *destination, const char *text, const char *ruby, std::int32_t width, const std::uint16_t *colours);
#endif
void SetMemoModelLightDirection(Vector3 position, const Vector3 &camera, std::int32_t yaw, std::uint32_t light);
void PrepareMemoModelLighting(const MemoModelDrawContext &context, std::uint32_t light);
void DrawMemoModel(const MemoModelDrawContext &context);
void RestoreMemoModelLighting(std::uint32_t light);

} // namespace

void MM_MemoCtrl(std::int8_t queue);
void SetMemoTextureName(std::byte *header, const char *name);
void ReleaseNamedTexture_Wrapper(std::int32_t index);
std::int32_t camCtrlSetPerspectiveWrapper(std::int16_t perspective);
std::int32_t camCtrlGetPerspectiveWrapper();
bool RefreshPadTask(std::int32_t mode, std::int32_t enabled);
void MEMO_RefreshControllerInput();
std::int32_t CancelSelectorMemo(std::int32_t force);
void CloseActiveMemo();
void RequestMemoClose();
void MEMO_Callback(void *);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void RenderMemoPageText(std::int32_t page, MemoTexture **textures);
#else
void RenderMemoPageText(std::int16_t page, MemoTexture **textures);
#endif
void DrawMemoModelContext(void *parameter);
void DrawReflectorBoardContext(void *parameter);
#else
namespace {

std::int32_t EnsureMemoTitleTextures();
std::uint32_t ReadMemoWord(const std::byte *source);
MapControlData *MemoMapControl();
void SetMemoWindowFlag(HLib::Task *task, std::int32_t flag);
std::uint32_t ExpandMemoColour(std::uint16_t colour);
void RasterMemoGlyph(MemoTexture &texture, std::size_t destination, std::uint16_t code, std::uint16_t colour);
void RasterMemoLine(std::uint32_t line, std::string_view text);
void ReleaseMemoResources();
bool SetupMemoCamera();
void RestoreMemoCamera();
void SetMemoState7(std::int16_t page);
void BeginMemoPageTurn();
bool StepMemoPageTurn();
void DrawMemoNotebook(void *);
void PlayMemoAudio(std::int32_t volume);
void MEMO_Callback(void *);
void DrawMemoPageText(std::uint32_t bank, std::uint32_t index);
std::string FormatMemoTransferredMoney(bool hongKong);
void ReplaceMemoMoney(std::string &text, std::string_view key, std::int32_t value);
void PrepareMemoNotebookLine(
    std::int32_t page, std::uint32_t line, std::uint32_t destination, std::uint32_t borrowed, char messageType, std::string_view legacy);
void PrepareNotebook(std::int32_t page, std::uint32_t left, std::uint32_t right);
void RebindMemoModelTextures();
void SwapMemoPageTargets(std::uint32_t first, std::uint32_t second);
void FillNotebookWritingArea(void *areas, const ModelMaterialDescriptor &descriptor);
std::uint32_t EnsureNotebookPageTarget(std::uint32_t kind, std::uint8_t index);
void SetNotebookPageTexture(std::uint32_t kind, std::uint32_t legacyTexture, std::uint8_t index);
void RebindNotebookWritingAreas();
void NotebookModelLoaded(void *, d3t::ObjectSource *source);
void UpdateNotebook(std::int16_t page, std::uint32_t left, std::uint32_t right);
bool SetupMemoTextureTaskData();
bool LoadNotebookModel();

} // namespace

void MM_MemoCtrl(std::uint8_t nextFunctionIndex);
std::int64_t RefreshPadTask(std::int32_t mode, std::int32_t inputMode);
void OpenMemoFromNotebookMenu();
void OpenMemoAtPage(std::int32_t page);
std::int64_t CloseMemoFromNotebookMenu();
void MEMO_RefreshControllerInput();
void RequestMemoClose() noexcept;
void SetupNotebook();
std::int64_t CreateMemoNotebookPages();
void ReleaseNotebookPageTargets();
std::int64_t DestroyMemoNotebookPages();
std::int64_t GetMemoNotebookPagesInitState();
void RenderMemoNotebookPages();
std::uint16_t GetMemoPageState(std::int32_t selector) noexcept;
void InitializeMemoPageState(std::int16_t page, std::int16_t flag) noexcept;
void ResetMemoPageState() noexcept;
bool NotifyMemoFlag(std::int32_t flag, bool selected);
const MT7Model *GetNullMemoPresentation(const MemoPageRenderer **pages, float *translation);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void MM_MemoCtrl(std::int8_t queue) {
    ResetMemoRuntime();
    memoPad.entry = AllocatePadControlEntry();
    memoPad.previous = 0;
    memoTask = HLib::EnqueueTaskWithoutParameter(MEMO_Callback, queue, 0, SHEN_NAME(MEMO));
    if (memoTask)
        HLib::SetupTimedTask(memoTask, 0);
}

#else

void MM_MemoCtrl(std::uint8_t nextFunctionIndex) {
    // InitMemo preserves the independent persistent page record.
    memoTask = nullptr;
    memoPad = nullptr;
    previousPad = nullptr;
    memoState = 0;
    memoSecondState = 0;
    memoInputMode = 0;
    repeatTurn = 0;
    explicitPage = -1;
    drawNotebook = false;
    cameraSaved = false;
    ownsSharedItems = false;
    turnDelay = 8;
    memoTextureHandles.fill(UINT32_MAX);
    memoPad = AllocatePadControlEntry();
    memoTask = HLib::EnqueueTaskWithoutParameter(MEMO_Callback, nextFunctionIndex, 0, SHEN_NAME(MEMO));
    if (memoTask)
        HLib::SetupTimedTask(memoTask, 0);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
bool RefreshPadTask(std::int32_t mode, std::int32_t enabled) {
    bool opened = false;
    if (!enabled || !g_memoRubyTable || !g_memoData || !g_memoMasterTable)
        return false;
    const std::int32_t selectable = IsItemSelectable();
    if (selectable)
        return opened;
    if (memoTask) {
        HLib::ResolveDeferredCallback(memoTask);
        opened = true;
        memoState.phase = 1;
        memoState.mode = mode;
        memoState.inputMode = enabled;
        memoState.repeat = selectable;
        icsToggleIconState(5, 1);
        BindMemoPad(1);
    }
    if (!opened)
        MEMO_AICA_COMMAND(0x800ab, 0, 0);
    else if (!mode)
        MEMO_AICA_COMMAND(0x500ab, 0, 0);
    return opened;
}

#else

std::int64_t RefreshPadTask(std::int32_t mode, std::int32_t inputMode) {
    if (!inputMode || !g_memoRubyTable || !g_memoData || !g_memoMasterTable || (GetCurrentItemSelectState() & 0x20U))
        return 0;
    if (!memoTask) {
        (void)ExecuteAicaCommand(0x800ab, 0, 0);
        return 0;
    }
    HLib::ResolveDeferredCallback(memoTask);
    memoState = 1;
    memoSecondState = mode;
    memoInputMode = inputMode;
    repeatTurn = 0;
    drawNotebook = false;
    if (mode == 2)
        memoPageState.requestFlags |= 2U;
    icsToggleIconState(5, 1);
    if (memoPad && (memoPad->flags & 2U) == 0)
        previousPad = BindPadControlEntry(memoPad);
    return 1;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void OpenMemoFromNotebookMenu() {
    (void)RefreshPadTask(0, 1);
    SetHudItemPromptIcon(0);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void OpenMemoAtPage(std::int32_t page) {
    explicitPage = page;
    (void)RefreshPadTask(0, 1);
    SetHudItemPromptIcon(0);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::int64_t CloseMemoFromNotebookMenu() {
    if (memoState == 0)
        return 1;
    if (static_cast<std::uint32_t>(memoState - 8) > 1U)
        memoState = 8;
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void MEMO_RefreshControllerInput() {
    BindMemoPad(0);
    if (!memoTask)
        return;
    HLib::SetupTimedTask(memoTask, 0);
    ReleaseMemoResources();
    memoState.phase = 0;
    icsToggleIconState(5, 0);
    StopConversationBuffers(0);
    SetSoundPaused(0);
}

#else

void MEMO_RefreshControllerInput() {
    if (memoPad && previousPad && (memoPad->flags & 2U)) {
        BindPadControlEntry(previousPad);
        previousPad = nullptr;
    }
    if (memoTask)
        HLib::SetupTimedTask(memoTask, 0);
    ReleaseMemoResources();
    memoState = 0;
    drawNotebook = false;
    icsToggleIconState(5, 0);
    scenCvsCtrlFree();
    PrimeAicaEmulationForPause(0);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void ResetMemoRuntime() {
    MemoStateWork *state = &memoState;
    memoTask = 0;
    for (std::int32_t index = 0; index < 4; ++index)
        memoTextureHandles[index] = -1;
    for (std::int32_t index = 0; index < 2; ++index)
        memoTextures[index] = 0;
    for (std::int32_t index = 0; index < 2; ++index)
        reinterpret_cast<PadControlEntry **>(&memoPad)[index] = 0;
    memo.model = 0;
    memo.draw.root = 0;
    memo.cover = 0;
    memo.leftPages = 0;
    memo.turningPage = 0;
    memo.rightPages = 0;
    memo.motionFrame = 0.0f;
    memo.turnDelay = 8;
    memo.pageOffset = 0.0f;
    memo.savedCamera = 0;
    memo.draw.colour.x = 2.0f;
    memo.draw.colour.y = 2.0f;
    memo.draw.colour.z = 2.0f;
    memo.draw.colour.w = 2.0f;
    memo.draw.style = 0;
    memoNextPage = 0;
    state->repeat = 0;
    state->phase = 0;
    state->mode = 0;
    state->inputMode = 1;
    MEMO_TEXTURE_DATA = 0;
    RefreshMemoFlagPage();
}
#else
// @verified - d
void ResetMemoRuntime() {
    memoTask = 0;
    for (std::int32_t index = 0; index < 4; ++index)
        memoTextureHandles[index] = -1;
    memoTextures[0] = memoTextures[1] = 0;
    memoPad.entry = memoPad.previous = 0;
    memo.model = 0;
    memo.draw.root = 0;
    memo.cover = memo.leftPages = memo.turningPage = memo.rightPages = 0;
    memo.motionFrame = memo.pageOffset = memoTranslation = 0.0f;
    memo.turnDelay = 8;
    memo.savedCamera = 0;
    memo.draw.colour.x = memo.draw.colour.y = memo.draw.colour.z = memo.draw.colour.w = 2.0f;
    memo.draw.style = 0;
    memoNextPage = 0;
    memoState.phase = memoState.mode = memoState.repeat = 0;
    memoState.inputMode = 1;
    MEMO_TEXTURE_DATA = 0;
    RefreshMemoFlagPage();
}
#endif

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void UpdateMemoHelp() {
    std::uint32_t flags = 0x21;
    const std::int32_t next = memoNextPage - 1;
    if (g_memoLatestPage <= 0)
        return;
    if (static_cast<std::int16_t>(g_memoPage) == g_memoLatestPage)
        return;
    if (next >= static_cast<std::int16_t>(g_memoPage) && next < g_memoLatestPage)
        flags |= 0x4000;
    if (flags != GetPersistentHelpIconFlags(0))
        CheckHelpIcon(flags, 0, CheckHelpIconOp_Set);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void MEMO_Callback(void *) {
    MemoStateWork *state = &memoState;
    std::uint16_t repeated = 0, pressed = 0;
    if (memoPad.entry) {
        pressed = ctrlGetPadInputBuffer0(memoPad.entry)->pressedPriority;
        repeated = ctrlGetPadInputBuffer0(memoPad.entry)->repeatedPresses;
        ctrlGetPadInputBuffer0(memoPad.entry);
    }
    std::int32_t draw = 0;
    switch (state->phase) {
    case 0:
        break;
    case 1:
        if (OpenMemoResources()) {
            if (GetUiFlag(2))
                SetHelpIconMenuFlags(0x21);
            else
                AddHelpIconMenuFlags(0x21);
            SetSoundPaused(1);
            memoAudio = shenPlayAudioWithFlags("MEMO", 1);
            SetConversationVolume(memoAudio, -75);
            if (!state->inputMode) {
                SetMemoOpeningPage(1);
            } else {
                g_memoPage = GetMemoOpeningPage();
                if (g_memoFlag == g_memoPreviousFlag && g_memoLatestPage > 0) {
                    g_memoPage = g_memoLatestPage;
                    g_memoPreviousFlag = -1;
                }
                SetMemoOpeningPage(static_cast<std::int16_t>(g_memoPage));
            }
            if (state->mode != 2 && g_combatStatistics.memoOpenCount < 0xffff)
                ++g_combatStatistics.memoOpenCount;
            state->phase = 7;
            draw = 1;
        } else {
            MEMO_RefreshControllerInput();
            MEMO_AICA_COMMAND(0x800ab, 0, 0);
        }
        break;
    case 7:
        draw = 1;
        if (pressed & 0x200U) {
            state->phase = 8;
        } else {
            memo.draw.position.x += CalculateFixedCosine(memo.draw.rotation.y) * 0.0018f;
            memo.draw.position.z += CalculateFixedSine(memo.draw.rotation.y) * -0.0018f;
            memo.turnAngle += 0x5b0;
            if (memo.turnAngle < 0x8000) {
                memo.cover->rotation.z = memo.turnAngle + 0x38e;
                memo.leftPages->rotation.z = memo.turnAngle;
            } else {
                memo.turnAngle = 0;
                memo.cover->rotation.z = 0x838e;
                if (memo.position > 0)
                    memo.leftPages->rotation.z = 0x8000;
                if (++memo.position > 201)
                    memo.position = 202;
                state->phase = 2;
            }
        }
        break;
    case 2:
        if (!cvsHasValidConverseData(memoAudio)) {
            memoAudio = shenPlayAudioWithFlags("MEMO", 1);
            SetConversationVolume(memoAudio, -75);
        }
        draw = 1;
        if (pressed & 0x200U) {
            state->phase = 8;
            repeated = 0;
        } else if (repeated & 0x8000U) {
            if (memo.position > 0) {
                state->phase = 5;
                if (memoPreviousTrigger != repeated)
                    state->repeat = 0;
                else
                    state->repeat = 1;
            }
        } else if (repeated & 0x4000U) {
            if (memo.position <= 201) {
                state->phase = 3;
                if (memoPreviousTrigger != repeated)
                    state->repeat = 0;
                else
                    state->repeat = 1;
            }
        } else if (pressed & 0x1000U) {
            SetMemoOpeningPage(1);
        } else if (pressed & 0x2000U) {
            SetMemoOpeningPage(GetMemoOpeningPage());
        }
        break;
    case 5:
        draw = 1;
        if (--memo.position < 1) {
            memo.position = 0;
            memo.leftPages->scale.x = 0.0f;
            memo.leftPages->scale.y = 0.0f;
            memo.leftPages->scale.z = 0.0f;
        }
        --memoNextPage;
        PrepareMemoPreviousPage();
        if (!memo.position) {
            memoMovingNode = memo.cover;
            memoMovingNode->rotation.z = 0x8000;
            memo.pageOffset = 0.0f;
            memo.turnFrames = 22;
            MEMO_AICA_COMMAND(0xb00ab, 0, 0);
        } else {
            memoMovingNode = memo.turningPage;
            memoMovingNode->rotation.z = 0x8000;
            memoMovingNode->scale.x = 1.0f;
            memoMovingNode->scale.z = 1.0f;
            memoMovingNode->scale.y = 1.0f;
            memo.leftPages->scale.y = (218 - memo.thickness) * 0.0046999999f;
            memo.pageOffset = (212 - memo.thickness) * 0.000039200000f;
            memoMovingNode->position.y = memo.pageOffset;
            BeginMemoPageTurn();
            memo.pageStep = (0.0f - (212 - memo.thickness) * 0.0000027000001f - memo.pageOffset) / memo.turnFrames;
            if (memo.position == 1) {
                memo.leftPages->scale.x = 0.0f;
                memo.leftPages->scale.y = 0.0f;
                memo.leftPages->scale.z = 0.0f;
            }
            MEMO_AICA_COMMAND(0xc00ab, 0, 0);
        }
        state->phase = 6;
        break;
    case 6:
        draw = 1;
        if (StepMemoPageTurn()) {
            memoMovingNode->rotation.z = 0;
            if (memoMovingNode != memo.cover)
                memo.cover->rotation.z = 0x838e;
            if (++memo.thickness > 211)
                memo.thickness = 212;
            memo.rightPages->scale.y = (memo.thickness + 2) * 0.0046999999f;
            if (memo.position == 1) {
                memo.leftPages->rotation.z = 0;
                memo.rightPages->scale.y = 1.0f;
            } else if (memo.position == 201) {
                memo.rightPages->rotation.z = 0;
                memo.rightPages->scale.x = 1.0f;
                memo.rightPages->scale.z = 1.0f;
            }
            state->phase = 2;
            FinishMemoTexturePage(1);
        } else if (memoMovingNode == memo.cover) {
            memoMovingNode->rotation.z = 0x838e - memo.turnAngle;
        } else {
            memoMovingNode->rotation.z = 0x8000 - memo.turnAngle;
            memo.cover->rotation.z = 0x838e;
        }
        break;
    case 3:
        draw = 1;
        PrepareMemoNextPage();
        ++memoNextPage;
        if (!memo.position) {
            memoMovingNode = memo.cover;
            memoMovingNode->rotation.z = 0;
            memo.pageOffset = 0.0f;
            memo.turnFrames = 22;
            MEMO_AICA_COMMAND(0xb00ab, 0, 0);
        } else {
            memoMovingNode = memo.turningPage;
            memoMovingNode->rotation.z = 0;
            memoMovingNode->scale.x = 1.0f;
            memoMovingNode->scale.z = 1.0f;
            memoMovingNode->scale.y = 1.0f;
            if (--memo.thickness < 2)
                memo.thickness = 1;
            memo.rightPages->scale.y = (memo.thickness + 2) * 0.0046999999f;
            memo.pageOffset = 0.0f - memo.position * 0.0000027000001f;
            memoMovingNode->position.y = memo.pageOffset;
            BeginMemoPageTurn();
            memo.pageStep = (memo.position * 0.000032899999f - memo.pageOffset) / memo.turnFrames;
            if (memo.position == 201) {
                memo.rightPages->scale.x = 0.0f;
                memo.rightPages->scale.y = 0.0f;
                memo.rightPages->scale.z = 0.0f;
            }
            MEMO_AICA_COMMAND(0xc00ab, 0, 0);
        }
        state->phase = 4;
        break;
    case 4:
        draw = 1;
        if (StepMemoPageTurn()) {
            memoMovingNode->rotation.z = 0x8000;
            memo.cover->rotation.z = 0x838e;
            memo.leftPages->scale.y = (219 - memo.thickness) * 0.0046999999f;
            if (memo.position == 1) {
                memo.leftPages->rotation.z = 0x8000;
                memo.leftPages->scale.x = 1.0f;
                memo.leftPages->scale.z = 1.0f;
            }
            if (++memo.position > 201)
                memo.position = 202;
            state->phase = 2;
            FinishMemoTexturePage(0);
        } else if (memoMovingNode == memo.cover) {
            memoMovingNode->rotation.z = memo.turnAngle + 0x38e;
        } else {
            memoMovingNode->rotation.z = memo.turnAngle;
            memo.cover->rotation.z = 0x838e;
        }
        break;
    case 8:
        RestoreMemoCamera();
        state->phase = 9;
        break;
    case 9:
        MEMO_RefreshControllerInput();
        g_memoPreviousFlag = g_memoFlag;
        break;
    default:
        state->phase = 8;
        break;
    }
    if (draw && memo.draw.root) {
        UpdateMemoHelp();
        RequestHelpIconSelection(0x200);
        HLib::ExecuteCallbackNow(DrawReflectorBoardContext, &memo.draw);
    }
    memoPreviousTrigger = repeated;
}

#else

namespace {

// MEMO_Callback: each case consumes one scheduler visit. In particular state 8
// restores the camera, while state 9 retires widgets, resources and audio.
void MEMO_Callback(void *) {
    const std::uint16_t left = memoPad ? memoPad->input.pressedPriority : 0;
    std::uint16_t right = memoPad ? memoPad->input.held : 0;
    switch (memoState) {
    case 0:
        drawNotebook = false;
        previousRightTrigger = right;
        return;
    case 1: {
        ShowElementByIndex(UIElement::NotebookPages);
        if (!IsSharedItemsWidgetOwnedBy(-1)) {
            (void)ShowSharedItemsWidget(0, 1);
            ShowElementByIndex(UIElement::SharedItems);
            ownsSharedItems = true;
        }
        const std::uint32_t loadState = static_cast<std::uint32_t>(GetNpcNewLoadState());
        if (loadState)
            EV_NPC_NewLoadSw(0);
        (void)EV_ResetFreeTalkFlags();
        const bool opened = SetupMemoTextureTaskData() && LoadNotebookModel() && SetupMemoCamera();
        if (!opened)
            ReleaseMemoResources();
        if (loadState)
            EV_NPC_NewLoadSw(1);
        if (opened) {
            g_uiElements[33].isVisible = GetUiFlag(2) != 0;
            PrimeAicaEmulationForPause(1);
            PlayMemoAudio(-235);
            std::int32_t page = explicitPage;
            if (page <= -1)
                page = (memoPageState.requestFlags & 3U) == 1 ? memoPageState.selectedPage : memoPageState.latestPage;
            else
                explicitPage = -1;
            SetMemoState7(static_cast<std::int16_t>(page));
            memoPageState.requestFlags &= ~3U;
            memoPageState.categoryMask = 0;
            for (std::size_t index = 0; index < memoCategories.size(); ++index)
                if (memoCategories[index][0] < 0 || EV_GetPlayerFlag(PlayerFlagReadMode::Internal, static_cast<std::int16_t>(memoCategories[index][0])))
                    memoPageState.categoryMask |= 1U << index;
            memoState = 7;
            previousPostFxBank = static_cast<std::int32_t>(GetPostFxBankValue(1));
            if (previousPostFxBank == 1)
                SelectPostFxBank(1, 0);
        } else {
            MEMO_RefreshControllerInput();
            (void)ExecuteAicaCommand(0x800ab, 0, 0);
        }
        break;
    }
    case 2:
        if (!cvsHasValidConverseData(memoAudio))
            PlayMemoAudio(-75);
        if (left & 0x200U) {
            right = 0;
            memoState = 8;
        } else if (right & 0x8000U) {
            if (pagePosition > 0) {
                memoState = 5;
                repeatTurn = previousRightTrigger == right;
            }
        } else if (right & 0x4000U) {
            if (pagePosition < 136) {
                memoState = 3;
                repeatTurn = previousRightTrigger == right;
            }
        } else if (left & 0x1000U) {
            SetMemoState7(memoPageState.latestPage);
        } else if (left & 0x2000U) {
            std::int32_t category = 0;
            if (memoPageState.categoryMask) {
                category = 4;
                while (category >= 0 && nextPage - 1 < memoCategories[category][1])
                    --category;
                if (category < 0)
                    category = 0;
                else {
                    if (nextPage - 1 == memoCategories[category][1] && --category < 0)
                        category = 4;
                    while ((memoPageState.categoryMask & (1U << category)) == 0)
                        if (--category < 0)
                            category = 4;
                }
            }
            memoPageState.category = category;
            SetMemoState7(memoCategories[category][1]);
        }
        break;
    case 3: {
        SwapMemoPageTargets(3, 1);
        RebindNotebookWritingAreas();
        UpdateNotebook(nextPage, 0, 3);
        pageModelBindings[1] = pageModelBindings[2];
        const auto bank = 2 * (static_cast<std::uint16_t>(nextPage) & 1U);
        pageModelBindings[0] = memoTextureHandles[bank];
        pageModelBindings[2] = memoTextureHandles[bank + 1];
        RebindMemoModelTextures();
        ++nextPage;
        if (pagePosition) {
            turningNode = memoNodes[1];
            turningNode->rotation.z = 0;
            turningNode->scale = {1.0f, 1.0f, 1.0f};
            pageThickness = std::max<std::int16_t>(1, static_cast<std::int16_t>(pageThickness - 1));
            memoNodes[2]->scale.y = pageThickness * 0.0046999999f * 0.69999999f;
            pageOffset = -(pagePosition * 0.0000027000001f);
            turningNode->position.y = pageOffset;
            BeginMemoPageTurn();
            pageOffsetStep = (pagePosition * 0.000032899999f - pageOffset) / turnFrames;
            if (pagePosition == 135)
                memoNodes[2]->scale = {};
            (void)ExecuteAicaCommand(0xc00ab, 0, 0);
        } else {
            turningNode = memoNodes[0];
            turningNode->rotation.z = 0;
            pageOffset = 0.0f;
            turnFrames = 22;
            (void)ExecuteAicaCommand(0xb00ab, 0, 0);
        }
        memoState = 4;
        break;
    }
    case 4:
        if (!StepMemoPageTurn()) {
            turningNode->rotation.z = turnAngle + (turningNode == memoNodes[0] ? 910 : 0);
            if (turningNode != memoNodes[0])
                memoNodes[0]->rotation.z = 33678;
        } else {
            turningNode->rotation.z = 32768;
            memoNodes[0]->rotation.z = 33678;
            memoNodes[3]->scale.y = (143 - pageThickness) * 0.0046999999f;
            if (pagePosition == 1) {
                memoNodes[3]->rotation.z = 32768;
                memoNodes[3]->scale.x = memoNodes[3]->scale.z = 1.0f;
            }
            pagePosition = std::min<std::int16_t>(136, static_cast<std::int16_t>(pagePosition + 1));
            memoState = 2;
            SwapMemoPageTargets(0, 2);
            RebindNotebookWritingAreas();
            pageModelBindings[3] = pageModelBindings[0];
            RebindMemoModelTextures();
        }
        break;
    case 5: {
        pagePosition = std::max<std::int16_t>(0, static_cast<std::int16_t>(pagePosition - 1));
        if (!pagePosition)
            memoNodes[3]->scale = {};
        --nextPage;
        SwapMemoPageTargets(2, 0);
        RebindNotebookWritingAreas();
        UpdateNotebook(std::max<std::int16_t>(0, static_cast<std::int16_t>(nextPage - 1)), 2, 1);
        pageModelBindings[0] = pageModelBindings[3];
        const auto bank = 2 - 2 * (static_cast<std::uint16_t>(nextPage) & 1U);
        pageModelBindings[3] = memoTextureHandles[bank];
        pageModelBindings[1] = memoTextureHandles[bank + 1];
        RebindMemoModelTextures();
        if (pagePosition) {
            turningNode = memoNodes[1];
            turningNode->rotation.z = 32768;
            turningNode->scale = {1.0f, 1.0f, 1.0f};
            memoNodes[3]->scale.y = (142 - pageThickness) * 0.0046999999f * 0.69999999f;
            pageOffset = 0.0026999998f;
            turningNode->position.y = pageOffset;
            BeginMemoPageTurn();
            pageOffsetStep = (-(136 - pageThickness) * 0.0000027000001f - pageOffset) / turnFrames;
            if (pagePosition == 1)
                memoNodes[3]->scale = {};
            (void)ExecuteAicaCommand(0xc00ab, 0, 0);
        } else {
            turningNode = memoNodes[0];
            turningNode->rotation.z = 32768;
            pageOffsetStep = 0.0f;
            turnFrames = 22;
            (void)ExecuteAicaCommand(0xb00ab, 0, 0);
        }
        memoState = 6;
        break;
    }
    case 6:
        if (!StepMemoPageTurn()) {
            turningNode->rotation.z = (turningNode == memoNodes[0] ? 33678 : 32768) - turnAngle;
            if (turningNode != memoNodes[0])
                memoNodes[0]->rotation.z = 33678;
        } else {
            turningNode->rotation.z = 0;
            if (turningNode != memoNodes[0])
                memoNodes[0]->rotation.z = 33678;
            pageThickness = std::min<std::int16_t>(136, static_cast<std::int16_t>(pageThickness + 1));
            memoNodes[2]->scale.y = pageThickness * 0.0046999999f;
            if (pagePosition == 1) {
                memoNodes[3]->rotation.z = 0;
                memoNodes[2]->scale.y = 1.0f;
            } else if (pagePosition == 135) {
                memoNodes[2]->rotation.z = 0;
                memoNodes[2]->scale.x = memoNodes[2]->scale.z = 1.0f;
            }
            memoState = 2;
            SwapMemoPageTargets(1, 3);
            RebindNotebookWritingAreas();
            pageModelBindings[2] = pageModelBindings[1];
            RebindMemoModelTextures();
        }
        break;
    case 7:
        drawNotebook = true;
        if (left & 0x200U) {
            memoState = 8;
        } else {
            SetHudItemPromptIcon(0);
            turnAngle += 1456;
            notebookTranslation += 0.0018f;
            if (turnAngle >= 32768) {
                turnAngle = 0;
                memoNodes[0]->rotation.z = 33678;
                if (pagePosition > 0)
                    memoNodes[3]->rotation.z = 32768;
                pagePosition = std::min<std::int16_t>(136, static_cast<std::int16_t>(pagePosition + 1));
                memoState = 2;
            } else {
                memoNodes[0]->rotation.z = turnAngle + 910;
                memoNodes[3]->rotation.z = turnAngle;
            }
        }
        memoNodes[2]->position.y = -0.0022460001f;
        break;
    case 8:
        drawNotebook = false;
        RestoreMemoCamera();
        memoState = 9;
        break;
    case 9:
        DestroyElementByIndex(UIElement::NotebookPages);
        if (ownsSharedItems) {
            (void)DestroySharedItemsWidget();
            DestroyElementByIndex(UIElement::SharedItems);
            ownsSharedItems = false;
        }
        drawNotebook = false;
        MEMO_RefreshControllerInput();
        SelectPostFxBank(1, previousPostFxBank);
        break;
    default:
        memoState = 8;
        break;
    }
    if (drawNotebook) {
        set_icon_select_flags_to(512);
        HLib::ExecuteCallbackNow(DrawMemoNotebook, nullptr);
    }
    previousRightTrigger = right;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void DrawMemoPageText(std::uint32_t bank, std::uint32_t index) {
    const std::int32_t textIndex = bank == 1 ? static_cast<std::int32_t>(index) : memoPages.borrowedText[index];
    if (textIndex < 0)
        return;
    const char *const text = memoPages.text[textIndex].data();
    const float savedScale = g_uiFontScale;
    float scale = MEMO_TEXT_FONT_SCALE[bank];
    const float spacing = MEMO_TEXT_LINE_SPACING[bank] / 10000.0f;
    const std::uint32_t language = static_cast<std::uint32_t>(GetLoadingScreenTextType(ResetSceneTextLanguage()));
    if (language - 4U <= 2U)
        scale *= 1.0f;
    std::int32_t category = -1;
    for (std::size_t entry = 0; entry < memoCategories.size(); ++entry) {
        if (memoPages.activePage == memoCategories[entry][1] && (index & 0xfdU) == 0) {
            category = static_cast<std::int32_t>(entry);
            break;
        }
    }
    nk_context &context = *GetNativeUiContext();
    context.style.window.fixed_background = nk_style_item_color(nk_rgba(0, 0, 0, 0));
    UI_SetFontFamily(3);
    UI_SetFontSize(2);
    UI_SetFontScale(scale);
    nk_begin_titled(&context, &memoPanel, "memo", "memo", nk_rect(0, 0, MEMO_PAGE_WIDTH, MEMO_PAGE_HEIGHT), NK_WINDOW_NO_SCROLLBAR);
    nk_layout_space_begin(&context, NK_STATIC, 0, 10);
    bool nonempty = false;
    std::int32_t line = 0;
    const char *cursor = text;
    if (*text) {
        do {
            const char *const start = cursor;
            std::uint32_t codepoint;
            do {
                cursor += DecodeUITextCharacter(cursor, &codepoint, static_cast<std::int32_t>(text - cursor) + 800);
                if (!nonempty && codepoint != 0 && codepoint != 10 && codepoint != 32)
                    nonempty = true;
            } while (*cursor && codepoint != 10);
            nk_layout_space_push(&context,
                nk_rect(MEMO_PAGE_WIDTH * 0.1f, static_cast<float>(line) * (MEMO_PAGE_HEIGHT * spacing) + MEMO_TEXT_FIRST_Y[bank],
                    MEMO_PAGE_WIDTH * 0.79999995f, MEMO_PAGE_HEIGHT * spacing));
            const nk_color savedColour = context.style.text.color;
            const std::uint32_t colour = category < 0 ? MEMO_TEXT_COLOUR : MEMO_TITLE_COLOUR;
            std::memcpy(&context.style.text.color, &colour, sizeof(colour));
            nk_text(&context, start, static_cast<std::int32_t>(cursor - start) - 1, category < 0 ? 17 : 18);
            ++line;
            context.style.text.color = savedColour;
        } while (*cursor);
    }
    if (category >= 0 && nonempty) {
        const std::int32_t pieces = MEMO_TITLE_PIECES[static_cast<std::size_t>(category)] + 2;
        float y = MEMO_TEXT_FIRST_Y[bank] - 55.0f;
        for (std::int32_t piece = 0; piece < pieces; ++piece) {
            y += 60.0f;
            const std::size_t image = piece == 0 ? 0 : piece == pieces - 1 ? 2 : 1;
            nk_layout_space_push(&context, nk_rect(30.0f, y, MEMO_PAGE_WIDTH - 60.0f, 60.0f));
            UI_DrawPreparedImage(context, &memoTitleImages[image].ready, &memoTitleImages[image].textureHandle, memoTitleImages[image].colour);
        }
    }
    nk_layout_space_end(&context);
    nk_end(&context);
    UI_SetFontScale(savedScale);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::uint16_t GetMemoPageState(std::int32_t selector) noexcept {
    switch (selector) {
    case 0:
        return static_cast<std::uint16_t>(memoPageState.selectedPage);
    case 1:
        return static_cast<std::uint16_t>(memoPageState.latestPage);
    case 2:
        return static_cast<std::uint16_t>(memoPageState.savedFlag);
    default:
        return 0;
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void InitializeMemoPageState(std::int16_t page, std::int16_t flag) noexcept {
    memoPageState.latestPage = page;
    memoPageState.selectedPage = page;
    memoPageState.categoryMask = 0;
    memoPageState.requestFlags = 0;
    memoPageState.savedFlag = flag;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

void ResetMemoPageState() noexcept {
    memoPageState.savedFlag = -1;
    memoPageState.category = 0;
    memoPageState.categoryMask = 0;
    memoPageState.selectedPage = 1;
    memoPageState.latestPage = 1;
    memoPageState.requestFlags = 0;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

bool NotifyMemoFlag(std::int32_t flag, bool selected) {
    if (selected && EV_GetPlayerFlag(PlayerFlagReadMode::Internal, static_cast<std::int16_t>(flag)))
        return false;
    std::int16_t first = -1;
    std::int16_t latest = -1;
    const std::size_t count = std::min<std::size_t>(g_memoRecordCount, memoLineFlags.size());
    for (std::size_t page = 0; page < count; ++page) {
        for (std::int16_t candidate : memoLineFlags[page]) {
            bool matches = candidate > 0 && candidate == flag;
            if (candidate < 0) {
                const auto &conditional = conditionalMemoLines[-candidate - 1];
                matches = conditional.flags[0] == flag || conditional.flags[1] == flag;
            }
            if (!matches)
                continue;
            if (first < 0)
                first = static_cast<std::int16_t>(page);
            if (page < 117 && flag != 1426 && flag != 2184)
                latest = static_cast<std::int16_t>(page);
        }
    }
    if (latest >= 0)
        memoPageState.latestPage = latest;
    if (first < 0)
        return false;
    memoPageState.selectedPage = first;
    if (memoPageState.savedFlag != flag)
        memoPageState.requestFlags |= 1U;
    memoPageState.savedFlag = static_cast<std::int16_t>(flag);
    return true;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

#if defined(SHENMUE_NULL_LIBRARY)
const MT7Model *GetNullMemoPresentation(const MemoPageRenderer **pages, float *translation) {
    const HLib::TaskCallback callback = memoTask ? memoTask->activeCallback : nullptr;
    const bool active = callback == MEMO_Callback || (callback == HLib::UpdateTimedTask && memoTask->delayedCallback == MEMO_Callback);
    if (pages)
        *pages = active && drawNotebook ? &memoPages : nullptr;
    if (translation)
        *translation = notebookTranslation;
    return active && drawNotebook && memoModelSource ? &memoModel : nullptr;
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
static const char *const memoTextDictionary[] = {
    "\245\323\245\320\245\352\241\274\245\322\245\353\245\272",
    "\245\246\245\251\245\363",
    "\275\250\261\315\244\265\244\363",
    "\310\267\304\365\270\370\261\340",
    "\312\270\311\360\311\300",
    "\245\333\245\246\245\252\245\246\245\255\245\347\245\246",
    "\245\267\245\247\245\363\245\325\245\241",
    "\245\261\245\244\245\253",
    "\271\310\306\356\263\271",
    "\274\353\270\265\303\243",
    "\244\306\244\337\244\353",
    "\245\267\245\345\245\246\245\250\245\244",
    "\244\351\244\267\244\244",
    "\244\253\244\342\244\267\244\354\244\312\244\244",
    "\245\317\245\303\245\253\245\275\245\363",
    "\245\351\245\263\245\246\245\273\245\244",
    "\245\337\245\343\245\252\245\263\245\246\245\267\241\311",
    "\245\267\245\345\245\261\241\311\245\363\245\277\245\304",
    "\303\316\244\303\244\306\244\244\244\353",
    "\315\345\271\246\300\254",
    "\244\300\244\303\244\277",
    "\245\310\245\246\245\352\245\267\245\347\245\246",
    "\245\333\245\246\245\351\245\244\245\352\245\347\245\267\245\343",
    "\245\263\245\246\245\312\245\363\245\253\241\311\245\244",
    "\245\317\245\257\245\267\245\363\245\253\241\311\245\244",
    "\245\252\245\363\245\277\241\311\245\355\245\246",
    "\245\337\245\343\245\252\245\275\245\363",
    "\245\310\245\246\245\267\245\347\245\246\245\261\245\363",
    "\245\355\245\246\245\357\245\363\245\325\245\310\245\246",
    "\245\257\241\274\245\355\245\363\245\267\241\311\245\347\245\246",
    "\245\273\245\244\245\246\245\363\245\355\245\246",
    "\245\357\245\363\245\301\245\343\245\244",
    "\311\360\275\321\262\310",
    "\244\313\244\244\244\353",
    "\244\306\244\244\244\353",
    "\244\244\244\353\244\351\244\267\244\244",
    "\267\313\271\341\244\265\244\363",
    "\244\313\312\271\244\257",
    "\311\360\306\301",
    "\244\350\244\246\244\300",
    "\245\301\245\347\245\246\245\275\245\363\245\341\245\244",
    "\245\351\245\363\245\306\245\244",
    "\245\273\245\244\245\344",
    "\245\301\245\347\245\246\245\341\245\244\245\355\245\246",
    "\244\255\244\345\244\246\244\253\244\363\244\301\244\347\244\246",
    "\244\263\244\267\241\311\244\244\244\363",
    "\245\257\241\274\245\353\243\312",
    "\244\265\244\354\244\277",
    "\245\263\245\246\245\306\245\363",
    "\245\322\245\343\245\257\245\267\245\355\245\246",
    "\350\265\262\326",
    "\305\355\315\373\276\257",
    "\244\342\244\351\244\303\244\277",
    "\245\271\245\310\245\352\241\274\245\310\245\325\245\241\245\244\245\310",
    "\243\260\243\264\243\266\243\270\241\335\241\241\241\241\241\241\241\241",
    "\245\301\245\343\241\274\245\352\241\274",
    "\305\267\316\266\303\317\315\247",
    "\245\336\245\303\245\311\245\250\245\363\245\270\245\247\245\353\245\271",
    "\241\371\271\254\261\277\244\316\270\260\241\241\241\241\241\241\241\241",
    "\245\242\245\270\245\242\316\271\271\324\274\322",
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
bool OpenMemoResources() {
    bool success = false;
    const std::uint32_t npcLoad = GetNpcNewLoadState();
    if (npcLoad)
        EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    if (SetupMemoTextures() && LoadMemoModel() && SetupMemoCamera())
        success = true;
    if (!success)
        ReleaseMemoResources();
    if (npcLoad)
        EV_NPC_NewLoadSw(1);
    return success;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void RefreshMemoFlagPage() {
    const std::int16_t flag = g_memoFlag;
    if (flag)
        NotifyMemoFlag(flag, false);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void CloseActiveMemo() {
    if (IsItemSelectable()) {
        RestoreMemoCamera();
        MEMO_RefreshControllerInput();
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void RequestMemoClose() {
    if (IsItemSelectable())
        memoState.phase = 8;
}

#else

void RequestMemoClose() noexcept {
    if (memoState)
        memoState = 8;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void SetupNotebook() {
    const d3t::Texture2DDescription description{512, 512, 1, 1, 2, 4, 0, 1, 0, 0, nullptr};
    for (std::size_t index = 0; index < 4; ++index) {
        if (!memoPages.targets[1][index])
            memoPages.targets[1][index] = d3t::CreateTexture2D(&description);
        memoPages.dirty[1][index] = true;
        if (!memoPages.targets[0][index])
            memoPages.targets[0][index] = d3t::CreateTexture2D(&description);
        memoPages.dirty[0][index] = true;
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

std::int64_t CreateMemoNotebookPages() {
    SetupNotebook();
    EnsureMemoTitleTextures();
    return 1;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// Releases the eight targets, not the process-lifetime three border textures installed by the UI callback's init operation.
// @verified - p
void ReleaseNotebookPageTargets() {
    for (std::size_t index = 0; index < 4; ++index) {
        for (std::size_t bank : {std::size_t{1}, std::size_t{0}}) {
            d3t::InitTexture2DResource(memoPages.targets[bank][index]);
            memoPages.targets[bank][index] = 0;
        }
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::int64_t DestroyMemoNotebookPages() {
    ReleaseNotebookPageTargets();
    return 1;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::int64_t GetMemoNotebookPagesInitState() {
    return static_cast<std::uint32_t>(memoTitlePhase);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void RenderMemoNotebookPages() {
    if (memoTitlePhase != 3)
        memoTitlePhase = EnsureMemoTitleTextures();
    memoDebugMode = g_notebookDebugMode;
    if (memoPendingOpen) {
        OpenMemoAtPage(memoPendingOpenPage);
        memoPendingOpen = 0;
    }
    // RenderMemoNotebookPages visits the borrowed bank first, then the four live text targets.
    if (memoForcePageRedraw) {
        memoPages.dirty[0].fill(true);
        memoPages.dirty[1].fill(true);
    }
    bool dirty = false;
    for (std ::size_t index = 0; index < 4; ++index)
        dirty |= memoPages.dirty[1][index];
    for (std::size_t index = 0; index < 4; ++index)
        dirty |= memoPages.dirty[0][index];
    if (!dirty)
        return;
    d3t::D3TDeviceContext *context = g_nuklearRenderDeviceContext;
    if (!context)
        context = SetupNuklearBuffer();
    if (!context)
        return;
    nk_clear(GetNativeUiContext());
    const std::array<float, 6> savedViewport = d3t::GetRenderState(*context).viewport;
    const auto savedTargets = d3t::GetRenderState(*context).outputTargets;
    const Matrix4x4 orthographic{
        .rows = {{.x = 2.0f / MEMO_PAGE_WIDTH, .w = -1.0f}, {.y = 2.0f / MEMO_PAGE_HEIGHT, .w = -1.0f}, {.z = -1.0f, .w = -0.0f}, {.w = 1.0f}}};
    SetNuklearViewMatrix(orthographic);
    NuklearCompositeNullsub(context);
    const UIViewport viewport{.rect = {.x = 0.0f, .y = 0.0f, .z = MEMO_PAGE_WIDTH, .w = MEMO_PAGE_HEIGHT}, .depthRange = {.x = 0.0f, .y = 1.0f}};
    std::array<float, 6> pageViewport;
    std::memcpy(pageViewport.data(), &viewport, sizeof(viewport));
    d3t::SetRenderViewport(context, pageViewport);
    for (std::uint32_t bank = 0; bank < 2; ++bank) {
        for (std::uint32_t index = 0; index < 4; ++index) {
            if (!memoPages.dirty[bank][index])
                continue;
            SetNuklearViewMatrix(orthographic);
            std::array<std::byte, 0x84> targets{};
            std::memcpy(targets.data(), &memoPages.targets[bank][index], sizeof(std::uint32_t));
            d3t::SetRenderTargets(context, targets);
#if defined(_WIN32) || defined(SHENMUE_NULL_LIBRARY)
            constexpr std::array<float, 4> transparent{};
            d3t::UnmapAndClearViews(context, memoPages.targets[bank][index], 0, transparent.data());
#endif
            GetAspectRatioModeShouldScale();
            InvokeNuklearFrame(context);
            DrawMemoPageText(bank, index);
            Nuklear_RenderPass(&viewport);
            memoPages.dirty[bank][index] = false;
        }
    }
    NuklearCompositeNullsub(context);
    d3t::SetRenderTargets(context, savedTargets);
    context = g_nuklearRenderDeviceContext;
    if (context) {
        GetAspectRatioModeShouldScale();
        InvokeNuklearFrame(context);
        if (savedViewport[2] != 0.0f)
            d3t::SetRenderViewport(context, savedViewport);
    }
    nk_clear(GetNativeUiContext());
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t CancelSelectorMemo(std::int32_t force) {
    std::int32_t result;
    if (!force)
        result = 0;
    else {
        RestoreMemoCamera();
        MEMO_RefreshControllerInput();
        result = 1;
    }
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void SetMemoOpeningPage(std::int16_t page) {
    memo.pageOffset = 0.0f;
    memo.draw.position.x = memoBaseX;
    memo.draw.position.z = memoBaseZ;
    memo.thickness = static_cast<std::int16_t>(212 - page);
    memoState.phase = 7;
    memo.position = page;
    memoNextPage = page;
    PrepareMemoNextPage();
    ++memoNextPage;
    memo.pageStep = ((212 - memo.thickness) * 0.0000027000001f - memo.pageOffset) * 0.045454498f;
    memo.cover->rotation.z = 0;
    memo.rightPages->scale.x = memo.rightPages->scale.z = 1.0f;
    memo.rightPages->scale.y = memo.thickness * 0.0046999999f;
    memo.leftPages->rotation.z = 0;
    memo.leftPages->scale.x = memo.leftPages->scale.z = memo.position < 1 ? 0.0f : 1.0f;
    memo.leftPages->scale.y = memo.position < 1 ? 0.0f : (226 - memo.thickness) * 0.0046999999f;
    FinishMemoTexturePage(0);
    shenExecuteAICACommand(0xb00ab, 0, 0);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void BeginMemoPageTurn() {
    MemoWork *work = &memo;
    std::uint16_t motions[2][2] = {{0xf0ee, 0xf0ef}, {0xf0ec, 0xf0ed}};
    work->turnAngle = 0;
    work->turnCounter = 0;
    work->motionFrame = 0.0f;
    work->motionStep = 1.0f;
    std::uint16_t *selected = motions[1];
    if (memoState.repeat == 1) {
        work->turnDelay = static_cast<std::int32_t>(work->turnDelay * 0.5f);
        if (work->turnDelay < 4) {
            work->turnFrames = 5;
            work->turnDelay = 1;
            selected = motions[0];
        } else {
            work->turnFrames = 11;
            work->turnDelay = 4;
            work->motionStep = 2.0f;
        }
    } else {
        work->turnFrames = 22;
        work->turnDelay = 8;
    }
    if (memoState.phase == 3)
        work->motion = selected[0];
    else
        work->motion = selected[1];
}
#else
// @verified - d
void BeginMemoPageTurn() {
    memo.motionFrame = 0.0f;
    memo.motionStep = 1.0f;
    memo.turnAngle = memo.turnCounter = 0;
    std::uint32_t motions = 0xf0edf0ecU;
    if (memoState.repeat == 1) {
        if (memo.turnDelay / 2 < 4) {
            memo.turnFrames = 5;
            memo.turnDelay = 1;
            motions = 0xf0eff0eeU;
        } else {
            memo.turnFrames = 11;
            memo.turnDelay = 4;
            memo.motionStep = 2.0f;
        }
    } else {
        memo.turnFrames = 22;
        memo.turnDelay = 8;
    }
    memo.motion = static_cast<std::int16_t>(memoState.phase == 3 ? motions : motions >> 16);
}
#endif

} // namespace

#else

namespace {

void BeginMemoPageTurn() {
    motionStep = 1.0f;
    motionFrame = 0.0f;
    turnCounter = 0;
    turnAngle = 0;
    std::uint32_t motions = 0xf0edf0ecU;
    if (repeatTurn == 1) {
        if (turnDelay / 2 >= 4) {
            motionStep = 2.0f;
            turnFrames = 11;
            turnDelay = 4;
        } else {
            turnFrames = 5;
            turnDelay = 1;
            motions = 0xf0eff0eeU;
        }
    } else {
        turnFrames = 22;
        turnDelay = 8;
    }
    pageMotion = static_cast<std::uint16_t>(memoState == 3 ? motions : motions >> 16U);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
bool StepMemoPageTurn() {
    const std::int32_t phase = memoState.phase;
    bool result = false;
    if (memo.position < 1) {
        if (phase == 4) {
            memo.draw.position.x += CalculateFixedCosine(memo.draw.rotation.y) * 0.0018f;
            memo.draw.position.z += CalculateFixedSine(memo.draw.rotation.y) * -0.0018f;
        } else if (phase == 6) {
            memo.draw.position.x -= CalculateFixedCosine(memo.draw.rotation.y) * 0.0018f;
            memo.draw.position.z -= CalculateFixedSine(memo.draw.rotation.y) * -0.0018f;
        }
    } else {
        ApplyNodeMotionFrame(
            reinterpret_cast<const TimedNodeMotionHeader *>(MOTN_GetDataForAnimId(static_cast<std::uint16_t>(memo.motion))), memo.draw.root, memo.motionFrame);
        memo.motionFrame += memo.motionStep;
        if (!(memo.motionFrame < 26.0f) && memo.motionStep == 2.0f)
            memo.motionFrame = 29.0f;
        else if (!(memo.motionFrame < 29.0f))
            memo.motionFrame = 29.0f;
        ++memo.turnCounter;
        MT7Node *left = memo.leftPages;
        left->rotation.z = 0x8000;
        left->scale.y = (218 - memo.thickness) * 0.0046999999f;
        if (memo.position == 1) {
            left->scale.x = 0.0f;
            left->scale.y = 0.0f;
            left->scale.z = 0.0f;
        }
        MT7Node *right = memo.rightPages;
        right->scale.y = (memo.thickness + 2) * 0.0046999999f;
        if (memo.position == 201) {
            right->scale.x = 0.0f;
            right->scale.y = 0.0f;
            right->scale.z = 0.0f;
        }
    }
    if (!memo.position || memo.turnCounter >= memo.turnDelay) {
        memo.pageOffset += memo.pageStep;
        memo.turnAngle += 32768 / memo.turnFrames;
    }
    memoMovingNode->position.y = memo.pageOffset;
    if (memo.turnAngle > 32768) {
        memo.turnAngle = 0;
        memo.turnCounter = 0;
        memo.turningPage->scale.x = 0.0f;
        memo.turningPage->scale.z = 0.0f;
        memo.turningPage->scale.y = 0.0f;
        result = true;
    }
    return result;
}

} // namespace

#else

namespace {

bool StepMemoPageTurn() {
    if (pagePosition >= 1) {
        const auto motion = FindLoadedMotionData(pageMotion);
        if (!motion.empty())
            ApplyNodeMotionFrame(memoModel, motion, motionFrame);
        memoNodes[2]->position.y = -0.0022460001f;
        motionFrame += motionStep;
        if ((motionFrame >= 26.0f && motionStep == 2.0f) || motionFrame >= 29.0f)
            motionFrame = 29.0f;
        ++turnCounter;
        memoNodes[3]->rotation.z = 32768;
        memoNodes[3]->scale.y = (142 - pageThickness) * 0.0046999999f * 0.69999999f;
        if (pagePosition == 1)
            memoNodes[3]->scale = {};
        memoNodes[2]->scale.y = pageThickness * 0.0046999999f * 0.69999999f;
        if (pagePosition == 135)
            memoNodes[2]->scale = {};
    } else if (memoState == 4) {
        notebookTranslation += 0.0018f;
    } else if (memoState == 6) {
        notebookTranslation -= 0.0018f;
    }
    if (!pagePosition || turnCounter >= turnDelay) {
        pageOffset += pageOffsetStep;
        turnAngle += 32768 / turnFrames;
    }
    turningNode->position.y = pageOffset;
    if (turnAngle <= 32768)
        return false;
    turnCounter = 0;
    turnAngle = 0;
    memoNodes[1]->scale = {};
    return true;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void DrawMemoNotebook(void *) {
    if (!drawNotebook || memoModel.nodes.empty() || !g_cameraControl)
        return;
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(&g_cameraControl->viewMatrix);
    Matrix4x4::preTranslate(notebookTranslation, 0.0049999999f, -0.2f);
    Matrix4x4::RotateAroundX_Wrapper(12743);
    Matrix4x4::Scale(1.3f, 1.3f, 1.3f);
    Matrix4x4 transform{};
    Matrix4x4::GetTransformMatrix(&transform);
    Matrix4x4::Pop(1);
    TemporaryGlobalLightingState lighting{};
    BeginTemporaryGlobalLighting({0.4f, 0.4f, 0.4f, 0.4f}, lighting);
    Matrix4x4_Push();
    Matrix4x4::MultiplyWithPrePassMatrix(&transform);
    const Vector3 origin{transform.rows[3].x, transform.rows[3].y, transform.rows[3].z};
    TemporaryAttachedLightState attachedLight{};
    BeginTemporaryAttachedLight(0, origin, 0, {2.0f, 2.0f, 2.0f, 2.0f}, attachedLight);
    SetLegacyModelScale(1.3f);
    DrawMT7Model(memoModel, nullptr, GetLegacyCharacterShaderPath(), 1.0f);
    EndTemporaryAttachedLight(attachedLight);
    Matrix4x4::Pop(1);
    EndTemporaryGlobalLighting(lighting);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void PlayMemoAudio(std::int32_t volume) {
    memoAudio = shenPlayAudio("MEMO", shen_name{}, 17, false);
    SetConversationVolume(memoAudio, volume);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void PrepareMemoNextPage() {
    UpdateMemoTexturePage(memoNextPage);
    memoTextureNames[1][4] = memoTextureNames[1][5];
    const std::int32_t bank = (memoNextPage & 1) * 2;
    memoTextureNames[1][3] = memoTextureNames[0][bank];
    memoTextureNames[1][5] = memoTextureNames[0][bank + 1];
    RebindMT7ModelTextures(memo.model, memoTextureNames[1], MEMO_TEXTURE_COUNT, memoOriginalTextures);
}
#else
// @verified - d
void PrepareMemoNextPage() {
    UpdateMemoTexturePage(memoNextPage);
    memoTextureNames[1][4] = memoTextureNames[1][5];
    const std::int32_t bank = (memoNextPage & 1) * 2;
    memoTextureNames[1][3] = memoTextureNames[0][bank];
    memoTextureNames[1][5] = memoTextureNames[0][bank + 1];
    RebindMT7ModelTextures(memo.model, memoTextureNames[1], MEMO_TEXTURE_COUNT);
}
#endif

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void PrepareMemoPreviousPage() {
    UpdateMemoTexturePage(memoNextPage - 1 < 0 ? 0 : static_cast<std::int16_t>(memoNextPage - 1));
    memoTextureNames[1][3] = memoTextureNames[1][6];
    const std::int32_t bank = 2 - (memoNextPage & 1) * 2;
    memoTextureNames[1][6] = memoTextureNames[0][bank];
    memoTextureNames[1][4] = memoTextureNames[0][bank + 1];
    RebindMT7ModelTextures(memo.model, memoTextureNames[1], MEMO_TEXTURE_COUNT, memoOriginalTextures);
}
#else
// @verified - d
void PrepareMemoPreviousPage() {
    UpdateMemoTexturePage(memoNextPage - 1 < 0 ? 0 : static_cast<std::int16_t>(memoNextPage - 1));
    memoTextureNames[1][3] = memoTextureNames[1][6];
    const std::int32_t bank = 2 - (memoNextPage & 1) * 2;
    memoTextureNames[1][6] = memoTextureNames[0][bank];
    memoTextureNames[1][4] = memoTextureNames[0][bank + 1];
    RebindMT7ModelTextures(memo.model, memoTextureNames[1], MEMO_TEXTURE_COUNT);
}
#endif

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void FinishMemoTexturePage(std::int32_t previous) {
    MemoTextureName *destination, *source;
    MemoTextureName *const names = memoTextureNames[0];
    if (previous == 1) {
        destination = names + 21;
        source = names + 20;
    } else {
        destination = names + 22;
        source = names + 19;
    }
    *destination = *source;
    RebindMT7ModelTextures(memo.model, memoTextureNames[1], MEMO_TEXTURE_COUNT, memoOriginalTextures);
}
#else
// @verified - d
void FinishMemoTexturePage(std::int32_t previous) {
    if (previous == 1)
        memoTextureNames[1][5] = memoTextureNames[1][4];
    else
        memoTextureNames[1][6] = memoTextureNames[1][3];
    RebindMT7ModelTextures(memo.model, memoTextureNames[1], MEMO_TEXTURE_COUNT);
}
#endif

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::int16_t GetMemoOpeningPage() {
    RefreshMemoFlagPage();
    if (static_cast<std::int16_t>(g_memoPage) < 1)
        g_memoPage = 1;
    return static_cast<std::uint8_t>(g_memoPage);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void RenderMemoPageText(std::int32_t page, MemoTexture **textures) {
    char output[32], text[32], ruby[64];
    std::uint16_t colours[9];
    const std::int16_t *flags = 0;
    MemoTexture *pageTextures[2] = {textures[0], textures[1]};
    std::int32_t selection[2];
    if (page < static_cast<std::int32_t>(g_memoRecordCount))
        flags = memoLineFlags[page];
    if (!flags)
        return;
    std::int32_t textIndex = page * 10;
    std::int32_t money = 0;
    if (page == 4)
        money = GetInternalFlag(138) != 0;
    for (std::int32_t line = 0; line < 10; ++line, ++textIndex) {
        const char *source = 0, *sourceRuby = 0;
        if (flags[line] < 0) {
            strcpy(text, "\241\241");
            strcpy(ruby, "\241\241");
            SelectMemoFlagText(text, ruby, flags[line]);
        } else {
            if (!flags[line] || GetInternalFlag(flags[line])) {
                source = GetMemoText(g_memoData + 8, textIndex);
                sourceRuby = GetMemoText(g_memoRubyTable, textIndex);
            }
            if (money && (line == 1 || line == 3)) {
                reinterpret_cast<std::uint32_t *>(text)[0] = 0;
                reinterpret_cast<std::uint32_t *>(text)[1] = 0;
                reinterpret_cast<std::uint32_t *>(text)[2] = 0;
                reinterpret_cast<std::uint32_t *>(text)[3] = 0;
                reinterpret_cast<std::uint32_t *>(text)[4] = 0;
                reinterpret_cast<std::uint32_t *>(text)[5] = 0;
                reinterpret_cast<std::uint32_t *>(text)[6] = 0;
                reinterpret_cast<std::uint32_t *>(text)[7] = 0;
                FormatMemoTransferredMoney(text, line != 1);
            } else {
                reinterpret_cast<std::uint32_t *>(text)[0] = 0;
                reinterpret_cast<std::uint32_t *>(text)[1] = 0;
                reinterpret_cast<std::uint32_t *>(text)[2] = 0;
                reinterpret_cast<std::uint32_t *>(text)[3] = 0;
                reinterpret_cast<std::uint32_t *>(text)[4] = 0;
                reinterpret_cast<std::uint32_t *>(text)[5] = 0;
                reinterpret_cast<std::uint32_t *>(text)[6] = 0;
                reinterpret_cast<std::uint32_t *>(text)[7] = 0;
                if (source)
                    ExpandMemoText(text, source);
                else
                    strcpy(text, "\241\241");
                memset(ruby, 0, sizeof(ruby));
                if (sourceRuby)
                    ExpandMemoText(ruby, sourceRuby);
                else
                    strcpy(ruby, "\241\241");
            }
        }
        PrepareMemoTextLine(output, text, colours, selection);
        static const std::uint16_t offsets[5] = {0xce10, 0x9e10, 0x6e10, 0x3e10, 0x0e10};
        std::byte *pixels = reinterpret_cast<std::byte *>(pageTextures[line / 5]->pixels);
        RasterizeMemoLine(reinterpret_cast<std::uint16_t *>(pixels + offsets[line % 5]), output, ruby, 256, colours);
    }
}
#else
// @verified - d
void RenderMemoPageText(std::int16_t page, MemoTexture **textures) {
    if (page >= static_cast<std::int32_t>(g_memoRecordCount))
        return;
    const std::int16_t *const flags = memoLineFlags[page];
    const bool money = page == 4 && GetInternalFlag(138);
    char text[32] = {}, ruby[64] = {};
    for (std::int32_t line = 0; line < 10; ++line) {
        const char *source = 0, *sourceRuby = 0;
        if (flags[line] < 0) {
            strcpy(text, "\241\241");
            strcpy(ruby, "\241\241");
            const ConditionalMemoLine &conditional = conditionalMemoLines[-flags[line] - 1];
            for (std::int32_t choice = 0; choice < 2; ++choice) {
                if (GetInternalFlag(conditional.flags[choice])) {
                    if (conditional.text[choice])
                        strcpy(text, conditional.text[choice]);
                    if (conditional.ruby[choice])
                        strcpy(ruby, conditional.ruby[choice]);
                    break;
                }
            }
        } else {
            if (!flags[line] || GetInternalFlag(flags[line])) {
                source = GetMemoText(g_memoData + 8, page * 10 + line);
                sourceRuby = GetMemoText(g_memoRubyTable, page * 10 + line);
            }
            if (money && (line == 1 || line == 3)) {
                memset(text, 0, sizeof(text));
                FormatMemoTransferredMoney(text, line != 1);
            } else {
                memset(text, 0, sizeof(text));
                memset(ruby, 0, sizeof(ruby));
                if (source)
                    ExpandMemoText(text, source);
                else
                    strcpy(text, "\241\241");
                if (sourceRuby)
                    ExpandMemoText(ruby, sourceRuby);
                else
                    strcpy(ruby, "\241\241");
            }
        }
        char output[32];
        std::uint16_t colours[9];
        std::int32_t selection[2];
        PrepareMemoTextLine(output, text, colours, selection);
        static const std::uint16_t offsets[5] = {0xce10, 0x9e10, 0x6e10, 0x3e10, 0x0e10};
        std::byte *const pixels = reinterpret_cast<std::byte *>(textures[line / 5]->pixels);
        RasterizeMemoLine(reinterpret_cast<std::uint16_t *>(pixels + offsets[line % 5]), output, ruby, 256, colours);
    }
}
#endif

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

Vector4 modelDrawLightColour = {};

std::int32_t modelDrawSavedLight = 0;

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void SetMemoModelLightDirection(Vector3 position, const Vector3 &camera, std::int32_t yaw, std::uint32_t light) {
    Vector3 target;
    camCtrlGetTargetPosition(&target);
    const float dx = target.x - camera.x, dy = target.y - camera.y, dz = target.z - camera.z;
    const float distance = sqrtf(dx * dx + dy * dy + dz * dz);
    const std::int32_t rotation = yaw - camCtrlGetNextRotY();
    Vector3 origin;
    origin.x = target.x - dc::sdk::SinFixed(rotation) * distance;
    origin.y = target.y;
    origin.z = target.z + dc::sdk::CosFixed(rotation) * distance;
    if (fabsf(position.x) < 0.00001f)
        position.x = 0.0f;
    if (fabsf(position.y) < 0.00001f)
        position.y = 0.0f;
    if (fabsf(position.z) < 0.00001f)
        position.z = 0.0f;
    Vector3 direction = {position.x - origin.x, position.y - origin.y, position.z - origin.z};
    NormalizeVector3(&direction);
    SetSelectedLightDirection(light, direction);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void PrepareMemoModelLighting(const MemoModelDrawContext &context, std::uint32_t light) {
    modelDrawSavedLight = GetCurrentActiveLight();
    Vector3 camera;
    camCtrlGetNextCamPos(&camera);
    ResetSelectedLightEntry(light);
    SetLightKind(light, 2);
    EnableSelectedLight(light);
    SetMemoModelLightDirection(context.position, camera, context.style, light);
    shenSetLightColour(light, modelDrawLightColour);
    PublishGlobalLighting();
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void DrawMemoModel(const MemoModelDrawContext &context) {
    matrix_stack::Matrix4x4_Push(0);
    dc::sdk::PushGraphicsWorkspace();
    Matrix4x4::preTranslate(context.position.x, context.position.y, context.position.z);
    Matrix4x4::RotateAroundZ_Wrapper(context.rotation.z);
    Matrix4x4::RotateAroundY_Wrapper(context.rotation.y);
    Matrix4x4::RotateAroundX_Wrapper(context.rotation.x);
    Matrix4x4::Scale(context.scale, context.scale, context.scale);
    dc::sdk::SetModelScale(context.scale);
    DrawSingleCharacterModel(context.root);
    dc::sdk::SetModelScale(1.0f);
    PopCurrentMatrix(1);
    dc::sdk::PopGraphicsWorkspace();
    DrawMainIconOverlay(0x76000000U, -0.5f);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void RestoreMemoModelLighting(std::uint32_t light) {
    SetCurrentActiveLight(modelDrawSavedLight);
    if (modelDrawSavedLight == 1 && light == 0) {
        EnableSelectedLight(0);
    } else {
        ResetSelectedLightEntry(light);
        DisableSelectedLight(light);
    }
    PublishGlobalLighting();
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void DrawMemoModelContext(void *parameter) {
    const MemoModelDrawContext &context = *static_cast<const MemoModelDrawContext *>(static_cast<const void *>(parameter));
    const Vector4 savedColour = modelDrawLightColour;
    modelDrawLightColour = context.colour;
    const std::uint32_t kind = GetSelectedLightKind(0);
    Vector3 direction;
    GetSelectedLightDirection(0, &direction);
    Vector4 colour;
    GetSelectedLightColour(0, &colour);
    const float intensity = GetSelectedLightSceneIntensity(0);
    PrepareMemoModelLighting(context, 0);
    DrawMemoModel(context);
    SetLightKind(0, kind);
    SetSelectedLightDirection(0, direction);
    shenSetLightColour(0, colour);
    SetSelectedLightSceneIntensity(0, intensity);
    RestoreMemoModelLighting(0);
    modelDrawLightColour = savedColour;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void (*memoPageRenderers[2])(std::int32_t, MemoTexture **) = {0, RenderMemoPageText};
#else
void (*memoPageRenderers[2])(std::int16_t, MemoTexture **) = {0, RenderMemoPageText};
#endif

// @verified - d
void UpdateMemoTexturePage(std::int16_t page) {
    std::int32_t *handle = &memoTextureHandles[(page & 1) * 2];
    MemoTexture *textures[2] = {memoTextures[0], memoTextures[1]};
    memset(textures[0]->pixels, 0, 0x10000);
    memset(textures[1]->pixels, 0, 0x10000);
    if (memoPageRenderers[memoState.inputMode]) {
        memoPageRenderers[memoState.inputMode](page, textures);
        ReplacePtxTexture(*handle++, &textures[0]->header);
        ReplacePtxTexture(*handle, &textures[1]->header);
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void InitializeMemoTexture(MemoTexture &texture, const char *name, std::int16_t width, std::int16_t height) {
    texture.header.name = shen_name(SHEN_NAME(TEXN));
    const std::int32_t pixels = width * height;
    const std::int32_t bytes = pixels * 2;
    texture.header.size = bytes + sizeof(TextureChunk);
    texture.header.globalIndexHeader[0] = SHEN_NAME(GBIX);
    texture.header.globalIndexHeader[1] = 4;
    texture.header.globalIndexHeader[2] = 0;
    char *imageName = reinterpret_cast<char *>(&texture.header.imageName);
    *imageName++ = 'P';
    *imageName++ = 'V';
    *imageName++ = 'R';
    *imageName = 'T';
    texture.header.width = width;
    texture.header.height = height;
    texture.header.imageSize = bytes + 8;
    texture.header.type = 0x902;
    SetMemoTextureName(reinterpret_cast<std::byte *>(&texture), name);
    memset(texture.pixels, 0, bytes);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t SetupMemoTextures() {
    std::int32_t result = 0;
    if (!MEMO_TEXTURE_DATA) {
        MemoTextureTaskData *data =
            static_cast<MemoTextureTaskData *>(HLib::AllocateTaskData(sizeof(MemoTextureTaskData), shen_name(SHEN_NAME4('M', 'E', 'M', 'T'))));
        MEMO_TEXTURE_DATA = data;
        if (data) {
            HLib::AssignDynamicAlignmentForTaskData(data);
            std::int32_t count = 0;
            std::int32_t index;
            static const char *const names[6] = {"AAA_TEX", "BBB_TEX", 0, "CCC_TEX", "DDD_TEX", 0};
            const char *const *name = names;
            for (index = 0; index < 2; ++index) {
                MemoTexture *texture = reinterpret_cast<MemoTexture *>(data);
                while (*name) {
                    InitializeMemoTexture(*texture, *name++, 256, 128);
                    ++texture;
                }
                ++name;
                memset(texture, 0, 4);
                count += LoadTextureChunks(
                    reinterpret_cast<TextureChunk *>(data), sizeof(MemoTextureTaskData), memoTextureHandles + count, memoTextureNames[0] + count);
            }
            for (index = 0; index < count; ++index)
                if (memoTextureHandles[index] == -1)
                    break;
            if (index >= count) {
                MemoTexture *texture = reinterpret_cast<MemoTexture *>(data);
                for (index = 0; index < 2; ++index) {
                    memoTextures[index] = texture;
                    texture = reinterpret_cast<MemoTexture *>(reinterpret_cast<std::byte *>(texture) + texture->header.size);
                }
                result = 1;
            }
        }
    }
    return result;
}
#else
// @verified - d
bool SetupMemoTextures() {
    if (MEMO_TEXTURE_DATA)
        return false;
    MEMO_TEXTURE_DATA = static_cast<MemoTextureTaskData *>(HLib::AllocateTaskData(sizeof(MemoTextureTaskData), shen_name(SHEN_NAME(MEMT))));
    if (!MEMO_TEXTURE_DATA)
        return false;
    HLib::AssignDynamicAlignmentForTaskData(MEMO_TEXTURE_DATA);
    static const char *const names[4] = {"AAA_TEX", "BBB_TEX", "CCC_TEX", "DDD_TEX"};
    std::int32_t count = 0;
    for (std::int32_t bank = 0; bank < 2; ++bank) {
        for (std::int32_t index = 0; index < 2; ++index)
            InitializeMemoTexture(MEMO_TEXTURE_DATA->textures[index], names[bank * 2 + index], 256, 128);
        MEMO_TEXTURE_DATA->terminator = 0;
        for (std::int32_t index = 0; index < 2; ++index) {
            const TextureChunk &texture = MEMO_TEXTURE_DATA->textures[index].header;
            memoTextureHandles[count] = LoadAndFindTEXN_Wrapper(&texture);
            memoTextureNames[0][count].texture = memoTextureHandles[count];
            memoTextureNames[0][count].nameLow = texture.nameLow;
            memoTextureNames[0][count].nameHigh = texture.nameHigh;
            ++count;
        }
    }
    for (std::int32_t index = 0; index < count; ++index)
        if (memoTextureHandles[index] == -1)
            return false;
    memoTextures[0] = &MEMO_TEXTURE_DATA->textures[0];
    memoTextures[1] = &MEMO_TEXTURE_DATA->textures[1];
    return true;
}
#endif

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

bool LoadMemoModel() {
    bool loaded = false;
    void *const source = shenLoadFile("model/item", "MMA02G1T.MT7", shen_name(SHEN_NAME(MEMO)));
    if (source) {
        MT7Model *const model = ReadMT7ModelTextureNames(source, memoTextureNames[1], memoOriginalTextures);
        if (model) {
            memo.draw.root = model->root;
            memo.model = model;
            MEMO_TEXTURE_COUNT = model->retainedTextureCount;
            memo.cover = memo.draw.root->child;
            memo.turningPage = memo.cover->sibling;
            memo.rightPages = memo.turningPage->sibling;
            memo.leftPages = memo.rightPages->sibling;
            loaded = true;
        } else
            HLib::FreeTaskData(source);
    }
    return loaded;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void BindMemoPad(std::int32_t enabled) {
    if (enabled) {
        if (memoPad.entry && !IsPadControlEntryBound(memoPad.entry))
            memoPad.previous = BindPadControlEntry(memoPad.entry);
    } else if (memoPad.entry && memoPad.previous && IsPadControlEntryBound(memoPad.entry)) {
        BindPadControlEntry(memoPad.previous);
        memoPad.previous = 0;
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
bool SetupMemoCamera() {
    std::int32_t result = 0;
    Character *const player = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo;
    if (!player)
        return result;
    memoCamera.playerDisplay = IsOfThisVisibilityType(player, static_cast<CharaVisibilityMode>(3));
    memoCamera.playerVisibility = IsCharacterCollisionCallbackEnabled(player);
    memoCamera.state = camCtrlGetState();
    memoCamera.perspective = camCtrlGetPerspectiveWrapper();
    camCtrlGetNextCamPos(&memoCamera.position);
    camCtrlGetTargetPosition(&memoCamera.target);
    memoCamera.visibleHelp = static_cast<std::uint16_t>(GetUiFlag(0) & 0xfdffU);
    memoCamera.selectedHelp = static_cast<std::uint8_t>(GetUiFlag(1));
    memo.savedCamera = &memoCamera;
    EV_SetCharDisplay(player, static_cast<CharaVisibilityMode>(3), 0);
    charaSetVisibilityFlagTo(player, 0);
    camCtrlSetState(9);
    camCtrlSetPerspectiveWrapper(0x2666);
    memoTranslation = 0.0f;
    memo.leftPages->scale.x = 0.0f;
    memo.leftPages->scale.z = 0.0f;
    memo.leftPages->scale.y = 0.0f;
    memo.turningPage->scale.x = 0.0f;
    memo.turningPage->scale.z = 0.0f;
    memo.turningPage->scale.y = 0.0f;
    memo.rightPages->scale.x = 1.0f;
    memo.rightPages->scale.z = 1.0f;
    memo.rightPages->scale.y = 1.0f;
    memo.draw.scale = 1.3f;
    std::int32_t inverseYaw = -playerGetRotY();
    memo.draw.rotation.x = 0x31c7;
    memo.draw.rotation.y = -inverseYaw;
    memo.draw.rotation.z = 0;
    memo.draw.style = 0;
    Vector3 camera;
    camCtrlGetNextCamPos(&camera);
    if (memoCamera.state == 24) {
        camCtrlGetNextCamPos(&camera);
        inverseYaw = -camCtrlGetNextRotY();
        memo.draw.rotation.y = -inverseYaw;
    } else {
        GetCharacterWorldPosition(player, &camera);
        camera.y += 1.6f;
    }
    camCtrlSetNextPosition(camera);
    memo.draw.position = camera;
    memo.draw.position.x += CalculateFixedSine(inverseYaw) * 0.2f;
    memo.draw.position.z += CalculateFixedCosine(inverseYaw) * -0.2f;
    camCtrlSetTargetPosition(memo.draw.position);
    memo.draw.position.y += 0.005f;
    memoBaseX = memo.draw.position.x;
    memoBaseZ = memo.draw.position.z;
    if (static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->retainedTaskD)
        wndSetFlag(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->retainedTaskD, 0);
    result = 1;
    return result;
}

} // namespace

#else

namespace {

bool SetupMemoCamera() {
    using enum CameraState;
    MapControlData *const control = MemoMapControl();
    auto *const player = control ? static_cast<Character *>(control->ryo) : nullptr;
    if (!player)
        return false;
    memoCamera.playerDisplay = IsOfThisVisibilityType(player, 3);
    memoCamera.playerVisibility = (player->flags & CF_VisibleThisCheck) != 0;
    memoCamera.playerAlpha = player->alpha;
    memoCamera.state = camCtrlGetState();
    memoCamera.perspective = camCtrlGetPerspective();
    camCtrlGetNextCamPos(&memoCamera.position);
    camCtrlGetTargetPosition(&memoCamera.target);
    memoCamera.visibleHelp = static_cast<std::uint16_t>(GetUiFlag(0) & 0xfdffU);
    memoCamera.selectedHelp = static_cast<std::uint16_t>(GetUiFlag(1));
    cameraSaved = true;
    player->renderFlags &= static_cast<std::uint8_t>(~4U);
    player->flags &= ~CF_VisibleThisCheck;
    camCtrlSetState(Event);
    camCtrlSetPerspective(9830);
    notebookTranslation = 0.0f;
    memoNodes[3]->scale = {};
    memoNodes[1]->scale = {};
    memoNodes[2]->scale = {1.0f, 1.0f, 1.0f};
    turnAngle = 0;
    Vector3 target = GetCharacterWorldPosition(player);
    target.y += 1.6f;
    Vector3 eye = target;
    const auto inverseYaw = static_cast<std::int16_t>(-plymGetAngY());
    eye.x -= sin_fixed16(inverseYaw) * 0.2f;
    eye.z -= sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(inverseYaw)))) * -0.2f;
    camCtrlSetNextPosition(eye);
    camCtrlSetTargetPosition(target);
    if (control->windowTask)
        SetMemoWindowFlag(control->windowTask, 0);
    memoCamera.weatherMode = SetWeatherLightManager(0);
    return true;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void RestoreMemoCamera() {
    MapControlData *const map = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    MemoCameraSave *const saved = memo.savedCamera;
    Character *const player = map->ryo;
    if (!saved)
        return;
    if (player) {
        EV_SetCharDisplay(player, static_cast<CharaVisibilityMode>(3), saved->playerDisplay);
        charaSetVisibilityFlagTo(player, saved->playerVisibility);
    }
    camCtrlSetState(saved->state);
    camCtrlSetPerspectiveWrapper(static_cast<std::int16_t>(saved->perspective));
    camCtrlSetNextPosition(saved->position);
    camCtrlSetTargetPosition(saved->target);
    if (static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->retainedTaskD)
        wndSetFlag(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->retainedTaskD, 1);
    SetHelpIconMenuFlags(saved->visibleHelp, saved->selectedHelp);
    if (!saved->visibleHelp)
        ResetHelpIconsToItemPrompt();
    memo.savedCamera = 0;
}

} // namespace

#else

namespace {

void RestoreMemoCamera() {
    if (!cameraSaved)
        return;
    MapControlData *const control = MemoMapControl();
    auto *const player = control ? static_cast<Character *>(control->ryo) : nullptr;
    camCtrlSetState(memoCamera.state);
    camCtrlSetPerspective(memoCamera.perspective);
    camCtrlSetNextPosition(memoCamera.position);
    camCtrlSetTargetPosition(memoCamera.target);
    camCtrlSetFovTransitionTo(true);
    if (player) {
        (void)EV_SetCharDisplay(player, CVM_DisplayEnabled, memoCamera.playerDisplay);
        charaSetVisibilityFlagTo(player, memoCamera.playerVisibility);
        modelSetCharaAlpha(player, memoCamera.playerAlpha);
    }
    if (control && control->windowTask)
        SetMemoWindowFlag(control->windowTask, 1);
    SetHelpIconMenuFlags(memoCamera.visibleHelp);
    if (!memoCamera.visibleHelp)
        ResetHelpIconsToItemPrompt();
    SetWeatherLightManager(memoCamera.weatherMode);
    cameraSaved = false;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void SetMemoState7(std::int16_t page) {
    pageOffset = 0.0f;
    notebookTranslation = 0.0f;
    pagePosition = page;
    nextPage = page;
    pageThickness = static_cast<std::int16_t>(136 - page);
    memoState = 7;
    UpdateNotebook(page, 2, 3);
    pageModelBindings[1] = pageModelBindings[2];
    const std::size_t bank = 2 * (static_cast<std::uint16_t>(page) & 1U);
    pageModelBindings[0] = memoTextureHandles[bank];
    pageModelBindings[2] = memoTextureHandles[bank + 1];
    RebindMemoModelTextures();
    ++nextPage;
    pageOffsetStep = ((136 - pageThickness) * 0.0000027000001f - pageOffset) * 0.045454498f;
    memoNodes[0]->rotation.z = 0;
    memoNodes[2]->scale = {1.0f, pageThickness * 0.0046999999f, 1.0f};
    memoNodes[3]->rotation.z = 0;
    memoNodes[3]->scale = pagePosition <= 0 ? Vector3{} : Vector3{1.0f, (150 - pageThickness) * 0.0046999999f, 1.0f};
    SwapMemoPageTargets(0, 2);
    RebindNotebookWritingAreas();
    pageModelBindings[3] = pageModelBindings[0];
    RebindMemoModelTextures();
    (void)ExecuteAicaCommand(0xb00ab, 0, 0);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void ReleaseMemoResources() {
    if (memo.model)
        ReleaseMT7Model(memo.model);
    memo.model = 0;
    memo.draw.root = 0;
    memo.cover = 0;
    memo.leftPages = 0;
    memo.turningPage = 0;
    memo.rightPages = 0;
    std::int32_t index;
    for (index = 0; index < 4; ++index) {
        if (memoTextureHandles[index] != -1)
            ReleaseNamedTexture_Wrapper(memoTextureHandles[index]);
        memoTextureHandles[index] = -1;
    }
    for (index = 0; index < 2; ++index)
        memoTextures[index] = 0;
    if (MEMO_TEXTURE_DATA)
        HLib::FreeHeapSpace(MEMO_TEXTURE_DATA, SHEN_NAME(MEMT));
    MEMO_TEXTURE_DATA = 0;
}

} // namespace

#else

namespace {

void ReleaseMemoResources() {
    ReleaseModelFormat7(memoModel);
    if (memoModelSource)
        HLib::FreeTaskData(memoModelSource);
    memoModelSource = nullptr;
    memoNodes = {};
    turningNode = nullptr;
    for (std::uint32_t handle : memoModelOwnedTextures)
        if (handle && handle != UINT32_MAX)
            d3t::InitTexture2DResource(handle);
    memoModelOwnedTextures.clear();
    for (std::uint32_t &handle : memoTextureHandles) {
        if (handle && handle != UINT32_MAX)
            d3t::InitTexture2DResource(handle);
        handle = UINT32_MAX;
    }
    memoPageTextureAliases.fill(0);
    if (memoTextureData)
        HLib::FreeTaskData(memoTextureData);
    memoTextureData = nullptr;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
std::uint8_t PrepareMemoTextLine(char *destination, const char *source, std::uint16_t *colours, std::int32_t *selection) {
    char text[32];
    std::int32_t index;
    std::int32_t result = 0;
    sprintf(text, "%s", source);
    for (index = 0; index < 9; ++index)
        colours[index] = 0xb110;
    selection[0] = selection[1] = -1;
    if (text[0] == '$') {
        for (index = 1; index < 3; ++index) {
            if (text[index] > '@')
                selection[index - 1] = text[index] - '7';
            else
                selection[index - 1] = text[index] - '0';
        }
        for (index = 3; index < 32; ++index) {
            text[index - 3] = text[index];
            if (!text[index])
                break;
        }
        if (selection[0] == selection[1])
            selection[0] = selection[1] = -1;
    }
    if (text[0] == '@') {
        std::int32_t shift = 16;
        std::uint32_t code = 0;
        for (index = 1; index <= 5; ++index, shift -= 4) {
            std::int32_t digit;
            if (text[index] > '@')
                digit = text[index] - '7';
            else
                digit = text[index] - '0';
            code |= static_cast<std::uint32_t>(digit) << shift;
        }
        static const std::uint16_t palette[4] = {0xb110, 0xb70b, 0xbf00, 0xb00f};
        code &= 0x3ffffU;
        for (index = 0; index < 9; ++index) {
            const std::int32_t bits = (8 - index) * 2;
            const std::uint32_t mask = 3U << bits;
            colours[index] = palette[(code & mask) >> bits];
        }
        for (index = 6; index < 32; ++index) {
            text[index - 6] = text[index];
            if (!text[index])
                break;
        }
    }
    strcpy(destination, text);
    return result;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void RasterizeMemoRuby(FontRuntimeData *font, std::uint16_t glyph, std::uint16_t *destination, std::int32_t bytePitch) {
    std::uint8_t coverage[400] = {};
    const std::byte *const bitmap = GetWindowGlyphBitmap(font, glyph);
    if (glyph <= 0x20ff || (glyph & 0x8080U) == 0x8080U)
        return;
    for (std::int32_t bit = 0; bit < 360; ++bit) {
        if (!(static_cast<std::uint8_t>(bitmap[bit >> 3]) & (1U << (bit & 7))))
            continue;
        const std::int32_t row = bit / 24, column = bit % 24;
        coverage[row * 25 + column] |= 2;
        coverage[row * 25 + column + 1] |= 1;
        coverage[(row + 1) * 25 + column] |= 1;
        coverage[(row + 1) * 25 + column + 1] |= 1;
    }
    static const std::uint16_t palette[4] = {0, 0, 0xa00b, 0xa00b};
    std::byte *row = reinterpret_cast<std::byte *>(destination) + bytePitch * 7;
    for (std::int32_t y = 0; y < 8; ++y) {
        std::uint16_t *const output = reinterpret_cast<std::uint16_t *>(row);
        std::int32_t offset = 0;
        for (std::int32_t x = 1; x < 25; x += 2)
            output[offset++] =
                palette[coverage[y * 50 + x]] | palette[coverage[y * 50 + x + 1]] | palette[coverage[y * 50 + x + 25]] | palette[coverage[y * 50 + x + 26]];
        row -= bytePitch;
    }
}
#endif

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void RasterizeMemoLine(std::uint16_t *destination, const char *text, const char *ruby, std::int32_t width, const std::uint16_t *colours) {
    FontRuntimeData *const font = GetFontRuntimeData();
    const std::int32_t bytePitch = width * 2;
    std::uint16_t *cursor = destination;
    while (*text) {
        std::uint16_t glyph;
        const bool multibyte = IsWindowMultibyteCharacter(static_cast<std::uint8_t>(*text));
        if (multibyte) {
            glyph = static_cast<std::uint16_t>(((static_cast<std::uint8_t>(text[0]) << 8) | static_cast<std::uint8_t>(text[1])) & 0x7f7f);
            text += 2;
        } else {
            glyph = static_cast<std::uint8_t>(*text++) | 0x100;
        }
        RasterizeWindowGlyph(font, glyph, cursor, bytePitch, *colours++, 0, 0);
        cursor += multibyte ? 26 : 14;
    }
    if (!ruby)
        return;
    WindowTaskData *const window = static_cast<WindowTaskData *>(HLib::GetTaskParameter(windowTask));
    const std::uint32_t savedFlag = window->customFlag;
    wndSetFlag(0, 1);
    cursor = destination + width * 16;
    while (*ruby) {
        if (!IsWindowMultibyteCharacter(static_cast<std::uint8_t>(*ruby)))
            break;
        const std::uint16_t glyph = static_cast<std::uint16_t>(((static_cast<std::uint8_t>(ruby[0]) << 8) | static_cast<std::uint8_t>(ruby[1])) & 0x7f7f);
        ruby += 2;
        RasterizeMemoRuby(font, glyph, cursor, bytePitch);
        cursor += 13;
    }
    wndSetFlag(0, savedFlag);
}
#endif

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void ExpandMemoText(char *destination, const char *source) {
    char *cursor = destination;
    while (*source) {
        if (IsWindowMultibyteCharacter(*source)) {
            *cursor++ = *source++;
            *cursor++ = *source++;
        } else if (*source == '0') {
            const std::int32_t count = source[1] - '0';
            for (std::int32_t index = 0; index < count; ++index) {
                *cursor++ = '\241';
                *cursor++ = '\241';
            }
            source += 2;
        } else {
            const std::int32_t tens = *source++ - '1';
            const char *const word = memoTextDictionary[tens * 10 + (*source++ - '0')];
            strcat(destination, word);
            cursor += strlen(word);
        }
    }
    *cursor = 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
const char *GetMemoText(const std::byte *table, std::int32_t index) {
    const char *text = 0;
    if (table) {
        const std::uintptr_t master = reinterpret_cast<std::uintptr_t>(g_memoMasterTable);
        if (master) {
            const std::int32_t offset = reinterpret_cast<const std::int32_t *>(table)[index];
            if (offset != -1)
                text = reinterpret_cast<const char *>(offset + master);
        }
    }
    return text;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

// @verified - d
void FormatMemoTransferredMoney(char *destination, std::int32_t hongKong) {
    std::int32_t value, divisor, first;
    char *cursor = destination;
    if (hongKong) {
        value = static_cast<std::int32_t>(static_cast<float>(g_transferredYen) * 0.033333335f);
        *cursor++ = '¡';
        *cursor++ = '¡';
        *cursor++ = '¡';
        *cursor++ = '¡';
        *cursor++ = '¡';
        *cursor++ = '¡';
        divisor = 100000;
        first = 2;
    } else {
        value = g_transferredYen;
        divisor = 10000000;
        first = 0;
    }
    std::int32_t nonzero = 0;
    for (std::int32_t index = first; index < 8; ++index) {
        const std::int32_t digit = value / divisor;
        if (digit) {
            if (hongKong && nonzero != 1) {
                cursor[-1] = 'ð';
                cursor[-2] = '¡';
                cursor[-3] = 'Ë';
                cursor[-4] = '£';
                cursor[-5] = 'È';
                cursor[-6] = '£';
            }
            nonzero = 1;
        }
        if (nonzero) {
            const std::int32_t code = 0xa3b0 + digit;
            const std::uint16_t glyph = code;
            value %= divisor;
            *cursor++ = static_cast<char>(glyph >> 8);
            *cursor++ = static_cast<char>(glyph);
        } else {
            *cursor++ = '¡';
            *cursor++ = '¡';
        }
        divisor /= 10;
    }
    if (!nonzero) {
        static const char yenZero[] = "¡¡¡¡¡¡¡¡¡¡¡¡¡¡£°±ß";
        static const char hongKongZero[] = "¡¡¡¡¡¡¡¡¡¡£È£Ë¡ð£°";
        if (!hongKong)
            memcpy(destination, yenZero, 19);
        else
            memcpy(destination, hongKongZero, 19);
    } else {
        *cursor = 0;
        if (!hongKong)
            strcat(destination, "±ß");
    }
}

} // namespace

#else

namespace {

std::string FormatMemoTransferredMoney(bool hongKong) {
    std::string result(hongKong ? 6 : 0, '\xa1');
    std::int32_t amount = GetTransferredYen();
    std::int32_t divisor = hongKong ? 100000 : 10000000;
    if (hongKong)
        amount = static_cast<std::int32_t>(static_cast<float>(amount) * 0.033333335f);
    bool started = false;
    for (std::int32_t digit = 0; digit < (hongKong ? 6 : 8); ++digit) {
        const std::int32_t value = amount / divisor;
        if (value || started) {
            if (hongKong && !started) {
                // Native writes the six-byte full-width HK$ prefix immediately
                // before the first significant digit, including leading blanks.
                constexpr std::array<char, 6> prefix{'\xc8', '\xa3', '\xc9', '\xa3', '\xa4', '\xa3'};
                const auto offset = result.size() - prefix.size();
                std::copy(prefix.begin(), prefix.end(), result.begin() + offset);
            }
            started = true;
            const auto code = static_cast<std::uint16_t>(value - 23632);
            result.push_back(static_cast<char>(code >> 8U));
            result.push_back(static_cast<char>(code));
            amount %= divisor;
        } else {
            result.append(2, '\xa1');
        }
        divisor /= 10;
    }
    if (!started)
        return hongKong ? "            HK$0" : "            0 Yen";
    if (!hongKong)
        result += "Yen";
    return result;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void MemoUnusedNullsub() {
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void ReplaceMemoMoney(std::string &text, std::string_view key, std::int32_t value) {
    const std::string replacement = std::to_string(value);
    std::size_t position = 0;
    while ((position = text.find(key, position)) != std::string::npos) {
        text.replace(position, key.size(), replacement);
        position += replacement.size();
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// PrepareMemoNotebookLine: localized PC labels retain the native A/B/0 conditional suffix.
void PrepareMemoNotebookLine(
    std::int32_t page, std::uint32_t line, std::uint32_t destination, std::uint32_t borrowed, char messageType, std::string_view legacy) {
    std::array<char, 11> label{};
    std::snprintf(label.data(), label.size(), "MEMP%i%i%iL%i%c", page / 100, page % 100 / 10, page % 10, static_cast<int>(line), messageType);
    std::string text;
    if (memoDebugMode == 2)
        text = legacy;
    else if (memoDebugMode == 3)
        text = label.data();
    else if (memoDebugMode == 4) {
        std::array<char, 100> pageText{};
        std::snprintf(pageText.data(), pageText.size(), "TEST: PAGE %u", destination + 1);
        text = pageText.data();
    } else {
        text = GetLocalizedSceneText(label.data());
        if (messageType == '0' || text == "[EMPTY LINE]")
            text = " ";
        else if (text.empty() || text == label.data())
            text = memoDebugMode == 1 ? legacy : " ";
        else {
            ReplaceMemoMoney(text, "[TRANSFER_YEN]", GetTransferredYen());
            ReplaceMemoMoney(text, "[TRANSFER_HKD]", static_cast<std::int32_t>(static_cast<float>(GetTransferredYen()) * 0.033333335f));
        }
    }
    auto &target = memoPages.text[destination];
    const std::size_t used = std::strlen(target.data());
    if (used + 1 < target.size()) {
        const std::size_t count = std::min(text.size(), target.size() - used - 2);
        std::memcpy(target.data() + used, text.data(), count);
        target[used + count] = '\n';
        target[used + count + 1] = 0;
    }
    memoPages.borrowedText[borrowed] = static_cast<std::int32_t>(destination);
    memoPages.activePage = page;
    RasterMemoLine(line, legacy);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void PrepareNotebook(std::int32_t page, std::uint32_t left, std::uint32_t right) {
    if (page < 0 || static_cast<std::uint32_t>(page) >= g_memoRecordCount || static_cast<std::size_t>(page) >= memoLineFlags.size())
        return;
    for (auto &text : memoPages.text)
        text[0] = 0;
    for (std::uint32_t line = 0; line < 10; ++line) {
        const std::int16_t flag = memoLineFlags[page][line];
        std::string_view legacy = " ";
        std::string money;
        char messageType = '0';
        if (flag >= 0) {
            if (!flag || EV_GetPlayerFlag(PlayerFlagReadMode::Internal, static_cast<std::int16_t>(flag))) {
                messageType = 'A';
                const std::uint32_t offset = ReadMemoWord(g_memoData + 8 + 4 * (10 * page + line));
                if (offset != UINT32_MAX)
                    legacy = reinterpret_cast<const char *>(g_memoMasterTable + offset);
            }
            if (page == 4 && EV_GetPlayerFlag(PlayerFlagReadMode::Internal, static_cast<std::int16_t>(138)) && (line == 5 || line == 7)) {
                money = FormatMemoTransferredMoney(line != 5);
                legacy = money;
            }
        } else {
            const ConditionalMemoLine &conditional = conditionalMemoLines[-flag - 1];
            for (std::size_t choice = 0; choice < 2; ++choice) {
                if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, static_cast<std::int16_t>(conditional.flags[choice])))
                    continue;
                legacy = conditional.text[choice];
                messageType = choice ? 'B' : 'A';
                break;
            }
        }
        PrepareMemoNotebookLine(page, line, line < 5 ? left : right, 2 * (page & 1) + line / 5, messageType, legacy);
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void RebindMemoModelTextures() {
    // RebindMemoModelTextures writes the existing texture table, then reparses each node's
    // bindings. Neither the vertex buffers nor the model owner are replaced.
    for (std::size_t index = 0; index < memoModel.textures.size(); ++index) {
        std::uint32_t handle = index >= 3 && index < 7 ? pageModelBindings[index - 3] : memoModel.textures[index].textureIndex;
        for (std::size_t page = 0; page < memoTextureHandles.size(); ++page) {
            if (handle == memoTextureHandles[page] && memoPageTextureAliases[page]) {
                handle = memoPageTextureAliases[page];
                break;
            }
        }
        memoModel.textures[index].textureIndex = handle;
    }
    for (MT7Node &node : memoModel.nodes) {
        for (LegacyTextureBinding &binding : node.drawResources.textureBindings) {
            if (binding.textureTableIndex >= 0 && static_cast<std::size_t>(binding.textureTableIndex) < memoModel.textures.size())
                binding.textureHandle = memoModel.textures[binding.textureTableIndex].textureIndex;
        }
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void SwapMemoPageTargets(std::uint32_t first, std::uint32_t second) {
    std::swap(memoPages.targets[1][first], memoPages.targets[1][second]);
    memoPages.dirty[1][first] = true;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void FillNotebookWritingArea(void *areas, const ModelMaterialDescriptor &descriptor) {
    const char *const suffix = std::strstr(descriptor.name, "Writing_Area_");
    if (!suffix)
        return;
    const std::int32_t index = std::atoi(suffix + 13);
    if (static_cast<std::uint32_t>(index - 1) <= 3U)
        static_cast<ModelMaterialDescriptor *>(areas)[index - 1] = descriptor;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// Bank one holds the writing-area targets, bank zero the page targets that replace the MEMT textures.
// @verified - p
std::uint32_t EnsureNotebookPageTarget(std::uint32_t kind, std::uint8_t index) {
    const std::size_t bank = kind == 1 ? 1 : 0;
    std::uint32_t &target = memoPages.targets[bank][index];
    if (!target) {
        const d3t::Texture2DDescription description{512, 512, 1, 1, 2, 4, 0, 1, 0, 0, nullptr};
        target = d3t::CreateTexture2D(&description);
    }
    memoPages.dirty[bank][index] = true;
    return target;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// The legacy texture's slot takes the page target; legacy textures are identified by their renderer handles, so the
// writing-area calls' texture 0 names none.
void SetNotebookPageTexture(std::uint32_t kind, std::uint32_t legacyTexture, std::uint8_t index) {
    const std::uint32_t target = EnsureNotebookPageTarget(kind, index);
    for (std::size_t page = 0; page < memoTextureHandles.size(); ++page) {
        if (memoTextureHandles[page] == legacyTexture) {
            memoPageTextureAliases[page] = target;
            break;
        }
    }
    memoPages.dirty[kind == 1 ? 1 : 0][index] = true;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void RebindNotebookWritingAreas() {
    std::array<ModelMaterialDescriptor, 4> areas{};
    if (!d3t::VisitModelMaterials(memoNotebookModelHandle, areas.data(), FillNotebookWritingArea))
        return;
    for (std::uint32_t page = 0; page < areas.size(); ++page) {
        ModelMaterialDescriptor &area = areas[page];
        if (!area.name)
            continue;
        for (std::uint32_t parameter = 0; parameter < area.material->parameterCount; ++parameter) {
            if (area.parameters[parameter].type == MEMO_PAGE_TEXTURE_PARAMETER) {
                d3t::SetModelMaterialTexture(area, parameter, memoPages.targets[1][page]);
                break;
            }
        }
    }
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

// @verified - p
void NotebookModelLoaded(void *, d3t::ObjectSource *source) {
    if (source->resolveState != 1)
        return;
    if (!memoPages.targets[1][0]) {
        for (std::uint8_t index = 0; index < 4; ++index)
            EnsureNotebookPageTarget(1, index);
    }
    memoNotebookModelHandle = source->renderHandle;
    RebindNotebookWritingAreas();
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

void UpdateNotebook(std::int16_t page, std::uint32_t left, std::uint32_t right) {
    for (MemoTexture &texture : memoTextureData->textures)
        texture.pixels.fill(0);
    PrepareNotebook(page, left, right);
    SetNotebookPageTexture(1, 0, static_cast<std::uint8_t>(left));
    SetNotebookPageTexture(1, 0, static_cast<std::uint8_t>(right));
    const std::uint32_t bank = 2 * (static_cast<std::uint16_t>(page) & 1U);
    SetNotebookPageTexture(0, memoTextureHandles[bank], static_cast<std::uint8_t>(bank));
    SetNotebookPageTexture(0, memoTextureHandles[bank + 1], static_cast<std::uint8_t>(bank + 1));
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool SetupMemoTextureTaskData() {
    if (memoTextureData)
        return false;
    memoTextureData = static_cast<MemoTextureTaskData *>(HLib::AllocateTaskData(sizeof(MemoTextureTaskData), SHEN_NAME(MEMT)));
    if (!memoTextureData)
        return false;
    HLib::AssignDynamicAlignmentForTaskData(memoTextureData);
    for (std::size_t bank = 0; bank < 2; ++bank) {
        for (std::size_t index = 0; index < 2; ++index) {
            MemoTexture &texture = memoTextureData->textures[index];
            texture.signature = SHEN_NAME(TEXN);
            texture.size = sizeof(MemoTexture);
            texture.id = {};
            std::memcpy(texture.id.data(), memoTextureNames[bank * 2 + index], 7);
            texture.gbix = SHEN_NAME(GBIX);
            texture.gbixSize = 4;
            texture.gbixValue = 0;
            texture.pvrt = SHEN_NAME(PVRT);
            texture.pvrtSize = 0x20008;
            texture.format = 0x0989;
            texture.width = 256;
            texture.height = 128;
            texture.pixels.fill(0);
            memoTextureHandles[bank * 2 + index] = UploadTexn({reinterpret_cast<const std::byte *>(&texture), sizeof(texture)});
        }
        memoTextureData->terminator = 0;
    }
    return std::all_of(memoTextureHandles.begin(), memoTextureHandles.end(), [](std::uint32_t handle) { return handle != 0 && handle != UINT32_MAX; });
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

bool LoadNotebookModel() {
    constexpr char path[] = "model/item/MMA02G1T.MT7";
    const std::uint64_t size = ArchiveData_GetFileSize(path);
    if (!size)
        return false;
    memoModelSource = static_cast<std::byte *>(HLib::AllocateTaskData(size, SHEN_NAME(MEMO)));
    if (!memoModelSource)
        return false;
    if (!ArchiveData_Read(path, 0, memoModelSource, size) || !ReadModelFormat7({memoModelSource, static_cast<std::size_t>(size)}, path, memoModel)) {
        ReleaseModelFormat7(memoModel);
        HLib::FreeTaskData(memoModelSource);
        memoModelSource = nullptr;
        return false;
    }
    d3t::RegisterObjectSource(GetMT7ReaderData(&memoModel), "model/item/MMA02G1T/MMA02G1T.fbx", 2, 0, nullptr, NotebookModelLoaded);
    for (std::size_t offset = ReadMemoWord(memoModelSource + 4); offset + 8 <= size;) {
        const std::uint32_t chunkSize = ReadMemoWord(memoModelSource + offset + 4);
        if (chunkSize < 8 || chunkSize > size - offset)
            break;
        if (ReadMemoWord(memoModelSource + offset) == SHEN_NAME(TEXN)) {
            const std::uint32_t handle = UploadTexn({memoModelSource + offset, chunkSize});
            memoModelOwnedTextures.push_back(handle);
            for (MT7TextureEntry &texture : memoModel.textures) {
                if (std::memcmp(texture.id, memoModelSource + offset + 8, 8) == 0)
                    texture.textureIndex = handle;
            }
        }
        offset += chunkSize;
    }
    std::size_t root = memoModel.nodes.size();
    for (std::size_t index = 0; index < memoModel.nodes.size(); ++index) {
        if (memoModel.nodes[index].parent == -1) {
            root = index;
            break;
        }
    }
    if (root == memoModel.nodes.size())
        return false;
    std::size_t siblings = 0;
    for (MT7Node &node : memoModel.nodes) {
        if (node.parent == static_cast<std::int32_t>(root) && siblings < memoNodes.size())
            memoNodes[siblings++] = &node;
    }
    if (siblings != memoNodes.size())
        return false;
    if (memoModel.textures.size() < pageModelBindings.size() + 3)
        return false;
    for (std::size_t index = 0; index < pageModelBindings.size(); ++index) {
        pageModelBindings[index] = memoTextureHandles[index];
        memoModel.textures[index + 3].textureIndex = memoTextureHandles[index];
    }
    RebindMemoModelTextures();
    return true;
}

} // namespace

#endif

} // namespace s2
} // namespace shenmue
