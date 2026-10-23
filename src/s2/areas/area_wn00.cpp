#include "s2/debug.hpp"
#include "s2/areas/area_wn00.hpp"
#include "s2/areas/area_entities.hpp"
#include "s2/areas/area_gambling.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/door.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/npc_package.hpp"
namespace shenmue::s2::areas {
namespace {
Vector3i modelRotation{};
std::int32_t entranceMirror{}, sideMirror{};
constexpr CharacterMirrorPlane middlePlane{{218.6999969482422f, 104.5f, 170.1999969482422f}, {1.0f, 0.0f, 0.0f}};
constexpr CharacterMirrorPlane entrancePlane{{216.0f, 104.5f, 174.64999389648438f}, {1.0f, 0.0f, 0.0f}};
constexpr CharacterMirrorPlane leftPlane{{218.60000610351562f, 104.5f, 159.89999389648438f}, {0.0f, 0.0f, 1.0f}};
void ToggleEntranceMirror(std::uint64_t) {
    const Vector3i rotation{};
    if (entranceMirror) {
        SetCharacterMirrorPlane(nullptr, &rotation);
        SetMapModelAlternateTraversalEnabled(0, 5);
        entranceMirror = 0;
    } else {
        SetCharacterMirrorPlane(&entrancePlane, &rotation);
        SetMapModelAlternateTraversalEnabled(1, 5);
        entranceMirror = 1;
    }
}
void ToggleMiddleMirror(std::uint64_t) {
    const Vector3i rotation{};
    if (sideMirror) {
        SetCharacterMirrorPlane(nullptr, &rotation);
        SetMapModelAlternateTraversalEnabled(0, 5);
        sideMirror = 0;
    } else {
        SetCharacterMirrorPlane(&middlePlane, &rotation);
        SetMapModelAlternateTraversalEnabled(1, 5);
        sideMirror = 1;
    }
}
void ToggleLeftMirror(std::uint64_t) {
    const Vector3i rotation{};
    if (sideMirror) {
        SetCharacterMirrorPlane(nullptr, &rotation);
        SetMapModelAlternateTraversalEnabled(0, 5);
        sideMirror = 0;
    } else {
        SetCharacterMirrorPlane(&leftPlane, &rotation);
        SetMapModelAlternateTraversalEnabled(1, 5);
        sideMirror = 1;
    }
}
void WN00MainLoop(void *) {
    EV_scenApplyPosOrRotToPartOf(SHEN_NAME(kur0), 152, nullptr, &modelRotation, EventCharacterPartTransformMode::Relative);
    EV_scenApplyPosOrRotToPartOf(SHEN_NAME(kur1), 152, nullptr, &modelRotation, EventCharacterPartTransformMode::Relative);
    EV_scenApplyPosOrRotToPartOf(SHEN_NAME(kur2), 152, nullptr, &modelRotation, EventCharacterPartTransformMode::Relative);
    if (EV_GetShenmueFlag(0x20d)) {
        EnableScenarioCharacterDisplay(SHEN_NAME(sak1));
        EnableScenarioCharacterDisplay(SHEN_NAME(sak2));
    } else {
        DisableScenarioCharacterDisplay(SHEN_NAME(sak1));
        DisableScenarioCharacterDisplay(SHEN_NAME(sak2));
    }
}
void ApplyLighting(std::int32_t night) {
    if (night) {
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        PushGlobalLightingProgram();
        SetupLightConfig();
        CheckIsArgEquZero(1);
        EV_SetObjectFlagByIndex(3, 0);
        EV_SetObjectFlagByIndex(4, 1);
    } else {
        SetDefaultVS();
        SetMapModelDataDefaultVertexShader();
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_SetObjectFlagByIndex(3, 1);
        EV_SetObjectFlagByIndex(4, 0);
    }
    EV_LoadAudio("WN00_000", night ? 1 : 0);
}
} // namespace
void AreaWN00Init() {
    modelRotation = Vector3i{0, 3640, 0};
}
void AreaWN00Main(std::uint32_t startParameters) {
    HLib::EnqueueTaskWithoutParameter(WN00MainLoop, 4, 0x0b, 1852399981);
    ApplyLighting(EV_GetShenmueFlag(FLAG_NIGHT_TIME_ENFORCED));
    SetGamble();
    areaClearAreaEntities();
    entranceMirror = 0;
    sideMirror = 0;
    if (startParameters) {
        switch (GetCurrentAuthorId().name) {
        case SHEN_NAME(WNT0):
            ToggleEntranceMirror(0);
            break;
        case SHEN_NAME(WNXL):
            ToggleLeftMirror(0);
            break;
        case SHEN_NAME(WNXM):
            ToggleMiddleMirror(0);
            break;
        }
    }
    SetDoorSubjectCallback(SHEN_NAME(R124), ToggleEntranceMirror);
    SetDoorSubjectCallback(SHEN_NAME(R160), ToggleMiddleMirror);
    SetDoorSubjectCallback(SHEN_NAME(R158), ToggleLeftMirror);
    if (EV_GetShenmueFlag(0x6f))
        SetNpcColdResetMode(0);
}
void AreaWN00Cleanup(std::int32_t night, std::uint32_t) {
    ApplyLighting(night);
}
const GameplayAreaDefinition areaWN00GameplayDefinition{SHEN_NAME(WN00), 0, AreaWN00Init, AreaWN00Main, AreaWN00Cleanup};
} // namespace shenmue::s2::areas
