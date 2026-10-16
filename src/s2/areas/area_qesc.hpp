#pragma once

#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "s2/hlib.hpp"
#include "common/math.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2 {

inline constexpr shen_name QESC_TASK{SHEN_NAME(EVNT)}, QESC_RYO{SHEN_NAME(RYO_)};
inline constexpr shen_name QESC_BASE{SHEN_NAME(BASE)}, QESC_DUMMY{SHEN_NAME(DUMR)}, QESC_INVALID{0xffffffffU};
inline constexpr shen_name QESC_YUN{SHEN_NAME(YUN_)}, QESC_GOI{SHEN_NAME(GOI_)}, QESC_HTK{SHEN_NAME(HTK_)};
inline constexpr shen_name QESC_SAM{SHEN_NAME(SAM_)}, QESC_LLY{SHEN_NAME(LLY_)};
inline constexpr shen_name QESC_CHARACTERS[]{QESC_RYO, QESC_YUN, QESC_GOI, QESC_HTK, QESC_SAM, QESC_LLY, QESC_INVALID};
inline constexpr Vector4 QESC_LIGHT_COLOUR{.x = 0.0f, .y = 0.25f, .z = 0.24f, .w = 0.23f};
inline constexpr std::uint32_t QESC_DEFAULT_ARGUMENTS[]{0, 0, 0xc000, 0};
struct QescEventWork {
    EventReadCharactersWork *characters;
    HLib::Task *task;
    std::int32_t motion;
    std::byte reserved14[0x0c];
    const void *arguments;
};
static_assert(sizeof(QescEventWork) == 0x28);
static_assert(offsetof(QescEventWork, task) == 0x08);
static_assert(offsetof(QescEventWork, motion) == 0x10);
static_assert(offsetof(QescEventWork, arguments) == 0x20);
void QescInit();
void QescMain(std::uintptr_t argument);
void QueueQescEscapeEvent(void *argument);
void QescEscapeCallback(void *taskData);
void InitializeQescEscape(QescEventWork *work);
void QescDormantCallback(void *taskData);
extern const ScriptAreaDefinition qescScriptDefinition;

} // namespace shenmue::s2
