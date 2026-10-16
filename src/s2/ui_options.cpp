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

std::uint32_t LoadNativePrompt() {
    return ReadUIDefinitionFile(UIElement::YesNoPrompt, "/ui/YesNoPrompt/YesNoPrompt_new.ui", 1);
}
std::int64_t BindNativePrompt() {
    const std::uint32_t slot = g_uiElements[static_cast<std::size_t>(UIElement::YesNoPrompt)].config.uiHandleIndex;
    for (std::size_t index = 0; index < nativePrompt.windows.size(); ++index)
        nativePrompt.windows[index] = UI_GetWindow(UI_FindViewWindow(slot, UI_PROMPT_WINDOWS[index]));
    for (std::size_t index = 0; index < nativePrompt.labelImages.size(); ++index) {
        const std::int32_t image = UI_FindViewImageWidget(slot, UI_PROMPT_LABEL_IMAGES[index]);
        nativePrompt.labelImages[index] = image >= 0 ? &g_uiImageWidgets[image] : nullptr;
    }
    for (std::size_t index = 0; index < nativePrompt.sprites.size(); ++index)
        nativePrompt.sprites[index] = UI_FindViewIconWidget(slot, UI_PROMPT_SPRITES[index], 20);
    for (std::size_t index = 0; index < nativePrompt.images.size(); ++index)
        nativePrompt.images[index] = UI_FindViewImage(slot, UI_PROMPT_IMAGES[index]);
    for (std::size_t index = 0; index < 2; ++index) {
        if (!nativePrompt.windows[index])
            continue;
        nativePrompt.windows[index]->mouseCallbacks.entered = EnterNativePrompt;
        nativePrompt.windows[index]->mouseCallbacks.clicked = ClickNativePrompt;
    }
    nativePrompt.bound = true;
    nativePrompt.timer = UIAnimAllocSlot();
    return 1;
}
std::int64_t ResetNativePrompt() {
    nativePrompt.windows.fill(nullptr);
    nativePrompt.labelImages.fill(nullptr);
    nativePrompt.sprites.fill(nullptr);
    nativePrompt.bound = false;
    return 0;
}
void StyleNativePrompt() {
    const bool visible = !nativePrompt.blinking || (std::bit_cast<std::int32_t>(GetUITransitionValue(nativePrompt.blink)) & 1) == 0;
    const std::array<bool, 4> shown{nativePrompt.selected == 2 && visible, nativePrompt.selected != 2 || !visible, nativePrompt.selected == 1 && visible,
        nativePrompt.selected != 1 || !visible};
    for (std::size_t index = 0; index < shown.size(); ++index)
        if (nativePrompt.sprites[index])
            nativePrompt.sprites[index]->colour = (nativePrompt.sprites[index]->colour & 0xffffffU) | (shown[index] ? 0xff000000U : 0U);
}
std::int64_t TickNativePrompt() {
    if (!nativePrompt.active)
        return 0;
    nativePrompt.blink = UIAnimAllocSlot();
    nativePrompt.blinking = false;
    const std::int32_t language = GetLoadingScreenTextType(g_sceneTextLanguage);
    if (nativePrompt.labelImages[0])
        nativePrompt.labelImages[0]->image = nativePrompt.images[2 + language];
    if (nativePrompt.labelImages[1])
        nativePrompt.labelImages[1]->image = nativePrompt.images[9 + language];
    StyleNativePrompt();
    AicaExecuteWrapper(2);
    return 0;
}
std::int64_t DestroyNativePrompt() {
    if (nativePrompt.active) {
        nativePrompt.active = nativePrompt.blinking = nativePrompt.limited = false;
        ResetUITransition(nativePrompt.blink);
        RestorePreviousInformationBarMessage();
    }
    return 1;
}
void FinishNativePrompt(std::int32_t result) {
    const auto callback = nativePrompt.callback;
    DestroyElementByIndex(UIElement::YesNoPrompt);
    nativePrompt.callback = nullptr;
    if (callback)
        callback(result);
}
void EnterNativePrompt(float *, void *data) {
    if (nativePrompt.blinking)
        return;
    const std::int32_t selected = data == nativePrompt.windows[0] ? 2 : 1;
    if (nativePrompt.selected != selected)
        AicaExecuteWrapper(3);
    nativePrompt.selected = selected;
    StyleNativePrompt();
}

void ClickNativePrompt(float *bounds, void *data) {
    if (nativePrompt.blinking)
        return;
    EnterNativePrompt(bounds, data);
    nativePrompt.blinking = true;
    StartUIIntegerTransition(nativePrompt.blink, 0, 6, 0.75f, 0.0f);
    AicaExecuteWrapper(2);
}

bool PointInsideMenuPanel(const AuthoredMenuLayout &layout, std::int32_t panel) {
    if (panel < 0 || panel >= static_cast<std::int32_t>(layout.panels.size()))
        return false;
    const auto &bounds = layout.panels[panel];
    const float x = uiPointer.position.x * 1920.0f / GetUIViewportWidth();
    return x >= bounds.x && x < bounds.x + bounds.width && uiPointer.position.y >= bounds.y && uiPointer.position.y < bounds.y + bounds.height;
}
std::int64_t NativePromptInput() {
    if (nativePrompt.limited && GetUITransitionValue(nativePrompt.timer) <= 0.0f) {
        FinishNativePrompt(3);
        RestartUITransition(nativePrompt.timer);
    }
    const std::uint16_t pressed = g_inputBuffer[0].pressedPriority;
    if (pressed & 0xc200U) {
        if (!nativePrompt.blinking) {
            const std::int32_t previous = nativePrompt.selected;
            if (pressed & 0x8000U)
                nativePrompt.selected = 1;
            else if (pressed & 0x4000U)
                nativePrompt.selected = 2;
            if (previous != nativePrompt.selected)
                AicaExecuteWrapper(3);
            if (pressed & 0x200U) {
                AicaExecuteWrapper(2);
                FinishNativePrompt(4);
            }
        }
        StyleNativePrompt();
    }
    if (!nativePrompt.blinking) {
        if (pressed & 0x400U) {
            AicaExecuteWrapper(2);
            nativePrompt.blinking = true;
            StartUIIntegerTransition(nativePrompt.blink, 0, 6, 0.75f, 0.0f);
        }
    } else {
        if (std::bit_cast<std::int32_t>(GetUITransitionValue(nativePrompt.blink)) == 6)
            FinishNativePrompt(nativePrompt.selected);
        StyleNativePrompt();
    }
    return 0;
}
std::int64_t RenderNativePrompt() {
    if (!nativePrompt.bound)
        return 0;
    if (nativePrompt.active && nativePrompt.limited) {
        std::array<char, 512> caption{};
        const std::string_view token = nativePrompt.useString ? std::string_view{nativePrompt.text.data()} : GetLocalizedSceneText(nativePrompt.text.data());
        std::snprintf(caption.data(), caption.size(), "%.*s%i", static_cast<int>(token.size()), token.data(),
            static_cast<std::int32_t>(GetUITransitionValue(nativePrompt.timer)) + (nativePrompt.useString ? 0 : 1));
        UIMessageBox message = GetCurrentUIMessageBox();
        message.primaryText = caption.data();
        ShowTemporaryUIMessageBox(&message, -1.0f);
    }
    const UIElementState &element = g_uiElements[static_cast<std::size_t>(UIElement::YesNoPrompt)];
    DrawUIEntity(*GetNativeUiContext(), element.config.uiHandleIndex, element.index);
    return 0;
}
std::int64_t NativePromptHints() {
    return CopyHudHintIds(UI_PROMPT_HINTS);
}
void NativeMessageBox(std::string_view text, bool useString, std::int32_t clearAfter, void (*callback)(std::int32_t), float seconds) {
    if (!callback || nativePrompt.active)
        return;
    SetInformationBarVisibilityMode(0);
    std::snprintf(nativePrompt.text.data(), nativePrompt.text.size(), "%.*s", static_cast<int>(text.size()), text.data());
    nativePrompt.callback = callback;
    nativePrompt.selected = (clearAfter == 1) + 1;
    nativePrompt.active = true;
    nativePrompt.useString = useString;
    nativePrompt.limited = seconds > 0.0f;
    if (nativePrompt.limited)
        UIAnim_Start(nativePrompt.timer, seconds, 0.0f, seconds, 0.0f);
    UIMessageBox message{};
    message.primaryText = nativePrompt.text.data();
    message.primaryLocalized = useString ? 1 : 0;
    message.hideTarget = 1;
    ShowTemporaryUIMessageBox(&message, -1.0f);
    ShowElementByIndex(UIElement::GameInfoBar);
    ShowElementByIndex(UIElement::YesNoPrompt);
}

std::span<const OptionSpec> OptionSpecs(std::int32_t child) {
    if (child == 0)
        return controlsOptions;
    if (child == 1)
        return audioOptions;
    return graphicsOptions;
}
std::int32_t ReadOptionInteger(const OptionSpec &spec) {
    const auto *data = reinterpret_cast<const std::byte *>(&GetDraftUserConfiguration()) + spec.offset;
    if (spec.byte)
        return std::to_integer<std::uint8_t>(*data);
    std::int32_t value;
    std::memcpy(&value, data, sizeof(value));
    return value;
}
float ReadOptionFraction(const OptionSpec &spec) {
    float value;
    std::memcpy(&value, reinterpret_cast<const std::byte *>(&GetDraftUserConfiguration()) + spec.offset, sizeof(value));
    return value;
}
void WriteOptionInteger(const OptionSpec &spec, std::int32_t value) {
    auto *data = reinterpret_cast<std::byte *>(&GetDraftUserConfiguration()) + spec.offset;
    if (spec.byte)
        *data = static_cast<std::byte>(value);
    else
        std::memcpy(data, &value, sizeof(value));
}
void WriteOptionFraction(const OptionSpec &spec, float value) {
    std::memcpy(reinterpret_cast<std::byte *>(&GetDraftUserConfiguration()) + spec.offset, &value, sizeof(value));
}
std::string_view LocalizedOption(const char *token) {
    return GetLocalizedSceneText(token);
}
std::string_view OptionChoiceCaption(const OptionSpec &spec, std::int32_t index) {
    static constexpr std::array<const char*, 2> offOn{"UIOTGLOFF0", "UIOTGLON00"}; // @text UIOTGLOFF0 EN: "Off" | JA: "いいえ" ; UIOTGLON00 EN: "On" | JA: "はい"
    static constexpr std::array<const char*, 2> languages{"UIOTGLENG0", "UIOTGLJPN0"}; // @text UIOTGLENG0 EN: "English" | JA: "英語" ; UIOTGLJPN0 EN: "Japanese" | JA: "日本語"
    static constexpr std::array<const char*, 7> subtitles{"UIOTGLENG0", "UIOTGLJAP0", "UIOTGLFRN0", "UIOTGLGER0", "UIOTGLKOR0", "UIOTGLCHTR", "UIOTGLCHSU"}; // @text UIOTGLENG0 EN: "English" | JA: "英語" ; UIOTGLFRN0 EN: "French" | JA: "フランス語" ; UIOTGLGER0 EN: "German" | JA: "ドイツ語" ; UIOTGLKOR0 EN: "Korean" | JA: "韓国語" ; UIOTGLCHTR EN: "T. Chinese" | JA: "中国語（繁体字）" ; UIOTGLCHSU EN: "S. Chinese" | JA: "中国語（簡体字）"
    static constexpr std::array<const char*, 4> dialogue{"UIMODECINE", "UIMODESHEN", "UIMODEGAME", "UIMODETEXT"}; // @text UIMODECINE EN: "Cinema Mode" | JA: "シネマモード" ; UIMODESHEN EN: "Shenmue Mode" | JA: "シェンムーモード" ; UIMODEGAME EN: "Game Mode" | JA: "ゲームモード" ; UIMODETEXT EN: "Text Mode" | JA: "テキストモード"
    static constexpr std::array<const char*, 3> windowModes{"UIOTGLWIND", "UIOTGLBORD", "UIOTGLFULL"}; // @text UIOTGLWIND EN: "Windowed" | JA: "ウィンドウ" ; UIOTGLBORD EN: "Borderless" | JA: "ボーダーレス" ; UIOTGLFULL EN: "Fullscreen" | JA: "フルスクリーン"
    static constexpr std::array<const char*, 3> bloom{"UIOTGLOFF0", "UIOTGLLOW0", "UIOTGLHIGH"}; // @text UIOTGLOFF0 EN: "Off" | JA: "いいえ" ; UIOTGLLOW0 EN: "Low" | JA: "低" ; UIOTGLHIGH EN: "High" | JA: "高"
    static constexpr std::array<const char*, 2> aspect{"UIASPECT43", "UIASPECT169"}; // @text UIASPECT43 EN: "4:3" | JA: "4:3" ; UIASPECT169 EN: "16:9" | JA: "16:9"
    if (index < 0)
        return {};
    switch (spec.choices) {
    case OptionChoices::OffOn:
        if (spec.offset == offsetof(S2UserConfiguration280, invertLookY))
            return index < 2 ? LocalizedOption(index ? "UIACTSYES0" : "UIGNRLNO00") : std::string_view{}; // @text UIACTSYES0 EN: "Yes" | JA: "はい" ; UIGNRLNO00 EN: "No" | JA: "いいえ"
        return index < 2 ? LocalizedOption(offOn[index]) : std::string_view{};
    case OptionChoices::AudioLanguages:
        return index < 2 ? LocalizedOption(languages[index]) : std::string_view{};
    case OptionChoices::SubtitleLanguages: {
        const auto available = GetS2AvailableSubtitleLanguages();
        if (index == static_cast<std::int32_t>(available.size())) return LocalizedOption("UIOTGLDEF0"); // @text EN: "Default" | JA: "デフォルト"
        return index < static_cast<std::int32_t>(available.size()) ? LocalizedOption(subtitles[available[index]]) : std::string_view{};
    }
    case OptionChoices::Dialogue:
        return index < 4 ? LocalizedOption(dialogue[index]) : std::string_view{};
    case OptionChoices::WindowModes:
        return index < 3 ? LocalizedOption(windowModes[index]) : std::string_view{};
    case OptionChoices::Bloom:
        return index < 3 ? LocalizedOption(bloom[index]) : std::string_view{};
    case OptionChoices::Aspect:
        return index < 2 ? LocalizedOption(aspect[index]) : std::string_view{};
    case OptionChoices::AudioOutputs:
        return index < 10 ? GetS2AudioOutputName(index) : std::string_view{};
    case OptionChoices::Displays:
        return GetS2DisplayOutputName(index);
    case OptionChoices::Resolutions:
        return GetS2DisplayResolutionName(GetRuntimeUserConfiguration().displayOutput, index);
    default:
        return {};
    }
}
std::int32_t OptionChoiceValue(const OptionSpec &spec, std::int32_t index) {
    if (spec.choices != OptionChoices::SubtitleLanguages)
        return index;
    const auto available = GetS2AvailableSubtitleLanguages();
    return index < static_cast<std::int32_t>(available.size()) ? available[index] : -1;
}
std::int32_t OptionChoiceCount(const OptionSpec &spec) {
    switch (spec.choices) {
    case OptionChoices::OffOn:
    case OptionChoices::AudioLanguages:
    case OptionChoices::Aspect:
        return 2;
    case OptionChoices::Dialogue:
        return 4;
    case OptionChoices::WindowModes:
    case OptionChoices::Bloom:
        return 3;
    case OptionChoices::SubtitleLanguages:
        return static_cast<std::int32_t>(GetS2AvailableSubtitleLanguages().size()) + 1;
    default:
        break;
    }
    std::int32_t count = 0;
    while (!OptionChoiceCaption(spec, count).empty())
        ++count;
    return count;
}
std::int32_t SelectedOptionChoice(const OptionSpec &spec) {
    const auto current = ReadOptionInteger(spec);
    const auto count = OptionChoiceCount(spec);
    for (std::int32_t index = 0; index < count; ++index)
        if (OptionChoiceValue(spec, index) == current)
            return index;
    return 0;
}
const OptionSpec *CurrentOption() {
    if (options.current < 0 || options.current >= 3)
        return nullptr;
    const auto &child = optionChildren[options.current];
    const auto ordinal = options.editing >= 0 ? options.editing : GetUIGridSelection(child.grid);
    if (ordinal < 0 || ordinal >= child.count)
        return nullptr;
    return &OptionSpecs(options.current)[child.dictionary[ordinal]];
}
void DescribeOption(const OptionSpec &spec, const Vector4 *bounds = nullptr) {
    std::string_view description = LocalizedOption(spec.description);
    if (spec.choices == OptionChoices::AudioOutputs || spec.choices == OptionChoices::Displays)
        if (const auto caption = OptionChoiceCaption(spec, SelectedOptionChoice(spec)); !caption.empty())
            description = caption;
    if (spec.choices == OptionChoices::Dialogue) {
        static constexpr std::array<const char*, 4> tokens{"UIMODECINEDS", "UIMODESHENDS", "UIMODEGAMEDS", "UIMODETEXTDS"}; // @text UIMODECINEDS EN: "Voice ON, Text OFF.\nDialog skip disabled." | JA: "声だけでゲームを進行します。\n会話はスキップできません。" ; UIMODESHENDS EN: "Voice ON, Text ON.\nSubtitles appear only if dialog is skipped." | JA: "声と字幕でゲームを進行します。\nスキップ時、字幕が表示されます。" ; UIMODEGAMEDS EN: "Voice ON, Text ON." | JA: "声と字幕でゲームを進行します。" ; UIMODETEXTDS EN: "Voice OFF, Text ON." | JA: "字幕だけでゲームを進行します。"
        const auto index = std::clamp(ReadOptionInteger(spec), 0, 3);
        description = LocalizedOption(tokens[index]);
    }
    UIMessageBox message{};
    message.imageIndex = message.widgetIndex = message.imageFrame = -1;
    message.primaryText = description.data();
    message.primaryLocalized = 1;
    message.hideTarget = IsCameraIconHidden() == 0;
    if (bounds)
        message.bounds = *bounds;
    CopyUIMessageBox(&message);
}
void RefreshOptionChildren() {
    for (std::int32_t child = 0; child < 3; ++child) {
        auto &runtime = optionChildren[child];
        if (!runtime.bound)
            continue;
        runtime.count = 0;
        const auto specs = OptionSpecs(child);
        for (std::int32_t row = 0; row < static_cast<std::int32_t>(specs.size()); ++row) {
            if (child == 1 && (row == 1 || row == 2) && GetS2IsEnglishVersion())
                continue;
            runtime.dictionary[runtime.count++] = row;
        }
        SetUIGridDictionary(runtime.grid, runtime.count, runtime.dictionary.data());
    }
}
const std::int32_t *GetOptionRowData(UIGrid *grid, std::int32_t ordinal) {
    return grid ? grid->dictionary + ordinal : nullptr;
}
void SelectOptionRow(float *rectangle, void *data) {
    const std::int32_t *const row = static_cast<const std::int32_t *>(data);
    Vector4 bounds{};
    if (rectangle)
        std::memcpy(&bounds, rectangle, sizeof(bounds));
    if (options.current < 0 || options.current >= 3 || options.editing >= 0)
        return;
    selectedOptionDescription = row ? &OptionSpecs(options.current)[*row] : nullptr;
    if (selectedOptionDescription)
        DescribeOption(*selectedOptionDescription, rectangle ? &bounds : nullptr);
    else {
        UIMessageBox message{};
        message.imageIndex = message.widgetIndex = message.imageFrame = -1;
        message.hideTarget = IsCameraIconHidden() == 0;
        if (rectangle)
            message.bounds = bounds;
        CopyUIMessageBox(&message);
    }
}
void BeginOptionEdit() {
    const auto *spec = CurrentOption();
    if (!spec || spec->kind == OptionKind::Heading)
        return;
    options.editing = GetUIGridSelection(optionChildren[options.current].grid);
    if (spec->kind == OptionKind::Slider)
        options.savedFraction = ReadOptionFraction(*spec);
    else
        options.savedInteger = ReadOptionInteger(*spec);
    options.previousKey = 256;
    PreviewDraftUserConfiguration();
    ClearWindowInput();
    ResetInputHandler();
}
std::string_view FormatOptionKeyMessage(const char *token, std::int32_t key, const OptionSpec *action) {
    const auto text = LocalizedOption(token);
    std::array<char, 64> keyName{};
    if (key != -1)
        FormatUIInputKeyName(key, keyName);
    const auto actionName = action ? LocalizedOption(action->token) : std::string_view{};
    std::size_t written = 0;
    for (std::size_t index = 0; index < text.size() && written + 1 < options.keyQuestion.size();) {
        std::string_view replacement;
        std::size_t consumed = 1;
        if (key != -1 && text.substr(index).starts_with("[KEY_NAME]")) {
            replacement = keyName.data();
            consumed = 10;
        } else if (action && text.substr(index).starts_with("[INPUT_ACTION]")) {
            replacement = actionName;
            consumed = 14;
        } else
            replacement = text.substr(index, 1);
        const auto count = std::min(replacement.size(), options.keyQuestion.size() - written - 1);
        std::memcpy(options.keyQuestion.data() + written, replacement.data(), count);
        written += count;
        index += consumed;
    }
    options.keyQuestion[written] = 0;
    return {options.keyQuestion.data(), written};
}
void ShowOptionStatus(const char *token) {
    const auto *spec = CurrentOption();
    if (spec)
        DescribeOption(*spec);
    const auto text = FormatOptionKeyMessage(token, options.pendingKey, spec);
    UIMessageBox message{};
    message.imageIndex = message.widgetIndex = message.imageFrame = -1;
    message.primaryText = text.data();
    message.primaryLocalized = 1;
    message.hideTarget = 1;
    ShowTemporaryUIMessageBox(&message, 4.0f);
}
void BindCapturedOptionKey() {
    const auto *spec = CurrentOption();
    if (spec)
        WriteOptionInteger(*spec, options.pendingKey);
    PreviewDraftUserConfiguration();
    RefreshOptionChildren();
    ShowOptionStatus("UIKMAPBIND"); // @text EN: "The [[KEY_NAME]] key has been successfully bound to the [[INPUT_ACTION]] input slot." | JA: "[[KEY_NAME]]キーは[[INPUT_ACTION]]の入力スロットにアサインされました。"
    options.editing = -1;
}
void ConfirmDuplicateOptionKey(std::int32_t response) {
    if (response == 2) {
        GetDraftUserConfiguration().keyMappings[options.duplicate] = -1;
        BindCapturedOptionKey();
    } else
        options.editing = -1;
}
void CaptureOptionKey(const OptionSpec &spec) {
    UIMessageBox waiting{};
    waiting.imageIndex = waiting.widgetIndex = waiting.imageFrame = -1;
    waiting.primaryText = FormatOptionKeyMessage("UIKMAPANYK", -1, &spec).data(); // @text EN: "Press any key to bind it to the [[INPUT_ACTION]] input slot." | JA: "[[INPUT_ACTION]]の入力スロットにアサインするには、好きなキーを押してください。"
    waiting.primaryLocalized = 1;
    waiting.hideTarget = 1;
    CopyUIMessageBox(&waiting);
    const auto key = GetLastReleasedWindowKey();
    if (key == 256 || !HasWindowInputStateChanged() || key == options.previousKey)
        return;
    options.previousKey = key;
    const auto action = (spec.offset - offsetof(S2UserConfiguration280, keyMappings)) / sizeof(std::int32_t);
    options.pendingKey = static_cast<std::int32_t>(key);
    if (!IsWindowKeyBindable(static_cast<std::int32_t>(action), key)) {
        ShowOptionStatus("UIKMAPINVK"); options.editing = -1; return; // @text EN: "This key cannot be bound. Please try another." | JA: "このキーはアサインできません。他のキーを選択してください。"
    }
    const std::size_t first = action < 18 ? 0 : action < 22 ? 18 : action < 32 ? 22 : 32;
    const std::size_t last = action < 18 ? 18 : action < 22 ? 22 : action < 32 ? 32 : 50;
    const auto &mappings = GetDraftUserConfiguration().keyMappings;
    options.duplicate = -1;
    for (std::size_t index = first; index < last; ++index)
        if (index != action && mappings[index] == static_cast<std::int32_t>(key)) {
            options.duplicate = static_cast<std::int32_t>(index);
            break;
        }
    if (options.duplicate == -1) {
        BindCapturedOptionKey();
        return;
    }
    const OptionSpec *duplicateAction = nullptr;
    for (const auto &entry : OptionSpecs(options.current))
        if (entry.kind == OptionKind::Key && entry.offset == offsetof(S2UserConfiguration280, keyMappings) + options.duplicate * sizeof(std::int32_t))
            duplicateAction = &entry;
    NativeMessageBox(FormatOptionKeyMessage("UIKMAPALBO", options.pendingKey, duplicateAction).data(), // @text EN: "The [[KEY_NAME]] key is already bound to [[INPUT_ACTION]], do you want to unbind it?" | JA: "[[KEY_NAME]]キーはすでに[[INPUT_ACTION]]にアサインされています。解除しますか？"
        true, 1, ConfirmDuplicateOptionKey);
}
void ChangeOptionChoice(const OptionSpec &spec, std::int32_t direction) {
    const auto count = OptionChoiceCount(spec);
    if (!count)
        return;
    auto index = SelectedOptionChoice(spec) + direction;
    if (index < 0)
        index = count - 1;
    if (index >= count)
        index = 0;
    WriteOptionInteger(spec, OptionChoiceValue(spec, index));
    PreviewDraftUserConfiguration();
    AicaExecuteWrapper(3);
    DescribeOption(spec);
}

std::int64_t OptionChildInput() {
    if (options.current < 0 || options.current >= 3)
        return 1;
    if (options.tabFocus)
        return 1;
    auto &child = optionChildren[options.current];
    const auto *spec = CurrentOption();
    const auto pressed = g_inputBuffer[0].pressedPriority;
    if (!spec || spec->kind != OptionKind::Key || options.editing < 0) {
        if (pressed & 2U)
            ApplyOptions();
        if (pressed & 4U) NativeMessageBox("UIOPTSREVD", false, 1, ConfirmOptionsReset); // @text EN: "Reset settings to default values?" | JA: "設定をデフォルト値に戻しますか？"
    }
    if (options.editing < 0 && (pressed & 0x4000U)) {
        options.tabFocus = true;
        options.hoveredTab = options.current;
        return 0;
    }
    if (options.editing < 0) {
        if (pressed & 0x400U) {
            BeginOptionEdit();
            return 0;
        }
        ProcessMovesGridInput(child.grid, uiPointer, GetFrameTime());
        auto row = GetUIGridSelection(child.grid);
        if (row >= 0 && row < child.count && OptionSpecs(options.current)[child.dictionary[row]].kind == OptionKind::Heading) {
            const auto direction = (g_inputBuffer[0].repeatedPresses & 0x1000U) ? -1 : 1;
            child.grid.row = std::clamp(child.grid.row + direction, 0, child.count - 1);
        }
        return 1;
    }
    if (!spec) {
        options.editing = -1;
        return 0;
    }
    if (spec->kind == OptionKind::Key) {
        CaptureOptionKey(*spec);
        return 0;
    }
    if (pressed & 0x200U) {
        if (spec->kind == OptionKind::Slider)
            WriteOptionFraction(*spec, options.savedFraction);
        else
            WriteOptionInteger(*spec, options.savedInteger);
        options.editing = -1;
        PreviewDraftUserConfiguration();
        DescribeOption(*spec);
        return 0;
    }
    if (pressed & 0x400U) {
        options.editing = -1;
        return 0;
    }
    const auto repeated = g_inputBuffer[0].repeatedPresses | pressed;
    if (spec->kind == OptionKind::Toggle) {
        if (pressed & 0x8000U) {
            WriteOptionInteger(*spec, 1);
            PreviewDraftUserConfiguration();
            AicaExecuteWrapper(3);
        }
        if (pressed & 0x4000U) {
            WriteOptionInteger(*spec, 0);
            PreviewDraftUserConfiguration();
            AicaExecuteWrapper(3);
        }
    } else if (spec->kind == OptionKind::List) {
        if (pressed & 0x8000U)
            ChangeOptionChoice(*spec, 1);
        if (pressed & 0x4000U)
            ChangeOptionChoice(*spec, -1);
    } else if (spec->kind == OptionKind::Slider) {
        const float step = 1.0f / static_cast<float>(spec->maximum - spec->minimum);
        float fraction = ReadOptionFraction(*spec);
        if (repeated & 0x8000U)
            fraction += step;
        if (repeated & 0x4000U)
            fraction = fraction * static_cast<float>(spec->maximum - spec->minimum) - 1.0f < 1.0f ? 0.0f : fraction - step;
        WriteOptionFraction(*spec, std::clamp(fraction, 0.0f, 1.0f));
        PreviewDraftUserConfiguration();
    }
    return 0;
}

void DrawOptionsKeyIcon(const d3t::UITextureObjectDefinition &object, const d3t::UIResolvedTextureObject &placement, std::int32_t key, float opacity) {
    const auto &icons = uiDefinitions[static_cast<std::size_t>(UIElement::PlatformIcons)];
    const auto *image = icons.findImage("ButtonIcons");
    if (!image || icons.spriteSheets.empty())
        return;
    std::int32_t frame = 26;
    switch (key) {
    case -1:
        frame = 40;
        break;
    case 37:
        frame = 36;
        break;
    case 38:
        frame = 37;
        break;
    case 39:
        frame = 38;
        break;
    case 40:
        frame = 39;
        break;
    case 256:
        frame = 41;
        break;
    case 257:
        frame = 43;
        break;
    case 258:
        frame = 42;
        break;
    default:
        break;
    }
    const auto &crop = icons.spriteSheets[0].frames[frame];
    const d3t::UISharedSpriteImage shared{image->textureHandle, {crop.x, crop.y, crop.width, crop.height}};
    d3t::DrawUITextureObject(uiDefinitions[16], object, placement, opacity, &shared);
    if (frame == 26) {
        std::array<char, 64> caption{};
        if (!FormatUIInputKeyName(key, caption))
            return;
        options.keyIconLabel.colour = object.fontColour;
        DrawMovesLabel(options.keyIconLabel, {placement.bounds, placement.clip}, caption.data(), opacity);
    }
}

void DrawOptionRow(
    UIGrid *grid, void *, const Vector4 *outer, const Vector4 *bounds, std::int32_t, std::uint32_t, const std::int32_t *dictionary, std::int32_t ordinal) {
    if (!dictionary || options.current < 0 || options.current >= 3 || options.root < 0 || options.rowRoot < 0)
        return;
    const auto &spec = OptionSpecs(options.current)[dictionary[ordinal]];
    const auto kind = static_cast<std::int8_t>(spec.kind);
    auto &definition = uiDefinitions[16];
    auto &layout = options.layout;
    const float sx = 1920.0f / GetUIViewportWidth();
    const d3t::UIRect clip{outer->x * sx, outer->y, outer->z * sx, outer->w};
    const auto clipToGrid = [&clip](const d3t::UIRect &window) {
        const float x = std::max(window.x, clip.x), y = std::max(window.y, clip.y);
        const float right = std::min(window.x + window.width, clip.x + clip.width);
        const float bottom = std::min(window.y + window.height, clip.y + clip.height);
        return d3t::UIRect{x, y, std::max(0.0f, right - x), std::max(0.0f, bottom - y)};
    };
    const std::array<d3t::UIPanelBoundsOverride, 2> overrides{{
        {static_cast<std::size_t>(options.root), layout.panels[options.root]},
        {static_cast<std::size_t>(options.rowRoot), {(outer->x + bounds->x) * sx, outer->y + bounds->y, bounds->z * sx, bounds->w}}}};
    d3t::ResolveUIDefinitionLayout(definition, layout.parents, layout.panels, layout.objects, layout.labels, layout.objectPanels, layout.labelPanels,
        overrides, GetUILayoutPaddingScale());
    const auto matches = [&](std::int32_t panel) { return panel >= 0 && options.panelKinds[panel] == kind; };
    const bool selected = ordinal == GetUIGridSelection(*grid);
    const auto integer = spec.kind == OptionKind::Heading || spec.kind == OptionKind::Slider ? 0 : ReadOptionInteger(spec);
    for (std::size_t index = 0; index < definition.textureObjects.size(); ++index) {
        const auto panel = layout.objectPanels[index];
        if (!matches(panel))
            continue;
        auto placement = layout.objects[index];
        placement.clip = clipToGrid(placement.clip);
        const auto &id = definition.panels[panel].id;
        if (spec.kind == OptionKind::Slider && id == "SubMenu_Options::Option_ScrollBar_Button_#") {
            const auto track = options.sliderTrack;
            if (track >= 0)
                placement.bounds.x = layout.panels[track].x + (layout.panels[track].width - placement.bounds.width) * ReadOptionFraction(spec);
        }
        const bool hover = PointInsideMenuPanel(layout, panel);
        if (selected && spec.kind != OptionKind::Heading && !nativePrompt.active) {
            if (uiPointer.leftClicks) {
                if (id == "SubMenu_Options::Option_ToggleLeft_#" && hover) {
                    WriteOptionInteger(spec, 0);
                    PreviewDraftUserConfiguration();
                } else if (id == "SubMenu_Options::Option_ToggleRight_#" && hover) {
                    WriteOptionInteger(spec, 1);
                    PreviewDraftUserConfiguration();
                } else if (id == "SubMenu_Options::Option_List_Left_#" && hover)
                    ChangeOptionChoice(spec, -1);
                else if ((id == "SubMenu_Options::Option_List_Right_#" || id == "SubMenu_Options::Option_ListSelected_#") && hover)
                    ChangeOptionChoice(spec, 1);
                else if (id == "SubMenu_Options::KeyBinding_Button_#" && hover)
                    BeginOptionEdit();
            }
            if (spec.kind == OptionKind::Slider && id == "SubMenu_Options::Option_ScrollBar_Button_#") {
                const auto track = options.sliderTrack;
                auto &drag = options.sliderDrag[options.current][dictionary[ordinal]];
                if (uiPointer.leftDown && (hover || drag.active) && track >= 0) {
                    const float pointer = uiPointer.position.x * sx;
                    if (!drag.active) {
                        drag.previousPointer = pointer;
                        drag.active = true;
                    }
                    const float width = layout.panels[track].width - placement.bounds.width;
                    if (width > 0.0f)
                        WriteOptionFraction(spec, std::clamp(ReadOptionFraction(spec) + (pointer - drag.previousPointer) / width, 0.0f, 1.0f));
                    drag.previousPointer = pointer;
                    PreviewDraftUserConfiguration();
                } else if (!uiPointer.leftDown)
                    drag.active = false;
            }
        }
        if (!drawingUI)
            continue;
        d3t::UITextureObjectDefinition object = definition.textureObjects[index];
        float opacity = selected ? 1.0f : 0.6f;
        if (object.id.ends_with("Option_Overlay") || object.id == "SubMenu_Options::KeyBinding_Overlay") {
            object.colour = (object.colour & 0x00ffffffU) | ((options.editing == -1 || options.editing == ordinal) ? 0U : 0x99000000U);
            opacity = 1.0f;
        }
        if (object.id.find("_Game") != std::string::npos && IsCameraIconHidden() == 0)
            continue;
        if (object.id.find("_Front") != std::string::npos && IsCameraIconHidden() != 0)
            continue;
        if (object.id.ends_with("_Selected") && (IsCameraIconHidden() != 0 || !selected))
            continue;
        if (spec.kind == OptionKind::Toggle && object.id.find("SubBackground_FrontOn") != std::string::npos &&
            ((object.id.find("ToggleRight") != std::string::npos) != (integer == 1)))
            continue;
        if (spec.kind == OptionKind::Toggle && object.id.find("SubBackground_FrontOff") != std::string::npos &&
            ((object.id.find("ToggleRight") != std::string::npos) == (integer == 1)))
            continue;
        if (object.kind == d3t::UITextureObjectKind::ButtonIcon && spec.kind == OptionKind::Key)
            DrawOptionsKeyIcon(object, placement, integer, selected ? 1.0f : 0.6f);
        else
            d3t::DrawUITextureObject(definition, object, placement, opacity);
    }
    std::array<char, 64> value{};
    std::string_view caption;
    if (spec.kind == OptionKind::Slider) {
        std::snprintf(value.data(), value.size(), "%d%%",
            static_cast<std::int32_t>(static_cast<float>(spec.maximum - spec.minimum) * ReadOptionFraction(spec) + spec.minimum));
        caption = value.data();
    } else if (spec.kind == OptionKind::Key) {
        FormatUIInputKeyName(integer, value);
        caption = value.data();
    } else if (spec.kind != OptionKind::Heading)
        caption = OptionChoiceCaption(spec, SelectedOptionChoice(spec));
    for (std::size_t index = 0; index < definition.labels.size(); ++index) {
        if (!matches(layout.labelPanels[index]))
            continue;
        auto placement = layout.labels[index];
        placement.clip = clipToGrid(placement.clip);
        auto &label = definition.labels[index];
        std::string_view text = LocalizedOption(spec.token);
        if (label.id == "SubMenu_Options::ToggleRight_Label")
            text = OptionChoiceCaption(spec, 1);
        else if (label.id == "SubMenu_Options::ToggleLeft_Label")
            text = OptionChoiceCaption(spec, 0);
        else if (label.id == "SubMenu_Options::ListSelected_Label" || label.id == "SubMenu_Options::ScrollButton_Label")
            text = caption;
        if (drawingUI) {
            float opacity = selected ? 1.0f : 0.6f;
            if (spec.kind == OptionKind::Toggle && ((label.id == "SubMenu_Options::ToggleRight_Label" && integer == 0) ||
                    (label.id == "SubMenu_Options::ToggleLeft_Label" && integer == 1)))
                opacity *= 0.15f;
            DrawMovesLabel(label, placement, text, opacity);
        }
    }
}
std::uint32_t LoadSubMenuOptions() {
    return ReadUIDefinitionFile(UIElement::SubMenuOptions, "/ui/gamemenu_New/SubMenu_Options.ui", 1);
}
std::uint32_t LoadOptionsControls() {
    return ReadUIDefinitionFile(optionChildElements[0], "/ui/gamemenu_New/SubMenu_Options_Controls.ui", 1);
}
std::uint32_t LoadOptionsAudio() {
    return ReadUIDefinitionFile(optionChildElements[1], "/ui/gamemenu_New/SubMenu_Options_Audio.ui", 1);
}
std::uint32_t LoadOptionsGraphics() {
    return ReadUIDefinitionFile(optionChildElements[2], "/ui/gamemenu_New/SubMenu_Options_Graphics.ui", 1);
}
std::int64_t BindOptionChild(std::int32_t child) {
    auto &runtime = optionChildren[child];
    auto &definition = uiDefinitions[static_cast<std::size_t>(optionChildElements[child])];
    BindAuthoredMenuLayout(definition, runtime.layout);
    static constexpr std::array<const char *, 3> roots{"SubMenu_Controls::Panel_Root", "SubMenu_Audio::Panel_Root", "SubMenu_Graphics::Panel_Root"};
    runtime.root = MenuPanelIndex(definition, roots[child]);
    InitializeUIGrid(runtime.grid,
        {.columns = 1,
            .visibleRows = 8,
            .rows = static_cast<std::int32_t>(OptionSpecs(child).size()),
            .drawRow = DrawOptionRow,
            .actions = {SelectOptionRow, nullptr, nullptr, nullptr},
            .selectionData = GetOptionRowData,
            .sound = 3});
    runtime.bound = true;
    RefreshOptionChildren();
    return 1;
}
std::int64_t BindOptionsControls() {
    return BindOptionChild(0);
}
std::int64_t BindOptionsAudio() {
    return BindOptionChild(1);
}
std::int64_t BindOptionsGraphics() {
    return BindOptionChild(2);
}
std::int64_t ResetOptionChild(std::int32_t child) {
    optionChildren[child].bound = false;
    return 0;
}
std::int64_t ResetOptionsControls() {
    return ResetOptionChild(0);
}
std::int64_t ResetOptionsAudio() {
    return ResetOptionChild(1);
}
std::int64_t ResetOptionsGraphics() {
    return ResetOptionChild(2);
}
std::int64_t TickOptionChild(std::int32_t child) {
    options.editing = -1;
    RefreshOptionChildren();
    ClearWindowInput();
    ResetInputHandler();
    ResetUIGrid(optionChildren[child].grid);
    return 0;
}
std::int64_t TickOptionsControls() {
    return TickOptionChild(0);
}
std::int64_t TickOptionsAudio() {
    return TickOptionChild(1);
}
std::int64_t TickOptionsGraphics() {
    return TickOptionChild(2);
}
std::int64_t DestroyOptionChild() {
    return 1;
}
std::int64_t RenderOptionChild() {
    if (options.current == 0)
        currentUIState = HasUnboundUserMappings() ? static_cast<std::int32_t>(UIElement::OptionsControls) : -1;
    if (options.current < 0 || options.current >= 3)
        return 0;
    auto &runtime = optionChildren[options.current];
    if (!runtime.bound)
        return 0;
    const auto &definition = uiDefinitions[static_cast<std::size_t>(optionChildElements[options.current])];
    auto &layout = runtime.layout;
    std::array<d3t::UIPanelBoundsOverride, 1> overrides{};
    std::size_t count = 0;
    if (runtime.root >= 0) {
        const std::uint32_t slot = g_uiElements[static_cast<std::size_t>(optionChildElements[options.current])].config.uiHandleIndex;
        d3t::UIWindow *const root = UI_GetWindow(UI_FindViewWindow(slot, definition.panels[runtime.root].id.c_str()));
        if (root) {
            UILayoutRect bounds{};
            UI_GetWindowRect(root, bounds);
            const float scaleX = 1920.0f / GetUIViewportWidth();
            overrides[count++] = {static_cast<std::size_t>(runtime.root), {bounds.x * scaleX, bounds.y, bounds.width * scaleX, bounds.height}};
        }
    }
    d3t::ResolveUIDefinitionLayout(definition, layout.parents, layout.panels, layout.objects, layout.labels, layout.objectPanels, layout.labelPanels,
        std::span{overrides}.first(count), GetUILayoutPaddingScale());
    if (runtime.root >= 0) {
        const auto &bounds = layout.panels[runtime.root];
        runtime.grid.bounds = {bounds.x * GetUIViewportWidth() / 1920.0f, bounds.y, bounds.width * GetUIViewportWidth() / 1920.0f, bounds.height};
    }
    RenderUIGrid(runtime.grid, static_cast<std::uint32_t>(optionChildElements[options.current]), uiPointer);
    if (drawingUI)
        DrawUIGridScrollbar(runtime.grid, static_cast<std::uint32_t>(optionChildElements[options.current]), uiPointer);
    return 0;
}
std::int64_t BindOptions() {
    auto &definition = uiDefinitions[16];
    BindAuthoredMenuLayout(definition, options.layout);
    options.sliderTrack = MenuPanelIndex(definition, "SubMenu_Options::Option_ScrollBar_#");
    options.keyIconLabel.alignment = "Centre";
    options.keyIconLabel.fontSize = "H3";
    options.root = MenuPanelIndex(definition, "SubMenu_Options::Panel_Root");
    options.rowRoot = MenuPanelIndex(definition, "SubMenu_Options::OptionParent_#");
    static constexpr std::array<const char *, 5> roots{"SubMenu_Options::Option_Toggle_#", "SubMenu_Options::Option_List_#", "SubMenu_Options::Option_Scroll_#",
        "SubMenu_Options::Option_KeyBinding_#", "SubMenu_Options::Option_Title_#"};
    for (std::size_t kind = 0; kind < roots.size(); ++kind)
        options.kinds[kind] = MenuPanelIndex(definition, roots[kind]);
    options.panelKinds.assign(definition.panels.size(), -1);
    for (std::size_t panel = 0; panel < definition.panels.size(); ++panel) {
        auto ancestor = static_cast<std::int32_t>(panel);
        while (ancestor >= 0) {
            for (std::size_t kind = 0; kind < roots.size(); ++kind)
                if (ancestor == options.kinds[kind]) {
                    options.panelKinds[panel] = static_cast<std::int8_t>(kind);
                    break;
                }
            if (options.panelKinds[panel] >= 0)
                break;
            ancestor = options.layout.parents[ancestor];
        }
    }
    static constexpr std::array<const char *, 4> rowLabels{"SubMenu_Options::Option_Toggle_Label_#", "SubMenu_Options::Option_List_Label_#",
        "SubMenu_Options::Option_Scroll_Label_#", "SubMenu_Options::Option_KeyBinding_Label_#"};
    for (std::size_t kind = 0; kind < rowLabels.size(); ++kind) {
        const std::int32_t panel = MenuPanelIndex(definition, rowLabels[kind]);
        if (panel >= 0)
            options.panelKinds[panel] = static_cast<std::int8_t>(kind);
    }
    const std::int32_t keyPanel = MenuPanelIndex(definition, "SubMenu_Options::KeyBinding_Key_#");
    if (keyPanel >= 0)
        options.panelKinds[keyPanel] = static_cast<std::int8_t>(OptionKind::Key);
    for (std::size_t tab = 0; tab < options.tabs.size(); ++tab) {
        std::array<char, 48> id{};
        std::snprintf(id.data(), id.size(), "SubMenu_Options::Tab%uButton", static_cast<unsigned>(tab + 1));
        options.tabs[tab] = MenuPanelIndex(definition, id.data());
    }
    return 1;
}
std::int64_t ResetOptions() {
    options.tabs.fill(-1);
    return 0;
}
void ConfirmOptionsKeep(std::int32_t response) {
    if (response < 0 || (response > 1 && static_cast<std::uint32_t>(response - 3) > 1U)) {
        ProcessUserSettingsConfig();
        SnapshotUserConfiguration();
        AicaExecuteWrapper(9);
    } else {
        RollbackUserConfiguration();
        options.editing = -1;
        RefreshOptionChildren();
    }
}
void ApplyOptions() {
    if (HasUserConfigurationChanges() && !HasUnboundUserMappings()) {
        const bool scalingChanged = HasScalingSettingsModified();
        ApplyDraftUserConfiguration();
        if (scalingChanged) NativeMessageBox("UIOPTSKEEP", false, 1, ConfirmOptionsKeep, 15.0f); // @text EN: "Keep changes? Reverting in " | JA: "変更を適用しますか？元に戻るまであと： "
        else {
            ProcessUserSettingsConfig();
            SnapshotUserConfiguration();
            AicaExecuteWrapper(9);
        }
    }
    options.editing = -1;
}
void ConfirmUnsavedOptions(std::int32_t response) {
    if (response == 2)
        ApplyOptions();
    else {
        RollbackUserConfiguration();
        options.editing = -1;
        RefreshOptionChildren();
    }
}
bool AskKeepOrRevertChanges() {
    if (!g_uiElements[16].isVisible || !HasUserConfigurationChanges())
        return false;
    NativeMessageBox("UIOPTUNSAV", false, 1, ConfirmUnsavedOptions); // @text EN: "Do you wish to save your changes?" | JA: "変更した項目をセーブしますか？"
    return true;
}
void ConfirmOptionsReset(std::int32_t response) {
    if (response == 2) {
        if (options.current == 0)
            ResetDraftUserControls();
        else if (options.current == 1)
            ResetDraftUserAudio();
        else if (options.current == 2)
            ResetDraftUserGraphics();
        ApplyDraftUserConfiguration();
        RefreshOptionChildren();
    }
    options.editing = -1;
}
// @verified - p
void ConfirmOptionsQuit(std::int32_t response) {
    if (response == 2 && DestroyElementByIndex(UIElement::GameMenu) && !AskKeepOrRevertChanges()) {
        DestroyElementByIndex(UIElement::SubMenuOptions);
        DestroyElementByIndex(UIElement::GameBackground);
        DestroyElementByIndex(UIElement::GameInfoBar);
        options.editing = -1;
        ResetSleepActionIcon();
        ResetToTitleScreen();
    }
}
void SelectOptionsTab(std::int32_t next) {
    options.requested = next;
    if (next == 3) {
        options.requested = options.current;
        if (IsCameraIconHidden()) NativeMessageBox("UIQUITLAB2", false, 0, ConfirmOptionsQuit); // @text EN: "Are you sure you want to quit?\nAny unsaved progress will be lost." | JA: "タイトル画面に戻ります。よろしいですか？\n（記録されていないゲームデータは失われます）"
        return;
    }
    if (next < 0 || next >= 3 || next == options.current)
        return;
    if (options.current >= 0 && !DestroyElementByIndex(optionChildElements[options.current]))
        return;
    options.current = next;
    options.editing = -1;
    ShowElementByIndex(optionChildElements[next]);
    AicaExecuteWrapper(3);
}
std::int64_t TickOptions() {
    ShowElementByIndex(UIElement::GameInfoBar);
    ResetUIGridTabSelector();
    options.current = -1;
    options.closing = false;
    options.editing = -1;
    options.tabFocus = false;
    options.hoveredTab = 0;
    if (IsCameraIconHidden() == 1 && options.cameraRequested) {
        options.savedCamera = camCtrlGetState();
        camCtrlSetState(CameraState::Event);
    }
    SnapshotUserConfiguration();
    SelectOptionsTab(options.requested);
    return 0;
}
std::int64_t DestroyOptions() {
    if (HasUserConfigurationChanges())
        ProcessUserSettingsConfig();
    const auto result = options.current >= 0 ? DestroyElementByIndex(optionChildElements[options.current]) : 1;
    if (!result)
        options.closing = true;
    if (options.cameraRequested) {
        camCtrlSetState(options.savedCamera);
        options.cameraRequested = false;
    }
    return result;
}
std::int64_t OptionsMenuInput() {
    const auto pressed = g_inputBuffer[0].pressedPriority;
    if (options.closing) {
        DestroyElementByIndex(UIElement::SubMenuOptions);
        return 0;
    }
    const auto maximum = IsCameraIconHidden() ? 3 : 2;
    if (options.tabFocus) {
        const auto repeated = g_inputBuffer[0].repeatedPresses;
        if (repeated & 0x1000U) {
            g_inputBuffer[0].repeatedPresses &= ~0x1000U;
            if (--options.hoveredTab < 0)
                options.hoveredTab = maximum;
            AicaExecuteWrapper(3);
        }
        if (repeated & 0x2000U) {
            g_inputBuffer[0].repeatedPresses &= ~0x2000U;
            if (++options.hoveredTab > maximum)
                options.hoveredTab = 0;
            AicaExecuteWrapper(3);
        }
        if (pressed & 0x400U) {
            g_inputBuffer[0].pressedPriority &= ~0x400U;
            SelectOptionsTab(options.hoveredTab);
            options.tabFocus = false;
            AicaExecuteWrapper(2);
        }
        if (pressed & 0x8000U) {
            g_inputBuffer[0].pressedPriority &= ~0x8000U;
            options.tabFocus = false;
            AicaExecuteWrapper(3);
        }
    }
    for (std::int32_t tab = 0; tab <= maximum; ++tab)
        if (uiPointer.leftClicks && PointInsideMenuPanel(options.layout, options.tabs[tab])) {
            SelectOptionsTab(tab);
            options.tabFocus = false;
        }
    if (options.requested != options.current)
        SelectOptionsTab(options.requested);
    return 1;
}
std::int64_t RenderOptions() {
    if (!uiPointer.leftDown)
        for (auto &child : options.sliderDrag)
            for (auto &drag : child)
                drag.active = false;
    auto &definition = uiDefinitions[16];
    auto &layout = options.layout;
    std::array<d3t::UIPanelBoundsOverride, 1> overrides{};
    std::size_t count = 0;
    if (options.root >= 0) {
        const std::uint32_t slot = g_uiElements[static_cast<std::size_t>(UIElement::SubMenuOptions)].config.uiHandleIndex;
        d3t::UIWindow *const root = UI_GetWindow(UI_FindViewWindow(slot, definition.panels[options.root].id.c_str()));
        if (root) {
            std::snprintf(root->layout.parentId.data(), root->layout.parentId.size(), "%s",
                IsCameraIconHidden() ? "GameMenu::SubContentPanel" : "MainMenu::ContentPanel");
            UILayoutRect bounds{};
            UI_GetWindowRect(root, bounds);
            const float scaleX = 1920.0f / GetUIViewportWidth();
            overrides[count++] = {static_cast<std::size_t>(options.root), {bounds.x * scaleX, bounds.y, bounds.width * scaleX, bounds.height}};
        }
    }
    d3t::ResolveUIDefinitionLayout(definition, layout.parents, layout.panels, layout.objects, layout.labels, layout.objectPanels, layout.labelPanels,
        std::span{overrides}.first(count), GetUILayoutPaddingScale());
    if (!drawingUI)
        return 0;
    for (std::size_t index = 0; index < definition.textureObjects.size(); ++index) {
        const auto panel = layout.objectPanels[index];
        if (panel >= 0 && options.panelKinds[panel] >= 0)
            continue;
        const auto &id = definition.textureObjects[index].id;
        const bool game = IsCameraIconHidden() != 0;
        if (id.find("_Game") != std::string::npos && !game)
            continue;
        if (id.find("_Front") != std::string::npos && game)
            continue;
        if (!game && (id == "SubMenu_Options::Image_Tab_4" || id == "SubMenu_Options::Texture_Front_4" || id == "SubMenu_Options::Texture_Game_4"))
            continue;
        d3t::DrawUITextureObject(definition, definition.textureObjects[index], layout.objects[index], 1.0f);
    }
    for (std::size_t index = 0; index < definition.labels.size(); ++index) {
        const auto panel = layout.labelPanels[index];
        if (panel >= 0 && options.panelKinds[panel] >= 0)
            continue;
        if (!IsCameraIconHidden() && definition.labels[index].id == "SubMenu_Options::Label_Tab_4")
            continue;
        DrawMovesLabel(definition.labels[index], layout.labels[index],
            definition.labels[index].token.empty() ? std::string_view(definition.labels[index].text) : GetLocalizedSceneText(definition.labels[index].token),
            1.0f);
    }
    return 0;
}
void RegisterOptionsAndPrompt() {
    auto &root = g_uiElements[16];
    root.name = "SubMenu: Options";
    root.parseDefinitionFromFile = LoadSubMenuOptions;
    root.isSelected = BindOptions;
    root.isPressed = OptionsMenuInput;
    root.render = RenderOptions;
    root.reset = ResetOptions;
    root.tick = TickOptions;
    root.destroy = DestroyOptions;
    const std::array<std::uint32_t (*)(), 3> loaders{LoadOptionsControls, LoadOptionsAudio, LoadOptionsGraphics};
    const std::array<UIAction, 3> binders{BindOptionsControls, BindOptionsAudio, BindOptionsGraphics};
    const std::array<UIAction, 3> resets{ResetOptionsControls, ResetOptionsAudio, ResetOptionsGraphics};
    const std::array<UIAction, 3> ticks{TickOptionsControls, TickOptionsAudio, TickOptionsGraphics};
    for (std::size_t child = 0; child < 3; ++child) {
        auto &element = g_uiElements[static_cast<std::size_t>(optionChildElements[child])];
        element.name = "Options Section";
        element.parseDefinitionFromFile = loaders[child];
        element.isSelected = binders[child];
        element.isPressed = OptionChildInput;
        element.render = RenderOptionChild;
        element.reset = resets[child];
        element.tick = ticks[child];
        element.destroy = DestroyOptionChild;
    }
    auto &prompt = g_uiElements[58];
    prompt.name = "Yes/No Prompt";
    prompt.parseDefinitionFromFile = LoadNativePrompt;
    prompt.isSelected = BindNativePrompt;
    prompt.isPressed = NativePromptInput;
    prompt.render = RenderNativePrompt;
    prompt.reset = ResetNativePrompt;
    prompt.tick = TickNativePrompt;
    prompt.destroy = DestroyNativePrompt;
    prompt.unknown98 = NativePromptHints;
}

// @verified - p
void ReleaseEasyOptionTextures() {
    for (std::int16_t &texture : easyOptionDescriptionTextures) {
        if (texture != -1)
            ReleaseNamedTexture(texture);
        texture = -1;
    }
    for (std::int16_t &texture : easyOptionLabelTextures) {
        if (texture != -1)
            ReleaseNamedTexture(texture);
        texture = -1;
    }
}

// @verified - p
void LoadEasyOptionLabels() {
    for (std::uint32_t i = 0; i < easyOptionLabelTextures.size(); ++i) {
        const std::uint16_t width = static_cast<std::uint16_t>(EASY_OPTION_LABEL_WIDTHS[i] * 64);
        const shen_name textureName{make_shen_name('E', 'Z', '1', static_cast<char>('0' + i))};
        const std::span<const std::byte> ptx = SetupPtxTexture(width, EASY_OPTION_LABEL_TEXTURE.name, textureName.name);
        if (!ptx.empty()) {
            RasterizeItemLabelNullsub(const_cast<std::byte *>(ptx.data()), width, EASY_OPTION_LABELS[i], 0, 1);
            easyOptionLabelTextures[i] = static_cast<std::int16_t>(ReadTexnTexture(ptx.data()));
        }
    }
}

// @verified - p
void RefreshEasyOptionDescriptions() {
    for (std::int16_t &texture : easyOptionDescriptionTextures) {
        if (texture != -1)
            ReleaseNamedTexture(texture);
        texture = -1;
    }
    for (std::uint32_t i = 0; i < easyOptionDescriptionTextures.size(); ++i) {
        const char *text = "d3t_translation_fail";
        if (easyOptionSelectedRow != 0)
            text = EASY_OPTION_DESCRIPTIONS[2 * easyOptionSelectedRow + 2 + i];
        else {
            const std::int8_t description = EASY_OPTION_DIALOGUE_DESCRIPTIONS[easyOptionDraft[0]][i];
            if (description != -1)
                text = EASY_OPTION_DESCRIPTIONS[description];
        }
        const shen_name textureName{make_shen_name('E', 'Z', '1', static_cast<char>('0' + i))};
        const std::span<const std::byte> ptx = SetupPtxTexture(1024, EASY_OPTION_DESCRIPTION_TEXTURE.name, textureName.name);
        if (!ptx.empty()) {
            RasterizeItemLabelNullsub(const_cast<std::byte *>(ptx.data()), 1024, text, 0, 1);
            easyOptionDescriptionTextures[i] = static_cast<std::int16_t>(ReadTexnTexture(ptx.data()));
        }
    }
}

// @verified - p
void RefreshEasyOptionDialogueCaption() {
    easyOptionDescriptionCaption.first = 1;
    easyOptionDescriptionCaption.second = 1;
    easyOptionValueCaption.first = static_cast<std::int8_t>(EASY_OPTION_DIALOGUE_LABELS[easyOptionDraft[0]]);
    if (easyOptionSelectedRow == 0 && easyOptionDraft[0] != 1)
        easyOptionDescriptionCaption.second = -1;
}

// @verified - p
void DrawEasyOptionCaption(const EasyOptionCaption &caption, const Vector2 &origin) {
    easyOptionTextSprite.x = caption.x + origin.x;
    easyOptionTextSprite.y = (caption.y - (caption.first >= 0 && caption.second >= 0 ? 20 : 0)) + origin.y;
    for (std::int32_t i = 0; i < 2; ++i) {
        const std::int8_t item = i == 0 ? caption.first : caption.second;
        if (item >= 0) {
            easyOptionTextSprite.width = static_cast<std::int16_t>(EASY_OPTION_LABEL_WIDTHS[item] * 64);
            easyOptionTextSprite.icon = static_cast<std::uint16_t>(easyOptionLabelTextures[item]);
            if (static_cast<std::int16_t>(easyOptionTextSprite.icon) >= 0)
                PrepareDrawTextureWithMode(easyOptionTextSprite, ScreenMode::Centered);
            easyOptionTextSprite.y += 40.0f;
        }
    }
}

// @verified - p
void DrawEasyOptionBox(const EasyOptionCaption &caption, const Vector2 &origin) {
    const std::int32_t value = caption.condition == 0 || caption.condition == 1 ? 0 :
        caption.condition == 2                                                  ? static_cast<std::int8_t>(easyOptionSelectedRow) :
                                                                                  static_cast<std::int8_t>(easyOptionDraft[caption.condition - 3]);
    const bool selected = caption.condition == 0 || (caption.condition != 1 && value == caption.value);
    const std::uint32_t fill = selected ? (static_cast<std::uint32_t>(easyOptionPulse) << 16) | 0xbf000000U : 0xbf000020U;
    const std::uint32_t border = selected ? 0xff650000U : 0xff000065U;
    const std::array<std::int16_t, 2> &size = EASY_OPTION_BOX_SIZES[caption.size];
    shenSubmitQuadsUI(caption.x + origin.x, caption.y + origin.y, fill, border, size[0], size[1], 3.0f);
    DrawEasyOptionCaption(caption, origin);
}

void UpdateEasyOptionSelectionInput() noexcept {
    const LegacyInputSlot &input = *ctrlGetPadInputBuffer0(easyOptionPadControl);
    std::uint16_t repeated = input.repeatedPresses;
    const std::uint8_t previousRow = easyOptionSelectedRow;
    const std::uint8_t previousValue = easyOptionPreviousValue;
    std::uint8_t *value = &easyOptionDraft[easyOptionSelectedRow];
    easyOptionPreviousValue = *value;
    if (easyOptionSelectedRow != 0)
        repeated &= *value != 0 ? 0x7fffU : 0xbfffU;
    if (input.pressedPriority & 0x0200U) {
        easyOptionControllerState = 4;
        ExecuteAicaCommand(0x900ab, 0, 0);
    } else {
        if (repeated & 0x3000U) {
            if (repeated & 0x1000U) {
                if (easyOptionSelectedRow != 0)
                    --easyOptionSelectedRow;
            } else if (easyOptionSelectedRow < 2)
                ++easyOptionSelectedRow;
            value = &easyOptionDraft[easyOptionSelectedRow];
            easyOptionPreviousValue = *value & (easyOptionSelectedRow == 0 ? 3U : 1U);
        } else if (repeated & 0xc000U) {
            if (repeated & 0x4000U)
                --easyOptionPreviousValue;
            if (repeated & 0x8000U)
                ++easyOptionPreviousValue;
            easyOptionPreviousValue &= easyOptionSelectedRow == 0 ? 3U : 1U;
        }
        *value = easyOptionPreviousValue;
    }
    if (previousRow != easyOptionSelectedRow || previousValue != easyOptionPreviousValue) {
        RefreshEasyOptionDialogueCaption();
        RefreshEasyOptionDescriptions();
    }
}

void UpdateEasyOptionController() noexcept {
    if ((g_loadingFlag & 2U) != 0)
        return;
    switch (easyOptionControllerState) {
    case 1:
        ShowElementByIndex(UIElement::Options);
        if (IsMagicRoomLoadingModels())
            break;
        easyOptionPosition = -640.0f;
        easyOptionVelocity = 65.0f;
        easyOptionDraft[0] = static_cast<std::uint8_t>(GetS2DialogueMode());
        easyOptionDraft[1] = static_cast<std::uint8_t>(GetS2ControlMode());
        easyOptionDraft[2] = AreS2TriggersSwapped() ? 1U : 0U;
        easyOptionPreviousValue = easyOptionDraft[easyOptionSelectedRow];
        easyOptionControllerState = 2;
        easyOptionShowsOptions = true;
        if (easyOptionPadControl && (easyOptionPadControl->flags & 2U) == 0) {
            easyOptionPreviousPadControl = BindPadControlEntry(easyOptionPadControl);
        }
        scenCvsCtrlFree();
        RefreshEasyOptionDialogueCaption();
        ReleaseEasyOptionTextures();
        LoadEasyOptionLabels();
        RefreshEasyOptionDescriptions();
        if (!easyOptionMode) {
            easyOptionSavedHelpVisible = static_cast<std::uint16_t>(GetUiFlag(0));
            easyOptionSavedHelpSelected = static_cast<std::uint8_t>(GetUiFlag(1));
            CheckHelpIcon(33, 0, CheckHelpIconOp::Set);
            easyOptionPreviousClockSuppression = IsClockAndCompassSuppressed();
            SetRenderClockAndCompassTo(1);
        }
        break;
    case 2:
        if (AdvanceIconPosition(easyOptionPosition, easyOptionVelocity, 130.0f))
            easyOptionControllerState = 3;
        break;
    case 3:
        UpdateEasyOptionSelectionInput();
        set_icon_select_flags_to(0x0200U);
        break;
    case 4:
        if (IsMagicRoomLoadingModels())
            break;
        ApplyS2DialogueMode(easyOptionDraft[0]);
        ApplyS2ControlMode(easyOptionDraft[1]);
        ApplyS2TriggerSwapMode(easyOptionDraft[2]);
        easyOptionControllerState = 5;
        break;
    case 5:
        DestroyElementByIndex(UIElement::Options);
        if (RetreatIconPosition(easyOptionPosition, easyOptionVelocity, -640.0f)) {
            easyOptionControllerState = 0;
            easyOptionShowsOptions = false;
            if (easyOptionPreviousPadControl && easyOptionPadControl && (easyOptionPadControl->flags & 2U) != 0) {
                BindPadControlEntry(easyOptionPreviousPadControl);
            }
            ReleaseEasyOptionTextures();
            if (!easyOptionMode) {
                CheckHelpIcon(easyOptionSavedHelpVisible, 0, CheckHelpIconOp::Set);
                SetRenderClockAndCompassTo(easyOptionPreviousClockSuppression ? 1 : 0);
            }
            icsToggleIconState(7, 0);
            EV_NPC_NewLoadSw(easyOptionPreviousNpcState);
        }
        break;
    case 6:
        easyOptionControllerState = 7;
        break;
    case 7:
        easyOptionControllerState = 1;
        break;
    default:
        break;
    }
    if (easyOptionShowsOptions) {
        EnableElement(UIElement::Options);
    }
}
void StepDebugTaskCallback(void *) {
    UpdateStepDebugCounters();
}

void EasyOptionTaskCallback(void *) {
    UpdateEasyOptionController();
}

void InitEasyOptionTaskCallback(void *) {
    easyOptionPreviousPadControl = nullptr;
    easyOptionPadControl = AllocatePadControlEntry();
    easyOptionPosition = -50.0f;
    easyOptionVelocity = 100.0f;
    easyOptionPulseStep = 2;
    for (std::int16_t &texture : easyOptionLabelTextures)
        texture = -1;
    easyOptionPulse = 32;
    easyOptionSelectedRow = 0;
    easyOptionMode = 0;
    easyOptionControllerState = 0;
    easyOptionShowsOptions = false;
    for (std::int16_t &texture : easyOptionDescriptionTextures)
        texture = -1;
    easyOptionTextSprite.x = -50.0f;
    easyOptionTextSprite.flags = 0;
    easyOptionTextSprite.rotation = 0;
    easyOptionTextSprite.width = 256;
    easyOptionTextSprite.height = 32;
    easyOptionTextSprite.scaleX = 1.0f;
    easyOptionTextSprite.scaleY = 1.0f;
    easyOptionTextSprite.depth = 5.0f;
    easyOptionTextSprite.alignment = 5;
    InitializeLegacyIconSprite(easyOptionButtonSprite);
    easyOptionButtonSprite.depth = 5.0f;
    HLib::SetCurrentTaskTo(EasyOptionTaskCallback);
}
} // namespace ui_detail

void RefreshOptionsLanguage() {
    if (selectedOptionDescription) {
        const Vector4 bounds = GetCurrentUIMessageBox().bounds;
        DescribeOption(*selectedOptionDescription, &bounds);
    }
}

bool IsEasyOptionActive() noexcept {
    return easyOptionControllerState != 0;
}

void SetNewEasyOptionState(std::int32_t mode) noexcept {
    easyOptionControllerState = 6;
    easyOptionMode = static_cast<std::int8_t>(mode);
    easyOptionPreviousNpcState = static_cast<std::uint32_t>(GetNpcNewLoadState());
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    icsToggleIconState(7, 1);
}

bool AbortEasyOptionState(std::int32_t requested) noexcept {
    if (!requested || (GetCurrentItemSelectState() & 0x80U) == 0)
        return false;
    easyOptionControllerState = 0;
    easyOptionShowsOptions = false;
    ReleaseEasyOptionTextures();
    icsToggleIconState(7, 0);
    EV_NPC_NewLoadSw(easyOptionPreviousNpcState);
    if (easyOptionPreviousPadControl && (easyOptionPadControl->flags & 2U))
        BindPadControlEntry(easyOptionPreviousPadControl);
    SetRenderClockAndCompassTo(easyOptionPreviousClockSuppression ? 1 : 0);
    return true;
}

std::uint8_t GetEasyOptionValue(std::uint32_t row) noexcept {
    if (row >= easyOptionDraft.size())
        return 0;
    if (IsEasyOptionActive())
        return easyOptionDraft[row];
    if (row == 0)
        return static_cast<std::uint8_t>(GetS2DialogueMode());
    if (row == 1)
        return static_cast<std::uint8_t>(GetS2ControlMode());
    return AreS2TriggersSwapped() ? 1U : 0U;
}
std::uint32_t GetEasyOptionSelectedRow() noexcept {
    return easyOptionSelectedRow;
}

HLib::Task *QueueEasyOptionTask() {
    for (auto &preview : posePreviewRecords) {
        preview.source = preview.model = nullptr;
        preview.state = 0;
        std::memcpy(preview.characterName.data(), "NULL", 5);
    }
    InitializeHandWork();
    easyOptionTask = HLib::EnqueueTaskWithoutParameter(InitEasyOptionTaskCallback, 0x0e, 0, TASK_ESOP.name);
    HLib::EnqueueTaskWithoutParameter(StepDebugTaskCallback, 0x0c, 0, static_cast<std::int32_t>(TASK_UPDB.name));
    UpdateStepDebugCounters();
    return easyOptionTask;
}
} // namespace shenmue::s2
