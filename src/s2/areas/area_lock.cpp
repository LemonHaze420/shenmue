#include "s2/areas/area_lock.hpp"
#include "s2/pak.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/motion.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include <cmath>
#include "s2/screen_film.hpp"
#include "s2/sk_event.hpp"

namespace shenmue::s2::areas {
namespace {
constexpr shen_name hgnCharacters[3]{LOCK_PLAYER, LOCK_HGN, LOCK_INVALID};
constexpr shen_name wtbCharacters[3]{LOCK_PLAYER, LOCK_WTB, LOCK_INVALID};
constexpr shen_name sf8Characters[3]{LOCK_PLAYER, LOCK_SF8, LOCK_INVALID};
constexpr shen_name playerCharacters[2]{LOCK_PLAYER, LOCK_INVALID};
constexpr Vector3 basePositions[3]{{225.94000244140625f, 139.27000427246094f, 84.6500015258789f}, {288.79998779296875f, 99.66000366210938f, 119.9000015258789f},
    {258.0199890136719f, 100.0f, 215.60000610351562f}};
constexpr Vector3 relativePlayerPositions[3]{{-0.5f, 0, 1}, {3.0f, -0.2f, 1}, {1, 0, 1.5f}};
constexpr float relativePlayerAngles[3]{50, 70, 165};
constexpr float opponentAngles[3]{240, 250, 220};
std::int32_t baseAngles[3];
Vector3 hgnRelativePosition, sf8RelativePosition;
constexpr Vector3 wtbRelativePosition{0, 0.2f, 0};
struct ExitPlacement {
    Vector3 start, finish;
    std::uint32_t yaw;
    float distance;
};
static_assert(sizeof(ExitPlacement) == 32);
constexpr ExitPlacement exitPlacements[7]{
    {{223.8000030517578f, 139.60000610351562f, 85.5999984741211f}, {230.60000610351562f, 139.60000610351562f, 85}, 49152, 2.5f},
    {{294, 100, 120.5f}, {284.29998779296875f, 100, 120.5f}, 16384, 2},
    {{258.70001220703125f, 99.9800033569336f, 218.1999969482422f}, {258.6000061035156f, 99.9800033569336f, 212.39999389648438f}, 0, 2.5f},
    {{0, 0, 0}, {0, 0, 0}, 0, 0}, {{313.6000061035156f, 100, 101.4000015258789f}, {310.70001220703125f, 100, 104}, 25486, 2},
    {{265.4700012207031f, 95.5f, 53.79999923706055f}, {265.3999938964844f, 95.5f, 59.41999816894531f}, 32768, 2},
    {{159.89999389648438f, 103.5999984741211f, 120.5999984741211f}, {164.10000610351562f, 103.5999984741211f, 120.5f}, 49152, 2}};
} // namespace
// @verified - p
void AreaLOCKInit() {
}
// @verified - p
void AreaLOCKMain(std::uintptr_t parameter) {
    const std::int32_t entry = static_cast<std::int32_t>(parameter);
    SetPlayerControlEnabled(0);
    mtwkRequestMotion(scenFindMotionwork(LOCK_PLAYER), 0xf002, MotionRegion::LowerBody);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(LOCKMainCallback, 4, 0x0b, sizeof(LockEventWork), LOCK_TASK);
    LockEventWork *const work = static_cast<LockEventWork *>(HLib::GetTaskParameter(task));
    work->state = 0;
    work->entry = 0;
    work->admission = nullptr;
    work->task = task;
    work->opponent = {};
    work->entry = entry;
    if (entry == 0) {
        work->admission = Queue_EVT_AT_ReadChr(task, hgnCharacters);
        work->opponent = LOCK_HGN;
    } else if (entry == 1) {
        work->admission = Queue_EVT_AT_ReadChr(task, wtbCharacters);
        work->opponent = LOCK_WTB;
    } else if (entry == 2) {
        work->admission = Queue_EVT_AT_ReadChr(task, sf8Characters);
        work->opponent = LOCK_SF8;
    } else {
        work->admission = Queue_EVT_AT_ReadChr(task, playerCharacters);
        work->opponent = {};
    }
    LOCKPrepareEvent();
}
// @verified - p
void LOCKPrepareEvent() {
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("LOCK"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("LOCK"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, LOCK_BASE);
    EV_FreeAuthorPak(1, "0363");
    FinishLoading();
    SetRenderClockAndCompassTo(1);
}
// @verified - p
void LOCKPlaceCharacters(LockEventWork *work) {
    if (work->opponent.name) {
        const std::int32_t index = work->entry == 0 ? 0 : work->entry == 2 ? 2 : 1;
        EVT_AT_SetPos(LOCK_BASE, LOCK_INVALID, basePositions[index], 0);
        EVT_AT_SetAngY(LOCK_BASE, LOCK_INVALID, static_cast<float>(baseAngles[index]), 0);
        AT_SetLocalBaseChara(LOCK_PLAYER, LOCK_BASE);
        AT_SetLocalBaseChara(work->opponent, LOCK_BASE);
        if (work->entry == 0)
            EVT_AT_SetPos(work->opponent, LOCK_BASE, hgnRelativePosition, 0);
        else if (work->entry == 1)
            EVT_AT_SetPos(work->opponent, LOCK_BASE, wtbRelativePosition, 0);
        else if (work->entry == 2)
            EVT_AT_SetPos(work->opponent, LOCK_BASE, sf8RelativePosition, 0);
        if (work->entry >= 0 && work->entry < 3)
            EVT_AT_SetAngY(work->opponent, LOCK_BASE, opponentAngles[work->entry], 0);
    }
    EVT_AT_SetMot(LOCK_PLAYER, 0xf002, 0, 1, 0x40000000U, 0);
    GetMotionFrameCount(0xf002);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("LOCK"), 0);
}
// @verified - p
void LOCKPlacePlayer(LockEventWork *work) {
    if (work->entry != 0 && work->entry != 1 && work->entry != 2)
        return;
    EVT_AT_SetPos(LOCK_PLAYER, LOCK_BASE, relativePlayerPositions[work->entry], 0);
    EVT_AT_SetAngY(LOCK_PLAYER, LOCK_BASE, relativePlayerAngles[work->entry], 0);
}
// @verified - p
std::int32_t LOCKHgnConversation(LockEventWork *work) {
    LOCKPlacePlayer(work);
    EVT_AT_SetMot(work->opponent, 11009, 0, 1, 0x40000000U, 0);
    GetMotionFrameCount(11009);
    Vector3 position;
    EV_GetCharaPosition(LOCK_PLAYER, 5, &position, EventTransformFlags::World);
    UpdateMotionWorkWithPosition(scenFindMotionwork(work->opponent), &position, 0);
    EV_GetCharaPosition(work->opponent, 5, &position, EventTransformFlags::World);
    UpdateMotionWorkWithPosition(scenFindMotionwork(LOCK_PLAYER), &position, 0);
    EV_GetCharaPosition(LOCK_PLAYER, 7, &position, EventTransformFlags::World);
    SetEyeToChara(FindCharacterByScenarioName(work->opponent), &position, 15, 2);
    EV_GetCharaPosition(work->opponent, 7, &position, EventTransformFlags::World);
    SetEyeToChara(FindCharacterByScenarioName(LOCK_PLAYER), &position, 15, 2);
    const bool storyFlag = EV_GetShenmueFlag(0x288) != 0;
    const std::int32_t first = static_cast<std::int32_t>(GetRandomNumberInRange(3.0f));
    if (storyFlag)
        EVT_AT_SetVoice(first == 0 ? "0363B001" : first == 1 ? "0363B002" : "0363B003", 1, 0); // @text 0363B001 EN: "Hey, wait!" | JA: "おい、待て！" ; 0363B002 EN: "Hey, stop!" | JA: "おい、止まれ！" ; 0363B003 EN: "You, wait!" | JA: "待て、おまえ！"
    else
        EVT_AT_SetVoice(first == 0 ? "0363B007" : first == 1 ? "0363B008" : "0363B009", 1, 0); // @text 0363B007 EN: "Wait up!" | JA: "待ちな、兄ちゃん！" ; 0363B008 EN: "Hold it!" | JA: "ちょっと待ちな！" ; 0363B009 EN: "Stop!" | JA: "そこで止まれ！"
    const std::int32_t second = static_cast<std::int32_t>(GetRandomNumberInRange(3.0f));
    if (storyFlag)
        EVT_AT_SetVoice(second == 0 ? "0363B004" : second == 1 ? "0363B005" : "0363B006", 1, 60); // @text 0363B004 EN: "Can't go in." | JA: "勝手に入るな。" ; 0363B005 EN: "This place is off limits." | JA: "ここは立ち入り禁止だ。" ; 0363B006 EN: "You can't go further." | JA: "その先に行くんじゃない。"
    else
        EVT_AT_SetVoice(second == 0 ? "0363B010" : second == 1 ? "0363B011" : "0363B012", 1, 60); // @text 0363B010 EN: "You're asking for trouble." | JA: "勝手に入られると困るんだよ。" ; 0363B011 EN: "You gotta pay to go through." | JA: "入るんなら金を払ってくれ。" ; 0363B012 EN: "Gotta pay to go in!" | JA: "金を払わねえうちは\n通せねえぜ。"
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("LOCK"), 0);
    EVT_AT_SetCam(2068, work->opponent, work->opponent, 0, 0, 0);
    camCtrlDisableFilmBars();
    return 150;
}
// @verified - p
std::int32_t LOCKWtbConversation(LockEventWork *work) {
    LOCKPlacePlayer(work);
    EVT_AT_SetMot(work->opponent, 11009, 0, 1, 0x40000000U, 0);
    GetMotionFrameCount(11009);
    Vector3 position;
    EV_GetCharaPosition(LOCK_PLAYER, 5, &position, EventTransformFlags::World);
    UpdateMotionWorkWithPosition(scenFindMotionwork(work->opponent), &position, 0);
    EV_GetCharaPosition(work->opponent, 5, &position, EventTransformFlags::World);
    UpdateMotionWorkWithPosition(scenFindMotionwork(LOCK_PLAYER), &position, 0);
    EV_GetCharaPosition(LOCK_PLAYER, 7, &position, EventTransformFlags::World);
    SetEyeToChara(FindCharacterByScenarioName(work->opponent), &position, 15, 2);
    EV_GetCharaPosition(work->opponent, 7, &position, EventTransformFlags::World);
    SetEyeToChara(FindCharacterByScenarioName(LOCK_PLAYER), &position, 15, 2);
    const bool storyFlag = EV_GetShenmueFlag(0x287) != 0;
    const std::int32_t first = static_cast<std::int32_t>(GetRandomNumberInRange(3.0f));
    if (storyFlag)
        EVT_AT_SetVoice(first == 0 ? "0363D001" : first == 1 ? "0363D002" : "0363D003", 1, 0); // @text 0363D001 EN: "Wait!" | JA: "おい、待て！" ; 0363D002 EN: "Stop!" | JA: "おい、止まれ！" ; 0363D003 EN: "You just wait!" | JA: "待てよ、おまえ！"
    else
        EVT_AT_SetVoice(first == 0 ? "0363D007" : first == 1 ? "0363D008" : "0363D009", 1, 0); // @text 0363D007 EN: "Wait up fella!" | JA: "待ちな、兄ちゃん！" ; 0363D008 EN: "Stop right there!" | JA: "ちょっと待った！" ; 0363D009 EN: "Don't walk in." | JA: "おいおい、ダメだぜ。"
    const std::int32_t second = static_cast<std::int32_t>(GetRandomNumberInRange(3.0f));
    if (storyFlag)
        EVT_AT_SetVoice(second == 0 ? "0363D004" : second == 1 ? "0363D005" : "0363D006", 1, 60); // @text 0363D004 EN: "You can't just walk in." | JA: "勝手に入るな。" ; 0363D005 EN: "Keep out!" | JA: "その先に行くんじゃない。" ; 0363D006 EN: "This place is off limits." | JA: "ここは立ち入り禁止だ。"
    else
        EVT_AT_SetVoice(second == 0 ? "0363D010" : second == 1 ? "0363D011" : "0363D012", 1, 60); // @text 0363D010 EN: "You're asking for trouble." | JA: "勝手に入られると困るんだよ。" ; 0363D011 EN: "Pay, if you wanna go in." | JA: "入るんなら金を払ってくれ。" ; 0363D012 EN: "You can't go in\nif you don't pay." | JA: "金を払わねえうちは\n通せねえな。"
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("LOCK"), 0);
    EVT_AT_SetCam(2060, work->opponent, work->opponent, 0, 0, 0);
    camCtrlDisableFilmBars();
    return 150;
}
// @verified - p
std::int32_t LOCKSf8Conversation(LockEventWork *work) {
    LOCKPlacePlayer(work);
    EVT_AT_SetMot(work->opponent, 11009, 0, 1, 0x40000000U, 0);
    GetMotionFrameCount(11009);
    Vector3 position;
    EV_GetCharaPosition(LOCK_PLAYER, 5, &position, EventTransformFlags::World);
    UpdateMotionWorkWithPosition(scenFindMotionwork(work->opponent), &position, 0);
    EV_GetCharaPosition(work->opponent, 5, &position, EventTransformFlags::World);
    UpdateMotionWorkWithPosition(scenFindMotionwork(LOCK_PLAYER), &position, 0);
    EV_GetCharaPosition(LOCK_PLAYER, 7, &position, EventTransformFlags::World);
    SetEyeToChara(FindCharacterByScenarioName(work->opponent), &position, 15, 2);
    EV_GetCharaPosition(work->opponent, 7, &position, EventTransformFlags::World);
    SetEyeToChara(FindCharacterByScenarioName(LOCK_PLAYER), &position, 15, 2);
    const bool storyFlag = EV_GetShenmueFlag(0x2bd) != 0;
    const std::int32_t first = static_cast<std::int32_t>(GetRandomNumberInRange(3.0f));
    if (storyFlag)
        EVT_AT_SetVoice(first == 0 ? "0363C001" : first == 1 ? "0363C002" : "0363C003", 1, 0); // @text 0363C001 EN: "Wait!" | JA: "おい、待て！" ; 0363C002 EN: "Stop!" | JA: "おい、止まれ！" ; 0363C003 EN: "Wait up fella!" | JA: "待ちな、兄ちゃん！"
    else
        EVT_AT_SetVoice(first == 0 ? "0363C007" : first == 1 ? "0363C008" : "0363C009", 1, 0); // @text 0363C007 EN: "Wait up fella!" | JA: "待ちな、兄ちゃん！" ; 0363C008 EN: "Hold it!" | JA: "ちょっと待った！" ; 0363C009 EN: "Hey, where do you think you're going?" | JA: "おいおい、どこ行くんだ。"
    const std::int32_t second = static_cast<std::int32_t>(GetRandomNumberInRange(3.0f));
    if (storyFlag)
        EVT_AT_SetVoice(second == 0 ? "0363C004" : second == 1 ? "0363C005" : "0363C006", 1, 60); // @text 0363C004 EN: "You can't just walk in." | JA: "勝手に入るなよ。" ; 0363C005 EN: "You can't go any further." | JA: "そこから先に行くな。" ; 0363C006 EN: "This place is off limits." | JA: "ここは立ち入り禁止だ。"
    else
        EVT_AT_SetVoice(second == 0 ? "0363C010" : second == 1 ? "0363C011" : "0363C012", 1, 60); // @text 0363C010 EN: "You're asking for trouble." | JA: "勝手に入られると困るんだよ。" ; 0363C011 EN: "Pay, if you wanna go in." | JA: "入るんなら金を払いな。" ; 0363C012 EN: "You can't go in\nif you don't pay." | JA: "金を払わねえうちは\n通せねえぜ。"
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("LOCK"), 0);
    EVT_AT_SetCam(2064, LOCK_BASE, LOCK_BASE, 0, 0, 0);
    camCtrlDisableFilmBars();
    return 155;
}
// @verified - p
void LOCKReleaseEvent(LockEventWork *work) {
    ET_AT_KillWork(work->admission);
    SetPlayerControlEnabled(0);
    SetCharaKill(LOCK_BASE, 1);
    SetMotionRegionFlag(scenFindMotionwork(LOCK_PLAYER), 71, 0, MotionRegion::LowerBody);
    EV_FreeAuthorPak(0, "0363");
    EV_NPC_NewLoadSw(1);
    SetRenderClockAndCompassTo(0);
    CleanupMapControlCharacters();
    HLib::EV_HLKillOtherHierarchyTask(work->task);
    SetPlayerControlEnabled(1);
    camCtrlSetState(CameraState::Init);
    camCtrlSetFovTransitionTo(true);
}
// @verified - p
void LOCKMainCallback(void *data) {
    LockEventWork *const work = static_cast<LockEventWork *>(data);
    switch (work->state) {
    case 0:
        LOCKPlaceCharacters(work);
        SetScreenFilmFade(15, 255, 0, 0, 0, 0, 0, 0, 0);
        ++work->state;
        return;
    case 1:
        if (work->entry == 0) {
            const std::int32_t delay = LOCKHgnConversation(work);
            ++work->state;
            HLib::SetupTaskDelay(delay);
        } else if (work->entry == 1) {
            const std::int32_t delay = LOCKWtbConversation(work);
            ++work->state;
            HLib::SetupTaskDelay(delay);
        } else if (work->entry == 2) {
            const std::int32_t delay = LOCKSf8Conversation(work);
            ++work->state;
            HLib::SetupTaskDelay(delay);
        } else {
            std::int32_t delay = 1;
            if (work->entry == 4 || work->entry == 5 || work->entry == 6) {
                HLib::Task *const task = HLib::EnqueueTaskWithParameter(SkEventMessageBeginCallback, 6, 0x0b, 16, EVENT_CHARACTER_TASK);
                SkEventMessageWork *const message = static_cast<SkEventMessageWork *>(HLib::GetTaskParameter(task));
                message->message = "勝手に入るな";
                message->frames = 60;
                delay = 60;
            }
            ++work->state;
            HLib::SetupTaskDelay(delay);
        }
        return;
    case 2: {
        if (work->opponent.name) {
            UpdateMotionWorkWithPosition(scenFindMotionwork(LOCK_PLAYER), nullptr, 0);
            UpdateMotionWorkWithPosition(scenFindMotionwork(work->opponent), nullptr, 0);
            SetEyeToChara(FindCharacterByScenarioName(LOCK_PLAYER), nullptr, 16, 0);
            SetEyeToChara(FindCharacterByScenarioName(work->opponent), nullptr, 16, 0);
            EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("LOCK"), 0);
            if (work->entry == 0)
                EVT_AT_SetCam(2069, LOCK_BASE, LOCK_BASE, 0, 0, 0);
            else if (work->entry == 1)
                EVT_AT_SetCam(2061, work->opponent, work->opponent, 0, 0, 0);
            else if (work->entry == 2)
                EVT_AT_SetCam(2065, LOCK_BASE, LOCK_BASE, 0, 0, 0);
            if (work->entry >= 0 && work->entry < 3)
                camCtrlDisableFilmBars();
        }
        EV_SetCharaControllerFlags(LOCK_PLAYER, ApplyGravity, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(LOCK_PLAYER, UpdateHeight, NpcCharacterControllerState::Enabled);
        Vector3 position;
        EV_GetCharaPosition(LOCK_PLAYER, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        const ExitPlacement &placement = exitPlacements[work->entry];
        const float deltaX = placement.finish.x - placement.start.x;
        const float deltaZ = placement.finish.z - placement.start.z;
        const float length = std::sqrt(deltaZ * deltaZ + deltaX * deltaX);
        const Vector3 target{(placement.distance * deltaX) / length + position.x, position.y, (deltaZ * placement.distance) / length + position.z};
        CopyMotionTransition(scenFindMotionwork(LOCK_PLAYER), &target, static_cast<std::uint16_t>(placement.yaw), 0, 0);
        ++work->state;
        [[fallthrough]];
    }
    case 3:
        if (!mtwkSetExtraFlag(scenFindMotionwork(LOCK_PLAYER), 58, 1)) {
            LOCKReleaseEvent(work);
            ++work->state;
        }
        return;
    default:
        return;
    }
}
const ScriptAreaDefinition areaLOCKScriptDefinition{.areaId = AREA_LOCK, .field04 = 0, .init = AreaLOCKInit, .main = AreaLOCKMain};
} // namespace shenmue::s2::areas
