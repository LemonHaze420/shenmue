#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_ytqb.hpp"

Vector3 ytqbSplashVolume = {0.1f, 0.0f, 0.1f};
Vector3 ytqbFineSplashVolume = {0.05f, 0.0f, 0.05f};
Vector3 ytqbSplashVelocity = {0.0f, 0.1f, 0.0f};
Vector3 ytqbFineSplashVelocity = {0.05f, 0.03f, 0.0f};
Vector3 ytqbSplashVariance = {0.5f, 0.5f, 0.5f};
Vector3 ytqbFineSplashVariance = {0.02f, 0.02f, 0.02f};
Vector3 ytqbSplashTrailVolume = {0.1f, 0.0f, 0.1f};
Vector3 ytqbFineSplashTrailVolume = {0.05f, 0.0f, 0.05f};
Vector3 ytqbSplashMovementVelocity = {0.05f, 0.03f, 0.0f};
Vector3 ytqbFineSplashTrailVariance = {0.02f, 0.02f, 0.02f};
Vector3 ytqbAmbientPositions[] = {{179.0f, 103.1f, 198.0f}, {182.4f, 103.1f, 190.2f}, {186.5f, 102.9f, 185.4f}, {188.5f, 102.9f, 179.2f}};
int ytqbAmbientSmoke = -1;

const int ytqbLanternParts[] = {175, 170, 152, 163, 158, 172, 167, 174, 169, 155, 162, 157, 166, 154, 173, 168, 153, 165, 160, 176, 171, 164, 159, 161, 156,
    180, 181, 182, 177, 178, 179};

void YtqbSetupLantern(shen_name lantern, shen_name broken)
{
    EVT_AT_SetPos(lantern, YTQB_INVALID, 193.9f, 102.25f, 175.0f, 0);
    EVT_AT_SetAngY(lantern, YTQB_INVALID, 30.0f, 0);
    FTBL.SetScenarioCharacterDrawControl(lantern, 0, 0, 0.0f);
    Queue_EVT_AT_PlayDynamix(lantern, "LDYN", 2.0f, 4.0f, 31, ytqbLanternParts, 2.0f, 0);
}

int YtqbBreakLantern(shen_name lantern, shen_name broken, int delay)
{
    if (lantern != YTQB_INVALID)
        EVT_AT_CharaDisp(lantern, delay, 0);
    if (broken != YTQB_INVALID)
        EVT_AT_CharaDisp(broken, delay, 1);
    EVT_AT_SetSound(2557, delay + -10.0f);
    Queue_EVT_AT_PlayDynamix(lantern, "LDYN", 40.0f, 172.0f, 31, ytqbLanternParts, 3.0f, delay);
    return delay + 44.0f;
}

const int ytqbBucketParts[] = {152, 153, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177,
    178, 179, 180, 181, 182, 183, 185, 184, 186, 187, 188, 189};

void YtqbSetupBucket(shen_name bucket, shen_name broken)
{
    if (bucket != YTQB_INVALID) {
        EVT_AT_CharaDisp(bucket, 0, 1);
        FTBL.SetScenarioCharacterDrawControl(bucket, 0, 0, 0.0f);
        EVT_AT_SetPos(bucket, YTQB_INVALID, 191.4f, 102.25f, 167.5f, 0);
        EVT_AT_SetAngY(bucket, YTQB_INVALID, 285.0f, 0);
    }
    if (broken != YTQB_INVALID) {
        EVT_AT_CharaDisp(broken, 0, 0);
        FTBL.SetScenarioCharacterDrawControl(broken, 0, 0, 0.0f);
        EVT_AT_SetPos(broken, YTQB_INVALID, 191.4f, 102.25f, 167.5f, 0);
        EVT_AT_SetAngY(broken, YTQB_INVALID, 285.0f, 0);
    }
}

int YtqbBreakBucket(shen_name bucket, shen_name broken, int delay)
{
    if (bucket != YTQB_INVALID)
        EVT_AT_CharaDisp(bucket, delay, 0);
    if (broken != YTQB_INVALID)
        EVT_AT_CharaDisp(broken, delay, 1);
    EVT_AT_SetSound(643, delay + 8.0f);
    QueueEventSmoke(0, bucket, -1, delay + 8.0f, 4, 0, 108);
    Queue_EVT_AT_PlayDynamix(bucket, "KADY", 2.0f, 100.0f, 37, ytqbBucketParts, 1.0f, delay);
    return delay + 98.0f;
}

void YtqbResetWater(shen_name water, shen_name spray)
{
    if (water != YTQB_INVALID) {
        EVT_AT_CharaDisp(water, 0, 1);
        FTBL.SetScenarioCharacterDrawControl(water, 0, 0, 0.0f);
        FTBL.SetScenarioCharacterRadius(water, 100.0f);
        EVT_AT_SetPos(water, YTQB_INVALID, 189.7f, 101.55f, 178.5f, 0);
        EVT_AT_SetAng(water, YTQB_INVALID, 0.0f, 325.0f, 330.0f, 0);
    }
    if (spray != YTQB_INVALID) {
        EVT_AT_CharaDisp(spray, 0, 1);
        FTBL.SetScenarioCharacterDrawControl(spray, 0, 0, 0.0f);
        EVT_AT_SetPos(spray, YTQB_INVALID, 189.7f, 101.55f, 178.5f, 0);
        EVT_AT_SetAng(spray, YTQB_INVALID, 0.0f, 325.0f, 330.0f, 0);
    }
}

const int ytqbWaterParts[] = {152, 158, 155, 157, 154, 153, 160, 159, 156, 161};

int QueueYtqbWaterSplash(shen_name water, shen_name spray, int delay)
{
    if (water != YTQB_INVALID)
        EVT_AT_CharaDisp(water, delay, 0);
    if (spray != YTQB_INVALID)
        EVT_AT_CharaDisp(spray, delay, 1);
    Queue_EVT_AT_PlayDynamix(water, "SUDY", 2.0f, 140.0f, 10, ytqbWaterParts, 2.0f, delay);
    BeginYtqbSplash(YTQB_RYO, 80, delay);
    return delay + 69.0f;
}

void YtqbSetupWater(shen_name water, shen_name spray)
{
    if (water != YTQB_INVALID) {
        EVT_AT_CharaDisp(water, 0, 1);
        FTBL.SetScenarioCharacterDrawControl(water, 0, 0, 0.0f);
        FTBL.SetScenarioCharacterRadius(water, 100.0f);
        EVT_AT_SetPos(water, YTQB_INVALID, 191.05f, 102.28f, 178.15f, 0);
        EVT_AT_SetAng(water, YTQB_INVALID, 0.0f, 147.68f, 0.0f, 0);
    }
    if (spray != YTQB_INVALID) {
        EVT_AT_CharaDisp(spray, 0, 1);
        FTBL.SetScenarioCharacterDrawControl(spray, 0, 0, 0.0f);
        EVT_AT_SetPos(water, YTQB_INVALID, 191.05f, 102.28f, 178.15f, 0);
        EVT_AT_SetAng(water, YTQB_INVALID, 0.0f, 147.68f, 0.0f, 0);
    }
}

const int ytqbStillWaterParts[] = {152, 158, 155, 157, 154, 153, 160, 159, 156, 161};

int YtqbSpillWater(shen_name water, shen_name spray, int delay)
{
    if (water != YTQB_INVALID)
        EVT_AT_CharaDisp(water, delay, 0);
    if (spray != YTQB_INVALID)
        EVT_AT_CharaDisp(spray, delay, 1);
    Queue_EVT_AT_PlayDynamix(water, "SNDY", 2.0f, 492.0f, 10, ytqbStillWaterParts, 2.0f, delay);
    return delay + 245.0f;
}

const float ytqbSplashStrength = 4.0f;

int BeginYtqbSplash(shen_name character, int duration, int delay)
{
    float lifetime = duration;
    Vector3 position;
    YtqbSplashWork *work;
    FTBL.GetCharaPosition(character, 14, &position, 0x40000000);
    work = (YtqbSplashWork *)EVT_RepeatCallData((EventRepeatDataFunction)YtqbSplashUpdate, sizeof(YtqbSplashWork), delay, duration);
    work->character = character;
    work->strength = 4.0f;
    work->previousPosition = position;
    work->coarse = FTBL.CreateSplashEffect(&position, &ytqbSplashVolume, 300, 0.0f, 0, lifetime, 0, 0);
    FTBL.SetParticleEffectColour(1, work->coarse, 255, 255, 0, 16);
    FTBL.SetParticleEffectVelocity(1, work->coarse, &ytqbSplashVelocity, &ytqbSplashVariance);
    FTBL.SetParticleEffectSize(1, work->coarse, 0.0f, 0.0f);
    FTBL.ResumeParticleEffect(1, work->coarse);
    work->fine = FTBL.CreateSplashEffect(&position, &ytqbFineSplashVolume, 1000, 0.0f, 0, lifetime, 0, 0);
    FTBL.SetParticleEffectColour(1, work->fine, 255, 255, 32, 32);
    FTBL.SetParticleEffectVelocity(1, work->fine, &ytqbFineSplashVelocity, &ytqbFineSplashVariance);
    FTBL.SetParticleEffectSize(1, work->fine, 0.0f, 0.0f);
    FTBL.ResumeParticleEffect(1, work->fine);
    return delay + duration;
}

void YtqbSplashUpdate(YtqbSplashWork *work, int index)
{
    Vector3 camera, position;
    float distance, moved;
    if (work->coarse == -1)
        return;
    FTBL.GetCharaPosition(work->character, 14, &position, 0x40000000);
    FTBL.camCtrlGetNextCamPos(&camera);
    distance = FTBL.GetEuclideanDistance(&position, &camera);
    moved = FTBL.CalculatePlanarDistanceBetweenPoints(&work->previousPosition, &position);
    FTBL.SetParticleEffectPosition(1, work->coarse, &position, &ytqbSplashTrailVolume);
    FTBL.SetParticleEffectBirthRate(1, work->coarse, moved * moved * 0.3f * work->strength);
    ytqbSplashMovementVelocity.x = (position.x - work->previousPosition.x) * 0.5f;
    ytqbSplashMovementVelocity.y = 0.03f;
    ytqbSplashMovementVelocity.z = (position.z - work->previousPosition.z) * 0.5f;
    FTBL.SetParticleEffectPosition(1, work->fine, &position, &ytqbFineSplashTrailVolume);
    FTBL.SetParticleEffectVelocity(1, work->fine, &ytqbSplashMovementVelocity, &ytqbFineSplashTrailVariance);
    FTBL.SetParticleEffectBirthRate(1, work->fine, 0.1f);
    work->previousPosition = position;
    if (index > 19) {
        if (index <= 24) {
            FTBL.SetParticleEffectSize(1, work->coarse, distance * 0.005f, distance * 0.005f);
            FTBL.SetParticleEffectSize(1, work->fine, distance * 0.007f, distance * 0.007f);
        } else if (index <= 69) {
            FTBL.SetParticleEffectBirthRate(1, work->coarse, 0.0f);
            FTBL.SetParticleEffectBirthRate(1, work->fine, 0.0f);
        } else {
            FTBL.FreeParticleEffect(1, work->coarse);
            FTBL.FreeParticleEffect(1, work->fine);
            work->coarse = -1;
            work->fine = -1;
        }
    }
}

void YtqbSetupFrontStall(shen_name stall, shen_name broken)
{
    if (stall != YTQB_INVALID)
        EVT_AT_CharaDisp(stall, 0, 1);
    if (broken != YTQB_INVALID)
        EVT_AT_CharaDisp(broken, 0, 0);
    FTBL.SetScenarioCharacterDrawControl(stall, 0, 0, 0.0f);
    FTBL.SetScenarioCharacterDrawControl(broken, 0, 0, 0.0f);
}

const int ytqbFrontStallParts[] = {173, 172, 171, 170, 169, 168, 167, 174, 180, 179, 178, 177, 176, 175, 166, 158, 157, 156, 155, 154, 153, 152, 159, 165, 164,
    163, 162, 161, 160, 181, 202, 201, 200, 199, 198, 197, 203, 209, 208, 207, 206, 205, 204, 196, 188, 187, 186, 185, 184, 183, 182, 189, 195, 194, 193, 192,
    191, 190};

int YtqbBreakFrontStall(shen_name stall, shen_name broken, int delay)
{
    if (stall != YTQB_INVALID)
        EVT_AT_CharaDisp(stall, delay, 0);
    if (broken != YTQB_INVALID)
        EVT_AT_CharaDisp(broken, delay, 1);
    EVT_AT_SetSound(647, delay + 4.0f);
    QueueEventSmoke(1, stall, -1, delay + 4.0f, 4, 0, 108);
    Queue_EVT_AT_PlayDynamix(stall, "FDYN", 2.0f, 504.0f, 58, ytqbFrontStallParts, 2.0f, delay);
    return delay + 251.0f;
}

void YtqbSetupNearStall(shen_name stall, shen_name broken)
{
    if (stall != YTQB_INVALID)
        EVT_AT_CharaDisp(stall, 0, 1);
    if (broken != YTQB_INVALID)
        EVT_AT_CharaDisp(broken, 0, 0);
    FTBL.SetScenarioCharacterDrawControl(stall, 0, 0, 0.0f);
    FTBL.SetScenarioCharacterDrawControl(broken, 0, 0, 0.0f);
}

const int ytqbNearStallParts[] = {165, 164, 163, 167, 171, 170, 169, 168, 166, 172, 180, 179, 178, 177, 173, 174, 175, 176, 162, 156, 155, 154, 153, 152, 161,
    160, 159, 157, 158};

int YtqbBreakNearStall(shen_name stall, shen_name broken, int delay)
{
    if (stall != YTQB_INVALID)
        EVT_AT_CharaDisp(stall, delay, 0);
    if (broken != YTQB_INVALID)
        EVT_AT_CharaDisp(broken, delay, 1);
    EVT_AT_SetSound(647, delay + 4.0f);
    QueueEventSmoke(1, stall, -1, delay + 4.0f, 4, 0, 130);
    Queue_EVT_AT_PlayDynamix(stall, "NDYN", 2.0f, 270.0f, 29, ytqbNearStallParts, 2.0f, delay);
    return delay + 134.0f;
}

void YtqbSetupStallObjects(shen_name stall, shen_name broken)
{
    if (stall != YTQB_INVALID)
        EVT_AT_CharaDisp(stall, 0, 1);
    if (broken != YTQB_INVALID)
        EVT_AT_CharaDisp(broken, 0, 0);
    FTBL.SetScenarioCharacterDrawControl(stall, 0, 0, 0.0f);
    FTBL.SetScenarioCharacterDrawControl(broken, 0, 0, 0.0f);
    FTBL.SetScenarioCharacterDrawControl(YTQB_NIW0, 0, 0, 0.0f);
    FTBL.SetScenarioCharacterDrawControl(YTQB_NIW1, 0, 0, 0.0f);
    FTBL.SetScenarioCharacterDrawControl(YTQB_NIW2, 0, 0, 0.0f);
    FTBL.AT_SetLocalBaseChara(YTQB_NIW0, stall);
    FTBL.AT_SetLocalBaseChara(YTQB_NIW1, stall);
    FTBL.AT_SetLocalBaseChara(YTQB_NIW2, stall);
    FTBL.AT_SetLocalBaseChara(YTQB_AHI0, stall);
    FTBL.AT_SetLocalBaseChara(YTQB_AHI1, stall);
    EVT_AT_SetPos(YTQB_NIW0, stall, -2.45f, 1.55f, 0.04f, 0);
    EVT_AT_SetAngY(YTQB_NIW0, stall, 180.0f, 0);
    EVT_AT_SetPos(YTQB_NIW1, stall, -1.89f, 1.55f, 0.04f, 0);
    EVT_AT_SetAngY(YTQB_NIW1, stall, 180.0f, 0);
    EVT_AT_SetPos(YTQB_NIW2, stall, -1.08f, 1.55f, 0.04f, 0);
    EVT_AT_SetAngY(YTQB_NIW2, stall, 180.0f, 0);
    EVT_AT_SetPos(YTQB_AHI0, stall, -2.05f, 0.2f, -0.53f, 0);
    EVT_AT_SetAngY(YTQB_AHI0, stall, 180.0f, 0);
    EVT_AT_SetPos(YTQB_AHI1, stall, -2.45f, 0.2f, -0.53f, 0);
    EVT_AT_SetAngY(YTQB_AHI1, stall, 180.0f, 0);
    FTBL.ConfigureScenarioNodeMotion(YTQB_NIW0, 5724, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
    FTBL.ConfigureScenarioNodeMotion(YTQB_NIW0, 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
    FTBL.ConfigureScenarioNodeMotion(YTQB_NIW0, 0, -1.0f, -1.0f, -1.0f, 1.0f, -1);
    FTBL.ConfigureScenarioNodeMotion(YTQB_NIW1, 5724, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
    FTBL.ConfigureScenarioNodeMotion(YTQB_NIW1, 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
    FTBL.ConfigureScenarioNodeMotion(YTQB_NIW1, 0, -1.0f, -1.0f, -1.0f, 0.9f, -1);
    FTBL.ConfigureScenarioNodeMotion(YTQB_NIW2, 5724, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
    FTBL.ConfigureScenarioNodeMotion(YTQB_NIW2, 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
    FTBL.ConfigureScenarioNodeMotion(YTQB_NIW2, 0, -1.0f, -1.0f, -1.0f, 1.2f, -1);
    FTBL.ConfigureScenarioNodeMotion(YTQB_AHI0, 5722, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
    FTBL.ConfigureScenarioNodeMotion(YTQB_AHI0, 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
    FTBL.ConfigureScenarioNodeMotion(YTQB_AHI0, 0, -1.0f, -1.0f, -1.0f, 1.0f, -1);
    FTBL.ConfigureScenarioNodeMotion(YTQB_AHI1, 5723, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
    FTBL.ConfigureScenarioNodeMotion(YTQB_AHI1, 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
    FTBL.ConfigureScenarioNodeMotion(YTQB_AHI1, 0, -1.0f, -1.0f, -1.0f, 1.0f, -1);
}

const int ytqbBirdStallParts[] = {172, 178, 162, 183, 158, 167, 168, 170, 171, 173, 175, 177, 180, 184, 190, 181, 161, 194, 152, 157, 153, 160, 159, 156, 155,
    154, 163, 176, 179, 169, 164, 174, 195, 193, 199, 192, 198, 191, 166, 165, 187, 186, 197, 185, 182, 196, 188, 189};

int YtqbBreakBirdStall(shen_name stall, shen_name broken, int delay)
{
    if (stall != YTQB_INVALID)
        EVT_AT_CharaDisp(stall, delay, 0);
    if (broken != YTQB_INVALID)
        EVT_AT_CharaDisp(broken, delay, 1);
    QueueYtqbBirdStallUpdate(delay + 18);
    QueueYtqbPaperStallUpdate(delay);
    EVT_AT_SetSound(648, delay + 4.0f);
    EVT_AT_SetSound(644, delay + 18.0f);
    EVT_AT_SetSound(645, delay + 26.5f);
    EVT_AT_SetSound(2558, delay + 49.0f);
    EVT_AT_SetSound(2559, delay + 49.0f);
    EVT_AT_SetSound(2560, delay + 49.0f);
    QueueEventSmoke(0, stall, -1, delay + 4.0f, 4, 0, 108);
    Queue_EVT_AT_PlayDynamix(stall, "TDYN", 2.0f, 374.0f, 48, ytqbBirdStallParts, 2.0f, delay);
    return delay + 186.0f;
}

int QueueYtqbBirdStallUpdate(int delay)
{
    EVT_RepeatCallData(YtqbBirdStallUpdate, 0, delay, 150);
    return delay + 150;
}

void YtqbBirdStallUpdate(void *data, int ticks)
{
    if (data)
        return;
    if (!ticks) {
        FTBL.ConfigureScenarioNodeMotion(YTQB_AHI0, 5721, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        FTBL.ConfigureScenarioNodeMotion(YTQB_AHI0, 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
        FTBL.ConfigureScenarioNodeMotion(YTQB_AHI0, 0, -1.0f, -1.0f, -1.0f, 1.6f, -1);
        FTBL.ConfigureScenarioNodeMotion(YTQB_AHI1, 5721, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        FTBL.ConfigureScenarioNodeMotion(YTQB_AHI1, 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
        FTBL.ConfigureScenarioNodeMotion(YTQB_AHI1, 0, -1.0f, -1.0f, -1.0f, 2.1f, -1);
    } else {
        EVT_AT_SetPosAng(YTQB_AHI0, YTQB_AHI0, 0.0f, -0.08f, 6.0f, 0);
        EVT_AT_SetPosAng(YTQB_AHI1, YTQB_AHI1, 0.0f, -0.1f, -2.0f, 0);
    }
}

int QueueYtqbPaperStallUpdate(int delay)
{
    EVT_RepeatCallData(YtqbPaperStallUpdate, 0, delay, 120);
    return delay + 120;
}

void YtqbPaperStallUpdate(void *data, int ticks)
{
    Vector3 position;
    if (data)
        return;
    if (!ticks) {
        FTBL.ConfigureScenarioNodeMotion(YTQB_NIW0, 0, -1.0f, -1.0f, -1.0f, 2.0f, -1);
        FTBL.ConfigureScenarioNodeMotion(YTQB_NIW1, 0, -1.0f, -1.0f, -1.0f, 1.3f, -1);
        FTBL.ConfigureScenarioNodeMotion(YTQB_NIW2, 0, -1.0f, -1.0f, -1.0f, 1.7f, -1);
    } else {
        FTBL.scenApplyPosOrRotToChildId(YTQB_YAD1, 188, &position, 0, 2);
        EVT_AT_SetPos(YTQB_NIW0, YTQB_YAD1, position.x, position.y - 0.2f, position.z, 0);
        FTBL.scenApplyPosOrRotToChildId(YTQB_YAD1, 172, &position, 0, 2);
        EVT_AT_SetPos(YTQB_NIW1, YTQB_YAD1, position.x, position.y - 0.2f, position.z, 0);
        FTBL.scenApplyPosOrRotToChildId(YTQB_YAD1, 181, &position, 0, 2);
        EVT_AT_SetPos(YTQB_NIW2, YTQB_YAD1, position.x, position.y - 0.2f, position.z, 0);
    }
}

int QueueYtqbAmbientSmoke(int delay)
{
    EVT_RepeatCallData(YtqbAmbientSmoke, 0, delay, 200000000);
    return delay + 200000000;
}

void YtqbAmbientSmoke(void *data, int ticks)
{
    Vector3 volume;
    int *smoke;
    volume.x = 0.005f;
    volume.y = 0.0f;
    volume.z = 0.005f;
    if (data)
        return;
    switch (ticks) {
    case 0:
        smoke = &ytqbAmbientSmoke;
        *smoke = FTBL.CreateSmokeEffect(&ytqbAmbientPositions[0], &volume, 15, 0.2f, 0, 0);
        FTBL.SetSmokeEffectFadeFactor(*smoke, 64);
        FTBL.SetParticleEffectColour(0, *smoke, 0, 108, 100, 82);
        FTBL.ResumeParticleEffect(0, *smoke);
        break;
    case 29900000:
        FTBL.SuspendParticleEffect(0, ytqbAmbientSmoke);
        FTBL.FreeParticleEffect(0, ytqbAmbientSmoke);
        ytqbAmbientSmoke = -1;
        break;
    default:
        if (ticks < 29000000)
            FTBL.SetParticleEffectPosition(0, ytqbAmbientSmoke, &ytqbAmbientPositions[(unsigned int)ticks >> 3 & 3], &volume);
        break;
    }
}
#else
#include "s2/areas/area_ytqb.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_smoke.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/game_runtime.hpp"
#include "s2/motion.hpp"
#include "s2/particle_effects.hpp"
#include "s2/step_event.hpp"
#include <bit>
#include <cmath>

namespace shenmue::s2::areas {
namespace {
inline constexpr shen_name BrokenBucket{YTQB_KBK0.name | 0x1000000U};
inline constexpr shen_name BrokenBirdStall{YTQB_YAD1.name | 0x100000U};
inline constexpr shen_name BrokenFrontStall{YTQB_YAD2.name | 0x100000U};
inline constexpr shen_name BrokenNearStall{YTQB_YAD3.name | 0x100000U};
inline constexpr shen_name StallBirds[]{shen_name{YTQB_BIR0.name | 0x5000cU}, shen_name{YTQB_BIR1.name | 0x5000cU}, shen_name{YTQB_BIR2.name | 0x5000cU}};
inline constexpr shen_name StallPapers[]{YTQB_NIW0, YTQB_NIW1, YTQB_NIW2};
inline constexpr std::uint16_t PaperParts[]{0xbc, 0xac, 0xb5};
inline constexpr std::uint32_t BirdIdleMotion = 5724, FirstBirdMotion = 5722, SecondBirdMotion = 5723, BirdEscapeMotion = 5721;
inline constexpr std::uint32_t WaterFailureLeadMotion = 0xf002;
inline constexpr Vector3 SplashVolume{.x = .1f, .y = 0, .z = .1f}, FineSplashVolume{.x = .05f, .y = 0, .z = .05f};
inline constexpr Vector3 SplashVelocity{.x = 0, .y = .1f, .z = 0}, SplashVariance{.x = .5f, .y = .5f, .z = .5f};
inline constexpr Vector3 FineSplashVelocity{.x = .05f, .y = .03f, .z = 0}, FineSplashVariance{.x = .02f, .y = .02f, .z = .02f};
inline constexpr Vector3 AmbientPositions[]{{.x = 179, .y = 103.0999984741211f, .z = 198},
    {.x = 182.39999389648438f, .y = 103.0999984741211f, .z = 190.1999969482422f}, {.x = 186.5f, .y = 102.9000015258789f, .z = 185.39999389648438f},
    {.x = 188.5f, .y = 102.9000015258789f, .z = 179.1999969482422f}};
Vector3 splashMovementVelocity{.x = .05f, .y = .03f, .z = 0};
struct SplashWork {
    std::int16_t coarse, fine;
    Vector3 previousPosition;
    float strength;
    shen_name character;
};
struct SplashTaskWork {
    EventTickFunctionWork call;
    SplashWork splash;
};
static_assert(sizeof(SplashWork) == 0x18 && offsetof(SplashWork, previousPosition) == 4 && offsetof(SplashWork, strength) == 0x10);
static_assert(sizeof(SplashTaskWork) == 0x38 && offsetof(SplashTaskWork, splash) == 0x20);
void QueueSmoke(shen_name character, std::int32_t delay, std::int32_t finish, std::int32_t profile, float birth) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EventSmokeCallback, 6, 11, sizeof(EventSmokeWork), YTQB_EVNT);
    HLib::SetupTimedTask(task, delay);
    EventSmokeWork *smoke = static_cast<EventSmokeWork *>(HLib::GetTaskParameter(task));
    smoke->state = 0;
    smoke->profile = profile;
    smoke->character = character;
    smoke->bone = 0xffffffffU;
    smoke->startDelay = 4;
    smoke->remaining = 0;
    smoke->finishDelay = finish;
    smoke->birthFraction = birth;
    smoke->birthStep = birth;
}
void QueueTick(EventTickFunction callback, std::int32_t maxTime, std::int32_t delay) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EventTickFunction_Callback, 6, 11, sizeof(EventTickFunctionWork), YTQB_EVNT);
    RegisterEventTask(task);
    EventTickFunctionWork *call = static_cast<EventTickFunctionWork *>(HLib::GetTaskParameter(task));
    call->callback = callback;
    call->maxTime = maxTime;
    call->currentTicks = 0;
    call->paramSize = 0;
    call->delay = static_cast<float>(delay);
}
void SetRawWaterAngle(shen_name character, const Vector3i &rotation) {
    EventSetRawAngle request{.cleanup = 0, .character = character, .parent = YTQB_INVALID, .rotation = rotation, .delay = 0};
    EVT_AT_SetAng_Callback(&request);
}
void InitializeBirdMotion(shen_name character, std::int32_t motion, float speed) {
    ConfigureScenarioNodeMotion(character, motion, 0, 32767, 0, 1, 0);
    ConfigureScenarioNodeMotion(character, 0, -1, -1, -1, 32767, 4);
    ConfigureScenarioNodeMotion(character, 0, -1, -1, -1, speed, -1);
}
void BeginSplash() {
    Vector3 position;
    EV_GetCharaPosition(YTQB_RYO_, 14, &position, EventTransformFlags::World);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EventTickFunction_Callback, 6, 11, sizeof(SplashTaskWork), YTQB_EVNT);
    RegisterEventTask(task);
    SplashTaskWork *work = static_cast<SplashTaskWork *>(HLib::GetTaskParameter(task));
    work->call.callback = YtqbSplashUpdate;
    work->call.maxTime = 80;
    work->call.currentTicks = 0;
    work->call.paramSize = 24;
    work->call.delay = 0;
    work->splash.character = YTQB_RYO_;
    work->splash.strength = 4;
    work->splash.previousPosition = position;
    work->splash.coarse =
        static_cast<std::int16_t>(CreateSplashEffect(position, SplashVolume, 300, 0, shen_name{0}, std::bit_cast<std::int32_t>(80.0f), nullptr, nullptr));
    SetParticleEffectColour(1, work->splash.coarse, 255, 255, 0, 16);
    SetParticleEffectVelocity(1, work->splash.coarse, SplashVelocity, SplashVariance);
    SetParticleEffectSize(1, work->splash.coarse, 0, 0);
    ResumeParticleEffect(1, work->splash.coarse);
    work->splash.fine =
        static_cast<std::int16_t>(CreateSplashEffect(position, FineSplashVolume, 1000, 0, shen_name{0}, std::bit_cast<std::int32_t>(80.0f), nullptr, nullptr));
    SetParticleEffectColour(1, work->splash.fine, 255, 255, 32, 32);
    SetParticleEffectVelocity(1, work->splash.fine, FineSplashVelocity, FineSplashVariance);
    SetParticleEffectSize(1, work->splash.fine, 0, 0);
    ResumeParticleEffect(1, work->splash.fine);
}
void AnimateWater() {
    EVT_AT_ObjectDisp_1(YTQB_SUI0, 0, 0);
    EVT_AT_ObjectDisp_1(YTQB_SUI1, 0, 1);
    Queue_EVT_AT_PlayDynamix(YTQB_SUI0, "SUDY", 2, 140, 10, YTQB_WaterParts, 2, 0);
    BeginSplash();
}
void HideWaterAfter(std::int32_t delay) {
    if (!delay) {
        EV_DisableCharacterDisplay(YTQB_SUI0);
        return;
    }
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 11, sizeof(StepObjectDisplayWork), YTQB_EVNT);
    RegisterEventTask(task);
    StepObjectDisplayWork *work = static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(task));
    work->cleanup = 1;
    work->character = YTQB_SUI0;
    work->mode = 1;
    work->delay = static_cast<float>(delay);
}
void PositionWaterFight() {
    YtqbPosition(YTQB_RYO_, YTQB_BASE, YTQB_WaterPlayer);
    YtqbAngle(YTQB_RYO_, YTQB_BASE, -45);
    YtqbPosition(YTQB_KIC_, YTQB_RYO_, YTQB_WaterFarOpponent);
    YtqbAngle(YTQB_KIC_, YTQB_RYO_, 180);
}
} // namespace
void YtqbSetupLantern() {
    YtqbPosition(YTQB_CAG0, YTQB_INVALID, YTQB_Lantern);
    YtqbAngle(YTQB_CAG0, YTQB_INVALID, 30);
    SetScenarioCharacterDrawControl(YTQB_CAG0, 0, 0);
    Queue_EVT_AT_PlayDynamix(YTQB_CAG0, "LDYN", 2, 4, 31, YTQB_LanternParts, 2, 0);
}
std::int32_t YtqbBreakLantern() {
    EVT_AT_ObjectDisp_1(YTQB_CAG0, 52, 0);
    EVT_AT_SetSound(0x9fd, 42);
    Queue_EVT_AT_PlayDynamix(YTQB_CAG0, "LDYN", 40, 172, 31, YTQB_LanternParts, 3, 52);
    return 96;
}
void YtqbSetupBucket() {
    EV_DisableCharacterDisplay(YTQB_KBK0);
    SetScenarioCharacterDrawControl(YTQB_KBK0, 0, 0);
    YtqbPosition(YTQB_KBK0, YTQB_INVALID, YTQB_Bucket);
    YtqbAngle(YTQB_KBK0, YTQB_INVALID, 285);
    EV_EnableCharacterDisplay(BrokenBucket);
    SetScenarioCharacterDrawControl(BrokenBucket, 0, 0);
    YtqbPosition(BrokenBucket, YTQB_INVALID, YTQB_Bucket);
    YtqbAngle(BrokenBucket, YTQB_INVALID, 285);
}
std::int32_t YtqbBreakBucket() {
    EVT_AT_ObjectDisp_1(YTQB_KBK0, 32, 0);
    EVT_AT_ObjectDisp_1(BrokenBucket, 32, 1);
    EVT_AT_SetSound(0x283, 40);
    QueueSmoke(YTQB_KBK0, 40, 108, 0, .1f);
    Queue_EVT_AT_PlayDynamix(YTQB_KBK0, "KADY", 2, 100, 37, YTQB_BucketParts, 1, 32);
    return 130;
}
void YtqbResetWater() {
    EV_DisableCharacterDisplay(YTQB_SUI0);
    SetScenarioCharacterDrawControl(YTQB_SUI0, 0, 0);
    SetScenarioCharacterRadius(YTQB_SUI0, 100);
    YtqbPosition(YTQB_SUI0, YTQB_INVALID, YTQB_WaterReset);
    SetRawWaterAngle(YTQB_SUI0, {.x = 0, .y = 59164, .z = 60074});
    EV_DisableCharacterDisplay(YTQB_SUI1);
    SetScenarioCharacterDrawControl(YTQB_SUI1, 0, 0);
    YtqbPosition(YTQB_SUI1, YTQB_INVALID, YTQB_WaterReset);
    SetRawWaterAngle(YTQB_SUI1, {.x = 0, .y = 59164, .z = 60074});
}
void YtqbSetupWater() {
    EV_DisableCharacterDisplay(YTQB_SUI0);
    SetScenarioCharacterDrawControl(YTQB_SUI0, 0, 0);
    SetScenarioCharacterRadius(YTQB_SUI0, 100);
    YtqbPosition(YTQB_SUI0, YTQB_INVALID, YTQB_WaterIdle);
    SetRawWaterAngle(YTQB_SUI0, {.x = 0, .y = 26884, .z = 0});
    EV_DisableCharacterDisplay(YTQB_SUI1);
    SetScenarioCharacterDrawControl(YTQB_SUI1, 0, 0);
    YtqbPosition(YTQB_SUI0, YTQB_INVALID, YTQB_WaterIdle);
    SetRawWaterAngle(YTQB_SUI0, {.x = 0, .y = 26884, .z = 0});
}
std::int32_t YtqbBreakFrontStall() {
    EVT_AT_ObjectDisp_1(YTQB_YAD2, 30, 0);
    EVT_AT_ObjectDisp_1(BrokenFrontStall, 30, 1);
    EVT_AT_SetSound(0x287, 34);
    QueueSmoke(YTQB_YAD2, 34, 108, 1, .05f);
    Queue_EVT_AT_PlayDynamix(YTQB_YAD2, "FDYN", 2, 504, 58, YTQB_FrontStallParts, 2, 30);
    return 281;
}
std::int32_t YtqbBreakNearStall() {
    EVT_AT_ObjectDisp_1(YTQB_YAD3, 20, 0);
    EVT_AT_ObjectDisp_1(BrokenNearStall, 20, 1);
    EVT_AT_SetSound(0x287, 24);
    QueueSmoke(YTQB_YAD3, 24, 130, 1, .05f);
    Queue_EVT_AT_PlayDynamix(YTQB_YAD3, "NDYN", 2, 270, 29, YTQB_NearStallParts, 2, 20);
    return 154;
}
void YtqbSetupStallObjects() {
    EV_DisableCharacterDisplay(YTQB_YAD1);
    EV_EnableCharacterDisplay(BrokenBirdStall);
    SetScenarioCharacterDrawControl(YTQB_YAD1, 0, 0);
    SetScenarioCharacterDrawControl(BrokenBirdStall, 0, 0);
    for (shen_name bird : StallBirds)
        SetScenarioCharacterDrawControl(bird, 0, 0);
    for (shen_name bird : StallBirds)
        AT_SetLocalBaseChara(bird, YTQB_YAD1);
    AT_SetLocalBaseChara(YTQB_AHI0, YTQB_YAD1);
    AT_SetLocalBaseChara(YTQB_AHI1, YTQB_YAD1);
    YtqbPosition(YTQB_NIW0, YTQB_YAD1, YTQB_FirstPaper);
    YtqbAngle(YTQB_NIW0, YTQB_YAD1, 180);
    YtqbPosition(YTQB_NIW1, YTQB_YAD1, YTQB_SecondPaper);
    YtqbAngle(YTQB_NIW1, YTQB_YAD1, 180);
    YtqbPosition(YTQB_NIW2, YTQB_YAD1, YTQB_ThirdPaper);
    YtqbAngle(YTQB_NIW2, YTQB_YAD1, 180);
    YtqbPosition(YTQB_AHI0, YTQB_YAD1, YTQB_FirstBird);
    YtqbAngle(YTQB_AHI0, YTQB_YAD1, 180);
    YtqbPosition(YTQB_AHI1, YTQB_YAD1, YTQB_SecondBird);
    YtqbAngle(YTQB_AHI1, YTQB_YAD1, 180);
    InitializeBirdMotion(StallBirds[0], BirdIdleMotion, 1);
    InitializeBirdMotion(StallBirds[1], BirdIdleMotion, .9f);
    InitializeBirdMotion(StallBirds[2], BirdIdleMotion, 1.2f);
    InitializeBirdMotion(YTQB_AHI0, FirstBirdMotion, 1);
    InitializeBirdMotion(YTQB_AHI1, SecondBirdMotion, 1);
}
std::int32_t YtqbBreakBirdStall() {
    EVT_AT_ObjectDisp_1(YTQB_YAD1, 37, 0);
    EVT_AT_ObjectDisp_1(BrokenBirdStall, 37, 1);
    QueueTick(YtqbBirdStallUpdate, 150, 55);
    QueueTick(YtqbPaperStallUpdate, 120, 37);
    EVT_AT_SetSound(0x288, 41);
    EVT_AT_SetSound(0x284, 55);
    EVT_AT_SetSound(0x285, 63);
    EVT_AT_SetSound(0x9fe, 86);
    EVT_AT_SetSound(0x9ff, 86);
    EVT_AT_SetSound(0xa00, 86);
    QueueSmoke(YTQB_YAD1, 41, 108, 0, .1f);
    Queue_EVT_AT_PlayDynamix(YTQB_YAD1, "TDYN", 2, 374, 48, YTQB_BirdStallParts, 2, 37);
    return 223;
}
void YtqbBirdStallUpdate(void *data, std::int32_t ticks) {
    if (data)
        return;
    if (!ticks) {
        InitializeBirdMotion(YTQB_AHI0, BirdEscapeMotion, 1.6f);
        InitializeBirdMotion(YTQB_AHI1, BirdEscapeMotion, 2.1f);
        return;
    }
    EventSetPositionAngle first{.cleanup = 0, .character = YTQB_AHI0, .parent = YTQB_AHI0, .position = {.x = 0, .y = 0, .z = -.08f}, .degrees = 6, .delay = 0};
    EVT_AT_SetPosAng_Callback(&first);
    EventSetPositionAngle second{.cleanup = 0, .character = YTQB_AHI1, .parent = YTQB_AHI1, .position = {.x = 0, .y = 0, .z = -.1f}, .degrees = -2, .delay = 0};
    EVT_AT_SetPosAng_Callback(&second);
}
void YtqbPaperStallUpdate(void *data, std::int32_t ticks) {
    if (data)
        return;
    if (!ticks) {
        ConfigureScenarioNodeMotion(StallBirds[0], 0, -1, -1, -1, 2, -1);
        ConfigureScenarioNodeMotion(StallBirds[1], 0, -1, -1, -1, 1.3f, -1);
        ConfigureScenarioNodeMotion(StallBirds[2], 0, -1, -1, -1, 1.7f, -1);
        return;
    }
    for (std::int32_t i = 0; i < 3; ++i) {
        Vector3 position;
        EV_scenApplyPosOrRotToPartOf(YTQB_YAD1, PaperParts[i], &position, nullptr, EventCharacterPartTransformMode::Get);
        position.y -= .2f;
        YtqbPosition(StallPapers[i], YTQB_YAD1, position);
    }
}
void YtqbAmbientSmoke(void *data, std::int32_t ticks) {
    if (data)
        return;
    const Vector3 volume{.x = .005f, .y = 0, .z = .005f};
    if (!ticks) {
        ytqbAmbientSmoke = static_cast<std::int32_t>(CreateSmokeEffect(AmbientPositions[0], volume, 15, .2f, shen_name{0}, 0));
        SetSmokeEffectFadeFactor(ytqbAmbientSmoke, 64);
        SetParticleEffectColour(0, ytqbAmbientSmoke, 0, 108, 100, 82);
        ResumeParticleEffect(0, ytqbAmbientSmoke);
    } else if (ticks == 29900000) {
        SuspendParticleEffect(0, ytqbAmbientSmoke);
        FreeParticleEffect(0, ytqbAmbientSmoke);
        ytqbAmbientSmoke = -1;
    } else if (ticks < 29000000)
        SetParticleEffectPosition(0, ytqbAmbientSmoke, &AmbientPositions[(ticks >> 3) & 3], &volume);
}
void YtqbSplashUpdate(void *data, std::int32_t ticks) {
    SplashWork &work = *static_cast<SplashWork *>(data);
    if (work.coarse == -1)
        return;
    Vector3 position, camera;
    EV_GetCharaPosition(work.character, 14, &position, EventTransformFlags::World);
    camCtrlGetNextCamPos(&camera);
    const float x = position.x - camera.x, y = position.y - camera.y, z = position.z - camera.z;
    const float distance = std::sqrt(x * x + y * y + z * z);
    const float dx = work.previousPosition.x - position.x, dz = work.previousPosition.z - position.z;
    const float travel = std::sqrt(dx * dx + dz * dz);
    SetParticleEffectPosition(1, work.coarse, &position, &SplashVolume);
    SetParticleEffectBirthRate(1, work.coarse, travel * travel * .3f * work.strength);
    splashMovementVelocity.x = (position.x - work.previousPosition.x) * .5f;
    splashMovementVelocity.y = .03f;
    splashMovementVelocity.z = (position.z - work.previousPosition.z) * .5f;
    SetParticleEffectPosition(1, work.fine, &position, &FineSplashVolume);
    SetParticleEffectVelocity(1, work.fine, splashMovementVelocity, FineSplashVariance);
    SetParticleEffectBirthRate(1, work.fine, .1f);
    work.previousPosition = position;
    if (ticks < 20)
        return;
    if (ticks < 25) {
        SetParticleEffectSize(1, work.coarse, distance * .005f, distance * .005f);
        SetParticleEffectSize(1, work.fine, distance * .007f, distance * .007f);
    } else if (ticks < 70) {
        SetParticleEffectBirthRate(1, work.coarse, 0);
        SetParticleEffectBirthRate(1, work.fine, 0);
    } else {
        FreeParticleEffect(1, work.coarse);
        FreeParticleEffect(1, work.fine);
        work.coarse = -1;
        work.fine = -1;
    }
}
void YtqbKnockdown(float angle, std::int32_t delay) {
    const std::int32_t start = delay + 25;
    EVT_AT_SetAngY(YTQB_RYO_, YTQB_RYO_, angle, start);
    EVT_AT_SetMot(YTQB_RYO_, YTQB_RYOM_RYO_0048E_SIPAII_YARARE_W2QTE, 19, 1, angle >= 0 ? 0x60000000U : 0x40000000U, start);
    const float cameraAngle = angle >= 0 ? -0.0f : 0.0f;
    YtqbCamera(7907, 0, cameraAngle);
    EVT_AT_SetCam(7910, YTQB_DUMR, YTQB_DUMR, cameraAngle, 0, start);
    EVT_AT_SetSound(0x282, delay + 45);
    EVT_AT_SetFaceLooks(YTQB_RYO_, FacialExpressionId::Suffer, 15, 1, start);
}
void YtqbBackKnockdown(float angle) {
    EVT_AT_SetAngY(YTQB_RYO_, YTQB_RYO_, angle, 25);
    YtqbCamera(7915);
    EVT_AT_SetCam(7933, YTQB_DUMR, YTQB_DUMR, 0, 0, 25);
    EVT_AT_SetMot(YTQB_RYO_, YTQB_RYOM_RYO_0048E_DOWN_B_0, 2, 1, 0, 25);
    EVT_AT_SetSound(0x282, 65);
    EVT_AT_SetFaceLooks(YTQB_RYO_, FacialExpressionId::Suffer, 15, 1, 25);
}
void YtqbSideKnockdown(float angle) {
    EVT_AT_SetAngY(YTQB_RYO_, YTQB_RYO_, angle, 25);
    YtqbCamera(7909);
    EVT_AT_SetCam(7935, YTQB_DUMR, YTQB_DUMR, 0, 0, 25);
    EVT_AT_SetFaceLooks(YTQB_RYO_, FacialExpressionId::Suffer, 15, 1, 25);
    EVT_AT_SetMot(YTQB_RYO_, YTQB_RYOM_RYO_0048E_DOWN_SIDE_R_M, 2, 1, 0, 25);
    EVT_AT_SetSound(0x282, 70);
}
void YtqbFinalKnockdown(float angle) {
    EVT_AT_SetAngY(YTQB_RYO_, YTQB_RYO_, angle, 40);
    EVT_AT_SetCam(7934, YTQB_DUMR, YTQB_DUMR, 0, 0, 40);
    EVT_AT_SetMot(YTQB_RYO_, YTQB_RYOM_RYO_0048E_DOWN_C_0, 2, 1, 0, 40);
    EVT_AT_SetSound(0x282, 110);
    EVT_AT_SetFaceLooks(YTQB_RYO_, FacialExpressionId::Suffer, 15, 1, 40);
}
std::int32_t YtqbWaterSuccess() {
    PositionWaterFight();
    EventSetMotion motion{
        .cleanup = 0, .character = YTQB_RYO_, .animation = YTQB_RYOM_RYO_0048E_URAKEN_SEIKOU_5QTE, .frame = 19, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    const std::int32_t delay = static_cast<std::int32_t>((static_cast<float>(GetMotionFrameCount(motion.animation)) - 19.0f) + 0.0f);
    YtqbStand(YTQB_KIC_);
    YtqbCamera(7937);
    EVT_AT_SetCam(7938, YTQB_DUMR, YTQB_DUMR, 0, 0, 21);
    EVT_AT_SetVoice("S2BVA005", 1, 16);
    EVT_AT_SetSound(3366, 19);
    AnimateWater();
    HideWaterAfter(delay);
    return delay;
}
std::int32_t YtqbWaterFailure() {
    PositionWaterFight();
    YtqbPosition(YTQB_SUI0, YTQB_INVALID, YTQB_WaterFailure);
    EventSetMotion motion{.cleanup = 0, .character = YTQB_RYO_, .animation = WaterFailureLeadMotion, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(WaterFailureLeadMotion);
    EVT_AT_SetMot(YTQB_RYO_, YTQB_RYOM_RYO_0048E_KERI_SIPPAI_1QTE, 0, 1, 0, 17);
    const std::int32_t delay = static_cast<std::int32_t>(static_cast<float>(GetMotionFrameCount(YTQB_RYOM_RYO_0048E_KERI_SIPPAI_1QTE)) + 17.0f);
    YtqbStand(YTQB_KIC_);
    YtqbCamera(7937);
    EVT_AT_SetCam(7938, YTQB_DUMR, YTQB_DUMR, 0, 0, 21);
    EVT_AT_SetFaceLooks(YTQB_RYO_, FacialExpressionId::Suffer, 5, 1, 23);
    EVT_AT_SetFaceLooks(YTQB_RYO_, FacialExpressionId::Angry, 15, 1, delay);
    EVT_AT_SetVoice("S2BVA010", 1, 23);
    EVT_AT_SetSound(3366, 19);
    AnimateWater();
    HideWaterAfter(delay);
    return delay;
}
} // namespace shenmue::s2::areas
#endif
