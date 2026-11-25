#pragma once

#include "s2/search_runtime_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

struct ConversationBuffer;
struct SearchChoiceOptions {
    std::int16_t first, second;
    std::int32_t style;
};
typedef char Layout_search_commands_dc_0[(sizeof(SearchChoiceOptions) == 8) ? 1 : -1];
static const SearchChoiceOptions searchChoiceOptions[2] = {
    {SHEN_FIELD(first) 82, SHEN_FIELD(second) 83, SHEN_FIELD(style) 0}, {SHEN_FIELD(first) 80, SHEN_FIELD(second) 81, SHEN_FIELD(style) 0}};
extern ConversationBuffer *searchDialogue, *randomSearchDialogue;
extern char randomSearchBank[32];
typedef std::int32_t (*SearchCommand)(SearchTaskData &search);
extern SearchCommand const searchCommands[26];
bool srchLockTarget(SearchTaskData &search, shen_name name, bool faceTarget);
void srchRequestRelease();
void srchGetLookBase(Vector3 *base);
bool srchIsPropIKSettled();
bool srchIsPropIKMoving();
bool srchIsPropIKActive();
void srchBindPropIK(SearchTaskData &search);
void srchFinishPropIKItem(std::int32_t held);
void srchBeginPropIKReturn();
void srchRestartPropIK();
bool camCtrlIsTransitionActive();
std::int32_t shenEvaluateFlagFor(std::int32_t queryId, std::int8_t queryArgument);
void EnableLightEntry(std::uint32_t index);
void DisableLightEntry(std::uint32_t index);
ConversationBuffer *EV_PlayDialogue(const char *code);
void CloseMoneyIcon();
void KeepMoneyIconOpen();

} // namespace s2
} // namespace shenmue

#endif
