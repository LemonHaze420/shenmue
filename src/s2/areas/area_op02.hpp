#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "s2/area_names.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name OP02_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name OP02_LOAD_TASK{SHEN_NAME(load)};
inline constexpr shen_name OP02_WAVE_TASK{SHEN_NAME(wave)};
inline constexpr shen_name OP02_PLAYER{SHEN_NAME(RYO_)};
inline constexpr shen_name OP02_SHENHUA{SHEN_NAME(SIN_)};
inline constexpr shen_name OP02_LIGHT_DAY{SHEN_NAME(l101)};
inline constexpr shen_name OP02_LIGHT_NIGHT{SHEN_NAME(l102)};
inline constexpr shen_name OP02_AUTHOR_CHARACTERS[]{OP02_SHENHUA, shen_name{0xffffffffU}};
inline constexpr shen_name OP02_SCENE_CHARACTERS[]{
    OP02_SHENHUA, SHEN_NAME(a350), SHEN_NAME(ab20) | 0x04000000U, SHEN_NAME(ab01) | 0x04020000U, SHEN_NAME(ab20) | 0x06000000U, SHEN_NAME(ab20) | 0x02000000U};
inline constexpr std::int32_t OP02_WAVE_SLOTS[]{9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
inline constexpr char OP02_AIX_PATH[] = "D:\\dolby\\S2_ADV.aix";
struct Op02EventWork {
    HLib::Task *task;
    std::int16_t mode;
    std::int16_t state;
    std::uint32_t reserved0c;
};
static_assert(sizeof(Op02EventWork) == 0x10);
static_assert(offsetof(Op02EventWork, mode) == 8 && offsetof(Op02EventWork, state) == 10);
void AreaOP02Init();
void AreaOP02Main(std::uintptr_t parameter);
void OP02MainCallback(void *data);
void OP02AuthorLoopCallback(void *data);
void OP02WaveCallback(void *data);
void OP02Prepare(Op02EventWork *work);
void OP02Release(Op02EventWork *work);
void OP02QueueAuthor(Op02EventWork *work);
void OP02PrepareSceneCharacters();
void OP02RestoreSceneCharacters();
void OP02SetHourAndMinute(std::int8_t hour, std::int8_t minute);
void OP02SetLightingAndWeather(std::int16_t mode);
void OP02ResetWeather();
extern const ScriptAreaDefinition areaOP02ScriptDefinition;
} // namespace shenmue::s2::areas
