#include "s2/duck_race.hpp"
#include "s2/character.hpp"
#include "s2/debug.hpp"
#include "s2/event_runtime.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_characters.hpp"
#include "s2/field_collision.hpp"
#include "s2/flag.hpp"
#include "s2/input.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/gauge.hpp"
#include "s2/save.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/camera.hpp"
#include "s2/legacy_hud_text.hpp"
#include "s2/particle_effects.hpp"
#include "common/math.hpp"
#include <cmath>
#include <cstring>
namespace shenmue::s2 {
DuckRaceRuntime duckRaceRuntime{};
const std::array<DuckProfile, 23> duckProfiles{{
{0.11574073880910873f,0.21759259700775146f,0.0555555559694767f,6.400000095367432f,0.30000001192092896f,0,2,50,30,1.0f,1.0f,0.009999999776482582f},
{0.10648148506879807f,0.2361111044883728f,0.0555555559694767f,4.900000095367432f,0.30000001192092896f,0,0,45,60,1.0f,1.0f,0.004000000189989805f},
    {0.12037037312984467f, 0.2083333283662796f, 0.06481481343507767f, 6.099999904632568f, 0.30000001192092896f, 0, 10, 80, 20, 1.100000023841858f, 1.0f,
        0.019999999552965164f},
{0.0972222238779068f,0.21296297013759613f,0.02777777798473835f,6.5f,0.30000001192092896f,1,4,30,20,1.100000023841858f,1.0f,0.019999999552965164f},
{0.1111111119389534f,0.19907407462596893f,0.03703703731298447f,6.099999904632568f,0.30000001192092896f,1,5,40,40,1.2000000476837158f,1.0f,0.02500000037252903f},
{0.10648148506879807f,0.1944444477558136f,0.02777777798473835f,6.199999809265137f,0.30000001192092896f,1,8,40,30,1.399999976158142f,1.0f,0.014999999664723873f},
{0.14351852238178253f,0.21296297013759613f,0.0416666679084301f,4.900000095367432f,0.30000001192092896f,2,5,90,25,1.0f,1.0f,0.03999999910593033f},
{0.1388888955116272f,0.22685185074806213f,0.032407406717538834f,5.5f,0.30000001192092896f,2,15,60,25,1.2000000476837158f,1.0f,0.019999999552965164f},
{0.13425925374031067f,0.23148147761821747f,0.03703703731298447f,6.199999809265137f,0.30000001192092896f,2,20,120,40,1.0f,1.0f,0.029999999329447746f},
{0.1388888955116272f,0.22685185074806213f,0.046296294778585434f,6.400000095367432f,0.30000001192092896f,3,6,20,60,1.100000023841858f,1.0f,0.02500000037252903f},
{0.14351852238178253f,0.2222222238779068f,0.02777777798473835f,6.5f,0.30000001192092896f,3,12,30,50,1.0f,1.0f,0.009999999776482582f},
    {0.14814814925193787f, 0.21759259700775146f, 0.06481481343507767f, 5.400000095367432f, 0.30000001192092896f, 3, 4, 45, 40, 1.2000000476837158f, 1.0f,
        0.019999999552965164f},
{0.16111111640930176f,0.2351851910352707f,0.0555555559694767f,7.0f,0.30000001192092896f,0,35,70,200,2.0f,1.0f,0.03999999910593033f},
{0.20370370149612427f,0.2361111044883728f,0.0555555559694767f,7.5f,0.30000001192092896f,3,30,40,60,1.5f,1.0f,0.029999999329447746f},
    {0.1388888955116272f, 0.2222222238779068f, 0.046296294778585434f, 6.199999809265137f, 0.30000001192092896f, 1, 40, 45, 30, 1.2999999523162842f, 1.0f,
        0.02500000037252903f},
    {0.12037037312984467f, 0.21296297013759613f, 0.03703703731298447f, 6.199999809265137f, 0.20000000298023224f, 1, 10, 45, 30, 1.2999999523162842f, 1.0f,
        0.02500000037252903f},
    {0.12037037312984467f, 0.21296297013759613f, 0.03703703731298447f, 6.199999809265137f, 0.20000000298023224f, 1, 10, 45, 30, 1.2999999523162842f, 1.0f,
        0.02500000037252903f},
    {0.12037037312984467f, 0.21296297013759613f, 0.03703703731298447f, 6.199999809265137f, 0.20000000298023224f, 1, 10, 45, 30, 1.2999999523162842f, 1.0f,
        0.02500000037252903f},
    {0.12037037312984467f, 0.21296297013759613f, 0.03703703731298447f, 6.199999809265137f, 0.20000000298023224f, 1, 10, 45, 30, 1.2999999523162842f, 1.0f,
        0.02500000037252903f},
    {0.12037037312984467f, 0.21296297013759613f, 0.03703703731298447f, 6.199999809265137f, 0.20000000298023224f, 1, 10, 45, 30, 1.2999999523162842f, 1.0f,
        0.02500000037252903f},
    {0.12037037312984467f, 0.21296297013759613f, 0.03703703731298447f, 6.199999809265137f, 0.20000000298023224f, 1, 10, 45, 30, 1.2999999523162842f, 1.0f,
        0.02500000037252903f},
{0.09259258955717087f,0.18518517911434174f,0.03703703731298447f,5.0f,0.30000001192092896f,0,0,40,30,1.0f,1.0f,0.02500000037252903f},
{0.15740740299224854f,0.25f,0.06481481343507767f,5.0f,0.30000001192092896f,0,60,80,30,1.399999976158142f,1.0f,0.02500000037252903f}
}};
const std::array<DuckCourseRange, 7> duckTurnZones{{
{{1349.0f,0.0f,747.0f},{1353.199951171875f,0.0f,752.7999877929688f}},
{{1356.5999755859375f,0.0f,746.9000244140625f},{1357.5f,0.0f,753.9000244140625f}},
{{1356.4000244140625f,0.0f,777.7999877929688f},{1352.800048828125f,0.0f,788.7999877929688f}},
{{1379.199951171875f,0.0f,799.0999755859375f},{1394.9000244140625f,0.0f,789.0999755859375f}},
{{255.8000030517578f,0.0f,310.6000061035156f},{263.6000061035156f,0.0f,312.6000061035156f}},
{{228.39999389648438f,0.0f,331.5f},{226.1999969482422f,0.0f,338.6000061035156f}},
    {{-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}}};
const std::array<std::array<Vector3, 3>, 6> duckTurnTargets{{
{{{1356.699951171875f,0.0f,748.0999755859375f},{1356.199951171875f,0.0f,750.0f},{1356.300048828125f,0.0f,751.9000244140625f}}},
{{{1364.699951171875f,0.0f,752.7999877929688f},{1364.5999755859375f,0.0f,753.7000122070312f},{1364.4000244140625f,0.0f,754.7999877929688f}}},
{{{1374.0f,0.0f,784.2999877929688f},{1376.800048828125f,0.0f,786.4000244140625f},{1370.5f,0.0f,788.9000244140625f}}},
{{{1399.0999755859375f,0.0f,797.0f},{1398.199951171875f,0.0f,799.7999877929688f},{1397.5f,0.0f,802.0999755859375f}}},
{{{257.6000061035156f,0.0f,331.20001220703125f},{259.20001220703125f,0.0f,331.29998779296875f},{259.8999938964844f,0.0f,331.3999938964844f}}},
{{{205.39999389648438f,0.0f,331.8999938964844f},{205.39999389648438f,0.0f,333.8999938964844f},{205.39999389648438f,0.0f,335.8999938964844f}}}
}};
const std::array<DuckTurnTrigger, 3> duckTurnTriggers{{{{220.3000030517578f, 109.5f, 324.8999938964844f}, 0, 4.0f},
    {{241.6999969482422f, 113.19999694824219f, 310.0f}, 0, 3.5f}, {{220.3000030517578f, 109.5f, 324.8999938964844f}, 32768, -1.0f}}};
const std::array<DuckCourseRange, 5> duckCourseSegments{{
{{1331.0999755859375f,110.0f,749.0f},{1377.5f,110.0f,763.0f}},
{{1377.5f,110.0f,763.0f},{1345.9000244140625f,110.0f,772.4000244140625f}},
{{1345.9000244140625f,110.0f,772.4000244140625f},{1371.0999755859375f,110.0f,804.7000122070312f}},
{{1371.0999755859375f,110.0f,804.7000122070312f},{1399.300048828125f,110.0f,775.5999755859375f}},
    {{194.0f, 107.5f, 304.70001220703125f}, {188.39999389648438f, 107.5f, 316.0f}}}};
namespace {
constexpr shen_name invalid{0xffffffffU}, courseBase{SHEN_NAME(JDM2)};
constexpr shen_name roster[]{SHEN_NAME(duk1), SHEN_NAME(duk2), SHEN_NAME(duk3), SHEN_NAME(duk4), SHEN_NAME(duk5), SHEN_NAME(duk6)};
constexpr shen_name markers[]{
    SHEN_NAME(DRM1), SHEN_NAME(DRM2), SHEN_NAME(DRM3), SHEN_NAME(DRM4), SHEN_NAME(DRM5), SHEN_NAME(DRM6), SHEN_NAME(DRM7), SHEN_NAME(DRM8), SHEN_NAME(DRM9)};
float FlushSmall(float value) {
    return std::fabs(value) >= 1.1920929e-7f ? value : 0.0f;
}
std::uint16_t CourseYaw(float x, float z) {
    return atan2_fixed16(-x, -z);
}
void SetActorMotion(DuckRaceActor *actor, std::int32_t motion) {
    if (actor->currentMotion == motion)
        return;
    ConfigureScenarioNodeMotion(actor->name, motion, 0, 32767, 0, 1, 0);
    ConfigureScenarioNodeMotion(actor->name, 0, -1, -1, -1, 32767, 4);
    ConfigureScenarioNodeMotion(actor->name, 0, -1, -1, -1, 1, -1);
    actor->currentMotion = motion;
}
std::uint16_t HoldOffSpeed(DuckRaceActor *actor) {
    const std::int32_t quarter = TruncateToInt32((actor->distance / duckRaceRuntime.courseLength) * 4.0f);
    return (quarter < 3 ? quarter : 3) == 3 ? 0 : static_cast<double>(actor->stamina) <= static_cast<double>(actor->maximumStamina) * .2;
}
std::uint16_t FastStartSpeed(DuckRaceActor *actor) {
    const std::int32_t distance = TruncateToInt32((actor->distance / duckRaceRuntime.courseLength) * 4.0f);
    const std::int32_t quarter = distance < 3 ? distance : 3;
    if (quarter == 0)
        return actor->stamina <= actor->maximumStamina * .75f;
    if (quarter == 1)
        return actor->stamina <= actor->maximumStamina * .5f;
    if (quarter == 2)
        return actor->stamina <= actor->maximumStamina * .25f;
    return quarter != 3;
}
std::uint16_t QuickLegsSpeed(DuckRaceActor *actor) {
    const std::int32_t quarter = TruncateToInt32((actor->distance / duckRaceRuntime.courseLength) * 4.0f);
    return (quarter < 3 ? quarter : 3) == 3 ? 0 : quarter == 2 ? static_cast<double>(actor->stamina) <= static_cast<double>(actor->maximumStamina) * .5 : 1;
}
std::uint16_t HomeStretchSpeed(DuckRaceActor *actor) {
    if (TruncateToInt32((actor->distance / duckRaceRuntime.courseLength) * 4.0f) >= 2)
        return 0;
    return actor->stamina >= actor->maximumStamina ? 1 : 2;
}
using DuckStyleCallback = std::uint16_t (*)(DuckRaceActor *);
constexpr DuckStyleCallback styles[]{HoldOffSpeed, FastStartSpeed, QuickLegsSpeed, HomeStretchSpeed, HoldOffSpeed};
}

// @verified - p
std::int32_t duckSetPlayerOverlayDisabled(std::int32_t disabled) {
    const std::int32_t previous = duckRaceRuntime.playerOverlayDisabled;
    duckRaceRuntime.playerOverlayDisabled = disabled;
    return previous;
}

void ResetDuckRaceRuntime() {
    duckRaceRuntime.actors.fill(nullptr);
    duckRaceRuntime.names.fill(shen_name{});
    duckRaceRuntime.finishNames.fill(shen_name{});
    duckRaceRuntime.finishTicks.fill(0);
    std::memset(duckRaceRuntime.replay.data(), 0, sizeof(duckRaceRuntime.replay));
    duckRaceRuntime.speedExperience = duckRaceRuntime.staminaExperience = duckRaceRuntime.gutsExperience = duckRaceRuntime.turnExperience = 0;
    duckRaceRuntime.courseLength = 0;
    duckRaceRuntime.staminaScale = 0;
    duckRaceRuntime.actorCount = 0;
    duckRaceRuntime.finishCount = 0;
    duckRaceRuntime.playerOverlayDisabled = 0;
}
void InitializeDuckRaceActor(DuckRaceActor *actor, std::int32_t profile) {
    constexpr shen_name special[]{SHEN_NAME(peng), SHEN_NAME(KAMO), SHEN_NAME(KAMM), SHEN_NAME(KAK0), SHEN_NAME(KAK1), SHEN_NAME(KAK2), SHEN_NAME(KAK3),
        SHEN_NAME(KAK4), SHEN_NAME(KAK5), SHEN_NAME(duk1)};
    actor->name = profile >= 12 && profile <= 21 ? special[profile - 12] : roster[duckRaceRuntime.actorCount];
    const DuckProfile &definition = duckProfiles[profile];
    actor->normalSpeed = definition.speedNormal;
    actor->maximumSpeed = definition.speedMax;
    actor->minimumSpeed = definition.speedMin;
    actor->probeBackoff = definition.obstacleProbeBackoff;
    actor->separationRadius = definition.separationRadiusBase;
    actor->style = definition.aiStyleId;
    actor->guts = definition.guts;
    actor->collisionStaminaCost = definition.collisionCost;
    actor->profile = static_cast<std::uint16_t>(profile);
    actor->behavior = 1;
    actor->handling = definition.handling;
    actor->steerTimer = 0;
    actor->speedTier = 1;
    actor->maximumStamina = TruncateToInt32(duckRaceRuntime.staminaScale * definition.staminaBase);
    actor->stamina = actor->maximumStamina;
    actor->aiControlled = 1;
    actor->speed = actor->normalSpeed;
    actor->flags = 0;
    actor->sizeScale = definition.sizeScale;
    actor->speedResponse = definition.speedResponse;
    actor->soundCooldown = 0;
    actor->collisionSlowTimer = actor->turnLockTimer = actor->actionTimer = 0;
    actor->distance = 0;
    actor->finishTriggerSeen = 0;
    actor->separationRadius *= actor->sizeScale;
    const Vector3 scale{actor->sizeScale, actor->sizeScale, actor->sizeScale};
    SetMotionworkFlagForCharaById(actor->name, &scale);
    EV_EnableCharacterDisplay(actor->name);
    const std::int32_t motion = profile == 12 ? 32544 :
        profile == 13 || profile == 14        ? 32542 :
        profile >= 15 && profile <= 20        ? 32540 :
        shenGetRandomInt32(2)                 ? 32527 :
                                                32526;
    SetActorMotion(actor, motion);
}
void UpdateDuckRaceActorMotion(DuckRaceActor *actor) {
    if (!(static_cast<double>(actor->speed) > .000925925925925926)) {
        SetActorMotion(actor,
            actor->profile == 12                             ? 32544 :
                actor->profile == 13 || actor->profile == 14 ? 32542 :
                actor->profile >= 15 && actor->profile <= 20 ? 32540 :
                                                               32527);
    } else if (actor->flags & 0x18) {
        SetActorMotion(actor,
            actor->profile == 12                             ? 32545 :
                actor->profile == 13 || actor->profile == 14 ? 32543 :
                actor->profile >= 15 && actor->profile <= 20 ? 32541 :
                                                               32534);
    } else {
        SetActorMotion(actor,
            actor->profile == 12                             ? 32545 :
                actor->profile == 13 || actor->profile == 14 ? 32543 :
                actor->profile >= 15 && actor->profile <= 20 ? 32541 :
                                                               32525);
        ConfigureScenarioNodeMotion(actor->name, 0, -1, -1, -1, actor->speed * 10.0f + .1f, -1);
    }
}
void UpdateDuckRaceAiSpeed(DuckRaceActor *actor) {
    if (actor->flags & 0x18) {
        if (actor->flags & 8) {
            const std::int32_t old = actor->collisionSlowTimer--;
            if (old >= 0)
                actor->speedTier = 2;
            else
                actor->flags &= ~8U;
        }
        if (actor->flags & 16) {
            const std::int32_t old = actor->turnLockTimer--;
            if (old < 0)
                actor->flags &= ~16U;
        }
    } else if (actor->flags & 1) {
        if (static_cast<double>(actor->stamina) > static_cast<double>(actor->maximumStamina) * .5)
            actor->flags &= ~1U;
        actor->speedTier = 2;
    } else {
        actor->speedTier = styles[actor->style](actor);
        if (actor->speedTier != 2 && actor->stamina <= 0) {
            actor->flags |= 1;
            actor->speedTier = 2;
        }
    }
    const float normal = actor->normalSpeed, range = actor->maximumSpeed - normal, current = actor->speed;
    const std::int32_t staminaTick = static_cast<double>(current) > static_cast<double>(range) * .75 + normal ? -2 :
        static_cast<double>(current) > static_cast<double>(range) * .5 + normal                               ? -1 :
        current > normal                                                                                      ? 0 :
        !(static_cast<double>(current) > static_cast<double>(normal) * .5)                                    ? 2 :
                                                                                                                1;
    if (actor->flags & 64)
        actor->speed += (normal - current) * (actor->speedResponse * .1f);
    else {
        std::int32_t stamina = actor->stamina + staminaTick;
        if (stamina < 0)
            stamina = 0;
        if (stamina >= actor->maximumStamina)
            stamina = actor->maximumStamina;
        actor->stamina = stamina;
        const float target = actor->speedTier == 0 ? actor->maximumSpeed : actor->speedTier == 1 ? actor->normalSpeed : actor->minimumSpeed;
        actor->speed += (target - current) * actor->speedResponse;
    }
}
void UpdateDuckRacePlayerSpeed(DuckRaceActor *actor) {
    if (actor->flags & 8) {
        const std::int32_t old = actor->collisionSlowTimer--;
        if (old >= 0)
            actor->speed += (actor->minimumSpeed - actor->speed) * .04f;
        else
            actor->flags &= ~8U;
    } else if (actor->flags & 16) {
        const std::int32_t old = actor->turnLockTimer--;
        if (old < 0)
            actor->flags &= ~16U;
    } else if (!(actor->flags & 64)) {
        if (actor->flags & 1) {
            const std::int32_t old = actor->stamina++;
            if (actor->maximumStamina * .4f > old)
                actor->speed = std::fmax(actor->minimumSpeed, actor->speed - .0018518518f);
            else
                actor->flags &= ~1U;
        } else {
            const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, 3));
            const float amount = input / 255.0f;
            const float target = !input ? 0.0f :
                amount <= .5f           ? (actor->normalSpeed - actor->minimumSpeed) * (amount + amount) + actor->normalSpeed :
                                          (actor->maximumSpeed - actor->normalSpeed) * ((amount - .5f) + (amount - .5f)) + actor->normalSpeed;
            actor->speed = std::fmax(0.0f, (target - actor->speed) * actor->speedResponse + actor->speed);
            if (actor->maximumSpeed <= actor->speed)
                actor->speed = actor->maximumSpeed;
            const float normal = actor->normalSpeed, range = actor->maximumSpeed - normal, current = actor->speed;
            if (static_cast<double>(current) > static_cast<double>(range) * .75 + normal)
                ++duckRaceRuntime.speedExperience;
            if (current > normal && static_cast<double>(range) * .25 + normal > current)
                ++duckRaceRuntime.staminaExperience;
            const std::int32_t delta = static_cast<double>(current) > static_cast<double>(range) * .75 + normal ? -3 :
                static_cast<double>(current) > static_cast<double>(range) * .5 + normal                         ? -1 :
                current > normal                                                                                ? 0 :
                static_cast<double>(current) > static_cast<double>(normal) * .5                                 ? 1 :
                                                                                                                  3;
            std::int32_t stamina = actor->stamina + delta;
            if (stamina < 0)
                stamina = 0;
            if (stamina >= actor->maximumStamina)
                stamina = actor->maximumStamina;
            actor->stamina = stamina;
            if (!stamina)
                actor->flags |= 1;
        }
    }
    const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, 0));
    const float response = FlushSmall(((.19444445f / actor->speed + 1.0f) * actor->handling) * .01f);
    std::uint32_t turn = static_cast<std::uint16_t>(TruncateToInt32(response * 8192.0f / 45.0f));
    if (turn > 455)
        turn = 455;
    if ((input & 0x4000) && !(actor->flags & 16)) {
        actor->rotation.y += turn;
        actor->speed = std::fmax(.027777778f, actor->speed);
    }
    if (input < 0 && !(actor->flags & 16)) {
        actor->rotation.y -= turn;
        actor->speed = std::fmax(.027777778f, actor->speed);
    }
}
void AdvanceDuckRaceActor(DuckRaceActor *actor) {
    actor->position.x -= sin_fixed16(static_cast<std::int16_t>(actor->rotation.y)) * actor->speed;
    actor->position.z -= cos_fixed16(static_cast<std::uint16_t>(actor->rotation.y)) * actor->speed;
    Vector3 destination{(actor->speed * 12.0f - actor->probeBackoff) * sin_fixed16(static_cast<std::int16_t>(actor->rotation.y)) + actor->position.x,
        actor->position.y + 2.0f, (actor->speed * 12.0f - actor->probeBackoff) * cos_fixed16(actor->rotation.y) + actor->position.z};
    Vector3 origin{sin_fixed16(static_cast<std::int16_t>(actor->rotation.y)) * .3f + actor->position.x, actor->position.y + 2.0f,
        cos_fixed16(actor->rotation.y) * .3f + actor->position.z};
    if (actor->behavior != 4) {
        if (!actor->aiControlled)
            return;
        if (actor->behavior != 8 && actor->behavior != 2) {
            if (actor->steerTimer) {
                float response = actor->flags & 128 ? .25f : .08f;
                if (actor->profile == 12)
                    response += .08f;
                const std::int16_t next = static_cast<std::int16_t>(TruncateToInt32(
                    static_cast<std::int16_t>(static_cast<std::uint16_t>(actor->targetYaw) - static_cast<std::uint16_t>(actor->rotation.y)) * response +
                    actor->rotation.y));
                actor->rotation.y = next;
                if (std::fabs(static_cast<float>(static_cast<std::int16_t>(next - static_cast<std::uint16_t>(actor->targetYaw)))) < 910) {
                    actor->steerTimer = 0;
                    actor->flags &= ~32U;
                } else
                    --actor->steerTimer;
            } else {
                Vector3 intersection;
                if (FindNearestRayIntersection(&intersection, &destination, &origin) && !(actor->flags & 0x18)) {
                    Vector3s normal;
                    transformIntersectionResult(normal);
                    const std::int16_t side = static_cast<std::int16_t>(normal.y - static_cast<std::uint16_t>(actor->rotation.y)) >= 0 ? -16384 : 16384;
                    actor->targetYaw = static_cast<std::int16_t>(normal.y + side);
                    actor->steerTimer = actor->handling + shenGetRandomInt32(30);
                    actor->flags |= 32;
                }
            }
        }
    }
    if (actor->aiControlled)
        actor->flags &= ~128U;
}
void UpdateDuckRaceTurnZones(DuckRaceActor *actor) {
    bool inside = false;
    for (std::size_t index = 0; duckTurnZones[index].first.x > 0; ++index) {
        if (TestCharaPositionWithinRange(actor->name, &duckTurnZones[index].first, &duckTurnZones[index].second, shen_name{})) {
            inside = true;
            if (!(actor->flags & 2) && actor->behavior == 1) {
                const Vector3 &target = duckTurnTargets[index][shenGetRandomInt32(3)];
                actor->targetYaw = CourseYaw(target.x - actor->position.x, target.z - actor->position.z);
                actor->flags |= 2;
                actor->steerTimer = actor->handling * 2;
            }
        }
    }
    if (!inside)
        actor->flags &= ~2U;
}
void AvoidDuckRaceCourseMarkers(DuckRaceActor *actor) {
    for (shen_name name : markers) {
        Vector3 position;
        EV_GetCharaPosition(name, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        const float radius = actor->separationRadius + .4f;
        const float x = position.x - actor->position.x, y = position.y - actor->position.y, z = position.z - actor->position.z;
        const float distance = std::sqrt(x * x + y * y + z * z);
        if (radius > distance) {
            actor->steerTimer = 0;
            Vector3 offset{0, 0, radius};
            Vector3i rotation{0, CourseYaw(x, z), 0};
            ApplyScenarioTranslationAndRotation(&position, &rotation, &offset);
            actor->position.x = offset.x;
            actor->position.z = offset.z;
            float response = .2f;
            if (actor->aiControlled) {
                if (static_cast<std::int16_t>(static_cast<std::uint16_t>(rotation.y) - static_cast<std::uint16_t>(actor->rotation.y)) >= 0)
                    rotation.y -= 0x4000;
                else
                    rotation.y += 0x4000;
                const std::int16_t difference =
                    static_cast<std::int16_t>(static_cast<std::uint16_t>(rotation.y) - static_cast<std::uint16_t>(actor->rotation.y));
                actor->rotation.y = static_cast<std::int16_t>(TruncateToInt32(difference * .05f + actor->rotation.y));
                response = .07f;
            }
            actor->speed += (actor->minimumSpeed - actor->speed) * response;
        }
    }
}
void CheckDuckRaceCourseBounds(DuckRaceActor *actor) {
    AT_SetLocalBaseChara(actor->name, courseBase);
    Vector3 basePosition, oldPosition, position;
    Vector3i baseRotation;
    EV_GetCharaPosition(courseBase, CHARACTER_TRANSLATION_ROOT, &basePosition, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(courseBase, &baseRotation, EventTransformFlags::World);
    EV_GetCharaPosition(actor->name, CHARACTER_TRANSLATION_ROOT, &oldPosition, EventTransformFlags::Local);
    EV_AT_SetPos(actor->name, EventTransformFlags::World | EventTransformFlags::AllAxes, actor->position);
    EV_GetCharaPosition(actor->name, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    if (position.z >= .5f || position.z <= -.5f - actor->separationRadius || position.x <= -3.0f - actor->separationRadius ||
        actor->separationRadius + 3.0f <= position.x) {
        if (actor->flags & 4) {
            if (actor->speed <= .1851851851851852f || (actor->flags & 0x18))
                actor->steerTimer = 0;
            else {
                actor->behavior = 4;
                actor->actionTimer = 0;
                actor->steerTimer = 0;
                actor->targetYaw = baseRotation.y;
            }
            actor->flags &= ~4U;
        }
    } else
        actor->flags |= 4;
    AT_SetLocalBaseChara(actor->name, invalid);
}
void UpdateDuckRaceTurnExperience(DuckRaceActor *actor) {
    actor->position.y += .2f;
    Vector3 position;
    if (RaycastInWorldGrid(position, actor->position, .3f)) {
        actor->flags &= ~16U;
        actor->position = position;
        actor->turnLockTimer = -1;
        if (!(actor->flags & 32))
            actor->steerTimer = 0;
        if (actor->aiControlled || (actor->flags & 128))
            actor->flags |= 128;
        else
            ++duckRaceRuntime.turnExperience;
        if (actor->speed > .027777778f && !actor->aiControlled)
            actor->speed += (0.0f - actor->speed) * .08f;
    } else if (!actor->aiControlled)
        actor->behavior &= ~128U;
    actor->position.y -= .2f;
}
void CheckDuckRaceTurnTriggers(DuckRaceActor *actor) {
    for (std::size_t index = 0; duckTurnTriggers[index].valid > 0; ++index) {
        const DuckTurnTrigger &trigger = duckTurnTriggers[index];
        if (ComputeAngle(trigger.position, 3.0f, 0, 0x8000, actor->position) &&
            std::fabs(static_cast<float>(static_cast<std::int16_t>(static_cast<std::uint16_t>(trigger.yaw) - static_cast<std::uint16_t>(actor->rotation.y)))) >
                21845) {
            actor->behavior = 8;
            actor->targetYaw = trigger.yaw;
        }
    }
}
void UpdateDuckRaceTurnRecovery(DuckRaceActor *actor) {
    if (!(actor->speed > .027777778f)) {
        const std::int16_t difference = static_cast<std::int16_t>(static_cast<std::uint16_t>(actor->targetYaw) - static_cast<std::uint16_t>(actor->rotation.y));
        const float target = difference * .03f + actor->rotation.y;
        actor->rotation.y = static_cast<std::int16_t>(TruncateToInt32(target));
        if (std::fabs(static_cast<float>(static_cast<std::int16_t>(actor->rotation.y - static_cast<std::uint16_t>(actor->targetYaw)))) < 910) {
            actor->behavior = 1;
            actor->steerTimer = 0;
            actor->flags = 0;
        }
    } else
        actor->speed -= .0037037036f;
}
void UpdateDuckRaceHopRecovery(DuckRaceActor *actor) {
    Vector3 position;
    if (RaycastInWorldGrid(position, actor->position, .3f))
        actor->position = position;
    ++actor->actionTimer;
    actor->speed = std::fmin(actor->speed + .00064814813f, .25f);
    if (actor->actionTimer < 20) {
        if (actor->aiControlled)
            actor->rotation.y = static_cast<std::int16_t>(
                TruncateToInt32(static_cast<std::int16_t>(static_cast<std::uint16_t>(actor->targetYaw) - static_cast<std::uint16_t>(actor->rotation.y)) * .15f +
                    actor->rotation.y));
        const float lift =
            FlushSmall((1.0f - sin_fixed16(static_cast<std::int16_t>(TruncateToInt32(actor->actionTimer * .33333334f * 8192.0f / 45.0f)))) * .008f);
        actor->position.y += lift;
    } else {
        actor->flags |= 64;
        actor->actionTimer = 45;
        actor->behavior = 1;
    }
}
void UpdateDuckRacePlayerHopInput(DuckRaceActor *actor) {
    const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, 0));
    if (input & 0x4000) {
        actor->rotation.y += 254;
        actor->speed = std::fmax(.027777778f, actor->speed);
    }
    if (input < 0) {
        actor->rotation.y -= 254;
        actor->speed = std::fmax(.027777778f, actor->speed);
    }
    if (EVT_GetControllerInput(0, 3))
        actor->stamina -= 3;
    else {
        actor->flags |= 64;
        actor->actionTimer = 45;
        actor->behavior = 1;
    }
    if (actor->stamina <= 0) {
        actor->flags |= 64;
        actor->actionTimer = 45;
        actor->behavior = 1;
    }
}
std::int16_t FindDuckRaceCourseSegment(shen_name name) {
    for (std::int16_t index = 0; index < 5; ++index)
        if (TestCharaPositionWithinRange(name, &duckCourseSegments[index].first, &duckCourseSegments[index].second, shen_name{}))
            return index;
    return -1;
}
std::uint32_t CalculateDuckRaceExperience(std::uint32_t increment) {
    const std::int32_t total = static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 50) + EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 51) +
        EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 52) + EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 53));
    if (static_cast<std::uint32_t>(total) + increment < 800)
        return increment;
    return total <= 800 ? 800 - total : 0;
}
void ResolveDuckRaceActorCollisions(DuckRaceActor *actor) {
    UpdateDuckRaceTurnExperience(actor);
    for (std::int32_t index = 0; index < duckRaceRuntime.actorCount; ++index) {
        DuckRaceActor *other = static_cast<DuckRaceActor *>(HLib::GetTaskParameter(duckRaceRuntime.actors[index]));
        if (actor->name.name == other->name.name)
            continue;
        const float x = other->position.x - actor->position.x, y = other->position.y - actor->position.y, z = other->position.z - actor->position.z;
        const float distance = std::sqrt(x * x + y * y + z * z), radius = actor->separationRadius + other->separationRadius;
        if (!(radius > distance))
            continue;
        const float displacement = FlushSmall((radius - distance) * .5f);
        Vector3 first{0, 0, displacement}, second{0, 0, -displacement};
        Vector3i rotation{0, CourseYaw(x, z), 0};
        ApplyScenarioTranslationAndRotation(&actor->position, &rotation, &first);
        ApplyScenarioTranslationAndRotation(&other->position, &rotation, &second);
        actor->position = first;
        other->position = second;
        EV_AT_SetPos(other->name, EventTransformFlags::World | EventTransformFlags::AllAxes, other->position);
        DuckRaceActor *approaching = nullptr;
        DuckRaceActor *obstacle = other;
        if (ComputeAngle(actor->position, 1.0f, static_cast<std::int16_t>(actor->rotation.y), 0x2000, other->position))
            approaching = actor;
        else if (ComputeAngle(other->position, 1.0f, static_cast<std::int16_t>(other->rotation.y), 0x2000, actor->position)) {
            approaching = other;
            obstacle = actor;
        }
        if (!approaching)
            continue;
        EV_GetCharaPosition(other->name, CHARACTER_TRANSLATION_ROOT, &other->position, EventTransformFlags::World);
        const float angle = std::fabs(static_cast<float>(
            static_cast<std::int16_t>(static_cast<std::uint16_t>(approaching->rotation.y) - static_cast<std::uint16_t>(obstacle->rotation.y))));
        if (!(angle < 8192))
            continue;
        if (approaching->guts <= obstacle->guts) {
            if (approaching->speed > .074074075f) {
                if (!actor->aiControlled)
                    duckRaceRuntime.gutsExperience += approaching->aiControlled ? 1 : 4;
                if (!actor->soundCooldown) {
                    Loop3DSoundEffect(0xbfb, approaching->speed);
                    actor->soundCooldown = 120;
                }
                approaching->steerTimer = approaching->handling;
                approaching->stamina -= approaching->collisionStaminaCost;
                approaching->flags |= 8;
                approaching->collisionSlowTimer = 30;
                approaching->speed = (approaching->speed - approaching->minimumSpeed) * .7f;
            }
        } else if (!(obstacle->flags & 16)) {
            if (!actor->aiControlled)
                duckRaceRuntime.gutsExperience += obstacle->aiControlled ? 1 : 4;
            if (!actor->soundCooldown) {
                Loop3DSoundEffect(0xbfb, angle);
                actor->soundCooldown = 120;
            }
            const std::uint16_t yaw = CourseYaw(approaching->position.x - obstacle->position.x, approaching->position.z - obstacle->position.z);
            if (static_cast<std::int16_t>(yaw - static_cast<std::uint16_t>(obstacle->rotation.y)) >= 0)
                obstacle->rotation.y -= 3640;
            else
                obstacle->rotation.y += 3640;
            EV_AT_SetAngY(obstacle->name, EventTransformFlags::World | EventTransformFlags::AllAxes, obstacle->rotation);
            obstacle->stamina -= obstacle->collisionStaminaCost;
            obstacle->flags |= 16;
            obstacle->steerTimer = 0;
            obstacle->turnLockTimer = 30;
        }
        if (approaching->stamina < 0)
            approaching->stamina = 0;
        if (obstacle->stamina < 0)
            obstacle->stamina = 0;
    }
    UpdateDuckRaceTurnExperience(actor);
    if (actor->behavior != 4) {
        actor->position.y += .5f;
        const float ground = shenFindIntersectionWithScene(actor->position, actor->position.y);
        const float offset = actor->sizeScale * .15000001f;
        actor->position.y -= .5f;
        const float difference = ground - actor->position.y + offset;
        if (ground > 0) {
            if (difference < -.05f && (actor->flags & 64)) {
                const std::int32_t timer = actor->actionTimer;
                if (timer <= 30)
                    actor->position.y -= std::fmin(.8f, (45 - timer) * .002f);
                actor->actionTimer = timer - 1;
                if (actor->actionTimer < 0)
                    actor->actionTimer = 0;
            } else if ((difference > .4f && (actor->flags & 64)) || difference < -.2f)
                actor->position.y -= .2f;
            else if (difference < .4f) {
                actor->flags &= ~64U;
                actor->position.y = offset + ground;
            }
        }
    }
}
void UpdateDuckRaceActorExperience(DuckRaceActor *actor) {
    if (FindDuckRaceCourseSegment(actor->name) != 4 || (actor->behavior & 2) || (actor->distance / duckRaceRuntime.courseLength) * 4.0f < 3 ||
        !actor->finishTriggerSeen)
        return;
    if (!duckRaceRuntime.finishCount)
        QueueDuckRaceReplay();
    actor->behavior = 2;
    actor->replaySlot = duckRaceRuntime.finishCount;
    duckRaceRuntime.finishNames[actor->replaySlot] = actor->name;
    duckRaceRuntime.finishTicks[actor->replaySlot] = duckRaceRuntime.ticks;
    if (!actor->aiControlled) {
        const std::int32_t values[]{static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 50)),
            static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 51)),
            static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 52)),
            static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 53))};
        const std::int32_t placement = 6 - duckRaceRuntime.finishCount;
        std::int32_t speed = duckRaceRuntime.speedExperience / 70, stamina = duckRaceRuntime.staminaExperience / 120;
        std::int32_t guts = duckRaceRuntime.gutsExperience / 4, turn = 10 - duckRaceRuntime.turnExperience / 20;
        if (speed > 10)
            speed = 10;
        if (stamina > 10)
            stamina = 10;
        if (guts > 10)
            guts = 10;
        if (turn < 0)
            turn = 0;
        const std::int32_t increments[]{speed, stamina, guts, turn};
        std::int32_t levels[4]{};
        for (std::int32_t index = 0; index < 4; ++index) {
            const std::int32_t updated = values[index] + static_cast<std::int32_t>(CalculateDuckRaceExperience(increments[index] + placement));
            levels[index] = updated >= 255 ? 255 : updated;
            EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, static_cast<std::int16_t>(50 + index), levels[index]);
        }
        EV_debug_log("DUCK\\eDUCK_et_duck_ctrl.c", 1562, "SpeedExp   = %04d : LV %03d\n", duckRaceRuntime.speedExperience, levels[0]);
        EV_debug_log("DUCK\\eDUCK_et_duck_ctrl.c", 1563, "StaminaExp = %04d : LV %03d\n", duckRaceRuntime.staminaExperience, levels[1]);
        EV_debug_log("DUCK\\eDUCK_et_duck_ctrl.c", 1564, "GutsExp    = %04d : LV %03d\n", duckRaceRuntime.gutsExperience, levels[2]);
        EV_debug_log("DUCK\\eDUCK_et_duck_ctrl.c", 1565, "TurnExp    = %04d : LV %03d\n", duckRaceRuntime.turnExperience, levels[3]);
        EV_debug_log("DUCK\\eDUCK_et_duck_ctrl.c", 1566, "Total = %d\n", levels[0] + levels[1] + levels[2] + levels[3]);
        duckRaceRuntime.experiencePublished = 1;
    }
    ++duckRaceRuntime.finishCount;
}
void UpdateDuckRaceReplayActor(DuckRaceActor *actor) {
    Vector3 position;
    if (RaycastInWorldGrid(position, actor->position, .3f))
        actor->position = position;
    if (FindDuckRaceCourseSegment(actor->name) == -1)
        actor->speed -= .0027777778f;
    else if (actor->speed > .018518519f)
        actor->speed += (.027777778f - actor->speed) * .02f;
    if (actor->speed < 0)
        actor->speed = 0;
    SetDuckRaceTimerCaption(nullptr);
}
void TickDuckRaceActor(DuckRaceActor *actor) {
    EV_GetCharaPosition(actor->name, CHARACTER_TRANSLATION_ROOT, &actor->position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(actor->name, &actor->rotation, EventTransformFlags::World);
    std::array<DuckReplaySample, 90> &replay = duckRaceRuntime.replay[actor->index];
    for (std::int32_t index = 89; index > 0; --index) {
        replay[index] = replay[index - 1];
    }
    replay[0].motion = actor->currentMotion;
    replay[0].position = actor->position;
    replay[0].yawDegrees = actor->rotation.y / 182.04445f;
    CopyScenarioNodeMotionTimes(actor->name, nullptr, nullptr, &replay[0].motionFrame, nullptr);
    switch (actor->behavior) {
    case 2:
        UpdateDuckRacePlacementText(actor->replaySlot, actor->index, duckRaceRuntime.finishTicks[actor->replaySlot]);
        AdvanceDuckRaceActor(actor);
        ResolveDuckRaceActorCollisions(actor);
        UpdateDuckRaceReplayActor(actor);
        break;
    case 4:
        UpdateDuckRaceHopRecovery(actor);
        AdvanceDuckRaceActor(actor);
        if (!actor->aiControlled)
            UpdateDuckRacePlayerHopInput(actor);
        UpdateDuckRaceActorExperience(actor);
        break;
    case 8:
        AdvanceDuckRaceActor(actor);
        UpdateDuckRaceTurnRecovery(actor);
        ResolveDuckRaceActorCollisions(actor);
        break;
    default:
        if (actor->aiControlled)
            UpdateDuckRaceAiSpeed(actor);
        else
            UpdateDuckRacePlayerSpeed(actor);
        AdvanceDuckRaceActor(actor);
        if (actor->aiControlled)
            UpdateDuckRaceTurnZones(actor);
        AvoidDuckRaceCourseMarkers(actor);
        CheckDuckRaceCourseBounds(actor);
        ResolveDuckRaceActorCollisions(actor);
        CheckDuckRaceTurnTriggers(actor);
        UpdateDuckRaceActorExperience(actor);
        break;
    }
    if (!actor->aiControlled && !duckRaceRuntime.playerOverlayDisabled) {
        SetFightGaugeVisible(1);
        UpdateSingleFightGauge(static_cast<float>(actor->stamina) * 100.0f / static_cast<float>(actor->maximumStamina));
    }
    EV_AT_SetPos(actor->name, EventTransformFlags::World | EventTransformFlags::AllAxes, actor->position);
    EV_AT_SetAngY(actor->name, EventTransformFlags::World | EventTransformFlags::AllAxes, actor->rotation);
    UpdateDuckRaceDust(actor->index, actor->speed);
    UpdateDuckRaceActorMotion(actor);
    if (actor->soundCooldown)
        --actor->soundCooldown;
    if (actor->behavior != 2)
        actor->distance += actor->speed;
    if (ComputeAngle(Vector3{259.60001f, 115.1f, 323.5f}, 4.0f, 0, 0x8000, actor->position))
        actor->finishTriggerSeen = 1;
}
void SnapDuckRaceCameraTarget() {
    const auto name = duckRaceRuntime.names[duckRaceRuntime.followedActor];
    Vector3i rotation{};
    EV_GetCharaPosition(name, CHARACTER_TRANSLATION_ROOT, &duckRaceRuntime.cameraTarget, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(name, &rotation, EventTransformFlags::World);
    duckRaceRuntime.cameraProbe = {0.0f, 1.0f, 1.0f};
    ApplyScenarioTranslationAndRotation(&duckRaceRuntime.cameraTarget, &rotation, &duckRaceRuntime.cameraProbe);
}
void SetDuckRacePreviewCameraTarget(std::int32_t actor) {
    duckRaceRuntime.followedActor = actor % duckRaceRuntime.actorCount;
    SnapDuckRaceCameraTarget();
}
void UpdateDuckRaceFollowCamera() {
    const auto name = duckRaceRuntime.names[duckRaceRuntime.followedActor];
    Vector3 position{}, offset{0.0f, 1.0f, 1.0f};
    Vector3i rotation{};
    EV_GetCharaPosition(name, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(name, &rotation, EventTransformFlags::World);
    auto &target = duckRaceRuntime.cameraTarget;
    target.x += (position.x - target.x) * 0.3f;
    target.z += (position.z - target.z) * 0.3f;
    target.y += (position.y - target.y) * 0.3f;
    ApplyScenarioTranslationAndRotation(&position, &rotation, &offset);
    constexpr float factors[]{0.1f, 0.1f, 0.5f, 0.45454543828964233f};
    const auto factor = factors[duckRaceRuntime.cameraIndex];
    auto &probe = duckRaceRuntime.cameraProbe;
    probe.x += (offset.x - probe.x) * factor;
    probe.y += (offset.y - probe.y) * factor;
    probe.z += (offset.z - probe.z) * factor;
    const auto height = shenFindIntersectionWithScene(probe, probe.y);
    if (height > 0.0f && probe.y - height < 0.3f)
        probe.y = height + 0.3f;
    camCtrlSetState(CameraState::Callback);
    camCtrlSetNextPosition(probe);
    camCtrlSetTargetPosition(target);
    camCtrlSetFov(18204);
}
void CreateDuckRaceDust() {
    for (auto &effect : duckRaceRuntime.dust)
        effect = static_cast<std::uint32_t>(
            CreateFallingParticleEffect(Vector3{}, Vector3{0.7f, 0.35f, 0.7f}, 100, 0.01f, shen_name{0}, "misc", "WING.pvr", 100.0f));
    EV_debug_log("DUCK\\eDUCK_et_duck_effect.c", 70, "CreateInitialize Leaf\n");
}
void UpdateDuckRaceDust(std::int32_t actor, float speed) {
    constexpr float threshold = 0.09259258955717087f;
    const float rate = threshold > speed ? 0.0f : (speed - threshold) / 15.0f;
    Vector3 position{0.0f, 0.15f, 0.0f}, volume{0.15f, 0.1f, 0.15f};
    EV_GetCharaPosition(duckRaceRuntime.names[actor], CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    const auto effect = duckRaceRuntime.dust[actor];
    SetParticleEffectPosition(3, effect, &position, &volume);
    SetParticleEffectVelocity(3, effect, Vector3{0.05f, 0.04f, 0.05f}, Vector3{0.004f, 0.005f, 0.004f});
    SetParticleEffectBirthRate(3, effect, rate);
    SetParticleEffectSize(3, effect, 0.03f, 0.08f);
    ResumeParticleEffect(3, effect);
}
void UpdateDuckRacePlacementText(std::int32_t replaySlot, std::int32_t actor, std::int32_t ticks) {
    if (duckRaceRuntime.playerOverlayDisabled == 1)
        return;
    auto *object = duckRaceRuntime.placementText[replaySlot];
    object->depth = 8.0f;
    auto &text = object->text;
    text[0] = static_cast<char>(ticks / 1800 + '0');
    text[1] = ':';
    text[4] = ':';
    text[7] = 0;
    const auto seconds = ticks / 30 % 60, hundredths = 100 * (ticks % 30) / 30;
    text[2] = static_cast<char>(seconds / 10 + '0');
    text[3] = static_cast<char>(seconds % 10 + '0');
    text[5] = static_cast<char>(hundredths / 10 + '0');
    text[6] = static_cast<char>(hundredths % 10 + '0');
    object->colour = 0xffffffffU;
    if (replaySlot == 6) {
        object->x = 520.0f;
        object->y = 420.0f;
        SetDuckRaceTimerCaption(text.data());
    } else {
        object->x = 580.0f;
        object->y = static_cast<float>(replaySlot) * 20.0f + 160.0f;
        object->scaleX = object->scaleY = 0.5f;
        SetDuckRacePlacementRow(replaySlot, text.data(), actor);
    }
}
namespace {
struct DuckReplayWork {
    HLib::Task *self;
    std::int32_t state, timer;
};
static_assert(sizeof(DuckReplayWork) == 0x10);
void SetReplayCamera(std::int32_t camera) {
    EventSetCamera event{0, camera, invalid, invalid, 0, {}, 0.0f, 0.0f};
    EVT_AT_SetCam_Callback(&event);
    camCtrlDisableFilmBars();
}
void DuckReplay_Callback(void *data) {
    auto *work = static_cast<DuckReplayWork *>(data);
    if (work->state == 0) {
        duckRaceRuntime.followCamera = 0;
        SetReplayCamera(3000);
        ++work->state;
    }
    if (work->state >= 1 && work->state <= 3) {
        if (work->timer-- <= 0) {
            if (work->state == 1) {
                SetReplayCamera(3001);
                work->timer = 30;
            } else if (work->state == 2) {
                SetReplayCamera(3002);
                work->timer = 60;
            }
            ++work->state;
        } else {
            for (std::size_t i = 0; i < 6; ++i) {
                const auto &sample = duckRaceRuntime.replay[i][work->timer];
                const auto name = duckRaceRuntime.names[i];
                EV_AT_SetPos(name, EventTransformFlags::World | EventTransformFlags::AllAxes, sample.position);
                EV_AT_SetAngY(name, EventTransformFlags::World | EventTransformFlags::AllAxes,
                    Vector3i{0, static_cast<std::uint16_t>(TruncateToInt32(sample.yawDegrees * 8192.0f / 45.0f)), 0});
                ConfigureScenarioNodeMotion(name, static_cast<std::uint32_t>(sample.motion), 0.0f, 32767.0f, 0.0f, 1.0f, 0);
                ConfigureScenarioNodeMotion(name, 0, -1.0f, -1.0f, sample.motionFrame, 32767.0f, 0xffffffffU);
                ConfigureScenarioNodeMotion(name, 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
                ConfigureScenarioNodeMotion(name, 0, -1.0f, -1.0f, -1.0f, 1.0f, 0xffffffffU);
            }
        }
        return;
    }
    if (work->state != 4 && work->state != 5)
        return;
    work->state = 5;
    for (auto *task : duckRaceRuntime.actors)
        if (task)
            HLib::ResolveDeferredCallback(task);
    duckRaceRuntime.followCamera = 1;
    HLib::DestroyCurrentTask();
    SnapDuckRaceCameraTarget();
}
}
void QueueDuckRaceReplay() {
    auto *task = HLib::EnqueueTaskWithParameter(DuckReplay_Callback, 3, 0x0b, sizeof(DuckReplayWork), SHEN_NAME(rply));
    auto *work = static_cast<DuckReplayWork *>(HLib::GetTaskParameter(task));
    *work = {task, 0, 30};
    for (auto *actor : duckRaceRuntime.actors)
        if (actor)
            HLib::SetupTimedTask(actor, 0);
}
}
