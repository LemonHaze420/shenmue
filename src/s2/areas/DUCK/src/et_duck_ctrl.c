#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_duck.hpp"

const float duckCameraFactors[] = {0.1f, 0.1f, 0.5f, 0.02f, 0.1f, 0.05f};
const Vector3 duckReservedPoints[] = {
    {137.6f, 0.0f, 94.0f},
    {160.7f, 0.0f, 93.9f},
    {169.7f, 0.0f, 94.9f},
    {177.8f, 0.0f, 91.3f},
    {180.6f, 0.0f, 82.2f},
    {171.9f, 0.0f, 73.9f},
    {147.5f, 0.0f, 72.8f},
    {132.9f, 0.0f, 76.3f},
    {127.6f, 0.0f, 84.9f},
    {127.6f, 0.0f, 84.9f}
};
const DuckCourseRange duckCourseSegments[] = {
    {{1331.1f, 110.0f, 749.0f}, {1377.5f, 110.0f, 763.0f}},
    {{1377.5f, 110.0f, 763.0f}, {1345.9f, 110.0f, 772.4f}},
    {{1345.9f, 110.0f, 772.4f}, {1371.1f, 110.0f, 804.7f}},
    {{1371.1f, 110.0f, 804.7f}, {1399.3f, 110.0f, 775.6f}},
    {{194.0f, 107.5f, 304.7f}, {188.4f, 107.5f, 316.0f}}
};
const DuckProfile duckProfiles[DUCK_PROFILE_COUNT] = {
    {0.11574074f, 0.2175926f, 0.055555556f, 6.4f, 0.3f, 0, 2, 50, 30, 1.0f, 1.0f, 0.01f},
    {0.106481485f, 0.2361111f, 0.055555556f, 4.9f, 0.3f, 0, 0, 45, 60, 1.0f, 1.0f, 0.004f},
    {0.12037037f, 0.20833333f, 0.06481481f, 6.1f, 0.3f, 0, 10, 80, 20, 1.1f, 1.0f, 0.02f},
    {0.097222224f, 0.21296297f, 0.027777778f, 6.5f, 0.3f, 1, 4, 30, 20, 1.1f, 1.0f, 0.02f},
    {0.11111111f, 0.19907407f, 0.037037037f, 6.1f, 0.3f, 1, 5, 40, 40, 1.2f, 1.0f, 0.025f},
    {0.106481485f, 0.19444445f, 0.027777778f, 6.2f, 0.3f, 1, 8, 40, 30, 1.4f, 1.0f, 0.015f},
    {0.14351852f, 0.21296297f, 0.041666668f, 4.9f, 0.3f, 2, 5, 90, 25, 1.0f, 1.0f, 0.04f},
    {0.1388889f, 0.22685185f, 0.032407407f, 5.5f, 0.3f, 2, 15, 60, 25, 1.2f, 1.0f, 0.02f},
    {0.13425925f, 0.23148148f, 0.037037037f, 6.2f, 0.3f, 2, 20, 120, 40, 1.0f, 1.0f, 0.03f},
    {0.1388889f, 0.22685185f, 0.046296295f, 6.4f, 0.3f, 3, 6, 20, 60, 1.1f, 1.0f, 0.025f},
    {0.14351852f, 0.22222222f, 0.027777778f, 6.5f, 0.3f, 3, 12, 30, 50, 1.0f, 1.0f, 0.01f},
    {0.14814815f, 0.2175926f, 0.06481481f, 5.4f, 0.3f, 3, 4, 45, 40, 1.2f, 1.0f, 0.02f},
    {0.16111112f, 0.23518519f, 0.055555556f, 7.0f, 0.3f, 0, 35, 70, 200, 2.0f, 1.0f, 0.04f},
    {0.2037037f, 0.2361111f, 0.055555556f, 7.5f, 0.3f, 3, 30, 40, 60, 1.5f, 1.0f, 0.03f},
    {0.1388889f, 0.22222222f, 0.046296295f, 6.2f, 0.3f, 1, 40, 45, 30, 1.3f, 1.0f, 0.025f},
    {0.12037037f, 0.21296297f, 0.037037037f, 6.2f, 0.2f, 1, 10, 45, 30, 1.3f, 1.0f, 0.025f},
    {0.12037037f, 0.21296297f, 0.037037037f, 6.2f, 0.2f, 1, 10, 45, 30, 1.3f, 1.0f, 0.025f},
    {0.12037037f, 0.21296297f, 0.037037037f, 6.2f, 0.2f, 1, 10, 45, 30, 1.3f, 1.0f, 0.025f},
    {0.12037037f, 0.21296297f, 0.037037037f, 6.2f, 0.2f, 1, 10, 45, 30, 1.3f, 1.0f, 0.025f},
    {0.12037037f, 0.21296297f, 0.037037037f, 6.2f, 0.2f, 1, 10, 45, 30, 1.3f, 1.0f, 0.025f},
    {0.12037037f, 0.21296297f, 0.037037037f, 6.2f, 0.2f, 1, 10, 45, 30, 1.3f, 1.0f, 0.025f},
    {0.09259259f, 0.18518518f, 0.037037037f, 5.0f, 0.3f, 0, 0, 40, 30, 1.0f, 1.0f, 0.025f},
    {0.1574074f, 0.25f, 0.06481481f, 5.0f, 0.3f, 0, 60, 80, 30, 1.4f, 1.0f, 0.025f}
};
const shen_name duckProfileNames[DUCK_PROFILE_COUNT] = {
    SHEN_NAME4('d', 'u', 'k', '1'), SHEN_NAME4('d', 'u', 'k', '2'), SHEN_NAME4('d', 'u', 'k', '3'), SHEN_NAME4('d', 'u', 'k', '4'),
    SHEN_NAME4('d', 'u', 'k', '5'), SHEN_NAME4('d', 'u', 'k', '6'), SHEN_NAME4('d', 'u', 'k', '1'), SHEN_NAME4('d', 'u', 'k', '2'),
    SHEN_NAME4('d', 'u', 'k', '3'), SHEN_NAME4('d', 'u', 'k', '4'), SHEN_NAME4('d', 'u', 'k', '5'), SHEN_NAME4('d', 'u', 'k', '6'),
    SHEN_NAME4('p', 'e', 'n', 'g'), SHEN_NAME4('K', 'A', 'M', 'O'), SHEN_NAME4('K', 'A', 'M', 'M'), SHEN_NAME4('K', 'A', 'K', '0'),
    SHEN_NAME4('K', 'A', 'K', '1'), SHEN_NAME4('K', 'A', 'K', '2'), SHEN_NAME4('K', 'A', 'K', '3'), SHEN_NAME4('K', 'A', 'K', '4'),
    SHEN_NAME4('K', 'A', 'K', '5'), SHEN_NAME4('d', 'u', 'k', '1'), SHEN_NAME4('d', 'u', 'k', '1')
};

static short duckCameraIndex;
static short duckFollowedActor;
static Vector3 duckCameraTarget;
static Vector3 duckCameraProbe;
static int duckReservedBss;
static HLibTask *duckRaceTasks[DUCK_RACER_COUNT];
static float duckCourseLength;
static int duckStaminaScale;
static DuckReplaySample duckReplay[DUCK_RACER_COUNT][DUCK_REPLAY_FRAMES];

Vector3 duckCameraOffset = {0.0f, 1.0f, 1.0f};
int duckSpeedExperience = 0;
int duckStaminaExperience = 0;
int duckGutsExperience = 0;
int duckTurnExperience = 0;
DuckStyleCallback duckStyleCallbacks[] = {HoldOffSpeed, FastStartSpeed, QuickLegsSpeed, HomeStretchSpeed, HoldOffSpeed};

void InitializeDuckRaceCamera(void)
{
    Vector3 position;
    duckFollowedActor = 0;
    duckCameraIndex = 1;
    FTBL.CreateEventCharacter(3, 0, DUCK_CAMERA);
    SnapDuckRaceCameraTarget();
}

void UpdateDuckRaceFollowCamera(void)
{
    Vector3 position;
    Vector3i rotation;
    Vector3 offset = duckCameraOffset;
    shen_name name = duckRaceRoster[duckFollowedActor];
    float height;
    FTBL.GetCharaPosition(name, -1, &position, 0x40000000);
    FTBL.GetScenarioCharacterEulerAngles(name, &rotation, 0x40000000);
    duckCameraTarget.x = DuckLerp(position.x, duckCameraTarget.x, 0.3f);
    duckCameraTarget.y = DuckLerp(position.y, duckCameraTarget.y, 0.3f);
    duckCameraTarget.z = DuckLerp(position.z, duckCameraTarget.z, 0.3f);
    FTBL.ApplyScenarioTranslationAndRotation(&position, &rotation, &offset);
    duckCameraProbe.x = DuckLerp(offset.x, duckCameraProbe.x, duckCameraFactors[duckCameraIndex]);
    duckCameraProbe.y = DuckLerp(offset.y, duckCameraProbe.y, duckCameraFactors[duckCameraIndex]);
    duckCameraProbe.z = DuckLerp(offset.z, duckCameraProbe.z, duckCameraFactors[duckCameraIndex]);
    height = FTBL.shenFindIntersectionWithScene(&duckCameraProbe);
    if (height > 0.0f && 0.3f > duckCameraProbe.y - height)
        duckCameraProbe.y = height + 0.3f;
    FTBL.camCtrlSetState(26);
    FTBL.camCtrlSetNextPosition(&duckCameraProbe);
    FTBL.camCtrlSetTargetPosition(&duckCameraTarget);
    FTBL.camCtrlSetPerspective(18204);
}

void SnapDuckRaceCameraTarget(void)
{
    Vector3i rotation;
    Vector3 offset = duckCameraOffset;
    FTBL.GetCharaPosition(duckRaceRoster[duckFollowedActor], -1, &duckCameraTarget, 0x40000000);
    FTBL.GetScenarioCharacterEulerAngles(duckRaceRoster[duckFollowedActor], &rotation, 0x40000000);
    FTBL.ApplyScenarioTranslationAndRotation(&duckCameraTarget, &rotation, &offset);
    duckCameraProbe = offset;
}

void SetDuckRacePreviewCameraTarget(int actor)
{
    Vector3i rotation;
    Vector3 offset = duckCameraOffset;
    duckFollowedActor = actor % duckEntrantCount;
    FTBL.GetCharaPosition(duckRaceRoster[duckFollowedActor], -1, &duckCameraTarget, 0x40000000);
    FTBL.GetScenarioCharacterEulerAngles(duckRaceRoster[duckFollowedActor], &rotation, 0x40000000);
    FTBL.ApplyScenarioTranslationAndRotation(&duckCameraTarget, &rotation, &offset);
    duckCameraProbe = offset;
}

void InitializeDuckRaceCourse(void)
{
    duckCourseLength = 260.0f;
    duckStaminaScale = 1040;
}

void ResetDuckRaceRuntime(void)
{
    int i;
    duckArrivalCount = 0;
    duckEntrantCount = 0;
    for (i = 0; i < DUCK_RACER_COUNT; i++) {
        duckPlaceNames[i] = 0;
        duckArrivalTicks[i] = 0;
        duckRaceRoster[i] = 0;
        duckRaceTasks[i] = 0;
    }
    for (i = 0; i < DUCK_PROFILE_COUNT; i++)
        EVT_AT_CharaDisp(duckProfileNames[i], 0, 1);
}

void SetupDuckRacePlayer(unsigned short slot)
{
    float speed, stamina, turn;
    int guts, total, i;
    DuckRaceActor *actor;
    speed = FTBL.EV_GetPlayerFlag(15, 50);
    stamina = FTBL.EV_GetPlayerFlag(15, 51);
    guts = (float)FTBL.EV_GetPlayerFlag(15, 52);
    turn = FTBL.EV_GetPlayerFlag(15, 53);
    total = (int)speed + (int)stamina + guts + (int)turn;
    duckSpeedExperience = 0;
    duckStaminaExperience = 0;
    duckGutsExperience = 0;
    duckTurnExperience = 0;
    actor = GetDuckRaceActor(slot);
    InitializeDuckRaceActor(actor, 21);
    actor->speedLevels[DUCK_SPEED_NORMAL] += (duckProfiles[22].speedNormal - duckProfiles[21].speedNormal) / 255.0f * speed;
    actor->speedLevels[DUCK_SPEED_FAST] += (duckProfiles[22].speedMax - duckProfiles[21].speedMax) / 255.0f * speed;
    actor->speedLevels[DUCK_SPEED_SLOW] += (duckProfiles[22].speedMin - duckProfiles[21].speedMin) / 255.0f * speed;
    actor->stamina = actor->maximumStamina =
        duckStaminaScale * (duckProfiles[21].staminaBase + (duckProfiles[22].staminaBase - duckProfiles[21].staminaBase) / 255.0f * stamina);
    actor->guts += (int)((duckProfiles[22].guts - duckProfiles[21].guts) / 255.0f * guts);
    actor->handling += (int)((duckProfiles[22].handling - duckProfiles[21].handling) / 255.0f * turn);
    FTBL.DebugLogToFileRemoved("PlayerStatus\n");
    FTBL.DebugLogToFileRemoved("    SpdNml %f\n", actor->speedLevels[DUCK_SPEED_NORMAL] * 30.0f * 60.0f * 60.0f / 1000.0f);
    FTBL.DebugLogToFileRemoved("    SpdMax %f\n", actor->speedLevels[DUCK_SPEED_FAST] * 30.0f * 60.0f * 60.0f / 1000.0f);
    FTBL.DebugLogToFileRemoved("    SpdMin %f\n", actor->speedLevels[DUCK_SPEED_SLOW] * 30.0f * 60.0f * 60.0f / 1000.0f);
    FTBL.DebugLogToFileRemoved("    Stamina %d\n", actor->maximumStamina);
    FTBL.DebugLogToFileRemoved("    Guts   %d\n", actor->guts);
    FTBL.DebugLogToFileRemoved("    Int    %d\n", actor->handling);
    duckRaceRoster[slot] = actor->name;
    actor->aiControlled = 0;
    if (FTBL.shenGetRandomInt32(5) == 0 && total == 800 && FTBL.EV_GetPlayerFlag(11, 1646) && FTBL.GetCurrentWeather() == 2) {
        DuckRaceActor *opponent;
        for (i = 0; i < 5; i++) {
            opponent = GetDuckRaceActor(i + 1);
            EVT_AT_CharaDisp(opponent->name, 0, 1);
            InitializeDuckRaceActor(opponent, i + 12);
            duckRaceRoster[i + 1] = opponent->name;
        }
    } else if (FTBL.GetCurrentWeather() == 2 && FTBL.EV_GetPlayerFlag(11, 1646) && total > 700) {
        DuckRaceActor *opponent = GetDuckRaceActor(1);
        EVT_AT_CharaDisp(opponent->name, 0, 1);
        InitializeDuckRaceActor(opponent, 12);
        duckRaceRoster[1] = opponent->name;
    } else if (total > 0 && total % 14 == 0) {
        DuckRaceActor *opponent;
        for (i = 0; i < 5; i++) {
            opponent = GetDuckRaceActor(i + 1);
            EVT_AT_CharaDisp(opponent->name, 0, 1);
            InitializeDuckRaceActor(opponent, i + 14);
            duckRaceRoster[i + 1] = opponent->name;
        }
    } else if (total > 600 && FTBL.EV_GetPlayerFlag(11, 1646)) {
        DuckRaceActor *opponent;
        for (i = 0; i < 5; i++) {
            opponent = GetDuckRaceActor(i + 1);
            EVT_AT_CharaDisp(opponent->name, 0, 1);
            InitializeDuckRaceActor(opponent, i + 13);
            duckRaceRoster[i + 1] = opponent->name;
        }
    } else if (total > 500 && FTBL.EV_GetPlayerFlag(11, 1646)) {
        DuckRaceActor *opponent = GetDuckRaceActor(1);
        EVT_AT_CharaDisp(opponent->name, 0, 1);
        InitializeDuckRaceActor(opponent, 13);
        duckRaceRoster[1] = opponent->name;
        opponent = GetDuckRaceActor(2);
        EVT_AT_CharaDisp(opponent->name, 0, 1);
        InitializeDuckRaceActor(opponent, 14);
        duckRaceRoster[2] = opponent->name;
    } else if (total > 400 && FTBL.EV_GetPlayerFlag(11, 1646)) {
        DuckRaceActor *opponent = GetDuckRaceActor(1);
        EVT_AT_CharaDisp(opponent->name, 0, 1);
        InitializeDuckRaceActor(opponent, 13);
        duckRaceRoster[1] = opponent->name;
    } else if (total > 300 && FTBL.EV_GetPlayerFlag(11, 1646)) {
        DuckRaceActor *opponent = GetDuckRaceActor(1);
        EVT_AT_CharaDisp(opponent->name, 0, 1);
        InitializeDuckRaceActor(opponent, 14);
        duckRaceRoster[1] = opponent->name;
    } else if (total > 200 && FTBL.EV_GetPlayerFlag(11, 1646)) {
        DuckRaceActor *opponent;
        for (i = 0; i < 3; i++) {
            opponent = GetDuckRaceActor(i + 1);
            EVT_AT_CharaDisp(opponent->name, 0, 1);
            InitializeDuckRaceActor(opponent, i + 15);
            duckRaceRoster[i + 1] = opponent->name;
        }
    }
}

void QueueDuckRaceActor(int profile)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(DuckRaceActor_Callback, 3, 11, sizeof(DuckRaceActor), DUCK_TASK);
    DuckRaceActor *actor = FTBL.HLib_GetTaskParameter(task);
    memset(actor, 0, sizeof(DuckRaceActor));
    actor->self = task;
    InitializeDuckRaceActor(actor, profile);
    duckRaceRoster[duckEntrantCount] = actor->name;
    duckRaceTasks[duckEntrantCount] = actor->self;
    actor->index = duckEntrantCount;
    duckEntrantCount++;
}

void DestroyDuckRaceActors(void)
{
    int i;
    for (i = 0; i < duckEntrantCount; i++) {
        if (duckRaceTasks[i]) {
            FTBL.HLib_DestroyTask(duckRaceTasks[i]);
            duckRaceTasks[i] = 0;
        }
    }
}

void PlaceDuckRaceActors(void)
{
    int used[DUCK_RACER_COUNT];
    int i;
    unsigned short slot;
    DuckRaceActor *actor;
    for (i = 0; i < duckEntrantCount; i++)
        used[i] = 0;
    for (i = 0; i < duckEntrantCount; i++) {
        slot = FTBL.shenGetRandomInt32(duckEntrantCount);
        while (used[slot] == 1)
            slot = FTBL.shenGetRandomInt32(duckEntrantCount);
        used[slot] = 1;
        actor = GetDuckRaceActor(slot);
        EVT_AT_CharaDisp(actor->name, 0, 0);
        switch (g_currentMap) {
        case DUCK_AREA:
            EVT_AT_SetPos(actor->name, EVENT_CHARACTER_INVALID, 188.8f, actor->sizeScale * 0.15f + 107.2f, i * 0.6f + 310.0f, 0);
            EVT_AT_SetAngY(actor->name, EVENT_CHARACTER_INVALID, 270.0f, 0);
            break;
        case DUCK_MAP:
            EVT_AT_SetPos(actor->name, EVENT_CHARACTER_INVALID, 188.8f, actor->sizeScale * 0.15f + 107.2f, i * 0.6f + 310.0f, 0);
            EVT_AT_SetAngY(actor->name, EVENT_CHARACTER_INVALID, 270.0f, 0);
            break;
        }
    }
}

#define DUCK_SET_MOTION(actor, motion)     if ((actor)->currentMotion != (motion)) {         FTBL.ConfigureScenarioNodeMotion((actor)->name, (motion), 0.0f, 32767.0f, 0.0f, 1.0f, 0);         FTBL.ConfigureScenarioNodeMotion((actor)->name, 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);         FTBL.ConfigureScenarioNodeMotion((actor)->name, 0, -1.0f, -1.0f, -1.0f, 1.0f, -1);         (actor)->currentMotion = (motion);     }

void InitializeDuckRaceActor(DuckRaceActor *actor, int profile)
{
    Vector3 scale;
    switch (profile) {
    case 12:
        actor->name = SHEN_NAME4('p', 'e', 'n', 'g');
        break;
    case 13:
        actor->name = SHEN_NAME4('K', 'A', 'M', 'O');
        break;
    case 14:
        actor->name = SHEN_NAME4('K', 'A', 'M', 'M');
        break;
    case 15:
        actor->name = SHEN_NAME4('K', 'A', 'K', '0');
        break;
    case 16:
        actor->name = SHEN_NAME4('K', 'A', 'K', '1');
        break;
    case 17:
        actor->name = SHEN_NAME4('K', 'A', 'K', '2');
        break;
    case 18:
        actor->name = SHEN_NAME4('K', 'A', 'K', '3');
        break;
    case 19:
        actor->name = SHEN_NAME4('K', 'A', 'K', '4');
        break;
    case 20:
        actor->name = SHEN_NAME4('K', 'A', 'K', '5');
        break;
    case 21:
        actor->name = SHEN_NAME4('d', 'u', 'k', '1');
        break;
    default:
        actor->name = duckProfileNames[duckEntrantCount];
        break;
    }
    actor->speedLevels[DUCK_SPEED_NORMAL] = duckProfiles[profile].speedNormal;
    actor->speedLevels[DUCK_SPEED_FAST] = duckProfiles[profile].speedMax;
    actor->speedLevels[DUCK_SPEED_SLOW] = duckProfiles[profile].speedMin;
    actor->probeBackoff = duckProfiles[profile].probeBackoff;
    actor->separationRadius = duckProfiles[profile].separationRadius;
    actor->style = duckProfiles[profile].style;
    actor->guts = duckProfiles[profile].guts;
    actor->collisionStaminaCost = duckProfiles[profile].collisionCost;
    actor->handling = duckProfiles[profile].handling;
    actor->profile = profile;
    actor->steerTimer = 0;
    actor->behavior = 1;
    actor->stamina = actor->maximumStamina = duckStaminaScale * duckProfiles[profile].staminaBase;
    actor->speedTier = 1;
    actor->speed = actor->speedLevels[actor->speedTier];
    actor->flags = 0;
    actor->aiControlled = 1;
    actor->sizeScale = duckProfiles[profile].sizeScale;
    actor->speedResponse = duckProfiles[profile].speedResponse;
    actor->soundCooldown = 0;
    actor->collisionSlowTimer = 0;
    actor->turnLockTimer = 0;
    actor->actionTimer = 0;
    actor->distance = 0.0f;
    actor->finishTriggerSeen = 0;
    actor->separationRadius *= duckProfiles[profile].sizeScale;
    scale.x = scale.y = scale.z = duckProfiles[profile].sizeScale;
    FTBL.mtwkSetMotionworkScale3(actor->name, &scale);
    EVT_AT_CharaDisp(actor->name, 0, 0);
    switch (actor->profile) {
    case 12:
        DUCK_SET_MOTION(actor, 32544)
        break;
    case 13:
        DUCK_SET_MOTION(actor, 32542)
        break;
    case 14:
        DUCK_SET_MOTION(actor, 32542)
        break;
    case 15:
        DUCK_SET_MOTION(actor, 32540)
        break;
    case 16:
        DUCK_SET_MOTION(actor, 32540)
        break;
    case 17:
        DUCK_SET_MOTION(actor, 32540)
        break;
    case 18:
        DUCK_SET_MOTION(actor, 32540)
        break;
    case 19:
        DUCK_SET_MOTION(actor, 32540)
        break;
    case 20:
        DUCK_SET_MOTION(actor, 32540)
        break;
    default:
        if (FTBL.shenGetRandomInt32(2)) {
            DUCK_SET_MOTION(actor, 32527)
        } else {
            DUCK_SET_MOTION(actor, 32526)
        }
        break;
    }
}

void DuckRaceActor_Callback(void *data)
{
    DuckRaceActor *actor = (DuckRaceActor *)data;
    switch (GetDuckRaceState()) {
    case 14:
        TickDuckRaceActor(actor);
        break;
    case 12:
        DuckRaceActorNullsub(actor);
        break;
    case 11:
        TickDuckRaceActor(actor);
        break;
    }
}

void DuckRaceActorNullsub(DuckRaceActor *actor)
{
}

void TickDuckRaceActor(DuckRaceActor *actor)
{
    Vector3 trigger;
    int i;
    FTBL.GetCharaPosition(actor->name, -1, &actor->position, 0x40000000);
    FTBL.GetScenarioCharacterEulerAngles(actor->name, &actor->rotation, 0x40000000);
    for (i = DUCK_REPLAY_FRAMES - 1; i > 0; i--)
        duckReplay[actor->index][i] = duckReplay[actor->index][i - 1];
    duckReplay[actor->index][0].motion = actor->currentMotion;
    duckReplay[actor->index][0].position = actor->position;
    duckReplay[actor->index][0].yawDegrees = actor->rotation.y / 182.04445f;
    FTBL.CopyScenarioNodeMotionTimes(actor->name, 0, 0, &duckReplay[actor->index][0].motionFrame, 0);
    if (actor->aiControlled)
        TickDuckRaceAi(actor);
    else
        TickDuckRacePlayer(actor);
    FTBL.EV_AT_SetPos(actor->name, 0x78000000, &actor->position);
    FTBL.EV_AT_SetAngY_0(actor->name, 0x78000000, &actor->rotation, 0);
    UpdateDuckRaceDust(actor->index, 0.0f);
    UpdateDuckRaceActorMotion(actor);
    DuckRaceActorTickNullsub1(actor);
    DuckRaceActorTickNullsub2(actor);
    if (actor->soundCooldown)
        actor->soundCooldown--;
    if (actor->behavior != 2)
        actor->distance += actor->speed;
    trigger.x = 259.6f;
    trigger.y = 115.1f;
    trigger.z = 323.5f;
    if (FTBL.ComputeAngle(&trigger, 4.0f, 0, 0x8000, &actor->position))
        actor->finishTriggerSeen = 1;
}

void TickDuckRacePlayer(DuckRaceActor *actor)
{
    switch (actor->behavior) {
    case 2:
        UpdateDuckRacePlacementText(actor->replaySlot, actor->index, duckArrivalTicks[actor->replaySlot]);
        AdvanceDuckRaceActor(actor);
        ResolveDuckRaceActorCollisions(actor);
        UpdateDuckRaceReplayActor(actor);
        break;
    case 4:
        UpdateDuckRaceHopRecovery(actor);
        AdvanceDuckRaceActor(actor);
        UpdateDuckRacePlayerHopInput(actor);
        UpdateDuckRaceActorExperience(actor);
        break;
    case 8:
        AdvanceDuckRaceActor(actor);
        UpdateDuckRaceTurnRecovery(actor);
        ResolveDuckRaceActorCollisions(actor);
        break;
    default:
        UpdateDuckRacePlayerSpeed(actor);
        AdvanceDuckRaceActor(actor);
        AvoidDuckRaceCourseMarkers(actor);
        CheckDuckRaceCourseBounds(actor);
        ResolveDuckRaceActorCollisions(actor);
        CheckDuckRaceTurnTriggers(actor);
        UpdateDuckRaceActorExperience(actor);
        break;
    }
    FTBL.SetFightGaugeVisible(1);
    FTBL.UpdateSingleFightGauge(actor->stamina * 100.0f / actor->maximumStamina);
}

void TickDuckRaceAi(DuckRaceActor *actor)
{
    switch (actor->behavior) {
    case 2:
        UpdateDuckRacePlacementText(actor->replaySlot, actor->index, duckArrivalTicks[actor->replaySlot]);
        AdvanceDuckRaceActor(actor);
        ResolveDuckRaceActorCollisions(actor);
        UpdateDuckRaceReplayActor(actor);
        break;
    case 4:
        UpdateDuckRaceHopRecovery(actor);
        AdvanceDuckRaceActor(actor);
        UpdateDuckRaceActorExperience(actor);
        break;
    case 8:
        AdvanceDuckRaceActor(actor);
        UpdateDuckRaceTurnRecovery(actor);
        ResolveDuckRaceActorCollisions(actor);
        break;
    default:
        UpdateDuckRaceAiSpeed(actor);
        AdvanceDuckRaceActor(actor);
        UpdateDuckRaceTurnZones(actor);
        AvoidDuckRaceCourseMarkers(actor);
        CheckDuckRaceCourseBounds(actor);
        ResolveDuckRaceActorCollisions(actor);
        CheckDuckRaceTurnTriggers(actor);
        UpdateDuckRaceActorExperience(actor);
        break;
    }
}

void AdvanceDuckRaceActor(DuckRaceActor *actor)
{
    Vector3 origin;
    Vector3 destination;
    Vector3 intersection;
    short normal[2];
    float response;
    actor->position.x += FTBL.CalculateFixedSine(actor->rotation.y) * -actor->speed;
    actor->position.z += FTBL.CalculateFixedCosine(actor->rotation.y) * -actor->speed;
    destination.x = FTBL.CalculateFixedSine(actor->rotation.y) * (-actor->probeBackoff + actor->speed * 12.0f) + actor->position.x;
    destination.y = actor->position.y + 2.0f;
    destination.z = FTBL.CalculateFixedCosine(actor->rotation.y) * (-actor->probeBackoff + actor->speed * 12.0f) + actor->position.z;
    origin.x = FTBL.CalculateFixedSine(actor->rotation.y) * 0.3f + actor->position.x;
    origin.y = actor->position.y + 2.0f;
    origin.z = FTBL.CalculateFixedCosine(actor->rotation.y) * 0.3f + actor->position.z;
    if (actor->behavior != 4 && actor->aiControlled && actor->behavior != 8 && actor->behavior != 2) {
        if (actor->steerTimer == 0) {
            if (FTBL.FindNearestRayIntersection(&intersection, &destination, &origin) && !(actor->flags & 0x18)) {
                FTBL.transformIntersectionResult(normal);
                if ((short)(normal[1] - actor->rotation.y) < 0)
                    normal[1] += 16384;
                else
                    normal[1] -= 16384;
                actor->targetYaw = normal[1];
                actor->steerTimer = actor->handling + FTBL.shenGetRandomInt32(30);
                actor->flags |= 32;
            }
        } else {
            response = actor->flags & 128 ? 0.25f : 0.08f;
            if (actor->profile == 12)
                response += 0.08f;
            actor->rotation.y = DuckBlendAngle(actor->targetYaw, actor->rotation.y, response);
            if (910.0f > __builtin_fabs((short)(actor->rotation.y - actor->targetYaw))) {
                actor->steerTimer = 0;
                actor->flags &= ~32;
            } else
                actor->steerTimer--;
        }
    }
    if (actor->aiControlled)
        actor->flags &= ~128;
}

const DuckCourseRange duckTurnZones[] = {
    {{1349.0f, 0.0f, 747.0f}, {1353.2f, 0.0f, 752.8f}},
    {{1356.6f, 0.0f, 746.9f}, {1357.5f, 0.0f, 753.9f}},
    {{1356.4f, 0.0f, 777.8f}, {1352.8f, 0.0f, 788.8f}},
    {{1379.2f, 0.0f, 799.1f}, {1394.9f, 0.0f, 789.1f}},
    {{255.8f, 0.0f, 310.6f}, {263.6f, 0.0f, 312.6f}},
    {{228.4f, 0.0f, 331.5f}, {226.2f, 0.0f, 338.6f}},
    {{-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}
};
const Vector3 duckTurnTargets[][3] = {
    {{1356.7f, 0.0f, 748.1f}, {1356.2f, 0.0f, 750.0f}, {1356.3f, 0.0f, 751.9f}},
    {{1364.7f, 0.0f, 752.8f}, {1364.6f, 0.0f, 753.7f}, {1364.4f, 0.0f, 754.8f}},
    {{1374.0f, 0.0f, 784.3f}, {1376.8f, 0.0f, 786.4f}, {1370.5f, 0.0f, 788.9f}},
    {{1399.1f, 0.0f, 797.0f}, {1398.2f, 0.0f, 799.8f}, {1397.5f, 0.0f, 802.1f}},
    {{257.6f, 0.0f, 331.2f}, {259.2f, 0.0f, 331.3f}, {259.9f, 0.0f, 331.4f}},
    {{205.4f, 0.0f, 331.9f}, {205.4f, 0.0f, 333.9f}, {205.4f, 0.0f, 335.9f}},
    {{1391.4f, 0.0f, 798.6f}, {1392.8f, 0.0f, 797.0f}, {1394.0f, 0.0f, 795.8f}}
};

void UpdateDuckRaceTurnZones(DuckRaceActor *actor)
{
    int inside = 0;
    int i;
    for (i = 0; duckTurnZones[i].first.x > 0.0f; i++) {
        if (FTBL.TestCharaPositionWithinRange(actor->name, &duckTurnZones[i].first, &duckTurnZones[i].second, 0)) {
            inside = 1;
            if (!(actor->flags & 2) && actor->behavior == 1) {
                actor->targetYaw = DuckYawBetween(&duckTurnTargets[i][FTBL.shenGetRandomInt32(3)], &actor->position);
                actor->flags |= 2;
                actor->steerTimer = actor->handling * 2.0f;
            }
        }
    }
    if (!inside && (actor->flags & 2))
        actor->flags &= ~2;
}

void ResolveDuckRaceActorCollisions(DuckRaceActor *actor)
{
    int i;
    DuckRaceActor *other;
    DuckRaceActor *approaching;
    DuckRaceActor *obstacle;
    float distance, ground, difference;
    UpdateDuckRaceTurnExperience(actor);
    for (i = 0; i < duckEntrantCount; i++) {
        other = GetDuckRaceActor(i);
        if (actor->name == other->name)
            continue;
        distance = FTBL.GetEuclideanDistance(&other->position, &actor->position);
        if (actor->separationRadius + other->separationRadius > distance) {
            Vector3 first = {0.0f, 0.0f, 0.0f};
            Vector3 second = {0.0f, 0.0f, 0.0f};
            Vector3i rotation = {0, 0, 0};
            first.z = DUCK_FLUSH((actor->separationRadius + other->separationRadius - distance) * 0.5f);
            second.z = -first.z;
            rotation.y = DuckYawBetween(&other->position, &actor->position);
            FTBL.ApplyScenarioTranslationAndRotation(&actor->position, &rotation, &first);
            FTBL.ApplyScenarioTranslationAndRotation(&other->position, &rotation, &second);
            actor->position = first;
            other->position = second;
            FTBL.EV_AT_SetPos(other->name, 0x78000000, &other->position);
            approaching = 0;
            obstacle = 0;
            if (FTBL.ComputeAngle(&actor->position, 1.0f, actor->rotation.y, 8192, &other->position)) {
                approaching = actor;
                obstacle = other;
                FTBL.GetCharaPosition(obstacle->name, -1, &other->position, 0x40000000);
            } else if (FTBL.ComputeAngle(&other->position, 1.0f, other->rotation.y, 8192, &actor->position)) {
                approaching = other;
                obstacle = actor;
                FTBL.GetCharaPosition(approaching->name, -1, &other->position, 0x40000000);
            }
            if (!approaching)
                continue;
            if (!(8192.0f > __builtin_fabs((short)(approaching->rotation.y - obstacle->rotation.y))))
                continue;
            if (approaching->guts > obstacle->guts) {
                if (!(obstacle->flags & 16)) {
                    if (!actor->aiControlled) {
                        if (!obstacle->aiControlled)
                            duckGutsExperience += 4;
                        else
                            duckGutsExperience += 1;
                    }
                    if (!actor->soundCooldown) {
                        EVT_AT_SetSound(3067, 0);
                        actor->soundCooldown = 120;
                    }
                    rotation.y = DuckYawBetween(&approaching->position, &obstacle->position);
                    if ((short)(DuckYawBetween(&approaching->position, &obstacle->position) - obstacle->rotation.y) < 0)
                        obstacle->rotation.y += 3640;
                    else
                        obstacle->rotation.y -= 3640;
                    FTBL.EV_AT_SetAngY_0(obstacle->name, 0x78000000, &obstacle->rotation, 0);
                    obstacle->steerTimer = 0;
                    obstacle->stamina -= obstacle->collisionStaminaCost;
                    obstacle->flags |= 16;
                    obstacle->turnLockTimer = 30;
                }
            } else if (approaching->speed > 0.074074075f) {
                if (!actor->aiControlled) {
                    if (!approaching->aiControlled)
                        duckGutsExperience += 4;
                    else
                        duckGutsExperience += 1;
                }
                if (!actor->soundCooldown) {
                    EVT_AT_SetSound(3067, 0);
                    actor->soundCooldown = 120;
                }
                approaching->speed = (approaching->speed - approaching->speedLevels[DUCK_SPEED_SLOW]) * 0.7f;
                approaching->steerTimer = approaching->handling;
                approaching->stamina -= approaching->collisionStaminaCost;
                approaching->flags |= 8;
                approaching->collisionSlowTimer = 30;
            }
            if (approaching->stamina < 0)
                approaching->stamina = 0;
            if (obstacle->stamina < 0)
                obstacle->stamina = 0;
        }
    }
    UpdateDuckRaceTurnExperience(actor);
    if (actor->behavior != 4) {
        actor->position.y += 0.5f;
        ground = FTBL.shenFindIntersectionWithScene(&actor->position);
        actor->position.y -= 0.5f;
        difference = ground - actor->position.y + actor->sizeScale * 0.15f;
        if (ground > 0.0f) {
            if (-0.05f > difference && (actor->flags & 64)) {
                if (actor->actionTimer <= 30)
                    actor->position.y -= DUCK_MIN(0.8f, (45 - actor->actionTimer) * 0.002f);
                if (--actor->actionTimer < 0)
                    actor->actionTimer = 0;
            } else if ((difference > 0.4f && (actor->flags & 64)) || -0.2f > difference)
                actor->position.y -= 0.2f;
            else if (0.4f > difference) {
                actor->position.y = ground + actor->sizeScale * 0.15f;
                actor->flags &= ~64;
            }
        }
    }
}

void UpdateDuckRaceTurnExperience(DuckRaceActor *actor)
{
    Vector3 position;
    actor->position.y += 0.2f;
    if (FTBL.RaycastInWorldGrid(&position, &actor->position, 0.3f)) {
        actor->position = position;
        actor->turnLockTimer = -1;
        actor->flags &= ~16;
        if (!(actor->flags & 32))
            actor->steerTimer = 0;
        if (!actor->aiControlled && !(actor->flags & 128))
            duckTurnExperience++;
        else
            actor->flags |= 128;
        if (actor->speed > 0.027777778f && !actor->aiControlled)
            actor->speed = DuckLerp(0.0f, actor->speed, 0.08f);
    } else if (!actor->aiControlled)
        actor->behavior &= ~128;
    actor->position.y -= 0.2f;
}

void UpdateDuckRaceHopRecovery(DuckRaceActor *actor)
{
    Vector3 position;
    if (FTBL.RaycastInWorldGrid(&position, &actor->position, 0.3f))
        actor->position = position;
    actor->speed += 0.0006481481f;
    actor->speed = DUCK_MIN(actor->speed, 0.25f);
    if (++actor->actionTimer < 20) {
        if (actor->aiControlled)
            actor->rotation.y = DuckBlendAngle(actor->targetYaw, actor->rotation.y, 0.15f);
        actor->position.y += DUCK_FLUSH((1.0f - FTBL.CalculateFixedSine((unsigned short)(actor->actionTimer * 0.33333334f * 8192.0f / 45.0f))) * 0.008f);
    } else {
        actor->actionTimer = 45;
        actor->behavior = 1;
        actor->flags |= 64;
    }
}

void UpdateDuckRacePlayerSpeed(DuckRaceActor *actor)
{
    int input;
    float amount, target;
    int delta;
    int turn;
    if (actor->flags & 8) {
        if (actor->collisionSlowTimer-- < 0)
            actor->flags &= ~8;
        else
            actor->speed = DuckLerp(actor->speedLevels[DUCK_SPEED_SLOW], actor->speed, 0.04f);
    } else if (actor->flags & 16) {
        if (actor->turnLockTimer-- < 0)
            actor->flags &= ~16;
    } else if (!(actor->flags & 64)) {
        if (!(actor->flags & 1)) {
            input = FTBL.EVT_GetControllerInput(0, 3);
            amount = input / 255.0f;
            if (!input)
                target = 0.0f;
            else if (amount > 0.5f)
                target = actor->speedLevels[DUCK_SPEED_NORMAL] + (actor->speedLevels[DUCK_SPEED_FAST] - actor->speedLevels[DUCK_SPEED_NORMAL]) * ((amount - 0.5f) * 2.0f);
            else
                target = actor->speedLevels[DUCK_SPEED_NORMAL] + (actor->speedLevels[DUCK_SPEED_NORMAL] - actor->speedLevels[DUCK_SPEED_SLOW]) * (amount * 2.0f);
            actor->speed = DuckLerp(target, actor->speed, actor->speedResponse);
            actor->speed = DUCK_MIN(DUCK_MAX(0.0f, actor->speed), actor->speedLevels[DUCK_SPEED_FAST]);
            if (actor->speed > actor->speedLevels[DUCK_SPEED_NORMAL] + (actor->speedLevels[DUCK_SPEED_FAST] - actor->speedLevels[DUCK_SPEED_NORMAL]) * 0.75f)
                duckSpeedExperience++;
            if (actor->speed > actor->speedLevels[DUCK_SPEED_NORMAL] && actor->speedLevels[DUCK_SPEED_NORMAL] + (actor->speedLevels[DUCK_SPEED_FAST] - actor->speedLevels[DUCK_SPEED_NORMAL]) * 0.25f > actor->speed)
                duckStaminaExperience++;
            delta = actor->speed > actor->speedLevels[DUCK_SPEED_NORMAL] + (actor->speedLevels[DUCK_SPEED_FAST] - actor->speedLevels[DUCK_SPEED_NORMAL]) * 0.75f ? -3 :
                actor->speed > actor->speedLevels[DUCK_SPEED_NORMAL] + (actor->speedLevels[DUCK_SPEED_FAST] - actor->speedLevels[DUCK_SPEED_NORMAL]) * 0.5f       ? -1 :
                actor->speed > actor->speedLevels[DUCK_SPEED_NORMAL]                                                           ? 0 :
                actor->speed > actor->speedLevels[DUCK_SPEED_NORMAL] * 0.5f                                                    ? 1 :
                                                                                                              3;
            actor->stamina = DUCK_MIN(DUCK_MAX(0, actor->stamina + delta), actor->maximumStamina);
            if (!actor->stamina)
                actor->flags |= 1;
        } else {
            if (actor->stamina++ < actor->maximumStamina * 0.4f)
                actor->speed = DUCK_MAX(actor->speedLevels[DUCK_SPEED_SLOW], actor->speed - 0.0018518518f);
            else
                actor->flags &= ~1;
        }
    }
    input = FTBL.EVT_GetControllerInput(0, 0);
    delta = actor->speed > actor->speedLevels[DUCK_SPEED_NORMAL] + (actor->speedLevels[DUCK_SPEED_FAST] - actor->speedLevels[DUCK_SPEED_NORMAL]) * 0.75f ? -2 :
        actor->speed > actor->speedLevels[DUCK_SPEED_NORMAL] + (actor->speedLevels[DUCK_SPEED_FAST] - actor->speedLevels[DUCK_SPEED_NORMAL]) * 0.25f     ? -1 :
        actor->speed > actor->speedLevels[DUCK_SPEED_NORMAL]                                                          ? 0 :
        actor->speed > actor->speedLevels[DUCK_SPEED_NORMAL] * 0.5f                                                   ? 1 :
                                                                                                     2;
    turn = __builtin_fabs((0.19444445f / actor->speed + 1.0f) * actor->handling * 0.01f) < 1.1920929e-07f ? 0 :
        ((int)(((0.19444445f / actor->speed + 1.0f) * actor->handling * 0.01f) * 8192.0f / 45.0f) & 65535);
    if (turn > 455)
        turn = 455;
    if ((input & 0x4000) && !(actor->flags & 16)) {
        actor->rotation.y += turn;
        actor->speed = DUCK_MAX(0.027777778f, actor->speed);
    }
    if ((input & 0x8000) && !(actor->flags & 16)) {
        actor->rotation.y -= turn;
        actor->speed = DUCK_MAX(0.027777778f, actor->speed);
    }
}

void UpdateDuckRacePlayerHopInput(DuckRaceActor *actor)
{
    unsigned int input = FTBL.EVT_GetControllerInput(0, 0);
    if (input & 0x4000) {
        actor->rotation.y += 254;
        actor->speed = DUCK_MAX(0.027777778f, actor->speed);
    }
    if (input & 0x8000) {
        actor->rotation.y -= 254;
        actor->speed = DUCK_MAX(0.027777778f, actor->speed);
    }
    if (!FTBL.EVT_GetControllerInput(0, 3)) {
        actor->actionTimer = 45;
        actor->behavior = 1;
        actor->flags |= 64;
    } else
        actor->stamina -= 3;
    if (actor->stamina <= 0) {
        actor->actionTimer = 45;
        actor->behavior = 1;
        actor->flags |= 64;
    }
}

const DuckTurnTrigger duckTurnTriggers[] = {
    {{220.3f, 109.5f, 324.9f}, 0, 4.0f},
    {{241.7f, 113.2f, 310.0f}, 0, 3.5f},
    {{220.3f, 109.5f, 324.9f}, 32768, -1.0f}
};

void CheckDuckRaceTurnTriggers(DuckRaceActor *actor)
{
    int i;
    for (i = 0; duckTurnTriggers[i].valid > 0.0f; i++) {
        if (FTBL.ComputeAngle(&duckTurnTriggers[i].position, 3.0f, 0, 0x8000, &actor->position)
            && __builtin_fabs((short)(duckTurnTriggers[i].yaw - actor->rotation.y)) > 21845.0f) {
            actor->behavior = 8;
            actor->targetYaw = duckTurnTriggers[i].yaw;
        }
    }
}

void UpdateDuckRaceTurnRecovery(DuckRaceActor *actor)
{
    if (actor->speed > 0.027777778f)
        actor->speed -= 0.0037037036f;
    else {
        actor->rotation.y = DuckBlendAngle(actor->targetYaw, actor->rotation.y, 0.03f);
        if (910.0f > __builtin_fabs((short)(actor->rotation.y - actor->targetYaw))) {
            actor->steerTimer = 0;
            actor->flags = 0;
            actor->behavior = 1;
        }
    }
}

void AvoidDuckRaceCourseMarkers(DuckRaceActor *actor)
{
    shen_name markers[10] = {SHEN_NAME4('D', 'R', 'M', '1'), SHEN_NAME4('D', 'R', 'M', '2'), SHEN_NAME4('D', 'R', 'M', '3'),
        SHEN_NAME4('D', 'R', 'M', '4'), SHEN_NAME4('D', 'R', 'M', '5'), SHEN_NAME4('D', 'R', 'M', '6'), SHEN_NAME4('D', 'R', 'M', '7'),
        SHEN_NAME4('D', 'R', 'M', '8'), SHEN_NAME4('D', 'R', 'M', '9'), 0};
    Vector3 position;
    int i;
    float radius;
    for (i = 0; markers[i]; i++) {
        FTBL.GetCharaPosition(markers[i], -1, &position, 0x40000000);
        radius = actor->separationRadius + 0.4f;
        if (radius > FTBL.CalculatePlanarDistanceBetweenPoints(&position, &actor->position)) {
            Vector3 offset = {0.0f, 0.0f, 0.0f};
            Vector3i rotation = {0, 0, 0};
            actor->steerTimer = 0;
            offset.z = radius;
            rotation.y = DuckYawBetween(&position, &actor->position);
            FTBL.ApplyScenarioTranslationAndRotation(&position, &rotation, &offset);
            actor->position.x = offset.x;
            actor->position.z = offset.z;
            if (actor->aiControlled) {
                if ((short)(rotation.y - actor->rotation.y) < 0)
                    rotation.y += 0x4000;
                else
                    rotation.y -= 0x4000;
                actor->rotation.y = DuckBlendAngle(rotation.y, actor->rotation.y, 0.05f);
                actor->speed = DuckLerp(actor->speedLevels[DUCK_SPEED_SLOW], actor->speed, 0.07f);
            } else
                actor->speed = DuckLerp(actor->speedLevels[DUCK_SPEED_SLOW], actor->speed, 0.2f);
        }
    }
}

void CheckDuckRaceJumpBounds(DuckRaceActor *actor)
{
    Vector3 oldPosition;
    Vector3 position;
    Vector3 basePosition;
    Vector3i baseRotation;
    Vector3 offset = {0.0f, 0.0f, 0.0f};
    float height;
    FTBL.AT_SetLocalBaseChara(actor->name, SHEN_NAME4('J', 'D', 'M', '1'));
    FTBL.GetCharaPosition(SHEN_NAME4('J', 'D', 'M', '1'), -1, &basePosition, 0x40000000);
    FTBL.GetScenarioCharacterEulerAngles(SHEN_NAME4('J', 'D', 'M', '1'), &baseRotation, 0x40000000);
    FTBL.GetCharaPosition(actor->name, -1, &oldPosition, 0);
    FTBL.EV_AT_SetPos(actor->name, 0x78000000, &actor->position);
    FTBL.GetCharaPosition(actor->name, -1, &position, 0);
    if (0.1f > position.z && position.z > -1.5f - actor->separationRadius && position.x > -0.8f - actor->separationRadius
        && actor->separationRadius + 0.8f > position.x) {
        height = basePosition.y + FTBL.FixedAngleTangent(4096) * -position.z;
        if (position.y + basePosition.y + FTBL.FixedAngleTangent(4096) * actor->speed + 0.02f > height && position.z > -1.5f
            && position.x > -0.8f - actor->separationRadius * 0.5f && actor->separationRadius * 0.5f + 0.8f > position.x) {
            actor->flags |= 4;
            actor->position.y = actor->sizeScale * 0.15f + height;
        } else if (actor->flags & 4) {
            actor->flags &= ~4;
            actor->behavior = 4;
            if (oldPosition.y > 0.53f && actor->speed > 0.18518518f && !(actor->flags & 24)) {
                actor->actionTimer = 0;
                actor->steerTimer = 0;
                actor->targetYaw = baseRotation.y;
            } else {
                actor->actionTimer = 100;
                actor->steerTimer = 0;
            }
        } else {
            if (-1.5f > oldPosition.z) {
                offset.x = position.x;
                offset.z = -1.5f - actor->separationRadius;
            } else {
                if (position.x > 0.0f)
                    offset.x = actor->separationRadius + 0.8f;
                else
                    offset.x = -0.8f - actor->separationRadius;
                offset.z = position.z;
            }
            FTBL.ApplyScenarioTranslationAndRotation(&basePosition, &baseRotation, &offset);
            actor->position.x = offset.x;
            actor->position.z = offset.z;
            actor->steerTimer = 0;
            actor->speed = DuckLerp(actor->speedLevels[DUCK_SPEED_SLOW], actor->speed, 0.2f);
        }
    } else
        actor->flags &= ~4;
    FTBL.AT_SetLocalBaseChara(actor->name, -1);
}

void CheckDuckRaceCourseBounds(DuckRaceActor *actor)
{
    Vector3 oldPosition;
    Vector3 position;
    Vector3 basePosition;
    Vector3i baseRotation;
    FTBL.AT_SetLocalBaseChara(actor->name, SHEN_NAME4('J', 'D', 'M', '2'));
    FTBL.GetCharaPosition(SHEN_NAME4('J', 'D', 'M', '2'), -1, &basePosition, 0x40000000);
    FTBL.GetScenarioCharacterEulerAngles(SHEN_NAME4('J', 'D', 'M', '2'), &baseRotation, 0x40000000);
    FTBL.GetCharaPosition(actor->name, -1, &oldPosition, 0);
    FTBL.EV_AT_SetPos(actor->name, 0x78000000, &actor->position);
    FTBL.GetCharaPosition(actor->name, -1, &position, 0);
    if (0.5f > position.z && position.z > -0.5f - actor->separationRadius && position.x > -3.0f - actor->separationRadius
        && actor->separationRadius + 3.0f > position.x)
        actor->flags |= 4;
    else if (actor->flags & 4) {
        if (actor->speed > 0.18518518f && !(actor->flags & 24)) {
            actor->actionTimer = 0;
            actor->steerTimer = 0;
            actor->targetYaw = baseRotation.y;
            actor->behavior = 4;
        } else
            actor->steerTimer = 0;
        actor->flags &= ~4;
    }
    FTBL.AT_SetLocalBaseChara(actor->name, -1);
}

void UpdateDuckRaceAiSpeed(DuckRaceActor *actor)
{
    int tick;
    if (!(actor->flags & 0x18)) {
        if (actor->flags & 1) {
            if (actor->stamina > actor->maximumStamina * 0.5f)
                actor->flags &= ~1;
            actor->speedTier = 2;
        } else {
            actor->speedTier = duckStyleCallbacks[actor->style](actor);
            if (actor->speedTier != 2 && actor->stamina <= 0) {
                actor->flags |= 1;
                actor->speedTier = 2;
            }
        }
    } else {
        if (actor->flags & 8) {
            if (actor->collisionSlowTimer-- < 0)
                actor->flags &= ~8;
            else
                actor->speedTier = 2;
        }
        if (actor->flags & 16) {
            if (actor->turnLockTimer-- < 0)
                actor->flags &= ~16;
        }
    }
    tick = actor->speed > actor->speedLevels[DUCK_SPEED_NORMAL] + (actor->speedLevels[DUCK_SPEED_FAST] - actor->speedLevels[DUCK_SPEED_NORMAL]) * 0.75f ? -2 :
        actor->speed > actor->speedLevels[DUCK_SPEED_NORMAL] + (actor->speedLevels[DUCK_SPEED_FAST] - actor->speedLevels[DUCK_SPEED_NORMAL]) * 0.5f   ? -1 :
        actor->speed > actor->speedLevels[DUCK_SPEED_NORMAL]                                                                                       ? 0 :
        actor->speed > actor->speedLevels[DUCK_SPEED_NORMAL] * 0.5f                                                                                ? 1 :
                                                                                                                                                     2;
    if (actor->flags & 64)
        actor->speed = DuckLerp(actor->speedLevels[DUCK_SPEED_NORMAL], actor->speed, actor->speedResponse * 0.1f);
    else {
        actor->stamina = DUCK_MIN(DUCK_MAX(0, actor->stamina + tick), actor->maximumStamina);
        actor->speed = DuckLerp(actor->speedLevels[actor->speedTier], actor->speed, actor->speedResponse);
    }
}

void UpdateDuckRaceReplayActor(DuckRaceActor *actor)
{
    Vector3 position;
    if (FTBL.RaycastInWorldGrid(&position, &actor->position, 0.3f))
        actor->position = position;
    if (FindDuckRaceCourseSegment(actor->name) != -1) {
        if (actor->speed > 0.018518519f)
            actor->speed = DuckLerp(0.027777778f, actor->speed, 0.02f);
    } else
        actor->speed -= 0.0027777778f;
    if (actor->speed < 0.0f)
        actor->speed = 0.0f;
}

void UpdateDuckRaceActorExperience(DuckRaceActor *actor)
{
    int speed, stamina, guts, turn, placement;
    if (FindDuckRaceCourseSegment(actor->name) == 4 && !(actor->behavior & 2) && DUCK_MIN((int)(actor->distance / duckCourseLength * 4.0f), 3) == 3
        && actor->finishTriggerSeen) {
        if (duckArrivalCount == 6)
            FTBL.EventAssertNullsub(__FILE__, 1519);
        if (!duckArrivalCount)
            QueueDuckRaceReplay();
        actor->behavior = 2;
        actor->replaySlot = duckArrivalCount;
        duckPlaceNames[duckArrivalCount] = actor->name;
        duckArrivalTicks[duckArrivalCount] = GetDuckRaceTicks();
        if (!actor->aiControlled) {
            speed = FTBL.EV_GetPlayerFlag(15, 50);
            stamina = FTBL.EV_GetPlayerFlag(15, 51);
            guts = FTBL.EV_GetPlayerFlag(15, 52);
            turn = FTBL.EV_GetPlayerFlag(15, 53);
            placement = 6 - duckArrivalCount;
            speed = DUCK_MIN(speed + CalculateDuckRaceExperience(DUCK_MIN(duckSpeedExperience / 70, 10) + placement), 255);
            FTBL.EV_SetPlayerFlag(16, 50, speed);
            stamina = DUCK_MIN(stamina + CalculateDuckRaceExperience(DUCK_MIN(duckStaminaExperience / 120, 10) + placement), 255);
            FTBL.EV_SetPlayerFlag(16, 51, stamina);
            guts = DUCK_MIN(guts + CalculateDuckRaceExperience(placement + DUCK_MIN(duckGutsExperience / 4, 10)), 255);
            FTBL.EV_SetPlayerFlag(16, 52, guts);
            turn = DUCK_MIN(turn + CalculateDuckRaceExperience(DUCK_MAX(0, 10 - duckTurnExperience / 20) + placement), 255);
            FTBL.EV_SetPlayerFlag(16, 53, turn);
            FTBL.DebugLogToFileRemoved("SpeedExp   = %04d : LV %03d\n", duckSpeedExperience, speed);
            FTBL.DebugLogToFileRemoved("StaminaExp = %04d : LV %03d\n", duckStaminaExperience, stamina);
            FTBL.DebugLogToFileRemoved("GutsExp    = %04d : LV %03d\n", duckGutsExperience, guts);
            FTBL.DebugLogToFileRemoved("TurnExp    = %04d : LV %03d\n", duckTurnExperience, turn);
            FTBL.DebugLogToFileRemoved("Total = %d\n", speed + stamina + guts + turn);
            duckRaceExperiencePublished = 1;
        }
        duckArrivalCount++;
    }
}

int CalculateDuckRaceExperience(int increment)
{
    int total = FTBL.EV_GetPlayerFlag(15, 50) + FTBL.EV_GetPlayerFlag(15, 51) + FTBL.EV_GetPlayerFlag(15, 52) + FTBL.EV_GetPlayerFlag(15, 53);
    if (total + increment >= 800) {
        increment = 800 - total;
        if (increment < 0)
            increment = 0;
    }
    return increment;
}

void UpdateDuckRaceActorMotion(DuckRaceActor *actor)
{
    if (actor->speed > 9.259259e-4f) {
        if (actor->flags & 0x18) {
            switch (actor->profile) {
            case 12:
                DUCK_SET_MOTION(actor, 32545)
                break;
            case 13:
                DUCK_SET_MOTION(actor, 32543)
                break;
            case 14:
                DUCK_SET_MOTION(actor, 32543)
                break;
            case 15:
                DUCK_SET_MOTION(actor, 32541)
                break;
            case 16:
                DUCK_SET_MOTION(actor, 32541)
                break;
            case 17:
                DUCK_SET_MOTION(actor, 32541)
                break;
            case 18:
                DUCK_SET_MOTION(actor, 32541)
                break;
            case 19:
                DUCK_SET_MOTION(actor, 32541)
                break;
            case 20:
                DUCK_SET_MOTION(actor, 32541)
                break;
            default:
                DUCK_SET_MOTION(actor, 32534)
                break;
            }
        } else {
            switch (actor->profile) {
            case 12:
                DUCK_SET_MOTION(actor, 32545)
                break;
            case 13:
                DUCK_SET_MOTION(actor, 32543)
                break;
            case 14:
                DUCK_SET_MOTION(actor, 32543)
                break;
            case 15:
                DUCK_SET_MOTION(actor, 32541)
                break;
            case 16:
                DUCK_SET_MOTION(actor, 32541)
                break;
            case 17:
                DUCK_SET_MOTION(actor, 32541)
                break;
            case 18:
                DUCK_SET_MOTION(actor, 32541)
                break;
            case 19:
                DUCK_SET_MOTION(actor, 32541)
                break;
            case 20:
                DUCK_SET_MOTION(actor, 32541)
                break;
            default:
                DUCK_SET_MOTION(actor, 32525)
                break;
            }
            FTBL.ConfigureScenarioNodeMotion(actor->name, 0, -1.0f, -1.0f, -1.0f, actor->speed * 10.0f + 0.1f, -1);
        }
    } else {
        switch (actor->profile) {
        case 12:
            DUCK_SET_MOTION(actor, 32544)
            break;
        case 13:
            DUCK_SET_MOTION(actor, 32542)
            break;
        case 14:
            DUCK_SET_MOTION(actor, 32542)
            break;
        case 15:
            DUCK_SET_MOTION(actor, 32540)
            break;
        case 16:
            DUCK_SET_MOTION(actor, 32540)
            break;
        case 17:
            DUCK_SET_MOTION(actor, 32540)
            break;
        case 18:
            DUCK_SET_MOTION(actor, 32540)
            break;
        case 19:
            DUCK_SET_MOTION(actor, 32540)
            break;
        case 20:
            DUCK_SET_MOTION(actor, 32540)
            break;
        default:
            DUCK_SET_MOTION(actor, 32527)
            break;
        }
    }
}

#define DUCK_QUARTER(actor) DUCK_MIN((int)((actor)->distance / duckCourseLength * 4.0f), 3)

short HoldOffSpeed(DuckRaceActor *actor)
{
    int result = actor->stamina <= actor->maximumStamina * 0.2f;
    if (DUCK_QUARTER(actor) == 3)
        result = 0;
    return result;
}

short FastStartSpeed(DuckRaceActor *actor)
{
    int result = 1;
    switch (DUCK_QUARTER(actor)) {
    case 0:
        if (actor->stamina > actor->maximumStamina * 0.75f)
            result = 0;
        break;
    case 1:
        if (actor->stamina > actor->maximumStamina * 0.5f)
            result = 0;
        break;
    case 2:
        if (actor->stamina > actor->maximumStamina * 0.25f)
            result = 0;
        break;
    case 3:
        result = 0;
        break;
    }
    return result;
}

short QuickLegsSpeed(DuckRaceActor *actor)
{
    int result = 1;
    if (DUCK_QUARTER(actor) == 2 && actor->stamina > actor->maximumStamina * 0.5f)
        result = 0;
    if (DUCK_QUARTER(actor) == 3)
        result = 0;
    return result;
}

short HomeStretchSpeed(DuckRaceActor *actor)
{
    int result = 1;
    if (DUCK_QUARTER(actor) >= 2)
        result = 0;
    else if (actor->stamina < actor->maximumStamina)
        result = 2;
    return result;
}

short FindDuckRaceCourseSegment(shen_name name)
{
    int i;
    for (i = 0; i < 5; i++) {
        if (FTBL.TestCharaPositionWithinRange(name, &duckCourseSegments[i].first, &duckCourseSegments[i].second, 0))
            return i;
    }
    return -1;
}

DuckRaceActor *GetDuckRaceActor(unsigned short index)
{
    if (index > 5 || !duckRaceTasks[index])
        FTBL.EventAssertNullsub(__FILE__, 1738);
    return FTBL.HLib_GetTaskParameter(duckRaceTasks[index]);
}

void DuckRaceActorTickNullsub1(DuckRaceActor *actor)
{
}

void DuckRaceActorTickNullsub2(DuckRaceActor *actor)
{
}

typedef struct DuckReplayWork {
    HLibTask *self;
    int state;
    int timer;
} DuckReplayWork;

void QueueDuckRaceReplay(void)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(DuckReplay_Callback, 3, 11, sizeof(DuckReplayWork), SHEN_NAME4('r', 'p', 'l', 'y'));
    DuckReplayWork *work = FTBL.HLib_GetTaskParameter(task);
    int i;
    int state = 0;
    work->self = task;
    work->state = state;
    work->timer = 30;
    for (i = 0; i < DUCK_RACER_COUNT; i++) {
        if (duckRaceTasks[i])
            FTBL.HLib_SetupTimedTask(duckRaceTasks[i], 0);
    }
}

shen_name GetDuckProfileName(int profile)
{
    return duckProfileNames[profile];
}

short GetDuckRaceActorProfile(int index)
{
    return GetDuckRaceActor(index)->profile;
}

void DuckReplay_Callback(void *data)
{
    DuckReplayWork *work = (DuckReplayWork *)data;
    Vector3i rotation = {0, 0, 0};
    int i;
    switch (work->state) {
    case 0:
        duckFollowCameraEnabled = 0;
        EVT_AT_SetCam(3000, EVENT_CHARACTER_INVALID, EVENT_CHARACTER_INVALID, 0, 0, 0);
        work->state++;
    case 1:
    case 2:
    case 3:
        if (work->timer-- > 0) {
            for (i = 0; i < DUCK_RACER_COUNT; i++) {
                FTBL.EV_AT_SetPos(duckRaceRoster[i], 0x78000000, &duckReplay[i][work->timer].position);
                rotation.y = (unsigned short)(duckReplay[i][work->timer].yawDegrees * 8192.0f / 45.0f);
                FTBL.EV_AT_SetAngY_0(duckRaceRoster[i], 0x78000000, &rotation, 0);
                FTBL.ConfigureScenarioNodeMotion(duckRaceRoster[i], duckReplay[i][work->timer].motion, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
                FTBL.ConfigureScenarioNodeMotion(duckRaceRoster[i], 0, -1.0f, -1.0f, duckReplay[i][work->timer].motionFrame, 32767.0f, -1);
                FTBL.ConfigureScenarioNodeMotion(duckRaceRoster[i], 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
                FTBL.ConfigureScenarioNodeMotion(duckRaceRoster[i], 0, -1.0f, -1.0f, -1.0f, 1.0f, -1);
            }
        } else {
            if (work->state == 1) {
                EVT_AT_SetCam(3001, EVENT_CHARACTER_INVALID, EVENT_CHARACTER_INVALID, 0, 0, 0);
                work->timer = 30;
            } else if (work->state == 2) {
                EVT_AT_SetCam(3002, EVENT_CHARACTER_INVALID, EVENT_CHARACTER_INVALID, 0, 0, 0);
                work->timer = 60;
            }
            work->state++;
        }
        break;
    case 4:
        work->state++;
    case 5:
        for (i = 0; i < DUCK_RACER_COUNT; i++) {
            if (duckRaceTasks[i])
                FTBL.HLib_ResolveDeferredCallback(duckRaceTasks[i]);
        }
        duckFollowCameraEnabled = 1;
        FTBL.HLib_DestroyCurrentEventTask();
        SnapDuckRaceCameraTarget();
        break;
    }
}
#endif
