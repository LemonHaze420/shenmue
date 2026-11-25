#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_api.h"
#include "s2/event_queued_dc.h"
#include "s2/butterfly.hpp"
static ButterflyRegion butterflyRegions[11] = {
#define KWM1_BUTTERFLY_REGION(recordFlags, runCount, posX, posY, posZ) {recordFlags, runCount, {posX, posY, posZ}},
#define KWM1_BUTTERFLY_END {0, 0, {0.0f, 0.0f, 0.0f}}
#include "kwm1_butterfly_regions.def"
#undef KWM1_BUTTERFLY_REGION
#undef KWM1_BUTTERFLY_END
};
enum { TASK_KWM1 = SHEN_NAME4('k', 'w', 'm', '1'), TASK_KWM1_WAVE = SHEN_NAME4('w', 'a', 'v', 'e') };
void AreaCleanup(int afternoon);
void LoadKWM1Audio(int afternoon);
void eKWM1_EVT_KWM1_mainloop_Callback(void *taskData);
void eKWM1_ET_WAVE_KWM1_Callback(void *taskData);
#else
#include "s2/areas/area_kwm1.hpp"
#include "s2/character.hpp"

#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/motion.hpp"
#include "s2/ui.hpp"
#include "s2/world_render.hpp"
#include "s2/maze.hpp"

#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace shenmue::s2::areas {

namespace {

constexpr float NativeFloat(std::uint32_t bits) {
    return std::bit_cast<float>(bits);
}

constexpr std::array butterflyRegions{
#define KWM1_BUTTERFLY_REGION(recordFlags, runCount, posX, posY, posZ) \
    ButterflyRegion{.flags = recordFlags, .totalRuns = runCount, .position = {posX, posY, posZ}},
#define KWM1_BUTTERFLY_END ButterflyRegion{}
#include "kwm1_butterfly_regions.def"
#undef KWM1_BUTTERFLY_REGION
#undef KWM1_BUTTERFLY_END
};

void eKWM1_EVT_KWM1_mainloop_Callback(void *taskData);
void eKWM1_ET_WAVE_KWM1_Callback(void *taskData);
void InitializeButterflyTaskData(ButterflyTaskData &data) {
    auto *const bytes = reinterpret_cast<std::byte *>(&data);
    for (std::size_t index = 0; index < 16; ++index) {
        const std::size_t record = 0x0c + index * 0x40;
        std::memset(bytes + record, 0, 0x20);
        std::memset(bytes + record + 0x20, 0, 0x04);
        std::memset(bytes + record + 0x30, 0, 0x10);
    }
    data.regions = butterflyRegions.data();
    data.butterflyCount = 16;
}

} // namespace

void AreaKWM1Init() {
}
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters) {
    EventDateTime dateTime;
    float distanceWork[64], firstWork[64], secondWork[64];
    Vector3 waveColour;
    void *firstKernel, *secondKernel;
#else
void AreaKWM1Main(std::uint32_t) {
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.HLib_EnqueueTaskWithoutParameter(eKWM1_EVT_KWM1_mainloop_Callback, 4, 0x0b, TASK_KWM1);
    FTBL.GetCurrentDateTime((DateTime *)&dateTime);
    AreaCleanup(0);
    FTBL.UpdateWeatherForMinute(0, 0, 0, 0.0f);
    FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
    waveColour.x = 0.3f;
    waveColour.y = 0.8f;
    waveColour.z = 0.5f;
    FTBL.GenerateFFT(0.5f);
    FTBL.fft_fill_dist(FTBL.fft_fill_krn(3.92f, FTBL.generate_distance_falloff_row_major(1.29f, distanceWork)));
    firstKernel = FTBL.fft_fill_krn(0.06f, FTBL.generate_distance_falloff_kernel_8x8(1.035f, firstWork));
    secondKernel = FTBL.fft_fill_krn(0.073f, FTBL.generate_distance_falloff_kernel_8x8(0.564f, secondWork));
    FTBL.fft_bins_load_pairwise_tables(firstKernel, secondKernel);
    FTBL.FFTUtil(0, -0.0015f);
    FTBL.set_lighting_flags(1.0f, &waveColour);
#else
    HLib::EnqueueTaskWithoutParameter(eKWM1_EVT_KWM1_mainloop_Callback, 4, 0x0b, static_cast<std::int32_t>(TASK_KWM1.name));

    DateTime dateTime{};
    shenGetCurrentDateTime(dateTime);
    CheckIsArgEquZero(1);

    EV_ReinitLights(1);
    EV_SetLightFlagForIndex(0, 2);
    EV_SetIndexForLightEntry(0, 1);
    EV_AssignLightParams(1, 1);
    EV_AssignLightParams(1, 2);
    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);

    const Vector3 lightingFlags{NativeFloat(0x3E99999AU), NativeFloat(0x3F4CCCCDU), NativeFloat(0x3F000000U)};
    std::array<std::byte, 0x100> distanceWork, firstWork, secondWork;
    GenerateFFT(0.5f);
    FillFftDistance(FillFftKernel(3.92f, GenerateDistanceFalloffRowMajor(1.29f, distanceWork)));
    void *const secondKernel = FillFftKernel(0.073f, GenerateDistanceFalloffKernel8x8(0.564f, secondWork));
    void *const firstKernel = FillFftKernel(0.06f, GenerateDistanceFalloffKernel8x8(1.035f, firstWork));
    ApplyFft(firstKernel, secondKernel);
    FinalizeFft(0.0f, -0.0015f);
    SetWaveLighting(1.0f, lightingFlags);

#endif

#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.HLib_EnqueueTaskWithoutParameter(eKWM1_ET_WAVE_KWM1_Callback, 3, 2, TASK_KWM1_WAVE);
    QueueButterflies(butterflyRegions, 16);
    FTBL.ConfigureScenarioNodeMotion(SHEN_NAME4('F', 'I', 'S', 'H'), 0xa003, 0, 32767.0f, 0, 1.0f, 0);
    FTBL.ConfigureScenarioNodeMotion(SHEN_NAME4('F', 'I', 'S', 'H'), 0, -1, -1, -1, 32767.0f, 4);
    FTBL.ControlMazeFish();
    FTBL.SetCurrentRefCount(0);
#else
    HLib::EnqueueTaskWithoutParameter(eKWM1_ET_WAVE_KWM1_Callback, 3, 2, static_cast<std::int32_t>(TASK_KWM1_WAVE.name));
    HLib::Task *const butterflyTask = HLib::EnqueueTaskWithParameter(EVT_MoveButterfly_Callback, 4, 0x0d, sizeof(ButterflyTaskData), TASK_BUTTERFLY);
    auto *const butterflyData = static_cast<ButterflyTaskData *>(HLib::GetTaskParameter(butterflyTask));
    InitializeButterflyTaskData(*butterflyData);

    ConfigureScenarioNodeMotion(TASK_FISH, 0xa003, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
    ConfigureScenarioNodeMotion(TASK_FISH, 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
    EV_MAZ_fishCtrl();
    EV_SetCurrentRefCount(0);
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon) {
#else
void AreaKWM1Cleanup(std::int32_t, std::uint32_t) {
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.CheckIsArgEquZero(1, 1);
    FTBL.ReinitializeLights(1);
    FTBL.SetLightFlagForIndex(0, 2);
    FTBL.SetLightEntryIndex(0, 1);
    FTBL.AssignLightParams(1, 1);
    FTBL.AssignLightParams(1, 2);
#else
    CheckIsArgEquZero(1);
    EV_ReinitLights(1);
    EV_SetLightFlagForIndex(0, 2);
    EV_SetIndexForLightEntry(0, 1);
    EV_AssignLightParams(1, 1);
    EV_AssignLightParams(1, 2);
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    LoadKWM1Audio(afternoon);
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void LoadKWM1Audio(int afternoon) {
}
#else
namespace {
#endif

void eKWM1_EVT_KWM1_mainloop_Callback(void *taskData) {
#if defined(SHENMUE_EVENT_OVERLAY)
    Vector3 firstCorner, secondCorner;
    if (FTBL.GetShouldRenderIcons(1))
        return;
    FTBL.SetCurrentRefCount(FTBL.mazeGetCurrentRefCount() + 1);
    firstCorner.x = 1400;
    firstCorner.y = 98;
    firstCorner.z = 960;
    secondCorner.x = 1230;
    secondCorner.y = 98.400001525878906f;
    secondCorner.z = 937.20001220703125f;
    FTBL.SetMazeFish(&firstCorner, &secondCorner, 30, 1234, FTBL.ctrlGetCharaByName(SHEN_NAME4('F', 'I', 'S', 'H')));
#else
    if (GetShouldRenderIcons(RenderIconSlot::ItemSelect))
        return;

    EV_SetCurrentRefCount(EV_GetCurrentRefCount() + 1U);
    const Vector3 firstCorner{1400.0f, 98.0f, 960.0f};
    const Vector3 secondCorner{1230.0f, NativeFloat(0x42C4CCCDU), NativeFloat(0x446A4CCDU)};
    EV_SetFish(&firstCorner, &secondCorner, 30, 1234, ctrlGetCharaByName(TASK_FISH));
#endif
}

void eKWM1_ET_WAVE_KWM1_Callback(void *taskData) {
#if defined(SHENMUE_EVENT_OVERLAY)
    Vector3 waveColour = {0.3f, -1.0f, 0.5f};
    float intensity = 1.0f;
    FTBL.BuildLightingTableFrom(0, intensity, intensity, intensity, intensity, intensity, intensity, intensity, intensity, 100, intensity);
    FTBL.ApplyFFTWaveformToStripTree(FTBL.GetMapModelRoot(1));
    FTBL.nlSetTexanimUV(0, 0.015f);
    FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(2));
#else
    BuildWaveLightingTable(0, 1.0f, 1.0f, {0.6f, 0.6f, 0.7f}, {0.5f, 0.5f, 0.5f}, 100.0f, 1.0f);
    ApplyWaveTextureResource(1);
    SetTextureAnimationUV(0.0f, 0.015f);
    ApplyTextureAnimationResource(2);
#endif
}

#if !defined(SHENMUE_EVENT_OVERLAY)

} // namespace

const GameplayAreaDefinition areaKWM1GameplayDefinition{
    .areaId = AREA_MAZE_FISH, .padding04 = 0, .init = AreaKWM1Init, .main = AreaKWM1Main, .cleanup = AreaKWM1Cleanup};

} // namespace shenmue::s2::areas

#endif
