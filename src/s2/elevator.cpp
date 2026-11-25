#include "shenmue.h"
#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/elevator.hpp"
#include "s2/event_matrix_dc.hpp"
#include "s2/camera_dc.hpp"
#include "s2/door_control_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/flag_context_dc.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/model_render_dc.hpp"
#include "s2/scene_data.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/qron_dc.hpp"
#include "s2/search_dc.hpp"
#include "s2/sound_program.hpp"
#include "platform/dc/matrix.hpp"
#include <cfloat>
#include <cmath>
#include <cstring>
#include <stdlib.h>
#else
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
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

constexpr std::int16_t MinimumFloor = -15;

constexpr std::int16_t MaximumFloor = 48;

constexpr std::int32_t MissingFloor = -1000;

constexpr std::size_t MaximumElevators = 8;

constexpr std::size_t SourceCount = 3;

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))
namespace {

ElevatorTaskPayload *FindElevator(shen_name elevator) noexcept;
void PublishElevatorTransforms(ElevatorTaskPayload &payload) noexcept;
bool GetFloorBit(const ElevatorResourceRecord &resource, std::int16_t floor, std::size_t &word, std::uint16_t &mask) noexcept;
bool IsFloorRequested(const ElevatorTaskPayload &payload, std::int16_t floor, std::uint32_t bank) noexcept;
std::int32_t SetFloorRequest(ElevatorTaskPayload &payload, std::int16_t floor, std::uint32_t bank, bool enabled) noexcept;
void SelectElevatorDirection(ElevatorTaskPayload &payload) noexcept;
void StepElevatorFloor(ElevatorTaskPayload &payload) noexcept;
std::int16_t UpdateElevatorDoor(ElevatorTaskPayload &payload, float soundContext) noexcept;
void DrawElevatorCallback(void *taskData);
void ElevatorTaskCallback(void *taskData);

} // namespace

void UpdateAllElevatorTransforms() noexcept;
std::int32_t GetElevatorTransform(shen_name elevator, Vector3 &position, std::uint32_t &yaw) noexcept;
std::int32_t GetCurrentElevatorFloor(shen_name elevator) noexcept;
bool IsElevatorFloorValid(shen_name elevator, std::int16_t floor) noexcept;
std::int32_t RequestElevatorFloor(shen_name elevator, std::int16_t floor, std::uint32_t requestBank) noexcept;
std::int32_t BeginElevatorTravel(float soundContext, shen_name elevator, shen_name actor, std::int32_t sourceFloor, std::int32_t destinationFloor) noexcept;
std::int32_t ArriveAndOpenElevator(float soundContext, shen_name elevator) noexcept;
std::int32_t CloseAndResumeElevator(shen_name elevator) noexcept;
std::int32_t SetCurrentElevatorFloor(shen_name elevator, std::int16_t floor) noexcept;
std::int32_t SetElevatorDisplayMode(shen_name elevator, std::int32_t selector, std::int32_t value) noexcept;
std::int32_t SetElevatorIndicatorFlag(shen_name elevator, std::int32_t index, std::int32_t enabled) noexcept;
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

constexpr float FloatBits(std::uint32_t bits) noexcept {
    return std::bit_cast<float>(bits);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

std::array<ElevatorResourceRecord, 5> qfElevators{{
    {.modelName = SHEN_NAME(R599), .currentFloor = 0, .yaw = -16384, .position = {FloatBits(0x43798ccd), FloatBits(0x42c80000), FloatBits(0x42ed999a)},
         .validFloorMasks = {0x8000, 0x0000, 0x0002, 0x0000}, .specialFloor = 49, .flags = 0x1008},
        {.modelName = SHEN_NAME(R598), .currentFloor = -4, .yaw = -16384, .position = {FloatBits(0x43798ccd), FloatBits(0x42c80000), FloatBits(0x42f4cccd)},
             .validFloorMasks = {0x0800, 0x0000, 0x0000, 0x0080}, .specialFloor = 49, .flags = 0x1008},
            {.modelName = SHEN_NAME(R597), .currentFloor = 0, .yaw = -16384, .position = {FloatBits(0x43798ccd), FloatBits(0x42c80000), FloatBits(0x42fc0000)},
                 .validFloorMasks = {0xa000, 0x0000, 0x0000, 0x0000}, .specialFloor = 49, .flags = 0x1000},
                {.modelName = SHEN_NAME(R596), .currentFloor = 17, .yaw = -32768,
                     .position = {FloatBits(0x435c8000), FloatBits(0x42c80000), FloatBits(0x4334428f)}, .validFloorMasks = {0x0000, 0x0080, 0x0001, 0x0000},
                     .specialFloor = 49, .flags = 0x0000},
                    {.modelName = SHEN_NAME(R595), .currentFloor = 1, .yaw = 16384,
                         .position = {FloatBits(0x436c7333), FloatBits(0x42c80000), FloatBits(0x43276666)}, .validFloorMasks = {0x0000, 0x0000, 0x0000, 0x0000},
                         .specialFloor = 49, .flags = 0x0400},
                }};

std::array<ElevatorResourceRecord, 4> qaElevators{{
    {.modelName = SHEN_NAME(R599), .currentFloor = 1, .yaw = -32768, .position = {FloatBits(0x438f8ccd), FloatBits(0x42c80000), FloatBits(0x43232666)},
         .validFloorMasks = {0x0000, 0x0201, 0x0000, 0x0000}, .specialFloor = 49, .flags = 0x0100},
        {.modelName = SHEN_NAME(R598), .currentFloor = 1, .yaw = -16384, .position = {FloatBits(0x43879333), FloatBits(0x42c80000), FloatBits(0x434b6666)},
             .validFloorMasks = {0x0000, 0x0175, 0x0000, 0x0000}, .specialFloor = 49, .flags = 0x0100},
            {.modelName = SHEN_NAME(R597), .currentFloor = 3, .yaw = 16384, .position = {FloatBits(0x4356d99a), FloatBits(0x42c80000), FloatBits(0x4359cccd)},
                 .validFloorMasks = {0x0000, 0x047c, 0x0000, 0x0000}, .specialFloor = 11, .flags = 0x0000},
                {.modelName = SHEN_NAME(R596), .currentFloor = 1, .yaw = 16384,
                     .position = {FloatBits(0x4356d99a), FloatBits(0x42c80000), FloatBits(0x435d6666)}, .validFloorMasks = {0x0000, 0x03c7, 0x0000, 0x0000},
                     .specialFloor = 49, .flags = 0x0000},
            }};

std::array<ElevatorResourceRecord, 1> qjbtElevators{{
    {.modelName = SHEN_NAME(R595), .currentFloor = 1, .yaw = -16384, .position = {FloatBits(0x438cf99a), FloatBits(0x42c80000), FloatBits(0x435c8000)},
         .validFloorMasks = {0x8000, 0x0008, 0x0000, 0x0000}, .specialFloor = 49, .flags = 0x0000},
}};

std::array<ElevatorResourceRecord, 1> qcElevators{{
    {.modelName = SHEN_NAME(R599), .currentFloor = 1, .yaw = 0, .position = {FloatBits(0x4333199a), FloatBits(0x42c80000), FloatBits(0x42fb8000)},
         .validFloorMasks = {0x8000, 0x0924, 0x0000, 0x0000}, .specialFloor = 49, .flags = 0x0000},
}};

std::array<ElevatorResourceRecord, 1> qdElevators{{
    {.modelName = SHEN_NAME(R598), .currentFloor = 1, .yaw = 0, .position = {FloatBits(0x432be666), FloatBits(0x42c80000), FloatBits(0x428f8000)},
         .validFloorMasks = {0x0000, 0x0005, 0x0000, 0x0000}, .specialFloor = 49, .flags = 0x0100},
}};

std::array<ElevatorResourceRecord, 5> qeElevators{{
    {.modelName = SHEN_NAME(R592), .currentFloor = 1, .yaw = -16384, .position = {FloatBits(0x43806000), FloatBits(0x42c80000), FloatBits(0x42a20000)},
         .validFloorMasks = {0x0000, 0x1555, 0x0000, 0x0000}, .specialFloor = 49, .flags = 0x0000},
        {.modelName = SHEN_NAME(R593), .currentFloor = 1, .yaw = -32768, .position = {FloatBits(0x43924000), FloatBits(0x42c80000), FloatBits(0x42cdb333)},
             .validFloorMasks = {0x0000, 0x03ff, 0x0000, 0x0000}, .specialFloor = 49, .flags = 0x0000},
            {.modelName = SHEN_NAME(R594), .currentFloor = 1, .yaw = -32768, .position = {FloatBits(0x43940ccd), FloatBits(0x42c80000), FloatBits(0x42cdb333)},
                 .validFloorMasks = {0x0000, 0x02ab, 0x0000, 0x0000}, .specialFloor = 49, .flags = 0x0000},
                {.modelName = SHEN_NAME(R595), .currentFloor = 1, .yaw = -32768,
                     .position = {FloatBits(0x4395d99a), FloatBits(0x42c80000), FloatBits(0x42cdb333)}, .validFloorMasks = {0x0000, 0x0155, 0x0000, 0x0000},
                     .specialFloor = 49, .flags = 0x0000},
                    {.modelName = SHEN_NAME(R596), .currentFloor = 1, .yaw = -32768,
                         .position = {FloatBits(0x4397a666), FloatBits(0x42c80000), FloatBits(0x42cdb333)}, .validFloorMasks = {0x0000, 0x0211, 0x0000, 0x0000},
                         .specialFloor = 49, .flags = 0x0000},
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
                                                                            {.map = SHEN_NAME(QF39), .records = qfElevators.data(),
                                                                                 .count = qfElevators.size()},
                                                                                {.map = SHEN_NAME(QJBT), .records = qjbtElevators.data(),
                                                                                     .count = qjbtElevators.size()},
                                                                                    {.map = SHEN_NAME(QKBT), .records = qfElevators.data(),
                                                                                         .count = qfElevators.size()},
                                                                                        {.map = SHEN_NAME(QUG0), .records = qfElevators.data(),
                                                                                             .count = qfElevators.size()},
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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

ElevatorTableView FindMapTable(shen_name map) noexcept {
    for (const ElevatorMapAlias &alias : mapAliases) {
        if (alias.map.name == map.name)
            return {.records = alias.records, .count = alias.count};
    }
    return {.records = nullptr, .count = 0};
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

void PublishElevatorTransforms(ElevatorTaskPayload *payload);

std::int32_t GetFloorBit(const ElevatorTaskPayload *payload, std::int32_t floor);

void SelectElevatorDirection(ElevatorTaskPayload *payload);

void StepElevatorFloor(ElevatorTaskPayload *payload);

void DrawElevator(void *parameter);

std::int32_t UpdateElevatorDoor(ElevatorTaskPayload *payload);

ElevatorTaskPayload *g_elevatorRegistry[8];

ElevatorResourceRecord g_elevatorResources[25] = {
    {SHEN_NAME4('R', '5', '9', '9'), 1, -32768, {287.100006f, 100.0f, 163.149994f}, {0x0U, 0x201U, 0x0U, 0x0U}, 49, 0x0U},
    {SHEN_NAME4('R', '5', '9', '8'), 1, -16384, {271.149994f, 100.0f, 203.399994f}, {0x0U, 0x175U, 0x0U, 0x0U}, 49, 0x0U},
    {SHEN_NAME4('R', '5', '9', '7'), 3, 16384, {214.850006f, 100.0f, 217.800003f}, {0x0U, 0x47cU, 0x0U, 0x0U}, 11, 0x0U},
    {SHEN_NAME4('R', '5', '9', '6'), 1, 16384, {214.850006f, 100.0f, 221.399994f}, {0x0U, 0x3c7U, 0x0U, 0x0U}, 49, 0x0U},
    {0, 0, 0, {0.0f, 0.0f, 0.0f}, {0x0U, 0x0U, 0x0U, 0x0U}, 0, 0x0U},
    {SHEN_NAME4('R', '5', '9', '5'), 1, -16384, {281.950012f, 100.0f, 220.5f}, {0x8000U, 0x8U, 0x0U, 0x0U}, 49, 0x0U},
    {0, 0, 0, {0.0f, 0.0f, 0.0f}, {0x0U, 0x0U, 0x0U, 0x0U}, 0, 0x0U},
    {SHEN_NAME4('R', '5', '9', '9'), 1, 0, {179.100006f, 100.0f, 125.75f}, {0x8000U, 0x924U, 0x0U, 0x0U}, 49, 0x0U},
    {0, 0, 0, {0.0f, 0.0f, 0.0f}, {0x0U, 0x0U, 0x0U, 0x0U}, 0, 0x0U},
    {SHEN_NAME4('R', '5', '9', '8'), 1, 0, {171.899994f, 100.0f, 71.75f}, {0x0U, 0x5U, 0x0U, 0x0U}, 49, 0x0U},
    {0, 0, 0, {0.0f, 0.0f, 0.0f}, {0x0U, 0x0U, 0x0U, 0x0U}, 0, 0x0U},
    {SHEN_NAME4('R', '5', '9', '2'), 1, -16384, {256.75f, 100.0f, 81.0f}, {0x0U, 0x1555U, 0x0U, 0x0U}, 49, 0x0U},
    {SHEN_NAME4('R', '5', '9', '3'), 1, -32768, {292.5f, 100.0f, 102.849998f}, {0x0U, 0x3ffU, 0x0U, 0x0U}, 49, 0x0U},
    {SHEN_NAME4('R', '5', '9', '4'), 1, -32768, {296.100006f, 100.0f, 102.849998f}, {0x0U, 0x2abU, 0x0U, 0x0U}, 49, 0x0U},
    {SHEN_NAME4('R', '5', '9', '5'), 1, -32768, {299.700012f, 100.0f, 102.849998f}, {0x0U, 0x155U, 0x0U, 0x0U}, 49, 0x0U},
    {SHEN_NAME4('R', '5', '9', '6'), 1, -32768, {303.299988f, 100.0f, 102.849998f}, {0x0U, 0x211U, 0x0U, 0x0U}, 49, 0x0U},
    {0, 0, 0, {0.0f, 0.0f, 0.0f}, {0x0U, 0x0U, 0x0U, 0x0U}, 0, 0x0U},
    {SHEN_NAME4('R', '5', '9', '9'), 18, -16384, {249.550003f, 100.0f, 118.800003f}, {0x8000U, 0x0U, 0x2U, 0x0U}, 49, 0x1008U},
    {SHEN_NAME4('R', '5', '9', '8'), -4, -16384, {249.550003f, 100.0f, 122.400002f}, {0x800U, 0x0U, 0x0U, 0x80U}, 49, 0x1008U},
    {SHEN_NAME4('R', '5', '9', '7'), 0, -16384, {249.550003f, 100.0f, 126.0f}, {0xa000U, 0x0U, 0x0U, 0x0U}, 49, 0x1000U},
    {SHEN_NAME4('R', '5', '9', '6'), 1, -32768, {220.5f, 100.0f, 180.25f}, {0x0U, 0x0U, 0x0U, 0x0U}, 49, 0x400U},
    {SHEN_NAME4('R', '5', '9', '5'), 1, 16384, {236.449997f, 100.0f, 167.399994f}, {0x0U, 0x0U, 0x0U, 0x0U}, 49, 0x400U},
    {0, 0, 0, {0.0f, 0.0f, 0.0f}, {0x0U, 0x0U, 0x0U, 0x0U}, 0, 0x0U},
    {SHEN_NAME4('R', '5', '9', '8'), -4, -16384, {249.550003f, 82.0f, 122.400002f}, {0x800U, 0x0U, 0x0U, 0x80U}, 49, 0x8U},
    {0, 0, 0, {0.0f, 0.0f, 0.0f}, {0x0U, 0x0U, 0x0U, 0x0U}, 0, 0x0U},
};

ElevatorModelSource g_elevatorModelSources[3] = {{0, "ELVTTOUG.MT7", {1.0f, 1.1799999475479126f, 0.10999999940395355f}, -32768, 0, 0x4445, 0.5f, 31857.0f},
    {0, "ELVTTUEG.MT7", {0.0f, 2.4700000286102295f, 0.10000000149011612f}, -32768, 0, 0, 0.0f, 0.0f},
    {0, "ELVTMKNG.MT7", {0.0f, 2.2300000190734863f, 0.11999999731779099f}, -32768, 0, 0x5545, 1.0f, 31857.0f}};

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void UpdateAllElevatorTransforms();
void PublishElevatorTransforms(ElevatorTaskPayload *payload);
ElevatorTaskPayload *FindElevator(shen_name elevator);
std::int32_t SetFloorRequest(ElevatorTaskPayload *payload, std::int32_t floor, std::int32_t bank, std::int32_t enabled);
std::int32_t IsFloorRequested(const ElevatorTaskPayload *payload, std::int32_t floor, std::int32_t bank);
std::int32_t OpenElevatorDoor(ElevatorTaskPayload *payload);
std::int32_t CloseElevatorDoor(ElevatorTaskPayload *payload);
std::int32_t GetFloorBit(const ElevatorTaskPayload *payload, std::int32_t floor);
void ElevatorTaskCallback(void *parameter);
void elevator_task_Enqueue();
std::int32_t IsElevatorFloorValid(shen_name elevator, std::int32_t floor);
void SelectElevatorDirection(ElevatorTaskPayload *payload);
void StepElevatorFloor(ElevatorTaskPayload *payload);
void DrawElevatorNode(MT7Node *root, std::int32_t id);
void DrawElevator(void *parameter);
std::int32_t UpdateElevatorDoor(ElevatorTaskPayload *payload);
std::int32_t GetCurrentElevatorFloor(shen_name elevator);
std::int32_t SetCurrentElevatorFloor(shen_name elevator, std::int32_t floor);
std::int32_t SetElevatorIndicatorFlag(shen_name elevator, std::int32_t index, std::int32_t enabled);
std::int32_t SetElevatorDisplayMode(shen_name elevator, std::int32_t selector, std::int32_t value);
std::int32_t GetElevatorTransform(shen_name elevator, Vector3 *position, std::uint32_t *yaw);
std::int32_t RequestElevatorFloor(shen_name elevator, std::int32_t floor, std::int32_t requestBank);
std::int32_t BeginElevatorTravel(shen_name actor, shen_name elevator, std::int32_t sourceFloor, std::int32_t destinationFloor);
std::int32_t ArriveAndOpenElevator(shen_name, shen_name elevator);
std::int32_t CloseAndResumeElevator(shen_name elevator);
#else
namespace {

constexpr float FloatBits(std::uint32_t bits) noexcept;
ElevatorTableView FindMapTable(shen_name map) noexcept;
void PublishElevatorTransforms(ElevatorTaskPayload &payload) noexcept;
ElevatorTaskPayload *FindElevator(shen_name elevator) noexcept;
std::int32_t SetFloorRequest(ElevatorTaskPayload &payload, std::int16_t floor, std::uint32_t bank, bool enabled) noexcept;
bool IsFloorRequested(const ElevatorTaskPayload &payload, std::int16_t floor, std::uint32_t bank) noexcept;
bool GetFloorBit(const ElevatorResourceRecord &resource, std::int16_t floor, std::size_t &word, std::uint16_t &mask) noexcept;
void ElevatorTaskCallback(void *taskData);
bool LoadSharedModel(std::size_t index);
void InitializePayload(ElevatorTaskPayload &payload, ElevatorResourceRecord &resource) noexcept;
void SelectElevatorDirection(ElevatorTaskPayload &payload) noexcept;
void StepElevatorFloor(ElevatorTaskPayload &payload) noexcept;
void ClearCurrentFloorRequests(ElevatorTaskPayload &payload) noexcept;
std::int32_t UpdateOpeningDoor(ElevatorTaskPayload &payload, float soundContext) noexcept;
std::int16_t UpdateElevatorDoor(ElevatorTaskPayload &payload, float soundContext) noexcept;
void DrawNode(MT7Model &model, const void *placement, std::uint16_t id, bool direct);
void DrawModelRoot(MT7Model &model);
void DrawFloorDigits(MT7Model &model, const void *placement, std::int16_t floor, bool direct);
void DrawPrimaryDisplay(ElevatorTaskPayload &payload, MT7Model &model, const void *placement, bool direct);
void DrawSecondaryDisplay(ElevatorTaskPayload &payload, MT7Model &model, const void *placement, bool direct);
bool ReadModelWord(const SharedElevatorModel &source, std::size_t offset, std::uint32_t &value) noexcept;
void WriteModelFloat(SharedElevatorModel &source, std::size_t offset, float value) noexcept;
void SetFloorListRootIntensity(SharedElevatorModel &source, bool dimmed) noexcept;
void DrawFloorList(ElevatorTaskPayload &payload, MT7Model &model, const void *placement, bool transforms, bool direct);
void DrawElevatorSource(ElevatorTaskPayload &payload, std::size_t sourceIndex, const Vector3 &position);
void DrawElevatorGroup(ElevatorTaskPayload &payload);
void DrawElevatorSingle(ElevatorTaskPayload *payload);
void DrawElevatorCallback(void *taskData);

} // namespace

void UpdateAllElevatorTransforms() noexcept;
void QueueElevatorTasks();
void ReleaseElevatorRuntime();
bool IsElevatorFloorValid(shen_name elevator, std::int16_t floor) noexcept;
std::int32_t GetCurrentElevatorFloor(shen_name elevator) noexcept;
std::int32_t SetCurrentElevatorFloor(shen_name elevator, std::int16_t floor) noexcept;
std::int32_t SetElevatorIndicatorFlag(shen_name elevator, std::int32_t index, std::int32_t enabled) noexcept;
void CaptureElevatorPersistentState(std::span<std::byte, ElevatorPersistentStateSize> output) noexcept;
void RestoreElevatorPersistentState(std::span<const std::byte, ElevatorPersistentStateSize> input) noexcept;
std::int32_t SetElevatorDisplayMode(shen_name elevator, std::int32_t selector, std::int32_t value) noexcept;
std::int32_t GetElevatorTransform(shen_name elevator, Vector3 &position, std::uint32_t &yaw) noexcept;
std::int32_t RequestElevatorFloor(shen_name elevator, std::int16_t floor, std::uint32_t requestBank) noexcept;
std::int32_t BeginElevatorTravel(float soundContext, shen_name elevator, shen_name actor, std::int32_t sourceFloor, std::int32_t destinationFloor) noexcept;
std::int32_t ArriveAndOpenElevator(float soundContext, shen_name elevator) noexcept;
std::int32_t CloseAndResumeElevator(shen_name elevator) noexcept;
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void elevator_task_Enqueue() {
    ElevatorResourceRecord *resource;
    std::int32_t index;
    std::int32_t floor;
    g_elevatorRegistry[0] = 0;
    g_elevatorRegistry[1] = 0;
    g_elevatorRegistry[2] = 0;
    g_elevatorRegistry[3] = 0;
    g_elevatorRegistry[4] = 0;
    g_elevatorRegistry[5] = 0;
    g_elevatorRegistry[6] = 0;
    g_elevatorRegistry[7] = 0;
    switch (g_currentMap) {
    case SHEN_NAME4('Q', 'A', '0', '0'):
    case SHEN_NAME4('Q', 'A', '1', '1'):
    case SHEN_NAME4('Q', 'A', 'E', '1'):
    case SHEN_NAME4('Q', 'A', 'W', '1'):
    case SHEN_NAME4('Q', 'A', 'E', '6'):
    case SHEN_NAME4('Q', 'A', 'W', '6'):
        resource = g_elevatorResources;
        break;
    case SHEN_NAME4('Q', 'J', 'B', 'T'):
        resource = g_elevatorResources + 5;
        break;
    case SHEN_NAME4('Q', 'C', '0', '0'):
    case SHEN_NAME4('Q', 'C', '0', '1'):
    case SHEN_NAME4('Q', 'C', '0', '6'):
        resource = g_elevatorResources + 7;
        break;
    case SHEN_NAME4('Q', 'D', '0', '0'):
    case SHEN_NAME4('Q', 'D', '0', '1'):
        resource = g_elevatorResources + 9;
        break;
    case SHEN_NAME4('Q', 'E', '0', '0'):
    case SHEN_NAME4('Q', 'E', '0', '1'):
    case SHEN_NAME4('Q', 'E', '0', '6'):
        resource = g_elevatorResources + 11;
        break;
    case SHEN_NAME4('Q', 'F', '0', '0'):
    case SHEN_NAME4('Q', 'F', '0', '1'):
    case SHEN_NAME4('Q', 'F', '0', '2'):
    case SHEN_NAME4('Q', 'F', '0', '6'):
    case SHEN_NAME4('Q', 'F', '3', '9'):
    case SHEN_NAME4('Q', 'K', 'B', 'T'):
    case SHEN_NAME4('Q', 'U', 'G', '0'):
        resource = g_elevatorResources + 17;
        break;
    case SHEN_NAME4('0', '2', '6', '0'):
        resource = g_elevatorResources + 23;
        break;
    default:
        resource = 0;
        break;
    }
    if (!resource)
        return;
    for (index = 0; resource->modelName; ++resource, ++index) {
        if (index > 7)
            continue;
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(ElevatorTaskCallback, 13, 14, sizeof(ElevatorTaskPayload), SHEN_NAME4('E', 'L', 'V', 'T'));
        ElevatorTaskPayload *payload;
        g_elevatorRegistry[index] = payload = static_cast<ElevatorTaskPayload *>(HLib::GetTaskParameter(task));
        std::uint32_t *const words = reinterpret_cast<std::uint32_t *>(payload);
        words[1] = 0;
        words[2] = 0;
        words[3] = 0;
        words[4] = 0;
        words[5] = 0;
        words[6] = 0;
        words[7] = 0;
        words[8] = 0;
        words[9] = 0;
        words[10] = 0;
        words[11] = 0;
        words[12] = 0;
        words[13] = 0;
        words[14] = 0;
        payload->resource = resource;
        payload->flags = resource->flags;
        payload->direction = 0;
        payload->currentFloor = resource->currentFloor;
        payload->highestValidFloor = -15;
        payload->lowestValidFloor = 48;
        for (floor = -15; floor <= 48; ++floor) {
            if (!GetFloorBit(payload, floor))
                continue;
            payload->highestValidFloor = payload->highestValidFloor > floor ? payload->highestValidFloor : floor;
            payload->lowestValidFloor = payload->lowestValidFloor < floor ? payload->lowestValidFloor : floor;
        }
        for (floor = payload->lowestValidFloor; floor <= payload->highestValidFloor; ++floor) {
            if (GetFloorBit(payload, floor)) {
                if (floor >= 1 && floor <= 9)
                    payload->floorListOffset -= 0.14f;
                else if (floor == payload->highestValidFloor)
                    payload->floorListOffset -= 0.12f;
                else
                    payload->floorListOffset -= 0.179999992f;
            }
        }
        payload->floorListOffset *= 0.5f;
        if (g_currentMap == SHEN_NAME4('Q', 'K', 'B', 'T'))
            payload->flags &= ~0x1000;
        PublishElevatorTransforms(payload);
    }
    for (index = 0; index < 3; ++index) {
        void *const data = shenLoadFile(Filepath_Gen(FilepathGenMode::CharaDirectory), g_elevatorModelSources[index].filename, SHEN_NAME4('E', 'L', 'V', 'T'));
        if (data)
            g_elevatorModelSources[index].modelRoot = InitializeMT7Model(data)->root;
        else
            g_elevatorModelSources[index].modelRoot = 0;
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void UpdateAllElevatorTransforms() {
    for (std::int32_t rangeIndex3 = 0; rangeIndex3 < 8; ++rangeIndex3) {
        ElevatorTaskPayload *const payload = (g_elevatorRegistry)[rangeIndex3];
        if (payload)
            PublishElevatorTransforms(payload);
    }
}

#else

void UpdateAllElevatorTransforms() noexcept {
    for (std::size_t index = 0; index < elevatorRegistry.size(); ++index) {
        if (elevatorRegistry[index])
            PublishElevatorTransforms(*elevatorRegistry[index]);
    }
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

void PublishElevatorTransforms(ElevatorTaskPayload *payload) {
    ElevatorResourceRecord *const resource = payload->resource;
    Vector3 position;
    std::memcpy(&position, &resource->position, sizeof(position));
    const float height = GetQronTemporaryFloorHeight();
    if (height != FLT_MAX)
        position.y = height;
    ElevatorModelSource *source;
    std::int32_t index;
    for (index = 0, source = g_elevatorModelSources; index < 3; ++index, ++source) {
        const std::uint32_t prefix = source->prefixToken;
        if (!prefix)
            continue;
        const shen_name name((resource->modelName & 0xffff0000U) | static_cast<std::uint16_t>(prefix));
        SearchTarget *const target = shenFindFlagTarget(name);
        if (!target)
            continue;
        if (!(payload->flags & 0x8800U))
            target->flags &= ~2U;
        else
            target->flags |= 2U;
        matrix_stack::PushUnitMatrix();
        matrix_stack::TranslateVector(&position);
        Matrix4x4::RotateAroundY_Wrapper(resource->yaw);
        matrix_stack::TranslateVector(&source->translation);
        matrix_stack::GetTranslation(&target->position);
        PopCurrentMatrix(1);
        target->lockonAngle = source->lockonAngle;
        target->yaw = static_cast<std::int16_t>(
            static_cast<std::uint32_t>(static_cast<std::int32_t>(resource->yaw)) + static_cast<std::uint32_t>(static_cast<std::int32_t>(source->yaw)));
        target->yawLimit = static_cast<std::uint16_t>(static_cast<std::int32_t>(source->yawLimit));
    }
}

#else

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
ElevatorTaskPayload *FindElevator(shen_name elevator) {
    for (std::int32_t rangeIndex4 = 0; rangeIndex4 < 8; ++rangeIndex4) {
        ElevatorTaskPayload *const payload = (g_elevatorRegistry)[rangeIndex4];
        if (payload && payload->resource->modelName == elevator)
            return payload;
    }
    return 0;
}

#else

namespace {

ElevatorTaskPayload *FindElevator(shen_name elevator) noexcept {
    for (std::size_t index = 0; index < elevatorRegistry.size(); ++index) {
        ElevatorTaskPayload *const payload = elevatorRegistry[index];
        if (payload && payload->resource->modelName.name == elevator.name)
            return payload;
    }
    return nullptr;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t SetFloorRequest(ElevatorTaskPayload *payload, std::int32_t floor, std::int32_t bank, std::int32_t enabled) {
    std::int32_t index, bit;
    index = (floor + 15) / 16;
    bit = (floor + 15) % 16;
    if (static_cast<std::uint32_t>(floor + 15) > 63U)
        return 0;
    if (((static_cast<std::int16_t>(payload->resource->validFloorMasks[index]) >> bit) & 1) == 0)
        return 0;
    if (enabled)
        payload->floorRequests[bank][index] |= 1 << bit;
    else
        payload->floorRequests[bank][index] &= ~(1 << bit);
    return 1;
}

#else

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t IsFloorRequested(const ElevatorTaskPayload *payload, std::int32_t floor, std::int32_t bank) {
    std::int32_t index, bit;
    index = (floor + 15) / 16;
    bit = (floor + 15) % 16;
    if (static_cast<std::uint32_t>(floor + 15) > 63U)
        return 0;
    if (((static_cast<std::int16_t>(payload->resource->validFloorMasks[index]) >> bit) & 1) == 0)
        return 0;
    if ((static_cast<std::int16_t>(payload->floorRequests[bank][index]) >> bit) & 1)
        return 1;
    return 0;
}

#else

namespace {

bool IsFloorRequested(const ElevatorTaskPayload &payload, std::int16_t floor, std::uint32_t bank) noexcept {
    std::size_t word = 0;
    std::uint16_t mask = 0;
    return GetFloorBit(*payload.resource, floor, word, mask) && (payload.floorRequests[bank][word] & mask) != 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

std::int32_t GetFloorBit(const ElevatorTaskPayload *payload, std::int32_t floor) {
    std::int32_t index, bit;
    index = (floor + 15) / 16;
    bit = (floor + 15) % 16;
    if (static_cast<std::uint32_t>(floor + 15) > 63U)
        return 0;
    if ((static_cast<std::int16_t>(payload->resource->validFloorMasks[index]) >> bit) & 1)
        return 1;
    return 0;
}

#else

namespace {

bool GetFloorBit(const ElevatorResourceRecord &resource, std::int16_t floor, std::size_t &word, std::uint16_t &mask) noexcept {
    const std::uint16_t shifted = static_cast<std::uint16_t>(static_cast<std::int32_t>(floor) + 15);
    if (shifted > 63)
        return false;
    word = shifted / 16;
    mask = static_cast<std::uint16_t>(1U << (shifted % 16));
    return (resource.validFloorMasks[word] & mask) != 0;
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t OpenElevatorDoor(ElevatorTaskPayload *payload) {
    if (payload->doorFrame <= 0) {
        doorPlayRigidMotion(payload->resource->modelName, 0xf0d0U, 0.0f, 1.0f);
        Loop3DSoundEffect(0x4b6U);
        Loop3DSoundEffect(0x4d3U);
        Loop3DSoundEffect(0xb41U);
        ++payload->doorFrame;
    } else {
        if (payload->doorFrame > 59)
            return 1;
        ++payload->doorFrame;
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t CloseElevatorDoor(ElevatorTaskPayload *payload) {
    if (payload->doorFrame > 59) {
        doorPlayRigidMotion(payload->resource->modelName, 0xf0cfU, 0.0f, 1.0f);
        Loop3DSoundEffect(0x4b6U);
        Loop3DSoundEffect(0x4bcU);
        --payload->doorFrame;
    } else {
        if (payload->doorFrame <= 0)
            return 1;
        --payload->doorFrame;
    }
    return 0;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void ElevatorTaskCallback(void *parameter) {
    ElevatorTaskPayload *const payload = static_cast<ElevatorTaskPayload *>(parameter);
    if (payload->flags & 0x8400U)
        return;
    SelectElevatorDirection(payload);
    StepElevatorFloor(payload);
    HLib::ExecuteCallbackNow(DrawElevator, payload);
    UpdateElevatorDoor(payload);
}

#else

namespace {

void ElevatorTaskCallback(void *taskData) {
    ElevatorTaskPayload *const payload = static_cast<ElevatorTaskPayload *>(taskData);
    if (!payload || (payload->flags & 0x8400U) != 0)
        return;
    SelectElevatorDirection(*payload);
    StepElevatorFloor(*payload);
    (void)HLib::ExecuteCallbackNow(DrawElevatorCallback, payload);
    (void)UpdateElevatorDoor(*payload, HLib::GetScaledDeltaTick());
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void SelectElevatorDirection(ElevatorTaskPayload *payload) {
    std::int32_t aboveRequest = -1000, aboveValid = -1000;
    for (std::int32_t floor = 48; floor >= payload->currentFloor; --floor) {
        if (IsFloorRequested(payload, floor, 1))
            aboveRequest = floor;
        if (floor == payload->currentFloor)
            continue;
        if (GetFloorBit(payload, floor))
            aboveValid = floor;
        if (IsFloorRequested(payload, floor, 2))
            aboveRequest = floor;
        if (IsFloorRequested(payload, floor, 0))
            aboveRequest = floor;
    }
    std::int32_t belowRequest = -1000, belowValid = -1000;
    for (std::int32_t floor = -15; floor <= payload->currentFloor; ++floor) {
        if (IsFloorRequested(payload, floor, 2))
            belowRequest = floor;
        if (floor == payload->currentFloor)
            continue;
        if (GetFloorBit(payload, floor))
            belowValid = floor;
        if (IsFloorRequested(payload, floor, 1))
            belowRequest = floor;
        if (IsFloorRequested(payload, floor, 0))
            belowRequest = floor;
    }
    std::int32_t requested;
    if (aboveRequest == -1000)
        requested = belowRequest == -1000 ? 0 : 2;
    else
        requested = belowRequest == -1000 ? 1 : 3;
    switch (payload->direction) {
    case 1:
        if (requested == 2)
            payload->direction = 2;
        if (!requested)
            payload->direction = 0;
        break;
    case 2:
        if (requested == 1)
            payload->direction = 1;
        if (!requested)
            payload->direction = 0;
        break;
    case 0:
        if (requested == 3) {
            if (payload->currentFloor - belowRequest >= aboveRequest - payload->currentFloor)
                payload->direction = 1;
            else
                payload->direction = 2;
        }
        if (requested == 1)
            payload->direction = 1;
        if (requested == 2)
            payload->direction = 2;
        if (!requested) {
            if (GetFloorBit(payload, payload->currentFloor))
                payload->direction = 0;
            else if (payload->currentFloor - belowValid > aboveValid - payload->currentFloor)
                payload->direction = 1;
            else
                payload->direction = 2;
        }
        break;
    }
}

#else

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

void StepElevatorFloor(ElevatorTaskPayload *payload) {
    std::int32_t duration = 30;
    const std::int32_t flags = payload->flags;
    if (flags & 1)
        return;
    if (flags & 8)
        duration = 10;
    switch (payload->direction) {
    case 1:
        if (++payload->floorStepTicks > duration) {
            payload->floorStepTicks = 0;
            ++payload->currentFloor;
        }
        break;
    case 2:
        if (++payload->floorStepTicks > duration) {
            payload->floorStepTicks = 0;
            --payload->currentFloor;
        }
        break;
    }
}

#else

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void ClearCurrentFloorRequests(ElevatorTaskPayload &payload) noexcept {
    (void)SetFloorRequest(payload, payload.currentFloor, 1, false);
    (void)SetFloorRequest(payload, payload.currentFloor, 2, false);
    (void)SetFloorRequest(payload, payload.currentFloor, 0, false);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void DrawElevatorNode(MT7Node *root, std::int32_t id) {
    MT7Node *const node = FindMT7Node(root, id);
    if (node)
        dc::sdk::DrawSingleLightMT7Mesh(node->mesh);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
void DrawElevator(void *parameter) {
    ElevatorTaskPayload *const payload = static_cast<ElevatorTaskPayload *>(parameter);
    Vector3 offsets[3];
    Vector3 position;
    Vector4 saved, lit, dim;
    Vector3 camera;
    offsets[0].x = 1.0f, offsets[0].y = 1.18f, offsets[0].z = 0.11f;
    offsets[1].x = 0.0f, offsets[1].y = 2.47f, offsets[1].z = 0.1f;
    offsets[2].x = 0.0f, offsets[2].y = 2.23f, offsets[2].z = 0.12f;
    lit.x = 0.6f, lit.y = 0.6f, lit.z = 0.3f, lit.w = 0.3f;
    dim.x = 0.3f, dim.y = 0.3f, dim.z = 0.3f, dim.w = 0.3f;
    if (!g_worldDrawingEnabled)
        return;
    camCtrlGetNextCamPos(&camera);
    camCtrlGetNextRotY();
    const float dx = camera.x - payload->resource->position.x, dz = camera.z - payload->resource->position.z;
    if (((dx) * (dx) + (dz * dz)) > 1600.0f)
        return;
    GetGlobalLighting(&saved);
    if (payload->flags & 0x1000U)
        UpdateGlobalLighting(dim);
    else
        UpdateGlobalLighting(lit);
    std::int32_t tens = labs(static_cast<std::int32_t>(payload->currentFloor));
    tens /= 10;
    std::int32_t ones = labs(static_cast<std::int32_t>(payload->currentFloor)) % 10;
    if (payload->currentFloor <= 0)
        ++ones;
    position = payload->resource->position;
    const std::int32_t displayedFloor = GetQronNextStream();
    const float height = GetQronTemporaryFloorHeight();
    if (height != FLT_MAX)
        position.y = height;
    matrix_stack::Matrix4x4_Push(0);
    matrix_stack::TranslateVector(&position);
    Matrix4x4::RotateAroundY_Wrapper(payload->resource->yaw);
    MT7Node *root = g_elevatorModelSources[0].modelRoot;
    if (root) {
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::TranslateVector(&offsets[0]);
        dc::sdk::DrawSingleLightMT7Mesh(root->mesh);
        if (payload->flags & 0x1000U) {
            DrawElevatorNode(root, 250);
            DrawElevatorNode(root, 251);
        } else if (payload->resource->specialFloor == payload->currentFloor) {
            DrawElevatorNode(root, 250);
            DrawElevatorNode(root, 249);
        } else {
            if (payload->currentFloor > 0)
                DrawElevatorNode(root, tens * 11 + 152);
            else
                DrawElevatorNode(root, 207);
            if (ones == 0)
                DrawElevatorNode(root, 162);
            else
                DrawElevatorNode(root, ones + 152);
        }
        if (IsFloorRequested(payload, displayedFloor, 1) || (payload->flags & 0x10U))
            DrawElevatorNode(root, 238);
        else
            DrawElevatorNode(root, 239);
        if (IsFloorRequested(payload, displayedFloor, 2) || (payload->flags & 0x20U))
            DrawElevatorNode(root, 237);
        else
            DrawElevatorNode(root, 236);
        if (!GetFloorBit(payload, displayedFloor)) {
            DrawElevatorNode(root, 234);
            DrawElevatorNode(root, 235);
        } else if (displayedFloor >= payload->highestValidFloor) {
            DrawElevatorNode(root, 234);
        } else if (displayedFloor <= payload->lowestValidFloor) {
            DrawElevatorNode(root, 235);
        }
        PopCurrentMatrix(1);
    }
    root = g_elevatorModelSources[1].modelRoot;
    if (root) {
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::TranslateVector(&offsets[1]);
        dc::sdk::DrawSingleLightMT7Mesh(root->mesh);
        if (payload->flags & 0x1000U) {
            DrawElevatorNode(root, 250);
            DrawElevatorNode(root, 251);
        } else if (payload->resource->specialFloor == payload->currentFloor) {
            DrawElevatorNode(root, 250);
            DrawElevatorNode(root, 249);
        } else {
            if (payload->currentFloor > 0)
                DrawElevatorNode(root, tens * 11 + 152);
            else
                DrawElevatorNode(root, 207);
            if (ones == 0)
                DrawElevatorNode(root, 162);
            else
                DrawElevatorNode(root, ones + 152);
        }
        if (payload->direction == 1 || (payload->flags & 0x40U)) {
            DrawElevatorNode(root, 232);
            DrawElevatorNode(root, 235);
        } else if (payload->direction == 2 || (payload->flags & 0x80U)) {
            DrawElevatorNode(root, 233);
            DrawElevatorNode(root, 234);
        } else {
            DrawElevatorNode(root, 232);
            DrawElevatorNode(root, 234);
        }
        PopCurrentMatrix(1);
    }
    root = g_elevatorModelSources[2].modelRoot;
    if (root) {
        MT7MeshMaterialCommand *const command = reinterpret_cast<MT7MeshMaterialCommand *>(static_cast<MT7MeshHeader *>(root->mesh) + 1);
        const float intensity = payload->flags & 0x1000U ? 0.3f : 1.0f;
        Vector3 *const colour = reinterpret_cast<Vector3 *>(&command->retainedFormatState[0x0b]);
        colour->x = intensity;
        colour->y = intensity;
        colour->z = intensity;
        matrix_stack::Matrix4x4_Push(0);
        matrix_stack::TranslateVector(&offsets[2]);
        dc::sdk::DrawSingleLightMT7Mesh(root->mesh);
        PopCurrentMatrix(1);
        matrix_stack::Matrix4x4_Push(0);
        Matrix4x4::preTranslate(payload->floorListOffset, offsets[2].y, offsets[2].z);
        for (std::int32_t floor = payload->lowestValidFloor; floor <= payload->highestValidFloor; ++floor) {
            if (!GetFloorBit(payload, floor))
                continue;
            if (floor == payload->resource->specialFloor) {
                DrawElevatorNode(root, 162);
                Matrix4x4::preTranslate(0.08f, 0.0f, 0.0f);
            } else if (floor >= 1 && floor <= 9) {
                DrawElevatorNode(root, 152 + floor);
                Matrix4x4::preTranslate(0.08f, 0.0f, 0.0f);
            } else if (floor > 9) {
                DrawElevatorNode(root, 172 + floor / 10);
                Matrix4x4::preTranslate(0.06f, 0.0f, 0.0f);
                DrawElevatorNode(root, 172 + floor % 10);
                Matrix4x4::preTranslate(0.06f, 0.0f, 0.0f);
            } else if (floor <= 0) {
                DrawElevatorNode(root, 182);
                Matrix4x4::preTranslate(0.06f, 0.0f, 0.0f);
                DrawElevatorNode(root, 173 - floor % 10);
                Matrix4x4::preTranslate(0.06f, 0.0f, 0.0f);
            }
            if (floor != payload->highestValidFloor) {
                DrawElevatorNode(root, 242);
                Matrix4x4::preTranslate(0.06f, 0.0f, 0.0f);
            }
        }
        PopCurrentMatrix(1);
    }
    PopCurrentMatrix(1);
    UpdateGlobalLighting(saved);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t UpdateElevatorDoor(ElevatorTaskPayload *payload) {
    SetFloorRequest(payload, payload->currentFloor, 1, 0);
    SetFloorRequest(payload, payload->currentFloor, 2, 0);
    SetFloorRequest(payload, payload->currentFloor, 0, 0);
    std::int32_t result;
    if (payload->flags & 2U)
        result = OpenElevatorDoor(payload);
    else
        result = CloseElevatorDoor(payload);
    return result;
}

#else

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void DrawModelRoot(MT7Model &model) {
    if (model.nodes.empty())
        return;
    MT7Node &root = model.nodes.front();
    if (root.xb01Offset == 0)
        return;
    (void)ModelFlagDispatch_SetModelFlagDataOpaque(model.source, root.xb01Offset, root.borrowedMesh ? root.borrowedMesh->drawResources : root.drawResources);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void DrawFloorDigits(MT7Model &model, const void *placement, std::int16_t floor, bool direct) {
    const std::int32_t magnitude = floor < 0 ? -static_cast<std::int32_t>(floor) : floor;
    const std::int16_t tens = floor < 1 ? 207 : static_cast<std::int16_t>(152 + 11 * (magnitude / 10));
    std::int16_t ones = static_cast<std::int16_t>(magnitude % 10);
    if (floor <= 0)
        ++ones;
    DrawNode(model, placement, static_cast<std::uint16_t>(tens), direct);
    DrawNode(model, placement, static_cast<std::uint16_t>(ones != 0 ? 152 + ones : 162), direct);
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

bool ReadModelWord(const SharedElevatorModel &source, std::size_t offset, std::uint32_t &value) noexcept {
    const std::span<const std::byte> bytes = source.model.source;
    if (offset > bytes.size() || bytes.size() - offset < sizeof(value))
        return false;
    std::memcpy(&value, bytes.data() + offset, sizeof(value));
    return true;
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

void WriteModelFloat(SharedElevatorModel &source, std::size_t offset, float value) noexcept {
    const std::span<const std::byte> bytes = source.model.source;
    if (offset > bytes.size() || bytes.size() - offset < sizeof(value))
        return;
    std::memcpy(const_cast<std::byte *>(bytes.data()) + offset, &value, sizeof(value));
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

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

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

namespace {

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

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t IsElevatorFloorValid(shen_name elevator, std::int32_t floor) {
    const ElevatorTaskPayload *const payload = FindElevator(elevator);
    if (!payload)
        return 0;
    return GetFloorBit(payload, floor);
}

#else

bool IsElevatorFloorValid(shen_name elevator, std::int16_t floor) noexcept {
    ElevatorTaskPayload *const payload = FindElevator(elevator);
    if (!payload)
        return false;
    std::size_t word = 0;
    std::uint16_t mask = 0;
    return GetFloorBit(*payload->resource, floor, word, mask);
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t GetCurrentElevatorFloor(shen_name elevator) {
    const ElevatorTaskPayload *const payload = FindElevator(elevator);
    return payload ? payload->currentFloor : -1000;
}

#else

std::int32_t GetCurrentElevatorFloor(shen_name elevator) noexcept {
    ElevatorTaskPayload *const payload = FindElevator(elevator);
    return payload ? payload->currentFloor : MissingFloor;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t SetCurrentElevatorFloor(shen_name elevator, std::int32_t floor) {
    ElevatorTaskPayload *const payload = FindElevator(elevator);
    if (!payload)
        return 0;
    payload->currentFloor = static_cast<std::int16_t>(floor);
    return 1;
}

#else

std::int32_t SetCurrentElevatorFloor(shen_name elevator, std::int16_t floor) noexcept {
    ElevatorTaskPayload *const payload = FindElevator(elevator);
    if (!payload)
        return 0;
    payload->currentFloor = floor;
    payload->resource->currentFloor = floor;
    return 1;
}

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

std::int32_t SetElevatorIndicatorFlag(shen_name elevator, std::int32_t index, std::int32_t enabled) {
    ElevatorTaskPayload *const payload = FindElevator(elevator);
    if (!payload)
        return 0;
    std::int32_t mask;
    switch (index) {
    case 0:
        mask = 0x10;
        break;
    case 1:
        mask = 0x20;
        break;
    case 2:
        mask = 0x40;
        break;
    case 3:
        mask = 0x80;
        break;
    default:
        return 0;
    }
    if (enabled)
        payload->flags |= mask;
    else
        payload->flags &= ~mask;
    return 1;
}

#else

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

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

void CaptureElevatorPersistentState(std::span<std::byte, ElevatorPersistentStateSize> output) noexcept {
    output[0] = std::byte{1};
    std::size_t outputIndex = 1;
    for (const ElevatorTableView &table : permanentTables) {
        for (std::size_t record = 0; record < table.count; ++record) {
            output[outputIndex++] = static_cast<std::byte>(static_cast<std::uint8_t>(table.records[record].currentFloor));
        }
    }
}

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM))

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t SetElevatorDisplayMode(shen_name elevator, std::int32_t selector, std::int32_t value) {
    ElevatorTaskPayload *const payload = FindElevator(elevator);
    if (!payload)
        return 0;
    switch (selector) {
    case 0:
        if (value)
            payload->flags &= 0x7fff;
        else
            payload->flags |= static_cast<std::int16_t>(0x8000);
        break;
    case 1:
        switch (value) {
        case 0:
            payload->flags &= static_cast<std::int16_t>(0xe3ff);
            break;
        case 1:
            payload->flags = (payload->flags | 0x400) & static_cast<std::int16_t>(0xe7ff);
            break;
        case 2:
            payload->flags = (payload->flags | 0x1000) & static_cast<std::int16_t>(0xf3ff);
            break;
        default:
            return 0;
        }
        break;
    default:
        return 0;
    }
    return 1;
}

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

std::int32_t GetElevatorTransform(shen_name elevator, Vector3 *position, std::uint32_t *yaw) {
    ElevatorTaskPayload *const payload = FindElevator(elevator);
    if (!payload)
        return 1;
    *position = payload->resource->position;
    *yaw = static_cast<std::uint32_t>(static_cast<std::int32_t>(payload->resource->yaw));
    const float height = GetQronTemporaryFloorHeight();
    if (height != FLT_MAX)
        position->y = height;
    if (payload->flags & 0x1000)
        return 2;
    return 0;
}

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t RequestElevatorFloor(shen_name elevator, std::int32_t floor, std::int32_t requestBank) {
    ElevatorTaskPayload *const payload = FindElevator(elevator);
    if (!payload || (floor == payload->highestValidFloor && requestBank == 1) || (floor == payload->lowestValidFloor && requestBank == 2))
        return -1;
    if (requestBank == 0) {
        SetFloorRequest(payload, floor, 1, 0);
        SetFloorRequest(payload, floor, 2, 0);
        return 1;
    }
    if (!SetFloorRequest(payload, floor, requestBank, 1))
        return -1;
    if (payload->currentFloor == floor) {
        if (payload->direction && payload->direction != requestBank)
            return 0;
        payload->flags |= 3U;
        if (payload->doorFrame == 60)
            return 1;
    }
    return 0;
}

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t BeginElevatorTravel(shen_name actor, shen_name elevator, std::int32_t sourceFloor, std::int32_t destinationFloor) {
    const std::int32_t difference = destinationFloor - sourceFloor;
    ElevatorTaskPayload *const payload = FindElevator(elevator);
    if (!payload)
        return 0;
    if (!GetFloorBit(payload, sourceFloor))
        return -1;
    if (!GetFloorBit(payload, destinationFloor))
        return -1;
    if (!difference)
        return -1;
    std::int32_t direction;
    if (difference > 0)
        direction = 1;
    else
        direction = 2;
    if (payload->currentFloor != sourceFloor)
        return 0;
    payload->actor = actor;
    payload->sourceFloor = sourceFloor;
    payload->destinationFloor = destinationFloor;
    SetFloorRequest(payload, destinationFloor, 0, 1);
    if (!payload->direction)
        payload->direction = static_cast<std::int16_t>(direction);
    payload->flags &= ~2;
    if (payload->doorFrame)
        return 0;
    payload->flags &= ~1;
    if (!(payload->flags & 4U)) {
        Loop3DSoundEffect(0x6b4U);
        payload->flags |= 4U;
    }
    payload->resource->currentFloor = static_cast<std::int16_t>(destinationFloor);
    return 1;
}

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t ArriveAndOpenElevator(shen_name, shen_name elevator) {
    ElevatorTaskPayload *const payload = FindElevator(elevator);
    if (!payload || payload->destinationFloor != payload->currentFloor)
        return 0;
    payload->flags |= 1U;
    if (payload->flags & 4U) {
        Loop3DSoundEffect(0x6b5U);
        payload->flags &= ~4;
    }
    payload->flags |= 2U;
    if (payload->doorFrame != 60)
        return 0;
    payload->actor = __typeof__(payload->actor)();
    payload->sourceFloor = -1000;
    payload->destinationFloor = -1000;
    return 1;
}

#else

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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)

// @verified - d
std::int32_t CloseAndResumeElevator(shen_name elevator) {
    ElevatorTaskPayload *const payload = FindElevator(elevator);
    if (!payload)
        return -1;
    std::int16_t flags = payload->flags;
    flags &= ~2;
    payload->flags = flags;
    if (payload->doorFrame)
        return 0;
    payload->flags = flags & ~1;
    return 1;
}

#else

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

#endif

} // namespace s2
} // namespace shenmue

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_GetAnimatedObjectState(std::uint32_t elevator, std::int32_t floor) {
    return shenmue::s2::IsElevatorFloorValid(shenmue::shen_name(elevator), floor);
}

extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_GetCurrentElevatorFloor(std::uint32_t elevator) {
    return shenmue::s2::GetCurrentElevatorFloor(shenmue::shen_name(elevator));
}

extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_SetAnimatedObjectState(std::uint32_t elevator, std::int32_t floor) {
    return shenmue::s2::SetCurrentElevatorFloor(shenmue::shen_name(elevator), floor);
}

extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_SetAnimatedObjectFlags(std::uint32_t elevator, std::int32_t index, std::int32_t enabled) {
    return shenmue::s2::SetElevatorIndicatorFlag(shenmue::shen_name(elevator), index, enabled);
}

extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_SetElevatorDisplayMode(std::uint32_t elevator, std::int32_t selector, std::int32_t value) {
    return shenmue::s2::SetElevatorDisplayMode(shenmue::shen_name(elevator), selector, value);
}

extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_GetElevatorTransform(std::uint32_t elevator, shenmue::Vector3 *position, std::uint32_t *yaw) {
    return shenmue::s2::GetElevatorTransform(shenmue::shen_name(elevator), position, yaw);
}

extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_SetAnimatedObjectDestination(std::uint32_t elevator, std::int32_t floor, std::int32_t bank) {
    return shenmue::s2::RequestElevatorFloor(shenmue::shen_name(elevator), floor, bank);
}

extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_SetAnimatedObjectMotionRange(
    std::uint32_t actor, std::uint32_t elevator, std::int32_t sourceFloor, std::int32_t destinationFloor) {
    return shenmue::s2::BeginElevatorTravel(shenmue::shen_name(actor), shenmue::shen_name(elevator), sourceFloor, destinationFloor);
}

extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_FinishAnimatedObjectTransition(std::uint32_t actor, std::uint32_t elevator) {
    return shenmue::s2::ArriveAndOpenElevator(shenmue::shen_name(actor), shenmue::shen_name(elevator));
}

extern "C" std::int32_t __attribute__((renesas)) shenmue_s2_event_CancelAnimatedObjectTransition(std::uint32_t elevator) {
    return shenmue::s2::CloseAndResumeElevator(shenmue::shen_name(elevator));
}
#else
#endif
