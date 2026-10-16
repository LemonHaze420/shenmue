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
    EV_mtwkSetEnabled(scenFindMotionwork(name), 1);
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
    SetMotionEnvironmentEnabled(scenFindMotionwork(get), 0);
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
        EV_mtwkSetEnabled(scenFindMotionwork(ana), 0);
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
        SetMotionEnvironmentEnabled(scenFindMotionwork(kjn), 0);
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
