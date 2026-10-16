#include "s2/areas/area_kesa.hpp"
#include "s2/areas/kesa_tables.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fog.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/particle_effects.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include <bit>
namespace shenmue::s2::areas {
namespace {
float openingWidth, openingHeight, openingLight, preparationAlpha, preparationLight, endingLightEnvelope, endingFog;
std::int32_t preparationPart, preparationWait, endingLight;
constexpr shen_name swordGlow{SHEN_NAME(a956)}, halo{SHEN_NAME(HLGT)}, specular{SHEN_NAME(SPEC)}, effectThree{SHEN_NAME(EFF3)}, sword{SHEN_NAME(TAMA)};
constexpr Vector3 firePositions[]{{136.17f, 106.64f, -10.8f}, {139.3f, 109.75f, -11.3f}, {134.07f, 107.19f, -13.497f}, {132.267f, 107.99f, -15.31f},
    {129.535f, 108.8f, -17.116f}, {125.854f, 108.26f, -17.91f}, {121.573f, 107.77f, -18.812f}, {117.73f, 110.28f, -19.855f}};
void SetTorchFlag(std::int32_t value) {
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 2228, value);
}
void Black() {
    SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
}
void Reveal() {
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
}
void ApplyPart(shen_name character, std::uint16_t part, Vector3 &position, Vector3i *rotation, EventCharacterPartTransformMode mode) {
    EV_scenApplyPosOrRotToPartOf(character, part, &position, rotation, mode);
}
void ActivateEndingInstance(std::int32_t index) {
    SetFirePosition(kesaFireEffects[1], index, &firePositions[index - 1]);
    SetFireInstanceSize(kesaFireEffects[1], index, .98039216f, 1.5294118f);
}
} // namespace
void KesaResetAuthoredState() {
    openingWidth = openingHeight = openingLight = preparationAlpha = preparationLight = endingLightEnvelope = 0;
    preparationPart = preparationWait = endingLight = 0;
    endingFog = .25f;
}
void KesaOpeningTorchLoop(void *) {
    const auto frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    KesaUpdateTorchLights(1, 2);
    if (frame == 1) {
        openingWidth = openingHeight = openingLight = 0;
        const Vector3 position{53.98f, 96.2f, 117.3f};
        auto &effect = kesaFireEffects[5];
        effect = static_cast<std::int32_t>(CreateFireEffect(position, .2f, .5f, 0, 0));
        SetFirePalette(effect, kesaFirePalette.data());
        SetFireFalloff(effect, 3.14f, 4.94f, .78f);
        SetFireFalloffScalar(effect, -.15f);
        SetParticleEffectSize(4, effect, 0, 0);
        SetParticleEffectPosition(4, effect, &position, &position);
        ResumeParticleEffect(4, effect);
        SetLightSceneIntensity(3, openingLight);
        SetParticleEffectSize(4, effect, openingWidth, openingHeight);
        SuspendParticleEffect(4, effect);
        EV_EnableCharacterDisplay(SHEN_NAME(a636));
    } else {
        if (frame == 410 || frame == 876)
            SuspendParticleEffect(4, kesaFireEffects[5]);
        else if (frame == 766) {
            ResumeParticleEffect(4, kesaFireEffects[5]);
        }
        if (frame != 766) {
            if (static_cast<std::uint32_t>(frame - 100) <= 19) {
                if (frame < 110)
                    openingWidth += .04f;
                openingHeight += .035f;
                SetParticleEffectSize(4, kesaFireEffects[5], openingWidth, openingHeight);
                ResumeParticleEffect(4, kesaFireEffects[5]);
            }
            if (static_cast<std::uint32_t>(frame - 110) <= 19) {
                openingLight += .025f;
                SetLightSceneIntensity(3, openingLight);
            }
        }
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        SuspendParticleEffect(4, kesaFireEffects[5]);
        EV_SetModelDataDefaultVertexShader();
        EV_lightLoad(SHEN_NAME(5141));
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(5141)));
        SetLightEntryIndex(13, 0);
        SetLightEntryIndex(14, 0);
        EV_LoadAudio("0514A000", 1);
        EV_spmBeginSampleExecute();
        Reveal();
        HLib::SetCurrentTaskTo(KesaOpeningWalkLoop);
    }
}
void KesaOpeningWalkLoop(void *) {
    KesaUpdateTorchLights(1, 2);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        Black();
        EV_AT_SetPos(KESA_RYO, EventTransformFlags::AllAxes, {68, 97, 88.7f});
        EV_AT_SetAngY(KESA_RYO, EventTransformFlags::AllAxes, {-1, -1, -1});
        EV_stepSetFlags(0, 732);
        KesaFinishAuthor();
    }
}
void KesaTransitionWalkLoop(void *) {
    KesaUpdateAuthorPostEffects(static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame)));
    KesaUpdateTorchLights(1, 2);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        SetTorchFlag(0);
        ++kesaAuthorSelection;
        KesaParseLights(SHEN_NAME(5380));
        EV_LoadCutsceneFromCurrentScene(kesaAuthors[kesaAuthorSelection].cutscene);
        --kesaAuthorSelection;
        EV_FreeAuthorPak(1, "0538");
        EV_stepSetFlags(0, 462);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(5380)));
        SetLightEntryIndex(13, 0);
        SetLightEntryIndex(14, 0);
        Reveal();
        HLib::SetCurrentTaskTo(KesaCaveWalkLoop);
    }
}
void KesaCaveWalkLoop(void *) {
    const auto frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    KesaUpdateAuthorPostEffects(frame);
    KesaUpdateTorchLights(1, 2);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        Black();
        EV_AT_SetPos(KESA_RYO, EventTransformFlags::AllAxes, {66, 100.68f, 34});
        EV_AT_SetAngY(KESA_RYO, EventTransformFlags::AllAxes, {0, 54614, 0});
        EV_FreeAuthorPak(0, "0538");
        EV_stepSetFlags(0, 463);
        KesaFinishAuthor();
    } else if (frame == 857)
        SetMapModelSlotEnabled(7, 0);
    else if (frame == 877)
        SetMapModelSlotEnabled(7, 1);
}
void KesaSwordDiscoveryLoop(void *) {
    const auto frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    KesaUpdateAuthorPostEffects(frame);
    KesaUpdateTorchLights(kesaAuthors[4].playerLight, kesaAuthors[4].companionLight);
    switch (frame) {
    case 0:
    case 2550:
        camCtrlResetClipDistances();
        break;
    case 290:
    case 1571:
    case 2451:
        SetTorchFlag(1);
        break;
    case 1361:
    case 2400:
    case 3021:
        SetTorchFlag(0);
        break;
    case 1450:
        camCtrlSetClipDistances(.01f, 2200);
        break;
    default:
        break;
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        camCtrlResetClipDistances();
        SetTorchFlag(0);
        Black();
        EV_stepSetFlags(0, 510);
        KesaFinishAuthor();
    }
}
void KesaSwordPreparationLoop(void *) {
    const auto frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    KesaUpdateAuthorPostEffects(frame);
    Vector3 zero{0, 0, 0};
    Vector3i rotation{0, 0, 0};
    switch (frame) {
    case 1:
        EV_AT_SetPos(KESA_TORCH, EventTransformFlags::World | EventTransformFlags::AllAxes, {0, -1000, 0});
        EV_EnableCharacterDisplay(SHEN_NAME(aa75));
        EV_EnableCharacterDisplay(SHEN_NAME(aa36));
        EV_EnableCharacterDisplay(effectThree);
        EV_EnsureCharacterDraw(effectThree);
        EV_EnsureCharacterDraw(swordGlow);
        EV_EnsureCharacterDraw(halo);
        EV_DisableCharacterDisplay(KESA_TORCH);
        SetScenarioCharacterTransparency(effectThree, 0);
        SetScenarioCharacterTransparency(swordGlow, 0);
        SetScenarioCharacterTransparency(halo, 0);
        AT_SetLocalBaseChara(specular, SHEN_NAME(aa39));
        EV_AT_SetPos(specular, EventTransformFlags::AllAxes, zero);
        EV_AT_SetAngY(specular, EventTransformFlags::AllAxes, rotation);
        EV_EnableCharacterDisplay(specular);
        preparationPart = 152;
        while (preparationPart <= 159)
            ApplyPart(specular, static_cast<std::uint16_t>(preparationPart++), zero, nullptr, EventCharacterPartTransformMode::Set);
        preparationPart = 152;
        preparationAlpha = 0;
        preparationWait = 10800;
        SetTorchFlag(1);
        break;
    case 240:
        SetIsSkySetup(0);
        break;
    case 435:
        SetIsSkySetup(1);
        break;
    case 746:
    case 2341:
        SetTorchFlag(0);
        break;
    case 831:
    case 2702:
        SetTorchFlag(1);
        break;
    case 1000:
        EV_authSceneCtrl(AuthSceneCtrl::SetAuthorStopped);
        GetBattleDataConfig(0, SHEN_NAME(BTSD));
        CheckHelpIcon(0x100000, 0, CheckHelpIconOp::FrameOverlay);
        if (GetBattleDataConfig(2, SHEN_NAME(BTSD)) || --preparationWait <= 0)
            EV_authSceneCtrl(AuthSceneCtrl::ResetAuthorStopped);
        break;
    case 1520:
        preparationAlpha = 1;
        break;
    case 1528:
        EV_DisableCharacterDisplay(effectThree);
        EV_ToggleCharacterFlag(effectThree);
        preparationAlpha = 0;
        break;
    case 1700:
        EV_EnableCharacterDisplay(swordGlow);
        EV_EnableCharacterDisplay(halo);
        SetScenarioCharacterTransparency(swordGlow, preparationAlpha);
        SetScenarioCharacterTransparency(halo, preparationAlpha);
        SetLightSceneIntensity(3, preparationLight);
        break;
    case 1710:
        SetNewWindPhaseAndSpeed(16384, .5f);
        break;
    case 1730:
        SetScenarioCharacterTransparency(swordGlow, 1);
        SetScenarioCharacterTransparency(halo, 1);
        SetLightSceneIntensity(3, 1.5f);
        preparationAlpha = 1;
        preparationLight = 1.5f;
        break;
    case 1750:
        kesaEnergyDuration = 180;
        KesaInitializeEnergyBall(1);
        HLib::EnqueueTaskWithoutParameter(KesaEnergyBallRiseLoop, 6, 0x0b, static_cast<std::int32_t>(SHEN_NAME(engy)));
        break;
    case 1930:
        SetScenarioCharacterTransparency(sword, 1);
        break;
    case 1935:
        SetNewWindPhaseAndSpeed(16384, 0);
        SetClothFlag(FindCharacterByScenarioName(KESA_SHENHUA), 16, 1);
        break;
    case 1945:
        SetLightSceneIntensity(3, preparationLight);
        break;
    default:
        break;
    }
    if (frame >= 1520 && frame <= 1525) {
        SetScenarioCharacterTransparency(effectThree, preparationAlpha);
        rotation.z = 5461;
        ApplyPart(effectThree, 152, zero, &rotation, EventCharacterPartTransformMode::Relative);
        rotation.z = 60075;
        ApplyPart(effectThree, 153, zero, &rotation, EventCharacterPartTransformMode::Relative);
        preparationAlpha -= .17f;
    }
    if (static_cast<std::uint32_t>(frame - 941) <= 7) {
        Vector3 leave{-1, -1, -1}, one{1, 1, 1};
        if (static_cast<std::uint32_t>(preparationPart - 152) <= 7)
            ApplyPart(specular, static_cast<std::uint16_t>(preparationPart), leave, nullptr, EventCharacterPartTransformMode::Set);
        ++preparationPart;
        if (static_cast<std::uint32_t>(preparationPart - 152) <= 7)
            ApplyPart(specular, static_cast<std::uint16_t>(preparationPart), one, nullptr, EventCharacterPartTransformMode::Set);
        if (frame == 945)
            ApplyPart(specular, 159, one, nullptr, EventCharacterPartTransformMode::Set);
    }
    if (frame >= 945 && frame < 949) {
        preparationAlpha += .25f;
        SetScenarioCharacterTransparency(specular, preparationAlpha);
    }
    if (frame >= 949 && frame < 952) {
        preparationAlpha -= .33333334f;
        SetScenarioCharacterTransparency(specular, preparationAlpha);
    }
    if (frame == 952)
        EV_DisableCharacterDisplay(specular);
    else {
        if (static_cast<std::uint32_t>(frame - 1710) <= 19) {
            preparationLight = preparationAlpha * 1.5f;
            SetScenarioCharacterTransparency(swordGlow, preparationAlpha);
            SetScenarioCharacterTransparency(halo, preparationAlpha);
            SetLightSceneIntensity(3, preparationLight);
            preparationAlpha += .05f;
        }
        if (frame >= 1875 && frame < 1925) {
            preparationAlpha -= .02f;
            SetScenarioCharacterTransparency(swordGlow, preparationAlpha);
            SetScenarioCharacterTransparency(halo, preparationAlpha);
        } else if (frame >= 1925 && frame < 1945) {
            preparationLight -= .075f;
            SetLightSceneIntensity(3, preparationLight);
        }
    }
    KesaUpdateTorchLights(0xffffffffU, kesaAuthors[5].companionLight);
    SetFireAlpha(kesaTorchEffect, 0);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        SetNewWindPhaseAndSpeed(16384, 0);
        SetClothFlag(FindCharacterByScenarioName(KESA_SHENHUA), 16, 1);
        SetScenarioCharacterTransparency(sword, 1);
        Black();
        EV_stepSetFlags(0, 227);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 2196, 1);
        KesaFinishAuthor();
    }
}
void KesaPrepareEnding(void *) {
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("MPK01"), 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("AUTHOR5"), 0);
    EV_npcLockUnlock(KESA_SHENHUA);
    EV_npcFree(KESA_SHENHUA);
    EV_ResetFreeTalkFlags();
    AttachFireEffect(kesaTorchEffect, shen_name{0}, 0, 0);
    SuspendParticleEffect(4, kesaTorchEffect);
    FreeParticleEffect(4, kesaTorchEffect);
    kesaTorchEffect = -1;
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("AUTHOR6"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_CheckWarmStartNpcRoute(KESA_SHENHUA);
    EV_ForceLoadNpc(KESA_SHENHUA);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_EnableCharacterDisplay(KESA_SHENHUA);
    EV_SetCharaControllerFlags(KESA_SHENHUA, 1, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(KESA_SHENHUA, 3, NpcCharacterControllerState::Disabled);
    mtwkSetExtraFlag(scenFindMotionwork(KESA_SHENHUA), 6, 2);
    EV_SetCharacterAlphaEnabled(KESA_SHENHUA, false);
    SetScenarioCharacterRadius(KESA_SHENHUA, 10);
    KesaCreateTorchEffect();
    ++kesaAuthorSelection;
    KesaParseLights(SHEN_NAME(5161));
    SetLightEntryIndex(13, 0);
    SetLightEntryIndex(14, 0);
    EV_LoadCutsceneFromCurrentScene("0516A010");
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(5161)));
    Reveal();
    KesaEndingLoop(nullptr);
    HLib::SetCurrentTaskTo(KesaEndingLoop);
}
void KesaEndingLoop(void *) {
    const auto frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    KesaUpdateAuthorPostEffects(frame);
    switch (frame) {
    case 0:
        ResetAuthorAudioState();
        if (IsAuthorMultichannelAudioEnabled())
            InitializeAuthorMultichannelAudio();
        if (IsAuthorMultichannelAudioEnabled() && IsAuthorMultichannelAudioActive()) {
            SetSamplePlaybackSuppressed(1);
            SetAuthorStreamingAudioEnabled(0);
            StartAuthorMultichannelAudio(0);
        }
        KesaInitializeEndingFire();
        ResumeParticleEffect(4, kesaFireEffects[1]);
        ActivateEndingInstance(1);
        KesaResetSparkHandles();
        EV_DisableCharacterDisplay(SHEN_NAME(RUBY));
        for (auto index : {4U, 5U, 6U, 7U})
            SetLightSceneIntensity(index, 0);
        SetTorchFlag(1);
        break;
    case 1:
    case 655:
        EV_SetObjectFlagByIndex(6, 0);
        break;
    case 179:
        KesaQueueFireGrowth(kesaFireEffects[2], 1);
        KesaQueueSpark(firePositions[1], 8, 1);
        endingLight = 4;
        endingLightEnvelope = 0;
        break;
    case 384:
        KesaQueueFireGrowth(kesaFireEffects[3], 2);
        KesaQueueSpark(firePositions[2], 8, 1);
        endingLight = 5;
        endingLightEnvelope = 0;
        break;
    case 400:
    case 715:
        EV_SetObjectFlagByIndex(6, 1);
        break;
    case 420:
        ResumeParticleEffect(4, kesaFireEffects[1]);
        SuspendParticleEffect(4, kesaFireEffects[2]);
        SuspendParticleEffect(4, kesaFireEffects[3]);
        for (std::int32_t index = 2; index < 6; ++index)
            SetFirePosition(kesaFireEffects[1], index, &firePositions[index - 1]);
        for (std::int32_t index = 2; index < 6; ++index)
            SetFireInstanceSize(kesaFireEffects[1], index, .98039216f, 1.5294118f);
        break;
    case 684:
        KesaQueueFireGrowth(kesaFireEffects[2], 6);
        KesaQueueSpark(firePositions[6], 8, 1);
        endingLight = 6;
        endingLightEnvelope = 0;
        break;
    case 820:
        SuspendParticleEffect(4, kesaFireEffects[0]);
        SuspendParticleEffect(4, kesaFireEffects[2]);
        SetFirePosition(kesaFireEffects[1], 6, &firePositions[5]);
        SetFireInstanceSize(kesaFireEffects[1], 6, .98039216f, 1.5294118f);
        break;
    case 855:
        KesaQueueFireGrowth(kesaFireEffects[2], 7);
        KesaQueueSpark(firePositions[7], 8, 1);
        endingLight = 7;
        endingLightEnvelope = 0;
        break;
    case 930:
        SuspendParticleEffect(4, kesaFireEffects[2]);
        for (std::int32_t index = 7; index < 9; ++index)
            SetFirePosition(kesaFireEffects[1], index, &firePositions[index - 1]);
        for (std::int32_t index = 7; index < 9; ++index)
            SetFireInstanceSize(kesaFireEffects[1], index, .98039216f, 1.5294118f);
        break;
    case 1000:
        EV_EnableCharacterDisplay(SHEN_NAME(RUBY));
        break;
    case 1446:
    case 4138:
        SetTorchFlag(0);
        break;
    case 2568:
        for (auto index : {5, 7, 13, 14})
            SetMapModelAlternateTraversalEnabled(0, index);
        break;
    case 2808:
        SuspendParticleEffect(4, kesaTorchEffect);
        for (auto index : {5, 7, 13, 14})
            SetMapModelAlternateTraversalEnabled(1, index);
        break;
    case 3028:
        ResumeParticleEffect(4, kesaTorchEffect);
        break;
    case 3428:
        SetFirePosition(kesaTorchEffect, 9, nullptr);
        break;
    case 3793: {
        const Vector3 zero{0, 0, 0};
        SetFirePosition(kesaTorchEffect, 9, &zero);
        AttachFireInstance(kesaTorchEffect, 9, KESA_COMPANION_TORCH, 152, std::bit_cast<std::int32_t>(.1f));
        SetTorchFlag(1);
        break;
    }
    case 4773:
        SuspendParticleEffect(4, kesaTorchEffect);
        SetFirePosition(kesaTorchEffect, 9, nullptr);
        SetLightEntryIndex(2, 0);
        break;
    case 5060:
        AttachFireEffect(kesaTorchEffect, KESA_COMPANION_TORCH, 152, .1f);
        ResumeParticleEffect(4, kesaTorchEffect);
        break;
    default:
        break;
    }
    const float wave = sin_fixed16(static_cast<std::int16_t>(((frame * 4.0f) * 8192) / 45));
    SetScenarioCharacterTransparency(SHEN_NAME(RUBY), wave * .25f + .25f);
    if (endingLight) {
        endingLightEnvelope += .05f;
        SetLightSceneIntensity(endingLight, endingLightEnvelope);
        if (endingLightEnvelope >= 1)
            endingLight = 0;
    }
    if (static_cast<std::uint32_t>(frame - 4774) <= 631) {
        endingFog += .0011867088f;
        SetFogBlue(endingFog);
        ReinitFogTasks(1);
    } else if (frame == 5406) {
        endingFog = 1;
        SetFogBlue(1);
    }
    Vector3 position;
    EV_GetCharaPosition(SHEN_NAME(a292), CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    SetParticleEffectPosition(4, kesaFireEffects[0], &position, &position);
    KesaUpdateTorchLights(kesaAuthors[7].playerLight, kesaAuthors[7].companionLight);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        Black();
        EV_stepSetFlags(0, 228);
        KesaFinishAuthor();
    }
}
} // namespace shenmue::s2::areas
