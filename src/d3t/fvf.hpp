#pragma once

#include "api/render.h"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::d3t {

using FVFElement = ShenRenderVertexElement;
using FVFDescription = ShenRenderVertexLayout;

static_assert(sizeof(FVFElement) == 0x20);
static_assert(offsetof(FVFElement, inputClassification) == 0x10);
static_assert(offsetof(FVFElement, inputSlot) == 0x14);
static_assert(offsetof(FVFElement, semanticHash) == 0x18);
static_assert(sizeof(FVFDescription) == 0x408);

std::uint32_t CreateFVF(const FVFDescription *description);
const FVFDescription *GetFVFDescription(std::uint32_t handle, std::uint32_t *index = nullptr);

} // namespace shenmue::d3t
