#include "s2/areas/area_wecf.hpp"

#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"
#include "s2/lighting.hpp"

namespace shenmue::s2::areas {

// @verified - p
void AreaWECFInit() {
}

// @verified - p
void AreaWECFMainLoop(void *) {
}

// @verified - p
void AreaWECFMain(std::uint32_t) {
    HLib::EnqueueTaskWithoutParameter(AreaWECFMainLoop, 4, 0x0b, static_cast<std::int32_t>(EVENT_CHARACTER_TASK.name));
    (void)EV_GetShenmueFlag(0xffU);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    (void)CheckIsArgEquZero(1);
    HLib::EnqueueTaskWithoutParameter(JukeboxInitCallback, 5, 0x0b, static_cast<std::int32_t>(EVENT_CHARACTER_TASK.name));
}

// @verified - p
void AreaWECFCleanup(std::int32_t, std::uint32_t) {
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    (void)CheckIsArgEquZero(1);
}

// @verified - p
void JukeboxInitCallback(void *) {
    if (GetNpcNewLoadState() != 1)
        return;
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("JUKB"), 0, 0) != 0)
        return;

    (void)EV_NPC_NewLoadSw(0);
    (void)EV_ResetFreeTalkFlags();
    const char *const packagePath = Filepath_Gen(FilepathGenMode::PackDirectory);
    if (EV_ControlPackageResources(
            PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(packagePath), reinterpret_cast<std::uintptr_t>("JUKB"), 0) != 0) {
        (void)EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0, 0);
        HLib::EnqueueTaskWithoutParameter(JukeboxRevolveCallback, 6, 0x0b, static_cast<std::int32_t>(EVENT_CHARACTER_TASK.name));
        Vector3 zero{};
        EV_scenApplyPosOrRotToPartOf(CHARACTER_WECF_JUKEBOX, 161, &zero, nullptr, EventCharacterPartTransformMode::Set);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 470, 0);
        HLib::DestroyCurrentTask();
    }
    (void)EV_NPC_NewLoadSw(1);
}

// @verified - p
void JukeboxRevolveCallback(void *) {
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 470) == 0)
        return;
    Vector3i rotation{.x = -1601, .y = 0, .z = 0};
    EV_scenApplyPosOrRotToPartOf(CHARACTER_WECF_JUKEBOX, 155, nullptr, &rotation, EventCharacterPartTransformMode::Relative);
    rotation = {.x = 0, .y = -1601, .z = 0};
    EV_scenApplyPosOrRotToPartOf(CHARACTER_WECF_JUKEBOX, 157, nullptr, &rotation, EventCharacterPartTransformMode::Relative);
}

const GameplayAreaDefinition areaWECFGameplayDefinition{
    .areaId = AREA_WECF,
    .padding04 = 0,
    .init = &AreaWECFInit,
    .main = &AreaWECFMain,
    .cleanup = &AreaWECFCleanup,
};

} // namespace shenmue::s2::areas
