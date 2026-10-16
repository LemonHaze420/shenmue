#include "s2/ui.hpp"
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

void UpdateSubtitleLabelGeometry() {
    auto &definition = uiDefinitions[static_cast<std::size_t>(UIElement::Subtitles)];
    constexpr std::array<std::string_view, 3> rows{
        "Sub_TopLine_Panel",
        "Sub_MidLine_Panel",
        "Sub_BotLine_Panel",
    };
    for (auto &panel : definition.panels) {
        if (std::find(rows.begin(), rows.end(), panel.id) != rows.end())
            panel.scaleY = subtitlePresentation.lineScaleY;
    }
    subtitleLabelRectangles = d3t::GetUILabelRectangles(definition);
}

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

void DrawSubtitleLabels() {
    if (!subtitlePresentation.active)
        return;
    const auto &definition = uiDefinitions[static_cast<std::size_t>(UIElement::Subtitles)];
    const float scaleX = static_cast<float>(d3t::g_originalScreenWidth) / 1920.0f;
    const float scaleY = static_cast<float>(d3t::g_originalScreenHeight) / 1080.0f;
    const std::size_t count = std::min({definition.labels.size(), subtitleLabelRectangles.size(), std::size_t{9}});
    for (std::size_t index = 0; index < count; ++index) {
        const auto &label = definition.labels[index];
        const auto *panel = definition.findPanel(label.panelId);
        const auto text = subtitlePresentation.lines[index / 3];
        if (!panel || text.empty())
            continue;
        const auto &resolved = subtitleLabelRectangles[index];
        const d3t::UIRect bounds{resolved.bounds.x * scaleX, resolved.bounds.y * scaleY, resolved.bounds.width * scaleX, resolved.bounds.height * scaleY};
        const std::array<std::int32_t, 4> scissor{
            std::max(0, static_cast<int>(resolved.clip.x * scaleX)),
            std::max(0, static_cast<int>(resolved.clip.y * scaleY)),
            std::min(static_cast<int>(d3t::g_originalScreenWidth), static_cast<int>((resolved.clip.x + resolved.clip.width) * scaleX)),
            std::min(static_cast<int>(d3t::g_originalScreenHeight), static_cast<int>((resolved.clip.y + resolved.clip.height) * scaleY)),
        };
        // RenderSubtitles publishes shadow/stroke/text in this
        // source order: image 0 throughout, family 1 for the first two layers.
        const UIFontSelection font{uiFontLanguage, index % 3 == 2 ? 0U : 1U, 0};
        const auto fit = FitSubtitleLabel(font, text, bounds, panel->width * label.width / 1920.0f, panel->height * label.height / 1080.0f, true);
        if (!(fit.height > 0.0f))
            continue;
        auto drawLine = [&](std::string_view line, d3t::UIRect rectangle) {
            const float width = GetUIFontTextWidth(font, fit.height, line);
            float x = rectangle.x;
            if (label.alignment == "Centre")
                x += std::max(0.0f, (rectangle.width - width) * 0.5f);
            else if (label.alignment == "Right")
                x += std::max(0.0f, rectangle.width - width);
            DrawUIFontText(font, fit.height, line, x, rectangle.y + (rectangle.height - fit.height) * 0.5f, subtitleVoiceColours[2 - index % 3], scissor);
        };
        if (fit.split == std::string_view::npos) {
            drawLine(text, bounds);
        } else {
            auto half = bounds;
            half.height *= 0.5f;
            drawLine(text.substr(0, fit.split), half);
            half.y += half.height;
            drawLine(text.substr(fit.split + 1), half);
        }
    }
}

void LoadSubtitleStyles() {
    // LoadSubtitleStyles consumes the first twenty UI variables in source
    // order. JsonCpp's member iterator is name-ordered, not file-ordered.
    const auto &bytes = uiDefinitionData[static_cast<std::size_t>(UIElement::Subtitles)];
    if (bytes.empty())
        return;
    Json::CharReaderBuilder builder;
    builder["collectComments"] = false;
    builder["rejectDupKeys"] = false;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    Json::Value root;
    std::string errors;
    const char *const first = reinterpret_cast<const char *>(bytes.data());
    if (!reader->parse(first, first + bytes.size() - 1, &root, &errors))
        return;
    UpdateSubtitleLabelGeometry();
    const Json::Value &variables = root["Variables"];
    if (!variables.isObject() || variables.size() > 20)
        return;
    std::array<const Json::Value *, 20> ordered{};
    std::size_t count = 0;
    for (const Json::Value &variable : variables)
        ordered[count++] = &variable;
    std::sort(ordered.begin(), ordered.begin() + count,
        [](const Json::Value *left, const Json::Value *right) { return left->getOffsetStart() < right->getOffsetStart(); });
    subtitleVoiceColours = {0xffffffffU, 0xff000000U, 0x80000000U};
    for (std::size_t index = 0; index < count; ++index) {
        const Json::Value &variable = *ordered[index];
        const Json::Value &value = variable["Value"];
        const std::string type = variable.get("Type", "").asString();
        if (type == "float4" && index < 12 && value["0"].isNumeric() && value["1"].isNumeric() && value["2"].isNumeric() && value["3"].isNumeric()) {
            const auto component = [&value](const char *channel) {
                return static_cast<std::uint32_t>(std::clamp(static_cast<std::int32_t>(value[channel].asFloat()), 0, 255));
            };
            subtitleVoicePalettes[index / 3][index % 3] = (component("3") << 24U) | (component("2") << 16U) | (component("1") << 8U) | component("0");
        } else if (type == "float" && value.isNumeric()) {
            subtitleCinemaOffset = value.asFloat() / 1080.0f;
        }
    }
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
        subtitleNativeLabels[label % 3][label / 3] = view->widgetPoolIndices[static_cast<std::size_t>(index)];
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

std::uint32_t ProcessVoiceFlags(std::int32_t speakerMode) {
    // ProcessVoiceFlags preserves the four distinct resource palettes.
    const std::size_t palette = speakerMode >= 0 && speakerMode < 3 ? static_cast<std::size_t>(speakerMode) : 3;
    subtitleVoiceColours = subtitleVoicePalettes[palette];
    return subtitleVoiceColours[2];
}

bool ProcessSubtitleText(std::string_view rawMessage) {


    static std::array<char, 0x600> text{};
    std::size_t length = 0;
    for (std::size_t index = 0; index < rawMessage.size() && rawMessage[index] != '\0'; ++index) {
        const unsigned char value = static_cast<unsigned char>(rawMessage[index]);
        if (index + 1 < rawMessage.size() && value == '=' && rawMessage[index + 1] == '@') {
            if (length + 3 >= text.size()) {
                EndSubtitlePresentation();
                return false;
            }
            text[length++] = '.';
            text[length++] = '.';
            text[length++] = '.';
            ++index;
        } else {
            if (length + 1 >= text.size()) {
                EndSubtitlePresentation();
                return false;
            }
            if (value == 0xa1 && index + 1 < rawMessage.size() && static_cast<unsigned char>(rawMessage[index + 1]) == 0xf5) {
                text[length++] = '\n';
                ++index;
            } else {
                text[length++] = value == '|' ? '"' : static_cast<char>(value);
            }
        }
    }
    text[length] = '\0';
    subtitlePresentation = {};
    subtitlePresentation.text = {text.data(), length};
    const std::string_view message = subtitlePresentation.text;
    if (message.find('\n') == std::string_view::npos) {
        subtitlePresentation.lines[1] = message.substr(0, 0x1ff);
    } else {
        std::size_t cursor = 0;
        std::size_t count = 0;
        while (count < subtitlePresentation.lines.size()) {
            const std::size_t end = message.find('\n', cursor);
            const std::size_t lineLength = end == std::string_view::npos ? message.size() - cursor : end - cursor;
            subtitlePresentation.lines[count++] = message.substr(cursor, std::min(lineLength, std::size_t{0x1ff}));
            if (end == std::string_view::npos)
                break;
            cursor = end + 1;
        }
        if (count == 2) {
            subtitlePresentation.lines[2] = subtitlePresentation.lines[1];
            subtitlePresentation.lines[1] = {};
            subtitlePresentation.lineScaleY = 0.48f;
        }
    }
    subtitlePresentation.active = true;
    UpdateSubtitleLabelGeometry();
    ShowElementByIndex(UIElement::Subtitles);
    EnableElement(UIElement::Subtitles);
    return true;
}

bool BeginSubtitlePresentation(std::string_view token, std::string_view localizedText, std::int32_t language, std::uint32_t durationFrames) {
    if (token.empty() || token.size() > 12 || localizedText.empty()) {
        return false;
    }
    if (!ProcessSubtitleText(localizedText))
        return false;
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
