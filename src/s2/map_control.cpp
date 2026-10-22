#include "s2/map_control.hpp"
#include "d3t/object_store.hpp"
#include "d3t/archive_handles.hpp"
#include "d3t/ui_renderer.hpp"
#include "s2/mt7.hpp"
#include "s2/advice.hpp"
#include "s2/area_jump.hpp"

#include "s2/game_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/face.hpp"
#include "s2/area_event.hpp"
#include "s2/audio_data.hpp"
#include "s2/camera.hpp"
#include "s2/conversation.hpp"
#include "s2/character.hpp"
#include "s2/model_component.hpp"
#include "s2/model_image.hpp"
#include "s2/input.hpp"
#include "s2/door.hpp"
#include "s2/elevator.hpp"
#include "s2/date_time.hpp"
#include "s2/directory_cache.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/flag.hpp"
#include "s2/font.hpp"
#include "s2/legacy_hud_text.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fog.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/lighting.hpp"
#include "s2/legacy_sprite.hpp"
#include "s2/maze.hpp"
#include "s2/magic_room.hpp"
#include "s2/load_title.hpp"
#include "s2/pause.hpp"
#include "s2/system_modules.hpp"
#include "s2/handcuffs.hpp"
#include "s2/hand.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_info.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/author.hpp"
#include "s2/navigation.hpp"
#include "s2/motion.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/item_wf.hpp"
#include "s2/motion_work.hpp"
#include "s2/pak.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/pawn_shop.hpp"
#include "s2/pati.hpp"
#include "s2/particle_effects.hpp"
#include "s2/renderer_initialization.hpp"
#include "s2/save.hpp"
#include "s2/search.hpp"
#include "s2/screen_film.hpp"
#include "s2/snapshot.hpp"
#include "s2/sound_program.hpp"
#include "s2/tree.hpp"
#include "s2/ui.hpp"
#include "s2/world_render.hpp"
#include "s2/weather.hpp"
#include "s2/warning.hpp"
#include "s2/telephone_cord.hpp"

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>
#include <string_view>
#include "s2/area_names.hpp"

namespace shenmue::s2 {

HLib::Task *g_mapControlTask = nullptr;
shen_name g_currentMap{};
std::int32_t g_actNumber = 0;
std::int32_t g_entryPoint = 0;
std::int32_t g_currentShenmue1SaveAreaId = 0;
bool g_worldDrawingEnabled = false;

// @verified - p
MapControlData *SetMapControlStatusFlag(std::uint8_t bit, std::int32_t enabled) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const std::uint32_t mask = 1U << (bit & 31U);
    if (enabled)
        control->statusFlags |= mask;
    else
        control->statusFlags &= ~mask;
    return control;
}

namespace {
struct Shenmue1SaveAreaMapping {
    std::uint32_t mapId;
    std::int32_t saveAreaId;
};



constexpr Shenmue1SaveAreaMapping shenmue1SaveAreaMappings[] = {
    {SHEN_NAME(D000), 0x63},
    {SHEN_NAME(JD00), 0x13},
    {SHEN_NAME(JU00), 0x0f},
    {SHEN_NAME(JHD0), 0x10},
    {SHEN_NAME(MN01), 0x55},
    {SHEN_NAME(MO01), 0x5f},
    {SHEN_NAME(YDB1), 0x0e},
    {SHEN_NAME(MS08), 0x4c},
    {SHEN_NAME(DSBA), 0x31},
    {SHEN_NAME(DCHA), 0x29},
    {SHEN_NAME(DPIA), 0x38},
    {SHEN_NAME(DKPA), 0x39},
    {SHEN_NAME(DRSA), 0x2e},
    {SHEN_NAME(DAZA), 0x28},
    {SHEN_NAME(DBHB), 0x2a},
    {SHEN_NAME(JABE), 0x17},
    {SHEN_NAME(DRME), 0x3a},
    {SHEN_NAME(DSKI), 0x26},
    {SHEN_NAME(DSLI), 0x37},
    {SHEN_NAME(DMAJ), 0x35},
    {SHEN_NAME(DCBN), 0x32},
    {SHEN_NAME(DURN), 0x34},
    {SHEN_NAME(JOMO), 0x00},
    {SHEN_NAME(DBYO), 0x2b},
    {SHEN_NAME(TATQ), 0x2d},
    {SHEN_NAME(MS8S), 0x53},
    {SHEN_NAME(DSUS), 0x33},
    {SHEN_NAME(DGCT), 0x25},
    {SHEN_NAME(DRHT), 0x2f},
    {SHEN_NAME(DSLT), 0x36},
    {SHEN_NAME(MKYU), 0x61},
    {SHEN_NAME(DTKY), 0x30},
    {SHEN_NAME(MKSG), 0x46},
    {SHEN_NAME(DKTY), 0x27},
    {SHEN_NAME(DJAZ), 0x2c},
    {SHEN_NAME(DYKZ), 0x3b},
    {SHEN_NAME(MFSY), 0x64},
    {0, -1},
};

constexpr shen_name mapEventTaskToken{SHEN_NAME(MEVE)};
constexpr shen_name mapEventFadeTaskToken{SHEN_NAME(EVFO)};

struct MapEventTaskParameters {
    const MapEventCallbackEntry *callbacks;
};

static_assert(sizeof(MapEventTaskParameters) == 0x08);
static_assert(offsetof(MapEventTaskParameters, callbacks) == 0x00);

#include "s2/map_event_data.inl"

const MapEventAreaDefinition *activeMapEventAreas = hongKongMapEventAreas;


constexpr MapEventCallbackEntry mapEventTaskEntryData[]{{.callback = nullptr, .eventId = -1, .reserved = 0, .argument = 0}};


shen_name g_newAreaId{};
std::int32_t g_newEntryId{};
std::uint32_t g_newScenarioFlags{};

struct QronStairConnection {
    shen_name areaId;
    std::int16_t floor;
    std::int8_t connectionId;
    std::int8_t entryId;
};

struct QronMapDefinition {
    shen_name areaId;
    float baseHeight;
    std::int32_t maximumFloor;
    std::int32_t minimumFloor;
    const QronStairConnection *connections;
    std::int32_t lightIndex;
    std::uint32_t flags;
};

static_assert(sizeof(QronStairConnection) == 8);
static_assert(sizeof(QronMapDefinition) == 0x20);
static_assert(offsetof(QronMapDefinition, maximumFloor) == 8);
static_assert(offsetof(QronMapDefinition, minimumFloor) == 0x0c);
static_assert(offsetof(QronMapDefinition, connections) == 0x10);

constexpr QronStairConnection qronStairConnections[] = {
    {SHEN_NAME(QAE1), 1, 1, 0},
    {SHEN_NAME(QAW1), 1, 3, 0},
    {SHEN_NAME(QAW1), 1, 4, 0},
    {SHEN_NAME(QAE6), 6, 1, 0},
    {SHEN_NAME(QAW6), 6, 3, 0},
    {SHEN_NAME(QAW6), 6, 4, 0},
    {},
    {SHEN_NAME(QA00), 2, 3, 0},
    {SHEN_NAME(QA00), 2, 4, 0},
    {},
    {SHEN_NAME(QA00), 2, 1, 0},
    {},
    {SHEN_NAME(QA00), 7, 3, 0},
    {SHEN_NAME(QA00), 7, 4, 0},
    {SHEN_NAME(QA00), 5, 3, 0},
    {SHEN_NAME(QA00), 5, 4, 0},
    {},
    {SHEN_NAME(QA00), 7, 1, 0},
    {SHEN_NAME(QA00), 5, 1, 0},
    {},
    {SHEN_NAME(QC01), 1, 1, 0},
    {SHEN_NAME(QC01), 1, 2, 0},
    {SHEN_NAME(QC06), 6, 1, 0},
    {SHEN_NAME(QC06), 6, 2, 0},
    {SHEN_NAME(QUG0), -1, 2, 0},
    {},
    {SHEN_NAME(QC00), 2, 1, 0},
    {SHEN_NAME(QC00), 2, 2, 0},
    {},
    {SHEN_NAME(QC00), 7, 1, 0},
    {SHEN_NAME(QC00), 7, 2, 0},
    {SHEN_NAME(QC00), 5, 1, 0},
    {SHEN_NAME(QC00), 5, 2, 0},
    {},
    {SHEN_NAME(QC00), 0, 2, 0},
    {},
    {SHEN_NAME(QD01), 1, 1, 0},
    {SHEN_NAME(QD01), 1, 2, 0},
    {SHEN_NAME(QD01), 1, 3, 0},
    {},
    {SHEN_NAME(QD00), 2, 1, 0},
    {SHEN_NAME(QD00), 2, 2, 0},
    {SHEN_NAME(QD00), 2, 3, 0},
    {},
    {SHEN_NAME(QE01), 1, 1, 0},
    {SHEN_NAME(QE01), 1, 2, 0},
    {SHEN_NAME(QE01), 1, 3, 0},
    {SHEN_NAME(QE06), 6, 1, 0},
    {SHEN_NAME(QE06), 6, 2, 0},
    {SHEN_NAME(QE06), 6, 3, 0},
    {},
    {SHEN_NAME(QE00), 2, 1, 0},
    {SHEN_NAME(QE00), 2, 2, 0},
    {SHEN_NAME(QE00), 2, 3, 0},
    {},
    {SHEN_NAME(QE00), 7, 1, 0},
    {SHEN_NAME(QE00), 7, 2, 0},
    {SHEN_NAME(QE00), 7, 3, 0},
    {SHEN_NAME(QE00), 5, 1, 0},
    {SHEN_NAME(QE00), 5, 2, 0},
    {SHEN_NAME(QE00), 5, 3, 0},
    {},
    {SHEN_NAME(QF02), 4, 1, 0},
    {SHEN_NAME(QF02), 4, 2, 0},
    {SHEN_NAME(QF02), 4, 3, 0},
    {SHEN_NAME(QF02), 4, 4, 0},
    {SHEN_NAME(QF06), 6, 1, 0},
    {SHEN_NAME(QF06), 6, 4, 0},
    {SHEN_NAME(QF39), 40, 4, 0},
    {},
    {SHEN_NAME(QF02), 2, 1, 0},
    {SHEN_NAME(QF02), 2, 2, 0},
    {SHEN_NAME(QF02), 2, 3, 0},
    {SHEN_NAME(QF02), 2, 4, 0},
    {},
    {SHEN_NAME(QF00), 5, 1, 0},
    {SHEN_NAME(QF00), 5, 2, 0},
    {SHEN_NAME(QF00), 5, 3, 0},
    {SHEN_NAME(QF00), 5, 4, 0},
    {SHEN_NAME(QF01), 1, 1, 0},
    {SHEN_NAME(QF01), 1, 2, 0},
    {SHEN_NAME(QF01), 1, 3, 0},
    {SHEN_NAME(QF01), 1, 4, 0},
    {},
    {SHEN_NAME(QF00), 7, 1, 0},
    {SHEN_NAME(QF00), 7, 4, 0},
    {SHEN_NAME(QF00), 5, 1, 0},
    {SHEN_NAME(QF00), 5, 4, 0},
    {},
    {SHEN_NAME(QF00), 39, 4, 0},
    {},
};

constexpr QronMapDefinition qronMapDefinitions[] = {
    {SHEN_NAME(QA00), 100.0f, 10, 2, qronStairConnections, -1, 1U},
    {SHEN_NAME(QAW1), 100.0f, 1, 1, qronStairConnections + 7, 2, 48U},
    {SHEN_NAME(QAE1), 100.0f, 1, 1, qronStairConnections + 10, -1, 48U},
    {SHEN_NAME(QAW6), 100.0f, 6, 6, qronStairConnections + 12, 2, 0U},
    {SHEN_NAME(QAE6), 100.0f, 6, 6, qronStairConnections + 17, -1, 0U},
    {SHEN_NAME(QB00), 100.0f, 3, 1, nullptr, -1, 24U},
    {SHEN_NAME(QA11), 100.0f, 11, 11, nullptr, -1, 18U},
    {SHEN_NAME(QJBT), 100.0f, 4, -3, nullptr, -1, 1U},
    {SHEN_NAME(QC00), 100.0f, 12, 0, qronStairConnections + 20, -1, 1U},
    {SHEN_NAME(QC01), 100.0f, 1, 1, qronStairConnections + 26, 6, 50U},
    {SHEN_NAME(QC06), 100.0f, 6, 6, qronStairConnections + 29, -1, 0U},
    {SHEN_NAME(QUG0), 100.0f, 1, -1, qronStairConnections + 34, -1, 8U},
    {SHEN_NAME(QD00), 100.0f, 10, 2, qronStairConnections + 36, -1, 19U},
    {SHEN_NAME(QD01), 100.0f, 1, 1, qronStairConnections + 40, -1, 50U},
    {SHEN_NAME(QE00), 100.0f, 13, 2, qronStairConnections + 44, -1, 1U},
    {SHEN_NAME(QE01), 100.0f, 1, 1, qronStairConnections + 51, -1, 50U},
    {SHEN_NAME(QE06), 100.0f, 6, 6, qronStairConnections + 55, -1, 0U},
    {SHEN_NAME(QE09), 100.0f, 2, 2, nullptr, -1, 1U},
    {SHEN_NAME(QLBT), 100.0f, 12, 9, nullptr, -1, 8U},
    {SHEN_NAME(QF00), 100.0f, 39, 5, qronStairConnections + 62, -1, 1U},
    {SHEN_NAME(QF01), 100.0f, 1, 1, qronStairConnections + 70, -1, 32U},
    {SHEN_NAME(QF02), 100.0f, 4, 2, qronStairConnections + 75, -1, 1U},
    {SHEN_NAME(QF06), 100.0f, 6, 6, qronStairConnections + 84, -1, 0U},
    {SHEN_NAME(QF39), 100.0f, 40, 40, qronStairConnections + 89, -1, 0U},
    {SHEN_NAME(QKBT), 100.0f, -2, -4, nullptr, -1, 8U},
    {{}, 100.0f, 0, 0, nullptr, 0, 0},
};

// QRON's retained floor/carry owners are separate from the EVFO request.
// In particular, an untracked destination does not erase the previous carry.
struct QronFloorRuntime {
    shen_name areaId;
    float baseHeight;
    std::int32_t maximumFloor;
    std::int32_t minimumFloor;
    float floorHeight;
    std::int32_t floor;
    QronFloorCallback floorCallback;
    std::uint32_t flags;
    const QronStairConnection *connections;
    std::int32_t lightIndex;
    QronFloorProp *props;
    std::int32_t previousIntersection;
    std::uint32_t savedMapSlots[5];
};
static_assert(sizeof(QronFloorRuntime) == 0x58);
static_assert(offsetof(QronFloorRuntime, floorCallback) == 0x18 && offsetof(QronFloorRuntime, flags) == 0x20);
static_assert(offsetof(QronFloorRuntime, lightIndex) == 0x30 && offsetof(QronFloorRuntime, previousIntersection) == 0x40);

struct QronSceneChange {
    shen_name areaId{};
    std::int32_t entryId{};
    std::int16_t mode{};
    std::int16_t floor{};
    std::int32_t yaw{};
    Vector3 position{};
};

QronFloorRuntime qronFloorRuntime{};
QronSceneChange qronSceneChange{};
std::int32_t qronEntryFloor = -1000;
std::uint64_t qronPropPlacements[17]{};

void BindQronFloorProps() {
    if (!qronFloorRuntime.props)
        return;
    MT7Node *const root = GetPropRoot();
    if (!root)
        return;
    const std::span<const PropReaderSlot, 17> readers = GetPropReaderSlots();
    for (std::int32_t index = 0; index < 17; ++index) {
        d3t::RetireObjectPlacement(&qronPropPlacements[index]);
        if (readers[index].reader && !readers[index].reader->nodes.empty() && readers[index].lastSibling)
            (void)d3t::RegisterObjectPlacement(GetMT7ReaderData(readers[index].reader), &qronPropPlacements[index], 0x10);
    }
    for (QronFloorProp *prop = qronFloorRuntime.props; prop->name.name; ++prop) {
        prop->node = FindModelCharacterRecursive(root, prop->name);
        if (prop->node && static_cast<std::uint8_t>(prop->name.name) != 'R')
            SetDisplayFlagForProp(prop->name, 0);
    }
}

void UpdateQronFloorProps() {
    if (!qronFloorRuntime.props)
        return;
    for (QronFloorProp *prop = qronFloorRuntime.props; prop->name.name; ++prop) {
        if (!prop->node)
            continue;
        const std::uint32_t above = static_cast<std::uint32_t>(qronFloorRuntime.floor) + 16U;
        const std::uint32_t current = static_cast<std::uint32_t>(qronFloorRuntime.floor) + 15U;
        const std::uint32_t below = static_cast<std::uint32_t>(qronFloorRuntime.floor) + 14U;
        prop->aboveEnabled = above > 63U ? 0 : (prop->floorMasks[above >> 4U] >> (above & 15U)) & 1U;
        prop->currentEnabled = current > 63U ? 0 : (prop->floorMasks[current >> 4U] >> (current & 15U)) & 1U;
        if (static_cast<std::uint8_t>(prop->name.name) == 'F')
            (void)ToggleFlagOnFieldEntity(prop->name, prop->currentEnabled);
        prop->belowEnabled = below > 63U ? 0 : (prop->floorMasks[below >> 4U] >> (below & 15U)) & 1U;
    }
}

void EnableQronUpperFloorPlacements() {
    if ((qronFloorRuntime.flags & 1U) == 0 || (qronFloorRuntime.flags & 4U) != 0)
        return;
    Vector3 translation{};
    for (std::int32_t index = 5; index < 10; ++index) {
        qronFloorRuntime.savedMapSlots[index - 5] = GetMagicRoomFlag(index);
        const std::uint32_t slot = GetMagicRoomFlag(index - 5);
        if (slot != 0x7fffU) {
            (void)AttachMapLightingPlacement(static_cast<std::int32_t>(slot), index);
            translation.y = (qronFloorRuntime.floorHeight - 100.0f) + 3.5999999f;
            (void)SetMapLightingPlacementPosition(&translation, index);
        }
    }
    qronFloorRuntime.flags |= 4U;
}

void RestoreQronUpperFloorPlacements() {
    if ((qronFloorRuntime.flags & 1U) == 0 || (qronFloorRuntime.flags & 4U) == 0)
        return;
    for (std::int32_t index = 5; index < 10; ++index) {
        const std::uint32_t slot = qronFloorRuntime.savedMapSlots[index - 5];
        (void)AttachMapLightingPlacement(slot == 0x7fffU ? -1 : static_cast<std::int32_t>(slot), index);
        qronFloorRuntime.savedMapSlots[index - 5] = 0x7fffU;
    }
    qronFloorRuntime.flags &= ~4U;
}

void DrawQronOpaqueBranch(const MT7Model &model, MT7Node *node) {
    for (; node; node = node->runtimeSibling) {
        (void)Matrix4x4_Push();
        ApplyMT7NodeTransform(*node);
        if (node->xb01Offset)
            (void)ExecuteLegacyDrawCommandStream(model.source, node->xb01Offset, node->borrowedMesh ? node->borrowedMesh->drawResources : node->drawResources,
                GetLegacyCharacterShaderPath(), 1.0f);
        if (node->runtimeChild)
            DrawQronOpaqueBranch(model, node->runtimeChild);
        Matrix4x4::Pop(1);
    }
}

void DrawQronFloorProps(void *) {
    MT7Node *const root = GetPropRoot();
    Vector3 position;
    GetPropVector4(&position);
    if (!root || !qronFloorRuntime.props)
        return;
    const std::span<const PropReaderSlot, 17> readers = GetPropReaderSlots();
    const bool upper = (qronFloorRuntime.flags & 0x40U) != 0;
    for (std::int32_t index = 0; index < 17; ++index) {
        MT7Model *const model = readers[index].reader;
        MT7Node *const last = readers[index].lastSibling;
        if (!model || model->nodes.empty() || !last)
            continue;
        MT7Node *const savedSibling = last->runtimeSibling;
        last->runtimeSibling = nullptr;
        MT7Node *const modelRoot = model->nodes.data();
        for (MT7Node *node = modelRoot->runtimeChild; node; node = node->runtimeSibling)
            node->runtimeParent = modelRoot;
        bool selected = false;
        for (QronFloorProp *prop = qronFloorRuntime.props; prop->name.name && !selected; ++prop) {
            if (!prop->node)
                continue;
            for (MT7Node *node = modelRoot->runtimeChild; node; node = node->runtimeSibling) {
                if (node == prop->node &&
                    ((prop->aboveEnabled && upper) ||
                        (static_cast<std::uint8_t>(prop->name.name) != 'R' &&
                            (prop->currentEnabled || (prop->belowEnabled && (upper || (prop->floorMasks[3] & 0xffff0000U))))))) {
                    selected = true;
                    break;
                }
            }
        }
        if (selected) {
            (void)Matrix4x4_Push(&transformMatrix_prePass);
            Matrix4x4::preTranslate(position.x, position.y + 3.5999999f, position.z);
            const bool hashed = d3t::HashModel(&qronPropPlacements[index], GetLegacyPerspectiveProjection());
            Matrix4x4::Pop(1);
            if (!hashed) {
                (void)Matrix4x4_Push();
                Matrix4x4::preTranslate(position.x, position.y, position.z);
                for (QronFloorProp *prop = qronFloorRuntime.props; prop->name.name; ++prop) {
                    if (!prop->node)
                        continue;
                    for (MT7Node *node = modelRoot->runtimeChild; node; node = node->runtimeSibling) {
                        if (node != prop->node)
                            continue;
                        (void)Matrix4x4_Push();
                        ApplyMT7NodeTransform(*node);
                        if (prop->aboveEnabled && upper) {
                            (void)Matrix4x4_Push();
                            Matrix4x4::preTranslate(0.0f, 3.5999999f, 0.0f);
                            if (node->xb01Offset)
                                (void)ExecuteLegacyDrawCommandStream(model->source, node->xb01Offset,
                                    node->borrowedMesh ? node->borrowedMesh->drawResources : node->drawResources, GetLegacyCharacterShaderPath(), 1.0f);
                            if (node->runtimeChild)
                                DrawQronOpaqueBranch(*model, node->runtimeChild);
                            Matrix4x4::Pop(1);
                        }
                        if (static_cast<std::uint8_t>(prop->name.name) != 'R') {
                            if (prop->currentEnabled) {
                                if (node->xb01Offset)
                                    (void)ExecuteLegacyDrawCommandStream(model->source, node->xb01Offset,
                                        node->borrowedMesh ? node->borrowedMesh->drawResources : node->drawResources, GetLegacyCharacterShaderPath(), 1.0f);
                                if (node->runtimeChild)
                                    DrawQronOpaqueBranch(*model, node->runtimeChild);
                            }
                            if (prop->belowEnabled && (upper || (prop->floorMasks[3] & 0xffff0000U))) {
                                (void)Matrix4x4_Push();
                                Matrix4x4::preTranslate(0.0f, -3.5999999f, 0.0f);
                                if (node->xb01Offset)
                                    (void)ExecuteLegacyDrawCommandStream(model->source, node->xb01Offset,
                                        node->borrowedMesh ? node->borrowedMesh->drawResources : node->drawResources, GetLegacyCharacterShaderPath(), 1.0f);
                                if (node->runtimeChild)
                                    DrawQronOpaqueBranch(*model, node->runtimeChild);
                                Matrix4x4::Pop(1);
                            }
                        }
                        Matrix4x4::Pop(1);
                    }
                }
                Matrix4x4::Pop(1);
            }
        }
        for (MT7Node *node = modelRoot->runtimeChild; node; node = node->runtimeSibling)
            node->runtimeParent = root;
        last->runtimeSibling = savedSibling;
    }
}

HLib::Task *QueueQronSceneChange();

const QronMapDefinition *FindQronMapDefinition(shen_name areaId) {
    for (const QronMapDefinition *entry = qronMapDefinitions; entry->areaId.name != 0; ++entry) {
        if (entry->areaId.name == areaId.name)
            return entry;
    }
    return nullptr;
}

float QronFloorHeight(std::int32_t floor, float baseHeight) {
    if (g_currentMap == AREA_QB00 || g_currentMap == AREA_QJBT || g_currentMap.name == SHEN_NAME(QLBT) || floor <= 1) {
        --floor;
    }
    return static_cast<float>(floor) * 3.5999999f + baseHeight;
}

std::int32_t QronFloorFromHeight(float height, float baseHeight) {
    const float offset = (height - baseHeight) + 0.1f;
    std::int32_t floor = static_cast<std::int32_t>(offset / 3.5999999f);
    if (offset > 0.0f)
        ++floor;
    if (g_currentMap != AREA_QB00 && g_currentMap != AREA_QJBT && g_currentMap.name != SHEN_NAME(QLBT) && floor > 1) {
        --floor;
    }
    return floor;
}

bool RetainQronMapEventDestination(shen_name areaId, std::int32_t eventId) {
    if (qronFloorRuntime.areaId.name == 0)
        return true;
    const QronMapDefinition *const destination = FindQronMapDefinition(areaId);
    if (!destination)
        return true;
    if (qronFloorRuntime.floor > destination->maximumFloor || qronFloorRuntime.floor < destination->minimumFloor) {
        return false;
    }
    // RetainQronMapEventDestination routes QD00 floor six through its own QRON owner,
    // returning false so MEVE consumes the trigger without queuing EVFO.
    if (g_currentMap.name == SHEN_NAME(QD00) && qronFloorRuntime.floor == 6 && (eventId == 2 || eventId == 3)) {
        qronSceneChange.areaId = eventId == 2 ? SHEN_NAME(QC06) : SHEN_NAME(QE06);
        qronSceneChange.entryId = eventId;
        qronSceneChange.mode = 1;
        qronSceneChange.floor = 6;
        shenToggleAreaFlags(2);
        QueueQronSceneChange();
        return false;
    }
    qronSceneChange.areaId = areaId;
    qronSceneChange.mode = 1;
    qronSceneChange.floor = static_cast<std::int16_t>(qronFloorRuntime.floor);
    return true;
}

// A relocating QRON map (flag 1) moves its single floor of field segments, props and lights to the selected floor.
// @verified - p
bool SetQronRelocatedFloor(std::int32_t floor) {
    if (qronFloorRuntime.areaId.name == 0 || (qronFloorRuntime.flags & 1U) == 0 || floor > qronFloorRuntime.maximumFloor ||
        floor < qronFloorRuntime.minimumFloor)
        return false;
    qronFloorRuntime.floor = floor;
    qronFloorRuntime.floorHeight = QronFloorHeight(floor, qronFloorRuntime.baseHeight);
    Vector3 translation{.x = 0.0f, .y = qronFloorRuntime.floorHeight - 100.0f, .z = 0.0f};
    if (qronFloorRuntime.floorCallback)
        qronFloorRuntime.floorCallback(&translation, static_cast<std::uint32_t>(floor));
    SetOccupancyForThisFieldSegment(SHEN_NAME(0000), &translation, 0);
    if (g_currentMap.name == SHEN_NAME(QF00))
        SetOccupancyForThisFieldSegment(SHEN_NAME(QF0X), &translation, 0);
    if (g_currentMap.name == SHEN_NAME(QF02)) {
        SetOccupancyForThisFieldSegment(SHEN_NAME(QF03), &translation, 0);
        SetOccupancyForThisFieldSegment(SHEN_NAME(QF04), &translation, 0);
    }
    SetPropVector3(&translation);
    AllocatePropFlagObject();
    (void)SetMapLightingPlacementPosition(&translation, 31);
    for (std::int32_t index = 0; index <= 4; ++index)
        if (GetMagicRoomFlag(index) != 0x7fffU)
            (void)SetMapLightingPlacementPosition(&translation, index);
    translation.y += 3.5999999f;
    for (std::int32_t index = 5; index <= 9; ++index)
        if (GetMagicRoomFlag(index) != 0x7fffU)
            (void)SetMapLightingPlacementPosition(&translation, index);
    translation.y += 3.5999999f;
    if (GetMagicRoomFlag(32) != 0x7fffU)
        (void)SetMapLightingPlacementPosition(&translation, 32);
    translation.y += -10.799999f;
    for (std::int32_t index = 10; index <= 14; ++index)
        if (GetMagicRoomFlag(index) != 0x7fffU)
            (void)SetMapLightingPlacementPosition(&translation, index);
    translation.y += 3.5999999f;
    for (std::int32_t index = 32; index <= 33; ++index)
        if (GetMagicRoomFlag(index) != 0x7fffU)
            SetMapLightingPlacementEnabled(1, index);
    for (std::int32_t index = 5; index <= 9; ++index)
        if (GetMagicRoomFlag(index) != 0x7fffU)
            SetMapLightingPlacementEnabled(1, index);
    for (std::int32_t index = 10; index <= 14; ++index)
        if (GetMagicRoomFlag(index) != 0x7fffU)
            SetMapLightingPlacementEnabled(1, index);
    (void)SetLightRootPosition(translation);
    UpdateAllElevatorTransforms();
    UpdateQronFloorProps();
    return true;
}

// A stacked QRON map (flag 8) keeps its floors in place and only tracks the current floor and height.
// @verified - p
bool SetQronStackedFloor(std::int32_t floor) {
    if (qronFloorRuntime.areaId.name == 0 || (qronFloorRuntime.flags & 8U) == 0 || floor > qronFloorRuntime.maximumFloor ||
        floor < qronFloorRuntime.minimumFloor)
        return false;
    qronFloorRuntime.floor = floor;
    qronFloorRuntime.floorHeight = QronFloorHeight(floor, qronFloorRuntime.baseHeight);
    UpdateAllElevatorTransforms();
    return true;
}


// @verified - p
void QronSceneChange_Callback(void *) {
    if (filmCheckFadeTrackColour())
        return;
    shenQueueChangeModuleAndScene(SystemModule::Game, g_actNumber, qronSceneChange.areaId, qronSceneChange.entryId);
    HLib::DestroyCurrentTask();
}


// @verified - p
HLib::Task *QueueQronSceneChange() {
    SetPlayerControlEnabled(0);
    SetScreenFilmFade(31, 0, 0, 0, 0, 255, 0, 0, 0);
    spmFadeOut();
    return HLib::EnqueueTaskWithoutParameter(QronSceneChange_Callback, 3, 2, static_cast<std::int32_t>(TASK_QRON.name));
}

void QronFloorControl_Callback(void *) {
    if (qronFloorRuntime.areaId.name == 0 || !g_playerRuntime.character)
        return;

    Vector3 position = GetCharacterWorldPosition(g_playerRuntime.character);
    position.y += 1.0f;
    const std::int32_t intersection = scenFindIntersection(position, position.y);
    Vector3 anchor{};
    std::int32_t yaw{};
    std::int32_t connectionId{};
    const std::uint32_t stepCount = flddQueryStairConnection(position, position.y, anchor, yaw, connectionId);
    position.y -= 1.0f;
    if (intersection != 0 && static_cast<std::uint32_t>(intersection - 2) > 2U) {
        qronFloorRuntime.flags &= ~0x40U;
    } else {
        qronFloorRuntime.flags |= 0x40U;
    }

    if (plymIsSearchBlocked()) {
        if (g_worldDrawingEnabled)
            (void)HLib::ExecuteCallbackNow(DrawQronFloorProps, nullptr);
        return;
    }

    if ((intersection == 0 || intersection == 4) && ryoNotBusy() && connectionId != 0 && qronFloorRuntime.connections) {
        const bool ascending = intersection == 4;
        for (const QronStairConnection *connection = qronFloorRuntime.connections; connection->areaId.name != 0; ++connection) {
            if (connection->connectionId == connectionId && connection->floor + (ascending ? -1 : 1) == qronFloorRuntime.floor) {
                Matrix4x4_PushActiveTransform();
                Matrix4x4::preTranslate(anchor.x, anchor.y, anchor.z);
                Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(yaw));
                Matrix4x4::preTranslate(ascending ? -0.89999998f : -2.7f, 0.0f, ascending ? -4.1999998f : 1.0f);
                const Vector3 landing{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
                Matrix4x4::Pop(1);
                shenToggleAreaFlags(2);
                QueueQronElevatorSceneChange(connection->areaId, connection->entryId, connection->floor, landing, yaw);
                qronSceneChange.mode = 2;
                break;
            }
        }
    }

    if ((qronFloorRuntime.flags & 8U) != 0) {
        if (position.y > QronFloorHeight(qronFloorRuntime.floor + 1, qronFloorRuntime.baseHeight) - 1.0f) {
            SetQronStackedFloor(qronFloorRuntime.floor + 1);
        } else if (position.y < QronFloorHeight(qronFloorRuntime.floor, qronFloorRuntime.baseHeight) - 1.0f) {
            SetQronStackedFloor(qronFloorRuntime.floor - 1);
        }
    }
    if (qronFloorRuntime.previousIntersection != intersection) {
        if (intersection == 5) {
            SetQronRelocatedFloor(qronFloorRuntime.floor + 1);
        }
        if ((static_cast<std::uint32_t>(intersection + 1) & ~2U) != 0U) {
            if (intersection >= 2 && intersection <= 4)
                EnableQronUpperFloorPlacements();
            else if (intersection == 5)
                RestoreQronUpperFloorPlacements();
        } else {
            RestoreQronUpperFloorPlacements();
        }
        qronFloorRuntime.previousIntersection = intersection;
    }
    if (intersection == 0 && stepCount != 0 && position.y - anchor.y < 1.575f) {
        SetQronRelocatedFloor(qronFloorRuntime.floor - 1);
        EnableQronUpperFloorPlacements();
    }
    if (g_worldDrawingEnabled)
        (void)HLib::ExecuteCallbackNow(DrawQronFloorProps, nullptr);
}


void QueueQronFloorControl() {
    std::memset(&qronFloorRuntime, 0, sizeof(qronFloorRuntime));
    qronFloorRuntime.previousIntersection = -1;
    for (std::uint32_t &slot : qronFloorRuntime.savedMapSlots)
        slot = 0x7fff;
    const QronMapDefinition *const definition = FindQronMapDefinition(g_currentMap);
    if (!definition)
        return;
    qronFloorRuntime.maximumFloor = definition->maximumFloor;
    qronFloorRuntime.minimumFloor = definition->minimumFloor;
    qronFloorRuntime.flags = definition->flags;
    qronFloorRuntime.connections = definition->connections;
    qronFloorRuntime.areaId = g_currentMap;
    qronFloorRuntime.lightIndex = definition->lightIndex;
    qronFloorRuntime.baseHeight = definition->baseHeight;
    Vector3 position{};
    std::int32_t yaw{};
    qronFloorRuntime.floor = ResolveQronEntryLocation(position, yaw);
    qronFloorRuntime.floorHeight = QronFloorHeight(qronFloorRuntime.floor, qronFloorRuntime.baseHeight);
    shenApplyTranslationToAll(&static_cast<Character *>(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo)->entity.node, position);
    shenDecrementRotYAllNodes(
        &static_cast<Character *>(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo)->entity.node, static_cast<std::int16_t>(yaw));
    ResetQronSceneChange();
    HLib::EnqueueTaskWithoutParameter(QronFloorControl_Callback, 12, 14, static_cast<std::int32_t>(TASK_QRON.name));
    if ((qronFloorRuntime.flags & 2U) == 0)
        SetIsSkySetup(0);
    if ((qronFloorRuntime.flags & 0x10U) == 0) {
        FreeWAIR();
        UpdateWeatherForMinute(1, 0, 0, 0.0f);
        UpdateWeather();
    }
}

} // namespace

// @verified - p
void SetTempMapIDs() {
    if (g_loadedActNumber > 0) {
        if (g_loadedActNumber <= 2)
            activeMapEventAreas = hongKongMapEventAreas;
        else if (g_loadedActNumber == 3)
            activeMapEventAreas = kowloonMapEventAreas;
        else if (g_loadedActNumber == 4)
            activeMapEventAreas = guilinMapEventAreas;
    }
    SetSpriteAreaConfigurations();
}

namespace {


bool plymPadCheckLeftTriggerFlags() {
    return plymIsSearchStartRequested() && (ctrlGetPadInputBuffer0(ctrlGetPadData())->pressedPriority & 0x400U) != 0;
}

// Both a refused destination and a QRON-owned redirect consume MEVE without constructing an EVFO callback.
// @verified - p
bool QueueMapEventTransition(const MapEventTransition &transition, std::int32_t eventId) {
    if (!RetainQronMapEventDestination(transition.areaId, eventId))
        return true;
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    SetPlayerControlEnabled(0);
    shenToggleAreaFlags(2);
    WaitForNextTask_0(control->screenFilmTask, 1, 31);
    HLib::EnqueueTaskWithoutParameter(Exec_MapEventSceneChange_Callback, 3, 2, static_cast<std::int32_t>(mapEventFadeTaskToken.name));
    g_newAreaId = transition.areaId;
    g_newEntryId = transition.entryId;
    g_newScenarioFlags = transition.scenarioFlags;
    spmFadeOut();
    return true;
}

// @verified - p
bool Exec_MapEventSceneChange_Enqueue(MapEventTaskParameters &parameters, std::int32_t eventId) {
    if (parameters.callbacks != mapEventTaskEntryData) {
        for (const MapEventCallbackEntry *entry = parameters.callbacks; entry->eventId != -1; ++entry) {
            if (entry->eventId != eventId)
                continue;
            if (!ryoNotBusy())
                return false;
            entry->callback(entry->argument);
            return true;
        }
    }

    const MapEventTransition *const transition = FindCurrentMapEventTransition(eventId);
    if (!transition)
        return true;
    return ryoNotBusy() && QueueMapEventTransition(*transition, eventId);
}

// @verified - p
void MEVW_Callback(Character *character, void *componentData) {
    auto *const state = static_cast<MapEventComponentState *>(componentData);
    if (!character || !state)
        return;
    const std::uint32_t region = flddTestMapEventIntersection(GetCharacterWorldPosition(character));
    const std::uint32_t currentRegion = static_cast<std::uint16_t>(region);
    if (currentRegion == state->currentRegion)
        return;
    if ((state->flags & 2U) != 0 || currentRegion == 255U) {
        state->flags &= ~1U;
    } else {
        state->flags |= 1U;
    }
    state->currentRegion = currentRegion;
    state->mode = region >> 16U;
}

// @verified - p
void VisitMapEventCharacter(Character *character, void *context) {
    auto *const state = static_cast<MapEventComponentState *>(FindModelComponent(character, mapEventComponentToken));
    auto *const parameters = static_cast<MapEventTaskParameters *>(context);
    if (!state || !parameters || ((state->flags & 1U) != 0 && state->mode != 4U)) {
        return;
    }

    const std::uint32_t region = state->currentRegion;
    if (region == 255U) {
        if (ryoNotBusy()) {
            // SetUnkOnNode writes CHAR +0x24 from MEVW +8.
            character->internalState = static_cast<std::int32_t>(state->mode);
            state->flags |= 1U;
        }
        return;
    }
    const std::uint8_t regionMask = static_cast<std::uint8_t>(1U << (region & 7U));
    if ((state->enabledRegions[region >> 3U] & regionMask) == 0)
        return;

    std::uint32_t eventId = region;
    bool markConsumed = true;
    switch (state->mode) {
    case 0:
    case 4:
        break;
    case 1:
        if (!plymPadCheckLeftTriggerFlags())
            return;
        markConsumed = false;
        break;
    case 2:
        state->enabledRegions[region >> 3U] &= static_cast<std::uint8_t>(~regionMask);
        break;
    case 3:
        if (!plymPadCheckLeftTriggerFlags())
            return;
        state->enabledRegions[state->currentRegion >> 3U] &= static_cast<std::uint8_t>(~(1U << (state->currentRegion & 7U)));
        eventId = state->currentRegion;
        break;
    default:
        return;
    }
    if (Exec_MapEventSceneChange_Enqueue(*parameters, eventId) && markConsumed) {
        state->flags |= 1U;
    }
}
struct CharacterMapEntry {
    CharacterMapEntry *next;
    Character *character;
    HLib::Task *handlerTask;
};

static_assert(offsetof(CharacterMapEntry, next) == 0x00);
static_assert(offsetof(CharacterMapEntry, character) == 0x08);
static_assert(offsetof(CharacterMapEntry, handlerTask) == 0x10);
static_assert(sizeof(CharacterMapEntry) == 0x18);

CharacterMapEntry *&CharacterBucket(MapControlData &control, std::size_t index) {
    return *reinterpret_cast<CharacterMapEntry **>(&control.characterTable[index]);
}

bool IsValidCEWP(const Character *character) {
    return character && character->entity.name.name == SHEN_NAME(CEWP) && (character->flags & CF_Valid) != 0;
}

MapControlData *ctrlIncrementCounter() {
    auto *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    ++control->pendingCharacterCleanupCount;
    return control;
}

std::int32_t gameSysUnusedAf0{};
std::uint32_t gameSysUnusedColour{};
std::uint32_t gameSysActivityFlags{};
std::int32_t inputSuppressionFrames{};
std::array<std::int32_t, 5> gameSysActivityCounters{};
bool gameSysStarted = false;
std::int32_t gameSystemNeedsReset = 0;

void MapControlTask_Callback(void *taskData) {
    MapControlData *const control = static_cast<MapControlData *>(taskData);
    if (control && control->pendingCharacterCleanupCount)
        CleanupMapControlCharacters();
}

} // namespace

// @verified - p
bool IsCharacterInCurrentMap(const Character *character) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (!character)
        return false;
    for (std::size_t bucket = 0; bucket < control->characterTable.size(); ++bucket) {
        for (CharacterMapEntry *entry = CharacterBucket(*control, bucket); entry; entry = entry->next) {
            if (IsValidCEWP(entry->character) && entry->character == character)
                return true;
        }
    }
    return false;
}

void KillCharactersByModelName(shen_name owner, std::uint32_t mode) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    for (std::size_t bucket = 0; bucket < control->characterTable.size(); ++bucket) {
        for (CharacterMapEntry *entry = CharacterBucket(*control, bucket); entry; entry = entry->next) {
            Character *const character = entry->character;
            if (IsValidCEWP(character) && GetCharacterModelName(character).name == owner.name)
                ctrlSetKillAndIncrementCounter(character, mode);
        }
    }
}

void CleanupMapControlCharacters() {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    for (std::size_t bucket = 0; bucket < control->characterTable.size(); ++bucket) {
        CharacterMapEntry **link = &CharacterBucket(*control, bucket);
        while (CharacterMapEntry *const entry = *link) {
            Character *const character = entry->character;
            if (IsValidCEWP(character)) {
                link = &entry->next;
                continue;
            }
            UnassignScenarioCharacter(control->scenarioTask, character);
            if (control->ryo == character)
                control->ryo = nullptr;
            FreeCharacter(character);
            --control->pendingCharacterCleanupCount;
        }
    }
}




void ReleaseMapControlOwners() {
    if (!g_mapControlTask)
        return;
    auto *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    ReleaseAreaEventTaskOwners();
    CleanupEventTaskQueue();
    FreeMotionRetirementTask();
    ShutdownSoundProgramControl();
    ReleaseItemWfOwners();
    Author_Free();
    ReleaseMagicRoomRuntime();
    FLDD_FreeFieldData();
    DestroyCharacterControlModelPairTask();
    if (control)
        ClearScenarioCharacterAssignments(control->scenarioTask);
    ReleaseNavigationTaskOwners();
    ReleasePawnTaskOwners();
    FreeNpcRegistryTask();
    ReleaseAdviceControlOwners();
    ReleaseMoveLearnedOwner();
    ReleaseElevatorRuntime();
    ResetDoorRuntime();
    ReleaseHandcuffsModels();
    ReleaseTelephoneCordHostResources();
    ReleaseLensflareHostResources();
    FreeCharacters();
    if (control)
        ReleaseModelImageManager(control->modelManagerTask);
    RetireCharacterMirrorStorage();
    ReleasePatiRuntime();
    FreeNpcPackages();
    ResetAirPresentation();
    FreeMapAssets();
    FreePakLoadTask();
    ResetNpcRouteLiveEntries();
    g_mapControlTask = nullptr;
}

namespace {

// @verified - p
void UpdateGameSysActivity() {
    const LegacyInputSlot &input = g_inputBuffer[0];
    std::uint32_t flags = gameSysActivityFlags;
    flags = (input.held & 0xf000U) != 0 ? flags | 1U : flags & ~1U;
    flags = (input.held & 0x40U) != 0 ? flags | 2U : flags & ~2U;
    flags = (input.held & 0x80U) != 0 ? flags | 4U : flags & ~4U;
    const bool stickMoved = static_cast<std::uint8_t>(input.analog0e - 126) > 4U || static_cast<std::uint8_t>(input.analog0f - 126) > 4U;
    gameSysActivityFlags = stickMoved ? flags | 8U : flags & ~8U;
    gameSysActivityFlags = camCtrlIsConversationActive() != 0 ? gameSysActivityFlags | 0x10U : gameSysActivityFlags & ~0x10U;
    if (GetParentMenu() != 0 && !IsItemSelectable() && gameSysActivityFlags != 0) {
        ++gameSysActivityCounters[4];
        for (std::size_t index = 0; index < 4; ++index)
            --gameSysActivityCounters[index];
    }
}

// @verified - p
void CloseAllLargeFileHandles() {
    for (d3t::LargeFileDataEntry &entry : d3t::g_largeFileData) {
        if (entry.inUse)
            d3t::ReadCachedFile(&entry.handle);
    }
}

void GameSys_Callback(void *) {
    if (--inputSuppressionFrames >= 0) {
        LegacyInputSlot &input = g_inputBuffer[0];
        input.analog0c = 0;
        input.analog0d = 0;
        input.analog0e = 0x80;
        input.analog0f = 0x80;
        input.held = 0;
        input.previousHeld = 0;
        input.pressedPriority = 0;
        input.repeatedPresses = 0;
        input.newPresses = 0;
    } else {
        inputSuppressionFrames = 0;
    }
    UpdateGameSysActivity();
    if (ctrlGetActive()) {
        const SystemModule module = UpdateCurrentSceneInfoFromTask(g_actNumber, g_currentMap, g_entryPoint);
        scenCvsCtrlFree();

        ReleaseMapControlOwners();
        CloseAllLargeFileHandles();
        HLib::MarkCurrentTaskForCleanup();
        const bool faded = GetIsWaitingFlag() && ((static_cast<std::uint32_t>(module) - 6U) & ~6U) == 0 && module != SystemModule::Credits;
        d3t::SetSceneClearColour(faded ? GetFadedClearColour() : 0xff000000U);
        HLib::SetCurrentTaskCustomUserData(nullsub, nullptr);
        ChangeSystemModule(module, 0);
    }
    IncrementSharedPrngFloat();
    IncrementFrameCount();
}

} // namespace
// @verified - p
std::int64_t SetAreaFogParams() {
    if (qronFloorRuntime.areaId.name == 0)
        return 0;
    const MapRenderProfile &profile = MatchMapRenderProfile(g_currentMap);
    SetFogRed(profile.farDistance * 0.875f);
    SetFogGreen(profile.farDistance);
    SetFogBlue(1.0f);
    SetFogDistance(0xff000000U);
    ReinitFogTasks(1);
    return 1;
}

bool SetQronFloorDescriptor(const QronFloorDescriptor *descriptor) {
    if (!qronFloorRuntime.areaId.name || (qronFloorRuntime.flags & 1U) == 0)
        return false;
    if (descriptor) {
        qronFloorRuntime.floorCallback = descriptor->callback;
        qronFloorRuntime.props = descriptor->props;
        BindQronFloorProps();
    }
    (void)RepositionPlayerForQronFloor(qronFloorRuntime.floor);
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Vector3 position = GetCharacterWorldPosition(static_cast<Character *>(control->ryo));
    position.y += 1.0f;
    const std::int32_t intersection = scenFindIntersection(position, position.y);
    if (intersection != 0 && static_cast<std::uint32_t>(intersection - 2) > 2U) {
        qronFloorRuntime.flags &= ~0x40U;
        RestoreQronUpperFloorPlacements();
    } else {
        qronFloorRuntime.flags |= 0x40U;
        EnableQronUpperFloorPlacements();
    }
    return true;
}

// @verified - p
std::uint64_t GetQronNextStream() noexcept {
    return static_cast<std::uint32_t>(qronFloorRuntime.areaId.name != 0 ? qronFloorRuntime.floor : -1000);
}

// @verified - p
bool SetQronMapLightingEnabled(std::int32_t enabled) noexcept {
    if (qronFloorRuntime.areaId.name == 0)
        return false;

    if ((qronFloorRuntime.flags & 1U) != 0) {
        if (GetMagicRoomFlag(31) == 0x7fffU)
            return false;
        SetMapLightingPlacementEnabled(enabled, 31);
        return true;
    }

    if (qronFloorRuntime.lightIndex == -1)
        return false;
    (void)SetMapModelSlotEnabled(qronFloorRuntime.lightIndex, enabled);
    return true;
}

std::int32_t ResolveQronEntryLocation(Vector3 &position, std::int32_t &yaw) noexcept {
    const QronMapDefinition *const definition = FindQronMapDefinition(g_currentMap);
    const bool savedLocation = GetSavedAreaJumpLocation(position, yaw);
    if (!definition) {
        if (!savedLocation && !MoveObjectToPosFromThisScene(SHEN_NAME(0000), g_entryPoint, position, yaw)) {
            const MapEventTransition *const transition = FindCurrentMapEventTransition(g_entryPoint);
            if (transition) {
                position = transition->position;
                yaw = transition->yaw;
            } else {
                position = {99999.898f, 99999.898f, 99999.898f};
            }
        }
        return -1000;
    }

    std::int32_t floor = qronEntryFloor;
    if (!savedLocation) {
        if (qronSceneChange.areaId.name != 0 && (qronSceneChange.mode == 2 || qronSceneChange.mode == 3)) {
            position = qronSceneChange.position;
            yaw = qronSceneChange.yaw;
        } else if (!MoveObjectToPosFromThisScene(SHEN_NAME(0000), g_entryPoint, position, yaw)) {
            const MapEventTransition *const transition = FindCurrentMapEventTransition(g_entryPoint);
            if (transition) {
                position = transition->position;
                yaw = transition->yaw;
            }
        }
        floor = qronSceneChange.areaId.name != 0 ? qronSceneChange.floor : QronFloorFromHeight(position.y, definition->baseHeight);
    } else if (floor == -1000) {
        floor = QronFloorFromHeight(position.y, definition->baseHeight);
    }

    floor = std::clamp(floor, definition->minimumFloor, definition->maximumFloor);
    const float floorHeight = QronFloorHeight(floor, definition->baseHeight);
    position.y = floorHeight;
    if (qronSceneChange.mode == 2 && (definition->flags & 0x20U) != 0)
        position.y += 3.5999999f;
    position.y += 3.5f;
    const float intersection = shenFindIntersectionWithScene(position, position.y);
    position.y = intersection == -std::numeric_limits<float>::max() ? floorHeight : intersection;
    return floor;
}

std::uint32_t GetQronCurrentFlags() noexcept {
    return qronFloorRuntime.areaId.name != 0 ? qronFloorRuntime.flags : 0;
}

std::int32_t GetQronFloorAtPosition(const Vector3 &position) noexcept {
    if (qronFloorRuntime.areaId.name == 0)
        return -1000;

    const auto floor = QronFloorFromHeight(position.y, qronFloorRuntime.baseHeight);
    return floor > qronFloorRuntime.maximumFloor || floor < qronFloorRuntime.minimumFloor ? -1000 : floor;
}

float GetQronFloorHeight(std::int32_t floor) noexcept {
    if (qronFloorRuntime.areaId.name == 0 || floor > qronFloorRuntime.maximumFloor || floor < qronFloorRuntime.minimumFloor) {
        return std::numeric_limits<float>::max();
    }
    return QronFloorHeight(floor, qronFloorRuntime.baseHeight);
}
// @verified - p
float GetQronTemporaryFloorHeight() noexcept {
    return qronFloorRuntime.areaId.name != 0 ? qronFloorRuntime.floorHeight : std::numeric_limits<float>::max();
}

float FindQronIntersectionHeight(const Vector3 &position, float maximumY, std::int32_t floor) {
    constexpr float noIntersection = -std::numeric_limits<float>::max();
    if (qronFloorRuntime.areaId.name == 0 || floor > qronFloorRuntime.maximumFloor || floor < qronFloorRuntime.minimumFloor) {
        return noIntersection;
    }

    if ((qronFloorRuntime.flags & 1U) == 0) {
        return shenFindIntersectionWithScene(position, maximumY);
    }

    const float floorHeight = GetQronFloorHeight(floor);
    Vector3 adjusted = position;
    adjusted.y += qronFloorRuntime.floorHeight - floorHeight;
    const float height = shenFindIntersectionWithScene(adjusted, floorHeight);
    return height == noIntersection ? noIntersection : (height + floorHeight) - qronFloorRuntime.floorHeight;
}

void SetQronEntryFloor(std::int32_t floor) noexcept {
    qronEntryFloor = floor;
}

// @verified - p
bool SetQronFloor(std::int32_t floor) {
    if (!qronFloorRuntime.areaId.name)
        return false;
    if (SetQronRelocatedFloor(floor))
        return true;
    return SetQronStackedFloor(floor);
}

// @verified - p
bool RepositionPlayerForQronFloor(std::int32_t floor) {
    if (!qronFloorRuntime.areaId.name)
        return false;
    if (!SetQronRelocatedFloor(floor))
        return SetQronStackedFloor(floor);
    MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Vector3 position = GetCharacterWorldPosition(static_cast<Character *>(control->ryo));
    control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const std::int16_t yaw = GetAllAngYForNode(&static_cast<Character *>(control->ryo)->entity.node);
    position.y = (qronFloorRuntime.floorHeight + 3.5999999f) - 0.1f;
    position.y = shenFindIntersectionWithScene(position, position.y);
    if (position.y == -std::numeric_limits<float>::max())
        position.y = qronFloorRuntime.floorHeight;
    control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    shenApplyTranslationToAll(&static_cast<Character *>(control->ryo)->entity.node, position);
    control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    shenDecrementRotYAllNodes(&static_cast<Character *>(control->ryo)->entity.node, yaw);
    return true;
}

// @verified - p
HLib::Task *QueueQronElevatorSceneChange(shen_name area, std::int32_t entry, std::int16_t floor, const Vector3 &position, std::int32_t yaw) {
    qronSceneChange = {.areaId = area, .entryId = entry, .mode = 3, .floor = floor, .yaw = yaw, .position = position};
    SetPlayerControlEnabled(0);
    SetScreenFilmFade(31, 0, 0, 0, 0, 255, 0, 0, 0);
    spmFadeOut();
    return HLib::EnqueueTaskWithoutParameter(QronSceneChange_Callback, 3, 2, static_cast<std::int32_t>(TASK_QRON.name));
}

void ResetQronSceneChange() noexcept {
    qronEntryFloor = -1000;
    qronSceneChange = {};
}

// @verified - p
const MapEventAreaDefinition *GetMapEventAreaDefinitions() {
    return activeMapEventAreas;
}

const MapEventAreaDefinition &GetCurrentMapEventAreaDefinition() {
    const MapEventAreaDefinition *area = activeMapEventAreas;
    while (area->areaId.name != 0xffffffffU && area->areaId.name != g_currentMap.name) {
        ++area;
    }
    return *area;
}

const MapEventTransition *FindCurrentMapEventTransition(std::int32_t eventId) {
    for (const MapEventAreaDefinition *area = activeMapEventAreas; area->areaId.name != 0xffffffffU; ++area) {
        if (area->areaId.name != g_currentMap.name)
            continue;

        for (const MapEventTransition *transition = area->transitions; transition && transition->eventId != -1; ++transition) {
            if (transition->eventId == eventId)
                return transition;
        }
    }
    return nullptr;
}

// @verified - p
bool AttachMapEventComponent(Character *character) {
    if (!character)
        return false;

    auto *const state =
        static_cast<MapEventComponentState *>(AllocateModelComponent(character, mapEventComponentToken, sizeof(MapEventComponentState), MEVW_Callback));
    if (!state)
        return false;

    state->flags |= 3U;
    state->currentRegion = 0;
    state->mode = 0;
    for (std::uint8_t &enabled : state->enabledRegions)
        enabled = 0xffU;
    state->enabledRegions[0] &= 0xfeU;
    return true;
}

// @verified - p
MapEventComponentState *ParseMapEv(Character *character, std::int32_t enabled) {
    MapEventComponentState *const state = static_cast<MapEventComponentState *>(FindModelComponent(character, mapEventComponentToken));
    if (enabled)
        state->flags |= 2U;
    else
        state->flags = (state->flags & ~2U) | 1U;
    return state;
}

// @verified - p
Character *ctrlGetRootNode() {
    return static_cast<Character *>(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo);
}

// @verified - p
std::uint8_t EnableScenarioCharacterDisplay(shen_name name) {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return EV_SetCharDisplay(FindScenarioCharacter(control->scenarioTask, name), CVM_DisplayEnabled, 1);
}

// @verified - p
std::uint8_t DisableScenarioCharacterDisplay(shen_name name) {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return EV_SetCharDisplay(FindScenarioCharacter(control->scenarioTask, name), CVM_DisplayEnabled, 0);
}

// @verified - p
void EnsureScenarioCharacterDraw(shen_name name) {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = FindScenarioCharacter(control->scenarioTask, name);
    if (!character)
        return;
    HLib::Task *const task = GetCharacterHandlerTask(character);
    charaSetFlags(character, 1);
    HLib::ResolveDeferredCallback(task);
}

// @verified - p
void ToggleScenarioCharacterFlag(shen_name name) {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = FindScenarioCharacter(control->scenarioTask, name);
    if (!character)
        return;
    HLib::Task *const task = GetCharacterHandlerTask(character);
    charaSetFlags(character, 0);
    HLib::SetupTimedTask(task, 0);
}

// @verified - p
MapEventComponentState *SetCharacterMapEventRegionEnabled(shen_name name, std::uint32_t region, std::uint8_t enabled) {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = FindScenarioCharacter(control->scenarioTask, name);
    if (!character)
        return nullptr;
    MapEventComponentState *const state = static_cast<MapEventComponentState *>(FindModelComponent(character, mapEventComponentToken));
    std::uint8_t *const byte = reinterpret_cast<std::uint8_t *>(state) + offsetof(MapEventComponentState, enabledRegions) + (region >> 3U);
    const std::uint8_t mask = static_cast<std::uint8_t>(1U << (region & 7U));
    if ((enabled & 1U) != 0)
        *byte |= mask;
    else
        *byte &= static_cast<std::uint8_t>(~mask);
    return state;
}

// @verified - p
std::uint64_t IsCharacterMapEventRegionEnabled(shen_name name, std::uint32_t region) {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = FindScenarioCharacter(control->scenarioTask, name);
    if (!character)
        return 0;
    const MapEventComponentState *const state = static_cast<const MapEventComponentState *>(FindModelComponent(character, mapEventComponentToken));
    const std::uint8_t *const byte = reinterpret_cast<const std::uint8_t *>(state) + offsetof(MapEventComponentState, enabledRegions) + (region >> 3U);
    return (*byte & static_cast<std::uint8_t>(1U << (region & 7U))) != 0;
}

// @verified - p
std::uint64_t GetCharacterMapEventRegion(shen_name name) {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = FindScenarioCharacter(control->scenarioTask, name);
    if (!character)
        return 0;
    const MapEventComponentState *const state = static_cast<const MapEventComponentState *>(FindModelComponent(character, mapEventComponentToken));
    return state->currentRegion;
}

// @verified - p
void MapEventTaskEntry_Callback(void *taskData) {
    ForEachLiveCharacterInMapOrder(VisitMapEventCharacter, taskData);
}

// @verified - p
HLib::Task *Queue_Callback_MapEventTaskEntry(std::uint8_t nextFunctionIndex) {
    HLib::Task *const task =
        HLib::EnqueueTaskWithParameter(MapEventTaskEntry_Callback, nextFunctionIndex, 2, sizeof(MapEventTaskParameters), mapEventTaskToken);
    if (!task)
        return nullptr;

    if (auto *const parameters = static_cast<MapEventTaskParameters *>(HLib::GetTaskParameter(task))) {
        parameters->callbacks = mapEventTaskEntryData;
    }
    return task;
}


// @verified - p
void SetMapEventCallbackTable(const MapEventCallbackEntry *callbacks) {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    static_cast<MapEventTaskParameters *>(HLib::GetTaskParameter(control->mapEventTask))->callbacks = callbacks;
}

// @verified - p
void Exec_MapEventSceneChange_Callback(void *) {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (DoesScreenFilmColourExist(control->screenFilmTask))
        return;
    const SystemModule module = static_cast<std::int32_t>(g_newAreaId.name) < 0 ? SystemModule::Credits : SystemModule::Game;
    if (g_newScenarioFlags)
        scenSetTaskFlags(static_cast<std::uint8_t>(g_newScenarioFlags));
    shenQueueChangeModuleAndScene(module, g_actNumber, g_newAreaId, g_newEntryId);
    HLib::DestroyCurrentTask();
}

void ctrlSetKillAndIncrementCounter(Character *character, std::uint32_t mode) {
    if (!character)
        return;
    if (mode == 0) {
        character->flags |= CF_Unknown02;
    }
    character->flags &= ~CF_Valid;
    ctrlIncrementCounter();
}

// @verified - p
void SetDirtAngle(std::int32_t dirtAngle) {
    static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->sunLocation = dirtAngle;
}



void EventBase_Wrapper(void *mapInfo) {
    if (const MapInfoChunk *const dirt = FindMapInfoChunk("DIRT", mapInfo)) {
        std::int32_t degrees;
        std::memcpy(&degrees, reinterpret_cast<const std::byte *>(dirt) + 8, sizeof(degrees));
        SetDirtAngle(static_cast<std::uint16_t>(static_cast<std::int32_t>((static_cast<float>(degrees) * 8192.0f) / 45.0f)));
    }
    SetNewScenarioTask(reinterpret_cast<const std::byte *>(FindMapInfoChunk("SCNF", mapInfo)));
    QueueAreaEventEntry();
    if (MapInfoChunk *const regd = FindMapInfoChunk("REGD", mapInfo)) {
        (void)HLib::ShrinkTaskData(mapInfo, static_cast<std::uint64_t>(reinterpret_cast<std::byte *>(regd) - static_cast<std::byte *>(mapInfo)) + 8U);
        const std::uint64_t terminator = make_shen_name('E', 'N', 'D', '\0');
        std::memcpy(regd, &terminator, sizeof(terminator));
    }
}

std::uint32_t g_fockEnabled{};

// @verified - p
void FOCK_Reader(const void *chunk) {
    std::uint32_t flags = 1;
    if (chunk)
        std::memcpy(&flags, static_cast<const std::byte *>(chunk) + 8, sizeof(flags));
    g_fockEnabled = flags & 1U;
}

void MapControlTask_Enqueue(std::uint8_t nextFunctionIndex) {
    if (ReturnZero() == 0)
        SetupMapRendererCaches();
    SetDebugHideCameraIcon(1);
    nullsub(g_currentMap.name);
    ReloadSpriteResources();
    g_facialFocusTarget = {};
    g_facialFocusId = facialFocusNone;
    ResetDirectoryCache(0);
    char mapDirectory[64];
    std::snprintf(mapDirectory, sizeof(mapDirectory), "/scene/maps/%.4s", g_currentMap.characters);
    AllocateDIRE(mapDirectory, 0, 30);
    SetupRenderBufferHandles();
    SkyDomeProbe(mapDirectory);
    LoadHandModels();
    g_mapControlTask = HLib::EnqueueTaskWithParameter(MapControlTask_Callback, nextFunctionIndex, 2, sizeof(MapControlData), TASK_CTRL);
    MapControlData *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    control->mapEventTask = nullptr;
    control->scenarioTask = nullptr;
    control->cameraControlTask = nullptr;
    control->objectTask = nullptr;
    control->airTask = nullptr;
    control->modelManagerTask = nullptr;
    control->screenFilmTask = nullptr;
    control->characterDisplayTask = nullptr;
    control->characterDrawTask = nullptr;
    control->windowTask = nullptr;
    control->unknown60 = nullptr;
    control->unknown68 = nullptr;
    control->mapInfo = SceneMapinfoReader(g_actNumber, g_currentMap);
    Queue_PrePadControlTask(2);
    Queue_PadControlTask(2);
    Queue_btnButtonMediationCtrl(2);
    plymSetDefaultFlags();
    CVS_CtrlConversTask_Enqueue(true);
    PakLoadTaskEntry_Enqueue(3, 0);
    EnqueueNpcRegistryTask(8);
    if (!g_bPaused)
        ExecPause();
    control->characterTable.fill(nullptr);
    control->pendingCharacterCleanupCount = 0;
    control->ryo = nullptr;
    control->flags = 0;
    control->statusFlags = 0;
    static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->sunLocation = 0;
    control->characterControlTable.fill({});
    std::memset(control->mapRuntimeData, 0, sizeof(control->mapRuntimeData));
    control->currentCharacter = -1;
    control->field78c = -1;
    control->field794 = -1;
    control->field79c = -1;
    QueueEasyOptionTask();
    control->padControl = AllocatePadControlEntry();
    BindPadControlEntry(static_cast<PadControlEntry *>(control->padControl));
    control->windowTask = QueueWindowTask();
    SetupDefaultFogParams();
    npcInit();
    resetHandcuffTask();
    telCodeInit();
    MAPINFORead(control->mapInfo);
    QueueSoundProgramControl();
    if (!SNDP_SoundProgramReader(FindMapInfoChunk("SNDP", control->mapInfo), g_actNumber, g_currentMap))
        SNDD_Reader(FindMapInfoChunk("SNDD", control->mapInfo), g_actNumber, g_entryPoint);
    LoadShadowSprite();
    SM_Init_Enqueue(0);
    KanjiFontLoad("DC_KANJI.FON");
    KanaFontLoad("DC_KANA.FON");
    control->mapEventTask = Queue_Callback_MapEventTaskEntry(3);
    control->scenarioTask = ScenarioTaskEntry_Enqueue(3);
    control->modelManagerTask = QueueModelManagerTask(3);
    control->cameraControlTask = camCtrlEnqueue(7);
    control->screenFilmTask = Queue_SCF_Callback_ScreenFilmTaskEntry(13);
    WRN_CtrlWarning_Enqueue();
    QueueAdviceControl();
    GUI_Tasks_Enqueue();
    InitTIMR();
    InitST_TitleFunc();
    const MapInfoChunk *const fieldData = FindMapInfoChunk("FLDD", control->mapInfo);
    FLDD_ReadFieldData(fieldData, fieldData ? fieldData->nextByteOffset : 0);
    if (!g_bPaused)
        ST_LoadTitleFadeIn();
    SetLoadingDrawFlags(1);
    AllocateMotionBanks();
    if (const MapInfoChunk *const msts = FindMapInfoChunk("MSTS", control->mapInfo))
        std::memcpy(&control->statusFlags, reinterpret_cast<const std::byte *>(msts) + 8, sizeof(control->statusFlags));
    MSTS_Reader(0, 0, 0);
    TXMO_Reader(FindMapInfoChunk("TXMO", control->mapInfo), g_entryPoint);
    FOCK_Reader(FindMapInfoChunk("FOCK", control->mapInfo));
    LightTask_Enqueue();
    MVTR_Reader();
    SHRG_Reader(FindMapInfoChunk("SHRG", control->mapInfo));
    SetupRefboard();
    initTimeProgram();
    initCtrlSystemManage();
    initCtrlAreaJump();
    QueueLcdmTask();
    InitializeSearchControlTask();
    authResetLoadFlag();
    SCEX_Reader(FindMapInfoChunk("SCEX", control->mapInfo));
    control->unknown78 = nullptr;
    Mapinfo_MAP_Reader(g_actNumber, g_currentMap, g_entryPoint, control->mapInfo);
    InitializeDoorRuntime(FindMapInfoChunk("DOOR", control->mapInfo));
    tree_Callback_Enqueue(8);
    const std::int32_t entryPoint = g_entryPoint;
    CHRS_Reader(FindMapInfoChunk("CHRD", static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->mapInfo), entryPoint, SHEN_NAME(DRHC));
    QueueWeatherExecution(FindMapInfoChunk("WTHR", control->mapInfo));
    blurInit();
    fightInit(0);
    WaitForNextTask_0(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->screenFilmTask, 1, 0);
    if (FindMapInfoChunk("EVFD", control->mapInfo))
        Queue_EI_Startup_EventFade1();
    EventBase_Wrapper(control->mapInfo);
    SetUnusedFlag();
    InitializeMazeRuntime(g_actNumber, g_currentMap);
    control->objectTask = QueueObjectDrawTask(13);
    control->airTask = QueueAirDrawTask(13);
    control->characterDisplayTask = HLib::EnqueueTaskWithoutParameter(CharDispCheck_Callback, 13, 2, static_cast<std::int32_t>(TASK_CLIP.name));
    control->characterDrawTask = CharDraw_Human_Object_Enqueue(13);
    QueueCharacterControlModelPairTask(12);
    InitializeEffectControl();
    EnqueueEffectControlTask();
    InitializeEffectPoints();
    if (!g_bPaused)
        ST_LoadTitleFade();
    QueueMotionRetirementTask(13);
    SetLegacyVertexColourType(1);
    Queue_FLAG_CheckTask();
    QueueQronFloorControl();
    QueueElevatorTasks();
    InitializeMagicRoom();
    InitVFM();
    PublishNpcRegistryTask();
    RetireLoadingDraw();
}

void shenQueueChangeModuleAndScene(SystemModule module, std::int32_t actNumber, shen_name areaId, std::int32_t entryId) {
    auto *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (!control)
        return;
    SetRequestModuleChange(1);



    control->module = module;
    control->actNumber = actNumber;
    control->areaId = areaId;
    control->entryId = entryId;
    control->flags |= 1U;
    const bool faded = GetIsWaitingFlag() && ((static_cast<std::uint32_t>(module) - 6U) & ~5U) == 0 && module != SystemModule::Sample;
    const std::uint32_t colour = faded ? GetFadedClearColour() : 0xff000000U;
    d3t::SetSceneClearColour(colour);
    SetClearColourForClouds(colour);
    SetIsSkySetup(0);
}

// @verified - p
std::int32_t GetGameSysActivityCounter(std::int32_t index) {
    return index >= 5 ? 0 : gameSysActivityCounters[static_cast<std::size_t>(index)];
}

// @verified - p
void SetGameSysActivityCounter(std::int32_t index, std::int32_t value) {
    if (index < 5)
        gameSysActivityCounters[static_cast<std::size_t>(index)] = value;
}

// @verified - p
SystemModule UpdateCurrentSceneInfoFromTask(std::int32_t &actNumber, shen_name &areaId, std::int32_t &entryId) {
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    actNumber = control->actNumber;
    areaId = control->areaId;
    entryId = control->entryId;
    return control->module;
}

// @verified - p
PadControlEntry *ctrlGetPadData() {
    return static_cast<PadControlEntry *>(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->padControl);
}

// @verified - p
std::int32_t ctrlGetActive() {
    return static_cast<std::int32_t>(static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->flags & 1U);
}

std::int32_t GetShenmue1SaveAreaId(shen_name areaId) noexcept {
    for (const Shenmue1SaveAreaMapping &mapping : shenmue1SaveAreaMappings) {
        if (mapping.mapId == areaId.name || mapping.mapId == 0) {
            return mapping.saveAreaId;
        }
    }
    return -1;
}
// @verified - p
void gameSysInit() {
    gameSystemNeedsReset = 0;
}

// @verified - p
std::uint64_t gameSysGetNeedsReset() {
    return static_cast<std::uint32_t>(gameSystemNeedsReset);
}

// @verified - p
void RequestGameSystemReset() {
    gameSystemNeedsReset = 1;
}

void GameSys(void *) {
    DispatchPostFxFilterRender(1);
    SetSnapshotDataOption(0);
    g_currentShenmue1SaveAreaId = GetShenmue1SaveAreaId(g_currentMap);
    gameSysActivityFlags = 0;
    gameSysActivityCounters[0] = 0;
    gameSysActivityCounters[1] = 0;
    gameSysActivityCounters[2] = 0;
    gameSysActivityCounters[3] = 0;
    gameSysUnusedAf0 = 0;
    inputSuppressionFrames = 0;
    gameSysUnusedColour = 0x00ffffffU;
    g_worldDrawingEnabled = true;
    gameSysStarted = true;
    ResetSceneArchiveFileHandles();
    InitSceneEffects();
    MapControlTask_Enqueue(2);
    Queue_KeyRepeat(0);
    Queue_KeyRepeat(0);
    InitializeFrameVars();
    Queue_SNAP_Control();
    if (gameSysGetNeedsReset()) {
        if (srchGetCurrentFollowCharacter().name) {
            SetScenarioFollowCharacter({});
            ReleaseNpcScriptRouteOwner();
        }
        gameSysInit();
    }
    InitEventTaskQueue();
    HLib::SetCurrentTaskTo(GameSys_Callback);
    SetRequestModuleChange(0);
}

bool GameSysWasStarted() {
    return gameSysStarted;
}

bool IsWorldDrawingEnabled() {
    return g_worldDrawingEnabled;
}

bool GameMapWasLoaded() {
    const auto *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return gameSysStarted && control && control->mapInfo;
}

shen_name GetAreaDefinitionMapNameForCurrentMap() {
    switch (g_currentMap.name) {
    case SHEN_NAME(AR09):
        return SHEN_NAME(AR03);
    case SHEN_NAME(AKT1):
        return SHEN_NAME(TBAK);
    case SHEN_NAME(Q109):
        return SHEN_NAME(Q100);
    case SHEN_NAME(QE09):
        return SHEN_NAME(QE00);
    case SHEN_NAME(AK09):
        return SHEN_NAME(AK00);
    case SHEN_NAME(WK09):
        return SHEN_NAME(WK00);
    case SHEN_NAME(QR09):
        return SHEN_NAME(QR00);
    case SHEN_NAME(WS09):
        return SHEN_NAME(WS00);
    case SHEN_NAME(QT09):
        return SHEN_NAME(QTB3);
    case SHEN_NAME(KWQA):
        return SHEN_NAME(KWQW);
    default:
        return g_currentMap;
    }
}

} // namespace shenmue::s2
