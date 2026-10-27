#include "s2/areas/area_gach.hpp"
#include "s2/areas/gach_tables.hpp"
#include "s2/area_event.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/sound_program.hpp"
#include "s2/face.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/inventory.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/ui.hpp"
#include "s2/step_event.hpp"
#include <cmath>
#include <cstring>
#include <cstdio>
namespace shenmue::s2 {
namespace {
shen_name machineCharacter;
std::int32_t capsuleColour, coinTurnFrames, secondTurnFrames, capsuleFallFrames, shellFallFrames, colourFallFrames, rightHandFrames, leftHandFrames;
std::uint16_t motionStartFrame;
float shellFallStep, colourFallStep, fadeInFrames, fadeOutFrames, prizeRandom;
float prizeRarityCounts[6];
Vector3 originalPartPosition[2];
Vector3i originalPartRotation[2];
GachHandRecord handRecord;
const GachHandRecord *handRecords;
inline constexpr shen_name controllerNames[]{GACH_RYO, SHEN_NAME(CAPR), SHEN_NAME(CAPB), SHEN_NAME(CAPG), SHEN_NAME(CAPY), SHEN_NAME(CUWH), GACH_SHELL,
    SHEN_NAME(CDBL), SHEN_NAME(CDRD), SHEN_NAME(CDGR), SHEN_NAME(CDYE)};
const GachCatalogEntry &Prize(const GachTaskWork *work) {
    return gachCatalog[gachSets[work->set].tiers[work->rarity][work->selection]];
}
std::int64_t Package(std::int32_t operation, const char *first = nullptr, const char *second = nullptr) {
    return EV_ControlPackageResources(PakCategory::Package, operation, reinterpret_cast<std::uintptr_t>(first), reinterpret_cast<std::uintptr_t>(second));
}
void Place(shen_name character, shen_name parent, const Vector3 &position) {
    EventSetPosition request{0, character, parent, position, 0};
    EVT_AT_SetPosRot(&request);
}
void Turn(shen_name character, shen_name parent, float degrees) {
    EventSetAngle request{0, character, parent, degrees, 0};
    EVT_AT_SetAngY_Callback(&request);
}
void Cling(shen_name target, shen_name character, std::int32_t bone, const Vector3 &position, const Vector3i &rotation, std::int32_t delay) {
    if (!delay) {
        EventCling request{0, target, character, bone, position, rotation, 0, 0};
        EVT_AT_Cling(&request);
    } else {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), GACH_TASK);
        RegisterEventTask(task);
        *static_cast<EventCling *>(HLib::GetTaskParameter(task)) = {1, target, character, bone, position, rotation, 0, static_cast<float>(delay)};
    }
}
void Uncling(shen_name target, std::int32_t delay) {
    if (!delay) {
        EventCling request;
        request.cleanup = 0;
        request.target = target;
        request.character = {};
        request.mode = 1;
        request.delay = 0;
        EVT_AT_Cling(&request);
    } else {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_Cling, 6, 0x0b, sizeof(EventCling), GACH_TASK);
        RegisterEventTask(task);
        EventCling *const request = static_cast<EventCling *>(HLib::GetTaskParameter(task));
        request->cleanup = 1;
        request->target = target;
        request->character = {};
        request->mode = 1;
        request->delay = static_cast<float>(delay);
    }
}
void Display(shen_name character, std::int32_t delay, std::int32_t mode) {
    EVT_AT_ObjectDisp_1(character, delay, mode);
}
void Pose(shen_name character, std::int32_t *command, std::int16_t ticks) {
    ApplyCharacterHandPoseCommand(ctrlGetCharaByName(character), 0, reinterpret_cast<std::byte *>(command), ticks);
}
void QueuePose(std::int32_t *command, std::uint32_t ticks, std::int32_t delay) {
    Queue_EVT_AT_CtrlHandUp_1(GACH_RYO, 0, reinterpret_cast<std::byte *>(command), ticks, delay);
}
void Queue(HLib::TaskCallback callback) {
    HLib::EnqueueTaskWithoutParameter(callback, 4, 0x0b, GACH_TASK.name);
}
// @verified - p
void Speak(GachTaskWork *work, std::uint8_t mode) {
    const char *voice = nullptr;
    switch (mode) {
    case 0:
        voice = static_cast<std::uint8_t>(GetRandomNumberInRange(2)) == 0 ? "0108A031" : "0108A036";
        break;
    case 1:
        voice = static_cast<std::uint8_t>(GetRandomNumberInRange(2)) == 0 ? "0108A026" : "0108A027";
        break;
    case 2: {
        const std::int32_t choice = static_cast<std::int32_t>(GetRandomNumberInRange(3));
        voice = choice == 0 ? "0108A006" : choice == 1 ? "0108A007" : "0108A008";
        break;
    }
    case 3: {
        const std::int32_t choice = static_cast<std::int32_t>(GetRandomNumberInRange(3));
        voice = choice == 0 ? "0108A003" : choice == 1 ? "0108A004" : "0108A005";
        break;
    }
    case 4: {
        const std::int32_t choice = static_cast<std::int32_t>(GetRandomNumberInRange(3));
        EVT_AT_SetVoice(choice == 0 ? "0108A009" : choice == 1 ? "0108A010" : "0108A011", 1, 4); // @text 0108A009 EN: "No, I guess not." | JA: "いや、やめとこう。" ; 0108A010 EN: "I guess not." | JA: "やっぱり、やめよう。" ; 0108A011 EN: "I won't this time." | JA: "やめとくか。"
        return;
    }
    default:
        return;
    }
    work->voice = EV_PlayDialogue(voice);
}
// @verified - p
void SpeakPrize(GachTaskWork *work) {
    const char *voice;
    if (work->rarity < 2) {
        const std::int32_t choice = static_cast<std::int32_t>(GetRandomNumberInRange(4));
        voice = choice == 0 ? "0108A022" : choice == 1 ? "0108A023" : choice == 2 ? "0108A024" : "0108A025";
    } else if (work->previousItem != Prize(work).item || work->set == 11) {
        const std::int32_t choice = static_cast<std::int32_t>(GetRandomNumberInRange(5));
        voice = choice == 0 ? "0108A014" : choice == 1 ? "0108A015" : choice == 2 ? "0108A016" : choice == 3 ? "0108A017" : "0108A018";
    } else {
        const std::int32_t choice = static_cast<std::int32_t>(GetRandomNumberInRange(3));
        voice = choice == 0 ? "0108A019" : choice == 1 ? "0108A020" : "0108A021";
    }
    work->voice = EV_PlayDialogue(voice);
    work->previousItem = Prize(work).item;
}
// @verified - p
void BeginExit(GachTaskWork *work) {
    using enum CameraState;
    camCtrlSetState(Init);
    UpdateMotionWorkWithPosition(scenFindMotionwork(GACH_RYO), nullptr, 0);
    work->state = 12;
}
// @verified - p
const char *MachineArchive(shen_name machine) {
    GachLocalDebugNullsub();
    switch (machine.name) {
    case SHEN_NAME(GA10):
    case SHEN_NAME(GA60):
    case SHEN_NAME(GA16):
        return "GACH2H6G";
    case SHEN_NAME(GA50):
    case SHEN_NAME(GA08):
        return "GACH2H3G";
    case SHEN_NAME(GA01):
    case SHEN_NAME(GA62):
        return "GACH2H5G";
    case SHEN_NAME(GA11):
    case SHEN_NAME(GA61):
        return "GACH2H8G";
    case SHEN_NAME(GA51):
    case SHEN_NAME(GA64):
    case SHEN_NAME(GA15):
    case SHEN_NAME(GA56):
    case SHEN_NAME(GA07):
        return "GACH2H1G";
    case SHEN_NAME(GA14):
    case SHEN_NAME(GA06):
    case SHEN_NAME(GA53):
    case SHEN_NAME(GA02):
        return "GACH2H0G";
    case SHEN_NAME(GA57):
    case SHEN_NAME(GA12):
    case SHEN_NAME(GA63):
        return "GACH2H4G";
    case SHEN_NAME(GA09):
    case SHEN_NAME(GA59):
    case SHEN_NAME(GA05):
    case SHEN_NAME(GA52):
        return "GACH2H9G";
    case SHEN_NAME(GA03):
    case SHEN_NAME(GA55):
        return "GACH2H2G";
    case SHEN_NAME(GA58):
    case SHEN_NAME(GA54):
    case SHEN_NAME(GA13):
        return "GACH2H7G";
    default:
        return nullptr;
    }
}
// @verified - p
void GachFirstTurnCallback(void *) {
    if (coinTurnFrames > 136 - motionStartFrame) {
        Vector3i rotation{0, 0, -4096};
        Vector3 position{0, 0.00079999998f, 0};
        EV_scenApplyPosOrRotToPartOf(machineCharacter, 152, nullptr, &rotation, EventCharacterPartTransformMode::Relative);
        EV_scenApplyPosOrRotToPartOf(machineCharacter, 153, &position, nullptr, EventCharacterPartTransformMode::Relative);
    }
    if (coinTurnFrames > 143 - motionStartFrame) {
        Vector3 position{0, -0.0063999998f, 0};
        EV_scenApplyPosOrRotToPartOf(machineCharacter, 153, &position, nullptr, EventCharacterPartTransformMode::Relative);
        coinTurnFrames = 0;
        HLib::DestroyCurrentTask();
    }
    ++coinTurnFrames;
}
// @verified - p
void GachSecondTurnCallback(void *) {
    if (secondTurnFrames > 155 - motionStartFrame) {
        Vector3i rotation{0, 0, -4096};
        Vector3 position{0, 0.00079999998f, 0};
        EV_scenApplyPosOrRotToPartOf(machineCharacter, 152, nullptr, &rotation, EventCharacterPartTransformMode::Relative);
        EV_scenApplyPosOrRotToPartOf(machineCharacter, 153, &position, nullptr, EventCharacterPartTransformMode::Relative);
    }
    if (secondTurnFrames > 162 - motionStartFrame) {
        Vector3 position{0, -0.0063999998f, 0};
        EV_scenApplyPosOrRotToPartOf(machineCharacter, 153, &position, nullptr, EventCharacterPartTransformMode::Relative);
        secondTurnFrames = 0;
        HLib::DestroyCurrentTask();
    }
    ++secondTurnFrames;
}
// @verified - p
void GachCapsuleFallCallback(void *) {
    const shen_name capsule = gachCapsules[capsuleColour];
    if (capsuleFallFrames == 160 - motionStartFrame) {
        Place(capsule, GACH_BASE, {0.1f, 0.5f, 0.2f});
        EV_EnableCharacterDisplay(capsule);
    }
    if (capsuleFallFrames > 160 - motionStartFrame) {
        Vector3 position;
        Vector3i rotation;
        EV_GetCharaPosition(capsule, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        position.y -= gachFallSamples[motionStartFrame - 161 + capsuleFallFrames];
        EV_AT_SetPos(capsule, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
        GetScenarioCharacterEulerAngles(capsule, &rotation, EventTransformFlags::World);
        rotation.x += 910;
        rotation.y -= 1820;
        rotation.z -= 3640;
        EV_AT_SetAngY(capsule, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    }
    if (capsuleFallFrames > 180 - motionStartFrame) {
        capsuleFallFrames = 0;
        HLib::DestroyCurrentTask();
    }
    ++capsuleFallFrames;
}
// @verified - p
void GachShellFallCallback(void *) {
    if (shellFallFrames > 255 - motionStartFrame) {
        Vector3 position;
        Vector3i rotation;
        EV_GetCharaPosition(GACH_SHELL, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        shellFallStep += 0.0040000002f;
        position.y -= shellFallStep;
        EV_AT_SetPos(GACH_SHELL, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
        GetScenarioCharacterEulerAngles(GACH_SHELL, &rotation, EventTransformFlags::World);
        rotation.x += 2730;
        rotation.y -= 1820;
        rotation.z -= 910;
        EV_AT_SetAngY(GACH_SHELL, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    }
    if (shellFallFrames > 305 - motionStartFrame) {
        shellFallStep = 0.003f;
        shellFallFrames = 0;
        HLib::DestroyCurrentTask();
    }
    ++shellFallFrames;
}
// @verified - p
void GachColourFallCallback(void *) {
    const shen_name colour = gachColours[capsuleColour];
    if (colourFallFrames > 303 - motionStartFrame) {
        Vector3 position;
        Vector3i rotation;
        EV_GetCharaPosition(colour, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        colourFallStep += 0.0040000002f;
        position.y -= colourFallStep;
        EV_AT_SetPos(colour, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
        GetScenarioCharacterEulerAngles(colour, &rotation, EventTransformFlags::World);
        rotation.x += 910;
        rotation.y += 1820;
        rotation.z += 3640;
        EV_AT_SetAngY(colour, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    }
    if (colourFallFrames > 353 - motionStartFrame) {
        colourFallStep = 0.003f;
        colourFallFrames = 0;
        HLib::DestroyCurrentTask();
    }
    ++colourFallFrames;
}
// @verified - p
void GachRightHandCallback(void *) {
    if (rightHandFrames > 125 - motionStartFrame) {
        const Vector3i rotation{};
        ApplyCharacterHandRotation(ctrlGetCharaByName(GACH_RYO), 0, 21, &rotation);
        Pose(GACH_RYO, gachTurnHandPose, 6);
        rightHandFrames = 0;
        HLib::DestroyCurrentTask();
    }
    ++rightHandFrames;
}
// @verified - p
void GachBothHandsCallback(void *) {
    if (leftHandFrames > 193 - motionStartFrame) {
        const Vector3i right{0, 3276, 63716}, left{0, 0, 2730};
        ApplyCharacterHandRotation(ctrlGetCharaByName(GACH_RYO), 0, 21, &right);
        ApplyCharacterHandRotation(ctrlGetCharaByName(GACH_RYO), 1, 21, &left);
        leftHandFrames = 0;
        HLib::DestroyCurrentTask();
    }
    ++leftHandFrames;
}
// @verified - p
void GachFadeInCallback(void *) {
    SetScenarioCharacterTransparency(GACH_RYO, fadeInFrames / 30);
    if (fadeInFrames > 30) {
        SetScenarioCharacterTransparency(GACH_RYO, 1);
        fadeInFrames = 0;
        HLib::DestroyCurrentTask();
    }
    fadeInFrames += 1;
}
// @verified - p
void GachFadeOutCallback(void *) {
    SetScenarioCharacterTransparency(GACH_RYO, (40 - fadeOutFrames) / 40);
    if (fadeOutFrames > 40) {
        SetScenarioCharacterTransparency(GACH_RYO, 0);
        fadeOutFrames = 0;
        HLib::DestroyCurrentTask();
    }
    fadeOutFrames += 1;
}
void QueueGachPurchase(GachTaskWork *work) {
    SetScenarioCharacterTransparency(GACH_RYO, 1);
    mtwkSetRequestedRootMode(scenFindMotionwork(GACH_RYO), 0);
    if (work->status) {
        SetScenarioCharacterTransparency(GACH_RYO, 1);
        EVS_LoopCamera(3020, GACH_BASE, GACH_BASE);
        Place(GACH_RYO, GACH_BASE, {-0.13694000244140625f, 0, 0.8783109784126282f});
        Turn(GACH_RYO, GACH_BASE, 0);
        EVT_AT_SetMot(GACH_RYO, 0x1883, 99, 1, 0x80000000, 0);
        motionStartFrame = 99;
    } else {
        Speak(work, 3);
        EVS_LoopCamera(3010, GACH_BASE, GACH_BASE);
        Place(GACH_RYO, GACH_BASE, {-0.05f, 0, 0.71f});
        Turn(GACH_RYO, GACH_BASE, 0);
        EVT_AT_SetMot(GACH_RYO, 0x1883, 0, 1, 0x80000000, 0);
        motionStartFrame = 0;
        work->status = 1;
    }
    camCtrlDisableFilmBars();
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(GACH_RYO), Scheme, 10, 1);
    PATI_StartMoneyCount(0xfffffffb, 10);
    UpdateMotionWorkWithPosition(scenFindMotionwork(GACH_RYO), nullptr, 0);
    const std::uint32_t sounds[]{327, 621, 621, 624, 623, 622, 906, 906};
    const std::int32_t times[]{115, 134, 157, 174, 189, 228, 267, 319};
    for (std::int32_t i = 0; i < 8; ++i)
        EVT_AT_SetSound(sounds[i], times[i] - motionStartFrame);
    EVT_AT_SetCam(3030, GACH_BASE, GACH_BASE, 0, 0, 195 - motionStartFrame);
    capsuleColour = static_cast<std::int32_t>(GetRandomNumberInRange(4));
    work->capsule = static_cast<std::uint8_t>(capsuleColour);
    for (shen_name name : gachCapsules) {
        EV_AT_SetPos(name, EventTransformFlags::AllAxes, {0.1f, 0.5f, 0.2f});
        EV_DisableCharacterDisplay(name);
    }
    const Vector3i wrist{0, 0xa0f4, 0};
    ApplyCharacterHandRotation(ctrlGetCharaByName(GACH_RYO), 0, 21, &wrist);
    EV_DisableCharacterDisplay(gachColours[work->capsule]);
    EV_DisableCharacterDisplay(Prize(work).object);
    EV_DisableCharacterDisplay(GACH_SHELL);
    Cling(GACH_COIN, GACH_RYO, 14, {0.139f, 0.033f, -0.031f}, {16384, 12743, 0}, 0);
    QueuePose(gachCoinHandPose, 10, 112 - motionStartFrame);
    Uncling(GACH_COIN, 112 - motionStartFrame);
    Display(GACH_COIN, 112 - motionStartFrame, 1);
    Queue(GachRightHandCallback);
    Queue(GachBothHandsCallback);
    QueuePose(gachCoinHandPose, 6, 138 - motionStartFrame);
    QueuePose(gachTurnHandPose, 6, 148 - motionStartFrame);
    QueuePose(gachOpenShellPose, 20, 164 - motionStartFrame);
    Queue(GachFirstTurnCallback);
    Queue(GachSecondTurnCallback);
    Queue(GachCapsuleFallCallback);
    Queue(GachShellFallCallback);
    Queue(GachColourFallCallback);
    Cling(GACH_SHELL, GACH_RYO, 14, {0.094f, 0.008f, -0.056f}, {16384, 0, 0}, 195 - motionStartFrame);
    Cling(gachColours[work->capsule], GACH_RYO, 14, {0.094f, 0.008f, -0.056f}, {16384, 0, 0}, 195 - motionStartFrame);
    Display(gachCapsules[capsuleColour], 195 - motionStartFrame, 1);
    Display(GACH_SHELL, 195 - motionStartFrame, 0);
    Display(gachColours[work->capsule], 195 - motionStartFrame, 0);
    Cling(GACH_SHELL, GACH_RYO, 14, {0.094f, 0.008f, -0.056f}, {16384, 0, 0}, 225 - motionStartFrame);
    Cling(gachColours[work->capsule], GACH_RYO, 21, {0.089f, 0.042f, 0.026f}, {16384, 0, 0}, 225 - motionStartFrame);
    Uncling(GACH_SHELL, 257 - motionStartFrame);
    QueuePose(gachPrizeHandPose, 100, 257 - motionStartFrame);
    const Vector3 scale{handRecord.scale, handRecord.scale, handRecord.scale};
    mtwkSetMotionworkScale3(Prize(work).object, &scale);
    Cling(Prize(work).object, GACH_RYO, 14, {handRecord.position.x + 0.117f, handRecord.position.y + 0.017000001f, handRecord.position.z - 0.064999998f},
        {handRecord.rotation.x, handRecord.rotation.y + 32768, handRecord.rotation.z + 49152}, 277 - motionStartFrame);
    Display(Prize(work).object, 282 - motionStartFrame, 0);
    work->state = 10;
    if (motionStartFrame != 330)
        HLib::SetupTaskDelay(static_cast<std::uint16_t>(330 - motionStartFrame));
}
void BeginPrizeView(GachTaskWork *work) {
    using enum CameraState;
    Uncling(gachColours[work->capsule], 0);
    Uncling(Prize(work).object, 0);
    SpeakPrize(work);
    EV_DisableCharacterDisplay(GACH_RYO);
    SetLowQualityHands(ctrlGetCharaByName(GACH_HAND));
    SetHighModelXYZ(ctrlGetCharaByName(GACH_HAND), 1, 0, 0);
    EV_EnableCharacterDisplay(GACH_HAND);
    Place(GACH_HAND, GACH_BASE, {0.09f, 1.06f, 0.85f});
    EVT_AT_SetAng_Enqueue(GACH_HAND, GACH_BASE, 0, 0, 0, 0);
    EV_EnsureCharacterDraw(GACH_HAND);
    AT_SetLocalBaseChara(GACH_HAND_BASE, GACH_HAND);
    EV_AT_SetPos(GACH_HAND_BASE, EventTransformFlags::AllAxes, {0.1816f, -0.1991f, -0.35859999f});
    EV_AT_SetAngY(GACH_HAND_BASE, EventTransformFlags::AllAxes, {65327, 14902, 3045});
    AT_SetLocalBaseChara(Prize(work).object, GACH_HAND_BASE);
    SetScenarioCharacterTransparency(Prize(work).object, 1);
    EV_AT_SetPos(
        Prize(work).object, EventTransformFlags::AllAxes, {handRecord.position.x + 0.112f, handRecord.position.y + 0.046f, handRecord.position.z + 0.015f});
    EV_AT_SetAngY(
        Prize(work).object, EventTransformFlags::AllAxes, {handRecord.rotation.x + 4551, handRecord.rotation.y + 45511, handRecord.rotation.z + 50972});
    const Vector3i wrist{12743, 0, 0};
    ApplyCharacterHandRotation(ctrlGetCharaByName(GACH_HAND), 0, 21, &wrist);
    Pose(GACH_HAND, gachPrizeHandPose, 0);
    AT_SetLocalBaseChara(GACH_TARGET, Prize(work).object);
    work->viewOffset = {};
    EV_AT_SetPos(GACH_TARGET, EventTransformFlags::AllAxes, work->viewOffset);
    EV_AT_SetAngY(GACH_TARGET, EventTransformFlags::AllAxes, {});
    Vector3 position;
    EV_GetCharaPosition(GACH_TARGET, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    camCtrlSetTargetPosition(position);
    AT_SetLocalBaseChara(GACH_CAMERA, GACH_BASE);
    EV_AT_SetPos(GACH_CAMERA, EventTransformFlags::AllAxes, {0.1785f, 1.1422f, 0.55379999f});
    EV_GetCharaPosition(GACH_CAMERA, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    camCtrlSetNextPosition(position);
    camCtrlSetState(Event);
    work->savedFov = camCtrlGetPerspective();
    work->currentFov = 9102;
    work->state = 11;
    work->viewFrames = 0;
    work->handTurn = 0;
    work->secret = 0;
    AddInventoryItem(Prize(work).item, 1, 1);
    HLib::SetCurrentTaskTo(GachItemViewCallback);
}
// @verified - p
void UpdateGachHandSway(GachTaskWork *work) {
    Vector3 position;
    EV_GetCharaPosition(GACH_HAND, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    work->sway[0] += 364;
    work->sway[1] += 546;
    work->sway[2] += 728;
    position.x += sin_fixed16(static_cast<std::int16_t>(work->sway[0])) * 0.000049999999f;
    position.y += sin_fixed16(static_cast<std::int16_t>(work->sway[1])) * 0.000049999999f;
    position.z += sin_fixed16(static_cast<std::int16_t>(work->sway[2])) * 0.000049999999f;
    EV_AT_SetPos(GACH_HAND, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
}
} // namespace
// @verified - p
void GachLocalDebugNullsub() {
}
// @verified - p
void InitGach() {
    std::memset(originalPartPosition, 0, sizeof(originalPartPosition));
    std::memset(originalPartRotation, 0, sizeof(originalPartRotation));
    machineCharacter = {};
    capsuleColour = 0;
    motionStartFrame = 0;
    handRecord = {};
    handRecords = nullptr;
    std::memset(prizeRarityCounts, 0, sizeof(prizeRarityCounts));
    coinTurnFrames = 0;
    secondTurnFrames = 0;
    capsuleFallFrames = 0;
    shellFallFrames = 0;
    colourFallFrames = 0;
    shellFallStep = 0.003f;
    colourFallStep = 0.003f;
    rightHandFrames = 0;
    leftHandFrames = 0;
    fadeInFrames = 0;
    fadeOutFrames = 0;
    prizeRandom = 0;
}
void QueueGach(std::uintptr_t parameters) {
    const GachEnqueueParams *const request = reinterpret_cast<const GachEnqueueParams *>(parameters);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(GachMainCallback, 5, 0x0b, sizeof(GachTaskWork), GACH_TASK);
    GachTaskWork *const work = static_cast<GachTaskWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->machine = request->machine;
    work->state = 1;
    work->set = request->set;
    work->debugView = 0;
    work->previousItem = 0;
    SetPlayerControlEnabled(0);
    for (shen_name name : {GACH_BASE, GACH_TARGET, GACH_HAND_BASE, GACH_CAMERA})
        CreateEventCharacter(CharacterCreationHighModel, nullptr, name);
    EV_EnableCharacterDisplay(GACH_HAND_BASE);
    EV_EnsureCharacterDraw(GACH_HAND_BASE);
    SetScenarioCharacterTransparency(GACH_HAND_BASE, 1);
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(work->machine, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(work->machine, &rotation, EventTransformFlags::Local);
    EV_AT_SetAngY(GACH_BASE, EventTransformFlags::AllAxes, rotation);
    EV_AT_SetPos(GACH_BASE, EventTransformFlags::AllAxes, position);
    EV_scenApplyPosOrRotToPartOf(work->machine, 152, &originalPartPosition[0], &originalPartRotation[0], EventCharacterPartTransformMode::Get);
    EV_scenApplyPosOrRotToPartOf(work->machine, 153, &originalPartPosition[1], &originalPartRotation[1], EventCharacterPartTransformMode::Get);
    for (shen_name name : controllerNames) {
        EV_SetCharaControllerFlags(name, ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(name, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    }
    EV_SetNewSceneIndex();
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("GACH"), 0);
    EV_FreeAuthorPak(1, "0108");
    EV_LoadCutsceneFromCurrentScene("01080000.smp");
}
// @verified - p
void SelectGachPrize(GachTaskWork *work) {
    const float random = GetRandomNumberInRange(1);
    work->rarity = 3;
    prizeRandom = random;
    prizeRarityCounts[5] += 1;
    if (random >= 0.02f) {
        const float reduced = random - 0.02f;
        prizeRandom = reduced;
        if (reduced >= 0.77999997f) {
            work->rarity = 3;
            prizeRandom = reduced - 0.77999997f;
            prizeRarityCounts[3] += 1;
            if (prizeRandom < 0.066f) {
                work->rarity = 4;
                prizeRarityCounts[3] -= 1;
                prizeRarityCounts[4] += 1;
            }
        } else {
            work->rarity = 2;
            prizeRarityCounts[2] += 1;
        }
    } else {
        work->rarity = 1;
        prizeRarityCounts[1] += 1;
        if (random < 0.0020000001f) {
            work->rarity = 0;
            prizeRarityCounts[1] -= 1;
            prizeRarityCounts[0] += 1;
        }
    }
    const std::uint16_t *const choices = gachSets[work->set].tiers[work->rarity];
    std::uint8_t count = 0;
    if (choices[0]) {
        do {
            if (count == 255) {
                work->selection = static_cast<std::uint16_t>(shenGetRandomInt32(0));
                work->selection = static_cast<std::uint16_t>(shenGetRandomInt32(0));
                return;
            }
            ++count;
        } while (choices[count]);
    }
    if (!count) {
        if (!work->rarity)
            work->rarity = 1;
        else if (work->rarity == 4)
            work->rarity = 3;
    }
    work->selection = static_cast<std::uint16_t>(shenGetRandomInt32(count));
    work->selection = static_cast<std::uint16_t>(shenGetRandomInt32(count));
}
// @verified - p
void LoadGachPrizeHand(GachTaskWork *work) {
    if (Package(5, Prize(work).archive))
        return;
    char directory[32]{};
    std::snprintf(directory, sizeof(directory), "scene/0%c/pack/gacha", static_cast<char>('0' + g_actNumber));
    Package(0, directory, Prize(work).archive);
    handRecord = {};
    handRecord.scale = 0.30000001f;
    handRecord = handRecords[gachSets[work->set].tiers[work->rarity][work->selection]];
    std::memcpy(gachPrizeHandPose, handRecord.pose, sizeof(handRecord.pose));
    Package(7);
}
// @verified - p
void GachMainCallback(void *data) {
    using enum CameraState;
    GachTaskWork *const work = static_cast<GachTaskWork *>(data);
    Vector3 position;
    Vector3i rotation;
    switch (work->state) {
    case 1: {
        EV_EventSetUpHelpIcon_0(6, 0);
        const char *const archive = MachineArchive(work->machine);
        if (!Package(5, archive)) {
            Package(0, Filepath_Gen(FilepathGenMode::PackDirectory), archive);
            GachLocalDebugNullsub();
            handRecords = static_cast<const GachHandRecord *>(LoadCurrentPakBinary("gachahand0.bin"));
            GachLocalDebugNullsub();
            handRecord = {};
            handRecord.scale = 0.30000001f;
        }
        Package(7);
        EV_DisableCharacterDisplay(work->machine);
        EV_GetCharaPosition(work->machine, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        GetScenarioCharacterEulerAngles(work->machine, &rotation, EventTransformFlags::World);
        EV_AT_SetPos(GACH_MACHINE, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
        EV_AT_SetAngY(GACH_MACHINE, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
        EV_EnableCharacterDisplay(GACH_MACHINE);
        work->character = GACH_MACHINE;
        work->state = 2;
        machineCharacter = GACH_MACHINE;
        break;
    }
    case 2: {
        work->status = 0;
        const Vector4 reflector{1, 0.30000001f, 0.30000001f, 0.30000001f};
        SetScenarioReflectorBoardValues(GACH_RYO, &reflector);
        EV_SetRefBoardParamsFor(GACH_RYO, 1);
        EV_SetRefBoardParamsFor(GACH_RYO, 1);
        SetLowQualityHands(ctrlGetCharaByName(GACH_RYO));
        SetHighModelXYZ(ctrlGetCharaByName(GACH_RYO), 1, 0, 0);
        SetHighModelXYZ(ctrlGetCharaByName(GACH_RYO), 1, 1, 0);
        Pose(GACH_RYO, gachInitialHandPose, 50);
        (void)FindMotionWork(ctrlGetCharaByName(GACH_RYO));
        const EventFilmControl film{0, 30, 255, 0, 0, 0, 0, 0, 0, 0, 0};
        EVT_AT_MenCtrl(film);
        Place(GACH_RYO, GACH_BASE, {-0.05f, 0, 0.71f});
        Turn(GACH_RYO, GACH_BASE, 0);
        EVT_AT_SetMot(GACH_RYO, 0xf002, 0, 1, 0x40000000, 0);
        Queue(GachFadeOutCallback);
        EVS_LoopCamera(3000, GACH_BASE, GACH_BASE);
        camCtrlDisableFilmBars();
        work->state = 4;
        HLib::SetupTaskDelay(20);
        break;
    }
    case 4:
        EV_GetCharaPosition(GACH_BASE, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        position.y += 0.80000001f;
        UpdateMotionWorkWithPosition(scenFindMotionwork(GACH_RYO), &position, 1);
        Speak(work, 0);
        work->state = 5;
        HLib::SetupTaskDelay(50);
        break;
    case 5:
        if (shenGetMinutesUntilNextFrameMinute()) {
            std::int16_t choices[]{-1, -1, 40, 41};
            SetupControlSpeakAndUI(choices, 200, 0, 2);
            work->state = 6;
            HLib::SetupTaskDelay(20);
        } else {
            Speak(work, 4);
            BeginExit(work);
            Place(GACH_RYO, GACH_BASE, {-0.05f, 0, 0.71f});
            Turn(GACH_RYO, GACH_BASE, 0);
            mtwkRequestMotion(scenFindMotionwork(GACH_RYO), 0x1884, MotionRegion::LowerBody);
            HLib::SetupTaskDelay(70);
        }
        break;
    case 6:
        if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            break;
        if (shenGetCtrlSpeakIconFlag() != 40 || static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) < 5) {
            Speak(work, shenGetCtrlSpeakIconFlag() != 40 ? 4 : 2);
            BeginExit(work);
            if (work->status) {
                mtwkRequestMotion(scenFindMotionwork(GACH_RYO), 0x1884, MotionRegion::LowerBody);
                EVS_LoopCamera(3060, GACH_BASE, GACH_BASE);
                camCtrlDisableFilmBars();
                HLib::SetupTaskDelay(70);
            } else {
                Queue(GachFadeInCallback);
                camCtrlSetFovTransitionTo(false);
                camCtrlSetState(Init);
                camCtrlResetTransition(CameraInterpolationMode::RestorePrevious);
                camCtrlSetInterpEaseFactor(0.1f);
                HLib::SetupTaskDelay(40);
            }
        } else {
            work->state = 7;
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 551, 1);
            if (work->secret == 9)
                work->set = 11;
            SelectGachPrize(work);
        }
        break;
    case 7:
    case 8:
        LoadGachPrizeHand(work);
        work->state = 9;
        break;
    case 9:
        QueueGachPurchase(work);
        break;
    case 10:
        BeginPrizeView(work);
        break;
    case 11: {
        const Vector3i wrist{};
        ApplyCharacterHandRotation(ctrlGetCharaByName(GACH_RYO), 0, 21, &wrist);
        EV_GetCharaPosition(GACH_BASE, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        position.y += 0.69999999f;
        UpdateMotionWorkWithPosition(scenFindMotionwork(GACH_RYO), &position, 1);
        EVS_LoopCamera(3050, GACH_BASE, GACH_BASE);
        mtwkRequestMotion(scenFindMotionwork(GACH_RYO), 0x1885, MotionRegion::LowerBody);
        camCtrlDisableFilmBars();
        EV_EnableCharacterDisplay(GACH_RYO);
        Package(6, Prize(work).archive);
        work->state = 5;
        break;
    }
    case 12:
        SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(GACH_RYO), Normal, 0, 1);
        EV_CheckWarmStartNpcRoute(GACH_EXIT_ROUTE);
        HLib::SetCurrentTaskTo(GachExitCallback);
        break;
    default:
        break;
    }
}
// @verified - p
void GachItemViewCallback(void *data) {
    GachTaskWork *const work = static_cast<GachTaskWork *>(data);
    const std::int32_t x = static_cast<std::uint16_t>(GetCurrentControlInput(INPUT_LEFT_X)),
                       y = static_cast<std::uint16_t>(GetCurrentControlInput(INPUT_LEFT_Y));
    const std::int32_t rightY = static_cast<std::uint16_t>(GetCurrentControlInput(INPUT_LEFT_TRIGGER)),
                       rightX = static_cast<std::uint16_t>(GetCurrentControlInput(INPUT_RIGHT_TRIGGER));
    const std::int16_t held = static_cast<std::int16_t>(GetCurrentControlInput(INPUT_HELD)),
                       pressed = static_cast<std::int16_t>(GetCurrentControlInput(INPUT_NEW_PRESSES));
    EV_EnsureCharacterDraw(GACH_HAND);
    ++work->viewFrames;
    Vector3i rotation;
    if (held & 0x9000) {
        GetScenarioCharacterEulerAngles(GACH_HAND, &rotation, EventTransformFlags::Local);
        if (work->handTurn < 10) {
            rotation.y -= 36;
            ++work->handTurn;
            EV_AT_SetAngY(GACH_HAND, EventTransformFlags::AllAxes, rotation);
            GetCharacterHandRotation(ctrlGetCharaByName(GACH_HAND), 0, &rotation);
            rotation.x += 728;
            ApplyCharacterHandRotation(ctrlGetCharaByName(GACH_HAND), 0, 21, &rotation);
            GetScenarioCharacterEulerAngles(GACH_HAND_BASE, &rotation, EventTransformFlags::Local);
            rotation.x += 728;
            EV_AT_SetAngY(GACH_HAND_BASE, EventTransformFlags::AllAxes, rotation);
        }
    }
    if (held & 0x6000) {
        GetScenarioCharacterEulerAngles(GACH_HAND, &rotation, EventTransformFlags::Local);
        if (work->handTurn > -18) {
            rotation.y += 36;
            --work->handTurn;
            EV_AT_SetAngY(GACH_HAND, EventTransformFlags::AllAxes, rotation);
            GetCharacterHandRotation(ctrlGetCharaByName(GACH_HAND), 0, &rotation);
            rotation.x -= 728;
            ApplyCharacterHandRotation(ctrlGetCharaByName(GACH_HAND), 0, 21, &rotation);
            GetScenarioCharacterEulerAngles(GACH_HAND_BASE, &rotation, EventTransformFlags::Local);
            rotation.x -= 728;
            EV_AT_SetAngY(GACH_HAND_BASE, EventTransformFlags::AllAxes, rotation);
        }
    }
    work->viewOffset.z = 0;
    const float stickX = x - 128.0f, stickY = (y - 128.0f) / 5000;
    work->viewOffset.x = ((stickX / 5000 - stickY - work->viewOffset.x) * 0.125f) + work->viewOffset.x;
    work->viewOffset.y = ((stickX / -5000 - stickY - work->viewOffset.y) * 0.125f) + work->viewOffset.y;
    EV_AT_SetPos(GACH_TARGET, EventTransformFlags::AllAxes, work->viewOffset);
    Vector3 position;
    EV_GetCharaPosition(GACH_TARGET, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    camCtrlSetTargetPosition(position);
    const std::int32_t verticalFov = static_cast<std::int32_t>((((static_cast<float>(rightY) * 20.0f) / 255.0f) * 8192.0f) / 45.0f);
    const std::int32_t horizontalFov = static_cast<std::int32_t>((((static_cast<float>(rightX) * 30.0f) / 255.0f) * 8192.0f) / 45.0f);
    work->targetFov = static_cast<std::uint16_t>(verticalFov - horizontalFov + 9102);
    const std::int32_t currentFov = static_cast<std::int16_t>(work->currentFov);
    const std::int32_t delta = static_cast<std::int16_t>(work->targetFov) - currentFov;
    work->currentFov = static_cast<std::uint16_t>(currentFov - static_cast<std::int32_t>(static_cast<float>(delta) * -0.0625f));
    camCtrlSetPerspective(work->currentFov);
    if (work->debugView != 1 && work->viewFrames >= 10) {
        GetBattleDataConfig(0, GACH_VIEW_BUTTON);
        if (GetBattleDataConfig(1, GACH_VIEW_BUTTON) && GetBattleDataConfig(2, GACH_VIEW_BUTTON)) {
            camCtrlSetPerspective(work->savedFov);
            HLib::SetCurrentTaskTo(GachHandReturnCallback);
            work->returnFrames = 0;
        }
    }
    if (!shenGetMinutesUntilNextFrameMinute()) {
        camCtrlSetPerspective(work->savedFov);
        HLib::SetCurrentTaskTo(GachHandReturnCallback);
        work->returnFrames = 0;
    }
    if (work->viewFrames >= 10000) {
        camCtrlSetPerspective(work->savedFov);
        HLib::SetCurrentTaskTo(GachHandReturnCallback);
        work->viewFrames = 0;
        work->returnFrames = 0;
    }
    UpdateGachHandSway(work);
    if (work->machine.name == SHEN_NAME(GA51)) {
        if ((pressed & 0x2000) && work->secret == 0)
            work->secret = 1;
        if ((pressed & 0x1000) && work->secret == 2)
            work->secret = 3;
        if (pressed < 0 && work->secret == 4)
            work->secret = 5;
        if ((pressed & 0x400) && work->secret == 6)
            work->secret = 7;
        if ((pressed & 0x4000) && work->secret == 8)
            work->secret = 9;
        if (pressed) {
            if (!(pressed & 0x1000) && work->secret == 2)
                work->secret = 0;
            if (pressed >= 0 && work->secret == 4)
                work->secret = 0;
            if (!(pressed & 0x400) && work->secret == 6)
                work->secret = 0;
            if (!(pressed & 0x4000) && work->secret == 8)
                work->secret = 0;
        } else {
            if (work->secret == 1)
                work->secret = 2;
            if (work->secret == 3)
                work->secret = 4;
            if (work->secret == 5)
                work->secret = 6;
            if (work->secret == 7)
                work->secret = 8;
        }
    }
}
// @verified - p
void GachHandReturnCallback(void *data) {
    GachTaskWork *const work = static_cast<GachTaskWork *>(data);
    if (!work->returnFrames) {
        Speak(work, 1);
        AT_SetLocalBaseChara(GACH_HAND, GACH_BASE);
    }
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(GACH_HAND, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    position.y -= 0.0020000001f;
    position.z += 0.012f;
    EV_AT_SetPos(GACH_HAND, EventTransformFlags::AllAxes, position);
    GetCharacterHandRotation(ctrlGetCharaByName(GACH_HAND), 0, &rotation);
    rotation.x -= 910;
    ApplyCharacterHandRotation(ctrlGetCharaByName(GACH_HAND), 0, 21, &rotation);
    GetScenarioCharacterEulerAngles(GACH_HAND_BASE, &rotation, EventTransformFlags::Local);
    rotation.x -= 910;
    EV_AT_SetAngY(GACH_HAND_BASE, EventTransformFlags::AllAxes, rotation);
    if (work->returnFrames > 30) {
        SetCharacterHandSecondaryPoseWork(ctrlGetCharaByName(GACH_HAND), 0, nullptr);
        AT_SetLocalBaseChara(GACH_HAND, EVENT_CHARACTER_INVALID);
        AT_SetLocalBaseChara(GACH_TARGET, EVENT_CHARACTER_INVALID);
        EV_DisableCharacterDisplay(GACH_HAND);
        EV_AT_SetPos(Prize(work).object, EventTransformFlags::AllAxes, {0, -100, 0});
        HLib::SetCurrentTaskTo(GachMainCallback);
    }
    ++work->returnFrames;
}
void GachExitCallback(void *data) {
    using enum CameraState;
    GachTaskWork *const work = static_cast<GachTaskWork *>(data);
    if (!EV_ForceLoadNpc(GACH_EXIT_ROUTE))
        return;
    Package(6, MachineArchive(work->machine));
    EV_EnableCharacterDisplay(work->machine);
    EV_FreeAuthorPak(0, "0108");
    EV_EnableCharacterDisplay(GACH_RYO);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 551, 0);
    LoadCutsceneAndSoundProgramForArea();
    hndlSetFlags(ctrlGetCharaByName(GACH_RYO));
    hndlSetFlags(ctrlGetCharaByName(GACH_HAND));
    (void)FindMotionWork(ctrlGetCharaByName(GACH_RYO));
    HLib::ResolveDeferredCallback(HLib::GetCurrentParentTask());
    for (shen_name name : {GACH_BASE, GACH_TARGET, GACH_HAND_BASE, GACH_CAMERA})
        SetScenarioCharacterKillMode(name, 1);
    EV_SetCharaControllerFlags(GACH_RYO, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(GACH_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    camCtrlSetState(Init);
    SetPlayerControlEnabled(1);
    UpdateMotionWorkWithPosition(scenFindMotionwork(GACH_RYO), nullptr, 0);
    EV_EventSetUpHelpIcon_0(0, 0);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->task);
    EV_NPC_NewLoadSw(1);
    EV_npcLockUnlock(GACH_EXIT_ROUTE);
    Package(6, "GACH");
}
const ScriptAreaDefinition gachScriptDefinition{.areaId = GACH_MACHINE, .field04 = 0, .init = InitGach, .main = QueueGach};
} // namespace shenmue::s2
