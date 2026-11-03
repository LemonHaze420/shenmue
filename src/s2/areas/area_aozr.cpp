#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_aozr.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void RunAozrEvent(int argument)
{
    int ok = 0;
    HLibTask *task = 0;
    const char *sound;
    int length;
    AozrShopWork *work;
    char *bytes;
    int i;
    if (argument >= 0 && argument < 1000) {
        length = 0;
        sound = FTBL.shenGetCurrentSbt();
        do {
            if (sound)
                while (sound[length])
                    length++;
        } while (0);
        task = FTBL.HLib_EnqueueTaskWithParameter(AozrStartPackageCallback, 4, 11, length + sizeof(AozrShopWork), TASK_EVNT);
        if (task) {
            work = (AozrShopWork *)FTBL.HLib_GetTaskParameter(task);
            if (work) {
                bytes = (char *)work;
                for (i = 0; i < 64; i++)
                    bytes[i] = 0;
                work->self = task;
                work->parent = FTBL.HLib_GetCurrentTask();
                work->package[0] = 'A';
                work->package[1] = 'O';
                work->package[2] = 'Z';
                work->package[3] = 'R';
                work->package[4] = ((argument >> 1) + 1) / 100 % 10 + '0';
                work->package[5] = ((argument >> 1) + 1) / 10 % 10 + '0';
                work->package[6] = ((argument >> 1) + 1) % 10 + '0';
                work->package[7] = 0;
                work->seller = 0;
                work->flags = 0;
                if (argument & 1)
                    work->flags = 0x80000000;
                if (FTBL.GetNpcNewLoadState())
                    work->flags |= 16;
                work->board = 0;
                work->zoom = 1.0f;
                work->bufferedInput = 0;
                work->soundIndex = FTBL.GetSelectedSoundSceneIndex();
                for (i = 0; i < length; i++)
                    work->savedAudio[i] = sound[i];
                work->savedAudio[i] = 0;
                FTBL.SetPlayerControlEnabled(0);
                EventDebugRegister(AOZR_DEBUG_ID, AozrDebugCallback, work, 0x7fffffff);
                FTBL.HLib_SetupTaskDelay(0);
                FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
                work->flags &= ~2;
                ok = 1;
            }
        }
    }
    if (!ok) {
        FTBL.DebugLogToFileRemoved(aozrBootError);
        do {
            if (task)
                FTBL.HLib_DestroyTask(task);
        } while (0);
    }
}
#elif defined(SHENMUE_EVENT_AOZR_SCRIPT)
int AozrStartScript(const char *name, int *script, int *result, unsigned int flags)
{
    int ok = 0;
    HLibTask *task = 0;
    AozrScriptWork *work;
    int i;
    while (script && (task = FTBL.HLib_EnqueueTaskWithParameter(AozrScriptCallback, 4, 11, sizeof(AozrScriptWork), TASK_EVNT))) {
        work = (AozrScriptWork *)FTBL.HLib_GetTaskParameter(task);
        if (!work)
            break;
        i = 0;
        if (name)
            while (name[i])
                work->name[i] = name[i];
        work->name[i] = 0;
        work->script = script;
        work->pc = 0;
        work->flags = flags;
        work->frame = 0;
        work->wait = 0;
        ok = 1;
        work->parent = FTBL.HLib_GetCurrentTask();
        work->result = result;
        break;
    }
    do {
        if (!ok && task)
            FTBL.HLib_DestroyTask(task);
    } while (0);
    return ok;
}

void AozrScriptCallback(void *data)
{
    AozrScriptWork *work = (AozrScriptWork *)data;
    int finish = 1;
    int *script;
    int pc;
    int next;
    int size;
    do {
        if (!work || !(script = work->script))
            break;
        pc = work->pc;
        work->frame++;
        finish = 0;
        if (!(AOZR_SCRIPT_WAITING & work->flags)) {
            work->wait = 0;
            switch (script[pc]) {
            case AOZR_SCRIPT_RET:
                pc += 4;
                finish = 1;
                break;
            case AOZR_SCRIPT_SETRESULT:
                pc += 4;
                if (work->result)
                    *work->result = *(int *)((char *)script + pc);
                pc += 4;
                break;
            case AOZR_SCRIPT_WAIT:
                if (script[pc + 1] <= work->wait)
                    pc += 8;
                break;
            case AOZR_SCRIPT_SERIF:
                next = AozrStartSerif((AozrSerif *)((char *)script + pc + 4));
                if (next) {
                    size = next + 4;
                    pc += size;
                    pc += 6;
                }
                break;
            case AOZR_SCRIPT_MOTION:
                AozrPlayMotion((AozrMotion *)((char *)script + pc + 4));
                pc += 24;
                break;
            case AOZR_SCRIPT_POSANG:
                AozrSetPosAng((AozrPosAng *)((char *)script + pc + 4));
                pc += 24;
                break;
            case AOZR_SCRIPT_CAMERA:
                AozrSetCamera((AozrCamera *)((char *)script + pc + 4), work);
                pc += 12;
                break;
            case AOZR_SCRIPT_SELECT:
                next = AozrStartSelect((AozrSelect *)((char *)script + pc + 4));
                if (next >= 0) {
                    work->pc = pc + 36;
                    pc = next;
                }
                break;
            }
        } else {
            work->wait++;
            switch (script[pc]) {
            case AOZR_SCRIPT_RET:
                FTBL.DebugLogToFileRemoved("Illigal call to RET from orign-script@AOZR\n");
                pc += 4;
                finish = 1;
                break;
            case AOZR_SCRIPT_SETRESULT:
                FTBL.DebugLogToFileRemoved("Illigal call to SETRESULT from orign-script@AOZR\n");
                pc += 8;
                break;
            case AOZR_SCRIPT_WAIT:
                if (script[pc + 1] <= work->wait)
                    pc += 8;
                break;
            case AOZR_SCRIPT_SERIF:
                FTBL.DebugLogToFileRemoved("Illigal call to SERIF from orign-script@AOZR\n");
                finish = 1;
                break;
            case AOZR_SCRIPT_MOTION:
                FTBL.DebugLogToFileRemoved("Illigal call to MOTION from orign-script@AOZR\n");
                pc += 24;
                break;
            case AOZR_SCRIPT_POSANG:
                FTBL.DebugLogToFileRemoved("Illigal call to POSANG from orign-script@AOZR\n");
                pc += 24;
                break;
            case AOZR_SCRIPT_CAMERA:
                pc += 12;
                break;
            case AOZR_SCRIPT_SELECT:
                next = AozrPollSelect((AozrSelect *)((char *)script + pc + 4));
                if (next >= 0) {
                    pc = next;
                    work->pc = pc + 36;
                }
                break;
            }
        }
        if (pc == work->pc)
            work->flags |= AOZR_SCRIPT_WAITING;
        else
            work->flags &= ~AOZR_SCRIPT_WAITING;
        work->pc = pc;
    } while (0);
    if (finish && work && (work->flags & 1) && work->parent)
        FTBL.HLib_ResolveDeferredCallback(work->parent);
}

int AozrStartSerif(AozrSerif *serif)
{
    int size = 0;
    HLibTask *task = 0;
    AozrSerifWork *work;
    while (serif && serif->frames > 0 && (task = FTBL.HLib_EnqueueTaskWithParameter(AozrSerifCallback, 4, 11, sizeof(AozrSerifWork), TASK_EVNT))) {
        work = (AozrSerifWork *)FTBL.HLib_GetTaskParameter(task);
        if (!work)
            break;
        if (serif->flags & 1) {
            work->conversation = FTBL.EV_PlayDialogue(serif->text);
            work->voice = 1;
        } else {
            work->conversation = 0;
            FTBL.dbgPrintEventMessage(serif->text);
            work->voice = 0;
        }
        work->frames = serif->frames;
        size = 0;
        while (serif->text[size])
            size++;
        if (size < 2)
            size = 2;
        size = ((size + 1) & ~3) + 2;
        task = 0;
        break;
    }
    if (task)
        FTBL.HLib_DestroyTask(task);
    return size;
}

void AozrSerifCallback(void *data)
{
    AozrSerifWork *work = (AozrSerifWork *)data;
    int done = 1;
    if (work) {
        work->frames--;
        done = work->frames <= 0;
        if (done) {
            if (work->voice & 1) {
                if (work->conversation)
                    FTBL.scenCvsCtrlFree(work->conversation);
            } else
                FTBL.ctrlStopSubtitles();
        }
    }
    if (done)
        FTBL.HLib_DestroyCurrentEventTask();
}

void AozrPlayMotion(AozrMotion *motion)
{
    unsigned int flags;
    shen_name name;
    if (motion) {
        flags = motion->flags;
        name = motion->name;
        if (flags & 2)
            FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(name), 0);
        FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(name), motion->motion, 0);
        if (flags & 1)
            FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(name), 69, 0, 0);
        FTBL.mtwkSetSpeed(FTBL.scenFindMotionwork(name), motion->speed, 0);
        FTBL.SetMotionLayerLoopStartTime(FTBL.scenFindMotionwork(name), motion->start, 0);
        FTBL.SetMotionRegionEvalTime(FTBL.scenFindMotionwork(name), motion->start, 0);
        FTBL.SetMotionLayerEndFrame(FTBL.scenFindMotionwork(name), motion->end, 0);
    }
}

void AozrSetPosAng(AozrPosAng *posAng)
{
    int flags;
    shen_name name;
    Vector3 position;
    Vector3i angles;
    if (posAng) {
        flags = posAng->flags;
        name = posAng->name;
        if (flags & 1) {
            FTBL.GetCharaPosition(AOZR_RYO, 0xffffffff, &position, 0x40000000);
            position.x = posAng->x;
            position.z = posAng->z;
            FTBL.EV_AT_SetPos(name, 0x78000000, &position);
        }
        if (flags & 2) {
            angles.x = 0;
            angles.y = (unsigned short)(posAng->angle * 8192.0f / 45.0f);
            angles.z = 0;
        }
    }
}

void AozrSetCamera(AozrCamera *camera, AozrScriptWork *work)
{
    if (camera) {
        if (work && work->name[0])
            FTBL.EV_ControlPackageResources(2, 4, work->name, 0, 0);
        if (camera->target)
            FTBL.EVS_LoopCamera(camera->camera, camera->target, camera->target);
        else
            FTBL.EVS_LoopCamera(camera->camera, 0, 0);
    }
}

int AozrStartSelect(AozrSelect *select)
{
    int result = -1;
    for (;;) {
        if (!select)
            return result;
        if (select->timeout < 0)
            break;
        if (select->mode & 1)
            FTBL.SetupControlSpeakAndUI((short *)select->choices, select->timeout, 0xffff, 2);
        else
            FTBL.SetupControlSpeakAndUI((short *)select->choices, select->timeout, 0xffff, 0);
        return -1;
    }
    if ((result = select->targets[3]) == -1 && (result = select->targets[2]) == -1 && (result = select->targets[1]) == -1 &&
        (result = select->targets[0]) == -1)
        result = select->fallback;
    return result;
}

int AozrPollSelect(AozrSelect *select)
{
    int next = -1;
    int flag;
    if (select && !FTBL.GetShouldRenderIcons(2)) {
        flag = FTBL.shenGetCtrlSpeakIconFlag();
        if (flag == select->choices[0])
            next = select->fallback;
        else if (flag == select->choices[1])
            next = select->targets[0];
        else if (flag == select->choices[2])
            next = select->targets[1];
        else if (flag == select->choices[3])
            next = select->targets[2];
        else if (select->targets[3] != -1)
            next = select->targets[3];
        else if (select->targets[2] != -1)
            next = select->targets[2];
        else if (select->targets[1] != -1)
            next = select->targets[1];
        else if (select->targets[0] != -1)
            next = select->targets[0];
        else
            next = select->fallback;
    }
    return next;
}
#else
const char aozrBootError[] = "AOZR.EV3 Boot Error!!\n";

void AozrDoneCallback(void *data)
{
    AozrShopWork *work = (AozrShopWork *)data;
    int finish = 1;
    HLibTask *parent = 0;
    EventDateTime time;
    while (work) {
        parent = work->parent;
        finish = 0;
        if (FTBL.filmCheckFadeTrackColour())
            break;
        if (work->flags & 2) {
            FTBL.SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->flags &= ~2;
            break;
        }
        FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        EventDebugUnregister(AOZR_DEBUG_ID);
        AozrHideBoard(work);
        if ((work->flags & 4) && work->savedAudio[0]) {
            work->flags &= ~4;
            FTBL.EV_LoadCutsceneFromCurrentScene(work->savedAudio);
            FTBL.EV_LoadAudio(work->savedAudio, work->soundIndex);
            work->savedAudio[0] = 0;
        }
        if (work->seller) {
            AozrRestoreCharacter(work->seller);
            FTBL.AT_SetLocalBaseChara(work->seller, AOZR_INVALID);
            FTBL.npcLockUnlock(work->seller);
            work->seller = 0;
        }
        if (work->flags & 16)
            FTBL.EV_NPC_NewLoadSw_0(1);
        else
            FTBL.EV_NPC_NewLoadSw_0(0);
        AozrRestoreCharacter(AOZR_RYO);
        if ((work->flags & 8) && work->board && work->board->authorBank[0]) {
            FTBL.EV_FreeAuthorPak(0, work->board->authorBank);
            work->flags &= ~8;
        }
        if (work->package[0] && FTBL.EV_ControlPackageResources(0, 5, work->package, 0, 0))
            FTBL.EV_ControlPackageResources(0, 6, work->package, 0, 0);
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        finish = 1;
        break;
    }
    if (finish) {
        FTBL.SetPlayerControlEnabled(1);
        FTBL.HLib_MarkCurrentTaskForCleanup();
        if (parent)
            FTBL.HLib_ResolveDeferredCallback(parent);
        else {
            FTBL.DebugLogToFileRemoved("Error!!\nParent Task ID is none!!  in AOZR.EV3\n");
            FTBL.GetCurrentDateTime((DateTime *)&time);
            time.hour = 23;
            time.minute = 30;
            FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&time);
        }
    }
}

void AozrDebugCallback(void *data)
{
    AozrShopWork *work = (AozrShopWork *)data;
    if (work) {
        FTBL.HLib_AssignCallbackForTask(work->parent, AozrDoneCallback);
        FTBL.HLib_SetupTimedTask(work->parent, 1);
    }
}

void AozrStartPackageCallback(void *data)
{
    if (data) {
        if (!FTBL.filmCheckFadeTrackColour()) {
            FTBL.EV_NPC_NewLoadSw_0(0);
            FTBL.EV_ResetFreeTalkFlags();
            FTBL.HLib_SetCurrentTaskTo(AozrWaitPackageCallback);
        }
    } else
        FTBL.HLib_SetCurrentTaskTo(AozrDoneCallback);
}

void AozrWaitPackageCallback(void *data)
{
    AozrShopWork *work = (AozrShopWork *)data;
    int fail = 0;
    if (work) {
        if (FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)work->package, 0)) {
            FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
            FTBL.EV_ControlPackageResources(2, 2, work->package, 0, 0);
            work->board = (const AozrBoard *)FTBL.LoadCurrentPakBinary("main.bin");
            if (!work->board)
                fail = 1;
            else {
                if (work->board->audio[0] && work->board->audioVariant >= 0) {
                    FTBL.EV_LoadAudio(work->board->audio, work->board->audioVariant);
                    work->flags |= 4;
                }
                if ((work->seller = work->board->seller) && FTBL.CheckWarmStartNpcRoute(work->seller)) {
                    work->flags |= 1;
                    FTBL.HLib_SetCurrentTaskTo(AozrGetNpcCallback);
                } else
                    FTBL.HLib_SetCurrentTaskTo(AozrInitializeCallback);
            }
        }
    } else
        fail = 1;
    if (fail)
        FTBL.HLib_SetCurrentTaskTo(AozrDoneCallback);
}
void AozrGetNpcCallback(void *data)
{
    AozrShopWork *work = (AozrShopWork *)data;
    if (work) {
        if (FTBL.ForceLoadNpc(work->seller)) {
            AozrPrepareSeller(work->seller);
            FTBL.HLib_SetCurrentTaskTo(AozrInitializeCallback);
        }
    } else
        FTBL.HLib_SetCurrentTaskTo(AozrDoneCallback);
}

void AozrInitializeCallback(void *data)
{
    AozrShopWork *work = (AozrShopWork *)data;
    int fail = 1;
    const AozrCell *cells;
    while (work && work->board && work->board->magic == AOZR_MAGIC) {
        if (FTBL.filmCheckFadeTrackColour()) {
            fail = 0;
            break;
        }
        FTBL.SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        work->flags |= 2;
        FTBL.DebugLogToFileRemoved("Now AOZR-DATA file header is %c%c%c%c\n", ((const char *)&work->board->magic)[0], ((const char *)&work->board->magic)[1],
            ((const char *)&work->board->magic)[2], ((const char *)&work->board->magic)[3]);
        EventDebugGetState(AOZR_DEBUG_ID, 1);
        EventDebugCheck(AOZR_DEBUG_ID, 4);
        work->x = work->board->initialX;
        work->y = work->board->initialY;
        work->transition = 0;
        cells = (const AozrCell *)((const char *)work->board + work->board->cellsOffset);
        if (!cells[work->x + work->y * work->board->columns].variant)
            work->camera = 1;
        else
            work->camera = 3;
        if (work->board->startScript == -1)
            FTBL.HLib_SetCurrentTaskTo(AozrStartSaleCallback);
        fail = 0;
        break;
    }
    if (fail)
        FTBL.HLib_SetCurrentTaskTo(AozrDoneCallback);
}

void AozrStartSaleCallback(void *data)
{
    AozrShopWork *work = (AozrShopWork *)data;
    int fail = 1;
    short choices[4];
    Vector3 seller;
    Vector3 player;
    Vector3i rotation;
    float x;
    float z;
    do {
        if (work) {
            if (work->seller) {
                FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(work->seller), 0);
                FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(work->seller), 0xf087, 0);
                FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(work->seller), 0);
                FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(AOZR_RYO), 0);
                FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(AOZR_RYO), 0xf002, 0);
                FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(AOZR_RYO), 0);
                FTBL.GetCharaPosition(AOZR_RYO, 0xffffffff, &player, 0x40000000);
                FTBL.GetCharaPosition(work->seller, 0xffffffff, &seller, 0x40000000);
                x = player.x - seller.x;
                AOZR_SNAP(x);
                z = player.z - seller.z;
                AOZR_SNAP(z);
                rotation.x = 0;
                rotation.y = FTBL.InverseFixedAtan2(x, z);
                rotation.z = 0;
                FTBL.EV_AT_SetAngY_0(work->seller, 0x78000000, &rotation, 0);
                rotation.y += 0x8000;
                FTBL.EV_AT_SetAngY_0(AOZR_RYO, 0x78000000, &rotation, 0);
                if (work->package[0])
                    FTBL.EV_ControlPackageResources(2, 4, work->package, 0, 0);
                FTBL.EVS_LoopCamera(1000, work->seller, work->seller);
            }
            FTBL.dbgPrintEventMessage("おっ、兄ちゃん。見てってくれよ");
            work->elapsed = 0;
            fail = 0;
            choices[0] = -1;
            choices[1] = 162;
            choices[2] = 154;
            choices[3] = -1;
            FTBL.SetupControlSpeakAndUI(choices, 21600, 0xffff, 2);
            FTBL.HLib_SetCurrentTaskTo(AozrWaitStartCallback);
        }
        if (fail)
            FTBL.HLib_SetCurrentTaskTo(AozrDoneCallback);
    } while (0);
}

void AozrWaitStartCallback(void *data)
{
    AozrShopWork *work = (AozrShopWork *)data;
    int fail = 1;
    while (work) {
        work->elapsed++;
        fail = 0;
        if (work->elapsed == 100)
            FTBL.ctrlStopSubtitles();
        if (FTBL.GetShouldRenderIcons(2))
            break;
        if (work->elapsed <= 100)
            FTBL.ctrlStopSubtitles();
        if (FTBL.shenGetCtrlSpeakIconFlag() != 162) {
            fail = 1;
            break;
        }
        AozrUpdateCamera(work);
        fail = !AozrShowBoard(work);
        break;
    }
    if (fail)
        FTBL.HLib_SetCurrentTaskTo(AozrDoneCallback);
}

int AozrShowBoard(AozrShopWork *work)
{
    int result = 0;
    int x;
    int y;
    shen_name name;
    Vector3 scale;
    do {
        if (work && work->board) {
            FTBL.EV_NPC_NewLoadSw_0(0);
            FTBL.EV_DisableCharacterDisplay(AOZR_RYO);
            if (work->seller)
                FTBL.EV_DisableCharacterDisplay(work->seller);
            FTBL.HLib_SetCurrentTaskTo(AozrLockOnCallback);
            result = 1;
            for (x = 0; x < work->board->columns; x++)
                for (y = 0; y < work->board->rows; y++) {
                    name = AozrBoardCell(work->board, 0, 0, x, y);
                    if (name)
                        FTBL.EV_EnableCharacterDisplay(name);
                }
            for (x = 0; x < work->board->columns; x++)
                for (y = 0; y < work->board->rows; y++) {
                    name = AozrBoardCell(work->board, 0, 0, x, y);
                    scale.x = 2.0f;
                    scale.y = 2.0f;
                    scale.z = 2.0f;
                    if (name)
                        FTBL.mtwkSetMotionworkScale3(name, &scale);
                }
        }
        return result;
    } while (0);
}

int AozrHideBoard(AozrShopWork *work)
{
    int result = 0;
    int x;
    int y;
    shen_name name;
    if (work && work->board) {
        FTBL.EV_EnableCharacterDisplay(AOZR_RYO);
        if (work->seller)
            FTBL.EV_EnableCharacterDisplay(work->seller);
        for (x = 0; x < work->board->columns; x++)
            for (y = 0; y < work->board->rows; y++) {
                name = AozrBoardCell(work->board, 0, 0, x, y);
                if (name)
                    FTBL.EV_DisableCharacterDisplay(name);
            }
        result = 1;
    }
    return result;
}

void AozrLockOnCallback(void *data)
{
    AozrShopWork *work = (AozrShopWork *)data;
    const AozrCell *cells;
    unsigned short input;
    short variant;
    int index;
    int distance;
    int x;
    int y;
    shen_name name;
    float amount;
    float goal;
    Vector3 position;
    Vector3i rotation;
    if (work && work->board) {
        cells = AOZR_SELECTED_CELL(work);
        input = FTBL.GetCurrentControlInput(2) & 0xf9fb;
        if (cells->explanation != -1) {
            FTBL.GetBattleDataConfig(0, AOZR_BTIL);
            if (FTBL.GetBattleDataConfig(1, AOZR_BTIL) && FTBL.GetBattleDataConfig(2, AOZR_BTIL))
                input |= 4;
        }
        FTBL.GetBattleDataConfig(0, AOZR_BTEV);
        if (FTBL.GetBattleDataConfig(1, AOZR_BTEV) && FTBL.GetBattleDataConfig(2, AOZR_BTEV))
            input |= 0x400;
        FTBL.GetBattleDataConfig(0, AOZR_BTCC);
        if (FTBL.GetBattleDataConfig(1, AOZR_BTCC) && FTBL.GetBattleDataConfig(2, AOZR_BTCC))
            input |= 0x200;
        if (input & 0x400) {
            if (work->board->buyScript != -1)
                AozrHideBoard(work);
            else
                FTBL.HLib_SetCurrentTaskTo(AozrAskBuyCallback);
        } else if (input & 0x200)
            FTBL.HLib_SetCurrentTaskTo(AozrDoneCallback);
        else if (input & 4)
            AozrExplainSelection(work);
        else if (work->transition <= 0) {
            if (!(input & 0xf000))
                input = work->bufferedInput;
            work->bufferedInput = 0;
            index = work->x + work->y * work->board->columns;
            cells = (const AozrCell *)((const char *)work->board + work->board->cellsOffset);
            variant = cells[index].variant;
            for (;;) {
                work->transition = 1;
                distance = 0;
                if ((input & 0x4000) && (distance = AozrMoveLeft(work)))
                    break;
                if ((input & 0x8000) && (distance = AozrMoveRight(work)))
                    break;
                if ((input & 0x1000) && (distance = AozrMoveUp(work)))
                    break;
                if ((input & 0x2000) && (distance = AozrMoveDown(work)))
                    break;
                work->transition = 0;
                break;
            }
            if (work->transition) {
                index = work->y * work->board->columns + work->x;
                if (!variant) {
                    if (!cells[index].variant) {
                        work->camera = 1;
                        work->transition = work->board->transitionFrames[0];
                    } else {
                        work->camera = 2;
                        work->transition = work->board->transitionFrames[1];
                    }
                } else if (!cells[index].variant) {
                    work->camera = 4;
                    work->transition = work->board->transitionFrames[3];
                } else {
                    work->camera = 3;
                    work->transition = work->board->transitionFrames[2];
                }
                work->transition = work->transition * distance;
            }
        } else {
            input &= 0xf000;
            if (input)
                work->bufferedInput = input;
        }
        amount = -(int)FTBL.GetCurrentControlInput(4) * 0.001953125f;
        AOZR_SNAP(amount);
        goal = amount + 1.0f;
        AOZR_SNAP(goal);
        work->zoom = work->zoom * 0.5f;
        AOZR_SNAP(work->zoom);
        goal = goal * 0.5f;
        AOZR_SNAP(goal);
        work->zoom = work->zoom + goal;
        AOZR_SNAP(work->zoom);
        AozrUpdateCamera(work);
        for (x = 0; x < work->board->columns; x++)
            for (y = 0; y < work->board->rows; y++) {
                name = AozrBoardCell(work->board, &position, &rotation, x, y);
                if (name) {
                    FTBL.EV_AT_SetPos(name, 0x78000000, &position);
                    FTBL.EV_AT_SetAngY_0(name, 0x78000000, &rotation, 0);
                }
            }
    } else {
        FTBL.EV_EnableCharacterDisplay(AOZR_RYO);
        if (work->seller)
            FTBL.EV_EnableCharacterDisplay(work->seller);
        FTBL.HLib_SetCurrentTaskTo(AozrDoneCallback);
    }
}

void AozrAskBuyCallback(void *data)
{
    AozrShopWork *work = (AozrShopWork *)data;
    short choices[4];
    if (work && work->board && work->board->cellsOffset > 0) {
        FTBL.dbgPrintEventMessage("これでいいんだな？");
        choices[0] = -1;
        choices[1] = 80;
        choices[2] = 154;
        choices[3] = -1;
        FTBL.SetupControlSpeakAndUI(choices, 21600, 0xffff, 2);
        FTBL.HLib_SetCurrentTaskTo(AozrWaitBuyCallback);
        work->elapsed = 0;
    } else
        FTBL.HLib_SetCurrentTaskTo(AozrDoneCallback);
}

void AozrWaitBuyCallback(void *data)
{
    AozrShopWork *work = (AozrShopWork *)data;
    const AozrCell *cell;
    if (work && work->board && work->board->cellsOffset > 0) {
        if (work->elapsed == 100)
            FTBL.ctrlStopSubtitles();
        work->elapsed++;
        if (!FTBL.GetShouldRenderIcons(2)) {
            if (work->elapsed <= 100)
                FTBL.ctrlStopSubtitles();
            cell = AOZR_SELECTED_CELL(work);
            if (FTBL.shenGetCtrlSpeakIconFlag() == 80 && cell->price > 0) {
                if (cell->price <= FTBL.GetPlayerStat(2)) {
                    work->elapsed = 0;
                    FTBL.dbgPrintEventMessage("●青ああ");
                } else {
                    work->elapsed = 100;
                    FTBL.dbgPrintEventMessage("●青・・・いや、金が足りないな");
                }
                FTBL.HLib_SetCurrentTaskTo(AozrDeliverCallback);
                FTBL.HLib_SetupTaskDelay(40);
            } else if (!AozrShowBoard(work))
                FTBL.HLib_SetCurrentTaskTo(AozrDoneCallback);
        }
    } else
        FTBL.HLib_SetCurrentTaskTo(AozrDoneCallback);
}

void AozrDeliverCallback(void *data)
{
    AozrShopWork *work = (AozrShopWork *)data;
    const AozrCell *cell;
    if (work && work->board && work->board->cellsOffset >= 0) {
        switch (work->elapsed) {
        case 0:
            cell = AOZR_SELECTED_CELL(work);
            FTBL.AddThisToCashOrSegaCoins(-cell->price, 2);
            FTBL.HLib_SetupTaskDelay(20);
            work->elapsed = 1;
            break;
        case 1:
            FTBL.ctrlStopSubtitles();
            FTBL.HLib_SetupTaskDelay(40);
            work->elapsed = 2;
            break;
        case 2:
            FTBL.dbgPrintEventMessage("ほい。毎度〜");
            FTBL.HLib_SetupTaskDelay(40);
            work->elapsed = 3;
            break;
        case 3:
            FTBL.ctrlStopSubtitles();
            work->elapsed = 4;
            break;
        case 4:
            if (!FTBL.GetShouldRenderIcons(4)) {
                cell = AOZR_SELECTED_CELL(work);
                FTBL.AddInventoryItem(cell->item, 1, 1);
                FTBL.HLib_SetupTaskDelay(40);
                work->elapsed = 200;
            }
            break;
        case 200:
            if (AozrShowBoard(work))
                FTBL.HLib_SetupTaskDelay(30);
            else
                FTBL.HLib_SetCurrentTaskTo(AozrDoneCallback);
            break;
        case 100:
            FTBL.ctrlStopSubtitles();
            FTBL.dbgPrintEventMessage("・・・そうかい。＆まぁ見るだけならただだ");
            FTBL.HLib_SetupTaskDelay(60);
            work->elapsed = 101;
            break;
        case 101:
            FTBL.ctrlStopSubtitles();
            work->elapsed = 200;
            break;
        }
    } else
        FTBL.HLib_SetCurrentTaskTo(AozrDoneCallback);
}

shen_name AozrBoardCell(const AozrBoard *board, Vector3 *position, Vector3i *rotation, int x, int y)
{
    shen_name name = 0;
    const AozrCell *cells;
    Vector3 point;
    Vector3 origin;
    Vector3i angles;
    while (board && board->magic == AOZR_MAGIC && board->columns > (short)x && board->rows > (short)y && x >= 0 && y >= 0 && board->cellsOffset > 0) {
        cells = (const AozrCell *)((const char *)board + board->cellsOffset);
        angles.x = (int)(board->angles.x * 8192.0f / 45.0f) & 0xffff;
        angles.y = (int)(board->angles.y * 8192.0f / 45.0f) & 0xffff;
        angles.z = (int)(board->angles.z * 8192.0f / 45.0f) & 0xffff;
        origin.x = board->origin.x;
        origin.y = board->origin.y;
        origin.z = board->origin.z;
        point.x = board->spacingX * x;
        point.y = board->spacingY * y;
        point.z = 0.0f;
        FTBL.ApplyScenarioTranslationAndRotation(&origin, &angles, &point);
        if (position)
            *position = point;
        if (rotation) {
            rotation->x = angles.x;
            rotation->y = angles.y;
            rotation->z = angles.z + 0x8000;
        }
        name = cells[board->columns * y + x].character;
        break;
    }
    return name;
}

void AozrPrepareSeller(shen_name name)
{
    int hasMotion = FTBL.scenFindMotionwork(name) != 0;
    float values[4];
    if (hasMotion) {
        values[0] = 1.0f;
        values[1] = 1.0f;
        values[2] = 1.0f;
        values[3] = 1.0f;
        FTBL.SetScenarioReflectorBoardValues(name, values);
        FTBL.SetRefBoardParamsForCharaById(name, 1);
    }
    FTBL.EV_SetCharaControllerFlags(name, 1, 0);
    FTBL.EV_SetCharaControllerFlags(name, 0, 0);
    if (hasMotion)
        FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 2);
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}

void AozrRestoreCharacter(shen_name name)
{
    int hasMotion = FTBL.scenFindMotionwork(name) != 0;
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
    if (hasMotion)
        FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 0);
    FTBL.EV_SetCharaControllerFlags(name, 0, 1);
    FTBL.EV_SetCharaControllerFlags(name, 1, 1);
    if (hasMotion)
        FTBL.SetRefBoardParamsForCharaById(name, 0);
}

void AozrUpdateCamera(AozrShopWork *work)
{
    Vector3 target;
    Vector3 eye;
    Vector3i angles;
    float value;
    HLibTask *task;
    int *beep;
    const AozrCell *cell;
    if (!work || !work->board)
        return;
    FTBL.camCtrlGetTargetPosition(&eye);
    target = eye;
    angles.x = 0;
    angles.y = 0;
    angles.z = 0;
    AozrBoardCell(work->board, &target, &angles, work->x, work->y);
    if (work->transition > 0) {
        value = eye.x - target.x;
        AOZR_SNAP(value);
        value *= 0.55f;
        AOZR_SNAP(value);
        value += target.x;
        AOZR_SNAP(value);
        target.x = value;
        value = eye.y - target.y;
        AOZR_SNAP(value);
        value *= 0.55f;
        AOZR_SNAP(value);
        value += target.y;
        AOZR_SNAP(value);
        target.y = value;
        value = eye.z - target.z;
        AOZR_SNAP(value);
        value *= 0.55f;
        AOZR_SNAP(value);
        value += target.z;
        AOZR_SNAP(value);
        target.z = value;
        if (--work->transition == 0) {
            cell = AOZR_SELECTED_CELL(work);
            if (cell->explanation != -1) {
                task = FTBL.HLib_EnqueueTaskWithParameter(AozrBeepCallback, 4, 11, 4, TASK_EVNT);
                if (task && (beep = (int *)FTBL.HLib_GetTaskParameter(task)))
                    *beep = 0;
            }
        }
    } else
        work->transition = 0;
    FTBL.camCtrlSetState(26);
    FTBL.camCtrlSetTargetPosition(&target);
    switch (work->camera) {
    case 1:
        eye.x = work->board->cameraOffsets[0].x;
        eye.y = work->board->cameraOffsets[0].y;
        eye.z = work->board->cameraOffsets[0].z;
        break;
    case 2:
        eye.x = work->board->cameraOffsets[1].x;
        eye.y = work->board->cameraOffsets[1].y;
        eye.z = work->board->cameraOffsets[1].z;
        break;
    case 3:
        eye.x = work->board->cameraOffsets[2].x;
        eye.y = work->board->cameraOffsets[2].y;
        eye.z = work->board->cameraOffsets[2].z;
        break;
    case 4:
        eye.x = work->board->cameraOffsets[3].x;
        eye.y = work->board->cameraOffsets[3].y;
        eye.z = work->board->cameraOffsets[3].z;
        break;
    default:
        eye.x = 0.0f;
        eye.y = 0.0f;
        eye.z = 0.5f;
        break;
    }
    eye.x *= work->zoom;
    eye.y *= work->zoom;
    eye.z *= work->zoom;
    AOZR_SNAP(eye.x);
    AOZR_SNAP(eye.y);
    AOZR_SNAP(eye.z);
    FTBL.ApplyScenarioTranslationAndRotation(&target, &angles, &eye);
    FTBL.camCtrlSetNextPosition(&eye);
    FTBL.camCtrlSetPerspective(5404);
}

void AozrBeepCallback(void *data)
{
    if (data) {
        FTBL.Loop3DSoundEffect(0xe00ab);
        FTBL.HLib_SetupTaskDelay(4);
        FTBL.HLib_DestroyCurrentEventTask();
    } else
        FTBL.HLib_DestroyCurrentEventTask();
}

int AozrExplainSelection(AozrShopWork *work)
{
    int result = 0;
    int delay;
    const AozrCell *cell;
    do {
        if (work || work->board || !work->board->cellsOffset) {
            cell = AOZR_SELECTED_CELL(work);
            delay = 0;
            work->elapsed = 0;
            result = 1;
            switch (cell->explanation) {
            case 0x7fffffd0:
                delay = 60;
                FTBL.dbgPrintEventMessage("これは、見たまんまジッポだよ");
                break;
            case 0x7fffffd1:
                delay = 60;
                FTBL.dbgPrintEventMessage("こいつは、良いライターだぜ");
                break;
            case 0x7fffffd2:
                delay = 60;
                FTBL.dbgPrintEventMessage("うん、１９７０年ものだな");
                break;
            case 0x7fffffe0:
                delay = 60;
                FTBL.dbgPrintEventMessage("これは、まがたまだよ");
                break;
            case 0x7fffffe1:
                delay = 60;
                FTBL.dbgPrintEventMessage("きっとご利益あるぜ");
                break;
            case 0x7ffffff0:
                delay = 60;
                FTBL.dbgPrintEventMessage("実は、白いのとセットなんだ");
                break;
            case 0x7ffffff1:
                delay = 60;
                FTBL.dbgPrintEventMessage("白黒、両方買っていきなよ");
                break;
            default:
                delay = 0;
                result = 0;
                break;
            }
            if (delay) {
                FTBL.HLib_SetCurrentTaskTo(AozrExplainCallback);
                FTBL.HLib_SetupTaskDelay(delay);
            }
        }
        return result;
    } while (0);
}

void AozrExplainCallback(void *data)
{
    AozrShopWork *work = (AozrShopWork *)data;
    if (work) {
        FTBL.ctrlStopSubtitles();
        if (!AozrShowBoard(work))
            FTBL.HLib_SetCurrentTaskTo(AozrDoneCallback);
    }
}

int AozrMoveUp(AozrShopWork *work)
{
    int distance = 0;
    int index;
    do {
        if (work && work->y >= 0) {
            index = work->y - 1;
            while (index >= 0 && !AozrBoardCell(work->board, 0, 0, work->x, index))
                index--;
            distance = work->y - index;
            if (index < 0) {
                index = work->board->rows - 1;
                while (index > work->y && !AozrBoardCell(work->board, 0, 0, work->x, index))
                    index--;
                if (index <= work->y)
                    distance = 0;
                else {
                    distance = index - work->y;
                    work->y = index;
                }
            } else
                work->y = index;
        }
        if (distance > 2)
            distance = 2;
        return distance;
    } while (0);
}

int AozrMoveDown(AozrShopWork *work)
{
    int distance = 0;
    int index;
    do {
        if (!work || work->y > work->board->rows - 1)
            break;
        index = work->y + 1;
        while (index < work->board->rows && !AozrBoardCell(work->board, 0, 0, work->x, index))
            index++;
        distance = index - work->y;
        if (index >= work->board->rows) {
            index = 0;
            while (index < work->y && !AozrBoardCell(work->board, 0, 0, work->x, index))
                index++;
            if (index >= work->y)
                distance = 0;
            else {
                distance = work->y - index;
                work->y = index;
            }
        } else
            work->y = index;
    } while (0);
    if (distance > 2)
        distance = 2;
    return distance;
}

int AozrMoveLeft(AozrShopWork *work)
{
    int distance = 0;
    int index;
    do {
        if (work && work->x >= 0) {
            index = work->x - 1;
            while (index >= 0 && !AozrBoardCell(work->board, 0, 0, index, work->y))
                index--;
            distance = work->x - index;
            if (index < 0) {
                index = work->board->columns - 1;
                while (index > work->x && !AozrBoardCell(work->board, 0, 0, index, work->y))
                    index--;
                if (index <= work->x)
                    distance = 0;
                else {
                    distance = index - work->x;
                    work->x = index;
                }
            } else
                work->x = index;
        }
        if (distance > 2)
            distance = 2;
        return distance;
    } while (0);
}

int AozrMoveRight(AozrShopWork *work)
{
    int distance = 0;
    int index;
    do {
        if (!work || work->x > work->board->columns - 1)
            break;
        index = work->x + 1;
        while (index < work->board->columns && !AozrBoardCell(work->board, 0, 0, index, work->y))
            index++;
        distance = index - work->x;
        if (index >= work->board->columns) {
            index = 0;
            while (index < work->x && !AozrBoardCell(work->board, 0, 0, index, work->y))
                index++;
            if (index >= work->x)
                distance = 0;
            else {
                distance = work->x - index;
                work->x = index;
            }
        } else
            work->x = index;
    } while (0);
    if (distance > 2)
        distance = 2;
    return distance;
}
#endif
#else
#include "s2/areas/area_aozr.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/input.hpp"
#include "s2/inventory.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
namespace shenmue::s2::areas {
namespace {
constexpr shen_name event{SHEN_NAME(EVNT)}, ryo{SHEN_NAME(RYO_)}, invalid{0xffffffffU};
struct Board {
    shen_name magic;
    std::byte reserved04[8];
    char audio[12];
    std::int32_t audioVariant;
    char authorBank[12];
    shen_name seller;
    float spacingX, spacingY;
    Vector3 origin, angles;
    std::uint32_t reserved4c;
    std::int16_t transitionFrames[4];
    Vector3 cameraOffsets[4];
    std::int16_t initialX, initialY, columns, rows;
    std::int32_t cellsOffset, startScript, buyScript;
};
struct Cell {
    shen_name character;
    std::int32_t price;
    std::uint32_t item;
    std::int32_t explanation, reserved10;
};
struct ShopWork {
    HLib::Task *self;
    HLib::Task *parent;
    char package[8];
    shen_name seller;
    std::uint32_t flags;
    std::int32_t x, y, camera, transition, elapsed, reserved34;
    const Board *board;
    float zoom;
    std::int32_t bufferedInput;
    std::uint64_t reserved48;
    char savedAudio[8];
};
static_assert(offsetof(Board, cellsOffset) == 0x90 && offsetof(Board, columns) == 0x8c && offsetof(Board, cameraOffsets) == 0x58);
static_assert(sizeof(Cell) == 0x14 && sizeof(ShopWork) == 0x58 && offsetof(ShopWork, savedAudio) == 0x50);
void StartPackage_Callback(void *);
void WaitPackage_Callback(void *);
void GetNpc_Callback(void *);
void Initialize_Callback(void *);
void StartSale_Callback(void *);
void WaitStart_Callback(void *);
void LockOn_Callback(void *);
void AskBuy_Callback(void *);
void WaitBuy_Callback(void *);
void Explain_Callback(void *);
void Deliver_Callback(void *);
void Done_Callback(void *);
const Cell *SelectedCell(const ShopWork *work) {
    return reinterpret_cast<const Cell *>(reinterpret_cast<const std::byte *>(work->board) + work->board->cellsOffset) + work->x +
        work->y * work->board->columns;
}
std::int16_t Variant(const Cell *cell) {
    return static_cast<std::int16_t>(cell->item >> 16);
}
float Snap(float value) {
    return std::fabs(value) < 1.1920929e-7f ? 0.0f : value;
}

shen_name BoardCell(const Board *board, Vector3 *position, Vector3i *rotation, std::int32_t x, std::int32_t y) {
    if (!board || board->magic.name != SHEN_NAME(AOZR) || x < 0 || y < 0 || x >= board->columns || y >= board->rows || board->cellsOffset <= 0)
        return shen_name{};
    Vector3i angles{static_cast<std::int32_t>((board->angles.x * 8192) / 45), static_cast<std::int32_t>((board->angles.y * 8192) / 45),
        static_cast<std::int32_t>((board->angles.z * 8192) / 45)};
    Vector3 point{static_cast<float>(x) * board->spacingX, static_cast<float>(y) * board->spacingY, 0};
    scenApplyTransRot(&board->origin, &angles, &point);
    if (position)
        *position = point;
    if (rotation) {
        *rotation = angles;
        rotation->z += 0x8000;
    }
    const auto *cells = reinterpret_cast<const Cell *>(reinterpret_cast<const std::byte *>(board) + board->cellsOffset);
    return cells[x + y * board->columns].character;
}

bool HideBoard(ShopWork *work) {
    if (!work || !work->board)
        return false;
    EV_EnableCharacterDisplay(ryo);
    if (work->seller.name)
        EV_EnableCharacterDisplay(work->seller);
    for (int x = 0; x < work->board->columns; ++x)
        for (int y = 0; y < work->board->rows; ++y) {
            const auto name = BoardCell(work->board, nullptr, nullptr, x, y);
            if (name.name)
                EV_DisableCharacterDisplay(name);
        }
    return true;
}
void Beep_Callback(void *data) {
    if (data) {
        Loop3DSoundEffect(0xe00ab);
        HLib::SetupTaskDelay(4);
    }
    HLib::DestroyCurrentTask();
}

void UpdateCamera(ShopWork *work) {
    using CameraState::Callback;
    if (!work || !work->board)
        return;
    Vector3 eye;
    camCtrlGetNextCamPos(&eye);
    Vector3 target = eye;
    Vector3i angles{};
    BoardCell(work->board, &target, &angles, work->x, work->y);
    if (work->transition <= 0)
        work->transition = 0;
    else {
        for (int i = 0; i < 3; ++i)
            target[i] = Snap(Snap(Snap(eye[i] - target[i]) * .55f) + target[i]);
        if (--work->transition == 0 && SelectedCell(work)->explanation != -1) {
            auto *task = HLib::EnqueueTaskWithParameter(Beep_Callback, 4, 0x0b, 4, event);
            if (task) {
                auto *data = static_cast<std::uint32_t *>(HLib::GetTaskParameter(task));
                if (data)
                    *data = 0;
            }
        }
    }
    camCtrlSetState(Callback);
    camCtrlSetTargetPosition(target);
    Vector3 offset = work->camera >= 1 && work->camera <= 4 ? work->board->cameraOffsets[work->camera - 1] : Vector3{0, 0, .5f};
    for (auto &axis : offset)
        axis = Snap(axis * work->zoom);
    scenApplyTransRot(&target, &angles, &offset);
    camCtrlSetNextPosition(offset);
    camCtrlSetFov(5404);
}

bool ShowBoard(ShopWork *work) {
    if (!work || !work->board)
        return false;
    EV_NPC_NewLoadSw(0);
    EV_DisableCharacterDisplay(ryo);
    if (work->seller.name)
        EV_DisableCharacterDisplay(work->seller);
    HLib::SetCurrentTaskTo(LockOn_Callback);
    for (int x = 0; x < work->board->columns; ++x)
        for (int y = 0; y < work->board->rows; ++y) {
            const auto name = BoardCell(work->board, nullptr, nullptr, x, y);
            if (name.name)
                EV_EnableCharacterDisplay(name);
        }
    const Vector3 scale{2, 2, 2};
    for (int x = 0; x < work->board->columns; ++x)
        for (int y = 0; y < work->board->rows; ++y) {
            const auto name = BoardCell(work->board, nullptr, nullptr, x, y);
            if (name.name)
                mtwkSetMotionworkScale3(name, &scale);
        }
    return true;
}

std::int32_t MoveSelection(ShopWork *work, bool horizontal, bool positive) {
    if (!work)
        return 0;
    auto &selected = horizontal ? work->x : work->y;
    const auto count = horizontal ? work->board->columns : work->board->rows;
    const auto old = selected;
    auto exists = [&](int index) { return BoardCell(work->board, nullptr, nullptr, horizontal ? index : work->x, horizontal ? work->y : index).name != 0; };
    int index = old + (positive ? 1 : -1);
    if (positive) {
        if (old > count - 1)
            return 0;
        while (index < count && !exists(index))
            ++index;
        if (index >= count) {
            index = 0;
            while (index < old && !exists(index))
                ++index;
            if (index >= old)
                return 0;
        }
    } else {
        if (old < 0)
            return 0;
        while (index >= 0 && !exists(index))
            --index;
        if (index < 0) {
            index = count - 1;
            while (index > old && !exists(index))
                --index;
            if (index <= old)
                return 0;
        }
    }
    selected = index;
    return std::min(2, std::abs(index - old));
}

bool ExplainSelection(ShopWork *work) {
    work->elapsed = 0;
    const char *text;
    switch (SelectedCell(work)->explanation) {
    case 0x7fffffd0:
        text = "これは、見たまんまジッポだよ";
        break;
    case 0x7fffffd1:
        text = "こいつは、良いライターだぜ";
        break;
    case 0x7fffffd2:
        text = "うん、１９７０年ものだな";
        break;
    case 0x7fffffe0:
        text = "これは、まがたまだよ";
        break;
    case 0x7fffffe1:
        text = "きっとご利益あるぜ";
        break;
    case 0x7ffffff0:
        text = "実は、白いのとセットなんだ";
        break;
    case 0x7ffffff1:
        text = "白黒、両方買っていきなよ";
        break;
    default:
        return false;
    }
    dbgPrintEventMessage(text);
    HLib::SetCurrentTaskTo(Explain_Callback);
    HLib::SetupTaskDelay(60);
    return true;
}

void LockOn_Callback(void *data) {
    auto *work = static_cast<ShopWork *>(data);
    if (!work || !work->board) {
        EV_EnableCharacterDisplay(ryo);
        if (work && work->seller.name)
            EV_EnableCharacterDisplay(work->seller);
        HLib::SetCurrentTaskTo(Done_Callback);
        return;
    }
    std::int16_t input = static_cast<std::int16_t>(GetCurrentControlInput(INPUT_NEW_PRESSES) & 0xf9fb);
    const auto *cell = SelectedCell(work);
    if (cell->explanation != -1) {
        GetBattleDataConfig(0, SHEN_NAME(BTIL));
        if (GetBattleDataConfig(1, SHEN_NAME(BTIL)) && GetBattleDataConfig(2, SHEN_NAME(BTIL)))
            input |= 4;
    }
    GetBattleDataConfig(0, SHEN_NAME(BTEV));
    if (GetBattleDataConfig(1, SHEN_NAME(BTEV)) && GetBattleDataConfig(2, SHEN_NAME(BTEV)))
        input |= 0x400;
    GetBattleDataConfig(0, SHEN_NAME(BTCC));
    if (GetBattleDataConfig(1, SHEN_NAME(BTCC)) && GetBattleDataConfig(2, SHEN_NAME(BTCC)))
        input |= 0x200;
    if (input & 0x400) {
        if (work->board->buyScript == -1)
            HLib::SetCurrentTaskTo(AskBuy_Callback);
        else
            HideBoard(work);
    } else if (input & 0x200)
        HLib::SetCurrentTaskTo(Done_Callback);
    else if (input & 4)
        ExplainSelection(work);
    else if (work->transition > 0) {
        if (input & 0xf000)
            work->bufferedInput = input & 0xf000;
    } else {
        if (!(input & 0xf000))
            input = static_cast<std::int16_t>(work->bufferedInput);
        work->bufferedInput = 0;
        const auto previousVariant = Variant(cell);
        work->transition = 1;
        int distance = 0;
        if ((input & 0x4000) && (distance = MoveSelection(work, true, false))) {
        } else if (input < 0 && (distance = MoveSelection(work, true, true))) {
        } else if ((input & 0x1000) && (distance = MoveSelection(work, false, false))) {
        } else if ((input & 0x2000) && (distance = MoveSelection(work, false, true))) {
        } else
            work->transition = 0;
        if (work->transition) {
            const bool nextVariant = Variant(SelectedCell(work)) != 0;
            work->camera = previousVariant ? (nextVariant ? 3 : 4) : (nextVariant ? 2 : 1);
            work->transition = distance * work->board->transitionFrames[work->camera - 1];
        }
    }
    const auto amount = Snap(-static_cast<float>(GetCurrentControlInput(INPUT_LEFT_TRIGGER)) * .001953125f);
    const auto goal = Snap(amount + 1);
    work->zoom = Snap(work->zoom * .5f);
    work->zoom = Snap(Snap(goal * .5f) + work->zoom);
    UpdateCamera(work);
    for (int x = 0; x < work->board->columns; ++x)
        for (int y = 0; y < work->board->rows; ++y) {
            Vector3 position;
            Vector3i rotation;
            const auto name = BoardCell(work->board, &position, &rotation, x, y);
            if (name.name) {
                EV_AT_SetPos(name, EventTransformFlags::AllAxes | EventTransformFlags::TurnMotion, position);
                EV_AT_SetAngY(name, EventTransformFlags::AllAxes | EventTransformFlags::TurnMotion, rotation);
            }
        }
}
void Explain_Callback(void *data) {
    auto *work = static_cast<ShopWork *>(data);
    if (work) {
        ctrlStopSubtitles();
        if (!ShowBoard(work))
            HLib::SetCurrentTaskTo(Done_Callback);
    }
}

void Deliver_Callback(void *data) {
    auto *work = static_cast<ShopWork *>(data);
    if (!work || !work->board || work->board->cellsOffset < 0) {
        HLib::SetCurrentTaskTo(Done_Callback);
        return;
    }
    switch (work->elapsed) {
    case 0:
        AddThisToCashOrSegaCoins(-SelectedCell(work)->price, ECashOrCoins::Cash2);
        HLib::SetupTaskDelay(20);
        work->elapsed = 1;
        break;
    case 1:
        ctrlStopSubtitles();
        HLib::SetupTaskDelay(40);
        work->elapsed = 2;
        break;
    case 2:
        dbgPrintEventMessage("ほい。毎度～");
        HLib::SetupTaskDelay(40);
        work->elapsed = 3;
        break;
    case 3:
        ctrlStopSubtitles();
        work->elapsed = 4;
        break;
    case 4:
        if (!GetShouldRenderIcons(RenderIconSlot::MoneyMessage)) {
            AddInventoryItem(SelectedCell(work)->item, 1, 1);
            HLib::SetupTaskDelay(40);
            work->elapsed = 200;
        }
        break;
    case 100:
        ctrlStopSubtitles();
        dbgPrintEventMessage("・・・そうかい。＆まぁ見るだけならただだ");
        HLib::SetupTaskDelay(60);
        work->elapsed = 101;
        break;
    case 101:
        ctrlStopSubtitles();
        work->elapsed = 200;
        break;
    case 200:
        if (!ShowBoard(work))
            HLib::SetCurrentTaskTo(Done_Callback);
        else
            HLib::SetupTaskDelay(30);
        break;
    default:
        break;
    }
}

void WaitBuy_Callback(void *data) {
    auto *work = static_cast<ShopWork *>(data);
    if (!work || !work->board || work->board->cellsOffset <= 0) {
        HLib::SetCurrentTaskTo(Done_Callback);
        return;
    }
    if (work->elapsed == 100)
        ctrlStopSubtitles();
    ++work->elapsed;
    if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
        return;
    if (work->elapsed <= 100)
        ctrlStopSubtitles();
    if (shenGetCtrlSpeakIconFlag() == 80 && SelectedCell(work)->price > 0) {
        if (static_cast<std::uint64_t>(SelectedCell(work)->price) > GetPlayerStat(PlayerStatsId::Cash)) {
            work->elapsed = 100;
            dbgPrintEventMessage("●青・・・いや、金が足りないな");
        } else {
            work->elapsed = 0;
            dbgPrintEventMessage("●青ああ");
        }
        HLib::SetCurrentTaskTo(Deliver_Callback);
        HLib::SetupTaskDelay(40);
    } else if (!ShowBoard(work))
        HLib::SetCurrentTaskTo(Done_Callback);
}
void AskBuy_Callback(void *data) {
    auto *work = static_cast<ShopWork *>(data);
    if (!work || !work->board || work->board->cellsOffset <= 0) {
        HLib::SetCurrentTaskTo(Done_Callback);
        return;
    }
    dbgPrintEventMessage("これでいいんだな？");
    std::int16_t choices[]{-1, 80, 154, -1};
    SetupControlSpeakAndUI(choices, 21600, -1, 2);
    HLib::SetCurrentTaskTo(WaitBuy_Callback);
    work->elapsed = 0;
}
void WaitStart_Callback(void *data) {
    auto *work = static_cast<ShopWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(Done_Callback);
        return;
    }
    ++work->elapsed;
    if (work->elapsed == 100)
        ctrlStopSubtitles();

    if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
        return;
    if (work->elapsed <= 100)
        ctrlStopSubtitles();
    if (shenGetCtrlSpeakIconFlag() != 162) {
        HLib::SetCurrentTaskTo(Done_Callback);
        return;
    }
    UpdateCamera(work);
    if (!ShowBoard(work))
        HLib::SetCurrentTaskTo(Done_Callback);
}

void StartSale_Callback(void *data) {
    auto *work = static_cast<ShopWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(Done_Callback);
        return;
    }
    if (work->seller.name) {
        mtwkSetRequestedRootMode(scenFindMotionwork(work->seller), 0);
        mtwkRequestMotion(scenFindMotionwork(work->seller), 0xf087, MotionRegion::LowerBody);
        mtwkSetRequestedRootMode(scenFindMotionwork(work->seller), 0);
        mtwkSetRequestedRootMode(scenFindMotionwork(ryo), 0);
        mtwkRequestMotion(scenFindMotionwork(ryo), 0xf002, MotionRegion::LowerBody);
        mtwkSetRequestedRootMode(scenFindMotionwork(ryo), 0);
        Vector3 player, seller;
        EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::TurnMotion);
        EV_GetCharaPosition(work->seller, CHARACTER_TRANSLATION_ROOT, &seller, EventTransformFlags::TurnMotion);
        Vector3i rotation{0, static_cast<std::int16_t>(atan2_fixed16(-Snap(player.x - seller.x), -Snap(player.z - seller.z))), 0};
        EV_AT_SetAngY(work->seller, EventTransformFlags::AllAxes | EventTransformFlags::TurnMotion, rotation);
        rotation.y += 0x8000;
        EV_AT_SetAngY(ryo, EventTransformFlags::AllAxes | EventTransformFlags::TurnMotion, rotation);
        if (work->package[0])
            EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>(work->package), 0);
        EVS_LoopCamera(1000, work->seller, work->seller);
    }
    dbgPrintEventMessage("おっ、兄ちゃん。見てってくれよ");
    work->elapsed = 0;
    std::int16_t choices[]{-1, 162, 154, -1};
    SetupControlSpeakAndUI(choices, 21600, -1, 2);
    HLib::SetCurrentTaskTo(WaitStart_Callback);
}
void Initialize_Callback(void *data) {
    auto *work = static_cast<ShopWork *>(data);
    if (!work || !work->board || work->board->magic.name != SHEN_NAME(AOZR)) {
        HLib::SetCurrentTaskTo(Done_Callback);
        return;
    }
    if (filmCheckFadeTrackColour())
        return;
    SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
    work->flags |= 2;
    work->x = work->board->initialX;
    work->y = work->board->initialY;
    work->transition = 0;
    work->camera = Variant(SelectedCell(work)) ? 3 : 1;
    if (work->board->startScript == -1)
        HLib::SetCurrentTaskTo(StartSale_Callback);
}

void PrepareSeller(shen_name name) {
    const bool hasMotion = scenFindMotionwork(name) != nullptr;
    if (hasMotion) {
        const Vector4 values{1, 1, 1, 1};
        SetScenarioReflectorBoardValues(name, &values);
        EV_SetRefBoardParamsFor(name, 1);
    }
    EV_SetCharaControllerFlags(name, 1, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(name, 0, NpcCharacterControllerState::Disabled);
    if (hasMotion)
        mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
    EV_SetCharacterAlphaEnabled(name, false);
}
void GetNpc_Callback(void *data) {
    auto *work = static_cast<ShopWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(Done_Callback);
        return;
    }
    if (EV_ForceLoadNpc(work->seller)) {
        PrepareSeller(work->seller);
        HLib::SetCurrentTaskTo(Initialize_Callback);
    }
}
void WaitPackage_Callback(void *data) {
    auto *work = static_cast<ShopWork *>(data);
    if (!work) {
        HLib::SetCurrentTaskTo(Done_Callback);
        return;
    }
    if (!EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>(work->package)))
        return;
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>(work->package), 0);
    work->board = static_cast<const Board *>(LoadCurrentPakBinary("main.bin"));
    if (!work->board) {
        HLib::SetCurrentTaskTo(Done_Callback);
        return;
    }
    if (work->board->audio[0] && work->board->audioVariant >= 0) {
        EV_LoadAudio(work->board->audio, work->board->audioVariant);
        work->flags |= 4;
    }
    work->seller = work->board->seller;
    if (work->seller.name && EV_CheckWarmStartNpcRoute(work->seller)) {
        work->flags |= 1;
        HLib::SetCurrentTaskTo(GetNpc_Callback);
    } else
        HLib::SetCurrentTaskTo(Initialize_Callback);
}
void StartPackage_Callback(void *data) {
    if (!data)
        HLib::SetCurrentTaskTo(Done_Callback);
    else if (!filmCheckFadeTrackColour()) {
        EV_NPC_NewLoadSw(0);
        EV_ResetFreeTalkFlags();
        HLib::SetCurrentTaskTo(WaitPackage_Callback);
    }
}

void Done_Callback(void *data) {
    using enum CameraState;
    auto *work = static_cast<ShopWork *>(data);
    HLib::Task *parent = nullptr;
    if (work) {
        parent = work->parent;
        if (filmCheckFadeTrackColour())
            return;
        if (work->flags & 2) {
            SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
            work->flags &= ~2U;
            return;
        }
        SetScreenFilmFade(32, 255, 0, 0, 0, 0, 0, 0, 0);
        HideBoard(work);
        if ((work->flags & 4) && work->savedAudio[0]) {
            work->flags &= ~4U;
            EV_LoadCutsceneFromCurrentScene(work->savedAudio);
            EV_LoadAudio(work->savedAudio, work->y);
            work->savedAudio[0] = 0;
        }
        auto restore = [](shen_name name) {
            const bool motion = scenFindMotionwork(name) != nullptr;
            EV_SetCharacterAlphaEnabled(name, true);
            if (motion)
                mtwkSetExtraFlag(scenFindMotionwork(name), 6, 0);
            EV_SetCharaControllerFlags(name, 0, NpcCharacterControllerState::Enabled);
            EV_SetCharaControllerFlags(name, 1, NpcCharacterControllerState::Enabled);
            if (motion)
                EV_SetRefBoardParamsFor(name, 0);
        };
        if (work->seller.name) {
            restore(work->seller);
            AT_SetLocalBaseChara(work->seller, invalid);
            EV_npcLockUnlock(work->seller);
            work->seller = shen_name{};
        }
        EV_NPC_NewLoadSw((work->flags & 16) != 0);
        restore(ryo);
        if ((work->flags & 8) && work->board && work->board->authorBank[0]) {
            EV_FreeAuthorPak(0, work->board->authorBank);
            work->flags &= ~8U;
        }
        if (work->package[0] && EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(work->package), 0))
            EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(work->package), 0);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
    }
    SetPlayerControlEnabled(1);
    HLib::MarkCurrentTaskForCleanup();
    if (parent)
        HLib::ResolveDeferredCallback(parent);
    else {
        DateTime time;
        shenGetCurrentDateTime(time);
        time.hour = 23;
        time.minute = 30;
        SetCurrentDateTimeTo(time);
    }
}
} // namespace
void AreaAOZRInit() {
}

void AreaAOZRMain(std::uintptr_t parameter) {
    const auto argument = static_cast<std::int32_t>(parameter);
    if (argument > 999)
        return;
    const char *sound = shenGetCurrentSbt();
    const auto length = sound ? std::strlen(sound) : 0;
    auto *task = HLib::EnqueueTaskWithParameter(StartPackage_Callback, 4, 0x0b, 0x58 + length, event);
    if (!task)
        return;
    auto *work = static_cast<ShopWork *>(HLib::GetTaskParameter(task));
    if (!work) {
        HLib::DestroyTask(task);
        return;
    }
    std::memset(work, 0, 0x58);
    work->self = task;
    work->parent = HLib::GetCurrentTask();
    std::memcpy(work->package, "AOZR", 4);
    const auto index = (argument >> 1) + 1;
    work->package[4] = static_cast<char>((index / 100) % 10 + 48);
    work->package[5] = static_cast<char>((index / 10) % 10 + 48);
    work->package[6] = static_cast<char>(index % 10 + 48);
    work->package[7] = 0;
    work->zoom = 1;
    work->y = static_cast<std::int32_t>(GetSelectedSoundSceneIndex());
    auto *savedAudio = reinterpret_cast<char *>(work) + 0x50;
    if (length)
        std::memcpy(savedAudio, sound, length);
    savedAudio[length] = 0;
    SetPlayerControlEnabled(0);
    HLib::SetupTaskDelay(0);
    SetScreenFilmFade(32, 0, 0, 0, 0, 255, 0, 0, 0);
    work->flags &= ~2U;
}
const ScriptAreaDefinition areaAOZRScriptDefinition{.areaId = SHEN_NAME(AOZR), .field04 = 0, .init = AreaAOZRInit, .main = AreaAOZRMain};
} // namespace shenmue::s2::areas
#endif
