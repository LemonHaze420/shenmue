#include "s2/areas/area_qd00.hpp"
#include "s2/areas/area_elevator.hpp"
#include "s2/areas/area_fall_events.hpp"
#include "s2/camera.hpp"
#include "s2/date_time.hpp"
#include "s2/door.hpp"
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
constexpr MagicRoomDescriptor roomDescriptors0[]{
    {{{172.10000610351562f, 86.4000015258789f}, {172.10000610351562f, 81.0f}, {162.89999389648438f, 81.0f}, {162.89999389648438f, 86.4000015258789f}},
        {166.5f, 83.69999694824219f}, {5.400000095367432f, 7.199999809265137f}, 16385, 0, 0},
    {{{172.10000610351562f, 81.0f}, {172.10000610351562f, 75.5999984741211f}, {162.89999389648438f, 75.5999984741211f}, {162.89999389648438f, 81.0f}},
        {166.5f, 78.30000305175781f}, {5.400000095367432f, 7.199999809265137f}, 16385, 0, 0},
    {{{171.6999969482422f, 75.5999984741211f}, {171.6999969482422f, 81.0f}, {180.89999389648438f, 81.0f}, {180.89999389648438f, 75.5999984741211f}},
        {177.3000030517578f, 78.30000305175781f}, {5.400000095367432f, 7.199999809265137f}, 49151, 0, 0},
    {{{171.6999969482422f, 81.0f}, {171.6999969482422f, 86.4000015258789f}, {180.89999389648438f, 86.4000015258789f}, {180.89999389648438f, 81.0f}},
        {177.3000030517578f, 83.69999694824219f}, {5.400000095367432f, 7.199999809265137f}, 49151, 0, 0},
    {{{196.89999389648438f, 102.5999984741211f}, {196.89999389648438f, 108.0f}, {206.10000610351562f, 108.0f}, {206.10000610351562f, 102.5999984741211f}},
        {202.5f, 105.30000305175781f}, {5.400000095367432f, 7.199999809265137f}, 49152, 0, 0},
    {{{196.89999389648438f, 97.19999694824219f}, {196.89999389648438f, 102.5999984741211f}, {206.10000610351562f, 102.5999984741211f},
         {206.10000610351562f, 97.19999694824219f}},
        {202.5f, 99.9000015258789f}, {5.400000095367432f, 7.199999809265137f}, 49151, 0, 0},
    {{{196.89999389648438f, 91.80000305175781f}, {196.89999389648438f, 97.19999694824219f}, {206.10000610351562f, 97.19999694824219f},
         {206.10000610351562f, 91.80000305175781f}},
        {202.5f, 94.5f}, {5.400000095367432f, 7.199999809265137f}, 49151, 0, 0},
};
constexpr MagicRoomFloorGroup roomGroups[]{
    {7, 0, 107.19999694824219f, 110.69999694824219f, roomDescriptors0},
    {7, 7, 110.80000305175781f, 114.29999542236328f, roomDescriptors0},
    {7, 14, 114.4000015258789f, 117.89999389648438f, roomDescriptors0},
    {7, 21, 118.0f, 121.5f, roomDescriptors0},
    {7, 28, 121.5999984741211f, 125.0999984741211f, roomDescriptors0},
    {7, 35, 125.19999694824219f, 128.6999969482422f, roomDescriptors0},
    {7, 42, 128.8000030517578f, 132.3000030517578f, roomDescriptors0},
    {7, 49, 132.39999389648438f, 135.89999389648438f, roomDescriptors0},
    {7, 56, 136.0f, 139.5f, roomDescriptors0},
    {0, 0, 0, 0, nullptr},
};
QronFloorProp floorProps[]{
    {SHEN_NAME(F002), {0x0U, 0x82U, 0x0U, 0x0U}, 0, 0, 0, nullptr},
    {SHEN_NAME(F004), {0x0U, 0x40U, 0x0U, 0x0U}, 0, 0, 0, nullptr},
    {SHEN_NAME(F108), {0x0U, 0x0U, 0x0U, 0x0U}, 0, 0, 0, nullptr},
    {SHEN_NAME(F110), {0x0U, 0x200U, 0x0U, 0x0U}, 0, 0, 0, nullptr},
    {SHEN_NAME(F013), {0xffffU, 0xffdbU, 0xffffU, 0x1ffffU}, 0, 0, 0, nullptr},
    {SHEN_NAME(F014), {0x0U, 0x0U, 0x0U, 0x0U}, 0, 0, 0, nullptr},
    {SHEN_NAME(F001), {0x0U, 0x110U, 0x0U, 0x0U}, 0, 0, 0, nullptr},
    {SHEN_NAME(F003), {0x0U, 0x88U, 0x0U, 0x0U}, 0, 0, 0, nullptr},
    {SHEN_NAME(F107), {0x0U, 0x0U, 0x0U, 0x0U}, 0, 0, 0, nullptr},
    {SHEN_NAME(F109), {0x0U, 0x200U, 0x0U, 0x0U}, 0, 0, 0, nullptr},
    {SHEN_NAME(F112), {0x0U, 0x0U, 0x0U, 0x0U}, 0, 0, 0, nullptr},
    {SHEN_NAME(F111), {0x0U, 0x200U, 0x0U, 0x0U}, 0, 0, 0, nullptr},
    {SHEN_NAME(R598), {0xffffU, 0xfdffU, 0xffffU, 0xffffU}, 0, 0, 0, nullptr},
    {SHEN_NAME(F006), {0xffffU, 0xfdffU, 0xffffU, 0xffffU}, 0, 0, 0, nullptr},
    {SHEN_NAME(R140), {0x0U, 0x200U, 0x0U, 0x0U}, 0, 0, 0, nullptr},
    {SHEN_NAME(F005), {0x0U, 0x200U, 0x0U, 0x0U}, 0, 0, 0, nullptr},
    {shen_name{0x00000000U}, {0x0U, 0x0U, 0x0U, 0x0U}, 0, 0, 0, nullptr},
};
void QD00FloorChanged(Vector3 *position, std::uint32_t floor) {
    UpdateAreaElevatorBindings(position, floor);
    const shen_name lowerDoor{0x38393552U}, upperDoor{0x30343152U};
    ToggleFlagOnFieldEntity(lowerDoor, floor == 10 ? 0 : 1);
    SetDisplayFlagForProp(lowerDoor, floor == 10 ? 0 : 1);
    SetDoorSubjectEnabled(lowerDoor, floor == 10 ? 0 : 1);
    ToggleFlagOnFieldEntity(upperDoor, floor == 10 ? 1 : 0);
    SetDisplayFlagForProp(upperDoor, floor == 10 ? 1 : 0);
    const bool showFirst = floor != 10 && (floor == 3 || floor == 6);
    ToggleFlagOnFieldEntity(SHEN_NAME(K502), showFirst ? 1 : 0);
    SetDisplayFlagForProp(SHEN_NAME(K502), showFirst ? 1 : 0);
    ToggleFlagOnFieldEntity(SHEN_NAME(K507), showFirst ? 0 : 1);
    SetDisplayFlagForProp(SHEN_NAME(K507), showFirst ? 0 : 1);
}
const QronFloorDescriptor floorDescriptor{QD00FloorChanged, floorProps};
struct CameraRegion {
    Vector3 minimum, maximum;
    std::uint32_t slot;
};
static_assert(sizeof(CameraRegion) == 28);
constexpr CameraRegion cameraRegions[]{
    {{162.89999389648438f, 0.0f, 104.0f}, {199.0f, 0.0f, 121.5f}, 6},
    {{160.89999389648438f, 0.0f, 69.0f}, {200.6999969482422f, 0.0f, 79.19999694824219f}, 5},
    {{208.0f, 0.0f, 63.70000076293945f}, {222.5f, 0.0f, 92.0f}, 5},
    {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 32767},
};
void QD00MainLoopCallback(void *) {
    Vector3 camera;
    camCtrlGetNextCamPos(&camera);
    bool fifth = false, sixth = false;
    for (const auto &region : cameraRegions) {
        if (region.slot == 0x7fff)
            break;
        if (camera.z > region.minimum.z && camera.z < region.maximum.z && camera.x > region.minimum.x && camera.x < region.maximum.x) {
            fifth = region.slot == 5;
            sixth = region.slot == 6;
            break;
        }
    }
    for (std::int32_t index = 5; index <= 9; ++index) {
        const auto slot = GetMagicRoomFlag(index);
        if (slot == 0x7fff)
            continue;
        SetMapModelSlotEnabled(index, slot == 5 ? fifth : slot == 6 ? sixth : true);
    }
    for (std::int32_t index = 10; index <= 14; ++index) {
        const auto slot = GetMagicRoomFlag(index);
        if (slot == 0x7fff)
            continue;
        SetMapModelSlotEnabled(index, slot == 5 ? fifth : slot == 6 ? sixth : true);
    }
}
void ApplyLighting() {
    EV_SetWeatherInstruction(0x0f, 0);
    EV_SetWeatherInstruction(0x780, 0);
    SetMapModelDataDefaultVertexShader();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    SetLightRootPosition({0, 0, 0});
    AssignLightProgram(1);
    SetLightRootPosition({0, GetQronTemporaryFloorHeight() - 100, 0});
    AssignLightProgram(0);
    AssignLightProgram(5);
    AssignLightProgram(6);
    AssignLightProgram(2);
    AssignLightProgram(4);
    SetupLightConfig();
    CheckIsArgEquZero(1);
    SetAreaFogParams();
}
} // namespace
void AreaQD00Init() {
}
void AreaQD00Main(std::uint32_t) {
    SetMapControlStatusFlag(0, false);
    ElevatorStuff();
    EV_SetObjectFlagByIndex(0, 0);
    EV_SetObjectFlagByIndex(1, 0);
    EV_SetObjectFlagByIndex(3, 0);
    EV_SetObjectFlagByIndex(5, 0);
    EV_SetObjectFlagByIndex(6, 0);
    EV_SetObjectFlagByIndex(2, 0);
    EV_SetObjectFlagByIndex(4, 0);
    AttachMapLightingPlacement(0, 0);
    AttachMapLightingPlacement(5, 5);
    AttachMapLightingPlacement(6, 6);
    AttachMapLightingPlacement(5, 10);
    AttachMapLightingPlacement(6, 13);
    AttachMapLightingPlacement(3, 15);
    AttachMapLightingPlacement(1, 16);
    AttachMapLightingPlacement(2, 1);
    AttachMapLightingPlacement(4, 2);
    AttachMapLightingPlacement(2, 11);
    AttachMapLightingPlacement(4, 12);
    SetQronFloorDescriptor(&floorDescriptor);
    DateTime date;
    GetCurrentDateTime(date);
    EV_GetShenmueFlag(FLAG_NIGHT_TIME_ENFORCED);
    ApplyLighting();
    QueueRoomFallEvent();
    HLib::EnqueueTaskWithoutParameter(QD00MainLoopCallback, 4, 0xb, static_cast<std::int32_t>(SHEN_NAME(EVNT)));
    QueueMagicRoom("scene/03/stream", "ROOM.afs", roomGroups, 53248);
}
void AreaQD00Cleanup(std::int32_t, std::uint32_t) {
    ApplyLighting();
}
const GameplayAreaDefinition areaQD00GameplayDefinition{SHEN_NAME(QD00), 0, AreaQD00Init, AreaQD00Main, AreaQD00Cleanup};
} // namespace shenmue::s2::areas
