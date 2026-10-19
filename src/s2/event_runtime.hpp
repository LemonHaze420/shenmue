#pragma once
#include "shenmue.h"
#include "s2/event_transform_flags.hpp"

#include "common/math.hpp"
#include "s2/hlib.hpp"
#include "s2/conversation.hpp"
#include "s2/flag.hpp"
#include "common/types.hpp"

#include <cstdint>
#include <array>
#include <cstddef>
#include <span>

#include "s2/scenario.hpp"
#include "s2/scenario_characters.hpp"
#include "s2/scenario_free_talk.hpp"
#include "s2/scenario_scnt.hpp"
#include "s2/scenario_actions.hpp"
#include "s2/scenario_window.hpp"
#include "s2/scenario_flags.hpp"
#include "s2/scenario_event_objects.hpp"
#include "s2/scenario_sequence.hpp"
#include "s2/event_qte.hpp"
#include "s2/event_darts.hpp"
#include "s2/event_weather.hpp"
#include "s2/event_cloth.hpp"
#include "s2/event_character_api.hpp"
#include "s2/dynamic_tracks.hpp"
#include "s2/butterfly.hpp"

namespace shenmue::s2 {

extern DateTime eventTemporaryDateTime;
extern DateTime eventSavedCurrentDateTime;
extern Vector3 eventPigeonTestPosition;
extern std::array<char, 13> eventSavedBgmName, eventSavedSoundBankName;
extern std::uint64_t eventSavedSoundScene;
extern std::int32_t eventEscapeRoute;
void ResetCompiledEventState();
void ReleaseEventGameplayModeOwners() noexcept;



void QueueGakeEnableSkipCallback();
void GakeEnableSkip_Callback(void *taskData);
void EV_SetAlignedShadowRange(float range);
void EV_CleanupMapControlTask();
void EV_scenQueueLoad(std::int32_t act, shen_name scene, std::int32_t entry);

void EV_ApplyTranslationClockVisibility(std::uint32_t startParameters);
std::int64_t EV_LoadAudio(const char *bank, std::int64_t sceneIndex);
std::int64_t EV_SetNewSceneIndex();
void EV_GetMapEventTask(const MapEventCallbackEntry *callbacks = nullptr);
void EV_SetCinemaBarVisibility(FilmControlFlag mode, std::int32_t toggle);
void EV_SetCanDoAreaJumpTo(std::int32_t enabled);
std::uint64_t EV_CanDoAreaJump();
inline constexpr std::uint32_t LoadingPhaseFightInitMask = 0xe0;

} // namespace shenmue::s2
