#pragma once

#include <array>
#include <cstdint>

namespace shenmue::s1 {

inline constexpr std::array<float, 12> kHarrierOutputMatrix{1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

inline constexpr std::uint32_t kHarrierAudioCommandOffset = 0xd0001;

extern std::int32_t g_harrierMusicFileIndex;

void Harrier_ResetAudioForCoreInit();
void mallocHarrierSFX();
void CreateAndStartPlayingSourceVoiceForHarrier(std::uint32_t soundIndex);
void HarrierStopAndResetVoiceSlot(std::uint32_t soundIndex);
void DoAudio(bool paused);
void HarrierLoadFileFromPath(const char *path);
void AudioFlushFromArchivesAndPlay();
void Harrier_UpdateAudioCommands();
void Harrier_ReleaseSoundEffects();
void CleanupAudioSources();

} // namespace shenmue::s1
