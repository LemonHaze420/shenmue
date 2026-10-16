#pragma once

#include <cstddef>
#include <cstdint>

namespace shenmue::d3t {

void *RetrieveHashedSamplerState(const std::uint64_t *flags);
void *RetrieveHashedBlendState(const std::byte *flags);
void *RetrieveHashedStencilState(const std::uint64_t *flags);
void *RetrieveHashedRasterizerState(const std::uint32_t *flags);
void ReleaseD3TPipelineStateCaches();

} // namespace shenmue::d3t
