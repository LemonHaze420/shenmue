#pragma once

#include "s2/hlib.hpp"
#include "s2/area_names.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/math.hpp"

#define SHENMUE_QRON_JOIN_(a, b) a##b
#define SHENMUE_QRON_JOIN(a, b) SHENMUE_QRON_JOIN_(a, b)
#define SHENMUE_QRON_LAYOUT(condition) typedef char SHENMUE_QRON_JOIN(QronLayout_, __LINE__)[(condition) ? 1 : -1]
#define SHENMUE_QRON_NOEXCEPT
namespace shenmue {
namespace s2 {
using ::shenmue::shen_name;

#define AREA_QA00 shen_name(SHEN_NAME4('Q', 'A', '0', '0'))
#define AREA_QC00 shen_name(SHEN_NAME4('Q', 'C', '0', '0'))
#define AREA_QC01 shen_name(SHEN_NAME4('Q', 'C', '0', '1'))
#define AREA_QD00 shen_name(SHEN_NAME4('Q', 'D', '0', '0'))
#define AREA_QD01 shen_name(SHEN_NAME4('Q', 'D', '0', '1'))
#define AREA_QE09 shen_name(SHEN_NAME4('Q', 'E', '0', '9'))
#define AREA_QLBT shen_name(SHEN_NAME4('Q', 'L', 'B', 'T'))
#define AREA_QF00 shen_name(SHEN_NAME4('Q', 'F', '0', '0'))
#define AREA_QF02 shen_name(SHEN_NAME4('Q', 'F', '0', '2'))
#define AREA_QKBT shen_name(SHEN_NAME4('Q', 'K', 'B', 'T'))

struct QronStairConnection {
    shen_name areaId;
    std::int16_t floor;
    std::int8_t connectionId, entryId;
};
struct QronMapDefinition {
    shen_name areaId;
    float baseHeight;
    std::int32_t maximumFloor, minimumFloor;
    const QronStairConnection *connections;
    std::int32_t lightIndex;
    std::uint32_t flags;
};
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
typedef void (*QronFloorCallback)(Vector3 *, std::uint32_t);
struct QronFloorDescriptor {
    std::byte retained00[0x14];
    QronFloorCallback callback;
    void *floorData;
    shen_name fieldName;
};
SHENMUE_QRON_LAYOUT(offsetof(QronFloorDescriptor, callback) == 0x14);
SHENMUE_QRON_LAYOUT(sizeof(QronFloorDescriptor) == 0x20);
#define QRON_FIELD_QF0X shen_name(SHEN_NAME4('Q', 'F', '0', 'X'))
#define QRON_FIELD_QF03 shen_name(SHEN_NAME4('Q', 'F', '0', '3'))
#define QRON_FIELD_QF04 shen_name(SHEN_NAME4('Q', 'F', '0', '4'))
#endif
struct QronFloorRuntime {
    shen_name areaId;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    float baseHeight;
    std::int32_t maximumFloor, minimumFloor;
    shen_name fieldName;
    void *floorData;
    float temporaryFloorHeight;
#else
    std::byte retainedFloorState[0x18];
#endif
    std::int32_t floor;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    QronFloorCallback callback;
    std::uint32_t flags;
    const QronStairConnection *connections;
    std::int32_t lightIndex, previousIntersection;
    std::uint32_t savedMapSlots[5];
#else
    std::byte retainedCallbackState[0x28];
#endif
};
SHENMUE_QRON_LAYOUT(sizeof(QronStairConnection) == 8);
SHENMUE_QRON_LAYOUT(offsetof(QronStairConnection, floor) == 4);
SHENMUE_QRON_LAYOUT(offsetof(QronStairConnection, connectionId) == 6);
SHENMUE_QRON_LAYOUT(offsetof(QronStairConnection, entryId) == 7);
SHENMUE_QRON_LAYOUT(sizeof(QronMapDefinition) == 0x1c);
SHENMUE_QRON_LAYOUT(offsetof(QronMapDefinition, maximumFloor) == 8);
SHENMUE_QRON_LAYOUT(offsetof(QronMapDefinition, minimumFloor) == 0x0c);
SHENMUE_QRON_LAYOUT(offsetof(QronMapDefinition, connections) == 0x10);
SHENMUE_QRON_LAYOUT(offsetof(QronMapDefinition, lightIndex) == 0x14);
SHENMUE_QRON_LAYOUT(offsetof(QronMapDefinition, flags) == 0x18);
SHENMUE_QRON_LAYOUT(sizeof(QronFloorRuntime) == 0x48);
SHENMUE_QRON_LAYOUT(offsetof(QronFloorRuntime, floor) == 0x1c);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
SHENMUE_QRON_LAYOUT(offsetof(QronFloorRuntime, temporaryFloorHeight) == 0x18);
SHENMUE_QRON_LAYOUT(offsetof(QronFloorRuntime, callback) == 0x20);
SHENMUE_QRON_LAYOUT(offsetof(QronFloorRuntime, flags) == 0x24);
SHENMUE_QRON_LAYOUT(offsetof(QronFloorRuntime, connections) == 0x28);
SHENMUE_QRON_LAYOUT(offsetof(QronFloorRuntime, lightIndex) == 0x2c);
SHENMUE_QRON_LAYOUT(offsetof(QronFloorRuntime, previousIntersection) == 0x30);
SHENMUE_QRON_LAYOUT(offsetof(QronFloorRuntime, savedMapSlots) == 0x34);
float GetQronTemporaryFloorHeight() SHENMUE_QRON_NOEXCEPT;
float GetQronFloorHeight(std::int32_t floor);
std::uint32_t GetQronCurrentFlags();
std::int32_t GetQronFloorAtPosition(const Vector3 &position);
float FindQronIntersectionHeight(const Vector3 *position, std::int32_t floor);
void qron_task_Enqueue();
void QronFloorControl_Callback(void *data);
std::int32_t SetQronRelocatedFloor(std::int32_t floor, std::int32_t unused = 0);
std::int32_t SetQronStackedFloor(std::int32_t floor);
std::int32_t qronSetFloorDescriptor(const QronFloorDescriptor *descriptor);
std::int32_t qronSetFloor(std::int32_t floor);
std::int32_t qronRepositionPlayerForFloor(std::int32_t floor);
void QueueQronElevatorSceneChange(shen_name area, std::int32_t entry, std::int32_t floor, const Vector3 *position, std::int32_t yaw);
#endif
static const std::size_t kQronSceneModeOffset = 8, kQronSceneFloorOffset = 10;
struct QronSceneChange {
    std::uint32_t pending, retained4;
    std::int16_t mode, floor;
    std::int32_t yaw;
    Vector3 position;
};
typedef char Layout_qron_dc_scene_change[(sizeof(QronSceneChange) == 28) ? 1 : -1];
#define TASK_QRON shen_name(SHEN_NAME4('Q', 'R', 'O', 'N'))
extern QronFloorRuntime g_qronFloorRuntime;
const QronMapDefinition *FindQronMapDefinition(shen_name areaId);
std::int32_t GetQronNextStream() SHENMUE_QRON_NOEXCEPT;
std::int32_t ResolveQronEntryLocation(const QronMapDefinition &definition, Vector3 &position, std::int32_t &yaw) SHENMUE_QRON_NOEXCEPT;
bool RetainQronMapEventDestination(shen_name areaId, std::int32_t eventId);
HLib::Task *QueueQronSceneChange();
void QronSceneChange_Callback(void *data);

} // namespace s2
} // namespace shenmue
#undef SHENMUE_QRON_JOIN_
#undef SHENMUE_QRON_JOIN
#undef SHENMUE_QRON_LAYOUT
#undef SHENMUE_QRON_NOEXCEPT

#endif
