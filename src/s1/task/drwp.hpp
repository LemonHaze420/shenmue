#pragma once

#include "s1/hlib.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct Character;

struct DRWPEntitySlot {
    float renderDistance;
    std::uint32_t reserved04;
    Character *character;
};

struct DRWP_t {
    std::byte reserved000[0x10];
    DRWPEntitySlot entitySlots[33];
    std::int32_t entityCount;
    std::int32_t reserved224;
    std::int32_t acceptedCandidateCount;
    std::int32_t frameCount;
};

static_assert(sizeof(DRWPEntitySlot) == 0x10);
static_assert(offsetof(DRWPEntitySlot, character) == 0x08);
static_assert(sizeof(DRWP_t) == 0x230);
static_assert(offsetof(DRWP_t, entitySlots) == 0x10);
static_assert(offsetof(DRWP_t, entityCount) == 0x220);
static_assert(offsetof(DRWP_t, acceptedCandidateCount) == 0x228);
static_assert(offsetof(DRWP_t, frameCount) == 0x22c);



HLib::Task *DRWP_Init();


HLib::Task *Rendering_n_Init();
void Rendering_n_Shutdown();

void CharDraw_DRWP(HLib::TaskData *taskData);


extern HLib::Task *g_DRWP_MainTask;
extern std::int32_t g_MaxEntitiesPerType;
extern std::int32_t g_bShouldDraw;
extern Character *g_CEWP_Ptr_0;
extern std::int32_t g_bDisableGlobalLightingAdjustment;
extern std::int32_t g_restoreCharacterLightMask;
extern float g_fLightingIntensity;

void UpdateGlobalLighting();
void charaEvaluateLightOverride();
void charaRestoreLightOverride();

HLib::Task *DRWP_GetTask();
DRWP_t *DRWP_GetWork();



bool DRWP_IsRetainedCharacter(const Character *character);
void DRWP_DispatchCharacter(Character *character);

} // namespace shenmue::s1
