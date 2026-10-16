#pragma once

#include <cstring>
#include <type_traits>

namespace shenmue {

template <typename T> T read_unaligned(const void *source) noexcept {
    static_assert(std::is_trivially_copyable_v<T>);
    T value{};
    std::memcpy(&value, source, sizeof(value));
    return value;
}

template <typename T> void write_unaligned(void *destination, const T &value) noexcept {
    static_assert(std::is_trivially_copyable_v<T>);
    std::memcpy(destination, &value, sizeof(value));
}

} // namespace shenmue
