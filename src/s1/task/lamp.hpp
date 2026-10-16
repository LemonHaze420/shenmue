#pragma once

#include "common/types.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

namespace HLib {
struct TaskData;
}

struct Character;
struct PackModel;

inline constexpr std::uint32_t kLampComponentId = SHEN_NAME(LAMP);

#pragma pack(push, 1)
struct LampComponentPayload {
    PackModel *modelOwner;
    std::uint32_t lightIndex;
    std::uint32_t active;
    std::uint32_t glowLevel;
    std::uint32_t reserved_14;
};
#pragma pack(pop)

static_assert(sizeof(LampComponentPayload) == 0x18);
static_assert(offsetof(LampComponentPayload, lightIndex) == 0x08);
static_assert(offsetof(LampComponentPayload, active) == 0x0c);
static_assert(offsetof(LampComponentPayload, glowLevel) == 0x10);

LampComponentPayload *LAMP_GetWork(Character *character);
LampComponentPayload *LAMP_Init(Character *character);
void EnqueueLAMP(Character *character, void *payload);
void LAMP_Callback(HLib::TaskData *taskData);
void LAMP_Cleanup(Character *character, void *payload);
bool LAMP_SetScriptState(Character *character, std::uint32_t mode, std::uint32_t value);

} // namespace shenmue::s1
