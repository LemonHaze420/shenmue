#pragma once
#include "s2/event_characters.hpp"
#include "s2/hlib.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2::areas {
inline constexpr shen_name GABT_MOLE_TASK{SHEN_NAME(EVNT)}, GABT_MOLE_RYO{SHEN_NAME(RYO_)}, GABT_MOLE_GAI{SHEN_NAME(GAI2)};
inline constexpr shen_name GABT_MOLE_BASE{SHEN_NAME(BASE)}, GABT_MOLE_INVALID{0xffffffffU};
inline constexpr shen_name GABT_MOLE_SCENERY{SHEN_NAME(a558)}, GABT_MOLE_REFLECTOR{SHEN_NAME(a333)}, GABT_MOLE_BUTTON{SHEN_NAME(BTPP)};
inline constexpr shen_name GABT_MOLE_CHARACTERS[]{GABT_MOLE_RYO, GABT_MOLE_GAI, GABT_MOLE_INVALID};
inline constexpr std::uint32_t GABT_MOLE_DIRECTIONS[]{0x8000, 0x9000, 0x1000, 0x5000, 0x4000};
inline constexpr float GABT_MOLE_ANGLES[]{2, 47, 90, 133, 178};
struct GabtMoleWork {
    HLib::Task *task;
    EventReadCharactersWork *characters;
    std::int32_t state, result, ticksLeft, remainingFrames, cooldown, currentDirection, previousDirection, targetDirection;
    std::int32_t reserved30, inputState, opponentState;
    float orbitAngle, rotationOffset;
    std::int32_t orbitSign, spinSign;
    float orbitSpeed, targetSpeed, radius;
    std::int32_t sector, randomSector;
    float alpha, alphaThreshold;
    std::int32_t enemyWait;
    float waitLimit;
    std::int32_t previousOrbitSign, inputFrames, interpolationFrames;
    std::uint32_t framesElapsed;
    std::int32_t voiceIndex;
    float difficulty;
    std::int32_t entry;
    std::uint32_t reserved8c;
};
static_assert(sizeof(GabtMoleWork) == 0x90 && offsetof(GabtMoleWork, inputState) == 0x34);
static_assert(offsetof(GabtMoleWork, alpha) == 0x60 && offsetof(GabtMoleWork, entry) == 0x88);
extern std::int32_t gabtOutcome, gabtInputPenalty, gabtInputArmed, gabtLatchedButtons, gabtHalfwayAdvice;
extern float gabtPlayerEndFrame, gabtOpponentEndFrame, gabtCameraTargetHeight, gabtCameraEyeHeight;
extern Vector4 gabtSavedGlobalLighting;
void InitializeGabtMoleGlobals();
void QueueGabtMoleAttack(std::int32_t entry);
void GabtMoleCallback(void *data);
void GabtDetachCharacterBases();
void GabtUpdateMoleOrbit(GabtMoleWork *work);
std::uint32_t GabtLatchMoleInput();
std::int32_t GabtProcessMoleInput(GabtMoleWork *work);
void GabtUpdateMoleOpponent(GabtMoleWork *work);
void GabtUpdateMoleReflector(GabtMoleWork *work);
} // namespace shenmue::s2::areas
