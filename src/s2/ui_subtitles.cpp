#include "s2/ui.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/easy_option.hpp"
#include "s2/opening_staff_roll.hpp"
#include "s2/magic_room.hpp"
#include "s2/ctrl_speak_icon.hpp"
#include "s2/now_qte.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/battle_qte_ui.hpp"
#include "s2/ui_transition.hpp"
#include "s2/ui_scene.hpp"
#include "s2/ui_grid.hpp"
#include "s2/ui_collection.hpp"
#include "s2/ui_moves.hpp"
#include "s2/ui_loadsave.hpp"
#include "s2/ui_prompt.hpp"
#include "s2/input_bindings.hpp"
#include "s2/character.hpp"
#include "s2/camera.hpp"
#include "s2/mt7.hpp"
#include "s2/world_render.hpp"
#include "s2/areas/area_0001.hpp"
#include "s2/areas/game_arcade.hpp"
#include "s2/areas/omak_tables.hpp"
#include "s2/omak_collection_ui.hpp"

#include "d3t/content.hpp"
#include "d3t/image_texture_loader.hpp"
#include "d3t/render_handles.hpp"
#include "d3t/texture.hpp"
#include "d3t/threads.hpp"
#include "d3t/ui_definition.hpp"
#include "d3t/ui_renderer.hpp"
#include "d3t/ui_view.hpp"
#include "d3t/ui_runtime.hpp"
#include "d3t/dx11_renderer.hpp"
#include "platform/performance_counter.hpp"
#include "s2/input.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/item_wf.hpp"
#include "s2/item_selector.hpp"
#include "s2/main_icon.hpp"
#include "s2/flag.hpp"
#include "s2/item_selector_actions.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/math.hpp"
#include "s2/screen_film.hpp"
#include "s2/renderer_initialization.hpp"
#include "s2/save.hpp"
#include "s2/snapshot_viewer.hpp"
#include "s2/snapshot.hpp"
#include "s2/conversation.hpp"
#include "s2/system_modules.hpp"
#include "s2/title.hpp"
#include "s2/scene_data.hpp"
#include "s2/font.hpp"
#include "s2/archive_data.hpp"
#include "s2/pak.hpp"
#include "s2/directory_cache.hpp"
#include "s2/map_control.hpp"
#include "s2/player.hpp"
#include "s2/event_runtime.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/inventory.hpp"
#include "s2/date_time.hpp"
#include "s2/gauge.hpp"
#include "s2/npc_package.hpp"
#include "s2/sound_program.hpp"
#include "s2/navigation.hpp"
#include "s2/pawn_shop.hpp"
#include "s2/author.hpp"
#include "s2/help.hpp"
#include "s2/pause.hpp"
#include "s2/memo.hpp"
#include "s2/search.hpp"
#include "s2/story_state.hpp"
#include "s2/weather.hpp"
#include "s2/display_configuration.hpp"
#include "s2/audio_data.hpp"
#include "s2/hand.hpp"
#include "s2/full_map_menu.hpp"
#include "s2/legacy_collection.hpp"
#include "s2/pawn.hpp"
#include "s2/game_runtime.hpp"
#include <json/json.h>
#include <algorithm>
#include <array>
#include <atomic>
#include <bit>
#include <charconv>
#include <cctype>
#include <cmath>
#include <limits>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <memory>
#include <mutex>
#include <new>
#include <string>
#include <stdexcept>
#include <vector>
#include "s2/area_names.hpp"

#include "s2/ui_internal.hpp"

namespace shenmue::s2 {
using namespace ui_detail;

namespace ui_detail {

std::size_t SubtitleWrapSpace(std::string_view text) {
    // SubtitleWrapSpace selects the ASCII space nearest the byte midpoint.
    std::size_t result = std::string_view::npos;
    std::size_t distance = text.size();
    for (std::size_t index = 0; index < text.size(); ++index) {
        if (text[index] != ' ')
            continue;
        const auto difference = index > text.size() / 2 ? index - text.size() / 2 : text.size() / 2 - index;
        if (difference < distance) {
            distance = difference;
            result = index;
        }
    }
    return result;
}

SubtitleLabelFit FitSubtitleLabel(
    const UIFontSelection &font, std::string_view text, const d3t::UIRect &bounds, float referenceWidth, float referenceHeight, bool allowWrap) {
    // FitSubtitleLabel's lower scale limit is 0.33; below
    // 0.5 it may split once, then both halves use the smaller fitted scale.
    const float baseHeight = GetUIFontHeight(font);
    if (!(baseHeight > 0.0f))
        return {};
    float scale = 0.33f;
    if (referenceWidth > bounds.width)
        scale = bounds.width / referenceWidth;
    if (referenceHeight <= bounds.height) {
        scale = 1.0f;
    } else {
        scale = std::max(scale, bounds.height / referenceHeight);
    }
    const float initialHeight = baseHeight * scale;
    const float width = GetUIFontTextWidth(font, initialHeight, text);
    if (width > bounds.width)
        scale *= bounds.width / width;
    if (initialHeight > bounds.height) {
        const float ratio = bounds.height / initialHeight;
        if (scale > ratio)
            scale *= ratio;
    }
    scale = std::max(scale, 0.33f);
    SubtitleLabelFit result{baseHeight * scale};
    if (allowWrap && scale < 0.5f) {
        result.split = SubtitleWrapSpace(text);
        if (result.split != std::string_view::npos) {
            d3t::UIRect half = bounds;
            half.height *= 0.5f;
            const auto first = FitSubtitleLabel(font, text.substr(0, result.split), half, referenceWidth, referenceHeight * 0.5f, false);
            const auto second = FitSubtitleLabel(font, text.substr(result.split + 1), half, referenceWidth, referenceHeight * 0.5f, false);
            result.height = std::min(first.height, second.height);
        }
    }
    return result;
}

// @verified - p
bool AreSubtitleContentsEmpty() {
    return (!subtitleBackgrounds[1] || subtitleBackgrounds[1]->field44 == 0) && (!subtitleBackgrounds[0] || subtitleBackgrounds[0]->field44 == 0) &&
        (!subtitlePanels[0] || subtitlePanels[0]->alpha == 0.0f) && (!subtitlePanels[1] || subtitlePanels[1]->alpha == 0.0f) && subtitleText[0][0] == 0 &&
        subtitleText[1][0] == 0 && subtitleText[2][0] == 0;
}
// @verified - p
void FadeSubtitlePanels() {
    if (g_debugUiDisabled == 0) {
        if (subtitlePanels[0] && subtitlePanels[0]->alpha == 1.0f && !IsUITransitionActive(subtitleAlphaBindings[0]))
            UIWidget_Animate(subtitleAlphaBindings[0], 0, 0.30000001f, 0.0f);
        if (g_debugUiDisabled == 0 && subtitlePanels[1] && subtitlePanels[1]->alpha == 1.0f && !IsUITransitionActive(subtitleAlphaBindings[1]))
            UIWidget_Animate(subtitleAlphaBindings[1], 0, 0.30000001f, 0.0f);
    }
}
// @verified - p
std::int64_t EnableSubtitles() {
    subtitlesEnabled = 1;

    return 0;
}
// @verified - p
std ::int64_t ClearSubtitles() {
    subtitlesEnabled = 0;
    subtitleScratch[0] = 0;
    std::memset(subtitleText.data(), 0, sizeof(subtitleText));
    return 1;
}
// @verified - p
std::int64_t RenderSubtitles() {
    const std ::uint32_t slot = g_uiElements[static_cast<std::size_t>(UIElement ::Subtitles)].config.uiHandleIndex;
    if (!GetNuklearElementByIdx(slot))
        return 0;
    if (subtitlesEnabled != 1 || AreSubtitleContentsEmpty()) {
        for (std::size_t row = 0; row < 3; ++row)
            for (const std::size_t layer : {2U, 1U, 0U}) {
                const std ::int32_t index = subtitleNativeLabels[layer][row];
                if (index != -1) {
                    d3t::UILabelWidget *const label = UI_GetLabel(static_cast<std::uint32_t>(index));
                    label->text = "";
                    label->token.alreadyLocalized = 1;
                }
            }
    } else {
        StartUIIntegerTransition(subtitleActivityTransition, 1, 0, 0.1f, 0.30000001f);
        if (!IsUITransitionActive(subtitleAlphaBindings[0]) && subtitlePanels[0] && subtitlePanels[0]->alpha == 0.0f)
            UIWidget_Animate(subtitleAlphaBindings[0], 1, 0.1f, 0.0f);
        if (!IsUITransitionActive(subtitleAlphaBindings[1]) && subtitlePanels[1] && subtitlePanels[1]->alpha == 0.0f)
            UIWidget_Animate(subtitleAlphaBindings[1], 1, 0.1f, 0.0f);
        for (std::size_t row = 0; row < 3; ++row) {
            for (const std::size_t layer : {2U, 1U, 0U}) {
                d3t::UILabelWidget *const label = UI_GetLabel(static_cast<std::uint32_t>(subtitleNativeLabels[layer][row]));
                label->text = subtitleText[row].data();
                label->colour = subtitleVoiceColours[layer];
                label->size = 0;
                label->fontFamily = layer == 0 ? 0 : 1;
                label->token.alreadyLocalized = 1;
            }
        }
    }
    if (GetUIIntegerTransitionValue(subtitleActivityTransition) == 0)
        FadeSubtitlePanels();
    if (GetLegacyPerspectiveProjection() - 1U > 2U && subtitleBackgrounds[1] && subtitleBackgrounds[0]) {
        subtitleBackgrounds[1]->field44 = 0;
        subtitleBackgrounds[0]->field44 = 0;
    }
    uiUpdateWithColour(*GetNativeUiContext(), g_uiElements[static_cast<std::size_t>(UIElement::Subtitles)].config.uiHandleIndex);
    return 0;
}

std::uint32_t LoadSubtitles() {
    return ReadUIDefinitionFile(UIElement::Subtitles, "/ui/gamesubtitles/GameSubtitles.ui", 0);
}

// @verified - p
std::int64_t BindSubtitles() {
    const std::uint32_t viewIndex = g_uiElements[static_cast<std::size_t>(UIElement::Subtitles)].config.uiHandleIndex;
    const d3t::UIViewSlot<kUIViewImageCapacity> *const view = GetNuklearElementByIdx(viewIndex);
    if (!view)
        return 1;
    for (std::size_t index = 0; index < subtitlePanels.size(); ++index)
        subtitlePanels[index] = UI_GetWindow(UI_FindViewWindow(viewIndex, SUBTITLE_PANEL_IDS[index]));
    for (std::size_t index = 0; index < subtitleBackgrounds.size(); ++index) {
        const std::int32_t widget = UI_FindViewImageWidget(viewIndex, SUBTITLE_BACKGROUND_IDS[index]);
        subtitleBackgrounds[index] = widget < 0 ? nullptr : &g_uiImageWidgets[static_cast<std::size_t>(widget)];
        if (subtitleBackgrounds[index])
            subtitleBackgrounds[index]->field44 = 0;
    }
    subtitleActivityTransition = UIAnimAllocSlot();
    for (std::size_t index = 0; index < subtitleAlphaBindings.size(); ++index)
        subtitleAlphaBindings[index] = UIWidget_Bind(UI_FindViewWindow(viewIndex, SUBTITLE_PANEL_IDS[index]), 0, 0, {0.0f, 0.0f}, {1.0f, 0.0f});
    subtitleVoiceColours = {0xffffffffU, 0xff000000U, 0x80000000U};
    std::size_t label = 0;
    for (std::int32_t index = 0; index < view->widgetCount; ++index) {
        if (view->widgetKinds[static_cast<std::size_t>(index)] != 15)
            continue;
        subtitleNativeLabels[2 - label % 3][label / 3] = view->widgetPoolIndices[static_cast<std::size_t>(index)];
        ++label;
    }
    for (std::size_t index = 0; index < view->variables.size(); ++index) {
        const d3t::UIViewVariable &variable = view->variables[index];
        if (variable.type == d3t::UIViewVariableType::Float4) {
            const std::array<float, 4> &colour = variable.value.float4;
            std::uint32_t packed = 0;
            for (std::size_t component = 0; component < colour.size(); ++component) {
                const std::int32_t value = static_cast<std::int32_t>(colour[component]);
                packed |= static_cast<std::uint32_t>(value < 0 ? 0 : value > 255 ? 255 : value) << (component * 8U);
            }
            subtitleNativeVoicePalettes[index] = packed;
        } else if (variable.type == d3t::UIViewVariableType::Float) {
            subtitleCinemaOffset = variable.value.floatValue / 1080.0f;
        }
    }
    return 1;
}

// @verified - p
std::int64_t ResetSubtitleBindings() {
    subtitlePanels.fill(nullptr);
    return 0;
}
} // namespace ui_detail

// @verified - p
std::uint32_t ProcessVoiceFlags(std::int32_t speakerMode) {
    // ProcessVoiceFlags preserves the four distinct resource palettes.
    const std::size_t palette = speakerMode >= 0 && speakerMode < 3 ? static_cast<std::size_t>(speakerMode) : 3;

    for (std::size_t layer = 0; layer < 3; ++layer)
        subtitleVoiceColours[layer] = subtitleNativeVoicePalettes[3 * palette + layer];
    return subtitleVoiceColours[2];
}

// @verified - p
d3t::UIWindow *ProcessSubtitleText(const char *rawMessage) {
    const char *source = rawMessage;
    char *target = subtitleScratch.data();

    if (std::strstr(source, "=@") || std::strstr(source, SUBTITLE_NEWLINE_MARKER)) {
        do {
            const char *marker = std::strstr(source, "=@");
            const char *newline = std::strstr(source, SUBTITLE_NEWLINE_MARKER);

            if (newline && (!marker || newline < marker))
                marker = newline;
            if (marker) {
                const std::size_t bytes = static_cast<std::size_t>(marker - source);
                std::memcpy(target, source, bytes);
                target += bytes;
                if (static_cast<unsigned char>(*marker) == 0xa1U)
                    *target++ = '\n';
                else {
                    target[0] = '.';
                    target[1] = '.';
                    target[2] = '.';
                    target += 3;
                }
                marker += 2;
            } else
                std::strcpy(target, source);
            source = marker;
        } while (source);
    } else
        std::strcpy(target, source);
    for (std::size_t index = 0; index < std::strlen(subtitleScratch.data()); ++index)
        if (subtitleScratch[index] == '|')
            subtitleScratch[index] = '"';
    std::memset(subtitleText.data(), 0, sizeof(subtitleText));
    const char *line = subtitleScratch.data();
    std::int32_t count = 0;
    float scale = 0.33f;
    if (!std::strstr(line, "\n"))
        std::strcpy(subtitleText[1].data(), line);
    else {
        do {
            if (count == 3)
                break;
            const char *newline = std::strstr(line, "\n");
            if (newline) {
                std::memcpy(subtitleText[count].data(), line, static_cast<std::size_t>(newline - line));
                ++newline;
            } else
                std::strcpy(subtitleText[count].data(), line);
            ++count;
            line = newline;
        } while (line);
        if (count == 2) {
            std::strcpy(subtitleText[2].data(), subtitleText[1].data());
            std::memset(subtitleText[1].data(), 0, SUBTITLE_LINE_BYTES);
            scale = 0.48f;
        }
    }
    if (subtitlePanels[2])
        subtitlePanels[2]->layout.height.scale = scale;
    if (subtitlePanels[4])
        subtitlePanels[4]->layout.height.scale = scale;
    return subtitlePanels[4];
}

bool BeginSubtitlePresentation(std::string_view token, std::string_view localizedText, std::int32_t language, std::uint32_t durationFrames) {
    if (token.empty() || token.size() > 12 || localizedText.empty()) {
        return false;
    }
    const std::string terminatedText(localizedText);
    ProcessSubtitleText(terminatedText.c_str());
    subtitlePresentation.lines = {std::string_view(subtitleText[0].data()), std::string_view(subtitleText[1].data()), std::string_view(subtitleText[2].data())};
    std::copy(token.begin(), token.end(), subtitlePresentation.token.begin());
    subtitlePresentation.text = localizedText;
    subtitlePresentation.language = language;
    subtitlePresentation.durationFrames = durationFrames;
    subtitlePresentation.remainingFrames = durationFrames;
    subtitlePresentation.active = true;
    ShowElementByIndex(UIElement::Subtitles);
    EnableElement(UIElement::Subtitles);
    return true;
}

bool TickSubtitlePresentation() {
    if (!subtitlePresentation.active)
        return false;
    if (subtitlePresentation.durationFrames == 0)
        return true;
    if (subtitlePresentation.remainingFrames > 0) {
        --subtitlePresentation.remainingFrames;
    }
    if (subtitlePresentation.remainingFrames == 0) {
        EndSubtitlePresentation();
        return false;
    }
    return true;
}

void EndSubtitlePresentation() {
    subtitlePresentation = {};
    DestroyElementByIndex(UIElement::Subtitles);
}

bool GetSubtitlePresentationState(SubtitlePresentationState &state) {
    state = subtitlePresentation;
    state.colours = subtitleVoiceColours;
    state.cinemaOffset = subtitleCinemaOffset;
    return state.active;
}
} // namespace shenmue::s2
