#include "s2/areas/area_kwm1.hpp"
#include "s2/character.hpp"

#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/motion.hpp"
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
    ButterflyRegion{.flags = 0, .totalRuns = 4, .position = {NativeFloat(0x449B3B33U), NativeFloat(0x42CDAE14U), NativeFloat(0x4477BF5CU)}},
    ButterflyRegion{.flags = 0, .totalRuns = 3, .position = {NativeFloat(0x449CAA3DU), NativeFloat(0x42CE3333U), NativeFloat(0x4476FE14U)}},
    ButterflyRegion{.flags = 0, .totalRuns = 3, .position = {NativeFloat(0x449DC666U), NativeFloat(0x42CAF5C3U), NativeFloat(0x4474347BU)}},
    ButterflyRegion{.flags = 0, .totalRuns = 5, .position = {NativeFloat(0x44A0E28FU), NativeFloat(0x42CC3D71U), NativeFloat(0x44718333U)}},
    ButterflyRegion{.flags = 0, .totalRuns = 1, .position = {NativeFloat(0x44A3199AU), NativeFloat(0x42C951ECU), NativeFloat(0x44705D71U)}},
    ButterflyRegion{.flags = 0, .totalRuns = 2, .position = {NativeFloat(0x44A4451FU), NativeFloat(0x42C7F0A4U), NativeFloat(0x44700000U)}},
    ButterflyRegion{.flags = 0, .totalRuns = 2, .position = {NativeFloat(0x44A560A4U), NativeFloat(0x42C8851FU), NativeFloat(0x446F5CCDU)}},
    ButterflyRegion{.flags = 0, .totalRuns = 3, .position = {NativeFloat(0x44A65EB8U), NativeFloat(0x42C86B85U), NativeFloat(0x4470728FU)}},
    ButterflyRegion{.flags = 0, .totalRuns = 2, .position = {NativeFloat(0x44A8A000U), NativeFloat(0x42C93D71U), NativeFloat(0x4470C148U)}},
    ButterflyRegion{.flags = 0, .totalRuns = 2, .position = {NativeFloat(0x44AA151FU), NativeFloat(0x42C8B852U), NativeFloat(0x4472A666U)}},
    ButterflyRegion{},
};

void eKWM1_EVT_KWM1_mainloop_Callback(void *) {
    if (IsAreaItemSelectionActive())
        return;

    EV_SetCurrentRefCount(EV_GetCurrentRefCount() + 1U);
    const Vector3 firstCorner{1400.0f, 98.0f, 960.0f};
    const Vector3 secondCorner{1230.0f, NativeFloat(0x42C4CCCDU), NativeFloat(0x446A4CCDU)};
    EV_SetFish(&firstCorner, &secondCorner, 30, 1234, ctrlGetCharaByName(TASK_FISH));
}

void eKWM1_ET_WAVE_KWM1_Callback(void *) {
    BuildWaveLightingTable(0, 1.0f, 1.0f, {0.6f, 0.6f, 0.7f}, {0.5f, 0.5f, 0.5f}, 100.0f, 1.0f);
    ApplyWaveTextureResource(1);
    SetTextureAnimationUV(0.0f, 0.015f);
    ApplyTextureAnimationResource(2);
}

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

void AreaKWM1Main(std::uint32_t) {
    HLib::EnqueueTaskWithoutParameter(eKWM1_EVT_KWM1_mainloop_Callback, 4, 0x0b, static_cast<std::int32_t>(TASK_KWM1.name));

    DateTime dateTime{};
    GetCurrentDateTime(dateTime);
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

    HLib::EnqueueTaskWithoutParameter(eKWM1_ET_WAVE_KWM1_Callback, 3, 2, static_cast<std::int32_t>(TASK_KWM1_WAVE.name));
    HLib::Task *const butterflyTask = HLib::EnqueueTaskWithParameter(EVT_MoveButterfly_Callback, 4, 0x0d, sizeof(ButterflyTaskData), TASK_BUTTERFLY);
    auto *const butterflyData = static_cast<ButterflyTaskData *>(HLib::GetTaskParameter(butterflyTask));
    InitializeButterflyTaskData(*butterflyData);

    ConfigureScenarioNodeMotion(TASK_FISH, 0xa003, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
    ConfigureScenarioNodeMotion(TASK_FISH, 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
    EV_MAZ_fishCtrl();
    EV_SetCurrentRefCount(0);
}

void AreaKWM1Cleanup(std::int32_t, std::uint32_t) {

    CheckIsArgEquZero(1);
    EV_ReinitLights(1);
    EV_SetLightFlagForIndex(0, 2);
    EV_SetIndexForLightEntry(0, 1);
    EV_AssignLightParams(1, 1);
    EV_AssignLightParams(1, 2);
}

const GameplayAreaDefinition areaKWM1GameplayDefinition{
    .areaId = AREA_MAZE_FISH, .padding04 = 0, .init = AreaKWM1Init, .main = AreaKWM1Main, .cleanup = AreaKWM1Cleanup};

} // namespace shenmue::s2::areas
