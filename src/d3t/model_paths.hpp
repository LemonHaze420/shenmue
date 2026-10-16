#pragma once

#include <cstdint>

#include "shenmue.h"

namespace shenmue::d3t {

#if SHENMUE_IS_S1
inline constexpr const char *kModelPathMiscSuffixes[][2] = {{"op99.afs/afs00022_/mals501g", "/Intro"}, {"scene/02/ms8s/mpk00/dork2clg", "/ms8s"}};
#endif
inline constexpr char kModelPathCharacterParts[] = {'b', 'f', 'm', 's', 'l', 'z', 'q', 'e', 'n', 'g', 'd', 'w'};
inline constexpr char kModelPathCharacterPairs[][2] = {
    {'s', 's'}, {'n', 's'}, {'m', 's'}, {'r', 's'}, {'t', 'l'}, {'t', 'r'}, {'u', 'l'}, {'u', 'r'}, {'x', 'l'}, {'x', 'r'}};

std::int32_t RemapModelPath(const char *source, char *destination);

} // namespace shenmue::d3t
