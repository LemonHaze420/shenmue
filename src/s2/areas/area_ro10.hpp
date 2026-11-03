#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_loop_dc.h"
#include "s2/event_debug_dc.h"

#define RO10_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define RO10_HAND SHEN_NAME4('r', 'y', 'o', 't')
#define RO10_BASE SHEN_NAME4('r', 'o', '1', '0')
#define RO10_FREE SHEN_NAME4('$', 'f', 'r', 'e')
#define RO10_ARDX SHEN_NAME4('A', 'R', 'D', 'X')
#define RO10_ARX2 SHEN_NAME4('A', 'R', 'X', '2')
#define RO10_ARXX SHEN_NAME4('A', 'R', 'X', 'X')
#define RO10_ARSF SHEN_NAME4('A', 'R', 'S', 'F')
#define RO10_AKX7 SHEN_NAME4('A', 'K', 'X', '7')
#define RO10_QR00 SHEN_NAME4('Q', 'R', '0', '0')
#define RO10_CANCEL SHEN_NAME4('B', 'T', 'C', 'C')
#define RO10_SELECT SHEN_NAME4('B', 'T', 'E', 'V')
#define RO10_DESCRIBE SHEN_NAME4('B', 'T', 'F', 'T')
#define RO10_ITEM(tens, ones) SHEN_NAME4('6', '.', tens, ones)
#define RO10_ITEM_OBJECTS                                                                                                                           \
    {RO10_ITEM('0', '1'), RO10_ITEM('0', '2'), RO10_ITEM('0', '3'), RO10_ITEM('0', '4'), RO10_ITEM('0', '5'), RO10_ITEM('0', '6'),             \
     RO10_ITEM('0', '7'), RO10_ITEM('0', '8'), RO10_ITEM('0', '9'), RO10_ITEM('1', '0'), RO10_ITEM('1', '1'), RO10_ITEM('1', '2'),             \
     RO10_ITEM('1', '3'), RO10_ITEM('1', '4'), RO10_ITEM('1', '5'), RO10_ITEM('1', '6')}
#define RO10_DEBUG_ID 3
#define RO10_EPSILON 0.00000011920929f
#define RO10_FLUSH(value) if (__builtin_fabs(value) < RO10_EPSILON) (value) = 0.0f
typedef struct Ro10Shop {
    shen_name shop, owner;
    int resetFlag, purchasedFlag, declinedFlag;
    shen_name prop;
    unsigned char help;
    float columnSpacing, rowSpacing;
    Vector3 position;
    Vector3i rotation;
    int columns, rows;
    float playerX, playerZ;
    int playerYaw;
    float ownerX, ownerZ;
    int ownerYaw;
    unsigned int ownerMotion, reservedMotion;
    shen_name objects[16];
    int prices[16], items[16], grips[16], dialogues[16];
} Ro10Shop;
typedef struct Ro10Work {
    HLibTask *parent;
    HLibTask *self;
    char package[16];
    char audio[16];
    shen_name shop, owner;
    int column, row, cameraFrames;
    float cameraDistance;
    int frames;
    int purchased[16];
    shen_name boards[16], ones[16], tens[16], hundreds[16];
    int flags;
    const Ro10Shop *definition;
    int handFrame, grip, awards[3];
} Ro10Work;
typedef struct Ro10ExitWork {
    HLibTask *parent;
    HLibTask *owner;
} Ro10ExitWork;
void RunRo10(const shen_name *argument);
void Ro10DebugQuit(void *data);
void Ro10Quit(void *data);
void ExitAfterFade(void *data);
void WaitShopReady(void *data);
void WaitShopPackage(void *data);
void WaitShopOwner(void *data);
void Ro10EnterItems(void *data);
void ShopMain(void *data);
void ShopSelect(void *data);
void ShopSelectionIcon(void *data);
void ShopChoice(void *data);
void ShopDeclined(void *data);
void ShopAward(void *data);
void ShopHand(void *data);
const Ro10Shop *FindRo10Shop(shen_name shop);
void RefreshRo10Items(const Ro10Shop *shop, int *purchased, shen_name *boards, shen_name *hundreds, shen_name *tens, shen_name *ones);
int GetRo10ItemTransform(const Ro10Shop *shop, int column, int row, Vector3 *position, Vector3i *rotation, Vector3 *scale, int grip);
void UpdateRo10ItemCamera(const Ro10Shop *shop, int column, int row, float distance, int frames);
void CreateRo10PriceBoards(const Ro10Shop *shop, shen_name *boards, shen_name *hundreds, shen_name *tens, shen_name *ones);
#else
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
#endif
