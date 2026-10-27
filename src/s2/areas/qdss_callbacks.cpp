#include "s2/areas/area_qdss.hpp"
#include "s2/character.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/inventory.hpp"
#include "s2/map_assets.hpp"
#include <cstdint>
namespace shenmue::s2::areas {
namespace {
std::int32_t keyGrip[19][3]{{0, 726, 63534}, {0, 59894, 65172}, {0, 62624, 3822}, {0, 0, 49702}, {0, 63716, 54434}, {0, 64627, 63716}, {0, 546, 1820},
    {0, 1274, 59894}, {0, 0, 43878}, {0, 64809, 57892}, {0, 546, 1456}, {0, 0, 56436}, {0, 909, 47336}, {0, 64627, 58984}, {0, 546, 909}, {0, 0, 52978},
    {0, 0, 47882}, {0, 0, 59348}, {0, 546, 1092}};
struct EyeWork {
    shen_name character, target;
    std::uint32_t part;
};
struct ScaleWork {
    shen_name character;
    Vector3 scale;
};
struct PropWork {
    shen_name prop;
    std::uint32_t enabled;
};
void ApplyEye(const EyeWork &work) {
    Vector3 position;
    if (work.target.name) {
        EV_GetCharaPosition(work.target, work.part, &position, EventTransformFlags::World);
        SetEyeToChara(ctrlGetCharaByName(work.character), &position, 1, 2);
    } else
        SetEyeToChara(ctrlGetCharaByName(work.character), nullptr, 16, 0);
}
void EyeCallback(void *parameter) {
    const EyeWork &source = *static_cast<EyeWork *>(parameter);
    const EyeWork request{.character = source.character, .target = source.target, .part = CHARACTER_TRANSLATION_ROOT};
    ApplyEye(request);
    HLib::DestroyCurrentTask();
}
void EyePartCallback(void *parameter) {
    ApplyEye(*static_cast<EyeWork *>(parameter));
    HLib::DestroyCurrentTask();
}
void AwardCallback(void *parameter) {
    AddInventoryItem(*static_cast<std::uint32_t *>(parameter), 1, 1);
    HLib::DestroyCurrentTask();
}
void PropCallback(void *parameter) {
    const PropWork &work = *static_cast<PropWork *>(parameter);
    SetDisplayFlagForProp(work.prop, work.enabled);
    HLib::DestroyCurrentTask();
}
void ScaleCallback(void *parameter) {
    const ScaleWork &work = *static_cast<ScaleWork *>(parameter);
    mtwkSetMotionworkScale3(work.character, &work.scale);
    HLib::DestroyCurrentTask();
}
void QueueEye(const EyeWork &request, std::int32_t delay) {
    if (!delay) {
        ApplyEye(request);
        return;
    }
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EyePartCallback, 4, 0x0b, sizeof(EyeWork), QDSS_EVENT);
    HLib::SetupTimedTask(task, delay);
    EyeWork *work = static_cast<EyeWork *>(HLib::GetTaskParameter(task));
    work->character = request.character;
    work->target = request.target;
    if (request.part != CHARACTER_TRANSLATION_ROOT)
        work->part = request.part;
}
} // namespace
void PrepareQdssHands(const Vector3i &left, const Vector3i &right) {
    Character *character = ctrlGetCharaByName(QDSS_RYO);
    SetLowQualityHands(character);
    SetHighModelXYZ(character, 1, 1, 0);
    SetHighModelXYZ(character, 1, 0, 0);
    ApplyCharacterHandPoseCommand(character, 1, reinterpret_cast<std::byte *>(keyGrip), 0);
    ApplyCharacterHandPoseCommand(character, 0, reinterpret_cast<std::byte *>(keyGrip), 0);
    ApplyCharacterHandRotation(character, 0, 21, &left);
    ApplyCharacterHandRotation(character, 1, 21, &right);
}
void ApplyQdssKeyGrip(shen_name character, std::int32_t side) {
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(character), side, reinterpret_cast<std::byte *>(keyGrip), 0);
}
void QueueQdssCling(
    shen_name target, shen_name character, std::int32_t bone, const Vector3 *position, const Vector3i *rotation, std::uint32_t mode, std::int32_t delay) {
    if (!delay) {
        EventCling work;
        work.cleanup = 0;
        work.target = target;
        work.character = character;
        work.mode = mode;
        work.delay = 0;
        if (position) {
            work.bone = bone;
            work.position = *position;
            work.rotation = *rotation;
        }
        EVT_AT_Cling(&work);
        return;
    }
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), QDSS_EVENT);
    RegisterEventTask(task);
    EventCling *work = static_cast<EventCling *>(HLib::GetTaskParameter(task));
    work->cleanup = 1;
    work->target = target;
    work->character = character;
    work->mode = mode;
    work->delay = static_cast<float>(delay);
    if (position) {
        work->bone = bone;
        work->position = *position;
        work->rotation = *rotation;
    }
}
void QueueQdssEye(shen_name target, std::int32_t delay) {
    if (!delay) {
        const EyeWork request{.character = QDSS_RYO, .target = target, .part = CHARACTER_TRANSLATION_ROOT};
        ApplyEye(request);
        return;
    }

    HLib::Task *task = HLib::EnqueueTaskWithParameter(EyeCallback, 4, 0x0b, sizeof(EyeWork), QDSS_EVENT);
    HLib::SetupTimedTask(task, delay);
    EyeWork *work = static_cast<EyeWork *>(HLib::GetTaskParameter(task));
    work->character = QDSS_RYO;
    work->target = target;
}
void QueueQdssEyePart(std::int32_t delay) {
    QueueEye(EyeWork{.character = QDSS_RYO, .target = QDSS_REN, .part = 5}, delay);
}
void QueueQdssAward(std::uint32_t item, std::int32_t delay) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(AwardCallback, 4, 0x0b, 4, QDSS_EVENT);
    HLib::SetupTimedTask(task, delay);
    *static_cast<std::uint32_t *>(HLib::GetTaskParameter(task)) = item;
}
void QueueQdssProp(shen_name prop, std::uint32_t enabled, std::int32_t delay) {
    if (!delay) {
        SetDisplayFlagForProp(prop, enabled);
        return;
    }
    HLib::Task *task = HLib::EnqueueTaskWithParameter(PropCallback, 4, 0x0b, sizeof(PropWork), QDSS_EVENT);
    HLib::SetupTimedTask(task, delay);
    PropWork *work = static_cast<PropWork *>(HLib::GetTaskParameter(task));
    work->prop = prop;
    work->enabled = enabled;
}
void QueueQdssScale(shen_name character, const Vector3 &scale, std::int32_t delay) {
    if (!delay) {
        mtwkSetMotionworkScale3(character, &scale);
        return;
    }
    HLib::Task *task = HLib::EnqueueTaskWithParameter(ScaleCallback, 4, 0x0b, sizeof(ScaleWork), QDSS_EVENT);
    HLib::SetupTimedTask(task, delay);
    ScaleWork *work = static_cast<ScaleWork *>(HLib::GetTaskParameter(task));
    work->character = character;
    work->scale = scale;
}
void QdssFade(std::int32_t frames, bool opaque, std::int32_t delay) {
    const EventFilmControl work{.frames = frames, .startAlpha = opaque ? 0 : 255, .targetAlpha = opaque ? 255 : 0, .delay = static_cast<float>(delay)};
    EVT_AT_MenCtrl(work);
}
} // namespace shenmue::s2::areas
