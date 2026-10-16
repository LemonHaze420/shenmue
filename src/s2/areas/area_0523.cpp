#include "s2/areas/area_0523.hpp"
#include "s2/author.hpp"
#include "s2/sound_program.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fog.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/world_render.hpp"
#include <array>
#include <bit>
namespace shenmue::s2::areas {
namespace {
std::uint32_t firstPlanetColour0523{}, secondPlanetColour0523{}, skyColour0523{}, cloudColour0523{}, fogColour0523{};
std::uint32_t Interpolate0523Colour(std::uint32_t source, std::int32_t blue, std::int32_t green, std::int32_t red, std::int32_t frame) {
    const std::int32_t oldBlue = static_cast<std::int32_t>((source >> 16) & 255U);
    const std::int32_t oldGreen = static_cast<std::int32_t>((source >> 8) & 255U);
    const std::int32_t oldRed = static_cast<std::int32_t>(source & 255U);
    return (source & 0xff000000U) | (static_cast<std::uint32_t>(oldBlue + frame * (blue - oldBlue) / 90) << 16) |
        (static_cast<std::uint32_t>(oldGreen + frame * (green - oldGreen) / 90) << 8) | static_cast<std::uint32_t>(oldRed + frame * (red - oldRed) / 90);
}
} // namespace
void Initialize0523() {
}
void Start0523(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0523, 4, 0x0b, sizeof(Area0523Parameter), AREA_0523_TASK);
    Area0523Parameter *const parent = static_cast<Area0523Parameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, "0523A.MOT"));
    parent->selection = 0;
    parent->state = 1;
    EV_stepSetFlags(0, 0xf1);
    EV_stepSetFlags(0, 0xda);
    Setup0523Waves();
}
void Update0523(void *parameter) {
    Area0523Parameter *const parent = static_cast<Area0523Parameter *>(parameter);
    if (parent->state == 1)
        Queue0523Author(parent);
    else if (parent->state == 3) {
        EV_FreeAuthorPak(0, "0523");
        DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
        EV_CleanupMapControlTask();
        HLib::DestroyTask(parent->self);
        EV_scenQueueLoad(g_actNumber, AREA_0523_RETURN_MAP, 0);
    }
}
void Setup0523Waves() {
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0523");
    EV_SetWeatherInstruction(0x1800, 0);
    EV_SetWeatherInstruction(0x780, 0);
    std::array<std::byte, 0x100> distance, first, second;
    GenerateFFT(0.5f);
    FillFftDistance(FillFftKernel(3.9200000762939453f, GenerateDistanceFalloffRowMajor(1.2899999618530273f, distance)));
    void *const firstKernel = FillFftKernel(0.0729999989271164f, GenerateDistanceFalloffKernel8x8(0.5640000104904175f, first));
    void *const secondKernel = FillFftKernel(0.05999999865889549f, GenerateDistanceFalloffKernel8x8(1.034999966621399f, second));
    ApplyFft(secondKernel, firstKernel);
    FinalizeFft(0.0f, 0.004699999932199717f);
    SetWaveLighting(0.1f, Vector3{0.3f, 0.8f, 0.5f});
    HLib::EnqueueTaskWithoutParameter(Update0523Waves, 3, 2, static_cast<std::int32_t>(AREA_0523_TASK.name));
}
void Queue0523Author(Area0523Parameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0523Author, 4, 0x0b, sizeof(Area0523AuthorParameter), AREA_0523_TASK);
    static_cast<Area0523AuthorParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    EV_spmBeginSampleExecute();
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0523_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0523_RYO, false);
    for (const shen_name *prop = AREA_0523_PROPS; prop->name != 0xffffffffU; ++prop)
        EV_SetCharacterAlphaEnabled(*prop, false);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}
void Update0523Waves(void *) {
    BuildWaveLightingTable(0, 1.0f, 1.0f, Vector3{0.48f, 0.48f, 0.67f}, Vector3{0.5f, 0.5f, 0.5f}, 50.0f, 1.0f);
    ApplyWaveTextureResource(1);
    SetTextureAnimationUV(0.0f, 0.004699999932199717f);
    ApplyTextureAnimationResource(0);
}
void Update0523Author(void *parameter) {
    Area0523AuthorParameter *const child = static_cast<Area0523AuthorParameter *>(parameter);
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (frame == 1) {
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_FreeWAIR();
        EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
        EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
        EV_UpdateWeather();
    } else if (frame == 340) {
        EV_FreeWAIR();
        EV_UpdateWeatherForMinute(0, 0, 60, 1.0f);
        EV_UpdateWeather();
    } else if (frame == 692)
        EV_UpdateWeatherForMinute(1, 0, 120, 0.1f);
    else if (frame == 822)
        EV_UpdateWeatherForMinute(1, 0, 130, 1.0f);
    else if (frame == 460) {
        firstPlanetColour0523 = static_cast<std::uint32_t>(GetPlanetLayerColour(0, 0));
        secondPlanetColour0523 = static_cast<std::uint32_t>(GetPlanetLayerColour(1, 0));
        skyColour0523 = GetWeatherCloudOrSkyColour(0);
        cloudColour0523 = GetWeatherCloudOrSkyColour(1);
        GetFogRed();
        GetFogGreen();
        GetFogBlue();
        fogColour0523 = static_cast<std::uint32_t>(GetFogColour());
    } else if (static_cast<std::uint32_t>(frame - 461) <= 88U) {
        const std::int32_t progress = frame - 460;
        const std::uint32_t first = Interpolate0523Colour(firstPlanetColour0523, 80, 80, 80, progress);
        const std::uint32_t second = Interpolate0523Colour(secondPlanetColour0523, 196, 184, 130, progress);
        EV_SetPlanetLayerColours_Impl(first, second, 0);
        EV_SetPlanetLayerColours_Impl(first, second, 1);
        SetCloudAndSkyColour(Interpolate0523Colour(skyColour0523, 160, 160, 160, progress), Interpolate0523Colour(cloudColour0523, 48, 48, 48, progress));
        SetFogRed(static_cast<float>(progress) * 0.1f / 90.0f + 0.0f);
        SetFogGreen(static_cast<float>(progress) * 100.0f / 90.0f + 0.0f);
        SetFogBlue(static_cast<float>(progress) * 0.68f / 90.0f + 0.0f);
        SetFogDistance(Interpolate0523Colour(fogColour0523, 58, 58, 58, progress));
        ReinitFogTasks(1);
    } else if (frame == 550) {
        SetCloudAndSkyColour(0x00a0a0a0U, 0x00303030U);
        EV_SetPlanetLayerColours_Impl(0x00505042U, 0x00c4b882U, 0);
        EV_SetPlanetLayerColours_Impl(0x00505042U, 0x00c4b882U, 1);
        SetFogRed(0.1f);
        SetFogGreen(100.0f);
        SetFogBlue(0.68f);
        SetFogDistance(0x003a3a3aU);
        ReinitFogTasks(1);
    }
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) == 0) {
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0523_RYO), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_0523_RYO, true);
        for (const shen_name *prop = AREA_0523_PROPS; prop->name != 0xffffffffU; ++prop)
            EV_SetCharacterAlphaEnabled(*prop, true);
        child->parent->state = 3;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::DestroyCurrentTask();
    }
}
const ScriptAreaDefinition area0523ScriptDefinition{.areaId = AREA_0523, .field04 = 0, .init = Initialize0523, .main = Start0523};
} // namespace shenmue::s2::areas
