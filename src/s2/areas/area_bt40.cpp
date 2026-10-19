#include "s2/areas/area_bt40.hpp"
#include "s2/pause.hpp"
#include "s2/areas/area_bt40_data.hpp"
#include "s2/author.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/flag.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <bit>
#include <cstring>
namespace shenmue::s2 {
namespace bt40 {
const shen_name *admittedCharacters{};
std::int32_t *completion{};
struct AuthorWork {
    HLib::Task *task;
    std::uint32_t reserved08;
    shen_name command;
    std::uint16_t phase;
    std::byte reserved12[6];
};
static_assert(sizeof(AuthorWork) == 0x18);
void NormalSequence(void *);
void ChallengeSequence(void *);
void FightSequence(void *);
void AuthorCallback(void *);
void Wait(CompiledSequenceWork &w, std::uint16_t mode, std::uint16_t next, int delay) {
    w.mode = mode;
    w.nextMode = next;
    w.continueAfterWait = 1;
    w.delay = delay;
}
void Transition(CompiledSequenceWork &w, std::uint16_t next) {
    w.previousMode = w.mode;
    w.mode = next;
    w.continueAfterWait = w.continueLoop = 1;
}
void QueueAuthor(shen_name command) {
    auto *task = HLib::EnqueueTaskWithParameter(AuthorCallback, 4, 0x0b, sizeof(AuthorWork), AREA_EVENT_TASK);
    auto *w = static_cast<AuthorWork *>(HLib::GetTaskParameter(task));
    w->task = task;
    w->command = command;
    w->phase = 1;
    SetPlayerControlEnabled(0);
}
void SetAuthorCharacters(bool restore, shen_name command) {
    if (!restore) {
        EnsureScenarioCharacterDraw(ryo);
        EnableScenarioCharacterDisplay(ryo);
        EV_SetCharaControllerFlags(ryo, 1, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(ryo, 3, NpcCharacterControllerState::Disabled);
    } else
        EnableScenarioCharacterDisplay(ryo);
    mtwkSetExtraFlag(scenFindMotionwork(ryo), 6, restore ? 0 : 2);
    EV_SetCharacterAlphaEnabled(ryo, restore);
    for (auto name : {SHEN_NAME(aa97), SHEN_NAME(aa98)}) {
        if (restore)
            DisableScenarioCharacterDisplay(name);
        else
            EnableScenarioCharacterDisplay(name);
        EV_SetCharaControllerFlags(name, 1, restore ? NpcCharacterControllerState::Enabled : NpcCharacterControllerState::Disabled);
        if (restore)
            EV_SetCharaControllerFlags(name, 2, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(name, 3, restore ? NpcCharacterControllerState::Enabled : NpcCharacterControllerState::Disabled);
        EV_SetCharacterAlphaEnabled(name, restore);
    }
    const shen_name *list = command.name == SHEN_NAME(AU_0) ? characters + 1 : command.name == SHEN_NAME(AU_1) ? challengeCharacters + 1 : nullptr;
    if (list)
        for (; list->name != 0xffffffffU; ++list) {
            if (!restore)
                EnsureScenarioCharacterDraw(*list);
            EnableScenarioCharacterDisplay(*list);
            if (!restore) {
                EV_SetCharaControllerFlags(*list, 1, NpcCharacterControllerState::Disabled);
                EV_SetCharaControllerFlags(*list, 3, NpcCharacterControllerState::Disabled);
            }
            mtwkSetExtraFlag(scenFindMotionwork(*list), 6, restore ? 0 : 2);
            EV_SetCharacterAlphaEnabled(*list, restore);
        }
}
void AuthorFrame(void *parameter) {
    auto &w = **static_cast<AuthorWork **>(parameter);
    const int frame = static_cast<int>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (!frame)
        SetAuthorCharacters(false, w.command);
    if (w.command.name == SHEN_NAME(AU_0)) {
        if (frame == EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining) - 10)
            SetScreenFilmFade(10, 0, 0, 0, 0, 255, 0, 0, 0);
    } else if (w.command.name == SHEN_NAME(AU_1)) {
        if (frame == 1) {
            DisableScenarioCharacterDisplay(SHEN_NAME(aa97));
            DisableScenarioCharacterDisplay(SHEN_NAME(aa98));
            EV_NPC_NewLoadSw(1);
        }
        if (frame == EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining) - 30)
            spmFadeOut();
    }
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    w.command = SHEN_NAME(AU_0);
    if (GetAuthorFlags())
        SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    w.phase = 3;
    SetAuthorCharacters(true, w.command);
    HLib::DestroyCurrentTask();
}
void AuthorCallback(void *parameter) {
    auto &w = *static_cast<AuthorWork *>(parameter);
    if (w.phase == 1) {
        auto *task = HLib::EnqueueTaskWithParameter(AuthorFrame, 4, 0x0b, sizeof(AuthorWork *), AREA_EVENT_TASK);
        *static_cast<AuthorWork **>(HLib::GetTaskParameter(task)) = &w;
        EV_spmBeginSampleExecute();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(w.command.name));
        w.phase = 2;
    } else if (w.phase == 3)
        HLib::DestroyTask(w.task);
}
void BattleSetup() {
    EV_LoadAudio("0340A000", 1);
    EV_spmBeginSampleExecute();
    DisableScenarioCharacterDisplay(SHEN_NAME(aa97));
    DisableScenarioCharacterDisplay(SHEN_NAME(aa98));
    for (int i = 0; i < 7; ++i)
        EV_AT_SetPos(characters[i], EventTransformFlags::World | EventTransformFlags::AllAxes, battlePositions[i]);
    for (int i = 0; i < 7; ++i)
        EV_AT_SetAngY(characters[i], EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, battleYaw[i], 0});
    for (int i = 0; i < 7; ++i)
        mtwkRequestMotion(scenFindMotionwork(characters[i]), i == 1 ? 0x29c1U : 306U, MotionRegion::LowerBody);
    for (int i = 0; i < 7; ++i)
        SetMotionRegionFlag(scenFindMotionwork(characters[i]), 71, 0, MotionRegion::LowerBody);
    EVS_LoopCamera(9001, shen_name{0xffffffffU}, shen_name{0xffffffffU});
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    for (int i = 0; i < 7; ++i)
        if (i != 1)
            EnableScenarioCharacterDisplay(characters[i]);
    DisableScenarioCharacterDisplay(joy);
    ToggleScenarioCharacterFlag(joy);
    EV_SetObjectFlagByIndex(1, 0);
}
void BeginBattle(Bt40BoutWork &owner, CompiledSequenceWork &w) {
    shenStartFGST(10, 10, 0, 0);
    vfmDispatchControl(FightControlOp::Package, {}, reinterpret_cast<std::uintptr_t>("BT40"), 1);
    const auto playerHp = static_cast<std::uint16_t>(owner.retries < 3 ? 330 : 30 * ((owner.retries > 12 ? 12 : owner.retries) + 9));
    shenInitFightData(ctrlGetCharaByName(ryo), nullptr, 0, 0, playerHp, 0);
    constexpr int hp[]{80, 100, 80, 140, 100};
    constexpr std::uint8_t styles[]{34, 35, 36, 38, 37};
    for (int i = 0; i < 5; ++i)
        shenInitFightData(
            ctrlGetCharaByName(characters[i + 2]), nullptr, 1, 1, static_cast<std::uint16_t>(difficultyScale[owner.difficulty] * hp[i]), styles[i]);
    for (int i = 0; i < 5; ++i)
        vfUpdateFlagsFor(ctrlGetCharaByName(characters[i + 2]), 0, 0x8000000U, difficultyFlags[owner.difficulty]);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800}, 1);
    std::int32_t zero = 0;
    std::memcpy(reinterpret_cast<std::byte *>(&w) + 0x1d0, &zero, 4);
    SetScreenFilmFade(10, 255, 0, 0, 0, 0, 0, 0, 0);
    Wait(w, 2, 101, 30);
}
void FightSequence(void *parameter) {
    auto &w = *static_cast<CompiledSequenceWork *>(parameter);
    auto &owner = *static_cast<Bt40BoutWork *>(w.event);
    w.callbackFlag = 0;
    do {
        w.continueLoop = 0;
        switch (w.mode) {
        case 0:
            BattleSetup();
            Wait(w, 2, 100, 1);
            break;
        case 12:
            if (!IsFightActive()) {
                owner.result = GetBtlStatus(ctrlGetCharaByName(ryo));
                if (owner.result == 3) {
                    EV_stepSetFlags(0, 879);
                    Transition(w, 108);
                } else
                    Transition(w, 105);
            }
            break;
        case 100:
            BeginBattle(owner, w);
            break;
        case 101:
            vfmQueueFightStart(6, 2, 1, 1, -1);
            Transition(w, 102);
            break;
        case 102:
            if (!IsFightActive())
                Transition(w, 12);
            else
                owner.rank = static_cast<int>(vfmDispatchControl(FightControlOp::BattleCounter, {}, 0, -1));
            break;
        case 105: {
            ++owner.retries;
            int adjustment = 0;
            switch (owner.rank) {
            case 0:
            case 1:
                adjustment = 4;
                break;
            case 2:
                adjustment = 3;
                break;
            case 3:
                adjustment = 2;
                break;
            case 4:
                adjustment = owner.lastAdjustment > 1;
                break;
            default:
                break;
            }
            owner.difficulty += adjustment;
            owner.lastAdjustment = adjustment;
            if (owner.difficulty > 15)
                owner.difficulty = 15;
            spmFadeOut();
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            Wait(w, 2, 106, 30);
            break;
        }
        case 106:
            Queue_ST_LoadTitleFadeIn(SHEN_NAME(RTR2));
            Wait(w, 2, 107, 60);
            break;
        case 107:
            ST_LoadTitleFade();
            Wait(w, 2, 109, 1);
            break;
        case 108:
            spmFadeOut();
            SetScreenFilmFade(5, 0, 0, 0, 0, 255, 0, 0, 0);
            Wait(w, 11, 109, 2);
            break;
        case 109:
            EnableScenarioCharacterDisplay(joy);
            EnsureScenarioCharacterDraw(joy);
            UpdateMotionWorkWithPosition(scenFindMotionwork(joy), nullptr, 0);
            figpAssignModeFor(ryo, 0);
            for (int i = 2; i < 7; ++i)
                figpAssignModeFor(characters[i], 0);
            RetireCompiledSequenceLinks(&w);
            HLib::DestroyCurrentTask();
            break;
        default:
            UpdateCompiledSequenceWait(&w);
            break;
        }
    } while (w.continueLoop);
}
void NormalSequence(void *parameter) {
    auto &w = *static_cast<CompiledSequenceWork *>(parameter);
    auto &owner = *static_cast<Bt40BoutWork *>(w.event);
    w.callbackFlag = 0;
    do {
        w.continueLoop = 0;
        switch (w.mode) {
        case 0:
            EV_SetObjectFlagByIndex(1, 1);
            for (int i = 0; i < 7; ++i)
                EV_SetCharaControllerFlags(characters[i], 1, NpcCharacterControllerState::Disabled);
            for (int i = 0; i < 7; ++i) {
                EV_SetCharaControllerFlags(characters[i], 2, NpcCharacterControllerState::Disabled);
                EV_SetCharaControllerFlags(characters[i], 3, NpcCharacterControllerState::Disabled);
            }
            for (int i = 0; i < 7; ++i)
                SetPluralControlFor(ctrlGetCharaByName(characters[i]), 1);
            Wait(w, 2, 100, 1);
            break;
        case 100:
            EV_spmBeginSampleExecute();
            Wait(w, 2, 101, 1);
            break;
        case 101:
            if (owner.retries) {
                EV_authSceneCtrl(static_cast<AuthSceneCtrl>(-16));
                EV_SetAuthorActiveTo(1);
            }
            QueueAuthor(SHEN_NAME(AU_0));
            Wait(w, 2, 102, 1);
            break;
        case 102:
            if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
                Wait(w, 2, 103, 1);
            break;
        case 103:
            if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
                if (GetAuthorFlags())
                    SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
                Wait(w, 2, 104, 1);
            }
            break;
        case 104:
            Wait(w, 2, 105, 1);
            return;
        case 105: {
            auto *child = QueueCompiledSequence(FightSequence, SHEN_NAME(BSBT), &owner);
            if (!w.children[0])
                w.children[0] = child;
            child->parent = &w;
            child->childSlot = 0;
            child->reference = w.reference + 1;
            w.timer = 0;
            std::uint32_t zero = 0;
            std::memcpy(w.parameters, &zero, 4);
            Wait(w, 10, 106, w.delay);
            w.waitChildIndex = 0;
            break;
        }
        case 106:
            if (owner.result != 3)
                Transition(w, 0);
            else {
                Wait(w, 2, 107, 1);
                return;
            }
            break;
        case 107: {
            for (int i = 3; i < 7; ++i)
                EV_npcLockUnlock(characters[i]);
            constexpr shen_name walkers[]{characters[3], characters[4], characters[5], characters[6], characters[2]};
            for (int i = 0; i < 5; ++i)
                PlaceNpcAtAreaPoint(walkers[i], walkAreas[i], nullptr, walkYaw[i]);
            for (int i = 0; i < 5; ++i)
                mtwkRequestMotion(scenFindMotionwork(walkers[i]), (i == 1 || i == 2) ? 0x9605U : 0x9607U, MotionRegion::LowerBody);
            Wait(w, 2, 108, 10);
            break;
        }
        case 108:
            QueueAuthor(SHEN_NAME(AU_1));
            Wait(w, 2, 109, 1);
            break;
        case 109:
            if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
                Wait(w, 2, 110, 1);
            break;
        case 110:
            if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
                Wait(w, 2, 111, 1);
            break;
        case 111:
            for (int i = 0; i < 7; ++i)
                if (i < 2 || DoesScenarioCharacterExist(characters[i]))
                    SetPluralControlFor(ctrlGetCharaByName(characters[i]), 0);
            owner.active = 0;
            RetireCompiledSequenceLinks(&w);
            HLib::DestroyCurrentTask();
            break;
        default:
            UpdateCompiledSequenceWait(&w);
            break;
        }
    } while (w.continueLoop);
}
void ChallengeSequence(void *parameter) {
    auto &w = *static_cast<CompiledSequenceWork *>(parameter);
    auto &owner = *static_cast<Bt40BoutWork *>(w.event);
    w.callbackFlag = 0;
    do {
        w.continueLoop = 0;
        switch (w.mode) {
        case 0:
            EV_SetObjectFlagByIndex(1, 1);
            for (auto name : {ryo, joy})
                EV_SetCharaControllerFlags(name, 1, NpcCharacterControllerState::Disabled);
            for (auto name : {ryo, joy}) {
                EV_SetCharaControllerFlags(name, 2, NpcCharacterControllerState::Disabled);
                EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Disabled);
            }
            for (auto name : {ryo, joy})
                SetPluralControlFor(ctrlGetCharaByName(name), 1);
            Wait(w, 2, 100, 1);
            break;
        case 100: {
            EV_spmBeginSampleExecute();
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
            EventSetPosition pos{0, base, shen_name{0xffffffffU}, {257, 244, 134}, 0};
            EVT_AT_SetPosRot(&pos);
            EventSetAngle angle{0, base, shen_name{0xffffffffU}, 240, 0};
            EVT_AT_SetAngY_Callback(&angle);
            const float keep = std::bit_cast<float>(0xffffffffU);
            pos = {0, ryo, base, {keep, keep, keep}, 0};
            EVT_AT_SetPosRot(&pos);
            angle = {0, ryo, base, keep, 0};
            EVT_AT_SetAngY_Callback(&angle);
            EventSetMotion motion{0, ryo, 10691, 0, 1, 0x40000000U, 0};
            EVT_AT_SetMot_Callback(&motion);
            pos = {0, joy, base, {0, 0, -2}, 0};
            EVT_AT_SetPosRot(&pos);
            angle = {0, joy, base, 180, 0};
            EVT_AT_SetAngY_Callback(&angle);
            motion = {0, joy, 10690, 0, 1, 0x40000000U, 0};
            EVT_AT_SetMot_Callback(&motion);
            EVT_AT_SetVoice("0340C005", 1, 0); // @text EN: "Ryo." | JA: "涼。"
            EVT_AT_SetVoice("0340C006", 1, 30); // @text EN: "You've got to hurry and save Wong!" | JA: "早くウォンを助けに行かないと！"
            EVT_AT_SetVoice("0340A010", 1, 100); // @text EN: "Yeah. Let's hurry." | JA: "ああ、急ごう！"
            EventSetCamera camera{0, 9100, base, base, 0, {}, 0, 0};
            EVT_AT_SetCam_Callback(&camera);
            camCtrlDisableFilmBars();
            EVT_AT_SetCam(9101, base, base, 0, 0, 110);
            Wait(w, 2, 101, 1);
            break;
        }
        case 101: {
            Vector3 p{};
            EV_GetCharaPosition(joy, 5, &p, EventTransformFlags::World);
            UpdateMotionWorkWithPosition(scenFindMotionwork(ryo), &p, 0);
            UpdateMotionWorkWithPosition(scenFindMotionwork(ryo), nullptr, 3);
            EV_GetCharaPosition(ryo, 5, &p, EventTransformFlags::World);
            UpdateMotionWorkWithPosition(scenFindMotionwork(joy), &p, 0);
            UpdateMotionWorkWithPosition(scenFindMotionwork(joy), nullptr, 3);
            Wait(w, 2, 102, 105);
            break;
        }
        case 102:
            UpdateMotionWorkWithPosition(scenFindMotionwork(ryo), &leavePosition, 0);
            Wait(w, 2, 103, 80);
            break;
        case 103:
            spmFadeOut();
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            Wait(w, 2, 104, 1);
            break;
        case 104:
            if (!filmCheckFadeTrackColour()) {
                UpdateMotionWorkWithPosition(scenFindMotionwork(ryo), nullptr, 0);
                UpdateMotionWorkWithPosition(scenFindMotionwork(joy), nullptr, 0);
                Wait(w, 2, 111, 1);
            }
            break;
        case 111:
            for (auto name : {ryo, joy})
                SetPluralControlFor(ctrlGetCharaByName(name), 0);
            owner.active = 0;
            RetireCompiledSequenceLinks(&w);
            HLib::DestroyCurrentTask();
            break;
        default:
            UpdateCompiledSequenceWait(&w);
            break;
        }
    } while (w.continueLoop);
}
void Exit(Bt40BoutWork &w) {
    const auto mode = w.mode;
    const auto result = w.result;
    ET_AT_KillWork(w.characters);
    mtwkRequestMotion(scenFindMotionwork(ryo), 0xf004, MotionRegion::LowerBody);
    SetMotionRegionFlag(scenFindMotionwork(ryo), 71, 0, MotionRegion::LowerBody);
    if (DoesScenarioCharacterExist(joy)) {
        mtwkRequestMotion(scenFindMotionwork(joy), 0xf05f, MotionRegion::LowerBody);
        SetMotionRegionFlag(scenFindMotionwork(joy), 71, 0, MotionRegion::LowerBody);
    }
    flddSetPointFlagOnGrid(SHEN_NAME(BT40), 0);
    flddSetPointFlagOnGrid(SHEN_NAME(0000), 1);
    SetCharaKill(base, 1);
    EV_FreeAuthorPak(0, "0340");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("BT40"), 0);
    for (auto *motion = w.motions; *motion; ++motion)
        DeleteMotion(*motion);
    if (!mode)
        LoadCutsceneAndSoundProgramForArea();
    SetRenderClockAndCompassTo(0);
    FinishLoading();
    EV_SetObjectFlagByIndex(1, 1);
    EV_lightLoad(SHEN_NAME(0001));
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(w.task);
    if (mode) {
        if (completion)
            *completion = 0;
    } else if (result == 3) {
        EV_spmBeginSampleExecute();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        SetPlayerControlEnabled(1);
        camCtrlSetState(CameraState::Init);
        camCtrlSetFovTransitionTo(true);
        EV_NPC_NewLoadSw(1);
    } else
        EV_scenQueueLoad(g_actNumber, SHEN_NAME(QF39), 1);
}
} // namespace bt40
void BT40_init_func() {
    bt40::admittedCharacters = nullptr;
    bt40::completion = nullptr;
}
void BT40_event_function(std::uintptr_t parameter) {
    using namespace bt40;
    SetPlayerControlEnabled(0);
    auto *task = HLib::EnqueueTaskWithParameter(Bt40BoutCallback, 4, 0x0b, sizeof(Bt40BoutWork), AREA_EVENT_TASK);
    auto &w = *static_cast<Bt40BoutWork *>(HLib::GetTaskParameter(task));
    w.task = task;
    w.phase = 0;
    std::uint32_t zero = 0;
    std::memcpy(reinterpret_cast<std::byte *>(&w) + 0x18, &zero, 4);
    w.difficulty = 0;
    w.retries = w.lastAdjustment = 0;
    if (static_cast<std::intptr_t>(parameter) >= 256) {
        completion = reinterpret_cast<std::int32_t *>(parameter);
        w.mode = 1;
    } else {
        completion = nullptr;
        w.mode = static_cast<std::intptr_t>(parameter);
    }
    admittedCharacters = w.mode ? challengeCharacters : characters;
    w.characters = Queue_EVT_AT_ReadChr(task, admittedCharacters);
    if (w.mode)
        SetPlayerGameFlags(static_cast<PlayerGameFlagMode>(22), nullptr);
    w.motions[0] = 0;
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("BT40"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("BT40"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, base);
    EV_FreeAuthorPak(1, "0340");
    if (!w.mode) {
        EV_SetNewSceneIndex();
        EV_LoadCutsceneFromCurrentScene("0340A000");
        LoadBlurModelForCharacter(ryo, "RYO_B.MT7", 1);
        EV_LoadAudio("0340A000", 0);
    }
    SetMapControlStatusFlag(0, 0);
    SetRenderClockAndCompassTo(1);
    FinishLoading();
}
void Bt40BoutCallback(void *parameter) {
    using namespace bt40;
    auto &w = *static_cast<Bt40BoutWork *>(parameter);
    switch (w.phase) {
    case 0:
        w.phase = 4;
        break;
    case 4:
        flddSetPointFlagOnGrid(SHEN_NAME(0000), 0);
        flddSetPointFlagOnGrid(SHEN_NAME(BT40), 1);
        w.sequence = QueueCompiledSequence(w.mode ? ChallengeSequence : NormalSequence, SHEN_NAME(SEQP), &w);
        w.active = 1;
        w.phase = 5;
        break;
    case 5:
        if (!w.active)
            w.phase = 6;
        break;
    case 6:
        w.phase = 7;
        break;
    case 7:
        w.phase = 8;
        break;
    case 8:
        w.phase = 9;
        break;
    case 9:
        Exit(w);
        break;
    default:
        break;
    }
}
const ScriptAreaDefinition areaBT40ScriptDefinition{.areaId = SHEN_NAME(BT40), .field04 = 0, .init = BT40_init_func, .main = BT40_event_function};
} // namespace shenmue::s2
