#include "s1/task/effe_internal.hpp"

namespace shenmue::s1::effe_detail {

std::array<EffeTaskRecord *, kEffeSlots> &FamilyTasks(EffeFamily family) {
    return family == EffeFamily::Special ? g_specialTasks : family == EffeFamily::Efpt ? g_efptTasks : g_genericTasks;
}

EffeTaskRecord *TaskAt(EffeFamily family, std::int32_t slot) {
    return slot >= 0 && slot < static_cast<std::int32_t>(kEffeSlots) ? FamilyTasks(family)[static_cast<std::size_t>(slot)] : nullptr;
}

void ResetEffeDescriptor(EffeRuntimeTexture &runtime) {
    const std::uint32_t first = ((runtime.flags & 1U) << 29U) | 0x80000002U;
    runtime.descriptor[0] = first;
    runtime.descriptor[1] = ((first & 0xfU) | 0xfffffe20U) << 22U;
    runtime.descriptor[2] = 0x208000c0U;
    runtime.descriptor[3] = 0;
}

void ReleaseEffeTexture(std::int32_t runtimeSlot) {
    if (runtimeSlot < 0 || runtimeSlot >= static_cast<std::int32_t>(g_effeRuntime.size()))
        return;
    EffeRuntimeTexture &runtime = g_effeRuntime[static_cast<std::size_t>(runtimeSlot)];
    if (!runtime.resource)
        return;
    if (runtime.resource->textureHandle) {
        InitTexture2DResource(runtime.resource->textureHandle);
        runtime.resource->textureHandle = 0;
        runtime.resource->residentHandle = 0;
        --g_TextureRefCount;
    }
    runtime.resource = nullptr;
    ResetEffeDescriptor(runtime);
}

std::uint32_t CreateEffeTexture(std::uint32_t width, std::uint32_t height, const void *pixels) {
    if (!width || !height)
        return 0;

    const Texture2DDescription description{
        width,
        height,
        1,
        1,
        2,
        0,
        0,
        1,
        0,
        0,
        pixels,
    };
    return CreateTexture2D(&description);
}

void AttachEffeTexture(std::int32_t runtimeSlot, std::uint32_t handle, const void *source, bool spectrum, std::uint32_t width, std::uint32_t height) {
    EffeRuntimeTexture &runtime = g_effeRuntime[static_cast<std::size_t>(runtimeSlot)];
    ReleaseEffeTexture(runtimeSlot);
    runtime.pvrSource = source;
    if (!handle)
        return;
    EffeTextureResource &resource = g_effeResources[static_cast<std::size_t>(runtimeSlot)];
    resource = {};
    resource.textureHandle = handle;
    resource.residentHandle = handle;
    runtime.resource = &resource;
    runtime.flags |= 0x50000000U;
    runtime.descriptor[0] = 0x8200000aU;
    runtime.descriptor[1] = 0x0000808aU;
    std::uint32_t widthBits = 0;
    for (std::uint32_t value = width; value; value >>= 1U)
        ++widthBits;
    std::uint32_t heightBits = 0;
    for (std::uint32_t value = height; value; value >>= 1U)
        ++heightBits;
    runtime.descriptor[2] = (spectrum ? 0x941222c0U : 0x941242c0U) | (8U * widthBits - 32U) | (heightBits - 4U);
    runtime.descriptor[3] = handle;
    ++g_TextureRefCount;
}

std::size_t EffectAllocationSize(const void *allocation) {
    if (!allocation)
        return 0;
    const auto *header = static_cast<const HLib::TaskData *>(allocation) - 1;
    return header->content_size >= sizeof(HLib::TaskData) ? static_cast<std::size_t>(header->content_size - sizeof(HLib::TaskData)) : 0;
}

const EffePvrHeader *FindEffePvr(const void *allocation) {
    const std::size_t size = EffectAllocationSize(allocation);
    const auto *bytes = static_cast<const std::byte *>(allocation);
    for (std::size_t offset = 0; bytes && offset + sizeof(EffePvrHeader) <= size; ++offset) {
        std::uint32_t magic = 0;
        std::memcpy(&magic, bytes + offset, sizeof(magic));
        if (magic == 0x54525650U)
            return reinterpret_cast<const EffePvrHeader *>(bytes + offset);
    }
    return nullptr;
}

std::uint32_t CreatePvrEffeTexture(const EffePvrHeader *pvr) {
    if (!pvr || pvr->pixelFormat > 4 || !pvr->width || !pvr->height)
        return 0;
    const std::size_t row = static_cast<std::size_t>(pvr->width) * sizeof(std::uint32_t);
    const std::size_t size = row * pvr->height;
    auto *scratch = static_cast<std::byte *>(HLib_AllocHeapMemBlock(static_cast<std::int64_t>(size * 2U), static_cast<std::int32_t>(SHEN_NAME(EFPT))));
    if (!scratch)
        return 0;
    PVR_DecodeMain(scratch, pvr);
    std::byte *flipped = scratch + size;
    for (std::uint32_t rowIndex = 0; rowIndex < pvr->height; ++rowIndex)
        std::memcpy(flipped + static_cast<std::size_t>(rowIndex) * row, scratch + static_cast<std::size_t>(pvr->height - rowIndex - 1U) * row, row);
    const std::uint32_t handle = CreateEffeTexture(pvr->width, pvr->height, flipped);
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(scratch), 0);
    return handle;
}

} // namespace shenmue::s1::effe_detail

namespace shenmue::s1 {

void EFFE_AttachPvrTexture(std::int32_t slot, const void *pvrt, const void *source, bool spectrum, std::uint32_t width, std::uint32_t height) {
    AttachEffeTexture(slot, CreatePvrEffeTexture(static_cast<const EffePvrHeader *>(pvrt)), source, spectrum, width, height);
}

} // namespace shenmue::s1

namespace shenmue::s1::effe_detail {

void LoadSakuraResource() {
    g_sakuraStrengthBase = 0;
    g_sakuraRotation = 0.0f;
    g_sakuraRotationBase = 0.0f;
    g_sakuraRotationScale = 0.0001f;
    g_sakuraStrength = 0;
    void *const file = ReadFile("model/object", "sakura.pvr", static_cast<std::int32_t>(SHEN_NAME(TAKI)));
    const std::size_t bytes = EffectAllocationSize(file);
    g_sakuraTasks.fill(nullptr);
    g_sakuraPvr = bytes > 12U ? static_cast<const std::byte *>(file) + 12U : nullptr;
    g_sakuraPvrBytes = bytes > 12U ? bytes - 12U : 0U;
}

} // namespace shenmue::s1::effe_detail
