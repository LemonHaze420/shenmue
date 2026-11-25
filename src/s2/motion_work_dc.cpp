#include "s2/motion_work_dc.hpp"
#include "s2/motion_ground_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include <cstring>
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

#include "s2/motion_rig.hpp"
#include "s2/model_image_component_dc.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/ui_state_dc.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/motion_stream_dc.hpp"
#include "s2/motion_rig_data_dc.hpp"
#include "s2/model_dc.hpp"
#include "s2/model_transform_dc.hpp"
#include "s2/character_components_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/npc_data_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/hand_dc.hpp"
#include "s2/hand_pose_dc.hpp"
#include "s2/hand_motion.hpp"
#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"
#include <cmath>
#endif

#define SHENMUE_MOTION_NULL 0
#define SHENMUE_MOTION_CONST_CAST(type, value) static_cast<type *>(value)
#include "shenmue.h"
#include "s2/character_pose_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include <cstdio>
#define SHENMUE_MOTION_DATA(name) name
#define SHENMUE_MOTION_NAME(value) shenmue::shen_name(value)
#include "s2/character_yaw_dc.hpp"
#include "s2/math.hpp"
#include "s2/event_matrix_dc.hpp"
#define SHENMUE_CHARACTER_POSE_NULL 0
#include "s2/motion_task_dc.hpp"
#include "s2/motion_snapshot_capture_dc.hpp"
#include "s2/motion_snapshot_control_dc.hpp"
#include "s2/event_character_dc.hpp"
#include "s2/character_dc.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "s2/character_draw_tasks_dc.hpp"
#include "s2/shadow_dc.hpp"
#include "s2/face_eye_control_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/event_character_model_dc.hpp"
#include "s2/map_region_dc.hpp"
#include "s2/door_state_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/pak_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/world_render.hpp"
#include "s2/field_collision_dc.hpp"
#endif
#include "s2/player_model_dc.hpp"
#include "s2/character_model_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/object_light_dc.hpp"
#include "s2/model_render_dc.hpp"
#include "s2/map_models_dc.hpp"
namespace shenmue {
namespace dc {
namespace sdk {
void SetMirroredModelDrawMode(std::int32_t enabled);
}
} // namespace dc
} // namespace shenmue
#include "s2/event_character_state_dc.hpp"
#include "s2/map_visibility_dc.hpp"
#include "s2/character_alpha_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/reflector_board_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/character_transparency_dc.hpp"
#include "s2/pause.hpp"
#endif
#include "s2/pad_control_dc.hpp"
#include "s2/player_state_dc.hpp"
#include "s2/model_image_dc.hpp"
#include "s2/cloth_dc.hpp"
#include "s2/osage_dc.hpp"
#include "s2/motion_node_dc.hpp"
#include "s2/prop_dc.hpp"
#include "s2/motion_yaw_dc.hpp"
namespace shenmue {
namespace s2 {

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
enum MotionRegionFlag {
    MRF_Unknown01 = 0x1U,
    MRF_Unknown02 = 0x2U,
    MRF_Loop = 0x4U,
    MRF_ExcludeEndFrame = 0x8U,
    MRF_Transition = 0x10U,
    MRF_LeadFoot = 0x20U,
    MRF_Mirrored = 0x40U,
    MRF_Reinitialize = 0x80U,
    MRF_Independent = 0x100U,
    MRF_FollowLowerBody = 0x200U,
    MRF_ReturnToLowerBody = 0x400U,
    MRF_BlendToLowerBody = 0x800U,
    MRF_Unknown1000 = 0x1000U,
};
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
enum MotionCopyFlag {
    MCF_Unknown01 = 0x1U,
    MCF_Unknown02 = 0x2U,
    MCF_LeadFoot = 0x4U,
    MCF_SampledLeadFoot = 0x8U,
    MCF_Persistent = 0x20U,
    MCF_Unknown80 = 0x80U,
    MCF_LookPending = 0x100U,
    MCF_Running = 0x200U,
    MCF_ExtraStepRequested = 0x400U,
    MCF_ExtraStepComplete = 0x800U,
    MCF_Unknown1000 = 0x1000U,
    MCF_Unknown2000 = 0x2000U,
    MCF_Unknown4000 = 0x4000U,
    MCF_AlternateLocomotion = 0x8000U,
    MCF_Unknown10000 = 0x10000U,
    MCF_Unknown20000 = 0x20000U,
    MCF_Unknown40000 = 0x40000U,
    MCF_Unknown80000 = 0x80000U,
    MCF_LookSuppressed = 0x100000U,
    MCF_TurnStepPending = 0x200000U,
    MCF_Unknown400000 = 0x400000U,
};
#endif

std::int32_t MotionRootSignedWord(std::uint32_t bits);
Vector3i CopyGetRotation(const Character *character);

} // namespace

void InitializeMotionRig(IKRig *rig, MotionOverrideRig *overrides, std::uint32_t rigType);
float mtwkGetSpeed(const MotionWork *work, MotionRegion region);
void mtwkSetEntityRotation(MotionWork *work, const Vector3i *rotation);
void MotionWorkUpdate(Character *character, void *payload);
void MotFileManageTaskEntry_Callback(void *);
void WriteMotionSnapshotData(MotionSnapshot *snapshot, Character *character, MotionWork *work);
bool charIsMotionworkEnabled(Character *character);
void mtwkSetActive(MotionWork *work);
bool charIsMotionworkCulled(Character *character);
void BindMotionControllerMatricesToModelNodes(MotionWork *work, MT7Node *model);
const Matrix4x4 *mtwkGetBoneTransform(const MotionWork *work, std::int32_t boneIndex);
const Matrix4x4 *mtwkGetBoneTransform(const Character *character, std::int32_t boneIndex);
bool mtwkGetBoneTargetPosition(const MotionWork *work, std::int32_t selector, Vector3 *position);
bool mtwkGetControllerTargetPosition(const MotionWork *work, std::uint32_t controller, Vector3 &position);
std::uint32_t mtwkGetClipMode(const MotionWork *work);
void mtwkSetClipMode(MotionWork *work, std::int32_t mode);
bool mtwkSetRequestedRootMode(MotionWork *work, std::uint32_t mode);
bool mtwkSetFrameTime(MotionWork *work, std::uint32_t frames, MotionRegion region);
std::uint32_t mtwkSetProjectionAngle(MotionWork *work, std::int32_t angle, std::int32_t slot);
bool SampleNativeMotionRoot(std::uint16_t animation, std::int32_t boneId, float frame, Vector3 *output);
std::int32_t charGetMotionRigType(const Character *character);
std::int32_t UpdateMotionTransitionControllers(Character *character);

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
struct CopyMotionContext {
    Character &character;
    MotionWork &work;
    bool hadMotion;
    CopyMotionContext(Character &c, MotionWork &w, bool h) : character(c), work(w), hadMotion(h) {
    }

    MotionWork &state() {
        return work;
    }
    Vector3 position() const {
        Vector3 value;
        GetCharacterWorldPosition(&character, &value);
        return value;
    }
    std::int16_t yaw() const {
        return static_cast<std::int16_t>(CopyGetRotation(&character).y);
    }
};
#endif

} // namespace

void TransformPointByCharacterNode(const CharacterNode *node, Vector3 *position);
void InverseTransformPointByCharacterNode(const CharacterNode *node, Vector3 *position);
void Matrix4x4_PushMatrixAndTranslate(const MotionHeadRig &head, Matrix4x4 *output);

void mtwkGetRegionClocks(MotionWork *work, MotionRegionClock **lower, MotionRegionClock **chest, MotionRegionClock **head, MotionRegionClock **rightArm,
    MotionRegionClock **leftArm);

std::uint32_t mtwkSetRegionFlag(MotionWork *work, std::uint32_t flag, std::int32_t operation, MotionRegion region);
void Matrix4x4_PushMatrixAndTranslate(const MotionHeadRig &head, Matrix4x4 *output);
std::uint32_t GetDefaultMotionTableIndex(std::uint32_t rig);

void mtwkGetRegionClocks(MotionWork *work, MotionRegionClock **lower, MotionRegionClock **chest, MotionRegionClock **head, MotionRegionClock **rightArm,
    MotionRegionClock **leftArm);

std::uint32_t mtwkSetRegionFlag(MotionWork *work, std::uint32_t flag, std::int32_t operation, MotionRegion region);
void SetCharacterWorldPosition(Character *character, const Vector3 *position);
void *AllocateModelComponent(Character *character, shen_name token, std::uint32_t payloadSize, void (*update)(Character *, void *));
void *charFindComp(const Character *character, shen_name token);
void SetModelComponentCleanup(Character *character, shen_name token, void (*cleanup)(Character *, void *));
void GetCharacterWorldRotation(const Character *character, Vector3i *rotation);
void charaSetWorldRot(Character *character, const Vector3i &rotation);
CharacterNode *GetCharacterNode(Character *character);
void AssignCharacterChildSpacePosition(Character *character, const Vector3 *delta);
void AddCharacterChildSpacePosition(Character *character, const Vector3 *delta);
void EV_SetCharDisplay(Character *character, CharaVisibilityMode mode, std::int32_t enabled);
void (*charaAssignCoreCallback(Character *character, void (*callback)(Character *)))(Character *);
CharacterUpdateCallback imgmAssignUpdateCallback(Character *character, CharacterUpdateCallback callback);
float GetCharacterDisplayRadius(const Character *character);
std::uint32_t GetDefaultMotionTableIndex(std::uint32_t rig);

void mtwkGetRegionClocks(MotionWork *work, MotionRegionClock **lower, MotionRegionClock **chest, MotionRegionClock **head, MotionRegionClock **rightArm,
    MotionRegionClock **leftArm);

std::uint32_t mtwkSetRegionFlag(MotionWork *work, std::uint32_t flag, std::int32_t operation, MotionRegion region);
void TransformPointByCharacterNode(const CharacterNode *node, Vector3 *position);
std::uint32_t GetDefaultMotionTableIndex(std::uint32_t rig);

void mtwkGetRegionClocks(MotionWork *work, MotionRegionClock **lower, MotionRegionClock **chest, MotionRegionClock **head, MotionRegionClock **rightArm,
    MotionRegionClock **leftArm);

void Matrix4x4_PushMatrixAndTranslate(const MotionHeadRig &head, Matrix4x4 *output);

const std::uint16_t motionBoneMatrixOffsets[] = {0x230, 0, 0, 0x1168, 0x12d0, 0x1508, 0, 0, 0x1690, 0x18c8, 0, 0, 0x1908, 0, 0x1a80, 0x1c10, 0x1e48, 0, 0,
    0x1e88, 0, 0x2000, 0x3e0, 0x620, 0x660, 0, 0x760, 0, 0xb10, 0xb50, 0, 0xc50};

const std::uint16_t motionOverrideMatrixPointerOffsets[] = {0x155c, 0, 0x1564, 0x156c, 0x1574, 0, 0x424, 0x914, 0xe04, 0xe0c, 0xe14};
const std::uint16_t motionControllerTargetOffsets[] = {0, 0x0fec, 0x14c4, 0x1884, 0x1e04, 0x05dc, 0x0acc};
const std::uint16_t motionControllerOverridePointerOffsets[] = {0x03dc, 0x1558};

const std::uint16_t defaultMotionIds[17][6] = {
    {0xf085U, 0xf005U, 0xf001U, 0xf001U, 0xf006U, 0xf085U},
    {0xf085U, 0xf005U, 0xf001U, 0xf001U, 0xf006U, 0xf085U},
    {0xf03fU, 0xf03eU, 0xf001U, 0xf001U, 0xf006U, 0xf03fU},
    {0xf05fU, 0xf0ffU, 0xf001U, 0xf001U, 0xf006U, 0xf05fU},
    {0xf07fU, 0xf07eU, 0xf001U, 0xf001U, 0xf006U, 0xf07fU},
    {0xf057U, 0xf084U, 0xf001U, 0xf001U, 0xf0e6U, 0xf057U},
    {0xf09fU, 0xf09eU, 0xf001U, 0xf001U, 0xf0e6U, 0xf09fU},
    {0xf0d2U, 0xf0d1U, 0xf001U, 0xf001U, 0xf0e6U, 0xf0d2U},
    {0xf061U, 0xf060U, 0xf001U, 0xf001U, 0xf006U, 0xf061U},
    {0xf087U, 0xf086U, 0xf001U, 0xf001U, 0xf006U, 0xf087U},
    {0xf079U, 0xf078U, 0xf001U, 0xf001U, 0xf006U, 0xf079U},
    {0xf059U, 0xf058U, 0xf001U, 0xf001U, 0xf006U, 0xf059U},
    {0xf0a8U, 0xf058U, 0xf001U, 0xf001U, 0xf006U, 0xf0a8U},
    {0xf085U, 0xf084U, 0xf001U, 0xf001U, 0xf006U, 0xf085U},
    {0xf130U, 0xf0a4U, 0xf001U, 0xf001U, 0xf006U, 0xf130U},
    {0xf0a7U, 0xf0a6U, 0xf001U, 0xf001U, 0xf006U, 0xf0a7U},
    {0xf0eaU, 0xf0f6U, 0xf001U, 0xf001U, 0xf006U, 0xf0eaU},
};

#include "s2/motion_rig_constants.inc"

static const char motionRigNames[17][4] = {
    "ARI", "JOY", "MEI", "BBY", "SEI", "WON", "KMN", "SAM", "RYO", "LLY", "HGN", "TGY", "CHA", "SIN", "SYE", "YUA", "ZBA"};
static const std::uint32_t *const motionRigDefaultRecords[17] = {motionRigDefaults[0], motionRigDefaults[1], motionRigDefaults[2], motionRigDefaults[3],
    motionRigDefaults[4], motionRigDefaults[5], motionRigDefaults[6], motionRigDefaults[7], motionRigDefaults[8], motionRigDefaults[9], motionRigDefaults[10],
    motionRigDefaults[11], motionRigDefaults[12], motionRigDefaults[13], motionRigDefaults[14], motionRigDefaults[15], motionRigDefaults[16]};
MotionRigConstantTables g_motionConstData = {motionRigNames, motionRigDefaultRecords, motionRigDimensionTable};

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t g_mtwkCurrentIsRyo = 0;

struct MotionCopyState {
    std::uint8_t copyOperationState, copyPhase, transitionSubstate, transitionClass;
    std::uint8_t copyDelayFrames, copyScaleCountdown, copyStepsRemaining, copyStepState;
    std::uint8_t copyRegionSelector, transitionVariant;
    std::uint16_t retainedCopyState;
    std::int16_t copyTravelYaw;
    std::uint16_t transitionStartFrame, transitionEndFrame;
    std::int16_t copySplineFrame;
    std::int16_t copySplineStatus;
    std::uint16_t transitionFrameTime;
    std::uint32_t copyFlags;
    float copyLayerStartFrame, motionScale, insideRectangleScale, copyExtraStepDistance;
    Vector3 copySourcePosition, copyDestinationPosition, copySplinePoints[5], copySplinePosition;
    std::uint16_t defaultAnimations[6];
};
#endif

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
Character *g_transitionCharacter = 0;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
MotionWork *g_transitionWork = 0;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint8_t *g_copyTransitionState = 0;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#define SHENMUE_COPY_TYPED (*reinterpret_cast<MotionCopyState *>(g_copyTransitionState))
#define SHENMUE_COPY_STATE (*reinterpret_cast<MotionWork *>(g_copyTransitionState - offsetof(MotionWork, copyOperationState)))
struct MotionCopyAnimationView {
    std::byte retained[offsetof(MotionWork, defaultAnimations) - offsetof(MotionWork, copyOperationState)];
    std::uint16_t animations[6];
};
#define SHENMUE_COPY_ANIMATION(index) (reinterpret_cast<MotionCopyAnimationView *>(g_copyTransitionState)->animations[index])
#define SHENMUE_COPY_VECTOR(field)                                                                                                                             \
    (*reinterpret_cast<Vector3 *>(g_copyTransitionState + (offsetof(MotionWork, field) - offsetof(MotionWork, copyOperationState))))
#define SHENMUE_COPY_FIELD(type, field)                                                                                                                        \
    (reinterpret_cast<type *>(g_copyTransitionState)[(offsetof(MotionWork, field) - offsetof(MotionWork, copyOperationState)) / sizeof(type)])
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t g_transitionHadMotion = 0;
#endif

} // namespace

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t mtwkEvaluateMotionFrame(MotionWork *work);
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void figpToggleMotionworkFlag(Character *character, MotionWork *work);
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void AddCharacterChildSpacePosition(Character *character, const Vector3 *position);
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t PlayFootstepForNode(std::uint32_t flags, Character *character);
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t g_motionUseOverride = 0;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t g_motionRigError = 0;
#endif

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/motion_entity_rigs.inc"
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static const std::uint32_t motionControllerIndices[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
#endif

} // namespace

#define SHENMUE_MOTION_ROOT_SIGNED_WORD MotionRootSignedWord
std::int32_t g_motionUseEnvironment;
void *charFindComp(const Character *character, shen_name token);

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
std::int32_t g_motionDrawOverridesEnabled = 1;
#else
std::int32_t g_motionDrawOverridesEnabled = 0;
#endif

void GetCharacterLocalPosition(const Character *character, Vector3 *position);

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void mtwkTransformWithoutIKEnabled(MotionWork *work, Vector3 *foot, Vector3 *pelvis) {
    IKRig &rig = reinterpret_cast<MotionWorkBindings *>(work)->rig;
    g_ikEnabled = 0;
    static_cast<MotionTargetParameters *>(rig.lowerBody.rightFoot.callback.parameters)->output = foot;
    rig.lowerBody.rightFoot.callback.evaluate(rig.lowerBody.rightFoot.callback.parameters);
    Matrix4x4::TransformPoint(*foot, *foot);
    static_cast<MotionTargetParameters *>(rig.lowerBody.pelvisPositionCallback.parameters)->output = pelvis;
    rig.lowerBody.pelvisPositionCallback.evaluate(rig.lowerBody.pelvisPositionCallback.parameters);
    Matrix4x4::TransformPoint(*pelvis, *pelvis);
    g_ikEnabled = 1;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
bool MOTN_HasHoldCommand(std::uint16_t animation) {
    if (MOTN_FindAnimationExtraRecord(animation, 24))
        return true;
    return false;
}

std::int32_t MOTN_IsEndlessMotion(std::uint16_t animation) {
    if (!animation)
        return 0;
    const MotnAnimationHeader *const header = MOTN_GetDataForAnimId(animation);
    if (!header || reinterpret_cast<const std::uint16_t *>(header)[1] != 0xFFFFU)
        return 0;
    return 1;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
const std::byte *mtwkFindFrameEvent(const MotionWork *work, std::int32_t eventType, std::int32_t occurrence) {
    return MOTN_FindFrameEvent(static_cast<std::int8_t>(eventType), work->currentMotionId[0],
        work->lowerBodyClock.ticksTime - work->lowerBodyClock.playbackSpeed, work->lowerBodyClock.ticksTime, occurrence);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
const std::byte *mtwkFindHandPoseEvent(const MotionWork *work, std::int32_t occurrence) {
    return mtwkFindFrameEvent(work, 18, occurrence);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t mtwkGetFrameEvent52Parameter(const MotionWork *work) {
    const std::byte *const event = mtwkFindFrameEvent(work, 52, 1);
    if (event)
        return *reinterpret_cast<const std::uint32_t *>(event + 4);
    return 0;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
MotionRegistryEntry *FindFreeMotionBankEntry() {
    std::int32_t index = static_cast<std::uint8_t>(g_motionBankCount);
    while (index < 32 && g_motionRegistry[index].handle != UINT32_MAX)
        ++index;
    if (index == 32)
        return 0;
    return &g_motionRegistry[index];
}

std::uint32_t *AllocateStaticMotionFileData(std::uint32_t size) {
    std::uint32_t *const data = static_cast<std::uint32_t *>(HLib::AllocateStaticOrDynamicBlock(size, MOTION_SNAPSHOT_STORAGE));
    std::uint32_t *cursor = data;
    for (std::uint32_t index = 0; index < (size >> 2U); ++index)
        *cursor++ = 0;
    return data;
}

void ReadMotiCompressed(const char *directory, const char *filename) {
    RegisterMotionBank(shenLoadFile(directory, filename, MOTION_FILE_STORAGE));
}

void ReadMotiFromDirectoryUncompressed(std::int32_t act, shen_name area, const char *filename) {
    char directory[64];
    std::sprintf(directory, "scene/%02d/%c%c%c%c", act, static_cast<std::int8_t>(area), static_cast<std::int8_t>(area >> 8),
        static_cast<std::int8_t>(static_cast<std::int32_t>(area << 8) >> 24), static_cast<std::int8_t>(static_cast<std::int32_t>(area) >> 24));
    RegisterMotionBank(ReadFile(directory, filename, MOTION_FILE_STORAGE));
}
#endif

// @verified - d
std::uint32_t DeleteMotionProps(std::int32_t handle) {
    if (handle < 1)
        return 0;
    MotionPropBank *bank = SHENMUE_MOTION_DATA(g_motionPropBanks);
    while (bank->handle != handle) {
        MotionPropBank *const next = bank->next;
        if (!next)
            return 0;
        bank = next;
    }
    FreeMotionFileData(bank->data);
    if (bank->next)
        bank->next->previous = bank->previous;
    if (bank->previous)
        bank->previous->next = bank->next;
    bank->handle = -1;
    bank->data = SHENMUE_MOTION_NULL;
    bank->previous = SHENMUE_MOTION_NULL;
    bank->next = SHENMUE_MOTION_NULL;
    return 1;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void BindMotionTargetParameters(MotionTargetParameters *parameters, MotionRigAxis *axes, Vector4 *pva, const std::uint32_t *controller) {
    parameters->channels[0] = reinterpret_cast<MotionRigChannel *>(axes);
    ++axes;
    parameters->channels[1] = reinterpret_cast<MotionRigChannel *>(axes);
    ++axes;
    parameters->channels[2] = reinterpret_cast<MotionRigChannel *>(axes);
    parameters->pva = pva;
    parameters->controller = controller;
    parameters->callback = SHENMUE_MOTION_NULL;
    parameters->user = SHENMUE_MOTION_NULL;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void InitializeMotionTargetOverrides(MotionWork *work) {
    MotionWorkBindings *const bindings = reinterpret_cast<MotionWorkBindings *>(work);
    BindMotionTargetParameters(&bindings->targets[0], bindings->rig.lowerBody.pelvisPosition, bindings->rig.lowerBody.pelvisPva, &motionControllerIndices[0]);
    BindMotionTargetParameters(&bindings->targets[2], bindings->rig.lowerBody.leftFoot.axes, bindings->rig.lowerBody.leftFoot.pva, &motionControllerIndices[5]);
    BindMotionTargetParameters(
        &bindings->targets[3], bindings->rig.lowerBody.rightFoot.axes, bindings->rig.lowerBody.rightFoot.pva, &motionControllerIndices[6]);
    if (bindings->rig.useOverride) {
        BindMotionTargetParameters(&bindings->targets[1], bindings->rig.lowerBody.overridePelvisTarget->axes, bindings->rig.lowerBody.overridePelvisTarget->pva,
            &motionControllerIndices[7]);
    }
    BindMotionTargetParameters(&bindings->targets[4], bindings->rig.chest.target.axes, bindings->rig.chest.target.pva, &motionControllerIndices[1]);
    if (bindings->rig.useOverride) {
        BindMotionTargetParameters(
            &bindings->targets[5], bindings->rig.head.aimTargetNode->axes, bindings->rig.head.aimTargetNode->pva, &motionControllerIndices[8]);
    }
    BindMotionTargetParameters(&bindings->targets[6], bindings->rig.head.target.axes, bindings->rig.head.target.pva, &motionControllerIndices[2]);
    BindMotionTargetParameters(&bindings->targets[7], bindings->rig.rightArm.target.axes, bindings->rig.rightArm.target.pva, &motionControllerIndices[3]);
    BindMotionTargetParameters(&bindings->targets[8], bindings->rig.leftArm.target.axes, bindings->rig.leftArm.target.pva, &motionControllerIndices[4]);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void BindMotionRotationParameters(MotionRotationParameters *parameters, MotionRigAxis *axes, MotionRotationBasis *basis, float *blendAlpha) {
    parameters->channels[0] = reinterpret_cast<MotionRigChannel *>(axes);
    ++axes;
    parameters->channels[1] = reinterpret_cast<MotionRigChannel *>(axes);
    ++axes;
    parameters->channels[2] = reinterpret_cast<MotionRigChannel *>(axes);
    parameters->basis = basis;
    parameters->blendAlpha = blendAlpha;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void BindMotionTwoJointRotationParameters(MotionRotationParameters *parameters, MotionRigAxis *axes, MotionRotationBasis *basis, float *blendAlpha) {
    const std::uintptr_t base = reinterpret_cast<std::uintptr_t>(axes);
    parameters->channels[0] = reinterpret_cast<MotionRigChannel *>(base);
    parameters->channels[1] = reinterpret_cast<MotionRigChannel *>(base + sizeof(MotionRigAxis));
    parameters->basis = basis;
    parameters->blendAlpha = blendAlpha;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void InitializeMotionRotationPointers(MotionWork *work) {
    BindMotionRotationParameters(&reinterpret_cast<MotionWorkBindings *>(work)->rotations[0],
        reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.pelvisRotation, &reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.pelvisBasis,
        &work->layerBlendAlpha[0]);
    if (!reinterpret_cast<MotionWorkBindings *>(work)->rig.useOverride) {
        BindMotionRotationParameters(&reinterpret_cast<MotionWorkBindings *>(work)->rotations[1],
            reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.hips.axes, &reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.hips.basis,
            &work->layerBlendAlpha[0]);
    } else {
        BindMotionRotationParameters(&reinterpret_cast<MotionWorkBindings *>(work)->rotations[1],
            reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.overrideHips->axes,
            &reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.overrideHips->basis, &work->layerBlendAlpha[0]);
        BindMotionRotationParameters(&reinterpret_cast<MotionWorkBindings *>(work)->rotations[2],
            reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.leftHipNode->axes,
            &reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.leftHipNode->basis, &work->layerBlendAlpha[0]);
        BindMotionRotationParameters(&reinterpret_cast<MotionWorkBindings *>(work)->rotations[5],
            reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightHipNode->axes,
            &reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightHipNode->basis, &work->layerBlendAlpha[0]);
        BindMotionTwoJointRotationParameters(&reinterpret_cast<MotionWorkBindings *>(work)->rotations[8],
            reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightToe1Node->axes,
            &reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightToe1Node->basis, &work->layerBlendAlpha[0]);
        BindMotionTwoJointRotationParameters(&reinterpret_cast<MotionWorkBindings *>(work)->rotations[9],
            reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightToe2Node->axes,
            &reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightToe2Node->basis, &work->layerBlendAlpha[0]);
        BindMotionTwoJointRotationParameters(&reinterpret_cast<MotionWorkBindings *>(work)->rotations[10],
            reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightToe3Node->axes,
            &reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightToe3Node->basis, &work->layerBlendAlpha[0]);
    }
    BindMotionRotationParameters(&reinterpret_cast<MotionWorkBindings *>(work)->rotations[3],
        reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.leftThigh.axes,
        &reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.leftThigh.basis, &work->layerBlendAlpha[0]);
    BindMotionRotationParameters(&reinterpret_cast<MotionWorkBindings *>(work)->rotations[4],
        reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.leftAnkleRotation,
        &reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.leftAnkleBasis, &work->layerBlendAlpha[0]);
    BindMotionRotationParameters(&reinterpret_cast<MotionWorkBindings *>(work)->rotations[6],
        reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightThigh.axes,
        &reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightThigh.basis, &work->layerBlendAlpha[0]);
    BindMotionRotationParameters(&reinterpret_cast<MotionWorkBindings *>(work)->rotations[7],
        reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightAnkleRotation,
        &reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightAnkleBasis, &work->layerBlendAlpha[0]);
    BindMotionRotationParameters(&reinterpret_cast<MotionWorkBindings *>(work)->rotations[11],
        reinterpret_cast<MotionWorkBindings *>(work)->rig.chest.rotation.axes, &reinterpret_cast<MotionWorkBindings *>(work)->rig.chest.rotation.basis,
        &work->layerBlendAlpha[1]);
    BindMotionRotationParameters(&reinterpret_cast<MotionWorkBindings *>(work)->rotations[12],
        reinterpret_cast<MotionWorkBindings *>(work)->rig.chest.upperRotation.axes,
        &reinterpret_cast<MotionWorkBindings *>(work)->rig.chest.upperRotation.basis, &work->layerBlendAlpha[1]);
    BindMotionRotationParameters(&reinterpret_cast<MotionWorkBindings *>(work)->rotations[13],
        reinterpret_cast<MotionWorkBindings *>(work)->rig.head.baseRotation.axes, &reinterpret_cast<MotionWorkBindings *>(work)->rig.head.baseRotation.basis,
        &work->layerBlendAlpha[2]);
    BindMotionRotationParameters(&reinterpret_cast<MotionWorkBindings *>(work)->rotations[15],
        reinterpret_cast<MotionWorkBindings *>(work)->rig.head.midRotation.axes, &reinterpret_cast<MotionWorkBindings *>(work)->rig.head.midRotation.basis,
        &work->layerBlendAlpha[2]);
    if (!reinterpret_cast<MotionWorkBindings *>(work)->rig.useOverride) {
        BindMotionRotationParameters(&reinterpret_cast<MotionWorkBindings *>(work)->rotations[14],
            reinterpret_cast<MotionWorkBindings *>(work)->rig.head.aimSourceNode->axes,
            &reinterpret_cast<MotionWorkBindings *>(work)->rig.head.aimSourceNode->basis, &work->layerBlendAlpha[2]);
        BindMotionRotationParameters(&reinterpret_cast<MotionWorkBindings *>(work)->rotations[16],
            reinterpret_cast<MotionWorkBindings *>(work)->rig.head.nodeA->axes, &reinterpret_cast<MotionWorkBindings *>(work)->rig.head.nodeA->basis,
            &work->layerBlendAlpha[2]);
        BindMotionRotationParameters(&reinterpret_cast<MotionWorkBindings *>(work)->rotations[17],
            reinterpret_cast<MotionWorkBindings *>(work)->rig.head.nodeB->axes, &reinterpret_cast<MotionWorkBindings *>(work)->rig.head.nodeB->basis,
            &work->layerBlendAlpha[2]);
        BindMotionRotationParameters(&reinterpret_cast<MotionWorkBindings *>(work)->rotations[18],
            reinterpret_cast<MotionWorkBindings *>(work)->rig.head.nodeC->axes, &reinterpret_cast<MotionWorkBindings *>(work)->rig.head.nodeC->basis,
            &work->layerBlendAlpha[2]);
    }
    BindMotionRotationParameters(&reinterpret_cast<MotionWorkBindings *>(work)->rotations[19],
        reinterpret_cast<MotionWorkBindings *>(work)->rig.rightArm.shoulder.axes, &reinterpret_cast<MotionWorkBindings *>(work)->rig.rightArm.shoulder.basis,
        &work->layerBlendAlpha[3]);
    BindMotionRotationParameters(&reinterpret_cast<MotionWorkBindings *>(work)->rotations[20],
        reinterpret_cast<MotionWorkBindings *>(work)->rig.rightArm.elbow.axes, &reinterpret_cast<MotionWorkBindings *>(work)->rig.rightArm.elbow.basis,
        &work->layerBlendAlpha[3]);
    BindMotionRotationParameters(&reinterpret_cast<MotionWorkBindings *>(work)->rotations[21],
        reinterpret_cast<MotionWorkBindings *>(work)->rig.rightArm.wristRotation, &reinterpret_cast<MotionWorkBindings *>(work)->rig.rightArm.forearmBasis,
        &work->layerBlendAlpha[3]);
    BindMotionRotationParameters(&reinterpret_cast<MotionWorkBindings *>(work)->rotations[22],
        reinterpret_cast<MotionWorkBindings *>(work)->rig.rightArm.wristRotation, &reinterpret_cast<MotionWorkBindings *>(work)->rig.rightArm.wristBasis,
        &work->layerBlendAlpha[3]);
    BindMotionRotationParameters(&reinterpret_cast<MotionWorkBindings *>(work)->rotations[23],
        reinterpret_cast<MotionWorkBindings *>(work)->rig.leftArm.shoulder.axes, &reinterpret_cast<MotionWorkBindings *>(work)->rig.leftArm.shoulder.basis,
        &work->layerBlendAlpha[4]);
    BindMotionRotationParameters(&reinterpret_cast<MotionWorkBindings *>(work)->rotations[24],
        reinterpret_cast<MotionWorkBindings *>(work)->rig.leftArm.elbow.axes, &reinterpret_cast<MotionWorkBindings *>(work)->rig.leftArm.elbow.basis,
        &work->layerBlendAlpha[4]);
    BindMotionRotationParameters(&reinterpret_cast<MotionWorkBindings *>(work)->rotations[25],
        reinterpret_cast<MotionWorkBindings *>(work)->rig.leftArm.wristRotation, &reinterpret_cast<MotionWorkBindings *>(work)->rig.leftArm.forearmBasis,
        &work->layerBlendAlpha[4]);
    BindMotionRotationParameters(&reinterpret_cast<MotionWorkBindings *>(work)->rotations[26],
        reinterpret_cast<MotionWorkBindings *>(work)->rig.leftArm.wristRotation, &reinterpret_cast<MotionWorkBindings *>(work)->rig.leftArm.wristBasis,
        &work->layerBlendAlpha[4]);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void InitializeMotionClockRates(MotionWork *work) {
    work->lowerBodyClock.playbackSpeed = 1.0f;
    work->chestClock.playbackSpeed = 1.0f;
    work->headClock.playbackSpeed = 1.0f;
    work->rightArmClock.playbackSpeed = 1.0f;
    work->leftArmClock.playbackSpeed = 1.0f;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void InitializeMotionRuntime(MotionWork *work, std::uint32_t rigType) {
    MotionOverrideRig *const overrides = work->overrides;
    InitializeMotionRig(reinterpret_cast<IKRig *>(work), overrides, rigType > 16U ? 8U : rigType);
    InitializeMotionClockRates(work);
    InitializeMotionTargetOverrides(work);
    InitializeMotionRotationPointers(work);
    mtwkSetProjectionAngle(work, DEG_TO_ANG(49.998779296875f), 0);
    mtwkSetProjectionAngle(work, DEG_TO_ANG(29.9981689453125f), 1);
    mtwkSetProjectionAngle(work, DEG_TO_ANG(19.9951171875f), 2);
    work->pelvisYawClamp = DEG_TO_ANG(54.99755859375f);
    work->pelvisYawStep = DEG_TO_ANG(2.109375f);
    work->motionFlags |= 1U;
    work->retainedMotionParameter = 1;
    work->entityYaw = DEG_TO_ANG(180.0f);
    work->overrideBlendTime = -1.0f;
    work->gaitSplitFraction = -1.0f;
    work->transitionTimer = -1.0f;
    work->insideRectangleScale = 1.0f;
    work->parentBoneSelector = 9;
    std::int32_t index;
    for (index = 0; index < 5; ++index) {
        work->layerLoopStartTime[index] = -1.0f;
        work->pendingLayerStartFrame[index] = -1.0f;
        work->layerEndFrame[index] = -1.0f;
        work->layerSampleFrame[index] = -1.0f;
        work->previousMotionId[index] = 0;
        work->currentMotionId[index] = 0;
        work->queuedMotionId[index] = 0;
        work->defaultRegionStepFrames[index] = 8;
        work->layerBlendAlpha[index] = 1.0f;
    }
    for (index = 0; index < 9; ++index) {
        work->constraintStepLimit[index] = 0.05f;
        if (index == 2)
            work->constraintStepLimit[index] = 0.061359234154224396f;
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void ReleaseMotionRuntime(MotionWork *work) {
    if (work && work->overrides) {
        FreeMotionFileData(work->overrides);
        work->overrides = SHENMUE_MOTION_NULL;
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void MotionWorkReleaseNullsub(MotionWork *) {
}

void charReleaseMotionWork(Character *character) {
    MotionWorkReleaseNullsub(static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION)));
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void MotionWorkCleanup(Character *, void *payload) {
    ReleaseMotionRuntime(static_cast<MotionWork *>(payload));
}
#endif

// @verified - d
std::uint32_t mtwkApplyTransforms(const Matrix4x4 *matrix) {
    Matrix4x4 rotation;
    Vector3 axis, direction;
    axis.x = 0.0f;
    axis.y = 0.0f;
    axis.z = -1.0f;
    if (!matrix)
        return 0;
    for (std::int32_t index = 0; index < 16; ++index)
        (&rotation.rows[0].x)[index] = (&matrix->rows[0].x)[index];
    do {
        rotation.rows[3].x = rotation.rows[3].y = rotation.rows[3].z = 0.0f;
        matrix_stack::Matrix4x4_Push(&rotation);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        Matrix4x4::TransformPoint(axis, direction);
        matrix_stack::GetMatrix(&rotation);
#else
        Matrix4x4::Transform(axis, direction);
        Matrix4x4::Multiply(&rotation);
#endif
    } while (0);
    PopCurrentMatrix(1);
    if (std::fabs(direction.y) > 0.6f) {
        axis.x = 0.0f;
        axis.z = 0.0f;
        if (direction.y < 0.0f)
            axis.y = 1.0f;
        else
            axis.y = -1.0f;
        matrix_stack::Matrix4x4_Push(&rotation);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
        Matrix4x4::TransformPoint(axis, direction);
        matrix_stack::GetMatrix(&rotation);
#else
        Matrix4x4::Transform(axis, direction);
        Matrix4x4::Multiply(&rotation);
#endif
        PopCurrentMatrix(1);
    }
    return dc::sdk::ArcTan2Fixed(-direction.x, -direction.z);
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void mtwkGetArmHandPoseChanges(MotionWork *work, std::int32_t *shapes, std::int32_t *frames) {
    const std::uint16_t independent = 0x100U, mirroredFlag = 0x40U, transition = 0x10U;
    for (std::int32_t side = 0; side < 2; ++side) {
        std::int32_t table = side;
        frames[side] = 0;
        float time, blend;
        if (side == 0) {
            std::int32_t flags = work->regionFlags[RightArm];
            if (!(flags & independent))
                flags = work->regionFlags[LowerBody];
            if (flags & mirroredFlag)
                table = 1;
            if (work->regionFlags[RightArm] & transition) {
                time = work->layerSampleFrame[RightArm] < 0.0f ? 0.0f : work->layerSampleFrame[RightArm];
            } else {
                time = work->rightArmClock.ticksTime;
            }
            blend = work->rightArmClock.playbackSpeed;
        } else {
            std::int32_t flags = work->regionFlags[LeftArm];
            if (!(flags & independent))
                flags = work->regionFlags[LowerBody];
            if (flags & mirroredFlag)
                table = 0;
            if (work->regionFlags[LeftArm] & transition) {
                time = work->layerSampleFrame[LeftArm] < 0.0f ? 0.0f : work->layerSampleFrame[LeftArm];
            } else {
                time = work->leftArmClock.ticksTime;
            }
            blend = work->leftArmClock.playbackSpeed;
        }
        if (work->armHandTracks[side]) {
            if (blend < 0.00000011920929f)
                blend = 1.0f;
            std::int32_t count = work->armHandTracks[side][0];
            const std::uint16_t *track;
            if (!table) {
                track = work->armHandTracks[side] + 1;
            } else {
                const std::int32_t mirroredCount = work->armHandTracks[side][count * 2 + 1];
                track = work->armHandTracks[side] + count * 2 + 2;
                count = mirroredCount;
            }
            shapes[side] = 0;
            for (std::int32_t entry = 0; entry < count - 1; ++entry) {
                if (time >= track[entry * 2] && track[entry * 2 + 2] > time) {
                    if (track[entry * 2 + 1] != track[entry * 2 + 3]) {
                        shapes[side] = track[entry * 2 + 3];
                        frames[side] = static_cast<std::int32_t>((track[entry * 2 + 2] - time) / blend);
                    } else {
                        shapes[side] = track[entry * 2 + 1];
                    }
                    break;
                }
            }
            if (shapes[side] != work->handPoseIndices[side]) {
                work->handPoseIndices[side] = shapes[side];
                continue;
            }
        } else if (work->handPoseIndices[side]) {
            shapes[side] = 0;
            work->handPoseIndices[side] = 0;
            continue;
        }
        shapes[side] = -1;
    }
}
#endif

// @verified - d
void Queue_mot_Callback_MotFileManageTaskEntry(std::uint8_t nextFunctionIndex) {
    const std::int8_t parameter = static_cast<std::int8_t>(nextFunctionIndex);
    HLib::EnqueueTaskWithParameter(MotFileManageTaskEntry_Callback, parameter, 2, 4, TASK_MOTF);
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void mtwkClearRetiredMotionRanges() {
    ClearRetiredMotionRanges();
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void mtwkCreateForChara(Character *character, std::int32_t) {
    MotionWork *const work = static_cast<MotionWork *>(AllocateModelComponent(character, CHARACTER_MOTION, sizeof(MotionWork), MotionWorkUpdate));
    charaAssignCoreCallback(character, mtwkDrawCharacterCore);
    imgmAssignUpdateCallback(character, mtUpdateTransparencyCallback);
    mtwkAssignDrawCallback(character, DrawShenmueEntity);
    SetModelComponentCleanup(character, CHARACTER_MOTION, MotionWorkCleanup);
    std::memset(work, 0, sizeof(*work));
    work->executionFlags = 0;
    if (charGetMotionRigType(character) == 16) {
        work->overrides = reinterpret_cast<MotionOverrideRig *>(AllocateMotionFileData(sizeof(MotionOverrideRig)));
        std::memset(work->overrides, 0, sizeof(MotionOverrideRig));
    } else {
        work->overrides = SHENMUE_MOTION_NULL;
    }
    InitializeMotionRuntime(work, static_cast<std::uint32_t>(charGetMotionRigType(character)));
    work->transformSource = SHENMUE_MOTION_NULL;
    work->transformSourceIndex = 0;
    Vector3i rotation;
    GetCharacterWorldRotation(character, &rotation);
    mtwkSetEntityRotation(work, &rotation);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void MotionRigVersionError(std::uint32_t error) {
    g_motionRigError = error;
    for (;;) {
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
static float motionRigVersion = 1.1f;

void InitializeMotionRig(IKRig *rig, MotionOverrideRig *overrides, std::uint32_t rigType) {
    std::memset(rig, 0, sizeof(*rig));
    const float *const defaults = reinterpret_cast<const float *>(g_motionConstData.defaults[rigType]);
    if (std::fabs(motionRigVersion - defaults[0]) > 0.01f)
        MotionRigVersionError(0);
    rig->boneId = rigType;
    if (rigType == 16U) {
        rig->useOverride = 1U;
        g_motionUseOverride = 1;
    } else {
        rig->useOverride = 0U;
        g_motionUseOverride = 0;
    }
    if (g_motionUseOverride) {
        rig->lowerBody.overrideHips = &overrides->hips;
        rig->lowerBody.overridePelvisTarget = &overrides->pelvisTarget;
        rig->lowerBody.leftHipNode = &overrides->leftHip;
        rig->lowerBody.leftHipWorld = &overrides->leftHipWorld;
        rig->lowerBody.rightHipNode = &overrides->rightHip;
        rig->lowerBody.rightHipWorld = &overrides->rightHipWorld;
        rig->lowerBody.rightToe1Node = &overrides->rightToe1;
        rig->lowerBody.rightToe1World = &overrides->rightToe1World;
        rig->lowerBody.rightToe2Node = &overrides->rightToe2;
        rig->lowerBody.rightToe2World = &overrides->rightToe2World;
        rig->lowerBody.rightToe3Node = &overrides->rightToe3;
        rig->lowerBody.rightToe3World = &overrides->rightToe3World;
        rig->head.aimSourceNode = &overrides->headAimSource;
        rig->head.aimTargetNode = &overrides->headAimTarget;
        rig->head.aimSourceWorld = &overrides->headAimSourceWorld;
        rig->head.nodeA = &overrides->headA;
        rig->head.nodeAWorld = &overrides->headAWorld;
        rig->head.nodeB = &overrides->headB;
        rig->head.nodeBWorld = &overrides->headBWorld;
        rig->head.nodeC = &overrides->headC;
        rig->head.nodeCWorld = &overrides->headCWorld;
    }
    if (!g_motionUseOverride)
        rig->chest.rotation.axes[1].dimensions[0] = defaults[1];
    else
        rig->chest.rotation.axes[2].dimensions[0] = defaults[2];
    rig->chest.target.axes[0].dimensions[0] = defaults[3];
    rig->chest.upperRotation.axes[0].dimensions[0] = defaults[4];
    if (!g_motionUseOverride) {
        rig->head.baseRotation.axes[0].dimensions[0] = defaults[5];
        rig->head.midRotation.axes[0].dimensions[0] = defaults[10];
        rig->head.eyeOffset.x = defaults[12];
        rig->head.eyeOffset.z = defaults[13];
    } else {
        rig->head.baseRotation.axes[1].dimensions[0] = defaults[6];
        rig->head.aimSourceNode->axes[0].dimensions[0] = defaults[7];
        rig->head.aimSourceNode->axes[1].dimensions[0] = defaults[8];
        rig->head.aimTargetNode->axes[0].dimensions[0] = defaults[9];
        rig->head.midRotation.axes[0].dimensions[0] = defaults[10];
        rig->head.midRotation.axes[1].dimensions[0] = defaults[11];
        rig->head.nodeA->axes[1].dimensions[0] = defaults[14];
        rig->head.nodeA->axes[2].dimensions[0] = defaults[15];
        rig->head.nodeB->axes[1].dimensions[0] = defaults[16];
        rig->head.nodeB->axes[2].dimensions[0] = defaults[17];
        rig->head.nodeC->axes[0].dimensions[0] = defaults[18];
        rig->head.nodeC->axes[2].dimensions[0] = defaults[19];
    }
    if (!g_motionUseOverride) {
        rig->rightArm.shoulder.axes[0].dimensions[0] = defaults[20];
        rig->rightArm.shoulder.axes[2].dimensions[0] = defaults[22];
        rig->rightArm.elbow.axes[2].dimensions[0] = defaults[25];
        rig->rightArm.target.axes[1].dimensions[0] = defaults[26];
        rig->rightArm.wristRotation[0].dimensions[0] = defaults[29];
        rig->rightArm.wristRotation[1].dimensions[0] = defaults[28];
    } else {
        rig->rightArm.shoulder.axes[0].dimensions[0] = defaults[20];
        rig->rightArm.shoulder.axes[1].dimensions[0] = defaults[21];
        rig->rightArm.shoulder.axes[2].dimensions[0] = defaults[22];
        rig->rightArm.elbow.axes[0].dimensions[0] = defaults[23];
        rig->rightArm.elbow.axes[1].dimensions[0] = defaults[24];
        rig->rightArm.elbow.axes[2].dimensions[0] = defaults[25];
        rig->rightArm.target.axes[1].dimensions[0] = defaults[26];
    }
    rig->rightArm.target.axes[0].dimensions[0] = defaults[27];
    if (!g_motionUseOverride) {
        rig->leftArm.shoulder.axes[0].dimensions[0] = defaults[30];
        rig->leftArm.shoulder.axes[2].dimensions[0] = defaults[32];
        rig->leftArm.elbow.axes[2].dimensions[0] = defaults[35];
        rig->leftArm.target.axes[1].dimensions[0] = defaults[36];
        rig->leftArm.wristRotation[0].dimensions[0] = defaults[39];
        rig->leftArm.wristRotation[1].dimensions[0] = defaults[38];
    } else {
        rig->leftArm.shoulder.axes[0].dimensions[0] = defaults[30];
        rig->leftArm.shoulder.axes[1].dimensions[0] = defaults[31];
        rig->leftArm.shoulder.axes[2].dimensions[0] = defaults[32];
        rig->leftArm.elbow.axes[0].dimensions[0] = defaults[33];
        rig->leftArm.elbow.axes[1].dimensions[0] = defaults[34];
        rig->leftArm.elbow.axes[2].dimensions[0] = defaults[35];
        rig->leftArm.target.axes[1].dimensions[0] = defaults[36];
    }
    rig->leftArm.target.axes[0].dimensions[0] = defaults[37];
    if (!g_motionUseOverride) {
        rig->lowerBody.hips.axes[1].dimensions[0] = defaults[40];
        rig->lowerBody.leftThigh.axes[1].dimensions[0] = defaults[47];
        rig->lowerBody.leftThigh.axes[2].dimensions[0] = defaults[48];
        rig->lowerBody.leftFootRotation[1].dimensions[0] = defaults[51];
        rig->lowerBody.leftFootRotation[2].dimensions[0] = defaults[52];
        rig->lowerBody.rightThigh.axes[1].dimensions[0] = defaults[57];
        rig->lowerBody.rightThigh.axes[2].dimensions[0] = defaults[58];
        rig->lowerBody.rightFootRotation[1].dimensions[0] = defaults[61];
        rig->lowerBody.rightFootRotation[2].dimensions[0] = defaults[62];
    } else {
        rig->lowerBody.overrideHips->axes[2].dimensions[0] = defaults[41];
        rig->lowerBody.overridePelvisTarget->axes[0].dimensions[0] = defaults[42];
        rig->lowerBody.leftHipNode->axes[0].dimensions[0] = defaults[43];
        rig->lowerBody.leftHipNode->axes[1].dimensions[0] = defaults[44];
        rig->lowerBody.leftHipNode->axes[2].dimensions[0] = defaults[45];
        rig->lowerBody.leftThigh.axes[0].dimensions[0] = defaults[46];
        rig->lowerBody.leftThigh.axes[1].dimensions[0] = defaults[47];
        rig->lowerBody.rightHipNode->axes[0].dimensions[0] = defaults[53];
        rig->lowerBody.rightHipNode->axes[1].dimensions[0] = defaults[54];
        rig->lowerBody.rightHipNode->axes[2].dimensions[0] = defaults[55];
        rig->lowerBody.rightThigh.axes[0].dimensions[0] = defaults[56];
        rig->lowerBody.rightThigh.axes[1].dimensions[0] = defaults[57];
        rig->lowerBody.rightToe1Node->axes[0].dimensions[0] = defaults[63];
        rig->lowerBody.rightToe1Node->axes[1].dimensions[0] = defaults[64];
        rig->lowerBody.rightToe2Node->axes[0].dimensions[0] = defaults[65];
        rig->lowerBody.rightToe2Node->axes[1].dimensions[0] = defaults[66];
        rig->lowerBody.rightToe3Node->axes[0].dimensions[0] = defaults[67];
        rig->lowerBody.rightToe3Node->axes[1].dimensions[0] = defaults[68];
    }
    rig->lowerBody.leftFoot.axes[0].dimensions[0] = defaults[50];
    rig->lowerBody.leftFoot.axes[1].dimensions[0] = defaults[49];
    rig->lowerBody.rightFoot.axes[0].dimensions[0] = defaults[60];
    rig->lowerBody.rightFoot.axes[1].dimensions[0] = defaults[59];
}
#endif

// @verified - d
bool mtwkRequestMotion(MotionWork *work, std::int32_t motion, MotionRegion region) {
    const std::uint32_t index = static_cast<std::uint32_t>(region);
    if (work && index < 5) {
        work->queuedMotionId[index] = motion;
        if (index && (work->regionFlags[index] & 0x800U))
            work->regionFlags[index] &= 0xf7ffU;
        return true;
    }
    return false;
}

// @verified - d
void mtwkRequestRegionDetach(MotionWork *work, MotionRegion region) {
    if (!work)
        return;
    if (region == LowerBody) {
        for (std::int32_t index = 1; index < 5; ++index)
            work->regionFlags[index] |= 0x400U;
    } else if (static_cast<std::uint32_t>(region) < 5) {
        work->regionFlags[region] |= 0x400U;
    }
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void mtwkInitFlags(MotionWork *work, std::uint32_t region) {
    if (!work)
        return;
    if (!region) {
        for (std::int32_t index = 1; index < 5; ++index)
            work->regionFlags[index] |= 0x200U;
    } else if (region < 5) {
        work->regionFlags[region] |= 0x200U;
    }
}
#endif

// @verified - d
void mtwkCopyRegionTimes(const MotionWork *work, float *split, float *current, float *maximum, MotionRegion region) {
    const std::uint32_t index = static_cast<std::uint32_t>(region);
    if (!work)
        return;
    MotionRegionClock *clocks[5];
    mtwkGetRegionClocks(const_cast<MotionWork *>(work), clocks, clocks + 1, clocks + 2, clocks + 3, clocks + 4);
    if (split)
        *split = clocks[index]->splitTimeFrames;
    if (current)
        *current = clocks[index]->ticksTime;
    if (maximum)
        *maximum = clocks[index]->maxTime;
}

// @verified - d
float mtwkGetSplitFrame(const MotionWork *work, MotionRegion region) {
    if (!work)
        return -1.0f;
    float split;
    mtwkCopyRegionTimes(work, &split, 0, 0, region);
    return split;
}

// @verified - d
float mtwkGetFrame(const MotionWork *work, MotionRegion region) {
    if (!work)
        return -1.0f;
    float frame;
    mtwkCopyRegionTimes(work, 0, &frame, 0, region);
    if (!(work->regionFlags[region] & 0x10U))
        return frame;
    const float speed = mtwkGetSpeed(work, region);
    switch (work->currentRootMode) {
    case 3:
        frame = work->layerSampleFrame[region] - speed * (static_cast<float>(work->blendTotalFrames[region]) - frame);
        while (frame < 0.0f)
            frame += static_cast<float>(GetMotionFrameCount(work->currentMotionId[region])) - 1.0f;
        break;
    case 4:
        frame = (static_cast<float>(GetMotionFrameCount(work->previousMotionId[region])) - 1.0f) -
            (static_cast<float>(work->blendTotalFrames[region]) - frame) * speed;
        break;
    case 5:
        if (!region)
            frame = work->transitionTimer;
        break;
    }
    return frame;
}

// @verified - d
float mtwkGetEndFrame(const MotionWork *work, MotionRegion region) {
    if (!work)
        return -1.0f;
    float maximum;
    mtwkCopyRegionTimes(work, 0, 0, &maximum, region);
    return maximum;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float mtwkGetRegionLoopStartOverride(const MotionWork *work, std::uint32_t region) {
    if (!work || region > 4 || !(work->regionFlags[region] & MRF_Transition))
        return -1.0f;
    if (work->regionFlags[region] & 0x100U)
        return work->layerLoopStartTime[region];
    return work->layerLoopStartTime[0];
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float mtwkGetRegionTimeOverride(const MotionWork *work, std::uint32_t region) {
    if (!work || region > 4 || !(work->regionFlags[region] & MRF_Transition))
        return -1.0f;
    if (work->regionFlags[region] & 0x100U)
        return work->layerSampleFrame[region];
    return work->layerSampleFrame[0];
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float mtwkGetRegionEndFrameOverride(const MotionWork *work, std::uint32_t region) {
    if (!work || region > 4 || !(work->regionFlags[region] & MRF_Transition))
        return -1.0f;
    if (work->regionFlags[region] & 0x100U)
        return work->layerEndFrame[region];
    return work->layerEndFrame[0];
}
#endif

// @verified - d
float mtwkGetSpeed(const MotionWork *work, MotionRegion region) {
    if (!work)
        return 0.0f;
    MotionRegionClock *clocks[5];
    mtwkGetRegionClocks(const_cast<MotionWork *>(work), clocks, clocks + 1, clocks + 2, clocks + 3, clocks + 4);
    return clocks[static_cast<std::uint32_t>(region)]->playbackSpeed;
}

// @verified - d
void mtwkSetLayerFrameRange(MotionWork *work, float loopStart, float start, float end, MotionRegion region) {
    const std::uint32_t index = static_cast<std::uint32_t>(region);
    if (!work || index >= 5)
        return;
    if (loopStart < 0.0f)
        loopStart = 0.0f;
    work->layerLoopStartTime[index] = loopStart;
    if (start < 0.0f)
        start = 0.0f;
    work->pendingLayerStartFrame[index] = start;
    if (end < 0.0f)
        end = 10000.0f;
    work->layerEndFrame[index] = end;
}

// @verified - d
void mtwkSetLayerLoopStartTime(MotionWork *work, float frame, MotionRegion region) {
    if (work) {
        if (frame < 0.0f)
            frame = 0.0f;
        if (static_cast<std::uint32_t>(region) < 5)
            work->layerLoopStartTime[static_cast<std::uint32_t>(region)] = frame;
    }
}

// @verified - d
void mtwkSetRegionEvalTime(MotionWork *work, float frame, MotionRegion region) {
    if (work) {
        if (frame < 0.0f)
            frame = 0.0f;
        if (static_cast<std::uint32_t>(region) < 5)
            work->pendingLayerStartFrame[static_cast<std::uint32_t>(region)] = frame;
    }
}

// @verified - d
void mtwkSetLayerEndFrame(MotionWork *work, float frame, MotionRegion region) {
    if (work) {
        if (frame < 0.0f)
            frame = 10000.0f;
        if (static_cast<std::uint32_t>(region) < 5)
            work->layerEndFrame[static_cast<std::uint32_t>(region)] = frame;
    }
}

// @verified - d
void mtwkSetSpeed(MotionWork *work, float speed, MotionRegion region) {
    if (!work || speed < 0.0f || speed > 4.0f)
        return;
    MotionRegionClock *clocks[5];
    mtwkGetRegionClocks(work, clocks, clocks + 1, clocks + 2, clocks + 3, clocks + 4);
    clocks[static_cast<std::uint32_t>(region)]->playbackSpeed = speed;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void mtwkGetRootPosition(const MotionWork *work, Vector3 *position) {
    position->x = work->cachedRootPosition.x;
    position->y = work->cachedRootPosition.y;
    position->z = work->cachedRootPosition.z;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void mtwkUpdateRootDeltaVector(const MotionWork *work, Vector3 *delta) {
    if (work->motionFlags & 1U) {
        delta->x = work->cachedRootPosition.x - work->previousRootPosition.x;
        delta->y = work->accumulatedTranslation.y;
        delta->z = work->cachedRootPosition.z - work->previousRootPosition.z;
    } else {
        delta->x = work->accumulatedTranslation.x;
        delta->y = work->accumulatedTranslation.y;
        delta->z = work->accumulatedTranslation.z;
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void mtwkSetRootPosition(MotionWork *work, const Vector3 *position) {
    work->cachedRootPosition.x = position->x;
    work->cachedRootPosition.y = position->y;
    work->cachedRootPosition.z = position->z;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t mtwkGetEntityYaw(const MotionWork *work) {
    return work->entityYaw;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void mtwkGetEntityRotation(const MotionWork *work, Vector3i *rotation) {
    rotation->x = work->entityRotationX;
    rotation->y = work->entityYaw;
    rotation->z = work->entityRotationZ;
}
#endif

// @verified - d
std::int32_t CaptureMotionSnapshot(shen_name subject) {
    std::int32_t index = -1;
    if (g_motionSnapshotState == kMotionSnapshotSlots)
        return index;
    MotionWork *const work = scenFindMotionwork(subject);
    if (!work)
        return index;
    for (index = 0; index < static_cast<std::int32_t>(kMotionSnapshotSlots); ++index) {
        if (g_motionSnapshots[index].slot == -1) {
            g_motionSnapshots[index].slot = index;
            break;
        }
    }
    ++g_motionSnapshotState;
    g_motionSnapshots[index].character = subject;
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, subject);
    WriteMotionSnapshotData(&g_motionSnapshots[index], character, work);
    return index;
}

// @verified - d
std::uint32_t ReleaseMotionSnapshot(std::uint32_t index) {
    if (index > kMotionSnapshotSlots)
        return 0;
    if (g_motionSnapshots[index].slot != -1) {
        std::memset(g_motionSnapshots + index, 0, sizeof(MotionSnapshot));
        g_motionSnapshots[index].slot = -1;
        g_motionSnapshots[index].character = EMPTY_MOTION_SNAPSHOT_CHARACTER;
        --g_motionSnapshotState;
    }
    return 1;
}

struct MotionFootstepAttribute {
    std::uint32_t primary, secondary;
};

// @verified - d
void mtwkApplySnapshot(MotionWork *work, const MotionSnapshot *snapshot) {
    MotionRegionClock *clocks[5];
    std::int32_t index;
    mtwkGetRegionClocks(work, clocks, clocks + 1, clocks + 2, clocks + 3, clocks + 4);
    work->requestedRootMode = snapshot->requestedRootMode;
    work->retainedMotionParameter = snapshot->retainedMotionParameter;
    work->retainedCollisionValues[0] = snapshot->retainedCollisionValues[0];
    work->retainedCollisionValues[1] = snapshot->retainedCollisionValues[1];
    work->retainedCollisionValues[2] = snapshot->retainedCollisionValues[2];
    work->executionFlags = snapshot->executionFlags;
    reinterpret_cast<MotionFootstepAttribute *>(work->footstepAttributes)[0] =
        reinterpret_cast<const MotionFootstepAttribute *>(snapshot->retainedExecutionWords)[0];
    reinterpret_cast<MotionFootstepAttribute *>(work->footstepAttributes)[1] =
        reinterpret_cast<const MotionFootstepAttribute *>(snapshot->retainedExecutionWords)[1];
    if (snapshot->motionFlags & 1U)
        work->motionFlags |= 1U;
    else
        work->motionFlags &= ~1U;
    if (snapshot->motionFlags & 8U)
        work->motionFlags |= 8U;
    else
        work->motionFlags &= ~8U;
    if (snapshot->motionFlags & 0x200U)
        work->motionFlags |= 0x200U;
    else
        work->motionFlags &= ~0x200U;
    if (snapshot->motionFlags & 0x40U)
        work->motionFlags |= 0x40U;
    else
        work->motionFlags &= ~0x40U;
    if (snapshot->motionFlags & 0x8000U)
        work->motionFlags |= 0x8000U;
    else
        work->motionFlags &= ~0x8000U;
    if (snapshot->motionFlags & 0x10000U)
        work->motionFlags |= 0x10000U;
    else
        work->motionFlags &= ~0x10000U;
    if (snapshot->sequenceFlags & 0x800000U)
        work->sequenceFlags |= 0x800000U;
    else
        work->sequenceFlags &= ~0x800000U;
    if (snapshot->sequenceFlags & 0x1000000U)
        work->sequenceFlags |= 0x1000000U;
    else
        work->sequenceFlags &= ~0x1000000U;
    for (index = 0; index < 5; ++index) {
        work->defaultRegionStepFrames[index] = snapshot->defaultStepFrames[index];
        clocks[index]->playbackSpeed = snapshot->playbackSpeed[index];
    }
    for (index = 0; index < 9; ++index)
        work->constraintStepLimit[index] = snapshot->constraintStepLimit[index];
}

// @verified - d
std::uint32_t RestoreMotionSnapshot(shen_name subject, std::uint32_t index) {
    if (index >= kMotionSnapshotSlots || g_motionSnapshots[index].slot == -1)
        return 0;
    MotionWork *const work = scenFindMotionwork(subject);
    if (!work)
        return 0;
    mtwkApplySnapshot(work, &g_motionSnapshots[index]);
    return 1;
}

// @verified - d
std::uint32_t RestoreMotionSnapshotPlacement(shen_name subject, std::uint32_t index) {
    Vector3i rotation;
    if (index >= kMotionSnapshotSlots || g_motionSnapshots[index].slot == -1)
        return 0;
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, subject);
    if (!character)
        return 0;
    MotionWork *const work = static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION));
    if (!work)
        return 0;
    const MotionSnapshot *const snapshot = &g_motionSnapshots[index];
    mtwkApplySnapshot(work, snapshot);
    if (snapshot->motionFlags & 0x200U)
        SetCharacterWorldPosition(character, &snapshot->worldPosition);
    else
        SetCharacterWorldPosition(character, &snapshot->rootPosition);
    rotation.x = snapshot->worldRotation.x;
    rotation.y = snapshot->worldRotation.y;
    rotation.z = snapshot->worldRotation.z;
    charaSetWorldRot(character, rotation);
    work->motionFlags |= 0x2000U;
    work->motionFlags &= ~0x4000U;
    work->motionFlags &= ~0x20000U;
    work->regionFlags[0] |= 0x80U;
    work->regionFlags[0] &= ~0x200U;
    work->regionFlags[0] &= ~0x400U;
    work->queuedMotionId[0] = snapshot->motionIds[0];
    work->layerLoopStartTime[0] = snapshot->splitFrames[0];
    work->pendingLayerStartFrame[0] = snapshot->ticksTime[0];
    work->layerEndFrame[0] = snapshot->maxTime[0];
    if (snapshot->regionFlags[0] & 4U)
        work->regionFlags[0] |= 0x1000U;
    if (mtwkSetRegionFlag(work, 70, 1, static_cast<MotionRegion>(0)))
        work->regionFlags[0] |= 0x20U;
    for (std::int32_t region = 1; region < 5; ++region) {
        if (!(snapshot->regionFlags[region] & 0x100U))
            continue;
        work->regionFlags[region] = (work->regionFlags[region] | 0x80U) & ~0x200U;
        work->regionFlags[region] &= ~0x400U;
        work->queuedMotionId[region] = snapshot->motionIds[region];
        work->layerLoopStartTime[region] = snapshot->splitFrames[region];
        work->pendingLayerStartFrame[region] = snapshot->ticksTime[region];
        work->layerEndFrame[region] = snapshot->maxTime[region];
        if (snapshot->regionFlags[region] & 4U)
            work->regionFlags[region] |= 0x1000U;
        if (mtwkSetRegionFlag(work, 70, 1, static_cast<MotionRegion>(region)))
            work->regionFlags[region] |= 0x20U;
    }
    return 1;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
bool charRequestMotion(Character *character, std::int32_t motion, MotionRegion region) {
    return mtwkRequestMotion(static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION)), motion, region);
}

std::uint16_t charGetAnimationId(Character *character, MotionRegion region) {
    return mtwkGetAnimationId(static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION)), region);
}

std::uint32_t charGetTrackedRootMode(Character *character) {
    return mtwkGetTrackedRootMode(static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION)));
}

bool charSetRequestedRootMode(Character *character, std::uint32_t mode) {
    return mtwkSetRequestedRootMode(static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION)), mode);
}

float charGetTransitionFramesRemaining(Character *character, MotionRegion region) {
    return mtwkGetTransitionFramesRemaining(static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION)), region);
}

bool charSetFrameTime(Character *character, std::uint32_t frames, MotionRegion region) {
    return mtwkSetFrameTime(static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION)), frames, region);
}

void charUpdateMotionDisplay(Character *character) {
    MotionWork *const work = static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION));
    mtwkSetActive(work);
    EV_SetCharDisplay(character, CVM_MotionworkDisplayCheck, mtwkEvaluateMotionFrame(work));
    mtwkSetActive(SHENMUE_MOTION_NULL);
}
#endif

// @verified - d
MotionWork *charFindMotionwork(const Character *character) {
    return static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION));
}

// @verified - d
bool HasCharacterMotionWork(const Character *character) {
    if (charFindComp(character, CHARACTER_MOTION))
        return true;
    return false;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void CopyRecursiveModelNodeTransform(MT7Node *node, std::uint32_t selector, Matrix4x4 *output) {
    if (node->sibling)
        matrix_stack::Matrix4x4_Push();
    Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
    Matrix4x4::SetViewMatrixFromEuler(node->rotation.x, node->rotation.y, node->rotation.z);
    if (static_cast<std::uint32_t>(static_cast<std::int32_t>(node->id)) == selector)
        matrix_stack::GetMatrix(output);
    if (node->child)
        CopyRecursiveModelNodeTransform(node->child, selector, output);
    if (node->sibling) {
        PopCurrentMatrix(1);
        CopyRecursiveModelNodeTransform(node->sibling, selector, output);
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void CopyCharacterSourceTransform(const Character *source, std::uint32_t selector, Matrix4x4 *output) {
    matrix_stack::Matrix4x4_Push();
    matrix_stack::UnitCurrentMatrix();
    MT7Node *const model = static_cast<MT7Node *>(source->model);
    if (model) {
        Vector3 position;
        Vector3i rotation;
        GetCharacterWorldPosition(source, &position);
        GetCharacterWorldRotation(source, &rotation);
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        Matrix4x4::SetViewMatrixFromEuler(rotation.x, rotation.y, rotation.z);
        matrix_stack::GetMatrix(output);
        CopyRecursiveModelNodeTransform(model, selector, output);
    } else {
        matrix_stack::GetMatrix(output);
    }
    PopCurrentMatrix(1);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void CopyCharacterSourcePosition(const Character *source, std::uint32_t selector, Vector3 *position) {
    Matrix4x4 transform;
    CopyCharacterSourceTransform(source, selector, &transform);
    position->x = transform.rows[3].x;
    position->y = transform.rows[3].y;
    position->z = transform.rows[3].z;
}
#endif

// @verified - d
std::uint32_t mtwkGetBoneYaw(const MotionWork *work, std::int32_t bone) {
    return mtwkApplyTransforms(mtwkGetBoneTransform(work, bone));
}

// @verified - d
void mtwkSetEntityRotation(MotionWork *work, const Vector3i *rotation) {
    work->entityRotationX = rotation->x;
    work->entityYaw = rotation->y;
    work->entityRotationZ = rotation->z;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void mtwkTranslateRigInterpolated(MotionWork *work, float x, float y, float z) {
    if (!work)
        return;
    work->accumulatedTranslation.x += x;
    work->accumulatedTranslation.y += y;
    work->accumulatedTranslation.z += z;
    mtwkAdvanceInterpolated(work, x, y, z);
    mtwkCopyInterpolated(work, x, y, z);
    work->cachedRootPosition.x = reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.pelvisWorld.rows[3].x;
    work->cachedRootPosition.z = reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.pelvisWorld.rows[3].z;
}

void mtwkGetPendingRootTranslation(const MotionWork *work, Vector3 *translation) {
    if (work->motionFlags & 1U) {
        translation->x = work->cachedRootPosition.x - work->previousRootPosition.x - work->accumulatedTranslation.x;
        translation->y = 0.0f;
        translation->z = work->cachedRootPosition.z - work->previousRootPosition.z - work->accumulatedTranslation.z;
    } else {
        translation->x = 0.0f;
        translation->y = 0.0f;
        translation->z = 0.0f;
    }
}

void mtwkGetAccumulatedTranslation(const MotionWork *work, Vector3 *translation) {
    translation->x = work->accumulatedTranslation.x;
    translation->y = work->accumulatedTranslation.y;
    translation->z = work->accumulatedTranslation.z;
}

static std::uint32_t ApplyMotionExtraFlag(MotionWork *work, std::uint32_t flag, std::int32_t operation) {
    std::uint32_t previous = 0;
    std::uint32_t *word;
    if (flag < 32) {
        word = &work->motionFlags;
    } else {
        if (flag > 63)
            return previous;
        flag -= 32;
        word = &work->sequenceFlags;
    }
    const std::uint32_t mask = 1U << flag;
    previous = mask & *word;
    if (operation == 0)
        *word |= mask;
    else if (operation == 2)
        *word &= ~(1U << flag);
    return previous;
}
#endif

// @verified - d
std::uint32_t mtwkSetExtraFlag(MotionWork *work, std::uint32_t flag, std::int32_t operation) {
    std::uint32_t result;
    if (!work)
        return 0;
    switch (flag) {
    case 0:
    case 1:
    case 3:
    case 6:
    case 9:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 55:
    case 56:
        result = ApplyMotionExtraFlag(work, flag, operation);
        break;
    default:
        result = ApplyMotionExtraFlag(work, flag, 1);
        break;
    }
    return result;
}

// @verified - d
std::int32_t ParentBoneId(std::uint32_t selector) {
    switch (selector) {
    case 0:
        return 0;
    case 2:
        return 1;
    case 7:
        return 2;
    case 10:
        return 3;
    case 17:
        return 4;
    case 25:
        return 5;
    case 30:
        return 6;
    case 38:
        return 7;
    case 34:
        return 8;
    default:
        return -1;
    }
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t ControllerBoneId(std::uint32_t controller) {
    switch (controller) {
    case 0:
        return 0;
    case 1:
        return 2;
    case 2:
        return 7;
    case 3:
        return 10;
    case 4:
        return 17;
    case 5:
        return 25;
    case 6:
        return 30;
    case 7:
        return 38;
    case 8:
        return 34;
    default:
        return -1;
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void UpdateMotionFootstepAttributes(Character *character) {
    MotionWork *const work = static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION));
    std::uint8_t attributes[3];
    if (MOTN_GetFootstepAttributes(work->currentMotionId[0], attributes) == 0) {
        work->footstepAttributes[attributes[2]][0] = attributes[0];
        work->footstepAttributes[attributes[2]][1] = attributes[1];
    } else {
        work->footstepAttributes[0][0] = 0;
        work->footstepAttributes[0][1] = 0;
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t charGetFootstepAttributes(Character *character, std::int32_t foot, std::uint32_t *secondary) {
    MotionWork *const work = static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION));
    *secondary = work->footstepAttributes[foot][1];
    return work->footstepAttributes[foot][0];
}

// @verified - d
void charSetFootstepAttributes(Character *character, std::int32_t foot, std::uint32_t secondary, std::uint32_t primary) {
    MotionWork *const work = static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION));
    work->footstepAttributes[foot][1] = secondary;
    work->footstepAttributes[foot][0] = primary;
}

std::int32_t charFindFootstepAttributes(Character *character, std::uint32_t primary, std::uint32_t *secondary) {
    std::int32_t found = 0;
    MotionWork *const work = static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION));
    if (work->footstepAttributes[0][0] == primary) {
        if (secondary)
            *secondary = work->footstepAttributes[0][1];
        found = 1;
    } else if (work->footstepAttributes[1][0] == primary) {
        if (secondary)
            *secondary = work->footstepAttributes[1][1];
        found = 1;
    }
    return found;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void MotionWorkUpdate(Character *character, void *payload) {
    MotionWork *work;
    Vector3 position;
    Vector3 transformed;
    Vector3i rotation;
    std::int32_t shapes[2], frames[2];
    work = static_cast<MotionWork *>(payload);
    if (work->drawOverrideData || charIsMotionworkCulled(character))
        return;
    mtwkSetActive(work);
    scenGetCharaName(SHENMUE_MOTION_CONST_CAST(MapControlData, HLib::GetTaskParameter(g_mapControlTask))->scenarioTask, character);
    if (character == SHENMUE_MOTION_CONST_CAST(MapControlData, HLib::GetTaskParameter(g_mapControlTask))->ryo)
        g_mtwkCurrentIsRyo = 1;
    else
        g_mtwkCurrentIsRyo = 0;
    mtwkClearSearchHandRequests(work);
    UpdateMotionTransitionControllers(character);
    GetCharacterWorldPosition(character, &position);
    mtwkSetRootPosition(work, &position);
    GetCharacterWorldRotation(character, &rotation);
    mtwkSetEntityRotation(work, &rotation);
    if (work->transformSource) {
        CopyCharacterSourceTransform(work->transformSource, static_cast<std::uint32_t>(work->transformSourceIndex), &work->overrideViewSpaceTransform);
        matrix_stack::Matrix4x4_Push();
        matrix_stack::LoadMatrix(&work->overrideViewSpaceTransform);
        Matrix4x4::preTranslate(work->transformOffsetPosition.x, work->transformOffsetPosition.y, work->transformOffsetPosition.z);
        Matrix4x4::SetViewMatrixFromEuler(work->transformOffsetRotation.x, work->transformOffsetRotation.y, work->transformOffsetRotation.z);
        matrix_stack::GetMatrix(&work->overrideViewSpaceTransform);
        PopCurrentMatrix(1);
    }
    if (character->model) {
        work->depthScale = GetCharacterDisplayRadius(character);
        EV_SetCharDisplay(character, CVM_MotionworkDisplayCheck, mtwkEvaluateMotionFrame(work));
    } else {
        EV_SetCharDisplay(character, CVM_MotionworkDisplayCheck, 0);
    }
    figpToggleMotionworkFlag(character, work);
    UpdateMotionFootstepAttributes(character);
    if (charIsMotionworkEnabled(character)) {
        mtwkUpdateRootDeltaVector(work, &position);
        AddCharacterChildSpacePosition(character, &position);
    }
    if (work->pendingFootstepCue) {
        work->pendingFootstepCue = (work->pendingFootstepCue & 0x87ffffU) | ((work->materialFlags & 0x7fU) << 24U) |
            (static_cast<std::uint32_t>(work->environmentFlags & 0xf0U) << 15U);
        if (work->materialFlags & 0x80U)
            PlayFootstepForNode(work->pendingFootstepCue, character);
        else
            PlayFootstepForNode(work->pendingFootstepCue, SHENMUE_MOTION_NULL);
    }
    std::uint8_t occurrence = 1;
    const std::int8_t *command;
    while ((command = reinterpret_cast<const std::int8_t *>(mtwkFindHandPoseEvent(work, occurrence++)))) {
        std::int32_t side;
        if (work->regionFlags[0] & 0x40U)
            side = command[3];
        else
            side = 1 - command[3];
        hmdlSetPoseFor(character, side, motionAuxiliaryHandPoses[static_cast<std::uint8_t>(command[4])][side], command[6] + command[7] * 256);
    }
    mtwkGetArmHandPoseChanges(work, shapes, frames);
    for (std::int32_t side = 0; side < 2; ++side) {
        if (shapes[side] != -1) {
            frames[side] = frames[side] < 0 ? 3 : frames[side];
            hmdlSetPoseFor(character, side, motionArmHandPoses[shapes[side]][side], frames[side]);
        }
    }
    BindMotionControllerMatricesToModelNodes(work, character->model);
    mtwkGetRootPosition(work, &position);
    Matrix4x4::TransformPoint(position, transformed);
    do {
        float *view = &character->viewPosition.x;
        const float *source = &transformed.x;
        *view++ = *source++;
        *view++ = *source++;
        *view = *source;
    } while (0);
    const float distance = Vector3::CalcMagnitude(transformed);
    if (0.0000099999997f > distance)
        character->drawDistance = 0.0f;
    else
        character->drawDistance = distance;
    GetSearchHandWristAngles(*work, g_characterHandWristAngles);
    mtwkSetActive(SHENMUE_MOTION_NULL);
}
#endif

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void RebaseSolvedMatrix(Matrix4x4 *matrix, const Matrix4x4 *root) {
    matrix_stack::Matrix4x4_Push();
    Matrix4x4::MultiplyWithPrePassMatrix(matrix);
    matrix_stack::GetMatrix(matrix);
    matrix_stack::LoadMatrix(root);
    Matrix4x4::MultiplyWithPrePassMatrix(matrix);
    matrix_stack::GetMatrix(matrix);
    PopCurrentMatrix(1);
}
#endif

} // namespace

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float ClampMotionTimePair(MotionTimePair *pair, float split, float ticks, float maximum) {
    if (!(0.0f > maximum) && pair->maxTime > maximum)
        pair->maxTime = maximum;
    if (!(0.0f > split) && pair->maxTime > split)
        pair->splitTimeFrames = split;
    else if (!(pair->maxTime > split))
        pair->splitTimeFrames = pair->maxTime;
    if (!(0.0f > ticks))
        pair->ticksTime = ticks;
    if (!(pair->ticksTime < pair->maxTime))
        pair->ticksTime = pair->maxTime;
    else if (!(pair->ticksTime > pair->splitTimeFrames))
        pair->ticksTime = pair->splitTimeFrames;
    return pair->ticksTime;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void mtwkRebaseSolvedMatrices(MotionWork *work) {
    matrix_stack::PushUnitMatrix();
    Vector3 origin;
    origin.x = work->transformStart->rows[3].x;
    origin.y = work->transformStart->rows[3].y;
    origin.z = work->transformStart->rows[3].z;
    Matrix4x4::preTranslate(origin.x, origin.y, origin.z);
    matrix_stack::InvertMatrix(SHENMUE_MOTION_NULL);
    RebaseSolvedMatrix(&reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.pelvisWorld, work->transformStart);
    RebaseSolvedMatrix(&reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.hipsWorld, work->transformStart);
    RebaseSolvedMatrix(&reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.leftThighWorld, work->transformStart);
    RebaseSolvedMatrix(&reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.leftShinWorld, work->transformStart);
    RebaseSolvedMatrix(&reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.leftAnkleWorld, work->transformStart);
    RebaseSolvedMatrix(&reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightThighWorld, work->transformStart);
    RebaseSolvedMatrix(&reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightShinWorld, work->transformStart);
    RebaseSolvedMatrix(&reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightAnkleWorld, work->transformStart);
    RebaseSolvedMatrix(&reinterpret_cast<MotionWorkBindings *>(work)->rig.chest.upperChestWorld, work->transformStart);
    RebaseSolvedMatrix(&reinterpret_cast<MotionWorkBindings *>(work)->rig.head.neckWorld, work->transformStart);
    RebaseSolvedMatrix(&reinterpret_cast<MotionWorkBindings *>(work)->rig.head.headWorld, work->transformStart);
    RebaseSolvedMatrix(&reinterpret_cast<MotionWorkBindings *>(work)->rig.rightArm.shoulderWorld, work->transformStart);
    RebaseSolvedMatrix(&reinterpret_cast<MotionWorkBindings *>(work)->rig.rightArm.upperArmWorld, work->transformStart);
    RebaseSolvedMatrix(&reinterpret_cast<MotionWorkBindings *>(work)->rig.rightArm.forearmWorld, work->transformStart);
    RebaseSolvedMatrix(&reinterpret_cast<MotionWorkBindings *>(work)->rig.rightArm.handWorld, work->transformStart);
    RebaseSolvedMatrix(&reinterpret_cast<MotionWorkBindings *>(work)->rig.leftArm.shoulderWorld, work->transformStart);
    RebaseSolvedMatrix(&reinterpret_cast<MotionWorkBindings *>(work)->rig.leftArm.upperArmWorld, work->transformStart);
    RebaseSolvedMatrix(&reinterpret_cast<MotionWorkBindings *>(work)->rig.leftArm.forearmWorld, work->transformStart);
    RebaseSolvedMatrix(&reinterpret_cast<MotionWorkBindings *>(work)->rig.leftArm.handWorld, work->transformStart);
    if (!reinterpret_cast<MotionWorkBindings *>(work)->rig.useOverride) {
        RebaseSolvedMatrix(&reinterpret_cast<MotionWorkBindings *>(work)->rig.chest.chestWorld, work->transformStart);
        RebaseSolvedMatrix(&reinterpret_cast<MotionWorkBindings *>(work)->rig.rightArm.wristWorld, work->transformStart);
        RebaseSolvedMatrix(&reinterpret_cast<MotionWorkBindings *>(work)->rig.leftArm.wristWorld, work->transformStart);
        RebaseSolvedMatrix(&reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.leftFootWorld, work->transformStart);
        RebaseSolvedMatrix(&reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightFootWorld, work->transformStart);
    } else {
        RebaseSolvedMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.leftHipWorld, work->transformStart);
        RebaseSolvedMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightHipWorld, work->transformStart);
        RebaseSolvedMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightToe1World, work->transformStart);
        RebaseSolvedMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightToe2World, work->transformStart);
        RebaseSolvedMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightToe3World, work->transformStart);
        RebaseSolvedMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.head.aimSourceWorld, work->transformStart);
        RebaseSolvedMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.head.nodeAWorld, work->transformStart);
        RebaseSolvedMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.head.nodeBWorld, work->transformStart);
        RebaseSolvedMatrix(reinterpret_cast<MotionWorkBindings *>(work)->rig.head.nodeCWorld, work->transformStart);
    }
    PopCurrentMatrix(1);
}
#endif

// @verified - d
std::uint32_t mtwkGetRigBoneId(const MotionWork *work) {
    if (!work)
        return UINT32_MAX;
    return work->rigBoneId;
}

// @verified - d
std::uint16_t mtwkGetAnimationId(const MotionWork *work, MotionRegion region) {
    const std::uint32_t index = static_cast<std::uint32_t>(region);
    return work && index <= 5 ? work->currentMotionId[index] : 0;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint16_t mtwkGetPreviousAnimationId(const MotionWork *work, MotionRegion region) {
    const std::uint32_t index = static_cast<std::uint32_t>(region);
    return work && index <= 5 ? work->previousMotionId[index] : 0;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint16_t mtwkGetQueuedAnimationId(const MotionWork *work, MotionRegion region) {
    return work && static_cast<std::uint32_t>(region) <= 5U ? work->queuedMotionId[region] : 0;
}
#endif

// @verified - d
bool mtwkIsMotionNonLooping(const MotionWork *work, std::uint32_t region) {
    if (!work || region > 5U)
        return false;
    if (MOTN_GetLoopFrame(work->currentMotionId[region]) == 0xffffU)
        return true;
    return false;
}

// @verified - d
void MotFileManageTaskEntry_Callback(void *) {
    ClearRetiredMotionRanges();
}

// @verified - d
void WriteMotionSnapshotData(MotionSnapshot *snapshot, Character *character, MotionWork *work) {
    MotionRegionClock *clocks[5];
    Vector3i rotation;
    std::int32_t index;
    snapshot->rootPosition.x = work->cachedRootPosition.x;
    snapshot->rootPosition.y = work->cachedRootPosition.y;
    snapshot->rootPosition.z = work->cachedRootPosition.z;
    snapshot->entityRotation.x = work->entityRotationX;
    snapshot->entityRotation.y = work->entityYaw;
    snapshot->entityRotation.z = work->entityRotationZ;
    GetCharacterWorldPosition(character, &snapshot->worldPosition);
    GetCharacterWorldRotation(character, &rotation);
    snapshot->worldRotation.x = rotation.x;
    snapshot->worldRotation.y = rotation.y;
    snapshot->worldRotation.z = rotation.z;
    snapshot->requestedRootMode = work->requestedRootMode;
    snapshot->motionFlags = work->motionFlags;
    snapshot->sequenceFlags = work->sequenceFlags;
    snapshot->retainedMotionParameter = work->retainedMotionParameter;
    snapshot->retainedCollisionValues[0] = work->retainedCollisionValues[0];
    snapshot->retainedCollisionValues[1] = work->retainedCollisionValues[1];
    snapshot->retainedCollisionValues[2] = work->retainedCollisionValues[2];
    snapshot->executionFlags = work->executionFlags;
    reinterpret_cast<MotionFootstepAttribute *>(snapshot->retainedExecutionWords)[0] =
        reinterpret_cast<const MotionFootstepAttribute *>(work->footstepAttributes)[0];
    reinterpret_cast<MotionFootstepAttribute *>(snapshot->retainedExecutionWords)[1] =
        reinterpret_cast<const MotionFootstepAttribute *>(work->footstepAttributes)[1];
    mtwkGetRegionClocks(work, clocks, clocks + 1, clocks + 2, clocks + 3, clocks + 4);
    for (index = 0; index < 5; ++index) {
        snapshot->motionIds[index] = work->currentMotionId[index];
        snapshot->defaultStepFrames[index] = work->defaultRegionStepFrames[index];
        snapshot->regionFlags[index] = work->regionFlags[index];
        snapshot->playbackSpeed[index] = clocks[index]->playbackSpeed;
        if (!(work->regionFlags[index] & 0x10U) || work->currentRootMode == 2) {
            snapshot->splitFrames[index] = clocks[index]->splitTimeFrames;
            snapshot->ticksTime[index] = clocks[index]->ticksTime;
            snapshot->maxTime[index] = clocks[index]->maxTime;
        } else {
            snapshot->splitFrames[index] = work->layerLoopStartTime[index];
            snapshot->ticksTime[index] = work->pendingLayerStartFrame[index];
            snapshot->maxTime[index] = work->layerEndFrame[index];
        }
    }
    for (index = 0; index < 9; ++index)
        snapshot->constraintStepLimit[index] = work->constraintStepLimit[index];
}

// @verified - d
void SetCharacterMotionEnabled(Character *character, std::int32_t enabled) {
    MotionWork *const work = static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION));
    if (enabled)
        work->executionFlags |= 1U;
    else
        work->executionFlags &= ~1U;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool charIsMotionworkEnabled(Character *character) {
    const MotionWork *const work = SHENMUE_MOTION_CONST_CAST(MotionWork, charFindComp(character, CHARACTER_MOTION));
    if (work->executionFlags & 1U)
        return true;
    return false;
}
#endif

// @verified - d
void mtwkSetEnabled(MotionWork *work, std::int32_t enabled) {
    if (work) {
        if (enabled)
            work->executionFlags |= 1U;
        else
            work->executionFlags &= ~1U;
    }
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void mtwkSetActive(MotionWork *work) {
    g_activeMotionWork = work;
}
#endif

// @verified - d
std::uint32_t mtwkIsEnabled(const MotionWork *work) {
    if (!work || (work->executionFlags & 1U) == 0)
        return 0U;
    return 1U;
}

// @verified - d
void SetCharacterUnknownFlag(Character *character, std::uint32_t enabled) {
    MotionWork *const work = static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION));
    if (enabled)
        work->executionFlags |= 8U;
    else
        work->executionFlags &= ~8U;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void mtwkSampleQueuedMotionRoot(MotionWork *work, float frame, Vector3 *output) {
    const std::uint32_t bone = mtwkGetRigBoneId(work);
    const std::uint16_t queued = work->queuedMotionId[0];
    std::uint16_t animation;
    std::uint16_t mirrored;
    if (queued) {
        animation = queued;
        const std::uint32_t lead = static_cast<std::int16_t>(work->regionFlags[0]) & 0x20U;
        mirrored = lead != MOTN_GetLeadFootFlag(queued);
    } else {
        animation = work->currentMotionId[0];
        mirrored = work->regionFlags[0] & MRF_Mirrored;
    }
    SampleNativeMotionRoot(animation, bone, frame, output);
    if (mirrored)
        output->x = -output->x;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void mtwkSampleCurrentMotion(MotionWork *work, float frame, Vector3 *output) {
    const std::int32_t bone = static_cast<std::int32_t>(mtwkGetRigBoneId(work));
    const std::uint16_t queued = work->queuedMotionId[0];
    std::uint16_t animation;
    std::uint32_t mirrored;
    float previousFrame;
    if (queued) {
        animation = queued;
        if (frame != 0)
            mtwkSetRegionEvalTime(work, frame, LowerBody);
        const std::uint32_t leadFoot = work->regionFlags[0] & 0x20U;
        mirrored = leadFoot != MOTN_GetLeadFootFlag(animation);
        previousFrame = -1;
    } else {
        animation = work->currentMotionId[0];
        previousFrame = work->layerPreviousTicks[0];
        if (previousFrame > frame)
            previousFrame = -1;
        mtwkSetRegionEvalTime(work, frame, LowerBody);
        mirrored = work->regionFlags[0] & 0x40U;
    }
    Vector3 previous;
    if (previousFrame >= 0) {
        SampleNativeMotionRoot(animation, bone, previousFrame, &previous);
        if (mirrored)
            previous.x = -previous.x;
    } else {
        previous.x = 0;
        previous.z = 0;
    }
    previous.y = 0;
    SampleNativeMotionRoot(animation, bone, frame, output);
    if (mirrored)
        output->x = -output->x;
    output->x -= previous.x;
    output->z -= previous.z;
}
#endif
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool charIsMotionworkCulled(Character *character) {
    const MotionWork *const work = SHENMUE_MOTION_CONST_CAST(MotionWork, charFindComp(character, CHARACTER_MOTION));
    if (work->executionFlags & 8U)
        return true;
    return false;
}
#endif

// @verified - d
std::uint32_t GetCharacterMotionCompatibilityResult(const Character *character) {
    std::uint32_t result;
    if (character)
        result = mtwkGetClipMode(SHENMUE_MOTION_CONST_CAST(MotionWork, charFindComp(character, CHARACTER_MOTION)));
    else
        result = 0;
    return result;
}

// @verified - d
void SetCharacterMotionClipMode(Character *character, std::int32_t mode) {
    mtwkSetClipMode(static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION)), mode);
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void charSetTransformSource(
    Character *character, std::uint32_t mode, Character *source, std::int32_t index, const Vector3 *position, const Vector3i *rotation) {
    MotionWork *const work = static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION));
    work->transformSource = source;
    work->transformSourceIndex = index;
    if (source) {
        CopyCharacterSourceTransform(source, static_cast<std::uint32_t>(index), &work->overrideViewSpaceTransform);
        if (position)
            work->transformOffsetPosition = *position;
        else
            work->transformOffsetPosition.x = work->transformOffsetPosition.y = work->transformOffsetPosition.z = 0.0f;
        if (rotation) {
            work->transformOffsetRotation.x = rotation->x;
            work->transformOffsetRotation.y = rotation->y;
            work->transformOffsetRotation.z = rotation->z;
        } else
            work->transformOffsetRotation.x = work->transformOffsetRotation.y = work->transformOffsetRotation.z = 0;
        work->transformStart = &work->overrideViewSpaceTransform;
        *reinterpret_cast<std::uint32_t *>(work->retainedCollisionState) = mode;
    } else {
        work->transformStart = 0;
        *reinterpret_cast<std::uint32_t *>(work->retainedCollisionState) = 0;
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t charMotionNullsubA() {
    return 0;
}

std::int32_t charMotionNullsubB() {
    return 0;
}

float charMotionNullsubFloat() {
    return 0.0f;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool mtwkSetIkTargetPosition(MotionWork *work, Vector3 *position) {
    if (!work)
        return false;
    work->ikTargetPosition = position;
    return true;
}
#endif

namespace {

std::int32_t MotionRootSignedWord(std::uint32_t bits) {
    std::int32_t value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

} // namespace

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void mtwkSetPelvisYawStep(MotionWork *work, std::int16_t step) {
    work->pelvisYawStep = step;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool mtwkHasRegionTracks(const MotionWork *work) {
    bool found = false;
    if (!work)
        return false;
    for (std::int32_t region = 0; region < 5; ++region)
        if (!(work->regionFlags[region] & 0x100U) && work->regionTrack[region])
            found = true;
    return found;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float mtwkGetHandTrackValue(const MotionWork *work) {
    return *reinterpret_cast<const float *>(work->retainedHandTrackState);
}

void mtwkSetHandTrackValue(MotionWork *work, float value) {
    *reinterpret_cast<float *>(work->retainedHandTrackState) = value;
}
#endif

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void BindMotionNode(MT7Node *model, std::int16_t id, const Matrix4x4 *matrix) {
    MT7Node *const node = FindMT7Node(model, id);
    if (node)
        node->transformOverride = matrix;
}
#endif

} // namespace

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void BindMotionControllerMatricesToModelNodes(MotionWork *work, MT7Node *model) {
    IKRig &rig = reinterpret_cast<MotionWorkBindings *>(work)->rig;
    if (work->sequenceFlags & MSF_ModelMatricesBound)
        return;
    ClearShadowNodeTransforms(model);
    BindMotionNode(model, 1, &rig.chest.upperChestWorld);
    BindMotionNode(model, 2, &rig.head.neckWorld);
    BindMotionNode(model, -67, &rig.head.headWorld);
    BindMotionNode(model, 4, &rig.rightArm.shoulderWorld);
    BindMotionNode(model, 5, &rig.rightArm.upperArmWorld);
    BindMotionNode(model, 6, &rig.rightArm.forearmWorld);
    BindMotionNode(model, -65, &rig.rightArm.handWorld);
    BindMotionNode(model, 9, &rig.leftArm.shoulderWorld);
    BindMotionNode(model, 10, &rig.leftArm.upperArmWorld);
    BindMotionNode(model, 11, &rig.leftArm.forearmWorld);
    BindMotionNode(model, -66, &rig.leftArm.handWorld);
    BindMotionNode(model, 14, &rig.lowerBody.hipsWorld);
    BindMotionNode(model, 16, &rig.lowerBody.leftThighWorld);
    BindMotionNode(model, 17, &rig.lowerBody.leftShinWorld);
    BindMotionNode(model, 18, &rig.lowerBody.leftAnkleWorld);
    BindMotionNode(model, 21, &rig.lowerBody.rightThighWorld);
    BindMotionNode(model, 22, &rig.lowerBody.rightShinWorld);
    BindMotionNode(model, 23, &rig.lowerBody.rightAnkleWorld);
    if (!rig.useOverride) {
        BindMotionNode(model, 0, &rig.chest.chestWorld);
        BindMotionNode(model, 7, &rig.rightArm.wristWorld);
        BindMotionNode(model, 12, &rig.leftArm.wristWorld);
    } else {
        BindMotionNode(model, 0, &rig.lowerBody.pelvisWorld);
        BindMotionNode(model, 15, rig.lowerBody.leftHipWorld);
        BindMotionNode(model, 20, rig.lowerBody.rightHipWorld);
        BindMotionNode(model, 55, rig.lowerBody.rightToe1World);
        BindMotionNode(model, 56, rig.lowerBody.rightToe2World);
        BindMotionNode(model, 57, rig.lowerBody.rightToe3World);
        BindMotionNode(model, -79, rig.head.aimSourceWorld);
        BindMotionNode(model, 58, rig.head.nodeAWorld);
        BindMotionNode(model, 59, rig.head.nodeBWorld);
        BindMotionNode(model, 119, rig.head.nodeCWorld);
    }
    work->sequenceFlags |= MSF_ModelMatricesBound;
}
#endif

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t ClassifyCurrentLocomotionPhase(const MotionWork *work) {
    const std::uint16_t motion = work->currentMotionId[0];
    if (motion == work->defaultAnimations[1])
        return 1;
    if (motion == work->defaultAnimations[4])
        return 2;
    for (std::uint8_t index = 0; index < 17; ++index) {
        if (motion == defaultMotionIds[index][1])
            return 1;
        if (motion == defaultMotionIds[index][4])
            return 2;
    }
    return 0;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void CommitCopyMotionTransition(MotionWork *work) {
    if (work->copyFlags & 1U)
        work->motionFlags |= 0x8000U;
    else
        work->motionFlags &= ~0x8000U;
    if (work->copyFlags & 2U)
        work->motionFlags |= 0x10000U;
    else
        work->motionFlags &= ~0x10000U;
    work->requestedRootMode = work->copyRegionSelector;
    work->defaultRegionStepFrames[0] = work->transitionFrameTime;
    work->copyStepState = 0;
    work->copyScaleCountdown = 0;
    work->copyStepsRemaining = 0;
    work->insideRectangleScale = 1.0f;
    if (work->copyFlags & 0x100000U) {
        mtwkUpdateWithPosition(work, 0, 0);
        mtwkUpdateWithPosition(work, 0, 0x304);
    }
    work->copyFlags &= 0x20U;
    work->copyLayerStartFrame = 0.0f;
    work->copyOperationState = 0;
    work->copyPhase = 0;
    work->copyDelayFrames = 0;
    work->sequenceFlags &= ~0x4000000U;
}
#endif

} // namespace

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool mtwkCopyTransition(MotionWork *work, const Vector3 *position, std::int32_t yaw, std::int32_t operation, std::int32_t copyType) {
    if (!work)
        return false;
    if (!work->defaultAnimations[0]) {
        const std::uint32_t index = GetDefaultMotionTableIndex(work->rigBoneId);
        const std::uint16_t (*defaults)[6] = defaultMotionIds;
        std::memcpy(work->defaultAnimations, defaults[index], sizeof(work->defaultAnimations));
    }
    switch (operation) {
    case 1:
        if (!(work->sequenceFlags & 0x4000000U))
            return false;
        CommitCopyMotionTransition(work);
        return true;
    case 3:
        if (!(work->copyFlags & 0x20U))
            return false;
        return true;
    case 15:
        if (work->copyFlags & 0x200U)
            return true;
        return false;
    case 8:
        if (!(work->sequenceFlags & 0x4000000U))
            return false;
        work->copyFlags |= 0x8000U;
        return true;
    case 14:
        if (!(work->sequenceFlags & 0x4000000U))
            return false;
        work->copyFlags &= ~0x8000U;
        return true;
    case 12:
        if (!(work->sequenceFlags & 0x4000000U))
            return false;
        work->copyFlags |= 0x2000U;
        return true;
    case 2:
        if (!(work->sequenceFlags & 0x4000000U))
            return false;
        work->copyFlags |= 0x10U;
        return true;
    case 16:
        if (!(work->sequenceFlags & 0x4000000U))
            return false;
        work->copyFlags |= 0x40000U;
        return true;
    case 13:
        work->transitionVariant = 2;
        if (!(work->sequenceFlags & 0x4000000U)) {
            work->transitionClass = 2;
            if (work->motionFlags & 0x8000U)
                work->copyFlags |= 1U;
            if (work->motionFlags & 0x10000U)
                work->copyFlags |= 2U;
            work->copyRegionSelector = work->requestedRootMode;
            work->transitionFrameTime = work->defaultRegionStepFrames[0];
            work->copyFlags &= ~0x10000U;
            work->copyFlags &= ~0x20000U;
            work->copyFlags |= 0x100U;
            work->transitionEndFrame = yaw;
            work->transitionStartFrame = yaw;
            work->copyFlags |= 0x4000U;
            if (ClassifyCurrentLocomotionPhase(work)) {
                work->copyOperationState = 5;
                work->copyPhase = 1;
            } else {
                work->copyOperationState = 1;
            }
            work->transitionSubstate = 2;
            work->motionScale = mtwkGetSpeed(work, LowerBody);
            if (work->motionScale < 0.5f) {
                work->motionScale = 0.5f;
                mtwkSetSpeed(work, 0.5f, LowerBody);
            }
            work->sequenceFlags |= 0x4000000U;
            return true;
        }
        work->copyFlags &= ~0x10000U;
        work->copyFlags &= ~0x20000U;
        work->copyFlags |= 0x4000U;
        work->copyFlags |= 0x100U;
        work->transitionSubstate = 2;
        work->transitionEndFrame = yaw;
        return true;
    case 9:
    case 10:
    case 11:
        if (!(work->sequenceFlags & 0x4000000U))
            return false;
        work->copyFlags |= 0x400U;
        return true;
    case 0:
    case 4:
        work->copyFlags |= 0x80U;
        work->transitionVariant = 0;
        if (!(work->sequenceFlags & 0x4000000U)) {
            work->transitionClass = 0;
            switch (copyType) {
            case 0:
            case 1:
            case 3:
                if (!position)
                    return false;
                work->copySourcePosition = *position;
            case 2:
                break;
            default:
                return false;
            }
            if (work->motionFlags & 0x8000U)
                work->copyFlags |= 1U;
            if (work->motionFlags & 0x10000U)
                work->copyFlags |= 2U;
            work->copyRegionSelector = work->requestedRootMode;
            work->transitionFrameTime = work->defaultRegionStepFrames[0];
            work->copyFlags &= ~0x10000U;
            work->copyFlags &= ~0x20000U;
            work->copyFlags &= ~0x800U;
            work->transitionStartFrame = yaw;
            if (ClassifyCurrentLocomotionPhase(work)) {
                work->copyFlags |= 0x100U;
                work->transitionEndFrame = work->transitionStartFrame;
                work->copyDestinationPosition = work->copySourcePosition;
                work->copyOperationState = 5;
                work->copyPhase = 1;
            } else {
                work->copyOperationState = 1;
            }
            if (copyType == 3)
                copyType = 0;
            work->transitionSubstate = copyType;
            work->motionScale = mtwkGetSpeed(work, LowerBody);
            if (work->motionScale < 0.5f) {
                work->motionScale = 0.5f;
                mtwkSetSpeed(work, 0.5f, LowerBody);
            }
            work->sequenceFlags |= 0x4000000U;
            if (work->queuedMotionId[0])
                work->queuedMotionId[0] = 0;
            return true;
        }
        switch (copyType) {
        case 0:
        case 1:
        case 3:
            if (!position)
                return false;
            work->copyDestinationPosition = *position;
            break;
        case 2:
            work->copyDestinationPosition = work->cachedRootPosition;
            break;
        default:
            return false;
        }
        work->copyFlags |= 0x100U;
        work->copyFlags &= ~0x4000U;
        work->copyFlags &= ~0x10000U;
        work->copyFlags &= ~0x20000U;
        work->transitionEndFrame = yaw;
        if (copyType == 3)
            copyType = 0;
        work->transitionSubstate = copyType;
        return true;
    case 5:
        work->transitionVariant = 1;
        if (!(work->sequenceFlags & 0x4000000U)) {
            work->transitionClass = 1;
            switch (copyType) {
            case 0:
            case 3:
                if (!position)
                    return false;
                work->copySourcePosition = *position;
                work->copyDestinationPosition = *position;
                work->copyFlags |= 0x80U;
                break;
            case 1:
                if (!position)
                    return false;
                work->copySourcePosition = *position;
                work->copyDestinationPosition = *position;
            case 2:
                work->copyFlags &= ~0x80U;
                break;
            default:
                return false;
            }
            if (work->motionFlags & 0x8000U)
                work->copyFlags |= 1U;
            if (work->motionFlags & 0x10000U)
                work->copyFlags |= 2U;
            work->copyRegionSelector = work->requestedRootMode;
            work->transitionFrameTime = work->defaultRegionStepFrames[0];
            work->copyFlags &= ~0x10000U;
            work->copyFlags &= ~0x20000U;
            work->transitionStartFrame = yaw;
            work->transitionEndFrame = yaw;
            if (copyType == 3) {
                copyType = 0;
                work->copyFlags |= 0x10000U;
            }
            work->transitionSubstate = copyType;
            if (ClassifyCurrentLocomotionPhase(work)) {
                work->copyFlags |= 0x100U;
                work->copyOperationState = 5;
                work->copyPhase = 1;
            } else {
                work->copyOperationState = 1;
            }
            work->motionScale = mtwkGetSpeed(work, LowerBody);
            if (work->motionScale < 0.5f) {
                work->motionScale = 0.5f;
                mtwkSetSpeed(work, 0.5f, LowerBody);
            }
            work->sequenceFlags |= 0x4000000U;
            if (work->queuedMotionId[0])
                work->queuedMotionId[0] = 0;
            return true;
        }
        switch (copyType) {
        case 0:
        case 1:
        case 3:
            if (!position)
                return false;
            work->copyDestinationPosition = *position;
        case 2:
            break;
        default:
            return false;
        }
        work->copyFlags |= 0x100U;
        work->copyFlags &= ~0x4000U;
        work->copyFlags &= ~0x10000U;
        work->copyFlags &= ~0x20000U;
        work->transitionEndFrame = yaw;
        if (copyType == 3) {
            copyType = 0;
            work->copyFlags |= 0x10000U;
        }
        work->transitionSubstate = copyType;
        return true;
    case 6:
        work->transitionVariant = 2;
        if (!(work->sequenceFlags & 0x4000000U)) {
            work->transitionClass = 2;
            switch (copyType) {
            case 0:
            case 1:
            case 3:
                if (!position)
                    return false;
                work->copySourcePosition = *position;
                work->copyDestinationPosition = *position;
            case 2:
                break;
            default:
                return false;
            }
            if (work->motionFlags & 0x8000U)
                work->copyFlags |= 1U;
            if (work->motionFlags & 0x10000U)
                work->copyFlags |= 2U;
            work->copyRegionSelector = work->requestedRootMode;
            work->transitionFrameTime = work->defaultRegionStepFrames[0];
            work->copyFlags &= ~0x10000U;
            work->copyFlags &= ~0x20000U;
            work->transitionStartFrame = yaw;
            work->transitionEndFrame = yaw;
            if (copyType == 3) {
                work->copyFlags |= 0x10000U;
                copyType = 0;
            }
            work->transitionSubstate = copyType;
            if (ClassifyCurrentLocomotionPhase(work)) {
                work->copyFlags |= 0x100U;
                work->copyOperationState = 5;
                work->copyPhase = 1;
            } else {
                work->copyFlags |= 0x100U;
                work->copyOperationState = 1;
            }
            work->motionScale = mtwkGetSpeed(work, LowerBody);
            if (work->motionScale < 0.5f) {
                work->motionScale = 0.5f;
                mtwkSetSpeed(work, 0.5f, LowerBody);
            }
            work->sequenceFlags |= 0x4000000U;
            if (work->queuedMotionId[0])
                work->queuedMotionId[0] = 0;
            return true;
        }
        switch (copyType) {
        case 0:
        case 1:
        case 3:
            if (!position)
                return false;
            work->copyDestinationPosition = *position;
            break;
        case 2:
            work->copyDestinationPosition = work->cachedRootPosition;
            break;
        default:
            return false;
        }
        work->copyFlags |= 0x100U;
        work->copyFlags &= ~0x4000U;
        work->copyFlags &= ~0x10000U;
        work->copyFlags &= ~0x20000U;
        work->transitionEndFrame = yaw;
        if (copyType == 3) {
            work->copyFlags |= 0x10000U;
            copyType = 0;
        }
        work->transitionSubstate = copyType;
        return true;
    case 7:
        work->transitionVariant = 3;
        if (!(work->sequenceFlags & 0x4000000U)) {
            work->transitionClass = 3;
            switch (copyType) {
            case 0:
            case 1:
            case 3:
                if (!position)
                    return false;
                work->copySourcePosition = *position;
                work->copyDestinationPosition = *position;
            case 2:
                break;
            default:
                return false;
            }
            if (work->motionFlags & 0x8000U)
                work->copyFlags |= 1U;
            if (work->motionFlags & 0x10000U)
                work->copyFlags |= 2U;
            work->copyRegionSelector = work->requestedRootMode;
            work->transitionFrameTime = work->defaultRegionStepFrames[0];
            work->copyFlags &= ~0x10000U;
            work->copyFlags &= ~0x20000U;
            work->copyFlags &= ~0x800U;
            work->transitionStartFrame = yaw;
            work->transitionEndFrame = yaw;
            if (copyType == 3)
                copyType = 0;
            if (copyType == 1)
                work->copyFlags |= 0x10000U;
            work->transitionSubstate = copyType;
            if (ClassifyCurrentLocomotionPhase(work)) {
                work->copyFlags |= 0x100U;
                work->copyOperationState = 5;
                work->copyPhase = 1;
            } else {
                work->copyFlags |= 0x100U;
                work->copyOperationState = 1;
            }
            work->motionScale = mtwkGetSpeed(work, LowerBody);
            if (work->motionScale < 0.5f) {
                work->motionScale = 0.5f;
                mtwkSetSpeed(work, 0.5f, LowerBody);
            }
            work->sequenceFlags |= 0x4000000U;
            if (work->queuedMotionId[0])
                work->queuedMotionId[0] = 0;
            return true;
        }
        switch (copyType) {
        case 0:
        case 1:
        case 3:
            if (!position)
                return false;
            work->copyDestinationPosition = *position;
        case 2:
            break;
        default:
            return false;
        }
        work->copyFlags |= 0x100U;
        work->copyFlags &= ~0x4000U;
        work->copyFlags &= ~0x10000U;
        work->copyFlags &= ~0x20000U;
        work->transitionEndFrame = yaw;
        if (copyType == 3)
            copyType = 0;
        if (copyType == 1)
            work->copyFlags |= 0x10000U;
        work->transitionSubstate = copyType;
        return true;
    }
    return false;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool mtwkRequestPositionTransition(MotionWork *work, const Vector3 *position, std::int32_t yaw, std::uint32_t motion, float startFrame) {
    if (!work)
        return false;
    const std::uint32_t mirrored = (motion >> 16U) & 0xffU;
    const std::uint32_t extra = motion >> 24U;
    if (position) {
        Vector3 target;
        if (!(startFrame < 0.0f)) {
            SampleNativeMotionRoot(static_cast<std::uint16_t>(motion), work->rigBoneId, startFrame, &target);
            target.y = 0.0f;
            if (mirrored)
                target.x = -target.x;
            matrix_stack::PushUnitMatrix();
            Matrix4x4::preTranslate(position->x, position->y, position->z);
            Matrix4x4::RotateAroundY_Wrapper(yaw);
            Matrix4x4::TransformPoint(target, target);
            PopCurrentMatrix(1);
        } else {
            target = *position;
        }
        mtwkCopyTransition(work, &target, yaw, 6, 0);
        if (extra)
            mtwkCopyTransition(work, 0, 0, 11, 0);
    } else {
        mtwkCopyTransition(work, 0, yaw, 13, 0);
    }
    mtwkSetTimeRelated(work, 1, 5, static_cast<std::uint16_t>(motion));
    if (!(startFrame < 0.0f))
        mtwkSetRegionEvalTime(work, startFrame, LowerBody);
    if (mirrored)
        work->regionFlags[0] |= 0x20U;
    return true;
}
#endif

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint8_t GetTargetFacingMotionClass(const MotionWork *work) {
    switch (work->rigBoneId) {
    case 0:
    case 1:
    case 2:
    case 13:
    case 14:
        return 2;
    case 3:
    case 4:
    case 5:
        return 4;
    case 15:
    default:
        return 1;
    case 6:
        return 3;
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void PlayTargetFacingTurn(MotionWork *work, bool moving, std::int32_t left, std::int16_t yaw) {
    const std::int32_t absolute = static_cast<std::int32_t>(std::fabs(static_cast<float>(yaw)));
    std::int16_t remainder = static_cast<std::int16_t>(absolute);
    std::int32_t index;
    if (work->targetFacingFlags & 0x40U) {
        if (absolute > 0x6000) {
            index = 5;
            remainder = 0;
        } else if (absolute > 0x31c7) {
            index = 9;
            remainder = static_cast<std::int16_t>(remainder - 0x4000);
        } else {
            index = 7;
        }
        if (index != 7 && ((yaw < 0 && !left) || (yaw > 0 && left)))
            --index;
    } else {
        if (absolute > 0x6000) {
            index = 5;
            remainder = 0;
        } else if (absolute > 0x4000) {
            index = 3;
            remainder = static_cast<std::int16_t>(remainder - 0x4000);
        } else if (absolute > 0x2000) {
            index = 1;
            remainder = static_cast<std::int16_t>(remainder - 0x2000);
        } else {
            index = 6;
        }
        if (index <= 5 && ((yaw < 0 && !left) || (yaw > 0 && left)))
            --index;
    }
    if (yaw < 0)
        remainder = static_cast<std::int16_t>(-remainder);
    work->pelvisYawTarget = remainder;
    std::uint8_t motionClass = work->targetFacingMotionClass;
    if (!motionClass)
        motionClass = GetTargetFacingMotionClass(work);
    const std::uint8_t row = static_cast<std::uint8_t>(motionClass - 1);
    const std::uint16_t *motions;
    std::uint16_t motion;
    std::int32_t frames;
    if (moving) {
        motions = targetFacingMotions[row];
        motion = motions[index];
        frames = static_cast<std::int32_t>(GetMotionFrameCount(motion)) / 3;
    } else {
        motions = &targetFacingMotions[row][10];
        motion = motions[index];
        frames = static_cast<std::int32_t>(GetMotionFrameCount(motion)) / 4;
    }
    work->pelvisYawStep = 384;
    frames = static_cast<std::int32_t>(static_cast<float>(frames) + std::fabs(static_cast<float>(work->pelvisYawTarget - work->pelvisYawCurrent)) / 384.0f);
    mtwkRequestMotion(work, motion, Chest);
    mtwkRequestMotion(work, motion, Head);
    mtwkSetFrameTime(work, static_cast<std::uint32_t>(frames), Chest);
    mtwkSetFrameTime(work, static_cast<std::uint32_t>(frames), Head);
    if (left) {
        work->regionFlags[Chest] |= 0x20U;
        work->regionFlags[Head] |= 0x20U;
        mtwkRequestMotion(work, motion, LeftArm);
        mtwkSetFrameTime(work, static_cast<std::uint32_t>(frames), LeftArm);
        work->regionFlags[LeftArm] |= 0x20U;
    } else {
        mtwkRequestMotion(work, motion, RightArm);
        mtwkSetFrameTime(work, static_cast<std::uint32_t>(frames), RightArm);
    }
    mtwkSetRequestedRootMode(work, 3);
    work->chestRotationOverrideBlendTime = 1.0f / static_cast<float>(frames);
}
#endif

} // namespace

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool mtwkRequestTargetFacing(MotionWork *work, const Vector3 *target, std::uint32_t flags) {
    if (!work || work->useMatrixOverrides)
        return false;
    if (target) {
        if ((work->sequenceFlags & 0x10000000U) && work->targetFacingState > 3) {
            if (work->targetFacingState == 4)
                PlayTargetFacingTurn(work, false, work->targetFacingFlags & 4U, work->targetFacingYawDelta);
            work->targetFacingFlags |= 8U;
            if (work->targetFacingFlags & 4U)
                work->targetFacingFlags |= 0x10U;
            else
                work->targetFacingFlags &= ~0x10U;
        }
        if (flags & 1U)
            work->targetFacingFlags |= 1U;
        else
            work->targetFacingFlags &= ~1U;
        if (flags & 6U) {
            work->targetFacingFlags |= 2U;
            if (flags & 2U)
                work->targetFacingFlags |= 4U;
            else
                work->targetFacingFlags &= ~4U;
        } else {
            work->targetFacingFlags &= ~2U;
        }
        work->targetFacingMotionClass = static_cast<std::uint8_t>(static_cast<std::int32_t>(flags) >> 16);
        work->targetFacingPosition = *target;
        work->targetFacingState = 1;
        work->sequenceFlags |= 0x10000000U;
        return true;
    }
    if ((work->sequenceFlags & 0x10000000U) && work->targetFacingState != 7 && work->targetFacingState != 6)
        work->targetFacingState = 5;
    return true;
}
#endif

// @verified - d
void SampleMotionStreamChannels(
    MotionRigAxis *axes, const MotnAnimationHeader *animation, float *output, std::uint32_t first, std::uint32_t count, float frame) {
    const std::uint16_t flags = animation->flags;
    const std::uint16_t frames = animation->frameCount;
    const std::int16_t keyDataOffset = animation->keyDataOffset;
    if ((flags >> 8) & 1U)
        g_motionWideKeyCounts = reinterpret_cast<const std::uint16_t *>(reinterpret_cast<const std::byte *>(animation) + sizeof(MotnAnimationHeader));
    else
        g_motionKeyCounts = reinterpret_cast<const std::uint8_t *>(reinterpret_cast<const std::byte *>(animation) + sizeof(MotnAnimationHeader));
    g_motionKeyData = reinterpret_cast<const std::uint16_t *>(reinterpret_cast<const std::byte *>(animation) + keyDataOffset);
    if (first)
        SetMotionStreamFlags(static_cast<std::int32_t>(first - 1U), motionInitStream, flags);
    g_motionInitFlags = motionInitStream;
    g_motionAnimFlags = flags;
    g_motionPreviousAnimTime = 0.0f;
    g_motionMaximumAnimTime = static_cast<float>(static_cast<std::int32_t>(frames) - 1) * kMotionStreamFrameSeconds;
    for (std::uint32_t index = 0; index < count; ++index)
        ReadMotionKeyBlock(axes[index].channel);
    g_motionInterpolationType = 0;
    if (0.0f > frame)
        g_motionCurrentAnimTime = g_motionMaximumAnimTime;
    else
        g_motionCurrentAnimTime = frame * kMotionStreamFrameSeconds;
    for (std::uint32_t index = 0; index < count; ++index)
        output[index] = SampleMotionRigChannelAdvance(axes[index].channel);
}

// @verified - d
float GetMotionRigScale(std::uint16_t target, std::uint16_t source, std::int32_t dimension) {
    float scale = 1.0f;
    const float (*const dims)[6] = g_motionConstData.dimensions;
    if (target != source)
        scale = dims[target][dimension] / dims[source][dimension];
    return scale;
}

// @verified - d
bool SampleNativeMotionRootAndRotation(std::uint16_t animation, std::int32_t boneId, float frame, Vector3 *position, Vector3i *rotation) {
    const MotnAnimationHeader *const data = MOTN_GetDataForAnimId(animation);
    if (!data) {
        if (position)
            position->x = position->y = position->z = 0.0f;
        if (rotation)
            rotation->x = rotation->y = rotation->z = 0;
        return false;
    }
    const std::uint16_t sourceRig = data->flags & 0xffU;
    if (boneId > 16)
        boneId = 8;
    float scale;
    if (boneId < 0)
        scale = 1.0f;
    else
        scale = GetMotionRigScale(static_cast<std::uint16_t>(boneId), sourceRig, 0);
    MotionRigAxis axes[6];
    UpdateAnimRate(axes[0].channel, scale, 0.0f);
    UpdateAnimRate(axes[1].channel, scale, 0.0f);
    UpdateAnimRate(axes[2].channel, scale, 0.0f);
    UpdateAnimRate(axes[3].channel, 1.0f, 0.0f);
    UpdateAnimRate(axes[4].channel, 1.0f, 0.0f);
    UpdateAnimRate(axes[5].channel, 1.0f, 0.0f);
    float sampled[6];
    SampleMotionStreamChannels(axes, data, sampled, 0, 6, frame);
    if (position) {
        position->x = sampled[0];
        position->y = sampled[1];
        position->z = sampled[2];
    }
    if (rotation) {
        rotation->x = static_cast<std::int32_t>((sampled[5] * 57.29578f) * 182.04443f);
        rotation->y = static_cast<std::int32_t>((sampled[4] * 57.29578f) * 182.04443f);
        rotation->z = static_cast<std::int32_t>((sampled[3] * 57.29578f) * 182.04443f);
    }
    return true;
}

// @verified - d
const Matrix4x4 *mtwkGetBoneTransform(const MotionWork *work, std::int32_t boneIndex) {
    const Matrix4x4 *transform;
    if (!work)
        return SHENMUE_MOTION_NULL;
    switch (boneIndex) {
    case 0:
        transform = reinterpret_cast<const Matrix4x4 *>(reinterpret_cast<const std::byte *>(work) + 0x230);
        break;
    case 3:
        transform = reinterpret_cast<const Matrix4x4 *>(reinterpret_cast<const std::byte *>(work) + 0x1168);
        break;
    case 4:
        transform = reinterpret_cast<const Matrix4x4 *>(reinterpret_cast<const std::byte *>(work) + 0x12d0);
        break;
    case 5:
        transform = reinterpret_cast<const Matrix4x4 *>(reinterpret_cast<const std::byte *>(work) + 0x1508);
        break;
    case 8:
        transform = reinterpret_cast<const Matrix4x4 *>(reinterpret_cast<const std::byte *>(work) + 0x1690);
        break;
    case 9:
        transform = reinterpret_cast<const Matrix4x4 *>(reinterpret_cast<const std::byte *>(work) + 0x18c8);
        break;
    case 12:
        transform = reinterpret_cast<const Matrix4x4 *>(reinterpret_cast<const std::byte *>(work) + 0x1908);
        break;
    case 14:
        transform = reinterpret_cast<const Matrix4x4 *>(reinterpret_cast<const std::byte *>(work) + 0x1a80);
        break;
    case 15:
        transform = reinterpret_cast<const Matrix4x4 *>(reinterpret_cast<const std::byte *>(work) + 0x1c10);
        break;
    case 16:
        transform = reinterpret_cast<const Matrix4x4 *>(reinterpret_cast<const std::byte *>(work) + 0x1e48);
        break;
    case 19:
        transform = reinterpret_cast<const Matrix4x4 *>(reinterpret_cast<const std::byte *>(work) + 0x1e88);
        break;
    case 21:
        transform = reinterpret_cast<const Matrix4x4 *>(reinterpret_cast<const std::byte *>(work) + 0x2000);
        break;
    case 22:
        transform = reinterpret_cast<const Matrix4x4 *>(reinterpret_cast<const std::byte *>(work) + 0x3e0);
        break;
    case 23:
        transform = reinterpret_cast<const Matrix4x4 *>(reinterpret_cast<const std::byte *>(work) + 0x620);
        break;
    case 24:
        transform = reinterpret_cast<const Matrix4x4 *>(reinterpret_cast<const std::byte *>(work) + 0x660);
        break;
    case 26:
        transform = reinterpret_cast<const Matrix4x4 *>(reinterpret_cast<const std::byte *>(work) + 0x760);
        break;
    case 28:
        transform = reinterpret_cast<const Matrix4x4 *>(reinterpret_cast<const std::byte *>(work) + 0xb10);
        break;
    case 29:
        transform = reinterpret_cast<const Matrix4x4 *>(reinterpret_cast<const std::byte *>(work) + 0xb50);
        break;
    case 31:
        transform = reinterpret_cast<const Matrix4x4 *>(reinterpret_cast<const std::byte *>(work) + 0xc50);
        break;
    default:
        if (!work->useMatrixOverrides) {
            switch (boneIndex) {
            case 1:
                transform = reinterpret_cast<const Matrix4x4 *>(reinterpret_cast<const std::byte *>(work) + 0x1030);
                break;
            case 13:
                transform = reinterpret_cast<const Matrix4x4 *>(reinterpret_cast<const std::byte *>(work) + 0x1980);
                break;
            case 20:
                transform = reinterpret_cast<const Matrix4x4 *>(reinterpret_cast<const std::byte *>(work) + 0x1f00);
                break;
            case 27:
                transform = reinterpret_cast<const Matrix4x4 *>(reinterpret_cast<const std::byte *>(work) + 0x898);
                break;
            case 32:
                transform = reinterpret_cast<const Matrix4x4 *>(reinterpret_cast<const std::byte *>(work) + 0xd88);
                break;
            default:
                transform = SHENMUE_MOTION_NULL;
                break;
            }
        } else {
            switch (boneIndex) {
            case 39:
                transform = *reinterpret_cast<const Matrix4x4 *const *>(reinterpret_cast<const std::byte *>(work) + 0x424);
                break;
            case 40:
                transform = *reinterpret_cast<const Matrix4x4 *const *>(reinterpret_cast<const std::byte *>(work) + 0x914);
                break;
            case 41:
                transform = *reinterpret_cast<const Matrix4x4 *const *>(reinterpret_cast<const std::byte *>(work) + 0xe04);
                break;
            case 42:
                transform = *reinterpret_cast<const Matrix4x4 *const *>(reinterpret_cast<const std::byte *>(work) + 0xe0c);
                break;
            case 43:
                transform = *reinterpret_cast<const Matrix4x4 *const *>(reinterpret_cast<const std::byte *>(work) + 0xe14);
                break;
            case 33:
                transform = *reinterpret_cast<const Matrix4x4 *const *>(reinterpret_cast<const std::byte *>(work) + 0x155c);
                break;
            case 35:
                transform = *reinterpret_cast<const Matrix4x4 *const *>(reinterpret_cast<const std::byte *>(work) + 0x1564);
                break;
            case 36:
                transform = *reinterpret_cast<const Matrix4x4 *const *>(reinterpret_cast<const std::byte *>(work) + 0x156c);
                break;
            case 37:
                transform = *reinterpret_cast<const Matrix4x4 *const *>(reinterpret_cast<const std::byte *>(work) + 0x1574);
                break;
            default:
                transform = SHENMUE_MOTION_NULL;
                break;
            }
        }
        break;
    }
    return transform;
}

// @verified - d
const Matrix4x4 *mtwkGetBoneTransform(const Character *character, std::int32_t boneIndex) {
    return mtwkGetBoneTransform(SHENMUE_MOTION_CONST_CAST(MotionWork, charFindComp(character, CHARACTER_MOTION)), boneIndex);
}

// @verified - d
bool mtwkCopyBoneTransform(const MotionWork *work, std::int32_t boneIndex, Matrix4x4 *output) {
    if (!work || !output)
        return false;
    const Matrix4x4 *const matrix = mtwkGetBoneTransform(work, boneIndex);
    if (!matrix)
        return false;
    matrix_stack::CopyMatrix(output, matrix);
    return true;
}

bool mtwkSetBoneTransform(MotionWork *work, std::int32_t boneIndex, const Matrix4x4 *transform) {
    if (!work)
        return false;
    switch (boneIndex) {
    case 0:
        matrix_stack::CopyMatrix(reinterpret_cast<Matrix4x4 *>(reinterpret_cast<std::byte *>(work) + 0x230), transform);
        break;
    case 3:
        matrix_stack::CopyMatrix(reinterpret_cast<Matrix4x4 *>(reinterpret_cast<std::byte *>(work) + 0x1168), transform);
        break;
    case 4:
        matrix_stack::CopyMatrix(reinterpret_cast<Matrix4x4 *>(reinterpret_cast<std::byte *>(work) + 0x12d0), transform);
        break;
    case 5:
        matrix_stack::CopyMatrix(reinterpret_cast<Matrix4x4 *>(reinterpret_cast<std::byte *>(work) + 0x1508), transform);
        break;
    case 8:
        matrix_stack::CopyMatrix(reinterpret_cast<Matrix4x4 *>(reinterpret_cast<std::byte *>(work) + 0x1690), transform);
        break;
    case 9:
        matrix_stack::CopyMatrix(reinterpret_cast<Matrix4x4 *>(reinterpret_cast<std::byte *>(work) + 0x18c8), transform);
        break;
    case 12:
        matrix_stack::CopyMatrix(reinterpret_cast<Matrix4x4 *>(reinterpret_cast<std::byte *>(work) + 0x1908), transform);
        break;
    case 14:
        matrix_stack::CopyMatrix(reinterpret_cast<Matrix4x4 *>(reinterpret_cast<std::byte *>(work) + 0x1a80), transform);
        break;
    case 15:
        matrix_stack::CopyMatrix(reinterpret_cast<Matrix4x4 *>(reinterpret_cast<std::byte *>(work) + 0x1c10), transform);
        break;
    case 16:
        matrix_stack::CopyMatrix(reinterpret_cast<Matrix4x4 *>(reinterpret_cast<std::byte *>(work) + 0x1e48), transform);
        break;
    case 19:
        matrix_stack::CopyMatrix(reinterpret_cast<Matrix4x4 *>(reinterpret_cast<std::byte *>(work) + 0x1e88), transform);
        break;
    case 21:
        matrix_stack::CopyMatrix(reinterpret_cast<Matrix4x4 *>(reinterpret_cast<std::byte *>(work) + 0x2000), transform);
        break;
    case 22:
        matrix_stack::CopyMatrix(reinterpret_cast<Matrix4x4 *>(reinterpret_cast<std::byte *>(work) + 0x3e0), transform);
        break;
    case 23:
        matrix_stack::CopyMatrix(reinterpret_cast<Matrix4x4 *>(reinterpret_cast<std::byte *>(work) + 0x620), transform);
        break;
    case 24:
        matrix_stack::CopyMatrix(reinterpret_cast<Matrix4x4 *>(reinterpret_cast<std::byte *>(work) + 0x660), transform);
        break;
    case 26:
        matrix_stack::CopyMatrix(reinterpret_cast<Matrix4x4 *>(reinterpret_cast<std::byte *>(work) + 0x760), transform);
        break;
    case 28:
        matrix_stack::CopyMatrix(reinterpret_cast<Matrix4x4 *>(reinterpret_cast<std::byte *>(work) + 0xb10), transform);
        break;
    case 29:
        matrix_stack::CopyMatrix(reinterpret_cast<Matrix4x4 *>(reinterpret_cast<std::byte *>(work) + 0xb50), transform);
        break;
    case 31:
        matrix_stack::CopyMatrix(reinterpret_cast<Matrix4x4 *>(reinterpret_cast<std::byte *>(work) + 0xc50), transform);
        break;
    default:
        if (!work->useMatrixOverrides) {
            switch (boneIndex) {
            case 1:
                matrix_stack::CopyMatrix(reinterpret_cast<Matrix4x4 *>(reinterpret_cast<std::byte *>(work) + 0x1030), transform);
                break;
            case 13:
                matrix_stack::CopyMatrix(reinterpret_cast<Matrix4x4 *>(reinterpret_cast<std::byte *>(work) + 0x1980), transform);
                break;
            case 20:
                matrix_stack::CopyMatrix(reinterpret_cast<Matrix4x4 *>(reinterpret_cast<std::byte *>(work) + 0x1f00), transform);
                break;
            case 27:
                matrix_stack::CopyMatrix(reinterpret_cast<Matrix4x4 *>(reinterpret_cast<std::byte *>(work) + 0x898), transform);
                break;
            case 32:
                matrix_stack::CopyMatrix(reinterpret_cast<Matrix4x4 *>(reinterpret_cast<std::byte *>(work) + 0xd88), transform);
                break;
            default:
                return false;
            }
        } else {
            switch (boneIndex) {
            case 39:
                matrix_stack::CopyMatrix(*reinterpret_cast<Matrix4x4 **>(reinterpret_cast<std::byte *>(work) + 0x424), transform);
                break;
            case 40:
                matrix_stack::CopyMatrix(*reinterpret_cast<Matrix4x4 **>(reinterpret_cast<std::byte *>(work) + 0x914), transform);
                break;
            case 41:
                matrix_stack::CopyMatrix(*reinterpret_cast<Matrix4x4 **>(reinterpret_cast<std::byte *>(work) + 0xe04), transform);
                break;
            case 42:
                matrix_stack::CopyMatrix(*reinterpret_cast<Matrix4x4 **>(reinterpret_cast<std::byte *>(work) + 0xe0c), transform);
                break;
            case 43:
                matrix_stack::CopyMatrix(*reinterpret_cast<Matrix4x4 **>(reinterpret_cast<std::byte *>(work) + 0xe14), transform);
                break;
            case 33:
                matrix_stack::CopyMatrix(*reinterpret_cast<Matrix4x4 **>(reinterpret_cast<std::byte *>(work) + 0x155c), transform);
                break;
            case 35:
                matrix_stack::CopyMatrix(*reinterpret_cast<Matrix4x4 **>(reinterpret_cast<std::byte *>(work) + 0x1564), transform);
                break;
            case 36:
                matrix_stack::CopyMatrix(*reinterpret_cast<Matrix4x4 **>(reinterpret_cast<std::byte *>(work) + 0x156c), transform);
                break;
            case 37:
                matrix_stack::CopyMatrix(*reinterpret_cast<Matrix4x4 **>(reinterpret_cast<std::byte *>(work) + 0x1574), transform);
                break;
            default:
                return false;
            }
        }
    }
    return true;
}

// @verified - d
bool mtwkGetBonePosition(MotionWork *work, MotionBoneId bone, Vector3 *position) {
    const Matrix4x4 *transform;
    Matrix4x4 eye;
    if (work) {
        switch (bone) {
        case 2:
        case 7:
        case 10:
        case 17:
        case 25:
        case 30:
        case 34:
        case 38:
            return mtwkGetBoneTargetPosition(work, static_cast<std::int32_t>(bone), position);
        case 6: {
            if (!reinterpret_cast<const Matrix4x4 *>(reinterpret_cast<const std::byte *>(work) + 0x1508))
                return false;
            Matrix4x4_PushMatrixAndTranslate(*reinterpret_cast<const MotionHeadRig *>(reinterpret_cast<const std::byte *>(work) + kMotionHeadRigOffset), &eye);
            position->x = eye.rows[3].x;
            position->y = eye.rows[3].y;
            position->z = eye.rows[3].z;
            return true;
        }
        case 11:
            return mtwkGetBonePosition(work, static_cast<MotionBoneId>(13), position);
        case 18:
            return mtwkGetBonePosition(work, static_cast<MotionBoneId>(20), position);
        default:
            transform = mtwkGetBoneTransform(work, static_cast<std::int32_t>(bone));
            if (transform)
                goto copy;
            break;
        }
    }
    return false;
copy:
    position->x = transform->rows[3].x;
    position->y = transform->rows[3].y;
    position->z = transform->rows[3].z;
    return true;
}

// @verified - d
bool mtwkGetBoneTargetPosition(const MotionWork *work, std::int32_t selector, Vector3 *position) {
    const std::int32_t controller = ParentBoneId(static_cast<std::uint32_t>(selector));
    if (controller == -1)
        return false;
    return mtwkGetControllerTargetPosition(work, static_cast<std::uint32_t>(controller), *position);
}

bool mtwkGetControllerTargetPosition(const MotionWork *work, std::uint32_t controller, Vector3 &position) {
    const std::byte *target = 0;
    if (!work || static_cast<std::int32_t>(controller) > 8)
        return false;
    if (controller < 7U) {
        switch (controller) {
        case 0:
            mtwkGetBonePosition(const_cast<MotionWork *>(work), static_cast<MotionBoneId>(0), &position);
            return true;
        case 1:
            target = reinterpret_cast<const std::byte *>(work) + 0x0fec;
            break;
        case 2:
            target = reinterpret_cast<const std::byte *>(work) + 0x14c4;
            break;
        case 3:
            target = reinterpret_cast<const std::byte *>(work) + 0x1884;
            break;
        case 4:
            target = reinterpret_cast<const std::byte *>(work) + 0x1e04;
            break;
        case 5:
            target = reinterpret_cast<const std::byte *>(work) + 0x05dc;
            break;
        case 6:
            target = reinterpret_cast<const std::byte *>(work) + 0x0acc;
            break;
        }
    } else {
        if (!work->useMatrixOverrides)
            return false;
        switch (controller) {
        case 7:
            target = *reinterpret_cast<const std::byte *const *>(reinterpret_cast<const std::byte *>(work) + 0x03dc) + kMotionOverrideTargetPositionOffset;
            break;
        case 8:
            target = *reinterpret_cast<const std::byte *const *>(reinterpret_cast<const std::byte *>(work) + 0x1558) + kMotionOverrideTargetPositionOffset;
            break;
        }
    }
    const Vector3 *const source = reinterpret_cast<const Vector3 *>(target);
    position.x = source->x;
    position.y = source->y;
    position.z = source->z;
    return true;
}

// @verified - d
bool mtwkSetBoneTarget(MotionWork *work, std::int32_t selector, const Vector3 *position) {
    const std::int32_t controller = ParentBoneId(static_cast<std::uint32_t>(selector));
    if (controller == -1)
        return false;
    if (!position) {
        work->activeConstraintTarget[controller] = 0;
        work->latchedConstraintTarget[controller] = 0;
        return false;
    }
    work->constraintBlendSource[controller] = *position;
    work->activeConstraintTarget[controller] = &work->constraintBlendSource[controller];
    work->latchedConstraintTarget[controller] = 0;
    return true;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
bool mtwkSetControllerTarget(MotionWork *work, std::uint32_t controller, const Vector3 *position) {
    if (!position) {
        work->activeConstraintTarget[controller] = 0;
        work->latchedConstraintTarget[controller] = 0;
        return false;
    }
    work->constraintBlendSource[controller] = *position;
    work->activeConstraintTarget[controller] = &work->constraintBlendSource[controller];
    work->latchedConstraintTarget[controller] = 0;
    return true;
}
#endif

// @verified - d
bool mtwkLatchBoneTarget(MotionWork *work, std::int32_t selector, const Vector3 *position) {
    const std::int32_t controller = ParentBoneId(static_cast<std::uint32_t>(selector));
    if (controller == -1)
        return false;
    if (!position) {
        work->activeConstraintTarget[controller] = 0;
        work->latchedConstraintTarget[controller] = 0;
        return false;
    }
    work->constraintBlendSource[controller] = *position;
    work->activeConstraintTarget[controller] = 0;
    work->latchedConstraintTarget[controller] = &work->constraintBlendSource[controller];
    return true;
}

// @verified - d
bool mtwkLatchControllerTarget(MotionWork *work, std::uint32_t controller, const Vector3 *position) {
    if (!position) {
        work->activeConstraintTarget[controller] = 0;
        work->latchedConstraintTarget[controller] = 0;
        return false;
    }
    work->constraintBlendSource[controller] = *position;
    work->activeConstraintTarget[controller] = 0;
    work->latchedConstraintTarget[controller] = &work->constraintBlendSource[controller];
    return true;
}

// @verified - d
float mtwkGetBoneTargetStepLimit(const MotionWork *work, MotionBoneId bone) {
    const std::int32_t controller = ParentBoneId(static_cast<std::uint32_t>(bone));
    return controller == -1 ? 0.0f : work->constraintStepLimit[controller];
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float mtwkGetConstraintStepLimit(const MotionWork *work, std::int32_t controller) {
    return work->constraintStepLimit[controller];
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t mtwkGetConstraintAngle(const MotionWork *work) {
    return static_cast<std::int32_t>(work->constraintStepLimit[2] * 10430.378f) & 0xFFFFU;
}
#endif

// @verified - d
bool mtwkSetBoneTargetStepLimit(MotionWork *work, MotionBoneId bone, float limit) {
    const std::int32_t controller = ParentBoneId(static_cast<std::uint32_t>(bone));
    if (controller != -1 && !(limit < 0.0f)) {
        work->constraintStepLimit[controller] = limit;
        return true;
    }
    return false;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
bool mtwkSetConstraintStepLimit(MotionWork *work, std::int32_t controller, float limit) {
    if (limit < 0.0f)
        return false;
    work->constraintStepLimit[controller] = limit;
    return true;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool mtwkSetConstraintAngle(MotionWork *work, std::int32_t angle) {
    const float value = static_cast<float>(angle);
    if (value < 0)
        return false;
    work->constraintStepLimit[2] = value / 10430.378f;
    return true;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t mtwkGetFootstepAttributes(const MotionWork *work, std::uint8_t *attributes) {
    return MOTN_GetFootstepAttributes(mtwkGetAnimationId(work, static_cast<MotionRegion>(0)), attributes);
}

bool mtwkSetSequenceParameter0(MotionWork *work, std::uint32_t value) {
    if (!work)
        return false;
    *reinterpret_cast<std::uint32_t *>(work->retainedSequenceState + 2) = value;
    return true;
}

bool mtwkSetSequenceParameter1(MotionWork *work, std::uint32_t value) {
    if (!work)
        return false;
    *reinterpret_cast<std::uint32_t *>(work->retainedSequenceState + 6) = value;
    return true;
}

bool mtwkSetSequenceParameter2(MotionWork *work, std::uint32_t value) {
    if (!work)
        return false;
    *reinterpret_cast<std::uint32_t *>(work->retainedSequenceState + 10) = value;
    return true;
}

bool mtwkSetSequenceParameter3(MotionWork *work, std::uint32_t value) {
    if (!work)
        return false;
    *reinterpret_cast<std::uint32_t *>(work->retainedSequenceState + 14) = value;
    return true;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool mtwkAssignTargetCallback(MotionWork *work, MotionTargetCallback callback, std::int32_t selector) {
    if (!work)
        return false;
    const std::int32_t controller = ParentBoneId(static_cast<std::uint32_t>(selector));
    if (controller == -1)
        return false;
    switch (controller) {
    case 0:
        reinterpret_cast<MotionWorkBindings *>(work)->targets[0].callback = callback;
        break;
    case 1:
        reinterpret_cast<MotionWorkBindings *>(work)->targets[4].callback = callback;
        break;
    case 2:
        reinterpret_cast<MotionWorkBindings *>(work)->targets[6].callback = callback;
        break;
    case 3:
        reinterpret_cast<MotionWorkBindings *>(work)->targets[7].callback = callback;
        break;
    case 4:
        reinterpret_cast<MotionWorkBindings *>(work)->targets[8].callback = callback;
        break;
    case 5:
        reinterpret_cast<MotionWorkBindings *>(work)->targets[2].callback = callback;
        break;
    case 6:
        reinterpret_cast<MotionWorkBindings *>(work)->targets[3].callback = callback;
        break;
    default:
        if (!work->useMatrixOverrides)
            return false;
        switch (controller) {
        case 7:
            reinterpret_cast<MotionWorkBindings *>(work)->targets[1].callback = callback;
            break;
        case 8:
            reinterpret_cast<MotionWorkBindings *>(work)->targets[5].callback = callback;
            break;
        }
        break;
    }
    return true;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool mtwkAssignTargetParameter(MotionWork *work, void *parameter, std::int32_t selector) {
    if (!work)
        return false;
    const std::int32_t controller = ParentBoneId(static_cast<std::uint32_t>(selector));
    if (controller == -1)
        return false;
    switch (controller) {
    case 0:
        reinterpret_cast<MotionWorkBindings *>(work)->targets[0].user = parameter;
        break;
    case 1:
        reinterpret_cast<MotionWorkBindings *>(work)->targets[4].user = parameter;
        break;
    case 2:
        reinterpret_cast<MotionWorkBindings *>(work)->targets[6].user = parameter;
        break;
    case 3:
        reinterpret_cast<MotionWorkBindings *>(work)->targets[7].user = parameter;
        break;
    case 4:
        reinterpret_cast<MotionWorkBindings *>(work)->targets[8].user = parameter;
        break;
    case 5:
        reinterpret_cast<MotionWorkBindings *>(work)->targets[2].user = parameter;
        break;
    case 6:
        reinterpret_cast<MotionWorkBindings *>(work)->targets[3].user = parameter;
        break;
    default:
        if (!work->useMatrixOverrides)
            return false;
        switch (controller) {
        case 7:
            reinterpret_cast<MotionWorkBindings *>(work)->targets[1].user = parameter;
            break;
        case 8:
            reinterpret_cast<MotionWorkBindings *>(work)->targets[5].user = parameter;
            break;
        }
        break;
    }
    return true;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
MotionTargetCallback mtwkGetTargetCallback(const MotionWork *work, std::int32_t selector) {
    if (!work)
        return 0;
    const std::int32_t controller = ParentBoneId(static_cast<std::uint32_t>(selector));
    if (controller == -1)
        return 0;
    switch (controller) {
    case 0:
        return reinterpret_cast<const MotionWorkBindings *>(work)->targets[0].callback;
    case 1:
        return reinterpret_cast<const MotionWorkBindings *>(work)->targets[4].callback;
    case 2:
        return reinterpret_cast<const MotionWorkBindings *>(work)->targets[6].callback;
    case 3:
        return reinterpret_cast<const MotionWorkBindings *>(work)->targets[7].callback;
    case 4:
        return reinterpret_cast<const MotionWorkBindings *>(work)->targets[8].callback;
    case 5:
        return reinterpret_cast<const MotionWorkBindings *>(work)->targets[2].callback;
    case 6:
        return reinterpret_cast<const MotionWorkBindings *>(work)->targets[3].callback;
    default:
        if (!work->useMatrixOverrides)
            return 0;
        switch (controller) {
        case 7:
            return reinterpret_cast<const MotionWorkBindings *>(work)->targets[1].callback;
        case 8:
            return reinterpret_cast<const MotionWorkBindings *>(work)->targets[5].callback;
        }
        return 0;
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void *mtwkGetTargetParameter(const MotionWork *work, std::int32_t selector) {
    if (!work)
        return 0;
    const std::int32_t controller = ParentBoneId(static_cast<std::uint32_t>(selector));
    if (controller == -1)
        return 0;
    switch (controller) {
    case 0:
        return reinterpret_cast<const MotionWorkBindings *>(work)->targets[0].user;
    case 1:
        return reinterpret_cast<const MotionWorkBindings *>(work)->targets[4].user;
    case 2:
        return reinterpret_cast<const MotionWorkBindings *>(work)->targets[6].user;
    case 3:
        return reinterpret_cast<const MotionWorkBindings *>(work)->targets[7].user;
    case 4:
        return reinterpret_cast<const MotionWorkBindings *>(work)->targets[8].user;
    case 5:
        return reinterpret_cast<const MotionWorkBindings *>(work)->targets[2].user;
    case 6:
        return reinterpret_cast<const MotionWorkBindings *>(work)->targets[3].user;
    default:
        if (!work->useMatrixOverrides)
            return 0;
        switch (controller) {
        case 7:
            return reinterpret_cast<const MotionWorkBindings *>(work)->targets[1].user;
        case 8:
            return reinterpret_cast<const MotionWorkBindings *>(work)->targets[5].user;
        }
        return 0;
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t mtwkGetBonePairHeading(MotionWork *work, MotionBoneId first, MotionBoneId second) {
    Vector3 a, b;
    mtwkGetBonePosition(work, first, &a);
    mtwkGetBonePosition(work, second, &b);
    if (a.x == b.x && a.z == b.z)
        return 0x10000;
    return static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(a.x - b.x, a.z - b.z));
}
#endif

// @verified - d
std::uint32_t mtwkGetClipMode(const MotionWork *work) {
    return work->clipMode;
}

// @verified - d
void mtwkSetClipMode(MotionWork *work, std::int32_t mode) {
    work->clipMode = mode;
}

// @verified - d
std::int32_t charGetMotionWorkYaw(const Character *character) {
    const MotionWork *const work = static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION));
    if (work) {
        std::int16_t yaw = static_cast<std::int16_t>(mtwkGetEntityYaw(work));
        for (const CharacterNode *parent = GetCharacterNode(const_cast<Character *>(character))->parent; parent; parent = parent->parent)
            yaw = static_cast<std::int16_t>(yaw - static_cast<std::int16_t>(parent->rotation.y));
        return yaw;
    }
    return shenGetRotYFromNode(character);
}

// @verified - d
std::int32_t GetCharacterMotionWorldYaw(const Character *character) {
    const MotionWork *const work = static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION));
    return work ? mtwkGetEntityYaw(work) : GetCharacterWorldYaw(character);
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t charGetBonePairHeading(Character *character, MotionBoneId first, MotionBoneId second) {
    MotionWork *const work = static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION));
    if (!work)
        return GetCharacterWorldYaw(character);
    const std::int32_t heading = mtwkGetBonePairHeading(work, first, second);
    if (heading == 0x10000)
        return GetCharacterWorldYaw(character);
    return heading;
}
#endif

// @verified - d
void charaGetMotWorldPos(Character *character, std::int32_t bone, Vector3 *position) {
    MotionWork *const work = static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION));
    if (!work || !mtwkGetBonePosition(work, static_cast<MotionBoneId>(bone), position))
        GetCharacterWorldPosition(character, position);
}

// @verified - d
void mtwkSetUseEnvFlag(std::int32_t enabled) {
    g_motionUseEnvironment = enabled;
}

// @verified - d
void mtwkSetUnknownField(MotionWork *work, std::uint32_t value) {
    if (work)
        work->retainedMotionParameter = value;
}

// @verified - d
std::uint32_t mtwkGetTrackedRootMode(const MotionWork *work) {
    for (std::int32_t index = 0; index < 5; ++index)
        if (work->regionFlags[index] & 0x10U)
            return work->currentRootMode;
    return 0;
}

// @verified - d
bool mtwkSetRequestedRootMode(MotionWork *work, std::uint32_t mode) {
    if (mode < 6) {
        work->requestedRootMode = static_cast<std::uint8_t>(mode);
        return true;
    }
    return false;
}

// @verified - d
float mtwkGetTransitionFramesRemaining(const MotionWork *work, MotionRegion region) {
    const std::uint32_t index = static_cast<std::uint32_t>(region);
    if (!work || !(work->regionFlags[index] & 0x10U))
        return 0.0f;
    float current;
    mtwkCopyRegionTimes(work, 0, &current, 0, static_cast<MotionRegion>(index));
    switch (work->currentRootMode) {
    case 0:
    default:
        return 0.0f;
    case 1:
    case 3:
    case 4:
    case 5:
        return static_cast<float>(work->blendTotalFrames[index]) - current;
    case 2:
        return static_cast<float>(work->blendRemainingFrames[index]);
    }
}

// @verified - d
bool mtwkSetFrameTime(MotionWork *work, std::uint32_t frames, MotionRegion region) {
    if (static_cast<std::uint32_t>(region) < 5) {
        work->defaultRegionStepFrames[region] = static_cast<std::uint16_t>(frames > 255 ? 255 : frames);
        return true;
    }
    return false;
}

// @verified - d
bool mtwkGetParentBoneWrapper(MotionWork *work, std::int32_t selector) {
    std::uint8_t flags = 0;
    std::uint32_t absolute = static_cast<std::uint32_t>(selector);
    if (selector < 0) {
        flags |= 0x80U;
        absolute = static_cast<std::uint32_t>(static_cast<std::int32_t>(std::fabs(static_cast<float>(selector))));
    } else if (absolute & 0x70000000U) {
        if (absolute & 0x40000000U)
            flags |= 0x80U;
        if (absolute & 0x20000000U)
            flags |= 0x40U;
        if (absolute & 0x10000000U)
            flags |= 0x20U;
    }
    const std::int32_t parent = ParentBoneId(absolute & 0xffU);
    if (parent != -1 && work->currentMotionId[0]) {
        work->parentBoneSelector = flags | static_cast<std::uint8_t>(parent);
        return true;
    }
    return false;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void mtwkSetRootTransformOverride(MotionWork *work, const Matrix4x4 *transform) {
    if (work)
        work->transformStart = transform;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void mtwkSetEnvironmentEnabled(MotionWork *work, std::uint32_t enabled) {
    if (work)
        work->retainedMotionParameter = enabled;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool mtwkIsRightFootLower(MotionWork *work) {
    Vector3 left, right;
    mtwkGetBonePosition(work, 0x19, &left);
    mtwkGetBonePosition(work, 0x1e, &right);
    if (left.y > right.y)
        return true;
    return false;
}
#endif

// @verified - d
bool mtwkIsRightFootTargetAhead(MotionWork *work) {
    Vector3 left, right, root;
    mtwkGetBonePosition(work, static_cast<MotionBoneId>(0), &root);
    mtwkGetBonePosition(work, static_cast<MotionBoneId>(25), &left);
    mtwkGetBonePosition(work, static_cast<MotionBoneId>(30), &right);
    left.x -= root.x;
    left.z -= root.z;
    right.x -= root.x;
    right.z -= root.z;
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(0U - static_cast<std::uint32_t>(work->entityYaw));
    Matrix4x4::TransformPoint(left, left);
    Matrix4x4::TransformPoint(right, right);
    PopCurrentMatrix(1);
    if (left.z < right.z)
        return true;
    return false;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t mtwkSetProjectionAngle(MotionWork *work, std::int32_t angle, std::int32_t slot) {
    switch (slot) {
    case 0:
        work->retainedCollisionValues[0] = dc::sdk::CosFixed(static_cast<std::uint16_t>(angle)) / dc::sdk::SinFixed(static_cast<std::uint16_t>(angle));
        return 1;
    case 1:
        work->retainedCollisionValues[1] = dc::sdk::CosFixed(static_cast<std::uint16_t>(angle)) / dc::sdk::SinFixed(static_cast<std::uint16_t>(angle));
        return 1;
    case 2:
        work->retainedCollisionValues[2] = -(dc::sdk::CosFixed(static_cast<std::uint16_t>(angle)) / dc::sdk::SinFixed(static_cast<std::uint16_t>(angle)));
        return 1;
    }
    return 0;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::uint32_t mtwkGetProjectionAngle(const MotionWork *work, std::int32_t slot) {
    float value;
    switch (slot) {
    case 0:
        value = work->retainedCollisionValues[0];
        break;
    case 1:
        value = work->retainedCollisionValues[1];
        break;
    case 2:
        value = work->retainedCollisionValues[2];
        break;
    default:
        return 0;
    }
    return dc::sdk::ArcTanFixed(std::fabs(value));
}
#endif

// @verified - d
void charaGetMotNodePos(Character *character, std::int32_t bone, Vector3 *position) {
    MotionWork *const work = static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION));
    if (work && mtwkGetBonePosition(work, static_cast<MotionBoneId>(bone), position))
        InverseTransformPointByCharacterNode(GetCharacterNode(character), position);
    else
        GetCharacterLocalPosition(character, position);
}

// @verified - d
void charaGetMotLocalPos(Character *character, std::int32_t bone, Vector3 *position) {
    MotionWork *const work = static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION));
    if (work && mtwkGetBonePosition(work, static_cast<MotionBoneId>(bone), position)) {
        CharacterNode *const parent = GetCharacterNode(character)->parent;
        if (parent)
            InverseTransformPointByCharacterNode(parent, position);
    } else {
        GetCharacterLocalPosition(character, position);
    }
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool charaLatchBoneTarget(Character *character, std::int32_t selector, const Vector3 *position) {
    MotionWork *const work = static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION));
    return mtwkLatchBoneTarget(work, selector, position);
}
#endif

// @verified - d
bool SetCharacterMotionTarget(Character *character, std::int32_t selector, const Vector3 *position) {
    return mtwkSetBoneTarget(static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION)), selector, position);
}

// @verified - d
bool LatchCharacterMotionTargetInParentSpace(Character *character, std::int32_t selector, const Vector3 *position) {
    MotionWork *const work = static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION));
    Vector3 transformed = *position;
    CharacterNode *const parent = GetCharacterNode(character)->parent;
    if (parent)
        TransformPointByCharacterNode(parent, &transformed);
    return mtwkLatchBoneTarget(work, selector, &transformed);
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t charMotionTargetNullsub() {
    return 1;
}
#endif

#if SHENMUE_KIND == SHENMUE_KIND_GAMEJAM
bool ApplyCharacterMotionToPose(Character *character, Vector3 *position, Vector3i *rotation, float frame, std::uint16_t motion) {
    if (character) {
        MotionWork *const work = static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION));
        if (work) {
            if (!position || !rotation)
                return false;
            if (work->motionFlags & 0x200U) {
                matrix_stack::PushUnitMatrix();
                Matrix4x4::preTranslate(position->x, position->y, position->z);
                Matrix4x4::RotateAroundY_Wrapper(rotation->y);
                SampleNativeMotionRootAndRotation(motion, work->rigBoneId, frame, position, rotation);
                Matrix4x4::preTranslate(position->x, position->y, position->z);
                Matrix4x4::RotateAroundZ_Wrapper(rotation->z);
                Matrix4x4::RotateAroundY_Wrapper(rotation->y);
                Matrix4x4::RotateAroundX_Wrapper(rotation->x);
                Matrix4x4 transform;
                matrix_stack::GetMatrix(&transform);
                PopCurrentMatrix(1);
                position->x = transform.rows[3].x;
                position->y = transform.rows[3].y;
                position->z = transform.rows[3].z;
                rotation->y = static_cast<std::int32_t>(mtwkApplyTransforms(&transform));
                rotation->x = rotation->z = 0;
                return true;
            }
        }
    }
    return false;
}
#endif

// @verified - d
bool SampleNativeMotionRoot(std::uint16_t animation, std::int32_t boneId, float frame, Vector3 *output) {
    const MotnAnimationHeader *const data = MOTN_GetDataForAnimId(animation);
    if (!data) {
        if (output)
            output->x = output->y = output->z = 0.0f;
        return false;
    }
    const std::uint16_t sourceRig = data->flags & 0xffU;
    if (boneId > 16)
        boneId = 8;
    float scale;
    if (boneId < 0)
        scale = 1.0f;
    else
        scale = GetMotionRigScale(static_cast<std::uint16_t>(boneId), sourceRig, 0);
    MotionRigAxis axes[3];
    UpdateAnimRate(axes[0].channel, scale, 0.0f);
    UpdateAnimRate(axes[1].channel, scale, 0.0f);
    UpdateAnimRate(axes[2].channel, scale, 0.0f);
    float sampled[3];
    SampleMotionStreamChannels(axes, data, sampled, 0, 3, frame);
    if (output) {
        output->x = sampled[0];
        output->y = sampled[1];
        output->z = sampled[2];
    }
    return true;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t charGetMotionRigType(const Character *character) {
    const std::int32_t type = GetCharacterImageSourceType(character);
    if (static_cast<std::uint32_t>(type) > 96U)
        return -1;
    return static_cast<std::int8_t>(entityToRig[type]);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t charSetExtraFlag(Character *character, std::uint32_t flag, std::int32_t operation) {
    return mtwkSetExtraFlag(static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION)), flag, operation);
}
#endif

// @verified - d
bool mtwkAssignRigYawOffset(MotionWork *work, std::int32_t offset) {
    if (!work)
        return false;
    work->requestedRootYawOffset = offset;
    return true;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float mtwkSetRigScale(MotionWork *work, float scale) {
    float result = -1.0f;
    const MotnAnimationHeader *sequence = 0;
    if (work) {
        if (!work->currentMotionId[0])
            return result;
        std::uint16_t motionId = work->currentMotionId[0];
        if (motionId)
            sequence = MOTN_GetDataForAnimId(motionId);
        else if ((motionId = work->queuedMotionId[0]) != 0)
            sequence = MOTN_GetDataForAnimId(motionId);
        if (!sequence)
            return result;
        const float factor = mtwkGetScaleFactorFor(work->rigBoneId, static_cast<std::uint32_t>(static_cast<std::int8_t>(sequence->flags)), 0);
        if (factor != 0.0f) {
            result = reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.pelvisPosition[2].channel.scale / factor;
            work->overrideBlendTime = scale;
        }
    }
    return result;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetCashOrSegaCoins() {
    if (moneyIconFlags & 0x20U)
        return;
    if (moneyIconDrawMode)
        g_nativePlayerStats.cash = moneyIconDisplay;
    else
        g_nativePlayerStats.segaCoins = moneyIconDisplay;
}

void SetMoneyRelatedUIElement_0Wrapper(std::int32_t maximum, std::int16_t duration, std::int32_t step, std::uint32_t flags) {
    SetMoneyRelatedUIElement_0(maximum, duration, step, flags);
}

void SetMoneyRelatedUIElementCopy(std::int16_t maximum, std::int16_t duration, std::int16_t step) {
    SetMoneyRelatedUIElement_0(maximum & 0x7fff, static_cast<std::int16_t>(duration & 0x7fff), step & 0x7fff, 15);
}
#endif

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t CopyAbs(std::int32_t value) {
    return value < 0 ? -value : value;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
Vector3i CopyGetRotation(const Character *character) {
    Vector3i rotation;
    GetCharacterWorldRotation(character, &rotation);
    return rotation;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int16_t CopyYawDifference(std::int32_t a, std::int32_t b) {
    return static_cast<std::int16_t>(a - b);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float CopySine(std::int32_t yaw) {
    return dc::sdk::SinFixed(static_cast<std::int16_t>(yaw));
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float CopyCosine(std::int32_t yaw) {
    return dc::sdk::CosFixed(static_cast<std::uint16_t>(yaw));
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float CopyDistance(const Vector3 &a, const Vector3 &b) {
    const Vector3 delta = {a.x - b.x, 0.0f, a.z - b.z};
    return Vector3::CalcMagnitude(delta);
}
#endif

} // namespace

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int16_t CopyHeading(const Vector3 &from, const Vector3 &to) {
    return static_cast<std::int16_t>(InverseFixedAtan2(to.x - from.x, to.z - from.z));
}
#endif

namespace {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int16_t CopyDirection(const Vector3 &from, const Vector3 &to, std::int16_t zeroYaw) {
    return CopyDistance(from, to) <= 0.00000011920929f ? zeroYaw : CopyHeading(from, to);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void CopyMoveForward(Vector3 &position, std::int16_t yaw, float distance) {
    position.x -= CopySine(yaw) * distance;
    position.z -= CopyCosine(yaw) * distance;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint16_t CopyGaitMotion(const MotionWork &state) {
    return state.defaultAnimations[(state.copyFlags & MCF_AlternateLocomotion) ? 4 : 1];
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
float CopyStride(const MotionWork &work) {
    return GetMotionRigScale(work.rigBoneId, 8, 0) * ((work.copyFlags & MCF_Running) ? 1.2f : 0.55000001f);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint8_t CopyScaledDelay(const MotionWork &state, float frames) {
    return static_cast<std::uint8_t>(TruncateToInt32(1.0f / state.motionScale * frames));
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void CopySetState(MotionWork &state, std::uint8_t operation, std::uint8_t phase) {
    state.copyOperationState = operation;
    state.copyPhase = phase;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void CopyRequestFootEventEarly(MotionWork &work) {
    if (work.copyFlags & MCF_LeadFoot)
        mtwkGetParentBoneWrapper(&work, 0x4000001e);
    else
        mtwkGetParentBoneWrapper(&work, 0x40000019);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void CopyRequestFootEvent(MotionWork &work) {
    if (work.copyFlags & MCF_LeadFoot)
        mtwkGetParentBoneWrapper(&work, 0x6000001e);
    else
        mtwkGetParentBoneWrapper(&work, 0x60000019);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void CopySetLeadFoot(MotionWork &work, std::int32_t stepping, std::int32_t motion, std::int32_t reversePhase, std::int32_t splitWindow) {
    std::int32_t halfPhase = -1;
    if (stepping)
        work.motionFlags |= 0x8000U;
    else
        work.motionFlags &= ~0x8000U;
    if (splitWindow)
        work.gaitSplitFraction = 0.64999998f;
    if (work.copyFlags & MCF_LeadFoot) {
        work.motionFlags &= ~0x10000U;
        if (motion)
            halfPhase = stepping != 0;
    } else {
        work.motionFlags |= 0x10000U;
        if (motion)
            halfPhase = stepping == 0;
    }
    if (reversePhase)
        halfPhase ^= 1;
    if (motion && halfPhase)
        mtwkSetRegionEvalTime(&work, static_cast<float>(static_cast<std::int32_t>(GetMotionFrameCount(motion)) / 2), LowerBody);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void CopyPlayAt(MotionWork &work, const Vector3 &position, std::int32_t yaw, std::uint16_t motion, std::uint16_t mode, std::uint32_t frames) {
    Vector3i rotation;
    SetCharacterWorldPosition(g_transitionCharacter, &position);
    rotation.x = 0;
    rotation.y = yaw;
    rotation.z = 0;
    charaSetWorldRot(g_transitionCharacter, rotation);
    mtwkRequestMotion(&work, motion, LowerBody);
    mtwkSetRequestedRootMode(&work, mode);
    frames = static_cast<std::int32_t>(static_cast<float>(frames) * (1.0f / work.motionScale));
    mtwkSetFrameTime(&work, frames, LowerBody);
    mtwkSetSpeed(&work, work.motionScale, LowerBody);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void CopyReleaseLook(MotionWork &work) {
    if (!(work.copyFlags & MCF_LookSuppressed))
        return;
    mtwkUpdateWithPosition(&work, 0, 0);
    mtwkUpdateWithPosition(&work, 0, 0x304);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void CopyAdvanceScale(MotionWork &work) {
    MotionWork &state = work;
    --state.copyScaleCountdown;
    const float scale = state.insideRectangleScale + (1.0f - state.insideRectangleScale) * (static_cast<float>(state.copyScaleCountdown) * 0.125f);
    if (scale < 1.0f) {
        mtwkSetRigScale(&work, scale);
        mtwkSetSpeed(&work, (1.4f - scale * 0.40000001f) * state.motionScale, LowerBody);
    }
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t CopyStepWindowFrames(const MotionWork &work, std::uint16_t motion, std::uint8_t full) {
    if (work.copyFlags & MCF_Running) {
        if (full)
            return static_cast<std::int32_t>(GetMotionFrameCount(motion)) - 2;
        return static_cast<std::int32_t>(GetMotionFrameCount(motion)) / 2 - 2;
    }
    if (full)
        return static_cast<std::int32_t>(GetMotionFrameCount(motion)) - 3;
    return static_cast<std::int32_t>(GetMotionFrameCount(motion)) / 2 - 3;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void CopyToggleLeadFoot(MotionWork &work) {
    if (work.copyFlags & MCF_LeadFoot)
        work.copyFlags &= ~MCF_LeadFoot;
    else
        work.copyFlags |= MCF_LeadFoot;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
bool CopyReachedStepWindow(MotionWork &work, std::uint16_t motion) {
    float frame, speed, window, end, limit;
    bool result = false;
    if (!(work.regionFlags[0] & MRF_Transition)) {
        if ((*reinterpret_cast<const std::uint32_t *>(&work.regionFlags[0]) & (MRF_Unknown02 | MRF_Loop)) == MRF_Unknown02) {
            result = true;
        } else {
            frame = mtwkGetFrame(&work, LowerBody);
            speed = mtwkGetSpeed(&work, LowerBody);
            window = static_cast<float>(CopyStepWindowFrames(work, motion, 0));
            end = static_cast<float>(static_cast<std::int32_t>(GetMotionFrameCount(motion)));
            end -= 1.0f;
            if (!(end * 0.5f * 0.5f > frame) && end * 0.5f * 1.5f > frame) {
                if (work.regionFlags[0] & MRF_Mirrored)
                    work.copyFlags |= MCF_SampledLeadFoot;
                else
                    work.copyFlags &= ~MCF_SampledLeadFoot;
            } else if (work.regionFlags[0] & MRF_Mirrored)
                work.copyFlags &= ~MCF_SampledLeadFoot;
            else
                work.copyFlags |= MCF_SampledLeadFoot;
            if (frame - speed > 0.0f) {
                if (!(window > frame) && window > frame - speed)
                    result = true;
                else {
                    window = static_cast<float>(CopyStepWindowFrames(work, motion, 1));
                    if (!(window > frame) && window > frame - speed)
                        result = true;
                }
            } else if ((!(window > frame) && window > 0.0f) || (!(window > end) && window > frame - speed + end)) {
                result = true;
            } else {
                window = static_cast<float>(CopyStepWindowFrames(work, motion, 1));
                if (!(window > frame) && window > 0.0f)
                    result = true;
                else if (!(window > end) && window > frame - speed + end)
                    result = true;
            }
        }
    } else {
        if (work.motionFlags & 0x8000U) {
            if (work.motionFlags & 0x10000U)
                work.copyFlags |= MCF_SampledLeadFoot;
            else
                work.copyFlags &= ~MCF_SampledLeadFoot;
        } else if (work.copyFlags & MCF_LeadFoot)
            work.copyFlags |= MCF_SampledLeadFoot;
        else
            work.copyFlags &= ~MCF_SampledLeadFoot;
        frame = mtwkGetTransitionFramesRemaining(&work, LowerBody);
        if (work.copyFlags & MCF_Running) {
            if (1.1f > frame)
                result = true;
        } else {
            if (2.0999999f > frame)
                result = true;
        }
    }
    return result;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void CopyChooseFinalFoot(MotionWork &state, std::int32_t yaw, std::int32_t mode) {
    const std::int32_t travel = static_cast<std::int16_t>(state.copyTravelYaw - yaw);
    const std::int32_t final = static_cast<std::int16_t>(state.transitionStartFrame - yaw);
    if (mode == 2) {
        if (state.copyFlags & MCF_SampledLeadFoot)
            state.copyFlags |= MCF_LeadFoot;
        else
            state.copyFlags &= ~MCF_LeadFoot;
    } else if (travel > 0) {
        if (mode == 1)
            state.copyFlags &= ~MCF_LeadFoot;
        else if (final > 0) {
            if (travel > 0x3fff)
                state.copyFlags &= ~MCF_LeadFoot;
            else if (travel < final)
                state.copyFlags |= MCF_LeadFoot;
            else
                state.copyFlags &= ~MCF_LeadFoot;
        } else if (travel > 0x6000 && travel + final > 0)
            state.copyFlags |= MCF_LeadFoot;
        else
            state.copyFlags &= ~MCF_LeadFoot;
    } else if (mode == 1)
        state.copyFlags |= MCF_LeadFoot;
    else if (final <= 0) {
        if (travel > -0x4000 && travel > final)
            state.copyFlags &= ~MCF_LeadFoot;
        else
            state.copyFlags |= MCF_LeadFoot;
    } else if (travel < -0x6000 && travel + final < 0)
        state.copyFlags &= ~MCF_LeadFoot;
    else
        state.copyFlags |= MCF_LeadFoot;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint8_t CopyRequestLook(const Vector3 &destination, std::int16_t targetYaw) {
    Vector3 target;
    Vector3 position;
    Vector3 delta;
    Vector3i rotation;
    std::int16_t difference = 0;
    GetCharacterWorldPosition(g_transitionCharacter, &position);
    GetCharacterWorldRotation(g_transitionCharacter, &rotation);
    switch (SHENMUE_COPY_TYPED.transitionSubstate) {
    case 2:
        target = position;
        target.x -= dc::sdk::SinFixed(targetYaw) * 10.0f;
        target.z -= dc::sdk::CosFixed(targetYaw) * 10.0f;
        difference = static_cast<std::int16_t>(targetYaw - static_cast<std::int16_t>(rotation.y));
        break;
    case 1:
        target = destination;
        delta.x = target.x - position.x;
        delta.y = 0.0f;
        delta.z = target.z - position.z;
        if (Vector3::CalcMagnitude(delta) < 0.1f)
            return 0;
        difference = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(-delta.x, -delta.z) - static_cast<std::int16_t>(rotation.y));
        break;
    case 0: {
        target = destination;
        delta.x = target.x - position.x;
        delta.y = 0.0f;
        delta.z = target.z - position.z;
        const float distance = Vector3::CalcMagnitude(delta);
        if (mtwkGetScaleFactorFor(g_transitionWork->rigBoneId, 8, 0) * 0.55000001f > distance) {
            if (distance > 0.30000001f) {
                difference = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(-delta.x, -delta.z));
                if (static_cast<std::int32_t>(std::fabs(static_cast<float>(difference - targetYaw))) > 28672) {
                    difference =
                        std::fabs(static_cast<float>(difference - rotation.y)) < std::fabs(static_cast<float>(targetYaw - rotation.y)) ? difference : targetYaw;
                } else {
                    difference = static_cast<std::int16_t>(targetYaw + (difference - targetYaw) / 2);
                }
            } else {
                difference = targetYaw;
            }
            target = position;
            target.x -= dc::sdk::SinFixed(difference) * 10.0f;
            target.z -= dc::sdk::CosFixed(difference) * 10.0f;
            difference = static_cast<std::int16_t>(difference - static_cast<std::int16_t>(rotation.y));
        } else {
            difference = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(-delta.x, -delta.z) - static_cast<std::int16_t>(rotation.y));
        }
        break;
    }
    }
    if (difference > 28672 || difference < -28672)
        return 0;
    mtwkUpdateWithPosition(g_transitionWork, &target, 0x501);
    g_transitionWork->motionFlags |= 0x40000U;
    SHENMUE_COPY_TYPED.copyFlags |= MCF_LookSuppressed;
    return static_cast<std::uint8_t>(std::fabs(static_cast<float>(difference)) / 2048.0f);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
int CopyUpdateLook(std::uint8_t pending) {
    Vector3 destination;
    if (!pending) {
        if (!(g_transitionWork->sequenceFlags & MSF_LookAt) && !SHENMUE_COPY_STATE.copyDelayFrames) {
            SHENMUE_COPY_STATE.copyDelayFrames =
                CopyRequestLook(SHENMUE_COPY_STATE.copySourcePosition, static_cast<std::int16_t>(SHENMUE_COPY_STATE.transitionStartFrame));
            if (SHENMUE_COPY_STATE.copyDelayFrames)
                return 0;
            return 1;
        }
        if (SHENMUE_COPY_STATE.copyDelayFrames) {
            --SHENMUE_COPY_STATE.copyDelayFrames;
            return false;
        }
        return true;
    }
    if (!(g_transitionWork->sequenceFlags & MSF_LookAt) || (SHENMUE_COPY_STATE.copyFlags & MCF_LookSuppressed)) {
        if (SHENMUE_COPY_STATE.copyFlags & MCF_Unknown20000) {
            GetCharacterWorldPosition(g_transitionCharacter, &destination);
            destination.x += SHENMUE_COPY_STATE.copyDestinationPosition.x;
            destination.y += SHENMUE_COPY_STATE.copyDestinationPosition.y;
            destination.z += SHENMUE_COPY_STATE.copyDestinationPosition.z;
        } else {
            destination = SHENMUE_COPY_STATE.copyDestinationPosition;
        }
        CopyRequestLook(destination, static_cast<std::int16_t>(SHENMUE_COPY_STATE.transitionEndFrame));
    }
    return true;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t CopyAngleDelta(std::int32_t from, std::int32_t to, std::int32_t keepSign) {
    std::int32_t delta = static_cast<std::int16_t>(to - from);
    if (!keepSign)
        delta = static_cast<std::int32_t>(std::fabs(static_cast<float>(delta)));
    return delta;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void CopyPrepareSpline(MotionCopyState &state, const Vector3 &position, std::int32_t preserveYaw, float stride) {
    Vector3 delta;
    Vector3 target;
    Vector3 *point;
    std::int32_t heading;
    std::int32_t angle;
    float length;
    float sine, cosine;
    target = state.copySourcePosition;
    delta.x = target.x - position.x;
    delta.z = target.z - position.z;
    if (std::fabs(delta.x) < 0.001f && std::fabs(delta.z) < 0.001f)
        goto fail;
    heading = dc::sdk::ArcTan2Fixed(-delta.x, -delta.z);
    if (!preserveYaw) {
        angle = CopyAngleDelta(state.copyTravelYaw, static_cast<std::int16_t>(state.transitionStartFrame), 0);
        if (angle < 256 || angle > 18200)
            goto fail;
    } else {
        angle = CopyAngleDelta(state.copyTravelYaw, heading, 1);
        const float angleFloat = static_cast<float>(angle);
        if (std::fabs(angleFloat) > 18200.0f || std::fabs(angleFloat) < 256.0f)
            goto fail;
    }
    state.copySplineStatus = 0;
    length = CalculatePlanarDistanceBetweenPoints(&position, &target);
    point = state.copySplinePoints;
    if (!preserveYaw) {
        if (length < 0.5f)
            goto fail;
        do {
            sine = length * dc::sdk::SinFixed(state.copyTravelYaw);
            cosine = length * dc::sdk::CosFixed(state.copyTravelYaw);
        } while (0);
        point->x = position.x + sine;
        point->y = position.y;
        point->z = position.z + cosine;
        ++point;
        *point = position;
        ++point;
        point->x = position.x - sine;
        point->y = position.y;
        point->z = position.z - cosine;
        if (length > 0.89999998f)
            length = 0.89999998f;
        sine = length * dc::sdk::SinFixed(static_cast<std::int16_t>(state.transitionStartFrame));
        cosine = length * dc::sdk::CosFixed(static_cast<std::int16_t>(state.transitionStartFrame));
    } else {
        length *= dc::sdk::CosFixed(angle);
        if (length > 0.89999998f) {
            length = 0.89999998f;
        } else if (length < 0.5f) {
            state.copySplineStatus = 1;
            return;
        }
        sine = length * dc::sdk::SinFixed(state.copyTravelYaw);
        cosine = length * dc::sdk::CosFixed(state.copyTravelYaw);
        point->x = position.x + sine;
        point->y = position.y;
        point->z = position.z + cosine;
        ++point;
        *point = position;
        ++point;
        point->x = position.x - sine;
        point->y = position.y;
        point->z = position.z - cosine;
        delta.x = target.x - point->x;
        delta.z = target.z - point->z;
        if (std::fabs(delta.x) < 0.001f && std::fabs(delta.z) < 0.001f)
            goto fail;
        heading = dc::sdk::ArcTan2Fixed(-delta.x, -delta.z);
        length = CalculatePlanarDistanceBetweenPoints(point, &target) - stride;
        if (length < 0.5f) {
        fail:
            state.copySplineStatus = 1;
            return;
        }
        sine = length * dc::sdk::SinFixed(heading);
        cosine = length * dc::sdk::CosFixed(heading);
    }
    point[1].x = point->x - sine;
    point[1].y = point->y;
    point[1].z = point->z - cosine;
    ++point;
    point[1].x = point->x - sine;
    point[1].y = point->y;
    point[1].z = point->z - cosine;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
bool CopyBuildSpline(std::uint8_t preserveYaw) {
    Vector3 position;
    Vector3 delta;
    Vector3i rotation;
    if (SHENMUE_COPY_TYPED.copyFlags & MCF_ExtraStepRequested) {
        GetCharacterWorldPosition(g_transitionCharacter, &position);
        GetCharacterWorldRotation(g_transitionCharacter, &rotation);
        if (preserveYaw) {
            SHENMUE_COPY_TYPED.copyTravelYaw = static_cast<std::int16_t>(rotation.y);
        } else {
            delta.x = SHENMUE_COPY_TYPED.copySourcePosition.x - position.x;
            delta.y = 0.0f;
            delta.z = SHENMUE_COPY_TYPED.copySourcePosition.z - position.z;
            const float distance = Vector3::CalcMagnitude(delta);
            SHENMUE_COPY_TYPED.copyTravelYaw =
                (distance > 1.1920929e-07f) ? static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(-delta.x, -delta.z)) : static_cast<std::int16_t>(rotation.y);
        }
        CopyPrepareSpline(SHENMUE_COPY_TYPED, position, preserveYaw,
            mtwkGetScaleFactorFor(g_transitionWork->rigBoneId, 8, 0) * ((SHENMUE_COPY_TYPED.copyFlags & MCF_Running) ? 1.2f : 0.55000001f));
        if (SHENMUE_COPY_TYPED.copySplineStatus == 1) {
            SHENMUE_COPY_TYPED.copyFlags &= ~MCF_ExtraStepRequested;
            SHENMUE_COPY_TYPED.copyFlags &= ~MCF_ExtraStepComplete;
        } else {
            SHENMUE_COPY_TYPED.copySplineFrame = 4;
            InterpolateNpcSplineNormalized(SHENMUE_COPY_TYPED.copySplinePoints, 5, SHENMUE_COPY_TYPED.copySplineFrame, &SHENMUE_COPY_TYPED.copySplinePosition);
            SHENMUE_COPY_TYPED.copyFlags |= MCF_ExtraStepComplete;
        }
        return true;
    }
    return false;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
int CopyAdjustStride(MotionWork &work, const Vector3 &position, std::int32_t yaw, std::uint16_t motion, float stride) {
    if (work.copyFlags & MCF_Unknown40000)
        return 0;
    if ((work.copyFlags & (MCF_ExtraStepRequested | MCF_ExtraStepComplete)) == (MCF_ExtraStepRequested | MCF_ExtraStepComplete))
        return 0;
    Vector3 rootTravel;
    Vector3 offset;
    offset.x = work.copySourcePosition.x - position.x;
    offset.y = 0.0f;
    offset.z = work.copySourcePosition.z - position.z;
    float distance = Vector3::CalcMagnitude(offset);
    SampleNativeMotionRoot(motion, work.rigBoneId, -1.0f, &rootTravel);
    const float travel = std::fabs(rootTravel.z);
    switch (work.transitionClass) {
    case 0:
    case 2:
        if (work.copyOperationState == 5U) {
            work.copyTravelYaw = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(-offset.x, -offset.z));
            CopyChooseFinalFoot(work, yaw, 0);
            if (!(travel + travel + stride > distance) || !(distance > travel * 1.5f + stride))
                return 1;
            distance -= stride * 0.5f;
            const int frames = static_cast<int>(GetMotionFrameCount(motion));
            int totalFrames;
            if ((work.copyFlags & (MCF_LeadFoot | MCF_SampledLeadFoot)) == MCF_LeadFoot ||
                (work.copyFlags & (MCF_LeadFoot | MCF_SampledLeadFoot)) == MCF_SampledLeadFoot) {
                totalFrames = static_cast<int>(static_cast<float>(frames) * 2.5f);
                work.copyStepsRemaining = 5;
            } else {
                totalFrames = frames * 2;
                work.copyStepsRemaining = 4;
            }
            const float frameTravel = travel / static_cast<float>(frames);
            work.copyScaleCountdown = 8;
            work.insideRectangleScale = (distance / frameTravel - 4.0f) / static_cast<float>(totalFrames - 4);
            work.copyStepState = 1;
            break;
        } else {
            if (stride * 0.80000001f > distance) {
                work.copyStepsRemaining = 0;
                work.copyScaleCountdown = 0;
                work.copyStepState = 0;
                return 1;
            }
            if (!(travel + travel + stride > distance))
                return 1;
            distance -= stride * 0.5f;
            int steps = static_cast<int>(distance / (travel * 0.5f)) + 1;
            int rightFoot;
            if (work.copyFlags & MCF_LeadFoot)
                rightFoot = 0;
            else
                rightFoot = 1;
            int right = rightFoot;
            bool extra;
            work.copyTravelYaw = static_cast<std::int16_t>(dc::sdk::ArcTan2Fixed(-offset.x, -offset.z));
            CopyChooseFinalFoot(work, yaw, 0);
            extra = false;
            if (steps & 1) {
                if (((work.copyFlags & MCF_LeadFoot) && right) || (!(work.copyFlags & MCF_LeadFoot) && !right))
                    extra = true;
            } else {
                if (((work.copyFlags & MCF_LeadFoot) && !right) || (!(work.copyFlags & MCF_LeadFoot) && right))
                    extra = true;
            }
            if (extra)
                steps++;
            float scale = distance / (static_cast<float>(steps) * (travel * 0.5f));
            work.copyStepsRemaining = static_cast<std::uint8_t>(steps);
            work.copyScaleCountdown = 0;
            work.copyStepState = 1;
            if (scale < 0.69999999f) {
                steps = static_cast<int>(distance / (travel * 0.5f)) + 1;
                float extraDistance = 0.0f;
                if (stride > extraDistance) {
                    right = !right;
                    extra = false;
                    if (steps & 1) {
                        if (((work.copyFlags & MCF_LeadFoot) && right) || (!(work.copyFlags & MCF_LeadFoot) && !right))
                            extra = true;
                    } else {
                        if (((work.copyFlags & MCF_LeadFoot) && !right) || (!(work.copyFlags & MCF_LeadFoot) && right))
                            extra = true;
                    }
                    if (extra)
                        steps++;
                    const float alternateScale = (distance - extraDistance) / (static_cast<float>(steps) * (travel * 0.5f));
                    if (alternateScale > scale) {
                        scale = alternateScale;
                        work.copyStepsRemaining = static_cast<std::uint8_t>(steps);
                        work.copyExtraStepDistance = extraDistance;
                        work.copyScaleCountdown = 0;
                        work.copyStepState = 2;
                    }
                }
            }
            work.insideRectangleScale = scale;
            if (work.insideRectangleScale < 1.0f) {
                mtwkSetRigScale(&work, work.insideRectangleScale);
                mtwkSetSpeed(&work, (1.4f - work.insideRectangleScale * 0.40000001f) * work.motionScale, LowerBody);
            }
            break;
        }
    case 1:
        if ((work.copyFlags & MCF_Unknown80) && std::fabs(static_cast<float>(static_cast<std::int16_t>(work.transitionStartFrame - yaw))) >= 4096.0f) {
            if (work.copyOperationState == 5U) {
                work.copyTravelYaw = static_cast<std::int16_t>(work.transitionStartFrame);
                CopyChooseFinalFoot(work, yaw, 1);
                float approach = 0.0f;
                if (std::fabs(static_cast<float>(static_cast<std::int16_t>(work.copyTravelYaw - yaw))) >= 8192.0f)
                    approach = stride * 0.60000002f * 0.60000002f;
                if (!(travel + travel + approach > distance) || !(distance > travel * 1.5f + approach))
                    return 1;
                distance -= approach;
                const int frames = static_cast<int>(GetMotionFrameCount(motion));
                int totalFrames;
                if ((work.copyFlags & (MCF_LeadFoot | MCF_SampledLeadFoot)) == (MCF_LeadFoot | MCF_SampledLeadFoot) ||
                    (work.copyFlags & (MCF_LeadFoot | MCF_SampledLeadFoot)) == 0) {
                    totalFrames = static_cast<int>(static_cast<float>(frames) * 2.5f);
                    work.copyStepsRemaining = 5;
                } else {
                    totalFrames = frames * 2;
                    work.copyStepsRemaining = 4;
                }
                const float frameTravel = travel / static_cast<float>(frames);
                work.copyScaleCountdown = 8;
                work.insideRectangleScale = (distance / frameTravel - 4.0f) / static_cast<float>(totalFrames - 4);
                work.copyStepState = 1;
                break;
            } else {
                if (stride * 0.80000001f > distance) {
                    work.copyStepsRemaining = 0;
                    work.copyScaleCountdown = 0;
                    work.copyStepState = 0;
                    break;
                }
                float approach = 0.0f;
                if (std::fabs(static_cast<float>(static_cast<std::int16_t>(work.transitionStartFrame - yaw))) >= 8192.0f)
                    approach = stride * 0.25f;
                if (!(travel + travel + approach > distance))
                    return 1;
                distance -= approach;
                int steps = static_cast<int>(distance / (travel * 0.5f)) + 1;
                int rightFoot;
                if (work.copyFlags & MCF_LeadFoot)
                    rightFoot = 0;
                else
                    rightFoot = 1;
                int right = rightFoot;
                bool extra;
                work.copyTravelYaw = static_cast<std::int16_t>(work.transitionStartFrame);
                CopyChooseFinalFoot(work, yaw, 1);
                extra = false;
                if (steps & 1) {
                    if (((work.copyFlags & MCF_LeadFoot) && !right) || (!(work.copyFlags & MCF_LeadFoot) && right))
                        extra = true;
                } else {
                    if (((work.copyFlags & MCF_LeadFoot) && right) || (!(work.copyFlags & MCF_LeadFoot) && !right))
                        extra = true;
                }
                if (extra)
                    steps++;
                float scale = distance / (static_cast<float>(steps) * (travel * 0.5f));
                work.copyStepsRemaining = static_cast<std::uint8_t>(steps);
                work.copyScaleCountdown = 0;
                work.copyStepState = 1;
                if (scale < 0.69999999f) {
                    steps = static_cast<int>(distance / (travel * 0.5f)) + 1;
                    float extraDistance = 0.0f;
                    if (stride > extraDistance) {
                        right = !right;
                        extra = false;
                        if (steps & 1) {
                            if (((work.copyFlags & MCF_LeadFoot) && !right) || (!(work.copyFlags & MCF_LeadFoot) && right))
                                extra = true;
                        } else {
                            if (((work.copyFlags & MCF_LeadFoot) && right) || (!(work.copyFlags & MCF_LeadFoot) && !right))
                                extra = true;
                        }
                        if (extra)
                            steps++;
                        const float alternateScale = (distance - extraDistance) / (static_cast<float>(steps) * (travel * 0.5f));
                        if (alternateScale > scale) {
                            scale = alternateScale;
                            work.copyStepsRemaining = static_cast<std::uint8_t>(steps);
                            work.copyExtraStepDistance = extraDistance;
                            work.copyScaleCountdown = 0;
                            work.copyStepState = 2;
                        }
                    }
                }
                work.insideRectangleScale = scale;
                if (work.insideRectangleScale < 1.0f) {
                    mtwkSetRigScale(&work, work.insideRectangleScale);
                    mtwkSetSpeed(&work, (1.4f - work.insideRectangleScale * 0.40000001f) * work.motionScale, LowerBody);
                }
                break;
            }
        }
        break;
    }
    return 1;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
int CopyBeginStep(std::uint8_t directional) {
    Vector3 position;
    Vector3 delta;
    Vector3i rotation;
    std::int16_t moveYaw, faceYaw;
    bool running;
    std::int32_t frames;
    std::uint16_t motion;
    float nearStride, rigScale, stride, distance, scale;
    int result = 0;
    GetCharacterWorldPosition(g_transitionCharacter, &position);
    GetCharacterWorldRotation(g_transitionCharacter, &rotation);
    if (SHENMUE_COPY_STATE.copyFlags & MCF_LookSuppressed) {
        mtwkUpdateWithPosition(g_transitionWork, 0, 0);
        mtwkUpdateWithPosition(g_transitionWork, 0, 0x304);
    }
    nearStride = mtwkGetScaleFactorFor(g_transitionWork->rigBoneId, 8, 0) * 0.55000001f;
    if (directional) {
        distance = nearStride + nearStride;
        delta.x = -distance * dc::sdk::SinFixed(SHENMUE_COPY_STATE.copyTravelYaw);
        delta.y = 0.0f;
        delta.z = -distance * dc::sdk::CosFixed(SHENMUE_COPY_STATE.copyTravelYaw);
    } else {
        delta.x = SHENMUE_COPY_STATE.copySourcePosition.x - position.x;
        delta.y = 0.0f;
        delta.z = SHENMUE_COPY_STATE.copySourcePosition.z - position.z;
        distance = Vector3::CalcMagnitude(delta);
        SHENMUE_COPY_STATE.copyTravelYaw = distance > 1.1920929e-07f ? dc::sdk::ArcTan2Fixed(-delta.x, -delta.z) : rotation.y;
    }
    if (distance > nearStride || directional) {
        rigScale = mtwkGetScaleFactorFor(g_transitionWork->rigBoneId, 8, 0);
        stride = rigScale * ((SHENMUE_COPY_STATE.copyFlags & MCF_Running) ? 1.2f : 0.55000001f);
        running = (SHENMUE_COPY_STATE.copyFlags & MCF_Running) != 0;
        motion = (SHENMUE_COPY_STATE.copyFlags & MCF_AlternateLocomotion) ? SHENMUE_COPY_ANIMATION(4) : SHENMUE_COPY_ANIMATION(1);
        CopyChooseFinalFoot(*g_transitionWork, rotation.y, 1);
        if (std::fabs(static_cast<float>(static_cast<std::int16_t>(SHENMUE_COPY_STATE.copyTravelYaw - rotation.y))) < 20480.0f) {
            CopySetLeadFoot(*g_transitionWork, 1, motion, 0, running);
            if (g_transitionHadMotion) {
                scale = 1.0f - std::fabs(static_cast<float>(static_cast<std::int16_t>(SHENMUE_COPY_STATE.copyTravelYaw - rotation.y))) * 0.30000001f / 20480.0f;
                position.x += scale * nearStride * (delta.x / distance);
                position.z += scale * nearStride * (delta.z / distance);
            }
            CopyPlayAt(*g_transitionWork, position, SHENMUE_COPY_STATE.copyTravelYaw, motion, 1, (SHENMUE_COPY_STATE.copyFlags & MCF_Running) ? 8 : 16);
            if (!directional)
                CopyAdjustStride(*g_transitionWork, position, SHENMUE_COPY_STATE.copyTravelYaw, motion, stride);
            *g_copyTransitionState = 5;
            SHENMUE_COPY_STATE.copyPhase = 1;
        } else {
            if (SHENMUE_COPY_STATE.copyFlags & MCF_LeadFoot) {
                moveYaw = SHENMUE_COPY_STATE.copyTravelYaw - 0x1000;
                faceYaw = rotation.y - 0x5000;
            } else {
                moveYaw = SHENMUE_COPY_STATE.copyTravelYaw + 0x1000;
                faceYaw = rotation.y + 0x5000;
            }
            if (g_transitionHadMotion) {
                scale = stride * 0.5f;
                position.x -= scale * dc::sdk::SinFixed(moveYaw);
                position.z -= scale * dc::sdk::CosFixed(moveYaw);
            }
            CopySetLeadFoot(*g_transitionWork, 1, motion, 0, running);
            frames = (SHENMUE_COPY_STATE.copyFlags & MCF_Running) ? 8 : 16;
            CopyPlayAt(*g_transitionWork, position, faceYaw, motion, 1, frames);
            SHENMUE_COPY_STATE.copyDelayFrames = static_cast<std::uint8_t>(static_cast<float>(frames) * 0.60000002f);
            SHENMUE_COPY_STATE.copyDelayFrames = static_cast<std::uint8_t>(SHENMUE_COPY_STATE.copyDelayFrames * (1.0f / SHENMUE_COPY_STATE.motionScale));
            *g_copyTransitionState = 4;
            SHENMUE_COPY_STATE.copyPhase = 5;
        }
    } else {
        CopyChooseFinalFoot(*g_transitionWork, rotation.y, 0);
        SHENMUE_COPY_STATE.copyDelayFrames = 0;
        result = 1;
        *g_copyTransitionState = 2;
        SHENMUE_COPY_STATE.copyPhase = result;
    }
    return result;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
int CopyFinishStep(std::uint8_t continueWalking) {
    Vector3 position;
    Vector3i rotation;
    std::int16_t difference, destinationYaw;
    std::int32_t frames;
    float scale;
    int finished = 0;
    switch (SHENMUE_COPY_TYPED.copyPhase) {
    case 1:
    default:
        if (SHENMUE_COPY_TYPED.copyDelayFrames) {
            --SHENMUE_COPY_TYPED.copyDelayFrames;
            break;
        }
        SHENMUE_COPY_TYPED.copyStepState = 0;
        SHENMUE_COPY_TYPED.copyScaleCountdown = 0;
        SHENMUE_COPY_TYPED.copyStepsRemaining = 0;
        SHENMUE_COPY_TYPED.insideRectangleScale = 1.0f;
        GetCharacterWorldPosition(g_transitionCharacter, &position);
        GetCharacterWorldRotation(g_transitionCharacter, &rotation);
        if (SHENMUE_COPY_TYPED.copyFlags & MCF_LookSuppressed) {
            mtwkUpdateWithPosition(g_transitionWork, 0, 0);
            mtwkUpdateWithPosition(g_transitionWork, 0, 0x304);
        }
        difference = static_cast<std::int16_t>(SHENMUE_COPY_TYPED.transitionStartFrame - rotation.y);
        scale = ((SHENMUE_COPY_TYPED.copyFlags & MCF_Running) ? 0.69999999f : 1.0f) * 0.69999999f;
        frames = 16;
        g_transitionWork->motionFlags |= 0x8000U;
        if (SHENMUE_COPY_TYPED.copyFlags & MCF_LeadFoot) {
            g_transitionWork->motionFlags &= ~0x10000U;
            if (difference > 0x2000) {
                destinationYaw = static_cast<std::int16_t>(SHENMUE_COPY_TYPED.transitionStartFrame) - (difference - 0x2000);
                SHENMUE_COPY_TYPED.copyDelayFrames = static_cast<std::uint8_t>(static_cast<float>(frames) * scale);
            } else if (difference < -0x6000) {
                destinationYaw = static_cast<std::int16_t>(SHENMUE_COPY_TYPED.transitionStartFrame) - (difference + 0x6000);
                SHENMUE_COPY_TYPED.copyDelayFrames = static_cast<std::uint8_t>(static_cast<float>(frames) * scale);
            } else {
                destinationYaw = SHENMUE_COPY_TYPED.transitionStartFrame;
                SHENMUE_COPY_TYPED.copyDelayFrames = 0;
            }
        } else {
            g_transitionWork->motionFlags |= 0x10000U;
            if (difference > 0x6000) {
                destinationYaw = static_cast<std::int16_t>(SHENMUE_COPY_TYPED.transitionStartFrame) - (difference - 0x6000);
                SHENMUE_COPY_TYPED.copyDelayFrames = static_cast<std::uint8_t>(static_cast<float>(frames) * scale);
            } else if (difference < -0x2000) {
                destinationYaw = static_cast<std::int16_t>(SHENMUE_COPY_TYPED.transitionStartFrame) - (difference + 0x2000);
                SHENMUE_COPY_TYPED.copyDelayFrames = static_cast<std::uint8_t>(static_cast<float>(frames) * scale);
            } else {
                destinationYaw = SHENMUE_COPY_TYPED.transitionStartFrame;
                SHENMUE_COPY_TYPED.copyDelayFrames = 0;
            }
        }
        SHENMUE_COPY_TYPED.copyDelayFrames = static_cast<std::uint8_t>(SHENMUE_COPY_TYPED.copyDelayFrames * (1.0f / SHENMUE_COPY_TYPED.motionScale));
        position.x = SHENMUE_COPY_TYPED.copySourcePosition.x;
        position.z = SHENMUE_COPY_TYPED.copySourcePosition.z;
        if (SHENMUE_COPY_TYPED.copyDelayFrames) {
            CopyPlayAt(*g_transitionWork, position, destinationYaw, SHENMUE_COPY_TYPED.defaultAnimations[0], 1, frames);
            SHENMUE_COPY_TYPED.copyPhase = 2;
        } else if (continueWalking) {
            SHENMUE_COPY_TYPED.copyTravelYaw = SHENMUE_COPY_TYPED.transitionStartFrame;
            CopySetLeadFoot(*g_transitionWork, 1,
                (SHENMUE_COPY_TYPED.copyFlags & MCF_AlternateLocomotion) ? SHENMUE_COPY_TYPED.defaultAnimations[4] : SHENMUE_COPY_TYPED.defaultAnimations[1], 0,
                (SHENMUE_COPY_TYPED.copyFlags & MCF_Running) != 0);
            CopyPlayAt(*g_transitionWork, position, SHENMUE_COPY_TYPED.copyTravelYaw,
                (SHENMUE_COPY_TYPED.copyFlags & MCF_AlternateLocomotion) ? SHENMUE_COPY_TYPED.defaultAnimations[4] : SHENMUE_COPY_TYPED.defaultAnimations[1], 1,
                (SHENMUE_COPY_TYPED.copyFlags & MCF_Running) ? 8 : 16);
            *g_copyTransitionState = 5;
            SHENMUE_COPY_TYPED.copyPhase = 1;
        } else {
            if (SHENMUE_COPY_TYPED.copyFlags & MCF_Unknown1000) {
                if (SHENMUE_COPY_TYPED.copyFlags & MCF_Unknown2000) {
                    if (SHENMUE_COPY_TYPED.copyFlags & MCF_LeadFoot)
                        g_transitionWork->regionFlags[0] |= MRF_LeadFoot;
                    else
                        g_transitionWork->regionFlags[0] &= ~MRF_LeadFoot;
                } else if (SHENMUE_COPY_TYPED.copyFlags & MCF_Unknown80000)
                    g_transitionWork->regionFlags[0] |= MRF_LeadFoot;
                CopyPlayAt(*g_transitionWork, position, destinationYaw, SHENMUE_COPY_TYPED.defaultAnimations[5], 1, frames);
                if (SHENMUE_COPY_TYPED.copyLayerStartFrame > 0.0f)
                    g_transitionWork->pendingLayerStartFrame[0] = SHENMUE_COPY_TYPED.copyLayerStartFrame;
            } else
                CopyPlayAt(*g_transitionWork, position, destinationYaw, SHENMUE_COPY_TYPED.defaultAnimations[0], 1, frames);
            SHENMUE_COPY_TYPED.copyPhase = 3;
        }
        break;
    case 2:
        if (SHENMUE_COPY_TYPED.copyDelayFrames) {
            --SHENMUE_COPY_TYPED.copyDelayFrames;
            break;
        }
        GetCharacterWorldPosition(g_transitionCharacter, &position);
        GetCharacterWorldRotation(g_transitionCharacter, &rotation);
        CopyToggleLeadFoot(*g_transitionWork);
        position.x = SHENMUE_COPY_TYPED.copySourcePosition.x;
        position.z = SHENMUE_COPY_TYPED.copySourcePosition.z;
        if (continueWalking) {
            SHENMUE_COPY_TYPED.copyTravelYaw = SHENMUE_COPY_TYPED.transitionStartFrame;
            CopySetLeadFoot(*g_transitionWork, 1,
                (SHENMUE_COPY_TYPED.copyFlags & MCF_AlternateLocomotion) ? SHENMUE_COPY_TYPED.defaultAnimations[4] : SHENMUE_COPY_TYPED.defaultAnimations[1], 0,
                (SHENMUE_COPY_TYPED.copyFlags & MCF_Running) != 0);
            CopyPlayAt(*g_transitionWork, position, SHENMUE_COPY_TYPED.copyTravelYaw,
                (SHENMUE_COPY_TYPED.copyFlags & MCF_AlternateLocomotion) ? SHENMUE_COPY_TYPED.defaultAnimations[4] : SHENMUE_COPY_TYPED.defaultAnimations[1], 1,
                (SHENMUE_COPY_TYPED.copyFlags & MCF_Running) ? 8 : 16);
            *g_copyTransitionState = 5;
            SHENMUE_COPY_TYPED.copyPhase = 1;
        } else {
            CopySetLeadFoot(*g_transitionWork, 1, 0, 0, 0);
            if (SHENMUE_COPY_TYPED.copyFlags & MCF_Unknown1000) {
                if (SHENMUE_COPY_TYPED.copyFlags & MCF_Unknown2000) {
                    if (SHENMUE_COPY_TYPED.copyFlags & MCF_LeadFoot)
                        g_transitionWork->regionFlags[0] |= MRF_LeadFoot;
                    else
                        g_transitionWork->regionFlags[0] &= ~MRF_LeadFoot;
                } else if (SHENMUE_COPY_TYPED.copyFlags & MCF_Unknown80000)
                    g_transitionWork->regionFlags[0] |= MRF_LeadFoot;
                CopyPlayAt(*g_transitionWork, position, static_cast<std::int16_t>(SHENMUE_COPY_TYPED.transitionStartFrame),
                    SHENMUE_COPY_TYPED.defaultAnimations[5], 1, 16);
                if (SHENMUE_COPY_TYPED.copyLayerStartFrame > 0.0f)
                    g_transitionWork->pendingLayerStartFrame[0] = SHENMUE_COPY_TYPED.copyLayerStartFrame;
            } else
                CopyPlayAt(*g_transitionWork, position, static_cast<std::int16_t>(SHENMUE_COPY_TYPED.transitionStartFrame),
                    SHENMUE_COPY_TYPED.defaultAnimations[0], 1, 16);
            SHENMUE_COPY_STATE.copyPhase = 3;
        }
        break;
    case 3:
        if (!(g_transitionWork->regionFlags[0] & MRF_Transition)) {
            *g_copyTransitionState = 6;
            finished = 1;
        }
        break;
    }
    return finished;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
inline std::int32_t CopyStepFrames() {
    return (SHENMUE_COPY_STATE.copyFlags & (MCF_Running | MCF_AlternateLocomotion)) ? 8 : 16;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
int CopyTurnStep(std::uint8_t towardPosition) {
    Vector3 position;
    Vector3 delta;
    Vector3i rotation;
    std::int16_t difference, moveYaw, faceYaw;
    bool running;
    std::int32_t frames;
    std::uint16_t motion;
    float rigScale, stride, scale, distance;
    std::int32_t loop = 1;
    do {
        switch (SHENMUE_COPY_STATE.copyPhase) {
        case 1:
        default:
            g_transitionWork->copyStepState = 0;
            g_transitionWork->copyScaleCountdown = 0;
            g_transitionWork->copyStepsRemaining = 0;
            GetCharacterWorldPosition(g_transitionCharacter, &position);
            GetCharacterWorldRotation(g_transitionCharacter, &rotation);
            if (towardPosition) {
                delta.x = SHENMUE_COPY_STATE.copySourcePosition.x - position.x;
                delta.y = 0.0f;
                delta.z = SHENMUE_COPY_STATE.copySourcePosition.z - position.z;
                const float magnitude = Vector3::CalcMagnitude(delta);
                SHENMUE_COPY_STATE.copyTravelYaw = magnitude > 1.1920929e-07f ? dc::sdk::ArcTan2Fixed(-delta.x, -delta.z) : rotation.y;
            }
            difference = static_cast<std::int16_t>(SHENMUE_COPY_STATE.copyTravelYaw - rotation.y);
            if (std::fabs(static_cast<float>(difference)) >= 4096.0f)
                SHENMUE_COPY_STATE.copyPhase = 3;
            else if (SHENMUE_COPY_STATE.insideRectangleScale < 1.0f)
                SHENMUE_COPY_STATE.copyPhase = 3;
            else {
                const std::uint8_t gait = ClassifyCurrentLocomotionPhase(g_transitionWork);
                if (!gait || (gait == 2 && !(SHENMUE_COPY_STATE.copyFlags & MCF_AlternateLocomotion)) ||
                    (gait == 1 && (SHENMUE_COPY_STATE.copyFlags & MCF_AlternateLocomotion)))
                    SHENMUE_COPY_STATE.copyPhase = 3;
                else {
                    SHENMUE_COPY_STATE.copyDelayFrames = (SHENMUE_COPY_STATE.copyFlags & MCF_Running) ? 3 : 6;
                    SHENMUE_COPY_STATE.copyDelayFrames =
                        static_cast<std::uint8_t>(SHENMUE_COPY_STATE.copyDelayFrames * (1.0f / SHENMUE_COPY_STATE.motionScale));
                    SHENMUE_COPY_STATE.copyPhase = 2;
                }
            }
            break;
        case 2:
            if (SHENMUE_COPY_STATE.copyFlags & MCF_LookSuppressed) {
                mtwkUpdateWithPosition(g_transitionWork, 0, 0);
                mtwkUpdateWithPosition(g_transitionWork, 0, 0x304);
            }
            if (SHENMUE_COPY_STATE.copyDelayFrames) {
                GetCharacterWorldRotation(g_transitionCharacter, &rotation);
                rotation.y += static_cast<std::int16_t>(SHENMUE_COPY_STATE.copyTravelYaw - rotation.y) / SHENMUE_COPY_STATE.copyDelayFrames;
                charaSetWorldRot(g_transitionCharacter, rotation);
                --SHENMUE_COPY_STATE.copyDelayFrames;
            } else {
                *g_copyTransitionState = 5;
                SHENMUE_COPY_STATE.copyPhase = 1;
            }
            loop = 0;
            break;
        case 3:
            CopyChooseFinalFoot(*g_transitionWork, rotation.y, 1);
            difference = static_cast<std::int16_t>(SHENMUE_COPY_STATE.copyTravelYaw - rotation.y);
            running = (SHENMUE_COPY_STATE.copyFlags & MCF_Running) != 0;
            frames = CopyStepFrames();
            rigScale = mtwkGetScaleFactorFor(g_transitionWork->rigBoneId, 8, 0);
            stride = rigScale * ((SHENMUE_COPY_STATE.copyFlags & MCF_Running) ? 1.2f : 0.55000001f);
            motion = (SHENMUE_COPY_STATE.copyFlags & MCF_AlternateLocomotion) ? SHENMUE_COPY_ANIMATION(4) : SHENMUE_COPY_ANIMATION(1);
            if ((SHENMUE_COPY_STATE.copyFlags & (MCF_LeadFoot | MCF_SampledLeadFoot)) == (MCF_LeadFoot | MCF_SampledLeadFoot) ||
                (SHENMUE_COPY_STATE.copyFlags & (MCF_LeadFoot | MCF_SampledLeadFoot)) == 0) {
                if (std::fabs(static_cast<float>(difference)) < 20480.0f) {
                    CopySetLeadFoot(*g_transitionWork, 1, motion, 0, running);
                    if (g_transitionHadMotion) {
                        scale = 1.0f - std::fabs(static_cast<float>(difference)) * 0.30000001f / 20480.0f;
                        position.x -= scale * stride * dc::sdk::SinFixed(SHENMUE_COPY_STATE.copyTravelYaw);
                        position.z -= scale * stride * dc::sdk::CosFixed(SHENMUE_COPY_STATE.copyTravelYaw);
                    }
                    CopyPlayAt(*g_transitionWork, position, SHENMUE_COPY_STATE.copyTravelYaw, motion, 1, frames);
                    if (towardPosition)
                        CopyAdjustStride(*g_transitionWork, position, SHENMUE_COPY_STATE.copyTravelYaw, motion, stride);
                    if (SHENMUE_COPY_STATE.copyFlags & MCF_LookSuppressed) {
                        mtwkUpdateWithPosition(g_transitionWork, 0, 0);
                        mtwkUpdateWithPosition(g_transitionWork, 0, 0x304);
                    }
                    *g_copyTransitionState = 5;
                    SHENMUE_COPY_STATE.copyPhase = 1;
                } else {
                    CopySetLeadFoot(*g_transitionWork, 1, motion, 0, running);
                    if (!running) {
                        if (SHENMUE_COPY_STATE.copyFlags & MCF_LeadFoot) {
                            moveYaw = SHENMUE_COPY_STATE.copyTravelYaw - 0x1000;
                            faceYaw = rotation.y - 0x5000;
                        } else {
                            moveYaw = SHENMUE_COPY_STATE.copyTravelYaw + 0x1000;
                            faceYaw = rotation.y + 0x5000;
                        }
                        if (g_transitionHadMotion) {
                            distance = stride * 0.5f;
                            position.x -= distance * dc::sdk::SinFixed(moveYaw);
                            position.z -= distance * dc::sdk::CosFixed(moveYaw);
                        }
                        CopyPlayAt(*g_transitionWork, position, faceYaw, motion, 1, frames);
                        SHENMUE_COPY_STATE.copyDelayFrames = static_cast<std::uint8_t>(static_cast<float>(frames) * 0.60000002f);
                        SHENMUE_COPY_STATE.copyDelayFrames =
                            static_cast<std::uint8_t>(SHENMUE_COPY_STATE.copyDelayFrames * (1.0f / SHENMUE_COPY_STATE.motionScale));
                        SHENMUE_COPY_STATE.copyPhase = 5;
                    } else {
                        if (g_transitionHadMotion) {
                            distance = stride * 0.5f;
                            position.x -= distance * dc::sdk::SinFixed(rotation.y);
                            position.z -= distance * dc::sdk::CosFixed(rotation.y);
                        }
                        CopyPlayAt(*g_transitionWork, position, rotation.y, motion, 1, frames);
                        SHENMUE_COPY_STATE.copyDelayFrames = static_cast<std::uint8_t>(static_cast<float>(frames) * 0.60000002f);
                        SHENMUE_COPY_STATE.copyDelayFrames =
                            static_cast<std::uint8_t>(SHENMUE_COPY_STATE.copyDelayFrames * (1.0f / SHENMUE_COPY_STATE.motionScale));
                        SHENMUE_COPY_STATE.copyPhase = 4;
                    }
                }
            } else {
                CopyToggleLeadFoot(*g_transitionWork);
                CopySetLeadFoot(*g_transitionWork, 1, motion, 0, running);
                if (std::fabs(static_cast<float>(difference)) < 8192.0f) {
                    if (g_transitionHadMotion) {
                        scale = 1.0f - std::fabs(static_cast<float>(difference)) * 0.30000001f / 8192.0f;
                        position.x -= scale * stride * dc::sdk::SinFixed(SHENMUE_COPY_STATE.copyTravelYaw);
                        position.z -= scale * stride * dc::sdk::CosFixed(SHENMUE_COPY_STATE.copyTravelYaw);
                    }
                    CopyPlayAt(*g_transitionWork, position, SHENMUE_COPY_STATE.copyTravelYaw, motion, 1, frames);
                    if (towardPosition)
                        CopyAdjustStride(*g_transitionWork, position, SHENMUE_COPY_STATE.copyTravelYaw, motion, stride);
                    if (SHENMUE_COPY_STATE.copyFlags & MCF_LookSuppressed) {
                        mtwkUpdateWithPosition(g_transitionWork, 0, 0);
                        mtwkUpdateWithPosition(g_transitionWork, 0, 0x304);
                    }
                    *g_copyTransitionState = 5;
                    SHENMUE_COPY_STATE.copyPhase = 1;
                } else {
                    if (SHENMUE_COPY_STATE.copyFlags & MCF_LeadFoot)
                        faceYaw = rotation.y + 0x2000;
                    else
                        faceYaw = rotation.y - 0x2000;
                    if (g_transitionHadMotion) {
                        distance = stride * 0.60000002f;
                        position.x -= distance * dc::sdk::SinFixed(rotation.y);
                        position.z -= distance * dc::sdk::CosFixed(rotation.y);
                    }
                    CopyPlayAt(*g_transitionWork, position, faceYaw, motion, 1, frames);
                    SHENMUE_COPY_STATE.copyDelayFrames = static_cast<std::uint8_t>(static_cast<float>(frames) * 0.60000002f);
                    SHENMUE_COPY_STATE.copyDelayFrames =
                        static_cast<std::uint8_t>(SHENMUE_COPY_STATE.copyDelayFrames * (1.0f / SHENMUE_COPY_STATE.motionScale));
                    SHENMUE_COPY_STATE.copyPhase = 5;
                }
            }
            loop = 0;
            break;
        case 4:
            if (SHENMUE_COPY_STATE.copyDelayFrames)
                --SHENMUE_COPY_STATE.copyDelayFrames;
            else {
                CopyToggleLeadFoot(*g_transitionWork);
                GetCharacterWorldPosition(g_transitionCharacter, &position);
                GetCharacterWorldRotation(g_transitionCharacter, &rotation);
                running = (SHENMUE_COPY_STATE.copyFlags & MCF_Running) != 0;
                frames = CopyStepFrames();
                rigScale = mtwkGetScaleFactorFor(g_transitionWork->rigBoneId, 8, 0);
                stride = rigScale * ((SHENMUE_COPY_STATE.copyFlags & MCF_Running) ? 1.2f : 0.55000001f);
                motion = (SHENMUE_COPY_STATE.copyFlags & MCF_AlternateLocomotion) ? SHENMUE_COPY_ANIMATION(4) : SHENMUE_COPY_ANIMATION(1);
                if (towardPosition) {
                    delta.x = SHENMUE_COPY_STATE.copySourcePosition.x - position.x;
                    delta.y = 0.0f;
                    delta.z = SHENMUE_COPY_STATE.copySourcePosition.z - position.z;
                    const float magnitude = Vector3::CalcMagnitude(delta);
                    SHENMUE_COPY_STATE.copyTravelYaw = magnitude > 1.1920929e-07f ? dc::sdk::ArcTan2Fixed(-delta.x, -delta.z) : rotation.y;
                }
                if (SHENMUE_COPY_STATE.copyFlags & MCF_LeadFoot) {
                    moveYaw = SHENMUE_COPY_STATE.copyTravelYaw + 0x1800;
                    faceYaw = rotation.y + 0x2000;
                } else {
                    moveYaw = SHENMUE_COPY_STATE.copyTravelYaw - 0x1800;
                    faceYaw = rotation.y - 0x2000;
                }
                CopySetLeadFoot(*g_transitionWork, 1, motion, 0, running);
                if (g_transitionHadMotion) {
                    distance = stride * 0.2f;
                    position.x -= distance * dc::sdk::SinFixed(moveYaw);
                    position.z -= distance * dc::sdk::CosFixed(moveYaw);
                }
                CopyPlayAt(*g_transitionWork, position, faceYaw, motion, 1, frames);
                SHENMUE_COPY_STATE.copyDelayFrames = static_cast<std::uint8_t>(static_cast<float>(frames) * 0.30000001f);
                SHENMUE_COPY_STATE.copyDelayFrames = static_cast<std::uint8_t>(SHENMUE_COPY_STATE.copyDelayFrames * (1.0f / SHENMUE_COPY_STATE.motionScale));
                SHENMUE_COPY_STATE.copyPhase = 5;
            }
            loop = 0;
            break;
        case 5:
            if (SHENMUE_COPY_STATE.copyDelayFrames)
                --SHENMUE_COPY_STATE.copyDelayFrames;
            else {
                CopyToggleLeadFoot(*g_transitionWork);
                GetCharacterWorldPosition(g_transitionCharacter, &position);
                GetCharacterWorldRotation(g_transitionCharacter, &rotation);
                difference = static_cast<std::int16_t>(SHENMUE_COPY_STATE.copyTravelYaw - rotation.y);
                running = (SHENMUE_COPY_STATE.copyFlags & MCF_Running) != 0;
                frames = CopyStepFrames();
                rigScale = mtwkGetScaleFactorFor(g_transitionWork->rigBoneId, 8, 0);
                stride = rigScale * ((SHENMUE_COPY_STATE.copyFlags & MCF_Running) ? 1.2f : 0.55000001f);
                motion = (SHENMUE_COPY_STATE.copyFlags & MCF_AlternateLocomotion) ? SHENMUE_COPY_ANIMATION(4) : SHENMUE_COPY_ANIMATION(1);
                if (towardPosition) {
                    delta.x = SHENMUE_COPY_STATE.copySourcePosition.x - position.x;
                    delta.y = 0.0f;
                    delta.z = SHENMUE_COPY_STATE.copySourcePosition.z - position.z;
                    const float magnitude = Vector3::CalcMagnitude(delta);
                    SHENMUE_COPY_STATE.copyTravelYaw = magnitude > 1.1920929e-07f ? dc::sdk::ArcTan2Fixed(-delta.x, -delta.z) : rotation.y;
                }
                CopySetLeadFoot(*g_transitionWork, 1, motion, 0, running);
                if (g_transitionHadMotion) {
                    scale = 1.0f - std::fabs(static_cast<float>(difference)) * 0.30000001f / 24576.0f;
                    position.x -= scale * stride * dc::sdk::SinFixed(SHENMUE_COPY_STATE.copyTravelYaw);
                    position.z -= scale * stride * dc::sdk::CosFixed(SHENMUE_COPY_STATE.copyTravelYaw);
                }
                CopyPlayAt(*g_transitionWork, position, SHENMUE_COPY_STATE.copyTravelYaw, motion, 1, frames);
                if (towardPosition)
                    CopyAdjustStride(*g_transitionWork, position, SHENMUE_COPY_STATE.copyTravelYaw, motion, stride);
                if (SHENMUE_COPY_STATE.copyFlags & MCF_LookSuppressed) {
                    mtwkUpdateWithPosition(g_transitionWork, 0, 0);
                    mtwkUpdateWithPosition(g_transitionWork, 0, 0x304);
                }
                *g_copyTransitionState = 5;
                SHENMUE_COPY_STATE.copyPhase = 1;
            }
            loop = 0;
            break;
        }
    } while (loop);
    return false;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
inline std::int32_t CopyStrideFrames() {
    return (SHENMUE_COPY_TYPED.copyFlags & MCF_Running) ? 8 : 16;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t CopyTranslateStep() {
    Vector3 position;
    Vector3 delta;
    Vector3i rotation;
    std::int16_t difference, yaw;
    bool running;
    std::int32_t frames;
    std::uint16_t motion;
    float rigScale, stride, distance;
    switch (SHENMUE_COPY_TYPED.copyPhase) {
    case 1:
    default:
        GetCharacterWorldPosition(g_transitionCharacter, &position);
        GetCharacterWorldRotation(g_transitionCharacter, &rotation);
        SHENMUE_COPY_TYPED.transitionStartFrame = static_cast<std::int16_t>(rotation.y);
        delta.x = SHENMUE_COPY_TYPED.copySourcePosition.x - position.x;
        delta.y = 0.0f;
        delta.z = SHENMUE_COPY_TYPED.copySourcePosition.z - position.z;
        distance = Vector3::CalcMagnitude(delta);
        SHENMUE_COPY_TYPED.copyTravelYaw = distance > 1.1920929e-07f ? dc::sdk::ArcTan2Fixed(-delta.x, -delta.z) : static_cast<std::int16_t>(rotation.y);
        CopyChooseFinalFoot(*g_transitionWork, rotation.y, 1);
        rigScale = mtwkGetScaleFactorFor(g_transitionWork->rigBoneId, 8, 0);
        stride = rigScale * ((SHENMUE_COPY_TYPED.copyFlags & MCF_Running) ? 1.2f : 0.55000001f);
        if ((SHENMUE_COPY_TYPED.copyFlags & (MCF_LeadFoot | MCF_SampledLeadFoot)) == (MCF_LeadFoot | MCF_SampledLeadFoot) ||
            !(SHENMUE_COPY_TYPED.copyFlags & (MCF_LeadFoot | MCF_SampledLeadFoot))) {
            frames = CopyStrideFrames();
            running = (SHENMUE_COPY_TYPED.copyFlags & MCF_Running) != 0;
            motion =
                (SHENMUE_COPY_TYPED.copyFlags & MCF_AlternateLocomotion) ? SHENMUE_COPY_TYPED.defaultAnimations[4] : SHENMUE_COPY_TYPED.defaultAnimations[1];
            CopySetLeadFoot(*g_transitionWork, 1, motion, 0, running);
            difference = static_cast<std::int16_t>(SHENMUE_COPY_TYPED.copyTravelYaw - static_cast<std::int16_t>(rotation.y));
            if (difference > 0x5000 || difference < -0x5000) {
                if (distance > stride) {
                    position.x += delta.x * (stride / distance);
                    position.z += delta.z * (stride / distance);
                } else {
                    position.x += delta.x;
                    position.z += delta.z;
                }
                yaw = static_cast<std::int16_t>(rotation.y);
                SHENMUE_COPY_TYPED.copyOperationState = 5;
                SHENMUE_COPY_TYPED.copyPhase = 1;
            } else if (distance > stride) {
                if (distance > stride + stride) {
                    position.x += delta.x * (stride / distance);
                    position.z += delta.z * (stride / distance);
                } else {
                    position.x += delta.x * 0.5f;
                    position.z += delta.z * 0.5f;
                }
                yaw = SHENMUE_COPY_TYPED.copyTravelYaw;
                SHENMUE_COPY_TYPED.copyDelayFrames = static_cast<std::uint8_t>(static_cast<float>(frames) * 0.60000002f);
                SHENMUE_COPY_TYPED.copyDelayFrames = static_cast<std::uint8_t>(SHENMUE_COPY_TYPED.copyDelayFrames * (1.0f / SHENMUE_COPY_TYPED.motionScale));
                SHENMUE_COPY_TYPED.copyFlags = SHENMUE_COPY_TYPED.copyFlags | MCF_TurnStepPending;
                motion = SHENMUE_COPY_TYPED.defaultAnimations[1];
                SHENMUE_COPY_TYPED.copyPhase = 3;
            } else {
                position.x += delta.x;
                position.z += delta.z;
                yaw = static_cast<std::int16_t>(rotation.y);
                SHENMUE_COPY_TYPED.copyOperationState = 5;
                SHENMUE_COPY_TYPED.copyPhase = 1;
            }
            CopyPlayAt(*g_transitionWork, position, yaw, motion, 1, frames);
            if (SHENMUE_COPY_TYPED.copyFlags & MCF_LookSuppressed) {
                mtwkUpdateWithPosition(g_transitionWork, 0, 0);
                mtwkUpdateWithPosition(g_transitionWork, 0, 0x304);
            }
            return false;
        }
        CopyToggleLeadFoot(*g_transitionWork);
        running = false;
        motion = SHENMUE_COPY_TYPED.defaultAnimations[1];
        frames = 16;
        CopySetLeadFoot(*g_transitionWork, 1, motion, 0, running);
        difference = static_cast<std::int16_t>(SHENMUE_COPY_TYPED.copyTravelYaw - static_cast<std::int16_t>(rotation.y));
        if (difference > 0x5000 || difference < -0x5000) {
            yaw = static_cast<std::int16_t>(rotation.y);
            SHENMUE_COPY_TYPED.copyPhase = 3;
        } else if (distance > stride) {
            position.x -= 0.2f * dc::sdk::SinFixed(rotation.y);
            position.z -= 0.2f * dc::sdk::CosFixed(rotation.y);
            yaw = SHENMUE_COPY_TYPED.copyTravelYaw;
            SHENMUE_COPY_TYPED.copyPhase = 2;
        } else {
            yaw = static_cast<std::int16_t>(rotation.y);
            SHENMUE_COPY_TYPED.copyPhase = 3;
        }
        CopyPlayAt(*g_transitionWork, position, yaw, motion, 1, frames);
        SHENMUE_COPY_TYPED.copyDelayFrames = static_cast<std::uint8_t>(static_cast<float>(frames) * 0.60000002f);
        SHENMUE_COPY_TYPED.copyDelayFrames = static_cast<std::uint8_t>(SHENMUE_COPY_TYPED.copyDelayFrames * (1.0f / SHENMUE_COPY_TYPED.motionScale));
        break;
    case 2:
        if (SHENMUE_COPY_TYPED.copyDelayFrames) {
            --SHENMUE_COPY_TYPED.copyDelayFrames;
            break;
        }
        GetCharacterWorldPosition(g_transitionCharacter, &position);
        delta.x = SHENMUE_COPY_TYPED.copySourcePosition.x - position.x;
        delta.y = 0.0f;
        delta.z = SHENMUE_COPY_TYPED.copySourcePosition.z - position.z;
        distance = Vector3::CalcMagnitude(delta);
        CopyToggleLeadFoot(*g_transitionWork);
        rigScale = mtwkGetScaleFactorFor(g_transitionWork->rigBoneId, 8, 0);
        stride = rigScale * ((SHENMUE_COPY_TYPED.copyFlags & MCF_Running) ? 1.2f : 0.55000001f);
        frames = CopyStrideFrames();
        running = false;
        motion = SHENMUE_COPY_TYPED.defaultAnimations[1];
        CopySetLeadFoot(*g_transitionWork, 1, motion, 0, running);
        if (distance > stride + stride) {
            position.x += delta.x * (stride / distance);
            position.z += delta.z * (stride / distance);
        } else {
            position.x += delta.x * 0.5f;
            position.z += delta.z * 0.5f;
        }
        SHENMUE_COPY_TYPED.copyDelayFrames = static_cast<std::uint8_t>(static_cast<float>(frames) * 0.60000002f);
        SHENMUE_COPY_TYPED.copyDelayFrames = static_cast<std::uint8_t>(SHENMUE_COPY_TYPED.copyDelayFrames * (1.0f / SHENMUE_COPY_TYPED.motionScale));
        SHENMUE_COPY_TYPED.copyFlags = SHENMUE_COPY_TYPED.copyFlags | MCF_TurnStepPending;
        CopyPlayAt(*g_transitionWork, position, SHENMUE_COPY_TYPED.copyTravelYaw, motion, 1, frames);
        SHENMUE_COPY_TYPED.copyPhase = 3;
        break;
    case 3:
        if (SHENMUE_COPY_TYPED.copyDelayFrames) {
            --SHENMUE_COPY_TYPED.copyDelayFrames;
            break;
        }
        GetCharacterWorldPosition(g_transitionCharacter, &position);
        delta.x = SHENMUE_COPY_TYPED.copySourcePosition.x - position.x;
        delta.y = 0.0f;
        delta.z = SHENMUE_COPY_TYPED.copySourcePosition.z - position.z;
        distance = Vector3::CalcMagnitude(delta);
        CopyToggleLeadFoot(*g_transitionWork);
        rigScale = mtwkGetScaleFactorFor(g_transitionWork->rigBoneId, 8, 0);
        stride = rigScale * ((SHENMUE_COPY_TYPED.copyFlags & MCF_Running) ? 1.2f : 0.55000001f);
        running = (SHENMUE_COPY_TYPED.copyFlags & MCF_Running) != 0;
        frames = CopyStrideFrames();
        motion = (SHENMUE_COPY_TYPED.copyFlags & MCF_AlternateLocomotion) ? SHENMUE_COPY_TYPED.defaultAnimations[4] : SHENMUE_COPY_TYPED.defaultAnimations[1];
        CopySetLeadFoot(*g_transitionWork, 1, motion, 0, running);
        if (distance > stride) {
            distance = stride / distance;
            delta.x *= distance;
            delta.z *= distance;
        }
        if (SHENMUE_COPY_TYPED.copyFlags & MCF_TurnStepPending) {
            delta.x -= 0.2f * dc::sdk::SinFixed(static_cast<std::int16_t>(SHENMUE_COPY_TYPED.transitionStartFrame));
            delta.z -= 0.2f * dc::sdk::CosFixed(static_cast<std::int16_t>(SHENMUE_COPY_TYPED.transitionStartFrame));
        }
        distance = Vector3::CalcMagnitude(delta);
        if (distance > stride) {
            distance = stride / distance;
            position.x += delta.x * distance;
            position.z += delta.z * distance;
        } else {
            position.x += delta.x;
            position.z += delta.z;
        }
        CopyPlayAt(*g_transitionWork, position, static_cast<std::int16_t>(SHENMUE_COPY_TYPED.transitionStartFrame), motion, 1, frames);
        if (SHENMUE_COPY_TYPED.copyFlags & MCF_LookSuppressed) {
            mtwkUpdateWithPosition(g_transitionWork, 0, 0);
            mtwkUpdateWithPosition(g_transitionWork, 0, 0x304);
        }
        SHENMUE_COPY_TYPED.copyFlags = SHENMUE_COPY_TYPED.copyFlags & ~MCF_TurnStepPending;
        SHENMUE_COPY_TYPED.copyOperationState = 5;
        SHENMUE_COPY_TYPED.copyPhase = 1;
        break;
    }
    return false;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void CopyLocalOffset(std::int32_t yaw, const Vector3 &offset, Vector3 &local) {
    matrix_stack::PushUnitMatrix();
    Matrix4x4::RotateAroundY_Wrapper(-yaw);
    Matrix4x4::TransformPoint(offset, local);
    PopCurrentMatrix(1);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
int CopyContinueStep(std::uint8_t stopAtYaw, std::uint8_t recheck) {
    Vector3 position;
    Vector3 offset;
    Vector3 delta;
    Vector3i rotation;
    float distance, stride, rigScale;
    std::int32_t frames;
    std::int32_t loop = 1;
    bool result = false;
    do {
        switch (SHENMUE_COPY_TYPED.copyPhase) {
        case 1:
        default:
            if (!(g_transitionWork->regionFlags[0] & MRF_Transition)) {
                if ((SHENMUE_COPY_TYPED.copyFlags & (MCF_ExtraStepRequested | MCF_ExtraStepComplete)) == (MCF_ExtraStepRequested | MCF_ExtraStepComplete))
                    SHENMUE_COPY_TYPED.copyPhase = 3;
                else
                    SHENMUE_COPY_TYPED.copyPhase = 2;
                break;
            }
            do {
                if (!(g_transitionWork->motionFlags & 0x8000U))
                    CopyRequestFootEvent(*g_transitionWork);
            } while (0);
            if (CopyReachedStepWindow(*g_transitionWork,
                    (SHENMUE_COPY_TYPED.copyFlags & MCF_Running) ? SHENMUE_COPY_TYPED.defaultAnimations[4] : SHENMUE_COPY_TYPED.defaultAnimations[1])) {
                SHENMUE_COPY_TYPED.copyPhase = 4;
                result = true;
            }
            loop = 0;
            break;
        case 2:
            if (SHENMUE_COPY_TYPED.copyScaleCountdown)
                CopyAdvanceScale(*g_transitionWork);
            if (CopyReachedStepWindow(*g_transitionWork,
                    (SHENMUE_COPY_TYPED.copyFlags & MCF_Running) ? SHENMUE_COPY_TYPED.defaultAnimations[4] : SHENMUE_COPY_TYPED.defaultAnimations[1])) {
                SHENMUE_COPY_TYPED.copyPhase = 4;
                result = true;
            }
            loop = 0;
            break;
        case 3:
            GetCharacterWorldPosition(g_transitionCharacter, &position);
            GetCharacterWorldRotation(g_transitionCharacter, &rotation);
            offset.x = SHENMUE_COPY_TYPED.copySplinePosition.x - position.x;
            offset.y = 0.0f;
            offset.z = SHENMUE_COPY_TYPED.copySplinePosition.z - position.z;
            CopyLocalOffset(rotation.y, offset, delta);
            if (delta.z > 0.050000001f) {
                if (SHENMUE_COPY_TYPED.copyFlags & MCF_Running)
                    SHENMUE_COPY_TYPED.copySplineFrame += 4;
                else
                    SHENMUE_COPY_TYPED.copySplineFrame += 2;
                if (SHENMUE_COPY_TYPED.copySplineFrame > 123) {
                    SHENMUE_COPY_TYPED.copyFlags &= ~MCF_ExtraStepRequested;
                    SHENMUE_COPY_TYPED.copyPhase = 2;
                } else {
                    delta = SHENMUE_COPY_TYPED.copySplinePosition;
                    InterpolateNpcSplineNormalized(
                        SHENMUE_COPY_TYPED.copySplinePoints, 5, SHENMUE_COPY_TYPED.copySplineFrame, &SHENMUE_COPY_TYPED.copySplinePosition);
                    delta.x = SHENMUE_COPY_TYPED.copySplinePosition.x - delta.x;
                    delta.z = SHENMUE_COPY_TYPED.copySplinePosition.z - delta.z;
                    if (!(std::fabs(delta.x) < 0.001f) || !(std::fabs(delta.z) < 0.001f)) {
                        rotation.y = dc::sdk::ArcTan2Fixed(-delta.x, -delta.z);
                        charaSetWorldRot(g_transitionCharacter, rotation);
                    }
                }
            }
            if (CopyReachedStepWindow(*g_transitionWork,
                    (SHENMUE_COPY_TYPED.copyFlags & MCF_Running) ? SHENMUE_COPY_TYPED.defaultAnimations[4] : SHENMUE_COPY_TYPED.defaultAnimations[1])) {
                SHENMUE_COPY_TYPED.copyPhase = 4;
                result = true;
            }
            loop = 0;
            break;
        case 4:
            GetCharacterWorldPosition(g_transitionCharacter, &position);
            GetCharacterWorldRotation(g_transitionCharacter, &rotation);
            offset.x = SHENMUE_COPY_TYPED.copySourcePosition.x - position.x;
            offset.y = 0.0f;
            offset.z = SHENMUE_COPY_TYPED.copySourcePosition.z - position.z;
            distance = Vector3::CalcMagnitude(offset);
            rigScale = mtwkGetScaleFactorFor(g_transitionWork->rigBoneId, 8, 0);
            stride = rigScale * ((SHENMUE_COPY_TYPED.copyFlags & MCF_Running) ? 1.2f : 0.55000001f);
            SHENMUE_COPY_TYPED.copyPhase = 1;
            if (SHENMUE_COPY_TYPED.copyStepState == 2) {
                CopyChooseFinalFoot(*g_transitionWork, rotation.y, 2);
                CopySetLeadFoot(*g_transitionWork, 1,
                    (SHENMUE_COPY_TYPED.copyFlags & MCF_AlternateLocomotion) ? SHENMUE_COPY_TYPED.defaultAnimations[4] :
                                                                               SHENMUE_COPY_TYPED.defaultAnimations[1],
                    0, (SHENMUE_COPY_TYPED.copyFlags & MCF_Running) != 0);
                position.x -= dc::sdk::SinFixed(rotation.y) * SHENMUE_COPY_TYPED.copyExtraStepDistance;
                position.z -= dc::sdk::CosFixed(rotation.y) * SHENMUE_COPY_TYPED.copyExtraStepDistance;
                CopyPlayAt(*g_transitionWork, position, rotation.y,
                    (SHENMUE_COPY_TYPED.copyFlags & MCF_AlternateLocomotion) ? SHENMUE_COPY_TYPED.defaultAnimations[4] :
                                                                               SHENMUE_COPY_TYPED.defaultAnimations[1],
                    1, (SHENMUE_COPY_TYPED.copyFlags & MCF_Running) ? 5 : 11);
                if (SHENMUE_COPY_TYPED.insideRectangleScale < 1.0f) {
                    mtwkSetRigScale(g_transitionWork, SHENMUE_COPY_TYPED.insideRectangleScale);
                    mtwkSetSpeed(g_transitionWork, (1.4f - SHENMUE_COPY_TYPED.insideRectangleScale * 0.40000001f) * SHENMUE_COPY_TYPED.motionScale, LowerBody);
                }
                SHENMUE_COPY_TYPED.copyStepState = 1;
                loop = 0;
                break;
            }
            if (!SHENMUE_COPY_TYPED.copyStepState)
                CopyAdjustStride(*g_transitionWork, position, rotation.y, g_transitionWork->currentMotionId[0], stride);
            else if (SHENMUE_COPY_TYPED.copyStepsRemaining && !(g_transitionWork->regionFlags[0] & MRF_Transition))
                --SHENMUE_COPY_TYPED.copyStepsRemaining;
            if ((SHENMUE_COPY_TYPED.copyFlags & (MCF_ExtraStepRequested | MCF_ExtraStepComplete)) == (MCF_ExtraStepRequested | MCF_ExtraStepComplete)) {
                loop = 0;
                break;
            }
            if (stride > distance && !SHENMUE_COPY_TYPED.copyStepsRemaining) {
                if (!(g_transitionWork->sequenceFlags & MSF_LookAt) && !(SHENMUE_COPY_TYPED.copyFlags & MCF_LookSuppressed))
                    CopyUpdateLook(false);
                if (stopAtYaw) {
                    SHENMUE_COPY_TYPED.copyOperationState = 4;
                    SHENMUE_COPY_TYPED.copyPhase = 1;
                    SHENMUE_COPY_TYPED.copyTravelYaw = SHENMUE_COPY_TYPED.transitionStartFrame;
                    result = true;
                    loop = 0;
                    break;
                }
                SHENMUE_COPY_TYPED.copyTravelYaw = distance > 1.1920929e-07f ? dc::sdk::ArcTan2Fixed(-offset.x, -offset.z) : rotation.y;
                CopyChooseFinalFoot(*g_transitionWork, rotation.y, 0);
                if ((SHENMUE_COPY_TYPED.copyFlags & (MCF_LeadFoot | MCF_SampledLeadFoot)) == (MCF_LeadFoot | MCF_SampledLeadFoot) ||
                    !(SHENMUE_COPY_TYPED.copyFlags & (MCF_LeadFoot | MCF_SampledLeadFoot))) {
                    SHENMUE_COPY_TYPED.copyDelayFrames = 0;
                    SHENMUE_COPY_TYPED.copyOperationState = 2;
                    SHENMUE_COPY_TYPED.copyPhase = 1;
                } else {
                    CopyToggleLeadFoot(*g_transitionWork);
                    CopySetLeadFoot(*g_transitionWork, 1, 0, 0, 0);
                    position.x = (position.x + SHENMUE_COPY_TYPED.copySourcePosition.x) * 0.5f;
                    position.z = (position.z + SHENMUE_COPY_TYPED.copySourcePosition.z) * 0.5f;
                    frames = (SHENMUE_COPY_TYPED.copyFlags & MCF_Running) ? 8 : 16;
                    CopyPlayAt(*g_transitionWork, position, rotation.y, SHENMUE_COPY_TYPED.defaultAnimations[0], 1, frames);
                    SHENMUE_COPY_TYPED.copyDelayFrames = frames - 1;
                    SHENMUE_COPY_TYPED.copyDelayFrames =
                        static_cast<std::uint8_t>(SHENMUE_COPY_TYPED.copyDelayFrames * (1.0f / SHENMUE_COPY_TYPED.motionScale));
                    CopyToggleLeadFoot(*g_transitionWork);
                    SHENMUE_COPY_TYPED.copyOperationState = 2;
                    SHENMUE_COPY_TYPED.copyPhase = 1;
                }
                result = true;
                loop = 0;
                break;
            }
            if (!recheck) {
                CopyLocalOffset(rotation.y, offset, delta);
                if (delta.z > 0.2f) {
                    if (stopAtYaw) {
                        SHENMUE_COPY_TYPED.copyOperationState = 4;
                        SHENMUE_COPY_TYPED.copyPhase = 1;
                        SHENMUE_COPY_TYPED.copyTravelYaw = SHENMUE_COPY_TYPED.transitionStartFrame;
                        result = true;
                        loop = 0;
                        break;
                    }
                    CopyChooseFinalFoot(*g_transitionWork, rotation.y, 2);
                    CopySetLeadFoot(*g_transitionWork, 1, 0, 0, 0);
                    CopyPlayAt(*g_transitionWork, position, rotation.y, SHENMUE_COPY_TYPED.defaultAnimations[0], 1,
                        (SHENMUE_COPY_TYPED.copyFlags & MCF_Running) ? 8 : 16);
                    SHENMUE_COPY_TYPED.copyDelayFrames = 0;
                    SHENMUE_COPY_TYPED.copyOperationState = 2;
                    SHENMUE_COPY_TYPED.copyPhase = 3;
                    result = true;
                }
            }
            loop = 0;
            break;
        }
    } while (loop);
    return result;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
int CopyCanStart() {
    if (SHENMUE_COPY_TYPED.copyFlags & 0x10U) {
        Vector3 position, delta;
        Vector3i rotation;
        GetCharacterWorldPosition(g_transitionCharacter, &position);
        GetCharacterWorldRotation(g_transitionCharacter, &rotation);
        const std::int16_t yaw = static_cast<std::int16_t>(SHENMUE_COPY_TYPED.transitionStartFrame - static_cast<std::int16_t>(rotation.y));
        delta.x = SHENMUE_COPY_TYPED.copySourcePosition.x - position.x;
        delta.y = 0.0f;
        delta.z = SHENMUE_COPY_TYPED.copySourcePosition.z - position.z;
        const float distance = Vector3::CalcMagnitude(delta);
        switch (SHENMUE_COPY_TYPED.transitionSubstate) {
        case 0:
            if (182.0f > std::fabs(static_cast<float>(yaw)) && 0.0049999999f > distance) {
                CommitCopyMotionTransition(g_transitionWork);
                return false;
            }
            break;
        case 1:
            if (0.0049999999f > distance) {
                CommitCopyMotionTransition(g_transitionWork);
                return false;
            }
            break;
        case 2:
            if (182.0f > std::fabs(static_cast<float>(yaw))) {
                CommitCopyMotionTransition(g_transitionWork);
                return false;
            }
            break;
        }
    }
    return true;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool CopyRetryUnlessArrived() {
    if (SHENMUE_COPY_STATE.copyFlags & MCF_LookPending) {
        SHENMUE_COPY_STATE.copyOperationState = 1;
        return true;
    }
    Vector3 position;
    GetCharacterWorldPosition(g_transitionCharacter, &position);
    position.x = SHENMUE_COPY_STATE.copySourcePosition.x - position.x;
    position.y = 0.0f;
    position.z = SHENMUE_COPY_STATE.copySourcePosition.z - position.z;
    if (Vector3::CalcMagnitude(position) > 0.1f) {
        SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) |= MCF_Persistent;
        if (!(SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) & 0x10U)) {
            SHENMUE_COPY_STATE.copyOperationState = 1;
            return true;
        }
    }
    return false;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t UpdateCopyToPosition() {
    std::uint8_t next = 1;
    bool result = false;
    Vector3 position;
    Vector3i rotation;
    do {
        switch (SHENMUE_COPY_FIELD(std::uint8_t, copyOperationState)) {
        case 6:
        default:
            if (!CopyRetryUnlessArrived()) {
                CommitCopyMotionTransition(g_transitionWork);
                next = 0;
            }
            break;
        case 0:
        case 1:
            if (SHENMUE_COPY_FIELD(std::uint8_t, transitionVariant) != SHENMUE_COPY_FIELD(std::uint8_t, transitionClass)) {
                SHENMUE_COPY_FIELD(std::uint8_t, transitionClass) = SHENMUE_COPY_FIELD(std::uint8_t, transitionVariant);
                result = true;
                next = 0;
                break;
            }
            if (SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) & MCF_LookPending) {
                SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) &= ~MCF_LookPending;
                SHENMUE_COPY_FIELD(std::uint16_t, transitionStartFrame) = SHENMUE_COPY_FIELD(std::uint16_t, transitionEndFrame);
                SHENMUE_COPY_VECTOR(copySourcePosition) = SHENMUE_COPY_VECTOR(copyDestinationPosition);
                if (SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) & MCF_Unknown20000) {
                    GetCharacterWorldPosition(g_transitionCharacter, &position);
                    SHENMUE_COPY_VECTOR(copySourcePosition).x += position.x;
                    SHENMUE_COPY_VECTOR(copySourcePosition).y += position.y;
                    SHENMUE_COPY_VECTOR(copySourcePosition).z += position.z;
                }
                SHENMUE_COPY_FIELD(std::uint8_t, copyDelayFrames) = 0;
            }
            next = CopyCanStart();
            if (!next)
                break;
            next = CopyUpdateLook(false);
            if (!next)
                break;
            switch (SHENMUE_COPY_FIELD(std::uint8_t, transitionSubstate)) {
            case 2:
                GetCharacterWorldPosition(g_transitionCharacter, &position);
                SHENMUE_COPY_VECTOR(copySourcePosition) = position;
                break;
            case 1:
                GetCharacterWorldRotation(g_transitionCharacter, &rotation);
                SHENMUE_COPY_FIELD(std::uint16_t, transitionStartFrame) = rotation.y;
                break;
            case 0:
                CopyBuildSpline(true);
                break;
            }
            if ((SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) & (MCF_ExtraStepRequested | MCF_ExtraStepComplete)) ==
                (MCF_ExtraStepRequested | MCF_ExtraStepComplete)) {
                GetCharacterWorldRotation(g_transitionCharacter, &rotation);
                SHENMUE_COPY_FIELD(std::int16_t, copyTravelYaw) = rotation.y;
                next = CopyBeginStep(true);
            } else {
                next = CopyBeginStep(false);
            }
            break;
        case 2:
            next = CopyFinishStep(false);
            break;
        case 4:
            next = CopyTurnStep(true);
            break;
        case 5:
            if (SHENMUE_COPY_FIELD(std::uint8_t, transitionVariant) != SHENMUE_COPY_FIELD(std::uint8_t, transitionClass)) {
                SHENMUE_COPY_FIELD(std::uint8_t, transitionClass) = SHENMUE_COPY_FIELD(std::uint8_t, transitionVariant);
                result = true;
                next = 0;
                break;
            }
            next = CopyContinueStep(false, false);
            if (!next)
                break;
            if (SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) & MCF_LookPending) {
                SHENMUE_COPY_FIELD(std::uint16_t, transitionStartFrame) = SHENMUE_COPY_FIELD(std::uint16_t, transitionEndFrame);
                SHENMUE_COPY_VECTOR(copySourcePosition) = SHENMUE_COPY_VECTOR(copyDestinationPosition);
                if (SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) & MCF_Unknown20000) {
                    GetCharacterWorldPosition(g_transitionCharacter, &position);
                    SHENMUE_COPY_VECTOR(copySourcePosition).x += position.x;
                    SHENMUE_COPY_VECTOR(copySourcePosition).y += position.y;
                    SHENMUE_COPY_VECTOR(copySourcePosition).z += position.z;
                }
            }
            switch (SHENMUE_COPY_FIELD(std::uint8_t, transitionSubstate)) {
            case 2:
                GetCharacterWorldPosition(g_transitionCharacter, &position);
                SHENMUE_COPY_VECTOR(copySourcePosition) = position;
                SHENMUE_COPY_FIELD(std::uint8_t, copyStepsRemaining) = 0;
                break;
            case 1:
                GetCharacterWorldRotation(g_transitionCharacter, &rotation);
                SHENMUE_COPY_FIELD(std::uint16_t, transitionStartFrame) = rotation.y;
                break;
            case 0:
                if (SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) & MCF_LookPending)
                    CopyBuildSpline(true);
                break;
            }
            if (SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) & MCF_LookPending) {
                SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) &= ~MCF_LookPending;
                if ((SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) & (MCF_ExtraStepRequested | MCF_ExtraStepComplete)) ==
                    (MCF_ExtraStepRequested | MCF_ExtraStepComplete)) {
                    SHENMUE_COPY_FIELD(std::uint8_t, copyPhase) = 1;
                    next = 0;
                    break;
                }
                next = CopyContinueStep(false, true);
                if (next)
                    break;
                GetCharacterWorldPosition(g_transitionCharacter, &position);
                SHENMUE_COPY_FIELD(std::int16_t, copyTravelYaw) = static_cast<std::int16_t>(
                    dc::sdk::ArcTan2Fixed(position.x - SHENMUE_COPY_VECTOR(copySourcePosition).x, position.z - SHENMUE_COPY_VECTOR(copySourcePosition).z));
                SHENMUE_COPY_FIELD(std::uint8_t, copyOperationState) = 4;
                SHENMUE_COPY_FIELD(std::uint8_t, copyPhase) = 1;
                next = 1;
            } else {
                next = CopyContinueStep(false, false);
            }
            break;
        }
    } while (next);
    return result;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t UpdateCopyAlongHeading() {
    std::uint8_t next = 1;
    bool result = false;
    Vector3 position;
    do {
        switch (SHENMUE_COPY_FIELD(std::uint8_t, copyOperationState)) {
        case 6:
        default:
            CommitCopyMotionTransition(g_transitionWork);
            next = 0;
            break;
        case 0:
        case 1:
            if (SHENMUE_COPY_FIELD(std::uint8_t, transitionVariant) != SHENMUE_COPY_FIELD(std::uint8_t, transitionClass)) {
                SHENMUE_COPY_FIELD(std::uint8_t, transitionClass) = SHENMUE_COPY_FIELD(std::uint8_t, transitionVariant);
                result = true;
                next = 0;
                break;
            }
            if (SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) & MCF_LookPending) {
                SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) &= ~MCF_LookPending;
                SHENMUE_COPY_FIELD(std::uint16_t, transitionStartFrame) = SHENMUE_COPY_FIELD(std::uint16_t, transitionEndFrame);
                SHENMUE_COPY_VECTOR(copySourcePosition) = SHENMUE_COPY_VECTOR(copyDestinationPosition);
                if (SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) & MCF_Unknown20000) {
                    GetCharacterWorldPosition(g_transitionCharacter, &position);
                    SHENMUE_COPY_VECTOR(copySourcePosition).x += position.x;
                    SHENMUE_COPY_VECTOR(copySourcePosition).y += position.y;
                    SHENMUE_COPY_VECTOR(copySourcePosition).z += position.z;
                }
                if (!SHENMUE_COPY_FIELD(std::uint8_t, transitionSubstate))
                    SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) |= MCF_Unknown80;
                else
                    SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) &= ~MCF_Unknown80;
            }
            next = CopyUpdateLook(false);
            if (!next)
                break;
            switch (SHENMUE_COPY_FIELD(std::uint8_t, transitionSubstate)) {
            case 0:
                CopyBuildSpline(false);
                next = CopyBeginStep(false);
                break;
            case 2:
                SHENMUE_COPY_FIELD(std::int16_t, copyTravelYaw) = SHENMUE_COPY_FIELD(std::uint16_t, transitionStartFrame);
                next = CopyBeginStep(true);
                break;
            case 1:
                GetCharacterWorldPosition(g_transitionCharacter, &position);
                SHENMUE_COPY_FIELD(std::int16_t, copyTravelYaw) = static_cast<std::int16_t>(
                    dc::sdk::ArcTan2Fixed(position.x - SHENMUE_COPY_VECTOR(copySourcePosition).x, position.z - SHENMUE_COPY_VECTOR(copySourcePosition).z));
                next = CopyBeginStep(true);
                break;
            }
            break;
        case 2:
            next = CopyFinishStep(true);
            if (SHENMUE_COPY_FIELD(std::uint8_t, copyOperationState) != 2U)
                SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) &= ~MCF_Unknown80;
            break;
        case 4:
            if (SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) & MCF_Unknown80)
                next = CopyTurnStep(1);
            else
                next = CopyTurnStep(0);
            break;
        case 5:
            if (SHENMUE_COPY_FIELD(std::uint8_t, transitionVariant) != SHENMUE_COPY_FIELD(std::uint8_t, transitionClass)) {
                SHENMUE_COPY_FIELD(std::uint8_t, transitionClass) = SHENMUE_COPY_FIELD(std::uint8_t, transitionVariant);
                result = true;
                next = 0;
                break;
            }
            if (!(SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) & (MCF_Unknown80 | MCF_LookPending)) && !(g_transitionWork->regionFlags[0] & MRF_Transition)) {
                SHENMUE_COPY_FIELD(std::uint8_t, copyOperationState) = 6;
                break;
            }
            next = CopyContinueStep(true, false);
            if (!next)
                break;
            if (SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) & MCF_LookPending) {
                SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) &= ~MCF_LookPending;
                SHENMUE_COPY_FIELD(std::uint16_t, transitionStartFrame) = SHENMUE_COPY_FIELD(std::uint16_t, transitionEndFrame);
                SHENMUE_COPY_VECTOR(copySourcePosition) = SHENMUE_COPY_VECTOR(copyDestinationPosition);
                if (SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) & MCF_Unknown20000) {
                    GetCharacterWorldPosition(g_transitionCharacter, &position);
                    SHENMUE_COPY_VECTOR(copySourcePosition).x += position.x;
                    SHENMUE_COPY_VECTOR(copySourcePosition).y += position.y;
                    SHENMUE_COPY_VECTOR(copySourcePosition).z += position.z;
                }
                if (!SHENMUE_COPY_FIELD(std::uint8_t, transitionSubstate))
                    SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) |= MCF_Unknown80;
                else
                    SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) &= ~MCF_Unknown80;
                switch (SHENMUE_COPY_FIELD(std::uint8_t, transitionSubstate)) {
                case 2:
                    SHENMUE_COPY_FIELD(std::int16_t, copyTravelYaw) = SHENMUE_COPY_FIELD(std::uint16_t, transitionStartFrame);
                    SHENMUE_COPY_FIELD(std::uint8_t, copyOperationState) = 4;
                    SHENMUE_COPY_FIELD(std::uint8_t, copyPhase) = 1;
                    break;
                case 1:
                    GetCharacterWorldPosition(g_transitionCharacter, &position);
                    SHENMUE_COPY_FIELD(std::int16_t, copyTravelYaw) = static_cast<std::int16_t>(
                        dc::sdk::ArcTan2Fixed(position.x - SHENMUE_COPY_VECTOR(copySourcePosition).x, position.z - SHENMUE_COPY_VECTOR(copySourcePosition).z));
                    SHENMUE_COPY_FIELD(std::uint8_t, copyOperationState) = 4;
                    SHENMUE_COPY_FIELD(std::uint8_t, copyPhase) = 1;
                    break;
                case 0:
                    CopyBuildSpline(false);
                    next = CopyContinueStep(false, true);
                    if (next)
                        break;
                    GetCharacterWorldPosition(g_transitionCharacter, &position);
                    SHENMUE_COPY_FIELD(std::int16_t, copyTravelYaw) = static_cast<std::int16_t>(
                        dc::sdk::ArcTan2Fixed(position.x - SHENMUE_COPY_VECTOR(copySourcePosition).x, position.z - SHENMUE_COPY_VECTOR(copySourcePosition).z));
                    SHENMUE_COPY_FIELD(std::uint8_t, copyOperationState) = 4;
                    SHENMUE_COPY_FIELD(std::uint8_t, copyPhase) = 1;
                    next = 1;
                    break;
                }
                break;
            }
            if (SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) & MCF_Unknown80) {
                next = CopyContinueStep(true, false);
                if (next)
                    SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) &= ~MCF_Unknown80;
                if ((SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) & (MCF_ExtraStepRequested | MCF_ExtraStepComplete)) == MCF_ExtraStepComplete) {
                    next = 1;
                    SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) &= ~MCF_Unknown80;
                }
            } else {
                SHENMUE_COPY_FIELD(std::uint8_t, copyPhase) = 1;
                next = 0;
            }
            break;
        }
    } while (next);
    return result;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t UpdateCopySideStep() {
    std::uint8_t next = 1;
    bool result = false;
    Vector3 position;
    do {
        switch (SHENMUE_COPY_FIELD(std::uint8_t, copyOperationState)) {
        default:
        case 2:
        case 6:
            CommitCopyMotionTransition(g_transitionWork);
            next = 0;
            break;
        case 0:
        case 1:
            if (SHENMUE_COPY_FIELD(std::uint8_t, transitionVariant) != SHENMUE_COPY_FIELD(std::uint8_t, transitionClass)) {
                SHENMUE_COPY_FIELD(std::uint8_t, transitionClass) = SHENMUE_COPY_FIELD(std::uint8_t, transitionVariant);
                result = true;
                next = 0;
                break;
            }
            if (SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) & MCF_LookPending) {
                SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) &= ~MCF_LookPending;
                SHENMUE_COPY_FIELD(std::uint16_t, transitionStartFrame) = SHENMUE_COPY_FIELD(std::uint16_t, transitionEndFrame);
                SHENMUE_COPY_VECTOR(copySourcePosition) = SHENMUE_COPY_VECTOR(copyDestinationPosition);
                if (SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) & MCF_Unknown20000) {
                    GetCharacterWorldPosition(g_transitionCharacter, &position);
                    SHENMUE_COPY_VECTOR(copySourcePosition).x += position.x;
                    SHENMUE_COPY_VECTOR(copySourcePosition).y += position.y;
                    SHENMUE_COPY_VECTOR(copySourcePosition).z += position.z;
                }
            }
            next = CopyUpdateLook(false);
            if (next) {
                switch (SHENMUE_COPY_FIELD(std::uint8_t, transitionSubstate)) {
                case 0:
                    GetCharacterWorldPosition(g_transitionCharacter, &position);
                    SHENMUE_COPY_FIELD(std::int16_t, copyTravelYaw) =
                        dc::sdk::ArcTan2Fixed(position.x - SHENMUE_COPY_VECTOR(copySourcePosition).x, position.z - SHENMUE_COPY_VECTOR(copySourcePosition).z);
                    next = CopyBeginStep(1);
                    break;
                case 2:
                    SHENMUE_COPY_FIELD(std::int16_t, copyTravelYaw) = SHENMUE_COPY_FIELD(std::uint16_t, transitionStartFrame);
                    next = CopyBeginStep(1);
                    break;
                case 1:
                    SHENMUE_COPY_FIELD(std::uint8_t, copyOperationState) = 3;
                    SHENMUE_COPY_FIELD(std::uint8_t, copyPhase) = 1;
                    next = 1;
                    break;
                }
            }
            break;
        case 3:
            next = CopyTranslateStep();
            break;
        case 4:
            next = CopyTurnStep(0);
            break;
        case 5:
            if (SHENMUE_COPY_FIELD(std::uint8_t, transitionVariant) != SHENMUE_COPY_FIELD(std::uint8_t, transitionClass)) {
                SHENMUE_COPY_FIELD(std::uint8_t, transitionClass) = SHENMUE_COPY_FIELD(std::uint8_t, transitionVariant);
                result = true;
                next = 0;
            } else if (!(SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) & MCF_LookPending) && !(g_transitionWork->regionFlags[0] & MRF_Transition)) {
                SHENMUE_COPY_FIELD(std::uint8_t, copyOperationState) = 6;
            } else {
                next = CopyContinueStep(1, 0);
                if (next) {
                    if (SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) & MCF_LookPending) {
                        SHENMUE_COPY_FIELD(std::uint16_t, transitionStartFrame) = SHENMUE_COPY_FIELD(std::uint16_t, transitionEndFrame);
                        SHENMUE_COPY_VECTOR(copySourcePosition) = SHENMUE_COPY_VECTOR(copyDestinationPosition);
                        if (SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) & MCF_Unknown20000) {
                            GetCharacterWorldPosition(g_transitionCharacter, &position);
                            SHENMUE_COPY_VECTOR(copySourcePosition).x += position.x;
                            SHENMUE_COPY_VECTOR(copySourcePosition).y += position.y;
                            SHENMUE_COPY_VECTOR(copySourcePosition).z += position.z;
                        }
                        SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) &= ~MCF_LookPending;
                        switch (SHENMUE_COPY_FIELD(std::uint8_t, transitionSubstate)) {
                        case 0:
                            GetCharacterWorldPosition(g_transitionCharacter, &position);
                            SHENMUE_COPY_FIELD(std::int16_t, copyTravelYaw) = dc::sdk::ArcTan2Fixed(
                                position.x - SHENMUE_COPY_VECTOR(copySourcePosition).x, position.z - SHENMUE_COPY_VECTOR(copySourcePosition).z);
                            SHENMUE_COPY_FIELD(std::uint8_t, copyOperationState) = 4;
                            SHENMUE_COPY_FIELD(std::uint8_t, copyPhase) = 1;
                            break;
                        case 2:
                            SHENMUE_COPY_FIELD(std::int16_t, copyTravelYaw) = SHENMUE_COPY_FIELD(std::uint16_t, transitionStartFrame);
                            SHENMUE_COPY_FIELD(std::uint8_t, copyOperationState) = 4;
                            SHENMUE_COPY_FIELD(std::uint8_t, copyPhase) = 1;
                            break;
                        case 1:
                            SHENMUE_COPY_FIELD(std::uint8_t, copyOperationState) = 3;
                            SHENMUE_COPY_FIELD(std::uint8_t, copyPhase) = 1;
                            next = 1;
                            break;
                        }
                    } else {
                        SHENMUE_COPY_FIELD(std::uint8_t, copyPhase) = 1;
                        next = 0;
                    }
                }
            }
            break;
        }
    } while (next);
    return result;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void UpdateCopyMotionTransition() {
    if (g_transitionWork->queuedMotionId[0]) {
        CommitCopyMotionTransition(g_transitionWork);
        return;
    }
    if (g_transitionWork->pendingLayerStartFrame[0] > 0.0f)
        SHENMUE_COPY_FIELD(float, copyLayerStartFrame) = g_transitionWork->pendingLayerStartFrame[0];
    g_transitionWork->pendingLayerStartFrame[0] = -1.0f;
    if (g_transitionWork->regionFlags[0] & MRF_LeadFoot) {
        SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) |= MCF_Unknown80000;
        g_transitionWork->regionFlags[0] &= ~MRF_LeadFoot;
    }
    g_transitionWork->layerLoopStartTime[0] = -1.0f;
    g_transitionWork->layerEndFrame[0] = -1.0f;
    g_transitionWork->parentBoneSelector = 9;
    g_transitionWork->requestedRootYawOffset = 0;
    g_transitionWork->latchedConstraintTarget[0] = 0;
    g_transitionWork->latchedConstraintTarget[5] = 0;
    g_transitionWork->latchedConstraintTarget[6] = 0;
    g_transitionWork->activeConstraintTarget[0] = 0;
    g_transitionWork->activeConstraintTarget[5] = 0;
    g_transitionWork->activeConstraintTarget[6] = 0;
    if (ClassifyCurrentLocomotionPhase(g_transitionWork) == 2)
        SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) |= MCF_Running;
    else if (ClassifyCurrentLocomotionPhase(g_transitionWork) == 1)
        SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) &= ~MCF_Running;
    if (SHENMUE_COPY_FIELD(std::uint8_t, copyOperationState) == 5 && !(SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) & MCF_LookPending) &&
        ((!(SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) & MCF_AlternateLocomotion) && ClassifyCurrentLocomotionPhase(g_transitionWork) == 2) ||
            ((SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) & MCF_AlternateLocomotion) && ClassifyCurrentLocomotionPhase(g_transitionWork) == 1))) {
        SHENMUE_COPY_FIELD(std::uint16_t, transitionEndFrame) = SHENMUE_COPY_FIELD(std::uint16_t, transitionStartFrame);
        SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) |= MCF_LookPending;
        SHENMUE_COPY_VECTOR(copyDestinationPosition) = SHENMUE_COPY_VECTOR(copySourcePosition);
    }
    if ((SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) & (MCF_Unknown10000 | MCF_Unknown20000)) == MCF_Unknown10000) {
        Vector3 position;
        GetCharacterWorldPosition(g_transitionCharacter, &position);
        SHENMUE_COPY_VECTOR(copyDestinationPosition).x -= position.x;
        SHENMUE_COPY_VECTOR(copyDestinationPosition).y -= position.y;
        SHENMUE_COPY_VECTOR(copyDestinationPosition).z -= position.z;
        SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) |= MCF_Unknown20000;
    }
    SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) &= ~MCF_Persistent;
    if (SHENMUE_COPY_FIELD(std::uint32_t, copyFlags) & MCF_LookPending)
        CopyUpdateLook(true);
    std::uint8_t next = 1;
    do {
        switch (SHENMUE_COPY_FIELD(std::uint8_t, transitionClass)) {
        case 0:
        case 2:
            next = UpdateCopyToPosition();
            break;
        case 1:
            next = UpdateCopyAlongHeading();
            break;
        case 3:
            next = UpdateCopySideStep();
            break;
        }
    } while (next);
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
float ScaleLookProjection(float cotangent, float scale) {
    const std::int32_t angle = static_cast<std::int32_t>(dc::sdk::ArcTanFixed(std::fabs(cotangent)));
    const std::int32_t expanded = 0x4000 - static_cast<std::int32_t>(static_cast<float>(0x4000 - angle) * scale);
    float result;
    if (cotangent < 0.0f)
        result = -dc::sdk::TanFixed(expanded);
    else
        result = dc::sdk::TanFixed(expanded);
    return result;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void UpdateCopyLookTransition(MotionWork *work) {
    struct LookBitFlags {
        std::uint16_t savedMotionFlag40 : 1;
        std::uint16_t savedSeqFlag800000 : 1;
        std::uint16_t enablePelvisYaw : 1;
        std::uint16_t flag3 : 1;
        std::uint16_t instantLook : 1;
        std::uint16_t leftTargetA : 1;
        std::uint16_t rightTargetA : 1;
        std::uint16_t leftTargetB : 1;
        std::uint16_t rightTargetB : 1;
        std::uint16_t unusedLow : 7;
        std::uint16_t extra0 : 1;
        std::uint16_t extra1 : 1;
        std::uint16_t extra2 : 1;
        std::uint16_t extra3 : 1;
        std::uint16_t unusedHigh : 12;
    };
#define SHENMUE_LOOK_FLAGS (reinterpret_cast<LookBitFlags *>(&work->locomotionMotionSet))
    switch (static_cast<std::uint8_t>(work->transitionState)) {
    case 1:
        if (work->motionFlags & 0x40U)
            work->locomotionMotionSet |= 1U;
        if (work->sequenceFlags & 0x800000U)
            work->locomotionMotionSet |= 2U;
        work->motionFlags |= 0x40U;
        work->extraMotionPosition.x = ScaleLookProjection(work->retainedCollisionValues[0], 0.7f);
        work->extraMotionPosition.y = ScaleLookProjection(work->retainedCollisionValues[1], 0.7f);
        work->extraMotionPosition.z = ScaleLookProjection(work->retainedCollisionValues[2], 0.7f);
        work->extraMotionMask = 0;
        if (work->locomotionMotionSet & 0x10U) {
            work->sequenceFlags &= ~0x800000U;
            work->lookAtBlendTime = 1.0f;
        } else {
            work->sequenceFlags |= 0x800000U;
        }
        mtwkLatchControllerTarget(work, 2, &work->lookAtTarget);
        work->transitionState = 2;
        break;
    case 2:
        if (work->locomotionMotionSet & 0x10U) {
            work->sequenceFlags &= ~0x800000U;
            work->lookAtBlendTime = 1.0f;
        } else {
            work->sequenceFlags |= 0x800000U;
        }
        if (!(work->sequenceFlags & 0x10000000U)) {
            if (SHENMUE_LOOK_FLAGS->enablePelvisYaw) {
                if (work->extraMotionMask & 8U) {
                    work->pelvisYawTarget = static_cast<std::int16_t>(work->pelvisYawCurrent + work->lookAtYawStep);
                    if (work->pelvisYawTarget > static_cast<std::int16_t>(work->pelvisYawClamp))
                        work->pelvisYawTarget = static_cast<std::int16_t>(work->pelvisYawClamp);
                } else if (work->extraMotionMask & 2U) {
                    work->pelvisYawTarget = static_cast<std::int16_t>(work->pelvisYawCurrent + work->lookAtYawStep);
                    if (work->pelvisYawTarget < -static_cast<std::int16_t>(work->pelvisYawClamp))
                        work->pelvisYawTarget = static_cast<std::int16_t>(-static_cast<std::int16_t>(work->pelvisYawClamp));
                } else if (*reinterpret_cast<const std::uint32_t *>(&work->locomotionMotionSet) & 0x000f0000U) {
                    work->pelvisYawTarget = work->pelvisYawCurrent;
                } else {
                    work->pelvisYawTarget = 0;
                }
            } else {
                work->pelvisYawTarget = 0;
            }
        }
        mtwkLatchControllerTarget(work, 2, &work->lookAtTarget);
        break;
    case 3:
        if (work->locomotionMotionSet & 0x10U) {
            work->sequenceFlags &= ~0x800000U;
            work->lookAtBlendTime = 0.0f;
            work->pelvisYawCurrent = 0;
            work->pelvisYawHalf = 0;
        } else {
            work->sequenceFlags |= 0x800000U;
        }
        if (!(work->sequenceFlags & 0x10000000U))
            work->pelvisYawTarget = 0;
        if (!((work->constraintStateBits >> 6) & 7U) && ((work->sequenceFlags & 0x10000000U) || !work->lookAtYaw) && work->extraBlendTime == 0.0f) {
            if (!SHENMUE_LOOK_FLAGS->savedMotionFlag40)
                work->motionFlags &= ~0x40U;
            if (!SHENMUE_LOOK_FLAGS->savedSeqFlag800000)
                work->sequenceFlags &= ~0x800000U;
            else
                work->sequenceFlags |= 0x800000U;
            work->transitionState = 0;
            work->extraMotionSpeed = 0;
            SHENMUE_LOOK_FLAGS->flag3 = 0;
            SHENMUE_LOOK_FLAGS->enablePelvisYaw = 0;
            SHENMUE_LOOK_FLAGS->leftTargetA = 0;
            SHENMUE_LOOK_FLAGS->leftTargetB = 0;
            SHENMUE_LOOK_FLAGS->rightTargetA = 0;
            SHENMUE_LOOK_FLAGS->rightTargetB = 0;
            work->sequenceFlags &= ~0x8000000U;
        }
        break;
    }
    if (*reinterpret_cast<const std::uint32_t *>(&work->locomotionMotionSet) & 0xa0U)
        mtwkLatchControllerTarget(work, 3, &work->leftControllerTarget);
    if (*reinterpret_cast<const std::uint32_t *>(&work->locomotionMotionSet) & 0x140U)
        mtwkLatchControllerTarget(work, 4, &work->rightControllerTarget);
    SHENMUE_LOOK_FLAGS->instantLook = 0;
#undef SHENMUE_LOOK_FLAGS
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void UpdateTargetFacingTransition(MotionWork *work) {
    Vector3 position;
    Vector3 direction;
    Vector3i rotation;
    float distance;
    switch (work->targetFacingState) {
    case 1:
        GetCharacterWorldPosition(g_transitionCharacter, &position);
        GetCharacterWorldRotation(g_transitionCharacter, &rotation);
        direction.x = work->targetFacingPosition.x - position.x;
        direction.y = 0.0f;
        direction.z = work->targetFacingPosition.z - position.z;
        Vector3::CalcMagnitude(direction);
        work->targetFacingYaw = dc::sdk::ArcTan2Fixed(-direction.x, -direction.z);
        if (work->targetFacingFlags & 1U) {
            mtwkCopyTransition(work, 0, work->targetFacingYaw, 0, 2);
            mtwkCopyTransition(work, 0, 0, 2, 0);
            work->targetFacingYawDelta = 0;
        } else if (work->sequenceFlags & 0x4000000U) {
            mtwkCopyTransition(work, 0, rotation.y, 0, 2);
        }
        work->targetFacingState = 2;
        return;
    case 2:
        if ((work->sequenceFlags & 0x4000000U) && ((work->entityYaw - work->targetFacingYaw) & 0x7fff) > 0x71b)
            return;
        work->targetFacingState = 3;
    case 3:
        GetCharacterWorldPosition(g_transitionCharacter, &position);
        GetCharacterWorldRotation(g_transitionCharacter, &rotation);
        direction.x = work->targetFacingPosition.x - position.x;
        direction.y = 0.0f;
        direction.z = work->targetFacingPosition.z - position.z;
        distance = Vector3::CalcMagnitude(direction);
        work->targetFacingYaw = dc::sdk::ArcTan2Fixed(-direction.x, -direction.z);
        work->targetFacingYawDelta = work->targetFacingYaw - static_cast<std::int16_t>(rotation.y);
        if (!(work->targetFacingFlags & 2U)) {
            if ((work->targetFacingYawDelta > 0 && distance > 1.0f) || (work->targetFacingYawDelta <= 0 && !(distance > 1.0f)))
                work->targetFacingFlags |= 4U;
            else
                work->targetFacingFlags &= ~4U;
        }
        if (distance > 40.0f)
            work->targetFacingFlags |= 0x40U;
        else
            work->targetFacingFlags &= ~0x40U;
        if ((work->targetFacingFlags & 8U) &&
            ((*reinterpret_cast<const std::uint32_t *>(&work->targetFacingState) & 0x140000U) == 0x100000U ||
                (*reinterpret_cast<const std::uint32_t *>(&work->targetFacingState) & 0x140000U) == 0x40000U))
            work->targetFacingFlags |= 0x20U;
        PlayTargetFacingTurn(work, true, work->targetFacingFlags & 4U, work->targetFacingYawDelta);
        work->targetFacingState = 4;
        return;
    case 4:
        work->chestClock.playbackSpeed = 1.0f;
        work->headClock.playbackSpeed = 1.0f;
        if (work->targetFacingFlags & 4U)
            work->leftArmClock.playbackSpeed = 1.0f;
        else
            work->rightArmClock.playbackSpeed = 1.0f;
        return;
    case 5:
        PlayTargetFacingTurn(work, false, work->targetFacingFlags & 4U, work->targetFacingYawDelta);
        work->pelvisYawTarget = 0;
        work->targetFacingState = 6;
        return;
    case 6: {
        work->chestClock.playbackSpeed = 1.0f;
        work->headClock.playbackSpeed = 1.0f;
        if (work->targetFacingFlags & 4U)
            work->leftArmClock.playbackSpeed = 1.0f;
        else
            work->rightArmClock.playbackSpeed = 1.0f;
        const float frame = mtwkGetFrame(work, Chest);
        const float count = static_cast<float>(static_cast<std::int32_t>(GetMotionFrameCount(work->currentMotionId[Chest])));
        if (frame > count * 0.5f) {
            mtwkRequestRegionDetach(work, Head);
            mtwkRequestRegionDetach(work, Chest);
            const std::uint32_t frames = static_cast<std::uint32_t>(count - frame);
            mtwkSetFrameTime(work, frames, Chest);
            mtwkSetFrameTime(work, frames, Head);
            if (work->targetFacingFlags & 0x20U) {
                mtwkRequestRegionDetach(work, LeftArm);
                mtwkRequestRegionDetach(work, RightArm);
                mtwkSetFrameTime(work, frames, LeftArm);
                mtwkSetFrameTime(work, frames, RightArm);
            } else if (work->targetFacingFlags & 4U) {
                mtwkRequestRegionDetach(work, LeftArm);
                mtwkSetFrameTime(work, static_cast<std::uint32_t>(count - frame), LeftArm);
            } else {
                mtwkRequestRegionDetach(work, RightArm);
                mtwkSetFrameTime(work, static_cast<std::uint32_t>(count - frame), RightArm);
            }
            work->chestRotationOverrideBlendTime = 1.0f / (count - frame);
            work->targetFacingState = 7;
        }
        return;
    }
    case 7:
        if (!(work->regionFlags[Chest] & 0x800U)) {
            work->chestClock.playbackSpeed = work->lowerBodyClock.playbackSpeed;
            work->headClock.playbackSpeed = work->lowerBodyClock.playbackSpeed;
            if (work->targetFacingFlags & 0x20U) {
                work->leftArmClock.playbackSpeed = work->lowerBodyClock.playbackSpeed;
                work->rightArmClock.playbackSpeed = work->lowerBodyClock.playbackSpeed;
            } else if (work->targetFacingFlags & 4U) {
                work->leftArmClock.playbackSpeed = work->lowerBodyClock.playbackSpeed;
            } else {
                work->rightArmClock.playbackSpeed = work->lowerBodyClock.playbackSpeed;
            }
            work->targetFacingFlags &= ~1U;
            work->targetFacingState = 0;
            work->sequenceFlags &= ~0x10000000U;
        }
        return;
    }
}
#endif

} // namespace

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
std::int32_t UpdateMotionTransitionControllers(Character *character) {
    g_transitionCharacter = character;
    g_transitionWork = charFindMotionwork(character);
    if (g_transitionWork->currentMotionId[0])
        g_transitionHadMotion = 1;
    else
        g_transitionHadMotion = 0;
    if (!g_transitionWork)
        return 0;
    if ((g_transitionWork->sequenceFlags & MSF_TargetFacing) && !g_transitionWork->useMatrixOverrides)
        UpdateTargetFacingTransition(g_transitionWork);
    if (g_transitionWork->sequenceFlags & MSF_PositionTransition) {
        g_copyTransitionState = &g_transitionWork->copyOperationState;
        UpdateCopyMotionTransition();
    }
    if (g_transitionWork->sequenceFlags & MSF_LookAt)
        UpdateCopyLookTransition(g_transitionWork);
    return 1;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
bool mtwkSetDrawOverrideData(MotionWork *work, const Matrix4x4 *matrix) {
    if (!work)
        return false;
    work->drawOverrideData = matrix;
    return true;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
const Matrix4x4 *mtwkGetDrawOverrideData(const MotionWork *work) {
    return work ? work->drawOverrideData : 0;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
bool mtwkSetDrawOverrideParameter(MotionWork *work, void *parameter) {
    if (!work)
        return false;
    *reinterpret_cast<void **>(work->retainedExecutionStateTail) = parameter;
    return true;
}

void *mtwkGetDrawOverrideParameter(const MotionWork *work) {
    return work ? *reinterpret_cast<void *const *>(work->retainedExecutionStateTail) : 0;
}
#endif
#endif

// @verified - d
void mtwkSetReactFlags(MotionWork *work, std::uint8_t flags) {
    if (work)
        work->environmentFlags |= flags & 15U;
}

// @verified - d
float mtwkGetClusterPosition(const MotionWork *work, Vector3 *position) {
    if (!work)
        return 0.0f;
    *position = work->clusterCenter;
    return work->clusterRadius;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t charSetMotionRegionFlag(Character *character, std::uint32_t flag, std::int32_t operation, MotionRegion region) {
    return mtwkSetRegionFlag(static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION)), flag, operation, region);
}
#endif

// @verified - d
void SetCharaTargetTransformOffset(shen_name name, const Vector3 *factor, std::int32_t rotation) {
    Vector3 delta;
    Vector3i angles = {0, 0, 0};
    Character *const character = scenFindChara(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->scenarioTask, name);
    if (!character)
        return;
    MotionWork *const work = static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION));
    if (!work)
        return;
    mtwkUpdateRootDeltaVector(work, &delta);
    if (rotation) {
        GetCharacterLocalRotation(character, &angles);
        matrix_stack::PushUnitMatrix();
        Matrix4x4::RotateAroundY_Wrapper(-angles.y);
        Matrix4x4::TransformPoint(delta, delta);
        delta.x *= factor->x;
        delta.y *= factor->y;
        delta.z *= factor->z;
        PopCurrentMatrix(1);
        matrix_stack::PushUnitMatrix();
        Matrix4x4::RotateAroundY_Wrapper(angles.y);
        Matrix4x4::TransformPoint(delta, delta);
        PopCurrentMatrix(1);
        AssignCharacterChildSpacePosition(character, &delta);
    } else {
        delta.x *= factor->x;
        delta.y *= factor->y;
        delta.z *= factor->z;
        AddCharacterChildSpacePosition(character, &delta);
    }
}

// @verified - d
bool mtwkSetSequenceFlags(Character *character) {
    MotionWork *const work = static_cast<MotionWork *>(charFindComp(character, CHARACTER_MOTION));
    if (!work)
        return false;
    work->sequenceFlags &= ~MSF_ModelMatricesBound;
    return true;
}

// @verified - d
std::int32_t mtwkSetOverridesEnabled(std::int32_t enabled) {
    const std::int32_t previous = g_motionDrawOverridesEnabled;
    g_motionDrawOverridesEnabled = enabled;
    return previous;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t mtwkGetOverridesEnabled() {
    return g_motionDrawOverridesEnabled;
}

std::int32_t mtwkSetLinkedVerticesEnabled(std::int32_t enabled) {
    const std::int32_t previous = g_characterVertexBlendEnabled;
    g_characterVertexBlendEnabled = enabled;
    return previous;
}

std::int32_t mtwkGetLinkedVerticesEnabled() {
    return g_characterVertexBlendEnabled;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t mtwkUpdateWithPosition(MotionWork *work, const Vector3 *position, std::uint32_t operation) {
    if (!work)
        return 0;
    work->copyFlags &= ~0x100000U;
    g_transitionWork->motionFlags &= ~0x40000U;
    std::uint32_t speedBits = operation & 0xff00U;
    std::uint32_t speed = speedBits >> 8;
    if (!speed)
        speed = 3;
    switch (operation & 0xffU) {
    case 0:
        work->locomotionMotionSet &= ~8U;
        work->locomotionMotionSet &= ~4U;
        if (!work->extraMotionSpeed)
            work->extraMotionSpeed = speed;
        break;
    case 1:
        work->locomotionMotionSet = (work->locomotionMotionSet & ~8U) | 4U;
        if (!work->extraMotionSpeed)
            work->extraMotionSpeed = speed;
        break;
    case 2:
        work->locomotionMotionSet |= 12U;
        if (!work->extraMotionSpeed)
            work->extraMotionSpeed = speed;
        break;
    case 4:
        work->extraMotionSpeed = speed;
        break;
    default:
        if (work->sequenceFlags & 0x8000000U) {
            switch (operation & 0xffU) {
            case 3:
                work->locomotionMotionSet |= 0x10U;
                return 1;
            case 5:
                work->locomotionMotionSet &= ~0x20U;
                work->locomotionMotionSet &= ~0x80U;
                work->locomotionMotionSet &= ~0x40U;
                work->locomotionMotionSet &= ~0x100U;
                return 1;
            case 6:
                work->locomotionMotionSet |= 0x60U;
                mtwkGetControllerTargetPosition(work, 3, work->leftControllerTarget);
                mtwkGetControllerTargetPosition(work, 4, work->rightControllerTarget);
                return 1;
            case 7:
                work->locomotionMotionSet |= 0x180U;
                mtwkGetControllerTargetPosition(work, 3, work->leftControllerTarget);
                mtwkGetControllerTargetPosition(work, 4, work->rightControllerTarget);
                return 1;
            case 8:
                work->locomotionMotionSet &= ~0x20U;
                work->locomotionMotionSet &= ~0x80U;
                return 1;
            case 9:
                work->locomotionMotionSet |= 0x20U;
                mtwkGetControllerTargetPosition(work, 3, work->leftControllerTarget);
                return 1;
            case 10:
                work->locomotionMotionSet |= 0x80U;
                mtwkGetControllerTargetPosition(work, 3, work->leftControllerTarget);
                return 1;
            case 11:
                work->locomotionMotionSet &= ~0x40U;
                work->locomotionMotionSet &= ~0x100U;
                return 1;
            case 12:
                work->locomotionMotionSet |= 0x40U;
                mtwkGetControllerTargetPosition(work, 4, work->rightControllerTarget);
                return 1;
            case 13:
                work->locomotionMotionSet |= 0x100U;
                mtwkGetControllerTargetPosition(work, 4, work->rightControllerTarget);
                return 1;
            }
        }
        return 0;
    }
    float scale;
    switch (work->extraMotionSpeed) {
    case 1:
        scale = 0.5f;
        break;
    case 2:
        scale = 0.75f;
        break;
    case 4:
        scale = 1.5f;
        break;
    case 5:
        scale = 2.0f;
        break;
    default:
        scale = 1.0f;
    }
    work->constraintStepLimit[2] = 0.061359234154224396f * scale;
    work->pelvisYawStep = static_cast<std::uint16_t>(384.0f * scale);
    work->extraBlendScale = 0.05f * scale;
    if ((operation & 0xffU) == 4)
        return 1;
    if (work->sequenceFlags & 0x10000000U) {
        work->locomotionMotionSet &= ~8U;
        work->locomotionMotionSet &= ~4U;
    }
    if (position) {
        if ((work->sequenceFlags & 0x8000000U) && work->transitionState != 1) {
            work->transitionState = 2;
        } else {
            work->sequenceFlags |= 0x8000000U;
            work->transitionState = 1;
        }
        work->lookAtTarget = *position;
        return 1;
    }
    if (!position) {
        if (work->sequenceFlags & 0x8000000U)
            work->transitionState = 3;
        else
            work->extraMotionSpeed = 0;
    }
    return 1;
}
#endif

} // namespace s2
} // namespace shenmue

#undef SHENMUE_MOTION_ROOT_SIGNED_WORD
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#undef SHENMUE_COPY_STATE
#undef SHENMUE_COPY_ANIMATION
#endif
#undef SHENMUE_MOTION_NULL
#undef SHENMUE_MOTION_CONST_CAST

#endif
