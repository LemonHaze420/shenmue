#include "s2/areas/wbbk_presentation.hpp"
#include "s2/areas/wbbk_carry.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/motion_work.hpp"
#include "s2/particle_effects.hpp"
#include "s2/sound_program.hpp"
namespace shenmue::s2::areas {
BookFaceControl bookFaceControl;
float bookPlayerAlpha;
std::uint32_t bookSmokeEffect;
void BookMotionSoundCallback(void *) {
    struct MotionSound {
        std::int32_t animation, frame, reserved;
    };
    static constexpr MotionSound cues[]{
        {.animation = 6300, .frame = 13, .reserved = 0},
        {.animation = 6301, .frame = 14, .reserved = 0},
        {.animation = 6301, .frame = 28, .reserved = 0},
        {.animation = 6305, .frame = 21, .reserved = 0},
        {.animation = 6306, .frame = 19, .reserved = 0},
        {.animation = 6306, .frame = 38, .reserved = 0},
        {.animation = 6311, .frame = 10, .reserved = 0},
        {.animation = 6311, .frame = 20, .reserved = 0},
        {.animation = 6310, .frame = 9, .reserved = 0},
        {.animation = 6313, .frame = 10, .reserved = 0},
        {.animation = 6309, .frame = 15, .reserved = 0},
        {.animation = 6304, .frame = 14, .reserved = 0},
        {.animation = 0, .frame = 0, .reserved = 0},
    };
    const std::int32_t animation = GetMotionAnimationId(scenFindMotionwork(BOOK_RYO), MotionRegion::LowerBody);
    const std::int32_t frame = static_cast<std::int32_t>(GetMotionFrame(scenFindMotionwork(BOOK_RYO), MotionRegion::LowerBody));
    for (std::int32_t index = 0; cues[index].animation; ++index)
        if (animation == cues[index].animation && frame == cues[index].frame)
            Loop3DSoundEffect(bookCarryVariant == 1 ? 705 : 1399);
}
void BookFaceLooksCallback(void *) {
    float amount = 1;
    if (bookFaceControl.previousPlayer != bookFaceControl.player) {
        if (bookFaceControl.player == 14)
            amount = .5f;
        SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(BOOK_RYO), bookFaceControl.player, 10, amount);
        bookFaceControl.previousPlayer = bookFaceControl.player;
    }
    if (bookFaceControl.previousCompanion != bookFaceControl.companion) {
        SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(BOOK_SAI), bookFaceControl.companion, 10, amount);
        bookFaceControl.previousCompanion = bookFaceControl.companion;
    }
}
void BookPlayerAlphaCallback(void *) {
    SetScenarioCharacterTransparency(BOOK_RYO, bookPlayerAlpha);
    bookPlayerAlpha += .015f;
    if (bookPlayerAlpha > 1) {
        SetScenarioCharacterTransparency(BOOK_RYO, 1);
        HLib::DestroyCurrentTask();
    }
}
void CopyBookPlayerAnchor() {
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(BOOK_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    GetScenarioCharacterEulerAngles(BOOK_RYO, &rotation, EventTransformFlags::Local);
    EV_AT_SetPos(bookPlayerAnchor, EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(bookPlayerAnchor, EventTransformFlags::AllAxes, rotation);
}
void CreateBookSmoke() {
    const Vector3 position{.x = 333.15079f, .y = 127.9809f, .z = 247.705f}, volume{.x = .1f, .y = 0, .z = .1f};
    const Vector3 velocity{.x = 0, .y = -.001f, .z = 0}, variance{.x = .0020000001f, .y = .0020000001f, .z = .0020000001f};
    bookSmokeEffect = static_cast<std::uint32_t>(CreateSmokeEffect(position, volume, 70, .079999998f, shen_name{0}, 0));
    SetSmokeEffectFadeFactor(bookSmokeEffect, 21);
    SetSmokeEffectFalloff(bookSmokeEffect, 144);
    SetParticleEffectColour(0, bookSmokeEffect, 0, 144, 144, 112);
    SetParticleEffectInitialScale(0, bookSmokeEffect, .328999996f);
    SetParticleEffectSize(0, bookSmokeEffect, 1.55999994f, 1.40999997f);
    SetParticleEffectBirthRate(0, bookSmokeEffect, 0);
    SetParticleEffectVelocity(0, bookSmokeEffect, velocity, variance);
    SuspendParticleEffect(0, bookSmokeEffect);
}
void BookSmokeStartCallback(void *) {
    SetParticleEffectBirthRate(0, bookSmokeEffect, 0);
    HLib::DestroyCurrentTask();
}
} // namespace shenmue::s2::areas
