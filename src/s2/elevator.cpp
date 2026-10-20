#include "s2/elevator.hpp"

#include "d3t/object_store.hpp"
#include "s2/camera.hpp"
#include "s2/door.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/mt7.hpp"
#include "s2/model_component.hpp"
#include "s2/search.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"
#include "s2/sound_program.hpp"

#include <array>
#include <bit>
#include <cfloat>
#include <cstdio>
#include <cstring>
#include <limits>

namespace shenmue::s2 {
namespace {

constexpr std::int16_t MinimumFloor = -15;
constexpr std::int16_t MaximumFloor = 48;
constexpr std::int32_t MissingFloor = -1000;
constexpr std::size_t MaximumElevators = 8;
constexpr std::size_t SourceCount = 3;

constexpr float FloatBits(std::uint32_t bits) noexcept {
    return std::bit_cast<float>(bits);
}

std::array<ElevatorResourceRecord, 5> qfElevators{{
    {.modelName = SHEN_NAME(R599),
        .currentFloor = 0,
        .yaw = -16384,
        .position = {FloatBits(0x43798ccd), FloatBits(0x42c80000), FloatBits(0x42ed999a)},
        .validFloorMasks = {0x8000, 0x0000, 0x0002, 0x0000},
        .specialFloor = 49,
        .flags = 0x1008},
    {.modelName = SHEN_NAME(R598),
        .currentFloor = -4,
        .yaw = -16384,
        .position = {FloatBits(0x43798ccd), FloatBits(0x42c80000), FloatBits(0x42f4cccd)},
        .validFloorMasks = {0x0800, 0x0000, 0x0000, 0x0080},
        .specialFloor = 49,
        .flags = 0x1008},
    {.modelName = SHEN_NAME(R597),
        .currentFloor = 0,
        .yaw = -16384,
        .position = {FloatBits(0x43798ccd), FloatBits(0x42c80000), FloatBits(0x42fc0000)},
        .validFloorMasks = {0xa000, 0x0000, 0x0000, 0x0000},
        .specialFloor = 49,
        .flags = 0x1000},
    {.modelName = SHEN_NAME(R596),
        .currentFloor = 17,
        .yaw = -32768,
        .position = {FloatBits(0x435c8000), FloatBits(0x42c80000), FloatBits(0x4334428f)},
        .validFloorMasks = {0x0000, 0x0080, 0x0001, 0x0000},
        .specialFloor = 49,
        .flags = 0x0000},
    {.modelName = SHEN_NAME(R595),
        .currentFloor = 1,
        .yaw = 16384,
        .position = {FloatBits(0x436c7333), FloatBits(0x42c80000), FloatBits(0x43276666)},
        .validFloorMasks = {0x0000, 0x0000, 0x0000, 0x0000},
        .specialFloor = 49,
        .flags = 0x0400},
}};

std::array<ElevatorResourceRecord, 4> qaElevators{{
    {.modelName = SHEN_NAME(R599),
        .currentFloor = 1,
        .yaw = -32768,
        .position = {FloatBits(0x438f8ccd), FloatBits(0x42c80000), FloatBits(0x43232666)},
        .validFloorMasks = {0x0000, 0x0201, 0x0000, 0x0000},
        .specialFloor = 49,
        .flags = 0x0100},
    {.modelName = SHEN_NAME(R598),
        .currentFloor = 1,
        .yaw = -16384,
        .position = {FloatBits(0x43879333), FloatBits(0x42c80000), FloatBits(0x434b6666)},
        .validFloorMasks = {0x0000, 0x0175, 0x0000, 0x0000},
        .specialFloor = 49,
        .flags = 0x0100},
    {.modelName = SHEN_NAME(R597),
        .currentFloor = 3,
        .yaw = 16384,
        .position = {FloatBits(0x4356d99a), FloatBits(0x42c80000), FloatBits(0x4359cccd)},
        .validFloorMasks = {0x0000, 0x047c, 0x0000, 0x0000},
        .specialFloor = 11,
        .flags = 0x0000},
    {.modelName = SHEN_NAME(R596),
        .currentFloor = 1,
        .yaw = 16384,
        .position = {FloatBits(0x4356d99a), FloatBits(0x42c80000), FloatBits(0x435d6666)},
        .validFloorMasks = {0x0000, 0x03c7, 0x0000, 0x0000},
        .specialFloor = 49,
        .flags = 0x0000},
}};

std::array<ElevatorResourceRecord, 1> qjbtElevators{{
    {.modelName = SHEN_NAME(R595),
        .currentFloor = 1,
        .yaw = -16384,
        .position = {FloatBits(0x438cf99a), FloatBits(0x42c80000), FloatBits(0x435c8000)},
        .validFloorMasks = {0x8000, 0x0008, 0x0000, 0x0000},
        .specialFloor = 49,
        .flags = 0x0000},
}};

std::array<ElevatorResourceRecord, 1> qcElevators{{
    {.modelName = SHEN_NAME(R599),
        .currentFloor = 1,
        .yaw = 0,
        .position = {FloatBits(0x4333199a), FloatBits(0x42c80000), FloatBits(0x42fb8000)},
        .validFloorMasks = {0x8000, 0x0924, 0x0000, 0x0000},
        .specialFloor = 49,
        .flags = 0x0000},
}};

std::array<ElevatorResourceRecord, 1> qdElevators{{
    {.modelName = SHEN_NAME(R598),
        .currentFloor = 1,
        .yaw = 0,
        .position = {FloatBits(0x432be666), FloatBits(0x42c80000), FloatBits(0x428f8000)},
        .validFloorMasks = {0x0000, 0x0005, 0x0000, 0x0000},
        .specialFloor = 49,
        .flags = 0x0100},
}};

std::array<ElevatorResourceRecord, 5> qeElevators{{
    {.modelName = SHEN_NAME(R592),
        .currentFloor = 1,
        .yaw = -16384,
        .position = {FloatBits(0x43806000), FloatBits(0x42c80000), FloatBits(0x42a20000)},
        .validFloorMasks = {0x0000, 0x1555, 0x0000, 0x0000},
        .specialFloor = 49,
        .flags = 0x0000},
    {.modelName = SHEN_NAME(R593),
        .currentFloor = 1,
        .yaw = -32768,
        .position = {FloatBits(0x43924000), FloatBits(0x42c80000), FloatBits(0x42cdb333)},
        .validFloorMasks = {0x0000, 0x03ff, 0x0000, 0x0000},
        .specialFloor = 49,
        .flags = 0x0000},
    {.modelName = SHEN_NAME(R594),
        .currentFloor = 1,
        .yaw = -32768,
        .position = {FloatBits(0x43940ccd), FloatBits(0x42c80000), FloatBits(0x42cdb333)},
        .validFloorMasks = {0x0000, 0x02ab, 0x0000, 0x0000},
        .specialFloor = 49,
        .flags = 0x0000},
    {.modelName = SHEN_NAME(R595),
        .currentFloor = 1,
        .yaw = -32768,
        .position = {FloatBits(0x4395d99a), FloatBits(0x42c80000), FloatBits(0x42cdb333)},
        .validFloorMasks = {0x0000, 0x0155, 0x0000, 0x0000},
        .specialFloor = 49,
        .flags = 0x0000},
    {.modelName = SHEN_NAME(R596),
        .currentFloor = 1,
        .yaw = -32768,
        .position = {FloatBits(0x4397a666), FloatBits(0x42c80000), FloatBits(0x42cdb333)},
        .validFloorMasks = {0x0000, 0x0211, 0x0000, 0x0000},
        .specialFloor = 49,
        .flags = 0x0000},
}};

struct ElevatorTableView {
    ElevatorResourceRecord *records;
    std::size_t count;
};

const std::array<ElevatorTableView, 6> permanentTables{{
    {.records = qaElevators.data(), .count = qaElevators.size()},
    {.records = qjbtElevators.data(), .count = qjbtElevators.size()},
    {.records = qcElevators.data(), .count = qcElevators.size()},
    {.records = qdElevators.data(), .count = qdElevators.size()},
    {.records = qeElevators.data(), .count = qeElevators.size()},
    {.records = qfElevators.data(), .count = qfElevators.size()},
}};

static_assert(4 + 1 + 1 + 1 + 5 + 5 + 1 == ElevatorPersistentStateSize);

struct ElevatorMapAlias {
    shen_name map;
    ElevatorResourceRecord *records;
    std::size_t count;
};

const std::array<ElevatorMapAlias, 22> mapAliases{{
    {.map = SHEN_NAME(QA00), .records = qaElevators.data(), .count = qaElevators.size()},
    {.map = SHEN_NAME(QA11), .records = qaElevators.data(), .count = qaElevators.size()},
    {.map = SHEN_NAME(QAE1), .records = qaElevators.data(), .count = qaElevators.size()},
    {.map = SHEN_NAME(QAE6), .records = qaElevators.data(), .count = qaElevators.size()},
    {.map = SHEN_NAME(QAW1), .records = qaElevators.data(), .count = qaElevators.size()},
    {.map = SHEN_NAME(QAW6), .records = qaElevators.data(), .count = qaElevators.size()},
    {.map = SHEN_NAME(QC00), .records = qcElevators.data(), .count = qcElevators.size()},
    {.map = SHEN_NAME(QC01), .records = qcElevators.data(), .count = qcElevators.size()},
    {.map = SHEN_NAME(QC06), .records = qcElevators.data(), .count = qcElevators.size()},
    {.map = SHEN_NAME(QD00), .records = qdElevators.data(), .count = qdElevators.size()},
    {.map = SHEN_NAME(QD01), .records = qdElevators.data(), .count = qdElevators.size()},
    {.map = SHEN_NAME(QE00), .records = qeElevators.data(), .count = qeElevators.size()},
    {.map = SHEN_NAME(QE01), .records = qeElevators.data(), .count = qeElevators.size()},
    {.map = SHEN_NAME(QE06), .records = qeElevators.data(), .count = qeElevators.size()},
    {.map = SHEN_NAME(QF00), .records = qfElevators.data(), .count = qfElevators.size()},
    {.map = SHEN_NAME(QF01), .records = qfElevators.data(), .count = qfElevators.size()},
    {.map = SHEN_NAME(QF02), .records = qfElevators.data(), .count = qfElevators.size()},
    {.map = SHEN_NAME(QF06), .records = qfElevators.data(), .count = qfElevators.size()},
    {.map = SHEN_NAME(QF39), .records = qfElevators.data(), .count = qfElevators.size()},
    {.map = SHEN_NAME(QJBT), .records = qjbtElevators.data(), .count = qjbtElevators.size()},
    {.map = SHEN_NAME(QKBT), .records = qfElevators.data(), .count = qfElevators.size()},
    {.map = SHEN_NAME(QUG0), .records = qfElevators.data(), .count = qfElevators.size()},
}};

std::array<ElevatorModelSource, SourceCount> modelDescriptors = InitialElevatorModelSources;

struct SharedElevatorModel {
    MT7Model model{};
    std::byte *allocation{};
    std::size_t allocationSize{};
};

std::array<SharedElevatorModel, SourceCount> sharedModels{};
std::array<HLib::Task *, MaximumElevators> elevatorTasks{};
std::array<ElevatorTaskPayload *, MaximumElevators> elevatorRegistry{};
std::size_t elevatorCount{};

ElevatorTableView FindMapTable(shen_name map) noexcept {
    for (const ElevatorMapAlias &alias : mapAliases) {
        if (alias.map.name == map.name)
            return {.records = alias.records, .count = alias.count};
    }
    return {.records = nullptr, .count = 0};
}

ElevatorTaskPayload *FindElevator(shen_name elevator) noexcept {
    for (std::size_t index = 0; index < elevatorRegistry.size(); ++index) {
        ElevatorTaskPayload *const payload = elevatorRegistry[index];
        if (payload && payload->resource->modelName.name == elevator.name)
            return payload;
    }
    return nullptr;
}

void PublishElevatorTransforms(ElevatorTaskPayload &payload) noexcept {
    Vector3 position = payload.resource->position;
    const float temporaryY = GetQronTemporaryFloorHeight();
    if (temporaryY != FLT_MAX)
        position.y = temporaryY;

    for (const ElevatorModelSource &descriptor : modelDescriptors) {
        if (descriptor.prefixToken == 0)
            continue;
        const std::uint16_t modelPrefix = static_cast<std::uint16_t>(payload.resource->modelName.name);
        const shen_name entityId{payload.resource->modelName.name ^ static_cast<std::uint16_t>(descriptor.prefixToken ^ modelPrefix)};
        SearchTarget *const entity = shenFindFlagTarget(entityId);
        if (!entity)
            continue;

        if ((payload.flags & 0x8800U) != 0)
            entity->flags |= 2U;
        else
            entity->flags &= ~2U;

        Vector3 local = descriptor.translation;
        local.z += 0.029999999f;
        Matrix4x4_PushActiveTransform();
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(payload.resource->yaw));
        Matrix4x4::preTranslate(local.x, local.y, local.z);
        entity->position = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        Matrix4x4::Pop(1);

        entity->lockonAngle = descriptor.lockonAngle;
        entity->yaw = static_cast<std::int16_t>(payload.resource->yaw + descriptor.yaw);
        entity->yawLimit = static_cast<std::uint16_t>(static_cast<std::int32_t>(descriptor.yawLimit));
    }
}

bool GetFloorBit(const ElevatorResourceRecord &resource, std::int16_t floor, std::size_t &word, std::uint16_t &mask) noexcept {
    const std::uint16_t shifted = static_cast<std::uint16_t>(static_cast<std::int32_t>(floor) + 15);
    if (shifted > 63)
        return false;
    word = shifted / 16;
    mask = static_cast<std::uint16_t>(1U << (shifted % 16));
    return (resource.validFloorMasks[word] & mask) != 0;
}

bool IsFloorRequested(const ElevatorTaskPayload &payload, std::int16_t floor, std::uint32_t bank) noexcept {
    std::size_t word = 0;
    std::uint16_t mask = 0;
    return GetFloorBit(*payload.resource, floor, word, mask) && (payload.floorRequests[bank][word] & mask) != 0;
}

std::int32_t SetFloorRequest(ElevatorTaskPayload &payload, std::int16_t floor, std::uint32_t bank, bool enabled) noexcept {
    std::size_t word = 0;
    std::uint16_t mask = 0;
    if (!GetFloorBit(*payload.resource, floor, word, mask))
        return 0;
    if (enabled)
        payload.floorRequests[bank][word] |= mask;
    else
        payload.floorRequests[bank][word] &= static_cast<std::uint16_t>(~mask);
    return 1;
}

void SelectElevatorDirection(ElevatorTaskPayload &payload) noexcept {
    const std::int16_t current = payload.currentFloor;
    std::int16_t above = MissingFloor;
    for (std::int16_t floor = MaximumFloor; floor >= current; --floor) {
        if (IsFloorRequested(payload, floor, 1) || (floor != current && (IsFloorRequested(payload, floor, 2) || IsFloorRequested(payload, floor, 0)))) {
            above = floor;
        }
        if (floor == (std::numeric_limits<std::int16_t>::min)())
            break;
    }

    std::int16_t below = MissingFloor;
    for (std::int16_t floor = MinimumFloor; floor <= current; ++floor) {
        if (IsFloorRequested(payload, floor, 2) || (floor != current && (IsFloorRequested(payload, floor, 1) || IsFloorRequested(payload, floor, 0)))) {
            below = floor;
        }
        if (floor == (std::numeric_limits<std::int16_t>::max)())
            break;
    }

    const bool hasAbove = above != MissingFloor;
    const bool hasBelow = below != MissingFloor;
    if (payload.direction == 0) {
        if (hasAbove && hasBelow)
            payload.direction = current - below < above - current ? 2 : 1;
        else if (hasAbove)
            payload.direction = 1;
        else if (hasBelow)
            payload.direction = 2;
        return;
    }
    if (payload.direction == 1) {
        if (!hasAbove && hasBelow)
            payload.direction = 2;
        else if (!hasAbove && !hasBelow)
            payload.direction = 0;
        return;
    }
    if (payload.direction == 2) {
        if (hasAbove && !hasBelow)
            payload.direction = 1;
        else if (!hasAbove && !hasBelow)
            payload.direction = 0;
    }
}

void StepElevatorFloor(ElevatorTaskPayload &payload) noexcept {
    if ((payload.flags & 0x0001U) != 0)
        return;
    const std::int16_t threshold = (payload.flags & 0x0008U) != 0 ? 20 : 30;
    if (payload.direction == 1) {
        ++payload.floorStepTicks;
        if (payload.floorStepTicks > threshold) {
            ++payload.currentFloor;
            payload.floorStepTicks = 0;
            payload.resource->currentFloor = payload.currentFloor;
        }
    } else if (payload.direction == 2) {
        ++payload.floorStepTicks;
        if (payload.floorStepTicks > threshold) {
            --payload.currentFloor;
            payload.floorStepTicks = 0;
            payload.resource->currentFloor = payload.currentFloor;
        }
    }
}

void ClearCurrentFloorRequests(ElevatorTaskPayload &payload) noexcept {
    (void)SetFloorRequest(payload, payload.currentFloor, 1, false);
    (void)SetFloorRequest(payload, payload.currentFloor, 2, false);
    (void)SetFloorRequest(payload, payload.currentFloor, 0, false);
}

std::int32_t UpdateOpeningDoor(ElevatorTaskPayload &payload, float soundContext) noexcept {
    if (payload.doorFrame >= 60)
        return 1;
    if (payload.doorFrame >= 1) {
        ++payload.doorFrame;
        return 0;
    }
    (void)doorPlayRigidMotion(payload.resource->modelName, 0xf0d0U, 0.0f, 1.0f);
    (void)Loop3DSoundEffect(0x4b6U, soundContext);
    (void)Loop3DSoundEffect(0x4d3U, soundContext);
    (void)Loop3DSoundEffect(0xb41U, soundContext);
    ++payload.doorFrame;
    return 0;
}

std::int16_t UpdateElevatorDoor(ElevatorTaskPayload &payload, float soundContext) noexcept {
    ClearCurrentFloorRequests(payload);
    if ((payload.flags & 0x0002U) != 0)
        return static_cast<std::int16_t>(UpdateOpeningDoor(payload, soundContext));
    if (payload.doorFrame >= 60) {
        (void)doorPlayRigidMotion(payload.resource->modelName, 0xf0cfU, 0.0f, 1.0f);
        (void)Loop3DSoundEffect(0x4b6U, soundContext);
        const std::int16_t result = static_cast<std::int16_t>(Loop3DSoundEffect(0x4bcU, soundContext));
        --payload.doorFrame;
        return result;
    } else if (payload.doorFrame > 0) {
        --payload.doorFrame;
    }
    return payload.doorFrame;
}

void DrawNode(MT7Model &model, const void *placement, std::uint16_t id, bool direct) {
    SetModelPlacementChildVisible(placement, id, 1);
    if (!direct)
        return;
    MT7Node *const node = GetChildObjectByIndex(model, const_cast<MT7Node *>(GetMT7ReaderData(&model)), id);
    if (!node || node->xb01Offset == 0)
        return;
    (void)ModelFlagDispatch_SetModelFlagDataOpaque(
        model.source, node->xb01Offset, node->borrowedMesh ? node->borrowedMesh->drawResources : node->drawResources);
}

void DrawModelRoot(MT7Model &model) {
    if (model.nodes.empty())
        return;
    MT7Node &root = model.nodes.front();
    if (root.xb01Offset == 0)
        return;
    (void)ModelFlagDispatch_SetModelFlagDataOpaque(model.source, root.xb01Offset, root.borrowedMesh ? root.borrowedMesh->drawResources : root.drawResources);
}

void DrawFloorDigits(MT7Model &model, const void *placement, std::int16_t floor, bool direct) {
    const std::int32_t magnitude = floor < 0 ? -static_cast<std::int32_t>(floor) : floor;
    const std::int16_t tens = floor < 1 ? 207 : static_cast<std::int16_t>(152 + 11 * (magnitude / 10));
    std::int16_t ones = static_cast<std::int16_t>(magnitude % 10);
    if (floor <= 0)
        ++ones;
    DrawNode(model, placement, static_cast<std::uint16_t>(tens), direct);
    DrawNode(model, placement, static_cast<std::uint16_t>(ones != 0 ? 152 + ones : 162), direct);
}

void DrawPrimaryDisplay(ElevatorTaskPayload &payload, MT7Model &model, const void *placement, bool direct) {
    if (direct)
        DrawModelRoot(model);
    if ((payload.flags & 0x1000U) != 0) {
        DrawNode(model, placement, 250, direct);
        DrawNode(model, placement, 251, direct);
    } else if (payload.currentFloor == payload.resource->specialFloor) {
        DrawNode(model, placement, 250, direct);
        DrawNode(model, placement, 249, direct);
    } else {
        DrawFloorDigits(model, placement, payload.currentFloor, direct);
    }

    const std::int16_t stream = static_cast<std::int16_t>(GetQronNextStream());
    DrawNode(model, placement, IsFloorRequested(payload, static_cast<std::int16_t>(stream), 1) || (payload.flags & 0x0010U) != 0 ? 238 : 239, direct);
    DrawNode(model, placement, IsFloorRequested(payload, static_cast<std::int16_t>(stream), 2) || (payload.flags & 0x0020U) != 0 ? 237 : 236, direct);

    std::size_t validWord = 0;
    std::uint16_t validMask = 0;
    const bool valid = GetFloorBit(*payload.resource, stream, validWord, validMask);
    if (!valid) {
        DrawNode(model, placement, 234, direct);
        DrawNode(model, placement, 235, direct);
    } else if (stream >= payload.highestValidFloor) {
        DrawNode(model, placement, 234, direct);
    } else if (stream <= payload.lowestValidFloor) {
        DrawNode(model, placement, 235, direct);
    }
}

void DrawSecondaryDisplay(ElevatorTaskPayload &payload, MT7Model &model, const void *placement, bool direct) {
    if (direct)
        DrawModelRoot(model);
    if ((payload.flags & 0x1000U) != 0) {
        DrawNode(model, placement, 250, direct);
        DrawNode(model, placement, 251, direct);
    } else if (payload.currentFloor == payload.resource->specialFloor) {
        DrawNode(model, placement, 250, direct);
        DrawNode(model, placement, 249, direct);
    } else {
        DrawFloorDigits(model, placement, payload.currentFloor, direct);
    }

    if (payload.direction == 1 || (payload.flags & 0x0040U) != 0) {
        DrawNode(model, placement, 232, direct);
        DrawNode(model, placement, 235, direct);
    } else if (payload.direction == 2 || (payload.flags & 0x0080U) != 0) {
        DrawNode(model, placement, 233, direct);
        DrawNode(model, placement, 234, direct);
    } else {
        DrawNode(model, placement, 232, direct);
        DrawNode(model, placement, 234, direct);
    }
}

bool ReadModelWord(const SharedElevatorModel &source, std::size_t offset, std::uint32_t &value) noexcept {
    const std::span<const std::byte> bytes = source.model.source;
    if (offset > bytes.size() || bytes.size() - offset < sizeof(value))
        return false;
    std::memcpy(&value, bytes.data() + offset, sizeof(value));
    return true;
}

void WriteModelFloat(SharedElevatorModel &source, std::size_t offset, float value) noexcept {
    const std::span<const std::byte> bytes = source.model.source;
    if (offset > bytes.size() || bytes.size() - offset < sizeof(value))
        return;
    std::memcpy(const_cast<std::byte *>(bytes.data()) + offset, &value, sizeof(value));
}

void SetFloorListRootIntensity(SharedElevatorModel &source, bool dimmed) noexcept {
    if (source.model.nodes.empty())
        return;
    const std::size_t xb01 = source.model.nodes.front().xb01Offset;
    std::uint32_t signature = 0;
    std::size_t colourOffset = 0;
    if (!ReadModelWord(source, xb01, signature))
        return;
    if (signature == SHEN_NAME(XB01)) {
        std::uint32_t count = 0;
        if (!ReadModelWord(source, xb01 + 24, count))
            return;
        const std::uint64_t first64 = static_cast<std::uint64_t>(xb01) + 20 + 4ULL * (static_cast<std::uint64_t>(count) + 6);
        if (first64 > (std::numeric_limits<std::size_t>::max)())
            return;
        std::size_t cursor = static_cast<std::size_t>(first64);
        std::uint32_t command = 0;
        if (!ReadModelWord(source, cursor, command))
            return;
        cursor += static_cast<std::size_t>(command >> 8U) * 4;
        if (!ReadModelWord(source, cursor, command))
            return;
        cursor += static_cast<std::size_t>(command >> 8U) * 4;
        colourOffset = cursor + 20;
    } else {
        colourOffset = xb01 + 80;
    }
    const float value = dimmed ? 0.30000001f : 1.0f;
    WriteModelFloat(source, colourOffset, value);
    WriteModelFloat(source, colourOffset + 4, value);
    WriteModelFloat(source, colourOffset + 8, value);
}

void DrawFloorList(ElevatorTaskPayload &payload, MT7Model &model, const void *placement, bool transforms, bool direct) {
    const auto selectNode = [&](std::uint16_t id) {
        if (transforms)
            SetModelPlacementChildTransform(placement, id, nullptr);
        else
            DrawNode(model, placement, id, direct);
    };
    for (std::int16_t floor = payload.lowestValidFloor; floor <= payload.highestValidFloor; ++floor) {
        std::size_t word = 0;
        std::uint16_t mask = 0;
        if (GetFloorBit(*payload.resource, floor, word, mask)) {
            if (floor == payload.resource->specialFloor) {
                selectNode(162);
                Matrix4x4::preTranslate(0.079999998f, 0.0f, 0.0f);
            } else if (floor <= 0) {
                selectNode(182);
                Matrix4x4::preTranslate(0.059999999f, 0.0f, 0.0f);
                selectNode(static_cast<std::uint16_t>(173 - floor % 10));
                Matrix4x4::preTranslate(0.059999999f, 0.0f, 0.0f);
            } else if (floor < 10) {
                selectNode(static_cast<std::uint16_t>(152 + floor));
                Matrix4x4::preTranslate(0.079999998f, 0.0f, 0.0f);
            } else {
                selectNode(static_cast<std::uint16_t>(172 + floor / 10));
                Matrix4x4::preTranslate(0.059999999f, 0.0f, 0.0f);
                selectNode(static_cast<std::uint16_t>(172 + floor % 10));
                Matrix4x4::preTranslate(0.059999999f, 0.0f, 0.0f);
            }
            if (floor != payload.highestValidFloor) {
                if (transforms)
                    SetModelPlacementChildTransform(placement, 242, nullptr);
                else
                    DrawNode(model, nullptr, 242, direct);
                Matrix4x4::preTranslate(0.059999999f, 0.0f, 0.0f);
            }
        }
        if (floor == (std::numeric_limits<std::int16_t>::max)())
            break;
    }
}

void DrawElevatorSource(ElevatorTaskPayload &payload, std::size_t sourceIndex, const Vector3 &position) {
    SharedElevatorModel &source = sharedModels[sourceIndex];
    const ElevatorModelSource &descriptor = modelDescriptors[sourceIndex];
    if (!descriptor.modelRoot)
        return;
    const Vector3 &drawOffset = ElevatorDrawOffsets[sourceIndex];
    if (sourceIndex == 2)
        SetFloorListRootIntensity(source, (payload.flags & 0x1000U) != 0);
    const void *const placement = &payload.placementTokens[sourceIndex];
    const std::uint32_t projection = GetLegacyPerspectiveProjection();
    Matrix4x4_PushActiveTransform();
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(payload.resource->yaw));
    if (sourceIndex == 2)
        Matrix4x4_Push();
    Matrix4x4::preTranslate(drawOffset.x, drawOffset.y, drawOffset.z);
    const bool direct = !d3t::HashModel(placement, projection);
    UpdateModelFrame(&source.model, nullptr, 1, 4, placement);
    SetModelPlacementVisible(placement, 0);
    SetModelPlacementChildVisible(placement, 0, 1);
    Matrix4x4::Pop(1);
    Matrix4x4_Push();
    if (sourceIndex == 2) {
        Matrix4x4::preTranslate(payload.floorListOffset, drawOffset.y, drawOffset.z + 0.1f);
        DrawFloorList(payload, source.model, placement, true, false);
        Matrix4x4::Pop(1);
        Matrix4x4::Pop(1);
        Matrix4x4_Push();
        Matrix4x4::preTranslate(drawOffset.x, drawOffset.y, drawOffset.z);
        if (direct)
            DrawModelRoot(source.model);
        Matrix4x4::Pop(1);
        Matrix4x4_Push();
        Matrix4x4::preTranslate(payload.floorListOffset, drawOffset.y, drawOffset.z);
        DrawFloorList(payload, source.model, placement, false, direct);
    } else {
        Matrix4x4::preTranslate(drawOffset.x, drawOffset.y, drawOffset.z);
        if (sourceIndex == 0)
            DrawPrimaryDisplay(payload, source.model, placement, direct);
        else
            DrawSecondaryDisplay(payload, source.model, placement, direct);
    }
    Matrix4x4::Pop(1);
}

void DrawElevatorGroup(ElevatorTaskPayload &payload) {
    Matrix4x4_Push();
    Vector3 position = payload.resource->position;
    const float temporaryY = GetQronTemporaryFloorHeight();
    if (temporaryY != FLT_MAX)
        position.y = temporaryY;
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(payload.resource->yaw));
    for (std::size_t source = 0; source < SourceCount; ++source)
        DrawElevatorSource(payload, source, position);
    Matrix4x4::Pop(1);
}

void DrawElevatorSingle(ElevatorTaskPayload *payload) {
    if (!payload || !IsWorldDrawingEnabled())
        return;
    Vector3 camera{};
    camCtrlGetNextCamPos(&camera);
    (void)camCtrlGetNextRotY();
    const float deltaX = camera.x - payload->resource->position.x;
    const float deltaZ = camera.z - payload->resource->position.z;
    if (deltaX * deltaX + deltaZ * deltaZ > 1600.0f)
        return;

    const Vector4 colour = (payload->flags & 0x1000U) != 0 ? Vector4{0.30000001f, 0.30000001f, 0.30000001f, 0.30000001f} :
                                                             Vector4{0.60000002f, 0.60000002f, 0.30000001f, 0.30000001f};
    Vector4 saved{};
    GetGlobalLighting(&saved);
    UpdateGlobalLighting(colour);
    DrawElevatorGroup(*payload);
    UpdateGlobalLighting(saved);
}

void DrawElevatorCallback(void *taskData) {
    ElevatorTaskPayload *const payload = static_cast<ElevatorTaskPayload *>(taskData);
    DrawElevatorSingle(payload);
    if (!payload)
        return;
    if ((payload->flags & 0x0100U) != 0 && (GetQronCurrentFlags() & 0x40U) != 0 &&
        (g_currentMap.name == SHEN_NAME(QA00) || g_currentMap.name == SHEN_NAME(QD00))) {
        Matrix4x4_Push();
        Matrix4x4::preTranslate(0.0f, 3.5999999f, 0.0f);
        DrawElevatorSingle(payload);
        Matrix4x4::Pop(1);
    }
}

void ElevatorTaskCallback(void *taskData) {
    ElevatorTaskPayload *const payload = static_cast<ElevatorTaskPayload *>(taskData);
    if (!payload || (payload->flags & 0x8400U) != 0)
        return;
    SelectElevatorDirection(*payload);
    StepElevatorFloor(*payload);
    (void)HLib::ExecuteCallbackNow(DrawElevatorCallback, payload);
    (void)UpdateElevatorDoor(*payload, HLib::GetScaledDeltaTick());
}

bool LoadSharedModel(std::size_t index) {
    SharedElevatorModel &source = sharedModels[index];
    modelDescriptors[index].modelRoot = nullptr;
    source.allocation =
        static_cast<std::byte *>(LoadLegacySceneFile(Filepath_Gen(FilepathGenMode::CharaDirectory), modelDescriptors[index].filename, TASK_ELVT));
    char path[0x105];
    std::snprintf(path, sizeof(path), "%s/%s", Filepath_Gen(FilepathGenMode::CharaDirectory), modelDescriptors[index].filename);
    if (!source.allocation)
        return false;
    source.allocationSize = static_cast<std::size_t>(HLib::GetTaskDataSize(source.allocation));
    if (!ReadModelFormat7({source.allocation, source.allocationSize}, path, source.model, 0x22))
        return false;
    modelDescriptors[index].modelRoot = GetMT7ReaderData(&source.model);
    return true;
}

void InitializePayload(ElevatorTaskPayload &payload, ElevatorResourceRecord &resource) noexcept {
    std::memset(&payload, 0, sizeof(payload));
    payload.resource = &resource;
    payload.flags = resource.flags;
    payload.currentFloor = resource.currentFloor;
    payload.highestValidFloor = MinimumFloor;
    payload.lowestValidFloor = MaximumFloor;
    for (std::int16_t floor = MinimumFloor; floor <= MaximumFloor; ++floor) {
        std::size_t word = 0;
        std::uint16_t mask = 0;
        if (GetFloorBit(resource, floor, word, mask)) {
            if (payload.highestValidFloor <= floor)
                payload.highestValidFloor = floor;
            if (payload.lowestValidFloor >= floor)
                payload.lowestValidFloor = floor;
        }
    }
    for (std::int16_t floor = payload.lowestValidFloor; floor <= payload.highestValidFloor; ++floor) {
        std::size_t word = 0;
        std::uint16_t mask = 0;
        if (GetFloorBit(resource, floor, word, mask)) {
            if (floor >= 1 && floor <= 9)
                payload.floorListOffset -= 0.14f;
            else
                payload.floorListOffset -= floor == payload.highestValidFloor ? 0.12f : 0.17999999f;
        }
        if (floor == (std::numeric_limits<std::int16_t>::max)())
            break;
    }
    payload.floorListOffset *= 0.5f;
    if (g_currentMap.name == SHEN_NAME(QKBT))
        payload.flags &= static_cast<std::uint16_t>(~0x1000U);
    payload.sourceFloor = 0;
    payload.destinationFloor = 0;
}

} // namespace

void QueueElevatorTasks() {
    elevatorRegistry.fill(nullptr);
    elevatorTasks.fill(nullptr);
    elevatorCount = 0;
    const ElevatorTableView table = FindMapTable(g_currentMap);
    if (!table.records)
        return;
    for (std::size_t index = 0; index < SourceCount; ++index)
        (void)LoadSharedModel(index);

    const std::size_t count = table.count < MaximumElevators ? table.count : MaximumElevators;
    for (std::size_t index = 0; index < count; ++index) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(ElevatorTaskCallback, 13, 14, sizeof(ElevatorTaskPayload), TASK_ELVT);
        ElevatorTaskPayload *const payload = static_cast<ElevatorTaskPayload *>(HLib::GetTaskParameter(task));
        InitializePayload(*payload, table.records[index]);
        elevatorTasks[elevatorCount] = task;
        elevatorRegistry[elevatorCount] = payload;
        ++elevatorCount;
        PublishElevatorTransforms(*payload);
        for (std::size_t source = 0; source < SourceCount; ++source) {
            if (modelDescriptors[source].modelRoot) {
                (void)d3t::RegisterObjectPlacement(modelDescriptors[source].modelRoot, &payload->placementTokens[source], 0x10);
            }
        }
    }
}

void ReleaseElevatorRuntime() {
    for (std::size_t index = 0; index < elevatorCount; ++index) {
        ElevatorTaskPayload *const payload = elevatorRegistry[index];
        if (!payload)
            continue;
        for (std::size_t source = 0; source < SourceCount; ++source) {
            if (sharedModels[source].allocation)
                d3t::RetireObjectPlacement(&payload->placementTokens[source]);
        }
    }
    while (d3t::ProcessObjectStore(true, -1)) {
    }
    for (std::size_t index = 0; index < elevatorCount; ++index) {
        if (elevatorTasks[index])
            HLib::DestroyTask(elevatorTasks[index]);
    }
    for (std::size_t source = 0; source < SourceCount; ++source) {
        modelDescriptors[source].modelRoot = nullptr;
        SharedElevatorModel &model = sharedModels[source];
        if (!model.allocation)
            continue;
        ReleaseModelFormat7(model.model);
        HLib::FreeTaskData(model.allocation);
        model = {};
    }
    elevatorTasks.fill(nullptr);
    elevatorRegistry.fill(nullptr);
    elevatorCount = 0;
}

void UpdateAllElevatorTransforms() noexcept {
    for (std::size_t index = 0; index < elevatorRegistry.size(); ++index) {
        if (elevatorRegistry[index])
            PublishElevatorTransforms(*elevatorRegistry[index]);
    }
}

// @verified - p
std::int32_t GetElevatorTransform(shen_name elevator, Vector3 &position, std::uint32_t &yaw) noexcept {
    ElevatorTaskPayload *const payload = FindElevator(elevator);
    if (!payload)
        return 1;
    position = payload->resource->position;
    yaw = static_cast<std::uint32_t>(static_cast<std::int32_t>(payload->resource->yaw));
    const float temporaryY = GetQronTemporaryFloorHeight();
    if (temporaryY != FLT_MAX)
        position.y = temporaryY;
    return (payload->flags >> 11U) & 2U;
}

std::int32_t GetCurrentElevatorFloor(shen_name elevator) noexcept {
    ElevatorTaskPayload *const payload = FindElevator(elevator);
    return payload ? payload->currentFloor : MissingFloor;
}

bool IsElevatorFloorValid(shen_name elevator, std::int16_t floor) noexcept {
    ElevatorTaskPayload *const payload = FindElevator(elevator);
    if (!payload)
        return false;
    std::size_t word = 0;
    std::uint16_t mask = 0;
    return GetFloorBit(*payload->resource, floor, word, mask);
}

std::int32_t RequestElevatorFloor(shen_name elevator, std::int16_t floor, std::uint32_t requestBank) noexcept {
    ElevatorTaskPayload *const payload = FindElevator(elevator);
    if (!payload)
        return -1;
    if ((floor == payload->highestValidFloor && requestBank == 1) || (floor == payload->lowestValidFloor && requestBank == 2))
        return -1;
    if (requestBank != 0) {
        if (!SetFloorRequest(*payload, floor, requestBank, true))
            return -1;
        if (payload->currentFloor == floor && (payload->direction == 0 || payload->direction == static_cast<std::int16_t>(requestBank))) {
            payload->flags |= 0x0003U;
            return payload->doorFrame == 60 ? 1 : 0;
        }
        return 0;
    }
    (void)SetFloorRequest(*payload, floor, 1, false);
    (void)SetFloorRequest(*payload, floor, 2, false);
    return 1;
}

std::int32_t BeginElevatorTravel(float soundContext, shen_name elevator, shen_name actor, std::int32_t sourceFloor, std::int32_t destinationFloor) noexcept {
    ElevatorTaskPayload *const payload = FindElevator(elevator);
    if (!payload)
        return 0;
    const std::int16_t source = static_cast<std::int16_t>(sourceFloor);
    const std::int16_t destination = static_cast<std::int16_t>(destinationFloor);
    const std::int32_t difference = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(destinationFloor) - static_cast<std::uint32_t>(sourceFloor));
    std::size_t word = 0;
    std::uint16_t mask = 0;
    if (!GetFloorBit(*payload->resource, source, word, mask) || !GetFloorBit(*payload->resource, destination, word, mask) || difference == 0)
        return -1;
    if (payload->currentFloor != source)
        return 0;
    payload->sourceFloor = sourceFloor;
    payload->actorId = actor.name;
    payload->destinationFloor = destinationFloor;
    (void)SetFloorRequest(*payload, destination, 0, true);
    if (payload->direction == 0)
        payload->direction = difference > 0 ? 1 : 2;
    payload->flags &= static_cast<std::uint16_t>(~0x0002U);
    if (payload->doorFrame != 0)
        return 0;
    payload->flags &= static_cast<std::uint16_t>(~0x0001U);
    if ((payload->flags & 0x0004U) == 0) {
        (void)Loop3DSoundEffect(0x6b4U, soundContext);
        payload->flags |= 0x0004U;
    }
    return 1;
}

std::int32_t ArriveAndOpenElevator(float soundContext, shen_name elevator) noexcept {
    ElevatorTaskPayload *const payload = FindElevator(elevator);
    if (!payload || payload->destinationFloor != payload->currentFloor)
        return 0;
    payload->flags |= 0x0001U;
    if ((payload->flags & 0x0004U) != 0) {
        (void)Loop3DSoundEffect(0x6b5U, soundContext);
        payload->flags &= static_cast<std::uint16_t>(~0x0004U);
    }
    payload->flags |= 0x0002U;
    if (payload->doorFrame != 60)
        return 0;
    payload->actorId = 0;
    payload->sourceFloor = MissingFloor;
    payload->destinationFloor = MissingFloor;
    return 1;
}

std::int32_t CloseAndResumeElevator(shen_name elevator) noexcept {
    ElevatorTaskPayload *const payload = FindElevator(elevator);
    if (!payload)
        return -1;
    payload->flags &= static_cast<std::uint16_t>(~0x0002U);
    if (payload->doorFrame != 0)
        return 0;
    payload->flags &= static_cast<std::uint16_t>(~0x0001U);
    return 1;
}

std::int32_t SetCurrentElevatorFloor(shen_name elevator, std::int16_t floor) noexcept {
    ElevatorTaskPayload *const payload = FindElevator(elevator);
    if (!payload)
        return 0;
    payload->currentFloor = floor;
    payload->resource->currentFloor = floor;
    return 1;
}

// @verified - p
std::int32_t SetElevatorDisplayMode(shen_name elevator, std::int32_t selector, std::int32_t value) noexcept {
    ElevatorTaskPayload *const payload = FindElevator(elevator);
    if (!payload)
        return 0;
    if (selector == 0) {
        if (value != 0)
            payload->flags &= static_cast<std::uint16_t>(~0x8000U);
        else
            payload->flags |= 0x8000U;
        return 1;
    }
    if (selector != 1 || value < 0 || value > 2)
        return 0;
    payload->flags &= 0xe3ffU;
    if (value == 1)
        payload->flags |= 0x0400U;
    else if (value == 2)
        payload->flags |= 0x1000U;
    return 1;
}

std::int32_t SetElevatorIndicatorFlag(shen_name elevator, std::int32_t index, std::int32_t enabled) noexcept {
    ElevatorTaskPayload *const payload = FindElevator(elevator);
    if (!payload || index < 0 || index > 3)
        return 0;
    const std::uint16_t flag = static_cast<std::uint16_t>(0x10U << index);
    if (enabled != 0)
        payload->flags |= flag;
    else
        payload->flags &= static_cast<std::uint16_t>(~flag);
    return 1;
}

void CaptureElevatorPersistentState(std::span<std::byte, ElevatorPersistentStateSize> output) noexcept {
    output[0] = std::byte{1};
    std::size_t outputIndex = 1;
    for (const ElevatorTableView &table : permanentTables) {
        for (std::size_t record = 0; record < table.count; ++record) {
            output[outputIndex++] = static_cast<std::byte>(static_cast<std::uint8_t>(table.records[record].currentFloor));
        }
    }
}

void RestoreElevatorPersistentState(std::span<const std::byte, ElevatorPersistentStateSize> input) noexcept {
    if (input[0] == std::byte{0})
        return;
    std::size_t inputIndex = 1;
    for (const ElevatorTableView &table : permanentTables) {
        for (std::size_t record = 0; record < table.count; ++record) {
            const std::int8_t floor = static_cast<std::int8_t>(std::to_integer<std::uint8_t>(input[inputIndex++]));
            table.records[record].currentFloor = floor;
        }
    }
}

} // namespace shenmue::s2
