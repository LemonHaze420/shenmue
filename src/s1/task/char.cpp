#include "s1/task/char.hpp"
#include "s1/task/char_components_internal.hpp"
#include "s1/task/conv.hpp"
#include "logging.hpp"
#include "s1/face.hpp"
#include "s1/boot.hpp"
#include "s1/file_loader.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/game_utils.hpp"
#include "s1/task/effe.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/graphics_resources.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/lighting.hpp"

#include "s1/task/came.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/drwp.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/mt5_render.hpp"
#include "s1/mt5_types.hpp"
#include "s1/motion.hpp"
#include "s1/motion_registry.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/cycle_man.hpp"
#include "s1/task/obje.hpp"
#include "s1/task/mdlm.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/padc.hpp"
#include "s1/task/sond.hpp"
#include "s1/task/time.hpp"
#include "s1/task/vfma.hpp"
#include "s1/task/mbst.hpp"
#include "s1/render_array.hpp"
#include "s1/skinning_cache.hpp"
#include "s1/cloth_presets.hpp"
#include "s1/collision.hpp"
#include "s1/scene.hpp"
#include "s1/shadow_volume.hpp"
#include "s1/player_control.hpp"
#include "s1/player_stats.hpp"
#include "s1/task/mmtk.hpp"
#include "s1/ui_runtime.hpp"
#include "s1/input.hpp"
#include "s1/task/wzog.hpp"
#include "s1/task/wind.hpp"

#include "s1/timing.hpp"
#include <bit>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <ctime>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <vector>
#include <limits>

namespace shenmue::s1 {
extern void *g_hndb;
extern void *g_shadowMeshData;
extern std::uint32_t g_hndbSize;
extern void *g_handCollisionPool;
std::int32_t g_refBoardDefaultType = 0;
std::int32_t g_refBoardDefaultField024 = 0;
Vector3 g_refBoardSavedDirection{};
Vector4 g_refBoardSavedColour{};
std::uint32_t g_refBoardActive = 0;
void refbSetGlobalActive(std::uint32_t value) {
    g_refBoardActive = value;
}
std::int32_t g_characterDrawMode = 0;
std::int32_t g_drawMode = 0;
std::int32_t g_forceFullCharacterOpacity{};
Character *g_drawingCharacter = nullptr;
void *g_drawingImageModel = nullptr;
std::int32_t g_bDebugLogVisibility{};
char g_currentCharacterName[5]{};
std::FILE *g_npcVisibilityLog{};

void CHAR_LogNPCVisibility(Character *character, bool visible) {
    if (!g_npcVisibilityLog) {
        const std::time_t now = std::time(nullptr);
        const std::tm *const local = std::localtime(&now);
        char filename[128]{};
        if (local)
            std::strftime(filename, sizeof(filename), "npc_log-%H.%M.%S%p_%F.txt", local);
        g_npcVisibilityLog = std::fopen(filename, "wb");
        if (g_npcVisibilityLog) {
            constexpr char header[] = "CharID, Visibility, SceneNumber, AreaId, EntryPoint, GameTime, GameDate\n";
            std::fwrite(header, sizeof(header) - 1, 1, g_npcVisibilityLog);
        }
    }

    auto *const control = reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
    HLib::Task *const scenarioTask = control->scenarioTask;
    auto *const scenario = reinterpret_cast<ScenarioTaskData *>(scenarioTask->task_data_ptr);
    SCEN_PromoteCharacterIndexNode(scenarioTask, character);
    const std::size_t bucket = reinterpret_cast<std::uintptr_t>(character) % scenario->characterBuckets.size();
    const CharacterIdLookupNode *const node = scenario->characterBuckets[bucket];
    const std::uint32_t characterId = node && node->character == character ? node->id : 0;

    char characterText[5]{};
    char areaText[5]{};
    std::memcpy(characterText, &characterId, sizeof(characterId));
    std::memcpy(areaText, &g_areaId, sizeof(g_areaId));
    Shenmue_LogPrintf(stderr, "CW_EnableDisplay %s [%02d:%s:%02u]\n", characterText, g_sceneId, areaText, g_entryPoint);

    if (!g_npcVisibilityLog)
        return;
    char gameTimeAndDate[128]{};
    std::snprintf(gameTimeAndDate, sizeof(gameTimeAndDate), "%02u:%02u:%02u, %02u:%02u:%02u", g_currentDateTime.hours, g_currentDateTime.minutes,
        g_currentDateTime.seconds, g_currentDateTime.day, g_currentDateTime.month, g_currentDateTime.year);
    char row[256]{};
    std::snprintf(
        row, sizeof(row), "%s, %s, %02d, %s, %02u, %s\n", characterText, visible ? "VISIBLE" : "HIDDEN", g_sceneId, areaText, g_entryPoint, gameTimeAndDate);
    std::fwrite(row, std::strlen(row), 1, g_npcVisibilityLog);
    std::fflush(g_npcVisibilityLog);
}

namespace {}

std::int32_t SetCharacterDrawMode(std::uint32_t mode, std::int32_t value) {
    std::int32_t *const target = mode == 0 ? &g_characterDrawMode : mode == 1 ? &g_drawMode : nullptr;
    if (!target)
        return 0;
    const std::int32_t previous = *target;
    *target = value;
    return previous;
}

void SetCharacterRenderOpacity(Character *character, float opacity) {
    if (character)
        character->renderOpacity = opacity;
}

void SetCharacterVisibility(Character *character, bool visible) {
    if (!character)
        return;
    auto *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    HLib::Task *characterTask = nullptr;
    if (control) {
        const std::size_t bucket = reinterpret_cast<std::uintptr_t>(character) % control->characterBuckets.size();
        for (auto *link = static_cast<CharacterBucketNode *>(control->characterBuckets[bucket]); link; link = link->next) {
            if (link->character == character) {
                characterTask = link->task;
                break;
            }
        }
    }
    if (visible) {
        character->flags.value |= 4U;
        if (characterTask && characterTask->initCallbackFnPtr) {
            if (characterTask->autoInitTaskCallbackFnPtr &&
                characterTask->mainTaskCallbackFnPtr == std::bit_cast<HLib::TaskCallback>(&HLib_ProcessGenericAutoCleanup)) {
                characterTask->autoInitTaskCallbackFnPtr = characterTask->initCallbackFnPtr;
            } else {
                characterTask->mainTaskCallbackFnPtr = characterTask->initCallbackFnPtr;
            }
            characterTask->initCallbackFnPtr = nullptr;
        }
    } else {
        character->flags.value &= ~4U;
        if (characterTask) {
            if (!characterTask->initCallbackFnPtr) {
                characterTask->initCallbackFnPtr = characterTask->mainTaskCallbackFnPtr;
                characterTask->mainTaskCallbackFnPtr = HLib_SetTaskCallbackToInitCallback;
            }
            characterTask->mainExecutionIterations = 0;
        }
    }
}

void CPAM_Callback(Character *character, void *) {
    if (!character)
        return;
    float viewX{};
    float viewZ{};
    if (MOTM_GetWork(character)) {
        if (character->viewDistance > CAME_GetNpcDisplayRadius()) {
            character->renderOpacity = std::max(0.0f, static_cast<float>(static_cast<double>(character->renderOpacity) - 0.05));
            return;
        }
        viewX = character->viewPosition.x;
        viewZ = character->viewPosition.z;
    } else {
        const Vector3 position = GetPositionForEntity(character->entity.node);
        viewX = workTransformMatrix.rows[1].x * position.y + workTransformMatrix.rows[0].x * position.x + workTransformMatrix.rows[2].x * position.z +
            workTransformMatrix.rows[3].x;
        viewZ = workTransformMatrix.rows[1].z * position.y + workTransformMatrix.rows[0].z * position.x + workTransformMatrix.rows[2].z * position.z +
            workTransformMatrix.rows[3].z;
    }
    if (g_forceFullCharacterOpacity) {
        character->renderOpacity = 1.0f;
    } else if (viewZ >= 0.5f || viewZ <= -0.5f || viewX <= -0.5f || viewX >= 0.5f) {
        character->renderOpacity = std::min(1.0f, static_cast<float>(static_cast<double>(character->renderOpacity) + 0.05));
    } else {
        character->renderOpacity = std::max(0.0f, viewZ * -2.0f);
    }
}

void CPAM_Alloc(Character *character) {
    if (!character)
        return;
    character->renderOpacity = 0.0f;
    AllocateHeapMemoryMODL(character, SHEN_NAME(CPAM), sizeof(std::uint32_t), CPAM_Callback);
}
void CPAM_SetEnabled(Character *character, bool enabled) {
    if (!character)
        return;
    if (!character->componentTree) {
        character->renderOpacity = 1.0f;
        return;
    }
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, SHEN_NAME(CPAM));
    if (!component) {
        if (!enabled)
            character->renderOpacity = 1.0f;
        return;
    }
    if (enabled) {
        component->flags |= 1U;
    } else {
        component->flags &= ~1U;
        character->renderOpacity = 1.0f;
    }
}

namespace {} // namespace

bool g_suppressClothPose = false;
namespace {

#pragma pack(push, 4)
struct ComponentHeapBlock {
    ComponentHeapBlock *next;
    std::uint32_t usedSize;
    std::uint32_t reserved_0c;
};
#pragma pack(pop)

static_assert(sizeof(ComponentHeapBlock) == 0x10);

#pragma pack(push, 1)
struct MapEventWork {
    std::uint32_t flags;
    std::int32_t eventId;
    std::uint32_t reserved008;
    std::uint8_t eventFlags[32];
};
#pragma pack(pop)

static_assert(sizeof(MapEventWork) == 0x2c);
static_assert(offsetof(MapEventWork, eventFlags) == 0x0c);

#pragma pack(push, 1)
#pragma pack(push, 1)

#pragma pack(pop)

#pragma pack(push, 1)

#pragma pack(pop)

#pragma pack(pop)

} // namespace

namespace {} // namespace

CharacterCollisionWork *FindCharacterControlWork(Character *character) {
    CharacterComponentNode *node = character ? character->componentTree : nullptr;
    constexpr std::uint32_t kControlWork = SHEN_NAME(CCOW);
    while (node) {
        if (node->componentId == kControlWork)
            return reinterpret_cast<CharacterCollisionWork *>(node + 1);
        node = node->componentId < kControlWork ? node->greaterChild : node->lessChild;
    }
    return nullptr;
}
namespace {} // namespace

MotionManagerWork *FindMotionManagerWork(Character *character) {
    CharacterComponentNode *node = character ? character->componentTree : nullptr;
    constexpr std::uint32_t kMotionManager = SHEN_NAME(MOTM);
    while (node) {
        if (node->componentId == kMotionManager) {
            return reinterpret_cast<MotionManagerWork *>(node + 1);
        }
        node = node->componentId < kMotionManager ? node->greaterChild : node->lessChild;
    }
    return nullptr;
}
namespace {} // namespace

void AssignMotionWorldTarget(MotionInformationWork &motion, std::size_t slot, const Vector3 &target) {
    if (slot >= std::size(motion.targetPositions))
        return;
    motion.targetPositions[slot] = target;
    motion.localTargets[slot] = nullptr;
    motion.worldTargets[slot] = &motion.targetPositions[slot];
}

void LKPT_Callback(Character *character, void *payload) {
    auto *const lookPoint = static_cast<LookPointWork *>(payload);
    if (!lookPoint || lookPoint->active == 0)
        return;
    MotionManagerWork *const manager = FindMotionManagerWork(character);
    if (!manager) {
        const auto *const control =
            g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, character);
        return;
    }
    if ((manager->motion.motionFlags & 0x4000U) == 0) {
        lookPoint->active = 0;
        return;
    }
    AssignMotionWorldTarget(manager->motion, 2, lookPoint->target);
    manager->motion.motionFlags &= ~0x08000000U;
}

// The REFB component payload of a character, if it has one.
ReferenceBoardWork *FindReferenceBoardWork(Character *character) {
    CharacterComponentNode *node = character ? character->componentTree : nullptr;
    constexpr std::uint32_t kReferenceBoard = SHEN_NAME(REFB);
    while (node) {
        if (node->componentId == kReferenceBoard)
            return reinterpret_cast<ReferenceBoardWork *>(node + 1);
        node = node->componentId < kReferenceBoard ? node->greaterChild : node->lessChild;
    }
    return nullptr;
}

namespace {} // namespace

namespace {
#pragma pack(push, 1)

#pragma pack(pop)

} // namespace

namespace {} // namespace

void PushCharacterMatrix() {
    if (!g_matrixStack.top.stack)
        return;
    *g_matrixStack.top.stack = workTransformMatrix;
    if (++g_matrixStack.top.info.index < g_matrixStack.top.info.capacity) {
        ++g_matrixStack.top.stack;
    }
}
namespace {} // namespace

void PopCharacterMatrix() {
    Matrix4x4::Pop(1);
}
namespace {

void ApplyCharacterTransform(const Character &character) {
    TransformCharacterHierarchyZYX(&character.entity.node);
    if (character.scale.x != 1.0f || character.scale.y != 1.0f || character.scale.z != 1.0f) {
        Matrix4x4::Scale(character.scale.x, character.scale.y, character.scale.z);
    }
}

} // namespace

void TransformCharacterHierarchyZYX(const CharacterNode *node) {
    if (!node)
        return;
    TransformCharacterHierarchyZYX(node->child);
    Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
    if (node->rotation.z)
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node->rotation.z));
    if (node->rotation.y)
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node->rotation.y));
    if (node->rotation.x)
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node->rotation.x));
}

namespace {

void RecursiveRotateZYX(const CharacterNode *node) {
    if (!node)
        return;
    RecursiveRotateZYX(node->child);
    if (node->rotation.z)
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node->rotation.z));
    if (node->rotation.y)
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node->rotation.y));
    if (node->rotation.x)
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node->rotation.x));
}

void RotatePointByCurrentTransform(const CharacterNode *node, Vector3 &point) {
    Matrix4x4::SetActiveTransformToPrePass();
    RecursiveRotateZYX(node);
    const Matrix4x4 transform = workTransformMatrix;
    PopCharacterMatrix();

    const float x = point.x - transform.rows[3].x;
    const float y = point.y - transform.rows[3].y;
    const float z = point.z - transform.rows[3].z;
    point = {
        transform.rows[0].x * x + transform.rows[0].y * y + transform.rows[0].z * z,
        transform.rows[1].x * x + transform.rows[1].y * y + transform.rows[1].z * z,
        transform.rows[2].x * x + transform.rows[2].y * y + transform.rows[2].z * z,
    };
}

void shenApplyTranslationToAllInternal(CharacterNode &node, const Vector3 &worldPosition) {




    if (!node.child) {
        node.position = worldPosition;
        return;
    }

    Matrix4x4::SetActiveTransformToPrePass();
    TransformCharacterHierarchyZYX(node.child);
    const Matrix4x4 transform = workTransformMatrix;
    PopCharacterMatrix();

    const float x = worldPosition.x - transform.rows[3].x;
    const float y = worldPosition.y - transform.rows[3].y;
    const float z = worldPosition.z - transform.rows[3].z;
    node.position = {
        transform.rows[0].y * y + transform.rows[0].x * x + transform.rows[0].z * z,
        transform.rows[1].y * y + transform.rows[1].x * x + transform.rows[1].z * z,
        transform.rows[2].y * y + transform.rows[2].x * x + transform.rows[2].z * z,
    };
}
} // namespace

std::int32_t FixedAtan2ForEuler(float y, float x) {
    return atan2_fixed16(y, x);
}
namespace {

Vector3i EulerAnglesFromMatrix(const Matrix4x4 &matrix) {
    const float sineY = -matrix.rows[0].z;
    Vector3i result{};
    if (sineY >= 1.0f) {
        result.x = FixedAtan2ForEuler(matrix.rows[1].x, matrix.rows[1].y);
        result.y = 0x4000;
    } else if (sineY <= -1.0f) {
        result.x = FixedAtan2ForEuler(-matrix.rows[1].x, matrix.rows[1].y);
        result.y = 0xc000;
    } else {
        const float cosineY = std::sqrt(1.0f - sineY * sineY);
        result.x = FixedAtan2ForEuler(matrix.rows[1].z / cosineY, matrix.rows[2].z / cosineY);
        result.y = FixedAtan2ForEuler(sineY, cosineY);
        result.z = FixedAtan2ForEuler(matrix.rows[0].y / cosineY, matrix.rows[0].x / cosineY);
    }
    if (static_cast<std::uint16_t>(result.x) >= 0x8000U && static_cast<std::uint16_t>(result.z) >= 0x8000U) {
        result.x = static_cast<std::uint16_t>(result.x) - 0x8000;
        result.y = 0x8000 - static_cast<std::uint16_t>(result.y);
        result.z = static_cast<std::uint16_t>(result.z) - 0x8000;
    }
    return result;
}

void FIXO_Callback(Character *character, void *rawWork) {
    FixedObjectWork &work = *static_cast<FixedObjectWork *>(rawWork);
    const std::int16_t state = static_cast<std::int16_t>(work.state);
    if (state == 0)
        return;
    Matrix4x4 fallback;
    const Matrix4x4 *source = nullptr;
    if (state == 2) {
        source = reinterpret_cast<const Matrix4x4 *>(work.sourceNodeOrMatrix);
    } else {
        if (state == 1 && work.owner)
            source = GetTransformForFACE_ForSpecificChara(work.owner, static_cast<std::int32_t>(work.sourceNodeOrMatrix));
        if (!source) {
            Matrix4x4::SetActiveTransformToPrePass();
            TransformCharacterHierarchyZYX(work.owner ? &work.owner->entity.node : nullptr);
            fallback = workTransformMatrix;
            Matrix4x4::Pop(1);
            source = &fallback;
        }
    }
    Vector3 position = work.position;
    Vector3i rotation = work.rotation;
    if ((work.flags & 1U) != 0) {
        Matrix4x4_Push(source);
        Matrix4x4::preTranslate(work.position.x, work.position.y, work.position.z);
        Matrix4x4::SetViewMatrixFromEuler(work.rotation.y, work.rotation.x, work.rotation.z);
        Matrix4x4::preTranslate(work.offsetPosition.x, work.offsetPosition.y, work.offsetPosition.z);
        Matrix4x4::SetViewMatrixFromEuler(work.offsetRotation.y, work.offsetRotation.x, work.offsetRotation.z);
        const Matrix4x4 result = workTransformMatrix;
        position = {result.rows[3].x, result.rows[3].y, result.rows[3].z};
        Matrix4x4::Pop(1);
        rotation = EulerAnglesFromMatrix(result);
    } else {
        Matrix4x4_TransformMatrixAndCalculateEulerAngles(source, position, rotation);
    }
    if (!character)
        return;
    if (work.owner)
        character->renderOpacity = work.owner->renderOpacity;
    shenApplyTranslationToAllInternal(character->entity.node, position);
    ApplyEulerAnglesToNode(character->entity.node, rotation);
}

} // namespace
void Matrix4x4_TransformMatrixAndCalculateEulerAngles(const Matrix4x4 *transform, Vector3 &position, Vector3i &rotation) {
    Matrix4x4_Push();
    workTransformMatrix = transform ? *transform : *g_matrixStack.top.stack;
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    Matrix4x4::SetViewMatrixFromEuler(rotation.y, rotation.x, rotation.z);
    const Matrix4x4 result = workTransformMatrix;
    position = {result.rows[3].x, result.rows[3].y, result.rows[3].z};
    Matrix4x4::Pop(1);
    rotation = EulerAnglesFromMatrix(result);
}
void Shenmue_RotatePointByCurrentTransform(const CharacterNode *node, Vector3 &point) {
    RotatePointByCurrentTransform(node, point);
}
Vector3i CHAR_EulerAnglesFromMatrix(const Matrix4x4 &matrix) {
    return EulerAnglesFromMatrix(matrix);
}




void Matrix4x4_ExtractEulerAngles(Vector3i &angles) {
    const Matrix4x4 &matrix = workTransformMatrix;
    const float sineY = -matrix.rows[0].z;
    if (static_cast<double>(sineY) > 0.999999) {
        angles.x = Shenmue_Atan2Fixed(matrix.rows[1].x, matrix.rows[1].y);
        angles.y = 0x4000;
        angles.z = 0;
    } else if (static_cast<double>(sineY) < -0.999999) {
        angles.x = Shenmue_Atan2Fixed(-matrix.rows[1].x, matrix.rows[1].y);
        angles.y = 0xc000;
        angles.z = 0;
    } else {
        const float cosineY = std::sqrt(1.0f - sineY * sineY);
        angles.x = Shenmue_Atan2Fixed(matrix.rows[1].z / cosineY, matrix.rows[2].z / cosineY);
        angles.y = Shenmue_Atan2Fixed(sineY, cosineY);
        angles.z = Shenmue_Atan2Fixed(matrix.rows[0].y / cosineY, matrix.rows[0].x / cosineY);
    }
    if (static_cast<std::uint16_t>(angles.x) >= 0x8000U && static_cast<std::uint16_t>(angles.z) >= 0x8000U) {
        angles.x = static_cast<std::uint16_t>(angles.x) - 0x8000;
        angles.y = 0x8000 - static_cast<std::uint16_t>(angles.y);
        angles.z = static_cast<std::uint16_t>(angles.z) - 0x8000;
    }
    const std::int32_t x = angles.x;
    const std::int32_t y = angles.y;
    const std::int32_t z = angles.z;
    const std::int32_t pastRightAngle = (fixed16_angle_magnitude(x) > 0x4000) + (fixed16_angle_magnitude(y) > 0x4000) + (fixed16_angle_magnitude(z) > 0x4000);
    const std::int32_t flippedPastRightAngle =
        (fixed16_angle_magnitude(x + 0x8000) > 0x4000) + (fixed16_angle_magnitude(0x8000 - y) > 0x4000) + (fixed16_angle_magnitude(z - 0x8000) > 0x4000);
    if (pastRightAngle > flippedPastRightAngle) {
        angles.x = x - 0x8000;
        angles.z = z - 0x8000;
        angles.y = 0x8000 - y;
    }
    angles.x = static_cast<std::int16_t>(angles.x);
    angles.y = static_cast<std::int16_t>(angles.y);
    angles.z = static_cast<std::int16_t>(angles.z);
}

namespace {} // namespace


std::int32_t getMOTNType(std::int32_t nodeId) {
    switch (nodeId) {
    case 0:
        return 0;
    case 3:
        return 1;
    case 6:
        return 2;
    case 11:
        return 3;
    case 17:
        return 4;
    case 21:
        return 5;
    case 26:
        return 6;
    case 33:
        return 7;
    default:
        return -1;
    }
}

void MOTM_SetTarget(Character *character, std::int32_t nodeId, const Vector3 *target, std::uint32_t flags) {
    if (!character || !target)
        return;
    MotionManagerWork *const manager = FindMotionManagerWork(character);
    if (!manager)
        return;
    const std::int32_t slot = getMOTNType(nodeId);
    if (slot < 0)
        return;

    Vector3 &stored = manager->motion.targetPositions[slot];
    stored = *target;
    if ((flags & 0x40000000U) != 0) {
        manager->motion.localTargets[slot] = nullptr;
        manager->motion.worldTargets[slot] = &stored;
    } else if ((flags & 0x01000000U) != 0) {
        manager->motion.localTargets[slot] = &stored;
        manager->motion.worldTargets[slot] = nullptr;
    } else {
        if (character->entity.node.child)
            stored = TransformPointByEntity(*character->entity.node.child, stored);
        manager->motion.localTargets[slot] = nullptr;
        manager->motion.worldTargets[slot] = &stored;
    }
}

Vector3 GetPositionForEntity(CharacterNode &node) {
    if (!node.child)
        return node.position;

    Matrix4x4::SetActiveTransformToPrePass();
    TransformCharacterHierarchyZYX(&node);
    const Vector3 result{
        workTransformMatrix.rows[3].x,
        workTransformMatrix.rows[3].y,
        workTransformMatrix.rows[3].z,
    };
    PopCharacterMatrix();
    return result;
}
Vector3 TransformPointByEntity(CharacterNode &node, const Vector3 &point) {
    Matrix4x4::SetActiveTransformToPrePass();
    TransformCharacterHierarchyZYX(&node);
    const Matrix4x4 transform = workTransformMatrix;
    PopCharacterMatrix();
    return {
        point.x * transform.rows[0].x + point.y * transform.rows[1].x + point.z * transform.rows[2].x + transform.rows[3].x,
        point.x * transform.rows[0].y + point.y * transform.rows[1].y + point.z * transform.rows[2].y + transform.rows[3].y,
        point.x * transform.rows[0].z + point.y * transform.rows[1].z + point.z * transform.rows[2].z + transform.rows[3].z,
    };
}

Vector3 WorldToEntityLocalPoint(CharacterNode &node, const Vector3 &point) {
    Matrix4x4::SetActiveTransformToPrePass();
    TransformCharacterHierarchyZYX(&node);
    const Matrix4x4 transform = workTransformMatrix;
    PopCharacterMatrix();
    const float x = point.x - transform.rows[3].x;
    const float y = point.y - transform.rows[3].y;
    const float z = point.z - transform.rows[3].z;
    return {transform.rows[0].x * x + transform.rows[0].y * y + transform.rows[0].z * z,
        transform.rows[1].x * x + transform.rows[1].y * y + transform.rows[1].z * z,
        transform.rows[2].x * x + transform.rows[2].y * y + transform.rows[2].z * z};
}
Vector3i GetEulerAnglesForEntity(CharacterNode &node) {
    if (!node.child)
        return node.rotation;

    Matrix4x4::SetActiveTransformToPrePass();
    TransformCharacterHierarchyZYX(&node);
    const Matrix4x4 transform = workTransformMatrix;
    PopCharacterMatrix();
    return EulerAnglesFromMatrix(transform);
}

std::int32_t charaGetWorldYaw(Character *character) {
    if (!character)
        return 0;
    if (MotionManagerWork *const manager = MOTM_GetWork(character)) {
        const auto yaw = static_cast<std::uint16_t>(manager->motion.extractedYaw);
        const auto auxiliary = static_cast<std::uint16_t>(manager->motion.extractedYawAux);
        return static_cast<std::int32_t>(static_cast<std::uint32_t>(yaw) | (static_cast<std::uint32_t>(auxiliary) << 16U));
    }

    std::int16_t yaw = 0;
    for (CharacterNode *node = &character->entity.node; node; node = node->child) {
        yaw = static_cast<std::int16_t>(yaw + static_cast<std::int16_t>(node->rotation.y));
    }
    return yaw;
}

std::uint32_t cpctGetAccumYaw(Character *character) {
    if (!character)
        return 0;
    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!manager)
        return static_cast<std::uint32_t>(character->entity.node.rotation.y);

    std::int16_t yaw = manager->motion.extractedYaw;
    for (CharacterNode *node = character->entity.node.child; node; node = node->child) {
        yaw = static_cast<std::int16_t>(yaw - static_cast<std::int16_t>(node->rotation.y));
    }
    return static_cast<std::uint32_t>(static_cast<std::int32_t>(yaw));
}

void ApplyEulerAnglesToNode(CharacterNode &node, const Vector3i &eulerAngles) {
    if (!node.child) {
        node.rotation = eulerAngles;
        return;
    }

    Matrix4x4::SetActiveTransformToPrePass();
    TransformCharacterHierarchyZYX(node.child);
    Matrix4x4::preTranslate(node.position.x, node.position.y, node.position.z);
    const Vector3 worldPosition{
        workTransformMatrix.rows[3].x,
        workTransformMatrix.rows[3].y,
        workTransformMatrix.rows[3].z,
    };
    Matrix4x4::InvertInPlace(nullptr);
    Matrix4x4::preTranslate(worldPosition.x, worldPosition.y, worldPosition.z);
    Matrix4x4::SetViewMatrixFromEuler(eulerAngles.y, eulerAngles.x, eulerAngles.z);
    const Matrix4x4 localRotation = workTransformMatrix;
    PopCharacterMatrix();
    node.rotation = EulerAnglesFromMatrix(localRotation);
}

void shenApplyTranslationToAll(CharacterNode &node, const Vector3 &worldPosition) {
    shenApplyTranslationToAllInternal(node, worldPosition);
}

CharacterComponentNode *HLib_GetCharacterComponent(Character *character, std::uint32_t componentId) {
    CharacterComponentNode *node = character ? character->componentTree : nullptr;
    while (node) {
        if (node->componentId == componentId)
            return node;
        node = node->componentId < componentId ? node->greaterChild : node->lessChild;
    }
    return nullptr;
}

void SetHeapMemoryFreeCallback(Character *character, std::uint32_t componentId, void (*destroyCallback)(Character *, void *)) {
    if (character)
        HLib_GetCharacterComponent(character, componentId)->destroyCallback = destroyCallback;
}

void CEWP_ToggleVisibility_Callback(Character *character, std::int32_t enabled) {
    if (!character)
        return;
    if (enabled)
        character->flags.value |= CharacterFlagVisible;
    else
        character->flags.value &= ~CharacterFlagVisible;
}

std::int32_t GetObjectAngle(const CharacterNode *node) {
    return node ? node->rotation.z : 0;
}

void GetEntityNodePos(Character *character, Vector3 *output) {
    *output = character ? GetPositionForEntity(character->entity.node) : Vector3{};
}

CharacterNode *shenGetNodeFromChara(Character *character) {
    return character ? &character->entity.node : nullptr;
}

void NPCLogger(Character *character, std::int32_t index, std::int32_t enabled) {
    constexpr std::array<std::uint8_t, 7> masks{0x3f, 1, 2, 4, 8, 0x10, 0x20};
    if (!character || index >= static_cast<std::int32_t>(masks.size()))
        return;
    const std::uint8_t mask = masks[static_cast<std::size_t>(index)];
    const bool previous = (character->motionFlags & mask) != 0;
    if (enabled)
        character->motionFlags |= mask;
    else
        character->motionFlags &= static_cast<std::uint8_t>(~mask);
    const bool current = (character->motionFlags & mask) != 0;
    if (g_bDebugLogVisibility && (character->flags.value & CharacterFlagHasCpct) != 0 && current != previous)
        CHAR_LogNPCVisibility(character, current);
}

void RotateAndPostTranslate(Character *character) {
    if (character)
        ApplyCharacterTransform(*character);
}

std::uint32_t charGetLightMask(const Character *character) {
    return character ? character->lightMask : 0;
}

float charGetLightRadius(const Character *character) {
    return character ? character->lightRadius : 0.0f;
}

std::uint32_t charaGetAvoidanceRes(Character *character) {
    CharacterCollisionWork *const work = FindCharacterControlWork(character);
    return work ? work->avoidanceResult : 0;
}

float cewpGetSpeedFor(Character *character) {
    CharacterCollisionWork *const work = FindCharacterControlWork(character);
    return work ? work->radialAabbExtent : 0.40000001f;
}

MotionManagerWork *MOTM_GetWork(Character *character) {
    return FindMotionManagerWork(character);
}


MotionInformationWork *MOTN_getMOTM(Character *character) {
    MotionManagerWork *const manager = FindMotionManagerWork(character);
    return manager ? &manager->motion : nullptr;
}

float scenGetCharaPlayRateScaledRemain(ScenarioContext *context, shen_name characterId) {
    MotionManagerWork *const manager = MOTM_GetWork(SCEN_FindCharacterByFunctionId(context, characterId.name));
    return manager ? MOTI_GetPlayRateScaledRemain(manager->motion) : 0.0f;
}

void scenCharaSetMotForcedFrame(ScenarioContext *context, shen_name characterId, float frame) {
    MotionManagerWork *const manager = MOTM_GetWork(SCEN_FindCharacterByFunctionId(context, characterId.name));
    if (manager && frame > 0.0f)
        manager->motion.forcedFrameTime = frame;
}

void scenCharaSetMotEndFrame(ScenarioContext *context, shen_name characterId, float frame) {
    MotionManagerWork *const manager = MOTM_GetWork(SCEN_FindCharacterByFunctionId(context, characterId.name));
    if (manager && frame > 0.0f)
        manager->motion.scriptEndFrame = frame;
}

void scenToggleCharaMotEnabled(ScenarioContext *context, shen_name characterId, bool enabled) {
    MotionManagerWork *const manager = MOTM_GetWork(SCEN_FindCharacterByFunctionId(context, characterId.name));
    if (!manager)
        return;
    if (enabled)
        manager->flags |= 1U;
    else
        manager->flags &= ~1U;
}

void scenSetCharaPos(ScenarioContext *context, shen_name characterId, const Vector3 &position) {
    if (Character *const character = SCEN_FindCharacterByFunctionId(context, characterId.name)) {
        character->entity.node.position = position;
    }
}

const Matrix4x4 *GetTransformForFACE_ForSpecificChara(Character *character, std::int32_t boneId) {
    MotionManagerWork *const manager = character ? MOTM_GetWork(character) : nullptr;
    const auto *node = manager ? static_cast<const MotionNodeRuntime *>(manager->motion.previousNodes) : nullptr;
    if (!node)
        return nullptr;
    while (static_cast<std::int8_t>(node->nodeId) != boneId) {
        if (node->nodeId == 0xffU)
            return nullptr;
        ++node;
    }
    return node->transform;
}

void mtwkGetCharaBonePosition(Character *character, std::int32_t nodeId, Vector3 *position) {
    if (!character) {
        *position = {};
        return;
    }
    MotionManagerWork *const manager = MOTM_GetWork(character);
    const MotionNodeRuntime *const row = manager ? MOTN_FindNode(static_cast<MotionNodeRuntime *>(manager->motion.previousNodes), nodeId) : nullptr;
    if (!row) {
        *position = GetPositionForEntity(character->entity.node);
        return;
    }
    *position = {row->transform->rows[3].x, row->transform->rows[3].y, row->transform->rows[3].z};
}

void scenGetCharaBonePos(ScenarioContext *context, shen_name characterId, std::uint8_t nodeId, Vector3 &position) {
    mtwkGetCharaBonePosition(SCEN_FindCharacterByFunctionId(context, characterId.name), nodeId, &position);
}

void scenGetCharaPos(ScenarioContext *context, shen_name characterId, Vector3 &position) {
    Character *const character = SCEN_FindCharacterByFunctionId(context, characterId.name);
    position = character ? character->entity.node.position : Vector3{};
}

void scenSetCharaRot(ScenarioContext *context, shen_name characterId, const Vector3i &rotation) {
    if (Character *const character = SCEN_FindCharacterByFunctionId(context, characterId.name)) {
        character->entity.node.rotation = rotation;
    }
}

void scenGetCharaRot(ScenarioContext *context, shen_name characterId, Vector3i &rotation) {
    if (Character *const character = SCEN_FindCharacterByFunctionId(context, characterId.name)) {
        rotation = character->entity.node.rotation;
    }
}

void scenApplyCharaRot(ScenarioContext *context, shen_name targetId, shen_name sourceId, const Vector3 &offset, std::int16_t yawOffset) {
    Character *const source = SCEN_FindCharacterByFunctionId(context, sourceId.name);
    Character *const target = SCEN_FindCharacterByFunctionId(context, targetId.name);
    if (!target)
        return;

    const Vector3 sourcePosition = source ? source->entity.node.position : Vector3{};
    const Vector3i sourceRotation = source ? source->entity.node.rotation : Vector3i{};
    const std::uint16_t yaw = static_cast<std::uint16_t>(sourceRotation.y + yawOffset);
    target->entity.node.rotation = {
        sourceRotation.x,
        static_cast<std::int32_t>(yaw),
        sourceRotation.z,
    };
    const float sine = sin_fixed16(static_cast<std::int16_t>(yaw));
    const float cosine = sin_fixed16(fixed16_cosine_angle(yaw));
    target->entity.node.position = {
        sourcePosition.x + cosine * offset.x + sine * offset.z,
        sourcePosition.y,
        sourcePosition.z + cosine * offset.z - sine * offset.x,
    };
}

bool shenPushCharaAwayFromOtherChara(ScenarioContext *context, shen_name targetId, shen_name sourceId) {
    Vector3 sourceExtremities[4]{};
    scenGetCharaBonePos(context, sourceId, 0x0c, sourceExtremities[0]);
    scenGetCharaBonePos(context, sourceId, 0x12, sourceExtremities[1]);
    scenGetCharaBonePos(context, sourceId, 0x1b, sourceExtremities[2]);
    scenGetCharaBonePos(context, sourceId, 0x22, sourceExtremities[3]);

    Vector3 targetSpine{};
    Vector3 sourceSpine{};
    scenGetCharaBonePos(context, targetId, 2, targetSpine);
    scenGetCharaBonePos(context, sourceId, 2, sourceSpine);

    float minimum = std::numeric_limits<float>::max();
    for (const Vector3 &extremity : sourceExtremities) {
        const float deltaX = targetSpine.x - extremity.x;
        const float deltaZ = targetSpine.z - extremity.z;
        minimum = std::min(minimum, deltaX * deltaX + deltaZ * deltaZ);
    }
    if (minimum >= 0.3f)
        return false;

    const std::uint16_t yaw = static_cast<std::uint16_t>(Shenmue_Atan2Fixed(targetSpine.x - sourceSpine.x, targetSpine.z - sourceSpine.z) + 0x8000U);
    const double displacement = 0.3 - static_cast<double>(minimum);
    Character *const target = SCEN_FindCharacterByFunctionId(context, targetId.name);
    if (target) {
        target->entity.node.position.x += static_cast<float>(static_cast<double>(-sin_fixed16(static_cast<std::int16_t>(yaw))) * displacement);
        target->entity.node.position.z += static_cast<float>(static_cast<double>(-sin_fixed16(fixed16_cosine_angle(yaw))) * displacement);
    }
    return true;
}
namespace {
Mt5RuntimeNode *FindModelNodeByKind(Mt5RuntimeNode *node, std::int16_t nodeKind);
} // namespace

FixedObjectWork *FindFixedObjectWork(Character *character) {
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, SHEN_NAME(FIXO));
    return component ? reinterpret_cast<FixedObjectWork *>(component + 1) : nullptr;
}

void AllocateFIXO(Character *character) {
    if (!character || FindFixedObjectWork(character))
        return;
    if (!AllocateHeapMemoryMODL(character, SHEN_NAME(FIXO), sizeof(FixedObjectWork), nullptr))
        return;
    HLib_GetCharacterComponent(character, SHEN_NAME(FIXO))->drawCallback = FIXO_Callback;
}

void fixoSetCharaPos(Character *target, Character *owner, std::int32_t nodeId, const Vector3 *position, const Vector3i *rotation) {
    FixedObjectWork *const work = FindFixedObjectWork(target);
    if (!work || !position || !rotation)
        return;
    work->state = 3;
    if (GetTransformForFACE_ForSpecificChara(owner, nodeId)) {
        work->state = 1;
        std::memcpy(&work->sourceNodeOrMatrix, &nodeId, sizeof(nodeId));
    }

    work->owner = owner;
    work->position = *position;
    work->rotation = *rotation;
    work->attachmentPosition = *position;
    work->attachmentRotation = *rotation;
}
namespace {
void CalculateMatrixRotation(const Matrix4x4 &source, Vector3 &position, Vector3i &rotation) {
    Matrix4x4_Push(&source);
    Matrix4x4::InvertInPlace(nullptr);
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    Matrix4x4::SetViewMatrixFromEuler(rotation.y, rotation.x, rotation.z);
    const Matrix4x4 result = workTransformMatrix;
    position = {result.rows[3].x, result.rows[3].y, result.rows[3].z};
    Matrix4x4::Pop(1);
    rotation = EulerAnglesFromMatrix(result);
}

void CalculateMatrixRotationFromParams(Character *character, FixedObjectWork &work, const Matrix4x4 &source) {
    Matrix4x4::SetActiveTransformToPrePass();
    TransformCharacterHierarchyZYX(character ? &character->entity.node : nullptr);
    if ((work.flags & 1U) != 0) {
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(-work.offsetRotation.x));
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-work.offsetRotation.y));
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(-work.offsetRotation.z));
        Matrix4x4::preTranslate(-work.offsetPosition.x, -work.offsetPosition.y, -work.offsetPosition.z);
    }
    work.position = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    const Matrix4x4 current = workTransformMatrix;
    Matrix4x4::Pop(1);
    work.rotation = EulerAnglesFromMatrix(current);
    CalculateMatrixRotation(source, work.position, work.rotation);
}
} // namespace

void CalculateMatrixRotationFromParamsHelper(Character *target, Character *source, std::int32_t nodeId) {
    FixedObjectWork *const work = FindFixedObjectWork(target);
    if (!work)
        return;
    const Matrix4x4 *transform = GetTransformForFACE_ForSpecificChara(source, nodeId);
    Matrix4x4 fallback;
    if (transform) {
        std::memcpy(&work->sourceNodeOrMatrix, &nodeId, sizeof(nodeId));
        work->state = 1;
        work->owner = source;
    } else {
        work->owner = source;
        work->state = 3;
        Matrix4x4::SetActiveTransformToPrePass();
        TransformCharacterHierarchyZYX(source ? &source->entity.node : nullptr);
        fallback = workTransformMatrix;
        Matrix4x4::Pop(1);
        transform = &fallback;
    }
    CalculateMatrixRotationFromParams(target, *work, *transform);
}

void fixoSetAttachmentOffset(Character *character, const Vector3 *position, const Vector3i *rotation) {
    FixedObjectWork *const work = FindFixedObjectWork(character);
    if (!work || !position || !rotation)
        return;
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(-rotation->x));
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-rotation->y));
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(-rotation->z));
    Matrix4x4::preTranslate(-position->x, -position->y, -position->z);
    const Matrix4x4 inverse = workTransformMatrix;
    work->offsetPosition = {inverse.rows[3].x, inverse.rows[3].y, inverse.rows[3].z};
    Matrix4x4::Pop(1);
    work->offsetRotation = EulerAnglesFromMatrix(inverse);
    work->flags = static_cast<std::uint8_t>(work->flags | 1U);
}

namespace {
Matrix4x4 g_chainRootTransformMatrix;

bool Recursive_TranslateRotate(Matrix4x4 *output, std::byte *modelBase, const Mt5RuntimeNode *node, const Mt5RuntimeNode *stop) {
    const Mt5RuntimeNode *const sibling = node->siblingOffset ? reinterpret_cast<const Mt5RuntimeNode *>(modelBase + node->siblingOffset) : nullptr;
    if (sibling)
        Matrix4x4_Push();
    if (node->transformOffset) {
        workTransformMatrix = g_chainRootTransformMatrix;
        Matrix4x4::Multiply(reinterpret_cast<const Matrix4x4 *>(modelBase + node->transformOffset));
    } else {
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node->rotation.z));
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node->rotation.y));
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node->rotation.x));
        Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
    }
    *(output ? output : g_matrixStack.top.stack) = workTransformMatrix;
    if (node == stop) {
        if (sibling)
            Matrix4x4::Pop(1);
        return true;
    }
    if (node->childOffset && Recursive_TranslateRotate(output, modelBase, reinterpret_cast<const Mt5RuntimeNode *>(modelBase + node->childOffset), stop)) {
        if (sibling)
            Matrix4x4::Pop(1);
        return true;
    }
    if (!sibling)
        return false;
    Matrix4x4::Pop(1);
    return Recursive_TranslateRotate(output, modelBase, sibling, stop);
}
} // namespace

void shenChainToTargetEval(Matrix4x4 *output, const Mt5RuntimeNode *root, const Mt5RuntimeNode *stop) {
    Matrix4x4_Push();
    g_chainRootTransformMatrix = workTransformMatrix;
    Recursive_TranslateRotate(output, MT5_ModelBaseForNode(root), root, stop);
    Matrix4x4::Pop(1);
}

bool fixoSetAttachmentOffsetFromNode(Character *character, std::int16_t nodeKind) {
    Mt5RuntimeNode *const root = character ? static_cast<Mt5RuntimeNode *>(character->model) : nullptr;
    Mt5RuntimeNode *const node = shenGetNodeByChildIdx(root, nodeKind);
    if (!node)
        return false;
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4 target;
    shenChainToTargetEval(&target, root, node);
    const Vector3 position{target.rows[3].x, target.rows[3].y, target.rows[3].z};
    Matrix4x4::Pop(1);
    const Vector3i rotation = EulerAnglesFromMatrix(target);
    fixoSetAttachmentOffset(character, &position, &rotation);
    return true;
}

void fixoClearAttachment(Character *character) {
    FixedObjectWork *const work = FindFixedObjectWork(character);
    if (!work)
        return;
    work->attachmentPosition = {};
    work->attachmentRotation = {};
    work->state = 0;
    work->flags = 0;
    work->reserved33 = 0;
}

void SetCharaLookAtPosition(Character *character, std::int32_t angle, const Vector3 *target, std::int32_t mode) {
    MotionManagerWork *const manager = FindMotionManagerWork(character);
    if (!manager) {
        const auto *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, character);
        return;
    }
    auto &motion = manager->motion;
    if (motion.motionControlLock == 1U)
        return;
    motion.motionFlags &= ~0x08000000U;

    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, SHEN_NAME(LKPT));
    auto *lookPoint = component ? reinterpret_cast<LookPointWork *>(component + 1) :
                                  static_cast<LookPointWork *>(AllocateHeapMemoryMODL(character, SHEN_NAME(LKPT), sizeof(LookPointWork), LKPT_Callback));
    if (!lookPoint)
        return;
    lookPoint->moveRequestLook = 0;
    if (lookPoint->active == 0)
        motion.headLookTimer = 0;
    if (angle < 0 && ((motion.rootAngleFlags & 0x40U) != 0 || lookPoint->active != 1)) {
        return;
    }

    lookPoint->target = target ? *target : motion.rootOffsetPosition;
    motion.motionFlags |= 0x4000U;
    motion.rootAngleFlags &= 0xffe359bcU;
    motion.headLookPreviousTimer = motion.headLookAngleBase;
    motion.lookAtModeBackup = motion.lookAtMode;
    motion.turnCurveState[0] = motion.currentTurnAngle;
    motion.lookAtBlendTimer = 0;
    motion.lookAtHoldTimer = 0;
    motion.turnCurveState[1] = 0;
    motion.turnCurveState[2] = 0;
    motion.lookAtState = 0;
    motion.lookAtFlags = 0;
    motion.turnCurveState[3] = 0;
    AssignMotionWorldTarget(motion, 2, lookPoint->target);



    constexpr std::array<std::uint16_t, 22> kLookAngleCommands{
        4096,
        2048,
        512,
        128,
        64,
        32,
        4096,
        2048,
        32770,
        32771,
        32772,
        32774,
        32776,
        32778,
        2048,
        32870,
        32872,
        32876,
        2048,
        2048,
        32869,
        2048,
    };
    std::uint32_t magnitude = static_cast<std::uint32_t>(angle);
    if (angle < 0) {
        const float absoluteAngle = std::fabs(static_cast<float>(angle));
        magnitude = angle == -1 ? 12U : absoluteAngle < 2147483648.0f ? static_cast<std::uint32_t>(static_cast<std::int32_t>(absoluteAngle)) : 0x80000000U;
    }
    std::uint16_t command = magnitude < kLookAngleCommands.size() ? kLookAngleCommands[magnitude] : static_cast<std::uint16_t>(magnitude);
    std::int16_t signedCommand = static_cast<std::int16_t>(command);
    if (signedCommand >= 0) {
        signedCommand = signedCommand >= 1280 ? (signedCommand >= 2304 ? static_cast<std::int16_t>(0x8066) : static_cast<std::int16_t>(0x8068)) :
                                                static_cast<std::int16_t>(0x806a);
    } else {
        const std::uint16_t type = command & 0x7fffU;
        if (type < 101U || type > 106U) {
            signedCommand = type <= 4U ? static_cast<std::int16_t>(0x8066) :
                type <= 10U            ? static_cast<std::int16_t>(0x8068) :
                                         static_cast<std::int16_t>(0x806a);
        }
    }
    if (motion.motionControlLock != 0)
        signedCommand = static_cast<std::int16_t>(0x8066);
    motion.headLookAngleCommand = static_cast<std::uint32_t>(static_cast<std::int32_t>(signedCommand));

    if (angle >= 0) {
        lookPoint->active = 1;
        motion.rootAngleFlags |= 0x200000U;
        switch (mode) {
        case 1:
            motion.motionFlags = (motion.motionFlags & ~0x800000U) | 0x400000U;
            motion.rootAngleFlags &= ~0x20000U;
            break;
        case 2:
            motion.motionFlags |= 0xc00000U;
            motion.rootAngleFlags &= ~0x20000U;
            break;
        case 3:
            motion.motionFlags |= 0xc00000U;
            motion.rootAngleFlags |= 0x20000U;
            break;
        default:
            motion.motionFlags &= ~0xc00000U;
            motion.rootAngleFlags &= ~0x20000U;
            break;
        }
    } else {
        lookPoint->active = 0;
        motion.rootAngleFlags |= 0x40U;
        motion.headLookTimer = 0;
    }
}

void SetLookPointDetails(Character *character, std::int32_t angle, const Vector3 *target, std::int32_t mode) {
    MotionManagerWork *const manager = FindMotionManagerWork(character);
    if (!manager) {
        const auto *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, character);
        return;
    }
    if (manager->motion.motionControlLock == 1U)
        return;
    auto &motion = manager->motion;
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, SHEN_NAME(LKPT));
    auto *const lookPoint = component ? reinterpret_cast<LookPointWork *>(component + 1) : nullptr;
    if (!lookPoint || lookPoint->active == 0 || (motion.rootAngleFlags & 0x40U) != 0) {
        SetCharaLookAtPosition(character, angle, target, mode);
        return;
    }

    if (target) {
        if (lookPoint->currentCommand == 0x8065U)
            motion.headLookAngleCommand = static_cast<std::uint32_t>(static_cast<std::int32_t>(lookPoint->savedCommand));
        if (motion.headLookAngleCommand != 0x8065U) {
            lookPoint->savedCommand = static_cast<std::int16_t>(motion.headLookAngleCommand);
        }
        lookPoint->currentCommand = motion.headLookAngleCommand;
        lookPoint->target = *target;
    } else {
        lookPoint->target = motion.rootOffsetPosition;
    }
    lookPoint->moveRequestLook = 0;
}

void *AllocateHeapMemoryMODL(Character *character, std::uint32_t componentId, std::int32_t payloadSize, void (*updateCallback)(Character *, void *)) {
    if (!character || payloadSize < 0)
        return nullptr;

    const std::uint32_t allocationSize = (static_cast<std::uint32_t>(payloadSize) + 67u) & ~3u;
    std::uint32_t blockCapacity = 512;
    ComponentHeapBlock *block = nullptr;
    if (allocationSize >= blockCapacity) {
        blockCapacity = allocationSize;
    } else {
        block = static_cast<ComponentHeapBlock *>(character->componentAllocationBlocks);
        while (block && allocationSize + block->usedSize > blockCapacity) {
            block = block->next;
        }
    }

    if (!block) {
        block = static_cast<ComponentHeapBlock *>(HLib_AllocHeapMemBlock(blockCapacity + sizeof(ComponentHeapBlock), SHEN_NAME(MODL)));
        if (!block)
            return nullptr;
        block->usedSize = 0;
        block->reserved_0c = 0;
        block->next = static_cast<ComponentHeapBlock *>(character->componentAllocationBlocks);
        character->allocatedSize = static_cast<std::uint16_t>(character->allocatedSize + blockCapacity + sizeof(ComponentHeapBlock));
        character->componentAllocationBlocks = block;
    }

    auto *const node = reinterpret_cast<CharacterComponentNode *>(reinterpret_cast<std::byte *>(block) + sizeof(ComponentHeapBlock) + block->usedSize);
    block->usedSize += allocationSize;
    std::memset(node, 0, allocationSize);
    node->flags = 1;
    node->updateCallback = updateCallback;
    node->componentId = componentId;

    CharacterComponentNode **insertion = &character->componentTree;
    while (*insertion) {
        if ((*insertion)->componentId == componentId)
            break;
        insertion = (*insertion)->componentId < componentId ? &(*insertion)->greaterChild : &(*insertion)->lessChild;
    }
    if (!*insertion)
        *insertion = node;

    CharacterComponentNode **tail = &character->componentList;
    while (*tail)
        tail = &(*tail)->next;
    *tail = node;
    return node + 1;
}
void CHAR_DestroyAndFreeComponents(Character *character) {
    if (!character)
        return;
    for (CharacterComponentNode *component = character->componentList; component; component = component->next) {
        if (component->destroyCallback) {
            component->destroyCallback(character, component + 1);
        }
    }

    auto freeBlocks = [](void *head) {
        auto *block = static_cast<ComponentHeapBlock *>(head);
        while (block) {
            ComponentHeapBlock *const next = block->next;
            HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(block), 0);
            block = next;
        }
    };
    freeBlocks(character->componentAllocationBlocks);
    freeBlocks(character->unknownAllocationBlocks);
    character->entity.name = 0;
}

void ccowEnableCharaFlags(Character *character, std::uint32_t flags) {
    if (auto *const control = FindCharacterControlWork(character)) {
        control->stateFlags |= flags;
        control->stateByte = 0;
    }
}
void ccowSetGroundDropLimit(Character *character, float value) {
    if (CharacterCollisionWork *const control = FindCharacterControlWork(character))
        control->groundDropLimit = value;
}
bool CCOW_UpdateCandidateObject(Character *character, std::int32_t mode, std::uint32_t objectId) {
    CharacterCollisionWork *const control = FindCharacterControlWork(character);
    if (!control)
        return false;

    if (mode == 0) {
        control->candidateObjectCount = 0;
        for (std::uint32_t &candidate : control->candidateObjects)
            candidate = 0;
        return true;
    }

    const auto findObject = [control](std::uint32_t value) {
        for (std::size_t index = 0; index < 8; ++index) {
            if (control->candidateObjects[index] == value)
                return index;
        }
        return std::size_t{8};
    };

    if (mode == 1)
        return findObject(objectId) != 8;

    if (mode == 2) {
        if (!objectId || findObject(objectId) != 8)
            return false;
        const std::size_t index = findObject(0);
        if (index == 8)
            return false;
        control->candidateObjects[index] = objectId;
        ++control->candidateObjectCount;
        return true;
    }

    if (mode == 3 && objectId) {
        const std::size_t index = findObject(objectId);
        if (index == 8)
            return false;
        control->candidateObjects[index] = 0;
        --control->candidateObjectCount;
        return true;
    }
    return false;
}

void ccowDisableCharaFlags(Character *character, std::uint32_t flags) {
    if (auto *const control = FindCharacterControlWork(character)) {
        control->stateByte = 0;
        control->stateFlags &= ~flags;
    }
}

bool ccowHasAnyCharaFlags(Character *character, std::uint32_t flags) {
    const auto *const control = FindCharacterControlWork(character);
    return control && (control->stateFlags & flags) != 0;
}

std::int32_t ccowSetOrGetValue(Character *character, std::int32_t value) {
    CharacterCollisionWork *const control = FindCharacterControlWork(character);
    if (!control)
        return 0;
    if (value >= 0)
        control->reserved004 = static_cast<std::uint8_t>(value);
    return static_cast<std::int8_t>(control->reserved004);
}
void ccowSetDistanceInteractLimits(Character *character, float distanceLimit, std::int32_t interactionHeightBits, std::int32_t maxAngle) {
    CharacterCollisionWork *const control = FindCharacterControlWork(character);
    if (!control)
        return;
    control->distanceLimit = distanceLimit;
    control->interactionHeight = std::bit_cast<float>(interactionHeightBits);
    control->maxAngle = maxAngle;
}
void CCOW_ApplyAuthoredDefinition(Character *character, std::int8_t level, bool disabled, std::uint32_t stateFlags, std::uint32_t maxAngle, float distanceLimit,
    std::uint32_t interactionHeightBits, const Vector3 &shapeExtents, float radialExtent, std::uint32_t shapeMode, std::uint32_t weightBits) {
    CharacterCollisionWork *const control = FindCharacterControlWork(character);
    if (!control)
        return;
    CCOW_ResetState(control);
    control->reserved004 = static_cast<std::uint8_t>(level);
    control->stateByte = disabled ? 1U : 0U;
    control->stateFlags = disabled ? 0U : stateFlags;
    control->maxAngle = static_cast<std::int32_t>(maxAngle);
    control->distanceLimit = distanceLimit;
    control->interactionHeight = std::bit_cast<float>(interactionHeightBits);
    control->shapeExtents = shapeExtents;
    control->radialAabbExtent = radialExtent;
    control->shape.mode = static_cast<IntersectionTestType>(shapeMode);
    control->reserved0b0 = std::bit_cast<std::int32_t>(weightBits);
    CCOW_RefreshAuthoredShape(character);
}

bool ccowConsumeInteractionFlag(Character *character) {
    CharacterCollisionWork *const control = FindCharacterControlWork(character);
    if (!control)
        return false;
    const bool wasSet = (control->interactionState & 1U) != 0;
    control->interactionState &= ~1U;
    return wasSet;
}

MapEventWork *FindMapEventWork(Character *character) {
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, SHEN_NAME(MEVW));
    return component ? reinterpret_cast<MapEventWork *>(component + 1) : nullptr;
}

void mapEventSetEnabled(Character *character, std::uint32_t index, bool enabled) {
    MapEventWork *const work = FindMapEventWork(character);
    if (!work || index >= sizeof(work->eventFlags) * 8U)
        return;
    std::uint8_t &byte = work->eventFlags[index >> 3U];
    const auto mask = static_cast<std::uint8_t>(1U << (index & 7U));
    if (enabled)
        byte = static_cast<std::uint8_t>(byte | mask);
    else
        byte = static_cast<std::uint8_t>(byte & ~mask);
}

bool mapEventIsEnabled(Character *character, std::uint32_t index) {
    const MapEventWork *const work = FindMapEventWork(character);
    if (!work || index >= sizeof(work->eventFlags) * 8U)
        return false;
    return (work->eventFlags[index >> 3U] & static_cast<std::uint8_t>(1U << (index & 7U))) != 0;
}

std::int32_t mapEventCurrentId(Character *character) {
    const MapEventWork *const work = FindMapEventWork(character);
    return work ? work->eventId : 0;
}

void motmEnableFlags(Character *character, std::uint32_t flags) {
    if (auto *const state = FindMotionManagerWork(character))
        state->flags |= flags;
}

void motmDisableFlags(Character *character, std::uint32_t flags) {
    if (auto *const state = FindMotionManagerWork(character))
        state->flags &= ~flags;
}

bool motmSetBasePoseMode(Character *character, std::int32_t mode) {
    auto *const manager = FindMotionManagerWork(character);
    if (!manager) {
        const auto *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
        SCEN_PromoteCharacterIndexNode(control ? control->scenarioTask : nullptr, character);
        return false;
    }
    auto &motion = manager->motion;
    motion.motionFlags |= 0x4000;
    switch (mode) {
    case 0:
        motion.activeBasePoseAnimIndex = 3;
        break;
    case 1:
        motion.activeBasePoseAnimIndex = 7;
        break;
    case 2:
        motion.activeBasePoseAnimIndex = 0;
        break;
    case 3:
        motion.activeBasePoseAnimIndex = 5;
        break;
    default:
        motion.activeBasePoseAnimIndex = 0;
        motion.motionFlags &= ~0x4000U;
        motion.headLookAngleBase = 0;
        motion.lookAtMode = 0;
        motion.currentTurnAngle = 0;
        break;
    }
    return true;
}

namespace {

namespace {
constexpr std::uint32_t kFightPoseComponent = SHEN_NAME(FIGP);
constexpr std::array<std::array<std::int16_t, 2>, 8> kFightPoseDrawRanges{{
    {{0, 0}},
    {{1, -1}},
    {{1, 14}},
    {{14, -1}},
    {{5, -1}},
    {{10, -1}},
    {{16, -1}},
    {{21, -1}},
}};

MotionNodeRuntime *FindFightPoseRuntimeNode(MotionInformationWork &motion, std::uint8_t nodeId) {
    auto *node = static_cast<MotionNodeRuntime *>(motion.previousNodes);
    for (std::size_t index = 0; node && index < 128 && node[index].nodeId != 0xffU; ++index) {
        if (node[index].nodeId == nodeId)
            return node + index;
    }
    return nullptr;
}

Matrix4x4 InterpolateFightPoseMatrix(const Matrix4x4 &older, const Matrix4x4 &newer, float newerWeight) {
    Matrix4x4 result{};
    const float *const olderValues = &older.rows[0].x;
    const float *const newerValues = &newer.rows[0].x;
    float *const resultValues = &result.rows[0].x;
    for (std::size_t index = 0; index < 16; ++index) {
        resultValues[index] = (newerValues[index] - olderValues[index]) * newerWeight + olderValues[index];
    }
    return result;
}

void DrawFightPoseModelRange(Mt5RuntimeNode *model, std::int16_t rootKind, std::int16_t terminalKind) {
    if (!g_updateRenderFrameMode)
        return;
    Mt5RuntimeNode *const root = shenGetNodeByChildIdx(model, rootKind);
    if (!root)
        return;

    Mt5RuntimeNode *const terminal = terminalKind >= 0 ? shenGetNodeByChildIdx(root, terminalKind) : nullptr;
    const std::int32_t savedTerminalChild = terminal ? terminal->childOffset : 0;
    const std::int32_t savedTerminalSibling = terminal ? terminal->siblingOffset : 0;
    if (terminal) {
        terminal->childOffset = 0;
        terminal->siblingOffset = 0;
    }
    d3t::MT5_SubmitObjectNode(root, 1.0f);
    if (terminal) {
        terminal->childOffset = savedTerminalChild;
        terminal->siblingOffset = savedTerminalSibling;
    }
}

void SubmitFightPoseSample(FightPoseWork &work, std::size_t sample) {
    if (!work.model || !g_pCurrentTaskData || sample >= std::size(work.poseCache))
        return;
    for (std::size_t bone = 0; bone < kFightPoseModelNodeKinds.size(); ++bone) {
        Mt5RuntimeNode *const node = shenGetNodeByChildIdx(work.model, kFightPoseModelNodeKinds[bone]);
        if (!node)
            continue;
        const std::ptrdiff_t offset = reinterpret_cast<std::byte *>(&work.poseCache[sample][bone]) - g_pCurrentTaskData;
        if (offset < INT32_MIN || offset > INT32_MAX)
            continue;
        HMDL_SetTemporaryTaskRelativeOffsets(node, true);
        node->transformOffset = static_cast<std::int32_t>(offset);
    }

    if (work.active == 8) {
        DrawFightPoseModelRange(work.model, 5, -1);
        DrawFightPoseModelRange(work.model, 10, -1);
    } else if (work.active < kFightPoseDrawRanges.size()) {
        const auto &range = kFightPoseDrawRanges[work.active];
        DrawFightPoseModelRange(work.model, range[0], range[1]);
    }
}

void FIGP_Callback(Character *character) {
    FightPoseWork *const work = FIGP_GetWork(character);
    if (!work)
        return;
    work->originalFigurePoseCallback(character);
    if (!work->model)
        return;

    MotionManagerWork *const manager = MOTM_GetWork(character);
    if (!manager)
        return;
    if (!work->active) {
        work->previousActive = 0;
        return;
    }
    const auto *const control = g_mapControlTask ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    if (control && control->ryo == character && !g_vfBattleForceEndRequested) {
        return;
    }

    work->poseCacheColumns = std::min<std::uint8_t>(work->poseCacheColumns, 8);
    work->poseCacheRows = std::min<std::uint8_t>(work->poseCacheRows, 4);
    const std::size_t rows = static_cast<std::size_t>(work->poseCacheRows);
    const std::size_t columns = static_cast<std::size_t>(work->poseCacheColumns);
    const std::size_t stride = rows + 1;
    const std::size_t sampleCount = stride * (columns + 1);

    if (!work->previousActive) {
        for (std::size_t bone = 0; bone < kFightPoseSourceNodeIds.size(); ++bone) {
            MotionNodeRuntime *const node = FindFightPoseRuntimeNode(manager->motion, kFightPoseSourceNodeIds[bone]);
            if (!node || !node->transform)
                continue;
            for (std::size_t sample = 0; sample < sampleCount; ++sample)
                work->poseCache[sample][bone] = *node->transform;
        }
    }
    work->previousActive = work->active;

    if ((Shenmue_nPauseState & 2) == 0) {
        for (std::size_t bone = 0; bone < kFightPoseSourceNodeIds.size(); ++bone) {
            MotionNodeRuntime *const node = FindFightPoseRuntimeNode(manager->motion, kFightPoseSourceNodeIds[bone]);
            if (!node || !node->transform)
                continue;
            for (std::size_t column = columns + 1; column > 1; --column) {
                work->poseCache[column * stride - 1][bone] = work->poseCache[(column - 1) * stride - 1][bone];
            }
            work->poseCache[rows][bone] = *node->transform;
            for (std::size_t column = 0; column <= columns; ++column) {
                const Matrix4x4 &newer = column ? work->poseCache[column * stride - 1][bone] : *node->transform;
                const Matrix4x4 &older = work->poseCache[(column + 1) * stride - 1][bone];
                for (std::size_t row = 0; row < rows; ++row) {
                    const float newerWeight = static_cast<float>(rows - row) / static_cast<float>(rows + 1);
                    work->poseCache[column * stride + row][bone] = InterpolateFightPoseMatrix(older, newer, newerWeight);
                }
            }
        }
    }

    if (!g_updateRenderFrameMode)
        return;
    std::int32_t drawMode = g_drawMode;
    if (control && control->ryo == character)
        drawMode = g_characterDrawMode;
    const VFMAData *const fight = vfMainGetCurrent();
    const bool overrideLighting = !drawMode || (fight && fight->battleType == 6);
    std::int32_t savedLightingMode = drawMode;
    if (overrideLighting) {
        savedLightingMode = g_cachedGlobalLightingAdjustFlag;
        g_cachedGlobalLightingAdjustFlag = 1;
        g_effectsNeedInitialization = 0;
        Shenmue_InitEffectByIndexExact();
    }
    for (std::size_t sample = 1; sample < sampleCount; ++sample)
        SubmitFightPoseSample(*work, sample);
    if (overrideLighting) {
        g_cachedGlobalLightingAdjustFlag = savedLightingMode;
        g_effectsNeedInitialization = savedLightingMode != 1;
        Shenmue_InitEffectByIndexExact();
    }
}

void ReleaseFightPoseBlurModel(FightPoseWork &work) {
    HMDL_FreeAssignedModel(work.model);
    if (work.modelOwner && !PACK_FindModelOwner(work.modelOwner))
        FreePVR(work.modelOwner);
    work.model = nullptr;
    work.modelOwner = nullptr;
    work.filename[0] = '\0';
}

bool LoadFightPoseBlurModel(FightPoseWork &work, const char *filename) {
    if (!filename)
        return false;
    if (std::strcmp(work.filename, filename) == 0)
        return work.model != nullptr;

    if (work.filename[0])
        ReleaseFightPoseBlurModel(work);
    if (*filename == '@' || *filename == '$')
        ++filename;
    if (!*filename)
        return false;
    std::strcpy(work.filename, filename);

    PackTaskData *const pack = g_packTaskData ? g_packTaskData : g_packTask ? reinterpret_cast<PackTaskData *>(g_packTask->task_data_ptr) : nullptr;
    PackModel *model = pack && pack->activeReader ? PACK_FindCharacterModel(pack->activeReader, work.filename) : nullptr;
    const bool modelFromPack = model != nullptr;
    if (!model) {
        char directory[64];
        std::snprintf(directory, sizeof(directory), "scene/%02d/model/chara", g_sceneId);
        model = d3t::ModelLoad_FBX_Replace(directory, work.filename);
    }
    work.modelOwner = model;
    work.model = model ? static_cast<Mt5RuntimeNode *>(hmdlAssignFor(model)) : nullptr;
    if (!work.model) {
        if (!modelFromPack && model)
            FreePVR(model);
        work.modelOwner = nullptr;
    }
    return work.model != nullptr;
}

} // namespace
} // namespace

FightPoseWork *FIGP_GetWork(Character *character) {
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kFightPoseComponent);
    return component ? reinterpret_cast<FightPoseWork *>(component + 1) : nullptr;
}

FightPoseWork *FIGP_Allocate(Character *character, const char *blurModelFilename) {
    if (!character)
        return nullptr;
    if (FightPoseWork *const existing = FIGP_GetWork(character))
        return existing;
    auto *const work = static_cast<FightPoseWork *>(AllocateHeapMemoryMODL(character, kFightPoseComponent, sizeof(FightPoseWork), nullptr));
    if (!work)
        return nullptr;
    work->filename[0] = '\0';
    work->originalFigurePoseCallback = character->figurePoseCallback;
    character->figurePoseCallback = FIGP_Callback;
    if (blurModelFilename) {
        LoadFightPoseBlurModel(*work, blurModelFilename);
        work->poseCacheColumns = 5;
        work->poseCacheRows = 2;
    }
    return work;
}

void FIGP_LoadBlurModel(Character *character, const char *filename) {
    if (FightPoseWork *const work = FIGP_GetWork(character))
        LoadFightPoseBlurModel(*work, filename);
}
void FIGP_Related(Character *character, MotionInformationWork *motion) {
    FightPoseWork *const work = FIGP_GetWork(character);
    if (!work || !motion)
        return;
    const MotnFigpControlEvent *const event = MOTN_FindFigpControlEvent(motion->currentAnimId, motion->frameTimer, motion->frameTimer + motion->playbackRate);
    if (!event)
        return;

    std::uint8_t mode = event->mode;
    if ((motion->motionFlags & 0x80U) != 0) {
        switch (mode) {
        case 4:
            mode = 5;
            break;
        case 5:
            mode = 4;
            break;
        case 6:
            mode = 7;
            break;
        case 7:
            mode = 6;
            break;
        default:
            break;
        }
    }
    if (mode != 0) {
        work->poseCacheColumns = event->cacheColumns;
        work->poseCacheRows = event->cacheRows;
    }
    work->active = mode;
}

namespace {

Mt5RuntimeNode *FindModelNodeByKind(Mt5RuntimeNode *node, std::int16_t nodeKind) {
    while (node) {
        if (static_cast<std::int16_t>(node->nodeKind) == nodeKind)
            return node;
        if (node->childOffset) {
            if (Mt5RuntimeNode *const found = FindModelNodeByKind(reinterpret_cast<Mt5RuntimeNode *>(g_pCurrentTaskData + node->childOffset), nodeKind)) {
                return found;
            }
        }
        node = node->siblingOffset ? reinterpret_cast<Mt5RuntimeNode *>(g_pCurrentTaskData + node->siblingOffset) : nullptr;
    }
    return nullptr;
}
} // namespace

Vector3 TransformPoint(const Matrix4x4 &transform, const Vector3 &point) {
    return {
        point.x * transform.rows[0].x + point.y * transform.rows[1].x + point.z * transform.rows[2].x + transform.rows[3].x,
        point.x * transform.rows[0].y + point.y * transform.rows[1].y + point.z * transform.rows[2].y + transform.rows[3].y,
        point.x * transform.rows[0].z + point.y * transform.rows[1].z + point.z * transform.rows[2].z + transform.rows[3].z,
    };
}
namespace {

void ComponentNoOp(Character *, void *) {
}
} // namespace

namespace {

constexpr std::uint32_t kGameDisplayComponent = SHEN_NAME(GAME);
constexpr std::size_t kGameDisplayCapacity = 64;

struct GameDisplayEntry {
    Vector3 position;
    std::uint32_t reserved0c;
    Mt5RuntimeNode *node;
    std::uint32_t nodeIdentity;
    float renderAlpha;
    Vector3 basis;
    std::uint32_t reserved2c;
};
static_assert(sizeof(GameDisplayEntry) == 0x30);

struct GameDisplayWork {
    Mt5RuntimeNode *model;
    Mt5RuntimeNode *detachedPrevious;
    std::int32_t stagedCount;
    std::int32_t activeCount;
    std::uint32_t baseNodeId;
    Vector3 digitStep;
    std::uint32_t maxValue;
    std::int32_t decimalShift;
    std::uint32_t reserved30;
    std::uint32_t cachePublished;
    std::array<GameDisplayEntry, kGameDisplayCapacity> staged;
    std::array<GameDisplayEntry, kGameDisplayCapacity> active;
    std::array<std::uint32_t, kGameDisplayCapacity> cachedNodeIds;
};
static_assert(offsetof(GameDisplayWork, staged) == 0x38);
static_assert(offsetof(GameDisplayWork, active) == 0xc38);
static_assert(offsetof(GameDisplayWork, cachedNodeIds) == 0x1838);
static_assert(sizeof(GameDisplayWork) == 0x1938);

struct FootstepWork {
    std::uint8_t surface;
    std::uint8_t enabled;
    std::uint8_t authoredMode;
};
static_assert(sizeof(FootstepWork) == 3);

struct ClockWork {
    std::int16_t minuteOffset;
};
static_assert(sizeof(ClockWork) == 2);

struct CalendarWork {
    Vector3 position;
    GameDateTime time;
    std::byte reserved14[8];
};
static_assert(sizeof(CalendarWork) == 28);

void CLOK_Callback(Character *character, void *payload) {
    const auto *const clock = static_cast<const ClockWork *>(payload);
    if (!character || !clock)
        return;
    auto *const root = static_cast<Mt5RuntimeNode *>(character->model);
    Mt5RuntimeNode *const minuteHand = shenGetNodeByChildIdx(root, 151);
    Mt5RuntimeNode *const hourHand = shenGetNodeByChildIdx(root, 150);
    if (!minuteHand || !hourHand)
        return;
    const std::int32_t sign = clock->minuteOffset < 0 ? -1 : 1;
    std::int16_t minutes = static_cast<std::int16_t>(clock->minuteOffset * sign);
    std::int16_t hours = 0;
    if (minutes >= 60) {
        hours = minutes / 60;
        minutes %= 60;
    }
    hours = static_cast<std::int16_t>(g_currentDateTime.hours + (hours % 24) * sign);
    minutes = static_cast<std::int16_t>(g_currentDateTime.minutes + minutes * sign);
    if (minutes < 0) {
        const std::int16_t borrow = (-1 - minutes) / 60 + 1;
        hours -= borrow;
        minutes += 60 * borrow;
    }
    if (hours < 0)
        hours += 24 * ((-1 - hours) / 24 + 1);
    if (minutes >= 60) {
        hours += minutes / 60;
        minutes %= 60;
    }
    hours %= 24;
    hourHand->rotation.z = static_cast<std::uint16_t>(-91 * minutes - 5461 * hours - g_currentDateTime.seconds);
    minuteHand->rotation.z = static_cast<std::uint16_t>(-18 * g_currentDateTime.seconds - 1092 * minutes);
}

bool IsCalendarFixedHoliday(const GameDateTime &date, std::uint8_t additionalHolidays) {
    constexpr std::uint8_t months[]{1, 1, 2, 4, 5, 5, 9, 10, 11, 11};
    constexpr std::uint8_t days[]{1, 15, 11, 29, 3, 5, 15, 10, 3, 23};
    constexpr std::uint8_t additionalMonths[]{5, 12, 7};
    constexpr std::uint8_t additionalDays[]{4, 23, 20};
    for (std::size_t index = 0; index < std::size(months); ++index)
        if (date.month == months[index] && date.day == days[index])
            return true;
    for (std::uint8_t index = 0; index < additionalHolidays; ++index)
        if (date.month == additionalMonths[index] && date.day == additionalDays[index])
            return true;
    return false;
}

bool IsCalendarEquinox(const GameDateTime &date) {
    if (date.month != 3 && date.month != 9)
        return false;
    const std::int32_t years = date.year - 80;
    const double baseline = date.month == 3 ? 20.8431 : 23.2488;
    return date.day == static_cast<std::int32_t>(years * 0.242194 + baseline - years / 4);
}

bool IsSpecialCalendarDate(const GameDateTime &date) {
    const std::uint8_t additionalHolidays = date.year >= 96 ? 3 : date.year >= 89 ? 2 : date.year >= 85 ? 1 : 0;
    if (!date.dayOfWeek || IsCalendarFixedHoliday(date, additionalHolidays) || IsCalendarEquinox(date))
        return true;
    GameDateTime previous = date;
    if (--previous.day == 0) {
        if (--previous.month == 0) {
            previous.month = 12;
            previous.year = previous.year ? previous.year - 1 : 99;
        }
        constexpr std::uint8_t daysInMonth[]{31, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        previous.day = previous.month == 2 && !(previous.year & 3U) ? 29 : daysInMonth[previous.month];
    }
    RecalculateDayInTheWeek(&previous);
    return !previous.dayOfWeek && (IsCalendarFixedHoliday(previous, additionalHolidays) || IsCalendarEquinox(previous));
}

std::size_t CalendarPhaseIndex(const GameDateTime &date) {
    if (date.year < 86)
        return 0;
    const CalendarPhaseMonth &entry = kCalendarPhaseMonths[date.month - 1 + 12 * (date.year - 86)];
    std::int32_t month = entry.nextMonth;
    std::int32_t day = date.day - entry.boundaryDay + 1;
    if (date.day < entry.boundaryDay) {
        month = entry.previousMonth;
        if (entry.dayAtMonthStart) {
            day = entry.dayAtMonthStart + date.day - 1;
        } else if (date.day == 1) {
            --month;
            day = 30;
        } else {
            day = date.day - 1;
        }
    }
    if (month > 12)
        month -= 12;
    return static_cast<std::size_t>((day + month) % 6);
}

void ApplyCalendarDate(Character *character, CalendarWork &calendar) {
    GAME_CommitStagedEntries(character);
    calendar.time = g_currentDateTime;
    const std::uint32_t digitBase = IsSpecialCalendarDate(calendar.time) ? 292 : 256;
    calendar.position = {0.0f, 0.0143f, 0.0176f};
    const std::uint8_t day = calendar.time.day;
    if (day > 9) {
        calendar.position.x = -0.018200001f;
        GAME_StageNodeData(character, digitBase + day / 10, &calendar.position, 1.0f, nullptr);
        calendar.position.x = 0.0138f;
    }
    GAME_StageNodeData(character, digitBase + day % 10, &calendar.position, 1.0f, nullptr);
    calendar.position.x = 0.044300001f;
    const std::uint8_t month = calendar.time.month;
    if (month == 12 || (month >= 1 && month <= 4)) {
        calendar.position.y = month == 12 ? -0.0013f : 0.003f;
        GAME_StageNodeData(character, month == 12 ? 364 : 364 + month, &calendar.position, 1.0f, nullptr);
    }
    calendar.position.x = 0.044100001f;
    calendar.position.y = 0.046799999f;
    GAME_StageNodeData(character, calendar.time.year == 86 ? 266 : 267, &calendar.position, 1.0f, nullptr);
    constexpr std::uint32_t phaseNodes[]{329, 332, 333, 330, 328, 331};
    calendar.position.x = -0.045200001f;
    calendar.position.y = 0.0385f;
    GAME_StageNodeData(character, phaseNodes[CalendarPhaseIndex(calendar.time)], &calendar.position, 1.0f, nullptr);
}

void CALE_Callback(Character *character, void *payload) {
    auto *const calendar = static_cast<CalendarWork *>(payload);
    if (calendar &&
        (calendar->time.dayOfWeek != g_currentDateTime.dayOfWeek || calendar->time.day != g_currentDateTime.day ||
            calendar->time.month != g_currentDateTime.month))
        ApplyCalendarDate(character, *calendar);
}

Mt5RuntimeNode *FindGameDisplayNode(Mt5RuntimeNode *node, std::int16_t nodeKind) {
    while (node) {
        if (static_cast<std::int16_t>(node->nodeKind) == nodeKind)
            return node;
        std::byte *const base = MT5_ModelBaseForNode(node);
        if (node->childOffset) {
            if (Mt5RuntimeNode *const found = FindGameDisplayNode(reinterpret_cast<Mt5RuntimeNode *>(base + node->childOffset), nodeKind)) {
                return found;
            }
        }
        node = node->siblingOffset ? reinterpret_cast<Mt5RuntimeNode *>(base + node->siblingOffset) : nullptr;
    }
    return nullptr;
}

void ApplyGameDisplayNodeTransform(const Mt5RuntimeNode &node) {
    Matrix4x4::preTranslate(node.position.x, node.position.y, node.position.z);
    if (node.rotation.z)
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node.rotation.z));
    if (node.rotation.y)
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node.rotation.y));
    if (node.rotation.x)
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node.rotation.x));
    if (node.scale.x != 1.0f || node.scale.y != 1.0f || node.scale.z != 1.0f) {
        Matrix4x4::Scale(node.scale.x, node.scale.y, node.scale.z);
    }
}

void VisitRelativeHandNode(Mt5RuntimeNode *node, std::byte *modelBase, std::int16_t nodeKind, Matrix4x4 &result) {
    const Matrix4x4 parent = workTransformMatrix;
    Matrix4x4::SetActiveTransformToPrePass();
    ApplyGameDisplayNodeTransform(*node);
    PushCharacterMatrix();
    workTransformMatrix = parent;
    Matrix4x4::Multiply(g_matrixStack.top.stack - 1);
    const Matrix4x4 current = workTransformMatrix;
    Matrix4x4::Pop(2);
    if (static_cast<std::int16_t>(node->nodeKind) == nodeKind) {
        result = current;
        return;
    }
    PushCharacterMatrix();
    workTransformMatrix = current;
    for (auto *child = node->childOffset ? reinterpret_cast<Mt5RuntimeNode *>(modelBase + node->childOffset) : nullptr; child;
         child = child->siblingOffset ? reinterpret_cast<Mt5RuntimeNode *>(modelBase + child->siblingOffset) : nullptr) {
        VisitRelativeHandNode(child, modelBase, nodeKind, result);
    }
    Matrix4x4::Pop();
}
} // namespace



float MeasureRelativeHandNodeDistance(Character *character, std::int16_t rootKind, std::int16_t nodeKind) {
    auto *const model = character ? static_cast<Mt5RuntimeNode *>(character->model) : nullptr;
    if (!shenGetNodeByChildIdx(model, nodeKind) || !shenGetNodeByChildIdx(model, rootKind))
        return -1.0f;
    Mt5RuntimeNode *const root = shenGetNodeByChildIdx(model, rootKind);
    std::byte *const modelBase = MT5_ModelBaseForNode(root);
    if (!modelBase)
        return -1.0f;
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4 relative = workTransformMatrix;
    for (auto *child = root->childOffset ? reinterpret_cast<Mt5RuntimeNode *>(modelBase + root->childOffset) : nullptr; child;
         child = child->siblingOffset ? reinterpret_cast<Mt5RuntimeNode *>(modelBase + child->siblingOffset) : nullptr) {
        VisitRelativeHandNode(child, modelBase, nodeKind, relative);
    }
    Matrix4x4::Pop();
    const auto &position = relative.rows[3];
    return std::sqrt(position.x * position.x + position.y * position.y + position.z * position.z);
}
namespace {} // namespace



float MeasureHandNodeChainLength(Character *character, std::int16_t rootKind, std::int16_t nodeKind, float fallback) {
    auto *const model = character ? static_cast<Mt5RuntimeNode *>(character->model) : nullptr;
    Mt5RuntimeNode *const root = shenGetNodeByChildIdx(model, rootKind);
    Mt5RuntimeNode *node = shenGetNodeByChildIdx(root, nodeKind);
    if (!root || !node)
        return fallback;
    std::byte *const modelBase = MT5_ModelBaseForNode(root);
    if (!modelBase)
        return fallback;
    float length = 0.0f;
    while (node != root) {
        const Vector3 &position = node->position;
        length += std::sqrt(position.x * position.x + position.y * position.y + position.z * position.z);
        if (!node->parentOffset)
            break;
        node = reinterpret_cast<Mt5RuntimeNode *>(modelBase + node->parentOffset);
    }
    return length;
}
namespace {

GameDisplayWork *FindGameDisplayWork(Character *character) {
    CharacterComponentNode *const component = character ? HLib_GetCharacterComponent(character, kGameDisplayComponent) : nullptr;
    return component ? reinterpret_cast<GameDisplayWork *>(component + 1) : nullptr;
}




void GAME_SubmitForSkinningBuffer(void *model, GameDisplayWork *display) {
    if (!display || !display->stagedCount || display->cachePublished)
        return;
    RenderArrayEntry *const entry = reinterpret_cast<RenderArrayEntry *>(ResolveHmdlEntry(reinterpret_cast<std::uintptr_t>(model)));
    if (!entry || entry->state != 1)
        return;
    std::uint32_t rigCache = 0;
    std::memcpy(&rigCache, entry->rigState + 0x110, sizeof(rigCache));
    std::uint64_t root = AssignNode(entry->handle, "TinyGameRootNode");
    const std::uint32_t modelHandle = GetSkinningNodeRenderHandle(entry->handle);
    if (!static_cast<std::uint32_t>(root) || !(root >> 32U) || !modelHandle)
        root = AllocateSkinningNode(entry->handle, "TinyGameRootNode", 0, false);
    if (!static_cast<std::uint32_t>(root) || !(root >> 32U))
        return;
    display->cachePublished = 1;
    for (std::uint32_t index = 0; index < static_cast<std::uint32_t>(display->stagedCount); ++index) {
        const GameDisplayEntry &staged = display->staged[index];
        if (index < static_cast<std::uint32_t>(display->activeCount) && staged.nodeIdentity != display->active[index].nodeIdentity &&
            display->cachedNodeIds[index]) {
            RemoveSkinningNode(rigCache | (static_cast<std::uint64_t>(display->cachedNodeIds[index]) << 32U));
            display->cachedNodeIds[index] = 0;
        }
        const Matrix4x4 placement{{
            {staged.basis.x, 0.0f, 0.0f, staged.position.x},
            {0.0f, staged.basis.y, 0.0f, staged.position.y},
            {0.0f, 0.0f, staged.basis.z, staged.position.z},
            {0.0f, 0.0f, 0.0f, 1.0f},
        }};
        if (display->cachedNodeIds[index]) {
            AppendHashForSkinningBuffer(rigCache | (static_cast<std::uint64_t>(display->cachedNodeIds[index]) << 32U), placement);
            continue;
        }
        std::uint32_t recordCount = 0;
        std::uint32_t rigInitialized = 0;
        std::uint32_t rigReady = 0;
        std::memcpy(&rigInitialized, entry->rigState + 0x108, sizeof(rigInitialized));
        std::memcpy(&rigReady, entry->rigState + 0x10c, sizeof(rigReady));
        std::memcpy(&recordCount, entry->rigState + 0x11c, sizeof(recordCount));
        std::uint32_t recordHash = 0;
        bool found = false;
        for (std::uint32_t record = 0; rigInitialized && rigReady && record < recordCount && !found; ++record) {
            const std::byte *const bytes = entry->rigState + 0x120 + static_cast<std::size_t>(record) * 0x20U;
            std::uint64_t node = 0;
            std::int16_t kind = 0;
            std::memcpy(&node, bytes + 0x08, sizeof(node));
            std::memcpy(&kind, bytes + 0x10, sizeof(kind));
            if (node && kind == static_cast<std::int32_t>(staged.nodeIdentity)) {
                std::memcpy(&recordHash, bytes + 0x14, sizeof(recordHash));
                found = true;
            }
        }
        if (!found || !rigCache || !recordHash)
            continue;
        const char *const name = GetSkinningNodeName(rigCache | (static_cast<std::uint64_t>(recordHash) << 32U));
        const std::uint64_t modelNode = name ? FindModelNodeByName(modelHandle, name) : 0U;
        if (!static_cast<std::uint32_t>(modelNode) || !(modelNode >> 32U))
            continue;
        const std::uint64_t node = AllocateSkinningNodeFromRenderHandle(static_cast<std::uint32_t>(entry->handle), modelNode, root);
        if (!static_cast<std::uint32_t>(node) || !(node >> 32U))
            continue;
        AppendHashForSkinningBuffer(node, placement);
        display->cachedNodeIds[index] = static_cast<std::uint32_t>(node >> 32U);
    }
}




void GAME_Main(Character *character) {
    if (!g_updateRenderFrameMode || !g_bShouldDraw || !character)
        return;
    GameDisplayWork *const display = FindGameDisplayWork(character);
    if (!display)
        return;
    const Mt5RuntimeNode &model = *static_cast<const Mt5RuntimeNode *>(character->model);
    Matrix4x4::SetActiveTransformToPrePass();
    TransformCharacterHierarchyZYX(&character->entity.node);
    ApplyGameDisplayNodeTransform(model);
    const bool cached = CachedAttachmentOwnsSubmission(character->model, model.scale.z, workTransformMatrix);
    PopCharacterMatrix();
    if (cached)
        return;
    PushCharacterMatrix();
    TransformCharacterHierarchyZYX(&character->entity.node);
    ApplyGameDisplayNodeTransform(model);
    PushCharacterMatrix();
    for (std::int32_t index = 0; index < display->stagedCount; ++index) {
        const GameDisplayEntry &entry = display->staged[index];
        workTransformMatrix = g_matrixStack.top.stack[-1];
        Matrix4x4::preTranslate(entry.position.x, entry.position.y, entry.position.z);
        Matrix4x4::Scale(entry.basis.x, entry.basis.y, entry.basis.z);
        if (!entry.node)
            continue;
        ApplyGameDisplayNodeTransform(*entry.node);
        std::byte *modelBase = nullptr;
        void *const binding = MT5_SceneBindingForNode(entry.node, &modelBase);
        if (!binding)
            continue;
        if (entry.renderAlpha == 1.0f)
            SubmitSceneObjectForDrawing(binding, modelBase);
        else if (entry.renderAlpha != 0.0f)
            SubmitToEffectBuffer(binding, entry.renderAlpha, 0, modelBase);
    }
    PopCharacterMatrix();
    PopCharacterMatrix();
}



void GAME_Callback(Character *character, void *payload) {
    GameDisplayWork *const display = static_cast<GameDisplayWork *>(payload);
    if (!display->stagedCount)
        return;
    static_cast<void>(SubmitRiggedSceneObjectForDrawing(character->model, character, 0, 4));
    GAME_SubmitForSkinningBuffer(character->model, display);
    GAME_Main(character);
}

GameDisplayWork *EnsureGameDisplayWork(Character *character) {
    if (!character)
        return nullptr;
    if (GameDisplayWork *const display = FindGameDisplayWork(character))
        return display;
    auto *const display = static_cast<GameDisplayWork *>(AllocateHeapMemoryMODL(character, kGameDisplayComponent, sizeof(GameDisplayWork), GAME_Callback));
    if (!display)
        return nullptr;
    display->baseNodeId = 256;
    display->digitStep.x = -0.02f;
    display->maxValue = 99999999;
    auto *const root = static_cast<Mt5RuntimeNode *>(character->model);
    if (!root || !root->childOffset)
        return display;
    std::byte *const base = MT5_ModelBaseForNode(root);
    auto *node = reinterpret_cast<Mt5RuntimeNode *>(base + root->childOffset);
    Mt5RuntimeNode *previous = nullptr;
    while (node->nodeKind != 255) {
        if (!node->siblingOffset)
            return display;
        previous = node;
        node = reinterpret_cast<Mt5RuntimeNode *>(base + node->siblingOffset);
    }
    display->model = node;
    display->detachedPrevious = previous;
    if (previous)
        previous->siblingOffset = node->siblingOffset;
    else
        root->childOffset = node->siblingOffset;
    node->siblingOffset = 0;
    return display;
}

bool StageGameDisplayNode(GameDisplayWork &display, std::uint32_t nodeId, const Vector3 *position, float renderAlpha, const Vector3 *basis) {
    if (!position || display.stagedCount < 0 || display.stagedCount >= static_cast<std::int32_t>(kGameDisplayCapacity)) {
        return false;
    }
    Mt5RuntimeNode *const node = FindGameDisplayNode(display.model, static_cast<std::int16_t>(nodeId));
    std::byte *modelBase = nullptr;
    if (!node || !MT5_SceneBindingForNode(node, &modelBase))
        return false;

    GameDisplayEntry &entry = display.staged[display.stagedCount++];
    entry.position = *position;
    entry.node = node;
    entry.nodeIdentity = static_cast<std::uint32_t>(node->nodeKind) | (static_cast<std::uint32_t>(node->nodeFlags) << 16U);
    entry.renderAlpha = renderAlpha;
    entry.basis = basis ? *basis : Vector3{1.0f, 1.0f, 1.0f};
    return true;
}

void MapMotionMatricesToModel(MotionInformationWork &motion, void *model) {
    auto *const nodes = static_cast<MotionNodeRuntime *>(motion.activeNodes);
    auto *const root = static_cast<Mt5RuntimeNode *>(model);
    if (!nodes || !root)
        return;



    HMDL_ClearNodeTransformLinks(root);
    for (const MotionToModelBone &mapping : kMotionToModelBones) {
        MotionNodeRuntime *node = nodes;
        while (node->nodeId != 0xff && node->nodeId != mapping.motionNodeId)
            ++node;
        if (node->nodeId == 0xff || (node->bindPositionPacked[1] & 2U) == 0 || !node->transform) {
            continue;
        }
        Mt5RuntimeNode *const modelNode = FindModelNodeByKind(root, static_cast<std::int16_t>(mapping.modelNodeKind));
        if (!modelNode)
            continue;
        const std::ptrdiff_t offset = reinterpret_cast<std::byte *>(node->transform) - g_pCurrentTaskData;
        if (offset >= INT32_MIN && offset <= INT32_MAX)
            modelNode->transformOffset = static_cast<std::int32_t>(offset);
    }
}

void RefreshExternalMotionTransform(MotionManagerWork &manager) {
    Character *const source = manager.transformSourceCharacter;
    if (!source)
        return;
    SetupTransformationMatrix(source, manager.transformModelEntryCount, &manager.transformMatrixCache);
    Matrix4x4_Push(&manager.transformMatrixCache);
    Matrix4x4::preTranslate(manager.transformTranslationOffset.x, manager.transformTranslationOffset.y, manager.transformTranslationOffset.z);
    Matrix4x4::SetViewMatrixFromEuler(manager.transformEulerOffset.y, manager.transformEulerOffset.x, manager.transformEulerOffset.z);
    manager.transformMatrixCache = workTransformMatrix;
    Matrix4x4::Pop(1);
}
} // namespace

MotionNodeRuntime *FindMotionNode(MotionNodeRuntime *nodes, std::int32_t nodeId) {
    if (!nodes)
        return nullptr;
    while (nodes->nodeId != 0xff) {
        if (nodes->nodeId == nodeId)
            return nodes;
        ++nodes;
    }
    return nullptr;
}
namespace {

void MOTM_Callback(Character *character, void *payload) {
    auto *const manager = static_cast<MotionManagerWork *>(payload);
    if (!character || !manager)
        return;
    g_motnCubicScratch = nullptr;
    if (const auto *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr)
        SCEN_PromoteCharacterIndexNode(control->scenarioTask, character);
    const Vector3 previousRootWorld = manager->motion.rootWorldPosition;
    const Vector3 previousWorldPosition = GetPositionForEntity(character->entity.node);
    ManageMotionAnimations(character);
    manager->motion.rootPreTranslate = GetPositionForEntity(character->entity.node);
    manager->motion.entityEuler = GetEulerAnglesForEntity(character->entity.node);
    RefreshExternalMotionTransform(*manager);
    if (character->model) {
        manager->motion.basePoseTemplateIndex = getIMGM(character);

        manager->motion.frameContactOverlay = reinterpret_cast<std::uintptr_t>(HMDL_CycleDataForNode(character->model));
        g_motnClipAngle = character->clipAngle;
        g_pMotion = &manager->motion;
        g_motnBasePoseOnly = 0;
        const std::uint32_t imageName = IMGM_GetImageName(character);
        g_motnIsRyo = imageName == IMAGE_YKA_M.name || imageName == IMAGE_YKB_M.name || imageName == IMAGE_YKC_M.name || imageName == IMAGE_YKD_M.name ||
                imageName == IMAGE_YKE_M.name || imageName == IMAGE_YKF_M.name ?
            1U :
            0U;
        NPCLogger(character, 5, mainMotionUpdate(manager->motion));
    } else {
        const bool wasVisible = (character->motionFlags & 0x10U) != 0;
        character->motionFlags &= static_cast<std::uint8_t>(~0x10U);
        if (g_bDebugLogVisibility && (character->flags.value & CharacterFlagHasCpct) != 0 && wasVisible)
            CHAR_LogNPCVisibility(character, false);
    }
    FIGP_Related(character, &manager->motion);
    if (const std::uint8_t *const cue = shenLookupAnimationIndex(manager->motion.currentAnimId, 20)) {
        const std::int32_t value0 = static_cast<std::int8_t>(cue[1]);
        const std::int32_t value1 = static_cast<std::int8_t>(cue[2]);
        std::byte *const slot = manager->cue20Values + static_cast<std::size_t>(cue[3]) * 8;
        std::memcpy(slot, &value0, sizeof(value0));
        std::memcpy(slot + 4, &value1, sizeof(value1));
    } else {
        std::memset(manager->cue20Values, 0, 8);
    }
    std::fill(std::begin(manager->motion.localTargets), std::end(manager->motion.localTargets), nullptr);
    std::fill(std::begin(manager->motion.worldTargets), std::end(manager->motion.worldTargets), nullptr);
    if ((manager->flags & 1U) != 0) {
        Vector3 delta = manager->motion.rootMotionDelta;
        if (manager->motion.interpolationMode == 7 || manager->motion.interpolationMode == 11) {
            delta.x = manager->motion.rootWorldPosition.x - previousRootWorld.x;
            delta.z = manager->motion.rootWorldPosition.z - previousRootWorld.z;
            shenApplyTranslationToAllInternal(character->entity.node, previousWorldPosition);
        }
        if (character->entity.node.child)
            RotatePointByCurrentTransform(character->entity.node.child, delta);
        character->rootMotionDelta.x += delta.x;
        character->rootMotionDelta.y += delta.y;
        character->rootMotionDelta.z += delta.z;
    }

    if (manager->motion.soundCommand)
        SNDWrapper(static_cast<std::uint32_t>(manager->motion.soundCommand), g_motnSoundBank, character);

    std::uint32_t handEventOccurrence = 1;
    while (const MotnHandCommandEvent *const event = MOTN_FindHandCommandEvent(
               manager->motion.currentAnimId, manager->motion.frameTimer - manager->motion.playbackRate, manager->motion.frameTimer, handEventOccurrence)) {
        ++handEventOccurrence;
        const std::int32_t encodedSide = static_cast<std::int8_t>(event->sideOrVariant);
        const std::int32_t hand = (manager->motion.motionFlags & 0x80U) != 0 ? encodedSide : 1 - encodedSide;
        if (event->commandIndex < std::size(kMotionHandPoses) && hand >= 0 && hand < 2) {
            const auto packed = static_cast<std::uint16_t>(event->frameCount);
            const std::int32_t frameCount = static_cast<std::int8_t>(packed >> 8) * 256 + static_cast<std::int8_t>(packed & 0xffU);
            mhndApplyCharaHandTarget(character, hand, kMotionHandPoses[event->commandIndex][hand], frameCount);
        }
    }
    if (character->model)
        MapMotionMatricesToModel(manager->motion, character->model);

    const Vector3 &root = manager->motion.rootWorldPosition;
    character->viewPosition.x = root.x * workTransformMatrix.rows[0].x + root.y * workTransformMatrix.rows[1].x + root.z * workTransformMatrix.rows[2].x +
        workTransformMatrix.rows[3].x;
    character->viewPosition.y = root.x * workTransformMatrix.rows[0].y + root.y * workTransformMatrix.rows[1].y + root.z * workTransformMatrix.rows[2].y +
        workTransformMatrix.rows[3].y;
    character->viewPosition.z = root.x * workTransformMatrix.rows[0].z + root.y * workTransformMatrix.rows[1].z + root.z * workTransformMatrix.rows[2].z +
        workTransformMatrix.rows[3].z;
    const float distance = std::sqrt(character->viewPosition.x * character->viewPosition.x + character->viewPosition.y * character->viewPosition.y +
        character->viewPosition.z * character->viewPosition.z);
    character->viewDistance = static_cast<double>(distance) >= 0.00001 ? distance : 0.0f;
}



void MOTM_TransformMatrix(void *model, float alpha) {
    if (!model || !g_drawingImageModel)
        return;
    PushCharacterMatrix();
    const Matrix4x4 cameraTransform = workTransformMatrix;
    PushCharacterMatrix();
    workTransformMatrix = transformMatrix_prePass;
    if (g_drawingCharacter)
        TransformCharacterHierarchyZYX(&g_drawingCharacter->entity.node);
    const bool cached = CachedAttachmentOwnsSubmission(model, alpha, workTransformMatrix);
    PopCharacterMatrix();
    if (!cached)
        Recursive_SubmitRiggedNodes(model, alpha, cameraTransform, g_drawingCharacter);
    static_cast<void>(SubmitRiggedSceneObjectForDrawing(model, g_drawingCharacter, 1, 0));
    PopCharacterMatrix();
}

} // namespace



void GAME_BindModelOffsets(void *model, Character *character) {
    GameDisplayWork *const display = model ? FindGameDisplayWork(character) : nullptr;
    if (!display || !display->model)
        return;
    std::byte *const base = MT5_ModelBaseForNode(display->model);
    const std::int32_t offset = static_cast<std::int32_t>(reinterpret_cast<std::byte *>(display->model) - base);
    if (display->detachedPrevious) {
        display->model->siblingOffset = display->detachedPrevious->siblingOffset;
        display->detachedPrevious->siblingOffset = offset;
        return;
    }
    Mt5RuntimeNode *const parent = reinterpret_cast<Mt5RuntimeNode *>(base + display->model->parentOffset);
    display->model->siblingOffset = parent->childOffset;
    parent->childOffset = offset;
}


void GAME_RestoreModelOffsets(void *model, Character *character) {
    GameDisplayWork *const display = model ? FindGameDisplayWork(character) : nullptr;
    if (!display || !display->model)
        return;
    std::byte *const base = MT5_ModelBaseForNode(display->model);
    if (display->detachedPrevious) {
        display->detachedPrevious->siblingOffset = display->model->siblingOffset;
    } else {
        Mt5RuntimeNode *const parent = reinterpret_cast<Mt5RuntimeNode *>(base + display->model->parentOffset);
        parent->childOffset = display->model->siblingOffset;
    }
    display->model->siblingOffset = 0;
}


void MOTM_DrawPlayerHuman(void *model, float alpha) {
    if (!model)
        return;

    PushCharacterMatrix();
    const Matrix4x4 cameraTransform = workTransformMatrix;
    const auto *const root = static_cast<const Mt5RuntimeNode *>(model);
    if (root->transformOffset) {
        if (const Matrix4x4 *const transform = HMDL_TransformForNode(root, root->transformOffset))
            workTransformMatrix = *transform;
    }
    if (g_drawingImageModel && !CachedAttachmentOwnsSubmission(model, alpha, workTransformMatrix))
        Recursive_SubmitRiggedNodes(model, alpha, cameraTransform, g_drawingCharacter);
    if (g_drawingCharacter && g_drawingImageModel) {
        PushCharacterMatrix();
        workTransformMatrix = transformMatrix_prePass;
        static_cast<void>(SubmitRiggedSceneObjectForDrawing(model, g_drawingCharacter, 1, 0));
        PopCharacterMatrix();
    }
    PopCharacterMatrix();
}

namespace {





void MapMirroredMotionMatricesToModel(MotionInformationWork &motion, void *model) {
    Matrix4x4_Push();
    auto *const nodes = static_cast<MotionNodeRuntime *>(motion.activeNodes);
    auto *const root = static_cast<Mt5RuntimeNode *>(model);
    for (std::size_t index = 0; index < kMotionToModelBones.size(); ++index) {
        const MotionToModelBone &mapping = kMotionToModelBones[index];
        if (!nodes)
            continue;
        MotionNodeRuntime *node = nodes;
        while (node->nodeId != 0xff && node->nodeId != mapping.motionNodeId)
            ++node;
        if (node->nodeId == 0xff || (node->bindPositionPacked[1] & 2U) == 0 || !root)
            continue;
        Mt5RuntimeNode *const modelNode = FindModelNodeByKind(root, static_cast<std::int16_t>(mapping.modelNodeKind));
        if (!modelNode)
            continue;
        workTransformMatrix = g_objectDebugMatrix;
        Matrix4x4::Multiply(node->transform);
        Matrix4x4 *const mirrored = static_cast<Matrix4x4 *>(g_objectDebugAllocation) + index;
        *(g_objectDebugAllocation ? mirrored : g_matrixStack.top.stack) = workTransformMatrix;
        modelNode->transformOffset = static_cast<std::int32_t>(reinterpret_cast<std::byte *>(mirrored) - g_pCurrentTaskData);
    }
    Matrix4x4::Pop(1);
}




void MOTM_Update(Character *character) {
    void *const model = character->model;
    if (!model)
        return;
    if (!g_suppressClothPose) {
        UpdateCharacterClothPose(*character);
        OSAGE_Update(character);
    }
    if (!g_updateRenderFrameMode)
        return;
    if (!g_drawingImageModel)
        g_drawingImageModel = IMGM_GetCurrentImageModel(character);
    g_drawingCharacter = character;
    const bool mirrored = static_cast<std::uint32_t>(g_perspectiveProjection - 1) > 2U && (character->flags.value & CharacterFlagReflectInScriptPlane) != 0U &&
        g_dbg_draw_enabled != 0;
    if (character->drawCallback && mirrored) {
        MotionManagerWork *const manager = FindMotionManagerWork(character);
        if (Shenmue_nPauseState != 0 && manager) {
            HMDL_ClearNodeTransformLinks(static_cast<Mt5RuntimeNode *>(model));
            MapMotionMatricesToModel(manager->motion, model);
        }
        character->drawCallback(model, character->renderOpacity);
        if (manager) {
            HMDL_ClearNodeTransformLinks(static_cast<Mt5RuntimeNode *>(model));
            MapMirroredMotionMatricesToModel(manager->motion, model);
        }
        g_is_busy_transform = 1;
        character->drawCallback(model, 1.0f);
        g_is_busy_transform = 0;
    } else if (character->drawCallback) {
        character->drawCallback(model, character->renderOpacity);
    }
    g_drawingImageModel = nullptr;
    g_drawingCharacter = nullptr;
}


float ResolveCharacterShadowGround(const Character &character) {
    if ((character.flags.value & CharacterFlagSkipShadowGroundCheck) != 0U)
        return character.shadowHeight;
    Matrix4x4_TranslateAndRotate(*reinterpret_cast<const CharacterIdTransform *>(&character.entity.node));
    return mapUpdateIntersectionStateAtLocalPoint(character.entity.node.position);
}

void MOTM_UpdateCharaShadow(Character *character) {
    if (!character || !g_updateRenderFrameMode || !character->model) {
        return;
    }
    MotionManagerWork *const manager = FindMotionManagerWork(character);
    if (!manager)
        return;

    const Vector3 position = manager->motion.rootWorldPosition;
    const float groundHeight = ResolveCharacterShadowGround(*character);
    switch (character->shadowMode - 1) {
    case 0:
    case 7:
        SubmitPosedCharacterShadow(character, character->model, position, groundHeight, 1.0f);
        return;
    case 1:
    case 2: {
        const Vector3i rotation{0, static_cast<std::int32_t>(character->entity.node.rotation.y + character->shadowYaw), 0};
        SubmitCharacterShadow(character, character->model, character->model, position, rotation, character->scale, groundHeight, -1.0f);
        return;
    }
    case 3:
    case 4:
        UpdateStencilShadowVolumesForParts(position, character->model, nullptr, character);
        return;
    case 5:
        SubmitPosedCharacterShadow(character, character->model, position, groundHeight, 0.5f);
        return;
    case 8: {
        const Vector3i rotation{0, static_cast<std::int32_t>(character->entity.node.rotation.y + character->shadowYaw), 0};
        SubmitCharacterShadow(character, character->model, character->model, position, rotation, character->scale, groundHeight, 1.0f);
        return;
    }
    case 10:
        SubmitPosedCharacterShadow(character, character->model, position, groundHeight, -2.0f);
        return;
    default:
        SubmitPosedCharacterShadow(character, character->model, position, groundHeight, -1.0f);
        return;
    }
}



// @verified - p
void REFB_0(Character *character, void *payload) {
    ReferenceBoardWork *const refboard = static_cast<ReferenceBoardWork *>(payload);
    if (refboard->sector == 2) {
        std::int32_t participant = 0;
        while (participant < static_cast<std::int8_t>(g_Conv.participantCount) && g_Conv.participantEntities[participant] != character)
            ++participant;
        if (participant >= static_cast<std::int8_t>(g_Conv.participantCount)) {
            if (refboard->colour.x <= 0.0f)
                return;
            refboard->colour.x = static_cast<float>(static_cast<double>(refboard->colour.x) - 0.02142857142857143);
            if (refboard->colour.x <= 0.0f) {
                refboard->colour = {};
                refboard->mode = 0;
                return;
            }
            refboard->colour.y = refboard->colour.x;
            refboard->colour.z = refboard->colour.x;
            return;
        }
    } else if (refboard->sector != 3) {
        return;
    }
    refboard->colour.x = static_cast<float>(static_cast<double>(refboard->colour.x) + 0.02142857142857143);
    if (static_cast<double>(refboard->colour.x) > 0.3)
        refboard->colour.x = 0.3f;
    refboard->mode = 1;
    refboard->colour.y = refboard->colour.x;
    refboard->colour.z = refboard->colour.x;
}

// The REFB light helpers clear the head point's near-zero components in place before measuring from it.
void RefBoardClearSmallComponents(Vector3 &point) {
    float *const components[] = {&point.x, &point.y, &point.z};
    for (float *component : components)
        if (static_cast<double>(std::fabs(*component)) < 0.00001)
            *component = 0.0f;
}

// The unit vector from `to` toward `from`, left as is when it has no length.
Vector3 RefBoardUnitDirection(const Vector3 &from, const Vector3 &to) {
    Vector3 direction{from.x - to.x, from.y - to.y, from.z - to.z};
    const float length = std::sqrt(direction.y * direction.y + direction.x * direction.x + direction.z * direction.z);
    if (length > 0.0f) {
        const float inverse = 1.0f / length;
        direction.x *= inverse;
        direction.y *= inverse;
        direction.z *= inverse;
    }
    return direction;
}

MotionNodeRuntime *RefBoardMotionNode(const MotionInformationWork &motion, std::uint8_t nodeId) {
    auto *node = static_cast<MotionNodeRuntime *>(motion.previousNodes);
    while (node && node->nodeId != 0xffU) {
        if (node->nodeId == nodeId)
            return node;
        ++node;
    }
    return nullptr;
}

Vector3 RefBoardHeadPosition(Character *character, const MotionInformationWork *motion) {
    if (motion) {
        if (MotionNodeRuntime *const node = RefBoardMotionNode(*motion, 47); node && node->transform) {
            return {node->transform->rows[3].x, node->transform->rows[3].y, node->transform->rows[3].z};
        }
    }
    return character ? GetPositionForEntity(character->entity.node) : Vector3{};
}



std::int16_t RefBoardCameraCharacterAngle(const MotionInformationWork &motion) {
    const std::uint16_t cameraYaw = static_cast<std::uint16_t>(g_fCamAccumYaw);
    const Vector3 camera = RefBoardUnitDirection({sin_fixed16(static_cast<std::int16_t>(cameraYaw)), 0.0f, -cos_fixed16(cameraYaw)}, {});
    const std::uint16_t euler = MOTN_ExtractEulerAngle(motion, 6);
    const Vector3 facing = RefBoardUnitDirection({-sin_fixed16(static_cast<std::int16_t>(euler)), 0.0f, -cos_fixed16(euler)}, {});
    float angle = ((facing.y * camera.y + facing.x * camera.x + facing.z * camera.z) * 180.0f - 180.0f) * -0.5f;
    if (static_cast<std::int16_t>(euler + cameraYaw) < 0)
        angle = -angle;
    if (angle > 90.0f)
        angle = angle * 0.5f - 90.0f;
    else if (angle < -90.0f)
        angle = angle * 0.5f + 90.0f;
    else
        angle *= -0.5f;
    return static_cast<std::int16_t>(static_cast<std::int32_t>(angle * 8192.0f / 45.0f));
}

// The camera-orbit point `angle` around the camera target, at the eye's horizontal distance from the target.
Vector3 RefBoardOrbitSource(const Vector3 &cameraEye, std::int16_t angle) {
    const Vector3 &target = g_CameraTargetPos;
    const float radius = std::sqrt((target.x - cameraEye.x) * (target.x - cameraEye.x) + (target.z - cameraEye.z) * (target.z - cameraEye.z));
    const std::uint16_t yaw = static_cast<std::uint16_t>(g_fCamAccumYaw + angle);
    return {target.x - sin_fixed16(static_cast<std::int16_t>(yaw)) * radius, target.y, cos_fixed16(yaw) * radius + target.z};
}


void RefBoardAimLight(Vector3 position, const Vector3 &source) {
    RefBoardClearSmallComponents(position);
    Vector3 direction = RefBoardUnitDirection(position, source);
    SetEffectTransformByIndex(31, &direction);
}



void RefBoardPlacePointLight(Vector3 position, const Vector3 &source) {
    RefBoardClearSmallComponents(position);
    const Vector3 direction = RefBoardUnitDirection(position, source);
    Vector3 lightPosition{static_cast<float>(static_cast<double>(position.x) - static_cast<double>(direction.x) * 0.4),
        static_cast<float>(static_cast<double>(position.y) - static_cast<double>(direction.y) * 0.4),
        static_cast<float>(static_cast<double>(position.z) - static_cast<double>(direction.z) * 0.4)};
    SetLightDirectionVectorByIndex(31, &lightPosition);
    SetLightFalloff(31, 0.25f);
    SetLightFalloffRangeByIndex(31, 1.5f);
    const ShenmueLightSlot *const slot = LGHT_GetDataSlot(31);
    SetLightTypeOrFlagsByIndex(31, slot && (slot->modeFlags & 8U) ? 11U : 3U);
}

Vector3 g_refBoardBasisRotation{};
Vector3 g_refBoardBasePosition{};
float g_refBoardRadius{};



void RefBoardEvaluateLight(Character *character, ReferenceBoardWork &refboard) {
    const MotionManagerWork *const manager = FindMotionManagerWork(character);
    const MotionInformationWork *const motion = manager ? &manager->motion : nullptr;
    const ShenmueLightSlot *slot = LGHT_GetDataSlot(31);
    if (!slot)
        return;
    g_refBoardSavedDirection = slot->direction;
    g_refBoardSavedColour = {slot->colourScale, slot->colourR, slot->colourG, slot->colourB};

    const Vector3 position = RefBoardHeadPosition(character, motion);
    const Vector3 cameraEye = g_CameraEyePos;
    SetLightTypeOrFlagsByIndex(31, (slot->modeFlags & 8U) ? 10U : 2U);
    const std::int32_t type = refboard.status == 1 ? refboard.type : g_refBoardDefaultType;
    const std::int32_t angle = refboard.status == 1 ? refboard.field024 : g_refBoardDefaultField024;
    switch (type) {
    case 0:
        RefBoardAimLight(position, cameraEye);
        break;
    case 1:
        RefBoardAimLight(position, RefBoardOrbitSource(cameraEye, static_cast<std::int16_t>(angle)));
        break;
    case 2:
        RefBoardAimLight(position, RefBoardOrbitSource(cameraEye, motion ? RefBoardCameraCharacterAngle(*motion) : 0));
        break;
    case 3:
        RefBoardPlacePointLight(position, cameraEye);
        break;
    case 4:
        RefBoardPlacePointLight(position, RefBoardOrbitSource(cameraEye, static_cast<std::int16_t>(angle)));
        break;
    case 5:
        RefBoardPlacePointLight(position, RefBoardOrbitSource(cameraEye, motion ? RefBoardCameraCharacterAngle(*motion) : 0));
        break;
    default:
        break;
    }

    Vector4 colour = g_effeObjectLightColour;
    if (refboard.status == 1)
        colour = {std::bit_cast<float>(refboard.reserved008), refboard.colour.x, refboard.colour.y, refboard.colour.z};
    SetRGBStruct(31, &colour);
    Shenmue_InitEffectByIndexExact();
}


void RefBoardRestoreLight() {
    SetRGBStruct(31, &g_refBoardSavedColour);
    SetEffectTransformByIndex(31, &g_refBoardSavedDirection);
    Shenmue_InitEffectByIndexExact();
}


void REFB_FigurePose(Character *character) {
    ReferenceBoardWork *const refboard = FindReferenceBoardWork(character);
    if (!character || !refboard)
        return;
    if (!g_refBoardActive) {
        refboard->figurePoseCallback(character);
        return;
    }

    const std::int32_t savedLightingMode = g_cachedGlobalLightingAdjustFlag;
    if (refboard->sector == 1) {
        if (savedLightingMode == 1) {
            g_cachedGlobalLightingAdjustFlag = 2;
            g_effectsNeedInitialization = 1;
        }
        const ShenmueLightSlot *const slot = LGHT_GetDataSlot(31);
        const bool wasEnabled = slot && slot->enabled;
        if (wasEnabled) {
            refboard->figurePoseCallback(character);
        } else {
            lightSetSlotEnabled(31, 0);
            SetLightTypeOrFlagsByIndex(31, 0);
            const Vector4 colour{1.0f, 1.0f, 1.0f, 1.0f};
            const Vector3 position{0.0f, 2.0f, 0.0f};
            const Vector3 direction{0.0f, -1.0f, 0.0f};
            SetRGBStruct(31, &colour);
            SetLightIntensity(31, 1.0f);
            SetLightFalloffRangeByIndex(31, 2.0f);
            SetLightFalloff(31, 0.25f);
            SetLightDirectionVectorByIndex(31, &position);
            SetEffectTransformByIndex(31, &direction);
            SetLightAngleByIndex(31, 1820);
            lightSetSlotEnabled(31, 1);
            RefBoardEvaluateLight(character, *refboard);
            refboard->figurePoseCallback(character);
            lightSetSlotEnabled(31, 0);
        }
        if (savedLightingMode == 1) {
            g_cachedGlobalLightingAdjustFlag = 1;
            g_effectsNeedInitialization = 0;
        }
        if (!wasEnabled)
            RefBoardRestoreLight();
        return;
    }
    if (!refboard->mode) {
        refboard->figurePoseCallback(character);
        return;
    }
    if (g_cachedGlobalLightingAdjustFlag == 1) {
        const ShenmueLightSlot *const sun = LGHT_GetDataSlot(0);
        Vector3 direction = sun ? sun->direction : Vector3{};
        const MotionManagerWork *const manager = FindMotionManagerWork(character);
        const Vector3 head = RefBoardHeadPosition(character, manager ? &manager->motion : nullptr);
        Vector3 base{g_CameraEyePos.x - head.x, g_CameraEyePos.y - head.y, g_CameraEyePos.z - head.z};
        Vector3::NormalizeInPlace(base);
        Vector3::NormalizeInPlace(direction);
        g_cachedGlobalLightingAdjustFlag = 2;
        g_effectsNeedInitialization = 1;
        float facing = static_cast<float>((static_cast<double>(direction.y * base.y + direction.x * base.x + direction.z * base.z) + 1.0) * 0.5);
        facing = 0.0f > facing ? 0.0f : facing;
        facing = 1.0f < facing ? 1.0f : facing;
        refboard->radius = refboard->colour.x * facing + refboard->colour.x;
        g_refBoardRadius = refboard->radius;
        g_refBoardBasePosition = base;
        g_refBoardBasisRotation = {workTransformMatrix.rows[1].x * base.y + workTransformMatrix.rows[0].x * base.x + workTransformMatrix.rows[2].x * base.z,
            workTransformMatrix.rows[1].y * base.y + workTransformMatrix.rows[0].y * base.x + workTransformMatrix.rows[2].y * base.z,
            workTransformMatrix.rows[1].z * base.y + workTransformMatrix.rows[0].z * base.x + workTransformMatrix.rows[2].z * base.z};
        REFB_FigurePose(character);
        return;
    }

    const ShenmueLightSlot *const slot = LGHT_GetDataSlot(31);
    if (slot && !slot->enabled) {
        lightSetSlotEnabled(31, 1);
        RefBoardEvaluateLight(character, *refboard);
        refboard->figurePoseCallback(character);
        lightSetSlotEnabled(31, 0);
        RefBoardRestoreLight();
    }
}

} // namespace
bool CHAR_TransformModelNodePoint(Character *character, std::uint32_t nodeKind, const Vector3 &localPoint, Vector3 *output) {
    if (!character || !output)
        return false;
    Matrix4x4 transform;
    SetupTransformationMatrix(character, nodeKind, &transform);
    *output = {
        localPoint.x * transform.rows[0].x + localPoint.y * transform.rows[1].x + localPoint.z * transform.rows[2].x + transform.rows[3].x,
        localPoint.x * transform.rows[0].y + localPoint.y * transform.rows[1].y + localPoint.z * transform.rows[2].y + transform.rows[3].y,
        localPoint.x * transform.rows[0].z + localPoint.y * transform.rows[1].z + localPoint.z * transform.rows[2].z + transform.rows[3].z,
    };
    return true;
}

void STEP_Allocate(Character *character, std::uint8_t surface, std::uint8_t authoredMode) {
    auto *const work = static_cast<FootstepWork *>(AllocateHeapMemoryMODL(character, SHEN_NAME(STEP), sizeof(FootstepWork), nullptr));
    work->surface = surface;
    work->enabled = 1;
    work->authoredMode = authoredMode;
}

void CLOK_Allocate(Character *character, std::int16_t minuteOffset) {
    auto *const work = static_cast<ClockWork *>(AllocateHeapMemoryMODL(character, SHEN_NAME(CLOK), sizeof(ClockWork), CLOK_Callback));
    if (work)
        work->minuteOffset = minuteOffset;
}

namespace {} // namespace

namespace {} // namespace

void GAME_Allocate(Character *character) {
    EnsureGameDisplayWork(character);
}

void CALE_Allocate(Character *character) {
    auto *const calendar = static_cast<CalendarWork *>(AllocateHeapMemoryMODL(character, SHEN_NAME(CALE), sizeof(CalendarWork), CALE_Callback));
    if (!calendar)
        return;
    GAME_Allocate(character);
    CALE_Callback(character, calendar);
}

void GAME_CommitStagedEntries(Character *character) {
    GameDisplayWork *const display = FindGameDisplayWork(character);
    if (!display)
        return;
    std::memcpy(display->active.data(), display->staged.data(), static_cast<std::size_t>(display->stagedCount) * sizeof(GameDisplayEntry));
    display->activeCount = display->stagedCount;
    display->cachePublished = 0;
    display->stagedCount = 0;
}

void GAME_StageNodeData(Character *character, std::uint32_t nodeId, const Vector3 *position, float renderAlpha, const Vector3 *basis) {
    if (GameDisplayWork *const display = FindGameDisplayWork(character))
        StageGameDisplayNode(*display, nodeId, position, renderAlpha, basis);
}

void GAME_ConfigureNumericDisplay(Character *character, std::uint32_t baseNodeId, const Vector3 &digitStep, std::uint32_t maxValue, std::int32_t decimalShift) {
    GameDisplayWork *const display = FindGameDisplayWork(character);
    if (!display)
        return;
    display->baseNodeId = baseNodeId;
    display->digitStep = digitStep;
    display->maxValue = maxValue;
    display->decimalShift = decimalShift;
}

void GAME_StageNumericDisplayValue(Character *character, std::uint32_t value, const Vector3 *origin, float renderAlpha, const Vector3 *basis) {
    GameDisplayWork *const display = FindGameDisplayWork(character);
    if (!display || !origin)
        return;
    for (std::int32_t shift = display->decimalShift; shift > 0; --shift)
        value *= 10U;
    if (value > display->maxValue)
        value = display->maxValue;

    bool emittedNonZero = false;
    for (std::int32_t digitIndex = 7; digitIndex >= 0; --digitIndex) {
        Vector3 position = *origin;
        std::uint32_t divisor = 1;
        for (std::int32_t step = 0; step < digitIndex; ++step) {
            divisor *= 10U;
            position.x = position.x + display->digitStep.x;
            position.y = position.y + display->digitStep.y;
            position.z = position.z + display->digitStep.z;
        }
        const std::uint32_t digit = value / divisor;
        value %= divisor;
        if (digit)
            emittedNonZero = true;
        if (emittedNonZero || digitIndex == 0)
            StageGameDisplayNode(*display, display->baseNodeId + digit, &position, renderAlpha, basis);
    }
}

void SetupTransformMatrixForMotionTask(Character *target, std::int32_t anchorNodeId, Character *source, std::uint32_t sourceModelNodeKind,
    const Vector3 *translationOffset, const Vector3i *eulerOffset) {
    MotionManagerWork *const manager = FindMotionManagerWork(target);
    if (!manager)
        return;
    manager->transformSourceCharacter = source;
    manager->transformModelEntryCount = sourceModelNodeKind;
    if (!source) {
        manager->motion.externalTransform = nullptr;
        manager->motion.externalTransformAnchorNodeId = 0;
        return;
    }
    SetupTransformationMatrix(source, sourceModelNodeKind, &manager->transformMatrixCache);
    manager->transformTranslationOffset = translationOffset ? *translationOffset : Vector3{};
    manager->transformEulerOffset = eulerOffset ? *eulerOffset : Vector3i{};
    manager->motion.externalTransform = &manager->transformMatrixCache;
    manager->motion.externalTransformAnchorNodeId = anchorNodeId;
}

namespace {
void RecursiveSetupXForms(std::byte *modelBase, const Mt5RuntimeNode *node, std::uint32_t nodeKind, Matrix4x4 *output) {
    while (true) {
        if (node->siblingOffset)
            Matrix4x4_Push();
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        Matrix4x4::SetViewMatrixFromEuler(node->rotation.y, node->rotation.x, node->rotation.z);
        if (static_cast<std::uint32_t>(static_cast<std::int16_t>(node->nodeKind)) == nodeKind)
            *(output ? output : g_matrixStack.top.stack) = workTransformMatrix;
        if (node->childOffset)
            RecursiveSetupXForms(modelBase, reinterpret_cast<const Mt5RuntimeNode *>(modelBase + node->childOffset), nodeKind, output);
        if (!node->siblingOffset)
            return;
        Matrix4x4::Pop(1);
        node = reinterpret_cast<const Mt5RuntimeNode *>(modelBase + node->siblingOffset);
    }
}
} // namespace

void SetupTransformationMatrix(Character *character, std::uint32_t nodeKind, Matrix4x4 *output) {
    Matrix4x4_Push();
    workTransformMatrix = transformMatrix_prePass;
    if (const Mt5RuntimeNode *const model = static_cast<const Mt5RuntimeNode *>(character->model)) {
        const Vector3 position = GetPositionForEntity(character->entity.node);
        const Vector3i rotation = GetEulerAnglesForEntity(character->entity.node);
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        Matrix4x4::SetViewMatrixFromEuler(rotation.y, rotation.x, rotation.z);
        *(output ? output : g_matrixStack.top.stack) = workTransformMatrix;
        RecursiveSetupXForms(MT5_ModelBaseForNode(model), model, nodeKind, output);
    } else {
        *(output ? output : g_matrixStack.top.stack) = transformMatrix_prePass;
    }
    Matrix4x4::Pop(1);
}

static bool CharacterNeedsAlphaMaskedDepthPass(Character *character) {
    if ((character->flags.value & CharacterFlagHasCpct) == 0)
        return false;
    const ImageModuleWork *const image = IMGM_GetWork(character);
    const std::uint16_t pose = image ? image->basePoseId : 0;
    if ((pose >= 16 && pose <= 18) || pose == 20)
        return true;
    return HLib_GetCharacterComponent(character, SHEN_NAME(VEHI)) == nullptr;
}

void Character_SubmitForDrawing(Character *character) {
    if (!character || !character->model || !character->figurePoseCallback || (character->motionFlags & 0x3fU) != 0x3fU) {
        return;
    }

    const bool alphaDepthPass = CharacterNeedsAlphaMaskedDepthPass(character);
    if (alphaDepthPass) {
        SetRenderFlags(52, 1);
        SetRenderFlags(1, 16);
        SetRenderFlags(55, 7681);
        SetRenderFlags(3, 255);
    } else {
        SetRenderFlags(52, 1);
        SetRenderFlags(53, 7680);
        SetRenderFlags(55, 7680);
        SetRenderFlags(56, 519);
        SetRenderFlags(1, 0);
        SetRenderFlags(2, 255);
        SetRenderFlags(54, 7680);
        SetRenderFlags(55, 0);
    }

    character->figurePoseCallback(character);
    if (alphaDepthPass && DX11::g_ppD3DDeviceContext) {
        SetupCharacterAlphaDepthTarget(DX11::g_ppD3DDeviceContext);
        SetMt5AlphaMaskedDepthPass(true);
        g_suppressClothPose = true;
        character->figurePoseCallback(character);
        g_suppressClothPose = false;
        SetMt5AlphaMaskedDepthPass(false);
        SetupPPFX(DX11::g_ppD3DDeviceContext);

        SetRenderFlags(52, 0);
        SetRenderFlags(1, 0);
        SetRenderFlags(55, 7680);
        SetRenderFlags(3, 15);
    } else {
        SetRenderFlags(52, 0);
        SetRenderFlags(53, 7680);
        SetRenderFlags(55, 7680);
        SetRenderFlags(56, 519);
        SetRenderFlags(1, 0);
        SetRenderFlags(2, -1);
        SetRenderFlags(54, 7680);
        SetRenderFlags(55, 7680);
    }
}

void MOTN_Alloc(Character *character, bool shouldInitializeWithoutBuffers) {
    if (!character || HLib_GetCharacterComponent(character, SHEN_NAME(MOTM))) {
        return;
    }
    auto *const manager = static_cast<MotionManagerWork *>(AllocateHeapMemoryMODL(character, SHEN_NAME(MOTM), sizeof(MotionManagerWork), MOTM_Callback));
    if (!manager)
        return;
    if (CharacterComponentNode *const component = HLib_GetCharacterComponent(character, SHEN_NAME(MOTM))) {
        component->destroyCallback = MOTM_Cleanup;
    }

    character->figurePoseCallback = MOTM_Update;
    character->drawCallback = MOTM_TransformMatrix;
    character->alphaDrawCallback = MOTM_TransformMatrix;
    character->processShadowCallback = MOTM_UpdateCharaShadow;
    manager->flags = shouldInitializeWithoutBuffers ? 2u : 0u;
    if (shouldInitializeWithoutBuffers) {
        UNFO_Init(manager->motion);
        manager->motion.motionFlags |= 0x20000000u;
        manager->motion.previousNodes = nullptr;
        manager->motion.activeNodes = nullptr;
        manager->motion.blendTargetNodes = nullptr;
        manager->motion.transitionNodes = nullptr;
        manager->motion.secondaryNodes = nullptr;
        manager->motion.nodeMatrixPool = nullptr;
        manager->motion.activeNodesCookie = 0;
        manager->motion.blendTargetNodesCookie = 0;
        manager->motion.transitionNodesCookie = 0;
        manager->motion.secondaryNodesCookie = 0;
    } else {
        MOTI_InitAndAllocPrimaryBuffers(manager->motion);
    }
    manager->transformSourceCharacter = nullptr;
    manager->transformModelEntryCount = 0;
    manager->motion.entityEuler = character->entity.node.rotation;
}

void AllocREFB(Character *character, std::int32_t sector) {
    if (!character)
        return;
    ReferenceBoardWork *refboard = FindReferenceBoardWork(character);
    if (!refboard) {
        const bool installUpdate = sector < 5;
        if (!installUpdate)
            sector -= 5;
        refboard =
            static_cast<ReferenceBoardWork *>(AllocateHeapMemoryMODL(character, SHEN_NAME(REFB), sizeof(ReferenceBoardWork), installUpdate ? REFB_0 : nullptr));
        if (!refboard)
            return;
        if (!installUpdate) {
            if (CharacterComponentNode *const component = HLib_GetCharacterComponent(character, SHEN_NAME(REFB))) {
                component->flags &= ~1U;
            }
        }
        *refboard = {};
        refboard->figurePoseCallback = character->figurePoseCallback;
        character->figurePoseCallback = REFB_FigurePose;
    }
    refboard->sector = static_cast<std::uint32_t>(sector);
    if (sector == 2)
        refboard->status = 1;
}

#pragma pack(push, 1)

#pragma pack(pop)

namespace {} // namespace

namespace {
#pragma pack(push, 4)
struct TitleCharacterWork {
    std::uint32_t state;
    std::uint32_t reserved04;
    void *sourceFile;
    PackModel *model;
    Vector3 lookTarget;
    std::uint32_t moveListState;
};
#pragma pack(pop)
static_assert(sizeof(TitleCharacterWork) == 0x28);
static_assert(offsetof(TitleCharacterWork, lookTarget) == 0x18);

void SPSS_Callback(Character *character, void *payload) {
    auto *const work = static_cast<TitleCharacterWork *>(payload);
    MotionManagerWork *const manager = MOTM_GetWork(character);
    MotionInformationWork &motion = manager->motion;
    const auto requestAnimation = [&](std::uint16_t animation) {
        motion.motionFlags |= 0x80U;
        motion.pendingAnimId = animation;
    };
    const auto setDisplayEnabled = [&](bool enabled) {
        const bool previous = (character->motionFlags & 4U) != 0;
        if (enabled)
            character->motionFlags |= 4U;
        else
            character->motionFlags &= static_cast<std::uint8_t>(~4U);
        if (g_bDebugLogVisibility && (character->flags.value & 0x400U) != 0U && previous != enabled)
            CHAR_LogNPCVisibility(character, enabled);
    };
    switch (work->state) {
    case 0: {
        work->state = 1;
        shenApplyTranslationToAll(character->entity.node, {-2.2f, 0.0f, 0.7f});
        std::int16_t yaw = static_cast<std::int16_t>(0x8000);
        for (CharacterNode *node = character->entity.node.child; node; node = node->child)
            yaw = static_cast<std::int16_t>(yaw - node->rotation.y);
        character->entity.node.rotation.y = yaw;
        work->lookTarget = g_CameraEyePos;
        SetNewAuthorGameState(9);
        setDisplayEnabled(true);
        manager->flags |= 1U;
        requestAnimation(0xb23f);
        break;
    }
    case 1:
        if (MOTN_GetFrameCountFromAnimId(motion.currentAnimId) * 0.5f > motion.frameTimer) {
            requestAnimation(0xb23e);
            ++work->state;
        }
        break;
    case 2:
        if ((motion.motionFlags & 2U) != 0U) {
            requestAnimation(0xb23d);
            ++work->state;
        }
        break;
    case 3:
        if ((motion.motionFlags & 2U) != 0U) {
            SetLookPointDetails(character, 16, &work->lookTarget, 0);
            requestAnimation(0xb206);
            ++work->state;
        }
        break;
    case 4:
        if ((motion.motionFlags & 2U) != 0U) {
            g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
            const float random = ((g_EnvPrngSeed >> 16U) & 0x7fffU) * 0.000030517578125f;
            if (random < 0.25f) {
                requestAnimation(0xb205);
                ++work->state;
            }
        }
        break;
    case 5:
        if ((motion.motionFlags & 2U) != 0U) {
            requestAnimation(0xb204);
            work->state = 3;
        }
        break;
    }
    const std::uint16_t pressed = g_inputState.slots[0].pressedPriority;
    if (work->moveListState == 0) {
        if (pressed == 0x0400U) {
            setDisplayEnabled(false);
            work->moveListState = 1;
        }
    } else if (work->moveListState == 1) {
        EnqueueWZOG();
        work->moveListState = 2;
    } else if (pressed == 0x0200U) {
        setDisplayEnabled(true);
        shenRefreshUI();
        work->moveListState = 0;
    }
}
} // namespace



void MAIL_Alloc() {
    MdlmRecord *record = MDLM_StoreCharacterName(g_mdlmTask, "NZM_M");
    if (!record)
        return;
    if (record->imageCount == 0) {
        record = MDLM_DefineImages("NZM_M", 1, nullptr);
        if (!record || !MDLM_InsertImageDefinition(record, 0, "NZM_M.MT5", 1))
            return;
    }
    MdlmImageEntry &entry = record->images[0];
    entry.selector = 1;
    MDLM_PublishImage(&entry, d3t::ModelLoad_FBX_Replace("/scene/80/model/chara", entry.imageName), true);
}

bool SPSS_Init() {
    auto *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    Character *const character = AddCharacterHandler(CharacterHandlerPriority::Low);
    if (!character)
        return false;
    auto *const work = static_cast<TitleCharacterWork *>(AllocateHeapMemoryMODL(character, SHEN_NAME(SPSS), sizeof(TitleCharacterWork), SPSS_Callback));
    if (!work)
        return false;
    *work = {};
    HLib_GetCharacterComponent(character, SHEN_NAME(SPSS))->destroyCallback = ComponentNoOp;
    CHID_Init(control->scenarioTask, SHEN_NAME(HRSK), character);
    work->sourceFile = ReadFile("/scene/80/model/chara", "NZM_M.MT5", SHEN_NAME(SPSS));
    work->model = MT5_InitRawModelAndTextures(work->sourceFile, "NZM_M.MT5", nullptr);
    MAIL_Alloc();
    static_cast<void>(IMGM_Create(character, "NZM_M"));
    character->flags.value &= ~CharacterFlagActive;
    character->model = work->model->model;
    character->renderOpacity = 1.0f;
    character->shadowModel = g_shadowMeshData;
    MOTN_Alloc(character, false);
    Clipper_Alloc(character);
    CLMD_Alloc(character, 0);
    OSAGE_Alloc(character);
    return true;
}

namespace {} // namespace

void refbSetColour(Character *character, const Vector3 &colour) {
    if (auto *const refboard = FindReferenceBoardWork(character)) {
        refboard->status = 1;
        refboard->colour = colour;
    }
}


void refbSetSector(Character *character, std::uint32_t sector) {
    if (ReferenceBoardWork *const refboard = FindReferenceBoardWork(character)) {
        refboard->sector = sector;
        refboard->status = 1;
        if (!sector)
            refboard->mode = 0;
    }
}

void SetRefBoardValues(Character *character, std::int32_t type, std::int32_t field024) {
    if (!character) {
        g_refBoardDefaultType = type;
        g_refBoardDefaultField024 = field024;
        return;
    }
    ReferenceBoardWork *const refboard = FindReferenceBoardWork(character);
    if (!refboard)
        return;
    refboard->type = type;
    refboard->field024 = field024;
    if (refboard->status == 0) {
        refboard->status = 1;
        refboard->colour = {g_effeObjectLightColour.y, g_effeObjectLightColour.z, g_effeObjectLightColour.w};
    }
}

namespace {

constexpr std::int32_t kCharacterTaskToken = SHEN_NAME(CHAR);
constexpr std::int32_t kTaskDataToken = SHEN_NAME(TASK);
constexpr std::int32_t kCharacterLinkToken = SHEN_NAME(CHAR);
constexpr std::int32_t kCharacterIdToken = SHEN_NAME(CHID);
constexpr std::uint32_t kCharacterEntityName = SHEN_NAME(CEWP);
std::int32_t g_characterValidationUpdateSequence = 0;
std::int32_t g_characterValidationDestroySequence = 0;
std::array<std::int32_t, 2> g_characterValidationUpdates{};
std::array<std::int32_t, 2> g_characterValidationDestroys{};





void ConfigureCharacterTaskForDestruction(HLib::Task *task) {
    if (!task)
        return;
    task->initCallbackFnPtr = nullptr;
    task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
    task->autoInitTaskCallbackFnPtr = nullptr;
}




void CharacterHandler_FigurePose(Character *character) {
    if (!g_updateRenderFrameMode)
        return;
    g_drawingImageModel = IMGM_GetCurrentImageModel(character);
    g_drawingCharacter = character;
    if (character && character->model && character->drawCallback) {
        PushCharacterMatrix();
        TransformCharacterHierarchyZYX(&character->entity.node);
        if (character->scale.x != 1.0f || character->scale.y != 1.0f || character->scale.z != 1.0f) {
            Matrix4x4::Scale(character->scale.x, character->scale.y, character->scale.z);
            g_sceneObjectMaxScale = character->scale.x;
        }
        character->drawCallback(character->model, character->renderOpacity);
        g_sceneObjectMaxScale = 1.0f;
        PopCharacterMatrix();
    }
    g_drawingImageModel = nullptr;
    g_drawingCharacter = nullptr;
}


void CharacterHandler_ProcessShadow(Character *character) {
    if (!character || !g_updateRenderFrameMode || !character->shadowModel)
        return;

    Matrix4x4_Push();
    const float groundHeight = ResolveCharacterShadowGround(*character);
    const Vector3 position = GetPositionForEntity(character->entity.node);
    Vector3i rotation = GetEulerAnglesForEntity(character->entity.node);
    void *shadowModel = character->model;
    float direction = -1.0f;
    switch (character->shadowMode) {
    case 1:
        shadowModel = character->shadowModel == g_shadowMeshData ? character->model : character->shadowModel;
        direction = 1.0f;
        break;
    case 2:
    case 3:
        rotation.y = static_cast<std::int32_t>(rotation.y + character->shadowYaw);
        break;
    case 6:
        direction = 0.5f;
        break;
    case 8:
        direction = 1.0f;
        break;
    case 9:
        rotation.y = static_cast<std::int32_t>(rotation.y + character->shadowYaw);
        direction = 1.0f;
        break;
    case 10:
        shadowModel = character->shadowModel == g_shadowMeshData ? character->model : character->shadowModel;
        direction = -1.0f;
        break;
    case 11:
        direction = -2.0f;
        break;
    default:
        break;
    }
    SubmitCharacterShadow(character, character->model, shadowModel, position, rotation, character->scale, groundHeight, direction);
    Matrix4x4::Pop(1);
}


std::array<Mt5RuntimeNode *, 12> g_loadedSelectModels{};

} // namespace



void CharacterHandler_DrawSubmitAndScale(void *model, float) {
    if (!model)
        return;
    if (g_drawingCharacter) {
        SubmitRiggedCharacterModel(model, g_drawingCharacter, 1.0f);
    } else if (!CachedAttachmentOwnsSubmission(model, 1.0f, workTransformMatrix)) {
        d3t::MT5_PushAndReadNodeTree(model, nullptr, 1.0f);
    }
}


void CharacterHandler_AlphaDraw(void *model, float opacity) {
    if (!model)
        return;
    if (g_drawingCharacter) {
        SubmitRiggedCharacterModel(model, g_drawingCharacter, opacity);
    } else if (!CachedAttachmentOwnsSubmission(model, opacity, workTransformMatrix)) {
        d3t::MT5_PushAndReadNodeTree(model, nullptr, opacity);
    }
}

void VEND_ReleaseModels() {
    for (Mt5RuntimeNode *&model : g_loadedSelectModels) {
        HMDL_FreeAssignedModel(model);
        model = nullptr;
    }
}




void VEND_Initialize(Character *character, std::int32_t variant) {
    if (!character)
        return;
    g_loadedSelectModels.fill(nullptr);
    if (variant != 2 || !character->model)
        return;
    auto *const root = static_cast<Mt5RuntimeNode *>(character->model);
    Mt5RuntimeNode *product = FindModelNodeByKind(root, 0xf2);
    if (!product)
        return;

    const auto linkSibling = [](Mt5RuntimeNode *node, Mt5RuntimeNode *sibling) {
        if (!node)
            return false;
        if (!sibling) {
            node->siblingOffset = 0;
            return false;
        }
        const std::ptrdiff_t offset = reinterpret_cast<std::byte *>(sibling) - g_pCurrentTaskData;
        if (offset < (std::numeric_limits<std::int32_t>::min)() || offset > (std::numeric_limits<std::int32_t>::max)()) {
            return false;
        }
        node->siblingOffset = static_cast<std::int32_t>(offset);
        return true;
    };

    product->nodeKind = 172;
    product->position.x = static_cast<float>(static_cast<double>(product->position.x) - 0.215);
    for (std::size_t index = 1; index <= 5; ++index) {
        auto *const clone = static_cast<Mt5RuntimeNode *>(InitChara(character->model, product));
        g_loadedSelectModels[index] = clone;
        if (!linkSibling(product, clone))
            return;
        ++clone->nodeKind;
        clone->position.x = static_cast<float>(static_cast<double>(clone->position.x) + 0.086);
        product = clone;
    }

    auto *const firstSelection = static_cast<Mt5RuntimeNode *>(InitChara(character->model, product));
    g_loadedSelectModels[10] = firstSelection;
    if (!linkSibling(product, firstSelection))
        return;
    firstSelection->nodeKind = 182;
    if (Mt5RuntimeNode *const source = FindModelNodeByKind(root, 176))
        firstSelection->position.x = source->position.x;
    firstSelection->position.y = static_cast<float>(static_cast<double>(firstSelection->position.y) - 0.3);

    auto *const secondSelection = static_cast<Mt5RuntimeNode *>(InitChara(character->model, firstSelection));
    g_loadedSelectModels[11] = secondSelection;
    if (!linkSibling(firstSelection, secondSelection))
        return;
    ++secondSelection->nodeKind;
    secondSelection->position.x = static_cast<float>(static_cast<double>(secondSelection->position.x) + 0.086);

    for (std::int16_t index = 0; index < 6; ++index) {
        Mt5RuntimeNode *const display = FindModelNodeByKind(root, static_cast<std::int16_t>(152 + index));
        const Mt5RuntimeNode *const row = FindModelNodeByKind(root, static_cast<std::int16_t>(172 + index));
        if (!display || !row)
            continue;
        display->position.x = row->position.x;
        display->position.y = static_cast<float>(static_cast<double>(display->position.y) + 1.0);
    }
    for (std::int16_t index = 0; index < 2; ++index) {
        Mt5RuntimeNode *const display = FindModelNodeByKind(root, static_cast<std::int16_t>(162 + index));
        const Mt5RuntimeNode *const selection = FindModelNodeByKind(root, static_cast<std::int16_t>(182 + index));
        if (!display || !selection)
            continue;
        display->position.x = selection->position.x;
        display->position.y = static_cast<float>(static_cast<double>(display->position.y) + 0.715);
    }
}



void KNIF_FigurePose(Character *character) {
    SubmitBoneEffectCharacterModel(character ? character->model : nullptr, character);
}

void HLTaskFunc_CharacterHandler_Callback(HLib::TaskData *taskData) {
    auto *const character = reinterpret_cast<Character *>(taskData);
    if (!character || character->entity.name != kCharacterEntityName)
        return;
    if ((character->flags.value & 0x24u) == CharacterFlagVisible) {
        for (CharacterComponentNode *component = character->componentList; component; component = component->next) {
            if ((component->flags & 1u) && component->updateCallback) {
                component->updateCallback(character, component + 1);
            }
        }
    }
    if (character->skipRootMotionDelta == 0 &&
        (character->rootMotionDelta.x != 0.0f || character->rootMotionDelta.y != 0.0f || character->rootMotionDelta.z != 0.0f)) {
        character->entity.node.position.x += character->rootMotionDelta.x;
        character->entity.node.position.y += character->rootMotionDelta.y;
        character->entity.node.position.z += character->rootMotionDelta.z;
    }
    character->rootMotionDelta = {};
    character->skipRootMotionDelta = 0;
}

Character *HLTaskFunc_CharacterHandler_Enqueue(std::uint16_t taskIndex, HLib::Task **taskOut) {
    HLib::Task *const task =
        HLib_EnqueueTaskWithoutParameter(HLTaskFunc_CharacterHandler_Callback, static_cast<std::int16_t>(taskIndex), 4, kCharacterTaskToken);
    if (!task || task->task_data_ptr) {
        ConfigureCharacterTaskForDestruction(task);
        if (taskOut)
            *taskOut = nullptr;
        return nullptr;
    }

    auto *const character = static_cast<Character *>(HLib_AllocHeapMemBlock(sizeof(Character), kTaskDataToken));
    if (!character) {
        ConfigureCharacterTaskForDestruction(task);
        if (taskOut)
            *taskOut = nullptr;
        return nullptr;
    }
    std::memset(character, 0, sizeof(*character));
    HLib_EnableTokenCheck(character);
    task->task_data_ptr = reinterpret_cast<HLib::TaskData *>(character);
    if (taskOut)
        *taskOut = task;

    character->entity.name = kCharacterEntityName;
    character->scale = {1.0f, 1.0f, 1.0f};
    character->shadowScale = {1.0f, 1.0f, 1.0f};
    character->flags.value = CharacterFlagUnknown | CharacterFlagVisible | CharacterFlagReflectInScriptPlane;
    character->unknown_C4 = -1;
    character->unknown_C6 = -1;
    character->clipAngle = 1.8f;
    character->allocatedSize = sizeof(Character);
    character->renderOpacity = 1.0f;
    character->lightMask = 7;
    character->lightRadius = 10.0f;
    character->shadowState.lightIndex[0] = -1;
    character->shadowState.lightIndex[1] = -1;
    character->shadowState.flag[0] = 1;
    character->shadowState.flag[1] = 1;
    character->motionFlags = 0x3b;
    character->figurePoseCallback = CharacterHandler_FigurePose;
    character->processShadowCallback = CharacterHandler_ProcessShadow;
    character->drawCallback = CharacterHandler_DrawSubmitAndScale;
    character->alphaDrawCallback = CharacterHandler_AlphaDraw;
    return character;
}

Character *AddCharacterHandler(CharacterHandlerPriority priority) {
    if (!g_mapControlTask || !g_mapControlTask->task_data_ptr)
        return nullptr;
    const auto index = static_cast<std::size_t>(priority);
    if (index >= kCharacterHandlerTaskIndices.size())
        return nullptr;

    if (g_currUIState == -1 || g_currUIState == static_cast<std::int32_t>(kQteMinigameUiElement)) {
        UIBaseRuntime &qte = g_uiElements[kQteMinigameUiElement];
        if (!qte.isVisible || !qte.destroy || qte.destroy() == 1 || g_busy_reset == 1) {
            qte.isVisible = 0;
        }
    }

    HLib::Task *task = nullptr;
    Character *const character = HLTaskFunc_CharacterHandler_Enqueue(kCharacterHandlerTaskIndices[index], &task);
    if (!character)
        return nullptr;

    auto *const control = reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
    auto *const link = static_cast<CharacterBucketNode *>(HLib_AllocHeapMemBlock(sizeof(CharacterBucketNode), kCharacterLinkToken));
    const std::size_t bucket = reinterpret_cast<std::uintptr_t>(character) % control->characterBuckets.size();
    link->next = static_cast<CharacterBucketNode *>(control->characterBuckets[bucket]);
    link->character = character;
    link->task = task;
    control->characterBuckets[bucket] = link;
    return character;
}


Character *CHID_Find(HLib::Task *scenarioTask, std::uint32_t id) {
    if (!scenarioTask || !scenarioTask->task_data_ptr)
        return nullptr;
    auto *const scenario = reinterpret_cast<ScenarioTaskData *>(scenarioTask->task_data_ptr);
    CharacterIdLookupNode *node = scenario->characterIdRoot;
    while (node) {
        if (node->id == id)
            return node->character;
        node = static_cast<std::int32_t>(id) > static_cast<std::int32_t>(node->id) ? node->left : node->right;
    }
    return nullptr;
}



void CHID_Init(HLib::Task *scenarioTask, std::uint32_t id, Character *character) {
    if (!scenarioTask || !scenarioTask->task_data_ptr || !character || !id)
        return;
    auto *const scenario = reinterpret_cast<ScenarioTaskData *>(scenarioTask->task_data_ptr);
    CharacterIdLookupNode **insertion = &scenario->characterIdRoot;
    bool duplicate = false;
    while (*insertion) {
        const auto nodeId = static_cast<std::int32_t>((*insertion)->id);
        if (nodeId == static_cast<std::int32_t>(id)) {
            duplicate = true;
            break;
        }
        insertion = nodeId >= static_cast<std::int32_t>(id) ? &(*insertion)->right : &(*insertion)->left;
    }
    auto *const node = static_cast<CharacterIdLookupNode *>(HLib_AllocHeapMemBlock(sizeof(CharacterIdLookupNode), kCharacterIdToken));
    if (!node)
        return;
    std::memset(node, 0, sizeof(*node));
    node->id = id;
    node->character = character;
    if (!duplicate)
        *insertion = node;

    const std::size_t bucket = reinterpret_cast<std::uintptr_t>(character) % scenario->characterBuckets.size();
    node->bucketNext = scenario->characterBuckets[bucket];
    scenario->characterBuckets[bucket] = node;
}
void SCEN_PromoteCharacterIndexNode(HLib::Task *scenarioTask, Character *character) {
    if (!scenarioTask || !scenarioTask->task_data_ptr || !character)
        return;
    auto *const scenario = reinterpret_cast<ScenarioTaskData *>(scenarioTask->task_data_ptr);
    const std::size_t bucket = reinterpret_cast<std::uintptr_t>(character) % scenario->characterBuckets.size();
    CharacterIdLookupNode **link = &scenario->characterBuckets[bucket];
    while (*link && (*link)->character != character)
        link = &(*link)->bucketNext;
    CharacterIdLookupNode *const node = *link;
    if (!node || link == &scenario->characterBuckets[bucket])
        return;
    *link = node->bucketNext;
    node->bucketNext = scenario->characterBuckets[bucket];
    scenario->characterBuckets[bucket] = node;
}

void SCEN_UnlinkCharacterIndexNode(HLib::Task *scenarioTask, Character *character) {
    if (!scenarioTask || !scenarioTask->task_data_ptr || !character)
        return;
    auto *const scenario = reinterpret_cast<ScenarioTaskData *>(scenarioTask->task_data_ptr);
    const std::size_t bucket = reinterpret_cast<std::uintptr_t>(character) % scenario->characterBuckets.size();
    CharacterIdLookupNode **bucketLink = &scenario->characterBuckets[bucket];
    while (*bucketLink && (*bucketLink)->character != character) {
        bucketLink = &(*bucketLink)->bucketNext;
    }
    CharacterIdLookupNode *const node = *bucketLink;
    if (!node)
        return;
    *bucketLink = node->bucketNext;

    CharacterIdLookupNode **treeLink = &scenario->characterIdRoot;
    while (*treeLink && *treeLink != node) {
        treeLink = static_cast<std::int32_t>(node->id) > static_cast<std::int32_t>((*treeLink)->id) ? &(*treeLink)->left : &(*treeLink)->right;
    }
    if (*treeLink == node) {
        if (!node->left) {
            *treeLink = node->right;
        } else if (!node->right) {
            *treeLink = node->left;
        } else {
            CharacterIdLookupNode **successorLink = &node->right;
            while ((*successorLink)->left) {
                successorLink = &(*successorLink)->left;
            }
            CharacterIdLookupNode *const successor = *successorLink;
            *successorLink = successor->right;
            successor->left = node->left;
            successor->right = node->right;
            *treeLink = successor;
        }
    }
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(node), 0);
}

namespace {} // namespace

const std::uint32_t *ScenarioWords(void *rawParams) {
    return static_cast<const std::uint32_t *>(rawParams);
}
namespace {} // namespace

Character *ScenarioCharacter(ScenarioContext *context, std::uint32_t characterId) {
    return context && context->task && characterId ? CHID_Find(context->task->task, characterId) : nullptr;
}
namespace {

void SetScenarioResult(ScenarioContext *context, std::uint32_t value) {
    if (context && context->stack)
        context->stack->accumulator = value;
}

} // namespace

void Script_motmGetCharaFlags(ScenarioContext *context, void *rawParams) {
    const std::uint32_t *const params = ScenarioWords(rawParams);
    if (!params)
        return;
    MotionManagerWork *const manager = MOTM_GetWork(ScenarioCharacter(context, params[0]));
    if (params[2]) {
        const auto value = manager ? static_cast<std::int32_t>(static_cast<std::int8_t>(manager->motion.poseEvaluationStage)) : 0;
        SetScenarioResult(context, static_cast<std::uint32_t>(value));
    } else if (manager) {
        manager->motion.poseEvaluationStage = static_cast<std::uint8_t>(params[1]);
    }
}

void Script_CewpFind(ScenarioContext *, void *rawParams) {
    const std::uint32_t *const params = ScenarioWords(rawParams);
    auto *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    if (!params || !control)
        return;

    constexpr std::uint32_t kCewp = SHEN_NAME(CEWP);
    for (void *const head : control->characterBuckets) {
        for (auto *node = static_cast<CharacterBucketNode *>(head); node; node = node->next) {
            Character *const character = node->character;
            if (!character || character->entity.name != kCewp)
                continue;
            const std::uint32_t flags = character->flags.value;
            if ((flags & CharacterFlagUnknown) == 0 && ((flags & 2U) == 0 || (character->motionFlags & 1U) == 0)) {
                continue;
            }
            if (character->groupId != static_cast<std::int32_t>(params[0])) {
                continue;
            }
            if (!params[1])
                character->flags.value |= 2U;
            character->flags.value &= ~CharacterFlagUnknown;
            ++control->pendingCharacterRetirements;
        }
    }
}

void Script_SetCCOW_Flags(ScenarioContext *context, void *rawParams) {
    const std::uint32_t *const params = ScenarioWords(rawParams);
    if (!params)
        return;
    CharacterCollisionWork *const control = FindCharacterControlWork(ScenarioCharacter(context, params[0]));
    if (!control)
        return;

    const std::uint32_t flags = params[1];
    if (!flags) {
        control->proximityCooldown = 0;
        control->proximityObject = 0;
        control->proximityDistance = 0.0f;
        control->proximityFlags = 0;
        return;
    }
    control->proximityCooldown = 0;
    if (flags & 0x10U)
        control->proximityFlags |= 0x10U;
    if (flags & 0x100U)
        control->proximityFlags = flags & 0x3fU;
    if (flags & 0x80U)
        control->proximityObject = params[2];
    if (flags & 0x40U)
        control->proximityDistance = std::bit_cast<float>(params[3]);
}

void Script_ccowUnknown(ScenarioContext *context, void *rawParams) {
    const std::uint32_t *const params = ScenarioWords(rawParams);
    if (!params)
        return;
    const std::uint32_t mode = params[1];
    const float value = std::bit_cast<float>(params[2]);
    if (!params[0]) {
        if (mode == 1)
            g_characterGroundDropLimit = value;
        else if (mode == 2)
            g_characterGroundDropLimit = 0.07f;
        SetScenarioResult(context, std::bit_cast<std::uint32_t>(g_characterGroundDropLimit));
        return;
    }

    CharacterCollisionWork *const control = FindCharacterControlWork(ScenarioCharacter(context, params[0]));
    if (control) {
        if (mode == 1)
            control->groundDropLimit = value;
        else if (mode == 2)
            control->groundDropLimit = -68342.0f;
    }
    const float result = control && control->groundDropLimit != -68342.0f ? control->groundDropLimit : g_characterGroundDropLimit;
    SetScenarioResult(context, std::bit_cast<std::uint32_t>(result));
}

void Script_motnUtils(ScenarioContext *context, void *rawParams) {
    const std::uint32_t *const params = ScenarioWords(rawParams);
    if (!params)
        return;
    MBST_t *const budget = MBST_GetWork();
    switch (params[0]) {
    case 0:
        if (budget)
            budget->capacity = static_cast<std::int16_t>(params[1]);
        break;
    case 1: {
        MotionManagerWork *const manager = MOTM_GetWork(ScenarioCharacter(context, params[1]));
        if (!manager)
            return;
        const bool enable = params[2] != 0;
        if (enable) {
            if ((manager->flags & 2U) != 0)
                return;
            manager->flags |= 2U;
            if (budget)
                ++budget->state;
        } else {
            if ((manager->flags & 2U) == 0)
                return;
            if (!manager->motion.activeNodes) {
                MOTI_TransferPrimaryNodes(nullptr, &manager->motion);
                manager->flags &= ~2U;
                return;
            }
            if (budget)
                --budget->state;
            manager->flags &= ~2U;
        }
        MBST_Refresh(true);
        break;
    }
    case 2: {
        const MotionManagerWork *const manager = MOTM_GetWork(ScenarioCharacter(context, params[1]));
        SetScenarioResult(context, manager ? manager->flags & 2U : 0U);
        break;
    }
    default:
        break;
    }
}

} // namespace shenmue::s1
