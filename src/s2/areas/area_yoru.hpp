#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
namespace shenmue::s2 {
inline constexpr shen_name YORU_AREA{SHEN_NAME(YORU)}, YORU_TASK{SHEN_NAME(EVNT)}, YORU_SPRING_TASK{SHEN_NAME(ETST)};
inline constexpr shen_name YORU_SPRING_MAP{SHEN_NAME(0126)}, YORU_TOWER_MAP{SHEN_NAME(0313)}, YORU_PLAYER{SHEN_NAME(RYO_)};
inline constexpr shen_name YORU_LIGHT_ZERO{SHEN_NAME(0000)}, YORU_LIGHT_ONE{SHEN_NAME(0001)}, YORU_LIGHT_TWO{SHEN_NAME(0002)};
struct YoruAuthorWork {
    HLib::Task *task;
    std::int16_t authorCommand;
    std::int16_t state;
    std::uint32_t reserved0c;
};
static_assert(sizeof(YoruAuthorWork) == 0x10);
static_assert(offsetof(YoruAuthorWork, authorCommand) == 8 && offsetof(YoruAuthorWork, state) == 0x0a);
void YoruInit();
void YoruMain(std::uintptr_t);
void LoadYoruMapForEntry(std::int8_t);
void InitializeYoruNightScene();
void YoruAuthorCallback(void *);
void YoruAuthorLoopCallback(void *);
extern const ScriptAreaDefinition yoruScriptDefinition;
} // namespace shenmue::s2
