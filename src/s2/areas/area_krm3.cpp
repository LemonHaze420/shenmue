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
constexpr shen_name startTaskToken{SHEN_NAME(krm3)};
constexpr shen_name waveTaskToken{SHEN_NAME(wave)};
constexpr shen_name sleepTaskToken{SHEN_NAME(slep)};
constexpr shen_name loadTaskToken{SHEN_NAME(load)};
constexpr std::uint32_t relativeXyz = 0x38000000U;
constexpr std::uint32_t shenhuaAreaMotion = 0x9502U;
constexpr Vector3 fftLightingFlags{0.30000001f, 0.80000001f, 0.5f};

void KRM3WaitCallback(void *) {
    if (IsScenarioBusy()) {
        shenToggleAreaFlags(0);
        HLib::DestroyCurrentTask();
    }
}

void KRM3StartCallback(void *) {
    const Vector3 position{381.0f, 100.0f, 156.0f};
    const Vector3i rotation{0, 12288, 0};

    EV_ActivatePartyCompanion(shenhua);
    SetPlayerControlEnabled(0);
    EV_PTYCUtils(shenhua, 3, 1);
    EV_AT_SetPos(shenhua, relativeXyz, position);
    EV_AT_SetAngY(shenhua, relativeXyz, rotation);
    mtwkSetExtraFlag(scenFindMotionwork(shenhua), 17, 0);
    SetMotionRegionFlag(scenFindMotionwork(shenhua), 71, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(shenhua), 13, 0);
    mtwkRequestMotion(scenFindMotionwork(shenhua), shenhuaAreaMotion, MotionRegion::LowerBody);
    EV_PTYCUtils(shenhua, 3, 0);
    shenToggleAreaFlags(3);
    SetPlayerGameFlags(PlayerGameFlagMode::RetainMovementMode, reinterpret_cast<void *>(1));
    HLib::SetCurrentTaskTo(KRM3WaitCallback);
    HLib::SetupTaskDelay(60);
}

void KRM3WaveCallback(void *) {
    BuildWaveLightingTable(0, 1.0f, 1.0f, {0.8f, 0.8f, 0.95f}, {0.5f, 0.5f, 0.5f}, 100.0f, 1.0f);
    ApplyWaveTextureResource(2);
    SetTextureAnimationUV(0.0f, 0.015f);
    ApplyTextureAnimationResource(3);
    SetTextureAnimationUV(0.0f, 0.00050000002f);
    ApplyTextureAnimationResource(1);
}

void KRM3NonSleepReadyCallback(void *) {
    EV_setHoldStatesFlags(0);
    HLib::DestroyCurrentTask();
}

} // namespace

void AreaKRM3Init() {
}

void AreaKRM3Main(std::uint32_t startParameters) {
    HLib::Task *startTask = nullptr;
    if (startParameters == 0) {
        startTask = HLib::EnqueueTaskWithoutParameter(KRM3StartCallback, 4, 0x0b, static_cast<std::int32_t>(startTaskToken.name));
    }

    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    DateTime dateTime{};
    GetCurrentDateTime(dateTime);
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
    HLib::EnqueueTaskWithoutParameter(KRM3WaveCallback, 3, 2, static_cast<std::int32_t>(waveTaskToken.name));
    HLib::Task *const readyTask = HLib::EnqueueTaskWithoutParameter(KRM3NonSleepReadyCallback, 4, 0x0b, static_cast<std::int32_t>(sleepTaskToken.name));
    HLib::SetupTimedTask(readyTask, 2);

    if (startParameters != 0) {
        EV_PTYCUtils(shenhua, 3, 1);
        return;
    }

    EV_NPC_NewLoadSw(0);
    AssignAreaNpcLoadContext(npcLoadCharacters.data(), startTask);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    if (startTask) {
        HLib::SetupTimedTask(startTask, 0);
    }
    HLib::EnqueueTaskWithoutParameter(EVT_UTL_LoadNPCChara_Callback_2, 6, 0x0b, static_cast<std::int32_t>(loadTaskToken.name));
    HLib::SetupTimedTask(startTask, 0);
}

void AreaKRM3Cleanup(std::int32_t, std::uint32_t) {

    (void)CheckIsArgEquZero(1);
    EV_ReinitLights(1);
    EV_SetLightFlagForIndex(0, 2);
    EV_SetIndexForLightEntry(0, 1);
}

const GameplayAreaDefinition areaKRM3GameplayDefinition{SHEN_NAME(KRM3), 0, AreaKRM3Init, AreaKRM3Main, AreaKRM3Cleanup};

} // namespace shenmue::s2::areas
