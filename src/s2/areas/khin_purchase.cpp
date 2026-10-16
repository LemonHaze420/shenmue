#include "s2/areas/area_khin.hpp"
#include "s2/areas/khin_tables.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/hand.hpp"
#include "s2/inventory.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/screen_film.hpp"
#include "s2/step_event.hpp"

namespace shenmue::s2 {
namespace {
void KhinGiveItemCallback(void *) {
    AddInventoryItem(khinItems[khinSelector], 1, 1);
    HLib::DestroyCurrentTask();
}
void KhinGiveSecondItemCallback(void *) {
    if (khinSelector == 18)
        AddInventoryItem(0x1095, 1, 1);
    if (khinSelector == 20)
        AddInventoryItem(0x1096, 1, 1);
    if (khinSelector == 21)
        AddInventoryItem(0x1097, 1, 1);
    if (khinSelector == 19)
        AddInventoryItem(0x1094, 1, 1);
    HLib::DestroyCurrentTask();
}
} // namespace
void KhinPurchaseCallback(void *data) {
    KhinWork &work = *static_cast<KhinWork *>(data);
    if (work.state == 0) {
        work.row = &khinCharacters[khinSelector];
        EV_CheckWarmStartNpcRoute(work.row->character);
        mtwkRequestMotion(scenFindMotionwork(KHIN_RYO), 0xf003, MotionRegion::LowerBody);
        work.state = 1;
        return;
    }
    if (work.state == 1) {
        if (!EV_ForceLoadNpc(work.row->character))
            return;
        switch (khinSelector) {
        case 0:
        case 3:
        case 4:
            EVT_AT_SetPos(KHIN_RYO, KHIN_RYO, {.15f, 0, 0}, 0);
            EVT_AT_SetAngY(KHIN_RYO, KHIN_RYO, khinSelector == 0 ? 50.0f : 60.0f, 0);
            break;
        case 11:
        case 15:
        case 19:
            EVT_AT_SetPos(KHIN_RYO, KHIN_RYO, {0, 0, 0}, 0);
            EVT_AT_SetAngY(KHIN_RYO, KHIN_RYO, 20, 0);
            break;
        default:
            if (khinSelector <= 21) {
                EVT_AT_SetPos(KHIN_RYO, KHIN_RYO, {0, 0, 0}, 0);
                EVT_AT_SetAngY(KHIN_RYO, KHIN_RYO, 50, 0);
            }
            break;
        }
        EVT_AT_MenCtrl({.frames = 16, .startAlpha = 255});
        SetScenarioReflectorBoardValues(KHIN_RYO, &khinReflector);
        EV_SetRefBoardParamsFor(KHIN_RYO, 1);
        EV_SetRefBoardParamsFor(KHIN_HAND, 1);
        SetScenarioReflectorBoardValues(work.row->character, &khinReflector);
        EV_SetRefBoardParamsFor(work.row->character, 1);
        EV_SetRefBoardParamsFor(KHIN_RYO, 1);
        EV_SetRefBoardParamsFor(KHIN_HAND, 1);
        EV_SetRefBoardParamsFor(work.row->character, 1);
        EV_SetCharacterAlphaEnabled(KHIN_RYO, false);
        EV_SetCharacterAlphaEnabled(work.row->character, false);
        if (khinSelector == 22) {
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
            EV_AT_SetPos(KHIN_RYO, EventTransformFlags::AllAxes, {257.9f, 101.0f, 404.4f});
            EV_AT_SetAngY(KHIN_RYO, EventTransformFlags::AllAxes, {0, 54613, 0});
        }
        EVT_AT_SetPos(work.row->character, KHIN_RYO, {-.77f, 0, .5f}, 0);
        EVT_AT_SetAngY(work.row->character, KHIN_RYO, 300, 0);
        EV_EnsureCharacterDraw(KHIN_HAND);
        EVT_AT_SetPos(KHIN_HAND, KHIN_RYO, {0, 1.4f, 0}, 0);
        EVT_AT_SetAngY(KHIN_HAND, KHIN_RYO, 145, 0);
        EV_DisableCharacterDisplay(KHIN_HAND);
        SetLowQualityHands(FindCharacterByScenarioName(KHIN_HAND));
        SetHighModelXYZ(FindCharacterByScenarioName(KHIN_HAND), 1, 0, 0);
        const Vector3i handRotation{18258, 2476, 5540};
        ApplyCharacterHandRotation(FindCharacterByScenarioName(KHIN_HAND), 0, 21, &handRotation);
        ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(KHIN_HAND), 0, reinterpret_cast<std::byte *>(khinHandCommand), 0);
        const Vector3 scale = khinSelector == 22 ? Vector3{1, 1, 1} : Vector3{.5f, .5f, .5f};
        SetMotionworkFlagForCharaById(work.row->firstItem, &scale);
        EVT_AT_SetPos(work.row->firstItem, KHIN_HAND, khinItemOffset, 0);
        if (khinSelector - 14 <= 3)
            EVT_AT_SetAng_Enqueue(work.row->firstItem, KHIN_HAND, 300, 0, 0, 0);
        else
            EVT_AT_SetAngY(work.row->firstItem, KHIN_HAND, 0, 0);
        EV_DisableCharacterDisplay(work.row->firstItem);
        if (work.row->secondItem.name) {
            EVT_AT_SetPos(work.row->secondItem, KHIN_HAND, khinItemOffset, 0);
            EVT_AT_SetAng_Enqueue(work.row->secondItem, KHIN_HAND, 300, 0, 0, 0);
            const Vector3 secondScale{.5f, .5f, .5f};
            SetMotionworkFlagForCharaById(work.row->secondItem, &secondScale);
            EV_DisableCharacterDisplay(work.row->secondItem);
        }
        (void)FindMotionWork(FindCharacterByScenarioName(KHIN_RYO));
        (void)FindMotionWork(FindCharacterByScenarioName(work.row->character));
        SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(work.row->character), Scheme, 10, 1);
        work.state = 2;
        return;
    }
    if (work.state == 4) {
        HLib::SetCurrentTaskTo(KhinPurchaseExitCallback);
        return;
    }
    if (work.state != 2)
        return;
    const KhinCharacterRow &row = *work.row;
    EVT_AT_SetMot(row.character, khinMotions[row.motion][0], 0, 1, 0x40000000U, 0);
    EVT_AT_SetPoint(row.character, KHIN_RYO, 5, 1, 0, 0, 0, 0xa0000000U);
    EVT_AT_SetCam(3000, KHIN_RYO, KHIN_RYO, 0, 0, 0);
    EVT_AT_SetVoice(row.voices[0], 1, 0);
    EVT_AT_SetMot(KHIN_RYO, 0xf022, 0, 1, 0x80000000U, 130);
    EVT_AT_SetCam(3010, KHIN_RYO, KHIN_RYO, 0, 0, 130);
    const float random = GetRandomNumberInRange(3);
    EVT_AT_SetVoice(row.voices[random == 0 ? 1 : random == 1 ? 2 : 3], 1, 130);
    EVT_AT_SetPoint(KHIN_RYO, row.character, 5, 1, 0, 130, 0, 0x20000000U);
    EVT_AT_SetPoint(row.character, KHIN_RYO, 5, 1, 0, 130, 0, 0x20000000U);
    EVT_AT_SetMot(row.character, khinMotions[row.motion][1], 0, 1, 0x80000000U, 180);
    EVT_AT_SetPos(KHIN_RYO, KHIN_RYO, {0, 0, 0}, 180);
    EVT_AT_SetAngY(KHIN_RYO, KHIN_RYO, 90, 180);
    EVT_AT_SetMot(KHIN_RYO, 0x2b87, 110, 1, 0x40000000U, 180);
    EVT_AT_SetVoice(row.voices[4], 1, 180);
    EVT_AT_SetPoint(KHIN_RYO, row.character, 5, 1, 0, 180, 0, 0x20000000U);
    EVT_AT_SetPoint(row.character, KHIN_RYO, 5, 1, 0, 180, 0, 0x10000000U);
    EVT_AT_SetCam(3015, KHIN_RYO, KHIN_RYO, 0, 0, 180);
    EVT_AT_SetPos(KHIN_RYO, KHIN_RYO, {0, 0, 0}, 260);
    EVT_AT_SetAngY(KHIN_RYO, KHIN_RYO, -90, 260);
    EVT_AT_ObjectDisp_1(KHIN_HAND, 260, 0);
    EVT_AT_ObjectDisp_1(KHIN_RYO, 260, 1);
    EVT_AT_SetMot(row.character, khinMotions[row.motion][0], 0, 1, 0x40000000U, 260);
    EVT_AT_SetPoint(row.character, KHIN_RYO, 5, 1, 0, 260, 0, 0x20000000U);
    EVT_AT_SetCam(3020, KHIN_HAND, KHIN_HAND, 0, 0, 260);
    EVT_AT_ObjectDisp_1(row.firstItem, 260, 0);
    HLib::Task *first = HLib::EnqueueTaskWithoutParameter(KhinGiveItemCallback, 4, 0x0b, static_cast<std::int32_t>(KHIN_TASK.name));
    HLib::SetupTimedTask(first, 260);
    std::int32_t end = 370;
    if (row.secondItem.name) {
        EVT_AT_SetCam(3015, KHIN_RYO, KHIN_RYO, 0, 0, 370);
        EVT_AT_ObjectDisp_1(KHIN_RYO, 370, 0);
        EVT_AT_ObjectDisp_1(KHIN_HAND, 370, 1);
        EVT_AT_ObjectDisp_1(row.firstItem, 370, 1);
        EVT_AT_SetMot(row.character, khinMotions[row.motion][1], 0, 1, 0x80000000U, 370);
        EVT_AT_SetPos(KHIN_RYO, KHIN_RYO, {0, 0, 0}, 370);
        EVT_AT_SetAngY(KHIN_RYO, KHIN_RYO, 90, 370);
        EVT_AT_SetMot(KHIN_RYO, 0x2b87, 110, 1, 0x40000000U, 370);
        EVT_AT_SetVoice(row.voices[6], 1, 370);
        EVT_AT_SetPoint(KHIN_RYO, row.character, 5, 1, 0, 370, 0, 0x20000000U);
        EVT_AT_SetPoint(row.character, KHIN_RYO, 5, 1, 0, 370, 0, 0x10000000U);
        EVT_AT_SetPos(KHIN_RYO, KHIN_RYO, {0, 0, 0}, 450);
        EVT_AT_SetAngY(KHIN_RYO, KHIN_RYO, -90, 450);
        EVT_AT_ObjectDisp_1(KHIN_HAND, 450, 0);
        EVT_AT_ObjectDisp_1(KHIN_RYO, 450, 1);
        EVT_AT_SetMot(row.character, khinMotions[row.motion][0], 0, 1, 0x40000000U, 450);
        EVT_AT_SetPoint(row.character, KHIN_RYO, 5, 1, 0, 450, 0, 0x20000000U);
        EVT_AT_SetCam(3020, KHIN_HAND, KHIN_HAND, 0, 0, 450);
        EVT_AT_ObjectDisp_1(row.secondItem, 450, 0);
        HLib::Task *second = HLib::EnqueueTaskWithoutParameter(KhinGiveSecondItemCallback, 4, 0x0b, static_cast<std::int32_t>(KHIN_TASK.name));
        HLib::SetupTimedTask(second, 450);
        end = 560;
    }
    EVT_AT_SetPos(KHIN_RYO, KHIN_RYO, {0, 0, 0}, end);
    EVT_AT_SetAngY(KHIN_RYO, KHIN_RYO, 90, end);
    EVT_AT_SetMot(KHIN_RYO, 0x2b89, 0, 1, 0x40000000U, end);
    EVT_AT_SetMot(row.character, khinMotions[row.motion][0], 0, 1, 0x40000000U, end);
    EVT_AT_SetCam(3030, KHIN_RYO, KHIN_RYO, 0, 0, end);
    EVT_AT_ObjectDisp_1(KHIN_HAND, end, 1);
    EVT_AT_ObjectDisp_1(row.secondItem.name ? row.secondItem : row.firstItem, end, 1);
    EVT_AT_ObjectDisp_1(KHIN_RYO, end, 0);
    EVT_AT_ObjectDisp_1(row.character, end, 0);
    EVT_AT_SetVoice(row.voices[5], 1, end);
    EVT_AT_MenCtrl({.frames = 10, .targetAlpha = 255, .delay = static_cast<float>(end + 70)});
    EVT_AT_SetPos(KHIN_RYO, KHIN_RYO, {0, 0, 0}, end + 80);
    EVT_AT_SetAngY(KHIN_RYO, KHIN_RYO, -90, end + 80);
    work.state = 4;
    HLib::SetupTaskDelay(end + 80);
}
} // namespace shenmue::s2
