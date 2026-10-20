#include "s2/areas/area_qdss.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/door.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/inventory.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/qdsa.hpp"
#include "s2/screen_film.hpp"
#include "s2/search.hpp"
#include "s2/step_event.hpp"
#include <bit>
#include <cstring>
namespace shenmue::s2::areas {
namespace {
constexpr float F(std::uint32_t bits) {
    return std::bit_cast<float>(bits);
}
const QdssKey initialKeys[8]{
    {.position = {F(0x3d27ef9e), F(0x3fba7efa), F(0xbf9851ec)}, .rotation = {0, 32768, 0}, .flag = 1059, .item = 71, .character = SHEN_NAME(_ky0)},
    {.position = {F(0x3db43958), F(0x3fc16873), F(0xbf9851ec)}, .rotation = {0, 32768, 0}, .flag = 1058, .item = 72, .character = SHEN_NAME(_ky1)},
    {.position = {F(0x3e0d4fdf), F(0x3fba7efa), F(0xbf9851ec)}, .rotation = {0, 32768, 0}, .flag = 1060, .item = 73, .character = SHEN_NAME(_ky2)},
    {.position = {F(0x3db43958), F(0x3faf9db2), F(0xbf9851ec)}, .rotation = {0, 32768, 0}, .flag = 1332, .item = 77, .character = SHEN_NAME(_ky3)},
    {.item = 78, .character = SHEN_NAME(_ky4)}, {.item = 76, .character = SHEN_NAME(_ky5)}, {.item = 79, .character = SHEN_NAME(_ky6)},
    {.item = 74, .character = SHEN_NAME(_ky7)}};
const shen_name shrineActors[]{QDSS_REN, QDSS_INVALID};
void PrepareShrineOwner() {
    SetPlayerControlEnabled(0);
    EV_SetCharaControllerFlags(QDSS_RYO, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(QDSS_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    SetPlayerGameFlags(PlayerGameFlagMode::Bit2, nullptr);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QDSS"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QDSS"), 0);
    EV_AT_SetPos(QDSS_BASE, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3{171.5f, 136, 64.800003f});
    EV_AT_SetAngY(QDSS_BASE, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, 49152, 0});
    std::memcpy(qdssKeys, initialKeys, sizeof(initialKeys));
}
void RemoveShrineItems(QdssWork &work) {
    for (std::int32_t index = 0; index < 8; ++index)
        if (GetItemState(work.keys[index].item))
            RemoveInventoryItem(work.keys[index].item, 1);
    if (GetItemState(10))
        RemoveInventoryItem(10, 1);
}
void PrepareShrineReturn(QdssWork &work) {
    using enum CameraState;
    for (std::int32_t index = 0; index < 4; ++index) {
        QdssKey &key = work.keys[index];
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, static_cast<std::int16_t>(key.flag))) {
            Vector3 position = key.position;
            position.z += -.14999999f;
            key.rotation.z = 57344;
            AT_SetLocalBaseChara(key.character, QDSS_BASE);
            EV_AT_SetPos(key.character, EventTransformFlags::AllAxes, position);
            EV_AT_SetAngY(key.character, EventTransformFlags::AllAxes, key.rotation);
            EV_EnableCharacterDisplay(key.character);
        } else
            EV_DisableCharacterDisplay(key.character);
    }
    SetHighModelXYZ(ctrlGetCharaByName(QDSS_RYO), 0, 1, 0);
    SetHighModelXYZ(ctrlGetCharaByName(QDSS_RYO), 0, 0, 0);
    EV_EnableCharacterDisplay(QDSS_RYO);
    EVT_AT_SetMot(QDSS_RYO, 0xf003, 0, 1, 0x40000000U, 0);
    GetMotionFrameCount(0xf003);
    EV_SetRefBoardParamsFor(QDSS_RYO, 0);
    EV_EventSetUpHelpIcon_0(0, 0);
    CameraUtilityParameters camera{.first = 1, .second = 0};
    camCtrlUtil(CameraUtilityMode::ForceSearchCam, reinterpret_cast<std::intptr_t>(&camera));
    if (EV_GetShenmueFlag(0x3c9))
        EV_DisableCharacterDisplay(QDSS_REN);
    else {
        EVT_AT_SetPos(QDSS_REN, QDSS_BASE, Vector3{F(0x40dc7ae1), 0, F(0xbec28f5c)}, 0);
        EVT_AT_SetAngY(QDSS_REN, QDSS_BASE, 90, 0);
        EVT_AT_SetMot(QDSS_REN, 0xf003, 0, 1, 0x40000000U, 0);
        GetMotionFrameCount(0xf003);
        EV_EnableCharacterDisplay(QDSS_REN);
        EV_SetCharaControllerFlags(QDSS_REN, ApplyGravity, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(QDSS_REN, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
        EV_SetRefBoardParamsFor(QDSS_REN, 0);
        EV_SetCharacterAlphaEnabled(QDSS_REN, true);
        SetNpcRouteYaw(QDSS_REN, 0);
        EV_SetCharaControllerFlags(QDSS_REN, EvaluateMotion | SolverAssist, NpcCharacterControllerState::Enabled);
    }
    EV_DisableCharacterDisplay(QDSS_HAND);
    SetHighModelXYZ(ctrlGetCharaByName(QDSS_HAND), 0, 1, 0);
    SetHighModelXYZ(ctrlGetCharaByName(QDSS_HAND), 0, 0, 0);
    ClaimDoor(SHEN_NAME(R140), QDSS_REN);
    srchSetLookMode();
    camCtrlSetState(Init);
    camCtrlSetFovTransitionTo(true);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    HLib::SetupTaskDelay(30);
}
void RestoreShrineControls() {
    SetPlayerControlEnabled(1);
    EV_SetCharaControllerFlags(QDSS_RYO, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(QDSS_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    SetPlayerGameFlags(PlayerGameFlagMode::Bit2, reinterpret_cast<void *>(1));
    AT_SetLocalBaseChara(QDSS_RYO, QDSS_INVALID);
    AT_SetLocalBaseChara(QDSS_HAND, QDSS_INVALID);
    for (const QdssKey &key : qdssKeys)
        AT_SetLocalBaseChara(key.character, QDSS_INVALID);
    AT_SetLocalBaseChara(QDSS_KEYS_BASE, QDSS_INVALID);
    AT_SetLocalBaseChara(QDSS_CAMERA_BASE, QDSS_INVALID);
    AT_SetLocalBaseChara(QDSS_BASE, QDSS_INVALID);
}
void ShrineIntroduction() {
    EV_DisableCharacterDisplay(QDSS_HAND);
    EV_DisableCharacterDisplay(QDSS_RYO);
    EVT_AT_SetMot(QDSS_RYO, 0xf003, 0, 1, 0x40000000U, 0);
    EVT_AT_SetCam(1020, QDSS_BASE, QDSS_BASE, 0, 0, 0);
    if (EV_GetShenmueFlag(0x37a))
        return;
    std::int32_t duration;
    if (EV_GetShenmueFlag(0x2e4)) {
        EVT_AT_SetVoice("0247A039", 0); // @text EN: "A statue of Buddha=@" | JA: "仏像か…"
        EVT_AT_SetVoice("0247A040", 55); // @text EN: "Hm?" | JA: "んっ？"
        EVT_AT_SetVoice("0247A041", 105); // @text EN: "There are holes in the face.\nLooks like a keyhole=@" | JA: "顔に穴が開いているな。\n何かを差し込めそうだ…"
        duration = 215;
        EV_stepSetFlags(0, 0x2e4);
    } else {
        const std::int32_t choice = shenGetRandomInt32(3);
        EVT_AT_SetVoice(choice == 0 ? "0247A185" : choice == 1 ? "0247A186" : "0247A187", 0); // @text 0247A185 EN: "There are four holes in the face.\nLike keyholes=@" | JA: "顔に４つ穴が開いているな。\n何かを差し込めそうだ…" ; 0247A186 EN: "Might be some sort of\nmechanism in this statue." | JA: "この仏像には、\n何かしかけがありそうだ…" ; 0247A187 EN: "Something has to be inserted\ninto these slots=@" | JA: "顔にある穴に、\n何かを入れるみたいだな…"
        duration = choice == 0 ? 135 : choice == 1 ? 130 : 110;
    }
    if (EV_GetShenmueFlag(0x323))
        EV_stepSetFlags(0, 0x323);
    if (EV_GetShenmueFlag(0x324)) {
        EV_stepSetFlags(0, 0x324);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1523, 1);
    }
    HLib::SetupTaskDelay(duration);
}
void RunQdss(std::uintptr_t selector);
void ShrineCallback(void *parameter) {
    QdssWork &work = *static_cast<QdssWork *>(parameter);
    switch (work.state) {
    case 0:
        work.savedSeconds = static_cast<std::uint32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 48)) << 24;
        work.savedSeconds += static_cast<std::uint32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 49)) << 16;
        work.savedSeconds += static_cast<std::uint32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 32)) << 8;
        work.savedSeconds += EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 33);
        PrepareShrineReturn(work);
        work.state = 18;
        break;
    case 1: {
        DateTime date;
        GetCurrentDateTime(date);
        work.savedSeconds = static_cast<std::uint32_t>(ComputeSecondsSince1980(date));
        EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 48, work.savedSeconds >> 24);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 49, (work.savedSeconds >> 16) & 255);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 32, (work.savedSeconds >> 8) & 255);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 33, work.savedSeconds & 255);
        BeginQdsaAuthor(work.self, 0);
        work.state = 2;
        break;
    }
    case 2:
        if (UpdateQdsaAuthor()) {
            PrepareShrineReturn(work);
            work.state = 18;
        }
        break;
    case 3:
        work.state = 4;
        break;
    case 4:
        work.authorRemaining = RunQdssDiscovery(work, work.selector);
        work.state = work.selector == 11 || work.selector == 12 ? 5 : 16;
        break;
    case 5:
        if (authGetFinished()) {
            EV_CleanupMapControlTask();
            work.state = 16;
            QdssFade(30, true, 0);
            QueueQdssCling(QDSS_DIAGRAM, shen_name{0}, 0, nullptr, nullptr, 1, 30);
            EVT_AT_ObjectDisp_1(QDSS_DIAGRAM, 30, 1);
            QueueQdssEye(shen_name{0}, 30);
            HLib::SetupTaskDelay(60);
        } else if (!work.authorRemaining--)
            work.state = 16;
        break;
    case 6:
        ShrineIntroduction();
        work.state = 7;
        break;
    case 7:
        work.state = PrepareQdssKeySelection(work) ? 8 : 16;
        break;
    case 8: {
        const std::int32_t result = UpdateQdssKeySelection(work);
        if (result == -1 || result == 0)
            work.state = 16;
        else if (result == 1)
            work.state = 9;
        break;
    }
    case 9:
        PrepareQdssHoleSelection(work);
        work.state = 10;
        break;
    case 10: {
        const std::int32_t result = UpdateQdssHoleSelection(work);
        if (result == -1)
            work.state = 16;
        else if (!result)
            work.state = 7;
        else if (result == 1)
            work.state = 11;
        break;
    }
    case 11:
        work.state = 12;
        break;
    case 12:
        if (work.hole == work.selected) {
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(work.keys[work.selected].flag), 1);
            work.inserted |= 1U << work.selected;
            RemoveInventoryItem(work.keys[work.selected].item, 1);
            work.state = (work.inserted & 15U) == 15U ? 14 : 7;
        } else
            work.state = 9;
        work.nextState = work.state;
        work.authorRemaining = RunQdssInsertion(work);
        work.state = 13;
        break;
    case 13:
        if (authGetFinished()) {
            EV_CleanupMapControlTask();
            QdssFinishInsertion(work);
            work.state = work.nextState;
        } else if (!work.authorRemaining--)
            work.state = work.nextState;
        break;
    case 14:
        for (std::int32_t index : {1, 3, 0, 2})
            AT_SetLocalBaseChara(work.keys[index].character, SHEN_NAME(a321));
        for (std::int32_t index : {1, 3, 0, 2})
            EVT_AT_SetAng_Enqueue(work.keys[index].character, QDSS_BASE, 0, static_cast<float>(work.keys[index].rotation.y) / 182.04445f, -45, 0);
        RemoveShrineItems(work);
        EV_stepSetFlags(0, 0x11e);
        BeginQdsaAuthor(work.self, 1);
        work.state = 15;
        break;
    case 15:
        if (UpdateQdsaAuthor())
            EV_scenQueueLoad(g_actNumber, SHEN_NAME(QDTB), 0);
        break;
    case 16:
        PrepareShrineReturn(work);
        work.state = 17;
        break;
    case 17:
        RestoreShrineControls();
        EV_CleanupMapControlTask();
        HLib::MarkCurrentTaskForCleanup();
        break;
    case 18:
        RestoreShrineControls();
        work.state = 19;
        break;
    case 19: {
        DateTime date;
        GetCurrentDateTime(date);
        const std::uint32_t earlier = static_cast<std::uint32_t>(ComputeSecondsSince1980(date)) - 7200U;
        if (static_cast<std::uint32_t>(static_cast<std::int32_t>(date.hour) - 2) > 2U) {
            if (EV_GetShenmueFlag(0x3c9) && work.savedSeconds < earlier) {
                if (srchHasActiveLockonTarget())
                    srchReset();
                if (ryoNotBusy()) {
                    EV_stepSetFlags(0, 0x3c9);
                    RunQdss(12);
                }
            }
        } else {
            if (srchHasActiveLockonTarget())
                srchReset();
            if (ryoNotBusy()) {
                PrepareShrineOwner();
                Queue_ST_LoadTitleFadeIn(SHEN_NAME(RTR2));
                QdssFade(30, true, 0);
                HLib::SetupTaskDelay(90);
                work.state = 20;
            }
        }
        break;
    }
    case 20: {
        DateTime date;
        IncrementDateTimeBySeconds(work.savedSeconds, date);
        SetCurrentDateTimeTo(date);
        RemoveShrineItems(work);
        EV_stepSetFlags(1, 284);
        ST_LoadTitleFade();
        HLib::SetupTaskDelay(30);
        work.state = 21;
        break;
    }
    case 21:
        HLib::MarkCurrentTaskForCleanup();
        EV_scenQueueLoad(g_actNumber, SHEN_NAME(QDKJ), 1);
        break;
    default:
        break;
    }
}
void InitializeQdss() {
}
void RunQdss(std::uintptr_t selector) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(ShrineCallback, 4, 0x0b, sizeof(QdssWork), SHEN_NAME(_SHU));
    QdssWork *work = static_cast<QdssWork *>(HLib::GetTaskParameter(task));
    std::memset(work, 0, sizeof(*work));
    const std::int32_t mode = static_cast<std::int32_t>(selector);
    work->self = task;
    work->keys = qdssKeys;
    work->selector = mode;
    if (mode == 0 || mode == 2) {
        work->state = mode == 0 ? 0 : 1;
        if (mode == 2) {
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 519, 1);
            if (EV_GetShenmueFlag(0x293) && !GetItemState(work->keys[1].item))
                AddInventoryItem(work->keys[1].item, 1, 0);
        }
        Queue_EVT_UTL_LoadNPCChara_3(shrineActors, task);
        EV_FreeAuthorPak(1, "0247");
    } else if (mode == 1)
        work->state = 6;
    else if (static_cast<std::uint32_t>(mode) - 10U <= 9U)
        work->state = 3;
    PrepareShrineOwner();
}
} // namespace
QdssKey qdssKeys[8];
const ScriptAreaDefinition qdssScriptDefinition{.areaId = SHEN_NAME(QDSS), .field04 = 0, .init = InitializeQdss, .main = RunQdss};
} // namespace shenmue::s2::areas
