#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/cloth_dc.hpp"

#include "s2/model_dc.hpp"
#include "s2/osage_dc.hpp"
#include "s2/math.hpp"
#include "s2/pause.hpp"
#include "s2/motion_work_dc.hpp"
#include "platform/dc/matrix.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "s2/flag_wind_dc.hpp"
#include "s2/weather_wind_dc.hpp"
#include "s2/event_vector_dc.hpp"
#include "platform/dc/sdk.hpp"
#include "platform/dc/am2_vector.hpp"
#include "s2/event_matrix_dc.hpp"
#include <cmath>
#include <cstring>
#else
#include "s2/model_component.hpp"
#include "d3t/buffer.hpp"
#include "d3t/dx11_renderer.hpp"
#include "d3t/object_store.hpp"
#include "d3t/model_cache.hpp"

#include "s2/archive_data.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/camera.hpp"
#include "s2/field_collision.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/pak.hpp"
#include "s2/motion_work.hpp"
#include "s2/motion.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/math.hpp"
#include "s2/motion_pose.hpp"
#include "s2/model_clone.hpp"
#include "s2/model_image.hpp"
#include "s2/hand.hpp"
#include "s2/handcuffs.hpp"
#include "s2/face.hpp"
#include "s2/lighting.hpp"
#include "s2/mt7.hpp"
#include "s2/npc_package.hpp"
#include "s2/osage.hpp"
#include "s2/door.hpp"
#include "s2/maze.hpp"
#include "s2/navi.hpp"
#include "s2/game_runtime.hpp"
#include "s2/event_runtime.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/string_utility.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <memory>
#include <new>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include "s2/area_names.hpp"
#include "s2/model_component_internal.hpp"
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

using namespace model_component_detail;

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
namespace model_component_detail {

void UpdateClothBoneCapsules(ClothComponent &cloth, const std::byte *records);

} // namespace model_component_detail

void mtAssignClothFlags(Character *character, std::int8_t assignMode);
void CLMD(Character *character, std::int8_t boneSet);
void clmdUtil(Character *character, std::int8_t mode);
void RestoreClothMeshBindings(Character *character);
void PrepareClothForDraw(Character *character);
ClothComponent *FindCLMD(Character *character);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

namespace {
float clothAnkleOffset = -0.06f;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void UpdateClothBoneCapsules(ClothComponent *cloth, const std::byte *records);
static float clmdComputeMinimumCapsuleRadius(const ClothComponent *cloth);
ClothComponent *clmdGetComponent(Character *character);
void PrepareClothForDraw(Character *character);
void CLMD(Character *character, std::uint8_t boneSet);
void FindCLMD(Character *character);
void RestoreClothMeshBindings(Character *character);
void FreeClothObjects(ClothComponent *cloth);
void FreeUSE_0(Character *, void *payload);
void ResetClothObjects(Character *character);
void UpdateWindReactiveNode(Character *character, void *payload);
void mtAssignClothFlags(Character *character, std::uint8_t mode);
static void clmdCopyColumnLinkNormals(ClothObject *object);
void clmdUtil(Character *character, std::uint8_t mode);
std::int32_t UpdateClothNodeTree(MT7Node *node);

#define CLOTH_VALUE(T, base, offset) (*reinterpret_cast<T *>((std::byte *)(base) + (offset)))

namespace {

Vector3 Vec(float x, float y, float z);
void ClothTranslate(float x, float y, float z);
void ClothBuildRotation(const Vector3 &first, const Vector3 &second);
void clmdBindMeshVertices(const std::byte *descriptor, const void *mesh, ClothVertexBinding *bindings);
void clmdWriteMeshVertices(const std::byte *descriptor, const ClothVertexBinding *bindings, void *mesh);
void clmdTransformBindings(ClothObject &object, ClothVertexBinding *bindings);
void clmdInverseTransformBindings(ClothObject &object, ClothVertexBinding *bindings);
void clmdInitializeGrid(ClothObject &object);
float clmdDistanceSquared(const Vector3 &first, const Vector3 &second);
void clmdInitializePins(ClothObject &object);
std::int32_t clmdTriangleNormal(const Vector3 *first, const Vector3 *second, const Vector3 *third, Vector3 *normal);
void clmdOrientNormals(ClothObject &object);
void clmdUpdateNormals(ClothObject &object);
void clmdClampRestLength(const Vector3 &anchor, const Vector3 &point, float length, Vector3 &output);
void clmdConstrainRow(ClothObject &object, ClothVertexState *end, std::int32_t row);
void clmdIntegrateVertex(ClothObject &object, ClothVertexState &vertex, Vector3 &velocity, const Vector3 &firstForce, const Vector3 &secondForce,
    std::int32_t columnsRemaining, std::int32_t rowsRemaining, Vector3 &point);
void clmdSolveRows(ClothObject &object, const Vector3 &firstForce, const Vector3 &secondForce);
void clmdLiftBelowAnkles(ClothObject &object);
void clmdUpdateDynamics(ClothObject &object);
void clmdUpdateDamping(ClothObject &object, ClothVertexState &vertex, Vector3 &velocity);
void clmdResolveBoneCollision(ClothObject &object, Vector3 &point, ClothVertexState &vertex);
void clmdFindColumnLinks(ClothObject &object);
void clmdUpdateMeshBounds(ClothObject &object);
void clmdCopyLinkedBindings(ClothObject &object);
std::int32_t clmdUpdateModelNode(MT7Node *node);
void clmdPublishSiblingMatrices(MT7Node *node);

} // namespace
#else
namespace model_component_detail {

template <typename Value> Value ReadClothValue(const std::byte *source);
void UpdateClothBoneCapsules(ClothComponent &cloth, const std::byte *records);
std::uint64_t modelGetClothSize(const MT7Model &model, const MT7Node &node);
void CLCB(ClothComponent &cloth, const std::byte *bones);
void clmdBindMeshVertices(const std::byte *descriptor, const std::byte *mesh, ClothVertexBinding *bindings, bool directX);
void clmdWriteMeshVertices(const std::byte *descriptor, const ClothVertexBinding *bindings, std::byte *mesh, bool directX);
float clmdDistanceSquared(const Vector3 &first, const Vector3 &second);
void clmdSelectColumnLink(ClothObject &object, std::byte *vertex, float *scores, double threshold);
void clmdFindColumnLinksMDC7(ClothObject &object);
void clmdFindColumnLinksMDCX(ClothObject &object);
void clmdInitializeGrid(ClothObject &object);
void clmdInitializePins(ClothObject &object);
void clmdUpdateMeshBounds(ClothObject &object);
void clmdTransformBindings(ClothObject &object, ClothVertexBinding *bindings, bool inverse);
Vector3 clmdTriangleNormal(const Vector3 &first, const Vector3 &second, const Vector3 &third);
void clmdOrientNormals(ClothObject &object);
void clmdUpdateNormals(ClothObject &object);
void clmdCopyLinkedBindings(ClothObject &object);
void clmdClampRestLength(const Vector3 &anchor, const Vector3 &point, float length, Vector3 &output);
bool clmdProjectSimpleSphere(Vector3 &point, const Vector3 &center, float squared, float radiusSquared);
void clmdProjectSphere(Vector3 &point, const Vector3 &center, std::uint8_t capsule, ClothVertexState &vertex, float radiusSquared, float aboveDistance,
    float outerDistance, float innerDistance);
void clmdResolveBoneCollision(ClothObject &object, Vector3 &point, ClothVertexState &vertex);
void clmdUpdateDamping(ClothObject &object, ClothVertexState &vertex, Vector3 &velocity);
void clmdIntegrateVertex(ClothObject &object, ClothVertexState &vertex, Vector3 &velocity, const Vector3 &firstForce, const Vector3 &secondForce,
    std::int32_t columnsRemaining, std::int32_t rowsRemaining, Vector3 &point);
void clmdConstrainRow(ClothObject &object, ClothVertexState *end, std::int32_t row);
void clmdSolveRows(ClothObject &object, const Vector3 &firstForce, const Vector3 &secondForce);
void clmdLiftBelowAnkles(ClothObject &object);
void clmdUpdateDynamics(ClothObject &object);
std::int32_t clmdUpdateModelNode(MT7Node *model, MT7Node *node);
void clmdPublishSiblingMatrices(MT7Node *node);
void AllocateCLTH_CLMS_CLID_CLVB_CLVO_CLVP(ClothComponent &cloth, MT7Model &model, MT7Node *root);
void AllocateCLTH_CLMS_CLID_CLVB_CLVO_CLVP_Wrapper(ClothComponent &cloth, MT7Model &model);
void FreeClothTaskData(void *data);
void FreeClothObjects(ClothComponent &cloth);
void FreeUSE_0(Character *, void *payload);
void ResetClothObjects(Character *character);
float ComputeWindOscillationNoise(std::int32_t oscillation);
void UpdateWindReactiveNode(Character *character, void *payload);

} // namespace model_component_detail

std::int64_t RecursivePrepareChildMatrices(MT7Node *model, MT7Node *node);
void PrepareClothForDraw(Character *character);
void CLMD(Character *character, std::int8_t boneSet);
ClothComponent *FindCLMD(Character *character);
void RestoreClothMeshBindings(Character *character);
void mtAssignClothFlags(Character *character, std::int8_t assignMode);
void clmdUtil(Character *character, std::int8_t mode);
void SetClothFlag(Character *character, std::uint8_t flags, std::int32_t enabled);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void mtAssignClothFlags(Character *character, std::uint8_t mode) {
    MT7Node *const model = character->model;
    if (!FindMT7Node(model, -90) && !FindMT7Node(model, -91))
        return;
    std::byte *const source = static_cast<std::byte *>(model->taskData);
    model->taskData = 0;
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const shen_name name = scenGetCharaName(control->scenarioTask, character);
    if (!name || !source || *reinterpret_cast<const std::uint32_t *>(source) != CLOTH_SOURCE)
        return;
    if (0.5199999809265137f > *reinterpret_cast<const float *>(source + 8) || mode == 0xffU)
        return;
    ClothComponent *const cloth =
        static_cast<ClothComponent *>(AllocateModelComponent(character, CLOTH_COMPONENT, sizeof(ClothComponent), UpdateWindReactiveNode));
    cloth->character = name;
    cloth->model = model;
    cloth->token = CLOTH_COMPONENT;
    cloth->assignMode = mode;
    cloth->source = source;
    cloth->parameters = 0;
    cloth->windVelocity.x = cloth->windVelocity.y = cloth->windVelocity.z = 0.0f;
    cloth->windStrength = 0.0f;
    cloth->oscillation = static_cast<std::int32_t>(dc::sdk::RandomFloat() * 65535.0f);
    cloth->oscillationRotation = static_cast<std::int32_t>(dc::sdk::RandomFloat() * 65535.0f);
    cloth->windOffset.x = cloth->windOffset.y = cloth->windOffset.z = 0.0f;
    const std::int32_t offset = *reinterpret_cast<const std::int32_t *>(source + 0x14);
    if (offset) {
        const std::int32_t *const header = reinterpret_cast<const std::int32_t *>(source + offset);
        if (static_cast<std::uint32_t>(header[0]) == CLOTH_PARAMETERS) {
            cloth->parameters = source + header[2];
            cloth->savedParameters[0] = cloth->parameters[0x16];
            cloth->savedParameters[1] = cloth->parameters[0x17];
            cloth->savedParameters[2] = cloth->parameters[0x18];
        }
    }
    AllocateClothObjects(cloth);
    SetModelComponentCleanup(character, CLOTH_COMPONENT, FreeUSE_0);
}

#else

void mtAssignClothFlags(Character *character, std::int8_t assignMode) {
    MT7Model *const sourceModel = static_cast<MT7Model *>(character->model);
    if (!sourceModel)
        return;
    MT7Model &model = *sourceModel;
    MT7Node *const root = &model.nodes.front();
    if (!GetChildObjectByIndex(model, root, 0xffa6U) && !GetChildObjectByIndex(model, root, 0xffa5U))
        return;
    auto *const source = static_cast<std::byte *>(root->taskData);
    root->taskData = nullptr;
    const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const shen_name name = scenGetCharaName(control->scenarioTask, character);
    if (!name.name || !source)
        return;
    const std::uint32_t signature = ReadClothValue<std::uint32_t>(source);
    if (signature != SHEN_NAME(CLSX) && signature != SHEN_NAME(CLSG))
        return;
    if (0.52f > ReadClothValue<float>(source + 8) || assignMode == -1)
        return;
    auto *const cloth = static_cast<ClothComponent *>(AllocateModelComponent(character, kClothToken, sizeof(ClothComponent), UpdateWindReactiveNode));
    if (!cloth)
        return;
    cloth->model = root;
    cloth->parameters = nullptr;
    cloth->windOffset = {};
    cloth->windStrength = 0.0f;
    cloth->field9c = 0.0f;
    cloth->name = name;
    cloth->token = kClothToken;
    cloth->assignMode = assignMode;
    cloth->source = source;
    cloth->oscillation = static_cast<std::int32_t>(IncrementSharedPrngFloat() * 65535.0f);
    const float rotation = IncrementSharedPrngFloat();
    cloth->field8c = 0.0f;
    cloth->field90 = 0;
    cloth->field88 = 0;
    cloth->field98 = 0.7f;
    cloth->oscillationRotation = static_cast<std::int32_t>(rotation * 65535.0f);
    if (const std::int32_t parametersOffset = ReadClothValue<std::int32_t>(source + 20)) {
        const std::uint32_t parametersSignature = ReadClothValue<std::uint32_t>(source + parametersOffset);
        if (parametersSignature == SHEN_NAME(CLCX) || parametersSignature == SHEN_NAME(CLCL)) {
            cloth->parameters = source + ReadClothValue<std::int32_t>(source + parametersOffset + 8);
            cloth->savedParameters[0] = static_cast<std::uint8_t>(cloth->parameters[0x16]);
            cloth->savedParameters[1] = static_cast<std::uint8_t>(cloth->parameters[0x17]);
            cloth->savedParameters[2] = static_cast<std::uint8_t>(cloth->parameters[0x18]);
        }
    }
    AllocateCLTH_CLMS_CLID_CLVB_CLVO_CLVP_Wrapper(*cloth, model);
    SetModelComponentCleanup(character, kClothToken, FreeUSE_0);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

struct ClothCapsuleSource {
    std::int16_t x, y, z, radius;
    std::uint8_t bone, group;
};

static const ClothCapsuleSource clothBonesMode1[] = {
    {-10, -45, 15, 146, 23, 10},
    {150, 0, -10, 130, 23, 10},
    {300, 0, 5, 120, 23, 10},
    {-10, -45, -15, 146, 28, 13},
    {150, 0, 10, 130, 28, 13},
    {300, 0, -5, 120, 28, 13},
    {45, -15, -15, 135, 24, 11},
    {165, -20, -10, 134, 24, 11},
    {310, -25, 0, 135, 24, 11},
    {55, 0, -5, 135, 29, 14},
    {185, -15, -5, 134, 29, 14},
    {310, -25, -20, 135, 29, 14},
    {0, 0, 0, 0, 44, 255},
};

static const ClothCapsuleSource clothBonesMode5[] = {
    {5, -145, 35, 140, 22, 9},
    {5, -145, -35, 140, 22, 9},
    {65, 0, -25, 140, 23, 10},
    {135, -25, -25, 140, 23, 10},
    {200, -35, 5, 140, 23, 10},
    {280, -35, -15, 138, 23, 10},
    {235, -45, -5, 140, 23, 10},
    {30, -25, -30, 140, 24, 11},
    {140, -35, -20, 152, 24, 11},
    {275, -45, -5, 160, 24, 11},
    {385, -70, -30, 190, 24, 11},
    {65, 0, 25, 140, 28, 13},
    {135, -25, 25, 140, 28, 13},
    {200, -35, -5, 140, 28, 13},
    {280, -35, 15, 138, 28, 13},
    {235, -45, 5, 140, 28, 13},
    {30, -20, 15, 140, 29, 14},
    {140, -35, 20, 152, 29, 14},
    {275, -45, 5, 160, 29, 14},
    {385, -70, 30, 190, 29, 14},
    {0, 0, 0, 0, 44, 255},
};

static const ClothCapsuleSource clothBonesMode8[] = {
    {5, -145, 35, 140, 22, 9},
    {5, -145, -35, 140, 22, 9},
    {65, 0, -20, 140, 23, 10},
    {135, 0, -25, 140, 23, 10},
    {200, 0, -5, 140, 23, 10},
    {300, 0, -15, 140, 23, 10},
    {260, 0, -5, 140, 23, 10},
    {105, 40, -15, 140, 24, 11},
    {140, 0, -20, 140, 24, 11},
    {325, 0, -40, 150, 24, 11},
    {470, 125, 0, 170, 24, 11},
    {65, 0, 20, 140, 28, 13},
    {135, 0, 25, 140, 28, 13},
    {200, 0, 5, 140, 28, 13},
    {300, 0, 15, 140, 28, 13},
    {260, 0, 5, 140, 28, 13},
    {105, 40, 15, 140, 29, 14},
    {140, 0, 20, 140, 29, 14},
    {325, 0, 40, 150, 29, 14},
    {540, 100, 55, 214, 29, 14},
    {0, 0, 0, 0, 44, 255},
};

static const ClothCapsuleSource clothBonesMode10[] = {
    {5, -145, 35, 140, 22, 9},
    {5, -145, -35, 140, 22, 9},
    {65, 0, -25, 140, 23, 10},
    {135, -25, -25, 140, 23, 10},
    {175, 0, -10, 140, 23, 10},
    {315, 0, -5, 150, 23, 10},
    {235, 0, -5, 140, 23, 10},
    {30, 0, 0, 150, 24, 11},
    {100, 0, -20, 155, 24, 11},
    {210, 0, -15, 160, 24, 11},
    {385, 30, -30, 190, 24, 11},
    {65, 0, 25, 140, 28, 13},
    {135, -25, 25, 140, 28, 13},
    {175, 0, 10, 140, 28, 13},
    {315, 0, 5, 150, 28, 13},
    {235, 0, 5, 140, 28, 13},
    {30, 0, 0, 150, 29, 14},
    {100, 0, 20, 155, 29, 14},
    {210, 0, 15, 160, 29, 14},
    {385, 30, 30, 190, 29, 14},
    {0, 0, 0, 0, 44, 255},
};

static const ClothCapsuleSource clothBonesMode14[] = {
    {5, -145, 35, 140, 22, 9},
    {5, -145, -35, 140, 22, 9},
    {65, 0, -45, 140, 23, 10},
    {135, 0, -45, 140, 23, 10},
    {200, 0, -45, 140, 23, 10},
    {300, 0, -45, 140, 23, 10},
    {260, 0, -50, 140, 23, 10},
    {75, 0, -45, 140, 24, 11},
    {100, -20, -45, 140, 24, 11},
    {265, 0, -40, 150, 24, 11},
    {410, 40, -40, 194, 24, 11},
    {65, 0, 45, 140, 28, 13},
    {135, 0, 45, 140, 28, 13},
    {200, 0, 45, 140, 28, 13},
    {300, 0, 45, 140, 28, 13},
    {260, 0, 50, 140, 28, 13},
    {75, 0, 45, 140, 29, 14},
    {100, -20, 45, 140, 29, 14},
    {265, 0, 40, 150, 29, 14},
    {410, 40, 40, 194, 29, 14},
    {0, 0, 0, 0, 44, 255},
};

static const ClothCapsuleSource clothBonesMode15[] = {
    {55, 5, 10, 150, 23, 10},
    {220, -40, 0, 130, 23, 10},
    {325, -30, 0, 120, 23, 10},
    {60, -5, 0, 150, 28, 13},
    {220, -40, 10, 130, 28, 13},
    {325, -30, 0, 120, 28, 13},
    {50, 0, -5, 140, 24, 11},
    {200, 0, 0, 160, 24, 11},
    {350, 0, 0, 160, 24, 11},
    {120, 0, -5, 140, 29, 14},
    {200, 0, 0, 160, 29, 14},
    {350, 0, 0, 160, 29, 14},
    {0, 0, 0, 0, 44, 255},
};

static const ClothCapsuleSource clothBonesMode17[] = {
    {5, -145, 35, 140, 22, 9},
    {5, -145, -35, 140, 22, 9},
    {65, 0, -25, 140, 23, 10},
    {135, 0, -25, 140, 23, 10},
    {175, 0, -5, 140, 23, 10},
    {255, 0, -15, 140, 23, 10},
    {355, 0, -5, 140, 23, 10},
    {95, -5, -5, 140, 24, 11},
    {215, -25, -15, 150, 24, 11},
    {355, 5, 0, 170, 24, 11},
    {585, 20, 0, 170, 24, 11},
    {65, 0, 25, 140, 28, 13},
    {135, 0, 25, 140, 28, 13},
    {175, 0, 5, 140, 28, 13},
    {255, 0, 15, 140, 28, 13},
    {355, 0, 5, 140, 28, 13},
    {95, -5, 5, 140, 29, 14},
    {215, -25, 15, 150, 29, 14},
    {355, 5, 0, 170, 29, 14},
    {585, 20, 0, 170, 29, 14},
    {0, 0, 0, 0, 44, 255},
};

static const ClothCapsuleSource clothBonesMode20[] = {
    {5, -145, 35, 140, 22, 9},
    {5, -145, -35, 140, 22, 9},
    {65, 0, -25, 140, 23, 10},
    {135, 0, -25, 140, 23, 10},
    {200, 0, -5, 140, 23, 10},
    {300, 0, -15, 140, 23, 10},
    {260, 0, -5, 140, 23, 10},
    {35, -10, -15, 160, 24, 11},
    {100, 0, -20, 170, 24, 11},
    {265, 0, -10, 170, 24, 11},
    {450, 0, -10, 170, 24, 11},
    {65, 0, 25, 140, 28, 13},
    {135, 0, 25, 140, 28, 13},
    {200, 0, 5, 140, 28, 13},
    {300, 0, 15, 140, 28, 13},
    {260, 0, 5, 140, 28, 13},
    {35, -10, 15, 160, 29, 14},
    {100, 0, 20, 170, 29, 14},
    {265, 0, 10, 170, 29, 14},
    {450, 0, 10, 170, 29, 14},
    {0, 0, 0, 0, 44, 255},
};

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

Vector3 Vec(float x, float y, float z) {
    Vector3 result = {x, y, z};
    return result;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

Vector3 clothVelocity, clothLeftConstraint, clothRightConstraint;
std::int32_t clothOscillationStep = 30;
std::int32_t clothOscillationRotationStep = 4537;
std::int32_t clothWavePhaseStep = 14112;

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void ClothTranslate(float x, float y, float z) {
    const Vector3 value = {x, y, z};
    matrix_stack::TranslateVector(&value);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void ClothBuildRotation(const Vector3 &first, const Vector3 &second) {
    Matrix4x4_BuildRotationMatrix(first, second);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void clmdUpdateDamping(ClothObject &object, ClothVertexState &vertex, Vector3 &velocity);

void clmdResolveBoneCollision(ClothObject &object, Vector3 &point, ClothVertexState &vertex);

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void clmdBindMeshVertices(const std::byte *descriptor, const void *mesh, ClothVertexBinding *bindings) {
    const std::int32_t count = CLOTH_VALUE(const std::int32_t, descriptor, 0);
    const std::byte *record = descriptor + 4;
    std::int32_t previous = -1;
    for (std::int32_t index = 0; index < count; ++index, record += 8) {
        const std::int32_t vertex = CLOTH_VALUE(const std::int32_t, record, 0);
        if (vertex == previous)
            ++bindings->flags;
        else {
            previous = vertex;
            bindings->sourceOffset = static_cast<std::uint16_t>(vertex >> 2);
            std::memcpy(&bindings->position, static_cast<const std::byte *>(mesh) + CLOTH_VALUE(const std::int32_t, record, 4), 24);
            ++bindings;
        }
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void clmdWriteMeshVertices(const std::byte *descriptor, const ClothVertexBinding *bindings, void *mesh) {
    const std::int32_t count = CLOTH_VALUE(const std::int32_t, descriptor, 0);
    const std::byte *record = descriptor + 4;
    for (std::int32_t index = 0; index < count; ++index, record += 8) {
        std::byte *output = static_cast<std::byte *>(mesh) + CLOTH_VALUE(const std::int32_t, record, 4);
        std::uint32_t positionX;
        std::memcpy(&positionX, &bindings->position.x, 4);
        positionX |= 1U;
        std::memcpy(output, &positionX, 4);
        std::memcpy(output + 4, &bindings->position.y, 20);
        if (CLOTH_VALUE(const std::int32_t, record, 0) != CLOTH_VALUE(const std::int32_t, record, 8))
            ++bindings;
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void clmdTransformBindings(ClothObject &object, ClothVertexBinding *bindings) {
    matrix_stack::Matrix4x4_Push();
    matrix_stack::LoadMatrix(&object.transform);
    for (std::int32_t i = 0; i < object.vertexCount; ++i) {
        Matrix4x4::TransformPoint(bindings[i].position, bindings[i].position);
        Matrix4x4::TransformPoint(bindings[i].normalEndpoint, bindings[i].normalEndpoint);
    }
    PopCurrentMatrix(1);
}

void clmdInverseTransformBindings(ClothObject &object, ClothVertexBinding *bindings) {
    matrix_stack::Matrix4x4_Push();
    matrix_stack::LoadMatrix(&object.transform);
    Matrix4x4::InvertRigidTransform();
    for (std::int32_t i = 0; i < object.vertexCount; ++i) {
        Matrix4x4::TransformPoint(bindings[i].position, bindings[i].position);
        Matrix4x4::TransformPoint(bindings[i].normalEndpoint, bindings[i].normalEndpoint);
    }
    PopCurrentMatrix(1);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void FindCLMD(Character *character) {
    ClothComponent *const cloth = static_cast<ClothComponent *>(charFindComp(character, CLOTH_COMPONENT));
    if (cloth) {
        if (cloth->mode & 0x40U)
            cloth->mode &= 0xffbfU;
        cloth->mode |= 0x20U;
        cloth->resetCountdown = 2;
    }
}

#else

// @verified - p
ClothComponent *FindCLMD(Character *character) {
    auto *const cloth = static_cast<ClothComponent *>(FindModelComponent(character, kClothToken));
    if (cloth) {
        if ((cloth->mode & 0x40U) != 0)
            cloth->mode &= static_cast<std::uint16_t>(~0x40U);
        cloth->mode |= 0x20U;
        cloth->resetCountdown = 2;
    }
    return cloth;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void ResetClothObjects(Character *character) {
    MT7Node *const model = character->model;
    charFindComp(character, CLOTH_COMPONENT);
    if (!FindMT7Node(model, -90) && !FindMT7Node(model, -91))
        return;
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const shen_name name = scenGetCharaName(control->scenarioTask, character);
    if (!name)
        return;
    ClothComponent *cloth = static_cast<ClothComponent *>(charFindComp(character, CLOTH_COMPONENT));
    if (!cloth)
        cloth = static_cast<ClothComponent *>(AllocateModelComponent(character, CLOTH_COMPONENT, sizeof(ClothComponent), UpdateWindReactiveNode));
    FreeUSE_0(character, cloth);
    cloth->character = name;
    cloth->model = model;
    AllocateClothObjects(cloth);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void UpdateWindReactiveNode(Character *character, void *payload) {
    ClothComponent *const cloth = static_cast<ClothComponent *>(payload);
    std::uint8_t index;
    if (!(cloth->mode & 4U) && ((cloth->mode ^ cloth->previousMode) & 4U))
        cloth->mode |= 0x10U;
    if (!(cloth->mode & 2U) && ((cloth->mode ^ cloth->previousMode) & 2U))
        cloth->mode |= 0x10U;
    for (index = 0; index < 4; index++) {
        if (cloth->cloths[index]) {
            cloth->cloths[index]->flags &= 0xffdf;
            if (cloth->mode & 0x10U)
                cloth->cloths[index]->flags |= 0x40U;
        }
    }
    if (cloth->mode & 0x10U)
        cloth->mode &= 0xffef;
    if (cloth->mode & 0x20U) {
        if (!--cloth->resetCountdown) {
            ResetClothObjects(character);
            cloth->mode &= 0xffdf;
        } else {
            for (index = 0; index < 4; index++) {
                if (cloth->cloths[index])
                    cloth->cloths[index]->flags |= 0x40U;
            }
        }
    }
    if (static_cast<std::uint8_t>(cloth->parameters[0x19]) != 1U) {
        float target;
        if (cloth->assignMode == 2) {
            Character *const source = ctrlGetScenarioChara(CLOTH_WIND_SOURCE_CHARACTER);
            if (source) {
                Vector3 location, direction;
                GetCharacterWorldPosition(source, &location);
                GetCharacterWorldPosition(character, &direction);
                am2Vector3SubtractInPlace(&direction, &location);
                am2Vector3NormalizeInPlace(&direction);
                target = 0.08f;
                cloth->windStrength = (target - cloth->windStrength) * 0.5f + cloth->windStrength;
                cloth->windVelocity.x = direction.x * cloth->windStrength;
                cloth->windVelocity.z = direction.z * cloth->windStrength;
                cloth->oscillation += clothOscillationStep;
                cloth->oscillationRotation += clothOscillationRotationStep;
            }
        } else {
            float speed = GetEffectiveWindSpeed(GetWind());
            if (speed > 0.08f)
                speed = 0.08f;
            const float wave = Noise(cloth->oscillation);
            target = speed * 0.5f;
            target = target * wave + target;
            cloth->windStrength = (target - cloth->windStrength) * 0.5f + cloth->windStrength;
            const std::int32_t direction = GetCurrentWindPhaseAccumulator(GetWind());
            cloth->windVelocity.x = cloth->windStrength * dc::sdk::SinFixed(direction);
            cloth->windVelocity.z = cloth->windStrength * dc::sdk::CosFixed(direction);
            cloth->oscillation += clothOscillationStep;
            cloth->oscillationRotation += clothOscillationRotationStep;
        }
    }
    cloth->updatedClothCount = 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void FreeUSE_0(Character *, void *payload) {
    ClothComponent *const cloth = static_cast<ClothComponent *>(payload);
    if (cloth->boneBuffer) {
        HLib::ClearTaskDataMark(cloth->boneBuffer);
        HLib::FreeTaskData(cloth->boneBuffer);
        cloth->boneBuffer = 0;
    }
    FreeClothObjects(cloth);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void RestoreClothMeshBindings(Character *character) {
    ClothComponent *const cloth = static_cast<ClothComponent *>(charFindComp(character, CLOTH_COMPONENT));
    if (!cloth)
        return;
    for (std::int32_t rangeIndex36 = 0; rangeIndex36 < static_cast<std::int32_t>(sizeof(cloth->cloths) / sizeof((cloth->cloths)[0])); ++rangeIndex36) {
        ClothObject *object = (cloth->cloths)[rangeIndex36];

        if (object) {
            object->node->mesh = object->sourceMesh;
            if (object->link)
                object->link->node->mesh = object->link->sourceMesh;
        }
    }
    cloth->mode |= 0x40U;
}

#else

// @verified - p
void RestoreClothMeshBindings(Character *character) {
    ClothComponent *const cloth = static_cast<ClothComponent *>(FindModelComponent(character, kClothToken));
    if (!cloth)
        return;
    for (ClothObject *object : cloth->cloths) {
        if (!object)
            continue;
        object->node->runtimeMesh = object->sourceMesh;
        if (object->link)
            object->link->node->runtimeMesh = object->link->sourceMesh;
    }
    cloth->mode |= 0x40U;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void FreeClothObjects(ClothComponent *cloth) {
    for (std::uint8_t index = 0; index < 4; ++index) {
        ClothObject *const object = cloth->cloths[index];
        if (!object)
            continue;
        if (object->columnLinks) {
            for (std::uint8_t column = 0; column < object->columns; ++column)
                object->columnLinks[column] = 0;
            HLib::ClearTaskDataMark(object->columnLinks);
            HLib::FreeTaskData(object->columnLinks);
            object->columnLinks = 0;
        }
        if (object->mesh) {
            HLib::ClearTaskDataMark(object->mesh);
            HLib::FreeTaskData(object->mesh);
            object->mesh = 0;
        }
        if (object->vertices) {
            HLib::ClearTaskDataMark(object->vertices);
            HLib::FreeTaskData(object->vertices);
            object->vertices = 0;
        }
        if (object->bindings) {
            HLib::ClearTaskDataMark(object->bindings);
            HLib::FreeTaskData(object->bindings);
            object->bindings = 0;
        }
        if (object->previousBindings) {
            HLib::ClearTaskDataMark(object->previousBindings);
            HLib::FreeTaskData(object->previousBindings);
            // Native clears the current slot again after retiring the previous buffer.
            object->bindings = 0;
        }
        if (object->link) {
            if (object->link->bindings) {
                HLib::ClearTaskDataMark(object->link->bindings);
                HLib::FreeTaskData(object->link->bindings);
                object->link->bindings = 0;
            }
            if (object->link->mesh) {
                HLib::ClearTaskDataMark(object->link->mesh);
                HLib::FreeTaskData(object->link->mesh);
                object->link->mesh = 0;
            }
            HLib::ClearTaskDataMark(object->link);
            HLib::FreeTaskData(object->link);
            object->link = 0;
        }
        object->node->taskData = 0;
        HLib::ClearTaskDataMark(object);
        HLib::FreeTaskData(object);
        cloth->cloths[index] = 0;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void CLMD(Character *character, std::uint8_t boneSet) {
    if (!character->model)
        return;
    ClothComponent *const cloth = static_cast<ClothComponent *>(charFindComp(character, CLOTH_COMPONENT));
    if (!cloth)
        return;
    const std::int32_t saved = reinterpret_cast<std::int32_t *>(cloth->parameters)[1];
    switch (boneSet) {
    case 0:
        break;
    case 1:
        reinterpret_cast<std::int32_t *>(cloth->parameters)[1] = reinterpret_cast<std::int32_t *>(cloth->parameters)[2];
        break;
    case 2:
        reinterpret_cast<std::int32_t *>(cloth->parameters)[1] = reinterpret_cast<std::int32_t *>(cloth->parameters)[3];
        break;
    case 3:
        reinterpret_cast<std::int32_t *>(cloth->parameters)[1] = reinterpret_cast<std::int32_t *>(cloth->parameters)[4];
        break;
    default:
        return;
    }
    if (!reinterpret_cast<std::int32_t *>(cloth->parameters)[1]) {
        reinterpret_cast<std::int32_t *>(cloth->parameters)[1] = saved;
    } else {
        SetupClothBones(cloth);
        reinterpret_cast<std::int32_t *>(cloth->parameters)[1] = saved;
        for (std::uint8_t index = 0; index < cloth->clothCount; ++index) {
            cloth->cloths[index]->boneRecordCount = cloth->boneRecordCount;
            cloth->cloths[index]->boneBuffer = cloth->boneBuffer;
        }
    }
}

#else

// @verified - p
void CLMD(Character *character, std::int8_t boneSet) {
    if (!character->model)
        return;
    auto *const cloth = static_cast<ClothComponent *>(FindModelComponent(character, kClothToken));
    if (!cloth)
        return;
    const std::int32_t saved = ReadClothValue<std::int32_t>(cloth->parameters + 4);
    if (boneSet != 0) {
        if (boneSet < 1 || boneSet > 3)
            return;
        std::memcpy(cloth->parameters + 4, cloth->parameters + 4 + 4 * boneSet, sizeof(std::int32_t));
    }
    const std::int32_t offset = ReadClothValue<std::int32_t>(cloth->parameters + 4);
    if (offset != 0) {
        CLCB(*cloth, cloth->source + offset);
        std::memcpy(cloth->parameters + 4, &saved, sizeof(saved));
        for (std::uint8_t index = 0; index < cloth->clothCount; ++index) {
            cloth->cloths[index]->boneRecordCount = cloth->boneRecordCount;
            cloth->cloths[index]->boneBuffer = cloth->boneBuffer;
        }
    } else {
        std::memcpy(cloth->parameters + 4, &saved, sizeof(saved));
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t UpdateClothNodeTree(MT7Node *node) {
    ClothObject *const object = static_cast<ClothObject *>(node->taskData);
    std::int32_t update = 1;
    if (!object || object->token != MEMORY_CLOTH_OBJECT || !object->owner)
        update = 0;
    else if ((object->flags & 0x20U) || (object->owner->mode & 0x20U))
        update = 0;
    matrix_stack::Matrix4x4_Push();
    if (update) {
        clmdUpdateModelNode(node);
        object->owner->previousMode = static_cast<std::uint8_t>(object->owner->mode);
    }
    if (node->transformOverride)
        matrix_stack::LoadMatrix(node->transformOverride);
    else {
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        if (node->rotation.z)
            Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
        if (node->rotation.y)
            Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
        if (node->rotation.x)
            Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
    }
    if (node->child) {
        clmdPublishSiblingMatrices(node->child);
        UpdateClothNodeTree(node->child);
    }
    PopCurrentMatrix(1);
    if (node->sibling)
        UpdateClothNodeTree(node->sibling);
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

std::int32_t clmdUpdateModelNode(MT7Node *node) {
    ClothObject *const object = static_cast<ClothObject *>(node->taskData);
    if (!object)
        return 0;
    if (shenIsGamePaused() || (object->owner->mode & 4U))
        return 1;
    if (object->owner->mode & 2U) {
        node->mesh = object->sourceMesh;
        if (object->link)
            object->link->node->mesh = object->link->sourceMesh;
        return 0;
    }
    clmdBindMeshVertices(object->descriptorData28, object->sourceMesh, object->bindings);
    if (object->flags & 1U) {
        object->flags &= static_cast<std::uint16_t>(~1U);
        object->boneBuffer = object->owner->boneBuffer;
        object->owner->minimumCapsuleRadius = clmdComputeMinimumCapsuleRadius(object->owner);
        node->mesh = object->sourceMesh;
        if (object->link)
            object->link->node->mesh = object->link->sourceMesh;
        clmdFindColumnLinks(*object);
        clmdInitializeGrid(*object);
        clmdInitializePins(*object);
        clmdUpdateMeshBounds(*object);
        clmdTransformBindings(*object, object->bindings);
        clmdOrientNormals(*object);
        std::memcpy(object->previousBindings, object->bindings, sizeof(ClothVertexBinding) * object->vertexCount);
        return 0;
    }
    clmdTransformBindings(*object, object->bindings);
    clmdUpdateDynamics(*object);
    clmdUpdateNormals(*object);
    if (object->owner->assignMode == 1)
        clmdLiftBelowAnkles(*object);
    std::memcpy(object->previousBindings, object->bindings, sizeof(ClothVertexBinding) * object->vertexCount);
    clmdInverseTransformBindings(*object, object->bindings);
    clmdCopyColumnLinkNormals(object);
    clmdWriteMeshVertices(object->descriptorData28, object->bindings, object->mesh);
    node->mesh = object->mesh;
    if (object->link) {
        clmdCopyLinkedBindings(*object);
        clmdWriteMeshVertices(object->link->descriptorData, object->link->bindings, object->link->mesh);
        object->link->node->mesh = object->link->mesh;
    }
    object->flags |= 0x20U;
    ++object->owner->updatedClothCount;
    return 1;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void PrepareClothForDraw(Character *character) {
    MotionWork *const work = charFindMotionwork(character);
    if (!work)
        return;
    ClothComponent *const cloth = static_cast<ClothComponent *>(charFindComp(character, CLOTH_COMPONENT));
    if (!cloth)
        return;
    cloth->owner = character;
    cloth->motionWork = work;
    UpdateClothBoneCapsules(cloth, cloth->boneRecords);
}

#else

// @verified - p
void PrepareClothForDraw(Character *character) {
    MotionWork *const motionWork = FindMotionWork(character);
    if (!motionWork)
        return;
    auto *const cloth = static_cast<ClothComponent *>(FindModelComponent(character, kClothToken));
    if (!cloth)
        return;
    const std::byte *const records = cloth->boneRecords;
    cloth->owner = character;
    cloth->motionWork = motionWork;
    UpdateClothBoneCapsules(*cloth, records);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void UpdateClothBoneCapsules(ClothComponent *cloth, const std::byte *records) {
    ClothBoneCapsule *capsule = static_cast<ClothBoneCapsule *>(cloth->boneBuffer);
    for (std::uint8_t index = 0; index < cloth->boneRecordCount; ++index, records += 10, ++capsule) {
        matrix_stack::Matrix4x4_Push();
        matrix_stack::LoadMatrix(mtwkGetBoneTransform(cloth->motionWork, static_cast<std::uint8_t>(records[8])));
        Vector3 position;
        position.x = static_cast<float>(*reinterpret_cast<const std::int16_t *>(records)) * 0.001f;
        position.y = static_cast<float>(*reinterpret_cast<const std::int16_t *>(records + 2)) * 0.001f;
        position.z = static_cast<float>(*reinterpret_cast<const std::int16_t *>(records + 4)) * 0.001f;
        Matrix4x4::TransformPoint(position, capsule->position);
        capsule->radius = static_cast<float>(*reinterpret_cast<const std::int16_t *>(records + 6)) * 0.001f;
        capsule->boneId = static_cast<std::uint8_t>(records[8]);
        const std::uint8_t group = static_cast<std::uint8_t>(records[9]);
        if (group != 0xffU)
            capsule->groupMask = static_cast<std::uint16_t>(1 << group);
        else
            capsule->groupMask = 0xffffU;
        PopCurrentMatrix(1);
    }
}

static float clmdComputeMinimumCapsuleRadius(const ClothComponent *cloth) {
    float minimum = 10.0f;
    for (std::uint8_t index = 0; index < cloth->boneRecordCount; ++index) {
        const float radius = static_cast<ClothBoneCapsule *>(cloth->boneBuffer)[index].radius;
        if (radius < minimum)
            minimum = radius;
    }
    return minimum;
}

ClothComponent *clmdGetComponent(Character *character) {
    if (!character->model)
        return 0;
    ClothComponent *const cloth = static_cast<ClothComponent *>(charFindComp(character, CLOTH_COMPONENT));
    if (!cloth)
        return 0;
    return cloth;
}

#else

namespace model_component_detail {

template <typename Value> Value ReadClothValue(const std::byte *source) {
    Value value{};
    std::memcpy(&value, source, sizeof(value));
    return value;
}

// Native UpdateClothBoneCapsules: places each CLCB capsule on its motion bone. Records hold the bone-space
// centre and radius in millimetres, the bone id at +8 and the capsule group at +9.
// @verified - p
void UpdateClothBoneCapsules(ClothComponent &cloth, const std::byte *records) {
    const ClothProfile *profile = clothProfiles.data();
    while (profile->name.name != make_shen_name('@', '@', '@', '@') && profile->name.name != cloth.name.name)
        ++profile;
    ClothBoneCapsule *capsule = cloth.boneBuffer;
    for (std::uint32_t index = 0; index < cloth.boneRecordCount; ++index, records += 10, ++capsule) {
        const std::uint8_t boneId = static_cast<std::uint8_t>(records[8]);
        const std::uint8_t group = static_cast<std::uint8_t>(records[9]);
        Matrix4x4_Push();
        Matrix4x4::SetActiveTransform(mtwkGetBoneTransform(cloth.motionWork, boneId));
        const Vector3 centre{static_cast<float>(ReadClothValue<std::int16_t>(records)) * 0.001f,
            static_cast<float>(ReadClothValue<std::int16_t>(records + 2)) * 0.001f, static_cast<float>(ReadClothValue<std::int16_t>(records + 4)) * 0.001f};
        Matrix4x4::TransformPoint(centre, capsule->position);
        capsule->radius = static_cast<float>(ReadClothValue<std::int16_t>(records + 6)) * 0.001f;
        capsule->radius += *(profile->groupRadius.data() + group);
        capsule->boneId = boneId;
        capsule->groupMask = group == 0xffU ? std::int16_t{-1} : static_cast<std::int16_t>(1U << group);
        Matrix4x4::Pop(1);
    }
}

// Native modelGetClothSize: the bytes of a cloth node's mesh that CLMS copies.
// @verified - p
std::uint64_t modelGetClothSize(const MT7Model &model, const MT7Node &node) {
    if (node.meshType == SHEN_NAME(MDCX)) {
        const std::byte *const mesh = node.runtimeMesh ? node.runtimeMesh : model.source.data() + node.xb01Offset;
        const std::int64_t command = 4LL * ReadClothValue<std::int32_t>(mesh + 0x1c) + 0x1c;
        const std::uint32_t count = ReadClothValue<std::uint32_t>(mesh + command + 4);
        std::int64_t size = command + 8 + 8LL * count;
        const std::byte *record = mesh + command + 0x0c;
        for (std::uint32_t index = 0; index < count; ++index, record += 0x0c)
            size += ReadClothValue<std::uint32_t>(record);
        return static_cast<std::uint32_t>(size + 8);
    }
    if (node.meshType == SHEN_NAME(MDC7)) {
        const std::byte *const mesh = node.runtimeMesh ? node.runtimeMesh : model.source.data() + node.xb01Offset;
        const std::byte *cursor = mesh + 0x68;
        for (;;) {
            const std::uint32_t flags = ReadClothValue<std::uint32_t>(cursor);
            std::uint32_t strips = ReadClothValue<std::uint32_t>(cursor + 4);
            cursor += 8;
            if ((flags & 8U) != 0)
                strips *= 3;
            for (std::uint32_t index = strips; index != 0; --index)
                cursor += (ReadClothValue<std::uint32_t>(cursor) & 1U) != 0 ? 0x20 : 8;
            const std::int32_t next = ReadClothValue<std::int32_t>(cursor);
            if (next == 0)
                break;
            if (next < 0)
                cursor += 0x50;
        }
        return static_cast<std::uint32_t>(cursor - mesh + 8);
    }
    return 0xffffffffULL;
}

// Native CLCB: counts the bone records and gives the component and its cloths a fresh CLCB buffer.
// @verified - p
void CLCB(ClothComponent &cloth, const std::byte *bones) {
    cloth.boneRecords = bones;
    cloth.boneRecordCount = 0;
    const std::byte *record = bones + 9;
    do {
        ++cloth.boneRecordCount;
        record += 10;
    } while (static_cast<std::uint8_t>(*record) != 0xffU);
    if (cloth.boneBuffer) {
        HLib::ClearTaskDataMark(cloth.boneBuffer);
        HLib::FreeTaskData(cloth.boneBuffer);
    }
    cloth.boneBuffer = static_cast<ClothBoneCapsule *>(HLib::AllocateTaskData(sizeof(ClothBoneCapsule) * cloth.boneRecordCount, SHEN_NAME(CLCB)));
    HLib::AssignDynamicAlignmentForTaskData(cloth.boneBuffer);
    for (std::uint8_t index = 0; index < cloth.clothCount; ++index) {
        cloth.cloths[index]->boneRecordCount = cloth.boneRecordCount;
        cloth.cloths[index]->boneBuffer = cloth.boneBuffer;
    }
}

// @verified - p
void clmdBindMeshVertices(const std::byte *descriptor, const std::byte *mesh, ClothVertexBinding *bindings, bool directX) {
    const std::int32_t count = ReadClothValue<std::int32_t>(descriptor);
    const std::byte *record = descriptor + 4;
    if (directX)
        mesh += 4LL * ReadClothValue<std::int32_t>(mesh + 28) + 44;
    std::int32_t previous = -1;
    for (std::int32_t index = 0; index < count; ++index, record += 8) {
        const std::int32_t vertex = ReadClothValue<std::int32_t>(record);
        if (vertex == previous)
            ++bindings->field02;
        else {
            previous = vertex;
            bindings->index = static_cast<std::uint16_t>(vertex >> 2);
            const std::byte *const source = mesh + ReadClothValue<std::int32_t>(record + 4);
            std::memcpy(&bindings->position, source, 24);
            ++bindings;
        }
    }
}

// @verified - p
void clmdWriteMeshVertices(const std::byte *descriptor, const ClothVertexBinding *bindings, std::byte *mesh, bool directX) {
    std::byte *const destination = directX ? static_cast<std::byte *>(CopyForDirectX(mesh, 0, 0, 0, 0)) : mesh;
    const std::int32_t count = ReadClothValue<std::int32_t>(descriptor);
    const std::byte *record = descriptor + 4;
    std::uint32_t vertex = ReadClothValue<std::uint32_t>(record);
    for (std::int32_t index = 0; index < count; ++index) {
        const std::byte *const offset = record + 4;
        record += 8;
        std::byte *const output = destination + ReadClothValue<std::int32_t>(offset);
        std::uint32_t positionX;
        std::memcpy(&positionX, &bindings->position.x, 4);
        positionX |= 1U;
        std::memcpy(output, &positionX, 4);
        std::memcpy(output + 4, &bindings->position.y, 20);
        const std::uint32_t next = ReadClothValue<std::uint32_t>(record);
        if (vertex != next)
            ++bindings;
        vertex = next;
    }
    if (directX)
        CopyBufferToDirectX_Wrapper(mesh, 0);
}

float clmdDistanceSquared(const Vector3 &first, const Vector3 &second) {
    const float y = first.y - second.y, x = first.x - second.x, z = first.z - second.z;
    return (y * y + x * x) + z * z;
}

// The MDC7 search compares the squared distance in double against 0.000025; the MDCX search uses the float constant.
// @verified - p
void clmdSelectColumnLink(ClothObject &object, std::byte *vertex, float *scores, double threshold) {
    Vector3 position, normal;
    std::memcpy(&position, vertex, sizeof(position));
    std::memcpy(&normal, vertex + 12, sizeof(normal));
    for (std::uint8_t column = 0; column < object.columns; ++column) {
        const ClothVertexBinding &binding = object.bindings[column];
        if (static_cast<double>(clmdDistanceSquared(binding.position, position)) < threshold) {
            const float score = (binding.normalEndpoint.x * normal.x + binding.normalEndpoint.y * normal.y) + binding.normalEndpoint.z * normal.z;
            if (score > scores[column]) {
                scores[column] = score;
                object.columnLinks[column] = vertex;
            }
        }
    }
}

// @verified - p
void clmdFindColumnLinksMDC7(ClothObject &object) {
    float scores[36];
    for (std::uint8_t column = 0; column < object.columns; ++column)
        scores[column] = 0.0f;
    std::byte *cursor = object.node->runtimeParent->runtimeMesh + 104;
    for (;;) {
        const std::uint32_t flags = ReadClothValue<std::uint32_t>(cursor);
        std::uint32_t count = ReadClothValue<std::uint32_t>(cursor + 4);
        cursor += 8;
        if (flags & 8U)
            count *= 3;
        for (std::uint32_t index = 0; index < count; ++index) {
            if (ReadClothValue<std::uint8_t>(cursor) & 1U) {
                clmdSelectColumnLink(object, cursor, scores, 0.000025);
                cursor += 32;
            } else
                cursor += 8;
        }
        const std::int32_t next = ReadClothValue<std::int32_t>(cursor);
        if (!next)
            break;
        if (next < 0)
            cursor += 80;
    }
}

// @verified - p
void clmdFindColumnLinksMDCX(ClothObject &object) {
    float scores[32];
    for (std::uint8_t column = 0; column < object.columns; ++column)
        scores[column] = 0.0f;
    std::byte *const mesh = object.node->runtimeParent->runtimeMesh;
    const std::int32_t offset = ReadClothValue<std::int32_t>(mesh + 28);
    const std::uint32_t count = ReadClothValue<std::uint32_t>(mesh + 4LL * offset + 32);
    const std::byte *record = mesh + 4LL * offset + 40;
    std::byte *first = mesh + 4LL * offset + 44;
    for (std::uint32_t index = 0; index < count; ++index, record += 12, first += 12) {
        std::byte *vertex = first;
        for (std::uint32_t remaining = ReadClothValue<std::uint32_t>(record) >> 5; remaining; --remaining, vertex += 32)
            clmdSelectColumnLink(object, vertex, scores, static_cast<double>(0.000025f));
    }
}

// @verified - p
void clmdInitializeGrid(ClothObject &object) {
    std::int32_t initialized[180]{};
    for (std::int32_t index = 0; index < object.vertexCount; ++index) {
        object.vertices[index].current = &object.bindings[index];
        object.vertices[index].previous = &object.previousBindings[index];
    }
    ClothVertexState *vertex = object.vertices;
    const std::byte *parameters = object.descriptorData10;
    for (std::int32_t row = 0; row < object.rows; ++row) {
        for (std::int32_t column = 0; column < object.columns; ++column, ++vertex) {
            if (row != object.rows - 1)
                vertex->below = vertex + object.columns;
            if (!row) {
                initialized[vertex - object.vertices] = 1;
                continue;
            }
            vertex->above = vertex - object.columns;
            std::memcpy(&vertex->velocity, parameters, sizeof(Vector3));
            vertex->aboveRestLength = ReadClothValue<float>(parameters + 12) * 10.0f;
            if (column || (object.flags & 0x80U)) {
                vertex->left = column ? vertex - 1 : vertex + object.columns - 1;
                vertex->leftRestLength = ReadClothValue<float>(parameters + 16) * 10.0f;
            }
            if (column != object.columns - 1 || (object.flags & 0x80U)) {
                vertex->right = column == object.columns - 1 ? vertex - (object.columns - 1) : vertex + 1;
                vertex->rightRestLength = ReadClothValue<float>(parameters + 20) * 10.0f;
            }
            if (vertex->flags != 0x40U) {
                parameters += 24;
                initialized[vertex - object.vertices] = 1;
            }
        }
    }
    bool complete;
    do {
        complete = true;
        ClothVertexState *const first = object.vertices + object.columns;
        vertex = first;
        for (std::int32_t row = 1; row < object.rows; ++row) {
            for (std::int32_t column = 0; column < object.columns; ++column, ++vertex) {
                const std::int32_t ready = initialized[vertex - first];
                if (!ready)
                    complete = false;
                if (vertex->flags != 0x40U || ready)
                    continue;
                if ((vertex->above && !initialized[vertex->above - first]) || (vertex->below && !initialized[vertex->below - first]) ||
                    (vertex->right && !initialized[vertex->right - first]) || (vertex->left && !initialized[vertex->left - first]))
                    continue;
                vertex->velocity = {};
                float count = 0.0f;
                const ClothVertexState *const neighbors[]{row > 1 ? vertex->above : nullptr, vertex->below, vertex->right, vertex->left};
                for (const ClothVertexState *neighbor : neighbors) {
                    if (!neighbor)
                        continue;
                    count += 1.0f;
                    vertex->velocity.x = neighbor->velocity.x + vertex->velocity.x;
                    vertex->velocity.y = neighbor->velocity.y + vertex->velocity.y;
                    vertex->velocity.z = neighbor->velocity.z + vertex->velocity.z;
                }
                if (count > 0.0f) {
                    vertex->velocity.x /= count;
                    vertex->velocity.y /= count;
                    vertex->velocity.z /= count;
                }
            }
        }
    } while (!complete);
}

// @verified - p
void clmdInitializePins(ClothObject &object) {
    const std::byte *const parameters = object.owner->parameters;
    for (std::uint8_t column = 0; column < object.columns; ++column)
        object.vertices[column].flags = 0x10U;
    const std::uint8_t pin = static_cast<std::uint8_t>(parameters[20]);
    if (pin == 1 || pin == 2) {
        ClothVertexState *vertex = object.vertices + object.columns + (pin == 2 ? object.columns - 1 : 0);
        for (std::uint8_t row = 1; row < object.rows; ++row, vertex += object.columns)
            vertex->flags = static_cast<std::uint8_t>((row - 1) | 0x20U);
    }
    const std::uint8_t selector = static_cast<std::uint8_t>(parameters[21]);
    const std::uint16_t pivot = selector == 254 ? object.columns >> 1 : selector == 255 ? object.columns - 1 : selector;
    for (std::uint8_t row = 0; row < object.rows; ++row)
        object.field50[row] = pivot;
    object.field0c = 0xffffU;
}

// @verified - p
void clmdUpdateMeshBounds(ClothObject &object) {
    Vector3 center{};
    for (std::uint8_t column = 0; column < object.columns; ++column) {
        const Vector3 &position = object.vertices[column].current->position;
        center.x += position.x;
        center.y += position.y;
        center.z += position.z;
    }
    center.x /= object.columns;
    center.y /= object.columns;
    center.z /= object.columns;
    const bool directX = IsModelXB01(object.mesh);
    const float radius = ReadClothValue<float>(object.mesh + 20) + ReadClothValue<float>(object.mesh + 20);
    std::byte *mesh = object.mesh;
    for (std::uint32_t pass = 0;; ++pass) {
        std::memcpy(mesh + 8, &center, sizeof(center));
        std::memcpy(mesh + 20, &radius, sizeof(radius));
        if (directX) {
            std::byte *command = mesh + 4LL * (ReadClothValue<std::int32_t>(mesh + 24) + 6);
            for (std::uint32_t flags = ReadClothValue<std::uint32_t>(command); flags; flags = ReadClothValue<std::uint32_t>(command)) {
                std::memcpy(command + 4, &center, sizeof(center));
                std::memcpy(command + 16, &radius, sizeof(radius));
                command += 4ULL * (flags >> 8U);
            }
        } else {
            std::byte *command = mesh + 24;
            do {
                const std::uint32_t bytes = ReadClothValue<std::uint32_t>(command + 76);
                std::memcpy(command + 16, &center, sizeof(center));
                std::memcpy(command + 28, &radius, sizeof(radius));
                command += bytes + 80;
            } while (ReadClothValue<std::uint32_t>(command));
        }
        if (!object.link || pass)
            break;
        mesh = object.link->mesh;
    }
}

// @verified - p
void clmdTransformBindings(ClothObject &object, ClothVertexBinding *bindings, bool inverse) {
    Matrix4x4_Push(nullptr);
    Matrix4x4::SetActiveTransform(&object.transform);
    if (inverse) {
        const Matrix4x4 source = workTransformMatrix;
        Matrix4x4 transform = source;
        for (unsigned row = 0; row < 3; ++row) {
            for (unsigned column = 0; column < 3; ++column)
                transform.rows[row][column] = source.rows[column][row];
            transform.rows[row].w = 0.0f;
            transform.rows[3][row] = -((source.rows[row].y * source.rows[3].y + source.rows[row].x * source.rows[3].x) + source.rows[row].z * source.rows[3].z);
        }
        transform.rows[3].w = 1.0f;
        Matrix4x4::SetActiveTransform(&transform);
    }
    for (std::int32_t index = 0; index < object.vertexCount; ++index) {
        Matrix4x4::TransformPoint(bindings[index].position, bindings[index].position);
        Matrix4x4::TransformPoint(bindings[index].normalEndpoint, bindings[index].normalEndpoint);
    }
    Matrix4x4::Pop(1);
}

// @verified - p
Vector3 clmdTriangleNormal(const Vector3 &first, const Vector3 &second, const Vector3 &third) {
    const Vector3 edge{second.x - first.x, second.y - first.y, second.z - first.z};
    const Vector3 next{third.x - second.x, third.y - second.y, third.z - second.z};
    Vector3 normal{next.z * edge.y - next.y * edge.z, next.x * edge.z - next.z * edge.x, next.y * edge.x - next.x * edge.y};
    const float squared = (normal.y * normal.y + normal.x * normal.x) + normal.z * normal.z;
    if (squared != 0.0f) {
        const float reciprocal = 1.0f / std::sqrt(squared);
        normal.x *= reciprocal;
        normal.y *= reciprocal;
        normal.z *= reciprocal;
    }
    return normal;
}

// @verified - p
void clmdOrientNormals(ClothObject &object) {
    const ClothVertexState &vertex = object.vertices[object.columns];
    const ClothVertexBinding &first = vertex.left ? *vertex.left->current : *vertex.above->current;
    const ClothVertexBinding &third = vertex.left ? *vertex.above->current : *vertex.right->current;
    const Vector3 normal = clmdTriangleNormal(first.position, vertex.current->position, third.position);
    const ClothVertexBinding &above = *vertex.above->current;
    const Vector3 direction{above.normalEndpoint.x - above.position.x, above.normalEndpoint.y - above.position.y, above.normalEndpoint.z - above.position.z};
    if ((direction.x * normal.x + direction.y * normal.y) + direction.z * normal.z < 0.0f)
        object.flags |= 0x100U;
}

// @verified - p
void clmdUpdateNormals(ClothObject &object) {
    ClothVertexState *vertex = object.vertices + object.columns;
    for (std::uint8_t row = 1; row < object.rows; ++row) {
        for (std::uint8_t column = 0; column < object.columns; ++column, ++vertex) {
            const ClothVertexBinding &first = vertex->left ? *vertex->left->current : *vertex->above->current;
            const ClothVertexBinding &third = vertex->left ? *vertex->above->current : *vertex->right->current;
            Vector3 normal = clmdTriangleNormal(first.position, vertex->current->position, third.position);
            if (object.flags & 0x100U) {
                normal.x *= -1.0f;
                normal.y *= -1.0f;
                normal.z *= -1.0f;
            }
            vertex->current->normalEndpoint = {
                normal.x + vertex->current->position.x, normal.y + vertex->current->position.y, normal.z + vertex->current->position.z};
        }
    }
}

// @verified - p
void clmdCopyLinkedBindings(ClothObject &object) {
    const ClothVertexBinding *source = object.bindings;
    ClothVertexBinding *destination = object.link->bindings;
    for (std::int32_t index = 0; index < object.vertexCount; ++index) {
        if (object.vertices[index].flags == 0x40U)
            continue;
        destination->position = source->position;
        destination->normalEndpoint = {destination->position.x - (source->normalEndpoint.x - source->position.x),
            destination->position.y - (source->normalEndpoint.y - source->position.y),
            destination->position.z - (source->normalEndpoint.z - source->position.z)};
        ++source;
        ++destination;
    }
}

// @verified - p
void clmdClampRestLength(const Vector3 &anchor, const Vector3 &point, float length, Vector3 &output) {
    const float squared = clmdDistanceSquared(anchor, point);
    if (!(squared < 0.0000001f)) {
        const float ratio = (1.0f / std::sqrt(squared)) * length;
        output = {(point.x - anchor.x) * ratio + anchor.x, (point.y - anchor.y) * ratio + anchor.y, (point.z - anchor.z) * ratio + anchor.z};
    } else
        output = anchor;
}

// @verified - p
bool clmdProjectSimpleSphere(Vector3 &point, const Vector3 &center, float squared, float radiusSquared) {
    const float limit = radiusSquared * 0.89999998f;
    if (limit < squared)
        return false;
    const float ratio = std::sqrt(limit / squared);
    point = {(point.x - center.x) * ratio + center.x, (point.y - center.y) * ratio + center.y, (point.z - center.z) * ratio + center.z};
    return true;
}

// @verified - p
void clmdProjectSphere(Vector3 &point, const Vector3 &center, std::uint8_t capsule, ClothVertexState &vertex, float radiusSquared, float aboveDistance,
    float outerDistance, float innerDistance) {
    Vector3 direction;
    const Vector3 &normal = vertex.current->normalEndpoint;
    if (aboveDistance > innerDistance) {
        const float blend = (aboveDistance - innerDistance) / (outerDistance - innerDistance);
        Vector3 radial{point.x - center.x, point.y - center.y, point.z - center.z};
        NormalizeVector3(&radial);
        Vector3 facing{normal.x - point.x, normal.y - point.y, normal.z - point.z};
        NormalizeVector3(&facing);
        direction = {(radial.x - facing.x) * blend + facing.x, (radial.y - facing.y) * blend + facing.y, (radial.z - facing.z) * blend + facing.z};
    } else
        direction = {normal.x - point.x, normal.y - point.y, normal.z - point.z};
    vertex.collisionIndex = capsule;
    const Vector3 offset{point.x - center.x, point.y - center.y, point.z - center.z};
    const float squared = (direction.y * direction.y + direction.x * direction.x) + direction.z * direction.z;
    const float dot = (offset.y * direction.y + offset.x * direction.x) + offset.z * direction.z;
    const float twiceDot = dot + dot;
    const float discriminant = twiceDot * twiceDot - (((offset.y * offset.y + offset.x * offset.x) + offset.z * offset.z) - radiusSquared) * (squared * 4.0f);
    if (!(discriminant < 0.0f)) {
        const float root = std::sqrt(discriminant);
        const float denominator = squared + squared;
        float distance = (root - twiceDot) / denominator;
        if (distance < 0.0f)
            distance = (-twiceDot - root) / denominator;
        point = {distance * direction.x + point.x, distance * direction.y + point.y, distance * direction.z + point.z};
    }
}

// @verified - p
void clmdResolveBoneCollision(ClothObject &object, Vector3 &point, ClothVertexState &vertex) {
    vertex.collisionIndex = 0xffU;
    const Vector3 &above = vertex.above->current->position;
    for (std::uint8_t index = 0; index < object.owner->boneRecordCount; ++index) {
        const ClothBoneCapsule &capsule = object.boneBuffer[index];
        const float radiusSquared = capsule.radius * capsule.radius;
        const float distance = clmdDistanceSquared(point, capsule.position);
        const float squared = 0.0000001f > distance ? 0.0000001f : distance;
        if (radiusSquared < squared)
            continue;
        if (!(object.owner->mode & 0x400U)) {
            const float aboveDistance = std::sqrt(clmdDistanceSquared(above, capsule.position));
            const float outerDistance = vertex.aboveRestLength + capsule.radius;
            clmdProjectSphere(point, capsule.position, index, vertex, radiusSquared, aboveDistance, outerDistance, outerDistance * object.owner->field98);
        } else if (!clmdProjectSimpleSphere(point, capsule.position, squared, radiusSquared))
            continue;
        clmdClampRestLength(above, point, vertex.aboveRestLength, point);
    }
}

// @verified - p
void clmdUpdateDamping(ClothObject &object, ClothVertexState &vertex, Vector3 &velocity) {
    if (vertex.collisionIndex != 0xffU || vertex.moved > 0)
        return;
    const ClothComponent &owner = *object.owner;
    if (!(owner.windStrength < 0.0000001f)) {
        const ClothVertexBinding &binding = *vertex.current;
        Vector3 facing{
            binding.normalEndpoint.x - binding.position.x, binding.normalEndpoint.y - binding.position.y, binding.normalEndpoint.z - binding.position.z};
        NormalizeVector3(&facing);
        clothWindReciprocal = 1.0f / owner.windStrength;
        const Vector3 wind{clothWindReciprocal * owner.windOffset.x, clothWindReciprocal * owner.windOffset.y, clothWindReciprocal * owner.windOffset.z};
        const float scale = ((facing.x * wind.x + facing.y * wind.y) + facing.z * wind.z + 1.0f) * 0.5f;
        velocity.x = scale * owner.windOffset.x + velocity.x;
        velocity.z = scale * owner.windOffset.z + velocity.z;
    }
    if (object.flags & 0x80U)
        velocity.y *= 0.5f;
    else {
        clothDamping = &clothDampingTable[static_cast<std::uint8_t>(owner.parameters[25])];
        velocity.x *= clothDamping->x;
        velocity.y = clothDamping->y * velocity.y;
        velocity.z = clothDamping->z * velocity.z;
    }
}

// @verified - p
void clmdIntegrateVertex(ClothObject &object, ClothVertexState &vertex, Vector3 &velocity, const Vector3 &firstForce, const Vector3 &secondForce,
    std::int32_t columnsRemaining, std::int32_t rowsRemaining, Vector3 &point) {
    const ClothVertexBinding &above = *vertex.above->current;
    const ClothVertexBinding *previous = &above;
    if (object.flags & 0x40U)
        velocity = {};
    else
        previous = vertex.previous;
    const ClothComponent &owner = *object.owner;
    const std::int32_t row = object.rows - rowsRemaining, column = object.columns - columnsRemaining;
    const Vector3 &force = static_cast<std::uint8_t>(owner.parameters[23]) > 0 && row >= static_cast<std::uint8_t>(owner.parameters[24]) &&
            (owner.assignMode != 4 || row >= 7 || column >= 7 || column <= 0) ?
        secondForce :
        firstForce;
    point = {(previous->position.x + velocity.x) + force.x, (velocity.y + previous->position.y) + force.y, (velocity.z + previous->position.z) + force.z};
    vertex.current->normalEndpoint.x = std::bit_cast<float>(owner.field88) + vertex.current->normalEndpoint.x;
    vertex.current->normalEndpoint.y = owner.field8c + vertex.current->normalEndpoint.y;
    vertex.current->normalEndpoint.z = std::bit_cast<float>(owner.field90) + vertex.current->normalEndpoint.z;
    clmdClampRestLength(above.position, point, vertex.aboveRestLength, point);
}

// @verified - p
void clmdConstrainRow(ClothObject &object, ClothVertexState *end, std::int32_t row) {
    ClothVertexState *const first = end - object.columns;
    if (!(object.flags & 0x80U)) {
        const std::uint16_t pivot = object.field50[row];
        if (pivot) {
            for (std::int32_t column = pivot - 1; column >= 0; --column) {
                ClothVertexState &vertex = first[column];
                clmdClampRestLength(vertex.right->current->position, vertex.current->position, vertex.rightRestLength, vertex.current->position);
            }
        }
        if (pivot != object.columns) {
            for (std::int32_t column = pivot + 1; column < object.columns; ++column) {
                ClothVertexState &vertex = first[column];
                clmdClampRestLength(vertex.left->current->position, vertex.current->position, vertex.leftRestLength, vertex.current->position);
            }
        }
    } else {
        float circumference = 0.0f;
        for (std::uint8_t column = 0; column < object.columns; ++column)
            circumference += std::sqrt(clmdDistanceSquared(first[column].current->position, first[column].right->current->position));
        const float diameter = object.owner->minimumCapsuleRadius + object.owner->minimumCapsuleRadius;
        const float average = circumference / static_cast<float>(object.columns);
        float length = diameter < average ? diameter : average;
        object.rowLengths[row] = length;
        for (std::uint8_t column = 0; column < object.columns; ++column) {
            ClothVertexState &vertex = first[column];
            if (vertex.moved)
                continue;
            if (object.owner->mode & 0x200U)
                length = vertex.leftRestLength;
            clmdClampRestLength(vertex.left->current->position, vertex.current->position, length, clothLeftConstraint);
            if (object.owner->mode & 0x200U)
                length = vertex.rightRestLength;
            clmdClampRestLength(vertex.right->current->position, vertex.current->position, length, clothRightConstraint);
            vertex.current->position = {(clothRightConstraint.x + clothLeftConstraint.x) * 0.5f, (clothRightConstraint.y + clothLeftConstraint.y) * 0.5f,
                (clothRightConstraint.z + clothLeftConstraint.z) * 0.5f};
        }
    }
}

// @verified - p
void clmdSolveRows(ClothObject &object, const Vector3 &firstForce, const Vector3 &secondForce) {
    ClothVertexState *vertex = object.vertices + object.columns;
    for (std::int32_t row = object.rows - 1; row > 0; --row) {
        for (std::int32_t column = object.columns; column > 0; --column, ++vertex) {
            const ClothVertexBinding &above = *vertex->above->current;
            const ClothVertexBinding &previous = *vertex->above->previous;
            clothVelocity = {above.position.x - previous.position.x, above.position.y - previous.position.y, above.position.z - previous.position.z};
            clmdUpdateDamping(object, *vertex, clothVelocity);
            Vector3 point;
            clmdIntegrateVertex(object, *vertex, clothVelocity, firstForce, secondForce, column, row, point);
            clmdResolveBoneCollision(object, point, *vertex);
            vertex->current->position = point;
        }
        clmdConstrainRow(object, vertex, row);
    }
}

// @verified - p
void clmdLiftBelowAnkles(ClothObject &object) {
    Vector3 position;
    mtwkGetBonePosition(object.owner->motionWork, MB_LeftAnkle, &position);
    const float firstHeight = position.y;
    mtwkGetBonePosition(object.owner->motionWork, MB_RightAnkle, &position);
    const float height = (position.y < firstHeight ? position.y : firstHeight) + clothAnkleOffset;
    ClothVertexState *vertex = object.vertices + object.columns;
    for (std::uint8_t row = 1; row < object.rows; ++row) {
        for (std::uint8_t column = 0; column < object.columns; ++column, ++vertex) {
            if (height > vertex->current->position.y) {
                vertex->current->position.y = (vertex->current->position.y - height) * 0.0099999998f + height;
                vertex->moved = 1;
            }
        }
    }
}

// @verified - p
void clmdUpdateDynamics(ClothObject &object) {
    ClothComponent &owner = *object.owner;
    MotionWork *const work = owner.motionWork;
    const std::byte *const parameters = owner.parameters;
    std::int32_t referenceBone = 1;
    const std::uint8_t firstKind = static_cast<std::uint8_t>(parameters[22]);
    if (firstKind == 2 || firstKind == 5) {
        if (firstKind == 5)
            referenceBone = 3;
        mtwkGetBonePosition(work, MB_LeftThigh, &clothRootPosition);
        mtwkGetBonePosition(work, MB_RightThigh, &clothPairedBonePosition);
    } else if (firstKind == 3) {
        mtwkGetBonePosition(work, MB_LeftShin, &clothRootPosition);
        mtwkGetBonePosition(work, MB_RightShin, &clothPairedBonePosition);
    } else if (firstKind == 4) {
        mtwkGetBonePosition(work, MB_Pelvis, &clothRootPosition);
        clothPairedBonePosition = {clothRootPosition.x, clothRootPosition.y - 2.0f, clothRootPosition.z};
        clothRootPosition.y = clothPairedBonePosition.y;
    } else {
        mtwkGetBonePosition(work, MB_LeftAnkle, &clothRootPosition);
        mtwkGetBonePosition(work, MB_RightAnkle, &clothPairedBonePosition);
    }
    clothRootPosition.x += (clothPairedBonePosition.x - clothRootPosition.x) * 0.5f;
    clothRootPosition.y += (clothPairedBonePosition.y - clothRootPosition.y) * 0.5f;
    clothRootPosition.z += (clothPairedBonePosition.z - clothRootPosition.z) * 0.5f;
    Vector3 reference;
    mtwkGetBonePosition(work, static_cast<MotionBoneId>(referenceBone), &reference);
    const float firstLength = std::sqrt(clmdDistanceSquared(clothRootPosition, reference));
    Vector3 firstForce{};
    if (!(std::fabs(firstLength) < 0.00000011920929f)) {
        const float ratio = 0.054444447f / firstLength;
        firstForce = {(clothRootPosition.x - reference.x) * ratio, (clothRootPosition.y - reference.y) * ratio, (clothRootPosition.z - reference.z) * ratio};
    }
    const std::uint8_t secondKind = static_cast<std::uint8_t>(parameters[23]);
    Vector3 secondForce{};
    if (secondKind) {
        Vector3 first, second;
        switch (secondKind) {
        case 1:
            mtwkGetBonePosition(work, MB_LeftAnkle, &first);
            mtwkGetBonePosition(work, MB_RightAnkle, &second);
            break;
        case 2:
            mtwkGetBonePosition(work, MB_LeftThigh, &first);
            mtwkGetBonePosition(work, MB_RightThigh, &second);
            break;
        case 3:
            mtwkGetBonePosition(work, MB_LeftShin, &first);
            mtwkGetBonePosition(work, MB_RightShin, &second);
            break;
        case 4:
            mtwkGetBonePosition(work, MB_Pelvis, &first);
            second = {first.x, first.y - 2.0f, first.z};
            first.y = second.y;
            break;
        }
        first.x += (second.x - first.x) * 0.5f;
        first.y += (second.y - first.y) * 0.5f;
        first.z += (second.z - first.z) * 0.5f;
        const float length = std::sqrt(clmdDistanceSquared(first, reference));
        if (!(std::fabs(length) < 0.00000011920929f)) {
            const float ratio = 0.054444447f / length;
            secondForce = {(first.x - reference.x) * ratio, (first.y - reference.y) * ratio, (first.z - reference.z) * ratio};
        }
    }
    if (owner.assignMode == 17) {
        firstForce.y += -0.0099999998f;
        secondForce.y += -0.02f;
    }
    if (owner.mode & 0x100U) {
        Matrix4x4_Push(nullptr);
        Matrix4x4::SetActiveTransform(&object.transform);
        Vector3 from{0.0f, -1.0f, 0.0f};
        Matrix4x4::Transform(from, from);
        ClothVertexState *vertex = object.vertices + object.columns;
        for (std::int32_t row = 1; row < object.rows; ++row) {
            if (!secondKind || row == 1 || row == static_cast<std::uint8_t>(parameters[24])) {
                Vector3 direction = !secondKind || row == 1 ? firstForce : secondForce;
                NormalizeVector3(&direction);
                Matrix4x4::SetPrePassToTempTransform();
                Matrix4x4_BuildRotationMatrix(from, direction);
            }
            for (std::uint8_t column = 0; column < object.columns; ++column, ++vertex) {
                ClothVertexBinding &binding = *vertex->current;
                binding.normalEndpoint.x -= binding.position.x;
                binding.normalEndpoint.y -= static_cast<float>(row) * 0.1f + binding.position.y;
                binding.normalEndpoint.z -= binding.position.z;
                NormalizeVector3(&binding.normalEndpoint);
                Vector3 direction;
                Matrix4x4::Transform(binding.normalEndpoint, direction);
                binding.normalEndpoint = {direction.x + binding.position.x, direction.y + binding.position.y, direction.z + binding.position.z};
            }
        }
        Matrix4x4::Pop(1);
    }
    const float rootDistance = clmdDistanceSquared(object.vertices[0].current->position, object.vertices[0].previous->position);
    if (rootDistance > 0.34999999f) {
        object.flags |= 0x40U;
        if (owner.name.name == CLOTH_CHARACTER_SRY.name && owner.assignMode == 2 && static_cast<double>(rootDistance) < 1.049999982118607)
            object.flags &= ~0x40U;
    }
    clmdSolveRows(object, firstForce, secondForce);
    if (object.flags & 0x200U) {
        const std::uint8_t threshold = owner.assignMode == 2 ? 0 : (object.flags & 0x80U) ? 13 : 6;
        float scale = 1.0f / static_cast<float>(object.rows);
        if (object.columns < threshold)
            scale *= static_cast<float>(object.columns) / static_cast<float>(threshold);
        ClothVertexState *vertex = object.vertices + object.columns;
        for (std::int32_t row = 1; row < object.rows; ++row) {
            const float amplitude = static_cast<float>(row) * scale;
            for (std::int32_t column = 0; column < object.columns; ++column, ++vertex) {
                ClothVertexBinding &binding = *vertex->current;
                const std::uint32_t phase = static_cast<std::uint32_t>(owner.oscillationRotation) - CLOTH_WIND_COLUMN_STEP * static_cast<std::uint32_t>(column);
                const float sine = sin_fixed16(static_cast<std::int16_t>(phase));
                binding.position.x += ((binding.normalEndpoint.x - binding.position.x) * owner.field9c) * sine * amplitude;
                binding.position.z += ((binding.normalEndpoint.z - binding.position.z) * owner.field9c) * sine * amplitude;
            }
        }
    }
    object.flags &= ~0x40U;
}

// @verified - p
std::int32_t clmdUpdateModelNode(MT7Node *model, MT7Node *node) {
    ClothObject *const object = static_cast<ClothObject *>(node->taskData);
    if (!object)
        return 0;
    ClothComponent &owner = *object->owner;
    if (owner.mode & 2U) {
        node->runtimeMesh = object->sourceMesh;
        if (object->link)
            object->link->node->runtimeMesh = object->link->sourceMesh;
        return 0;
    }
    if (shenIsGamePaused() || (owner.mode & 0x804U))
        return 1;
    const bool directX = node->meshType == SHEN_NAME(MDCX);
    const bool supported = directX || node->meshType == SHEN_NAME(MDC7);
    if (supported)
        clmdBindMeshVertices(object->descriptorData28, object->sourceMesh, object->bindings, directX);
    if (object->flags & 1U) {
        object->flags &= ~1U;
        object->boneBuffer = owner.boneBuffer;
        float minimum = 10.0f;
        for (std::uint8_t index = 0; index < owner.boneRecordCount; ++index)
            minimum = owner.boneBuffer[index].radius < minimum ? owner.boneBuffer[index].radius : minimum;
        owner.minimumCapsuleRadius = minimum;
        node->runtimeMesh = object->sourceMesh;
        if (object->link)
            object->link->node->runtimeMesh = object->link->sourceMesh;
        if (directX)
            clmdFindColumnLinksMDCX(*object);
        else if (supported)
            clmdFindColumnLinksMDC7(*object);
        clmdInitializeGrid(*object);
        clmdInitializePins(*object);
        clmdUpdateMeshBounds(*object);
        clmdTransformBindings(*object, object->bindings, false);
        clmdOrientNormals(*object);
        std::memcpy(object->previousBindings, object->bindings, 28ULL * object->vertexCount);
        clmdUpdateModelNode(model, node);
        return 0;
    }
    clmdTransformBindings(*object, object->bindings, false);
    clmdUpdateDynamics(*object);
    clmdUpdateNormals(*object);
    if (owner.assignMode == 1)
        clmdLiftBelowAnkles(*object);
    std::memcpy(object->previousBindings, object->bindings, 28ULL * object->vertexCount);
    clmdTransformBindings(*object, object->bindings, true);
    for (std::uint8_t column = 0; column < object->columns; ++column) {
        if (object->columnLinks[column])
            std::memcpy(&object->bindings[column].normalEndpoint, static_cast<const std::byte *>(object->columnLinks[column]) + 12, sizeof(Vector3));
    }
    if (supported)
        clmdWriteMeshVertices(object->descriptorData28, object->bindings, object->mesh, directX);
    node->runtimeMesh = object->mesh;
    if (object->link) {
        clmdCopyLinkedBindings(*object);
        if (supported)
            clmdWriteMeshVertices(object->link->descriptorData, object->link->bindings, object->link->mesh, directX);
        object->link->node->runtimeMesh = object->link->mesh;
    }
    object->flags |= 0x20U;
    ++owner.field0a;
    return 1;
}

// @verified - p
void clmdPublishSiblingMatrices(MT7Node *node) {
    for (; node; node = node->runtimeSibling) {
        ClothObject *const object = static_cast<ClothObject *>(node->taskData);
        if (object && object->token.name == CLOTH_OBJECT.name)
            Matrix4x4::GetTransformMatrix(&object->transform);
    }
}

// @verified - p
void AllocateCLTH_CLMS_CLID_CLVB_CLVO_CLVP(ClothComponent &cloth, MT7Model &model, MT7Node *root) {
    std::byte *const descriptors = cloth.source + ReadClothValue<std::uint32_t>(cloth.source + ReadClothValue<std::int32_t>(cloth.source + 0x0c) + 8);
    for (MT7Node *current = root; current; current = current->runtimeSibling) {
        MT7Node &node = *current;
        const std::uint16_t id = static_cast<std::uint16_t>(node.id);
        std::byte *const sourceMesh = node.runtimeMesh ? node.runtimeMesh :
            node.xb01Offset                            ? const_cast<std::byte *>(model.source.data()) + node.xb01Offset :
                                                         nullptr;
        if (id == 0xffa6U) {
            auto *const object = static_cast<ClothObject *>(HLib::AllocateTaskData(sizeof(ClothObject), SHEN_NAME(CLTH)));
            HLib::AssignDynamicAlignmentForTaskData(object);
            node.taskData = object;
            object->index = cloth.clothCount;
            object->boneRecordCount = cloth.boneRecordCount;
            cloth.cloths[cloth.clothCount++] = object;
            object->flags = 0;
            object->token = SHEN_NAME(CLTH);
            object->rows = 0;
            object->columns = 0;
            object->vertexCount = 0;
            object->owner = &cloth;
            object->nodeId = id;
            object->field0c = 0xffffU;
            object->flags = 1;
            object->node = &node;
            object->bindings = nullptr;
            object->previousBindings = nullptr;
            object->link = nullptr;
            object->columnLinks = nullptr;
            object->field50.fill(0xffffU);
            object->sourceMesh = sourceMesh;
            object->meshSize = static_cast<std::uint32_t>(modelGetClothSize(model, node));
            object->mesh = static_cast<std::byte *>(HLib::AllocateTaskData(object->meshSize, SHEN_NAME(CLMS)));
            HLib::AssignDynamicAlignmentForTaskData(object->mesh);
            std::memcpy(object->mesh, object->sourceMesh, object->meshSize);
            const std::byte *const descriptor = descriptors + (static_cast<std::size_t>(object->index) << 6);
            object->columns = static_cast<std::uint8_t>(descriptor[0]);
            object->rows = static_cast<std::uint8_t>(descriptor[4]);
            object->vertexCount = object->rows * object->columns;
            object->descriptorData28 = cloth.source + ReadClothValue<std::int32_t>(descriptor + 0x28);
            object->descriptorData10 = cloth.source + ReadClothValue<std::int32_t>(descriptor + 0x10);
            if (ReadClothValue<std::uint32_t>(descriptor + 8) != 0)
                object->flags |= 0x80U;
            if (!object->descriptorData28 || !object->descriptorData10)
                object->owner->mode |= 2U;
            const std::uint64_t vertexCount = static_cast<std::uint64_t>(object->vertexCount);
            object->vertices = static_cast<ClothVertexState *>(HLib::AllocateTaskData(sizeof(ClothVertexState) * vertexCount, SHEN_NAME(CLID)));
            HLib::AssignDynamicAlignmentForTaskData(object->vertices);
            for (std::int32_t index = 0; index < object->vertexCount; ++index) {
                ClothVertexState &vertex = object->vertices[index];
                vertex.collisionIndex = 0xffU;
                vertex.moved = 0;
                vertex.flags = 0;
                std::memset(&vertex.velocity, 0, sizeof(ClothVertexState) - offsetof(ClothVertexState, velocity));
            }
            object->bindings = static_cast<ClothVertexBinding *>(HLib::AllocateTaskData(sizeof(ClothVertexBinding) * vertexCount, SHEN_NAME(CLVB)));
            object->previousBindings = static_cast<ClothVertexBinding *>(HLib::AllocateTaskData(sizeof(ClothVertexBinding) * vertexCount, SHEN_NAME(CLVO)));
            HLib::AssignDynamicAlignmentForTaskData(object->bindings);
            HLib::AssignDynamicAlignmentForTaskData(object->previousBindings);
            for (std::int32_t index = 0; index < object->vertexCount; ++index) {
                object->bindings[index].index = 0xffffU;
                object->previousBindings[index].index = 0xffffU;
                object->bindings[index].field02 = 1;
                object->previousBindings[index].field02 = 1;
            }
            object->columnLinks = static_cast<void **>(HLib::AllocateTaskData(8ULL * object->columns, SHEN_NAME(CLPV)));
            HLib::AssignDynamicAlignmentForTaskData(object->columnLinks);
            for (std::uint8_t index = 0; index < object->columns; ++index)
                object->columnLinks[index] = nullptr;
            if (cloth.name.name != SHEN_NAME(XGH_) || static_cast<std::uint16_t>(node.runtimeParent->id) != 1U) {
                object->flags |= 0x200U;
            }
        } else if (id == 0xffa5U) {
            ClothObject *const primary = static_cast<ClothObject *>(node.runtimeParent->taskData);
            std::byte *const data = cloth.source + ReadClothValue<std::int32_t>(descriptors + (static_cast<std::size_t>(primary->index) << 6) + 0x2c);
            if (data) {
                auto *const link = static_cast<ClothLink *>(HLib::AllocateTaskData(sizeof(ClothLink), SHEN_NAME(CLTH)));
                HLib::AssignDynamicAlignmentForTaskData(link);
                primary->link = link;
                link->primary = primary;
                link->descriptorData = data;
                link->node = &node;
                link->sourceMesh = sourceMesh;
                const std::uint32_t size = static_cast<std::uint32_t>(modelGetClothSize(model, node));
                link->mesh = static_cast<std::byte *>(HLib::AllocateTaskData(size, SHEN_NAME(CLMS)));
                HLib::AssignDynamicAlignmentForTaskData(link->mesh);
                std::memcpy(link->mesh, link->sourceMesh, size);
                const std::uint64_t vertexCount = static_cast<std::uint64_t>(primary->vertexCount);
                link->bindings = static_cast<ClothVertexBinding *>(HLib::AllocateTaskData(sizeof(ClothVertexBinding) * vertexCount, SHEN_NAME(CLVB)));
                HLib::AssignDynamicAlignmentForTaskData(link->bindings);
                for (std::int32_t index = 0; index < primary->vertexCount; ++index) {
                    primary->bindings[index].index = 0xffffU;
                    primary->previousBindings[index].index = 0xffffU;
                    primary->bindings[index].field02 = 1;
                    primary->previousBindings[index].field02 = 1;
                }
            }
        }
        if (node.runtimeChild)
            AllocateCLTH_CLMS_CLID_CLVB_CLVO_CLVP(cloth, model, node.runtimeChild);
    }
}


// @verified - p
void AllocateCLTH_CLMS_CLID_CLVB_CLVO_CLVP_Wrapper(ClothComponent &cloth, MT7Model &model) {
    cloth.mode = 0;
    cloth.boneRecords = nullptr;
    cloth.clothCount = 0;
    cloth.cloths = {};
    const std::byte *bones = nullptr;
    if (cloth.parameters) {
        bones = cloth.source + ReadClothValue<std::int32_t>(cloth.parameters + 4);
    } else {
        cloth.parameters = defaultClothParameters.data();
        bones = reinterpret_cast<const std::byte *>(defaultClothBones.data());
    }
    CLCB(cloth, bones);
    if (cloth.parameters)
        cloth.mode = static_cast<std::uint16_t>(static_cast<std::uint8_t>(cloth.parameters[0x1a]) << 8);
    AllocateCLTH_CLMS_CLID_CLVB_CLVO_CLVP(cloth, model, &model.nodes.front());
    const float distanceSquared = cloth.clothCount == 1 && cloth.cloths[0]->rows <= 2 ? 289.0f : 1156.0f;
    std::memcpy(cloth.parameters + 0x1c, &distanceSquared, sizeof(distanceSquared));
}

void FreeClothTaskData(void *data) {
    HLib::ClearTaskDataMark(data);
    HLib::FreeTaskData(data);
}

// Native FreeClothObjects: releases every CLTH, its link and their buffers, and unlinks the cloth nodes.
// @verified - p
void FreeClothObjects(ClothComponent &cloth) {
    for (ClothObject *&object : cloth.cloths) {
        if (!object)
            continue;
        if (object->columnLinks) {
            for (std::uint8_t index = 0; index < object->columns; ++index)
                object->columnLinks[index] = nullptr;
            FreeClothTaskData(object->columnLinks);
            object->columnLinks = nullptr;
        }
        if (object->mesh) {
            FreeClothTaskData(object->mesh);
            object->mesh = nullptr;
        }
        if (object->vertices) {
            FreeClothTaskData(object->vertices);
            object->vertices = nullptr;
        }
        if (object->bindings) {
            FreeClothTaskData(object->bindings);
            object->bindings = nullptr;
        }
        if (object->previousBindings) {
            FreeClothTaskData(object->previousBindings);
            object->bindings = nullptr;
        }
        if (ClothLink *const link = object->link) {
            if (link->bindings) {
                FreeClothTaskData(link->bindings);
                link->bindings = nullptr;
            }
            if (link->mesh) {
                FreeClothTaskData(link->mesh);
                link->mesh = nullptr;
            }
            FreeClothTaskData(link);
            object->link = nullptr;
        }
        object->node->taskData = nullptr;
        FreeClothTaskData(object);
        object = nullptr;
    }
}

// Native FreeUSE_0: the CLMD cleanup.
// @verified - p
void FreeUSE_0(Character *, void *payload) {
    ClothComponent &cloth = *static_cast<ClothComponent *>(payload);
    if (cloth.boneBuffer) {
        FreeClothTaskData(cloth.boneBuffer);
        cloth.boneBuffer = nullptr;
    }
    FreeClothObjects(cloth);
}

void UpdateWindReactiveNode(Character *character, void *payload);

// Native ResetClothObjects: rebuilds a named character's CLMD objects from its current cloth source.
// @verified - p
void ResetClothObjects(Character *character) {
    MT7Model &model = *static_cast<MT7Model *>(character->model);
    MT7Node *const root = &model.nodes.front();
    if (!GetChildObjectByIndex(model, root, 0xffa6U) && !GetChildObjectByIndex(model, root, 0xffa5U))
        return;
    const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const shen_name name = scenGetCharaName(control->scenarioTask, character);
    if (!name.name)
        return;
    auto *cloth = static_cast<ClothComponent *>(FindModelComponent(character, kClothToken));
    if (!cloth)
        cloth = static_cast<ClothComponent *>(AllocateModelComponent(character, kClothToken, sizeof(ClothComponent), UpdateWindReactiveNode));
    if (cloth->boneBuffer) {
        FreeClothTaskData(cloth->boneBuffer);
        cloth->boneBuffer = nullptr;
    }
    FreeClothObjects(*cloth);
    cloth->name = name;
    cloth->model = root;
    AllocateCLTH_CLMS_CLID_CLVB_CLVO_CLVP_Wrapper(*cloth, model);
}


// @verified - p
float ComputeWindOscillationNoise(std::int32_t oscillation) {
    const std::uint32_t value = static_cast<std::uint32_t>(oscillation);
    float noise = sin_fixed16(static_cast<std::int16_t>(value * 22U)) * 0.3f;
    noise = sin_fixed16(static_cast<std::int16_t>(value * 31U)) * 0.37f + noise;
    noise = sin_fixed16(static_cast<std::int16_t>(value * 103U)) * 0.15f + noise;
    return sin_fixed16(static_cast<std::int16_t>(value * 147U)) * 0.18f + noise;
}

// Native UpdateWindReactiveNode, the CLMD update: gates the cloth by camera distance, latches reset
// requests and eases the wind offset that the cloth simulation applies.
// @verified - p
void UpdateWindReactiveNode(Character *character, void *payload) {
    ClothComponent &cloth = *static_cast<ClothComponent *>(payload);
    // Native also keeps the cloth active while a quick-icon word is set; only InitQuickIcon_Callback writes
    // it, always to zero.
    bool active = authGetLoaded() != 0 || !plymIsSearchStartRequested();
    if (!active) {
        const Vector3 position = GetCharacterWorldPosition(character);
        Vector3 camera{};
        camCtrlGetNextCamPos(&camera);
        const float dz = position.z - camera.z;
        const float dx = position.x - camera.x;
        active = !(dz * dz + dx * dx > ReadClothValue<float>(cloth.parameters + 0x1c));
    }
    if (active)
        cloth.mode &= static_cast<std::uint16_t>(~0x800U);
    else
        cloth.mode |= 0x800U;
    if ((cloth.mode & 4U) == 0 && ((static_cast<std::uint8_t>(cloth.mode) ^ cloth.previousMode) & 4U) != 0)
        cloth.mode |= 0x10U;
    if ((cloth.mode & 2U) == 0 && ((static_cast<std::uint8_t>(cloth.mode) ^ cloth.previousMode) & 2U) != 0)
        cloth.mode |= 0x10U;
    for (ClothObject *const object : cloth.cloths) {
        if (!object)
            continue;
        object->flags &= static_cast<std::uint16_t>(~0x20U);
        if ((cloth.mode & 0x10U) != 0)
            object->flags |= 0x40U;
    }
    if ((cloth.mode & 0x10U) != 0)
        cloth.mode &= static_cast<std::uint16_t>(~0x10U);
    if ((cloth.mode & 0x20U) != 0) {
        if (cloth.resetCountdown-- == 1) {
            ResetClothObjects(character);
            cloth.mode &= static_cast<std::uint16_t>(~0x20U);
        } else {
            for (ClothObject *const object : cloth.cloths) {
                if (object)
                    object->flags |= 0x40U;
            }
        }
    }
    constexpr shen_name kun{SHEN_NAME(KUN_)};
    if (static_cast<std::uint8_t>(cloth.parameters[0x19]) != 1U) {
        if (cloth.assignMode == 2) {
            const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
            if (Character *const author = scenFindChara(control->scenarioTask, SHEN_NAME(a334))) {
                const Vector3 location = GetCharacterWorldPosition(author);
                Vector3 direction = GetCharacterWorldPosition(character);
                direction.x -= location.x;
                direction.y -= location.y;
                direction.z -= location.z;
                const float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
                if (magnitude > 0.0f) {
                    const float inverse = 1.0f / magnitude;
                    direction.x *= inverse;
                    direction.y *= inverse;
                    direction.z *= inverse;
                }
                const float delta = 0.08f - cloth.windStrength;
                cloth.oscillation = static_cast<std::int32_t>(static_cast<std::uint32_t>(cloth.oscillation) + CLOTH_OSCILLATION_STEP);
                cloth.oscillationRotation = static_cast<std::int32_t>(static_cast<std::uint32_t>(cloth.oscillationRotation) + CLOTH_OSCILLATION_ROTATION_STEP);
                const float strength = delta * 0.5f + cloth.windStrength;
                cloth.windStrength = strength;
                cloth.windOffset.x = strength * direction.x;
                cloth.field9c = strength;
                cloth.windOffset.z = strength * direction.z;
            }
        } else {
            const float effective = GetEffectiveWindSpeed(GetWind());
            const float speed = (0.08f < effective ? 0.08f : effective) * 0.5f;
            cloth.windStrength = ((ComputeWindOscillationNoise(cloth.oscillation) * speed + speed) - cloth.windStrength) * 0.5f + cloth.windStrength;
            const std::uint16_t phase = static_cast<std::uint16_t>(GetCurrentWindPhaseAccumulator(GetWind()));
            const float strength = cloth.windStrength;
            cloth.windOffset.x = sin_fixed16(static_cast<std::int16_t>(phase)) * strength;
            cloth.windOffset.z = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(phase))) * strength;
            cloth.oscillation = static_cast<std::int32_t>(static_cast<std::uint32_t>(cloth.oscillation) + CLOTH_OSCILLATION_STEP);
            cloth.oscillationRotation = static_cast<std::int32_t>(static_cast<std::uint32_t>(cloth.oscillationRotation) + CLOTH_OSCILLATION_ROTATION_STEP);
            cloth.field9c = strength;
            if (cloth.name.name != kun.name) {
                cloth.field0a = 0;
                return;
            }
            cloth.windStrength = 0.0f;
        }
    }
    if (cloth.name.name == kun.name) {
        std::byte *const parameters = cloth.parameters;
        const std::uint16_t animation = mtwkGetAnimationId(cloth.motionWork, MotionRegion::LowerBody);
        if (g_currentMap.name ==
            SHEN_NAME(AKS1) || (g_currentMap == AREA_WT00 && (static_cast<std::uint32_t>(animation) - 0xe3b2U <= 2U || animation == 0xe299U))) {
            parameters[0x16] = std::byte{1};
            parameters[0x17] = std::byte{3};
            parameters[0x18] = std::byte{2};
            parameters[0x19] = std::byte{1};
            cloth.mode |= 0x300U;
        } else if (cloth.assignMode == 0) {
            parameters[0x16] = std::byte{cloth.savedParameters[0]};
            parameters[0x17] = std::byte{cloth.savedParameters[1]};
            parameters[0x19] = std::byte{cloth.savedParameters[3]};
            parameters[0x18] = std::byte{cloth.savedParameters[2]};
            cloth.mode = static_cast<std::uint16_t>((cloth.mode & 0x00ffU) | (static_cast<std::uint8_t>(parameters[0x1a]) << 8));
        }
    }
    cloth.field0a = 0;
}

} // namespace model_component_detail

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::int64_t RecursivePrepareChildMatrices(MT7Node *model, MT7Node *node) {
    ClothObject *const object = static_cast<ClothObject *>(node->taskData);
    const bool admitted = object && object->token.name == CLOTH_OBJECT.name && object->owner && !(object->flags & 0x20U) && !(object->owner->mode & 0x20U);
    Matrix4x4_Push(nullptr);
    if (admitted) {
        clmdUpdateModelNode(model, node);
        object->owner->previousMode = static_cast<std::uint8_t>(object->owner->mode);
    }
    if (node->transformOverride)
        Matrix4x4::SetActiveTransform(node->transformOverride);
    else {
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        if (node->rotation.z)
            Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node->rotation.z));
        if (node->rotation.y)
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node->rotation.y));
        if (node->rotation.x)
            Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node->rotation.x));
    }
    if (node->runtimeChild) {
        MT7Node *const child = node->runtimeChild;
        ClothObject *const childObject = static_cast<ClothObject *>(child->taskData);
        if (childObject && childObject->token.name == CLOTH_OBJECT.name)
            Matrix4x4::GetTransformMatrix(&childObject->transform);
        if (child->runtimeSibling)
            clmdPublishSiblingMatrices(child->runtimeSibling);
        RecursivePrepareChildMatrices(model, child);
    }
    Matrix4x4::Pop(1);
    if (node->runtimeSibling)
        RecursivePrepareChildMatrices(model, node->runtimeSibling);
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void clmdPublishSiblingMatrices(MT7Node *node) {
    ClothObject *const object = static_cast<ClothObject *>(node->taskData);
    if (object && object->token == MEMORY_CLOTH_OBJECT)
        matrix_stack::GetMatrix(&object->transform);
    if (node->sibling)
        clmdPublishSiblingMatrices(node->sibling);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void clmdUtil(Character *character, std::uint8_t mode) {
    if (!character->model)
        return;
    ClothComponent *const cloth = static_cast<ClothComponent *>(charFindComp(character, CLOTH_COMPONENT));
    if (!cloth)
        return;
    cloth->assignMode = mode;
    std::byte *const parameters = cloth->parameters;
    cloth->windOffset.x = cloth->windOffset.y = cloth->windOffset.z = 0.0f;
    CLMD(character, 0);
    parameters[0x16] = static_cast<std::byte>(cloth->savedParameters[0]);
    parameters[0x17] = static_cast<std::byte>(cloth->savedParameters[1]);
    parameters[0x18] = static_cast<std::byte>(cloth->savedParameters[2]);
    cloth->mode &= 0x00ffU;
    cloth->mode |= static_cast<std::uint8_t>(cloth->parameters[0x1a]) << 8;
    for (int index = 0; index < cloth->clothCount; ++index) {
        ClothObject *const object = cloth->cloths[index];
        ClothVertexState *vertex = object->vertices + object->columns;
        for (int row = 1; row < object->rows; ++row)
            for (int column = 0; column < object->columns; ++column, ++vertex)
                vertex->moved = 0;
    }
    switch (mode) {
    case 1:
        ReadClothCapsules(cloth, reinterpret_cast<const std::byte *>(clothBonesMode1));
        cloth->mode |= 0x200U;
        cloth->mode |= 0x100U;
        clothAnkleOffset = -0.06f;
        return;
    case 3:
        CLMD(character, 1);
        cloth->mode |= 0x200U;
        return;
    case 4:
        parameters[0x16] = 3;
        parameters[0x17] = 4;
        parameters[0x18] = 4;
        cloth->mode |= 0x100U;
        return;
    case 5:
    case 13:
        parameters[0x16] = 3;
        parameters[0x17] = 2;
        parameters[0x18] = 3;
        ReadClothCapsules(cloth, reinterpret_cast<const std::byte *>(clothBonesMode5));
        cloth->mode |= 0x100U;
        return;
    case 6:
        cloth->windOffset.y = 0.5f;
        return;
    case 7:
        cloth->mode |= 0x100U;
        parameters[0x16] = 3;
        parameters[0x18] = 2;
        cloth->windOffset.y = 0.6f;
        return;
    case 8:
        parameters[0x16] = 3;
        parameters[0x17] = 2;
        parameters[0x18] = 2;
        cloth->windOffset.y = 0.5f;
        ReadClothCapsules(cloth, reinterpret_cast<const std::byte *>(clothBonesMode8));
        cloth->mode |= 0x100U;
        return;
    case 9:
        parameters[0x16] = 3;
        parameters[0x17] = 1;
        return;
    case 10:
        parameters[0x16] = 3;
        parameters[0x17] = 1;
        ReadClothCapsules(cloth, reinterpret_cast<const std::byte *>(clothBonesMode10));
        cloth->mode |= 0x100U;
        return;
    case 11:
        cloth->windOffset.y = 0.15f;
        return;
    case 12:
        cloth->mode |= 0x100U;
        CLMD(character, 1);
        return;
    case 14:
        parameters[0x16] = 3;
        parameters[0x17] = 4;
        cloth->mode |= 0x100U;
        cloth->mode |= 0x200U;
        cloth->windOffset.y = 0.3f;
        ReadClothCapsules(cloth, reinterpret_cast<const std::byte *>(clothBonesMode14));
        return;
    case 15:
        parameters[0x16] = 3;
        parameters[0x17] = 4;
        parameters[0x18] = 4;
        cloth->mode |= 0x100U;
        cloth->mode |= 0x200U;
        cloth->windOffset.y = 0.5f;
        ReadClothCapsules(cloth, reinterpret_cast<const std::byte *>(clothBonesMode15));
        return;
    case 16:
        parameters[0x16] = 3;
        clothAnkleOffset = 0.32f;
        cloth->assignMode = 1;
        cloth->mode |= 0x100U;
        return;
    case 17:
        parameters[0x16] = 3;
        parameters[0x17] = 1;
        ReadClothCapsules(cloth, reinterpret_cast<const std::byte *>(clothBonesMode17));
        cloth->mode |= 0x100U;
        cloth->mode |= 0x200U;
        return;
    case 18:
        parameters[0x16] = 5;
        SetOsageMode(character, 1);
        return;
    case 19:
        CLMD(character, 1);
        cloth->assignMode = 2;
        return;
    case 20:
        ReadClothCapsules(cloth, reinterpret_cast<const std::byte *>(clothBonesMode20));
        parameters[0x16] = 3;
        parameters[0x17] = 1;
        parameters[0x18] = 4;
        cloth->mode |= 0x200U;
        cloth->windOffset.y = 0.6f;
        return;
    case 21:
        cloth->mode |= 0x400U;
        return;
    case 22:
        cloth->mode |= 0x100U;
        cloth->windOffset.y = 0.15f;
        return;
    default:
        return;
    }
}

#else

// @verified - p
void clmdUtil(Character *character, std::int8_t mode) {
    if (!character->model)
        return;
    auto *const cloth = static_cast<ClothComponent *>(FindModelComponent(character, kClothToken));
    if (!cloth)
        return;
    std::byte *const parameters = cloth->parameters;
    cloth->field8c = 0.0f;
    cloth->field90 = 0;
    cloth->field88 = 0;
    cloth->assignMode = mode;
    CLMD(character, 0);
    parameters[0x16] = std::byte{cloth->savedParameters[0]};
    parameters[0x17] = std::byte{cloth->savedParameters[1]};
    parameters[0x18] = std::byte{cloth->savedParameters[2]};
    cloth->mode = static_cast<std::uint16_t>((cloth->mode & 0x00ffU) | (static_cast<std::uint8_t>(cloth->parameters[0x1a]) << 8));
    cloth->field98 = 0.7f;
    for (std::uint8_t index = 0; index < cloth->clothCount; ++index) {
        const ClothObject *const object = cloth->cloths[index];
        ClothVertexState *vertex = object->vertices + object->columns;
        for (std::int32_t row = 1; row < object->rows; ++row) {
            for (std::int32_t column = 0; column < object->columns; ++column, ++vertex)
                vertex->moved = 0;
        }
    }
    std::uint16_t modeFlags = 0x100U;
    switch (mode) {
    case 1:
        parameters[0x16] = std::byte{3};
        CLCB(*cloth, reinterpret_cast<const std::byte *>(clothBonesMode1.data()));
        clothAnkleOffset = -0.06f;
        modeFlags = 0x300U;
        break;
    case 3:
        parameters[0x16] = std::byte{1};
        parameters[0x17] = std::byte{0};
        parameters[0x18] = std::byte{0};
        CLMD(character, 1);
        modeFlags = 0x200U;
        break;
    case 4:
        parameters[0x16] = std::byte{3};
        parameters[0x17] = std::byte{4};
        parameters[0x18] = std::byte{4};
        break;
    case 5:
    case 13:
        parameters[0x16] = std::byte{3};
        parameters[0x17] = std::byte{2};
        parameters[0x18] = std::byte{3};
        CLCB(*cloth, reinterpret_cast<const std::byte *>(clothBonesMode5.data()));
        break;
    case 6:
        cloth->field8c = 0.5f;
        return;
    case 7:
        cloth->mode |= 0x100U;
        parameters[0x16] = std::byte{3};
        parameters[0x18] = std::byte{2};
        cloth->field8c = 0.6f;
        return;
    case 8:
        parameters[0x16] = std::byte{3};
        parameters[0x17] = std::byte{2};
        parameters[0x18] = std::byte{2};
        cloth->field8c = 0.5f;
        CLCB(*cloth, reinterpret_cast<const std::byte *>(clothBonesMode8.data()));
        break;
    case 9:
        parameters[0x16] = std::byte{3};
        parameters[0x17] = std::byte{1};
        return;
    case 10:
        parameters[0x16] = std::byte{3};
        parameters[0x17] = std::byte{1};
        CLCB(*cloth, reinterpret_cast<const std::byte *>(clothBonesMode10.data()));
        break;
    case 11:
        cloth->field8c = 0.15f;
        return;
    case 12:
        cloth->mode |= 0x100U;
        CLMD(character, 1);
        return;
    case 14:
        parameters[0x16] = std::byte{3};
        parameters[0x17] = std::byte{1};
        cloth->field8c = 0.3f;
        cloth->mode |= 0x300U;
        CLCB(*cloth, reinterpret_cast<const std::byte *>(clothBonesMode14.data()));
        return;
    case 15:
        parameters[0x16] = std::byte{3};
        parameters[0x17] = std::byte{4};
        parameters[0x18] = std::byte{4};
        cloth->mode |= 0x300U;
        cloth->field8c = 0.5f;
        CLCB(*cloth, reinterpret_cast<const std::byte *>(clothBonesMode15.data()));
        return;
    case 16:
        parameters[0x16] = std::byte{3};
        clothAnkleOffset = 0.32f;
        cloth->assignMode = 1;
        break;
    case 17:
        parameters[0x16] = std::byte{3};
        parameters[0x17] = std::byte{1};
        CLCB(*cloth, reinterpret_cast<const std::byte *>(clothBonesMode17.data()));
        cloth->field98 = 0.25f;
        modeFlags = 0x300U;
        break;
    case 18:
        parameters[0x16] = std::byte{5};
        SetCharacterOsageMode(character, 1);
        return;
    case 19:
        CLMD(character, 1);
        cloth->assignMode = 2;
        return;
    case 20:
        CLCB(*cloth, reinterpret_cast<const std::byte *>(clothBonesMode20.data()));
        parameters[0x16] = std::byte{3};
        parameters[0x17] = std::byte{1};
        parameters[0x18] = std::byte{4};
        cloth->field8c = 0.6f;
        modeFlags = 0x200U;
        break;
    case 21:
        modeFlags = 0x400U;
        break;
    case 22:
        cloth->field8c = 0.15f;
        break;
    case 23:
        CLMD(character, 2);
        return;
    case 24:
        CLMD(character, 1);
        parameters[0x18] = std::byte{4};
        return;
    case 25:
        CLMD(character, 1);
        return;
    case 26:
        CLCB(*cloth, reinterpret_cast<const std::byte *>(clothBonesMode26.data()));
        break;
    case 27:
    case 28:
        CLMD(character, mode == 27 ? 2 : 1);
        cloth->mode |= 0x100U;
        parameters[0x16] = std::byte{3};
        return;
    case 29:
        parameters[0x17] = std::byte{1};
        parameters[0x18] = std::byte{2};
        parameters[0x16] = std::byte{3};
        break;
    default:
        return;
    }
    cloth->mode |= modeFlags;
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
void SetClothFlag(Character *character, std::uint8_t flags, std::int32_t enabled) {
    if (!character->model)
        return;
    auto *const cloth = static_cast<ClothComponent *>(FindModelComponent(character, kClothToken));
    if (!cloth)
        return;
    if (enabled)
        cloth->mode |= flags;
    else
        cloth->mode &= static_cast<std::uint16_t>(~static_cast<std::uint32_t>(flags));
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void clmdInitializeGrid(ClothObject &object) {
    ClothVertexBinding *binding = object.bindings;
    ClothVertexBinding *previous = object.previousBindings;
    ClothVertexState *vertex = object.vertices;
    for (std::int32_t i = 0; i < object.vertexCount; ++i, ++vertex, ++binding, ++previous) {
        vertex->current = binding;
        vertex->previous = previous;
    }
    vertex = object.vertices;
    const float *parameters = reinterpret_cast<const float *>(object.descriptorData10);
    const std::uint16_t wrap = object.flags & 0x80U;
    for (std::int32_t row = 0; row < object.rows; ++row) {
        for (std::int32_t column = 0; column < object.columns; ++column, ++vertex) {
            if (row != object.rows - 1)
                vertex->below = vertex + object.columns;
            if (!row)
                continue;
            vertex->above = vertex - object.columns;
            vertex->velocity.x = parameters[0];
            vertex->velocity.y = parameters[1];
            vertex->velocity.z = parameters[2];
            vertex->aboveRestLength = parameters[3] * 10.0f;
            if (column) {
                vertex->left = vertex - 1;
                vertex->leftRestLength = parameters[4] * 10.0f;
            } else if (wrap) {
                vertex->left = vertex + (object.columns - 1);
                vertex->leftRestLength = parameters[4] * 10.0f;
            }
            if (column != object.columns - 1) {
                vertex->right = vertex + 1;
                vertex->rightRestLength = parameters[5] * 10.0f;
            } else if (wrap) {
                vertex->right = vertex - (object.columns - 1);
                vertex->rightRestLength = parameters[5] * 10.0f;
            }
            parameters += 6;
        }
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

float clmdDistanceSquared(const Vector3 &first, const Vector3 &second) {
    const float y = first.y - second.y, x = first.x - second.x, z = first.z - second.z;
    return (y * y + x * x) + z * z;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void clmdUpdateDynamics(ClothObject &object) {
    Vector3 firstForce, secondForce, reference, root, paired, first, second, normal, offset;
    float forceScale = 0.054444447f;
    ClothVertexState *vertex;
    ClothVertexBinding *binding;
    std::int32_t row;
    MotionWork *const work = object.owner->motionWork;
    const std::byte *const parameters = object.owner->parameters;
    std::int32_t referenceBone = 1;
    switch (static_cast<std::uint8_t>(parameters[22])) {
    case 3:
        mtwkGetBonePosition(work, 24, &root);
        mtwkGetBonePosition(work, 29, &paired);
        break;
    case 5:
        referenceBone = 3;
    case 2:
        mtwkGetBonePosition(work, 23, &root);
        mtwkGetBonePosition(work, 28, &paired);
        break;
    case 4:
        mtwkGetBonePosition(work, 0, &root);
        root.y -= 2.0f;
        paired.x = root.x;
        paired.y = root.y;
        paired.z = root.z;
        break;
    case 1:
    default:
        mtwkGetBonePosition(work, 26, &root);
        mtwkGetBonePosition(work, 31, &paired);
        break;
    }
    root.x += (paired.x - root.x) * 0.5f;
    root.y += (paired.y - root.y) * 0.5f;
    root.z += (paired.z - root.z) * 0.5f;
    mtwkGetBonePosition(work, static_cast<MotionBoneId>(referenceBone), &reference);
    float length = GetEuclideanDistance(&root, &reference);
    if (std::fabs(length) < 0.00000011920929f) {
        firstForce.x = firstForce.y = firstForce.z = 0.0f;
    } else {
        const float ratio = forceScale / length;
        firstForce.x = (root.x - reference.x) * ratio;
        firstForce.y = (root.y - reference.y) * ratio;
        firstForce.z = ratio * (root.z - reference.z);
    }
    secondForce.x = secondForce.y = secondForce.z = 0.0f;
    const std::uint8_t secondKind = static_cast<std::uint8_t>(parameters[23]);
    if (secondKind) {
        switch (secondKind) {
        case 3:
            mtwkGetBonePosition(work, 24, &first);
            mtwkGetBonePosition(work, 29, &second);
            break;
        case 2:
            mtwkGetBonePosition(work, 23, &first);
            mtwkGetBonePosition(work, 28, &second);
            break;
        case 4:
            mtwkGetBonePosition(work, 0, &first);
            first.y -= 2.0f;
            second.x = first.x;
            second.y = first.y;
            second.z = first.z;
            break;
        case 1:
            mtwkGetBonePosition(work, 26, &first);
            mtwkGetBonePosition(work, 31, &second);
            break;
        }
        first.x += (second.x - first.x) * 0.5f;
        first.y += (second.y - first.y) * 0.5f;
        first.z += (second.z - first.z) * 0.5f;
        length = GetEuclideanDistance(&first, &reference);
        if (!(std::fabs(length) < 0.00000011920929f)) {
            Vector3 *const out = &secondForce;
            const float ratio = forceScale / length;
            out->x = (first.x - reference.x) * ratio;
            out->y = (first.y - reference.y) * ratio;
            out->z = ratio * (first.z - reference.z);
        }
    }
    if (object.owner->assignMode == 17) {
        firstForce.y -= 0.0099999998f;
        secondForce.y -= 0.02f;
    }
    if (object.owner->mode & 0x100U) {
        matrix_stack::Matrix4x4_Push();
        matrix_stack::LoadMatrix(&object.transform);
        second.x = 0.0f;
        second.y = -1.0f;
        second.z = 0.0f;
        Matrix4x4::Transform(second, first);
        second = first;
        vertex = object.vertices + object.columns;
        for (std::uint32_t level = 1; level < object.rows; ++level) {
            if (!parameters[23] || level == 1) {
                first = firstForce;
                am2Vector3NormalizeInPlace(&first);
                matrix_stack::UnitCurrentMatrix();
                Matrix4x4_BuildRotationMatrix(second, first);
            } else if (level == static_cast<std::uint8_t>(parameters[24])) {
                first = secondForce;
                am2Vector3NormalizeInPlace(&first);
                matrix_stack::UnitCurrentMatrix();
                Matrix4x4_BuildRotationMatrix(second, first);
            }
            for (std::uint32_t column = 0; column < object.columns; ++column, ++vertex) {
                ClothVertexBinding *const binding = vertex->current;
                binding->normalEndpoint.x -= binding->position.x;
                binding->normalEndpoint.y -= binding->position.y + static_cast<float>(level) * 0.1f;
                binding->normalEndpoint.z -= binding->position.z;
                am2Vector3NormalizeInPlace(&binding->normalEndpoint);
                Matrix4x4::Transform(binding->normalEndpoint, normal);
                binding->normalEndpoint.x = normal.x + binding->position.x;
                binding->normalEndpoint.y = normal.y + binding->position.y;
                binding->normalEndpoint.z = normal.z + binding->position.z;
            }
        }
        PopCurrentMatrix(1);
    }
    vertex = object.vertices;
    binding = vertex->current;
    const float rootDistance = am2Vector3DistanceSquared(&binding->position, &vertex->previous->position);
    if (rootDistance > 0.34999999f) {
        object.flags |= 0x40U;
        if (object.owner->character == SHEN_NAME4('S', 'R', 'Y', '_') && object.owner->assignMode == 2 && rootDistance < 1.0499999523162842f)
            object.flags &= ~0x40U;
    }
    clmdSolveRows(object, firstForce, secondForce);
    ClothComponent *const owner = object.owner;
    std::uint8_t threshold = 13;
    if (!(object.flags & 0x80U))
        threshold = 6;
    if (owner->assignMode == 2)
        threshold = 0;
    float scale = 1.0f / static_cast<float>(object.rows);
    if (object.columns < threshold)
        scale *= static_cast<float>(object.columns) / static_cast<float>(static_cast<std::int32_t>(threshold));
    vertex = object.vertices + object.columns;
    for (row = 1; row < object.rows; ++row) {
        const float amplitude = scale * static_cast<float>(row);
        for (std::int32_t column = 0; column < object.columns; ++column, ++vertex) {
            binding = vertex->current;
            const float sine = CalculateFixedSine(owner->oscillationRotation + -clothWavePhaseStep * column);
            offset.x = (binding->normalEndpoint.x - binding->position.x) * owner->windStrength * sine;
            offset.z = (binding->normalEndpoint.z - binding->position.z) * owner->windStrength * sine;
            binding->position.x += offset.x * amplitude;
            binding->position.z += offset.z * amplitude;
        }
    }
    if (object.flags & 0x40U)
        object.flags &= ~0x40U;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void clmdUpdateDamping(ClothObject &object, ClothVertexState &vertex, Vector3 &velocity) {
    if (vertex.collisionIndex != 0xffU || vertex.moved > 0)
        return;
    const ClothComponent &owner = *object.owner;
    if (owner.windStrength > 0.0000001f) {
        const ClothVertexBinding &binding = *vertex.current;
        Vector3 facing = Vector3::Subtract(binding.normalEndpoint, binding.position);
        dc::sdk::njUnitVector(&facing);
        const float reciprocal = 1.0f / owner.windStrength;
        const Vector3 wind = Vector3::Scale(owner.windVelocity, reciprocal);
        const float scale = ((facing.x * wind.x + facing.y * wind.y) + facing.z * wind.z + 1.0f) * 0.5f;
        velocity.x = scale * owner.windVelocity.x + velocity.x;
        velocity.z = scale * owner.windVelocity.z + velocity.z;
    }
    if (object.flags & 0x80U)
        velocity.y *= 0.5f;
    else {
        static const float damping[4][3] = {{0.5f, 0.5f, 0.5f}, {0.8f, 0.65f, 0.8f}, {0.2f, 0.1f, 0.2f}, {1.0f, 1.0f, 1.0f}};
        const float *entry = damping[static_cast<std::uint8_t>(owner.parameters[25])];
        velocity.x *= entry[0];
        velocity.y *= entry[1];
        velocity.z *= entry[2];
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void clmdResolveBoneCollision(ClothObject &object, Vector3 &point, ClothVertexState &vertex) {
    vertex.collisionIndex = 0xffU;
    const ClothBoneCapsule *capsules = static_cast<ClothBoneCapsule *>(object.boneBuffer);
    for (std::uint8_t i = 0; i < object.boneRecordCount; ++i) {
        const ClothBoneCapsule &capsule = capsules[i];
        const float radiusSquared = capsule.radius * capsule.radius;
        const float squared = clmdDistanceSquared(point, capsule.position);
        if (squared > radiusSquared)
            continue;
        if (object.owner->mode & 0x400U) {
            const float limit = radiusSquared * 0.9f;
            if (squared > limit)
                continue;
            const float scale = std::sqrt(limit / squared);
            point = Vec((point.x - capsule.position.x) * scale + capsule.position.x, (point.y - capsule.position.y) * scale + capsule.position.y,
                (point.z - capsule.position.z) * scale + capsule.position.z);
        } else {
            vertex.collisionIndex = i;
            const Vector3 direction = Vector3::Subtract(vertex.current->normalEndpoint, point);
            const Vector3 offset = Vector3::Subtract(point, capsule.position);
            const float directionSquared = (direction.x * direction.x + direction.y * direction.y) + direction.z * direction.z;
            const float twiceDot = 2.0f * ((direction.x * offset.x + direction.y * offset.y) + direction.z * offset.z);
            const float discriminant = twiceDot * twiceDot - 4.0f * directionSquared * (squared - radiusSquared);
            if (discriminant > 0.0f) {
                const float root = std::sqrt(discriminant);
                float distance = (root - twiceDot) / (directionSquared + directionSquared);
                if (!(distance > 0.0f))
                    distance = (-twiceDot - root) / (directionSquared + directionSquared);
                point = Vec(direction.x * distance + point.x, direction.y * distance + point.y, direction.z * distance + point.z);
            }
        }
        clmdClampRestLength(vertex.above->current->position, point, vertex.aboveRestLength, point);
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void clmdLiftBelowAnkles(ClothObject &object) {
    Vector3 position;
    mtwkGetBonePosition(object.owner->motionWork, 26, &position);
    float height = position.y;
    mtwkGetBonePosition(object.owner->motionWork, 31, &position);
    if (height > position.y)
        height = position.y;
    height += clothAnkleOffset;
    ClothVertexState *vertex = object.vertices + object.columns;
    for (std::uint8_t row = 1; row < object.rows; ++row) {
        for (std::uint8_t column = 0; column < object.columns; ++column, ++vertex) {
            if (height > vertex->current->position.y) {
                vertex->current->position.y = (vertex->current->position.y - height) * 0.0099999998f + height;
                vertex->moved = 1;
            }
        }
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void clmdOrientNormals(ClothObject &object) {
    const ClothVertexState *const vertex = &object.vertices[object.columns];
    Vector3 normal, direction;
    const ClothVertexBinding *first;
    const ClothVertexBinding *const middle = vertex->current;
    const ClothVertexState *other;
    if (vertex->left) {
        first = vertex->left->current;
        other = vertex->above;
    } else {
        first = vertex->above->current;
        other = vertex->right;
    }
    const ClothVertexBinding *const third = other->current;
    clmdTriangleNormal(&first->position, &middle->position, &third->position, &normal);
    const ClothVertexBinding *const above = vertex->above->current;
    direction.x = above->normalEndpoint.x - above->position.x;
    direction.y = above->normalEndpoint.y - above->position.y;
    direction.z = above->normalEndpoint.z - above->position.z;
    if (0.0f > am2Vector3Dot(&direction, &normal))
        object.flags |= 0x100U;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void clmdUpdateNormals(ClothObject &object) {
    ClothVertexState *vertex = object.vertices + object.columns;
    for (std::uint8_t row = 1; row < object.rows; ++row) {
        for (std::uint8_t column = 0; column < object.columns; ++column, ++vertex) {
            const ClothVertexBinding &first = vertex->left ? *vertex->left->current : *vertex->above->current;
            const ClothVertexBinding &third = vertex->left ? *vertex->above->current : *vertex->right->current;
            Vector3 normal;
            clmdTriangleNormal(&first.position, &vertex->current->position, &third.position, &normal);
            if (object.flags & 0x100U) {
                normal.x *= -1.0f;
                normal.y *= -1.0f;
                normal.z *= -1.0f;
            }
            vertex->current->normalEndpoint =
                Vec(normal.x + vertex->current->position.x, normal.y + vertex->current->position.y, normal.z + vertex->current->position.z);
        }
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void clmdClampRestLength(const Vector3 &anchor, const Vector3 &point, float length, Vector3 &output) {
    const float squared = clmdDistanceSquared(anchor, point);
    if (!(squared < 0.0000001f)) {
        const float ratio = (1.0f / std::sqrt(squared)) * length;
        output = Vec((point.x - anchor.x) * ratio + anchor.x, (point.y - anchor.y) * ratio + anchor.y, (point.z - anchor.z) * ratio + anchor.z);
    } else
        output = anchor;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void clmdConstrainRow(ClothObject &object, ClothVertexState *end, std::int32_t row) {
    ClothVertexState *const first = end - object.columns;
    if (!(object.flags & 0x80U)) {
        const std::uint16_t pivot = object.bindingIndices[row];
        if (pivot) {
            for (std::int32_t column = pivot - 1; column >= 0; --column) {
                ClothVertexState &vertex = first[column];
                clmdClampRestLength(vertex.right->current->position, vertex.current->position, vertex.rightRestLength, vertex.current->position);
            }
        }
        if (pivot != object.columns) {
            for (std::int32_t column = pivot + 1; column < object.columns; ++column) {
                ClothVertexState &vertex = first[column];
                clmdClampRestLength(vertex.left->current->position, vertex.current->position, vertex.leftRestLength, vertex.current->position);
            }
        }
    } else {
        float circumference = 0.0f;
        for (std::uint8_t column = 0; column < object.columns; ++column)
            circumference += std::sqrt(clmdDistanceSquared(first[column].current->position, first[column].right->current->position));
        const float diameter = object.owner->minimumCapsuleRadius + object.owner->minimumCapsuleRadius;
        const float average = circumference / static_cast<float>(object.columns);
        float length = diameter < average ? diameter : average;
        for (std::uint8_t column = 0; column < object.columns; ++column) {
            ClothVertexState &vertex = first[column];
            if (vertex.moved)
                continue;
            if (object.owner->mode & 0x200U)
                length = vertex.leftRestLength;
            clmdClampRestLength(vertex.left->current->position, vertex.current->position, length, clothLeftConstraint);
            if (object.owner->mode & 0x200U)
                length = vertex.rightRestLength;
            clmdClampRestLength(vertex.right->current->position, vertex.current->position, length, clothRightConstraint);
            vertex.current->position = Vec((clothRightConstraint.x + clothLeftConstraint.x) * 0.5f, (clothRightConstraint.y + clothLeftConstraint.y) * 0.5f,
                (clothRightConstraint.z + clothLeftConstraint.z) * 0.5f);
        }
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void clmdIntegrateVertex(ClothObject &object, ClothVertexState &vertex, Vector3 &velocity, const Vector3 &firstForce, const Vector3 &secondForce,
    std::int32_t columnsRemaining, std::int32_t rowsRemaining, Vector3 &point) {
    const ClothVertexBinding &above = *vertex.above->current;
    const ClothVertexBinding *previous = &above;
    if (object.flags & 0x40U)
        velocity = Vec(0, 0, 0);
    else
        previous = vertex.previous;
    const ClothComponent &owner = *object.owner;
    const std::int32_t row = object.rows - rowsRemaining, column = object.columns - columnsRemaining;
    const Vector3 &force = static_cast<std::uint8_t>(owner.parameters[23]) > 0 && row >= static_cast<std::uint8_t>(owner.parameters[24]) &&
            (owner.assignMode != 4 || row >= 7 || column >= 7 || column <= 0) ?
        secondForce :
        firstForce;
    point = Vec((previous->position.x + velocity.x) + force.x, (velocity.y + previous->position.y) + force.y, (velocity.z + previous->position.z) + force.z);
    vertex.current->normalEndpoint.x = owner.windOffset.x + vertex.current->normalEndpoint.x;
    vertex.current->normalEndpoint.y = owner.windOffset.y + vertex.current->normalEndpoint.y;
    vertex.current->normalEndpoint.z = owner.windOffset.z + vertex.current->normalEndpoint.z;
    clmdClampRestLength(above.position, point, vertex.aboveRestLength, point);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void clmdSolveRows(ClothObject &object, const Vector3 &firstForce, const Vector3 &secondForce) {
    ClothVertexState *vertex = object.vertices + object.columns;
    for (std::int32_t row = object.rows - 1; row > 0; --row) {
        for (std::int32_t column = object.columns; column > 0; --column, ++vertex) {
            const ClothVertexBinding &above = *vertex->above->current;
            const ClothVertexBinding &previous = *vertex->above->previous;
            clothVelocity = Vec(above.position.x - previous.position.x, above.position.y - previous.position.y, above.position.z - previous.position.z);
            clmdUpdateDamping(object, *vertex, clothVelocity);
            Vector3 point;
            clmdIntegrateVertex(object, *vertex, clothVelocity, firstForce, secondForce, column, row, point);
            clmdResolveBoneCollision(object, point, *vertex);
            vertex->current->position = point;
        }
        clmdConstrainRow(object, vertex, row);
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void clmdCopyLinkedBindings(ClothObject &object) {
    const ClothVertexBinding *source = object.bindings;
    ClothVertexBinding *destination = object.link->bindings;
    for (std::int32_t i = 0; i < object.vertexCount; ++i, ++source, ++destination) {
        destination->position.x = source->position.x;
        destination->position.y = source->position.y;
        destination->position.z = source->position.z;
        destination->normalEndpoint.x = destination->position.x - (source->normalEndpoint.x - source->position.x);
        destination->normalEndpoint.y = destination->position.y - (source->normalEndpoint.y - source->position.y);
        destination->normalEndpoint.z = destination->position.z - (source->normalEndpoint.z - source->position.z);
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void clmdInitializePins(ClothObject &object) {
    const std::byte *const parameters = object.owner->parameters;
    ClothVertexState *vertex = object.vertices;
    std::uint8_t row;
    for (std::uint8_t column = 0; column < object.columns; ++column, ++vertex)
        vertex->flags = 0x10U;
    const std::uint16_t columns = object.columns;
    switch (static_cast<std::uint8_t>(parameters[20])) {
    case 1:
        vertex = &object.vertices[columns];
        for (row = 1; row < object.rows; vertex += columns, ++row)
            vertex->flags = static_cast<std::uint8_t>((row - 1) | 0x20U);
        break;
    case 2:
        vertex = object.vertices + columns * 2 - 1;
        for (row = 1; row < object.rows; vertex += columns, ++row)
            vertex->flags = static_cast<std::uint8_t>((row - 1) | 0x20U);
        break;
    }
    std::int16_t pivot;
    switch (static_cast<std::uint8_t>(parameters[21])) {
    case 255:
        pivot = columns - 1;
        break;
    case 254:
        pivot = columns >> 1;
        break;
    default:
        pivot = static_cast<std::uint8_t>(parameters[21]);
        break;
    }
    for (row = 0; row < object.rows; ++row)
        object.bindingIndices[row] = pivot;
    object.descriptorIndex = 0xffffU;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

std::int32_t clmdTriangleNormal(const Vector3 *first, const Vector3 *second, const Vector3 *third, Vector3 *normal) {
    const Vector3 edge = Vec(second->x - first->x, second->y - first->y, second->z - first->z);
    const Vector3 next = Vec(third->x - second->x, third->y - second->y, third->z - second->z);
    *normal = Vec(next.z * edge.y - next.y * edge.z, next.x * edge.z - next.z * edge.x, next.y * edge.x - next.x * edge.y);
    const float squared = (normal->y * normal->y + normal->x * normal->x) + normal->z * normal->z;
    if (squared == 0.0f)
        return 0;
    const float reciprocal = 1.0f / std::sqrt(squared);
    normal->x *= reciprocal;
    normal->y *= reciprocal;
    normal->z *= reciprocal;
    return 1;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void clmdFindColumnLinks(ClothObject &object) {
    float scores[30];
    std::uint8_t i;
    for (i = 0; i < object.columns; ++i)
        scores[i] = 0.0f;
    std::byte *cursor = static_cast<std::byte *>(object.node->parent->mesh) + 0x68;
    for (;;) {
        std::int8_t flags = static_cast<std::int8_t>(*cursor);
        for (;;) {
            cursor += 4;
            std::uint8_t count = static_cast<std::uint8_t>(*cursor);
            cursor += 4;
            if (flags & 8)
                count = static_cast<std::uint8_t>(count * 3);
            for (i = 0; i < count; ++i) {
                if (CLOTH_VALUE(std::uint32_t, cursor, 0) & 1U) {
                    for (std::uint8_t column = 0; column < object.columns; ++column) {
                        ClothVertexBinding *const binding = &object.bindings[column];
                        if (0.000025f > GetSquaredEuclideanDistance(&binding->position, reinterpret_cast<Vector3 *>(cursor))) {
                            const float score = GetVector3DotProduct(binding->normalEndpoint, CLOTH_VALUE(Vector3, cursor, 12));
                            if (score > scores[column]) {
                                object.columnLinks[column] = cursor;
                                scores[column] = score;
                            }
                        }
                    }
                    cursor += 32;
                } else
                    cursor += 8;
            }
            const std::int32_t next = CLOTH_VALUE(const std::int32_t, cursor, 0);
            if (!next)
                return;
            if (next >= 0)
                break;
            cursor += 80;
            flags = static_cast<std::int8_t>(*cursor);
        }
    }
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

static void clmdCopyColumnLinkNormals(ClothObject *object) {
    for (std::uint8_t i = 0; i < object->columns; ++i) {
        if (!object->columnLinks[i])
            continue;
        ClothVertexBinding &binding = object->bindings[i];
        binding.normalEndpoint.x = reinterpret_cast<const float *>(static_cast<std::byte *>(object->columnLinks[i]) + 12)[0];
        binding.normalEndpoint.y = reinterpret_cast<const float *>(static_cast<std::byte *>(object->columnLinks[i]) + 12)[1];
        binding.normalEndpoint.z = reinterpret_cast<const float *>(static_cast<std::byte *>(object->columnLinks[i]) + 12)[2];
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

void clmdUpdateMeshBounds(ClothObject &object) {
    std::int32_t index;
    const ClothVertexState *vertex = object.vertices;
    Vector3 center;
    center.x = center.y = center.z = 0.0f;
    for (index = 0; index < object.columns; ++index, ++vertex) {
        const ClothVertexBinding *const current = vertex->current;
        center.x += current->position.x;
        center.y += current->position.y;
        center.z += current->position.z;
    }
    center.x /= static_cast<float>(object.columns);
    center.y /= static_cast<float>(object.columns);
    center.z /= static_cast<float>(object.columns);
    std::byte *mesh = static_cast<std::byte *>(object.mesh);
    const float radius = CLOTH_VALUE(float, mesh, 20) * 2.0f;
    index = 0;
    for (;;) {
        CLOTH_VALUE(Vector3, mesh, 8) = center;
        CLOTH_VALUE(float, mesh, 20) = radius;
        std::byte *command = mesh + 24;
        do {
            CLOTH_VALUE(Vector3, command, 16) = center;
            CLOTH_VALUE(float, command, 28) = radius;
            command = command + CLOTH_VALUE(std::uint32_t, command, 76) + 80;
        } while (CLOTH_VALUE(std::uint32_t, command, 0));
        if (!object.link || index)
            break;
        mesh = static_cast<std::byte *>(object.link->mesh);
        index = 1;
    }
}

} // namespace

#endif

} // namespace s2
} // namespace shenmue

#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern "C" void __attribute__((renesas)) shenmue_s2_event_clmdUtil(shenmue::s2::Character *character, std::uint8_t mode) {
    shenmue::s2::clmdUtil(character, mode);
}
#else
#endif
