#include "s1/task/save.hpp"
#include "s1/task/warn.hpp"

#include "s1/boot.hpp"
#include "s1/d3t/d3t_threads.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/initialization.hpp"
#include "s1/task/hplg.hpp"
#include "s1/input.hpp"
#include "s1/screen_space.hpp"
#include "s1/ui_runtime.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/padc.hpp"
#include "s1/task/paus.hpp"
#include "s1/player_stats.hpp"
#include "s1/task/lcdm.hpp"
#include "s1/texture_cache.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/disp.hpp"
#include "s1/task/cycle_man.hpp"

#include <array>
#include <cstdio>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <cstring>

namespace shenmue::s1 {

std::int64_t LCDM_Init();
HLib::Task *WIND_Init();
void ApplyFadeToPacket(void *strip, float maxExtent);
std::int32_t shenExecuteAICACommand(std::uint32_t command, std::uint32_t argument, std::int32_t parameter);
extern std::uint8_t g_timeValue;
extern std::uint8_t g_shouldTimeAdvance;
extern std::int32_t g_busy_reset;
extern std::array<std::uint8_t, 128> g_shenmueFlags;

HLib::Task *g_saveTask = nullptr;
SaveTaskData *g_saveTaskData = nullptr;
std::int32_t g_temporarySaveSceneId = 0;
std::uint32_t g_temporarySaveAreaId = 0;
std::int32_t g_saveScreenFlag = 0;
std::uint32_t g_temporarySaveEntryPoint = 0;
std::array<std::uint32_t, 32> g_arcadeHighScores{};
std::array<ArcadeScoreEntry, 20> g_spaceHarrierScores{};
std::array<ArcadeScoreEntry, 20> g_hangOnScores{};

namespace {

std::uint32_t DecodeRankingScore(const std::uint8_t *digits) {
    std::uint32_t score = 0;
    for (std::size_t i = 0; i < 4; ++i)
        score = (score * 10U + (digits[i] >> 4U)) * 10U + (digits[i] & 0x0fU);
    return score;
}
} // namespace


void Arcade_InitDefaultHangOnRanking() {
    std::memcpy(g_hangOnScores.data(), kHangOnDefaultRankingImage.data(), sizeof(g_hangOnScores));
    for (std::size_t i = 0; i < g_hangOnScores.size(); ++i)
        g_hangOnScores[i].score = DecodeRankingScore(kHangOnDefaultRankingImage[i].data());
}


void Arcade_InitDefaultSpaceHarrierRanking() {
    for (std::size_t i = 0; i < g_spaceHarrierScores.size(); ++i) {
        const std::array<std::uint8_t, 10> &record = kSpaceHarrierDefaultRanking[i];
        ArcadeScoreEntry &entry = g_spaceHarrierScores[i];
        entry.score = DecodeRankingScore(record.data());
        std::memcpy(entry.initials, &record[4], sizeof(entry.initials));
        entry.reserved[0] = record[7];
    }
}
std::array<std::uint32_t, 5> g_saveDataRelated{};
std::uint32_t g_saveScriptFlags = 0;
std::int32_t g_saveDisplayModuleState = SAVE_DISPLAY_MODULE_NONE;
S1SaveResult g_lastS1SaveResult = S1SaveResult::Success;
std::int32_t g_saveOrLoad = -1;
std::int32_t g_loadType = 0;
std::uint8_t g_hasSaved = 0;
std::int32_t g_hasConfirmedButton = 0;
std::atomic<std::int32_t> g_saveWriteStatus = 0;
std::int32_t g_saveWriteSlot = 0;
std::int32_t g_currentSlotIndex = 0;
std::int32_t g_loadRequested = 0;
std::array<char, 16> g_currentSaveSlot{};

float g_saveDisplayInitScale = 0.0f;

namespace {

constexpr std::uint32_t kPlaySuccessSound = 0x1100abU;




using SaveDisplayRow = std::array<std::int8_t, 20>;
constexpr std::size_t kRowLcdm = 5;
constexpr std::size_t kRowAction = 6;
constexpr std::size_t kRowAccept = 8;
constexpr std::size_t kRowCancel = 9;
constexpr std::size_t kRowNext = 15;
constexpr std::size_t kRowEntrySound = 17;
constexpr std::size_t kRowSkipCycles = 18;

constexpr std::array<SaveDisplayRow, 25> kSaveGridRows{{
    {38, 0, 0, 0, 1, 0, 0, 9, -1, -1, 1, 9, 10, 1, 1, 1, 1, 0, 0, 0},
    {38, 0, 0, 0, 0, 0, 2, 0, 2, 99, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 1, 0, 0, 9, -1, -1, 8, 9, 10, 11, 14, 3, 13, 0, 0, 0},
    {38, 0, 0, 0, 1, 0, 0, 2, 4, 5, 8, -1, -1, -1, -1, -1, -1, 0, 0, 0},
    {38, 0, 0, 0, 2, 0, 2, 0, 5, 99, 8, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {0, 8, 0, 0, 3, 2, 0, 5, -1, -1, 22, 14, 14, 14, 14, 6, -1, 0, 0, 0},
    {0, 8, 0, 0, 3, 2, 0, 0, -1, -1, 22, 14, 14, 14, 14, 7, -1, 0, 0, 0},
    {0, 8, 0, 0, 6, 3, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, 1, 0},
    {2, 0, 0, 0, 15, 0, 4, 0, -1, 99, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {2, 0, 0, 0, 17, 0, 4, 0, -1, 99, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {2, 0, 0, 0, 18, 0, 4, 0, -1, 99, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 19, 0, 3, 0, 12, 99, 8, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {0, 8, 0, 0, 3, 2, 0, 5, -1, -1, 14, 14, 14, 14, 14, 6, -1, 0, 0, 0},
    {38, 0, 0, 0, 23, 0, 3, 0, 5, 99, 8, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {4, 0, 0, 0, 22, 4, 5, 0, 15, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {4, 0, 0, 0, 1, 2, 0, 4, -1, -1, 18, 19, 20, 21, 14, 16, 16, 0, 0, 0},
    {0, 8, 0, 0, 3, 2, 0, 6, -1, -1, 18, 19, 20, 21, 14, 17, -1, 0, 0, 0},
    {0, 8, 0, 0, 3, 2, 0, 0, -1, -1, 18, 19, 20, 21, 14, 7, -1, 0, 0, 0},
    {2, 0, 0, 0, 15, 0, 4, 0, -1, 22, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {2, 0, 0, 0, 17, 0, 4, 0, -1, 14, 18, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {2, 0, 0, 0, 18, 0, 4, 0, -1, 14, 18, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 19, 0, 3, 0, 16, 14, 18, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {4, 0, 0, 0, 22, 4, 5, 0, 23, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {4, 0, 0, 0, 1, 2, 0, 4, -1, -1, 18, 19, 20, 21, 14, 24, 24, 0, 0, 0},
    {38, 0, 0, 0, 23, 0, 3, 0, 15, 22, 18, -1, -1, -1, -1, -1, -1, 0, 1, 0},
}};
constexpr std::array<SaveDisplayRow, 29> kScriptedRows{{
    {4, 0, 0, 0, 24, 0, 5, 0, 1, 0, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 1, 0, 0, 9, -1, -1, 10, 11, 12, 26, 2, 2, 22, 0, 0, 0},
    {38, 0, 0, 0, 9, 0, 1, 0, 3, 0, 10, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 12, 0, 2, 0, 5, 2, 10, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 11, 0, 2, 0, 5, 2, 10, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {0, 8, 0, 0, 1, 2, 0, 9, -1, -1, 10, 11, 12, 13, 14, 6, 6, 0, 0, 0},
    {0, 8, 0, 0, 10, 2, 0, 5, -1, -1, 23, 14, 14, 14, 14, 7, -1, 0, 0, 0},
    {0, 8, 0, 0, 10, 2, 0, 0, -1, -1, 23, 14, 14, 14, 14, 8, -1, 0, 0, 0},
    {0, 8, 0, 0, 14, 3, 6, 0, -1, -1, -1, -1, -1, -1, -1, 9, -1, 1, 1, 0},
    {0, 8, 0, 0, 25, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {2, 0, 0, 0, 15, 0, 4, 0, -1, 0, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {2, 0, 0, 0, 17, 0, 4, 0, -1, 0, 10, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {2, 0, 0, 0, 18, 0, 4, 0, -1, 0, 10, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 19, 0, 3, 0, 6, 0, 10, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {4, 0, 0, 0, 22, 4, 5, 0, 15, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {4, 0, 0, 0, 1, 2, 0, 4, -1, -1, 18, 19, 20, 21, 14, 16, 16, 0, 0, 0},
    {0, 8, 0, 0, 10, 2, 0, 6, -1, -1, 18, 19, 20, 21, 14, 17, -1, 0, 0, 0},
    {0, 8, 0, 0, 10, 2, 0, 0, -1, -1, 18, 19, 20, 21, 14, 8, -1, 0, 0, 0},
    {2, 0, 0, 0, 15, 0, 4, 0, -1, 23, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {2, 0, 0, 0, 17, 0, 4, 0, -1, 14, 18, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {2, 0, 0, 0, 18, 0, 4, 0, -1, 14, 18, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 19, 0, 3, 0, 16, 14, 18, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 23, 0, 3, 0, 2, 0, 10, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {4, 0, 0, 0, 22, 4, 5, 0, 24, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {4, 0, 0, 0, 1, 2, 0, 4, -1, -1, 18, 19, 20, 21, 23, 25, 25, 0, 0, 0},
    {38, 0, 0, 0, 23, 0, 3, 0, 15, 23, 18, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 9, 0, 1, 0, 27, 0, 10, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {0, 8, 0, 0, 1, 2, 0, 9, -1, -1, 10, 11, 12, 13, 14, 6, 6, 0, 0, 0},
    {0, 8, 0, 0, 1, 2, 0, 9, -1, -1, 10, 11, 12, 13, 14, 6, 6, 0, 0, 0},
}};
constexpr std::array<SaveDisplayRow, 32> kDefaultRows{{
    {38, 0, 0, 0, 1, 0, 0, 9, -1, -1, 8, 9, 10, 29, 1, 1, 25, 0, 0, 0},
    {38, 0, 0, 0, 9, 0, 1, 0, 2, 99, 8, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 12, 0, 2, 0, 4, 1, 8, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 11, 0, 2, 0, 4, 1, 8, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {0, 8, 0, 0, 1, 2, 0, 9, -1, -1, 8, 9, 10, 11, 12, 5, 5, 0, 0, 0},
    {0, 8, 0, 0, 10, 2, 0, 5, -1, -1, 26, 12, 12, 12, 12, 6, -1, 0, 0, 0},
    {0, 8, 0, 0, 10, 2, 0, 0, -1, -1, 26, 12, 12, 12, 12, 7, -1, 0, 0, 0},
    {0, 8, 0, 0, 14, 3, 6, 0, -1, -1, -1, -1, -1, -1, -1, 99, -1, 1, 1, 0},
    {2, 0, 0, 0, 15, 0, 4, 0, -1, 99, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {2, 0, 0, 0, 17, 0, 4, 0, -1, 99, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {2, 0, 0, 0, 18, 0, 4, 0, -1, 99, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 19, 0, 3, 0, 23, 99, 8, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {4, 0, 0, 0, 22, 4, 5, 0, 13, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {0, 8, 0, 0, 1, 2, 0, 4, -1, -1, 17, 18, 19, 20, 12, 14, 14, 0, 0, 0},
    {0, 8, 0, 0, 10, 2, 0, 6, -1, -1, 17, 18, 19, 20, 12, 15, -1, 0, 0, 0},
    {0, 8, 0, 0, 10, 2, 0, 0, -1, -1, 17, 18, 19, 20, 12, 16, -1, 0, 0, 0},
    {0, 8, 0, 0, 14, 3, 6, 0, -1, -1, -1, -1, -1, -1, -1, 99, -1, 1, 1, 0},
    {2, 0, 0, 0, 15, 0, 4, 0, -1, 26, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {2, 0, 0, 0, 17, 0, 4, 0, -1, 12, 17, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {2, 0, 0, 0, 18, 0, 4, 0, -1, 12, 17, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 19, 0, 3, 0, 24, 12, 17, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 23, 0, 3, 0, 5, 99, 8, 9, 10, 11, 12, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 23, 0, 3, 0, 14, 12, 17, 18, 19, 20, 12, -1, -1, 0, 1, 0},
    {0, 8, 0, 0, 10, 2, 0, 5, -1, -1, 12, 12, 12, 12, 12, 6, -1, 0, 0, 0},
    {0, 8, 0, 0, 10, 2, 0, 6, -1, -1, 17, 18, 19, 20, 12, 15, -1, 0, 0, 0},
    {38, 0, 0, 0, 23, 0, 3, 0, 1, 99, 8, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {4, 0, 0, 0, 22, 4, 5, 0, 27, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {4, 0, 0, 0, 1, 2, 0, 4, -1, -1, 17, 18, 19, 20, 26, 28, 28, 0, 0, 0},
    {38, 0, 0, 0, 23, 0, 3, 0, 13, 26, 17, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 9, 0, 1, 0, 30, 99, 8, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {0, 8, 0, 0, 1, 2, 0, 9, -1, -1, 8, 9, 10, 11, 12, 5, 5, 0, 0, 0},
    {0, 8, 0, 0, 1, 2, 0, 9, -1, -1, 8, 9, 10, 11, 12, 5, 5, 0, 0, 0},
}};
constexpr std::array<SaveDisplayRow, 30> kSaveProgressRows{{
    {38, 0, 0, 0, 1, 0, 0, 9, -1, -1, 8, 9, 10, 1, 1, 1, 25, 0, 0, 0},
    {38, 0, 0, 0, 9, 0, 1, 0, 2, 26, 8, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 12, 0, 2, 0, 4, 1, 8, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 11, 0, 2, 0, 4, 1, 8, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {0, 8, 0, 0, 1, 2, 0, 9, -1, -1, 8, 9, 10, 11, 12, 5, 5, 0, 0, 0},
    {0, 8, 0, 0, 10, 2, 0, 5, -1, -1, 27, 12, 12, 12, 12, 6, -1, 0, 0, 0},
    {0, 8, 0, 0, 10, 2, 0, 0, -1, -1, 27, 12, 12, 12, 12, 7, -1, 0, 0, 0},
    {0, 8, 0, 0, 14, 3, 6, 0, -1, -1, -1, -1, -1, -1, -1, 99, -1, 1, 1, 0},
    {2, 0, 0, 0, 15, 0, 4, 0, -1, 26, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {2, 0, 0, 0, 17, 0, 4, 0, -1, 26, 8, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {2, 0, 0, 0, 18, 0, 4, 0, -1, 26, 8, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 19, 0, 3, 0, 23, 26, 8, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {4, 0, 0, 0, 22, 4, 5, 0, 13, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {0, 8, 0, 0, 1, 2, 0, 4, -1, -1, 17, 18, 19, 20, 12, 14, 14, 0, 0, 0},
    {0, 8, 0, 0, 10, 2, 0, 6, -1, -1, 17, 18, 19, 20, 12, 15, -1, 0, 0, 0},
    {0, 8, 0, 0, 10, 2, 0, 0, -1, -1, 17, 18, 19, 20, 12, 16, -1, 0, 0, 0},
    {0, 8, 0, 0, 14, 3, 6, 0, -1, -1, -1, -1, -1, -1, -1, 99, -1, 1, 1, 0},
    {2, 0, 0, 0, 15, 0, 4, 0, -1, 27, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {2, 0, 0, 0, 17, 0, 4, 0, -1, 12, 17, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {2, 0, 0, 0, 18, 0, 4, 0, -1, 12, 17, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 19, 0, 3, 0, 24, 12, 17, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 23, 0, 3, 0, 5, 0, 8, 9, 10, 11, 12, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 23, 0, 3, 0, 14, 12, 17, 18, 19, 20, 12, -1, -1, 0, 1, 0},
    {0, 8, 0, 0, 10, 2, 0, 5, -1, -1, 12, 12, 12, 12, 12, 6, -1, 0, 0, 0},
    {0, 8, 0, 0, 10, 2, 0, 6, -1, -1, -1, -1, -1, -1, -1, 15, -1, 0, 0, 0},
    {38, 0, 0, 0, 23, 0, 3, 0, 1, 26, 8, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 13, 0, 3, 0, 99, 0, -1, -1, -1, -1, -1, -1, 0, 0, 1, 0},
    {4, 0, 0, 0, 22, 4, 5, 0, 28, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {4, 0, 0, 0, 1, 2, 0, 4, -1, -1, 17, 18, 19, 20, 27, 29, 29, 0, 0, 0},
    {38, 0, 0, 0, 23, 0, 3, 0, 13, 27, 17, 18, 19, 20, 27, -1, -1, 0, 1, 0},
}};
constexpr std::array<SaveDisplayRow, 21> kTitleLoadRows{{
    {38, 0, 0, 0, 7, 0, 2, 0, 98, 1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 8, 0, 2, 0, 2, 99, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {0, 8, 0, 0, 1, 2, 0, 9, -1, -1, 5, 6, 7, 8, 9, 3, 17, 0, 0, 0},
    {0, 8, 0, 0, 10, 2, 0, 7, -1, -1, 18, -1, -1, -1, -1, 4, -1, 0, 0, 0},
    {0, 8, 0, 0, 10, 2, 0, 0, -1, -1, 18, 9, 9, 9, 9, 99, -1, 0, 0, 0},
    {2, 0, 0, 0, 15, 0, 4, 0, -1, 1, -1, -1, -1, -1, -1, -1, 0, 0, 1, 0},
    {2, 0, 0, 0, 17, 0, 4, 0, -1, 1, 5, -1, -1, -1, -1, -1, 0, 0, 1, 0},
    {2, 0, 0, 0, 18, 0, 4, 0, -1, 1, 5, -1, -1, -1, -1, -1, 0, 0, 1, 0},
    {38, 0, 0, 0, 19, 0, 3, 0, 3, 1, 5, -1, -1, -1, -1, -1, 0, 0, 1, 0},
    {4, 0, 0, 0, 22, 4, 5, 0, 10, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {0, 8, 0, 0, 1, 2, 0, 4, -1, -1, 13, 14, 15, 16, 9, 11, 11, 0, 0, 0},
    {0, 8, 0, 0, 10, 2, 0, 6, -1, -1, 13, 14, 15, 16, 9, 12, -1, 0, 0, 0},
    {0, 8, 0, 0, 10, 2, 0, 0, -1, -1, 13, 14, 15, 16, 9, 99, -1, 0, 0, 0},
    {2, 0, 0, 0, 15, 0, 4, 0, -1, 18, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {2, 0, 0, 0, 17, 0, 4, 0, -1, 9, 13, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {2, 0, 0, 0, 18, 0, 4, 0, -1, 9, 13, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 19, 0, 3, 0, 11, 9, 13, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 23, 0, 3, 0, 3, 1, 10, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {4, 0, 0, 0, 22, 4, 5, 0, 19, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {4, 0, 0, 0, 1, 2, 0, 4, -1, -1, 13, 14, 15, 16, 18, 20, 20, 0, 0, 0},
    {38, 0, 0, 0, 23, 0, 3, 0, 10, 18, 13, -1, -1, -1, -1, -1, -1, 0, 1, 0},
}};
constexpr std::array<SaveDisplayRow, 12> kVariant8Rows{{
    {0, 8, 0, 0, 1, 5, 0, 4, -1, -1, 3, 4, 5, 6, 7, 1, 1, 0, 0, 0},
    {0, 8, 0, 0, 26, 5, 0, 6, -1, -1, 3, 4, 5, 6, 7, 2, -1, 0, 0, 0},
    {0, 8, 0, 0, 26, 5, 0, 0, -1, -1, 3, 4, 5, 6, 7, 99, -1, 0, 0, 0},
    {2, 0, 0, 0, 16, 0, 4, 0, -1, 9, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {2, 0, 0, 0, 17, 0, 4, 0, -1, 8, 3, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {2, 0, 0, 0, 18, 0, 4, 0, -1, 8, 3, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 19, 0, 3, 0, 1, 8, 3, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {2, 0, 0, 0, 21, 7, 4, 0, -1, 8, 3, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {4, 0, 0, 0, 27, 7, 5, 0, 0, -1, 3, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {4, 0, 0, 0, 27, 7, 5, 0, 10, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {0, 8, 0, 0, 1, 5, 0, 4, -1, -1, 3, 4, 5, 6, 7, 11, 11, 0, 0, 0},
    {38, 0, 0, 0, 23, 0, 3, 0, 1, 9, 3, -1, -1, -1, -1, -1, -1, 0, 1, 0},
}};
constexpr std::array<SaveDisplayRow, 12> kVariant9Rows{{
    {0, 8, 0, 0, 1, 2, 0, 9, -1, -1, 3, 4, 5, 1, 7, 1, 1, 0, 0, 0},
    {0, 8, 0, 0, 4, 2, 0, 11, -1, -1, 3, 4, 5, 6, 7, 2, -1, 0, 0, 0},
    {0, 8, 0, 0, 4, 2, 0, 0, -1, -1, 3, 4, 5, 6, 7, 9, -1, 0, 0, 0},
    {2, 0, 0, 0, 15, 0, 4, 0, -1, 10, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {2, 0, 0, 0, 17, 0, 4, 0, -1, 10, 3, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {2, 0, 0, 0, 18, 0, 4, 0, -1, 10, 3, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 19, 0, 3, 0, 1, 10, 3, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {2, 0, 0, 0, 21, 4, 4, 0, -1, 10, 3, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {0, 8, 0, 0, 1, 1, 0, 4, -1, -1, 3, 4, 5, 1, 7, 11, 11, 0, 0, 0},
    {0, 8, 0, 0, 5, 3, 6, 0, -1, -1, -1, -1, -1, -1, -1, 99, -1, 0, 1, 0},
    {4, 0, 0, 0, 28, 4, 5, 0, 8, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0},
    {38, 0, 0, 0, 23, 0, 3, 0, 1, 10, 3, -1, -1, -1, -1, -1, -1, 0, 1, 0},
}};

std::array<S1SaveImage, 2> g_saveImages{};
std::size_t g_activeSaveImageIndex = 0;

void SAVE_IdleDisplayTask(SaveTaskData *) {
}
void SAVE_UpdateDisplayModuleStateMachine(SaveTaskData *data, std::int64_t, float);
void SAVE_FadeOutDisplayModule(SaveTaskData *data, std::int64_t, float);
void SAVE_CloseDisplayModuleFlow(SaveTaskData *data, std::int64_t, float);
void SAVE_FadeIntoDisplayModule(SaveTaskData *data, std::int64_t, float);
void SAVE_EnterDisplayModule(SaveTaskData *data, std::int64_t, float);

PadCaptureTaskData *CurrentPadCaptureData() {
    return g_pPadCaptureTask && g_pPadCaptureTask->task_data_ptr ? reinterpret_cast<PadCaptureTaskData *>(g_pPadCaptureTask->task_data_ptr) : nullptr;
}


PadCaptureEntry *BindPadCapture(PadCaptureEntry *entry) {
    PadCaptureTaskData *const capture = CurrentPadCaptureData();
    if (!capture) {
        return nullptr;
    }
    if (entry && (entry->flags & 1u) == 0) {
        entry = nullptr;
    }

    PadCaptureEntry *const previous = capture->currentBoundEntry;
    if (previous == entry) {
        return previous;
    }
    if (previous) {
        previous->previousHeld = previous->held;
        previous->unknownMask = 0;
        previous->held = 0;
        previous->pressedPriority = 0;
        previous->repeatedPresses = 0;
        previous->triggers = 0;
        previous->rightStick = 0x8080;
        previous->flags &= ~2u;
    }
    if (entry) {
        const InputSlot &input = g_inputState.slots[0];
        entry->enabled = input.enabled;
        entry->unknownMask = input.unknownMask;
        entry->held = input.held;
        entry->previousHeld = input.previousHeld;
        entry->pressedPriority = input.pressedPriority;
        entry->repeatedPresses = input.repeatedPresses;
        entry->triggers = input.triggers;
        entry->rightStick = input.rightStick;
        entry->leftStick = input.leftStick;
        entry->xinputReserved = g_xinputReserved;
        entry->flags |= 2u;
    }
    capture->currentBoundEntry = entry;
    return previous;
}

void InvokeElementStartup(UIBaseRuntime &element) {
    if (!element.isVisible) {
        std::int64_t (*callback)() = nullptr;
        if (element.render) {
            element.render();
            callback = element.tick;
        } else if (element.initialize) {
            element.initialize();
            callback = element.postInit;
        }
        if (callback) {
            g_waitingForNextTick = 0;
            callback();
            g_waitingForNextTick = 1;
        }
        element.isVisible = 1;
    }
    element.isHandledExternally = 0;
}

void DestroyElementForSave(UIBaseRuntime &element) {
    if (!element.isVisible || !element.destroy || element.destroy() == 1 || g_busy_reset == 1)
        element.isVisible = 0;
}

constexpr std::size_t kSaveSurfaceSlotsOffset = 0x590;
constexpr std::size_t kSaveSurfaceSlotCount = 16;

std::int32_t *SaveSurfaceSlots(SaveTaskData &data) {
    constexpr std::size_t offset = kSaveSurfaceSlotsOffset - offsetof(SaveTaskData, reserved_008);
    return reinterpret_cast<std::int32_t *>(data.reserved_008 + offset);
}


void InitializeSaveSurfaceSlots(SaveTaskData &data) {
    for (std::size_t index = 0; index < kSaveSurfaceSlotCount; ++index)
        SaveSurfaceSlots(data)[index] = -1;
}


void SubmitSaveFade(std::uint32_t alpha, std::int32_t screenMode, float maxExtent) {
    OriginalStrip strip{};
    strip.corner0 = alpha << 24;
    strip.vertexCount = 0;
    strip.normalizedY1 = 0.0f;
    strip.normalizedX2 = 0.0f;
    strip.normalizedX3 = 1.0f;
    strip.normalizedY2 = 1.0f;
    strip.normalizedY3 = 0.0f;
    AdjustAspectRatioMatrix(&strip, 0, 0, 0, 640, 480, screenMode);
    strip.flags |= 0x400U;
    ApplyFadeToPacket(&strip, maxExtent);
}

S1SaveImage &InactiveSaveImage() {
    return g_saveImages[g_activeSaveImageIndex ^ 1u];
}

void PublishInactiveSaveImage() {
    g_activeSaveImageIndex ^= 1u;
}

std::uint32_t ActiveSaveIdentifier() {
    std::uint32_t identifier = 0;
    std::memcpy(&identifier, ActiveSaveImage().bytes.data() + kS1SaveInternalHeaderOffset, sizeof(identifier));
    return identifier;
}

} // namespace

S1SaveImage &ActiveSaveImage() {
    return g_saveImages[g_activeSaveImageIndex];
}


void InitSaveData() {
    S1Save_InitializeImage(ActiveSaveImage());
}


std::int64_t shenSetupPlayerDefaultsNewGame() {
    const std::uint32_t settingsFlags = S1SettingsFlags_Get();
    g_shenmueFlags.fill(0);
    g_scenarioStepLut = {};
    shenInitDefaultSettings();
    shenInitDefaultStats();
    shenInitDefaultCassetteInventory();
    InternalInit();
    Arcade_InitDefaultHangOnRanking();
    Arcade_InitDefaultSpaceHarrierRanking();
    g_tmp_cm_path[0] = 0;
    g_prevCycleSeconds = 0;
    g_loadedSaveBlock = -1;
    const std::int32_t result = S1Save_ReadCurrentSlot(std::filesystem::path{"save"} / g_currentSaveSlot.data(), ActiveSaveImage());
    if (result < -4 || result == -2 || result > 0) {
        S1SettingsFlags_Set(settingsFlags);
        return 0;
    }
    if (result < 0)
        InitSaveData();
    g_homeSnapshotApplyPending = 0;
    S1SettingsFlags_Set(settingsFlags);
    return 1;
}



std::int64_t LoadScene(std::int32_t block) {
    const std::uint32_t settingsFlags = S1SettingsFlags_Get();
    if (block > 3)
        return 0;
    g_shenmueFlags.fill(0);
    g_scenarioStepLut = {};
    shenInitDefaultSettings();
    shenInitDefaultStats();
    shenInitDefaultCassetteInventory();
    InternalInit();
    Arcade_InitDefaultHangOnRanking();
    Arcade_InitDefaultSpaceHarrierRanking();
    g_prevCycleSeconds = 0;
    g_tmp_cm_path[0] = 0;
    g_loadedSaveBlock = -1;
    S1SaveImage &candidate = InactiveSaveImage();
    g_lastS1SaveResult = S1Save_Load(std::filesystem::path{"save"} / g_currentSaveSlot.data(), candidate, block);
    S1SettingsFlags_Set(settingsFlags);
    if (g_lastS1SaveResult != S1SaveResult::Success)
        return 0;
    PublishInactiveSaveImage();
    g_currentSaveSlot[0] = '\0';
    InitialiseShenmueSettings();
    return 1;
}

namespace {

// SaveGame_WriteQueuedSlot: 3 for a full store (-5), 4 for any other failure, otherwise the slot summary cache
// takes the active image's summary and the status becomes 2. Local slot files have no storage quota, so 3 is not produced here.
std::int32_t SaveGame_WriteQueuedSlot(void *) {
    const std::int32_t slot = g_saveWriteSlot;
    const S1SaveResult result = S1Save_Write(HPLG_SaveSlotPath(static_cast<std::size_t>(slot)), ActiveSaveImage());
    g_lastS1SaveResult = result;
    if (result != S1SaveResult::Success) {
        g_saveWriteStatus.exchange(4);
        return 0;
    }
    if (slot <= 9) {
        S1SaveSummary summary{};
        std::memcpy(&summary, ActiveSaveImage().bytes.data() + kS1SaveSummaryOffset, sizeof(summary));
        HPLG_SetSaveSummary(static_cast<std::size_t>(slot), summary);
    }
    g_saveWriteStatus.exchange(2);
    return 0;
}

} // namespace



void SaveGame_0(std::int32_t slot) {
    InactiveSaveImage() = ActiveSaveImage();
    if (!ActiveSaveIdentifier())
        InitSaveData();
    const S1SaveResult capture = S1Save_Capture(ActiveSaveImage(), g_saveTaskData && g_saveTaskData->mode != 0);
    if (capture != S1SaveResult::Success) {
        g_lastS1SaveResult = capture;
        g_saveWriteStatus.exchange(4);
        return;
    }
    g_saveWriteStatus.exchange(1);
    g_saveWriteSlot = slot;
    const d3t::D3TTaskDescriptor task{.data = nullptr, .callback = SaveGame_WriteQueuedSlot, .completion = nullptr, .priority = 2};
    d3t::d3t_QueueTask(&task, 4);
}



std::int32_t BOOK_KEEP(void *) {
    const std::int32_t result = UploadBookkeepingGlobalStats();
    g_saveWriteStatus.exchange(result == -5 ? 3 : result ? 4 : 2);
    return 0;
}

bool SAVE_AccessScriptData(std::uint32_t mode, std::uint32_t offset, std::uint32_t value, std::uint32_t &result) {
    const std::uint32_t bit = mode <= 2 ? 1U << (offset & 31U) : 0;
    switch (mode) {
    case 0:
        g_saveScriptFlags |= bit;
        return false;
    case 1:
        g_saveScriptFlags &= ~bit;
        return false;
    case 2:
        result = g_saveScriptFlags & bit;
        return true;
    case 3:
        if (offset < g_saveDataRelated.size())
            g_saveDataRelated[offset] = value;
        return false;
    case 4:
        result = offset < g_saveDataRelated.size() ? g_saveDataRelated[offset] : 0;
        return true;
    default:
        return false;
    }
}

std::uint32_t SAVE_GetArcadeHighScore(std::size_t slot) {
    return slot < g_arcadeHighScores.size() ? g_arcadeHighScores[slot] : 0;
}

void SAVE_SetArcadeHighScore(std::size_t slot, std::uint32_t value) {
    if (slot < g_arcadeHighScores.size())
        g_arcadeHighScores[slot] = value;
}

void SAVE_Callback(HLib::TaskData *) {
    g_saveTaskData->inputCapture = g_saveTaskData->mode != 0 ? nullptr : AllocateClearedPADCEntry();
    if (CurrentTask) {
        CurrentTask->initCallbackFnPtr = nullptr;
        CurrentTask->mainTaskCallbackFnPtr = SAVE_Callback_0;
        CurrentTask->autoInitTaskCallbackFnPtr = nullptr;
    }
}

void SAVE_Callback_0(HLib::TaskData *) {
    g_saveTaskData->completionCallback(g_saveTaskData);
}


void Save_CompletionTaskCallback(SaveTaskData *data) {
    data->inputState = data->mode ? static_cast<void *>(&g_inputState.slots[0]) : static_cast<std::byte *>(data->inputCapture) + 4;
    data->displayModuleFlow(data, 0, 0.0f);
    const float blend = g_saveDisplayModuleState == SAVE_DISPLAY_MODULE_TITLE_LOAD ? 0.07f : 0.2f;
    data->fadeAlpha = (static_cast<float>(data->fadeAlphaTarget) - data->fadeAlpha) * blend + data->fadeAlpha;
    if (data->fadeAlpha < 0.0f)
        data->fadeAlpha = 0.0f;
    if (data->fadeAlpha > 255.0f)
        data->fadeAlpha = 255.0f;
    if (g_updateRenderFrameMode)
        SubmitSaveFade(static_cast<std::uint32_t>(static_cast<std::int32_t>(data->fadeAlpha)), 3, 6.0f);
    g_warningShowColour = 0;
}

std::int64_t SAVE_Init(std::int32_t mode) {
    HLib::Task *task = HLib_EnqueueTaskWithoutParameter(SAVE_Callback, 14, 2, SHEN_NAME(SAVE));
    if (task) {
        if (!task->task_data_ptr) {
            auto *const data = static_cast<SaveTaskData *>(HLib_AllocHeapMemBlock(sizeof(SaveTaskData), SHEN_NAME(TASK)));
            task->task_data_ptr = reinterpret_cast<HLib::TaskData *>(data);
            if (data) {
                std::memset(data, 0, sizeof(*data));
                InitializeSaveSurfaceSlots(*data);
                HLib_EnableTokenCheck(data);
            }
        }
        if (!task->task_data_ptr) {
            task->initCallbackFnPtr = nullptr;
            task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
            task->autoInitTaskCallbackFnPtr = nullptr;
            task = nullptr;
        }
    }

    g_saveTask = task;
    if (task) {
        g_saveTaskData = reinterpret_cast<SaveTaskData *>(task->task_data_ptr);
        g_saveTaskData->mode = mode;
        if (mode) {
            LCDM_Init();
            WIND_Init();
            WRN_CtrlWarning_Enqueue();
        }
        g_saveTaskData->completionCallback = SAVE_IdleDisplayTask;
        g_saveTaskData->displayModuleFlow = SAVE_InitDisplayModuleFlow;
        g_saveTaskData->field_000 &= 0xffff0000u;
    } else {
        g_saveTaskData = nullptr;
    }

    SAVE_SetDisplayModuleStateAndTimeFlags(SAVE_DISPLAY_MODULE_NONE);
    g_temporarySaveSceneId = g_sceneId;
    g_temporarySaveAreaId = g_areaId;
    g_saveScreenFlag = 0;
    g_temporarySaveEntryPoint = g_entryPoint;
    return g_entryPoint;
}

// Map teardown: the module tree destroys the SAVE task with its data.
void SAVE_Shutdown() {
    HLib::Task *const task = g_saveTask;
    g_saveTask = nullptr;
    g_saveTaskData = nullptr;
    if (task && (task->mainTaskCallbackFnPtr == SAVE_Callback || task->mainTaskCallbackFnPtr == SAVE_Callback_0))
        HLib_DestroyTask(task);
}

void SAVE_SetOrEnqueueUiStateBits(std::int32_t state) {
    if (g_saveTaskData->mode)
        return;
    const std::uint16_t current = UI_GetPresentationSnapshot().sicnState;
    if (state) {
        if (current) {
            if (current != static_cast<std::uint16_t>(state))
                UI_SICNSetState(static_cast<std::uint16_t>(state), UISicnPausePolicy::KeepOnPause);
            return;
        }
    } else if (current) {
        UI_SICNSetState(0, UISicnPausePolicy::KeepOnPause);
        return;
    }
    UI_SICNEnqueue(static_cast<std::uint16_t>(state), UISicnPausePolicy::KeepOnPause);
}

void SAVE_InitDisplayModuleFlow(SaveTaskData *data, std::int64_t, float) {
    g_saveDisplayInitScale = 1.0f;
    data->displayInitState = 0;
    data->fadeAlphaTarget = 0;
    data->fadeAlpha = 0.0f;
    data->fadeFrame = 0;
    data->flowActive = 1;
    data->previousUiState = UI_GetPresentationSnapshot().sicnState;
    data->flowFrame = 0;
    data->fadeInAlpha = 0;
    if (!data->mode)
        data->previousInputCapture = BindPadCapture(static_cast<PadCaptureEntry *>(data->inputCapture));

    std::int32_t uiState = 38;
    switch (g_saveDisplayModuleState) {
    case SAVE_DISPLAY_MODULE_SAVE_GRID:
        data->flowActive = 0;
        data->closeAsAccepted = 0;
        uiState = -1;
        break;
    case SAVE_DISPLAY_MODULE_SCRIPTED:
    case SAVE_DISPLAY_MODULE_TITLE_LOAD:
        uiState = 4;
        break;
    case SAVE_DISPLAY_MODULE_SAVE_PROGRESS:
        if (g_playerStats.currentSaveProgress < 255)
            ++g_playerStats.currentSaveProgress;
        break;
    case SAVE_DISPLAY_MODULE_VARIANT8:
        uiState = -1;
        break;
    default:
        break;
    }
    data->flowFrame = 0;
    data->fadeInAlpha = 0;
    data->displayModuleFlow = SAVE_UpdateDisplayModuleStateMachine;
    data->stateIndex = 0;
    if (uiState >= 0)
        SAVE_SetOrEnqueueUiStateBits(uiState);
}

namespace {

const SaveDisplayRow *SaveDisplayStateTable(std::int32_t state) {
    switch (state) {
    case SAVE_DISPLAY_MODULE_SAVE_GRID:
        return kSaveGridRows.data();
    case SAVE_DISPLAY_MODULE_SCRIPTED:
        return kScriptedRows.data();
    case SAVE_DISPLAY_MODULE_SAVE_PROGRESS:
        return kSaveProgressRows.data();
    case SAVE_DISPLAY_MODULE_TITLE_LOAD:
        return kTitleLoadRows.data();
    case SAVE_DISPLAY_MODULE_VARIANT8:
        return kVariant8Rows.data();
    case SAVE_DISPLAY_MODULE_VARIANT9:
        return kVariant9Rows.data();
    default:
        return kDefaultRows.data();
    }
}

void SAVE_UpdateDisplayModuleStateMachine(SaveTaskData *data, std::int64_t, float) {
    std::int32_t next = data->stateIndex;
    const SaveDisplayRow &row = SaveDisplayStateTable(g_saveDisplayModuleState)[next];
    if (!data->stateFrame && row[kRowEntrySound])
        shenExecuteAICACommand(kPlaySuccessSound, 0, 0);
    g_bSkipCycles = static_cast<std::uint8_t>(row[kRowSkipCycles]);
    switch (row[kRowLcdm]) {
    case 0:
        lcdmInit(0);
        break;
    case 1:
        lcdmInit(3);
        break;
    case 2:
        lcdmInit(5);
        break;
    case 3:
    case 6:
        lcdmInit(data->stateFrame > 45 ? 0 : 7);
        break;
    case 4:
    case 7:
        lcdmInit(9);
        break;
    case 5:
        lcdmInit(11);
        break;
    default:
        break;
    }

    const std::uint16_t pressed = static_cast<const InputSlot *>(data->inputState)->pressedPriority;
    switch (row[kRowAction]) {
    case 1:
        g_uiElements[kUiLoadSave].isActive = 1;
        if (g_hasConfirmedButton == 1)
            next = row[kRowCancel];
        else if (g_hasConfirmedButton == 2)
            next = row[kRowAccept];
        if (g_hasConfirmedButton)
            g_hasConfirmedButton = 0;
        break;
    case 2:
    case 3:
        data->transitionByte = 0;
        next = row[kRowAccept];
        break;
    case 4:
        if ((pressed & 0x200U) != 0)
            next = row[kRowCancel];
        break;
    case 5:
        if ((pressed & 0x400U) != 0)
            next = row[kRowAccept];
        break;
    case 6:
        if (data->stateFrame < 45) {
            ++data->stateFrame;
            return;
        }
        break;
    default:
        break;
    }

    if (row[kRowNext] >= 0)
        next = row[kRowNext];
    ++data->stateFrame;
    if (data->stateIndex == next)
        return;
    data->transitionByte = 0;
    data->stateIndex = next;
    data->stateFrame = 0;
    if (next == 98 || next == 99) {
        g_bSkipCycles = 1;
        data->displayModuleFlow = SAVE_FadeOutDisplayModule;
        data->closeAsAccepted = next != 99;
    }
}

void SAVE_FadeOutDisplayModule(SaveTaskData *data, std::int64_t, float) {
    data->fadeAlphaTarget = g_saveDisplayModuleState == SAVE_DISPLAY_MODULE_TITLE_LOAD ? 190 : 118;
    g_warningMessageIndex = 0;
    g_warningChoiceOrDisc = 0;
    if (data->flowFrame)
        data->flowFrame = 0;
    data->fadeFrame = 0;
    if (g_saveDisplayModuleState == SAVE_DISPLAY_MODULE_TITLE_LOAD) {
        data->displayModuleFlow = SAVE_CloseDisplayModuleFlow;
        return;
    }
    ++data->stateFrame;
    if (data->fadeAlpha <= 15.0f || data->stateFrame >= 45) {
        data->stateFrame = 0;
        data->displayModuleFlow = SAVE_CloseDisplayModuleFlow;
    }
    data->fadeAlphaTarget = 0;
}

void SAVE_CloseDisplayModuleFlow(SaveTaskData *data, std::int64_t, float) {
    lcdmInit(0);
    if (!data->mode) {
        Shenmue_nPauseState &= ~PAUSE_STATE_FREE;
        BindPadCapture(static_cast<PadCaptureEntry *>(data->previousInputCapture));
    }
    const std::int32_t state = g_saveDisplayModuleState;
    std::int32_t *const surfaces = SaveSurfaceSlots(*data);
    bool restoreUiState = true;
    if (state == SAVE_DISPLAY_MODULE_SAVE_GRID) {
        if (surfaces[0] >= 0)
            PVR_FreeAllocatedSurfaceByIndex(surfaces[0]);
    } else {
        if (state != SAVE_DISPLAY_MODULE_VARIANT8) {
            for (std::size_t index = 0; index < kSaveSurfaceSlotCount; ++index) {
                if (surfaces[index] >= 0)
                    PVR_FreeAllocatedSurfaceByIndex(surfaces[index]);
            }
        }
        if (state == SAVE_DISPLAY_MODULE_SLEEP_SAVE || state == SAVE_DISPLAY_MODULE_ALT_SAVE)
            g_sleepSaveTrainingMenu.requested = 1;
        if (state == SAVE_DISPLAY_MODULE_SLEEP_SAVE) {
            if (!g_saveTaskData->mode) {
                const std::uint16_t current = UI_GetPresentationSnapshot().sicnState;
                if (!current)
                    UI_SICNEnqueue(3, UISicnPausePolicy::KeepOnPause);
                else if (current != 3)
                    UI_SICNSetState(3, UISicnPausePolicy::KeepOnPause);
            }
            restoreUiState = false;
        } else if (state == SAVE_DISPLAY_MODULE_TITLE_LOAD) {
            data->displayModuleFlow = SAVE_FadeIntoDisplayModule;
            data->fadeInAlpha = 118;
            if (!data->closeAsAccepted) {
                UI_SICNSetState(0, UISicnPausePolicy::KeepOnPause);
                return;
            }
            restoreUiState = false;
        }
    }
    if (restoreUiState)
        SAVE_SetOrEnqueueUiStateBits(data->previousUiState);
    data->completionCallback = SAVE_IdleDisplayTask;
    data->displayModuleFlow = SAVE_InitDisplayModuleFlow;
    SAVE_SetDisplayModuleStateAndTimeFlags(SAVE_DISPLAY_MODULE_NONE);
}

void SAVE_FadeIntoDisplayModule(SaveTaskData *, std::int64_t, float) {
    SaveTaskData *const data = g_saveTaskData;
    if (g_updateRenderFrameMode)
        SubmitSaveFade(static_cast<std::uint32_t>(data->fadeInAlpha), 4, 16.0f);
    if (data->fadeInAlpha == 255) {
        data->displayModuleFlow = SAVE_EnterDisplayModule;
        return;
    }
    data->fadeInAlpha += 4;
    if (data->fadeInAlpha >= 255)
        data->fadeInAlpha = 255;
}

void SAVE_EnterDisplayModule(SaveTaskData *, std::int64_t, float) {
    SaveTaskData *const data = g_saveTaskData;
    data->completionCallback = SAVE_IdleDisplayTask;
    data->displayModuleFlow = SAVE_InitDisplayModuleFlow;
    SAVE_SetDisplayModuleStateAndTimeFlags(SAVE_DISPLAY_MODULE_NONE);
    if (g_updateRenderFrameMode)
        SubmitSaveFade(0xff, 4, 16.0f);
    g_ClearColour = 0xff000000;
    if (!g_saveTaskData->closeAsAccepted)
        HLib_UpdateTaskQueue(SystemModule::Display);
}

} // namespace

void SAVE_SetDisplayModuleStateAndTimeFlags(std::int32_t state) {
    if (g_saveDisplayModuleState == state)
        return;
    g_saveDisplayModuleState = state;
    if (state == SAVE_DISPLAY_MODULE_NONE) {
        if (g_currUIState == -1 || g_currUIState == kUiLoadSave)
            DestroyElementForSave(g_uiElements[kUiLoadSave]);
        if (g_currUIState == -1 || g_currUIState == kUiGameMenuBackground)
            DestroyElementForSave(g_uiElements[kUiGameMenuBackground]);
        if (g_timeValue)
            g_shouldTimeAdvance = 1;
    } else if (g_timeValue) {
        g_shouldTimeAdvance = 0;
    }
}



std::int32_t SAVE_OpenDisplayModule(std::int32_t state) {
    if (g_saveDisplayModuleState != SAVE_DISPLAY_MODULE_NONE)
        return SAVE_DISPLAY_MODULE_NONE;
    if (g_currUIState == -1 || g_currUIState == kUiGameMenuBackground)
        InvokeElementStartup(g_uiElements[kUiGameMenuBackground]);
    g_saveOrLoad = 1;
    if (g_currUIState == -1 || g_currUIState == kUiLoadSave)
        InvokeElementStartup(g_uiElements[kUiLoadSave]);
    SAVE_SetDisplayModuleStateAndTimeFlags(state);
    g_saveTaskData->completionCallback = Save_CompletionTaskCallback;
    return g_saveDisplayModuleState;
}

void Script_saveDisplayUtils(ScenarioContext *context, void *rawParams) {
    if (!context || !context->stack || !rawParams)
        return;
    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    switch (params[0]) {
    case 0:
        context->stack->accumulator = static_cast<std::uint32_t>(SAVE_OpenDisplayModule(SAVE_DISPLAY_MODULE_SCRIPTED));
        break;
    case 1:
        context->stack->accumulator = static_cast<std::uint32_t>(g_saveDisplayModuleState);
        break;
    case 2:
    case 3:
        g_temporarySaveSceneId = static_cast<std::int32_t>(params[1]);
        g_temporarySaveAreaId = params[2];
        g_temporarySaveEntryPoint = params[3];
        break;
    default:
        break;
    }
}

void SAVE_Script_UpdateForSaveData(ScenarioContext *context, void *rawParams) {
    if (!context || !context->stack || !rawParams)
        return;

    const auto *const params = static_cast<const std::uint32_t *>(rawParams);
    std::uint32_t result = 0;
    if (SAVE_AccessScriptData(params[0], params[1], params[2], result)) {
        context->stack->accumulator = result;
    }
}




} // namespace shenmue::s1
