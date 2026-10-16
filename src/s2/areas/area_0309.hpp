#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0309_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0309_a512{SHEN_NAME(a512)};
inline constexpr shen_name AREA_0309_a516{SHEN_NAME(a516)};
inline constexpr shen_name AREA_0309_BF1_{SHEN_NAME(BF1_)};
inline constexpr shen_name AREA_0309_BF7_{SHEN_NAME(BF7_)};
inline constexpr shen_name AREA_0309_BF8_{SHEN_NAME(BF8_)};
inline constexpr shen_name AREA_0309_KF5_{SHEN_NAME(KF5_)};
inline constexpr shen_name AREA_0309_BF9_{SHEN_NAME(BF9_)};
inline constexpr shen_name AREA_0309_KR3_{SHEN_NAME(KR3_)};
inline constexpr shen_name AREA_0309_CHARACTERS[]{
    AREA_0309_BF1_, AREA_0309_BF7_, AREA_0309_BF8_, AREA_0309_KF5_, AREA_0309_BF9_, AREA_0309_KR3_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0309_MOTION[] = "0309A.MOT", AREA_0309_CONVERSATION[] = "0309";
inline constexpr const char *AREA_0309_DEBUG_NAMES[]{"0309A000.c\n"};
inline constexpr Vector3 AREA_0309_MAP_POSITION{.x = 0.0f, .y = std::bit_cast<float>(0x4281999aU), .z = 0.0f};
inline constexpr shen_name AREA_0309_ELEVATOR{SHEN_NAME(R599)};
struct Area0309ElevatorFrame {
    shen_name elevator;
    std::int32_t frame;
    std::int32_t floor;
};
static_assert(sizeof(Area0309ElevatorFrame) == 12);
inline constexpr Area0309ElevatorFrame AREA_0309_ELEVATOR_FRAMES[]{
    {.elevator = AREA_0309_ELEVATOR, .frame = 0, .floor = 0},
    {.elevator = AREA_0309_ELEVATOR, .frame = 30, .floor = 1},
    {.elevator = AREA_0309_ELEVATOR, .frame = 60, .floor = 2},
    {.elevator = AREA_0309_ELEVATOR, .frame = 90, .floor = 3},
    {.elevator = AREA_0309_ELEVATOR, .frame = 120, .floor = 4},
    {.elevator = AREA_0309_ELEVATOR, .frame = 240, .floor = 4},
    {.elevator = AREA_0309_ELEVATOR, .frame = 270, .floor = 5},
    {.elevator = AREA_0309_ELEVATOR, .frame = 300, .floor = 6},
    {.elevator = AREA_0309_ELEVATOR, .frame = 330, .floor = 7},
    {.elevator = AREA_0309_ELEVATOR, .frame = 360, .floor = 8},
    {.elevator = AREA_0309_ELEVATOR, .frame = 410, .floor = 16},
    {.elevator = AREA_0309_ELEVATOR, .frame = 440, .floor = 17},
    {.elevator = AREA_0309_ELEVATOR, .frame = 470, .floor = 18},
    {.elevator = AUTHOR_VIEWER_INVALID_CHARACTER, .frame = 0, .floor = 0},
};
void Setup0309();
void Initialize0309();
void Start0309(std::uintptr_t argument);
void Update0309(void *taskData);
void Cleanup0309(AuthorViewerTaskParameter *parent);
std::int64_t Update0309Author(AuthorViewerLoopTaskParameter *child);
void Update0309AuthorTask(void *taskData);
void Prepare0309Characters();
void Restore0309Characters();
extern const ScriptAreaDefinition area0309ScriptDefinition;

} // namespace shenmue::s2::areas
