#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_smoke.hpp"

typedef struct SmokeProfile {
    Vector3 volume, velocity, variance;
    int count;
    float birthFraction, initialScale, width, height;
    int red, green, blue, falloff, fade;
} SmokeProfile;

const SmokeProfile smokeProfiles[] = {
    {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.014509805f, 0.0f, 0.014117648f}, 100, 0.1f, 0.35490197f, 2.235294f, 2.4313726f, 192, 186, 130, 156, 35},
    {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.023921568f, 0.0f, 0.016078431f}, 100, 0.05f, 0.12156863f, 5.3333335f, 3.137255f, 248, 240, 161, 122, 62},
    {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.007058824f, 0.0f, 0.0050980393f}, 100, 0.1f, 0.29607844f, 0.74509805f, 1.372549f, 255, 255, 255, 255, 64},
    {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.007058824f, 0.0f, 0.0050980393f}, 100, 0.1f, 0.29411766f, 0.74509805f, 1.372549f, 255, 254, 176, 255, 64}};

void QueueEventSmoke(int profile, shen_name character, unsigned int bone, int delay, int startDelay, int remaining, int finishDelay)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EventSmokeCallback, 6, 11, sizeof(EventSmokeWork), TASK_EVNT);
    EventSmokeWork *work;
    if (delay > 0)
        FTBL.HLib_SetupTimedTask(task, delay);
    work = (EventSmokeWork *)FTBL.HLib_GetTaskParameter(task);
    work->profile = profile;
    work->character = character;
    work->bone = bone;
    work->startDelay = startDelay;
    work->remaining = remaining;
    work->finishDelay = finishDelay;
    work->state = 0;
    work->birthFraction = smokeProfiles[profile].birthFraction;
    if (remaining <= 0)
        work->birthStep = work->birthFraction;
    else
        work->birthStep = work->birthFraction / remaining;
}

void EventSmokeCallback(void *data)
{
    EventSmokeWork *work = (EventSmokeWork *)data;
    switch (work->state) {
    case 0:
        CreateEventSmokeAtCharacter(work);
        break;
    case 1:
        FadeEventSmoke(work);
        break;
    case 2:
        FreeEventSmoke(work);
        break;
    }
}

void CreateEventSmokeAtCharacter(EventSmokeWork *work)
{
    Vector3 position;
    int profile = work->profile;
    if (work->bone == -1)
        FTBL.GetCharaPosition(work->character, -1, &position, 0x40000000);
    else
        FTBL.GetCharaPosition(work->character, work->bone, &position, 0x40000000);
    work->effect = FTBL.CreateSmokeEffect(&position, &smokeProfiles[profile].volume, smokeProfiles[profile].count, smokeProfiles[profile].birthFraction, 0, 0);
    FTBL.SetParticleEffectVelocity(0, work->effect, &smokeProfiles[profile].velocity, &smokeProfiles[profile].variance);
    FTBL.SetParticleEffectSize(0, work->effect, smokeProfiles[profile].width, smokeProfiles[profile].height);
    FTBL.SetParticleEffectColour(0, work->effect, 0, smokeProfiles[profile].red, smokeProfiles[profile].green, smokeProfiles[profile].blue);
    FTBL.SetParticleEffectInitialScale(0, work->effect, smokeProfiles[profile].initialScale);
    FTBL.SetSmokeEffectFalloff(work->effect, smokeProfiles[profile].falloff);
    FTBL.SetSmokeEffectFadeFactor(work->effect, smokeProfiles[profile].fade);
    FTBL.ResumeParticleEffect(0, work->effect);
    work->state++;
    if (work->startDelay > 0)
        FTBL.HLib_SetupTaskDelay(work->startDelay);
}

void FadeEventSmoke(EventSmokeWork *work)
{
    if (work->remaining-- > 0) {
        work->birthFraction -= work->birthStep;
        FTBL.SetParticleEffectBirthRate(0, work->effect, work->birthFraction);
    } else {
        work->birthFraction = 0.0f;
        FTBL.SetParticleEffectBirthRate(0, work->effect, 0.0f);
        work->state++;
        if (work->finishDelay > 0)
            FTBL.HLib_SetupTaskDelay(work->finishDelay);
    }
}

void FreeEventSmoke(EventSmokeWork *work)
{
    FTBL.FreeParticleEffect(0, work->effect);
    FTBL.HLib_DestroyCurrentTask();
}
#else
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
#endif
