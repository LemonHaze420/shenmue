#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/wbbk_carry.hpp"
#include "s2/areas/wbbk_models.hpp"
#include "s2/areas/wbbk_presentation.hpp"
#include "s2/search.hpp"
#include <cstdint>
namespace shenmue::s2::areas {
struct BookEventPhase {
    std::int32_t phase, frame;
};
static_assert(sizeof(BookEventPhase) == 8);
extern BookEventPhase bookEventPhase;
extern std::int8_t bookEventState, bookCompletion;
extern std::int32_t bookStoryVariant, bookIntroWait, bookEntrySelection;
extern std::int16_t bookCarryExitTicks;
extern std::int32_t bookCarryIdleFrames;
extern Vector3 bookCompanionTarget, bookCarryAnchor;
extern float bookCarryAnchorBlend;
extern SearchTarget bookSearchTargets[15];
extern std::uint8_t bookAvailableStacks[14];
void ChangeBookEventState(std::int8_t state);
void BookMainCallback(void *);
void BookIntroduction();
void BookPickUpStack();
void BookLeaveShelf();
void ResetBookMotionLayers();
void SelectBookStoryVariant();
void BookPickupCameraCallback(void *);
void BookIdleInput();
void UpdateBookCompanionPosition();
void UpdateBookCarryAnchor();
void ResetBookRound();
void FinishBookRound();
void InitializeBookCarryState();
void InitializeBookActors();
void CleanupBookActors();
bool IsBookTimeExpired();
void RecordBookCount(std::int32_t books, std::int32_t failures);
void CompleteBookWork();
extern const ScriptAreaDefinition wbbkScriptDefinition;
} // namespace shenmue::s2::areas
