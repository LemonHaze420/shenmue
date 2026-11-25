#pragma once

#include "s2/author_controller_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

#define AUTHOR_MASK_TASK SHEN_NAME4('S', 'C', 'M', 'S')
struct AuthSmokeState {
    std::int32_t effect;
    Vector3 position, spawnVolume, velocityVariance, velocity;
    std::uint8_t fade, red, green, blue, falloff, state;
    std::int16_t count;
    float initialScale, width, height, birthRate;
};
struct AuthParticleState {
    std::int32_t effect;
    Vector3 position, spawnVolume, velocityVariance, velocity;
    std::uint16_t state;
    std::int16_t count;
    float floorHeight, width, height, birthRate;
    std::int32_t texture;
};
typedef char Layout_author_cleanup_dc_0[(sizeof(AuthSmokeState) == 0x4c) ? 1 : -1];
typedef char Layout_author_cleanup_dc_1[(offsetof(AuthSmokeState, state) == 0x39) ? 1 : -1];
typedef char Layout_author_cleanup_dc_2[(sizeof(AuthParticleState) == 0x4c) ? 1 : -1];
typedef char Layout_author_cleanup_dc_3[(offsetof(AuthParticleState, state) == 0x34) ? 1 : -1];
extern std::int32_t g_authorMaskDelay;
extern std::uint32_t g_authorSavedWeatherInstruction, g_authorSavedPlayerFlags;
extern std::uint32_t g_authorSavedMapModelMask, g_authorSavedMapFieldMask;
extern std::int32_t g_authorApplyYawOffset;
extern AuthSmokeState g_authorSmoke;
extern AuthParticleState g_authorParticles;
void AuthorMask_Callback(void *data);
void CleanupAuthCommands(AuthorController *control);
void SCMS_Enqueue(AuthorController *control);
void RestoreAuthorMapSlots();
void RestoreAuthCharacterStates();
void SeekAnimatedCameraSequence(std::uint32_t frame);
void sctlAuthApplyMotion();
std::int32_t ApplyAuthorCharacterMotion(std::int32_t phase, const std::int32_t *arguments, AuthActorContext *context);

} } // namespace shenmue::s2

#endif
