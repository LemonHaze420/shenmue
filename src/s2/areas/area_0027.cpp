#include "s2/areas/area_0027.hpp"
#include "s2/author.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/extra_telop.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/particle_effects.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include <array>
#include <bit>
namespace shenmue::s2::areas {
namespace {
constexpr shen_name chy{SHEN_NAME(CHY_)}, dummy{SHEN_NAME(DUMY)};
constexpr shen_name characters[]{chy, AREA_AUTHOR_INVALID};
constexpr shen_name props[]{SHEN_NAME(KAWA), SHEN_NAME(AHQB) | 0x1002050aU, SHEN_NAME(a203) | 0x20100U};
constexpr AuthSceneCtrl scenes[]{static_cast<AuthSceneCtrl>(SHEN_NAME(AT01)), static_cast<AuthSceneCtrl>(SHEN_NAME(AT02)),
    static_cast<AuthSceneCtrl>(SHEN_NAME(AT03)), static_cast<AuthSceneCtrl>(SHEN_NAME(AT04)), static_cast<AuthSceneCtrl>(SHEN_NAME(AT05)),
    static_cast<AuthSceneCtrl>(SHEN_NAME(AT06))};
constexpr const char *debugNames[]{"0027A000.c\n", "0027A001.c\n", "0027A005.c\n", "0027A006.c\n", "0027A020.c\n", "0027A030.c\n"};
bool IsGameplayMap() {
    return g_currentMap.name == SHEN_NAME(WNT0) || g_currentMap.name == SHEN_NAME(WN00);
}
void MainCallback(void *);
void Loop0(void *);
void Loop1(void *);
void Loop2(void *);
void Loop4(void *);
void Loop4Resume(void *);
void Loop5(void *);
// @verified - p
void InitNullsub() {
}
void SetCharacters(bool restore) {
    for (shen_name name : {AREA_AUTHOR_RYO, chy}) {
        mtwkSetExtraFlag(scenFindMotionwork(name), 6, restore ? 0 : 2);
        EV_SetCharacterAlphaEnabled(name, restore);
    }
    for (shen_name name : props)
        EV_SetCharacterAlphaEnabled(name, restore);
    if (!restore) {
        SetScenarioCharacterRadius(AREA_AUTHOR_RYO, 10);
        SetScenarioCharacterRadius(chy, 10);
    }
}
// @verified - p
void SetupCharacters() {
    SetCharacters(false);
}
// @verified - p
void RestoreCharacters() {
    SetCharacters(true);
}
// @verified - p
void ConfigureEnvironment() {
    SetPlayerControlEnabled(0);
    if (!IsGameplayMap()) {
        EV_SetWeatherInstruction(12, 0);
        EV_lightLoad(SHEN_NAME(0000));
        PushGlobalLightingProgram();
        EV_FreeAuthorPak(1, "0027");
        const CharacterMirrorPlane plane{{216, 104.5f, 174.649993896484375f}, {1, 0, 0}};
        const Vector3i rotation{};
        SetCharacterMirrorPlane(&plane, &rotation);
        SetMapModelWeatherLightingOverride(0, 1);
    }
}
// @verified - p
void Cleanup(E0027TaskParameter *parent) {
    EV_CleanupMapControlTask();
    if (IsGameplayMap()) {
        *parent->requestCommand = -1;
        HLib::DestroyTask(parent->self);
        SetPluralControlFor(FindCharacterByScenarioName(AREA_AUTHOR_RYO), 0);
        SetPluralControlFor(FindCharacterByScenarioName(chy), 0);
    } else {
        for (const shen_name *name = characters; name->name != AREA_AUTHOR_INVALID.name; ++name)
            if (FindCharacterByScenarioName(*name)) {
                EV_npcLockUnlock(*name);
                EV_npcFree(*name);
            }
        EV_FreeAuthorPak(0, "0027");
        DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
        HLib::DestroyTask(parent->self);
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
    }
}
// @verified - p
void AttachInitialDummy() {
    const Vector3 position{0.13600000739097595f, -0.0020000000949949026f, 0.050000001f};
    const Vector3i rotation{};
    AttachScenarioCharacter(dummy, chy, 14, &position, &rotation);
}
// @verified - p
void InitializeSmoke(E0027LoopParameter *child) {
    child->smoke = static_cast<std::uint32_t>(
        CreateSmokeEffect({221.2100067138672f, 105.54000091552734f, 176.97999572753906f}, {}, 250, 0.07999999821186066f, shen_name{0}, 0));
    SetSmokeEffectFalloff(child->smoke, 9);
    SetSmokeEffectFadeFactor(child->smoke, 191);
    SetParticleEffectColour(0, child->smoke, 0, 139, 153, 150);
    SetParticleEffectInitialScale(0, child->smoke, 0);
    SetParticleEffectSize(0, child->smoke, 0.03921568766236305f, 0.0784313753247261f);
    SetParticleEffectBirthRate(0, child->smoke, 0.024313727393746376f);
    SetParticleEffectVelocity(0, child->smoke, {0, 0.0031372548546642065f, 0}, {});
    ResumeParticleEffect(0, child->smoke);
}
// @verified - p
void QueueLoop(E0027TaskParameter *parent) {
    constexpr HLib::TaskCallback callbacks[]{Loop0, Loop1, Loop2, Loop2, Loop4, Loop5};
    HLib::Task *task = HLib::EnqueueTaskWithParameter(callbacks[parent->selection], 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    auto *child = static_cast<E0027LoopParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->frame = 0;
    SetupCharacters();
    if (IsGameplayMap()) {
        if (parent->selection >= 0 && parent->selection <= 3) {
            if (parent->selection <= 1)
                InitializeSmoke(child);
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        }
        EV_authSceneCtrl(scenes[parent->selection]);
        parent->fadeFrame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining) - 30);
    } else {
        SetScreenFilmFade(static_cast<std::uint32_t>(parent->selection - 4) <= 1 ? 0 : 30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_LoadAudio("0027A000", 0);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    }
    parent->state = 2;
}
void Plural(shen_name name, bool enabled) {
    SetPluralControlFor(FindCharacterByScenarioName(name), enabled ? 1 : 0);
}
void AttachDummyAt(const Vector3 &position) {
    ResetScenarioCharacterAttachment(dummy);
    const Vector3i rotation{};
    AttachScenarioCharacter(dummy, chy, 14, &position, &rotation);
}
void UpdateSmokePosition(E0027LoopParameter *child) {
    Vector3 position;
    EV_GetCharaPosition(dummy, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    const Vector3 volume{};
    SetParticleEffectPosition(0, child->smoke, &position, &volume);
}
void CompleteLoop(E0027LoopParameter *child) {
    child->parent->state = IsGameplayMap() ? 3 : 0;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}
// @verified - p
void Loop0(void *data) {
    auto *child = static_cast<E0027LoopParameter *>(data);
    Plural(AREA_AUTHOR_RYO, true);
    Plural(chy, true);
    child->frame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (!child->frame)
        AttachInitialDummy();
    else if (child->frame == 240)
        SetParticleEffectBirthRate(0, child->smoke, 0.07725490629673004f);
    switch (child->frame) {
    case 490:
        AttachDummyAt({-0.067000002f, 0.23999999f, 0.33500001f});
        break;
    case 660:
        AttachDummyAt({-0.168f, 0.146f, -0.059999999f});
        break;
    case 990:
        AttachDummyAt({0.13600001f, -0.0020000001f, 0.050000001f});
        break;
    default:
        if (child->frame == child->parent->fadeFrame)
            ProcessAicaCommand(0xaa0, 30);
        break;
    }
    UpdateSmokePosition(child);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        ResetScenarioCharacterAttachment(dummy);
        FreeParticleEffect(0, child->smoke);
        RestoreCharacters();
        CompleteLoop(child);
    }
}
// @verified - p
void Loop1(void *data) {
    auto *child = static_cast<E0027LoopParameter *>(data);
    Plural(chy, true);
    child->frame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    Plural(AREA_AUTHOR_RYO, static_cast<std::uint32_t>(child->frame - 200) > 95);
    if (!child->frame) {
        SetParticleEffectBirthRate(0, child->smoke, 0.0815686285495758f);
        AttachInitialDummy();
    }
    if (child->frame == 210)
        AttachDummyAt({-0.001f, 0.199f, 0.33500001f});
    else if (child->frame == 590)
        AttachDummyAt({0.13600001f, -0.0020000001f, 0.050000001f});
    else if (child->frame == child->parent->fadeFrame)
        ProcessAicaCommand(0xaa0, 30);
    UpdateSmokePosition(child);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        ResetScenarioCharacterAttachment(dummy);
        RestoreCharacters();
        FreeParticleEffect(0, child->smoke);
        CompleteLoop(child);
    }
}
// @verified - p
void Loop2(void *data) {
    auto *child = static_cast<E0027LoopParameter *>(data);
    child->frame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    const bool early = child->frame < 784;
    Plural(AREA_AUTHOR_RYO, early);
    Plural(chy, early);
    EV_SetIndexForLightEntry(6, early ? 0 : 1);
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    if (IsGameplayMap()) {
        Plural(AREA_AUTHOR_RYO, false);
        Plural(chy, false);
        child->parent->state = 3;
        HLib::DestroyCurrentTask();
    } else {
        RestoreCharacters();
        CompleteLoop(child);
    }
}
// @verified - p
void Loop4(void *data) {
    auto *child = static_cast<E0027LoopParameter *>(data);
    child->frame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (child->frame == 359) {
        EV_authSceneCtrl(AuthSceneCtrl::SetAuthorStopped);
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetCurrentTaskTo(Loop4Resume);
        HLib::SetupTaskDelay(30);
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        RestoreCharacters();
        CompleteLoop(child);
    }
}
// @verified - p
void Loop4Resume(void *) {
    EV_authSceneCtrl(AuthSceneCtrl::ResetAuthorStopped);
    HLib::SetCurrentTaskTo(Loop4);
}
// @verified - p
void Loop5(void *data) {
    auto *child = static_cast<E0027LoopParameter *>(data);
    child->frame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    Plural(chy, static_cast<std::uint32_t>(child->frame - 847) <= 243);
    Plural(AREA_AUTHOR_RYO, child->frame >= 847);
    if (!child->frame)
        QueueExtraTelop("\xe8\x83\x86", 99);
    else if (child->frame == 1235) {
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        ProcessAicaCommand(0xaa0, 30);
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        QueueExtraTelop("MM", 1);
        RestoreCharacters();
        CompleteLoop(child);
    }
}
// @verified - p
void MainCallback(void *data) {
    auto *parent = static_cast<E0027TaskParameter *>(data);
    if (parent->state == 1)
        QueueLoop(parent);
    else if (parent->state == 3)
        Cleanup(parent);
    else if (!parent->state) {
        const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
        if (input == 1024)
            parent->state = 1;
        else if (input == 64) {
            if (--parent->selection < 0)
                parent->selection = 5;
        } else if (input == 128 && ++parent->selection > 5)
            parent->selection = 0;
        if (parent->selection >= 0 && parent->selection <= 5) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(debugNames[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", parent->selection);
    }
}
// @verified - p
void Main(std::uintptr_t argument) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(MainCallback, 4, 0x0b, 0x20, AREA_AUTHOR_TASK);
    auto *parent = static_cast<E0027TaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    if (IsGameplayMap()) {
        parent->selection = *reinterpret_cast<const std::int16_t *>(argument);
        parent->state = 1;
        parent->requestCommand = reinterpret_cast<std::int32_t *>(argument);
    } else {
        parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, "0027A.MOT"));
        parent->selection = 0;
        parent->state = 0;
        Queue_EVT_UTL_LoadNPCChara_3(characters, parent->self);
    }
    ConfigureEnvironment();
    if (parent->selection > 5)
        Cleanup(parent);
}
} // namespace
const ScriptAreaDefinition area0027ScriptDefinition{.areaId = SHEN_NAME(0027), .field04 = 0, .init = InitNullsub, .main = Main};
} // namespace shenmue::s2::areas
