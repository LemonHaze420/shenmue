#include "s2/areas/area_kesa.hpp"
#include "s2/areas/kesa_tables.hpp"
#include "s2/author.hpp"
#include "s2/audio_data.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/particle_effects.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/world_render.hpp"
#include <algorithm>
namespace shenmue::s2::areas {
namespace {
constexpr shen_name glow{SHEN_NAME(a956)}, energy{SHEN_NAME(ENRG)}, ripple{SHEN_NAME(RPL3)}, rippleRoot{SHEN_NAME(rpl3)};
constexpr shen_name beamOne{SHEN_NAME(a922)}, beamTwo{SHEN_NAME(a924)};
constexpr Vector3 sunPosition{125.62f, 106.62f, 3.68f}, sparkPosition{115.75f, 107, -14.92f}, firePosition{136.17f, 106.59f, -10.79f};
Vector4 reflector, companionReflector;
Vector3 rippleOneScale, rippleTwoScale, energyScale, partScale;
float alpha, lightFalloff[2], fireWidth, fireHeight, fireRadiusX, fireRadiusY, fireGain, fireScalar;
std::int32_t waitFrames, partIndex, fire;
bool firstBeam, secondBeam;
void Scale(shen_name character, const Vector3 &value) {
    mtwkSetMotionworkScale3(character, &value);
}
void TorchFlag(std::int32_t value) {
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 2228, value);
}
bool In(std::int32_t frame, std::int32_t first, std::uint32_t extent) {
    return static_cast<std::uint32_t>(frame - first) <= extent;
}
shen_name Flame(std::int32_t index) {
    return shen_name{SHEN_NAME(FLC0) + static_cast<std::uint32_t>(index) * 0x01000000U};
}
void Reflect(shen_name character, const Vector4 &value) {
    SetScenarioReflectorBoardValues(character, &value);
}
void ApproachReflector(Vector4 &value, float y, float z, float w, float divisor) {
    value.y += (y - value.y) / divisor;
    value.z += (z - value.z) / divisor;
    value.w += (w - value.w) / divisor;
}
void ApplyFire() {
    SetParticleEffectSize(4, fire, fireWidth, fireHeight);
    SetFireFalloff(fire, fireRadiusX, fireRadiusY, fireGain);
    SetFireFalloffScalar(fire, fireScalar);
}
bool AdvanceBeam(shen_name character, std::int32_t frame, std::int32_t first, std::int32_t last) {
    if (frame < first || frame > last)
        return false;
    Vector3 zero{0, 0, 0};
    Vector3 one{1, 1, 1};
    if (frame == first) {
        EV_EnableCharacterDisplay(character);
        for (std::int32_t bone = character.name == beamOne.name ? 152 : 184, end = bone + 32; bone < end; ++bone)
            EV_scenApplyPosOrRotToPartOf(character, static_cast<std::uint16_t>(bone), &zero, nullptr, EventCharacterPartTransformMode::Set);
    }
    auto bone = partIndex;
    EV_scenApplyPosOrRotToPartOf(character, static_cast<std::uint16_t>(bone), &one, nullptr, EventCharacterPartTransformMode::Set);
    if ((character.name == beamOne.name && bone < 183) || (character.name == beamTwo.name && bone < 215))
        ++bone;
    Vector3 position;
    Vector3i rotation;
    EV_scenApplyPosOrRotToPartOf(character, static_cast<std::uint16_t>(bone), &position, &rotation, EventCharacterPartTransformMode::Get);
    SetLightRuntimePosition(13, position);
    return true;
}
} // namespace
void KesaResetSwordEffects() {
    reflector = companionReflector = {};
    rippleOneScale = rippleTwoScale = energyScale = partScale = {};
    alpha = lightFalloff[0] = lightFalloff[1] = fireWidth = fireHeight = fireRadiusX = fireRadiusY = fireGain = fireScalar = 0;
    waitFrames = partIndex = fire = 0;
    firstBeam = secondBeam = false;
}

void KesaSwordActivationLoop(void *) {
    using enum CameraState;
    const auto frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    KesaUpdateAuthorPostEffects(frame);
    KesaUpdateTorchLights(kesaAuthors[6].playerLight, kesaAuthors[6].companionLight);
    switch (frame) {
    case 1: {
        EV_EnableCharacterDisplay(glow);
        SetScenarioCharacterTransparency(glow, 0);
        EV_DisableCharacterDisplay(energy);
        energyScale = {0, 0, 0};
        Scale(energy, energyScale);
        SetScenarioCharacterTransparency(energy, 0);
        TorchFlag(1);
        for (std::int32_t index = 0; index < 5; ++index) {
            const auto character = Flame(index);
            AT_SetLocalBaseChara(character, glow);
            EV_AT_SetPos(character, EventTransformFlags::AllAxes, {index % 2 ? .05f : -.05f, index * .15f - .3f, -.01f});
            EV_AT_SetAngY(character, EventTransformFlags::AllAxes, {0, 0, 0});
            EV_DisableCharacterDisplay(character);
            SetScenarioCharacterTransparency(character, 0);
        }
        CreateEventCharacter(CharacterCreationHighModel, nullptr, rippleRoot);
        EV_AT_SetPos(rippleRoot, EventTransformFlags::AllAxes, {115.8f, 106.98f, -14.9f});
        EV_AT_SetAngY(rippleRoot, EventTransformFlags::AllAxes, {16384, 14563, 0});
        AT_SetLocalBaseChara(ripple, rippleRoot);
        EV_AT_SetPos(ripple, EventTransformFlags::AllAxes, {0, 0, 0});
        EV_AT_SetAngY(ripple, EventTransformFlags::AllAxes, {0, 0, 0});
        EV_DisableCharacterDisplay(ripple);
        partIndex = 152;
        firstBeam = secondBeam = false;
        alpha = 0;
        fire = 0;
        waitFrames = 10800;
        for (std::int32_t index = 0; index < 4; ++index)
            EV_SetObjectFlagByIndex(index, 0);
        ResetAuthorAudioState();
        if (IsAuthorMultichannelAudioEnabled())
            InitializeAuthorMultichannelAudio();
        break;
    }
    case 625:
        EV_authSceneCtrl(AuthSceneCtrl::SetAuthorStopped);
        GetBattleDataConfig(0, SHEN_NAME(BTMR));
        CheckHelpIcon(0x100000, 0, CheckHelpIconOp::FrameOverlay);
        if (GetBattleDataConfig(2, SHEN_NAME(BTMR)) || --waitFrames <= 0)
            EV_authSceneCtrl(AuthSceneCtrl::ResetAuthorStopped);
        break;
    case 806:
        reflector = {1, 0, .05f, .25f};
        break;
    case 861:
        SetLightSceneIntensity(3, alpha);
        TorchFlag(0);
        break;
    case 951:
        SetLightSceneIntensity(3, alpha);
        TorchFlag(1);
        break;
    case 1240:
        KesaInitializeEnergyBall(2);
        HLib::EnqueueTaskWithoutParameter(KesaEnergyBallFollowLoop, 6, 0x0b, static_cast<std::int32_t>(SHEN_NAME(engy)));
        alpha = 0;
        break;
    case 1251:
    case 1666:
    case 1880:
    case 2096:
        TorchFlag(0);
        if (frame == 1666)
            EV_EnableCharacterDisplay(KESA_EFFECT_TWO);
        break;
    case 1535:
        if (IsAuthorMultichannelAudioEnabled() && IsAuthorMultichannelAudioActive()) {
            SetSamplePlaybackSuppressed(1);
            SetAuthorStreamingAudioEnabled(0);
            StartAuthorMultichannelAudio(0);
        }
        KesaInitializeEnergyBall(0);
        HLib::EnqueueTaskWithoutParameter(KesaEnergyBallLoop, 6, 0x0b, static_cast<std::int32_t>(SHEN_NAME(engy)));
        break;
    case 1536:
        TorchFlag(1);
        break;
    case 1650:
        reflector = {1, .07f, .05f, 0};
        companionReflector = {1, 0, 0, .05f};
        break;
    case 1741:
        energyScale = {.3f, .07f, .3f};
        Scale(KESA_EFFECT_TWO, energyScale);
        TorchFlag(1);
        break;
    case 1791:
        EV_DisableCharacterDisplay(KESA_EFFECT_TWO);
        break;
    case 1820:
        SetSunDirectionVector(&sunPosition);
        break;
    case 1843:
        EV_EnableCharacterDisplay(beamOne);
        break;
    case 1850:
        SetSunDirectionVector(nullptr);
        break;
    case 1851:
        EV_DisableCharacterDisplay(KESA_EFFECT_ONE);
        break;
    case 1852:
    case 1866:
        if (frame == 1866) {
            SetLightEntryIndex(8, 1);
            CopyLightRuntimeEntry(13, 8);
            SetLightEntryIndex(8, 0);
        }
        rippleOneScale = {.5f, .5f, 1};
        rippleTwoScale = {0, 0, 1};
        alpha = 1;
        EV_EnableCharacterDisplay(KESA_RIPPLE_ONE);
        EV_EnableCharacterDisplay(KESA_RIPPLE_TWO);
        EV_DisableCharacterDisplay(beamOne);
        break;
    case 1870:
        EV_DisableCharacterDisplay(KESA_RIPPLE_ONE);
        EV_DisableCharacterDisplay(KESA_RIPPLE_TWO);
        break;
    case 1897:
        firstBeam = true;
        KesaResetSparkHandles();
        KesaQueueSpark(sparkPosition, 5, 1);
        break;
    case 1911:
        reflector = {1, .05f, .2f, .25f};
        lightFalloff[0] = lightFalloff[1] = 0;
        break;
    case 2003:
        SetLightEntryIndex(1, 1);
        CopyLightRuntimeEntry(13, 1);
        SetLightEntryIndex(1, 0);
        secondBeam = true;
        fire = static_cast<std::int32_t>(CreateFireEffect(firePosition, .2f, .5f, 0, 0));
        SetFirePalette(fire, kesaFirePalette.data());
        ResumeParticleEffect(4, fire);
        fireRadiusX = 2.3137255f;
        fireRadiusY = 5.5294118f;
        fireGain = .70588237f;
        fireScalar = -.19607843f;
        fireWidth = fireHeight = 0;
        KesaResetSparkHandles();
        KesaQueueSpark(firePosition, 25, 2);
        TorchFlag(1);
        break;
    case 2041:
        fireRadiusX = 3.1764705f;
        fireRadiusY = 6.7843137f;
        fireScalar = -.29803923f;
        fireWidth = .98039216f;
        fireHeight = 1.5294118f;
        ApplyFire();
        break;
    case 2186:
        reflector = reflectorDefaultValues;
        break;
    default:
        break;
    }
    if (In(frame, 806, 54)) {
        Reflect(SHEN_NAME(a067), reflector);
        ApproachReflector(reflector, 1, 1, 1, 875.0f - frame);
    }
    if (In(frame, 2186, 64)) {
        Reflect(SHEN_NAME(a067), reflector);
        ApproachReflector(reflector, 0, .05f, .25f, 2160.0f - frame);
    }
    if (In(frame, 925, 25)) {
        SetLightSceneIntensity(3, alpha);
        alpha += .04f;
    }
    if (In(frame, 1453, 5) || In(frame, 1461, 4)) {
        alpha += .1f;
        SetScenarioCharacterTransparency(glow, std::min(alpha, 1.0f));
        for (std::int32_t index = 0; index < 5; ++index)
            SetScenarioCharacterTransparency(Flame(index), alpha >= 1 ? .3f : alpha * .3f);
        SetLightSceneIntensity(3, alpha);
    }
    if (In(frame, 1620, 45)) {
        SetLightFalloff(7, lightFalloff);
        if (frame != 1665)
            lightFalloff[0] += (.6f - lightFalloff[0]) / (1665.0f - frame);
        lightFalloff[1] = lightFalloff[0];
    }
    if (In(frame, 1650, 15)) {
        Reflect(KESA_RYO, reflector);
        Reflect(KESA_SHENHUA, reflector);
        Reflect(SHEN_NAME(aa36), companionReflector);
        if (frame != 1665) {
            ApproachReflector(reflector, -.14f, -.1f, 0, 1665.0f - frame);
            ApproachReflector(companionReflector, -.2f, -.2f, -.4f, 1665.0f - frame);
        }
    }
    if (In(frame, 1851, 3) || In(frame, 1866, 3)) {
        Scale(KESA_RIPPLE_ONE, rippleOneScale);
        Scale(KESA_RIPPLE_TWO, rippleTwoScale);
        SetScenarioCharacterTransparency(KESA_RIPPLE_ONE, std::max(alpha, 0.0f));
        SetScenarioCharacterTransparency(KESA_RIPPLE_TWO, std::max(alpha, 0.0f));
        rippleOneScale.y += 4;
        rippleOneScale.x = rippleOneScale.y;
        rippleTwoScale.y += 3;
        rippleTwoScale.x = rippleTwoScale.y;
        alpha -= .25f;
    }
    if (frame == 1900) {
        alpha = 0;
        partScale = {0, 0, 0};
        EV_EnableCharacterDisplay(ripple);
    }
    if (In(frame, 1900, 70)) {
        Vector3i rotation;
        GetScenarioCharacterEulerAngles(ripple, &rotation, EventTransformFlags::Local);
        rotation.y += 1820;
        EV_AT_SetAngY(ripple, EventTransformFlags::AllAxes, rotation);
        if (frame <= 1940) {
            SetScenarioCharacterTransparency(ripple, alpha);
            alpha += .025f;
        }
        partScale.z += .02f;
        partScale.y = partScale.z;
        partScale.x = partScale.z;
        EV_scenApplyPosOrRotToPartOf(ripple, 154, &partScale, nullptr, EventCharacterPartTransformMode::Set);
    }
    if (frame == 1971) {
        EV_DisableCharacterDisplay(ripple);
        EV_DisableCharacterDisplay(energy);
    }
    if (In(frame, 1911, 54)) {
        SetLightFalloff(4, lightFalloff);
        if (frame != 1965)
            lightFalloff[0] += (.6f - lightFalloff[0]) / (1965.0f - frame);
        lightFalloff[1] = lightFalloff[0];
        Reflect(SHEN_NAME(aa39), reflector);
        if (frame != 1965)
            ApproachReflector(reflector, 1, .1f, .1f, 1965.0f - frame);
    }
    if (In(frame, 1898, 72)) {
        SetScenarioCharacterTransparency(energy, .5f);
        const float scale = ((frame - 1897.0f) / 74) * ((frame - 1897.0f) / 74) * .3f;
        energyScale = {scale, scale, scale};
        Scale(energy, energyScale);
        Vector3 zero{0, 0, 0};
        Vector3i rotation{0, 0, 63716};
        EV_scenApplyPosOrRotToPartOf(energy, 152, &zero, &rotation, EventCharacterPartTransformMode::Relative);
        rotation.z = 1820;
        EV_scenApplyPosOrRotToPartOf(energy, 153, &zero, &rotation, EventCharacterPartTransformMode::Relative);
        EV_EnableCharacterDisplay(energy);
    }
    if (In(frame, 1453, 537) || frame == 1971) {
        for (std::int32_t index = 0; index < 5; ++index) {
            const auto character = Flame(index);
            const float scale = GetRandomNumberInRange(.4f) - .2f + 4;
            Scale(character, {scale, scale, scale});
            Vector3i rotation;
            GetScenarioCharacterEulerAngles(character, &rotation, EventTransformFlags::Local);
            rotation.z += (character.name & 0x01000000U) ? 910 : -910;
            EV_AT_SetAngY(character, EventTransformFlags::AllAxes, rotation);
            EV_EnableCharacterDisplay(character);
        }
    }
    if (frame == 1990)
        for (std::int32_t index = 0; index < 5; ++index)
            EV_EnableCharacterDisplay(Flame(index));
    if (frame == 1852)
        partIndex = 152;
    if (AdvanceBeam(beamOne, frame, 1852, 1865))
        ++partIndex;
    if (frame == 1866)
        partIndex = 152;
    if (AdvanceBeam(beamOne, frame, 1866, 1897))
        ++partIndex;
    if (frame == 1971)
        partIndex = 184;
    if (AdvanceBeam(beamTwo, frame, 1971, 2002))
        ++partIndex;
    if (In(frame, 2003, 21)) {
        ApplyFire();
        fireWidth += .20915033f;
        fireHeight += .22222222f;
        fireRadiusX += .078431368f;
        fireRadiusY += .10457516f;
        fireScalar -= .0067973863f;
    }
    if (In(frame, 2025, 15)) {
        SetParticleEffectSize(4, fire, fireWidth, fireHeight);
        SetFireFalloff(fire, fireRadiusX, fireRadiusY, fireGain);
        fireWidth -= .098039217f;
        fireHeight -= .081996433f;
        fireRadiusX -= .014260248f;
        fireRadiusY -= .014260248f;
    }
    if (frame == 2069)
        SetParticleEffectSize(4, fire, fireWidth, fireHeight);
    if (firstBeam) {
        EV_DisableCharacterDisplay(beamOne);
        EV_SetObjectFlagByIndex(0, 1);
        EV_SetObjectFlagByIndex(1, 1);
        const float v = -GetRandomNumberInRange(.005f);
        SetTextureAnimationUV(GetRandomNumberInRange(.01f), v);
        ApplyTextureAnimationResource(1);
    }
    if (secondBeam) {
        EV_DisableCharacterDisplay(beamTwo);
        EV_SetObjectFlagByIndex(2, 1);
        EV_SetObjectFlagByIndex(3, 1);
        ApplyTextureAnimationResource(3);
    }
    if (frame == 2096) {
        alpha = 1;
        EV_EnableCharacterDisplay(beamOne);
        EV_EnableCharacterDisplay(beamTwo);
        EV_SetCharacterAlphaEnabled(beamOne, false);
        EV_SetCharacterAlphaEnabled(beamTwo, false);
        for (std::int32_t index = 0; index < 4; ++index)
            EV_SetObjectFlagByIndex(index, 0);
        firstBeam = secondBeam = false;
        if (IsAuthorMultichannelAudioEnabled() && IsAuthorMultichannelAudioActive()) {
            ReleaseAuthorMultichannelAudio();
            ResetAuthorAudioState();
        }
    }
    if (In(frame, 2096, 89)) {
        alpha -= .011f;
        if (alpha > 0) {
            SetScenarioCharacterTransparency(beamOne, alpha);
            SetScenarioCharacterTransparency(beamTwo, alpha);
        } else {
            EV_DisableCharacterDisplay(beamOne);
            EV_DisableCharacterDisplay(beamTwo);
        }
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        SetScenarioCharacterKillMode(rippleRoot, 1);
        if (fire) {
            SuspendParticleEffect(4, fire);
            FreeParticleEffect(4, fire);
        }
        camCtrlSetState(Init);
        camCtrlSetFovTransitionTo(true);
        SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
        for (shen_name name : {SHEN_NAME(aa39), SHEN_NAME(aa75), SHEN_NAME(aa20), SHEN_NAME(TAMA), SHEN_NAME(aa36), SHEN_NAME(a636), SHEN_NAME(aa85),
                 SHEN_NAME(aa33), SHEN_NAME(a704)})
            EV_DisableCharacterDisplay(name);
        HLib::SetCurrentTaskTo(KesaPrepareEnding);
        HLib::SetupTaskDelay(5);
    }
}
} // namespace shenmue::s2::areas
