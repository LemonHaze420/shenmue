#include "s2/areas/gabt_mole.hpp"
#include "s2/advice.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fog.hpp"
#include "s2/game_runtime.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/world_render.hpp"
#include <algorithm>
#include <bit>
#include <cmath>
namespace shenmue::s2::areas {
std::int32_t gabtOutcome, gabtInputPenalty, gabtInputArmed, gabtLatchedButtons, gabtHalfwayAdvice;
float gabtPlayerEndFrame, gabtOpponentEndFrame, gabtCameraTargetHeight, gabtCameraEyeHeight;
Vector4 gabtSavedGlobalLighting;
namespace {
constexpr std::uint32_t recursiveTransform = 0x40000000U, recursivePosition = 0x78000000U;
// @verified - p: the two CheckECXequalToZero calls in the presentation producer are deliberate native no-ops.
void GabtPresentationNullsub(std::int32_t value) {
    (void)value;
}
void SetMoleMotion(shen_name character, std::uint32_t animation, std::uint32_t flags) {
    EventSetMotion motion{.cleanup = 0, .character = character, .animation = animation, .frame = 0, .speed = 1, .flags = flags, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(static_cast<std::uint16_t>(animation));
}
std::int32_t MoleDirectionIndex(std::uint32_t buttons) {
    std::int32_t index = 0;
    while (index < 5 && GABT_MOLE_DIRECTIONS[index] != buttons)
        ++index;
    return index;
}
std::uint16_t MoleFixedAngle(float angle) {
    return static_cast<std::uint16_t>(static_cast<std::int32_t>(angle * 8192.0f / 45.0f));
}
float MoleCharacterDistance() {
    Vector3 player, opponent;
    EV_GetCharaPosition(GABT_MOLE_RYO, CHARACTER_TRANSLATION_ROOT, &player, recursiveTransform);
    EV_GetCharaPosition(GABT_MOLE_GAI, CHARACTER_TRANSLATION_ROOT, &opponent, recursiveTransform);
    const float x = player.x - opponent.x, z = player.z - opponent.z;
    return std::sqrt(z * z + x * x);
}
void StartMoleCamera() {
    using enum CameraState;
    camCtrlSetState(Event);
    gabtCameraTargetHeight = 118.1f;
    gabtCameraEyeHeight = 120.7f;
    camCtrlSetTargetPosition(Vector3{.x = 287.70001f, .y = 118.1f, .z = 75.800003f});
    camCtrlSetNextPosition(Vector3{.x = 287.70001f, .y = 120.7f, .z = 79.400002f});
    camCtrlSetFov(10922);
    const EulerRotation16 rotation{};
    camCtrlSetNextCamRot(&rotation);
    camCtrlToggleSmoothing(1);
    camCtrlSetMotionScaleFactor(.1f);
}
void PrepareMolePresentation() {
    EV_SetCharacterAlphaEnabled(GABT_MOLE_RYO, true);
    EV_SetCharacterAlphaEnabled(GABT_MOLE_GAI, true);
    LoadBlurModelForCharacter(GABT_MOLE_GAI, nullptr, .7f);
    figpAssignModeFor(GABT_MOLE_GAI, 1);
    figpSetFlagsBlendForBlur(GABT_MOLE_GAI, 5, 1);
    GabtPresentationNullsub(0);
    GabtPresentationNullsub(1);
    Vector4 reflector{.x = 1, .y = 0, .z = 0, .w = 0};
    EV_SetRefBoardParamsFor(GABT_MOLE_GAI, 1);
    SetScenarioReflectorBoardValues(GABT_MOLE_GAI, &reflector);
    reflector.y = .9f;
    reflector.z = .9f;
    reflector.w = .9f;
    EV_SetRefBoardParamsFor(GABT_MOLE_RYO, 1);
    SetScenarioReflectorBoardValues(GABT_MOLE_RYO, &reflector);
    if (DoesScenarioCharacterExist(GABT_MOLE_REFLECTOR))
        EV_SetRefBoardParamsFor(GABT_MOLE_REFLECTOR, 0);
}
void PrepareMolePositions() {
    for (shen_name character : {GABT_MOLE_RYO, GABT_MOLE_GAI}) {
        EV_SetCharaControllerFlags(character, UpdateHeight, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(character, ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    }
    EventSetPosition position{
        .cleanup = 0, .character = GABT_MOLE_BASE, .parent = GABT_MOLE_INVALID, .position = {.x = 287.70001f, .y = 118, .z = 77.4f}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = GABT_MOLE_BASE, .parent = GABT_MOLE_INVALID, .degrees = 180, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    AT_SetLocalBaseChara(GABT_MOLE_RYO, GABT_MOLE_BASE);
    AT_SetLocalBaseChara(GABT_MOLE_GAI, GABT_MOLE_BASE);
    SetMoleMotion(GABT_MOLE_RYO, 306, 0);
    position.character = GABT_MOLE_RYO;
    position.parent = GABT_MOLE_BASE;
    position.position = {.x = 0, .y = 0, .z = 0};
    EVT_AT_SetPosRot(&position);
    angle.character = GABT_MOLE_RYO;
    angle.parent = GABT_MOLE_BASE;
    EVT_AT_SetAngY_Callback(&angle);
    position.character = GABT_MOLE_GAI;
    position.position = {.x = 2, .y = 0, .z = 0};
    EVT_AT_SetPosRot(&position);
    angle.character = GABT_MOLE_GAI;
    angle.degrees = 0;
    EVT_AT_SetAngY_Callback(&angle);
}
void UpdateMoleCamera(const GabtMoleWork &work) {
    if (work.ticksLeft > 4500) {
        gabtCameraEyeHeight += .0008888889f;
        gabtCameraTargetHeight -= .00022222222f;
    } else if ((work.ticksLeft / 900) & 1) {
        gabtCameraEyeHeight += .003111111f;
        gabtCameraTargetHeight -= .0013333334f;
    } else {
        gabtCameraEyeHeight -= .003111111f;
        gabtCameraTargetHeight += .0013333334f;
    }
    camCtrlSetNextPosition(Vector3{.x = 287.70001f, .y = gabtCameraEyeHeight, .z = 79.400002f});
    camCtrlSetTargetPosition(Vector3{.x = 287.70001f, .y = gabtCameraTargetHeight, .z = 75.800003f});
}
void RetireMole(GabtMoleWork &work) {
    SetGlobalState(-1);
    mtwkSetRequestedRootMode(scenFindMotionwork(GABT_MOLE_RYO), 0);
    mtwkSetRequestedRootMode(scenFindMotionwork(GABT_MOLE_GAI), 0);
    SetMotionFrameTime(scenFindMotionwork(GABT_MOLE_RYO), 0, MotionRegion::LowerBody);
    SetMotionFrameTime(scenFindMotionwork(GABT_MOLE_GAI), 0, MotionRegion::LowerBody);
    figpAssignModeFor(GABT_MOLE_RYO, 0);
    figpAssignModeFor(GABT_MOLE_GAI, 0);
    if (DoesScenarioCharacterExist(GABT_MOLE_REFLECTOR))
        EV_SetRefBoardParamsFor(GABT_MOLE_REFLECTOR, 1);
    ReinitFogTasks(1);
    EV_EnableCharacterDisplay(GABT_MOLE_SCENERY);
    EV_EnsureCharacterDraw(GABT_MOLE_SCENERY);
    ExchangeRetainedGlobalLighting(gabtSavedGlobalLighting, nullptr);
    UpdateGlobalLighting(gabtSavedGlobalLighting);
    gabtOutcome = work.result;
    HLib::EV_HLKillOtherHierarchyTask(work.task);
}
bool BeginMoleAttack(GabtMoleWork &work, std::uint32_t direction, std::uint32_t attack) {
    if (!attack)
        return false;
    if (work.interpolationFrames || work.inputState == 1)
        work.currentDirection = work.targetDirection;
    std::uint32_t animation = 9479;
    if (direction) {
        work.targetDirection = MoleDirectionIndex(direction);
        switch (work.targetDirection - work.currentDirection) {
        case -4:
        case 4:
            animation = 9484;
            work.currentDirection = 4 - work.currentDirection;
            break;
        case -3:
            --work.currentDirection;
            animation = 9483;
            break;
        case -2:
            animation = 9483;
            break;
        case -1:
            animation = 9481;
            break;
        case 1:
            animation = 9480;
            break;
        case 2:
            animation = 9482;
            break;
        case 3:
            ++work.currentDirection;
            animation = 9482;
            break;
        default:
            break;
        }
    }
    const Vector3i rotation{.x = 0, .y = static_cast<std::int32_t>(GABT_MOLE_ANGLES[work.currentDirection] * 8192.0f / 45.0f) - 0x4000, .z = 0};
    EV_AT_SetAngY(GABT_MOLE_RYO, recursivePosition, rotation);
    SetMoleMotion(GABT_MOLE_RYO, animation, 0x40000000U);
    SetMotionSpeed(scenFindMotionwork(GABT_MOLE_RYO), 1, MotionRegion::LowerBody);
    mtwkSetExtraFlag(scenFindMotionwork(GABT_MOLE_RYO), 0, 2);
    work.inputFrames = 15;
    return true;
}
void AdvanceMolePlay(GabtMoleWork &work) {
    work.sector = std::max(static_cast<std::int32_t>(work.orbitAngle / 36.0f), 0);
    GabtUpdateMoleOpponent(&work);
    GabtUpdateMoleReflector(&work);
    --work.ticksLeft;
    --work.remainingFrames;
    if (work.cooldown)
        --work.cooldown;
    if (work.difficulty < 1)
        work.difficulty += .00013888889f;
    if (work.waitLimit > 30)
        work.waitLimit -= .011111111f;
    if (work.ticksLeft % 16 == 0) {
        const std::int32_t phase = (work.ticksLeft / 16) % 3;
        if (phase == 0)
            CheckHelpIcon(16416, 0, CheckHelpIconOp::Set);
        else if (phase == 1)
            CheckHelpIcon(32800, 0, CheckHelpIconOp::Set);
        else if (phase == 2)
            CheckHelpIcon(4128, 0, CheckHelpIconOp::Set);
        else
            CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    } else if (work.ticksLeft % 8 == 0)
        CheckHelpIcon(32, 0, CheckHelpIconOp::Set);
}
} // namespace
// @verified - p: GABT_init_function resets the whole owner-specific global block.
void InitializeGabtMoleGlobals() {
    gabtInputPenalty = 0;
    gabtOpponentEndFrame = 0;
    gabtPlayerEndFrame = 0;
    gabtCameraTargetHeight = 0;
    gabtCameraEyeHeight = 0;
    gabtSavedGlobalLighting = {.x = 0, .y = 0, .z = 0, .w = 0};
    gabtHalfwayAdvice = 0;
    gabtOutcome = 0;
    gabtLatchedButtons = 0;
    gabtInputArmed = 0;
}
// @verified - p: MoleAttack_Enqueue2905f0; prepare2907e0. Native allocation leaves all non-written work fields untouched.
void QueueGabtMoleAttack(std::int32_t entry) {
    SetPlayerControlEnabled(0);
    SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    SetIsSkySetup(0);
    SetMapControlStatusFlag(0, 0);
    EV_SetModelDataDefaultVertexShader();
    ReinitFogTasks(0);
    EV_ReinitLights(0);
    EV_SetWeatherInstruction(15, 0);
    EV_SetWeatherInstruction(0x780, 0);
    EV_SetWeatherInstruction(0x1800, 0);
    EV_SetIndexForLightEntry(1, 0);
    EV_SetIndexForLightEntry(2, 0);
    EV_SetIndexForLightEntry(3, 0);
    EV_DisableCharacterDisplay(GABT_MOLE_SCENERY);
    EV_ToggleCharacterFlag(GABT_MOLE_SCENERY);
    const Vector4 darkness{.x = 0, .y = 0, .z = 0, .w = 0};
    ExchangeRetainedGlobalLighting(darkness, &gabtSavedGlobalLighting);
    UpdateGlobalLighting(darkness);
    EV_EventSetUpHelpIcon_0(0x5a, 0);
    if (entry != 99) {
        mtwkSetRequestedRootMode(scenFindMotionwork(GABT_MOLE_RYO), 1);
        mtwkSetRequestedRootMode(scenFindMotionwork(GABT_MOLE_GAI), 1);
        SetMotionFrameTime(scenFindMotionwork(GABT_MOLE_RYO), 6, MotionRegion::LowerBody);
        SetMotionFrameTime(scenFindMotionwork(GABT_MOLE_GAI), 6, MotionRegion::LowerBody);
    }
    gabtLatchedButtons = 0;
    gabtInputArmed = 1;
    HLib::Task *task = HLib::EnqueueTaskWithParameter(GabtMoleCallback, 5, 0x0b, 0x90, GABT_MOLE_TASK);
    GabtMoleWork &work = *static_cast<GabtMoleWork *>(HLib::GetTaskParameter(task));
    work.entry = entry;
    FinishLoading();
    work.task = task;
    work.state = 0;
    gabtOutcome = 0;
    gabtHalfwayAdvice = 0;
    SetGlobalState(3);
    if (work.entry == 99) {
        work.characters = Queue_EVT_AT_ReadChr(work.task, GABT_MOLE_CHARACTERS);
        EV_FreeAuthorPak(1, "0232");
        CreateEventCharacter(CharacterCreationHighModel, nullptr, GABT_MOLE_BASE);
        EV_SetNewSceneIndex();
        FinishLoading();
        SetRenderClockAndCompassTo(1);
    }
}
// @verified - p: preserves absolute position and rotation across each native parent removal.
void GabtDetachCharacterBases() {
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(GABT_MOLE_RYO, CHARACTER_TRANSLATION_ROOT, &position, recursiveTransform);
    GetScenarioCharacterEulerAngles(GABT_MOLE_RYO, &rotation, recursiveTransform);
    AT_SetLocalBaseChara(GABT_MOLE_RYO, GABT_MOLE_INVALID);
    EV_AT_SetPos(GABT_MOLE_RYO, recursivePosition, position);
    EV_AT_SetAngY(GABT_MOLE_RYO, recursivePosition, rotation);
    GetScenarioCharacterEulerAngles(GABT_MOLE_GAI, &rotation, recursiveTransform);
    EV_GetCharaPosition(GABT_MOLE_GAI, CHARACTER_TRANSLATION_ROOT, &position, recursiveTransform);
    AT_SetLocalBaseChara(GABT_MOLE_GAI, GABT_MOLE_INVALID);
    EV_AT_SetPos(GABT_MOLE_GAI, recursivePosition, position);
    EV_AT_SetAngY(GABT_MOLE_GAI, recursivePosition, rotation);
}
// @verified - p
void GabtUpdateMoleOrbit(GabtMoleWork *work) {
    Vector3 position;
    EV_GetCharaPosition(GABT_MOLE_RYO, CHARACTER_TRANSLATION_ROOT, &position, recursiveTransform);
    const std::uint16_t orbit = MoleFixedAngle(work->orbitAngle), rotation = MoleFixedAngle(work->orbitAngle + work->rotationOffset);
    position.x += work->radius * cos_fixed16(orbit);
    position.z -= work->radius * sin_fixed16(static_cast<std::int16_t>(orbit));
    position.x += cos_fixed16(rotation) * .25f;
    position.z -= sin_fixed16(static_cast<std::int16_t>(rotation)) * .25f;
    EV_AT_SetPos(GABT_MOLE_GAI, recursivePosition, position);
}
// @verified - p: input capture deliberately waits for a second poll and retains the native invalid-combination admission.
std::uint32_t GabtLatchMoleInput() {
    std::uint32_t buttons = static_cast<std::uint32_t>(GetCurrentControlInput(INPUT_NEW_PRESSES)) & 0xf606U;
    if (gabtInputArmed) {
        if (buttons && buttons != 62982) {
            gabtLatchedButtons = static_cast<std::int32_t>(buttons);
            gabtInputArmed = 0;
            return 0;
        }
    } else {
        const std::uint32_t held = buttons ? static_cast<std::uint32_t>(GetCurrentControlInput(INPUT_HELD)) & gabtLatchedButtons : gabtLatchedButtons;
        buttons |= held;
        gabtLatchedButtons = 0;
        gabtInputArmed = 1;
    }
    return buttons;
}
// @verified - p: playerinput; direction masks outside the five authored combinations are a native caller precondition.
std::int32_t GabtProcessMoleInput(GabtMoleWork *pointer) {
    GabtMoleWork &work = *pointer;
    const std::uint32_t input = GabtLatchMoleInput(), direction = input & 0xd000U, attack = input & 4U;
    const std::uint32_t held = static_cast<std::uint32_t>(GetCurrentControlInput(INPUT_HELD)) & 0xd000U;
    switch (work.inputState) {
    case 0:
        if (direction && !attack) {
            work.targetDirection = MoleDirectionIndex(direction);
            work.interpolationFrames = 7;
            SetMoleMotion(GABT_MOLE_RYO, 409, 0x40000000U);
            work.inputState = 1;
        } else if (BeginMoleAttack(work, held, attack))
            work.inputState = 2;
        break;
    case 1:
        if (BeginMoleAttack(work, held, attack))
            work.inputState = 2;
        else if (SetMotionRegionFlag(scenFindMotionwork(GABT_MOLE_RYO), 65, 1, MotionRegion::LowerBody) && !work.interpolationFrames) {
            work.currentDirection = work.targetDirection;
            SetMoleMotion(GABT_MOLE_RYO, 306, 0);
            work.inputState = 0;
        }
        break;
    case 2:
        if (work.inputFrames-- == 1) {
            work.interpolationFrames = work.currentDirection != work.targetDirection ? 7 : 0;
            SetMoleMotion(GABT_MOLE_RYO, 306, 0);
            work.inputState = 3;
        } else if (static_cast<std::uint32_t>(work.inputFrames - 3) <= 5 && work.targetDirection == work.sector) {
            if (work.opponentState == 3 && work.ticksLeft <= 5100) {
                if (work.alpha > .5f)
                    return 1;
            } else if (work.opponentState != 5) {
                if (work.alpha > work.alphaThreshold) {
                    work.orbitSign = work.orbitAngle < 90 ? 1 : -1;
                    work.rotationOffset = 0;
                    work.radius = MoleCharacterDistance() - .25f;
                    work.opponentState = 5;
                }
                return -1;
            }
        }
        break;
    case 3:
        if (!work.interpolationFrames) {
            work.currentDirection = work.targetDirection;
            work.inputState = 0;
        }
        break;
    default:
        break;
    }
    if (work.interpolationFrames) {
        if (work.currentDirection != work.targetDirection) {
            const float angle = (7 - work.interpolationFrames) / 7.0f * GABT_MOLE_ANGLES[work.targetDirection] +
                work.interpolationFrames / 7.0f * GABT_MOLE_ANGLES[work.currentDirection];
            const Vector3i rotation{.x = 0, .y = static_cast<std::int32_t>(angle * 8192.0f / 45.0f) - 0x4000, .z = 0};
            EV_AT_SetAngY(GABT_MOLE_RYO, recursivePosition, rotation);
        }
        --work.interpolationFrames;
    }
    return 0;
}
// @verified - p: opponent state graph, including the float wait-limit's native signed DWORD comparison and state3 early return.
void GabtUpdateMoleOpponent(GabtMoleWork *pointer) {
    GabtMoleWork &work = *pointer;
    switch (work.opponentState) {
    case 0: {
        const float delta = work.orbitSign * work.orbitSpeed;
        if (work.ticksLeft % 30 == 0)
            work.targetSpeed = GetRandomNumberInRange(2) + 1;
        if (work.targetSpeed > work.orbitSpeed)
            work.orbitSpeed = static_cast<float>(work.orbitSpeed + .5);
        else if (work.orbitSpeed > work.targetSpeed)
            work.orbitSpeed = static_cast<float>(work.orbitSpeed - .5);
        work.orbitAngle += delta;
        if (work.orbitAngle <= .2f || work.orbitAngle >= 178) {
            work.orbitAngle = work.orbitAngle <= .2f ? .2f : 178;
            work.spinSign = work.orbitSign;
            work.opponentState = work.ticksLeft < 3600 && (shenGetRandomInt32(2) & 1) ? 1 : 2;
        }
        if (work.rotationOffset != 0) {
            work.rotationOffset += work.spinSign * work.orbitSpeed * 5;
            if (work.rotationOffset <= -360 || work.rotationOffset >= 360)
                work.rotationOffset = 0;
        }
        work.radius = std::min(work.radius + .050000001f, 1.6f);
        if (work.rotationOffset == 0 && static_cast<std::uint32_t>(work.ticksLeft - 1800) <= 0x9d7U && work.ticksLeft % 100 == 0 &&
            work.difficulty >= GetRandomNumberInRange(1) && work.previousOrbitSign != work.orbitSign) {
            EVT_AT_SetSound(0x108ab, 3);
            work.spinSign = work.orbitSign;
            work.previousOrbitSign = work.orbitSign;
            work.opponentState = 1;
            break;
        }
        ++work.enemyWait;
        bool nearDirection = false;
        const std::int32_t waitBits = std::bit_cast<std::int32_t>(work.waitLimit);
        if (work.enemyWait <= waitBits)
            nearDirection = std::fabs(GABT_MOLE_ANGLES[work.randomSector] - work.orbitAngle) < 1.5f;
        else
            for (float angle : GABT_MOLE_ANGLES)
                if (std::fabs(angle - work.orbitAngle) < 1.5f) {
                    nearDirection = true;
                    break;
                }
        if (work.enemyWait > 60 && nearDirection && work.rotationOffset == 0 && work.radius == 1.6f &&
            ((shenGetRandomInt32(2) & 1) || work.enemyWait > waitBits)) {
            EVT_AT_SetMot(GABT_MOLE_GAI, 0x163, 0, 1, 0, 0);
            const float speed = std::fmax(0.0f, (work.ticksLeft - 900) / 3600.0f * .54999995f) + .75f;
            SetMotionSpeed(scenFindMotionwork(GABT_MOLE_GAI), speed, MotionRegion::LowerBody);
            work.randomSector = shenGetRandomInt32(5);
            work.opponentState = 3;
        } else if (SetMotionRegionFlag(scenFindMotionwork(GABT_MOLE_GAI), 65, 1, MotionRegion::LowerBody)) {
            EVT_AT_SetMot(GABT_MOLE_GAI, work.orbitSpeed >= 0 ? 0x248 : 0x249, 0, 1, 0, 0);
            SetMotionSpeed(scenFindMotionwork(GABT_MOLE_GAI), .5f, MotionRegion::LowerBody);
        }
        break;
    }
    case 1:
    case 2:
        work.rotationOffset += work.spinSign * 3.0f * 5;
        if (work.rotationOffset > 180 || work.rotationOffset < -180) {
            work.orbitSign = -work.spinSign;
            work.opponentState = 0;
        }
        break;
    case 3:
        if (SetMotionRegionFlag(scenFindMotionwork(GABT_MOLE_GAI), 65, 1, MotionRegion::LowerBody)) {
            work.enemyWait = 0;
            const float distance = MoleCharacterDistance();
            work.radius = distance + .25f;
            work.spinSign = work.orbitAngle >= 90 ? -1 : 1;
            work.rotationOffset = work.spinSign >= 0 ? 180.0f : -180.0f;
            work.orbitSpeed = 3;
            if (shenGetRandomInt32(4) & 1)
                work.opponentState = 4;
            else {
                work.rotationOffset = 0;
                work.opponentState = 0;
                work.radius = distance - .25f;
            }
            return;
        }
        break;
    case 4:
        work.rotationOffset += work.spinSign * work.orbitSpeed * 4;
        if (work.rotationOffset <= -360 || work.rotationOffset >= 360) {
            work.orbitSign = work.spinSign;
            work.rotationOffset = 0;
            work.opponentState = 0;
        }
        break;
    case 5:
        work.orbitAngle += work.orbitSign * 3.0f * 4;
        if (work.alphaThreshold >= work.alpha)
            work.opponentState = 0;
        break;
    default:
        break;
    }
    if (work.opponentState >= 0 && (work.opponentState <= 2 || static_cast<std::uint32_t>(work.opponentState - 4) <= 1))
        GabtUpdateMoleOrbit(&work);
    Vector3 player, opponent;
    Vector3i rotation;
    EV_GetCharaPosition(GABT_MOLE_RYO, CHARACTER_TRANSLATION_ROOT, &player, recursiveTransform);
    EV_GetCharaPosition(GABT_MOLE_GAI, CHARACTER_TRANSLATION_ROOT, &opponent, recursiveTransform);
    const std::int32_t yaw = InverseFixedAtan2(player.x - opponent.x, player.z - opponent.z);
    GetScenarioCharacterEulerAngles(GABT_MOLE_GAI, &rotation, recursiveTransform);
    rotation.y = yaw;
    EV_AT_SetAngY(GABT_MOLE_GAI, recursivePosition, rotation);
}
// @verified - p
void GabtUpdateMoleReflector(GabtMoleWork *pointer) {
    GabtMoleWork &work = *pointer;
    if (work.opponentState == 3) {
        if (work.alpha > .69999999f)
            work.alpha -= .1f;
        else if (work.alpha < .60000002f)
            work.alpha += .1f;
    } else if (work.opponentState == 1) {
        if (work.alpha > .34999999f)
            work.alpha -= .050000001f;
        else if (work.alpha < .30000001f)
            work.alpha += .050000001f;
    } else {
        work.alpha -= work.opponentState == 5 ? .050000001f : .02f;
        if (work.alpha < work.alphaThreshold)
            work.alpha = work.alphaThreshold;
    }
    const float reflected = std::max(work.alpha - .2f, 0.0f);
    const Vector4 reflector{.x = 1, .y = reflected, .z = work.alpha, .w = reflected};
    if (work.ticksLeft < 3600 && work.alphaThreshold < .30000001f && work.ticksLeft > 0 && work.ticksLeft % 20 == 0)
        work.alphaThreshold += .0049999999f;
    EV_SetRefBoardParamsFor(GABT_MOLE_GAI, 1);
    SetScenarioReflectorBoardValues(GABT_MOLE_GAI, &reflector);
}
// @verified - p: MoleAttack_Callback290970, all native states and outcome lifetimes.
void GabtMoleCallback(void *data) {
    GabtMoleWork &work = *static_cast<GabtMoleWork *>(data);
    switch (work.state) {
    case 0:
        work.targetDirection = 2;
        work.reserved30 = 0;
        work.inputState = 0;
        work.opponentState = 0;
        work.alpha = 0;
        work.alphaThreshold = 0;
        work.enemyWait = 0;
        work.currentDirection = 2;
        work.previousDirection = -1;
        work.orbitAngle = 90;
        work.orbitSign = 1;
        work.spinSign = 1;
        work.targetSpeed = 3;
        work.orbitSpeed = 3;
        work.radius = 1.6f;
        work.previousOrbitSign = 1;
        work.difficulty = .5f;
        work.waitLimit = 90;
        StartMoleCamera();
        PrepareMolePresentation();
        PrepareMolePositions();
        work.state = 1;
        break;
    case 1:
        GabtDetachCharacterBases();
        GabtUpdateMoleOrbit(&work);
        EVT_AT_SetMot(GABT_MOLE_RYO, 306, 0, 1, 0, 0);
        EVT_AT_SetMot(GABT_MOLE_GAI, 306, 0, 1, 0, 0);
        CheckHelpIcon(32, 0, CheckHelpIconOp::Set);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        work.state = 2;
        break;
    case 2:
        if (!filmCheckFadeTrackColour()) {
            work.remainingFrames = 5400;
            work.cooldown = 0;
            work.inputFrames = 0;
            work.interpolationFrames = 0;
            work.ticksLeft = 5400;
            work.randomSector = shenGetRandomInt32(5);
            work.orbitSign = shenGetRandomInt32(2);
            const std::uint32_t animation = work.orbitSign == 1 ? 0x248 : 0x249;
            if (work.orbitSign != 1)
                work.orbitSign = -1;
            EVT_AT_SetMot(GABT_MOLE_GAI, animation, 0, 1, 0, 0);
            SetMotionSpeed(scenFindMotionwork(GABT_MOLE_GAI), .5f, MotionRegion::LowerBody);
            EV_PlayDialogue("0232B053");
            work.framesElapsed = 0;
            work.voiceIndex = 0;
            work.state = 3;
        }
        break;
    case 3: {
        const std::int32_t result = GabtProcessMoleInput(&work);
        bool finish = false;
        if (result == 1) {
            scenCvsCtrlFree();
            Loop3DSoundEffect(0x900ab);
            Loop3DSoundEffect(0xb0d);
            work.result = 1;

            gabtPlayerEndFrame = GetMotionFrame(scenFindMotionwork(GABT_MOLE_RYO), MotionRegion::LowerBody);
            if (gabtPlayerEndFrame < 9)
                gabtPlayerEndFrame = 9;
            gabtOpponentEndFrame = GetMotionFrame(scenFindMotionwork(GABT_MOLE_GAI), MotionRegion::LowerBody);
            finish = true;
        } else {
            if (work.framesElapsed % 450 == 0) {
                constexpr const char *voices[]{"0232B017", "0232B018", "0232B019", "0232B020", "0232B021", "0232B022"};
                if (work.voiceIndex >= 0 && work.voiceIndex <= 5)
                    EV_PlayDialogue(voices[work.voiceIndex]);
                if (work.voiceIndex <= 5)
                    ++work.voiceIndex;
                else
                    work.voiceIndex = 0;
            }
            if (result == -1 && (gabtInputPenalty & 1))
                work.remainingFrames = std::max(work.remainingFrames - 90, 0);
            if (work.remainingFrames < 2700 && !gabtHalfwayAdvice) {
                gabtHalfwayAdvice = 1;
                QueueFightAdviceControl(19);
            }
            if (!work.remainingFrames) {
                work.result = -1;
                gabtPlayerEndFrame = 999;
                gabtOpponentEndFrame = 999;
                finish = true;
            }
        }
        if (finish) {
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            work.state = 6;
        }
        AdvanceMolePlay(work);
        break;
    }
    case 6:
        if (GetMotionFrame(scenFindMotionwork(GABT_MOLE_RYO), MotionRegion::LowerBody) > gabtPlayerEndFrame)
            SetMotionSpeed(scenFindMotionwork(GABT_MOLE_RYO), 0, MotionRegion::LowerBody);
        if (GetMotionFrame(scenFindMotionwork(GABT_MOLE_GAI), MotionRegion::LowerBody) > gabtOpponentEndFrame)
            SetMotionSpeed(scenFindMotionwork(GABT_MOLE_GAI), 0, MotionRegion::LowerBody);
        if (!filmCheckFadeTrackColour()) {
            ctrlStopSubtitles();
            CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
            if (work.entry != 99) {
                RetireMole(work);
                return;
            }
            work.state = 0;
            return;
        }
        break;
    default:
        break;
    }
    SetScenarioCharacterTransparency(GABT_MOLE_RYO, .5f);
    SetScenarioCharacterTransparency(GABT_MOLE_GAI, work.alpha);
    UpdateMoleCamera(work);
    GetBattleDataConfig(0, GABT_MOLE_BUTTON);
    ++work.framesElapsed;
}
} // namespace shenmue::s2::areas
