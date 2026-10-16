#include "s2/areas/area_hasi.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/flag.hpp"
#include "s2/inventory.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/navi.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/world_render.hpp"
#include <bit>
#include <cstring>

namespace shenmue::s2 {
namespace {
Matrix4x4 hasiPlacement;
std::int32_t hasiVoiceFade;
HLib::Task *hasiBridgeTask;
BridgeBendingDefinition hasiBridgeDefinition{.sectionWidth = BRIDGE_SECTION_WIDTH,
    .character = BRIDGE_DUMMY,
    .model = BRIDGE_MODEL,
    .attachedCharacter = {},
    .partCount = 20,
    .reserved14 = 0,
    .parts = BRIDGE_SECTION_PARTS};
void HasiAdmissionNullsub() {
}

void HasiBridgeCallback(void *data) {
    BridgeBendingWork *const work = static_cast<BridgeBendingWork *>(data);
    if (!work) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (work->phaseStep)
        work->phase = static_cast<std::int32_t>(static_cast<float>(work->phaseStep) * work->tickFactor + static_cast<float>(work->phase));
    BendHasiBridge(work);
    UpdateBridgeAttachedCharacter(work);
}
void QueueHasiBridge() {
    if (hasiBridgeTask)
        return;
    hasiBridgeTask = HLib::EnqueueTaskWithParameter(HasiBridgeCallback, 6, 0x0b, sizeof(BridgeBendingWork), BRIDGE_BENDING_TASK);
    if (!hasiBridgeTask)
        return;
    BridgeBendingWork *const work = static_cast<BridgeBendingWork *>(HLib::GetTaskParameter(hasiBridgeTask));
    if (!work) {
        HLib::DestroyTaskInternal(hasiBridgeTask);
        hasiBridgeTask = nullptr;
        return;
    }
    std::memset(work, 0, sizeof(*work));
    work->phaseStep = 1820;
    work->definition = &hasiBridgeDefinition;
    work->amplitude = 910;
    work->scale = .5f;
    work->pairCount = hasiBridgeDefinition.partCount / 2;
    work->range = static_cast<float>(hasiBridgeDefinition.partCount + 1) * hasiBridgeDefinition.sectionWidth;
    work->tickFactor = 1;
    EV_GetCharaPosition(hasiBridgeDefinition.character, CHARACTER_TRANSLATION_ROOT, &work->position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(work->definition->character, &work->rotation, EventTransformFlags::World);
    if (hasiBridgeTask) {
        BridgeBendingWork *const attached = static_cast<BridgeBendingWork *>(HLib::GetTaskParameter(hasiBridgeTask));
        if (attached)
            attached->attachedCharacter = hasiBridgeDefinition.attachedCharacter;
    }
    ResetBridgeSections(work);
    HLib::SetupTimedTask(hasiBridgeTask, 1);
}
void HasiAttachBridgeCallback(void *data) {
    HasiBridgeAttachment *const request = static_cast<HasiBridgeAttachment *>(data);
    if (!request) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (hasiBridgeTask) {
        BridgeBendingWork *const work = static_cast<BridgeBendingWork *>(HLib::GetTaskParameter(hasiBridgeTask));
        if (work)
            work->attachedCharacter = request->character;
    }
    if (request->delay)
        HLib::DestroyCurrentTask();
}
void QueueHasiBridgeAttachment(shen_name character, std::int32_t delay) {
    if (delay) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(HasiAttachBridgeCallback, 6, 0x0b, sizeof(HasiBridgeAttachment), BRIDGE_BENDING_TASK);
        if (task) {
            HasiBridgeAttachment *const request = static_cast<HasiBridgeAttachment *>(HLib::GetTaskParameter(task));
            if (request) {
                request->character = character;
                request->delay = delay;
                HLib::SetupTimedTask(task, delay);
            }
        }
    } else if (hasiBridgeTask) {
        BridgeBendingWork *const work = static_cast<BridgeBendingWork *>(HLib::GetTaskParameter(hasiBridgeTask));
        if (work)
            work->attachedCharacter = character;
    }
}
void HasiEndBridgeCallback(void *data) {
    const std::int32_t *const cleanup = static_cast<const std::int32_t *>(data);
    if (!cleanup) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (hasiBridgeTask) {
        BridgeBendingWork *const work = static_cast<BridgeBendingWork *>(HLib::GetTaskParameter(hasiBridgeTask));
        if (work) {
            ResetBridgeSections(work);
            EV_AT_SetPos(work->definition->character, EventTransformFlags::World | EventTransformFlags::AllAxes, work->position);
            EV_AT_SetAngY(work->definition->character, EventTransformFlags::World | EventTransformFlags::AllAxes, work->rotation, nullptr);
            HLib::DestroyTaskInternal(hasiBridgeTask);
            hasiBridgeTask = nullptr;
        }
    }
    if (*cleanup)
        HLib::DestroyCurrentTask();
}
void QueueHasiBridgeEnd(std::int32_t delay) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(HasiEndBridgeCallback, 6, 0x0b, sizeof(std::int32_t), BRIDGE_BENDING_TASK);
    if (task) {
        std::int32_t *const cleanup = static_cast<std::int32_t *>(HLib::GetTaskParameter(task));
        if (cleanup) {
            *cleanup = 1;
            HLib::SetupTimedTask(task, delay);
        }
    }
}
void HasiVoiceFadeCallback(void *) {
    hsFadeVoice(nullptr, hasiVoiceFade);
    HLib::DestroyCurrentTask();
}
std::int32_t HasiFailureLevel() {
    std::int32_t level = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1286) != 0;
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1287))
        level = 2;
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1288))
        level = 3;
    return level;
}
std::int32_t HasiAuthorIndex(shen_name author) {
    for (std::int32_t index = 0; index < 12; ++index)
        if (author.name == HASI_AUTHORS[index].name)
            return index + 17;
    return 0;
}
void StartHasiAuthor(HasiEventWork *work) {
    if (!EV_GetShenmueFlag(0x1a8) &&
        (work->author.name == HASI_AUTHOR_2.name || work->author.name == HASI_AUTHOR_3.name || work->author.name == HASI_AUTHOR_4.name ||
            work->author.name == HASI_AUTHOR_5.name || work->author.name == HASI_AUTHOR_B.name))
        work->author = HASI_AUTHOR_1;
    mtwkSetExtraFlag(scenFindMotionwork(HASI_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(HASI_RYO, false);
    Matrix4x4_Push(&transformMatrix_prePass);
    Matrix4x4::preTranslate(work->translation.x, work->translation.y, work->translation.z);
    const std::uint16_t angle = static_cast<std::uint16_t>(work->rotation.y);
    const std::uint16_t folded = angle <= 0x8000 ? angle : static_cast<std::uint16_t>(-angle);
    const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000 - folded));
    const float sine = sin_fixed16(static_cast<std::int16_t>(angle));
    ApplyShearToPrePassMatrix(sine, cosine);
    hasiPlacement = workTransformMatrix;
    Matrix4x4::Pop(1);
    if (work->author.name != HASI_AUTHOR_8.name) {
        if (work->author.name != HASI_AUTHOR_B.name)
            Author_SetPlacementMatrix(&hasiPlacement);
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    }
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    if (IsAuthorStateFlagSet(HasiAuthorIndex(work->author)))
        EV_SetAuthorActiveTo(1);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(work->author.name));
}
bool WaitHasiAuthor(HasiEventWork *work) {
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return false;
    if (work->author.name == HASI_AUTHOR_8.name)
        spmFadeOut();
    SetAuthorStateFlag(HasiAuthorIndex(work->author), 1);
    mtwkSetExtraFlag(scenFindMotionwork(HASI_RYO), 6, 0);
    return true;
}
void RestoreHasiStages() {
    Vector3 scale{1, 1, 1};
    for (const HasiStageRestoreDefinition &definition : HASI_RESTORE_STAGES) {
        if (!DoesScenarioCharacterExist(definition.stage))
            continue;
        if (!DoesScenarioCharacterExist(definition.first)) {
            CreateEventCharacter(CharacterCreationHighModel, nullptr, definition.first);
            EVT_AT_SetPos(definition.first, definition.stage, {3.8f, 0, 0}, 0);
            EVT_AT_SetAngY(definition.first, definition.stage, 0, 0);
        }
        if (!DoesScenarioCharacterExist(definition.second)) {
            CreateEventCharacter(CharacterCreationHighModel, nullptr, definition.second);
            EVT_AT_SetPos(definition.second, definition.stage, {-3.8f, 0, 0}, 0);
            EVT_AT_SetAngY(definition.second, definition.stage, 180, 0);
        }
        scale.z = definition.scale;
        EV_SetCharacterScale(definition.stage, scale);
        SetScenarioCharacterRadius(definition.stage, 20);
    }
}
void HasiAuthCallback(void *data) {
    HasiEventWork *const work = static_cast<HasiEventWork *>(data);
    switch (work->state) {
    case 0:
        work->state = 1;
        HLib::SetupTaskDelay(10);
        break;
    case 1:
        StartHasiAuthor(work);
        work->state = 2;
        break;
    case 2:
        if (WaitHasiAuthor(work)) {
            work->state = 3;
            HLib::SetupTaskDelay(30);
        }
        break;
    case 3:
        camCtrlSetState(CameraState::Init);
        LoadCutsceneAndSoundProgramForArea();
        EV_spmBeginSampleExecute();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_SetCharaControllerFlags(HASI_RYO, ApplyGravity, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(HASI_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
        SetPlayerGameFlags(PlayerGameFlagMode::Bit2, reinterpret_cast<void *>(1));
        FinishLoading();
        SetPlayerControlEnabled(1);
        EV_NPC_NewLoadSw(1);
        SetNaviDisplayOverride(false);
        EV_FreeAuthorPak(0, "0289");
        EV_FreeAuthorPak(0, "0332");
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("HASI"), 0);
        scenFreeEvent("HASI.EV3");
        EV_CleanupMapControlTask();
        HLib::MarkCurrentTaskForCleanup();
        break;
    default:
        break;
    }
}
void InitializeHasiAuth(HLib::Task *task, std::int32_t entry) {
    HasiEventWork *const work = static_cast<HasiEventWork *>(HLib::GetTaskParameter(task));
    std::memset(work, 0, sizeof(*work));
    work->task = task;
    work->entry = entry;
    work->author = HASI_AUTHOR_8;
    EV_SetCharaControllerFlags(HASI_RYO, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(HASI_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    SetPlayerGameFlags(PlayerGameFlagMode::Bit2, nullptr);
    EV_SetRefBoardParamsFor(HASI_RYO, 1);
    EventSetMotion motion{.cleanup = 0, .character = HASI_RYO, .animation = HASI_MOTION_IDLE, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(HASI_MOTION_IDLE);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0332E000.smp");
    EV_LoadAudio("0332E000", 0);
    EV_spmBeginSampleExecute();
    EV_FreeAuthorPak(1, "0289");
    EV_FreeAuthorPak(1, "0332");
}
void InitializeHasiEvent(HLib::Task *task, std::int32_t entry) {
    HasiEventWork *const work = static_cast<HasiEventWork *>(HLib::GetTaskParameter(task));
    std::memset(work, 0, sizeof(*work));
    work->task = task;
    work->entry = entry;
    work->actions = HASI_ACTIONS;
    AT_SetLocalBaseChara(HASI_RYO, HASI_STAGES[entry][0].character);
    Vector3 position;
    EV_GetCharaPosition(HASI_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    AT_SetLocalBaseChara(HASI_RYO, EVENT_CHARACTER_INVALID);
    work->definition = &HASI_STAGES[entry][position.x <= 0 ? 1 : 0];
    const std::uint32_t time = static_cast<std::uint32_t>(work->definition->qteTime + 4 * HasiFailureLevel());
    work->qteTime = static_cast<std::uint16_t>(time);
    work->reserved1a = static_cast<std::uint16_t>(time >> 16);
    work->author = work->definition->author;
    work->translation = {0, 0, 4.6f};
    EV_GetCharaPosition(work->definition->character, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(work->definition->character, &work->rotation, EventTransformFlags::World);
    work->rotation.y += work->definition->reversed ? -0x4000 : 0x4000;
    ApplyScenarioTranslationAndRotation(&position, &work->rotation, &work->translation);
    EVT_AT_SetPos(BRIDGE_MODEL, work->definition->character, {}, 0);
    EVT_AT_SetAngY(BRIDGE_MODEL, work->definition->character, 0, 0);
    EVT_AT_SetPos(BRIDGE_DUMMY, work->definition->character, {}, 0);
    EVT_AT_SetAngY(BRIDGE_DUMMY, work->definition->character, 0, 0);
    EventSetRawAngle rotation{.cleanup = 0, .character = BRIDGE_DUMMY, .parent = work->definition->character, .rotation = {}, .delay = 0};
    EVT_AT_SetAng_Callback(&rotation);
    if (work->definition->reversed)
        EVT_AT_SetAngY(BRIDGE_DUMMY, BRIDGE_DUMMY, 180, 0);
    AT_SetLocalBaseChara(HASI_CAMERA, HASI_RYO);
    EVT_AT_SetPos(HASI_CAMERA, HASI_RYO, {}, 0);
    EVT_AT_SetAngY(HASI_CAMERA, HASI_RYO, 0, 0);
    if (GetItemState(6004))
        SetNaviDisplayOverride(true);
    hasiBridgeDefinition.character = work->definition->character;
    EV_SetCharaControllerFlags(HASI_RYO, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(HASI_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    SetPlayerGameFlags(PlayerGameFlagMode::Bit2, nullptr);
    EV_SetRefBoardParamsFor(HASI_RYO, 1);
    EventSetMotion motion{.cleanup = 0, .character = HASI_RYO, .animation = HASI_MOTION_IDLE, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(HASI_MOTION_IDLE);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0332E000.smp");
    EV_LoadAudio("0332E000", 0);
    EV_spmBeginSampleExecute();
    EV_FreeAuthorPak(1, "0289");
    EV_FreeAuthorPak(1, "0332");
    StartHasiAuthor(work);
}
void HasiEventCallback(void *data) {
    HasiEventWork *const work = static_cast<HasiEventWork *>(data);
    switch (work->state) {
    case 0:
        if (WaitHasiAuthor(work))
            work->state = 1;
        break;
    case 1:
        SetGlobalShadowLightingEnabled(false);
        HasiAdmissionNullsub();
        AT_SetLocalBaseChara(HASI_RYO, BRIDGE_DUMMY);
        EV_AT_SetPos(HASI_RYO, EventTransformFlags::AllAxes, {4.4f, 0, 0});
        EV_AT_SetAngY(HASI_RYO, EventTransformFlags::AllAxes, {0, 0x4000, 0}, nullptr);
        AT_SetLocalBaseChara(HASI_RYO, EVENT_CHARACTER_INVALID);
        EVT_AT_SetMot(HASI_RYO, HASI_MOTION_START, 0, 1, 0x40000000U, 0);
        mtwkSetRequestedRootMode(scenFindMotionwork(HASI_RYO), 3);
        AT_SetLocalBaseChara(HASI_CAMERA, HASI_RYO);
        EVT_AT_SetPos(HASI_CAMERA, HASI_RYO, {}, 0);
        EVT_AT_SetAng_Enqueue(HASI_CAMERA, HASI_RYO, 0, 0, 0, 0);
        EVT_AT_SetCam(work->definition->camera, HASI_CAMERA, HASI_CAMERA, 0, 0, 1);
        EVT_AT_MenCtrl({.cleanup = 1, .frames = 30, .startAlpha = 255, .targetAlpha = 0, .delay = 1});
        QueueHasiBridge();
        QueueHasiBridgeAttachment(HASI_RYO, 1);
        work->action = static_cast<std::uint32_t>(work->definition->steps[work->step + 2 * work->sequence]);
        if (work->action > 3) {
            work->state = 4;
            HLib::SetupTaskDelay(110);
        } else {
            EVT_AT_QTE(static_cast<std::uint16_t>(work->actions[work->action].qte), static_cast<std::int16_t>(work->qteTime), 0, 40);
            work->state = 2;
            HLib::SetupTaskDelay(40);
        }
        break;
    case 2:
        if (!GetShouldRenderIcons(RenderIconSlot::Qte))
            work->state = work->actions[work->action].qte == static_cast<std::uint32_t>(EV_GetCurrentQteStatus() & 0xfff) ? 3 : 8;
        break;
    case 3:
        if (GetMotionFrame(scenFindMotionwork(HASI_RYO), MotionRegion::LowerBody) >= 110)
            work->state = 4;
        break;
    case 4:
        if (++work->step >= 2) {
            ++work->sequence;
            work->step = 0;
        }
        work->action = static_cast<std::uint32_t>(work->definition->steps[work->step + 2 * work->sequence]);
        if (work->action == 5)
            work->state = 9;
        else {
            std::int32_t delay = 0;
            if (!work->step) {
                EVT_AT_SetMot(HASI_RYO, HASI_MOTION_WALK, 0, 1, 0, 0);
                delay = 44;
            }
            work->state = 5;
            if (delay)
                HLib::SetupTaskDelay(delay);
        }
        break;
    case 5:
        if (work->action > 3)
            work->state = 4;
        else {
            AT_SetLocalBaseChara(HASI_CAMERA, EVENT_CHARACTER_INVALID);
            EVT_AT_SetCam(work->actions[work->action].camera + shenGetRandomInt32(2), HASI_CAMERA, HASI_CAMERA, 0, 0, 0);
            EVT_AT_QTE(static_cast<std::uint16_t>(work->actions[work->action].qte), static_cast<std::int16_t>(work->qteTime), 0, 0);
            EV_GetCharaPosition(HASI_RYO, CHARACTER_TRANSLATION_ROOT, &work->previousPosition, EventTransformFlags::World);
            EVT_AT_SetMot(HASI_RYO, work->actions[work->action].motion, 0, 1, 0, 0);
            EVT_AT_SetVoice(HASI_LEAN_VOICES[shenGetRandomInt32(4)], 0);
            work->state = 6;
        }
        break;
    case 6:
        if (!GetShouldRenderIcons(RenderIconSlot::Qte))
            work->state = work->actions[work->action].qte == static_cast<std::uint32_t>(EV_GetCurrentQteStatus() & 0xfff) ? 7 : 8;
        break;
    case 7: {
        AT_SetLocalBaseChara(HASI_CAMERA, HASI_RYO);
        EVT_AT_SetPos(HASI_CAMERA, HASI_RYO, {}, 0);
        EVT_AT_SetAngY(HASI_CAMERA, HASI_RYO, 0, 0);
        EVT_AT_SetCam(work->definition->camera, HASI_CAMERA, HASI_CAMERA, 0, 0, 0);
        EVT_AT_SetVoice(HASI_RECOVER_VOICES[shenGetRandomInt32(4)], 0);
        EV_AT_SetPos(HASI_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, work->previousPosition);
        EVT_AT_SetMot(HASI_RYO, work->actions[work->action].recoverMotion, 0, 1, 0x40000000U, 0);
        work->state = 4;
        HLib::SetupTaskDelay(70);
        break;
    }
    case 8: {
        if (!work->sequence && !work->step)
            work->action = 1;
        SetScenarioCharacterDrawControl(HASI_RYO, 0, 0);
        BridgeBendingWork *const bridge = static_cast<BridgeBendingWork *>(HLib::GetTaskParameter(hasiBridgeTask));
        if (bridge)
            bridge->attachedHeight = .1f;
        EVT_AT_SetCam(work->actions[work->action].fallCamera, HASI_CAMERA, HASI_CAMERA, 0, 0, 0);
        EVT_AT_SetMot(HASI_RYO, work->actions[work->action].fallMotion, 0, 1, 0x40000000U, 0);
        EVT_AT_SetVoice("0289A016", 0);
        EVT_AT_MenCtrl({.cleanup = 1, .frames = 30, .startAlpha = 0, .targetAlpha = 255, .delay = 40});
        EVT_AT_FadeBGM(0, 1, 70);
        HLib::Task *const fade = HLib::EnqueueTaskWithoutParameter(HasiVoiceFadeCallback, 4, 0x0b, BRIDGE_BENDING_TASK.name);
        hasiVoiceFade = 60;
        HLib::SetupTimedTask(fade, 70);
        QueueHasiBridgeEnd(130);
        work->state = 11;
        HLib::SetupTaskDelay(130);
        break;
    }
    case 9:
        EVT_AT_SetMot(HASI_RYO, HASI_MOTION_EXIT, 0, 1, 0x20000000U, 0);
        EVT_AT_MenCtrl({.cleanup = 1, .frames = 30, .startAlpha = 0, .targetAlpha = 255, .delay = 40});
        EVT_AT_FadeBGM(0, 1, 40);
        QueueHasiBridgeEnd(70);
        EVT_AT_SetMot(HASI_RYO, HASI_MOTION_IDLE, 0, 1, 0, 70);
        work->state = 10;
        HLib::SetupTaskDelay(70);
        break;
    case 10:
        SetGlobalShadowLightingEnabled(true);
        if (EV_GetShenmueFlag(0x1a8) && work->definition->character.name == HASI_STAGE_6.name)
            EV_stepSetFlags(0, 0x1a8);
        camCtrlSetState(CameraState::Init);
        LoadCutsceneAndSoundProgramForArea();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_spmBeginSampleExecute();
        EV_SetCharaControllerFlags(HASI_RYO, ApplyGravity, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(HASI_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
        SetPlayerGameFlags(PlayerGameFlagMode::Bit2, reinterpret_cast<void *>(1));
        FinishLoading();
        SetPlayerControlEnabled(1);
        SetNaviDisplayOverride(false);
        EV_NPC_NewLoadSw(1);
        EV_FreeAuthorPak(0, "0289");
        EV_FreeAuthorPak(0, "0332");
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("HASI"), 0);
        scenFreeEvent("HASI.EV3");
        EV_CleanupMapControlTask();
        HLib::MarkCurrentTaskForCleanup();
        break;
    case 11: {
        SetScenarioCharacterDrawControl(HASI_RYO, 1, 0);
        const std::int32_t level = HasiFailureLevel();
        if (level < 3)
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(1286 + level), 1);
        if (work->definition->character.name == HASI_STAGE_9.name)
            EV_scenQueueLoad(g_actNumber, HASI_RETURN_AREA, 5);
        else {
            EV_scenQueueLoad(g_actNumber, HASI_RETURN_AREA, 4);
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 2071, 1);
        }
        break;
    }
    default:
        break;
    }
}
} // namespace
void HasiInit() {
    std::memset(&hasiPlacement, 0, sizeof(hasiPlacement));
    hasiVoiceFade = 0;
    hasiBridgeTask = nullptr;
}
void HasiMain(std::uintptr_t argument) {
    const std::int32_t entry = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(argument));
    EV_SetNewSceneIndex();
    if (entry >= 0) {
        SetPlayerControlEnabled(0);
        HasiAdmissionNullsub();
        EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("HASI"), 0);
        EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("HASI"), 0);
        if (entry == 10) {
            HLib::Task *const task = HLib::EnqueueTaskWithParameter(HasiAuthCallback, 4, 0x0b, sizeof(HasiEventWork), BRIDGE_BENDING_TASK);
            InitializeHasiAuth(task, 10);
        } else {
            HLib::Task *const task = HLib::EnqueueTaskWithParameter(HasiEventCallback, 4, 0x0b, sizeof(HasiEventWork), BRIDGE_BENDING_TASK);
            InitializeHasiEvent(task, entry);
            if (EV_GetShenmueFlag(0x141))
                EV_stepSetFlags(0, 0x141);
        }
    } else {
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("HASI"), 0);
        RestoreHasiStages();
        scenFreeEvent("HASI.EV3");
        SetPlayerControlEnabled(1);
    }
}
const ScriptAreaDefinition hasiScriptDefinition{.areaId = AREA_HASI, .field04 = 0, .init = HasiInit, .main = HasiMain};
} // namespace shenmue::s2
