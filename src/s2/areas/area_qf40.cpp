#include "s2/areas/area_qf40.hpp"
#include "s2/door.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fog.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"

namespace shenmue::s2::areas {

std::int32_t qf40HurryFrames{};
std::int32_t qf40BattleState{};

namespace {
void QF40MainloopNullsub(void *) {
}
void QF40HurryWaitCallback(void *);
void QF40HurryStartCallback(void *);
void QF40StartStairAuthorCallback(void *);

void QF40CheckHurryCallback(void *) {
    if (ryoNotBusy() && EV_GetShenmueFlag(0x16c)) {
        HLib::SetCurrentTaskTo(QF40HurryWaitCallback);
        qf40HurryFrames = 5400;
    }
}

void QF40HurryWaitCallback(void *) {
    if (ryoNotBusy() && --qf40HurryFrames == 0) {
        SetPlayerGameFlags(static_cast<PlayerGameFlagMode>(25), reinterpret_cast<void *>(1));
        EV_PTYCUtils(QF40_JOY, 3, 1);
        SetPlayerControlEnabled(0);
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        shenToggleAreaFlags(1);
        HLib::SetCurrentTaskTo(QF40HurryStartCallback);
    }
}

void QF40HurryStartCallback(void *) {
    if (filmCheckFadeTrackColour())
        return;
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    const char *const directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>("BT40"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    qf40BattleState = 1;
    ExecuteCurrAreaMainFunc_Wrapper(3, reinterpret_cast<std::uintptr_t>(&qf40BattleState));
    HLib::SetCurrentTaskTo(QF40StartStairAuthorCallback);
}

void QF40StartStairAuthorCallback(void *) {
    if (qf40BattleState)
        return;
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    const char *const directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_ControlPackageResources(
        PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>("0273A"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    ExecuteCurrAreaMainFunc_Wrapper(3, 1);
    HLib::DestroyCurrentTask();
}
} // namespace

void AreaQF40Init() {
    qf40HurryFrames = 0;
    qf40BattleState = 0;
}

void AreaQF40Cleanup(std::int32_t, std::uint32_t) {
    EV_SetWeatherInstruction(0x0f, 0);
    EV_SetWeatherInstruction(0x780, 0);
    EV_SetWeatherInstruction(0x1800, 0);
    EV_FreeWAIR();
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    EV_UpdateWeather();
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    (void)CheckIsArgEquZero(1);
    SetAreaFogParams();
    SetDoorSubjectEnabled(QF40_DOOR, 0);
}

void AreaQF40Main(std::uint32_t) {
    SetMapControlStatusFlag(0, 0);
    (void)EV_GetShenmueFlag(0xff);
    AreaQF40Cleanup(0, 0);
    HLib::EnqueueTaskWithoutParameter(QF40MainloopNullsub, 4, 0x0b, static_cast<std::int32_t>(QF40_EVENT_TASK.name));
    qf40BattleState = 0;
    HLib::EnqueueTaskWithoutParameter(QF40CheckHurryCallback, 4, 0x0b, static_cast<std::int32_t>(QF40_EVENT_TASK.name));
}

const GameplayAreaDefinition areaQF40GameplayDefinition{
    .areaId = SHEN_NAME(QF40),
    .padding04 = 0,
    .init = &AreaQF40Init,
    .main = &AreaQF40Main,
    .cleanup = &AreaQF40Cleanup,
};

} // namespace shenmue::s2::areas
