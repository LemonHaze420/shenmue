#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_duck.hpp"

static unsigned int duckRaceDust[DUCK_RACER_COUNT];

void CreateDuckRaceDust(void)
{
    Vector3 position = {0.0f, 0.0f, 0.0f};
    Vector3 volume;
    int i;
    volume.x = 0.7f;
    volume.y = 0.35f;
    volume.z = 0.7f;
    for (i = 0; i < DUCK_RACER_COUNT; i++)
        duckRaceDust[i] = FTBL.CreateFallingParticleEffect(&position, &volume, 100, 0.01f, 0, "misc", "WING.pvr", 100.0f);
    FTBL.DebugLogToFileRemoved("CreateInitialize Leaf\n");
}

void UpdateDuckRaceDust(int actor, float speed)
{
    Vector3 position = {0.0f, 0.0f, 0.0f};
    Vector3 volume, velocity, variance;
    float rate;
    volume.x = 0.15f;
    volume.y = 0.1f;
    volume.z = 0.15f;
    velocity.x = 0.05f;
    velocity.y = 0.04f;
    velocity.z = 0.05f;
    variance.x = 0.004f;
    variance.y = 0.005f;
    variance.z = 0.004f;
    if (0.09259259f > speed)
        rate = 0.0f;
    else
        rate = (speed - 0.09259259f) / 15.0f;
    position.y += 0.15f;
    FTBL.GetCharaPosition(duckRaceRoster[actor], -1, &position, 0x40000000);
    FTBL.SetParticleEffectPosition(3, duckRaceDust[actor], &position, &volume);
    FTBL.SetParticleEffectVelocity(3, duckRaceDust[actor], &velocity, &variance);
    FTBL.SetParticleEffectBirthRate(3, duckRaceDust[actor], rate);
    FTBL.SetParticleEffectSize(3, duckRaceDust[actor], 0.03f, 0.08f);
    FTBL.ResumeParticleEffect(3, duckRaceDust[actor]);
}

void DuckEffectNullsub(void)
{
}

void DuckEffectUnusedNullsub(void)
{
}
#else
#endif
