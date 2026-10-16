#pragma once

#include <cstddef>
#include <cstdint>

#include "s1/matrix_stack.hpp"

namespace shenmue::s1 {
struct ScenarioContext;

struct GameNode {
    std::int32_t bufferHandle;
    std::int32_t abc;
    std::int32_t renderFlags;
    char entityName[6];
    std::int16_t bNum;
    std::uint32_t vertexCount;
    std::int32_t materialTexture;
    std::uint32_t field_1c;
    std::byte reserved_020[1524];
};

static_assert(sizeof(GameNode) == 0x614);

#pragma pack(push, 4)
struct SceneEntityTable {
    GameNode *entries[65536];
    std::int32_t keys[65536];
    std::int16_t chainOffsets[65536];
    std::int32_t entityCount;
};
#pragma pack(pop)

static_assert(sizeof(SceneEntityTable) == 0xe0004);
static_assert(offsetof(SceneEntityTable, keys) == 0x80000);
static_assert(offsetof(SceneEntityTable, chainOffsets) == 0xc0000);

extern SceneEntityTable g_Scene;
extern std::int32_t g_UploadedSceneRenderHandles[32768];
extern std::uint32_t g_UploadedSceneRenderHandleCount;
extern Vector3 g_sceneBoundsMin;
extern Vector3 g_sceneBoundsMax;
extern Vector3 g_scriptTransformScale;

void FreeSceneSpace(std::uint64_t index);
void InitSceneDataWrapper();
GameNode **AddEntryToSceneTable(std::int32_t key);
GameNode *FindSceneEntityByID(std::int32_t key);
void MT5_RemoveSceneModel(std::int32_t key);
void D3T_SetSceneNodeWorldTransform(const GameNode *node, const Matrix4x4 &transform);
bool D3T_GetSceneNodeWorldTransform(const GameNode *node, Matrix4x4 *transform);
void D3T_RemoveSceneNodeWorldTransform(const GameNode *node);
void Script_TranslateRotateScale_Wrapper(ScenarioContext *context, void *arguments);

} // namespace shenmue::s1
