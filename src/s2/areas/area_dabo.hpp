#pragma once
#include "s2/area_event.hpp"
#include "s2/hlib.hpp"
#include "s2/pati.hpp"
#include "s2/pati_nails.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
#pragma pack(push, 1)
struct DaboBoard {
    std::uint32_t reserved00, area, table;
    shen_name character;
    HLib::Task *task;
    std::int32_t state18, state1c;
    std::byte reserved20[8];
    std::int32_t ballState;
    std::byte reserved2c[0x60b4];
};
struct DaboPachi {
    DaboBoard *board;
    std::byte reserved08[8];
    HLib::Task *nailDrawTask;
    PatiTableData tables;
    float scale;
    std::byte reserved85b4[0x34];
    const char *field85e8;
    std::byte reserved85f0[0x18];
    float launcherHeight, launcherStep, launcherMinimum, launcherMaximum;
    std::byte reserved8618[0x10];
    const char *field8628;
    PatiNailData *nails;
    std::int16_t field8638;
    std::byte reserved863a[0xd6];
    const char *field8710;
    std::byte reserved8718[0xbc];
    std::uint16_t field87d4;
    std::byte reserved87d6[0x52a];
    std::int32_t cameraMode;
    std::byte reserved8d04[0xc];
};
struct DaboEventWork {
    HLib::Task *task;
    std::uint64_t counters[8];
    std::int32_t selectedBoard, previousBoard, boardSize, previousSize, gameType, previousType;
    DaboPachi *pachi;
    DaboPachi inlinePachi;
    DaboBoard inlineBoard;
};
#pragma pack(pop)
static_assert(sizeof(DaboBoard) == 0x60e0);
static_assert(sizeof(DaboPachi) == 0x8d10);
static_assert(offsetof(DaboPachi, tables) == 0x18);
static_assert(offsetof(DaboPachi, scale) == 0x85b0);
static_assert(offsetof(DaboPachi, nails) == 0x8630);
static_assert(offsetof(DaboPachi, cameraMode) == 0x8d00);
static_assert(sizeof(DaboEventWork) == 0xee58);
static_assert(offsetof(DaboEventWork, inlinePachi) == 0x68);
static_assert(offsetof(DaboEventWork, inlineBoard) == 0x8d78);
void DABO_init_func();
void DABO_main_func(std::uintptr_t);
void DaboDebugCallback(void *);
extern const ScriptAreaDefinition areaDABOScriptDefinition;
} // namespace shenmue::s2
