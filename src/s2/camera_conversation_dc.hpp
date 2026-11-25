#pragma once

#include "s2/camera_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

struct ConversationCandidateState {
    std::uint8_t flags, group, groupIndex, collisionClass;
};
struct ConversationShotHistory {
    std::int32_t candidate;
    std::int16_t duplicateCandidate, shotType;
    shen_name eyeName, targetName;
};
struct Character;
struct ConversationParticipantAlpha {
    std::uint32_t flags;
    shen_name name;
};
struct ConversationOffset {
    float distance;
    std::int32_t angle;
    float height;
};
struct ConversationDialogueRecord {
    std::uint32_t command;
    float duration;
    Character *speaker, *listener;
};
#if SHENMUE_IS_DC && SHENMUE_IS_S2
struct ConversationCameraState {
    std::uint32_t flags, sceneFlags;
    Character *root, *specialTarget, *packetSpeaker, *packetListener, *targetActor, *eyeActor;
    shen_name rootScenarioName, specialTargetName, targetScenarioName, eyeScenarioName;
    std::uint8_t shotGenerationCount, retained31;
    std::int16_t shotDurationFrames, shotFrame, retained36;
    float interpolationProgress, shotProgress;
    std::int16_t totalFrames;
    std::uint16_t viewUpdateCount;
    std::uint32_t shotStartTime, pauseStartTime;
    std::uint8_t side, mirrorFlags, collisionMode;
    std::int8_t angleClass;
    Vector3 eye, target;
    std::uint16_t fieldOfView, framePreviousFieldOfView;
    Vector3 framePreviousEye, framePreviousTarget, previousEye, previousTarget;
    std::uint16_t previousFieldOfView, retained9e;
    Vector3 actorEye, actorTarget, previousActorEye, previousActorTarget;
    std::int16_t actorYaw, previousActorYaw;
    std::uint32_t retainedd4;
    Vector3s actorDirection;
    std::byte retainedde[0x1e];
    std::uint32_t shotScratch;
    float actorDistance, actorHeightDelta, groundBias;
    std::int8_t shotTypes[24];
    std::uint8_t groupSelections[4];
    std::int8_t shotType, candidateIndex, forcedCandidate;
    std::uint8_t retained12b;
    Vector3 lookLocation;
    std::int16_t lookDelayFrames, lookTurnFrames;
    std::int8_t orientationClass;
    std::uint8_t retained13d[3];
    const ConversationDialogueRecord *source, *preparedStream;
    const ConversationDialogueRecord *shots[24];
    std::int8_t shotCount, memberCount, shotIndex;
    std::uint8_t retained1ab;
    Character *members[5];
    std::uint8_t memberSeen[5], shotFlags[24], currentShotFlags;
    std::uint8_t retained1de[2];
    Character *eyeAnchors[24], *targetAnchors[24], *partners[24];
    ConversationParticipantAlpha alphaParticipants[6];
    ConversationCandidateState candidates[96];
    std::int8_t candidateCount;
    std::byte retainedCandidateState[3];
    ConversationShotHistory history[4];
    ConversationShotHistory *previousHistory;
    std::int8_t historyIndex;
    std::byte retained4f9[0x1b];
    std::int16_t entryBlendFrames, specialOverrideFrames, activationFrames, overrideElapsed;
    Vector3 fixedPosition;
    std::byte retainedTail[4];
};
#else
struct ConversationCameraState {
    std::uint32_t flags;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    std::byte retained04[0x40];
    std::uint32_t shotStartTime, pauseStartTime;
    std::byte retained4c[0xde];
#else
    std::byte retainedHeader[0x126];
#endif
    std::int8_t forcedCandidate;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    std::byte retainedConversationState[0x7e];
    std::int8_t memberCount;
    std::byte retainedConversationMembers[0x186];
#else
    std::byte retainedConversationState[0x205];
#endif
    ConversationCandidateState candidates[96];
    std::uint8_t candidateCount;
    std::byte retainedCandidateState[3];
    ConversationShotHistory history[4];
    std::uint32_t historyIndex;
    std::uint8_t collisionPhase;
    std::byte retainedState[0x23];
    Vector3 fixedPosition;
    std::byte retainedTail[4];
};
#endif
typedef char Layout_camera_conversation_dc_0[(sizeof(ConversationCameraState) == 0x52c) ? 1 : -1];
typedef char Layout_camera_conversation_dc_1[(offsetof(ConversationCameraState, forcedCandidate) == 0x12a) ? 1 : -1];
typedef char Layout_camera_conversation_dc_2[(offsetof(ConversationCameraState, candidates) == 0x330) ? 1 : -1];
typedef char Layout_camera_conversation_dc_3[(offsetof(ConversationCameraState, candidateCount) == 0x4b0) ? 1 : -1];
typedef char Layout_camera_conversation_dc_4[(offsetof(ConversationCameraState, history) == 0x4b4) ? 1 : -1];
#if SHENMUE_IS_DC && SHENMUE_IS_S2
typedef char Layout_camera_conversation_dc_5[(offsetof(ConversationCameraState, previousHistory) == 0x4f4) ? 1 : -1];
typedef char Layout_camera_conversation_dc_6[(offsetof(ConversationCameraState, historyIndex) == 0x4f8) ? 1 : -1];
#else
typedef char Layout_camera_conversation_dc_5[(offsetof(ConversationCameraState, historyIndex) == 0x4f4) ? 1 : -1];
typedef char Layout_camera_conversation_dc_6[(offsetof(ConversationCameraState, collisionPhase) == 0x4f8) ? 1 : -1];
#endif
typedef char Layout_camera_conversation_dc_7[(offsetof(ConversationCameraState, fixedPosition) == 0x51c) ? 1 : -1];
struct ConversationCameraDescriptor {
    std::uint32_t flags;
    const void *parameters;
    std::uint32_t id;
};
typedef char Layout_camera_conversation_dc_8[(sizeof(ConversationCameraDescriptor) == 12) ? 1 : -1];
std::int32_t camCtrlSetConversationFlag4000(std::int32_t enabled);
extern ConversationCameraState g_conversationCamera;
extern ConversationCameraState *g_conversationCameraState;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
struct Character;
static const std::size_t kConversationCameraMemberOffset = 0x1ac;
typedef char Layout_camera_conversation_dc_9[(offsetof(ConversationCameraState, memberCount) == 0x1a9) ? 1 : -1];
typedef char Layout_camera_conversation_dc_10[(kConversationCameraMemberOffset + 127 * sizeof(std::uintptr_t) <= sizeof(ConversationCameraState)) ? 1 : -1];
bool IsConversationCameraCharacter(const Character *character);
float UnprojectViewPointToWorld(float x, float y, float depth, Vector3 *world);
float ProjectConversationActorToScreen(const Vector3 &world, Vector3 &screen);
typedef char Layout_camera_conversation_dc_11[(offsetof(ConversationCameraState, shotStartTime) == 0x44) ? 1 : -1];
typedef char Layout_camera_conversation_dc_12[(offsetof(ConversationCameraState, pauseStartTime) == 0x48) ? 1 : -1];
void PauseConversationCameraClock(std::int32_t paused);
void RestoreConversationCamera();
void UpdateConversationCamera();
void ActivateConversationCameraStream(const ConversationDialogueRecord *stream);
void EndConversationCameraStream();
void ResumeConversationCamera();
std::int32_t camCtrlHoldConversationShot(std::int32_t release);
shen_name GetCameraSpecialTargetName();
std::int32_t camCtrlSetConversationLookTarget(const Vector3 *location);
void TickConversationActivation(ConversationCameraState *state);
void AdvanceConversationShotClock(ConversationCameraState *state);
void UpdateConversationMemberAlpha(ConversationCameraState *state);
void BeginConversationShot(ConversationCameraState *state);
void EvaluateConversationShot(ConversationCameraState *state);
typedef char Layout_camera_conversation_dc_13[(offsetof(ConversationCameraState, shotDurationFrames) == 0x32) ? 1 : -1];
typedef char Layout_camera_conversation_dc_14[(offsetof(ConversationCameraState, eye) == 0x50) ? 1 : -1];
typedef char Layout_camera_conversation_dc_15[(offsetof(ConversationCameraState, previousEye) == 0x84) ? 1 : -1];
typedef char Layout_camera_conversation_dc_16[(offsetof(ConversationCameraState, actorDistance) == 0x100) ? 1 : -1];
typedef char Layout_camera_conversation_dc_17[(offsetof(ConversationCameraState, shots) == 0x148) ? 1 : -1];
typedef char Layout_camera_conversation_dc_18[(offsetof(ConversationCameraState, alphaParticipants) == 0x300) ? 1 : -1];
typedef char Layout_camera_conversation_dc_19[(offsetof(ConversationCameraState, activationFrames) == 0x518) ? 1 : -1];
#endif
extern ConversationCameraDescriptor conversationCameraDescriptors[65];
extern std::byte customConversationCameraParameters[];
void ResetConversationCandidates(ConversationCameraState *state);
void ResetConversationCameraState(ConversationCameraState *state);
void InitializeConversationCameraState();
std::int32_t SetConversationCameraEnabled(std::int32_t enabled);
std::int32_t SetConversationCameraFixedPosition(const Vector3 *position);
std::int32_t IsConversationCameraShotEstablished();

static const std::uint32_t conversationFixedShotParameters[3][2] = {
    {0x00000000U, 0x40400000U},
    {0x2aaa0000U, 0x40400000U},
    {0xd5560000U, 0x40400000U},
};

static const std::uint32_t conversationStaticShotParameters[12][13] = {
    {0x1c711c71U, 0xbeb33333U, 0x3df5c28fU, 0xbf028f5cU, 0x3cf5c28fU, 0x3cf5c28fU, 0xbdb851ecU, 0xbe800000U, 0x3db851ecU, 0xbf170a3dU, 0x3ca3d70aU, 0x3cf5c28fU,
        0xbda3d70aU},
    {0x1c711c71U, 0xbe75c28fU, 0x3d75c28fU, 0xbf170a3dU, 0x80000000U, 0x3cf5c28fU, 0xbdf5c28fU, 0xbe23d70aU, 0x3ca3d70aU, 0xbf1eb852U, 0x80000000U, 0x3cf5c28fU,
        0xbde147aeU},
    {0x1c711c71U, 0xbee66666U, 0x3e051eb8U, 0xbf170a3dU, 0x3cf5c28fU, 0x3ca3d70aU, 0xbd8f5c29U, 0xbe99999aU, 0x3dcccccdU, 0xbf23d70aU, 0x3ca3d70aU, 0x3ca3d70aU,
        0xbd8f5c29U},
    {0x1c711c71U, 0xbea3d70aU, 0x3ca3d70aU, 0xbf28f5c3U, 0x3ca3d70aU, 0x3ca3d70aU, 0xbd8f5c29U, 0xbe2e147bU, 0x00000000U, 0xbf428f5cU, 0x3c23d70aU, 0x3ca3d70aU,
        0xbd75c28fU},
    {0x1c711c71U, 0xbeb33333U, 0xbe4ccccdU, 0xbf2147aeU, 0x3c23d70aU, 0x3c23d70aU, 0xbd75c28fU, 0xbe570a3dU, 0xbe0f5c29U, 0xbf3851ecU, 0x3c23d70aU, 0x3c23d70aU,
        0xbd75c28fU},
    {0x1c711c71U, 0xbf07ae14U, 0xbdb851ecU, 0xbf23d70aU, 0x3ca3d70aU, 0x3ca3d70aU, 0xbdb851ecU, 0xbed1eb85U, 0xbe6147aeU, 0xbf333333U, 0x3ca3d70aU, 0x3ca3d70aU,
        0xbdb851ecU},
    {0x1c711c71U, 0xbf07ae14U, 0xbdb851ecU, 0xbf23d70aU, 0x3ca3d70aU, 0x3ca3d70aU, 0xbdb851ecU, 0xbed1eb85U, 0xbe6147aeU, 0xbf333333U, 0x3ca3d70aU, 0x3ca3d70aU,
        0xbdb851ecU},
    {0x1c711c71U, 0xbf147ae1U, 0x3e23d70aU, 0xbf5c28f6U, 0x3d23d70aU, 0xbd4ccccdU, 0xbda3d70aU, 0xbec28f5cU, 0x3db851ecU, 0xbf570a3dU, 0x3d23d70aU, 0xbcf5c28fU,
        0xbda3d70aU},
    {0x1c711c71U, 0xbf0a3d71U, 0xbe23d70aU, 0xbf4ccccdU, 0x3cf5c28fU, 0xbd23d70aU, 0xbd8f5c29U, 0xbeeb851fU, 0xbe947ae1U, 0xbf6e147bU, 0x3cf5c28fU, 0xbd4ccccdU,
        0xbda3d70aU},
    {0x1c711c71U, 0xbe4ccccdU, 0xbe3851ecU, 0xbf933333U, 0xbd75c28fU, 0xbef5c28fU, 0xbf07ae14U, 0xbde147aeU, 0xbe4ccccdU, 0xbf8e147bU, 0xbd75c28fU, 0xbefae148U,
        0xbf0a3d71U},
    {0x1c711c71U, 0xbef0a3d7U, 0xbe4ccccdU, 0xbf733333U, 0xbd8f5c29U, 0xbef5c28fU, 0xbf028f5cU, 0xbecccccdU, 0xbe4ccccdU, 0xbf800000U, 0xbda3d70aU, 0xbf000000U,
        0xbf051eb8U},
    {0x1c711c71U, 0xbe23d70aU, 0xbe051eb8U, 0xbfb70a3dU, 0xbd4ccccdU, 0xbef5c28fU, 0xbee66666U, 0xbe0f5c29U, 0xbe428f5cU, 0xbfa147aeU, 0xbd4ccccdU, 0xbefae148U,
        0xbee66666U},
};

static const std::uint32_t conversationMovingShotParameters[48][26] = {
    {0x1c711c71U, 0x3f0a3d71U, 0x0000a1d3U, 0x3df5c28fU, 0x3e800000U, 0x0000d54fU, 0xbd8f5c29U, 0x3f0a3d71U, 0x0000a62bU, 0x3d23d70aU, 0x3e800000U, 0x0000d54eU,
        0xbd8f5c29U, 0x1c711c71U, 0x3ef0a3d7U, 0x00009545U, 0x3dcccccdU, 0x3f028f5cU, 0x0000d1dbU, 0xbed1eb85U, 0x3ecccccdU, 0x00009a74U, 0x3ca3d70aU,
        0x3f051eb8U, 0x0000d07eU, 0xbe99999aU},
    {0x1c711c71U, 0x3edc28f6U, 0x0000a001U, 0xbca3d70aU, 0x3e6147aeU, 0x0000c7d4U, 0xbcf5c28fU, 0x3f0f5c29U, 0x0000a2deU, 0xbd8f5c29U, 0x3e800000U, 0x0000c8c8U,
        0xbcf5c28fU, 0x1c711c71U, 0x3ecccccdU, 0x00009a1aU, 0xbca3d70aU, 0x3efae148U, 0x0000c7caU, 0xbcf5c28fU, 0x3ed70a3dU, 0x00009c91U, 0xbd4ccccdU,
        0x3f30a3d7U, 0x0000c879U, 0xbcf5c28fU},
    {0x1c711c71U, 0x3ec7ae14U, 0x0000b0e2U, 0xbde147aeU, 0x3eae147bU, 0x0000b017U, 0xbc23d70aU, 0x3eb33333U, 0x0000a8c8U, 0xbd8f5c29U, 0x3ea8f5c3U, 0x0000aec8U,
        0xbc23d70aU, 0x1c711c71U, 0x3eb33333U, 0x0000ab3eU, 0xbd8f5c29U, 0x3f0ccccdU, 0x0000ba5bU, 0xbc23d70aU, 0x3e947ae1U, 0x0000a083U, 0xbd23d70aU,
        0x3f266666U, 0x0000bcb7U, 0x3db851ecU},
    {0x1c711c71U, 0x3f4ccccdU, 0x00009cb3U, 0x3e851eb8U, 0x3ee66666U, 0x0000f819U, 0xbd23d70aU, 0x3f5eb852U, 0x0000a3fdU, 0x3e0f5c29U, 0x3edc28f6U, 0x0000f8c8U,
        0xbd23d70aU, 0x1c711c71U, 0x3f51eb85U, 0x000095fbU, 0x3e6b851fU, 0x3f4f5c29U, 0x0000e1c2U, 0xbed70a3dU, 0x3f570a3dU, 0x00009ae4U, 0x3e4ccccdU,
        0x3f666666U, 0x0000df3bU, 0xbec7ae14U},
    {0x1c711c71U, 0x3f4a3d71U, 0x00009f0dU, 0x00000000U, 0x3e8f5c29U, 0x0000db8dU, 0x00000000U, 0x3f170a3dU, 0x00009ddaU, 0x3cf5c28fU, 0x3e851eb8U, 0x0000dc00U,
        0x00000000U, 0x1c711c71U, 0x3f4ccccdU, 0x000096dfU, 0x00000000U, 0x3f547ae1U, 0x0000d007U, 0x00000000U, 0x3f0ccccdU, 0x00009864U, 0x00000000U,
        0x3f3ae148U, 0x0000d063U, 0x00000000U},
    {0x1c711c71U, 0x3f4f5c29U, 0x0000a414U, 0xbdcccccdU, 0x3e8a3d71U, 0x0000d10eU, 0xbca3d70aU, 0x3f428f5cU, 0x0000acb7U, 0xbe19999aU, 0x3ea3d70aU, 0x0000d297U,
        0xbca3d70aU, 0x1c711c71U, 0x3f2b851fU, 0x00009bccU, 0xbe19999aU, 0x3f30a3d7U, 0x0000cc10U, 0x3e6b851fU, 0x3f35c28fU, 0x0000a1d8U, 0xbe6147aeU,
        0x3f4ccccdU, 0x0000cc41U, 0x3e6b851fU},
    {0x1c711c71U, 0x3faf5c29U, 0x0000b01fU, 0x3cf5c28fU, 0x3ebd70a4U, 0x0000eaa4U, 0xbe19999aU, 0x3fa147aeU, 0x0000a7f0U, 0x3e947ae1U, 0x3ebd70a4U, 0x0000eb98U,
        0xbe3851ecU, 0x1c711c71U, 0x3faf5c29U, 0x00009989U, 0x3d75c28fU, 0x3f70a3d7U, 0x0000ddfeU, 0xbee147aeU, 0x3f851eb8U, 0x000094b6U, 0x3e051eb8U,
        0x3f3d70a4U, 0x0000e3ffU, 0xbf028f5cU},
    {0x1c711c71U, 0x3fb5c28fU, 0x0000ad53U, 0xbeb33333U, 0x3eb851ecU, 0x0000de48U, 0xbd75c28fU, 0x3faccccdU, 0x0000a1e1U, 0xbe6b851fU, 0x3eae147bU, 0x0000de0fU,
        0xbdcccccdU, 0x1c711c71U, 0x3fbc28f6U, 0x00009b22U, 0xbdf5c28fU, 0x3f733333U, 0x0000d53cU, 0xbde147aeU, 0x3f9c28f6U, 0x00009660U, 0xbdf5c28fU,
        0x3f400000U, 0x0000d6a4U, 0xbd8f5c29U},
    {0x1c711c71U, 0x3faf5c29U, 0x0000a807U, 0xbe0f5c29U, 0x3eb33333U, 0x0000dfa6U, 0xbdcccccdU, 0x3fc00000U, 0x0000b02fU, 0xbefae148U, 0x3ec7ae14U, 0x0000e04aU,
        0xbca3d70aU, 0x1c711c71U, 0x3ffd70a4U, 0x00009ec7U, 0xbea8f5c3U, 0x3f88f5c3U, 0x0000d879U, 0xbdf5c28fU, 0x40070a3dU, 0x0000a46aU, 0xbf4ccccdU,
        0x3f851eb8U, 0x0000da48U, 0x3e800000U},
    {0x1c711c71U, 0x4017ae14U, 0x0000983aU, 0x3e800000U, 0x3e947ae1U, 0x0000c3e2U, 0xbec7ae14U, 0x4028f5c3U, 0x0000a296U, 0x3f0ccccdU, 0x3ecccccdU, 0x0000cecaU,
        0xbefae148U, 0x1c711c71U, 0x40470a3dU, 0x00009a63U, 0x3f147ae1U, 0x3f3851ecU, 0x0000cec1U, 0xbf70a3d7U, 0x4048f5c3U, 0x0000a182U, 0x3f4f5c29U,
        0x3f828f5cU, 0x0000cfc5U, 0xbf733333U},
    {0x1c711c71U, 0x3fe8f5c3U, 0x0000a697U, 0xbe9eb852U, 0x3eb33333U, 0x0000eb76U, 0xbe428f5cU, 0x4023d70aU, 0x0000b3edU, 0xbf051eb8U, 0x3edc28f6U, 0x0000ea87U,
        0xbe9eb852U, 0x1c711c71U, 0x3ffd70a4U, 0x000096f4U, 0xbea3d70aU, 0x3f70a3d7U, 0x0000dbf5U, 0xbc23d70aU, 0x401a3d71U, 0x00009fb6U, 0xbf547ae1U,
        0x3f933333U, 0x0000dbc2U, 0x3e051eb8U},
    {0x1c711c71U, 0x4011eb85U, 0x00009fe0U, 0xbf3ae148U, 0x3ec28f5cU, 0x0000ecebU, 0xbe428f5cU, 0x3ff47ae1U, 0x0000b019U, 0xbea3d70aU, 0x3ed70a3dU, 0x0000ea87U,
        0xbe23d70aU, 0x1c711c71U, 0x4021eb85U, 0x00009830U, 0xbf400000U, 0x3f7d70a4U, 0x0000dddfU, 0x3da3d70aU, 0x4021eb85U, 0x00009e21U, 0xbef5c28fU,
        0x3f8e147bU, 0x0000dc11U, 0xbe570a3dU},
    {0x2aaa2aaaU, 0x403147aeU, 0x000094feU, 0x3dcccccdU, 0x3e99999aU, 0x0000e03bU, 0xbf23d70aU, 0x403147aeU, 0x0000a0b2U, 0x3eb33333U, 0x3ebd70a4U, 0x0000df7aU,
        0xbf23d70aU, 0x2aaa2aaaU, 0x4035c28fU, 0x00009a85U, 0x3dcccccdU, 0x3f87ae14U, 0x0000d604U, 0xbf7851ecU, 0x403851ecU, 0x0000a354U, 0x3ef5c28fU,
        0x3fa3d70aU, 0x0000d7aaU, 0xbf8a3d71U},
    {0x2aaa2aaaU, 0x402e147bU, 0x0000a64aU, 0xbe800000U, 0x3f028f5cU, 0x0000ff62U, 0xbf0f5c29U, 0x402ccccdU, 0x0000a160U, 0xbf1c28f6U, 0x3f000000U, 0x0000ff49U,
        0xbf0f5c29U, 0x2aaa2aaaU, 0x40366666U, 0x0000a842U, 0xbe75c28fU, 0x3fbae148U, 0x0000e66bU, 0xbf3d70a4U, 0x403a3d71U, 0x0000a057U, 0xbf1c28f6U,
        0x3fae147bU, 0x0000e398U, 0xbefae148U},
    {0x2aaa2aaaU, 0x40347ae1U, 0x00009e1eU, 0xbf800000U, 0x3eb851ecU, 0x0000da02U, 0xbee147aeU, 0x40370a3dU, 0x0000a407U, 0xbf800000U, 0x3ee66666U, 0x0000db5eU,
        0xbee147aeU, 0x2aaa2aaaU, 0x402eb852U, 0x0000a19aU, 0xbf800000U, 0x3fa147aeU, 0x0000d6ceU, 0xbe6b851fU, 0x40328f5cU, 0x0000a59bU, 0xbf800000U,
        0x3fc28f5cU, 0x0000dbb2U, 0xbe6b851fU},
    {0x2aaa2aaaU, 0x4069999aU, 0x00009e0fU, 0x3e75c28fU, 0x3ea8f5c3U, 0x0000d943U, 0xbee147aeU, 0x4073d70aU, 0x0000a4f2U, 0x3f2147aeU, 0x3ec28f5cU, 0x0000db9cU,
        0xbee147aeU, 0x2aaa2aaaU, 0x408428f6U, 0x00009d01U, 0x3e8f5c29U, 0x3f87ae14U, 0x0000d625U, 0xbf19999aU, 0x40875c29U, 0x0000a57bU, 0x3f07ae14U,
        0x3faccccdU, 0x0000d97bU, 0xbf2e147bU},
    {0x2aaa2aaaU, 0x4077ae14U, 0x00009d4cU, 0xbf400000U, 0x3f333333U, 0x000007c7U, 0xbea3d70aU, 0x4086b852U, 0x00009954U, 0xbebd70a4U, 0x3f333333U, 0x00000816U,
        0xbe800000U, 0x2aaa2aaaU, 0x40633333U, 0x00009f9dU, 0xbf451eb8U, 0x3f9d70a4U, 0x0000ec63U, 0xbea3d70aU, 0x40733333U, 0x00009b6aU, 0xbed70a3dU,
        0x3f970a3dU, 0x0000ec13U, 0xbed1eb85U},
    {0x2aaa2aaaU, 0x406f5c29U, 0x0000a99fU, 0xbf8147aeU, 0x3f266666U, 0x00000a56U, 0xbeae147bU, 0x4077ae14U, 0x0000a1e0U, 0xbf866666U, 0x3f30a3d7U, 0x00000656U,
        0xbe9eb852U, 0x2aaa2aaaU, 0x405d70a4U, 0x0000aaacU, 0xbf800000U, 0x3fc147aeU, 0x0000eafbU, 0xbe6147aeU, 0x4069999aU, 0x0000a3a0U, 0xbf87ae14U,
        0x3f90a3d7U, 0x0000efbbU, 0xbe8f5c29U},
    {0x1c711c71U, 0x3f8ccccdU, 0x00009cb3U, 0x3d4ccccdU, 0x3ea3d70aU, 0x0000e7b3U, 0xbdf5c28fU, 0x3fdc28f6U, 0x0000ab04U, 0xbe6147aeU, 0x3ec7ae14U, 0x0000e788U,
        0xbe3851ecU, 0x1c711c71U, 0x3fbc28f6U, 0x0000998eU, 0xbdb851ecU, 0x3f70a3d7U, 0x0000d095U, 0xbe051eb8U, 0x401d70a4U, 0x0000a150U, 0xbf0ccccdU,
        0x3f933333U, 0x0000dc05U, 0xbe2e147bU},
    {0x1c711c71U, 0x3f9ae148U, 0x0000a987U, 0xbee66666U, 0x3ea3d70aU, 0x0000ef96U, 0xbe6b851fU, 0x3f3d70a4U, 0x0000a239U, 0x3d23d70aU, 0x3e947ae1U, 0x0000ee34U,
        0xbdcccccdU, 0x1c711c71U, 0x3f9eb852U, 0x00009d73U, 0xbed1eb85U, 0x3f4a3d71U, 0x0000e4d9U, 0xbf2147aeU, 0x3f7ae148U, 0x0000971cU, 0xbcf5c28fU,
        0x3f3851ecU, 0x0000e37cU, 0xbe9eb852U},
    {0x1c711c71U, 0x3fe66666U, 0x00009d14U, 0xbe2e147bU, 0x3eae147bU, 0x0000eff2U, 0xbe428f5cU, 0x3ff5c28fU, 0x0000b05cU, 0x3ea3d70aU, 0x3edc28f6U, 0x0000e8bfU,
        0xbea3d70aU, 0x1c711c71U, 0x4021eb85U, 0x000097efU, 0xbe6147aeU, 0x4005c28fU, 0x00000028U, 0xbee147aeU, 0x403f5c29U, 0x0000a6d8U, 0x3f000000U,
        0x40028f5cU, 0x000001f0U, 0xbef5c28fU},
    {0x1c711c71U, 0x3f2147aeU, 0x0000aa75U, 0xbf051eb8U, 0x3e851eb8U, 0x0000e902U, 0x3d8f5c29U, 0x3f30a3d7U, 0x0000b04cU, 0xbf19999aU, 0x3e851eb8U, 0x0000e902U,
        0x3d8f5c29U, 0x1c711c71U, 0x3f6e147bU, 0x00009d9cU, 0xbedc28f6U, 0x3f2147aeU, 0x0000d5f7U, 0x3d8f5c29U, 0x3f7851ecU, 0x0000a067U, 0xbf0a3d71U,
        0x3f333333U, 0x0000d521U, 0x3d8f5c29U},
    {0x1c711c71U, 0x3f3ae148U, 0x0000aae5U, 0xbf266666U, 0x3e851eb8U, 0x0000e94cU, 0x3c23d70aU, 0x3f451eb8U, 0x0000aedeU, 0xbf400000U, 0x3e9eb852U, 0x0000e720U,
        0x3c23d70aU, 0x1c711c71U, 0x3f8f5c29U, 0x00009dcbU, 0xbf23d70aU, 0x3f51eb85U, 0x0000d531U, 0x3c23d70aU, 0x3fa147aeU, 0x0000a1b3U, 0xbf35c28fU,
        0x3f70a3d7U, 0x0000d509U, 0x3c23d70aU},
    {0x1c711c71U, 0x3f733333U, 0x0000a7fdU, 0xbf5eb852U, 0x3e947ae1U, 0x0000e793U, 0xbcf5c28fU, 0x3f2e147bU, 0x0000ab08U, 0xbf59999aU, 0x3e800000U, 0x0000e997U,
        0xbcf5c28fU, 0x1c711c71U, 0x3fa00000U, 0x00009ff1U, 0xbf6147aeU, 0x3f3ae148U, 0x0000d75fU, 0xbcf5c28fU, 0x3f63d70aU, 0x0000a330U, 0xbf451eb8U,
        0x3f2147aeU, 0x0000d8ebU, 0xbcf5c28fU},
    {0x1c711c71U, 0x3f8b851fU, 0x0000b5a2U, 0xbf547ae1U, 0x3e851eb8U, 0x0000db97U, 0xbe4ccccdU, 0x3fb1eb85U, 0x0000bf2dU, 0xbf63d70aU, 0x3e99999aU, 0x0000db23U,
        0xbe570a3dU, 0x1c711c71U, 0x3f9ae148U, 0x00009f76U, 0xbf4a3d71U, 0x3f266666U, 0x0000d13eU, 0xbe6147aeU, 0x3fe147aeU, 0x0000a31dU, 0xbf800000U,
        0x3f6e147bU, 0x0000d17dU, 0xbe800000U},
    {0x1c711c71U, 0x3fbc28f6U, 0x0000b415U, 0xbfbc28f6U, 0x3e2e147bU, 0x0000d7b8U, 0xbebd70a4U, 0x3fe28f5cU, 0x0000b917U, 0xbf828f5cU, 0x3e2e147bU, 0x0000d836U,
        0xbebd70a4U, 0x1c711c71U, 0x4007ae14U, 0x00009ebdU, 0xbfa00000U, 0x3f70a3d7U, 0x0000d437U, 0xbf28f5c3U, 0x402c28f6U, 0x0000a4efU, 0xbf6147aeU,
        0x3f828f5cU, 0x0000d4d1U, 0xbf3ae148U},
    {0x1c711c71U, 0x3f9c28f6U, 0x0000b318U, 0xbfbeb852U, 0x3e6147aeU, 0x0000b731U, 0xbe8f5c29U, 0x3f8147aeU, 0x0000c743U, 0xbf851eb8U, 0x3e428f5cU, 0x0000c268U,
        0xbe428f5cU, 0x1c711c71U, 0x3fa147aeU, 0x00009ca5U, 0xbfa28f5cU, 0x3f666666U, 0x0000df9eU, 0xbe851eb8U, 0x3f666666U, 0x0000ab4cU, 0xbf90a3d7U,
        0x3f5c28f6U, 0x0000e0c0U, 0xbe75c28fU},
    {0x2aaa2aaaU, 0x3ee147aeU, 0x0000b798U, 0x3d23d70aU, 0x3ef0a3d7U, 0x0000dbc9U, 0xbda3d70aU, 0x3f83d70aU, 0x0000d09fU, 0xbe2e147bU, 0x3f2e147bU, 0x0000e093U,
        0xbdf5c28fU, 0x2aaa2aaaU, 0x3ea8f5c3U, 0x0000ad62U, 0x3c23d70aU, 0x3fd5c28fU, 0x0000f154U, 0xbe6147aeU, 0x3f3ae148U, 0x0000cde2U, 0xbe2e147bU,
        0x403ae148U, 0x0000edccU, 0x3d23d70aU},
    {0x2aaa2aaaU, 0x3fa147aeU, 0x0000bdf5U, 0xbe19999aU, 0x3f11eb85U, 0x0000defeU, 0xbe6b851fU, 0x3fab851fU, 0x0000c668U, 0xbe6147aeU, 0x3f1eb852U, 0x0000e087U,
        0xbe3851ecU, 0x2aaa2aaaU, 0x4068f5c3U, 0x0000c2e9U, 0xbf23d70aU, 0x40000000U, 0x0000dcbeU, 0xbe23d70aU, 0x407ae148U, 0x0000cbbcU, 0xbf400000U,
        0x400851ecU, 0x0000de93U, 0xbde147aeU},
    {0x2aaa2aaaU, 0x3edc28f6U, 0x0000a707U, 0x3c23d70aU, 0x3e4ccccdU, 0x0000db5eU, 0xbca3d70aU, 0x3f07ae14U, 0x0000b0daU, 0x3c23d70aU, 0x3e851eb8U, 0x0000dd32U,
        0xbcf5c28fU, 0x2aaa2aaaU, 0x3ea3d70aU, 0x00009539U, 0x3c23d70aU, 0x3e4ccccdU, 0x0000db5fU, 0xbca3d70aU, 0x3edc28f6U, 0x0000a2daU, 0x3c23d70aU,
        0x3f2b851fU, 0x0000cf66U, 0xbca3d70aU},
    {0x2aaa2aaaU, 0x3f7851ecU, 0x00009de6U, 0x3cf5c28fU, 0x3ed1eb85U, 0x0000ee1aU, 0xbe428f5cU, 0x3f6e147bU, 0x000098e0U, 0x3e051eb8U, 0x3eb33333U, 0x0000f3a7U,
        0xbe428f5cU, 0x2aaa2aaaU, 0x3f7851ecU, 0x00009ddaU, 0xbe19999aU, 0x3f7d70a4U, 0x0000d9a0U, 0xbe428f5cU, 0x3f6b851fU, 0x0000968aU, 0x3d75c28fU,
        0x3f68f5c3U, 0x0000db4cU, 0xbef5c28fU},
    {0x2aaa2aaaU, 0x3ebd70a4U, 0x0000a420U, 0xbc23d70aU, 0x3f11eb85U, 0x0000a473U, 0x3d23d70aU, 0x3ee66666U, 0x0000af05U, 0xbc23d70aU, 0x3f23d70aU, 0x0000af22U,
        0x3d23d70aU, 0x2aaa2aaaU, 0x3ebd70a4U, 0x0000a441U, 0xbc23d70aU, 0x3f83d70aU, 0x0000b823U, 0x3d23d70aU, 0x3ed70a3dU, 0x0000aa47U, 0xbc23d70aU,
        0x3fb5c28fU, 0x0000bfcaU, 0x3d23d70aU},
    {0x2aaa2aaaU, 0x3f970a3dU, 0x0000a246U, 0xbd75c28fU, 0x3f266666U, 0x0000b595U, 0xbec28f5cU, 0x3f90a3d7U, 0x0000b0b6U, 0x3de147aeU, 0x3f35c28fU, 0x0000b892U,
        0xbe9eb852U, 0x2aaa2aaaU, 0x3f970a3dU, 0x0000a247U, 0xbd75c28fU, 0x3fbeb852U, 0x0000c70dU, 0xbec28f5cU, 0x3fa7ae14U, 0x0000ab0aU, 0x3e0f5c29U,
        0x3fc147aeU, 0x0000ca1fU, 0xbf051eb8U},
    {0x2aaa2aaaU, 0x3fa147aeU, 0x0000bdf5U, 0xbe19999aU, 0x3f11eb85U, 0x0000defeU, 0xbe6b851fU, 0x3fab851fU, 0x0000c668U, 0xbe6147aeU, 0x3f1eb852U, 0x0000e087U,
        0xbe3851ecU, 0x2aaa2aaaU, 0x4068f5c3U, 0x0000c2e9U, 0xbf23d70aU, 0x40000000U, 0x0000dcbeU, 0xbe23d70aU, 0x407ae148U, 0x0000cbbcU, 0xbf400000U,
        0x400851ecU, 0x0000de93U, 0xbde147aeU},
    {0x2aaa2aaaU, 0x3f7851ecU, 0x00009de6U, 0x3cf5c28fU, 0x3ed1eb85U, 0x0000ee1aU, 0xbe428f5cU, 0x3f6e147bU, 0x000098e0U, 0x3e051eb8U, 0x3eb33333U, 0x0000f3a7U,
        0xbe428f5cU, 0x2aaa2aaaU, 0x3f7851ecU, 0x00009ddaU, 0xbe19999aU, 0x3f7d70a4U, 0x0000d9a0U, 0xbe428f5cU, 0x3f6b851fU, 0x0000968aU, 0x3d75c28fU,
        0x3f68f5c3U, 0x0000db4cU, 0xbef5c28fU},
    {0x2aaa2aaaU, 0x3f970a3dU, 0x0000a246U, 0xbd75c28fU, 0x3f266666U, 0x0000b595U, 0xbec28f5cU, 0x3f90a3d7U, 0x0000b0b6U, 0x3de147aeU, 0x3f35c28fU, 0x0000b892U,
        0xbe9eb852U, 0x2aaa2aaaU, 0x3f970a3dU, 0x0000a247U, 0xbd75c28fU, 0x3fbeb852U, 0x0000c70dU, 0xbec28f5cU, 0x3fa7ae14U, 0x0000ab0aU, 0x3e0f5c29U,
        0x3fc147aeU, 0x0000ca1fU, 0xbf051eb8U},
    {0x1c711c71U, 0x3f11eb85U, 0x00009ef7U, 0x3d75c28fU, 0x3e23d70aU, 0x0000c854U, 0xbd23d70aU, 0x3f2e147bU, 0x0000a801U, 0x3cf5c28fU, 0x3e800000U, 0x0000cedeU,
        0xbcf5c28fU, 0x1c711c71U, 0x3f1eb852U, 0x00009d95U, 0x3cf5c28fU, 0x3ea3d70aU, 0x0000f0a0U, 0xbd75c28fU, 0x3f4a3d71U, 0x0000a63bU, 0x3d23d70aU,
        0x3e9eb852U, 0x0000f171U, 0xbd75c28fU},
    {0x1c711c71U, 0x3f666666U, 0x0000b6e3U, 0x3c23d70aU, 0x3e947ae1U, 0x0000f9f9U, 0xbd4ccccdU, 0x3f51eb85U, 0x0000a9a4U, 0x3d8f5c29U, 0x3e8f5c29U, 0x0000fafbU,
        0xbd4ccccdU, 0x1c711c71U, 0x3fbc28f6U, 0x0000b484U, 0xbe19999aU, 0x3f000000U, 0x00000336U, 0xbe0f5c29U, 0x3fa7ae14U, 0x0000a7cbU, 0xbd23d70aU,
        0x3f028f5cU, 0x00000224U, 0xbdf5c28fU},
    {0x1c711c71U, 0x3f4a3d71U, 0x0000ada9U, 0xbe75c28fU, 0x3e851eb8U, 0x0000f9c9U, 0xbd4ccccdU, 0x3f30a3d7U, 0x0000a596U, 0xbde147aeU, 0x3e851eb8U, 0x0000fbc3U,
        0xbcf5c28fU, 0x1c711c71U, 0x3f866666U, 0x0000aae0U, 0xbea8f5c3U, 0x3f000000U, 0x0000012fU, 0xbd4ccccdU, 0x3f4f5c29U, 0x0000a449U, 0xbe051eb8U,
        0x3f028f5cU, 0x00000131U, 0xbd4ccccdU},
    {0x1c711c71U, 0x3fb9999aU, 0x0000a4d7U, 0x3e75c28fU, 0x3e99999aU, 0x0000f2baU, 0xbe3851ecU, 0x3fc147aeU, 0x0000ad9aU, 0x3d4ccccdU, 0x3e8f5c29U, 0x0000f236U,
        0xbe23d70aU, 0x1c711c71U, 0x3fb9999aU, 0x0000a4dcU, 0x3e75c28fU, 0x3ebd70a4U, 0x0000eec8U, 0xbe6147aeU, 0x3fc28f5cU, 0x0000aeb8U, 0x3d4ccccdU,
        0x3ee66666U, 0x0000ff76U, 0xbe4ccccdU},
    {0x1c711c71U, 0x3fc3d70aU, 0x0000a8bfU, 0xbe23d70aU, 0x3e6b851fU, 0x0000e125U, 0xbe23d70aU, 0x3f8147aeU, 0x0000a1e4U, 0xbda3d70aU, 0x3e19999aU, 0x0000e3a9U,
        0xbd75c28fU, 0x1c711c71U, 0x3fc3d70aU, 0x0000a8b7U, 0xbe23d70aU, 0x3eae147bU, 0x0000e08aU, 0xbe23d70aU, 0x3f800000U, 0x0000a20cU, 0xbda3d70aU,
        0x3e8f5c29U, 0x0000dcc4U, 0xbd75c28fU},
    {0x1c711c71U, 0x3fa00000U, 0x0000aa8aU, 0xbeb851ecU, 0x3ea8f5c3U, 0x0000c21eU, 0x00000000U, 0x3fd851ecU, 0x0000b17bU, 0xbeb33333U, 0x3e9eb852U, 0x0000d6f7U,
        0xbe4ccccdU, 0x1c711c71U, 0x3fa00000U, 0x0000aa7bU, 0xbeb851ecU, 0x3ec28f5cU, 0x0000c692U, 0xbc23d70aU, 0x3fe7ae14U, 0x0000b0dcU, 0xbeb33333U,
        0x3ecccccdU, 0x0000dab0U, 0xbe3851ecU},
    {0x1c711c71U, 0x3f866666U, 0x0000bd69U, 0xbf63d70aU, 0x3ea3d70aU, 0x00000671U, 0xbebd70a4U, 0x3f7ae148U, 0x0000c8dbU, 0xbf35c28fU, 0x3ea3d70aU, 0x0000090dU,
        0xbeae147bU, 0x1c711c71U, 0x3f866666U, 0x0000bd5cU, 0xbf63d70aU, 0x3ea3d70aU, 0x0000066bU, 0xbebd70a4U, 0x3f7d70a4U, 0x0000c8f2U, 0xbf35c28fU,
        0x3e9eb852U, 0x0000091aU, 0xbea8f5c3U},
    {0x1c711c71U, 0x3f83d70aU, 0x0000c9c7U, 0xbfae147bU, 0x3e6147aeU, 0x0000b7daU, 0xbe851eb8U, 0x3fdd70a4U, 0x0000c031U, 0xbfd5c28fU, 0x3e75c28fU, 0x0000bdb0U,
        0xbe947ae1U, 0x1c711c71U, 0x3f8b851fU, 0x0000a4deU, 0xbfb47ae1U, 0x3feb851fU, 0x00000125U, 0xbf2e147bU, 0x4007ae14U, 0x0000ab57U, 0xbfcb851fU,
        0x3feccccdU, 0x000000bdU, 0xbf2e147bU},
    {0x1c711c71U, 0x3f9ae148U, 0x0000c9d0U, 0xbfdd70a4U, 0x3e947ae1U, 0x0000bf95U, 0xbeb33333U, 0x3faf5c29U, 0x0000cb82U, 0xbf851eb8U, 0x3e800000U, 0x0000c332U,
        0xbe851eb8U, 0x1c711c71U, 0x3f99999aU, 0x0000a1e9U, 0xbfd33333U, 0x3f35c28fU, 0x0000d131U, 0xbeb851ecU, 0x3fb33333U, 0x0000a476U, 0xbf8e147bU,
        0x3f4ccccdU, 0x0000cfb7U, 0xbf07ae14U},
    {0x1c711c71U, 0x3fc51eb8U, 0x0000bcabU, 0xbf8147aeU, 0x3ed70a3dU, 0x0000cac6U, 0xbe9eb852U, 0x3fbd70a4U, 0x0000c69bU, 0xbf428f5cU, 0x3edc28f6U, 0x0000cddeU,
        0xbe851eb8U, 0x1c711c71U, 0x3fee147bU, 0x0000a550U, 0xbfa28f5cU, 0x3fbc28f6U, 0x0000f751U, 0xbf5c28f6U, 0x3fd47ae1U, 0x0000abdeU, 0xbf800000U,
        0x3fc00000U, 0x0000f677U, 0xbf4ccccdU},
    {0x1c711c71U, 0x3f451eb8U, 0x0000cf84U, 0xbfe28f5cU, 0x3ee147aeU, 0x0000e0f0U, 0xbe8f5c29U, 0x3f83d70aU, 0x0000cdd5U, 0xbfcccccdU, 0x3ee147aeU, 0x0000e0f1U,
        0xbe8f5c29U, 0x1c711c71U, 0x3f59999aU, 0x00009c6bU, 0xbfd9999aU, 0x400eb852U, 0x0000fd6aU, 0xbf5eb852U, 0x3f75c28fU, 0x0000a315U, 0xbfca3d71U,
        0x400d70a4U, 0x0000fdbeU, 0xbf68f5c3U},
    {0x1c711c71U, 0x4008f5c3U, 0x0000cbc8U, 0xbfa147aeU, 0x3f028f5cU, 0x0000f53aU, 0xbeae147bU, 0x40033333U, 0x0000c30fU, 0xbfa28f5cU, 0x3f000000U, 0x0000f4feU,
        0xbeb851ecU, 0x1c711c71U, 0x4010a3d7U, 0x0000ad4dU, 0xbfdeb852U, 0x3fe7ae14U, 0x0000fa86U, 0xbec28f5cU, 0x40166666U, 0x0000a4a9U, 0xbfce147bU,
        0x3fea3d71U, 0x0000fa98U, 0xbebd70a4U},
};

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void camCtrlShakeFightCamera(float amount);
std::int16_t FightCameraYawBetween(float x1, float z1, float x2, float z2);
void FightCameraSubtract(const Vector3 &from, const Vector3 &to, Vector3 &result);
void FightCameraCopy(const Vector3 &source, Vector3 &destination);
void FightCameraBlendPoint(Vector3 &point, const Vector3 &other, float amount);
void ResumeConversationCameraWrapper();
void RotateConversationOffset(const ConversationOffset &offset, Vector3 &result);
std::int32_t camCtrlSetConversationFlag20000000(std::int32_t enabled);
std::int32_t LerpInteger(std::int32_t target, std::int32_t base, float amount);
float CameraTransitionValue(float base, float target, float amount);
float CosineEaseValue(float base, float target, float amount);
float TangentEaseValue(float base, float target, float amount, float steepness);
void OffsetPointAlongYaw(Vector3 *result, const Vector3 *point, float distance, std::int16_t yaw);
void *GetCameraMathWork();
void TouchCameraMathWork(void *);
void GetTransformedForwardVector(Vector3 *direction);
#endif

} // namespace s2
} // namespace shenmue

#endif
