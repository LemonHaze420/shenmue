#include "s2/character_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/map_control_dc.hpp"
#include "s2/character_draw_tasks_dc.hpp"
#include "s2/character_yaw_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/shadow_dc.hpp"
#include "s2/face_eye_control_dc.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "shenmue.h"
#include "s2/map_event_character_dc.hpp"
#include "s2/model_image_component_dc.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/event_character_model_dc.hpp"
#include <cstring>
#include "s2/map_region_dc.hpp"
#include "s2/door_state_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/character_components_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/pak_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/world_render.hpp"
#include "s2/field_collision_dc.hpp"
#include "platform/dc/matrix.hpp"
#include "platform/dc/am2_vector.hpp"
#endif
#include <cstdio>
#include "s2/math.hpp"
#include "s2/motion_work_dc.hpp"
#define SHENMUE_CHARACTER_POSE_NULL 0
#include "platform/dc/sdk.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/model_dc.hpp"
#include "s2/character_model_dc.hpp"
#include "s2/model_transform_dc.hpp"
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
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/character_transparency_dc.hpp"
#include "s2/hand_dc.hpp"
#include "s2/pause.hpp"
#endif
#include "s2/pad_control_dc.hpp"
#include "s2/player_state_dc.hpp"
#include <cmath>
#include "s2/model_image_dc.hpp"
#include "s2/cloth_dc.hpp"
#include "s2/osage_dc.hpp"
#include "s2/hand_pose_dc.hpp"
#include "s2/motion_node_dc.hpp"
#include "s2/prop_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/event_character_control_dc.hpp"
#include "s2/field_depth_dc.hpp"
#include "s2/object_xforms.hpp"
#include "s2/character_control_constructor_dc.hpp"
#include "s2/character_control_dc.hpp"
#include "s2/map_load_dc.hpp"
#include "s2/map_event_task_dc.hpp"
#include "s2/map_event_transition_dc.hpp"
#include "s2/load_title_dc.hpp"
#include "s2/screen_film_dc.hpp"
#include "platform/dc/runtime_arithmetic.hpp"
#include "common/matrix_stack.hpp"
#include "s2/map_info.hpp"
#include "s2/game_system_dc.hpp"
#include "s2/event_character_dc.hpp"
#include "s2/event_matrix_dc.hpp"
#include "platform/dc/mt7.hpp"
#include "s2/area_jump_dc.hpp"
#include "s2/character_table_dc.hpp"
#include "s2/conversation_dc.hpp"
#include "s2/directory_cache.hpp"
#include "s2/door_control_dc.hpp"
#include "s2/font.hpp"
#include "s2/screen_film.hpp"
#include "s2/title_dc.hpp"
#include "s2/texture_dc.hpp"
#include "s2/weather_dc.hpp"
#include "s2/window.hpp"
#include "s2/area_event.hpp"
#include "s2/scenario_control_dc.hpp"
#include "common/math.hpp"
#include "s2/hlib.hpp"
#include "s2/game_save_area.hpp"
#include "s2/input_dc.hpp"
#include "s2/system_modules_dc.hpp"
#include "s2/ui_state_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/calendar_dc.hpp"
#include "s2/debug_screen_dc.hpp"
#include "s2/save_screen_state_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/story_state.hpp"
#include <cstdarg>
#endif
#include "s2/scenario_character_query_dc.hpp"

namespace shenmue {
namespace s2 {

MT7Node *CreateShadowModel(const char *name);

std::int32_t TestCharacterVisibility(Character *character, void *);

void UpdateCharacterPositionDelta(Character *character);

std::int32_t IsCharacterValid(const Character *character);

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t IsCharacterInvalid(const Character *character);
#endif

void GetCharacterWorldPosition(const Character *character, Vector3 *position);

void InverseRotateVectorByCharacterNode(const CharacterNode *node, Vector3 *vector);

void EV_SetCharDisplay(Character *character, CharaVisibilityMode mode, std::int32_t enabled);

bool IsOfThisVisibilityType(const Character *character, CharaVisibilityMode mode);

void charaSetVisibilityFlagTo(Character *character, std::uint32_t set);

void charaCore(Character *character);

void charaCollisionDistanceChecks(void *data);

float GetCharacterDisplayRadius(const Character *character);

bool InsertNameTree(NameTreeNode **root, NameTreeNode *node);

NameTreeNode *FindNameTree(NameTreeNode *root, shen_name name);

std::uint32_t *GetControllerForNode(Character *character);

void IncrementPendingCharacterCleanup();

std::int32_t ForEachLiveCharacterInMapOrder(void *context, std::int32_t (*callback)(Character *, void *), std::uint32_t flags);

void ApplyRecursiveCharacterNodeTransforms(const CharacterNode *node);

void ApplyRecursiveCharacterNodeRotations(const CharacterNode *node);

void GetNodeWorldPosition(const CharacterNode *node, Vector3 *position);

void GetNodeWorldRotation(const CharacterNode *node, Vector3i *rotation);

void shenApplyTranslationToAll(CharacterNode *node, const Vector3 &position);

void SetCharacterNodeWorldRotation(CharacterNode *node, const Vector3i &rotation);

const Matrix4x4 *GetCharacterMirrorMatrix();

std::int32_t IsCharacterMirroredForDraw(const Character *character);

void BeginFacialBlink(FacialModelData &face);

void AdvanceFacialModel(FacialModelData &face);

void PrepareFacialMouth(FacialModelData &face, std::int32_t expression, std::int32_t duration, float scale);

void PrepareUpperFace(FacialModelData &face, std::int32_t expression, std::int32_t duration, float scale);

void PrepareLowerFace(FacialModelData &face, std::int32_t expression, std::int32_t duration, float scale);

void SetFacialEyelidPitch(FacialModelData &face, float pitch);

const Matrix4x4 *GetCharacterMirrorMatrix();

std::int32_t IsCharacterMirroredForDraw(const Character *character);

void ApplyRecursiveCharacterNodeTransforms(const CharacterNode *node);

void ApplyRecursiveCharacterNodeRotations(const CharacterNode *node);

void GetNodeWorldRotation(const CharacterNode *node, Vector3i *rotation);

void shenApplyTranslationToAll(CharacterNode *node, const Vector3 &position);

void SetCharacterNodeWorldRotation(CharacterNode *node, const Vector3i &rotation);

bool HasCharacterMotionWork(const Character *character);

void ApplyRecursiveCharacterNodeTransforms(const CharacterNode *node);

void ApplyRecursiveCharacterNodeRotations(const CharacterNode *node);

void GetNodeWorldRotation(const CharacterNode *node, Vector3i *rotation);

void shenApplyTranslationToAll(CharacterNode *node, const Vector3 &position);

void SetCharacterNodeWorldRotation(CharacterNode *node, const Vector3i &rotation);

void ApplyRecursiveCharacterNodeTransforms(const CharacterNode *node);

void ApplyRecursiveCharacterNodeRotations(const CharacterNode *node);

void GetNodeWorldRotation(const CharacterNode *node, Vector3i *rotation);

void shenApplyTranslationToAll(CharacterNode *node, const Vector3 &position);

void SetCharacterNodeWorldRotation(CharacterNode *node, const Vector3i &rotation);

void ApplyRecursiveCharacterNodeTransforms(const CharacterNode *node);

void ApplyRecursiveCharacterNodeRotations(const CharacterNode *node);

void GetNodeWorldRotation(const CharacterNode *node, Vector3i *rotation);

void shenApplyTranslationToAll(CharacterNode *node, const Vector3 &position);

void SetCharacterNodeWorldRotation(CharacterNode *node, const Vector3i &rotation);

void ApplyRecursiveCharacterNodeTransforms(const CharacterNode *node);

void ApplyRecursiveCharacterNodeRotations(const CharacterNode *node);

void GetNodeWorldRotation(const CharacterNode *node, Vector3i *rotation);

void shenApplyTranslationToAll(CharacterNode *node, const Vector3 &position);

void SetCharacterNodeWorldRotation(CharacterNode *node, const Vector3i &rotation);

const Matrix4x4 *GetCharacterMirrorMatrix();

std::int32_t IsCharacterMirroredForDraw(const Character *character);

MT7Model *GetImageModuleSource(const Character *character);

void PrepareUpperFace(FacialModelData &face, std::int32_t expression, std::int32_t duration, float scale);

void PrepareLowerFace(FacialModelData &face, std::int32_t expression, std::int32_t duration, float scale);

std::int16_t g_characterHandWristAngles[4];

HLib::Task *characterDrawTask = 0;

std::int32_t characterHumanDrawLimit = 30, characterObjectDrawLimit = 99;

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t characterPluralControlOffState = 2;
#endif

std::int32_t npcPoolSizingModeOrOverride = -5, characterLightAuthorOverride = 0;

float characterMinimumRed = 0.2f, characterMinimumGreen = 0.2f, characterMinimumBlue = 0.2f;

MT7Node *FindCachedShadowModel(const char *filename);

__attribute__((aligned(4))) std::byte fixedCharacterShadowMesh0[2136] = {
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x32),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x2d),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0x83),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x88),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x32),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x6e),
    static_cast<std::byte>(0xdb),
    static_cast<std::byte>(0x36),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x33),
    static_cast<std::byte>(0x33),
    static_cast<std::byte>(0x33),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x33),
    static_cast<std::byte>(0x33),
    static_cast<std::byte>(0x33),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x33),
    static_cast<std::byte>(0x33),
    static_cast<std::byte>(0x33),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0xe8),
    static_cast<std::byte>(0x07),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x72),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xd7),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0xdd),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x28),
    static_cast<std::byte>(0xb2),
    static_cast<std::byte>(0x5c),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x55),
    static_cast<std::byte>(0xba),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0xb5),
    static_cast<std::byte>(0xb6),
    static_cast<std::byte>(0xde),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xdf),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0x5d),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xc0),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x2b),
    static_cast<std::byte>(0xb2),
    static_cast<std::byte>(0xdc),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x55),
    static_cast<std::byte>(0xba),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0xd4),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0xec),
    static_cast<std::byte>(0xd9),
    static_cast<std::byte>(0x6e),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xa0),
    static_cast<std::byte>(0x11),
    static_cast<std::byte>(0xfc),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x66),
    static_cast<std::byte>(0xd3),
    static_cast<std::byte>(0x5e),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x07),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0xec),
    static_cast<std::byte>(0xd9),
    static_cast<std::byte>(0x6e),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0xd7),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0x5d),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0xa4),
    static_cast<std::byte>(0x11),
    static_cast<std::byte>(0x7c),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x67),
    static_cast<std::byte>(0xd3),
    static_cast<std::byte>(0x5e),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x48),
    static_cast<std::byte>(0x4c),
    static_cast<std::byte>(0xda),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xf2),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x33),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xb4),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x05),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0xd6),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0xdd),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x02),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xd6),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0x5d),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x30),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x48),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xf2),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xd9),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0xdd),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x28),
    static_cast<std::byte>(0xb2),
    static_cast<std::byte>(0x5c),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x53),
    static_cast<std::byte>(0xba),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0xcd),
    static_cast<std::byte>(0x79),
    static_cast<std::byte>(0x14),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xdf),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0x5d),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x2b),
    static_cast<std::byte>(0xb2),
    static_cast<std::byte>(0xdc),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x54),
    static_cast<std::byte>(0xba),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0xd5),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x60),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x78),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xf2),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0xa4),
    static_cast<std::byte>(0x30),
    static_cast<std::byte>(0x89),
    static_cast<std::byte>(0x3d),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xa0),
    static_cast<std::byte>(0x11),
    static_cast<std::byte>(0xfc),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x66),
    static_cast<std::byte>(0xd3),
    static_cast<std::byte>(0x5e),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x7e),
    static_cast<std::byte>(0x0e),
    static_cast<std::byte>(0x13),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x05),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0xa4),
    static_cast<std::byte>(0x30),
    static_cast<std::byte>(0x89),
    static_cast<std::byte>(0x3d),
    static_cast<std::byte>(0xd6),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0x5d),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0xa4),
    static_cast<std::byte>(0x11),
    static_cast<std::byte>(0x7c),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x66),
    static_cast<std::byte>(0xd3),
    static_cast<std::byte>(0x5e),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x4a),
    static_cast<std::byte>(0x4c),
    static_cast<std::byte>(0xda),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x60),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x78),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xf2),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x88),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xd5),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0x5d),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x02),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xc0),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x22),
    static_cast<std::byte>(0xb2),
    static_cast<std::byte>(0xdc),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x55),
    static_cast<std::byte>(0xba),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0xd6),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xa0),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xfb),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xbd),
    static_cast<std::byte>(0xec),
    static_cast<std::byte>(0xd9),
    static_cast<std::byte>(0x6e),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0xdb),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0x5d),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x98),
    static_cast<std::byte>(0x11),
    static_cast<std::byte>(0x7c),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x67),
    static_cast<std::byte>(0xd3),
    static_cast<std::byte>(0x5e),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x4c),
    static_cast<std::byte>(0x4c),
    static_cast<std::byte>(0xda),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xf2),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xb8),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xfb),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x7f),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0xd9),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0xdd),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0xfa),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x3a),
    static_cast<std::byte>(0xcd),
    static_cast<std::byte>(0x13),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0xd9),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0x5d),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x60),
    static_cast<std::byte>(0xfd),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x08),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x78),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xf2),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xd0),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xd3),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0x5d),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x02),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xc0),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x24),
    static_cast<std::byte>(0xb2),
    static_cast<std::byte>(0xdc),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x53),
    static_cast<std::byte>(0xba),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0xd9),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x60),
    static_cast<std::byte>(0xfd),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x50),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x90),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xf2),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xe8),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xfb),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xbd),
    static_cast<std::byte>(0xa4),
    static_cast<std::byte>(0x30),
    static_cast<std::byte>(0x89),
    static_cast<std::byte>(0x3d),
    static_cast<std::byte>(0xd9),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0x5d),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x9c),
    static_cast<std::byte>(0x11),
    static_cast<std::byte>(0x7c),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x65),
    static_cast<std::byte>(0xd3),
    static_cast<std::byte>(0x5e),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x4c),
    static_cast<std::byte>(0x4c),
    static_cast<std::byte>(0xda),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x60),
    static_cast<std::byte>(0xfd),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x68),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x90),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xf2),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xe8),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xd9),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0xdd),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x33),
    static_cast<std::byte>(0x28),
    static_cast<std::byte>(0xb2),
    static_cast<std::byte>(0x5c),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x55),
    static_cast<std::byte>(0xba),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x34),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xe8),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0xec),
    static_cast<std::byte>(0xd9),
    static_cast<std::byte>(0x6e),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0x33),
    static_cast<std::byte>(0x9f),
    static_cast<std::byte>(0x11),
    static_cast<std::byte>(0xfc),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x66),
    static_cast<std::byte>(0xd3),
    static_cast<std::byte>(0x5e),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x33),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xf2),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xe8),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xa0),
    static_cast<std::byte>(0x33),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0x34),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x60),
    static_cast<std::byte>(0xfd),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x68),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x78),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xf2),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xe8),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xd9),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0xdd),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xa0),
    static_cast<std::byte>(0x33),
    static_cast<std::byte>(0x28),
    static_cast<std::byte>(0xb2),
    static_cast<std::byte>(0x5c),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x54),
    static_cast<std::byte>(0xba),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0x34),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x60),
    static_cast<std::byte>(0xfd),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x90),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xf2),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xe8),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0xa4),
    static_cast<std::byte>(0x30),
    static_cast<std::byte>(0x89),
    static_cast<std::byte>(0x3d),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0x33),
    static_cast<std::byte>(0x9f),
    static_cast<std::byte>(0x11),
    static_cast<std::byte>(0xfc),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x66),
    static_cast<std::byte>(0xd3),
    static_cast<std::byte>(0x5e),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0xbd),
    static_cast<std::byte>(0x95),
    static_cast<std::byte>(0xdc),
    static_cast<std::byte>(0x33),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x60),
    static_cast<std::byte>(0xfd),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x90),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xf2),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xe8),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xdb),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0x5d),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xc0),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x2a),
    static_cast<std::byte>(0xb2),
    static_cast<std::byte>(0xdc),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x55),
    static_cast<std::byte>(0xba),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0xd5),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xe8),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x03),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0xec),
    static_cast<std::byte>(0xd9),
    static_cast<std::byte>(0x6e),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0xd8),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0x5d),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0xa4),
    static_cast<std::byte>(0x11),
    static_cast<std::byte>(0x7c),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x65),
    static_cast<std::byte>(0xd3),
    static_cast<std::byte>(0x5e),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x4a),
    static_cast<std::byte>(0x4c),
    static_cast<std::byte>(0xda),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xf2),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xe8),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x03),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0xd7),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0xdd),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x02),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xd7),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0x5d),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x60),
    static_cast<std::byte>(0xfd),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x68),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x78),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xf2),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xe8),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xdb),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0x5d),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xc0),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x2b),
    static_cast<std::byte>(0xb2),
    static_cast<std::byte>(0xdc),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x53),
    static_cast<std::byte>(0xba),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0xd6),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x60),
    static_cast<std::byte>(0xfd),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x90),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xf2),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xe8),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x03),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0xa4),
    static_cast<std::byte>(0x30),
    static_cast<std::byte>(0x89),
    static_cast<std::byte>(0x3d),
    static_cast<std::byte>(0xd7),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0x5d),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0xa4),
    static_cast<std::byte>(0x11),
    static_cast<std::byte>(0x7c),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x65),
    static_cast<std::byte>(0xd3),
    static_cast<std::byte>(0x5e),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x49),
    static_cast<std::byte>(0x4c),
    static_cast<std::byte>(0xda),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x60),
    static_cast<std::byte>(0xfd),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x90),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xf2),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xe8),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xd7),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0x5d),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xc0),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x26),
    static_cast<std::byte>(0xb2),
    static_cast<std::byte>(0xdc),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x54),
    static_cast<std::byte>(0xba),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0xd7),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xe8),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0xec),
    static_cast<std::byte>(0xd9),
    static_cast<std::byte>(0x6e),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0xda),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0x5d),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x9d),
    static_cast<std::byte>(0x11),
    static_cast<std::byte>(0x7c),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x67),
    static_cast<std::byte>(0xd3),
    static_cast<std::byte>(0x5e),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x4c),
    static_cast<std::byte>(0x4c),
    static_cast<std::byte>(0xda),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xf2),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xe8),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x7f),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0xd8),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0xdd),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x33),
    static_cast<std::byte>(0xd8),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0x5d),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x60),
    static_cast<std::byte>(0xfd),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x68),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x78),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xf2),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xe8),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xd7),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0x5d),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xc0),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x26),
    static_cast<std::byte>(0xb2),
    static_cast<std::byte>(0xdc),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x53),
    static_cast<std::byte>(0xba),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0xd7),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x60),
    static_cast<std::byte>(0xfd),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x90),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xf2),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xe8),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x3d),
    static_cast<std::byte>(0xa4),
    static_cast<std::byte>(0x30),
    static_cast<std::byte>(0x89),
    static_cast<std::byte>(0x3d),
    static_cast<std::byte>(0xd8),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0x5d),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x9f),
    static_cast<std::byte>(0x11),
    static_cast<std::byte>(0x7c),
    static_cast<std::byte>(0x3e),
    static_cast<std::byte>(0x66),
    static_cast<std::byte>(0xd3),
    static_cast<std::byte>(0x5e),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x4c),
    static_cast<std::byte>(0x4c),
    static_cast<std::byte>(0xda),
    static_cast<std::byte>(0xbe),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x60),
    static_cast<std::byte>(0xfd),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x90),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xf2),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xe8),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xe0),
    static_cast<std::byte>(0xf5),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xfa),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xe0),
    static_cast<std::byte>(0xf5),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x30),
    static_cast<std::byte>(0xfa),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xf2),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x18),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xe0),
    static_cast<std::byte>(0xf5),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x60),
    static_cast<std::byte>(0xfa),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x60),
    static_cast<std::byte>(0xfd),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xb0),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xf5),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xc8),
    static_cast<std::byte>(0xf9),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xf2),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x30),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xe0),
    static_cast<std::byte>(0xf5),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x90),
    static_cast<std::byte>(0xfa),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x60),
    static_cast<std::byte>(0xfd),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xe0),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xf5),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x28),
    static_cast<std::byte>(0xfa),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xf2),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x48),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xe0),
    static_cast<std::byte>(0xf5),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xc0),
    static_cast<std::byte>(0xfa),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x60),
    static_cast<std::byte>(0xfd),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xf8),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xf5),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x58),
    static_cast<std::byte>(0xfa),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x6a),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x0c),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x60),
    static_cast<std::byte>(0xf5),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xa0),
    static_cast<std::byte>(0xfa),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x60),
    static_cast<std::byte>(0xfd),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x18),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xe0),
    static_cast<std::byte>(0xfb),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xa0),
    static_cast<std::byte>(0xf3),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x70),
    static_cast<std::byte>(0xf9),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xc0),
    static_cast<std::byte>(0xfc),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xc0),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xa0),
    static_cast<std::byte>(0xfa),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xa0),
    static_cast<std::byte>(0xfd),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x98),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xf9),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xe0),
    static_cast<std::byte>(0xfc),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xfb),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xf0),
    static_cast<std::byte>(0xfd),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xb0),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xfa),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x78),
    static_cast<std::byte>(0xfd),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xe0),
    static_cast<std::byte>(0xf7),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x58),
    static_cast<std::byte>(0xfc),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x68),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x60),
    static_cast<std::byte>(0xf6),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x98),
    static_cast<std::byte>(0xfb),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0xf8),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xa8),
    static_cast<std::byte>(0xfc),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0xf7),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x30),
    static_cast<std::byte>(0xfc),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xf5),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x10),
    static_cast<std::byte>(0xfb),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xc0),
    static_cast<std::byte>(0xfd),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x38),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xa0),
    static_cast<std::byte>(0xf3),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x50),
    static_cast<std::byte>(0xfa),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xf5),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x60),
    static_cast<std::byte>(0xfb),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xc0),
    static_cast<std::byte>(0xfd),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x50),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xf4),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xe8),
    static_cast<std::byte>(0xfa),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x60),
    static_cast<std::byte>(0xf2),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xc8),
    static_cast<std::byte>(0xf9),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xfd),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x08),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xe0),
    static_cast<std::byte>(0xf0),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xc0),
    static_cast<std::byte>(0xf8),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xc0),
    static_cast<std::byte>(0xf2),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x18),
    static_cast<std::byte>(0xfa),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xfd),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xc0),
    static_cast<std::byte>(0xf1),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xa0),
    static_cast<std::byte>(0xf9),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xf1),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x78),
    static_cast<std::byte>(0xf9),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0xfc),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xd8),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xf0),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x70),
    static_cast<std::byte>(0xf8),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xf0),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0x88),
    static_cast<std::byte>(0xf8),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0xfc),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x5f),
    static_cast<std::byte>(0xf0),
    static_cast<std::byte>(0xfe),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x84),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
};

__attribute__((aligned(4))) std::byte fixedCharacterShadowMesh1[928] = {
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xd7),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0x5d),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x2d),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0x83),
    static_cast<std::byte>(0x40),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x88),
    static_cast<std::byte>(0x20),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xd7),
    static_cast<std::byte>(0xb3),
    static_cast<std::byte>(0x5d),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0xff),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x6e),
    static_cast<std::byte>(0xdb),
    static_cast<std::byte>(0x36),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x33),
    static_cast<std::byte>(0x33),
    static_cast<std::byte>(0x33),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x33),
    static_cast<std::byte>(0x33),
    static_cast<std::byte>(0x33),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x33),
    static_cast<std::byte>(0x33),
    static_cast<std::byte>(0x33),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x30),
    static_cast<std::byte>(0x03),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x16),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x96),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x96),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x96),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x96),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x96),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x04),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x3f),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0xbf),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x80),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x01),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x18),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
    static_cast<std::byte>(0x00),
};

const Matrix4x4 *GetCharacterMirrorMatrix();

MT7Model *GetImageModuleSource(const Character *character);

bool HasCharacterMotionWork(const Character *character);

std::uint32_t characterClipPointCount;

static Vector4 legacyModelColour = {1.0f, 0.2f, 0.2f, 0.2f};

const Matrix4x4 *GetCharacterMirrorMatrix();

float GetShadowRangeInverse() {
    return g_shadowRangeInverse;
}

// @verified - d
void CharDispCheck_Callback(void *) {
    characterClipPointCount = GetFieldDepthPointCount();
    ForEachLiveCharacterInMapOrder(0, TestCharacterVisibility, 0);
}

// @verified - d
std::int32_t TestCharacterVisibility(Character *character, void *) {
    bool shouldBeVisible = false;
    if (IsOfThisVisibilityType(character, CVM_DisplayEnabled)) {
        std::int32_t cameraVisible = 1;
        if (HasCharacterMotionWork(character)) {
            if (IsOfThisVisibilityType(character, CVM_MotionworkDisplayCheck)) {
                if (characterClipPointCount && charaCheckFlags(character)) {
                    Vector3 root;
                    mtwkGetBonePosition(charFindMotionwork(character), static_cast<MotionBoneId>(0), &root);
                    cameraVisible = camCtrlCheckPosition(root, kCharacterMotionClipRadius);
                }
                if (cameraVisible)
                    shouldBeVisible = true;
            }
        } else {
            shouldBeVisible = true;
            Vector3 root;
            GetCharacterWorldPosition(character, &root);
            bool frustumVisible;
            if (FindFixo(character))
                frustumVisible = true;
            else {
                const float radius = GetCharacterDisplayRadius(character);
                frustumVisible = camCtrlIsSphereVisible(root, radius);
                if (frustumVisible && characterClipPointCount && charaCheckFlags(character))
                    cameraVisible = camCtrlCheckPosition(root, radius);
            }
            EV_SetCharDisplay(character, CVM_FrustumResult, frustumVisible);
        }
        EV_SetCharDisplay(character, CVM_CameraOcclusionResult, cameraVisible);
    }
    std::int32_t visible = 0;
    if ((shouldBeVisible || charaForceVisibleCheck(character)) && charaIsDisplayCheckEnabled(character))
        visible = 1;
    charaSetVisibilityFlagTo(character, visible);
    return 1;
}

// @verified - d
void SetupCharacterLightFrame(CharacterDrawTaskParameters *data) {
    data->previousActiveLight = GetCurrentActiveLight();
    std::memcpy(data->savedLights, g_selectedLights, sizeof(data->savedLights));
    GetLightRootPosition(&data->lightRoot);
    Vector4 *const global = &data->globalLighting;
    GetGlobalLighting(global);
    Vector4 *const minimum = &data->minimumGlobalLighting;
    Vector4 *const normalized = &data->normalizedLighting;
    *minimum = *global;
    if (minimum->y < characterMinimumRed)
        minimum->y = characterMinimumRed;
    if (minimum->z < characterMinimumGreen)
        minimum->z = characterMinimumGreen;
    if (minimum->w < characterMinimumBlue)
        minimum->w = characterMinimumBlue;
    float maximum = minimum->y;
    if (minimum->z > maximum)
        maximum = minimum->z;
    if (minimum->w > maximum)
        maximum = minimum->w;
    if (maximum < kCharacterLightNormalizationThreshold) {
        normalized->y = normalized->z = normalized->w = 1.0f;
        normalized->x = 0.0f;
    } else {
        normalized->x = maximum;
        normalized->y = minimum->y / maximum;
        normalized->z = minimum->z / maximum;
        normalized->w = minimum->w / maximum;
    }
    characterLightAuthorOverride = 0;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
float GetReservedWeatherOverride0();

void DrawLights(Character *character, CharacterDrawTaskParameters *data) {
    CharacterLightControl *const control = reinterpret_cast<CharacterLightControl *>(GetControllerForNode(character));
    if (data->previousActiveLight == 1) {
        if (control) {
            control->ids[0] = 0;
            control->weights[0] = 1.0f;
            control->intensities[0] = 1.0f;
            control->ids[1] = -1;
        }
        return;
    }
    std::int16_t rankedIds[kCharacterLightCount];
    float rankedScores[kCharacterLightCount], rankedAttenuation[kCharacterLightCount];
    Vector3 position, source, offset, direction;
    LightRuntimeEntry *light;
    std::int32_t i, j, k, count, mode, fadeLimit;
    std::int16_t *ids;
    float *weights, *intensities;
    float attenuation, intensity, rangePower, cutoff, level, score;
    std::uint32_t entryStorage[sizeof(LightRuntimeEntry) / sizeof(std::uint32_t)];
    Vector4 colour;
    charaGetMotWorldPos(character, 1, &position);
    count = 0;
    for (i = 0; g_characterLightCandidates[i] >= 0; ++i) {
        light = &g_lightRuntimeEntries[g_characterLightCandidates[i]];
        source = light->vector20;
        attenuation = 1.0f;
        switch (static_cast<std::int8_t>(light->indexes[1]) & ~8) {
        case 3:
            source.x += data->lightRoot.x;
            source.y += data->lightRoot.y;
            source.z += data->lightRoot.z;
        case 4:
            offset.x = source.x - position.x;
            offset.y = source.y - position.y;
            offset.z = source.z - position.z;
            if ((offset.x < 0.0f ? -offset.x : offset.x) < kCharacterLightNormalizationThreshold &&
                (offset.y < 0.0f ? -offset.y : offset.y) < kCharacterLightNormalizationThreshold &&
                (offset.z < 0.0f ? -offset.z : offset.z) < kCharacterLightNormalizationThreshold)
                offset.y = kCharacterLightNormalizationThreshold;
            attenuation = am2Vector3Length(&offset);
            if (attenuation > light->vectors[1].z + kCharacterLightRangeMargin)
                continue;
            attenuation += kCharacterLightDistanceBias;
            break;
        default:
            offset.x = -light->values2c[0];
            offset.y = -light->values2c[1];
            offset.z = -light->value34;
            break;
        }
        attenuation = 1.0f / attenuation;
        intensity = ((light->vectors[0].y + light->vectors[0].z) + light->vectors[1].x) * light->vectors[1].y;
        rangePower = light->value1c * light->vectors[1].z;
        rangePower *= rangePower;
        rangePower *= rangePower;
        rangePower *= rangePower;
        cutoff = dc::sdk::CosFixed(light->unknown02 + 910);
        score = -100000.0f;
        switch (static_cast<std::int8_t>(light->indexes[1]) & ~8) {
        case 3:
            attenuation *= attenuation;
            attenuation *= attenuation;
            attenuation *= attenuation;
            attenuation *= rangePower;
            if (attenuation > 1.0f)
                attenuation = 1.0f;
            score = attenuation * intensity;
            break;
        case 4: {
            direction = *reinterpret_cast<const Vector3 *>(light->values2c);
            am2Vector3NormalizeFast(&direction);
            level = (am2Vector3Dot(&offset, &direction) * attenuation - cutoff) / (1.0f - cutoff);
            attenuation *= attenuation;
            attenuation *= attenuation;
            attenuation *= attenuation;
            attenuation *= rangePower;
            if (attenuation > 1.0f)
                attenuation = 1.0f;
            attenuation *= level;
            score = attenuation * intensity;
            break;
        }
        case 2:
            score = attenuation * intensity - 10000.0f;
            break;
        }
        if (0.0f > attenuation)
            attenuation = 0.0f;
        for (j = 0; j < count; ++j) {
            if (rankedScores[j] < score - kCharacterLightTieMargin ||
                (rankedScores[j] < score + kCharacterLightTieMargin && rankedIds[j] < g_characterLightCandidates[i])) {
                for (k = count > kCharacterLightCount - 1 ? kCharacterLightCount - 1 : count; k > j; --k) {
                    rankedIds[k] = rankedIds[k - 1];
                    rankedScores[k] = rankedScores[k - 1];
                    rankedAttenuation[k] = rankedAttenuation[k - 1];
                }
                break;
            }
        }
        if (j < kCharacterLightCount) {
            rankedIds[j] = g_characterLightCandidates[i];
            rankedScores[j] = score;
            rankedAttenuation[j] = attenuation;
        }
        if (count < kCharacterLightCount)
            ++count;
    }
    if (control) {
        mode = control->state;
        fadeLimit = control->fadeLimit;
        ids = control->ids;
        weights = control->weights;
        intensities = control->intensities;
        for (i = 0; i < kCharacterLightCount && ids[i] >= 0; ++i) {
            for (j = 0; j < count; ++j) {
                if (ids[i] == rankedIds[j]) {
                    intensities[i] = rankedAttenuation[j];
                    break;
                }
            }
            if (j < count && j < fadeLimit) {
                weights[i] += kCharacterLightFadeStep;
                if (weights[i] > 1.0f)
                    weights[i] = 1.0f;
            } else {
                if (j < count) {
                    weights[i] -= kCharacterLightFadeStep;
                    if (weights[i] > 0.0f)
                        continue;
                }
                for (j = i; j < kCharacterLightCount - 1; ++j) {
                    ids[j] = ids[j + 1];
                    weights[j] = weights[j + 1];
                    intensities[j] = intensities[j + 1];
                }
                ids[j] = -1;
                --i;
            }
        }
        for (i = 0; i < count; ++i) {
            for (j = 0; j < kCharacterLightCount && rankedIds[i] != ids[j]; ++j) {
                if (ids[j] < 0) {
                    ids[j] = rankedIds[i];
                    weights[j] = j < fadeLimit ? 1.0f : 0.0f;
                    intensities[j] = rankedAttenuation[i];
                    if (++j < kCharacterLightCount)
                        ids[j] = -1;
                    break;
                }
            }
        }
    } else {
        fadeLimit = mode = 1;
        ids = rankedIds;
        weights = rankedAttenuation;
        intensities = rankedScores;
        for (i = 0; i < kCharacterLightCount; ++i) {
            intensities[i] = rankedAttenuation[i];
            if (i < count)
                weights[i] = i < fadeLimit ? 1.0f : 0.0f;
            else
                ids[i] = -1;
        }
    }
    intensity = 0.0f;
    for (i = 0; i < kCharacterLightCount && ids[i] >= 0; ++i)
        intensity += intensities[i] * weights[i];
    if (characterLightAuthorOverride && intensity < kCharacterLightFallbackThreshold) {
        SetCurrentActiveLight(1);
        SelectLightForFrame(0, &g_lightRuntimeEntries[0]);
        SetSelectedLightEnabled(0, 1);
        colour.x = colour.y = colour.z = colour.w = GetReservedWeatherOverride0();
        UpdateGlobalLighting(colour);
        j = 1;
    } else if (mode) {
        SetCurrentActiveLight(1);
        ResetSelectedLightEntry(0);
        SetLightKind(0, 2);
        SetSelectedLightEnabled(0, 1);
        offset.x = offset.y = offset.z = 0.0f;
        colour.x = colour.y = colour.z = colour.w = 0.0f;
        level = 0.0f;
        j = 0;
        for (i = 0; i < kCharacterLightCount && ids[i] >= 0; ++i) {
            intensity = intensities[i] * weights[i];
            if (intensity > kCharacterLightNormalizationThreshold) {
                light = &g_lightRuntimeEntries[ids[i]];
                source = light->vector20;
                switch (static_cast<std::int8_t>(light->indexes[1]) & ~8) {
                case 3:
                    source.x += data->lightRoot.x;
                    source.y += data->lightRoot.y;
                    source.z += data->lightRoot.z;
                case 4:
                    direction.x = position.x - source.x;
                    direction.y = position.y - source.y;
                    direction.z = position.z - source.z;
                    if ((offset.x < 0.0f ? -offset.x : offset.x) < kCharacterLightNormalizationThreshold &&
                        (offset.y < 0.0f ? -offset.y : offset.y) < kCharacterLightNormalizationThreshold &&
                        (offset.z < 0.0f ? -offset.z : offset.z) < kCharacterLightNormalizationThreshold)
                        offset.y = -kCharacterLightNormalizationThreshold;
                    break;
                default:
                    direction = *reinterpret_cast<const Vector3 *>(light->values2c);
                    break;
                }
                am2Vector3NormalizeFast(&direction);
                offset.x += intensity * direction.x;
                offset.y += intensity * direction.y;
                offset.z += intensity * direction.z;
                intensity *= light->vectors[1].y;
                colour.y += intensity * light->vectors[0].y;
                colour.z += intensity * light->vectors[0].z;
                colour.w += intensity * light->vectors[1].x;
                ++j;
            }
        }
        if (j) {
            if ((offset.x < 0.0f ? -offset.x : offset.x) < kCharacterLightNormalizationThreshold &&
                (offset.y < 0.0f ? -offset.y : offset.y) < kCharacterLightNormalizationThreshold &&
                (offset.z < 0.0f ? -offset.z : offset.z) < kCharacterLightNormalizationThreshold)
                offset.y = -kCharacterLightNormalizationThreshold;
            am2Vector3NormalizeFast(&offset);
            if (0.0f > colour.y)
                colour.y = 0.0f;
            if (0.0f > colour.z)
                colour.z = 0.0f;
            if (0.0f > colour.w)
                colour.w = 0.0f;
            level = colour.y;
            if (colour.z > level)
                level = colour.z;
            if (colour.w > level)
                level = colour.w;
            if (level < kCharacterLightNormalizationThreshold)
                j = 0;
        }
        if (!j) {
            level = 0.0f;
            colour.x = colour.y = colour.z = colour.w = 1.0f;
            offset.x = offset.y = offset.z = 0.0f;
        }
        intensity = 1.0f - level;
        if (intensity < kCharacterLightMinimumAmbient)
            intensity = kCharacterLightMinimumAmbient;
        colour.y += intensity * data->normalizedLighting.y;
        colour.z += intensity * data->normalizedLighting.z;
        colour.w += intensity * data->normalizedLighting.w;
        intensity = colour.y;
        if (colour.z > intensity)
            intensity = colour.z;
        if (colour.w > intensity)
            intensity = colour.w;
        colour.y /= intensity;
        colour.z /= intensity;
        colour.w /= intensity;
        level *= intensity;
        shenSetLightColour(0, colour);
        SetSelectedLightDirection(0, offset);
        SetSelectedLightSceneIntensity(0, level);
        colour.x = colour.y = colour.z = colour.w = data->normalizedLighting.x;
        UpdateGlobalLighting(colour);
        j = 1;
    } else {
        SetCurrentActiveLight(data->previousActiveLight);
        SetSelectedLightEnabled(0, 0);
        j = 1;
        for (i = 0; i < kCharacterLightCount && ids[i] >= 0; ++i) {
            if (weights[i] > kCharacterLightFallbackThreshold) {
                const LightRuntimeEntry *const selected = &g_lightRuntimeEntries[ids[i]];
                *reinterpret_cast<LightRuntimeEntry *>(entryStorage) = *selected;
                reinterpret_cast<LightRuntimeEntry *>(entryStorage)->vectors[0].y *= weights[i];
                reinterpret_cast<LightRuntimeEntry *>(entryStorage)->vectors[0].z *= weights[i];
                reinterpret_cast<LightRuntimeEntry *>(entryStorage)->vectors[1].x *= weights[i];
                SelectLightForFrame(j++, reinterpret_cast<LightRuntimeEntry *>(entryStorage));
            }
        }
        UpdateGlobalLighting(data->minimumGlobalLighting);
    }
    while (j < 16)
        SetSelectedLightEnabled(j++, 0);
    PublishGlobalLighting();
}
#else
// @verified - d
void DrawLights(Character *character, CharacterDrawTaskParameters *data) {
    std::uint32_t *const controller = GetControllerForNode(character);
    if (data->previousActiveLight == 1) {
        if (controller) {
            character->lightIds[0] = 0;
            character->lightWeights[0] = 1.0f;
            character->lightIntensities[0] = 1.0f;
            character->lightIds[1] = -1;
        }
        return;
    }
    Vector3 position;
    charaGetMotWorldPos(character, 1, &position);
    std::int16_t rankedIds[kCharacterLightCount];
    float rankedScores[kCharacterLightCount], rankedAttenuation[kCharacterLightCount];
    std::int32_t count = 0;
    for (std::int32_t candidate = 0; g_characterLightCandidates[candidate] >= 0; ++candidate) {
        const std::int16_t id = g_characterLightCandidates[candidate];
        const LightRuntimeEntry &light = g_lightRuntimeEntries[id];
        const std::uint32_t kind = light.indexes[1] & 0xf7U;
        Vector3 source = light.vector20, displacement;
        float distance = 1.0f;
        if (kind == 3 || kind == 4) {
            if (kind == 3) {
                source.x += data->lightRoot.x;
                source.y += data->lightRoot.y;
                source.z += data->lightRoot.z;
            }
            displacement = (__typeof__(displacement)){source.x - position.x, source.y - position.y, source.z - position.z};
            if (std::fabs(displacement.x) < kCharacterLightNormalizationThreshold && std::fabs(displacement.y) < kCharacterLightNormalizationThreshold &&
                std::fabs(displacement.z) < kCharacterLightNormalizationThreshold)
                displacement.y = kCharacterLightNormalizationThreshold;
            distance = dc::sdk::njScalor(&displacement);
            if (distance > light.vectors[1].z + kCharacterLightRangeMargin)
                continue;
            distance += kCharacterLightDistanceBias;
        } else {
            displacement = (__typeof__(displacement)){-light.values2c[0], -light.values2c[1], -light.value34};
        }
        float attenuation = 1.0f / distance;
        const float brightness = ((light.vectors[0].y + light.vectors[0].z) + light.vectors[1].x) * light.vectors[1].y;
        float rangePower = light.value1c * light.vectors[1].z;
        rangePower *= rangePower;
        rangePower *= rangePower;
        rangePower *= rangePower;
        const float cutoff = dc::sdk::CosFixed(light.unknown02 + 910);
        float score = -100000.0f;
        if (kind == 2) {
            score = attenuation * brightness - 10000.0f;
        } else if (kind == 3 || kind == 4) {
            float cone = 1.0f;
            if (kind == 4) {
                Vector3 direction = {light.values2c[0], light.values2c[1], light.value34};
                dc::sdk::njUnitVector(&direction);
                cone = (dc::sdk::njInnerProduct(&displacement, &direction) * attenuation - cutoff) / (1.0f - cutoff);
            }
            attenuation *= attenuation;
            attenuation *= attenuation;
            attenuation *= attenuation;
            attenuation *= rangePower;
            if (attenuation > 1.0f)
                attenuation = 1.0f;
            if (kind == 4)
                attenuation *= cone;
            score = attenuation * brightness;
        }
        if (0.0f > attenuation)
            attenuation = 0.0f;
        std::int32_t insert = 0;
        for (; insert < count; ++insert) {
            if (score - kCharacterLightTieMargin > rankedScores[insert] ||
                (score + kCharacterLightTieMargin > rankedScores[insert] && rankedIds[insert] < id)) {
                std::int32_t move = count > kCharacterLightCount - 1 ? kCharacterLightCount - 1 : count;
                for (; move > insert; --move) {
                    rankedIds[move] = rankedIds[move - 1];
                    rankedScores[move] = rankedScores[move - 1];
                    rankedAttenuation[move] = rankedAttenuation[move - 1];
                }
                break;
            }
        }
        if (insert < kCharacterLightCount) {
            rankedIds[insert] = id;
            rankedScores[insert] = score;
            rankedAttenuation[insert] = attenuation;
        }
        if (count < kCharacterLightCount)
            ++count;
    }
    std::int16_t *ids;
    float *weights, *intensities;
    std::int32_t mode = 1, fadeLimit = 1;
    if (controller) {
        mode = static_cast<std::int16_t>(character->retainedLightState);
        fadeLimit = static_cast<std::int8_t>(character->retainedLightEnabled);
        ids = character->lightIds;
        weights = character->lightWeights;
        intensities = character->lightIntensities;
        for (std::int32_t slot = 0; slot < kCharacterLightCount && ids[slot] >= 0;) {
            std::int32_t rank = 0;
            for (; rank < count; ++rank) {
                if (rankedIds[rank] == ids[slot]) {
                    intensities[slot] = rankedAttenuation[rank];
                    break;
                }
            }
            if (rank < count && rank < fadeLimit) {
                weights[slot] += kCharacterLightFadeStep;
                if (weights[slot] > 1.0f)
                    weights[slot] = 1.0f;
                ++slot;
            } else if (rank < count && (weights[slot] -= kCharacterLightFadeStep) > 0.0f) {
                ++slot;
            } else {
                for (std::int32_t move = slot; move < kCharacterLightCount - 1; ++move) {
                    ids[move] = ids[move + 1];
                    weights[move] = weights[move + 1];
                    intensities[move] = intensities[move + 1];
                }
                ids[kCharacterLightCount - 1] = -1;
            }
        }
        for (std::int32_t rank = 0; rank < count; ++rank) {
            for (std::int32_t slot = 0; slot < kCharacterLightCount; ++slot) {
                if (ids[slot] == rankedIds[rank])
                    break;
                if (ids[slot] < 0) {
                    ids[slot] = rankedIds[rank];
                    weights[slot] = slot < fadeLimit ? 1.0f : 0.0f;
                    intensities[slot] = rankedAttenuation[rank];
                    if (slot + 1 < kCharacterLightCount)
                        ids[slot + 1] = -1;
                    break;
                }
            }
        }
    } else {
        ids = rankedIds;
        weights = rankedAttenuation;
        intensities = rankedScores;
        for (std::int32_t slot = 0; slot < kCharacterLightCount; ++slot) {
            if (slot < count) {
                intensities[slot] = weights[slot];
                weights[slot] = slot < fadeLimit ? 1.0f : 0.0f;
            } else {
                ids[slot] = -1;
            }
        }
    }
    float total = 0.0f;
    for (std::int32_t slot = 0; slot < kCharacterLightCount && ids[slot] >= 0; ++slot)
        total = (intensities[slot] * weights[slot] + total);
    std::uint32_t destination = 1;
    if (characterLightAuthorOverride && total < kCharacterLightFallbackThreshold) {
        SetCurrentActiveLight(1);
        SelectLightForFrame(0, &g_lightRuntimeEntries[0]);
        SetSelectedLightEnabled(0, 1);
        const float ambient = GetWeatherLighting();
        UpdateGlobalLighting((Vector4){ambient, ambient, ambient, ambient});
    } else if (mode) {
        SetCurrentActiveLight(1);
        ResetSelectedLightEntry(0);
        SetLightKind(0, 2);
        SetSelectedLightEnabled(0, 1);
        Vector3 direction = {};
        Vector4 colour = {};
        std::int32_t contributors = 0;
        for (std::int32_t slot = 0; slot < kCharacterLightCount && ids[slot] >= 0; ++slot) {
            const float weight = intensities[slot] * weights[slot];
            if (!(weight > kCharacterLightNormalizationThreshold))
                continue;
            const LightRuntimeEntry &light = g_lightRuntimeEntries[ids[slot]];
            const std::uint32_t kind = light.indexes[1] & 0xf7U;
            Vector3 source = light.vector20, incoming;
            if (kind == 3 || kind == 4) {
                if (kind == 3) {
                    source.x += data->lightRoot.x;
                    source.y += data->lightRoot.y;
                    source.z += data->lightRoot.z;
                }
                incoming = (__typeof__(incoming)){position.x - source.x, position.y - source.y, position.z - source.z};
                if (std::fabs(incoming.x) < kCharacterLightNormalizationThreshold && std::fabs(incoming.y) < kCharacterLightNormalizationThreshold &&
                    std::fabs(incoming.z) < kCharacterLightNormalizationThreshold)
                    incoming.y = -kCharacterLightNormalizationThreshold;
            } else {
                incoming = (__typeof__(incoming)){light.values2c[0], light.values2c[1], light.value34};
            }
            dc::sdk::njUnitVector(&incoming);
            direction.x = (weight * incoming.x + direction.x);
            direction.y = (weight * incoming.y + direction.y);
            direction.z = (weight * incoming.z + direction.z);
            const float intensity = weight * light.vectors[1].y;
            colour.y = (intensity * light.vectors[0].y + colour.y);
            colour.z = (intensity * light.vectors[0].z + colour.z);
            colour.w = (intensity * light.vectors[1].x + colour.w);
            ++contributors;
        }
        float brightness = 0.0f;
        if (contributors) {
            if (std::fabs(direction.x) < kCharacterLightNormalizationThreshold && std::fabs(direction.y) < kCharacterLightNormalizationThreshold &&
                std::fabs(direction.z) < kCharacterLightNormalizationThreshold)
                direction.y = -kCharacterLightNormalizationThreshold;
            dc::sdk::njUnitVector(&direction);
            if (colour.y < 0.0f)
                colour.y = 0.0f;
            if (colour.z < 0.0f)
                colour.z = 0.0f;
            if (colour.w < 0.0f)
                colour.w = 0.0f;
            brightness = colour.y;
            if (colour.z > brightness)
                brightness = colour.z;
            if (colour.w > brightness)
                brightness = colour.w;
            if (brightness < kCharacterLightNormalizationThreshold)
                contributors = 0;
        }
        if (!contributors) {
            brightness = 0.0f;
            colour = (__typeof__(colour)){1.0f, 1.0f, 1.0f, 1.0f};
            direction = __typeof__(direction)();
        }
        float ambient = 1.0f - brightness;
        if (ambient < kCharacterLightMinimumAmbient)
            ambient = kCharacterLightMinimumAmbient;
        colour.y = (ambient * data->normalizedLighting.y + colour.y);
        colour.z = (ambient * data->normalizedLighting.z + colour.z);
        colour.w = (ambient * data->normalizedLighting.w + colour.w);
        float maximum = colour.y;
        if (colour.z > maximum)
            maximum = colour.z;
        if (colour.w > maximum)
            maximum = colour.w;
        colour.y /= maximum;
        colour.z /= maximum;
        colour.w /= maximum;
        shenSetLightColour(0, colour);
        SetSelectedLightDirection(0, direction);
        SetSelectedLightSceneIntensity(0, brightness * maximum);
        const float intensity = data->normalizedLighting.x;
        UpdateGlobalLighting((Vector4){intensity, intensity, intensity, intensity});
    } else {
        SetCurrentActiveLight(data->previousActiveLight);
        SetSelectedLightEnabled(0, 0);
        for (std::int32_t slot = 0; slot < kCharacterLightCount && ids[slot] >= 0; ++slot) {
            const float weight = weights[slot];
            if (!(weight > kCharacterLightFallbackThreshold))
                continue;
            LightRuntimeEntry light = g_lightRuntimeEntries[ids[slot]];
            light.vectors[0].y *= weight;
            light.vectors[0].z *= weight;
            light.vectors[1].x *= weight;
            SelectLightForFrame(destination++, &light);
        }
        UpdateGlobalLighting(data->globalLighting);
    }
    for (; destination < 16; ++destination)
        SetSelectedLightEnabled(destination, 0);
    PublishGlobalLighting();
}
#endif

// @verified - d
void RestoreCharacterLightFrame(CharacterDrawTaskParameters *data) {
    if (data->previousActiveLight == 1)
        return;
    SetCurrentActiveLight(data->previousActiveLight);
    std::int32_t index = 0;
    LightRuntimeEntry *light = data->savedLights;
    for (; index < 16; ++index, ++light)
        SelectLightForFrame(index, light);
    UpdateGlobalLighting(data->globalLighting);
    PublishGlobalLighting();
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t GetCharacterLocalYaw(const Character *character) {
    return character ? character->node.rotation.y : 0;
}
#endif

// @verified - d
void SetCharacterWorldPosition(Character *character, const Vector3 *position) {
    if (character)
        shenApplyTranslationToAll(&character->node, *position);
}

// @verified - d
Character *GetCharacterFromNode(CharacterNode *node) {
    Character *character = reinterpret_cast<Character *>(reinterpret_cast<std::uintptr_t>(node) - offsetof(Character, node));
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (IsCharacterInvalid(character))
        return SHENMUE_CHARACTER_POSE_NULL;
    return character;
#else
    return IsCharacterValid(character) ? character : SHENMUE_CHARACTER_POSE_NULL;
#endif
}

// @verified - d
void CharacterHandler_Callback(void *data) {
    Character *const character = static_cast<Character *>(data);
    if (character->name == CHARACTER_ENTITY) {
        if ((character->flags & (CF_Active | CF_HandlerDisabled)) == CF_Active) {
            g_characterHandWristAngles[0] = g_characterHandWristAngles[1] = g_characterHandWristAngles[2] = g_characterHandWristAngles[3] = 0;
            for (ModelComponentHeader *component = character->modelComponents; component; component = component->next) {
                if ((component->flags & 1U) && component->update)
                    component->update(character, component + 1);
            }
        }
        UpdateCharacterPositionDelta(character);
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void UpdateCharacterPositionDelta(Character *character) {
    float *delta = &character->positionDelta.x;
    if (!character->positionDeltaApplied && (delta[0] != 0.0f || delta[1] != 0.0f || delta[2] != 0.0f)) {
        float *position = &character->node.position.x;
        float x = *position++, y = *position++, z = *position;
        x += *delta++;
        y += *delta++;
        z += *delta++;
        delta -= 3;
        *position = z;
        *--position = y;
        *--position = x;
    }
    delta += 3;
    *--delta = 0.0f;
    *--delta = 0.0f;
    *--delta = 0.0f;
    character->positionDeltaApplied = 0;
}
#else
void UpdateCharacterPositionDelta(Character *character) {
    Vector3 *const delta = &character->positionDelta;
    if (!character->positionDeltaApplied && (delta->x != 0.0f || delta->y != 0.0f || delta->z != 0.0f)) {
        float x = character->node.position.x, y = character->node.position.y, z = character->node.position.z;
        x += delta->x;
        y += delta->y;
        z += delta->z;
        character->node.position.z = z;
        character->node.position.y = y;
        character->node.position.x = x;
    }
    delta->x = delta->y = delta->z = 0.0f;
    character->positionDeltaApplied = 0;
}
#endif

Character *EnqueueCharacterHandler(std::int8_t index, HLib::Task **outputTask) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(CharacterHandler_Callback, index, 4, sizeof(Character), TASK_CHAR);
    if (outputTask)
        *outputTask = task;
    Character *const character = static_cast<Character *>(HLib::GetTaskParameter(task));
    character->name = CHARACTER_ENTITY;
    CharacterNode *const node = &character->node;
    node->parent = 0;
    node->position.x = node->position.y = node->position.z = 0.0f;
    node->rotation.x = node->rotation.y = node->rotation.z = 0;
    node->layer = 0;
    node->collision = 0;
    character->positionDelta.x = character->positionDelta.y = character->positionDelta.z = 0.0f;
    character->scale.x = character->scale.y = character->scale.z = 1.0f;
    character->shadowOffset.x = character->shadowOffset.y = character->shadowOffset.z = 0.0f;
    character->shadowScale.x = character->shadowScale.y = character->shadowScale.z = 1.0f;
    character->shadowYaw = 0;
    character->type = 0;
    character->shadowY = 0.0f;
    character->positionDeltaApplied = 0;
    character->model = 0;
    character->shadowModel = 0;
    character->flags = CF_Valid | CF_Active | CF_Unknown40 | CF_DrawMirrored;
    character->modelComponentStorage = 0;
    character->modelComponents = 0;
    character->tokenTree = 0;
    character->core = charaCore;
    character->collisionCheck = charaCollisionDistanceChecks;
    character->applyScale = reinterpret_cast<CharacterUpdateCallback>(charaApplyScale);
    character->prepareForDraw = charaPrepareNodesForDraw;
    character->auxiliaryComponentStorage = 0;
    character->retainedComponentState[0] = -1;
    character->retainedComponentState[1] = -1;
    character->radius = 1.8f;
    character->componentAllocationBytes = sizeof(Character);
    character->resourceOwner = __typeof__(character->resourceOwner)();
    character->alpha = 1.0f;
    character->retainedLightState = 1;
    character->retainedLightEnabled = 1;
    character->retainedLightMode = 1;
    for (std::int32_t index = 0; index < 6; ++index) {
        character->lightIds[index] = -1;
        character->lightWeights[index] = 0.0f;
        character->lightIntensities[index] = 0.0f;
    }
    character->drawDistance = 0.0f;
    character->viewPosition.x = 0.0f;
    character->viewPosition.y = 0.0f;
    character->viewPosition.z = 0.0f;
    character->facialModel = 0;
    EV_SetCharDisplay(character, CVM_All, 1);
    EV_SetCharDisplay(character, CVM_DisplayEnabled, 0);
    return character;
}

// @verified - d
void FreeModelComponents(Character *character) {
    if (!character)
        return;
    for (ModelComponentHeader *component = character->modelComponents; component; component = component->next) {
        if (component->cleanup)
            component->cleanup(character, component + 1);
    }
    ModelComponentStorageBlock *block = character->modelComponentStorage;
    while (block) {
        ModelComponentStorageBlock *const previous = block->previous;
        HLib::FreeTaskData(block);
        block = previous;
    }
    block = character->auxiliaryComponentStorage;
    while (block) {
        ModelComponentStorageBlock *const previous = block->previous;
        HLib::FreeTaskData(block);
        block = previous;
    }
    character->name = __typeof__(character->name)();
}

// @verified - d
void MarkCharacterForCleanup(Character *character, std::int32_t mode) {
    if (character) {
        if (!mode)
            character->flags |= CF_Unknown02;
        character->flags &= ~CF_Valid;
        IncrementPendingCharacterCleanup();
    }
}

// @verified - d
std::int32_t IsCharacterValid(const Character *character) {
    if (!character)
        return false;
    if (character->name != CHARACTER_ENTITY)
        return false;
    if (character->flags & CF_Valid)
        return true;
    return false;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t IsCharacterInvalid(const Character *character) {
    if (!character)
        return 1;
    if (character->name != CHARACTER_ENTITY)
        return 1;
    if (character->flags & CF_Valid)
        return 0;
    return 1;
}
#endif

// @verified - d
void *AllocateModelComponent(Character *character, shen_name token, std::uint32_t payloadSize, void (*update)(Character *, void *)) {
    if (!character)
        return 0;
    payloadSize = (payloadSize + 0x23U) & ~3U;
    std::uint32_t storageSize;
    ModelComponentStorageBlock *storage;
    if (payloadSize < 0x200) {
        storageSize = 0x200;
        for (storage = character->modelComponentStorage; storage; storage = storage->previous) {
            if (storage->usedBytes + payloadSize <= 0x200)
                break;
        }
    } else {
        storageSize = payloadSize;
        storage = 0;
    }
    if (!storage) {
        storage = static_cast<ModelComponentStorageBlock *>(HLib::AllocateTaskData(storageSize + sizeof(ModelComponentStorageBlock), MEMORY_MODL));
        storage->usedBytes = 0;
        storage->previous = character->modelComponentStorage;
        character->modelComponentStorage = storage;
        const std::uint16_t allocationBytes =
            static_cast<std::uint16_t>(character->componentAllocationBytes) + sizeof(ModelComponentStorageBlock) + storageSize;
        character->componentAllocationBytes = static_cast<std::int16_t>(allocationBytes);
    }
    ModelComponentHeader *const component = reinterpret_cast<ModelComponentHeader *>(reinterpret_cast<std::byte *>(storage + 1) + storage->usedBytes);
    storage->usedBytes += payloadSize;
    std::memset(static_cast<void *>(component), 0, payloadSize);
    component->flags = 1;
    component->update = update;
    component->cleanup = 0;
    component->core = 0;
    component->tree.name = token;
    InsertNameTree(&character->tokenTree, &component->tree);
    component->next = 0;
    ModelComponentHeader **link = &character->modelComponents;
    while (*link)
        link = &(*link)->next;
    *link = component;
    return component + 1;
}

// @verified - d
void *charFindComp(const Character *character, shen_name token) {
    if (character) {
        NameTreeNode *const node = FindNameTree(character->tokenTree, token);
        if (node)
            return reinterpret_cast<ModelComponentHeader *>(node) + 1;
    }
    return 0;
}

// @verified - d
std::int32_t SetFlagForToken(Character *character, shen_name token, std::int32_t enabled) {
    if (character) {
        NameTreeNode *const node = FindNameTree(character->tokenTree, token);
        if (node) {
            ModelComponentHeader *const component = reinterpret_cast<ModelComponentHeader *>(node);
            component->flags = enabled ? component->flags | 1U : component->flags & ~1U;
            return 1;
        }
    }
    return 0;
}

// @verified - d
bool IsModelComponentEnabled(const Character *character, shen_name token) {
    if (!character)
        return false;
    const ModelComponentHeader *const header = reinterpret_cast<const ModelComponentHeader *>(FindNameTree(character->tokenTree, token));
    if (!header)
        return false;
    if (header->flags & 1U)
        return true;
    return false;
}

// @verified - d
void SetCharacterActive(Character *character, std::int32_t enabled) {
    if (character) {
        if (enabled)
            character->flags |= CF_Active;
        else
            character->flags &= ~CF_Active;
    }
}

// @verified - d
bool IsCharacterActive(const Character *character) {
    if (character) {
        if (character->flags & CF_Active)
            return true;
        return false;
    }
    return false;
}

void SetCharacterHandlerDisabled(Character *character, std::int32_t disabled) {
    if (character) {
        if (disabled)
            character->flags |= CF_HandlerDisabled;
        else
            character->flags &= ~CF_HandlerDisabled;
    }
}

// @verified - d
void SetModelComponentCleanup(Character *character, shen_name token, void (*cleanup)(Character *, void *)) {
    if (character) {
        ModelComponentHeader *const header = reinterpret_cast<ModelComponentHeader *>(FindNameTree(character->tokenTree, token));
        header->cleanup = cleanup;
    }
}

// @verified - d
void SetModelComponentCore(Character *character, shen_name token, void (*core)(Character *, void *)) {
    if (character)
        reinterpret_cast<ModelComponentHeader *>(FindNameTree(character->tokenTree, token))->core = core;
}

// @verified - d
void RunModelComponentCoreCallbacks(Character *character) {
    if (character) {
        for (ModelComponentHeader *component = character->modelComponents; component; component = component->next) {
            if (component->core)
                component->core(character, component + 1);
        }
    }
}

// @verified - d
void GetCharacterLocalPosition(const Character *character, Vector3 *position) {
    if (character) {
        const CharacterNode *const node = &character->node;
        *position = node->position;
    } else {
        position->x = 0.0f;
        position->y = 0.0f;
        position->z = 0.0f;
    }
}

// @verified - d
void SetCharacterLocalPosition(Character *character, const Vector3 *position) {
    if (character) {
        CharacterNode *const node = &character->node;
        node->position = *position;
    }
}

float GetCharacterLocalPositionY(const Character *character) {
    if (character)
        return character->node.position.y;
    return 0.0f;
}

void SetCharacterLocalPositionY(Character *character, float y) {
    if (character)
        character->node.position.y = y;
}

// @verified - d
void SetCharacterLocalRotation(Character *character, const Vector3i *rotation) {
    if (character) {
        CharacterNode *const node = &character->node;
        node->rotation.x = rotation->x;
        node->rotation.y = rotation->y;
        node->rotation.z = rotation->z;
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void shenSetRotY(Character *character, std::int32_t yaw) {
    if (character)
        character->node.rotation.y = yaw;
}
#endif

// @verified - d
void GetCharacterLocalRotation(const Character *character, Vector3i *rotation) {
    if (character) {
        const CharacterNode *const node = &character->node;
        rotation->x = node->rotation.x;
        rotation->y = node->rotation.y;
        rotation->z = node->rotation.z;
    }
}

// @verified - d
std::int32_t shenGetRotYFromNode(const Character *character) {
    if (character)
        return character->node.rotation.y;
    return 0;
}

// @verified - d
float GetCharacterAlpha(const Character *character) {
    if (character)
        return character->alpha;
    return 0.0f;
}

// @verified - d
void SetCharacterAlpha(Character *character, float alpha) {
    if (character)
        character->alpha = alpha;
}

float GetCharacterDrawDistance(const Character *character) {
    if (character)
        return character->drawDistance;
    return 0.0f;
}

void SetCharacterDrawDistance(Character *character, float distance) {
    if (character)
        character->drawDistance = distance;
}

void GetCharacterViewPosition(const Character *character, Vector3 *position) {
    if (character) {
        position->x = character->viewPosition.x;
        position->y = character->viewPosition.y;
        position->z = character->viewPosition.z;
    } else {
        position->x = 0.0f;
        position->y = 0.0f;
        position->z = 0.0f;
    }
}

void SetCharacterViewPosition(Character *character, const Vector3 *position) {
    if (character) {
        character->viewPosition.x = position->x;
        character->viewPosition.y = position->y;
        character->viewPosition.z = position->z;
    }
}

// @verified - d
void GetCharacterWorldPosition(const Character *character, Vector3 *position) {
    if (character)
        GetNodeWorldPosition(&character->node, position);
    else {
        position->x = 0.0f;
        position->y = 0.0f;
        position->z = 0.0f;
    }
}

// @verified - d
void GetCharacterWorldRotation(const Character *character, Vector3i *rotation) {
    if (character)
        GetNodeWorldRotation(&character->node, rotation);
    else {
        rotation->x = 0;
        rotation->y = 0;
        rotation->z = 0;
    }
}

// @verified - d
void charaSetWorldRot(Character *character, const Vector3i &rotation) {
    if (character)
        SetCharacterNodeWorldRotation(&character->node, rotation);
}

// @verified - d
std::int32_t GetCharacterWorldYaw(const Character *character) {
    if (!character)
        return 0;
    std::int16_t yaw = 0;
    for (const CharacterNode *node = &character->node; node; node = node->parent)
        yaw = static_cast<std::int16_t>(yaw + static_cast<std::int16_t>(node->rotation.y));
    return yaw;
}

// @verified - d
void SetCharacterWorldYaw(Character *character, std::int32_t yaw) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (character) {
        std::int16_t localYaw = static_cast<std::int16_t>(yaw);
        for (const CharacterNode *node = character->node.parent; node; node = node->parent)
            localYaw = static_cast<std::int16_t>(localYaw - static_cast<std::int16_t>(node->rotation.y));
        character->node.rotation.y = localYaw;
    }
#else
    if (!character)
        return;
    for (const CharacterNode *node = character->node.parent; node; node = node->parent)
        yaw = static_cast<std::int16_t>(yaw - static_cast<std::int16_t>(node->rotation.y));
    character->node.rotation.y = yaw;
#endif
}

// @verified - d
CharacterNode *GetCharacterNode(Character *character) {
    if (character)
        return &character->node;
    return 0;
}

// @verified - d
void GetCharacterPositionDelta(const Character *character, Vector3 *position) {
    if (character)
        *position = character->positionDelta;
}

// @verified - d
void SetCharacterPositionDelta(Character *character, const Vector3 *position) {
    if (character)
        character->positionDelta = *position;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void AssignCharacterChildSpacePosition(Character *character, const Vector3 *delta) {
    if (!character)
        return;
    Vector3 local = *delta;
    if (character->node.parent)
        InverseRotateVectorByCharacterNode(character->node.parent, &local);
    character->positionDelta = local;
}
#endif

// @verified - d
void coliPositionAdd(Character *character, const Vector3 *delta) {
    if (character) {
        character->positionDelta.x += delta->x;
        character->positionDelta.y += delta->y;
        character->positionDelta.z += delta->z;
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void InverseRotateVectorByCharacterNode(const CharacterNode *node, Vector3 *vector) {
    matrix_stack::GetMatrix(&g_characterNodeSavedTransform);
    matrix_stack::UnitCurrentMatrix();
    ApplyRecursiveCharacterNodeRotations(node);
    Matrix4x4 transform;
    matrix_stack::GetMatrix(&transform);
    matrix_stack::LoadMatrix(&g_characterNodeSavedTransform);
    const Vector4 delta = {vector->x - transform.rows[3].x, vector->y - transform.rows[3].y, vector->z - transform.rows[3].z, 0.0f};
    for (std::int32_t index = 2; index >= 0; --index)
        (*vector)[index] = matrix_stack::InnerProduct4(delta, transform.rows[index]);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void AddCharacterChildSpacePosition(Character *character, const Vector3 *delta) {
    if (!character)
        return;
    Vector3 local = *delta;
    if (character->node.parent)
        InverseRotateVectorByCharacterNode(character->node.parent, &local);
    character->positionDelta.x = character->positionDelta.x + local.x;
    character->positionDelta.y = character->positionDelta.y + local.y;
    character->positionDelta.z = character->positionDelta.z + local.z;
}
#endif

void SetCharacterPositionDeltaApplied(Character *character, std::int32_t applied) {
    if (character)
        character->positionDeltaApplied = applied;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void SetCharacterScale(Character *character, const Vector3 *scale) {
    if (character)
        character->scale = *scale;
}
#endif

// @verified - d
void GetCharacterScale(const Character *character, Vector3 *scale) {
    if (character)
        *scale = character->scale;
}

// @verified - d
void SetCharacterShadowOffset(Character *character, const Vector3 *offset) {
    if (character)
        character->shadowOffset = *offset;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void GetCharacterShadowOffset(const Character *character, Vector3 *offset) {
    if (character)
        *offset = character->shadowOffset;
}
#endif

// @verified - d
void SetCharacterShadowScale(Character *character, const Vector3 *scale) {
    if (character)
        character->shadowScale = *scale;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void GetCharacterShadowScale(const Character *character, Vector3 *scale) {
    if (character)
        *scale = character->shadowScale;
}
#endif

// @verified - d
void SetCharacterShadowYaw(Character *character, std::int32_t yaw) {
    if (character)
        character->shadowYaw = yaw;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t GetCharacterShadowYaw(const Character *character) {
    if (character)
        return character->shadowYaw;
    return 0;
}
#endif

// @verified - d
void SetCharacterType(Character *character, std::int32_t type) {
    if (character)
        character->type = type;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t GetCharacterType(const Character *character) {
    if (character)
        return character->type;
    return -1;
}
#endif

// @verified - d
void SetCharacterShadowY(Character *character, float y) {
    if (character)
        character->shadowY = y;
}

// @verified - d
void scenCreateModel(Character *character, const CharacterTableArgument *arguments, std::int32_t) {
    const std::uint32_t *cursor = arguments[0].source + 1;
    const std::uint32_t *const end = cursor + arguments[0].value.bits;
    while (cursor < end) {
        CharacterTableRecord record;
        cursor = ParseCharacterTableRecord(&record, shadowModelTokens, cursor);
        switch (record.tokenIndex) {
        case 0:
            SetCharacterType(character, 1);
            character->shadowModel = CreateShadowModel(record.arguments[0].value.string);
            break;
        case 1: {
            const float value = record.arguments[0].value.floating;
            std::int32_t type;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
            type = static_cast<std::int32_t>(value);
#else
            asm volatile("ftrc %1,FPUL\n\tsts FPUL,%0" : "=r"(type) : "f"(value) : "fpul");
#endif
            if (type > 11)
                type = 0;
            SetCharacterType(character, type);
            break;
        }
        case 2: {
            Vector3 offset;
            offset.x = record.arguments[0].value.floating;
            offset.y = record.arguments[1].value.floating;
            offset.z = record.arguments[2].value.floating;
            SetCharacterShadowOffset(character, &offset);
            break;
        }
        case 3: {
            Vector3 scale;
            scale.x = record.arguments[0].value.floating;
            scale.y = record.arguments[1].value.floating;
            scale.z = record.arguments[2].value.floating;
            SetCharacterShadowScale(character, &scale);
            break;
        }
        case 4: {
            const float degrees = (record.arguments[0].value.floating * 8192.0f) / 45.0f;
            std::int32_t angle;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
            angle = static_cast<std::int32_t>(degrees);
#else
            asm volatile("ftrc %1,FPUL\n\tsts FPUL,%0" : "=r"(angle) : "f"(degrees) : "fpul");
#endif
            SetCharacterShadowYaw(character, static_cast<std::uint16_t>(angle));
            break;
        }
        case 5:
            SetCharacterStoredShadowYEnabled(character, 1);
            SetCharacterShadowY(character, record.arguments[0].value.floating);
            break;
        }
    }
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
float GetCharacterShadowY(const Character *character) {
    if (character)
        return character->shadowY;
    return 0.0f;
}
#endif

// @verified - d
void EV_SetCharDisplay(Character *character, CharaVisibilityMode mode, std::int32_t enabled) {
    if (character && static_cast<std::uint32_t>(mode) < 7) {
        if (enabled)
            character->renderFlags |= characterVisibilityMasks[mode];
        else
            character->renderFlags &= static_cast<std::uint8_t>(~characterVisibilityMasks[mode]);
    }
}

// @verified - d
bool IsOfThisVisibilityType(const Character *character, CharaVisibilityMode mode) {
    if (character && static_cast<std::uint32_t>(mode) < 7 && (character->renderFlags & characterVisibilityMasks[mode]) == characterVisibilityMasks[mode])
        return true;
    return false;
}

// @verified - d
void charaSetVisibilityFlagTo(Character *character, std::uint32_t set) {
    if (character) {
        if (set)
            character->flags |= CF_VisibleThisCheck;
        else
            character->flags &= ~CF_VisibleThisCheck;
    }
}

// @verified - d
bool IsCharacterCollisionCallbackEnabled(const Character *character) {
    if (character) {
        if (character->flags & CHARACTER_COLLISION_CALLBACK_ENABLED)
            return true;
        return false;
    }
    return false;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
bool IsCharacterStoredShadowYEnabled(const Character *character) {
    if (character) {
        if (character->flags & CF_UseStoredShadowY)
            return true;
        return false;
    }
    return false;
}
#endif

void SetCharacterForceVisibleCheck(Character *character, std::int32_t enabled) {
    if (character) {
        if (enabled)
            character->flags |= CF_ForceVisibleCheck;
        else
            character->flags &= ~CF_ForceVisibleCheck;
    }
}

// @verified - d
void (*charaAssignCoreCallback(Character *character, void (*callback)(Character *)))(Character *) {
    if (character) {
        void (*const previous)(Character *) = character->core;
        character->core = callback;
        return previous;
    }
    return 0;
}

// @verified - d
CharacterUpdateCallback imgmAssignUpdateCallback(Character *character, CharacterUpdateCallback callback) {
    if (character) {
        const CharacterUpdateCallback previous = character->applyScale;
        character->applyScale = callback;
        return previous;
    }
    return 0;
}

// @verified - d
CharacterUpdateCallback charaAssignPrepareForDrawCallback(Character *character, CharacterUpdateCallback callback) {
    if (character) {
        const CharacterUpdateCallback previous = character->prepareForDraw;
        character->prepareForDraw = callback;
        return previous;
    }
    return 0;
}

// @verified - d
CharacterUpdateCallback GetMotionCallback(const Character *character) {
    if (character)
        return character->applyScale;
    return 0;
}

CharacterUpdateCallback GetCharacterPrepareForDrawCallback(const Character *character) {
    if (character)
        return character->prepareForDraw;
    return 0;
}

// @verified - d
void ExecuteCharacterCoreDraw(Character *character) {
    if (IsOfThisVisibilityType(character, CVM_All))
        HLib::ExecuteCallbackNow(reinterpret_cast<HLib::TaskCallback>(character->core), character);
}

// @verified - d
void ExecuteCharacterCollisionDraw(Character *character) {
    if (IsCharacterCollisionCallbackEnabled(character))
        HLib::ExecuteCallbackNow(character->collisionCheck, character);
}

// @verified - d
void charaHandlerDrawHuman(Character *character) {
    ExecuteCharacterCoreDraw(character);
    ExecuteCharacterCollisionDraw(character);
}

// @verified - d
void DrawHumanList(CharacterDrawTaskParameters *data) {
    CharacterDrawRecord *record = data->humans;
    std::int32_t count = data->humanCount;
    if (count > characterHumanDrawLimit)
        count = characterHumanDrawLimit;
    for (std::int32_t index = 0; index < count; ++index, ++record) {
        DrawLights(record->character, data);
        charaHandlerDrawHuman(record->character);
    }
}

// @verified - d
std::int32_t GatherHumanDrawList(Character *character, void *payload) {
    CharacterDrawTaskParameters *const data = static_cast<CharacterDrawTaskParameters *>(payload);
    if (!character || !data || character == data->player || !charFindMotionwork(character))
        return 1;
    CharacterDrawRecord *const records = data->humans;
    const std::int32_t count = data->humanCount;
    if (IsOfThisVisibilityType(character, CVM_All) && character->alpha != 0.0f) {
        float distance;
        if (character->viewPosition.z >= 0.0f)
            distance = kCharacterDrawUnprojectedDistance;
        else
            distance = character->drawDistance;
        std::int32_t index;
        for (index = 0; index < count; ++index) {
            if (records[index].distance > distance) {
                for (std::int32_t move = count; move > index; --move) {
                    records[move].character = records[move - 1].character;
                    records[move].distance = records[move - 1].distance;
                }
                break;
            }
        }
        records[index].character = character;
        records[index].distance = distance;
        ++data->humanCount;
        if (data->humanCount > kHumanDrawListCapacity)
            data->humanCount = kHumanDrawListCapacity;
    }
    return 1;
}

// @verified - d
void DrawObjectList(CharacterDrawTaskParameters *data) {
    CharacterDrawRecord *record = data->objects;
    std::int32_t count = data->objectCount;
    if (count > characterObjectDrawLimit)
        count = characterObjectDrawLimit;
    for (std::int32_t index = 0; index < count; ++index, ++record) {
        DrawLights(record->character, data);
        charaHandlerDrawHuman(record->character);
    }
}

// @verified - d
std::int32_t GatherObjectDrawList(Character *character, void *payload) {
    CharacterDrawTaskParameters *const data = static_cast<CharacterDrawTaskParameters *>(payload);
    if (character && data && character != data->player && !charFindMotionwork(character)) {
        CharacterDrawRecord *const records = data->objects;
        const std::int32_t count = data->objectCount;
        RunModelComponentCoreCallbacks(character);
        if (IsOfThisVisibilityType(character, CVM_All) && character->alpha != 0.0f) {
            float distance;
            if (character->viewPosition.z >= 0.0f)
                distance = kCharacterDrawUnprojectedDistance;
            else
                distance = character->drawDistance;
            std::int32_t index;
            for (index = 0; index < count; ++index) {
                if (records[index].distance > distance) {
                    for (std::int32_t move = count; move > index; --move) {
                        records[move].character = records[move - 1].character;
                        records[move].distance = records[move - 1].distance;
                    }
                    break;
                }
            }
            records[index].character = character;
            records[index].distance = distance;
            ++data->objectCount;
            if (data->objectCount > kObjectDrawListCapacity)
                data->objectCount = kObjectDrawListCapacity;
        }
    }
    return 1;
}

// @verified - d
void CharDraw_Callback(void *payload) {
    CharacterDrawTaskParameters *const data = static_cast<CharacterDrawTaskParameters *>(payload);
    data->player = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo;
    data->humanCount = 0;
    data->objectCount = 0;
    characterLightAuthorOverride = 0;
    if (g_worldDrawingEnabled) {
        SetupCharacterLightFrame(data);
        DrawLights(data->player, data);
        charaHandlerDrawHuman(data->player);
    }
}

// @verified - d
void CharDrawHuman_Callback(void *) {
    if (g_currentMap == DRAW_AUTHOR_AREA_WTA0 || g_currentMap == DRAW_AUTHOR_AREA_WSY0)
        characterLightAuthorOverride = 1;
    if (scenFindMapRegionName(0) == DRAW_AUTHOR_AREA_ARK0)
        characterLightAuthorOverride = 1;
    if (drGetCurrentAuthorId())
        characterLightAuthorOverride = 1;
    if (g_worldDrawingEnabled) {
        CharacterDrawTaskParameters *const data = static_cast<CharacterDrawTaskParameters *>(HLib::GetTaskParameter(characterDrawTask));
        ForEachLiveCharacterInMapOrder(data, GatherHumanDrawList, 0);
        DrawHumanList(data);
    }
}

// @verified - d
void CharDrawObject_Callback(void *) {
    if (g_worldDrawingEnabled) {
        CharacterDrawTaskParameters *const data = static_cast<CharacterDrawTaskParameters *>(HLib::GetTaskParameter(characterDrawTask));
        ForEachLiveCharacterInMapOrder(data, GatherObjectDrawList, 0);
        DrawObjectList(data);
        RestoreCharacterLightFrame(data);
    }
}

// @verified - d
void charaCore(Character *character) {
    if (!character->model)
        return;
    matrix_stack::Matrix4x4_Push(0);
    ApplyRecursiveCharacterNodeTransforms(&character->node);
    if (character->scale.x != 1.0f || character->scale.y != 1.0f || character->scale.z != 1.0f) {
        Matrix4x4::Scale(character->scale.x, character->scale.y, character->scale.z);
        dc::sdk::SetModelScale(character->scale.x);
    }
    const CharacterUpdateCallback callback = GetMotionCallback(character);
    callback(character->model, character->alpha);
    dc::sdk::SetModelScale(1.0f);
    if (IsCharacterMirroredForDraw(character)) {
        PopCurrentMatrix(1);
        matrix_stack::Matrix4x4_Push(0);
        Matrix4x4::MultiplyWithPrePassMatrix(GetCharacterMirrorMatrix());
        ApplyRecursiveCharacterNodeTransforms(&character->node);
        if (character->scale.x != 1.0f || character->scale.y != 1.0f || character->scale.z != 1.0f)
            Matrix4x4::Scale(character->scale.x, character->scale.y, character->scale.z);
        dc::sdk::SetMirroredModelDrawMode(1);
        callback(character->model, 1.0f);
        dc::sdk::SetMirroredModelDrawMode(0);
    }
    PopCurrentMatrix(1);
}

void DrawDummyCharacterCore(Character *character) {
    if (!character->model)
        return;
    CharacterNode *const node = &character->node;
    matrix_stack::Matrix4x4_Push(0);
    if (node->parent)
        ApplyRecursiveCharacterNodeTransforms(node->parent);
    Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
    if (node->rotation.y)
        Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
    if (node->rotation.z)
        Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
    if (node->rotation.x)
        Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
    if (character->scale.x != 1.0f || character->scale.y != 1.0f || character->scale.z != 1.0f)
        Matrix4x4::Scale(character->scale.x, character->scale.y, character->scale.z);
    GetMotionCallback(character)(character->model, character->alpha);
    PopCurrentMatrix(1);
}

void (*charaAssignDummyCore(Character *character))(Character *) {
    return charaAssignCoreCallback(character, DrawDummyCharacterCore);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void charaCollisionDistanceChecks(void *data) {
    Character *const character = static_cast<Character *>(data);
    Vector3 position, scale, offset;
    Vector3i rotation;
    const MT7Node *root;
    std::int32_t yaw;
    void *mesh;
    if (!character->shadowModel)
        return;
    matrix_stack::Matrix4x4_Push(0);
    GetCharacterWorldPosition(character, &position);
    GetCharacterWorldRotation(character, &rotation);
    float groundY;
    if (!IsCharacterStoredShadowYEnabled(character)) {
        Vector3 probe;
        probe.x = position.x;
        probe.y = position.y + 0.5f;
        probe.z = position.z;
        groundY = FindFieldIntersectionHeight(&probe);
        if (groundY < -1000.0f)
            groundY = position.y;
        else if (groundY > position.y)
            groundY = position.y;
        groundY += 0.01f;
    } else
        groundY = GetCharacterShadowY(character);
    const CharacterUpdateCallback callback = GetCharacterPrepareForDrawCallback(character);
#define SHENMUE_FIXED_SHADOW(meshIndex, strength)                                                                                                              \
    do {                                                                                                                                                       \
        mesh = GetFixedShadowMesh(meshIndex);                                                                                                                  \
        GetCharacterShadowOffset(character, &offset);                                                                                                          \
        yaw = GetCharacterShadowYaw(character);                                                                                                                \
        GetCharacterShadowScale(character, &scale);                                                                                                            \
        position.x += offset.x;                                                                                                                                \
        position.y += offset.y;                                                                                                                                \
        position.z += offset.z;                                                                                                                                \
        rotation.y = static_cast<std::int32_t>(static_cast<std::uint32_t>(rotation.y) + static_cast<std::uint32_t>(yaw));                                      \
        DrawProjectedFixedShadow(&position, &rotation, &scale, groundY, mesh, strength, character);                                                            \
    } while (0)
#define SHENMUE_CHARACTER_SHADOW(shadowRoot, strength)                                                                                                         \
    do {                                                                                                                                                       \
        root = shadowRoot;                                                                                                                                     \
        GetCharacterScale(character, &scale);                                                                                                                  \
        DrawProjectedCharacterShadow(callback, &position, &rotation, &scale, groundY, strength, character->model, root, character);                            \
    } while (0)
    switch (GetCharacterType(character)) {
    case 2:
        SHENMUE_FIXED_SHADOW(1, -1.0f);
        break;
    case 9:
        SHENMUE_FIXED_SHADOW(1, 1.0f);
        break;
    case 3:
        SHENMUE_FIXED_SHADOW(0, -1.0f);
        break;
    case 1:
        SHENMUE_CHARACTER_SHADOW(character->shadowModel == GetSHDS() ? character->model : character->shadowModel, 1.0f);
        break;
    case 10:
        SHENMUE_CHARACTER_SHADOW(character->shadowModel == GetSHDS() ? character->model : character->shadowModel, -1.0f);
        break;
    case 6:
        SHENMUE_CHARACTER_SHADOW(character->model, 0.5f);
        break;
    case 0:
    default:
        SHENMUE_CHARACTER_SHADOW(character->model, -1.0f);
        break;
    case 8:
        SHENMUE_CHARACTER_SHADOW(character->model, 1.0f);
        break;
    case 11:
        SHENMUE_CHARACTER_SHADOW(character->model, -2.0f);
        break;
    }
#undef SHENMUE_FIXED_SHADOW
#undef SHENMUE_CHARACTER_SHADOW
    PopCurrentMatrix(1);
}
#endif

void ApplyCharacterNodeTransformAndScale(Character *character) {
    if (character) {
        ApplyRecursiveCharacterNodeTransforms(&character->node);
        if (character->scale.x != 1.0f || character->scale.y != 1.0f || character->scale.z != 1.0f)
            Matrix4x4::Scale(character->scale.x, character->scale.y, character->scale.z);
    }
}

CharacterPropertySlot *FindCharacterPropertySlot(Character *character, std::int32_t index) {
    if (!character || index == -1)
        return 0;
    CharacterPropertyBlock *block = reinterpret_cast<CharacterPropertyBlock *>(character->auxiliaryComponentStorage);
    while (index > 31) {
        index -= 32;
        block = block->next;
    }
    return &block->slots[index];
}

void AllocateCharacterPropertyBlock(Character *character) {
    if (!character)
        return;
    std::int32_t index = 0;
    CharacterPropertyBlock **link = reinterpret_cast<CharacterPropertyBlock **>(&character->auxiliaryComponentStorage);
    if (*link) {
        do {
            link = &(*link)->next;
            index += 32;
        } while (*link);
    }
    CharacterPropertyBlock *const block = static_cast<CharacterPropertyBlock *>(HLib::AllocateTaskData(sizeof(CharacterPropertyBlock), MEMORY_PROP));
    *link = block;
    block->next = 0;
    std::int32_t slot;
    for (slot = 0; slot < 31; ++slot)
        block->slots[slot].lower = static_cast<std::int16_t>(++index);
    block->slots[slot].lower = character->retainedComponentState[1];
    character->retainedComponentState[1] = static_cast<std::int16_t>(index - 31);
}

CharacterPropertySlot *FindOrCreateCharacterProperty(Character *character, std::uint32_t key) {
    if (!character)
        return 0;
    std::int16_t *link = &character->retainedComponentState[0];
    CharacterPropertySlot *slot;
    while (*link != -1) {
        slot = FindCharacterPropertySlot(character, *link);
        if (slot->key < key)
            link = &slot->lower;
        else if (slot->key > key)
            link = &slot->higher;
        else
            return slot;
    }
    if (character->retainedComponentState[1] == -1)
        AllocateCharacterPropertyBlock(character);
    const std::int16_t index = character->retainedComponentState[1];
    slot = FindCharacterPropertySlot(character, index);
    character->retainedComponentState[1] = slot->lower;
    slot->higher = -1;
    slot->lower = -1;
    slot->key = key;
    slot->value = 0;
    *link = index;
    return slot;
}

void SetCharacterProperty(Character *character, std::uint32_t key, std::uint32_t value) {
    if (character)
        FindOrCreateCharacterProperty(character, key)->value = value;
}

std::uint32_t GetCharacterProperty(Character *character, std::uint32_t key) {
    if (character)
        return FindOrCreateCharacterProperty(character, key)->value;
    return 0;
}

void SetCharacterFlag80000000(Character *character, std::int32_t enabled) {
    if (character) {
        if (enabled)
            character->flags |= 0x80000000U;
        else
            character->flags &= 0x7fffffffU;
    }
}

bool IsCharacterFlag80000000Set(const Character *character) {
    if (character) {
        if (static_cast<std::int32_t>(character->flags) < 0)
            return true;
        return false;
    }
    return false;
}

// @verified - d
void SetCharacterRadius(Character *character, float radius) {
    if (character)
        character->radius = radius;
}

// @verified - d
void InitializeMapCharacterNullsub() {
}

// @verified - d
void InitializeMapCharacterNullsubAlt() {
}

// @verified - d
void InitializeHumanCharacterNullsub(Character *) {
}

// @verified - d
void CharacterModuleNullsubA() {
}

// @verified - d
void CharacterModuleNullsubB() {
}

// @verified - d
std::int32_t CharacterModuleNullsubC() {
    return 0;
}

// @verified - d
void CharacterModuleNullsubD() {
}

// @verified - d
void CharacterModuleNullsubE() {
}

void CharacterModuleNullsubF() {
}

void CharacterModuleNullsubG() {
}

void CharacterModuleNullsubH() {
}

void CharacterModuleNullsubI() {
}

void CharacterModuleNullsubJ() {
}

void CharacterModuleNullsubK() {
}

void CharacterModuleNullsubL() {
}

void CharacterModuleNullsubM() {
}

std::int32_t CharacterModuleNullsubN() {
    return 0;
}

std::int32_t CharacterModuleNullsubO() {
    return 0;
}

void CharacterModuleNullsubP() {
}

void CharacterModuleNullsubQ() {
}

void CharacterModuleNullsubR() {
}

void CharacterModuleNullsubS() {
}

std::int32_t CharacterModuleNullsubT() {
    return 0;
}

// @verified - d
void SetCharacterClockModeNullsub(Character *, std::int16_t) {
}

// @verified - d
float GetCharacterDisplayRadius(const Character *character) {
    if (character)
        return character->radius;
    return 0.0f;
}

// @verified - d
void SetCharacterLayer(Character *character, std::int32_t layer) {
    if (character)
        character->node.layer = layer;
}

// @verified - d
std::int32_t GetCharacterLayer(const Character *character) {
    if (character)
        return character->node.layer;
    return 0;
}

std::uint16_t GetCharacterComponentAllocationBytes(const Character *character) {
    if (character)
        return static_cast<std::uint16_t>(character->componentAllocationBytes);
    return 0;
}

// @verified - d
bool InsertNameTree(NameTreeNode **root, NameTreeNode *node) {
    node->higher = 0;
    node->lower = 0;
    while (*root) {
        if (node->name == (*root)->name)
            return false;
        root = static_cast<std::int32_t>(node->name) > static_cast<std::int32_t>((*root)->name) ? &(*root)->higher : &(*root)->lower;
    }
    *root = node;
    return true;
}

// @verified - d
bool RemoveNameTree(NameTreeNode **root, NameTreeNode *node) {
    for (;;) {
        NameTreeNode *const current = *root;
        if (current == node) {
            if (!node->higher) {
                *root = node->lower;
            } else if (!node->lower) {
                *root = node->higher;
            } else {
                NameTreeNode **replacementLink = &node->lower;
                while ((*replacementLink)->higher)
                    replacementLink = &(*replacementLink)->higher;
                NameTreeNode *const replacement = *replacementLink;
                *replacementLink = replacement->lower;
                replacement->lower = node->lower;
                replacement->higher = node->higher;
                *root = replacement;
            }
            return true;
        }
        if (node->name == current->name)
            return false;
        root = static_cast<std::int32_t>(node->name) > static_cast<std::int32_t>(current->name) ? &current->higher : &current->lower;
    }
}

// @verified - d
NameTreeNode *FindNameTree(NameTreeNode *root, shen_name name) {
    while (root && root->name != name)
        root = static_cast<std::int32_t>(name) > static_cast<std::int32_t>(root->name) ? root->higher : root->lower;
    return root;
}

// @verified - d
shen_name GetCharacterResourceOwner(const Character *character) {
    if (character)
        return character->resourceOwner;
    return CHARACTER_INVALID_NAME;
}

// @verified - d
void SetCharacterModelName(Character *character, shen_name name) {
    if (character)
        character->resourceOwner = name;
}

std::uint32_t charaHasMirroredFlags(const Character *character) {
    if (character)
        return character->flags & CF_DrawMirrored;
    return 0;
}

std::uint32_t GetCharacterFlags(const Character *character) {
    return character->flags;
}

// @verified - d
std::uint32_t *GetControllerForNode(Character *character) {
    return character ? reinterpret_cast<std::uint32_t *>(&character->retainedLightState) : 0;
}

// @verified - d
void BindCharacterFacialModel(Character *character) {
    if (character) {
        MT7Model *const source = GetImageModuleSource(character);
        if (source)
            character->facialModel = source->face;
        else
            character->facialModel = 0;
        if (character->facialModel)
            GraftFacialModelNode(character->facialModel, character->model);
    }
}

// @verified - d
void UpdateFacialMorphs(Character *character) {
    if (character && character->facialModel)
        AdvanceFacialModel(*static_cast<FacialModelData *>(character->facialModel));
}

void PrepareFacialExpression(FacialModelData &face, std::int32_t expression, std::int32_t transitionFrames, float scale) {
    PrepareUpperFace(face, expression, transitionFrames, scale);
    PrepareLowerFace(face, expression, transitionFrames, scale);
}

void SetHighModelAndFaceFlagsForChara(Character *character, std::int32_t expression, std::int32_t transitionFrames, float scale) {
    if (character && character->facialModel)
        PrepareFacialExpression(*static_cast<FacialModelData *>(character->facialModel), expression, transitionFrames, scale);
}

// @verified - d
void SetUpperFacialExpression(Character *character, std::int32_t expression, std::int32_t transitionFrames, float scale) {
    if (character && character->facialModel) {
        if (scale > 2.0f)
            scale = 1.0f;
        PrepareUpperFace(*static_cast<FacialModelData *>(character->facialModel), expression, transitionFrames, scale);
    }
}

// @verified - d
void SetLowerFacialExpression(Character *character, std::int32_t expression, std::int32_t transitionFrames, float scale) {
    if (character && character->facialModel)
        PrepareLowerFace(*static_cast<FacialModelData *>(character->facialModel), expression, transitionFrames, scale);
}

// @verified - d
void SetFacialMouthExpression(Character *character, std::int32_t expression, std::int32_t transitionFrames, float scale) {
    if (character && character->facialModel)
        PrepareFacialMouth(*static_cast<FacialModelData *>(character->facialModel), expression, transitionFrames, scale);
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t IsFacialMouthIdle(const FacialModelData *face);

std::int32_t IsCharacterFacialMouthIdle(Character *character) {
    if (!character)
        return 0;
    FacialModelData *const face = static_cast<FacialModelData *>(character->facialModel);
    if (!face)
        return 0;
    return IsFacialMouthIdle(face);
}
#endif

std::int32_t HasCharacterFacialModel(const Character *character) {
    return character && character->facialModel;
}

// @verified - d
bool IsFacialReplacementSelected(const Character *character) {
    bool selectedReplacement = false;
    if (character) {
        const FacialModelData *const face = reinterpret_cast<const FacialModelData *>(character->facialModel);
        if (face) {
            MT7Node *const *const selected = reinterpret_cast<MT7Node *const *>(static_cast<std::uint32_t>(face->modelOffset));
            if (selected)
                selectedReplacement = static_cast<std::uint32_t>(face->reserved14) == reinterpret_cast<std::uintptr_t>(*selected);
        }
    }
    return selectedReplacement;
}

// @verified - d
void SelectCharacterFacialNode(Character *character, std::int32_t replacement) {
    if (!character)
        return;
    FacialModelData *const face = static_cast<FacialModelData *>(character->facialModel);
    if (face && face->modelOffset) {
        MT7Node **const selected = reinterpret_cast<MT7Node **>(static_cast<std::uint32_t>(face->modelOffset));
        *selected = reinterpret_cast<MT7Node *>(static_cast<std::uint32_t>(replacement ? face->reserved14 : face->reserved10));
        if (character->model)
            SetMT7Parents(character->model, 0);
    }
    mtwkSetSequenceFlags(character);
}

// @verified - d
void StartFacialBlink(Character *character) {
    if (character && character->facialModel)
        BeginFacialBlink(*static_cast<FacialModelData *>(character->facialModel));
}

// @verified - d
void AT_SetEyeCtrl(Character *character, float transitionFrames, float leftWeight, float rightWeight) {
    if (!character)
        return;
    FacialModelData *const face = static_cast<FacialModelData *>(character->facialModel);
    if (!face)
        return;
    if (1.0f > transitionFrames)
        transitionFrames = 0.0f;
    face->eyelidDuration = 0;
    SetOuterEyelidWeights(*face, transitionFrames, leftWeight * FACIAL_EYE_OUTER_SCALE, rightWeight * FACIAL_EYE_OUTER_SCALE);
    SetInnerEyelidWeights(*face, transitionFrames, leftWeight * FACIAL_EYE_INNER_SCALE, rightWeight * FACIAL_EYE_INNER_SCALE);
    face->eyelidFrame = 0;
    std::int32_t duration;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    duration = static_cast<std::int32_t>(transitionFrames);
#else
    asm volatile("ftrc %1,fpul\n\tsts fpul,%0" : "=r"(duration) : "f"(transitionFrames) : "fpul");
#endif
    face->eyelidDuration = duration;
}

// @verified - d
void ClearCharacterFacialModel(Character *character) {
    if (character)
        character->facialModel = 0;
}

void FacialDisplacement(Character *character, Vector3 *displacement) {
    if (character && character->facialModel)
        GetFacialOsageDisplacement(character->facialModel, displacement);
}

// @verified - d
void SetCharacterEyelidPitch(Character *character, float pitch) {
    if (character && character->facialModel)
        SetFacialEyelidPitch(*static_cast<FacialModelData *>(character->facialModel), pitch);
}

void CharacterWorldToParentSpace(Character *character, const Vector3 *position, Vector3 *output) {
    const float *source = &position->x;
    output->x = *source++;
    output->y = *source++;
    output->z = *source;
    CharacterNode *const parent = GetCharacterNode(character)->parent;
    if (parent)
        InverseTransformPointByCharacterNode(parent, output);
}

void CharacterParentToWorldSpace(Character *character, const Vector3 *position, Vector3 *output) {
    const float *source = &position->x;
    output->x = *source++;
    output->y = *source++;
    output->z = *source;
    CharacterNode *const parent = GetCharacterNode(character)->parent;
    if (parent)
        TransformPointByCharacterNode(parent, output);
}

// @verified - d
void DrawImageModelSelection(Character *character, std::uint32_t flags) {
    MT7Node *const root = character->model;
    if (!root)
        return;
    const std::int32_t frameIndex = flags & 0xffU;
    MT7Node *group = root->child;
    for (std::uint32_t index = (flags >> 8U) & 15U; index; --index)
        group = group->sibling;
    MT7Node *frame = group->child;
    for (std::int32_t index = 0; index < frameIndex; ++index)
        frame = frame->sibling;
    matrix_stack::Matrix4x4_Push(0);
    ApplyRecursiveCharacterNodeTransforms(&character->node);
    if (character->scale.x != 1.0f || character->scale.y != 1.0f || character->scale.z != 1.0f) {
        Matrix4x4::Scale(character->scale.x, character->scale.y, character->scale.z);
        dc::sdk::SetModelScale(character->scale.x);
    }
    const CharacterUpdateCallback callback = GetMotionCallback(character);
    callback(root, GetCharacterAlpha(character));
    callback(group, GetCharacterAlpha(character));
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    const float alpha = GetCharacterAlpha(character);
    CharacterUpdateCallback drawBranch = reinterpret_cast<CharacterUpdateCallback>(DrawCharacterModelBranch);
    drawBranch(frame, alpha);
#else
    DrawCharacterModelBranch(frame, GetCharacterAlpha(character));
#endif
    dc::sdk::SetModelScale(1.0f);
    if (IsCharacterMirroredForDraw(character)) {
        PopCurrentMatrix(1);
        matrix_stack::Matrix4x4_Push(0);
        Matrix4x4::MultiplyWithPrePassMatrix(GetCharacterMirrorMatrix());
        ApplyRecursiveCharacterNodeTransforms(&character->node);
        if (character->scale.x != 1.0f || character->scale.y != 1.0f || character->scale.z != 1.0f)
            Matrix4x4::Scale(character->scale.x, character->scale.y, character->scale.z);
        dc::sdk::SetMirroredModelDrawMode(1);
        callback(root, 1.0f);
        callback(group, 1.0f);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        drawBranch(frame, 1.0f);
#else
        DrawCharacterModelBranch(frame, 1.0f);
#endif
        dc::sdk::SetMirroredModelDrawMode(0);
    }
    PopCurrentMatrix(1);
}

// @verified - d
std::int32_t SetPluralControlFor(Character *character, std::int16_t state) {
    std::int32_t previous = 1;
    std::int16_t *const controller = reinterpret_cast<std::int16_t *>(GetControllerForNode(character));
    if (controller) {
        previous = *controller;
        *controller = state;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        reinterpret_cast<std::uint8_t *>(controller)[3] = state ? 1 : characterPluralControlOffState;
#else
        reinterpret_cast<std::uint8_t *>(controller)[3] = state ? 1 : 2;
#endif
    }
    return previous;
}

std::int32_t GetCharacterDrawControlEnabled(Character *character) {
    std::int8_t enabled = 0;
    std::uint32_t *const controller = GetControllerForNode(character);
    if (controller)
        enabled = reinterpret_cast<std::int8_t *>(controller)[2];
    return enabled != 0;
}

// @verified - d
void SetCharacterDrawControlEnabled(Character *character, std::int32_t enabled) {
    std::uint32_t *const controller = GetControllerForNode(character);
    if (controller)
        reinterpret_cast<std::uint8_t *>(controller)[2] = enabled != 0;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t CheckIsArgEquZero(std::int32_t value) {
    return value == 0;
}
#endif

// @verified - d
std::int32_t GetNpcBatchCountOverride() {
    return characterHumanDrawLimit;
}

// @verified - d
void ExchangeLegacyModelColour(const Vector4 *colour, Vector4 *previous) {
    if (previous)
        *previous = legacyModelColour;
    legacyModelColour = *colour;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SetNewNpcBatchCountOverride(std::int32_t mode, std::int32_t count) {
    npcPoolSizingModeOrOverride = mode;
    characterHumanDrawLimit = count;
    if (!mode || count > 49)
        characterHumanDrawLimit = 49;
}

void SetChildObjectFlagByIndex(Character *character, std::uint8_t mode) {
    if (mode >= 5 || !character->model)
        return;
    MT7Node *const right = FindMT7Node(character->model, 5);
    if (!right)
        return;
    MT7Node *const left = FindMT7Node(character->model, 10);
    if (!left)
        return;
    std::uint32_t *const rightFlags = reinterpret_cast<std::uint32_t *>(right) + 0x21;
    std::uint32_t *const leftFlags = reinterpret_cast<std::uint32_t *>(left) + 0x21;
    *rightFlags = (*rightFlags & 0xffffU) | (static_cast<std::uint32_t>(mode) << 16);
    *leftFlags = (*leftFlags & 0xffffU) | (static_cast<std::uint32_t>(mode) << 16);
}

#endif

} // namespace s2
} // namespace shenmue

#endif
