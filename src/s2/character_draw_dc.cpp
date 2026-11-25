#include "s2/character_dc.hpp"
#include "s2/event_matrix_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/model_dc.hpp"
#include "s2/character_model_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/model_transform_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/object_light_dc.hpp"
#include "s2/model_render_dc.hpp"
#include "s2/map_models_dc.hpp"
#include "s2/map_load_dc.hpp"
#include "platform/dc/am2_vector.hpp"
#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"

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
#include "s2/character_pose_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/character_state_flags_dc.hpp"
#include "s2/reflector_board_dc.hpp"
#include "s2/event_character_model_dc.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/cloth_dc.hpp"
#include "s2/osage_dc.hpp"
#include "s2/hand_pose_dc.hpp"
#include "s2/model_image_component_dc.hpp"
#include "s2/motion_node_dc.hpp"
#include "s2/prop_dc.hpp"
#include "s2/scenario_dc.hpp"

namespace shenmue {
namespace s2 {

const Matrix4x4 *GetCharacterMirrorMatrix();

void DrawKnifeModel(const MT7Node *node);
Character *ctrlGetCharaByName(shen_name name);
void SetMapModelAlternateTraversalEnabled(std::uint32_t enabled, std::int32_t index);
Matrix4x4 characterMirrorMatrix;
std::int32_t g_characterMirroredTransformEnabled;

#if SHENMUE_IS_DC && SHENMUE_IS_S2
const shen_name CHARACTER_MIRROR_STORAGE = SHEN_NAME4('M', 'I', 'R', 'M');
#endif

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
Matrix4x4 *characterMirroredControllerMatrices = 0;
#endif

} // namespace

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SetScenarioCharacterDrawControl(shen_name name, std::int32_t mode, std::int32_t argument) {
    Character *character;
    if (name) {
        const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        character = scenFindChara(control->scenarioTask, name);
    } else {
        character = 0;
    }
    switch (mode) {
    case 0:
        SetCharacterDrawControlEnabled(character, 0);
        break;
    case 1:
        SetCharacterDrawControlEnabled(character, 1);
        break;
    case 2:
        SetCharacterDrawControlEnabled(character, argument > 0);
        break;
    case 3:
    default:
        break;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
ReflectorBoardWork *SetScenarioReflectorBoardValues(shen_name name, const Vector4 *values) {
    const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return SetReflectorBoardValues(scenFindChara(control->scenarioTask, name), values);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SetScenarioReflectorBoardMode(shen_name name, std::int32_t sector) {
    const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    ResetReflectorBoardMode(scenFindChara(control->scenarioTask, name), sector);
}
#endif

// @verified - d
void EV_SetMapModelAlternateTraversalEnabled(std::uint32_t enabled, std::int32_t index) {
    SetMapModelAlternateTraversalEnabled(enabled, index);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void EV_SetCharacterMirrorPlane(const CharacterMirrorPlane *plane, const Vector3i *rotation) {
    SetCharacterMirrorPlane(plane, rotation);
}
#endif

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void AllocateCharacterMirrorMatrices(std::int32_t enabled) {
    if (enabled) {
        characterMirroredControllerMatrices = static_cast<Matrix4x4 *>(HLib::AllocateTaskData(0x780, shen_name(SHEN_NAME4('M', 'I', 'R', 'M'))));
    } else if (characterMirroredControllerMatrices) {
        HLib::FreeTaskData(characterMirroredControllerMatrices);
        characterMirroredControllerMatrices = 0;
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void BindMirroredMotionNode(MT7Node *model, std::int16_t id, const Matrix4x4 *matrix, std::int32_t index) {
    MT7Node *const node = FindMT7Node(model, id);
    if (node) {
        matrix_stack::LoadMatrix(GetCharacterMirrorMatrix());
        Matrix4x4::MultiplyWithPrePassMatrix(matrix);
        matrix_stack::GetMatrix(characterMirroredControllerMatrices + index);
        node->transformOverride = characterMirroredControllerMatrices + index;
    }
}
#endif

} // namespace

void ResetCharacterMirrorAdmission() {
    g_characterMirroredTransformEnabled = 0;
    dc::sdk::SetMirroredModelDrawMode(0);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
static void ApplyCharacterMirrorPlane(const CharacterMirrorPlane *plane) {
    Matrix4x4 reflection;
    const float x = plane->normal.x;
    float square = (x + x) * x;
    reflection.rows[0].x = 1.0f - square;
    float &diagonalY = reflection.rows[1].y;
    const float y = plane->normal.y;
    square = (y + y) * y;
    diagonalY = 1.0f - square;
    float &diagonalZ = reflection.rows[2].z;
    const float z = plane->normal.z;
    square = (z + z) * z;
    diagonalZ = 1.0f - square;
    float &xy = reflection.rows[0].y;
    float &yx = reflection.rows[1].x;
    const float negatedDoubleX = x * -2.0f;
    xy = yx = negatedDoubleX * y;
    reflection.rows[0].z = reflection.rows[2].x = negatedDoubleX * z;
    reflection.rows[1].z = reflection.rows[2].y = (y * -2.0f) * z;
    float distance = am2Vector3Dot(&plane->position, &plane->normal);
    distance += distance;
    reflection.rows[3].x = plane->normal.x * distance;
    reflection.rows[3].y = plane->normal.y * distance;
    reflection.rows[3].z = plane->normal.z * distance;
    reflection.rows[0].w = 0.0f;
    reflection.rows[1].w = 0.0f;
    reflection.rows[2].w = 0.0f;
    reflection.rows[3].w = 1.0f;
    Matrix4x4::MultiplyWithPrePassMatrix(&reflection);
}

// @verified - d
void SetCharacterMirrorPlane(const CharacterMirrorPlane *plane, const Vector3i *rotation) {
    matrix_stack::PushUnitMatrix();
    if (!plane) {
        if (g_characterMirroredTransformEnabled)
            AllocateCharacterMirrorMatrices(0);
        g_characterMirroredTransformEnabled = 0;
    } else {
        if (!g_characterMirroredTransformEnabled)
            AllocateCharacterMirrorMatrices(1);
        g_characterMirroredTransformEnabled = 1;
        if (rotation) {
            Matrix4x4::preTranslate(plane->position.x, plane->position.y, plane->position.z);
            Matrix4x4::RotateAroundZ_Wrapper(rotation->z);
            Matrix4x4::RotateAroundY_Wrapper(rotation->y);
            Matrix4x4::RotateAroundX_Wrapper(rotation->x);
            Matrix4x4::preTranslate(-plane->position.x, -plane->position.y, -plane->position.z);
        }
        ApplyCharacterMirrorPlane(plane);
    }
    matrix_stack::GetMatrix(&characterMirrorMatrix);
    PopCurrentMatrix(1);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void mtwkBindMirroredControllerMatricesToModelNodes(MotionWork *work, MT7Node *model) {
    matrix_stack::Matrix4x4_Push(0);
    ClearShadowNodeTransforms(model);
    std::int32_t index = 0;
    BindMirroredMotionNode(model, 1, &reinterpret_cast<MotionWorkBindings *>(work)->rig.chest.upperChestWorld, index++);
    BindMirroredMotionNode(model, -67, &reinterpret_cast<MotionWorkBindings *>(work)->rig.head.headWorld, index++);
    BindMirroredMotionNode(model, 2, &reinterpret_cast<MotionWorkBindings *>(work)->rig.head.neckWorld, index++);
    BindMirroredMotionNode(model, 4, &reinterpret_cast<MotionWorkBindings *>(work)->rig.rightArm.shoulderWorld, index++);
    BindMirroredMotionNode(model, 5, &reinterpret_cast<MotionWorkBindings *>(work)->rig.rightArm.upperArmWorld, index++);
    BindMirroredMotionNode(model, 6, &reinterpret_cast<MotionWorkBindings *>(work)->rig.rightArm.forearmWorld, index++);
    BindMirroredMotionNode(model, -65, &reinterpret_cast<MotionWorkBindings *>(work)->rig.rightArm.handWorld, index++);
    BindMirroredMotionNode(model, 9, &reinterpret_cast<MotionWorkBindings *>(work)->rig.leftArm.shoulderWorld, index++);
    BindMirroredMotionNode(model, 10, &reinterpret_cast<MotionWorkBindings *>(work)->rig.leftArm.upperArmWorld, index++);
    BindMirroredMotionNode(model, 11, &reinterpret_cast<MotionWorkBindings *>(work)->rig.leftArm.forearmWorld, index++);
    BindMirroredMotionNode(model, -66, &reinterpret_cast<MotionWorkBindings *>(work)->rig.leftArm.handWorld, index++);
    BindMirroredMotionNode(model, 14, &reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.hipsWorld, index++);
    BindMirroredMotionNode(model, 16, &reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.leftThighWorld, index++);
    BindMirroredMotionNode(model, 17, &reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.leftShinWorld, index++);
    BindMirroredMotionNode(model, 18, &reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.leftAnkleWorld, index++);
    BindMirroredMotionNode(model, 21, &reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightThighWorld, index++);
    BindMirroredMotionNode(model, 22, &reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightShinWorld, index++);
    BindMirroredMotionNode(model, 23, &reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightAnkleWorld, index++);

    if (!reinterpret_cast<MotionWorkBindings *>(work)->rig.useOverride) {
        BindMirroredMotionNode(model, 0, &reinterpret_cast<MotionWorkBindings *>(work)->rig.chest.chestWorld, index++);
        BindMirroredMotionNode(model, 7, &reinterpret_cast<MotionWorkBindings *>(work)->rig.rightArm.wristWorld, index++);
        BindMirroredMotionNode(model, 12, &reinterpret_cast<MotionWorkBindings *>(work)->rig.leftArm.wristWorld, index++);
    } else {
        BindMirroredMotionNode(model, 0, &reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.pelvisWorld, index++);
        BindMirroredMotionNode(model, 15, reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.leftHipWorld, index++);
        BindMirroredMotionNode(model, 20, reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightHipWorld, index++);
        BindMirroredMotionNode(model, 55, reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightToe1World, index++);
        BindMirroredMotionNode(model, 56, reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightToe2World, index++);
        BindMirroredMotionNode(model, 57, reinterpret_cast<MotionWorkBindings *>(work)->rig.lowerBody.rightToe3World, index++);
        BindMirroredMotionNode(model, -79, reinterpret_cast<MotionWorkBindings *>(work)->rig.head.aimSourceWorld, index++);
        BindMirroredMotionNode(model, 58, reinterpret_cast<MotionWorkBindings *>(work)->rig.head.nodeAWorld, index++);
        BindMirroredMotionNode(model, 59, reinterpret_cast<MotionWorkBindings *>(work)->rig.head.nodeBWorld, index++);
        BindMirroredMotionNode(model, 119, reinterpret_cast<MotionWorkBindings *>(work)->rig.head.nodeCWorld, index++);
    }
    PopCurrentMatrix(1);
}
#endif

// @verified - d
std::int32_t IsCharacterMirroredForDraw(const Character *character) {
    return charaHasMirroredFlags(character) && g_characterMirroredTransformEnabled != 0;
}

// @verified - d
std::int32_t IsMapModelMirrored(std::int32_t index) {
    return g_characterMirroredTransformEnabled && GetMapModelAlternateTraversalEnabled(index);
}

std::int32_t IsCharacterMirrorEnabled() {
    return g_characterMirroredTransformEnabled;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void DrawKnifeCharacter(Character *character) {
    const MT7Node *const model = character->model;
    if (model) {
        matrix_stack::Matrix4x4_Push(0);
        ApplyCharacterNodeTransformAndScale(character);
        DrawKnifeModel(model);
        PopCurrentMatrix(1);
    }
}
#endif

// @verified - d
void SetScenarioCharacterType(shen_name name, std::int32_t type) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    SetCharacterType(scenFindChara(control->scenarioTask, name), type);
}

// @verified - d
const Matrix4x4 *GetCharacterMirrorMatrix() {
    return &characterMirrorMatrix;
}

} // namespace s2
} // namespace shenmue

#endif
