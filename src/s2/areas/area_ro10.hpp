#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/area_loop.hpp"
#include "common/math.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2::areas {
inline constexpr shen_name RO10_RYO{SHEN_NAME(RYO_)}, RO10_HAND{SHEN_NAME(ryot)}, RO10_BASE{SHEN_NAME(ro10)}, RO10_EVENT{SHEN_NAME(EVNT)};
struct Ro10Shop {
    shen_name shop, owner;
    std::int32_t resetFlag, purchasedFlag, declinedFlag;
    shen_name prop;
    std::uint32_t help;
    float columnSpacing, rowSpacing;
    Vector3 position;
    Vector3i rotation;
    std::int32_t columns, rows;
    float playerX, playerZ;
    std::int32_t playerYaw;
    float ownerX, ownerZ;
    std::int32_t ownerYaw, ownerMotion, reservedMotion;
    shen_name objects[16];
    std::int32_t prices[16], items[16], grips[16], dialogues[16];
};
static_assert(sizeof(Ro10Shop) == 420);
static_assert(offsetof(Ro10Shop, objects) == 100);
struct Ro10Work {
    HLib::Task *parent;
    HLib::Task *self;
    char package[12];
    std::int32_t entryMode;
    char audio[16];
    shen_name shop, owner;
    std::int32_t column, row, cameraFrames;
    float cameraDistance;
    std::int32_t frames;
    std::uint32_t purchased[16];
    shen_name boards[16], ones[16], tens[16], hundreds[16];
    std::uint32_t flags;
    const Ro10Shop *definition;
    std::int32_t handFrame, grip, awards[3], reserved1ac;
};
static_assert(sizeof(Ro10Work) == 0x1b0);
static_assert(offsetof(Ro10Work, entryMode) == 28);
static_assert(offsetof(Ro10Work, boards) == 140);
static_assert(offsetof(Ro10Work, flags) == 396);
static_assert(offsetof(Ro10Work, definition) == 400);
extern const Ro10Shop ro10Shops[7];
float Ro10FlushSmall(float value);
bool GetRo10ItemTransform(const Ro10Shop &shop, std::int32_t column, std::int32_t row, Vector3 *position, Vector3i *rotation, Vector3 *scale, bool grip);
void CreateRo10PriceBoards(Ro10Work &work);
void RefreshRo10Items(Ro10Work &work);
void UpdateRo10ItemCamera(const Ro10Shop &shop, std::int32_t column, std::int32_t row, float distance, std::int32_t frames);
void QueueRo10Voice(const char *voice, std::int32_t delay, std::int32_t duration = -1, bool resume = false);
void QueueRo10Motion(shen_name character, std::uint32_t motion, float lifetime, std::uint32_t flags);
void PlayRo10Greeting(Ro10Work &work);
void PlayRo10ItemDescription(std::int32_t dialogue);
void PlayRo10Selection(shen_name shop);
void PlayRo10InsufficientCash(shen_name shop);
void PlayRo10Payment(shen_name shop, bool after);
void PrepareRo10Award(Ro10Work &work);
void UpdateRo10Hand(Ro10Work &work);
void Ro10EnterItems(void *parameter);
void Ro10Quit(void *parameter);
extern const ScriptAreaDefinition ro10ScriptDefinition;
} // namespace shenmue::s2::areas
