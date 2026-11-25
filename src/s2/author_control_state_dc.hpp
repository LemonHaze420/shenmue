#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include <cstddef>
#include <cstdint>

#define SHENMUE_AUTH_STATE_CONSTANT static const

namespace shenmue { namespace s2 {

struct AuthorController;
SHENMUE_AUTH_STATE_CONSTANT std::size_t kAuthorLoadedFlagOffset = 4;
extern AuthorController *g_authorController;
void authResetLoadFlag();
std::int32_t authGetLoaded();

} } // namespace shenmue::s2

#undef SHENMUE_AUTH_STATE_CONSTANT

#endif
