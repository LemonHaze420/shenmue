#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_0504.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void Start0504(void)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(Update0504, 4, 11, sizeof(Area0504Parameter), AREA_0504_EVENT);
    Area0504Parameter *parent = (Area0504Parameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
    parent->state = 1;
    parent->selection = 0;
    FTBL.InitializeShipSpray();
    Setup0504Environment();
    Queue_EVT_UTL_LoadNPCChara_3(area0504Characters, parent->self);
}
#else
#define AREA_0504_SQRT(x) ({ float root_ = (x); root_ = __builtin_sqrtf(root_); root_; })
static float ryoHeight0504 = 0.0f;
static float shenhuaHeight0504 = 0.0f;
static int cameraRide0504 = 1;
static int previousCameraRide0504 = 0;
static shen_name sprayCharacter0504 = AREA_0504_BOAT;
shen_name area0504Characters[] = {AREA_0504_BOAT, INVALID_CHARACTER};
static unsigned short splashFrame0504 = 0;
static Vector3 characterPositions0504[2], previousEye0504, previousTarget0504;

void Update0504(void *data)
{
    Area0504Parameter *parent = (Area0504Parameter *)data;
    switch (parent->state) {
    case 1:
        Queue0504Author(parent);
        break;
    case 3:
        Cleanup0504(parent);
        break;
    }
}

void Queue0504Author(Area0504Parameter *parent)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(Update0504Author, 5, 11, sizeof(Area0504AuthorParameter), AREA_0504_AUTHOR);
    ((Area0504AuthorParameter *)FTBL.HLib_GetTaskParameter(task))->parent = parent;
    Prepare0504Characters();
    FTBL.EV_authSceneCtrl(parent->selection);
    parent->state = 2;
}

void Update0504Author(void *data)
{
    Area0504AuthorParameter *child = (Area0504AuthorParameter *)data;
    short frame = FTBL.EV_authSceneCtrl(-6);
    FTBL.GetCharaPosition(AREA_0504_RYO, -1, &characterPositions0504[0], 0);
    FTBL.GetCharaPosition(AREA_0504_SIN, -1, &characterPositions0504[1], 0);
    switch (frame) {
    case 1:
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.SyncWindWithWDATWeatherState();
        break;
    case 821:
        FTBL.SetNewWindPhaseAndSpeed(0xf10c, 0.2f);
        break;
    case 1011:
        FTBL.SyncWindWithWDATWeatherState();
        break;
    case 1486:
        cameraRide0504 = 0;
        break;
    case 1516:
        cameraRide0504 = 1;
        break;
    case 1538:
        FTBL.EV_SetupImageModule(AREA_0504_SIN, "WSH_M");
        FTBL.FindOSAG(FTBL.ctrlGetCharaByName(AREA_0504_SIN));
        break;
    case 1776:
        FTBL.SetPlayerGameFlags(4, 0);
        break;
    case 1850:
        FTBL.EV_SetupImageModule(AREA_0504_RYO, "WRY_M");
        break;
    }
    switch (frame) {
    case 471:
        FTBL.EV_WeatherEffectUtil_Impl(25486, 1.5f, 1.15f, 0.95f, 0.8f, 7.8f);
        break;
    case 541:
        FTBL.EV_WeatherEffectUtil_Impl(21845, 1.5f, 1.15f, 0.95f, 1.5f, 7.8f);
        break;
    case 671:
        FTBL.EV_WeatherEffectUtil_Impl(25486, 1.5f, 1.15f, 0.95f, 0.8f, 7.8f);
        break;
    case 821:
        FTBL.EV_WeatherEffectUtil_Impl(21845, 1.5f, 1.15f, 0.95f, 1.5f, 7.8f);
        break;
    case 1306:
        FTBL.EV_WeatherEffectUtil_Impl(25486, 1.5f, 1.15f, 0.95f, 0.8f, 7.8f);
        break;
    case 1336:
        FTBL.EV_WeatherEffectUtil_Impl(21845, 1.5f, 1.15f, 0.95f, 1.5f, 7.8f);
        break;
    case 1371:
        FTBL.EV_WeatherEffectUtil_Impl(29127, 1.5f, 1.15f, 0.95f, 0.4f, 7.8f);
        break;
    case 1451:
        FTBL.EV_WeatherEffectUtil_Impl(21845, 1.5f, 1.15f, 0.95f, 1.5f, 7.8f);
        break;
    case 2556:
        FTBL.EV_WeatherEffectUtil_Impl(25486, 1.5f, 1.15f, 0.95f, 0.8f, 7.8f);
        break;
    case 2596:
        FTBL.EV_WeatherEffectUtil_Impl(21845, 1.5f, 1.15f, 0.95f, 1.5f, 7.8f);
        break;
    case 3266:
        FTBL.EV_WeatherEffectUtil_Impl(25486, 1.5f, 1.15f, 0.95f, 0.5f, 7.8f);
        break;
    case 3326:
        FTBL.EV_WeatherEffectUtil_Impl(21845, 1.5f, 1.15f, 0.95f, 1.5f, 7.8f);
        break;
    case 3536:
        FTBL.EV_WeatherEffectUtil_Impl(29127, 1.5f, 1.15f, 0.95f, 0.8f, 7.8f);
        break;
    case 3566:
        FTBL.EV_WeatherEffectUtil_Impl(21845, 1.5f, 1.15f, 0.95f, 1.5f, 7.8f);
        break;
    }
    if (frame == 2052)
        Create0504Splash(AREA_0504_BOAT, 100, 0.010588235f, 21);
    switch (frame) {
    case 1520:
        Create0504Splash(AREA_0504_SIN, 2000, 0.023137255f, 5);
        break;
    case 1614:
        Create0504Splash(AREA_0504_SIN, 100, 0.010588235f, 14);
        break;
    case 1629:
        Create0504Splash(AREA_0504_SIN, 100, 0.010588235f, 21);
        break;
    case 1664:
        Create0504Splash(AREA_0504_SIN, 100, 0.010588235f, 14);
        break;
    case 2090:
        Create0504Splash(AREA_0504_SIN, 100, 0.010588235f, 21);
        break;
    case 2178:
        Create0504Splash(AREA_0504_SIN, 100, 0.010588235f, 21);
        break;
    case 2197:
        Create0504Splash(AREA_0504_SIN, 100, 0.010588235f, 14);
        break;
    case 2213:
        Create0504Splash(AREA_0504_SIN, 100, 0.010588235f, 21);
        break;
    case 2227:
        Create0504Splash(AREA_0504_SIN, 100, 0.010588235f, 14);
        break;
    }
    switch (frame) {
    case 1840:
        Create0504Splash(AREA_0504_RYO, 2000, 0.023137255f, 5);
        break;
    case 1942:
        Create0504Splash(AREA_0504_RYO, 100, 0.010588235f, 21);
        break;
    case 1966:
        Create0504Splash(AREA_0504_RYO, 100, 0.010588235f, 14);
        break;
    case 1981:
        Create0504Splash(AREA_0504_RYO, 100, 0.010588235f, 21);
        break;
    case 2007:
        Create0504Splash(AREA_0504_RYO, 100, 0.010588235f, 14);
        break;
    case 2139:
        Create0504Splash(AREA_0504_RYO, 100, 0.010588235f, 14);
        break;
    case 2158:
        Create0504Splash(AREA_0504_RYO, 100, 0.010588235f, 21);
        break;
    case 2259:
        Create0504Splash(AREA_0504_RYO, 100, 0.010588235f, 14);
        break;
    case 2441:
        Create0504Splash(AREA_0504_RYO, 300, 0.0043137255f, 5);
        break;
    case 2473:
        Create0504Splash(AREA_0504_RYO, 300, 0.0043137255f, 5);
        break;
    case 2498:
        Create0504Splash(AREA_0504_RYO, 300, 0.0043137255f, 5);
        break;
    case 2513:
        Create0504Splash(AREA_0504_RYO, 300, 0.0043137255f, 5);
        break;
    case 2536:
        Create0504Splash(AREA_0504_RYO, 300, 0.0043137255f, 5);
        break;
    case 2627:
        Create0504Splash(AREA_0504_RYO, 100, 0.010588235f, 14);
        break;
    case 2663:
        Create0504Splash(AREA_0504_RYO, 100, 0.010588235f, 21);
        break;
    case 2697:
        Create0504Splash(AREA_0504_RYO, 100, 0.010588235f, 21);
        break;
    case 2744:
        Create0504Splash(AREA_0504_RYO, 100, 0.010588235f, 21);
        break;
    case 2786:
        Create0504Splash(AREA_0504_RYO, 100, 0.010588235f, 21);
        break;
    case 2851:
        Create0504Splash(AREA_0504_RYO, 100, 0.010588235f, 14);
        break;
    case 2878:
        Create0504Splash(AREA_0504_RYO, 100, 0.010588235f, 14);
        break;
    case 2907:
        Create0504Splash(AREA_0504_RYO, 100, 0.010588235f, 14);
        break;
    case 3133:
        Create0504Splash(AREA_0504_RYO, 500, 0.023137255f, 3);
        break;
    case 3266:
        Create0504Splash(AREA_0504_RYO, 300, 0.010588235f, 5);
        break;
    case 3440:
        Create0504Splash(AREA_0504_RYO, 300, 0.010588235f, 5);
        break;
    case 3442:
        Create0504Splash(AREA_0504_RYO, 100, 0.010588235f, 21);
        break;
    case 3485:
        Create0504Splash(AREA_0504_RYO, 300, 0.010588235f, 5);
        break;
    case 3530:
        Create0504Splash(AREA_0504_RYO, 300, 0.0043137255f, 5);
        break;
    case 3555:
        Create0504Splash(AREA_0504_RYO, 300, 0.0043137255f, 5);
        break;
    case 3585:
        Create0504Splash(AREA_0504_RYO, 300, 0.0043137255f, 5);
        break;
    case 3598:
        Create0504Splash(AREA_0504_RYO, 100, 0.010588235f, 21);
        break;
    case 3600:
        Create0504Splash(AREA_0504_RYO, 300, 0.010588235f, 5);
        break;
    case 3626:
        Create0504Splash(AREA_0504_RYO, 300, 0.010588235f, 5);
        break;
    case 3665:
        Create0504Splash(AREA_0504_RYO, 300, 0.010588235f, 5);
        break;
    case 3701:
        Create0504Splash(AREA_0504_RYO, 300, 0.010588235f, 5);
        break;
    case 3811:
        Create0504Splash(AREA_0504_RYO, 300, 0.010588235f, 5);
        break;
    }
    switch (frame) {
    case 1:
        sprayCharacter0504 = AREA_0504_BOAT;
        break;
    case 1590:
        sprayCharacter0504 = AREA_0504_SIN;
        break;
    case 1686:
        sprayCharacter0504 = AREA_0504_BOAT;
        break;
    case 2026:
        sprayCharacter0504 = AREA_0504_BOAT;
        break;
    case 2061:
        sprayCharacter0504 = AREA_0504_SIN;
        break;
    case 2165:
        sprayCharacter0504 = AREA_0504_SIN;
        break;
    case 2286:
        sprayCharacter0504 = AREA_0504_BOAT;
        break;
    case 1928:
    case 2106:
    case 2436:
        sprayCharacter0504 = AREA_0504_RYO;
        break;
    }
    if (frame > 1850)
        Update0504CharacterHeight(AREA_0504_RYO, 0);
    if (frame > 1570 && frame < 3326)
        Update0504CharacterHeight(AREA_0504_SIN, 0);
    else if (frame > 3325)
        Update0504CharacterHeight(AREA_0504_SIN, AREA_0504_RYO);
    if (frame > 2286)
        Update0504CharacterHeight(AREA_0504_BOAT, AREA_0504_SIN);
    else
        Update0504CharacterHeight(AREA_0504_BOAT, 0);
    if (!FTBL.EV_authSceneCtrl(-1)) {
        FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        Restore0504Characters();
        child->parent->state = 3;
        FTBL.HLib_DestroyCurrentTask();
    }
}

void Update0504CharacterHeight(shen_name character, shen_name heightSource)
{
    Vector3 samples[5], position;
    float height = 0.0f;
    void *root = FTBL.GetMapModelRoot(1);
    int i;
    FTBL.GetCharaPosition(character, -1, &samples[0], 0);
    FTBL.GetCharaPosition(character, -1, &position, 0);
    for (i = 1; i <= 4; i++) {
        samples[i].x = samples[0].x;
        samples[i].y = samples[0].y;
        samples[i].z = samples[0].z;
    }
    samples[1].x += 0.5f;
    samples[2].x -= 0.5f;
    samples[3].z += 0.5f;
    samples[4].z -= 0.5f;
    for (i = 0; i <= 4; i++)
        height += FTBL.FindFieldGroundHeight(samples[i], root) - 95.995003f;
    switch (character) {
    case AREA_0504_RYO:
        ryoHeight0504 = height / 5.0f;
        break;
    case AREA_0504_SIN:
        shenhuaHeight0504 = height / 5.0f;
        break;
    }
    switch (heightSource) {
    case AREA_0504_RYO:
        position.y += ryoHeight0504;
        break;
    case AREA_0504_SIN:
        position.y += shenhuaHeight0504;
        break;
    default:
        position.y += height / 5.0f;
        break;
    }
    if (position.y > 100.0f)
        position.y = 100.0f;
    FTBL.EV_AT_SetPos(character, AREA_0504_RECURSIVE_XYZ, &position);
    {
        Vector3 spray, offset = {0.0f, -0.1f, 0.0f};
        Vector3i rotation;
        if (character == sprayCharacter0504) {
            FTBL.GetCharaPosition(character, 5, &spray, 0);
            FTBL.GetScenarioCharacterEulerAngles(AREA_0504_BOAT, &rotation, 0);
            rotation.y += 0x8000;
            spray.y = FTBL.FindFieldGroundHeight(spray, root);
            FTBL.EventFunction862(&offset);
            FTBL.AssignShipSprayPosition(&spray);
            FTBL.AssignShipSprayYaw(rotation.y);
        }
    }
}

void Create0504Splash(shen_name character, int count, float size, unsigned int bone)
{
    Vector3 position;
    Vector3 spawn = {0.2f, 0.0f, 0.2f};
    Vector3 velocity = {-0.0050980374f, 0.047450982f, 0.028627448f};
    Vector3 variance = {0.02784314f, 0.041176476f, 0.052156858f};
    void *root = FTBL.GetMapModelRoot(1);
    float height;
    HLibTask *task;
    Area0504SplashParameter *child;
    FTBL.GetCharaPosition(character, bone, &position, 0);
    height = FTBL.FindFieldGroundHeight(position, root);
    if (character == AREA_0504_SIN)
        position.y = height - 0.3f;
    else
        position.y = height;
    task = FTBL.HLib_EnqueueTaskWithParameter(Update0504Splash, 4, 11, sizeof(Area0504SplashParameter), AREA_0504_SPLASH);
    child = (Area0504SplashParameter *)FTBL.HLib_GetTaskParameter(task);
    child->effect = FTBL.CreateSplashEffect(&position, &spawn, count, 0.02f, 0, 95.995003f, 0, 0);
    child->birthFrame = 20;
    child->freeFrame = 70;
    FTBL.SetParticleEffectColour(1, child->effect, 77, 227, 212, 142);
    FTBL.SetParticleEffectPosition(1, child->effect, &position, &spawn);
    FTBL.SetParticleEffectVelocity(1, child->effect, &velocity, &variance);
    FTBL.SetParticleEffectBirthRate(1, child->effect, 0.078431375f);
    FTBL.SetParticleEffectSize(1, child->effect, size, size);
    FTBL.ResumeParticleEffect(1, child->effect);
}

void Update0504Splash(void *data)
{
    Area0504SplashParameter *child = (Area0504SplashParameter *)data;
    if (!FTBL.EV_authSceneCtrl(-1)) {
        FTBL.SuspendParticleEffect(1, child->effect);
        FTBL.FreeParticleEffect(1, child->effect);
        FTBL.HLib_DestroyCurrentTask();
    } else if (splashFrame0504 == child->birthFrame) {
        FTBL.SetParticleEffectBirthRate(1, child->effect, 0.0f);
        splashFrame0504++;
    } else if (splashFrame0504 == child->freeFrame) {
        splashFrame0504 = 0;
        FTBL.SuspendParticleEffect(1, child->effect);
        FTBL.FreeParticleEffect(1, child->effect);
        FTBL.HLib_DestroyCurrentTask();
    } else
        splashFrame0504++;
}

void Setup0504Environment(void)
{
    EventDateTime date;
    unsigned char distance[256], second[256], first[256];
    Vector3 direction;
    void *secondKernel;
    FTBL.GetCurrentDateTime((DateTime *)&date);
    date.hour = 12;
    date.minute = 0;
    FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&date);
    FTBL.SetPlayerControlEnabled(0);
    FTBL.SetRenderClockAndCompassTo(1);
    FTBL.EV_SetWeatherInstruction(0x780, 0);
    FTBL.FreeWAIR();
    FTBL.UpdateWeatherForMinute(1, 0, 0, 1.0f);
    FTBL.UpdateWeather();
    FTBL.EV_WeatherEffectUtil_Impl(21845, 1.5f, 1.15f, 0.95f, 1.5f, 7.8f);
    FTBL.EV_FreeAuthorPak(1, "0504");
    direction.x = 0.0f;
    direction.y = 0.872f;
    direction.z = -0.489f;
    FTBL.GenerateFFT(1.0f);
    FTBL.fft_fill_dist(FTBL.fft_fill_krn(31.4f, FTBL.generate_distance_falloff_row_major(1.5f, distance)));
    secondKernel = FTBL.fft_fill_krn(0.06f, FTBL.generate_distance_falloff_kernel_8x8(-0.85f, second));
    FTBL.fft_bins_load_pairwise_tables((const float *)secondKernel, (const float *)FTBL.fft_fill_krn(0.0f, FTBL.generate_distance_falloff_kernel_8x8(-1.15f, first)));
    FTBL.FFTUtil(0.0f, 0.008f);
    FTBL.set_lighting_flags(0.1f, &direction);
    previousEye0504.x = previousEye0504.y = previousEye0504.z = 0.0f;
    previousTarget0504.x = previousTarget0504.y = previousTarget0504.z = 0.0f;
    FTBL.camCtrlSetRotationCallback(Update0504CameraRideHeight);
    FTBL.HLib_EnqueueTaskWithoutParameter(Update0504Waves, 15, 2, AREA_0504_WAVE);
}

void Cleanup0504(Area0504Parameter *parent)
{
    EVT_UTL_FreeNPCChara(area0504Characters);
    FTBL.camCtrlSetRotationCallback(0);
    FTBL.camCtrlSetRotation(0, 0);
    FTBL.EV_FreeAuthorPak(0, "0504");
    FTBL.HLib_DestroyTask(parent->self);
    FTBL.EV_scenQueueLoad(g_actNumber, AREA_0504_RETURN, 0);
}

void Prepare0504Characters(void)
{
    HideAuthorCharacter(AREA_0504_RYO);
    HideAuthorCharacter(AREA_0504_SIN);
    HideAuthorCharacter(AREA_0504_BOAT);
    HideAuthorProp(AREA_0504_PROP);
    FTBL.clmdUtil(FTBL.ctrlGetCharaByName(AREA_0504_SIN), 6);
}

void Restore0504Characters(void)
{
    ShowAuthorCharacter(AREA_0504_RYO);
    ShowAuthorCharacter(AREA_0504_SIN);
    ShowAuthorCharacter(AREA_0504_BOAT);
    ShowAuthorProp(AREA_0504_PROP);
    FTBL.clmdUtil(FTBL.ctrlGetCharaByName(AREA_0504_SIN), 0);
}

#define SHENMUE_EVENT_AUTHOR_PROPS
#include "s2/events/event_author_dc.h"

void Update0504Waves(void *data)
{
    if (!FTBL.shenIsGamePaused()) {
        float scale = 1.0f;
        FTBL.BuildLightingTableFrom(0, scale, scale, scale, scale, scale, scale, scale, scale, 10.0f, scale);
        FTBL.ApplyFFTWaveformToStripTree(FTBL.GetMapModelRoot(1));
    }
}

void Update0504CameraRideHeight(void)
{
    Vector3 eye, target, translation;
    Area0504Rotation correction, rotation;
    Vector3 ryo, shenhua, eyeDelta, targetDelta, delta;
    short pitch = 0;
    int reset = cameraRide0504 != 0 && previousCameraRide0504 == 0;
    float characterHeight, surface, height, ryoDistance, shenhuaDistance, level;
    void *root;
    previousCameraRide0504 = cameraRide0504;
    if (cameraRide0504 != 1 || !(root = FTBL.GetMapModelRoot(1)))
        return;
    FTBL.camCtrlGetRot(&rotation.pitch);
    FTBL.camCtrlGetNextCamPos(&eye);
    FTBL.camCtrlGetTargetPosition(&target);
    eyeDelta.x = eye.x - previousEye0504.x;
    eyeDelta.y = eye.y - previousEye0504.y;
    eyeDelta.z = eye.z - previousEye0504.z;
    targetDelta.x = target.x - previousTarget0504.x;
    targetDelta.y = target.y - previousTarget0504.y;
    targetDelta.z = target.z - previousTarget0504.z;
    previousEye0504 = eye;
    previousTarget0504 = target;
    if (eyeDelta.x * eyeDelta.x + eyeDelta.y * eyeDelta.y + eyeDelta.z * eyeDelta.z > 0.25f)
        reset = 1;
    else if (targetDelta.x * targetDelta.x + targetDelta.y * targetDelta.y + targetDelta.z * targetDelta.z > 0.25f)
        reset = 1;
    FTBL.GetCharaPosition(AREA_0504_RYO, -1, &ryo, 0);
    FTBL.GetCharaPosition(AREA_0504_SIN, -1, &shenhua, 0);
    ryoDistance = FTBL.GetEuclideanDistance(&characterPositions0504[0], &target);
    shenhuaDistance = FTBL.GetEuclideanDistance(&characterPositions0504[1], &target);
    characterHeight = shenhuaDistance > ryoDistance ? ryo.y - characterPositions0504[0].y : shenhua.y - characterPositions0504[1].y;
    surface = FTBL.FindFieldGroundHeight(eye, root);
    FTBL.camCtrlGetRotationXform(&translation, &correction.pitch);
    if (surface != 9999.0f) {
        height = 0.0f;
        level = surface + 0.3f;
        if (level > eye.y)
            height = level - eye.y;
        if (height > 0.0f && characterHeight > 0.2f) {
            target.y += characterHeight;
            eye.y += height;
            delta.x = target.x - eye.x;
            delta.y = target.y - eye.y;
            delta.z = target.z - eye.z;
            pitch = FTBL.InverseFixedAtan2(-delta.y, -AREA_0504_SQRT(delta.x * delta.x + delta.z * delta.z));
            pitch -= rotation.pitch;
        }
        if ((translation.y - height < 0.0f ? -(translation.y - height) : translation.y - height) > 1.0f)
            reset = 1;
        translation.x = 0.0f;
        translation.y += (height - translation.y) * 0.3f;
        if (reset)
            translation.y = height;
        translation.z = 0.0f;
        correction.pitch = (int)((pitch - correction.pitch) * 0.3f) + correction.pitch;
        if (reset)
            correction.pitch = pitch;
        correction.yaw = 0;
        correction.roll = 0;
    } else {
        translation.x = translation.y = translation.z = 0.0f;
        correction.pitch = correction.yaw = correction.roll = 0;
    }
    FTBL.camCtrlSetRotation(&translation, &correction.pitch);
}
#endif
#else
#include "s2/areas/area_0504.hpp"
#include "s2/author.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/model_component.hpp"
#include "s2/particle_effects.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/ship_spray.hpp"
#include "s2/snapshot.hpp"
#include "s2/surface_wave.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include <array>
#include <bit>
#include <cmath>

namespace shenmue::s2::areas {
namespace {
std::int32_t cameraRide0504, previousCameraRide0504;
Vector3 ryoPosition0504, shenhuaPosition0504, previousEye0504, previousTarget0504;
float ryoHeight0504, shenhuaHeight0504;
std::int16_t splashFrame0504;
shen_name sprayCharacter0504;
void Weather0504(std::uint32_t fov, float minimumY) {
    EV_WeatherEffectUtil_Impl(fov, 1.5f, std::bit_cast<std::int32_t>(1.15f), 0.94999999f, minimumY, 7.8000002f);
}
void Splash0504AtFrames(
    std::int32_t frame, shen_name character, std::uint32_t count, float size, std::uint32_t bone, std::initializer_list<std::int32_t> frames) {
    for (const std::int32_t candidate : frames)
        if (candidate == frame) {
            Create0504Splash(character, count, size, bone);
            break;
        }
}
} // namespace
void Initialize0504() {
    cameraRide0504 = 1;
    previousCameraRide0504 = 0;
    ryoPosition0504 = {};
    shenhuaPosition0504 = {};
    previousEye0504 = {};
    previousTarget0504 = {};
    ryoHeight0504 = 0.0f;
    shenhuaHeight0504 = 0.0f;
    splashFrame0504 = 0;
    sprayCharacter0504 = AREA_0504_BOAT;
}
void Start0504(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0504, 4, 11, sizeof(Area0504Parameter), AREA_0504_EVENT);
    Area0504Parameter *const parent = static_cast<Area0504Parameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = 1;
    InitializeShipSpray();
    Setup0504Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0504_CHARACTERS, parent->self);
}
void Update0504(void *data) {
    Area0504Parameter *const parent = static_cast<Area0504Parameter *>(data);
    if (parent->state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0504Author, 5, 11, sizeof(Area0504AuthorParameter), AREA_0504_AUTHOR);
        static_cast<Area0504AuthorParameter *>(HLib::GetTaskParameter(task))->parent = parent;
        Prepare0504Characters();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
        parent->state = 2;
    } else if (parent->state == 3)
        Cleanup0504(parent);
}
void Setup0504Environment() {
    DateTime date{};
    shenGetCurrentDateTime(date);
    date.hour = 12;
    date.minute = 0;
    SetCurrentDateTimeTo(date);
    SetPlayerControlEnabled(0);
    shenSetClockAndCompassRenderTo(1);
    EV_SetWeatherInstruction(0x780, 0);
    EV_FreeWAIR();
    EV_UpdateWeatherForMinute(1, 0, 0, 1.0f);
    EV_UpdateWeather();
    Weather0504(21845, 1.5f);
    EV_FreeAuthorPak(1, "0504");
    std::array<std::byte, 256> distance, first, second;
    GenerateFFT(1.0f);
    GenerateDistanceFalloffRowMajor(1.5f, distance);
    FillFftKernel(31.4f, distance.data());
    FillFftDistance(distance.data());
    GenerateDistanceFalloffKernel8x8(-1.15f, first);
    FillFftKernel(0.0f, first.data());
    GenerateDistanceFalloffKernel8x8(-0.85f, second);
    FillFftKernel(0.06f, second.data());
    ApplyFft(second.data(), first.data());
    FinalizeFft(0.0f, 0.008f);
    SetWaveLighting(0.1f, Vector3{0.0f, 0.872f, -0.489f});
    previousEye0504 = {};
    previousTarget0504 = {};
    camCtrlSetRotationCallback(Update0504CameraRideHeight);
    HLib::EnqueueTaskWithoutParameter(Update0504Waves, 15, 2, static_cast<std::int32_t>(AREA_0504_WAVE.name));
    ConfigureSurfaceWaveModel(3, 2.0f);
    InitializeSurfaceWaveParameters();
}
void Prepare0504Characters() {
    for (const shen_name character : {AREA_0504_RYO, AREA_0504_SIN, AREA_0504_BOAT}) {
        mtwkSetExtraFlag(FindMotionWork(ctrlGetScenarioChara(character)), 6, 2);
        EV_SetCharacterAlphaEnabled(character, false);
    }
    EV_SetCharacterAlphaEnabled(AREA_0504_PROP, false);
    clmdUtil(ctrlGetScenarioChara(AREA_0504_SIN), 6);
}
void Restore0504Characters() {
    for (const shen_name character : {AREA_0504_RYO, AREA_0504_SIN, AREA_0504_BOAT}) {
        mtwkSetExtraFlag(FindMotionWork(ctrlGetScenarioChara(character)), 6, 0);
        EV_SetCharacterAlphaEnabled(character, true);
    }
    EV_SetCharacterAlphaEnabled(AREA_0504_PROP, true);
    clmdUtil(ctrlGetScenarioChara(AREA_0504_SIN), 0);
}
void Update0504Waves(void *) {
    if (static_cast<std::uint8_t>(shenIsGamePaused()) != 0)
        return;
    BuildWaveLightingTable(0, 1.0f, 1.0f, Vector3{1.0f, 1.0f, 1.0f}, Vector3{1.0f, 1.0f, 1.0f}, 10.0f, 1.0f);
    ApplyWaveTextureResource(1);
}
void Create0504Splash(shen_name character, std::uint32_t count, float size, std::uint32_t bone) {
    GetMapModelRoot(1);
    Vector3 position{};
    EV_GetCharaPosition(character, bone, &position, EventTransformFlags::Local);
    position.y = GetSurfaceHeightAt(position.x, position.z);
    if (character.name == AREA_0504_SIN.name || (character.name == AREA_0504_RYO.name && EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 1840))
        position.y -= 0.30000001f;
    const Vector3 spawn{0.2f, 0.0f, 0.2f};
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0504Splash, 4, 11, sizeof(Area0504SplashParameter), AREA_0504_SPLASH);
    Area0504SplashParameter *const child = static_cast<Area0504SplashParameter *>(HLib::GetTaskParameter(task));
    child->effect = static_cast<std::uint32_t>(
        CreateSplashEffect(position, spawn, static_cast<std::int32_t>(count), 0.02f, shen_name{}, std::bit_cast<std::int32_t>(95.995003f), nullptr, nullptr));
    child->birthFrame = 20;
    child->freeFrame = 70;
    SetParticleEffectColour(1, child->effect, 77, 227, 212, 142);
    SetParticleEffectPosition(1, child->effect, &position, &spawn);
    SetParticleEffectVelocity(1, child->effect, Vector3{-0.0050980374f, 0.047450982f, 0.028627448f}, Vector3{0.02784314f, 0.041176476f, 0.052156858f});
    SetParticleEffectBirthRate(1, child->effect, 0.078431375f);
    SetParticleEffectSize(1, child->effect, size, size);
    ResumeParticleEffect(1, child->effect);
}
void Update0504Splash(void *data) {
    Area0504SplashParameter *const child = static_cast<Area0504SplashParameter *>(data);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        SuspendParticleEffect(1, child->effect);
        FreeParticleEffect(1, child->effect);
        HLib::DestroyCurrentTask();
    } else if (splashFrame0504 == child->birthFrame) {
        SetParticleEffectBirthRate(1, child->effect, 0.0f);
        ++splashFrame0504;
    } else if (splashFrame0504 == child->freeFrame) {
        splashFrame0504 = 0;
        SuspendParticleEffect(1, child->effect);
        FreeParticleEffect(1, child->effect);
        HLib::DestroyCurrentTask();
    } else
        ++splashFrame0504;
}
void Update0504CharacterHeight(shen_name character, shen_name heightSource) {
    GetMapModelRoot(1);
    Vector3 base{}, position{};
    EV_GetCharaPosition(character, CHARACTER_TRANSLATION_ROOT, &base, EventTransformFlags::Local);
    EV_GetCharaPosition(character, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    const Vector3 samples[]{base, Vector3{base.x + 0.05f, base.y, base.z}, Vector3{base.x - 0.05f, base.y, base.z}, Vector3{base.x, base.y, base.z + 0.05f},
        Vector3{base.x, base.y, base.z - 0.05f}};
    float height = 0.0f;
    for (const Vector3 &point : samples)
        height += GetSurfaceHeightAt(point.x, point.z) - 95.995003f;
    if (character.name == AREA_0504_SIN.name)
        shenhuaHeight0504 = height / 5.0f;
    else if (character.name == AREA_0504_RYO.name)
        ryoHeight0504 = height / 5.0f;
    position.y += heightSource.name == AREA_0504_SIN.name ? shenhuaHeight0504 : heightSource.name == AREA_0504_RYO.name ? ryoHeight0504 : height / 5.0f;
    if (character.name == AREA_0504_SIN.name && static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) - 2181) <= 58U)
        position.y -= 0.34999999f;
    if (position.y > 100.0f)
        position.y = 100.0f;
    EV_AT_SetPos(character, EventTransformFlags::AllAxes, position);
    if (character.name == sprayCharacter0504.name) {
        Vector3 spray{};
        Vector3i rotation{};
        EV_GetCharaPosition(character, 5, &spray, EventTransformFlags::Local);
        GetScenarioCharacterEulerAngles(AREA_0504_BOAT, &rotation, EventTransformFlags::Local);
        rotation.y += 0x8000;
        spray.y = GetSurfaceHeightAt(spray.x, spray.z);
        AssignShipSprayLocalPosition(Vector3{0.0f, -0.1f, 0.0f});
        AssignShipSprayPosition(spray);
        AssignShipSprayYaw(rotation.y);
    }
}
void Update0504Author(void *data) {
    Area0504AuthorParameter *const child = static_cast<Area0504AuthorParameter *>(data);
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    SetMapModelSlotEnabled(1, 0);
    AdvanceSurfacePhaseAndUpdateGrid(frame);
    EV_GetCharaPosition(AREA_0504_RYO, CHARACTER_TRANSLATION_ROOT, &ryoPosition0504, EventTransformFlags::Local);
    EV_GetCharaPosition(AREA_0504_SIN, CHARACTER_TRANSLATION_ROOT, &shenhuaPosition0504, EventTransformFlags::Local);
    switch (frame) {
    case 1:
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        SyncWindWithWDATWeatherState();
        snapSetDesc(1);
        break;
    case 821:
        SetNewWindPhaseAndSpeed(61708, 0.2f);
        break;
    case 1011:
        SyncWindWithWDATWeatherState();
        break;
    case 1180:
        snapSetDesc(0);
        break;
    case 1486:
        cameraRide0504 = 0;
        break;
    case 1516:
        cameraRide0504 = 1;
        break;
    case 1776:
        SetPlayerGameFlags(PlayerGameFlagMode::Bit5, nullptr);
        break;
    }
    switch (frame) {
    case 471:
    case 671:
    case 1306:
    case 2556:
        Weather0504(25486, 0.8f);
        break;
    case 541:
    case 821:
    case 1336:
    case 1451:
    case 2596:
    case 3326:
    case 3566:
        Weather0504(21845, 1.5f);
        break;
    case 1371:
        Weather0504(29127, 0.4f);
        break;
    case 3266:
        Weather0504(25486, 0.5f);
        break;
    case 3536:
        Weather0504(29127, 0.8f);
        break;
    }
    Splash0504AtFrames(frame, AREA_0504_BOAT, 100, 0.010588235f, 21, {2052});
    Splash0504AtFrames(frame, AREA_0504_SIN, 2000, 0.023137255f, 5, {1520});
    Splash0504AtFrames(frame, AREA_0504_SIN, 100, 0.010588235f, 14, {1614, 1664, 2197, 2227});
    Splash0504AtFrames(frame, AREA_0504_SIN, 100, 0.010588235f, 21, {1629, 2090, 2178, 2213});
    Splash0504AtFrames(frame, AREA_0504_RYO, 2000, 0.023137255f, 5, {1840});
    Splash0504AtFrames(frame, AREA_0504_RYO, 100, 0.010588235f, 21, {1942, 1981, 2158, 2663, 2697, 2744, 2786, 3442, 3598});
    Splash0504AtFrames(frame, AREA_0504_RYO, 100, 0.010588235f, 14, {1966, 2007, 2139, 2259, 2627, 2851, 2878, 2907});
    Splash0504AtFrames(frame, AREA_0504_RYO, 300, 0.0043137255f, 5, {2441, 2473, 2498, 2513, 2536, 3530, 3555, 3585});
    Splash0504AtFrames(frame, AREA_0504_RYO, 300, 0.010588235f, 5, {3266, 3440, 3485, 3600, 3626, 3665, 3701, 3811});
    Splash0504AtFrames(frame, AREA_0504_RYO, 500, 0.023137255f, 3, {3133});
    switch (frame) {
    case 1:
    case 1686:
    case 2026:
    case 2286:
        sprayCharacter0504 = AREA_0504_BOAT;
        break;
    case 1590:
    case 2061:
    case 2165:
        sprayCharacter0504 = AREA_0504_SIN;
        break;
    case 1928:
    case 2106:
    case 2436:
        sprayCharacter0504 = AREA_0504_RYO;
        break;
    }
    if (frame > 1850)
        Update0504CharacterHeight(AREA_0504_RYO, shen_name{});
    if (frame > 1570 || frame == 1590 || frame == 1686)
        Update0504CharacterHeight(AREA_0504_SIN, frame >= 3326 ? AREA_0504_RYO : shen_name{});
    Update0504CharacterHeight(AREA_0504_BOAT, frame > 2286 ? AREA_0504_SIN : shen_name{});
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        Restore0504Characters();
        child->parent->state = 3;
        HLib::DestroyCurrentTask();
    }
}
void Update0504CameraRideHeight() {
    bool reset = cameraRide0504 != 0 && previousCameraRide0504 == 0;
    previousCameraRide0504 = cameraRide0504;
    if (cameraRide0504 != 1 || !GetMapModelRoot(1))
        return;
    EulerRotation16 rotation{};
    camCtrlGetRot(&rotation);
    Vector3 eye{}, target{};
    camCtrlGetNextCamPos(&eye);
    camCtrlGetTargetPosition(&target);
    const Vector3 eyeDelta{.x = eye.x - previousEye0504.x, .y = eye.y - previousEye0504.y, .z = eye.z - previousEye0504.z};
    const Vector3 targetDelta{.x = target.x - previousTarget0504.x, .y = target.y - previousTarget0504.y, .z = target.z - previousTarget0504.z};
    previousEye0504 = eye;
    previousTarget0504 = target;
    if (eyeDelta.y * eyeDelta.y + eyeDelta.x * eyeDelta.x + eyeDelta.z * eyeDelta.z > 0.25f ||
        targetDelta.y * targetDelta.y + targetDelta.x * targetDelta.x + targetDelta.z * targetDelta.z > 0.25f)
        reset = true;
    Vector3 ryo{}, shenhua{};
    EV_GetCharaPosition(AREA_0504_RYO, CHARACTER_TRANSLATION_ROOT, &ryo, EventTransformFlags::Local);
    EV_GetCharaPosition(AREA_0504_SIN, CHARACTER_TRANSLATION_ROOT, &shenhua, EventTransformFlags::Local);
    const Vector3 ryoDelta{.x = ryoPosition0504.x - target.x, .y = ryoPosition0504.y - target.y, .z = ryoPosition0504.z - target.z};
    const Vector3 shenhuaDelta{.x = shenhuaPosition0504.x - target.x, .y = shenhuaPosition0504.y - target.y, .z = shenhuaPosition0504.z - target.z};
    const float ryoDistance = std::sqrt(ryoDelta.x * ryoDelta.x + ryoDelta.y * ryoDelta.y + ryoDelta.z * ryoDelta.z);
    const float shenhuaDistance = std::sqrt(shenhuaDelta.x * shenhuaDelta.x + shenhuaDelta.y * shenhuaDelta.y + shenhuaDelta.z * shenhuaDelta.z);
    const float characterHeight = shenhuaDistance <= ryoDistance ? shenhua.y - shenhuaPosition0504.y : ryo.y - ryoPosition0504.y;
    const float surface = GetSurfaceHeightAt(eye.x, eye.z);
    Vector3 translation{};
    EulerRotation16 correction{};
    camCtrlGetRotationXform(&translation, &correction);
    std::int16_t pitch = 0;
    if (surface == 9999.0f) {
        translation = {};
        correction.pitch = 0;
    } else {
        float height = 0.0f;
        if (surface + 0.30000001f > eye.y) {
            height = surface + 0.30000001f - eye.y;
            if (height > 0.0f && characterHeight > 0.2f) {
                eye.y += height;
                target.y += characterHeight;
                pitch = static_cast<std::int16_t>(
                    atan2_fixed16(-(target.y - eye.y), -std::sqrt((target.z - eye.z) * (target.z - eye.z) + (target.x - eye.x) * (target.x - eye.x))) -
                    rotation.pitch);
            }
        }
        translation.x = 0.0f;
        if (std::fabs(translation.y - height) > 1.0f)
            reset = true;
        translation.y += (height - translation.y) * 0.30000001f;
        if (reset)
            translation.y = height;
        translation.z = 0.0f;
        correction.pitch = static_cast<std::int16_t>(static_cast<std::int32_t>((pitch - correction.pitch) * 0.3f) + correction.pitch);
        if (reset)
            correction.pitch = pitch;
    }
    correction.yaw = 0;
    correction.roll = 0;
    camCtrlSetRotation(&translation, &correction);
}
void Cleanup0504(Area0504Parameter *parent) {
    for (const shen_name character : AREA_0504_CHARACTERS) {
        if (character.name == 0xffffffffU)
            break;
        if (DoesScenarioCharacterExist(character)) {
            EV_npcLockUnlock(character);
            EV_npcFree(character);
        }
    }
    camCtrlSetRotationCallback(nullptr);
    camCtrlSetRotation(nullptr, nullptr);
    EV_FreeAuthorPak(0, "0504");
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, AREA_0504_RETURN, 0);
}
const ScriptAreaDefinition area0504ScriptDefinition{AREA_0504, 0, Initialize0504, Start0504};
} // namespace shenmue::s2::areas
#endif
