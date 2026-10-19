#include "s2/areas/area_qc00.hpp"
#include "s2/field_collision.hpp"
#include "s2/fog.hpp"
#include "s2/areas/area_elevator.hpp"
#include "s2/date_time.hpp"
#include "s2/door.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/magic_room.hpp"
#include "s2/map_control.hpp"
#include "s2/map_assets.hpp"
#include "s2/pati.hpp"
namespace shenmue::s2::areas {
namespace {
void QC00NativeNullsub(void *) {
}
constexpr MagicRoomDescriptor rooms10231600[]{
    {{{177.3000030517578f, 134.8000030517578f}, {171.89999389648438f, 134.8000030517578f}, {171.89999389648438f, 144.0f}, {177.3000030517578f, 144.0f}},
        {174.60000610351562f, 140.39999389648438f}, {5.400000095367432f, 7.199999809265137f}, 32768, 0, 0},
    {{{171.89999389648438f, 134.8000030517578f}, {166.5f, 134.8000030517578f}, {166.5f, 144.0f}, {171.89999389648438f, 144.0f}},
        {169.1999969482422f, 140.39999389648438f}, {5.400000095367432f, 7.199999809265137f}, 32768, 0, 0},
    {{{166.5f, 131.60000610351562f}, {171.89999389648438f, 131.60000610351562f}, {171.89999389648438f, 122.4000015258789f}, {166.5f, 122.4000015258789f}},
        {169.1999969482422f, 126.0f}, {5.400000095367432f, 7.199999809265137f}, 0, 0, 0},
    {{{171.89999389648438f, 131.60000610351562f}, {177.3000030517578f, 131.60000610351562f}, {177.3000030517578f, 122.4000015258789f},
         {171.89999389648438f, 122.4000015258789f}},
        {174.60000610351562f, 126.0f}, {5.400000095367432f, 7.199999809265137f}, 0, 0, 0},
    {{{164.5f, 158.39999389648438f}, {164.5f, 169.1999969482422f}, {173.6999969482422f, 169.1999969482422f}, {173.6999969482422f, 158.39999389648438f}},
        {170.10000610351562f, 163.8000030517578f}, {5.400000095367432f, 7.199999809265137f}, 49152, 0, 0},
    {{{164.5f, 169.1999969482422f}, {164.5f, 180.0f}, {173.6999969482422f, 180.0f}, {173.6999969482422f, 169.1999969482422f}},
        {170.10000610351562f, 174.60000610351562f}, {5.400000095367432f, 7.199999809265137f}, 49152, 0, 0},
    {{{164.5f, 147.60000610351562f}, {164.5f, 158.39999389648438f}, {173.6999969482422f, 158.39999389648438f}, {173.6999969482422f, 147.60000610351562f}},
        {170.10000610351562f, 153.0f}, {5.400000095367432f, 7.199999809265137f}, 49152, 0, 0},
};
constexpr MagicRoomDescriptor rooms10232000[]{
    {{{177.3000030517578f, 134.8000030517578f}, {171.89999389648438f, 134.8000030517578f}, {171.89999389648438f, 144.0f}, {177.3000030517578f, 144.0f}},
        {174.60000610351562f, 140.39999389648438f}, {5.400000095367432f, 7.199999809265137f}, 32768, 0, 0},
    {{{171.89999389648438f, 134.8000030517578f}, {166.5f, 134.8000030517578f}, {166.5f, 144.0f}, {171.89999389648438f, 144.0f}},
        {169.1999969482422f, 140.39999389648438f}, {5.400000095367432f, 7.199999809265137f}, 32768, 0, 0},
    {{{166.5f, 131.60000610351562f}, {171.89999389648438f, 131.60000610351562f}, {171.89999389648438f, 122.4000015258789f}, {166.5f, 122.4000015258789f}},
        {169.1999969482422f, 126.0f}, {5.400000095367432f, 7.199999809265137f}, 0, 0, 0},
    {{{171.89999389648438f, 131.60000610351562f}, {177.3000030517578f, 131.60000610351562f}, {177.3000030517578f, 122.4000015258789f},
         {171.89999389648438f, 122.4000015258789f}},
        {174.60000610351562f, 126.0f}, {5.400000095367432f, 7.199999809265137f}, 0, 0, 0},
    {{{164.5f, 147.60000610351562f}, {164.5f, 158.39999389648438f}, {173.6999969482422f, 158.39999389648438f}, {173.6999969482422f, 147.60000610351562f}},
        {170.10000610351562f, 153.0f}, {5.400000095367432f, 7.199999809265137f}, 49152, 0, 0},
    {{{164.5f, 169.1999969482422f}, {164.5f, 180.0f}, {173.6999969482422f, 180.0f}, {173.6999969482422f, 169.1999969482422f}},
        {170.10000610351562f, 174.60000610351562f}, {5.400000095367432f, 7.199999809265137f}, 49152, 0, 0},
};
constexpr MagicRoomDescriptor rooms10233088[]{
    {{{177.3000030517578f, 134.8000030517578f}, {171.89999389648438f, 134.8000030517578f}, {171.89999389648438f, 144.0f}, {177.3000030517578f, 144.0f}},
        {174.60000610351562f, 140.39999389648438f}, {5.400000095367432f, 7.199999809265137f}, 32768, 0, 0},
    {{{171.89999389648438f, 134.8000030517578f}, {166.5f, 134.8000030517578f}, {166.5f, 144.0f}, {171.89999389648438f, 144.0f}},
        {169.1999969482422f, 140.39999389648438f}, {5.400000095367432f, 7.199999809265137f}, 32768, 0, 0},
    {{{166.5f, 131.60000610351562f}, {171.89999389648438f, 131.60000610351562f}, {171.89999389648438f, 122.4000015258789f}, {166.5f, 122.4000015258789f}},
        {169.1999969482422f, 126.0f}, {5.400000095367432f, 7.199999809265137f}, 0, 0, 0},
    {{{171.89999389648438f, 131.60000610351562f}, {177.3000030517578f, 131.60000610351562f}, {177.3000030517578f, 122.4000015258789f},
         {171.89999389648438f, 122.4000015258789f}},
        {174.60000610351562f, 126.0f}, {5.400000095367432f, 7.199999809265137f}, 0, 0, 0},
    {{{164.5f, 147.60000610351562f}, {164.5f, 158.39999389648438f}, {173.6999969482422f, 158.39999389648438f}, {173.6999969482422f, 147.60000610351562f}},
        {170.10000610351562f, 153.0f}, {5.400000095367432f, 7.199999809265137f}, 49152, 0, 0},
    {{{164.5f, 158.39999389648438f}, {164.5f, 169.1999969482422f}, {173.6999969482422f, 169.1999969482422f}, {173.6999969482422f, 158.39999389648438f}},
        {170.10000610351562f, 163.8000030517578f}, {5.400000095367432f, 7.199999809265137f}, 49152, 0, 0},
};
constexpr MagicRoomFloorGroup floors[]{
    {7, 0, {107.19999694824219f, 110.69999694824219f}, rooms10231600},
    {6, 7, {110.80000305175781f, 114.29999542236328f}, rooms10231600},
    {6, 14, {114.4000015258789f, 117.89999389648438f}, rooms10231600},
    {7, 21, {118.0f, 121.5f}, rooms10231600},
    {7, 35, {125.19999694824219f, 128.6999969482422f}, rooms10231600},
    {6, 42, {128.8000030517578f, 132.3000030517578f}, rooms10232000},
    {7, 49, {132.39999389648438f, 135.89999389648438f}, rooms10231600},
    {6, 56, {136.0f, 139.5f}, rooms10231600},
    {7, 63, {139.60000610351562f, 143.09999084472656f}, rooms10231600},
    {6, 70, {143.1999969482422f, 146.6999969482422f}, rooms10233088},
    {7, 77, {96.4000015258789f, 99.89999389648438f}, rooms10231600},
    {0, 0, {0.0f, 0.0f}, nullptr},
};
QronFloorProp floorProps[]{
    {SHEN_NAME(F010), {0x8000U, 0x0U, 0x0U, 0x0U}, 0, 0, 0, nullptr},
    {SHEN_NAME(F011), {0x8000U, 0x0U, 0x0U, 0x0U}, 0, 0, 0, nullptr},
    {SHEN_NAME(F012), {0x8000U, 0x0U, 0x0U, 0x0U}, 0, 0, 0, nullptr},
    {SHEN_NAME(F013), {0x0U, 0x800U, 0x0U, 0x0U}, 0, 0, 0, nullptr},
    {SHEN_NAME(F014), {0x0U, 0x800U, 0x0U, 0x0U}, 0, 0, 0, nullptr},
    {SHEN_NAME(R103), {0xffffU, 0xffffU, 0xffffU, 0xffffU}, 0, 0, 0, nullptr},
    {SHEN_NAME(R104), {0xffffU, 0xffffU, 0xffffU, 0xffffU}, 0, 0, 0, nullptr},
    {SHEN_NAME(F001), {0xffffU, 0xffdbU, 0xffffU, 0x1ffffU}, 0, 0, 0, nullptr},
    {SHEN_NAME(F015), {0x0U, 0x0U, 0x0U, 0x0U}, 0, 0, 0, nullptr},
    {shen_name{0x0U}, {0x0U, 0x0U, 0x0U, 0x0U}, 0, 0, 0, nullptr},
};

void SetFieldAndProp(shen_name name, std::int32_t enabled) {
    ToggleFlagOnFieldEntity(name, enabled);
    SetDisplayFlagForProp(name, static_cast<std::uint32_t>(enabled));
}
void SetFieldAndDoor(shen_name name, std::int32_t enabled) {
    ToggleFlagOnFieldEntity(name, enabled);
    SetDoorSubjectEnabled(name, enabled);
}
void QC00FloorChanged(Vector3 *position, std::uint32_t floor) {
    UpdateAreaElevatorBindings(position, floor);
    if (floor - 3U <= 1U) {
        SetFieldAndProp(SHEN_NAME(F002), 0);
        SetFieldAndDoor(SHEN_NAME(R105), 0);
        SetFieldAndDoor(SHEN_NAME(R143), 0);
        SetFieldAndProp(SHEN_NAME(F003), 1);
        SetFieldAndDoor(SHEN_NAME(R208), 1);
        SetFieldAndDoor(SHEN_NAME(R209), 1);
    } else {
        SetFieldAndDoor(SHEN_NAME(R143), floor == 10 ? 1 : 0);
        SetFieldAndDoor(SHEN_NAME(R105), floor == 10 ? 0 : 1);
        SetFieldAndProp(SHEN_NAME(F002), 1);
        SetFieldAndProp(SHEN_NAME(F003), 0);
        SetFieldAndDoor(SHEN_NAME(R208), 0);
        SetFieldAndDoor(SHEN_NAME(R209), 0);
    }
    if (floor == 8) {
        SetFieldAndDoor(SHEN_NAME(R144), 1);
        SetFieldAndDoor(SHEN_NAME(R106), 0);
    } else {
        SetFieldAndDoor(SHEN_NAME(R144), 0);
        SetFieldAndDoor(SHEN_NAME(R106), 1);
    }
    const bool special12 = floor != 8 && floor == 12;
    SetFieldAndDoor(SHEN_NAME(R145), special12 ? 1 : 0);
    SetFieldAndDoor(SHEN_NAME(R107), special12 ? 0 : 1);
    const bool props = !special12 && (floor == 3 || floor == 6);
    SetFieldAndProp(SHEN_NAME(K502), props ? 1 : 0);
    SetFieldAndProp(SHEN_NAME(K503), props ? 0 : 1);
}
const QronFloorDescriptor descriptor{QC00FloorChanged, floorProps};
void ApplyLighting() {
    EV_SetWeatherInstruction(0x0f, 0);
    EV_SetWeatherInstruction(0x780, 0);
    EV_SetWeatherInstruction(0x1800, 0);
    SetMapModelDataDefaultVertexShader();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    SetLightRootPosition(Vector3{0.0f, GetQronTemporaryFloorHeight() - 100.0f, 0.0f});
    PushGlobalLightingProgram();
    CheckIsArgEquZero(1);
    SetAreaFogParams();
}
} // namespace
void AreaQC00Init() {
}
void AreaQC00Main(std::uint32_t) {
    SetMapControlStatusFlag(0, false);
    ElevatorStuff();
    for (std::int32_t index = 0; index < 5; ++index)
        EV_SetObjectFlagByIndex(index, 0);
    AttachMapLightingPlacement(0, 0);
    AttachMapLightingPlacement(1, 5);
    AttachMapLightingPlacement(1, 10);
    AttachMapLightingPlacement(2, 15);
    AttachMapLightingPlacement(3, 1);
    AttachMapLightingPlacement(4, 2);
    AttachMapLightingPlacement(3, 11);
    AttachMapLightingPlacement(4, 12);
    AttachMapLightingPlacement(1, 32);
    SetQronFloorDescriptor(&descriptor);
    DateTime now;
    GetCurrentDateTime(now);
    EV_GetShenmueFlag(FLAG_NIGHT_TIME_ENFORCED);
    ApplyLighting();
    InitPati();
    HLib::EnqueueTaskWithoutParameter(QC00NativeNullsub, 4, 0x0b, static_cast<std::int32_t>(SHEN_NAME(EVNT)));
    QueueMagicRoom("scene/03/stream", "ROOM.afs", floors, 49152);
}
void AreaQC00Cleanup(std::int32_t, std::uint32_t) {
    ApplyLighting();
}
const GameplayAreaDefinition areaQC00GameplayDefinition{SHEN_NAME(QC00), 0, AreaQC00Init, AreaQC00Main, AreaQC00Cleanup};
} // namespace shenmue::s2::areas
