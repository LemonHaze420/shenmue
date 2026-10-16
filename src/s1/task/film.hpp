#pragma once

#include "s1/hlib.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct Colour4b {
    std::uint8_t red;
    std::uint8_t green;
    std::uint8_t blue;
    std::uint8_t alpha;
};

struct ColourBlendPair {
    Colour4b colourA;
    Colour4b colourB;
    std::uint32_t interpolationFrames;
};

struct FilmColourInterpolation {
    std::int32_t current[4];
    std::int32_t target[4];
    std::int32_t delta[4];
    std::int32_t remainingFrames;
    std::uint32_t packedColour;
};

static_assert(sizeof(ColourBlendPair) == 0xc);
static_assert(sizeof(FilmColourInterpolation) == 0x38);
static_assert(offsetof(FilmColourInterpolation, remainingFrames) == 0x30);

enum class ConstructColourMode : std::uint32_t {
    ApplyImmediately = 0,
    UseTemporaryClearColour = 1,
    CopyTemporaryClearColour = 2,
};

extern bool g_shouldUseTemporaryClearColour;
extern Colour4b g_temporaryClearColour;
extern std::uint32_t g_currentFilmColour;

HLib::Task *FILM_Init(std::uint16_t nextFunctionIndex);
void FILM_Callback(HLib::TaskData *taskData);
bool FILM_Callback_0(void *interpolationData);

std::uint32_t ConstructColour(std::int32_t red, std::int32_t green, std::int32_t blue, std::int32_t alpha);
void ConstructColours(HLib::Task *task, const ColourBlendPair *parameters, ConstructColourMode mode);
std::int32_t ctrlSetCurrentFilmFade();
void ApplyFadeToPacket(void *strip, float maxExtent);

bool FILM_HasTemporaryClearColour();
Colour4b FILM_GetTemporaryClearColour();

} // namespace shenmue::s1
