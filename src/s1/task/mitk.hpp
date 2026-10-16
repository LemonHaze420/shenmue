#pragma once

#include "s1/hlib.hpp"
#include "s1/task/scen.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

enum class MITKCommand : std::int32_t {
    StartFadeIn = 0,
    StartFadeOut = 1,
};

#pragma pack(push, 1)
struct MITKParameters {
    MITKCommand command;
    std::int8_t timeOfDay;
    std::byte reserved05[3];
    std::int16_t fadeFrames;
};
#pragma pack(pop)

static_assert(sizeof(MITKParameters) == 0x0a);
static_assert(offsetof(MITKParameters, command) == 0x00);
static_assert(offsetof(MITKParameters, timeOfDay) == 0x04);
static_assert(offsetof(MITKParameters, fadeFrames) == 0x08);



extern HLib::Task *g_MITK_Task;
extern std::array<std::int32_t, 18> g_memoryLoadingIconTexHandles;
extern std::int16_t g_memoryLoadingIconAlpha;
extern std::int16_t g_memoryLoadingIconAlphaStep;
extern std::int8_t g_memoryLoadingIconTimeOfDay;
extern std::uint8_t g_memoryLoadingIconFadeIn;

void EnqueueMITK(std::int8_t timeOfDay, std::int16_t fadeFrames);
void MITK_Callback(HLib::TaskData *taskData);
void MITK_Destroy();
void Script_SetMITKFlags(ScenarioContext *context, void *rawParams);

struct MemoryQuadLayout {
    std::int16_t centerX;
    std::int16_t centerY;
    float scaleX;
    float scaleY;
    std::uint32_t flags;
};



inline constexpr std::array<MemoryQuadLayout, 20> kMemoryLayouts{{
    {160, 120, 20.0f, 15.0f, 0x200},
    {480, 120, 20.0f, 15.0f, 0x300},
    {160, 360, 20.0f, 15.0f, 0x000},
    {480, 360, 20.0f, 15.0f, 0x100},

    {120, 100, 15.0f, 10.0f, 0x200},
    {520, 100, 15.0f, 10.0f, 0x300},
    {120, 380, 15.0f, 10.0f, 0x000},
    {520, 380, 15.0f, 10.0f, 0x100},

    {120, 110, 17.5f, 12.5f, 0x200},
    {520, 110, 17.5f, 12.5f, 0x300},
    {120, 370, 17.5f, 12.5f, 0x000},
    {520, 370, 17.5f, 12.5f, 0x100},

    {160, 120, 20.0f, 15.0f, 0x200},
    {480, 120, 20.0f, 15.0f, 0x300},
    {160, 360, 20.0f, 15.0f, 0x000},
    {480, 360, 20.0f, 15.0f, 0x100},

    {64, 84, 14.0f, 10.0f, 0x200},
    {288, 84, 14.0f, 10.0f, 0x300},
    {64, 244, 14.0f, 10.0f, 0x000},
    {288, 244, 14.0f, 10.0f, 0x100},
}};

inline constexpr std::array<MemoryQuadLayout, 20> kMemoryLayoutsPV{{
    {160, 120, 20.0f, 15.0f, 0x200},
    {480, 120, 20.0f, 15.0f, 0x300},
    {160, 360, 20.0f, 15.0f, 0x000},
    {480, 360, 20.0f, 15.0f, 0x100},

    {120, 80, 15.0f, 10.0f, 0x200},
    {520, 80, 15.0f, 10.0f, 0x300},
    {120, 400, 15.0f, 10.0f, 0x000},
    {520, 400, 15.0f, 10.0f, 0x100},

    {120, 100, 17.5f, 12.5f, 0x200},
    {520, 100, 17.5f, 12.5f, 0x300},
    {120, 380, 17.5f, 12.5f, 0x000},
    {520, 380, 17.5f, 12.5f, 0x100},

    {160, 120, 20.0f, 15.0f, 0x200},
    {480, 120, 20.0f, 15.0f, 0x300},
    {160, 360, 20.0f, 15.0f, 0x000},
    {480, 360, 20.0f, 15.0f, 0x100},

    {64, 84, 14.0f, 10.0f, 0x200},
    {288, 84, 14.0f, 10.0f, 0x300},
    {64, 244, 14.0f, 10.0f, 0x000},
    {288, 244, 14.0f, 10.0f, 0x100},
}};

} // namespace shenmue::s1
