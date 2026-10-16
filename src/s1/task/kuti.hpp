#pragma once

#include "s1/hlib.hpp"
#include "common/math.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct Character;



struct KutiScratchBank {
    std::array<void *, 3> pools{};
    std::int16_t vertexCount{};
    std::int16_t frame{};
    std::int16_t frameCount{};
    std::uint16_t allocated{};
};

static_assert(sizeof(KutiScratchBank) == 0x20);
static_assert(offsetof(KutiScratchBank, vertexCount) == 0x18);
static_assert(offsetof(KutiScratchBank, allocated) == 0x1e);




extern std::array<KutiScratchBank, 3> g_kutiScratchBanks;
extern std::array<Character *, 128> g_clipMemory;
extern void *g_pCurrentFaceActor;
extern void *g_pFACE;
extern Vector3 g_activeDialoguePosition;
extern std::uint16_t g_activeDialogueSlot;
extern std::int16_t g_clipSlotHighWaterIndex;
extern std::int16_t g_clipScanIndex;

extern HLib::Task *g_kutiTask;

void KUTI_Callback(HLib::TaskData *taskData);
Character **KUTI_Init();
void KUTI_Cleanup();
void KUTI_BeginFaceHandoff(void *faceActor, void *faceWork);
void KUTI_EndFaceHandoff();

std::int16_t KUTI_RegisterClipCharacter(Character *character);
bool KUTI_GetActiveDialogueAnchor(Character *&character, Vector3 &position);

} // namespace shenmue::s1
