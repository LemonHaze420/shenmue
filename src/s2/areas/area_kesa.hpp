#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "common/math.hpp"
#include <array>
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name KESA_RYO{SHEN_NAME(RYO_)}, KESA_SHENHUA{SHEN_NAME(SIN_)}, KESA_INVALID{0xffffffffU};
inline constexpr shen_name KESA_TORCH{SHEN_NAME(a287)}, KESA_COMPANION_TORCH{SHEN_NAME(a288)};
inline constexpr shen_name KESA_ENERGY_BASE{SHEN_NAME(BASE)}, KESA_ENERGY_PIVOT{SHEN_NAME(BAS2)}, KESA_ENERGY_BALL{SHEN_NAME(BALL)},
    KESA_ENERGY_DUMMY{SHEN_NAME(dumy)};
inline constexpr shen_name KESA_EFFECT_ONE{SHEN_NAME(EFF1)}, KESA_EFFECT_TWO{SHEN_NAME(EFF2)}, KESA_RIPPLE_ONE{SHEN_NAME(RPL1)},
    KESA_RIPPLE_TWO{SHEN_NAME(RPL2)};
struct KesaAuthorEntry {
    shen_name scene, lighting;
    std::uint32_t playerLight, companionLight;
    std::int32_t audioScene;
    std::uint32_t reserved14;
    const char *cutscene;
    const char *authorPackage;
    const char *characterPackage;
};
static_assert(sizeof(KesaAuthorEntry) == 0x30 && offsetof(KesaAuthorEntry, cutscene) == 0x18);
struct KesaEnergyParticle {
    shen_name character;
    Vector3 position, scale;
    std::byte reserved1c[12];
    float direction;
    Vector3 velocity;
    float maximumScale, scaleStep;
    std::int32_t mode;
    shen_name state;
    std::int32_t delay, visible;
};
static_assert(sizeof(KesaEnergyParticle) == 0x50 && offsetof(KesaEnergyParticle, state) == 0x44);
struct KesaSparkWork {
    std::uint16_t state;
    std::uint16_t reserved02;
    std::int32_t duration, endFrame;
};
static_assert(sizeof(KesaSparkWork) == 0xc);
extern std::int32_t kesaAuthorSelection, kesaTorchEffect;
extern std::array<std::int32_t, 6> kesaFireEffects;
extern std::array<KesaEnergyParticle, 40> kesaEnergyParticles;
extern std::int32_t kesaEnergyFrame, kesaEnergyDuration, kesaEnergyActiveParticles;
extern float kesaEnergyLifetime;
extern const std::array<KesaAuthorEntry, 8> kesaAuthors;
void AreaKESAInit();
void AreaKESAMain(std::uintptr_t argument);
void KesaCreateTorchEffect();
void KesaSetLight(void *);
void KesaSetupLighting();
void KesaUpdateTorchLights(std::uint32_t playerLight, std::uint32_t companionLight);
void KesaParseLights(shen_name light);
void KesaSetupTorch();
void KesaResetTorchLightState();
void KesaRestoreCharacter(shen_name character);
void KesaInitializeAuthor(void *);
void KesaFinishAuthor();
void KesaUpdateAuthorPostEffects(std::int32_t frame);
void KesaOpeningTorchLoop(void *);
void KesaOpeningWalkLoop(void *);
void KesaTransitionWalkLoop(void *);
void KesaCaveWalkLoop(void *);
void KesaSwordDiscoveryLoop(void *);
void KesaSwordPreparationLoop(void *);
void KesaSwordActivationLoop(void *);
void KesaEndingLoop(void *);
void KesaPrepareEnding(void *);
void KesaInitializeEnergyBall(std::int32_t mode);
void KesaEnergyBallLoop(void *);
void KesaEnergyBallRiseLoop(void *);
void KesaEnergyBallFollowLoop(void *);
void KesaInitializeEndingFire();
void KesaQueueFireGrowth(std::int32_t effect, std::int32_t light);
void KesaQueueSpark(const Vector3 &position, std::int32_t duration, std::int32_t sizeMode);
void KesaSparkLoop(void *);
void KesaInitializeSpark(const Vector3 &position, float scale);
void KesaUpdateSpark(const Vector3 &position, std::int32_t sizeMode);
void KesaFreeSpark();
void KesaResetSparkHandles();
void KesaResetSparkState();
void KesaResetSwordEffects();
void KesaResetAuthoredState();
extern const ScriptAreaDefinition areaKESAScriptDefinition;
} // namespace shenmue::s2::areas
