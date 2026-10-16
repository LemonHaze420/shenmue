#pragma once
#include "s2/areas/area_qes3.hpp"
namespace shenmue::s2 {
void Qes3SoundIntensityCallback(void *);
void QueueQes3SoundIntensity(std::int32_t initial, std::int32_t target, std::int32_t frames, std::int32_t delay);
void Qes3SparkCallback(void *);
void CreateQes3Sparks(std::int16_t index, const Vector3 &position, float floorHeight, float rate);
void UpdateQes3Sparks(std::int16_t index, const Vector3 &position);
void FreeQes3Sparks(std::int16_t index);
void FreeAllQes3Sparks();
std::int32_t Qes3PoleSuccess();
std::int32_t Qes3PoleFailure();
std::int32_t Qes3FirstChainsawSuccess();
std::int32_t Qes3FirstChainsawFailure();
std::int32_t Qes3SecondChainsawSuccess();
std::int32_t Qes3SecondChainsawFailure();
std::int32_t Qes3ThirdChainsawSuccess();
std::int32_t Qes3ThirdChainsawFailure();
void Qes3AuthorAudioCallback(void *);
void Qes3AfterFightAuthorCallback(void *);
void RestoreQes3FightCharacter(shen_name character);
} // namespace shenmue::s2
