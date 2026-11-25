#include "s2/areas/area_krm3.hpp"
#include "s2/events/event_api.h"
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
#include "s2/event_utility_dc.h"
enum {
    shenhua = SHEN_NAME4('S', 'I', 'N', '_'),
    relativeXyz = 0x38000000,
    shenhuaAreaMotion = 0x9502,
    startTaskToken = SHEN_NAME4('k', 'r', 'm', '3'),
    waveTaskToken = SHEN_NAME4('w', 'a', 'v', 'e'),
    sleepTaskToken = SHEN_NAME4('s', 'l', 'e', 'p')
};
shen_name krm3Characters[] = {shenhua, 0xffffffff};
#else
#include "s2/areas/area_krm3.hpp"
#include "s2/character.hpp"
#include "s2/motion_work.hpp"

#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/world_render.hpp"

#include <array>

namespace shenmue::s2::areas {
namespace {

constexpr shen_name shenhua{SHEN_NAME(SIN_)};
constexpr std::array<shen_name, 2> npcLoadCharacters{shenhua, shen_name{0xffffffffU}};
constexpr std::int32_t startTaskToken = static_cast<std::int32_t>(SHEN_NAME(krm3));
constexpr std::int32_t waveTaskToken = static_cast<std::int32_t>(SHEN_NAME(wave));
constexpr std::int32_t sleepTaskToken = static_cast<std::int32_t>(SHEN_NAME(slep));
constexpr shen_name loadTaskToken{SHEN_NAME(load)};
constexpr std::uint32_t relativeXyz = 0x38000000U;
constexpr std::uint32_t shenhuaAreaMotion = 0x9502U;
constexpr Vector3 fftLightingFlags{0.30000001f, 0.80000001f, 0.5f};

void KRM3WaitCallback(void *data);
void KRM3StartCallback(void *data);
void KRM3WaveCallback(void *data);
void KRM3NonSleepReadyCallback(void *data);
} // namespace

void AreaKRM3Init() {}
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters)
#else
void AreaKRM3Main(std::uint32_t startParameters)
#endif
{
    HLibTask *startTask = 0;
#if defined(SHENMUE_EVENT_OVERLAY)
    EventDateTime dateTime;
    float distance[64], first[64], second[64];
#endif
    if (startParameters == 0)
        startTask = FTBL.HLib_EnqueueTaskWithoutParameter(KRM3StartCallback, 4, 0x0b, startTaskToken);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.UpdateWeatherForMinute(0, 0, 0, 0.0f);
    FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
    FTBL.GetCurrentDateTime((DateTime *)&dateTime);
    AreaCleanup(0);
    {
        Vector3 lighting = {0.3f, 0.8f, 0.5f};
        void *firstKernel, *secondKernel;
        FTBL.GenerateFFT(0.5f);
        FTBL.fft_fill_dist(FTBL.fft_fill_krn(3.92f, FTBL.generate_distance_falloff_row_major(1.29f, distance)));
        firstKernel = FTBL.fft_fill_krn(0.06f, FTBL.generate_distance_falloff_kernel_8x8(1.035f, first));
        secondKernel = FTBL.fft_fill_krn(0.073f, FTBL.generate_distance_falloff_kernel_8x8(0.564f, second));
        FTBL.fft_bins_load_pairwise_tables(firstKernel, secondKernel);
        FTBL.FFTUtil(0.0f, 0.001f);
        FTBL.set_lighting_flags(1.0f, &lighting);
#else
    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    DateTime dateTime{};
    shenGetCurrentDateTime(dateTime);
    (void)CheckIsArgEquZero(1);
    EV_ReinitLights(1);
    EV_SetLightFlagForIndex(0, 2);
    EV_SetIndexForLightEntry(0, 1);
    std::array<std::byte, 0x100> distanceWork, firstWork, secondWork;
    GenerateFFT(0.5f);
    FillFftDistance(FillFftKernel(3.92f, GenerateDistanceFalloffRowMajor(1.29f, distanceWork)));
    void *const secondKernel = FillFftKernel(0.073f, GenerateDistanceFalloffKernel8x8(0.564f, secondWork));
    void *const firstKernel = FillFftKernel(0.06f, GenerateDistanceFalloffKernel8x8(1.035f, firstWork));
    ApplyFft(firstKernel, secondKernel);
    FinalizeFft(0.0f, 0.001f);
    SetWaveLighting(1.0f, fftLightingFlags);
#endif
        FTBL.HLib_EnqueueTaskWithoutParameter(KRM3WaveCallback, 3, 2, waveTaskToken);
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.HLib_SetupTimedTask(FTBL.HLib_EnqueueTaskWithoutParameter(KRM3NonSleepReadyCallback, 4, 0x0b, sleepTaskToken), 2);
#else
        HLib::Task *const readyTask = FTBL.HLib_EnqueueTaskWithoutParameter(KRM3NonSleepReadyCallback, 4, 0x0b, sleepTaskToken);
        FTBL.HLib_SetupTimedTask(readyTask, 2);
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
        if (startParameters == 0) {
            FTBL.EV_NPC_NewLoadSw_0(0);
            Queue_EVT_UTL_LoadNPCChara_3(krm3Characters, startTask);
            FTBL.HLib_SetupTimedTask(startTask, 0);
        } else {
            FTBL.PTYCUtils(shenhua, 3, 1);
        }
    }
#else
    if (startParameters != 0) {
        EV_PTYCUtils(shenhua, 3, 1);
        return;
    }
    EV_NPC_NewLoadSw(0);
    AssignAreaNpcLoadContext(npcLoadCharacters.data(), startTask);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    if (startTask)
        HLib::SetupTimedTask(startTask, 0);
    HLib::EnqueueTaskWithoutParameter(EVT_UTL_LoadNPCChara_Callback_2, 6, 0x0b, static_cast<std::int32_t>(loadTaskToken.name));
    HLib::SetupTimedTask(startTask, 0);
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon)
#else
void AreaKRM3Cleanup(std::int32_t, std::uint32_t)
#endif
{
    (void)FTBL.CheckIsArgEquZero(1, 1);
    FTBL.ReinitializeLights(1);
    FTBL.SetLightFlagForIndex(0, 2);
    FTBL.SetLightEntryIndex(0, 1);
}

#if !defined(SHENMUE_EVENT_OVERLAY)
namespace {
#endif
void KRM3NonSleepReadyCallback(void *data)
{
    FTBL.EV_setHoldStatesFlags(0);
    FTBL.HLib_DestroyCurrentTask();
}

void KRM3StartCallback(void *data)
{
#if defined(SHENMUE_EVENT_OVERLAY)
    Vector3 position = {381.0f, 100.0f, 156.0f};
    Vector3i rotation = {0, 12288, 0};
#else
    const Vector3 position{381.0f, 100.0f, 156.0f};
    const Vector3i rotation{0, 12288, 0};
#endif
    FTBL.ActivatePartyCompanion(shenhua);
    FTBL.SetPlayerControlEnabled(0);
    FTBL.PTYCUtils(shenhua, 3, 1);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.EV_AT_SetPos(shenhua, relativeXyz, &position);
    FTBL.EV_AT_SetAngY_0(shenhua, relativeXyz, &rotation, 0);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(shenhua), 17, 0);
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(shenhua), 71, 0, 0);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(shenhua), 13, 0);
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(shenhua), shenhuaAreaMotion, 0);
#else
    EV_AT_SetPos(shenhua, relativeXyz, position);
    EV_AT_SetAngY(shenhua, relativeXyz, rotation);
    mtwkSetExtraFlag(scenFindMotionwork(shenhua), 17, 0);
    mtwkSetRegionFlag(scenFindMotionwork(shenhua), 71, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(shenhua), 13, 0);
    mtwkRequestMotion(scenFindMotionwork(shenhua), shenhuaAreaMotion, MotionRegion::LowerBody);
#endif
    FTBL.PTYCUtils(shenhua, 3, 0);
    FTBL.shenToggleAreaFlags(3);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.SetPlayerGameFlags(64, 1);
#else
    SetPlayerGameFlags(PlayerGameFlagMode::RetainMovementMode, reinterpret_cast<void *>(1));
#endif
    FTBL.HLib_SetCurrentTaskTo(KRM3WaitCallback);
    FTBL.HLib_SetupTaskDelay(60);
}

void KRM3WaitCallback(void *data)
{
    if (FTBL.IsScenarioBusy()) {
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.DebugLogToFileRemoved("player mode clear\n");
#endif
        FTBL.shenToggleAreaFlags(0);
        FTBL.HLib_DestroyCurrentTask();
    }
}

void KRM3WaveCallback(void *data)
{
#if defined(SHENMUE_EVENT_OVERLAY)
    float white = 1.0f;
    FTBL.BuildLightingTableFrom(0, white, white, white, white, white, white, white, white, 100.0f, white);
    FTBL.ApplyFFTWaveformToStripTree(FTBL.GetMapModelRoot(2));
    FTBL.nlSetTexanimUV(0, 0.015f);
    FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(3));
    FTBL.nlSetTexanimUV(0, 0.00050000002f);
    FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(1));
#else
    BuildWaveLightingTable(0, 1.0f, 1.0f, {0.8f, 0.8f, 0.95f}, {0.5f, 0.5f, 0.5f}, 100.0f, 1.0f);
    ApplyWaveTextureResource(2);
    SetTextureAnimationUV(0.0f, 0.015f);
    ApplyTextureAnimationResource(3);
    SetTextureAnimationUV(0.0f, 0.00050000002f);
    ApplyTextureAnimationResource(1);
#endif
}

#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace
const GameplayAreaDefinition areaKRM3GameplayDefinition{SHEN_NAME(KRM3), 0, AreaKRM3Init, AreaKRM3Main, AreaKRM3Cleanup};
} // namespace shenmue::s2::areas
#endif
