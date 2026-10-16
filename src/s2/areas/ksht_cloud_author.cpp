#include "s2/areas/ksht_authored.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/model_component.hpp"
#include "s2/npc.hpp"
#include "s2/particle_effects.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/weather.hpp"
namespace shenmue::s2::areas {
extern std::uint64_t kshtAuthorMapLighting;
void SelectKshtCloudPackage(std::int32_t);
void PrepareKshtCloudCharacters(std::int32_t);
void RestoreKshtCloudCharacters(std::int32_t);
void CreateKshtLeaves(bool);
void CreateKshtBreath();
namespace {
void CloudAuthorCallback(void *data) {
    KshtAuthorLoop *loop = static_cast<KshtAuthorLoop *>(data);
    KshtAuthorController *parent = loop->parent;
    const std::int32_t frame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (parent->selection == 0) {
        if (!frame)
            EV_SetObjectFlagByIndex(4, 0);
        SetPluralControlFor(FindCharacterByScenarioName(KSHT_SIN), frame <= 480);
        SetPluralControlFor(FindCharacterByScenarioName(KSHT_RYO), frame <= 480);
    } else if (parent->selection == 5) {
        SetPluralControlFor(FindCharacterByScenarioName(KSHT_RYO), 1);
        SetPluralControlFor(FindCharacterByScenarioName(KSHT_SIN), frame > 244);
    } else if (parent->selection == 6) {
        SetPluralControlFor(FindCharacterByScenarioName(KSHT_SIN), 1);
        SetPluralControlFor(FindCharacterByScenarioName(KSHT_RYO), 1);
    }
    if (parent->selection == 7) {
        const Vector3 velocity{0, 0.003f, 0};
        Vector3 position;
        EV_GetCharaPosition(SHEN_NAME(a904), CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        SetParticleEffectPosition(0, kshtBreathEffects[0], &position, &velocity);
        EV_GetCharaPosition(SHEN_NAME(a906), CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        SetParticleEffectPosition(0, kshtBreathEffects[1], &position, &velocity);
        if (frame == 0 || frame == 681 || frame == 1111) {
            ResumeParticleEffect(0, kshtBreathEffects[0]);
            ResumeParticleEffect(0, kshtBreathEffects[1]);
        } else if (frame == 311 || frame == 801 || frame == 1211) {
            SuspendParticleEffect(0, kshtBreathEffects[0]);
            SuspendParticleEffect(0, kshtBreathEffects[1]);
        } else if (frame == 500) {
            camCtrlSetNearDistance(0.01f);
            camCtrlSetFarDistance(2200);
        } else if (frame == 1250)
            camCtrlResetClipDistances();
    }
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    camCtrlResetClipDistances();
    if (kshtLeafEffect != 0xffffffffU)
        FreeParticleEffect(3, kshtLeafEffect);
    if (kshtBreathEffects[0] != 0xffffffffU) {
        FreeParticleEffect(0, kshtBreathEffects[0]);
        kshtBreathEffects[0] = 0xffffffffU;
    }
    if (kshtBreathEffects[1] != 0xffffffffU) {
        FreeParticleEffect(0, kshtBreathEffects[1]);
        kshtBreathEffects[1] = 0xffffffffU;
    }
    RestoreKshtCloudCharacters(parent->selection);
    switch (parent->selection) {
    case 0:
    case 1:
    case 3:
    case 4:
        EV_SetCharaControllerFlags(KSHT_RYO, 2, NpcCharacterControllerState::Enabled);
        SetAllMapFieldDepthSlotsEnabled(1);
        SetScreenFilmFade(0, 0, 255, 255, 255, 255, 255, 255, 255);
        parent->state = 1;
        ++parent->selection;
        break;
    case 2:
        EV_SetCharaControllerFlags(KSHT_RYO, 2, NpcCharacterControllerState::Enabled);
        EV_stepSetFlags(0, 243);
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        parent->state = 1;
        ++parent->selection;
        break;
    case 6:
        EV_SetCharaControllerFlags(KSHT_RYO, 2, NpcCharacterControllerState::Enabled);
        [[fallthrough]];
    case 5:
        EV_SetObjectFlagByIndex(1, 1);
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        parent->state = 4;
        break;
    case 7:
        SetScreenFilmFade(0, 0, 255, 255, 255, 255, 255, 255, 255);
        parent->state = 4;
        break;
    default:
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        parent->state = 1;
        ++parent->selection;
        break;
    }
    HLib::DestroyCurrentTask();
}
} // namespace
void QueueKshtCloudAuthor(KshtAuthorController *parent) {
    kshtBreathEffects[0] = 0xffffffffU;
    kshtBreathEffects[1] = 0xffffffffU;
    kshtLeafEffect = 0xffffffffU;
    if (parent->state == 1)
        SelectKshtCloudPackage(parent->selection);
    if (parent->state != 2) {
        parent->state = 2;
        return;
    }
    HLib::Task *task = HLib::EnqueueTaskWithParameter(CloudAuthorCallback, 4, 0x0b, sizeof(KshtAuthorLoop), SHEN_NAME(051B));
    KshtAuthorLoop *loop = static_cast<KshtAuthorLoop *>(HLib::GetTaskParameter(task));
    loop->frame = 0;
    loop->parent = parent;
    EventSetPosition position{.cleanup = 0, .character = KSHT_RYO, .parent = KSHT_INVALID, .position = {0, 0, 0}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    if (parent->selection != 7)
        CreateKshtLeaves(parent->selection == 1);
    switch (parent->selection) {
    case 0:
        EV_LoadCutsceneFromCurrentScene("0510A000");
        EV_LoadAudio("0510A000", 0);
        break;
    case 1:
        EV_SetCharaControllerFlags(KSHT_RYO, 2, NpcCharacterControllerState::Disabled);
        EV_LoadCutsceneFromCurrentScene("0511A000");
        break;
    case 2:
        EV_LoadCutsceneFromCurrentScene("0510A000");
        EV_LoadAudio("0510A000", 1);
        break;
    case 3:
        EV_SetCharaControllerFlags(KSHT_RYO, 2, NpcCharacterControllerState::Disabled);
        EV_LoadCutsceneFromCurrentScene("0511A000");
        EV_LoadAudio("0511A000", 0);
        break;
    case 4:
        EV_SetCharaControllerFlags(KSHT_RYO, 2, NpcCharacterControllerState::Disabled);
        EV_LoadCutsceneFromCurrentScene("0511A000");
        EV_LoadAudio("0511A000", 2);
        break;
    case 5:
        clmdUtil(FindCharacterByScenarioName(KSHT_SIN), 14);
        EV_LoadCutsceneFromCurrentScene("0511A000");
        EV_LoadAudio("0511A000", 3);
        break;
    case 6:
        EV_LoadCutsceneFromCurrentScene("0511A000");
        LoadSoundProgramWithoutSelectedRecord("0511A000", 3);
        break;
    case 7:
        EV_LoadCutsceneFromCurrentScene("0509A000");
        break;
    default:
        break;
    }
    switch (parent->selection) {
    case 0:
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_SetWeatherInstruction(12, 0);
        EV_SetModelDataDefaultVertexShader();
        EV_lightLoad(SHEN_NAME(l102));
        break;
    case 2:
        EV_SetWeatherInstruction(12, 0);
        EV_SetModelDataDefaultVertexShader();
        EV_lightLoad(SHEN_NAME(l101));
        PushGlobalLightingProgram();
        SetAllMapFieldDepthSlotsEnabled(0);
        break;
    case 3:
    case 6:
        kshtAuthorMapLighting = 0x100000001ULL;
        ExecuteCurrAreaMainFunc_Wrapper(1, reinterpret_cast<std::uintptr_t>(&kshtAuthorMapLighting));
        EV_SetObjectFlagByIndex(1, 1);
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        break;
    case 5:
        EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
        kshtAuthorMapLighting = 0x100000001ULL;
        ExecuteCurrAreaMainFunc_Wrapper(1, reinterpret_cast<std::uintptr_t>(&kshtAuthorMapLighting));
        break;
    case 7:
        EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
        CreateKshtBreath();
        EV_SetWeatherInstruction(12, 0);
        EV_SetModelDataDefaultVertexShader();
        EV_lightLoad(SHEN_NAME(l105));
        PushGlobalLightingProgram();
        break;
    default:
        EV_SetModelDataDefaultVertexShader();
        SetWeatherUpdateEnabledTo(1);
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        break;
    }
    PrepareKshtCloudCharacters(parent->selection);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(kshtAuthorIds[parent->selection].name));
    parent->state = 3;
}
void SetupKshtCloudAuthor() {
    EV_SetNewSceneIndex();
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0509");
    EV_FreeAuthorPak(1, "0510");
    EV_FreeAuthorPak(1, "0511");
}
} // namespace shenmue::s2::areas
