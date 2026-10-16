#include "s1/game_utils.hpp"

#include "d3t/ui_view.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/hlib.hpp"
#include "s1/mt5_types.hpp"
#include "s1/task/char.hpp"
#include "s1/task/scen.hpp"

namespace shenmue::s1 {

std::array<d3t::UIWindow *, static_cast<std::size_t>(QteMinigameIndicator::Count)> g_qteMinigameIndicatorWindows{};
Mt5RuntimeNode *g_qteMinigameIndicatorModel = nullptr;
GameUtilsTitlIndicatorState g_gameUtilsTitlIndicatorEnabled{};
GameUtilsTitlIndicatorState *g_gameUtilsTitlIndicatorEnabledPtr = nullptr;

namespace {

constexpr std::uint32_t kTitlOwner = SHEN_NAME(TITL);
constexpr std::uint32_t kQtemOwner = SHEN_NAME(QTEM);
constexpr std::uint32_t kTitlLeftNode = 0x124;
constexpr std::uint32_t kTitlCenterNode = 0x125;
constexpr std::uint32_t kTitlRightNode = 0x126;
constexpr std::int16_t kExciteButtonNode = 183;
constexpr std::int16_t kExciteDirectionNode = 187;

void SetIndicatorVisibility(QteMinigameIndicator indicator, float visibility) {
    if (d3t::UIWindow *const window = g_qteMinigameIndicatorWindows[static_cast<std::size_t>(indicator)])
        window->alpha = visibility;
}

template <typename T> const T *ScriptData(std::uint32_t offset) {
    return offset ? reinterpret_cast<const T *>(g_pCurrentTaskData + static_cast<std::int32_t>(offset)) : nullptr;
}

} // namespace

void GameUtils_UpdateTitlIndicators() {
    if (Mt5RuntimeNode *const model = g_qteMinigameIndicatorModel) {
        for (std::int16_t button = 0; button < 4; ++button) {
            Mt5RuntimeNode *const node = shenGetNodeByChildIdx(model, static_cast<std::int16_t>(kExciteButtonNode + button));
            if (node && node->position.z > 0.001f) {
                node->position.y = -100.0f;
                SetIndicatorVisibility(static_cast<QteMinigameIndicator>(button), 1.0f);
            }
        }
        Mt5RuntimeNode *const direction = shenGetNodeByChildIdx(model, kExciteDirectionNode);
        if (direction && direction->position.z > 0.001f) {
            const float degrees = static_cast<float>(direction->rotation.z) / 182.04445f;
            direction->position.y = -100.0f;
            QteMinigameIndicator indicator = QteMinigameIndicator::ExciteUp;
            if (degrees >= -135.0f && degrees <= -45.0f)
                indicator = QteMinigameIndicator::ExciteRight;
            else if (degrees >= 45.0f && degrees <= 135.0f)
                indicator = QteMinigameIndicator::ExciteLeft;
            else if (degrees >= 135.0f && degrees <= 225.0f)
                indicator = QteMinigameIndicator::ExciteDown;
            SetIndicatorVisibility(indicator, 1.0f);
        }
    }
    if (const GameUtilsTitlIndicatorState *const enabled = g_gameUtilsTitlIndicatorEnabledPtr) {
        if (enabled->left)
            SetIndicatorVisibility(QteMinigameIndicator::TitleA, 1.0f);
        if (enabled->center)
            SetIndicatorVisibility(QteMinigameIndicator::TitleB, 1.0f);
        if (enabled->right)
            SetIndicatorVisibility(QteMinigameIndicator::TitleX, 1.0f);
    }
}

void GameUtils_ClearTitlIndicators() {
    for (d3t::UIWindow *const window : g_qteMinigameIndicatorWindows) {
        if (window)
            window->alpha = 0.0f;
    }
}

void Script_GameUtils(ScenarioContext *context, void *rawParams) {
    const auto *const params = static_cast<const ScriptGameUtilsParams *>(rawParams);
    Character *const owner = SCEN_FindCharacterByFunctionId(context, params->ownerTaskId);
    UIBaseRuntime &qte = g_uiElements[kQteMinigameUiElement];
    const ScriptGameUtilsArgs &args = params->args;
    switch (params->command) {
    case GameUtilsCommand::CommitStagedEntries:
        GameUtils_ClearTitlIndicators();
        qte.isHandledExternally = 1;
        if (qte.elementDraw)
            qte.elementDraw();
        GAME_CommitStagedEntries(owner);
        break;
    case GameUtilsCommand::StageNodeData:
        if (params->ownerTaskId == kTitlOwner) {
            g_gameUtilsTitlIndicatorEnabled.left = args.stageNode.nodeId == kTitlLeftNode;
            g_gameUtilsTitlIndicatorEnabled.center = args.stageNode.nodeId == kTitlCenterNode;
            g_gameUtilsTitlIndicatorEnabled.right = args.stageNode.nodeId == kTitlRightNode;
            qte.isHandledExternally = 0;
            if (qte.elementDraw)
                qte.elementDraw();
            g_gameUtilsTitlIndicatorEnabledPtr = &g_gameUtilsTitlIndicatorEnabled;
            GameUtils_UpdateTitlIndicators();
            break;
        }
        if (params->ownerTaskId == kQtemOwner && owner && owner->model) {
            g_qteMinigameIndicatorModel = static_cast<Mt5RuntimeNode *>(owner->model);
            qte.isHandledExternally = 0;
            if (qte.elementDraw)
                qte.elementDraw();
        }
        GAME_StageNodeData(owner, args.stageNode.nodeId, ScriptData<Vector3>(args.stageNode.positionOffset), args.stageNode.scale,
            ScriptData<Vector3>(args.stageNode.basisOffset));
        break;
    case GameUtilsCommand::ConfigureNumericDisplay:
        if (const Vector3 *const digitStep = ScriptData<Vector3>(args.configureNumericDisplay.digitStepOffset)) {
            GAME_ConfigureNumericDisplay(owner, args.configureNumericDisplay.baseNodeId, *digitStep, args.configureNumericDisplay.maxValue,
                static_cast<std::int32_t>(args.configureNumericDisplay.decimalShift));
        }
        break;
    case GameUtilsCommand::StageNumericDisplayValue:
        GAME_StageNumericDisplayValue(owner, args.stageNumericDisplayValue.value, ScriptData<Vector3>(args.stageNumericDisplayValue.digitOriginOffset),
            args.stageNumericDisplayValue.scale, ScriptData<Vector3>(args.stageNumericDisplayValue.basisOffset));
        break;
    default:
        break;
    }
}

} // namespace shenmue::s1
