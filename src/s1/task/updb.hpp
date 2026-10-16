#pragma once

#include "s1/hlib.hpp"
#include <array>

namespace shenmue::s1 {

extern std::uint8_t g_scenarioStepDebugDurationsEnabled;

void RequestStep(std::int16_t requestedStep) noexcept;
void UPDB_Callback(HLib::TaskData *taskData);
void UpdateScenarioProgressStep();
HLib::Task *UPDB_Init();
void UPDB_Shutdown();
HLib::Task *UPDB_GetTask();

struct ScenarioStepStatMapEntry {
    std::int16_t stepCode;
    std::int16_t statBit;
};

inline constexpr std::array<ScenarioStepStatMapEntry, 81> kScenarioStepStatMap{{
    {979, 979},
    {241, 550},
    {234, 545},
    {233, 540},
    {232, 535},
    {231, 530},
    {225, 527},
    {224, 525},
    {223, 520},
    {222, 510},
    {221, 500},
    {218, 480},
    {217, 475},
    {216, 470},
    {215, 460},
    {214, 450},
    {213, 440},
    {212, 430},
    {211, 420},
    {203, 415},
    {202, 410},
    {201, 400},
    {193, 395},
    {192, 390},
    {191, 380},
    {181, 370},
    {176, 360},
    {175, 350},
    {174, 345},
    {173, 340},
    {172, 330},
    {171, 320},
    {164, 310},
    {163, 305},
    {162, 300},
    {161, 290},
    {154, 287},
    {153, 285},
    {152, 283},
    {151, 280},
    {141, 270},
    {133, 266},
    {132, 263},
    {131, 260},
    {125, 250},
    {124, 249},
    {123, 248},
    {122, 245},
    {121, 242},
    {114, 240},
    {113, 230},
    {112, 220},
    {111, 210},
    {102, 200},
    {101, 190},
    {91, 180},
    {84, 170},
    {83, 168},
    {82, 165},
    {81, 160},
    {71, 155},
    {62, 150},
    {61, 140},
    {53, 130},
    {52, 120},
    {51, 110},
    {43, 106},
    {42, 103},
    {41, 100},
    {33, 90},
    {32, 80},
    {31, 70},
    {24, 65},
    {23, 60},
    {22, 50},
    {21, 40},
    {14, 30},
    {13, 20},
    {12, 10},
    {11, 5},
    {0, 0},
}};

} // namespace shenmue::s1
