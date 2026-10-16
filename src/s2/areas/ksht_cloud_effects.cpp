#include "s2/areas/area_ksht.hpp"
#include "s2/particle_effects.hpp"
namespace shenmue::s2::areas {
void CreateKshtLeaves(bool alternate) {
    const Vector3 position = alternate ? Vector3{56.1f, 104.4f, 51.9f} : Vector3{164.66f, 110.4f, 192.4f};
    const Vector3 spawn{2.5f, 0.02f, 2.5f}, velocity{0.01f, 0.015f, 0.01f}, variance{0.01f, 0.005f, 0.01f};
    kshtLeafEffect = static_cast<std::uint32_t>(
        CreateFallingParticleEffect(position, spawn, 300, alternate ? 0.2f : 0.4f, shen_name{0}, "MISC", "SHENLEAF.PVR", alternate ? 99.3f : 96.6f));
    SetParticleEffectSize(3, kshtLeafEffect, 0.05f, 0.05f);
    SetParticleEffectVelocity(3, kshtLeafEffect, velocity, variance);
    ResumeParticleEffect(3, kshtLeafEffect);
}
void CreateKshtBreath() {
    const Vector3 position{0, 0, 0}, spawn{0, 0.003f, 0}, velocity{0, 0.003f, 0}, variance{0, 0.003f, 0};
    for (std::uint32_t &effect : kshtBreathEffects) {
        effect = static_cast<std::uint32_t>(CreateSmokeEffect(position, spawn, 40, 0.08f, shen_name{0}, 0));
        SetSmokeEffectFalloff(effect, 10);
        SetSmokeEffectFadeFactor(effect, 144);
        SetParticleEffectColour(0, effect, 0, 255, 255, 255);
        SetParticleEffectInitialScale(0, effect, 0.5f);
        SetParticleEffectSize(0, effect, 0.3f, 0.3f);
        SetParticleEffectBirthRate(0, effect, 0.003f);
        SetParticleEffectVelocity(0, effect, velocity, variance);
        ResumeParticleEffect(0, effect);
    }
}
} // namespace shenmue::s2::areas
