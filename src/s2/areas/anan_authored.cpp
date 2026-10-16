#include "s2/areas/area_anan.hpp"
#include "s2/areas/anan_tables.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include <cstring>
namespace shenmue::s2::areas {
void ApplyAnanPlacements(const AnanPlacement *placements) {
    for (const AnanPlacement *row = placements; row->character.name != ANAN_INVALID.name; ++row) {
        EV_AT_SetPos(row->character, EventTransformFlags::AllAxes, row->position);
        EV_AT_SetAngY(row->character, EventTransformFlags::AllAxes, row->rotation);
    }
}
void AnanSelectCamera(std::int32_t camera) {
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("ANAN"), 0);
    EVS_LoopCamera(camera, ANAN_BASE, ANAN_BASE);
}
void AnanLoadPackage(const char *package) {
    const char *directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_ControlPackageResources(
        PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>(package));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>(package), 0);
}
void AnanFilm(std::uint32_t frames, std::uint32_t from, std::uint32_t to) {
    SetScreenFilmFade(frames, from, 0, 0, 0, to, 0, 0, 0);
}
void AnanSuspendCharacter(shen_name character) {
    const bool hasMotion = scenFindMotionwork(character) != nullptr;
    if (hasMotion) {
        SetScenarioReflectorBoardValues(character, &ananRefboard);
        EV_SetRefBoardParamsFor(character, 1);
    }
    EV_SetCharaControllerFlags(character, 1, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, 0, NpcCharacterControllerState::Disabled);
    if (hasMotion)
        mtwkSetExtraFlag(scenFindMotionwork(character), 6, 2);
    EV_SetCharacterAlphaEnabled(character, false);
}
bool QueueAnanAuthor(shen_name author, std::int32_t fadeOut, std::int32_t finishBeforeUnload) {
    std::uint32_t count = 0;
    while (ananAuthorCharacters[count].name != ANAN_INVALID.name)
        ++count;
    ++count;
    HLib::Task *task = HLib::EnqueueTaskWithParameter(AnanInitializeAuthor, 4, 11, 8 * count + 48, ANAN_EVENT);
    if (!task)
        return false;
    AnanAuthWork *work = static_cast<AnanAuthWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return false;
    }
    work->caller = HLib::GetCurrentTask();
    work->finishBeforeUnload = finishBeforeUnload;
    work->frameCallback = nullptr;
    work->fadeOut = fadeOut;
    work->author = author;
    work->characters = reinterpret_cast<shen_name *>(work + 1);
    work->parents = work->characters + count;
    std::uint32_t i = 0;
    for (; ananAuthorCharacters[i].name != ANAN_INVALID.name; ++i) {
        work->characters[i] = ananAuthorCharacters[i];
        work->parents[i] = EV_SCEN_FindChar_Wrapper(ananAuthorCharacters[i]);
        AT_SetLocalBaseChara(ananAuthorCharacters[i], ANAN_FREE);
    }
    work->characters[i] = ANAN_INVALID;
    work->parents[i] = ANAN_INVALID;
    HLib::SetupTaskDelay(-1);
    return true;
}
void AnanInitializeAuthor(void *data) {
    AnanAuthWork *work = static_cast<AnanAuthWork *>(data);
    if (!work)
        return;
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(work->author.name));
    AnanWaitAuthor(work);
    HLib::SetCurrentTaskTo(AnanWaitAuthor);
}
void AnanWaitAuthor(void *data) {
    AnanAuthWork *work = static_cast<AnanAuthWork *>(data);
    if (!work)
        return;
    bool forced = false;
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (work->frameCallback && work->frameCallback(frame)) {
        EV_authSceneCtrl(AuthSceneCtrl::RefreshAndEnqueue);
        forced = true;
    }
    if (work->fadeOut && frame + 32 == EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining))
        AnanFilm(32, 0, 255);
    if ((EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining) - 1 <= frame && work->finishBeforeUnload) || !EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) ||
        forced) {
        if (work->characters && work->parents) {
            for (std::uint32_t i = 0; work->characters[i].name != ANAN_INVALID.name; ++i)
                AT_SetLocalBaseChara(work->characters[i], work->parents[i]);
        }
        HLib::ResolveDeferredCallback(work->caller);
        HLib::DestroyCurrentTask();
    }
}
void QueueAnanVoice(const char *voice, std::int32_t startDelay, std::int32_t remaining, AnanVoiceCallback callback) {
    HLib::Task *caller = HLib::GetCurrentTask();
    if (!voice) {
        if (callback)
            callback(0, reinterpret_cast<std::intptr_t>(caller));
        return;
    }
    const std::size_t length = std::strlen(voice);
    const std::size_t size = startDelay > 0 ? length + 40 : 39;
    HLib::Task *task = HLib::EnqueueTaskWithParameter(AnanVoiceLoop, 4, 11, size, ANAN_EVENT);
    if (!task)
        return;
    AnanVoiceWork *work = static_cast<AnanVoiceWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        if (callback)
            callback(0, reinterpret_cast<std::intptr_t>(caller));
        return;
    }
    work->remaining = remaining;
    work->callback = callback;
    work->caller = caller;
    work->startDelay = startDelay;
    work->mode = 1;
    work->dialogue = nullptr;
    if (startDelay > 0) {
        std::memcpy(work + 1, voice, length + 1);
        return;
    }
    work->dialogue = EV_PlayDialogue(voice);
    if (work->dialogue) {
        if (callback)
            callback(1, 0);
    } else {
        HLib::DestroyTask(task);
        if (callback)
            callback(0, reinterpret_cast<std::intptr_t>(caller));
    }
}
void AnanVoiceLoop(void *data) {
    AnanVoiceWork *work = static_cast<AnanVoiceWork *>(data);
    if (!work) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (work->startDelay-- == 1) {
        if (work->mode) {
            work->dialogue = EV_PlayDialogue(reinterpret_cast<const char *>(work + 1));
            if (!work->dialogue) {
                HLib::DestroyCurrentTask();
                if (work->callback)
                    work->callback(0, reinterpret_cast<std::intptr_t>(work->caller));
            } else if (work->callback)
                work->callback(1, 0);
        } else {
            dbgPrintEventMessage(reinterpret_cast<const char *>(work + 1));
            if (work->callback)
                work->callback(1, 0);
        }
    }
    if (work->remaining-- == 1) {
        if (work->startDelay <= 0) {
            if (work->mode) {
                if (work->dialogue && cvsHasValidConverseData(work->dialogue))
                    scenCvsCtrlFree(work->dialogue);
                work->dialogue = nullptr;
            } else
                ctrlStopSubtitles();
        }
        HLib::DestroyCurrentTask();
        if (work->callback)
            work->callback(3, reinterpret_cast<std::intptr_t>(work->caller));
    }
    if (work->callback)
        work->callback(2, work->remaining);
}
void AnanVoiceResumeCaller(std::int64_t event, std::intptr_t argument) {
    if (event == 3)
        HLib::ResolveDeferredCallback(reinterpret_cast<HLib::Task *>(argument));
}
void AnanVoiceUnlockInput(std::int64_t event, std::intptr_t argument) {
    if (event != 3)
        return;
    AnanWork *work = static_cast<AnanWork *>(HLib::GetTaskParameter(reinterpret_cast<HLib::Task *>(argument)));
    if (work)
        work->gameFlags &= ~0x80000000U;
}
void QueueAnanCamera(std::int32_t camera, std::int32_t delay) {
    if (delay <= 0) {
        AnanSelectCamera(camera);
        return;
    }
    HLib::Task *task = HLib::EnqueueTaskWithParameter(AnanCamera, 4, 11, 20, ANAN_EVENT);
    if (!task)
        return;
    AnanCameraWork *work = static_cast<AnanCameraWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    work->camera = camera;
    work->target = ANAN_BASE;
    work->delay = delay;
    std::memcpy(work->packageBytes, ananCameraPackageBytes, 20);
    work->packageBytes[20] = 0;
}
void AnanCamera(void *data) {
    AnanCameraWork *work = static_cast<AnanCameraWork *>(data);
    if (work) {
        if (--work->delay > 0)
            return;
        if (work->packageBytes[0])
            EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>(work->packageBytes), 0);
        EVS_LoopCamera(work->camera, work->target, work->target);
    }
    HLib::DestroyCurrentTask();
}
void QueueAnanWait() {
    HLib::Task *caller = HLib::GetCurrentTask();
    if (!caller)
        return;
    HLib::Task *task = HLib::EnqueueTaskWithParameter(AnanWait, 4, 11, 12, ANAN_EVENT);
    if (!task)
        return;
    AnanWaitWork *work = static_cast<AnanWaitWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    work->caller = caller;
    work->updatePose = 0;
}
void AnanWait(void *data) {
    AnanWaitWork *work = static_cast<AnanWaitWork *>(data);
    if (!work) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (work->caller) {
        AnanWork *parent = static_cast<AnanWork *>(HLib::GetTaskParameter(work->caller));
        if (parent) {
            if (work->updatePose)
                AnanUpdatePose(parent);
            if (parent->gameFlags & 0x80000000U)
                return;
        }
    }
    if (work->caller)
        HLib::ResolveDeferredCallback(work->caller);
    HLib::DestroyCurrentTask();
}
} // namespace shenmue::s2::areas
