#pragma once

#include "s2/hlib.hpp"
#include "common/types.hpp"

#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <optional>

namespace shenmue::s2 {
inline constexpr shen_name SCREEN_FILM_PARAMETER_FILE{SHEN_NAME(SFIL)}, USER_FILTER_PARAMETER_FILE{SHEN_NAME(UFIL)};

enum class FilmControlFlag : std::uint32_t {
    Hide = 0,
    Show = 1,
    Force = 2,
    ForceShow = 3,
};

#pragma pack(push, 4)
struct ScreenFilmState {
    float alpha{};
    float red{};
    float green{};
    float blue{};
    float targetAlpha{};
    float targetRed{};
    float targetGreen{};
    float targetBlue{};
    float alphaStep{};
    float redStep{};
    float greenStep{};
    float blueStep{};
    std::int32_t remainingFrames{};
    std::uint32_t overlayColour{};
    std::int32_t disabled{};
    std::uint32_t maximumTime{};
};

struct ScreenFilmColourPair {
    std::uint32_t pending{};
    std::uint32_t colour{};
};

struct ScreenFilmTaskData {
    shen_name name{};
    ScreenFilmState state{};
    std::int32_t value{};
    ScreenFilmState alternateState{};
    std::uint32_t alternateActive{};
    ScreenFilmColourPair colourPair{};
};
#pragma pack(pop)

struct ScreenFilmColourRequest {
    std::array<std::uint8_t, 4> start{};
    std::array<std::uint8_t, 4> target{};
    std::int32_t durationFrames{};
};
static_assert(sizeof(ScreenFilmColourRequest) == 0x0c);
static_assert(offsetof(ScreenFilmColourRequest, durationFrames) == 0x08);

extern std::int32_t g_authoredFilmTrackSelection;

static_assert(sizeof(ScreenFilmState) == 0x40);
static_assert(offsetof(ScreenFilmState, alpha) == 0x00);
static_assert(offsetof(ScreenFilmState, targetAlpha) == 0x10);
static_assert(offsetof(ScreenFilmState, alphaStep) == 0x20);
static_assert(offsetof(ScreenFilmState, remainingFrames) == 0x30);
static_assert(offsetof(ScreenFilmState, disabled) == 0x38);
static_assert(sizeof(ScreenFilmTaskData) == 0x94);
static_assert(offsetof(ScreenFilmTaskData, state) == 0x04);
static_assert(offsetof(ScreenFilmTaskData, alternateState) == 0x48);
static_assert(offsetof(ScreenFilmTaskData, alternateActive) == 0x88);
static_assert(offsetof(ScreenFilmTaskData, colourPair) == 0x8c);

void filmSetFadeColourTo(HLib::Task *task, std::uint32_t colour);
void ResetTaskWaitFlags();
std::uint64_t GetIsWaitingFlag();
std::uint32_t GetFadedClearColour();
HLib::Task *Queue_SCF_Callback_ScreenFilmTaskEntry(std::uint8_t nextFunctionIndex);
std::int64_t GetScreenFilmAlpha(HLib::Task *filmTask);
bool IsScreenFilmNotShowing(const HLib::Task *filmTask);
bool DoesScreenFilmColourExist(const HLib::Task *filmTask);
bool filmCheckFadeTrackColour();
bool EV_GetScreenFilmIsNotShowing();
void SetScreenFilmFade(std::uint32_t frames, std::uint8_t startAlpha, std::uint8_t startRed, std::uint8_t startGreen, std::uint8_t startBlue,
    std::uint8_t targetAlpha, std::uint8_t targetRed, std::uint8_t targetGreen, std::uint8_t targetBlue);
void WaitForNextTask(HLib::Task *filmTask, const ScreenFilmColourRequest &request, std::int32_t mode);
void WaitForNextTask_0(HLib::Task *filmTask, std::int32_t mode, std::uint32_t durationFrames);
void StartScreenFilmFade(HLib::Task *filmTask, std::uint32_t durationFrames, std::uint8_t startAlpha, std::uint8_t targetAlpha);
void filmSetCinemaBarVisibility(ScreenFilmState *state, FilmControlFlag mode);


struct GlowParameters {
    std::int32_t index;
    float intensity;
    std::array<float, 4> values;
};
struct CrossParameters {
    std::array<float, 4> values;
    std::array<std::int32_t, 2> integerParameters;
    std::array<std::array<float, 14>, 4> streaks;
};
static_assert(sizeof(GlowParameters) == 0x18);
static_assert(sizeof(CrossParameters) == 0xf8);
static_assert(offsetof(CrossParameters, streaks) == 0x18);
inline constexpr GlowParameters INITIAL_POST_FX_GLOW{.index = 10, .intensity = 0.0f, .values = {0.0f, 0.0f, 0.0f, 0.2f}};
inline constexpr CrossParameters INITIAL_POST_FX_CROSS{.values = {0.0f, 0.3f, 0.3f, 0.3f},
    .integerParameters = {10, 4},
    .streaks = {{{0.0f, 0.0f, -1.0f, -1.0f, std::bit_cast<float>(0x3e672ef1U), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, std::bit_cast<float>(0x3cbbd9cbU),
                     std::bit_cast<float>(0x3ca5204bU), std::bit_cast<float>(0x3cb9d7feU)},
        {0.0f, 0.0f, -1.0f, 1.0f, 0.2f}, {0.0f, 0.0f, 1.0f, 1.0f, 0.2f}, {0.0f, 0.0f, 1.0f, -1.0f, 0.2f}}}};
extern GlowParameters cachedGlowParameters;
extern CrossParameters cachedCrossParameters;

struct PostEffectShaderPacket {
    std::uint32_t kind;
    std::array<std::byte, 0xf8> parameters;
};
struct PostProcessingState {
    std::array<std::int32_t, 3> enabled;
    std::array<PostEffectShaderPacket, 6> packets;
    std::array<std::int32_t, 6> writeGates;
};
static_assert(sizeof(PostEffectShaderPacket) == 0xfc);
static_assert(sizeof(PostProcessingState) == 0x60c);
static_assert(offsetof(PostProcessingState, packets) == 0x0c);
static_assert(offsetof(PostProcessingState, writeGates) == 0x5f4);

const PostProcessingState &GetPostProcessingState() noexcept;
void InitializePostProcessingState();
std::int64_t InitializePostFxModuleDefaults();
void RenderPostProcessing(std::int32_t pass);
inline constexpr GlowParameters defaultGlowParameters{5, 0.0f, {0.3f, 0.3f, 0.3f, 0.2f}};
inline constexpr GlowParameters nightGlowParameters{10, 0.0f, {0.3f, 0.3f, 0.3f, 0.26f}};

std::int64_t GetPostFxBankValue(std::int32_t bank) noexcept;
extern float g_configuredGlareIntensity;
extern float g_frameGlareIntensity;
void SetItemPreviewGlareIntensity(std::int32_t enabled) noexcept;
std::int64_t SelectPostFxBank(std::int32_t bank, std::int32_t value) noexcept;
std::int64_t UploadGlowParams(const GlowParameters &parameters);
std::int64_t UploadCrossParams(const CrossParameters &parameters);
std::int64_t UploadBlurStrength(float strength);
bool SFIL_UploadBloomParams(std::int32_t afternoon);

void InitializePostFxFilterState();
bool LoadPostFxFilterFile();


std::optional<std::int64_t> DispatchPostFxFilterRender(std::int32_t enabled);
std::int32_t GetPostFxFilterMode() noexcept;
void SetPostFxFilterMode(std::int32_t mode);
void ProcessPostFxFilterInput();
void PerformPostFxFilterRenderPass();
std::int32_t GetPostFxFilterTransitionState() noexcept;

} // namespace shenmue::s2
