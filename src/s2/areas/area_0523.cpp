#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_0523.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void Start0523(void)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(Update0523, 4, 11, sizeof(AreaAuthorTaskParameter), TASK_EVNT);
    AreaAuthorTaskParameter *parent = (AreaAuthorTaskParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
    parent->motion = FTBL.ReadMotiFromDirectory(g_actNumber, g_currentMap, "0523A.MOT");
    parent->state = 1;
    parent->selection = 0;
    FTBL.EV_stepSetFlags(0xf1, 0);
    FTBL.EV_stepSetFlags(0xda, 0);
    Setup0523Waves();
}
#else

static int colourWork0523[3];
static int firstPlanetColour0523[4], secondPlanetColour0523[4], skyColour0523[4], cloudColour0523[4], fogColour0523[4];
static float fogRed0523, fogGreen0523, fogBlue0523;

void Update0523(void *parameter)
{
    AreaAuthorTaskParameter *parent = (AreaAuthorTaskParameter *)parameter;
    switch (parent->state) {
    case 1:
        Queue0523Author(parent);
        break;
    case 3:
        Cleanup0523(parent);
        break;
    }
}

void Queue0523Author(AreaAuthorTaskParameter *parent)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(Update0523Author, 4, 11, sizeof(AreaAuthorLoopTaskParameter), TASK_EVNT);
    ((AreaAuthorLoopTaskParameter *)FTBL.HLib_GetTaskParameter(task))->parent = parent;
    FTBL.EV_spmBeginSampleExecute(0);
    Setup0523Characters();
    FTBL.EV_authSceneCtrl(parent->selection);
    parent->state = 2;
}

void Update0523Author(void *parameter)
{
    AreaAuthorLoopTaskParameter *child = (AreaAuthorLoopTaskParameter *)parameter;

    float blue = 0.0f, green = 0.0f, red = 0.0f;
    unsigned int firstPlanet, secondPlanet, cloud, fog;
    int frame = FTBL.EV_authSceneCtrl(-6);
    switch (frame) {
    case 1:
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.FreeWAIR();
        FTBL.UpdateWeatherForMinute(0, 0, 0, 0.0f);
        FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
        FTBL.UpdateWeather();
        break;
    case 340:
        FTBL.FreeWAIR();
        FTBL.UpdateWeatherForMinute(0, 0, 60, 1.0f);
        FTBL.UpdateWeather();
        break;
    case 692:
        FTBL.UpdateWeatherForMinute(1, 0, 120, 0.1f);
        break;
    case 822:
        FTBL.UpdateWeatherForMinute(1, 0, 130, 1.0f);
        break;
    }
    if (frame == 460) {
        unsigned int sky;
        firstPlanet = FTBL.GetPlanetLayerColour(0, 0);
        secondPlanet = FTBL.GetPlanetLayerColour(1, 0);
        sky = FTBL.GetWeatherCloudOrSkyColour(0);
        cloud = FTBL.GetWeatherCloudOrSkyColour(1);
        red = FTBL.GetFogRed();
        SHENMUE_EVENT_CLOBBER("r9");
        green = FTBL.GetFogGreen();
        blue = FTBL.GetFogBlue();
        fog = FTBL.GetFogColour();
        fogColour0523[0] = fog >> 24;
        fogColour0523[1] = (fog & 0xff0000) >> 16;
        fogColour0523[2] = (fog & 0xff00) >> 8;
        fogColour0523[3] = fog & 0xff;
        firstPlanetColour0523[0] = firstPlanet >> 24;
        firstPlanetColour0523[1] = (firstPlanet & 0xff0000) >> 16;
        firstPlanetColour0523[2] = (firstPlanet & 0xff00) >> 8;
        firstPlanetColour0523[3] = firstPlanet & 0xff;
        secondPlanetColour0523[0] = secondPlanet >> 24;
        secondPlanetColour0523[1] = (secondPlanet & 0xff0000) >> 16;
        secondPlanetColour0523[2] = (secondPlanet & 0xff00) >> 8;
        secondPlanetColour0523[3] = secondPlanet & 0xff;
        skyColour0523[0] = sky >> 24;
        skyColour0523[1] = (sky & 0xff0000) >> 16;
        skyColour0523[2] = (sky & 0xff00) >> 8;
        skyColour0523[3] = sky & 0xff;
        cloudColour0523[0] = cloud >> 24;
        cloudColour0523[1] = (cloud & 0xff0000) >> 16;
        cloudColour0523[2] = (cloud & 0xff00) >> 8;
        cloudColour0523[3] = cloud & 0xff;
    } else if (frame > 460 && frame < 550) {
        int *work = colourWork0523;
        int a, b, g, r, skyA, skyB, skyG, skyR;
        unsigned int sky;
        a = firstPlanetColour0523[0];
        b = firstPlanetColour0523[1] + (80 - firstPlanetColour0523[1]) * (frame - 460) / 90;
        g = firstPlanetColour0523[2] + (80 - firstPlanetColour0523[2]) * (frame - 460) / 90;
        r = firstPlanetColour0523[3] + (80 - firstPlanetColour0523[3]) * (frame - 460) / 90;
        a <<= 24;
        firstPlanet = a + (b << 16) + (g << 8) + r;
        work[0] = secondPlanetColour0523[0];
        work[1] = secondPlanetColour0523[1] + (196 - secondPlanetColour0523[1]) * (frame - 460) / 90;
        work[2] = secondPlanetColour0523[2] + (184 - secondPlanetColour0523[2]) * (frame - 460) / 90;
        work[3] = secondPlanetColour0523[3] + (130 - secondPlanetColour0523[3]) * (frame - 460) / 90;
        a = work[0] << 24;
        secondPlanet = a + (work[1] << 16) + (work[2] << 8) + work[3];
        FTBL.EV_SetPlanetLayerColours_Impl(firstPlanet, secondPlanet, 0);
        FTBL.EV_SetPlanetLayerColours_Impl(firstPlanet, secondPlanet, 1);
        skyA = skyColour0523[0];
        skyB = skyColour0523[1] + (160 - skyColour0523[1]) * (frame - 460) / 90;
        skyG = skyColour0523[2] + (160 - skyColour0523[2]) * (frame - 460) / 90;
        skyR = skyColour0523[3] + (160 - skyColour0523[3]) * (frame - 460) / 90;
        sky = (skyA << 24) + (skyB << 16) + (skyG << 8) + skyR;
        work[0] = cloudColour0523[0];
        work[1] = cloudColour0523[1] + (48 - cloudColour0523[1]) * (frame - 460) / 90;
        work[2] = cloudColour0523[2] + (48 - cloudColour0523[2]) * (frame - 460) / 90;
        work[3] = cloudColour0523[3] + (48 - cloudColour0523[3]) * (frame - 460) / 90;
        a = work[0] << 24;
        cloud = a + (work[1] << 16) + (work[2] << 8) + work[3];
        FTBL.SetCloudAndSkyColour(sky, cloud);
        work[0] = fogColour0523[0];
        work[1] = fogColour0523[1] + (58 - fogColour0523[1]) * (frame - 460) / 90;
        work[2] = fogColour0523[2] + (58 - fogColour0523[2]) * (frame - 460) / 90;
        work[3] = fogColour0523[3] + (58 - fogColour0523[3]) * (frame - 460) / 90;
        a = work[0] << 24;
        fog = a + (work[1] << 16) + (work[2] << 8) + work[3];
        fogRed0523 = red + (0.1f - red) * (frame - 460) / 90.0f;
        fogGreen0523 = green + (100.0f - green) * (frame - 460) / 90.0f;
        fogBlue0523 = blue + (0.68f - blue) * (frame - 460) / 90.0f;
        FTBL.SetFogRed(fogRed0523);
        FTBL.SetFogGreen(fogGreen0523);
        FTBL.SetFogBlue(fogBlue0523);
        FTBL.SetFogDistance(fog);
        FTBL.ReinitFogTasks(1);
    } else if (frame == 550) {
        FTBL.SetCloudAndSkyColour(0xa0a0a0, 0x303030);
        FTBL.EV_SetPlanetLayerColours_Impl(0x505042, 0xc4b882, 0);
        FTBL.EV_SetPlanetLayerColours_Impl(0x505042, 0xc4b882, 1);
        FTBL.SetFogRed(0.1f);
        FTBL.SetFogGreen(100.0f);
        FTBL.SetFogBlue(0.68f);
        FTBL.SetFogDistance(0x3a3a3a);
        FTBL.ReinitFogTasks(1);
    }
    if (!FTBL.EV_authSceneCtrl(-1)) {
        Restore0523Characters();
        child->parent->state = 3;
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        FTBL.HLib_DestroyCurrentTask();
    }
}

void Setup0523Waves(void)
{
    unsigned char distance[256], second[256], first[256];
    Vector3 colour;
    void *secondKernel;
    void *firstKernel;
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_FreeAuthorPak(1, "0523");
    FTBL.EV_SetWeatherInstruction(0x1800, 0);
    FTBL.EV_SetWeatherInstruction(0x780, 0);
    colour.x = 0.3f;
    colour.y = 0.8f;
    colour.z = 0.5f;
    FTBL.GenerateFFT(0.5f);
    FTBL.fft_fill_dist(FTBL.fft_fill_krn(3.92f, FTBL.generate_distance_falloff_row_major(1.29f, distance)));
    secondKernel = FTBL.fft_fill_krn(0.06f, FTBL.generate_distance_falloff_kernel_8x8(1.035f, second));
    firstKernel = FTBL.fft_fill_krn(0.073f, FTBL.generate_distance_falloff_kernel_8x8(0.564f, first));
    FTBL.fft_bins_load_pairwise_tables((const float *)secondKernel, (const float *)firstKernel);
    FTBL.FFTUtil(0.0f, 0.0047f);
    FTBL.set_lighting_flags(0.1f, &colour);
    FTBL.HLib_EnqueueTaskWithoutParameter(Update0523Waves, 3, 2, TASK_EVNT);
}

void Cleanup0523(AreaAuthorTaskParameter *parent)
{
    FTBL.EV_FreeAuthorPak(0, "0523");
    FTBL.DeleteMotion(parent->motion);
    FTBL.HLib_DestroyTask(parent->self);
    FTBL.EV_scenQueueLoad(g_actNumber, AREA_0523_RETURN_MAP, 0);
}

const shen_name area0523Props[] = {AREA_0523_a150, AREA_0523_a149, AREA_0523_a151, AREA_0523_a152, AREA_0523_a153, AREA_0523_a154, AREA_0523_a155,
    AREA_0523_a156, AREA_0523_a157, AREA_0523_a158, AREA_0523_a159, AREA_0523_a160, AREA_0523_a161, AREA_0523_a162, AREA_0523_a179, AREA_0523_a178,
    AREA_0523_a175, AREA_0523_a176, AREA_0523_a177, AREA_0523_a180, AREA_0523_a181, AREA_0523_a182, AREA_0523_a183, AREA_0523_a184, AREA_0523_a188,
    AREA_0523_a189, AREA_0523_a186, AREA_0523_a185, AREA_0523_a187, AREA_0523_a174, INVALID_CHARACTER};

void Setup0523Characters(void)
{
    int i;
    HideAuthorCharacter(RYO_CHARACTER);
    for (i = 0; area0523Props[i] != INVALID_CHARACTER; i++)
        HideAuthorProp(area0523Props[i]);
}

void Restore0523Characters(void)
{
    int i;
    ShowAuthorCharacter(RYO_CHARACTER);
    for (i = 0; area0523Props[i] != INVALID_CHARACTER; i++)
        ShowAuthorProp(area0523Props[i]);
}

#define SHENMUE_EVENT_AUTHOR_PROPS
#include "s2/events/event_author_dc.h"

void Update0523Waves(void *parameter)
{
    float scale = 1.0f;
    FTBL.BuildLightingTableFrom(0, scale, scale, scale, scale, scale, scale, scale, scale, 50.0f, scale);
    FTBL.ApplyFFTWaveformToStripTree(FTBL.GetMapModelRoot(1));
    FTBL.nlSetTexanimUV(0.0f, 0.0047f);
    FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(0));
}
#endif
#else
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
#endif
