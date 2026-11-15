#if defined(SHENMUE_EVENT_OVERLAY) && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/scripts/cwon/area_cwon.hpp"

void CwonQueueEscape(CwonWork *work, int entry) {
    HLibTaskCallback starts[6] = {(HLibTaskCallback)CwonEscapeStreetCallback, (HLibTaskCallback)CwonEscapeStreetCallback, 0, 0,
        CwonStartWarehouseEscapeCallback, CwonStartStairsEscapeCallback};
    CwonEscapeWork *data;
    if (entry < 6) {
        if (work->escapeTask)
            FTBL.DebugLogToFileRemoved("MakeEscapeWonTask Error\n");
        else if (starts[entry]) {
            data = CwonCreateTask(&work->escapeTask, starts[entry], sizeof(*data));
            if (data) {
                data->root = work;
                data->task = work->escapeTask;
                data->state = 0;
            } else if (!data)
                FTBL.EventAssertNullsub("./src/et_ChaseWon_won.c", 100);
        }
    } else
        FTBL.DebugLogToFileRemoved("MakeEscapeWonTask Error %d\n");
}

void CwonDestroyEscape(CwonWork *work) {
    if (work->escapeTask) {
        FTBL.HLib_DestroyTaskInternal(work->escapeTask);
        work->escapeTask = 0;
    }
}

void CwonStartOpponentAt(int entry) {
    static Vector3 CWON_OPPONENT_POSITIONS[] = {{233.119995f, 100.300003f, 396.0f}, {233.119995f, 100.300003f, 396.0f}, {260.0f, 101.0f, 399.799988f},
        {260.0f, 101.0f, 399.799988f}, {265.0f, 100.900002f, 405.100006f}, {246.0f, 100.0f, 443.5f}};
    static int CWON_OPPONENT_YAWS[] = {300, 300, 250, 250, 205, 180};

    FTBL.EV_SetCharacterAlphaEnabled(WON, 1);
    EVT_AT_SetPos(WON, CENTER, CWON_OPPONENT_POSITIONS[entry].x, CWON_OPPONENT_POSITIONS[entry].y, CWON_OPPONENT_POSITIONS[entry].z, 0);
    EVT_AT_SetAngY(WON, CENTER, CWON_OPPONENT_YAWS[entry], 0);
    FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(WON), 1);
    CwonShowCharacter(WON);
}

void CwonStartWarehouseEscapeCallback(void *data) {
    CwonStartOpponentAt(4);
    EVT_AT_SetMot(WON, 0x1043, 0, 1, 0x40000000, 0);
    FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)CwonEscapeWarehouseCallback);
}

void CwonStartStairsEscapeCallback(void *data) {
    CwonStartOpponentAt(5);
    EVT_AT_SetMot(WON, 0x1023, -1, 1, 0x20000000, 0);
    FTBL.HLib_SetCurrentTaskTo((HLibTaskCallback)CwonEscapeStairsCallback);
}

void CwonEscapeStreetCallback(CwonEscapeWork *work) {
    if (!work)
        FTBL.DebugLogToFileRemoved("WonTask Error\n");
    else
        switch (work->state) {
        case 0:
            work->state++;
            EVT_AT_SetMot(WON, 0x1050, -1, .9f, 0, 0);
            CwonRestoreGravity(WON);
            EVT_AT_SetPos(WON, CENTER, 236.7f, 100.3f, 384.5f, 0);
            EVT_AT_SetAngY(WON, CENTER, 350, 0);
            break;
        case 1:
            if (CwonTurnTowardYaw(WON, 0xc000, 364, 1))
                CwonDestroyEscape(work->root);
            break;
        }
}

void CwonEscapeWarehouseCallback(CwonEscapeWork *work) {
    static const int parts[10] = {161, 160, 159, 158, 157, 156, 155, 154, 153, 152};
    if (!work)
        FTBL.DebugLogToFileRemoved("WonTask Error\n");
    else
        switch (work->state) {
        case 0:
            work->state++;
            EVT_AT_SetVoice("0011B006", 35);
            EVT_AT_SetAngY(WON, CENTER, 225, 90);
            EVT_AT_SetMot(WON, 0x1050, -1, .9f, 0, 90);
            EVT_AT_SetPos(WON, CENTER, 269.6f, 100.9f, 415.7f, 100);
            EVT_AT_SetAngY(WON, CENTER, 145, 100);
            FTBL.HLib_SetupTaskDelay(100);
            cwonDynamixTask = Queue_EVT_AT_PlayDynamix(SHEN_NAME4('H', 'A', 'K', '0'), "HAKB", 2, 124, 10, parts, 2, 38);
            break;
        case 1:
            if (CwonTurnTowardYaw(WON, 0x471c, 327, 1))
                CwonDestroyEscape(work->root);
            break;
        }
}

void CwonAdvanceWonWarehouse(void) {
    EVT_AT_SetPos(WON, CENTER, 264, 100.9f, 420.4f, 0);
}

void CwonEscapeStairsCallback(CwonEscapeWork *work) {
    int done, yaw, reverse;
    if (!work)
        FTBL.DebugLogToFileRemoved("WonTask Error\n");
    else
        switch (work->state) {
        case 0:
            if (CwonPositionThreshold(WON, 445.5f, 1, 0)) {
                work->state++;
                EVT_AT_SetMot(WON, 0x1050, -1, .9f, 0, 0);
            } else {
                CwonMoveBackward(WON, .09f);
                EVT_AT_SetPos(WON, WON, 0, .0675f, 0, 0);
            }
            break;
        case 1:
            if (CwonTurnTowardYaw(WON, 0x438e, 910, 1))
                work->state++;
            break;
        case 2:
            if (CwonPositionThreshold(WON, 238, 0, 1))
                work->state++;
            break;
        case 3:
            if (CwonPositionThreshold(WON, 235, 0, 1)) {
                if (CwonTurnTowardYaw(WON, 0x8000, 910, 0))
                    work->state++;
            }
            break;
        case 4:
            if (CwonPositionThreshold(WON, 459, 1, 0)) {
                work->state++;
                CwonDisableGravity(WON);
                FTBL.HLib_SetupTaskDelay(10);
            }
            break;
        case 5:
            CwonFinishEscape(work->root);
            break;
        }
}

void CwonFinishEscape(CwonWork *work) {
    CwonDestroyEscape(work);
    EVT_AT_SetPos(WON, CENTER, 233.2f, 99.1f, 460.1f, 0);
    EVT_AT_SetAngY(WON, CENTER, 0, 0);
    EVT_AT_SetMot(WON, 0x1052, -1, .5f, 0, 0);
}

#else
#include "s2/character.hpp"
#include "s2/scripts/cwon/cwon_helpers.hpp"
#include "s2/scripts/cwon/cwon_tables.hpp"
#include "s2/debug.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
namespace shenmue::s2 {
namespace {
constexpr Vector3 opponentPositions[]{{233.1199951171875f, 100.30000305175781f, 396}, {233.1199951171875f, 100.30000305175781f, 396},
    {260, 101, 399.79998779296875f}, {260, 101, 399.79998779296875f}, {265, 100.9000015258789f, 405.1000061035156f}, {246, 100, 443.5f}};
constexpr float opponentYaws[]{300, 300, 250, 250, 205, 180};
void StartOpponentAt(std::int32_t entry) {
    EV_SetCharacterAlphaEnabled(CWON_WON, true);
    EventSetPosition position{0, CWON_WON, CWON_CENTER, opponentPositions[entry], 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{0, CWON_WON, CWON_CENTER, opponentYaws[entry], 0};
    EVT_AT_SetAngY_Callback(&angle);
    mtwkSetEnabled(scenFindMotionwork(CWON_WON), 1);
    EV_EnsureCharacterDraw(CWON_WON);
    EV_EnableCharacterDisplay(CWON_WON);
}
void DestroyEscape(CwonEventWork *root) {
    if (root->escapeTask) {
        HLib::DestroyTaskInternal(root->escapeTask);
        root->escapeTask = nullptr;
    }
}
void EscapeStreetCallback(void *data) {
    auto *work = static_cast<CwonEscapeWork *>(data);
    if (!work) {
        EV_debug_log("CWON\\eCWON_et_ChaseWon_won.c", 182, "WonTask Error\n");
        return;
    }
    if (work->state == 0) {
        work->state = 1;
        EVT_AT_SetMot(CWON_WON, 4176, -1, .89999998f, 0, 0);
        CwonRestoreGravity(CWON_WON);
        EventSetPosition position{0, CWON_WON, CWON_CENTER, {236.7f, 100.3f, 384.5f}, 0};
        EVT_AT_SetPosRot(&position);
        EventSetAngle angle{0, CWON_WON, CWON_CENTER, 350, 0};
        EVT_AT_SetAngY_Callback(&angle);
    } else if (work->state == 1 && CwonTurnTowardYaw(CWON_WON, 49152, 364, true))
        DestroyEscape(work->root);
}
void EscapeWarehouseCallback(void *data) {
    auto *work = static_cast<CwonEscapeWork *>(data);
    if (!work) {
        EV_debug_log("CWON\\eCWON_et_ChaseWon_won.c", 211, "WonTask Error\n");
        return;
    }
    if (work->state == 0) {
        work->state = 1;
        EVT_AT_SetVoice("0011B006", 1, 15); // @text EN: "Take this!" | JA: "これでも食らえっ！"
        EVT_AT_SetAngY(CWON_WON, CWON_CENTER, 225, 90);
        EVT_AT_SetMot(CWON_WON, 4176, -1, .89999998f, 0, 90);
        EVT_AT_SetPos(CWON_WON, CWON_CENTER, {269.60001f, 100.9f, 415.70001f}, 100);
        EVT_AT_SetAngY(CWON_WON, CWON_CENTER, 145, 100);
        HLib::SetupTaskDelay(100);
        cwonDynamixTask = Queue_EVT_AT_PlayDynamix(SHEN_NAME(HAK0), CWON_BOX_DYNAMICS, 2, 124, 10, CWON_BOX_DYNAMIC_PARTS, 2, 38);
    } else if (work->state == 1 && CwonTurnTowardYaw(CWON_WON, 18204, 327, true))
        DestroyEscape(work->root);
}
void EscapeStairsCallback(void *data) {
    auto *work = static_cast<CwonEscapeWork *>(data);
    if (!work) {
        EV_debug_log("CWON\\eCWON_et_ChaseWon_won.c", 247, "WonTask Error\n");
        return;
    }
    Vector3 position;
    switch (work->state) {
    case 0:
        EV_GetCharaPosition(CWON_WON, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
        if (position.z <= 445.5f) {
            CwonMoveBackward(CWON_WON, .090000004f);
            EventSetPosition offset{0, CWON_WON, CWON_WON, {0, .067500003f, 0}, 0};
            EVT_AT_SetPosRot(&offset);
        } else {
            ++work->state;
            EVT_AT_SetMot(CWON_WON, 4176, -1, .89999998f, 0, 0);
        }
        break;
    case 1:
        if (CwonTurnTowardYaw(CWON_WON, 17294, 910, true))
            ++work->state;
        break;
    case 2:
        EV_GetCharaPosition(CWON_WON, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
        if (position.x < 238)
            ++work->state;
        break;
    case 3:
        EV_GetCharaPosition(CWON_WON, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
        if (position.x < 235 && CwonTurnTowardYaw(CWON_WON, 32768, 910, false))
            ++work->state;
        break;
    case 4:
        EV_GetCharaPosition(CWON_WON, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
        if (position.z > 459) {
            ++work->state;
            CwonDisableGravity(CWON_WON);
            HLib::SetupTaskDelay(10);
        }
        break;
    case 5:
        CwonFinishEscape(work->root);
        break;
    default:
        break;
    }
}
// @verified - p
void CwonStartWarehouseEscapeCallback(void *) {
    StartOpponentAt(4);
    EventSetMotion motion{.cleanup = 0, .character = CWON_WON, .animation = 4163, .frame = 0.0f, .speed = 1.0f, .flags = 0x40000000U, .delay = 0.0f};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(4163);
    HLib::SetCurrentTaskTo(EscapeWarehouseCallback);
}
void StartStairsEscapeCallback(void *) {
    StartOpponentAt(5);
    EventSetMotion motion{0, CWON_WON, 4131, -1, 1, 0x20000000U, 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(4131);
    HLib::SetCurrentTaskTo(EscapeStairsCallback);
}
} // namespace
void CwonQueueEscape(CwonEventWork *root, std::int32_t entry) {
    constexpr HLib::TaskCallback starts[]{
        EscapeStreetCallback, EscapeStreetCallback, nullptr, nullptr, CwonStartWarehouseEscapeCallback, StartStairsEscapeCallback};
    if (entry >= 6) {
        EV_debug_log("CWON\\eCWON_et_ChaseWon_won.c", 115, "MakeEscapeWonTask Error %d\n", entry);
        return;
    }
    if (root->escapeTask) {
        EV_debug_log("CWON\\eCWON_et_ChaseWon_won.c", 101, "MakeEscapeWonTask Error\n");
        return;
    }
    if (starts[entry]) {
        root->escapeTask = HLib::EnqueueTaskWithParameter(starts[entry], 4, 11, sizeof(CwonEscapeWork), CWON_TASK);
        auto *work = root->escapeTask ? static_cast<CwonEscapeWork *>(HLib::GetTaskParameter(root->escapeTask)) : nullptr;
        if (work) {
            work->root = root;
            work->task = root->escapeTask;
            work->state = 0;
        }
    }
}

} // namespace shenmue::s2

#endif
