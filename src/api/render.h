#ifndef SHENMUE_API_RENDER_H
#define SHENMUE_API_RENDER_H

#include <stdint.h>

typedef struct ShenRenderBufferDesc {
    int32_t flags;
    int32_t size;
    int32_t count;
    int32_t flags3;
    const void *buffer;
    const char *name;
} ShenRenderBufferDesc;

typedef struct ShenRenderTextureDesc {
    uint32_t width;
    uint32_t height;
    uint32_t arraySize;
    uint32_t mipLevels;
    uint32_t format;
    uint32_t bindFlags;
    uint32_t flags;
    uint32_t sampleCount;
    uint32_t sampleQuality;
    uint32_t reserved_24;
    const void *initialData;
} ShenRenderTextureDesc;

typedef struct ShenRenderVertexElement {
    const char *semanticName;
    uint32_t semanticIndex;
    uint32_t format;
    uint32_t inputClassification;
    uint8_t inputSlot;
    uint8_t extraStride;
    uint16_t reserved_16;
    uint32_t semanticHash;
    uint32_t reserved_1c;
} ShenRenderVertexElement;

typedef struct ShenRenderVertexLayout {
    ShenRenderVertexElement elements[32];
    uint32_t elementCount;
    uint32_t reserved_404;
} ShenRenderVertexLayout;

typedef struct ShenRenderDisplayMode {
    uint32_t width;
    uint32_t height;
    uint32_t refreshRateNumerator;
    uint32_t refreshRateDenominator;
    char description[16];
} ShenRenderDisplayMode;

enum ShenRenderShaderStage { SHEN_RENDER_VERTEX = 0, SHEN_RENDER_PIXEL = 1, SHEN_RENDER_COMPUTE = 2 };

#ifdef __cplusplus
extern "C" {
#endif

int shen_render_open(const ShenRenderDisplayMode *display, void *window);
void shen_render_close(void);
void shen_render_resize(uint32_t width, uint32_t height);
void shen_render_present(int32_t mode);
void *shen_render_context(int32_t mode);
void shen_render_finish(void *context);

int32_t shen_render_create_buffer(const ShenRenderBufferDesc *description);
void shen_render_release_buffer(int32_t buffer);
void *shen_render_map_buffer(void *context, int32_t buffer, int32_t mode, uint32_t offset);
int64_t shen_render_upload_buffer(void *context, int32_t buffer);
uint32_t shen_render_create_texture(const ShenRenderTextureDesc *description);
void shen_render_release_texture(uint32_t texture);
int shen_render_texture_description(uint32_t texture, ShenRenderTextureDesc *description);
uint32_t shen_render_create_shader(int32_t stage);
int shen_render_load_shader(uint32_t shader, const void *bytecode, uint64_t size, int32_t stage);
void shen_render_release_shader(uint32_t shader);
uint32_t shen_render_create_layout(const ShenRenderVertexLayout *description);

void *shen_render_map_vertices(void *context, uint32_t size);
void *shen_render_map_constants(void *context, int32_t stages, uint32_t slot, int32_t size);
int shen_render_prepare(void *context);
int shen_render_draw(void *context, uint32_t count, uint32_t first);
int shen_render_draw_indexed(void *context, uint32_t count, uint32_t first, int32_t base_vertex);
int shen_render_draw_indexed_instanced(void *context, uint32_t count, uint32_t instances, uint32_t first, int32_t base_vertex, uint32_t first_instance);

#ifdef __cplusplus
}
#endif

#endif
