#pragma once

#include "s2/area_event.hpp"
#include "s2/event_characters.hpp"
#include "s2/hlib.hpp"
#include "common/math.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {

inline constexpr shen_name TGLB_AREA{SHEN_NAME(TGLB)};
inline constexpr shen_name TOGYU_NAME{SHEN_NAME(TGY_)};
inline constexpr std::array<shen_name, 9> togyuCharacterNames{
    SHEN_NAME(RYO_), SHEN_NAME(SYE_), TOGYU_NAME, SHEN_NAME(REN_), SHEN_NAME(JOP_), SHEN_NAME(WON_), SHEN_NAME(SGT_), SHEN_NAME(SRY_), shen_name{0xffffffffU}};

inline constexpr shen_name TOGYU_RYO{SHEN_NAME(RYO_)}, TOGYU_SYE{SHEN_NAME(SYE_)}, TOGYU_SGT{SHEN_NAME(SGT_)};
inline constexpr shen_name TOGYU_JOP{SHEN_NAME(JOP_)}, TOGYU_WON{SHEN_NAME(WON_)}, TOGYU_REN{SHEN_NAME(REN_)}, TOGYU_SRY{SHEN_NAME(SRY_)};
inline constexpr shen_name TOGYU_SPECTATOR{SHEN_NAME(a532)}, TOGYU_BASE{SHEN_NAME(BASE)}, TOGYU_INVALID{0xffffffffU};
inline constexpr shen_name TOGYU_EVENT_TOKEN{SHEN_NAME(EVNT)}, TOGYU_RETRY_TITLE{SHEN_NAME(RTR2)}, TOGYU_EXIT_SCENE{SHEN_NAME(0276)};
inline constexpr shen_name TOGYU_AUTHOR_AU_3{SHEN_NAME(AU_3)}, TOGYU_AUTHOR_AU_4{SHEN_NAME(AU_4)}, TOGYU_AUTHOR_AU_5{SHEN_NAME(AU_5)};
inline constexpr shen_name TOGYU_LIGHT{SHEN_NAME(l501)};
inline constexpr std::array<shen_name, 8> togyuHandCharacters{TOGYU_RYO, TOGYU_NAME, TOGYU_SYE, TOGYU_SGT, TOGYU_JOP, TOGYU_WON, TOGYU_REN, TOGYU_SRY};
inline constexpr std::array<shen_name, 6> togyuSpectators{TOGYU_SYE, TOGYU_REN, TOGYU_JOP, TOGYU_WON, TOGYU_SGT, TOGYU_SRY};
inline constexpr std::array<shen_name, 4> togyuLookCharacters{TOGYU_SGT, TOGYU_WON, TOGYU_JOP, TOGYU_REN};
inline constexpr std::array<shen_name, 5> togyuArrangementCharacters{TOGYU_JOP, TOGYU_SGT, TOGYU_SPECTATOR, TOGYU_WON, TOGYU_REN};
inline constexpr std::array<Vector3, 5> togyuArrangementPositions{Vector3{247.75999f, 244, 131.03f}, Vector3{248.19f, 244, 131.3f},
    Vector3{248.19f, 244, 131.3f}, Vector3{247.92999f, 244, 132.13f}, Vector3{263.26999f, 244, 131.57f}};
inline constexpr std::array<Vector3i, 5> togyuArrangementRotations{
    Vector3i{0, 45511, 0}, Vector3i{0, 43690, 0}, Vector3i{0, 10922, 0}, Vector3i{0, 49152, 0}, Vector3i{0, 22755, 0}};
inline constexpr std::array<shen_name, 4> togyuArrangementMotionCharacters{TOGYU_JOP, TOGYU_SGT, TOGYU_WON, TOGYU_REN};
inline constexpr std::array<std::uint16_t, 4> togyuArrangementMotions{0x6017, 0x6018, 0x601b, 0x6019};
inline constexpr Vector3 togyuRyoPosition{252.39999f, 244, 136.67999f}, togyuOpponentPosition{256.76001f, 244, 137.39999f};
inline constexpr Vector3i togyuRyoRotation{0, 49152, 0}, togyuOpponentRotation{0, 16384, 0};
inline constexpr Vector3 togyuAuthorPosition{255.87f, 244, 134.48f};
inline constexpr std::array<std::array<std::int32_t, 3>, 19> togyuIdleHandCommands{
    {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1},
        {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}}};
inline constexpr std::array<std::array<std::int32_t, 3>, 3> togyuQteWaitFrames{{{16, 20, 24}, {24, 30, 36}, {32, 40, 48}}};
inline constexpr std::int32_t TOGYU_RETRY_FLAG = 0x3f8, TOGYU_QTE_DIFFICULTY_NIBBLE = 78;
inline constexpr std::int32_t TOGYU_HEALTH_NIBBLE = 142, TOGYU_BATTLE_LOSS = 2, TOGYU_ENTRY_MOTION = 6880;
inline constexpr std::int32_t TOGYU_RYO_FIGHT_MOTION = 306, TOGYU_OPPONENT_FIGHT_MOTION = 2070, TOGYU_BATTLE_CAMERA = 5020;
inline constexpr std::int32_t TOGYU_MAIN_SOUND = 3465;
inline constexpr char togyuPackage[] = "TGLB", togyuAuthorBank[] = "0275", togyuAudioBank[] = "0275A000";
inline constexpr char togyuSetupTelop[] = "\xe5\xa4\x96\xe9\x96\x80\xe9\xa0\x82\xe8\x82\x98";
inline constexpr std::array<const char*, 4> togyuFirstSuccessVoices{"0275A014", "0275E017", "0275E018", "0275E019"}; // @text 0275A014 EN: " " | JA: "…くっ！" ; 0275E017 EN: " " | JA: "ぬうっ！?" ; 0275E018 EN: "I just can't stand rats!" | JA: "ネズミがチョロチョロしやがって！" ; 0275E019 EN: " " | JA: "うおおおぉーっ！"
inline constexpr std::array<const char*, 4> togyuLaterSuccessVoices{"0275A015", "0275E020", "0275E021", "0275E022"}; // @text 0275A015 EN: " " | JA: "…ふっ！" ; 0275E020 EN: " " | JA: "うおっ！" ; 0275E021 EN: "You scampering rat!" | JA: "ちょこまかとこのガキッ！" ; 0275E022 EN: "You're mine!" | JA: "くたばれぇーっ！"
inline constexpr std::array<std::int32_t, 3> togyuFirstSuccessVoiceDelays{10, 35, 110}, togyuLaterSuccessVoiceDelays{15, 50, 140};
inline constexpr Vector2 togyuFightRingCentre{256.32000732421875f, 135.17999267578125f};
inline constexpr float togyuFightRingRadius = 5.0f;
inline constexpr std::array<shen_name, 2> togyuFighters{TOGYU_RYO, TOGYU_NAME};
struct TogyuEventWork {
    HLib::Task *task;
    std::array<shen_name, 11> opponents;
    shen_name area;
    std::int32_t phase;
    std::int32_t nextState;
    std::uint32_t initialMotionStorage;
    std::array<std::uint32_t, 16> motionHandles;
    std::int32_t savedNpcBatchCount;
    EventReadCharactersWork *characters;
    std::array<std::byte, 0x28> reserved090;
    std::int32_t selectedOpponent;
    std::int32_t smokeId;
    std::array<std::byte, 0x80> reserved0c0;
    std::int32_t laterPhase;
    std::array<std::byte, 0x3c> reserved144;
};
static_assert(sizeof(TogyuEventWork) == 0x180);
static_assert(offsetof(TogyuEventWork, opponents) == 8);
static_assert(offsetof(TogyuEventWork, area) == 0x34);
static_assert(offsetof(TogyuEventWork, phase) == 0x38);
static_assert(offsetof(TogyuEventWork, nextState) == 0x3c);
static_assert(offsetof(TogyuEventWork, motionHandles) == 0x44);
static_assert(offsetof(TogyuEventWork, savedNpcBatchCount) == 0x84);
static_assert(offsetof(TogyuEventWork, characters) == 0x88);
static_assert(offsetof(TogyuEventWork, selectedOpponent) == 0xb8);
static_assert(offsetof(TogyuEventWork, smokeId) == 0xbc);
static_assert(offsetof(TogyuEventWork, laterPhase) == 0x140);

extern const ScriptAreaDefinition tglbScriptDefinition;

} // namespace shenmue::s2
