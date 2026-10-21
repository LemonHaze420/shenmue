#include "s2/areas/area_wesm.hpp"
#include "s2/pak.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/particle_effects.hpp"
#include "s2/scene_data.hpp"
#include "s2/world_render.hpp"

namespace shenmue::s2::areas {
namespace {
constexpr shen_name fireBreather{SHEN_NAME(MAK_)}, fireProp{SHEN_NAME(MAKI)}, bell{SHEN_NAME(BEEL)}, ryo{SHEN_NAME(RYO_)};
constexpr Vector3 breatherPosition{149.39999389648438f, 102.1500015258789f, 116.45999908447266f};
constexpr Vector3 propOffset{0.07000000029802322f, 0.0f, -0.029999999329447746f};
constexpr Vector3 bellOffset{0.07000000029802322f, -0.07999999821186066f, 0.029999999329447746f};
constexpr Vector3i propRotation{-1, -1, -1};
std::int32_t firePhase{};
std::uint32_t fireId{};
std::int16_t previousFloorMode = 1;
void WESMFireBreatherLoop(void *) {
    MotionWork *work = scenFindMotionwork(fireBreather);
    if (SetMotionRegionFlag(work, 65, 1, MotionRegion::LowerBody))
        EV_AT_SetPos(fireBreather, EventTransformFlags::World | EventTransformFlags::AllAxes, breatherPosition);
    const float frame = GetMotionFrame(scenFindMotionwork(fireBreather), MotionRegion::LowerBody);
    Vector3 position;
    if (firePhase == 0) {
        if (frame > 430.0f) {
            EV_GetCharaPosition(fireBreather, 7, &position, EventTransformFlags::World);
            SetParticleEffectPosition(4, fireId, &position, &position);
            ResumeParticleEffect(4, fireId);
            firePhase = 1;
        }
    } else if (firePhase == 1) {
        if (frame <= 480.0f) {
            EV_GetCharaPosition(fireBreather, 7, &position, EventTransformFlags::World);
            SetParticleEffectPosition(4, fireId, &position, &position);
        } else {
            SuspendParticleEffect(4, fireId);
            firePhase = 2;
        }
    } else if (frame < 400.0f) {
        firePhase = 0;
    }
}
void InitializeWESMFireBreather() {
    HLib::EnqueueTaskWithoutParameter(WESMFireBreatherLoop, 6, 0x0b, 1684627812);
    EV_AT_SetPos(fireBreather, EventTransformFlags::World | EventTransformFlags::AllAxes, breatherPosition);
    EV_AT_SetAngY(fireBreather, EventTransformFlags::AllAxes, Vector3i{0, 16384, 0}, nullptr);
    mtwkRequestMotion(scenFindMotionwork(fireBreather), 0x2644, MotionRegion::LowerBody);
    SetMotionRegionFlag(scenFindMotionwork(fireBreather), 66, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(fireBreather), 0, 0);
    SetMotionEnvironmentEnabled(scenFindMotionwork(fireBreather), 1);
    EV_SetCharaControllerFlags(fireBreather, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(fireBreather, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    AttachScenarioCharacter(fireProp, fireBreather, 14, &propOffset, &propRotation);
    AttachScenarioCharacter(bell, fireBreather, 21, &bellOffset, &propRotation);
    hmdlSetFlagsFor(ctrlGetCharaByName(fireBreather), 0, 0, 16);
    hmdlSetFlagsFor(ctrlGetCharaByName(fireBreather), 1, 3, 16);
    Vector3 position;
    EV_GetCharaPosition(fireProp, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    fireId = static_cast<std::uint32_t>(CreateFireEffect(position, 1.0f, 1.0f, 0, 0));
    SetParticleEffectSize(4, fireId, 0.25f, 0.4000000059604645f);
    ResumeParticleEffect(4, fireId);
    AttachFireEffect(fireId, fireProp, 152, 0.0f);
    EV_GetCharaPosition(fireBreather, 7, &position, EventTransformFlags::World);
    fireId = static_cast<std::uint32_t>(CreateFireEffect(position, 1.0f, 1.0f, 0, 0));
    SetParticleEffectSize(4, fireId, 0.800000011920929f, 0.800000011920929f);
    ResumeParticleEffect(4, fireId);
}
void WESMSpringLoop(void *) {
    SetTextureAnimationUV(0.0f, 0.04500000178813934f);
    ApplyTextureAnimationResource(3);
    SetTextureAnimationUV(0.0f, 0.027000000700354576f);
    ApplyTextureAnimationResource(4);
    Vector3 position;
    EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    const std::int16_t mode = position.y >= 107.77999877929688f ? 2 : 1;
    if (previousFloorMode != mode) {
        SetMapFieldDepthSlotEnabled(5, position.y >= 107.77999877929688f ? 0 : 1);
        previousFloorMode = mode;
    }
}
} // namespace
void AreaWESMInit() {
    firePhase = 0;
    fireId = 0;
}
void AreaWESMMain(std::uint32_t) {
    EV_SetWeatherInstruction(0x780, 0);
    EV_SetWeatherInstruction(0x0c, 0);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    AssignLightProgram(0);
    AssignLightProgram(1);
    SetupLightConfig();
    EV_AssignLightParams(1, 2);
    (void)CheckIsArgEquZero(1);
    HLib::EnqueueTaskWithoutParameter(WESMSpringLoop, 3, 2, 1853124198);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    EV_CheckWarmStartNpcRoute(fireBreather);
    EV_ForceLoadNpc(fireBreather);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    InitializeWESMFireBreather();
    EV_NPC_NewLoadSw(1);
    firePhase = 0;
}
void AreaWESMCleanup(std::int32_t, std::uint32_t) {
}
const GameplayAreaDefinition areaWESMGameplayDefinition{SHEN_NAME(WESM), 0, AreaWESMInit, AreaWESMMain, AreaWESMCleanup};
} // namespace shenmue::s2::areas
