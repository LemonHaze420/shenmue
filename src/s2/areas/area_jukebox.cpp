#include "s2/areas/area_jukebox.hpp"
#include "s2/events/event_api.h"

#if defined(SHENMUE_EVENT_OVERLAY)
enum { JUKEBOX_CHARACTER = SHEN_NAME4('J', 'B', 'O', 'X') };
#else
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"

namespace shenmue::s2 {
namespace {
inline constexpr shen_name JUKEBOX_CHARACTER{SHEN_NAME(JBOX)};
} // namespace
#endif

void EVT_JB_InitWk_Callback(void *data)
{
    if (FTBL.GetNpcNewLoadState() != 1)
        return;
#if defined(SHENMUE_EVENT_OVERLAY) && SHENMUE_JUKEBOX_REVISION <= 10
    if (!FTBL.EV_ControlPackageResources(0, 5, "JUKB", 0, 0)) {
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(7), (EventPackageArgument)"JUKB", 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        FTBL.EV_ControlPackageResources(2, 2, "JUKB", 0, 0);
    }
#elif defined(SHENMUE_EVENT_OVERLAY)
    if (FTBL.EV_ControlPackageResources(0, 5, "JUKB", 0, 0))
        return;
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    if (FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(7), (EventPackageArgument)"JUKB", 0)) {
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
#else
    if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("JUKB"), 0))
        return;
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    const char *const directory = Filepath_Gen(FilepathGenMode::PackDirectory);
    if (EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory),
            reinterpret_cast<std::uintptr_t>("JUKB"))) {
        EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
#endif
        FTBL.HLib_EnqueueTaskWithoutParameter(EVT_Juke_revolve_Callback, 6, 0x0b, AREA_JUKEBOX_TASK_TOKEN);
#if defined(SHENMUE_EVENT_OVERLAY)
        {
            Vector3 position;
            position.x = 0.0f;
            position.y = 0.0f;
            position.z = 0.0f;
            FTBL.scenApplyPosOrRotToChildId(JUKEBOX_CHARACTER, 161, &position, 0, 3);
            FTBL.EV_SetPlayerFlag(12, 470, 0);
            FTBL.HLib_DestroyCurrentTask();
        }
#else
        Vector3 position{};
        EV_scenApplyPosOrRotToPartOf(JUKEBOX_CHARACTER, 161, &position, nullptr, EventCharacterPartTransformMode::Set);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 470, 0);
        HLib::DestroyCurrentTask();
#endif
#if !defined(SHENMUE_EVENT_OVERLAY) || SHENMUE_JUKEBOX_REVISION > 10
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.DebugLogToFileRemoved("Pack Load Success! [JUKB]2\n");
    } else {
        FTBL.DebugLogToFileRemoved("Pack Load Failed [JUKB]2\n");
#endif
    }
    FTBL.EV_NPC_NewLoadSw_0(1);
#endif
}

void EVT_Juke_revolve_Callback(void *data)
{
#if defined(SHENMUE_EVENT_OVERLAY)
    if (!FTBL.EV_GetPlayerFlag(11, 470))
        return;
    {
        Vector3i rotation = {-1601, 0, 0};
        FTBL.scenApplyPosOrRotToChildId(JUKEBOX_CHARACTER, 155, 0, &rotation, 1);
        rotation.x = 0;
        rotation.y = -1601;
        rotation.z = 0;
        FTBL.scenApplyPosOrRotToChildId(JUKEBOX_CHARACTER, 157, 0, &rotation, 1);
    }
#else
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 470))
        return;
    Vector3i rotation{.x = -1601, .y = 0, .z = 0};
    EV_scenApplyPosOrRotToPartOf(JUKEBOX_CHARACTER, 155, nullptr, &rotation, EventCharacterPartTransformMode::Relative);
    rotation = {.x = 0, .y = -1601, .z = 0};
    EV_scenApplyPosOrRotToPartOf(JUKEBOX_CHARACTER, 157, nullptr, &rotation, EventCharacterPartTransformMode::Relative);
#endif
}

#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace shenmue::s2
#endif
