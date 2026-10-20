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

FootstepComponent *FindFootstepComponent(const Character *character) {
    return static_cast<FootstepComponent *>(FindModelComponent(character, kFootstepToken));
}

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

std::int32_t ResolveFootstepGroundToken(Character *character) {
    const FootstepComponent *const footstep = FindFootstepComponent(character);
    if (!footstep || !footstep->enabled)
        return -1;
    Vector3 position{};
    GetMotionBonePosition(FindMotionWork(character), MB_Pelvis, &position);
    const std::uint32_t result = QueryGroundMaterialToken(position, position.y);
    if ((result & 0xffffU) != 0)
        return static_cast<std::int32_t>(result);
    return static_cast<std::int32_t>((result & 0xffff0000U) | static_cast<std::uint32_t>(static_cast<std::int32_t>(footstep->authoredMode)));
}

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

} // namespace shenmue::s2
