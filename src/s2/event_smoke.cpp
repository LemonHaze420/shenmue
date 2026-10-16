#include "s2/event_smoke.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/particle_effects.hpp"
namespace shenmue::s2 {
namespace {
struct SmokeProfile {
    Vector3 volume, velocity, variance;
    std::int32_t count;
    float birthFraction, initialScale, width, height;
    std::int32_t red, green, blue, falloff, fade;
};
static_assert(sizeof(SmokeProfile) == 76);
static_assert(offsetof(SmokeProfile, velocity) == 0x0c);
static_assert(offsetof(SmokeProfile, variance) == 0x18);
static_assert(offsetof(SmokeProfile, count) == 0x24);
static_assert(offsetof(SmokeProfile, birthFraction) == 0x28);
static_assert(offsetof(SmokeProfile, initialScale) == 0x2c);
static_assert(offsetof(SmokeProfile, width) == 0x30);
static_assert(offsetof(SmokeProfile, height) == 0x34);
static_assert(offsetof(SmokeProfile, red) == 0x38);
static_assert(offsetof(SmokeProfile, green) == 0x3c);
static_assert(offsetof(SmokeProfile, blue) == 0x40);
static_assert(offsetof(SmokeProfile, falloff) == 0x44);
static_assert(offsetof(SmokeProfile, fade) == 0x48);
const SmokeProfile smokeProfiles[]{
    {{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},{0.014509804546833038f,0.0f,0.014117647893726826f},
        100,0.10000000149011612f,0.3549019694328308f,2.2352941036224365f,2.43137264251709f,192,186,130,156,35},
    {{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},{0.02392156794667244f,0.0f,0.01607843115925789f},
        100,0.05000000074505806f,0.12156862765550613f,5.333333492279053f,3.1372549533843994f,248,240,161,122,62}
};
}
// @verified - p: InitializeWarehouseFallingLeaves.
void InitializeWarehouseFallingLeaves(std::uint32_t *effect) {
    const Vector3 position{332.8f, 130.34f, 231.26f}, volume{1.3f, 0.02f, 1.3f};
    const Vector3 velocity{0.0f, -0.03f, 0.0f}, variance{0.002f, -0.005f, 0.002f};
    *effect = static_cast<std::uint32_t>(CreateFallingParticleEffect(position, volume, 50, 0.5f, shen_name{0x400}, "misc", "REDLEAF.pvr", 126.4f));
    SetParticleEffectVelocity(3, *effect, velocity, variance);
    SetParticleEffectSize(3, *effect, 0.03f, 0.09f);
    ResumeParticleEffect(3, *effect);
}
// @verified - p
void CreateEventSmokeAtCharacter(EventSmokeWork *work) {
    Vector3 position{};
    EV_GetCharaPosition(work->character, work->bone, &position, EventTransformFlags::World);
    const SmokeProfile &profile = smokeProfiles[work->profile];
    work->effect = static_cast<std::uint32_t>(CreateSmokeEffect(position, profile.volume, profile.count, profile.birthFraction, shen_name{0}, 0));
    SetParticleEffectVelocity(0, work->effect, profile.velocity, profile.variance);
    SetParticleEffectSize(0, work->effect, profile.width, profile.height);
    SetParticleEffectColour(
        0, work->effect, 0, static_cast<std::uint8_t>(profile.red), static_cast<std::uint8_t>(profile.green), static_cast<std::uint8_t>(profile.blue));
    SetParticleEffectInitialScale(0, work->effect, profile.initialScale);
    SetSmokeEffectFalloff(work->effect, profile.falloff);
    SetSmokeEffectFadeFactor(work->effect, profile.fade);
    ResumeParticleEffect(0, work->effect);
    ++work->state;
    if (work->startDelay > 0)
        HLib::SetupTaskDelay(work->startDelay);
}
// @verified - p
void EventSmokeCallback(void *parameter) {
    EventSmokeWork *const work = static_cast<EventSmokeWork *>(parameter);
    if (work->state == 0) {
        CreateEventSmokeAtCharacter(work);
        return;
    }
    if (work->state == 1) {
        const std::int32_t remaining = work->remaining--;
        if (remaining <= 0) {
            work->birthFraction = 0.0f;
            SetParticleEffectBirthRate(0, work->effect, 0.0f);
            ++work->state;
            if (work->finishDelay > 0)
                HLib::SetupTaskDelay(work->finishDelay);
        } else {
            work->birthFraction -= work->birthStep;
            SetParticleEffectBirthRate(0, work->effect, work->birthFraction);
        }
    } else if (work->state == 2) {
        FreeParticleEffect(0, work->effect);
        HLib::DestroyCurrentTask();
    }
}
} // namespace shenmue::s2
