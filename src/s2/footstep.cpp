#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/footstep_dc.hpp"

#include "s2/string_utility.hpp"
#include "s2/field_event_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/character_table_dc.hpp"
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
std::uint32_t ResolveFootstepMaterialKey(const Character *character, std::uint16_t stepId, std::uint32_t material);
std::int32_t ResolveFootstepGroundToken(Character *character);
void AttachFootstepComponent(Character *character, const char *surface, std::int8_t authoredMode);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

FootstepComponent *FindFootstepComponent(const Character *character) {
    return static_cast<FootstepComponent *>(FindModelComponent(character, kFootstepToken));
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace {

static const std::uint16_t footstepMaterialKeys[16][4] = {{1021, 1022, 1023, 1024}, {1025, 1026, 1027, 1028}, {1029, 1030, 1031, 1032},
    {1033, 1034, 1035, 1036}, {1037, 1038, 1039, 1040}, {1041, 1042, 1043, 1044}, {1045, 1046, 1047, 1048}, {1049, 1050, 1051, 1052}, {1053, 1054, 1055, 1056},
    {1057, 1058, 1059, 1060}, {1061, 1062, 1063, 1064}, {871, 1065, 1066, 1067}, {1069, 1070, 1071, 1072}, {2112, 2113, 2114, 2112}, {2115, 2116, 2117, 2115},
    {1530, 1531, 1532, 1530}};

static const std::uint16_t *const footstepMaterialRows[40] = {footstepMaterialKeys[11], footstepMaterialKeys[15], footstepMaterialKeys[11],
    footstepMaterialKeys[15], footstepMaterialKeys[0], footstepMaterialKeys[15], footstepMaterialKeys[1], footstepMaterialKeys[15], footstepMaterialKeys[11],
    footstepMaterialKeys[15], footstepMaterialKeys[11], footstepMaterialKeys[15], footstepMaterialKeys[11], footstepMaterialKeys[15], footstepMaterialKeys[2],
    footstepMaterialKeys[15], footstepMaterialKeys[3], footstepMaterialKeys[15], footstepMaterialKeys[4], footstepMaterialKeys[15], footstepMaterialKeys[5],
    footstepMaterialKeys[15], footstepMaterialKeys[6], footstepMaterialKeys[15], footstepMaterialKeys[7], footstepMaterialKeys[15], footstepMaterialKeys[8],
    footstepMaterialKeys[15], footstepMaterialKeys[9], footstepMaterialKeys[15], footstepMaterialKeys[14], footstepMaterialKeys[15], footstepMaterialKeys[10],
    footstepMaterialKeys[15], footstepMaterialKeys[11], footstepMaterialKeys[15], footstepMaterialKeys[12], footstepMaterialKeys[15], footstepMaterialKeys[13],
    footstepMaterialKeys[15]};

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::int32_t ResolveFootstepGroundToken(Character *character) {
    const FootstepComponent *const footstep = static_cast<FootstepComponent *>(charFindComp(character, CHARACTER_FOOTSTEP));
    if (!footstep || !footstep->enabled)
        return -1;
    Vector3 position;
    mtwkGetBonePosition(charFindMotionwork(character), static_cast<MotionBoneId>(0), &position);
    std::uint32_t token = GetFieldSoundProperties(&position);
    if (!(token & 0xffffU))
        token = (token & 0xffff0000U) | static_cast<std::uint32_t>(static_cast<std::int32_t>(footstep->authoredMode));
    return static_cast<std::int32_t>(token);
}

#else

std::int32_t ResolveFootstepGroundToken(Character *character) {
    const FootstepComponent *const footstep = FindFootstepComponent(character);
    if (!footstep || !footstep->enabled)
        return -1;
    Vector3 position{};
    mtwkGetBonePosition(FindMotionWork(character), MB_Pelvis, &position);
    const std::uint32_t result = QueryGroundMaterialToken(position, position.y);
    if ((result & 0xffffU) != 0)
        return static_cast<std::int32_t>(result);
    return static_cast<std::int32_t>((result & 0xffff0000U) | static_cast<std::uint32_t>(static_cast<std::int32_t>(footstep->authoredMode)));
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
std::uint32_t ResolveFootstepMaterialKey(Character *character, std::uint32_t stepToken, std::uint32_t flags) {
    const FootstepComponent *const footstep = static_cast<FootstepComponent *>(charFindComp(character, CHARACTER_FOOTSTEP));
    const std::uint16_t *const row = footstepMaterialRows[(stepToken & 0xffffU) * 2U + footstep->surface];
    std::uint32_t key;
    if ((flags & 0x7fffffU) - 6U < 2U)
        key = row[3];
    else
        key = row[static_cast<std::int32_t>(dc::sdk::RandomInt()) % 3];
    return key;
}

std::uint32_t GetSoundEnvironmentFromProperties(std::uint32_t properties) {
    return properties >> 16;
}

void SetFootstepComponentByte(Character *character, std::uint8_t which, std::uint8_t value) {
    FootstepComponent *const footstep = static_cast<FootstepComponent *>(charFindComp(character, CHARACTER_FOOTSTEP));
    if (!footstep)
        return;
    switch (which) {
    case 0:
        footstep->enabled = value;
        break;
    case 1:
        footstep->authoredMode = static_cast<std::int8_t>(value);
        break;
    }
}

#else

std::uint32_t ResolveFootstepMaterialKey(const Character *character, std::uint16_t stepId, std::uint32_t material) {
    const FootstepComponent *const footstep = FindFootstepComponent(character);
    const std::size_t rowIndex = !footstep || footstep->surface == 0 || stepId == 19 ? stepId : static_cast<std::size_t>(footstep->surface) + 20U;
    if (rowIndex >= kFootstepMaterialKeys.size())
        return material & 0x1fffffU;
    const std::array<std::uint16_t, 4> &row = kFootstepMaterialKeys[rowIndex];
    const std::uint32_t key = material & 0x1fffffU;
    if (key - 6U <= 1U)
        return row[3];
    if (key >= 9U)
        return key;
    return row[IncrementSharedPrngUInt16() % 3U];
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

// @verified - d
void AttachFootstepComponent(Character *character, const char *surface, std::int32_t collisionFallback) {
    FootstepComponent *const component = static_cast<FootstepComponent *>(AllocateModelComponent(character, CHARACTER_FOOTSTEP, sizeof(FootstepComponent), 0));
    if (!CompareCharacterTableTokens(surface, "SOFT"))
        component->surface = FootstepSoft;
    else if (!CompareCharacterTableTokens(surface, "NORMAL"))
        component->surface = FootstepNormal;
    else if (!CompareCharacterTableTokens(surface, "HARD"))
        component->surface = FootstepHard;
    component->authoredMode = static_cast<std::int8_t>(collisionFallback);
    component->enabled = 1;
}

#else

void AttachFootstepComponent(Character *character, const char *surface, std::int8_t authoredMode) {
    FootstepComponent *const footstep = static_cast<FootstepComponent *>(AllocateModelComponent(character, kFootstepToken, sizeof(FootstepComponent), nullptr));
    if (CompareTokenIgnoreCase(surface, "SOFT") == 0)
        footstep->surface = 0;
    else if (CompareTokenIgnoreCase(surface, "HARD") == 0)
        footstep->surface = 1;
    else if (CompareTokenIgnoreCase(surface, "SIN_") == 0)
        footstep->surface = 2;
    else if (CompareTokenIgnoreCase(surface, "YUA_") == 0)
        footstep->surface = 3;
    else if (CompareTokenIgnoreCase(surface, "JOP_") == 0)
        footstep->surface = 4;
    footstep->authoredMode = authoredMode;
    footstep->enabled = 1;
}

#endif

} // namespace s2
} // namespace shenmue
