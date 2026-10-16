#pragma once
#include "s2/pati.hpp"
#include <cstddef>
#include <cstdint>
#include <span>
namespace shenmue::s2 {
struct PatiNailNode {
    std::uint32_t reserved00;
    Vector3 position;
    Vector3i rotation;
    Vector3 scale;
    std::uint64_t reserved28;
};
static_assert(sizeof(PatiNailNode) == 0x30);
static_assert(offsetof(PatiNailNode, position) == 4);
static_assert(offsetof(PatiNailNode, rotation) == 0x10);
static_assert(offsetof(PatiNailNode, scale) == 0x1c);
#pragma pack(push,1)
struct PatiNailData {
    std::byte header[0x30];
    PatiNailNode nails[512];
    shen_name character;
};
#pragma pack(pop)
static_assert(sizeof(PatiNailData) == 0x6034);
static_assert(offsetof(PatiNailData, nails) == 0x30);
static_assert(offsetof(PatiNailData, character) == 0x6030);
inline constexpr std::size_t PATI_NAIL_DATA_BYTES = 0x6034;
void DrawPatiNailsCallback(void *parameter);
std::uint64_t ParsePatiNailData(PatiNailData *destination, std::span<const std::byte> source);
std::uint64_t ParsePatiNailFile(PatiNailData *destination, const char *resource);
extern PatiModelCache *g_patiPocketModelCache;
MT7Model *LoadPocketModel(const char *name);
}
