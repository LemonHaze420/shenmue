#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/screen_film.hpp"

#define SHENMUE_FILM_CHECK(name, test) typedef char name[(test) ? 1 : -1]
namespace shenmue { namespace s2 {

struct ScreenFadeState {
    std::int32_t alpha, red, green, blue;
    std::int32_t targetAlpha, targetRed, targetGreen, targetBlue;
    std::int32_t alphaStep, redStep, greenStep, blueStep;
    std::uint32_t remainingFrames;
    std::uint32_t overlayColour;
    std::int32_t disabled;
};

struct CinemaBarState {
    std::int32_t visible;
    float height;
    float step;
};

struct ScreenFilmWork {
    shen_name name;
    ScreenFadeState fade;
    CinemaBarState bars;
    ScreenFilmColourPair flash;
};

SHENMUE_FILM_CHECK(ScreenFadeStateSizeCheck, sizeof(ScreenFadeState) == 0x3c);
SHENMUE_FILM_CHECK(ScreenFadeStateRemainingFramesOffsetCheck, offsetof(ScreenFadeState, remainingFrames) == 0x30);
SHENMUE_FILM_CHECK(ScreenFadeStateOverlayColourOffsetCheck, offsetof(ScreenFadeState, overlayColour) == 0x34);
SHENMUE_FILM_CHECK(ScreenFadeStateDisabledOffsetCheck, offsetof(ScreenFadeState, disabled) == 0x38);
SHENMUE_FILM_CHECK(CinemaBarStateSizeCheck, sizeof(CinemaBarState) == 0x0c);
SHENMUE_FILM_CHECK(ScreenFilmWorkSizeCheck, sizeof(ScreenFilmWork) == 0x54);
SHENMUE_FILM_CHECK(ScreenFilmWorkFadeOffsetCheck, offsetof(ScreenFilmWork, fade) == 4);
SHENMUE_FILM_CHECK(ScreenFilmWorkBarsOffsetCheck, offsetof(ScreenFilmWork, bars) == 0x40);
SHENMUE_FILM_CHECK(ScreenFilmWorkFlashOffsetCheck, offsetof(ScreenFilmWork, flash) == 0x4c);
inline shen_name ScreenFilmTaskName() { return shen_name(SHEN_NAME4('F', 'I', 'L', 'M')); }
inline shen_name ScreenFilmDataName() { return shen_name(SHEN_NAME4('S', 'C', 'F', 'T')); }
#define SCREEN_FILM_TASK ScreenFilmTaskName()
#define SCREEN_FILM_DATA ScreenFilmDataName()

void DrawScreenFilmRectangle(
    std::int32_t left, std::int32_t top, std::int32_t right, std::int32_t bottom, std::uint32_t colour, std::int32_t transparent, float depth);
bool UpdateScreenFade(ScreenFadeState *state);
bool UpdateCinemaBars(CinemaBarState *state);
void UpdateScreenFlash(ScreenFilmColourPair *state);
void ScreenFilmTask_Callback(void *data);

} } // namespace shenmue::s2

#undef SHENMUE_FILM_CHECK

#endif
