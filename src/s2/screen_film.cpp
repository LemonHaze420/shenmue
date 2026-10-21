#include "s2/screen_film.hpp"

#include "s2/legacy_draw.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/map_control.hpp"
#include "s2/pak.hpp"
#include "s2/input.hpp"
#include "s2/save.hpp"
#include "s2/ui.hpp"

#include <bit>
#include <charconv>
#include <cstdio>
#include <cstring>
#include <limits>
#include <string_view>
#if defined(_M_X64) || defined(__x86_64__)
#include <emmintrin.h>
#endif

namespace shenmue::s2 {
// @verified - p
void filmSetFadeColourTo(HLib::Task *task, std::uint32_t colour) {
    ScreenFilmTaskData *const data = static_cast<ScreenFilmTaskData *>(HLib::GetTaskParameter(task));
    data->colourPair.pending = 1;
    data->colourPair.colour = colour;
    HLib::ResolveDeferredCallback(task);
}

float g_configuredGlareIntensity = 1.0f;
float g_frameGlareIntensity = 1.0f;
GlowParameters cachedGlowParameters{};
CrossParameters cachedCrossParameters{};
namespace {

constexpr shen_name kScreenFilmTaskName = SHEN_NAME(SCFT);
constexpr shen_name kFilmToken = SHEN_NAME(FILM);
constexpr float kFixedColourUnit = 65536.0F;
constexpr float kOpaqueAlpha = 255.0F * kFixedColourUnit;

std::uint32_t savedFilmTargetActive{};
std::array<std::uint8_t, 4> savedFilmTarget{};

PostProcessingState postProcessing{};
std::array<std::uint32_t, 4> postFxBankValues{};
float cachedBlurStrength{};

std::array<float, 2> defaultFilterContrast{0.0f, 11.526273727416992f};
std::array<float, 5> defaultUserFilter{1.0f, 0.025255000218749046f, -0.0065100002102553844f, 7.396069049835205f, 37.19606018066406f};
std::array<float, 5> defaultSepiaFilter{1.0f, 0.10999999940395355f, -0.028999999165534973f, 0.0f, 28.17799949645996f};
CrossParameters defaultFilterCross = INITIAL_POST_FX_CROSS;
std::array<float, 2> filterContrast{};
std::array<float, 5> userFilter{};
std::array<float, 5> sepiaFilter{};
CrossParameters filterCross{};
std::array<std::array<float, 7>, 3> filterPresets{};
std::int32_t filterDispatchEnabled{};
std::int32_t filterShaderType{1};
std::int32_t filterFadeBusy{};
float filterFadeAlpha{};
void (*activePostFxRenderCallback)(){};

std::string_view NextParameterToken(std::string_view &remaining, std::string_view delimiters) {
    const std::size_t begin = remaining.find_first_not_of(delimiters);
    if (begin == std::string_view::npos || remaining[begin] == '\0') {
        remaining = {};
        return {};
    }
    remaining.remove_prefix(begin);
    std::size_t end = remaining.find_first_of(delimiters);
    const std::size_t terminator = remaining.find('\0');
    if (terminator < end)
        end = terminator;
    const std::string_view result = remaining.substr(0, end);
    if (end == std::string_view::npos || remaining[end] == '\0')
        remaining = {};
    else
        remaining.remove_prefix(end + 1);
    return result;
}

bool ReadParameterFloat(std::string_view &remaining, std::string_view delimiters, float &value) {
    std::string_view text = NextParameterToken(remaining, delimiters);
    if (text.starts_with('+'))
        text.remove_prefix(1);
    double parsed{};
    const auto result = std::from_chars(text.data(), text.data() + text.size(), parsed);
    if (result.ec != std::errc{})
        return false;
    value = static_cast<float>(parsed);
    return true;
}

bool ReadParameterInteger(std::string_view &remaining, std::string_view delimiters, std::int32_t &value) {
    std::string_view text = NextParameterToken(remaining, delimiters);
    if (text.starts_with('+'))
        text.remove_prefix(1);
    return std::from_chars(text.data(), text.data() + text.size(), value).ec == std::errc{};
}

bool ReadCrossParameters(std::string_view &remaining, std::string_view delimiters, CrossParameters &parameters) {
    for (float &value : parameters.values)
        if (!ReadParameterFloat(remaining, delimiters, value))
            return false;
    for (std::int32_t &value : parameters.integerParameters)
        if (!ReadParameterInteger(remaining, delimiters, value))
            return false;
    for (auto &streak : parameters.streaks)
        for (float &value : streak)
            if (!ReadParameterFloat(remaining, delimiters, value))
                return false;
    return true;
}

std::int64_t CopyPostEffectShaderParameters(std::int32_t pass, std::uint32_t slot, const PostEffectShaderPacket &packet) {
    const std::size_t index = slot + 2 * static_cast<std::size_t>(pass);
    if (postProcessing.writeGates[index] > 0)
        return slot;
    postProcessing.packets[index] = packet;
    std::uint32_t lastWord;
    std::memcpy(&lastWord, packet.parameters.data() + 0xf4, sizeof(lastWord));
    return lastWord;
}

std::int64_t SetPostProcessingEnabled(std::int32_t pass, std::int32_t enabled) {
    postProcessing.enabled[static_cast<std::size_t>(pass)] = enabled;
    return pass;
}

std::int64_t UploadFilterBypassParameters() {
    PostEffectShaderPacket packet{};
    packet.kind = 7;
    return CopyPostEffectShaderParameters(1, 1, packet);
}

void UploadFilterContrastParameters() {
    PostEffectShaderPacket packet{};
    packet.kind = 3;
    std::memcpy(packet.parameters.data(), filterContrast.data(), sizeof(filterContrast));
    CopyPostEffectShaderParameters(1, 1, packet);
}

void UploadUserFilterParameters() {
    PostEffectShaderPacket packet{};
    packet.kind = 2;
    std::memcpy(packet.parameters.data(), userFilter.data(), sizeof(userFilter));
    CopyPostEffectShaderParameters(1, 1, packet);
}

void UploadSepiaFilterParameters() {
    PostEffectShaderPacket packet{};
    packet.kind = 2;
    std::memcpy(packet.parameters.data(), sepiaFilter.data(), sizeof(sepiaFilter));
    CopyPostEffectShaderParameters(1, 1, packet);
}

void UploadCrossFilterParameters() {
    PostEffectShaderPacket packet{};
    packet.kind = 5;
    std::memcpy(packet.parameters.data(), &filterCross, sizeof(filterCross));
    CopyPostEffectShaderParameters(0, 0, packet);
    ++postProcessing.writeGates[0];
    UploadFilterContrastParameters();
}

void UploadPresetFilterParameters(std::int32_t preset) {
    const auto &parameters = filterPresets[static_cast<std::size_t>(preset)];
    PostEffectShaderPacket packet{};
    packet.kind = 6;
    std::memcpy(packet.parameters.data(), parameters.data(), sizeof(parameters));
    CopyPostEffectShaderParameters(1, 1, packet);
}

void RestoreScenePostFxParameters() {
    if (postProcessing.writeGates[0] > 0)
        --postProcessing.writeGates[0];
    if (GetPostFxBankValue(1) == 1)
        UploadGlowParams(cachedGlowParameters);
    else if (GetPostFxBankValue(2) == 1)
        UploadCrossParams(cachedCrossParameters);
}

void RenderPostFxFilterFadeCallback() {
    filterFadeAlpha += 0.075000003f;
    if (filterFadeAlpha >= 1.0f) {
        activePostFxRenderCallback = nullptr;
        filterFadeBusy = 0;
    }
    RenderLegacyPostFxTransition(filterFadeAlpha);
}

void SetGameScreenFade(void *taskData) {
    const ScreenFilmState *const state = static_cast<const ScreenFilmState *>(taskData);
    SetScreenFilmColour(state->overlayColour);
    if (!g_uiElements[static_cast<std::size_t>(UIElement::ShenmueCollection)].isVisible)
        EnableElement(UIElement::GameScreenFade);
}

void ApplyCurrentScreenFilmColour(void *taskData) {
    const ScreenFilmColourPair *const colour = static_cast<const ScreenFilmColourPair *>(taskData);
    LegacyColourQuad packet{.quad = {.flags = 0}, .colour = (colour->colour << 24U) | 0x00ffffffU};
    AdjustScreenAspectRatio(packet.quad, static_cast<ScreenAlignment>(0), 0, 0, 640, 480, ScreenMode::Adjusted);
    packet.quad.flags |= 0x400U;
    shenSubmitQuad(packet, 0.0f);
}

bool advanceFilmState(ScreenFilmState &state) {
    if (state.remainingFrames != 0) {
        state.remainingFrames = std::bit_cast<std::int32_t>(std::bit_cast<std::uint32_t>(state.remainingFrames) - 1U);
    }
    if (state.remainingFrames != 0) {
        state.alpha += state.alphaStep;
        state.red += state.redStep;
        state.green += state.greenStep;
        state.blue += state.blueStep;
    } else {
        state.alpha = state.targetAlpha;
        state.red = state.targetRed;
        state.green = state.targetGreen;
        state.blue = state.targetBlue;
    }

    if (state.alpha == 0.0F)
        return state.remainingFrames != 0;

    const std::int32_t alpha = TruncateSpriteCoordinate(state.alpha) >> 16;
    const std::uint8_t displayAlpha =
        alpha <= 200 ? static_cast<std::uint8_t>(TruncateSpriteCoordinate(static_cast<float>(alpha) * 1.275f)) : std::uint8_t{255};
    if (state.maximumTime == static_cast<std::uint32_t>(state.remainingFrames)) {
        state.overlayColour = 0xff000000U;
    } else {
        state.overlayColour = (static_cast<std::uint32_t>(displayAlpha) << 24U) |
            (static_cast<std::uint32_t>(static_cast<std::uint8_t>(TruncateSpriteCoordinate(state.red) >> 16)) << 16U) |
            (static_cast<std::uint32_t>(static_cast<std::uint8_t>(TruncateSpriteCoordinate(state.green) >> 16)) << 8U) |
            static_cast<std::uint8_t>(TruncateSpriteCoordinate(state.blue) >> 16);
    }
    // HLib::ExecuteCallbackNow invokes this synchronously. Its separate
    // PAUS replay list is not the task post-update callback queue.
    HLib::ExecuteCallbackNow(SetGameScreenFade, &state);
    return state.remainingFrames != 0;
}

void ScreenFilmTask_Callback(void *taskData) {
    ScreenFilmTaskData *const data = static_cast<ScreenFilmTaskData *>(taskData);

    ScreenFilmState &state = GetLegacyRenderMask() && g_authoredFilmTrackSelection != -1 ? data->alternateState : data->state;
    advanceFilmState(state);
    if (data->colourPair.pending != 0) {
        HLib::ExecuteCallbackNow(ApplyCurrentScreenFilmColour, &data->colourPair);
        data->colourPair.pending = 0;
    }
}

} // namespace

std::int32_t g_authoredFilmTrackSelection = -1;

void ResetTaskWaitFlags() {
    savedFilmTargetActive = false;
    savedFilmTarget = {255, 0, 0, 0};
}

// @verified - p
std::uint64_t GetIsWaitingFlag() {
    return savedFilmTargetActive;
}


std::uint32_t GetFadedClearColour() {
    return static_cast<std::uint32_t>(savedFilmTarget[0]) << 24 | static_cast<std::uint32_t>(savedFilmTarget[1]) << 16 |
        static_cast<std::uint32_t>(savedFilmTarget[2]) << 8 | savedFilmTarget[3];
}

HLib::Task *Queue_SCF_Callback_ScreenFilmTaskEntry(std::uint8_t nextFunctionIndex) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(ScreenFilmTask_Callback, nextFunctionIndex, 2, sizeof(ScreenFilmTaskData), kFilmToken);
    ScreenFilmTaskData *const data = static_cast<ScreenFilmTaskData *>(HLib::GetTaskParameter(task));
    *data = {};
    data->name = kScreenFilmTaskName;
    ToggleCinemaBars(0);
    DestroyElementByIndex(UIElement::CinemaBars);
    HLib::SetupTimedTask(task, 0);
    return task;
}


// @verified - p
std::int64_t GetScreenFilmAlpha(HLib::Task *filmTask) {
    const float alpha = static_cast<const ScreenFilmTaskData *>(HLib::GetTaskParameter(filmTask))->state.alpha;
#if defined(_M_X64) || defined(__x86_64__)
    return _mm_cvttss_si64(_mm_set_ss(alpha));
#else
    return alpha >= -0x1p63f && alpha < 0x1p63f ? static_cast<std::int64_t>(alpha) : std::numeric_limits<std::int64_t>::min();
#endif
}

bool IsScreenFilmNotShowing(const HLib::Task *filmTask) {
    const ScreenFilmTaskData *const data = static_cast<const ScreenFilmTaskData *>(HLib::GetTaskParameter(const_cast<HLib::Task *>(filmTask)));
    return data->state.disabled != 0 || data->state.alpha == 0.0F;
}

bool DoesScreenFilmColourExist(const HLib::Task *filmTask) {
    const ScreenFilmTaskData *const data = static_cast<const ScreenFilmTaskData *>(HLib::GetTaskParameter(const_cast<HLib::Task *>(filmTask)));
    return data->state.disabled == 0 && data->state.remainingFrames != 0;
}

// @verified - p
bool filmCheckFadeTrackColour() {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return DoesScreenFilmColourExist(control->screenFilmTask);
}

// @verified - p
bool EV_GetScreenFilmIsNotShowing() {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return IsScreenFilmNotShowing(control->screenFilmTask);
}

// @verified - p
void SetScreenFilmFade(std::uint32_t frames, std::uint8_t startAlpha, std::uint8_t startRed, std::uint8_t startGreen, std::uint8_t startBlue,
    std::uint8_t targetAlpha, std::uint8_t targetRed, std::uint8_t targetGreen, std::uint8_t targetBlue) {
    const ScreenFilmColourRequest request{.start = {startAlpha, startRed, startGreen, startBlue},
        .target = {targetAlpha, targetRed, targetGreen, targetBlue},
        .durationFrames = std::bit_cast<std::int32_t>(frames)};
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    WaitForNextTask(control->screenFilmTask, request, 0);
}

void WaitForNextTask(HLib::Task *filmTask, const ScreenFilmColourRequest &request, std::int32_t mode) {
    ScreenFilmTaskData *const data = static_cast<ScreenFilmTaskData *>(HLib::GetTaskParameter(filmTask));
    data->value = 0;
    ScreenFilmState &state = mode == 1 ? data->alternateState : data->state;
    if (mode == 1)
        data->alternateActive = 1;
    if (mode == 3 && savedFilmTargetActive) {
        state.alpha = static_cast<float>(savedFilmTarget[0]) * kFixedColourUnit;
        state.red = static_cast<float>(savedFilmTarget[1]) * kFixedColourUnit;
        state.green = static_cast<float>(savedFilmTarget[2]) * kFixedColourUnit;
        // WaitForNextTask repeats the retained red byte for blue.
        state.blue = static_cast<float>(savedFilmTarget[1]) * kFixedColourUnit;
    } else {
        state.alpha = static_cast<float>(request.start[0]) * kFixedColourUnit;
        state.red = static_cast<float>(request.start[1]) * kFixedColourUnit;
        state.green = static_cast<float>(request.start[2]) * kFixedColourUnit;
        state.blue = static_cast<float>(request.start[3]) * kFixedColourUnit;
    }
    state.targetAlpha = static_cast<float>(request.target[0]) * kFixedColourUnit;
    state.targetRed = static_cast<float>(request.target[1]) * kFixedColourUnit;
    state.targetGreen = static_cast<float>(request.target[2]) * kFixedColourUnit;
    state.targetBlue = static_cast<float>(request.target[3]) * kFixedColourUnit;
    savedFilmTargetActive = mode == 2;
    if (savedFilmTargetActive)
        savedFilmTarget = request.target;

    state.disabled = request.durationFrames < 0;
    const std::uint32_t durationBits = std::bit_cast<std::uint32_t>(request.durationFrames);
    const std::uint32_t duration = state.disabled ? 0U - durationBits : durationBits;
    state.remainingFrames = std::bit_cast<std::int32_t>(duration);
    if (state.remainingFrames != 0) {
        const float divisor = static_cast<float>(state.remainingFrames);
        state.alphaStep = (state.targetAlpha - state.alpha) / divisor;
        state.redStep = (state.targetRed - state.red) / divisor;
        state.greenStep = (state.targetGreen - state.green) / divisor;
        state.blueStep = (state.targetBlue - state.blue) / divisor;
    } else {
        state.alphaStep = 0.0F;
        state.redStep = 0.0F;
        state.greenStep = 0.0F;
        state.blueStep = 0.0F;
    }
    state.maximumTime = 0xffffffffU;
    if (state.red == 0.0F && state.green == 0.0F && state.blue == 0.0F && state.targetRed == 0.0F && state.targetGreen == 0.0F && state.targetBlue == 0.0F) {
        if (state.alpha == kOpaqueAlpha && state.targetAlpha == 0.0F)
            state.maximumTime = duration - 1U;
        else if (state.alpha == 0.0F && state.targetAlpha == kOpaqueAlpha)
            state.maximumTime = 1;
    }
    HLib::ResolveDeferredCallback(filmTask);
}

void WaitForNextTask_0(HLib::Task *filmTask, std::int32_t mode, std::uint32_t durationFrames) {
    ScreenFilmColourRequest request{};
    request.durationFrames = std::bit_cast<std::int32_t>(durationFrames);
    switch (mode) {
    case 0:
        request.start[0] = 255;
        break;
    case 2:
        request.start = {255, 255, 255, 255};
        request.target = {0, 255, 255, 255};
        break;
    case 3:
        request.start = {0, 255, 255, 255};
        request.target = {255, 255, 255, 255};
        break;
    default:
        request.target[0] = 255;
        break;
    }
    WaitForNextTask(filmTask, request, 0);
}

void filmSetCinemaBarVisibility(ScreenFilmState *, FilmControlFlag mode) {
    const bool show = (static_cast<std::uint32_t>(mode) & 1U) != 0;
    ToggleCinemaBars(show ? 1 : 0);
    if (show) {
        ShowElementByIndex(UIElement::CinemaBars);
    } else {
        DestroyElementByIndex(UIElement::CinemaBars);
    }
}

void StartScreenFilmFade(HLib::Task *filmTask, std::uint32_t durationFrames, std::uint8_t startAlpha, std::uint8_t targetAlpha) {
    ScreenFilmColourRequest request{};
    request.start[0] = startAlpha;
    request.target[0] = targetAlpha;
    request.durationFrames = std::bit_cast<std::int32_t>(durationFrames);
    WaitForNextTask(filmTask, request, 0);
}

// @verified - p
void SetItemPreviewGlareIntensity(std::int32_t enabled) noexcept {
    g_frameGlareIntensity = enabled ? 0.5f : g_configuredGlareIntensity;
}
const PostProcessingState &GetPostProcessingState() noexcept {
    return postProcessing;
}

void InitializePostProcessingState() {
    postProcessing = {};

    InitializeLegacyPostProcessingParameters();
}

// @verified - p
std::int64_t InitializePostFxModuleDefaults() {
    for (std::int32_t &gate : postProcessing.writeGates)
        gate = 0;
    UploadGlowParams(INITIAL_POST_FX_GLOW);
    UploadCrossParams(defaultFilterCross);
    g_configuredGlareIntensity = 0.8f;
    PostEffectShaderPacket packet{.kind = 4};
    const float intensity = 0.8f;
    std::memcpy(packet.parameters.data(), &intensity, sizeof(intensity));
    CopyPostEffectShaderParameters(1, 0, packet);
    SelectPostFxBank(1, 0);
    SelectPostFxBank(2, 0);
    return SelectPostFxBank(3, 0);
}

std::int64_t GetPostFxBankValue(std::int32_t bank) noexcept {
    return postFxBankValues[static_cast<std::size_t>(bank)];
}

std::int64_t SelectPostFxBank(std::int32_t bank, std::int32_t value) noexcept {
    postFxBankValues[static_cast<std::size_t>(bank)] = static_cast<std::uint32_t>(value);
    if (bank > 0 && bank <= 2)
        return SetPostProcessingEnabled(0, value == 1);
    if (bank == 3) {
        PostEffectShaderPacket packet{};
        if (value != 0) {
            packet.kind = 4;
            std::memcpy(packet.parameters.data(), &cachedBlurStrength, sizeof(cachedBlurStrength));
        } else {
            packet.kind = 7;
        }
        return CopyPostEffectShaderParameters(1, 0, packet);
    }
    return bank;
}

std::int64_t UploadGlowParams(const GlowParameters &parameters) {
    cachedGlowParameters = parameters;
    PostEffectShaderPacket packet{};
    packet.kind = 1;
    std::memcpy(packet.parameters.data(), &parameters, sizeof(parameters));
    return CopyPostEffectShaderParameters(0, 0, packet);
}

std::int64_t UploadCrossParams(const CrossParameters &parameters) {
    cachedCrossParameters = parameters;
    PostEffectShaderPacket packet{};
    packet.kind = 5;
    std::memcpy(packet.parameters.data(), &parameters, sizeof(parameters));
    return CopyPostEffectShaderParameters(0, 0, packet);
}

std::int64_t UploadBlurStrength(float strength) {
    cachedBlurStrength = strength;
    PostEffectShaderPacket packet{};
    packet.kind = 4;
    std::memcpy(packet.parameters.data(), &strength, sizeof(strength));
    return CopyPostEffectShaderParameters(1, 0, packet);
}

bool SFIL_UploadBloomParams(std::int32_t afternoon) {
    char directory[32];
    char filename[32];
    const std::uint32_t map = g_currentMap.name;
    std::snprintf(directory, sizeof(directory), "scene/%02d/%c%c%c%c", g_actNumber, static_cast<char>(map), static_cast<char>(map >> 8),
        static_cast<char>(map >> 16), static_cast<char>(map >> 24));
    std::snprintf(filename, sizeof(filename), "SFIL%c%c%c%c.txt", static_cast<char>(map), static_cast<char>(map >> 8), static_cast<char>(map >> 16),
        static_cast<char>(map >> 24));
    void *allocation;
    std::uint32_t size;
    if (!(allocation = LoadLegacySceneFile(directory, filename, SCREEN_FILM_PARAMETER_FILE, &size))) {
        return false;
    }
    std::string_view remaining{static_cast<const char *>(allocation), size};
    constexpr std::string_view delimiters = ",=\n\r\t ";
    const auto nextToken = [&remaining, delimiters]() { return NextParameterToken(remaining, delimiters); };
    const auto readFloat = [&remaining, delimiters](float &value) { return ReadParameterFloat(remaining, delimiters, value); };
    const auto readInteger = [&remaining, delimiters](std::int32_t &value) { return ReadParameterInteger(remaining, delimiters, value); };
    bool complete = true;
    for (std::string_view label = nextToken(); !label.empty(); label = nextToken()) {
        if (label == (afternoon ? "@GLOW2" : "@GLOW1")) {
            GlowParameters parameters;
            complete = readInteger(parameters.index) && readFloat(parameters.intensity);
            for (float &value : parameters.values)
                complete = complete && readFloat(value);
            if (!complete)
                break;
            SelectPostFxBank(1, 1);
            UploadGlowParams(parameters);
        } else if (label == (afternoon ? "@CROSS2" : "@CROSS1")) {
            CrossParameters parameters;
            complete = ReadCrossParameters(remaining, delimiters, parameters);
            if (!complete)
                break;
            SelectPostFxBank(2, 1);
            UploadCrossParams(parameters);
        } else if (label == (afternoon ? "@BLUR2" : "@BLUR1")) {
            float strength;
            complete = readFloat(strength);
            if (!complete)
                break;
            SelectPostFxBank(3, 1);
            UploadBlurStrength(strength);
        }
    }
    HLib::FreeHeapSpace(allocation, static_cast<std::int32_t>(SCREEN_FILM_PARAMETER_FILE.name));
    return complete;
}

void InitializePostFxFilterState() {
    filterContrast = defaultFilterContrast;
    userFilter = defaultUserFilter;
    sepiaFilter = defaultSepiaFilter;
    filterCross = defaultFilterCross;
    filterFadeAlpha = 0.0f;
    filterFadeBusy = 0;
    SetSnapshotDataOption(0);
    filterShaderType = 1;
    if (filterDispatchEnabled != 0) {
        filterDispatchEnabled = 0;
        UploadFilterBypassParameters();
    }
    UploadFilterContrastParameters();
    InitializeLegacyPostFxTransitionTexture();
}

bool LoadPostFxFilterFile() {
    constexpr std::string_view delimiters = ",=\n\r\tf ";
    void *allocation;
    std::uint32_t size;
    if (!(allocation = LoadLegacySceneFile("misc", "USERFILT.txt", USER_FILTER_PARAMETER_FILE, &size)))
        return false;
    std::string_view remaining{static_cast<const char *>(allocation), size};
    bool complete = NextParameterToken(remaining, delimiters) == "01";
    std::size_t preset = 0;
    while (complete) {
        const std::string_view label = NextParameterToken(remaining, delimiters);
        if (label.empty())
            break;
        if (label == "CONTRAST") {
            std::array<float, 2> values;
            for (float &value : values)
                complete = complete && ReadParameterFloat(remaining, delimiters, value);
            if (complete)
                defaultFilterContrast = filterContrast = values;
        } else if (label == "GRAYSCALE" || label == "SEPIA") {
            std::array<float, 5> values;
            for (float &value : values)
                complete = complete && ReadParameterFloat(remaining, delimiters, value);
            if (complete) {
                if (label == "GRAYSCALE")
                    defaultUserFilter = userFilter = values;
                else
                    defaultSepiaFilter = sepiaFilter = values;
            }
        } else if (label == "CROSS") {
            CrossParameters values;
            complete = ReadCrossParameters(remaining, delimiters, values);
            if (complete)
                defaultFilterCross = filterCross = values;
        } else if (label == "CAF" && preset < filterPresets.size()) {
            std::array<float, 7> values;
            for (float &value : values)
                complete = complete && ReadParameterFloat(remaining, delimiters, value);
            if (complete)
                filterPresets[preset++] = values;
        }
    }
    HLib::FreeHeapSpace(allocation, static_cast<std::int32_t>(USER_FILTER_PARAMETER_FILE.name));
    return complete;
}

std::optional<std::int64_t> DispatchPostFxFilterRender(std::int32_t enabled) {
    if (filterDispatchEnabled == enabled)
        return std::nullopt;
    filterDispatchEnabled = enabled;
    if (enabled != 0) {
        SetPostProcessingEnabled(1, 1);
        SetPostFxFilterMode(filterShaderType);
        return std::nullopt;
    }
    if (filterShaderType == 4)
        RestoreScenePostFxParameters();
    return UploadFilterBypassParameters();
}

std::int32_t GetPostFxFilterMode() noexcept {
    return filterShaderType;
}

void SetPostFxFilterMode(std::int32_t mode) {
    if (filterDispatchEnabled != 0) {
        if (filterShaderType != mode) {
            filterFadeAlpha = 0.40000001f;
            filterFadeBusy = 1;
            activePostFxRenderCallback = RenderPostFxFilterFadeCallback;
        }
        if (filterShaderType == 4)
            RestoreScenePostFxParameters();
        filterShaderType = mode;
        switch (mode) {
        case 0:
            UploadFilterBypassParameters();
            break;
        case 1:
            UploadFilterContrastParameters();
            break;
        case 2:
            UploadUserFilterParameters();
            break;
        case 3:
            UploadSepiaFilterParameters();
            break;
        case 4:
            UploadCrossFilterParameters();
            break;
        case 5:
            UploadPresetFilterParameters(0);
            break;
        case 6:
            UploadPresetFilterParameters(1);
            break;
        case 7:
            UploadPresetFilterParameters(2);
            break;
        default:
            break;
        }
    } else {
        filterShaderType = mode;
    }
}

void ProcessPostFxFilterInput() {
    if (filterDispatchEnabled != 1 || GetSnapshotDataOption() != 0 || (g_inputBuffer[0].pressedPriority & 0x10U) == 0)
        return;
    const std::int32_t inputType = GetCurrentUIInputType();
    if (inputType == 0 || inputType == 1)
        return;
    switch (filterShaderType) {
    case 1:
        SetPostFxFilterMode(5);
        break;
    case 2:
        SetPostFxFilterMode(1);
        break;
    case 3:
        SetPostFxFilterMode(2);
        break;
    case 5:
        SetPostFxFilterMode(6);
        break;
    case 6:
        SetPostFxFilterMode(3);
        break;
    default:
        break;
    }
}

void PerformPostFxFilterRenderPass() {
    if (activePostFxRenderCallback)
        activePostFxRenderCallback();
}

std::int32_t GetPostFxFilterTransitionState() noexcept {
    return filterFadeBusy;
}

} // namespace shenmue::s2
