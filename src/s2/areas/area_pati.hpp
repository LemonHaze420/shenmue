#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/pati_physics.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
inline constexpr shen_name PATI_AREA{SHEN_NAME(PATI)}, PATI_PLAYER{SHEN_NAME(RYO_)}, PATI_BACKPACK{SHEN_NAME(RUCK)};
inline constexpr shen_name PATI_CAMERA{SHEN_NAME(CMDM)}, PATI_PIN{SHEN_NAME(PIN_)}, PATI_OPPONENT_HAND{SHEN_NAME(VSCH)};
inline constexpr shen_name PATI_ACCEPT{SHEN_NAME(BTBA)}, PATI_CANCEL{SHEN_NAME(BTCC)}, PATI_INSPECT{SHEN_NAME(BTIF)};
struct PatiVoiceLength {
    shen_name code;
    std::int32_t frames;
};
struct PatiBallCharacter {
    shen_name character;
    Vector3 position;
};
struct PatiGameWork {
    std::uint32_t reserved00;
    shen_name area, table, board;
    HLib::Task *parent;
    std::int32_t balls, wins, pocket, prize, result, mode;
    std::uint16_t ballLimit, winLimit, missLimit, reserved36;
    std::int32_t targetVariation, targetX;
    PatiNailData nails;
    char voicePrefix[16];
    std::int32_t activeBall;
    PatiBallCharacter ballCharacters[5];
    PatiVoiceLength *voiceLengths;
    Vector3 savedPlayerPosition;
    Vector3i savedPlayerRotation;
};
struct PatiDealerMotion {
    std::int32_t idle, speak, concern, lose, reply, finish;
};
struct PatiHeldPrize {
    std::int32_t item, count;
};
struct PatiBoardChoice {
    shen_name character;
    Vector3 position;
    Vector3i rotation;
    char size;
    std::byte reserved1d[3];
    const char *nailFile;
    std::uint8_t option, type;
    std::byte reserved2a[6];
};
#pragma pack(push, 4)
struct PatiEventWork {
    HLib::Task *parent;
    HLib::Task *task;
    char bgm[13], sbt[19];
    std::int64_t soundScene;
    std::int32_t selectedBoard, selectionFrames;
    char selectedName[8], argument[8], opponent[8], versusBoard[8], dealerGroup[8], previousBoard[8], board[8], nextBoard[8], excludedBoard[8];
    shen_name nearbyDealer, dealer;
    std::int32_t savedMotion, reserved94;
    const PatiDealerMotion *motions;
    std::int8_t state;
    std::byte reserveda1[3];
    std::int32_t delay;
    std::uint8_t entryFlag;
    std::byte reserveda9[3];
    std::int32_t played;
    std::uint8_t option, type, wagerMode, payoutMode;
    float scale;
    std::int32_t intro, totalBet, payout, bet, remainingGames, pendingMoney, round, challengeComplete, totalRounds, playerWins, dealerWins;
    std::uint32_t prizeCount;
    std::int32_t prizeIndex;
    PatiHeldPrize heldPrizes[5];
    PatiTableData tables;
    std::uint32_t reserved86ac;
    PatiGameWork game;
    PatiPlacementNode nailPlacements[512];
    PatiBoardChoice choices[20];
};
#pragma pack(pop)
static_assert(sizeof(PatiVoiceLength) == 8 && sizeof(PatiBallCharacter) == 16 && sizeof(PatiBoardChoice) == 48);
static_assert(sizeof(PatiGameWork) == 0x60f8 && offsetof(PatiGameWork, nails) == 0x40);
static_assert(offsetof(PatiGameWork, voicePrefix) == 0x6074 && offsetof(PatiGameWork, ballCharacters) == 0x6088);
static_assert(offsetof(PatiEventWork, state) == 0xa0 && offsetof(PatiEventWork, option) == 0xb0);
static_assert(offsetof(PatiEventWork, tables) == 0x114 && offsetof(PatiEventWork, game) == 0x86b0);
static_assert(offsetof(PatiEventWork, nailPlacements) == 0xe7a8 && offsetof(PatiEventWork, choices) == 0x167a8);
static_assert(sizeof(PatiEventWork) == 0x16b68);
void PATIInit();
void PATIMain(std::uintptr_t argument);
extern const ScriptAreaDefinition patiScriptDefinition;
void PatiEventCallback(void *data);
void QueuePatiBall(PatiGameWork *game);
void PatiBallCallback(void *data);
void InitializePatiBall(PatiBallWork *ball, HLib::Task *task);
void ConfigurePatiBallBoard(PatiBallWork *ball);
void InitializePatiBallNails(PatiBallWork *ball);
void ConfigurePatiPrizes(PatiBallWork *ball);
void QueuePatiVoice(PatiGameWork *game, const char *code, std::int32_t *frame);
} // namespace shenmue::s2
