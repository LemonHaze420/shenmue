#include "s2/areas/area_kesa.hpp"
#include "s2/areas/kesa_tables.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/fog.hpp"
#include "s2/hand.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/screen_film.hpp"
#include "s2/search.hpp"
#include <bit>
namespace shenmue::s2::areas {
namespace {
std::int32_t flickerPhase, flickerStep, endingPhase, torchHandDelay;
void UpdateTorchPosition(std::uint32_t light, shen_name torch) {
    if (light == 0xffffffffU)
        return;
    Vector3 root, position{-.01f, .35f, -.013f};
    Vector3i rotation;
    EV_GetCharaPosition(torch, CHARACTER_TRANSLATION_ROOT, &root, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(torch, &rotation, EventTransformFlags::World);
    ApplyScenarioTranslationAndRotation(&root, &rotation, &position);
    SetLightRuntimePosition(light, position);
    PrioritizeLight(static_cast<std::int16_t>(light));
}
void ConfigureTorchHands() {
    auto *character = FindCharacterByScenarioName(KESA_SHENHUA);
    SetHighModelXYZ(character, 0, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(KESA_SHENHUA), 0, 1, 0);
    hmdlSetFlagsFor(FindCharacterByScenarioName(KESA_SHENHUA), 1, 3, 16);
}
void SetupCaveObjects() {
    constexpr shen_name aa33{SHEN_NAME(aa33)}, a704{SHEN_NAME(a704)}, aa36{SHEN_NAME(aa36)}, aa39{SHEN_NAME(aa39)}, aa75{SHEN_NAME(aa75)},
        tama{SHEN_NAME(TAMA)}, aa85{SHEN_NAME(aa85)}, a636{SHEN_NAME(a636)};
    for (shen_name name : {a636, aa33, a704, aa36, aa39, aa75, tama})
        EV_DisableCharacterDisplay(name);
    EV_EnableCharacterDisplay(aa85);
    ToggleFlagOnFieldEntity(aa36, 0);
    srchSetTargetEnabled(SHEN_NAME(MA01), 0);
    if (EV_GetShenmueFlag(690)) {
        for (shen_name name : {aa33, a704, aa36, aa75, tama})
            EV_EnableCharacterDisplay(name);
        EV_SetCharacterScale(tama, {1.01f, 1.01f, 1.01f});
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 2228, 0);
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 436)) {
            EV_AT_SetPos(aa39, EventTransformFlags::World | EventTransformFlags::AllAxes, {115.7349f, 106.585f, -14.9312f});
            EV_AT_SetAngY(aa39, EventTransformFlags::AllAxes, {49152, 32768, 16384});
            EV_EnableCharacterDisplay(aa39);
            EV_AT_SetPos(aa36, EventTransformFlags::World | EventTransformFlags::AllAxes, {125.7248f, 104.72f, 3.8757f});
            EV_AT_SetAngY(aa36, EventTransformFlags::AllAxes, {0, 38021, 0});
            ToggleFlagOnFieldEntity(aa36, 1);
            EV_DisableCharacterDisplay(aa75);
            EV_DisableCharacterDisplay(aa33);
            srchSetTargetEnabled(SHEN_NAME(MA01), 1);
        }
        if (EV_GetShenmueFlag(227)) {
            const Vector3 position{.12f, .27f, -.005f};
            const Vector3i rotation{32768, 0, 49152};
            AttachScenarioCharacter(aa75, KESA_RYO, 14, &position, &rotation);
            EV_EnableCharacterDisplay(tama);
            EV_DisableCharacterDisplay(aa33);
        }
    } else {
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1207)) {
            EV_AT_SetPos(a636, EventTransformFlags::World | EventTransformFlags::AllAxes, {50.4894f, 95.9601f, 114.1168f});
            EV_EnableCharacterDisplay(a636);
        }
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 2228, 1);
    }
}
} // namespace
void KesaResetTorchLightState() {
    flickerPhase = 0;
    flickerStep = 1820;
    endingPhase = 0;
    torchHandDelay = 0;
}
void KesaUpdateTorchLights(std::uint32_t playerLight, std::uint32_t companionLight) {
    UpdateTorchPosition(playerLight, KESA_TORCH);
    UpdateTorchPosition(companionLight, KESA_COMPANION_TORCH);
    const float wave = sin_fixed16(static_cast<std::int16_t>(flickerPhase));
    const float intensity = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1757) ? wave * .05f + .7f : wave * .075f + .725f;
    flickerPhase += flickerStep;
    if (flickerPhase > 65535) {
        flickerPhase = 0;
        flickerStep = shenGetRandomInt32(2048) + 2048;
    }
    if (playerLight != 0xffffffffU)
        SetLightSceneIntensity(playerLight, intensity);
    if (companionLight != 0xffffffffU)
        SetLightSceneIntensity(companionLight, intensity);
    if (kesaAuthorSelection == 7) {
        const float range = (sin_fixed16(static_cast<std::int16_t>(endingPhase)) * .5f + 8) - .5f;
        endingPhase += 1820;
        SetLightRange(9, range);
        SetLightRange(10, range);
        SetLightSceneIntensity(3, intensity);
        SetLightSceneIntensity(8, intensity);
        SetLightSceneIntensity(9, wave * .05f + .7f);
        SetLightSceneIntensity(10, wave * .05f + .7f);
    }
}
void KesaSetupTorch() {
    const Vector3 position{.075f, -.033f, .046f};
    const Vector3i rotation{61895, 0, 62805};
    AttachScenarioCharacter(KESA_TORCH, KESA_RYO, 21, &position, &rotation);
    EV_EnableCharacterDisplay(KESA_TORCH);
    mtwkRequestMotion(scenFindMotionwork(KESA_RYO), 0x2a41, MotionRegion::LeftArm);
    SetMotionRegionFlag(scenFindMotionwork(KESA_RYO), 69, 0, MotionRegion::LeftArm);
    SetMotionSpeed(scenFindMotionwork(KESA_RYO), .1f, MotionRegion::LeftArm);
    hmdlSetFlagsFor(FindCharacterByScenarioName(KESA_RYO), 1, 0, 16);
    const Vector4 colour{1, 1, .5f, .2f};
    const float falloff[]{.7f, 0};
    SetLightFlagForIndex(32, 3);
    SetLightRuntimeColour(32, colour);
    SetLightRange(32, 5);
    SetLightFalloff(32, falloff);
    SetLightEntryIndex(32, 1);
    CopyLightRuntimeEntry(13, 32);
    CopyLightRuntimeEntry(14, 32);
    SetLightEntryIndex(13, 1);
    SetLightEntryIndex(14, 1);
    SetLightEntryIndex(32, 0);
    SetLightEntryIndex(32, 0);
}
void KesaSetLight(void *) {
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1757))
        return;
    SetMotionSpeed(scenFindMotionwork(KESA_RYO), .1f, MotionRegion::LeftArm);
    if (DoesScenarioCharacterExist(KESA_SHENHUA)) {
        if (GetMotionAnimationId(scenFindMotionwork(KESA_SHENHUA), MotionRegion::LeftArm) != 0x2a41) {
            SetNpcRouteControlHold(KESA_SHENHUA, 1);
            mtwkRequestMotion(scenFindMotionwork(KESA_SHENHUA), 0x2a41, MotionRegion::LeftArm);
            SetMotionRegionFlag(scenFindMotionwork(KESA_SHENHUA), 69, 0, MotionRegion::LeftArm);
            EV_AT_SetAngY(KESA_COMPANION_TORCH, EventTransformFlags::AllAxes, {0, 0, 0});
            ConfigureTorchHands();
            hmdlSetFlagsFor(FindCharacterByScenarioName(KESA_SHENHUA), 0, 0, 16);
            torchHandDelay = 5;
            if (EV_GetShenmueFlag(732)) {
                mtwkRequestMotion(scenFindMotionwork(KESA_SHENHUA), 0x9505, MotionRegion::RightArm);
                SetMotionRegionFlag(scenFindMotionwork(KESA_SHENHUA), 69, 0, MotionRegion::RightArm);
                hmdlSetFlagsFor(FindCharacterByScenarioName(KESA_SHENHUA), 0, 2, 16);
            }
        }
        SetMotionSpeed(scenFindMotionwork(KESA_SHENHUA), .1f, MotionRegion::LeftArm);
        if (torchHandDelay >= 0 && --torchHandDelay == 0) {
            ConfigureTorchHands();
            hmdlSetFlagsFor(FindCharacterByScenarioName(KESA_SHENHUA), 0, EV_GetShenmueFlag(732) ? 0 : 2, 16);
        }
        Vector3 position{.07f, 0, .03f};
        Vector3i rotation{62624, 0, 62988};
        TransformScenarioMotionBonePose(KESA_SHENHUA, 21, &position, &rotation);
        EV_AT_SetPos(KESA_COMPANION_TORCH, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
        EV_AT_SetAngY(KESA_COMPANION_TORCH, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
        EV_EnableCharacterDisplay(KESA_COMPANION_TORCH);
        if (EV_GetShenmueFlag(732)) {
            position = {.08f, 0, -.02f};
            rotation = {364, 0, 14196};
            TransformScenarioMotionBonePose(KESA_SHENHUA, 14, &position, &rotation);
            EV_AT_SetPos(SHEN_NAME(a636), EventTransformFlags::World | EventTransformFlags::AllAxes, position);
            EV_AT_SetAngY(SHEN_NAME(a636), EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
            EV_EnableCharacterDisplay(SHEN_NAME(a636));
        }
    } else {
        EV_AT_SetPos(KESA_COMPANION_TORCH, EventTransformFlags::World | EventTransformFlags::AllAxes, {0, 0, 0});
        if (EV_GetShenmueFlag(732))
            EV_AT_SetPos(SHEN_NAME(a636), EventTransformFlags::World | EventTransformFlags::AllAxes, {0, 0, 0});
    }
    KesaUpdateTorchLights(13, 14);
}
void KesaSetupLighting() {
    EV_SetWeatherInstruction(0x780, 0);
    EV_SetWeatherInstruction(12, 0);
    EV_AssignLightParams(1, 6);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(SHEN_NAME(0000));
    for (auto index : {7U, 8U, 12U, 13U})
        AssignLightProgram(index);
    EV_lightLoad(SHEN_NAME(0002));
    AssignLightProgram(10);
    if (EV_GetShenmueFlag(690)) {
        const CrossParameters parameters = cachedCrossParameters;
        UploadCrossParams(parameters);
        EV_AT_SetPos(SHEN_NAME(aa85), EventTransformFlags::AllAxes, {59.5759f, 100.7594f, 37.8922f});
        EV_AT_SetAngY(SHEN_NAME(aa85), EventTransformFlags::AllAxes, {0, 56433, 0});
        EV_lightLoad(SHEN_NAME(0000));
        constexpr std::uint8_t flags[]{0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1};
        for (std::int32_t index = 0; index < 17; ++index)
            EV_SetObjectFlagByIndex(index, flags[index]);
        for (std::uint32_t index = 1; index <= 12; ++index)
            SetLightEntryIndex(index, kesaEarlyLightIndexes[index - 1]);
        for (std::uint32_t index = 16; index <= 96; ++index)
            SetLightEntryIndex(index, kesaRoomLightIndexes[index - 16]);
    } else {
        const GlowParameters parameters = cachedGlowParameters;
        UploadGlowParams(parameters);
        EV_AT_SetPos(SHEN_NAME(aa85), EventTransformFlags::AllAxes, {71.6074f, 98.78f, 70.0309f});
        EV_AT_SetAngY(SHEN_NAME(aa85), EventTransformFlags::AllAxes, {0, 57344, 0});
        constexpr std::uint8_t flags[]{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0};
        for (std::int32_t index = 0; index < 17; ++index)
            EV_SetObjectFlagByIndex(index, flags[index]);
        SetLightEntryIndex(33, 1);
        if (EV_GetShenmueFlag(462)) {
            SetLightEntryIndex(45, 1);
            SetLightEntryIndex(46, 1);
        }
    }
    SetFogRed(20);
    SetFogGreen(100);
    SetFogBlue(.5f);
    SetFogDistance(0xffb9d3dfU);
    ReinitFogTasks(1);
    SetupCaveObjects();
    KesaSetupTorch();
}
} // namespace shenmue::s2::areas
