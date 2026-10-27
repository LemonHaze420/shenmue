#include "s2/areas/area_qkbt.hpp"
#include "s2/model_component.hpp"
#include "s2/areas/area_street_watch.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/drop_particles.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/flag.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/world_render.hpp"
namespace shenmue::s2::areas {
namespace {
constexpr shen_name fighter{SHEN_NAME(SF4_)}, partner{SHEN_NAME(GOI_)};
constexpr shen_name withAnnouncer[]{STREET_WATCH_ANNOUNCER, fighter, partner, shen_name{0xffffffffU}};
constexpr shen_name withoutAnnouncer[]{fighter, partner, shen_name{0xffffffffU}};
StreetWatchState street{.package = "QKBT",
    .centre = {238.5f, 81.5f, 126.0f},
    .approachCentre = {238.5f, 81.5f, 126.0f},
    .rooftopCentre = {229.5500030517578f, 83.91000366210938f, 129.0f},
    .fighter = fighter,
    .partner = partner,
    .withAnnouncer = withAnnouncer,
    .withoutAnnouncer = withoutAnnouncer,
    .fighterStyle = 26,
    .partnerStyle = 17,
    .approachRadiusSquared = 36.0f};
std::uint32_t startParameters;
std::int32_t preAuthorAdmitted, preAuthorBusy;
StreetFightStartParameters preAuthor;
void MinigameCallback(void *payload) {
    StreetWatchMinigameCallback(payload, street);
}
void StreetCallback(void *payload) {
    StreetWatchMainCallback(payload, street);
}
void PreAuthorCallback(void *) {
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1234))
        return;
    preAuthor.area = SHEN_NAME(SFLU);
    preAuthor.reserved08 = 0;
    preAuthor.scene = 99;
    SetPlayerControlEnabled(0);
    preAuthor.reserved18 = 0;
    preAuthor.flags = 0;
    preAuthor.signal = &preAuthorBusy;
    if (GetFightModeFlags() & 8)
        preAuthor.flags |= 0x10U;
    preAuthor.flags |= 1U;
    LoadAreaFunc_Wrapper_0("SFLU.EV3", 0);
    ExecuteCurrAreaMainFunc("SFLU.EV3", reinterpret_cast<std::uintptr_t>(&preAuthor));
    HLib::DestroyCurrentTask();
}
bool IsBattleMotionAvailable() {
    const Vector3 first{244.0f, 0.0f, 123.5f}, second{244.75f, 0.0f, 132.0f};
    Vector3 position;
    EV_GetCharaPosition(STREET_WATCH_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    const Vector3i rotation = GetCharacterWorldRotation(FindCharacterByScenarioName(STREET_WATCH_RYO));
    return TestCharaPositionWithinRange(STREET_WATCH_RYO, &first, &second, shen_name{0}) && position.y > 89.0f &&
        static_cast<std::uint32_t>(rotation.y) - 5463U <= 21842U;
}
void UpdateBattleMotion() {
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
void MainCallback(void *) {
    ryoNotBusy();
    UpdateBattleMotion();
    if (!preAuthorAdmitted && !EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1233) && !startParameters) {
        SetPlayerControlEnabled(0);
        preAuthorBusy = 1;
        preAuthorAdmitted = 1;
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1234, 1);
        HLib::Task *task = HLib::EnqueueTaskWithoutParameter(PreAuthorCallback, 5, 0xb, static_cast<std::int32_t>(STREET_WATCH_TASK.name));
        HLib::SetupTimedTask(task, 30);
    }
}
void ApplyLighting() {
    SetMapModelDataDefaultVertexShader();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    CheckIsArgEquZero(1);
}
} // namespace
void AreaQKBTInit() {
    preAuthorAdmitted = 0;
    startParameters = 0;
    preAuthor = {};
    preAuthorBusy = 0;
}
void AreaQKBTCleanup(std::int32_t, std::uint32_t) {
    ApplyLighting();
}
void AreaQKBTMain(std::uint32_t parameters) {
    startParameters = parameters;
    preAuthorAdmitted = 0;
    SetIsSkySetup(0);
    SetMapControlStatusFlag(0, 0);
    EV_SetWeatherInstruction(0x0f, 0);
    EV_SetWeatherInstruction(0x780, 0);
    EV_SetWeatherInstruction(0x1800, 0);
    EV_GetShenmueFlag(0xff);
    ApplyLighting();
    shenSetTimeSimulationTo(0);
    shenSetClockAndCompassRenderTo(1);
    InitializeDropParticles();
    EV_SetCharaControllerFlags(STREET_WATCH_RYO, CharacterControlApplyGravity, NpcCharacterControllerState::Enabled);
    HLib::EnqueueTaskWithoutParameter(MainCallback, 4, 0xb, static_cast<std::int32_t>(STREET_WATCH_TASK.name));
    if (!parameters) {
        SetFightModeFlags(static_cast<std::uint16_t>(GetFightModeFlags() | 1U));
        SetFightModeFlags(static_cast<std::uint16_t>(GetFightModeFlags() | 2U));
        SetFightModeFlags(static_cast<std::uint16_t>(GetFightModeFlags() | 4U));
    }
    QueueStreetWatch(street, MinigameCallback, StreetCallback);
}
const GameplayAreaDefinition areaQKBTGameplayDefinition{SHEN_NAME(QKBT), 0, AreaQKBTInit, AreaQKBTMain, AreaQKBTCleanup};
} // namespace shenmue::s2::areas
