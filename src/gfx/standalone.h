#pragma once

#include <stddef.h>

#if defined(SHENMUE_GFX_STANDALONE)
#ifdef __cplusplus
extern "C" {
#endif

int gfxGetShader(const char *path, int stage, const void **bytecode, size_t *size);

#ifdef __cplusplus
}
#endif
#endif
