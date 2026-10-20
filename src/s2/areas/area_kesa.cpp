#include "s2/areas/area_kesa.hpp"
#include "s2/areas/kesa_tables.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/lighting.hpp"
#include "s2/load_title.hpp"
#include "s2/map_assets.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/particle_effects.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include <bit>
#include <cstring>
namespace shenmue::s2::areas {
std::int32_t kesaAuthorSelection, kesaTorchEffect;
std::array<std::int32_t, 6> kesaFireEffects;
const std::array<KesaAuthorEntry, 8> kesaAuthors{{{SHEN_NAME(5140), SHEN_NAME(5140), 1, 2, 0, 0, "0514A000", "0514", "AUTHOR1"},
    {SHEN_NAME(5141), SHEN_NAME(5141), 1, 2, 1, 0, "0514A000", "0514", "AUTHOR1"},
    {SHEN_NAME(5142), SHEN_NAME(5142), 1, 2, 2, 0, "0514A000", "0514", "AUTHOR2"},
    {SHEN_NAME(5380), SHEN_NAME(5380), 1, 2, -1, 0, "0538A000", "0538", "AUTHOR2"},
    {SHEN_NAME(5160), SHEN_NAME(5160), 1, 2, 0, 0, "0516A000", "0516", "AUTHOR3"},
    {SHEN_NAME(5162), SHEN_NAME(5162), 0xffffffffU, 2, -1, 0, "0516A001", "0516", "AUTHOR4"},
    {SHEN_NAME(5163), SHEN_NAME(5163), 1, 2, 1, 0, "0516A001", "0516", "AUTHOR5"},
    {SHEN_NAME(5161), SHEN_NAME(5161), 1, 2, 1, 0, "0516A010", "0516", "AUTHOR6"}}};
namespace {
constexpr shen_name authorCharacters[]{KESA_SHENHUA, KESA_INVALID};
constexpr shen_name alphaCharacters[]{SHEN_NAME(aa85), KESA_TORCH, KESA_COMPANION_TORCH, SHEN_NAME(aa33), SHEN_NAME(a704)};
constexpr std::array<HLib::TaskCallback, 8> authorCallbacks{KesaOpeningTorchLoop, KesaOpeningWalkLoop, KesaTransitionWalkLoop, KesaCaveWalkLoop,
    KesaSwordDiscoveryLoop, KesaSwordPreparationLoop, KesaSwordActivationLoop, KesaEndingLoop};
const std::uint32_t *postEffects;
std::uint32_t postEffectCount;
void LoadCharacterPackage(const char *package) {
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>(package));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
}
void SetAuthorCharacters() {
    for (shen_name name : {KESA_RYO, KESA_SHENHUA}) {
        EV_EnableCharacterDisplay(name);
        EV_SetCharaControllerFlags(name, 1, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Disabled);
        mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
        EV_SetCharacterAlphaEnabled(name, false);
        SetScenarioCharacterRadius(name, 10);
    }
    for (shen_name name : alphaCharacters) {
        EV_SetCharaControllerFlags(name, 1, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Disabled);
        EV_SetCharacterAlphaEnabled(name, false);
    }
    EV_EnableCharacterDisplay(KESA_TORCH);
    EV_EnableCharacterDisplay(KESA_COMPANION_TORCH);
}
void SelectPostEffects() {
    const std::uint32_t *const tables[]{kesaAuthorPostFx0.data(), kesaAuthorPostFx1.data(), kesaAuthorPostFx2.data(), kesaAuthorPostFx3.data(),
        kesaAuthorPostFx4.data(), kesaAuthorPostFx5.data(), kesaAuthorPostFx6.data(), kesaAuthorPostFx7.data()};
    postEffects = tables[kesaAuthorSelection];
    postEffectCount = kesaAuthorSelection == 4 ? 2 : 1;
}
void EndingExit(void *) {
    HLib::DestroyCurrentTask();
    EV_scenQueueLoad(-1, shen_name{0}, 0);
}
void EndingFade(void *) {
    ST_LoadTitleFade();
    spmFadeOut();
    HLib::SetCurrentTaskTo(EndingExit);
    HLib::SetupTaskDelay(30);
}
} // namespace
void AreaKESAInit() {
    kesaAuthorSelection = 0;
    kesaTorchEffect = 0;
    kesaFireEffects = {};
    kesaEnergyParticles = {};
    kesaEnergyFrame = 0;
    kesaEnergyDuration = 0;
    kesaEnergyActiveParticles = 0;
    kesaEnergyLifetime = 0;
    KesaResetTorchLightState();
    KesaResetSparkState();
    KesaResetSwordEffects();
    KesaResetAuthoredState();
}
void AreaKESAMain(std::uintptr_t argument) {
    const auto selector = static_cast<std::intptr_t>(argument);
    if (selector == -2) {
        kesaTorchEffect = -1;
        KesaCreateTorchEffect();
        return;
    }
    if (selector == -1) {
        KesaSetupTorch();
        KesaCreateTorchEffect();
        HLib::EnqueueTaskWithoutParameter(KesaSetLight, 6, 0x0b, static_cast<std::int32_t>(SHEN_NAME(lght)));
        KesaSetupLighting();
        return;
    }
    kesaAuthorSelection = static_cast<std::int32_t>(argument);
    const auto &author = kesaAuthors[kesaAuthorSelection];
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1757, 0);
    SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    EV_NPC_NewLoadSw(0);
    EV_npcLockUnlock(KESA_SHENHUA);
    EV_ResetFreeTalkFlags();
    SetPlayerControlEnabled(0);
    LoadCharacterPackage(author.characterPackage);
    SetMotionRegionFlag(scenFindMotionwork(KESA_RYO), 73, 0, MotionRegion::LeftArm);
    EV_LoadCutsceneFromCurrentScene(author.cutscene);
    EV_FreeAuthorPak(1, author.authorPackage);
    kesaFireEffects.fill(-1);
    ResetScenarioCharacterAttachment(KESA_TORCH);
    EV_SetIndexForLightEntry(13, 0);
    EV_SetIndexForLightEntry(14, 0);
    SetNpcRouteControlHold(KESA_SHENHUA, 0);
    ResetScenarioCharacterAttachment(SHEN_NAME(aa75));
    AttachFireEffect(kesaTorchEffect, KESA_TORCH, 152, .1f);
    AttachFireInstance(kesaTorchEffect, 9, KESA_COMPANION_TORCH, 152, std::bit_cast<std::int32_t>(.1f));
    auto *const task = HLib::EnqueueTaskWithoutParameter(KesaInitializeAuthor, 4, 0x0b, static_cast<std::int32_t>(SHEN_NAME(auth)));
    camCtrlCacheTransform();
    AssignAreaNpcLoadContext(authorCharacters, task);
    g_usingCustomAuthorCharacterList = 0;
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    if (task)
        HLib::SetupTimedTask(task, 0);
    HLib::EnqueueTaskWithoutParameter(EVT_UTL_LoadNPCChara_Callback_1, 6, 0x0b, static_cast<std::int32_t>(SHEN_NAME(load)));
}
void KesaParseLights(shen_name light) {
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(light);
    constexpr std::uint8_t early[]{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0};
    constexpr std::uint8_t cave[]{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0};
    constexpr std::uint8_t discovery[]{0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1};
    constexpr std::uint8_t ending[]{0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0};
    const std::uint8_t *flags = nullptr;
    if (kesaAuthorSelection >= 0 && kesaAuthorSelection <= 2)
        flags = early;
    else if (kesaAuthorSelection == 3)
        flags = cave;
    else if (kesaAuthorSelection == 4)
        flags = discovery;
    else if (kesaAuthorSelection == 7)
        flags = ending;
    for (std::int32_t index = 0; index < 17; ++index)
        EV_SetObjectFlagByIndex(index, flags ? flags[index] : 1);
    if (kesaAuthorSelection == 3)
        camCtrlSetNearDistance(0);
    if (kesaAuthorSelection >= 3 && kesaAuthorSelection != 6)
        PushGlobalLightingProgram();
    if (kesaAuthorSelection == 7)
        SetDefaultVSByIndex(6);
}
void KesaInitializeAuthor(void *) {
    SetAuthorCharacters();
    SetScenarioCharacterDrawControl(shen_name{0}, 0, 0);
    const auto &author = kesaAuthors[kesaAuthorSelection];
    KesaParseLights(author.lighting);
    SelectPostEffects();
    if (kesaAuthorSelection > 4)
        EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(author.scene.name));
    if (author.audioScene != -1) {
        EV_LoadAudio(author.cutscene, author.audioScene);
        EV_spmBeginSampleExecute();
    }
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    HLib::SetCurrentTaskTo(authorCallbacks[kesaAuthorSelection]);
}
void KesaUpdateAuthorPostEffects(std::int32_t frame) {
    std::uint32_t index = postEffectCount - 1;
    if (static_cast<float>(frame) < std::bit_cast<float>(postEffects[index * 64])) {
        index = 0;
        while (index < postEffectCount && std::bit_cast<float>(postEffects[index * 64]) <= static_cast<float>(frame))
            ++index;
        --index;
    }
    const auto *entry = postEffects + index * 64;
    if (entry[1] == 1) {
        GlowParameters parameters;
        std::memcpy(&parameters, entry + 2, sizeof(parameters));
        UploadGlowParams(parameters);
    } else if (entry[1] == 2) {
        CrossParameters parameters;
        std::memcpy(&parameters, entry + 2, sizeof(parameters));
        UploadCrossParams(parameters);
    }
}
void KesaRestoreCharacter(shen_name character) {
    EV_EnableCharacterDisplay(character);
    EV_SetCharaControllerFlags(character, 1, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(character, 2, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(character, 3, NpcCharacterControllerState::Enabled);
    mtwkSetExtraFlag(scenFindMotionwork(character), 6, 0);
    EV_SetCharacterAlphaEnabled(character, true);
    SetScenarioCharacterRadius(character, 1.8f);
}
void KesaFinishAuthor() {
    using enum CameraState;
    const auto &author = kesaAuthors[kesaAuthorSelection];
    if (kesaAuthorSelection < 6) {
        KesaSetupLighting();
        KesaRestoreCharacter(KESA_RYO);
        KesaRestoreCharacter(KESA_SHENHUA);
        for (shen_name name : alphaCharacters)
            EV_SetCharacterAlphaEnabled(name, true);
        SetScenarioCharacterDrawControl(shen_name{0}, 1, 0);
    }
    EV_npcLockUnlock(KESA_SHENHUA);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(author.characterPackage), 0);
    constexpr std::uint8_t cleanupOrder[]{0, 2, 3, 4, 1, 5};
    for (auto index : cleanupOrder)
        if (kesaFireEffects[index] != -1) {
            SuspendParticleEffect(4, kesaFireEffects[index]);
            FreeParticleEffect(4, kesaFireEffects[index]);
        }
    AttachFireEffect(kesaTorchEffect, KESA_TORCH, 152, 0);
    AttachFireInstance(kesaTorchEffect, 9, KESA_COMPANION_TORCH, 152, 0);
    EV_FreeAuthorPak(0, author.authorPackage);
    if (kesaAuthorSelection >= 6) {
        Queue_ST_LoadTitleFadeIn(SHEN_NAME(TBCN));
        HLib::SetCurrentTaskTo(EndingFade);
        HLib::SetupTaskDelay(210);
    } else {
        SetPlayerControlEnabled(1);
        camCtrlSetState(Init);
        camCtrlSetFovTransitionTo(true);
        EV_LoadCutsceneFromCurrentScene("KES1");
        EV_LoadAudio("KES1_000", EV_GetShenmueFlag(690) != 0);
        EV_spmBeginSampleExecute();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1757, 1);
        EV_NPC_NewLoadSw(1);
        HLib::DestroyCurrentTask();
    }
    camCtrlResetClipDistances();
    camCtrlRestoreCachedTransform();
}
const ScriptAreaDefinition areaKESAScriptDefinition{.areaId = AREA_KESA, .field04 = 0, .init = AreaKESAInit, .main = AreaKESAMain};
} // namespace shenmue::s2::areas
