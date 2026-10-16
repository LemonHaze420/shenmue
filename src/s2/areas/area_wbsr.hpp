#pragma once
#include "s2/area_event.hpp"
#include "s2/area_names.hpp"
namespace shenmue::s2 {
void WbsrInit();
void WbsrMain(std::uintptr_t argument);
void WbsrInitializeCallback(void *);
void WbsrPackageWaitCallback(void *);
void WbsrAuthorWaitCallback(void *);
void WbsrChoiceWaitCallback(void *);
void WbsrScenarioACallback(void *);
void WbsrScenarioBCallback(void *);
void WbsrSelect1Callback(void *);
void WbsrSelect2Callback(void *);
void WbsrExit2Callback(void *);
void WbsrExit3Callback(void *);
void WbsrExit4Callback(void *);
void WbsrSceneChangeCallback(void *);
extern const ScriptAreaDefinition wbsrScriptDefinition;
} // namespace shenmue::s2
