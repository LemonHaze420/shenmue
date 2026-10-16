#include "s2/texture_source.hpp"

#include "d3t/archive_data.hpp"
#include "d3t/image_texture_loader.hpp"
#include "d3t/render_handles.hpp"
#include "d3t/texture.hpp"

#include <cstring>

namespace shenmue::s2 {
namespace {
constexpr std::size_t sourceCount = 4096;
constexpr std::uint32_t sourceMask = sourceCount - 1;
std::array<EffectTextureSource, sourceCount> sources{};
std::array<std::uint32_t, sourceCount> keys{};
std::array<std::uint16_t, sourceCount> deltas{};
std::uint32_t count{};
bool dirty{};

EffectTextureSource *FindSource(std::uint32_t key) {
    std::uint32_t index = key & sourceMask;
    for (;;) {
        if (keys[index] == key)
            return &sources[index];
        if (!deltas[index])
            return nullptr;
        index = (index + deltas[index]) & sourceMask;
    }
}

// Relative-probe chains retain empty interior slots.
bool InsertSource(std::uint32_t key, const EffectTextureSource &source) {
    if (count >= sourceCount)
        return false;
    std::uint32_t index = key & sourceMask;
    std::uint32_t firstEmpty = sourceCount;
    for (;;) {
        if (keys[index] == key)
            return true;
        if (!keys[index]) {
            if (firstEmpty == sourceCount)
                firstEmpty = index;
            if (!deltas[index]) {
                keys[index] = key;
                sources[index] = source;
                ++count;
                return true;
            }
        }
        if (!deltas[index])
            break;
        index = (index + deltas[index]) & sourceMask;
    }
    if (firstEmpty != sourceCount) {
        keys[firstEmpty] = key;
        sources[firstEmpty] = source;
        ++count;
        return true;
    }
    std::uint32_t free = (index + 1) & sourceMask;
    while (keys[free]) {
        free = (free + 1) & sourceMask;
        if (free == index)
            return false;
    }
    keys[free] = key;
    sources[free] = source;
    deltas[index] = static_cast<std::uint16_t>((free - index) & sourceMask);
    ++count;
    return true;
}

// Deleting a head/interior preserves its outgoing probe link.
void EraseSource(std::uint32_t key) {
    std::uint32_t index = key & sourceMask;
    std::uint32_t previous = index;
    while (keys[index] != key) {
        if (!deltas[index])
            return;
        previous = index;
        index = (index + deltas[index]) & sourceMask;
    }
    keys[index] = 0;
    sources[index] = {};
    if (previous != index && !deltas[index])
        deltas[previous] = 0;
    --count;
}

// A published GPU handle admits immediately; only a missing handle waits
// for render-task readiness before becoming unavailable.
bool ResolveSource(EffectTextureSource &source) {
    if (source.textureHandle) {
        d3t::d3t_CloseRenderHandle(source.renderHandle);
        source.state = 1;
        return true;
    }
    if (!source.state && !d3t::d3t_IsRenderHandlePending(source.renderHandle)) {
        d3t::d3t_CloseRenderHandle(source.renderHandle);
        source.reserved00 = 0;
        source.state = source.textureHandle ? 1 : 2;
        return source.state == 1;
    }
    return false;
}
} // namespace

bool CopyEffectTextureSource(std::uint32_t key, EffectTextureSource &source) {
    const EffectTextureSource *const found = FindSource(key);
    if (!found)
        return false;
    source = *found;
    return true;
}

// LoadImageTexture publishes its reserved generation synchronously;
// the copied descriptor owns no stack pointer.
bool LoadEffectTextureSource(const char *path, std::uint32_t key, std::uint32_t flags) {
    EffectTextureSource source{};
    if (CopyEffectTextureSource(key, source))
        return true;
    char cleaned[512]{'/'};
    std::strcat(cleaned, path);
    d3t::FilepathCleaner(cleaned);
    source.renderHandle = d3t::d3t_AllocateRenderHandle();
    d3t::LoadImageTexture(cleaned, true, &source.textureHandle, source.renderHandle);
    source.flags = flags;
    std::strcpy(source.path.data(), cleaned);
    const bool inserted = InsertSource(key, source);
    dirty = true;
    return inserted;
}

bool PumpEffectTextureSources() {
    if (!dirty)
        return false;
    bool settled = true;
    for (std::size_t index = 0; index < sourceCount; ++index) {
        if (keys[index] && !sources[index].state) {
            ResolveSource(sources[index]);
            settled &= sources[index].state != 0;
        }
    }
    dirty &= !settled;
    return dirty;
}

// Only admitted descriptors are swept. Pending/failed entries retain
// their state; direct HIKI/PVR handles are not registered here.
void SweepEffectTextureSources() {
    for (std::size_t index = 0; index < sourceCount; ++index) {
        if (keys[index] && sources[index].state == 1) {
            d3t::InitTexture2DResource(sources[index].textureHandle);
            EraseSource(keys[index]);
        }
    }
}

// Unknown keys and the flag-1 projection exclusion remain direct handles.
std::uint32_t ResolveEffectTextureSource(std::uint32_t key, std::uint32_t projectionMode) {
    EffectTextureSource *const source = FindSource(key);
    if (!source || ((projectionMode - 1U) > 2U && (source->flags & 1U)))
        return key;
    if ((source->state == 1 || (!source->state && ResolveSource(*source))) && source->state == 1)
        return source->textureHandle;
    return d3t::g_defaultWhiteTexture;
}

} // namespace shenmue::s2
