#pragma once
#include "s1_multiplayer.hpp"
#if SHENMUE_ENHANCEMENT_STEAM_S1_MP && SHENMUE_IS_S1 && SHENMUE_IS_PC
#include "common/math.hpp"
namespace shenmue::s1 {
inline constexpr unsigned kMultiplayerEnemyLimit = 2;
inline constexpr std::uint32_t kMultiplayerGuizhangHealthFloorFlag = 0x20000;
inline constexpr std::int32_t kMultiplayerGuizhangHealthFloor = 70;
inline constexpr unsigned kMultiplayerFighterCount = 1 + kMultiplayerEnemyLimit;
inline constexpr unsigned kMultiplayerFormationCount = 2 + 2 * kMultiplayerEnemyLimit;
inline constexpr unsigned kMultiplayerEnemyRecycleFrames = 60;
inline constexpr shen_name kMultiplayerMotionToken = SHEN_NAME(MOTM);
inline constexpr Vector3 kMultiplayerRyoPosition{39.4f, 0.0f, 22.6f};
inline constexpr std::int32_t kMultiplayerRyoYaw = 0xa000;
inline constexpr shen_name kMultiplayerModelToken = make_name('M', 'D', 'L', ' ');
inline constexpr shen_name kMultiplayerStyleToken = SHEN_NAME(FIGH);
inline constexpr shen_name kMultiplayerGuizhang = SHEN_NAME(MPGZ);
struct S1MultiplayerFighterAsset {
    shen_name image;
    const char *model;
    const char *styleFile;
    std::uint8_t style;
    std::uint16_t hp;
};
inline constexpr S1MultiplayerFighterAsset kMultiplayerGuizhangAsset{
    .image = kMultiplayerGuizhang, .model = "KIS_M.MT5", .styleFile = "EN_KISY3.BIN", .style = 29, .hp = 200};
inline constexpr S1MultiplayerFighterAsset kMultiplayerEnemyAssets[] = {
    {.image = SHEN_NAME(MPA0), .model = "KMA_L.MT5", .styleFile = "EN_SATOS.BIN", .style = 17, .hp = 60},
    {.image = SHEN_NAME(MPA1), .model = "JNC_L.MT5", .styleFile = "EN_PEDRO.BIN", .style = 33, .hp = 60},
    {.image = SHEN_NAME(MPA2), .model = "BKR_L.MT5", .styleFile = "EN_ZAKO1.BIN", .style = 1, .hp = 60},
    {.image = SHEN_NAME(MPA3), .model = "PPL_L.MT5", .styleFile = "EN_ZAKO2.BIN", .style = 2, .hp = 60},
};
struct S1MultiplayerFighterSpawn {
    shen_name id;
    Vector3 position;
    std::int32_t yaw;
};
inline constexpr S1MultiplayerFighterSpawn kMultiplayerFighters[kMultiplayerFighterCount] = {
    {.id = kMultiplayerGuizhang, .position = {40.0f, 0.0f, 23.5f}, .yaw = 0xa000},
    {.id = SHEN_NAME(MPE1), .position = {43.0f, 0.0f, 21.8f}, .yaw = 0x6000},
    {.id = SHEN_NAME(MPE2), .position = {43.0f, 0.0f, 23.5f}, .yaw = 0x6000},
};
struct Character;
class S1MultiplayerFightRandomScope {
    std::uint32_t savedSeed_;
    bool active_;

  public:
    explicit S1MultiplayerFightRandomScope(bool active);
    ~S1MultiplayerFightRandomScope();
    S1MultiplayerFightRandomScope(const S1MultiplayerFightRandomScope &) = delete;
    S1MultiplayerFightRandomScope &operator=(const S1MultiplayerFightRandomScope &) = delete;
};
bool S1Multiplayer_IsFightCharacter(const Character *character);
bool S1Multiplayer_CanStartFight();
bool S1Multiplayer_CreateFight();
void S1Multiplayer_EndFight();
void S1Multiplayer_UpdateFight();
bool S1Multiplayer_OwnsFight();
std::uint32_t S1Multiplayer_FightChecksum();
S1MultiplayerCombatProfile S1Multiplayer_GetCombatProfile();
bool S1Multiplayer_ApplyCombatProfile(const S1MultiplayerCombatProfile &profile);
void S1Multiplayer_RestoreCombatProfile();
} // namespace shenmue::s1
#endif
