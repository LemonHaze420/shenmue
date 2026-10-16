#include "d3t/fvf.hpp"

#include <array>
#include <atomic>

namespace shenmue::d3t {
namespace {

std::array<FVFDescription, 64> descriptions{};
std::array<std::uint32_t, 64> tags{};
std::uint32_t nextIndex = 0;
std::atomic<std::int32_t> readers{0};
std::atomic<std::uint8_t> writer{0};

class ReadGuard {
  public:
    ReadGuard() {
        readers.fetch_add(1, std::memory_order_seq_cst);
        while (writer.load(std::memory_order_seq_cst) != 0) {
        }
    }
    ~ReadGuard() {
        readers.fetch_sub(1, std::memory_order_seq_cst);
    }
};

class WriteGuard {
  public:
    WriteGuard() {
        for (;;) {
            while (readers.load(std::memory_order_seq_cst) != 0) {
            }
            std::uint8_t expected = 0;
            if (!writer.compare_exchange_strong(expected, 1, std::memory_order_seq_cst)) {
                continue;
            }
            if (readers.load(std::memory_order_seq_cst) == 0)
                break;
            writer.store(0, std::memory_order_seq_cst);
        }
    }
    ~WriteGuard() {
        writer.store(0, std::memory_order_seq_cst);
    }
};

bool matches(const FVFElement &left, const FVFElement &right) {
    return left.semanticHash == right.semanticHash && left.semanticIndex == right.semanticIndex && left.format == right.format &&
        left.inputClassification == right.inputClassification && left.inputSlot == right.inputSlot && left.extraStride == right.extraStride;
}

} // namespace

std::uint32_t CreateFVF(const FVFDescription *description) {
    if (!description || description->elementCount > 32)
        return 0;
    {
        ReadGuard guard;
        for (std::uint32_t index = 0; index < tags.size(); ++index) {
            if (tags[index] < 0x80000000U || descriptions[index].elementCount != description->elementCount) {
                continue;
            }
            bool equal = true;
            for (std::uint32_t element = 0; element < description->elementCount; ++element) {
                if (!matches(descriptions[index].elements[element], description->elements[element])) {
                    equal = false;
                    break;
                }
            }
            if (equal) {
                const std::uint32_t generation = tags[index] >> 6U;
                return generation >= 0x2000000U ? (generation << 6U) | index : 0;
            }
        }
    }

    WriteGuard guard;
    const std::uint32_t first = nextIndex;
    std::uint32_t index = first;
    do {
        std::uint32_t generation = tags[index] >> 6U;
        if (generation < 0x2000000U) {
            ++generation;
            if ((generation & 0xfe000000U) == 0x2000000U)
                generation = 1;
            const std::uint32_t tag = (generation | 0xfe000000U) << 6U;
            tags[index] = tag;
            nextIndex = (index + 1U) & 0x3fU;
            descriptions[index] = *description;
            return tag | index;
        }
        index = (index + 1U) & 0x3fU;
    } while (index != first);
    return 0;
}

const FVFDescription *GetFVFDescription(std::uint32_t handle, std::uint32_t *index) {
    const std::uint32_t resolved = handle & 0x3fU;
    ReadGuard guard;
    if (tags[resolved] != (handle & 0xffffffc0U))
        return nullptr;
    if (index)
        *index = resolved;
    return &descriptions[resolved];
}

} // namespace shenmue::d3t
