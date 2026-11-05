#include "gfx/standalone.h"

#if defined(SHENMUE_GFX_STANDALONE)
#pragma region StandaloneGraphics

typedef unsigned char BYTE;

typedef struct GfxShader {
    const char *path;
    int stage;
    const unsigned char *bytecode;
    size_t size;
} GfxShader;

#include "gfx_shaders.h"

static int gfxShaderPathEquals(const char *left, const char *right) {
    while (*left && *right) {
        int a = (unsigned char)*left++;
        int b = (unsigned char)*right++;
        if (a >= 'A' && a <= 'Z')
            a += 'a' - 'A';
        if (b >= 'A' && b <= 'Z')
            b += 'a' - 'A';
        if (a != b)
            return 0;
    }
    return *left == *right;
}

int gfxGetShader(const char *path, int stage, const void **bytecode, size_t *size) {
    size_t index;
    if (!path || !bytecode || !size)
        return 0;
    *bytecode = NULL;
    *size = 0;
    for (index = 0; index < sizeof(gfxShaders) / sizeof(gfxShaders[0]); ++index) {
        const GfxShader *shader = &gfxShaders[index];
        if (shader->stage == stage && gfxShaderPathEquals(path, shader->path)) {
            *bytecode = shader->bytecode;
            *size = shader->size;
            return 1;
        }
    }
    return 0;
}

#pragma endregion
#endif
