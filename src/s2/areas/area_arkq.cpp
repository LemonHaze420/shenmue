#include "s2/areas/area_arkq.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_call.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/inventory.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/model_image.hpp"
#include "s2/motion_work.hpp"
#include "s2/pak.hpp"
#include "s2/particle_effects.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/snapshot.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"

namespace shenmue::s2::areas {
ArkqGlobals arkqGlobals;
namespace {
void ArkqSetupNullsub() {
}
void ArkqSuccessAuthorNullsub(void *) {
}

void SetupArkqCharacters() {
    CreateEventCharacter(CharacterCreationHighModel, nullptr, ARKQ_BASE);
    EVT_AT_SetPos(ARKQ_BASE, ARKQ_INVALID, {.x = 329.5f, .y = 95.3f, .z = 597.5f}, 0);
    EVT_AT_SetAngY(ARKQ_BASE, ARKQ_INVALID, 90, 0);
    EVT_AT_SetPos(ARKQ_DUMMY, ARKQ_BASE, {.x = 6, .y = 0, .z = -7.9f}, 0);
    EVT_AT_SetAngY(ARKQ_DUMMY, ARKQ_BASE, 180, 0);
    EVT_AT_SetPos(ARKQ_RYO, ARKQ_BASE, {.x = 10.83f, .y = 0, .z = -2.3943f}, 0);
    EVT_AT_SetAngY(ARKQ_RYO, ARKQ_BASE, 450, 0);
    EVT_AT_SetPos(ARKQ_REN, ARKQ_BASE, {.x = 4.0601997f, .y = 0, .z = -10.9572f}, 0);
    EVT_AT_SetAngY(ARKQ_REN, ARKQ_BASE, 224.01669f, 0);
    EVT_AT_SetPos(ARKQ_CLJ, ARKQ_BASE, {.x = 2.459f, .y = 0, .z = -10.296f}, 0);
    EVT_AT_SetAngY(ARKQ_CLJ, ARKQ_BASE, 229.032f, 0);
    EVT_AT_SetPos(ARKQ_SAM, ARKQ_BASE, {.x = 6.8f, .y = 0, .z = -10.3f}, 0);
    EVT_AT_SetAngY(ARKQ_SAM, ARKQ_BASE, 180, 0);
    EVT_AT_SetPos(ARKQ_LLY, ARKQ_BASE, {.x = 7.8f, .y = 0, .z = -10.9f}, 0);
    EVT_AT_SetAngY(ARKQ_LLY, ARKQ_BASE, 180, 0);
    for (shen_name character : ARKQ_SPECTATORS)
        SetMotionRegionFlag(scenFindMotionwork(character), 66, 0, MotionRegion::LowerBody);
}
std::int32_t SetupArkqResources(ArkqWork &work) {
    DateTime time;
    GetCurrentDateTime(time);
    if (time.hour >= 19 || (time.hour == 6 && time.minute >= 30)) {
        SetIsSkySetup(0);
        EV_SetObjectFlagByIndex(1, 0);
    }
    qteBattleUtil(BattleQteMode::SetWait, 0, 6, 40);
    qteBattleUtil(BattleQteMode::SetFirstCommands, 64, 4, 256);
    qteBattleUtil(BattleQteMode::SetLastCommands);
    qteBattleUtil(BattleQteMode::ResetStates);
    qteBattleUtil(BattleQteMode::Load);
    qteBattleUtil(BattleQteMode::Queue);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("MPK00"), 0);
    EV_LoadCutsceneFromCurrentScene("0057A000");
    EV_LoadAudio("0057A000", 0);
    EV_spmBeginSampleExecute();
    EV_FreeAuthorPak(1, "0057");
    SetupArkqCharacters();
    SetWeatherUpdateEnabledTo(0);
    EV_SetWeatherInstruction(15, 0);
    EV_SetWeatherInstruction(0x780, 0);
    for (const shen_name *character = ARKQ_CHARACTERS; character->name != ARKQ_INVALID.name; ++character)
        SetLowQualityHands(ctrlGetCharaByName(*character));
    GetCharacterMotionCompatibilityResult(ctrlGetCharaByName(ARKQ_RYO));
    GetCharacterMotionCompatibilityResult(ctrlGetCharaByName(ARKQ_REN));
    LoadBlurModelForCharacter(ARKQ_REN, "REN_B.MT7", 1);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(ARKQ_LIGHT);
    PushGlobalLightingProgram();
    work.state = 2;
    return 0;
}
std::int32_t BeginArkqIntro(ArkqWork &work) {
    EventFilmControl film{.cleanup = 0,
        .frames = 60,
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
    AT_SetLocalBaseChara(ARKQ_KNIFE, ARKQ_INVALID);
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_SK_AuthStart_Callback, 3, 0x0b, sizeof(StepAuthorStartWork), ARKQ_EVENT);
    StepAuthorStartWork *child = static_cast<StepAuthorStartWork *>(HLib::GetTaskParameter(task));
    child->task = task;
    child->parent = work.task;
    child->command = ARKQ_INTRO.name;
    child->callback = ArkqIntroAuthorCallback;
    child->phase = 1;
    child->callbackData = &work;
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(ARKQ_INTRO.name));
    child->callback(child->callbackData);
    HLib::SetupTimedTask(child->parent, 0);
    work.state = 5;
    snapAssignFrmCntFlags(1, 575);
    return 0;
}
std::int32_t BeginArkqSuccessAuthor(ArkqWork &work) {
    EV_DisableCharacterDisplay(ARKQ_KNIFE);
    EV_EnableCharacterDisplay(ARKQ_HAND_ANCHOR);
    EventCling detach;
    detach.cleanup = 0;
    detach.target = ARKQ_HAND_ANCHOR;
    detach.character = {};
    detach.mode = 1;
    detach.delay = 0;
    EVT_AT_Cling(&detach);
    AT_SetLocalBaseChara(ARKQ_KNIFE, shen_name{});
    EV_SetWeatherInstruction(12, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(ARKQ_SUCCESS_LIGHT);
    PushGlobalLightingProgram();
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_SK_AuthStart_Callback, 3, 0x0b, sizeof(StepAuthorStartWork), ARKQ_EVENT);
    StepAuthorStartWork *child = static_cast<StepAuthorStartWork *>(HLib::GetTaskParameter(task));
    child->task = task;
    child->parent = work.task;
    child->command = ARKQ_SUCCESS_AUTHOR.name;
    child->callback = ArkqSuccessAuthorNullsub;
    child->phase = 1;
    child->callbackData = nullptr;
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(ARKQ_SUCCESS_AUTHOR.name));
    child->callback(child->callbackData);
    HLib::SetupTimedTask(child->parent, 0);
    work.state = 8;
    snapAssignFrmCntFlags(898, 20);
    return 0;
}
std::int32_t BeginArkqFailureAuthor(ArkqWork &work) {
    EventCling detach;
    detach.cleanup = 0;
    detach.target = ARKQ_HAND_ANCHOR;
    detach.character = {};
    detach.mode = 1;
    detach.delay = 0;
    EVT_AT_Cling(&detach);
    AT_SetLocalBaseChara(ARKQ_KNIFE, ARKQ_INVALID);
    EV_DisableCharacterDisplay(ARKQ_KNIFE);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_SK_AuthStart_Callback, 3, 0x0b, sizeof(StepAuthorStartWork), ARKQ_EVENT);
    StepAuthorStartWork *child = static_cast<StepAuthorStartWork *>(HLib::GetTaskParameter(task));
    child->task = task;
    child->parent = work.task;
    child->command = ARKQ_FAILURE_AUTHOR.name;
    child->callback = nullptr;
    child->phase = 1;
    child->callbackData = nullptr;
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(ARKQ_FAILURE_AUTHOR.name));
    HLib::SetupTimedTask(child->parent, 0);
    work.state = 9;
    snapAssignFrmCntFlags(1, 21);
    return 0;
}
std::int32_t FinishArkqResult(ArkqWork &work) {
    if (g_entryPoint <= 90U) {
        spmFadeOut();
        const std::int32_t delay = work.state == 8 ? 180 : 30;
        if (work.state == 8)
            AddInventoryItem(0x1789, 1, 1);
        if (EV_GetScreenFilmIsNotShowing())
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetCurrentTaskTo(ArkqCleanupCallback);
        return delay;
    }
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    work.commandPhase = 0;
    work.state = 2;
    return 0;
}
std::int32_t PlayArkqResult(ArkqWork &work) {
    EV_DisableCharacterDisplay(ARKQ_KNIFE_IMAGE);
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    QueueEventCallFunction(ArkqFreeGlintsCallback, nullptr, 20);
    const std::int32_t completed = static_cast<std::int32_t>(qteBattleUtil(BattleQteMode::GetCompletedCount));
    std::int32_t delay = 0;
    if (completed > 2)
        delay = ArkqAllDodges();
    else {
        if (completed > 0)
            delay = ArkqFirstDodge();
        if (completed > 1)
            delay = ArkqSecondDodge(delay);
        if (completed == 0)
            delay = ArkqFirstFailure(delay);
        else if (completed == 1)
            delay = ArkqSecondFailure(delay);
        else if (completed == 2)
            delay = ArkqThirdFailure(delay);
    }
    work.state = completed == 3 ? 6 : 7;
    EVT_AT_DispBlur(ARKQ_KNIFE, 1, 30, 1, delay, 0);
    EV_DisableCharacterDisplay(ARKQ_MONEY);
    SetHighModelXYZ(ctrlGetCharaByName(ARKQ_RYO), 1, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(ARKQ_RYO), 1, 1, 0);
    SetHighModelXYZ(ctrlGetCharaByName(ARKQ_REN), 1, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(ARKQ_REN), 1, 1, 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(ARKQ_RYO), 0, reinterpret_cast<std::byte *>(arkqOpenHandPose), 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(ARKQ_RYO), 1, reinterpret_cast<std::byte *>(arkqOpenHandPose), 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(ARKQ_REN), 0, reinterpret_cast<std::byte *>(arkqKnifeHandPose), 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(ARKQ_REN), 1, reinterpret_cast<std::byte *>(arkqOpenHandPose), 0);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(ARKQ_RYO), 1, 16, 1);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(ARKQ_REN), 1, 16, 1);
    for (shen_name character : ARKQ_SPECTATORS) {
        EV_ToggleCharacterFlag(character);
        EV_DisableCharacterDisplay(character);
    }
    for (shen_name character : ARKQ_SPECTATORS) {
        if (delay == -1) {
            EV_EnsureCharacterDraw(character);
            EV_EnableCharacterDisplay(character);
        } else {
            HLib::Task *task = HLib::EnqueueTaskWithParameter(ArkqCharacterPauseCallback, 6, 0x0b, sizeof(ArkqCharacterPauseWork), ARKQ_EVENT);
            RegisterEventTask(task);
            *static_cast<ArkqCharacterPauseWork *>(HLib::GetTaskParameter(task)) = {
                .cleanup = 1, .character = character, .paused = 0, .delay = static_cast<float>(delay + 1)};
        }
    }
    return delay;
}
} // namespace

void ArkqCharacterPauseCallback(void *data) {
    ArkqCharacterPauseWork &work = *static_cast<ArkqCharacterPauseWork *>(data);
    if (work.delay <= 0) {
        if (work.paused) {
            EV_ToggleCharacterFlag(work.character);
            EV_DisableCharacterDisplay(work.character);
        } else {
            EV_EnsureCharacterDraw(work.character);
            EV_EnableCharacterDisplay(work.character);
        }
        if (work.cleanup) {
            UnregisterEventTask(HLib::GetCurrentTask());
            HLib::DestroyCurrentTask();
        }
    } else
        work.delay -= HLib::GetScaledDeltaTick();
}
std::uint32_t CreateArkqKnifeGlint() {
    const Vector3 zero{};
    const std::uint32_t effect = static_cast<std::uint32_t>(CreateSmokeEffect(zero, zero, 100, .1f, shen_name{}, 0));
    SetSmokeEffectFadeFactor(effect, 255);
    SetSmokeEffectFadeFactor(effect, 112);
    SetSmokeEffectFalloff(effect, 255);
    SetParticleEffectSize(0, effect, .003f, .003f);
    SetParticleEffectColour(0, effect, 0, 255, 255, 255);
    SetParticleEffectBirthRate(0, effect, .050196077674627304f);
    SetParticleEffectInitialScale(0, effect, .08039215952157974f);
    SetParticleEffectSize(0, effect, .03921568766236305f, 10);
    SetParticleEffectVelocity(0, effect, zero, zero);
    return effect;
}
void ArkqSuspendGlintCallback(void *data) {
    const std::uint32_t effect = *static_cast<std::uint32_t *>(data);
    if (effect != 0xffffffffU)
        SuspendParticleEffect(0, effect);
}
void ArkqFreeGlintsCallback(void *) {
    for (std::uint32_t &effect : arkqGlobals.glints) {
        if (effect != 0xffffffffU) {
            FreeParticleEffect(0, effect);
            effect = 0xffffffffU;
        }
    }
}
void ArkqTurnKnifeCallback(void *) {
    EventSetRawAngle angle{.cleanup = 0, .character = ARKQ_KNIFE, .parent = ARKQ_KNIFE, .rotation = {.x = 58983, .y = 0, .z = 0}, .delay = 0};
    EVT_AT_SetAng_Callback(&angle);
    if (++arkqGlobals.knifeTurnFrame >= 5) {
        arkqGlobals.knifeTurnFrame = 0;
        HLib::DestroyCurrentTask();
    }
}
void ArkqKnifeLightCallback(void *) {
    Vector3 position;
    Vector3i rotation;
    const Vector3 volume{};
    Vector3 point{.x = 0, .y = 0, .z = 0};
    EV_GetCharaPosition(ARKQ_KNIFE, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(ARKQ_KNIFE, &rotation, EventTransformFlags::World);
    point.z = -.05f - static_cast<float>(arkqGlobals.knifeLightFrame) * .01f;
    ApplyScenarioTranslationAndRotation(&position, &rotation, &point);
    std::uint32_t *effect = &arkqGlobals.glints[arkqGlobals.knifeLightFrame / 3];
    SetParticleEffectPosition(0, *effect, &point, &volume);
    QueueEventCallFunction(ArkqSuspendGlintCallback, effect, 6);
    if (++arkqGlobals.knifeLightFrame >= 10) {
        arkqGlobals.knifeLightFrame = 0;
        HLib::DestroyCurrentTask();
    }
}
void ArkqIntroAuthorCallback(void *) {
    Vector3 position{.x = 0, .y = 0, .z = 0}, point{.x = 0, .y = 0, .z = -.07f};
    const Vector3 volume{};
    Vector3i rotation;
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    switch (frame) {
    case 1:
        arkqGlobals.authorStopDelay = -1;
        for (std::uint32_t &effect : arkqGlobals.glints) {
            if (effect != 0xffffffffU) {
                FreeParticleEffect(0, effect);
                effect = 0xffffffffU;
            }
            effect = CreateArkqKnifeGlint();
        }
        EV_EnableCharacterDisplay(ARKQ_MONEY);
        break;
    case 1320:
        AddThisToCashOrSegaCoins(-500, ECashOrCoins::Cash2);
        break;
    case 1498:
        qteBattleUtil(BattleQteMode::Start);
        EV_DisableCharacterDisplay(ARKQ_MONEY);
        break;
    case 1500:
        for (std::uint32_t effect : arkqGlobals.glints)
            ResumeParticleEffect(0, effect);
        break;
    case 1505:
        EV_EnableCharacterDisplay(ARKQ_KNIFE_IMAGE);
        EV_EnableCharacterDisplay(ARKQ_KNIFE);
        AssignImageModuleParameters(ctrlGetCharaByName(ARKQ_KNIFE_IMAGE), 1, 1, .5f, 0);
        break;
    default:
        break;
    }
    EV_GetCharaPosition(ARKQ_LIGHT_ANCHOR, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    SetLightRuntimePosition(39, position);
    if (static_cast<std::uint32_t>(frame) - 1501U <= 18U && (frame & 1) == 0) {
        EV_GetCharaPosition(ARKQ_HAND_ANCHOR, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        GetScenarioCharacterEulerAngles(ARKQ_HAND_ANCHOR, &rotation, EventTransformFlags::World);
        point.z = static_cast<float>(1520 - frame) * .006f - .18f;
        ApplyScenarioTranslationAndRotation(&position, &rotation, &point);
        std::uint32_t *effect = &arkqGlobals.glints[(1520 - frame) / 3];
        SetParticleEffectPosition(0, *effect, &point, &volume);
        QueueEventRepeatedFunction(ArkqSuspendGlintCallback, effect, 4, 1);
    }
    const std::int32_t result = static_cast<std::int32_t>(qteBattleUtil(BattleQteMode::GetResult));
    std::int32_t remaining;
    if (result)
        remaining = 15;
    else {
        remaining = arkqGlobals.authorStopDelay;
        if (remaining == -1)
            return;
    }
    arkqGlobals.authorStopDelay = remaining - 1;
    if (!remaining)
        EV_authSceneCtrl(AuthSceneCtrl::RefreshAndEnqueue);
}
void ArkqCleanupCallback(void *data) {
    ArkqWork &work = *static_cast<ArkqWork *>(data);
    qteBattleUtil(BattleQteMode::RetireAndFree);
    const std::int16_t state = work.state;
    AT_SetLocalBaseChara(ARKQ_KNIFE, ARKQ_INVALID);
    hndlSetFlags(ctrlGetCharaByName(ARKQ_RYO));
    EV_FreeAuthorPak(0, "0057");
    ET_AT_KillWork(work.characters);
    FinishLoading();
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    if (state == 8) {
        EV_stepSetFlags(0, 0x7b);
        EV_scenQueueLoad(g_actNumber, ARKQ_SUCCESS_MAP, 50);
    } else {
        DateTime time;
        GetCurrentDateTime(time);
        if (time.hour >= 1)
            ++time.day;
        time.hour = 1;
        time.minute = 0;
        SetCurrentDateTimeTo(time);
        EV_stepSetFlags(1, 0x7b);
        EV_scenQueueLoad(g_actNumber, ARKQ_FAILURE_MAP, 0);
    }
}
void ArkqMainCallback(void *data) {
    ArkqWork &work = *static_cast<ArkqWork *>(data);
    std::int32_t delay;
    switch (work.state) {
    case 0:
        work.characters = Queue_EVT_AT_ReadChr(work.task, ARKQ_CHARACTERS);
        work.state = 1;
        return;
    case 1:
        delay = SetupArkqResources(work);
        break;
    case 2:
        delay = BeginArkqIntro(work);
        break;
    case 3:
        delay = ArkqBeginKnifeDisplay(work);
        break;
    case 4:
        if (work.commandPhase) {
            if (qteBattleUtil(BattleQteMode::GetResult)) {
                work.commandPhase = 0;
                work.state = 5;
            }
        } else {
            qteBattleUtil(BattleQteMode::Start);
            ++work.commandPhase;
        }
        return;
    case 5:
        delay = PlayArkqResult(work);
        break;
    case 6:
        delay = BeginArkqSuccessAuthor(work);
        break;
    case 7:
        delay = BeginArkqFailureAuthor(work);
        break;
    case 8:
    case 9:
        delay = FinishArkqResult(work);
        break;
    default:
        return;
    }
    if (delay > 0)
        HLib::SetupTaskDelay(delay);
}
void InitializeArkqEvent() {
    arkqGlobals.authorStopDelay = -1;
    for (std::uint32_t &effect : arkqGlobals.glints)
        effect = 0;
    for (std::uint32_t &reserved : arkqGlobals.reserved2c)
        reserved = 0;
    arkqGlobals.knifeTurnFrame = 0;
    arkqGlobals.knifeLightFrame = 0;
}
bool InitializeArkqWork(ArkqWork &work, HLib::Task *task) {
    SetPlayerControlEnabled(0);
    work.task = task;
    work.scratch[0] = 0;
    work.scratch[1] = 0;
    work.commandPhase = 0;
    work.inputMasks[2] = 0x200;
    work.state = 0;
    work.inputMasks[0] = 0x100;
    work.inputMasks[1] = 0x10;
    const bool result = CheckIsArgEquZero(1);
    for (std::uint32_t &effect : arkqGlobals.glints)
        effect = 0xffffffffU;
    return result;
}
void RunArkqEvent(std::uintptr_t) {
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(ArkqMainCallback, 5, 0x0d, sizeof(ArkqWork), ARKQ_TASK);
    ArkqWork *work = static_cast<ArkqWork *>(HLib::GetTaskParameter(task));
    HLib::SetupTimedTask(task, 5);
    ArkqSetupNullsub();
    InitializeArkqWork(*work, task);
}
const ScriptAreaDefinition arkqScriptDefinition{.areaId = ARKQ_AREA, .field04 = 0, .init = InitializeArkqEvent, .main = RunArkqEvent};
} // namespace shenmue::s2::areas
