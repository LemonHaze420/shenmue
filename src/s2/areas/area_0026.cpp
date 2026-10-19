#include "s2/areas/area_0026.hpp"
#include "s2/extra_telop.hpp"
#include "s2/author.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/input.hpp"
#include "s2/event_runtime.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/motion.hpp"
#include "s2/npc_package.hpp"
#include "s2/particle_effects.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include <bit>
namespace shenmue::s2::areas {
namespace {
constexpr shen_name kem{SHEN_NAME(KEM_)};
constexpr shen_name characters[]{kem, AREA_AUTHOR_INVALID};
constexpr const char *sceneNames[]{
    "0026A000.c\n", "0026A002.c\n", "0026A022.c\n", "0026A028.c\n", "0026A024.c\n", "0026A026.c\n", "0026A003.c\n", "0026A004.c\n"};
void MainCallback(void *);
void Loop0(void *);
void Loop1(void *);
void Loop3(void *);
void Loop4(void *);
void Loop5(void *);
constexpr HLib::TaskCallback loops[]{Loop0, Loop1, Loop1, Loop3, Loop4, Loop5, Loop0, Loop0};
// @verified - p
void InitNullsub() {
}
void SetupCharacters() {
    for (shen_name name : {AREA_AUTHOR_RYO, kem}) {
        EV_EnableCharacterDisplay(name);
        EV_SetCharaControllerFlags(name, UpdateHeight, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(name, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharacterAlphaEnabled(name, false);
    }
    LoadBlurModelForCharacter(AREA_AUTHOR_RYO, "RYO_B.MT7", 1);
    LoadBlurModelForCharacter(kem, "KEM_B.MT7", 1);
}
void RestoreCharacters() {
    for (shen_name name : {AREA_AUTHOR_RYO, kem}) {
        EV_EnableCharacterDisplay(name);
        EV_SetCharaControllerFlags(name, UpdateHeight, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(name, ApplyGravity, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(name, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
        EV_SetCharacterAlphaEnabled(name, true);
    }
}
// @verified - p
void ConfigureEnvironment() {
    SetPlayerControlEnabled(0);
    DateTime time{};
    GetCurrentDateTime(time);
    if (time.hour < 19) {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
    } else {
        EV_lightLoad(SHEN_NAME(0000));
        PushGlobalLightingProgram();
        CheckIsArgEquZero(1);
    }
    EV_FreeAuthorPak(1, "0026");
}
// @verified - p
void InitializeSmoke(E0026LoopParameter *child) {
    if (child->smokeProfile == 1) {
        child->smoke = static_cast<std::uint32_t>(
            CreateSmokeEffect({40.689998626708984f, 102.19999694824219f, 241.27000427246094f}, {}, 100, 0.0010000000474974513f, shen_name{0}, 3));
        SetSmokeEffectFalloff(child->smoke, 10);
        SetSmokeEffectFadeFactor(child->smoke, 255);
        SetParticleEffectColour(0, child->smoke, 0, 142, 125, 110);
        SetParticleEffectInitialScale(0, child->smoke, 0.43529412150382996f);
        SetParticleEffectSize(0, child->smoke, 3.1372549533843994f, 3.1764705181121826f);
        SetParticleEffectBirthRate(0, child->smoke, 0);
        SetParticleEffectVelocity(0, child->smoke, {0, 0.009999999776482582f, 0}, {});
        SetSmokeEffectSpread(child->smoke, 56.36399841308594f, 0.7941176295280457f);
        SetSmokeEffectRadialMotion(child->smoke, 0.772549033164978f, 0.46666666865348816f);
        SetParticleEffectBirthRate(0, child->smoke, 0.10000000149011612f);
    } else {
        child->smoke = static_cast<std::uint32_t>(
            CreateSmokeEffect({42.84000015258789f, 102.26000213623047f, 250.5500030517578f}, {}, 35, 0.07999999821186066f, shen_name{0}, 0));
        SetSmokeEffectFadeFactor(child->smoke, 138);
        SetSmokeEffectFalloff(child->smoke, 10);
        SetParticleEffectColour(0, child->smoke, 0, 140, 122, 103);
        SetParticleEffectInitialScale(0, child->smoke, 0.5f);
        SetParticleEffectSize(0, child->smoke, 2.0392158031463623f, 1.9215686321258545f);
        SetParticleEffectBirthRate(0, child->smoke, 0.012549019418656826f);
        SetParticleEffectVelocity(0, child->smoke, {0.03176470845937729f, 0, -0.013333333656191826f}, {});
    }
}
// @verified - p
void InitializeLeaves(std::uint32_t *effect, std::uint32_t count, float rate) {
    *effect = static_cast<std::uint32_t>(CreateFallingParticleEffect({39.310001373291016f, 105.80000305175781f, 240.89999389648438f},
        {1.5f, 0.019999999552965164f, 2}, static_cast<std::int32_t>(count), rate, shen_name{0}, "misc", "REDLEAF.pvr", -1));
    SetParticleEffectVelocity(
        3, *effect, {0.02199999988079071f, 0.05000000074505806f, 0.02199999988079071f}, {0.012000000104308128f, 0.009999999776482582f, 0.012000000104308128f});
    SetParticleEffectSize(3, *effect, 0.05000000074505806f, 0.15000000596046448f);
}
// @verified - p
void InitializeLeafVolume(std::uint32_t *effect, std::uint32_t count) {
    *effect = static_cast<std::uint32_t>(CreateFallingParticleEffect({39.310001373291016f, 105.80000305175781f, 240.89999389648438f},
        {2, 0.019999999552965164f, 2}, static_cast<std::int32_t>(count), 0.10000000149011612f, shen_name{0}, "misc", "REDLEAF.pvr", 101.4000015258789f));
    SetParticleEffectVelocity(
        3, *effect, {0.02199999988079071f, 0.019999999552965164f, 0.02199999988079071f}, {0.012000000104308128f, 0.009999999776482582f, 0.012000000104308128f});
    SetParticleEffectSize(3, *effect, 0.05000000074505806f, 0.15000000596046448f);
}
// @verified - p
void Loop0(void *data) {
    auto *child = static_cast<E0026LoopParameter *>(data);
    child->currentFrame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (!child->currentFrame) {
        InitializeLeafVolume(&child->secondaryLeaf, 80);
        ResumeParticleEffect(3, child->secondaryLeaf);
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        child->parent->state = 0;
        FreeParticleEffect(3, child->secondaryLeaf);
        RestoreCharacters();
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::DestroyCurrentTask();
    }
}
// @verified - p
void Loop1(void *data) {
    auto *child = static_cast<E0026LoopParameter *>(data);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        child->parent->state = 0;
        RestoreCharacters();
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::DestroyCurrentTask();
    }
}
// @verified - p
void Loop3(void *data) {
    auto *child = static_cast<E0026LoopParameter *>(data);
    child->currentFrame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (!child->currentFrame)
        InitializeLeafVolume(&child->secondaryLeaf, 50);
    else if (child->currentFrame == 850)
        ResumeParticleEffect(3, child->secondaryLeaf);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        child->parent->state = 0;
        FreeParticleEffect(3, child->secondaryLeaf);
        RestoreCharacters();
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::DestroyCurrentTask();
    }
}
// @verified - p
void Loop4(void *data) {
    auto *child = static_cast<E0026LoopParameter *>(data);
    child->currentFrame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    switch (child->currentFrame) {
    case 0:
        InitializeLeaves(&child->leaf, 200, 0.5f);
        break;
    case 30:
        ResumeParticleEffect(3, child->leaf);
        SetParticleEffectVelocity(3, child->leaf, {0.02199999988079071f, 0.03999999910593033f, 0.02199999988079071f},
            {0.012000000104308128f, 0.009999999776482582f, 0.012000000104308128f});
        break;
    case 200:
        SetParticleEffectBirthRate(3, child->leaf, 0);
        break;
    case 420:
        SuspendParticleEffect(3, child->leaf);
        break;
    default:
        break;
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        child->parent->state = 0;
        FreeParticleEffect(3, child->leaf);
        RestoreCharacters();
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::DestroyCurrentTask();
    }
}
// @verified - p
void Loop5(void *data) {
    auto *child = static_cast<E0026LoopParameter *>(data);
    child->currentFrame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    switch (child->currentFrame) {
    case 0:
        child->smokeProfile = 1;
        InitializeSmoke(child);
        InitializeLeaves(&child->leaf, 600, 0.004999999888241291f);
        InitializeLeaves(&child->secondaryLeaf, 6000, 0.004999999888241291f);
        QueueExtraTelop("\xe5\x8a\x9f", 99);
        break;
    case 90:
        ResumeParticleEffect(0, child->smoke);
        break;
    case 119:
        SetParticleEffectBirthRate(0, child->smoke, 0);
        break;
    case 120:
        SuspendParticleEffect(0, child->smoke);
        break;
    case 145:
        ResumeParticleEffect(3, child->leaf);
        SetParticleEffectVelocity(3, child->leaf, {0.02199999988079071f, 0.0010000000474974513f, 0.02199999988079071f},
            {0.012000000104308128f, 0.009999999776482582f, 0.012000000104308128f});
        break;
    case 440:
        SuspendParticleEffect(3, child->leaf);
        break;
    case 1270:
        ResumeParticleEffect(3, child->secondaryLeaf);
        break;
    case 1630:
        ProcessAicaCommand(0xaa0, 5);
        break;
    default:
        break;
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        child->parent->state = 0;
        QueueExtraTelop("LL", 1);
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        FreeParticleEffect(0, child->smoke);
        FreeParticleEffect(3, child->leaf);
        FreeParticleEffect(3, child->secondaryLeaf);
        RestoreCharacters();
        HLib::DestroyCurrentTask();
    }
}
// @verified - p
void QueueAuthor(AreaAuthorTaskParameter *parent) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(loops[parent->selection], 4, 0x0b, 0x20, AREA_AUTHOR_TASK);
    auto *child = static_cast<E0026LoopParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->currentFrame = 0;
    child->smokeProfile = 0;
    child->smoke = 0;
    child->leaf = 0;
    child->secondaryLeaf = 0;
    child->reserved18 = 0;
    EV_LoadCutsceneFromCurrentScene(parent->selection == 3 ? "0026A028" : "0026A000");
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetupCharacters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}
// @verified - p
void Cleanup(AreaAuthorTaskParameter *parent) {
    for (const shen_name *name = characters; name->name != AREA_AUTHOR_INVALID.name; ++name)
        if (FindCharacterByScenarioName(*name)) {
            EV_npcLockUnlock(*name);
            EV_npcFree(*name);
        }
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_FreeAuthorPak(0, "0026");
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}
// @verified - p
void MainCallback(void *data) {
    auto *parent = static_cast<AreaAuthorTaskParameter *>(data);
    if (parent->state) {
        if (parent->state == 1)
            QueueAuthor(parent);
        else if (parent->state == 3)
            Cleanup(parent);
        return;
    }
    const auto input = EVT_GetControllerInput(0, 1);
    if (input == 1024)
        parent->state = 1;
    else if (input == 64) {
        if (--parent->selection < 0)
            parent->selection = 7;
    } else if (input == 128) {
        if (++parent->selection > 7)
            parent->selection = 0;
    }
    if (parent->selection >= 0 && parent->selection <= 7) {
        EV_set_debug_text_pos(2, 4);
        EV_debug_log_to_screen("%s", sceneNames[parent->selection]);
    }
    EV_set_debug_text_pos(2, 5);
    EV_debug_log_to_screen("seqdata%x.avb\n", parent->selection);
}
// @verified - p
void Main(std::uintptr_t) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    auto *parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, "0026A.MOT"));
    parent->self = task;
    parent->selection = 0;
    parent->state = 0;
    ConfigureEnvironment();
    Queue_EVT_UTL_LoadNPCChara_3(characters, parent->self);
}
} // namespace
const ScriptAreaDefinition area0026ScriptDefinition{.areaId = SHEN_NAME(0026), .field04 = 0, .init = InitNullsub, .main = Main};
} // namespace shenmue::s2::areas
