#include "s2/areas/area_entities.hpp"
#include "s2/area_event.hpp"
#include "s2/character.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/map_control.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"

#include <cstring>

namespace shenmue::s2 {

void PlaceAreaEntity(std::int32_t placementIndex, std::int32_t characterIndex) {
    const std::int16_t flagIndex = static_cast<std::int16_t>((placementIndex < 16 ? placementIndex : placementIndex - 16) + 16);
    const AreaEntityPlacement &placement = AREA_ENTITY_PLACEMENTS[placementIndex];
    const shen_name character{placement.character.name + (static_cast<std::uint32_t>(characterIndex) << 24)};
    const std::int32_t flags = static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, flagIndex));
    const std::int32_t selector = (flags >> (2 * characterIndex)) & 3;
    const std::byte *const offset = reinterpret_cast<const std::byte *>(&AREA_ENTITY_POSITION_IMAGE) + selector * sizeof(Vector3);
    float x;
    float z;
    std::memcpy(&x, offset, sizeof(x));
    std::memcpy(&z, offset + offsetof(Vector3, z), sizeof(z));
    const Vector3 position{.x = characterIndex * 0.1f + x, .y = 0.75f, .z = z};
    const Vector3i rotation{.x = 0, .y = 40960, .z = 0};
    AT_SetLocalBaseChara(character, AREA_ENTITY_BASE);
    EV_AT_SetPos(character, EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(character, EventTransformFlags::AllAxes, rotation, nullptr);
    AT_SetLocalBaseChara(character, EVENT_CHARACTER_INVALID);
}

void PlaceAreaEntityGroup(std::int32_t placementIndex) {
    const AreaEntityPlacement &placement = AREA_ENTITY_PLACEMENTS[placementIndex];
    const Vector3i rotation{.x = 0, .y = placement.yaw, .z = 0};
    EV_AT_SetPos(AREA_ENTITY_BASE, EventTransformFlags::AllAxes, placement.position);
    EV_AT_SetAngY(AREA_ENTITY_BASE, EventTransformFlags::AllAxes, rotation, nullptr);
    for (std::int32_t characterIndex = 0; characterIndex < 4; ++characterIndex)
        PlaceAreaEntity(placementIndex, characterIndex);
}

void areaClearAreaEntities() {
    if (g_actNumber == 1)
        return;
    bool enabled = EV_GetShenmueFlag(0x31) != 0;
    if (!enabled && EV_GetShenmueFlag(0x1e3)) {
        std::int64_t flags = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 823);
        flags &= EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 822);
        flags &= EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 821);
        enabled = flags == 0;
    }
    if (!enabled && !EV_GetShenmueFlag(0x10e))
        return;
    char packageName[12]{};
    std::memcpy(packageName, &AREA_ENTITY_PACKAGE.name, sizeof(AREA_ENTITY_PACKAGE.name));
    std::memcpy(packageName + 4, &g_currentMap.name, sizeof(g_currentMap.name));
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    const char *const directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_ControlPackageResources(
        PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>(packageName));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, AREA_ENTITY_BASE);
    for (std::int32_t index = 0; index < 22; ++index) {
        if (AREA_ENTITY_PLACEMENTS[index].area.name == g_currentMap.name)
            PlaceAreaEntityGroup(index);
    }
    SetCharaKill(AREA_ENTITY_BASE, 1);
    EV_NPC_NewLoadSw(1);
}

} // namespace shenmue::s2
