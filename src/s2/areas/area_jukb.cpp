#include "s2/areas/area_jukb.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/camera.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/hlib.hpp"
#include "s2/input.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include <array>
#include <bit>

namespace shenmue::s2::areas {
namespace {
constexpr std::uint32_t musicSelections[8][10]{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {12, 20, 18, 10, 5, 6, 4, 9, 17, 3}, {27, 26, 8, 24, 5, 6, 4, 9, 17, 3},
    {7, 14, 19, 15, 5, 6, 4, 9, 17, 3}, {22, 16, 11, 21, 5, 6, 4, 9, 17, 3}, {2, 1, 0, 13, 5, 6, 4, 9, 17, 3}, {28, 29, 30, 31, 32, 33, 34, 25, 23, 37},
    {35, 36, 38, 39, 32, 33, 34, 25, 23, 37}};
constexpr float moveSteps[8]{.003f, .004f, .006f, .006f, .006f, .004f, .002f, .001f};
std::int32_t handCommand[57]{1456, 2547, 60438, 0, 63716, 62988, 2912, 544, 3639, 725, 182, 906, 0, 65175, 56811, 0, 274, 58064, 0, 182, 182, 0, 0, 56795, 0, 0,
    47878, 0, 0, 50426, 0, 0, 0, 0, 0, 48969, 0, 0, 47331, 0, 0, 51155, 0, 0, 0, 0, 0, 46057, 0, 0, 47150, 0, 64445, 52610, 0, 0, 64990};
const char *RecordBank() {
    return g_currentMap.name == JUKB_AKS1.name ? "0107AKS1.sbt" : g_currentMap.name == JUKB_QTB3.name ? "0107QTB3.sbt" : "01070000.sbt";
}
void RotatePart(std::uint16_t part, Vector3i rotation) {
    EV_scenApplyPosOrRotToPartOf(JUKB_BOX, part, nullptr, &rotation, EventCharacterPartTransformMode::Relative);
}
void ScalePart(std::uint16_t part, float scale) {
    Vector3 value{scale, scale, scale};
    EV_scenApplyPosOrRotToPartOf(JUKB_BOX, part, &value, nullptr, EventCharacterPartTransformMode::Set);
}
void SetTargetPosition(shen_name name, const Vector3 &value) {
    EV_AT_SetPos(name, EventTransformFlags::AllAxes, value);
}
void ReadPosition(shen_name name, Vector3 &position, std::uint32_t flags = 0x40000000U) {
    EV_GetCharaPosition(name, CHARACTER_TRANSLATION_ROOT, &position, flags);
}
void EyeAndMotionTarget(shen_name name) {
    Vector3 position;
    ReadPosition(name, position);
    UpdateMotionWorkWithPosition(scenFindMotionwork(JUKB_PLAYER), &position, 1);
    SetEyeToChara(FindCharacterByScenarioName(JUKB_PLAYER), &position, 0, 2);
}
void QueueSelection(JukbTaskData &work, std::uint32_t input) {
    if (input == 0x8000 && work.selection < 9) {
        work.rightPhase = 1;
        ++work.selection;
    }
    if (input == 0x4000 && work.selection != 0) {
        work.leftPhase = 1;
        --work.selection;
    }
}
} // namespace

// @verified - p
void AreaJUKBInit() {
}
// @verified - p
void AreaJUKBMain(std::uintptr_t parameter) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(JUKBCheckCallback, 5, 0x0b, sizeof(JukbTaskData), JUKB_TASK);
    JukbTaskData *const work = static_cast<JukbTaskData *>(HLib::GetTaskParameter(task));
    work->self = task;
    work->mainState = 0;
    work->dialogue = nullptr;
    work->jukeboxIndex = *reinterpret_cast<const std::uint32_t *>(parameter);
    EV_DisableCharacterDisplay(JUKB_FINGER);
    for (std::int32_t i = 5; i != 9; ++i)
        CreateEventCharacter(CharacterCreationHighModel, nullptr, JUKB_TARGETS[i]);
    const Vector3 fingerScale{.1f, .1f, .1f};
    SetMotionworkFlagForCharaById(JUKB_FINGER, &fingerScale);
    AT_SetLocalBaseChara(JUKB_FINGER, JUKB_BOX);
    for (std::int32_t i = 0; i != 7; ++i)
        AT_SetLocalBaseChara(JUKB_TARGETS[i], JUKB_BOX);
    AT_SetLocalBaseChara(JUKB_TARGETS[7], JUKB_TARGETS[1]);
    AT_SetLocalBaseChara(JUKB_TARGETS[8], JUKB_TARGETS[1]);
    for (std::int32_t i = 0; i != 4; ++i)
        EV_AT_SetAngY(JUKB_TARGETS[i], EventTransformFlags::AllAxes, Vector3i{0, 0, 0});
    EV_AT_SetAngY(JUKB_FINGER, EventTransformFlags::AllAxes, Vector3i{20751, 60622, 32047});
    SetTargetPosition(JUKB_FINGER, {-.36700001f, 1.036f, .78799999f});
    SetTargetPosition(JUKB_TARGETS[0], {-.33520001f, 1.1834f, .31420001f});
    SetTargetPosition(JUKB_TARGETS[1], {-.47099999f, .95999998f, .63f});
    SetTargetPosition(JUKB_TARGETS[2], {-.1753f, .96679997f, .5819f});
    SetTargetPosition(JUKB_TARGETS[3], {-.57770002f, 1.4385999f, 1.0913f});
    SetTargetPosition(JUKB_TARGETS[4], {-.34f, 0, 1.1f});
    SetTargetPosition(JUKB_TARGETS[6], {.026000001f, 1.0190001f, 1.054f});
    SetTargetPosition(JUKB_TARGETS[7], {.1234f, .36590001f, .17739999f});
    EV_SetCharaControllerFlags(JUKB_PLAYER, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(JUKB_PLAYER, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    SetPlayerControlEnabled(0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("JUKB"), 0);
    EV_FreeAuthorPak(1, "0107");
    EV_SetNewSceneIndex();
    camCtrlCacheUtil(2);
    SetAllMapFieldDepthSlotsEnabled(0);
}
// @verified - p
void JUKBCheckCallback(void *data) {
    JukbTaskData &work = *static_cast<JukbTaskData *>(data);
    if (shenGetMinutesUntilNextFrameMinute())
        HLib::SetCurrentTaskTo(JUKBMainCallback);
    else {
        work.dialogue = EV_PlayDialogue(GetRandomNumberInRange(2.0f) == 0 ? "0107A009" : "0107A011");
        HLib::SetCurrentTaskTo(JUKBExitCallback);
    }
}
// @verified - p
void JUKBMainCallback(void *data) {
    JukbTaskData &work = *static_cast<JukbTaskData *>(data);
    switch (work.mainState) {
    case 0:
        EV_EventSetUpHelpIcon_0(7, 0);
        HLib::SetupTaskDelay(8);
        ++work.mainState;
        break;
    case 1: {
        Vector3 position;
        ReadPosition(JUKB_TARGETS[4], position);
        EV_AT_SetPos(JUKB_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
        Vector3i rotation;
        GetScenarioCharacterEulerAngles(JUKB_BOX, &rotation, EventTransformFlags::Local);
        EV_AT_SetAngY(JUKB_PLAYER, EventTransformFlags::AllAxes, rotation);
        EVS_LoopCamera(1200, JUKB_TARGETS[0], JUKB_TARGETS[0]);
        SetReflectorBoardValues(FindCharacterByScenarioName(JUKB_PLAYER), Vector4{1, 1, 1, 1});
        EV_SetRefBoardParamsFor(JUKB_PLAYER, 1);
        EV_SetRefBoardParamsFor(JUKB_PLAYER, 1);
        SetLowQualityHands(FindCharacterByScenarioName(JUKB_PLAYER));
        SetHighModelXYZ(FindCharacterByScenarioName(JUKB_PLAYER), 1, 0, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(JUKB_PLAYER), 1, 1, 0);
        ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(JUKB_PLAYER), 0, reinterpret_cast<std::byte *>(handCommand), 0);
        EventCling cling{0, JUKB_COIN, JUKB_PLAYER, 14, {std::bit_cast<float>(0x3e041893U), std::bit_cast<float>(0x3cdd2f1bU), -.07f}, {20024, 12743, 0}, 0, 0};
        EVT_AT_Cling(&cling);
        EV_EnableCharacterDisplay(JUKB_PLAYER);
        ++work.mainState;
        break;
    }
    case 3: {
        EventFilmControl fade{0, 24, 255, 0, 0, 0, 0, 0, 0, 0, 0};
        EVT_AT_MenCtrl_Callback(&fade);
        Vector3 position;
        ReadPosition(JUKB_TARGETS[4], position);
        EV_AT_SetPos(JUKB_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
        work.dialogue = EV_PlayDialogue(static_cast<std::int32_t>(GetRandomNumberInRange(2.0f)) == 0 ? "0107A017" : "0107A022");
        EyeAndMotionTarget(JUKB_TARGETS[2]);
        break;
    }
    case 70: {
        const std::int32_t choice = static_cast<std::int32_t>(GetRandomNumberInRange(3.0f));
        work.dialogue = EV_PlayDialogue(choice == 0 ? "0107A003" : choice == 1 ? "0107A004" : "0107A005");
        break;
    }
    case 110:
        HLib::SetCurrentTaskTo(JUKBWaitAnswerCallback);
        break;
    default:
        break;
    }
    if (work.mainState > 1)
        ++work.mainState;
}
// @verified - p
void JUKBWaitAnswerCallback(void *) {
    std::int16_t choices[4]{-1, 36, 23, -1};
    SetupControlSpeakAndUI(choices, 200, 0, 2);
    HLib::SetCurrentTaskTo(JUKBWaitAnswerLoopCallback);
}
// @verified - p
void JUKBWaitAnswerLoopCallback(void *data) {
    JukbTaskData &work = *static_cast<JukbTaskData *>(data);
    if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
        return;
    if (shenGetCtrlSpeakIconFlag() == 36) {
        if (GetPlayerStat(PlayerStatsId::Cash) < 5) {
            const std::int32_t choice = static_cast<std::int32_t>(GetRandomNumberInRange(4.0f));
            work.dialogue = EV_PlayDialogue(choice == 0 ? "0107A006" : choice == 1 ? "0107A007" : choice == 2 ? "0107A036" : "0107A008");
            HLib::SetCurrentTaskTo(JUKBExitCallback);
            HLib::SetupTaskDelay(100);
            return;
        }
        HLib::SetCurrentTaskTo(JUKBUseMoneyCallback);
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 470) == 0)
            EV_LoadCutsceneFromCurrentScene(g_currentMap.name == JUKB_AKS1.name ? "0107AKS1.smp" :
                    g_currentMap.name == JUKB_QTB3.name                         ? "0107QTB3.smp" :
                                                                                  "01070000.smp");
        work.frame = 0;
        return;
    }
    const std::int32_t choice = static_cast<std::int32_t>(GetRandomNumberInRange(3.0f));
    work.dialogue = EV_PlayDialogue(choice == 0 ? "0107A009" : choice == 1 ? "0107A010" : "0107A011");
    HLib::SetCurrentTaskTo(JUKBExitCallback);
    HLib::SetupTaskDelay(50);
}
// @verified - p
void JUKBUseMoneyCallback(void *data) {
    JukbTaskData &work = *static_cast<JukbTaskData *>(data);
    switch (work.frame) {
    case 0:
        EVS_LoopCamera(1205, JUKB_TARGETS[3], JUKB_TARGETS[3]);
        mtwkSetRequestedRootMode(scenFindMotionwork(JUKB_PLAYER), 0);
        mtwkRequestMotion(scenFindMotionwork(JUKB_PLAYER), 0x1801, MotionRegion::LowerBody);
        EVT_AT_SetSound(0x5c9, 30);
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 470) == 0)
            ProcessAicaCommand(0xaa0, 30);
        break;
    case 10: {
        UpdateMotionWorkWithPosition(scenFindMotionwork(JUKB_PLAYER), nullptr, 0);
        Vector3 position;
        ReadPosition(JUKB_TARGETS[6], position);
        SetEyeToChara(FindCharacterByScenarioName(JUKB_PLAYER), &position, 0, 2);
        break;
    }
    case 50:
        EyeAndMotionTarget(JUKB_TARGETS[2]);
        (void)scenFindMotionwork(JUKB_PLAYER);
        break;
    case 100: {
        PATI_StartMoneyCount(0xfffffffbU, 10);
        Loop3DSoundEffect(0x163);
        Vector3 position;
        ReadPosition(JUKB_TARGETS[0], position);
        SetEyeToChara(FindCharacterByScenarioName(JUKB_PLAYER), &position, 0, 2);
        ReadPosition(JUKB_TARGETS[0], position);
        UpdateMotionWorkWithPosition(scenFindMotionwork(JUKB_PLAYER), &position, 1);
        break;
    }
    case 180:
        work.dialogue = EV_PlayDialogue(GetRandomNumberInRange(2.0f) == 0 ? "0107A024" : "0107A025");
        break;
    case 200:
        HLib::SetCurrentTaskTo(JUKBSelectInitCallback);
        work.selectFrame = 0;
        break;
    default:
        break;
    }
    ++work.frame;
}
// @verified - p
void JUKBSelectInitCallback(void *data) {
    JukbTaskData &work = *static_cast<JukbTaskData *>(data);
    EV_EnableCharacterDisplay(JUKB_FINGER);
    EV_DisableCharacterDisplay(JUKB_PLAYER);
    EVS_LoopCamera(1210, JUKB_TARGETS[1], JUKB_TARGETS[1]);
    work.selection = 2;
    work.selected = 0;
    work.leftPhase = 0;
    work.rightPhase = 0;
    work.buttonPhase = 0;
    Vector3 fingerPosition, cameraPosition, eyePosition;
    if (g_currentMap.name == JUKB_ARA0.name || g_currentMap.name == JUKB_AKT0.name || g_currentMap.name == JUKB_QTB3.name ||
        g_currentMap.name == JUKB_QCFE.name) {
        fingerPosition.x = -.62300003f;
        cameraPosition.x = -.727f;
        eyePosition = {-.4384f, 1.0973001f, .40220001f};
    }
    if (g_currentMap.name == JUKB_WECF.name || g_currentMap.name == JUKB_QB00.name || g_currentMap.name == JUKB_QCAE.name ||
        g_currentMap.name == JUKB_ARC0.name) {
        fingerPosition.x = -.43100002f;
        cameraPosition.x = -.53499997f;
        eyePosition = {-.3434f, 1.0973001f, .40220001f};
    }
    if (g_currentMap.name == JUKB_AKS1.name || g_currentMap.name == JUKB_WES1.name || g_currentMap.name == JUKB_WEG0.name ||
        g_currentMap.name == JUKB_QA22.name) {
        fingerPosition.x = -.36700001f;
        cameraPosition.x = -.47099999f;
        eyePosition = {-.3414f, 1.0973001f, .29609999f};
    }
    SetTargetPosition(JUKB_TARGETS[5], eyePosition);
    EV_AT_SetAngY(JUKB_TARGETS[5], EventTransformFlags::AllAxes, Vector3i{0, 0, 0});
    fingerPosition.y = 1.036f;
    fingerPosition.z = .78799999f;
    SetTargetPosition(JUKB_FINGER, fingerPosition);
    cameraPosition.y = .95999998f;
    cameraPosition.z = .63f;
    SetTargetPosition(JUKB_TARGETS[1], cameraPosition);
    SetTargetPosition(JUKB_TARGETS[3], {-.57770002f, 1.4385999f, 1.0913f});
    HLib::SetCurrentTaskTo(JUKBSelectLoopCallback);
}
// @verified - p
void JUKBSelectLoopCallback(void *data) {
    using enum CameraState;
    JukbTaskData &work = *static_cast<JukbTaskData *>(data);
    const std::int32_t stickX = GetCurrentControlInput(5), stickY = GetCurrentControlInput(6);
    GetBattleDataConfig(0, JUKB_INPUT);
    ++work.selectFrame;
    if (work.selectFrame == 30) {
        camCtrlSetState(Event);
        SetTargetPosition(JUKB_TARGETS[7], {std::bit_cast<float>(0x3dfcb924U), std::bit_cast<float>(0x3ebb573fU), .17739999f});
        SetTargetPosition(JUKB_TARGETS[8], {0, -.0015f, -.0255f});
        camCtrlSetPerspective(0x1555);
        QueueSelection(work, GetCurrentControlInput(0));
    }
    if (work.selectFrame > 30) {
        Vector3 position;
        ReadPosition(JUKB_TARGETS[7], position, 0);
        const float leftX = static_cast<float>(stickX - 128), leftY = static_cast<float>(stickY - 128) / 1280.0f;
        const float moveX = leftX / 1280.0f + leftY, moveZ = leftX / -1280.0f + leftY;
        position.x = ((position.x - .1234f) + ((moveX - (position.x - .1234f)) / 10.0f)) + .1234f;
        position.z = ((position.z - .17739999f) + ((moveZ - (position.z - .17739999f)) / 10.0f)) + .17739999f;
        SetTargetPosition(JUKB_TARGETS[7], position);
        ReadPosition(JUKB_TARGETS[8], position, 0);
        position.x = ((position.x + 0.0f) + ((moveX - (position.x + 0.0f)) / 10.0f)) + 0.0f;
        position.z = ((position.z + .0255f) + ((moveZ - (position.z + .0255f)) / 10.0f)) - .0255f;
        SetTargetPosition(JUKB_TARGETS[8], position);
        ReadPosition(JUKB_TARGETS[7], position);
        camCtrlSetNextPosition(position);
        ReadPosition(JUKB_TARGETS[8], position);
        camCtrlSetTargetPosition(position);
        if (work.leftPhase == 0 && work.rightPhase == 0 && work.buttonPhase == 0) {
            if (GetBattleDataConfig(1, JUKB_INPUT) && GetBattleDataConfig(2, JUKB_INPUT))
                work.buttonPhase = 1;
            else
                QueueSelection(work, GetCurrentControlInput(7));
        }
    }
    if (work.rightPhase) {
        Vector3 finger, target;
        ReadPosition(JUKB_FINGER, finger, 0);
        ReadPosition(JUKB_TARGETS[1], target, 0);
        const float step = moveSteps[work.rightPhase - 1];
        finger.x += step;
        target.x += step;
        SetTargetPosition(JUKB_FINGER, finger);
        SetTargetPosition(JUKB_TARGETS[1], target);
        if (++work.rightPhase > 8) {
            work.rightPhase = 0;
            if (GetCurrentControlInput(0) == 0x8000)
                QueueSelection(work, 0x8000);
        }
    }
    if (work.leftPhase) {
        Vector3 finger, target;
        ReadPosition(JUKB_FINGER, finger, 0);
        ReadPosition(JUKB_TARGETS[1], target, 0);
        const float step = moveSteps[work.leftPhase - 1];
        finger.x -= step;
        target.x -= step;
        SetTargetPosition(JUKB_FINGER, finger);
        SetTargetPosition(JUKB_TARGETS[1], target);
        if (++work.leftPhase > 8) {
            work.leftPhase = 0;
            if (GetCurrentControlInput(0) == 0x4000)
                QueueSelection(work, 0x4000);
        }
    }
    if (work.buttonPhase) {
        Vector3 finger;
        ReadPosition(JUKB_FINGER, finger, 0);
        if (work.buttonPhase == 1) {
            const std::int32_t choice = static_cast<std::int32_t>(GetRandomNumberInRange(4.0f));
            work.dialogue = EV_PlayDialogue(choice == 0 ? "0107A027" : choice == 1 ? "0107A028" : choice == 2 ? "0107A029" : "0107A030");
        }
        const std::uint16_t part = static_cast<std::uint16_t>(162 + work.selection);
        if (work.buttonPhase == 8) {
            Loop3DSoundEffect(0x15f);
            ScalePart(part, 0);
        }
        if (work.buttonPhase == 11)
            ScalePart(part, 1);
        Vector3 delta{0, work.buttonPhase < 9 ? -.0018f : .0018f, work.buttonPhase < 9 ? -.0014f : .0014f};
        finger.y += delta.y;
        finger.z += work.buttonPhase < 9 ? -.00089999998f : .00089999998f;
        EV_scenApplyPosOrRotToPartOf(JUKB_BOX, part, &delta, nullptr, EventCharacterPartTransformMode::Relative);
        SetTargetPosition(JUKB_FINGER, finger);
        if (++work.buttonPhase > 16) {
            work.buttonPhase = 0;
            work.selected = 1;
            work.animationFrame = 1;
        }
    }
    if (!shenGetMinutesUntilNextFrameMinute()) {
        work.dialogue = EV_PlayDialogue(GetRandomNumberInRange(2.0f) == 0 ? "0107A009" : "0107A011");
        HLib::SetCurrentTaskTo(JUKBExitCallback);
        HLib::SetupTaskDelay(45);
        work.timedOut = 1;
    }
    if (work.selected) {
        CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
        EV_DisableCharacterDisplay(JUKB_FINGER);
        EV_EnableCharacterDisplay(JUKB_PLAYER);
        LoadSoundProgramWithoutSelectedRecord(RecordBank(), musicSelections[work.jukeboxIndex][work.selection]);
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 470)) {
            HLib::SetCurrentTaskTo(JUKBStopRecordCallback);
            EVS_LoopCamera(1219, JUKB_TARGETS[5], JUKB_TARGETS[5]);
        } else {
            HLib::SetCurrentTaskTo(JUKBOpenMechanismCallback);
            EVS_LoopCamera(1220, JUKB_TARGETS[5], JUKB_TARGETS[5]);
        }
    }
}
// @verified - p
void JUKBOpenMechanismCallback(void *data) {
    JukbTaskData &work = *static_cast<JukbTaskData *>(data);
    if (work.animationFrame == 1)
        Loop3DSoundEffect(0x160);
    if (work.animationFrame != 0) {
        RotatePart(0x99, {-364, 0, 0});
        if (work.animationFrame - 11U <= 44)
            RotatePart(0x9a, {0, 0, 364});
        if (++work.animationFrame > 90) {
            work.animationFrame = 1;
            ScalePart(0x9b, 0);
            ScalePart(0xa1, 1);
            HLib::SetCurrentTaskTo(JUKBChangeRecordCallback);
            Loop3DSoundEffect(0x167);
        }
    }
}
// @verified - p
void JUKBChangeRecordCallback(void *data) {
    JukbTaskData &work = *static_cast<JukbTaskData *>(data);
    RotatePart(0x9f, {0, 0, -182});
    RotatePart(0xa0, {0, 0, -182});
    RotatePart(0xa1, {0, 0, -182});
    if (++work.animationFrame > 33) {
        work.animationFrame = 1;
        ScalePart(0x9b, 1);
        ScalePart(0xa0, 0);
        HLib::SetCurrentTaskTo(JUKBCloseMechanismCallback);
        Loop3DSoundEffect(0x168);
    }
}
// @verified - p
void JUKBCloseMechanismCallback(void *data) {
    JukbTaskData &work = *static_cast<JukbTaskData *>(data);
    if (work.animationFrame == 10)
        Loop3DSoundEffect(0x165);
    if (work.animationFrame == 20)
        Loop3DSoundEffect(0x160);
    if (work.animationFrame > 20) {
        RotatePart(0x99, {364, 0, 0});
        if (work.animationFrame - 56U <= 44)
            RotatePart(0x9a, {0, 0, -364});
    }
    if (++work.animationFrame > 110) {
        Loop3DSoundEffect(0x164);
        work.animationFrame = 1;
        HLib::SetCurrentTaskTo(JUKBStartRecordCallback);
    }
}
// @verified - p
void JUKBStartRecordCallback(void *data) {
    JukbTaskData &work = *static_cast<JukbTaskData *>(data);
    if (work.animationFrame >= 12 && work.animationFrame <= 21) {
        const std::int32_t angle = -static_cast<std::int32_t>((work.animationFrame - 12U) * .88 * 8192.0 / 45.0);
        RotatePart(0x9b, {angle, 0, 0});
        RotatePart(0x9d, {0, angle, 0});
    } else if (work.animationFrame > 21) {
        if (work.animationFrame < 60) {
            RotatePart(0x9b, {-1601, 0, 0});
            RotatePart(0x9d, {0, -1601, 0});
        }
        if (work.animationFrame < 37)
            RotatePart(0x9c, {0, -182, 0});
        else if (work.animationFrame - 47U <= 12)
            RotatePart(0x9c, {18, 0, 0});
    }
    if (work.animationFrame == 60) {
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 470, 1);
        Loop3DSoundEffect(0x161);
    }
    if (work.animationFrame == 70)
        Loop3DSoundEffect(0x162);
    if (work.animationFrame == 80) {
        EV_LoadAudio(RecordBank(), musicSelections[work.jukeboxIndex][work.selection]);
        EVS_LoopCamera(1230, JUKB_TARGETS[0], JUKB_TARGETS[0]);
    }
    if (work.animationFrame == 260)
        HLib::SetCurrentTaskTo(JUKBExitCallback);
    ++work.animationFrame;
}
// @verified - p
void JUKBStopRecordCallback(void *data) {
    JukbTaskData &work = *static_cast<JukbTaskData *>(data);
    if (work.animationFrame > 21) {
        if (work.animationFrame < 35)
            RotatePart(0x9c, {-18, 0, 0});
        else if (work.animationFrame < 50)
            RotatePart(0x9c, {0, 182, 0});
        else if (work.animationFrame < 70) {
            const std::int32_t angle = -static_cast<std::int32_t>((70U - work.animationFrame) * .44 * 8192.0 / 45.0);
            RotatePart(0x9b, {angle, 0, 0});
            RotatePart(0x9d, {0, angle, 0});
        } else if (work.animationFrame >= 90) {
            if (work.animationFrame >= 180) {
                work.animationFrame = 1;
                ScalePart(0x9b, 0);
                ScalePart(0xa1, 1);
                HLib::SetCurrentTaskTo(JUKBChangeRecordCallback);
                Loop3DSoundEffect(0x167);
            } else {
                RotatePart(0x99, {-364, 0, 0});
                if (work.animationFrame - 101U <= 44)
                    RotatePart(0x9a, {0, 0, 364});
            }
        }
    }
    if (work.animationFrame == 88)
        Loop3DSoundEffect(0x166);
    if (work.animationFrame == 90)
        Loop3DSoundEffect(0x160);
    if (work.animationFrame == 50) {
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 470, 0);
        EVS_LoopCamera(1220, JUKB_TARGETS[5], JUKB_TARGETS[5]);
    }
    if (work.animationFrame == 20)
        ProcessAicaCommand(0x1200a0, 0);
    ++work.animationFrame;
}
// @verified - p
void JUKBExitCallback(void *data) {
    using enum CameraState;
    JukbTaskData &work = *static_cast<JukbTaskData *>(data);
    EV_EventSetUpHelpIcon_0(0, 0);
    EventCling cling{0, JUKB_COIN, {}, 0, {}, {}, 1, 0};
    EVT_AT_Cling(&cling);
    EV_DisableCharacterDisplay(JUKB_COIN);
    SetEyeToChara(FindCharacterByScenarioName(JUKB_PLAYER), nullptr, 16, 0);
    hndlSetFlags(FindCharacterByScenarioName(JUKB_PLAYER));
    EV_SetCharaControllerFlags(JUKB_PLAYER, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(JUKB_PLAYER, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    for (std::int32_t i = 5; i != 9; ++i)
        SetCharaKill(JUKB_TARGETS[i], 1);
    EV_DisableCharacterDisplay(JUKB_FINGER);
    EV_EnableCharacterDisplay(JUKB_PLAYER);
    SetAllMapFieldDepthSlotsEnabled(1);
    camCtrlCacheUtil(3);
    EV_NPC_NewLoadSw(1);
    camCtrlSetState(Init);
    Vector3i zeroRotation{0, 0, 0};
    EV_scenApplyPosOrRotToPartOf(JUKB_BOX, 0x9f, nullptr, &zeroRotation, EventCharacterPartTransformMode::Absolute);
    EV_scenApplyPosOrRotToPartOf(JUKB_BOX, 0xa0, nullptr, &zeroRotation, EventCharacterPartTransformMode::Absolute);
    EV_scenApplyPosOrRotToPartOf(JUKB_BOX, 0xa1, nullptr, &zeroRotation, EventCharacterPartTransformMode::Absolute);
    ScalePart(0xa1, 0);
    ScalePart(0xa0, 1);
    UpdateMotionWorkWithPosition(scenFindMotionwork(JUKB_PLAYER), nullptr, 0);
    SetPlayerControlEnabled(1);
    EV_FreeAuthorPak(0, "0107");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("JBEV"), 0);
    CleanupMapControlCharacters();
    EV_HLKillOtherHierarchyTask(work.self);
}
const ScriptAreaDefinition areaJUKBScriptDefinition{.areaId = AREA_JUKB, .field04 = 0, .init = AreaJUKBInit, .main = AreaJUKBMain};
} // namespace shenmue::s2::areas
