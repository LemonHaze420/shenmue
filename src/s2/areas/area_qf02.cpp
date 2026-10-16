#include "s2/areas/area_qf02.hpp"
#include "s2/areas/area_elevator.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/fog.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/magic_room.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"

namespace shenmue::s2::areas {
namespace {
QronFloorProp qf02FloorProps[]{
    {SHEN_NAME(F004), {0, 4, 0, 0}, 0, 0, 0, nullptr},
    {SHEN_NAME(F005), {0, 10, 0, 0}, 0, 0, 0, nullptr},
    {SHEN_NAME(F006), {0, 0, 0, 0}, 0, 0, 0, nullptr},
    {SHEN_NAME(F010), {0, 2, 0, 0}, 0, 0, 0, nullptr},
    {SHEN_NAME(F011), {0, 0, 0, 0}, 0, 0, 0, nullptr},
    {SHEN_NAME(F012), {0, 12, 0, 0}, 0, 0, 0, nullptr},
    {SHEN_NAME(F013), {0, 10, 0, 0}, 0, 0, 0, nullptr},
    {SHEN_NAME(F014), {0, 4, 0, 0}, 0, 0, 0, nullptr},
    {SHEN_NAME(F015), {0, 0, 0, 0}, 0, 0, 0, nullptr},
    {SHEN_NAME(R595), {65535, 65535, 65535, 65535}, 0, 0, 0, nullptr},
    {},
};
constexpr MagicRoomDescriptor rooms[]{
    {{{215.10000610351562f, 126.19999694824219f}, {220.5f, 126.19999694824219f}, {220.5f, 117.0f}, {215.10000610351562f, 117.0f}},
        {217.8000030517578f, 120.5999984741211f}, {5.400000095367432f, 7.199999809265137f}, 0, 0, 0},
    {{{220.5f, 126.19999694824219f}, {225.89999389648438f, 126.19999694824219f}, {225.89999389648438f, 117.0f}, {220.5f, 117.0f}},
        {223.1999969482422f, 120.5999984741211f}, {5.400000095367432f, 7.199999809265137f}, 0, 0, 0},
    {{{231.3000030517578f, 126.19999694824219f}, {236.6999969482422f, 126.19999694824219f}, {236.6999969482422f, 117.0f}, {231.3000030517578f, 117.0f}},
        {234.0f, 120.5999984741211f}, {5.400000095367432f, 7.199999809265137f}, 0, 0, 0},
    {{{225.89999389648438f, 126.19999694824219f}, {231.3000030517578f, 126.19999694824219f}, {231.3000030517578f, 117.0f}, {225.89999389648438f, 117.0f}},
        {228.60000610351562f, 120.5999984741211f}, {5.400000095367432f, 7.199999809265137f}, 0, 0, 0},
};
constexpr MagicRoomFloorGroup floors[]{
    {3, 2, {107.19999694824219f, 110.69999694824219f}, rooms},
    {4, 5, {110.80000305175781f, 114.29999542236328f}, rooms},
    {4, 9, {114.4000015258789f, 117.89999389648438f}, rooms},
    {},
};
void QF02FloorChanged(Vector3 *position, std::uint32_t floor) {
    UpdateAreaElevatorBindings(position, floor);
    constexpr shen_name qf03{SHEN_NAME(QF03)};
    constexpr shen_name qf04{SHEN_NAME(QF04)};
    constexpr shen_name f016{SHEN_NAME(F016)};
    if (floor == 2) {
        flddSetPointFlagOnGrid(qf03, 0);
        flddSetPointFlagOnGrid(qf04, 0);
        ToggleFlagOnFieldEntity(f016, 0);
        SetDisplayFlagForProp(f016, 0);
    } else if (floor == 3 || floor == 4) {
        flddSetPointFlagOnGrid(qf03, floor == 3 ? 1 : 0);
        flddSetPointFlagOnGrid(qf04, floor == 4 ? 1 : 0);
        ToggleFlagOnFieldEntity(f016, 1);
        SetDisplayFlagForProp(f016, 1);
    }
}
const QronFloorDescriptor qf02FloorDescriptor{QF02FloorChanged, qf02FloorProps};
void QF02Nullsub(void *) {
}
void ApplyQF02Lighting() {
    EV_SetWeatherInstruction(0x0f, 0);
    EV_SetWeatherInstruction(0x780, 0);
    EV_SetWeatherInstruction(0x1800, 0);
    SetMapModelDataDefaultVertexShader();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    Vector3 position{0.0f, GetQronTemporaryFloorHeight() - 100.0f, 0.0f};
    SetLightRootPosition(position);
    SetMapLightingPlacementPosition(&position, 2);
    SetMapLightingPlacementRotation(0, 2);
    SetMapLightingPlacementPosition(&position, 3);
    SetMapLightingPlacementRotation(0, 3);
    PushGlobalLightingProgram();
    (void)CheckIsArgEquZero(1);
    position = {0.0f, 10.800000190734863f, 0.0f};
    SetMapLightingPlacementPosition(&position, 2);
    SetMapLightingPlacementRotation(0, 2);
    position = {0.0f, 14.399999618530273f, 0.0f};
    SetMapLightingPlacementPosition(&position, 3);
    SetMapLightingPlacementRotation(0, 3);
    SetAreaFogParams();
}
} // namespace
void AreaQF02Init() {
}
void AreaQF02Main(std::uint32_t) {
    SetMapControlStatusFlag(0, false);
    ElevatorStuff();
    EV_SetObjectFlagByIndex(0, 0);
    EV_SetObjectFlagByIndex(1, 0);
    EV_SetObjectFlagByIndex(4, 0);
    EV_SetObjectFlagByIndex(5, 0);
    AttachMapLightingPlacement(0, 0);
    AttachMapLightingPlacement(1, 1);
    AttachMapLightingPlacement(5, 5);
    AttachMapLightingPlacement(5, 10);
    AttachMapLightingPlacement(4, 15);
    SetQronFloorDescriptor(&qf02FloorDescriptor);
    (void)EV_GetShenmueFlag(0xff);
    ApplyQF02Lighting();
    HLib::EnqueueTaskWithoutParameter(QF02Nullsub, 4, 0x0b, 1414420037);
    QueueMagicRoom("scene/03/stream", "ROOM.afs", floors, 61952);
}
void AreaQF02Cleanup(std::int32_t, std::uint32_t) {
    ApplyQF02Lighting();
}
const GameplayAreaDefinition areaQF02GameplayDefinition{SHEN_NAME(QF02), 0, AreaQF02Init, AreaQF02Main, AreaQF02Cleanup};
} // namespace shenmue::s2::areas
