#include "s2/pawn.hpp"

#include "s2/archive_data.hpp"
#include "s2/hlib.hpp"
#include "s2/inventory.hpp"
#include "s2/pak.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>

namespace shenmue::s2 {
namespace {

// InitDynamicSceneData: aligned static PAWN storage, immediately
// preceding the camera-data arena. This file is not an HLib allocation.
alignas(32) std::array<std::byte, 0x4040> pawnStorage{};
PawnDataHeader *pawnData = reinterpret_cast<PawnDataHeader *>(pawnStorage.data());

std::uintptr_t PawnHeapOffsetBase() {
    return reinterpret_cast<std::uintptr_t>(HLib::DynamicHeapBase) + 0x40;
}

template <typename T> T *ResolvePawnOffset(std::int32_t offset) {
    return offset ? reinterpret_cast<T *>(PawnHeapOffsetBase() + static_cast<std::uintptr_t>(static_cast<std::intptr_t>(offset))) : nullptr;
}

std::int32_t EncodePawnOffset(const void *pointer) {
    if (!pointer)
        return 0;
    const std::intptr_t offset = reinterpret_cast<std::intptr_t>(pointer) - static_cast<std::intptr_t>(PawnHeapOffsetBase());
    assert(offset >= std::numeric_limits<std::int32_t>::min() &&
           offset <= std::numeric_limits<std::int32_t>::max());
    return static_cast<std::int32_t>(offset);
}

void SortPawnSetAvailability(std::uint32_t *scores, std::int32_t *order, std::int32_t first, std::int32_t last) {
    // SortPawnSetAvailability retains descending unsigned keys and paired indices.
    while (first < last) {
        const std::uint32_t pivot = scores[(first + last) >> 1];
        std::int32_t low = first - 1;
        std::int32_t high = last + 1;
        for (;;) {
            do {
                ++low;
            } while (scores[low] > pivot);
            do {
                --high;
            } while (scores[high] < pivot);
            if (low >= high)
                break;
            const std::uint32_t score = scores[low];
            scores[low] = scores[high];
            scores[high] = score;
            const std::int32_t index = order[low];
            order[low] = order[high];
            order[high] = index;
        }
        SortPawnSetAvailability(scores, order, first, low - 1);
        first = high + 1;
    }
}

} // namespace

PawnShopRecord *FindPawnShop(PawnDataHeader *data, shen_name name) {
    // FindPawnShop retains first-match ordering, including duplicate shop IDs.
    if (!data)
        return nullptr;
    PawnShopRecord *shop = reinterpret_cast<PawnShopRecord *>(data + 1);
    for (std::int32_t index = 0; index < data->shopCount; ++index, ++shop) {
        if (shop->name.name == name.name)
            return shop;
    }
    return nullptr;
}

// @verified - p
PawnSetReference *GetPawnShopSetReferences(const PawnShopRecord *shop) {
    return ResolvePawnOffset<PawnSetReference>(shop->setsOffset);
}

PawnSetRecord *GetPawnShopSet(const PawnShopRecord *shop, std::int32_t index) {
    // GetPawnShopSet uses eight-byte table entries with four-byte offsets.
    if (!shop || index < 0 || index >= shop->setCount)
        return nullptr;
    PawnSetReference *references = GetPawnShopSetReferences(shop);
    return references ? ResolvePawnOffset<PawnSetRecord>(references[index].offset) : nullptr;
}

PawnDataHeader *GetPawnData() {
    return pawnData;
}
// @verified - p
PawnDataHeader *LoadPawnFile(const char *name) {
    // LoadPawnData fills and relocates the permanent buffer in place;
    // relocation does not allocate another file owner or mutate COLD storage.
    if (!name)
        return nullptr;
    if (!LoadLegacyFiles("MISC", name, pawnData, 0xffffffffU))
        return nullptr;
    if (pawnData->signature.name != SHEN_NAME(PAWN) || pawnData->version.name != SHEN_NAME(0201)) {
        HLib::FreeTaskData(pawnData);
        return nullptr;
    }

    PawnShopRecord *shop = reinterpret_cast<PawnShopRecord *>(pawnData + 1);
    PawnSetReference *setReferences = nullptr;
    PawnSetRecord *set = nullptr;
    std::byte *items = nullptr;
    for (std::int32_t index = 0; index < pawnData->shopCount; ++index, ++shop) {



        if (shop->setsOffset) {
            setReferences = reinterpret_cast<PawnSetReference *>(pawnStorage.data() + static_cast<std::uint32_t>(shop->setsOffset));
            for (std::int32_t item = 0; item < shop->setCount; ++item) {
                if (setReferences[item].offset) {
                    set = reinterpret_cast<PawnSetRecord *>(pawnStorage.data() + static_cast<std::uint32_t>(setReferences[item].offset));
                }
                setReferences[item].offset = EncodePawnOffset(set);
            }
        }
        if (shop->itemsOffset) {
            items = pawnStorage.data() + static_cast<std::uint32_t>(shop->itemsOffset);
        }
        shop->setsOffset = EncodePawnOffset(setReferences);
        shop->itemsOffset = EncodePawnOffset(items);
    }
    return pawnData;
}

// @verified - p
PawnDataHeader *LoadPawnData() {
    return LoadPawnFile("PAWNDATA.BIN");
}

void BuildPawnSetAvailability(const PawnShopRecord *shop, std::int32_t *order, std::uint32_t *scores, PawnStockCount *scratch) {
    // BuildPawnSetAvailability counts every occurrence, not merely distinct item availability.
    if (!shop || !order || !scores)
        return;
    std::int32_t count = 0;
    for (std::int32_t index = 0; index < shop->setCount; ++index) {
        const PawnSetRecord *const set = GetPawnShopSet(shop, index);
        if (!set)
            continue;
        for (std::int32_t item = 0; item <= set->itemCount; ++item) {
            scratch[item] = {.item = -1, .remaining = 0};
        }
        const std::int16_t *const items = reinterpret_cast<const std::int16_t *>(set + 1);
        std::int32_t distinct = 0;
        std::int32_t present = 0;
        for (std::int32_t item = 0; item < set->itemCount; ++item) {
            std::int32_t slot = 0;
            while (slot < distinct && scratch[slot].item != items[item])
                ++slot;
            if (slot == distinct) {
                scratch[slot].item = items[item];
                scratch[slot].remaining = static_cast<std::int32_t>(GetItemState(static_cast<std::uint32_t>(static_cast<std::int32_t>(items[item]))));
                ++distinct;
            }
            if (scratch[slot].remaining > 0) {
                --scratch[slot].remaining;
                ++present;
            }
        }
        order[count] = index;
        scores[count] = (static_cast<std::uint32_t>(shop->setCount - index) & 0xffffffU) | (static_cast<std::uint32_t>(255 * present / set->itemCount) << 24);
        ++count;
    }
    for (std::int32_t index = count; index < shop->setCount; ++index) {
        order[index] = -1;
        scores[index] = 0;
    }
    SortPawnSetAvailability(scores, order, 0, count - 1);
}

// @verified - p
std::uint32_t CalculatePawnSelectedItemsPrice(
    std::int32_t priceCount, const PawnItemPrice *prices, const std::uint16_t *items, const std::int16_t *quantities, std::uint32_t itemCount) {
    // CalculatePawnSelectedItemsPrice uses signed quantities/prices, first-match price and DWORD sum.
    std::uint32_t total = 0;
    if (priceCount > 0 && prices && items && quantities && static_cast<std::int32_t>(itemCount) > 0) {
        for (std::uint32_t item = 0; item < itemCount; ++item) {
            if (quantities[item] <= 0)
                continue;

            for (std::int32_t price = 0; price < priceCount; ++price) {
                if (prices[price].item != items[item])
                    continue;
                total += static_cast<std::uint32_t>(static_cast<std::int32_t>(quantities[item]) * prices[price].price);
                break;
            }
        }
    }
    return static_cast<std::int32_t>(total) > 0 ? total : 0xffffffffU;
}

} // namespace shenmue::s2
