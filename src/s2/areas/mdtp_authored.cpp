#include "s2/areas/area_mdtp.hpp"
#include "s2/areas/mdtp_tables.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/door.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/load_title.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include <cstring>
namespace shenmue::s2 {
HLib::Task *mdtpSubtitleTask;
float mdtpSubtitleSeconds;
namespace {
void RestoreRen() {
    if (!DoesScenarioCharacterExist(MDTP_REN))
        return;
    if (scenFindMotionwork(MDTP_REN)) {
        EV_SetCharacterAlphaEnabled(MDTP_REN, true);
        mtwkSetExtraFlag(scenFindMotionwork(MDTP_REN), 6, 0);
        EV_SetRefBoardParamsFor(MDTP_REN, 0);
    }
    EV_SetCharaControllerFlags(MDTP_REN, 0, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(MDTP_REN, 1, NpcCharacterControllerState::Enabled);
    EV_npcFree(MDTP_REN);
    EV_npcLockUnlock(MDTP_REN);
}
} // namespace

void SetupAuthorOfIdAtTime(std::int32_t index) {
    const shen_name source = index ? MDTP_LOADED_RECORDER : MDTP_EMPTY_RECORDER;
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(source, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(source, &rotation, EventTransformFlags::World);
    EV_AT_SetPos(MDTP_RECORDER, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(MDTP_RECORDER, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    EV_EnableCharacterDisplay(MDTP_RECORDER);
    if (index)
        EV_EnableCharacterDisplay(MDTP_TAPE);
    else
        EV_DisableCharacterDisplay(MDTP_TAPE);
}

const char *ResolveMdtpSubtitle(const char *token) {
    if (!token || !*token)
        return "";
    if (GetConversationAudioLanguage() == 1) {
        char alternate[13]{'Z', 'Z'};
        for (std::size_t index = 0; index < 10 && token[index]; ++index)
            alternate[index + 2] = token[index];

        const std::string_view text = GetLocalizedSceneText(alternate);
        if (!text.empty())
            return text.data();
    }
    const std::string_view text = GetLocalizedSceneText(token);
    return text.empty() ? " " : text.data();
}

void MdtpSubtitleCallback(void *data) {
    MdtpSubtitleWork &work = *static_cast<MdtpSubtitleWork *>(data);
    if (work.state) {
        if (work.state == 1 && mdtpSubtitleSeconds > work.deadline) {
            ctrlStopSubtitles();
            ++work.cursor;
            work.state = 0;
        }
    } else if (work.cursor->token) {
        dbgPrintEventMessage(ResolveMdtpSubtitle(work.cursor->token));
        float duration = work.cursor->duration;
        if (GetConversationAudioLanguage() == 0 && work.cursor->alternateDuration != 0.0f)
            duration = work.cursor->alternateDuration;
        ++work.state;
        work.deadline += duration;
    } else if (mdtpSubtitleTask) {
        HLib::DestroyTaskInternal(mdtpSubtitleTask);
        mdtpSubtitleTask = nullptr;
    }
}

MdtpSubtitleWork *QueueMdtpSubtitles(std::int32_t clip) {
    if (mdtpSubtitleTask) {
        HLib::DestroyTaskInternal(mdtpSubtitleTask);
        mdtpSubtitleTask = nullptr;
    }
    mdtpSubtitleTask = HLib::EnqueueTaskWithParameter(MdtpSubtitleCallback, 5, 0x0b, sizeof(MdtpSubtitleWork), AREA_EVENT_TASK);
    MdtpSubtitleWork *const work = static_cast<MdtpSubtitleWork *>(HLib::GetTaskParameter(mdtpSubtitleTask));
    mdtpSubtitleSeconds = 0;
    work->reserved04 = 0;
    work->deadline = 0;
    work->cursor = MDTP_SUBTITLE_TRACKS[clip];
    work->state = 0;

    return work;
}

void MdtpAuthorCallback(void *data) {
    MdtpAuthorWork &work = *static_cast<MdtpAuthorWork *>(data);
    if (!(work.flags & 1)) {
        if (!EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load,
                reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)), reinterpret_cast<std::uintptr_t>(work.package)))
            return;
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        work.flags |= 1;
    }
    if (!(work.flags & 2)) {
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(work.author.name));
        work.flags |= 2;
        if (work.completion)
            work.completion(0, 0, work.parent);
    }
    const std::int32_t frame = work.frame++;
    if (work.completion)
        work.completion(2, frame, work.parent);
    const bool finished = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) == 0;
    if (finished || (work.endFrame >= 0 && frame >= work.endFrame)) {
        if (work.completion)
            work.completion(1, 0, work.parent);
        if (!finished)
            EV_authSceneCtrl(AuthSceneCtrl::RefreshAndEnqueue);
        if (work.package && EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(work.package), 0))
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(work.package), 0);
        if (work.flags & 0x80000000U)
            work.flags &= 0x7fffffffU;
        else
            HLib::DestroyCurrentTask();
    }
}

bool QueueMdtpAuthor(shen_name author, AreaLoopCompletion completion) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(MdtpAuthorCallback, 4, 0x0b, sizeof(MdtpAuthorWork) + 5, AREA_EVENT_TASK);
    if (!task)
        return false;
    MdtpAuthorWork *const work = static_cast<MdtpAuthorWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return false;
    }
    work->parent = HLib::GetCurrentTask();
    work->completion = completion;
    work->flags = 0;
    work->package = reinterpret_cast<char *>(work + 1);
    work->author = author;
    work->frame = 0;
    work->endFrame = -1;

    std::memcpy(const_cast<char *>(work->package), MDTP_AUTHOR_PACKAGE, 6);
    work->flags |= 0x80000000U;
    MdtpAuthorCallback(work);
    if (work->flags & 0x80000000U)
        work->flags &= 0x7fffffffU;
    else
        HLib::DestroyTask(task);
    return true;
}

const MdtpTape *FindMdtpTape(const char *package, std::uint32_t index) {
    if (!EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(package), 0))
        return nullptr;
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::GetFlags, reinterpret_cast<std::uintptr_t>(package), 0);
    char filename[] = "umi000.bin";
    filename[3] = static_cast<char>('0' + index / 100);
    filename[4] = static_cast<char>('0' + index / 10 % 10);
    filename[5] = static_cast<char>('0' + index % 10);
    const MdtpTape *const tape = static_cast<const MdtpTape *>(LoadCurrentPakBinary(filename));
    return tape && tape->signature.name == MDTP_TAPE_SIGNATURE.name ? tape : nullptr;
}
const MdtpTapeClip *GetMdtpTapeClip(const MdtpTape *tape, std::int32_t index) {
    if (!tape)
        return nullptr;
    const MdtpTapeEntry *const entries = reinterpret_cast<const MdtpTapeEntry *>(tape + 1);
    return entries[index].offset ? reinterpret_cast<const MdtpTapeClip *>(reinterpret_cast<const std::byte *>(tape) + entries[index].offset) : nullptr;
}
std::int32_t FindMdtpTapeClip(const MdtpTape *tape, std::int32_t frame) {
    for (std::int32_t index = 0; index < tape->count; ++index) {
        const MdtpTapeClip *const clip = GetMdtpTapeClip(tape, index);
        if (clip && clip->start <= frame && clip->start + clip->duration > frame)
            return index;
    }
    return -1;
}

std::int64_t MdtpInitAuthorCompletion(std::int32_t phase, std::int64_t frame, HLib::Task *parent) {
    if (!phase)
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    else if (phase == 1) {
        if (EV_authSceneCtrl(AuthSceneCtrl::GetCantSkipAuthor))
            SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        SetDoorSubjectEnabled(MDTP_DOOR, 1);
        RestoreRen();
        EV_EnableCharacterDisplay(MDTP_DESK);
        EV_EnableCharacterDisplay(MDTP_LAMP);
        EV_EnableCharacterDisplay(MDTP_CHAIR);
        EV_EnableCharacterDisplay(MDTP_RECORDER);
        EV_EnableCharacterDisplay(MDTP_TAPE);
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        if (parent) {
            HLib::ResolveDeferredCallback(parent);
            HLib::SetupTimedTask(parent, 1);
        }
    } else if (phase == 2) {
        if (frame == 2600) {
            EV_authSceneCtrl(AuthSceneCtrl::SetAuthorStopped);
            DateTime time;
            GetCurrentDateTime(time);
            if (time.hour > 7)
                ++time.day;
            time.hour = 8;
            time.minute = 0;
            SetCurrentDateTimeTo(time);
            EV_stepSetFlags(0, 100);
            EV_stepSetFlags(0, 0x12f);
            EV_setHoldStatesFlags(2);
            Queue_ST_LoadTitleFadeIn(MDTP_NEXT_DAY);
        } else if (frame == 2700) {
            EV_authSceneCtrl(AuthSceneCtrl::ResetAuthorStopped);
            ST_LoadTitleFade();
        }
        if (frame == EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining) - 32)
            SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
    }
    return 1;
}
std::int64_t MdtpQuitAuthorCompletion(std::int32_t phase, std::int64_t frame, HLib::Task *parent) {
    if (!phase)
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    else if (phase == 1) {
        if (EV_authSceneCtrl(AuthSceneCtrl::GetCantSkipAuthor))
            SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        SetDoorSubjectEnabled(MDTP_DOOR, 1);
        RestoreRen();
        EV_EnableCharacterDisplay(MDTP_DESK);
        EV_EnableCharacterDisplay(MDTP_LAMP);
        EV_EnableCharacterDisplay(MDTP_CHAIR);
        EV_EnableCharacterDisplay(MDTP_RECORDER);
        EV_EnableCharacterDisplay(MDTP_TAPE);
        if (parent) {
            HLib::ResolveDeferredCallback(parent);
            HLib::SetupTimedTask(parent, 1);
        }
    } else if (phase == 2 && frame == EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining) - 32)
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
    return 1;
}
} // namespace shenmue::s2
