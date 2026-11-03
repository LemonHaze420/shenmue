#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_loop_dc.h"

#define BRIB_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define BRIB_REN SHEN_NAME4('R', 'E', 'N', '_')
#define BRIB_KUK SHEN_NAME4('K', 'U', 'K', '_')
#define BRIB_WTE SHEN_NAME4('W', 'T', 'E', '_')
#define BRIB_GREN SHEN_NAME4('G', 'R', 'E', 'N')
#define BRIB_SENSE SHEN_NAME4('S', 'E', 'N', 'S')
#define BRIB_CAMERA_TARGET SHEN_NAME4('c', 'a', 'm', 'd')
#define BRIB_GRIP_HAND_POSE 11
#define BRIB_READING_HAND_POSE 19
typedef struct BribWork {
    HLibTask *parent;
    unsigned int flags;
} BribWork;
typedef struct BribPlacement {
    Vector3 position;
    Vector3i rotation;
} BribPlacement;
extern char bribPackage[], bribConversation[];
extern BribPlacement bribKukPlacement;
extern shen_name bribRenCharacters[], bribSenseCharacters[];
void RunBribEvent(int argument);
void QueueBribRen(void);
void BribRenGetNpcs(void *data);
void BribRenGetPackage(void *data);
void BribRenStart(void *data);
int BribRenHandCallback(int phase, int value, HLibTask *parent);
int BribKukMotionCallback(int phase, int value, HLibTask *parent);
void BribRenLoop(void *data);
void QueueBribSense(void);
void BribSenseGetNpcs(void *data);
void BribSenseGetPackage(void *data);
void BribSenseStart(void *data);
void BribSenseLoop(void *data);
int BribSenseAuthorCallback(int phase, int value, HLibTask *parent);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/area_loop.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2::areas {
inline constexpr shen_name BRIB_AREA{SHEN_NAME(BRIB)}, BRIB_RYO{SHEN_NAME(RYO_)}, BRIB_REN{SHEN_NAME(REN_)}, BRIB_KUK{SHEN_NAME(KUK_)};
inline constexpr shen_name BRIB_WTE{SHEN_NAME(WTE_)};
inline constexpr shen_name BRIB_GREN{SHEN_NAME(GREN)}, BRIB_RYOS{SHEN_NAME(RYOS)}, BRIB_CAMERA{SHEN_NAME(cbmd)}, BRIB_SENSE{SHEN_NAME(SENS)};
inline constexpr shen_name BRIB_CAMERA_TARGET{SHEN_NAME(camd)};
inline constexpr shen_name BRIB_EMPTY{0};
inline constexpr shen_name BRIB_REN_CHARACTERS[]{BRIB_REN, BRIB_KUK, BRIB_EMPTY};
inline constexpr shen_name BRIB_SENSE_CHARACTERS[]{BRIB_REN, BRIB_KUK, BRIB_WTE, BRIB_EMPTY};
inline constexpr Vector3 BRIB_KUK_POSITION{301.2f, 128.8f, 72.56f}, BRIB_RYO_OFFSET{0.098f, 0, 1.083f}, BRIB_RYOS_OFFSET{0.939f, 0, -0.289f};
inline constexpr Vector3i BRIB_ROTATION{0, 49152, 0};
struct BribWork {
    HLib::Task *parent;
    std::uint32_t flags, reserved0c;
};
static_assert(sizeof(BribWork) == 0x10);
static_assert(offsetof(BribWork, flags) == 8);
void InitializeBribEvent();
void RunBribEvent(std::uintptr_t argument);
extern const ScriptAreaDefinition bribScriptDefinition;
} // namespace shenmue::s2::areas
#endif
