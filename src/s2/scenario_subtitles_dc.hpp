#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/hlib.hpp"

#define SHENMUE_SCENARIO_SUBTITLE_LAYOUT_NAME_(line) ShenmueScenarioSubtitleLayoutLayoutCheck##line
#define SHENMUE_SCENARIO_SUBTITLE_LAYOUT_NAME(line) SHENMUE_SCENARIO_SUBTITLE_LAYOUT_NAME_(line)
#define SHENMUE_SCENARIO_SUBTITLE_LAYOUT(condition) typedef char SHENMUE_SCENARIO_SUBTITLE_LAYOUT_NAME(__LINE__)[(condition) ? 1 : -1]

namespace shenmue {
namespace s2 {

struct ScenarioTextDecoder {
    const std::uint8_t *cursor;
    std::uint32_t legacyEncodingMode;
    std::int32_t (*isLeadByte)(std::uint32_t character);
    std::int32_t (*isTrailByte)(std::uint32_t character);
    std::uint16_t pendingPrefix;
    std::uint16_t retained12;
    std::uint32_t currentCharacterWidth;
    std::uint16_t currentCharacter;
    std::uint16_t retained1a;
};
struct ScenarioWindowTextState {
    std::int32_t columnCount;
    std::int32_t rowCount;
    ScenarioTextDecoder decoder;
    std::uint32_t scenarioStateFlags;
    std::int32_t surfaceRecoveryFrames;
    std::int32_t surfaceReady;
    std::int32_t callbackEnabled;
    std::int32_t lastWindowEvent;
};
SHENMUE_SCENARIO_SUBTITLE_LAYOUT(sizeof(ScenarioTextDecoder) == 0x1c);
SHENMUE_SCENARIO_SUBTITLE_LAYOUT(offsetof(ScenarioTextDecoder, isLeadByte) == 8);
SHENMUE_SCENARIO_SUBTITLE_LAYOUT(offsetof(ScenarioTextDecoder, pendingPrefix) == 0x10);
SHENMUE_SCENARIO_SUBTITLE_LAYOUT(offsetof(ScenarioTextDecoder, currentCharacterWidth) == 0x14);
SHENMUE_SCENARIO_SUBTITLE_LAYOUT(offsetof(ScenarioTextDecoder, currentCharacter) == 0x18);
SHENMUE_SCENARIO_SUBTITLE_LAYOUT(sizeof(ScenarioWindowTextState) == 0x38);
SHENMUE_SCENARIO_SUBTITLE_LAYOUT(offsetof(ScenarioWindowTextState, decoder) == 8);
SHENMUE_SCENARIO_SUBTITLE_LAYOUT(offsetof(ScenarioWindowTextState, scenarioStateFlags) == 0x24);
SHENMUE_SCENARIO_SUBTITLE_LAYOUT(offsetof(ScenarioWindowTextState, surfaceRecoveryFrames) == 0x28);
SHENMUE_SCENARIO_SUBTITLE_LAYOUT(offsetof(ScenarioWindowTextState, surfaceReady) == 0x2c);
SHENMUE_SCENARIO_SUBTITLE_LAYOUT(offsetof(ScenarioWindowTextState, callbackEnabled) == 0x30);
SHENMUE_SCENARIO_SUBTITLE_LAYOUT(offsetof(ScenarioWindowTextState, lastWindowEvent) == 0x34);

#define SHENMUE_SCENARIO_TEXT_DATA static const
#define SHENMUE_SCENARIO_TEXT_POINTERS static
#define SHENMUE_SCENARIO_TEXT_POINTER_CONST const
#define SHENMUE_SCENARIO_TEXT_INIT =

SHENMUE_SCENARIO_TEXT_DATA std::uint16_t kScenarioLineStartForbidden0[] SHENMUE_SCENARIO_TEXT_INIT{0x2139, 0x2465, 0x2529, 0x2576, 0};
SHENMUE_SCENARIO_TEXT_DATA std::uint16_t kScenarioLineStartForbidden1[] SHENMUE_SCENARIO_TEXT_INIT{0x2427, 0x2443, 0x2523, 0};
SHENMUE_SCENARIO_TEXT_DATA std::uint16_t kScenarioLineStartForbidden2[] SHENMUE_SCENARIO_TEXT_INIT{0x2421, 0x2467, 0x2563, 0x2126, 0};
SHENMUE_SCENARIO_TEXT_DATA std::uint16_t kScenarioLineStartForbidden3[] SHENMUE_SCENARIO_TEXT_INIT{0x2429, 0x2525, 0x213c, 0};
SHENMUE_SCENARIO_TEXT_DATA std::uint16_t kScenarioLineStartForbidden4[] SHENMUE_SCENARIO_TEXT_INIT{0x2423, 0x2565, 0x2144, 0x213d, 0};
SHENMUE_SCENARIO_TEXT_DATA std::uint16_t kScenarioLineStartForbidden5[] SHENMUE_SCENARIO_TEXT_INIT{0x2122, 0x2463, 0x2527, 0x2543, 0x2129, 0};
SHENMUE_SCENARIO_TEXT_DATA std::uint16_t kScenarioLineStartForbidden6[] SHENMUE_SCENARIO_TEXT_INIT{0x2123, 0x2425, 0x2521, 0x2567, 0x212a, 0};
SHENMUE_SCENARIO_TEXT_POINTERS const std::uint16_t *SHENMUE_SCENARIO_TEXT_POINTER_CONST kScenarioLineStartForbidden[7] SHENMUE_SCENARIO_TEXT_INIT{
    kScenarioLineStartForbidden0, kScenarioLineStartForbidden1, kScenarioLineStartForbidden2, kScenarioLineStartForbidden3, kScenarioLineStartForbidden4,
    kScenarioLineStartForbidden5, kScenarioLineStartForbidden6};
SHENMUE_SCENARIO_TEXT_DATA std::uint16_t kScenarioTextOpeningQuote = 0x2156, kScenarioTextClosingQuote = 0x2157, kScenarioTextLineBreak = 0x2175;
SHENMUE_SCENARIO_TEXT_DATA std::uint16_t kScenarioTextEscape = 0x217c, kScenarioTextColourMode1 = 0x4044, kScenarioTextColourMode0 = 0x4056;

#undef SHENMUE_SCENARIO_TEXT_DATA
#undef SHENMUE_SCENARIO_TEXT_POINTERS
#undef SHENMUE_SCENARIO_TEXT_POINTER_CONST
#undef SHENMUE_SCENARIO_TEXT_INIT

struct WindowSlot;
std::int32_t IsScenarioShiftJisLeadByte(std::uint32_t character);
std::int32_t IsScenarioShiftJisTrailByte(std::uint32_t character);
std::int32_t IsScenarioEucTrailByte(std::uint32_t character);
std::uint16_t ConvertWindowShiftJisCharacter(std::uint16_t character);
void InitializeScenarioTextDecoder(std::int32_t mode, const char *text, ScenarioTextDecoder &decoder);
std::int32_t AdvanceScenarioTextDecoder(ScenarioTextDecoder &decoder);
std::int32_t FindScenarioTextOpeningQuote(std::int32_t mode, ScenarioTextDecoder &decoder);
bool IsScenarioTextLineBreak(std::uint32_t mode, std::uint16_t character);
bool IsScenarioTextQuote(std::uint32_t mode, std::uint16_t character);
bool IsScenarioLineStartForbidden(std::uint32_t mode, std::uint16_t character);
std::uint32_t GetScenarioTextControlCode(std::uint32_t mode, std::uint16_t character);
void SCNF_RenderWindowText(WindowSlot *window, ScenarioWindowTextState *state);
void SCNF_Handler(WindowSlot *window, std::int32_t event, void *payload);
void RequestScenarioWindowRecovery(HLib::Task *scenarioTask);
void shenPrepareDialogue(HLib::Task *scenarioTask, std::int32_t mode, const char *text, std::int32_t speakerMode);

} // namespace s2
} // namespace shenmue
#undef SHENMUE_SCENARIO_SUBTITLE_LAYOUT_NAME
#undef SHENMUE_SCENARIO_SUBTITLE_LAYOUT_NAME_
#undef SHENMUE_SCENARIO_SUBTITLE_LAYOUT

#endif
