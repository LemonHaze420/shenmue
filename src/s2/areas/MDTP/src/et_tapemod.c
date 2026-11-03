#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_mdtp.hpp"

void MDTPMain(int *argument)
{
    if (argument) {
        switch (argument[0]) {
        case 0:
            PlaceMdtpRecorder(0);
            FTBL.SetPlayerControlEnabled(1);
            break;
        case 1:
            PlaceMdtpRecorder(1);
            FTBL.SetPlayerControlEnabled(1);
            break;
        default:
            QueueMdtp(argument[0], argument[1], mdtpPackage);
        }
    }
}

#define SHENMUE_AREA_LOOP_VOICE_REVISION 1
#include "s2/areas/area_loop_dc.c"

void MdtpQuitCallback(void *data)
{
    MdtpWork *work = (MdtpWork *)data;
    Vector3 position;
    Vector3i rotation;
    int wheel, reel, clip;
    while (!FTBL.filmCheckFadeTrackColour()) {
        reel = 0;
        wheel = -1;
        rotation.x = reel;
        rotation.y = reel;
        rotation.z = reel;
        if (work) {
            if (!(work->flags & 1)) {
                FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
                work->flags |= 1;
                return;
            }
            position.x = 105.87f;
            position.y = 89.27f;
            position.z = 153.67f;
            rotation.y = 2621;
            QueueAreaLoopPosition(MDTP_PLAYER, -1, 0, &position, &rotation, 0);
            FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(MDTP_PLAYER), 17, 0);
            QueueAreaLoopMotion(MDTP_PLAYER, 0xf003, 0, -1, -1.0f, -1.0f, 1.0f, 0.0f, 0, 2);
            FTBL.SetScreenFilmFade(64, 255, 0, 0, 0, 0, 0, 0, 0);
            if (work->voice && FTBL.cvsHasValidConverseData(work->voice)) {
                FTBL.scenCvsCtrlFree(work->voice);
                work->voice = 0;
                work->voiceClip = -1;
            }
            if (work->pausedVoice && FTBL.cvsHasValidConverseData(work->pausedVoice)) {
                FTBL.scenCvsCtrlFree(work->pausedVoice);
                work->pausedVoice = 0;
                work->pausedClip = -1;
            }
            clip = FindMdtpTapeClip(work->tape, work->tapeFrame);
            if (clip >= 0)
                work->tapeFrame = MDTP_CLIP(work->tape, clip)->start;
            else
                work->tapeFrame = 0;
            if (work->tape && work->bank)
                FTBL.EV_FreeAuthorPak(0, work->bank);
            if ((int)work->flags < 0) {
                FTBL.LoadCutsceneAndSoundProgramForArea();
                work->flags &= 0x7fffffff;
            }
            if (work->flags & 2) {
                FTBL.hndlSetFlags(FTBL.ctrlGetCharaByName(MDTP_PLAYER));
                work->flags &= ~2;
            }
            reel = (unsigned short)work->tapeFrame;
            wheel = (unsigned short)work->bingoClip;
            FTBL.HLib_DestroyTask(work->pauseTask);
        }
        FTBL.AT_SetLocalBaseChara(MDTP_PLAYER, MDTP_FREE_BASE);
        FTBL.EV_EventSetUpHelpIcon_0(0, 0);
        if (FTBL.EV_GetShenmueFlag(269))
            FTBL.EV_stepSetFlags(269, 1);
        EventDebugUnregister(MDTP_DEBUG_ID);
        SetAreaLoopCharacterControl(MDTP_RECORDER, 0);
        SetAreaLoopCharacterControl(MDTP_TAPE, 0);
        SetAreaLoopCharacterControl(MDTP_PLAYER, 0);
        FTBL.scenApplyPosOrRotToChildId(MDTP_RECORDER, 167, 0, &rotation, 2);
        rotation.y = reel;
        FTBL.scenApplyPosOrRotToChildId(MDTP_RECORDER, 167, 0, &rotation, 0);
        FTBL.scenApplyPosOrRotToChildId(MDTP_RECORDER, 166, 0, &rotation, 2);
        rotation.y = wheel;
        FTBL.scenApplyPosOrRotToChildId(MDTP_RECORDER, 166, 0, &rotation, 0);
        if (work->flags & 0x40000000)
            FTBL.SetPlayerGameFlags(2, 1);
        else
            FTBL.SetPlayerGameFlags(2, 0);
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        if (!FTBL.shenIsGamePaused())
            FTBL.FinishLoading();
        FTBL.SetPlayerControlEnabled(1);
        FTBL.HLib_MarkCurrentTaskForCleanup();
        return;
    }
}

void MdtpDebugQuitCallback(void *data)
{
    MdtpWork *work = (MdtpWork *)data;
    if (work) {
        FTBL.HLib_AssignCallbackForTask(work->task, MdtpQuitCallback);
        FTBL.HLib_SetupTimedTask(work->task, 1);
    }
}

void MdtpPauseCallback(void *data)
{
    MdtpPauseWork *pause = (MdtpPauseWork *)data;
    int paused;
    if (pause && pause->owner) {
        paused = FTBL.shenIsGamePaused();
        pause->owner->flags &= ~0x10000;
        switch (pause->phase) {
        default:
        case 0:
            pause->phase = paused ? 1 : 2;
            break;
        case 1:
            pause->phase = paused ? 3 : 2;
            break;
        case 2:
            pause->phase = paused ? 1 : 4;
            pause->owner->flags |= 0x10000;
            break;
        case 3:
            pause->phase = paused ? 3 : 2;
            break;
        case 4:
            pause->phase = paused ? 1 : 4;
            break;
        }
    }
}

int QueueMdtp(int command, unsigned int tape, char *package)
{
    int result = 0;
    HLibTask *task = 0, *pause = 0;
    MdtpPauseWork *pauseWork;
    MdtpWork *work;
    unsigned int flags;
    char *bytes;
    int i;
    for (;;) {
        if (tape > 999) {
            FTBL.DebugLogToFileRemoved("\n\n\n\nMDTP.EV3 Illigal No.(%d) Given!!\n\n\n\n\n", tape);
            break;
        }
        pause = FTBL.HLib_EnqueueTaskWithParameter(MdtpPauseCallback, 15, 11, sizeof(MdtpPauseWork), TASK_EVNT);
        if (!pause)
            break;
        pauseWork = (MdtpPauseWork *)FTBL.HLib_GetTaskParameter(pause);
        if (!pauseWork)
            break;
        task = FTBL.HLib_EnqueueTaskWithParameter(MdtpInitializeCallback, 4, 11, sizeof(MdtpWork), TASK_EVNT);
        if (!task)
            break;
        work = (MdtpWork *)FTBL.HLib_GetTaskParameter(task);
        if (!work)
            break;
        bytes = (char *)work;
        for (i = 0; i < (int)sizeof(MdtpWork); i++)
            bytes[i] = 0;
        work->parent = FTBL.HLib_GetCurrentTask();
        work->task = task;
        work->pauseTask = pause;
        work->tapeIndex = tape;
        work->package = package;
        work->media = mdtpMedia;
        work->flags = (command == 2 ? 0x201 : 1) | (command == 4 ? 0x400 : 0);
        pauseWork->owner = work;
        pauseWork->phase = 0;
        FTBL.EV_EventSetUpHelpIcon_0(62, 0);
        EventDebugRegister(MDTP_DEBUG_ID, MdtpDebugQuitCallback, work, 0x7fffffff);
        result = 1;
        break;
    }
    if (!result) {
        if (task)
            FTBL.HLib_DestroyTask(task);
        if (pause)
            FTBL.HLib_DestroyTask(pause);
    }
    return result;
}

void MdtpInitializeCallback(void *data)
{
    MdtpWork *work = (MdtpWork *)data;
    for (;;) {
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(MdtpQuitCallback);
            break;
        }
        if (work->flags & 0x200)
            FTBL.HLib_SetCurrentTaskTo(MdtpInitializeAuthorCallback);
        else
            FTBL.HLib_SetCurrentTaskTo(MdtpInitializeFlagsCallback);
        FTBL.EV_SetNewSceneIndex();
        work->flags |= 0x80000000;
        FTBL.spmFadeOut(32);
        if (FTBL.SetPlayerGameFlags(2, -1))
            work->flags |= 0x40000000;
        FTBL.SetPlayerGameFlags(2, 0);
        QueueMdtpPackage(work->package, 1);
        break;
    }
}

void MdtpInitializeAuthorCallback(void *data)
{
    MdtpWork *work = (MdtpWork *)data;
    shen_name characters[2];
    if (!work) {
        FTBL.HLib_SetCurrentTaskTo(MdtpQuitCallback);
        return;
    }
    FTBL.EV_LoadCutsceneFromCurrentScene(work->media);
    FTBL.EV_LoadAudio(work->media, 0);
    FTBL.HLib_SetCurrentTaskTo(MdtpBeginAuthorCallback);
    FTBL.HLib_SetupTaskDelay(-1);
    characters[0] = MDTP_REN;
    characters[1] = 0;
    QueueAreaLoopNpcs(characters, ResolveAreaLoopParent);
}

void MdtpBeginAuthorCallback(void *data)
{
    MdtpWork *work = (MdtpWork *)data;
    if (!work) {
        FTBL.HLib_SetCurrentTaskTo(MdtpQuitCallback);
        return;
    }
    FTBL.HLib_SetCurrentTaskTo(MdtpInitializeFlagsCallback);
    FTBL.HLib_SetupTaskDelay(-1);
    FTBL.EV_authSceneCtrl(-11);
    FTBL.EV_DisableCharacterDisplay(MDTP_DESK);
    FTBL.EV_DisableCharacterDisplay(MDTP_LAMP);
    FTBL.EV_DisableCharacterDisplay(MDTP_CHAIR);
    FTBL.EV_DisableCharacterDisplay(MDTP_RECORDER);
    FTBL.EV_DisableCharacterDisplay(MDTP_TAPE);
    FTBL.EV_FreeAuthorPak(1, mdtpConversation);
    work->bank = mdtpConversation;
    FTBL.SetDoorSubjectEnabled(MDTP_DOOR, 0);
    QueueAreaLoopAuthor(mdtpAuthorPackage, MDTP_INIT_AUTHOR, 0, -1, MdtpInitAuthorCompletion);
}

int MdtpInitAuthorCompletion(int phase, int value, HLibTask *parent)
{
    EventDateTime now;
    shen_name characters[2];
    switch (phase) {
    case 0:
        FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        break;
    case 2:
        if (value == 2600) {
            FTBL.EV_authSceneCtrl(-2);
            FTBL.GetCurrentDateTime((DateTime *)&now);
            if (now.hour > 7)
                now.day++;
            now.hour = 8;
            now.minute = 0;
            FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&now);
            FTBL.EV_stepSetFlags(100, 0);
            FTBL.EV_stepSetFlags(303, 0);
            FTBL.EV_setHoldStatesFlags(2);
            FTBL.Queue_ST_LoadTitleFadeIn(MDTP_NEXT_DAY);
        }
        if (value == 2700) {
            FTBL.EV_authSceneCtrl(-3);
            FTBL.ST_LoadTitleFade();
        }
        if (value == FTBL.EV_authSceneCtrl(-5) - 32)
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        break;
    case 1:
        if (FTBL.EV_authSceneCtrl(-13))
            FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        characters[0] = MDTP_REN;
        characters[1] = 0;
        FTBL.SetDoorSubjectEnabled(MDTP_DOOR, 1);
        RestoreAreaLoopCharacters(characters, 0);
        FTBL.EV_EnableCharacterDisplay(MDTP_DESK);
        FTBL.EV_EnableCharacterDisplay(MDTP_LAMP);
        FTBL.EV_EnableCharacterDisplay(MDTP_CHAIR);
        FTBL.EV_EnableCharacterDisplay(MDTP_RECORDER);
        FTBL.EV_EnableCharacterDisplay(MDTP_TAPE);
        FTBL.EV_authSceneCtrl(-10);
        break;
    }
    return ResolveAreaLoopParent(phase, value, parent);
}

void MdtpInitializeFlagsCallback(void *data)
{
    MdtpWork *work = (MdtpWork *)data;
    Vector3i rotation;
    int i;
    for (;;) {
        if (!work)
            FTBL.HLib_SetCurrentTaskTo(MdtpQuitCallback);
        if (!(work->tape = LoadMdtpTape(work->package, work->tapeIndex)))
            break;
        SetAreaLoopCharacterControl(MDTP_PLAYER, 1);
        SetAreaLoopCharacterControl(MDTP_TAPE, 1);
        SetAreaLoopCharacterControl(MDTP_RECORDER, 1);
        FTBL.AT_SetLocalBaseChara(MDTP_PLAYER, MDTP_BASE);
        MaintainMdtpMotion(0);
        FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(MDTP_PLAYER), MDTP_MOTION, 0);
        FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(MDTP_PLAYER), 0);
        FTBL.SetMotionLayerLoopStartTime(FTBL.scenFindMotionwork(MDTP_PLAYER), 0.0f, 0);
        FTBL.SetMotionRegionEvalTime(FTBL.scenFindMotionwork(MDTP_PLAYER), 0.0f, 0);
        FTBL.SetMotionLayerEndFrame(FTBL.scenFindMotionwork(MDTP_PLAYER), 114.0f, 0);
        QueueAreaLoopCamera(work->package, 3000, MDTP_BASE, 0, 0);
        EventDebugGetState(MDTP_DEBUG_ID, 1);
        EventDebugCheck(MDTP_DEBUG_ID, 4);
        FTBL.EV_LoadCutsceneFromCurrentScene(work->media);
        FTBL.EV_LoadAudio(work->media, 1);
        FTBL.GetMotionBonePosition(FTBL.scenFindMotionwork(MDTP_PLAYER), 14, &work->handPosition);
        work->button = MDTP_STOP;
        for (i = 0; i <= 4; i++)
            work->buttonFrames[i] = 0;
        if (work->flags & 0x200) {
            work->tapeFrame = 0;
            work->bingoClip = -1;
        } else {
            FTBL.scenApplyPosOrRotToChildId(MDTP_RECORDER, 167, 0, &rotation, 2);
            work->tapeFrame = rotation.y;
            FTBL.scenApplyPosOrRotToChildId(MDTP_RECORDER, 166, 0, &rotation, 2);
            work->bingoClip = rotation.y;
            if (rotation.y > 0x8000)
                work->bingoClip = -1;
        }
        work->voice = 0;
        work->pausedVoice = 0;
        work->voiceClip = -1;
        work->pausedClip = -1;
        work->seekClip = -1;
        FTBL.HLib_SetCurrentTaskTo(MdtpRecorderCallback);
        break;
    }
}

void MdtpRecorderCallback(void *data)
{
    MdtpWork *work = (MdtpWork *)data;
    Vector3 position;
    Vector3i rotation;
    float counters[3];
    EventDateTime now;
    int stopping;
    int ready, input, previous, changed, i, reel;
    float angle;
    MdtpTapeClip *last;
    for (;;) {
        FTBL.GetCurrentDateTime((DateTime *)&now);
        if (!FTBL.shenGetMinutesUntilNextFrameMinute() &&
            (work->button != MDTP_PLAY || !(work->tape->entries[FindMdtpTapeClip(work->tape, work->tapeFrame)].flags & 1))) {
            FTBL.HLib_SetCurrentTaskTo(MdtpQuitCallback);
            break;
        }
        MaintainMdtpMotion(1);
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(MdtpQuitCallback);
            return;
        }
        ready = 1;
        if (FTBL.filmCheckFadeTrackColour())
            ready = 0;
        else if (work->flags & 1) {
            FTBL.SetScreenFilmFade(32, 255, 0, 0, ready = 0, 0, 0, 0, 0);
            work->flags &= ~1;
            QueueAreaLoopCamera(work->package, 3000, MDTP_BASE, 0, 0);
        }
        if (work->pressFrames || !ready)
            input = 0;
        else {
            input = FTBL.GetCurrentControlInput(2) & 0xf9f9;
            if (work->flags & 0x2000) {
                input = 0;
                if (!work->handBlendFrames) {
                    if (work->button != MDTP_STOP)
                        input = 0x400;
                    else {
                        for (i = 0; i <= 4; i++)
                            work->buttonFrames[i] = 0;
                        FTBL.HLib_SetCurrentTaskTo(MdtpBeginExitQuestionCallback);
                    }
                }
            } else if (work->button == MDTP_REWIND || work->button == MDTP_FAST_FORWARD)
                input &= 0xc600;
            else {
                if (work->button != MDTP_PLAY)
                    FTBL.CheckHelpIcon(33, 0, 3);
                FTBL.GetBattleDataConfig(0, MDTP_ACCEPT);
                if (FTBL.GetBattleDataConfig(1, MDTP_ACCEPT) && FTBL.GetBattleDataConfig(2, MDTP_ACCEPT))
                    input |= 0x400;
                FTBL.GetBattleDataConfig(0, MDTP_CANCEL);
                if (FTBL.GetBattleDataConfig(1, MDTP_CANCEL) && FTBL.GetBattleDataConfig(2, MDTP_CANCEL))
                    input |= 0x200;
            }
        }
        if (!(work->flags & 2)) {
            FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(MDTP_PLAYER));
            FTBL.SetHighModelXYZ(FTBL.ctrlGetCharaByName(MDTP_PLAYER), 1, 0, 0);
            work->flags |= 2;
            FTBL.SetScenarioCharacterDrawControl(MDTP_PLAYER, 1, 4, 0.0f);
            work->pressFrames = 1;
        }
        if ((input >> 9) & 1) {
            work->handBlendFrames = (work->selectedButton + 2) * 4;
            work->selectedButton = 2;
            work->flags |= 0x2000;
        } else if (input & 0x4000) {
            if (!work->selectedButton)
                work->selectedButton = -1;
            if (--work->selectedButton < -2)
                work->selectedButton = -2;
            work->handBlendFrames = 5;
        } else if (input & 0x8000) {
            if (work->selectedButton == -2)
                work->selectedButton = -1;
            if (++work->selectedButton > 2)
                work->selectedButton = 2;
            work->handBlendFrames = 5;
        } else if (input & 0x1400)
            work->pressFrames = 15;
        previous = work->button;
        stopping = work->flags & 0x1000;
        changed = UpdateMdtpButtons(work->selectedButton, &work->pressFrames, &work->button, work->buttonFrames, &stopping);
        if (stopping)
            work->flags |= 0x1000;
        else
            work->flags &= ~0x1000;
        if (changed) {
            FTBL.Loop3DSoundEffect(1456);
            switch (work->button) {
            case MDTP_REWIND:
                work->bingoClip = -1;
                FTBL.Loop3DSoundEffect(1455);
                BeginMdtpRewind(work, previous);
                break;
            case 153:
                break;
            case MDTP_PLAY:
                FTBL.SetLoadingBusy();
                BeginMdtpPlay(work);
                break;
            case MDTP_FAST_FORWARD:
                work->bingoClip = -1;
                FTBL.Loop3DSoundEffect(1455);
                BeginMdtpFastForward(work, previous);
                break;
            case MDTP_STOP:
                BeginMdtpStop(work, previous);
                break;
            }
        } else {
            switch (work->button) {
            case MDTP_REWIND:
                RewindMdtpTape(work);
                if (work->flags & 0x10000)
                    FTBL.Loop3DSoundEffect(1455);
                break;
            case 153:
                break;
            case MDTP_PLAY:
                if (PlayMdtpTape(work) && !(work->flags & 0x400))
                    FTBL.HLib_SetCurrentTaskTo(MdtpBeginBingoCallback);
                break;
            case MDTP_FAST_FORWARD:
                FastForwardMdtpTape(work);
                if (work->flags & 0x10000)
                    FTBL.Loop3DSoundEffect(1455);
                break;
            case MDTP_STOP:
            default:
                StopMdtpTape(work);
            }
        }
        i = work->tapeFrame;
        counters[0] = i * 1.2f;
        if (i % 300 > 270) {
            counters[1] = i / 300 * 36.0f;
            MDTP_CLEAN(counters[1]);
            counters[1] += (i - 270) / 30.0f * 36.0f;
        } else
            counters[1] = i / 300 * 36.0f;
        MDTP_CLEAN(counters[1]);
        if (i % 3000 > 2970) {
            counters[2] = i / 3000 * 36.0f;
            MDTP_CLEAN(counters[2]);
            counters[2] += (i - 2970) / 30.0f * 36.0f;
        } else
            counters[2] = i / 3000 * 36.0f;
        MDTP_CLEAN(counters[2]);
        for (i = 0; i <= 2; i++) {
            FTBL.scenApplyPosOrRotToChildId(MDTP_RECORDER, mdtpCounterParts[i], &position, &rotation, 2);
            rotation.x = (unsigned short)(-counters[i] * 8192.0f / 45.0f);
            FTBL.scenApplyPosOrRotToChildId(MDTP_RECORDER, mdtpCounterParts[i], &position, &rotation, 0);
        }
        angle = work->tapeFrame * 360.0f * 0.016666668f;
        MDTP_CLEAN(angle);
        FTBL.scenApplyPosOrRotToChildId(MDTP_RECORDER, 167, &position, &rotation, 2);
        reel = (unsigned short)(angle * 8192.0f / 45.0f);
        rotation.y = reel;
        FTBL.scenApplyPosOrRotToChildId(MDTP_RECORDER, 167, &position, &rotation, 0);
        FTBL.scenApplyPosOrRotToChildId(MDTP_RECORDER, 166, &position, &rotation, 2);
        rotation.y = reel;
        FTBL.scenApplyPosOrRotToChildId(MDTP_RECORDER, 166, &position, &rotation, 0);
        FTBL.scenApplyPosOrRotToChildId(MDTP_RECORDER, 164, &position, &rotation, 2);
        angle = rotation.y / 182.04445f;
        if (work->button == MDTP_PLAY) {
            angle *= 0.0112499995f;
            MDTP_CLEAN(angle);
            angle += FTBL.GetRandomNumberInRange(0.1f);
            MDTP_CLEAN(angle);
            angle *= 80.0f;
        } else
            angle *= 0.85f;
        MDTP_CLEAN(angle);
        rotation.y = (unsigned short)(angle * 8192.0f / 45.0f);
        FTBL.scenApplyPosOrRotToChildId(MDTP_RECORDER, 164, &position, &rotation, 0);
        last = MDTP_CLIP(work->tape, work->tape->count - 1);
        angle = (last->start + last->duration - 1) * 2;
        angle = work->tapeFrame / angle;
        MDTP_CLEAN(angle);
        position.x = angle + 0.5f;
        position.y = 1.0f;
        position.z = angle + 0.5f;
        FTBL.scenApplyPosOrRotToChildId(MDTP_TAPE, 18, &position, 0, 3);
        angle = 0.5f - angle;
        MDTP_CLEAN(angle);
        position.x = angle + 0.5f;
        position.z = angle + 0.5f;
        FTBL.scenApplyPosOrRotToChildId(MDTP_TAPE, 23, &position, 0, 3);
        UpdateMdtpHand(work);
        break;
    }
}

void BeginMdtpRewind(MdtpWork *work, int previous)
{
    int clip, margin;
    MdtpTapeClip *record;
    if (previous == MDTP_PLAY || previous == MDTP_STOP) {
        if (work->voice && FTBL.cvsHasValidConverseData(work->voice)) {
            FTBL.scenCvsCtrlFree(work->voice);
            work->voice = 0;
            work->voiceClip = -1;
        }
        if (work->pausedVoice && FTBL.cvsHasValidConverseData(work->pausedVoice)) {
            FTBL.scenCvsCtrlFree(work->pausedVoice);
            work->pausedVoice = 0;
            work->pausedClip = -1;
        }
    }
    while ((clip = FindMdtpTapeClip(work->tape, work->tapeFrame)) < 0)
        work->tapeFrame--;
    if (clip >= 0) {
        while (clip > 0 && !(work->tape->entries[clip].flags & 2))
            clip--;
        if (work->tape->entries[clip].flags & 2) {
            if (clip > 0) {
                record = MDTP_CLIP(work->tape, clip);
                margin = record->duration / 10;
                if (margin > 15)
                    margin = 15;
                margin += record->start;
                if (margin > work->tapeFrame)
                    clip--;
            }
        }
        while (clip > 0 && !(work->tape->entries[clip].flags & 2))
            clip--;
        work->seekClip = clip;
    } else
        work->seekClip = -1;
}

void BeginMdtpPlay(MdtpWork *work)
{
    int clip;
    if (work->bingoClip >= 0)
        return;
    clip = FindMdtpTapeClip(work->tape, work->tapeFrame);
    if (clip >= 0) {
        if (MDTP_CLIP(work->tape, clip)->start < work->tapeFrame)
            clip++;
        if (clip < work->tape->count) {
            do {
                if (work->tape->entries[clip].flags & 2)
                    goto found;
                clip++;
            } while (clip < work->tape->count);
        }
        clip = -1;
    }
found:
    work->bingoClip = clip;
}

void BeginMdtpFastForward(MdtpWork *work, int previous)
{
    int clip, margin;
    MdtpTapeClip *record;
    if (previous == MDTP_PLAY || previous == MDTP_STOP) {
        if (work->voice && FTBL.cvsHasValidConverseData(work->voice)) {
            FTBL.scenCvsCtrlFree(work->voice);
            work->voice = 0;
            work->voiceClip = -1;
        }
        if (work->pausedVoice && FTBL.cvsHasValidConverseData(work->pausedVoice)) {
            FTBL.scenCvsCtrlFree(work->pausedVoice);
            work->pausedVoice = 0;
            work->pausedClip = -1;
        }
    }
    clip = FindMdtpTapeClip(work->tape, work->tapeFrame);
    if (clip >= 0) {
        for (;;) {
            if (work->tape->entries[clip].flags & 4) {
                record = MDTP_CLIP(work->tape, clip);
                margin = record->duration / 10;
                if (margin <= 14)
                    margin *= 9;
                else
                    margin = record->duration - 15;
                margin += record->start;
                if (margin < work->tapeFrame && ++clip > work->tape->count) {
                    clip = work->tape->count;
                    break;
                }
            }
            clip++;
            while (clip < work->tape->count && !(work->tape->entries[clip].flags & 2))
                clip++;
            break;
        }
        work->seekClip = clip;
    } else
        work->seekClip = -1;
}

void BeginMdtpStop(MdtpWork *work, int previous)
{
    for (;;) {
        if (previous != MDTP_PLAY || !work->voice)
            break;
        if (!FTBL.cvsHasValidConverseData(work->voice)) {
            work->voice = 0;
            work->voiceClip = -1;
            work->pausedVoice = 0;
            work->pausedClip = -1;
            break;
        }
        FTBL.SetConversationFade(work->voice, 1);
        work->pausedVoice = work->voice;
        work->pausedClip = work->voiceClip;
        work->voice = 0;
        work->voiceClip = -1;
        break;
    }
}

void RewindMdtpTape(MdtpWork *work)
{
    int clip;
    MdtpTapeClip *record;
    do {
        if (work->seekClip < 0) {
            work->flags |= 0x1000;
            break;
        }
        do {
            if (work->tapeFrame > 10)
                work->tapeFrame = work->tapeFrame - 10;
            else
                work->tapeFrame = 0;
            if (work->tapeFrame < 0)
                work->tapeFrame = 0;
            clip = FindMdtpTapeClip(work->tape, work->tapeFrame);
            if (clip < work->seekClip) {
                record = MDTP_CLIP(work->tape, work->seekClip);
                work->tapeFrame = record->start;
            } else {
                if (clip != work->seekClip)
                    return;
                record = MDTP_CLIP(work->tape, clip);
                if (record->start < work->tapeFrame)
                    return;
                work->tapeFrame = record->start;
            }
            work->seekClip = -1;
            work->flags |= 0x1000;
        } while (0);
    } while (0);
}

static __inline__ unsigned int ReadMdtpClipNameField(const MdtpTapeClip *record, int voice)
{
    do {
        if (!record)
            break;
        if (voice)
            return record->voiceOffset;
        return record->bankOffset;
    } while (0);
    return 0;
}

static __inline__ unsigned int ReadMdtpAudioOffset(const MdtpTapeClip *record, int voice)
{
    unsigned int offset;
    do {
        offset = ReadMdtpClipNameField(record, voice);
    } while (0);
    return offset;
}

int PlayMdtpTape(MdtpWork *work)
{
    int clip, bingo = 0, match;
    MdtpTapeClip *record;
    char *bank, *voice;
    unsigned int bankOffset, voiceOffset;
    EventFunctionTable *functions = &FTBL;
    clip = FindMdtpTapeClip(work->tape, work->tapeFrame);
    work->tapeFrame++;
    if (clip >= 0) {
        if (work->bingoClip >= 0 && work->bingoClip <= clip && (work->tape->entries[clip].flags & 4) &&
            (work->tape->entries[clip].flags & 1))
            bingo = clip != FindMdtpTapeClip(work->tape, work->tapeFrame);
        do {
            record = 0;
            if (work->tape && work->tape->entries[clip].offset)
                record = (MdtpTapeClip *)(work->tape->entries[clip].offset + (unsigned int)work->tape);
        } while (0);
        SHENMUE_EVENT_BOUNDARY();
        do {
            if (!work->voice) {
                if (work->pausedClip == clip && work->pausedVoice && functions->GetConversationPaused(work->pausedVoice)) {
                    functions->SetConversationFade(work->pausedVoice, 0);
                    work->voice = work->pausedVoice;
                    work->voiceClip = work->pausedClip;
                    work->pausedVoice = 0;
                    work->pausedClip = -1;
                    break;
                }
                if (work->voiceClip == clip)
                    break;
                if (work->pausedVoice && FTBL.GetConversationPaused(work->pausedVoice)) {
                    FTBL.scenCvsCtrlFree(work->pausedVoice);
                    work->pausedVoice = 0;
                    work->pausedClip = -1;
                }
            } else {
                if (!FTBL.cvsHasValidConverseData(work->voice)) {
                    work->voice = 0;
                    work->pausedVoice = 0;
                    SHENMUE_EVENT_BOUNDARY();
                    work->pausedClip = -1;
                    break;
                }
                if (work->voiceClip == clip)
                    break;
                FTBL.scenCvsCtrlFree(work->voice);
                work->voice = 0;
            }
            if (record) {
                bank = (bankOffset = ReadMdtpAudioOffset(record, 0)) ? (char *)(bankOffset + (unsigned int)work->tape) : 0;
                voice = record && (voiceOffset = ReadMdtpAudioOffset(record, 1)) ? (char *)(voiceOffset + (unsigned int)work->tape) : 0;
                if (bank && *bank && voice && *voice) {
                    match = -2;
                    if (work->bank) {
                        match = 0;
                        while (bank[match]) {
                            if (bank[match] != work->bank[match]) {
                                match = -1;
                                break;
                            }
                            match++;
                        }
                    }
                    if (match < 0) {
                        if (match == -1)
                            FTBL.EV_FreeAuthorPak(0, work->bank);
                        FTBL.EV_FreeAuthorPak(1, bank);
                        work->bank = bank;
                    }
                    work->voice = FTBL.shenPlayAudioWithFlags(voice, 1);
                    work->voiceClip = clip;
                }
            }
        } while (0);
        return bingo;
    }
    while (work->tapeFrame > 0 && FindMdtpTapeClip(work->tape, work->tapeFrame - 1) < 0)
        work->tapeFrame--;
    work->flags |= 0x1000;
    return bingo;
}

void FastForwardMdtpTape(MdtpWork *work)
{
    int clip;
    MdtpTapeClip *record;
    do {
        if (work->seekClip < 0) {
            work->flags |= 0x1000;
            break;
        }
        do {
            work->tapeFrame += 10;
            clip = FindMdtpTapeClip(work->tape, work->tapeFrame);
            if (clip < 0) {
                while ((clip = FindMdtpTapeClip(work->tape, work->tapeFrame - 1)) < 0)
                    work->tapeFrame--;
                clip++;
                record = 0;
            } else
                record = MDTP_CLIP(work->tape, clip);
            if (work->flags & 0x1000) {
                work->seekClip = -1;
                break;
            }
            if (clip == work->seekClip) {
                work->seekClip = -1;
                if (record)
                    work->tapeFrame = record->start;
                work->flags |= 0x1000;
            }
        } while (0);
    } while (0);
}

void StopMdtpTape(MdtpWork *work)
{
}

void MdtpBeginExitQuestionCallback(void *data)
{
    MdtpWork *work = (MdtpWork *)data;
    int stopping;
    for (;;) {
        MaintainMdtpMotion(1);
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(MdtpQuitCallback);
            break;
        }
        stopping = 0;
        UpdateMdtpButtons(work->selectedButton, &work->pressFrames, &work->button, work->buttonFrames, &stopping);
        UpdateMdtpHand(work);
        if (FTBL.filmCheckFadeTrackColour())
            return;
        if (!(work->flags & 1)) {
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->flags |= 1;
            return;
        }
        FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        work->flags &= ~1;
        QueueAreaLoopCamera(work->package, 3001, MDTP_BASE, 0, 0);
        FTBL.HLib_SetCurrentTaskTo(MdtpExitQuestionCallback);
        break;
    }
}

void MdtpExitQuestionCallback(void *data)
{
    MdtpWork *work = (MdtpWork *)data;
    int stopping;
    for (;;) {
        MaintainMdtpMotion(1);
        if (!work) {
            FTBL.HLib_SetCurrentTaskTo(MdtpQuitCallback);
            break;
        }
        stopping = 0;
        UpdateMdtpButtons(work->selectedButton, &work->pressFrames, &work->button, work->buttonFrames, &stopping);
        UpdateMdtpHand(work);
        if (FTBL.filmCheckFadeTrackColour())
            return;
        FTBL.GetBattleDataConfig(0, MDTP_ACCEPT);
        if (FTBL.GetBattleDataConfig(1, MDTP_ACCEPT) && FTBL.GetBattleDataConfig(2, MDTP_ACCEPT)) {
            if (!(work->flags & 1)) {
                FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
                work->flags |= 1;
            }
            work->flags &= ~0x2000;
            FTBL.HLib_SetCurrentTaskTo(MdtpRecorderCallback);
            return;
        }
        FTBL.GetBattleDataConfig(0, MDTP_CANCEL);
        if (FTBL.GetBattleDataConfig(1, MDTP_CANCEL) && FTBL.GetBattleDataConfig(2, MDTP_CANCEL)) {
            if (!(work->flags & 1)) {
                FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
                work->flags |= 1;
            }
            FTBL.HLib_SetCurrentTaskTo(MdtpFinishExitCallback);
        }
        break;
    }
}

void MdtpFinishExitCallback(void *data)
{
    MdtpWork *work = (MdtpWork *)data;
    int stopping;
    if (work) {
        stopping = 0;
        UpdateMdtpButtons(work->selectedButton, &work->pressFrames, &work->button, work->buttonFrames, &stopping);
        UpdateMdtpHand(work);
        if (!FTBL.filmCheckFadeTrackColour()) {
            FTBL.HLib_SetCurrentTaskTo(MdtpQuitCallback);
            QueueAreaLoopMotion(MDTP_PLAYER, 0xf003, 0, -1, -1.0f, -1.0f, 1.0f, 0.0f, 0, 2);
        }
    } else
        FTBL.HLib_SetCurrentTaskTo(MdtpQuitCallback);
}

void GetMdtpHandTarget(int selected, Vector3 *target, int press)
{
    Vector3 bone, local;
    float matrix[16];
    Vector3 offset, position, row;
    Vector3i rotation;
    if (!target || selected < -2 || selected > 2)
        return;
    FTBL.getSpecialBonePos(FTBL.ctrlGetCharaByName(MDTP_PLAYER), 0, 3, &bone);
    FTBL.CopyMotionBoneTransform(FTBL.scenFindMotionwork(MDTP_PLAYER), 14, matrix);
    FTBL.matrix_stack_Matrix4x4_Push(0);
    FTBL.Matrix4x4_SetActiveTransform(matrix);
    FTBL.Matrix4x4_GetTranslationRow(&row);
    FTBL.Matrix4x4_TransformPoint(&bone, &local);
    FTBL.PopCurrentMatrix(0);
    FTBL.GetCharaPosition(MDTP_RECORDER, -1, &position, 0x40000000);
    FTBL.GetScenarioCharacterEulerAngles(MDTP_RECORDER, &rotation, 0x40000000);
    offset.x = selected * 0.02f;
    MDTP_CLEAN(offset.x);
    offset.x -= 0.066f;
    MDTP_CLEAN(offset.x);
    if (press > 8) {
        offset.y = (press - 8) * 0.000875f;
        MDTP_CLEAN(offset.y);
        offset.y += -0.007f;
        MDTP_CLEAN(offset.y);
    } else {
        offset.y = press * -0.000875f;
        MDTP_CLEAN(offset.y);
    }
    offset.y += 0.068f;
    MDTP_CLEAN(offset.y);
    offset.z = 0.008f;
    FTBL.ApplyScenarioTranslationAndRotation(&position, &rotation, &offset);
    row.x -= local.x;
    MDTP_CLEAN(row.x);
    row.y -= local.y;
    MDTP_CLEAN(row.y);
    row.z -= local.z;
    MDTP_CLEAN(row.z);
    target->x = row.x + offset.x;
    MDTP_CLEAN(target->x);
    target->y = row.y + offset.y;
    MDTP_CLEAN(target->y);
    target->z = row.z + offset.z;
    MDTP_CLEAN(target->z);
}

void BlendMdtpHand(Vector3 *current, Vector3 *target, int frames)
{
    float scale, delta;
    if (!current || !target)
        return;
    if (frames <= 1) {
        *current = *target;
        return;
    }
    scale = frames;
    scale = 1.0f / scale;
    MDTP_CLEAN(scale);
    delta = target->x - current->x;
    MDTP_CLEAN(delta);
    delta *= scale;
    MDTP_CLEAN(delta);
    current->x += delta;
    MDTP_CLEAN(current->x);
    delta = target->y - current->y;
    MDTP_CLEAN(delta);
    delta *= scale;
    MDTP_CLEAN(delta);
    current->y += delta;
    MDTP_CLEAN(current->y);
    delta = target->z - current->z;
    MDTP_CLEAN(delta);
    delta *= scale;
    MDTP_CLEAN(delta);
    current->z += delta;
    MDTP_CLEAN(current->z);
}

int UpdateMdtpButtons(int selected, int *press, int *active, int *frames, int *stopping)
{
    Vector3 position;
    Vector3i rotation;
    int changed = 0, moving, wasStopping;
    int pose, value, i;
    float scale;
    if (stopping)
        wasStopping = *stopping;
    else
        wasStopping = 0;
    moving = 0;
    if (press && active && frames) {
        if (*press) {
            if (--*press < 0)
                *press = 0;
            if (*press > 8) {
                pose = 1;
                value = *press - 8;
            } else {
                pose = 0;
                value = *press;
            }
            FTBL.ApplyCharacterHandPoseCommand(FTBL.ctrlGetCharaByName(MDTP_PLAYER), 0, mdtpHandPoses[pose], value);
        }
        for (i = 0; mdtpButtonParts[i] != -1; i++) {
            moving |= frames[i] != 0;
            if ((!wasStopping && i == selected + 2) || (wasStopping && mdtpButtonParts[i] == MDTP_STOP)) {
                value = wasStopping ? 0 : *press;
                if ((value <= 7 && frames[i]) || wasStopping) {
                    changed = *active != mdtpButtonParts[i];
                    if (changed) {
                        switch (*active) {
                        case MDTP_PLAY:
                            FTBL.Loop3DSoundEffect(1170);
                            FTBL.FinishLoading();
                            break;
                        case MDTP_REWIND:
                        case MDTP_FAST_FORWARD:
                            FTBL.Loop3DSoundEffect(1170);
                            break;
                        }
                        *active = mdtpButtonParts[i];
                    }
                }
            } else {
                value = frames[i];
                if (value) {
                    if (value > 8)
                        value = 15 - value;
                    value--;
                    if (value <= 7)
                        value--;
                    if (value < 0)
                        value = 0;
                }
            }
            if (!wasStopping && *active == mdtpButtonParts[i] &&
                (selected + 2 == i || frames[selected + 2] > 12.8f || !frames[selected + 2]) && frames[i] &&
                (mdtpButtonParts[i] == MDTP_REWIND || mdtpButtonParts[i] == MDTP_PLAY || mdtpButtonParts[i] == MDTP_FAST_FORWARD))
                value = 7;
            frames[i] = value;
            FTBL.scenApplyPosOrRotToChildId(MDTP_RECORDER, mdtpButtonParts[i], &position, &rotation, 2);
            scale = frames[i] * 0.125f;
            MDTP_CLEAN(scale);
            scale -= 1.0f;
            MDTP_CLEAN(scale);
            scale *= scale;
            MDTP_CLEAN(scale);
            scale = 1.0f - scale;
            MDTP_CLEAN(scale);
            do {
                rotation.x = (unsigned short)(24.0f * scale * 8192.0f / 45.0f);
            } while (0);
            FTBL.scenApplyPosOrRotToChildId(MDTP_RECORDER, mdtpButtonParts[i], &position, &rotation, 0);
        }
        moving = (moving || *active != MDTP_STOP) && wasStopping;
    }
    if (stopping)
        *stopping = moving;
    return changed;
}

void UpdateMdtpHand(MdtpWork *work)
{
    Vector3 target;
    Vector3i rotation;
    if (work) {
        GetMdtpHandTarget(work->selectedButton, &target, work->pressFrames);
        BlendMdtpHand(&work->handPosition, &target, work->handBlendFrames);
        FTBL.GetMTWK_NodeParent_0(MDTP_PLAYER, 0x78000000, 10, &work->handPosition);
        rotation.x = 0xf778;
        rotation.y = 0;
        rotation.z = 0;
        FTBL.imgmSetHandPos(FTBL.ctrlGetCharaByName(MDTP_PLAYER), 0, 1, &rotation);
        if (--work->handBlendFrames < 0)
            work->handBlendFrames = 0;
    }
}

int QueueMdtpPackage(char *package, int recorder)
{
    int result, length, i;
    HLibTask *task;
    MdtpPackageWork *work;
    if (package && package[0]) {
        if (FTBL.EV_ControlPackageResources(0, 5, package, 0, 0)) {
            PlaceMdtpRecorder(recorder);
            result = 1;
        } else {
            result = 0;
            do {
                for (length = 0; package[length]; length++)
                    ;
                task = FTBL.HLib_EnqueueTaskWithParameter(MdtpPackageCallback, 4, 11, sizeof(MdtpPackageWork) + length, TASK_EVNT);
                if (task && (work = (MdtpPackageWork *)FTBL.HLib_GetTaskParameter(task))) {
                    work->parent = FTBL.HLib_GetCurrentTask();
                    work->task = task;
                    work->recorder = recorder;
                    if (FTBL.GetNpcNewLoadState()) {
                        work->npcState = 1;
                        FTBL.EV_NPC_NewLoadSw_0(0);
                    } else
                        work->npcState = 0;
                    if (!FTBL.EV_ResetFreeTalkFlags())
                        work->npcState = -1;
                    for (i = 0; i <= length; i++)
                        work->package[i] = package[i];
                    if (work->parent)
                        FTBL.HLib_SetupTimedTask(work->parent, 0);
                    result = 1;
                }
            } while (0);
            if (!result && task)
                FTBL.HLib_DestroyTask(task);
        }
    } else {
        result = 0;
        FTBL.DebugLogToFileRemoved("can't found TAPE-module's data\n");
        FTBL.EventAssertNullsub(__FILE__, 2207);
    }
    return result;
}

void MdtpPackageCallback(void *data)
{
    MdtpPackageWork *work = (MdtpPackageWork *)data;
    if (work) {
        if (FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)work->package, 0)) {
            FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
            PlaceMdtpRecorder(work->recorder);
            if (work->npcState == 1)
                FTBL.EV_NPC_NewLoadSw_0(1);
            if (work->parent) {
                FTBL.HLib_ResolveDeferredCallback(work->parent);
                FTBL.HLib_SetupTimedTask(work->parent, 1);
            }
            FTBL.HLib_DestroyCurrentEventTask();
        } else if (work->npcState == -1) {
            if (FTBL.GetNpcNewLoadState()) {
                work->npcState = 1;
                FTBL.EV_NPC_NewLoadSw_0(0);
            } else
                work->npcState = 0;
            if (!FTBL.EV_ResetFreeTalkFlags())
                work->npcState = -1;
        }
    } else {
        FTBL.DebugLogToFileRemoved("\n\n\n\npakload failt!!!\n\n\n\n");
        FTBL.EventAssertNullsub(__FILE__, 2255);
    }
}

void PlaceMdtpRecorder(int loaded)
{
    Vector3 position;
    Vector3i rotation;
    shen_name sources[2] = {MDTP_EMPTY_RECORDER, MDTP_LOADED_RECORDER};
    FTBL.GetCharaPosition(sources[loaded != 0], -1, &position, 0x40000000);
    FTBL.GetScenarioCharacterEulerAngles(sources[loaded != 0], &rotation, 0x40000000);
    FTBL.EV_AT_SetPos(MDTP_RECORDER, 0x78000000, &position);
    FTBL.EV_AT_SetAngY_0(MDTP_RECORDER, 0x78000000, &rotation, 0);
    FTBL.EV_EnableCharacterDisplay(MDTP_RECORDER);
    if (loaded)
        FTBL.EV_EnableCharacterDisplay(MDTP_TAPE);
    else
        FTBL.EV_DisableCharacterDisplay(MDTP_TAPE);
}

void MdtpBeginBingoCallback(void *data)
{
    MdtpWork *work = (MdtpWork *)data;
    shen_name characters[2];
    while ((UpdateMdtpHand(work), !FTBL.filmCheckFadeTrackColour())) {
        if (work && !(work->flags & 1)) {
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->flags |= 1;
            return;
        }
        FTBL.EV_EventSetUpHelpIcon_0(0, 0);
        if (FTBL.EV_GetShenmueFlag(269))
            FTBL.EV_stepSetFlags(269, 0);
        FTBL.HLib_SetCurrentTaskTo(MdtpBingoCallback);
        FTBL.HLib_SetupTaskDelay(-1);
        characters[0] = MDTP_REN;
        characters[1] = 0;
        QueueAreaLoopNpcs(characters, ResolveAreaLoopParent);
        return;
    }
}

void MdtpBingoCallback(void *data)
{
    FTBL.HLib_SetCurrentTaskTo(MdtpOutdoorCallback);
    FTBL.EV_FreeAuthorPak(1, mdtpConversation);
    if (data) {
        FTBL.HLib_SetupTaskDelay(-1);
        FTBL.EV_DisableCharacterDisplay(MDTP_DESK);
        FTBL.EV_DisableCharacterDisplay(MDTP_LAMP);
        FTBL.EV_DisableCharacterDisplay(MDTP_CHAIR);
        FTBL.EV_DisableCharacterDisplay(MDTP_RECORDER);
        FTBL.EV_DisableCharacterDisplay(MDTP_TAPE);
        FTBL.AT_SetLocalBaseChara(MDTP_PLAYER, 0);
        FTBL.SetDoorSubjectEnabled(MDTP_DOOR, 0);
        QueueAreaLoopAuthor(mdtpAuthorPackage, MDTP_QUIT_AUTHOR, 0, -1, MdtpQuitAuthorCompletion);
    }
}

void MdtpOutdoorCallback(void *data)
{
    MdtpWork *work = (MdtpWork *)data;
    if (work) {
        if (work->flags & 0x40000000)
            FTBL.SetPlayerGameFlags(2, 1);
        else
            FTBL.SetPlayerGameFlags(2, 0);
    }
    if (!FTBL.shenIsGamePaused())
        FTBL.FinishLoading();
    FTBL.EV_EventSetUpHelpIcon_0(62, 0);
    FTBL.spmFadeOut(32);
    FTBL.HLib_MarkCurrentTaskForCleanup();
    FTBL.AT_SetLocalBaseChara(MDTP_PLAYER, -1);
    FTBL.EV_scenQueueLoad(3, MDTP_OUTDOOR, 97);
}

int MdtpQuitAuthorCompletion(int phase, int value, HLibTask *parent)
{
    shen_name characters[2];
    switch (phase) {
    case 0:
        FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        break;
    case 2:
        if (FTBL.EV_authSceneCtrl(-5) - 32 == value)
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
        break;
    case 1:
        if (FTBL.EV_authSceneCtrl(-13))
            FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        characters[0] = MDTP_REN;
        characters[1] = 0;
        FTBL.SetDoorSubjectEnabled(MDTP_DOOR, 1);
        RestoreAreaLoopCharacters(characters, 0);
        FTBL.EV_EnableCharacterDisplay(MDTP_DESK);
        FTBL.EV_EnableCharacterDisplay(MDTP_LAMP);
        FTBL.EV_EnableCharacterDisplay(MDTP_CHAIR);
        FTBL.EV_EnableCharacterDisplay(MDTP_RECORDER);
        FTBL.EV_EnableCharacterDisplay(MDTP_TAPE);
        break;
    }
    return ResolveAreaLoopParent(phase, value, parent);
}

int FindMdtpTapeClip(MdtpTape *tape, int frame)
{
    int index, result = -1;
    MdtpTapeClip *clip;
    for (index = 0; index < tape->count; index++) {
        clip = MDTP_CLIP(tape, index);
        if (clip && clip->start <= frame && clip->start + clip->duration > frame) {
            result = index;
            break;
        }
        result = -1;
    }
    return result;
}

MdtpTape *LoadMdtpTape(char *package, int index)
{
    char name[16];
    MdtpTape *tape = 0;
    int length, i;
    do {
        if (!FTBL.EV_ControlPackageResources(0, 5, package, 0, 0)) {
            FTBL.DebugLogToFileRemoved("pakfile %s is not load on memory!!\n", package);
            break;
        }
        FTBL.EV_ControlPackageResources(2, 2, package, 0, 0);
        length = 0;
        for (i = 0; mdtpTapePrefix[i]; i++)
            name[length++] = mdtpTapePrefix[i];
        name[length++] = index / 100 + '0';
        name[length++] = index / 10 - index / 100 * 10 + '0';
        name[length++] = index % 10 + '0';
        for (i = 0; mdtpTapeSuffix[i]; i++)
            name[length++] = mdtpTapeSuffix[i];
        name[length] = 0;
        tape = (MdtpTape *)FTBL.LoadCurrentPakBinary(name);
        if (!tape)
            FTBL.DebugLogToFileRemoved("chunk %s is not found in pak(%s)!!\n", name, package);
        else if (tape->signature != MDTP_TAPE_SIGNATURE) {
            FTBL.DebugLogToFileRemoved("chunk %s in pak(%s) is illigal id had!!\n", name, package);
            tape = 0;
        }
    } while (0);
    return tape;
}

void MaintainMdtpMotion(int restart)
{
    Vector3 position;
    Vector3i rotation;
    if (FTBL.GetMotionAnimationId(FTBL.scenFindMotionwork(MDTP_PLAYER), 0) == MDTP_MOTION &&
        FTBL.GetMotionFrame(FTBL.scenFindMotionwork(MDTP_PLAYER), 0) <= 40.0f)
        return;
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(MDTP_PLAYER), MDTP_MOTION, 0);
    if (restart) {
        FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(MDTP_PLAYER), 1);
        FTBL.SetMotionFrameTime(FTBL.scenFindMotionwork(MDTP_PLAYER), 32, 0);
    } else
        FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(MDTP_PLAYER), 17, 0);
    FTBL.SetMotionLayerLoopStartTime(FTBL.scenFindMotionwork(MDTP_PLAYER), 0.0f, 0);
    FTBL.SetMotionRegionEvalTime(FTBL.scenFindMotionwork(MDTP_PLAYER), 0.0f, 0);
    FTBL.SetMotionLayerEndFrame(FTBL.scenFindMotionwork(MDTP_PLAYER), 114.0f, 0);
    position.x = 0.004f;
    position.y = -0.154f;
    position.z = 1.126f;
    rotation.x = 0;
    rotation.y = 0;
    rotation.z = 0;
    FTBL.EV_AT_SetPos(MDTP_PLAYER, 0x38000000, &position);
    FTBL.EV_AT_SetAngY_0(MDTP_PLAYER, 0x38000000, &rotation, 0);
}

char mdtpPackage[] = "MDTP";
char mdtpTapePrefix[] = "umi";
char mdtpTapeSuffix[] = ".bin";
char mdtpMedia[] = "0225A000";
char mdtpAuthorPackage[] = "0225";
char mdtpConversation[] = "0225";
int mdtpHandPoses[2][57] = {
    {0, 726, 55162, 0, 0, 63534, 0, 62988, 3276, 0, 0, 63352, 0, 0, 61168, 0, 0, 63352, 0, 546, 1638, 0, 0, 52796, 0, 0, 48246, 0, 0,
     57528, 0, 546, 1274, 0, 0, 53342, 0, 0, 52432, 0, 0, 58074, 0, 546, 1273, 0, 0, 51522, 0, 0, 52250, 0, 0, 56618, 0, 546, 909},
    {0, 726, 55162, 0, 0, 63534, 0, 62988, 2730, 0, 0, 63352, 0, 0, 64080, 0, 0, 62806, 0, 546, 1638, 0, 0, 52796, 0, 0, 48246, 0, 0,
     57528, 0, 546, 909, 0, 0, 53342, 0, 0, 52432, 0, 0, 58074, 0, 546, 1091, 0, 0, 51522, 0, 0, 52250, 0, 0, 56618, 0, 727, 546},
};
int mdtpButtonParts[] = {MDTP_REWIND, 153, MDTP_PLAY, MDTP_FAST_FORWARD, MDTP_STOP, -1};
int mdtpCounterParts[] = {162, 161, 160, -1};
#endif
