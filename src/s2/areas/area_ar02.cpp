#include "s2/areas/area_ar02.hpp"
#include "s2/areas/area_entities.hpp"
#include "s2/areas/area_pigeons.hpp"
#include "s2/field_collision.hpp"
#include "s2/map_assets.hpp"
#include "s2/motion.hpp"
#include "s2/weather.hpp"
#include "s2/lighting.hpp"

#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/world_render.hpp"

#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstdio>

namespace shenmue::s2::areas {
namespace {

constexpr float NativeFloat(std::uint32_t bits) {
    return std::bit_cast<float>(bits);
}

constexpr std::int32_t nightTimeFlag = 0xff;
constexpr std::int32_t robBagFlag = 0x8b;
constexpr std::int32_t gateFlag = 0x8f;
constexpr std::int32_t waveSuppressedPlayerFlag = 2228;
constexpr std::int32_t disableArmwFlag = 0x3c4;
constexpr shen_name defaultLight{SHEN_NAME(0000)};
constexpr shen_name firstPigeon{SHEN_NAME(hat0)};
constexpr shen_name ryo{SHEN_NAME(RYO_)};
constexpr shen_name gate1{SHEN_NAME(gat1)};
constexpr shen_name gate2{SHEN_NAME(gat2)};
constexpr shen_name armw{SHEN_NAME(armw)};
constexpr shen_name btc1{SHEN_NAME(BTC1)};
constexpr std::int32_t waveTaskToken = static_cast<std::int32_t>(SHEN_NAME(wave));
constexpr std::int32_t springTaskToken = static_cast<std::int32_t>(SHEN_NAME(fntn));

struct AR02State {
    std::int32_t initFlag{};
    std::array<std::uint64_t, 32> distanceFalloff{};
    std::array<std::uint64_t, 32> firstKernel{};
    std::array<std::uint64_t, 32> secondKernel{};
};

static_assert(sizeof(AR02State::distanceFalloff) == 0x100);
static_assert(sizeof(AR02State::firstKernel) == 0x100);
static_assert(sizeof(AR02State::secondKernel) == 0x100);

AR02State state{};

void LoadRobBagPackage() {
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("ROBBAG"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
}

void EnableMapControlMotionSetZero() {
    SetMapControlStatusFlag(0, 1);
}

void LoadAR02Audio(std::int32_t afternoon) {
    if (EV_GetShenmueFlag(0xd3)) {
        EV_LoadAudio("AR02_000", 2);
    } else if (EV_GetShenmueFlag(0xd4)) {
        EV_LoadAudio("AR02_000", 3);
    } else if (EV_GetShenmueFlag(0x7f)) {
        EV_LoadAudio("0061A000", 0);
    } else if (EV_GetShenmueFlag(0x84)) {
        EV_LoadAudio("0061A000", 1);
    } else if (afternoon) {
        EV_LoadAudio("AR02_000", 1);
    } else {
        EV_LoadAudio("AR02_000", 0);
    }
}

void ApplyAR02LightingAndAudio(std::int32_t afternoon) {
    EV_SetModelDataDefaultVertexShader();
    if (afternoon) {
        EV_lightLoad(defaultLight);
        PushGlobalLightingProgram();
        CheckIsArgEquZero(1);

        EV_SetObjectFlagByIndex(12, 1);
        EV_SetObjectFlagByIndex(13, 0);
    } else {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_SetObjectFlagByIndex(12, 0);
        EV_SetObjectFlagByIndex(13, 1);
    }
    if (state.initFlag == 0)
        LoadAR02Audio(afternoon);
}

void PrepareAR02NpcState() {
    std::int32_t npcState = 7;
    if (EV_GetShenmueFlag(9))
        npcState = 0;
    if (EV_GetShenmueFlag(0x0a))
        npcState = 1;
    if (EV_GetShenmueFlag(0x0d))
        npcState = 4;
    if (EV_GetShenmueFlag(0x0e))
        npcState = 5;
    if (EV_GetShenmueFlag(0x1b))
        npcState = 6;

    if (EV_GetShenmueFlag(0x2de))
        EV_LoadNPCRoute("NPC_AR02.BIN");
    if (EV_GetShenmueFlag(0x7f) || EV_GetShenmueFlag(0x84))
        SetNpcColdResetMode(0);

    if (npcState != 7) {
        EV_NPC_NewLoadSw(0);
        EV_ResetFreeTalkFlags();
        ReadMotiFromDirectory(g_actNumber, g_currentMap, "0012E.MOT");
        EV_NPC_NewLoadSw(1);
    }
}

bool IsPlayerInAR02Shelter() {
    if (!(GetWeatherPressure() >= 0.1f))
        return false;
    auto *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (!control->ryo)
        return false;

    Vector3 location{};
    plymUpdatePlayerSearchLocation(&location);
    return location.x > 256.0f && location.x < 268.0f && location.z > 387.0f && location.z < 406.0f;
}

void AR02WaveCallback(void *) {
    SetGlobalShadowFadeRequested(IsPlayerInAR02Shelter());
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, waveSuppressedPlayerFlag)) {
        return;
    }

    const std::int32_t initialFrame = state.initFlag;
    DateTime dateTime{};
    GetCurrentDateTime(dateTime);
    bool applyFrame = ((dateTime.minute & 7) == 0 && dateTime.second == 0) || initialFrame != 0;
    if (!applyFrame) {
        Vector3 position{};
        EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        applyFrame = position.x <= 244.0f || (position.z >= 446.0f && position.x <= 249.0f);
    }
    if (!applyFrame)
        return;

    GetInterpolatedWeatherValues();
    const auto colours = GetWeatherWaveColours();
    const std::uint32_t specular = colours[0], diffuse = colours[1];
    BuildWaveLightingTable(1, 1.0f, 1.0f,
        {static_cast<float>((diffuse >> 16) & 255U) / 255.0f * 0.75f, static_cast<float>((diffuse >> 8) & 255U) / 255.0f * 0.75f,
            static_cast<float>(diffuse & 255U) / 255.0f * 0.8999999761581421f},
        {static_cast<float>((specular >> 16) & 255U) / 255.0f * 0.3100000023841858f, static_cast<float>((specular >> 8) & 255U) / 255.0f * 0.3100000023841858f,
            static_cast<float>(specular & 255U) / 255.0f * 0.30000001192092896f},
        42.540000915527344f, 1.0f);
    SetMapModelSlotEnabled(15, 0);
    SetMapModelSlotEnabled(16, 0);
    AdvanceWaterTextureAnimation();
}

void AR02SpringCallback(void *) {
    SetMapModelSlotEnabled(17, 0);
    SetMapModelSlotEnabled(18, 0);
    AdvanceSpringTextureAnimation();
}

void BuildAR02FftLighting() {
    const std::span<std::byte, 0x100> distance = std::as_writable_bytes(std::span(state.distanceFalloff));
    const std::span<std::byte, 0x100> first = std::as_writable_bytes(std::span(state.firstKernel));
    const std::span<std::byte, 0x100> second = std::as_writable_bytes(std::span(state.secondKernel));
    GenerateFFT(2.0f);
    FillFftDistance(FillFftKernel(NativeFloat(0x41200000U), GenerateDistanceFalloffRowMajor(NativeFloat(0x40053f7dU), distance)));
    void *const firstKernel = FillFftKernel(NativeFloat(0x3d0f5c29U), GenerateDistanceFalloffKernel8x8(NativeFloat(0x3f73b646U), first));
    void *const secondKernel = FillFftKernel(NativeFloat(0x3d6703b0U), GenerateDistanceFalloffKernel8x8(NativeFloat(0x3f70a3d7U), second));
    ApplyFft(secondKernel, firstKernel);
    FinalizeFft(NativeFloat(0x3a83126fU), NativeFloat(0x3a83126fU));
    SetWaveLighting(0.1f, Vector3{NativeFloat(0x3d74f0d8U), NativeFloat(0x3f7a0f91U), NativeFloat(0x3e525461U)});
}

} // namespace

void AreaAR02Init() {
    state.initFlag = 0;
    state.distanceFalloff.fill(0);
    state.firstKernel.fill(0);
    state.secondKernel.fill(0);
    areaPigeons1.fill({});
}

void AreaAR02Main(std::uint32_t) {
    const Vector3 pigeonOrigin{NativeFloat(0x436c4000U), NativeFloat(0x42c8a666U), NativeFloat(0x43d70000U)};
    state.initFlag = 1;
    ApplyAR02LightingAndAudio(EV_GetShenmueFlag(nightTimeFlag));

    if (EV_GetShenmueFlag(robBagFlag))
        LoadRobBagPackage();
    EnableMapControlMotionSetZero();
    QueueAreaPigeons(&pigeonOrigin, 4.5f, areaPigeons1.data(), firstPigeon);

    if (EV_GetShenmueFlag(gateFlag)) {
        EV_SetObjectFlagByIndex(19, 1);
        ToggleFlagOnFieldEntity(gate1, 1);
        ToggleFlagOnFieldEntity(gate2, 1);
    } else {
        EV_SetObjectFlagByIndex(19, 0);
        ToggleFlagOnFieldEntity(gate1, 0);
        ToggleFlagOnFieldEntity(gate2, 0);
    }

    PrepareAR02NpcState();
    areaClearAreaEntities();
    InitializeWaterTextureAnimation(2);
    InitializeSpringTextureAnimation(24);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, waveSuppressedPlayerFlag, 0);
    BuildAR02FftLighting();

    HLib::EnqueueTaskWithoutParameter(AR02WaveCallback, 3, 2, waveTaskToken);
    SetMapModelVertexShaderType(15, 1);
    SetMapModelVertexShaderType(16, 1);
    AR02WaveCallback(nullptr);
    HLib::EnqueueTaskWithoutParameter(AR02SpringCallback, 3, 2, springTaskToken);
    state.initFlag = 0;

    if (EV_GetShenmueFlag(disableArmwFlag))
        ToggleFlagOnFieldEntity(armw, 0);
    flddSetPointFlagOnGrid(btc1, 0);
}

void AreaAR02Cleanup(std::int32_t afternoon, std::uint32_t) {
    ApplyAR02LightingAndAudio(afternoon);
}

const GameplayAreaDefinition areaAR02GameplayDefinition{SHEN_NAME(AR02), 0, AreaAR02Init, AreaAR02Main, AreaAR02Cleanup};

} // namespace shenmue::s2::areas
