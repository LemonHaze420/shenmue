#include "s2/qron_dc.hpp"
#include "s2/event_matrix_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/qron_map_data_dc.hpp"
#include "s2/area_jump_dc.hpp"
#include "s2/field_query_dc.hpp"
#include "s2/map_event_transition_dc.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/scene_state_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/air_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/elevator.hpp"
#include "s2/lighting.hpp"
#include "s2/fog_dc.hpp"
#include "s2/map_render_profile.hpp"
#include "s2/map_visibility_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/player_control_state_dc.hpp"
#include "s2/prop_dc.hpp"
#include "s2/weather.hpp"
#include "platform/dc/matrix.hpp"
#endif
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#endif
#include <cfloat>
#include <cstring>
#include "s2/field_collision_dc.hpp"
#include "s2/screen_film.hpp"
#include "s2/player_config_dc.hpp"
#include "s2/scene_assets_dc.hpp"
#include "s2/area_jump_data_dc.hpp"
#include "s2/inventory_dc.hpp"
#include "s2/game_system_dc.hpp"
#include "s2/door_state_dc.hpp"
#include "s2/hlib.hpp"
#include "s2/load_title_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include "s2/player_model_dc.hpp"
#include "s2/player_readiness_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/speak_icon_state_dc.hpp"
#include "s2/ui_state_dc.hpp"
#include "s2/fight_blur_dc.hpp"
#include "s2/map_region_dc.hpp"
#define INV_NOEXCEPT
#define INV_NULL 0
#define INV_INIT =
#define INV_ZERO
#include "platform/dc/sdk.hpp"
#include "s2/camera_dc.hpp"
#include "s2/conversation_dc.hpp"
#include "s2/game_save_area.hpp"
#include "s2/input_dc.hpp"
#include "s2/system_modules_dc.hpp"
#include "s2/texture_dc.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/calendar_dc.hpp"
#include "s2/debug_screen_dc.hpp"
#include "s2/save_screen_state_dc.hpp"
#include "s2/story_state.hpp"
#include <cstdarg>
#include <cstdio>
#endif

namespace shenmue {
namespace s2 {

namespace {

std::int32_t RefreshQronUpperFloorPlacements(QronFloorRuntime *runtime);
std::int32_t EnableQronUpperFloorPlacements(QronFloorRuntime *runtime);
std::int32_t RestoreQronUpperFloorPlacements(QronFloorRuntime *runtime);
bool FindMapEventEntryTransform(shen_name areaId, std::int32_t entryId, Vector3 &position, std::int32_t &yaw);

} // namespace

std::int32_t ResolveQronEntryLocation(const QronMapDefinition &definition, Vector3 &position, std::int32_t &yaw);
void ResetQronSceneChange();
std::int32_t qronRepositionPlayerForFloor(std::int32_t floor);
void QueueQronElevatorSceneChange(shen_name area, std::int32_t entry, std::int32_t floor, const Vector3 *position, std::int32_t yaw);
void QronFloorControl_Callback(void *);
HLib::Task *QueueQronSceneChange();
void QronSceneChange_Callback(void *);

QronFloorRuntime g_qronFloorRuntime;
float FindFieldPartIntersectionHeight(const Vector3 *position);
std::uint32_t g_qronSceneChange[7] = {};

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t QronFloorFromHeight(float height, float baseHeight) {
    const float difference = (height - baseHeight) + 0.1f;
    std::int32_t floor = static_cast<std::int32_t>(difference / 3.6f);
    if (difference > 0.0f)
        ++floor;
    switch (g_currentMap) {
    case AREA_QJBT:
    case AREA_QB00:
    case AREA_QLBT:
        break;
    default:
        if (floor > 1)
            --floor;
        break;
    }
    return floor;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
float QronFloorHeight(std::int32_t floor, float baseHeight) {
    switch (g_currentMap) {
    case AREA_QJBT:
    case AREA_QB00:
    case AREA_QLBT:
        return baseHeight + static_cast<float>(floor - 1) * 3.6f;
    default:
        if (floor > 1)
            return baseHeight + static_cast<float>(floor) * 3.6f;
        return baseHeight + static_cast<float>(floor - 1) * 3.6f;
    }
}
#endif

} // namespace

// @verified - d
const QronMapDefinition *FindQronMapDefinition(shen_name areaId) {
    for (const QronMapDefinition *definition = qronMapDefinitions; definition->areaId; ++definition)
        if (definition->areaId == areaId)
            return definition;
    return 0;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void qron_task_Enqueue() {
    QronFloorRuntime &runtime = g_qronFloorRuntime;
    std::size_t size = sizeof(runtime);
    std::memset(&runtime, 0, size);
    runtime.previousIntersection = -1;
    for (std::int32_t index = 0; index < 5; ++index)
        runtime.savedMapSlots[index] = 0x7fff;
    const QronMapDefinition *const definition = FindQronMapDefinition(g_currentMap);
    if (!definition)
        return;
    runtime.areaId = g_currentMap;
    runtime.baseHeight = definition->baseHeight;
    runtime.maximumFloor = definition->maximumFloor;
    runtime.minimumFloor = definition->minimumFloor;
    runtime.flags = definition->flags;
    runtime.connections = definition->connections;
    runtime.lightIndex = definition->lightIndex;
    Vector3 position;
    std::int32_t yaw;
    runtime.floor = ResolveQronEntryLocation(*definition, position, yaw);
    runtime.temporaryFloorHeight = QronFloorHeight(runtime.floor, runtime.baseHeight);
    MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    SetCharacterWorldPosition(control->ryo, &position);
    control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    SetCharacterWorldYaw(control->ryo, yaw);
    ResetQronSceneChange();
    HLib::EnqueueTaskWithoutParameter(QronFloorControl_Callback, 13, 14, static_cast<std::int32_t>(TASK_QRON));
    if (!(runtime.flags & 2U))
        SetIsSkySetup(0);
    if (!(runtime.flags & 0x10U)) {
        FreeWAIR();
        UpdateWeatherForMinute(1, 0, 0, 0.0f);
        UpdateWeather();
    }
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t ResolveQronEntryLocation(const QronMapDefinition &definition, Vector3 &position, std::int32_t &yaw) {
    const QronSceneChange *change = reinterpret_cast<const QronSceneChange *>(g_qronSceneChange);
    std::int32_t floor;
    if (!GetSavedAreaJumpLocation(position, yaw)) {
        if (change->pending)
            goto pendingChange;
        if (!MoveObjectToPosFromThisScene(SHEN_NAME(0000), g_entryPoint, position, yaw))
            FindMapEventEntryTransform(g_currentMap, g_entryPoint, position, yaw);
    }
    floor = QronFloorFromHeight(position.y, definition.baseHeight);
    goto clampFloor;
pendingChange:
    if (change->mode == 2) {
        floor = change->floor;
        position = change->position;
        yaw = change->yaw;
    } else if (change->mode == 3) {
        floor = change->floor;
        position = change->position;
        yaw = change->yaw;
    } else {
        floor = change->floor;
        if (!MoveObjectToPosFromThisScene(SHEN_NAME(0000), g_entryPoint, position, yaw))
            FindMapEventEntryTransform(g_currentMap, g_entryPoint, position, yaw);
    }
clampFloor:
    floor = definition.maximumFloor > (floor < definition.minimumFloor ? definition.minimumFloor : floor) ?
        (floor < definition.minimumFloor ? definition.minimumFloor : floor) :
        definition.maximumFloor;
    const float floorHeight = QronFloorHeight(floor, definition.baseHeight);
    position.y = floorHeight;
    if (change->mode == 2 && (definition.flags & 0x20U))
        position.y += 3.6f;
    position.y += 3.5f;
    position.y = FindFieldIntersectionHeight(&position);
    if (position.y == -FLT_MAX)
        position.y = floorHeight;
    return floor;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t qronSetFloorDescriptor(const QronFloorDescriptor *descriptor) {
    QronFloorRuntime &runtime = g_qronFloorRuntime;
    if (!runtime.areaId || (runtime.flags & 1U) == 0)
        return 0;
    if (descriptor) {
        runtime.callback = descriptor->callback;
        runtime.floorData = descriptor->floorData;
        runtime.fieldName = descriptor->fieldName;
    }
    qronRepositionPlayerForFloor(runtime.floor);
    return 1;
}
#endif

// @verified - d
void ResetQronSceneChange() {
    g_qronSceneChange[0] = 0;
    g_qronSceneChange[1] = 0;
    g_qronSceneChange[2] = 0;
    g_qronSceneChange[3] = 0;
    g_qronSceneChange[4] = 0;
    g_qronSceneChange[5] = 0;
    g_qronSceneChange[6] = 0;
}

// @verified - d
bool RetainQronMapEventDestination(shen_name areaId, std::int32_t eventId) {
    QronFloorRuntime *runtime = &g_qronFloorRuntime;
    std::uint32_t *change = g_qronSceneChange;
    if (!runtime->areaId)
        return true;
    const QronMapDefinition *const destination = FindQronMapDefinition(areaId);
    if (!destination)
        return true;
    if (runtime->floor > destination->maximumFloor || runtime->floor < destination->minimumFloor)
        return false;
    if (g_currentMap == AREA_QD00 && runtime->floor == 6) {
        if (eventId == 2)
            change[0] = AREA_QC06;
        else if (eventId == 3)
            change[0] = AREA_QE06;
        else
            goto retain;
        change[1] = static_cast<std::uint32_t>(eventId);
        *reinterpret_cast<std::uint16_t *>(reinterpret_cast<std::byte *>(change) + kQronSceneFloorOffset) = static_cast<std::uint16_t>(runtime->floor);
        *reinterpret_cast<std::uint16_t *>(reinterpret_cast<std::byte *>(change) + kQronSceneModeOffset) = 1;
        shenToggleAreaFlags(2);
        QueueQronSceneChange();
        return false;
    }
retain:
    change[0] = areaId;
    *reinterpret_cast<std::uint16_t *>(reinterpret_cast<std::byte *>(change) + kQronSceneFloorOffset) = static_cast<std::uint16_t>(runtime->floor);
    *reinterpret_cast<std::uint16_t *>(reinterpret_cast<std::byte *>(change) + kQronSceneModeOffset) = 1;
    return true;
}

// @verified - d
std::int32_t GetQronNextStream() {
    return g_qronFloorRuntime.areaId ? g_qronFloorRuntime.floor : -1000;
}

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
float GetQronTemporaryFloorHeight() {
    return g_qronFloorRuntime.areaId ? g_qronFloorRuntime.temporaryFloorHeight : FLT_MAX;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t GetQronFloorAtPosition(const Vector3 &position) {
    if (g_qronFloorRuntime.areaId) {
        const std::int32_t floor = QronFloorFromHeight(position.y, g_qronFloorRuntime.baseHeight);
        if (floor <= g_qronFloorRuntime.maximumFloor && floor >= g_qronFloorRuntime.minimumFloor)
            return floor;
    }
    return -1000;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
float GetQronFloorHeight(std::int32_t floor) {
    QronFloorRuntime *runtime = &g_qronFloorRuntime;
    if (!runtime->areaId || floor > runtime->maximumFloor || floor < runtime->minimumFloor)
        return FLT_MAX;
    return QronFloorHeight(floor, runtime->baseHeight);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t GetQronMaximumFloor() {
    return g_qronFloorRuntime.areaId ? g_qronFloorRuntime.maximumFloor : -1000;
}

std::int32_t GetQronMinimumFloor() {
    return g_qronFloorRuntime.areaId ? g_qronFloorRuntime.minimumFloor : -1000;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::uint32_t GetQronCurrentFlags() {
    return g_qronFloorRuntime.areaId ? g_qronFloorRuntime.flags : 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
float FindQronIntersectionHeight(const Vector3 *position, std::int32_t floor) {
    QronFloorRuntime *runtime = &g_qronFloorRuntime;
    if (!runtime->areaId || floor > runtime->maximumFloor || floor < runtime->minimumFloor)
        return -FLT_MAX;
    if ((runtime->flags & 1U) == 0)
        return FindFieldIntersectionHeight(position);
    const float floorHeight = GetQronFloorHeight(floor);
    Vector3 local = *position;
    local.y = (runtime->temporaryFloorHeight - floorHeight) + local.y;
    const float height = FindFieldIntersectionHeight(&local);
    if (height == -FLT_MAX)
        return -FLT_MAX;
    return (height + floorHeight) - runtime->temporaryFloorHeight;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t SetAreaFogParams() {
    if (!g_qronFloorRuntime.areaId)
        return 0;
    const MapRenderProfile &profile = MatchMapRenderProfile(g_currentMap);
    SetFogRed(profile.farDistance * 0.875f);
    SetFogGreen(profile.farDistance);
    SetFogBlue(1.0f);
    SetFogDistance(0xff000000U);
    ReinitFogTasks(1);
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t mapSetupLighting(std::int32_t enabled) {
    QronFloorRuntime *runtime = &g_qronFloorRuntime;
    if (!runtime->areaId)
        return 0;
    if (runtime->flags & 1U) {
        if (GetMagicRoomFlag(31) == 0x7fffU)
            return 0;
        SetMapLightingPlacementEnabled(enabled, 31);
    } else {
        if (runtime->lightIndex == -1)
            return 0;
        SetMapModelEnabled(static_cast<std::uint32_t>(enabled), runtime->lightIndex);
    }
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t ResolveQronLoadLocation(std::uint32_t *subarea) {
    const QronMapDefinition *const definition = FindQronMapDefinition(g_currentMap);
    if (!definition)
        return -1000;
    Vector3 position;
    std::int32_t yaw;
    const std::int32_t stream = ResolveQronEntryLocation(*definition, position, yaw);
    *subarea = scenFindMapRegionName(&position);
    return stream;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
bool IsCurrentAreaQA11() {
    if (g_currentMap == AREA_QA11)
        return true;
    return false;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t SetQronRelocatedFloor(std::int32_t floor, std::int32_t) {
    QronFloorRuntime &runtime = g_qronFloorRuntime;
    Vector3 translation = {0.0f, 0.0f, 0.0f};
    if (!runtime.areaId || (runtime.flags & 1U) == 0 || floor > runtime.maximumFloor || floor < runtime.minimumFloor)
        return 0;
    runtime.floor = floor;
    runtime.temporaryFloorHeight = QronFloorHeight(floor, runtime.baseHeight);
    translation.y = runtime.temporaryFloorHeight - 100.0f;
    if (runtime.callback)
        runtime.callback(&translation, static_cast<std::uint32_t>(runtime.floor));
    RefreshQronUpperFloorPlacements(&runtime);
    SetOccupancyForThisFieldSegment(runtime.fieldName, &translation, 0);
    if (g_currentMap == AREA_QF00)
        SetOccupancyForThisFieldSegment(QRON_FIELD_QF0X, &translation, 0);
    if (g_currentMap == AREA_QF02) {
        SetOccupancyForThisFieldSegment(QRON_FIELD_QF03, &translation, 0);
        SetOccupancyForThisFieldSegment(QRON_FIELD_QF04, &translation, 0);
    }
    SetPropVector3(&translation);
    UpdatePropRoot();
    SetMapLightingPlacementPosition(&translation, 31);
    for (std::int32_t index = 0; index <= 4; ++index)
        if (GetMagicRoomFlag(index) != 0x7fffU)
            SetMapLightingPlacementPosition(&translation, index);
    translation.y += 3.5999999f;
    for (std::int32_t index = 5; index <= 9; ++index)
        if (GetMagicRoomFlag(index) != 0x7fffU)
            SetMapLightingPlacementPosition(&translation, index);
    translation.y -= 3.5999999f;
    translation.y -= 3.5999999f;
    for (std::int32_t index = 10; index <= 14; ++index)
        if (GetMagicRoomFlag(index) != 0x7fffU)
            SetMapLightingPlacementPosition(&translation, index);
    translation.y += 3.5999999f;
    std::int32_t enabled = 1;
    for (std::int32_t index = 5; index <= 9; ++index)
        if (GetMagicRoomFlag(index) != 0x7fffU)
            SetMapLightingPlacementEnabled(enabled, index);
    for (std::int32_t index = 10; index <= 14; ++index)
        if (GetMagicRoomFlag(index) != 0x7fffU)
            SetMapLightingPlacementEnabled(enabled, index);
    SetLightRootPosition(translation);
    UpdateAllElevatorTransforms();
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t SetQronStackedFloor(std::int32_t floor) {
    QronFloorRuntime &runtime = g_qronFloorRuntime;
    if (!runtime.areaId || !(runtime.flags & 8U) || floor > runtime.maximumFloor || floor < runtime.minimumFloor)
        return 0;
    runtime.floor = floor;
    runtime.temporaryFloorHeight = QronFloorHeight(floor, runtime.baseHeight);
    UpdateAllElevatorTransforms();
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t qronSetFloor(std::int32_t floor) {
    QronFloorRuntime *runtime = &g_qronFloorRuntime;
    if (!runtime->areaId)
        return 0;
    if ((runtime->flags & 1U) == 0)
        return 0;
    if (!SetQronRelocatedFloor(floor, 1))
        return 0;
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t qronRepositionPlayerForFloor(std::int32_t floor) {
    QronFloorRuntime *runtime = &g_qronFloorRuntime;
    if (!runtime->areaId)
        return 0;
    if ((runtime->flags & 1U) == 0)
        return 0;
    if (!SetQronRelocatedFloor(floor, 1))
        return 0;
    MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Vector3 position;
    GetCharacterWorldPosition(control->ryo, &position);
    control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const std::int32_t yaw = GetCharacterWorldYaw(control->ryo);
    position.y = (runtime->temporaryFloorHeight + 3.5999999f) - 0.1f;
    position.y = FindFieldIntersectionHeight(&position);
    if (position.y == -FLT_MAX)
        position.y = runtime->temporaryFloorHeight;
    control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    SetCharacterWorldPosition(control->ryo, &position);
    control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    SetCharacterWorldYaw(control->ryo, yaw);
    return 1;
}
#endif

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t RefreshQronUpperFloorPlacements(QronFloorRuntime *runtime) {
    Vector3 position;
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    GetCharacterWorldPosition(control->ryo, &position);
    position.y += 1.0f;
    switch (scenFindIntersection(&position)) {
    case 0:
    case 2:
    case 3:
    case 4:
        return EnableQronUpperFloorPlacements(runtime);
    case -1:
    case 1:
    case 5:
        return RestoreQronUpperFloorPlacements(runtime);
    }
    return 0;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t EnableQronUpperFloorPlacements(QronFloorRuntime *runtime) {
    if ((runtime->flags & 1U) == 0)
        return 0;
    if (runtime->flags & 4U)
        return 0;
    for (std::int32_t index = 0; index < 5; ++index) {
        runtime->savedMapSlots[index] = GetMagicRoomFlag(index + 5);
        const std::uint32_t slot = GetMagicRoomFlag(index);
        if (slot != 0x7fffU)
            AttachMapLightingPlacement(static_cast<std::int32_t>(slot), index + 5);
    }
    runtime->flags |= 4U;
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t RestoreQronUpperFloorPlacements(QronFloorRuntime *runtime) {
    if ((runtime->flags & 1U) == 0)
        return 0;
    if ((runtime->flags & 4U) == 0)
        return 0;
    for (std::int32_t index = 0; index < 5; ++index) {
        if (runtime->savedMapSlots[index] == 0x7fffU)
            AttachMapLightingPlacement(-1, index + 5);
        else
            AttachMapLightingPlacement(static_cast<std::int32_t>(runtime->savedMapSlots[index]), index + 5);
        runtime->savedMapSlots[index] = 0x7fff;
    }
    runtime->flags &= ~4U;
    return 1;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
std::int32_t ChangeQronStairArea(std::int32_t direction, std::int32_t connectionId, const Vector3 *anchor, std::int32_t yaw) {
    QronFloorRuntime *runtime = &g_qronFloorRuntime;
    if (!connectionId || !runtime->connections)
        return 0;
    for (const QronStairConnection *connection = runtime->connections; connection->areaId; ++connection) {
        if (connection->connectionId != connectionId || (direction == 0 && connection->floor - 1 != runtime->floor) ||
            (direction == 1 && connection->floor + 1 != runtime->floor))
            continue;
        std::int32_t rotation = yaw;
        matrix_stack::PushUnitMatrix();
        matrix_stack::TranslateVector(anchor);
        Matrix4x4::RotateAroundY_Wrapper(rotation);
        if (direction == 0)
            Matrix4x4::preTranslate(-0.89999998f, 0.0f, -4.1999998f);
        else if (direction == 1)
            Matrix4x4::preTranslate(-2.7f, 0.0f, 1.0f);
        Vector3 position;
        matrix_stack::GetTranslation(&position);
        PopCurrentMatrix(1);
        shenToggleAreaFlags(2);
        QueueQronElevatorSceneChange(connection->areaId, connection->entryId, connection->floor, &position, rotation);
        std::uint16_t *mode = reinterpret_cast<std::uint16_t *>(reinterpret_cast<std::byte *>(g_qronSceneChange) + kQronSceneModeOffset);
        *mode = 2;
        return 1;
    }
    return 0;
}
#endif

} // namespace

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void QueueQronElevatorSceneChange(shen_name area, std::int32_t entry, std::int32_t floor, const Vector3 *position, std::int32_t yaw) {
    g_qronSceneChange[0] = area;
    g_qronSceneChange[1] = static_cast<std::uint32_t>(entry);
    *reinterpret_cast<std::uint16_t *>(reinterpret_cast<std::byte *>(g_qronSceneChange) + kQronSceneModeOffset) = 3;
    *reinterpret_cast<std::uint16_t *>(reinterpret_cast<std::byte *>(g_qronSceneChange) + kQronSceneFloorOffset) = static_cast<std::uint16_t>(floor);
    g_qronSceneChange[3] = static_cast<std::uint32_t>(yaw);
    *reinterpret_cast<Vector3 *>(g_qronSceneChange + 4) = *position;
    QueueQronSceneChange();
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
void QronFloorControl_Callback(void *) {
    QronFloorRuntime &runtime = g_qronFloorRuntime;
    Vector3 position;
    Vector3 anchor = {0.0f, 0.0f, 0.0f};
    std::int32_t yaw = 0, connectionId = 0;
    std::int32_t floor;
    if (!runtime.areaId || plymIsSearchBlocked())
        return;
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    GetCharacterWorldPosition(control->ryo, &position);
    Vector3 query = position;
    query.y += 1.0f;
    const std::int32_t intersection = scenFindIntersection(&query);
    const std::uint32_t steps = flddQueryStairConnection(&query, &anchor, &yaw, &connectionId);
    if (runtime.flags & 8U) {
        floor = runtime.floor;
        if (position.y > QronFloorHeight(static_cast<std::int32_t>(static_cast<std::uint32_t>(floor) + 1U), runtime.baseHeight) - 1.0f)
            SetQronStackedFloor(static_cast<std::int32_t>(static_cast<std::uint32_t>(floor) + 1U));
        else if (position.y < QronFloorHeight(floor, runtime.baseHeight) - 1.0f)
            SetQronStackedFloor(static_cast<std::int32_t>(static_cast<std::uint32_t>(floor) - 1U));
    }
    if (runtime.previousIntersection != intersection) {
        floor = runtime.floor;
        switch (intersection) {
        case 0:
            ChangeQronStairArea(1, connectionId, &anchor, yaw);
            break;
        case 1:
            RestoreQronUpperFloorPlacements(&runtime);
            break;
        case 4:
            ChangeQronStairArea(0, connectionId, &anchor, yaw);
        case 2:
        case 3:
            EnableQronUpperFloorPlacements(&runtime);
            break;
        case 5:
            SetQronRelocatedFloor(static_cast<std::int32_t>(static_cast<std::uint32_t>(floor) + 1U));
            break;
        case -1:
            RestoreQronUpperFloorPlacements(&runtime);
            break;
        }
        runtime.previousIntersection = intersection;
    }
    if (intersection == 0 && steps && position.y - anchor.y < 1.575f)
        SetQronRelocatedFloor(static_cast<std::int32_t>(static_cast<std::uint32_t>(runtime.floor) - 1U));
}
#endif

// @verified - d
HLib::Task *QueueQronSceneChange() {
    SetPlayerControlEnabled(0);
    SetScreenFilmFade(20, 0, 0, 0, 0, 255, 0, 0, 0);
    spmFadeOut(0);
    return HLib::EnqueueTaskWithoutParameter(QronSceneChange_Callback, 3, 2, static_cast<std::int32_t>(TASK_QRON));
}

// @verified - d
void QronSceneChange_Callback(void *) {
    std::uint32_t *change = g_qronSceneChange;
    if (filmCheckFadeTrackColour())
        return;
    shenQueueChangeModuleAndScene(SystemModule_Game, g_actNumber, shen_name(change[0]), static_cast<std::int32_t>(change[1]));
    HLib::DestroyCurrentEventTask();
}

namespace {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
// @verified - d
bool FindMapEventEntryTransform(shen_name areaId, std::int32_t entryId, Vector3 &position, std::int32_t &yaw) {
    const AreaTableEntry *const area = GetAreaTableEntry(areaId);
    if (area->area == UINT32_MAX)
        return false;
    const MapEventTransition *entry = area->transitions;
    while (entry->eventId != -1 && entry->eventId != entryId)
        ++entry;
    if (entry->eventId == -1)
        return false;
    position = entry->position;
    yaw = *reinterpret_cast<const std::int32_t *>(&entry->yaw);
    return true;
}
#endif

} // namespace

} // namespace s2
} // namespace shenmue

#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern "C" float __attribute__((renesas)) shenmue_s2_event_shenFindIntersectionWithScene(const shenmue::Vector3 *position) {
    return shenmue::s2::FindFieldIntersectionHeight(position);
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_qronSetFloorDescriptor(const shenmue::s2::QronFloorDescriptor *descriptor) {
    return shenmue::s2::qronSetFloorDescriptor(descriptor);
}
extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_qronSetFloor(std::int32_t floor) {
    return shenmue::s2::qronSetFloor(floor);
}
extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_qronRepositionPlayerForFloor(std::int32_t floor) {
    return shenmue::s2::qronRepositionPlayerForFloor(floor);
}
extern "C" float __attribute__((renesas)) shenmue_s2_event_GetQronTemporaryFloorHeight() {
    return shenmue::s2::GetQronTemporaryFloorHeight();
}
#endif

#endif
