#include "s2/areas/area_jukebox.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"

namespace shenmue::s2 {
namespace {
inline constexpr shen_name JUKEBOX_CHARACTER{SHEN_NAME(JBOX)};
}

void EVT_JB_InitWk_Callback(void *) {
    if (GetNpcNewLoadState() != 1)
        return;
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("JUKB"), 0))
        return;
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    const char *const directory = Filepath_Gen(FilepathGenMode::PackDirectory);
    if (EV_ControlPackageResources(
            PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>("JUKB"))) {
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        HLib::EnqueueTaskWithoutParameter(EVT_Juke_revolve_Callback, 6, 0x0b, static_cast<std::int32_t>(AREA_JUKEBOX_TASK.name));
        Vector3 position{};
        EV_scenApplyPosOrRotToPartOf(JUKEBOX_CHARACTER, 161, &position, nullptr, EventCharacterPartTransformMode::Set);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 470, 0);
        HLib::DestroyCurrentTask();
    }
    EV_NPC_NewLoadSw(1);
}

void EVT_Juke_revolve_Callback(void *) {
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 470))
        return;
    Vector3i rotation{.x = -1601, .y = 0, .z = 0};
    EV_scenApplyPosOrRotToPartOf(JUKEBOX_CHARACTER, 155, nullptr, &rotation, EventCharacterPartTransformMode::Relative);
    rotation = {.x = 0, .y = -1601, .z = 0};
    EV_scenApplyPosOrRotToPartOf(JUKEBOX_CHARACTER, 157, nullptr, &rotation, EventCharacterPartTransformMode::Relative);
}

} // namespace shenmue::s2
