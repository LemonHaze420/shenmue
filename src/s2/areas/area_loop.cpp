#include "s2/areas/area_loop.hpp"
#include "s2/area_event.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/scene_data.hpp"
#include "s2/pak.hpp"
#include <cmath>
#include <cstring>
namespace shenmue::s2 {
namespace {
constexpr std::uint32_t immediate = 0x80000000U;
void Complete(std::uint32_t &flags) {
    if (flags & immediate)
        flags &= ~immediate;
    else
        HLib::DestroyCurrentTask();
}
} // namespace
std::int64_t ResolveAreaLoopParent(std::int32_t phase, std::int64_t, HLib::Task *parent) {
    if (phase == 1 && parent) {
        HLib::ResolveDeferredCallback(parent);
        HLib::SetupTimedTask(parent, 1);
    }
    return 1;
}
void AreaLoopPositionCallback(void *parameter) {
    auto *work = static_cast<AreaLoopPositionWork *>(parameter);
    if (!work) {
        HLib::DestroyCurrentTask();
        return;
    }
    const auto transformFlags = (work->flags & 4) ? 0x38000000U : 0x78000000U;
    if (work->flags & 1)
        EV_AT_SetPos(work->character, transformFlags, work->position);
    if (work->flags & 2)
        EV_AT_SetAngY(work->character, transformFlags, work->rotation);
    if (work->flags & 3)
        mtwkSetExtraFlag(scenFindMotionwork(work->character), 17, 0);
    if (work->completion)
        work->completion(0, work->character.name, work->parent);
    Complete(work->flags);
}
void AreaLoopMotionCallback(void *parameter) {
    auto *work = static_cast<AreaLoopMotionWork *>(parameter);
    if (!work) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (!(work->flags & 0x40000000U)) {
        mtwkRequestMotion(scenFindMotionwork(work->character), static_cast<std::uint16_t>(work->motion), MotionRegion::LowerBody);
        if (work->flags & 1)
            SetMotionRegionFlag(scenFindMotionwork(work->character), 69, 0, MotionRegion::LowerBody);
        if (work->flags & 2)
            mtwkSetRequestedRootMode(scenFindMotionwork(work->character), 0);
        else {
            mtwkSetRequestedRootMode(scenFindMotionwork(work->character), 1);
            SetMotionFrameTime(scenFindMotionwork(work->character), work->frameTime, MotionRegion::LowerBody);
        }
        if (work->flags & 8)
            SetMotionRegionFlag(scenFindMotionwork(work->character), 66, 0, MotionRegion::LowerBody);
        if (std::fabs(work->startFrame) >= 0.00000011920929f) {
            SetMotionLayerLoopStartTime(scenFindMotionwork(work->character), work->startFrame, MotionRegion::LowerBody);
            SetMotionRegionEvalTime(scenFindMotionwork(work->character), work->startFrame, MotionRegion::LowerBody);
        }
        if (work->endFrame > 0)
            SetMotionLayerEndFrame(scenFindMotionwork(work->character), work->endFrame, MotionRegion::LowerBody);
        SetMotionSpeed(scenFindMotionwork(work->character), work->speed, MotionRegion::LowerBody);
        if (work->completion)
            work->completion(0, work->character.name, work->parent);
        work->flags |= 0x40000000U;
    }
    EV_mtwkSetEnabled(scenFindMotionwork(work->character), work->flags & 4);
    if (work->completion)
        work->completion(2, work->elapsed, work->parent);
    ++work->elapsed;
    if ((work->duration < static_cast<std::int32_t>(work->elapsed) && !(work->flags & 0x10)) ||
        (SetMotionRegionFlag(scenFindMotionwork(work->character), 65, 1, MotionRegion::LowerBody) && (work->flags & 0x10))) {
        if (work->completion)
            work->completion(1, 0, work->parent);
        Complete(work->flags);
    }
}
void AreaLoopVoiceCallback(void *parameter) {
    auto *work = static_cast<AreaLoopVoiceWork *>(parameter);
    if (!work) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (!(work->flags & 2)) {
        if (work->flags & 1)
            dbgPrintEventMessage(work->voice);
        else
            work->conversation = EV_PlayDialogue(work->voice);
        if (work->completion)
            work->completion(0, 0, work->parent);
        work->flags |= 2;
    }
    if (work->completion)
        work->completion(2, work->elapsed, work->parent);
    ++work->elapsed;
    if (work->duration >= 0) {
        if (static_cast<std::uint32_t>(work->duration) >= work->elapsed)
            return;
        if (work->flags & 1)
            ctrlStopSubtitles();
        else if (work->conversation && cvsHasValidConverseData(work->conversation))
            scenCvsCtrlFree(work->conversation);
    } else {
        if (work->flags & 1) {
            Complete(work->flags);
            return;
        }
        if (work->conversation && cvsHasValidConverseData(work->conversation))
            return;
    }
    work->conversation = nullptr;
    if (work->completion)
        work->completion(1, 0, work->parent);
    Complete(work->flags);
}
void AreaLoopCameraCallback(void *parameter) {
    auto *work = static_cast<AreaLoopCameraWork *>(parameter);
    if (!work) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (work->area && work->area[0])
        EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>(work->area), 0);
    EVS_LoopCamera(work->camera, work->character, work->character);
    if (work->completion)
        work->completion(0, 0, work->parent);
    Complete(work->flags);
}
void AreaLoopAttachmentCallback(void *parameter) {
    AreaLoopAttachmentWork *const work = static_cast<AreaLoopAttachmentWork *>(parameter);
    if (!work) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (!(work->flags & 0x40000000U)) {
        AttachScenarioCharacter(work->target, work->character, work->attachmentFlags, &work->position, &work->rotation);
        if (work->completion)
            work->completion(0, 0, work->parent);
        work->flags |= 0x40000000U;
    }
    if (work->completion)
        work->completion(2, work->elapsed, work->parent);
    if (static_cast<std::int32_t>(++work->elapsed) > work->duration) {
        ResetScenarioCharacterAttachment(work->target);
        if (work->completion)
            work->completion(1, 0, work->parent);
        Complete(work->flags);
    }
}
void AreaLoopLookAtCallback(void *parameter) {
    AreaLoopLookAtWork *const work = static_cast<AreaLoopLookAtWork *>(parameter);
    if (!work) {
        HLib::DestroyCurrentTask();
        return;
    }
    Vector3 position;
    if (work->character.name) {
        if (work->bone < 0)
            EV_GetCharaPosition(work->target, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        else
            GetMotionBonePosition(scenFindMotionwork(work->target), work->bone, &position);
    } else
        position = work->position;
    UpdateMotionWorkWithPosition(scenFindMotionwork(work->character), &position, 3);
    SetEyeToChara(ctrlGetCharaByName(work->character), nullptr, 16, 0);
    SetEyeToChara(ctrlGetCharaByName(work->character), &position, 0, 2);
    if (!(work->flags & 0x40000000U)) {
        if (work->completion)
            work->completion(0, 0, work->parent);
        work->flags |= 0x40000000U;
    }
    if (work->completion)
        work->completion(2, work->elapsed, work->parent);
    if (static_cast<std::int32_t>(++work->elapsed) > work->duration) {
        SetEyeToChara(ctrlGetCharaByName(work->character), nullptr, 16, 0);
        UpdateMotionWorkWithPosition(scenFindMotionwork(work->character), nullptr, 0);
        if (work->completion)
            work->completion(1, 0, work->parent);
        Complete(work->flags);
    }
}
void AreaLoopAuthorCallback(void *parameter) {
    AreaLoopAuthorWork *const work = static_cast<AreaLoopAuthorWork *>(parameter);
    if (!(work->flags & 1U)) {
        if (!EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load,
                reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)), reinterpret_cast<std::uintptr_t>(work->package)))
            return;
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        work->flags |= 1U;
    }
    if (!(work->flags & 2U)) {
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(work->author.name));
        work->flags |= 2U;
        if (work->completion)
            work->completion(0, 0, work->parent);
    }
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (work->completion)
        work->completion(2, frame, work->parent);
    const bool finished = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) == 0;
    if (finished || (work->endFrame >= 0 && frame >= work->endFrame)) {
        if (work->completion)
            work->completion(1, 0, work->parent);
        if (!finished)
            EV_authSceneCtrl(AuthSceneCtrl::RefreshAndEnqueue);
        if (work->package && EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(work->package), 0))
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(work->package), 0);
        Complete(work->flags);
    }
}
bool QueueAreaLoopAuthor(const char *package, shen_name author, AreaLoopCompletion completion) {
    const std::size_t bytes = package && package[0] ? std::strlen(package) + 1 : 0;
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AreaLoopAuthorCallback, 4, 0x0b, sizeof(AreaLoopAuthorWork) + bytes, AREA_EVENT_TASK);
    if (!task)
        return false;
    AreaLoopAuthorWork *const work = static_cast<AreaLoopAuthorWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return false;
    }
    work->parent = HLib::GetCurrentTask();
    work->flags = bytes ? 0U : 1U;
    work->package = bytes ? reinterpret_cast<char *>(work + 1) : nullptr;
    work->author = author;
    work->endFrame = -1;
    work->completion = completion;
    if (bytes) {

        char *const copy = reinterpret_cast<char *>(work + 1);
        for (std::size_t index = bytes + 1; index > 0; --index)
            copy[index - 1] = package[index - 1];
    }
    work->flags |= immediate;
    AreaLoopAuthorCallback(work);
    if (work->flags & immediate) {
        work->flags &= ~immediate;
        return true;
    }
    HLib::DestroyTask(task);
    return true;
}
void AreaLoopPackageCallback(void *parameter) {
    AreaLoopPackageWork *const work = static_cast<AreaLoopPackageWork *>(parameter);
    if (!work) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (!(work->flags & 0x40000000U)) {
        work->flags |= 0x40000000U;
        if (work->completion)
            work->completion(0, 0, work->parent);
    }
    if (work->completion)
        work->completion(2, 0, work->parent);
    if (EV_ControlPackageResources(
            PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(work->folder), reinterpret_cast<std::uintptr_t>(work->package))) {
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        if (work->completion)
            work->completion(1, 0, work->parent);
        Complete(work->flags);
    }
}
bool QueueAreaLoopPackage(const char *folder, const char *package) {
    if (!package || !package[0] || !folder || !folder[0])
        return false;
    const std::size_t packageBytes = std::strlen(package) + 1, folderBytes = std::strlen(folder) + 1;
    const std::size_t alignedPackageBytes = (packageBytes + 3) & ~std::size_t{3};
    HLib::Task *const task =
        HLib::EnqueueTaskWithParameter(AreaLoopPackageCallback, 4, 0x0b, sizeof(AreaLoopPackageWork) + alignedPackageBytes + folderBytes, AREA_EVENT_TASK);
    if (!task)
        return false;
    AreaLoopPackageWork *const work = static_cast<AreaLoopPackageWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return false;
    }
    char *const copy = reinterpret_cast<char *>(work + 1);
    work->parent = HLib::GetCurrentTask();
    work->flags = 0;
    work->completion = ResolveAreaLoopParent;
    work->package = copy;
    work->folder = copy + alignedPackageBytes;
    std::memcpy(copy, package, packageBytes);
    std::memcpy(copy + alignedPackageBytes, folder, folderBytes);
    work->flags |= immediate;
    AreaLoopPackageCallback(work);
    if (work->flags & immediate)
        work->flags &= ~immediate;
    else
        HLib::DestroyTask(task);
    return true;
}
// @verified - p
void SetAreaLoopCharacterControl(shen_name character, std::int32_t scripted) {
    if (!character.name)
        return;
    const bool hasMotion = scenFindMotionwork(character) != nullptr;
    if (scripted) {
        EV_SetCharaControllerFlags(character, 1, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(character, 0, NpcCharacterControllerState::Disabled);
        if (hasMotion) {
            const Vector4 reflector{1, 1, 1, 1};
            EV_SetRefBoardParamsFor(character, 1);
            SetScenarioReflectorBoardValues(character, &reflector);
            mtwkSetExtraFlag(scenFindMotionwork(character), 6, 2);
            EV_SetCharacterAlphaEnabled(character, false);
        }
    } else {
        if (hasMotion) {
            EV_SetCharacterAlphaEnabled(character, true);
            mtwkSetExtraFlag(scenFindMotionwork(character), 6, 0);
            EV_SetRefBoardParamsFor(character, 0);
        }
        EV_SetCharaControllerFlags(character, 0, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(character, 1, NpcCharacterControllerState::Enabled);
    }
}
void RestoreAreaLoopCharacters(const shen_name *characters) {
    if (!characters)
        return;
    for (const shen_name *name = characters; name->name; ++name)
        if (DoesScenarioCharacterExist(*name)) {
            SetAreaLoopCharacterControl(*name, 0);
            EV_npcFree(*name);
            EV_npcLockUnlock(*name);
        }
}
void AreaLoopNpcCallback(void *parameter) {
    auto *work = static_cast<AreaLoopNpcWork *>(parameter);
    if (!work) {
        HLib::DestroyCurrentTask();
        return;
    }
    bool done = work->remaining == 0;
    if (work->remaining) {
        EV_CheckWarmStartNpcRoute(work->characters[work->remaining - 1]);
        while (work->remaining) {
            const auto state = EV_npcStreamOrLoadHuman(work->characters[work->remaining - 1]);
            if (!state)
                break;
            if (state != 1) {
                --work->remaining;
                break;
            }
            SetAreaLoopCharacterControl(work->characters[work->remaining - 1], 1);
            if (!--work->remaining)
                break;
        }
        done = work->remaining == 0;
    }
    if (work->completion)
        work->completion(2, work->remaining, work->parent);
    if (done) {
        if (work->previousLoadState)
            EV_NPC_NewLoadSw(1);
        if (work->completion)
            work->completion(1, 0, work->parent);
        HLib::DestroyCurrentTask();
    }
}
AreaLoopPositionWork *QueueAreaLoopPosition(const AreaLoopPositionWork &request, std::int32_t delay) {
    auto *task = HLib::EnqueueTaskWithParameter(AreaLoopPositionCallback, 4, 0x0b, sizeof(AreaLoopPositionWork), AREA_EVENT_TASK);
    if (!task)
        return nullptr;
    auto *work = static_cast<AreaLoopPositionWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return nullptr;
    }
    *work = request;
    if (delay > 0)
        HLib::SetupTimedTask(task, delay);
    else {
        work->flags |= immediate;
        AreaLoopPositionCallback(work);
        if (work->flags & immediate)
            work->flags &= ~immediate;
        else {
            HLib::DestroyTask(task);
            return nullptr;
        }
    }
    return work;
}
AreaLoopMotionWork *QueueAreaLoopMotion(const AreaLoopMotionWork &request, std::int32_t delay) {
    auto *task = HLib::EnqueueTaskWithParameter(AreaLoopMotionCallback, 4, 0x0b, sizeof(AreaLoopMotionWork), AREA_EVENT_TASK);
    if (!task)
        return nullptr;
    auto *work = static_cast<AreaLoopMotionWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return nullptr;
    }
    *work = request;
    if (delay > 0)
        HLib::SetupTimedTask(task, delay);
    else {
        work->flags |= immediate;
        AreaLoopMotionCallback(work);
        if (work->flags & immediate)
            work->flags &= ~immediate;
        else {
            HLib::DestroyTask(task);
            return nullptr;
        }
    }
    return work;
}
AreaLoopVoiceWork *QueueAreaLoopVoice(const char *voice, std::uint32_t flags, std::int32_t duration, AreaLoopCompletion completion, std::int32_t delay) {
    if (!voice)
        return nullptr;
    const auto length = std::strlen(voice);
    auto *task = HLib::EnqueueTaskWithParameter(AreaLoopVoiceCallback, 4, 0x0b, sizeof(AreaLoopVoiceWork) + length + 1, AREA_EVENT_TASK);
    if (!task)
        return nullptr;
    auto *work = static_cast<AreaLoopVoiceWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return nullptr;
    }
    auto *copy = reinterpret_cast<char *>(work + 1);
    std::memcpy(copy, voice, length + 1);
    *work = {HLib::GetCurrentTask(), nullptr, flags, duration, 0, 0, completion, copy};
    if (delay > 0)
        HLib::SetupTimedTask(task, delay);
    else {
        work->flags |= immediate;
        AreaLoopVoiceCallback(work);
        if (work->flags & immediate)
            work->flags &= ~immediate;
        else {
            HLib::DestroyTask(task);
            return nullptr;
        }
    }
    return work;
}
AreaLoopCameraWork *QueueAreaLoopCamera(const char *area, std::int32_t camera, shen_name character, AreaLoopCompletion completion, std::int32_t delay) {
    const auto length = area ? std::strlen(area) : 0;
    auto *task = HLib::EnqueueTaskWithParameter(AreaLoopCameraCallback, 4, 0x0b, sizeof(AreaLoopCameraWork) + length + 1, AREA_EVENT_TASK);
    if (!task)
        return nullptr;
    auto *work = static_cast<AreaLoopCameraWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return nullptr;
    }
    auto *copy = reinterpret_cast<char *>(work + 1);
    if (area)
        std::memcpy(copy, area, length + 1);
    else
        *copy = 0;
    *work = {HLib::GetCurrentTask(), 0, camera, character, 0, completion, copy};
    if (delay > 0)
        HLib::SetupTimedTask(task, delay);
    else {
        work->flags |= immediate;
        AreaLoopCameraCallback(work);
        if (work->flags & immediate)
            work->flags &= ~immediate;
        else {
            HLib::DestroyTask(task);
            return nullptr;
        }
    }
    return work;
}
bool QueueAreaLoopNpcs(const shen_name *characters, AreaLoopCompletion completion) {
    if (!characters)
        return false;
    std::int32_t count = 0;
    while (characters[count].name)
        ++count;
    auto *task = HLib::EnqueueTaskWithParameter(AreaLoopNpcCallback, 4, 0x0b, sizeof(AreaLoopNpcWork) + 4 * count, AREA_EVENT_TASK);
    if (!task)
        return false;
    auto *work = static_cast<AreaLoopNpcWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return false;
    }
    auto *copy = reinterpret_cast<shen_name *>(work + 1);
    *work = {HLib::GetCurrentTask(), GetNpcNewLoadState(), count, 0, copy, completion};
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    std::memcpy(copy, characters, 4 * count);
    return true;
}
} // namespace shenmue::s2
