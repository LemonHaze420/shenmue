#include "s2/areas/area_q7qb.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/face.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/map_control.hpp"
#include "s2/load_title.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/qte_hand_poses.hpp"
#include "s2/scene_data.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"

namespace shenmue::s2::areas {
namespace {
void Q7QbFirstAttackCallback(void *);
void Q7QbSecondAttackCallback(void *);
void Q7QbThirdAttackCallback(void *);
void Q7QbFourthAttackCallback(void *);
void Q7QbFifthAttackCallback(void *);
void Q7QbSixthAttackCallback(void *);
void Q7QbFinalAttackCallback(void *);
void Q7QbStartCallback(void *);
void Q7QbStartResultAuthorCallback(void *);
void Q7QbWaitResultAuthorCallback(void *);
void Q7QbWaitMotionCallback(void *);
void Q7QbWaitIntroCallback(void *);
void Q7QbEndCallback(void *);

// @verified - p
void Q7QbSetupNullsub() {
}

void Q7QbSetup(Q7QbWork &work) {
    EVT_SetQteRetryFrames(24);
    EVT_SetQteFailureCount(0);
    GetCurrentDateTime(eventTemporaryDateTime);
    Vector3 position;
    EV_GetCharaPosition(Q7QB_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    work.baseHeight = position.y;
    work.characters = Queue_EVT_AT_ReadChr(work.task, Q7QB_CHARACTERS);
    work.succeeded = 1;
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("0290"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("Q7QB"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("Q7QB"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("Q7QB"), 0);
    EV_FreeAuthorPak(1, "S2BV");
    EV_FreeAuthorPak(1, "0290");
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0290A000");
    SetRenderClockAndCompassTo(1);
    Q7QbSetupNullsub();
    qteBattleUtil(BattleQteMode::Load);
    qteBattleUtil(BattleQteMode::Queue);
}
void Q7QbStartCallback(void *data) {
    Q7QbWork &work = *static_cast<Q7QbWork *>(data);
    work.resultParameters = {.player = Q7QB_RYO, .firstOpponent = Q7QB_KK9, .secondOpponent = Q7QB_KK7, .motionFlags = 0x44000000U};
    work.continuation = Q7QbFirstAttackCallback;
    work.phase = 0;
    SetLowQualityHands(FindCharacterByScenarioName(Q7QB_RYO));
    EventSetPosition position{
        .cleanup = 0, .character = Q7QB_BASE, .parent = Q7QB_INVALID, .position = {.x = 198.1f, .y = work.baseHeight, .z = 161.9f}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = Q7QB_BASE, .parent = Q7QB_INVALID, .degrees = 180, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position = {.cleanup = 0, .character = Q7QB_RYO, .parent = Q7QB_BASE, .position = Q7QB_RYO_POSITION, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = Q7QB_RYO, .parent = Q7QB_BASE, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    position.character = Q7QB_REN;
    position.position = Q7QB_REN_POSITION;
    EVT_AT_SetPosRot(&position);
    angle.character = Q7QB_REN;
    EVT_AT_SetAngY_Callback(&angle);
    position.character = Q7QB_CAMERA_DUMMY;
    position.position = Q7QB_CAMERA_POSITION;
    EVT_AT_SetPosRot(&position);
    angle.character = Q7QB_CAMERA_DUMMY;
    angle.degrees = -90;
    EVT_AT_SetAngY_Callback(&angle);
    for (shen_name character : Q7QB_HIDDEN_CHARACTERS) {
        position.character = character;
        position.position = Q7QB_HIDDEN_POSITION;
        EVT_AT_SetPosRot(&position);
        angle.character = character;
        angle.degrees = character.name == Q7QB_KN1.name || character.name == Q7QB_RIK.name || character.name == Q7QB_SBK.name ? 180.0f : 0.0f;
        EVT_AT_SetAngY_Callback(&angle);
    }
    AT_SetLocalBaseChara(Q7QB_DUMMY, Q7QB_RYO);
    position = {.cleanup = 0, .character = Q7QB_DUMMY, .parent = Q7QB_RYO, .position = {}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    angle = {.cleanup = 0, .character = Q7QB_DUMMY, .parent = Q7QB_RYO, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    for (shen_name character : Q7QB_STANDING_CHARACTERS) {
        EventSetMotion motion{.cleanup = 0, .character = character, .animation = 13, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(13);
    }
    for (shen_name character : Q7QB_PLURAL_CHARACTERS)
        SetPluralControlFor(FindCharacterByScenarioName(character), 0);
    EV_LoadAudio("0290A000", 1);
    EV_spmBeginSampleExecute();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(ATH1)));
    HLib::SetCurrentTaskTo(Q7QbWaitIntroCallback);
}
void Q7QbWaitIntroCallback(void *data) {
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    Q7QbWork &work = *static_cast<Q7QbWork *>(data);
    Character *character = FindCharacterByScenarioName(Q7QB_RYO);
    SetHighModelXYZ(character, 1, 0, 0);
    SetHighModelXYZ(character, 1, 1, 0);
    ApplyCharacterHandPoseCommand(character, 0, reinterpret_cast<std::byte *>(ryoQteHandPose), 0);
    ApplyCharacterHandPoseCommand(character, 1, reinterpret_cast<std::byte *>(ryoQteHandPose), 0);
    SetHighModelAndFaceFlagsForChara(character, static_cast<std::int32_t>(FacialExpressionId::Angry), 0, 1);
    EventSetPositionAngle position{
        .cleanup = 0, .character = Q7QB_KK8, .parent = Q7QB_BASE, .position = {.x = -1000, .y = 0, .z = 0}, .degrees = 0, .delay = 0};
    EVT_AT_SetPosAng_Callback(&position);
    HLib::SetCurrentTaskTo(work.continuation);
}
void Q7QbFirstAttackCallback(void *data) {
    Q7QbWork &work = *static_cast<Q7QbWork *>(data);
    const std::int32_t phase = work.phase++;
    if (!phase) {
        work.resultParameters.firstOpponent = Q7QB_KK9;
        const std::uint32_t delay = Queue_EVT_AT_ResultQTE(&work.succeeded, &work.resultParameters, &Q7QB_QTE_DATA[0]);
        HLib::SetupTaskDelay(static_cast<std::int32_t>(delay));
    } else if (phase == 1) {
        if (!work.succeeded) {
            HLib::SetCurrentTaskTo(Q7QbEndCallback);
            HLib::SetupTaskDelay(60);
        }
    } else {
        work.phase = 0;
        work.continuation = Q7QbSecondAttackCallback;
        HLib::SetCurrentTaskTo(work.continuation);
        HLib::SetupTaskDelay(15);
    }
}
void Q7QbSecondAttackCallback(void *data) {
    Q7QbWork &work = *static_cast<Q7QbWork *>(data);
    const std::int32_t phase = work.phase++;
    if (!phase) {
        work.resultParameters.firstOpponent = Q7QB_KK7;
        const std::uint32_t delay = Queue_EVT_AT_ResultQTE(&work.succeeded, &work.resultParameters, &Q7QB_QTE_DATA[1]);
        HLib::SetupTaskDelay(static_cast<std::int32_t>(delay));
    } else if (phase == 1) {
        if (!work.succeeded) {
            HLib::SetCurrentTaskTo(Q7QbEndCallback);
            HLib::SetupTaskDelay(60);
        }
    } else {
        work.phase = 0;
        work.continuation = Q7QbThirdAttackCallback;
        HLib::SetCurrentTaskTo(work.continuation);
        HLib::SetupTaskDelay(30);
    }
}
void Q7QbThirdAttackCallback(void *data) {
    Q7QbWork &work = *static_cast<Q7QbWork *>(data);
    const std::int32_t phase = work.phase++;
    if (!phase) {
        work.resultParameters.firstOpponent = Q7QB_KK8;
        const std::uint32_t delay = Queue_EVT_AT_ResultQTE(&work.succeeded, &work.resultParameters, &Q7QB_QTE_DATA[2]);
        HLib::SetupTaskDelay(static_cast<std::int32_t>(delay));
    } else if (phase == 1) {
        if (!work.succeeded) {
            HLib::SetCurrentTaskTo(Q7QbEndCallback);
            HLib::SetupTaskDelay(60);
        }
    } else {
        work.phase = 0;
        work.continuation = Q7QbFourthAttackCallback;
        HLib::SetCurrentTaskTo(work.continuation);
        HLib::SetupTaskDelay(30);
    }
}
void Q7QbFourthAttackCallback(void *data) {
    Q7QbWork &work = *static_cast<Q7QbWork *>(data);
    const std::int32_t phase = work.phase++;
    if (!phase) {
        work.resultParameters.firstOpponent = Q7QB_KKA;
        const std::uint32_t delay = Queue_EVT_AT_ResultQTE(&work.succeeded, &work.resultParameters, &Q7QB_QTE_DATA[3]);
        HLib::SetupTaskDelay(static_cast<std::int32_t>(delay));
    } else if (phase == 1) {
        if (!work.succeeded) {
            HLib::SetCurrentTaskTo(Q7QbEndCallback);
            HLib::SetupTaskDelay(60);
        }
    } else {
        work.phase = 0;
        work.continuation = Q7QbFifthAttackCallback;
        HLib::SetCurrentTaskTo(work.continuation);
        HLib::SetupTaskDelay(30);
    }
}
void Q7QbFifthAttackCallback(void *data) {
    Q7QbWork &work = *static_cast<Q7QbWork *>(data);
    const std::int32_t phase = work.phase++;
    if (!phase) {
        work.resultParameters.firstOpponent = Q7QB_KN1;
        const std::uint32_t delay = Queue_EVT_AT_ResultQTE(&work.succeeded, &work.resultParameters, &Q7QB_QTE_DATA[4]);
        HLib::SetupTaskDelay(static_cast<std::int32_t>(delay));
    } else if (phase == 1) {
        if (!work.succeeded) {
            HLib::SetCurrentTaskTo(Q7QbEndCallback);
            HLib::SetupTaskDelay(60);
        }
    } else {
        work.phase = 0;
        work.continuation = Q7QbSixthAttackCallback;
        HLib::SetCurrentTaskTo(work.continuation);
        HLib::SetupTaskDelay(60);
    }
}
void Q7QbSixthAttackCallback(void *data) {
    Q7QbWork &work = *static_cast<Q7QbWork *>(data);
    const std::int32_t phase = work.phase++;
    if (!phase) {
        work.resultParameters.firstOpponent = Q7QB_KN1;
        work.resultParameters.secondOpponent = Q7QB_KN2;
        const std::uint32_t delay = Queue_EVT_AT_ResultQTE(&work.succeeded, &work.resultParameters, &Q7QB_QTE_DATA[5]);
        HLib::SetupTaskDelay(static_cast<std::int32_t>(delay));
    } else if (phase == 1) {
        if (!work.succeeded) {
            HLib::SetCurrentTaskTo(Q7QbEndCallback);
            HLib::SetupTaskDelay(60);
        }
    } else {
        work.phase = 0;
        work.continuation = Q7QbFinalAttackCallback;
        HLib::SetCurrentTaskTo(work.continuation);
        HLib::SetupTaskDelay(60);
    }
}
void Q7QbFinalAttackCallback(void *data) {
    Q7QbWork &work = *static_cast<Q7QbWork *>(data);
    const std::int32_t phase = work.phase++;
    if (!phase) {
        for (shen_name character : Q7QB_PLURAL_CHARACTERS)
            SetPluralControlFor(FindCharacterByScenarioName(character), 1);
        work.resultParameters.firstOpponent = Q7QB_RIK;
        work.resultParameters.secondOpponent = Q7QB_SBK;
        HLib::SetupTaskDelay(static_cast<std::int32_t>(Queue_EVT_AT_ResultQTE(&work.succeeded, &work.resultParameters, &Q7QB_QTE_DATA[6])));
    } else if (phase == 1) {
        if (work.succeeded) {
            HLib::SetupTaskDelay(static_cast<std::int32_t>(Queue_EVT_AT_ResultQTE(&work.succeeded, &work.resultParameters, &Q7QB_QTE_DATA[7])));
        } else {
            HLib::SetCurrentTaskTo(Q7QbEndCallback);
            HLib::SetupTaskDelay(120);
        }
    } else if (phase == 2) {
        if (work.succeeded)
            HLib::SetupTaskDelay(25);
        else {
            HLib::SetCurrentTaskTo(Q7QbEndCallback);
            HLib::SetupTaskDelay(120);
        }
    } else if (phase == 3) {
        qteBattleUtil(BattleQteMode::SetFirstCommands, 1, 256);
        ReInitElement(UIElement::GameQte);
        std::int16_t maximum = 16;
        if (g_eventQteRetryFrames > 0)
            maximum = static_cast<std::int16_t>(static_cast<std::int16_t>(EVT_GetQteFailureCount()) * ((g_eventQteRetryFrames - 16) / 2) + 16);
        EventQtePromptTaskData prompt{
            .autoCleanupTask = 0, .requiredStatus = 0, .maximumFrames = maximum, .reserved08 = 0, .displayState = 4, .delayFrames = 0};
        EVT_AT_Qte_Callback(&prompt);
    } else if (qteBattleUtil(BattleQteMode::GetResult)) {
        if (qteBattleUtil(BattleQteMode::GetCompletedCount) == 2) {
            work.succeeded = 1;
            work.phase = 0;
            work.continuation = Q7QbStartResultAuthorCallback;
            Q7QbBattleSuccess(&work.resultParameters, 0);
            HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_MenCtrl_Callback, 6, 0x0b, sizeof(EventFilmControl), Q7QB_TASK);
            RegisterEventTask(task);
            EventFilmControl *film = static_cast<EventFilmControl *>(HLib::GetTaskParameter(task));
            *film = {.cleanup = 1,
                .frames = 30,
                .startAlpha = 0,
                .startRed = 0,
                .startGreen = 0,
                .startBlue = 0,
                .targetAlpha = 255,
                .targetRed = 0,
                .targetGreen = 0,
                .targetBlue = 0,
                .delay = 70};
            HLib::SetCurrentTaskTo(Q7QbWaitMotionCallback);
            HLib::SetupTaskDelay(100);
        } else {
            work.succeeded = 0;
            Q7QbFinalCounterFailure(&work.resultParameters, 0);
            HLib::SetCurrentTaskTo(Q7QbEndCallback);
            HLib::SetupTaskDelay(120);
        }
    }
}
void Q7QbWaitMotionCallback(void *data) {
    if (SetMotionRegionFlag(scenFindMotionwork(Q7QB_RYO), 65, 1, MotionRegion::LowerBody))
        HLib::SetCurrentTaskTo(static_cast<Q7QbWork *>(data)->continuation);
}
void Q7QbStartResultAuthorCallback(void *data) {
    Q7QbWork &work = *static_cast<Q7QbWork *>(data);
    RestoreEventCharacter(work.characters);
    RestoreEventCharacter(work.characters);
    RestoreEventCharacter(work.characters);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(ATH2)));
    work.continuation = Q7QbEndCallback;
    HLib::SetCurrentTaskTo(Q7QbWaitResultAuthorCallback);
}
void Q7QbWaitResultAuthorCallback(void *data) {
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        spmFadeOut();
        HLib::SetCurrentTaskTo(static_cast<Q7QbWork *>(data)->continuation);
        HLib::SetupTaskDelay(60);
    }
}
void Q7QbCleanup(Q7QbWork &work) {
    hndlSetFlags(FindCharacterByScenarioName(Q7QB_RYO));
    qteBattleUtil(BattleQteMode::RetireAndFree);
    FinishLoading();
    SetRenderClockAndCompassTo(0);
    LoadCutsceneAndSoundProgramForArea();
    EV_spmBeginSampleExecute();
    EV_FreeAuthorPak(0, "0290");
    EV_FreeAuthorPak(0, "S2BV");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("Q7QB"), 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0290"), 0);
    ET_AT_KillWork(work.characters);
    EventFilmControl film{.cleanup = 0,
        .frames = 30,
        .startAlpha = 255,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 0,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 0};
    EVT_AT_MenCtrl_Callback(&film);
    EV_NPC_NewLoadSw(1);
}
void Q7QbEndCallback(void *data) {
    Q7QbWork &work = *static_cast<Q7QbWork *>(data);
    if (work.succeeded) {
        Q7QbCleanup(work);
        EV_CleanupMapControlTask();
        HLib::EV_HLKillOtherHierarchyTask(work.task);
        if (g_currentMap.name == Q7QB_AREA.name) {
            scenSetTaskFlags(6);
            EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
        }
    } else {
        const std::int32_t delay = PlayStandingQteRetryVoices();
        EventFilmControl film{.cleanup = 0,
            .frames = 30,
            .startAlpha = 0,
            .startRed = 0,
            .startGreen = 0,
            .startBlue = 0,
            .targetAlpha = 255,
            .targetRed = 0,
            .targetGreen = 0,
            .targetBlue = 0,
            .delay = 0};
        EVT_AT_MenCtrl_Callback(&film);
        EVT_AT_FadeBGM(0, 1, delay - 60);
        if (EVT_GetQteFailureCount() < 2)
            EVT_SetQteFailureCount(EVT_GetQteFailureCount() + 1);
        SetCurrentDateTimeTo(eventTemporaryDateTime);
        shenSetTimeSimulationTo(1);
        HLib::SetCurrentTaskTo(Q7QbStartCallback);
        HLib::SetupTaskDelay(delay);
    }
}
} // namespace
std::int32_t PlayStandingQteRetryVoices() {
    if (GetRandomNumberInRange(100) >= 50) {
        EVT_AT_SetVoice("S2BVC007", 1, 21);
        EVT_AT_SetVoice("S2BVC009", 1, 57);
        EVT_AT_SetVoice("S2BVA012", 1, 105);
        EVT_AT_SetSound(649, 62);
        EVT_AT_SetSound(650, 76);
        EVT_AT_SetSound(132, 87);
        EVT_AT_SetSound(653, 98);
        EVT_AT_SetSound(651, 114);
        EVT_AT_SetSound(652, 130);
        EVT_AT_SetSound(649, 140);
        EVT_AT_SetSound(628, 155);
        EVT_AT_SetSound(652, 160);
        EVT_AT_SetSound(650, 187);
        EVT_AT_SetSound(628, 52);
        EVT_AT_SetSound(629, 58);
        EVT_AT_SetSound(132, 127);
        EVT_AT_SetSound(133, 165);
        EVT_AT_SetSound(649, 194);
        EVT_AT_SetSound(628, 160);
        EVT_AT_SetSound(629, 166);
        return 260;
    }
    EVT_AT_SetVoice("S2BVB010", 1, 23);
    EVT_AT_SetVoice("S2BVA010", 1, 92);
    EVT_AT_SetSound(653, 53);
    EVT_AT_SetSound(651, 85);
    EVT_AT_SetSound(652, 115);
    EVT_AT_SetSound(652, 131);
    EVT_AT_SetSound(650, 142);
    EVT_AT_SetSound(649, 158);
    EVT_AT_SetSound(628, 45);
    EVT_AT_SetSound(629, 51);
    EVT_AT_SetSound(649, 58);
    EVT_AT_SetSound(650, 70);
    return 225;
}
// @verified - p
void InitializeQ7QbEvent() {
}
void RunQ7QbEvent(std::uintptr_t) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(Q7QbStartCallback, 4, 0x0b, sizeof(Q7QbWork), Q7QB_TASK);
    Q7QbWork *work = static_cast<Q7QbWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    Q7QbSetup(*work);
}
const ScriptAreaDefinition q7qbScriptDefinition{.areaId = Q7QB_AREA, .field04 = 0, .init = InitializeQ7QbEvent, .main = RunQ7QbEvent};
} // namespace shenmue::s2::areas
