#pragma once
#include "s2/area_event.hpp"
#include "common/math.hpp"
#include "s2/area_names.hpp"
namespace shenmue::s2 {
inline constexpr shen_name WONC_TASK{SHEN_NAME(wonc)}, WONC_PLAYER{SHEN_NAME(RYO_)};
inline constexpr shen_name WONC_WONG{SHEN_NAME(WON_)}, WONC_LILLY{SHEN_NAME(LLY_)}, WONC_SAM{SHEN_NAME(SAM_)};
inline constexpr shen_name WONC_CLIFF{SHEN_NAME(CLJ_)}, WONC_HIDDEN{SHEN_NAME(IS_8)};
inline constexpr shen_name WONC_TEMPORARY{SHEN_NAME(a023)};
inline constexpr shen_name WONC_AUT1{SHEN_NAME(AUT1)}, WONC_AUT2{SHEN_NAME(AUT2)}, WONC_AUT3{SHEN_NAME(AUT3)}, WONC_AUT4{SHEN_NAME(AUT4)};
inline constexpr shen_name WONC_INVALID{0xffffffffU};
inline constexpr shen_name WONC_ALL_CHARACTERS[]{WONC_WONG, WONC_CLIFF, WONC_LILLY, WONC_SAM, WONC_INVALID};
inline constexpr shen_name WONC_SECOND_CHARACTERS[]{WONC_WONG, WONC_LILLY, WONC_SAM, WONC_INVALID};
inline constexpr shen_name WONC_LAST_CHARACTERS[]{WONC_WONG, WONC_CLIFF, WONC_INVALID};
struct WoncAuthorDescriptor {
    const shen_name *characters;
    shen_name author;
    std::int32_t audioScene;
    const char *authorBank;
    const char *soundProgram;
    const char *package;
    std::int32_t npcLoad;
    std::int32_t playerYaw;
};
static_assert(sizeof(WoncAuthorDescriptor) == 48);
static_assert(offsetof(WoncAuthorDescriptor, author) == 8);
static_assert(offsetof(WoncAuthorDescriptor, playerYaw) == 44);
extern const WoncAuthorDescriptor woncAuthors[4];
extern const WoncAuthorDescriptor *woncCurrentAuthor;
extern std::int32_t woncPauseFrames;
extern Vector3i woncPlayerRotation;
void WoncInit();
void WoncMain(std::uintptr_t argument);
void WoncPauseCallback(void *);
void WoncPlayAuthorCallback(void *);
void PrepareWoncCharacters();
void WoncWaitAuthorCallback(void *);
void WoncCompleteAuthorCallback(void *);
void RestoreWoncCharacters();
void WoncEndCallback(void *);
void WoncEndFadeCallback(void *);
void WoncDormantNullsub();
extern const ScriptAreaDefinition woncScriptDefinition;
} // namespace shenmue::s2
