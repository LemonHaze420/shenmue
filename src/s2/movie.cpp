#include "s2/movie.hpp"

#include "s2/audio_data.hpp"
#include "s2/hlib.hpp"
#include "s2/input.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/pak.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/system_modules.hpp"
#include "s2/title.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace shenmue::s2 {
namespace {
struct MovieElement {
    float x, y, depth, scaleX, scaleY;
    std::uint32_t alignment;
};
constexpr std::array<MovieElement, 5> movieElements{{
    {320.0f, 240.0f, -4.0f, 1.25f, 1.25f, 5},
    {-50.0f, -27.0f, 0.0f, 1.0f, 1.0f, 0},
    {267.0f, -16.0f, 0.0f, 1.0f, 1.0f, 0},
    {74.0f, -69.0f, 0.0f, 1.0f, 1.0f, 0},
    {72.0f, 285.0f, 0.0f, 1.0f, 1.0f, 0},
}};
constexpr std::array<const char *, 5> movieFiles{"s1d_bg.dds", "s1d_f01.dds", "s1d_f02.dds", "s1d_she.dds", "s1d_bme.dds"};
std::array<NamedSpriteTextureRecord, 5> movieTextures{};
std::array<LegacyStripType0Vertex, 4> movieFadeVertices{{
    {{0.0f, 0.0f, 0.0f}, 0}, {{640.0f, 0.0f, 0.0f}, 0},
    {{0.0f, 480.0f, 0.0f}, 0}, {{640.0f, 480.0f, 0.0f}, 0},
}};


std::array<std::byte, 28> movieEnvironment{};
std::array<std::byte, 28> savedMovieEnvironment{};
bool environmentSaved{};
bool movieSpritesLoaded{};
std::int32_t runningMovieRender{};
std::int32_t movieRendererCounter{};
std::int32_t movieTaskState{};
bool hasMovieTaskFaded{};
float movieAlpha{};
float movieFadeStep{};
std::int32_t movieRenderInterval{};
std::int32_t movieRenderFrameCount{};
float movieFrameTime{};
float movieFrameDelta{};



std::int32_t MovieTaskReloadState() noexcept {
    return 4;
}
bool ShouldEndMovieRender() noexcept {
    return true;
}
void MoviePlaybackNullsub(std::int32_t) noexcept {
}

void LoadMovieSprites() {
    for (auto &texture : movieTextures)
        texture.textureIndex = -1;
    for (std::size_t i = 0; i < movieTextures.size(); ++i) {
        void *allocation = nullptr;
        std::size_t size = 0;
        if (!LoadLegacySprite("SCENE/04/MOVIE", movieFiles[i], SHEN_NAME(MOVI), allocation, size))
            continue;
        auto *bytes = static_cast<std::byte *>(allocation);
        auto &texture = movieTextures[i];
        std::memcpy(&texture.format, bytes + 0x14, sizeof(texture.format));
        std::memcpy(&texture.width, bytes + 0x18, sizeof(texture.width));
        std::memcpy(&texture.height, bytes + 0x1a, sizeof(texture.height));
        texture.imageOffset =
            static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(bytes + 0x1c) - reinterpret_cast<std::uintptr_t>(HLib::GetDynamicUseStoragePtr()));
        texture.textureIndex = ReadNamedSpriteTexture(texture, movieFiles[i], size - 0x1c);
        HLib::FreeTaskData(allocation);
    }
    movieSpritesLoaded = true;
}

void UpdateMovieElement(std::size_t index) {
    const auto &texture = movieTextures[index];
    if (!movieSpritesLoaded)
        return;
    const auto &element = movieElements[index];
    LegacyIconSprite sprite{};
    sprite.x = element.x;
    sprite.y = element.y;
    sprite.depth = element.depth;
    sprite.scaleX = element.scaleX;
    sprite.scaleY = element.scaleY;
    sprite.flags = 0x400;
    sprite.icon = static_cast<std::uint16_t>(texture.textureIndex);
    sprite.width = static_cast<std::int16_t>(texture.width);
    sprite.height = static_cast<std::int16_t>(texture.height);
    sprite.alignment = static_cast<std::uint8_t>(element.alignment);
    PrepareDrawTextureWithMode(sprite, ScreenMode::Centered);
}

void UpdateMovieAlpha() {
    movieRenderInterval = std::max(movieRenderInterval - 1, 0);
    movieAlpha = std::clamp(movieAlpha + movieFadeStep, 0.0f, 255.0f);
    const auto alpha = static_cast<std::uint32_t>(movieAlpha);
    if (!alpha)
        return;
    for (auto &vertex : movieFadeVertices)
        vertex.diffuse = (vertex.diffuse & 0x00ffffffU) | (alpha << 24U);
    SubmitStripType3Quad(movieFadeVertices);
}
} // namespace

void FreeMovieResources() {
    if (!runningMovieRender)
        return;


    MoviePlaybackNullsub(0);
    MoviePlaybackNullsub(0);
    if (movieSpritesLoaded) {
        for (auto &texture : movieTextures) {
            ReleaseNamedTexture(texture.textureIndex);
            texture.textureIndex = -1;
        }
    }
    ResetSceneArchiveFileHandles();
    if (environmentSaved)
        movieEnvironment = savedMovieEnvironment;
}
namespace {

void MovieTaskCallback(void *) {
    if (!runningMovieRender)
        return;
    if (!hasMovieTaskFaded && MovieTaskReloadState() == 2) {
        movieFadeStep = -8.5f;
        movieAlpha = 255.0f;
        hasMovieTaskFaded = true;
        movieRenderInterval = 30;
    }
    for (std::size_t i = 0; i < movieTextures.size(); ++i)
        UpdateMovieElement(i);
    MoviePlaybackNullsub(0);
    UpdateMovieAlpha();
    if (movieRenderFrameCount > 0) {
        --movieRenderFrameCount;
        movieFrameDelta += movieFrameTime;
        std::int32_t elapsed = static_cast<std::int32_t>(movieFrameDelta);
        elapsed = elapsed > 0 ? 0 : std::max(elapsed, -999);
        MoviePlaybackNullsub(elapsed);
    }
    if (ShouldEndMovieRender() || (g_inputBuffer[0].pressedPriority & 0x201U)) {
        if (movieTaskState == 0) {
            movieFadeStep = 8.5f;
            movieFrameTime = -33.299999f;
            movieTaskState = 1;
            movieRenderInterval = 30;
            movieAlpha = 0.0f;
            movieRenderFrameCount = 30;
            movieFrameDelta = 0.0f;
            return;
        }
    } else if (movieTaskState == 0) {
        return;
    }
    if (movieRenderInterval > 0)
        return;
    FreeMovieResources();
    if (g_titleFlagShouldRun == 11) {
        ChangeSystemModule(SystemModule::MovieToOmake, 0);
    } else {
        SetNextSceneNumber(1);
        ChangeSystemModule(SystemModule::Hot, 0);
    }
}
} // namespace


void MovieTask(void *taskData) {
    const auto *module = static_cast<const SystemModuleTaskData *>(taskData);
    movieSpritesLoaded = false;
    DispatchPostFxFilterRender(1);
    HLib::SetCurrentTaskTo(MovieTaskCallback);
    DisableAicaIfEnabled();
    LoadMovieSprites();
    runningMovieRender = 1;
    movieRendererCounter = 0;
    MoviePlaybackNullsub(module->currentEntry);
    environmentSaved = module->currentEntry != 1;
    if (environmentSaved) {
        savedMovieEnvironment = movieEnvironment;
        MoviePlaybackNullsub(0);
    }
    movieFadeStep = 0.0f;
    movieAlpha = 255.0f;
    movieRenderInterval = 0;
    g_titleFlagShouldRun = module->currentEntry == 0 ? 1 : 11;
    movieTaskState = 0;
    hasMovieTaskFaded = false;
}

std::int32_t GetIsRunningMovieRenderer() noexcept {
    return runningMovieRender;
}
void ResetRunningMovieRender() noexcept {
    runningMovieRender = 0;
}
} // namespace shenmue::s2
