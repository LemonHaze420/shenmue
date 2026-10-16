#include "s2/areas/area_duck.hpp"
#include "s2/area_event.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/debug.hpp"
#include "s2/duck_race.hpp"
#include "s2/event_call.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/event_voice.hpp"
#include "s2/face.hpp"
#include "s2/field_collision.hpp"
#include "s2/flag.hpp"
#include "s2/gauge.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/inventory.hpp"
#include "s2/legacy_hud_text.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/model_image.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sk_event.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <cstring>
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {
namespace {
constexpr shen_name invalid{0xffffffffU}, ryo{SHEN_NAME(RYO_)}, xgh{SHEN_NAME(XGH_)}, coh{SHEN_NAME(COH_)}, hand{SHEN_NAME(HAND)}, base{SHEN_NAME(BASE)},
    hdum{SHEN_NAME(HDUM)}, pcam{SHEN_NAME(PCAM)}, medal{SHEN_NAME(MEDL)}, board{SHEN_NAME(aa82)}, money{SHEN_NAME(KANE)}, aa95{SHEN_NAME(aa95)},
    event{SHEN_NAME(EVNT)};
constexpr shen_name initialCharacters[]{ryo, xgh, coh, invalid};
constexpr shen_name introCharacters[]{SHEN_NAME(PG5_), SHEN_NAME(JJ1_), SHEN_NAME(CLY_), SHEN_NAME(JN8_), SHEN_NAME(OJ5_), SHEN_NAME(WTC_), invalid};
constexpr shen_name duckNames[]{SHEN_NAME(duk1), SHEN_NAME(duk2), SHEN_NAME(duk3), SHEN_NAME(duk4), SHEN_NAME(duk5), SHEN_NAME(duk6)};
constexpr shen_name obstacleNames[]{
    SHEN_NAME(DRM1), SHEN_NAME(DRM2), SHEN_NAME(DRM3), SHEN_NAME(DRM4), SHEN_NAME(DRM5), SHEN_NAME(DRM6), SHEN_NAME(DRM7), SHEN_NAME(DRM8), SHEN_NAME(DRM9)};
struct DuckAreaRuntime {
    HLib::Task *main;
    HLib::Task *debug;
    HLib::Task *paddock;
    std::array<std::int32_t, 6> profiles, odds;
    std::array<shen_name, 18> digitCharacters;
    std::array<LegacyHudText *, 5> statistics;
    std::array<std::int32_t, 5> statisticTextures;
    std::array<LegacySpritePacket, 5> statisticPackets;
    std::array<std::int32_t, 4> previousExperience;
    std::array<std::array<char, 12>, 16> voices;
    std::array<char, 64> voicePrefix;
    std::int32_t voiceCount, paidGame, playerRace, debugStandings, menuShown, browseIndex, previousBrowse;
    float paddockAlpha;
};
DuckAreaRuntime state{};
constexpr std::array<Vector3, 33> authoredCoordinates{{Vector3{258.20001220703125f, 115.69999694824219f, 310.3999938964844f},
    Vector3{258.3999938964844f, 115.69999694824219f, 314.5f}, Vector3{259.79998779296875f, 115.69999694824219f, 313.6000061035156f},
    Vector3{261.1000061035156f, 115.69999694824219f, 314.29998779296875f}, Vector3{258.8999938964844f, 115.69999694824219f, 315.5f},
    Vector3{260.0f, 115.69999694824219f, 311.1000061035156f}, Vector3{257.1000061035156f, 115.69999694824219f, 316.5f},
    Vector3{258.20001220703125f, 115.69999694824219f, 314.8999938964844f}, Vector3{259.0f, 115.69999694824219f, 312.29998779296875f},
    Vector3{260.0f, 115.69999694824219f, 314.5f}, Vector3{259.8999938964844f, 115.69999694824219f, 309.8999938964844f},
    Vector3{258.29998779296875f, 115.69999694824219f, 310.29998779296875f}, Vector3{255.5f, 115.5999984741211f, 300.6000061035156f},
    Vector3{253.1999969482422f, 115.5999984741211f, 300.0f}, Vector3{252.60000610351562f, 115.5999984741211f, 298.29998779296875f},
    Vector3{251.3000030517578f, 115.5f, 300.1000061035156f}, Vector3{255.89999389648438f, 115.4000015258789f, 298.3999938964844f},
    Vector3{257.0f, 115.5f, 299.3999938964844f}, Vector3{1255.5f, 115.5999984741211f, 300.6000061035156f},
    Vector3{1253.199951171875f, 115.5999984741211f, 300.0f}, Vector3{1252.5999755859375f, 115.5999984741211f, 298.29998779296875f},
    Vector3{1251.300048828125f, 115.5f, 300.1000061035156f}, Vector3{1255.9000244140625f, 115.4000015258789f, 298.3999938964844f},
    Vector3{1257.0f, 115.5f, 299.3999938964844f}, Vector3{1368.4000244140625f, 100.4000015258789f, 756.2999877929688f},
    Vector3{1369.0999755859375f, 100.4000015258789f, 754.7999877929688f}, Vector3{1370.0999755859375f, 100.4000015258789f, 756.4000244140625f},
    Vector3{1368.5f, 102.0999984741211f, 786.5999755859375f}, Vector3{250.6999969482422f, 115.0999984741211f, 335.1000061035156f},
    Vector3{188.10000610351562f, 107.19999694824219f, 312.29998779296875f}, Vector3{-0.8999999761581421f, 0.20000000298023224f, 0.0f},
    Vector3{-0.30000001192092896f, 0.20000000298023224f, 0.0f}, Vector3{0.30000001192092896f, 0.20000000298023224f, 0.0f}}};
std::array<std::int32_t, 57> holdHand{0, 0, 56072, 0, 0, 64627, 0, 62624, 2912, 0, 0, 60258, 0, 0, 61168, 0, 0, 62989, 0, 546, 1820, 0, 0, 59712, 0, 0, 59894,
    0, 0, 62624, 0, 546, 2547, 0, 0, 61897, 0, 0, 59167, 0, 0, 0, 0, 546, 2182, 0, 0, 58802, 0, 0, 61532, 0, 0, 65354, 0, 546, 2001};
std::array<std::int32_t, 57> releaseHand{0, 0, 61896, 0, 0, 364, 0, 62623, 3639, 0, 0, 60257, 0, 0, 61167, 0, 0, 63352, 0, 546, 2001, 0, 0, 59712, 0, 0, 59893,
    0, 0, 62805, 0, 546, 2730, 0, 0, 61896, 0, 0, 59166, 0, 0, 64989, 0, 546, 2182, 0, 0, 58802, 0, 0, 61532, 0, 0, 65354, 0, 546, 2001};
void Main_Callback(void *);
void End_Callback(void *);
void PaddockInfo_Callback(void *);
void QueueMain(void *);
void DebugNullsub(void *) {
}
void Camera(std::int32_t id, shen_name eye = invalid, shen_name target = invalid, std::int32_t delay = 0) {
    EventSetCamera immediate{};
    EventSetCamera *request = &immediate;
    if (delay) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_SetCam_Callback, 6, 0x0b, sizeof(EventSetCamera), event);
        RegisterEventTask(task);
        request = static_cast<EventSetCamera *>(HLib::GetTaskParameter(task));
    }
    *request = {
        .cleanup = delay != 0, .camera = id, .eye = eye, .target = target, .context = 0, .reserved11 = {}, .angle = 0, .delay = static_cast<float>(delay)};
    if (!delay) {
        EVT_AT_SetCam_Callback(request);
        camCtrlDisableFilmBars();
    }
}
void Position(shen_name name, const Vector3 &position, float yaw, shen_name parent = invalid) {
    EVT_AT_SetPos(name, parent, position, 0);
    EVT_AT_SetAngY(name, parent, yaw, 0);
}
void Rotation(shen_name name, const Vector3i &rotation, shen_name parent = invalid) {
    EventSetRawAngle request{0, name, parent, rotation, 0.0f};
    EVT_AT_SetAng_Callback(&request);
}
void Motion(shen_name name, std::uint32_t motion) {
    EventSetMotion request{.cleanup = 0, .character = name, .animation = motion, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&request);
    GetMotionFrameCount(static_cast<std::uint16_t>(motion));
}
void FadeIn() {
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
}
void InitialPositions(bool includePrizeDuck) {
    Position(ryo, Vector3{184.7f, 107.2f, 309.8f}, 315.0f);
    Position(xgh, Vector3{185.5f, 107.2f, 310.2f}, 337.5f);
    Position(coh, Vector3{187.1f, 107.3f, 307.1f}, 180.0f);
    if (includePrizeDuck)
        Position(aa95, Vector3{185.6f, 107.4f, 309.6f}, 320.0f);
}
void InitialMotions(bool includePrizeDuck) {
    Motion(ryo, 61442);
    Motion(xgh, 32532);
    Motion(coh, 32518);
    if (includePrizeDuck) {
        ConfigureScenarioNodeMotion(aa95, 32526, 0, 32767, 0, 1, 0);
        ConfigureScenarioNodeMotion(aa95, 0, -1, -1, -1, 32767, 4);
    }
}
void SetRaceFieldVisibility(bool paddock) {
    constexpr shen_name names[]{SHEN_NAME(DIC1) | 0x282628U, SHEN_NAME(DAI2) | 0x222e28U, SHEN_NAME(a800) | 0x4004004U, SHEN_NAME(a005) | 0x4804U,
        SHEN_NAME(f11a), SHEN_NAME(f11b), SHEN_NAME(f21a), SHEN_NAME(f21b), SHEN_NAME(f91a), SHEN_NAME(f91b), SHEN_NAME(fa1a), SHEN_NAME(fa1b)};
    constexpr bool states[]{true, false, false, false, true, true, true, true, false, false, false, false};
    for (std::size_t i = 0; i < std::size(names); ++i)
        ToggleFlagOnFieldEntity(names[i], states[i] == paddock);
}
void PaddockActors() {
    for (std::int32_t i = 0; i < 5; ++i) {
        const shen_name first{static_cast<std::uint32_t>(SHEN_NAME(PAI0) + i * 0x01000000U)},
            second{static_cast<std::uint32_t>(SHEN_NAME(PAI5) + i * 0x01000000U)};
        Position(first, Vector3{224.3f, 108.8f, static_cast<float>(i) + 333.2f}, 0);
        Position(second, Vector3{static_cast<float>(i) * 1.2f + 219.0f, 109.0f, static_cast<float>(i) * 0.4f + 330.2f}, 0);
        EV_EnableCharacterDisplay(first);
        EV_EnableCharacterDisplay(second);
        modelSetCharaAlpha(FindCharacterByScenarioName(first), 1);
        modelSetCharaAlpha(FindCharacterByScenarioName(second), 0);
        SetRaceFieldVisibility(true);
    }
}
void FadePaddock_Callback(void *) {
    state.paddockAlpha -= 0.05f;
    for (std::int32_t i = 0; i < 5; ++i) {
        modelSetCharaAlpha(FindCharacterByScenarioName(shen_name{static_cast<std::uint32_t>(SHEN_NAME(PAI0) + i * 0x01000000U)}), state.paddockAlpha);
        modelSetCharaAlpha(FindCharacterByScenarioName(shen_name{static_cast<std::uint32_t>(SHEN_NAME(PAI5) + i * 0x01000000U)}), 1 - state.paddockAlpha);
    }
    if (state.paddockAlpha <= 0) {
        SetRaceFieldVisibility(false);
        state.paddockAlpha = 1;
        HLib::DestroyCurrentTask();
    }
}
void SelectRoster() {
    std::array<bool, 12> selected{};
    for (auto &profile : state.profiles) {
        do
            profile = shenGetRandomInt32(12);
        while (selected[profile]);
        selected[profile] = true;
        EV_debug_log("DUCK\\eDUCK_et_duck_race.c", 939, "Entry %d: %d ", static_cast<int>(&profile - state.profiles.data()), profile);
    }
    EV_debug_log("DUCK\\eDUCK_et_duck_race.c", 941, "\n");
}
void HideRoster() {
    duckRaceRuntime.finishCount = duckRaceRuntime.actorCount = 0;
    duckRaceRuntime.finishNames.fill(shen_name{0});
    duckRaceRuntime.finishTicks.fill(0);
    duckRaceRuntime.names.fill(shen_name{0});
    duckRaceRuntime.actors.fill(nullptr);
    constexpr shen_name allNames[]{SHEN_NAME(duk1), SHEN_NAME(duk2), SHEN_NAME(duk3), SHEN_NAME(duk4), SHEN_NAME(duk5), SHEN_NAME(duk6), SHEN_NAME(duk1),
        SHEN_NAME(duk2), SHEN_NAME(duk3), SHEN_NAME(duk4), SHEN_NAME(duk5), SHEN_NAME(duk6), SHEN_NAME(peng), SHEN_NAME(KAMO), SHEN_NAME(KAMM), SHEN_NAME(KAK0),
        SHEN_NAME(KAK1), SHEN_NAME(KAK2), SHEN_NAME(KAK3), SHEN_NAME(KAK4), SHEN_NAME(KAK5), SHEN_NAME(duk1), SHEN_NAME(duk1)};
    for (auto name : allNames)
        EV_DisableCharacterDisplay(name);
}
void DuckActor_Callback(void *data) {
    const auto *work = static_cast<const DuckEventWork *>(HLib::GetTaskParameter(state.main));
    if (work->state == DuckState::PostRaceHold || work->state == DuckState::Running)
        TickDuckRaceActor(static_cast<DuckRaceActor *>(data));
}
void QueueActor(std::int32_t profile) {
    auto *task = HLib::EnqueueTaskWithParameter(DuckActor_Callback, 3, 0x0b, sizeof(DuckRaceActor), SHEN_NAME(DUCK) | 0x20202020U);
    auto *actor = static_cast<DuckRaceActor *>(HLib::GetTaskParameter(task));
    std::memset(actor, 0, sizeof(*actor));
    actor->self = task;
    InitializeDuckRaceActor(actor, profile);
    const auto index = duckRaceRuntime.actorCount++;
    duckRaceRuntime.names[index] = actor->name;
    duckRaceRuntime.actors[index] = task;
    actor->index = index;
}
void ShuffleGrid() {
    std::array<bool, 6> selected{};
    for (std::int32_t slot = 0; slot < duckRaceRuntime.actorCount; ++slot) {
        std::int32_t index;
        do
            index = shenGetRandomInt32(duckRaceRuntime.actorCount);
        while (selected[index]);
        selected[index] = true;
        auto *actor = static_cast<DuckRaceActor *>(HLib::GetTaskParameter(duckRaceRuntime.actors[index]));
        EV_EnableCharacterDisplay(actor->name);
        if (g_currentMap == AREA_WT00 || g_currentMap.name == SHEN_NAME(DUCK))
            Position(actor->name, Vector3{188.8f, actor->sizeScale * 0.15f + 107.2f, static_cast<float>(slot) * 0.6f + 310.0f}, 270);
    }
}
void RebuildRoster() {
    for (auto *&task : duckRaceRuntime.actors)
        if (task) {
            HLib::DestroyTask(task);
            task = nullptr;
        }
    SelectRoster();
    HideRoster();
    PaddockActors();
    for (auto profile : state.profiles)
        QueueActor(profile);
    ShuffleGrid();
    duckRaceRuntime.ticks = 0;
    SnapDuckRaceCameraTarget();
    PaddockActors();
}
void BuildPlayerRoster() {
    std::array<std::int32_t, 4> stats{};
    for (std::uint32_t i = 0; i < 4; ++i)
        stats[i] = static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, static_cast<std::int16_t>(50 + i)));
    duckRaceRuntime.speedExperience = duckRaceRuntime.staminaExperience = duckRaceRuntime.gutsExperience = duckRaceRuntime.turnExperience = 0;
    const auto total = stats[0] + stats[1] + stats[2] + stats[3];
    auto *actor = static_cast<DuckRaceActor *>(HLib::GetTaskParameter(duckRaceRuntime.actors[0]));
    InitializeDuckRaceActor(actor, 21);
    actor->normalSpeed += static_cast<float>(stats[0]) * 0.00025417574f;
    actor->maximumSpeed += static_cast<float>(stats[0]) * 0.00025417577f;
    actor->minimumSpeed += static_cast<float>(stats[0]) * 0.00010893246f;
    actor->maximumStamina = actor->stamina =
        TruncateToInt32((static_cast<float>(stats[1]) * 0.0015686273f + 1.0f) * static_cast<float>(duckRaceRuntime.staminaScale));
    actor->guts = static_cast<std::uint16_t>(actor->guts + TruncateToInt32(static_cast<float>(stats[2]) * 0.2352941176470588f));
    actor->handling = static_cast<std::int16_t>(actor->handling + TruncateToInt32(static_cast<float>(stats[3]) * 0.1568627450980392f));
    EV_debug_log("DUCK\\eDUCK_et_duck_ctrl.c", 477, "PlayerStatus\n");
    EV_debug_log("DUCK\\eDUCK_et_duck_ctrl.c", 478, "    SpdNml %f\n", actor->normalSpeed * 30.0 * 60.0 * 60.0 / 1000.0);
    EV_debug_log("DUCK\\eDUCK_et_duck_ctrl.c", 479, "    SpdMax %f\n", actor->maximumSpeed * 30.0 * 60.0 * 60.0 / 1000.0);
    EV_debug_log("DUCK\\eDUCK_et_duck_ctrl.c", 480, "    SpdMin %f\n", actor->minimumSpeed * 30.0 * 60.0 * 60.0 / 1000.0);
    EV_debug_log("DUCK\\eDUCK_et_duck_ctrl.c", 481, "    Stamina %d\n", actor->maximumStamina);
    EV_debug_log("DUCK\\eDUCK_et_duck_ctrl.c", 482, "    Guts   %d\n", actor->guts);
    EV_debug_log("DUCK\\eDUCK_et_duck_ctrl.c", 483, "    Int    %d\n", actor->handling);
    duckRaceRuntime.names[0] = actor->name;
    actor->aiControlled = 0;
    auto replace = [](std::int32_t index, std::int32_t profile) {
        auto *other = static_cast<DuckRaceActor *>(HLib::GetTaskParameter(duckRaceRuntime.actors[index]));
        EV_DisableCharacterDisplay(other->name);
        InitializeDuckRaceActor(other, profile);
        duckRaceRuntime.names[index] = other->name;
    };
    if (!shenGetRandomInt32(5) && total == 800 && EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1646) && GetCurrentWeather() == 2) {
        for (std::int32_t i = 0; i < 5; ++i)
            replace(i + 1, i + 12);
    } else if (GetCurrentWeather() == 2 && EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1646) && total > 700)
        replace(1, 12);
    else if (total > 0 && total % 14 == 0) {
        for (std::int32_t i = 0; i < 5; ++i)
            replace(i + 1, i + 14);
    } else if (total > 600 && EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1646)) {
        for (std::int32_t i = 0; i < 5; ++i)
            replace(i + 1, i + 13);
    } else if (total > 500 && EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1646)) {
        replace(1, 13);
        replace(2, 14);
    } else if (total > 400 && EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1646))
        replace(1, 13);
    else if (total > 300 && EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1646))
        replace(1, 14);
    else if (total > 200 && EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1646)) {
        for (std::int32_t i = 0; i < 3; ++i)
            replace(i + 1, i + 15);
    }
}
void Obstacles(std::int32_t configuration) {
    if (configuration >= 0 && configuration <= 3) {
        for (std::size_t i = 0; i < 6; ++i)
            EVT_AT_SetPos(obstacleNames[i], invalid, authoredCoordinates[configuration * 6 + i], 0);
        for (std::size_t i = 6; i < 9; ++i)
            EVT_AT_SetPos(obstacleNames[i], invalid, authoredCoordinates[24 + i - 6], 0);
    }
    for (auto name : obstacleNames)
        Rotation(name, Vector3i{0, 0, 0x4000});
    constexpr shen_name jdm1{SHEN_NAME(JDM1)}, jdm2{SHEN_NAME(JDM2)}, jmp1{SHEN_NAME(JMP1)};
    if (!FindCharacterByScenarioName(jdm1))
        CreateEventCharacter(CharacterCreationHighModel, nullptr, jdm1);
    if (!FindCharacterByScenarioName(jdm2))
        CreateEventCharacter(CharacterCreationHighModel, nullptr, jdm2);
    const Vector3 scale{0.01f, 4, 10};
    SetMotionworkFlagForCharaById(jmp1, &scale);
    EVT_AT_SetPos(jmp1, invalid, authoredCoordinates[27], 0);
    Rotation(jmp1, Vector3i{45511, 1820, 20480});
    EVT_AT_SetPos(jdm1, jmp1, Vector3{}, 0);
    Rotation(jdm1, Vector3i{0, 45056, 0});
    EVT_AT_SetPos(jdm2, invalid, authoredCoordinates[28], 0);
    Rotation(jdm2, Vector3i{0, 0x4000, 0});
    EV_debug_log("DUCK\\eDUCK_et_duck_obstacle.c", 104, "CreateInitialize Obstacle\n");
}
void RecalculateOdds() {
    std::array<std::int32_t, 6> ratings{};
    std::int32_t total{};
    for (std::size_t i = 0; i < 6; ++i) {
        auto *actor = static_cast<DuckRaceActor *>(HLib::GetTaskParameter(duckRaceRuntime.actors[i]));
        const auto &profile = duckProfiles[actor->profile];
        auto &rating = ratings[i];
        rating = profile.speedMax > 0.2268518518518519 ? 6 : profile.speedMax > 0.212962962962963 ? 4 : profile.speedMax > 0.1990740740740741 ? 2 : 1;
        rating += profile.speedNormal > 0.1388888888888889 ? 2 : 1;
        rating += profile.staminaBase > 1.25 ? 3 : profile.staminaBase > 1.15 ? 2 : profile.staminaBase > 1.05 ? 1 : 0;
        const auto guts = static_cast<std::int16_t>(profile.guts);
        rating += guts > 35 ? 5 : guts > 25 ? 4 : guts > 15 ? 3 : guts > 10 ? 2 : guts > 5 ? 1 : 0;
        total += rating;
    }
    for (std::size_t i = 0; i < 6; ++i)
        state.odds[i] = 10 * total / ratings[i];
}
void OddsBoard() {
    constexpr const char *images[]{"DACKMS0G", "DACKMS1G", "DACKMS2G", "DACKMS3G", "DACKMS4G", "DACKMS5G", "DACKMS6G", "DACKMS7G", "DACKMS8G", "DACKMS9G"};
    RecalculateOdds();
    for (std::size_t i = 0; i < 6; ++i) {
        std::int32_t divisor = 10000;
        for (std::size_t j = 0; j < 3; ++j, divisor /= 10) {
            const auto digit = 10 * state.odds[i] % divisor / (divisor / 10);
            ReplaceScenarioImageModule(state.digitCharacters[i * 3 + j], divisor == 10000 && !digit ? nullptr : images[digit], 0);
        }
    }
}
void FallMoney_Callback(void *) {
    AT_SetLocalBaseChara(money, base);
    Vector3 position{};
    Vector3i rotation{};
    EV_GetCharaPosition(money, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    GetScenarioCharacterEulerAngles(money, &rotation, EventTransformFlags::Local);
    position.y -= 0.01f;
    if (position.y < 0.7f) {
        position.y = 0.7f;
        Loop3DSoundEffect(0x4cc, 0.7f);
        HLib::DestroyCurrentTask();
    }
    EV_AT_SetPos(money, EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(money, EventTransformFlags::AllAxes, rotation);
}
struct DuckVoiceWork {
    HLib::Task *parent;
    std::int32_t delay, index, reserved10, ticks;
    std::uint32_t mode, reserved1c;
};
static_assert(sizeof(DuckVoiceWork) == 0x20);
void AddVoice(const char *suffix) {
    std::strcpy(state.voices[state.voiceCount].data(), state.voicePrefix.data());
    std::strcat(state.voices[state.voiceCount++].data(), suffix);
}
void VoiceSequence_Callback(void *data) {
    auto *work = static_cast<DuckVoiceWork *>(data);
    if (cvsHasValidConverseData()) {
        if (work->ticks++ > 600)
            scenCvsCtrlFree();
        if (CheckMediatedDoublePress() && (work->mode & 0x40000000U))
            scenCvsCtrlFree();
    } else {
        const auto *voice = state.voices[work->index].data();
        if (*voice) {
            EVT_AT_SetVoice(voice, 1, work->delay);
            if (work->delay > 0)
                HLib::SetupTaskDelay(work->delay);
            ++work->index;
            work->ticks = 0;
            if (state.voices[work->index][0] || !(work->mode & 0x20000000U))
                return;
        }
        if (work->parent)
            HLib::ResolveDeferredCallback(work->parent);
        HLib::DestroyCurrentTask();
    }
}
void QueueVoiceSequence() {
    auto *task = HLib::EnqueueTaskWithParameter(VoiceSequence_Callback, 6, 0x0b, sizeof(DuckVoiceWork), event);
    auto *work = static_cast<DuckVoiceWork *>(HLib::GetTaskParameter(task));
    *work = {state.main, 8, 0, 0, 0, 0x40000000U, 0};
    state.voices[state.voiceCount][0] = 0;
    state.voiceCount = 0;
    if (work->parent)
        HLib::SetupTimedTask(work->parent, 0);
}
void QueuePaddockInfo(std::int32_t index) {
    if (state.paddock)
        HLib::EV_HLKillOtherHierarchyTask(state.paddock);
    state.paddock = HLib::EnqueueTaskWithParameter(PaddockInfo_Callback, 4, 0x0b, 4, SHEN_NAME(pifo));
    *static_cast<std::int32_t *>(HLib::GetTaskParameter(state.paddock)) = index;
}
void PaddockInfo_Callback(void *data) {
    const auto index = *static_cast<std::uint16_t *>(data);
    auto *actor = static_cast<DuckRaceActor *>(HLib::GetTaskParameter(duckRaceRuntime.actors[index]));
    const auto &profile = duckProfiles[actor->profile];
    constexpr const char *entry[]{"0122C024", "0122C025", "0122C026", "0122C027", "0122C028", "0122C029"};
    const char *voices[]{entry[index],
        profile.speedMax > 0.2268518518518519     ? "0122C030" : // @text EN: "“A”for speed!" | JA: "スピードＳ！"
            profile.speedMax > 0.212962962962963  ? "0122C031" : // @text EN: "“B”for speed." | JA: "スピードＡ。"
            profile.speedMax > 0.1990740740740741 ? "0122C032" : // @text EN: "“C”for speed." | JA: "スピードＢ。"
                                                     "0122C033", // @text EN: "“D”for speed." | JA: "スピードＣ。"
        profile.staminaBase > 1.25     ? "0122C034" : // @text EN: "“A”for stamina." | JA: "スタミナＳ！"
            profile.staminaBase > 1.15 ? "0122C035" : // @text EN: "“B”for stamina." | JA: "スタミナＡ。"
            profile.staminaBase > 1.05 ? "0122C036" : // @text EN: "“C”for stamina." | JA: "スタミナＢ。"
                                          "0122C037", // @text EN: "“D”for stamina." | JA: "スタミナＣ。"
        static_cast<std::int16_t>(profile.guts) > 15     ? "0122C038" : // @text EN: "“A”for guts." | JA: "根性Ｓ！"
            static_cast<std::int16_t>(profile.guts) > 10 ? "0122C039" : // @text EN: "“B”for guts." | JA: "根性Ａ。"
            static_cast<std::int16_t>(profile.guts) > 5  ? "0122C040" : // @text EN: "“C”for guts." | JA: "根性Ｂ。"
                                                           "0122C041", // @text EN: "“D”for guts." | JA: "根性Ｃ。"
        profile.aiStyleId == 0     ? "0122C042" : // @text EN: "Good at holding opponents down." | JA: "脚質は逃げだ。"
            profile.aiStyleId == 1 ? "0122C043" : // @text EN: "Good at getting a head start." | JA: "先行逃げ切り型だ。"
            profile.aiStyleId == 2 ? "0122C044" : // @text EN: "Has sharp and quick legs." | JA: "脚質は切れ味のある差しだ。"
                                     "0122C045", // @text EN: "Strong in the home stretch." | JA: "追い込みが得意だ。"
        nullptr};
    QueueEventVoiceList(nullptr, voices, 4, 0, 0);
    HLib::SetupTaskDelay(0);
}
void DrawStandings() {
    if (!state.playerRace || !state.debugStandings)
        return;
    for (std::size_t i = 0; i < 4; ++i) {
        auto *text = state.statistics[i];
        text->depth = 8;
        state.statisticPackets[i].quad.flags |= 0x400;
        TransformScreenSpace(state.statisticPackets[i].quad, ScreenAlignment::HorizontalStretch, 32, 64 + static_cast<std::int32_t>(i) * 32, 128, 32, 1, 1, 0,
            ScreenMode::AdjustedWithOffset);
        LegacySpriteNullsub(state.statisticPackets[i], static_cast<std::uint32_t>(state.statisticTextures[i]), 8);
        const auto value = state.previousExperience[i], count = value / 100 > 0 ? 3 : value / 10 > 0 ? 2 : 1;
        std::sprintf(text->text.data(), "%d", value);
        auto x = count == 3 ? 176.0f : count == 2 ? 192.0f : 210.0f;
        if (duckRaceRuntime.experiencePublished) {
            const auto current = static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, static_cast<std::int16_t>(50 + i)));
            std::sprintf(text->text.data() + count, "  %3d", current);
            x += 58;
            state.statisticPackets[4].quad.flags |= 0x400;
            TransformScreenSpace(state.statisticPackets[4].quad, ScreenAlignment::HorizontalStretch, 226, 64 + static_cast<std::int32_t>(i) * 32, 32, 32, 1, 1,
                0, ScreenMode::AdjustedWithOffset);
            LegacySpriteNullsub(state.statisticPackets[4], static_cast<std::uint32_t>(state.statisticTextures[4]), 8);
        }
        text->scaleX = text->scaleY = 0.7f;
        text->x = x;
        text->y = static_cast<float>(i) * 32 + 80;
        text->colour = 0xffffffffU;
        text->draw(text);
    }
}
void InitializeRace(DuckEventWork *work) {
    CreateEventCharacter(CharacterCreationHighModel, nullptr, pcam);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, hdum);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("DUCK"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("DUCK"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("DUCK"), 0);
    EV_LoadCutsceneFromCurrentScene("0122E000");
    EV_LoadAudio("0122E000", 0);
    EV_FreeAuthorPak(1, "0122");
    std::strcpy(state.voicePrefix.data(), "0122");
    EV_LoadNPCRoute("NPC_DUCK.BIN");
    CreateEventCharacter(CharacterCreationHighModel, nullptr, base);
    SetLowQualityHands(FindCharacterByScenarioName(hand));
    SetHighModelXYZ(FindCharacterByScenarioName(hand), 1, 0, 0);
    AT_SetLocalBaseChara(hdum, board);
    AT_SetLocalBaseChara(hand, hdum);
    LoadGauge();
    SetFightGaugeVisible(0);
    Position(medal, Vector3{194.0f, 107.3f, 311.2f}, 90, board);
    Position(SHEN_NAME(GOAL), Vector3{194.0f, 107.22f, 311.2f}, 90);
    Position(base, Vector3{187.15f, 107.2f, 308.03f}, 0);
    Position(board, Vector3{}, 0, base);
    EVT_AT_SetPos(medal, board, Vector3{0.4f, 0.71f, 0}, 0);
    Rotation(medal, Vector3i{0xc000, 0, 0}, board);
    EV_DisableCharacterDisplay(medal);
    EVT_AT_SetPos(hand, hdum, Vector3{-0.18f, 0, 0.57f}, 0);
    Rotation(hand, Vector3i{0, 4096, 12288}, hdum);
    constexpr float xs[]{-0.53f, -0.47f, -0.41f, -0.33f, -0.27f, -0.21f, -0.13f, -0.07f, -0.01f, 0.07f, 0.13f, 0.19f, 0.27f, 0.33f, 0.39f, 0.47f, 0.53f, 0.59f};
    for (std::size_t i = 0; i < 18; ++i) {
        const auto name = CreateEventCharacter(CharacterCreationHighModel, reinterpret_cast<const std::byte *>("DACKMS0G"), shen_name{0});
        state.digitCharacters[i] = name;
        EV_EnableCharacterDisplay(name);
        Position(name, Vector3{xs[i] - 0.06f, 0, 0}, 0, board);
    }
    for (auto *&text : duckRaceRuntime.placementText)
        text = CreateLegacyHudText();
    const void *const ribbon = LoadCurrentPakBinary("ribon01.SPR");
    duckRaceRuntime.placementText[0]->loadGlyph(ribbon);
    constexpr const char *labels[]{"SPEED", "STAMINA", "GUTS", "TURN", ">"};
    for (std::size_t i = 0; i < 5; ++i) {
        auto &packet = state.statisticPackets[i];
        packet = {};
        packet.colour = 0xffffffffU;
        packet.uv = {0, 0, 1, 1};
        state.statistics[i] = CreateLegacyHudText();
        state.statisticTextures[i] = static_cast<std::int32_t>(SetupUIElement(nullptr, labels[i], i == 4 ? 32 : 128, -1, static_cast<std::int16_t>(0xf000), 0));
    }
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1646))
        EV_DisableCharacterDisplay(aa95);
    duckRaceRuntime.courseLength = 260;
    duckRaceRuntime.staminaScale = 1040;
    SelectRoster();
    HideRoster();
    PaddockActors();
    for (auto profile : state.profiles)
        QueueActor(profile);
    ShuffleGrid();
    duckRaceRuntime.ticks = 0;
    duckRaceRuntime.followedActor = 0;
    duckRaceRuntime.cameraIndex = 1;
    CreateEventCharacter(CharacterCreationHighModel, nullptr, SHEN_NAME(BALL) | 0x13010001U);
    SnapDuckRaceCameraTarget();
    CreateDuckRaceDust();
    PaddockActors();
    work->state = g_entryPoint ? DuckState::IntroAuth : DuckState::EntryDialogue;
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1021, 1);
    flddSetPointFlagOnGrid(SHEN_NAME(dwl1), 1);
    flddSetPointFlagOnGrid(SHEN_NAME(dwl2), 1);
    flddSetPointFlagOnGrid(SHEN_NAME(0000), 0);
    flddSetPointFlagOnGrid(SHEN_NAME(PROP), 0);
    EV_NPC_NewLoadSw(1);
}
void EntryEligibility(DuckEventWork *work) {
    switch (work->phase) {
    case 0:
        Queue_EVT_UTL_LoadNPCChara_3(introCharacters, HLib::GetCurrentTask());
        ++work->phase;
        break;
    case 1:
        FadeIn();
        QueueSkAuthorStart(
            HLib::GetCurrentTask(), EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1335) ? SHEN_NAME(ST02) : SHEN_NAME(ST01), nullptr, nullptr, 0);
        ++work->phase;
        break;
    case 2:
        for (auto *name = introCharacters; name->name != invalid.name; ++name)
            if (FindCharacterByScenarioName(*name)) {
                EV_npcLockUnlock(*name);
                EV_npcFree(*name);
            }
        work->state = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1335) ? DuckState::EntryDialogue :
            GetPlayerStat(PlayerStatsId::Cash) >= 20                       ? DuckState::BetMenu :
                                                                             DuckState::InsufficientFunds;
        work->phase = 0;
        break;
    }
}
std::int32_t EntryDialogue(DuckEventWork *work) {
    SetupHandPoseScript(FindCharacterByScenarioName(xgh));
    if (!EV_GetScreenFilmIsNotShowing())
        FadeIn();
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1335) || (state.paidGame && GetPlayerStat(PlayerStatsId::Cash) < 50)) {
        work->state = DuckState::BetMenu;
        return 0;
    }
    if (work->phase) {
        work->phase = 0;
        work->state = shenGetCtrlSpeakIconFlag() == 382 ? DuckState::PayEntry : DuckState::BettingIntro;
        return 0;
    }
    EV_EnableCharacterDisplay(ryo);
    EV_EnableCharacterDisplay(xgh);
    EV_EnableCharacterDisplay(coh);
    InitialPositions(true);
    InitialMotions(true);
    Camera(4000);
    ++work->phase;
    std::int32_t delay{};
    if (state.paidGame) {
        EVT_AT_SetVoice("0122C009", 1, 0); // @text EN: "It'll be $50 a try if you're going\nto enter the race." | JA: "エントリーするなら\n１レース５０ドルだぜい。"
        if (GetPlayerStat(PlayerStatsId::Cash) < 50) {
            work->state = DuckState::BettingIntro;
            work->phase = 0;
            return 100;
        }
        QueueSkChoiceWait(state.main, 0, 382, 34, -1, -1, 0, 100);
    } else {
        if (!g_entryPoint) {
            EVT_AT_SetVoice("0122C008", 1, 0); // @text EN: "Hey, you've got a nice duck there." | JA: "おっ。兄ちゃん、\nいいアヒルだな。"
            EVT_AT_SetVoice("0122C009", 1, 120); // @text EN: "It'll be $50 a try if you're going\nto enter the race." | JA: "エントリーするなら\n１レース５０ドルだぜい。"
            delay = 220;
        }
        if (GetPlayerStat(PlayerStatsId::Cash) < 50) {
            EVT_AT_SetVoice("0122A013", 1, delay); // @text EN: "Not enough to enter." | JA: "今は持ち合わせが…"
            EVT_AT_SetVoice("0122C010", 1, delay + 60); // @text EN: "Do you want to place a bet?" | JA: "じゃあ、賭けて遊ぶかい？"
            Camera(1000, ryo, ryo, delay);
            Camera(4150, invalid, invalid, delay + 60);
            work->state = DuckState::BetMenu;
            work->phase = 0;
            delay += 120;
        } else {
            QueueSkChoiceWait(state.main, 0, 382, 34, -1, -1, 0, delay);
            delay = 0;
        }
        state.paidGame = 1;
    }
    return delay;
}
std::int32_t PayEntry(DuckEventWork *work) {
    if (!work->phase) {
        Camera(1000, ryo, ryo);
        EV_EventSetUpHelpIcon_0(9, 0);
        AddVoice("A011");
        AddThisToCashOrSegaCoins(-50, ECashOrCoins::Cash2);
        ++work->phase;
        QueueVoiceSequence();
    } else {
        Camera(4100);
        AddVoice("C011");
        AddVoice("C018");
        AddVoice("C012");
        state.playerRace = 1;
        duckRaceRuntime.experiencePublished = 0;
        for (std::int16_t i = 0; i < 4; ++i)
            state.previousExperience[i] = static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 50 + i));
        BuildPlayerRoster();
        SetDuckRacePreviewCameraTarget(0);
        work->state = DuckState::Countdown;
        work->phase = 0;
        QueueVoiceSequence();
    }
    return 0;
}
std::int32_t BettingIntro(DuckEventWork *work) {
    if (!work->phase) {
        Camera(1000, ryo, ryo);
        AddVoice("A010");
        QueueVoiceSequence();
        ++work->phase;
    } else {
        Camera(4150);
        EV_PlayDialogue("0122C010");
        QueueSkChoiceWait(state.main, 0, 36, 480, 34, 34, 0, 80);
        EV_EventSetUpHelpIcon_0(8, 0);
        work->state = DuckState::BetMenu;
        work->phase = 1;
    }
    return 0;
}
std::int32_t ExitDialogue(DuckEventWork *work, bool insufficient) {
    Camera(1000, ryo, ryo);
    Camera(4150, invalid, invalid, insufficient ? 60 : 40);
    EV_PlayDialogue(insufficient ? "0122A013" : "0122A010");
    EVT_AT_SetVoice("0122C004", 1, insufficient ? 60 : 40); // @text EN: "All right.\nCome again." | JA: "そうかい、また来な。"
    work->state = DuckState::Cleanup;
    return insufficient ? 120 : 100;
}
std::int32_t BettingMenu(DuckEventWork *work) {
    if (work->phase) {
        state.menuShown = 1;
        const auto selection = shenGetCtrlSpeakIconFlag();
        work->state = selection == 36 ? DuckState::WagerInput : selection == 480 ? DuckState::PaddockView : DuckState::Decline;
        work->phase = 0;
        return 0;
    }
    if (!EV_GetScreenFilmIsNotShowing())
        FadeIn();
    EV_EventSetUpHelpIcon_0(8, 0);
    InitialPositions(true);
    InitialMotions(true);
    Camera(4020);
    if (!state.menuShown && !g_entryPoint)
        EVT_AT_SetVoice("0122C001", 1, 0); // @text EN: "Hi, do you want to play?" | JA: "よう、兄ちゃんも遊んでくかい？"
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1646))
        EV_DisableCharacterDisplay(aa95);
    if (GetPlayerStat(PlayerStatsId::Cash) >= 20) {
        QueueSkChoiceWait(state.main, 0, 36, 480, 34, 34, 0, state.menuShown || g_entryPoint ? 0 : 80);
        ++work->phase;
        return 0;
    }
    work->state = DuckState::InsufficientFunds;
    return EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1335) ? 0 : 80;
}
std::int32_t WagerInput(DuckEventWork *work) {
    if (work->phase == 0) {
        EV_PlayDialogue("0122C002");
        Camera(4010);
        QueueSkMoneyIconWait(state.main, 1000, 20);
        ++work->phase;
    } else if (work->phase == 1) {
        work->wager = static_cast<std::int32_t>(GetNumBet());
        if (work->wager <= 0)
            work->state = DuckState::BetMenu;
        else {
            SetPlayerStat(PlayerStatsId::Cash, static_cast<std::uint32_t>(GetPlayerStat(PlayerStatsId::Cash) - work->wager));
            work->state = DuckState::SelectDuck;
        }
        work->phase = 0;
    }
    return 0;
}
std::int32_t DuckSelection(DuckEventWork *work) {
    if (work->phase == 0) {
        work->selection = 0;
        work->phase = 1;
        EV_EnableCharacterDisplay(hand);
        EV_EnableCharacterDisplay(money);
        Camera(5500, hdum, hdum);
        Motion(coh, 32533);
        EVT_AT_SetPos(hdum, board, Vector3{-0.5f, 0.9f, 0.15f}, 0);
        EVT_AT_SetPos(money, hand, Vector3{0.2f, -0.18f, -0.5f}, 0);
        Rotation(money, Vector3i{0, 0, 20480}, hand);
        OddsBoard();
        ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(hand), 0, reinterpret_cast<std::byte *>(holdHand.data()), 0);
        AT_SetLocalBaseChara(money, hand);
    } else if (work->phase != 1)
        return 0;
    const auto input = static_cast<std::int16_t>(GetCurrentControlInput(7));
    if (input & 0x4000)
        --work->selection;
    if (input < 0)
        ++work->selection;
    work->selection = std::clamp(work->selection, 0, 5);
    Vector3 position{};
    EV_GetCharaPosition(hdum, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    position.x = (static_cast<float>(work->selection) * 0.2f - 0.5f) * 0.2f + position.x * 0.8f;
    EVT_AT_SetPos(hdum, board, Vector3{position.x, 0.9f, 0.15f}, 0);
    if (!(input & 0x400) || std::fabs(position.x - (static_cast<float>(work->selection) * 0.2f - 0.5f)) >= 0.01f)
        return 0;
    state.playerRace = 0;
    work->state = DuckState::Countdown;
    work->phase = 0;
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(hand), 0, reinterpret_cast<std::byte *>(releaseHand.data()), 4);
    auto *task = HLib::EnqueueTaskWithoutParameter(FallMoney_Callback, 4, 0x0b, 2037280621);
    HLib::SetupTimedTask(task, 4);
    AT_SetLocalBaseChara(money, invalid);
    SetDuckRacePreviewCameraTarget(work->selection);
    return 60;
}
std::int32_t PaddockView(DuckEventWork *work) {
    Position(base, authoredCoordinates[29], 0);
    constexpr Vector3 positions[]{
        Vector3{-1.5f, 0.2f, 0}, Vector3{-0.9f, 0.2f, 0}, Vector3{-0.3f, 0.2f, 0}, Vector3{0.3f, 0.2f, 0}, Vector3{0.9f, 0.2f, 0}, Vector3{1.5f, 0.2f, 0}};
    for (std::size_t i = 0; i < 6; ++i)
        Position(duckNames[i], positions[i], 0, base);
    Position(pcam, Vector3{}, 0, duckNames[0]);
    Camera(5000, pcam, pcam);
    CheckHelpIcon(32, 0, CheckHelpIconOp::Set);
    QueuePaddockInfo(0);
    ++work->phase;
    return 0;
}
std::int32_t PaddockBrowse(DuckEventWork *work) {
    const auto input = static_cast<std::uint16_t>(GetCurrentControlInput(7));
    if (input == 0x4000)
        ++state.browseIndex;
    else if (input == 0x8000)
        --state.browseIndex;
    state.browseIndex = std::clamp(state.browseIndex, 0, 5);
    if (state.browseIndex != state.previousBrowse) {
        QueuePaddockInfo(state.browseIndex);
        scenCvsCtrlFree();
        state.previousBrowse = state.browseIndex;
    }
    const shen_name cancel{SHEN_NAME(BTCC)};
    GetBattleDataConfig(0, cancel);
    std::int32_t index{};
    if (GetBattleDataConfig(1, cancel) && GetBattleDataConfig(2, cancel)) {
        if (state.paddock)
            HLib::EV_HLKillOtherHierarchyTask(state.paddock);
        state.paddock = nullptr;
        CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
        scenCvsCtrlFree();
        state.browseIndex = 0;
        work->state = DuckState::BetMenu;
        work->phase = 0;
    } else
        index = state.browseIndex;
    Vector3 duckPosition{}, cameraPosition{};
    EV_GetCharaPosition(duckNames[index], CHARACTER_TRANSLATION_ROOT, &duckPosition, EventTransformFlags::World);
    EV_GetCharaPosition(pcam, CHARACTER_TRANSLATION_ROOT, &cameraPosition, EventTransformFlags::World);
    cameraPosition.x += (duckPosition.x - cameraPosition.x) * 0.1f;
    cameraPosition.z += (duckPosition.z - cameraPosition.z) * 0.1f;
    cameraPosition.y += (duckPosition.y - cameraPosition.y) * 0.1f;
    EV_AT_SetPos(pcam, EventTransformFlags::World | EventTransformFlags::AllAxes, cameraPosition);
    return 0;
}
std::int32_t Countdown(DuckEventWork *work) {
    switch (work->phase) {
    case 0:
        ShuffleGrid();
        SnapDuckRaceCameraTarget();
        state.debugStandings = 0;
        Obstacles(state.playerRace ? shenGetRandomInt32(4) : 0);
        EV_PlayDialogue("0122C003");
        Camera(4020);
        Motion(coh, 32518);
        duckRaceRuntime.ticks = 0;
        ++work->phase;
        return 70;
    case 1:
        if (state.playerRace) {
            const auto input = static_cast<std::uint16_t>(GetCurrentControlInput(0));
            if ((input & 6) == 6)
                Obstacles(3);
            if ((input & 0x204) == 0x204)
                state.debugStandings = 1;
        }
        Loop3DSoundEffect(0x185);
        for (auto name : std::array<shen_name, 5>{ryo, xgh, coh, hand, aa95})
            EV_DisableCharacterDisplay(name);
        Camera(6000);
        Camera(6001, invalid, invalid, 45);
        ++work->phase;
        return 90;
    case 2:
        Loop3DSoundEffect(0xd2b);
        work->state = DuckState::Running;
        work->phase = 0;
        ShowElementByIndex(UIElement::DuckRace);
        break;
    }
    return 0;
}
void QueueDisplay(shen_name name, std::int32_t mode, std::int32_t delay) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 0x0b, sizeof(StepObjectDisplayWork), event);
    RegisterEventTask(task);
    *static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(task)) = {1, name, mode, static_cast<float>(delay)};
}
void AwardPrize(void *) {
    AddInventoryItem(25, 1, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1646, 1);
}
std::int32_t ResolveResults(DuckEventWork *work) {
    const bool win = state.playerRace ? duckRaceRuntime.finishNames[0].name == duckNames[0].name :
                                        duckRaceRuntime.finishNames[0].name == duckRaceRuntime.names[work->selection].name;
    const std::int32_t delay = static_cast<std::int32_t>(state.playerRace ? (win ? DuckRaceRunSpecialWinSequence() : DuckRaceRunSpecialLossSequence()) :
                                                                            (win ? DuckRaceRunBettingWinSequence(work) : DuckRaceRunBettingLossSequence()));
    RebuildRoster();
    SetFightGaugeVisible(0);
    DestroyElementByIndex(UIElement::DuckRace);
    work->state = DuckState::EntryDialogue;
    return delay;
}
void TickRace(DuckEventWork *work) {
    if (duckRaceRuntime.followCamera) {
        UpdateDuckRaceFollowCamera();
        ++duckRaceRuntime.ticks;
    }
    DrawStandings();
    UpdateDuckRacePlacementText(6, 0, duckRaceRuntime.ticks);
    if (duckRaceRuntime.finishCount == duckRaceRuntime.actorCount || duckRaceRuntime.ticks >= (state.playerRace ? 2400 : 1800)) {
        EV_debug_log("DUCK\\eDUCK_et_duck_race.c", 703, "GOAL : ");
        for (std::int32_t index = 0; index < duckRaceRuntime.actorCount; ++index) {
            EV_debug_log("DUCK\\eDUCK_et_duck_race.c", 705, "  %d:", index + 1);
            const auto name = duckRaceRuntime.finishNames[index].name;
            EV_debug_log("DUCK\\eDUCK_et_duck_race.c", 706, "%c%c%c%c", name & 255, (name >> 8) & 255, (name >> 16) & 255, (name >> 24) & 255);
        }
        EV_debug_log("DUCK\\eDUCK_et_duck_race.c", 708, "\n");
        work->state = DuckState::PostRaceHold;
    }
    if (duckRaceRuntime.ticks == 600)
        HLib::EnqueueTaskWithoutParameter(FadePaddock_Callback, 6, 0x0b, SHEN_NAME(chgo));
}
void End_Callback(void *data) {
    auto *work = static_cast<DuckEventWork *>(data);
    for (auto *&task : duckRaceRuntime.actors)
        if (task) {
            HLib::DestroyTask(task);
            task = nullptr;
        }
    ReleaseGauge();
    for (auto name : state.digitCharacters)
        SetCharaKill(name, 1);
    for (auto *text : duckRaceRuntime.placementText)
        text->release(text);
    for (std::size_t i = 0; i < 5; ++i) {
        ReleaseNamedTexture(state.statisticTextures[i]);
        state.statistics[i]->release(state.statistics[i]);
    }
    AT_SetLocalBaseChara(hand, invalid);
    AT_SetLocalBaseChara(hdum, invalid);
    SetCharaKill(base, 1);
    SetCharaKill(pcam, 1);
    SetCharaKill(hdum, 1);
    ET_AT_KillWork(work->npcLoad);
    EV_FreeAuthorPak(0, "0122");
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1021, 0);
    SetPlayerControlEnabled(0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("DUCK"), 0);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(state.main);
    HLib::EV_HLKillOtherHierarchyTask(state.debug);
    scenFreeEvent("DUCK.EV3");
    EV_NPC_NewLoadSw(0);
    EV_scenQueueLoad(g_actNumber, SHEN_NAME(WET0), 1);
}
void Main_Callback(void *data) {
    auto *work = static_cast<DuckEventWork *>(data);
    std::int32_t delay{};
    switch (work->state) {
    case DuckState::Init:
        InitializeRace(work);
        return;
    case DuckState::IntroAuth:
        EntryEligibility(work);
        return;
    case DuckState::EntryDialogue:
        delay = EntryDialogue(work);
        break;
    case DuckState::PayEntry:
        delay = PayEntry(work);
        break;
    case DuckState::BettingIntro:
        delay = BettingIntro(work);
        break;
    case DuckState::InsufficientFunds:
        delay = ExitDialogue(work, true);
        break;
    case DuckState::BetMenu:
        delay = BettingMenu(work);
        break;
    case DuckState::WagerInput:
        delay = WagerInput(work);
        break;
    case DuckState::SelectDuck:
        delay = DuckSelection(work);
        break;
    case DuckState::PaddockView:
        if (work->phase == 0)
            delay = PaddockView(work);
        else if (work->phase == 1)
            delay = PaddockBrowse(work);
        break;
    case DuckState::Decline:
        delay = ExitDialogue(work, false);
        break;
    case DuckState::PostRaceHold:
        DrawStandings();
        if (static_cast<std::int16_t>(work->phase++) >= 60) {
            work->state = DuckState::Results;
            work->phase = 0;
        }
        return;
    case DuckState::Countdown:
        delay = Countdown(work);
        break;
    case DuckState::Results:
        delay = ResolveResults(work);
        break;
    case DuckState::Running:
        TickRace(work);
        return;
    case DuckState::Cleanup:
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetCurrentTaskTo(End_Callback);
        HLib::SetupTaskDelay(30);
        return;
    }
    if (delay > 0)
        HLib::SetupTaskDelay(delay);
}
void QueueMain(void *) {
    state.debug = HLib::EnqueueTaskWithoutParameter(DebugNullsub, 4, 0x0b, SHEN_NAME(ddbg));
    state.main = HLib::EnqueueTaskWithParameter(Main_Callback, 5, 0x0b, sizeof(DuckEventWork), SHEN_NAME(rmin));
    auto *work = static_cast<DuckEventWork *>(HLib::GetTaskParameter(state.main));
    *work = {};
    duckRaceRuntime.followCamera = 1;
    state.paddock = nullptr;
    work->npcLoad = Queue_EVT_AT_ReadChr(state.main, initialCharacters);
}
} // namespace
// @verified - p
std::int64_t DuckRaceRunBettingWinSequence(const DuckEventWork *work) {
    EV_EnableCharacterDisplay(ryo);
    EV_EnableCharacterDisplay(xgh);
    EV_EnableCharacterDisplay(coh);
    InitialPositions(false);
    Motion(ryo, 61442);
    if (shenGetRandomInt32(2)) {
        hmdlSetFlagsFor(FindCharacterByScenarioName(xgh), 1, 0, 16);
        hmdlSetFlagsFor(FindCharacterByScenarioName(xgh), 0, 0, 16);
        Motion(xgh, 32528);
        Camera(2103);
    } else {
        Motion(xgh, 32529);
        Camera(2100);
    }
    Motion(coh, 32533);
    EV_PlayDialogue("0122B009");
    EVT_AT_SetVoice("0122C005", 1, 70); // @text EN: "I'm impressed.\nHere you go." | JA: "兄ちゃん、やるなあ。ほらよ。"
    EVT_AT_SetVoice("0122C017", 1, 200); // @text EN: "Another try?" | JA: "どうだい、もういっちょ？"
    Camera(2100);
    Camera(2101, invalid, invalid, 65);
    const float payout = static_cast<float>(state.odds[work->selection]) * static_cast<float>(work->wager) / 10.0f;
    PATI_StartMoneyCount(static_cast<std::uint32_t>(TruncateToInt32(payout)), 80);
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(xgh), Smile, 0, 1);
    EVT_AT_SetFaceLooks(xgh, Normal, 0, 1, 200);
    return 240;
}

// @verified - p
std::int64_t DuckRaceRunBettingLossSequence() {
    EV_EnableCharacterDisplay(ryo);
    EV_EnableCharacterDisplay(xgh);
    EV_EnableCharacterDisplay(coh);
    InitialPositions(false);
    Motion(ryo, 61442);
    if (shenGetRandomInt32(2)) {
        Motion(xgh, 32530);
        Camera(2104);
    } else {
        Motion(xgh, 32531);
        Camera(2105);
    }
    Motion(coh, 32533);
    EV_PlayDialogue("0122B010");
    EVT_AT_SetVoice("0122C007", 1, 100); // @text EN: "That was too bad, eh?" | JA: "兄ちゃん、残念だったな。"
    EVT_AT_SetVoice("0122C017", 1, 200); // @text EN: "Another try?" | JA: "どうだい、もういっちょ？"
    Camera(2100);
    Camera(2101, invalid, invalid, 100);
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(xgh), Sad, 0, 1);
    EVT_AT_SetFaceLooks(xgh, Normal, 0, 1, 200);
    return 240;
}

// @verified - p
std::int64_t DuckRaceRunSpecialWinSequence() {
    EV_EnableCharacterDisplay(ryo);
    EV_EnableCharacterDisplay(xgh);
    EV_EnableCharacterDisplay(coh);
    InitialPositions(false);
    Motion(ryo, 61442);
    if (shenGetRandomInt32(2)) {
        hmdlSetFlagsFor(FindCharacterByScenarioName(xgh), 1, 0, 16);
        hmdlSetFlagsFor(FindCharacterByScenarioName(xgh), 0, 0, 16);
        Motion(xgh, 32528);
        Camera(2103);
    } else {
        Motion(xgh, 32529);
        Camera(2100);
    }
    Motion(coh, 32533);
    EV_PlayDialogue("0122B012");
    EVT_AT_SetVoice("0122C014", 1, 70); // @text EN: "Here's your prize." | JA: "こいつは景品だ。"
    EVT_AT_SetVoice("0122C015", 1, 180); // @text EN: "How about another race?" | JA: "どうだい？　もう１レース。"
    QueueDisplay(medal, 0, 70);
    QueueDisplay(medal, 1, 241);
    Camera(2102, invalid, invalid, 70);
    QueueEventCallFunction(AwardPrize, nullptr, 70);
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(xgh), Smile, 0, 1);
    EVT_AT_SetFaceLooks(xgh, Normal, 0, 1, 180);
    return 240;
}

// @verified - p
std::int64_t DuckRaceRunSpecialLossSequence() {
    EV_EnableCharacterDisplay(ryo);
    EV_EnableCharacterDisplay(xgh);
    EV_EnableCharacterDisplay(coh);
    InitialPositions(false);
    Motion(ryo, 61442);
    if (shenGetRandomInt32(2)) {
        Motion(xgh, 32530);
        Camera(2104);
    } else {
        Motion(xgh, 32531);
        Camera(2105);
    }
    Motion(coh, 32533);
    EV_PlayDialogue("0122B013");
    EVT_AT_SetVoice("0122C016", 1, 70); // @text EN: "That was close." | JA: "兄ちゃん、惜しかったな。"
    EVT_AT_SetVoice("0122C017", 1, 130); // @text EN: "Another try?" | JA: "どうだい、もういっちょ？"
    Camera(2101, invalid, invalid, 70);
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(xgh), Sad, 0, 1);
    EVT_AT_SetFaceLooks(xgh, Normal, 0, 1, 170);
    return 170;
}

void AreaDUCKInit() {
    state = {};
    state.paddockAlpha = 1;
    duckRaceRuntime = {};
    ResetDuckRaceRuntime();
}
void AreaDUCKMain(std::uintptr_t) {
    if (g_currentMap == AREA_WT00 || g_currentMap.name == SHEN_NAME(DUCK))
        Position(ryo, Vector3{193.9f, 107.2f, 310.8f}, 270);
    SetPlayerControlEnabled(0);
    EV_EventSetUpHelpIcon_0(8, 0);
    SetPlayerControlEnabled(0);
    auto *task = HLib::EnqueueTaskWithParameter(EVT_AT_NPCClear_Callback, 6, 0x0b, sizeof(StepNpcClearWork), event);
    RegisterEventTask(task);
    auto *work = static_cast<StepNpcClearWork *>(HLib::GetTaskParameter(task));
    *work = {};
    work->callback = QueueMain;
    work->delay = 3.0f;
}
const ScriptAreaDefinition areaDUCKScriptDefinition{.areaId = SHEN_NAME(DUCK), .field04 = 0, .init = AreaDUCKInit, .main = AreaDUCKMain};
} // namespace shenmue::s2::areas
