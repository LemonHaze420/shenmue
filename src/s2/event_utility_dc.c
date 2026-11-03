#include "s2/event_utility_dc.h"
#include "s2/event_queued_dc.h"

static int npcLowQualityHands = 1;
static int npcCharactersVisible = 1;
static const shen_name *npcLoadCharacters;
static int npcLoadPhase;
static int npcLoadComplete;
static HLibTask *npcLoadStartupTask;
static EventDateTime utilityDateTime;

HLibTask *Queue_EVT_UTL_LoadNPCChara_3(const shen_name *characters, HLibTask *startupTask)
{
    npcLoadCharacters = characters;
    npcLoadPhase = 0;
    npcLoadComplete = 0;
    npcLoadStartupTask = startupTask;
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    if (startupTask)
        FTBL.HLib_SetupTimedTask(startupTask, 0);
    return FTBL.HLib_EnqueueTaskWithoutParameter(EVT_UTL_LoadNPCChara_Callback, 6, 11, TASK_NPC_LOAD);
}

void EVT_UTL_LoadNPCChara_Callback(void *taskData)
{
    if (npcLoadPhase) {
        if (FTBL.ForceLoadNpc(*npcLoadCharacters)) {
            if (npcCharactersVisible)
                EnableAuthoredCharacterFlagsAndDisplay(*npcLoadCharacters);
            else
                DisableAuthoredCharacterFlagsAndDisplay(*npcLoadCharacters);
            if (npcLowQualityHands)
                FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(*npcLoadCharacters));
            npcLoadCharacters++;
            npcLoadPhase = 0;
            if (*npcLoadCharacters == INVALID_CHARACTER) {
                npcLoadComplete = 1;
                if (npcLoadStartupTask)
                    FTBL.HLib_ResolveDeferredCallback(npcLoadStartupTask);
                FTBL.HLib_DestroyCurrentTask();
            }
        }
    } else {
        FTBL.CheckWarmStartNpcRoute(*npcLoadCharacters);
        FTBL.ForceLoadNpc(*npcLoadCharacters);
        npcLoadPhase = 1;
    }
}

void EVT_UTL_SetNPCCharaVisible(int visible)
{
    npcCharactersVisible = visible;
}

int EVT_UTL_GetNPCCharaVisible(void)
{
    return npcCharactersVisible;
}

void EVT_UTL_SetNPCLowQualityHands(int enabled)
{
    npcLowQualityHands = enabled;
}

int EVT_UTL_GetNPCLowQualityHands(void)
{
    return npcLowQualityHands;
}

void EVT_UTL_FreeNPCChara(const shen_name *characters)
{
    while (*characters != INVALID_CHARACTER) {
        if (FTBL.DoesScenarioCharacterExist(*characters)) {
            FTBL.npcLockUnlock(*characters);
            FTBL.npcFree(*characters);
        }
        characters++;
    }
}

int EVT_UTL_IsNPCCharaLoaded(void)
{
    return npcLoadComplete;
}

void EnableAuthoredCharacterFlagsAndDisplay(shen_name character)
{
    FTBL.EV_EnableCharacterDisplay(character);
    FTBL.EV_EnsureCharacterDraw(character);
    SHENMUE_EVENT_BOUNDARY();
    FTBL.EV_SetCharaControllerFlags(character, 2, 0);
    FTBL.EV_SetCharaControllerFlags(character, 3, 0);
    FTBL.SetRefBoardParamsForCharaById(character, 1);
    SHENMUE_EVENT_BOUNDARY();
    FTBL.EV_SetCharacterAlphaEnabled(character, 0);
}

void DisableAuthoredCharacterFlagsAndDisplay(shen_name character)
{
    FTBL.EV_DisableCharacterDisplay(character);
    FTBL.EV_ToggleCharacterFlag(character);
    SHENMUE_EVENT_BOUNDARY();
    FTBL.EV_SetCharaControllerFlags(character, 2, 0);
    FTBL.EV_SetCharaControllerFlags(character, 3, 0);
    FTBL.SetRefBoardParamsForCharaById(character, 1);
    SHENMUE_EVENT_BOUNDARY();
    FTBL.EV_SetCharacterAlphaEnabled(character, 0);
}

void EVT_UTL_SaveDateTime(void)
{
    FTBL.GetCurrentDateTime((DateTime *)&utilityDateTime);
}

void EVT_UTL_RestoreDateTime(void)
{
    FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&utilityDateTime);
}
