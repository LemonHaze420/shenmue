#pragma once

#include "s1/hlib.hpp"

#include <array>
#include <cstdint>

namespace shenmue::s1 {

inline constexpr std::int32_t kEwzoUiElementIndex = 43;
inline constexpr const char *kEwzoUiElementName = "UI_Game_Move_Learned";
inline constexpr std::array<const char *, 3> kEwzoUiLabelNames{"Text_Line_1", "Text_Line_2", "Text_Line_3"};

enum class EwzoUiEvent : std::uint32_t {
    Activate = 0,
    BindLabels = 1,
    Close = 2,
};

struct EwzoUiBinding {
    std::uint8_t mode;
    std::uint8_t animationSetId;
    std::uint16_t moveId;
    const std::array<std::int16_t, 3> *surfaceIds;
    const std::array<char, 256> *trainingText;
    const std::array<char, 256> *moveNameText;
    const std::array<char, 256> *resultText;
};

// The signature mirrors the UI owner's transition result; EWZO owns its
// task lifetime independently.
using EwzoUiHandler = std::int64_t (*)(EwzoUiEvent event, const EwzoUiBinding &binding);

extern HLib::Task *g_pMovePopupTask;
extern std::uint8_t g_pendingMovePopupAnimSetId;
extern std::uint16_t g_pendingMovePopupMoveId;
extern std::array<std::int16_t, 3> g_movePopupSurfaceIds;
extern std::array<char, 256> g_movePopupTrainingText;
extern std::array<char, 256> g_movePopupMoveNameText;
extern std::array<char, 256> g_movePopupResultText;

void EWZO_SetUiHandler(EwzoUiHandler handler);
EwzoUiBinding EWZO_GetUiBinding();
void EnqueueEWZO(std::uint8_t mode, std::uint16_t animationSetId, std::uint16_t moveId);
void EWZO_Callback(HLib::TaskData *taskData);
void EWZO_Destroy();

} // namespace shenmue::s1
