#pragma once

#include "common/math.hpp"
#include "common/types.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {
struct MT7Model;
struct MazeRoad;


struct MazeModelReader {
    const void *source{};
    MT7Model *data{};
};
static_assert(offsetof(MazeModelReader, data) == 8);

// S2 5b90f0 traverses 32-byte authored records; negative flags terminate.
struct MazeAuthoredObject {
    shen_name name{};
    std::uint32_t reserved04{};
    const char *resource{};
    MazeModelReader *modelReader{};
    std::uint32_t flags{};
    std::uint32_t reserved1c{};
};
static_assert(sizeof(MazeAuthoredObject) == 32);
static_assert(offsetof(MazeAuthoredObject, modelReader) == 16);
static_assert(offsetof(MazeAuthoredObject, flags) == 24);

void FISH_Draw(const MazeAuthoredObject &object, const MazeRoad &context);
} // namespace shenmue::s2
