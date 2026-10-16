#pragma once
#include "s2/hlib.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
struct WarehouseScriptHeader {
    std::int32_t taskOffset;
    std::uint32_t cursor;
    std::uint32_t ticks;
    std::uint32_t command;
};
static_assert(sizeof(WarehouseScriptHeader) == 0x10);
static_assert(offsetof(WarehouseScriptHeader, command) == 0x0c);
HLib::Task *QueueWarehouseScriptLoop(std::uint32_t *script);
void SeekWarehouseScriptLabel(std::uint32_t *script, std::uint16_t label);
std::int64_t ExecuteWarehouseScript(std::uint32_t *script);
void StopWarehouseSerifCallback(void *);
void LoadWarehouseAuthorPackages();
std::int64_t ReleaseWarehouseAuthorPackages();
void StartWarehouseExitAuthor(std::uint32_t author);
void HideWarehouseAuthorCharacter(shen_name character);
void ResetWarehouseLockonSequence();
std::int64_t StartWarehouseLockonSequence();
}
