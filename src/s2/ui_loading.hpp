#pragma once

#include "s2/date_time.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

namespace shenmue::s2::ui_detail {

enum LoadingWindow : std::size_t {
    LoadingBackground,
    LoadingBlackBackground,
    LoadingHeader,
    LoadingCollectionPoster,
    LoadingLocation,
    LoadingSubLocation,
    LoadingPreviousHeader,
    LoadingPreviousLocation,
    LoadingTime,
    LoadingDate,
    LoadingText,
    LoadingWindowCount,
};

inline constexpr std::array<const char *, LoadingWindowCount> LOADING_WINDOWS{"LoadScreen_BackgroundPanel", "LoadScreen_BlackBackground", "LoadScreen_Header",
    "LoadScreen_SMCollectionPoster", "LoadScreen_LocationNameLabel", "LoadScreen_SubLocationNameLabel", "LoadScreen_PrevHeader",
    "LoadScreen_PrevLocationNameLabel", "LoadScreen_TimeLabel", "LoadScreen_DateLabel", "LoadScreen_LoadingText"};
inline constexpr std::array<const char *, 3> LOADING_IMAGE_WIDGETS{"Image_Header", "Image_SMCollectionPoster", "Prev_Image_Header"};
inline constexpr std::array<const char *, 6> LOADING_LABEL_TOKENS{"TIME", "DATE", "AREA", "SUB_AREA", "PREV_AREA", "LOADING"};
inline constexpr std::array<const char *, 4> LOADING_DOTS{"     ", ".    ", ". .  ", ". . ."};
inline constexpr std::array<LoadingWindow, 10> LOADING_SHOW_RESET_ORDER{LoadingLocation, LoadingSubLocation, LoadingPreviousLocation, LoadingHeader,
    LoadingPreviousHeader, LoadingDate, LoadingTime, LoadingBackground, LoadingBlackBackground, LoadingText};
inline constexpr std::array<LoadingWindow, 2> LOADING_BACKGROUNDS{LoadingBackground, LoadingBlackBackground};
inline constexpr std::array<LoadingWindow, 7> LOADING_FADE_OUT_ORDER{
    LoadingLocation, LoadingSubLocation, LoadingPreviousHeader, LoadingPreviousLocation, LoadingText, LoadingDate, LoadingTime};
inline constexpr std::array<LoadingWindow, 5> LOADING_FADE_IN_ORDER{
    LoadingSubLocation, LoadingPreviousHeader, LoadingPreviousLocation, LoadingDate, LoadingTime};
inline constexpr std::array<LoadingWindow, LoadingWindowCount> LOADING_ACTIVE_ORDER{LoadingBackground, LoadingBlackBackground, LoadingHeader,
    LoadingCollectionPoster, LoadingLocation, LoadingSubLocation, LoadingPreviousHeader, LoadingPreviousLocation, LoadingText, LoadingDate, LoadingTime};
inline constexpr shen_name LOADING_GOV2{SHEN_NAME(GOV2)};
inline constexpr shen_name LOADING_TBCN{SHEN_NAME(TBCN)};
inline constexpr shen_name LOADING_NDAY{SHEN_NAME(NDAY)};

struct LoadingScreenRuntime {
    shen_name area{};
    shen_name subarea{};
    DateTime dateTime{};
    std::uint32_t flags{};
    std::int32_t floor{};
    std::int32_t fadeMode{};
    std::int32_t entry{};
    std::array<std::string, LOADING_LABEL_TOKENS.size()> text{};
    std::array<std::int32_t, LOADING_LABEL_TOKENS.size()> labels{};
    std::array<std::int32_t, LOADING_IMAGE_WIDGETS.size()> images{};
    std::array<std::int32_t, LoadingWindowCount> windows{};
    std::array<std::int32_t, LoadingWindowCount> animations{};
    std::array<std::uint32_t, 2> headerRequests{};
    std::array<std::uint32_t, 2> renderHandles{};
    bool collectionPoster{};
    bool hasLocationState{};
    bool labelsBound{};
};

} // namespace shenmue::s2::ui_detail
