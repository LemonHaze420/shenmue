#pragma once

#include <cstddef>
#include <cstdint>

namespace hangon {

inline constexpr wchar_t kDllName[] = L"GameHangon_Winx64_Release.dll";
inline constexpr unsigned kFrameWidth = 512;
inline constexpr unsigned kFrameHeight = 256;
inline constexpr unsigned kPictureX = 96;
inline constexpr unsigned kPictureY = 16;
inline constexpr unsigned kPictureWidth = 320;
inline constexpr unsigned kPictureHeight = 224;
inline constexpr unsigned kSampleRate = 48000;
inline constexpr unsigned kSampleFrames = 800;
inline constexpr unsigned kAudioBytes = kSampleFrames * 2 * sizeof(std::int16_t);
inline constexpr unsigned kAudioBuffers = 4;
inline constexpr unsigned kTickRate = 60;

#pragma pack(push, 1)
struct ScoreRecord {
    std::uint8_t scoreBcd[4];
    char initials[4];
    std::uint32_t extra;
};

struct SharedState {
    std::int32_t field00;
    std::int32_t credits;
    std::uint32_t field08;
    std::int32_t field0c;
    ScoreRecord scores[20];
    std::uint8_t gameInProgress;
    std::uint8_t exitRequested;
    std::uint16_t reserved102;
};

struct Config {
    std::int32_t mode;
    std::int32_t field04;
    SharedState* shared;
};

struct UpdateBlock {
    std::int32_t startSignal;
    std::uint8_t steering;
    std::uint8_t accelerator;
    std::uint16_t brake;
    std::uint32_t audioPlayLength;
    std::uint32_t reserved0c;
    const void* frame;
    std::uint64_t frameBytes;
    void* audioData;
    std::uint64_t audioBytes;
    std::uint8_t debugFreezeTime;
    std::uint8_t debugForceTimeOne;
    std::uint8_t debugForceFinalStage;
    std::uint8_t reserved33[5];
};
#pragma pack(pop)

static_assert(sizeof(ScoreRecord) == 0x0c);
static_assert(sizeof(SharedState) == 0x104 && offsetof(SharedState, scores) == 0x10 && offsetof(SharedState, gameInProgress) == 0x100);
static_assert(sizeof(Config) == 0x10 && offsetof(Config, shared) == 0x08);
static_assert(sizeof(UpdateBlock) == 0x38 && offsetof(UpdateBlock, frame) == 0x10 && offsetof(UpdateBlock, audioData) == 0x20);
static_assert(offsetof(UpdateBlock, audioBytes) == 0x28 && offsetof(UpdateBlock, debugFreezeTime) == 0x30);

using ComputeMemoryRequired = std::size_t (*)(const Config*);
using Create = void* (*)(const Config*, void*);
using Update = void (*)(void*, UpdateBlock*);
using Destroy = void (*)(void*);

inline constexpr ScoreRecord kDefaultRanking[20] = {
    {.scoreBcd = {0x01, 0, 0, 0}, .initials = {'K', 'A', 'N', 0}, .extra = 0},
    {.scoreBcd = {0, 0x90, 0, 0}, .initials = {'Y', 'U', '.', 0}, .extra = 0},
    {.scoreBcd = {0, 0x80, 0, 0}, .initials = {'O', 'H', 'E', 0}, .extra = 0},
    {.scoreBcd = {0, 0x70, 0, 0}, .initials = {'I', 'S', 'H', 0}, .extra = 0},
    {.scoreBcd = {0, 0x60, 0, 0}, .initials = {'N', 'A', 'G', 0}, .extra = 0},
    {.scoreBcd = {0, 0x50, 0, 0}, .initials = {'K', 'A', 'J', 0}, .extra = 0},
    {.scoreBcd = {0, 0x40, 0, 0}, .initials = {'I', 'N', 'O', 0}, .extra = 0},
    {.scoreBcd = {}, .initials = {' ', ' ', ' ', 0}, .extra = 0},
    {.scoreBcd = {}, .initials = {' ', ' ', ' ', 0}, .extra = 0},
    {.scoreBcd = {}, .initials = {' ', ' ', ' ', 0}, .extra = 0},
    {.scoreBcd = {}, .initials = {' ', ' ', ' ', 0}, .extra = 0},
    {.scoreBcd = {}, .initials = {' ', ' ', ' ', 0}, .extra = 0},
    {.scoreBcd = {}, .initials = {' ', ' ', ' ', 0}, .extra = 0},
    {.scoreBcd = {}, .initials = {' ', ' ', ' ', 0}, .extra = 0},
    {.scoreBcd = {}, .initials = {' ', ' ', ' ', 0}, .extra = 0},
    {.scoreBcd = {}, .initials = {' ', ' ', ' ', 0}, .extra = 0},
    {.scoreBcd = {}, .initials = {' ', ' ', ' ', 0}, .extra = 0},
    {.scoreBcd = {}, .initials = {' ', ' ', ' ', 0}, .extra = 0},
    {.scoreBcd = {}, .initials = {' ', ' ', ' ', 0}, .extra = 0},
    {.scoreBcd = {}, .initials = {' ', ' ', ' ', 0}, .extra = 0},
};

} // namespace hangon
