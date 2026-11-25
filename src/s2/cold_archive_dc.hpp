#pragma once

#include "s2/cold_archive.hpp"
#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

static const std::uintptr_t kColdArchiveAddress =
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    0x0c400000;
#else
    0x8c400000;
#endif
static const std::size_t kColdArchiveAllocationBytes = 0x500000;
static const std::uint32_t kColdArchiveSectionCount = 12;
#define COLD_ARCHIVE ::shenmue::shen_name(SHEN_NAME(COLD))

void LoadColdStorageFromMisc();

} } // namespace shenmue::s2

#endif
