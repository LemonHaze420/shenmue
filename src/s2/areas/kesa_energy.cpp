#include "s2/areas/area_kesa.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/motion_work.hpp"
#include "s2/world_render.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
namespace shenmue::s2::areas {
std::array<KesaEnergyParticle, 40> kesaEnergyParticles;
std::int32_t kesaEnergyFrame, kesaEnergyDuration, kesaEnergyActiveParticles;
float kesaEnergyLifetime;
namespace {
constexpr shen_name idle{SHEN_NAME(IDLE)}, initialize{SHEN_NAME(INIT)}, fade{SHEN_NAME(FADE)}, move{SHEN_NAME(MOVE)};
void CreateEnergyHierarchy(const Vector3 &position, const Vector3i &rotation) {
    CreateEventCharacter(CharacterCreationHighModel, nullptr, KESA_ENERGY_BASE);
    EV_AT_SetPos(KESA_ENERGY_BASE, EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(KESA_ENERGY_BASE, EventTransformFlags::AllAxes, rotation);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, KESA_ENERGY_PIVOT);
    AT_SetLocalBaseChara(KESA_ENERGY_PIVOT, KESA_ENERGY_BASE);
    EV_AT_SetPos(KESA_ENERGY_PIVOT, EventTransformFlags::AllAxes, {0, .02f, 0});
    EV_AT_SetAngY(KESA_ENERGY_PIVOT, EventTransformFlags::AllAxes, {-1, -1, -1});
    CreateEventCharacter(CharacterCreationHighModel, nullptr, KESA_ENERGY_BALL);
    AT_SetLocalBaseChara(KESA_ENERGY_BALL, KESA_ENERGY_PIVOT);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, KESA_ENERGY_DUMMY);
}
void InitializeParticle(KesaEnergyParticle &particle) {
    if (--particle.delay > 0)
        return;
    particle.maximumScale = GetRandomNumberInRange(.59999996f) + .1f;
    if (particle.mode != 0) {
        const float spread = particle.mode == 1 ? .4f : .2f;
        particle.position.x = GetRandomNumberInRange(spread) - spread * .5f;
        particle.position.z = GetRandomNumberInRange(spread) - spread * .5f;
        particle.position.y = GetRandomNumberInRange(particle.mode == 1 ? .1f : .5f) + (particle.mode == 1 ? 0 : .2f);
        particle.velocity.x = 0;
        particle.velocity.z = 0;
        particle.velocity.y = 1 / kesaEnergyLifetime;
        particle.maximumScale *= particle.mode == 1 ? .5f : 2;
    } else {
        particle.position.x = GetRandomNumberInRange(1) - .5f;
        particle.position.z = GetRandomNumberInRange(1) - .5f;
        const float inverseLifetime = 1 / kesaEnergyLifetime;
        particle.position.y = (std::fabs(particle.position.x) + std::fabs(particle.position.z)) * .5f + .05f;
        particle.velocity.x = (particle.direction * particle.position.x) * inverseLifetime;
        particle.velocity.y = (particle.position.y * particle.direction) * inverseLifetime;
        particle.velocity.z = (particle.direction * particle.position.z) * inverseLifetime;
    }
    particle.scale.x = 0;
    particle.scale.z = 0;
    particle.scaleStep = particle.maximumScale * .1f;
    particle.visible = 1;
    particle.state = fade;
}
void MoveParticle(KesaEnergyParticle &particle) {
    if (particle.position.y <= .5f / kesaEnergyLifetime || particle.position.y >= 1.5f) {
        if (kesaEnergyDuration - 50.0f < kesaEnergyFrame) {
            particle.state = idle;
            particle.visible = 0;
        } else
            particle.state = initialize;
    } else {
        particle.position.x += particle.velocity.x;
        particle.position.y += particle.velocity.y;
        particle.position.z += particle.velocity.z;
    }
}
void ActivateParticles(std::int32_t count) {
    kesaEnergyActiveParticles = count;
    for (std::int32_t index = count - 1; index >= 0; --index) {
        auto &particle = kesaEnergyParticles[index];
        if (particle.state.name != idle.name)
            break;
        particle.state = initialize;
    }
}
void UpdateParticles() {
    ++kesaEnergyFrame;
    EulerRotation16 cameraRotation;
    camCtrlGetRot(&cameraRotation);
    const Vector3i rotation{cameraRotation.pitch, cameraRotation.yaw, cameraRotation.roll};
    for (auto &particle : kesaEnergyParticles) {
        if (particle.state.name == fade.name) {
            if (particle.maximumScale <= particle.scale.x)
                particle.state = move;
            else {
                particle.scale.x += particle.scaleStep;
                particle.scale.y += particle.scaleStep;
                particle.scale.z += particle.scaleStep;
            }
            MoveParticle(particle);
        } else if (particle.state.name == idle.name)
            particle.visible = 0;
        else if (particle.state.name == move.name)
            MoveParticle(particle);
        else if (particle.state.name == initialize.name)
            InitializeParticle(particle);
        if (particle.visible) {
            EV_AT_SetPos(KESA_ENERGY_BALL, EventTransformFlags::AllAxes, particle.position);
            Vector3 position;
            EV_GetCharaPosition(KESA_ENERGY_BALL, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
            ApplyTranslationRotationScaleXYZToCharaByID(particle.character, 1, &position, &rotation, &particle.scale);
        }
    }
    if (kesaEnergyFrame > kesaEnergyDuration) {
        AT_SetLocalBaseChara(KESA_ENERGY_PIVOT, KESA_INVALID);
        AT_SetLocalBaseChara(KESA_ENERGY_BALL, KESA_INVALID);
        if (DoesScenarioCharacterExist(KESA_EFFECT_TWO))
            AT_SetLocalBaseChara(KESA_EFFECT_TWO, KESA_INVALID);
        for (shen_name name : {KESA_ENERGY_BASE, KESA_ENERGY_PIVOT, KESA_ENERGY_BALL, KESA_ENERGY_DUMMY})
            SetScenarioCharacterKillMode(name, 1);
        HLib::DestroyCurrentTask();
    }
}
} // namespace
void KesaInitializeEnergyBall(std::int32_t mode) {
    Vector3 position;
    Vector3i rotation;
    if (mode == 2) {
        EV_GetCharaPosition(SHEN_NAME(aa39), CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        rotation = {32768, 0, 0};
    } else if (mode == 1) {
        position = {115.74f, 106.3f, -14.93f};
        rotation = {-1, -1, -1};
    } else {
        position = {125.618f, 106.583f, 3.684f};
        rotation = {49152, 5188, 0};
    }
    CreateEnergyHierarchy(position, rotation);
    kesaEnergyFrame = 0;
    kesaEnergyDuration = mode == 0 ? 315 : (mode == 1 ? 180 : 264);
    if (mode == 0) {
        AT_SetLocalBaseChara(KESA_EFFECT_ONE, KESA_ENERGY_PIVOT);
        EV_AT_SetPos(KESA_EFFECT_ONE, EventTransformFlags::AllAxes, {0, 0, 0});
        EV_AT_SetAngY(KESA_EFFECT_ONE, EventTransformFlags::AllAxes, {16384, 0, 0});
        const Vector3 zeroScale{0, 0, 0};
        SetMotionworkFlagForCharaById(KESA_EFFECT_ONE, &zeroScale);
        SetScenarioCharacterTransparency(KESA_EFFECT_ONE, 0);
        EV_EnableCharacterDisplay(KESA_EFFECT_ONE);
        AT_SetLocalBaseChara(KESA_EFFECT_ONE, KESA_INVALID);
        for (shen_name name : {KESA_RIPPLE_ONE, KESA_RIPPLE_TWO}) {
            AT_SetLocalBaseChara(name, KESA_ENERGY_PIVOT);
            EV_AT_SetPos(name, EventTransformFlags::AllAxes, {0, 0, 0});
            EV_AT_SetAngY(name, EventTransformFlags::AllAxes, {16384, name.name == KESA_RIPPLE_ONE.name ? 0 : 8192, 0});
            AT_SetLocalBaseChara(name, KESA_INVALID);
        }
        AT_SetLocalBaseChara(KESA_EFFECT_TWO, KESA_ENERGY_PIVOT);
        EV_AT_SetPos(KESA_EFFECT_TWO, EventTransformFlags::AllAxes, {0, .1f, 0});
        EV_AT_SetAngY(KESA_EFFECT_TWO, EventTransformFlags::AllAxes, {32768, 0, 0});
        const Vector3 effectScale{.09f, .15f, .09f};
        SetMotionworkFlagForCharaById(KESA_EFFECT_TWO, &effectScale);
    }
    kesaEnergyLifetime = 50;
    kesaEnergyActiveParticles = mode == 0 ? 10 : 20;
    for (std::int32_t index = 0; index < 40; ++index) {
        auto &particle = kesaEnergyParticles[index];
        if (mode == 0) {
            particle.position = {0, .1f, 0};
            std::memset(particle.reserved1c, 0, sizeof(particle.reserved1c));
            particle.direction = -1;
            particle.mode = 0;
            particle.character = GetRandomNumberInRange(1) < .2f ? SHEN_NAME(EB_6) : SHEN_NAME(EB_7);
        } else {
            std::memset(particle.reserved1c, 0, sizeof(particle.reserved1c));
            particle.direction = 1;
            particle.mode = mode;
            particle.character = mode == 2 ? SHEN_NAME(KIRA) : (GetRandomNumberInRange(1) < .2f ? SHEN_NAME(EB_2) : SHEN_NAME(EB_0));
        }
        particle.visible = 0;
        if (index < kesaEnergyActiveParticles) {
            particle.state = initialize;
            particle.delay = shenGetRandomInt32(static_cast<std::int32_t>(kesaEnergyLifetime));
        } else {
            particle.state = idle;
            particle.delay = 0;
        }
    }
}
void KesaEnergyBallLoop(void *) {
    Vector3i rotation;
    GetScenarioCharacterEulerAngles(KESA_ENERGY_PIVOT, &rotation, EventTransformFlags::Local);
    rotation.y += 182;
    EV_AT_SetAngY(KESA_ENERGY_PIVOT, EventTransformFlags::AllAxes, rotation);
    Vector3 zero{0, 0, 0};
    const Vector3i effectRotation{0, 65354, 0};
    for (std::uint16_t bone = 152; bone < 155; ++bone)
        EV_scenApplyPosOrRotToPartOf(KESA_EFFECT_TWO, bone, &zero, const_cast<Vector3i *>(&effectRotation), EventCharacterPartTransformMode::Relative);
    if (kesaEnergyFrame <= 285) {
        kesaEnergyLifetime = 50 - kesaEnergyFrame * .1f;
        ActivateParticles(kesaEnergyFrame / 10 + 10);
    }
    const float scale = kesaEnergyFrame * .0033333334f;
    const Vector3 effectScale{scale, scale, 1};
    SetMotionworkFlagForCharaById(KESA_EFFECT_ONE, &effectScale);
    SetScenarioCharacterTransparency(KESA_EFFECT_ONE, std::min(1.0f, scale));
    UpdateParticles();
}
void KesaEnergyBallRiseLoop(void *) {
    if (kesaEnergyFrame <= 100)
        ActivateParticles(kesaEnergyFrame / 5 + 20);
    UpdateParticles();
}
void KesaEnergyBallFollowLoop(void *) {
    Vector3 position;
    EV_GetCharaPosition(SHEN_NAME(aa39), CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    EV_AT_SetPos(KESA_ENERGY_BASE, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    if (kesaEnergyFrame <= 200)
        ActivateParticles(kesaEnergyFrame / 10 + 20);
    UpdateParticles();
}
} // namespace shenmue::s2::areas
