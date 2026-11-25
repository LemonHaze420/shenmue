#include "s2/game_system_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/sdk.hpp"
#include "s2/camera_dc.hpp"
#include "s2/conversation_dc.hpp"
#include "s2/game_save_area.hpp"
#include "s2/hlib.hpp"
#include "s2/input_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/system_modules_dc.hpp"
#include "s2/ui_state_dc.hpp"
#include "s2/screen_film.hpp"
#include "s2/texture_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/calendar_dc.hpp"
#include "s2/debug_screen_dc.hpp"
#include "s2/save_screen_state_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/story_state.hpp"
#include <cstdarg>
#include <cstdio>
#endif

namespace shenmue {
namespace s2 {

void IncrementFrameCount();

std::int32_t ctrlGetActive();

void UpdateGameSysActivity();

void UpdateGameInputSuppression();

SystemModule UpdateCurrentSceneInfoFromTask(std::int32_t &actNumber, shen_name &areaId, std::int32_t &entryId);

#if SHENMUE_IS_DC && SHENMUE_IS_S2
void CloseAllAudioFiles();
#endif

namespace {

extern "C" void ADXF_CloseAll();

} // namespace

// @verified - d
void GameSys_Callback(void *) {
    UpdateGameInputSuppression();
    UpdateGameSysActivity();
    if (ctrlGetActive()) {
        const SystemModule module = UpdateCurrentSceneInfoFromTask(g_actNumber, g_currentMap, g_entryPoint);
        StopConversationBuffers(0);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        CloseAllAudioFiles();
        HLib::MarkCurrentTaskForCleanup();
        if (static_cast<std::uint32_t>(GetIsWaitingFlag()) && (module == SystemModule_Game || module == SystemModule_OutRun || module == SystemModule_HangOn)) {
            const std::uint32_t colour = GetFadedClearColour();
            dc::sdk::njSetBackColor(colour, colour, colour);
        } else {
            const std::uint32_t colour = 0xff000000U;
            dc::sdk::njSetBackColor(colour, colour, colour);
        }
#else
        ADXF_CloseAll();
        HLib::EV_HLKillOtherHierarchyTask(HLib::CurrentTask);
        std::uint32_t colour = 0xff000000U;
        if (GetIsWaitingFlag() && (module == SystemModule_Game || module == SystemModule_OutRun || module == SystemModule_HangOn))
            colour = GetFadedClearColour();
        dc::sdk::njSetBackColor(colour, colour, colour);
#endif
        ChangeSystemModule(module, 0);
    }
    dc::sdk::RandomFloat();
    IncrementFrameCount();
}

} // namespace s2
} // namespace shenmue

#endif
