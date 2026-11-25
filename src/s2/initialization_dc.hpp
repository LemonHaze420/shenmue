#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/legacy_types.hpp"
#define SHENMUE_BOOT_DATA static const

namespace shenmue { namespace s2 {

SHENMUE_BOOT_DATA std::uintptr_t kTimerStartRegister = 0xffd80004;
SHENMUE_BOOT_DATA std::uintptr_t kTimerConstantRegister = 0xffd80008;
SHENMUE_BOOT_DATA std::uintptr_t kTimerControlRegister = 0xffd80010;
SHENMUE_BOOT_DATA std::uintptr_t kG2DsTimeoutRegister = 0xa05f7890;
SHENMUE_BOOT_DATA std::uintptr_t kG2TrTimeoutRegister = 0xa05f7894;
SHENMUE_BOOT_DATA std::uintptr_t kUncachedAlias = 0xa0000000;

extern std::int32_t g_spriteBufferCapacities[4];
void InitializeMemoryAndHLib();
void Initialization();
void InitializeSprites(std::int32_t useCurrentArea, std::int32_t initializeCommonTextures);
void InitializeTextureCellStorage();
void InitializeSystemLibrariesNullsub();
void ShutdownSystemLibrariesNullsub();
void ResetSceneRuntime();
void FinishAica();

} } // namespace shenmue::s2
#undef SHENMUE_BOOT_DATA

#endif
