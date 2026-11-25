#pragma once

#include "s2/hlib.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/character_control_dc.hpp"

namespace shenmue {
namespace s2 {

static const std::size_t kMaximumCharacterCollisionCount = 129;
static const float kCharacterControlEpsilon = 0.00000011920929f;
struct CharacterCollisionFrame {
    Vector3 position;
    CharacterControlModel *controls[kMaximumCharacterCollisionCount + 1];
};
typedef char Layout_character_collision_task_dc_0[(sizeof(CharacterCollisionFrame) == 0x214) ? 1 : -1];
typedef char Layout_character_collision_task_dc_1[(offsetof(CharacterCollisionFrame, controls) == 0x0c) ? 1 : -1];
typedef char Layout_character_collision_task_dc_2
    [(offsetof(CharacterCollisionFrame, controls) + kMaximumCharacterCollisionCount * sizeof(CharacterControlModel *) == 0x210) ? 1 : -1];
#define TASK_CCT SHEN_NAME4('C', 'C', 'T', '_')
extern void *g_characterCollisionTaskData;
std::int32_t CharCollisionSort(Character *character, void *context);
bool IsConversationCameraCharacter(const Character *character);
std::int32_t ResolveCharacterControlPair(CharacterControlModel *first, CharacterControlModel *second);
std::int32_t GatherCharacterControls(CharacterControlModel **controls);
void ccCharaColiTask_Callback(void *data);
std::int32_t Queue_ccCharaColiTask(std::int32_t nextFunctionIndex);

} // namespace s2
} // namespace shenmue

#endif
