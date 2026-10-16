#pragma once

#include <cstddef>
#include <array>
#include <cstdint>

namespace shenmue::s1 {

extern std::uint8_t g_harrierMemory[];

inline constexpr std::uint32_t kHarrierRankingOffset = 0x83400;
inline constexpr std::uint32_t kHarrierInputSelectorAddress = 0xd0031;
inline constexpr std::uint32_t kHarrierInputMaskOffset = 0xd0011;
inline constexpr std::uint32_t kHarrierDipSwitchOffset = 0xd0014;
inline constexpr std::uint32_t kHarrierCreditsTextOffset = 0x5f27;

struct HarrierSession {
    std::int32_t mode;
    std::int32_t aicaBootrom;
    std::int32_t coreKind;
    std::int32_t result;
    std::int32_t reserved10;
    std::int32_t exitDelay;
    std::int32_t credits;
    std::uint32_t scoreBcd;
    std::int32_t gameFlags;
    std::int32_t completed;
    std::int32_t playedFrames;
    std::int32_t reserved2c;
    std::uint8_t rankings[200];
};
static_assert(sizeof(HarrierSession) == 0xf8);
static_assert(offsetof(HarrierSession, credits) == 0x18);
static_assert(offsetof(HarrierSession, gameFlags) == 0x20);
static_assert(offsetof(HarrierSession, rankings) == 0x30);

inline constexpr std::array<std::uint8_t, 70> kHarrierDefaultRankings{
    1,
    0,
    0,
    0,
    'K',
    'A',
    'N',
    ' ',
    0,
    0,
    0,
    0x90,
    0,
    0,
    'Y',
    'U',
    '.',
    ' ',
    0,
    0,
    0,
    0x80,
    0,
    0,
    'K',
    'A',
    'W',
    ' ',
    0,
    0,
    0,
    0x70,
    0,
    0,
    'B',
    'I',
    'N',
    ' ',
    0,
    0,
    0,
    0x60,
    0,
    0,
    'T',
    'U',
    'D',
    ' ',
    0,
    0,
    0,
    0x50,
    0,
    0,
    'N',
    'A',
    'G',
    ' ',
    0,
    0,
    0,
    0x40,
    0,
    0,
    'A',
    'S',
    'O',
    ' ',
    0,
    0,
};
extern HarrierSession g_harrierSession;

extern float g_harrierInputScaleX;
extern float g_harrierInputScaleY;
extern std::uint8_t g_harrierInputSelector;
extern std::uint8_t &g_harrierInputMask;
extern std::uint8_t g_harrierContinueHookSeen;
extern std::int32_t g_harrierCoinEnabled;
extern std::int32_t g_harrierInputOrientation;
extern std::int32_t g_harrierDisplayCredits;
extern std::int32_t g_harrierRemainingCredits;
extern std::int32_t g_emu68_state;
extern std::int32_t g_harrierAttractReady;
extern std::int32_t g_harrierControlEnableDelay;
extern std::uint64_t g_harrierDebugHoldCount;
extern std::uint64_t g_harrierDebugFrameCount;
extern std::int32_t g_harrierPlaying;
extern std::int32_t g_harrierDebugEnabled;
extern std::int32_t g_harrierResumeDelay;
extern std::int32_t g_harrierStartPulse;
extern std::int32_t g_harrierCoinPulse;
extern std::int32_t g_harrierPauseFrames;
extern std::int32_t g_harrierControlsActive;
extern std::int32_t g_harrierHelpChoice;
extern std::int32_t g_harrierPaused;
extern std::int32_t g_harrierControllerDisconnected;
extern std::int32_t g_harrierExitAfterGame;
extern std::int32_t g_harrierResetRequested;
extern std::int32_t g_harrierExitReason;
extern std::int32_t g_harrierExitDelay;
extern std::int32_t g_harrierPlayedFrames;
extern std::int32_t g_harrierSkipHighScore;
extern std::int32_t g_harrierContinuePlay;
extern std::int32_t g_harrierHighScoreEntered;
extern std::uint32_t g_harrierBestScore;
extern std::int32_t g_harrierEmulatorFrameCount;

void Harrier_Init();
void Harrier_UpdateStateFromController();
std::uint8_t Harrier_HandleInput(std::uint32_t backingAddress);
void Harrier_WriteInputSelector(std::uint32_t backingAddress, std::uint8_t value);
void emu68_instruction_handler();

void HarrierInit();
void HarrierExecute();
void HarrierDestroy();
void HarrierLoadAndInitResources();
void CreateAndDrawEmu();
void HarrierControllerRelated();
void Harrier_ReleaseRenderResources();
void mallocHarrierSFX();
void DoAudio(bool paused);
void AudioFlushFromArchivesAndPlay();
void Harrier_UpdateAudioCommands();
void Harrier_ReleaseSoundEffects();
void CleanupAudioSources();

} // namespace shenmue::s1
