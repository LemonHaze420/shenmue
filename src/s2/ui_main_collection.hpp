#pragma once
#include <array>
#include <cstdint>

namespace shenmue::s2 {
inline constexpr std::array<std::int32_t, 36> MAIN_COLLECTION_PREVIEWS{
    13, 2, 3, 7, 8, 6, 5, 4, 10, 9, 11, 12, 14, 15, 16, 0, 17, 18, 20, 21, 1, 19, 0, 0, 24, 25, 22, 23, 0, 0, 0, 0, 0, 0, 0, 0};
inline constexpr std::int32_t MAIN_COLLECTION_LOCKED_PREVIEW = 26;

std::uint32_t LoadMainCollection();
void BindMainCollection();
std::int64_t RenderMainCollection();
std::int64_t MainCollectionInput();
std::int64_t ReloadMainCollection();
std::int64_t GetMainCollectionInitState();

std::uint32_t LoadCollectionBackground();
std::int64_t RenderCollectionBackground();
std::int64_t GetCollectionBackgroundInitState();
} // namespace shenmue::s2
