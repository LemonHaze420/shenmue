#include "s2/areas/area_mdtp.hpp"
#include "s2/audio_data.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/door.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <cmath>
#include <cstring>
#include <initializer_list>
namespace shenmue::s2 {
namespace {
float CleanFloat(float value) {
    return std::fabs(value) < 0.00000011920929f ? 0.0f : value;
}
const MdtpTapeEntry *TapeEntries(const MdtpTape *tape) {
    return reinterpret_cast<const MdtpTapeEntry *>(tape + 1);
}
void StopSubtitlesTask() {
    if (mdtpSubtitleTask) {
        HLib::DestroyTaskInternal(mdtpSubtitleTask);
        mdtpSubtitleTask = nullptr;
    }
}
void StopTapeVoice(ConversationBuffer *&voice, std::int32_t &clip) {
    if (voice && cvsHasValidConverseData(voice)) {
        scenCvsCtrlFree(voice);
        voice = nullptr;
        clip = -1;
        StopSubtitlesTask();
    }
}
void BeginRewind(MdtpWork &work, std::int32_t previous) {
    if (((previous - 154) & ~2) == 0) {
        StopTapeVoice(work.voice, work.voiceClip);
        StopTapeVoice(work.pausedVoice, work.pausedClip);
    }
    std::int32_t clip;
    while ((clip = FindMdtpTapeClip(work.tape, work.tapeFrame)) < 0)
        --work.tapeFrame;
    const MdtpTapeEntry *const entries = TapeEntries(work.tape);
    while (clip > 0 && !(entries[clip].flags & 2))
        --clip;
    if (entries[clip].flags & 2) {
        if (clip <= 0) {
            work.seekClip = clip;
            return;
        }
        const MdtpTapeClip *const record = GetMdtpTapeClip(work.tape, clip);
        const std::int32_t margin = record->duration / 10 < 15 ? record->duration / 10 : 15;
        if (record->start + margin > work.tapeFrame)
            --clip;
    }
    while (clip > 0 && !(entries[clip].flags & 2))
        --clip;
    work.seekClip = clip;
}
void BeginPlay(MdtpWork &work) {
    if (work.bingoClip >= 0)
        return;
    std::int32_t clip = FindMdtpTapeClip(work.tape, work.tapeFrame);
    if (clip >= 0) {
        if (GetMdtpTapeClip(work.tape, clip)->start < work.tapeFrame)
            ++clip;
        const MdtpTapeEntry *const entries = TapeEntries(work.tape);
        while (clip < work.tape->count && !(entries[clip].flags & 2))
            ++clip;
        if (clip >= work.tape->count)
            clip = -1;
    }
    work.bingoClip = clip;
}
void BeginFastForward(MdtpWork &work, std::int32_t previous) {
    if (((previous - 154) & ~2) == 0) {
        StopTapeVoice(work.voice, work.voiceClip);
        StopTapeVoice(work.pausedVoice, work.pausedClip);
    }
    std::int32_t clip = FindMdtpTapeClip(work.tape, work.tapeFrame);
    if (clip < 0) {
        work.seekClip = -1;
        return;
    }
    const MdtpTapeEntry *const entries = TapeEntries(work.tape);
    if (entries[clip].flags & 4) {
        const MdtpTapeClip *const record = GetMdtpTapeClip(work.tape, clip);
        const std::int32_t margin = record->duration >= 150 ? record->duration - 15 : 9 * (record->duration / 10);
        if (record->start + margin < work.tapeFrame && ++clip > work.tape->count) {
            work.seekClip = work.tape->count;
            return;
        }
    }
    ++clip;
    while (clip < work.tape->count && !(entries[clip].flags & 2))
        ++clip;
    work.seekClip = clip;
}
void BeginStop(MdtpWork &work, std::int32_t previous) {
    if (previous != 154 || !work.voice)
        return;
    if (cvsHasValidConverseData(work.voice)) {
        PauseConversation(work.voice, true);
        work.pausedVoice = work.voice;
        work.pausedClip = work.voiceClip;
        work.voice = nullptr;
        work.voiceClip = -1;
        if (mdtpSubtitleTask) {
            ctrlStopSubtitles();
            HLib::SetupTimedTask(mdtpSubtitleTask, 0);
        }
    } else {
        work.voiceClip = -1;
        work.voice = nullptr;
        work.pausedVoice = nullptr;
    }
}
void RewindTape(MdtpWork &work) {
    if (work.seekClip < 0) {
        work.flags |= 0x1000;
        return;
    }
    work.tapeFrame = work.tapeFrame <= 10 ? 0 : work.tapeFrame - 10;
    if (work.tapeFrame < 0)
        work.tapeFrame = 0;
    const std::int32_t clip = FindMdtpTapeClip(work.tape, work.tapeFrame);
    if (clip < work.seekClip || (clip == work.seekClip && GetMdtpTapeClip(work.tape, work.seekClip)->start >= work.tapeFrame)) {
        work.tapeFrame = GetMdtpTapeClip(work.tape, work.seekClip)->start;
        work.seekClip = -1;
        work.flags |= 0x1000;
    }
}
bool PlayTape(MdtpWork &work) {
    const std::int32_t clip = FindMdtpTapeClip(work.tape, work.tapeFrame);
    ++work.tapeFrame;
    bool bingo = false;
    if (clip < 0) {
        while (work.tapeFrame > 0 && FindMdtpTapeClip(work.tape, work.tapeFrame - 1) < 0)
            --work.tapeFrame;
        work.flags |= 0x1000;
    } else {
        bingo = work.bingoClip >= 0 && work.bingoClip <= clip && (TapeEntries(work.tape)[clip].flags & 5) == 5 &&
            clip != FindMdtpTapeClip(work.tape, work.tapeFrame);
        const MdtpTapeClip *const record = GetMdtpTapeClip(work.tape, clip);
        bool begin = false;
        if (!work.voice) {
            if (work.pausedClip == clip && work.pausedVoice && GetConversationPaused(work.pausedVoice)) {
                PauseConversation(work.pausedVoice, false);
                work.voice = work.pausedVoice;
                work.voiceClip = work.pausedClip;
                work.pausedClip = -1;
                work.pausedVoice = nullptr;
                if (mdtpSubtitleTask) {
                    MdtpSubtitleWork *const subtitles = static_cast<MdtpSubtitleWork *>(HLib::GetTaskParameter(mdtpSubtitleTask));
                    dbgPrintEventMessage(ResolveMdtpSubtitle(subtitles->cursor->token));
                    HLib::ResolveDeferredCallback(mdtpSubtitleTask);
                }
            } else if (work.voiceClip != clip) {
                if (work.pausedVoice && GetConversationPaused(work.pausedVoice)) {
                    scenCvsCtrlFree(work.pausedVoice);
                    work.pausedVoice = nullptr;
                    work.pausedClip = -1;
                }
                begin = true;
            }
        } else if (!cvsHasValidConverseData(work.voice)) {
            work.voice = nullptr;
            work.pausedVoice = nullptr;
            work.pausedClip = -1;
        } else if (work.voiceClip != clip) {
            scenCvsCtrlFree(work.voice);
            work.voice = nullptr;
            begin = true;
        }
        if (begin && record) {
            const char *const base = reinterpret_cast<const char *>(work.tape);
            const char *const bank = record->bankOffset ? base + record->bankOffset : nullptr;
            const char *const voice = record->voiceOffset ? base + record->voiceOffset : nullptr;
            if (bank && *bank && voice && *voice) {
                bool same = false;
                if (work.bank) {
                    std::size_t index = 0;
                    while (bank[index] && bank[index] == work.bank[index])
                        ++index;
                    same = bank[index] == 0;
                    if (!same) {
                        EV_FreeAuthorPak(0, work.bank);
                        StopSubtitlesTask();
                    }
                }
                if (!same) {
                    EV_FreeAuthorPak(1, bank);
                    work.bank = bank;
                }
                work.voice = shenPlayAudioWithFlags(voice, 0);
                QueueMdtpSubtitles(clip - 1);
                work.voiceClip = clip;
            }
        }
    }
    if (work.voice)
        mdtpSubtitleSeconds = GetConversationPlaybackSeconds(work.voice);
    return bingo;
}
void FastForwardTape(MdtpWork &work) {
    if (work.seekClip < 0) {
        work.flags |= 0x1000;
        return;
    }
    work.tapeFrame += 10;
    std::int32_t clip = FindMdtpTapeClip(work.tape, work.tapeFrame);
    const MdtpTapeClip *record = nullptr;
    if (clip >= 0)
        record = GetMdtpTapeClip(work.tape, clip);
    else {
        while ((clip = FindMdtpTapeClip(work.tape, work.tapeFrame - 1)) < 0)
            --work.tapeFrame;
        ++clip;
    }
    if (work.flags & 0x1000)
        work.seekClip = -1;
    else if (clip == work.seekClip) {
        work.seekClip = -1;
        if (record)
            work.tapeFrame = record->start;
        work.flags |= 0x1000;
    }
}
void RecorderPlayNullCallback() {
}
void IdleMotionCallback(void *data) {
    AreaLoopMotionWork *const work = static_cast<AreaLoopMotionWork *>(data);
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
        if (work->completion)
            work->completion(0, work->character.name, work->parent);
        work->flags |= 0x40000000U;
    }

    EV_mtwkSetEnabled(scenFindMotionwork(work->character), work->flags & 4);
    if (work->completion)
        work->completion(2, work->elapsed, work->parent);
    if ((work->duration < static_cast<std::int32_t>(++work->elapsed) && !(work->flags & 0x10)) ||
        (SetMotionRegionFlag(scenFindMotionwork(work->character), 65, 1, MotionRegion::LowerBody) && (work->flags & 0x10))) {
        if (work->completion)
            work->completion(1, 0, work->parent);
        if (work->flags & 0x80000000U)
            work->flags &= 0x7fffffffU;
        else
            HLib::DestroyCurrentTask();
    }
}
void QueueCamera(const char *package, std::int32_t camera) {
    const std::size_t length = package && *package ? std::strlen(package) : 0;
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AreaLoopCameraCallback, 4, 0x0b, sizeof(AreaLoopCameraWork) + length + 1, AREA_EVENT_TASK);
    if (!task)
        return;
    AreaLoopCameraWork *const work = static_cast<AreaLoopCameraWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    work->parent = HLib::GetCurrentTask();
    work->area = reinterpret_cast<char *>(work + 1);
    work->flags = 0;
    work->camera = camera;
    work->character = MDTP_BASE;
    work->completion = nullptr;
    std::memcpy(work + 1, package, length + 1);
    work->flags |= 0x80000000U;
    AreaLoopCameraCallback(work);
    if (work->flags & 0x80000000U)
        work->flags &= 0x7fffffffU;
    else
        HLib::DestroyTask(task);
}
void QueueIdleMotion() {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(IdleMotionCallback, 4, 0x0b, sizeof(AreaLoopMotionWork), AREA_EVENT_TASK);
    if (!task)
        return;
    AreaLoopMotionWork *const work = static_cast<AreaLoopMotionWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    work->parent = HLib::GetCurrentTask();
    work->startFrame = -1;
    work->endFrame = -1;
    work->speed = 1;
    work->frameTime = 0;
    work->character = MDTP_PLAYER;
    work->motion = 61443;
    work->duration = -1;
    work->elapsed = 0;
    work->completion = nullptr;
    work->flags = 0x80000002U;
    IdleMotionCallback(work);
    if (work->flags & 0x80000000U)
        work->flags &= 0x7fffffffU;
    else
        HLib::DestroyTask(task);
}
void RestorePosition(void *data) {
    AreaLoopPositionWork *const work = static_cast<AreaLoopPositionWork *>(data);
    if (!work) {
        HLib::DestroyCurrentTask();
        return;
    }
    const std::uint32_t mode = (work->flags & 4) ? 0x38000000U : 0x78000000U;
    if (work->flags & 1)
        EV_AT_SetPos(work->character, mode, work->position);
    if (work->flags & 2)
        EV_AT_SetAngY(work->character, mode, work->rotation);
    if (work->completion)
        work->completion(0, work->character.name, work->parent);
    if (work->flags & 0x80000000U)
        work->flags &= 0x7fffffffU;
    else
        HLib::DestroyCurrentTask();
}
void QueueRestorePosition() {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(RestorePosition, 4, 0x0b, sizeof(AreaLoopPositionWork), AREA_EVENT_TASK);
    if (!task)
        return;
    AreaLoopPositionWork *const work = static_cast<AreaLoopPositionWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    work->position = {.x = 105.87f, .y = 89.27f, .z = 153.67f};
    work->character = MDTP_PLAYER;
    work->rotation = {.x = 0, .y = 2621, .z = 0};
    work->parent = HLib::GetCurrentTask();
    work->completion = nullptr;
    work->flags = 0x80000003U;
    RestorePosition(work);
    if (work->flags & 0x80000000U)
        work->flags &= 0x7fffffffU;
    else
        HLib::DestroyTask(task);
}
void BeginPackage(const char *package) {
    if (!package || !*package)
        return;
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(package), 0)) {
        SetupAuthorOfIdAtTime(1);
        return;
    }
    const std::size_t length = std::strlen(package);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(MdtpPackageCallback, 4, 0x0b, length + 32, AREA_EVENT_TASK);
    if (!task)
        return;
    MdtpPackageWork *const work = static_cast<MdtpPackageWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    work->parent = HLib::GetCurrentTask();
    work->task = task;
    work->recorder = 1;
    if (GetNpcNewLoadState()) {
        work->npcState = 1;
        EV_NPC_NewLoadSw(0);
    } else
        work->npcState = 0;
    if (!EV_ResetFreeTalkFlags())
        work->npcState = -1;
    std::memcpy(work + 1, package, length + 1);
    if (work->parent)
        HLib::SetupTimedTask(work->parent, 0);
}
void ScriptCharacter(shen_name character) {
    if (!character.name)
        return;
    const bool hasMotion = scenFindMotionwork(character) != nullptr;
    EV_SetCharaControllerFlags(character, 1, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, 0, NpcCharacterControllerState::Disabled);
    if (hasMotion) {
        const Vector4 reflector{.x = 1, .y = 1, .z = 1, .w = 1};
        EV_SetRefBoardParamsFor(character, 1);
        SetScenarioReflectorBoardValues(character, &reflector);
        mtwkSetExtraFlag(scenFindMotionwork(character), 6, 2);
        EV_SetCharacterAlphaEnabled(character, false);
    }
}
void RestoreCharacter(shen_name character) {
    if (scenFindMotionwork(character)) {
        EV_SetCharacterAlphaEnabled(character, true);
        mtwkSetExtraFlag(scenFindMotionwork(character), 6, 0);
        EV_SetRefBoardParamsFor(character, 0);
    }
    EV_SetCharaControllerFlags(character, 0, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(character, 1, NpcCharacterControllerState::Enabled);
}
void NpcCallback(void *data) {
    AreaLoopNpcWork *const work = static_cast<AreaLoopNpcWork *>(data);
    if (!work) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (work->remaining) {
        EV_CheckWarmStartNpcRoute(work->characters[work->remaining - 1]);
        while (work->remaining) {
            const std::uint64_t state = EV_npcStreamOrLoadHuman(work->characters[work->remaining - 1]);
            if (!state)
                break;
            if (state != 1) {
                --work->remaining;
                break;
            }
            ScriptCharacter(work->characters[work->remaining - 1]);
            if (--work->remaining == 0)
                break;
        }
    }
    if (work->completion)
        work->completion(2, work->remaining, work->parent);
    if (!work->remaining) {
        if (work->previousLoadState)
            EV_NPC_NewLoadSw(1);
        if (work->completion)
            work->completion(1, 0, work->parent);
        HLib::DestroyCurrentTask();
    }
}
void QueueNpcs(const shen_name *characters) {
    if (!characters)
        return;
    std::int32_t count = 0;
    while (characters[count].name)
        ++count;
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(NpcCallback, 4, 0x0b, sizeof(AreaLoopNpcWork) + count * sizeof(shen_name), AREA_EVENT_TASK);
    if (!task)
        return;
    AreaLoopNpcWork *const work = static_cast<AreaLoopNpcWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    work->parent = HLib::GetCurrentTask();
    work->previousLoadState = GetNpcNewLoadState();
    work->remaining = count;
    work->characters = reinterpret_cast<shen_name *>(work + 1);
    work->completion = ResolveAreaLoopParent;
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    std::memcpy(work->characters, characters, count * sizeof(shen_name));
}
void HideRecorderRoom() {
    EV_DisableCharacterDisplay(MDTP_DESK);
    EV_DisableCharacterDisplay(MDTP_LAMP);
    EV_DisableCharacterDisplay(MDTP_CHAIR);
    EV_DisableCharacterDisplay(MDTP_RECORDER);
    EV_DisableCharacterDisplay(MDTP_TAPE);
}
void UpdateRecorderParts(MdtpWork &work) {
    const std::int32_t frame = work.tapeFrame;
    float counters[3]{static_cast<float>(frame) * 1.2f, static_cast<float>(frame / 300) * 36.0f, static_cast<float>(frame / 3000) * 36.0f};
    if (frame % 300 > 270)
        counters[1] = CleanFloat(counters[1]) + ((static_cast<float>(frame - 270) / 30.0f) * 36.0f);
    counters[1] = CleanFloat(counters[1]);
    if (frame % 3000 > 2970)
        counters[2] = CleanFloat(counters[2]) + ((static_cast<float>(frame - 2970) / 30.0f) * 36.0f);
    counters[2] = CleanFloat(counters[2]);
    Vector3 position;
    Vector3i rotation;
    for (std::int32_t index = 0; index < 3; ++index) {
        EV_scenApplyPosOrRotToPartOf(MDTP_RECORDER, MDTP_COUNTER_PARTS[index], &position, &rotation, EventCharacterPartTransformMode::Get);
        rotation.x = static_cast<std::uint16_t>(static_cast<std::int32_t>((-counters[index] * 8192.0f) / 45.0f));
        EV_scenApplyPosOrRotToPartOf(MDTP_RECORDER, MDTP_COUNTER_PARTS[index], &position, &rotation, EventCharacterPartTransformMode::Absolute);
    }
    const float reel = CleanFloat((static_cast<float>(frame) * 360.0f) * 0.016666668f);
    for (const std::uint16_t part : {std::uint16_t{0xa7}, std::uint16_t{0xa6}}) {
        EV_scenApplyPosOrRotToPartOf(MDTP_RECORDER, part, &position, &rotation, EventCharacterPartTransformMode::Get);
        rotation.y = static_cast<std::uint16_t>(static_cast<std::int32_t>((reel * 8192.0f) / 45.0f));
        EV_scenApplyPosOrRotToPartOf(MDTP_RECORDER, part, &position, &rotation, EventCharacterPartTransformMode::Absolute);
    }
    EV_scenApplyPosOrRotToPartOf(MDTP_RECORDER, 0xa4, &position, &rotation, EventCharacterPartTransformMode::Get);
    float meter = static_cast<float>(rotation.y) / 182.04445f;
    if (work.button == 154)
        meter = CleanFloat(CleanFloat(meter * 0.01125f) + GetRandomNumberInRange(0.1f)) * 80.0f;
    else
        meter *= 0.85000002f;
    meter = CleanFloat(meter);
    rotation.y = static_cast<std::uint16_t>(static_cast<std::int32_t>((meter * 8192.0f) / 45.0f));
    EV_scenApplyPosOrRotToPartOf(MDTP_RECORDER, 0xa4, &position, &rotation, EventCharacterPartTransformMode::Absolute);
    const MdtpTapeClip *const last = GetMdtpTapeClip(work.tape, work.tape->count - 1);
    const float winding = CleanFloat(static_cast<float>(frame) / static_cast<float>(2 * (last->start + last->duration) - 2));
    position = {.x = winding + 0.5f, .y = 1, .z = winding + 0.5f};
    EV_scenApplyPosOrRotToPartOf(MDTP_TAPE, 0x12, &position, nullptr, EventCharacterPartTransformMode::Set);
    position.x = position.z = CleanFloat(0.5f - winding) + 0.5f;
    EV_scenApplyPosOrRotToPartOf(MDTP_TAPE, 0x17, &position, nullptr, EventCharacterPartTransformMode::Set);
}
} // namespace

void MDTPInit() {
    mdtpSubtitleTask = nullptr;
}
void MDTPMain(std::uintptr_t argument) {
    if (!argument)
        return;
    const MdtpCommand &command = *reinterpret_cast<const MdtpCommand *>(argument);
    if (command.command == 0 || command.command == 1) {
        SetupAuthorOfIdAtTime(command.command);
        SetPlayerControlEnabled(1);
    } else
        QueueMdtp(command.command, command.tape);
}
bool QueueMdtp(std::int32_t command, std::uint32_t tape) {
    if (tape > 999)
        return false;
    HLib::Task *const pause = HLib::EnqueueTaskWithParameter(MdtpPauseCallback, 15, 0x0b, sizeof(MdtpPauseWork), AREA_EVENT_TASK);
    if (!pause)
        return false;
    MdtpPauseWork *const pauseWork = static_cast<MdtpPauseWork *>(HLib::GetTaskParameter(pause));
    if (!pauseWork) {
        HLib::DestroyTask(pause);
        return false;
    }
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(MdtpInitializeCallback, 4, 0x0b, sizeof(MdtpWork), AREA_EVENT_TASK);
    if (!task) {
        HLib::DestroyTask(pause);
        return false;
    }
    MdtpWork *const work = static_cast<MdtpWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        HLib::DestroyTask(pause);
        return false;
    }
    std::memset(work, 0, sizeof(*work));
    work->parent = HLib::GetCurrentTask();
    work->task = task;
    work->pauseTask = pause;
    work->package = MDTP_PACKAGE;
    work->media = MDTP_MEDIA;
    work->tapeIndex = tape;
    work->flags = (command == 2 ? 513U : 1U) | (command == 4 ? 1024U : 0U);
    pauseWork->owner = work;
    pauseWork->phase = 0;
    EV_EventSetUpHelpIcon_0(62, 0);
    return true;
}
void MdtpPauseCallback(void *data) {
    MdtpPauseWork *const work = static_cast<MdtpPauseWork *>(data);
    if (!work || !work->owner)
        return;
    const bool paused = shenIsGamePaused() != 0;
    work->owner->flags &= ~0x10000U;
    switch (work->phase) {
    case 1:
    case 3:
        work->phase = paused ? 3 : 2;
        break;
    case 2:
        work->phase = paused ? 1 : 4;
        work->owner->flags |= 0x10000;
        break;
    case 4:
        work->phase = paused ? 1 : 4;
        break;
    default:
        work->phase = paused ? 1 : 2;
        break;
    }
}
void MdtpPackageCallback(void *data) {
    MdtpPackageWork *const work = static_cast<MdtpPackageWork *>(data);
    if (!work)
        return;
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>(work + 1))) {
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        SetupAuthorOfIdAtTime(work->recorder);
        if (work->npcState == 1)
            EV_NPC_NewLoadSw(1);
        if (work->parent) {
            HLib::ResolveDeferredCallback(work->parent);
            HLib::SetupTimedTask(work->parent, 1);
        }
        HLib::DestroyCurrentTask();
    } else if (work->npcState == -1) {
        if (GetNpcNewLoadState()) {
            work->npcState = 1;
            EV_NPC_NewLoadSw(0);
        } else
            work->npcState = 0;
        if (!EV_ResetFreeTalkFlags())
            work->npcState = -1;
    }
}
void MdtpInitializeCallback(void *data) {
    MdtpWork *const work = static_cast<MdtpWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(MdtpQuitCallback);
        return;
    }
    HLib::SetCurrentTaskTo(work->flags & 0x200 ? MdtpInitializeAuthorCallback : MdtpInitializeFlagsCallback);
    EV_SetNewSceneIndex();
    work->flags |= 0x80000000U;
    spmFadeOut();
    if (SetPlayerGameFlags(PlayerGameFlagMode::Bit2, reinterpret_cast<void *>(~std::uintptr_t{0})))
        work->flags |= 0x40000000U;
    SetPlayerGameFlags(PlayerGameFlagMode::Bit2, nullptr);
    BeginPackage(work->package);
}
void MdtpInitializeAuthorCallback(void *data) {
    MdtpWork *const work = static_cast<MdtpWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(MdtpQuitCallback);
        return;
    }
    EV_LoadCutsceneFromCurrentScene(work->media);
    EV_LoadAudio(work->media, 0);
    HLib::SetCurrentTaskTo(MdtpBeginAuthorCallback);
    HLib::SetupTaskDelay(-1);
    const shen_name characters[]{MDTP_REN, {}};
    QueueNpcs(characters);
}
void MdtpBeginAuthorCallback(void *data) {
    MdtpWork *const work = static_cast<MdtpWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(MdtpQuitCallback);
        return;
    }
    HLib::SetCurrentTaskTo(MdtpInitializeFlagsCallback);
    HLib::SetupTaskDelay(-1);
    HideRecorderRoom();
    EV_FreeAuthorPak(1, MDTP_AUTHOR_PACKAGE);
    work->bank = MDTP_AUTHOR_PACKAGE;
    SetDoorSubjectEnabled(MDTP_DOOR, 0);
    QueueMdtpAuthor(MDTP_INIT_AUTHOR, MdtpInitAuthorCompletion);
}
void MdtpInitializeFlagsCallback(void *data) {
    MdtpWork *const work = static_cast<MdtpWork *>(data);
    if (!work)
        HLib::SetCurrentTaskTo(MdtpQuitCallback);
    work->tape = FindMdtpTape(work->package, work->tapeIndex);
    if (!work->tape)
        return;
    ScriptCharacter(MDTP_PLAYER);
    ScriptCharacter(MDTP_TAPE);
    ScriptCharacter(MDTP_RECORDER);
    AT_SetLocalBaseChara(MDTP_PLAYER, MDTP_BASE);
    MaintainMdtpMotion(0);
    mtwkRequestMotion(scenFindMotionwork(MDTP_PLAYER), MDTP_MOTION, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(scenFindMotionwork(MDTP_PLAYER), 0);
    SetMotionLayerLoopStartTime(scenFindMotionwork(MDTP_PLAYER), 0, MotionRegion::LowerBody);
    SetMotionRegionEvalTime(scenFindMotionwork(MDTP_PLAYER), 0, MotionRegion::LowerBody);
    SetMotionLayerEndFrame(scenFindMotionwork(MDTP_PLAYER), 114, MotionRegion::LowerBody);
    QueueCamera(work->package, 3000);
    EV_LoadCutsceneFromCurrentScene(work->media);
    EV_LoadAudio(work->media, 1);
    GetMotionBonePosition(scenFindMotionwork(MDTP_PLAYER), MB_RightHand, &work->handPosition);
    work->button = 156;
    for (std::int32_t &frame : work->buttonFrames)
        frame = 0;
    if (work->flags & 0x200) {
        work->tapeFrame = 0;
        work->bingoClip = -1;
    } else {
        Vector3i rotation;
        EV_scenApplyPosOrRotToPartOf(MDTP_RECORDER, 0xa7, nullptr, &rotation, EventCharacterPartTransformMode::Get);
        work->tapeFrame = rotation.y;
        EV_scenApplyPosOrRotToPartOf(MDTP_RECORDER, 0xa6, nullptr, &rotation, EventCharacterPartTransformMode::Get);
        work->bingoClip = rotation.y > 0x8000 ? -1 : rotation.y;
    }
    work->voice = nullptr;
    work->pausedVoice = nullptr;
    work->voiceClip = -1;
    work->pausedClip = -1;
    work->seekClip = -1;
    HLib::SetCurrentTaskTo(MdtpRecorderCallback);
}
void MdtpRecorderCallback(void *data) {
    MdtpWork *const work = static_cast<MdtpWork *>(data);
    DateTime time;
    GetCurrentDateTime(time);
    if (!shenGetMinutesUntilNextFrameMinute()) {
        if (work->button != 154 || !(TapeEntries(work->tape)[FindMdtpTapeClip(work->tape, work->tapeFrame)].flags & 1)) {
            HLib::SetCurrentTaskTo(MdtpQuitCallback);
            return;
        }
    }
    MaintainMdtpMotion(1);
    if (!work) {
        HLib::SetCurrentTaskTo(MdtpQuitCallback);
        return;
    }
    bool inputAllowed = true;
    if (filmCheckFadeTrackColour())
        inputAllowed = false;
    else if (work->flags & 1) {
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        work->flags &= ~1U;
        QueueCamera(work->package, 3000);
        inputAllowed = false;
    }
    std::uint16_t input = 0;
    if (!work->pressFrames && inputAllowed) {
        input = static_cast<std::uint16_t>(GetCurrentControlInput(INPUT_REPEAT)) & 0xf9f9;
        if (work->flags & 0x2000) {
            input = 0;
            if (!work->handBlendFrames) {
                if (work->button == 156) {
                    for (std::int32_t &frame : work->buttonFrames)
                        frame = 0;
                    HLib::SetCurrentTaskTo(MdtpBeginExitQuestionCallback);
                } else
                    input = 1024;
            }
        } else if (work->button == 152 || work->button == 155)
            input &= 0xc600;
        else {
            if (work->button != 154)
                CheckHelpIcon(33, 0, CheckHelpIconOp::FrameOverlay);
            GetBattleDataConfig(0, MDTP_ACCEPT);
            if (GetBattleDataConfig(1, MDTP_ACCEPT) && GetBattleDataConfig(2, MDTP_ACCEPT))
                input |= 0x400;
            GetBattleDataConfig(0, MDTP_CANCEL);
            if (GetBattleDataConfig(1, MDTP_CANCEL) && GetBattleDataConfig(2, MDTP_CANCEL))
                input |= 0x200;
        }
    }
    if (!(work->flags & 2)) {
        SetLowQualityHands(ctrlGetCharaByName(MDTP_PLAYER));
        SetHighModelXYZ(ctrlGetCharaByName(MDTP_PLAYER), 1, 0, 0);
        work->flags |= 2;
        SetScenarioCharacterDrawControl(MDTP_PLAYER, 1, 4);
        work->pressFrames = 1;
    }
    if (input & 0x200) {
        const std::int32_t previous = work->selectedButton;
        work->flags |= 0x2000;
        work->selectedButton = 2;
        work->handBlendFrames = 4 * previous + 8;
    } else if (input & 0x4000) {
        if (!work->selectedButton)
            work->selectedButton = -1;
        --work->selectedButton;
        work->handBlendFrames = 5;
        if (work->selectedButton < -2)
            work->selectedButton = -2;
    } else if (input & 0x8000) {
        if (work->selectedButton == -2)
            work->selectedButton = -1;
        ++work->selectedButton;
        work->handBlendFrames = 5;
        if (work->selectedButton > 2)
            work->selectedButton = 2;
    } else if (input & 0x1400)
        work->pressFrames = 15;
    const std::int32_t previous = work->button;
    std::int32_t stopping = work->flags & 0x1000;
    const bool changed = UpdateMdtpButtons(work->selectedButton, &work->pressFrames, &work->button, work->buttonFrames, &stopping);
    if (stopping)
        work->flags |= 0x1000;
    else
        work->flags &= ~0x1000U;
    if (changed) {
        Loop3DSoundEffect(0x5b0);
        switch (work->button) {
        case 152:
            work->bingoClip = -1;
            Loop3DSoundEffect(0x5af);
            BeginRewind(*work, previous);
            break;
        case 154:
            RecorderPlayNullCallback();
            BeginPlay(*work);
            break;
        case 155:
            work->bingoClip = -1;
            Loop3DSoundEffect(0x5af);
            BeginFastForward(*work, previous);
            break;
        case 156:
            BeginStop(*work, previous);
            break;
        default:
            break;
        }
    } else if (work->button == 152 || work->button == 155) {
        if (work->button == 152)
            RewindTape(*work);
        else
            FastForwardTape(*work);
        if (work->flags & 0x10000)
            Loop3DSoundEffect(0x5af);
    } else if (work->button == 154 && PlayTape(*work) && !(work->flags & 0x400))
        HLib::SetCurrentTaskTo(MdtpBeginBingoCallback);
    UpdateRecorderParts(*work);
    UpdateMdtpHand(work);
}
void MdtpBeginExitQuestionCallback(void *data) {
    MaintainMdtpMotion(1);
    MdtpWork *const work = static_cast<MdtpWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(MdtpQuitCallback);
        return;
    }
    std::int32_t stopping = 0;
    UpdateMdtpButtons(work->selectedButton, &work->pressFrames, &work->button, work->buttonFrames, &stopping);
    UpdateMdtpHand(work);
    if (filmCheckFadeTrackColour())
        return;
    if (work->flags & 1) {
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        work->flags &= ~1U;
        QueueCamera(work->package, 3001);
        HLib::SetCurrentTaskTo(MdtpExitQuestionCallback);
    } else {
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1;
    }
}
void MdtpExitQuestionCallback(void *data) {
    MaintainMdtpMotion(1);
    MdtpWork *const work = static_cast<MdtpWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(MdtpQuitCallback);
        return;
    }
    std::int32_t stopping = 0;
    UpdateMdtpButtons(work->selectedButton, &work->pressFrames, &work->button, work->buttonFrames, &stopping);
    UpdateMdtpHand(work);
    if (filmCheckFadeTrackColour())
        return;
    GetBattleDataConfig(0, MDTP_ACCEPT);
    if (GetBattleDataConfig(1, MDTP_ACCEPT) && GetBattleDataConfig(2, MDTP_ACCEPT)) {
        if (!(work->flags & 1)) {
            SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->flags |= 1;
        }
        work->flags &= ~0x2000U;
        HLib::SetCurrentTaskTo(MdtpRecorderCallback);
    } else {
        GetBattleDataConfig(0, MDTP_CANCEL);
        if (GetBattleDataConfig(1, MDTP_CANCEL) && GetBattleDataConfig(2, MDTP_CANCEL)) {
            if (!(work->flags & 1)) {
                SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
                work->flags |= 1;
            }
            HLib::SetCurrentTaskTo(MdtpFinishExitCallback);
        }
    }
}
void MdtpFinishExitCallback(void *data) {
    MdtpWork *const work = static_cast<MdtpWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(MdtpQuitCallback);
        return;
    }
    std::int32_t stopping = 0;
    UpdateMdtpButtons(work->selectedButton, &work->pressFrames, &work->button, work->buttonFrames, &stopping);
    UpdateMdtpHand(work);
    if (!filmCheckFadeTrackColour()) {
        HLib::SetCurrentTaskTo(MdtpQuitCallback);
        QueueIdleMotion();
    }
}
void MdtpQuitCallback(void *data) {
    using enum CameraState;
    if (filmCheckFadeTrackColour())
        return;
    MdtpWork *const work = static_cast<MdtpWork *>(data);
    std::int32_t savedFrame = 0, savedBingo = -1;
    Vector3i rotation{.x = 0, .y = 0, .z = 0};
    if (work) {
        if (!(work->flags & 1)) {
            SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->flags |= 1;
            return;
        }
        rotation.y = 2621;
        QueueRestorePosition();
        mtwkSetExtraFlag(scenFindMotionwork(MDTP_PLAYER), 17, 0);
        QueueIdleMotion();
        SetScreenFilmFade(64, 255, 0, 0, 0, 0, 0, 0, 0);
        StopTapeVoice(work->voice, work->voiceClip);
        StopTapeVoice(work->pausedVoice, work->pausedClip);
        const std::int32_t clip = FindMdtpTapeClip(work->tape, work->tapeFrame);
        work->tapeFrame = clip < 0 ? 0 : GetMdtpTapeClip(work->tape, clip)->start;
        if (work->tape && work->bank)
            EV_FreeAuthorPak(0, work->bank);
        if (work->flags & 0x80000000U) {
            LoadCutsceneAndSoundProgramForArea();
            work->flags &= 0x7fffffffU;
        }
        if (work->flags & 2) {
            hndlSetFlags(ctrlGetCharaByName(MDTP_PLAYER));
            work->flags &= ~2U;
        }
        savedFrame = static_cast<std::uint16_t>(work->tapeFrame);
        savedBingo = static_cast<std::uint16_t>(work->bingoClip);
        HLib::DestroyTask(work->pauseTask);
    }
    AT_SetLocalBaseChara(MDTP_PLAYER, MDTP_FREE_BASE);
    EV_EventSetUpHelpIcon_0(0, 0);
    if (EV_GetShenmueFlag(0x10d))
        EV_stepSetFlags(1, 0x10d);
    RestoreCharacter(MDTP_RECORDER);
    RestoreCharacter(MDTP_TAPE);
    RestoreCharacter(MDTP_PLAYER);
    EV_scenApplyPosOrRotToPartOf(MDTP_RECORDER, 0xa7, nullptr, &rotation, EventCharacterPartTransformMode::Get);
    rotation.y = savedFrame;
    EV_scenApplyPosOrRotToPartOf(MDTP_RECORDER, 0xa7, nullptr, &rotation, EventCharacterPartTransformMode::Absolute);
    EV_scenApplyPosOrRotToPartOf(MDTP_RECORDER, 0xa6, nullptr, &rotation, EventCharacterPartTransformMode::Get);
    rotation.y = savedBingo;
    EV_scenApplyPosOrRotToPartOf(MDTP_RECORDER, 0xa6, nullptr, &rotation, EventCharacterPartTransformMode::Absolute);
    SetPlayerGameFlags(PlayerGameFlagMode::Bit2, reinterpret_cast<void *>(static_cast<std::uintptr_t>((work->flags & 0x40000000U) != 0)));
    camCtrlSetFovTransitionTo(1);
    camCtrlSetState(Init);
    if (!shenIsGamePaused())
        FinishLoading();
    SetPlayerControlEnabled(1);
    HLib::MarkCurrentTaskForCleanup();
}
void MdtpBeginBingoCallback(void *data) {
    MdtpWork *const work = static_cast<MdtpWork *>(data);
    UpdateMdtpHand(work);
    if (filmCheckFadeTrackColour())
        return;
    if (!work || (work->flags & 1)) {
        EV_EventSetUpHelpIcon_0(0, 0);
        if (EV_GetShenmueFlag(0x10d))
            EV_stepSetFlags(0, 0x10d);
        HLib::SetCurrentTaskTo(MdtpBingoCallback);
        HLib::SetupTaskDelay(-1);
        const shen_name characters[]{MDTP_REN, {}};
        QueueNpcs(characters);
    } else {
        SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        work->flags |= 1;
    }
}
void MdtpBingoCallback(void *data) {
    HLib::SetCurrentTaskTo(MdtpOutdoorCallback);
    EV_FreeAuthorPak(1, MDTP_AUTHOR_PACKAGE);
    if (!data)
        return;
    HLib::SetupTaskDelay(-1);
    HideRecorderRoom();
    AT_SetLocalBaseChara(MDTP_PLAYER, {});
    SetDoorSubjectEnabled(MDTP_DOOR, 0);
    QueueMdtpAuthor(MDTP_QUIT_AUTHOR, MdtpQuitAuthorCompletion);
}
void MdtpOutdoorCallback(void *data) {
    MdtpWork *const work = static_cast<MdtpWork *>(data);
    if (work)
        SetPlayerGameFlags(PlayerGameFlagMode::Bit2, reinterpret_cast<void *>(static_cast<std::uintptr_t>((work->flags & 0x40000000U) != 0)));
    if (!shenIsGamePaused())
        FinishLoading();
    EV_EventSetUpHelpIcon_0(62, 0);
    spmFadeOut();
    EV_CleanupMapControlTask();
    HLib::MarkCurrentTaskForCleanup();
    AT_SetLocalBaseChara(MDTP_PLAYER, shen_name{0xffffffffU});
    EV_scenQueueLoad(3, MDTP_OUTDOOR, 97);
}
const ScriptAreaDefinition mdtpScriptDefinition{.areaId = MDTP_AREA, .field04 = 0, .init = MDTPInit, .main = MDTPMain};
} // namespace shenmue::s2
