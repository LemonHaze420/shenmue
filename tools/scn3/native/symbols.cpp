#include "scn3/native/symbol_tables.hpp"
#include "scn3/native/symbols.hpp"

namespace shenmue::scenario {

using namespace symbols;

std::optional<std::uint32_t> shenTypeConstantValueByName(std::string_view name) {
    std::size_t first = 0;
    std::size_t last = std::size(constantNames);
    while (first < last) {
        const std::size_t middle = first + (last - first) / 2;
        const int comparison = name.compare(constantNames[middle].name);
        if (comparison == 0)
            return constantNames[middle].value;
        if (comparison > 0)
            first = middle + 1;
        else
            last = middle;
    }
    return std::nullopt;
}

} // namespace shenmue::scenario
