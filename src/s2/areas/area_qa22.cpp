#include "s2/areas/area_qa22.hpp"
#include "s2/areas/area_jukebox.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
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
inline constexpr shen_name QA22_PLAYER{SHEN_NAME(RYO_)};
inline constexpr shen_name QA22_LIGHT{SHEN_NAME(0001)};

void QA22MainloopCallback(void *) {
    if (!ryoNotBusy())
        return;
    std::uint64_t authorParameter = 0x2500000000ULL;
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

void QA22FanCallback(void *) {
    for (shen_name fan : QA22_FANS) {
        Vector3i rotation;
        GetScenarioCharacterEulerAngles(fan, &rotation, EventTransformFlags::World);
        rotation.y -= 728;
        EV_AT_SetAngY(fan, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation, nullptr);
    }
}
} // namespace

void AreaQA22Init() {
}

void AreaQA22Main(std::uint32_t) {
    HLib::EnqueueTaskWithoutParameter(QA22MainloopCallback, 4, 0x0b, static_cast<std::int32_t>(AREA_JUKEBOX_TASK.name));
    EV_SetObjectFlagByIndex(1, 0);
    EV_SetWeatherInstruction(0x780, 0);
    EV_SetWeatherInstruction(0x0c, 0);
    DateTime dateTime;
    GetCurrentDateTime(dateTime);
    (void)EV_GetShenmueFlag(0xff);
    EV_lightLoad(QA22_LIGHT);
    PushGlobalLightingProgram();
    SetupLightConfig();
    SetPluralControlFor(ctrlGetCharaByName(QA22_PLAYER), 1);
    HLib::EnqueueTaskWithoutParameter(QA22FanCallback, 5, 0x0b, static_cast<std::int32_t>(AREA_JUKEBOX_TASK.name));
    HLib::EnqueueTaskWithoutParameter(EVT_JB_InitWk_Callback, 5, 0x0b, static_cast<std::int32_t>(AREA_JUKEBOX_TASK.name));
}

void AreaQA22Cleanup(std::int32_t, std::uint32_t) {
    EV_lightLoad(QA22_LIGHT);
    PushGlobalLightingProgram();
    SetupLightConfig();
    SetPluralControlFor(ctrlGetCharaByName(QA22_PLAYER), 1);
}

const GameplayAreaDefinition areaQA22GameplayDefinition{
    .areaId = SHEN_NAME(QA22),
    .padding04 = 0,
    .init = &AreaQA22Init,
    .main = &AreaQA22Main,
    .cleanup = &AreaQA22Cleanup,
};

} // namespace shenmue::s2::areas
