#include "s2/memo.hpp"

#include "d3t/image_texture_loader.hpp"
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
#include <cstring>
#include <limits>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace shenmue::s2 {
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

HLib::Task *memoTask{};
PadControlEntry *memoPad{};
PadControlEntry *previousPad{};
MemoTextureTaskData *memoTextureData{};
std::array<std::uint32_t, 4> memoTextureHandles{};
std::array<std::uint32_t, 4> memoPageTextureAliases{};
MT7Model memoModel{};
std::byte *memoModelSource{};
std::vector<std::uint32_t> memoModelOwnedTextures;
std::array<MT7Node *, 4> memoNodes{};
MT7Node *turningNode{};
MemoCameraSave memoCamera{};
bool cameraSaved{};
ConversationBuffer *memoAudio{};
std::int32_t memoState{};
std::int32_t memoSecondState{};
std::int32_t memoInputMode{};
std::int32_t repeatTurn{};
std::int32_t explicitPage{-1};

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

std::uint32_t ReadMemoWord(const std::byte *source) {
    std::uint32_t value{};
    std::memcpy(&value, source, sizeof(value));
    return value;
}

MapControlData *MemoMapControl() {
    return static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
}

void SetMemoWindowFlag(HLib::Task *task, std::int32_t flag) {
    if (auto *data = static_cast<MemoWindowTaskView *>(HLib::GetTaskParameter(task)))
        data->customFlag = flag;
}

std::uint32_t ExpandMemoColour(std::uint16_t colour) {
    return 17U * ((colour & 15U) | (((colour >> 4U) & 15U) << 8U) | (((colour >> 8U) & 15U) << 16U) | ((colour >> 12U) << 24U));
}

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

void ReplaceMemoMoney(std::string &text, std::string_view key, std::int32_t value) {
    const std::string replacement = std::to_string(value);
    std::size_t position = 0;
    while ((position = text.find(key, position)) != std::string::npos) {
        text.replace(position, key.size(), replacement);
        position += replacement.size();
    }
}


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

void SwapMemoPageTargets(std::uint32_t first, std::uint32_t second) {
    std::swap(memoPages.targets[1][first], memoPages.targets[1][second]);
    memoPages.dirty[1][first] = true;
}

void UpdateNotebook(std::int16_t page, std::uint32_t left, std::uint32_t right) {
    for (MemoTexture &texture : memoTextureData->textures)
        texture.pixels.fill(0);
    PrepareNotebook(page, left, right);
    memoPages.dirty[1][left] = true;
    memoPages.dirty[1][right] = true;
    const std::size_t bank = 2 * (static_cast<std::uint16_t>(page) & 1U);
    for (std::size_t index = 0; index < 2; ++index) {
        // UpdateNotebook replaces the legacy texture slot's renderer handle with
        // the page target. The retained MEMT bitmap is not re-uploaded.
        memoPageTextureAliases[bank + index] = memoPages.targets[0][bank + index];
        memoPages.dirty[0][bank + index] = true;
    }
}

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

bool LoadNotebookModel() {
    constexpr char path[] = "model/item/MMA02G1T.MT7";
    const std::uint64_t size = ArchiveData_GetFileSize(path);
    if (!size)
        return false;
    memoModelSource = static_cast<std::byte *>(HLib::AllocateTaskData(size, SHEN_NAME(MEMO)));
    if (!memoModelSource || !ArchiveData_Read(path, 0, memoModelSource, size))
        return false;
    if (!ReadModelFormat7({memoModelSource, static_cast<std::size_t>(size)}, path, memoModel, 2))
        return false;
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
        player->renderFlags = static_cast<std::uint8_t>((player->renderFlags & ~CRF_DisplayEnabled) | (memoCamera.playerDisplay ? 4U : 0U));
        player->flags = (player->flags & ~CF_VisibleThisCheck) | (memoCamera.playerVisibility ? CF_VisibleThisCheck : 0U);
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
    pageModelBindings[3] = pageModelBindings[0];
    RebindMemoModelTextures();
    (void)ExecuteAicaCommand(0xb00ab, 0, 0);
}

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
    DrawMT7Model(memoModel, nullptr, LegacyModelShaderPath::VertexColour, 1.0f);
    EndTemporaryAttachedLight(attachedLight);
    Matrix4x4::Pop(1);
    EndTemporaryGlobalLighting(lighting);
}

void PlayMemoAudio(std::int32_t volume) {
    memoAudio = shenPlayAudio("MEMO", shen_name{}, 17, false);
    SetConversationVolume(memoAudio, volume);
}

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

void InitializeMemoPageState(std::int16_t page, std::int16_t flag) noexcept {
    memoPageState.latestPage = page;
    memoPageState.selectedPage = page;
    memoPageState.categoryMask = 0;
    memoPageState.requestFlags = 0;
    memoPageState.savedFlag = flag;
}

void ResetMemoPageState() noexcept {
    memoPageState.savedFlag = -1;
    memoPageState.category = 0;
    memoPageState.categoryMask = 0;
    memoPageState.selectedPage = 1;
    memoPageState.latestPage = 1;
    memoPageState.requestFlags = 0;
}

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

// @verified - p
void OpenMemoFromNotebookMenu() {
    (void)RefreshPadTask(0, 1);
    SetHudItemPromptIcon(0);
}

// @verified - p
void OpenMemoAtPage(std::int32_t page) {
    explicitPage = page;
    (void)RefreshPadTask(0, 1);
    SetHudItemPromptIcon(0);
}

// @verified - p
std::int64_t CloseMemoFromNotebookMenu() {
    if (memoState == 0)
        return 1;
    if (static_cast<std::uint32_t>(memoState - 8) > 1U)
        memoState = 8;
    return 0;
}

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

void RequestMemoClose() noexcept {
    if (memoState)
        memoState = 8;
}

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

std::int64_t CreateMemoNotebookPages() {
    SetupNotebook();
    EnsureMemoTitleTextures();
    return 1;
}

std::int64_t DestroyMemoNotebookPages() {
    // DestroyMemoNotebookPages releases the eight targets, not the process-lifetime
    // three border textures installed by the UI callback's init operation.
    for (std::size_t index = 0; index < 4; ++index) {
        for (std::size_t bank : {std::size_t{1}, std::size_t{0}}) {
            d3t::InitTexture2DResource(memoPages.targets[bank][index]);
            memoPages.targets[bank][index] = 0;
        }
    }
    return 1;
}

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
    d3t::D3TDeviceContext *context = g_nuklearRenderDeviceContext;
    if (!dirty)
        return;
    nk_clear(GetNativeUiContext());
    const std::array<float, 6> savedViewport = d3t::GetRenderState(*context).viewport;
    const Matrix4x4 orthographic{
        .rows = {{.x = 2.0f / MEMO_PAGE_WIDTH, .w = -1.0f}, {.y = 2.0f / MEMO_PAGE_HEIGHT, .w = -1.0f}, {.z = -1.0f, .w = -0.0f}, {.w = 1.0f}}};
    SetNuklearViewMatrix(orthographic);
    if (!context)
        context = SetupNuklearBuffer();
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
    context = g_nuklearRenderDeviceContext;
    if (context) {
        GetAspectRatioModeShouldScale();
        InvokeNuklearFrame(context);
        if (savedViewport[2] != 0.0f)
            d3t::SetRenderViewport(context, savedViewport);
    }
    nk_clear(GetNativeUiContext());
}

} // namespace shenmue::s2
