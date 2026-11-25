#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/types.hpp"
#include <stddef.h>

namespace shenmue {
namespace s2 {
extern std::uint16_t g_memoPage;
extern std::int16_t g_memoFlag, g_memoPreviousFlag;
extern std::byte *g_systemTalkData;
extern std::int16_t g_memoLatestPage;
void InitializeMemoPageState(std::int16_t page, std::int16_t side);
bool NotifyMemoFlag(std::int32_t flag, bool selected);

#define LCD_MEMORY_STORAGE ::shenmue::shen_name(SHEN_NAME(LCDM))
#define SYSTEM_TALK_STORAGE ::shenmue::shen_name(SHEN_NAME(SCST))
#define ARM_MOTION_STORAGE ::shenmue::shen_name(SHEN_NAME(ARMM))
#define WEATHER_DATA_STORAGE ::shenmue::shen_name(SHEN_NAME(WEDT))
#define MEMO_WORK_STORAGE ::shenmue::shen_name(SHEN_NAME(MWRK))
#define FONT_STORAGE ::shenmue::shen_name(SHEN_NAME(FONT))
#define FLAG_DESCRIPTOR_STORAGE ::shenmue::shen_name(SHEN_NAME(FLGB))
#define FLAG_WORK_STORAGE ::shenmue::shen_name(SHEN_NAME(FLGW))
static const std::size_t kArmMotionMemoryBytes = 0x8400;
static const std::size_t kFlagWorkBytes = 0xbb6;
extern std::byte *g_armMotionMemory;
extern std::byte *g_memoShenmue1Table;
extern std::uint32_t g_memoShenmue1RecordCount;
void AllocateARMM();

} // namespace s2
} // namespace shenmue

#endif
