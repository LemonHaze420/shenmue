#include "api/render.h"

#include "d3t/buffer.hpp"
#include "d3t/dx11_renderer.hpp"
#include "d3t/fvf.hpp"
#include "d3t/shader.hpp"
#include "d3t/texture.hpp"

#if defined(_WIN32)
#include <d3d11.h>
#endif

using namespace shenmue::d3t;

extern "C" {

int shen_render_open(const ShenRenderDisplayMode *display, void *window) {
#if defined(_WIN32)
    if (!display || !window)
        return 0;
    DX11_D3D11CreateDeviceAndSwapChain(display, window);
    return g_D3DDevice && GetNativeSwapChain();
#else
    (void)display;
    (void)window;
    return 0;
#endif
}

void shen_render_close(void) {
#if defined(_WIN32)
    ShutdownDX11();
#endif
}

void shen_render_resize(uint32_t width, uint32_t height) {
#if defined(_WIN32)
    DirectXUpdateWindowDimensions(width, height);
#else
    (void)width;
    (void)height;
#endif
}

void shen_render_present(int32_t mode) {
#if defined(_WIN32)
    PresentFrame(mode);
#else
    (void)mode;
#endif
}

void *shen_render_context(int32_t mode) {
#if defined(_WIN32)
    return DXGetPipelineContext(mode);
#else
    (void)mode;
    return nullptr;
#endif
}

void shen_render_finish(void *context) {
#if defined(_WIN32)
    if (context)
        DX11::WaitForCommandListFinish(static_cast<D3TDeviceContext *>(context));
#else
    (void)context;
#endif
}

int32_t shen_render_create_buffer(const ShenRenderBufferDesc *description) {
    if (!description)
        return 0;
    try {
        return CreateBuffer(description);
    } catch (...) {
        return 0;
    }
}

void shen_render_release_buffer(int32_t buffer) {
    ValidateAndCommitBuffer(buffer);
}

void *shen_render_map_buffer(void *context, int32_t buffer, int32_t mode, uint32_t offset) {
    return GetWriteMappedBufferPointer(context, buffer, mode, offset);
}

int64_t shen_render_upload_buffer(void *context, int32_t buffer) {
    return DX11_CopySceneNodeToGPU(context, buffer);
}

uint32_t shen_render_create_texture(const ShenRenderTextureDesc *description) {
    if (!description)
        return 0;
    try {
        return CreateTexture2D(description);
    } catch (...) {
        return 0;
    }
}

void shen_render_release_texture(uint32_t texture) {
    InitTexture2DResource(texture);
}

int shen_render_texture_description(uint32_t texture, ShenRenderTextureDesc *description) {
    return description && GetTexture2DDescription(texture, description);
}

uint32_t shen_render_create_shader(int32_t stage) {
    if (stage < SHEN_RENDER_VERTEX || stage > SHEN_RENDER_COMPUTE)
        return 0;
    try {
        return CreateShader(stage);
    } catch (...) {
        return 0;
    }
}

int shen_render_load_shader(uint32_t shader, const void *bytecode, uint64_t size, int32_t stage) {
    if (!bytecode || !size || stage < SHEN_RENDER_VERTEX || stage > SHEN_RENDER_COMPUTE)
        return 0;
    return CreateShaderResource(shader, bytecode, size, static_cast<ShaderType>(stage));
}

void shen_render_release_shader(uint32_t shader) {
    DestroyShader(shader);
}

uint32_t shen_render_create_layout(const ShenRenderVertexLayout *description) {
    return CreateFVF(description);
}

void *shen_render_map_vertices(void *context, uint32_t size) {
#if defined(_WIN32)
    return GetMappedConstantBuffer(static_cast<D3TDeviceContext *>(context), 0, size);
#else
    (void)context;
    (void)size;
    return nullptr;
#endif
}

void *shen_render_map_constants(void *context, int32_t stages, uint32_t slot, int32_t size) {
#if defined(_WIN32)
    return MapTempConstantBufferForWriteDiscard(static_cast<D3TDeviceContext *>(context), stages, slot, size);
#else
    (void)context;
    (void)stages;
    (void)slot;
    (void)size;
    return nullptr;
#endif
}

int shen_render_prepare(void *context) {
#if defined(_WIN32)
    D3TDeviceContext *pipeline = static_cast<D3TDeviceContext *>(context);
    if (!pipeline || !pipeline->deviceContext)
        return 0;
    DX11_ResetMappedResources(pipeline);
    DX11_UnmapAndSyncShaderBindings(pipeline);
    DX11_SetupShadersForDXAndSetViewports(pipeline);
    return 1;
#else
    (void)context;
    return 0;
#endif
}

int shen_render_draw(void *context, uint32_t count, uint32_t first) {
#if defined(_WIN32)
    D3TDeviceContext *pipeline = static_cast<D3TDeviceContext *>(context);
    if (!pipeline || !pipeline->deviceContext)
        return 0;
    static_cast<ID3D11DeviceContext *>(pipeline->deviceContext)->Draw(count, first);
    return 1;
#else
    (void)context;
    (void)count;
    (void)first;
    return 0;
#endif
}

int shen_render_draw_indexed(void *context, uint32_t count, uint32_t first, int32_t base_vertex) {
#if defined(_WIN32)
    D3TDeviceContext *pipeline = static_cast<D3TDeviceContext *>(context);
    if (!pipeline || !pipeline->deviceContext)
        return 0;
    static_cast<ID3D11DeviceContext *>(pipeline->deviceContext)->DrawIndexed(count, first, base_vertex);
    return 1;
#else
    (void)context;
    (void)count;
    (void)first;
    (void)base_vertex;
    return 0;
#endif
}

int shen_render_draw_indexed_instanced(void *context, uint32_t count, uint32_t instances, uint32_t first, int32_t base_vertex, uint32_t first_instance) {
#if defined(_WIN32)
    D3TDeviceContext *pipeline = static_cast<D3TDeviceContext *>(context);
    if (!pipeline || !pipeline->deviceContext)
        return 0;
    static_cast<ID3D11DeviceContext *>(pipeline->deviceContext)->DrawIndexedInstanced(count, instances, first, base_vertex, first_instance);
    return 1;
#else
    (void)context;
    (void)count;
    (void)instances;
    (void)first;
    (void)base_vertex;
    (void)first_instance;
    return 0;
#endif
}
}
