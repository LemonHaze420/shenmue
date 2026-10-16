#include "s2/areas/area_toma.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include <cstring>
namespace shenmue::s2::areas {
const TomaMapEntry *FindTomaMap(shen_name map) {
    for (const TomaMapEntry *row = TOMA_MAPS; row->map.name; ++row)
        if (row->map.name == map.name)
            return row;
    return nullptr;
}
void QueueTomaVoice(const char *voice, std::int32_t delay, AreaLoopCompletion completion) {
    if (!voice)
        return;
    const std::size_t length = std::strlen(voice);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AreaLoopVoiceCallback, 4, 0x0b, sizeof(AreaLoopVoiceWork) + length + 1, AREA_EVENT_TASK);
    if (!task)
        return;
    AreaLoopVoiceWork *const work = static_cast<AreaLoopVoiceWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    work->parent = HLib::GetCurrentTask();
    work->conversation = nullptr;
    work->flags = 0;
    work->duration = -1;
    work->elapsed = 0;
    work->completion = completion;
    work->voice = reinterpret_cast<char *>(work + 1);
    char *const copy = reinterpret_cast<char *>(work + 1);
    for (std::size_t index = length + 1; index > 0; --index)
        copy[index - 1] = voice[index - 1];
    HLib::SetupTimedTask(task, delay);
}
void QueueTomaCamera(std::int32_t camera, std::int32_t delay) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AreaLoopCameraCallback, 4, 0x0b, sizeof(AreaLoopCameraWork) + 5, AREA_EVENT_TASK);
    if (!task)
        return;
    AreaLoopCameraWork *const work = static_cast<AreaLoopCameraWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    work->parent = HLib::GetCurrentTask();
    work->flags = 0;
    work->camera = camera;
    work->character = TOMA_PROP;
    work->completion = nullptr;
    work->area = reinterpret_cast<char *>(work + 1);
    std::memcpy(work + 1, "TOMA", 5);
    if (delay > 0) {
        HLib::SetupTimedTask(task, delay);
        return;
    }
    work->flags |= 0x80000000U;
    AreaLoopCameraCallback(work);
    if (work->flags & 0x80000000U)
        work->flags &= 0x7fffffffU;
    else
        HLib::DestroyTask(task);
}
bool QueueTomaNpcs(const shen_name *characters) {
    if (!characters)
        return false;
    std::int32_t count = 0;
    while (characters[count].name)
        ++count;
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AreaLoopNpcCallback, 4, 0x0b, sizeof(AreaLoopNpcWork) + 4 * count, AREA_EVENT_TASK);
    if (!task)
        return false;
    AreaLoopNpcWork *const work = static_cast<AreaLoopNpcWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return false;
    }
    work->parent = HLib::GetCurrentTask();
    work->previousLoadState = GetNpcNewLoadState();
    work->remaining = count;
    work->characters = reinterpret_cast<shen_name *>(work + 1);
    work->completion = ResolveAreaLoopParent;
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    for (std::int32_t index = 0; index < count; ++index)
        work->characters[index] = characters[index];
    return true;
}
bool QueueTomaAuthor(shen_name author) {
    return QueueAreaLoopAuthor(nullptr, author, ResolveAreaLoopParent);
}
bool QueueTomaPlusPackage(const char *folder) {
    return QueueAreaLoopPackage(folder, "TOMAPLUS");
}
void QueueTomaMotion(shen_name character, std::uint32_t motion, AreaLoopCompletion completion, std::int32_t delay) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AreaLoopMotionCallback, 4, 0x0b, sizeof(AreaLoopMotionWork), AREA_EVENT_TASK);
    if (!task)
        return;
    AreaLoopMotionWork *const work = static_cast<AreaLoopMotionWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    work->parent = HLib::GetCurrentTask();
    work->character = character;
    work->flags = 0x16;
    work->motion = motion;
    work->duration = -1;
    work->elapsed = 0;
    work->startFrame = -1;
    work->endFrame = -1;
    work->speed = 1;
    work->frameTime = 0;
    work->completion = completion;
    if (delay > 0) {
        HLib::SetupTimedTask(task, delay);
        return;
    }
    work->flags |= 0x80000000U;
    AreaLoopMotionCallback(work);
    if (work->flags & 0x80000000U)
        work->flags &= 0x7fffffffU;
    else
        HLib::DestroyTask(task);
}
} // namespace shenmue::s2::areas
