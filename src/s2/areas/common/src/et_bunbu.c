#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
#include "s2/event_queued_dc.h"

#define WAREHOUSE_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define WAREHOUSE_LOCAL_XYZ 0x38000000U
#define WAREHOUSE_ARG(n) script[script[1] + (n)]

char warehouseDigits[16] = "0123456789ABCDEF";
void *warehouseDialogue = 0;
short warehouseButtons[6] = {3, 4, 5, 6, 3, 4};

void QueueWarehouseScriptLoop(unsigned int *script);
unsigned int GetWarehouseScriptCommand(unsigned int *script);
void SetWarehouseScriptCommand(unsigned int *script, unsigned int command);
int IsWarehouseScriptRunning(unsigned int *script);
void WarehouseScriptCallback(void *data);
void StopWarehouseScript(unsigned int *script);
void ExecuteWarehouseScript(unsigned int *script);
void AdvanceWarehouseScript(unsigned int *script);
void SeekWarehouseScriptLabel(unsigned int *script, unsigned short label);
void PrintWarehouseHex(int column, int line, unsigned int value);
void PrintWarehouseDecimal(int column, int line, unsigned int value);
void StopWarehouseSerifCallback(void *data);
void QueueWarehouseSerif(const char *text, int frames);
short GetWarehouseInput(short pad, short button);
void AdvanceWarehouseMorning(void);
void SetWarehouseNight(void);

void QueueWarehouseScriptLoop(unsigned int *script)
{
    script[0] = (unsigned int)FTBL.HLib_EnqueueTaskWithParameter(WarehouseScriptCallback, 4, 11, 4, TASK_EVNT);
    *(unsigned int **)FTBL.HLib_GetTaskParameter((HLibTask *)script[0]) = script;
    script[2] = script[3] = 0;
    script[1] = 4;
    FTBL.DebugLogToFileRemoved("Script_Start:%8X\n", script);
}

unsigned int GetWarehouseScriptCommand(unsigned int *script)
{
    return script[3];
}

void SetWarehouseScriptCommand(unsigned int *script, unsigned int command)
{
    script[3] = command;
}

int IsWarehouseScriptRunning(unsigned int *script)
{
    return script[0] != 0;
}

void WarehouseScriptCallback(void *data)
{
    ExecuteWarehouseScript(*(unsigned int **)data);
}

void StopWarehouseScript(unsigned int *script)
{
    if (script[0])
        FTBL.HLib_DestroyTaskInternal((HLibTask *)script[0]);
}

void ExecuteWarehouseScript(unsigned int *script)
{
    int loop = 1;
    short choices[4], selectChoices[4];
    Vector3 position;
    Vector3i rotation;
    Vector3 placePosition;
    Vector3i placeRotation;
    while (loop) {
        switch (script[script[1]]) {
        case 0x80010001:
            FTBL.EV_authSceneCtrl(-11);
            FTBL.EV_authSceneCtrl(WAREHOUSE_ARG(1));
            AdvanceWarehouseScript(script);
            break;
        case 0x80000004:
            FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
            AdvanceWarehouseScript(script);
            break;
        case 0x80030006:
            if (FTBL.EV_GetShenmueFlag(WAREHOUSE_ARG(1)))
                SeekWarehouseScriptLabel(script, WAREHOUSE_ARG(2));
            else
                SeekWarehouseScriptLabel(script, WAREHOUSE_ARG(3));
            continue;
        case 0x80020007:
            FTBL.EV_stepSetFlags(WAREHOUSE_ARG(1), WAREHOUSE_ARG(2));
            AdvanceWarehouseScript(script);
            break;
        case 0x80010008:
            SeekWarehouseScriptLabel(script, WAREHOUSE_ARG(1));
            continue;
        case 0x80010012:
            if (FTBL.EV_authSceneCtrl(-1)) {
                SHENMUE_EVENT_BOUNDARY();
                if (FTBL.EVT_GetControllerInput(0, 1) == 1024) {
                    SeekWarehouseScriptLabel(script, WAREHOUSE_ARG(1));
                } else {
                    loop = 0;
                }
            } else {
                AdvanceWarehouseScript(script);
            }
            break;
        case 0x80000002:
            if (!FTBL.EV_authSceneCtrl(-1)) {
                AdvanceWarehouseScript(script);
            } else {
                loop = 0;
            }
            break;
        case 0x80010009:
            if (FTBL.EV_authSceneCtrl(-1)) {
                SHENMUE_EVENT_BOUNDARY();
                if (FTBL.EV_authSceneCtrl(-6) > (int)WAREHOUSE_ARG(1) + FTBL.EV_authSceneCtrl(-5)) {
                    AdvanceWarehouseScript(script);
                } else {
                    script[2]++;
                    loop = 0;
                }
                break;
            }
            AdvanceWarehouseScript(script);
            break;
        case 0x8001000e:
            script[0] = 0;
            script[3] = WAREHOUSE_ARG(1);
            FTBL.HLib_DestroyCurrentTask();
            loop = 0;
            break;
        case 0x80040003:
            if (!script[2]) {
                choices[0] = -1;
                choices[1] = WAREHOUSE_ARG(1);
                choices[2] = WAREHOUSE_ARG(2);
                choices[3] = -1;
                FTBL.SetupControlSpeakAndUI(choices, 1800, WAREHOUSE_ARG(2), 2);
                FTBL.SetRenderClockAndCompassTo(1);
                FTBL.shenSetTimeSimulationTo(0);
            } else {
                int choice = FTBL.shenGetCtrlSpeakIconFlag();
                if (!FTBL.GetShouldRenderIcons(2)) {
                    FTBL.SetRenderClockAndCompassTo(0);
                    FTBL.shenSetTimeSimulationTo(1);
                    if (choice == WAREHOUSE_ARG(1)) {
                        SeekWarehouseScriptLabel(script, WAREHOUSE_ARG(3));
                        continue;
                    }
                    SeekWarehouseScriptLabel(script, WAREHOUSE_ARG(4));
                }
            }
            script[2]++;
            loop = 0;
            break;
        case 0x8006001a:
            if (!script[2]) {
                selectChoices[0] = -1;
                selectChoices[1] = WAREHOUSE_ARG(1);
                selectChoices[2] = WAREHOUSE_ARG(2);
                selectChoices[3] = -1;
                FTBL.SetupControlSpeakAndUI(selectChoices, WAREHOUSE_ARG(6), WAREHOUSE_ARG(5), 2);
                FTBL.SetRenderClockAndCompassTo(1);
                FTBL.shenSetTimeSimulationTo(0);
            } else {
                int choice = FTBL.shenGetCtrlSpeakIconFlag();
                if (!FTBL.GetShouldRenderIcons(2)) {
                    FTBL.SetRenderClockAndCompassTo(0);
                    FTBL.shenSetTimeSimulationTo(1);
                    if (choice == WAREHOUSE_ARG(1))
                        SeekWarehouseScriptLabel(script, WAREHOUSE_ARG(3));
                    else
                        SeekWarehouseScriptLabel(script, WAREHOUSE_ARG(4));
                    continue;
                }
            }
            script[2]++;
            loop = 0;
            break;
        case 0x80010005:
            if (!script[2])
                FTBL.CheckWarmStartNpcRoute(WAREHOUSE_ARG(1));
            else if (FTBL.ForceLoadNpc(WAREHOUSE_ARG(1))) {
                FTBL.EV_SetCharacterAlphaEnabled(WAREHOUSE_ARG(1), 0);
                AdvanceWarehouseScript(script);
                break;
            }
            script[2]++;
            loop = 0;
            break;
        case 0x80010015:
            FTBL.CheckWarmStartNpcRoute(WAREHOUSE_ARG(1));
            FTBL.npcFree(WAREHOUSE_ARG(1));
            AdvanceWarehouseScript(script);
            break;
        case 0x80010018:
            FTBL.CheckWarmStartNpcRoute(WAREHOUSE_ARG(1));
            AdvanceWarehouseScript(script);
            break;
        case 0x8004000f:
            FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(WAREHOUSE_ARG(1)), 71, 0, 0);
            position.x = (float)WAREHOUSE_ARG(2) / 100.0f;
            position.y = (float)WAREHOUSE_ARG(3) / 100.0f;
            position.z = (float)WAREHOUSE_ARG(4) / 100.0f;
            FTBL.EV_AT_SetPos(WAREHOUSE_ARG(1), WAREHOUSE_LOCAL_XYZ, &position);
            AdvanceWarehouseScript(script);
            break;
        case 0x80040010:
            FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(WAREHOUSE_ARG(1)), 71, 0, 0);
            rotation.x = WAREHOUSE_ARG(2);
            rotation.y = WAREHOUSE_ARG(3);
            rotation.z = WAREHOUSE_ARG(4);
            FTBL.EV_AT_SetAngY_0(WAREHOUSE_ARG(1), WAREHOUSE_LOCAL_XYZ, &rotation, 0);
            AdvanceWarehouseScript(script);
            break;
        case 0x80050011:
            FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(WAREHOUSE_ARG(1)), 71, 0, 0);
            placePosition.x = (float)WAREHOUSE_ARG(2) / 100.0f;
            placePosition.y = (float)WAREHOUSE_ARG(3) / 100.0f;
            placePosition.z = (float)WAREHOUSE_ARG(4) / 100.0f;
            placeRotation.x = 0;
            placeRotation.y = WAREHOUSE_ARG(5);
            placeRotation.z = 0;
            FTBL.EV_AT_SetPos(WAREHOUSE_ARG(1), WAREHOUSE_LOCAL_XYZ, &placePosition);
            FTBL.EV_AT_SetAngY_0(WAREHOUSE_ARG(1), WAREHOUSE_LOCAL_XYZ, &placeRotation, 0);
            AdvanceWarehouseScript(script);
            break;
        case 0x8001000a:
            FTBL.CheckHelpIcon(WAREHOUSE_ARG(1), 0, 0);
            AdvanceWarehouseScript(script);
            break;
        case 0x8001000b:
            FTBL.CheckHelpIcon(WAREHOUSE_ARG(1), WAREHOUSE_ARG(1), 2);
            AdvanceWarehouseScript(script);
            break;
        case 0x8001000d:
            script[2]++;
            if (WAREHOUSE_ARG(1) < script[2])
                AdvanceWarehouseScript(script);
            loop = 0;
            break;
        case 0x80020014:
            script[2]++;
            {
                Vector3 attachPosition = {0.15f, 0.0f, -0.035f};
                Vector3i attachRotation = {0, 0, 0xc000};
                if (script[2] == 1) {
                    FTBL.EV_SetCharaControllerFlags(WAREHOUSE_ARG(1), 3, 0);
                    FTBL.EV_SetCharaControllerFlags(WAREHOUSE_ARG(2), 3, 0);
                    FTBL.AttachScenarioCharacter(WAREHOUSE_ARG(1), WAREHOUSE_ARG(2), 14, &attachPosition, &attachRotation);
                }
            }
            if (script[2] == 2)
                AdvanceWarehouseScript(script);
            loop = 0;
            break;
        case 0x8002001c:
            script[2]++;
            {
                Vector3 attachPosition = {0.1f, 0.0f, -0.03f};
                Vector3i attachRotation = {0, 0, 0xf1c7};
                if (script[2] == 1) {
                    FTBL.EV_SetCharaControllerFlags(WAREHOUSE_ARG(1), 3, 0);
                    FTBL.EV_SetCharaControllerFlags(WAREHOUSE_ARG(2), 3, 0);
                    FTBL.AttachScenarioCharacter(WAREHOUSE_ARG(1), WAREHOUSE_ARG(2), 14, &attachPosition, &attachRotation);
                }
            }
            if (script[2] == 2)
                AdvanceWarehouseScript(script);
            loop = 0;
            break;
        case 0x8001001d:
            FTBL.ResetScenarioCharacterAttachment(WAREHOUSE_ARG(1));
            AdvanceWarehouseScript(script);
            break;
        case 0x80020016:
            FTBL.EVS_LoopCamera(WAREHOUSE_ARG(2), WAREHOUSE_ARG(1), WAREHOUSE_ARG(1));
            AdvanceWarehouseScript(script);
            break;
        case 0x80020017:
            QueueWarehouseSerif((const char *)WAREHOUSE_ARG(1), WAREHOUSE_ARG(2));
            AdvanceWarehouseScript(script);
            break;
        case 0x80010019:
            FTBL.EV_EventSetUpHelpIcon_0((unsigned char)WAREHOUSE_ARG(1), 0);
            AdvanceWarehouseScript(script);
            break;
        case 0x8001000c:
            script[3] = WAREHOUSE_ARG(1);
            AdvanceWarehouseScript(script);
            break;
        case 0x8002001b:
            FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(WAREHOUSE_ARG(1)), WAREHOUSE_ARG(2), 0);
            AdvanceWarehouseScript(script);
            break;
        case 0x8002001e:
            if (!script[2]) {
                EventDateTime time;
                FTBL.SetRenderClockAndCompassTo(1);
                FTBL.GetCurrentDateTime((DateTime *)&time);
                if (time.hour < 8 || time.hour > 22)
                    FTBL.SetSleepActionIcon(15);
                else if (time.hour > 19)
                    FTBL.SetSleepActionIcon(271);
                else
                    FTBL.SetSleepActionIcon(266);
                FTBL.shenSetTimeSimulationTo(0);
            } else {
                int result = FTBL.GetSleepActionIconResult();
                if (result == 2) {
                    FTBL.SetRenderClockAndCompassTo(0);
                    FTBL.shenSetTimeSimulationTo(1);
                    SeekWarehouseScriptLabel(script, WAREHOUSE_ARG(1));
                    continue;
                }
                if (result == 1) {
                    FTBL.SetRenderClockAndCompassTo(0);
                    FTBL.shenSetTimeSimulationTo(1);
                    SeekWarehouseScriptLabel(script, WAREHOUSE_ARG(2));
                    continue;
                }
            }
            script[2]++;
            loop = 0;
            break;
        case 0x8000001f:
            if (!script[2])
                FTBL.RefreshPadTaskData(0);
            else if (!FTBL.IsItemSelectable())
                AdvanceWarehouseScript(script);
            script[2]++;
            loop = 0;
            break;
        case 0x80000020:
            FTBL.SetPlayerControlEnabled(1);
            AdvanceWarehouseScript(script);
            break;
        case 0x80000025:
            FTBL.camCtrlSetFovTransitionTo(1);
            FTBL.camCtrlSetState(0);
            FTBL.EV_SetCharaControllerFlags(WAREHOUSE_RYO, 1, 1);
            FTBL.EV_SetCharaControllerFlags(WAREHOUSE_RYO, 2, 1);
            FTBL.EV_SetCharaControllerFlags(WAREHOUSE_RYO, 3, 1);
            AdvanceWarehouseScript(script);
            break;
        case 0x80000021:
            FTBL.SetPlayerControlEnabled(0);
            AdvanceWarehouseScript(script);
            break;
        case 0x80010022:
            FTBL.EV_SetCharaControllerFlags(WAREHOUSE_ARG(1), 1, 0);
            FTBL.EV_SetCharaControllerFlags(WAREHOUSE_ARG(1), 2, 0);
            FTBL.EV_SetCharaControllerFlags(WAREHOUSE_ARG(1), 3, 0);
            FTBL.mtwkSetRequestedRootMode(FTBL.scenFindMotionwork(WAREHOUSE_ARG(1)), 0);
            FTBL.SetMotionRegionFlag(FTBL.scenFindMotionwork(WAREHOUSE_ARG(1)), 71, 0, 0);
            FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(WAREHOUSE_ARG(1)), 0xf003, 0);
            AdvanceWarehouseScript(script);
            break;
        case 0x80000023:
            FTBL.SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            AdvanceWarehouseScript(script);
            break;
        case 0x80010024:
            FTBL.npcLockUnlock(WAREHOUSE_ARG(1));
            AdvanceWarehouseScript(script);
            break;
        case 0x80010026:
            if (WAREHOUSE_ARG(1))
                FTBL.ProcessAicaCommand(0xaa0, WAREHOUSE_ARG(1));
            else
                FTBL.ProcessAicaCommand(0x1200a0, 0);
            AdvanceWarehouseScript(script);
            break;
        case 0x80020027:
            FTBL.EV_LoadAudio((const char *)WAREHOUSE_ARG(1), WAREHOUSE_ARG(2));
            AdvanceWarehouseScript(script);
            break;
        case 0x80010028:
            FTBL.EV_LoadCutsceneFromCurrentScene((const char *)WAREHOUSE_ARG(1));
            AdvanceWarehouseScript(script);
            break;
        case 0x80010029:
            if (script[2]) {
                if (script[2] == WAREHOUSE_ARG(1)) {
                    FTBL.ST_LoadTitleFade();
                    AdvanceWarehouseScript(script);
                }
                loop = 0;
            } else {
                FTBL.Queue_ST_LoadTitleFadeIn(0);
                loop = 0;
            }
            script[2]++;
            break;
        case 0x8001002a:
        {
            int alpha = 255 * WAREHOUSE_ARG(1);
            FTBL.SetScreenFilmFade(0, alpha, 0, 0, 0, alpha, 0, 0, 0);
            AdvanceWarehouseScript(script);
            break;
        }
        case 0x8000002b:
            if (FTBL.GetNpcNewLoadState()) {
                FTBL.EV_NPC_NewLoadSw_0(0);
                FTBL.EV_ResetFreeTalkFlags();
                AdvanceWarehouseScript(script);
                break;
            }
            loop = 0;
            break;
        case 0x8001002d:
            FTBL.dbgPrintEventMessage((const char *)WAREHOUSE_ARG(1));
            AdvanceWarehouseScript(script);
            break;
        case 0x8000002e:
            FTBL.ctrlStopSubtitles();
            AdvanceWarehouseScript(script);
            break;
        case 0x8000002f:
            FTBL.CheckHelpIcon(4, 0, 3);
            if (FTBL.EVT_GetControllerInput(0, 1) == 1024) {
                AdvanceWarehouseScript(script);
            } else {
                loop = 0;
            }
            break;
        case 0x80020030:
            FTBL.EV_scenQueueLoad(g_actNumber, WAREHOUSE_ARG(1), WAREHOUSE_ARG(2));
            AdvanceWarehouseScript(script);
            loop = 0;
            break;
        case 0x8000002c:
            AdvanceWarehouseMorning();
            AdvanceWarehouseScript(script);
            break;
        case 0x80010031:
            warehouseDialogue = FTBL.EV_PlayDialogue((const char *)WAREHOUSE_ARG(1));
            AdvanceWarehouseScript(script);
            break;
        case 0x80000032:
            if (!FTBL.cvsHasValidConverseData(warehouseDialogue))
                AdvanceWarehouseScript(script);
            loop = 0;
            break;
        case 0x80030033:
            SeekWarehouseScriptLabel(script, WAREHOUSE_ARG(1 + FTBL.shenGetRandomInt32(3)));
            continue;
        case 0x80010034:
            if (!script[2])
                FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), WAREHOUSE_ARG(1), 0);
            else if (!FTBL.EV_ControlPackageResources(0, 2, 0, 0, 0)) {
                AdvanceWarehouseScript(script);
                break;
            }
            script[2]++;
            loop = 0;
            break;
        case 0x80010035:
            FTBL.EV_ControlPackageResources(0, 6, (const void *)WAREHOUSE_ARG(1), 0, 0);
            AdvanceWarehouseScript(script);
            break;
        case 0x80020036: {
            EventDateTime time;
            FTBL.GetCurrentDateTime((DateTime *)&time);
            time.hour = WAREHOUSE_ARG(1);
            time.minute = WAREHOUSE_ARG(2);
            FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&time);
            AdvanceWarehouseScript(script);
            break;
        }
        default:
            FTBL.DebugLogToFileRemoved(":::::ERROR::::: (et_bunbu.c)\n");
            FTBL.DebugLogToFileRemoved("Data Address: %8X\n", script);
            FTBL.DebugLogToFileRemoved("PC          : %8X\n", script[1]);
            FTBL.EventAssertNullsub(__FILE__, 670);
            break;
        }
    }
}

void AdvanceWarehouseScript(unsigned int *script)
{
    unsigned int count = (script[script[1]] >> 16) & 15;
    script[1] += count + 1;
    script[2] = 0;
}

void SeekWarehouseScriptLabel(unsigned int *script, unsigned short label)
{
    unsigned int cursor = 4;
    for (;;) {
        unsigned int command = script[cursor++];
        switch (command & 0xf0000000) {
        case 0x80000000:
            cursor += (command >> 16) & 15;
            break;
        case 0x40000000:
            if ((unsigned short)command == label) {
                script[1] = cursor;
                script[2] = 0;
                return;
            }
            break;
        case 0:
        default:
            FTBL.DebugLogToFileRemoved(":::::ERROR::::: (et_bunbu.c)\n");
            FTBL.DebugLogToFileRemoved("Not Found Number:%d\n", label);
            FTBL.DebugLogToFileRemoved("Data Address: %8X\n", script);
            FTBL.DebugLogToFileRemoved("Before PC   : %8X\n", script[1]);
            FTBL.EventAssertNullsub(__FILE__, 718);
            break;
        }
    }
}

void PrintWarehouseHex(int column, int line, unsigned int value)
{
    char text[9];
    int i = 8;
    char *p = &text[8];
    *p-- = 0;
    for (; i != 0; i--) {
        *p-- = warehouseDigits[value & 15];
        value >>= 4;
    }
    FTBL.SetDebugScreenTextPosition(column, line);
    FTBL.DebugLogToScreen(text);
}

void PrintWarehouseDecimal(int column, int line, unsigned int value)
{
    char text[9];
    unsigned int divisors[8] = {10000000, 1000000, 100000, 10000, 1000, 100, 10, 1};
    char *p;
    int i, n;
    unsigned int digit;
    text[8] = 0;
    p = text;
    for (n = 8, i = 0; n != 0; n--, i++) {
        digit = value / divisors[i];
        value -= digit * divisors[i];
        *p++ = warehouseDigits[digit];
    }
    FTBL.SetDebugScreenTextPosition(column, line);
    FTBL.DebugLogToScreen(text);
}

void StopWarehouseSerifCallback(void *data)
{
    FTBL.ctrlStopSubtitles();
    FTBL.HLib_DestroyCurrentTask();
}

void QueueWarehouseSerif(const char *text, int frames)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithoutParameter(StopWarehouseSerifCallback, 6, 11, TASK_EVNT);
    FTBL.dbgPrintEventMessage(text);
    FTBL.HLib_SetupTimedTask(task, frames);
}

short GetWarehouseInput(short pad, short button)
{
    return FTBL.EVT_GetControllerInput(pad, warehouseButtons[button]);
}

void AdvanceWarehouseMorning(void)
{
    EventDateTime time;
    FTBL.GetCurrentDateTime((DateTime *)&time);
    time.day++;
    time.hour = 8;
    time.minute = 0;
    FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&time);
    FTBL.EV_stepSetFlags(100, 0);
}

void SetWarehouseNight(void)
{
    EventDateTime time;
    FTBL.GetCurrentDateTime((DateTime *)&time);
    if (time.hour >= 8 && time.hour <= 22) {
        time.hour = 23;
        time.minute = 0;
        FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&time);
    }
}
#endif
