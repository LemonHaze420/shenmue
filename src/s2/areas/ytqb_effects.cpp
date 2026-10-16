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
    EVT_AT_SetFaceLooks(YTQB_RYO_, 6, 15, 1, start);
}
void YtqbBackKnockdown(float angle) {
    EVT_AT_SetAngY(YTQB_RYO_, YTQB_RYO_, angle, 25);
    YtqbCamera(7915);
    EVT_AT_SetCam(7933, YTQB_DUMR, YTQB_DUMR, 0, 0, 25);
    EVT_AT_SetMot(YTQB_RYO_, YTQB_RYOM_RYO_0048E_DOWN_B_0, 2, 1, 0, 25);
    EVT_AT_SetSound(0x282, 65);
    EVT_AT_SetFaceLooks(YTQB_RYO_, 6, 15, 1, 25);
}
void YtqbSideKnockdown(float angle) {
    EVT_AT_SetAngY(YTQB_RYO_, YTQB_RYO_, angle, 25);
    YtqbCamera(7909);
    EVT_AT_SetCam(7935, YTQB_DUMR, YTQB_DUMR, 0, 0, 25);
    EVT_AT_SetFaceLooks(YTQB_RYO_, 6, 15, 1, 25);
    EVT_AT_SetMot(YTQB_RYO_, YTQB_RYOM_RYO_0048E_DOWN_SIDE_R_M, 2, 1, 0, 25);
    EVT_AT_SetSound(0x282, 70);
}
void YtqbFinalKnockdown(float angle) {
    EVT_AT_SetAngY(YTQB_RYO_, YTQB_RYO_, angle, 40);
    EVT_AT_SetCam(7934, YTQB_DUMR, YTQB_DUMR, 0, 0, 40);
    EVT_AT_SetMot(YTQB_RYO_, YTQB_RYOM_RYO_0048E_DOWN_C_0, 2, 1, 0, 40);
    EVT_AT_SetSound(0x282, 110);
    EVT_AT_SetFaceLooks(YTQB_RYO_, 6, 15, 1, 40);
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
    EVT_AT_SetFaceLooks(YTQB_RYO_, 6, 5, 1, 23);
    EVT_AT_SetFaceLooks(YTQB_RYO_, 1, 15, 1, delay);
    EVT_AT_SetVoice("S2BVA010", 1, 23);
    EVT_AT_SetSound(3366, 19);
    AnimateWater();
    HideWaterAfter(delay);
    return delay;
}
} // namespace shenmue::s2::areas
