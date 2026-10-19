#include "s2/areas/area_wes1.hpp"
#include "s2/areas/area_jukebox.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"

namespace shenmue::s2::areas {
namespace {
inline constexpr shen_name WES1_JUKEBOX_TASK{SHEN_NAME(jukb)};

void WES1MainloopCallback(void *) {
    if (!ryoNotBusy())
        return;
    std::uint64_t authorParameter = 0x1b00000000ULL;
    if (shenGetMinutesUntilNextFrameMinute())
        return;
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1809))
        return;
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("KHIN"), 0))
        return;
    SetPlayerControlEnabled(0);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    const char *const directory = Filepath_Gen(FilepathGenMode::PackDirectory);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>("KHIN"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("KHIN"), 0);
    ExecuteCurrAreaMainFunc("KHIN.EV3", reinterpret_cast<std::uintptr_t>(&authorParameter));
}
} // namespace

void AreaWES1Init() {
}

void AreaWES1Main(std::uint32_t) {
    HLib::EnqueueTaskWithoutParameter(WES1MainloopCallback, 4, 0x0b, static_cast<std::int32_t>(AREA_JUKEBOX_TASK.name));
    (void)EV_GetShenmueFlag(0xff);
    EV_SetObjectFlagByIndex(2, 0);
    EV_SetWeatherInstruction(0x780, 0);
    EV_SetWeatherInstruction(0x0c, 0);
    EV_AssignLightParams(1, 1);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    AssignLightProgram(0);
    SetupLightConfig();
    (void)CheckIsArgEquZero(1);
    EV_setHoldStatesFlags(0);
    HLib::EnqueueTaskWithoutParameter(EVT_JB_InitWk_Callback, 5, 0x0b, static_cast<std::int32_t>(WES1_JUKEBOX_TASK.name));
}

void AreaWES1Cleanup(std::int32_t, std::uint32_t) {
}

const GameplayAreaDefinition areaWES1GameplayDefinition{
    .areaId = SHEN_NAME(WES1),
    .padding04 = 0,
    .init = &AreaWES1Init,
    .main = &AreaWES1Main,
    .cleanup = &AreaWES1Cleanup,
};

} // namespace shenmue::s2::areas
