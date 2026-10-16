#include "s2/pati_nails.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/map_info.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/world_render.hpp"
#include <cstring>
namespace shenmue::s2 {
PatiModelCache *g_patiPocketModelCache{};

void DrawPatiNailsCallback(void *parameter) {
    auto &data = *static_cast<PatiNailData *>(parameter);
    for (const auto &nail : data.nails)
        if (nail.position.y > 0.0f)
            DrawPIN(data.character, 1, &nail.position, nullptr, &nail.scale);
}

std::uint64_t ParsePatiNailData(PatiNailData *destination, std::span<const std::byte> source) {
    const auto words = source.size() / 4;
    std::uint32_t identifier;
    std::memcpy(&identifier, source.data(), 4);
    if (identifier == SHEN_NAME(NAIL)) {
        for (std::size_t index = 0; index + 1 < words; ++index) {
            std::int16_t x, y;
            std::memcpy(&x, source.data() + 4 + index * 4, 2);
            std::memcpy(&y, source.data() + 6 + index * 4, 2);
            destination->nails[index].position.x = static_cast<float>(x) * 0.0005000000237487257f;
            destination->nails[index].position.y = static_cast<float>(y) * 0.0005000000237487257f;
        }
        return 48 * words + 4;
    }
    std::memcpy(destination, source.data(), words * 4);
    return words;
}
std::uint64_t ParsePatiNailFile(PatiNailData *destination, const char *resource) {
    EV_NPC_NewLoadSw(0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("PATI"), 0);
    if (std::strlen(resource) >= 8) {
        const auto *bytes = static_cast<const std::byte *>(LoadCurrentPakBinary(resource));
        return ParsePatiNailData(destination, {bytes, static_cast<std::size_t>(GetCurrentPakBinarySize(resource))});
    }
    auto *control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    auto *chunk = GetNativeChunkSizeField(resource, static_cast<MapInfoChunk *>(control->mapInfo));
    return ParsePatiNailData(destination, {reinterpret_cast<const std::byte *>(chunk + 1), *chunk});
}


MT7Model *LoadPocketModel(const char *name) {
    return LoadPatiModel(name, 0, g_patiPocketModelCache);
}
}
