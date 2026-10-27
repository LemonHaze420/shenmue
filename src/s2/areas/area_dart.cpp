#include "s2/areas/area_dart.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/darts.hpp"
#include "s2/darts_conversation.hpp"
#include "s2/debug.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/model_component.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"
#include "d3t/dx11_renderer.hpp"
#include <cmath>
#include <cstring>

namespace shenmue::s2::areas {
namespace {
inline constexpr shen_name DART_COIN{SHEN_NAME(Coin)};
inline constexpr shen_name DART_FIRST_ARROW{SHEN_NAME(YA00)};
inline constexpr shen_name DART_ADMISSION_ARROW{SHEN_NAME(Ya14)};
inline constexpr shen_name DART_THROW_BUTTON{SHEN_NAME(BTBA)};
inline constexpr shen_name DART_AKS0{SHEN_NAME(AKS0)};
inline constexpr shen_name DART_REWARD_AREA{SHEN_NAME(OMAK)};
inline constexpr std::int32_t DART_SINGLE_FLAG = 0x3e0;
inline constexpr Vector3i DART_HAND_ROTATION{.x = 64863, .y = 3457, .z = 64360};
inline constexpr Vector3 DART_AIM_ARROW_POSITION{.x = -0.09030000120401382f, .y = 1.625100016593933f, .z = 1.313599944114685f};
inline constexpr std::array<Vector3, 5> DART_READY_ARROW_OFFSETS{{{.x = 0.15800000727176666f, .y = 0.008999999612569809f, .z = 0.004000000189989805f},
    {.x = 0.14499999582767487f, .y = 0.009999999776482582f, .z = -0.0020000000949949026f},
    {.x = 0.12800000607967377f, .y = 0.017000000923871994f, .z = -0.007000000216066837f}, {.x = 0.13500000536441803f, .y = 0.00800000037997961f, .z = 0},
    {.x = 0.15000000596046448f, .y = 0.008999999612569809f, .z = 0.004000000189989805f}}};
struct DartsVoiceWaitWork {
    std::int32_t frames, reserved04;
    ConversationBuffer *conversation;
    HLib::Task *parent;
};
struct DartsCameraWaitWork {
    DartsEventWork *root;
    std::int32_t camera;
    shen_name eye, target;
};
struct DartsProjectileWork {
    Vector3 step, target;
    std::int32_t arcStep;
    std::uint32_t arcPhase;
    Vector3i targetRotation;
    std::int32_t maximumFrames;
    shen_name board, arrow;
    Vector3 start;
    std::int32_t frame;
};
struct DartsAdmissionScoreWork {
    std::int32_t soundStarted;
    shen_name board;
    std::array<std::int32_t, 4> scores;
    std::int32_t remaining, frames;
};
static_assert(sizeof(DartsVoiceWaitWork) == 0x18 && sizeof(DartsCameraWaitWork) == 0x18);
static_assert(sizeof(DartsProjectileWork) == 0x48 && offsetof(DartsProjectileWork, frame) == 0x44);
static_assert(sizeof(DartsAdmissionScoreWork) == 0x20);

void DartsInitStart(void *);
void DartsInitStart2(void *);
void DartsPositionOpponentHands(void *);
void DartsOpponentAdmission(void *);
void DartsMain(void *);
void DartsMainOk(void *);
void DartsAskPlay(void *);
void DartsInsertCoin(void *);
void DartsEnemyShot(void *);
void DartsRyoShot(void *);
void DartsPlaying(void *);
void DartsPlayingLoop(void *);
void DartsEnemyPlaying(void *);
void DartsGameMain(void *);
void DartsGameScore(void *);
void DartsShotEnd(void *);
void DartsShotEndBlink(void *);
void DartsShotEndRecord(void *);
void DartsScoreAdd(void *);
void DartsNextGame(void *);
void DartsPlayEnd(void *);
void DartsExitBattle(void *);
void DartsContinuePrompt(void *);
void DartsContinueOk(void *);
void DartsDoubleUp(void *);
void DartsExit(void *);


std::size_t DartsOpponentIndex(shen_name opponent) {
    std::size_t index = 0;
    while (index < 5 && DART_OPPONENT_WORDS[index][0] != opponent.name)
        ++index;
    return index;
}
shen_name DartsArrowName(std::int32_t index, bool enemy) {
    return shen_name{make_shen_name('Y', enemy ? 'a' : 'A', static_cast<char>('0' + index / 10), static_cast<char>('0' + index % 10))};
}
ConversationBuffer *DartsResponse(DartsEventWork *work, std::int32_t index) {
    const std::int32_t playerWon = work->playerWon;
    scenCvsCtrlFree(0);
    dartsPlayerWon = playerWon;
    return work->entry->opponentMode == 2 ? ExecuteDartsResponse(index, 1, work->enemyTurn == 0, work->entry->enemy->name) :
                                            ExecuteDartsResponse(index, 0, true, EVENT_CHARACTER_INVALID);
}
void DartsWaitVoice(void *data) {
    DartsVoiceWaitWork *const wait = static_cast<DartsVoiceWaitWork *>(data);
    if (!wait) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (!cvsHasValidConverseData(wait->conversation) || --wait->frames <= 0) {
        HLib::ResolveDeferredCallback(wait->parent);
        HLib::DestroyCurrentTask();
    }
}
HLib::Task *DartsQueueVoiceWait(HLib::Task *parent, ConversationBuffer *conversation) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(DartsWaitVoice, 4, 11, sizeof(DartsVoiceWaitWork), DART_TASK);
    DartsVoiceWaitWork *const wait = static_cast<DartsVoiceWaitWork *>(HLib::GetTaskParameter(task));
    if (wait) {
        wait->frames = 1000;
        wait->conversation = conversation;
        wait->parent = parent;
        HLib::SetupTimedTask(parent, 0);
    }
    return task;
}
void DartsCamera(DartsEventWork *work, std::int32_t camera, shen_name eye, shen_name target, std::int32_t delay) {
    if (camera == 7201 && !work->standardBoard)
        camera = 7218;
    else if (camera == 7207)
        camera = 7221;
    EVT_AT_SetCam(camera, eye, target, 0, 0, delay);
}
void DartsCameraWait(void *data) {
    DartsCameraWaitWork *const wait = static_cast<DartsCameraWaitWork *>(data);
    if (!camCtrlCheckIsEventCam()) {
        DartsCamera(wait->root, wait->camera, wait->eye, wait->target, 0);
        HLib::DestroyCurrentTask();
    }
}
HLib::Task *DartsQueueCameraWait(DartsEventWork *root, std::int32_t camera, shen_name eye, shen_name target) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(DartsCameraWait, 4, 11, sizeof(DartsCameraWaitWork), DART_TASK);
    DartsCameraWaitWork *const wait = static_cast<DartsCameraWaitWork *>(HLib::GetTaskParameter(task));
    if (wait) {
        wait->root = root;
        wait->camera = camera;
        wait->eye = eye;
        wait->target = target;
        HLib::SetupTimedTask(task, 2);
    } else
        EV_debug_log("DART\\eDART_et_darts.c", 3618, "CamctrlDartsWait Error\n");
    return task;
}
void DartsResetRound(DartsEventWork *work) {
    work->state = 5;
    if (work->standardBoard && (work->entry->opponentMode != 2 || work->entry->enemy->name.name != DART_XHO.name)) {
        work->throws = 0;
        work->fieldF4 = 0;
        work->field118 = 0;
    } else {
        if (work->standardBoard) {
            work->fieldF4 = 3;
            work->field118 = 3;
        }
        work->throws = 3;
    }
    work->opponentScore.impactIterations = 0;
    work->fieldF8 = 0;
    work->playerScore.impactIterations = 0;
    work->usedBands = {};
}
void DartsInitializeGame(DartsEventWork *work) {
    EV_mtwkSetEnabled(scenFindMotionwork(DART_PLAYER), 0);
    EV_mtwkSetEnabled(scenFindMotionwork(work->entry->enemy->name), 0);
    EV_EnsureCharacterDraw(DartsSelectBoard(work, {}));
    EV_EnableCharacterDisplay(DartsSelectBoard(work, {}));
    for (std::int32_t index = 0; index < 5; ++index) {
        Vector3 hidden{.x = 0, .y = 0, .z = -10.05f};
        Vector3i rotation{};
        EV_scenApplyPosOrRotToPartOf(
            DartsSelectBoard(work, DART_PLAYER), static_cast<std::uint16_t>(index + 153), &hidden, &rotation, EventCharacterPartTransformMode::Absolute);
        const shen_name playerArrow = DartsArrowName(index, false);
        EV_EnsureCharacterDraw(playerArrow);
        EV_EnableCharacterDisplay(playerArrow);
        EV_DisableCharacterDisplay(playerArrow);
        AT_SetLocalBaseChara(playerArrow, DartsSelectBoard(work, DART_PLAYER));
        EV_scenApplyPosOrRotToPartOf(DartsSelectBoard(work, work->entry->enemy->name), static_cast<std::uint16_t>(index + 153), &hidden, &rotation,
            EventCharacterPartTransformMode::Absolute);
        const shen_name enemyArrow = DartsArrowName(index, true);
        EV_EnsureCharacterDraw(enemyArrow);
        EV_EnableCharacterDisplay(enemyArrow);
        EV_DisableCharacterDisplay(enemyArrow);
        AT_SetLocalBaseChara(enemyArrow, DartsSelectBoard(work, work->entry->enemy->name));
    }
    DartsResetScoring(work);
    DartsResetRound(work);
    DartsStartChildTask(work, DartsChildTask::Frames);
    DartsStartChildTask(work, DartsChildTask::Display);
    DartsStartChildTask(work, DartsChildTask::VoiceMonitor);
}
void DartsInitStart(void *data) {
    DartsEventWork *const work = static_cast<DartsEventWork *>(data);
    if (!work) {
        HLib::DestroyCurrentTask();
        return;
    }
    GetCurrentDateTime(work->previousTime);
    shenSetTimeSimulationTo(0);
    shenSetClockAndCompassRenderTo(1);
    SetPlayerControlEnabled(0);
    mtwkSetRequestedRootMode(scenFindMotionwork(DART_PLAYER), 0);
    EventSetMotion motion{.cleanup = 0, .character = DART_PLAYER, .animation = 61442, .frame = -1, .speed = 1, .flags = 0x80000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(61442);
    if (work->entry->opponentMode == 2) {
        const shen_name opponent = work->entry->enemy->name;
        std::uint8_t flag = 11;
        if (opponent.name == DART_FCJ.name)
            flag = 9;
        else if (opponent.name == DART_HLN.name)
            flag = 10;
        else if (opponent.name == DART_JON.name)
            flag = 13;
        else if (opponent.name == DART_HCY.name)
            flag = 12;
        SetCurrentGameFlag(flag, 1);
        for (std::int32_t index = 0; index < 15; ++index)
            EV_DisableCharacterDisplay(DartsArrowName(index % 15, true));
        mtwkSetRequestedRootMode(scenFindMotionwork(opponent), 0);
        DartsInitStart2(work);
    } else {
        HLib::SetCurrentTaskTo(DartsInitStart2);
        HLib::SetupTaskDelay(5);
    }
}
void DartsPositionOpponentHands(void *data) {
    DartsEventWork *const work = static_cast<DartsEventWork *>(data);
    if (!work) {
        HLib::DestroyCurrentTask();
        return;
    }
    SetPlayerControlEnabled(0);
    const shen_name opponent = work->entry->enemy->name;
    SetLowQualityHands(ctrlGetCharaByName(opponent));
    SetHighModelXYZ(ctrlGetCharaByName(opponent), 1, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(opponent), 1, 1, 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(opponent), 0, reinterpret_cast<std::byte *>(dartsHandPoseCommand), 0);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(opponent), 1, reinterpret_cast<std::byte *>(dartsHandPoseCommand), 0);
    EV_SetCharacterAlphaEnabled(opponent, false);
    EV_SetRefBoardParamsFor(opponent, 1);
    QueueDartsGameArrow(opponent, DART_ADMISSION_ARROW);
    DartsLoadAudioBank(work, 2);
    EV_spmBeginSampleExecute();
    EventFilmControl film{.cleanup = 0, .frames = 30, .startAlpha = 255};
    EVT_AT_MenCtrl_Callback(&film);
    if (GetShenmueFlag(DART_SINGLE_FLAG) && g_currentMap.name == DART_AKS1.name)
        HLib::SetCurrentTaskTo(DartsInitStart);
    else
        HLib::SetCurrentTaskTo(DartsOpponentAdmission);
}
void DartsInitStart2(void *data) {
    DartsEventWork *const work = static_cast<DartsEventWork *>(data);
    SetScenarioCharacterTransparency(DART_PLAYER, 1);
    EV_SetCharacterAlphaEnabled(DART_PLAYER, false);
    EV_SetCharaControllerFlags(DART_PLAYER, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(DART_PLAYER, ApplyGravity | UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(DART_PLAYER, UpdateHeight, NpcCharacterControllerState::Disabled);
    SetScenarioCharacterFlag40(DART_PLAYER, 0);
    if (work->entry->opponentMode == 2) {
        const shen_name opponent = work->entry->enemy->name;
        SetScenarioCharacterFlag40(opponent, 0);
        SetScenarioCharacterTransparency(opponent, 1);
        EV_SetCharacterAlphaEnabled(opponent, false);
        DartsInitializeGame(work);
        SetPlayerGameFlags(PlayerGameFlagMode::Bit5, nullptr);
        HLib::SetCurrentTaskTo(DartsEnemyShot);
    } else {
        EventFilmControl film{.cleanup = 0, .frames = 30, .startAlpha = 255};
        EVT_AT_MenCtrl_Callback(&film);
        EV_spmBeginSampleExecute();
        const Vector3 position{.x = 0, .y = 0, .z = 0.60000002f};
        const Vector3i rotation{};
        SetMotionRegionFlag(scenFindMotionwork(DART_PLAYER), 71, 0, MotionRegion::LowerBody);
        QueueDartsPosition(DART_PLAYER, DartsSelectBoard(work, {}), &position, &rotation, 0);
        const shen_name board = DartsSelectBoard(work, {});
        EVT_AT_SetCam(7200, board, board, 0, 0, 0);
        DartsLoadAudioBank(work, 1);
        HLib::SetCurrentTaskTo(DartsMain);
    }
    SetLowQualityHands(ctrlGetCharaByName(DART_PLAYER));
    SetHighModelXYZ(ctrlGetCharaByName(DART_PLAYER), 1, 0, 0);
    SetHighModelXYZ(ctrlGetCharaByName(DART_PLAYER), 1, 1, 0);
    ApplyCharacterHandRotation(ctrlGetCharaByName(DART_PLAYER), 0, 21, &DART_HAND_ROTATION);
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(DART_PLAYER), 0, reinterpret_cast<std::byte *>(dartsHandPoseCommand), 0);
}
void DartsMain(void *data) {
    DartsEventWork *const work = static_cast<DartsEventWork *>(data);
    if (GetPlayerStat(PlayerStatsId::Cash) >= 5)
        HLib::SetCurrentTaskTo(DartsAskPlay);
    else {
        ConversationBuffer *const voice = DartsResponse(work, 2);
        HLib::SetCurrentTaskTo(DartsMainOk);
        HLib::SetupTaskDelay(30);
        if (voice)
            DartsQueueVoiceWait(work->task, voice);
    }
}
void DartsMainOk(void *data) {
    DartsEventWork *const work = static_cast<DartsEventWork *>(data);
    if (work->requestIcon == 36) {
        DartsStartChildTask(work, DartsChildTask::Frames);
        DartsStartChildTask(work, DartsChildTask::Display);
        HLib::SetCurrentTaskTo(DartsInsertCoin);
    } else
        HLib::SetCurrentTaskTo(DartsExit);
}
void DartsAskPlay(void *data) {
    DartsEventWork *const work = static_cast<DartsEventWork *>(data);
    if (work->nextState == 0) {
        work->nextState = 1;
        const shen_name board = DartsSelectBoard(work, {});
        DartsQueueCameraWait(work, 7207, board, board);
        DartsResponse(work, 0);
        HLib::SetupTaskDelay(135);

    } else if (work->nextState == 1) {
        work->nextState = 2;
        SetMotionFrameTime(scenFindMotionwork(DART_PLAYER), 5, MotionRegion::LowerBody);
        std::int16_t choices[4]{-1, 36, 23, -1};
        SetupControlSpeakAndUI(choices, 1800, 23, 2);
    } else if (work->nextState == 2 && !GetShouldRenderIcons(RenderIconSlot::ActionSelector)) {
        work->nextState = 0;
        work->requestIcon = shenGetCtrlSpeakIconFlag();
        ConversationBuffer *voice;
        if (work->requestIcon == 36) {
            voice = DartsResponse(work, 1);
            if (DartsSelectBoard(work, {}).name != DART_COMPACT.name)
                SetCurrentGameFlag(8, 1);
        } else {
            voice = DartsResponse(work, 3);
            EVT_AT_SetCam(7205, DART_PLAYER, DART_PLAYER, 0, 0, 0);
        }
        HLib::SetCurrentTaskTo(DartsMainOk);
        HLib::SetupTaskDelay(30);
        if (voice)
            DartsQueueVoiceWait(work->task, voice);
    }
}

void DartsAttachReadyArrows() {
    const Vector3 position{.x = 0.088200003f, .y = -0.0112f, .z = 0};
    const Vector3i rotation{};
    const Vector3i attachedRotation{.x = 0, .y = 2001, .z = 15469};
    for (std::int32_t index = 0; index < 5; ++index) {
        const shen_name arrow = DartsArrowName(index, false);
        EV_EnsureCharacterDraw(arrow);
        EV_EnableCharacterDisplay(arrow);
        scenSetCharaPosRotById(arrow, &position, &rotation);
        AttachScenarioCharacter(arrow, DART_PLAYER, 21, &DART_READY_ARROW_OFFSETS[index], &attachedRotation);
    }
}
void DartsInsertCoin(void *data) {
    DartsEventWork *const work = static_cast<DartsEventWork *>(data);
    if (!work->gameState) {
        work->gameState = 1;
        DartsResetScoring(work);
        for (std::int32_t index = 0; index < 5; ++index) {
            const shen_name arrow = DartsArrowName(index, false);
            EV_DisableCharacterDisplay(arrow);
            AT_SetLocalBaseChara(arrow, DartsSelectBoard(work, {}));
        }
        SetPlayerGameFlags(PlayerGameFlagMode::Bit5, nullptr);
        const Vector3 position{.x = 0, .y = 0, .z = 0.60000002f};
        const Vector3i rotation{};
        SetMotionRegionFlag(scenFindMotionwork(DART_PLAYER), 71, 0, MotionRegion::LowerBody);
        QueueDartsPosition(DART_PLAYER, DartsSelectBoard(work, {}), &position, &rotation, 0);
        EventSetMotion motion{.cleanup = 0, .character = DART_PLAYER, .animation = 6369, .frame = -1, .speed = 1, .flags = 0x80000000U, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(6369);
        EV_mtwkSetEnabled(scenFindMotionwork(DART_PLAYER), 1);
        const shen_name board = DartsSelectBoard(work, {});
        EVT_AT_SetCam(7203, board, board, 0, 0, 0);
        DartsQueueCameraWait(work, 7204, board, board);
        AddThisToCashOrSegaCoins(-5, ECashOrCoins::Cash2);
    }
    if (work->gameState == 1) {
        const std::int32_t frame = static_cast<std::int32_t>(GetMotionFrame(scenFindMotionwork(DART_PLAYER), MotionRegion::LowerBody));
        Vector3 position;
        Vector3i rotation;
        if (frame == 30) {
            Loop3DSoundEffect(381, frame);
            EV_GetCharaPosition(DART_PLAYER, 14, &position, EventTransformFlags::World);
            EV_AT_SetPos(DART_COIN, EventTransformFlags::AllAxes, position);
        } else if (frame == 31) {
            EV_EnsureCharacterDraw(DART_COIN);
            EV_EnableCharacterDisplay(DART_COIN);
            position = {.x = 0.124f, .y = DART_INNER_BULL_RADIUS, .z = -0.085000001f};
            rotation = {.x = 64990, .y = 64260, .z = 46788};
            AttachScenarioCharacter(DART_COIN, DART_PLAYER, 14, &position, &rotation);
        } else if (frame == 140) {
            EV_EnsureCharacterDraw(DartsSelectBoard(work, {}));
            EV_EnableCharacterDisplay(DartsSelectBoard(work, {}));
            Loop3DSoundEffect(0x17c, frame);
            ResetCharaFixo(ctrlGetCharaByName(DART_COIN));
            EV_DisableCharacterDisplay(DART_COIN);
            for (std::int32_t index = 0; index < 5; ++index) {
                Vector3 hidden{.x = 0, .y = 0, .z = -10.05f};
                Vector3i zero{};
                EV_scenApplyPosOrRotToPartOf(
                    DartsSelectBoard(work, {}), static_cast<std::uint16_t>(index + 153), &hidden, &zero, EventCharacterPartTransformMode::Absolute);
            }
        } else if (frame == 169) {
            ++work->gameState;
            const Vector3 shifted{.x = -0.045000002f, .y = 0, .z = 0.60000002f};
            EventDartsPositionTaskData placement{
                .cleanupTask = 0, .hasPosition = 1, .position = shifted, .parent = DartsSelectBoard(work, {}), .character = DART_PLAYER};
            DartsPositionCallback(&placement);
            const shen_name board = DartsSelectBoard(work, {});
            EVT_AT_SetCam(7212, board, board, 0, 0, 0);
            EventSetMotion motion{.cleanup = 0, .character = DART_PLAYER, .animation = 6372, .frame = -1, .speed = 1, .flags = 0x80000000U, .delay = 0};
            EVT_AT_SetMot_Callback(&motion);
            GetMotionFrameCount(6372);
            Loop3DSoundEffect(0x180, shifted.x);
        }
        const Vector3 offset{.x = 1, .y = 1, .z = 1};
        SetCharaTargetTransformOffset(DART_PLAYER, &offset, 1);
    } else if (work->gameState == 2) {
        if (SetMotionRegionFlag(scenFindMotionwork(DART_PLAYER), 65, 1, MotionRegion::LowerBody)) {
            ++work->gameState;
            EV_mtwkSetEnabled(scenFindMotionwork(DART_PLAYER), 0);
            return;
        }
        const std::int32_t frame = static_cast<std::int32_t>(GetMotionFrame(scenFindMotionwork(DART_PLAYER), MotionRegion::LowerBody));
        Vector3 position;
        if (frame == 55) {
            Loop3DSoundEffect(0x181, frame);
            EV_GetCharaPosition(DART_PLAYER, 21, &position, EventTransformFlags::World);
            for (std::int32_t index = 0; index < 5; ++index)
                EV_AT_SetPos(DartsArrowName(index, false), EventTransformFlags::AllAxes, position);
        } else if (frame == 56)
            DartsAttachReadyArrows();
        else if (frame == 125) {
            const shen_name board = DartsSelectBoard(work, {});
            EVT_AT_SetCam(7208, board, board, 0, 0, 0);
        } else if (frame == 180) {
            EV_GetCharaPosition(DART_PLAYER, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
            EV_AT_SetPos(DART_FIRST_ARROW, EventTransformFlags::AllAxes, position);
        } else if (frame == 181) {
            position = {.x = 0.088200003f, .y = -0.0112f, .z = 0};
            Vector3i rotation{};
            scenSetCharaPosRotById(DART_FIRST_ARROW, &position, &rotation);
            position = {.x = 0.13699999f, .y = 0.028000001f, .z = -0.068000004f};
            rotation.z = 15834;
            AttachScenarioCharacter(DART_FIRST_ARROW, DART_PLAYER, 14, &position, &rotation);
        }
        const Vector3 offset{.x = 0, .y = 0, .z = 0.5f};
        SetCharaTargetTransformOffset(DART_PLAYER, &offset, 1);
    } else if (work->gameState == 3) {
        work->gameState = 0;
        DartsResetRound(work);
        DartsStartChildTask(work, DartsChildTask::VoiceMonitor);
        HLib::SetCurrentTaskTo(DartsPlaying);
    }
}
void DartsSetActorVisibility(DartsEventWork *work) {
    if (work->entry->opponentMode != 2)
        return;
    if (work->enemyTurn) {
        EV_DisableCharacterDisplay(DART_PLAYER);
        EV_EnableCharacterDisplay(work->entry->enemy->name);
    } else {
        EV_EnableCharacterDisplay(DART_PLAYER);
        EV_DisableCharacterDisplay(work->entry->enemy->name);
    }
}
void DartsPositionActors(DartsEventWork *work) {
    const std::int32_t index = work->state % 5;
    const shen_name arrow = DartsArrowName(index, false);

    Vector3 position{.x = 0.088200003f, .y = -0.0112f, .z = 0};
    Vector3i rotation{};
    scenSetCharaPosRotById(arrow, &position, &rotation);
    if (work->standardBoard) {
        position = {.x = 0.13600001f, .y = 0.024f, .z = -0.072999999f};
        rotation = {.x = 62077, .y = 2000, .z = 15104};
    } else {
        position = {.x = 0.13699999f, .y = 0.028000001f, .z = -0.068000004f};
        rotation = {.x = 0, .y = 0, .z = 15834};
    }
    const shen_name actor = work->entry->opponentMode == 2 && work->enemyTurn ? work->entry->enemy->name : DART_PLAYER;
    AttachScenarioCharacter(arrow, actor, DartsOpponentAttachBone(actor, work->difficulty), &position, &rotation);
}
float DartsVerticalWobble(std::int16_t phase) {
    const float first = sin_fixed16(static_cast<std::uint16_t>(22 * phase)) * 0.30000001f;
    const float second = sin_fixed16(static_cast<std::uint16_t>(31 * phase)) * 0.37f + first;
    const float third = sin_fixed16(static_cast<std::uint16_t>(103 * phase)) * 0.15000001f + second;
    return sin_fixed16(static_cast<std::uint16_t>(147 * phase)) * 0.18000001f + third;
}
void DartsWobbleVertical(DartsEventWork *work) {
    const shen_name arrow = DartsArrowName(work->state % 5, false);
    const shen_name resolved = EV_SCEN_FindChar_Wrapper(arrow);
    const float wobble = DartsVerticalWobble(static_cast<std::int16_t>(work->wobblePhaseX));
    const std::uint32_t next = work->wobblePhaseX + 18U;
    DartsVerticalWobble(static_cast<std::int16_t>(next));
    Vector3 position;
    EV_GetCharaPosition(arrow, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    position.y = wobble * 0.050000001f + 1.5891f;
    EV_AT_SetPos(arrow, EventTransformFlags::AllAxes, position);
    AT_SetLocalBaseChara(DART_COIN, resolved);
    EV_GetCharaPosition(DART_COIN, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    position.y = wobble * 0.050000001f + 1.5891f;
    EV_AT_SetPos(DART_COIN, EventTransformFlags::AllAxes, position);
    AT_SetLocalBaseChara(DART_COIN, EVENT_CHARACTER_INVALID);
    work->wobblePhaseX = next;
}
void DartsWobbleHorizontal(DartsEventWork *work) {
    const shen_name arrow = DartsArrowName(work->state % 5, false);
    const shen_name resolved = EV_SCEN_FindChar_Wrapper(arrow);
    const float wobble = sin_fixed16(static_cast<std::uint16_t>(work->wobblePhaseY)) * 0.039999999f;
    const std::uint32_t next = work->wobblePhaseY + 218U;
    sin_fixed16(static_cast<std::uint16_t>(next));
    Vector3 position;
    EV_GetCharaPosition(arrow, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    position.x = wobble - 0.119f;
    EV_AT_SetPos(arrow, EventTransformFlags::AllAxes, position);
    AT_SetLocalBaseChara(DART_COIN, resolved);
    EV_GetCharaPosition(DART_COIN, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    position.x = wobble - 0.119f;
    EV_AT_SetPos(DART_COIN, EventTransformFlags::AllAxes, position);
    AT_SetLocalBaseChara(DART_COIN, EVENT_CHARACTER_INVALID);
    work->wobblePhaseY = next;
}
void DartsRotateAimingArrow(DartsEventWork *work) {
    const shen_name arrow = DartsArrowName(work->state % 5, false);
    Vector3 position;
    EV_GetCharaPosition(arrow, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    position.y -= 1.5891f;
    position.x += 0.119f;
    position.z += 0.037799999f;
    Vector3i rotation;
    rotation.y = InverseFixedAtan2(-position.x, -position.z);
    const std::int16_t pitch = InverseFixedAtan2(-position.y, -std::sqrt(position.x * position.x + position.z * position.z));
    rotation.z = 22478;
    rotation.x = 10279 - pitch;
    rotation.y = 50706 - rotation.y;
    EV_AT_SetAngY(arrow, EventTransformFlags::AllAxes, rotation);
}
void DartsProjectImpact(DartsEventWork *work, Vector3 *impact) {
    const shen_name arrow = EV_SCEN_FindChar_Wrapper(DartsArrowName(work->state % 5, false));
    Vector3 origin{.x = -0.119f, .y = 1.5891f, .z = 1.3261f}, current;
    Vector2 currentScreen, originScreen;
    AT_SetLocalBaseChara(DART_COIN, EVENT_CHARACTER_INVALID);
    EV_GetCharaPosition(DART_COIN, CHARACTER_TRANSLATION_ROOT, &current, EventTransformFlags::Local);
    ProjectWorldToScreen(&current, &currentScreen);
    AT_SetLocalBaseChara(DART_COIN, arrow);
    EV_AT_SetPos(DART_COIN, EventTransformFlags::AllAxes, origin);
    AT_SetLocalBaseChara(DART_COIN, EVENT_CHARACTER_INVALID);
    EV_GetCharaPosition(DART_COIN, CHARACTER_TRANSLATION_ROOT, &origin, EventTransformFlags::Local);
    ProjectWorldToScreen(&origin, &originScreen);
    const float width = static_cast<float>(d3t::g_originalScreenWidth), height = static_cast<float>(d3t::g_originalScreenHeight);
    impact->x = std::fmin((width / height - 1.3333334f) + 1.0f, 1.33f) * (((currentScreen.x - originScreen.x) * 0.16599999f / 85.5f / width) * 640.0f);
    impact->y = ((originScreen.y - currentScreen.y) * 0.16599999f / 85.5f / height) * 480.0f + 1.4174f;
    impact->z = -0.068200007f;
    EV_AT_SetPos(DART_COIN, EventTransformFlags::AllAxes, current);
}
void DartsComputeCompactImpact(DartsEventWork *work, Vector3 *impact) {
    const float frame = GetMotionFrame(scenFindMotionwork(DART_PLAYER), MotionRegion::LowerBody);
    const std::int32_t phase = static_cast<std::int32_t>(work->wobblePhaseY);
    if (phase <= 22)
        impact->x = phase * 0.0090909088f;
    else if (phase <= 44)
        impact->x = 0.2f - (phase - 22) * 0.0090909088f;
    else if (phase <= 67)
        impact->x = (phase - 44) * -0.0086956527f;
    else
        impact->x = (phase - 67) * 0.0086956527f - 0.2f;
    if (frame <= 13)
        impact->y = frame * -0.2f / 13.0f;
    else if (frame <= 37)
        impact->y = (frame - 13) * 0.40000001f / 24.0f - 0.2f;
    else
        impact->y = 0.2f - (frame - 37) * 0.2f / 7.0f;
    impact->y += 1.4174f;
    impact->z = -0.068200007f;
    impact->x += GetRandomNumberInRange(17) * 0.0020000001f - 0.017000001f;
    impact->y += GetRandomNumberInRange(17) * 0.0020000001f - 0.017000001f;
}
void DartsCompactWobble(DartsEventWork *work) {
    work->wobblePhaseY %= 90;
    const std::int32_t phase = static_cast<std::int32_t>(work->wobblePhaseY);
    std::int32_t angle;
    if (phase <= 22)
        angle = 41 * phase;
    else if (phase <= 44)
        angle = 1812 - 41 * phase;
    else if (phase <= 67)
        angle = 39 * (44 - phase);
    else
        angle = 39 * phase - 3523;
    EventDartsPositionTaskData placement{
        .cleanupTask = 0, .hasRotation = 1, .rotation = {.x = 0, .y = -angle, .z = 0}, .parent = DartsSelectBoard(work, {}), .character = DART_PLAYER};
    DartsPositionCallback(&placement);
    ++work->wobblePhaseY;
}
// @verified - p
bool DartsAiTriggerReady(DartsEventWork *work) {
    if (!work->standardBoard)
        return false;
    Vector3 impact;
    DartsProjectImpact(work, &impact);
    const float outer = work->entry->maximumRadius;
    float maximum = work->targetBand * outer;
    if (maximum < 10)
        maximum = outer < 10 ? outer : 10;
    const float inner = static_cast<std::int8_t>(work->entry->minimumRadius);
    float minimum = inner * work->targetBand;
    if (minimum >= 10 || inner <= 10) {
        if (maximum < 10)
            maximum = outer;
    } else
        minimum = 10;
    if (work->field09)
        return false;
    const float y = impact.y * DART_BOARD_Y_SCALE;
    const float radius = static_cast<float>(std::sqrt(static_cast<double>(impact.x * impact.x + y * y)));
    return static_cast<double>(maximum * 0.2f) / 100.0 > radius && radius > static_cast<double>(minimum * 0.2f) / 100.0;
}
void DartsPlayingLoop(void *data) {
    DartsEventWork *const work = static_cast<DartsEventWork *>(data);
    if (work->gameState != 0 && work->gameState != 1)
        return;
    bool throwNow = false;
    if (!work->gameState) {
        if (work->entry->opponentMode != 2 || !work->enemyTurn) {
            GetBattleDataConfig(0, DART_THROW_BUTTON);
            if (work->actorControl[1])
                --work->actorControl[1];
            else
                throwNow = GetCurrentControlInput(INPUT_PRESSED) == PAD_BUTTON_X;
        } else if (!work->field08)
            work->field08 = static_cast<std::uint8_t>(DartsAiTriggerReady(work));

        else {
            work->field08 = 0;
            throwNow = true;
        }
    }
    work->gameState = 0;
    if (!throwNow) {
        if (!work->standardBoard)
            DartsCompactWobble(work);
        else {
            DartsWobbleVertical(work);
            DartsWobbleHorizontal(work);
            DartsRotateAimingArrow(work);
            const Vector3i rotation{.x = std::bit_cast<std::int32_t>(work->ryoRotation.x),
                .y = std::bit_cast<std::int32_t>(work->ryoRotation.y),
                .z = std::bit_cast<std::int32_t>(work->ryoRotation.z)};
            QueueDartsPosition(DART_PLAYER, DartsArrowName(work->state % 5, false), &work->dartPosition, &rotation, 0);
        }
        return;
    }
    if (work->standardBoard)
        DartsProjectImpact(work, &work->impactPosition);
    else
        DartsComputeCompactImpact(work, &work->impactPosition);
    DartsPositionActors(work);
    mtwkRequestMotion(scenFindMotionwork(DART_PLAYER), 6370, MotionRegion::LowerBody);
    SetMotionLayerEndFrame(scenFindMotionwork(DART_PLAYER), 70, MotionRegion::LowerBody);
    SetMotionSpeed(scenFindMotionwork(DART_PLAYER), 1, MotionRegion::LowerBody);
    HLib::SetCurrentTaskTo(DartsGameMain);
}
void DartsPlaying(void *data) {
    DartsEventWork *const work = static_cast<DartsEventWork *>(data);
    if (!work->gameState) {
        work->gameState = 1;
        if (work->state == 1) {
            ConversationBuffer *const voice = DartsResponse(work, 4);
            if (voice)
                DartsQueueVoiceWait(work->task, voice);
            else
                HLib::SetupTaskDelay(30);
        }
    } else if (work->gameState == 1) {
        work->gameState = 2;
        if (!work->standardBoard)
            work->wobblePhaseY = 0;
        else {
            work->wobblePhaseX = static_cast<std::uint32_t>(GetRandomNumberInRange(360) * 8192.0f / 45.0f);
            work->wobblePhaseY = static_cast<std::uint32_t>(GetRandomNumberInRange(360) * 8192.0f / 45.0f);
        }
        DartsPositionActors(work);
    } else if (work->gameState == 2) {
        work->gameState = 0;
        DartsSetActorVisibility(work);
        const shen_name arrow = DartsArrowName(work->state % 5, false);
        EV_EnableCharacterDisplay(arrow);
        const shen_name board = DartsSelectBoard(work, {});
        Vector3 position{.x = work->standardBoard ? -0.015f : 0.010000001f, .y = -0.0020000001f, .z = 1.8f};
        const Vector3i zero{};
        SetMotionRegionFlag(scenFindMotionwork(DART_PLAYER), 71, 0, MotionRegion::LowerBody);
        QueueDartsPosition(DART_PLAYER, board, &position, &zero, 0);
        if (work->standardBoard) {
            EVT_AT_SetMot(DART_PLAYER, 6371, 27, 0, 0x80000000U, 0);
            EV_SetCharacterAlphaEnabled(DART_PLAYER, true);
            SetScenarioCharacterTransparency(DART_PLAYER, 0.5f);
            ResetCharaFixo(ctrlGetCharaByName(arrow));
            EventSetPosition placement{.cleanup = 0, .character = arrow, .parent = board, .position = DART_AIM_ARROW_POSITION, .delay = 0};
            EVT_AT_SetPosRot(&placement);
            EVT_AT_SetAng_Enqueue(arrow, board, 58.2715f, 279.1571f, 119.5203f, 0);
            AT_SetLocalBaseChara(DART_PLAYER, arrow);
            EV_GetCharaPosition(DART_PLAYER, CHARACTER_TRANSLATION_ROOT, &work->dartPosition, EventTransformFlags::Local);
            Vector3i rotation;
            GetScenarioCharacterEulerAngles(DART_PLAYER, &rotation, EventTransformFlags::Local);
            work->ryoRotation = {.x = std::bit_cast<float>(rotation.x), .y = std::bit_cast<float>(rotation.y), .z = std::bit_cast<float>(rotation.z)};
            AT_SetLocalBaseChara(DART_PLAYER, EVENT_CHARACTER_INVALID);
            EV_GetCharaPosition(arrow, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
            EV_AT_SetPos(DART_COIN, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
        } else {
            EVT_AT_SetMot(DART_PLAYER, 6371, 44, 0, 0x80000000U, 0);
            SetMotionSpeed(scenFindMotionwork(DART_PLAYER), 1, MotionRegion::LowerBody);
        }
        if (work->entry->opponentMode == 2 && work->enemyTurn)
            DartsSelectAiTargetBand(work);
        work->scoreTicker = 100000;
        work->opponentScore.impactIterations = 0;
        work->scoreUiFlags = 1;
        work->actorControl[1] = 5;
        EVT_AT_SetCam(work->standardBoard ? 7201 : 7218, board, board, 0, 0, 0);
        HLib::SetCurrentTaskTo(DartsPlayingLoop);
        DartsPlayingLoop(work);
    }
}

void DartsProjectile(void *data) {
    DartsProjectileWork *const work = static_cast<DartsProjectileWork *>(data);
    if (!work) {
        UnregisterEventTask(HLib::GetCurrentTask());
        HLib::DestroyCurrentTask();
        return;
    }
    if (work->frame >= work->maximumFrames) {
        EV_AT_SetPos(work->arrow, EventTransformFlags::AllAxes, work->target);
        EV_AT_SetAngY(work->arrow, EventTransformFlags::AllAxes, work->targetRotation);
        const float random = GetRandomNumberInRange(2);
        Loop3DSoundEffect(random >= 1 ? 383 : 382, random);
        UnregisterEventTask(HLib::GetCurrentTask());
        HLib::DestroyCurrentTask();
    } else {
        const Vector3 position{.x = work->frame * work->step.x + work->start.x,
            .y = work->frame * work->step.y + work->start.y + sin_fixed16(static_cast<std::uint16_t>(work->arcPhase)) * 0.5f,
            .z = work->frame * work->step.z + work->start.z};
        EV_AT_SetPos(work->arrow, EventTransformFlags::AllAxes, position);
        EV_AT_SetAngY(work->arrow, EventTransformFlags::AllAxes, work->targetRotation);
        work->arcPhase += work->arcStep;
        ++work->frame;
    }
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(work->arrow, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(work->arrow, &rotation, EventTransformFlags::World);
    EV_AT_SetPos(DART_COIN, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(DART_COIN, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
}
void DartsQueueProjectile(shen_name board, shen_name arrow, std::int32_t score, const Vector3 *impact, float duration, bool arc) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(DartsProjectile, 4, 11, sizeof(DartsProjectileWork), DART_TASK);
    RegisterEventTask(task);
    DartsProjectileWork *const work = static_cast<DartsProjectileWork *>(HLib::GetTaskParameter(task));
    if (!work)
        return;
    work->board = board;
    work->arrow = arrow;
    work->arcPhase = 0;
    work->maximumFrames = static_cast<std::int32_t>(static_cast<std::uint32_t>(static_cast<std::int32_t>(duration)) - 1U);
    work->arcStep = arc ? static_cast<std::uint16_t>(static_cast<std::int32_t>(180.0 / work->maximumFrames * 8192.0 / 45.0)) : 0;
    work->frame = 0;
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(arrow, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(arrow, &rotation, EventTransformFlags::World);
    EV_AT_SetPos(DART_COIN, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(DART_COIN, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    EV_DisableCharacterDisplay(DART_COIN);
    EV_EnsureCharacterDraw(DART_COIN);
    if (impact)
        work->target = *impact;
    else
        CalculateDartsPositionOrScore(score / 10000, &work->target);
    EV_GetCharaPosition(arrow, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    GetScenarioCharacterEulerAngles(arrow, &rotation, EventTransformFlags::Local);
    const Vector3 target = work->target;
    const Vector3i boardRotation{.x = 0, .y = 49152, .z = 0};
    SetMotionRegionFlag(scenFindMotionwork(arrow), 71, 0, MotionRegion::LowerBody);
    QueueDartsPosition(arrow, board, &target, &boardRotation, 0);
    EV_GetCharaPosition(arrow, CHARACTER_TRANSLATION_ROOT, &work->target, EventTransformFlags::Local);
    GetScenarioCharacterEulerAngles(arrow, &work->targetRotation, EventTransformFlags::Local);
    work->step = {.x = (work->target.x - position.x) / duration, .y = (work->target.y - position.y) / duration, .z = (work->target.z - position.z) / duration};
    position.x += work->step.x;
    position.y += work->step.y;
    position.z += work->step.z;
    EV_AT_SetPos(arrow, EventTransformFlags::AllAxes, position);
    work->start = position;
    EV_AT_SetAngY(arrow, EventTransformFlags::AllAxes, rotation);
    Loop3DSoundEffect(204, position.x);
}
void DartsGameMain(void *data) {
    DartsEventWork *const work = static_cast<DartsEventWork *>(data);
    if (GetMotionFrame(scenFindMotionwork(DART_PLAYER), MotionRegion::LowerBody) < 26)
        return;
    const shen_name arrow = DartsArrowName(work->state % 5, false);
    ResetCharaFixo(ctrlGetCharaByName(arrow));
    DartsQueueProjectile(DartsSelectBoard(work, {}), arrow, 0, &work->impactPosition, 13, false);

    EVT_AT_SetCam(7202, DART_COIN, DART_COIN, 0, 0, 0);
    const shen_name board = DartsSelectBoard(work, {});
    DartsQueueCameraWait(work, 7206, board, board);
    HLib::SetCurrentTaskTo(DartsGameScore);
    HLib::SetupTaskDelay(13);
}
void DartsEnemyShot(void *data) {
    DartsEventWork *const work = static_cast<DartsEventWork *>(data);
    const shen_name opponent = work->entry->enemy->name;
    work->actorControl[0] = 1;
    EV_EnableCharacterDisplay(DART_PLAYER);
    EV_EnableCharacterDisplay(opponent);
    EV_SetCharacterAlphaEnabled(opponent, false);
    EV_SetCharacterAlphaEnabled(DART_PLAYER, false);
    const shen_name board = DartsSelectBoard(work, {});
    DartsCamera(work, static_cast<std::int32_t>(DartsOpponentIndex(opponent)) + 7230, board, board, 0);
    const std::int32_t playerWon = work->playerWon;
    scenCvsCtrlFree(0);
    dartsPlayerWon = playerWon;
    if (work->entry->opponentMode == 2)
        PlayDartsThrowStartResponse(1, !work->enemyTurn, opponent);
    else
        PlayDartsThrowStartResponse(0, true, EVENT_CHARACTER_INVALID);
    const Vector3 position{.x = 0, .y = 0, .z = 4};
    const Vector3i rotation{};
    SetMotionRegionFlag(scenFindMotionwork(opponent), 71, 0, MotionRegion::LowerBody);
    QueueDartsPosition(opponent, board, &position, &rotation, 0);
    const Vector3 ryoPosition{.x = 0.80000001f, .y = 0, .z = 4.6999998f};
    const Vector3i ryoRotation{.x = 0, .y = 1820, .z = 0};
    SetMotionRegionFlag(scenFindMotionwork(DART_PLAYER), 71, 0, MotionRegion::LowerBody);
    QueueDartsPosition(DART_PLAYER, board, &ryoPosition, &ryoRotation, 0);
    EventSetMotion motion{.cleanup = 0, .character = DART_PLAYER, .animation = 61442, .frame = -1, .speed = 1, .flags = 0x80000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(61442);
    HLib::SetCurrentTaskTo(DartsEnemyPlaying);
    DartsEnemyPlaying(work);
}
void DartsEnemyPlaying(void *data) {
    DartsEventWork *const work = static_cast<DartsEventWork *>(data);
    const shen_name opponent = work->entry->enemy->name;
    if (work->gameState == 100) {
        if (HasPressedCancel()) {
            EV_CleanupMapControlTask();
            work->resetFrame = 0;
        }
        const std::int32_t frames = static_cast<std::int32_t>(work->resetFrame);
        --work->resetFrame;
        if (frames <= 0)
            work->gameState = 1;
    } else if (work->gameState == 1) {
        work->gameState = 0;
        const shen_name arrow = DartsArrowName(work->state % 5, true);
        ResetCharaFixo(ctrlGetCharaByName(arrow));
        const float duration = opponent.name == DART_FCJ.name ? 26 : 13;
        DartsQueueProjectile(DartsSelectBoard(work, {}), arrow, 0, &work->impactPosition, duration, opponent.name == DART_FCJ.name);
        EVT_AT_SetCam(7202, DART_COIN, DART_COIN, 0, 0, 0);
        const shen_name board = DartsSelectBoard(work, {});
        EVT_AT_SetCam(7206, board, board, 0, 0, static_cast<std::int32_t>(duration) + 15);
        HLib::SetCurrentTaskTo(DartsGameScore);
        HLib::SetupTaskDelay(static_cast<std::int32_t>(duration));
    } else if (!work->gameState) {
        work->scoreTicker = 100000;
        work->scoreUiFlags = 1;
        EV_EnableCharacterDisplay(DART_PLAYER);
        EV_SetCharacterAlphaEnabled(DART_PLAYER, false);
        EV_EnableCharacterDisplay(opponent);
        EV_SetCharacterAlphaEnabled(opponent, false);
        const shen_name board = DartsSelectBoard(work, {});
        const Vector3 position{.x = 0, .y = 0, .z = 4};
        const Vector3i rotation{};
        SetMotionRegionFlag(scenFindMotionwork(opponent), 71, 0, MotionRegion::LowerBody);
        QueueDartsPosition(opponent, board, &position, &rotation, 0);
        const Vector3 ryoPosition{.x = 0.80000001f, .y = 0, .z = 4.6999998f};
        const Vector3i ryoRotation{.x = 0, .y = 1820, .z = 0};
        SetMotionRegionFlag(scenFindMotionwork(DART_PLAYER), 71, 0, MotionRegion::LowerBody);
        QueueDartsPosition(DART_PLAYER, board, &ryoPosition, &ryoRotation, 0);
        EventSetMotion motion{.cleanup = 0, .character = DART_PLAYER, .animation = 61442, .frame = -1, .speed = 1, .flags = 0x80000000U, .delay = 0};
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(61442);
        const shen_name arrow = DartsArrowName(work->state % 5, true);
        EV_EnableCharacterDisplay(arrow);
        EV_EnsureCharacterDraw(arrow);
        const std::int32_t bone = DartsOpponentAttachBone(opponent, work->difficulty);
        const Vector3 &offset = bone == 21 ? DART_JON_ARROW_OFFSET : DART_OPPONENT_ARROW_OFFSETS[DartsOpponentIndex(opponent)];
        const Vector3i attachedRotation{.x = DART_ARROW_ROTATION[0], .y = DART_ARROW_ROTATION[1], .z = DART_ARROW_ROTATION[2]};
        AttachScenarioCharacter(arrow, opponent, bone, &offset, &attachedRotation);
        if (!work->actorControl[0])
            EVT_AT_SetCam(static_cast<std::int32_t>(DartsOpponentIndex(opponent)) + 7230, board, board, 0, 0, 1);
        DartsSelectAiTargetBand(work);
        work->opponentScore.impactIterations = DartsComputeAiImpactPoint(work);
        work->gameState = 100;
        work->resetFrame = DartsBeginOpponentThrow(opponent, work->difficulty);
        SetMotionRegionFlag(scenFindMotionwork(opponent), 71, 0, MotionRegion::LowerBody);
    }
}
void DartsRyoShot(void *data) {
    DartsEventWork *const work = static_cast<DartsEventWork *>(data);
    const shen_name opponent = work->entry->enemy->name;
    work->actorControl[0] = 0;
    EV_EnableCharacterDisplay(DART_PLAYER);
    EV_EnableCharacterDisplay(opponent);
    DartsPositionActors(work);
    EV_EnableCharacterDisplay(DartsArrowName(work->state % 5, false));
    EV_SetCharacterAlphaEnabled(opponent, false);
    EV_SetCharacterAlphaEnabled(DART_PLAYER, false);
    const shen_name board = DartsSelectBoard(work, {});
    EVT_AT_SetCam(7225, board, board, 0, 0, 0);
    DartsResponse(work, 28);
    const Vector3 opponentPosition{.x = -1, .y = 0, .z = 3.5f};
    const Vector3i opponentRotation{.x = 0, .y = 40960, .z = 0};
    SetMotionRegionFlag(scenFindMotionwork(opponent), 71, 0, MotionRegion::LowerBody);
    QueueDartsPosition(opponent, board, &opponentPosition, &opponentRotation, 0);
    SetMotionRegionFlag(scenFindMotionwork(opponent), 71, 0, MotionRegion::LowerBody);
    QueueDartsPosition(opponent, board, &opponentPosition, &opponentRotation, 60);
    const Vector3 position{.x = 0, .y = 0, .z = 4};
    const Vector3i rotation{};
    SetMotionRegionFlag(scenFindMotionwork(DART_PLAYER), 71, 0, MotionRegion::LowerBody);
    QueueDartsPosition(DART_PLAYER, board, &position, &rotation, 0);
    EventSetMotion motion{.cleanup = 0, .character = DART_PLAYER, .animation = 6371, .frame = -1, .speed = 1, .flags = 0x80000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(6371);
    motion.character = opponent;
    motion.animation = 61442;
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(61442);
    HLib::SetCurrentTaskTo(DartsPlaying);
    HLib::SetupTaskDelay(120);
}
std::int32_t DartsCompactScore(Vector3 *position) {
    const float y = (position->y - 1.4174f) * DART_BOARD_Y_SCALE;
    const float radius = static_cast<float>(std::sqrt(static_cast<double>(position->x * position->x + y * y)));
    if (radius <= 0.016000001f)
        return 100;
    if (radius <= 0.046999998f)
        return 50;
    if (radius <= 0.079000004f)
        return 40;
    if (radius <= 0.11f)
        return 30;
    if (radius <= 0.147f)
        return 20;
    return radius <= 0.20100001f ? 10 : 0;
}
void DartsGameScore(void *data) {
    DartsEventWork *const work = static_cast<DartsEventWork *>(data);
    if (!work->gameState) {
        work->gameState = 1;
        work->opponentScore.score = work->standardBoard ? static_cast<std::int32_t>(CalculateDartsPositionOrScore(-1, &work->impactPosition)) :
                                                          DartsCompactScore(&work->impactPosition);
        if (work->opponentScore.score == 500000)
            ++work->opponentBullseyes;
        work->scoreUiFlags = 0;
    } else if (work->gameState == 1) {
        work->gameState = 0;
        const std::int8_t part = static_cast<std::int8_t>(5 - work->state);
        if (part < 5) {
            Vector3 position{};
            Vector3i rotation{};
            EV_scenApplyPosOrRotToPartOf(
                DartsSelectBoard(work, {}), static_cast<std::uint16_t>(part + 153), &position, &rotation, EventCharacterPartTransformMode::Absolute);
        }
        if (work->entry->opponentMode == 2 && !work->scoreStep)
            work->scoreStep = 1;
        else {
            if (work->entry->opponentMode == 2)
                work->scoreStep = 0;
            --work->state;
        }
        EVT_AT_SetMot(DART_PLAYER, work->state <= 0 ? 61442 : 6371, work->state <= 0 ? -1 : 0, work->state <= 0 ? 1 : 0, 0x80000000U, 0);
        HLib::SetCurrentTaskTo(DartsShotEnd);
        HLib::SetupTaskDelay(15);
    }
}
void DartsBeginScoreTransfer(DartsEventWork *work, std::int32_t *score) {
    work->pendingScore = work->standardBoard ? *score / 10000 : *score;
    if (!work->pendingScore) {

        work->scoreTarget = score;
        HLib::SetCurrentTaskTo(DartsScoreAdd);
        work->opponentScore.accumulated += *score;
        *score = 0;
        return;
    }
    if (work->pendingScore >= 20) {
        work->scoreTickInterval = 1;
        work->scoreTick[1] = work->pendingScore >= 40 ? 2 : 1;
    } else {
        work->scoreTick[1] = 1;
        work->scoreTickInterval = 2;
    }
    if (work->standardBoard) {
        work->scoreTick[1] *= 10000;
        work->pendingScore *= 10000;
    }
    work->scoreTick[0] = work->scoreTickInterval;
    Loop3DSoundEffect(0x184);
    Loop3DSoundEffect(0x183);
    if (DartsAdvanceScoreTransfer(work, score, 0)) {
        work->opponentScore.accumulated += *score;
        *score = 0;
        return;
    }
    work->scoreTarget = score;
    HLib::SetCurrentTaskTo(DartsScoreAdd);
}
void DartsScoreAdd(void *data) {
    DartsEventWork *const work = static_cast<DartsEventWork *>(data);
    if (DartsAdvanceScoreTransfer(work, work->scoreTarget, 0)) {
        work->opponentScore.accumulated += *work->scoreTarget;
        *work->scoreTarget = 0;
        HLib::SetCurrentTaskTo(DartsShotEnd);
    }
}
void DartsShotEnd(void *data) {
    DartsEventWork *const work = static_cast<DartsEventWork *>(data);
    SetScenarioCharacterTransparency(DART_PLAYER, 1);
    switch (work->gameState) {
    case 0:
        work->gameState = 1;
        CheckHelpIcon(1, 1, CheckHelpIconOp::Remove);
        nullsub();
        work->opponentScore.finalScore = work->standardBoard ? work->opponentScore.score / 10000 : work->opponentScore.score >> 1;
        return;
    case 1: {
        const std::int32_t score = work->opponentScore.finalScore;
        std::int32_t response = 5;
        if (score)
            response = score <= 10 ? 6 : score <= 15 ? 7 : score <= 20 ? 8 : score <= 30 ? 9 : score <= 45 ? 10 : score == 50 ? 11 : 12;
        work->gameState = 2;
        ConversationBuffer *const voice = DartsResponse(work, response);
        if (voice)
            DartsQueueVoiceWait(work->task, voice);
        else
            HLib::SetupTaskDelay(30);
        return;
    }
    case 2:
        work->opponentScore.finalScore = work->opponentScore.score;
        work->gameState = 3;
        HLib::SetupTaskDelay(24);
        return;
    case 3:
        work->gameState = 4;
        DartsBeginScoreTransfer(work, &work->opponentScore.finalScore);
        return;
    case 4:
        work->gameState = 5;
        HLib::SetupTaskDelay(36);
        return;
    case 5:
        work->gameState = 6;
        if (work->scoreTicker > 0 && work->standardBoard) {
            work->field0e |= 0x60U;
            work->scoreAnimationActive = 1;
            DartsBeginScoreTransfer(work, &work->scoreTicker);
            return;
        }
        [[fallthrough]];
    case 6:
        ++work->gameState;
        if (work->scoreAnimationActive) {
            work->scoreAnimationActive = 0;
            HLib::SetupTaskDelay(30);
            work->field0e &= 0x9fU;
            return;
        }
        [[fallthrough]];
    case 7:
        ++work->gameState;
        if (work->entry->opponentMode == 2 && work->enemyTurn)
            EVT_AT_SetMot(work->entry->enemy->name, 61442, 0, 0, 0x80000000U, 0);
        [[fallthrough]];
    case 8: {
        ++work->gameState;
        CheckHelpIcon(1, 0, CheckHelpIconOp::Add);
        FinishLoading();
        if (work->standardBoard && (!work->state || (work->state == 1 && work->scoreStep && work->entry->opponentMode == 2))) {
            ++work->throws;
            if (work->throws > 2) {
                if (work->entry->opponentMode == 2) {
                    ConversationBuffer *const voice = DartsResponse(work, 15);
                    if (voice)
                        DartsQueueVoiceWait(work->task, voice);
                    else
                        HLib::SetupTaskDelay(30);
                    return;
                }
            } else {
                if (work->opponentScore.accumulated / 10000 >= 120 * work->throws) {
                    DartsNextGame(work);
                    HLib::SetCurrentTaskTo(DartsNextGame);
                    return;
                }
                if (work->entry->opponentMode == 2) {
                    ConversationBuffer *const voice = DartsResponse(work, 29);
                    if (voice)
                        DartsQueueVoiceWait(work->task, voice);
                    else
                        HLib::SetupTaskDelay(30);
                }
                return;
            }
        }
        [[fallthrough]];
    }
    case 9:
        work->gameState = 0;
        work->pendingReset = 0;
        if (!work->state || (!work->playerWon && work->state == 1 && work->scoreStep && work->entry->opponentMode == 2))
            HLib::SetCurrentTaskTo(DartsShotEndBlink);
        else {
            if (work->entry->opponentMode == 2)
                DartsSwitchScoringTurn(work);
            if (work->state == 5 && work->actorControl[0])
                HLib::SetCurrentTaskTo(DartsRyoShot);
            else
                HLib::SetCurrentTaskTo(work->entry->opponentMode == 2 && work->enemyTurn ? DartsEnemyPlaying : DartsPlaying);
        }
        return;
    default:
        return;
    }
}
void DartsShotEndBlink(void *data) {
    DartsEventWork *const work = static_cast<DartsEventWork *>(data);
    const std::int8_t reset = static_cast<std::int8_t>(work->pendingReset);
    if (reset >= 30)
        HLib::SetCurrentTaskTo(DartsShotEndRecord);
    else {
        work->field0e = reset % 4 < 2 ? 14 : 15;
        work->pendingReset = static_cast<std::uint8_t>(reset + 1);
    }
}
void DartsShotEndRecord(void *data) {
    DartsEventWork *const work = static_cast<DartsEventWork *>(data);
    const std::int32_t score = work->opponentScore.accumulated;
    work->field0e = 15;
    work->previousScore = score;
    if (work->bestScore >= score) {
        HLib::SetCurrentTaskTo(DartsPlayEnd);
        return;
    }
    work->bestScore = score;
    const bool record = work->standardBoard ? score > 777777 : score > 50;
    if (work->entry->opponentMode == 2 || !record) {
        HLib::SetCurrentTaskTo(DartsPlayEnd);
        HLib::SetupTaskDelay(15);
    } else {
        Loop3DSoundEffect(0x185);
        DartsResponse(work, 13);
        HLib::SetCurrentTaskTo(DartsPlayEnd);
        HLib::SetupTaskDelay(105);
    }
}

void DartsAdmissionScore(void *data) {
    DartsAdmissionScoreWork *const work = static_cast<DartsAdmissionScoreWork *>(data);
    if (!work) {
        UnregisterEventTask(HLib::GetCurrentTask());
        HLib::DestroyCurrentTask();
        return;
    }
    const std::int32_t previousFrames = work->frames--;
    if (previousFrames > 0)
        return;
    if (work->remaining >= 2) {
        work->scores[3] += 20000;
        work->remaining -= 20000;
        if (!work->soundStarted) {
            work->soundStarted = 1;
            Loop3DSoundEffect(0x184);
            Loop3DSoundEffect(0x183);
        }
    } else {
        work->scores[3] += work->remaining;
        Loop3DSoundEffect(0x184);
        UnregisterEventTask(HLib::GetCurrentTask());
        HLib::DestroyCurrentTask();
    }
    scenCopyCharaToGame(work->board);
    for (std::int32_t row = 0; row < 4; ++row)
        DartsUpdateMachineScore(work->board, work->scores[row], row);
}
void DartsQueueAdmissionScore(DartsEventWork *root) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(DartsAdmissionScore, 4, 11, sizeof(DartsAdmissionScoreWork), DART_TASK);
    DartsAdmissionScoreWork *const work = static_cast<DartsAdmissionScoreWork *>(HLib::GetTaskParameter(task));
    if (!work)
        return;
    RegisterEventTask(task);
    work->soundStarted = 0;
    work->board = root->entry->placements[root->entry->placementIndex].character;
    work->scores = {root->entry->savedScores[0], root->entry->savedScores[1], 0, root->entry->savedScores[1] - root->entry->reserved0c};
    work->remaining = root->entry->reserved0c;
    work->frames = 55;
    scenCopyCharaToGame(work->board);
    for (std::int32_t row = 0; row < 4; ++row)
        DartsUpdateMachineScore(work->board, work->scores[row], row);
}
std::int32_t DartsAdmissionIntroduction(DartsEventWork *work) {
    const shen_name opponent = work->entry->enemy->name;
    const std::size_t index = DartsOpponentIndex(opponent);
    const shen_name board = work->entry->placements[work->entry->placementIndex].character;
    const Vector3 playerPosition{.x = 1, .y = 0, .z = 4};
    const Vector3i playerRotation{.x = 0, .y = 0x4000, .z = 0};
    SetMotionRegionFlag(scenFindMotionwork(DART_PLAYER), 71, 0, MotionRegion::LowerBody);
    QueueDartsPosition(DART_PLAYER, board, &playerPosition, &playerRotation, 0);
    const Vector3 opponentPosition{.x = 0, .y = 0, .z = 4.1999998f};
    const Vector3i opponentRotation{};
    SetMotionRegionFlag(scenFindMotionwork(opponent), 71, 0, MotionRegion::LowerBody);
    QueueDartsPosition(opponent, board, &opponentPosition, &opponentRotation, 0);
    EVT_AT_SetMot(DART_PLAYER, 61442, -1, 1, 0x80000000U, 0);
    EVT_AT_SetMot(opponent, DART_OPPONENT_WORDS[index][9], -1, 1, 0x80000000U, 0);
    PrepareDartsOpponentAdmission(opponent);
    EVT_AT_SetCam(static_cast<std::int32_t>(index) + 7415, DART_DUMMY1, DART_DUMMY1, 0, 0, 0);
    return static_cast<std::int32_t>(DART_OPPONENT_WORDS[index][12]);
}
std::int32_t DartsAdmissionDemonstration(DartsEventWork *work) {
    const shen_name opponent = work->entry->enemy->name;
    dartsTemporaryCharacter = opponent;
    DartsQueueAdmissionScore(work);
    const shen_name board = work->entry->placements[work->entry->placementIndex].character;
    DartsQueueProjectile(
        board, DART_ADMISSION_ARROW, work->entry->reserved0c, nullptr, opponent.name == DART_FCJ.name ? 26 : 13, opponent.name == DART_FCJ.name);
    EVT_AT_SetCam(7311, board, board, 0, 0, 10);
    EVT_AT_SetCam(7312, board, board, 0, 0, 70);
    return 215;
}
std::int32_t DartsAdmissionOffer(DartsEventWork *work) {
    const shen_name opponent = work->entry->enemy->name;
    const std::size_t index = DartsOpponentIndex(opponent);
    const std::int32_t frames = PlayDartsOpponentAdmission(opponent);
    const shen_name board = work->entry->placements[work->entry->placementIndex].character;
    std::int16_t flag;
    if (opponent.name == DART_FCJ.name) {
        work->entry->bet = 10;
        flag = 478;
    } else if (opponent.name == DART_HLN.name) {
        work->entry->bet = 5;
        flag = 477;
    } else if (opponent.name == DART_JON.name) {
        work->entry->bet = 10;
        flag = 476;
    } else if (opponent.name == DART_HCY.name) {
        work->entry->bet = 50;
        flag = 479;
    } else {
        work->entry->bet = 30;
        flag = 475;
    }
    EVT_AT_SetCam(opponent.name == DART_FCJ.name || opponent.name == DART_HLN.name ? 7290 : 7310, board, board, 0, 0, 0);
    if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, flag))
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, flag, 1);
    EV_DisableCharacterDisplay(DART_ADMISSION_ARROW);
    const Vector3 position{.x = 0, .y = 0, .z = 4};
    const Vector3i rotation{};
    SetMotionRegionFlag(scenFindMotionwork(opponent), 71, 0, MotionRegion::LowerBody);
    QueueDartsPosition(opponent, board, &position, &rotation, 0);
    EVT_AT_SetMot(opponent, DART_OPPONENT_WORDS[index][11], -1, 1, 0x80000000U, 0);
    return frames;
}
// @verified - p
std::int32_t DartsCheckBet(DartsEventWork *work) {
    if (static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) >= work->entry->bet)
        return 0;
    const shen_name opponent = work->entry->enemy->name;
    const std::int32_t frames = PlayDartsOpponentResult(opponent, work->entry->bet);
    EVT_AT_SetMot(DART_PLAYER, 61442, -1, 1, 0x80000000U, 0);
    EVT_AT_SetCam(7319, DART_PLAYER, DART_PLAYER, 0, 0, 0);
    const shen_name board = work->entry->placements[work->entry->placementIndex].character;
    EVT_AT_SetCam(7317, board, board, 0, 0, 80);
    if (opponent.name == DART_FCJ.name) {
        if (work->entry->bet == 5)
            work->entry->bet = 1;
        else if (work->entry->bet == 10)
            work->entry->bet = 5;
    }
    return frames;
}
std::int32_t DartsAcceptBet(DartsEventWork *work) {
    const shen_name opponent = work->entry->enemy->name;
    const std::int32_t delay = opponent.name == DART_FCJ.name || opponent.name == DART_JON.name ? 50 : opponent.name == DART_HCY.name ? 100 : 40;
    const std::int32_t frames = PlayDartsBetResponse(opponent, work->difficulty, work->entry->bet);
    const std::size_t index = DartsOpponentIndex(opponent);
    const shen_name board = work->entry->placements[work->entry->placementIndex].character;
    const Vector3 position{.x = 0, .y = 0, .z = 4};
    const Vector3i rotation{};
    SetMotionRegionFlag(scenFindMotionwork(opponent), 71, 0, MotionRegion::LowerBody);
    QueueDartsPosition(opponent, board, &position, &rotation, 0);
    EVT_AT_SetMot(opponent, DART_OPPONENT_WORDS[index][7], -1, 1, 0x80000000U, 0);
    EVT_AT_SetCam(static_cast<std::int32_t>(index) + 7550, board, board, 0, 0, delay);
    EVT_AT_ObjectDisp_1(opponent, delay, 0);
    EVT_AT_ObjectDisp_1(DART_PLAYER, delay, 1);
    if (frames == -10)
        EV_EnableCharacterDisplay(DART_PLAYER);
    else
        EVT_AT_ObjectDisp_1(DART_PLAYER, frames + 10, 0);
    return frames;
}
std::int32_t DartsDeclineBet(DartsEventWork *work) {
    const shen_name opponent = work->entry->enemy->name;
    const std::uint32_t animation = DART_OPPONENT_WORDS[DartsOpponentIndex(opponent)][4];
    EVT_AT_SetMot(opponent, animation, -1, 1, 0x80000000U, 0);
    std::int32_t frames = static_cast<std::int32_t>(GetMotionFrameCount(static_cast<std::uint16_t>(animation)) + 1);
    if (frames < 120)
        frames = 120;
    DartsResponse(work, 20);
    const shen_name board = work->entry->placements[work->entry->placementIndex].character;
    EVT_AT_SetCam(7317, board, board, 0, 0, 80);
    EVT_AT_ObjectDisp_1(opponent, 80, 0);
    work->gameState = 0;
    return frames;
}
void DartsOpponentAdmission(void *data) {
    DartsEventWork *const work = static_cast<DartsEventWork *>(data);
    switch (work->gameState) {
    case 0:
        work->gameState = 100;
        work->resetFrame = DartsAdmissionIntroduction(work);
        break;
    case 1:
        work->gameState = 101;
        work->resetFrame = DartsAdmissionDemonstration(work);
        break;
    case 2:
        work->gameState = 102;
        work->resetFrame = DartsAdmissionOffer(work);
        break;
    case 3: {
        const std::int32_t frames = DartsCheckBet(work);
        if (frames) {
            EV_DisableCharacterDisplay(work->entry->enemy->name);
            EVT_AT_ObjectDisp_1(work->entry->enemy->name, 80, 0);
            if (frames > 0) {
                work->gameState = 0;
                HLib::SetCurrentTaskTo(DartsExit);
                HLib::SetupTaskDelay(frames);
            } else
                HLib::SetupTaskDelay(-frames);
        } else {
            ++work->gameState;
            EVT_AT_KsIcon_Enqueue(-1, 36, 23, -1, 1800, 23, 0, 0);
            EVT_AT_SetCam(7319, DART_PLAYER, DART_PLAYER, 0, 0, 0);
            EV_DisableCharacterDisplay(work->entry->enemy->name);
        }
        break;
    }
    case 4:
        if (GetShouldRenderIcons(RenderIconSlot::ActionSelector)) {
            if (SetMotionRegionFlag(scenFindMotionwork(work->entry->enemy->name), 65, 1, MotionRegion::LowerBody))
                EVT_AT_SetMot(work->entry->enemy->name, DART_OPPONENT_WORDS[DartsOpponentIndex(work->entry->enemy->name)][10], -1, 1, 0x80000000U, 0);
        } else {
            work->gameState = 0;
            const bool accepted = shenGetCtrlSpeakIconFlag() == 36;
            const std::int32_t frames = accepted ? DartsAcceptBet(work) : DartsDeclineBet(work);
            HLib::SetCurrentTaskTo(accepted ? DartsInitStart : DartsExit);
            HLib::SetupTaskDelay(frames);
        }
        break;
    case 100:
        if (HasPressedCancel()) {
            EV_CleanupMapControlTask();
            dartsTemporaryCharacter = work->entry->enemy->name;
            work->gameState = 2;
        } else {
            const std::int32_t frames = static_cast<std::int32_t>(work->resetFrame);
            --work->resetFrame;
            if (frames <= 0)
                work->gameState = 1;
        }
        break;
    case 101:
    case 102: {
        if (HasPressedCancel()) {
            EV_CleanupMapControlTask();
            work->resetFrame = 0;
        }
        const std::int32_t frames = static_cast<std::int32_t>(work->resetFrame);
        --work->resetFrame;
        if (frames <= 0) {
            if (work->gameState == 101)
                Loop3DSoundEffect(0x184);
            work->gameState = work->gameState == 101 ? 2 : 3;
        }
        break;
    }
    default:
        break;
    }
}

void DartsNextGameGlyph(std::uint8_t code, std::int32_t index, shen_name board) {
    constexpr std::array<std::uint16_t, 8> glyphs{0, 331, 295, 335, 298, 292, 330, 296};
    const std::uint16_t glyph = code < glyphs.size() ? glyphs[code] : 0;
    const Vector3 position{
        .x = static_cast<float>(index - (index >= 4 ? 7 : 3)) * 0.050000001f + 0.022399999f, .y = index >= 4 ? 2.0050001f : 2.155f, .z = -0.039999999f};
    if (glyph)
        scenGetCharaGameNode(board, glyph, &position, 1, nullptr);
}
void DartsNextGame(void *data) {
    DartsEventWork *const work = static_cast<DartsEventWork *>(data);
    const std::uint32_t nextRound = static_cast<std::uint32_t>(work->throws) + 1U;
    if (!work->nextState) {
        work->nextState = 1;
        work->field0e = 0;
        work->impactPosition = {.x = -0.12760001f, .y = 2.155f, .z = -0.039999999f};
        const std::array<std::uint8_t, 8> text{0, static_cast<std::uint8_t>(nextRound == 2 ? 1 : 3), 2, 0, 4, 5, 6, 7};
        std::memcpy(&work->cursorX, text.data(), text.size());
        Loop3DSoundEffect(0x185);
        if (nextRound == 2)
            DartsResponse(work, 14);
        else if (nextRound == 3)
            DartsResponse(work, 16);
        work->scoreTickInterval = 0;
    } else if (work->nextState != 1)
        return;
    if (work->scoreTickInterval >= 220) {
        work->field0e = 15;
        if (work->entry->opponentMode == 2 && work->scoreStep) {
            work->playerWon = 1;
            work->nextState = 0;
            HLib::SetCurrentTaskTo(DartsShotEnd);
            return;
        }
        if (work->entry->opponentMode != 2)
            work->playerWon = 1;
        if (work->playerWon)
            work->state = 5;
        work->playerWon = 0;
        Vector3 hidden{.x = 0, .y = 0, .z = -10.05f};
        Vector3i rotation{};
        for (std::int32_t index = 0; index < 5; ++index) {
            EV_DisableCharacterDisplay(DartsArrowName(index, false));
            work->usedBands[index] = 0;
            EV_scenApplyPosOrRotToPartOf(
                DartsSelectBoard(work, DART_PLAYER), static_cast<std::uint16_t>(index + 153), &hidden, &rotation, EventCharacterPartTransformMode::Absolute);
            if (work->entry->opponentMode == 2) {
                EV_DisableCharacterDisplay(DartsArrowName(index, true));
                EV_scenApplyPosOrRotToPartOf(DartsSelectBoard(work, work->entry->enemy->name), static_cast<std::uint16_t>(index + 153), &hidden, &rotation,
                    EventCharacterPartTransformMode::Absolute);
            }
        }
        work->nextState = 0;
        HLib::SetCurrentTaskTo(DartsShotEnd);
        return;
    }
    const Vector3 step{.x = -0.050000001f, .y = 0, .z = 0};
    scenCopyCharaToGame(DartsSelectBoard(work, {}));
    SetPosRotForGame(DartsSelectBoard(work, {}), 256, &step, 9, 0);
    UpdateGameNodeFlags(DartsSelectBoard(work, {}), nextRound, &work->impactPosition, 1, nullptr);
    std::array<std::uint8_t, 8> text;
    std::memcpy(text.data(), &work->cursorX, text.size());
    for (std::int32_t index = 0; index < 8; ++index)
        if (work->scoreTickInterval >= index * 7)
            DartsNextGameGlyph(text[index], index, DartsSelectBoard(work, {}));
    const std::int32_t ticker = work->scoreTicker;
    const std::int32_t tickerRemainder = work->field0e & 0x40U ? static_cast<std::int32_t>(shenGetRandomInt32(10000)) : ticker % 10000;
    DartsUpdateMachineScore(DartsSelectBoard(work, {}), 10000 * (ticker / 10000) + tickerRemainder, 2);
    if (work->scoreTickInterval % 4 < 2) {
        const std::int32_t score = work->opponentScore.accumulated;
        const std::int32_t remainder = work->field0e & 0x20U ? static_cast<std::int32_t>(shenGetRandomInt32(10000)) : score % 10000;
        DartsUpdateMachineScore(DartsSelectBoard(work, {}), 10000 * (score / 10000) + remainder, 3);
    }
    ++work->scoreTickInterval;
}
void DartsSaveScores(DartsEventWork *work) {
    if (work->standardBoard && work->scoreStep) {
        DartsSwitchScoringTurn(work);
        work->previousScore = work->opponentScore.accumulated;
        if (work->bestScore < work->opponentScore.accumulated)
            work->bestScore = work->opponentScore.accumulated;
    }
    if (work->enemyTurn) {
        std::memcpy(&work->alternateBestScore, &work->bestScore, 16);
        work->playerScore = work->opponentScore;
        work->playerBullseyes = work->opponentBullseyes;
    } else {
        std::memcpy(&work->fieldE8, &work->bestScore, 16);
        std::memcpy(&work->fieldF8, &work->opponentScore, 16);
        work->bullseyes = work->opponentBullseyes;
    }
    if (!work->standardBoard) {
        SetSavedMinigameScore(SavedScoreType::Darts_BestScore1, work->bestScore);
        SetSavedMinigameScore(SavedScoreType::Darts_BestScore2, work->previousScore);
        return;
    }
    SetSavedMinigameScore(SavedScoreType::Darts_BestScore4, work->fieldE8);
    SetSavedMinigameScore(SavedScoreType::Darts_BestScore3, work->fieldEc);
    if (work->entry->opponentMode == 2) {
        const shen_name opponent = work->entry->enemy->name;
        const std::size_t index = DartsOpponentIndex(opponent);
        // The executable tests the FourCC rather than the found row index. Keep that genuine low-name branch.
        constexpr std::array<std::uint32_t, 18> scoreWords{SHEN_NAME(FCJ_), 16, 17, SHEN_NAME(HCY_), 18, 19, SHEN_NAME(HLN_), 20, 21, SHEN_NAME(JON_), 22, 23,
            SHEN_NAME(XHO_), 24, 25, 0, 1041194025, 3178611343};
        if (opponent.name < 5) {
            SetSavedMinigameScore(static_cast<SavedScoreType>(scoreWords[index * 3 + 1]), work->alternateBestScore);
            SetSavedMinigameScore(static_cast<SavedScoreType>(scoreWords[index * 3 + 2]), work->alternatePreviousScore);
        }
    }
}
void DartsRetireChildren(DartsEventWork *work) {
    for (HLib::Task *&task : work->childTasks) {
        if (task) {
            HLib::DestroyTaskInternal(task);
            task = nullptr;
        }
    }
    if (work->voiceTask) {
        HLib::DestroyTaskInternal(work->voiceTask);
        work->voiceTask = nullptr;
    }
}
void DartsPlayEnd(void *data) {
    DartsEventWork *const work = static_cast<DartsEventWork *>(data);
    if (work->state) {
        DartsSwitchScoringTurn(work);
        HLib::SetCurrentTaskTo(work->entry->opponentMode == 2 && work->enemyTurn ? DartsEnemyPlaying : DartsPlaying);
        return;
    }
    SetScenarioCharacterTransparency(DART_PLAYER, 1);
    EV_mtwkSetEnabled(scenFindMotionwork(DART_PLAYER), 1);
    EVT_AT_SetMot(DART_PLAYER, 61442, -1, 1, 0x80000000U, 0);
    const Vector3 position{.x = -0.015f, .y = -0.0020000001f, .z = 1.8f};
    const Vector3i rotation{};
    SetMotionRegionFlag(scenFindMotionwork(DART_PLAYER), 71, 0, MotionRegion::LowerBody);
    QueueDartsPosition(DART_PLAYER, DartsSelectBoard(work, {}), &position, &rotation, 0);
    DartsRetireChildren(work);
    work->previousTime.minute = static_cast<std::uint8_t>(work->previousTime.minute + 15);
    SetCurrentDateTimeTo(work->previousTime);
    DartsSaveScores(work);
    if (work->entry->opponentMode == 2) {
        const shen_name opponent = work->entry->enemy->name;
        SetScenarioCharacterTransparency(opponent, 1);
        EV_mtwkSetEnabled(scenFindMotionwork(opponent), 1);
        EVT_AT_SetMot(opponent, 61442, -1, 1, 0x80000000U, 0);
        HLib::AssignCallbackForTask(work->task, DartsExitBattle);
    } else
        HLib::AssignCallbackForTask(work->task, DartsContinuePrompt);
    EV_DisableCharacterDisplay(DartsSelectBoard(work, {}));
}
// @verified - p
std::int32_t DartsBattleResult(DartsEventWork *work) {
    const bool single = EV_GetShenmueFlag(DART_SINGLE_FLAG) != 0;
    const std::int32_t player = single ? work->field104 : work->field104 / 10000;
    const std::int32_t enemy = single ? work->playerScore.accumulated : work->playerScore.accumulated / 10000;
    if (work->entry->enemy->name.name == DART_XHO.name) {
        if (work->bullseyes > work->playerBullseyes)
            return 2;
        return work->bullseyes == work->playerBullseyes;
    }
    if (player > enemy)
        return 2;
    return player == enemy;
}
// @verified - p
void DartsExitBattle(void *data) {
    DartsEventWork *const work = static_cast<DartsEventWork *>(data);
    const shen_name opponent = work->entry->enemy->name;
    EV_EnableCharacterDisplay(opponent);
    EV_SetCharacterAlphaEnabled(opponent, false);
    const std::size_t index = DartsOpponentIndex(opponent);
    const std::array<std::uint32_t, 32> &row = DART_OPPONENT_WORDS[index];
    const std::int32_t result = DartsBattleResult(work);
    if (EV_GetShenmueFlag(DART_SINGLE_FLAG) && g_currentMap.name == DART_AKS1.name) {
        EV_stepSetFlags(result != 2, DART_SINGLE_FLAG);
        HLib::SetCurrentTaskTo(DartsExit);
        return;
    }
    std::uint32_t animation;
    float firstFrame, playerX, playerZ;
    std::int32_t delay, duration;

    EventSetMotion motion;
    const shen_name board = work->entry->placements[work->entry->placementIndex].character;
    if (result == 2) {
        animation = row[23];
        firstFrame = std::bit_cast<float>(row[25]);
        playerX = std::bit_cast<float>(row[26]);
        playerZ = std::bit_cast<float>(row[27]);
        const float total = static_cast<float>(static_cast<std::int32_t>(row[24]));
        delay = static_cast<std::int32_t>(200.0f > total - firstFrame ? 0.0f : total - 200.0f - firstFrame);
        motion.cleanup = 0;
        motion.character = DART_PLAYER;
        motion.animation = 6851;
        motion.frame = -1;
        motion.speed = 0.2f;
        motion.flags = 0x40000000U;
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(6851);
        EVT_AT_SetMot(DART_PLAYER, 6851, 58, 1, 0x40000000U, delay);
        DartsResponse(work, 21);
        PATI_StartMoneyCount(static_cast<std::uint32_t>(work->entry->bet), 330);
        if (opponent.name == DART_JON.name)
            ++work->difficulty;
        EVT_AT_SetCam(7222, DART_DUMMY1, DART_DUMMY1, 0, 0, 1);
    } else if (!result) {
        bool secondMotion;
        if (opponent.name == DART_JON.name)
            secondMotion = work->difficulty >= 2;
        else
            secondMotion = shenGetRandomInt32(2) != 0;
        animation = row[secondMotion ? 16 : 15];
        firstFrame = std::bit_cast<float>(row[secondMotion ? 20 : 19]);
        playerX = std::bit_cast<float>(row[21]);
        playerZ = std::bit_cast<float>(row[22]);
        const float total = static_cast<float>(static_cast<std::int32_t>(row[secondMotion ? 18 : 17]));
        delay = static_cast<std::int32_t>(150.0f > total - firstFrame ? 0.0f : total - 150.0f - firstFrame);
        motion.cleanup = 0;
        motion.character = DART_PLAYER;
        motion.animation = 6343;
        motion.frame = -1;
        motion.speed = 0.2f;
        motion.flags = 0x40000000U;
        EVT_AT_SetMot_Callback(&motion);
        GetMotionFrameCount(6343);
        EVT_AT_SetMot(DART_PLAYER, 6343, -1, 1, 0x40000000U, delay);
        DartsResponse(work, 23);
        PATI_StartMoneyCount(0U - static_cast<std::uint32_t>(work->entry->bet), 330);
        EVT_AT_SetCam(7229, DART_DUMMY1, DART_DUMMY1, 0, 0, 1);
    } else {
        playerX = 1;
        playerZ = 4.4000001f;
        animation = row[28];
        firstFrame = 0;
        delay = 0;
        DartsResponse(work, 22);
        EVT_AT_SetCam(7228, DART_DUMMY1, DART_DUMMY1, 0, 0, 1);
    }
    if (result == 1) {
        EV_DisableCharacterDisplay(DART_PLAYER);
        EVT_AT_SetMot(opponent, animation, firstFrame, 1, 0x40000000U, 0);
        duration = 160;
    } else {
        DartsCamera(work, static_cast<std::int32_t>(index) + 7523, board, board, delay);
        EV_DisableCharacterDisplay(DART_PLAYER);
        EVT_AT_ObjectDisp_1(DART_PLAYER, delay, 0);
        duration = EVT_AT_SetMot(opponent, animation, firstFrame, 1, 0x40000000U, 0);
        if (duration < delay)
            duration = delay;
    }
    EVT_AT_SetMot(opponent, row[10], -1, 1, 0x80000000U, duration);
    EVT_AT_ObjectDisp_1(DART_PLAYER, duration, 0);
    Vector3 position{.x = playerX, .y = 0, .z = playerZ};
    Vector3i rotation{.x = 0, .y = 0x4000, .z = 0};
    SetMotionRegionFlag(scenFindMotionwork(DART_PLAYER), 71, 0, MotionRegion::LowerBody);
    QueueDartsPosition(DART_PLAYER, board, &position, &rotation, 0);
    position = {.x = 0, .y = 0, .z = 4};
    rotation.y = 60074;
    SetMotionRegionFlag(scenFindMotionwork(opponent), 71, 0, MotionRegion::LowerBody);
    QueueDartsPosition(opponent, board, &position, &rotation, 0);
    position = {.x = 1.2f, .y = 0, .z = 4.1999998f};
    rotation.y = 0x4000;
    SetMotionRegionFlag(scenFindMotionwork(DART_PLAYER), 71, 0, MotionRegion::LowerBody);
    QueueDartsPosition(DART_PLAYER, board, &position, &rotation, duration);
    position = {.x = 0, .y = 0, .z = 4};
    rotation.y = 60074;
    SetMotionRegionFlag(scenFindMotionwork(opponent), 71, 0, MotionRegion::LowerBody);
    QueueDartsPosition(opponent, board, &position, &rotation, duration);
    DartsCamera(work, static_cast<std::int32_t>(index) + 7540, board, board, duration);
    HLib::SetCurrentTaskTo(DartsContinuePrompt);
    HLib::SetupTaskDelay(duration);
}
void DartsContinuePrompt(void *data) {
    DartsEventWork *const work = static_cast<DartsEventWork *>(data);
    const bool reward = !work->entry->mode && work->field104 >= 2500000 && !EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014);
    if (!work->gameState) {
        work->gameState = 1;
        ConversationBuffer *voice;
        if (work->entry->opponentMode != 2) {
            EVT_AT_SetCam(7205, DART_PLAYER, DART_PLAYER, 0, 0, 0);
            voice = DartsResponse(work, 17);
        } else {
            const std::int32_t result = DartsBattleResult(work);
            voice = DartsResponse(work, result == 2 ? 24 : result == 1 ? 25 : 26);
            EV_EnableCharacterDisplay(DART_PLAYER);
        }
        if (voice)
            DartsQueueVoiceWait(work->task, voice);
        else
            HLib::SetupTaskDelay(200);
        return;
    }
    if (work->gameState == 1) {
        if (work->entry->opponentMode == 2) {
            const std::int32_t frames = DartsCheckBet(work);
            if (frames) {
                EV_DisableCharacterDisplay(work->entry->enemy->name);
                EVT_AT_ObjectDisp_1(work->entry->enemy->name, 80, 0);
                if (frames > 0) {
                    work->gameState = 0;
                    HLib::SetCurrentTaskTo(DartsExit);
                    HLib::SetupTaskDelay(frames);
                } else
                    HLib::SetupTaskDelay(-frames);
                return;
            }
            ++work->gameState;
            EVT_AT_SetCam(7319, DART_PLAYER, DART_PLAYER, 0, 0, 0);
            EVT_AT_SetMot(DART_PLAYER, 61442, -1, 1, 0x80000000U, 0);
            if (shenGetMinutesUntilNextFrameMinute() <= 0)
                return;
            if (work->difficulty) {
                work->gameState = 0;
                HLib::SetCurrentTaskTo(DartsDoubleUp);
                return;
            }
        } else {
            if (static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) < 5) {
                work->gameState = 0;
                ConversationBuffer *const voice = DartsResponse(work, 2);
                HLib::SetCurrentTaskTo(DartsExit);
                HLib::SetupTaskDelay(30);
                if (voice)
                    DartsQueueVoiceWait(work->task, voice);
                return;
            }
            ++work->gameState;
            if (shenGetMinutesUntilNextFrameMinute() <= 0 || reward)
                return;
        }
        EVT_AT_KsIcon_Enqueue(-1, 37, 34, -1, 1800, 34, 0, 0);
        return;
    }
    if (work->gameState != 2 || GetShouldRenderIcons(RenderIconSlot::ActionSelector))
        return;
    work->gameState = 0;
    if (shenGetMinutesUntilNextFrameMinute() > 0 && !reward && (work->entry->opponentMode == 1 || shenGetCtrlSpeakIconFlag() == 37)) {
        if (work->entry->opponentMode == 2) {
            DartsContinueOk(work);
            return;
        }
        work->requestIcon = 36;
        ConversationBuffer *const voice = DartsResponse(work, 18);
        HLib::SetCurrentTaskTo(DartsMainOk);
        HLib::SetupTaskDelay(60);
        if (voice)
            DartsQueueVoiceWait(work->task, voice);
    } else if (work->entry->opponentMode != 2) {
        ConversationBuffer *const voice = DartsResponse(work, 19);
        HLib::SetCurrentTaskTo(DartsExit);
        HLib::SetupTaskDelay(60);
        if (voice)
            DartsQueueVoiceWait(work->task, voice);
    } else {
        const std::int32_t frames = PlayDartsOpponentExit(work->entry->enemy->name);
        const shen_name board = work->entry->placements[work->entry->placementIndex].character;
        EVT_AT_SetCam(7317, board, board, 0, 0, 80);
        HLib::SetCurrentTaskTo(DartsExit);
        HLib::SetupTaskDelay(frames);
    }
}
void DartsContinueOk(void *data) {
    DartsEventWork *const work = static_cast<DartsEventWork *>(data);
    DartsInitializeGame(work);
    const std::int32_t frames = DartsAcceptBet(work);
    HLib::SetCurrentTaskTo(DartsEnemyShot);
    HLib::SetupTaskDelay(frames);
}
void DartsDoubleUp(void *data) {
    DartsEventWork *const work = static_cast<DartsEventWork *>(data);
    if (!work->gameState) {
        work->gameState = 1;
        std::int16_t choices[4]{static_cast<std::int16_t>(work->difficulty == 1 ? 212 : 215), static_cast<std::int16_t>(work->difficulty == 1 ? 213 : 216),
            static_cast<std::int16_t>(work->difficulty == 1 ? 214 : 217), 23};
        SetupControlSpeakAndUI(choices, 1800, 23, 2);
        return;
    }
    if (work->gameState != 1 || GetShouldRenderIcons(RenderIconSlot::ActionSelector))
        return;
    work->gameState = 0;
    const std::int32_t choice = shenGetCtrlSpeakIconFlag();
    switch (choice) {
    case 212:
        work->entry->bet = 10;
        break;
    case 213:
        work->entry->bet = 20;
        break;
    case 214:
        work->entry->bet = 30;
        break;
    case 215:
        work->entry->bet = 100;
        break;
    case 216:
        work->entry->bet = 200;
        break;
    case 217:
        work->entry->bet = 300;
        break;
    default:
        work->entry->bet = 0;
        DartsResponse(work, 20);
        break;
    }
    if (choice >= 215 && choice <= 217) {
        work->entry->maximumRadius = 5;
        work->entry->minimumRadius = static_cast<std::int32_t>(static_cast<std::uint32_t>(work->entry->minimumRadius) & 0xffffff00U);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 502, 1);
        EV_LoadAudio("01130000.sbt", 6);
    }
    if (work->entry->bet) {
        if (static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) >= work->entry->bet)
            HLib::SetCurrentTaskTo(DartsContinueOk);
        else {
            ConversationBuffer *const voice = DartsResponse(work, 2);
            if (voice)
                DartsQueueVoiceWait(work->task, voice);
            else
                HLib::SetupTaskDelay(30);
        }
    } else {
        HLib::SetCurrentTaskTo(DartsExit);
        HLib::SetupTaskDelay(120);
    }
}

void DartsRestorePlayer(DartsEventWork *work) {
    hndlSetFlags(ctrlGetCharaByName(DART_PLAYER));
    SetScenarioCharacterFlag40(DART_PLAYER, 1);
    AT_SetLocalBaseChara(DART_PLAYER, EVENT_CHARACTER_INVALID);
    EV_EnableCharacterDisplay(DART_PLAYER);
    UpdateMotionWorkWithPosition(scenFindMotionwork(DART_PLAYER), nullptr, 0);
    EV_SetCharacterAlphaEnabled(DART_PLAYER, true);
    EV_SetCharaControllerFlags(DART_PLAYER, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(DART_PLAYER, ApplyGravity | UpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(DART_PLAYER, UpdateHeight, NpcCharacterControllerState::Enabled);
    EVT_AT_SetMot(DART_PLAYER, 61442, -1, 1, 0x40000000U, 0);
    if (work->entry->opponentMode == 2) {
        EV_AT_SetPos(DART_PLAYER, EventTransformFlags::AllAxes, work->previousPosition);
        EV_AT_SetAngY(DART_PLAYER, EventTransformFlags::AllAxes, work->previousRotation);
    } else {
        const Vector3 position{.x = 0, .y = 0, .z = 1.5f};
        const Vector3i rotation{};
        SetMotionRegionFlag(scenFindMotionwork(DART_PLAYER), 71, 0, MotionRegion::LowerBody);
        QueueDartsPosition(DART_PLAYER, work->entry->placements[work->entry->placementIndex].character, &position, &rotation, 0);
    }
    if (work->gameFlag)
        SetPlayerGameFlags(PlayerGameFlagMode::Bit5, reinterpret_cast<void *>(std::uintptr_t{1}));
}
void DartsRestoreOpponent(DartsEventWork *work) {
    if (work->entry->opponentMode != 2)
        return;
    const shen_name opponent = work->entry->enemy->name;
    SetScenarioCharacterFlag40(opponent, 1);
    AT_SetLocalBaseChara(opponent, EVENT_CHARACTER_INVALID);
    EV_EnableCharacterDisplay(opponent);
    UpdateMotionWorkWithPosition(scenFindMotionwork(opponent), nullptr, 0);
    EV_SetCharacterAlphaEnabled(opponent, true);
    DartsBeginOpponentThrow(opponent, work->difficulty);
    InitiateCleanupForSceneChara(opponent);
    hndlSetFlags(ctrlGetCharaByName(opponent));
    const Vector3 position{.x = g_currentMap.name == DART_AKS0.name && opponent.name == DART_XHO.name ? -0.30000001f : 0, .y = 0, .z = 3.5999999f};
    const Vector3i rotation{};
    SetMotionRegionFlag(scenFindMotionwork(opponent), 71, 0, MotionRegion::LowerBody);
    QueueDartsPosition(opponent, work->entry->placements[work->entry->placementIndex].character, &position, &rotation, 0);
}
void DartsRetireCoinAndArrows(DartsEventWork *work) {
    EventSetPosition placement{.cleanup = 0, .character = DART_COIN, .parent = EVENT_CHARACTER_INVALID, .position = {}, .delay = 0};
    EVT_AT_SetPosRot(&placement);
    EV_ToggleCharacterFlag(DART_COIN);
    EV_DisableCharacterDisplay(DART_COIN);
    ResetCharaFixo(ctrlGetCharaByName(DART_COIN));
    for (std::int32_t index = 0; index < 5; ++index) {
        const shen_name arrow = DartsArrowName(index, false);
        placement.character = arrow;
        EVT_AT_SetPosRot(&placement);
        EV_ToggleCharacterFlag(arrow);
        EV_DisableCharacterDisplay(arrow);
        ResetCharaFixo(ctrlGetCharaByName(arrow));
        if (work->entry->opponentMode == 2) {
            const shen_name enemyArrow = DartsArrowName(index, true);
            placement.character = enemyArrow;
            EVT_AT_SetPosRot(&placement);
            EV_ToggleCharacterFlag(enemyArrow);
            EV_DisableCharacterDisplay(enemyArrow);
            ResetCharaFixo(ctrlGetCharaByName(enemyArrow));
        }
    }
}
void DartsExit(void *data) {
    using enum CameraState;
    DartsEventWork *const work = static_cast<DartsEventWork *>(data);
    if (!work->gameState) {
        work->gameState = 1;
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetupTaskDelay(30);
        return;
    }
    if (work->gameState == 1) {
        work->gameState = 2;
        CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
        DartsRestorePlayer(work);
        DartsRestoreOpponent(work);
        DartsRetireCoinAndArrows(work);
        EV_FreeAuthorPak(0, "0137");
        EV_FreeAuthorPak(0, "0113");
        for (HLib::Task *const task : work->childTasks) {
            if (task)
                HLib::DestroyTaskInternal(task);
        }
        if (work->voiceTask)
            HLib::DestroyTaskInternal(work->voiceTask);
        shenSetTimeSimulationTo(1);
        shenSetClockAndCompassRenderTo(0);
        EVS_LoopCamera(0, {}, {});
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        EVT_AT_SetCam(-1, {}, {}, 0, 0, 0);
        camCtrlDisableFilmBars();
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
            EV_scenQueueLoad(g_actNumber, DART_REWARD_AREA, 1);
        else {
            EV_LoadCutsceneFromCurrentScene(work->previousBgm.data());
            EV_LoadAudio(work->previousSbt.data(), work->previousSoundProgram);
            EV_spmBeginSampleExecute();
        }
        return;
    }
    if (work->gameState == 2) {
        const std::uint8_t plays = static_cast<std::uint8_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 1));
        EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 1, plays == 255 ? 255 : plays + 1);
        if (work->entry->mode || work->field104 < 2500000) {
            if (work->entry->opponentMode != 2)
                SetPlayerControlEnabled(1);
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        } else
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1732, 1);
        EV_NPC_NewLoadSw(1);
        EV_CleanupMapControlTask();
        HLib::EV_HLKillOtherHierarchyTask(work->task);
        HLib::ResolveDeferredCallback(work->entry->parent);
        dartsEventActive = 0;
    } else
        work->gameState = 0;
}
void DartsInitializeRoot(DartsEventWork *work, HLib::Task *task, DartsGameEntry *entry) {
    std::memset(work, 0, sizeof(*work));
    work->task = task;
    work->entry = entry;
    work->gameFlag = static_cast<std::int32_t>(SetPlayerGameFlags(PlayerGameFlagMode::Bit5, reinterpret_cast<void *>(~std::uintptr_t{0})));
    const shen_name board = DartsSelectBoard(work, {});
    work->standardBoard = board.name != DART_COMPACT.name;
    const std::uint8_t topic = !work->standardBoard ? 3 : entry->opponentMode == 2 ? (entry->enemy->name.name == DART_XHO.name ? 48 : 76) : 47;
    EV_EventSetUpHelpIcon_0(topic, 0);
    EV_GetCharaPosition(DART_PLAYER, CHARACTER_TRANSLATION_ROOT, &work->previousPosition, EventTransformFlags::Local);
    GetScenarioCharacterEulerAngles(DART_PLAYER, &work->previousRotation, EventTransformFlags::Local);
    std::memcpy(work->previousBgm.data(), GetCurrentBgm(), 16);
    std::memcpy(work->previousSbt.data(), shenGetCurrentSbt(), 16);
    work->previousSoundProgram = GetSelectedSoundSceneIndex();
    EV_FreeAuthorPak(1, "0113");
    EV_FreeAuthorPak(1, "0137");
    SetPlayerControlEnabled(0);
    CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
    FinishLoading();
    EV_SetRefBoardParamsFor(DART_PLAYER, 1);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("DARTS"), 0);
    DartsInitializeScoreBanks(work);
    DartsPlaceCameraDummies(work);
}
void DartsAreaInit() {
    dartsPlayerWon = 0;
}
void DartsAreaMain(std::uintptr_t argument) {
    DartsGameEntry *const entry = reinterpret_cast<DartsGameEntry *>(argument);
    if (entry->mode < 2) {
        const HLib::TaskCallback callback = entry->mode ? DartsPositionOpponentHands : DartsInitStart;
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(callback, 4, 11, sizeof(DartsEventWork), DART_TASK);
        DartsEventWork *const work = static_cast<DartsEventWork *>(HLib::GetTaskParameter(task));
        if (work) {
            HLib::SetupTimedTask(task, 2);
            DartsInitializeRoot(work, task, entry);
        }
    }
    dartsEventActive = 1;
}
} // namespace
const ScriptAreaDefinition dartScriptDefinition{.areaId = AREA_DART, .field04 = 0, .init = DartsAreaInit, .main = DartsAreaMain};
} // namespace shenmue::s2::areas
