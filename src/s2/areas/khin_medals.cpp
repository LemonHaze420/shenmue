#include "s2/areas/area_khin.hpp"
#include "s2/areas/khin_tables.hpp"
#include "s2/camera.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/game_runtime.hpp"
#include "s2/inventory.hpp"
#include "s2/map_control.hpp"
#include "s2/save.hpp"

namespace shenmue::s2 {
void KhinTokenReward3000Callback(void *) {
    AddInventoryItem(4248, 1, 1);
    HLib::DestroyCurrentTask();
}
void KhinTokenReward10000Callback(void *) {
    AddInventoryItem(4249, 1, 1);
    if (!GetItemState(4248))
        AddInventoryItem(4248, 1, 0);
    HLib::DestroyCurrentTask();
}
void KhinTokenReward50000Callback(void *) {
    AddInventoryItem(4250, 1, 1);
    if (!GetItemState(4248))
        AddInventoryItem(4248, 1, 0);
    if (!GetItemState(4249))
        AddInventoryItem(4249, 1, 0);
    HLib::DestroyCurrentTask();
}
void KhinTokenReward200000Callback(void *) {
    AddInventoryItem(4251, 1, 1);
    if (!GetItemState(4248))
        AddInventoryItem(4248, 1, 0);
    if (!GetItemState(4249))
        AddInventoryItem(4249, 1, 0);
    if (!GetItemState(4250))
        AddInventoryItem(4250, 1, 0);
    HLib::DestroyCurrentTask();
}
void KhinTokenReward500000Callback(void *) {
    AddInventoryItem(4252, 1, 1);
    if (!GetItemState(4248))
        AddInventoryItem(4248, 1, 0);
    if (!GetItemState(4249))
        AddInventoryItem(4249, 1, 0);
    if (!GetItemState(4250))
        AddInventoryItem(4250, 1, 0);
    if (!GetItemState(4251))
        AddInventoryItem(4251, 1, 0);
    HLib::DestroyCurrentTask();
}
void KhinTokenReward1000000Callback(void *) {
    AddInventoryItem(4253, 1, 1);
    if (!GetItemState(4248))
        AddInventoryItem(4248, 1, 0);
    if (!GetItemState(4249))
        AddInventoryItem(4249, 1, 0);
    if (!GetItemState(4250))
        AddInventoryItem(4250, 1, 0);
    if (!GetItemState(4251))
        AddInventoryItem(4251, 1, 0);
    if (!GetItemState(4252))
        AddInventoryItem(4252, 1, 0);
    HLib::DestroyCurrentTask();
}
std::int32_t KhinXhgAward(KhinWork &work) {
    std::int32_t medal = -1;
    const std::int32_t coins = static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::SegaCoins));
    for (std::int32_t index = 0; index < 6; ++index)
        if (coins >= khinMedalThresholds[index] && !GetItemState(4248 + index))
            medal = index;
    if (medal < 0) {
        EVT_AT_SetCam(4055, KHIN_RYO, KHIN_RYO, 0, 0, 0);
        camCtrlDisableFilmBars();
        EVT_AT_SetVoice(work.row->voices[2], 1, 0);
        EVT_AT_MenCtrl({.frames = 16, .targetAlpha = 255, .delay = 104});
        return 120;
    }
    const shen_name model = khinMedalModels[medal];
    EVT_AT_SetVoice(work.row->voices[4], 1, 0);
    EVT_AT_SetMot(KHIN_RYO, 0xf003, 0, 1, 0x40000000U, 0);
    EVT_AT_SetCam(4051, KHIN_RYO, KHIN_RYO, 0, 0, 0);
    camCtrlDisableFilmBars();
    EVT_AT_SetCam(4053, KHIN_RYO, KHIN_RYO, 0, 0, 80);
    EVT_AT_SetVoice(work.row->voices[7], 1, 70);
    EVT_AT_SetVoice(work.row->voices[5], 1, 170);
    HLib::Task *attach = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), KHIN_TASK);
    RegisterEventTask(attach);
    EventCling *held = static_cast<EventCling *>(HLib::GetTaskParameter(attach));
    *held = {.cleanup = 1,
        .target = model,
        .character = work.row->character,
        .bone = 14,
        .position = {.24f, -.009f, -.089f},
        .rotation = {60074, 32768, 21845},
        .mode = 0,
        .delay = 140};
    EVT_AT_SetPos(model, KHIN_INVALID, {116.44f, 101.51f, 112.78f}, 220);
    EVT_AT_SetAng_Enqueue(model, KHIN_INVALID, 270, 90, 0, 220);
    HLib::Task *detach = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), KHIN_TASK);
    RegisterEventTask(detach);
    EventCling *released = static_cast<EventCling *>(HLib::GetTaskParameter(detach));
    released->cleanup = 1;
    released->target = model;
    released->character = KHIN_NONE;
    released->mode = 1;
    released->delay = 220;
    EVT_AT_SetCam(4052, KHIN_RYO, KHIN_RYO, 0, 0, 220);
    HLib::Task *award = HLib::EnqueueTaskWithoutParameter(khinMedalCallbacks[medal], 4, 0x0b, static_cast<std::int32_t>(KHIN_TASK.name));
    HLib::SetupTimedTask(award, 220);
    EVT_AT_MenCtrl({.frames = 16, .targetAlpha = 255, .delay = 364});
    return 380;
}
std::int32_t KhinGstAward(KhinWork &work) {
    std::int32_t medal = -1;
    const std::int32_t coins = static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::SegaCoins));
    for (std::int32_t index = 0; index < 6; ++index)
        if (coins >= khinMedalThresholds[index] && !GetItemState(4248 + index))
            medal = index;
    if (medal < 0) {
        EVT_AT_SetCam(4151, KHIN_RYO, KHIN_RYO, 0, 0, 0);
        camCtrlDisableFilmBars();
        EVT_AT_SetMot(KHIN_RYO, 0xf003, 0, 1, 0x40000000U, 0);
        EVT_AT_SetVoice(work.row->voices[2], 1, 0);
        EVT_AT_MenCtrl({.frames = 16, .targetAlpha = 255, .delay = 104});
        return 120;
    }
    const shen_name model = khinMedalModels[medal];
    EVT_AT_SetVoice(work.row->voices[4], 1, 0);
    EVT_AT_SetMot(KHIN_RYO, 0xf003, 0, 1, 0x40000000U, 80);
    EVT_AT_SetCam(4152, KHIN_RYO, KHIN_RYO, 0, 0, 80);
    EVT_AT_SetVoice(work.row->voices[7], 1, 70);
    EVT_AT_SetVoice(work.row->voices[5], 1, 110);
    HLib::Task *attach = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), KHIN_TASK);
    RegisterEventTask(attach);
    EventCling *held = static_cast<EventCling *>(HLib::GetTaskParameter(attach));
    *held = {.cleanup = 1,
        .target = model,
        .character = work.row->character,
        .bone = 14,
        .position = {.24f, -.009f, -.089f},
        .rotation = {60074, 32768, 21845},
        .mode = 0,
        .delay = 140};
    EVT_AT_SetPos(model, KHIN_INVALID, {209.34f, 104.71f, 86.6f}, 220);
    EVT_AT_SetAng_Enqueue(model, KHIN_INVALID, 106.5f, 272, 163.7f, 220);
    HLib::Task *detach = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), KHIN_TASK);
    RegisterEventTask(detach);
    EventCling *released = static_cast<EventCling *>(HLib::GetTaskParameter(detach));
    released->cleanup = 1;
    released->target = model;
    released->character = KHIN_NONE;
    released->mode = 1;
    released->delay = 220;
    EVT_AT_SetCam(4153, KHIN_RYO, KHIN_RYO, 0, 0, 220);
    HLib::Task *award = HLib::EnqueueTaskWithoutParameter(khinMedalCallbacks[medal], 4, 0x0b, static_cast<std::int32_t>(KHIN_TASK.name));
    HLib::SetupTimedTask(award, 220);
    EVT_AT_MenCtrl({.frames = 16, .targetAlpha = 255, .delay = 364});
    return 380;
}
std::int32_t KhinAgnAward(KhinWork &work) {
    std::int32_t medal = -1;
    const std::int32_t coins = static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::SegaCoins));
    for (std::int32_t index = 0; index < 6; ++index)
        if (coins >= khinMedalThresholds[index] && !GetItemState(4248 + index))
            medal = index;
    if (medal < 0) {
        EVT_AT_SetCam(4251, KHIN_RYO, KHIN_RYO, 0, 0, 0);
        camCtrlDisableFilmBars();
        EVT_AT_SetVoice(work.row->voices[2], 1, 0);
        EVT_AT_SetMot(KHIN_RYO, 0xf003, 0, 1, 0x40000000U, 0);
        EVT_AT_MenCtrl({.frames = 16, .targetAlpha = 255, .delay = 104});
        return 300;
    }
    const shen_name model = khinMedalModels[medal];
    EVT_AT_SetVoice(work.row->voices[4], 1, 0);
    EVT_AT_SetMot(KHIN_RYO, 0xf003, 0, 1, 0x40000000U, 0);
    EVT_AT_SetCam(4251, KHIN_RYO, KHIN_RYO, 0, 0, 0);
    camCtrlDisableFilmBars();
    EVT_AT_SetVoice(work.row->voices[7], 1, 100);
    EVT_AT_SetVoice(work.row->voices[5], 1, 200);
    HLib::Task *attach = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), KHIN_TASK);
    RegisterEventTask(attach);
    EventCling *held = static_cast<EventCling *>(HLib::GetTaskParameter(attach));
    *held = {.cleanup = 1,
        .target = model,
        .character = work.row->character,
        .bone = 14,
        .position = {.24f, -.009f, -.089f},
        .rotation = {60074, 32768, 21845},
        .mode = 0,
        .delay = 140};
    EVT_AT_SetMot(work.row->character, khinMotions[work.row->motion][0], 0, 1, 0x40000000U, 220);
    HLib::Task *detach = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), KHIN_TASK);
    RegisterEventTask(detach);
    EventCling *released = static_cast<EventCling *>(HLib::GetTaskParameter(detach));
    released->cleanup = 1;
    released->target = model;
    released->character = KHIN_NONE;
    released->mode = 1;
    released->delay = 219;
    EVT_AT_SetPos(model, KHIN_INVALID, {288.15f, 100.92f, 192.68f}, 220);
    EVT_AT_SetAng_Enqueue(model, KHIN_INVALID, 90, 90, 180, 220);
    EVT_AT_SetCam(4253, KHIN_RYO, KHIN_RYO, 0, 0, 220);
    HLib::Task *award = HLib::EnqueueTaskWithoutParameter(khinMedalCallbacks[medal], 4, 0x0b, static_cast<std::int32_t>(KHIN_TASK.name));
    HLib::SetupTimedTask(award, 220);
    EVT_AT_MenCtrl({.frames = 16, .targetAlpha = 255, .delay = 364});
    return 560;
}
} // namespace shenmue::s2
