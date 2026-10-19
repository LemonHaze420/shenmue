#include "s2/areas/area_kesa.hpp"
#include "s2/areas/kesa_tables.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/particle_effects.hpp"
#include <bit>
#include <cmath>
namespace shenmue::s2::areas {
namespace {
struct FireGrowthWork {
    std::uint32_t effect, frame;
    float width, height, scalar, radiusX, radiusY, gain;
};
static_assert(sizeof(FireGrowthWork) == 0x20);
constexpr Vector3 firePositions[]{{136.17f, 106.64f, -10.8f}, {139.3f, 109.75f, -11.3f}, {134.07f, 107.19f, -13.497f}, {132.267f, 107.99f, -15.31f},
    {129.535f, 108.8f, -17.116f}, {125.854f, 108.26f, -17.91f}, {121.573f, 107.77f, -18.812f}, {117.73f, 110.28f, -19.855f}};
std::array<std::uint16_t, 5> sparkEffects, sparkFineEffects;
Vector3 sparkPreviousPosition;
float sparkRateScale;
void ApplyFireGrowth(const FireGrowthWork &work) {
    SetFireFalloff(work.effect, work.radiusX, work.radiusY, work.gain);
    SetParticleEffectSize(4, work.effect, work.width, work.height);
    SetFireFalloffScalar(work.effect, work.scalar);
}
void FireGrowthLoop(void *data) {
    auto &work = *static_cast<FireGrowthWork *>(data);
    if (work.frame == 0) {
        work.width = 0;
        work.height = 0;
        work.radiusX = 2.3137255f;
        work.radiusY = 5.5294118f;
        work.gain = .70588237f;
        work.scalar = -.19607843f;
        ApplyFireGrowth(work);
        ResumeParticleEffect(4, work.effect);
    } else if (work.frame == 15) {
        work.width = 2.0784314f;
        work.height = 2.1568627f;
        work.radiusX = 3.490196f;
        work.radiusY = 7.098039f;
        work.scalar = -.20980392f;
        ApplyFireGrowth(work);
    } else if (work.frame == 28) {
        work.width = .98039216f;
        work.height = 1.5294118f;
        work.radiusX = 3.1764705f;
        work.radiusY = 6.7843137f;
        work.scalar = -.29803923f;
        ApplyFireGrowth(work);
        HLib::DestroyCurrentTask();
    }
    if (work.frame <= 4) {
        work.width += .086274512f;
        work.height += .10196079f;
        SetParticleEffectSize(4, work.effect, work.width, work.height);
    } else if (work.frame - 5 <= 9) {
        work.width += .16470589f;
        work.height += .16470587f;
        work.radiusX += .11764705f;
        work.radiusY += .15686274f;
        work.scalar -= .001372549f;
        SetParticleEffectSize(4, work.effect, work.width, work.height);
        SetFireFalloff(work.effect, work.radiusX, work.radiusY, work.gain);
        SetFireFalloffScalar(work.effect, work.scalar);
    } else if (work.frame - 15 <= 12) {
        work.width -= .08446455f;
        work.height -= .048265457f;
        work.radiusX -= .024132729f;
        work.radiusY -= .030165892f;
        work.scalar -= .0067873308f;
        SetFireFalloff(work.effect, work.radiusX, work.radiusY, work.gain);
        SetParticleEffectSize(4, work.effect, work.width, work.height);
        SetFireFalloffScalar(work.effect, work.scalar);
    }
    ++work.frame;
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        HLib::DestroyCurrentTask();
}
} // namespace
void KesaCreateTorchEffect() {
    if (kesaTorchEffect != -1)
        return;
    const Vector3 zero{0, 0, 0};
    kesaTorchEffect = static_cast<std::int32_t>(CreateFireEffect(zero, .2f, .5f, 0, 4));
    SetFirePalette(kesaTorchEffect, kesaFirePalette.data());
    SetFireFalloff(kesaTorchEffect, 3.1764705f, 6.7843137f, .70588237f);
    SetFireFalloffScalar(kesaTorchEffect, -.23529412f);
    SetParticleEffectSize(4, kesaTorchEffect, .2f, .5f);
    SetParticleEffectPosition(4, kesaTorchEffect, &zero, &zero);
    ResumeParticleEffect(4, kesaTorchEffect);
    AttachFireEffect(kesaTorchEffect, KESA_TORCH, 152, 0);
    SetFirePosition(kesaTorchEffect, 9, &zero);
    AttachFireInstance(kesaTorchEffect, 9, KESA_COMPANION_TORCH, 152, 0);
}
void KesaInitializeEndingFire() {
    const Vector3 zero{0, 0, 0};
    kesaFireEffects[0] = static_cast<std::int32_t>(CreateFireEffect(zero, .5f, .5f, 0, 1));
    SetFirePalette(kesaFireEffects[0], kesaFirePalette.data());
    SetFireFalloffScalar(kesaFireEffects[0], -.21568628f);
    SetFireFalloff(kesaFireEffects[0], 3.1764705f, 6.3137255f, .8627451f);
    SetParticleEffectSize(4, kesaFireEffects[0], .11764706f, .43137255f);
    ResumeParticleEffect(4, kesaFireEffects[0]);
    for (std::int32_t index = 2; index < 5; ++index) {
        kesaFireEffects[index] = static_cast<std::int32_t>(CreateFireEffect(zero, .2f, .5f, 0, 0));
        SetFirePalette(kesaFireEffects[index], kesaFirePalette.data());
        SetParticleEffectPosition(4, kesaFireEffects[index], &zero, &zero);
        SetFireFalloff(kesaFireEffects[index], 3.1764705f, 6.7843137f, .70588237f);
        SetFireFalloffScalar(kesaFireEffects[index], -.29803923f);
        SetParticleEffectSize(4, kesaFireEffects[index], 0, 0);
    }
    kesaFireEffects[1] = static_cast<std::int32_t>(CreateFireEffect(zero, .2f, .5f, 0, 4));
    SetFirePalette(kesaFireEffects[1], kesaFirePalette.data());
    SetParticleEffectPosition(4, kesaFireEffects[1], &zero, &zero);
    SetFireFalloff(kesaFireEffects[1], 3.1764705f, 6.7843137f, .70588237f);
    SetFireFalloffScalar(kesaFireEffects[1], -.29803923f);
    SetParticleEffectSize(4, kesaFireEffects[1], .98039216f, 1.5294118f);
}
void KesaQueueFireGrowth(std::int32_t effect, std::int32_t positionIndex) {
    auto *task = HLib::EnqueueTaskWithParameter(FireGrowthLoop, 4, 0x0b, sizeof(FireGrowthWork), SHEN_NAME(fire));
    auto &work = *static_cast<FireGrowthWork *>(HLib::GetTaskParameter(task));
    work.effect = effect;
    work.frame = 0;
    SetFireFalloff(effect, 3.1764705f, 6.7843137f, .70588237f);
    SetFireFalloffScalar(effect, -.29803923f);
    SetParticleEffectSize(4, effect, .98039216f, 1.5294118f);
    SetParticleEffectSize(4, effect, 0, 0);
    SetParticleEffectPosition(4, effect, &firePositions[positionIndex], &firePositions[positionIndex]);
}
void KesaInitializeSpark(const Vector3 &position, float scale) {
    if (sparkEffects[0] != 0xffffU)
        return;
    sparkEffects[0] = static_cast<std::uint16_t>(
        CreateSplashEffect(position, {.1f, 0, .1f}, 300, 0, shen_name{0}, std::bit_cast<std::int32_t>(position.y - 1), nullptr, nullptr));
    SetParticleEffectColour(1, sparkEffects[0], 255, 255, 112, 16);
    SetParticleEffectVelocity(1, sparkEffects[0], {0, .1f, 0}, {.5f, .5f, .5f});
    SetParticleEffectSize(1, sparkEffects[0], .03f, .03f);
    ResumeParticleEffect(1, sparkEffects[0]);
    sparkFineEffects[0] = static_cast<std::uint16_t>(
        CreateSplashEffect(position, {.05f, 0, .05f}, 1000, 0, shen_name{0}, std::bit_cast<std::int32_t>(position.y - 1), nullptr, nullptr));
    SetParticleEffectColour(1, sparkFineEffects[0], 255, 255, 160, 32);
    SetParticleEffectVelocity(1, sparkFineEffects[0], {.05f, .03f, 0}, {.02f, .02f, .02f});
    SetParticleEffectSize(1, sparkFineEffects[0], .03f, .03f);
    ResumeParticleEffect(1, sparkFineEffects[0]);
    sparkRateScale = scale;
    sparkPreviousPosition = position;
}
void KesaUpdateSpark(const Vector3 &position, std::int32_t sizeMode) {
    if (sparkEffects[0] == 0xffffU)
        return;
    const Vector3 velocity{(position.x - sparkPreviousPosition.x) * .5f, .03f, (position.z - sparkPreviousPosition.z) * .5f};
    Vector3 camera;
    camCtrlGetNextCamPos(&camera);
    const float dx = position.x - camera.x, dy = position.y - camera.y, dz = position.z - camera.z;
    const float size = std::sqrt(dx * dx + dy * dy + dz * dz) * (sizeMode == 0 ? .0002f : sizeMode == 1 ? .002f : .001f);
    const float movedX = position.x - sparkPreviousPosition.x, movedZ = position.z - sparkPreviousPosition.z;
    const float distance = std::sqrt(movedX * movedX + movedZ * movedZ);
    const Vector3 volume{.1f, 0, .1f};
    SetParticleEffectPosition(1, sparkEffects[0], &position, &volume);
    SetParticleEffectBirthRate(1, sparkEffects[0], .1f);
    SetParticleEffectBirthRate(1, sparkEffects[0], ((distance * distance) * .3f) * sparkRateScale);
    SetParticleEffectSize(1, sparkEffects[0], size, size);
    const Vector3 fineVolume{.05f, 0, .05f};
    SetParticleEffectPosition(1, sparkFineEffects[0], &position, &fineVolume);
    SetParticleEffectVelocity(1, sparkFineEffects[0], velocity, {.02f, .02f, .02f});
    SetParticleEffectBirthRate(1, sparkFineEffects[0], ((distance * distance) * .8f) * sparkRateScale);
    SetParticleEffectBirthRate(1, sparkFineEffects[0], .1f);
    SetParticleEffectSize(1, sparkFineEffects[0], size + size, size + size);
    sparkPreviousPosition = position;
}
void KesaQueueSpark(const Vector3 &position, std::int32_t duration, std::int32_t sizeMode) {
    auto *task = HLib::EnqueueTaskWithParameter(KesaSparkLoop, 6, 0x0b, sizeof(KesaSparkWork), SHEN_NAME(sprk));
    auto &work = *static_cast<KesaSparkWork *>(HLib::GetTaskParameter(task));
    work.state = 0;
    work.duration = duration;
    work.endFrame = duration + 60;
    KesaInitializeSpark(position, 2);
    KesaUpdateSpark(position, sizeMode);
}
void KesaSparkLoop(void *data) {
    auto &work = *static_cast<KesaSparkWork *>(data);
    if (--work.duration <= 0) {
        if (work.duration == 0 && work.state <= 4 && sparkEffects[work.state] != 0xffffU) {
            SetParticleEffectBirthRate(1, sparkEffects[work.state], 0);
            SetParticleEffectBirthRate(1, sparkFineEffects[work.state], 0);
        }
        if (--work.endFrame <= 0) {
            KesaFreeSpark();
            HLib::DestroyCurrentTask();
        }
    }
}
void KesaFreeSpark() {
    if (sparkEffects[0] != 0xffffU) {
        FreeParticleEffect(1, sparkEffects[0]);
        FreeParticleEffect(1, sparkFineEffects[0]);
        sparkEffects[0] = sparkFineEffects[0] = 0xffffU;
    }
}
void KesaResetSparkHandles() {
    for (std::size_t index = 0; index < 4; ++index)
        sparkEffects[index] = sparkFineEffects[index] = 0xffffU;
}
void KesaResetSparkState() {
    sparkEffects = {};
    sparkFineEffects = {};
    sparkPreviousPosition = {};
    sparkRateScale = 0;
}
} // namespace shenmue::s2::areas
