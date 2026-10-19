#pragma once

#include "s2/hlib.hpp"
#include "common/types.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

inline constexpr shen_name TGLB_AUTHOR_AU_0{SHEN_NAME(AU_0)};
inline constexpr shen_name TGLB_AUTHOR_AU_1{SHEN_NAME(AU_1)};
inline constexpr shen_name TGLB_AUTHOR_AU_2{SHEN_NAME(AU_2)};
inline constexpr shen_name TGLB_AUTHOR_AU_7{SHEN_NAME(AU_7)};
inline constexpr shen_name TGLB_AUTHOR_AU_8{SHEN_NAME(AU_8)};
inline constexpr shen_name TGLB_AUTHOR_RYO{SHEN_NAME(RYO_)};
inline constexpr shen_name TGLB_AUTHOR_LIGHT{SHEN_NAME(l501)};
inline constexpr std::array<shen_name, 4> tglbAuthorProps{SHEN_NAME(a334), SHEN_NAME(a372), SHEN_NAME(a532), SHEN_NAME(aa72)};
inline constexpr std::array<shen_name, 6> tglbAuthorDefaultCharacters{
    SHEN_NAME(JOP_), SHEN_NAME(SGT_), SHEN_NAME(REN_), SHEN_NAME(WON_), SHEN_NAME(TGY_), shen_name{0xffffffffU}};
inline constexpr std::array<shen_name, 7> tglbAuthorVictoryCharacters{
    SHEN_NAME(JOP_), SHEN_NAME(SGT_), SHEN_NAME(REN_), SHEN_NAME(WON_), SHEN_NAME(TGY_), SHEN_NAME(SRY_), shen_name{0xffffffffU}};
inline constexpr std::array<shen_name, 3> tglbAuthorFinalCharacters{SHEN_NAME(TGY_), SHEN_NAME(SYE_), shen_name{0xffffffffU}};


struct TglbAuthorContext {
    HLib::Task *task;
    std::uint32_t reserved08;
    shen_name authorCmd;
    std::uint16_t state;
    std::array<std::byte, 6> reserved12;
};
static_assert(sizeof(TglbAuthorContext) == 0x18);
static_assert(offsetof(TglbAuthorContext, authorCmd) == 0x0c);
static_assert(offsetof(TglbAuthorContext, state) == 0x10);

struct TglbAuthorLoopWork {
    TglbAuthorContext *context;
};
static_assert(sizeof(TglbAuthorLoopWork) == 8);

TglbAuthorContext *QueueTglbAuthorMain(shen_name command);
void TglbAuthorMain_Callback(void *taskData);
std::int64_t QueueTglbAuthorLoop(TglbAuthorContext *context);
void TglbAuthorLoop_Callback(void *taskData);
void SetupTglbAuthorCharacters(TglbAuthorContext *context);
std::int64_t RestoreTglbAuthorCharacters(TglbAuthorContext *context);

} // namespace shenmue::s2
