#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/game_display_dc.hpp"
#include "s2/event_matrix_dc.hpp"

#include "platform/dc/matrix.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/model_render_dc.hpp"
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

void ApplyGameDisplayTransform(const MT7Node &node);

} // namespace model_component_detail

bool AttachGameDisplayComponent(Character *character);
void RunGameCallback(Character *character, void *payload);
void GAME_Callback(void *payload);
void StageGameDisplayNode(GameDisplayWork *game, std::uint16_t nodeId, const Vector3 *position, float alpha, const Vector3 *scale);
std::uint64_t gameSetObjectFlags(GameDisplayWork *game, std::uint32_t value, const Vector3 *position, float alpha, const Vector3 *scale);
std::uint64_t scenCopyCharaToGame(shen_name id);
void scenGetCharaGameNode(shen_name id, std::uint16_t nodeId, const Vector3 *position, float alpha, const Vector3 *scale);
GameDisplayWork *SetPosRotForGame(shen_name id, std::uint32_t baseNodeId, const Vector3 *digitStep, std::int32_t maximumValue, std::int32_t decimalShift);
std::uint64_t UpdateGameNodeFlags(shen_name id, std::uint32_t value, const Vector3 *position, float alpha, const Vector3 *scale);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

inline void ApplyGameDisplayTransform(const MT7Node *node) {
    Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
    if (node->rotation.z)
        Matrix4x4::RotateAroundZ_Wrapper(node->rotation.z);
    if (node->rotation.y)
        Matrix4x4::RotateAroundY_Wrapper(node->rotation.y);
    if (node->rotation.x)
        Matrix4x4::RotateAroundX_Wrapper(node->rotation.x);
    if (node->scale.x != 1.0f || node->scale.y != 1.0f || node->scale.z != 1.0f)
        Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
}

} // namespace

#else

namespace model_component_detail {

// @verified - p
MT7Node *FindGameDisplayNode(MT7Node *node, std::uint16_t id) {
    if (!node)
        return nullptr;
    if (static_cast<std::uint16_t>(node->id) == id)
        return node;
    if (MT7Node *const child = FindGameDisplayNode(node->runtimeChild, id))
        return child;
    return FindGameDisplayNode(node->runtimeSibling, id);
}

// @verified - p
std::uint64_t AppendGameDisplayEntry(GameDisplayWork *game, MT7Node *node, const Vector3 *position, float alpha, const Vector3 *scale) {
    GameDisplayEntry &entry = game->staged[static_cast<std::ptrdiff_t>(game->stagedCount)];
    entry.node = node;
    entry.nodeIdentity = static_cast<std::int16_t>(node->id);
    entry.position.x = position->x;
    entry.position.y = position->y;
    entry.position.z = position->z;
    entry.alpha = alpha;
    std::uint32_t result = static_cast<std::uint32_t>(entry.nodeIdentity);
    if (scale) {
        entry.scale.x = scale->x;
        entry.scale.y = scale->y;
        result = std::bit_cast<std::uint32_t>(scale->z);
        entry.scale.z = std::bit_cast<float>(result);
    } else {
        entry.scale = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
    }
    game->stagedCount = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(game->stagedCount) + 1U);
    return result;
}

// @verified - p
Character *FindGameDisplayCharacter(shen_name id) {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return GetScenarioCharacterNode(control->scenarioTask, id);
}

// @verified - p
void ApplyGameDisplayTransform(const MT7Node &node) {
    Matrix4x4::preTranslate(node.position.x, node.position.y, node.position.z);
    if (node.rotation.z)
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node.rotation.z));
    if (node.rotation.y)
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node.rotation.y));
    if (node.rotation.x)
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node.rotation.x));
    if (node.scale.x != 1.0f || node.scale.y != 1.0f || node.scale.z != 1.0f)
        Matrix4x4::Scale(node.scale.x, node.scale.y, node.scale.z);
}

// @verified - p
void DrawGameDisplayEntry(const MT7Model &model, const GameDisplayEntry &entry) {
    if (entry.alpha == 0.0f)
        return;
    const MT7Node &node = *entry.node;
    const MT7Node &meshNode = node.borrowedMesh ? *node.borrowedMesh : node;
    const std::span<const std::byte> bytes = node.commandOverride ? node.commandOverride->bytes : std::span<const std::byte>(model.source);
    const std::uint32_t offset = node.commandOverride ? node.commandOverride->xb01Offset : node.xb01Offset;
    const LegacyDrawResources &resources = node.commandOverride && node.commandOverride->resources ? *node.commandOverride->resources : meshNode.drawResources;
    if (entry.alpha == 1.0f)
        ModelFlagDispatch_SetModelFlagDataOpaque(bytes, offset, resources);
    else
        SetModelFlagDataWithAlpha(bytes, offset, resources, entry.alpha);
}

// @verified - p
void InsertGameModelNodes(MT7Node *source, Character *character) {
    if (!source || !character)
        return;
    GameDisplayWork *const game = static_cast<GameDisplayWork *>(FindModelComponent(character, MODEL_COMPONENT_GAME));
    if (!game || !game->inserted)
        return;
    if (game->previous) {
        game->inserted->runtimeSibling = game->previous->runtimeSibling;
        game->previous->runtimeSibling = game->inserted;
    } else {
        MT7Node *const parent = game->inserted->runtimeParent;
        game->inserted->runtimeSibling = parent->runtimeChild;
        parent->runtimeChild = game->inserted;
    }
}

// @verified - p
void RestoreGameModelNodes(MT7Node *source, Character *character) {
    if (!source || !character)
        return;
    GameDisplayWork *const game = static_cast<GameDisplayWork *>(FindModelComponent(character, MODEL_COMPONENT_GAME));
    if (!game || !game->inserted)
        return;
    if (game->previous)
        game->previous->runtimeSibling = game->inserted->runtimeSibling;
    else
        game->inserted->runtimeParent->runtimeChild = game->inserted->runtimeSibling;
    game->inserted->runtimeSibling = nullptr;
}

} // namespace model_component_detail

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void GAME_Callback(void *parameter) {
    Character *const character = static_cast<Character *>(parameter);
    if (!g_worldDrawingEnabled || !character)
        return;
    const GameDisplayWork *const game = reinterpret_cast<const GameDisplayWork *>(charFindComp(character, MODEL_COMPONENT_GAME));
    if (!game)
        return;
    const MT7Node *node = character->model;
    matrix_stack::Matrix4x4_Push(0);
    ApplyRecursiveCharacterNodeTransforms(GetCharacterNode(character));
    ApplyGameDisplayTransform(node);
    matrix_stack::Matrix4x4_Push(0);
    for (std::int32_t index = 0; index < game->stagedCount; ++index) {
        const GameDisplayEntry &entry = game->staged[index];
        node = entry.node;
        matrix_stack::GetPreviousMatrix(0);
        Matrix4x4::preTranslate(entry.position.x, entry.position.y, entry.position.z);
        Matrix4x4::Scale(entry.scale.x, entry.scale.y, entry.scale.z);
        ApplyGameDisplayTransform(node);
        if (entry.alpha == 1.0f)
            dc::sdk::DrawSingleLightMT7Mesh(node->mesh);
        else if (entry.alpha != 0.0f)
            dc::sdk::DrawSingleLightMT7MeshAlpha(node->mesh, entry.alpha);
    }
    PopCurrentMatrix(1);
    PopCurrentMatrix(1);
}

#else

// @verified - p
void GAME_Callback(void *payload) {
    Character *const character = static_cast<Character *>(payload);
    if (!IsWorldDrawingEnabled() || !character)
        return;
    const GameDisplayWork *const game = static_cast<const GameDisplayWork *>(FindModelComponent(character, MODEL_COMPONENT_GAME));
    if (!game)
        return;
    const MT7Model &model = *static_cast<const MT7Model *>(character->model);
    Matrix4x4_Push();
    ApplyCharacterEntityTransform(character);
    ApplyGameDisplayTransform(model.nodes.front());
    Matrix4x4_Push();
    for (std::int32_t index = 0; index < game->stagedCount; ++index) {
        const GameDisplayEntry &entry = game->staged[index];
        workTransformMatrix = *(g_matrixStack.top.stack - 1);
        Matrix4x4::preTranslate(entry.position.x, entry.position.y, entry.position.z);
        Matrix4x4::Scale(entry.scale.x, entry.scale.y, entry.scale.z);
        ApplyGameDisplayTransform(*entry.node);
        DrawGameDisplayEntry(model, entry);
    }
    Matrix4x4::Pop(1);
    Matrix4x4::Pop(1);
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

// @verified - p
std::uint64_t CopyNodeIntoGame(const void *model, GameDisplayWork *game) {
    static_cast<void>(model);
    const std::size_t bytes = sizeof(GameDisplayEntry) * static_cast<std::size_t>(static_cast<std::int64_t>(game->stagedCount));
    std::memcpy(game->copied, game->staged, bytes);
    const std::uint32_t result = static_cast<std::uint32_t>(game->stagedCount);
    game->copiedCount = game->stagedCount;
    game->cachedWord34 = 0;
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void RunGameCallback(Character *character, void *payload) {
    if (reinterpret_cast<const GameDisplayWork *>(payload)->stagedCount)
        HLib::ExecuteCallbackNow(GAME_Callback, character);
}

#else

// @verified - p
void RunGameCallback(Character *character, void *payload) {
    if (static_cast<GameDisplayWork *>(payload)->stagedCount != 0)
        HLib::ExecuteCallbackNow(GAME_Callback, character);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void AttachGameDisplayComponent(Character *character) {
    if (charFindComp(character, MODEL_COMPONENT_GAME))
        return;
    GameDisplayWork *const game =
        static_cast<GameDisplayWork *>(AllocateModelComponent(character, MODEL_COMPONENT_GAME, sizeof(GameDisplayWork), RunGameCallback));
    std::memset(game, 0, sizeof(*game));
    game->baseNodeId = 0x100;
    game->digitStep.x = -0.02f;
    game->decimalShift = 0;
    game->maximumValue = 99999999;
    game->retainedState = 0;
    MT7Node *const root = character->model;
    MT7Node *previous = 0;
    MT7Node *node = root ? root->child : 0;
    while (node && node->id != 255) {
        previous = node;
        node = node->sibling;
    }
    game->inserted = node;
    if (node) {
        if (previous)
            previous->sibling = node->sibling;
        else
            root->child = node->sibling;
        node->sibling = 0;
    }
}

#else

// @verified - p
bool AttachGameDisplayComponent(Character *character) {
    if (FindModelComponent(character, MODEL_COMPONENT_GAME))
        return true;
    GameDisplayWork *const game =
        static_cast<GameDisplayWork *>(AllocateModelComponent(character, MODEL_COMPONENT_GAME, sizeof(GameDisplayWork), RunGameCallback));
    if (!game)
        return false;
    std::memset(game, 0, sizeof(*game));
    game->baseNodeId = 256;
    game->digitStep = {.x = 0.0f, .y = -0.02f, .z = 0.0f};
    game->maximumValue = 99999999;
    MT7Model *const model = static_cast<MT7Model *>(character->model);
    if (!model)
        return true;
    MT7Node &root = model->nodes.front();
    MT7Node *previous = nullptr;
    MT7Node *node = root.runtimeChild;
    while (node && static_cast<std::uint16_t>(node->id) != 255) {
        previous = node;
        node = node->runtimeSibling;
    }
    game->inserted = node;
    if (!node)
        return true;
    game->previous = previous;
    if (previous) {
        previous->runtimeSibling = node->runtimeSibling;
        previous->nextSibling = node->nextSibling;
    } else {
        root.runtimeChild = node->runtimeSibling;
        root.firstChild = node->nextSibling;
    }
    node->runtimeSibling = nullptr;
    node->nextSibling = -1;
    return true;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void StageGameDisplayNode(GameDisplayWork *game, std::int32_t nodeId, const Vector3 *position, float alpha, const Vector3 *scale) {
    if (!game || !position || game->stagedCount >= 64)
        return;
    MT7Node *const node = FindMT7Node(game->inserted, nodeId);
    if (!node || !node->mesh)
        return;
    GameDisplayEntry &entry = game->staged[game->stagedCount];
    entry.node = node;
    std::memcpy(&entry.position.x, &position->x, sizeof(float));
    std::memcpy(&entry.position.y, &position->y, sizeof(float));
    std::memcpy(&entry.position.z, &position->z, sizeof(float));
    entry.alpha = alpha;
    if (scale) {
        std::memcpy(&entry.scale.x, &scale->x, sizeof(float));
        std::memcpy(&entry.scale.y, &scale->y, sizeof(float));
        std::memcpy(&entry.scale.z, &scale->z, sizeof(float));
    } else {
        entry.scale.x = 1.0f;
        entry.scale.y = 1.0f;
        entry.scale.z = 1.0f;
    }
    ++game->stagedCount;
}

#else

// @verified - p
void StageGameDisplayNode(GameDisplayWork *game, std::uint16_t nodeId, const Vector3 *position, float alpha, const Vector3 *scale) {
    if (!game || !position || game->stagedCount >= 64)
        return;
    MT7Node *const node = FindGameDisplayNode(game->inserted, nodeId);
    if (node && node->xb01Offset)
        AppendGameDisplayEntry(game, node, position, alpha, scale);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
GameDisplayWork *ResetGameDisplayEntries(Character *character) {
    GameDisplayWork *const game = static_cast<GameDisplayWork *>(charFindComp(character, MODEL_COMPONENT_GAME));
    if (game)
        game->stagedCount = 0;
    return game;
}

void StageCharacterGameDisplayNode(Character *character, std::int32_t nodeId, const Vector3 *position, float alpha, const Vector3 *scale) {
    GameDisplayWork *const game = static_cast<GameDisplayWork *>(charFindComp(character, MODEL_COMPONENT_GAME));
    if (game)
        StageGameDisplayNode(game, nodeId, position, alpha, scale);
}

// @verified - d
GameDisplayWork *scenCopyCharaToGame(shen_name name) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return ResetGameDisplayEntries(scenFindChara(control->scenarioTask, name));
}
#endif

#else

// @verified - p
std::uint64_t scenCopyCharaToGame(shen_name id) {
    Character *const character = FindGameDisplayCharacter(id);
    GameDisplayWork *const game = static_cast<GameDisplayWork *>(FindModelComponent(character, MODEL_COMPONENT_GAME));
    if (!game)
        return 0;
    const std::uint64_t result = CopyNodeIntoGame(character->model, game);
    game->stagedCount = 0;
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void scenGetCharaGameNode(shen_name name, std::int32_t nodeId, const Vector3 *position, float alpha, const Vector3 *scale) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    StageGameDisplayNode(static_cast<GameDisplayWork *>(charFindComp(character, MODEL_COMPONENT_GAME)), nodeId, position, alpha, scale);
}
#endif

#else

// @verified - p
void scenGetCharaGameNode(shen_name id, std::uint16_t nodeId, const Vector3 *position, float alpha, const Vector3 *scale) {
    Character *const character = FindGameDisplayCharacter(id);
    StageGameDisplayNode(static_cast<GameDisplayWork *>(FindModelComponent(character, MODEL_COMPONENT_GAME)), nodeId, position, alpha, scale);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
GameDisplayWork *SetPosRotForGame(shen_name name, std::uint32_t baseNodeId, const Vector3 *digitStep, std::uint32_t maximumValue, std::int32_t decimalShift) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    GameDisplayWork *const game = static_cast<GameDisplayWork *>(charFindComp(character, MODEL_COMPONENT_GAME));
    game->baseNodeId = baseNodeId;
    game->digitStep.x = digitStep->x;
    game->digitStep.y = digitStep->y;
    game->digitStep.z = digitStep->z;
    game->maximumValue = maximumValue;
    game->decimalShift = decimalShift;
    return game;
}
#endif

#else

// @verified - p
GameDisplayWork *SetPosRotForGame(shen_name id, std::uint32_t baseNodeId, const Vector3 *digitStep, std::int32_t maximumValue, std::int32_t decimalShift) {
    Character *const character = FindGameDisplayCharacter(id);
    GameDisplayWork *const game = static_cast<GameDisplayWork *>(FindModelComponent(character, MODEL_COMPONENT_GAME));
    game->baseNodeId = baseNodeId;
    game->digitStep.x = digitStep->x;
    game->digitStep.y = digitStep->y;
    game->digitStep.z = digitStep->z;
    game->maximumValue = static_cast<std::uint32_t>(maximumValue);
    game->decimalShift = decimalShift;
    return game;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void gameSetObjectFlags(GameDisplayWork *game, std::uint32_t value, const Vector3 *position, float alpha, const Vector3 *scale) {
    std::int32_t place;
    for (place = 0; place < game->decimalShift; ++place)
        value *= 10U;
    if (value > game->maximumValue)
        value = game->maximumValue;
    bool significant = false;
    std::uint32_t remaining = value;
    for (place = 7; place >= 0; --place) {
        std::uint32_t divisor = 1;
        Vector3 digitPosition;
        digitPosition.x = position->x;
        digitPosition.y = position->y;
        digitPosition.z = position->z;
        for (std::int32_t step = 0; step < place; ++step) {
            divisor *= 10U;
            digitPosition.x += game->digitStep.x;
            digitPosition.y += game->digitStep.y;
            digitPosition.z += game->digitStep.z;
        }
        std::uint32_t digit = remaining / divisor;
        remaining -= digit * divisor;
        if (digit)
            significant = true;
        digit += game->baseNodeId;
        if (significant || place == 0)
            StageGameDisplayNode(game, digit, &digitPosition, alpha, scale);
    }
}
#endif

#else

// @verified - p
std::uint64_t gameSetObjectFlags(GameDisplayWork *game, std::uint32_t value, const Vector3 *position, float alpha, const Vector3 *scale) {
    for (std::int32_t shift = game->decimalShift; shift > 0; --shift)
        value *= 10U;
    if (value > game->maximumValue)
        value = game->maximumValue;
    bool significant = false;
    std::uint64_t result = 0;
    for (std::int32_t place = 7; place >= 0; --place) {
        Vector3 digitPosition = *position;
        std::uint32_t divisor = 1;
        for (std::int32_t addition = 0; addition < place; ++addition) {
            divisor *= 10U;
            digitPosition.x += game->digitStep.x;
            digitPosition.y += game->digitStep.y;
            digitPosition.z += game->digitStep.z;
        }
        const std::uint32_t digit = value / divisor;
        value %= divisor;
        if (digit != 0)
            significant = true;
        result = game->baseNodeId + digit;
        if ((!significant && place != 0) || game->stagedCount >= 64)
            continue;
        MT7Node *const node = FindGameDisplayNode(game->inserted, static_cast<std::uint16_t>(result));
        result = 0;
        if (node && node->xb01Offset)
            result = AppendGameDisplayEntry(game, node, &digitPosition, alpha, scale);
    }
    return result;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void UpdateGameNodeFlags(shen_name name, std::uint32_t value, const Vector3 *position, float alpha, const Vector3 *scale) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    gameSetObjectFlags(static_cast<GameDisplayWork *>(charFindComp(character, MODEL_COMPONENT_GAME)), value, position, alpha, scale);
}
#endif

#else

// @verified - p
std::uint64_t UpdateGameNodeFlags(shen_name id, std::uint32_t value, const Vector3 *position, float alpha, const Vector3 *scale) {
    Character *const character = FindGameDisplayCharacter(id);
    return gameSetObjectFlags(static_cast<GameDisplayWork *>(FindModelComponent(character, MODEL_COMPONENT_GAME)), value, position, alpha, scale);
}

#endif

} // namespace s2
} // namespace shenmue

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern "C" {
__attribute__((renesas)) shenmue::s2::GameDisplayWork *shenmue_s2_event_scenCopyCharaToGame(std::uint32_t name) {
    return shenmue::s2::scenCopyCharaToGame(shenmue::shen_name(name));
}
__attribute__((renesas)) void shenmue_s2_event_scenGetCharaGameNode(
    std::uint32_t name, std::int32_t nodeId, const shenmue::Vector3 *position, float alpha, const shenmue::Vector3 *scale) {
    shenmue::s2::scenGetCharaGameNode(shenmue::shen_name(name), nodeId, position, alpha, scale);
}
__attribute__((renesas)) shenmue::s2::GameDisplayWork *shenmue_s2_event_SetPosRotForGame(
    std::uint32_t name, std::uint32_t baseNodeId, const shenmue::Vector3 *digitStep, std::uint32_t maximumValue, std::int32_t decimalShift) {
    return shenmue::s2::SetPosRotForGame(shenmue::shen_name(name), baseNodeId, digitStep, maximumValue, decimalShift);
}
__attribute__((renesas)) void shenmue_s2_event_UpdateGameNodeFlags(
    std::uint32_t name, std::uint32_t value, const shenmue::Vector3 *position, float alpha, const shenmue::Vector3 *scale) {
    shenmue::s2::UpdateGameNodeFlags(shenmue::shen_name(name), value, position, alpha, scale);
}
}
#endif
#else
#endif
