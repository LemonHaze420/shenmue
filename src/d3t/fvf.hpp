#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::d3t {

struct FVFElement {
    const char *semanticName;
    std::uint32_t semanticIndex;
    std::uint32_t format;
    std::uint32_t inputClassification;
    std::uint8_t inputSlot;
    std::uint8_t extraStride;
    std::uint16_t reserved_16;
    std::uint32_t semanticHash;
    std::uint32_t reserved_1c;
};

struct FVFDescription {
    std::array<FVFElement, 32> elements;
    std::uint32_t elementCount;
    std::uint32_t reserved_404;
};

static_assert(sizeof(FVFElement) == 0x20);
static_assert(offsetof(FVFElement, inputClassification) == 0x10);
static_assert(offsetof(FVFElement, inputSlot) == 0x14);
static_assert(offsetof(FVFElement, semanticHash) == 0x18);
static_assert(sizeof(FVFDescription) == 0x408);

std::uint32_t CreateFVF(const FVFDescription *description);
const FVFDescription *GetFVFDescription(std::uint32_t handle, std::uint32_t *index = nullptr);

} // namespace shenmue::d3t
