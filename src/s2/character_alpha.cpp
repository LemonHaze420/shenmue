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

namespace shenmue::s2 {
using namespace model_component_detail;

namespace model_component_detail {

// CPAM_Handler.
// @verified - p
void UpdateCharacterAlpha(Character *character, void *payload) {
    CharacterAlphaState &state = *static_cast<CharacterAlphaState *>(payload);
    if (state.frameCount)
        ++state.frameCount;
    const float alpha = character->alpha;
    Vector3 origin;
    MotionWork *const motion = FindMotionWork(character);
    bool fallback = motion == nullptr;
    if (motion) {
        const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        const shen_name name = scenGetCharaName(control->scenarioTask, character);
        std::memcpy(cpamCharacterName.data(), &name.name, 4);
        cpamCharacterName[4] = 0;
        if (IsScenarioCharacterForcedVisible(name) || name.name == GetScenarioFollowCharacter().name || name.name == SHEN_NAME(SIN_)) {
            character->alpha = 1.0f;
            state = {};
            return;
        }
        bool empty = true;
        if (const Matrix4x4 *const hip = charaGetBoneTransform(motion, 1)) {
            for (const Vector4 &row : hip->rows) {
                if (!(0.00000011920929f > std::abs(row.x)) || !(0.00000011920929f > std::abs(row.y)) || !(0.00000011920929f > std::abs(row.z)) ||
                    !(0.00000011920929f > std::abs(row.w))) {
                    empty = false;
                    break;
                }
            }
        }
        fallback = !GetMotionBonePosition(motion, 1, &origin) || empty;
    }
    if (fallback)
        origin = GetCharacterWorldPosition(character);
    Matrix4x4 transform{};
    Matrix4x4::GetTransformMatrix(&transform);
    const Vector3 position{origin.x * transform.rows[0].x + origin.y * transform.rows[1].x + origin.z * transform.rows[2].x + transform.rows[3].x,
        origin.x * transform.rows[0].y + origin.y * transform.rows[1].y + origin.z * transform.rows[2].y + transform.rows[3].y,
        origin.x * transform.rows[0].z + origin.y * transform.rows[1].z + origin.z * transform.rows[2].z + transform.rows[3].z};
    float nextAlpha = alpha - 0.050000001f;
    if (forceCharacterAlphaVisible) {
        nextAlpha = 1.0f;
    } else if (state.alphaSteps >= -5) {
        float radius = camCtrlGetNpcRadius() + 0.5f;
        if (state.alphaSteps < 0)
            radius -= 1.0f;
        if (state.alphaSteps <= 0)
            state.alphaSteps = static_cast<std::int32_t>(!(std::abs(position.z) > radius)) - 1;
        if (state.alphaSteps >= 0 && state.frameCount <= 9U) {
            if (!(0.75f > position.z) || !(position.z > -0.75f) || !(position.x > -0.75f) || !(0.75f > position.x)) {
                state.alphaSteps = 0;
                nextAlpha = alpha + 0.050000001f;
            } else if (state.alphaSteps <= 8) {
                nextAlpha = (-position.z - 0.25f) * 2.0f;
                state.alphaSteps = 0.9 > static_cast<double>(nextAlpha) ? state.alphaSteps + 1 : 0;
                if (static_cast<double>(nextAlpha) > static_cast<double>(alpha) + 0.05)
                    nextAlpha = alpha + 0.050000001f;
            }
        }
    }
    const float upper = 1.0f < nextAlpha ? 1.0f : nextAlpha;
    character->alpha = 0.0f > upper ? 0.0f : upper;
}

} // namespace model_component_detail

bool AttachCharacterAlphaComponent(Character *character) {
    if (!character)
        return false;
    constexpr shen_name token{SHEN_NAME(CPAM)};
    if (FindModelComponent(character, token))
        return true;
    CharacterAlphaState *const state =
        static_cast<CharacterAlphaState *>(AllocateModelComponent(character, token, sizeof(CharacterAlphaState), UpdateCharacterAlpha));
    if (!state)
        return false;
    modelSetCharaAlpha(character, 0.0f);
    state->alphaSteps = -1;
    state->frameCount = 0;
    return true;
}

void npcSetAlphaOnOff(Character *character, bool enabled) {
    if (!character)
        return;
    constexpr shen_name token{SHEN_NAME(CPAM)};
    ModelComponentHeader *const component = FindHeader(character, token);
    if (component) {
        auto *const state = reinterpret_cast<CharacterAlphaState *>(Payload(component));
        state->alphaSteps = enabled ? 0 : -10;
        if ((component->flags & 1U) != 0)
            return;
    }
    modelSetCharaAlpha(character, enabled ? 1.0f : 0.0f);
}

std::int64_t cpamSetFlag(Character *character, std::int32_t enabled) {
    if (!character)
        return 0;
    constexpr shen_name token{SHEN_NAME(CPAM)};
    ModelComponentHeader *const component = FindHeader(character, token);
    if (!component)
        return 0;
    const std::int64_t previous = (component->flags & 1U) != 0;
    if (enabled)
        component->flags |= 1U;
    else
        component->flags &= ~1U;
    if (!enabled) {
        modelSetCharaAlpha(character, 1.0f);
        auto *const state = reinterpret_cast<CharacterAlphaState *>(Payload(component));
        state->alphaSteps = 0;
        state->frameCount = 0;
    }
    return previous;
}

// @verified-s2: 0x7ff6185be7b0. This queries the CPAM enable flag, not payload existence.
bool shenCheckCharaHasAlphaManager(const Character *character) {
    const ModelComponentHeader *const component = FindHeader(character, SHEN_NAME(CPAM));
    return component && (component->flags & 1U) != 0;
}

} // namespace shenmue::s2
