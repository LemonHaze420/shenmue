#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"
namespace shenmue::s2::areas {
struct E0027TaskParameter {
    HLib::Task *self;
    std::uint32_t motion;
    std::int16_t selection, state;
    std::int16_t fadeFrame;
    std::byte reserved12[6];
    std::int32_t *requestCommand;
};
struct E0027LoopParameter {
    E0027TaskParameter *parent;
    std::int16_t frame;
    std::uint16_t reserved0a;
    std::uint32_t smoke;
};
static_assert(sizeof(E0027TaskParameter) == 0x20);
static_assert(offsetof(E0027TaskParameter, selection) == 0x0c);
static_assert(offsetof(E0027TaskParameter, fadeFrame) == 0x10);
static_assert(offsetof(E0027TaskParameter, requestCommand) == 0x18);
static_assert(sizeof(E0027LoopParameter) == 0x10);
static_assert(offsetof(E0027LoopParameter, smoke) == 0x0c);
extern const ScriptAreaDefinition area0027ScriptDefinition;
} // namespace shenmue::s2::areas
