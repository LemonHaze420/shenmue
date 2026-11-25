#include "s2/events/event_api.h"

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
#include "s2/areas/area_jukebox.hpp"
void AreaCleanup(int afternoon);
void AreaWECFMainLoop(void *taskData);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void LoadWECFAudio(int afternoon);
#endif
#else
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
#endif

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters) {
#else
void AreaWECFMain(std::uint32_t) {
#endif
#if defined(SHENMUE_EVENT_OVERLAY) && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    EventDateTime dateTime;
#endif
    FTBL.HLib_EnqueueTaskWithoutParameter(AreaWECFMainLoop, 4, 0x0b, TASK_EVNT);
#if defined(SHENMUE_EVENT_OVERLAY)
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    FTBL.GetCurrentDateTime((DateTime *)&dateTime);
    if ((unsigned char)(dateTime.hour - 19) <= 5)
#else
    if (FTBL.EV_GetShenmueFlag(0xff))
#endif
        AreaCleanup(1);
    else
        AreaCleanup(0);
#else
    (void)EV_GetShenmueFlag(0xffU);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    (void)CheckIsArgEquZero(1);
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.HLib_EnqueueTaskWithoutParameter(EVT_JB_InitWk_Callback, 5, 0x0b, TASK_EVNT);
#else
    FTBL.HLib_EnqueueTaskWithoutParameter(JukeboxInitCallback, 5, 0x0b, TASK_EVNT);
#endif
}

// @verified - p
#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon) {
#else
void AreaWECFCleanup(std::int32_t, std::uint32_t) {
#endif
    FTBL.EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    FTBL.PushGlobalLightingProgram();
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.CheckIsArgEquZero(1, 1);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    LoadWECFAudio(afternoon);
#endif
#else
    (void)CheckIsArgEquZero(1);
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY) && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void LoadWECFAudio(int afternoon) {
    if (afternoon)
        FTBL.EV_LoadAudio("WECF_000", 0);
    else
        FTBL.EV_LoadAudio("WECF_000", 0);
}
#endif

// @verified - p
void AreaWECFMainLoop(void *taskData) {
}
#if defined(SHENMUE_EVENT_OVERLAY) && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void AreaWECFNullsub(void) {
}
#endif

#if !defined(SHENMUE_EVENT_OVERLAY)
// @verified - p
void JukeboxInitCallback(void *) {
    if (GetNpcNewLoadState() != 1)
        return;
    if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("JUKB"), 0, 0) != 0)
        return;

    (void)EV_NPC_NewLoadSw(0);
    (void)EV_ResetFreeTalkFlags();
    const char *const packagePath = Filepath_Gen(FilepathGenMode::PackDirectory);
    if (EV_pakUtils(
            PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(packagePath), reinterpret_cast<std::uintptr_t>("JUKB"), 0) != 0) {
        (void)EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0, 0);
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
#endif

#if !defined(SHENMUE_EVENT_OVERLAY)
const GameplayAreaDefinition areaWECFGameplayDefinition{
    .areaId = AREA_WECF,
    .padding04 = 0,
    .init = &AreaWECFInit,
    .main = &AreaWECFMain,
    .cleanup = &AreaWECFCleanup,
};

} // namespace shenmue::s2::areas
#endif
