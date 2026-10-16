#pragma once
#include <cstdint>
namespace shenmue::s2::areas {
struct BookFaceControl {
    std::int16_t player, previousPlayer, companion, previousCompanion;
};
static_assert(sizeof(BookFaceControl) == 8);
extern BookFaceControl bookFaceControl;
extern float bookPlayerAlpha;
extern std::uint32_t bookSmokeEffect;
void BookMotionSoundCallback(void *);
void BookFaceLooksCallback(void *);
void BookPlayerAlphaCallback(void *);
void CopyBookPlayerAnchor();
void CreateBookSmoke();
void BookSmokeStartCallback(void *);
} // namespace shenmue::s2::areas
