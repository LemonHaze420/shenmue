#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_tejo.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void TejoMain(void)
{
    PrepareTejoEvent();
    tejoMainTask = FTBL.HLib_EnqueueTaskWithoutParameter(TejoProgressionCallback, 4, 11, TEJO_MAIN_TASK);
    Queue_EVT_UTL_LoadNPCChara_3(tejoMainCharacters, tejoMainTask);
}
#else
RenQteVoice tejoLowerLeftCornerInitialVoices[] = {{"0209B051", 65}, {0, 0}};
RenQteVoice tejoLowerLeftCornerSuccessVoices[] = {{"0209B003", 16}, {0, 0}};
RenQteVoice tejoLowerLeftCornerFailureVoices[] = {{"0209A002", 30}, {"0209B004", 65}, {"0209B005", 160}, {0, 0}};
RenQteVoice tejoLowerPitInitialVoices[] = {{"0209B060", 0}, {0, 0}};
RenQteVoice tejoLowerPitSuccessVoices[] = {{"0209A009", 5}, {"0209B032", 40}, {0, 0}};
RenQteVoice tejoLowerPitFailureVoices[] = {{"0209B015", 10}, {"0209A010", 35}, {"0209B016", 100}, {"0209B017", 170}, {"0209B049", 230}, {0, 0}};
RenQteVoice tejoLowerRightCornerInitialVoices[] = {{"0209B062", 20}, {0, 0}};
RenQteVoice tejoLowerRightCornerSuccessVoices[] = {{"0209A003", 19}, {0, 0}};
RenQteVoice tejoLowerRightCornerFailureVoices[] = {{"0209A004", 17}, {"0209B007", 60}, {0, 0}};
RenQteVoice tejoLowerSecondLeftCornerInitialVoices[] = {{"0209B052", 15}, {0, 0}};
RenQteVoice tejoLowerSecondLeftCornerSuccessVoices[] = {{"0209A001", 2}, {"0209B003", 16}, {0, 0}};
RenQteVoice tejoLowerSecondLeftCornerFailureVoices[] = {{"0209A011", 30}, {"0209B020", 160}, {"0209B022", 230}, {0, 0}};
RenQteVoice tejoLowerSecondRightCornerInitialVoices[] = {{"0209B063", 12}, {0, 0}};
RenQteVoice tejoLowerSecondRightCornerSuccessVoices[] = {{"0209A003", 19}, {0, 0}};
RenQteVoice tejoLowerSecondRightCornerFailureVoices[] = {{"0209A004", 20}, {"0209B007", 62}, {0, 0}};
RenQteVoice tejoLowerSecondPitInitialVoices[] = {{"0209B059", 0}, {0, 0}};
RenQteVoice tejoLowerSecondPitSuccessVoices[] = {{"0209A009", 5}, {"0209B009", 40}, {0, 0}};
RenQteVoice tejoLowerSecondPitFailureVoices[] = {{"0209B015", 10}, {"0209A010", 35}, {"0209B016", 100}, {"0209B017", 170}, {"0209B049", 230}, {0, 0}};
RenQteVoice tejoUpperLeftCornerInitialVoices[] = {{"0209B056", 75}, {0, 0}};
RenQteVoice tejoUpperLeftCornerSuccessVoices[] = {{"0209A001", 2}, {"0209B003", 16}, {0, 0}};
RenQteVoice tejoUpperLeftCornerFailureVoices[] = {{"0209A002", 30}, {"0209B004", 65}, {"0209B005", 160}, {0, 0}};
RenQteVoice tejoUpperRightCornerInitialVoices[] = {{"0209B062", 25}, {0, 0}};
RenQteVoice tejoUpperRightCornerSuccessVoices[] = {{"0209A003", 19}, {0, 0}};
RenQteVoice tejoUpperRightCornerFailureVoices[] = {{"0209A004", 20}, {"0209B007", 62}, {0, 0}};
RenQteVoice tejoUpperPoleInitialVoices[] = {{"0209B051", 0}, {0, 0}};
RenQteVoice tejoUpperPoleSuccessVoices[] = {{"0209A005", 3}, {"0209B009", 12}, {0, 0}};
RenQteVoice tejoUpperPoleFailureVoices[] = {{"0209A006", 15}, {"0209B010", 30}, {"0209B011", 80}, {0, 0}};
RenQteVoice tejoUpperPitInitialVoices[] = {{"0209B057", 0}, {0, 0}};
RenQteVoice tejoUpperPitSuccessVoices[] = {{"0209A009", 5}, {"0209B009", 40}, {0, 0}};
RenQteVoice tejoUpperPitFailureVoices[] = {{"0209B015", 10}, {"0209A010", 35}, {"0209B016", 80}, {"0209B017", 140}, {"0209B049", 230}, {0, 0}};
RenQteVoice tejoUpperSecondRightCornerInitialVoices[] = {{"0209B063", 20}, {0, 0}};
RenQteVoice tejoUpperSecondRightCornerSuccessVoices[] = {{"0209A003", 19}, {0, 0}};
RenQteVoice tejoUpperSecondRightCornerFailureVoices[] = {{"0209A004", 17}, {"0209B007", 60}, {0, 0}};
RenQteVoice tejoUpperLeaderSwitchInitialVoices[] = {{"0209B068", 1}, {0, 0}};
RenQteVoice tejoUpperLeaderSwitchSuccessVoices[] = {{"0209B025", 5}, {"0209A012", 80}, {0, 0}};
RenQteVoice tejoUpperLeaderSwitchFailureVoices[] = {{"0209A002", 7}, {"0209B004", 27}, {"0209B011", 58}, {"0209A013", 120}, {"0209B027", 175}, {0, 0}};
RenQteVoice tejoUpperFinalLeftCornerInitialVoices[] = {{"0209B051", 30}, {0, 0}};
RenQteVoice tejoUpperFinalLeftCornerSuccessVoices[] = {{"0209A001", 2}, {"0209B003", 16}, {0, 0}};
RenQteVoice tejoUpperFinalLeftCornerFailureVoices[] = {{"0209A011", 20}, {"0209B020", 160}, {0, 0}};
RenQteVoice tejoUpperDoorInitialVoices[] = {{"0209A014", 0}, {"0209B029", 71}, {"0209B030", 145}, {"0209A008", 220}, {"0209B031", 275}, {0, 0}};
RenQteVoice tejoUpperDoorSuccessVoices[] = {{"0209A015", 6}, {0, 0}};
RenQteVoice tejoDoorSecondRetrySuccessVoices[] = {{"0209A015", 6}, {0, 0}};
RenQteVoice tejoDoorFirstRetryFailureVoices[] = {{"0209A015", 34}, {"0209B033", 70}, {"0209A016", 130}, {"0209B031", 175}, {0, 0}};
RenQteVoice tejoDoorSecondRetryFailureVoices[] = {{"0209A015", 34}, {"0209B034", 70}, {"0209A017", 130}, {"0209B031", 175}, {0, 0}};
RenQteVoice tejoDoorThirdRetryFailureVoices[] = {{"0209A015", 34}, {"0209B020", 70}, {0, 0}};
RenQteCamera tejoEmptyCameras[] = {{0, 0, 0}};
RenQteCamera tejoLowerLeftCornerInitialCameras[] = {{10, REN_QTE_SECOND_BASE, 0}, {0, 0, 0}};
RenQteCamera tejoUpperLeftCornerInitialCameras[] = {{11, REN_QTE_SECOND_BASE, 0}, {0, 0, 0}};
RenQteCamera tejoUpperPitInitialCameras[] = {{12, REN_QTE_SECOND_BASE, 0}, {0, 0, 0}};
RenQteCamera tejoUpperRightCornerInitialCameras[] = {{13, REN_QTE_SECOND_BASE, 0}, {0, 0, 0}};
RenQteCamera tejoUpperPoleInitialCameras[] = {{14, REN_QTE_SECOND_BASE, 0}, {0, 0, 0}};
RenQteCamera tejoLowerRightCornerInitialCameras[] = {{15, REN_QTE_SECOND_BASE, 0}, {0, 0, 0}};
RenQteCamera tejoLowerPitInitialCameras[] = {{16, REN_QTE_SECOND_BASE, 0}, {0, 0, 0}};
RenQteCamera tejoLowerSecondRightCornerInitialCameras[] = {{17, REN_QTE_SECOND_BASE, 0}, {0, 0, 0}};
RenQteCamera tejoLowerSecondLeftCornerInitialCameras[] = {{18, REN_QTE_SECOND_BASE, 0}, {0, 0, 0}};
RenQteCamera tejoUpperSecondRightCornerInitialCameras[] = {{19, REN_QTE_SECOND_BASE, 0}, {0, 0, 0}};
RenQteCamera tejoUpperLeaderSwitchInitialCameras[] = {{20, REN_QTE_SECOND_BASE, 0}, {0, 0, 0}};
RenQteCamera tejoLowerExitInitialCameras[] = {{801, REN_QTE_SECOND_BASE, 0}, {802, REN_QTE_BASE, 41}, {0, 0, 0}};
RenQteCamera tejoUpperDoorInitialCameras[] = {{1700, REN_QTE_BASE, 0}, {1710, REN_QTE_BASE, 105}, {1720, REN_QTE_BASE, 145}, {0, 0, 0}};
RenQteCamera tejoLowerLeftCornerSuccessCameras[] = {{101, REN_QTE_BASE, 0}, {0, 0, 0}};
RenQteCamera tejoLowerPitSuccessCameras[] = {{201, REN_QTE_BASE, 0}, {0, 0, 0}};
RenQteCamera tejoLowerRightCornerSuccessCameras[] = {{301, REN_QTE_BASE, 0}, {0, 0, 0}};
RenQteCamera tejoLowerSecondLeftCornerSuccessCameras[] = {{501, REN_QTE_BASE, 0}, {0, 0, 0}};
RenQteCamera tejoLowerSecondRightCornerSuccessCameras[] = {{601, REN_QTE_BASE, 0}, {0, 0, 0}};
RenQteCamera tejoUpperLeftCornerSuccessCameras[] = {{1101, REN_QTE_BASE, 0}, {0, 0, 0}};
RenQteCamera tejoUpperPoleSuccessCameras[] = {{1301, REN_QTE_BASE, 0}, {0, 0, 0}};
RenQteCamera tejoUpperSecondRightCornerSuccessCameras[] = {{1501, REN_QTE_BASE, 0}, {0, 0, 0}};
RenQteCamera tejoUpperLeaderSwitchSuccessCameras[] = {{1601, REN_QTE_BASE, 0}, {0, 0, 0}};
RenQteCamera tejoUpperFinalLeftCornerSuccessCameras[] = {{1650, REN_QTE_BASE, 0}, {20, REN_QTE_SECOND_BASE, 55}, {0, 0, 0}};
RenQteCamera tejoUpperDoorSuccessCameras[] = {{1810, REN_QTE_BASE, 0}, {1811, REN_QTE_BASE, 16}, {1812, REN_QTE_BASE, 31}, {0, 0, 0}};
RenQteCamera tejoLowerLeftCornerFailureCameras[] = {{102, REN_QTE_BASE, 0}, {103, REN_QTE_BASE, 150}, {0, 0, 0}};
RenQteCamera tejoLowerPitFailureCameras[] = {{202, REN_QTE_BASE, 0}, {203, REN_QTE_BASE, 96}, {0, 0, 0}};
RenQteCamera tejoLowerRightCornerFailureCameras[] = {{302, REN_QTE_BASE, 0}, {0, 0, 0}};
RenQteCamera tejoLowerSecondLeftCornerFailureCameras[] = {{502, REN_QTE_BASE, 0}, {503, REN_QTE_BASE, 160}, {0, 0, 0}};
RenQteCamera tejoLowerSecondRightCornerFailureCameras[] = {{602, REN_QTE_BASE, 0}, {0, 0, 0}};
RenQteCamera tejoUpperLeftCornerFailureCameras[] = {{1102, REN_QTE_BASE, 0}, {0, 0, 0}};
RenQteCamera tejoUpperRightCornerFailureCameras[] = {{1202, REN_QTE_BASE, 0}, {0, 0, 0}};
RenQteCamera tejoUpperPoleFailureCameras[] = {{1302, REN_QTE_BASE, 0}, {1303, REN_QTE_BASE, 71}, {0, 0, 0}};
RenQteCamera tejoUpperSecondRightCornerFailureCameras[] = {{1502, REN_QTE_BASE, 0}, {0, 0, 0}};
RenQteCamera tejoUpperLeaderSwitchFailureCameras[] = {{1602, REN_QTE_BASE, 0}, {1603, REN_QTE_BASE, 61}, {0, 0, 0}};
RenQteCamera tejoUpperFinalLeftCornerFailureCameras[] = {{1651, REN_QTE_BASE, 0}, {1652, REN_QTE_BASE, 141}, {20, REN_QTE_SECOND_BASE, 267}, {0, 0, 0}};
RenQteCamera tejoDoorFirstRetryCameras[] = {{1820, REN_QTE_BASE, 0}, {1821, REN_QTE_BASE, 71}, {1800, REN_QTE_BASE, 140}, {0, 0, 0}};
RenQteCamera tejoDoorSecondRetryCameras[] = {{1830, REN_QTE_BASE, 0}, {1831, REN_QTE_BASE, 71}, {1800, REN_QTE_BASE, 140}, {0, 0, 0}};
RenQteCamera tejoDoorThirdRetryCameras[] = {{1840, REN_QTE_BASE, 0}, {1841, REN_QTE_BASE, 71}, {0, 0, 0}};
RenQteEntry tejoLowerQteEntries[] = {
    {1, 287.0f, 207.0f, 49152, 285.3f, 201.0f, 0, 64, 30.0f,
        tejoLowerLeftCornerInitialCameras, tejoLowerLeftCornerSuccessCameras, tejoLowerLeftCornerFailureCameras,
        tejoLowerLeftCornerInitialVoices, tejoLowerLeftCornerSuccessVoices, tejoLowerLeftCornerFailureVoices, 54, 275},
    {4, 285.3f, 192.5f, 0, 285.3f, 184.0f, 0, 256, 12.3f,
        tejoLowerPitInitialCameras, tejoLowerPitSuccessCameras, tejoLowerPitFailureCameras,
        tejoLowerPitInitialVoices, tejoLowerPitSuccessVoices, tejoLowerPitFailureVoices, 90, 360},
    {2, 285.3f, 176.0f, 0, 291.0f, 178.2f, 49152, 4, 15.0f,
        tejoLowerRightCornerInitialCameras, tejoLowerRightCornerSuccessCameras, tejoLowerRightCornerFailureCameras,
        tejoLowerRightCornerInitialVoices, tejoLowerRightCornerSuccessVoices, tejoLowerRightCornerFailureVoices, 60, 0},
    {1, 301.0f, 155.6f, 0, 293.0f, 158.4f, 16384, 64, 17.5f,
        tejoLowerSecondLeftCornerInitialCameras, tejoLowerSecondLeftCornerSuccessCameras, tejoLowerSecondLeftCornerFailureCameras,
        tejoLowerSecondLeftCornerInitialVoices, tejoLowerSecondLeftCornerSuccessVoices, tejoLowerSecondLeftCornerFailureVoices, 60, 0},
    {2, 285.8f, 158.4f, 16384, 287.1f, 150.0f, 0, 4, 15.5f,
        tejoLowerSecondRightCornerInitialCameras, tejoLowerSecondRightCornerSuccessCameras, tejoLowerSecondRightCornerFailureCameras,
        tejoLowerSecondRightCornerInitialVoices, tejoLowerSecondRightCornerSuccessVoices, tejoLowerSecondRightCornerFailureVoices, 65, 135},
    {4, 287.1f, 145.2f, 0, 287.1f, 134.0f, 0, 256, 13.0f,
        tejoLowerRightCornerInitialCameras, tejoLowerPitSuccessCameras, tejoLowerPitFailureCameras,
        tejoLowerSecondPitInitialVoices, tejoLowerSecondPitSuccessVoices, tejoLowerSecondPitFailureVoices, 0, 0},
    {8, 286.2f, 129.3f, 0, 0.0f, 0.0f, 0, 0, 13.0f,
        tejoLowerExitInitialCameras, tejoEmptyCameras, tejoEmptyCameras,
        0, 0, 0, 0, 0},
    {0, 0.0f, 0.0f, 0, 0.0f, 0.0f, 0, 0, 0.0f,
        tejoEmptyCameras, tejoEmptyCameras, tejoEmptyCameras,
        0, 0, 0, 0, 0}};
RenQteEntry tejoUpperQteEntries[] = {
    {1, 287.1f, 160.0f, 32768, 295.7f, 158.4f, 49152, 64, 30.0f,
        tejoUpperLeftCornerInitialCameras, tejoUpperLeftCornerSuccessCameras, tejoUpperLeftCornerFailureCameras,
        tejoUpperLeftCornerInitialVoices, tejoUpperLeftCornerSuccessVoices, tejoUpperLeftCornerFailureVoices, 54, 250},
    {2, 303.0f, 158.4f, 49152, 301.1f, 172.7f, 32768, 4, 17.5f,
        tejoUpperRightCornerInitialCameras, tejoLowerRightCornerSuccessCameras, tejoUpperRightCornerFailureCameras,
        tejoUpperRightCornerInitialVoices, tejoUpperRightCornerSuccessVoices, tejoUpperRightCornerFailureVoices, 65, 0},
    {5, 291.0f, 178.0f, 16384, 285.3f, 184.0f, 32768, 64, 9.5f,
        tejoUpperPoleInitialCameras, tejoUpperPoleSuccessCameras, tejoUpperPoleFailureCameras,
        tejoUpperPoleInitialVoices, tejoUpperPoleSuccessVoices, tejoUpperPoleFailureVoices, 60, 170},
    {4, 285.3f, 189.8f, 32768, 285.3f, 199.0f, 32768, 256, 13.0f,
        tejoUpperPitInitialCameras, tejoLowerPitSuccessCameras, tejoLowerPitFailureCameras,
        tejoUpperPitInitialVoices, tejoUpperPitSuccessVoices, tejoUpperPitFailureVoices, 90, 360},
    {2, 284.85f, 208.8f, 32768, 276.0f, 207.0f, 16384, 4, 17.5f,
        tejoUpperSecondRightCornerInitialCameras, tejoUpperSecondRightCornerSuccessCameras, tejoUpperSecondRightCornerFailureCameras,
        tejoUpperSecondRightCornerInitialVoices, tejoUpperSecondRightCornerSuccessVoices, tejoUpperSecondRightCornerFailureVoices, 70, 140},
    {6, 271.5f, 207.0f, 16384, 263.5f, 205.3f, 12743, 16, 12.5f,
        tejoUpperLeaderSwitchInitialCameras, tejoUpperLeaderSwitchSuccessCameras, tejoUpperLeaderSwitchFailureCameras,
        tejoUpperLeaderSwitchInitialVoices, tejoUpperLeaderSwitchSuccessVoices, tejoUpperLeaderSwitchFailureVoices, 100, 220},
    {1, 244.7f, 204.0f, 16384, 247.5f, 212.0f, 32768, 64, 17.5f,
        tejoLowerLeftCornerInitialCameras, tejoUpperFinalLeftCornerSuccessCameras, tejoUpperFinalLeftCornerFailureCameras,
        tejoUpperFinalLeftCornerInitialVoices, tejoUpperFinalLeftCornerSuccessVoices, tejoUpperFinalLeftCornerFailureVoices, 0, 286},
    {7, 245.8f, 217.8f, 16384, 247.5f, 217.7f, 32768, 256, 1.4f,
        tejoUpperDoorInitialCameras, tejoUpperDoorSuccessCameras, tejoDoorFirstRetryCameras,
        tejoUpperDoorInitialVoices, tejoUpperDoorSuccessVoices, tejoDoorFirstRetryFailureVoices, 0, 0},
    {0, 0.0f, 0.0f, 0, 0.0f, 0.0f, 0, 0, 0.0f,
        tejoEmptyCameras, tejoEmptyCameras, tejoEmptyCameras,
        0, 0, 0, 0, 0}};
shen_name tejoMainCharacters[] = {REN_QTE_REN, EVENT_CHARACTER_INVALID};
shen_name tejoQteCharacters[] = {REN_QTE_KD7, REN_QTE_KK1, REN_QTE_KT1, REN_QTE_KU7, TEJO_SOY, EVENT_CHARACTER_INVALID};
Vector3 tejoSecondBasePosition = {0.0f, 0.0f, 0.0f};
Vector3i tejoSecondBaseRotation = {0, 0, 0};
#define TEJO_BSS __attribute__((section(".bss")))
HLibTask *tejoMainTask TEJO_BSS;
HLibTask *tejoQteTask TEJO_BSS;
int tejoProgression TEJO_BSS;
int tejoQteIndex TEJO_BSS;
int tejoWaitFlag TEJO_BSS;
int tejoRetryMode TEJO_BSS;
int tejoExtendedRetry TEJO_BSS;
EventDateTime tejoForceCheckpoint TEJO_BSS;
RenQteWork tejoQteWork TEJO_BSS;
RenQteEntry *tejoQteEntries TEJO_BSS;

void TejoProgressionCallback(void *data)
{
    switch (tejoProgression) {
    case TEJO_TRAP:
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"TTRP", 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        FTBL.ExecuteCurrAreaMainFunc("TTRP.EV3", 0);
        tejoProgression = TEJO_RATI;
        tejoWaitFlag = 311;
        FTBL.HLib_SetCurrentTaskTo(TejoWaitEventCallback);
        break;
    case TEJO_RATI:
        FTBL.SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
        FTBL.EV_stepSetFlags(311, 0);
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"RATI", 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        tejoProgression = TEJO_FIRST_QTE;
        tejoWaitFlag = 312;
        tejoQteEntries = tejoLowerQteEntries;
        if (!tejoRetryMode || tejoRetryMode == 3) {
            EventDateTime date;
            FTBL.GetCurrentDateTime((DateTime *)&date);
            if (date.hour <= 22)
                date.hour++;
            else
                date.minute = 45;
            FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&date);
            EVT_UTL_SaveDateTime();
        } else if (tejoRetryMode <= 3)
            EVT_UTL_RestoreDateTime();
        FTBL.qronSetFloor(5);
        FTBL.ExecuteCurrAreaMainFunc("RATI.EV3", (const void *)tejoRetryMode);
        tejoRetryMode = 0;
        FTBL.HLib_SetCurrentTaskTo(TejoWaitEventCallback);
        break;
    case TEJO_FIRST_QTE:
        FTBL.EV_stepSetFlags(312, 0);
        QueueTejoQte();
        tejoProgression = TEJO_FORCE;
        break;
    case TEJO_FORCE:
        FTBL.EV_stepSetFlags(314, 0);
        FTBL.EV_SetPlayerFlag(12, 952, 0);
        FTBL.SetAllMapFieldDepthSlotsEnabled(0);
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"TOPA", 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        FTBL.qronSetFloor(4);
        FTBL.mapSetupLighting(0);
        if (!tejoRetryMode)
            FTBL.GetCurrentDateTime((DateTime *)&tejoForceCheckpoint);
        else
            FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&tejoForceCheckpoint);
        FTBL.ExecuteCurrAreaMainFunc("TOPA.EV3", (const void *)tejoRetryMode);
        tejoProgression = TEJO_SECOND_QTE;
        tejoWaitFlag = 626;
        tejoQteEntries = tejoUpperQteEntries;
        tejoRetryMode = 0;
        FTBL.HLib_SetCurrentTaskTo(TejoWaitEventCallback);
        break;
    case TEJO_SECOND_QTE:
        FTBL.EV_stepSetFlags(626, 0);
        QueueTejoQte();
        tejoProgression = TEJO_HIDE;
        break;
    case TEJO_HIDE: {
        int index;
        FTBL.EV_stepSetFlags(627, 0);
        FTBL.EV_SetPlayerFlag(12, 954, 0);
        FTBL.SetAllMapFieldDepthSlotsEnabled(0);
        for (index = 1; index < 32; index++)
            if (FTBL.GetMagicRoomFlag(index))
                FTBL.EV_SetMapLightingPlacementEnabled(index, 0);
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"HIDE", 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        FTBL.ExecuteCurrAreaMainFunc("HIDE.EV3", 0);
        tejoProgression = TEJO_ROOF;
        tejoWaitFlag = 628;
        FTBL.HLib_SetCurrentTaskTo(TejoWaitEventCallback);
        break;
    }
    case TEJO_ROOF: {
        EventDateTime date;
        FTBL.EV_stepSetFlags(628, 0);
        FTBL.GetCurrentDateTime((DateTime *)&date);
        date.hour = 21;
        date.minute = 0;
        FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&date);
        FTBL.EV_scenQueueLoad(g_actNumber, TEJO_DESTINATION, 1);
        break;
    }
    case TEJO_YUAN:
        if (!FTBL.filmCheckFadeTrackColour()) {
            CleanupTejoEvent();
            FTBL.scenSetTaskFlags(6);
            FTBL.EV_scenQueueLoad(g_actNumber, TEJO_DESTINATION, 30);
        }
        break;
    case TEJO_WAIT:
        break;
    }
}

void TejoWaitEventCallback(void *data)
{
    if (!FTBL.EV_GetShenmueFlag(tejoWaitFlag))
        FTBL.HLib_SetCurrentTaskTo(TejoProgressionCallback);
}

void QueueTejoQte(void)
{
    tejoQteTask = FTBL.HLib_EnqueueTaskWithoutParameter(TejoQteCallback, 5, 11, TEJO_QTE_TASK);
    Queue_EVT_UTL_LoadNPCChara_3(tejoQteCharacters, tejoQteTask);
    FTBL.HLib_SetupTaskDelay(0);
    PrepareTejoQteResources();
}

void PrepareTejoQteResources(void)
{
    Vector3 door, raised;
    FTBL.SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    tejoQteIndex = 0;
    tejoQteWork.lives = 4;
    FTBL.SetAllMapFieldDepthSlotsEnabled(1);
    if (tejoProgression == TEJO_FIRST_QTE) {
        FTBL.qronSetFloor(5);
        EVT_AT_SetPos(REN_QTE_PLAYER, -1, 0.0f, 118.0f, 0.0f, 0);
    } else {
        FTBL.qronSetFloor(4);
        EVT_AT_SetPos(REN_QTE_PLAYER, -1, 0.0f, 114.4f, 0.0f, 0);
    }
    FTBL.EV_FreeAuthorPak(1, "0209");
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"TEJO", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, "TEJO", 0, 0);
    FTBL.EV_LoadCutsceneFromCurrentScene("0209A000");
    FTBL.EV_LoadAudio("0209A000", 1);
    FTBL.Loop3DSoundEffect(2283);
    FTBL.CreateEventCharacter(3, 0, REN_QTE_BASE);
    FTBL.CreateEventCharacter(3, 0, REN_QTE_SECOND_BASE);
    FTBL.AT_SetLocalBaseChara(REN_QTE_SECOND_BASE, REN_QTE_PLAYER);
    FTBL.EV_AT_SetPos(REN_QTE_SECOND_BASE, 0x38000000, &tejoSecondBasePosition);
    FTBL.EV_AT_SetAngY_0(REN_QTE_SECOND_BASE, 0x38000000, &tejoSecondBaseRotation, 0);
    FTBL.charaSetDisplayEnabledFlag(FTBL.ctrlGetCharaByName(REN_QTE_PLAYER), 0);
    FTBL.charaSetDisplayEnabledFlag(FTBL.ctrlGetCharaByName(REN_QTE_REN), 0);
    FTBL.mapSetupLighting(0);
    FTBL.GetCharaPosition(REN_QTE_DOOR, -1, &door, 0x40000000);
    raised = door;
    raised.y = 100.0f;
    FTBL.EV_AT_SetPos(REN_QTE_DOOR, 0x78000000, &raised);
    FTBL.EV_AT_SetPos(REN_QTE_BROKEN_DOOR, 0x78000000, &raised);
    if (tejoProgression == TEJO_FIRST_QTE)
        FTBL.EV_lightLoad(TEJO_LIGHT_FIRST_QTE);
    else
        FTBL.EV_lightLoad(TEJO_LIGHT_SECOND_QTE);
    FTBL.PushGlobalLightingProgram();
    FTBL.EV_AT_SetPos(REN_QTE_DOOR, 0x78000000, &door);
    FTBL.EV_AT_SetPos(REN_QTE_BROKEN_DOOR, 0x78000000, &door);
    FTBL.camCtrlSetState(0);
    FTBL.camCtrlSetFovTransitionTo(1);
    if (tejoProgression == TEJO_FIRST_QTE)
        FTBL.EV_PlayDialogue("0209B046");
    else
        FTBL.EV_PlayDialogue("0209B047");
    EVT_AT_SetSound(2283, 0);
    if (FTBL.GetItemState(6001))
        FTBL.naviSetFlag(1);
}

void TejoQteCallback(void *data)
{
    if (tejoQteEntries[tejoQteIndex].type) {
        if (tejoQteWork.hidePursuers)
            HideTejoPursuers();
        if (tejoQteWork.state == REN_QTE_INIT)
            tejoQteWork.entry = &tejoQteEntries[tejoQteIndex];
        if (tejoQteWork.entry->type == 7 && (tejoQteWork.state == REN_QTE_SUCCESS || tejoQteWork.state == REN_QTE_FAILURE)) {
            switch (tejoQteWork.doorAttempts % 3) {
            case 1:
                tejoQteWork.entry->failureCameras = tejoDoorFirstRetryCameras;
                tejoQteWork.entry->failureVoices = tejoDoorFirstRetryFailureVoices;
                break;
            case 2:
                tejoQteWork.entry->failureCameras = tejoDoorSecondRetryCameras;
                tejoQteWork.entry->successVoices = tejoDoorSecondRetrySuccessVoices;
                tejoQteWork.entry->failureVoices = tejoDoorSecondRetryFailureVoices;
                break;
            default:
                tejoQteWork.entry->failureCameras = tejoDoorThirdRetryCameras;
                tejoQteWork.entry->failureVoices = tejoDoorThirdRetryFailureVoices;
                break;
            }
        }
        if (UpdateRenQteEvent(&tejoQteWork)) {
            tejoQteIndex++;
            if (tejoQteWork.lives <= 0) {
                CleanupTejoQte();
                if (tejoQteWork.retryCount <= 4)
                    tejoQteWork.retryCount++;
                if (tejoProgression == TEJO_HIDE && tejoQteWork.retryCount <= 3) {
                    tejoProgression = TEJO_FORCE;
                    tejoRetryMode = 1;
                } else {
                    if (tejoProgression == TEJO_FORCE) {
                        if (tejoQteWork.retryCount == 1)
                            tejoRetryMode = 1;
                        else
                            tejoRetryMode = 2;
                    } else {
                        if (!tejoExtendedRetry)
                            tejoRetryMode = 3;
                        else
                            tejoRetryMode = 4;
                        tejoExtendedRetry = 1;
                    }
                    tejoProgression = TEJO_RATI;
                }
            }
        }
    } else
        BeginTejoEndAuthor();
}

void BeginTejoEndAuthor(void)
{
    shen_name light, author;
    FTBL.AT_SetLocalBaseChara(REN_QTE_PLAYER, -1);
    FTBL.AT_SetLocalBaseChara(REN_QTE_REN, -1);
    if (tejoProgression == TEJO_FORCE) {
        FTBL.qronSetFloor(4);
        light = TEJO_LIGHT_FIRST_END;
        author = TEJO_FIRST_END_AUTHOR;
    } else {
        Vector3 position = {0.0f, 0.0f, 0.0f};
        FTBL.scenApplyPosOrRotToChildId(REN_QTE_BROKEN_DOOR, 161, &position, 0, 3);
        light = TEJO_LIGHT_SECOND_END;
        author = TEJO_SECOND_END_AUTHOR;
    }
    FTBL.SetMapModelDataDefaultVertexShader();
    FTBL.EV_lightLoad(light);
    FTBL.PushGlobalLightingProgram();
    FTBL.EV_authSceneCtrl(author);
    FTBL.SetScreenFilmFade(20, 255, 0, 0, 0, 0, 0, 0, 0);
    FTBL.mapSetupLighting(0);
    FTBL.EV_LoadAudio("0209A000", 0);
    FTBL.naviSetFlag(0);
    FTBL.HLib_SetCurrentTaskTo(TejoEndAuthorCallback);
}

void TejoEndAuthorCallback(void *data)
{
    if (!FTBL.EV_authSceneCtrl(-1)) {
        FTBL.SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
        CleanupTejoQte();
        tejoQteWork.retryCount = 0;
    }
}

void HideTejoPursuers(void)
{
    tejoQteWork.hidePursuers = 0;
    FTBL.AT_SetLocalBaseChara(REN_QTE_KD7, REN_QTE_BASE);
    FTBL.AT_SetLocalBaseChara(REN_QTE_KK1, REN_QTE_BASE);
    FTBL.AT_SetLocalBaseChara(REN_QTE_KT1, REN_QTE_BASE);
    FTBL.AT_SetLocalBaseChara(REN_QTE_KU7, REN_QTE_BASE);
    FTBL.EV_DisableCharacterDisplay(REN_QTE_KD7);
    FTBL.EV_DisableCharacterDisplay(REN_QTE_KK1);
    FTBL.EV_DisableCharacterDisplay(REN_QTE_KT1);
    FTBL.EV_DisableCharacterDisplay(REN_QTE_KU7);
    FTBL.EV_ToggleCharacterFlag(REN_QTE_KD7);
    FTBL.EV_ToggleCharacterFlag(REN_QTE_KK1);
    FTBL.EV_ToggleCharacterFlag(REN_QTE_KT1);
    FTBL.EV_ToggleCharacterFlag(REN_QTE_KU7);
    FTBL.Loop3DSoundEffect(2285);
    FTBL.SetScreenFilmFade(90, 255, 0, 0, 0, 0, 0, 0, 0);
    EventDebugRegister(TEJO_DEBUG_ID, TejoDebugExitCallback, 0, 0x7fffffff);
    EventDebugCheck(TEJO_DEBUG_ID, 4);
}

void CleanupTejoQte(void)
{
    tejoQteWork.hidePursuers = 1;
    tejoQteWork.state = REN_QTE_INIT;
    FTBL.naviSetFlag(0);
    FTBL.EV_FreeAuthorPak(0, "0209");
    FTBL.EV_ControlPackageResources(0, 6, "TEJO", 0, 0);
    EVT_UTL_FreeNPCChara(tejoQteCharacters);
    FTBL.AT_SetLocalBaseChara(REN_QTE_PLAYER, -1);
    FTBL.AT_SetLocalBaseChara(REN_QTE_REN, -1);
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(REN_QTE_PLAYER), 0xf003, 0);
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(REN_QTE_REN), 0xf003, 0);
    FTBL.camCtrlSetState(0);
    FTBL.camCtrlSetFovTransitionTo(1);
    if (FTBL.DoesScenarioCharacterExist(REN_QTE_BASE))
        FTBL.SetScenarioCharacterKillMode(REN_QTE_BASE, 1);
    if (FTBL.DoesScenarioCharacterExist(REN_QTE_SECOND_BASE))
        FTBL.SetScenarioCharacterKillMode(REN_QTE_SECOND_BASE, 1);
    FTBL.charaSetDisplayEnabledFlag(FTBL.ctrlGetCharaByName(REN_QTE_PLAYER), 1);
    FTBL.charaSetDisplayEnabledFlag(FTBL.ctrlGetCharaByName(REN_QTE_REN), 1);
    FTBL.HLib_ResolveDeferredCallback(tejoMainTask);
    FTBL.EV_HLKillOtherHierarchyTask(tejoQteTask);
    EventDebugUnregister(TEJO_DEBUG_ID);
    tejoQteTask = 0;
}

void PrepareTejoEvent(void)
{
    FTBL.SetPlayerControlEnabled(0);
    FTBL.SetLoadingBusy();
    FTBL.SetRefBoardParamsForCharaById(REN_QTE_PLAYER, 1);
    FTBL.EV_SetCharaControllerFlags(REN_QTE_PLAYER, 2, 0);
    FTBL.EV_SetCharaControllerFlags(REN_QTE_PLAYER, 3, 0);
    SHENMUE_EVENT_BOUNDARY();
    FTBL.EV_SetCharaControllerFlags(REN_QTE_PLAYER, 1, 0);
    FTBL.EV_SetCharaControllerFlags(REN_QTE_PLAYER, 2, 0);
    FTBL.EV_SetCharaControllerFlags(REN_QTE_PLAYER, 3, 0);
    SHENMUE_EVENT_BOUNDARY();
    FTBL.SetPlayerGameFlags(2, 0);
    FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(REN_QTE_PLAYER), 0xf003, 0);
    FTBL.SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    FTBL.qteBattleUtil(0, 0, 0, 0, 0);
    FTBL.qteBattleUtil(1, 0, 0, 0, 0);
    FTBL.qteBattleUtil(2, 0, 0, 0, 0);
    tejoProgression = TEJO_TRAP;
    tejoWaitFlag = 0;
    tejoRetryMode = 0;
    tejoExtendedRetry = 0;
    tejoMainTask = 0;
    tejoQteTask = 0;
    tejoQteWork.base = REN_QTE_BASE;
    tejoQteWork.state = REN_QTE_INIT;
    tejoQteWork.entry = 0;
    tejoQteWork.retryCount = 0;
    tejoQteWork.lives = 0;
    tejoQteWork.doorAttempts = 0;
    tejoQteWork.hidePursuers = 1;
    FTBL.mapSetupLighting(0);
}

void CleanupTejoEvent(void)
{
    FTBL.qteBattleUtil(5, 0, 0, 0, 0);
    FTBL.EV_cuffsSetFlags();
    EVT_UTL_FreeNPCChara(tejoMainCharacters);
    FTBL.SetRefBoardParamsForCharaById(REN_QTE_PLAYER, 0);
    FTBL.camCtrlSetState(0);
    FTBL.camCtrlSetFovTransitionTo(1);
    FTBL.SetPlayerControlEnabled(1);
    FTBL.EV_HLKillOtherHierarchyTask(tejoMainTask);
}

void TejoDebugExitCallback(void *data)
{
    FTBL.AT_SetLocalBaseChara(REN_QTE_PLAYER, -1);
    FTBL.AT_SetLocalBaseChara(REN_QTE_REN, -1);
    CleanupTejoQte();
}
#endif
#else
#include "s2/areas/area_tejo.hpp"
#include "s2/areas/tejo_tables.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"
#include "s2/step_event.hpp"
#include <cmath>
#include <limits>

namespace shenmue::s2 {
HLib::Task *tejoMainTask{};
HLib::Task *tejoQteTask{};
shen_name tejoProgression{};
std::int32_t tejoQteIndex{}, tejoWaitFlag{}, tejoExtendedRetry{};
std::intptr_t tejoRetryMode{};
RenQteWork tejoQteWork{};
RenQteEntry *tejoQteEntries{};
TejoDateCheckpoint tejoForceCheckpoint{};
// @verified - p
void TejoInit() {
    tejoMainTask = nullptr;
    tejoQteTask = nullptr;
    tejoProgression = shen_name{};
    tejoQteIndex = 0;
    tejoWaitFlag = 0;
    tejoRetryMode = 0;
    tejoExtendedRetry = 0;
    tejoForceCheckpoint = {};
    tejoQteWork = {};
    tejoQteEntries = nullptr;
}
// @verified - p
void TejoMain(std::uintptr_t) {
    PrepareTejoEvent();
    tejoMainTask = HLib::EnqueueTaskWithoutParameter(TejoProgressionCallback, 4, 0x0b, static_cast<std::int32_t>(TEJO_MAIN_TASK.name));
    Queue_EVT_UTL_LoadNPCChara(TEJO_MAIN_CHARACTERS, tejoMainTask);
}
// @verified - p
void TejoProgressionCallback(void *) {
    switch (tejoProgression.name) {
    case SHEN_NAME(TRAP):
        EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("TTRP"));
        EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        ExecuteCurrAreaMainFunc("TTRP.EV3", 0);
        tejoProgression = TEJO_RATI;
        tejoWaitFlag = 311;
        HLib::SetCurrentTaskTo(TejoWaitEventCallback);
        return;
    case SHEN_NAME(RATI): {
        UploadBlurStrength(0.9F);
        SetConversationCameraCodesEnabled(0);
        SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
        EV_stepSetFlags(0, 311);
        EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("RATI"));
        EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        tejoProgression = TEJO_FIRST_QTE;
        tejoQteEntries = tejoLowerQteEntries;
        tejoWaitFlag = 312;
        if (!tejoRetryMode || tejoRetryMode == 3) {
            DateTime date;
            shenGetCurrentDateTime(date);
            if (date.hour >= 23)
                date.minute = 45;
            else
                ++date.hour;
            SetCurrentDateTimeTo(date);
            shenGetCurrentDateTime(eventSavedCurrentDateTime);
        } else if (tejoRetryMode < 4)
            SetCurrentDateTimeTo(eventSavedCurrentDateTime);
        SetQronFloor(5);
        ExecuteCurrAreaMainFunc("RATI.EV3", static_cast<std::uintptr_t>(tejoRetryMode));
        tejoRetryMode = 0;
        HLib::SetCurrentTaskTo(TejoWaitEventCallback);
        return;
    }
    case SHEN_NAME(TEJ1):
        UploadBlurStrength(0.9F);
        EV_stepSetFlags(0, 312);
        QueueTejoQte();
        tejoProgression = TEJO_FORCE;
        return;
    case SHEN_NAME(FORC):
        if (tejoRetryMode)
            EV_SetAuthorActiveTo(1);
        UploadBlurStrength(0.9F);
        EV_stepSetFlags(0, 314);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 952, 0);
        SetAllMapFieldDepthSlotsEnabled(0);
        EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("TOPA"));
        EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        SetQronFloor(4);
        SetQronMapLightingEnabled(0);
        if (tejoRetryMode)
            SetCurrentDateTimeTo(tejoForceCheckpoint.date);
        else
            shenGetCurrentDateTime(tejoForceCheckpoint.date);
        ExecuteCurrAreaMainFunc("TOPA.EV3", static_cast<std::uintptr_t>(tejoRetryMode));
        tejoProgression = TEJO_SECOND_QTE;
        tejoQteEntries = tejoUpperQteEntries;
        tejoWaitFlag = 626;
        tejoRetryMode = 0;
        HLib::SetCurrentTaskTo(TejoWaitEventCallback);
        return;
    case SHEN_NAME(TEJ2):
        UploadBlurStrength(0.9F);
        EV_stepSetFlags(0, 626);
        QueueTejoQte();
        tejoProgression = TEJO_HIDE;
        return;
    case SHEN_NAME(HIDE):
        UploadBlurStrength(0.9F);
        EV_stepSetFlags(0, 627);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 954, 0);
        SetAllMapFieldDepthSlotsEnabled(0);
        for (std::int32_t index = 1; index < 32; ++index)
            if (GetMagicRoomFlag(index))
                SetMapLightingPlacementEnabled(0, index);
        EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("HIDE"));
        EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        ExecuteCurrAreaMainFunc("HIDE.EV3", 0);
        tejoProgression = TEJO_ROOF;
        tejoWaitFlag = 628;
        HLib::SetCurrentTaskTo(TejoWaitEventCallback);
        return;
    case SHEN_NAME(ROOF): {
        EV_stepSetFlags(0, 628);
        DateTime date;
        shenGetCurrentDateTime(date);
        date.hour = 21;
        date.minute = 0;
        SetCurrentDateTimeTo(date);
        EV_scenQueueLoad(g_actNumber, TEJO_DESTINATION, 1);
        return;
    }
    case SHEN_NAME(YUAN):
        if (!filmCheckFadeTrackColour()) {
            CleanupTejoEvent();
            EV_SetScenarioTaskFlags(6);
            EV_scenQueueLoad(g_actNumber, TEJO_DESTINATION, 30);
        }
        return;
    default:
        return;
    }
}
// @verified - p
void TejoWaitEventCallback(void *) {
    if (!EV_GetShenmueFlag(tejoWaitFlag))
        HLib::SetCurrentTaskTo(TejoProgressionCallback);
}
// @verified - p
void QueueTejoQte() {
    tejoQteTask = HLib::EnqueueTaskWithoutParameter(TejoQteCallback, 5, 0x0b, static_cast<std::int32_t>(TEJO_QTE_TASK.name));
    Queue_EVT_UTL_LoadNPCChara(TEJO_QTE_CHARACTERS, tejoQteTask);
    HLib::SetupTaskDelay(0);
    PrepareTejoQteResources();
}
// @verified - p
void TejoQteCallback(void *) {
    if (!tejoQteEntries[tejoQteIndex].type) {
        UploadBlurStrength(0.9F);
        BeginTejoEndAuthor();
        return;
    }
    if (tejoQteWork.hidePursuers)
        HideTejoPursuers();
    if (tejoQteWork.state.name == REN_QTE_INIT.name) {
        tejoQteWork.entry = tejoQteEntries + tejoQteIndex;
        if (std::fabs(tejoQteWork.entry->cameraMotionScale) <= std::numeric_limits<float>::min()) {
            UploadBlurStrength(0.9F);
            camCtrlToggleSmoothing(0);
            camCtrlSetYOffset(0);
        } else {
            UploadBlurStrength(0.45F);
            camCtrlToggleSmoothing(1);
            camCtrlSetMotionScaleFactor(tejoQteWork.entry->cameraMotionScale);
            camCtrlSetYOffset(tejoQteWork.entry->cameraYOffset);
        }
        if (tejoProgression.name == TEJO_FORCE.name && tejoQteIndex == 6)
            UploadBlurStrength(0.45F);
    }
    if (tejoQteWork.entry->type == 7 && (tejoQteWork.state.name == REN_QTE_SUCCESS.name || tejoQteWork.state.name == REN_QTE_FAILURE.name)) {
        switch (tejoQteWork.doorAttempts % 3) {
        case 1:
            tejoQteWork.entry->failureCameras = tejoDoorFirstRetryCameras;
            tejoQteWork.entry->failureVoices = tejoDoorFirstRetryFailureVoices;
            break;
        case 2:
            tejoQteWork.entry->failureCameras = tejoDoorSecondRetryCameras;
            tejoQteWork.entry->successVoices = tejoDoorSecondRetrySuccessVoices;
            tejoQteWork.entry->failureVoices = tejoDoorSecondRetryFailureVoices;
            break;
        default:
            tejoQteWork.entry->failureCameras = tejoDoorThirdRetryCameras;
            tejoQteWork.entry->failureVoices = tejoDoorThirdRetryFailureVoices;
            break;
        }
    }
    if (!UpdateRenQteEvent(&tejoQteWork))
        return;
    ++tejoQteIndex;
    if (tejoQteWork.lives > 0)
        return;
    CleanupTejoQte();
    if (tejoQteWork.retryCount < 5)
        ++tejoQteWork.retryCount;
    if (tejoProgression.name == TEJO_HIDE.name) {
        if (tejoQteWork.retryCount <= 3) {
            tejoProgression = TEJO_FORCE;
            tejoRetryMode = 1;
            return;
        }
    } else if (tejoProgression.name == TEJO_FORCE.name) {
        tejoRetryMode = tejoQteWork.retryCount != 1 ? 2 : 1;
        tejoProgression = TEJO_RATI;
        return;
    }
    const bool previousRetry = tejoExtendedRetry != 0;
    tejoExtendedRetry = 1;
    tejoRetryMode = previousRetry ? 4 : 3;
    tejoProgression = TEJO_RATI;
}
// @verified - p
void TejoEndAuthorCallback(void *) {
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
    CleanupTejoQte();
    tejoQteWork.retryCount = 0;
}
const ScriptAreaDefinition tejoScriptDefinition{.areaId = AREA_TEJO, .field04 = 0, .init = TejoInit, .main = TejoMain};
} // namespace shenmue::s2
#endif
