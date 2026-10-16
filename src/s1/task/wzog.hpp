#pragma once

#include "s1/hlib.hpp"

#include <array>
#include <cstdint>

namespace shenmue::s1 {

struct Character;



struct MoveListRecord {
    const char *nameToken;
    std::int16_t commandIndex;
};


using MoveCommandRecord = std::array<std::uint8_t, 7>;



struct MoveAnimationRecord {
    std::uint32_t flags;
    std::uint16_t animation;
    std::uint16_t masteredAnimation;
    std::uint8_t cameraPhase;
    std::uint8_t kind;
    std::uint16_t reserved0a;
};
static_assert(sizeof(MoveAnimationRecord) == 12);



extern const std::array<MoveListRecord, 19> g_handMoveList;
extern const std::array<MoveListRecord, 18> g_legMoveList;
extern const std::array<MoveListRecord, 14> g_throwMoveList;
extern const std::array<MoveCommandRecord, 17> g_handMoveCommands;
extern const std::array<MoveCommandRecord, 16> g_legMoveCommands;
extern const std::array<MoveCommandRecord, 14> g_throwMoveCommands;
extern const std::array<const char *, 19> g_handMoveNames;
extern const std::array<const char *, 18> g_legMoveNames;
extern const std::array<const char *, 14> g_throwMoveNames;
extern const std::array<MoveAnimationRecord, 19> g_handMoveAnimations;
extern const std::array<MoveAnimationRecord, 18> g_legMoveAnimations;
extern const std::array<MoveAnimationRecord, 14> g_throwMoveAnimations;


extern Character *g_pRyoMovesScreen;
extern std::uint16_t g_movesScreenAnimationId;

std::uint16_t ResolveAnimID(std::uint8_t animSet, std::uint16_t moveId, std::uint16_t experience);
std::uint16_t ResolveAnimationById(std::uint8_t animSet, std::uint8_t moveId);
void RequestAnimation(std::uint8_t animSet, std::uint8_t moveId);
std::int32_t InitMoves(std::int16_t move, std::int16_t mode, std::int16_t animSet);


void LoadRyoForMovesScreen(std::int16_t animSet, std::int16_t move, bool revealSooner);

void DestroyForMovesScreen();


void MovesScreen_PlaceAndDrawRyo();


extern void (*g_pMovesScreenSceneBuffer)();


void MovesScreen_RenderScene();


void InitMovesScreenPipelineContext();


void RenderMovesScreenSceneToTexture();

extern HLib::Task *g_pMovesScreenTask;


extern std::uint16_t g_movesScreenCurrentAnimSet;
extern std::uint8_t g_movesScreenSyncedMoveSet;
extern std::uint8_t g_selectedMoveListIndex;
// Index 0 is the all-moves list, 1-3 the AnimSets.
extern std::array<std::uint8_t, 4> g_moveListMoveIds;
// 0x40 focused set, 0x80 specific move.
extern std::array<std::uint8_t, 4> g_moveListFocusFlags;
extern std::array<std::uint8_t, 3> g_moveListCounts;
// The listed AnimSet's progress and usage.
extern std::array<std::uint16_t, 20> g_movesScreenMoveProgress;
extern std::array<std::uint16_t, 20> g_movesScreenMoveUsage;
extern std::uint16_t g_movesScreenRefreshState;
extern std::uint16_t g_movesScreenTransitionFlags;
extern std::int32_t g_movesScreenViewActive;
extern std::uint8_t g_movesScreenTransitionGate;
extern float g_movesScreenTransitionOffset;
extern std::uint8_t g_movesScreenRuntimePrimed;
extern std::uint8_t g_movesScreenNativeMoveCount;

std::uint16_t WZOG_GetTransitionFlags();
std::uint16_t WZOG_GetRefreshState();
bool WZOG_IsViewActive();
void SyncMovesScreenRuntimeState();
std::uint32_t SetMovesScreenMoveSetAndSelection(std::int32_t moveSet, std::int32_t move);
std::uint32_t SetTrainingFocusMoveSet(std::int32_t trainingFocus, std::int32_t move);
void ResetSearchToFree();
void ExitSearchAndMoveCameraRelative(float eyeHeight, float targetDistance, std::uint16_t yaw);
std::uint8_t BeginMovesScreenViewTransition();
void EnqueueWZOG();
void WZOG_RequestClose();
void shenRefreshUI();
void WZOG_Callback(HLib::TaskData *taskData);
void WZOG_Destroy();

} // namespace shenmue::s1
