#include "s2/areas/area_weg0.hpp"
#include "s2/field_collision.hpp"
#include "s2/sound_program.hpp"
#include "s2/areas/area_jukebox.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/qte_game_entry.hpp"
#include "s2/screen_film.hpp"
#include "s2/scene_data.hpp"
namespace shenmue::s2::areas {
namespace {
std::int16_t previousRegion{};
std::int32_t initialAudioPending{};
constexpr EventDartsPlacement dartPlacements[]{
    {0, {0, 16384, 0}, {203.10000610351562f, 100.80000305175781f, 97.0f}, SHEN_NAME(DAR1), SHEN_NAME(DAR2)},
    {1, {0, 16384, 0}, {203.10000610351562f, 100.80000305175781f, 95.0f}, SHEN_NAME(DAR2), SHEN_NAME(DAR2)},
    {1, {0, 16384, 0}, {203.10000610351562f, 100.80000305175781f, 97.0f}, SHEN_NAME(DAR1), SHEN_NAME(DAR1)},
    {0, {0, 49152, 0}, {210.8769989013672f, 100.80000305175781f, 94.0999984741211f}, SHEN_NAME(DAR3), SHEN_NAME(DAR3)},
};
constexpr EventDartsEnemy primaryEnemy{SHEN_NAME(XHO_), 4, 0, 0};
constexpr EventDartsEnemy alternateEnemy{SHEN_NAME(HLN_), 2, 0, 0};
constexpr shen_name dartVisibility[]{SHEN_NAME(DRT0), SHEN_NAME(DRT1), SHEN_NAME(DRT2), SHEN_NAME(TITL), SHEN_NAME(MIT1), SHEN_NAME(MIT2), SHEN_NAME(MIT3),
    SHEN_NAME(QTEM), SHEN_NAME(VM00), SHEN_NAME(NULL)};
constexpr shen_name jukeboxVisibility[]{SHEN_NAME(JBOX), SHEN_NAME(ST01), SHEN_NAME(ST02), SHEN_NAME(ST03), SHEN_NAME(ST04), SHEN_NAME(ST05), SHEN_NAME(ST06),
    SHEN_NAME(ST07), SHEN_NAME(ST08), SHEN_NAME(ST09), SHEN_NAME(ST10), SHEN_NAME(ST11), SHEN_NAME(ST12), SHEN_NAME(ST13), SHEN_NAME(ST14), SHEN_NAME(ST15),
    SHEN_NAME(ST16), SHEN_NAME(ST17), SHEN_NAME(ST18), SHEN_NAME(ST19), SHEN_NAME(ST20), SHEN_NAME(NULL)};

void ApplyLighting() {
    EV_SetWeatherInstruction(0x780, 0);
    EV_SetWeatherInstruction(0x0c, 0);
    SetMapModelDataDefaultVertexShader();
    SetDefaultVS();
    EV_AssignLightParams(1, 1);
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1015)) {
        SetMapModelWeatherLightingOverride(3, 0);
        SetMapModelWeatherLightingOverride(5, 0);
    }
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    for (std::int32_t index = 2; index < 7; ++index)
        AssignLightProgram(index);
    SetupLightConfig();
    SetPluralControlFor(FindCharacterByScenarioName(SHEN_NAME(DRT0)), 0);
    SetPluralControlFor(FindCharacterByScenarioName(SHEN_NAME(DRT1)), 0);
    SetPluralControlFor(FindCharacterByScenarioName(SHEN_NAME(DRT2)), 0);
    SetPluralControlFor(FindCharacterByScenarioName(SHEN_NAME(TITL)), 0);
    const Vector4 reflection{1.0f, 0.800000011920929f, 0.800000011920929f, 0.800000011920929f};
    for (std::int32_t index = 1; index <= 19; index += 2) {
        const shen_name name{make_shen_name('S', 'T', static_cast<char>('0' + index / 10), static_cast<char>('0' + index % 10))};
        SetScenarioReflectorBoardValues(name, &reflection);
    }
    for (std::int32_t index = 2; index <= 20; index += 2) {
        const shen_name name{make_shen_name('S', 'T', static_cast<char>('0' + index / 10), static_cast<char>('0' + index % 10))};
        SetScenarioReflectorBoardValues(name, &reflection);
    }
    SetPluralControlFor(FindCharacterByScenarioName(SHEN_NAME(RYO_)), 1);
}
void DispatchKhin() {
    std::uint64_t work = 0x2000000000ULL;
    if (!shenGetMinutesUntilNextFrameMinute() && EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1809) &&
        !EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("KHIN"), 0)) {
        SetPlayerControlEnabled(0);
        EV_NPC_NewLoadSw(0);
        EV_ResetFreeTalkFlags();
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::PackDirectory)),
            reinterpret_cast<std::uintptr_t>("KHIN"));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("KHIN"), 0);
        ExecuteAreaMainFunction("KHIN.EV3", reinterpret_cast<std::uintptr_t>(&work));
    }
}
std::int32_t GetPlayerRegion() {
    Vector3 position;
    EV_GetCharaPosition(SHEN_NAME(RYO_), CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    if (position.x <= 205.0f) {
        if (position.y > 102.0f)
            return position.y > 106.80000305175781f ? 6 : 4;
        return 2;
    }
    if (position.y <= 102.0f)
        return 1;
    return position.y > 106.80000305175781f ? 5 : 3;
}
void UpdateVisibility() {
    const std::int32_t region = GetPlayerRegion();
    if (previousRegion == region)
        return;
    previousRegion = static_cast<std::int16_t>(region);
    std::int16_t flags = 0;
    switch (region) {
    case 1:
        flags = 285;
        break;
    case 3:
        flags = 618;
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1015) && initialAudioPending) {
            EV_LoadAudio("WEG0_000", 0);
            initialAudioPending = 0;
        }
        break;
    case 5:
        flags = 136;
        break;
    case 2:
        flags = 125;
        break;
    case 4:
        flags = 622;
        break;
    case 6:
        flags = 168;
        break;
    }
    for (std::int32_t index = 1; index <= 6; ++index)
        EV_SetObjectFlagByIndex(index, flags & (1 << (index + 1)));
    SetMapModelWeatherLightingOverride(3, flags & 0x100);
    SetMapModelWeatherLightingOverride(5, flags & 0x200);
    for (std::int16_t index = 0; dartVisibility[index].name != 0x4c4c554eU; ++index) {
        if (flags & 1)
            EnableScenarioCharacterDisplay(dartVisibility[index]);
        else
            DisableScenarioCharacterDisplay(dartVisibility[index]);
    }
    for (std::int16_t index = 0; jukeboxVisibility[index].name != 0x4c4c554eU; ++index) {
        if (flags & 2)
            EnableScenarioCharacterDisplay(jukeboxVisibility[index]);
        else
            DisableScenarioCharacterDisplay(jukeboxVisibility[index]);
    }
}
void WEG0MainLoop(void *) {
    if (ryoNotBusy())
        DispatchKhin();
    UpdateVisibility();
    if (EV_GetShenmueFlag(0x39d)) {
        DisableScenarioCharacterDisplay(SHEN_NAME(roup));
        ToggleFlagOnFieldEntity(SHEN_NAME(rope), 0);
    } else {
        EnableScenarioCharacterDisplay(SHEN_NAME(roup));
        ToggleFlagOnFieldEntity(SHEN_NAME(rope), 1);
    }
}
} // namespace
void AreaWEG0Init() {
    previousRegion = 0;
    initialAudioPending = 0;
}
void AreaWEG0Main(std::uint32_t) {
    initialAudioPending = 1;
    previousRegion = 0;
    HLib::EnqueueTaskWithoutParameter(WEG0MainLoop, 4, 0x0b, static_cast<std::int32_t>(SHEN_NAME(EVNT)));
    EV_GetShenmueFlag(FLAG_NIGHT_TIME_ENFORCED);
    ApplyLighting();
    EV_LoadCutsceneFromCurrentScene("WEG0");
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1015))
        EV_LoadAudio("WEG0_000", 0);
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) || g_entryPoint == 20)
        HLib::EnqueueTaskWithoutParameter(EVT_JB_InitWk_Callback, 5, 0x0b, static_cast<std::int32_t>(SHEN_NAME(EVNT)));
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        SetPlayerGameFlags(PlayerGameFlagMode::Bit22, reinterpret_cast<void *>(1));
    EV_SetObjectFlagByIndex(7, 0);
    EVT_QteGameEnter();
    if (EV_GetShenmueFlag(0x33b)) {
        EVT_SetEnemyDarts(dartPlacements, &primaryEnemy);
        EVT_DartsEasySingle(3, dartPlacements + 1);
        QueueConditionalNpcLoads(areaWEG0DartsNpcLoads);
    } else if (EV_GetShenmueFlag(0x396)) {
        EVT_SetEnemyDarts(dartPlacements, &alternateEnemy);
        EVT_DartsEasySingle(3, dartPlacements + 1);
        QueueConditionalNpcLoads(areaWEG0DartsNpcLoads);
    } else {
        EVT_DartsEasySingle(3, dartPlacements + 1);
        ToggleFlagOnFieldEntity(SHEN_NAME(DART), 0);
    }
    const GlowParameters glow{5, 0.30000001192092896f, {0.30000001192092896f, 0.30000001192092896f, 0.30000001192092896f, 0.17000000178813934f}};
    SelectPostFxBank(1, 1);
    UploadGlowParams(glow);
}
void AreaWEG0Cleanup(std::int32_t, std::uint32_t) {
    ApplyLighting();
}
const GameplayAreaDefinition areaWEG0GameplayDefinition{SHEN_NAME(WEG0), 0, AreaWEG0Init, AreaWEG0Main, AreaWEG0Cleanup};
} // namespace shenmue::s2::areas
