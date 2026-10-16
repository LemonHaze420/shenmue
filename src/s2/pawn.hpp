#pragma once

#include "common/types.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

#pragma pack(push, 4)
struct PawnDataHeader {
    shen_name signature;
    shen_name version;
    std::int32_t shopCount;
};

struct PawnShopRecord {
    shen_name name;
    std::uint32_t flags;
    char label[24];
    std::int32_t setCount;
    std::int32_t setsOffset;
    std::int32_t itemCount;
    std::int32_t itemsOffset;
};

struct PawnSetReference {
    std::int32_t offset;
    std::int32_t price;
};

struct PawnSetRecord {
    char label[24];
    std::int32_t itemCount;
};

struct PawnItemPrice {
    std::uint16_t item;
    std::int16_t price;
};

struct PawnStockCount {
    std::int32_t item;
    std::int32_t remaining;
};
#pragma pack(pop)

static_assert(sizeof(PawnDataHeader) == 0x0c);
static_assert(sizeof(PawnShopRecord) == 0x30);
static_assert(offsetof(PawnShopRecord, setCount) == 0x20);
static_assert(offsetof(PawnShopRecord, setsOffset) == 0x24);
static_assert(offsetof(PawnShopRecord, itemCount) == 0x28);
static_assert(offsetof(PawnShopRecord, itemsOffset) == 0x2c);
static_assert(sizeof(PawnSetReference) == 8);
static_assert(sizeof(PawnSetRecord) == 0x1c);
static_assert(sizeof(PawnItemPrice) == 4);
static_assert(sizeof(PawnStockCount) == 8);

PawnDataHeader *LoadPawnFile(const char *filename);
PawnDataHeader *LoadPawnData();
PawnDataHeader *GetPawnData();
PawnShopRecord *FindPawnShop(PawnDataHeader *data, shen_name name);
PawnSetReference *GetPawnShopSetReferences(const PawnShopRecord *shop);
PawnSetRecord *GetPawnShopSet(const PawnShopRecord *shop, std::int32_t index);
void BuildPawnSetAvailability(const PawnShopRecord *shop, std::int32_t *order, std::uint32_t *scores, PawnStockCount *scratch);
std::uint32_t CalculatePawnSelectedItemsPrice(
    std::int32_t priceCount, const PawnItemPrice *prices, const std::uint16_t *items, const std::int16_t *quantities, std::uint32_t itemCount);

} // namespace shenmue::s2
