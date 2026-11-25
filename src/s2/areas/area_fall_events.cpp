#include "s2/areas/area_fall_events.hpp"
#include "s2/events/event_api.h"
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
extern int g_storyStep;
void *memset(void *destination, int value, unsigned int size);
enum { ryo = SHEN_NAME4('R', 'Y', 'O', '_'), shenhua = SHEN_NAME4('S', 'I', 'N', '_'), dummy = SHEN_NAME4('_', 'd', 'u', 'm') };
#else
#include "s2/area_event.hpp"
#include "s2/author.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/field_collision.hpp"
#include "s2/fight.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/help.hpp"
#include "s2/pak.hpp"
#include "s2/hlib.hpp"
#include "s2/input.hpp"
#include "s2/map_control.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/story_state.hpp"
#include "s2/ui.hpp"
#include "s2/screen_film.hpp"
#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>

namespace shenmue::s2::areas {
namespace {
constexpr shen_name ryo{SHEN_NAME(RYO_)}, shenhua{SHEN_NAME(SIN_)}, dummy{SHEN_NAME(_dum)};
constexpr shen_name cameraTarget{SHEN_NAME(_dum)}, eventToken{SHEN_NAME(EVNT)}, fallToken{SHEN_NAME(__kf)};
#endif

typedef struct FallDirection {
    unsigned int begin, recover, fail;
    int camera, failureCamera;
    unsigned int qte;
} FallDirection;
#if defined(SHENMUE_EVENT_OVERLAY)
static FallDirection rockyDirections[] = {
#else
static_assert(sizeof(FallDirection) == 24);
constexpr FallDirection rockyDirections[] = {
#endif
    {10625, 10628, 10631, 1000, 2020, 16}, {10635, 10630, 10633, 1010, 2000, 64}, {10634, 10629, 10632, 1020, 2010, 4}};
#if !defined(SHENMUE_EVENT_OVERLAY)
constexpr FallDirection roomDirections[]{{10625, 10628, 10631, 4000, 3020, 16}, {10635, 10630, 10631, 4010, 3020, 64}, {10634, 10629, 10631, 4020, 3020, 4}};
#endif
typedef struct RockyPlacement {
    Vector3 position;
    Vector3i rotation;
    unsigned int story;
    shen_name field;
} RockyPlacement;
#if defined(SHENMUE_EVENT_OVERLAY)
static RockyPlacement rockyPlacements[] = {
#else
static_assert(sizeof(RockyPlacement) == 32);
constexpr RockyPlacement rockyPlacements[] = {
#endif

    {{-21.6200008392334f, -1.0f, -33.599998474121094f}, {0, 54613, 0}, 851, SHEN_NAME4('k', 'w', '1', 's')},
    {{-8.260000228881836f, -0.9900000095367432f, -38.41999816894531f}, {0, 52792, 0}, 852, SHEN_NAME4('k', 'w', '1', 'e')},
    {{-6.920000076293945f, -1.0f, -38.66999816894531f}, {0, 49152, 0}, 853, SHEN_NAME4('k', 'w', '2', 's')},
    {{6.099999904632568f, -0.550000011920929f, -36.310001373291016f}, {0, 23665, 0}, 854, SHEN_NAME4('k', 'w', '2', 'e')},
    {{2.4600000381469727f, 0.10999999940395355f, -33.959999084472656f}, {0, 20935, 0}, 855, SHEN_NAME4('k', 'w', '3', 's')},
    {{-9.5600004196167f, 1.440000057220459f, -21.43000030517578f}, {0, 20935, 0}, 856, SHEN_NAME4('k', 'w', '3', 'e')},
    {{-8.100000381469727f, 1.3899999856948853f, -15.369999885559082f}, {0, 40049, 0}, 1001, SHEN_NAME4('k', 'w', '4', 's')},
    {{-2.059999942779541f, 1.2899999618530273f, -9.25f}, {0, 40049, 0}, 1002, SHEN_NAME4('k', 'w', '4', 'e')},
    {{3.75f, 0.8299999833106995f, -11.0f}, {0, 54613, 0}, 1003, SHEN_NAME4('k', 'w', '5', 's')},
    {{18.459999084472656f, 1.3300000429153442f, -11.899999618530273f}, {0, 32768, 0}, 1004, SHEN_NAME4('k', 'w', '5', 'e')},
    {{15.899999618530273f, 3.5f, -1.350000023841858f}, {0, 27306, 0}, 1005, SHEN_NAME4('k', 'w', '6', 's')},
    {{9.220000267028809f, 3.5f, 8.619999885559082f}, {0, 27306, 0}, 0, SHEN_NAME4('k', 'w', '6', 'e')},
};
#if !defined(SHENMUE_EVENT_OVERLAY)
struct RoomExit {
    Vector3 position;
    shen_name scene;
    std::int32_t entry;
};
constexpr RoomExit roomExits[]{
    {{168.3000030517578f, 100.0f, 102.5999984741211f}, SHEN_NAME(QD01), 4},
    {{175.5f, 100.0f, 102.5999984741211f}, SHEN_NAME(QD01), 4},
    {{168.3000030517578f, 100.0f, 91.80000305175781f}, SHEN_NAME(QD01), 4},
    {{175.5f, 0.0f, 91.80000305175781f}, SHEN_NAME(QD01), 4},
    {{206.10000610351562f, 0.0f, 76.5f}, SHEN_NAME(QD01), 5},
    {{206.10000610351562f, 0.0f, 84.5999984741211f}, SHEN_NAME(QD01), 5},
};
constexpr Vector3 roomLookPositions[]{
    {std::bit_cast<float>(0x432be666U), std::bit_cast<float>(0x42c80000U), std::bit_cast<float>(0x42b7999aU)},
    {std::bit_cast<float>(0x432be666U), std::bit_cast<float>(0x42c80000U), std::bit_cast<float>(0x42cd3333U)},
    {std::bit_cast<float>(0x434e199aU), std::bit_cast<float>(0x42c80000U), std::bit_cast<float>(0x42a20000U)},
    {std::bit_cast<float>(0x00002981U), std::bit_cast<float>(0x00002984U), std::bit_cast<float>(0x00002987U)},
    {std::bit_cast<float>(0x000003e8U), std::bit_cast<float>(0x000007e4U), std::bit_cast<float>(0x00000010U)},
    {std::bit_cast<float>(0x0000298bU), std::bit_cast<float>(0x00002986U), std::bit_cast<float>(0x00002989U)},
};
#pragma pack(push, 4)
#endif
typedef struct RockyFallState {
    HLibTask *task;
    int state;
    Vector3 startPosition;
    Vector3i targetRotation;
    Vector3 intersection;
    short intersectionPitch, intersectionYaw;
    Vector3 savedPosition;
    Vector3i savedRotation, motionRotation;
    int direction, qteFrames, releaseFrames, cooldown;
    const FallDirection *configuration;
    int placementIndex;
#if !defined(SHENMUE_EVENT_OVERLAY)
    int reserved74;
#endif
    const RockyPlacement *placements;
    int companionPhase;
#if !defined(SHENMUE_EVENT_OVERLAY)
    int reserved84;
#endif
} RockyFallState;
#if !defined(SHENMUE_EVENT_OVERLAY)
struct RoomFallState {
    HLib::Task *task;
    std::int32_t state;
    Vector3 intersection;
    std::int16_t intersectionPitch, intersectionYaw;
    Vector3 savedPosition;
    Vector3i savedRotation, motionRotation;
    std::int32_t direction, qteFrames, releaseFrames, cooldown;
    const FallDirection *configuration;
    std::uint64_t reserved58;
    Vector3 exitPosition;
    Vector3i exitRotation;
    Vector3 lookPosition;
    Vector3i lookRotation;
    EulerRotation16 cameraRotation;
    std::int16_t reserved96;
    Vector3 cameraEye, cameraTarget, filteredTarget;
    Vector3i targetRotation;
    const RoomExit *exits;
};
#pragma pack(pop)
static_assert(sizeof(RockyFallState) == 0x88 && offsetof(RockyFallState, savedPosition) == 0x34);
static_assert(sizeof(RoomFallState) == 0xd0 && offsetof(RoomFallState, configuration) == 0x50);
RockyFallState rockyFall{};
RoomFallState roomFall{};
shen_name roomExitScene{};
std::int32_t roomExitEntry{};
struct FixLeafWork {
    shen_name character;
    std::int32_t bone, frames;
};
static_assert(sizeof(FixLeafWork) == 12);
struct FallCameraWork {
    Vector3 startEye, eye, target, startTarget;
    std::int32_t frames, totalFrames;
};
static_assert(sizeof(FallCameraWork) == 56);
#else
static RockyFallState rockyFall;
typedef struct FixLeafWork {
    shen_name character;
    int bone, frames;
} FixLeafWork;
void RockyFallCallback(void *);
void UpdateRockyCompanion(RockyFallState *);
void FixLeafCallback(void *);
void QueueFixLeaf(shen_name, int, int, int);
void SetBagCallback(void *);
void QueueSetBag(int, int);
void FallVoiceCallback(void *);
void QueueFallVoice(const char *, int);
int GetRockyFallDifficulty(void);
void AdvanceRockyFallDifficulty(int);
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
void QueueRockyAreaFallEvent(void) {
    RockyFallState *state = &rockyFall;
    HLibTask *task;
    FTBL.EV_FreeAuthorPak(1, "0540");
    FTBL.EV_FreeAuthorPak(1, "0100");
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(7), (int)"FALL", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, "FALL", 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "FALL", 0, 0);
    FTBL.LoadBlurModelForCharacter_0(ryo, "RYO_B.MT7", 1.0f);
    task = FTBL.HLib_EnqueueTaskWithoutParameter(RockyFallCallback, 4, 11, SHEN_NAME4('_', '_', 'k', 'f'));
    memset(state, 0, sizeof(*state));
    state->task = task;
    state->placements = rockyPlacements;
    state->placementIndex = 12;
    state->qteFrames = 6 * GetRockyFallDifficulty() + 18;
    FTBL.AT_SetLocalBaseChara(dummy, ryo);
    EVT_AT_SetPos(dummy, ryo, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(dummy, ryo, 0.0f, 0);
}
void RockyFallCallback(void *data) {
    RockyFallState *state = &rockyFall;
    int delay = 0;
    UpdateRockyCompanion(&rockyFall);
    if (FTBL.ryoNotBusy())
        FTBL.CheckHelpIcon(1, 0, 3);
    switch (state->state) {
    case 0: {
        Vector3 probe;
        int contact = 0;
        FTBL.AT_SetLocalBaseChara(dummy, ryo);
        EVT_AT_SetPos(dummy, ryo, 0.0f, 0.0f, 0.0f, 0);
        EVT_AT_SetAngY(dummy, ryo, 0.0f, 0);
        FTBL.GetCharaPosition(ryo, 0xffffffff, &state->savedPosition, 0x40000000);
        FTBL.GetScenarioCharacterEulerAngles(ryo, &state->savedRotation, 0x40000000);
        FTBL.GetScenarioCharacterEulerAngles(ryo, &state->motionRotation, 0x42000000);
        probe = state->savedPosition;
        probe.y += 0.5f;
        FTBL.flddPushIntersectionFlag(10);
        if (FTBL.shenFindIntersectionWithScene(&probe) != -3.4028234663852886e38f) {
            if (state->cooldown <= 0)
                FTBL.SetPlayerGameFlags(7, 1);
            FTBL.flddPushIntersectionFlag(3);
            if (FTBL.IsWithinRadiusFromVector3PolyIntersectTest(&probe, 0.5f))
                contact = 1;
        }
        if (contact) {
            short delta;
            FTBL.TransformAgainstIntersectionResult(&state->intersection);
            FTBL.transformIntersectionResult(&state->intersectionPitch);
            delta = (short)(state->motionRotation.y - state->intersectionYaw);
            state->startPosition = state->savedPosition;
            if (delta > 8192) {
                state->direction = 1;
                state->targetRotation.y = state->intersectionYaw + 0x4000;
            } else if (delta > -8192) {
                state->direction = 0;
                state->targetRotation.y = state->intersectionYaw;
            } else {
                state->direction = 2;
                state->targetRotation.y = state->intersectionYaw - 0x4000;
            }
            state->configuration = &rockyDirections[state->direction];
            if (FTBL.ryoNotBusy() && state->cooldown <= 0) {
                FTBL.SetPlayerControlEnabled(0);
                SHENMUE_EVENT_CLOBBER("r8");
                FTBL.SetLoadingBusy();
                FTBL.EV_SetCharaControllerFlags(ryo, 2, 0);
                FTBL.EV_SetCharaControllerFlags(ryo, 3, 0);
                state->state = 1;
            }
        }
        --state->cooldown;
        break;
    }
    case 1:
        FTBL.SetShowQteFlagForButtonType(state->configuration->qte, (short)state->qteFrames, 2);
        switch (state->direction) {
        case 1:
            FTBL.SetQteButtonState(0x4000);
            break;
        case 0:
            FTBL.SetQteButtonState(0x2000);
            break;
        case 2:
            FTBL.SetQteButtonState(0x8000);
            break;
        }
        state->releaseFrames = 10;
        FTBL.DebugLogToFileRemoved("---- QTE frame [%d] ----\n", state->qteFrames);
        FTBL.EV_AT_SetAngY_0(ryo, 0x78000000, &state->targetRotation, 0);
        FTBL.AT_SetLocalBaseChara(dummy, 0xffffffff);
        EVT_AT_SetMot(ryo, state->configuration->begin, 0.0f, 1.0f, 0x40000000, 0);
        EVT_AT_SetCam(state->configuration->camera + FTBL.shenGetRandomInt32(2), dummy, dummy, 0, 0, 1);
        FTBL.figpSetFlagsBlendForBlur(ryo, 5, 1);
        FTBL.figpAssignModeFor(ryo, 1);
        switch (FTBL.shenGetRandomInt32(4)) {
        case 0:
            EVT_AT_SetVoice("0540A018", 0);
            break;
        case 1:
            EVT_AT_SetVoice("0540A019", 0);
            break;
        case 2:
            EVT_AT_SetVoice("0540A020", 0);
            break;
        case 3:
            EVT_AT_SetVoice("0540A021", 0);
            break;
        }
        state->state = 2;
        break;
    case 2:
        if (state->releaseFrames <= 0)
            FTBL.SetQteButtonState(0xffff);
        --state->releaseFrames;
        if (!FTBL.GetShouldRenderIcons(3)) {
            if ((FTBL.GetCurrentQteStatus() & 0xfff) == state->configuration->qte)
                state->state = 3;
            else
                state->state = 4;
        }
        break;
    case 3: {
        Vector3 origin, position;
        Vector3i rotation;
        FTBL.figpAssignModeFor(ryo, 0);
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        if (state->direction == 0) {
            origin = state->intersection;
            origin.y = state->savedPosition.y;
            position.x = 0;
            position.y = 0;
            position.z = 0.8f;
            rotation.x = 0;
            rotation.y = state->intersectionYaw;
            rotation.z = 0;
            FTBL.ApplyScenarioTranslationAndRotation(&origin, &rotation, &position);
        } else {
            origin = state->savedPosition;
            origin.y += 0.5f;
            FTBL.RaycastInWorldGrid(&position, &origin, 0.55f);
            position.y -= 0.5f;
        }
        FTBL.EV_AT_SetPos(ryo, 0x78000000, &position);
        state->cooldown = 60;
        switch (state->direction) {
        case 1:
            QueueFixLeaf(ryo, 30, 60, 1);
            QueueFixLeaf(ryo, 25, 60, 1);
            FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(ryo), 0);
            break;
        case 2:
            QueueFixLeaf(ryo, 30, 60, 1);
            QueueFixLeaf(ryo, 25, 60, 1);
            FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(ryo), 0);
            break;
        case 0:
            FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(ryo), 0);
            break;
        }
        EVT_AT_SetMot(ryo, state->configuration->recover, 0.0f, 1.0f, 0x40000000, 0);
        delay = 60;
        state->state = 5;
        break;
    }
    case 5: {
        register shen_name player __asm__("r9");
        FTBL.EV_mtwkSetEnabled(FTBL.scenFindMotionwork(player = ryo), 1);
        FTBL.SetPlayerControlEnabled(1);
        FTBL.FinishLoading();
        FTBL.EV_SetCharaControllerFlags(player, 2, 1);
        FTBL.EV_SetCharaControllerFlags(player, 3, 1);
        SHENMUE_EVENT_USE(&FTBL.EV_EventSetUpHelpIcon_0);
        FTBL.EV_EventSetUpHelpIcon_0(0x3f, 0);
        state->state = 0;
        break;
    }
    case 4: {
        if (FTBL.DoesScenarioCharacterExist(shenhua)) {
            Vector3 position;
            FTBL.PTYCUtils(shenhua, 3, 1);
            FTBL.GetCharaPosition(shenhua, 0xffffffff, &position, 0x40000000);
            position.x = state->startPosition.x - position.x;
            position.z = state->startPosition.z - position.z;
            EVT_AT_SetAngY(shenhua, 0xffffffff, FTBL.InverseFixedAtan2(position.x, position.z) / 182.04445f, 0);
            EVT_AT_SetMot(shenhua, 0xf0a5, 0.0f, 1.0f, 0x40000000, 0);
            EVT_AT_SetMot(shenhua, 0x298c, 0.0f, 1.0f, 0, 15);
        }
        FTBL.SetHighModelAndFaceFlagsForChara(FTBL.ctrlGetCharaByName(ryo), 4, 10, 1.0f);
        FTBL.SetFacialMouthExpression(FTBL.ctrlGetCharaByName(ryo), 1, 10, 1.0f);
        FTBL.figpSetFlagsBlendForBlur(ryo, 5, 1);
        FTBL.figpAssignModeFor(ryo, 1);
        FTBL.EV_AT_SetPos(ryo, 0x78000000, &state->startPosition);
        FTBL.EV_AT_SetAngY_0(ryo, 0x78000000, &state->targetRotation, 0);
        FTBL.AT_SetLocalBaseChara(dummy, 0xffffffff);
        EVT_AT_SetMot(ryo, state->configuration->fail, 0.0f, 0.6f, 0x40000000, 0);
        FTBL.EV_ControlPackageResources(2, 4, "FALL", 0, 0);
        EVT_AT_SetCam(state->configuration->failureCamera, dummy, dummy, 0, 0, 1);
        EVT_AT_SetVoice("0100A080", delay + 20);
        QueueFallVoice("0540B018", delay + 70);
        delay = 100;
        EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, delay);
        EVT_AT_SetSample(0, 1, delay + 30);
        if (state->direction == 1) {
            QueueSetBag(0, delay);
            EVT_AT_CharaDisp(ryo, delay, 1);
        }
        delay += 60;
        QueueSetBag(1, delay);
        state->state = 6;
        break;
    }
    case 6:
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, 0);
        AdvanceRockyFallDifficulty(GetRockyFallDifficulty());
        g_storyStep = 750000;
        break;
    }
    if (delay)
        FTBL.HLib_SetupTaskDelay(delay);
}
void UpdateRockyCompanion(RockyFallState *state) {
    int changed = 0, index = 11;
    int i = 0;
    const RockyPlacement *placement = state->placements;
    for (; i < 11; ++i) {
        if (FTBL.EV_GetShenmueFlag(placement->story)) {
            index = i;
            break;
        }
        ++placement;
    }
    if (state->placementIndex != index) {
        i = 0;
        placement = state->placements;
        for (; i < 12;) {
            if (index == i)
                FTBL.ToggleFlagOnFieldEntity(placement->field, 1);
            else
                FTBL.ToggleFlagOnFieldEntity(placement->field, 0);
            ++i;
            ++placement;
        }
        changed = 1;
        FTBL.DebugLogToFileRemoved("----- change bridge %d (et_keirinfall.c) -----\n", index);
    }
    state->placementIndex = index;
    if (FTBL.ryoNotBusy() && FTBL.DoesScenarioCharacterExist(shenhua)) {
        placement = &state->placements[state->placementIndex];
        switch (state->companionPhase) {
        case 0: {
            Vector3 position;
            float dx, dz;
            FTBL.GetCharaPosition(shenhua, 0xffffffff, &position, 0x40000000);
            dx = placement->position.x - position.x;
            dz = placement->position.z - position.z;
            if (dx * dx + dz * dz < 9.0f) {
                FTBL.PTYCUtils(shenhua, 3, 1);
                FTBL.CopyMotionTransition(FTBL.scenFindMotionwork(shenhua), &placement->position, placement->rotation.y, 0, 0);
                state->companionPhase = 1;
            }
            break;
        }
        case 1:
            if (FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(shenhua), 58, 1))
                state->companionPhase = 2;
            break;
        case 2:
            if (changed) {
                FTBL.PTYCUtils(shenhua, 3, 0);
                state->companionPhase = 0;
            }
            break;
        }
    }
}
void FixLeafCallback(void *parameter) {
    FixLeafWork *work = parameter;
    if (work->frames <= 0)
        FTBL.HLib_DestroyCurrentTask();
    else {
        FTBL.GetParentBoneWrapper(FTBL.scenFindMotionwork(work->character), work->bone | 0x40000000);
        --work->frames;
    }
}
void QueueFixLeaf(shen_name character, int bone, int frames, int delay) {
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(FixLeafCallback, 4, 11, 12, TASK_EVNT);
    FixLeafWork *work = FTBL.HLib_GetTaskParameter(task);
    work->character = character;
    work->bone = bone;
    work->frames = frames;
    if (delay)
        FTBL.HLib_SetupTimedTask(task, delay);
    else
        FTBL.GetParentBoneWrapper(FTBL.scenFindMotionwork(work->character), work->bone | 0x40000000);
}
void SetBagCallback(void *parameter) {
    if (*(int *)parameter)
        FTBL.SetPlayerGameFlags(4, 1);
    else
        FTBL.SetPlayerGameFlags(4, 0);
    FTBL.HLib_DestroyCurrentTask();
}
void QueueSetBag(int enabled, int delay) {
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(SetBagCallback, 4, 11, 4, TASK_EVNT);
    *(int *)FTBL.HLib_GetTaskParameter(task) = enabled;
    if (delay)
        FTBL.HLib_SetupTimedTask(task, delay);
    else {
        if (enabled)
            FTBL.SetPlayerGameFlags(4, 1);
        else
            FTBL.SetPlayerGameFlags(4, 0);
    }
}
void FallVoiceCallback(void *parameter) {
    FTBL.EV_PlayDialogue(parameter);
    FTBL.HLib_DestroyCurrentTask();
}
void QueueFallVoice(const char *voice, int delay) {
    if (delay) {
        HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(FallVoiceCallback, 4, 11, 32, TASK_EVNT);
        strcpy(FTBL.HLib_GetTaskParameter(task), voice);
        FTBL.HLib_SetupTimedTask(task, delay);
    } else
        FTBL.EV_PlayDialogue(voice);
}
int GetRockyFallDifficulty(void) {
    int level = FTBL.EV_GetPlayerFlag(11, 1662) != 0;
    if (FTBL.EV_GetPlayerFlag(11, 1663))
        level = 2;
    return level;
}
void AdvanceRockyFallDifficulty(int level) {
    switch (level) {
    case 0:
        FTBL.EV_SetPlayerFlag(12, 1662, 1);
        break;
    case 1:
        FTBL.EV_SetPlayerFlag(12, 1663, 1);
        break;
    }
}
#else
void RockyFallCallback(void *);
void RoomFallCallback(void *);
void FallControlNullsub() {
}
void AnchorDummyToPlayer() {
    AT_SetLocalBaseChara(dummy, ryo);
    EventSetPosition position{0, dummy, ryo, {}, 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{0, dummy, ryo, 0, 0};
    EVT_AT_SetAngY_Callback(&angle);
}
void QueueFallCamera(std::int32_t camera) {
    EVT_AT_SetCam(camera, dummy, cameraTarget, 0, 0, 1);
}
void FixLeafCallback(void *parameter) {
    auto &work = *static_cast<FixLeafWork *>(parameter);
    if (work.frames < 1) {
        HLib::DestroyCurrentTask();
        return;
    }
    mtwkGetParentBoneWrapper(scenFindMotionwork(work.character), work.bone | 0x40000000);
    --work.frames;
}
void QueueFixLeaf(std::int32_t bone) {
    auto *task = HLib::EnqueueTaskWithParameter(FixLeafCallback, 4, 0xb, sizeof(FixLeafWork), eventToken);
    *static_cast<FixLeafWork *>(HLib::GetTaskParameter(task)) = {ryo, bone, 60};
    HLib::SetupTimedTask(task, 1);
}
void SetBagCallback(void *parameter) {
    SetPlayerGameFlags(PlayerGameFlagMode::Bit5, *static_cast<std::int32_t *>(parameter) ? reinterpret_cast<void *>(1) : nullptr);
    HLib::DestroyCurrentTask();
}
void QueueSetBag(std::int32_t enabled, std::int32_t delay) {
    auto *task = HLib::EnqueueTaskWithParameter(SetBagCallback, 4, 0xb, 4, eventToken);
    *static_cast<std::int32_t *>(HLib::GetTaskParameter(task)) = enabled;
    HLib::SetupTimedTask(task, delay);
}
void FallVoiceCallback(void *parameter) {
    EV_PlayDialogue(static_cast<const char *>(parameter));
    HLib::DestroyCurrentTask();
}
void QueueFallVoice() {
    auto *task = HLib::EnqueueTaskWithParameter(FallVoiceCallback, 4, 0xb, 32, eventToken);
    std::strcpy(static_cast<char *>(HLib::GetTaskParameter(task)), "0540B018");
    HLib::SetupTimedTask(task, 70);
}
float InterpolateFallCamera(float first, float last, float fraction) {
    fraction = std::min(1.0f, std::max(0.0f, fraction));
    if (std::fabs(last - first) < 0.00000011920929f)
        return first;
    return ((fraction * 3.0f) * fraction - ((fraction + fraction) * fraction) * fraction) * (last - first) + first;
}
void FallCameraCallback(void *parameter) {
    auto &work = *static_cast<FallCameraWork *>(parameter);
    if (work.frames <= 0) {
        camCtrlSetNextPosition(work.eye);
        camCtrlSetTargetPosition(work.target);
        HLib::DestroyCurrentTask();
        return;
    }
    const float fraction = static_cast<float>(work.totalFrames - work.frames) / work.totalFrames;
    Vector3 eye{InterpolateFallCamera(work.startEye.x, work.eye.x, fraction), InterpolateFallCamera(work.startEye.y, work.eye.y, fraction),
        InterpolateFallCamera(work.startEye.z, work.eye.z, fraction)};
    Vector3 target{InterpolateFallCamera(work.startTarget.x, work.target.x, fraction), InterpolateFallCamera(work.startTarget.y, work.target.y, fraction),
        InterpolateFallCamera(work.startTarget.z, work.target.z, fraction)};
    camCtrlSetNextPosition(eye);
    camCtrlSetTargetPosition(target);
    --work.frames;
}
void QueueFallLookCamera() {
    using enum CameraState;
    auto *task = HLib::EnqueueTaskWithParameter(FallCameraCallback, 4, 0xb, sizeof(FallCameraWork), eventToken);
    auto &work = *static_cast<FallCameraWork *>(HLib::GetTaskParameter(task));
    work.frames = work.totalFrames = 60;
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(ryo, 5, &position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(ryo, &rotation, EventTransformFlags::World | EventTransformFlags::TurnMotion);
    work.eye = {0, -.30000001192092896f, -2};
    scenApplyTransRot(&position, &rotation, &work.eye);
    work.target = {0, -30, -15};
    scenApplyTransRot(&position, &rotation, &work.target);
    camCtrlGetNextCamPos(&work.startEye);
    camCtrlGetTargetPosition(&work.startTarget);
    camCtrlSetState(Event);
}
void MoveFallLookTarget() {
    float perspective = static_cast<std::int16_t>(camCtrlGetPerspective()) / 182.04445f;
    if ((GetCurrentControlInput(INPUT_LEFT_TRIGGER) & 0x40U) && perspective < 90)
        perspective += 2;
    else if ((GetCurrentControlInput(INPUT_RIGHT_TRIGGER) & 0x80U) && perspective > 20)
        perspective -= 2;
    camCtrlSetPerspective(static_cast<std::uint16_t>((perspective * 8192) / 45));
    std::int16_t x = static_cast<std::int16_t>(GetCurrentControlInput(INPUT_RIGHT_X)) - 128;
    std::int16_t y = 128 - static_cast<std::int16_t>(GetCurrentControlInput(INPUT_RIGHT_Y));
    x = std::clamp<std::int16_t>(x, -127, 127);
    y = std::clamp<std::int16_t>(y, -127, 127);
    const float dx = std::min(15.0f, std::max(-10.0f, static_cast<float>(x * (y + 127) / 127) * .1f));
    const float dy = std::min(15.0f, std::max(-10.0f, y * .1f));
    Matrix4x4_PushActiveTransform();
    Matrix4x4::preTranslate(roomFall.cameraTarget.x, roomFall.cameraTarget.y, roomFall.cameraTarget.z);
    ApplyShearToPrePassMatrix(sin_fixed16(roomFall.cameraRotation.yaw), cos_fixed16(roomFall.cameraRotation.yaw));
    Matrix4x4::RotateAroundX(sin_fixed16(roomFall.cameraRotation.pitch), cos_fixed16(roomFall.cameraRotation.pitch));
    Matrix4x4::preTranslate(dx, dy, 0);
    Vector3 target{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop(1);
    roomFall.filteredTarget.x += (target.x - roomFall.filteredTarget.x) * .1f;
    roomFall.filteredTarget.y += (target.y - roomFall.filteredTarget.y) * .1f;
    roomFall.filteredTarget.z += (target.z - roomFall.filteredTarget.z) * .1f;
    camCtrlSetTargetPosition(roomFall.filteredTarget);
}
void FindRoomFallTargets() {
    std::int32_t closest = 6;
    float distance = 3.4028234663852886e38f;
    for (std::int32_t i = 0; i < 6; ++i) {
        const float dx = roomFall.exits[i].position.x - roomFall.intersection.x, dz = roomFall.exits[i].position.z - roomFall.intersection.z;
        if (dx * dx + dz * dz < distance) {
            distance = dx * dx + dz * dz;
            closest = i;
        }
    }
    roomFall.exitPosition = roomFall.exits[closest].position;
    roomFall.exitRotation = {0,
        static_cast<std::int32_t>(
            static_cast<std::int16_t>(atan2_fixed16(roomFall.exitPosition.x - roomFall.savedPosition.x, roomFall.exitPosition.z - roomFall.savedPosition.z))),
        0};
    roomExitScene = roomFall.exits[closest].scene;
    roomExitEntry = roomFall.exits[closest].entry;
    closest = 3;
    distance = 3.4028234663852886e38f;
    for (std::int32_t i = 0; i < 6; ++i) {
        const float dx = roomLookPositions[i].x - roomFall.intersection.x, dz = roomLookPositions[i].z - roomFall.intersection.z;
        if (dx * dx + dz * dz < distance) {
            distance = dx * dx + dz * dz;
            closest = i;
        }
    }
    roomFall.lookPosition = roomLookPositions[closest];
    roomFall.lookRotation = {0,
        static_cast<std::int32_t>(
            static_cast<std::int16_t>(atan2_fixed16(roomFall.lookPosition.x - roomFall.savedPosition.x, roomFall.lookPosition.z - roomFall.savedPosition.z))),
        0};
}
void UpdateRockyCompanion() {
    std::int32_t index = 11;
    for (std::int32_t i = 0; i < 11; ++i)
        if (EV_GetShenmueFlag(rockyFall.placements[i].story)) {
            index = i;
            break;
        }
    const bool changed = rockyFall.placementIndex != index;
    if (changed)
        for (std::int32_t i = 0; i < 12; ++i)
            ToggleFlagOnFieldEntity(rockyFall.placements[i].field, i == index);
    rockyFall.placementIndex = index;
    if (!ryoNotBusy() || !DoesScenarioCharacterExist(shenhua))
        return;
    const auto &placement = rockyFall.placements[index];
    if (rockyFall.companionPhase == 1) {
        if (mtwkSetExtraFlag(scenFindMotionwork(shenhua), 58, 1))
            rockyFall.companionPhase = 2;
    } else if (rockyFall.companionPhase == 2) {
        if (changed) {
            EV_PTYCUtils(shenhua, 3, 0);
            rockyFall.companionPhase = 0;
        }
    } else if (!rockyFall.companionPhase) {
        Vector3 position;
        EV_GetCharaPosition(shenhua, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        const float dx = placement.position.x - position.x, dz = placement.position.z - position.z;
        if (dx * dx + dz * dz < 9) {
            EV_PTYCUtils(shenhua, 3, 1);
            mtwkCopyTransition(scenFindMotionwork(shenhua), &placement.position, static_cast<std::uint16_t>(placement.rotation.y), 0, 0);
            rockyFall.companionPhase = 1;
        }
    }
}
void RockyFallCallback(void *) {
    using enum CameraState;
    UpdateRockyCompanion();
    if (ryoNotBusy())
        CheckHelpIcon(1, 0, CheckHelpIconOp::FrameOverlay);
    const FallDirection *config = rockyFall.configuration;
    switch (rockyFall.state) {
    case 0: {
        AnchorDummyToPlayer();
        EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &rockyFall.savedPosition, EventTransformFlags::World);
        GetScenarioCharacterEulerAngles(ryo, &rockyFall.savedRotation, EventTransformFlags::World);
        GetScenarioCharacterEulerAngles(ryo, &rockyFall.motionRotation, EventTransformFlags::World | EventTransformFlags::TurnMotion);
        Vector3 probe = rockyFall.savedPosition;
        probe.y += .5f;
        flddPushIntersectionFlag(10);
        if (shenFindIntersectionWithScene(probe, probe.y) != -3.4028234663852886e38f) {
            if (rockyFall.cooldown < 1)
                SetPlayerGameFlags(static_cast<PlayerGameFlagMode>(7), reinterpret_cast<void *>(1));
            flddPushIntersectionFlag(3);
            if (IsWithinRadiusFromVector3PolyIntersectTest(probe, .5f)) {
                TransformAgainstIntersectionResult(rockyFall.intersection);
                Vector3s angles;
                transformIntersectionResult(angles);
                rockyFall.intersectionPitch = angles.x;
                rockyFall.intersectionYaw = angles.y;
                rockyFall.startPosition = rockyFall.savedPosition;
                std::int32_t direction = 0, yaw = rockyFall.intersectionYaw;
                const std::int32_t delta =
                    static_cast<std::int16_t>(static_cast<std::uint16_t>(rockyFall.motionRotation.y) - static_cast<std::uint16_t>(rockyFall.intersectionYaw));
                if (delta > 8192) {
                    direction = 1;
                    yaw += 0x4000;
                } else if (delta <= -8192) {
                    direction = 2;
                    yaw -= 0x4000;
                }
                rockyFall.targetRotation.y = yaw;
                rockyFall.direction = direction;
                rockyFall.configuration = &rockyDirections[direction];
                if (ryoNotBusy() && rockyFall.cooldown < 1) {
                    SetPlayerControlEnabled(0);
                    FallControlNullsub();
                    EV_SetCharaControllerFlags(ryo, 2, NpcCharacterControllerState::Disabled);
                    EV_SetCharaControllerFlags(ryo, 3, NpcCharacterControllerState::Disabled);
                    rockyFall.state = 1;
                }
            }
        }
        --rockyFall.cooldown;
        return;
    }
    case 1: {
        SetShowQteFlag(config->qte, static_cast<std::int16_t>(rockyFall.qteFrames), 2);
        SetQteButtonState(static_cast<std::uint16_t>(0x2000U << rockyFall.direction));
        rockyFall.releaseFrames = 10;
        EV_AT_SetAngY(ryo, EventTransformFlags::World | EventTransformFlags::AllAxes, rockyFall.targetRotation);
        AT_SetLocalBaseChara(dummy, shen_name{});
        EVT_AT_SetMot(ryo, config->begin, 0, 1, 0x40000000U, 0);
        QueueFallCamera(config->camera + shenGetRandomInt32(2));
        figpSetFlagsBlendForBlur(ryo, 5, 1);
        figpAssignModeFor(ryo, 1);
        const char *voices[]{"0540A018", "0540A019", "0540A020", "0540A021"}; // @text 0540A018 EN: " " | JA: "…おっと！" ; 0540A019 EN: " " | JA: "…うわっ！"
        EVT_AT_SetVoice(voices[shenGetRandomInt32(4)], 0);
        rockyFall.state = 2;
        return;
    }
    case 2:
        if (rockyFall.releaseFrames < 1)
            SetQteButtonState(0xffff);
        --rockyFall.releaseFrames;
        if (!GetShouldRenderIcons(RenderIconSlot::Qte))
            rockyFall.state = ((GetCurrentQteStatus() & 0xfff) != config->qte) + 3;
        return;
    case 3: {
        figpAssignModeFor(ryo, 0);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        Vector3 position;
        if (rockyFall.direction) {
            Vector3 origin = rockyFall.savedPosition;
            origin.y += .5f;
            RaycastInWorldGrid(position, origin, .550000011920929f);
            position.y -= .5f;
        } else {
            Vector3 origin{rockyFall.intersection.x, rockyFall.savedPosition.y, rockyFall.intersection.z};
            Vector3i rotation{0, rockyFall.intersectionYaw, 0};
            position = {0, 0, .800000011920929f};
            scenApplyTransRot(&origin, &rotation, &position);
        }
        EV_AT_SetPos(ryo, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
        rockyFall.cooldown = 60;
        if (!rockyFall.direction || static_cast<std::uint32_t>(rockyFall.direction - 1) <= 1U) {
            if (rockyFall.direction) {
                QueueFixLeaf(30);
                QueueFixLeaf(25);
            }
            mtwkSetEnabled(scenFindMotionwork(ryo), 0);
        }
        EVT_AT_SetMot(ryo, config->recover, 0, 1, 0x40000000U, 0);
        rockyFall.state = 5;
        HLib::SetupTaskDelay(60);
        return;
    }
    case 4: {
        if (DoesScenarioCharacterExist(shenhua)) {
            EV_PTYCUtils(shenhua, 3, 1);
            Vector3 position;
            EV_GetCharaPosition(shenhua, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
            EventSetAngle angle{0, shenhua, shen_name{},
                static_cast<std::int16_t>(atan2_fixed16(rockyFall.startPosition.x - position.x, rockyFall.startPosition.z - position.z)) / 182.04445f, 0};
            EVT_AT_SetAngY_Callback(&angle);
            EVT_AT_SetMot(shenhua, 0xf0a5, 0, 1, 0x40000000U, 0);
            EVT_AT_SetMot(shenhua, 0x298c, 0, 1, 0, 15);
        }
        auto *player = ctrlGetScenarioChara(ryo);
        SetHighModelAndFaceFlagsForChara(player, 4, 10, 1);
        SetFacialMouthExpression(ctrlGetScenarioChara(ryo), 1, 10, 1);
        figpSetFlagsBlendForBlur(ryo, 5, 1);
        figpAssignModeFor(ryo, 1);
        EV_AT_SetPos(ryo, EventTransformFlags::World | EventTransformFlags::AllAxes, rockyFall.startPosition);
        EV_AT_SetAngY(ryo, EventTransformFlags::World | EventTransformFlags::AllAxes, rockyFall.targetRotation);
        AT_SetLocalBaseChara(dummy, shen_name{});
        EVT_AT_SetMot(ryo, config->fail, 0, .6000000238418579f, 0x40000000U, 0);
        EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("FALL"), 0);
        QueueFallCamera(config->failureCamera);
        EVT_AT_SetVoice("0100A080", 20); // @text EN: " " | JA: "うわぁぁぁーっ！"
        QueueFallVoice();
        EVT_AT_MenCtrl(EventFilmControl{1, 30, 0, 0, 0, 0, 255, 0, 0, 0, 100});
        EVT_AT_FadeBGM(0, 1, 130);
        if (rockyFall.direction == 1) {
            QueueSetBag(0, 100);
            auto *task = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 0xb, sizeof(StepObjectDisplayWork), eventToken);
            RegisterEventTask(task);
            *static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(task)) = {1, ryo, 1, 100};
        }
        QueueSetBag(1, 160);
        rockyFall.state = 6;
        HLib::SetupTaskDelay(160);
        return;
    }
    case 5:
        mtwkSetEnabled(scenFindMotionwork(ryo), 1);
        SetPlayerControlEnabled(1);
        FinishLoading();
        EV_SetCharaControllerFlags(ryo, 2, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(ryo, 3, NpcCharacterControllerState::Enabled);
        EV_EventSetUpHelpIcon_0(0x3f, 0);
        rockyFall.state = 0;
        return;
    case 6: {
        EV_scenQueueLoad(g_actNumber, g_currentMap, 0);
        std::int32_t level = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1662) != 0;
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1663))
            level = 2;
        if (level < 2)
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1662 + level, 1);
        SetSTEP(750000);
        return;
    }
    default:
        return;
    }
}
void RoomFallCallback(void *) {
    using enum CameraState;
    const auto *config = roomFall.configuration;
    switch (roomFall.state) {
    case 0: {
        if (!ryoNotBusy())
            return;
        bool distant = false;
        EV_GetCharaPosition(ryo, CHARACTER_TRANSLATION_ROOT, &roomFall.savedPosition, EventTransformFlags::World);
        GetScenarioCharacterEulerAngles(ryo, &roomFall.savedRotation, EventTransformFlags::World);
        GetScenarioCharacterEulerAngles(ryo, &roomFall.motionRotation, EventTransformFlags::World | EventTransformFlags::TurnMotion);
        Vector3 probe = roomFall.savedPosition;
        probe.y += .6000000238418579f;
        flddPushIntersectionFlag(3);
        if (IsWithinRadiusFromVector3PolyIntersectTest(probe, 1.2000000476837158f)) {
            TransformAgainstIntersectionResult(roomFall.intersection);
            Vector3s angles;
            transformIntersectionResult(angles);
            roomFall.intersectionPitch = angles.x;
            roomFall.intersectionYaw = angles.y;
            FindRoomFallTargets();
            flddToggleFlag(2);
            flddPushIntersectionFlag(1);
            Vector3 point;
            if (!FindNearestRayIntersection(&point, &roomFall.intersection, &probe)) {
                const float dx = roomFall.intersection.x - roomFall.savedPosition.x, dz = roomFall.intersection.z - roomFall.savedPosition.z;
                if (dx * dx + dz * dz >= .36000001430511475f)
                    distant = true;
                else if (roomFall.cooldown < 1) {
                    const std::int32_t delta =
                        static_cast<std::int16_t>(static_cast<std::uint16_t>(roomFall.motionRotation.y) - static_cast<std::uint16_t>(roomFall.intersectionYaw));
                    std::int32_t direction = 0, yaw = roomFall.intersectionYaw;
                    if (delta > 8192) {
                        direction = 1;
                        yaw += 0x4000;
                    } else if (delta <= -8192) {
                        direction = 2;
                        yaw -= 0x4000;
                    }
                    roomFall.direction = direction;
                    roomFall.targetRotation.y = yaw;
                    roomFall.configuration = &roomDirections[direction];
                    SetPlayerControlEnabled(0);
                    FallControlNullsub();
                    EV_SetRefBoardParamsFor(ryo, 1);
                    EV_SetCharaControllerFlags(ryo, 2, NpcCharacterControllerState::Disabled);
                    EV_SetCharaControllerFlags(ryo, 3, NpcCharacterControllerState::Disabled);
                    roomFall.state = 1;
                }
            }
        }
        --roomFall.cooldown;
        if (distant && static_cast<std::uint16_t>(static_cast<std::uint16_t>(roomFall.motionRotation.y) - roomFall.lookRotation.y + 0x1fff) <= 0x3ffeU) {
            GetBattleDataConfig(0, SHEN_NAME(BTLD));
            if (GetBattleDataConfig(2, SHEN_NAME(BTLD))) {
                SetPlayerControlEnabled(0);
                FallControlNullsub();
                EV_SetCharaControllerFlags(ryo, 2, NpcCharacterControllerState::Disabled);
                EV_SetCharaControllerFlags(ryo, 3, NpcCharacterControllerState::Disabled);
                roomFall.state = 5;
            }
        }
        return;
    }
    case 1:
        SetShowQteFlag(config->qte, static_cast<std::int16_t>(roomFall.qteFrames), 0);
        SetQteButtonState(static_cast<std::uint16_t>(0x2000U << roomFall.direction));
        roomFall.releaseFrames = 10;
        EV_AT_SetPos(ryo, EventTransformFlags::World | EventTransformFlags::AllAxes, roomFall.savedPosition);
        EV_AT_SetAngY(ryo, EventTransformFlags::World | EventTransformFlags::AllAxes, roomFall.targetRotation);
        AT_SetLocalBaseChara(dummy, shen_name{});
        EVT_AT_SetMot(ryo, config->begin, 0, 1, 0x40000000U, 0);
        EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("FALL"), 0);
        QueueFallCamera(config->camera);
        figpSetFlagsBlendForBlur(ryo, 5, 1);
        figpAssignModeFor(ryo, 1);
        roomFall.state = 2;
        return;
    case 2:
        if (roomFall.releaseFrames < 1)
            SetQteButtonState(0xffff);
        --roomFall.releaseFrames;
        if (!GetShouldRenderIcons(RenderIconSlot::Qte))
            roomFall.state = ((GetCurrentQteStatus() & 0xfff) != config->qte) + 3;
        return;
    case 3: {
        EV_SetRefBoardParamsFor(ryo, 0);
        figpAssignModeFor(ryo, 0);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        Vector3 position;
        if (roomFall.direction) {
            Vector3 origin = roomFall.savedPosition;
            origin.y += .6000000238418579f;
            RaycastInWorldGrid(position, origin, .6499999761581421f);
            position.y -= .6000000238418579f;
        } else {
            Vector3 origin{roomFall.intersection.x, roomFall.savedPosition.y, roomFall.intersection.z};
            Vector3i rotation{0, roomFall.intersectionYaw, 0};
            position = {0, 0, .8999999761581421f};
            scenApplyTransRot(&origin, &rotation, &position);
        }
        EV_AT_SetPos(ryo, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
        roomFall.cooldown = 60;
        if (!roomFall.direction || static_cast<std::uint32_t>(roomFall.direction - 1) <= 1U) {
            if (roomFall.direction) {
                QueueFixLeaf(30);
                QueueFixLeaf(25);
            }
            mtwkSetEnabled(scenFindMotionwork(ryo), 0);
        }
        EVT_AT_SetMot(ryo, config->recover, 0, 1, 0x40000000U, 0);
        roomFall.state = 9;
        HLib::SetupTaskDelay(60);
        return;
    }
    case 4: {
        EV_FreeAuthorPak(1, "0100");
        SetHighModelAndFaceFlagsForChara(ctrlGetScenarioChara(ryo), 4, 10, 1);
        SetFacialMouthExpression(ctrlGetScenarioChara(ryo), 1, 10, 1);
        figpSetFlagsBlendForBlur(ryo, 5, 1);
        figpAssignModeFor(ryo, 1);
        AT_SetLocalBaseChara(dummy, ryo);
        Vector3 position{0, -.6000000238418579f, -1.5f};
        scenApplyTransRot(&roomFall.intersection, &roomFall.exitRotation, &position);
        EV_AT_SetPos(ryo, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
        EV_AT_SetAngY(ryo, EventTransformFlags::World | EventTransformFlags::AllAxes, roomFall.exitRotation);
        AT_SetLocalBaseChara(dummy, shen_name{});
        EVT_AT_SetMot(ryo, config->fail, 0, .699999988079071f, 0x40000000U, 0);
        EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("FALL"), 0);
        QueueFallCamera(config->failureCamera);
        EVT_AT_SetVoice("0100A080", 20); // @text EN: " " | JA: "うわぁぁぁーっ！"
        EVT_AT_MenCtrl(EventFilmControl{1, 30, 0, 0, 0, 0, 255, 0, 0, 0, 40});
        roomFall.state = 10;
        HLib::SetupTaskDelay(100);
        return;
    }
    case 5:
        EVT_AT_SetMot(ryo, 0xf01b, 0, 1, 0x40000000U, 0);
        roomFall.state = 6;
        return;
    case 6:
        QueueFallLookCamera();
        roomFall.state = 7;
        HLib::SetupTaskDelay(70);
        return;
    case 7:
        camCtrlGetRot(&roomFall.cameraRotation);
        camCtrlGetNextCamPos(&roomFall.cameraEye);
        camCtrlGetTargetPosition(&roomFall.cameraTarget);
        camCtrlGetTargetPosition(&roomFall.filteredTarget);
        camCtrlToggleSmoothing(1);
        camCtrlSetMotionScaleFactor(1.5f);
        roomFall.state = 8;
        return;
    case 8:
        CheckHelpIcon(320, 0, CheckHelpIconOp::FrameOverlay);
        GetBattleDataConfig(0, SHEN_NAME(BTCC));
        if (GetBattleDataConfig(2, SHEN_NAME(BTCC))) {
            EVT_AT_SetMot(ryo, 0xf01a, 0, 1, 0x40000000U, 0);
            camCtrlToggleSmoothing(0);
            camCtrlSetFovTransitionTo(false);
            camCtrlSetState(Init);
            roomFall.state = 9;
            HLib::SetupTaskDelay(35);
        } else
            MoveFallLookTarget();
        return;
    case 9:
        mtwkSetEnabled(scenFindMotionwork(ryo), 1);
        SetPlayerControlEnabled(1);
        FinishLoading();
        EV_SetCharaControllerFlags(ryo, 2, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(ryo, 3, NpcCharacterControllerState::Enabled);
        AnchorDummyToPlayer();
        roomFall.state = 0;
        return;
    case 10:
        EV_scenQueueLoad(g_actNumber, roomExitScene, roomExitEntry);
        return;
    default:
        return;
    }
}
} // namespace
void QueueRockyAreaFallEvent() {
    EV_FreeAuthorPak(1, "0100");
    EV_FreeAuthorPak(1, "0540");
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::PackDirectory)),
        reinterpret_cast<std::uintptr_t>("FALL"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("FALL"), 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("FALL"), 0);
    LoadBlurModelForCharacter(ryo, "RYO_B.MT7", 1);
    auto *task = HLib::EnqueueTaskWithoutParameter(RockyFallCallback, 4, 0xb, static_cast<std::int32_t>(fallToken.name));
    rockyFall = {};
    rockyFall.task = task;
    rockyFall.placements = rockyPlacements;
    rockyFall.placementIndex = 12;
    std::int32_t level = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1662) != 0;
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1663))
        level = 2;
    rockyFall.qteFrames = 6 * (level + 3);
    AnchorDummyToPlayer();
}
void QueueRoomFallEvent() {
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::PackDirectory)),
        reinterpret_cast<std::uintptr_t>("FALL"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    LoadBlurModelForCharacter(ryo, "RYO_B.MT7", 1);
    auto *task = HLib::EnqueueTaskWithoutParameter(RoomFallCallback, 4, 0xb, static_cast<std::int32_t>(fallToken.name));
    roomFall = {};
    roomFall.task = task;
    roomFall.exits = g_currentMap.name == SHEN_NAME(QD00) ? roomExits : nullptr;
    roomFall.qteFrames = 18;
    AnchorDummyToPlayer();
}
void UpdateShenhuaOcclusionAlpha() {
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    auto *character = ctrlGetScenarioChara(shenhua);
    if (!character)
        return;
    SetFlagForToken(character, SHEN_NAME(MAPC), 0);
    Matrix4x4_Push(&g_cameraControl->projectionMatrix);
    Vector3 playerPosition, companionPosition;
    mtwkGetBonePosition(scenFindMotionwork(ryo), MB_Chest, &playerPosition);
    Matrix4x4::TransformPoint(playerPosition, playerPosition);
    mtwkGetBonePosition(scenFindMotionwork(shenhua), MB_Chest, &companionPosition);
    Matrix4x4::TransformPoint(companionPosition, companionPosition);
    character->alpha = std::min(1.0f, std::max(0.0f, character->alpha + (-playerPosition.z > -companionPosition.z ? -.1f : .1f)));
    Matrix4x4::Pop(1);
}
void ResetAreaFallEvents() {
    roomFall = {};
    rockyFall = {};
}
} // namespace shenmue::s2::areas
#endif
