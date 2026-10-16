#include "s2/areas/etrk_event.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/inventory.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/motion.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/step_event.hpp"
#include "s2/map_control.hpp"
#include "s2/ui.hpp"
#include "s2/game_runtime.hpp"
#include <bit>
#include <cstdlib>
namespace shenmue::s2::areas {
namespace {
// @verified - p
void PlaceEtrkKey(shen_name character, float fraction) {
    const Vector3i rotation{0, 32768, 5461};
    Matrix4x4_Push(nullptr);
    workTransformMatrix = transformMatrix_prePass;
    const std::int32_t angle = static_cast<std::int32_t>(fraction * 65535.0f);
    const std::int16_t folded = static_cast<std::int16_t>(static_cast<std::uint32_t>(angle) <= 0x8000U ? angle : -angle);
    const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000 - folded));
    const float sine = sin_fixed16(static_cast<std::int16_t>(angle));
    ApplyShearToPrePassMatrix(sine, cosine);
    Matrix4x4::preTranslate(0, 0, -.15000001f);
    const Vector3 position{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop(1);
    const float amount = 1.0f - static_cast<float>(std::abs(angle)) / 65535.0f;
    const Vector3 scale{amount, amount, amount};
    AT_SetLocalBaseChara(character, ETRK_KEY_BASE);
    EV_AT_SetPos(character, EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(character, EventTransformFlags::AllAxes, rotation, nullptr);
    SetMotionworkFlagForCharaById(character, &scale);
    EVT_AT_ObjectDisp_1(character, 0, 0);
}
std::int32_t CancelEtrkKeys(EtrkWork &work) {
    work.key = 6;
    SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::SetupTaskDelay(30);
    for (std::int32_t index = 0; index < 6; ++index)
        if (work.available[index].available)
            EVT_AT_ObjectDisp_1(work.keys[index].character, 30, 1);
    return -1;
}
// @verified - p
void OpenEtrkBoardFrame(void *data) {
    EtrkOpenWork &work = *static_cast<EtrkOpenWork *>(data);
    Vector3 position{.2f, 0, .079999998f};
    if (work.remaining <= 0)
        HLib::DestroyCurrentTask();
    else
        EV_scenApplyPosOrRotToPartOf(
            work.character, 153, &position, const_cast<Vector3i *>(&etrkOpenRotations[30 - work.remaining]), EventCharacterPartTransformMode::Absolute);
    --work.remaining;
}
} // namespace
// @verified - p
std::int32_t EtrkPrepareKeys(EtrkWork &work) {
    work.keyCount = 0;
    for (std::int32_t index = 0; index < 6; ++index) {
        if (GetItemState(work.keys[index].item)) {
            work.available[index].available = 1;
            work.available[index].ordinal = work.keyCount++;
        } else
            work.available[index].available = 0;
    }
    if (!work.keyCount) {
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetupTaskDelay(30);
        return -1;
    }
    EV_EventSetUpHelpIcon_0(0x70, 0);
    EV_DisableCharacterDisplay(ETRK_RYO);
    EVT_AT_SetMot(ETRK_RYO, ETRK_IDLE_MOTION, 0, 1, ETRK_MOTION_FLAGS, 0);
    GetMotionFrameCount(ETRK_IDLE_MOTION);
    EV_DisableCharacterDisplay(ETRK_HAND);
    EVT_AT_SetCam(1120, work.board, work.board, 0, 0, 0);
    camCtrlDisableFilmBars();
    AT_SetLocalBaseChara(ETRK_KEY_BASE, work.board);
    EVT_AT_SetPos(ETRK_KEY_BASE, work.board, Vector3{-.16f, .17f, .47f}, 0);
    EventSetRawAngle rotation{.cleanup = 0, .character = ETRK_KEY_BASE, .parent = work.board, .rotation = {0, 27306, 0}, .delay = 0};
    EVT_AT_SetAng_Callback(&rotation);
    if (!work.available[work.key].available)
        for (std::int32_t index = 0; index < 6; ++index)
            if (work.available[index].available) {
                work.key = index;
                break;
            }
    for (std::int32_t index = 0; index < 6; ++index)
        if (work.available[index].available)
            PlaceEtrkKey(work.keys[index].character,
                static_cast<float>(work.keyCount + work.available[work.key].ordinal - work.available[index].ordinal) / work.keyCount);
    work.moving = 0;
    work.remaining = 1800;
    return 1;
}
// @verified - p
std::int32_t EtrkPollKeys(EtrkWork &work) {
    const std::int32_t remaining = work.remaining--;
    if (remaining < 1)
        return CancelEtrkKeys(work);
    CheckHelpIcon(49185, 0, CheckHelpIconOp::FrameOverlay);
    if (work.moving >= 1) {
        --work.moving;
        for (std::int32_t index = 0; index < 6; ++index)
            if (work.available[index].available) {
                float fraction = static_cast<float>(work.keyCount + work.available[work.key].ordinal - work.available[index].ordinal) / work.keyCount;
                if (work.keyCount != 1) {
                    const float offset = static_cast<float>(work.moving) / static_cast<float>(10 * work.keyCount);
                    fraction += work.direction ? offset : -offset;
                }
                PlaceEtrkKey(work.keys[index].character, fraction);
            }
        GetBattleDataConfig(0, ETRK_ACCEPT);
        GetBattleDataConfig(0, ETRK_CANCEL);
        return 0;
    }
    GetBattleDataConfig(0, ETRK_ACCEPT);
    if (GetBattleDataConfig(2, ETRK_ACCEPT)) {
        const Vector3 scale{1, 1, 1};
        for (std::int32_t index = 0; index < 6; ++index)
            if (work.available[index].available) {
                EV_DisableCharacterDisplay(work.keys[index].character);
                SetMotionworkFlagForCharaById(work.keys[index].character, &scale);
            }
        return 1;
    }
    GetBattleDataConfig(0, ETRK_CANCEL);
    if (GetBattleDataConfig(2, ETRK_CANCEL))
        return CancelEtrkKeys(work);
    if (GetCurrentControlInput(2) & 0x8000U) {
        std::int32_t key = (work.key + 1) % 6;
        while (!work.available[key].available)
            key = (key + 1) % 6;
        work.key = key;
        work.direction = 0;
        work.moving = 10;
    } else if (GetCurrentControlInput(2) & 0x4000U) {
        std::int32_t key = (work.key + 5) % 6;
        while (!work.available[key].available)
            key = (key + 5) % 6;
        work.key = key;
        work.direction = 1;
        work.moving = 10;
    }
    return 0;
}
// @verified - p
std::int32_t EtrkTryKey(EtrkWork &work) {
    const shen_name key = work.keys[work.key].character;
    EV_EnableCharacterDisplay(ETRK_RYO);
    EV_DisableCharacterDisplay(ETRK_HAND);
    EVT_AT_ObjectDisp_1(key, 0, 0);
    ApplyCharacterHandRotation(FindCharacterByScenarioName(ETRK_RYO), 0, 21, &etrkPlayerHandRotation);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(ETRK_RYO), 0, reinterpret_cast<std::byte *>(etrkKeyHand), 0);
    EVT_AT_SetCam(1300, work.board, work.board, 0, 0, 0);
    EVT_AT_SetPos(ETRK_RYO, work.board, Vector3{-.074000001f, -1.3940001f, .79799998f}, 0);
    EVT_AT_SetAngY(ETRK_RYO, work.board, 0, 0);
    EVT_AT_SetMot(ETRK_RYO, ETRK_KEY_MOTION, 0, 1, ETRK_MOTION_FLAGS, 0);
    EventCling cling{.cleanup = 0,
        .target = key,
        .character = ETRK_RYO,
        .bone = 14,
        .position = {std::bit_cast<float>(0x3e1cac08U), std::bit_cast<float>(0x3d343958U), -.015f},
        .rotation = {0, 52064, 16384},
        .mode = 0,
        .delay = 0};
    EVT_AT_Cling(&cling);
    EVT_AT_SetSound(0xbf3, 30);
    const bool correct = work.keys[work.key].area.name == g_currentMap.name;
    std::int32_t duration;
    if (correct) {
        EVT_AT_SetSound(0xbf5, 65);
        EVT_AT_SetCam(1301, work.board, work.board, 0, 0, 90);
        if (g_currentMap.name == ETRK_QF00.name)
            EVT_AT_SetVoice("0304A011", 1, 90); // @text EN: "It opened!" | JA: "開いた…！"
        else if (g_currentMap.name == ETRK_QUG0.name)
            EVT_AT_SetVoice("0304A034", 1, 90); // @text EN: "Good!" | JA: "よし！"
        duration = 150;
    } else {
        EVT_AT_SetMot(ETRK_RYO, ETRK_WRONG_KEY_MOTION, 0, 1, ETRK_MOTION_FLAGS, 60);
        EVT_AT_SetSound(0xbf4, 65);
        EVT_AT_SetSound(0xbf4, 81);
        EVT_AT_SetCam(1302, work.board, work.board, 0, 0, 130);
        EVT_AT_SetMot(ETRK_RYO, ETRK_IDLE_MOTION, 0, 1, ETRK_MOTION_FLAGS, 130);
        EVT_AT_SetVoice(shenGetRandomInt32(2) ? "0304A013" : "0304A012", 1, 130); // @text 0304A013 EN: "This is the wrong key." | JA: "このカギは違うみたいだ…" ; 0304A012 EN: "The key won't fit." | JA: "ん…カギが入らないな。"
        if (work.keyCount < 6) {
            if (shenGetRandomInt32(2)) {
                EVT_AT_SetVoice("0304A015", 1, 230); // @text EN: "I should insert a different key." | JA: "他のカギを試すか。"
                EVT_AT_SetVoice("0304A018", 1, 290); // @text EN: "I'm sure there was a key left\nin that room." | JA: "まだ部屋にカギが残っていたはずだ…"
            } else {
                EVT_AT_SetVoice("0304A016", 1, 230); // @text EN: "There was a key left in the room." | JA: "まだ部屋にカギが残っていたな。"
                EVT_AT_SetVoice("0304A017", 1, 320); // @text EN: "I should go back and get it." | JA: "取りに戻るか…"
            }
            duration = 380;
        } else {
            EVT_AT_SetVoice(shenGetRandomInt32(2) ? "0304A015" : "0304A014", 1, 230); // @text 0304A015 EN: "I should insert a different key." | JA: "他のカギを試すか。" ; 0304A014 EN: "I'll try a different key." | JA: "他のカギを使ってみるか。"
            duration = 290;
        }
    }
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), ETRK_TASK);
    RegisterEventTask(task);
    EventCling *const detach = static_cast<EventCling *>(HLib::GetTaskParameter(task));
    detach->cleanup = 1;
    detach->target = key;
    detach->character = ETRK_NONE;
    detach->mode = 1;
    detach->delay = static_cast<float>(duration);
    EVT_AT_ObjectDisp_1(key, duration, 1);
    HLib::SetupTaskDelay(duration);
    return correct ? 1 : 0;
}
// @verified - p
void EtrkOpenBoard(EtrkWork &work) {
    EV_EnableCharacterDisplay(ETRK_RYO);
    EV_DisableCharacterDisplay(ETRK_HAND);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(ETRK_RYO), 0, reinterpret_cast<std::byte *>(etrkOpenHand), 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(ETRK_RYO), 0, reinterpret_cast<std::byte *>(etrkReturnHand), 40);
    ApplyCharacterHandRotation(FindCharacterByScenarioName(ETRK_RYO), 0, 21, &etrkPlayerHandRotation);
    EVT_AT_SetCam(1310, work.board, work.board, 0, 0, 0);
    EVT_AT_SetPos(ETRK_RYO, work.board, Vector3{-.11f, -1.355f, .73000002f}, 0);
    EVT_AT_SetAngY(ETRK_RYO, work.board, 0, 0);
    EVT_AT_SetMot(ETRK_RYO, ETRK_OPEN_MOTION, 0, 1, ETRK_MOTION_FLAGS, 0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(OpenEtrkBoardFrame, 4, 0x0b, sizeof(EtrkOpenWork), ETRK_TASK);
    EtrkOpenWork *const opening = static_cast<EtrkOpenWork *>(HLib::GetTaskParameter(task));
    opening->remaining = 30;
    opening->character = work.board;
    HLib::SetupTimedTask(task, 54);
    EVT_AT_SetSound(3062, 54);
    std::int32_t duration = 114;
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, static_cast<std::int16_t>(work.keys[work.key].flag))) {
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(work.keys[work.key].flag), 1);
        EVT_AT_SetCam(1311, work.board, work.board, 0, 0, 114);
        EVT_AT_SetPos(ETRK_RYO, work.board, Vector3{-.11f, -1.355f, .80500001f}, 114);
        EVT_AT_SetAngY(ETRK_RYO, work.board, 0, 114);
        EVT_AT_SetMot(ETRK_RYO, ETRK_IDLE_MOTION, 0, 1, ETRK_MOTION_FLAGS, 114);
        if (g_currentMap.name == ETRK_QF00.name) {
            EVT_AT_SetVoice("0304A050", 1, 114); // @text EN: "Yes!\nHere's the switch!" | JA: "あった！　スイッチだ！"
            EVT_AT_SetVoice("0304A051", 1, 194); // @text EN: "Now I can get the elevator moving!" | JA: "これでエレベーターが動かせる！"
            duration = 274;
        } else if (g_currentMap.name == ETRK_QUG0.name) {
            EVT_AT_SetVoice("0304A035", 1, 114); // @text EN: "Switches=@" | JA: "スイッチだ…"
            duration = 274;
        }
    }
    HLib::SetupTaskDelay(duration);
}
} // namespace shenmue::s2::areas
