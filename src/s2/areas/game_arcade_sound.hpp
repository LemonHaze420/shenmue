#pragma once

#include <cstdint>

namespace shenmue::s2 {

void InitializeArcadeSoundEffects(const char *const *files, std::int32_t count);
void ResetArcadeMusic();
void PlayArcadeSoundEffect(std::uint32_t index, bool retrigger);
bool IsArcadeSoundEffectActive(std::uint32_t index);
void StopArcadeSoundEffect(std::uint32_t index);
void PauseArcadeSoundEffects(bool paused);
void StopArcadeSoundEffects();
void ReleaseArcadeSoundEffects();
void QueueArcadeMusic(const char *path);
void AdvanceArcadeMusic(std::int32_t frames);
void FadeOutArcadeMusic(std::int32_t frames);
void StopArcadeMusic();
void ReleaseArcadeMusic();

} // namespace shenmue::s2
