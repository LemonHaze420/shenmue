#pragma once

#include "s1/boot.hpp"
#include "s1/date_time.hpp"
#include "s1/save_format.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

inline constexpr shen_name TASK_BOOT_ROM{SHEN_NAME(BROM)};
inline constexpr std::array<float, 4> kArcadeClearColour{0.0f, 0.0f, 0.0f, 1.0f};

enum class ArcadeGame : std::uint8_t {
    None = 0,
    SpaceHarrier,
    HangOn,
};

enum class ArcadePixelFormat : std::uint8_t {
    Rgba8 = 0,
    Bgra8,
};

#pragma pack(push, 1)

struct HangOnUpdateBlock {
    // 2 on a debounced Start press while no race is in progress, otherwise 0.
    std::int32_t startSignal;
    std::uint8_t steering;
    std::uint8_t accelerator;
    std::uint16_t brake;
    std::uint32_t audioPlayLength;
    std::uint32_t reserved0c;
    // The 512x256 frame the library publishes.
    const void *frame;
    std::uint64_t frameBytes;
    void *audioData;
    std::uint64_t audioBytes;
    std::uint8_t debugFreezeTime;
    std::uint8_t debugForceTimeOne;
    std::uint8_t debugForceFinalStage;
    std::array<std::uint8_t, 5> reserved33;
};

// One packed high score in the Hang-On shared state: eight BCD digits, most significant first.
struct HangOnScoreRecord {
    std::array<std::uint8_t, 4> scoreBcd;
    std::array<char, 4> initials;
    std::uint32_t extra;
};


struct HangOnSharedState {
    std::int32_t field00;
    std::int32_t credits;
    std::uint32_t field08;
    std::int32_t field0c;
    std::array<HangOnScoreRecord, 20> scores;
    std::uint8_t gameInProgress;
    std::uint8_t exitRequested;
    std::uint16_t reserved102;
};


struct HangOnConfig {
    std::int32_t mode;
    std::int32_t field04;
    HangOnSharedState *shared;
};
#pragma pack(pop)
static_assert(sizeof(HangOnUpdateBlock) == 0x38);
static_assert(offsetof(HangOnUpdateBlock, frame) == 0x10);
static_assert(offsetof(HangOnUpdateBlock, audioData) == 0x20);
static_assert(offsetof(HangOnUpdateBlock, audioBytes) == 0x28);
static_assert(offsetof(HangOnUpdateBlock, debugFreezeTime) == 0x30);
static_assert(sizeof(HangOnScoreRecord) == 0x0c);
static_assert(sizeof(HangOnSharedState) == 0x104);
static_assert(offsetof(HangOnSharedState, scores) == 0x10);
static_assert(offsetof(HangOnSharedState, gameInProgress) == 0x100);
static_assert(sizeof(HangOnConfig) == 0x10);

struct HangOnLibrary {
    std::size_t (*computeMemoryRequired)(const HangOnConfig *config);
    void *(*create)(const HangOnConfig *config, void *memory);
    void (*update)(void *session, HangOnUpdateBlock *block);
    void (*destroy)(void *session);
};

extern HangOnLibrary g_hangOnLibrary;
bool LoadHangOnLibrary();

#pragma pack(push, 1)

struct EmulatorSessionContext {
    std::int32_t result;
    std::int64_t mode;
    std::int32_t field0c;
    std::int32_t aicaBootrom;
    std::int32_t credits;
    std::array<std::uint8_t, 4> lastScoreBcd;
    std::int32_t field1c;
    std::uint64_t flags;
};
#pragma pack(pop)
static_assert(sizeof(EmulatorSessionContext) == 0x28);
static_assert(offsetof(EmulatorSessionContext, credits) == 0x14);
static_assert(offsetof(EmulatorSessionContext, flags) == 0x20);

struct ArcadeVideoFrame {
    const void *pixels;
    std::uint32_t width;
    std::uint32_t height;
    std::uint32_t rowPitch;
    ArcadePixelFormat format;
};


void HangOnDestroy();
ArcadeGame Arcade_ActiveGame() noexcept;


extern EmulatorSessionContext g_emulatorSession;
extern GameDateTime g_harrierCurrentTime;
extern std::uint8_t g_harrierNeedsTimeUpdate;
extern float g_harrierSeconds;
extern std::int32_t g_harrierHaltRequested;

extern std::int32_t g_harrierExitedEmulator;

extern std::int32_t g_harrierShouldReturn;
extern GameDateTime g_hangOnCurrentTime;
extern float g_hangOnSeconds;
extern std::int32_t g_hangOnPaused;
extern std::int32_t g_hangOnPauseDebounce;
extern std::int32_t g_hangOnFramesToSkip;
extern float g_hangOnSteeringResponse;
extern HangOnUpdateBlock g_hangOnUpdate;

GameDateTime *CalculateElapsedTime();
float UpdateShenmueTimeForEmulator(GameDateTime &time, float seconds);
void Harrier_UpdateShenmueTime();

void HarrierLoop();
void HarrierModule(HLib::TaskData *taskData);
void HangOnCreate(HLib::TaskData *taskData);


inline constexpr std::array<HangOnScoreRecord, 20> kHangOnCreateDefaultRanking{{
    {{0x01, 0x00, 0x00, 0x00}, {'K', 'A', 'N', 0}, 0},
    {{0x00, 0x90, 0x00, 0x00}, {'Y', 'U', '.', 0}, 0},
    {{0x00, 0x80, 0x00, 0x00}, {'O', 'H', 'E', 0}, 0},
    {{0x00, 0x70, 0x00, 0x00}, {'I', 'S', 'H', 0}, 0},
    {{0x00, 0x60, 0x00, 0x00}, {'N', 'A', 'G', 0}, 0},
    {{0x00, 0x50, 0x00, 0x00}, {'K', 'A', 'J', 0}, 0},
    {{0x00, 0x40, 0x00, 0x00}, {'I', 'N', 'O', 0}, 0},
    {{}, {' ', ' ', ' ', 0}, 0},
    {{}, {' ', ' ', ' ', 0}, 0},
    {{}, {' ', ' ', ' ', 0}, 0},
    {{}, {' ', ' ', ' ', 0}, 0},
    {{}, {' ', ' ', ' ', 0}, 0},
    {{}, {' ', ' ', ' ', 0}, 0},
    {{}, {' ', ' ', ' ', 0}, 0},
    {{}, {' ', ' ', ' ', 0}, 0},
    {{}, {' ', ' ', ' ', 0}, 0},
    {{}, {' ', ' ', ' ', 0}, 0},
    {{}, {' ', ' ', ' ', 0}, 0},
    {{}, {' ', ' ', ' ', 0}, 0},
    {{}, {' ', ' ', ' ', 0}, 0},
}};

} // namespace shenmue::s1
