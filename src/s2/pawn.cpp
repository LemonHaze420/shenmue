#include "s2/pawn.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/file_loader_dc.hpp"
#include "s2/scene_data.hpp"
#else
#include "s2/archive_data.hpp"
#endif
#include "s2/hlib.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/inventory_dc.hpp"
#else
#include "s2/inventory.hpp"
#include "s2/pak.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#endif

namespace shenmue {
namespace s2 {

#if SHENMUE_IS_DC && SHENMUE_IS_S2
PawnDataHeader *LoadPawnFile(const char *filename);
#endif

namespace {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
// InitDynamicSceneData: aligned static PAWN storage, immediately
// preceding the camera-data arena. This file is not an HLib allocation.
alignas(32) std::array<std::byte, 0x4040> pawnStorage{};
PawnDataHeader *pawnData = reinterpret_cast<PawnDataHeader *>(pawnStorage.data());
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::uintptr_t PawnHeapOffsetBase() {
    return reinterpret_cast<std::uintptr_t>(HLib::DynamicHeapBase) + 0x40;
}
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
template <typename T> T *ResolvePawnOffset(std::int32_t value) {
    return reinterpret_cast<T *>(static_cast<std::uintptr_t>(value));
}
#else
template <typename T> T *ResolvePawnOffset(std::int32_t offset) {
    return offset ? reinterpret_cast<T *>(PawnHeapOffsetBase() + static_cast<std::uintptr_t>(static_cast<std::intptr_t>(offset))) : nullptr;
}
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
std::int32_t EncodePawnOffset(const void *pointer) {
    if (!pointer)
        return 0;
    const std::intptr_t offset = reinterpret_cast<std::intptr_t>(pointer) - static_cast<std::intptr_t>(PawnHeapOffsetBase());
    assert(offset >= std::numeric_limits<std::int32_t>::min() && offset <= std::numeric_limits<std::int32_t>::max());
    return static_cast<std::int32_t>(offset);
}
#endif

// @verified - d
void SortPawnSetAvailability(std::uint32_t *scores, std::int32_t *order, std::int32_t first, std::int32_t last) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (first < last) {
#else
    while (first < last) {
#endif
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
#if SHENMUE_IS_DC && SHENMUE_IS_S2
        do {
            SortPawnSetAvailability(scores, order, first, low - 1);
            SortPawnSetAvailability(scores, order, high + 1, last);
        } while (0);
#else
        SortPawnSetAvailability(scores, order, first, low - 1);
        first = high + 1;
#endif
    }
}

} // namespace

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
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
#endif

// @verified - d
PawnSetReference *GetPawnShopSetReferences(const PawnShopRecord *shop) {
    return ResolvePawnOffset<PawnSetReference>(shop->setsOffset);
}

// @verified - d
PawnSetRecord *GetPawnShopSet(const PawnShopRecord *shop, std::int32_t index) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (shop && index >= 0) {
        const PawnSetReference *const references = GetPawnShopSetReferences(shop);
        if (!references)
            return 0;
        if (index < shop->setCount)
            return ResolvePawnOffset<PawnSetRecord>(references[index].offset);
    }
    return 0;
#else
    // GetPawnShopSet uses eight-byte table entries with four-byte offsets.
    if (!shop || index < 0 || index >= shop->setCount)
        return nullptr;
    PawnSetReference *references = GetPawnShopSetReferences(shop);
    return references ? ResolvePawnOffset<PawnSetRecord>(references[index].offset) : nullptr;
#endif
}

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
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
        // Native locals survive zero offsets, including across shop rows.
        // Do not reset these pointers per row or turn this into a generic
        // null-preserving relocation walker.
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
#endif

PawnDataHeader *LoadPawnData() {
    return LoadPawnFile("PAWNDATA.BIN");
}

// @verified - d
void BuildPawnSetAvailability(const PawnShopRecord *shop, std::int32_t *order, std::uint32_t *scores, PawnStockCount *scratch) {
    // BuildPawnSetAvailability counts every occurrence, not merely distinct item availability.
    if (!shop || !order || !scores)
        return;
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    std::int32_t index, slot;
    std::int32_t count = 0;
    for (index = 0; index < shop->setCount; ++index) {
        const PawnSetRecord *const set = GetPawnShopSet(shop, index);
        if (!set)
            continue;
        std::int32_t present;
        for (present = 0; present < set->itemCount + 1; ++present) {
            scratch[present].item = -1;
            scratch[present].remaining = 0;
        }
        std::int32_t distinct = 0;
        present = 0;
        std::int32_t item = 0;
        if (item < set->itemCount) {
            const std::int16_t *const items = reinterpret_cast<const std::int16_t *>(set + 1);
            do {
                slot = 0;
                if (distinct > 0) {
                    const std::int32_t searchOffset = item * sizeof(std::int16_t);
                    const char *const searchItems = reinterpret_cast<const char *>(set + 1);
                    do {
                        if (*reinterpret_cast<const std::int16_t *>(searchItems + searchOffset) == scratch[slot].item) {
                            if (scratch[slot].remaining > 0) {
                                --scratch[slot].remaining;
                                ++present;
                            }
                            break;
                        }
                        ++slot;
                    } while (distinct > slot);
                }
                if (slot == distinct) {
                    slot = static_cast<std::int32_t>(GetItemState(static_cast<std::uint32_t>(static_cast<std::int32_t>(items[item]))));
                    scratch[distinct].item = items[item];
                    scratch[distinct].remaining = slot;
                    --item;
                    ++distinct;
                }
                ++item;
            } while (item < set->itemCount);
        }
        order[count] = index;
        const std::int32_t coverage = 255 * present / set->itemCount;
        scores[count] = static_cast<std::uint32_t>(coverage) << 24 | (static_cast<std::uint32_t>(shop->setCount - index) & 0xffffffU);
        ++count;
    }
    for (slot = count; slot < shop->setCount; ++slot) {
        order[slot] = -1;
        scores[slot] = 0;
    }
#else
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
#endif
    SortPawnSetAvailability(scores, order, 0, count - 1);
}

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
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
#endif

} // namespace s2
} // namespace shenmue
