#if defined(SHENMUE_EVENT_OVERLAY)

#include "s2/areas/area_entities.hpp"

void areaClearAreaEntities(void)
{
    shen_name package[3];
    int i;
    if (g_actNumber == 1)
        return;
    if (FTBL.EV_GetShenmueFlag(49)
        || (FTBL.EV_GetShenmueFlag(483) && (FTBL.EV_GetPlayerFlag(11, 821) & FTBL.EV_GetPlayerFlag(11, 822) & FTBL.EV_GetPlayerFlag(11, 823)) == 0)
        || FTBL.EV_GetShenmueFlag(270)) {
        package[0] = AREA_ENTITY_PACKAGE;
        package[1] = g_currentMap;
        package[2] = 0;
        FTBL.EV_NPC_NewLoadSw_0(0);
        FTBL.EV_ResetFreeTalkFlags();
        FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)package, 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        FTBL.CreateEventCharacter(3, 0, AREA_ENTITY_BASE);
        for (i = 0; i < 22; i++) {
            if (areaEntityPlacements[i].area == g_currentMap)
                PlaceAreaEntityGroup(i);
        }
        FTBL.SetScenarioCharacterKillMode(AREA_ENTITY_BASE, 1);
        FTBL.EV_NPC_NewLoadSw_0(1);
    }
}

void PlaceAreaEntityGroup(int placementIndex)
{
    Vector3i rotation;
    int characterIndex;
    rotation.x = 0;
    rotation.y = areaEntityPlacements[placementIndex].yaw;
    rotation.z = 0;
    FTBL.EV_AT_SetPos(AREA_ENTITY_BASE, 0x38000000, &areaEntityPlacements[placementIndex].position);
    FTBL.EV_AT_SetAngY_0(AREA_ENTITY_BASE, 0x38000000, &rotation, 0);
    for (characterIndex = 0; characterIndex < 4; characterIndex++)
        PlaceAreaEntity(placementIndex, characterIndex);
}

void PlaceAreaEntity(int placementIndex, int characterIndex)
{
    Vector3 position;
    Vector3i rotation;
    shen_name character;
    int selector;
    character = areaEntityPlacements[placementIndex].character + (characterIndex << 24);
    if (placementIndex > 15)
        placementIndex -= 16;
    selector = (FTBL.EV_GetPlayerFlag(15, placementIndex + 16) >> (characterIndex * 2)) & 3;
    position.x = areaEntityOffsets[selector].x + characterIndex * 0.1f;
    position.y = 0.75f;
    position.z = areaEntityOffsets[selector].z;
    rotation.x = rotation.z = 0;
    rotation.y = 40960;
    FTBL.AT_SetLocalBaseChara(character, AREA_ENTITY_BASE);
    FTBL.EV_AT_SetPos(character, 0x38000000, &position);
    FTBL.EV_AT_SetAngY_0(character, 0x38000000, &rotation, 0);
    FTBL.AT_SetLocalBaseChara(character, 0xffffffffU);
}

const Vector3 areaEntityOffsets[3] = {{-0.15f, 0.75f, 0.3f}, {-0.15f, 0.75f, 0.4f}, {-0.15f, 0.75f, 0.5f}};
AreaEntityPlacement areaEntityPlacements[22] = {
    {SHEN_NAME4('W', 'S', '0', '0'), {61.6f, 100.1f, 181.55f}, 49152, 0, 0, SHEN_NAME4('c', 'w', '0', '0')},
    {SHEN_NAME4('W', 'S', '0', '0'), {35.95f, 100.1f, 176.6f}, 24576, 0, 0, SHEN_NAME4('c', 'w', '1', '0')},
    {SHEN_NAME4('W', 'N', '0', '0'), {218.5f, 104.5f, 212.56f}, 57344, 1, 0, SHEN_NAME4('c', 'w', '2', '0')},
    {SHEN_NAME4('W', 'N', '0', '0'), {212.5f, 104.5f, 197.65f}, 40960, 0, 0, SHEN_NAME4('c', 'w', '3', '0')},
    {SHEN_NAME4('W', 'N', '0', '0'), {264.9f, 104.5f, 204.5f}, 49152, 0, 0, SHEN_NAME4('c', 'w', '4', '0')},
    {SHEN_NAME4('W', 'N', '0', '0'), {190.35f, 104.6f, 224.0f}, 32768, 0, 2, SHEN_NAME4('c', 'w', '5', '0')},
    {SHEN_NAME4('W', 'N', '0', '0'), {204.3f, 104.6f, 207.8f}, 24576, 0, 2, SHEN_NAME4('c', 'w', '6', '0')},
    {SHEN_NAME4('W', 'N', '0', '0'), {235.55f, 104.5f, 202.2f}, 32768, 0, 0, SHEN_NAME4('c', 'w', '7', '0')},
    {SHEN_NAME4('W', 'N', '0', '0'), {220.32f, 104.6f, 206.96f}, 40960, 0, 0, SHEN_NAME4('c', 'w', '8', '0')},
    {SHEN_NAME4('W', 'T', '0', '0'), {264.35f, 115.3f, 330.3f}, 16384, 0, 0, SHEN_NAME4('c', 'w', '9', '0')},
    {SHEN_NAME4('W', 'R', '0', '0'), {178.6f, 102.25f, 192.3f}, 60074, 0, 1, SHEN_NAME4('c', 'w', 'A', '0')},
    {SHEN_NAME4('A', 'R', '0', '2'), {257.42f, 101.0f, 398.32f}, 64807, 0, 0, SHEN_NAME4('c', 'w', 'B', '0')},
    {SHEN_NAME4('A', 'R', '0', '3'), {303.42f, 100.15f, 478.38f}, 0, 0, 0, SHEN_NAME4('c', 'w', 'C', '0')},
    {SHEN_NAME4('A', 'R', '0', '2'), {244.8f, 100.0f, 425.1f}, 32768, 0, 1, SHEN_NAME4('c', 'w', 'D', '0')},
    {SHEN_NAME4('A', 'K', 'S', '0'), {268.7f, 100.0f, 260.7f}, 32768, 0, 0, SHEN_NAME4('c', 'w', 'E', '0')},
    {SHEN_NAME4('A', 'K', 'S', '1'), {269.2f, 106.075f, 262.75f}, 32768, 0, 0, SHEN_NAME4('c', 'w', 'F', '0')},
    {SHEN_NAME4('Q', '3', '0', '0'), {264.9f, 100.09f, 195.8f}, 55887, 0, 0, SHEN_NAME4('c', 'w', 'G', '0')},
    {SHEN_NAME4('Q', '3', '0', '0'), {249.97f, 100.09f, 197.15f}, 0, 0, 0, SHEN_NAME4('c', 'w', 'H', '0')},
    {SHEN_NAME4('Q', '1', '0', '0'), {188.75f, 100.0f, 80.4f}, 15109, 0, 1, SHEN_NAME4('c', 'w', 'I', '0')},
    {SHEN_NAME4('Q', '3', '0', '0'), {262.2f, 100.0f, 161.75f}, 58254, 0, 0, SHEN_NAME4('c', 'w', 'J', '0')},
    {SHEN_NAME4('Q', '1', '0', '0'), {176.2f, 100.0f, 191.43f}, 57963, 0, 0, SHEN_NAME4('c', 'w', 'K', '0')},
    {SHEN_NAME4('W', 'K', 'A', '0'), {57.7f, 103.373f, 312.0f}, 24576, 0, 0, SHEN_NAME4('0', '0', '0', '0')},
};
#else
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
    EV_pakUtils(
        PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>(packageName));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, AREA_ENTITY_BASE);
    for (std::int32_t index = 0; index < 22; ++index) {
        if (AREA_ENTITY_PLACEMENTS[index].area.name == g_currentMap.name)
            PlaceAreaEntityGroup(index);
    }
    SetCharaKill(AREA_ENTITY_BASE, 1);
    EV_NPC_NewLoadSw(1);
}

} // namespace shenmue::s2
#endif
