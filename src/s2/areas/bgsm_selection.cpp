#include "s2/areas/area_bgsm.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/conversation.hpp"
#include "s2/game_runtime.hpp"
#include "s2/scene_data.hpp"
#include "s2/input.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <cstring>
#include <cmath>
namespace shenmue::s2::areas {
void BgsmSelectSet(void *data) {
    BgsmWork &work = *static_cast<BgsmWork *>(data);
    EV_EnableCharacterDisplay(BGSM_CASH);
    EventCling cling{.cleanup = 0,
        .target = BGSM_CASH,
        .character = BGSM_HAND_BASE,
        .bone = 14,
        .position = {-.01f, .064f, .075f},
        .rotation = {0, 36864, 36864},
        .mode = 0,
        .delay = 0};
    EVT_AT_Cling(&cling);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(BgsmSelectMove, 4, 0x0b, sizeof(BgsmSelectionWork), BGSM_TASK);
    BgsmSelectionWork *selection = static_cast<BgsmSelectionWork *>(HLib::GetTaskParameter(task));
    selection->index = 0;
    selection->requested = 0;
    selection->state = 2;
    selection->board = work.board;
    work.selection = task;
    work.elapsed = 0;
    CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
    CheckHelpIcon(32, 0, CheckHelpIconOp::Set);
    BgsmMotion(work.opponent, BGSM_TALK_MOTIONS[bgsmOpponentPose], 0x40000000U);
    BgsmHand(BGSM_HAND, bgsmBetHand, 0);
    BgsmPosition(BGSM_HAND_BASE, BGSM_SPOTS[selection->index], {});
    BgsmRawAngle(BGSM_HAND_BASE, BGSM_SPOTS[selection->index], {0, 5461, 0});
    BgsmPosition(BGSM_CAMERA_BASE, BGSM_HAND_BASE, {});
    BgsmRawAngle(BGSM_CAMERA_BASE, BGSM_HAND_BASE, {});
    EV_DisableCharacterDisplay(BGSM_RYO);
    EV_EnableCharacterDisplay(BGSM_HAND);
    BgsmCamera(4400, BGSM_CAMERA_BASE);
    HLib::SetCurrentTaskTo(BgsmSelectBet);
}
void BgsmSelectBet(void *data) {
    BgsmWork &work = *static_cast<BgsmWork *>(data);
    BgsmSelectionWork &selection = *static_cast<BgsmSelectionWork *>(HLib::GetTaskParameter(work.selection));
    const BgsmChoice &choice = BGSM_CHOICES[work.board][selection.index];
    if (work.elapsed++ > 10800) {
        ctrlStopSubtitles();
        CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
        HLib::DestroyTaskInternal(work.selection);
        BgsmCamera(2017);
        BgsmPlayVoice(0, "008");
        EV_DisableCharacterDisplay(BGSM_HAND);
        EV_EnableCharacterDisplay(BGSM_RYO);
        HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_StartMoneyCount_Callback, 6, 0x0b, sizeof(EventMoneyCount), BGSM_TASK);
        RegisterEventTask(task);
        EventMoneyCount *cash = static_cast<EventMoneyCount *>(HLib::GetTaskParameter(task));
        cash->cleanup = 1;
        cash->cash = work.wager;
        cash->delay = 40;
        HLib::SetCurrentTaskTo(BgsmStop);
        HLib::SetupTaskDelay(80);
        return;
    }
    GetBattleDataConfig(0, BGSM_BATTLE_INPUT);
    const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
    std::int8_t next;
    switch (input) {
    case 0x400:
        if (selection.state != 2)
            return;
        ctrlStopSubtitles();
        CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
        CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
        BgsmHand(BGSM_HAND, bgsmReleaseHand, 8);
        {
            EventCling cling{.cleanup = 0, .target = BGSM_CASH, .character = {}, .bone = 0, .position = {}, .rotation = {}, .mode = 1, .delay = 0};
            EVT_AT_Cling(&cling);
        }
        BgsmAppendVoice(0, shenGetRandomInt32(2) ? "006" : "007");
        {
            static constexpr const char *voices[]{"021", "022", "023"};
            BgsmAppendVoice(1, voices[shenGetRandomInt32(3)]);
        }
        work.choice = choice.combination;
        work.chosenFace = choice.face;
        work.elapsed = 0;
        HLib::EnqueueTaskWithoutParameter(BgsmFallMoney, 4, 0x0b, static_cast<std::int32_t>(BGSM_MONEY_TASK.name));
        HLib::DestroyTaskInternal(work.selection);
        HLib::SetCurrentTaskTo(BgsmShakePot);
        BgsmQueueVoices(work.self, 0x20000000U);
        return;
    case 0x1000:
        next = choice.up;
        break;
    case 0x2000:
        next = choice.down;
        break;
    case 0x4000:
        next = choice.left;
        break;
    case 0x8000:
        next = choice.right;
        break;
    default:
        return;
    }
    if (next != -1) {
        selection.index = next;
        selection.state = 0;
        selection.requested = -1;
    }
}
void BgsmFallMoney(void *) {
    AT_SetLocalBaseChara(BGSM_CASH, BGSM_BASE);
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(BGSM_CASH, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    GetScenarioCharacterEulerAngles(BGSM_CASH, &rotation, EventTransformFlags::Local);
    rotation.x = static_cast<std::int32_t>((static_cast<std::uint16_t>(rotation.x) + 32768) * 0.1 + 32768);
    rotation.z = static_cast<std::int32_t>(static_cast<std::int16_t>(rotation.z) * 0.1);
    position.y -= .007f;
    if (position.y < .76f) {
        Loop3DSoundEffect(0x100);
        position.y = .76f;
        HLib::DestroyCurrentTask();
    }
    EV_AT_SetPos(BGSM_CASH, EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(BGSM_CASH, EventTransformFlags::AllAxes, rotation);
}
void BgsmSelectMove(void *data) {
    BgsmSelectionWork &work = *static_cast<BgsmSelectionWork *>(data);
    if (work.state) {
        if (work.state != 1)
            return;
        Vector3 position, target;
        EV_GetCharaPosition(BGSM_HAND_BASE, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        EV_GetCharaPosition(BGSM_SPOTS[work.index], CHARACTER_TRANSLATION_ROOT, &target, EventTransformFlags::World);
        position.y = target.y;
        const float dx = position.x - target.x, dz = position.z - target.z;
        if (std::sqrt(dx * dx + dz * dz) <= .01f)
            ++work.state;
        else {
            position.x += (target.x - position.x) * .3f;
            position.z += (target.z - position.z) * .3f;
            EV_AT_SetPos(BGSM_HAND_BASE, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
        }
        BgsmPosition(BGSM_CAMERA_BASE, BGSM_HAND_BASE, {});
        return;
    }
    const BgsmChoice &choice = BGSM_CHOICES[work.board][work.index];
    if (choice.face == -1) {
        const std::string_view label = GetLocalizedSceneText(BGSM_LABELS[choice.combination]);
        std::strcpy(bgsmMessage, label.empty() ? "" : label.data());
    } else {
        const std::string_view label = GetLocalizedSceneText(choice.combination ? "UIBOSYIDPO" : "UIBOSYIDWS"); // @text UIBOSYIDPO EN: "Yidian Pair of" | JA: "三～十" ; UIBOSYIDWS EN: "Yidian Wai Sik of" | JA: "三～十"
        std::strcpy(bgsmMessage, label.empty() ? "" : label.data());
        std::strcat(bgsmMessage, " ");
        const std::string_view number = GetLocalizedSceneText(BGSM_NUMBER_LABELS[choice.face]);
        std::strcat(bgsmMessage, number.empty() ? "" : number.data());
    }
    if (((ResetSceneTextLanguage() - 1) & 0xfffffffbU) != 0) {
        ctrlStopSubtitles();
        MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        SCEN_ShowEventMessage(control->scenarioTask, 0, bgsmMessage);
    }
    ++work.state;
}
} // namespace shenmue::s2::areas
