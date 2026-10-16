#pragma once

#include "s1/hlib.hpp"

namespace shenmue::s1 {
struct ScenarioContext;

extern HLib::Task *g_lghtTask;

HLib::Task *LGHT_InitializeTask();
void LGHT_TaskCallback(HLib::TaskData *taskData);
void Script_lightCreatePoint(ScenarioContext *context, void *arguments);
void Script_lightUtils(ScenarioContext *context, void *arguments);
void Script_GetCameraInputDetails(ScenarioContext *context, void *arguments);

} // namespace shenmue::s1
