#include "s2/arcade_rankings_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s1/arcade_scores.hpp"
#include "s2/arcade_modules_dc.hpp"

#include <cstring>
#include "s2/system_modules_dc.hpp"
#include "s2/frame_dc.hpp"
#include "s2/pause.hpp"
#include "s2/player_stats_dc.hpp"
#include "s2/scene_data.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/initialization_dc.hpp"
#include "s2/input_dc.hpp"
#include "platform/dc/weather_texture.hpp"
#include "platform/dc/aica.hpp"
#include "platform/dc/aica_stream.hpp"
#include "platform/dc/ninja.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/file_loader_dc.hpp"
#include <sg_gd.h>
#if SHENMUE_S2_GCC295
#define SHENMUE_SYSTEM_MODULE(name) SystemModule_##name
#define SHENMUE_SYSTEM_MODULE_NULL 0
#else
#define SHENMUE_SYSTEM_MODULE(name) SystemModule::name
#define SHENMUE_SYSTEM_MODULE_NULL nullptr
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif
#if SHENMUE_S2_GCC295
#else
#endif

namespace shenmue {
namespace s2 {

std::uint32_t DecodeArcadeRankingScore(const std::uint8_t *digits);

std::uint32_t DecodeArcadeRankingScore(const std::uint8_t *digits);
std::uint32_t DecodeArcadeRankingScore(const std::uint8_t *digits);
std::uint32_t DecodeArcadeRankingScore(const std::uint8_t *digits);
std::uint32_t DecodeArcadeRankingScore(const std::uint8_t *digits);
ArcadeRankingRecord outRunRankingStorage[20], afterBurnerRankingStorage[20], hangOnRankingStorage[20], harrierRankingStorage[20];
ArcadeRankingRecord *g_outRunRankings = outRunRankingStorage;
ArcadeRankingRecord *g_afterBurnerRankings = afterBurnerRankingStorage;
ArcadeRankingRecord *g_hangOnRankings = hangOnRankingStorage;
ArcadeRankingRecord *g_harrierRankings = harrierRankingStorage;

struct ArcadeRankingTable {
    ArcadeRankingRecord records[20];
};

// @verified - d
void ResetHangOnRankings() {
    std::memset(g_hangOnRankings, 0, sizeof(hangOnRankingStorage));
    *reinterpret_cast<ArcadeRankingTable *>(g_hangOnRankings) = *reinterpret_cast<const ArcadeRankingTable *>(s1::kHangOnDefaultRankingImage);
    for (std::uint32_t index = 0; index < 20; ++index)
        g_hangOnRankings[index].score = DecodeArcadeRankingScore(s1::kHangOnDefaultRankingImage[index]);
}

// @verified - d
void HangOnVblankNullsub() {
}

void ResetSpaceHarrierRankings() {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    for (std::int32_t index = 0; index < 20; ++index) {
        g_harrierRankings[index].score = DecodeArcadeRankingScore(kSpaceHarrierModuleDefaultRankings[index]);
        reinterpret_cast<char *>(&g_harrierRankings[index].initials)[0] = static_cast<char>(kSpaceHarrierModuleDefaultRankings[index][4]);
        reinterpret_cast<char *>(&g_harrierRankings[index].initials)[1] = static_cast<char>(kSpaceHarrierModuleDefaultRankings[index][5]);
        reinterpret_cast<char *>(&g_harrierRankings[index].initials)[2] = static_cast<char>(kSpaceHarrierModuleDefaultRankings[index][6]);
        reinterpret_cast<char *>(&g_harrierRankings[index].initials)[3] = static_cast<char>(kSpaceHarrierModuleDefaultRankings[index][7]);
    }
#else
    for (std::int32_t index = 0; index < 20; ++index) {
        const std::uint8_t (&source)[10] = s1::kSpaceHarrierDefaultRanking[index];
        g_harrierRankings[index].score = DecodeArcadeRankingScore(source);
        for (std::int32_t character = 0; character < 4; ++character)
            reinterpret_cast<char *>(&g_harrierRankings[index].initials)[character] = static_cast<char>(source[character + 4]);
    }
#endif
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
void HarrierVblankNullsub() {
}
#endif

void ResetOutRunRankings() {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    for (std::int32_t index = 0; index < 20; ++index) {
        g_outRunRankings[index].score = DecodeArcadeRankingScore(kOutRunModuleDefaultRankings[index]);
        reinterpret_cast<char *>(&g_outRunRankings[index].initials)[0] = static_cast<char>(kOutRunModuleDefaultRankings[index][4]);
        reinterpret_cast<char *>(&g_outRunRankings[index].initials)[1] = static_cast<char>(kOutRunModuleDefaultRankings[index][5]);
        reinterpret_cast<char *>(&g_outRunRankings[index].initials)[2] = static_cast<char>(kOutRunModuleDefaultRankings[index][6]);
        reinterpret_cast<char *>(&g_outRunRankings[index].initials)[3] = static_cast<char>(kOutRunModuleDefaultRankings[index][7]);
    }
#else
    for (std::int32_t index = 0; index < 20; ++index) {
        const std::uint8_t (&source)[10] = s1::kSpaceHarrierDefaultRanking[index];
        g_outRunRankings[index].score = DecodeArcadeRankingScore(source);
        for (std::int32_t character = 0; character < 4; ++character)
            reinterpret_cast<char *>(&g_outRunRankings[index].initials)[character] = static_cast<char>(source[character + 4]);
    }
#endif
}

void ResetAfterBurnerRankings() {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    for (std::int32_t index = 0; index < 20; ++index) {
        g_afterBurnerRankings[index].score = DecodeArcadeRankingScore(s1::kSpaceHarrierDefaultRanking[index]);
        reinterpret_cast<char *>(&g_afterBurnerRankings[index].initials)[0] = static_cast<char>(s1::kSpaceHarrierDefaultRanking[index][4]);
        reinterpret_cast<char *>(&g_afterBurnerRankings[index].initials)[1] = static_cast<char>(s1::kSpaceHarrierDefaultRanking[index][5]);
        reinterpret_cast<char *>(&g_afterBurnerRankings[index].initials)[2] = static_cast<char>(s1::kSpaceHarrierDefaultRanking[index][6]);
        reinterpret_cast<char *>(&g_afterBurnerRankings[index].initials)[3] = static_cast<char>(s1::kSpaceHarrierDefaultRanking[index][7]);
    }
#else
    for (std::int32_t index = 0; index < 20; ++index) {
        const std::uint8_t (&source)[10] = s1::kSpaceHarrierDefaultRanking[index];
        g_afterBurnerRankings[index].score = DecodeArcadeRankingScore(source);
        for (std::int32_t character = 0; character < 4; ++character)
            reinterpret_cast<char *>(&g_afterBurnerRankings[index].initials)[character] = static_cast<char>(source[character + 4]);
    }
#endif
}

void AfterBurnerVblankNullsub() {
}

} // namespace s2
} // namespace shenmue

#endif
