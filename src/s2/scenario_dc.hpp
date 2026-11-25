#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/character_dc.hpp"
#include "s2/scenario_subtitles_dc.hpp"

#define SHENMUE_SCENARIO_LAYOUT_NAME_(line) SHENMUE_SCENARIOLayoutCheck##line
#define SHENMUE_SCENARIO_LAYOUT_NAME(line) SHENMUE_SCENARIO_LAYOUT_NAME_(line)
#define SHENMUE_SCENARIO_LAYOUT(condition) typedef char SHENMUE_SCENARIO_LAYOUT_NAME(__LINE__)[(condition) ? 1 : -1]
#define SHENMUE_SCENARIO_CONSTANT static const
namespace shenmue {
namespace s2 {

#define MEMORY_CHID (::shenmue::shen_name(SHEN_NAME4('C', 'H', 'I', 'D')))
#define SCENARIO_TASK (::shenmue::shen_name(SHEN_NAME4('S', 'C', 'E', 'N')))
enum ScenarioSubtitleFlag {
    SSF_TextComplete = 0x01,
    SSF_PageReady = 0x04,
    SSF_AdvanceRequested = 0x08,
    SSF_Suppressed = 0x10,
    SSF_InactiveMask = 0x12,
    SSF_ForceSubtitles = 0x40,
    SSF_Preparing = 0x80,
};
struct ScenarioCharacterAssignment {
    NameTreeNode tree;
    ScenarioCharacterAssignment *bucketNext;
    Character *character;
};
struct WindowSlot;
struct ScenarioWindowRecord;
struct ScenarioTaskData {
    shen_name name;
    ScenarioWindowRecord *scenarioWindow;
    void *scenarioEventRuntime, *scenarioControl;
    NameTreeNode *characterTree;
    ScenarioCharacterAssignment *characterBuckets[13];
    WindowSlot *windowDialog;
    ScenarioWindowTextState windowTextState;
    char subtitleText[0xbc];
    std::byte retainedScenarioData[0x100];
};
SHENMUE_SCENARIO_LAYOUT(sizeof(ScenarioCharacterAssignment) == 0x14);
SHENMUE_SCENARIO_LAYOUT(offsetof(ScenarioCharacterAssignment, bucketNext) == 0x0c);
SHENMUE_SCENARIO_LAYOUT(offsetof(ScenarioCharacterAssignment, character) == 0x10);
SHENMUE_SCENARIO_LAYOUT(sizeof(ScenarioTaskData) == 0x240);
SHENMUE_SCENARIO_LAYOUT(offsetof(ScenarioTaskData, scenarioWindow) == 4);
SHENMUE_SCENARIO_LAYOUT(offsetof(ScenarioTaskData, characterTree) == 0x10);
SHENMUE_SCENARIO_LAYOUT(offsetof(ScenarioTaskData, characterBuckets) == 0x14);
SHENMUE_SCENARIO_LAYOUT(offsetof(ScenarioTaskData, windowDialog) == 0x48);
SHENMUE_SCENARIO_LAYOUT(offsetof(ScenarioTaskData, windowTextState) == 0x4c);
SHENMUE_SCENARIO_LAYOUT(offsetof(ScenarioTaskData, windowTextState) + offsetof(ScenarioWindowTextState, scenarioStateFlags) == 0x70);
SHENMUE_SCENARIO_LAYOUT(offsetof(ScenarioTaskData, subtitleText) == 0x84);
SHENMUE_SCENARIO_LAYOUT(offsetof(ScenarioTaskData, retainedScenarioData) == 0x140);
HLib::Task *ScenarioTaskEntry_Enqueue(std::int8_t nextFunctionIndex);
void ScenarioTaskEntry_Callback(void *parameter);
void ctrlStopSubtitles();
void scenDestroySubtitles(HLib::Task *task);
void SCEN_ShowEventMessage(HLib::Task *scenarioTask, std::int32_t mode, const char *message);
void dbgPrintEventMessage(const char *message);
bool scenHasActiveSubtitles(HLib::Task *task);
void ForceScenarioSubtitles(HLib::Task *task);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void RequestForcedScenarioSubtitles(HLib::Task *task);
#endif
std::int32_t scenEvaluateCurrentSequence(shen_name name, std::int8_t mode);
void scenAssignFacialClip(HLib::Task *scenarioTask, const std::int16_t *clip, shen_name speaker);
void HideWindowSlot(WindowSlot *window);
void CreateScenarioCharacterId(HLib::Task *scenarioTask, shen_name name, Character *character);
Character *scenFindChara(HLib::Task *scenarioTask, shen_name name);
shen_name scenGetCharaName(HLib::Task *scenarioTask, const Character *character);
void UnassignScenarioCharacter(HLib::Task *scenarioTask, Character *character);

} // namespace s2
} // namespace shenmue
#undef SHENMUE_SCENARIO_LAYOUT_NAME_
#undef SHENMUE_SCENARIO_LAYOUT_NAME
#undef SHENMUE_SCENARIO_LAYOUT
#undef SHENMUE_SCENARIO_CONSTANT

#endif
