#include "s2/areas/area_qjbt.hpp"
#include "s2/fog.hpp"
#include "s2/areas/area_elevator.hpp"
#include "s2/areas/area_street_watch.hpp"
#include "s2/character.hpp"
#include "s2/door.hpp"
#include "s2/elevator.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/field_collision.hpp"
#include "s2/flag.hpp"
#include "s2/help.hpp"
#include "s2/lighting.hpp"
#include "s2/input.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include <bit>
namespace shenmue::s2::areas {
namespace {
constexpr shen_name fighter{SHEN_NAME(AW3_)}, partner{SHEN_NAME(SFB_)};
constexpr shen_name withAnnouncer[]{STREET_WATCH_ANNOUNCER, fighter, partner, shen_name{0xffffffffU}};
constexpr shen_name withoutAnnouncer[]{fighter, partner, shen_name{0xffffffffU}};
StreetWatchState street{.package = "QJBT",
    .centre = {267.29998779296875f, 85.5999984741211f, 233.10000610351562f},
    .approachCentre = {240.5f, 85.5999984741211f, 92.5f},
    .rooftopCentre = {229.5500030517578f, 83.91000366210938f, 129.0f},
    .fighter = fighter,
    .partner = partner,
    .withAnnouncer = withAnnouncer,
    .withoutAnnouncer = withoutAnnouncer,
    .fighterStyle = 23,
    .partnerStyle = 17,
    .approachRadiusSquared = 45.5625f};
std::int32_t authorBusy;
StreetFightStartParameters author;
QronFloorProp floorProps[]{{SHEN_NAME(F001), {0, 0, 0, 0}, 0, 0, 0, nullptr}, {SHEN_NAME(F004), {32768, 8, 0, 0}, 0, 0, 0, nullptr},
    {SHEN_NAME(F005), {4096, 1, 0, 0}, 0, 0, 0, nullptr}, {SHEN_NAME(F006), {0, 0, 0, 0}, 0, 0, 0, nullptr}, {}};
void FloorCallback(Vector3 *position, std::uint32_t floor) {
    UpdateAreaElevatorBindings(position, floor);
    const std::int32_t level = std::bit_cast<std::int32_t>(floor);
    ToggleFlagOnFieldEntity(SHEN_NAME(F002), level > 0 ? 0 : 1);
    SetDisplayFlagForProp(SHEN_NAME(F002), level > 0 ? 0 : 1);
    ToggleFlagOnFieldEntity(SHEN_NAME(F007), level < 2 ? 0 : 1);
    SetDisplayFlagForProp(SHEN_NAME(F007), level < 2 ? 0 : 1);
    const std::int32_t upper = level > 4 ? 1 : 0;
    ToggleFlagOnFieldEntity(SHEN_NAME(F003), upper);
    SetDisplayFlagForProp(SHEN_NAME(F003), upper);
    ToggleFlagOnFieldEntity(SHEN_NAME(R595), upper ? 0 : 1);
    SetDoorSubjectEnabled(SHEN_NAME(R595), upper ? 0 : 1);
    SetElevatorDisplayMode(SHEN_NAME(R595), 0, upper ? 0 : 1);
}
const QronFloorDescriptor floorDescriptor{FloorCallback, floorProps};
void MinigameCallback(void *payload) {
    StreetWatchMinigameCallback(payload, street);
}
void StreetCallback(void *payload) {
    StreetWatchMainCallback(payload, street);
}
void AuthorCompleteCallback(void *) {
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1233))
        return;
    EV_NPC_NewLoadSw(1);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_spmBeginSampleExecute();
    SetPlayerControlEnabled(1);
    HLib::DestroyCurrentTask();
}
void AuthorWaitCallback(void *) {
    if (!authorBusy)
        HLib::SetCurrentTaskTo(AuthorCompleteCallback);
}
void AuthorBeginCallback(void *) {
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1234))
        return;
    authorBusy = 1;
    author.area = SHEN_NAME(SFJF);
    author.scene = 99;
    SetPlayerControlEnabled(0);
    author.reserved18 = 0;
    author.flags = 0;
    author.signal = &authorBusy;
    if (GetFightModeFlags() & 8)
        author.flags |= 0x10U;
    author.flags |= 1U;
    LoadAreaFunc_Wrapper_0("SFJF.EV3", 0);
    ExecuteCurrAreaMainFunc("SFJF.EV3", reinterpret_cast<std::uintptr_t>(&author));
    HLib::SetCurrentTaskTo(AuthorWaitCallback);
}
void AuthorAdmissionCallback(void *) {
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1233)) {
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1234, 1);
        HLib::SetCurrentTaskTo(AuthorBeginCallback);
    }
}
bool IsBattleMotionAvailable() {
    Vector3 position;
    EV_GetCharaPosition(STREET_WATCH_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    const Vector3i rotation = GetCharacterWorldRotation(FindCharacterByScenarioName(STREET_WATCH_RYO));
    const Vector3 first{273.0f, 0.0f, 235.0f}, second{271.0f, 0.0f, 230.0f};
    return TestCharaPositionWithinRange(STREET_WATCH_RYO, &first, &second, shen_name{0}) && GetQronNextStream() == 0 &&
        static_cast<std::uint32_t>(rotation.y) - 6736U <= 25484U;
}
void MainCallback(void *) {
    if (!IsBattleMotionAvailable())
        return;
    if (vfmDispatchControl(FightControlOp::CameraMode, shen_name{0}, 0, -1) == 5)
        return;
    if (!shenGetMinutesUntilNextFrameMinute())
        return;
    if (GetBattleDataConfig(2, SHEN_NAME(BTLD)))
        vfmDispatchControl(FightControlOp::MotionUpdateCamera, fighter, 0, 1);
    else
        GetBattleDataConfig(0, SHEN_NAME(BTLD));
}
void ApplyLighting() {
    SetPluralControlFor(FindCharacterByScenarioName(STREET_WATCH_RYO), 1);
    EV_SetWeatherInstruction(0x0f, 0);
    EV_SetWeatherInstruction(0x780, 0);
    EV_SetWeatherInstruction(0x1800, 0);
    SetMapModelDataDefaultVertexShader();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    Vector3 position{0.0f, 0.0f, 0.0f};
    SetLightRootPosition(position);
    AssignLightProgram(0);
    position.y = GetQronTemporaryFloorHeight() - 100.0f;
    SetLightRootPosition(position);
    AssignLightProgram(1);
    AssignLightProgram(2);
    SetupLightConfig();
    CheckIsArgEquZero(1);
    SetAreaFogParams();
}
} // namespace
void AreaQJBTInit() {
    authorBusy = 0;
    author = {};
}
void AreaQJBTCleanup(std::int32_t, std::uint32_t) {
    ApplyLighting();
}
void AreaQJBTMain(std::uint32_t) {
    SetMapControlStatusFlag(0, 0);
    ElevatorStuff();
    EV_SetObjectFlagByIndex(0, 0);
    EV_SetObjectFlagByIndex(1, 0);
    EV_SetObjectFlagByIndex(2, 0);
    AttachMapLightingPlacement(1, 0);
    AttachMapLightingPlacement(2, 5);
    AttachMapLightingPlacement(2, 10);
    AttachMapLightingPlacement(0, 15);
    SetQronFloorDescriptor(&floorDescriptor);
    EV_GetShenmueFlag(0xff);
    ApplyLighting();
    SetPlayerControlEnabled(0);
    QueueStreetWatch(street, MinigameCallback, StreetCallback);
    if (g_entryPoint == 99) {
        SetPlayerControlEnabled(0);
        SetFightModeFlags(static_cast<std::uint16_t>(GetFightModeFlags() | 1U));
        SetFightModeFlags(static_cast<std::uint16_t>(GetFightModeFlags() | 2U));
        HLib::EnqueueTaskWithoutParameter(AuthorAdmissionCallback, 5, 0x0b, static_cast<std::int32_t>(STREET_WATCH_TASK.name));
    }
    HLib::EnqueueTaskWithoutParameter(MainCallback, 4, 0x0b, static_cast<std::int32_t>(0x6e69616dU));
    SetPlayerControlEnabled(1);
}
const GameplayAreaDefinition areaQJBTGameplayDefinition{SHEN_NAME(QJBT), 0, AreaQJBTInit, AreaQJBTMain, AreaQJBTCleanup};
} // namespace shenmue::s2::areas
