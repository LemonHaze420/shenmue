#include "s2/door_control_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/prop_dc.hpp"
#include "s2/model_render_dc.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/qron_dc.hpp"
#include "s2/search_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/date_time.hpp"
#include "s2/camera_dc.hpp"
#include "s2/camera_state_query_dc.hpp"
#include "s2/character_control_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/motion_ground_dc.hpp"
#include "s2/help_icon_dc.hpp"
#include "s2/magic_room_dc.hpp"
#include "s2/scenario_control_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/map_event_task_dc.hpp"
#include "s2/event_character_control_dc.hpp"
#include "s2/map_info.hpp"
#include "s2/scene_data.hpp"
#include <cstdio>
#include "s2/door_state_dc.hpp"
#include "s2/field_event_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/math.hpp"
#include "s2/motion_registry_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/npc_data_dc.hpp"
#include "platform/dc/matrix.hpp"
#include "s2/event_matrix_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/story_state.hpp"
#include "s2/sound_program.hpp"
#include "s2/startup_assets_dc.hpp"
#include "platform/dc/sdk.hpp"
#include <cstring>
#endif
#include "s2/title_dc.hpp"
#include "s2/title_warning.hpp"
#include "s2/flag_startup_dc.hpp"
#include "s2/lighting.hpp"
#include "s2/load_title.hpp"
#include "s2/map_event_transition_dc.hpp"
#include "s2/map_models_dc.hpp"
#include "s2/map_render_profile.hpp"
#include "s2/map_visibility_dc.hpp"
#include "s2/pak_dc.hpp"
#include "s2/screen_film.hpp"
#include "s2/weather.hpp"
#include "s2/weather_instruction_dc.hpp"
#include "s2/hlib.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/npc_area_route.hpp"
#endif
#define SHENMUE_NPC_NULL 0
#include "s2/legacy_sprite_slot.hpp"
#include "s2/texture_dc.hpp"
#include "s2/legacy_sprite_dc.hpp"
#include "s2/title_sprite_dc.hpp"
#include "auto_test.hpp"
#include "s2/debug_screen_dc.hpp"
#include "s2/input_dc.hpp"
#include "s2/frame_dc.hpp"
#include "s2/load_title_dc.hpp"
#include "s2/save_screen_state_dc.hpp"
#include "s2/title_save_dc.hpp"
#include "s2/sound_dc.hpp"
#include "s2/area_names.hpp"
#include "s2/calendar_dc.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/system_modules_dc.hpp"
#include <cmath>
#include "s2/directory_cache.hpp"
#include <sg_gd.h>
#include <string.h>
#define SHENMUE_LOADER_NULL 0
#define SHENMUE_LOADER_ALIGN __attribute__((aligned(32)))
#define SHENMUE_LOADER_INIT =
#define SHENMUE_LOADER_MEMCPY ::memcpy
#define SHENMUE_LOADER_STRCPY ::strcpy
#define SHENMUE_LOADER_STRCMP ::strcmp
#define SHENMUE_LOADER_STRSTR ::strstr
#define SHENMUE_LOADER_STRLEN ::strlen
#define SHENMUE_LOADER_STRNCPY ::strncpy
#define SHENMUE_LOADER_FILE_COUNT (sizeof(g_files) / sizeof(g_files[0]))
#define SHENMUE_LOADER_CONST_BYTES(value) static_cast<const std::byte *>(static_cast<const void *>(value))
#include <zlib.h>
#include "s2/event_overlay.hpp"

extern "C" std::uint32_t __attribute__((renesas)) shenmue_s2_event_EV_LoadAudio(const char *name, std::uint32_t sceneIndex);

namespace shenmue {
namespace s2 {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
struct DoorAreaScript {
    void (*eventCallback)(std::int32_t);
    std::int32_t eventId;
    shen_name condition;
    std::int8_t conditionKind, doorNumbers[2], mode;
};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
typedef char DoorSetupLayout0[(sizeof(DoorAreaScript) == 0x10) ? 1 : -1];
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
typedef char DoorSetupLayout1[(offsetof(DoorTaskEntry, digits) == 0xc4) ? 1 : -1];
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
typedef char DoorSetupLayout2[(offsetof(DoorTaskEntry, motionScale) == 0x1d8) ? 1 : -1];
#endif

std::int32_t BeginDoorAnimation(std::int32_t doorId, shen_name claimant, std::int32_t animationId, float frame, float speed);

std::int32_t GetDoorInteractionAction(shen_name subject);

static void UnlinkExcludedDoors();

DoorTaskEntry *FindDoorEntry(shen_name subject);

void DoorAdmission_Callback(void *);

void SetDoorNodeHierarchyHidden(MT7Node *node, std::uint32_t hidden);

void ApplyDoorSceneStateToSiblings(MT7Node *node, std::int32_t sceneId);

std::int32_t ApplyDoorSceneState(std::int32_t sceneId);

void DoorTask_Callback(void *);

void drUpdateEntry();

static std::int32_t SetDoorRouteCondition(shen_name subject, shen_name condition, std::int32_t mode, std::int32_t sense);

static std::int32_t SetDoorRequiredEntry(shen_name subject, shen_name required, std::int32_t mode);

static void DoorMapEventNullsub(std::int32_t);

std::uint16_t GetDoorLeafAnimation(std::int32_t animationId);

static void DoorTransitionNullsub(shen_name);

const char *GetDoorAuthorName(shen_name name);

void UpdateDoorAuthorController();

void UpdateDoorDestinationController();

static std::int32_t IsDoorAuthorActive();

std::int32_t LoadDoorAuthor(shen_name author);

std::int32_t RestoreDoorAuthor(std::int32_t restoreAudio);

std::int32_t IsDoorTransitionActive();

void SetupDoorAreaCallbacks();

void DoorSceneCleanup(std::int32_t);

void authLoadCurrentQueued();

void RestoreDoorAreaCallbacks();

std::int32_t AdvanceDoorMotion(DoorTaskEntry *entry);

void FinishDoorAuthorLeaf(shen_name subject);

static void LoadDoorNumberModels();

void CompleteDoorPlayerPlacement(const Matrix4x4 *transform);

void CompleteAutomaticDoorPlayerPlacement(const Matrix4x4 *transform);

void TransformPropObject();

void RefreshDoorSceneStateForCurrentStep();

std::int32_t SelectDiscDirectory(const char *directory, std::int32_t mode);

std::int32_t shenLoadFile(const char *directory, const char *filename, void *destination, std::uint32_t size);

void *shenLoadFile(const char *directory, const char *filename, shen_name allocationName);

#if SHENMUE_IS_DC && SHENMUE_IS_S2
int shenFileExists(const char *directory, const char *filename);
#else
bool shenFileExists(const char *directory, const char *filename);
#endif

void SubmitTitleWarningLogo(void *);

void DrawMainIconOverlay(std::uint32_t colour, float depth);

std::int32_t SelectDiscDirectory(const char *directory, std::int32_t mode);

#if SHENMUE_IS_DC && SHENMUE_IS_S2
int shenFileExists(const char *directory, const char *filename);
#else
bool shenFileExists(const char *directory, const char *filename);
#endif

std::int32_t shenLoadFile(const char *directory, const char *filename, void *destination, std::uint32_t size);

void *shenLoadFile(const char *directory, const char *filename, shen_name allocationName);

DoorDefinitionTable *g_doorDefinitionTable = 0;

std::int32_t shenUpdateFlagEvaluation(std::int32_t mode);

namespace {

std::uint32_t doorLightingReaderMask;

} // namespace

namespace {

std::int32_t doorAuthorCameraState;

} // namespace

namespace {

Vector3 doorAuthorCameraPosition;

} // namespace

namespace {

DoorAreaCleanupCallback savedDoorCleanupCallback = 0;

} // namespace

void FreeTempSprites(std::int32_t index);

void AddWeatherInstructionFlags(std::uint32_t flags);

void RemoveWeatherInstructionFlags(std::uint32_t flags);

std::int32_t AllocateTempSprites(std::int32_t index);

bool BindTempSprite(std::int32_t index, std::int32_t texture, std::int32_t slot, const std::uint16_t *rectangles);

#if SHENMUE_IS_DC && SHENMUE_IS_S2
DoorTaskEntry *g_doorEntryHead = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
DoorIngressState g_doorIngress = {};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
shen_name g_doorEntrySubject;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t g_doorEntryAction, g_doorEntryState;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static MT7Node *g_doorNumberModels[10];
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static MapEventCallbackEntry g_doorMapCallbacks[32] = {};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static DoorTaskEntry *g_doorEntryTail = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static char g_doorAuthorName[5];
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static shen_name g_doorCrossingSubject;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static float g_doorPreviousCrossingZ;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
float g_doorCrossingZ;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t g_doorNearbyCharacterCount = 0, g_doorAutomaticOpeningAllowed = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::uint32_t g_doorAutomaticExitSide = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static const Matrix4x4 *g_doorAutomaticTransform = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t g_doorCurrentStream = 1;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t g_doorEntryMotion, g_doorEntryWait;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t g_doorAnswerSide, g_doorAnswerMotion = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t g_doorAnswerDoorMotion;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static float g_doorAnswerFrame;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static Vector3 g_doorAnswerPosition;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static Vector3i g_doorAnswerRotation;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t g_doorCameraFrame = 0, g_doorCameraDuration = 0;
static std::uint32_t g_doorPreviousCameraState = 0;
static std::int32_t g_doorMessageFrames = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static Vector3 g_doorCameraStartPosition = {}, g_doorCameraStartTarget = {}, g_doorCameraEndPosition = {}, g_doorCameraEndTarget = {};
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
DoorAreaCleanupCallback g_doorAreaCleanupCallback = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::uint32_t titleWarningPhase, titleWarningTailFrames = 0;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::uint32_t titleWarningBaseColour, titleWarningCurrentColour, titleWarningDesiredColour;
#endif

DoorTaskEntry *g_doorTaskEntries = 0;

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t SetQeDoorSceneState(std::int32_t mode) {
    switch (mode) {
    case 0:
        return 0;
    case 1: {
        DoorTaskEntry *const entry = FindDoorEntry(SHEN_NAME(R146));
        if (!entry)
            return 0;
        if (entry->primary)
            ApplyDoorSceneStateToSiblings(entry->primary, 1);
        return 1;
    }
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t drInitEnter_Enqueue(const MapInfoChunk *chunk) {
    g_doorDefinitionTable = 0;
    g_doorTaskEntries = 0;
    g_doorEntryHead = 0;
    g_doorIngress.selectedDoor = 0;
    g_doorCrossingSubject = 0;
    g_doorPreviousCrossingZ = 0.0f;
    g_doorEntrySubject = 0;
    g_doorEntryAction = 0;
    g_doorEntryMotion = 0;
    g_doorEntryState = 0;
    g_doorEntryWait = 0;
    g_doorAnswerSide = 0;
    g_doorAnswerMotion = 0;
    g_doorAreaCleanupCallback = 0;
    savedDoorCleanupCallback = 0;
    g_doorCurrentStream = 1;
    if (!chunk)
        return -1;
    std::byte *const payload = reinterpret_cast<std::byte *>(const_cast<MapInfoChunk *>(chunk)) + 8;
    if (!(payload + 8))
        return -1;
    g_doorDefinitionTable = reinterpret_cast<DoorDefinitionTable *>(payload + 8);
    shen_name *pair = reinterpret_cast<shen_name *>(g_doorDefinitionTable + 1);
    MT7Node *const root = GetPropRoot();
    g_doorRuntimeState.transitionState = 0;
    g_doorRuntimeState.temporaryAuthor = 0;
    g_doorRuntimeState.destinationDoor = 0;
    g_doorDestination.phase = 0;
    g_doorDestination.area = 0;
    g_doorDestination.region = 0;
    if (!root)
        return -1;
    HLib::EnqueueTaskWithParameter(DoorAdmission_Callback, 8, 2, 0, SHEN_NAME(DOOR));
    DoorTaskEntry *entry = g_doorTaskEntries =
        static_cast<DoorTaskEntry *>(HLib::AllocateTaskData(g_doorDefinitionTable->count * sizeof(DoorTaskEntry), SHEN_NAME(DOOR)));
    std::memset(entry, 0, g_doorDefinitionTable->count * sizeof(DoorTaskEntry));
    if (!entry)
        return -1;
    LoadDoorNumberModels();
    Vector3 origin;
    DateTime date;
    shenGetCurrentDateTime(date);
    std::int32_t *const cachedSceneId = &g_doorCachedSceneId;
    if (date.hour > 18)
        *cachedSceneId = 1;
    else
        *cachedSceneId = 0;
    for (std::int32_t index = 0; index < g_doorDefinitionTable->count; ++index, ++entry, pair += 2) {
        entry->motionScale = 1.0f;
        entry->previous = entry - 1;
        entry->next = entry + 1;
        entry->flags = 0x10U;
        entry->sourcePair = pair;
        entry->primary = FindModelCharacterRecursive(root, pair[0]);
        if (!pair[1] && entry->primary->parent) {
            const std::uint32_t marker = GetModelMarker(entry->primary->parent);
            if ((marker & 0xffffU) == 0x3052U)
                pair[1] = marker;
        }
        entry->secondary = FindModelCharacterRecursive(root, pair[1]);
        entry->motionNode = *entry->primary;
        entry->primary->child = &entry->motionNode;
        entry->motionNode.parent = entry->primary;
        if (entry->motionNode.child)
            entry->motionNode.child->parent = &entry->motionNode;
        entry->primary->mesh = 0;
        entry->motionNode.sibling = 0;
        entry->motionNode.position.x = 0.0f;
        entry->motionNode.position.y = 0.0f;
        entry->motionNode.position.z = 0.0f;
        entry->motionNode.rotation.x = 0;
        entry->motionNode.rotation.y = 0;
        entry->motionNode.rotation.z = 0;
        entry->motion.mode = 0;
        matrix_stack::PushUnitMatrix();
        if (entry->secondary) {
            Matrix4x4::preTranslate(entry->secondary->position.x, entry->secondary->position.y, entry->secondary->position.z);
            Matrix4x4::SetViewMatrixFromEuler(entry->secondary->rotation.x, entry->secondary->rotation.y, entry->secondary->rotation.z);
        }
        matrix_stack::GetTranslation(&origin);
        Matrix4x4::preTranslate(entry->primary->position.x, entry->primary->position.y, entry->primary->position.z);
        Matrix4x4::SetViewMatrixFromEuler(entry->primary->rotation.x, entry->primary->rotation.y, entry->primary->rotation.z);
        matrix_stack::GetMatrix(&entry->worldTransform);
        matrix_stack::GetTranslation(&entry->interactionOrigin);
        PopCurrentMatrix(1);
        InverseTransformPointByMatrix(&entry->worldTransform, &origin);
        if (!(origin.z < 0.0f))
            entry->flags |= 8U;
        ApplyDoorSceneStateToSiblings(entry->primary, *cachedSceneId);
    }
    g_doorTaskEntries->previous = 0;
    g_doorEntryHead = g_doorTaskEntries;
    g_doorTaskEntries[g_doorDefinitionTable->count - 1].next = 0;
    g_doorEntryTail = &g_doorTaskEntries[g_doorDefinitionTable->count - 1];
    UnlinkExcludedDoors();
    TransformPropObject();
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
const Matrix4x4 *doorFindEntryTransform(shen_name subject) {
    const DoorTaskEntry *const entry = FindDoorEntry(subject);
    if (!entry)
        return 0;
    return &entry->worldTransform;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t IsDoorSecondaryNodeEnabled(shen_name subject) {
    const DoorTaskEntry *const entry = FindDoorEntry(subject);
    if (!entry || !entry->secondary)
        return 0;
    return entry->secondary->kind == 0;
}
#endif

// @verified - d
std::uint32_t SetDoorSubjectEnabled(shen_name subject, std::int32_t enabled) {
    if (static_cast<char>(subject >> 8U) == '0') {
        MT7Node *const root = GetPropRoot();
        if (!root)
            return 0;
        MT7Node *const node = FindModelCharacterRecursive(root, subject);
        if (!node)
            return 0;
        node->kind = enabled == 0;
    } else {
        DoorTaskEntry *const entry = FindDoorEntry(subject);
        MT7Node *child;
        if (entry) {
            if (enabled)
                entry->flags |= 0x10U;
            else
                entry->flags &= ~0x10U;
            entry->primary->kind = enabled == 0;
            child = entry->primary->child;
        } else {
            MT7Node *const root = GetPropRoot();
            if (!root)
                return 0;
            MT7Node *const node = FindModelCharacterRecursive(root, subject);
            if (!node)
                return 0;
            node->kind = enabled == 0;
            child = node->child;
        }
        if (!child)
            return 1;
        SetDoorNodeHierarchyHidden(child, enabled == 0);
    }
    return 1;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static std::uint16_t IsDoorSubjectEnabled(shen_name subject) {
    DoorTaskEntry *const entry = FindDoorEntry(subject);
    if (entry)
        return entry->flags & 0x10U;
    MT7Node *const root = GetPropRoot();
    if (root) {
        MT7Node *const node = FindModelCharacterRecursive(root, subject);
        if (node)
            return node->kind == 0;
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
bool HasDoorEntry(shen_name door) {
    if (FindDoorEntry(door))
        return true;
    return false;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
shen_name GetNearbyDoorForNpc(const Vector3 *position) {
    if (!position)
        return 0;
    if (!g_doorDefinitionTable)
        return 0;
    if (!g_doorEntryHead)
        return 0;
    if (g_doorEntryHead->distanceSquared > kDoorPlayerClaimDistanceSquared)
        return 0;
    const shen_name name = *g_doorEntryHead->sourcePair;
    if (static_cast<char>(name >> 8U) == '7')
        return 0;
    return name;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
bool drIsDoorButtonAvailable(shen_name subject) {
    const DoorTaskEntry *const entry = FindDoorEntry(subject);
    if (!entry)
        return false;
    const std::uint32_t state = entry->flags & 3U;
    if (state == 2U)
        return true;
    if (state)
        return false;
    if (entry->claimant)
        return false;
    return true;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
bool IsDoorAvailableForNpcRoute(shen_name door, const Vector3 *position) {
    const DoorTaskEntry *const entry = FindDoorEntry(door);
    if (!entry)
        return false;
    Vector3 delta;
    delta.x = position->x - entry->interactionOrigin.x;
    delta.z = position->z - entry->interactionOrigin.z;
    if (delta.x * delta.x + delta.z * delta.z > 400.0f)
        return false;
    const std::uint32_t state = entry->flags & 3U;
    if (state != 2U) {
        if (state)
            goto failed;
        if (entry->claimant)
            goto failed;
    }
    return true;
failed:
    return false;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
bool ComputeDoorIngress(std::int32_t doorId, const Vector3 *currentPosition, Vector3 *targetPosition) {
    g_doorIngress.selectedDoor = 0;
    const DoorTaskEntry *const entry = FindDoorEntry(shen_name(static_cast<std::uint32_t>(doorId)));
    if (!entry)
        return false;
    matrix_stack::Matrix4x4_Push();
    matrix_stack::LoadMatrix(&entry->worldTransform);
    *targetPosition = *currentPosition;
    InverseTransformPointByMatrix(&entry->worldTransform, targetPosition);
    g_doorIngress.selectedDoor = shen_name(static_cast<std::uint32_t>(doorId));
    if (targetPosition->x * targetPosition->x + targetPosition->z * targetPosition->z > 400.0f) {
        PopCurrentMatrix(1);
        return false;
    }
    ExtractMatrixEulerAnglesFixed(&entry->worldTransform, &g_doorIngress.rotation);
    if (targetPosition->z < 0.0f)
        g_doorIngress.leafSide = (static_cast<std::int16_t>(entry->flags) >> 3) & 1;
    else
        g_doorIngress.leafSide = ((static_cast<std::int16_t>(entry->flags) >> 3) ^ 1) & 1;
    switch (static_cast<char>(static_cast<std::uint32_t>(doorId) >> 8U)) {
    case '1':
        if (targetPosition->z < 0.0f) {
            g_doorIngress.side = 1;
            Matrix4x4::preTranslate(0.5f, 0.0f, -0.55000001f);
            g_doorIngress.rotation.y += 0x8000;
            break;
        }
        Matrix4x4::preTranslate(0.55000001f, 0.0f, 0.80000001f);
        g_doorIngress.side = 0;
        break;
    case '2':
    case '3':
    case '4':
    case '6':
        if (targetPosition->z < 0.0f) {
            Matrix4x4::preTranslate(0.5f, 0.0f, -0.55000001f);
            g_doorIngress.rotation.y += 0x8000;
            g_doorIngress.side = 1;
            break;
        }
        Matrix4x4::preTranslate(0.5f, 0.0f, 0.55000001f);
        g_doorIngress.side = 0;
        break;
    case '5':
    case '7':
        if (targetPosition->z < 0.0f) {
            if (targetPosition->x > 0.44999999f)
                Matrix4x4::preTranslate(0.44999999f, 0.0f, -0.44999999f);
            else if (targetPosition->x < -0.44999999f)
                Matrix4x4::preTranslate(-0.44999999f, 0.0f, -0.44999999f);
            else
                Matrix4x4::preTranslate(targetPosition->x, 0.0f, -0.44999999f);
            g_doorIngress.rotation.y += 0x8000;
            g_doorIngress.side = 1;
            break;
        }
        if (targetPosition->x > 0.44999999f)
            Matrix4x4::preTranslate(0.44999999f, 0.0f, 0.44999999f);
        else if (targetPosition->x < -0.44999999f)
            Matrix4x4::preTranslate(-0.44999999f, 0.0f, 0.44999999f);
        else
            Matrix4x4::preTranslate(targetPosition->x, 0.0f, 0.44999999f);
        g_doorIngress.side = 0;
        break;
    }
    matrix_stack::GetTranslation(targetPosition);
    g_doorIngress.position = *targetPosition;
    PopCurrentMatrix(1);
    return true;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
bool ComputeDoorEgress(std::int32_t doorId, const Vector3 *currentPosition, Vector3 *targetPosition) {
    g_doorIngress.selectedDoor = 0;
    const DoorTaskEntry *const entry = FindDoorEntry(shen_name(static_cast<std::uint32_t>(doorId)));
    if (!entry)
        return false;
    g_doorIngress.selectedDoor = shen_name(static_cast<std::uint32_t>(doorId));
    matrix_stack::Matrix4x4_Push();
    matrix_stack::LoadMatrix(&entry->worldTransform);
    *targetPosition = *currentPosition;
    InverseTransformPointByMatrix(&entry->worldTransform, targetPosition);
    ExtractMatrixEulerAnglesFixed(&entry->worldTransform, &g_doorIngress.rotation);
    switch (static_cast<char>(static_cast<std::uint32_t>(doorId) >> 8U)) {
    case '1':
        if (targetPosition->z > 0.0f) {
            Matrix4x4::preTranslate(0.5f, 0.0f, -0.55000001f);
            g_doorIngress.rotation.y += 0x8000;
            g_doorIngress.side = 0;
            break;
        }
        Matrix4x4::preTranslate(0.55000001f, 0.0f, 0.80000001f);
        g_doorIngress.side = 1;
        break;
    case '2':
    case '3':
    case '4':
    case '6':
        if (targetPosition->z > 0.0f) {
            Matrix4x4::preTranslate(0.5f, 0.0f, -0.55000001f);
            g_doorIngress.rotation.y += 0x8000;
            g_doorIngress.side = 0;
            break;
        }
        Matrix4x4::preTranslate(0.5f, 0.0f, 0.55000001f);
        g_doorIngress.side = 1;
        break;
    case '5':
    case '7':
        if (targetPosition->z > 0.0f) {
            if (targetPosition->x > 0.44999999f)
                Matrix4x4::preTranslate(0.44999999f, 0.0f, -0.55000001f);
            else if (targetPosition->x < -0.44999999f)
                Matrix4x4::preTranslate(-0.44999999f, 0.0f, -0.55000001f);
            else
                Matrix4x4::preTranslate(targetPosition->x, 0.0f, -0.55000001f);
            g_doorIngress.rotation.y += 0x8000;
            g_doorIngress.side = 0;
            break;
        }
        if (targetPosition->x > 0.44999999f)
            Matrix4x4::preTranslate(0.44999999f, 0.0f, 0.55000001f);
        else if (targetPosition->x < -0.44999999f)
            Matrix4x4::preTranslate(-0.44999999f, 0.0f, 0.55000001f);
        else
            Matrix4x4::preTranslate(targetPosition->x, 0.0f, 0.55000001f);
        g_doorIngress.side = 1;
        break;
    }
    matrix_stack::GetTranslation(targetPosition);
    g_doorIngress.position = *targetPosition;
    PopCurrentMatrix(1);
    return true;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t ClaimDoor(shen_name subject, shen_name claimant) {
    DoorTaskEntry *const entry = FindDoorEntry(subject);
    if (!entry)
        return 0;
    const std::uint32_t state = entry->flags & 3U;
    if (state == 2U)
        return 1;
    if (state == 0) {
        if (claimant != SHEN_NAME(DOOR)) {
            const MapControlData *const first = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
            const MapControlData *const second = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
            Character *const character = scenFindChara(second->scenarioTask, claimant);
            if (first->ryo != character && subject == (*g_doorEntryHead->sourcePair) && kDoorPlayerClaimDistanceSquared > g_doorEntryHead->distanceSquared)
                return 0;
            if (entry->claimant && claimant != entry->claimant)
                return 0;
        }
        entry->claimant = claimant;
        return 1;
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t ReleaseDoor(shen_name subject, shen_name claimant) {
    DoorTaskEntry *const entry = FindDoorEntry(subject);
    if (!entry)
        return 0;
    if (!(entry->flags & 3U)) {
        if (entry->claimant && claimant != entry->claimant)
            return 0;
        entry->claimant = __typeof__(entry->claimant)();
        return 1;
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t ReleaseActiveDoorClaims(shen_name claimant) {
    DoorTaskEntry *entry = g_doorTaskEntries;
    if (!g_doorDefinitionTable || !entry)
        return 0;
    for (std::int32_t index = 0; index < g_doorDefinitionTable->count; ++index, ++entry) {
        if ((entry->flags & 3U) && entry->claimant == claimant)
            entry->claimant = 0;
    }
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void drBeginEntry(shen_name subject) {
    g_doorEntryState = 0;
    if (!scenEvaluateCurrentSequence(subject, 0)) {
        g_doorEntrySubject = subject;
        g_doorEntryAction = GetDoorInteractionAction(subject);
    } else {
        const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        ReleaseDoor(subject, ctrlGetScenarioCharaName(control->ryo));
        plymFinishRegionInteraction();
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t BeginDoorAnimation(std::int32_t doorId, shen_name claimant, std::int32_t animationId) {
    return BeginDoorAnimation(doorId, claimant, animationId, 0.0f, 1.0f);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t BeginDoorAnimation(std::int32_t doorId, shen_name claimant, std::int32_t animationId, float frame, float speed) {
    DoorTaskEntry *const entry = FindDoorEntry(shen_name(static_cast<std::uint32_t>(doorId)));
    if (!entry)
        return 0;
    if (claimant != entry->claimant && static_cast<std::int8_t>(static_cast<std::uint32_t>(doorId) >> 8U) != '7')
        return 0;
    if (static_cast<std::int8_t>(static_cast<std::uint32_t>(doorId) >> 8U) == '7')
        return 1;
    entry->animationId = GetDoorLeafAnimation(animationId);
    TimedNodeMotion *const state = &entry->motion;
    MT7Node *const node = &entry->motionNode;
    InitializeTimedNodeMotion(state, node, reinterpret_cast<const TimedNodeMotionHeader *>(MOTN_GetDataForAnimId(entry->animationId)));
    entry->motion.speed *= speed;
    entry->motion.frame = frame;
    AdvanceDoorMotion(entry);
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t doorTriggerTransition(shen_name subject, shen_name claimant) {
    std::int32_t result = 0;
    DoorTaskEntry *const entry = FindDoorEntry(subject);
    if (!entry)
        return 0;
    const MapControlData *const first = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(first->scenarioTask, claimant);
    const MapControlData *const second = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (second->ryo != character && claimant != SHEN_NAME(DOOR))
        return result;
    if (g_doorCurrentStream != entry->routeMode && entry->routeMode)
        return result;
    if (entry->requiredEntry) {
        ++g_doorRuntimeState.transitionState;
        g_doorRuntimeState.temporaryAuthor = entry->requiredEntry;
        g_doorRuntimeState.destinationDoor = subject;
        result = 1;
        goto finish;
    }
    if (entry->routeCondition) {
        if (entry->routeCondition == SHEN_NAME(QCCE)) {
            if (g_doorCurrentStream == 3)
                entry->routeSense = 1;
            if (g_doorCurrentStream == 4)
                entry->routeSense = 2;
        }
        ++g_doorDestination.phase;
        g_doorDestination.area = entry->routeCondition;
        g_doorDestination.entry = entry->routeSense;
        g_doorDestination.region = subject;
        result = 1;
        goto finish;
    }
    if (entry->callback)
        entry->callback(subject);
    else
        DoorTransitionNullsub(subject);
finish:
    return result;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t doorPlayRigidMotionRange(shen_name subject, std::int32_t motionId, float startFrame, float speed, float endFrame) {
    DoorTaskEntry *const entry = FindDoorEntry(subject);
    if (!entry)
        return 0;
    entry->animationId = motionId;
    TimedNodeMotion *const state = &entry->motion;
    MT7Node *const node = &entry->motionNode;
    InitializeTimedNodeMotion(state, node, reinterpret_cast<const TimedNodeMotionHeader *>(MOTN_GetDataForAnimId(motionId)));
    entry->motion.speed *= speed;
    entry->motion.frame = startFrame;
    if (endFrame > 0.0f)
        entry->motion.end = endFrame;
    AdvanceDoorMotion(entry);
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t doorPlayRigidMotion(shen_name subject, std::int32_t motionId, float startFrame, float speed) {
    if (!ClaimDoor(subject, SHEN_NAME(DOOR)))
        return 0;
    return doorPlayRigidMotionRange(subject, motionId, startFrame, speed, -1.0f);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t doorLoopRigidMotion(shen_name subject, std::int32_t motionId, float startFrame, float speed) {
    ClaimDoor(subject, SHEN_NAME(DOOR));
    return doorPlayRigidMotionRange(subject, motionId, startFrame, speed, -1.0f);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t SetDoorSubjectCallback(shen_name subject, DoorSubjectCallback callback) {
    DoorTaskEntry *const entry = FindDoorEntry(subject);
    if (!entry)
        return 0;
    entry->callback = callback;
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
DoorSubjectCallback GetDoorSubjectCallback(shen_name subject) {
    DoorTaskEntry *const entry = FindDoorEntry(subject);
    return entry ? entry->callback : 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SetDoorAreaCleanupCallback(DoorAreaCleanupCallback callback) {
    g_doorAreaCleanupCallback = callback;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void ExecuteCleanupFuncForAreaWithCachedScene() {
    if (g_doorAreaCleanupCallback)
        g_doorAreaCleanupCallback(g_doorCachedSceneId);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void ExecuteDoorAreaCleanup(std::int32_t sceneId, std::int32_t apply) {
    if (g_doorAreaCleanupCallback)
        g_doorAreaCleanupCallback(sceneId);
    if (apply) {
        g_doorCachedSceneId = sceneId;
        ApplyDoorSceneState(sceneId);
    }
}
#endif

// @verified - d
void ResetNpcScheduleSeeds() {
    npcScheduleSeedNames[0] = 0;
    npcScheduleSeedNames[1] = 0;
    hasPendingNpcScheduleSeeds = 0;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t GetNpcDoorAnimation(std::int32_t doorId, Character *character, const Vector3 *position) {
    const std::int32_t rig = charGetMotionRigType(character);
    std::uint16_t motion = 0;
    Vector3 target;
    g_doorIngress.selectedDoor = 0;
    ComputeDoorIngress(doorId, position, &target);
    if (!g_doorIngress.selectedDoor)
        return 0;
    {
        switch (static_cast<char>(static_cast<std::uint32_t>(doorId) >> 8U)) {
        case '1':
            if (!g_doorIngress.side) {
                switch (rig) {
                case 2:
                    motion = 0xf081;
                    break;
                case 3:
                case 4:
                    motion = 0xf0a1;
                    break;
                case 5:
                    motion = 0xf0e2;
                    break;
                case 6:
                    motion = 0xf071;
                    break;
                case 8:
                    motion = 0xf00c;
                    break;
                case 9:
                    motion = 0xf07b;
                    break;
                case 10:
                case 11:
                    motion = 0xf05b;
                    break;
                case 0:
                case 1:
                case 13:
                case 14:
                    motion = 0xf050;
                    break;
                default:
                    motion = 0xf097;
                    break;
                }
            } else {
                switch (rig) {
                case 2:
                    motion = 0xf082;
                    break;
                case 3:
                case 4:
                    motion = 0xf0a2;
                    break;
                case 5:
                    motion = 0xf0e5;
                    break;
                case 6:
                    motion = 0xf074;
                    break;
                case 8:
                    motion = 0xf010;
                    break;
                case 9:
                    motion = 0xf07c;
                    break;
                case 10:
                case 11:
                    motion = 0xf05c;
                    break;
                case 0:
                case 1:
                case 13:
                case 14:
                    motion = 0xf053;
                    break;
                default:
                    motion = 0xf09a;
                    break;
                }
            }
            break;
        case '2':
        case '4':
        case '6':
            if (!g_doorIngress.side) {
                switch (rig) {
                case 3:
                case 4:
                case 5:
                    motion = 0xf0e4;
                    break;
                case 6:
                    motion = 0xf073;
                    break;
                case 8:
                    motion = 0xf00f;
                    break;
                case 0:
                case 1:
                case 2:
                case 13:
                case 14:
                    motion = 0xf052;
                    break;
                default:
                    motion = 0xf099;
                    break;
                }
            } else {
                switch (rig) {
                case 3:
                case 4:
                case 5:
                    motion = 0xf0e3;
                    break;
                case 6:
                    motion = 0xf072;
                    break;
                case 8:
                    motion = 0xf00e;
                    break;
                case 0:
                case 1:
                case 2:
                case 13:
                case 14:
                    motion = 0xf051;
                    break;
                default:
                    motion = 0xf098;
                    break;
                }
            }
            break;
        case '3':
            if (!g_doorIngress.side) {
                switch (rig) {
                case 3:
                case 4:
                case 5:
                    motion = 0xf0e8;
                    break;
                case 6:
                    motion = 0xf076;
                    break;
                case 8:
                    motion = 0xf01f;
                    break;
                case 0:
                case 1:
                case 2:
                case 13:
                case 14:
                    motion = 0xf055;
                    break;
                default:
                    motion = 0xf09c;
                    break;
                }
            } else {
                switch (rig) {
                case 3:
                case 4:
                case 5:
                    motion = 0xf0e7;
                    break;
                case 6:
                    motion = 0xf075;
                    break;
                case 8:
                    motion = 0xf01c;
                    break;
                case 0:
                case 1:
                case 2:
                case 13:
                case 14:
                    motion = 0xf054;
                    break;
                default:
                    motion = 0xf09b;
                    break;
                }
            }
            break;
        case '5':
        case '7':
            switch (rig) {
            case 3:
            case 4:
            case 5:
                motion = 0xf0d2;
                break;
            case 6:
                motion = 0xf061;
                break;
            case 7:
            case 9:
            case 10:
            case 11:
            case 12:
                motion = 0xf087;
                break;
            case 0:
            case 1:
            case 2:
            case 13:
            case 14:
                motion = 0xf03f;
                break;
            default:
                motion = SelectNpcWalkingMotion(character);
                break;
            }
            break;
        }
    }
    return motion;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t drGetAnswerSide(std::uint32_t *subject) {
    const std::int32_t side = g_doorAnswerSide;
    if (side) {
        if (subject)
            *subject = g_doorEntrySubject;
    } else if (subject) {
        *subject = 0;
    }
    return side;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
DoorDefinitionTable *GetDoorDefinitionTable() {
    return g_doorDefinitionTable;
}

shen_name FindDoorSecondaryName(shen_name primary) {
    const DoorDefinitionTable *const table = g_doorDefinitionTable;
    const shen_name *pair = reinterpret_cast<const shen_name *>(table + 1);
    const std::int32_t count = table->count;
    if (!table)
        return 0;
    for (std::int32_t index = 0; index < count; ++index, pair += 2) {
        if (pair[0] == primary)
            return pair[1];
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
const DoorAreaMetadata *GetDataForCurrentMapAndScene(shen_name subject) {
    typedef std::uint32_t(__attribute__((renesas)) * DoorDataHeader)(std::uint32_t map);
    DoorDataHeader header;
    std::memcpy(&header, &g_doorDataCursor, sizeof(header));
    const volatile DoorAreaMetadata *entry = reinterpret_cast<const volatile DoorAreaMetadata *>(header(g_currentMap));
    std::int32_t radix = 10;
    if (entry) {
        entry = reinterpret_cast<const volatile DoorAreaMetadata *>(reinterpret_cast<std::uintptr_t>(g_doorData) + reinterpret_cast<std::uintptr_t>(entry));
        const std::int32_t number = radix * (static_cast<std::int32_t>((subject >> 16U) & 0xffU) - '0') + static_cast<std::int32_t>(subject >> 24U) - '0';
        goto testNumber;
    nextEntry:
        ++entry;
    testNumber:
        if (number != entry->number && entry->number)
            goto nextEntry;
        if (entry->number)
            return const_cast<const DoorAreaMetadata *>(entry);
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t GetDoorAreaFlags(shen_name subject) {
    const DoorAreaMetadata *const metadata = GetDataForCurrentMapAndScene(subject);
    return metadata ? metadata->flags : 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t GetDoorOpeningState(shen_name subject) {
    const DoorAreaMetadata *const metadata = GetDataForCurrentMapAndScene(subject);
    if (!metadata)
        return 2;
    DateTime date;
    shenGetCurrentDateTime(date);
    std::uint32_t time = 100 * date.hour;
    time += date.minute;
    if (metadata->opens == metadata->closes)
        return 3;
    std::uint32_t firstStart, firstEnd, secondStart, secondEnd;
    if (metadata->opens <= metadata->closes) {
        secondStart = firstStart = metadata->opens;
        secondEnd = firstEnd = metadata->closes;
    } else {
        secondStart = 0;
        firstStart = metadata->opens;
        firstEnd = 2400;
        secondEnd = metadata->closes;
    }
    if ((firstStart < time && time < firstEnd) || (secondStart < time && time < secondEnd))
        return 1;
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t IsDoorOpeningAllowed(shen_name subject) {
    return GetDoorOpeningState(subject) != 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t GetDoorInteractionAction(shen_name subject) {
    const DoorAreaMetadata *const metadata = GetDataForCurrentMapAndScene(subject);
    if (!metadata)
        return 0;
    const std::int32_t state = GetDoorOpeningState(subject);
    if (state != 0 && state != 3) {
        const std::int16_t *rule = reinterpret_cast<const std::int16_t *>(metadata->flagRulesOffset);
        if (rule) {
            rule = reinterpret_cast<const std::int16_t *>(reinterpret_cast<std::uintptr_t>(rule) + reinterpret_cast<std::uintptr_t>(g_doorData));
            while (rule[0]) {
                if (EV_GetShenmueFlag(rule[0])) {
                    if (reinterpret_cast<const std::int8_t *>(rule)[2] == 0)
                        return 0x10;
                    const std::int8_t answer = reinterpret_cast<const std::int8_t *>(rule)[2];
                    if (answer == 1)
                        return 0x11;
                    if (answer == 2)
                        return 0x12;
                    if (answer == 3)
                        return 0x13;
                }
                rule += 2;
            }
        }
        if (!metadata->knock)
            return 0;
        if (static_cast<std::int8_t>(metadata->knock) == 1)
            return 1;
    } else {
        if (static_cast<std::int8_t>(metadata->answer) == 0)
            return 0x10;
        if (static_cast<std::int8_t>(metadata->answer) == 1)
            return 0x11;
        if (static_cast<std::int8_t>(metadata->answer) == 2)
            return 0x12;
        if (static_cast<std::int8_t>(metadata->answer) == 3)
            return 0x13;
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t GetDoorExitSide(shen_name subject) {
    return GetDoorAreaFlags(subject) & 3U;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::uint32_t GetDoorAnswerFlag(shen_name subject) {
    return GetDoorAreaFlags(subject) & 4U;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void UnlinkDoorEntry(shen_name subject) {
    DoorTaskEntry *const entry = FindDoorEntry(subject);
    if (!entry)
        return;
    entry->flags |= 4U;
    if (entry->previous)
        entry->previous->next = entry->next;
    else
        g_doorEntryHead = entry->next;
    if (entry->next)
        entry->next->previous = entry->previous;
    else
        g_doorEntryTail = entry->previous;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void UnlinkExcludedDoors() {
    const shen_name map = g_currentMap;
    if (map != SHEN_NAME(WS00))
        return;
    UnlinkDoorEntry(SHEN_NAME(R311));
    UnlinkDoorEntry(SHEN_NAME(R333));
    UnlinkDoorEntry(SHEN_NAME(R342));
    UnlinkDoorEntry(SHEN_NAME(R344));
    UnlinkDoorEntry(SHEN_NAME(R345));
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static std::uint32_t FindDoorNameByNumber(std::int8_t number) {
    DoorTaskEntry *entry = g_doorTaskEntries;
    if (!g_doorDefinitionTable || !entry)
        return 0;
    const std::uint32_t prefix = static_cast<std::uint32_t>(static_cast<std::int8_t>(number / 10) + '0') * 0x10000U +
        static_cast<std::uint32_t>(static_cast<std::int8_t>(number % 10) + '0') * 0x1000000U;
    for (std::int32_t index = 0; index < g_doorDefinitionTable->count; ++index, ++entry) {
        const shen_name name = *entry->sourcePair;
        if ((name & 0xffff0000U) == prefix)
            return name;
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void SetDoorEntrySortKey(float key, DoorTaskEntry *entry) {
    DoorTaskEntry *cursor = entry;
    entry->distanceSquared = key;
    if (!entry->previous) {
        while (cursor->next && cursor->next->distanceSquared < entry->distanceSquared)
            cursor = cursor->next;
    } else if (entry->distanceSquared < entry->previous->distanceSquared) {
        do {
            if (!cursor->previous)
                break;
            cursor = cursor->previous;
        } while (cursor->distanceSquared > entry->distanceSquared);
    } else if (!entry->next) {
        do {
            if (!cursor->previous)
                break;
            cursor = cursor->previous;
        } while (cursor->distanceSquared > entry->distanceSquared);
    } else if (entry->next->distanceSquared < entry->distanceSquared) {
        while (cursor->next && cursor->next->distanceSquared < entry->distanceSquared)
            cursor = cursor->next;
    }
    if (cursor != entry && cursor->next != entry) {
        if (entry->previous)
            entry->previous->next = entry->next;
        if (entry->next)
            entry->next->previous = entry->previous;
        entry->next = cursor->next;
        entry->previous = cursor;
        if (entry->next)
            entry->next->previous = entry;
        if (entry->previous)
            entry->previous->next = entry;
    }
    if (!entry->previous)
        g_doorEntryHead = entry;
    if (!entry->next)
        g_doorEntryTail = entry;
}
#endif

// @verified - d
DoorTaskEntry *FindDoorEntry(shen_name subject) {
    DoorTaskEntry *entry = g_doorTaskEntries;
    if (!g_doorDefinitionTable || !entry)
        return 0;
    for (std::int32_t index = 0; index < g_doorDefinitionTable->count; ++index, ++entry) {
        if ((*entry->sourcePair) == subject)
            return entry;
    }
    return 0;
}

// @verified - d
DoorTaskEntry *FindDoorByAuthor(shen_name author) {
    DoorTaskEntry *entry = g_doorTaskEntries;
    if (!g_doorDefinitionTable || !entry)
        return 0;
    for (std::int32_t index = 0; index < g_doorDefinitionTable->count; ++index, ++entry)
        if (entry->requiredEntry == author)
            return entry;
    return 0;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static std::int32_t SetDoorSecondaryNodeEnabled(shen_name subject, std::int32_t enabled) {
    DoorTaskEntry *const entry = FindDoorEntry(subject);
    if (!entry)
        return 0;
    if (entry->secondary)
        entry->secondary->kind = enabled == 0;
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t SetDoorAuthorSecondaryNodes(shen_name author, std::int32_t visible) {
    typedef std::uint32_t(__attribute__((renesas)) * DoorTableHeader)(std::uint32_t);
    DoorTableHeader header;
    std::memcpy(&header, &g_doorTableCursor, sizeof(header));
    const DoorAreaScript *script = reinterpret_cast<const DoorAreaScript *>(header(g_currentMap));
    if (!script)
        return 0;
    script = reinterpret_cast<const DoorAreaScript *>(reinterpret_cast<std::uintptr_t>(g_doorTableData) + reinterpret_cast<std::uintptr_t>(script));
    while (author != script->condition) {
        if (script->eventId == -1)
            return 0;
        ++script;
    }
    const shen_name first = FindDoorNameByNumber(script->doorNumbers[0]);
    if (first)
        SetDoorSecondaryNodeEnabled(first, visible);
    const shen_name second = FindDoorNameByNumber(script->doorNumbers[1]);
    if (second)
        SetDoorSecondaryNodeEnabled(second, visible);
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DoorAdmission_Callback(void *) {
    typedef std::uint32_t(__attribute__((renesas)) * DoorTableHeader)(std::uint32_t);
    DoorTableHeader header;
    std::int32_t count = 0;
    std::memcpy(&header, &g_doorTableCursor, sizeof(header));
    const DoorAreaScript *script = reinterpret_cast<const DoorAreaScript *>(header(g_currentMap));
    if (script) {
        script = reinterpret_cast<const DoorAreaScript *>(reinterpret_cast<std::uintptr_t>(g_doorTableData) + reinterpret_cast<std::uintptr_t>(script));
        for (; script->eventId != -1; ++script) {
            if (script->eventCallback) {
                std::memcpy(&g_doorMapCallbacks[count], script, sizeof(MapEventCallbackEntry));
                g_doorMapCallbacks[count].callback = DoorMapEventNullsub;
                ++count;
            }
            if (script->conditionKind < 0) {
                for (std::int32_t leaf = 0; leaf < 2; ++leaf) {
                    const shen_name subject = FindDoorNameByNumber(script->doorNumbers[leaf]);
                    if (subject)
                        SetDoorRouteCondition(subject, script->condition, script->mode, -script->conditionKind);
                }
            } else if (script->conditionKind) {
                char filename[16], directory[16];
                std::sprintf(filename, "%s.PKF", GetDoorAuthorName(script->condition));
                std::sprintf(directory, "scene/%02d/%s", g_actNumber, GetDoorAuthorName(script->condition));
                if (shenFileExists(Filepath_Gen(FilepathGenMode::SceneDirectory), filename)) {
                    for (std::int32_t leaf = 0; leaf < 2; ++leaf) {
                        const shen_name subject = FindDoorNameByNumber(script->doorNumbers[leaf]);
                        if (subject)
                            SetDoorRequiredEntry(subject, script->condition, script->mode);
                    }
                } else if (SelectDiscDirectoryDefault(directory)) {
                    for (std::int32_t leaf = 0; leaf < 2; ++leaf) {
                        const shen_name subject = FindDoorNameByNumber(script->doorNumbers[leaf]);
                        if (subject)
                            SetDoorRouteCondition(subject, script->condition, script->mode, script->conditionKind);
                    }
                }
            }
        }
        std::memcpy(&g_doorMapCallbacks[count], script, sizeof(MapEventCallbackEntry));
        if (count)
            SetMapEventCallbacks(g_doorMapCallbacks);
    }
    HLib::SetCurrentTaskTo(DoorTask_Callback);
    shen_name *const queuedAuthor = &g_doorQueuedAuthor;
    if (*queuedAuthor) {
        LoadDoorAuthor(*queuedAuthor);
        *queuedAuthor = 0;
    }
}
#endif

// @verified - d
void SetDoorNodeHierarchyHidden(MT7Node *node, std::uint32_t hidden) {
    node->kind = static_cast<std::int16_t>(hidden);
    if (node->child)
        SetDoorNodeHierarchyHidden(node->child, hidden);
    if (node->sibling)
        SetDoorNodeHierarchyHidden(node->sibling, hidden);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void ApplyDoorSceneStateToSiblings(MT7Node *node, std::int32_t sceneId) {
    const std::uint32_t marker = GetModelMarker(node);
    if (static_cast<std::uint8_t>(marker) == 'H')
        node->kind = static_cast<std::int16_t>(sceneId);
    if (static_cast<std::uint8_t>(marker) == 'N')
        node->kind = sceneId == 0;
    if (node->child)
        ApplyDoorSceneStateToSiblings(node->child, sceneId);
    if (node->sibling)
        ApplyDoorSceneStateToSiblings(node->sibling, sceneId);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t ApplyDoorSceneState(std::int32_t sceneId) {
    DoorTaskEntry *entry = g_doorTaskEntries;
    if (!g_doorDefinitionTable || !entry)
        return 0;
    for (std::int32_t index = 0; index < g_doorDefinitionTable->count; ++index, ++entry)
        ApplyDoorSceneStateToSiblings(entry->primary, sceneId);
    RefreshDoorSceneStateForCurrentStep();
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void DrawDoorMotionNodes(MT7Node *node, float alpha) {
    if (node->sibling)
        matrix_stack::Matrix4x4_Push();
    Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
    Matrix4x4::SetViewMatrixFromEuler(node->rotation.x, node->rotation.y, node->rotation.z);
    if (node->mesh && node->kind == 0) {
        if (reinterpret_cast<const std::uint32_t *>(node->mesh)[1] & 4U)
            UpdateMT7MeshTextureCoordinates(node->mesh);
        dc::sdk::DrawSingleLightMT7MeshAlpha(node->mesh, alpha);
    }
    if (node->child)
        DrawDoorMotionNodes(node->child, alpha);
    if (node->sibling) {
        PopCurrentMatrix(1);
        DrawDoorMotionNodes(node->sibling, alpha);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void PublishDoorMotionNodes(void *) {
    DoorTaskEntry *entry = g_doorTaskEntries;
    if (!g_doorDefinitionTable || !entry)
        return;
    const std::int32_t previous = dc::sdk::GetMT7VertexColourType();
    dc::sdk::SetMT7VertexColourType(0);
    for (std::int32_t index = 0; index < g_doorDefinitionTable->count; ++index, ++entry) {
        if (1.0f > entry->motionScale) {
            MT7Node *const sibling = entry->motionNode.sibling;
            entry->motionNode.sibling = 0;
            entry->primary->child = 0;
            matrix_stack::Matrix4x4_Push();
            Matrix4x4::MultiplyWithPrePassMatrix(&entry->worldTransform);
            DrawDoorMotionNodes(&entry->motionNode, entry->motionScale);
            PopCurrentMatrix(1);
            entry->motionNode.sibling = sibling;
        } else {
            entry->primary->child = &entry->motionNode;
        }
    }
    dc::sdk::SetMT7VertexColourType(previous);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static std::int32_t CountDoorNearbyCharacter(Character *character, void *context) {
    Vector3 position;
    GetCharacterWorldPosition(character, &position);
    Vector3 *const origin = static_cast<Vector3 *>(context);
    position.x -= origin->x;
    position.y -= origin->y;
    position.z -= origin->z;
    if (1.9320999f > position.x * position.x + position.y * position.y + position.z * position.z) {
        const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        if (control->ryo == character && !g_doorAutomaticOpeningAllowed) {
            GetCharacterWorldPosition(control->ryo, &position);
            InverseTransformPointByMatrix(g_doorAutomaticTransform, &position);
            if (g_doorAutomaticExitSide && position.z > 0.0f)
                return 1;
            if (!g_doorAutomaticExitSide && 0.0f > position.z)
                return 1;
        }
        ++g_doorNearbyCharacterCount;
    }
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void DoorTask_Callback(void *) {
    DoorTaskEntry *entry = g_doorTaskEntries;
    if (!g_doorDefinitionTable || !entry)
        return;
    DateTime date;
    shenGetCurrentDateTime(date);
    std::int32_t *const cachedSceneId = &g_doorCachedSceneId;
    if (date.hour > 18 && !*cachedSceneId) {
        *cachedSceneId = 1;
        ApplyDoorSceneState(1);
    }
    std::int32_t stream = GetQronNextStream();
    if (stream == -1000)
        stream = 1;
    if (stream != g_doorCurrentStream) {
        g_doorCurrentStream = stream;
        TransformPropObject();
        RefreshDoorSceneStateForCurrentStep();
    }
    Vector3 position;
    plymUpdatePlayerSearchLocation(&position);
    Vector3 delta;
    for (std::int32_t index = 0; index < g_doorDefinitionTable->count; ++index, ++entry) {
        delta.x = position.x - entry->interactionOrigin.x;
        delta.y = position.y - entry->interactionOrigin.y;
        delta.z = position.z - entry->interactionOrigin.z;
        if (!IsDoorSubjectEnabled(*entry->sourcePair))
            SetDoorEntrySortKey(1000.0f, entry);
        else if (!(entry->flags & 4U))
            SetDoorEntrySortKey((delta.x * delta.x + delta.y * delta.y) + delta.z * delta.z, entry);
        if (entry->primary && ((entry->flags & 3U) == 1U || (entry->flags & 3U) == 3U))
            AdvanceDoorMotion(entry);
        const shen_name subject = *entry->sourcePair;
        if (static_cast<std::int8_t>(subject >> 8U) == '7') {
            g_doorNearbyCharacterCount = 0;
            g_doorAutomaticOpeningAllowed = IsDoorOpeningAllowed(subject);
            g_doorAutomaticExitSide = GetDoorExitSide(*entry->sourcePair);
            g_doorAutomaticTransform = doorFindEntryTransform(*entry->sourcePair);
            ForEachLiveCharacterInMapOrder(&entry->interactionOrigin, CountDoorNearbyCharacter, 0);
            const std::int32_t state = entry->flags & 3U;
            if (g_doorNearbyCharacterCount) {
                if (!state) {
                    doorPlayRigidMotion(*entry->sourcePair, 0xf0aa, 0.0f, 1.0f);
                } else if (state == 3) {
                    entry->flags &= ~3U;
                    doorPlayRigidMotion(*entry->sourcePair, 0xf0aa, entry->motion.end - entry->motion.frame, 1.0f);
                }
            } else if (state == 2) {
                doorPlayRigidMotion(*entry->sourcePair, 0xf0a9, 0.0f, 1.0f);
            }
        }
    }
    if (static_cast<std::int8_t>((*g_doorEntryHead->sourcePair) >> 8U) == '7') {
        Vector3 local = position;
        InverseTransformPointByMatrix(&g_doorEntryHead->worldTransform, &local);
        if (*g_doorEntryHead->sourcePair == g_doorCrossingSubject &&
            ((g_doorPreviousCrossingZ < 0.0f && local.z > 0.0f) || (g_doorPreviousCrossingZ > 0.0f && local.z < 0.0f)) &&
            0.8f > g_doorEntryHead->distanceSquared && !IsDoorAuthorActive() && !IsDoorTransitionActive()) {
            BeginDoorAnimation(static_cast<std::int32_t>((*g_doorEntryHead->sourcePair)), SHEN_NAME(DOOR), 0, 0.0f, 1.0f);
            doorTriggerTransition(*g_doorEntryHead->sourcePair, SHEN_NAME(DOOR));
            SetPlayerControlEnabled(0);
            g_doorCrossingZ = local.z;
        }
        g_doorCrossingSubject = *g_doorEntryHead->sourcePair;
        g_doorPreviousCrossingZ = local.z;
    }
    if (g_doorEntrySubject)
        drUpdateEntry();
    UpdateDoorAuthorController();
    UpdateDoorDestinationController();
    HLib::ExecuteCallbackNow(PublishDoorMotionNodes, 0);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t drRunEntryScripts(std::int32_t bits) {
    if (bits) {
        for (std::int32_t index = 0; index < 5; ++index) {
            if (((bits >> index) & 1) && scenEvaluateCurrentSequence(g_doorEntrySubject, static_cast<std::int8_t>(index + 1))) {
                const MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
                charaToggleFlag(control->ryo, 1);
                control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
                toggleCCMflagsByAmt(control->ryo, 4U);
                control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
                toggleCCMflagsByAmt(control->ryo, 0x80U);
                control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
                ccmClearFlags(control->ryo, 0x800U);
                const shen_name subject = g_doorEntrySubject;
                control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
                ReleaseDoor(subject, ctrlGetScenarioCharaName(control->ryo));
                g_doorEntryState = 0;
                g_doorEntrySubject = 0;
                return 1;
            }
        }
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void camCtrlSwitchToEventCam(std::int32_t frames, const Vector3 *target, std::int32_t yaw) {
    g_doorCameraFrame = 0;
    g_doorCameraDuration = frames;
    camCtrlGetNextCamPos(&g_doorCameraStartPosition);
    camCtrlGetTargetPosition(&g_doorCameraStartTarget);
    camCtrlComputeOrbitCameraFromTarget(target, yaw, &g_doorCameraEndPosition, &g_doorCameraEndTarget);
    g_doorPreviousCameraState = camCtrlGetState();
    camCtrlSetState(9);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void RestoreDoorCameraState() {
    camCtrlSetState(g_doorPreviousCameraState);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void camCtrlApplyNextAndTargetPos() {
    ++g_doorCameraFrame;
    float progress = static_cast<float>(g_doorCameraFrame) / static_cast<float>(g_doorCameraDuration);
    if (0.0f > progress)
        progress = 0.0f;
    if (progress > 1.0f)
        progress = 1.0f;
    const float remaining = 1.0f - progress;
    Vector3 value;
    value.x = progress * g_doorCameraEndPosition.x + remaining * g_doorCameraStartPosition.x;
    value.y = progress * g_doorCameraEndPosition.y + remaining * g_doorCameraStartPosition.y;
    value.z = progress * g_doorCameraEndPosition.z + remaining * g_doorCameraStartPosition.z;
    camCtrlSetNextPosition(value);
    value.x = progress * g_doorCameraEndTarget.x + remaining * g_doorCameraStartTarget.x;
    value.y = progress * g_doorCameraEndTarget.y + remaining * g_doorCameraStartTarget.y;
    value.z = progress * g_doorCameraEndTarget.z + remaining * g_doorCameraStartTarget.z;
    camCtrlSetTargetPosition(value);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static std::int32_t ManageDoorEntryAlpha(std::int32_t nextState, std::int32_t scriptBits) {
    switch (g_doorEntryState) {
    case 0: {
        SetPlayerControlEnabled(0);
        shenToggleAreaFlags(1);
        ++g_doorEntryState;
        const std::uint32_t side = GetDoorExitSide(g_doorEntrySubject);
        DoorTaskEntry *const entry = FindDoorEntry(g_doorEntrySubject);
        if (!entry)
            return 1;
        Vector3 location;
        plymUpdatePlayerSearchLocation(&location);
        InverseTransformPointByMatrix(&entry->worldTransform, &location);
        if (side != 2U) {
            if (0.0f > location.z && side)
                g_doorEntryAction = 2;
            if (location.z > 0.0f && !side)
                g_doorEntryAction = 2;
        }
        break;
    }
    case 1:
        if (GetPlayerAreaControlState() == 1U) {
            shenToggleAreaFlags(0);
            const MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
            toggleCCMflagsByAmt(control->ryo, 0x800U);
            control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
            ccmClearFlags(control->ryo, 0x80U);
            g_doorEntryState = nextState;
            return drRunEntryScripts(scriptBits);
        }
        break;
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t drTryDoor(std::int32_t nextState, std::int32_t scriptBits) {
    switch (g_doorEntryState) {
    case 0x20: {
        Vector3 position;
        plymUpdatePlayerSearchLocation(&position);
        DoorTaskEntry *const entry = FindDoorEntry(g_doorEntrySubject);
        if (!entry)
            return 1;
        matrix_stack::Matrix4x4_Push();
        matrix_stack::LoadMatrix(&entry->worldTransform);
        InverseTransformPointByMatrix(&entry->worldTransform, &position);
        g_doorIngress.selectedDoor = g_doorEntrySubject;
        ExtractMatrixEulerAnglesFixed(&entry->worldTransform, &g_doorIngress.rotation);
        g_doorIngress.side = 0;
        if (0.0f > position.z) {
            g_doorIngress.leafSide = (entry->flags >> 3U) & 1U;
            Matrix4x4::preTranslate(0.5f, 0.0f, -0.75f);
            g_doorIngress.rotation.y += 0x8000;
        } else {
            g_doorIngress.leafSide = ((static_cast<std::int16_t>(entry->flags) >> 3) ^ 1) & 1;
            Matrix4x4::preTranslate(0.5f, 0.0f, 0.75f);
        }
        matrix_stack::GetTranslation(&position);
        PopCurrentMatrix(1);
        g_doorEntryMotion = 0xf00a;
        mtwkRequestPositionTransition(charFindMotionwork(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo), &position,
            g_doorIngress.rotation.y, g_doorEntryMotion, 11.0f);
        matrix_stack::PushUnitMatrix();
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        Matrix4x4::SetViewMatrixFromEuler(0, g_doorIngress.rotation.y, 0);
        SampleNativeMotionRoot(static_cast<std::uint16_t>(g_doorEntryMotion),
            mtwkGetRigBoneId(charFindMotionwork(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo)), -1.0f, &position);
        Matrix4x4::preTranslate(position.x, 0.0f, position.z);
        matrix_stack::GetTranslation(&position);
        PopCurrentMatrix(1);
        camCtrlSwitchToEventCam(30, &position, g_doorIngress.rotation.y);
        ++g_doorEntryState;
        break;
    }
    case 0x21:
        if (mtwkSetExtraFlag(charFindMotionwork(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo), 0x3a, 1))
            break;
        if (nextState != -1 && nextState != 0x50) {
            doorPlayRigidMotionRange(g_doorEntrySubject, 0xf0fc, 12.0f, 1.0f, -1.0f);
            g_doorEntryState += 2;
        } else {
            doorPlayRigidMotionRange(g_doorEntrySubject, 0xf0fc, 12.0f, 1.0f, 45.0f);
            ++g_doorEntryState;
        }
    case 0x22:
        if (!mtwkSetRegionFlag(charFindMotionwork(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo), 0x41, 1, LowerBody))
            break;
        g_doorEntryState = nextState;
        RestoreDoorCameraState();
        return drRunEntryScripts(scriptBits);
    case 0x23:
        if (46.0f > mtwkGetFrame(charFindMotionwork(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo), LowerBody))
            break;
        g_doorEntryState = nextState;
        RestoreDoorCameraState();
        return drRunEntryScripts(scriptBits);
    default:
        return 0;
    }
    camCtrlApplyNextAndTargetPos();
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static float GetDoorWalkMotionStart(std::int32_t motion) {
    switch (motion) {
    case 0xf010:
        return 16.0f;
    case 0xf00c:
    case 0xf00e:
        return 10.0f;
    case 0xf00f:
        return 6.0f;
    case 0xf01c:
    case 0xf01f:
        return 15.0f;
    }
    return 0.0f;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static std::int32_t drWalkThroughDoor(std::int32_t nextState, std::int32_t scriptBits) {
    Vector3 position, target;
    switch (g_doorEntryState) {
    case 0x10:
        plymUpdatePlayerSearchLocation(&position);
        ComputeDoorIngress(static_cast<std::int32_t>(g_doorEntrySubject), &position, &target);
        g_doorEntryMotion = GetNpcDoorAnimation(
            static_cast<std::int32_t>(g_doorEntrySubject), static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo, &position);
        if (static_cast<std::int8_t>(g_doorEntrySubject >> 8U) == '1') {
            matrix_stack::PushUnitMatrix();
            matrix_stack::LoadMatrix(doorFindEntryTransform(g_doorEntrySubject));
            if (g_doorEntryMotion == 0xf010)
                Matrix4x4::preTranslate(0.475f, 0.0f, -0.5f);
            else
                Matrix4x4::preTranslate(0.55f, 0.0f, 0.5f);
            matrix_stack::GetTranslation(&target);
            PopCurrentMatrix(1);
        }
        SelectMagicRoomForPlayer();
        camCtrlSwitchToEventCam(30, &target, g_doorIngress.rotation.y);
        mtwkRequestPositionTransition(charFindMotionwork(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo), &target,
            g_doorIngress.rotation.y, g_doorEntryMotion, GetDoorWalkMotionStart(g_doorEntryMotion));
        ++g_doorEntryState;
    case 0x11:
        if (!mtwkSetExtraFlag(charFindMotionwork(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo), 0x3a, 1)) {
            BeginDoorAnimation(static_cast<std::int32_t>(g_doorEntrySubject),
                ctrlGetScenarioCharaName(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo), g_doorEntryMotion,
                GetDoorWalkMotionStart(g_doorEntryMotion) + 1.0f, 1.0f);
            if (doorTriggerTransition(
                    g_doorEntrySubject, ctrlGetScenarioCharaName(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo))) {
                RestoreDoorCameraState();
                g_doorEntryState += 2;
            } else {
                RestoreDoorCameraState();
                plymUpdatePlayerSearchLocation(&position);
                matrix_stack::PushUnitMatrix();
                Matrix4x4::preTranslate(position.x, position.y, position.z);
                Matrix4x4::SetViewMatrixFromEuler(0, playerGetRotY(), 0);
                SampleNativeMotionRoot(static_cast<std::uint16_t>(g_doorEntryMotion),
                    mtwkGetRigBoneId(charFindMotionwork(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo)), -1.0f, &position);
                SampleNativeMotionRoot(static_cast<std::uint16_t>(g_doorEntryMotion),
                    mtwkGetRigBoneId(charFindMotionwork(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo)),
                    GetDoorWalkMotionStart(g_doorEntryMotion) + 1.0f, &target);
                Matrix4x4::preTranslate(position.x - target.x, 0.0f, position.z - target.z);
                matrix_stack::GetTranslation(&target);
                PopCurrentMatrix(1);
                camCtrlSwitchToEventCam(static_cast<std::int32_t>(static_cast<float>(GetMotionFrameCount(static_cast<std::uint16_t>(g_doorEntryMotion))) -
                                            GetDoorWalkMotionStart(g_doorEntryMotion)),
                    &target, playerGetRotY());
                ++g_doorEntryState;
            }
        }
        break;
    case 0x12: {
        const float frame = mtwkGetFrame(charFindMotionwork(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo), LowerBody);
        const float start = GetDoorWalkMotionStart(g_doorEntryMotion);
        if ((start + static_cast<float>(GetMotionFrameCount(static_cast<std::uint16_t>(g_doorEntryMotion)))) * 0.5f > frame)
            break;
        FindDoorEntry(g_doorEntrySubject)->motionScale -=
            2.0f / (static_cast<float>(GetMotionFrameCount(static_cast<std::uint16_t>(g_doorEntryMotion))) - GetDoorWalkMotionStart(g_doorEntryMotion));
        if (mtwkSetRegionFlag(charFindMotionwork(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo), 0x41, 1, LowerBody)) {
            FindDoorEntry(g_doorEntrySubject)->motionScale = 1.0f;
            toggleCCMflagsByAmt(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo, 0x80U);
            ccmClearFlags(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo, 0x800U);
            SetPlayerControlEnabled(1);
            g_doorEntryState = nextState;
            g_doorEntrySubject = 0;
            RestoreDoorCameraState();
            if (scriptBits == 0x20)
                LeaveMagicRoom();
            return drRunEntryScripts(scriptBits);
        }
        break;
    }
    case 0x13:
        ccmClearFlags(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo, 4U);
        charaToggleFlag(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo, 0);
        g_doorEntryState = nextState;
        g_doorEntrySubject = 0;
        return 1;
    default:
        return 0;
    }
    camCtrlApplyNextAndTargetPos();
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static std::int32_t drKnockOnDoor(std::int32_t nextState, std::int32_t variant, std::int32_t scriptBits) {
    const MapControlData *control;
    std::int32_t motion = 0;
    switch (g_doorEntryState) {
    case 0x30: {
        static const std::int32_t motions[6] = {0xf013, 0xf00d, 0xf011, 0xf012, 0xf01d, 0xf01e};
        Vector3 position;
        plymUpdatePlayerSearchLocation(&position);
        DoorTaskEntry *const entry = FindDoorEntry(g_doorEntrySubject);
        if (!entry)
            return 1;
        matrix_stack::Matrix4x4_Push();
        matrix_stack::LoadMatrix(&entry->worldTransform);
        InverseTransformPointByMatrix(&entry->worldTransform, &position);
        g_doorIngress.selectedDoor = g_doorEntrySubject;
        ExtractMatrixEulerAnglesFixed(&entry->worldTransform, &g_doorIngress.rotation);
        g_doorIngress.side = 0;
        if (0.0f > position.z) {
            g_doorEntryMotion = motions[(static_cast<std::int8_t>(g_doorEntrySubject >> 8U) - '1') * 2];
            g_doorIngress.leafSide = (entry->flags >> 3U) & 1U;
            Matrix4x4::preTranslate(0.5f, 0.0f, -0.75f);
            g_doorIngress.rotation.y += 0x8000;
        } else {
            g_doorEntryMotion = motions[(static_cast<std::int8_t>(g_doorEntrySubject >> 8U) - '1') * 2 + 1];
            g_doorIngress.leafSide = ((static_cast<std::int16_t>(entry->flags) >> 3) ^ 1) & 1;
            Matrix4x4::preTranslate(0.5f, 0.0f, 0.75f);
        }
        matrix_stack::GetTranslation(&position);
        PopCurrentMatrix(1);
        control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        mtwkRequestPositionTransition(charFindMotionwork(control->ryo), &position, g_doorIngress.rotation.y, g_doorEntryMotion, variant ? 17.0f : 75.0f);
        matrix_stack::PushUnitMatrix();
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        Matrix4x4::SetViewMatrixFromEuler(0, g_doorIngress.rotation.y, 0);
        SampleNativeMotionRoot(static_cast<std::uint16_t>(g_doorEntryMotion),
            mtwkGetRigBoneId(charFindMotionwork(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo)), -1.0f, &position);
        Matrix4x4::preTranslate(position.x, 0.0f, position.z);
        matrix_stack::GetTranslation(&position);
        PopCurrentMatrix(1);
        camCtrlSwitchToEventCam(30, &position, g_doorIngress.rotation.y);
        ++g_doorEntryState;
    }
    case 0x31:
        control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        if (!mtwkSetExtraFlag(charFindMotionwork(control->ryo), 0x3a, 1)) {
            switch (g_doorEntryMotion) {
            case 0xf013:
                motion = 0xf0fe;
                break;
            case 0xf00d:
                motion = 0xf0fd;
                break;
            case 0xf011:
                motion = 0xf0f8;
                break;
            case 0xf012:
                motion = 0xf0f9;
                break;
            case 0xf01d:
                motion = 0xf0fa;
                break;
            case 0xf01e:
                motion = 0xf0fb;
                break;
            }
            doorPlayRigidMotion(g_doorEntrySubject, motion, variant ? 18.0f : 76.0f, 1.0f);
            control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
            if (mtwkSetRegionFlag(charFindMotionwork(control->ryo), 0x41, 1, LowerBody)) {
                g_doorEntryState = nextState;
                RestoreDoorCameraState();
                return drRunEntryScripts(scriptBits);
            }
        }
        break;
    default:
        return 0;
    }
    camCtrlApplyNextAndTargetPos();
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static std::int32_t drSetAnswerMotion(std::int32_t motion) {
    std::int32_t side = 0;
    std::int32_t doorMotion;
    float frame;
    switch (motion) {
    case 0xf053:
        doorMotion = 0xf103;
        frame = 89.0f;
        side = 1;
        break;
    case 0xf05c:
    case 0xf07c:
        doorMotion = 0xf10d;
        frame = 60.0f;
        side = 1;
        break;
    case 0xf074:
        doorMotion = 0xf109;
        frame = 125.0f;
        side = 1;
        break;
    case 0xf082:
        doorMotion = 0xf10f;
        frame = 95.0f;
        side = 1;
        break;
    case 0xf09a:
        doorMotion = 0xf113;
        frame = 136.0f;
        side = 1;
        break;
    case 0xf0a2:
        doorMotion = 0xf117;
        frame = 86.0f;
        side = 1;
        break;
    case 0xf0e5:
        doorMotion = 0xf11b;
        frame = 79.0f;
        side = 1;
        break;
    case 0xf050:
        doorMotion = 0xf100;
        frame = 85.0f;
        side = 2;
        break;
    case 0xf05b:
    case 0xf07b:
        doorMotion = 0xf10c;
        frame = 66.0f;
        side = 2;
        break;
    case 0xf071:
        doorMotion = 0xf106;
        frame = 90.0f;
        side = 2;
        break;
    case 0xf081:
        doorMotion = 0xf10e;
        frame = 80.0f;
        side = 2;
        break;
    case 0xf097:
        doorMotion = 0xf110;
        frame = 100.0f;
        side = 2;
        break;
    case 0xf0a1:
        doorMotion = 0xf116;
        frame = 93.0f;
        side = 2;
        break;
    case 0xf0e2:
        doorMotion = 0xf118;
        frame = 93.0f;
        side = 2;
        break;
    case 0xf051:
        doorMotion = 0xf101;
        frame = 61.0f;
        side = 3;
        break;
    case 0xf072:
        doorMotion = 0xf107;
        frame = 90.0f;
        side = 3;
        break;
    case 0xf098:
        doorMotion = 0xf111;
        frame = 58.0f;
        side = 3;
        break;
    case 0xf0e3:
        doorMotion = 0xf119;
        frame = 51.0f;
        side = 3;
        break;
    case 0xf052:
        doorMotion = 0xf102;
        frame = 56.0f;
        side = 4;
        break;
    case 0xf073:
        doorMotion = 0xf108;
        frame = 84.0f;
        side = 4;
        break;
    case 0xf099:
        doorMotion = 0xf112;
        frame = 50.0f;
        side = 4;
        break;
    case 0xf0e4:
        doorMotion = 0xf11a;
        frame = 52.0f;
        side = 4;
        break;
    case 0xf054:
        doorMotion = 0xf104;
        frame = 44.0f;
        side = 5;
        break;
    case 0xf075:
        doorMotion = 0xf10a;
        frame = 72.0f;
        side = 5;
        break;
    case 0xf09b:
        doorMotion = 0xf114;
        frame = 42.0f;
        side = 5;
        break;
    case 0xf0e7:
        doorMotion = 0xf11c;
        frame = 51.0f;
        side = 5;
        break;
    case 0xf055:
        doorMotion = 0xf105;
        frame = 46.0f;
        side = 6;
        break;
    case 0xf076:
        doorMotion = 0xf10b;
        frame = 70.0f;
        side = 6;
        break;
    case 0xf09c:
        doorMotion = 0xf115;
        frame = 52.0f;
        side = 6;
        break;
    case 0xf0e8:
        doorMotion = 0xf11d;
        frame = 53.0f;
        side = 6;
        break;
    default:
        doorMotion = 0xf110;
        frame = 100.0f;
        break;
    }
    g_doorAnswerDoorMotion = doorMotion;
    g_doorAnswerSide = side;
    g_doorAnswerFrame = frame;
    return doorMotion;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static std::int32_t drOccupantAnswers(std::int32_t nextState) {
    const MapControlData *control;
    Character *character;
    MotionWork *work;
    DoorTaskEntry *entry;
    Vector3 position;
    float frame;
    switch (g_doorEntryState) {
    case 0x50:
        entry = FindDoorEntry(g_doorEntrySubject);
        if (!entry) {
            g_doorEntryState = nextState;
            return 1;
        }
        GetDoorAnswerFlag(g_doorEntrySubject);
        if (GetDoorAnswerFlag(g_doorEntrySubject)) {
            const std::int32_t wait = static_cast<std::int32_t>(npcFindAreaPointOccupant(g_doorEntrySubject));
            g_doorEntryWait = wait;
            if (!wait) {
                g_doorEntryState = nextState;
                return 1;
            }
        } else {
            g_doorEntryState = nextState;
            return 1;
        }
        plymUpdatePlayerSearchLocation(&position);
        matrix_stack::Matrix4x4_Push();
        matrix_stack::LoadMatrix(&entry->worldTransform);
        InverseTransformPointByMatrix(&entry->worldTransform, &position);
        g_doorIngress.selectedDoor = g_doorEntrySubject;
        ExtractMatrixEulerAnglesFixed(&entry->worldTransform, &g_doorIngress.rotation);
        if ((static_cast<std::int8_t>(g_doorEntrySubject >> 8U) == '1' && position.z > 0.0f) || static_cast<std::int8_t>(g_doorEntrySubject >> 8U) == '2') {
            float offset = 1.0f;
            if (0.0f > position.z) {
                Matrix4x4::preTranslate(0.8f, 0.0f, -offset);
                g_doorIngress.rotation.y += 0x8000;
            } else {
                Matrix4x4::preTranslate(0.8f, 0.0f, offset);
            }
            matrix_stack::GetTranslation(&position);
            control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
            mtwkRequestPositionTransition(charFindMotionwork(control->ryo), &position, g_doorIngress.rotation.y, 0xf002, 0.0f);
        } else {
            control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
            mtwkRequestMotion(charFindMotionwork(control->ryo), 0xf002, LowerBody);
        }
        PopCurrentMatrix(1);
        EV_CheckWarmStartNpcRoute(g_doorEntryWait);
        SetNpcSpecialRouteQueryPaused(g_doorEntryWait, 1);
        if (!EV_ForceLoadNpc(g_doorEntryWait)) {
            EV_NPC_NewLoadSw(0);
            EV_ResetFreeTalkFlags();
        }
        plymUpdatePlayerSearchLocation(&position);
        matrix_stack::Matrix4x4_Push();
        matrix_stack::LoadMatrix(&entry->worldTransform);
        InverseTransformPointByMatrix(&entry->worldTransform, &position);
        ExtractMatrixEulerAnglesFixed(&entry->worldTransform, &g_doorAnswerRotation);
        if (0.0f > position.z) {
            Matrix4x4::preTranslate(0.8f, 0.0f, 0.85f);
        } else {
            Matrix4x4::preTranslate(0.8f, 0.0f, -0.85f);
            g_doorAnswerRotation.y += 0x8000;
        }
        matrix_stack::GetTranslation(&position);
        PopCurrentMatrix(1);
        ComputeDoorIngress(static_cast<std::int32_t>(g_doorEntrySubject), &position, &g_doorAnswerPosition);
        ++g_doorEntryState;
        break;
    case 0x51:
        if (!EV_ForceLoadNpc(g_doorEntryWait))
            break;
        control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        character = scenFindChara(control->scenarioTask, g_doorEntryWait);
        work = charFindMotionwork(character);
        toggleCCMflagsByAmt(character, 0x800U);
        ccmClearFlags(character, 0x80U);
        ccmClearFlags(character, 4U);
        mtwkSetRegionFlag(work, 0x47, 0, LowerBody);
        SetCharacterWorldPosition(character, &g_doorAnswerPosition);
        charaSetWorldRot(character, g_doorAnswerRotation);
        g_doorAnswerMotion = GetNpcDoorAnimation(static_cast<std::int32_t>(g_doorEntrySubject), character, &g_doorAnswerPosition);
        mtwkRequestMotion(work, g_doorAnswerMotion, LowerBody);
        drSetAnswerMotion(g_doorAnswerMotion);
        mtwkSetSpeed(work, 0.9f, LowerBody);
        mtwkSetLayerEndFrame(work, g_doorAnswerFrame, LowerBody);
        doorLoopRigidMotion(g_doorEntrySubject, GetDoorLeafAnimation(g_doorAnswerMotion), 0.0f, 0.9f);
        ++g_doorEntryState;
        break;
    case 0x52:
        StartScenarioFreeTalkRecord(g_doorEntryWait, -1);
        ++g_doorEntryState;
    case 0x53:
        control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        character = scenFindChara(control->scenarioTask, g_doorEntryWait);
        work = charFindMotionwork(character);
        if (!mtwkSetRegionFlag(work, 0x41, 1, LowerBody))
            break;
        mtwkGetBonePosition(work, 5, &position);
        control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        mtwkUpdateWithPosition(charFindMotionwork(control->ryo), &position, 0);
        control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        mtwkGetBonePosition(charFindMotionwork(control->ryo), 5, &position);
        mtwkUpdateWithPosition(work, &position, 0);
        mtwkRequestMotion(work, g_doorAnswerDoorMotion, LowerBody);
        doorLoopRigidMotion(g_doorEntrySubject, GetDoorLeafAnimation(g_doorAnswerMotion), g_doorAnswerFrame, 0.0f);
        ++g_doorEntryState;
        break;
    case 0x54:
        if (IsScenarioBusy())
            break;
        reinterpret_cast<void (*)(shen_name)>(StopScenarioFreeTalkRecord)(g_doorEntryWait);
        control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        character = scenFindChara(control->scenarioTask, g_doorEntryWait);
        work = charFindMotionwork(character);
        mtwkRequestPositionTransition(work, &g_doorAnswerPosition, g_doorAnswerRotation.y, g_doorAnswerMotion, g_doorAnswerFrame);
        mtwkUpdateWithPosition(work, 0, 0);
        ++g_doorEntryState;
        break;
    case 0x55:
        control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        character = scenFindChara(control->scenarioTask, g_doorEntryWait);
        work = charFindMotionwork(character);
        if (mtwkSetExtraFlag(work, 0x3a, 1))
            break;
        mtwkSetSpeed(work, 0.0f, LowerBody);
        doorLoopRigidMotion(g_doorEntrySubject, GetDoorLeafAnimation(g_doorAnswerMotion), g_doorAnswerFrame - 1.0f, -1.0f);
        ++g_doorEntryState;
    case 0x56:
        control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        character = scenFindChara(control->scenarioTask, g_doorEntryWait);
        work = charFindMotionwork(character);
        frame = mtwkGetFrame(work, LowerBody) - 1.0f;
        if (1.0f > frame) {
            ++g_doorEntryState;
            break;
        }
        matrix_stack::PushUnitMatrix();
        Matrix4x4::preTranslate(g_doorAnswerPosition.x, g_doorAnswerPosition.y, g_doorAnswerPosition.z);
        Matrix4x4::SetViewMatrixFromEuler(g_doorAnswerRotation.x, g_doorAnswerRotation.y, g_doorAnswerRotation.z);
        SampleNativeMotionRoot(static_cast<std::uint16_t>(g_doorAnswerMotion), mtwkGetRigBoneId(work), frame - 1.0f, &position);
        Matrix4x4::preTranslate(position.x, 0.0f, position.z);
        matrix_stack::GetTranslation(&position);
        PopCurrentMatrix(1);
        mtwkSetRegionFlag(work, 0x47, 0, LowerBody);
        SetCharacterWorldPosition(character, &position);
        mtwkRequestMotion(work, g_doorAnswerMotion, LowerBody);
        mtwkSetRegionEvalTime(work, frame, LowerBody);
        return 0;
    case 0x57:
        entry = FindDoorEntry(g_doorEntrySubject);
        if (entry->flags & 3)
            break;
        control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        character = scenFindChara(control->scenarioTask, g_doorEntryWait);
        ccmClearFlags(character, 0x800U);
        toggleCCMflagsByAmt(character, 0x80U);
        toggleCCMflagsByAmt(character, 4U);
        SetNpcSpecialRouteQueryPaused(g_doorEntryWait, 0);
        EV_NPC_NewLoadSw(1);
        EV_npcLockUnlock(g_doorEntryWait);
        g_doorAnswerDoorMotion = 0;
        g_doorAnswerSide = 0;
        g_doorAnswerFrame = 0.0f;
        g_doorEntryWait = 0;
        g_doorAnswerMotion = 0;
        g_doorEntryState = nextState;
        return 0;
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static std::int32_t drFinishEntry(std::int32_t nextState) {
    const std::int32_t state = g_doorEntryState;
    if (state == -2) {
        g_doorEntrySubject = 0;
        g_doorEntryState = nextState;
        return 1;
    }
    if (state == -1) {
        const MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        toggleCCMflagsByAmt(control->ryo, 0x80U);
        control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        ccmClearFlags(control->ryo, 0x800U);
        SetPlayerControlEnabled(1);
        const shen_name subject = g_doorEntrySubject;
        control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        ReleaseDoor(subject, ctrlGetScenarioCharaName(control->ryo));
        --g_doorEntryState;
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static std::int32_t drPlayAnswerDialogue(std::int32_t nextState) {
    static const char message[] = "\244\242\244\253\244\312\244\244\244\312\241\252";
    switch (g_doorEntryState) {
    case 0x40:
        g_doorMessageFrames = 0;
        dbgPrintEventMessage(message);
        ++g_doorEntryState;
        break;
    case 0x41:
        if (g_doorMessageFrames > 60)
            ++g_doorEntryState;
        ++g_doorMessageFrames;
        break;
    case 0x42:
        g_doorEntryState = nextState;
        ctrlStopSubtitles();
        return 1;
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void drUpdateEntry() {
    CheckHelpIcon(0x100000U, 0, CheckHelpIconOp_FrameOverlay);
    switch (g_doorEntryAction) {
    case 0:
        if (!ManageDoorEntryAlpha(0x10, 7))
            drWalkThroughDoor(0, 8);
        break;
    case 1:
        if (!ManageDoorEntryAlpha(0x20, 1) && !drTryDoor(0x10, 6))
            drWalkThroughDoor(0, 8);
        break;
    case 2:
        if (!ManageDoorEntryAlpha(0x10, 0x10))
            drWalkThroughDoor(0, 0x20);
        break;
    case 0x10:
        if (!ManageDoorEntryAlpha(0x30, 1) && !drKnockOnDoor(-1, 1, 6))
            drFinishEntry(0);
        break;
    case 0x11:
        if (!ManageDoorEntryAlpha(0x20, 1) && !drTryDoor(0x50, 6) && !drOccupantAnswers(-1))
            drFinishEntry(0);
        break;
    case 0x12:
        if (!ManageDoorEntryAlpha(0x30, 1) && !drKnockOnDoor(-1, 0, 6))
            drFinishEntry(0);
        break;
    case 0x13:
        if (!ManageDoorEntryAlpha(0x40, 0)) {
            drPlayAnswerDialogue(-1);
            drFinishEntry(0);
        }
        break;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t GetSelectedNpcDoorId() {
    return static_cast<std::int32_t>(g_doorIngress.selectedDoor);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
const Vector3 &GetDoorIngressOrigin() {
    return g_doorIngress.position;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static std::int32_t SetDoorRouteCondition(shen_name subject, shen_name condition, std::int32_t mode, std::int32_t sense) {
    DoorTaskEntry *const entry = FindDoorEntry(subject);
    if (!entry)
        return 0;
    entry->routeCondition = condition;
    entry->routeMode = mode;
    entry->routeSense = sense;
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static std::int32_t SetDoorRequiredEntry(shen_name subject, shen_name required, std::int32_t mode) {
    DoorTaskEntry *const entry = FindDoorEntry(subject);
    if (!entry)
        return 0;
    entry->requiredEntry = required;
    entry->routeMode = mode;
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void DoorMapEventNullsub(std::int32_t) {
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void EmitDoorMotionSound(DoorTaskEntry *entry) {
    std::int32_t flags = 0;
    if (entry->flags & 0x20U)
        return;
    float first, last;
    if (entry->motion.flags & TNMF_Reverse) {
        first = entry->motion.frame + entry->motion.speed;
        last = first + entry->motion.speed;
    } else {
        last = entry->motion.frame - entry->motion.speed;
        first = last - entry->motion.speed;
    }
    std::uint32_t sound;
    const std::byte *frameEvent = MOTN_FindFrameEvent(4, entry->animationId, first, last, 1);
    if (frameEvent) {
        flags = static_cast<std::int8_t>(frameEvent[3]);
        sound = reinterpret_cast<const std::uint32_t *>(frameEvent)[1];
    } else {
        frameEvent = MOTN_FindFrameEvent(49, entry->animationId, first, last, 1);
        if (frameEvent) {
            flags = static_cast<std::int8_t>(frameEvent[3]);
            sound = reinterpret_cast<const std::uint32_t *>(frameEvent)[static_cast<std::int32_t>(3.0f * dc::sdk::RandomFloat()) + 1];
        } else {
            sound = 0;
        }
    }
    if (!sound)
        return;
    std::uint32_t material = GetModelSoundMaterial(entry->primary);
    if (!(material & 0xf0U)) {
        std::int32_t family = 0;
        switch (static_cast<std::int8_t>(*entry->sourcePair >> 8U)) {
        case '1':
            family = 1;
            break;
        case '2':
            family = 2;
            break;
        case '3':
            family = 3;
            break;
        case '4':
            family = 4;
            break;
        case '5':
            family = 5;
            break;
        case '6':
            family = 6;
            break;
        case '7':
            family = 7;
            break;
        }
        material |= static_cast<std::uint32_t>(family) << 4U;
    }
    const std::uint32_t environment = GetModelSoundEnvironment(entry->primary);
    sound = (sound & 0x87ffffU) | (static_cast<std::uint32_t>(flags & 0x7f) << 24U) | ((environment & 15U) << 19U) | material;
    if ((flags & 0x80) && entry->claimant != SHEN_NAME4('R', 'Y', 'O', '_')) {
        Vector3 position;
        position.x = entry->worldTransform.rows[3].x;
        position.y = entry->worldTransform.rows[3].y;
        position.z = entry->worldTransform.rows[3].z;
        Execute3DSoundCommand(sound, &position, 2.0f, 30.0f);
    } else {
        Loop3DSoundEffect(sound);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint16_t GetDoorLeafAnimation(std::int32_t animationId) {
    switch (animationId) {
    case 0xf050:
        animationId = 0xf0bf;
        break;
    case 0xf053:
        animationId = 0xf0c0;
        break;
    case 0xf05b:
        animationId = 0xf0c1;
        break;
    case 0xf05c:
        animationId = 0xf0c2;
        break;
    case 0xf071:
        animationId = 0xf0c3;
        break;
    case 0xf074:
        animationId = 0xf0c4;
        break;
    case 0xf07b:
        animationId = 0xf0c5;
        break;
    case 0xf07c:
        animationId = 0xf0c6;
        break;
    case 0xf081:
        animationId = 0xf0c7;
        break;
    case 0xf082:
        animationId = 0xf0c8;
        break;
    case 0xf097:
        animationId = 0xf0cb;
        break;
    case 0xf09a:
        animationId = 0xf0cc;
        break;
    case 0xf0a1:
        animationId = 0xf0f1;
        break;
    case 0xf0a2:
        animationId = 0xf0f2;
        break;
    case 0xf0e2:
        animationId = 0xf0cd;
        break;
    case 0xf0e5:
        animationId = 0xf0ce;
        break;
    case 0xf00c:
        animationId = 0xf0c9;
        break;
    case 0xf010:
        animationId = 0xf0ca;
        break;
    case 0xf051:
        animationId = 0xf0ab;
        break;
    case 0xf052:
        animationId = 0xf0ac;
        break;
    case 0xf072:
        animationId = 0xf0ad;
        break;
    case 0xf073:
        animationId = 0xf0ae;
        break;
    case 0xf098:
        animationId = 0xf0b1;
        break;
    case 0xf099:
        animationId = 0xf0b2;
        break;
    case 0xf0e3:
        animationId = 0xf0b3;
        break;
    case 0xf0e4:
        animationId = 0xf0b4;
        break;
    case 0xf00e:
        animationId = 0xf0af;
        break;
    case 0xf00f:
        animationId = 0xf0b0;
        break;
    case 0xf054:
        animationId = 0xf0b5;
        break;
    case 0xf055:
        animationId = 0xf0b6;
        break;
    case 0xf075:
        animationId = 0xf0b7;
        break;
    case 0xf076:
        animationId = 0xf0b8;
        break;
    case 0xf09b:
        animationId = 0xf0bb;
        break;
    case 0xf09c:
        animationId = 0xf0bc;
        break;
    case 0xf01c:
        animationId = 0xf0b9;
        break;
    case 0xf01f:
        animationId = 0xf0ba;
        break;
    case 0xf0e7:
        animationId = 0xf0bd;
        break;
    case 0xf0e8:
        animationId = 0xf0be;
        break;
    default:
        animationId = 0xf0aa;
        break;
    }
    return animationId;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void DoorTransitionNullsub(shen_name) {
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
const char *GetDoorAuthorName(shen_name name) {
    *reinterpret_cast<shen_name *>(g_doorAuthorName) = name;
    g_doorAuthorName[4] = 0;
    return g_doorAuthorName;
}
#endif

// @verified - d
void SetDoorScreenFade(std::uint32_t frames, std::uint8_t startAlpha, std::uint8_t startRed, std::uint8_t startGreen, std::uint8_t startBlue,
    std::uint8_t targetAlpha, std::uint8_t targetRed, std::uint8_t targetGreen, std::uint8_t targetBlue) {
    ScreenFilmColourRequest request;
    request.durationFrames = frames;
    request.start[0] = startAlpha;
    request.start[1] = startRed;
    request.start[2] = startGreen;
    request.start[3] = startBlue;
    request.target[0] = targetAlpha;
    request.target[1] = targetRed;
    request.target[2] = targetGreen;
    request.target[3] = targetBlue;
    WaitForNextTask(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->filmTask, request, 0);
}

// @verified - d
void UpdateDoorAuthorController() {
    switch (g_doorRuntimeState.transitionState) {
    case 0:
    case 0x26:
        return;
    case 1: {
        std::int32_t &cameraState = doorAuthorCameraState;
        cameraState = camCtrlGetState();
        camCtrlGetNextCamPos(&doorAuthorCameraPosition);
        camCtrlSetState(9);
        camCtrlSetNextPosition(doorAuthorCameraPosition);
        std::uint32_t &readerMask = doorLightingReaderMask;
        readerMask = GetMapModelReaderMask();
    }
    fadeOut:
        SetDoorScreenFade(20, 0, 0, 0, 0, 255, 0, 0, 0);
        if (static_cast<std::int8_t>(g_doorRuntimeState.destinationDoor >> 8U) == '7') {
            SetPlayerControlEnabled(0);
            shenToggleAreaFlags(1);
        }
        EV_NPC_NewLoadSw(0);
        spmFadeOut(0);
        break;
    case 2:
    case 0x28:
        if (DoesScreenFilmColourExist(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->filmTask))
            return;
        EV_ResetFreeTalkFlags();
        break;
    case 3:
        Queue_ST_LoadTitleFadeIn(g_doorRuntimeState.temporaryAuthor);
        ++g_doorRuntimeState.transitionState;
        return;
    case 0x22: {
        DoorTaskEntry *const entry = FindDoorEntry(g_doorRuntimeState.destinationDoor);
        if (entry && entry->callback)
            entry->callback(g_doorRuntimeState.destinationDoor);
        charaToggleFlag(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo, 1);
        FinishDoorAuthorLeaf(g_doorRuntimeState.destinationDoor);
        if (static_cast<std::int8_t>(g_doorRuntimeState.destinationDoor >> 8U) == '7')
            CompleteAutomaticDoorPlayerPlacement(doorFindEntryTransform(g_doorRuntimeState.destinationDoor));
        else
            CompleteDoorPlayerPlacement(doorFindEntryTransform(g_doorRuntimeState.destinationDoor));
        ++g_doorRuntimeState.transitionState;
    }
    case 0x23: {
        if (PakLoadWait())
            return;
        const char *const name = GetDoorAuthorName(g_doorRuntimeState.temporaryAuthor);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        if (FindPakByEntryName(name) || !RequestMapPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), name))
            return;
#else
        if (FindPakByEntryName(name) || !LoadMapPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), name))
            return;
#endif
        pakLoadFlush();
        SetupDoorAreaCallbacks();
        DoorSceneCleanup(0);
        EV_NPC_NewLoadSw(1);
        SetDoorAuthorSecondaryNodes(g_doorRuntimeState.temporaryAuthor, 0);
        SetWeatherLightManager(0);
        camCtrlSetState(doorAuthorCameraState);
        camCtrlSetFovTransitionTo(1);
        toggleCCMflagsByAmt(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo, 0x80U);
        ccmClearFlags(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo, 0x800U);
        toggleCCMflagsByAmt(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo, 4U);
        SetPlayerControlEnabled(1);
        EV_spmBeginSampleExecute(0);
        if (Queue_FLAG_CheckTask(g_doorRuntimeState.temporaryAuthor)) {
            shenUpdatePlayerDataFlags(1);
            shenUpdateFlagEvaluation(1);
        }
        ++g_doorRuntimeState.transitionState;
        ST_LoadTitleFade();
        return;
    }
    case 0x24:
    case 0x4a:
        SetDoorScreenFade(20, 255, 0, 0, 0, 0, 0, 0, 0);
        break;
    case 0x25:
        if (DoesScreenFilmColourExist(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->filmTask))
            return;
        break;
    case 0x27: {
        std::int32_t &cameraState = doorAuthorCameraState;
        cameraState = camCtrlGetState();
    }
        camCtrlGetNextCamPos(&doorAuthorCameraPosition);
        camCtrlSetState(9);
        camCtrlSetNextPosition(doorAuthorCameraPosition);
        goto fadeOut;
    case 0x29:
        g_doorRuntimeState.transitionState = 0x48;
        return;
    case 0x48: {
        DoorTaskEntry *const entry = FindDoorEntry(g_doorRuntimeState.destinationDoor);
        if (entry && entry->callback)
            entry->callback(g_doorRuntimeState.destinationDoor);
        charaToggleFlag(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo, 1);
        ++g_doorRuntimeState.transitionState;
    }
    case 0x49:
        FinishDoorAuthorLeaf(g_doorRuntimeState.destinationDoor);
        if (static_cast<std::int8_t>(g_doorRuntimeState.destinationDoor >> 8U) == '7')
            CompleteAutomaticDoorPlayerPlacement(doorFindEntryTransform(g_doorRuntimeState.destinationDoor));
        else
            CompleteDoorPlayerPlacement(doorFindEntryTransform(g_doorRuntimeState.destinationDoor));
        FreePakSlot(FindPakByEntryName(GetDoorAuthorName(g_doorRuntimeState.temporaryAuthor)));
        authLoadCurrentQueued();
        EV_NPC_NewLoadSw(1);
        SetDoorAuthorSecondaryNodes(g_doorRuntimeState.temporaryAuthor, 1);
        SetWeatherLightManager(1);
        camCtrlSetState(doorAuthorCameraState);
        camCtrlSetFovTransitionTo(1);
        for (std::int32_t index = 0; index < 32; ++index) {
            if ((doorLightingReaderMask >> index) & 1U) {
                SetMapModelGridCullingEnabled(1, index);
                SetMapFieldDepthSlotEnabled(1, index);
            }
        }
        {
            const MapRenderProfile &profile = MatchMapRenderProfile(g_currentMap);
            camCtrlSetFarDistance(profile.farDistance);
            camCtrlSetNearDistance(profile.nearDistance);
            camCtrlSetNpcRadius(profile.npcRadius);
        }
        toggleCCMflagsByAmt(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo, 0x80U);
        ccmClearFlags(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo, 0x800U);
        toggleCCMflagsByAmt(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo, 4U);
        SetPlayerControlEnabled(1);
        EV_spmBeginSampleExecute(0);
        if (Queue_FLAG_CheckTask(g_currentMap))
            shenUpdatePlayerDataFlags(1);
        break;
    case 0x4b:
        if (DoesScreenFilmColourExist(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->filmTask))
            return;
        g_doorRuntimeState.transitionState = 0;
        g_doorRuntimeState.temporaryAuthor = 0;
        g_doorRuntimeState.destinationDoor = 0;
        return;
    }
    ++g_doorRuntimeState.transitionState;
}

// @verified - d
void UpdateDoorDestinationController() {
    switch (g_doorDestination.phase) {
    case 0:
        break;
    case 1:
        camCtrlGetNextCamPos(&doorAuthorCameraPosition);
        camCtrlSetState(9);
        camCtrlSetNextPosition(doorAuthorCameraPosition);
        spmFadeOut(0);
        SetDoorScreenFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        EV_NPC_NewLoadSw(0);
        if (static_cast<std::int8_t>(g_doorDestination.region >> 8U) == '7')
            shenToggleAreaFlags(2);
        ++g_doorDestination.phase;
        break;
    case 2:
        if (!DoesScreenFilmColourExist(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->filmTask))
            ++g_doorDestination.phase;
        break;
    case 3:
        shenQueueChangeModuleAndScene(SystemModule_Game, g_loadedActNumber, g_doorDestination.area, g_doorDestination.entry);
        break;
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static std::int32_t IsDoorAuthorActive() {
    return g_doorRuntimeState.transitionState != 0 && g_doorRuntimeState.transitionState != 0x26;
}
#endif

// @verified - d
std::int32_t EnterDoorSubscene(shen_name author) {
    return author ? LoadDoorAuthor(author) : RestoreDoorAuthor(1);
}

// @verified - d
std::int32_t LeaveDoorSubscene(shen_name author) {
    return author ? LoadDoorAuthor(author) : RestoreDoorAuthor(0);
}

// @verified - d
std::int32_t LoadDoorAuthor(shen_name author) {
    if (!author)
        return 0;
    g_doorRuntimeState.temporaryAuthor = author;
    g_doorRuntimeState.transitionState = 0x26;
    doorLightingReaderMask = GetMapModelReaderMask();
    EV_NPC_NewLoadSw(0);
    spmFadeOut(0);
    EV_ResetFreeTalkFlags();
    const char *const name = GetDoorAuthorName(g_doorRuntimeState.temporaryAuthor);
    if (FindPakByEntryName(name))
        return 0;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    RequestMapPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), name);
#else
    LoadMapPackage(Filepath_Gen(FilepathGenMode::SceneDirectory), name);
#endif
    pakLoadFlush();
    SetupDoorAreaCallbacks();
    DoorSceneCleanup(0);
    EV_NPC_NewLoadSw(1);
    SetDoorAuthorSecondaryNodes(g_doorRuntimeState.temporaryAuthor, 0);
    SetWeatherLightManager(0);
    if (Queue_FLAG_CheckTask(g_doorRuntimeState.temporaryAuthor)) {
        shenUpdatePlayerDataFlags(1);
        shenUpdateFlagEvaluation(1);
    }
    EV_spmBeginSampleExecute();
    return 1;
}

// @verified - d
std::int32_t RestoreDoorAuthor(std::int32_t restoreAudio) {
    if (g_doorRuntimeState.transitionState != 0x26)
        return 0;
    spmFadeOut(0);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    FreePakSlot(FindPakByEntryName(GetDoorAuthorName(g_doorRuntimeState.temporaryAuthor)));
    if (restoreAudio)
        authLoadCurrentQueued();
    else
        RestoreDoorAreaCallbacks();
    EV_NPC_NewLoadSw(1);
    SetDoorAuthorSecondaryNodes(g_doorRuntimeState.temporaryAuthor, 1);
    SetWeatherLightManager(1);
    for (std::int32_t index = 0; index < 32; ++index) {
        if ((doorLightingReaderMask >> index) & 1U) {
            SetMapModelGridCullingEnabled(1, index);
            SetMapFieldDepthSlotEnabled(1, index);
        }
    }
    const MapRenderProfile &profile = MatchMapRenderProfile(g_currentMap);
    camCtrlSetFarDistance(profile.farDistance);
    camCtrlSetNearDistance(profile.nearDistance);
    camCtrlSetNpcRadius(profile.npcRadius);
    DoorTaskEntry *const entry = FindDoorByAuthor(g_doorRuntimeState.temporaryAuthor);
    if (entry && entry->callback)
        entry->callback(g_doorRuntimeState.temporaryAuthor);
    EV_spmBeginSampleExecute(0);
    if (Queue_FLAG_CheckTask(g_currentMap))
        shenUpdatePlayerDataFlags(1);
    g_doorRuntimeState.transitionState = 0;
    g_doorRuntimeState.temporaryAuthor = 0;
    return 1;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void TitleWarningCallback(void *) {
    switch (titleWarningPhase) {
    case 0:
        if (titleWarningLogo > 0) {
            titleWarningPhase = 1;
            titleWarningDesiredColour = titleWarningBaseColour;
        }
        break;
    case 1:
        titleWarningPhase = 2;
        break;
    case 2:
        if (titleWarningLogo > 0)
            HLib::ExecuteCallbackNow(SubmitTitleWarningLogo, 0);
        else {
            titleWarningPhase = 3;
            titleWarningTailFrames = 4;
            titleWarningDesiredColour = 0;
        }
        break;
    case 3:
        if (static_cast<std::int32_t>(--titleWarningTailFrames) < 0)
            titleWarningPhase = 0;
        break;
    }
    std::uint32_t difference;
    if (titleWarningCurrentColour > titleWarningDesiredColour) {
        difference = titleWarningCurrentColour - titleWarningDesiredColour;
        if (difference > 0xff000000U)
            difference = 0xff000000U;
        titleWarningCurrentColour -= difference;
    } else {
        difference = titleWarningDesiredColour - titleWarningCurrentColour;
        if (difference > 0xff000000U)
            difference = 0xff000000U;
        titleWarningCurrentColour += difference;
    }
    if (titleWarningCurrentColour && (!titleWarningUsesLogoParameters || titleWarningLogoParameters[0]))
        DrawMainIconOverlay(titleWarningCurrentColour, 6.0f);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void InitTitleWarningTaskData(TitleWarningTaskData *data);
std::uint32_t SetTitleWarningBaseColour(std::uint32_t colour);

// @verified - d
void QueueTitleWarningTask() {
    titleWarningPhase = 0;
    titleWarningLogo = 0;
    titleWarningTask = HLib::EnqueueTaskWithParameter(TitleWarningCallback, 15, 0, sizeof(TitleWarningTaskData), shen_name(SHEN_NAME(TWRN)));
    TitleWarningTaskData *data = static_cast<TitleWarningTaskData *>(HLib::GetTaskParameter(titleWarningTask));
    if (data)
        InitTitleWarningTaskData(data);
    SetTitleWarningBaseColour(0x76000000U);
    titleWarningCurrentColour = titleWarningDesiredColour = 0;
    titleWarningUsesLogoParameters = 0;
}

std::uint32_t SetTitleWarningBaseColour(std::uint32_t colour) {
    const std::uint32_t previous = titleWarningBaseColour;
    titleWarningBaseColour = colour;
    return previous;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t IsDoorTransitionActive() {
    return g_doorDestination.phase != 0;
}
#endif

// @verified - d
void SetupDoorAreaCallbacks() {
    savedDoorCleanupCallback = g_doorAreaCleanupCallback;
    g_doorAreaCleanupCallback = DoorSceneCleanup;
    const char *source = GetCurrentBgm();
    for (std::int32_t index = 0; index < 16; ++index) {
        g_doorRuntimeState.savedSoundMap[index] = source[index];
        if (!source[index])
            break;
    }
    source = shenGetCurrentSbt();
    for (std::int32_t index = 0; index < 16; ++index) {
        g_doorRuntimeState.savedSoundSequence[index] = source[index];
        if (!source[index])
            break;
    }
    g_doorRuntimeState.savedSoundScene = GetCurrentSoundSceneIndex();
    mapSetupLighting(0);
}

// @verified - d
void DoorSceneCleanup(std::int32_t) {
    std::uint32_t sequence[3];
    std::uint32_t name[2] = {0, 0};
    name[0] = g_doorRuntimeState.temporaryAuthor;
    if (sndLoadSceneMap(reinterpret_cast<const char *>(name))) {
        sequence[0] = name[0];
        sequence[1] = 0x3030305fU;
        sequence[2] = 0;
        shenmue_s2_event_EV_LoadAudio(reinterpret_cast<const char *>(sequence), 0);
    }
    RemoveWeatherInstructionFlags(0x780U);
    RemoveWeatherInstructionFlags(0x0cU);
    LGHT_Parser_Internal(name[0]);
    const std::uint32_t changedReaders = GetMapModelReaderMask() ^ doorLightingReaderMask;
    for (std::int32_t index = 0; index < 32; ++index) {
        if ((changedReaders >> index) & 1U) {
            SetMapModelGridCullingEnabled(0, index);
            SetMapFieldDepthSlotEnabled(1, index);
            AssignLightProgram(index);
        } else {
            EV_AssignLightParams(1, index);
            SetMapModelGridCullingEnabled(1, index);
            SetMapFieldDepthSlotEnabled(0, index);
        }
    }
    SetupLightConfig();
    camCtrlSetFarDistance(20.0f);
    camCtrlSetNearDistance(20.0f);
    camCtrlSetNpcRadius(20.0f);
}

// @verified - d
void authLoadCurrentQueued() {
    sndLoadSceneMap(g_doorRuntimeState.savedSoundMap);
    shenmue_s2_event_EV_LoadAudio(g_doorRuntimeState.savedSoundSequence, static_cast<std::uint32_t>(g_doorRuntimeState.savedSoundScene));
    SetMapModelDataDefaultVertexShader();
    AddWeatherInstructionFlags(0x780U);
    AddWeatherInstructionFlags(0x0cU);
    g_doorAreaCleanupCallback = savedDoorCleanupCallback;
    if (savedDoorCleanupCallback)
        savedDoorCleanupCallback(g_doorCachedSceneId);
    mapSetupLighting(1);
}

// @verified - d
void RestoreDoorAreaCallbacks() {
    SetMapModelDataDefaultVertexShader();
    AddWeatherInstructionFlags(0x780U);
    AddWeatherInstructionFlags(0x0cU);
    g_doorAreaCleanupCallback = savedDoorCleanupCallback;
    if (savedDoorCleanupCallback)
        savedDoorCleanupCallback(g_doorCachedSceneId);
    mapSetupLighting(1);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t AdvanceDoorMotion(DoorTaskEntry *entry) {
    std::int32_t completed = 0;
    MT7Node *first, *second;
    if (!(entry->flags & 3U))
        entry->flags = static_cast<std::uint16_t>((entry->flags & ~3U) | 1U);
    else if ((entry->flags & 3U) == 2U)
        entry->flags |= 3U;
    const std::int32_t family = static_cast<std::int8_t>((*entry->sourcePair) >> 8U) - '0';
    switch (family) {
    case 1:
    case 2:
    case 3:
    case 4: {
        first = entry->motionNode.child;
        entry->motionNode.child = 0;
        second = entry->motionNode.sibling;
        entry->motionNode.sibling = 0;
        completed = TickTimedNodeMotion(&entry->motion);
        entry->motionNode.child = first;
        entry->motionNode.sibling = second;
        break;
    }
    case 5:
    case 6: {
        first = entry->motionNode.child->child;
        entry->motionNode.child->child = 0;
        second = entry->motionNode.child->sibling->child;
        entry->motionNode.child->sibling->child = 0;
        completed = TickTimedNodeMotion(&entry->motion);
        entry->motionNode.child->child = first;
        entry->motionNode.child->sibling->child = second;
        break;
    }
    case 7: {
        first = entry->motionNode.child->child;
        entry->motionNode.child->child = 0;
        second = entry->motionNode.child->sibling->child;
        entry->motionNode.child->sibling->child = 0;
        const float previous = entry->motion.frame;
        completed = TickTimedNodeMotion(&entry->motion);
        entry->motionNode.child->child = first;
        entry->motionNode.child->sibling->child = second;
        const std::int32_t openingAllowed = IsDoorOpeningAllowed(*entry->sourcePair);
        if ((entry->flags & 3U) == 1U && 25.0f > entry->motion.end - entry->motion.frame && !(25.0f > entry->motion.end - previous)) {
            if (openingAllowed) {
                ToggleFlagOnFieldEntity(*entry->sourcePair, 0);
            } else if (entry == g_doorEntryHead) {
                const std::uint32_t side = GetDoorExitSide(*entry->sourcePair);
                Vector3 position;
                plymUpdatePlayerSearchLocation(&position);
                InverseTransformPointByMatrix(&entry->worldTransform, &position);
                if ((!side && position.z > 0.0f) || (side && 0.0f > position.z))
                    ToggleFlagOnFieldEntity(*entry->sourcePair, 0);
            }
        }
        if ((entry->flags & 3U) == 3U && entry->motion.frame > 25.0f && !(previous > 25.0f))
            ToggleFlagOnFieldEntity(*entry->sourcePair, 1);
        break;
    }
    }
    if (completed) {
        if (static_cast<std::int8_t>((*entry->sourcePair) >> 8U) == '7') {
            if ((entry->flags & 3U) == 1U)
                entry->flags = static_cast<std::uint16_t>((entry->flags & ~3U) | 2U);
            else if ((entry->flags & 3U) == 3U)
                entry->flags &= ~3U;
        } else {
            entry->flags &= ~3U;
        }
        entry->claimant = __typeof__(entry->claimant)();
        return 1;
    }
    if (!IsDoorTransitionActive() || static_cast<std::int8_t>(g_doorDestination.region >> 8U) != '7')
        EmitDoorMotionSound(entry);
    return 0;
}
#endif

// @verified - d
void FinishDoorAuthorLeaf(shen_name subject) {
    DoorTaskEntry *const entry = FindDoorEntry(subject);
    if (!entry)
        return;
    entry->flags |= 0x20U;
    if (static_cast<std::int8_t>(subject >> 8U) == '7') {
        entry->flags = (entry->flags & ~3U) | 2U;
        doorPlayRigidMotion(*entry->sourcePair, 0xf0a9, 0.0f, 1.0f);
        while (!AdvanceDoorMotion(entry)) {
        }
    } else if ((entry->flags & 3U) == 1U) {
        while (!AdvanceDoorMotion(entry)) {
        }
    }
    entry->flags &= ~0x20U;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void LoadDoorNumberModels() {
    MT7Model *const model = InitializeMT7Model(shenLoadFile("misc", "QRRNWA0G.MT7", shen_name(SHEN_NAME4('d', 'r', 'r', 'n'))));
    MT7Node *node = model->root->child;
    for (std::int32_t index = 0; index < 10; ++index) {
        g_doorNumberModels[index] = node;
        node = node->sibling;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void CompleteDoorPlayerPlacement(const Matrix4x4 *transform) {
    Vector3 position, first, last;
    Vector3i rotation = {0, 0, 0};
    float split, current, maximum;
    Character *const player = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo;
    MotionWork *const motion = charFindMotionwork(player);
    const std::uint16_t animation = mtwkGetAnimationId(motion, LowerBody);
    mtwkCopyRegionTimes(motion, &split, &current, &maximum, LowerBody);
    SetPlayerControlEnabled(0);
    plymFinishRegionInteraction();
    SampleNativeMotionRoot(animation, mtwkGetRigBoneId(motion), current, &first);
    SampleNativeMotionRoot(animation, mtwkGetRigBoneId(motion), maximum, &last);
    last.x -= first.x;
    last.z -= first.z;
    GetCharacterWorldPosition(player, &position);
    GetCharacterWorldRotation(player, &rotation);
    matrix_stack::PushUnitMatrix();
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    Matrix4x4::SetViewMatrixFromEuler(rotation.x, rotation.y, rotation.z);
    Matrix4x4::preTranslate(last.x, 0.0f, last.z);
    matrix_stack::GetTranslation(&position);
    InverseTransformPointByMatrix(transform, &position);
    matrix_stack::LoadMatrix(transform);
    if (position.z < 0.0f)
        position.z = -1.4f;
    else
        position.z = 1.4f;
    Matrix4x4::TransformPoint(position, position);
    PopCurrentMatrix(1);
    SetCharacterWorldPosition(player, &position);
    mtwkSetRegionFlag(motion, 71, 0, LowerBody);
}
#else
void CompleteDoorPlayerPlacement(const Matrix4x4 *transform) {
    Character *const player = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo;
    MotionWork *const motion = charFindMotionwork(player);
    const std::uint16_t animation = mtwkGetAnimationId(motion, LowerBody);
    float split, current, maximum;
    mtwkCopyRegionTimes(motion, &split, &current, &maximum, LowerBody);
    SetPlayerControlEnabled(0);
    plymFinishRegionInteraction();
    Vector3 first, last, position;
    SampleNativeMotionRoot(animation, mtwkGetRigBoneId(motion), current, &first);
    SampleNativeMotionRoot(animation, mtwkGetRigBoneId(motion), maximum, &last);
    last.x -= first.x;
    last.z -= first.z;
    GetCharacterWorldPosition(player, &position);
    Vector3i rotation = {};
    GetCharacterWorldRotation(player, &rotation);
    matrix_stack::PushUnitMatrix();
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    Matrix4x4::SetViewMatrixFromEuler(rotation.x, rotation.y, rotation.z);
    Matrix4x4::preTranslate(last.x, 0.0f, last.z);
    matrix_stack::GetTranslation(&position);
    InverseTransformPointByMatrix(transform, &position);
    Matrix4x4::SetActiveTransform(transform);
    position.z = position.z < 0.0f ? -1.4f : 1.4f;
    Matrix4x4::TransformPoint(position, position);
    PopCurrentMatrix(1);
    shenApplyTranslationToAll(&player->node, position);
    mtwkSetRegionFlag(motion, 71, 0, LowerBody);
}
#endif

// @verified - d
void CompleteAutomaticDoorPlayerPlacement(const Matrix4x4 *transform) {
    SetPlayerControlEnabled(0);
    Character *const player = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo;
    MotionWork *const motion = charFindMotionwork(player);
    Vector3 position;
    GetCharacterWorldPosition(player, &position);
    matrix_stack::PushUnitMatrix();
    InverseTransformPointByMatrix(transform, &position);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    matrix_stack::LoadMatrix(transform);
#else
    Matrix4x4::SetActiveTransform(transform);
#endif
    if (g_doorCrossingZ < 0.0f)
        position.z = -1.5f;
    else
        position.z = 1.5f;
    if (position.x < -0.69999999f)
        position.x = -0.69999999f;
    else if (position.x > 0.69999999f)
        position.x = 0.69999999f;
    Matrix4x4::TransformPoint(position, position);
    PopCurrentMatrix(1);
    mtwkSetRegionFlag(motion, 71, 0, LowerBody);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    SetCharacterWorldPosition(player, &position);
#else
    shenApplyTranslationToAll(&player->node, position);
#endif
    mtwkRequestMotion(motion, 0xf002, LowerBody);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static std::int32_t GetDoorNumber(shen_name subject) {
    static const std::int8_t qa[32] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 1, 2, 3, 4, 5, 6, 7, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 10, 11};
    static const std::int8_t qb[5] = {1, 2, 1, 2, 2};
    static const std::int8_t qc[7] = {1, 2, 3, 4, 5, 6, 7};
    static const std::int8_t qd[7] = {5, 6, 7, 8, 13, 14, 15};
    static const std::int8_t qe[21] = {1, 1, 1, 2, 3, 4, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const std::uint32_t area = g_currentMap & 0xffffU;
    std::int32_t index = (static_cast<std::int32_t>((subject >> 16U) & 0xffU) - '0') * 10 + static_cast<std::int32_t>(subject >> 24U);
    index -= '1';
    if (area == 0x4151U && index <= 31)
        return qa[index];
    if (area == 0x4251U && index <= 4)
        return qb[index];
    if (area == 0x4351U && index <= 6)
        return qc[index];
    if (area == 0x4451U && index <= 6)
        return qd[index];
    if (area == 0x4551U && index <= 20)
        return qe[index];
    return index + 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void UpdateDoorNumberModels(DoorTaskEntry *entry) {
    float offset = 0.0f;
    const shen_name name = *entry->sourcePair;
    MT7Node *const anchor = FindModelCharacterRecursive(entry->primary, shen_name((name & ~0xffU) | 'Q'));
    if (!anchor)
        return;
    MT7Node *node = anchor->child = entry->digits;
    std::int32_t floor = g_doorCurrentStream;
    if (floor <= 0)
        floor = static_cast<std::int32_t>(__builtin_fabsf(static_cast<float>(floor)) + 1.0f);
    const std::int32_t number = GetDoorNumber(name);
    for (std::int32_t index = 0; index < 4; ++index) {
        switch (index) {
        case 0:
            *node = *g_doorNumberModels[floor / 10];
            break;
        case 1:
            *node = *g_doorNumberModels[floor % 10];
            break;
        case 2:
            *node = *g_doorNumberModels[number / 10];
            break;
        case 3:
            *node = *g_doorNumberModels[number % 10];
            break;
        }
        node->child = 0;
        node->parent = anchor;
        node->position.x += static_cast<float>(index) * 0.08f - offset;
        node->kind = ((static_cast<std::int16_t>(entry->flags) >> 4) ^ 1) & 1;
        if (!index && floor / 10 == 0) {
            node->mesh = 0;
            offset = 0.04f;
        }
        if (index == 3)
            node->sibling = 0;
        else
            node = node->sibling = node + 1;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
static void TransformDoorPropNode(MT7Node *node) {
    if (node->sibling)
        matrix_stack::Matrix4x4_Push();
    Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
    Matrix4x4::SetViewMatrixFromEuler(node->rotation.x, node->rotation.y, node->rotation.z);
    DoorTaskEntry *const entry = FindDoorEntry(shen_name(GetModelMarker(node)));
    if (entry) {
        matrix_stack::GetMatrix(&entry->worldTransform);
        matrix_stack::Matrix4x4_Push();
        const std::uint32_t family = (((*entry->sourcePair) >> 8U) - '5') & 0xffU;
        if (family > 2U)
            Matrix4x4::preTranslate(0.45f, 0.0f, 0.0f);
        matrix_stack::GetTranslation(&entry->interactionOrigin);
        PopCurrentMatrix(1);
        UpdateDoorNumberModels(entry);
    }
    if (node->child)
        TransformDoorPropNode(node->child);
    if (node->sibling) {
        PopCurrentMatrix(1);
        TransformDoorPropNode(node->sibling);
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void TransformPropObject() {
    matrix_stack::PushUnitMatrix();
    Vector3 position;
    GetPropVector4(&position);
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    TransformDoorPropNode(GetPropRoot());
    PopCurrentMatrix(1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void RefreshDoorSceneStateForCurrentStep() {
    if ((g_currentMap & 0xffffU) != SHEN_NAME4('Q', 'E', 0, 0))
        return;
    DoorTaskEntry *const entry = FindDoorEntry(SHEN_NAME(R146));
    if (!entry)
        return;
    if (static_cast<std::int32_t>(GetCurrentStep()) >= 340000)
        ApplyDoorSceneStateToSiblings(entry->primary, 1);
    else
        ApplyDoorSceneStateToSiblings(entry->primary, 0);
}
#endif

} // namespace s2
} // namespace shenmue

#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern "C" {
bool __attribute__((renesas)) shenmue_s2_event_ComputeDoorIngress(std::int32_t doorId, const shenmue::Vector3 *position, shenmue::Vector3 *target) {
    return shenmue::s2::ComputeDoorIngress(doorId, position, target);
}
bool __attribute__((renesas)) shenmue_s2_event_ComputeDoorIngressAlternative(std::int32_t doorId, const shenmue::Vector3 *position, shenmue::Vector3 *target) {
    return shenmue::s2::ComputeDoorEgress(doorId, position, target);
}
std::uint16_t __attribute__((renesas)) shenmue_s2_event_GetNpcDoorAnimation(
    std::int32_t doorId, shenmue::s2::Character *character, const shenmue::Vector3 *position) {
    return shenmue::s2::GetNpcDoorAnimation(doorId, character, position);
}
std::int32_t __attribute__((renesas)) shenmue_s2_event_ClaimDoor(std::uint32_t subject, std::uint32_t claimant) {
    return shenmue::s2::ClaimDoor(shenmue::shen_name(subject), shenmue::shen_name(claimant));
}
std::int32_t __attribute__((renesas)) shenmue_s2_event_ReleaseDoor(std::uint32_t subject, std::uint32_t claimant) {
    return shenmue::s2::ReleaseDoor(shenmue::shen_name(subject), shenmue::shen_name(claimant));
}
std::int32_t __attribute__((renesas)) shenmue_s2_event_doorPlayRigidMotion(std::uint32_t subject, std::uint32_t motionId, float startFrame, float speed) {
    return shenmue::s2::doorPlayRigidMotion(shenmue::shen_name(subject), static_cast<std::uint16_t>(motionId), startFrame, speed);
}
std::int32_t __attribute__((renesas)) shenmue_s2_event_doorLoopRigidMotion(std::uint32_t subject, std::uint32_t motionId, float startFrame, float speed) {
    return shenmue::s2::doorLoopRigidMotion(shenmue::shen_name(subject), static_cast<std::uint16_t>(motionId), startFrame, speed);
}

std::int32_t __attribute__((renesas)) shenmue_s2_event_SetDoorCallback(std::uint32_t subject, shenmue::s2::DoorSubjectCallback callback) {
    return shenmue::s2::SetDoorSubjectCallback(shenmue::shen_name(subject), callback);
}
shenmue::s2::DoorSubjectCallback __attribute__((renesas)) shenmue_s2_event_GetDoorCallback(std::uint32_t subject) {
    return shenmue::s2::GetDoorSubjectCallback(shenmue::shen_name(subject));
}
std::uint32_t __attribute__((renesas)) shenmue_s2_event_ActivateDefaultDoor(std::int32_t mode) {
    return shenmue::s2::SetQeDoorSceneState(mode);
}
}
#endif

#endif
