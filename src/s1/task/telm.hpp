#pragma once

#include "s1/hlib.hpp"
#include "common/math.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct Character;
struct HandModelWork;
struct InputSlot;
struct Mt5RuntimeNode;
struct ScenarioContext;

#pragma pack(push, 1)


struct TelephoneUiData {
    void *cordBinding;
    float segmentLength;
    std::int32_t segmentCount;
    float swayAccumulator;
    Vector3 receiverRotatedPosition;
    Vector3 characterRotatedPosition;
    Vector3 previousMidpoint;
    Vector3 midpointBias;
    std::int32_t arcAngle;
    float arcRadius;
    float arcOffset;
};

// One two-key FCV channel as written by AllocSPLI_Wrapper_0 and authInitAndStartSequence.
struct FcvSplineChannel {
    std::int32_t keyCount;
    float times[2];
    float values[2];
    float slopes[2];
};


struct FcvSplineReader {
    std::int32_t keyCount;
    std::int32_t keyOffset;
    std::int32_t segmentKeyCount;
    std::int32_t reserved0c;
    float *times;
    float *values;
    float *slopes;
    float *coefficients;
};



struct TelmHandSequence {
    Character *target;
    HandModelWork *hand;
    std::int32_t state;
    std::int32_t reserved14;
    float frame;
    std::int32_t reverse;
    std::int32_t finished;
    std::int32_t reserved24;
    std::int32_t sequenceId;
    float startFrame;
    float endFrame;
    float frameStep;
    float blendFrames;
    float restartBlendFrames;
    std::int32_t blendPending;
    std::int32_t loop;
    std::int32_t cuesEnabled;
    std::int32_t firstCuePlayed;
    std::int32_t secondCuePlayed;
    std::int32_t positionDone[3];
    std::int32_t rotationDone[3];
    float frameCounter;
    FcvSplineChannel positionChannels[3];
    FcvSplineChannel rotationChannels[3];
    FcvSplineReader positionReaders[3];
    FcvSplineReader rotationReaders[3];
    std::byte *coefficientPool;
};

// The in-place ASEQ chunk built by authInitAndStartSequence: camera-origin XYZ, look-at XYZ, Z-angle and field of view.
struct TelephoneAuthSequence {
    std::uint32_t flags;
    FcvSplineChannel channels[8];
};


struct TelephoneWork {
    std::int32_t active;
    std::int32_t motionFlag0Enabled;
    std::int32_t internalFlag8;
    std::int32_t dialDisplayDirty;
    std::uint32_t field010;
    std::uint32_t reserved014;
    ScenarioContext *scriptContext;
    InputSlot *padInput;
    std::int32_t cleanup;
    std::int32_t coinChargeApplied;
    std::uint32_t pauseCountdown;
    std::int32_t audioResetLatched;
    std::int32_t mainRanLastFrame;
    std::byte reserved03c[0x58];
    std::int32_t helpTokenId;
    std::uint8_t savedHelpTokenId;
    std::byte reserved099[3];
    std::uint8_t savedButtonId;
    std::byte reserved09d[3];
    std::int32_t phoneVariant;
    std::int32_t savedCpctAnimState;
    std::int32_t playerOwnedFrame;
    std::uint32_t reserved0ac;
    HLib::Task *scenarioTask;
    Mt5RuntimeNode *hmdl;
    std::byte reserved0c0[8];
    std::int32_t initialDialYaw;
    float receiverBaseY;
    float receiverLiftedY;
    std::uint32_t reserved0d4;
    Character *currentPeerNode;
    std::int32_t currentPeerMode;
    std::uint32_t reserved0e4;
    Character *ownerTaskData;
    Character *ownerNode;
    Character *cameraNode;
    Character *receiverNode;
    Vector3i savedOwnerRot;
    Vector3i savedCameraRot;
    std::uint32_t handMotionTaskData;
    Vector3 ownerSplinePosition;
    Vector3 dialHomePosition;
    Vector3 dialFollowPosition;
    Vector3 numberDisplayPosition;
    Vector3 digitPopupPosition;
    Vector3 alternateReceiverPosition;
    std::byte reserved16c[0x18];
    std::int32_t state;
    std::int32_t stateFrame;
    std::int32_t previousFrameState;
    std::int32_t previousState;
    std::int32_t ringAudioPhase;
    float ringAudioTimer;
    std::int32_t selectionIdleTimer;
    std::int32_t stateTimer;
    std::int32_t dialTargetYawVelocity;
    std::int32_t dialTargetDigitDelta;
    std::int32_t dialTargetYaw;
    std::int32_t dialYawVelocity;
    std::int32_t dialDigitIndex;
    std::int32_t savedDialDigitIndex;
    std::int32_t dialCurrentYaw;
    std::int32_t digitDisplaySideToggle;
    std::int32_t digitDisplayBlinkCounter;
    TelmHandSequence ownerSequence;
    TelmHandSequence cameraSequence;
    char dialedDigits[11];
    char dialedDigitsPad;
    std::int32_t dialedDigitsDirty;
    char phoneBookEntries[32][11];
    TelephoneAuthSequence authSequence;
    float receiverAnimTimer;
    Vector3 callAuthStartPosition;
    Vector3 callAuthEndPosition;
    Vector3 authEyePosition;
    Vector3 authTargetPosition;
    Vector3 defaultReceiverPosition;
    Vector3 defaultReceiverRotation;
    Vector3 dialAuthStartPosition;
    Vector3 dialAuthEndPosition;
    Mt5RuntimeNode *receiverCordNode;
    Mt5RuntimeNode *handsetCordNode;
    std::int32_t customReceiverPoseEnabled;
    Vector3 customReceiverOffset;
    Vector3i customReceiverRotation;
    std::int32_t field92c;
    TelephoneUiData phoneUi;
};


struct TelmScriptParameters {
    std::int32_t command;
    std::uint32_t owner;
    std::int32_t argument2;
    std::int32_t argument3;
};
#pragma pack(pop)

static_assert(sizeof(TelephoneUiData) == 0x50);
static_assert(offsetof(TelephoneUiData, receiverRotatedPosition) == 0x14);
static_assert(offsetof(TelephoneUiData, arcAngle) == 0x44);
static_assert(sizeof(FcvSplineChannel) == 0x1c);
static_assert(sizeof(FcvSplineReader) == 0x30);
static_assert(sizeof(TelmHandSequence) == 0x240);
static_assert(offsetof(TelmHandSequence, sequenceId) == 0x28);
static_assert(offsetof(TelmHandSequence, positionDone) == 0x54);
static_assert(offsetof(TelmHandSequence, frameCounter) == 0x6c);
static_assert(offsetof(TelmHandSequence, positionChannels) == 0x70);
static_assert(offsetof(TelmHandSequence, rotationChannels) == 0xc4);
static_assert(offsetof(TelmHandSequence, positionReaders) == 0x118);
static_assert(offsetof(TelmHandSequence, rotationReaders) == 0x1a8);
static_assert(offsetof(TelmHandSequence, coefficientPool) == 0x238);
static_assert(sizeof(TelephoneAuthSequence) == 0xe4);
static_assert(sizeof(TelephoneWork) == 0x980);
static_assert(offsetof(TelephoneWork, scriptContext) == 0x18);
static_assert(offsetof(TelephoneWork, mainRanLastFrame) == 0x38);
static_assert(offsetof(TelephoneWork, phoneVariant) == 0xa0);
static_assert(offsetof(TelephoneWork, scenarioTask) == 0xb0);
static_assert(offsetof(TelephoneWork, currentPeerNode) == 0xd8);
static_assert(offsetof(TelephoneWork, ownerNode) == 0xf0);
static_assert(offsetof(TelephoneWork, receiverNode) == 0x100);
static_assert(offsetof(TelephoneWork, handMotionTaskData) == 0x120);
static_assert(offsetof(TelephoneWork, alternateReceiverPosition) == 0x160);
static_assert(offsetof(TelephoneWork, state) == 0x184);
static_assert(offsetof(TelephoneWork, digitDisplayBlinkCounter) == 0x1c4);
static_assert(offsetof(TelephoneWork, ownerSequence) == 0x1c8);
static_assert(offsetof(TelephoneWork, cameraSequence) == 0x408);
static_assert(offsetof(TelephoneWork, dialedDigits) == 0x648);
static_assert(offsetof(TelephoneWork, phoneBookEntries) == 0x658);
static_assert(offsetof(TelephoneWork, authSequence) == 0x7b8);
static_assert(offsetof(TelephoneWork, receiverAnimTimer) == 0x89c);
static_assert(offsetof(TelephoneWork, authEyePosition) == 0x8b8);
static_assert(offsetof(TelephoneWork, dialAuthStartPosition) == 0x8e8);
static_assert(offsetof(TelephoneWork, receiverCordNode) == 0x900);
static_assert(offsetof(TelephoneWork, customReceiverPoseEnabled) == 0x910);
static_assert(offsetof(TelephoneWork, phoneUi) == 0x930);
static_assert(sizeof(TelmScriptParameters) == 0x10);


extern std::array<char, 25> g_telephoneDisplayedNumber;
extern std::int32_t g_telmIsActive;
extern Character *g_telmRetailProbeCharacter;
extern TelephoneWork *g_telmRetailProbeWork;

void EnqueueTELM(Character *character, std::int32_t variant);
void TELM_Callback(Character *character, void *payload);
void telmUpdateMotion(Character *character);
void TELM_Cleanup(TelephoneWork *phone);
void TELM_SetTargetTaskAndMode(TelephoneWork *phone, std::uint32_t ownerId, std::uint32_t targetId, std::int32_t mode);
void TELM_LoadHandMotionAndSetup(TelephoneWork *phone, std::uint32_t ownerId);
bool telmGetIsActive(Character *character);
void Script_TELMUtils(ScenarioContext *context, void *rawParams);
std::int32_t Sequence_ReadFCVKey(FcvSplineReader *reader, float time, float *output);

} // namespace shenmue::s1
