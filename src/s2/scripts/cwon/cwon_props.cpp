#if defined(SHENMUE_EVENT_OVERLAY) && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/scripts/cwon/area_cwon.hpp"

void CwonPlaceScenarioCharacter(shen_name name, float x, float y, float z, float yaw) {
    FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(name), 1);
    FTBL.EV_SetCharaControllerFlags(name, 2, 0);
    FTBL.EV_SetCharaControllerFlags(name, 3, 0);
    CwonShowCharacter(name);
    EVT_AT_SetPos(name, CENTER, x, y, z, 0);
    EVT_AT_SetAngY(name, CENTER, yaw, 0);
}

void CwonGetDodgeCallback(CwonTemporaryWork *work) {
    const shen_name name = SHEN_NAME4('G', 'E', 'T', '_');
    if (!work)
        FTBL.DebugLogToFileRemoved("WkTask Error\n");
    else {
        switch (work->ticks) {
        case 0:
            if (!FTBL.DoesScenarioCharacterExist(name))
                return;
            work->ticks++;
        case 1:
            FTBL.EV_SetCharacterAlphaEnabled(name, 0);
            CwonPlaceScenarioCharacter(name, 256.6f, 100.9f, 383.4f, 0);
            EVT_AT_SetVoice("0011E002", 0);
            FTBL.mtwkSetUnknownField(FTBL.scenFindMotionwork(name), 0);
            EVT_AT_SetMot(name, 0x1039, -1, 1, 0, 0);
            CwonQueueLook(name, RYO, 5, 0, 1, 3, 60, 0);
            CwonShowCharacter(name);
            CwonDestroyTemporary(work->root, work->slot);
            break;
        }
    }
}

void CwonMarketPropsCallback(CwonTemporaryWork *work) {
    if (!work)
        FTBL.DebugLogToFileRemoved("WkTask Error\n");
    else {
        CwonPlaceScenarioCharacter(SHEN_NAME4('D', 'Y', 'U', '_'), 260.1f, 101, 393.7f, 240);
        EVT_AT_SetMot(SHEN_NAME4('D', 'Y', 'U', '_'), 0x1006, -1, 1, 0, 0);
        CwonShowCharacter(SHEN_NAME4('D', 'Y', 'U', '_'));
        CwonPlaceScenarioCharacter(SHEN_NAME4('M', 'E', 'Y', '_'), 263.6f, 100.9f, 402.5f, 270);
        EVT_AT_SetMot(SHEN_NAME4('M', 'E', 'Y', '_'), 0x100d, -1, 0, 0, 0);
        CwonShowCharacter(SHEN_NAME4('M', 'E', 'Y', '_'));
        CwonPlaceScenarioCharacter(SHEN_NAME4('S', 'E', 'E', '_'), 263.4500122f, 101, 399.1f, 135);
        EVT_AT_SetMot(SHEN_NAME4('S', 'E', 'E', '_'), 0x1006, -1, 1, 0, 0);
        CwonShowCharacter(SHEN_NAME4('S', 'E', 'E', '_'));
        CwonPlaceScenarioCharacter(SHEN_NAME4('E', 'E', 'N', '_'), 261.25f, 101, 399.5f, 256);
        EVT_AT_SetMot(SHEN_NAME4('E', 'E', 'N', '_'), 0x1006, -1, 1, 0, 0);
        CwonShowCharacter(SHEN_NAME4('E', 'E', 'N', '_'));
        CwonRestoreMarketProps(0);
        CwonShowCharacter(SHEN_NAME4('T', 'B', 'L', 'e'));
        CwonDestroyTemporary(work->root, work->slot);
    }
}

void CwonRestoreMarketProps(int showTables) {
    static float CWON_CHAIR_YAWS[] = {40.0f, 310.0f, 100.0f, 20.0f, 45.0f, 250.0f, 45.0f, 307.0f, 51.0f, 32.0f, 314.0f, 298.0f, 67.0f, 49.0f, 105.0f, 221.0f,
        41.0f, 180.0f, 41.0f, 26.0f, 185.0f, 302.0f, 90.0f};
    static float CWON_TABLE_YAWS[] = {180.0f, 180.0f, 180.0f, 0.0f, 0.0f};
    static Vector3 CWON_CHAIR_POSITIONS[] = {{257.100006f, 101.0f, 394.0f}, {256.5f, 101.0f, 395.399994f}, {257.299988f, 101.0f, 395.799988f},
        {258.200012f, 101.0f, 395.100006f}, {258.399994f, 101.0f, 397.600006f}, {256.5f, 101.0f, 397.899994f}, {257.350006f, 101.0f, 399.420013f},
        {258.299988f, 101.0f, 399.049988f}, {257.700012f, 101.0f, 401.0f}, {257.0f, 101.0f, 402.0f}, {257.799988f, 101.0f, 402.600006f},
        {258.100006f, 101.0f, 401.799988f}, {257.600006f, 101.0f, 405.299988f}, {259.200012f, 101.0f, 405.100006f}, {260.899994f, 101.0f, 405.299988f},
        {262.600006f, 101.0f, 400.299988f}, {261.200012f, 101.0f, 399.5f}, {262.399994f, 101.0f, 398.600006f}, {263.299988f, 101.0f, 399.200012f},
        {261.5f, 101.0f, 396.100006f}, {262.399994f, 101.0f, 395.299988f}, {261.5f, 101.0f, 393.899994f}, {260.299988f, 101.0f, 395.0f}};
    static Vector3 CWON_TABLE_POSITIONS[] = {{257.100006f, 101.0f, 395.100006f}, {257.399994f, 101.0f, 398.299988f}, {257.700012f, 101.0f, 402.0f},
        {260.799988f, 101.0f, 394.5f}, {262.299988f, 101.0f, 399.5f}};

    int i;
    shen_name name;
    for (i = 1; i < 24; i++) {
        if (i < 10)
            name = 0x5f5349 | ((i + 48) << 24);
        else
            name = ((i % 10 + 48) << 24) | (((i / 10 + 48) & 255) << 16) | 0x5349;
        CwonEnablePositionedCharacter(name, CWON_CHAIR_POSITIONS[i].x, CWON_CHAIR_POSITIONS[i].y, CWON_CHAIR_POSITIONS[i].z, 0, CWON_CHAIR_YAWS[i], 0);
        CwonShowCharacter(name);
    }
    for (i = 0; i < 5; i++) {
        name = 0x4c4254 | ((i + 97) << 24);
        CwonEnablePositionedCharacter(name, CWON_TABLE_POSITIONS[i].x, CWON_TABLE_POSITIONS[i].y, CWON_TABLE_POSITIONS[i].z, 0, CWON_TABLE_YAWS[i], 0);
        FTBL.DebugLogToFileRemoved("%x\n", name);
        if (showTables)
            CwonShowCharacter(name);
    }
    if (g_currentMap == SHEN_NAME4('A', 'R', '0', '2'))
        EVT_AT_SetPos(SHEN_NAME4('T', 'B', 'L', 'd'), -1, 261.5f, 101, 395, 0);
}

void CwonPrepareTableProps(void) {
    EVT_AT_SetPos(SHEN_NAME4('H', 'A', 'K', '0'), CENTER, 265.5f, 100.9f, 408, 0);
    EVT_AT_SetAngY(SHEN_NAME4('H', 'A', 'K', '0'), CENTER, 40, 0);
    CwonShowCharacter(SHEN_NAME4('H', 'A', 'K', '0'));
    EVT_AT_SetPos(SHEN_NAME4('H', 'A', 'K', '1'), CENTER, 265, 100.98f, 408, 0);
    EVT_AT_SetAngY(SHEN_NAME4('H', 'A', 'K', '1'), CENTER, 190, 0);
    CwonShowCharacter(SHEN_NAME4('H', 'A', 'K', '1'));
}

void CwonWarehousePropsCallback(CwonTemporaryWork *work) {
    if (!work)
        FTBL.DebugLogToFileRemoved("WkTask Error\n");
    else {
        CwonEnablePositionedCharacter(SHEN_NAME4('K', 'Y', 'A', 'T'), 266.3f, 100.9f, 419, 0, 10, 0);
        FTBL.SetScenarioCharacterRadius(SHEN_NAME4('K', 'A', 'N', 'B'), 20);
        CwonEnablePositionedCharacter(SHEN_NAME4('K', 'A', 'N', 'B'), 267, 103.6f, 421.2f, 0, 0, 0);
        CwonPlaceScenarioCharacter(SHEN_NAME4('A', 'N', 'A', '_'), 266.3f, 102.4f, 419, 270);
        EVT_AT_SetMot(SHEN_NAME4('A', 'N', 'A', '_'), 0x103b, -1, 1, 0, 0);
        CwonDisableGravity(SHEN_NAME4('A', 'N', 'A', '_'));
        CwonEnablePositionedCharacter(SHEN_NAME4('D', 'D', 'O', 'R'), 260.47f, 100.9f, 421.1700134f, 0, 180, 0);
        CwonPlaceScenarioCharacter(SHEN_NAME4('K', 'J', 'N', '_'), 259.57f, 100.9f, 421.1700134f, 0);
        CwonHideCharacter(SHEN_NAME4('K', 'J', 'N', '_'));
        CwonPlaceScenarioCharacter(SHEN_NAME4('S', 'H', 'I', '_'), 254, 100, 423.5f, 200);
        EVT_AT_SetMot(SHEN_NAME4('S', 'H', 'I', '_'), 0x103e, -1, 1, 0, 0);
        CwonShowCharacter(SHEN_NAME4('S', 'H', 'I', '_'));
        CwonPlaceScenarioCharacter(SHEN_NAME4('E', 'I', 'N', '_'), 255, 100, 424.9f, 45);
        EVT_AT_SetMot(SHEN_NAME4('E', 'I', 'N', '_'), 0x1007, -1, 1, 0, 0);
        CwonShowCharacter(SHEN_NAME4('E', 'I', 'N', '_'));
        CwonEnablePositionedCharacter(SHEN_NAME4('G', 'O', 'M', 'a'), 255.8f, 100, 425, 0, 0, 0);
        CwonDestroyTemporary(work->root, work->slot);
    }
}

HLibTask *CwonQueuePipeDynamics(void) {
    return Queue_EVT_AT_PlayDynamix(SHEN_NAME4('P', 'I', 'P', '1'), "PIPE", 2, 3, 12, 0, 2, 0);
}

void CwonFallingWorkerCallback(CwonTemporaryWork *work) {
    Vector3 position;
    HLibTask *task;
    const shen_name name = SHEN_NAME4('A', 'N', 'A', '_');
    if (!work)
        FTBL.DebugLogToFileRemoved("WkTask Error\n");
    else
        switch (work->ticks) {
        case 0:
            if (!FTBL.DoesScenarioCharacterExist(name))
                return;
            work->ticks++;
        case 1:
            work->ticks++;
            CwonGetLocalTransform(name, &position, 0);
            EVT_AT_SetPos(name, CENTER, position.x, 100.9f, position.z, 0);
            EVT_AT_SetAngY(name, CENTER, 270, 0);
            FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(name), 0);
            EVT_AT_SetSound(669, 0);
            EVT_AT_SetSound(670, 20);
            EVT_AT_SetMot(name, 0x1055, 20, 1, 0x40000000, 0);
            task = CwonGetTemporaryTask(work);
            if (task)
                CwonQueueMotionWait(task, name, 170);
            else
                FTBL.DebugLogToFileRemoved("Taskid Error\n");
            break;
        case 2:
            work->ticks++;
            task = CwonGetTemporaryTask(work);
            if (task)
                CwonQueueMotionWait(task, name, 0);
            else
                FTBL.DebugLogToFileRemoved("Taskid Error\n");
            break;
        case 3:
            break;
        case 4:
            CwonDestroyTemporary(work->root, work->slot);
            break;
        }
}

void CwonFallingSignCallback(CwonTemporaryWork *work) {
    Vector3i rotation;
    Vector3 position;
    unsigned rotationFlags, positionFlags;
    const shen_name name = SHEN_NAME4('K', 'Y', 'A', 'T');
    if (!work)
        FTBL.DebugLogToFileRemoved("WkTask Error\n");
    else {
        if (work->ticks == 0) {
            rotationFlags = 0;
            SHENMUE_EVENT_CLOBBER("r11");
            positionFlags = 0;
            CwonGetLocalTransform(name, &position, &rotation);
            if (position.x > 265.6999817f)
                positionFlags |= 8;
            if (position.y < 101.1f)
                positionFlags |= 16;
            position.x = -.03f;
            position.y = .05f;
            if (rotation.z < 0x4000)
                rotationFlags |= 32;
            if (rotation.y > 0)
                rotationFlags |= 16;
            rotation.y = -182;
            rotation.z = 910;
            if (positionFlags || rotationFlags) {
                CwonSetPositionMasked(name, positionFlags, &position);
                CwonSetRotationMasked(name, rotationFlags, &rotation);
            } else
                CwonDestroyTemporary(work->root, work->slot);
        }
    }
}

void CwonOpeningDoorCallback(CwonTemporaryWork *work) {
    Vector3i rotation;
    HLibTask *task;
    const shen_name name = SHEN_NAME4('K', 'J', 'N', '_');
    if (!work)
        FTBL.DebugLogToFileRemoved("WkTask Error\n");
    else {
        switch (work->ticks) {
        case 0:
            if (!FTBL.DoesScenarioCharacterExist(name))
                return;
            work->ticks++;
            EVT_AT_SetSound(671, 0);
            CwonQueueLook(name, RYO, 5, 0, 1, 3, 50, 30);
        case 1:
            CwonAddYaw(SHEN_NAME4('D', 'D', 'O', 'R'), -910);
            CwonGetLocalTransform(SHEN_NAME4('D', 'D', 'O', 'R'), 0, &rotation);
            if (rotation.y < 0x3c71) {
                CwonShowCharacter(name);
                EVT_AT_SetMot(name, 0x1038, -1, .8f, 0, 0);
                task = CwonGetTemporaryTask(work);
                if (task)
                    CwonQueueMotionWait(task, RYO, 0);
                else
                    FTBL.DebugLogToFileRemoved("Taskid Error\n");
                FTBL.mtwkSetUnknownField(FTBL.scenFindMotionwork(name), 0);
                EVT_AT_SetMot(name, 0x103a, -1, 1, 0x20000000, 0);
                CwonDestroyTemporary(work->root, work->slot);
            }
            break;
        }
    }
}

void CwonTurningPedestrianCallback(CwonTemporaryWork *work) {
    Vector3 position;
    const shen_name name = SHEN_NAME4('K', 'L', 'N', '_');
    if (!work)
        FTBL.DebugLogToFileRemoved("WkTask Error\n");
    else {
        switch (work->ticks) {
        case 0:
            if (!FTBL.DoesScenarioCharacterExist(name))
                return;
            work->ticks++;
            CwonShowCharacter(name);
            CwonDisableGravity(name);
            CwonPlaceScenarioCharacter(name, 253.3f, 100.9f, 422.5f, 350);
            EVT_AT_SetMot(name, 0x1038, -1, 1, 0, 0);
            CwonGetLocalTransform(WON, &position, 0);
        case 1:
            if (!FTBL.DoesScenarioCharacterExist(name) || CwonTurnTowardYaw(name, 0xc000, 182, 1))
                CwonDestroyTemporary(work->root, work->slot);
            break;
        }
    }
}

void CwonAttachBagPose1(shen_name character) {
    Vector3 position = {.218f, -0.f, -.025f};
    Vector3i rotation = {0x4d24, 0xd213, 0xa05b};
    shen_name bag = SHEN_NAME4('S', 'A', 'L', 'A');
    CwonShowCharacter(bag);
    CwonDisableGravity(bag);
    FTBL.AttachScenarioCharacter(bag, character, 14, &position, &rotation);
}

void CwonAttachBagPose2(shen_name character) {
    Vector3 position = {.134f, -.12f, .019f};
    Vector3i rotation = {0x41c3, 0x6f32, 0xcb05};
    shen_name bag = SHEN_NAME4('S', 'A', 'L', 'A');
    CwonShowCharacter(bag);
    CwonDisableGravity(bag);
    FTBL.AttachScenarioCharacter(bag, character, 14, &position, &rotation);
}

void CwonAttachBagPose3(shen_name character) {
    Vector3 position = {.095f, .048f, -.11f};
    Vector3i rotation = {0xf0b6, 0xae76, 0xc71};
    shen_name bag = SHEN_NAME4('S', 'A', 'L', 'A');
    CwonShowCharacter(bag);
    CwonDisableGravity(bag);
    FTBL.AttachScenarioCharacter(bag, character, 14, &position, &rotation);
}

void CwonAttachBagPose4(shen_name character) {
    Vector3 position = {.134f, .083f, -.041f};
    Vector3i rotation = {0x2a4c, 0xe8d4, 0xbc16};
    shen_name bag = SHEN_NAME4('S', 'A', 'L', 'A');
    CwonShowCharacter(bag);
    CwonDisableGravity(bag);
    FTBL.AttachScenarioCharacter(bag, character, 14, &position, &rotation);
}

void CwonPrepareChairProps(void) {
    EVT_AT_SetPos(SHEN_NAME4('I', 'S', '2', '3'), CENTER, 260.1f, 101, 393.7f, 0);
}

void CwonSetCollisionTables(void) {
    EVT_AT_SetPos(SHEN_NAME4('T', 'B', 'L', 'a'), CENTER, 260.1205139f, 100.9890976f, 401.5202026f, 0);
    EVT_AT_SetPos(SHEN_NAME4('T', 'B', 'L', 'b'), CENTER, 259.2f, 101, 398.6f, 0);
    EVT_AT_SetPos(SHEN_NAME4('T', 'B', 'L', 'e'), CENTER, 263.0960999f, 100.9890976f, 401.5719910f, 0);
    EVT_AT_SetPos(SHEN_NAME4('I', 'S', '1', '9'), CENTER, 264.0480042f, 100.9990005f, 401.5841064f, 0);
    EVT_AT_SetPos(SHEN_NAME4('I', 'S', '1', '7'), CENTER, 262.0747986f, 100.9990005f, 401.2644043f, 0);
    EVT_AT_SetPos(SHEN_NAME4('I', 'S', '_', '5'), CENTER, 260.0451050f, 100.9990005f, 400.3627014f, 0);
    EVT_AT_SetPos(SHEN_NAME4('I', 'S', '_', '9'), CENTER, 258.8f, 100.9990005f, 397.7f, 0);
    EVT_AT_SetPos(SHEN_NAME4('I', 'S', '1', '0'), CENTER, 259.8f, 100.9990005f, 399.2f, 0);
    EVT_AT_SetPos(SHEN_NAME4('I', 'S', '1', '1'), CENTER, 258.9f, 100.9990005f, 399.3f, 0);
    EVT_AT_SetAngY(SHEN_NAME4('T', 'B', 'L', 'a'), CENTER, 90, 0);
}

void CwonRestoreChair17(void) {
    EVT_AT_SetPos(SHEN_NAME4('I', 'S', '1', '7'), CENTER, 262.0747986f, 100.9990005f, 401.2644043f, 0);
}

void CwonSetFallenTable(void) {
    EVT_AT_SetPos(SHEN_NAME4('I', 'S', '1', '7'), CENTER, 262.0440979f, 100.9990005f, 401.6568909f, 0);
    EVT_AT_SetPos(SHEN_NAME4('I', 'S', '_', '5'), CENTER, 263.9382935f, 100.9990005f, 401.2322083f, 0);
    EVT_AT_SetPos(SHEN_NAME4('T', 'B', 'L', 'e'), CENTER, 262.9399109f, 100.9890976f, 401.9465942f, 0);
    EVT_AT_SetAngY(SHEN_NAME4('T', 'B', 'L', 'e'), CENTER, 0, 0);
}

#else
#include "s2/scripts/cwon/cwon_helpers.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/debug.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/sound_program.hpp"
namespace shenmue::s2 {
namespace {
constexpr shen_name get{SHEN_NAME(GET_)}, dyu{SHEN_NAME(DYU_)}, see{SHEN_NAME(SEE_)}, een{SHEN_NAME(EEN_)}, mey{SHEN_NAME(MEY_)}, ana{SHEN_NAME(ANA_)},
    kjn{SHEN_NAME(KJN_)}, shi{SHEN_NAME(SHI_)}, ein{SHEN_NAME(EIN_)}, kln{SHEN_NAME(KLN_)}, sign{SHEN_NAME(KYAT)};
constexpr Vector3 chairPositions[]{{257.1f, 101, 394}, {256.5f, 101, 395.4f}, {257.3f, 101, 395.8f}, {258.2f, 101, 395.1f}, {258.4f, 101, 397.6f},
    {256.5f, 101, 397.9f}, {257.35f, 101, 399.42f}, {258.3f, 101, 399.05f}, {257.7f, 101, 401}, {257, 101, 402}, {257.8f, 101, 402.6f}, {258.1f, 101, 401.8f},
    {257.6f, 101, 405.3f}, {259.2f, 101, 405.1f}, {260.9f, 101, 405.3f}, {262.6f, 101, 400.3f}, {261.2f, 101, 399.5f}, {262.4f, 101, 398.6f},
    {263.3f, 101, 399.2f}, {261.5f, 101, 396.1f}, {262.4f, 101, 395.3f}, {261.5f, 101, 393.9f}, {260.3f, 101, 395}};
constexpr float chairYaws[]{40, 310, 100, 20, 45, 250, 45, 307, 51, 32, 314, 298, 67, 49, 105, 221, 41, 180, 41, 26, 185, 302, 90};
constexpr shen_name chairNames[]{SHEN_NAME(IS_1), SHEN_NAME(IS_2), SHEN_NAME(IS_3), SHEN_NAME(IS_4), SHEN_NAME(IS_5), SHEN_NAME(IS_6), SHEN_NAME(IS_7),
    SHEN_NAME(IS_8), SHEN_NAME(IS_9), SHEN_NAME(IS10), SHEN_NAME(IS11), SHEN_NAME(IS12), SHEN_NAME(IS13), SHEN_NAME(IS14), SHEN_NAME(IS15), SHEN_NAME(IS16),
    SHEN_NAME(IS17), SHEN_NAME(IS18), SHEN_NAME(IS19), SHEN_NAME(IS20), SHEN_NAME(IS21), SHEN_NAME(IS22), SHEN_NAME(IS23)};
constexpr shen_name tableNames[]{SHEN_NAME(TBLa), SHEN_NAME(TBLb), SHEN_NAME(TBLc), SHEN_NAME(TBLd), SHEN_NAME(TBLe)};
constexpr Vector3 tablePositions[]{{257.1f, 101, 395.1f}, {257.4f, 101, 398.3f}, {257.7f, 101, 402}, {260.8f, 101, 394.5f}, {262.3f, 101, 399.5f}};
constexpr float tableYaws[]{35, 108, 270, 305, 0};
void SetPosition(shen_name name, shen_name parent, const Vector3 &value) {
    EventSetPosition request{0, name, parent, value, 0};
    EVT_AT_SetPosRot(&request);
}
void SetYaw(shen_name name, shen_name parent, float value) {
    EventSetAngle request{0, name, parent, value, 0};
    EVT_AT_SetAngY_Callback(&request);
}
void SetMotion(shen_name name, std::uint32_t animation, float frame, float speed, std::uint32_t flags) {
    EventSetMotion request{0, name, animation, frame, speed, flags, 0};
    EVT_AT_SetMot_Callback(&request);
    GetMotionFrameCount(static_cast<std::uint16_t>(animation));
}
void Show(shen_name name) {
    EV_EnsureCharacterDraw(name);
    EV_EnableCharacterDisplay(name);
}
CwonTemporaryWork *RequireWork(void *data, std::int32_t line) {
    auto *work = static_cast<CwonTemporaryWork *>(data);
    if (!work)
        EV_debug_log("CWON\\eCWON_et_ChaseWon_jama.c", line, "WkTask Error\n");
    return work;
}
} // namespace
std::uint32_t CwonFinishEscape(CwonEventWork *work) {
    if (work->escapeTask) {
        HLib::DestroyTaskInternal(work->escapeTask);
        work->escapeTask = nullptr;
    }
    SetPosition(CWON_WON, CWON_CENTER, {233.2f, 99.099998f, 460.10001f});
    SetYaw(CWON_WON, CWON_CENTER, 0);
    SetMotion(CWON_WON, 4178, -1, .5f, 0);
    return GetMotionFrameCount(4178);
}
void CwonEnablePositionedCharacter(shen_name name, const Vector3 &position, float yaw) {
    mtwkSetEnabled(scenFindMotionwork(name), 1);
    EV_SetCharaControllerFlags(name, 2, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Disabled);
    Show(name);
    SetPosition(name, CWON_CENTER, position);
    SetYaw(name, CWON_CENTER, yaw);
}
void CwonGetDodgeCallback(void *data) {
    auto *work = RequireWork(data, 139);
    if (!work)
        return;
    if (work->ticks == 0) {
        if (!DoesScenarioCharacterExist(get))
            return;
        ++work->ticks;
    } else if (work->ticks != 1)
        return;
    EV_SetCharacterAlphaEnabled(get, false);
    CwonEnablePositionedCharacter(get, {256.60001f, 100.9f, 383.39999f}, 0);
    EV_PlayDialogue("0011E002");
    mtwkSetEnvironmentEnabled(scenFindMotionwork(get), 0);
    SetMotion(get, 4153, -1, 1, 0);
    CwonQueueLook(get, CWON_RYO, 0, 0, 0, 3, 60, 0);
    Show(get);
    CwonDestroyTemporary(work->root, static_cast<std::int32_t>(work->slot));
}
void CwonMarketPropsCallback(void *data) {
    auto *work = RequireWork(data, 165);
    if (!work)
        return;
    CwonRestoreMarketProps(0);
    Show(SHEN_NAME(TBLe));
    if (DoesScenarioCharacterExist(dyu)) {
        CwonEnablePositionedCharacter(dyu, {260.79999f, 101, 393.60001f}, 290);
        SetMotion(dyu, 4101, 0, 1, 0);
        CwonSetPositionAnglesDegrees(SHEN_NAME(IS23), {260, 101, 394}, {});
        Show(dyu);
    }
    if (DoesScenarioCharacterExist(see)) {
        CwonEnablePositionedCharacter(see, {263.45001f, 101, 399.10001f}, 135);
        SetMotion(see, 4102, -1, 1, 0);
        Show(see);
    }
    if (DoesScenarioCharacterExist(een)) {
        CwonEnablePositionedCharacter(een, {261.25f, 101, 399.5f}, 256);
        SetMotion(een, 4102, -1, 1, 0);
        Show(een);
    }
    if (DoesScenarioCharacterExist(mey)) {
        CwonEnablePositionedCharacter(mey, {263.60001f, 100.9f, 402.5f}, 270);
        SetMotion(mey, 4109, -1, 0, 0);
        Show(mey);
        CwonDestroyTemporary(work->root, static_cast<std::int32_t>(work->slot));
    }
}
void CwonRestoreMarketProps(std::int32_t mode) {
    for (std::int32_t i = 0; i < 23; ++i)
        if (i < 12 || i > 14) {
            CwonSetPositionAnglesDegrees(chairNames[i], chairPositions[i], {0, chairYaws[i], 0});
            Show(chairNames[i]);
        }
    if (mode != 2) {
        for (std::int32_t i = 0; i < 5; ++i) {
            CwonSetPositionAnglesDegrees(tableNames[i], tablePositions[i], {0, tableYaws[i], 0});
            if (mode)
                Show(tableNames[i]);
        }
        if (g_currentMap.name == SHEN_NAME(AR02))
            SetPosition(SHEN_NAME(TBLd), CWON_INVALID, {261.5f, 101, 395});
    }
}
void CwonRestoreBoxes() {
    SetPosition(SHEN_NAME(HAK0), CWON_CENTER, {265.5f, 100.9f, 408});
    SetYaw(SHEN_NAME(HAK0), CWON_CENTER, 40);
    Show(SHEN_NAME(HAK0));
    SetPosition(SHEN_NAME(HAK1), CWON_CENTER, {265, 100.98f, 408});
    SetYaw(SHEN_NAME(HAK1), CWON_CENTER, 190);
    Show(SHEN_NAME(HAK1));
}
void CwonWarehousePropsCallback(void *data) {
    auto *work = RequireWork(data, 274);
    if (!work)
        return;
    CwonSetPositionAnglesDegrees(sign, {266.29999f, 100.9f, 419}, {0, 10, 0});
    SetScenarioCharacterRadius(SHEN_NAME(KANB), 20);
    CwonSetPositionAnglesDegrees(SHEN_NAME(KANB), {267, 103.6f, 421.20001f}, {});
    CwonEnablePositionedCharacter(ana, {266.29999f, 102.4f, 419}, 270);
    SetMotion(ana, 4155, -1, 1, 0);
    CwonDisableGravity(ana);
    CwonSetPositionAnglesDegrees(SHEN_NAME(DDOR), {260.47f, 100.9f, 421.17001f}, {0, 180, 0});
    CwonEnablePositionedCharacter(kjn, {259.57001f, 100.9f, 421.17001f}, 0);
    EV_ToggleCharacterFlag(kjn);
    EV_DisableCharacterDisplay(kjn);
    CwonEnablePositionedCharacter(shi, {254, 100, 423.5f}, 200);
    SetMotion(shi, 4158, -1, 1, 0);
    Show(shi);
    CwonEnablePositionedCharacter(ein, {255, 100, 424.89999f}, 45);
    SetMotion(ein, 4103, -1, 1, 0);
    Show(ein);
    CwonSetPositionAnglesDegrees(SHEN_NAME(GOMa), {255.8f, 100, 425}, {});
    CwonDestroyTemporary(work->root, static_cast<std::int32_t>(work->slot));
}
HLib::Task *CwonQueuePipeDynamics() {
    return Queue_EVT_AT_PlayDynamix(SHEN_NAME(PIP1), "PIPE", 2, 3, 12, nullptr, 2, 0);
}
void CwonFallingWorkerCallback(void *data) {
    auto *work = RequireWork(data, 322);
    if (!work)
        return;
    switch (work->ticks) {
    case 0:
        if (!DoesScenarioCharacterExist(ana))
            return;
        ++work->ticks;
        [[fallthrough]];
    case 1: {
        ++work->ticks;
        Vector3 position;
        EV_GetCharaPosition(ana, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
        position.y = 100.9f;
        SetPosition(ana, CWON_CENTER, position);
        SetYaw(ana, CWON_CENTER, 270);
        mtwkSetEnabled(scenFindMotionwork(ana), 0);
        Loop3DSoundEffect(669);
        EVT_AT_SetSound(670, 20);
        EVT_AT_SetMot(ana, 4181, 20, 1, 0x40000000U, 0);
        auto *task = work->root->temporaryTasks[work->slot];
        if (task)
            CwonQueueMotionWait(task, ana, 170);
        else
            EV_debug_log("CWON\\eCWON_et_ChaseWon_jama.c", 341, "Taskid Error\n");
        break;
    }
    case 2: {
        work->ticks = 3;
        auto *task = work->root->temporaryTasks[work->slot];
        if (task)
            CwonQueueMotionWait(task, ana, 0);
        else
            EV_debug_log("CWON\\eCWON_et_ChaseWon_jama.c", 347, "Taskid Error\n");
        break;
    }
    case 4:
        CwonDestroyTemporary(work->root, static_cast<std::int32_t>(work->slot));
        break;
    default:
        break;
    }
}
void CwonFallingSignCallback(void *data) {
    auto *work = RequireWork(data, 367);
    if (!work || work->ticks)
        return;
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(sign, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    GetScenarioCharacterEulerAngles(sign, &rotation, EventTransformFlags::Local);
    std::uint8_t positionFlags = position.x > 265.69998f ? 8 : 0;
    if (position.y < 101.1f)
        positionFlags |= 16;
    std::uint8_t rotationFlags = rotation.z < 0x4000 ? 32 : 0;
    if (rotation.y > 0)
        rotationFlags |= 16;
    position.x = -.029999999f;
    position.y = .05f;
    rotation.y = -182;
    rotation.z = 910;
    if (positionFlags || rotationFlags) {
        CwonSetPositionMasked(sign, positionFlags, position);
        CwonSetRotationMasked(sign, rotationFlags, rotation);
    } else
        CwonDestroyTemporary(work->root, static_cast<std::int32_t>(work->slot));
}
void CwonOpeningDoorCallback(void *data) {
    auto *work = RequireWork(data, 401);
    if (!work)
        return;
    if (work->ticks == 0) {
        if (!DoesScenarioCharacterExist(kjn))
            return;
        ++work->ticks;
        Loop3DSoundEffect(671);
        CwonQueueLook(kjn, CWON_RYO, 0, 0, 0, 3, 50, 30);
    } else if (work->ticks != 1)
        return;
    CwonRotateDoorCallback(nullptr);
    Vector3i rotation;
    GetScenarioCharacterEulerAngles(SHEN_NAME(DDOR), &rotation, EventTransformFlags::Local);
    if (rotation.y < 15473) {
        Show(kjn);
        EVT_AT_SetMot(kjn, 4152, -1, .8f, 0, 0);
        auto *task = work->root->temporaryTasks[work->slot];
        if (task)
            CwonQueueMotionWait(task, CWON_RYO, 0);
        else
            EV_debug_log("CWON\\eCWON_et_ChaseWon_jama.c", 421, "Taskid Error\n");
        mtwkSetEnvironmentEnabled(scenFindMotionwork(kjn), 0);
        EVT_AT_SetMot(kjn, 4154, -1, 1, 0x20000000U, 0);
        CwonDestroyTemporary(work->root, static_cast<std::int32_t>(work->slot));
    }
}
void CwonTurningPedestrianCallback(void *data) {
    auto *work = RequireWork(data, 449);
    if (!work)
        return;
    if (work->ticks == 0) {
        if (!DoesScenarioCharacterExist(kln))
            return;
        ++work->ticks;
        CwonInitializePedestrian();
        Vector3 position;
        EV_GetCharaPosition(CWON_WON, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    } else if (work->ticks != 1)
        return;
    if (!DoesScenarioCharacterExist(kln) || CwonTurnTowardYaw(kln, 49152, 182, true))
        CwonDestroyTemporary(work->root, static_cast<std::int32_t>(work->slot));
}
void CwonSetCollisionTables() {
    SetPosition(SHEN_NAME(TBLa), CWON_CENTER, {260.1205139160156f, 100.98909759521484f, 401.52020263671875f});
    SetPosition(SHEN_NAME(TBLb), CWON_CENTER, {259.2f, 101, 398.6f});
    SetPosition(SHEN_NAME(TBLe), CWON_CENTER, {263.0960998535156f, 100.98909759521484f, 401.5719909667969f});
    SetPosition(SHEN_NAME(IS19), CWON_CENTER, {264.0480041503906f, 100.9990005493164f, 401.5841064453125f});
    SetPosition(SHEN_NAME(IS17), CWON_CENTER, {262.0747985839844f, 100.9990005493164f, 401.264404296875f});
    SetPosition(SHEN_NAME(IS_5), CWON_CENTER, {260.04510498046875f, 100.9990005493164f, 400.3627014160156f});
    SetPosition(SHEN_NAME(IS_9), CWON_CENTER, {258.8f, 100.9990005493164f, 397.7f});
    SetPosition(SHEN_NAME(IS10), CWON_CENTER, {259.8f, 100.9990005493164f, 399.2f});
    SetPosition(SHEN_NAME(IS11), CWON_CENTER, {258.9f, 100.9990005493164f, 399.3f});
    SetYaw(SHEN_NAME(TBLa), CWON_CENTER, 90);
}
void CwonSetFallenTable() {
    SetPosition(SHEN_NAME(IS17), CWON_CENTER, {262.0440979003906f, 100.9990005493164f, 401.6568908691406f});
    SetPosition(SHEN_NAME(IS_5), CWON_CENTER, {263.93829345703125f, 100.9990005493164f, 401.2322082519531f});
    SetPosition(SHEN_NAME(TBLe), CWON_CENTER, {262.9399108886719f, 100.98909759521484f, 401.94659423828125f});
    SetYaw(SHEN_NAME(TBLe), CWON_CENTER, 0);
}
// @verified - p
std::int32_t CwonInitializePedestrian() {
    if (!DoesScenarioCharacterExist(kln))
        return 0;
    Show(kln);
    CwonDisableGravity(kln);
    CwonEnablePositionedCharacter(kln, {253.3f, 100.9f, 422.5f}, 350);
    EventSetMotion motion{.cleanup = 0, .character = kln, .animation = 4152, .frame = 0.0f, .speed = 1.0f, .flags = 0, .delay = 0.0f};
    EVT_AT_SetMot_Callback(&motion);
    return static_cast<std::int32_t>(GetMotionFrameCount(4152));
}
} // namespace shenmue::s2

#endif
