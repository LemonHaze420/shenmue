#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_wesm.hpp"

#if defined(SHENMUE_EVENT_WESM_NPCS)
static Vector3 breatherPosition = {149.4f, 102.15f, 116.46f};
static Vector3 propOffset = {0.07f, 0.0f, -0.03f};
static Vector3 bellOffset = {0.07f, -0.08f, 0.03f};
static Vector3i breatherRotation = {0, 16384, 0};
static Vector3i propRotation = {0, 0, 0};
static int firePhase;
static unsigned int fireId;
#endif

#if !defined(SHENMUE_EVENT_WESM_NPCS)
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static unsigned short previousFloorMode = 1;
#endif
void AreaMain(unsigned int startParameters)
{
    FTBL.EV_SetWeatherInstruction(0x780, 0);
    FTBL.EV_SetWeatherInstruction(12, 0);
    FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    FTBL.AssignLightProgram(0);
    FTBL.AssignLightProgram(1);
    FTBL.SetupLightConfig();
    FTBL.AssignLightParams(1, 2);
    FTBL.CheckIsArgEquZero(1, 1);
    FTBL.HLib_EnqueueTaskWithoutParameter(WESMSpringLoop, 3, 2, SHEN_NAME4('f', 'n', 't', 'n'));
    LoadWESMNpcs();
}

void AreaCleanup(int afternoon)
{
}

void WESMSpringLoop(void *taskData)
{
    FTBL.nlSetTexanimUV(0.0f, 0.045f);
    FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(3));
    FTBL.nlSetTexanimUV(0.0f, 0.027f);
    FTBL.RecursiveApplyTexAnimUVs(FTBL.GetMapModelRoot(4));
#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    WESMUpdateFloor();
#endif
}

#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void WESMUpdateFloor(void)
{
    Vector3 position;
    int mode, enabled;
    FTBL.GetCharaPosition(SHEN_NAME4('R', 'Y', 'O', '_'), 0xffffffff, &position, 0x40000000);
    if (position.y < 107.78f) {
        mode = 1;
        enabled = 1;
    } else {
        mode = 2;
        enabled = 0;
    }
    if (previousFloorMode != mode) {
        FTBL.SetMapFieldDepthSlotEnabled(enabled, 5);
        previousFloorMode = mode;
    }
}
#endif

#else
void LoadWESMNpcs(void)
{
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    FTBL.CheckWarmStartNpcRoute(WESM_BREATHER);
    FTBL.ForceLoadNpc(WESM_BREATHER);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    InitializeWESMFireBreather();
    FTBL.EV_NPC_NewLoadSw_0(1);
    firePhase = 0;
}

void InitializeWESMFireBreather(void)
{
    Vector3 position;
    FTBL.HLib_EnqueueTaskWithoutParameter(WESMFireBreatherLoop, 6, 11, SHEN_NAME4('d', 'a', 'i', 'd'));
    do {
        FTBL.EV_AT_SetPos(WESM_BREATHER, 0x78000000, &breatherPosition);
        FTBL.EV_AT_SetAngY_0(WESM_BREATHER, 0x38000000, &breatherRotation, 0);
    } while (0);
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(WESM_BREATHER), 0x2644, 0);
    FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(WESM_BREATHER), 66, 0, 0);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(WESM_BREATHER), 0, 0);
    FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(WESM_BREATHER), 1);
    do {
        FTBL.EV_SetCharaControllerFlags(WESM_BREATHER, 2, 0);
        FTBL.EV_SetCharaControllerFlags(WESM_BREATHER, 3, 0);
    } while (0);
    FTBL.AttachScenarioCharacter(WESM_PROP, WESM_BREATHER, 14, &propOffset, &propRotation);
    FTBL.AttachScenarioCharacter(WESM_BELL, WESM_BREATHER, 21, &bellOffset, &propRotation);
    FTBL.hmdlSetFlagsFor(FTBL.ctrlGetCharaByName(WESM_BREATHER), 0, 0, 16);
    FTBL.hmdlSetFlagsFor(FTBL.ctrlGetCharaByName(WESM_BREATHER), 1, 3, 16);
    FTBL.GetCharaPosition(WESM_PROP, 0xffffffff, &position, 0x40000000);
    fireId = FTBL.CreateFireEffect(&position, 1.0f, 1.0f, 0, 0);
    FTBL.SetParticleEffectSize(4, fireId, 0.25f, 0.4f);
    FTBL.ResumeParticleEffect(4, fireId);
    FTBL.AttachFireEffect(fireId, WESM_PROP, 152, 0.0f);
    FTBL.GetCharaPosition(WESM_BREATHER, 7, &position, 0x40000000);
    fireId = FTBL.CreateFireEffect(&position, 1.0f, 1.0f, 0, 0);
    FTBL.SetParticleEffectSize(4, fireId, 0.8f, 0.8f);
    FTBL.ResumeParticleEffect(4, fireId);
}

void WESMFireBreatherLoop(void *taskData)
{
    Vector3 position;
    float frame;
    if (FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(WESM_BREATHER), 65, 1, 0))
        FTBL.EV_AT_SetPos(WESM_BREATHER, 0x78000000, &breatherPosition);
    frame = FTBL.GetMotionFrame(FTBL.scenFindMotionwork(WESM_BREATHER), 0);
    switch (firePhase) {
    case 0:
        if (frame > 430.0f) {
            FTBL.GetCharaPosition(WESM_BREATHER, 7, &position, 0x40000000);
            FTBL.SetParticleEffectPosition(4, fireId, &position, &position);
            FTBL.ResumeParticleEffect(4, fireId);
            firePhase = 1;
        }
        break;
    case 1:
        if (frame > 480.0f) {
            FTBL.SuspendParticleEffect(4, fireId);
            firePhase = 2;
        } else {
            FTBL.GetCharaPosition(WESM_BREATHER, 7, &position, 0x40000000);
            FTBL.SetParticleEffectPosition(4, fireId, &position, &position);
        }
        break;
    default:
        if (frame < 400.0f)
            firePhase = 0;
        break;
    }
}
#endif
#else
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
    if (mtwkSetRegionFlag(work, 65, 1, MotionRegion::LowerBody))
        EV_AT_SetPos(fireBreather, EventTransformFlags::World | EventTransformFlags::AllAxes, breatherPosition);
    const float frame = mtwkGetFrame(scenFindMotionwork(fireBreather), MotionRegion::LowerBody);
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
    mtwkSetRegionFlag(scenFindMotionwork(fireBreather), 66, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(fireBreather), 0, 0);
    mtwkSetEnabled(scenFindMotionwork(fireBreather), 1);
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
        SetMapFieldDepthSlotEnabled(position.y >= 107.77999877929688f ? 0 : 1, 5);
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
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    InitializeWESMFireBreather();
    EV_NPC_NewLoadSw(1);
    firePhase = 0;
}
void AreaWESMCleanup(std::int32_t, std::uint32_t) {
}
const GameplayAreaDefinition areaWESMGameplayDefinition{SHEN_NAME(WESM), 0, AreaWESMInit, AreaWESMMain, AreaWESMCleanup};
} // namespace shenmue::s2::areas
#endif
