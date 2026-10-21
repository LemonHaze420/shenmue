#include "s2/scripts/cwon/cwon_helpers.hpp"
#include "s2/scripts/cwon/cwon_tables.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/debug.hpp"
#include "s2/door.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/load_title.hpp"
#include "s2/map_assets.hpp"
#include "s2/scene_data.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/particle_effects.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"
#include <bit>
#include <cstring>
namespace shenmue::s2 {
namespace {
void StreetRun(void *);
void BeeQte(void *);
void BeePass(void *);
void BeeFail(void *);
void DodgePass(void *);
void DodgeFail(void *);
void CheckStreet(void *);
void ChairQte(void *);
void ChairPass(void *);
void ChairFail(void *);
void TableQte(void *);
void TablePass(void *);
void TableFail(void *);
void MeyQte(void *);
void MeyPass(void *);
void MeyFail(void *);
void CheckMarket(void *);
void BoxQte(void *);
void BoxPass(void *);
void BoxFail(void *);
void MarketExit(void *);
void WorkerPass(void *);
void WorkerFail(void *);
void DoorQte(void *);
void DoorPass(void *);
void DoorFail(void *);
void CleanQte(void *);
void CleanPass(void *);
void CleanFail(void *);
void CheckWarehouse(void *);
void StairsQte(void *);
void StairsPass(void *);
void StairsFail(void *);
void SignQte(void *);
void SignPass(void *);
void SignFail(void *);
void EndQte(void *);
void EndPass(void *);
void EndFail(void *);
void RunEnd(void *);
void LostEnd(void *);
void WonSmoke(void *);
void WaitLoad(void *);
void WaitFade(void *);
constexpr shen_name playerOnly[]{CWON_RYO, shen_name{}};
constexpr shen_name meyOnly[]{SHEN_NAME(MEY_), shen_name{}};
void Position(shen_name name, const Vector3 &value, shen_name parent = CWON_CENTER) {
    EventSetPosition request{0, name, parent, value, 0};
    EVT_AT_SetPosRot(&request);
}
void Angle(shen_name name, float degrees, shen_name parent = CWON_CENTER) {
    EventSetAngle request{0, name, parent, degrees, 0};
    EVT_AT_SetAngY_Callback(&request);
}
void Camera(std::int32_t camera) {
    EVT_AT_SetCam(camera, CWON_DUMMY, CWON_DUMMY, 0, 0, 0);
    camCtrlDisableFilmBars();
}
void Motion(std::uint32_t id, float frame = -1, float speed = 1, std::uint32_t flags = 0) {
    EVT_AT_SetMot(CWON_RYO, id, frame, speed, flags, 0);
}
void WaitMotion(CwonEventWork *work, float frame = 0) {
    CwonQueueMotionWait(work->task, CWON_RYO, frame);
}
void Next(HLib::TaskCallback callback) {
    HLib::SetCurrentTaskTo(callback);
}
void FreeCharacters(const shen_name *list) {
    for (; list->name != CWON_INVALID.name; ++list)
        if (DoesScenarioCharacterExist(*list)) {
            EV_npcLockUnlock(*list);
            EV_npcFree(*list);
        }
}
void WaitPackages() {
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
}
void RequestPartition(std::int32_t index) {
    EV_ControlPackageResources(
        PakCategory::Package, PakPackageOp::LoadAfs, reinterpret_cast<std::uintptr_t>(cwonPack), static_cast<std::uintptr_t>(index), index + 1);
}
void FreePartition(std::int32_t index) {
    EV_ControlPackageResources(
        PakCategory::Package, PakPackageOp::FreeAfs, reinterpret_cast<std::uintptr_t>(cwonPack), static_cast<std::uintptr_t>(index), index + 1);
}
void LoadCharacters(const shen_name *names, HLib::Task *parent = nullptr) {
    cwonNpcLoadTask = Queue_EVT_UTL_LoadNPCChara_3(names, parent);
}
void DestroyEscape(CwonEventWork *work) {
    if (work->escapeTask) {
        HLib::DestroyTaskInternal(work->escapeTask);
        work->escapeTask = nullptr;
    }
}
void DeleteMotions(CwonEventWork *work) {
    for (std::uint32_t &motion : work->motions)
        if (motion) {
            DeleteMotion(motion);
            motion = 0;
        }
}
void ImmediateQte(std::uint16_t status, std::int16_t frames) {
    ReInitElement(UIElement::GameQte);
    if (g_eventQteRetryFrames > 0)
        frames = static_cast<std::int16_t>(EVT_GetQteFailureCount() * ((g_eventQteRetryFrames - frames) / 2) + frames);
    EventQtePromptTaskData request{0, status, frames, 0, 3, 0};
    EVT_AT_Qte_Callback(&request);
}
} // namespace

void CwonRootStreetIntroCallback(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    if (w->authorState == 0) {
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        ++w->authorState;
        cwonWaveDisabled = 0;
        cwonSpringDisabled = 0;
        RequestPartition(0);
        cwonPackIndex = 0;
        cwonPackState = 1;
        WaitPackages();
        QueueImmediateNextFadeTask(nullptr);
        CwonQueueTemporary(w, CwonBirdCallback, 0);
        CwonStartAuthor(0, w->task, 0, CWON_AUTHOR_CHARACTERS, nullptr);
        LoadCharacters(CWON_CHASE_NPC_CHARACTERS);
    } else if (w->authorState == 1) {
        w->authorState = 0;
        EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
        CwonQueueEscape(w, w->entry);
        CwonStartPlayerAt(1);
        Motion(4137);
        EVT_AT_SetVoice("0011A004", 1, 0); // @text EN: "Wait!\nGive me back my bag!" | JA: "待て！　バッグを返せ！"
        EVT_AT_SetVoice("0011B005", 1, 55); // @text EN: "What bag?" | JA: "知らないよー！"
        Next(StreetRun);
    }
}
namespace {

void StreetRun(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    Position(CWON_RYO, {231.85001f, 100.21f, 393.35001f});
    EventSetRawAngle angle{0, CWON_RYO, CWON_CENTER, {0, 59164, 0}, 0};
    EVT_AT_SetAng_Callback(&angle);
    Motion(4137, -1, 1, 0x40000000U);
    Camera(141);
    Next([](void *p) {
        auto *root = static_cast<CwonEventWork *>(p);
        auto index = root->battleResult;
        if (index >= 0) {
            const auto &entry = CWON_STREET_ROUTE[index];
            bool complete = false;
            switch (entry.kind) {
            case 0:
                complete = CwonTurnTowardYaw(entry.character, entry.yaw, entry.step, entry.direction != 0) != 0;
                break;
            case 1:
                complete = CwonPositionThreshold(entry.character, entry.threshold, entry.axis != 0, entry.direction != 0) != 0;
                break;
            case 2:
                complete = true;
                break;
            default:
                index = -1;
                break;
            }
            if (complete)
                ++index;
            if (index >= 0 && CWON_STREET_ROUTE[index].kind == 3)
                index = -1;
        }
        root->battleResult = index;
        if (index == -1) {
            root->battleResult = 0;
            Next(BeeQte);
        }
    });
    WaitMotion(w);
}
void BeeQte(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    if (w->authorState == 1)
        CwonPollQte(w, 256, BeePass, BeeFail);
    else if (w->authorState == 0) {
        w->authorState = 1;
        DestroyEscape(w);
        const auto frames = CwonStartAuthor(9, w->task, 0, CWON_AUTHOR_CHARACTERS, CWON_BEE_GRAVITY_CHARACTERS);
        EVT_AT_QTE(256, 30, 3, frames - 35);
        const shen_name bee{SHEN_NAME(BDMA)};
        Position(bee, CWON_BEE_POSITION);
        EventSetRawAngle angle{0, bee, CWON_CENTER, {0, 62805, 0}, 0};
        EVT_AT_SetAng_Callback(&angle);
        EV_DisableCharacterDisplay(bee);
        EVT_AT_ObjectDisp_1(bee, 56, 0);
        cwonDynamixTask = Queue_EVT_AT_PlayDynamix(bee, "BDMA", 2, 600, 20, CWON_BEE_DYNAMIC_PARTS, 4, 56);
    }
}
void BeePass(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    if (w->authorState == 1)
        CwonPollQte(w, 64, DodgePass, DodgeFail);
    else if (w->authorState == 0 && g_usingCustomAuthorCharacterList) {
        w->authorState = 1;
        CwonPrepareCharacters(CWON_MARKET_WITH_GET_CHARACTERS);
        const auto frames = CwonStartAuthor(10, w->task, 0, CWON_AUTHOR_CHARACTERS, nullptr);
        EVT_AT_QTE(64, 15, 3, frames - 20);
        CwonQueueTemporary(w, CwonPlayerDustCallback, 30);
        if (cwonDynamixTask)
            HLib::DestroyTaskInternal(cwonDynamixTask);
        RequestPartition(2);
        FreePartition(0);
        cwonPackIndex = 2;
        cwonPackState = 3;
    }
}
void BeeFail(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    if (w->authorState == 0) {
        ++w->failures;
        w->authorState = 1;
        CwonStartAuthor(11, w->task, 0, playerOnly, nullptr);
        EV_DisableCharacterDisplay(CWON_WON);
        CwonDisableGravity(CWON_WON);
        CwonQueueTemporary(w, CwonMarketPropsCallback, 0);
    } else if (w->authorState == 1) {
        w->authorState = 0;
        w->routePass = 0;
        Next(LostEnd);
        LostEnd(w);
    } else if (w->authorState == 2) {
        CwonSetPositionMasked(CWON_RYO, 0x82, {260.10001f, 101, 393.70001f});
        if (CwonTurnTowardYaw(CWON_RYO, 33678, 364, true)) {
            w->authorState = 0;
            Next(CheckStreet);
        }
    }
}
void DodgePass(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    if (w->authorState == 0) {
        w->authorState = 1;
        CwonResetSmoke(w);
        CwonQueueTemporary(w, CwonGetDodgeCallback, 0);
        Position(CWON_RYO, CWON_STREET_DODGE_POSITION);
        Angle(CWON_RYO, 180);
        Camera(129);
        Motion(4145, -1, 1, 0x60000000U);
        SetMotionLayerEndFrame(scenFindMotionwork(CWON_RYO), 40, MotionRegion::LowerBody);
        EVT_AT_SetVoice("0011A021", 1, 0);
        WaitMotion(w);
    } else if (w->authorState == 1) {
        w->authorState = 0;
        Motion(4137);
        Next(CheckStreet);
    }
}
void DodgeFail(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    switch (w->authorState) {
    case 0:
        w->authorState = 1;
        CwonResetSmoke(w);
        CwonStartAuthor(12, w->task, 135, CWON_GET_AUTHOR_CHARACTERS, nullptr);
        break;
    case 1:
        w->authorState = 2;
        EV_DisableCharacterDisplay(CWON_WON);
        CwonStartAuthor(-1, w->task, 0, CWON_GET_AUTHOR_CHARACTERS, nullptr);
        break;
    case 2:
        w->authorState = 0;
        EV_SetRefBoardParamsFor(SHEN_NAME(GET_), 1);
        Next(CheckStreet);
        CheckStreet(w);
        break;
    }
}
void CheckStreet(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    if (w->authorState == 1) {
        if (CwonPositionThreshold(CWON_RYO, 385.5f, true, false)) {
            w->authorState = 0;
            w->routePass = 0;
            Next(LostEnd);
        }
    } else if (w->authorState == 0) {
        camCtrlSetFarDistance(30);
        camCtrlSetNearDistance(30);
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
        if (w->failures < 2) {
            w->authorState = 0;
            w->failures = 0;
            EV_stepSetFlags(0, 9);
            if (w->entry < 2) {
                WaitPackages();
                CwonQueueTemporary(w, CwonMarketPropsCallback, 0);
            }
            for (shen_name name : {SHEN_NAME(HAK0), SHEN_NAME(HAK1)})
                Queue_EVT_AT_PlayDynamix(name, CWON_BOX_DYNAMICS, 2, 4, 10, nullptr, 2, 0);
            Next(ChairQte);
            ChairQte(w);
        } else
            ++w->authorState;
    }
}
void ChairQte(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    if (w->authorState == 1)
        CwonPollQte(w, 4, ChairPass, ChairFail);
    else if (w->authorState == 0) {
        w->authorState = 1;
        cwonWaveDisabled = 1;
        cwonSpringDisabled = 1;
        Position(SHEN_NAME(IS23), {260.10001f, 101, 393.70001f});
        CwonStartAuthor(19, w->task, 0, CWON_CHAIR_AUTHOR_CHARACTERS, nullptr);
        EVT_AT_QTE(4, 25, 3, 71);
        for (shen_name name : {SHEN_NAME(IS_1), SHEN_NAME(IS_2), SHEN_NAME(IS_3), SHEN_NAME(IS_4)}) {
            EV_DisableCharacterDisplay(name);
            EV_ToggleCharacterFlag(name);
        }
    }
}
void ChairPass(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    Next(TableQte);
    CwonStartAuthor(20, w->task, 0, CWON_CHAIR_AUTHOR_CHARACTERS, nullptr);
}
void ChairFail(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    Next(TableQte);
    CwonStartAuthor(21, w->task, 0, CWON_CHAIR_AUTHOR_CHARACTERS, nullptr);
}
void TableQte(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    if (w->authorState == 1)
        CwonPollQte(w, 256, TablePass, TableFail);
    else if (w->authorState == 0) {
        w->authorState = 1;
        const auto frames = CwonStartAuthor(25, w->task, 0, CWON_TABLE_AUTHOR_CHARACTERS, nullptr);
        EVT_AT_QTE(256, 20, 3, frames - 25);
        CwonPrepareMotionCharacters(meyOnly);
        CwonSetCollisionTables();
        CwonRestoreBoxes();
        EVT_AT_SetPos(SHEN_NAME(DYU_), SHEN_NAME(DYU_), {-.5f, 0, 0}, 1);
    }
}
void TablePass(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    Next(MeyQte);
    const auto frames = CwonStartAuthor(26, w->task, 0, CWON_TABLE_AUTHOR_CHARACTERS, nullptr);
    EVT_AT_QTE(512, 20, 3, frames - 25);
}
void TableFail(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1937, 1);
    CwonSetFallenTable();
    Queue_EVT_AT_PlayDynamix(SHEN_NAME(TBLe), "TUKU", 2, 200, 17, CWON_TABLE_DYNAMIC_PARTS, 2, 0);
    Next(CheckMarket);
    HLib::SetupTaskDelay(5);
    CwonStartAuthor(27, w->task, 0, CWON_TABLE_AUTHOR_CHARACTERS, nullptr);
}
void MeyQte(void *data) {
    CwonPollQte(static_cast<CwonEventWork *>(data), 512, MeyPass, MeyFail);
}
void MeyPass(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    if (w->authorState == 0) {
        w->authorState = 1;
        CwonStartAuthor(17, w->task, 0, CWON_MEY_AUTHOR_CHARACTERS, nullptr);
    } else if (w->authorState == 1) {
        w->authorState = 0;
        CwonDisableGravity(meyOnly[0]);
        EV_SetRefBoardParamsFor(meyOnly[0], 1);
        Next(CheckMarket);
    }
}
void MeyFail(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    switch (w->authorState) {
    case 0:
        w->authorState = 1;
        CwonStartAuthor(18, w->task, 30, CWON_MEY_AUTHOR_CHARACTERS, nullptr);
        break;
    case 1:
        w->authorState = 2;
        CwonStartAuthor(-1, w->task, 56, CWON_MEY_AUTHOR_CHARACTERS, nullptr);
        break;
    case 2:
        w->authorState = 3;
        CwonStartAuthor(-1, w->task, 0, CWON_MEY_AUTHOR_CHARACTERS, nullptr);
        break;
    case 3:
        w->authorState = 0;
        CwonDisableGravity(meyOnly[0]);
        EV_SetRefBoardParamsFor(meyOnly[0], 1);
        Next(CheckMarket);
        break;
    }
}
void CheckMarket(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    if (w->authorState == -1) {
        w->authorState = 0;
        w->entry = 4;
        Motion(4137);
        SetScreenFilmFade(1, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    } else if (w->authorState == 0) {
        camCtrlSetFarDistance(140);
        camCtrlSetNearDistance(140);
        CwonQueueEscape(w, 4);
        Camera(160);
        Position(CWON_RYO, CWON_MARKET_EXIT_POSITION);
        Angle(CWON_RYO, 210);
        Motion(4151);
        EVT_AT_SetMot(CWON_RYO, 4143, -1, 1, 0, 45);
        CwonQueueTemporary(w, CwonWarehousePropsCallback, 0);
        EVT_AT_QTE(64, 20, 3, 35);
        Next(BoxQte);
        HLib::SetupTaskDelay(55);
    }
}
} // namespace
void CwonRootMarketIntroCallback(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    switch (w->authorState) {
    case -1:
        w->authorState = 0;
        RequestPartition(2);
        cwonPackIndex = 2;
        cwonPackState = 3;
        WaitPackages();
        CwonQueueTemporary(w, CwonMarketPropsCallback, 0);
        break;
    case 0:
        cwonWaveDisabled = 1;
        cwonSpringDisabled = 1;
        w->authorState = 1;
        QueueImmediateNextFadeTask(nullptr);
        CwonStartAuthor(w->entry == 2 ? 15 : 14, w->task, 0, CWON_AUTHOR_CHARACTERS, nullptr);
        w->entry = 3;
        LoadCharacters(CWON_MARKET_WAREHOUSE_CHARACTERS);
        break;
    case 1:
        w->authorState = 2;
        CwonStartAuthor(16, w->task, 0, CWON_AUTHOR_CHARACTERS, nullptr);
        break;
    case 2:
        if (g_usingCustomAuthorCharacterList) {
            w->authorState = 0;
            Motion(4137);
            EVT_AT_SetMot(CWON_WON, 4137, -1, 1, 0, 0);
            Next(CheckStreet);
            CheckStreet(w);
        }
        break;
    }
}
namespace {
// ed770-edf90: crate and worker branches.
void BoxQte(void *data) {
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    CwonPollQte(static_cast<CwonEventWork *>(data), 64, BoxPass, BoxFail);
}
void BoxPass(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    Camera(144);
    EVT_AT_SetVoice("0011A021", 1, 20);
    Motion(4145, 0, 1, 0x20000000U);
    Next(MarketExit);
    WaitMotion(w);
}
void BoxFail(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    EV_DisableCharacterDisplay(CWON_WON);
    Camera(166);
    EVT_AT_SetVoice("0011A022", 1, 10);
    Motion(4182, 0, 1, 0x60000000U);
    EV_AT_SetPos(CWON_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, CWON_BOX_FALL_POSITION);
    Next(MarketExit);
    WaitMotion(w, 45);
}
void PrepareWorkerQte(CwonEventWork *w) {
    w->authorState = 3;
    FreeCharacters(CWON_MARKET_WITH_GET_CHARACTERS);
    if (cwonDynamixTask)
        HLib::DestroyTaskInternal(cwonDynamixTask);
    RequestPartition(4);
    FreePartition(2);
    cwonPackIndex = 4;
    cwonPackState = 5;
    Camera(162);
    Motion(4143, 0);
    Position(CWON_RYO, CWON_WAREHOUSE_APPROACH_POSITION);
    Angle(CWON_RYO, 180);
    EVT_AT_QTE(64, 20, 3, 10);
}
void MarketExit(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    switch (w->authorState) {
    case 0:
        if (g_usingCustomAuthorCharacterList) {
            EV_EnableCharacterDisplay(CWON_WON);
            PrepareWorkerQte(w);
            EVT_AT_SetVoice("0011A050", 1, 0); // @text EN: "Wait!" | JA: "待てっ！"
        }
        break;
    case 1:
        if (CwonTurnTowardYaw(CWON_RYO, 32768, 910, true))
            ++w->authorState;
        break;
    case 2:
        PrepareWorkerQte(w);
        break;
    case 3:
        if (CwonTurnTowardYaw(CWON_RYO, 20024, 637, true))
            ++w->authorState;
        break;
    case 4:
        CwonPollQte(w, 64, WorkerPass, WorkerFail);
        break;
    }
}
void WorkerPass(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    Camera(147);
    Motion(4145, -1, .8f, 0x20000000U);
    EVT_AT_SetVoice("0011A051", 1, 0);
    Next(DoorQte);
    HLib::SetupTaskDelay(30);
    Position(CWON_WON, {264, 100.9f, std::bit_cast<float>(1137849139U)});
}
void WorkerFail(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    Camera(167);
    Position(CWON_RYO, {268, 100.9f, std::bit_cast<float>(1137793434U)});
    Motion(4141, -1, 1, 0x20000000U);
    EVT_AT_SetVoice("0011A015", 1, 0); // @text EN: " " | JA: "うわっ！"
    EVT_AT_SetVoice("0011K001", 1, 15); // @text EN: " " | JA: "うわ～！！"
    EVT_AT_SetVoice("0011A016", 1, 30); // @text EN: "I'm sorry!" | JA: "…すいません！"
    CwonQueueTemporary(w, CwonFallingWorkerCallback, 10);
    CwonQueueTemporary(w, CwonFallingSignCallback, 10);
    Next(DoorQte);
    HLib::SetupTaskDelay(40);
}

void DoorQte(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    switch (w->authorState) {
    case 0:
        w->authorState = 1;
        SetDoorSubjectEnabled(SHEN_NAME(R122), 0);
        EV_ToggleCharacterFlag(SHEN_NAME(ANA_));
        EV_ToggleCharacterFlag(SHEN_NAME(KYAT));
        for (shen_name name : {SHEN_NAME(ANA_), SHEN_NAME(KYAT), SHEN_NAME(KANB)})
            EVT_AT_ObjectDisp_1(name, 1, 1);
        CwonStartAuthor(28, w->task, 60, CWON_AUTHOR_CHARACTERS, nullptr);
        DestroyEscape(w);
        break;
    case 1:
        w->authorState = 2;
        CwonStartAuthor(-1, w->task, 0, CWON_AUTHOR_CHARACTERS, nullptr);
        ImmediateQte(4, 15);
        CwonQueueLook(CWON_RYO, SHEN_NAME(KJN_), 0, 0, 0, 5, 5, 15);
        CwonQueueTemporary(w, CwonOpeningDoorCallback, 0);
        break;
    case 2:
        if (CwonPollQte(w, 4, DoorPass, DoorFail)) {
            EV_EnableCharacterDisplay(SHEN_NAME(KANB));
            EV_DisableCharacterDisplay(CWON_WON);
            Angle(CWON_RYO, 90);
        }
        break;
    }
}
void DoorPass(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    if (w->authorState)
        return;
    w->authorState = 1;
    CwonQueueTemporary(w, CwonTurningPedestrianCallback, 0);
    Position(CWON_RYO, CWON_DOOR_APPROACH_POSITION);
    Camera(149);
    EVT_AT_SetVoice("0011A053", 1, 0);
    EVT_AT_SetVoice("0011A054", 1, 20);
    EVT_AT_SetVoice("0011A056", 1, 30); // @text EN: "Wait!" | JA: "待て！"
    Motion(4145, 5, 1, 0x40000000U);
    EVT_AT_SetMot(CWON_RYO, 4137, 0, .8f, 0x40000000U, 39);
    w->authorState = 0;
    Next(CleanQte);
    HLib::SetupTaskDelay(20);
}
void DoorFail(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    if (w->authorState == 0) {
        w->authorState = 1;
        Position(CWON_RYO, {261.39999f, 100.9f, 419.70001f});
        Camera(159);
        Motion(4111, 7, 1, 0x20000000U);
        Next(DoorFail);
        WaitMotion(w);
        EVT_AT_SetVoice("0011A055", 1, 0);
        EVT_AT_SetVoice("0011L002", 1, 10); // @text EN: " " | JA: "きゃっ！"
        EVT_AT_SetVoice("0011A016", 1, 30); // @text EN: "I'm sorry!" | JA: "…すいません！"
    } else if (w->authorState == 1) {
        w->authorState = 0;
        Motion(4137, 0, .8f);
        Position(CWON_RYO, {261, 100.9f, 418.8f});
        Camera(113);
        CwonQueueTemporary(w, CwonTurningPedestrianCallback, 0);
        Next(CleanQte);
    }
}
// eeb90-ef190: authored stair-height correction is performed once per callback.
void CleanQte(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    switch (w->authorState) {
    case 0:
        w->authorState = 1;
        SetMotionSpeed(scenFindMotionwork(CWON_RYO), .8f, MotionRegion::LowerBody);
        ImmediateQte(256, 20);
        break;
    case 1:
        if (CwonPositionThreshold(CWON_RYO, 256, false, true))
            ++w->authorState;
        break;
    case 2:
        if (CwonTurnTowardYaw(CWON_RYO, 29127, 2730, false))
            ++w->authorState;
        break;
    case 3:
        if (CwonPositionThreshold(CWON_RYO, 419, true, false))
            ++w->authorState;
        break;
    case 4:
        if (CwonPollQte(w, 256, CleanPass, CleanFail))
            EV_SetCharaControllerFlags(CWON_RYO, 3, NpcCharacterControllerState::Disabled);
        break;
    }
}
void CleanPass(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    switch (w->authorState) {
    case 0:
        w->authorState = 1;
        if (w->battleResult) {
            Position(CWON_RYO, {w->routePosition, w->routeSpeed, std::bit_cast<float>(static_cast<std::uint32_t>(w->routeTimer))});
            EVT_AT_SetAng_Enqueue(CWON_RYO, CWON_CENTER, w->birdTimer / 182.04445f, static_cast<std::int32_t>(w->reserved10) / 182.04445f,
                static_cast<std::int32_t>(w->reserved14) / 182.04445f, 0);
        } else {
            Vector3 position;
            Vector3i rotation;
            EV_GetCharaPosition(CWON_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
            GetScenarioCharacterEulerAngles(CWON_RYO, &rotation, EventTransformFlags::Local);
            w->routePosition = position.x;
            w->routeSpeed = position.y;
            w->routeTimer = std::bit_cast<std::int32_t>(position.z);
            w->birdTimer = rotation.x;
            w->reserved10 = static_cast<std::uint32_t>(rotation.y);
            w->reserved14 = static_cast<std::uint32_t>(rotation.z);
            CwonQueueLook(SHEN_NAME(SHI_), CWON_RYO, 0, 0, 0, 3, 50, 15);
        }
        CwonDisableGravity(CWON_RYO);
        Position(CWON_RYO, CWON_DOOR_PASS_POSITION);
        Angle(CWON_RYO, 160);
        Motion(4150, -1, 1.2f, 0x40000000U);
        Position(SHEN_NAME(KLN_), CWON_DOOR_SMOKE_OFFSET, SHEN_NAME(KLN_));
        if (w->battleResult == 0) {
            EVT_AT_SetVoice("0011A057", 1, 2);
            EVT_AT_SetVoice("0011N001", 1, 15); // @text EN: " " | JA: "うわっ！"
            Camera(138);
        } else if (w->battleResult == 1)
            Camera(170);
        else if (w->battleResult == 2)
            Camera(171);
        HLib::SetupTaskDelay(7);
        break;
    case 1: {
        Vector3 position;
        EV_GetCharaPosition(CWON_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
        position.y -= .050000001f;
        position.z += .029999999f;
        if (position.y <= 100) {
            ++w->authorState;
            position.y = 100;
            HLib::SetupTaskDelay(5);
        } else if (position.y < 100.35f && position.y >= 100.3f && w->smoke == -1)
            w->smoke = static_cast<std::int32_t>(CwonCreateDust(CWON_CLEAN_DUST_POSITION, {.005f, 0, .005f}, {}));
        Position(CWON_RYO, position);
        break;
    }
    case 2:
        w->authorState = 0;
        w->battleResult = 0;
        if (w->failures < 2)
            EVT_AT_SetMot(CWON_RYO, 4137, 0, 1, 0x40000000U, 1);
        mtwkSetRequestedRootMode(scenFindMotionwork(CWON_RYO), 0);
        Next(CheckWarehouse);
        break;
    }
}
void CleanFail(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    w->authorState = 0;
    CwonDisableGravity(CWON_RYO);
    Camera(1500);
    EVT_AT_SetCam(150, CWON_DUMMY, CWON_DUMMY, 0, 0, 48);
    mtwkSetRequestedRootMode(scenFindMotionwork(CWON_RYO), 0);
    Position(CWON_RYO, CWON_DOOR_FAILURE_POSITION);
    Angle(CWON_RYO, 180);
    EVT_AT_SetVoice("0011A029", 1, 10);
    EVT_AT_SetVoice("0011N004", 1, 70); // @text EN: "Sorry." | JA: "おっと、ごめんよ。"
    Motion(4141);
    EVT_AT_SetMot(CWON_RYO, 4132, 4, 1, 0, 48);
    const auto frame = static_cast<std::int32_t>(GetMotionFrameCount(4132) - 4 + 48);
    EVT_AT_SetPos(CWON_RYO, CWON_RYO, {0, -.89999998f, 0}, frame - 1);
    const auto delay = EVT_AT_SetMot(CWON_RYO, 4137, 0, 1, 0x20000000U, frame);
    Next(CheckWarehouse);
    HLib::SetupTaskDelay(delay);
}
} // namespace
// ef520: alternate warehouse admission owns the CWONTRY2 package and character load task.
void CwonRootWarehouseIntroCallback(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    switch (w->authorState) {
    case -1:
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("CWONTRY2"));
        WaitPackages();
        LoadCharacters(CWON_CHS_HLN_CHARACTERS, HLib::GetCurrentTask());
        ++w->authorState;
        break;
    case 0:
        cwonWaveDisabled = 1;
        cwonSpringDisabled = 1;
        ST_LoadTitleFade();
        EV_SetCharaControllerFlags(CWON_RYO, 2, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(CWON_RYO, 3, NpcCharacterControllerState::Disabled);
        CwonPrepareCharacters(CWON_CHS_HLN_CHARACTERS);
        EV_authSceneCtrl(AuthSceneCtrl::SetCharaHQFlags);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(w->entry == 4 ? SHEN_NAME(RAU1) : SHEN_NAME(RAU2)));
        w->entry = 5;
        w->authorState = 2;
        HLib::SetupTaskDelay(10);
        break;
    case 1:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            WaitPackages();
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("CWONTRY2"), 0);
            FreeCharacters(CWON_CHS_HLN_CHARACTERS);
            Motion(4137);
            EVT_AT_SetMot(CWON_WON, 4137, -1, 1, 0, 0);
            w->authorState = 0;
            Next(CheckWarehouse);
            CheckWarehouse(w);
        }
        break;
    case 2:
        RequestPartition(4);
        cwonPackIndex = 4;
        cwonPackState = 5;
        w->authorState = 1;
        break;
    }
}
namespace {
// ef7e0-f0840: stairs, barrel, falling-sign and final QTE branches.
void CheckWarehouse(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    if (w->authorState == -1) {
        w->entry = 5;
        w->authorState = 0;
        EVT_AT_SetVoice("0011A031", 1, 0); // @text EN: "Hey! Wait!" | JA: "おい！　待てっ！"
        EVT_AT_SetVoice("0011B011", 1, 20); // @text EN: "Why don't you give up?!" | JA: "しつこいんだよー！"
        HLib::SetupTaskDelay(20);
        SetScreenFilmFade(20, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    } else if (w->authorState == 0) {
        CwonResetSmoke(w);
        CwonQueuePipeDynamics();
        if (w->failures < 2) {
            w->authorState = 0;
            w->failures = 0;
            CwonRestoreGravity(CWON_RYO);
            CwonQueueEscape(w, 5);
            EV_stepSetFlags(0, 10);
            mtwkSetRequestedRootMode(scenFindMotionwork(CWON_RYO), 0);
            CwonStartPlayerAt(5);
            camCtrlSetFovTransitionTo(true);
            Camera(137);
            Next(StairsQte);
            FreeCharacters(CWON_WAREHOUSE_CHARACTERS);
            LoadCharacters(CWON_END_CHARACTERS);
        } else {
            w->routePass = 1;
            Camera(204);
            Angle(CWON_RYO, 160);
            Position(CWON_RYO, CWON_LOST_WON_POSITION);
            Next(LostEnd);
            LostEnd(w);
        }
    }
}
void StairsQte(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    switch (w->authorState) {
    case 0:
        w->authorState = 2;
        mtwkSetRequestedRootMode(scenFindMotionwork(CWON_RYO), 0);
        Motion(4137, 0);
        break;
    case 1:
        if (CwonTurnTowardYaw(CWON_RYO, 29127, 910, true))
            ++w->authorState;
        break;
    case 2:
        if (CwonPositionThreshold(CWON_RYO, 443, true, false)) {
            ++w->authorState;
            EV_mtwkSetEnabled(scenFindMotionwork(CWON_WON), 1);
            ImmediateQte(4, 20);
            camCtrlSetMotionScaleFactor(.30000001f);
            camCtrlToggleSmoothing(0);
            Camera(114);
            CwonDisableGravity(CWON_RYO);
            Position(CWON_RYO, CWON_DRUM_QTE_POSITION);
            Angle(CWON_RYO, 160);
            const auto frames = EVT_AT_SetMot(CWON_RYO, 0xf02a, 0, 1, 0, 0);
            EVT_AT_SetPos(CWON_RYO, CWON_RYO, {0, .89999998f, 0}, frames - 1);
        }
        break;
    case 3:
        if (CwonPositionThreshold(CWON_RYO, 445.5f, true, false)) {
            Position(CWON_RYO, {247.14999f, 101.575f, 445.5f});
            EVT_AT_SetMot(CWON_RYO, 4137, 3, .85000002f, 0x02000000U, 0);
            HLib::SetupTaskDelay(5);
            ++w->authorState;
        }
        break;
    case 4:
        if (CwonTurnTowardYaw(CWON_RYO, 20024, 910, true))
            ++w->authorState;
        break;
    case 5:
        CwonPollQte(w, 4, StairsPass, StairsFail);
        break;
    }
}
void StairsPass(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    EVT_AT_SetVoice("0011A021", 1, 0);
    Camera(164);
    Position(CWON_RYO, CWON_BARREL_FAILURE_POSITION);
    Motion(4145);
    SetMotionLayerEndFrame(scenFindMotionwork(CWON_RYO), 39, MotionRegion::LowerBody);
    Next(SignQte);
    WaitMotion(w);
}
void StairsFail(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    if (w->authorState == 0) {
        w->authorState = 1;
        Camera(151);
        EVT_AT_SetVoice("0011A032", 1, 0);
        Position(CWON_RYO, {245.2f, 101.575f, 448.3f});
        Angle(CWON_RYO, 90);
        mtwkSetRequestedRootMode(scenFindMotionwork(CWON_RYO), 3);
        Motion(4111, 10, 1, 0x20000000U);
        EVT_AT_SetMot(CWON_RYO, 4143, 8, 1, 0, 55);
        HLib::SetupTaskDelay(45);
    } else if (w->authorState == 1 && CwonTurnTowardYaw(CWON_RYO, 21845, 910, false)) {
        w->authorState = 0;
        Next(SignQte);
        HLib::SetupTaskDelay(10);
    }
}
void SignQte(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    switch (w->authorState) {
    case 0:
        cwonWaveDisabled = 0;
        cwonSpringDisabled = 0;
        w->authorState = 1;
        SetScenarioCharacterRadius(CWON_WON, 10);
        mtwkSetRequestedRootMode(scenFindMotionwork(CWON_RYO), 3);
        DestroyEscape(w);
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
        CwonStartAuthor(22, w->task, 65, CWON_AUTHOR_CHARACTERS, nullptr);
        break;
    case 1:
        w->authorState = 2;
        cwonPipeStep = 1;
        w->battleResult = static_cast<std::int32_t>(CwonQueueTemporary(w, CwonPipeCallback, 0));
        EVT_AT_QTE(16, 20, 3, 9);
        HLib::SetupTaskDelay(10);
        break;
    case 2:
        CwonPollQte(w, 16, SignPass, SignFail);
        break;
    }
}
void ReplacePipe(CwonEventWork *w, std::int32_t ticks) {
    CwonDestroyTemporary(w, w->battleResult);
    const auto slot = CwonQueueTemporary(w, CwonPipeCallback, 0);
    w->battleResult = static_cast<std::int32_t>(slot);
    if (slot <= 9 && w->temporaryTasks[slot])
        if (auto *work = static_cast<CwonTemporaryWork *>(HLib::GetTaskParameter(w->temporaryTasks[slot])))
            work->ticks = ticks;
}
void SignPass(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    switch (w->authorState) {
    case 0:
        w->authorState = 1;
        CwonQueueTemporary(w, CwonPlayerOffsetDustCallback, 0);
        CwonStartAuthor(23, w->task, 26, CWON_AUTHOR_CHARACTERS, nullptr);
        ReplacePipe(w, 32);
        break;
    case 1:
        w->authorState = 2;
        CwonDestroyTemporary(w, w->battleResult);
        EVT_AT_SetSound(668, 20);
        ReplacePipe(w, 10);
        break;
    case 2:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            w->authorState = 0;
            EV_EnableCharacterDisplay(CWON_WON);
            Next(EndQte);
            CwonStartAuthor(29, w->task, 74, CWON_AUTHOR_CHARACTERS, nullptr);
        }
        break;
    }
}
void SignFail(void *data) {
    using enum CameraState;
    auto *w = static_cast<CwonEventWork *>(data);
    if (w->authorState == 0) {
        w->authorState = 1;
        CwonStartAuthor(24, w->task, 75, CWON_AUTHOR_CHARACTERS, nullptr);
        if (w->failures >= 2)
            EV_DisableCharacterDisplay(CWON_WON);
        cwonPipeStep = 2;
        ReplacePipe(w, 14);
    } else if (w->authorState == 1) {
        w->authorState = 0;
        if (w->failures < 2) {
            CwonFinishEscape(w);
            Next(EndQte);
            CwonStartAuthor(29, w->task, 74, CWON_AUTHOR_CHARACTERS, nullptr);
        } else {
            EV_authSceneCtrl(AuthSceneCtrl::RefreshAndEnqueue);
            for (const shen_name *name = CWON_AUTHOR_CHARACTERS; name->name; ++name) {
                EV_mtwkSetEnabled(scenFindMotionwork(*name), 1);
                mtwkSetRequestedRootMode(scenFindMotionwork(*name), 3);
                EV_SetRefBoardParamsFor(*name, 1);
            }
            EVT_AT_SetPos(CWON_WON, CWON_CENTER, {}, 1);
            Position(CWON_RYO, {233.25f, 101.57f, 449.9f});
            Angle(CWON_RYO, 180);
            Motion(4137, 1, 1, 0x22000000U);
            EVT_AT_SetVoice("0011A061", 1, 10);
            mtwkSetRequestedRootMode(scenFindMotionwork(CWON_RYO), 0);
            camCtrlSetState(Init);
            Next(RunEnd);
            w->battleResult = 0;
        }
    }
}
void EndQte(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    if (w->authorState == 0) {
        w->authorState = 1;
        ImmediateQte(256, 20);
        CwonResetSmoke(w);
    } else if (w->authorState == 1)
        CwonPollQte(w, 256, EndPass, EndFail);
}

void EndPass(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    switch (w->authorState) {
    case 0:
        if (g_usingCustomAuthorCharacterList) {
            IncrementSteamStat(13);
            CwonPrepareCharacters(CWON_END_CHARACTERS);
            FreeCharacters(CWON_DEY_CHARACTERS);
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load,
                reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)), reinterpret_cast<std::uintptr_t>("CWONHAND"));
            ++w->authorState;
            EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
            CwonStartAuthor(1, w->task, 0, CWON_AUTHOR_CHARACTERS, CWON_VICTORY_GRAVITY_CHARACTERS);
            for (shen_name name : {SHEN_NAME(BHK0), SHEN_NAME(BHK1), SHEN_NAME(BHK2), SHEN_NAME(BHK3)})
                Queue_EVT_AT_PlayDynamix(name, CWON_END_BOX_DYNAMICS, 2, 4, 38, nullptr, 2, 0);
            HLib::EnqueueTaskWithoutParameter(WonSmoke, 6, 11, SHEN_NAME(smok));
        }
        break;
    case 1:
        w->authorState = 2;
        w->routePass = 4;
        {
            DateTime date;
            GetCurrentDateTime(date);
            if (date.hour >= 19)
                EV_lightLoad(SHEN_NAME(0001));
        }
        WaitPackages();
        cwonWaveDisabled = 1;
        cwonSpringDisabled = 1;
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        CwonStartAuthor(2, nullptr, 0, CWON_END_AUTHOR_CHARACTERS, CWON_VICTORY_GRAVITY_CHARACTERS);
        if (CwonUpdateVictoryAuthorHands(w))
            ++w->authorState;
        break;
    case 2:
        if (CwonUpdateVictoryAuthorHands(w))
            ++w->authorState;
        break;
    case 3:
        w->authorState = 0;
        DeleteMotions(w);
        EV_ToggleCharacterFlag(SHEN_NAME(a014));
        EV_DisableCharacterDisplay(SHEN_NAME(a014));
        camCtrlToggleSmoothing(0);
        Next(CwonInitializeBattleCallback);
        EV_stepSetFlags(0, 11);
        break;
    }
}
void WonSmoke(void *) {
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        HLib::DestroyCurrentTask();
        return;
    }
    const auto frame = EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame);
    if (frame == 200) {
        cwonSmoke = static_cast<std::int32_t>(CreateSmokeEffect(cwonSmokePosition, cwonSmokeVolume, 50, .07999999821186066f, shen_name{}, 0));
        const auto smoke = static_cast<std::uint32_t>(cwonSmoke);
        SetSmokeEffectFadeFactor(smoke, 27);
        SetSmokeEffectFalloff(smoke, 158);
        SetParticleEffectColour(0, smoke, 0, 106, 104, 101);
        SetParticleEffectInitialScale(0, smoke, .21568627655506134f);
        SetParticleEffectSize(0, smoke, 2.313725471496582f, 2.8235294818878174f);
        SetParticleEffectBirthRate(0, smoke, .03019607998430729f);
        SetParticleEffectVelocity(0, smoke, cwonSmokeVelocity, cwonSmokeVariance);
        ResumeParticleEffect(0, smoke);
    } else if (frame == 250) {
        SuspendParticleEffect(0, static_cast<std::uint32_t>(cwonSmoke));
        FreeParticleEffect(0, static_cast<std::uint32_t>(cwonSmoke));
    }
}
void EndFail(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    if (w->authorState == 0 && g_usingCustomAuthorCharacterList) {
        w->authorState = 1;
        CwonPrepareCharacters(CWON_END_CHARACTERS);
        EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
        CwonStartAuthor(13, w->task, 0, CWON_FAILURE_AUTHOR_CHARACTERS, nullptr);
        EVT_AT_SetSound(899, 20);
        EV_DisableCharacterDisplay(CWON_WON);
    } else if (w->authorState == 1) {
        w->authorState = 0;
        w->routePass = 3;
        EV_stepSetFlags(1, 11);
        Next(CwonEndChaseCallback);
    }
}

void RunEnd(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    auto index = w->battleResult;
    if (index >= 0) {
        const auto &entry = CWON_EXIT_ROUTE[index];
        bool complete = false;
        switch (entry.kind) {
        case 0:
            complete = CwonTurnTowardYaw(entry.character, entry.yaw, entry.step, entry.direction != 0) != 0;
            break;
        case 1:
            complete = CwonPositionThreshold(entry.character, entry.threshold, entry.axis != 0, entry.direction != 0) != 0;
            break;
        case 2:
            complete = true;
            break;
        default:
            index = -1;
            break;
        }
        if (complete)
            ++index;
        if (index >= 0 && CWON_EXIT_ROUTE[index].kind == 3)
            index = -1;
    }
    if (index == w->battleResult)
        return;
    w->battleResult = index;
    if (index == -1) {
        CwonDisableGravity(CWON_RYO);
        mtwkSetRequestedRootMode(scenFindMotionwork(CWON_RYO), 0);
        Position(CWON_RYO, CWON_END_POSITION);
        Motion(0xf008, 0);
        const auto frames = static_cast<std::int32_t>(GetMotionFrameCount(0xf008));
        EVT_AT_SetPos(CWON_RYO, CWON_RYO, {0, -.89999998f, 0}, frames - 1);
        EVT_AT_SetPos(CWON_RYO, CWON_RYO, {0, -.67000002f, 0}, frames + 8);
        w->battleResult = 0;
        w->routePass = 2;
        Next(LostEnd);
        HLib::SetupTaskDelay(frames + 7);
    } else if (index == 5) {
        mtwkSetRequestedRootMode(scenFindMotionwork(CWON_RYO), 0);
        EV_AT_SetPos(CWON_RYO, EventTransformFlags::AllAxes, {250.70849609375f, 101.57499694824219f, 470});
        EV_AT_SetAngY(CWON_RYO, EventTransformFlags::AllAxes, {0, 49152, 0});
        Camera(156);
    }
}
} // namespace
namespace {

void LostEnd(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    if (w->authorState == 1) {
        w->authorState = 0;
        if (w->routePass <= 2)
            EV_stepSetFlags(1, w->routePass == 0 ? 9 : w->routePass == 1 ? 10 : 11);
        SetScreenFilmFade(20, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        Next(CwonEndChaseCallback);
        CwonQueueFadeWait(w->task);
        return;
    }
    if (w->authorState)
        return;
    w->authorState = 1;
    switch (w->routePass) {
    case 2:
        EVT_AT_SetPos(CWON_RYO, CWON_RYO, {.69999999f, 0, 0}, 50);
        EVT_AT_SetCam(203, CWON_DUMMY, CWON_DUMMY, 0, 0, 50);
        WaitMotion(w);
        EVT_AT_SetVoice("0011A023", 1, 60); // @text EN: "Damn=@\nI lost him=@" | JA: "くそ…見失ったか…"
        break;
    case 0:
        mtwkSetRequestedRootMode(scenFindMotionwork(CWON_RYO), 0);
        EV_AT_SetPos(CWON_RYO, EventTransformFlags::AllAxes, {257.85651f, 100.9f, 386.78671f});
        EV_AT_SetAngY(CWON_RYO, EventTransformFlags::AllAxes, {0, 33678, 0});
        Camera(200);
        EVT_AT_SetCam(201, CWON_DUMMY, CWON_DUMMY, 0, 0, 60);
        WaitMotion(w, 167);
        EVT_AT_SetVoice("0011A011", 1, 60); // @text EN: "I lost him." | JA: "見失ったか…"
        break;
    case 1:
        SetAllMapFieldDepthSlotsEnabled(0);
        EVT_AT_SetCam(205, CWON_DUMMY, CWON_DUMMY, 0, 0, 60);
        WaitMotion(w);
        EVT_AT_SetVoice("0011A023", 1, 60); // @text EN: "Damn=@\nI lost him=@" | JA: "くそ…見失ったか…"
        break;
    default:
        break;
    }
    camCtrlToggleSmoothing(0);
    Motion(4138);
}
void WaitLoad(void *) {
    if (EV_ForceLoadNpc(CWON_WON)) {
        LoadCutsceneAndSoundProgramForArea();
        EV_spmBeginSampleExecute();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_NPC_NewLoadSw(1);
        Next(WaitFade);
    }
}
void WaitFade(void *data) {
    auto *w = static_cast<CwonEventWork *>(data);
    if (!filmCheckFadeTrackColour()) {
        FinishLoading();
        SetPlayerControlEnabled(1);
        CwonRestoreGravity(CWON_RYO);
        SetRenderClockAndCompassTo(0);
        scenFreeEvent("CWON.EV3");
        EV_CleanupMapControlTask();
        HLib::EV_HLKillOtherHierarchyTask(w->task);
    }
}
} // namespace
// f1550: admission, package teardown, host restoration, then task teardown.
void CwonEndChaseCallback(void *data) {
    using enum CameraState;
    auto *w = static_cast<CwonEventWork *>(data);
    if (!g_usingCustomAuthorCharacterList)
        return;
    WaitPackages();
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>("CWONHAND"), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("CWONHAND"), 0);
    EV_npcLockUnlock(SHEN_NAME(SAM_));
    EV_npcLockUnlock(SHEN_NAME(LLY_));
    for (const shen_name *list : {CWON_CHARACTERS, CWON_GET_CHARACTERS, CWON_MARKET_CHARACTERS, CWON_WAREHOUSE_CHARACTERS, CWON_DEY_CHARACTERS,
             CWON_ADDITIONAL_CHARACTERS, CWON_CHS_HLN_CHARACTERS})
        FreeCharacters(list);
    mtwkRequestMotion(scenFindMotionwork(CWON_RYO), 0xf003, MotionRegion::LowerBody);
    SetPlayerGameFlags(static_cast<PlayerGameFlagMode>(2), reinterpret_cast<void *>(1));
    const bool retailHost = g_currentMap.name == SHEN_NAME(AR02);
    if (!retailHost) {
        if (cwonPackIndex != -1 && cwonPackState != 1)
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FreeAfs, reinterpret_cast<std::uintptr_t>(cwonPack),
                static_cast<std::uintptr_t>(cwonPackIndex), cwonPackState);
        EV_ControlPackageResources(PakCategory::Partition, PakPartitionOp::Unmount, reinterpret_cast<std::uintptr_t>(cwonPack), 0);
    }
    EV_NPC_NewLoadSw(1);
    if (w->temporarySlot <= 1)
        scenSetTaskFlags(0);
    else if (w->temporarySlot == 2)
        CwonQueueLoadingTitle(SHEN_NAME(ARXB), 20);
    else
        EV_debug_log("CWON\\eCWON_et_ChaseWon.c", 2332, "titlectrl error\n");
    CwonResetSmoke(w);
    if (retailHost) {
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 2228, 0);
        CwonRestoreMarketProps(0);
        camCtrlSetState(Init);
        camCtrlSetFovTransitionTo(true);
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("CWONBATL"), 0);
        if (w->entry == 7)
            for (shen_name name : {SHEN_NAME(TBLa), SHEN_NAME(TBLb), SHEN_NAME(TBLc), SHEN_NAME(TBLd), SHEN_NAME(TBLe)})
                ToggleFlagOnFieldEntity(name, 1);
        if (w->battleResult) {
            Position(CWON_RYO, w->entry == 7 ? CWON_MARKET_RETURN_POSITION : CWON_WAREHOUSE_RETURN_POSITION);
            Angle(CWON_RYO, 0);
            EV_stepSetFlags(0, w->entry == 7 ? 13 : 14);
            EV_LoadNPCRoute("NPC_AR02.BIN");
        } else {
            EV_stepSetFlags(1, w->entry == 7 ? 13 : 14);
            CwonRestoreGravity(CWON_RYO);
            FinishLoading();
            SetPlayerControlEnabled(1);
            camCtrlSetState(Init);
            camCtrlSetFovTransitionTo(true);
            SetRenderClockAndCompassTo(0);
            LoadCutsceneAndSoundProgramForArea();
            EV_spmBeginSampleExecute();
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        }
    } else if (w->routeChoice) {
        if (!w->nextArea.name)
            w->nextArea = SHEN_NAME(AR02);
        EV_scenQueueLoad(g_actNumber, w->nextArea, static_cast<std::int8_t>(w->routePass));
    } else if (w->entry) {
        const auto pass = static_cast<std::int8_t>(w->routePass);
        if (pass <= 3)
            EV_scenQueueLoad(g_actNumber, SHEN_NAME(AR02), pass + 70);
        else if (static_cast<std::uint8_t>(pass - 9) <= 2)
            EV_scenQueueLoad(g_actNumber, SHEN_NAME(AR02), pass + 65);
    }
    for (shen_name name : {CWON_CENTER, CWON_DUMMY, CWON_SECOND_DUMMY})
        SetCharaKill(name, 1);
    DestroyEscape(w);
    for (std::int32_t i = 0; i < 10; ++i)
        CwonDestroyTemporary(w, i);
    EV_FreeAuthorPak(0, "0011");
    EV_FreeAuthorPak(0, "0012");
    if (w->dummyTask)
        HLib::DestroyTaskInternal(w->dummyTask);
    DeleteMotions(w);
    if (w->routePass == 8)
        EV_stepSetFlags(0, 14);
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    if (retailHost && w->battleResult) {
        EV_NPC_NewLoadSw(0);
        EV_ForceLoadNpc(CWON_WON);
        Next(WaitLoad);
    } else {
        scenFreeEvent("CWON.EV3");
        EV_CleanupMapControlTask();
        HLib::EV_HLKillOtherHierarchyTask(w->task);
    }
}
} // namespace shenmue::s2
