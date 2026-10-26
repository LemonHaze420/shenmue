#pragma once

#include "shenmue.h"

#if SHENMUE_ENHANCEMENT_STEAM_S1_MP && SHENMUE_IS_S1 && SHENMUE_IS_PC

#include "common/types.hpp"
#include "s1/input.hpp"
#include "s1/player_stats.hpp"
#include <cstdint>

namespace shenmue::s1 {
namespace HLib {
struct Task;
}
struct Character;
struct CharacterComponentNode;

enum class S1MultiplayerRole : std::uint32_t { None, Host, Guest };
enum class S1MultiplayerMessageKind : std::uint32_t { Hello = 1, Accept, Acknowledge, FightCreated, Leave, Input, Ready, Profile, ProfileAccepted, Begin };

inline constexpr std::uint32_t kS1MultiplayerMagic = SHEN_NAME(SPM1);
inline constexpr std::uint32_t kS1MultiplayerVersion = 4;
inline constexpr std::uint32_t kS1MultiplayerScene = 1;
inline constexpr std::uint32_t kS1MultiplayerEntry = 1;
inline constexpr shen_name kS1MultiplayerArea = SHEN_NAME(D000);

struct S1MultiplayerCombatProfile {
    std::array<std::int32_t, 4> skills;
    std::array<MoveSetRuntimeState, 3> moves;
    std::array<std::array<std::uint16_t, 20>, 3> usage;
    std::int32_t health;
    std::uint32_t cameraShake;
    std::uint32_t assist;
    std::uint16_t currentMove;
    std::uint8_t regeneration;
    std::uint8_t activeRegeneration;
    std::uint8_t defaultRegeneration;
};

struct S1MultiplayerMessage {
    std::uint32_t magic;
    std::uint32_t version;
    S1MultiplayerMessageKind kind;
    S1MultiplayerRole role;
    std::uint32_t scene;
    std::uint32_t area;
    std::uint32_t entry;
    std::uint32_t frame;
    std::uint32_t checksum;
    InputSlot input;
    std::uint16_t reserved;
    std::uint64_t session;
    S1MultiplayerCombatProfile profile;
};
static_assert(sizeof(S1MultiplayerMessage) <= 1200);

extern S1MultiplayerRole g_s1MultiplayerRole;

bool S1Multiplayer_Initialize(std::uint64_t invitedLobby = 0);
void S1Multiplayer_Release();
void S1Multiplayer_CloseSession();
bool S1Multiplayer_OnDial(const char *digits);
void S1Multiplayer_OnPhoneClosed();
void S1Multiplayer_Update();
void S1Multiplayer_BeginInputFrame();
void S1Multiplayer_ExecuteTask(HLib::Task *task);
void S1Multiplayer_ExecuteCharacterComponent(Character *character, CharacterComponentNode *component);
void S1Multiplayer_PumpTransport();
} // namespace shenmue::s1

#endif
