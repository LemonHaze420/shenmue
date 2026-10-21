#include "s2/areas/area_cb00.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/event_runtime.hpp"
#include "s2/particle_effects.hpp"
namespace shenmue::s2 {
void Cb00SmokeCreateCallback(void *data) {
    Cb00SmokeWork *const work = static_cast<Cb00SmokeWork *>(data);
    if (!work) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (*work->effect != -1) {
        SuspendParticleEffect(0, *work->effect);
        FreeParticleEffect(0, *work->effect);
    }
    if (work->character.name)
        EV_GetCharaPosition(work->character, 22, &work->position, EventTransformFlags::World);
    const std::uint32_t effect = static_cast<std::uint32_t>(CreateSmokeEffect(work->position, work->volume, 40, 0.06f, shen_name{0}, 0));
    SetSmokeEffectFadeFactor(effect, 27);
    SetSmokeEffectFalloff(effect, 158);
    SetParticleEffectColour(0, effect, 0, 106, 104, 101);
    SetParticleEffectInitialScale(0, effect, 0.21568627655506134f);
    SetParticleEffectSize(0, effect, 2.313725471496582f, 2.8235294818878174f);
    SetParticleEffectBirthRate(0, effect, 0.03019607998430729f);
    SetParticleEffectVelocity(0, effect, work->velocity, {0.0235294122248888f, 0.0235294122248888f, 0.0235294122248888f});
    ResumeParticleEffect(0, effect);
    *work->effect = static_cast<std::int32_t>(effect);
    HLib::SetCurrentTaskTo(work->killImmediately ? Cb00SmokeKillCallback : Cb00SmokeStopCallback);
    HLib::SetupTaskDelay(work->frames);
}
void Cb00SmokeStopCallback(void *data) {
    Cb00SmokeWork *const work = static_cast<Cb00SmokeWork *>(data);
    if (!work) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (*work->effect != -1)
        SetParticleEffectBirthRate(0, *work->effect, 0);
    HLib::SetCurrentTaskTo(Cb00SmokeKillCallback);
    HLib::SetupTaskDelay(50);
}
void Cb00SmokeKillCallback(void *data) {
    Cb00SmokeWork *const work = static_cast<Cb00SmokeWork *>(data);
    if (work && *work->effect != -1) {
        SuspendParticleEffect(0, *work->effect);
        FreeParticleEffect(0, *work->effect);
        *work->effect = -1;
    }
    HLib::DestroyCurrentTask();
}
void QueueCb00Smoke(Cb00EventWork *parent, shen_name character, const Vector3 *position, const Vector3 &volume, std::int32_t delay, std::int32_t frames,
    std::int32_t killImmediately) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Cb00SmokeCreateCallback, 6, 11, sizeof(Cb00SmokeWork), CB00_TASK);
    HLib::SetupTimedTask(task, delay);
    Cb00SmokeWork *const work = static_cast<Cb00SmokeWork *>(HLib::GetTaskParameter(task));
    if (work) {
        if (position)
            work->position = *position;
        work->velocity = {};
        work->volume = volume;
        work->killImmediately = killImmediately;
        work->character = character;
        work->effect = &parent->smoke;
        work->frames = frames;
    }
}
HLib::Task *QueueCb00Blur(std::int32_t delay, std::int32_t frames) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Cb00BlurCreateCallback, 6, 11, sizeof(Cb00BlurWork), CB00_TASK);
    if (delay > 0)
        HLib::SetupTimedTask(task, delay);
    Cb00BlurWork *const work = static_cast<Cb00BlurWork *>(HLib::GetTaskParameter(task));
    if (work) {
        work->character = CB00_RYO;
        work->scale = 0;
        work->frames = frames;
        work->maximumScale = 8;
    }
    return task;
}
void Cb00BlurCreateCallback(void *data) {
    Cb00BlurWork *const work = static_cast<Cb00BlurWork *>(data);
    if (!work) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (!work->scale) {
        figpAssignModeFor(work->character, 1);
        figpSetFlagsBlendForBlur(work->character, 0, 1);
        ++work->scale;
    } else {
        if (work->scale < work->maximumScale)
            figpSetFlagsBlendForBlur(work->character, static_cast<std::int8_t>(work->scale), 1);
        else {
            HLib::SetCurrentTaskTo(Cb00BlurDestroyCallback);
            HLib::SetupTaskDelay(work->frames);
        }
        ++work->scale;
    }
}
void Cb00BlurDestroyCallback(void *data) {
    Cb00BlurWork *const work = static_cast<Cb00BlurWork *>(data);
    if (work->scale-- == 1) {
        figpAssignModeFor(work->character, 0);
        HLib::DestroyCurrentTask();
    } else
        figpSetFlagsBlendForBlur(work->character, static_cast<std::int8_t>(work->scale), 1);
}
void Cb00TruckQuakeBaseCallback(void *data) {
    Cb00TruckQuakeWork *const work = static_cast<Cb00TruckQuakeWork *>(data);
    Vector3 position;
    for (std::int32_t index = 0; index < 3; ++index) {
        EV_GetCharaPosition(CB00_TRUCKS[index], 0xffffffffU, &position, EventTransformFlags::Local);
        work->baseY[index] = position.y;
    }
    HLib::SetCurrentTaskTo(Cb00TruckQuakeCallback);
}
void Cb00TruckQuakeCallback(void *data) {
    Cb00TruckQuakeWork *const work = static_cast<Cb00TruckQuakeWork *>(data);
    if (static_cast<float>(work->angle) > 60) {
        if (--work->angleStep < 1)
            work->angleStep = 1;
    }
    if (static_cast<float>(work->angle) > 180)
        work->angleStep = -work->angleStep;
    work->angle += work->angleStep;
    const std::int32_t frames = work->frames--;
    const float displacement = frames ? sin_fixed16(static_cast<std::int16_t>(work->angle)) * work->amplitude : 0;
    for (std::int32_t index = 0; index < 3; ++index) {
        Vector3 position;
        EV_GetCharaPosition(CB00_TRUCKS[index], 0xffffffffU, &position, EventTransformFlags::Local);
        position.y = displacement + work->baseY[index];
        EV_AT_SetPos(CB00_TRUCKS[index], EventTransformFlags::AllAxes, position);
    }
    if (frames)
        work->amplitude -= work->decay;
    else
        HLib::DestroyCurrentTask();
}
void QueueCb00TruckQuake(float amplitude) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Cb00TruckQuakeBaseCallback, 6, 11, sizeof(Cb00TruckQuakeWork), CB00_TASK);
    HLib::SetupTimedTask(task, 50);
    Cb00TruckQuakeWork *const work = static_cast<Cb00TruckQuakeWork *>(HLib::GetTaskParameter(task));
    work->baseY.x = 0;
    work->baseY.y = 0;
    work->amplitude = amplitude;
    work->decay = amplitude / 20;
    work->angle = 0;
    work->angleStep = 1820;
    work->frames = 20;
}
void Cb00DormantRetireCallback(void *) {
    HLib::DestroyCurrentTask();
}
void Cb00InitializationNullsub() {
}
} // namespace shenmue::s2
