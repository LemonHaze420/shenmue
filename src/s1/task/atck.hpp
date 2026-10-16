#pragma once

#include "s1/hlib.hpp"
#include "s1/task/film.hpp"
#include "s1/task/scen.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

enum class AtckUtilityMode : std::int32_t {
    Start = 0,
    SetRaceDialSource = 1,
    Destroy = 2,
    SetUpdateFlag = 3,
    SetGaugeSource = 4,
    SetRaceDialMode = 5,
    SetLapTime = 6,
    SetBlinkEnabled = 7,
    RecordBestTime = 8,
    CompleteRaceDial = 9,
};

// Parameter 2 is a script word: some modes consume its raw integer bits while
// mode 6 consumes the same four bytes as a float.
struct AtckUtilityParameters {
    AtckUtilityMode mode;
    std::int32_t parameter1;
    std::uint32_t parameter2;
    std::int32_t parameter3;
};

// The script's 16-byte race layout (MA00 task+0x2D8): the three laps' running frame counts, then the signed lap and rank bytes UpdateRaceDial
// writes to the Spr_Lap_1/Spr_Lap_2 and Spr_Rnk_1/Spr_Rnk_2 nodes. The script seeds lap 1 of 3 and rank 5 of 5.
struct AtckRaceDialConfig {
    std::int32_t lapFrames[3];
    std::int8_t currentLap;
    std::int8_t lapCount;
    std::int8_t rank;
    std::int8_t racerCount;
};



struct AtckNativeHudState {
    std::int32_t dialPreset{};
    std::int32_t bestLapIndex{-1};
    std::int32_t lapValue0{};
    std::int32_t lapValue1{};
    std::int32_t lapValue2{};
    std::int32_t currentLap{};
    std::int32_t gameHudTopInfoParent{-1};
    std::int32_t gameHudTopLeftInfoParent{-1};
    std::int32_t gameHudRaceDialHand{};
    std::int32_t gameHudDial{};
    float previousDialValue{};
    float dialTargetValue{160.0f};
    float currentDialValue{};
    float unknownFloat{};
    void *raceSpeedLabel{};
    char unknownString[8]{'0'};
    std::int32_t field48[8]{};


    void *nodes[47]{};
    std::int32_t dialMaterialA{};
    std::int32_t field1e4[2]{};
    std::int32_t alphaNodeIndices[7]{};
    void *gameHudRace{};
};

static_assert(offsetof(AtckNativeHudState, gameHudTopInfoParent) == 0x18);
static_assert(offsetof(AtckNativeHudState, previousDialValue) == 0x28);
static_assert(offsetof(AtckNativeHudState, field48) == 0x48);
static_assert(offsetof(AtckNativeHudState, nodes) == 0x68);
static_assert(offsetof(AtckNativeHudState, dialMaterialA) == 0x1e0);
static_assert(offsetof(AtckNativeHudState, alphaNodeIndices) == 0x1ec);
static_assert(offsetof(AtckNativeHudState, gameHudRace) == 0x208);
static_assert(sizeof(AtckNativeHudState) == 0x210);


struct AtckResetRecord {
    float rect[4];
    float timer;
    std::int16_t field14;
    std::byte reserved16[2];
    std::int32_t field18;
    std::int16_t field1c;
    std::int16_t field1e;
    std::int16_t field20;
    std::uint8_t field22;
};

static_assert(offsetof(AtckResetRecord, timer) == 0x10);
static_assert(offsetof(AtckResetRecord, field18) == 0x18);
static_assert(offsetof(AtckResetRecord, field1c) == 0x1c);
static_assert(offsetof(AtckResetRecord, field22) == 0x22);

static_assert(sizeof(AtckUtilityParameters) == 0x10);
static_assert(offsetof(AtckUtilityParameters, parameter1) == 0x04);
static_assert(offsetof(AtckUtilityParameters, parameter2) == 0x08);
static_assert(offsetof(AtckUtilityParameters, parameter3) == 0x0c);
static_assert(sizeof(AtckRaceDialConfig) == 0x10);





extern AtckNativeHudState ATCK;
extern HLib::Task *g_pATCK;
extern std::uint8_t g_ATCK_flags;
extern std::uint8_t g_ATCK_enable_state;
extern std::uint8_t g_BlinkPhase;
extern std::uint8_t g_racedial_state;
extern std::uint8_t g_RaceDialMode;
extern const AtckRaceDialConfig *g_RaceDial_UI;
extern const float *g_pGaugeValueSrc;
extern std::array<std::int32_t, 3> g_RaceTimeSlots;
extern std::array<std::int32_t, 5> g_LAPTIME_sprites;
extern AtckResetRecord g_atckResetRecord;
void ATCK_AssignNewDialPreset(std::int32_t preset);
void ATCK_RaceHUDWriteNumberToStrip(std::int32_t baseSlot, std::int32_t value);

void shenStartForkliftRaceMode();
void ATCK_Callback(HLib::TaskData *taskData);
void FreeAtck();
void ATCK_UpdateRaceDial();
void ATCK_UpdateRaceLapTimeSlots();
std::int32_t ATCK_EncodeRaceDialCompositeValue(std::int32_t dialA, std::int32_t dialB, std::int32_t dialC);
void Script_atckUtils(ScenarioContext *context, void *rawParams);

inline constexpr std::array<std::int32_t, 17> kBlinkGlyphLut{
    0,
    0,
    6,
    3,
    5,
    1,
    7,
    2,
    4,
    0,
    8,
    1,
    6,
    2,
    0,
    1,
    7,
};

} // namespace shenmue::s1
