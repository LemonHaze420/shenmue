#include "s2/areas/area_ablb.hpp"
#include "s2/pause.hpp"
#include "s2/areas/area_ablb_data.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/flag.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/hand.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include <array>
#include <bit>
#include <cstring>

namespace shenmue::s2 {
namespace alley_battle {
constexpr shen_name ryo{SHEN_NAME(RYO_)};
constexpr std::array<shen_name, 6> enemies{{SHEN_NAME(H01_), SHEN_NAME(H02_), SHEN_NAME(H03_), SHEN_NAME(H04_), SHEN_NAME(H05_), SHEN_NAME(H06_)}};
constexpr std::array<shen_name, 4> decorations{{SHEN_NAME(a073), SHEN_NAME(a417), SHEN_NAME(aa50), SHEN_NAME(a014)}};
bool colourBlendInitialized{};
struct AuthorWork {
    HLib::Task *task;
    std::uint32_t reserved08;
    std::uint16_t author, phase;
};
struct AuthorLoopWork {
    AuthorWork *parent;
    std::int16_t frame;
    std::byte reserved0a[6];
};
static_assert(sizeof(AuthorWork) == 0x10 && offsetof(AuthorWork, author) == 0x0c);
static_assert(sizeof(AuthorLoopWork) == 0x10);
void Objects(bool visible) {
    for (int i = 0; i < 13; ++i)
        EV_SetObjectFlagByIndex(i, visible && i != 10);
}
void SetAuthoredRadius(shen_name character) {
    SetScenarioCharacterRadius(character, 50.0f);
}
void AuthoredFlags(bool enabled) {
    if (!enabled)
        SetAuthoredRadius(ryo);
    mtwkSetExtraFlag(scenFindMotionwork(ryo), 6, enabled ? 0 : 2);
    EV_SetCharacterAlphaEnabled(ryo, enabled);
    for (auto name : decorations)
        EV_SetCharacterAlphaEnabled(name, enabled);
    for (auto name : authoredCharacters) {
        if (name.name == 0xffffffffU)
            break;
        if (!enabled)
            SetAuthoredRadius(name);
        mtwkSetExtraFlag(scenFindMotionwork(name), 6, enabled ? 0 : 2);
        EV_SetCharacterAlphaEnabled(name, enabled);
    }
}
void SelectAuthorPackage(std::uint16_t author) {
    const char *discard = author ? "AKA3" : "AB00";
    const char *load = author ? "AB00" : "AKA3";
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(discard), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(discard), 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>(load));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>(load), 0);
}
void AuthorLoopCallback(void *parameter) {
    auto &work = *static_cast<AuthorLoopWork *>(parameter);
    work.frame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (work.frame == 1)
        AuthoredFlags(false);
    else if (work.frame == 500 && work.parent->author == 2)
        EV_NPC_NewLoadSw(1);
    else if (work.frame == 600 && work.parent->author == 2)
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 250, 1);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        AuthoredFlags(true);
        work.parent->phase = 3;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::DestroyCurrentTask();
    }
}
void QueueAuthorLoop(AuthorWork &work) {
    auto *task = HLib::EnqueueTaskWithParameter(AuthorLoopCallback, 4, 0x0b, sizeof(AuthorLoopWork), AREA_EVENT_TASK);
    auto *loop = static_cast<AuthorLoopWork *>(HLib::GetTaskParameter(task));
    loop->parent = &work;
    loop->frame = 0;
    EV_SetModelDataDefaultVertexShader();
    switch (work.author) {
    case 0:
        SetWeatherUpdateEnabledTo(1);
        EV_lightLoad(SHEN_NAME(l001));
        PushGlobalLightingProgram();
        break;
    case 1:
        SetWeatherUpdateEnabledTo(1);
        EV_lightLoad(SHEN_NAME(l101));
        PushGlobalLightingProgram();
        break;
    case 2:
        SetWeatherUpdateEnabledTo(0);
        EV_lightLoad(SHEN_NAME(l201));
        PushGlobalLightingProgram();
        EV_lightLoad(SHEN_NAME(l202));
        break;
    }
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(work.author));
    work.phase = 2;
}
void AuthorCallback(void *parameter) {
    auto &work = *static_cast<AuthorWork *>(parameter);
    if (work.phase == 1)
        QueueAuthorLoop(work);
    else if (work.phase == 3) {
        EV_CleanupMapControlTask();
        HLib::DestroyTask(work.task);
    }
}
void QueueAuthor(std::uint8_t author) {
    auto *task = HLib::EnqueueTaskWithParameter(AuthorCallback, 4, 0x0b, sizeof(AuthorWork), AREA_EVENT_TASK);
    auto *work = static_cast<AuthorWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->author = author;
    work->phase = 1;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0138");
    SelectAuthorPackage(author);
}
void Pose(shen_name character, std::uint32_t animation, const Vector3 &position, float degrees) {
    EventSetMotion motion{0, character, animation, 0, 1, 0x40000000U, 0};
    EVT_AT_SetMot_Callback(&motion);
    EventSetPosition pos{0, character, shen_name{0xffffffffU}, position, 0};
    EVT_AT_SetPosRot(&pos);
    EventSetAngle angle{0, character, shen_name{0xffffffffU}, degrees, 0};
    EVT_AT_SetAngY_Callback(&angle);
}
void SetupBattle(AlleyBattleWork &work) {
    for (auto name : authoredCharacters) {
        if (name.name == 0xffffffffU)
            break;
        DisableScenarioCharacterDisplay(name);
        ToggleScenarioCharacterFlag(name);
    }
    for (auto name : decorations) {
        DisableScenarioCharacterDisplay(name);
        ToggleScenarioCharacterFlag(name);
    }
    for (auto name : enemies) {
        EnableScenarioCharacterDisplay(name);
        EnsureScenarioCharacterDraw(name);
    }
    Pose(ryo, 306, {251.66000366210938f, 96.8499984741211f, 492.2300109863281f}, 85.94000244140625f);
    Pose(enemies[0], 3632, {249.8300018310547f, 96.8499984741211f, 490.44000244140625f}, 235.94000244140625f);
    Pose(enemies[1], 3636, {249.74000549316406f, 96.8499984741211f, 494.19000244140625f}, 300.9200134277344f);
    Pose(enemies[2], 3640, {251.32000732421875f, 96.8499984741211f, 494.1300048828125f}, 5.909999847412109f);
    Pose(enemies[3], 3644, {252.0800018310547f, 96.8499984741211f, 490.9800109863281f}, 140.9600067138672f);
    Pose(enemies[4], 3648, {247.75999450683594f, 96.8499984741211f, 492.92999267578125f}, 295.9200134277344f);
    Pose(enemies[5], 3632, {248.72000122070312f, 96.8499984741211f, 489.739990234375f}, 265.92999267578125f);
    camCtrlSetState(CameraState::Event);
    camCtrlSetNextPosition({251.15961f, 98.82f, 487.0921f});
    camCtrlSetTargetPosition({250.2169952392578f, 98.05000305175781f, 491.5154113769531f});
    EV_LoadAudio("0138A020", 1);
    EV_spmBeginSampleExecute();
    work.delay = 30;
    EventFilmControl film{0, 30, 255, 0, 0, 0, 0, 0, 0, 0, 0};
    EVT_AT_MenCtrl_Callback(&film);
    camCtrlSetFov(0x1c71);
    work.phase = 8;
}
void FightFlags(Character *character) {
    vfUpdateFlagsFor(character, 0, 0x400000, 128);
    vfUpdateFlagsFor(character, 0, 0x200000, 2048);
}
void BeginFight(AlleyBattleWork &work) {
    shenStartFGST(9, 20, 0, 6);
    vfmDispatchControl(FightControlOp::Package, {}, reinterpret_cast<std::uintptr_t>("ABLB"), 1);
    constexpr Vector3 generators[]{{249.12f, 96.849998f, 489.23999f}, {246.92f, 96.849998f, 493.04001f}, {248.12f, 96.849998f, 495.73999f},
        {253.02f, 96.849998f, 496.23999f}, {254.92f, 96.849998f, 496.94f}, {253.12f, 96.849998f, 489.34f}};
    for (const auto &p : generators)
        vfAddGeneratorPoint(p.x, p.y, p.z);
    auto *player = ctrlGetCharaByName(ryo);
    shenInitFightData(player, nullptr, 0, 0, 0, 0);
    vfUpdateFlagsFor(player, 0, 0x20000, 31);
    for (std::size_t i = 0; i < enemies.size(); ++i) {
        auto *actor = ctrlGetCharaByName(enemies[i]);
        shenInitFightData(actor, nullptr, 1, 1, 100, static_cast<std::uint8_t>(i == 5 ? 2 : i + 2));
        FightFlags(actor);
    }

    for (const auto &spawn : reinforcements) {
        PublishFightRespawn(spawn.hp, spawn.style, spawn.animation, spawn.position.x, spawn.position.y, spawn.position.z, spawn.yaw);
        FightFlags(reinterpret_cast<Character *>(std::uintptr_t{SHEN_NAME(DEAD)}));
    }
    vfCopyPosFromEntity(0, nullptr, 250.95f, 492.92999f, 4.8f);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800}, 1);
    SetFaceFlagsForChara_0(player, FacialNodeOriginal);
    SetHighModelXYZ(player, 0, 1, 0);
    SetHighModelXYZ(player, 0, 0, 0);
    SetPluralControlFor(player, 1);
    for (auto name : characters) {
        if (name.name == 0xffffffffU)
            break;
        SetCharacterType(FindCharacterByScenarioName(name), CharacterTypeDefault);
    }
    vfmQueueFightStart(6, 2, 1, 1, -1);
    work.elapsed = 690;
    work.phase = 10;
}
void SaveWeatherColours(AlleyBattleWork &work) {
    SetWeatherUpdateEnabledTo(0);
    const auto sky = GetWeatherCloudOrSkyColour(0), cloud = GetWeatherCloudOrSkyColour(1);
    for (int i = 0; i < 4; ++i) {
        work.colours[i][2] = (sky >> (24 - 8 * i)) & 255;
        work.colours[i][4] = (cloud >> (24 - 8 * i)) & 255;
    }
}
void BlendWeatherColours(AlleyBattleWork &work) {
    auto &blend = work.colourBlend;
    if (!colourBlendInitialized) {
        colourBlendInitialized = true;
        blend[0] = std::bit_cast<float>(work.colours[0][3]);
        blend[8] = std::bit_cast<float>(work.colours[0][5]);
        for (int i = 0; i < 4; ++i) {
            blend[4 + i] = static_cast<float>(work.colours[i][2]);
            blend[12 + i] = static_cast<float>(work.colours[i][4]);
        }
        const float frames = 30.0f * work.fadeOutSeconds;
        for (int i = 1; i < 4; ++i) {
            const float target = static_cast<float>(work.colours[i][1]) * (i == 3 ? 1.0f : 2.0f);
            blend[i] = (target - blend[4 + i]) / frames;
            blend[8 + i] = (target - blend[12 + i]) / frames;
        }
    }
    if (!work.fadeOutFrames) {
        for (int i = 0; i < 4; ++i)
            work.colours[i][2] = work.colours[i][4] = work.colours[i][1];
    } else if (work.fadeOutFrames > 0) {
        work.skyColour = work.cloudColour = 0;
        for (int i = 0; i < 4; ++i) {
            if (i)
                blend[4 + i] += blend[i];
            blend[12 + i] += blend[8 + i];
            const auto sky = static_cast<std::uint32_t>(static_cast<std::int32_t>(blend[4 + i])) << (24 - 8 * i);
            const auto cloud = static_cast<std::uint32_t>(static_cast<std::int32_t>(blend[12 + i])) << (24 - 8 * i);
            work.colours[i][2] = static_cast<std::int32_t>(sky);
            work.colours[i][4] = static_cast<std::int32_t>(cloud);
            work.skyColour |= sky;
            work.cloudColour |= cloud;
        }
    }
    SetCloudAndSkyColour(work.skyColour, work.cloudColour);
}
void OutroVoices() {
    EVT_AT_SetVoice("S2BVA004", 1, 3);
    EVT_AT_SetSound(14, 17);
    EVT_AT_SetVoice("S2BVB030", 1, 18);
    EVT_AT_SetVoice("S2BVC006", 1, 33);
    EVT_AT_SetSound(34, 35);
    EVT_AT_SetSound(16, 39);
    EVT_AT_SetVoice("S2BVA007", 1, 51);
    EVT_AT_SetSound(34, 66);
    EVT_AT_SetVoice("S2BVB014", 1, 66);
    EVT_AT_SetSound(16, 83);
    EVT_AT_SetVoice("S2BVA012", 1, 84);
    EVT_AT_SetSound(38, 90);
    EVT_AT_SetVoice("S2BVA013", 1, 100);
    EVT_AT_SetSound(38, 125);
    EVT_AT_SetVoice("S2BVC011", 1, 130);
}
void Exit(AlleyBattleWork &work) {
    SetPluralControlFor(ctrlGetCharaByName(ryo), 0);
    vfmDispatchControl(FightControlOp::GaugeResources, {}, 0, 0);
    EV_FreeAuthorPak(0, "S2BV");
    SetWeatherUpdateEnabledTo(1);
    colourBlendInitialized = false;
    ET_AT_KillWork(work.characters);
    GetNpcBatchCountOverride(work.npcBatchCount);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("ABLB"), 0);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    EV_scenQueueLoad(2, SHEN_NAME(0055), 6);
}
} // namespace alley_battle
void ABLB_init_func() {
    alley_battle::colourBlendInitialized = false;
}
void ABLB_main_func(std::uintptr_t) {
    using namespace alley_battle;
    SetPlayerControlEnabled(0);
    SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    for (int i = 0; i < 13; ++i)
        EV_SetObjectFlagByIndex(i, 0);
    auto *task = HLib::EnqueueTaskWithParameter(AlleyBattleCallback, 5, 0x0b, sizeof(AlleyBattleWork), AREA_EVENT_TASK);
    auto *work = static_cast<AlleyBattleWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->phase = 0;
    work->field3c = work->field40 = 0;
    work->npcBatchCount = GetNpcBatchCountOverride(35);
    work->characters = Queue_EVT_AT_ReadChr(task, characters.data());
    work->entry = static_cast<std::uint8_t>(g_entryPoint);
    work->reservedA0 = work->delay = 0;
    work->fadeOutFrames = work->fadeInFrames = 0;
    work->elapsed = 0;
    work->field08 = 0;
    work->field10 = -1;
    work->skyColour = work->cloudColour = 0;
    std::memset(work->colourBlend, 0, sizeof(work->colourBlend));
    work->outroFrames = 0;
    std::memset(work->colours, 0, sizeof(work->colours));
    work->colours[0][0] = work->colours[0][1] = 185;
    work->colours[1][0] = work->colours[1][1] = 35;
    work->colours[2][0] = work->colours[2][1] = 20;
    work->fadeOutSeconds = 12;
    work->fadeInSeconds = 1;
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("ABLB"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("ABLB"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("ABLB"), 0);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0138A010");
    FinishLoading();
    SetRenderClockAndCompassTo(1);
    vfmDispatchControl(FightControlOp::GaugeResources, {}, 0, 1);
    EV_SetObjectFlagByIndex(10, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 250, 0);
    Objects(true);
}
void AlleyBattleCallback(void *parameter) {
    using namespace alley_battle;
    auto &work = *static_cast<AlleyBattleWork *>(parameter);
    switch (work.phase) {
    case 0:
        for (auto name : enemies) {
            DisableScenarioCharacterDisplay(name);
            ToggleScenarioCharacterFlag(name);
        }
        EV_LoadAudio("0138A010", 0);
        EV_spmBeginSampleExecute();
        QueueAuthor(1);
        work.phase = 1;
        break;
    case 1:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work.phase = 2;
        break;
    case 2:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            EV_LoadCutsceneFromCurrentScene("0138A020");
            EV_LoadAudio("0138A020", 0);
            EV_spmBeginSampleExecute();
            QueueAuthor(2);
            work.phase = 4;
        }
        break;
    case 4:
        if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
            work.phase = 5;
        break;
    case 5:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            if (EV_authSceneCtrl(AuthSceneCtrl::GetCantSkipAuthor) && !EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 250)) {
                EV_NPC_NewLoadSw(1);
                EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 250, 1);
            }
            work.phase = 7;
        }
        break;
    case 7:
        SetWeatherUpdateEnabledTo(1);
        colourBlendInitialized = false;
        SetupBattle(work);
        break;
    case 8:
        HLib::SetupTaskDelay(work.delay);
        work.phase = 9;
        break;
    case 9:
        BeginFight(work);
        break;
    case 10:
        if (work.elapsed-- == 1) {
            vfmDispatchControl(FightControlOp::SpriteDepth, {}, 0, 1);
            SetScreenFilmFade(
                30 * work.fadeOutSeconds, 0, 0, 0, 0, work.colours[0][0] & 255, work.colours[1][0] & 255, work.colours[2][0] & 255, work.colours[3][0] & 255);
            work.fadeOutFrames = 30 * work.fadeOutSeconds;
            SaveWeatherColours(work);
            work.phase = 11;
        }
        break;
    case 11:
        if (--work.fadeOutFrames < 0) {
            SetScreenFilmFade(
                30 * work.fadeInSeconds, work.colours[0][0] & 255, work.colours[1][0] & 255, work.colours[2][0] & 255, work.colours[3][0] & 255, 255, 0, 0, 0);
            work.phase = 12;
            work.fadeInFrames = 30 * work.fadeInSeconds;
        } else
            BlendWeatherColours(work);
        break;
    case 12:
        if (--work.fadeInFrames < 0) {
            check_dead_status(1);
            EV_FreeAuthorPak(0, "0138");
            EV_stepSetFlags(0, 513);
            work.phase = 13;
        }
        break;
    case 13:
        if (!filmCheckFadeTrackColour()) {
            if (work.entry == 99)
                work.phase = 6;
            else {
                EV_NPC_NewLoadSw(0);
                EV_ResetFreeTalkFlags();
                EV_FreeAuthorPak(1, "S2BV");
                Objects(false);
                work.phase = 14;
            }
        }
        break;
    case 14:
        if (!work.outroFrames)
            OutroVoices();
        if (++work.outroFrames <= 180) {
            if (work.outroFrames == 150)
                spmFadeOut();
        } else
            work.phase = 15;
        break;
    case 15:
        Exit(work);
        break;
    default:
        return;
    }
}
const ScriptAreaDefinition areaABLBScriptDefinition{.areaId = SHEN_NAME(ABLB), .field04 = 0, .init = ABLB_init_func, .main = ABLB_main_func};
} // namespace shenmue::s2
