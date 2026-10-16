#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::d3t {
struct UIWindow;
}

namespace shenmue::s1 {

struct Mt5RuntimeNode;
struct ScenarioContext;

enum class GameUtilsCommand : std::uint32_t {
    CommitStagedEntries = 0,
    StageNodeData = 1,
    ConfigureNumericDisplay = 2,
    StageNumericDisplayValue = 3,
};

// Script data pointers in these argument blocks are offsets into g_pCurrentTaskData, with zero meaning none.
struct GameUtilsStageNodeArgs {
    std::uint32_t nodeId;
    std::uint32_t positionOffset;
    float scale;
    std::uint32_t basisOffset;
};

struct GameUtilsConfigureNumericDisplayArgs {
    std::uint32_t baseNodeId;
    std::uint32_t digitStepOffset;
    std::uint32_t maxValue;
    std::uint32_t decimalShift;
};

struct GameUtilsStageNumericDisplayValueArgs {
    std::uint32_t value;
    std::uint32_t digitOriginOffset;
    float scale;
    std::uint32_t basisOffset;
};

union ScriptGameUtilsArgs {
    GameUtilsStageNodeArgs stageNode;
    GameUtilsConfigureNumericDisplayArgs configureNumericDisplay;
    GameUtilsStageNumericDisplayValueArgs stageNumericDisplayValue;
};

struct ScriptGameUtilsParams {
    std::uint32_t ownerTaskId;
    GameUtilsCommand command;
    ScriptGameUtilsArgs args;
};
static_assert(sizeof(ScriptGameUtilsParams) == 0x18);
static_assert(offsetof(ScriptGameUtilsParams, args) == 0x08);

// Which QTE Title answer panel the TITL cabinet lights.
struct GameUtilsTitlIndicatorState {
    std::int32_t left;
    std::int32_t center;
    std::int32_t right;
};
static_assert(sizeof(GameUtilsTitlIndicatorState) == 0x0c);

// STATIC_QTE_MiniGame_IsSelected resolves these QTEMinigame.ui panel objects in this order.
enum class QteMinigameIndicator : std::uint8_t {
    ExciteA = 0,
    ExciteB,
    ExciteX,
    ExciteY,
    ExciteLeft,
    ExciteUp,
    ExciteDown,
    ExciteRight,
    TitleA,
    TitleB,
    TitleX,
    Count,
};

inline constexpr std::array<const char *, static_cast<std::size_t>(QteMinigameIndicator::Count)> kQteMinigameIndicatorObjectNames{"QTEMinigame::Panel_Excite_A",
    "QTEMinigame::Panel_Excite_B", "QTEMinigame::Panel_Excite_X", "QTEMinigame::Panel_Excite_Y", "QTEMinigame::Panel_Excite_PL", "QTEMinigame::Panel_Excite_PU",
    "QTEMinigame::Panel_Excite_PD", "QTEMinigame::Panel_Excite_PR", "QTEMinigame::Panel_Title_A", "QTEMinigame::Panel_Title_B", "QTEMinigame::Panel_Title_X"};

// UI_QTE_MiniGame in the 80-entry element table.
inline constexpr std::size_t kQteMinigameUiElement = 0x44;



extern std::array<::shenmue::d3t::UIWindow *, static_cast<std::size_t>(QteMinigameIndicator::Count)> g_qteMinigameIndicatorWindows;

extern Mt5RuntimeNode *g_qteMinigameIndicatorModel;
extern GameUtilsTitlIndicatorState g_gameUtilsTitlIndicatorEnabled;
extern GameUtilsTitlIndicatorState *g_gameUtilsTitlIndicatorEnabledPtr;

void GameUtils_UpdateTitlIndicators();
void GameUtils_ClearTitlIndicators();
void Script_GameUtils(ScenarioContext *context, void *rawParams);

} // namespace shenmue::s1
