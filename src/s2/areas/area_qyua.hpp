#pragma once
#include "s2/area_event.hpp"
#include "s2/date_time.hpp"
#include "s2/event_characters.hpp"
#include <cstddef>
#include "s2/area_names.hpp"
namespace shenmue::s2 {
inline constexpr shen_name QYUA_EVENT_TASK{SHEN_NAME(EVNT)}, QYUA_PLAYER{SHEN_NAME(RYO_)};
inline constexpr shen_name QYUA_REN{SHEN_NAME(REN_)}, QYUA_YUAN{SHEN_NAME(YUA_)}, QYUA_TOGYU{SHEN_NAME(TGY_)};
inline constexpr shen_name QYUA_KNIFE{SHEN_NAME(KNIF)}, QYUA_AUTHOR_KNIFE{SHEN_NAME(a514)}, QYUA_BASE{SHEN_NAME(BASE)};
inline constexpr shen_name QYUA_DUMMY_RYO{SHEN_NAME(DUMR)}, QYUA_DUMMY_REN{SHEN_NAME(DUML)}, QYUA_DUMMY_YUAN{SHEN_NAME(DUMY)};
inline constexpr shen_name QYUA_DUMMY_TOGYU{SHEN_NAME(DUMT)}, QYUA_AUTHOR_START{SHEN_NAME(ATH0)};
inline constexpr shen_name QYUA_AUTHOR_SUCCESS{SHEN_NAME(ATH1)}, QYUA_AUTHOR_FAILURE{SHEN_NAME(ATH2)}, QYUA_INVALID{0xffffffffU};
inline constexpr shen_name QYUA_CHARACTERS[]{QYUA_PLAYER, QYUA_REN, QYUA_YUAN, QYUA_TOGYU, QYUA_INVALID};
inline constexpr Vector3 QYUA_BASE_POSITION{218.1999969482422F, 110.80000305175781F, 217.60000610351562F};
inline constexpr Vector3 QYUA_KNIFE_POSITION{0.07178399711847305F, -0.02375599928200245F, -0.010230000130832195F};
inline constexpr Vector3i QYUA_KNIFE_ROTATION{-16690, 2613, -11462};
inline constexpr std::byte QYUA_DEFAULT_ARGUMENTS[]{std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0},
    std::byte{0}, std::byte{0x8e}, std::byte{0x43}, std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0}};
struct QyuaEventWork {
    EventReadCharactersWork *characters;
    std::uint64_t reserved08;
    HLib::Task *self;
    HLib::TaskCallback nextCallback;
    std::int32_t succeeded;
    std::uint32_t reserved24;
    const void *arguments;
    std::int32_t authorHasPlayed;
    std::uint32_t reserved34;
};
static_assert(sizeof(QyuaEventWork) == 0x38);
static_assert(offsetof(QyuaEventWork, self) == 0x10);
static_assert(offsetof(QyuaEventWork, nextCallback) == 0x18);
static_assert(offsetof(QyuaEventWork, succeeded) == 0x20);
static_assert(offsetof(QyuaEventWork, arguments) == 0x28);
static_assert(offsetof(QyuaEventWork, authorHasPlayed) == 0x30);
extern DateTime qyuaStartDate;
void QyuaInit();
void QyuaMain(std::uintptr_t argument);
void InitializeQyuaResources(QyuaEventWork *);
void QyuaStartCallback(void *);
void QyuaWaitOpeningAuthorCallback(void *);
void QyuaFirstResultCallback(void *);
void QyuaSecondResultCallback(void *);
void QyuaThirdResultCallback(void *);
void QyuaFailureAuthorCallback(void *);
void QyuaSuccessAuthorCallback(void *);
void QyuaWaitResultAuthorCallback(void *);
void QyuaRetryOrFinishCallback(void *);
void FinishQyuaEvent(QyuaEventWork *);
void RestoreQyuaResources(QyuaEventWork *);
std::int32_t BeginQyuaKnifeQte();
std::int32_t BeginQyuaSecondKnifeQte();
std::int32_t BeginQyuaThirdKnifeQte();
std::int32_t PlayQyuaFirstKnifeFailure();
std::int32_t PlayQyuaSecondKnifeFailure();
std::int32_t PlayQyuaKnifeSuccess();
extern const ScriptAreaDefinition qyuaScriptDefinition;
} // namespace shenmue::s2
